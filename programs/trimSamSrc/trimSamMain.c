/*#######################################################\
# Name: trimSamFile.c
#   - driver for triming sam file entries 
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o fun01: pversion_trimSamMain
'     - prints version number
'   o fun02: phelp_trimSamMain
'     - prints help message
'   o fun03: input_trimSamMain
'     - gets user input from input arguments
'   o main:
'     -  Main function to glue everything together
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*I still will have issues with fprintf on plan9, so this
`  is just one part of the problem
*/
#ifdef plan9
   #include <u.h>
   #include <libc.h> /*think includes <string.h>*/
#else
   #include <stdlib.h>
   #include <string.h>
#endif

#include <stdio.h>

#include "../genLib/trimSam.h"

/*no .c files*/
#include "../genLib/dataTypeShortHand.h"
#include "trimSamVer.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/samEntry.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/base10str.h"
!   - .h  #include "../genLib/dataTypeShortHand.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_keepNoMap_trimSamMain 0

/*-------------------------------------------------------\
| Fun01: pversion_trimSamMain
|   - prints version number
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_trimSamMain(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "trimSam version: %i-%02i-%02i\n",
      def_year_trimSamVer,
      def_month_trimSamVer,
      def_day_trimSamVer
   );
} /*pversion_trimSamMain*/

/*-------------------------------------------------------\
| Fun02: phelp_trimSamMain
|   - prints help message
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_trimSamMain(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '      - usage block
   '   o fun02 sec02:
   '      - input block
   '   o fun02 sec03:
   '      - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "trimSam -sam reads.sam [options ...] > out.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - removes softmaskng from reads in a sam file\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required; stdin]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to trim\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -out out.sam: [stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file to print trimmed reads to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -keep-no-map: [Optional; No]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o keep unmapped reads\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with -no-unmapped\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -h: prints this help message and exits\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: prints version number and exits\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "   - prints trimmed reads to stdout or -out\n"
   );
} /*phelp_trimSamMain*/

