/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' diCoords SOF: Start Of File
'   - has functions to get DI coordinates from a sam
'     file
'   o header
'     - included libraries
'   o fun01: get_diCoords
'     - gets start and ending coordinates for DI events
'   o fun02: scan_diCoords
'     - scans to see if DI events and returns the number
'       found (get_diCoords without memory allocation)
'   o fun03: pDIHead_diCoords:
'     - print out diCoords tsv header
'   o fun04: pDI_diCoords:
'     - print out a di entry to a file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../genBio/samEntry.h"

/*.h files only*/
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: get_diCoords
|   - gets start and ending coordinates for DI events
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with sequencer to scan
|   - minDILenUI:
|     o minimum deletion length to classify cigar entry as
|       a DI entry
|   - minEndNtUI:
|     o how many bases in a DI event must be to be a DI
|       event
|   - indelFlagSC:
|     o holds what type of indels targeting
|       - 1: dels
|       - 2: ins
|       - 3: del/ins
|   - diStartAryUI:
|     o pointer to unsigned long to hold the starting
|       coordinates of each DI event
|   - diEndAryUI:
|     o pointer to unsigned long to hold the ending
|       coordinates of each DI event
|   - delSizeAryUI:
|     o pointer to unsigned int array to hold the deletion
|       size of each DI event
|   - indelArySC:
|     o holds indels types found ("D" = del; "I" = ins)
|   - lenArysUIPtr:
|     o pointer to unsigned long to hold diStartAryUI and
|       diEndAryUI lengths
| Output:
|   - Modifies:
|     o diStartAryUI to have DI starting coordinates
|     o diEndAryUI to have DI ending coordinates
|     o lenArysUIPtr to have new array lengths if
|       diStartAryUI and diEndAryUI are changed
|   - Returns:
|     o number of DI events detected
|     o < 0 for memory errors
\-------------------------------------------------------*/
signed int
get_diCoords(
   struct samEntry *samSTPtr,  /*sam entry to scan*/
   unsigned int minDILenUI,    /*min del length for DI*/
   unsigned int minEndNtUI,    /*max NT at ends*/
   signed char indelFlagSC,    /*holds indel type*/
   unsigned int **diStartAryUI,/*gets start coordinates*/
   unsigned int **diEndAryUI,  /*gets DI end coordinates*/
   unsigned int **delSizeAryUI,/*gets DI deletion sizes*/
   signed char **indelArySC,   /*indel types found*/
   unsigned int *lenArysUIPtr  /*current arrays lengths*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - gets start and ending coordinates for DI events
   '   o fun01 sec01:
   '     - variable declerations
   '   o fun01 sec02:
   '     - allocate memory
   '   o fun01 sec03:
   '     - scan for DI coordinates
   '   o fun01 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int numDISI = 0;
   unsigned int coordUI = 0;
   unsigned int uiCig = 0;
   unsigned int curNonDIUI = 0;
   unsigned int startUI = 0;    /*start of DI event*/

   unsigned int stopScanUI =
      samSTPtr->readLenUI - minEndNtUI;
      /*point in query to stop scaning for DI's*/
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         ! *diStartAryUI
      || ! *diEndAryUI
      || ! *indelArySC
      || *lenArysUIPtr < samSTPtr->cigLenUI
   ){ /*If: need to resize the arrays*/
      *lenArysUIPtr = samSTPtr->cigLenUI;

      if(*diStartAryUI)
         free(*diStartAryUI);
      *diStartAryUI = 0;
      *diStartAryUI =
         malloc(*lenArysUIPtr * sizeof(unsigned int));
      if(! *diStartAryUI)
         goto memErr_sec04;


      if(*diEndAryUI)
         free(*diEndAryUI);
      *diEndAryUI = 0;
      *diEndAryUI =
         malloc(*lenArysUIPtr * sizeof(unsigned int));
      if(! *diEndAryUI)
         goto memErr_sec04;


      if(*delSizeAryUI)
         free(*delSizeAryUI);
      *delSizeAryUI = 0;
      *delSizeAryUI =
         malloc(*lenArysUIPtr * sizeof(unsigned int));
      if(! *delSizeAryUI)
         goto memErr_sec04;


      if(*indelArySC)
         free(*indelArySC);
     *indelArySC = 0;
     *indelArySC =
        malloc(*lenArysUIPtr * sizeof(signed char));
     if(! *indelArySC)
        goto memErr_sec04;
   } /*If: need to resize the arrays*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - scan for DI coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->cigTypeStr[samSTPtr->cigLenUI - 1] == 'S')
      stopScanUI -=
         samSTPtr->cigArySI[samSTPtr->cigLenUI - 1];
         /*account for softmasking at end*/

   if(samSTPtr->cigTypeStr[0] == 'S')
      stopScanUI -= samSTPtr->cigArySI[0];
      /*account for softmasking at start*/
      
   coordUI = samSTPtr->refStartUI;


   for(
      uiCig = 0;
      uiCig < samSTPtr->cigLenUI;
      ++uiCig
   ){ /*Loop: find di events*/ 

      if(samSTPtr->cigTypeStr[uiCig] == 'S')
         continue;

      else if(samSTPtr->cigTypeStr[uiCig] == 'H')
         continue;

      else if( samSTPtr->cigTypeStr[uiCig] == 'D' )
      { /*If: have deletion*/
         startUI = coordUI; /*start of deletion*/
         coordUI += samSTPtr->cigArySI[uiCig];

         if(! (indelFlagSC & 1) )
            continue; /*ignoring deletions*/
      } /*If: have deletion*/
  
      else if(samSTPtr->cigTypeStr[uiCig] == 'I')
      { /*If: have insertion*/
         startUI = coordUI;

         if(curNonDIUI < minEndNtUI)
         { /*If: to early for recording DI*/
            curNonDIUI += samSTPtr->cigArySI[uiCig];
            continue; /*ignoring insertions*/
         } /*If: to early for recording DI*/

         curNonDIUI += samSTPtr->cigArySI[uiCig];

         if(! (indelFlagSC & 2) )
            continue; /*ignoring insertions*/
      } /*If: have insertion*/

      else
      { /*If: not indel*/
         curNonDIUI += samSTPtr->cigArySI[uiCig];
         coordUI += samSTPtr->cigArySI[uiCig];
         continue;
      } /*If: not indel*/


      if(curNonDIUI >= stopScanUI)
         break; /*finished*/

      else if(curNonDIUI < minEndNtUI)
         continue; /*to early for DI event*/

      else if(
           samSTPtr->cigArySI[uiCig]
         < (signed int) minDILenUI
      ) continue; /*DI event to small*/


      (*delSizeAryUI)[numDISI]= samSTPtr->cigArySI[uiCig];
      (*indelArySC)[numDISI]= samSTPtr->cigTypeStr[uiCig];
      (*diStartAryUI)[numDISI] = startUI;/*first DI base*/

      if(samSTPtr->cigTypeStr[uiCig] == 'D')
         (*diEndAryUI)[numDISI] = coordUI - 1;/*end del*/
      else
         (*diEndAryUI)[numDISI] = coordUI + 1;
         /*base after insertion*/

      ++numDISI; /*number DI events*/

   } /*Loop: find di events*/ 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cleanUp_sec04;

   memErr_sec04:;
      numDISI = -1;
      goto cleanUp_sec04;

   cleanUp_sec04:;
      return(numDISI);
} /*get_diCoords*/

