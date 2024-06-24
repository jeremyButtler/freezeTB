/*########################################################
# Name: tbSpoligo
#   - Identifies spoligotypes in TB samples (driver)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: getInput_tbSpoligo
'     - gets user input
'   o fun02: pHelp_tbSpoligo
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
#include "kmerFind.h"


/*.h files only*/
#include "../generalLib/ulCpStr.h"
#include "../generalLib/dataTypeShortHand.h"
#include "tbSpoligo-errors.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c #include "../memwater/memwater.h"
!   o .h #include "../generalLib/gen-shellSort.h"
!   o .h #include "../memwater/alnSeqDefaults.h"
!   o .h #include "../generalLib/base10StrToNum.h"
!   o .h #include "../generalLib/numToStr.h"
!   o .h #include "../generalLib/genMath.h"
!   o .h #include "../generalLib/ntToFiveBit.h"
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
   '   o main sec01:
   '     - Variable declerations
   '   o main sec02:
   '     - Process user input
   '   o main sec03:
   '     - Read in the spoligotype sequences
   '   o main sec04:
   '     - Read in the spoligotype database
   '   o main sec05:
   '     - Check reads for spoligotypes
   '   o main sec06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 0;
   schar errSC = 0;
   uint lenCodeSI = 64;
   uint codeAryUI[lenCodeSI]; /*Barcode for spoligo*/

   schar *seqFileStr = 0;    /*sequences to type*/
   schar *spoligoFileStr = 0;/*spoligotype Sequences*/
   schar *spoligoDbStr = 0;  /*Database of lineages*/
   schar *outFileStr = 0;    /*file to output to*/

   sint drStartSI = def_DRStart_tbSpoligo;
   sint drEndSI = def_DREnd_tbSpoligo;

   /*Type of sequence file input*/
   schar seqTypeFlagSC = def_seqStdin_tbSpoligo;

   schar fragCheckBl = def_frag_tbSpoligoCheckInput;
     /*do fragmentation checks*/

   schar conFragBl = def_conFrag_tbSpoligoCheckInput;
      /*consensus fragment mode*/

   uint numSupReadsUI = 0; /*number reads with an spacer*/

   float minPercScoreF = def_minPercScore_tbSpoligoWater;

   /*for kmer checks*/
   float minKmerPercF = def_minKmerPerc_kmerFind;
   float percShiftF = def_percShift_kmerFind;
   float percExtraNtInWinF = def_extraNtInWin_kmerFind;
   uchar lenKmerUC = def_lenKmer_kmerFind;
   schar fastBl = def_fastSeach_tbSpoligoCheckInput;

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

   /*kmer finding of spoligotype variables*/
   struct tblST_kmerFind kmerTblStackST;
   struct refST_kmerFind *kmerRefAryST = 0;

   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - Process user input and intialize variables
   ^   o main sec02 sub01:
   ^     - Extract the user input
   ^   o main sec02 sub02:
   ^     - initialize variables
   ^   o main sec02 sub03:
   ^     - Check if I can open spoligotype file
   ^   o main sec02 sub04:
   ^     - Check if I can open the output file
   ^   o main sec02 sub05:
   ^     - Check if I can open the sequence file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
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
            &minKmerPercF,
            &fastBl,
            &fragCheckBl,
            &conFragBl
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
   * Main Sec02 Sub02:
   *   - initialize variables
   \*****************************************************/

   init_tblST_kmerFind(
      &kmerTblStackST,
      lenKmerUC
   );

   for(
      siArg = 0;
      siArg < lenCodeSI;
      ++siArg
   ) codeAryUI[siArg] = 0;

   initSeqST(&seqStackST);
   initAlnSet(&alnSetStackST);

   errSC = (schar) initSamEntry(&samStackST);

   if(errSC)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory\n");
      goto errCleanUp_main_sec06_sub02;
   } /*If: I had an memory error*/

   /*****************************************************\
   * Main Sec02 Sub03:
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
   * Main Sec02 Sub04:
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
   * Main Sec02 Sub05:
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
   ^ Main Sec03:
   ^   - Read in the spoligotype sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fastBl)
   { /*If: I am doing the kmer method*/
      kmerRefAryST =
         mkAry_refST_kmerFind(
           spoligoFileStr,
           lenKmerUC,
           &numSpoligosSI,
           minKmerPercF,
           &kmerTblStackST,
           percExtraNtInWinF,
           percShiftF,
           &alnSetStackST,
           (uchar *) &errSC
      );
   } /*If: I am doing the kmer method*/

   else
   { /*Else: I am doing the waterman method*/
      spoligoHeapAryST =
        getSpoligoRefs(
           spoligoFileStr,
           &numSpoligosSI,
           &errSC
        );
   } /*Else: I am doing the waterman method*/

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
   ^ Main Sec04:
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
   ^ Main Sec05:
   ^   - Check reads for spoligotypes
   ^   o main sec05 sub01:
   ^     - print header
   ^   o main sec05 sub02:
   ^     - fastx file spoligotype detection
   ^   o main sec05 sub03:
   ^     - sam file spoligotype detection
   ^   o main sec05 sub04:
   ^     - print out fragment check results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - print header
   \*****************************************************/

   pSpoligoHead(
      (!conFragBl) & fragCheckBl,
      outFILE
   );

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - fastx file spoligotype detection
   *   o main sec05 sub02 cat01:
   *     - Check if fastx file and read in first entry
   *   o main sec05 sub02 cat02:
   *     - Look for spoligotypes for each entry in fastx
   *   o main sec05 sub02 cat03:
   *     - Report fastx errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat01:
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
      + Main Sec05 Sub02 Cat02:
      +   - Look for spoligotypes for each entry in fastx
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(errSC == 1)
      { /*Loop: Check read for spoligotypes*/

         if(fastBl)
         { /*If: I am doing the faster kmer method*/
            errSC =
               (schar)
               fxFindSpoligos_kmerFind(
                  &kmerTblStackST,
                  kmerRefAryST,
                  numSpoligosSI,
                  &seqStackST,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*find spoligotype with kmer search*/
         } /*If: I am doing the faster kmer method*/

         else
         { /*Else: I am doing the slower waterman search*/
            errSC =
               fxSpoligoSearch(
                  &seqStackST,
                  spoligoHeapAryST,
                  numSpoligosSI,
                  minPercScoreF,
                  codeAryUI,
                  1, /*Print progress*/
                  fragCheckBl,
                  &alnSetStackST
               ); /*Detect spoligotypes*/
         } /*Else: I am doing the slower waterman search*/

         if(errSC & memErr_tbSpoligo)
         { /*If: I had an memory error*/
            fprintf(stderr, "Ran out of memory\n");
            goto errCleanUp_main_sec06_sub02;
         } /*If: I had an memory error*/

         if(! errSC)
         { /*If: I detected an spoligotype*/
            if(! fragCheckBl)
            { /*If: I am not doing fragment checks*/
               pSpoligo(
                  (schar *) seqStackST.idStr,
                  codeAryUI,
                  (!conFragBl) & fragCheckBl,
                  numSupReadsUI,
                  lineageHeapAryST,
                  numLineagesSI,
                  outFILE
               );
            } /*If: I am not doing fragment checks*/

            /*add read to total sup read count*/
            ++numSupReadsUI;
         } /*If: I detected an spoligotype*/

         ++siArg;

         if(seqTypeFlagSC == def_fqFile_tbSpoligo)
            errSC= (schar) readFqSeq(inFILE, &seqStackST);
         else
            errSC= (schar) readFaSeq(inFILE, &seqStackST);
      } /*Loop: Check read read for spoligotypes*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec05 Sub02 Cat03:
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
   * Main Sec05 Sub03:
   *   - sam file spoligotype detection
   *   o main sec05 sub03 cat01:
   *     - Read in the first sam file entry
   *   o main sec05 sub03 cat02:
   *     - Look for spoligotypes for each sam entry
   *   o main sec05 sub03 cat03:
   *     - Report sam file errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub03 Cat01:
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
      + Main Sec05 Sub03 Cat02:
      +   - Look for spoligotypes for each sam entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: Check read read for spoligotypes*/
         if(*samStackST.seqStr == '\0')
            goto nextSamEntry_main_sec04_sub03_cat02;

         if(fastBl)
         { /*If: I am doing the faster kmer method*/
            errSC =
               (schar)
               samFindSpoligos_kmerFind(
                  &kmerTblStackST,
                  kmerRefAryST,
                  numSpoligosSI,
                  &samStackST,
                  drStartSI,
                  drEndSI,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*find spoligotype with kmer search*/
         } /*If: I am doing the faster kmer method*/

         else
         { /*Else: I am doing the slower waterman*/
            errSC =
               samSpoligoSearch(
                  &samStackST,
                  spoligoHeapAryST,
                  numSpoligosSI,
                  drStartSI,
                  drEndSI,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*Detect spoligotypes*/
         } /*Else: I am doing the slower waterman*/

         if(errSC == memErr_tbSpoligo)
         { /*If: I had an memory error*/
            goto errCleanUp_main_sec06_sub02;
         } /*If: I had an memory error*/

         if(! errSC)
         { /*If: I could detect spoligotypes*/
            if(! fragCheckBl)
            { /*If: I am not checking for fragments*/
               pSpoligo(
                  (schar *) samStackST.qryIdStr,
                  codeAryUI,
                  (!conFragBl) & fragCheckBl,
                  numSupReadsUI,
                  lineageHeapAryST,
                  numLineagesSI,
                  outFILE
               );
            } /*If: I am not checking for fragments*/

            /*add read to total sup read count*/
            ++numSupReadsUI;
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
      + Main Sec05 Sub03 Cat03:
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

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - print out fragment check results
   \*****************************************************/

   if(fragCheckBl && ! conFragBl)
   { /*If: I am did fragment checks*/
      pSpoligo(
         (schar *) seqFileStr,
         codeAryUI,
         1,                 /*do not output lineage*/
         numSupReadsUI,
         lineageHeapAryST,
         numLineagesSI,
         outFILE
      );
   } /*If: I am did fragment checks*/

   else if(conFragBl)
   { /*Else: I am working with consensus fragments*/
      pSpoligo(
         (schar *) samStackST.qryIdStr,
         codeAryUI,
         0,     /*I want to output the lineage*/
         numSupReadsUI,
         lineageHeapAryST,
         numLineagesSI,
         outFILE
      );
   } /*Else: I am working with consensus fragments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - Clean up
   ^   o main sec06 sub01:
   ^     - Clean up after no problems
   ^   o main sec06 sub02:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - Clean up after no problems
   \*****************************************************/

   /*check if did an slow waterman or fast kmer*/
   if(spoligoHeapAryST)
      freeSeqAryST(spoligoHeapAryST, numSpoligosSI);

   else
      freeHeapAry_refST_kmerFind(
         kmerRefAryST,
         numSpoligosSI
      );

   spoligoHeapAryST = 0;
   kmerRefAryST = 0;

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSamEntryStack(&samStackST);
   freeSeqSTStack(&seqStackST);
   freeAlnSetStack(&alnSetStackST);

   freeStack_tblST_kmerFind(&kmerTblStackST);

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
   * Main Sec06 Sub02:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_main_sec06_sub02:;

   /*check if did an slow waterman or fast kmer*/
   if(spoligoHeapAryST)
      freeSeqAryST(spoligoHeapAryST, numSpoligosSI);

   else
      freeHeapAry_refST_kmerFind(
         kmerRefAryST,
         numSpoligosSI
      );

   spoligoHeapAryST = 0;
   kmerRefAryST = 0;

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSamEntryStack(&samStackST);
   freeSeqSTStack(&seqStackST);
   freeAlnSetStack(&alnSetStackST);

   freeStack_tblST_kmerFind(&kmerTblStackST);

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
