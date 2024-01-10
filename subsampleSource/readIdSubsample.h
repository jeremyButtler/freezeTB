/*#######################################################\
# Name: readIdSubsample
#   - Has functions for subsampling reads from a sam file
# Libraries:
#   - "minAlnStatsStruct.h"
#   - "../generalLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   - "../generalLib/trimSam.h"
# C Standard Libraries:
#   o <stdint.h>
#   o <stdlib.h>
#   o <string.h>
#   o <stdio.h>
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start of File
'   o header:
'     - Libraries & defined variables/small macros
'   o st-01: idStack
'     - idStack structure to hold read ids
'   o fun-01: freeIdStackAry
'     - Frees an array of idStack structures
'   o fun-02: 
'     - Push an idStack structure onto an idStack stack
'   o fun-03: mvIdStackST
'     - Move an idStack structure to another stack
'   o fun-04: checkIfKeepRead
'     - Checks to see if read meets the minimum thresholds
'       to keep
'   o fun-05: qHistToMed
'     - Uses a histogram of q-scores to find the median
'       q-score
'   o fun-06: findSamQScores
'     - Finds the mean and median Q-scores for a line from
'       a sam file stored in an samEntry struct
'   o fun-07: getXSamIds
'     - Get X (number) best read ids from a sam file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and defintions
\-------------------------------------------------------*/

#ifndef READ_ID_SUBSAMPLE_H
#define READ_ID_SUBSAMPLE_H

#include "../generalLib/trimSam.h"
#include "../generalLib/samEntryStruct.h"
#include "minAlnStatsStruct.h"

#define maxUS 1 << 15
#define charToQ(inC) ((inC) - Q_ADJUST)

/*-------------------------------------------------------\
| ST-01: idStack
|   - Part of a stack of linked list of read ids
\-------------------------------------------------------*/
typedef struct idStack{
   char idStr[60];
   struct idStack *stack;
}idStack;

/*-------------------------------------------------------\
| Fun-01: freeIdStackAry
|   - Frees an array of idStack structures
| Input:
|   - idStackAryST
|     o Pointer to an heap allocated array of idStack
|       structures
| Output:
|   - Frees:
|     o idStackAryST (and sets to 0)
\-------------------------------------------------------*/
#define freeIdStackAry(idStackAryST){\
   free((idStackAryST));\
   (idStackAryST) = 0;\
} /*freeIdStackAry*/

/*-------------------------------------------------------\
| Fun-02: 
|   - Push an idStack structure onto an idStack stack
| Input:
|   - idStackST:
|     o Pointer to stack of idStack structures to push a
|       new idStack onto
|   - newIdST:
|     o Pionter to idStack structure to be pushed onto
|       (new head) of idStackST
|   - idStr:
|     o id that idStr should hold
| Output:
|   - Modifes:
|     o idStackST to have newIdST as the head of the
|       stack
\-------------------------------------------------------*/
#define pushIdStack(idStackST, newST, newIdStr){\
   int iChar = 0;\
   (newST)->stack = (idStackST);\
   (idStackST) = (newST);\
   \
   /*Copy the new read id*/\
   for(iChar = 0; (newIdStr)[iChar] > 32; ++iChar)\
      (newST)->idStr[iChar] = (newIdStr)[iChar];\
   \
   (newST)->idStr[iChar] = '\0';\
} /*pushIdStack*/

/*-------------------------------------------------------\
| Fun-03: mvIdStackST
|   - Move an idStack structure to another stack
| Input:
|   - idStackST:
|     o Pointer to stack of idStack structures to pop a
|       new idStack off
|   - newSackST:
|     o Pionter to stack of idStack structures to push
|       the new id onto
|   - idStr:
|     o New id to change the moved id stack ST to
| Output:
|   - Modifies:
|     o Pops the first node of idStackST and pushes it
|       on newStackST and sets the pushed nodes idStr
|       to idStr
\-------------------------------------------------------*/
#define mvIdStackST(idStackST, newStackST, newIdStr){\
   struct idStack *swapST = (idStackST);\
   \
   (idStackST) = (idStackST)->stack;\
   pushIdStack((newStackST), swapST, (newIdStr));\
} /*mvIdStackST*/

