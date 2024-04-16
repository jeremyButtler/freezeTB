/*########################################################
# Name: filtsam
#  - Filters sam files
#  - This is a little slower than samtools.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and defaults/defined variables
'   o fun-01: getFiltSamInput
'     - Gets user input from argsAryStr
'   o fun-02: pHelp_filtsam
'     - Prints the help message for filtsam
'   o fun-03: pversion_filtsam
'     - Prints out the version number for filtsam
'   o main:
'     - Filters sam files and converts sam output to the
'       desiered format
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and defaults / defined variables
|   o header sec-01:
|     - Included libraries
|   o header sec-02:
|     - Default variables or defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^   - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../generalLib/samEntryStruct.h"
#include "../generalLib/trimSam.h"

/*These do not have .c files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependencies
!   o .h #include "../generalLib/numToStr.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^   - Default variables or defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_minLen_filtsam 0
#define def_maxLen_filtsam 0
#define def_minAlnLen_filtsam 0

#define def_minMapq_filtsam 0
#define def_minMedQ_filtsam 7.0f
#define def_minMeanQ_filtsam 7.0f

#define def_trimReadsBl_filtsam 1

/*Flags for the printout type*/
#define defSamPSam 0
#define defSamPFastq 1
#define defSamPFasta 2
#define defSamPStats 3

/*Default output type*/
#define defDefaultOut defSamPSam

/*Version number*/
#define def_year_filtsam 2024
#define def_month_filtsam 4
#define def_day_filtsam 16

