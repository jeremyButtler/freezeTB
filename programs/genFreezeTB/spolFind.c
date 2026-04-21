/*########################################################
# Name: spolFind
#   - holds functions for spoligotype detection
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: fx_spolFind
'     - finds spoligotype spacers in an sequence (from
'       fastx file) using an faster kmer search followed
'       by an slower waterman to finalize alignments
'   o fun02: sam_spolFind
'     - finds spoligotype spacers in an sequence using
'       an faster kmer search followed by an slower
'       waterman to finalize alignments
'   o fun03: getRefs_spolFind
'     - gets the sequence for each spoligotypes (for
'       waterman find only (fun04/fun05)
'   o fun04: fxwater_spolFind
'     - searches for spoligotypes in fastx sequences using
'       a waterman
'   o fun05: samWater_spolFind
'     - searches for spoligotypes in sam files using a
'       waterman alignment (slow)
'   o license:
'     - licensing for this code (CC0)
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

#include "../genLib/ulCp.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/memwater.h"
#include "../genAln/kmerFind.h"

/*.h files only (no .c files*/
#include "../genLib/endLine.h"
#include "../genAln/alnDefs.h"
#include "tbSpolDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genLib/inflate.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genLib/genMath.h" .h macro only
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: fx_spolFind
|   - finds spoligotype spacers in an sequence (from
|     fastx file) using an faster kmer search followed by
|     an slower waterman to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       alnSetSTPtr and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - seqSTPtr:
|     o pointer to an seqST structure with the
|       sequence to check for spoligotypes in
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - def_noSpol_tbSpolDefs if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o def_noSpol_tbSpolDefs if no spoligotypes were found
\-------------------------------------------------------*/
signed char
fx_spolFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct seqST *seqSTPtr,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ^ Fun01 TOC:
   '   o fun01 sec01:
   '     - varaible declerations
   '   o fun01 sec02:
   '     - assign sequence to table
   '   o fun01 sec03:
   '     - check sequence for spacers
   '   o fun01 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - varaible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0; /*error messages*/
   signed char retSC = def_noSpol_tbSpolDefs; /*return*/

   unsigned char matchBl = 0;
   signed char firstTimeBl = 1;

   unsigned int uiSpoligo = 0;

   /*alignemnt variables; i ingore*/
   signed long scoreSL = 0;
   unsigned long qryStartUL = 0;
   unsigned long qryEndUL = 0;
   unsigned long refStartUL = 0;
   unsigned long refEndUL = 0;

   /*to keep the old assigned sequence*/
   struct seqST *oldSeqST = 0;

   /*code array for spoligotyping*/
   unsigned int bitAryUI[def_numSpol_tbSpolDefs + 1];
   unsigned int *spolAryUIPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - check positions and assign sequence to table
   ^   o fun01 sec02 sub01:
   ^     - see if i have an direct repeat region
   ^   o fun01 sec02 sub02:
   ^     - add the sequence to the kmer table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *   - see if i have an direct repeat region
   \*****************************************************/

   if(fragCheckBl)
   { /*If: only have direct repeat fragments*/
      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) bitAryUI[uiSpoligo] = 0; /*blank for new seq*/

      bitAryUI[uiSpoligo] = -1; /*mark end*/

      spolAryUIPtr = bitAryUI;
   } /*If: only have direct repeat fragments*/

   else
   { /*Else: read has full direct repeat region*/
      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] = 0; /*blank for new seq*/

      spolAryUIPtr = codeAryUI;
   } /*Else: read has full direct repeat region*/

   codeAryUI[lenRefAryUI] = -1;

   /*****************************************************\
   * Fun01 Sec02 Sub02:
   *   - add the sequence to the kmer table
   \*****************************************************/

   retSC = def_noSpol_tbSpolDefs; /*allows detection*/

   oldSeqST = tblSTPtr->seqSTPtr;
   tblSTPtr->seqSTPtr = seqSTPtr;

   blank_tblST_kmerFind(
      tblSTPtr,
      0          /*I do not want to blank the sequence*/
   );

   /*prepare for waterman alignment*/
   seqToIndex_alnSet(tblSTPtr->seqSTPtr->seqStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - check sequence for spacers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   do{
      errSC =
         nextSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         );

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ){ /*Loop: detect spoligotypes in each chunk*/
         matchBl =
            findRefInChunk_kmerFind(
               tblSTPtr,
               &(refSTAry[uiSpoligo]),
               alnSetSTPtr,
               minPercScoreF,
               &scoreSL,
               &qryStartUL,
               &qryEndUL,
               &refStartUL,
               &refEndUL
            );

         if(matchBl)
         { /*If: have a match*/
            spolAryUIPtr[uiSpoligo] = 1;
            retSC = 0;
         } /*If: have a match*/
      } /*Loop: detect spoligotypes in each chunk*/
   } while(! errSC);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fragCheckBl)
   { /*If: add detected fragments to code array*/
      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] += bitAryUI[uiSpoligo];
   } /*If: add detected fragments to code array*/

   /*reset table to original sequences*/
   indexToSeq_alnSet(tblSTPtr->seqSTPtr->seqStr);
   tblSTPtr->seqSTPtr = oldSeqST;

   return retSC;
} /*fx_spolFind*/

