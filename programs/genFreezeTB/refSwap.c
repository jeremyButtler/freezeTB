/*########################################################
# Name: refSwap
#   - has functions to swap the reference for an tbAmr
#     database
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF:
'   o header:
'     - included libraries
'   o fun01: cnvtCoords_refSwap
'     - converts an set of coordinates from one reference
'       to another reference
'   o fun02: swapAmrAryRef_refSwap
'     - swaps the reference coordinates for an array of
'       amr structures
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
#endif

#include "refSwap.h"

#include "../genBio/samEntry.h"
#include "amrST.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - std #include <stdio.h>
!   - std #include <stdlib.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "drugAry.h"
!   - .h  #include "../genLib/endLine.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: cnvtCoords_refSwap
|   - converts an set of coordinates from one reference
|     to another reference
| Input:
|   - samSTPtr:
|     o pointer to an samEntry structure with the new
|       reference mapped to the old reference
|   - posArySI:
|     o unsigned int array with the current position
|       in the sam entry struct
|     o posAryUI[0] = current cigar entry on
|     o posAryUI[1] = bases not checked in current cigar
|       entry
|     o posAryUI[2] = reference position at currently
|     o posAryUI[3] = sequence (new ref) position at
|       currently
|   - refPosSI:
|     o unsigned int with position to change
| Output:
|   - Modifies:
|     o newCoordsAryUI to hold the new start coordinate
|       (index 0) and new ending coordinate (index 1)
|     o posAryUI to be moved to the start of the current
|       entry
|   - Returns
|     o new coordinate
|     o -1 if the coordinate could not be found
\-------------------------------------------------------*/
signed int
cnvtCoords_refSwap(
   struct samEntry *samSTPtr,
   signed int posArySI[],
   signed int refStartSI
){
   /*If: this is the first time*/
   if(
         posArySI[0] == 0
      && posArySI[1] == 0
   ) posArySI[1] = samSTPtr->cigArySI[posArySI[0]];

   if(posArySI[2] >= (signed int) samSTPtr->refEndUI)
      goto outOfBounds_fun01_sec05;

   else if(
      posArySI[3] >= (signed int) samSTPtr->readLenUI
   ) goto outOfBounds_fun01_sec05;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[0],  /*cigar entry*/
      &posArySI[1],  /*number bases left in cigar entry*/
      refStartSI,    /*start of target*/
      &posArySI[2],  /*reference position*/
      &posArySI[3]   /*sequence position*/
   );

   return posArySI[3];

   outOfBounds_fun01_sec05:;

   return -1;
} /*cnvtCoords_refSwap*/

/*-------------------------------------------------------\
| Fun02: swapAmrAryRef_refSwap
|   - swaps the reference coordinates for an array of
|     amr structures
| Input:
|   - amrAryST
|     o array of amrStructs to swap reference coordinates
|       with
|   - numAmrUI:
|     o number of amrStructs in amrAryST
|   - samSTPtr:
|     o pointer to an samEntry structure with the new
|       reference mapped to the old reference
| Output:
|   - Modifies:
|     o coordinates in amrStructs
|   - Returns:
|     o the number of amrStructs that could be converted
\-------------------------------------------------------*/
unsigned int
swapAmrAryRef_refSwap(
   struct amrST *amrAryST,
   unsigned int numAmrUI,
   struct samEntry *samSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - variable declerations
   '   o fun02 sec02:
   '     - convert amr coordinates
   '   o fun02 sec03:
   '     - remove uncoverted structures at start
   '   o fun02 sec04:
   '     - return the number of amrs converted
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int ntPosArySI[4];
   signed int aaPosArySI[4];

   signed int genePosArySI[4];
   signed int geneStartSI = 0;
   signed int geneEndSI = 0;

   signed int newCoordSI = 0;
   unsigned int uiAmr = 0;
   signed char startBl = 1;
   unsigned int firstCnvtAmrUI = 0;

   for(
      uiAmr = 0;
      uiAmr < 4;
      ++uiAmr
   ){ /*Loop: initialize arrays*/
      ntPosArySI[uiAmr] = 0;
      aaPosArySI[uiAmr] = 0;
      genePosArySI[uiAmr] = 0;
   } /*Loop: initialize arrays*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - convert amr coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uiAmr = 0;

   while(uiAmr < numAmrUI)
   { /*Loop: convert amr coordinates*/ 
      if(
                 amrAryST[uiAmr].geneFirstRefUI
            != (unsigned int) geneStartSI
         ||
                amrAryST[uiAmr].geneLastRefUI
            != (unsigned int) geneEndSI
      ){ /*If: I am changing genes*/
         geneStartSI =
            cnvtCoords_refSwap(
               samSTPtr,
               genePosArySI,
               amrAryST[uiAmr].geneFirstRefUI
            );

         if(geneStartSI < 0)
            geneStartSI = samSTPtr->readLenUI;

         geneEndSI =
            cnvtCoords_refSwap(
               samSTPtr,
               genePosArySI,
               amrAryST[uiAmr].geneLastRefUI
            );

         if(geneEndSI < 0)
            geneEndSI = samSTPtr->readLenUI;
      } /*If: I am changing genes*/

      newCoordSI =
         cnvtCoords_refSwap(
            samSTPtr,
            ntPosArySI,
            amrAryST[uiAmr].refPosUI
         );

      if(newCoordSI < 0)
      { /*If: I could not find the coordinates*/
         if(! startBl)
         { /*If: I am at the end of the reference*/
            numAmrUI = uiAmr - 1;
            break;
            /*at this point no other coordinates should
              work
            */
         } /*If: I am at the end of the reference*/

         /*count the number of amr structures I could
         `  not convert
         */
         ++firstCnvtAmrUI;
      } /*If: I could not find the coordinates*/

      amrAryST[uiAmr].refPosUI =
         (unsigned int) newCoordSI;

      if(amrAryST[uiAmr].codonPosUI > 0)
      { /*If: I have an codon position*/
         newCoordSI =
            cnvtCoords_refSwap(
               samSTPtr,
               aaPosArySI,
               amrAryST[uiAmr].codonPosUI
            );

         /*move problematic structures to the end*/
         if(newCoordSI < 0)
         { /*If: I could not find the coordinates*/
            numAmrUI = uiAmr - 1;
            break;
         } /*If: I could not find the coordinates*/

         amrAryST[uiAmr].codonPosUI =
            (unsigned int) newCoordSI;
      } /*If: I have an codon position*/

      amrAryST[uiAmr].geneFirstRefUI = geneStartSI;
      amrAryST[uiAmr].geneLastRefUI = geneEndSI;

      startBl = 0;
      ++uiAmr;
   } /*Loop: convert amr coordinates*/ 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - remove uncoverted structures at start
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uiAmr = 0;

   while(uiAmr < firstCnvtAmrUI)
   { /*Loop: remove the uncoverted AMRs at start*/
      swap_amrST(
         &amrAryST[uiAmr],
         &amrAryST[numAmrUI]
      );

      --numAmrUI;
      ++uiAmr;
   } /*Loop: remove the uncoverted AMRs at start*/

   if(firstCnvtAmrUI > 0)
   { /*If: I need to resort the array*/
      sortPos_amrST(
         amrAryST,
         0,
         numAmrUI
      );
   } /*If: I need to resort the array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return the number of amrs converted
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return numAmrUI;
} /*swapAmrAryRef_refSwap*/

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