/*-------------------------------------------------------\
| Fun02: scan_diCoords
|   - scans to see if DI events and returns the number
|     found (get_diCoords without memory allocation)
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with sequencer to scan
|   - minDILenUI:
|     o minimum deletion length to classify cigar entry as
|       a DI entry
|   - minEndNtUI:
|     o how many bases in a DI event must be to be a DI
|       event
| Output:
|   - Returns:
|     o number of DI events detected
|     o < 0 for memory errors
\-------------------------------------------------------*/
signed int
scan_diCoords(
   struct samEntry *samSTPtr,  /*sam entry to scan*/
   unsigned int minDILenUI,    /*min del length for DI*/
   unsigned int minEndNtUI     /*max NT at ends*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - gets start and ending coordinates for DI events
   '   o fun02 sec02:
   '     - variable declerations
   '   o fun02 sec02:
   '     - scan for DI coordinates
   '   o fun02 sec03:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int numDISI = 0;
   unsigned int coordUI = 0;
   unsigned int uiCig = 0;

   unsigned int stopScanUI =
      samSTPtr->readLenUI - minEndNtUI;
      /*point in query to stop scaning for DI's*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - scan for DI coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->cigTypeStr[samSTPtr->cigLenUI - 1] == 'S')
      stopScanUI -=
         samSTPtr->cigArySI[samSTPtr->cigLenUI - 1];
         /*account for softmasking at end*/

   if(samSTPtr->cigTypeStr[0] == 'S')
      stopScanUI -= samSTPtr->cigArySI[0];
      /*account for softmasking at start*/

   coordUI = samSTPtr->refStartUI;

   for(
      uiCig = 0;
      uiCig < samSTPtr->cigLenUI;
      ++uiCig
   ){ /*Loop: find di events*/ 
      if(samSTPtr->cigTypeStr[uiCig] == 'S')
         continue; /*soft masked, do not care*/

      else if(samSTPtr->cigTypeStr[uiCig] != 'D')
      { /*Else If: not a deletion*/
         coordUI += samSTPtr->cigArySI[uiCig];
         continue;
      } /*Else If: not a deletion*/

      else if(
           samSTPtr->cigArySI[uiCig]
         < (signed int) minDILenUI
      ){ /*Else If: deletion is to small*/
         coordUI += samSTPtr->cigArySI[uiCig];
         continue; /*if not a deletion*/
      }  /*Else If: deletion is to small*/

      else if(coordUI >= stopScanUI)
         break; /*finished (end of query scan area)*/

      ++numDISI; /*number DI events*/
   } /*Loop: find di events*/ 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return(numDISI);
} /*scan_diCoords*/

