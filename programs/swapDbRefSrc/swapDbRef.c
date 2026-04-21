/*########################################################
# Name: swapDbRef
#   - swaps reference coordinates in an tbAmr database
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: pversion_swapDbRef
'     - prints version number to outFILE
'   o fun02: phelp_swapDbRef
'     - prints the help message to outFILE
'   o fun03: getInput_swapDbRef
'     - gets user input for swapDbRef
'   o main:
'     - driver function
'   o license:
'     - licensing is cc0
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

#include "../genLib/charCp.h"
#include "../genBio/samEntry.h"
#include "../genFreezeTB/amrST.h"
#include "../genFreezeTB/refSwap.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../ftbVersion.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genFreezeTB/drugAry.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_swapDbRef
|   - prints version number to outFILE
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_swapDbRef(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "swapDbRef from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_swapDbRef*/

/*-------------------------------------------------------\
| Fun02: phelp_swapDbRef
|   - prints help message to outFILE
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_swapDbRef(
   void *outFILE
){
   fprintf(
     (FILE* ) outFILE,
     "swapDbRef -sam ref.sam -amr amr-database.tsv\n"
   );

   fprintf(
     outFILE,
     "  - Change coordinates in an tbAmr database\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "Input:\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "  -amr amr-database.tsv: [Required]\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "    o amr databse to change coordinates for\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "  -sam ref.sam: [stdin]\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "    o sam file with new reference mapped to old\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "      reference\n"
   );

   fprintf(
     (FILE* ) outFILE,
     "Output:\n "
   );

   fprintf(
     (FILE* ) outFILE,
      "  - Prints database with converted coordinates\n"
   );

   fprintf(
     (FILE* ) outFILE,
      "    to outFILE\n"
   );
} /*phelp_swapDbRef*/

/*-------------------------------------------------------\
| Fun03: getInput_swapDbRef
|   - gets user input for swapDbRef
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argsAryStr:
|     o array of c-strings with user input
|   - dbFileStr:
|     o pointer to c-string be set to the dbFile c-string
|   - samFileStr:
|     o pointer to c-string be set to the sam c-string
| Output:
|   - Modifies:
|     o dbFileStr to hold the AMR database to convert
|     o samFileStr to hold the mapping coordinates for
|       the reference to convert
|   - Returns:
|     o 0 for no problems
|     o 1 for printed help message or version number
|     o 2 for invalid input
\-------------------------------------------------------*/
char
getInput_swapDbRef(
   signed int numArgsSI,
   char *argsAryStr[],
   signed char **dbFileStr,
   signed char **samFileStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - return errors/no error
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI < 2)
   { /*If: no arguments were input*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
   } /*If: no arguments were input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - get user input
   ^   o fun03 sec03 sub02:
   ^     - check for help message requests
   ^   o fun03 sec03 sub03:
   ^     - check for version number requests
   ^   o fun03 sec03 sub04:
   ^     - invalid input
   ^   o fun03 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - get user input
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (signed char *) "-amr",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: this is the amr database file*/
         ++siArg;
         *dbFileStr = (signed char *) argsAryStr[siArg];
      } /*If: this is the amr database file*/

      else if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: this is the reference sam file*/
         ++siArg;
         *samFileStr = (signed char *) argsAryStr[siArg];
      } /*Else If: this is the reference sam file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - check for help message requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - check for version number requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argsAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not reconginized\n",
           argsAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return errors/no error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;
   
   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
   errSC = 1;
   goto ret_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;
   return errSC;
} /*getInput_swapDbRef*/

/*-------------------------------------------------------\
| Main:
|   - driver function
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize and get user input
   '   o main sec03:
   '     - read in input files
   '   o main sec04:
   '     - convert the amr database coordinates
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *amrDbFileStr = 0;
   signed char *samFileStr = 0;

   signed char errSC = 0;

   struct samEntry samStackST;
   FILE *samFILE = 0;

   struct amrST *amrHeapAryST = 0;
   unsigned int numAmrsUI = 0;
   unsigned int numCnvtAmrsUI = 0;

   signed char *drugHeapAryStr = 0;
   unsigned int numDrugsUI = 0;
   unsigned int maxDrugsUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and get user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStackST);

   errSC =
      getInput_swapDbRef(
         numArgsSI,
         argsAryStr,
         &amrDbFileStr,
         &samFileStr
   );

   if(errSC)
   { /*If: I had an error*/
      --errSC; /*help message is 1, so goes to 0*/
      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );

      goto memErr_main_sec05;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in input files
   ^   o main sec05 sub01:
   ^     - read in the amr database
   ^   o main sec05 sub02:
   ^     - read in the sam file with mapped references
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - read in the amr database
   \*****************************************************/

   amrHeapAryST =
      readTbl_amrST(
         amrDbFileStr,
         &numAmrsUI,
         &drugHeapAryStr,
         (signed int *) &numDrugsUI,
         (signed int *) &maxDrugsUI,
         &errSC
      );

   if(! amrHeapAryST)
   { /*If: I had an error*/
      if(errSC == def_memErr_amrST)
      { /*If: had a memory error*/
         fprintf(
            stderr,
            "mem error reading AMR database (-amr %s)\n",
            amrDbFileStr
         );

         goto memErr_main_sec05;
      } /*If: had a memory error*/

      fprintf(
         stderr,
         "could not opem AMR database (-amr %s)\n",
         amrDbFileStr
      );

      goto fileErr_main_sec05;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - read in the sam file with mapped references
   *   o main sec05 sub02 cat01:
   *     - open the sam file
   *   o main sec05 sub02 cat02:
   *     - get the reference from the sam file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat01:
   +   - open the sam file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! samFileStr || samFileStr[0] == '-')
      samFILE = stdin;

   else
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

   if(! samFILE)
   { /*If: could not open the sam file*/
      fprintf(
         stderr,
         "could not open -sam %s\n",
         samFileStr
      );

      errSC = -2;
      goto fileErr_main_sec05;
   } /*If: could open the sam file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat02:
   +   - get the reference from the sam file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   do{ /*Loop: read in reference sequence in sam file*/
      errSC = get_samEntry(&samStackST, samFILE);

      if(errSC > 1)
      { /*If: I had an error*/
         fprintf(
            stderr,
            "memory error reading sam file (-sam %s)\n",
            samFileStr
         );

         goto memErr_main_sec05;
      } /*If: I had an error*/
   } while(*samStackST.extraStr == '@');
     /*Loop: read in reference sequence in sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - convert the amr database coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numCnvtAmrsUI =
      swapAmrAryRef_refSwap(
         amrHeapAryST,
         numAmrsUI,
         &samStackST
      );

   errSC =
      p_amrST(
         amrHeapAryST,
         numCnvtAmrsUI,
         drugHeapAryStr,
         numDrugsUI - 1,
         (signed char *) "-"    /*print to stdout*/
      ); /*print the converted database*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;         /*no errors*/
   goto cleanUp_main_sec05;
   
   memErr_main_sec05:;
      errSC = 1;
      goto cleanUp_main_sec05;

   fileErr_main_sec05:;
      errSC = 2;
      goto cleanUp_main_sec05;

   cleanUp_main_sec05:;
      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else if(samFILE == stderr) ;
      else
         fclose(samFILE);
      samFILE = 0;

      if(drugHeapAryStr)
         free(drugHeapAryStr);
      drugHeapAryStr = 0;

      freeStack_samEntry(&samStackST);
      freeHeapAry_amrST(amrHeapAryST, numAmrsUI);
      amrHeapAryST = 0;

      return (int) errSC;
} /*main*/

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