/*-------------------------------------------------------\
| Fun-04: checkIfKeepRead
|   - Checks to see if read meets the minimum thresholds
|     to keep
| Input:
|   - maxDiffST:
|     o Maximum difference from the reference to keep a
|       read (thesholds)
|   - samST:
|     o Pointer to samEntry struct with read to check
| Output:
|    Returns:
|        - 0: Discard read (under minimu threshods)
|        - 1: Keep read (at or over minimum thresholds)
\-------------------------------------------------------*/
static char checkIfKeepRead(
   struct minAlnStats *maxDiffST,
   struct samEntry *samST
){ 
   float percSnpFlt = 0;
   float percIndelFlt = 0;
   float percDelFlt = 0;
   float percDiffFlt = 0;


   /*Check snps*/
   percSnpFlt =
        samST->numSNPUInt
      / ((float) samST->readLenUInt);

   if(maxDiffST->minSNPsFlt < percSnpFlt) return 0;

   /*check indels*/
   percIndelFlt =
        samST->numInsUInt
      / ((float) samST->readLenUInt);

   percDelFlt =
        samST->numDelUInt
      / ((float) samST->readLenUInt);

   percIndelFlt += percDelFlt;

   if(maxDiffST->minIndelsFlt < percIndelFlt) return 0;

   /*Check the total difference*/
   percDiffFlt = percIndelFlt + percSnpFlt;
   if(maxDiffST->minDiffFlt < percDiffFlt) return 0;

   return 1;
} /*checkIfKeepRead*/

/*-------------------------------------------------------\
| Fun-05: qHistToMed
|   - Uses a histogram of q-scores to find the median
|     q-score
| Input:
|   - qHistUInt:
|     o q-score histogram to get the median Q-score from
|   - readLenUInt:
|     o Number of bases in the qHistUInt
| Output:
|   - Returns:
|     o The median Q-score as a float
\-------------------------------------------------------*/
static float qHistToMed(
   uint32_t qHistUInt[], /*Histogram of Q-scores*/
   uint32_t readLenUInt /*Number of bases in the read*/
){ /*qHistToMed*/
    uint32_t numBasesUInt = 0;
    uint32_t midPointULng = readLenUInt / 2;
    uint uiQ = 0;
    uint oldQUI = 0;

    for(uiQ = 0; uiQ < MAX_Q_SCORE; uiQ++)
    { /*Loop: to find the median piont in the histogram*/
        numBasesUInt += qHistUInt[uiQ];

        if(numBasesUInt >= midPointULng)
        { /*if found the midpoint, then find the median*/

            if(numBasesUInt > midPointULng) return uiQ;
            else if(numBasesUInt % 2 == 1) return  uiQ;

            else
            { /*Else is even, so two numbers at mid*/
                oldQUI = uiQ;
                ++uiQ;

                while(qHistUInt[uiQ] == 0) ++uiQ;

                return (oldQUI + uiQ) / ((float) 2);
            } /*Else is even, so two numbers at mid*/
        } /*if found the midpoint, then find median*/
    } /*Loop: to find the median piont in the histogram*/

    return 0; /*Just in case was all 0's in the array*/
} /*qHistToMed*/

