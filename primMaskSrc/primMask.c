/*########################################################
# Name: primMask
#   - mask primers in an sequence using input coordaintes
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and defined variables
'   o main:
'     - driver function to mask primers
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"
#include "primMask-fun.h"
#include "primMask-input.h"

/*No .c files (only .h)*/
#include "../generalLib/dataTypeShortHand.h"
#include "primMask-version.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .h #include "../generalLib/base10StrToNum.h"
!   o .h #include "../generalLIb/numToStr.h"
!   o .h #include "../generalLib/gen-shellSort.h"
!   o .h #include "../generalLib/ulCpStr.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Main:
|   - driver function to mask primers
| Input:
|   - numArgsSI:
|     o Number of arguments the user input
|   - argAryStr:
|     o Pointer to an array of c-strings with user
|       arguments
| Output:
|   - Prints:
|     o sam file with masked sequenes to output file
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   signed int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to mask primers
   '   o main sec-01:
   '     - Variable declerations
   '   o main sec-02:
   '     - Get and check user input
   '   o main sec-03:
   '     - Read in the primer coordinates
   '   o main sec-04:
   '     - Mask primers
   '   o main sec-05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *samFileStr = 0;
   schar *primFileStr = 0;
   schar *outFileStr = 0;
   schar maskSC = def_mask_primMask;
   sint fudgeSI = def_fudge_primMask;

   schar filterBl = def_filter_primMask;
     /*Remove reads with no primers*/

   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   uint *startAryHeapUI = 0;
   uint *endAryHeapUI = 0;
   uint *flagAryHeapUI = 0;
   sint numPrimSI = 0;

   struct samEntry samStackST;
   schar firstReadBl =0;/*marks if print primMask header*/

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*For error reporting*/
   slong errSL = 0;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Get and check user input
   ^   o main sec-02 sub-01:
   ^     - Get (process) the user input
   ^   o main sec-02 sub-02:
   ^     - Check can open the primer file
   ^   o main sec-02 sub-03:
   ^     - Check can open the output file
   ^   o main sec-02 sub-04:
   ^     - Check if can open the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-02 Sub-01:
   *   - Get (process) the user input
   \*****************************************************/

   initSamEntry(&samStackST);

   if(numArgsSI < 2)
   { /*If: Nothing was input*/
      pHelpPrimMask(stderr);
      fprintf(stdout, "Nothing input\n");

      goto errCleanUp_main_sec05_sub02;
   } /*If: Nothing was input*/

   errSL =
      getInputPrimMask(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &primFileStr,
         &outFileStr,
         &maskSC,
         &fudgeSI,
         &filterBl
      );

   if(errSL)
   { /*If: There was an error in the user input*/
      if(errSL & def_help_primMask)
      { /*If: The user wanted the help message*/
         pHelpPrimMask(stdout);
         goto cleanUp_main_sec05_sub01;
      } /*If: The user wanted the help message*/

      if(errSL & def_version_primMask)
      { /*If: The user wanted the version number*/
         fprintf(
            stdout,
            "primMask version %u-%02u-%02u\n",
            def_year_primMask,
            def_month_primMask,
            def_day_primMask
         );

         goto cleanUp_main_sec05_sub01;
      } /*If: The user wanted the version number*/

      pHelpPrimMask(stderr);

      if(errSL & def_nonNumeric_primMask)
      { /*If: There was an non-numeric error*/
         fprintf(
            stdout,
            "%s %s is not numeric\n",
            argAryStr[errSL >> 8],
            argAryStr[(errSL >> 8) + 1]
         );

         goto errCleanUp_main_sec05_sub02;
      } /*If: There was an non-numeric error*/

      fprintf(
         stdout,
         "%s is not recognized\n",
         argAryStr[errSL >> 8]
      );

      goto errCleanUp_main_sec05_sub02;
   } /*If: There was an error in the user input*/

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Check can open the primer file
   \*****************************************************/

   outFILE = fopen((char *) primFileStr, "r");
   
   if(! outFILE)
   { /*If: I could not open the primer coordinates file*/
      fprintf(
         stderr,
         "Unable to open -prim %s\n",
         primFileStr
      );

      goto errCleanUp_main_sec05_sub02;
   } /*If: I could not open the primer coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Check can open the output file
   \*****************************************************/

   if(! outFileStr  || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: The user supplied an output file*/
      outFILE = fopen((char *) outFileStr, "w");
      
      if(! outFILE)
      { /*If: I could not open primer coordinates file*/
         fprintf(
            stderr,
            "Unable to open -out %s\n",
            outFileStr
         );

         goto errCleanUp_main_sec05_sub02;
      } /*If: I could not open primer coordinates file*/
   } /*Else: The user supplied an output file*/

   /*****************************************************\
   * Main Sec-02 Sub-04:
   *   - Check can open the sam file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
      samFILE = stdin;
   else
   { /*Else: The sam file was provided*/
      samFILE = fopen((char *) samFileStr, "r");

      if(! samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
            stderr,
            "Unable to open -sam %s\n",
            samFileStr
         );

         goto errCleanUp_main_sec05_sub02;
      } /*If: I could not open the sam file*/
   } /*Else: The sam file was provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Read in the primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numPrimSI =
      readPrimCoords(
         primFileStr,
         &startAryHeapUI,
         &endAryHeapUI,
         &flagAryHeapUI,
         &errSL
      ); /*Get the primer list*/

   if(! numPrimSI)
   { /*If: I had an error*/
      if(errSL & def_emptyFileErr_primMask)
      { /*If: I had an empty file*/
         fprintf(
            stderr,
            "-prim %s is emtpy\n",
            primFileStr
         );
      } /*If: I had an empty file*/

      else if(errSL & def_emptyFileErr_primMask)
      { /*Else If: I had an invalid line*/
         fprintf(
            stderr,
            "line number %lu in -prim %s is invalid\n",
            (errSL >> 8),
            primFileStr
         );
      } /*Else If: I had an invalid line*/
      
      else
      { /*Else: I had an memory error*/
         fprintf(
            stderr,
            "Memory error reading %s\n",
            primFileStr
         );

      } /*Else: I had an memory error*/

      goto errCleanUp_main_sec05_sub02;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Mask primers
   ^   o main sec-04 sub-01:
   ^     - read in the first line
   ^   o main sec-04 sub-02:
   ^     - print out any comments/header
   ^   o main sec-04 sub-03:
   ^     - make and print the primMask header
   ^   o main sec-04 sub-04:
   ^     - mask primers and print entry
   ^   o main sec-04 sub-05:
   ^     - get the next entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-04 Sub-01:
   *  - read in the first line and start loop
   \*****************************************************/

   errSC =
      (schar)
      readSamLine(
        &samStackST,
        (char **) &buffHeapStr,
        &lenBuffUL,
        samFILE
      ); /*Get the first line in the sam file*/

   while(! errSC)
   { /*Loop: Read in all lines and mask primers*/

      /**************************************************\
      * Main Sec-04 Sub-02:
      *  - print out any comments/header
      \**************************************************/

      if(*samStackST.extraStr == '@')
      { /*If: this is an comment; print out*/
         pSamEntry(
            &samStackST,
            (char **) &buffHeapStr,
            &lenBuffUL,
            outFILE
         );

         goto nextLine_main_sec04;
      } /*If: this is an comment; print out*/

      /**************************************************\
      * Main Sec-04 Sub-03:
      *  - make and print the primMask header
      \**************************************************/

      if(! firstReadBl)
      { /*If: I need to print out the program header*/
         fprintf(
            outFILE,
            "@PG\tID:primMask\tPN:primMask"
         );

         fprintf(
            outFILE,
            "\tVN:%i-%02i-%02i\tCL:primMask -mask %c",
            def_year_primMask,
            def_month_primMask,
            def_day_primMask,
            maskSC
         );

         if(filterBl)
            fprintf(
               outFILE,
               "\t-filter"
            );
         else
            fprintf(
               outFILE,
               "\t-no-filter"
            );

         fprintf(
            outFILE,
            "\t-fudge %i -prim %s",
            fudgeSI,
            primFileStr
         );

         if(samFileStr)
            fprintf(
               outFILE,
               "\t-sam %s",
               samFileStr
            );
         else
            fprintf(
               outFILE,
               "\t-sam -"
            );

         if(outFileStr)
            fprintf(
               outFILE,
               "\t-out %s\n",
               outFileStr
            );
         else
            fprintf(
               outFILE,
               "\t-out -\n"
            );

         firstReadBl = 1;
      } /*If: I need to print out the program header*/

      /**************************************************\
      * Main Sec-04 Sub-04:
      *  - mask primers and print entry
      \**************************************************/

      errSL =
         maskPrimers(
            &samStackST,
            startAryHeapUI,
            endAryHeapUI,
            flagAryHeapUI,
            numPrimSI,
            fudgeSI,
            maskSC
         ); /*Mask primers in the sequence*/

      if(filterBl && errSL)
         goto nextLine_main_sec04; /*rm non-primer seq*/

      pSamEntry(
         &samStackST,
         (char **) &buffHeapStr,
         &lenBuffUL,
         outFILE
      );

      /**************************************************\
      * Main Sec-04 Sub-05:
      *  - get the next entry
      \**************************************************/

      nextLine_main_sec04:;

      errSC =
         (schar)
         readSamLine(
           &samStackST,
           (char **) &buffHeapStr,
           &lenBuffUL,
           samFILE
         ); /*Get the first line in the sam file*/
   } /*Loop: Read in all lines and mask primers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Clean up
   ^   o main sec-05 sub-01:
   ^     - Success clean up
   ^   o main sec-05 sub-02:
   ^     - Error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(errSC != 1)
   { /*If: I had an memroy error*/
      fprintf(
         stderr,
         "Memory error readin -sam %s\n",
         samFileStr
      );

      goto errCleanUp_main_sec05_sub02;
   } /*If: I had an memroy error*/

   /*****************************************************\
   * Main Sec-05 Sub-01:
   *   - Success clean up
   \*****************************************************/

   cleanUp_main_sec05_sub01:;

   free(startAryHeapUI);
   startAryHeapUI = 0;

   free(endAryHeapUI);
   endAryHeapUI = 0;

   free(flagAryHeapUI);
   flagAryHeapUI = 0;

   free(buffHeapStr);
   buffHeapStr = 0;

   freeSamEntryStack(&samStackST);

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   exit(0);

   /*****************************************************\
   * Main Sec-05 Sub-02:
   *   - Error clean up
   \*****************************************************/

   errCleanUp_main_sec05_sub02:;

   free(startAryHeapUI);
   startAryHeapUI = 0;

   free(endAryHeapUI);
   endAryHeapUI = 0;

   free(flagAryHeapUI);
   flagAryHeapUI = 0;

   free(buffHeapStr);
   buffHeapStr = 0;

   freeSamEntryStack(&samStackST);

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   exit(-1);
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
