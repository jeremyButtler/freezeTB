/*########################################################
# Name: tbSpoligo-checkInput
#   - checks if the users input is valid and updates the
#     correct values
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: checkInput_tbSpoligo
'     - checks the users input & sets to correct variable
'   o fun02: phelp_tbSpoligo
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
#include "kmerFind.h"
   /*Normally this would include an .c file, but this is
   `   in this case I am only using a few default values
   `   from the header file.
   */

#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"

/*-------------------------------------------------------\
| Fun01: checkInput_tbSpoligo
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
|   o minKmersPercF:
|     - pointer to float to hold the minimum percent of
|       kmers needed to do an waterman alignment on an
|       window in the fast kmer method
|   o fastTypingBl:
|     - Set to 1 if user wants faster kmer method
|     - Set to 0 if user wants slower method
|   o fragBl:
|     - Set to 1 if user is inputing fragments
|     - Set to 0 if user is inputing direct repeat regions
|   o conFragBl:
|     - Set to 1 if user is inputing consensus fragments
|     - Set to 0 if user not consensus or not fragments
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
   float *minKmersPercF,     /*Min number kmers for fast*/
   signed char *fastTypingBl,/*1: Do fast spoligo typing*/
   signed char *fragBl,      /*1: fragment checking*/
   signed char *conFragBl    /*1: non-con fragment mode*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   o fun01 sec01:
   '     - Variable declerations
   '   o fun01 sec02:
   '     - Check if the input was an file
   '   o fun01 sec03:
   '     - Return invalid arguments
   '   o fun01 sec04:
   '     - Check if the input is an filerting parameter
   '   o fun01 sec05:
   '     - Check if the user wanted the help message
   '   o fun01 sec06:
   '     - Check if the user wanted the version number
   '   o fun01 sec07:
   '     - Check if user is choosing an alignment method
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
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
   ^ Fun01 Sec03:
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

   else if(! cStrEql("-min-perc-kmer", parmStr, 0))
   { /*Else If: Min kmers for fast method selected*/
      *minKmersPercF = atof((char *) argStr);

      if(*minKmersPercF == 0)
         return 1 | def_numericErr_tbSpoligo;

      *fastTypingBl = 1;
      return 1;
   } /*Else If: Min kmers for fast method selected*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
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

   else if(! cStrEql("-frag", parmStr, 0))
   { /*Else If: sequence is in fragments*/
      *fragBl = 1;
      return 0;
   } /*Else If: sequence is in fragments*/

   else if(! cStrEql("-con-frag", parmStr, 0))
   { /*Else If: consensus is in fragments*/
      *conFragBl = 1; /*using consensuses*/
      *fragBl = 1;
      return 0;
   } /*Else If: consensus is in fragments*/

   else if(! cStrEql("-read-frag", parmStr, 0))
   { /*Else If: sequence is in fragments*/
      *conFragBl = 0; /*using consensuses*/
      *fragBl = 1;
      return 0;
   } /*Else If: sequence is in fragments*/

   else if(! cStrEql("-no-frag", parmStr, 0))
   { /*Else If: sequence is in fragments*/
      *fragBl = 0;
      *conFragBl = 0; /*using consensuses*/
      return 0;
   } /*Else If: sequence is in fragments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
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
   ^ Fun01 Sec06:
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
   ^ Fun01 Sec07:
   ^   - Return invalid arguments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
      return def_invalidErr_tbSpoligo;
} /*checkInput_tbSpoligo*/