/*-------------------------------------------------------\
| Fun-06: findSamQScores
|   - Finds the mean and median Q-scores for a line from
|     a sam file stored in an samEntry struct
| Input:
|   - samST:
|     o samEntry struct with the sam file entry to get the
|       mean and median q-scores for
| Output:
|   - Modifies:
|     o samST to have mean and median q-scores
\-------------------------------------------------------*/
static void findQScores(struct samEntry *samST)
{ /*findQScores*/
    ulong ulBase = 0;
    int qScoreI = 0;

    samST->totalQScoreULng = 0;
    samST->meanQFlt = 0;
    samST->medianQFlt = 0;

    if(samST->qCStr == 0) return; /*no Q-score entry*/

    if(samST->qCStr[0] == '*'|| samST->qCStr[1] == '\t')
       return; /*No q-score entry*/

    for(ulBase = 0; samST->qCStr[ulBase] > 32; ++ulBase)
    { /*Loop: through the Q-score entry*/
        qScoreI = charToQ(samST->qCStr[ulBase]);
           /*charToQ is in scoreReadsFun.h*/

        ++samST->seqQHistUInt[qScoreI];
        samST->totalQScoreULng += qScoreI;
    } /*Loop: through the Q-score entry*/
    
    /*Find the mean and median*/
    samST->meanQFlt =
        samST->totalQScoreULng / ((float) ulBase);

    samST->medianQFlt =
        qHistToMed(samST->seqQHistUInt, ulBase);
        /*qHistToMed is in scoreReadsFun.h*/

    return;
} /*findQScores*/

