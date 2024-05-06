/*########################################################
# Name: tbSpoligo
#   - Identifies spoligotypes in TB samples (driver)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun-01: getInput_tbSpoligo
'     - gets user input
'   o fun-02: pHelp_tbSpoligo
'     - prints the help message to an file
'   o main:
'     - driver function (runs the spoligotyping)
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

#include "tbSpoligo-readDb.h"
#include "tbSpoligo-memwater-fun.h"
#include "../memwater/seqStruct.h"
#include "../memwater/alnSetStruct.h"

#include "../generalLib/samEntryStruct.h"

#include "tbSpoligo-checkInput.h"
#include "tbSpoligo-version.h"


/*.h files only*/
#include "../generalLib/ulCpStr.h"
#include "../generalLib/dataTypeShortHand.h"
#include "tbSpoligo-errors.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c #include "../memwater/memwater.h"
!   o .h #include "../memwater/alnSeqDefaults.h"
!   o .h #include "../generalLib/base10StrToNum.h"
!   o .h #include "../generalLib/numToStr.h"
!   o .h #include "../generalLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Main:
|   - driver function for tbSpoligo
| Input:
|   o argsAryStr:
|     - Array of c-strings the user input
|     - see the help message for an list of options
|   o numArgsSI:
|     - Length of argsAryStr
| Output:
|   - Prints:
|     o Spoligotypes to specifed files
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   signed int numArgsSI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   -
   '   o main sec-01:
   '     - Variable declerations
   '   o main sec-02:
   '     - Process user input
   '   o main sec-03:
   '     - Read in the spoligotype sequences
   '   o main sec-04:
   '     - Read in the spoligotype database
   '   o main sec-05:
   '     - Check reads for spoligotypes
   '   o main sec-06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 0;
   schar errSC = 0;
   schar barcodeStr[256]; /*Barcode for spoligo*/

   schar *seqFileStr = 0;    /*sequences to type*/
   schar *spoligoFileStr = 0;/*spoligotype Sequences*/
   schar *spoligoDbStr = 0;  /*Database of lineages*/
   schar *outFileStr = 0;    /*file to output to*/

   sint drStartSI = def_DRStart_tbSpoligo;
   sint drEndSI = def_DREnd_tbSpoligo;

   /*Type of sequence file input*/
   schar seqTypeFlagSC = def_seqStdin_tbSpoligo;

   float minPercScoreF = def_minPercScore_tbSpoligoWater;
   sint minKmersSI = 1;     /*Not set up yet*/ 
   schar fastBl = 0;        /*For kmer, not set up yet*/

   struct seqStruct *spoligoHeapAryST= 0;/*ref sequences*/
   sint numSpoligosSI = 0;

   struct seqStruct seqStackST;     /*Sequences to check*/
   struct alnSet alnSetStackST;     /*Settings*/

   struct samEntry samStackST;      /*Sequences to check*/
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;


   /*Varaibles dealng with the spoligo lineage database*/
   struct spoligoST *lineageHeapAryST = 0;
   sint numLineagesSI = 0; /*Number lineages in database*/

   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Process user input
   ^   o main sec-02 sub-01:
   ^     - Extract the user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-02 Sub-01:
   *   - Extract the user input
   \*****************************************************/

   siArg = 1;

   while(siArg < numArgsSI)
   { /*Loop: Check user input*/
      errSC =
         checkInput_tbSpoligo(
            (schar *) argsAryStr[siArg],
            (schar *) argsAryStr[siArg + 1],
            &seqFileStr,
            &seqTypeFlagSC,
            &spoligoFileStr,
            &spoligoDbStr,
            &outFileStr,
            &minPercScoreF,
            &drStartSI,
            &drEndSI,
            &minKmersSI,
            &fastBl
         );

      if(errSC & (~1))
      { /*If: I had an error*/
         if(errSC & def_phelp_tbSpoligo)
         { /*If: I need to print the help message*/
            phelp_tbSpoligo(0);
            exit(0);
         } /*If: I need to print the help message*/

         if(errSC & def_pversion_tbSpoligo)
         { /*If: I need to print the version number*/
            pVersion_tbSpoligo(0);
            exit(0);
         } /*If: I need to print the version number*/

         if(errSC & def_numericErr_tbSpoligo)
         { /*If: I need to print the version number*/
            fprintf(
               stderr,
               "%s from %s is non-numeric\n",
               argsAryStr[siArg + 1],
               argsAryStr[siArg]
            );

            exit(1);
         } /*If: I need to print the version number*/

        phelp_tbSpoligo((schar *) "2");

        fprintf(
           stderr,
           "%s is not recongnized\n",
           argsAryStr[siArg]
        );

        exit(2);
      } /*If: I had an error*/

      siArg += 1 + (errSC & 1);
   } /*Loop: Check user input*/

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Check if I can open spoligotype file
   \*****************************************************/

   inFILE = fopen((char *) spoligoFileStr, "r");

   if(! inFILE)
   { /*If: The spoligotype file could not be opened*/
      fprintf(
         stderr,
         "Unable to open -spoligo %s\n",
         spoligoFileStr
      );

      goto errCleanUp_main_sec06_sub02;
   } /*If: The spoligotype file could not be opened*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Check if I can open the output file
   \*****************************************************/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: An output file was provided*/
      outFILE = fopen((char *) outFileStr, "w");

      if(! outFILE)
      { /*If: The output file could not be opened*/
         fprintf(
            stderr,
            "Unable to open -out %s\n",
            outFileStr
         );

         goto errCleanUp_main_sec06_sub02;
      } /*If: The output file could not be opened*/
   } /*Else: An output file was provided*/

   /*****************************************************\
   * Main Sec-02 Sub-04:
   *   - Check if I can open the sequence file
   \*****************************************************/

   if(! seqFileStr || *seqFileStr == '-')
      inFILE = stdin;

   else
   { /*Else: An input file was provided*/
      inFILE = fopen((char *) seqFileStr, "r");

      if(! inFILE)
      { /*If: The input file could not be opened*/
         fprintf(
            stderr,
            "Unable to open - %s\n",
            seqFileStr
         );

         if(seqTypeFlagSC == def_fqFile_tbSpoligo)
            fprintf(stderr, "-fq %s\n", seqFileStr);

         if(seqTypeFlagSC == def_faFile_tbSpoligo)
            fprintf(stderr, "-fa %s\n", seqFileStr);

         if(seqTypeFlagSC == def_samFile_tbSpoligo)
            fprintf(stderr, "-sam %s\n", seqFileStr);

         if(seqTypeFlagSC == def_samConFile_tbSpoligo)
            fprintf(stderr, "-sam-con %s\n", seqFileStr);

         goto errCleanUp_main_sec06_sub02;
      } /*If: The input file could not be opened*/
   } /*Else: An input file was provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Read in the spoligotype sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   spoligoHeapAryST =
     getSpoligoRefs(
        spoligoFileStr,
        &numSpoligosSI,
        &errSC
     );

   if(errSC)
   { /*If: I had an error*/
      if(errSC == fileErr_tbSpoligo)
      { /*If: I had an file error*/
         fprintf(
            stderr,
            "-spoligo %s has a non-fasta entry\n",
            spoligoFileStr
        );
      } /*If: I had an file error*/

      else
      { /*Else: This is an memory error*/
         fprintf(
            stderr,
            "Ran out of memory getting spoligo seq\n"
        );
      } /*Else: This is an memory error*/

      goto errCleanUp_main_sec06_sub02;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Read in the spoligotype database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(spoligoDbStr)
   { /*If: An database of lineages was input*/
      lineageHeapAryST =
         readSpoligoDb(
            spoligoDbStr,
            &numLineagesSI,
            &errSC
      );

      if(errSC)
      { /*If: I had an error*/
         if(errSC == fileErr_tbSpoligo)
         { /*If: I had an file error*/
            fprintf(
               stderr,
               "Could not open -db %s\n",
               spoligoDbStr
            );
         } /*If: I had an file error*/

         else
         { /*Else: I had an memory error*/
            fprintf(
               stderr,
               "Ran out of memory reading -db %s\n",
               spoligoDbStr
            );
         } /*Else: I had an memory error*/

         goto errCleanUp_main_sec06_sub02;
      } /*If: I had an error*/
   } /*If: An database of lineages was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Check reads for spoligotypes
   ^   o main sec-05 sub-01:
   ^     - Initialize variables and print header
   ^   o main sec-05 sub-02:
   ^     - fastx file spoligotype detection
   ^   o main sec-05 sub-03:
   ^     - sam file spoligotype detection
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-05 Sub-01:
   *   - Initialize variables and print header
   \*****************************************************/

   initSeqST(&seqStackST);
   initAlnSet(&alnSetStackST);

   errSC = (schar) initSamEntry(&samStackST);

   if(errSC)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory\n");
      goto errCleanUp_main_sec06_sub02;
   } /*If: I had an memory error*/

   pSpoligoHead(outFILE);

   /*****************************************************\
   * Main Sec-05 Sub-02:
   *   - fastx file spoligotype detection
   *   o main sec-05 sub-02 cat-01:
   *     - Check if fastx file and read in first entry
   *   o main sec-05 sub-02 cat-02:
   *     - Look for spoligotypes for each entry in fastx
   *   o main sec-05 sub-02 cat-03:
   *     - Report fastx errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-05 Sub-02 Cat-01:
   +   - Check if fastx file and read in first entry
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
        seqTypeFlagSC
      & (def_fqFile_tbSpoligo | def_faFile_tbSpoligo)
   ){ /*If: I am checking an fastq file*/
      if(seqTypeFlagSC == def_fqFile_tbSpoligo)
         errSC = (schar) readFqSeq(inFILE, &seqStackST);
      else
         errSC = (schar) readFaSeq(inFILE, &seqStackST);

      siArg = 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec-05 Sub-02 Cat-02:
      +   - Look for spoligotypes for each entry in fastx
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(errSC == 1)
      { /*Loop: Check read for spoligotypes*/
         errSC =
            fxSpoligoSearch(
               &seqStackST,
               spoligoHeapAryST,
               numSpoligosSI,
               minPercScoreF,
               barcodeStr,
               1, /*Print progress*/
               &alnSetStackST
            ); /*Detect spoligotypes*/

         if(errSC)
         { /*If: I had an memory error*/
            fprintf(stderr, "Ran out of memory\n");
            goto errCleanUp_main_sec06_sub02;
         } /*If: I had an memory error*/

         pSpoligo(
            (schar *) seqStackST.idStr,
            barcodeStr,
            lineageHeapAryST,
            numLineagesSI,
            outFILE
         );

         ++siArg;

         if(seqTypeFlagSC == def_fqFile_tbSpoligo)
            errSC= (schar) readFqSeq(inFILE, &seqStackST);
         else
            errSC= (schar) readFqSeq(inFILE, &seqStackST);
      } /*Loop: Check read read for spoligotypes*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec-05 Sub-02 Cat-03:
      +   - Report fastx errors
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errSC)
      { /*If: I had an error*/
         if(seqTypeFlagSC == def_fqFile_tbSpoligo)
         { /*If: it was an fastq file error*/
            fprintf(
               stderr,
               "Invalid entry %i in -fq %s\n",
               siArg,
               seqFileStr
            );
         } /*If: it was an fastq file error*/

         else
         { /*Else: it was an fasta file error*/
            fprintf(
               stderr,
               "Invalid entry %i in -fa %s\n",
               siArg,
               seqFileStr
            );
         } /*Else: it was an fasta file error*/

         goto errCleanUp_main_sec06_sub02;
      } /*If: I had an error*/
   } /*If: I am checking an fastq file*/

   /*****************************************************\
   * Main Sec-05 Sub-03:
   *   - sam file spoligotype detection
   *   o main sec-05 sub-03 cat-01:
   *     - Read in the first sam file entry
   *   o main sec-05 sub-03 cat-02:
   *     - Look for spoligotypes for each sam entry
   *   o main sec-05 sub-03 cat-03:
   *     - Report sam file errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-05 Sub-03 Cat-01:
   +   - Read in the first sam file entry
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: I am checking an sam file for spoligotypes*/
      errSC =
         (schar)
         readSamLine(
            &samStackST,
            (char **) &buffHeapStr,
            &lenBuffUL,
            inFILE
         );

      siArg = 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec-05 Sub-03 Cat-02:
      +   - Look for spoligotypes for each sam entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: Check read read for spoligotypes*/
         if(*samStackST.seqStr == '\0')
            goto nextSamEntry_main_sec04_sub03_cat02;

         errSC =
            samSpoligoSearch(
               &samStackST,
               spoligoHeapAryST,
               numSpoligosSI,
               drStartSI,
               drEndSI,
               minPercScoreF,
               barcodeStr,
               &alnSetStackST
            ); /*Detect spoligotypes*/

         if(errSC == memErr_tbSpoligo)
         { /*If: I had an memory error*/
            goto errCleanUp_main_sec06_sub02;
         } /*If: I had an memory error*/

         if(! errSC)
         { /*If: I could detect spoligotypes*/
            pSpoligo(
               (schar *) samStackST.qryIdStr,
               barcodeStr,
               lineageHeapAryST,
               numLineagesSI,
               outFILE
            );
         } /*If: I could detect spoligotypes*/

         ++siArg;

         nextSamEntry_main_sec04_sub03_cat02:;

         errSC =
            (schar)
            readSamLine(
               &samStackST,
               (char **) &buffHeapStr,
               (ulong *) &lenBuffUL,
               inFILE
            );
      } /*Loop: Check read read for spoligotypes*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec-05 Sub-03 Cat-03:
      +   - Report sam file errors
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errSC != 1)
      { /*If: I had an error*/
         fprintf(
            stderr,
            "Invalid entry %i in -sam %s\n",
            siArg,
            seqFileStr
         );

         goto errCleanUp_main_sec06_sub02;
      } /*If: I had an error*/
   } /*Else: I am checking an sam file for spoligotypes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-06:
   ^   - Clean up
   ^   o main sec-06 sub-01:
   ^     - Clean up after no problems
   ^   o main sec-06 sub-02:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-06 Sub-01:
   *   - Clean up after no problems
   \*****************************************************/

   freeSeqAryST(spoligoHeapAryST, numSpoligosSI);
   spoligoHeapAryST = 0;

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSamEntryStack(&samStackST);
   freeSeqSTStack(&seqStackST);
   freeAlnSetStack(&alnSetStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   if(inFILE)
      fclose(inFILE);

   inFILE = 0;

   if(outFILE)
      fclose(outFILE);

   outFILE = 0;

   exit(0);

   /*****************************************************\
   * Main Sec-06 Sub-02:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_main_sec06_sub02:;

   freeSeqAryST(spoligoHeapAryST, numSpoligosSI);
   spoligoHeapAryST = 0;

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSamEntryStack(&samStackST);
   freeSeqSTStack(&seqStackST);
   freeAlnSetStack(&alnSetStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   if(inFILE)
      fclose(inFILE);

   inFILE = 0;

   if(outFILE)
      fclose(outFILE);

   outFILE = 0;

   exit(4);
} /*main*/
