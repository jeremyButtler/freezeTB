/*########################################################
# Name: tbSpoligo-checkInput
#   - checks if the users input is valid and updates the
#     correct values
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun-01: checkInput_tbSpoligo
'     - checks the users input & sets to correct variable
'   o fun-02: phelp_tbSpoligo
'     - prints the help messge for tbSpoligo
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

#include <stdio.h>

#include "tbSpoligo-checkInput.h"

/*.h files only*/
#include "tbSpoligo-memwater-fun.h"
   /*Normally this would include an .c file, but this is
   `   in this case I am only using a few default values
   `   from the header file.
   */

#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"

/*-------------------------------------------------------\
| Fun-01: checkInput_tbSpoligo
|   - checks the users input and sets to correct variable
| Input:
|   o parmStr:
|     - C-string with the paramter to check
|   o argStr:
|     - C-string with the argument to check
|   o seqFileStr:
|     - Pointer to c-string to hold the sequence file name
|   o seqTypeFlag:
|     - Holds the type of file input for seqFileStr
|   o refFileStr:
|     - Pointer to c-string to hold the spoligo sequences
|       file name
|   o spoliogDbStr:
|     - Pointer to c-string to hold the path to the
|       database with spoligo lineages
|   o outFileStr:
|     - Pointer to c-string to hold the output file name
|   o minPercScoreF:
|     - Pointer to float to hold the minimum percent score
|       to detect an spoligotype hit
|   o dirStartSI:
|     - Will hold the start of the DR region
|   o dirEndSI:
|     - Will hold the end of the DR region
|   o minKmersSI:
|     - int to hold the minimum number of kmer matches to
|       have to do an waterman alignment (kmer method)
|   o fastTypingBl:
|     - Set to 1 if user wants faster kmer method
|     - Set to 0 if user wants slower method
| Output:
|   - Modifies:
|     o All input variables except parmStr and argStr to
|       hold the users input
|       - Sets seqTypeFlag to:
|         o def_fqFile_tbSpoligo if fastq file input
|         o def_faFile_tbSpoligo if fasta file input
|         o def_samFile_tbSpoligo if an sam file input
|         o def_samConFile_tbSpoligo if an sam file with
|           mapped consensuses was input
|   - Returns:
|     o 0: Parameter is a boolean (no argument)
|     o 1: Parameter has an agrument
|     o 1 | def_numericErr_tbSpoligo for numeric errors
|     o def_phelp_tbSpoligo if help message requested
|     o def_pversoin_tbSpoligo if version number requested
|     o def_invalidErr_tbSpoligo for unrecongnized entries
\-------------------------------------------------------*/
signed char
checkInput_tbSpoligo(
   signed char *parmStr,     /*Paramter to check*/
   signed char *argStr,      /*Argument to check*/
   signed char **seqFileStr, /*sequence file name*/
   signed char *seqTypeFlag, /*Type of file input*/
   signed char **refFileStr, /*Spoligotype seq file name*/
   signed char **spoliogDbStr,/*database of lineages*/
   signed char **outFileStr, /*File to save to*/
   float *minPercScoreF,     /*min % score to count hit*/
   signed int *dirStartSI,   /*start of DR region*/
   signed int *dirEndSI,     /*start of DR region*/
   signed int *minKmersSI,   /*Min number kmers for fast*/
   signed char *fastTypingBl /*1: Do fast spoligo typing*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC:
   '   o fun-01 sec-01:
   '     - Variable declerations
   '   o fun-01 sec-02:
   '     - Check if the input was an file
   '   o fun-01 sec-03:
   '     - Return invalid arguments
   '   o fun-01 sec-04:
   '     - Check if the input is an filerting parameter
   '   o fun-01 sec-05:
   '     - Check if the user wanted the help message
   '   o fun-01 sec-06:
   '     - Check if the user wanted the version number
   '   o fun-01 sec-07:
   '     - Check if user is choosing an alignment method
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - Check if the input was an file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! cStrEql("-spoligo", parmStr, 0))
   { /*If: an fasta file with spoligotypes*/
      *refFileStr = argStr;
      return 1;
   } /*If: an fasta file with spoligotypes*/

   else if(! cStrEql("-db", parmStr, 0))
   { /*Else If: the lineage database was input*/
      *spoliogDbStr = argStr;
      return 1;
   } /*Else If: the lineage database was input*/

   else if(! cStrEql("-out", parmStr, 0))
   { /*Else If: an fastq file was input*/
      *outFileStr = argStr;
      return 1;
   } /*Else If: an fastq file was input*/

   else if(! cStrEql("-fq", parmStr, 0))
   { /*Else If: an fastq file was input*/
      *seqTypeFlag = def_fqFile_tbSpoligo;
      *seqFileStr = argStr;
      return 1;
   } /*Else If: an fastq file was input*/

   else if(! cStrEql("-fa", parmStr, 0))
   { /*Else If: an fastq file was input*/
      *seqTypeFlag = def_faFile_tbSpoligo;
      *seqFileStr = argStr;
      return 1;
   } /*Else If: an fastq file was input*/

   else if(! cStrEql("-sam", parmStr, 0))
   { /*Else If: an sam file was input*/
      *seqTypeFlag = def_samFile_tbSpoligo;
      *seqFileStr = argStr;
      return 1;
   } /*Else If: an sam file was input*/

   else if(! cStrEql("-sam-con", parmStr, 0))
   { /*Else If: an sam file of an consensus was input*/
      *seqTypeFlag = def_samConFile_tbSpoligo;
      *seqFileStr = argStr;
      return 1;
   } /*Else If: an sam file of an consensus was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - Check if the input is an filerting parameter
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-dr-start", parmStr, 0))
   { /*Else If: the direct repeat start is input*/
      tmpStr =
         (signed char *)
         base10StrToSI((char *) argStr, *dirStartSI);

      if(*tmpStr != '\0')
         return 1 | def_numericErr_tbSpoligo;

       return 1;
   } /*Else If: the direct repeat start is input*/

   else if(! cStrEql("-dr-end", parmStr, 0))
   { /*Else If: the direct repeat start is input*/
      tmpStr =
         (signed char *)
         base10StrToSI((char *) argStr, *dirEndSI);

      if(*tmpStr != '\0')
         return 1 | def_numericErr_tbSpoligo;

       return 1;
   } /*Else If: the direct repeat start is input*/

   else if(! cStrEql("-min-score", parmStr, 0))
   { /*Else If: min score to keep an spoligotype*/
      *minPercScoreF = atof((char *) argStr);
      return 1;
   } /*Else If: min score to keep an spoligotype*/

   else if(! cStrEql("-min-kmers", parmStr, 0))
   { /*Else If: Min kmers for fast method selected*/
      tmpStr =
         (signed char *)
         base10StrToSI((char *) argStr,*minKmersSI);

      if(*tmpStr != '\0')
         return 1 | def_numericErr_tbSpoligo;

      *fastTypingBl = 1;
      return 1;
   } /*Else If: Min kmers for fast method selected*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^   - Check if user is choosing an alignment method
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-fast", parmStr, 0))
   { /*Else If: The fast kmer method was selected*/
      *fastTypingBl = 1;
      return 0;
   } /*Else If: The fast kmer method was selected*/

   else if(! cStrEql("-slow", parmStr, 0))
   { /*Else If: the slow waterman was slected*/
      *fastTypingBl = 0;
      return 0;
   } /*Else If: the slow waterman was slected*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - Check if the user wanted the help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-h", parmStr, 0))
      return 1 | def_phelp_tbSpoligo;

   else if(! cStrEql("--h", parmStr, 0))
      return 1 | def_phelp_tbSpoligo;

   else if(! cStrEql("help", parmStr, 0))
      return 1 | def_phelp_tbSpoligo;

   else if(! cStrEql("-help", parmStr, 0))
      return 1 | def_phelp_tbSpoligo;

   else if(! cStrEql("--help", parmStr, 0))
      return 1 | def_phelp_tbSpoligo;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^   - Check if the user wanted the version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(! cStrEql("-v", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("--v", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("-V", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("--V", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("version", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("-version", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   else if(! cStrEql("--version", parmStr, 0))
      return 1 | def_pversion_tbSpoligo;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-07:
   ^   - Return invalid arguments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
      return def_invalidErr_tbSpoligo;
} /*checkInput_tbSpoligo*/

