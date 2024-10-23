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
'     - licensing for this code (public domain / mit)
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
#include "../tbAmrSrc/amrST.h"
#include "../genLib/charCp.h"

#include "refSwap.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../tbAmr/drugAry.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_year_swapDbRef 2024
#define def_month_swapDbRef 8
#define def_day_swapDbRef 14

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
      "swapDbRef version: %i-%02i-%02i\n",
      def_year_swapDbRef,
      def_month_swapDbRef,
      def_day_swapDbRef
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

   sint siArg = 1;
   schar errSC = 0;

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
            (schar *) "-amr",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: this is the amr database file*/
         ++siArg;
         *dbFileStr = (schar *) argsAryStr[siArg];
      } /*If: this is the amr database file*/

      else if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: this is the reference sam file*/
         ++siArg;
         *samFileStr = (schar *) argsAryStr[siArg];
      } /*Else If: this is the reference sam file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - check for help message requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: user asked for help*/
         phelp_swapDbRef(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user asked for help*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
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
            (schar *) "-v",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number request*/
         pversion_swapDbRef(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argsAryStr[siArg],
            (schar) '\0'
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

   schar *amrDbFileStr = 0;
   schar *samFileStr = 0;

   schar errSC = 0;

   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   FILE *samFILE = 0;

   struct amrST *amrHeapAryST = 0;
   uint numAmrsUI = 0;
   uint numCnvtAmrsUI = 0;

   schar *drugHeapAryStr = 0;
   uint numDrugsUI = 0;
   uint maxDrugsUI = 0;

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
         (sint *) &numDrugsUI,
         (sint *) &maxDrugsUI,
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
      errSC =
        get_samEntry(
           &samStackST,
           &buffHeapStr,
           &lenBuffUL,
           samFILE
      );

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
         (schar *) "-"    /*print to stdout*/
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

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(buffHeapStr)
     free(buffHeapStr);

   buffHeapStr = 0;

   if(drugHeapAryStr)
      free(drugHeapAryStr);

   drugHeapAryStr = 0;

   freeStack_samEntry(&samStackST);

   freeHeapAry_amrST(
      amrHeapAryST,
      numAmrsUI
   );

   amrHeapAryST = 0;

   return (int) errSC;
} /*main*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
