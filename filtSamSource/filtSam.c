/*########################################################
# Name: filtSam
#  - Filters sam files (TODO: Add this in)
#  - This is a little slower than samtools.
# Libraries
#   o "../generalLib/samEntryStruct.h"     (No .c file)
#   o "../generalLib/base10StrToNum.h"     (No .c file)
#   o "../generalLib/dataTypeShortHand.h"  (No .c file)
#   o "../generalLib/ulCpStr.h"            (No .c file)
#   o "../generalLib/numToStr.h"           (No .c file)
# C Standard Libraries:
#   o <string.h>
#   o <stdlib.h>
#   o <stdio.h>
#   o <limits.h>
# Note:
#  - End of file has some general sam file basics
########################################################*/

#include "../generalLib/samEntryStruct.h"
#include <string.h>


#define defSamPSam 0
#define defSamPFastq 1
#define defSamPFasta 2
#define defSamPStats 3

#define defDefaultOut defSamPSam

char * getSamFiltInput(
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
   uint *minAlnLenUI
); /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: getSamFiltInput
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main(
   int lenArgsI,
   char *argsAryStr[]
){
   char *samStr = 0;
   char *outStr = 0;
   char pFlagC = defDefaultOut;

   uchar checkFlagUC = 0;
   ushort rmAryS[256];
   uchar numRmFlagsS = 0;
   ushort keepAryS[256];
   uchar numKeepFlagsS = 0;


   uint minLenUI = 0;
   uint maxLenUI = 0;
   float minMedianQF = 0;
   float minMeanQF = 0;
   uchar minMapqUC = 0;
   uint minAlnLenUI = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry samST;

   char errC = 0;    /*Error reporting*/
   char *inStr = 0;  /*Error reporting; getSamFiltInput*/
   char *buffStr = 0;
   ulong lenBuffUL = 0;
   char pHeadBl = 1;  /*For printing a stats tsv file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inStr =
      getSamFiltInput(
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
         &minAlnLenUI
      ); /*Get the user input*/

   if(inStr)
   { /*If: I had an error*/
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
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(initSamEntry(&samST))
   { /*If: I had a memory error*/
      freeSamEntryStack(&samST);
      fprintf(stderr, "Memory error\n");

      fclose(samFILE);
      fclose(outFILE);

      exit(-1);
   } /*If: I had a memory error*/

   errC = readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);

   while(!errC)
   { /*Loop: Read in the sam file*/
      if(samST.readLenUI < minLenUI) goto samGetNextLine;

      if(samST.readLenUI > maxLenUI && maxLenUI > 0)
          goto samGetNextLine;
      
      if(samST.medianQF < minMedianQF)
         goto samGetNextLine;

      if(samST.meanQF < minMeanQF) goto samGetNextLine;
      if(samST.mapqUC < minMapqUC) goto samGetNextLine;

      if(samST.alnReadLenUI < minAlnLenUI)
         goto samGetNextLine;

      /*Were I would check use flags*/
      /*TODO: Check read lengths*/
      /*TODO: call trimSam*/
      /*TODO: Check Q-scores/snps/indels/dels*/

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
          ) goto printSam;
      } /*Loop: Check all keep read flags*/

      if(numKeepFlagsS > 0) goto samGetNextLine;

      for(
         checkFlagUC = 0;
         checkFlagUC < numRmFlagsS;
         ++checkFlagUC
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(   samST.flagUS & rmAryS[checkFlagUC]
            || samST.flagUS == rmAryS[checkFlagUC]
          ) goto samGetNextLine;
      } /*Loop: Check all keep read flags*/

      printSam:;

      switch(pFlagC)
      { /*Switch; decide printing method*/

         case defSamPSam:
            pSamEntry(&samST,buffStr,lenBuffUL,outFILE);
            break;

         case defSamPFastq:
            pSamEntryAsFastq(&samST, outFILE);
            break;

         case defSamPFasta:
            pSamEntryAsFasta(&samST, outFILE);
            break;

         case defSamPStats:
            pSamEntryStats(&samST, pHeadBl, outFILE);
            break;
      } /*Switch; decide printing method*/


      samGetNextLine:

      errC =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the sam file*/

   freeSamEntryStack(&samST);
   free(buffStr);
   fclose(samFILE);
   fclose(outFILE);

   exit(0);
}

char * getSamFiltInput(
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
   uint *minAlnLenUI
){
   char *parmStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < lenArgsI; ++iArg)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      if(!strcmp("-sam", parmStr))
      { /*If: The user input a sam file*/
         ++iArg;
         *samStr = argsAryStr[iArg];
      } /*If: The user input a sam file*/

      else if(!strcmp("-out", parmStr))
      { /*Else If: The user provided an output file*/
         ++iArg;
         *outStr = argsAryStr[iArg];
      } /*Else If: The user provided an output file*/

      else if(!strcmp("-out-sam", parmStr))
         outFlagC[0] = defSamPSam;

      else if(!strcmp("-out-fastq", parmStr))
         outFlagC[0] = defSamPFastq;

      else if(!strcmp("-out-fasta", parmStr))
         outFlagC[0] = defSamPFasta;

      else if(!strcmp("-out-stats", parmStr))
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

      else if(!strcmp("-min-len", parmStr))
      { /*Else If: the min length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *minLenUI);
      } /*Else If: the min length was input*/

      else if(!strcmp("-max-len", parmStr))
      { /*Else If: the max length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *maxLenUI);
      } /*Else If: the max length was input*/

      else if(!strcmp("-min-aln-len", parmStr))
      { /*Else If: the min aligned length was input*/
         ++iArg;
         base10StrToUI(argsAryStr[iArg], *minAlnLenUI);
      } /*Else If: the min aligned length was input*/

      else if(!strcmp("-min-median-q", parmStr))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *minMedianQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(!strcmp("-min-mean-q", parmStr))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *minMeanQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(!strcmp("-min-mapq", parmStr))
      { /*Else If: the min mapq was input*/
         ++iArg;
         base10StrToUC(argsAryStr[iArg], *minMapqUC);
      } /*Else If: the min mapq was input*/

      else return parmStr;
   } /*Loop: Get user input*/

   return 0;
} /*getSamFiltInput*/