/*-------------------------------------------------------\
| Fun-02: phelp_tbSpoligo
|   - prints the help messge for tbSpoligo
| Input:
|   o outFileStr:
|     - c-string with name of file to print help message
|       to
|     - Use "-" or "1" for stdout and "2" for stderr
| Output:
|   o Prints:
|     - The help message to outFileStr
|   o Returns:
|     - 0 for no errors
|     - 1 if I could not open the file
\-------------------------------------------------------*/
signed char
phelp_tbSpoligo(
   signed char *outFileStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC:
   '   - prints the help messge for tbSpoligo
   '   o fun-02 sec-01:
   '     - Variable declerations
   '   o fun-02 sec-02:
   '     - Open the output file
   '   o fun-02 sec-03:
   '     - Print usage lines
   '   o fun-02 sec-04:
   '     - Print user input lines
   '   o fun-02 sec-05:
   '     - Print output options
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outFileStr)
      outFILE = stdout;

   else if(*outFileStr == '-')
      outFILE = stdout;
   else if(*outFileStr == '1')
      outFILE = stdout;
   else if(*outFileStr == '2')
      outFILE = stderr;
   else
   { /*Else: The user wants to print to an file*/
      outFILE = fopen((char *) outFileStr, "w");

      if(! outFILE)
         return 1;
   } /*Else: The user wants to print to an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print usage lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "tbSpoligo -spoligo spacers.fa -sam consensus.sam\n"
   );

   fprintf(
     outFILE,
     "or tbSpoligo -spoligo spacers.fa -fa consensus.fa\n"
   );

   fprintf(
      outFILE,
      "  - Finds spoligotype of an sequence\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-04:
   ^   - Print user input line
   ^   o fun-02 sec-04 sub-01:
   ^     - Print input header
   ^   o fun-02 sec-04 sub-02:
   ^     - Print spoligotype sequence input
   ^   o fun-02 sec-04 sub-03:
   ^     - Print sam/fasta/fastq input options
   ^   o fun-02 sec-04 sub-04:
   ^     - Print database input
   ^   o fun-02 sec-04 sub-05:
   ^     - Print mapping coordinates options
   ^   o fun-02 sec-04 sub-06:
   ^     - Print the filtering paramerters
   ^   o fun-02 sec-04 sub-07:
   ^     - Print alignment method
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-04 Sub-01:
   *   - Print input header
   \*****************************************************/

   fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun-02 Sec-04 Sub-02:
   *   - Print spoligotype sequence input
   \*****************************************************/

   fprintf(
      outFILE,
      "  -spoligo: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Fasta file with spoligotype references\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-03:
   *   - Print sam/fasta/fastq input options
   \*****************************************************/

   if(def_seqStdin_tbSpoligo == def_samFile_tbSpoligo)
      fprintf(
         outFILE,
         "  Input file: [-sam -]\n"
      );
   else if(def_seqStdin_tbSpoligo == def_faFile_tbSpoligo)
      fprintf(
         outFILE,
         "  Input file: [-fa -]\n"
      );
   else if(def_seqStdin_tbSpoligo == def_fqFile_tbSpoligo)
      fprintf(
         outFILE,
         "  Input file: [-fq -]\n"
      );
   else
      fprintf(
         outFILE,
         "  Input file: [Required]\n"
      );

   fprintf(
      outFILE,
      "    o File with sequences to spoligotype\n"
   );

   fprintf(
      outFILE,
      "    o Make sure your sequence(s) cover the\n"
   );

   fprintf(
      outFILE,
      "      entire direct repeat region\n"
   );

   fprintf(
      outFILE,
      "    o Use \"-option -\" to use stdin input\n"
   );

   fprintf(
      outFILE,
      "    o Options\n"
   );

   fprintf(
      outFILE,
      "        -sam: Use an sam file\n"
   );

   fprintf(
      outFILE,
      "        -fa: use an fasta file (slow)\n"
   );

   fprintf(
      outFILE,
      "        -fq: use an fastq file (slow)\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-04:
   *   - Print database input
   \*****************************************************/

   fprintf(outFILE, "  -db: [None]\n");

   fprintf(
      outFILE,
      "    o Database (csv file) to look up lineages in\n"
   );

   fprintf(
     outFILE,
     "    o Strain,barcode,ignored,lineage,sit,countries\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-05:
   *   - Print output file
   \*****************************************************/

   fprintf(outFILE, "  -out: [stdout]\n");

   fprintf(
     outFILE,
     "    o File to output results to (- is stdout)\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-06:
   *   - Print mapping coordinates options
   \*****************************************************/

   fprintf(
      outFILE,
      "  -dr-start: [%i]\n",
      def_DRStart_tbSpoligo
   );

   fprintf(
    outFILE,
    "    o First direct repeat region base in reference\n"
   );

   fprintf(
     outFILE,
     "    o -sam: to align spacers to DR region only\n"
   );

   fprintf(
      outFILE,
      "  -dr-end: [%i]\n",
      def_DREnd_tbSpoligo
   );

   fprintf(
    outFILE,
    "    o Last direct repeat region base in reference\n"
   );

   fprintf(
     outFILE,
     "    o -sam: to align spacers to DR region only\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-07:
   *   - Print the filtering paramerters
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-score: [%.2f]\n",
      def_minPercScore_tbSpoligoWater
   );

   fprintf(
     outFILE,
     "    o Minimim percentage of score to count an hit\n"
   );

   fprintf(
     outFILE,
     "    o Scores: match = 5, snp = -4, indel = -10\n"
   );

   fprintf(
      outFILE,
      "  -min-kmers: [FUTURE]\n"
   );

   fprintf(
     outFILE,
     "    o This is a future option for an kmer search\n"
   );

   fprintf(
     outFILE,
     "    o May never be implemented\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-08:
   *   - Print alignment method
   \*****************************************************/

   fprintf(
      outFILE,
      "  -fast: [FUTURE]\n"
   );

   fprintf(
     outFILE,
     "    o This is a future option for an kmer search\n"
   );

   fprintf(
     outFILE,
     "    o May never be implemented\n"
   );

   fprintf(
      outFILE,
      "  -slow: [Default]\n"
   );

   fprintf(
     outFILE,
     "    o Uses an Waterman alignment to map spaces\n"
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-09:
   *   - Print help/verson command
   \*****************************************************/

   fprintf(outFILE, "  -h: print this help message\n");
   fprintf(outFILE, "  -v: print the version number\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - Print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");

   fprintf(
      outFILE,
      "  - Prints sequence name and spoligotype barcode\n"
   );

   fprintf(
      outFILE,
      "  - name\\tlineageId\\tbarcode\\toctal\\tstrain"

   );

   fprintf(outFILE, "\\tSIT\\tcountries\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-06:
   ^   - Close the output file and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outFILE != stdout && outFILE != stderr)
      fclose(outFILE);

   outFILE = 0;

   return 0;
} /*phelp_tbSpoligo*/

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