/*-------------------------------------------------------\
| Fun02: sam_spolFind
|   - finds spoligotype spacers in an sequence using
|     an faster kmer search followed by an slower waterman
|     to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       alnSetSTPtr and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       to check for spoligotypes in
|   - dirStartSI:
|     o position of first base in the direct repeat region
|   - dirEndSI:
|     o position of last base in the direct repeat region
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - def_noSpol_tbSpolDefs if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o def_noSpol_tbSpolDefs if no spoligotypes were found
\-------------------------------------------------------*/
signed char
sam_spolFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct samEntry *samSTPtr,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - varaible declerations
   '   o fun02 sec02:
   '     - assign sequence to table
   '   o fun02 sec03:
   '     - check sequence for spacers
   '   o fun02 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - varaible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char errSC = 0; /*error messages / return*/
   unsigned char retSC = def_noSpol_tbSpolDefs; /*return*/

   unsigned char swapBl = 0;
      /*swapped in sequence into table*/

   unsigned char matchBl = 0;
   signed char firstTimeBl = 1;

   /*to keep the old assigned sequence*/
   signed char *oldSeqStr = 0;
   unsigned long oldLenSeqUL = 0;

   unsigned int uiSpoligo = 0;

   /*alignemnt variables; i ingore*/
   signed long scoreSL = 0;
   unsigned long qryStartUL = 0;
   unsigned long qryEndUL = 0;
   unsigned long refStartUL = 0;
   unsigned long refEndUL = 0;

   /*variables for finding dr region*/
   signed int siCig = 0;
   signed int cigBaseOnSI = 0;
   signed int refPosSI = 0;
   signed int stopPosSI = 0;

   /*code array for spoligotyping*/
   unsigned int bitAryUI[def_numSpol_tbSpolDefs + 1];
   unsigned int *spolAryUIPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - check positions and assign sequence to table
   ^   o fun02 sec02 sub01:
   ^     - see if i have an direct repeat region
   ^   o fun02 sec02 sub02:
   ^     - add the sequence to the kmer table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - see if i have an direct repeat region
   \*****************************************************/

   if(fragCheckBl)
   { /*If: I am checking fragments*/
      if(samSTPtr->refEndUI < (unsigned int) dirStartSI)
         goto noSpoligo_fun02_sec04_sub02;

      if(samSTPtr->refStartUI > (unsigned int) dirEndSI)
         goto noSpoligo_fun02_sec04_sub02;

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) bitAryUI[uiSpoligo] = 0; /*blank for new seq*/

      bitAryUI[uiSpoligo] = -1; /*mark end*/

      spolAryUIPtr = bitAryUI;
   } /*If: I am checking fragments*/

   else
   { /*Else: I am checking full direct repeat regions*/
      /*dirStart and dirEnd might be index 1*/
      if(
           samSTPtr->refStartUI
         > (unsigned int) dirStartSI - 1
      ) goto noSpoligo_fun02_sec04_sub02;

      if(samSTPtr->refEndUI < (unsigned int) dirEndSI - 1)
         goto noSpoligo_fun02_sec04_sub02;

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] = 0; /*blank for new seq*/

      spolAryUIPtr = codeAryUI;
   } /*Else: I am checking full direct repeat regions*/

   codeAryUI[lenRefAryUI] = -1;

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - add the sequence to the kmer table
   \*****************************************************/

   swapBl = 1;
   oldSeqStr = (signed char *) tblSTPtr->seqSTPtr->seqStr;
   oldLenSeqUL = tblSTPtr->seqSTPtr->seqLenSL;

   blank_tblST_kmerFind(
      tblSTPtr,
      0          /*I do not want to blank the sequence*/
   );

   tblSTPtr->seqSTPtr->seqStr = samSTPtr->seqStr;
   tblSTPtr->seqSTPtr->seqLenSL = samSTPtr->readLenUI;
   
   cigBaseOnSI = samSTPtr->cigArySI[0];
   refPosSI = (signed int) samSTPtr->refStartUI;

   /*find the direct repeat start and end coordiantes*/
   findRefPos_samEntry(
      samSTPtr,
      &siCig,
      &cigBaseOnSI,
      dirStartSI,
      &refPosSI,
      &stopPosSI
   );

   tblSTPtr->seqPosUL = (unsigned long) stopPosSI;

   findRefPos_samEntry(
      samSTPtr,
      &siCig,
      &cigBaseOnSI,
      dirEndSI,
      &refPosSI,
      &stopPosSI
   );

   /*prepare for waterman alignment*/
   seqToIndex_alnSet(tblSTPtr->seqSTPtr->seqStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - check sequence for spacers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   do{
      errSC =
         nextSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         );

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ){ /*Loop: detect spoligotypes in each chunk*/
         matchBl =
            findRefInChunk_kmerFind(
               tblSTPtr,
               &(refSTAry[uiSpoligo]),
               alnSetSTPtr,
               minPercScoreF,
               &scoreSL,
               &qryStartUL,
               &qryEndUL,
               &refStartUL,
               &refEndUL
            );

         if(matchBl)
         { /*If: have a match*/
            spolAryUIPtr[uiSpoligo] = 1;
            retSC = 0;
         } /*If: have a match*/
      } /*Loop: detect spoligotypes in each chunk*/

      if(tblSTPtr->seqPosUL >= (unsigned long) stopPosSI)
         break;
   } while(! errSC);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - clean up
   ^   o fun02 sec04 sub01:
   ^     - no error
   ^   o fun02 sec04 sub02:
   ^     - memory error clean up
   ^   o fun02 sec04 sub03:
   ^     - general clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - no error
   \*****************************************************/

   if(fragCheckBl)
   { /*If: add detected fragments to code array*/
      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] += bitAryUI[uiSpoligo];
   } /*If: add detected fragments to code array*/

   errSC = retSC;
   goto cleanUp_fun02_sec04_sub04;

   /*****************************************************\
   * Fun02 Sec04 Sub02:
   *   - sequene out of bounds
   \*****************************************************/

   noSpoligo_fun02_sec04_sub02:;
   errSC = def_noSpol_tbSpolDefs;
   goto cleanUp_fun02_sec04_sub04;

   /*****************************************************\
   * Fun02 Sec04 Sub03:
   *   - memory error
   \*****************************************************/

   /*
   memErr_fun02_sec04_sub03:;
   errSC = memErr_tbSpoligo;
   goto cleanUp_fun02_sec04_sub04;
   */

   /*****************************************************\
   * Fun02 Sec04 Sub04:
   *   - general clean up
   \*****************************************************/

   cleanUp_fun02_sec04_sub04:;

   /*reset table to original sequences*/

   /*convert back to sequence*/
   if(swapBl)
   { /*If: I tried to detect spoligotypes*/
      indexToSeq_alnSet(tblSTPtr->seqSTPtr->seqStr);

      tblSTPtr->seqSTPtr->seqStr = oldSeqStr;
      tblSTPtr->seqSTPtr->seqLenSL = oldLenSeqUL;
   } /*If: I tried to detect spoligotypes*/

   return errSC;
} /*sam_spolFind*/