/*-------------------------------------------------------\
| Fun02: phelp_tbSpoligo
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
   ' Fun02 TOC:
   '   - prints the help messge for tbSpoligo
   '   o fun02 sec01:
   '     - Variable declerations
   '   o fun02 sec02:
   '     - Open the output file
   '   o fun02 sec03:
   '     - Print usage lines
   '   o fun02 sec04:
   '     - Print user input lines
   '   o fun02 sec05:
   '     - Print output options
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
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
   ^ Fun02 Sec03:
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
   ^ Fun02 Sec04:
   ^   - Print user input line
   ^   o fun02 sec04 sub01:
   ^     - Print input header
   ^   o fun02 sec04 sub02:
   ^     - Print spoligotype sequence input
   ^   o fun02 sec04 sub03:
   ^     - Print sam/fasta/fastq input options
   ^   o fun02 sec04 sub04:
   ^     - Print database input
   ^   o fun02 sec04 sub05:
   ^     - Print mapping coordinates options
   ^   o fun02 sec04 sub06:
   ^     - Print the filtering paramerters
   ^   o fun02 sec04 sub07:
   ^     - Print alignment method
   ^   o fun02 sec04 sub08:
   ^     - Print alignment method
   ^   o fun02 sec04 sub09:
   ^     - fragment mode options
   ^   o fun02 sec04 sub10:
   ^     - Print help/verson command
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - Print input header
   \*****************************************************/

   fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun02 Sec04 Sub02:
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
   * Fun02 Sec04 Sub03:
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
   * Fun02 Sec04 Sub04:
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
   * Fun02 Sec04 Sub05:
   *   - Print output file
   \*****************************************************/

   fprintf(outFILE, "  -out: [stdout]\n");

   fprintf(
     outFILE,
     "    o File to output results to (- is stdout)\n"
   );

   /*****************************************************\
   * Fun02 Sec04 Sub06:
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
   * Fun02 Sec04 Sub07:
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
      "  -min-kmer-perc: [%f]\n",
      def_minKmerPerc_kmerFind
   );

   fprintf(
     outFILE,
     "    o Minimum percent of kmers needed to do an\n"
   );

   fprintf(
     outFILE,
     "      waterman alignment on an window for -fast\n"
   );

   fprintf(
     outFILE,
     "    o 50%% is 10 or more kmers for spoligotypes\n"
   );

   /*****************************************************\
   * Fun02 Sec04 Sub08:
   *   - Print alignment method
   \*****************************************************/

   if(def_fastSeach_tbSpoligoCheckInput)
   { /*If: I fast kmer is default method*/
      fprintf(
         outFILE,
         "  -fast: [Default]\n"
      );
   } /*If: I fast kmer is default method*/

   else
   { /*Else: I slow is default method*/
      fprintf(
         outFILE,
         "  -fast: [No]\n"
      );
   } /*Else: I slow is default method*/

   fprintf(
     outFILE,
     "    o Use kmers to narrow down possible\n"
   );

   fprintf(
     outFILE,
     "      spoligotype locations (faster)\n"
   );

   if(! def_fastSeach_tbSpoligoCheckInput)
      fprintf(
         outFILE,
         "  -slow: [Default]\n"
      );

   else
      fprintf(
         outFILE,
         "  -slow: [No]\n"
      );

   fprintf(
     outFILE,
     "    o Uses an Waterman alignment to map spacers\n"
   );

   /*****************************************************\
   * Fun02 Sec04 Sub09:
   *   - fragment mode options
   \*****************************************************/

   if(   def_frag_tbSpoligoCheckInput
      || def_conFrag_tbSpoligoCheckInput
   ) fprintf(
         outFILE,
         "  -frag: [Yes]\n"
      );

    else 
      fprintf(
         outFILE,
         "  -frag: [No]\n"
      );

   fprintf(
     outFILE,
     "    o search for fragments (not full genomes)\n"
   );

   if(def_conFrag_tbSpoligoCheckInput)
      fprintf(
         outFILE,
         "  -con-frag: [Yes]\n"
      );

   else
      fprintf(
         outFILE,
         "  -con-frag: [No]\n"
      );

   fprintf(
     outFILE,
     "    o fragments are in an conensus\n"
   );
   
   if(! def_conFrag_tbSpoligoCheckInput
      && def_frag_tbSpoligoCheckInput
   )
      fprintf(
         outFILE,
         "  -read-frag: [Yes]\n"
      );

   else
      fprintf(
         outFILE,
         "  -read-frag: [No]\n"
      );

   fprintf(
     outFILE,
     "    o fragments are in reads\n"
   );

   if(! def_frag_tbSpoligoCheckInput
      && ! def_conFrag_tbSpoligoCheckInput
   ) fprintf(
         outFILE,
         "  -no-frag: [Yes]\n"
      );

   else
      fprintf(
         outFILE,
         "  -no-frag: [No]\n"
      );

   fprintf(
     outFILE,
     "    o reads/consensus have full direct repeat\n"
   );
   
   /*****************************************************\
   * Fun02 Sec04 Sub10:
   *   - Print help/verson command
   \*****************************************************/

   fprintf(outFILE, "  -h: print this help message\n");
   fprintf(outFILE, "  -v: print the version number\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
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
   ^ Fun01 Sec06:
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
