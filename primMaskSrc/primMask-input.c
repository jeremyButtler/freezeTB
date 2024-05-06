/*########################################################
# Name: primMask-input
#   - get input the user provided by commandline and print
#     help message
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header
'     - included libraries
'   o .c fun-01: checkIfHelp
'     - Checks if input paramter is help message request
'   o .c fun-02: checkIfVersion
'     - Checks if input paramter is an version request
'   o fun-03: getInputPrimMask
'     - processes user command line supplied input
'   o fun-04: pHelpPrimMask
'     - print the help message for primMask
'   o license:
'     - Licensing for this code (public domain / mit)
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

#include "primMask-input.h"

#include <stdio.h>

/*.h files only*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"

#include "primMask-fun.h"
   /*This is only using the default variable defintions,
   `   so the .c file is not needed
   */

/*-------------------------------------------------------\
| Fun-01: checkIfHelp
|   - Checks if input paramter is an help message request
| Input:
|   - parmStr:
|     o Parameter to see if is an help message
| Output:
|   - Returns:
|     o 0 if not an help message request
|     o 1 if is an help message request
\-------------------------------------------------------*/
#define \
checkIfHelp( \
   parmStr \
)({ \
   schar retSC = 0; \
   \
   if(! cStrEql("-h", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("--h", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("help", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("-help", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("--help", (parmStr), 0)) \
      retSC = 1; \
   \
   retSC; \
}) /*checkIfHelp*/

/*-------------------------------------------------------\
| Fun-02: checkIfVersion
|   - Checks if input paramter is an version request
| Input:
|   - parmStr:
|     o Parameter to see if is an version requiest
| Output:
|   - Returns:
|     o 0 if not an verson request
|     o 1 if is an version request
\-------------------------------------------------------*/
#define \
checkIfVersion( \
   parmStr \
)({ \
   schar retSC = 0; \
   \
   if(! cStrEql("-v", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("--v", (parmStr), 0)) \
      retSC = 1; \
   if(! cStrEql("-V", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("--V", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("version", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("-version", (parmStr), 0)) \
      retSC = 1; \
   else if(! cStrEql("--version", (parmStr), 0)) \
      retSC = 1; \
   \
   retSC; \
}) /*checkIfHelp*/

/*-------------------------------------------------------\
| Fun-03: getInputPrimMask
|   - processes user command line supplied input
| Input:
|   - numArgsUI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with the users input
|   - samFileStr:
|     o pointer to c-string to point to the input sam file
|       name
|   - primFileStr:
|     o pointer to c-string to point to the input primer
|       coorindates file name
|   - outFileStr:
|     o pointer to c-string to point to ouput file name
|   - maskSC:
|     o Pointer to char to hold charcter to mask with
|   - fudgeSI:
|     o int Pointer to hold the amount to fudge primer
|       start/end by
|   - filterBl:
|     o character set to 1 or 0 if doing filtering.
|       - 1 removing reads without primers
|       - 0 not removing reads without primers
| Output:
|   - Modifies:
|     o all input values except numArgsUI and argAryStr
|       to the user input.
|   - Returns:
|     o 0 for no errors
|     o def_help_primMask if help message requested
|     o def_version_primMask if version number requested
|     o def_nonNumeric | (arg_index << 8) for numeric
|       errors
|     o def_unkownInput | (arg_index << 8) if an argument
|       could not be recongnized
\-------------------------------------------------------*/
signed long
getInputPrimMask(
   unsigned int numArgsUI,
   char *argAryStr[],
   signed char **samFileStr,
   signed char **primFileStr,
   signed char **outFileStr,
   signed char *maskSC,
   signed int *fudgeSI,
   signed char *filterBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-03 TOC:
   '   - process user command line input
   '   o fun-03 sec-01:
   '     - variable declerations
   '   o fun-03 sec-02:
   '     - Process user input
   '   o fun-03 sec-03:
   '     - Return success
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiArg = 1;
   char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-02:
   ^   - Process user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(uiArg < numArgsUI)
   { /*Loop: Process each peice of user input*/
      if(! cStrEql("-sam", argAryStr[uiArg], 0))
      { /*If: This was an sam file*/
         ++uiArg;
         *samFileStr = (schar *) argAryStr[uiArg];
      } /*If: This was an sam file*/

      else if(! cStrEql("-prim", argAryStr[uiArg], 0))
      { /*Else If: This was an primer coordiantes file*/
         ++uiArg;
         *primFileStr = (schar *) argAryStr[uiArg];
      } /*Else If: This was an primer coordinates file*/

      else if(! cStrEql("-out", argAryStr[uiArg], 0))
      { /*Else If: This was an output file*/
         ++uiArg;
         *outFileStr = (schar *) argAryStr[uiArg];
      } /*Else If: This was an output file*/

      else if(! cStrEql("-mask", argAryStr[uiArg], 0))
      { /*Else If: This was an output file*/
         ++uiArg;
         *maskSC = (schar) *argAryStr[uiArg];
      } /*Else If: This was an output file*/

      else if(! cStrEql("-fudge", argAryStr[uiArg], 0))
      { /*Else If: This was the fudge length*/
         ++uiArg;

         tmpStr =
            base10StrToSI(
               (char *) argAryStr[uiArg],
               *fudgeSI
            );

         if(*tmpStr)
            return 
                 def_nonNumeric_primMask
               | ((uiArg - 1) << 8);
      } /*Else If: This was the fudge length*/

      else if(! cStrEql("-filter", argAryStr[uiArg], 0))
         *filterBl = 1;

      else if(! cStrEql("-no-filter",argAryStr[uiArg],0))
         *filterBl = 1;

      else if( checkIfHelp( argAryStr[uiArg] ) )
         return def_help_primMask;

      else if( checkIfVersion( argAryStr[uiArg] ) )
         return def_version_primMask;

      else
         return def_unkownInput_primMask | (uiArg << 8);

      ++uiArg;
   } /*Loop: Process each peice of user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-03:
   ^   - Return success
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;
} /*getInputPrimMask*/

/*-------------------------------------------------------\
| Fun-04: pHelpPrimMask
|   - print the help message for primMask
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints:
|     o the help messge to outFILE
\-------------------------------------------------------*/
void
pHelpPrimMask(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-04 TOC:
   '   - print the help message for primMask
   '   o fun-04 sec-01:
   '     - usage block
   '   o fun-04 sec-02:
   '     - input block
   '   o fun-04 sec-03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "primMask -sam reads.sam -prim primer-coords.tsv\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Masks primer positions in reads\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-02:
   ^   - input block
   ^   o fun-04 sec-02 sub-01:
   ^     - input block header
   ^   o fun-04 sec-02 sub-02:
   ^     - sam file entry
   ^   o fun-04 sec-02 sub-03:
   ^     - output entry
   ^   o fun-04 sec-02 sub-04:
   ^     - primer coordinates entry
   ^   o fun-04 sec-02 sub-05:
   ^     - mask character entry
   ^   o fun-04 sec-02 sub-06:
   ^     - fudge length entry
   ^   o fun-04 sec-02 sub-07:
   ^     - filtering
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-04 Sec-02 Sub-01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-02:
   *   - sam file entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam: [stdin]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Sam file with reads to mask primers in\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdin input\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-03:
   *   - output entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out: [stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o File to output masked sam file to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdout output\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-04:
   *   - primer coordinates entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prim: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Tsv file with the primer coordinages\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - First column is ingored (refernce id?)\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Second column is ingored (primer name?)\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Third column is 1 primers are paired or 0\n"
   );

   fprintf(
     (FILE *) outFILE,
     "        o Paired = both primers must be present to"
   );

   fprintf((FILE *) outFILE,  " mask\n");

   fprintf(
     (FILE *) outFILE,
     "      - Fourth column is forward start coordinate\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Fifth column is forward end coordinate\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Six column is reverse start coordinate\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Seventh column is reverse end coordinate\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-05:
   *   - mask character entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -mask: [%c]\n",
      def_mask_primMask
   );

   fprintf(
      (FILE *) outFILE,
      "    o Chacter to mask with\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-06:
   *   - fudge length entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -fudge: [%i]\n",
      def_fudge_primMask
   );

   fprintf(
      (FILE *) outFILE,
      "    o How much seqence can come before an primer\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o -1 = mask regardless of position\n"
   );

   /*****************************************************\
   * Fun-04 Sec-02 Sub-07:
   *   - filtering
   \*****************************************************/

   if(def_filter_primMask)
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [Yes]\n"
      );
   } /*If: default is to filter*/

   else
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [No]\n"
      );
   } /*If: default is to filter*/

   fprintf(
      (FILE *) outFILE,
      "    o Remove reads with no detected primers\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Disable with -no-filter\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints sam file with masked reads to -out\n"
   );
} /*pHelpPrimMask*/

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