/*-------------------------------------------------------\
| Fun03: getRefs_spolFind
|   - gets the sequence for each spoligotypes; for
|     waterman find only (fun04/fun05)
| Input:
|   - spoligoFileStr:
|     o c-string with spoligotype sequences
|   - numSpoligosSIPtr:
|     o pointer to an int with the number of spoligotype
|       sequences in the returned spoligotype array
|   - errSCPtr:
|     o pointer to character to hold the error type
| Output:
|   - Modifies:
|     o numSpoligosSI to hold the number of spoligotypes
|       in the returned array
|     o errSCPtr to hold the error type
|       - 0 for no errors
|       - def_fileErr_tbSpolDefs for file errors
|       - def_memErr_tbSpolDefs for memory errors
|   - Returns:
|     o array of seqSTs for success
|     o 0 for an error
\-------------------------------------------------------*/
struct seqST *
getRefs_spolFind(
   signed char *spoligoFileStr,
   signed int *numSpoligosSIPtr,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets the sequence for each spoligotypes
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - initalize pointers and open spolitotype file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuffSS (2 << 10)
   signed char buffStr[lenBuffSS + 9];
   signed char *tmpStr = 0;
   unsigned long bytesReadUL = 0;

   signed char newLineBl = 1; /*1: On an new line*/

   signed int siSpoligo = 0;
      /*Counter for getting spoligotypes*/
   struct seqST *seqHeapAryST = 0;
   signed char errUC = 0;
   signed char cpErrSC = 0;

   FILE *spoligoFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initalize pointers and open spolitotype file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   *numSpoligosSIPtr = 0;

   spoligoFILE =
      fopen(
         (char *) spoligoFileStr,
         "r"
      );

   if(! spoligoFILE)
      goto fileErr_fun03_sec05_sub03;
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - Find the number of sequences in the file
   ^   o fun03 sec03 sub01:
   ^     - Get the first line of file and start loop
   ^   o fun03 sec03 sub02:
   ^     - Start loop & check if have new line in buffer
   ^   o fun03 sec03 sub03:
   ^     - If missing newline, get next part of file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - Get the first line of file and start loop
   \*****************************************************/

   bytesReadUL =
      fread(
         (char *) buffStr,
         sizeof(char),
         lenBuffSS,
         spoligoFILE
      ); /*Read in the file*/

   buffStr[bytesReadUL] = '\0';
   newLineBl = 1;
   siSpoligo = 0;
   tmpStr = buffStr;
   goto checkHead_fun03_sec03_sub02;

   /*****************************************************\
   * Fun03 Sec03 Sub02:
   *   Start loop & check if have new line in buffer
   \*****************************************************/

   while(bytesReadUL)
   { /*Loop: Find the number of sequences*/
      tmpStr += endLine_ulCp(tmpStr);

      if(*tmpStr == '\n' || *tmpStr == '\r')
      { /*If: I found an new line*/
         newLineBl = 1;

         if(*(tmpStr + 1) == '\0')
         { /*If: new line is at the end of the buffer*/
            getBuff_fun03_sec03:;

            if(! bytesReadUL)
               break; /*Make sure not EOF*/

            bytesReadUL =
               fread(
                  (char *) buffStr,
                  sizeof(char),
                  lenBuffSS,
                  spoligoFILE
               ); /*Read in the file*/

            buffStr[bytesReadUL] = '\0';
            tmpStr = buffStr;
         } /*If: new line is at the end of the buffer*/

         checkHead_fun03_sec03_sub02:;

         while(*tmpStr < 33)
            ++tmpStr;

         if(*tmpStr == '\0')
            goto getBuff_fun03_sec03;

         /*Count the number of fastq headers*/
         siSpoligo += ((*tmpStr == '>') & newLineBl);
      } /*If: I found an new line*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - If missing newline, get next part of file
      \**************************************************/

      else
      { /*Else: I am still on the old line*/
         bytesReadUL =
            fread(
               (char *) buffStr,
               sizeof(char),
               lenBuffSS,
               spoligoFILE
            ); /*Read in the file*/

         buffStr[bytesReadUL] = '\0';
         tmpStr = buffStr;
      } /*Else: I am still on the old line*/
   } /*Loop: Find the number of sequences*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - Read in the sequences
   ^   o fun03 sec04 sub01:
   ^     - Allocate memory for the sequence arrays
   ^   o fun03 sec04 sub02:
   ^     - Read in the sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - Allocate memory for the sequence arrays
   \*****************************************************/

   seqHeapAryST = 0;

   siSpoligo <<= 1; /*Acount for having both directions*/

   seqHeapAryST=
      malloc(siSpoligo * sizeof(struct seqST));

   if(! seqHeapAryST)
      goto memErr_fun03_sec05_sub02;

   /*Initialize the sequence structure array*/
   for(
      *numSpoligosSIPtr = 0;
      *numSpoligosSIPtr < siSpoligo;
      ++(*numSpoligosSIPtr)
   ) init_seqST(&seqHeapAryST[*numSpoligosSIPtr]);

   /*****************************************************\
   * Fun03 Sec04 Sub02:
   *   - Read in the sequences
   \*****************************************************/

   fseek(
      spoligoFILE,
      0,
      SEEK_SET
   );

   for(
      *numSpoligosSIPtr = 0;
      *numSpoligosSIPtr < siSpoligo;
      (*numSpoligosSIPtr) += 2
   ){ /*Loop: Read in the sequences*/
      errUC =
         getFa_seqST(
            spoligoFILE,
            &seqHeapAryST[*numSpoligosSIPtr]
         ); /*Read in the first sequence*/

      if(errUC)
      { /*If: I had an error*/
         if(errUC & 64)
            goto memErr_fun03_sec05_sub02;

         if(errUC & 2)
            goto fileErr_fun03_sec05_sub03;
      } /*If: I had an error*/

      cpErrSC =
         cp_seqST(
            &seqHeapAryST[*numSpoligosSIPtr + 1],
            &seqHeapAryST[*numSpoligosSIPtr]
         );

      if(cpErrSC)
         goto memErr_fun03_sec05_sub02;

      revComp_seqST(&seqHeapAryST[*numSpoligosSIPtr + 1]);

      seqToIndex_alnSet(
         seqHeapAryST[*numSpoligosSIPtr].seqStr
      ); /*For alignments*/

      seqToIndex_alnSet(
         seqHeapAryST[*numSpoligosSIPtr + 1].seqStr
      ); /*For alingments*/

      if(errUC & ~1)
      { /*If: I hade fewer spoligotypes than expected*/
         *numSpoligosSIPtr +=  2;
         break;
      } /*If: I hade fewer spoligotypes than expected*/
   }  /*Loop: Read in the sequences*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - Clean up
   ^   o fun03 sec03 sub02:
   ^     - Clean up when there are no errors
   ^   o fun03 sec03 sub02:
   ^     - Clean up when there are memory errors
   ^   o fun03 sec03 sub03:
   ^     - Clean up when there are file errors
   ^   o fun03 sec03 sub04:
   ^     - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec05 Sub01:
   *   - Clean up when there are no errors
   \*****************************************************/

   *errSCPtr = 0;
   goto cleanUp_fun03_sec05_sub04;

   /*****************************************************\
   * Fun03 Sec05 Sub02:
   *   - Clean up when there are memory errors
   \*****************************************************/

   memErr_fun03_sec05_sub02:;
   *errSCPtr = def_memErr_tbSpolDefs;
   goto errCleanUp_fun03_sec05_sub04;

   /*****************************************************\
   * Fun03 Sec05 Sub03:
   *   - Clean up when there are file errors
   \*****************************************************/

   fileErr_fun03_sec05_sub03:;
   *errSCPtr = def_fileErr_tbSpolDefs;
   goto errCleanUp_fun03_sec05_sub04;

   /*****************************************************\
   * Fun03 Sec05 Sub04:
   *   - clean up
   \*****************************************************/

   errCleanUp_fun03_sec05_sub04:;

   freeHeapAry_seqST(
      seqHeapAryST,
      *numSpoligosSIPtr
   );

   seqHeapAryST = 0;


   cleanUp_fun03_sec05_sub04:;

   if(spoligoFILE)
      fclose(spoligoFILE);

   spoligoFILE = 0;

   return seqHeapAryST;
} /*getRefs_spolFind*/