/*-------------------------------------------------------\
| Fun03: pDIHead_diCoords:
|   - print out diCoords tsv header
| Input:
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o header for pDI_diCoords to outFILE
\-------------------------------------------------------*/
void
pDIHead_diCoords(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "read\tref\tstart_di\tend_di\tnum_del\ttype"
   );

   fprintf(
      (FILE *) outFILE,
      "\tref_start\tref_end\tnum_di%s",
      str_endLine
   );
} /*pDIHead_diCoords*/

/*-------------------------------------------------------\
| Fun04: pDI_diCoords:
|   - print out a di entry to a file
| Input:
|   - qryIdStr:
|     o c-string with query id to print out
|   - refIdStr:
|     o c-string with reference id to print out
|   - diStartAryUI:
|     o unsigned int array with starting coordinates
|       for each DI event (get_diCoords fun01)
|   - diEndAryUI:
|     o unsigned int array with ending coordinates
|       for each DI event (get_diCoords fun01)
|   - delSizeAryUI:
|     o unsigned int array with the deletion size for each
|       DI event
|   - indelArySC:
|     o holds indels types found ("D" = del; "I" = ins)
|   - numDIsSI:
|     o number of DI events in diStartAryUI/diEndAryUI
|   - refStartUI:
|     o frist reference base in alignment
|   - refEndUI:
|     o last reference base in alignment
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o read id, number of DI events, and the coordinates
|       for each event as a tsv to outFILE
\-------------------------------------------------------*/
void
pDI_diCoords(
   signed char *qryIdStr,      /*read id*/
   signed char *refIdStr,      /*reference id*/
   unsigned int *diStartAryUI, /*starting coordiantes*/
   unsigned int *diEndAryUI,   /*ending coordiantes*/
   unsigned int *delSizeAryUI, /*size of each deletion*/
   signed char *indelArySC,    /*indel types found*/
   signed int numDIsSI,        /*number of DI events*/
   unsigned int refStartUI,    /*first mapped ref base*/
   unsigned int refEndUI,      /*last mapped ref base*/
   void *outFILE               /*filt to print to*/
){
   signed int siDI = 0;

   for(
      siDI = 0;
      siDI < numDIsSI;
      ++siDI
   ){ /*Loop: print out coordinates*/
      fprintf(
         (FILE *) outFILE,
         "%s\t%s\t%u\t%u\t%u\t%c\t%u\t%u\t%i%s",
         qryIdStr,
         refIdStr,
         diStartAryUI[siDI] + 1,
         diEndAryUI[siDI] + 1,
         delSizeAryUI[siDI],
         indelArySC[siDI],
         refStartUI + 1,
         refEndUI + 1,
         numDIsSI,
         str_endLine
      );
   } /*Loop: print out coordinates*/

   if(! siDI)
   { /*If: no DI events*/
      fprintf(
         (FILE *) outFILE,
         "%s\t%s\tNA\tNA\tNA\tNA\tNA\tNA\t0%s",
         qryIdStr,
         refIdStr,
         str_endLine
      );
   } /*If: no DI events*/
} /*pDI_diCoords*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