/*-------------------------------------------------------\
| Fun03: input_trimSamMain
|   - gets user input from input arguments
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStr:
|     o pointer to c-string to point to sam file name in
|       argAryStr
|   - outFileStr:
|     o pointer to c-string to point to output file name
|       in argAryStr
|   - keepNoMapBl:
|     o set to 1 if keeping unmapped reads
|     o set to 0 if discarding unmapped reads
| Output:
|   - Modifies:
|     o input variables (except lenArgsSI and argAryStr)
|       to hold or point to user input
|   - Returns:
|     o 0: if valid input
|     o 1: if printed help message/version number
|     o 2: if invalid input
\-------------------------------------------------------*/
char input_trimSamMain(
    int lenArgsSI,           /*number arguments input*/
    char *argAryStr[],       /*array with user arguments*/
    signed char **samFileStr,/*file name of input file*/
    signed char **outFileStr,/*file name of output file*/
    signed char *keepNoMapBl /*keeping unmapped reads?*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from input arguments
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if no input provided
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec01:
    ^   - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    schar errSC = 0;
    sint siArg = 1;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec02:
    ^   - check if no input provided
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(lenArgsSI <= 1)
    { /*If: no user input; print help message*/
       phelp_trimSamMain(stdout);
       goto phelp_fun03_sec04;
    } /*If: no user input; print help message*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec03:
    ^   - get user input
    ^   o fun03 sec03 sub01:
    ^     - start loop and check for normal input
    ^   o fun03 sec03 sub02:
    ^     - check for help message requests
    ^   o fun03 sec03 sub03:
    ^     - check for version number requests
    ^   o fun03 sec03 sub04:
    ^     - handel invalid input
    ^   o fun03 sec03 sub05:
    ^     - move to next argument
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun03 Sec03 Sub01:
    *   - start loop and check for normal input
    \****************************************************/

    while(siArg < lenArgsSI)
    { /*loop through all user input arguments*/
        if(! strcmp("-sam", argAryStr[siArg]))
        { /*If: This is the input file*/
           ++siArg;
           *samFileStr = (schar *) argAryStr[siArg];
        } /*If: This is the input file*/

        else if(! strcmp("-out", argAryStr[siArg]))
        { /*Else If: This is the output file*/
           ++siArg;
           *outFileStr = (schar *) argAryStr[siArg];
        } /*Else If: This is the output file*/

        else if(! strcmp("-keep-no-map",argAryStr[siArg]))
           *keepNoMapBl = 1;

        else if(! strcmp("-no-unmapped",argAryStr[siArg]))
           *keepNoMapBl = 0;

        /************************************************\
        * Fun03 Sec03 Sub02:
        *   - check for help message requests
        \************************************************/

        else if(! strcmp("-h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_trimSamMain(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_trimSamMain(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_trimSamMain(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("-help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_trimSamMain(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_trimSamMain(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        /************************************************\
        * Fun03 Sec03 Sub03:
        *   - check for version number requests
        \************************************************/

        else if(! strcmp("-v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_trimSamMain(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_trimSamMain(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_trimSamMain(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("-version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_trimSamMain(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_trimSamMain(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        /************************************************\
        * Fun03 Sec03 Sub04:
        *   - handel invalid input
        \************************************************/

        else
        { /*Else: invalid input*/
           fprintf(
              stderr,
              "%s is not recognized\n",
              argAryStr[siArg]
           );

           goto err_fun03_sec04;
        } /*Else: invalid input*/

        /************************************************\
        * Fun03 Sec03 Sub05:
        *   - move to next argument
        \************************************************/

       ++siArg;
    } /*loop through all user input arguments*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec04:
    ^   - clean up
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_fun03_sec04;

    phelp_fun03_sec04:;
    errSC = 1;
    goto cleanUp_fun03_sec04;

    pversion_fun03_sec04:;
    errSC = 1;
    goto cleanUp_fun03_sec04;

    err_fun03_sec04:;
    errSC = 2;
    goto cleanUp_fun03_sec04;

    cleanUp_fun03_sec04:;
    return errSC;
} /*input_trimSamMain*/

/*-------------------------------------------------------\
| Main:
|   - driver function for trimSam
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - prints trimmed reads to output file
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   int lenArgsSI,
   char *argsPtrCStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC: main
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get input and open files
   '   o main sec03:
   '     - trim reads
   '   o main Sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec01:
    ^   - variable declarations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    schar *samFileStr = 0;     /*sam file to trim*/
    schar *outFileStr = 0;     /*output file (as sam)*/
    schar keepNoMapBl = def_keepNoMap_trimSamMain;
    schar errSC = 0;

    FILE *samFILE = 0;
    FILE *outFILE = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec02:
    ^   - get input and open files
    ^   o main sec02 sub01:
    ^     - get input
    ^   o main sec02 sub02:
    ^     - open input sam file
    ^   o main sec02 sub03:
    ^     - open ouput file (sam)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec02 Sub01:
    *   - get input
    \****************************************************/

    errSC =
        input_trimSamMain(
            lenArgsSI,     /*number argumnets input*/
            argsPtrCStr,   /*input arguments*/
            &samFileStr,   /*input sam file*/
            &outFileStr,   /*output file*/
            &keepNoMapBl   /*Keep unammped reads?*/
    ); /*Get user input*/

    if(errSC)
    { /*If: had error*/
       --errSC; /*reduce help/versoin numbers to 0*/
       goto cleanUp_main_sec04;
    } /*If: had error*/

    /****************************************************\
    * Main Sec02 Sub02:
    *   - open input sam file
    \****************************************************/

    if(! samFileStr || samFileStr[0] == '-')
       samFILE = stdin;

    else
    { /*Else: using a file for input*/
        samFILE =
           fopen(
              (char *) samFileStr,
              "r"
           );
    
        if(samFILE == 0)
        { /*If: could not open sam file*/
            fprintf(
                stderr,
                "could not open -sam %s\n",
                samFileStr
            ); /*If file was not valid*/

            goto fileErr_main_sec04;
        } /*If: could not open sam file*/
    } /*Else: using a file for input*/

    /****************************************************\
    * Main Sec02 Sub03:
    *   - open ouput file (sam)
    \****************************************************/

    if(! outFILE || outFileStr[0] == '-')
       outFILE = stdout;

    else
    { /*Else: user provided output file*/
        outFILE =
           fopen(
              (char *) outFileStr,
              "w"
           );

        if(outFILE == 0)
        { /*If: could no open output file*/
            fprintf(
                stderr,
                "could not open -out (%s)\n",
                outFileStr
            );
                
            goto fileErr_main_sec04;
        } /*If: could no open output file*/
    } /*Else: user provided output file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec03:
    ^   - trim reads
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC =
       trimReads_trimSam(
          samFILE,
          outFILE,
          keepNoMapBl
       );

    if(errSC)
    { /*If: had an error*/
       fprintf(
          stderr,
          "memory error when trimming reads\n"
       );

       goto memErr_main_sec04;
    } /*If: had an error*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec04:
    ^   - clean up
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_main_sec04;

    memErr_main_sec04:;
    errSC = def_memErr_trimSam;
    goto cleanUp_main_sec04;

    fileErr_main_sec04:;
    errSC = def_fileErr_trimSam;
    goto cleanUp_main_sec04;

    cleanUp_main_sec04:;

    if(
          samFILE
       && samFILE != stdin
       && samFILE != stdout
    ) fclose(samFILE);

    samFILE = 0;

    if(
          outFILE
       && outFILE != stdin
       && outFILE != stdout
    ) fclose(outFILE);

    outFILE = 0;

    exit(errSC);
} /*main function*/


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