/*-------------------------------------------------------\
| Fun04: fxwater_spolFind
|   - searches for spoligotypes in fastx sequences using
|     a waterman
| Input:
|   - fx_seqSTPtr:
|     o pointer to an seqST with the sequence to
|       search (sequence as characters. This function
|       converts)
|   - spoliogAry_SeqSTPtr:
|     o pointer to an seqST array with the converted
|       spoloigotype sequences (fun03: getRefs_spolFind
|       return)
|   - minPercScoreF:
|     o minimum percentage to count a spoligotype
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - pProgressBl:
|     o 1: print progress reports to the screen (stderr)
|     o 0: do not print progress reports
|   - fragCheckBl:
|     o 1: do not blank codeAryUI (checking fragments)
|     o 0: blank codeAryUI (expect full length reads)
|   - alnSetSTPtr:
|     o pointer to an alnSet structer with alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Prints:
|     o Status to stderr if user requested
|   - Returns:
|     o 0 for success
|     o def_noSpol_tbSpolDefs if no spacers found
|     o def_memErr_tbSpolDefs for memory errors
\-------------------------------------------------------*/
signed char
fxwater_spolFind(
   struct seqST *refAryST,
   struct seqST *spoligoAryST,
   signed int numSpoligosSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char pProgressBl,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04:
   '   - Searches for spoligotypes in fastx sequences
   '     using an memory efficent waterman
   '   o fun04 sec01:
   '     - Variable declerations
   '   o fun04 sec02:
   '     - Check if there are spoligotypes
   '   o fun04 sec03:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siSpoligo = 0;
   signed char revBl = 0;    /*Check reverse complement*/
   signed char noSpoligoBl = def_noSpol_tbSpolDefs;

   float percScoreF = 0;
   signed long maxScoreSL = 0;
   signed long scoreSL = 0;
   signed long spoligoStartSL = 0;
      /*Start on spoligotype seq*/
   signed long spoligoEndSL = 0;
      /*End on spoligotype seq*/
   signed long startAlnSL = 0;
       /*First aligned base in seq*/
   signed long endAlnSL = 0;
      /*Last alignned base in seq*/

   /*code array for spoligotyping*/
   unsigned int bitAryUI[def_numSpol_tbSpolDefs + 1];
   unsigned int *spolAryUIPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - Check if there are spoligotypes
   ^   o fun04 sec02 sub01:
   ^     - blank barcode array if not fragment mode
   ^   o fun04 sec02 sub02:
   ^     - Check if an spoligotype is in forward sequence
   ^   o fun04 sec02 sub03:
   ^     - Check if spoligotype is in reverse sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01:
   *   - blank barcode array if not fragment mode
   \*****************************************************/

   if(fragCheckBl)
   { /*If: only have direct repeat fragments*/
      for(
         siSpoligo = 0;
         siSpoligo < (numSpoligosSI >> 1);
         ++siSpoligo
      ) bitAryUI[siSpoligo] = 0; /*blank for new seq*/

      bitAryUI[siSpoligo] = -1; /*mark end*/

      spolAryUIPtr = bitAryUI;
   } /*If: only have direct repeat fragments*/

   else
   { /*Else: expect full direct repeat regions*/
      for(
         siSpoligo = 0;
         siSpoligo < (numSpoligosSI >> 1);
         ++siSpoligo
      ) codeAryUI[siSpoligo] = 0; /*blank for new seq*/

      spolAryUIPtr = codeAryUI;
   } /*Else: expect full direct repeat regions*/

   codeAryUI[numSpoligosSI >> 1] = -1;

   /*****************************************************\
   * Fun04 Sec02 Sub02:
   *   - Check if an spoligotype is in forward sequence
   \*****************************************************/
   
   seqToIndex_alnSet(refAryST->seqStr);

   for(
      siSpoligo = 0;
      siSpoligo < numSpoligosSI;
      siSpoligo += 2
   ){ /*Loop: Find the matching spoligotypes*/
      if(pProgressBl)
      { /*If: The user wanted progress reports*/
         if(! revBl)
            fprintf(
               stderr,
               "checking foward spoligotype %i%s",
               1 + (siSpoligo >> 1),
               str_endLine
            );

         else
            fprintf(
               stderr,
               "checking reverse spoligotype %i%s",
               1 + (siSpoligo >> 1),
               str_endLine
            );
      } /*If the user wanted progress reports*/

      maxScoreSL =
         maxScore_alnDefs(
           spoligoAryST[siSpoligo].seqLenSL
         );

      scoreSL =
         memwater(
            &spoligoAryST[siSpoligo + revBl],
            refAryST,
            &spoligoStartSL,
            &spoligoEndSL,
            &startAlnSL,
            &endAlnSL,
            alnSetSTPtr 
         );

       percScoreF = (float) scoreSL / (float) maxScoreSL;

       if(percScoreF >= minPercScoreF)
       { /*If: I had an match*/
          spolAryUIPtr[siSpoligo >> 1] = 1;
          noSpoligoBl = 0;
          continue;
       } /*If: I had an match*/

      /*I am ignoring memory errors*/

      /**************************************************\
      * Fun04 Sec02 Sub03:
      *   - Check if spoligotype is in reverse sequence
      \**************************************************/

      if(pProgressBl)
      { /*If: The user wanted progress reports*/
         if(! revBl)
            fprintf(
               stderr,
               "Trying the reverse spoligotype %i%s",
               1 + (siSpoligo >> 1),
               str_endLine
            );

         else
            fprintf(
               stderr,
               "Trying the forward spoligotype %i%s",
               1 + (siSpoligo >> 1),
               str_endLine
            );
      } /*If the user wanted progress reports*/

      maxScoreSL =
         maxScore_alnDefs(
           spoligoAryST[siSpoligo].seqLenSL
         );

      scoreSL =
         memwater(
            &spoligoAryST[siSpoligo + (! revBl)],
            refAryST,
            &spoligoStartSL,
            &spoligoEndSL,
            &startAlnSL,
            &endAlnSL,
            alnSetSTPtr 
         );

       percScoreF = (float) scoreSL / (float) maxScoreSL;

       if(percScoreF >= minPercScoreF)
       { /*If: I found an hit*/
          spolAryUIPtr[siSpoligo >> 1] = 1;
          revBl = !revBl; /*likely future hits are same*/
          noSpoligoBl = 0;
       } /*If: I found an hit*/
   } /*Loop: Find the matching spoligotypes*/
      
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fragCheckBl)
   { /*If: add detected fragments to code array*/
      for(
         siSpoligo = 0;
         siSpoligo < (numSpoligosSI >> 1);
         ++siSpoligo
      ) codeAryUI[siSpoligo] += bitAryUI[siSpoligo];
   } /*If: add detected fragments to code array*/

   codeAryUI[numSpoligosSI >> 1] = -1;
   indexToSeq_alnSet(refAryST->seqStr);

   return noSpoligoBl;
} /*fxwater_spolFind*/