/*-------------------------------------------------------\
| Fun-07: getXSamIds
|   - Get X (number) best read ids from a sam file
| Input:
|   - numIdsToExtractUL:
|     o Maximum number of read ids to extract
|   - numIdsKeptUL:
|     o Modified to hold the number of read ids extracted
|   - useMapqBl:
|     o -1: use mapq (1st check) to compare reads
|     o 0: Do not use mapping quality (mapq)
|   - minStats:
|     o minAlnStats structure with the minimum stats to
|       keep a read id
|   - samFILE:
|     o Sam file to search for best read ids in
| Output:
|   - Returns:
|     o An array of idStack structers with the read ids
|       to keep. Free with freeIdStackAry() (fun-01)
|     o 0: for an error
| Note:
|   - Score method:
|     o percMult * (keptSNPs + keptIns + keptDels) /
|       read length
|   - minSimUSht ranges from 1 (0.01%) to precMult (100%)
\-------------------------------------------------------*/
static struct idStack * getXSamIds(
    ulong numIdsToExtractUL,
    ulong *numIdsKeptUL,
    short useMapqBl,
    struct minAlnStats *minStats,
    FILE *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-07 TOC: getXSamIds
   '  o fun-07 sec-01:
   '    - variable declerations
   '   fun-07 sec-02:
   '    - Get the first line of the sam file (as a check)
   '  o fun-07 sec-03:
   '    - Initialize and make the idStack to to return
   '  o fun-07 sec-04:
   '   - select top x read ids
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar errUC = 0; /*Holds error output*/

   ushort maxScoreUS = 128 + (2020 & (-(useMapqBl != 0)));
   ushort lowScoreUS = 0;
   ushort scoreUS = 0;

   struct idStack *idsST;
   struct idStack *scoresAry[maxScoreUS + 1]; /*Hash table*/
   struct idStack *tmpIdST;

   struct samEntry samST;

   long lIter;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-02:
   ^   - Get the first line of the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samST);
   errUC = readSamLine(&samST, samFILE);

   if(!(errUC & 1)) return 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-03:
   ^   - Initialize and make the idStack to return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *numIdsKeptUL = 0; /*Make sure start at 0 reads*/
   useMapqBl = -(useMapqBl != 0);

   idsST =
      malloc(numIdsToExtractUL *sizeof(struct idStack));
 
   if(idsST == 0)
   { /*If: I had a memory error*/
      freeStackSamEntry(&samST);
      return 0;
   } /*If: I had a memory error*/

   scoresAry[0] = &idsST[0];
   scoresAry[0]->idStr[0] = '\0';

   for(
      lIter = numIdsToExtractUL - 1;
      lIter > -1;
      --lIter
   ) { /*Loop: Add ids to 0 score and initialize*/
      idsST[lIter].stack = 0;
      pushIdStack(scoresAry[0], &idsST[lIter], "\0");
   } /*Loop: Add ids to 0 score and initialize*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-04:
   ^   - Trim, score, & select best mapped reads
   ^   o fun-07 sec-04 sub-01:
   ^     - Check flags to see if is an primary alignment
   ^   o fun-07 sec-04 sub-02:
   ^     - Score reads and check if scores above minimum
   ^   o fun-07 sec-04 sub-03:
   ^     - Add read to list if there is a read with a
   ^       lower score to remove
   ^   o fun-07 sec-04 sub-04:
   ^     - Get next read in score file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-07 Sec-04 Sub-01:
   *    - Check flags to see if is an primary alignment
   \*****************************************************/

   while(errUC & 1)
   { /*Loop: Find read ids to subsample in sam file*/
      if(*samST.samEntryCStr == '@') goto getNextLine;
      if(samST.flagUSht & (2048|256|4)) goto getNextLine;

      /*Check if the read is to long*/
      if(   samST.readLenUInt > minStats->maxReadLenULng
         && minStats->maxReadLenULng > 0
      ) goto getNextLine;

      /*Convert & print out sam file entry*/
      errUC = trimSamEntry(&samST);

       /*This will be the aligned length*/
      if(samST.readLenUInt < minStats->minReadLenULng)
         goto getNextLine;

      if(errUC >> 2) goto getNextLine;

      /**************************************************\
      * Fun-07 Sec-04 Sub-02:
      *   - Score reads and check if scores above minimum
      \**************************************************/

      findQScores(&samST); /*Find the Q-scores*/

      if(samST.medianQFlt < minStats->minMedianQFlt)
         goto getNextLine;

      if(samST.meanQFlt < minStats->minMeanQFlt)
         goto getNextLine;

      /*Get scores and aligned stats for the read*/
      /*scoreAln(minStats, &samST);*/

      /*Check the mapqs and similarity*/
      if(   samST.mapqUChar < minStats->minMapqUInt
         || !checkIfKeepRead(minStats, &samST)
      ) goto getNextLine;

      /*Discard decimal part of the Q-score and build a
      ` score from the mapq and Q-scores
      */
      scoreUS =
         (
              ((ushort) samST.mapqUChar << 2)
            & (ushort) useMapqBl
         ) + (ushort) samST.medianQFlt;

      /*Logic:
      `  - Mapping quality is 0 if useMapqBl is 0.
      `  - mapqUChar is 8 bits (1111 1111)
      `    o mapqUChar << 2 max is 2027.
      `    with a max of 127.
      `    mapq << 2:  11 1111 1100
      `    median Q:       111 1111
      `                || |||| ||||
      `                      16 4 1
      `                     32 8 2
      `                    64
      `  | Median Q-score | Mapq |
      `  |:--------------:|:----:|
      `  |       4        |   2  |
      `  |       8        |   4  |
      `  |      16        |   8  |
      `  |      32        |  16  |
      `  |      64        |  32  | 
      `
      `  Table: Median Q-score needed to select an Q-score
      `    over a mapping quality
      */

      /**************************************************\
      * Fun-07 Sec-04 Sub-03:
      *   - Add read to list if there is a read with a
      *     lower score to remove
      \**************************************************/

      if(scoreUS <= lowScoreUS) goto getNextLine;

      mvIdStackST(
         scoresAry[lowScoreUS],
         scoresAry[scoreUS],
         samST.queryCStr
      );

      *numIdsKeptUL += numIdsToExtractUL > *numIdsKeptUL;

      while(scoresAry[lowScoreUS] == 0) ++lowScoreUS;
 
      /**************************************************\
      * Fun-04 Sec-05 Sub-04:
      *   - Get the next line in the sam file
      \**************************************************/

      getNextLine:

      blankSamEntry(&samST); /*Make sure is blank*/
      errUC = readSamLine(&samST, samFILE);
   } /*Loop: Find read ids to subsample in sam file*/

   return idsST;
} /*getXSamIds*/

#endif