/*-------------------------------------------------------\
| Fun-01: getFiltSamInput
|   - Gets user input from argsAryStr
| Input:
|   - lenArgsI:
|     o Numbe of arguments and parameters the user input
|   - argsAryStr:
|     o Pointer to c-string array with user input
|   - samStr:
|     o Will point to path to the sam file to process
|   - outStr:
|     o Will point to path to the output file
|   - outFlagC:
|     o Will hold the output type (sam/fastq/fasta/stats)
|   - rmAryS:
|     o Will hold flags to remove
|   - numRmFlagsS:
|     o Will hold numbe of flags in rmAryS
|   - keepAryS:
|     o Will hold flags to keep
|   - numKeepFlagsS:
|     o Will hold number of flags in keepAryS
|   - minLenUI:
|     o Will hold the minimum read length
|   - maxLenUI:
|     o Will hold the maximum read length
|   - minmedainQF:
|     o Will hold the minimum median Q-score
|   - minMeanQF:
|     o Will hold the minimum mean Q-score
|   - minMapqUC:
|     o Will hold the minimum mapping quality
|   - minAlnLenUI:
|     o Will hold the minimum aligned length
|   - trimReadsBl:
|     o 1: Trim soft masked regions off of reads
|     o 0: Do not trim reads
| Output:
|   - Modifies:
|     o all input variables except lenArgsI and argsAryStr
|       to hold user input.
\-------------------------------------------------------*/
char * getFiltSamInput(
   int lenArgsI,
   char *argsAryStr[],
   char **samStr,
   char **outStr,
   char *outFlagC,
   ushort rmAryS[],
   uchar *numRmFlagsS,
   ushort keepAryS[],
   uchar *numKeepFlagsS,
   uint *minLenUI,
   uint *maxLenUI,
   float *minMedianQF,
   float *minMeanQF,
   uchar *minMapqUC,
   uint *minAlnLenUI,
   char *trimReadsBl
){
   char *parmStr = 0;
   int iArg = 1;

   while(iArg < lenArgsI)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      if(! cStrEql("-sam", parmStr, '\0'))
      { /*If: The user input a sam file*/
         ++iArg;
         *samStr = argsAryStr[iArg];
      } /*If: The user input a sam file*/

      else if(! cStrEql("-out", parmStr, '\0'))
      { /*Else If: The user provided an output file*/
         ++iArg;
         *outStr = argsAryStr[iArg];
      } /*Else If: The user provided an output file*/

      else if(! cStrEql("-out-sam", parmStr, '\0'))
         outFlagC[0] = defSamPSam;

      else if(! cStrEql("-out-fastq", parmStr, '\0'))
         outFlagC[0] = defSamPFastq;

      else if(! cStrEql("-out-fasta", parmStr, '\0'))
         outFlagC[0] = defSamPFasta;

      else if(! cStrEql("-out-stats", parmStr, '\0'))
         outFlagC[0] = defSamPStats;

      else if(parmStr[0] == '-' && parmStr[1] == 'F')
      { /*Else if: this is a removal entry*/
         ++iArg;

         base10StrToUS(
            argsAryStr[iArg],
            rmAryS[*numRmFlagsS]
         );

         ++(*numRmFlagsS);
      } /*Else if: this is a removal entry*/

      else if(parmStr[0] == '-' && parmStr[1] == 'f')
      { /*Else if: this is a removal entry*/
         ++iArg;

         base10StrToUS(
            argsAryStr[iArg],
            keepAryS[*numKeepFlagsS]
         );

         ++(*numKeepFlagsS);
      } /*Else if: this is a removal entry*/

      else if(! cStrEql("-min-len", parmStr, '\0'))
      { /*Else If: the min length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *minLenUI);
      } /*Else If: the min length was input*/

      else if(! cStrEql("-max-len", parmStr, '\0'))
      { /*Else If: the max length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *maxLenUI);
      } /*Else If: the max length was input*/

      else if(! cStrEql("-min-aln-len", parmStr, '\0'))
      { /*Else If: the min aligned length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *minAlnLenUI);
      } /*Else If: the min aligned length was input*/

      else if(! cStrEql("-min-median-q", parmStr, '\0'))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *minMedianQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(! cStrEql("-min-mean-q", parmStr, '\0'))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *minMeanQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(! cStrEql("-min-mapq", parmStr, '\0'))
      { /*Else If: the min mapq was input*/
         ++iArg;
         base10StrToUC(argsAryStr[iArg], *minMapqUC);
      } /*Else If: the min mapq was input*/

   
      else if(! cStrEql("-trim", parmStr, '\0'))
         *trimReadsBl = 1;
      else if(! cStrEql("-no-trim", parmStr, '\0'))
         *trimReadsBl = 0;

      else return parmStr;

      ++iArg;
   } /*Loop: Get user input*/

   return 0;
} /*getFiltSamInput*/


/*-------------------------------------------------------\
| Fun-02: pHelp_filtsam
|   - Prints the help message for filtsam
| Input:
|   - outFILE:
|     o File to print help message to
| Output:
|   - Prints:
|     o Help message to outFILE
\-------------------------------------------------------*/
void
pHelp_filtsam(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC:
   '   o fun-02 sec-01:
   '     - Usage block of help message
   '   o fun-02 sec-02:
   '     - Input block of help message
   '   o fun-02 sec-03:
   '     - Print the output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Usage block of help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "filtsam -sam reads.sam [options]\n");
   fprintf(outFILE, "  - Filters sam file from user\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Input block of help message
   ^   o fun-02 Sec-02 sub-01:
   ^     - Print the input header
   ^   o fun-02 Sec-02 sub-02:
   ^     - Print sam file input
   ^   o fun-02 Sec-02 sub-03:
   ^     - Print flag filtering options
   ^   o fun-02 Sec-02 sub-04:
   ^     - Print min, min aligned, and max length options
   ^   o fun-02 Sec-02 sub-05:
   ^     - Print min median Q, mean Q, and mapq options
   ^   o fun-02 Sec-02 sub-06:
   ^     - Print output options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 sec-02 Sub-01:
   *   - Print the input header
   \*****************************************************/

   fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun-02 sec-02 Sub-02:
   *   - Print sam file input
   \*****************************************************/

   fprintf(outFILE, "  -sam: [Required]\n");
   fprintf(outFILE, "    o Sam file to filter\n");

   /*****************************************************\
   * Fun-02 sec-02 Sub-03:
   *   - Print flag filtering options
   \*****************************************************/

   fprintf(outFILE, "  -F: [Nothing]\n");
   fprintf(outFILE, "    o Filter out reads with flag\n");
   fprintf(
      outFILE,
      "    o -F can be input multiple times\n"
   );

   fprintf(outFILE, "  -f: [Nothing]\n");
   fprintf(
      outFILE,
      "    o Keep only reads with input flags\n"
   );
   fprintf(
      outFILE,
      "    o -f can be input multiple times\n"
   );

   fprintf(outFILE, "  - Some usefull flags\n");
   fprintf(outFILE, "     o 0: forward (not as flag)\n");
   fprintf(outFILE, "     o 4: unmapped read\n");
   fprintf(outFILE, "     o 16: reverse complement\n");
   fprintf(outFILE, "     o 256: secondary alignment\n");
   fprintf(outFILE, "     o 2048: supplemental align\n");

   /*****************************************************\
   * Fun-02 sec-02 Sub-04:
   *   - Print min, min aligned, and max length options
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-aln-len: [%u]\n",
     def_minAlnLen_filtsam
   );

   fprintf(
      outFILE,
      "    o Minimum aligned read length to keep an read\n"
   );
   fprintf(
      outFILE,
      "  -min-len: [%u]\n",
     def_minLen_filtsam
   );

   fprintf(
      outFILE,
      "    o Minimum read length to keep an read\n"
   );

   fprintf(
      outFILE,
      "  -max-len: [%u]\n",
     def_maxLen_filtsam
   );

   fprintf(
      outFILE,
      "    o Maximum read length to keep an read\n"
   );

   /*****************************************************\
   * Fun-02 sec-02 Sub-05:
   *   - Print min median Q, mean Q, and mapq options
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-median-q: [%f]\n",
     def_minMedQ_filtsam
   );
   fprintf(
      outFILE,
      "    o Minimum median Q-score to keep an read\n"
   );

   fprintf(
      outFILE,
      "  -min-mean-q: [%f]\n",
     def_minMeanQ_filtsam
   );
   fprintf(
      outFILE,
      "    o Minimum mean Q-score to keep an read\n"
   );

   fprintf(
      outFILE,
      "  -min-mapq: [%i]\n",
     def_minMapq_filtsam
   );
   fprintf(
      outFILE,
      "    o Minimum mapping quality to keep an read\n"
   );

   /*****************************************************\
   * Fun-02 sec-02 Sub-06:
   *   - Print output options
   \*****************************************************/

   if(def_trimReadsBl_filtsam)
      fprintf(outFILE, "  -trim: [Yes]\n");
   else
      fprintf(outFILE, "  -trim: [No]\n");

   fprintf(
      outFILE,
      "    o Remove (trim) soft masked regions of reads\n"
   );
   fprintf(outFILE, "    o disble with -no-trim\n");

   fprintf(outFILE, "  -out: [stdout]\n");
   fprintf(outFILE, "    o File to print output to\n");

   if(defDefaultOut == defSamPSam)
      fprintf(outFILE, "  -out-sam: [Default]\n");
   else
      fprintf(outFILE, "  -out-sam: [No]\n");

   fprintf(outFILE, "    o Print -out as sam file\n");

   if(defDefaultOut == defSamPFastq)
      fprintf(outFILE, "  -out-fastq: [Default]\n");
   else
      fprintf(outFILE, "  -out-fastq: [No]\n");

   fprintf(outFILE, "    o Print -out as fastq file\n");

   if(defDefaultOut == defSamPFasta)
      fprintf(outFILE, "  -out-fasta: [Default]\n");
   else
      fprintf(outFILE, "  -out-fasta: [No]\n");

   fprintf(outFILE, "    o Print -out as fasta file\n");

   if(defDefaultOut == defSamPStats)
      fprintf(outFILE, "  -out-stats: [Default]\n");
   else
      fprintf(outFILE, "  -out-stats: [No]\n");

   fprintf(outFILE, "    o Print -out as stats file\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print the output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");
   fprintf(
      outFILE,
      "  - Prints filtered reads to selected file type\n"
   );
} /*pHelp_filtsam*/

/*-------------------------------------------------------\
| Fun-03: pversion_filtsam
|   - Prints out the version number for filtsam
| Input:
|   - outFILE:
|     o File to print the version numbe to
| Output:
|   - Prints;
|     o the version number to outFILE
\-------------------------------------------------------*/
void
pversion_filtsam(
   FILE *outFILE
){

   fprintf(
      outFILE,
      "filtsam version: %i-%02i-%02i\n",
      def_year_filtsam,
      def_month_filtsam,
      def_day_filtsam
   );
} /*pversion_filtsam*/

/*-------------------------------------------------------\
| Main:
|   - Filters sam files and converts sam output to the
|     desiered format
| Input:
| Output:
|   - Prints:
|     o Filtered sam filt to format speficified by
|       "-out-format"
\-------------------------------------------------------*/
int main(
   int lenArgsI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - Filters sam files and converts sam output to the
   '     desiered format
   '   o main Sec-01:
   '     - Variable declerations
   '   o main Sec-02:
   '     - Check user input
   '   o main Sec-03:
   '     - Check user supplied files
   '   o main Sec-04:
   '     - Filter the user input
   '   o main Sec-05:
   '     - Clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *samStr = 0;
   char *outStr = 0;
   char pFlagC = defDefaultOut;

   uchar checkFlagUC = 0;
   ushort rmAryS[256];
   uchar numRmFlagsS = 0;
   ushort keepAryS[256];
   uchar numKeepFlagsS = 0;

   char trimReadsBl = def_trimReadsBl_filtsam;

   uint minLenUI = def_minLen_filtsam;
   uint maxLenUI = def_maxLen_filtsam;
   uint minAlnLenUI = def_minAlnLen_filtsam;
   float minMedianQF = def_minMedQ_filtsam;
   float minMeanQF = def_minMeanQ_filtsam;
   uchar minMapqUC = def_minMapq_filtsam;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry samST;

   char errC = 0;    /*Error reporting*/
   char *inStr = 0;  /*Error reporting; getFiltSamInput*/
   char *buffStr = 0;
   ulong lenBuffUL = 0;
   char pHeadBl = 1;  /*For printing a stats tsv file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inStr =
      getFiltSamInput(
         lenArgsI,
         argsAryStr,
         &samStr,
         &outStr,
         &pFlagC,
         rmAryS,
         &numRmFlagsS,
         keepAryS,
         &numKeepFlagsS,
         &minLenUI,
         &maxLenUI,
         &minMedianQF,
         &minMeanQF,
         &minMapqUC,
         &minAlnLenUI,
         &trimReadsBl
      ); /*Get the user input*/

   if(inStr)
   { /*If: I had an error*/
      if(
            ! cStrEql(inStr, "-h", '\0')
         || ! cStrEql(inStr, "--h", '\0')
         || ! cStrEql(inStr, "help", '\0')
         || ! cStrEql(inStr, "-help", '\0')
         || ! cStrEql(inStr, "--help", '\0')
      ){ /*If: user wanted help message*/
         pHelp_filtsam(stdout);
         exit(0);
      } /*If: user wanted help message*/

      if(
            ! cStrEql(inStr, "-v", '\0')
         || ! cStrEql(inStr, "--v", '\0')
         || ! cStrEql(inStr, "-V", '\0')
         || ! cStrEql(inStr, "--V", '\0')
         || ! cStrEql(inStr, "version", '\0')
         || ! cStrEql(inStr, "-version", '\0')
         || ! cStrEql(inStr, "--version", '\0')
      ){ /*If: user wanted help message*/
         pversion_filtsam(stdout);
         exit(0);
      } /*If: user wanted help message*/

      pHelp_filtsam(stderr);
      fprintf(stderr, "%s is not valid\n", inStr);
      exit(-1);
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Check user supplied files
   ^   o main sec-03 sub-01:
   ^     - Check the sam file (input)
   ^   o main sec-03 sub-02:
   ^     - Check the output file (output)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Check the sam file (input)
   \*****************************************************/

   if(!samStr || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: The user supplied a sam file*/
      samFILE = fopen(samStr, "r");

      if(!samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
             stderr,
             "-sam %s could not be opened\n",
             samStr
         );

         exit(-1);
      } /*If: I could not open the sam file*/
   } /*Else: The user supplied a sam file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check the out file (output)
   \*****************************************************/

   if(!outStr || outStr[0] == '-') outFILE = stdout;

   else
   { /*Else: The user supplied a out file*/
      outFILE = fopen(outStr, "r");

      if(!outFILE)
      { /*If: I could not open the out file*/
         fprintf(
             stderr,
             "-out %s could not be opened\n",
             outStr
         );

         fclose(samFILE);
         exit(-1);
      } /*If: I could not open the out file*/
   } /*Else: The user supplied a out file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Filter the user input
   ^   o main sec-04 sub-01:
   ^     - Set up samEntry struct and read first line
   ^   o main sec-04 sub-02:
   ^     - Start loop and check if need to  print header
   ^   o main sec-04 sub-03:
   ^     - Filter reads by length, Q-scores, and mapq
   ^   o main sec-04 sub-04:
   ^     - Trim reads (if requested)
   ^   o main sec-04 sub-05:
   ^     - Check if read has flag user wanted to keep
   ^   o main sec-04 sub-06:
   ^     - Check if read has flag user wanted to discard
   ^   o main sec-04 sub-07:
   ^     - Print out kept reads
   ^   o main sec-04 sub-08:
   ^     - Get the next entry in the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-04 Sub-01:
   *   - Set up samEntry struct and read first line
   \*****************************************************/

   if(initSamEntry(&samST))
   { /*If: I had a memory error*/
      freeSamEntryStack(&samST);
      fprintf(stderr, "Memory error\n");

      fclose(samFILE);
      fclose(outFILE);

      exit(-1);
   } /*If: I had a memory error*/

   errC = readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);

   /*****************************************************\
   * Main Sec-04 Sub-02:
   *   - Start loop and check if need to  print header
   \*****************************************************/

   while(!errC)
   { /*Loop: Read in the sam file*/
      if(samST.extraStr[0] == '@')
      { /*If: I am on an header; check if need to print*/
         if(pFlagC == defSamPSam)
            pSamEntry(&samST,&buffStr,&lenBuffUL,outFILE);
      } /*If: I am on an header; check if need to print*/

      /**************************************************\
      * Main Sec-04 Sub-03:
      *   - Filter reads by length, Q-scores, and mapq
      \**************************************************/
         
      if(samST.readLenUI < minLenUI)
         goto nextLine_main_sec04_sub_08;

      if(samST.readLenUI > maxLenUI && maxLenUI > 0)
          goto nextLine_main_sec04_sub_08;
      
      if(samST.medianQF < minMedianQF)
         goto nextLine_main_sec04_sub_08;

      if(samST.meanQF < minMeanQF)
         goto nextLine_main_sec04_sub_08;

      if(samST.mapqUC < minMapqUC)
         goto nextLine_main_sec04_sub_08;

      if(samST.alnReadLenUI < minAlnLenUI)
         goto nextLine_main_sec04_sub_08;

      /**************************************************\
      * Main Sec-04 Sub-04:
      *   - Trim reads (if requested)
      \**************************************************/

      if(trimReadsBl)
         trimSamEntry(&samST);

      /**************************************************\
      * Main Sec-04 Sub-05:
      *   - Check if read has flag user wanted to keep
      \**************************************************/

      /*Ideally I would sort by flag and then reduce to
      ` the minimum flag
      */
      for(
         checkFlagUC = 0;
         checkFlagUC < numKeepFlagsS;
         ++checkFlagUC
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(   samST.flagUS & keepAryS[checkFlagUC]
            || samST.flagUS == keepAryS[checkFlagUC]
          ) goto printSam_main_sec04_sub07;
      } /*Loop: Check all keep read flags*/

      if(numKeepFlagsS > 0)
         goto nextLine_main_sec04_sub_08;

      /**************************************************\
      * Main Sec-04 Sub-06:
      *   - Check if read has flag user wanted to discard
      \**************************************************/

      for(
         checkFlagUC = 0;
         checkFlagUC < numRmFlagsS;
         ++checkFlagUC
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(   samST.flagUS & rmAryS[checkFlagUC]
            || samST.flagUS == rmAryS[checkFlagUC]
          ) goto nextLine_main_sec04_sub_08;
      } /*Loop: Check all keep read flags*/

      /**************************************************\
      * Main Sec-04 Sub-07:
      *   - Print out kept reads
      \**************************************************/

      printSam_main_sec04_sub07:;

      switch(pFlagC)
      { /*Switch; decide printing method*/
         case defSamPSam:
            pSamEntry(&samST,&buffStr,&lenBuffUL,outFILE);
            break;

         case defSamPFastq:
            pSamEntryAsFastq(&samST, outFILE);
            break;

         case defSamPFasta:
            pSamEntryAsFasta(&samST, outFILE);
            break;

         case defSamPStats:
            pSamEntryStats(&samST, &pHeadBl, outFILE);
            break;
      } /*Switch; decide printing method*/

      /**************************************************\
      * Main Sec-04 Sub-08:
      *   - Get the next entry in the sam file
      \**************************************************/

      nextLine_main_sec04_sub_08:

      errC =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   freeSamEntryStack(&samST);
   free(buffStr);
   fclose(samFILE);
   fclose(outFILE);

   exit(0);
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