/*-------------------------------------------------------\
| Fun05: samWater_spolFind
|   - searches for spoligotypes in sam files using a
|     waterman alignment (slow)
| Input:
|   - samSTPtr:
|     o pointer to an samEntry structure with sequence to
|       search for spoligotypes
|   - spoliogAry_SeqSTPtr:
|     o pointer to an seqST array with the converted
|       spoloigotype sequences (fun03: getRefs_spolFind
|       return)
|   - drStartSI:
|     o starting position of the direct repeat region on
|       the reference
|   - drEndSI:
|     o ending position on the direct repeat region on the
|       reference
|   - minPercScoreF:
|     o float with minimum percentage to count an
|       spoligotype
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structer with alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Returns:
|     o 0 for success
|     o def_noSpol_tbSpolDefs
|     o def_memErr_tbSpolDefs for memory errors
\-------------------------------------------------------*/
signed char
samWater_spolFind(
   struct samEntry *samSTPtr,
   struct seqST *spoligoAryST,
   signed int numSpoligosSI,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - Searches for spoligotypes in sam files
   '   o fun05 sec01:
   '     - Variable declerations
   '   o fun05 sec02:
   '     - Find direct repeat (DR) region coordinates
   '   o fun05 sec03:
   '     - Run the spoligo checks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siCig = 0;
   signed int siNt = 0;
   unsigned int refPosUI = 0;
   signed int seqPosSI = 0;
   
   struct seqST seqSTDoNotFree;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - Find direct repeat (DR) region coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Check if I have the full DR region*/
   if(fragCheckBl)
   { /*If: I am checking fragments*/
      if(samSTPtr->refEndUI < (unsigned int) dirStartSI)
         return def_noSpol_tbSpolDefs;

      if(samSTPtr->refStartUI > (unsigned int) dirEndSI)
         return def_noSpol_tbSpolDefs;
   } /*If: I am checking fragments*/

   else
   { /*Else: I am checking full direct repeat regions*/
      /*-1 is to make sure start and end are not index 1*/
      if(
           samSTPtr->refStartUI
         > (unsigned int) dirStartSI - 1
      ) return def_noSpol_tbSpolDefs;

      if(samSTPtr->refEndUI < (unsigned int) dirEndSI - 1)
         return def_noSpol_tbSpolDefs;
   } /*Else: I am checking full direct repeat regions*/

   init_seqST(&seqSTDoNotFree);

   siNt = samSTPtr->cigArySI[0];
   refPosUI = samSTPtr->refStartUI;

   findRefPos_samEntry(
      samSTPtr,
      &siCig,
      &siNt,
      dirStartSI,
      (signed int *) &refPosUI,
      &seqPosSI
   );

   seqSTDoNotFree.offsetSL = (unsigned long) seqPosSI;

   if(samSTPtr->refEndUI > (unsigned int) dirEndSI)
   { /*If: the read extends beyond the direct repeat*/
      findRefPos_samEntry(
         samSTPtr,
         &siCig,
         &siNt,
         dirEndSI,
         (signed int *) &refPosUI,
         &seqPosSI
      );

      seqSTDoNotFree.endAlnSL = (unsigned long) seqPosSI;
   } /*If: the read extends beyond the direct repeat*/

   else
      seqSTDoNotFree.endAlnSL = samSTPtr->readLenUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - Run the spoligo checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqSTDoNotFree.seqStr = samSTPtr->seqStr;

   return
      fxwater_spolFind(
         &seqSTDoNotFree,
         spoligoAryST,
         numSpoligosSI,
         minPercScoreF,
         codeAryUI,
         0,           /*do not show progress*/
         fragCheckBl,
         alnSetSTPtr
      );
} /*samWater_spolFind*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
