/*#######################################################\
# Name: readIdSubsample
#   - Has functions for subsampling reads from a sam file
# Libraries:
#   - "minAlnStatsStruct.h"
#   - "../generalLib/trimSam.h"
#   o "../generalLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   o "../generalLib/ulCpStr"
#   o "../generalLib/numToStr.h"
# C Standard Libraries:
#   o <stdlib.h>
#   o <stdio.h>
#   o <limits.h>
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
'   o fun-04: checkIfRmRead
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
| Fun-04: checkIfRmRead
|   - Checks to see if read meets the minimum thresholds
|     to keep
| Input:
|   - minAlnStatsST:
|     o Pointer to minAlnStats structure with the Maximum
|       difference from the reference to keep a read
|       (thesholds)
|   - samST:
|     o Pointer to samEntry struct with read to check
| Output:
|  - Returns:
|    o 0: Keep read (at or over minimum thresholds)
|    o 1: Discard read (under minimu threshods)
\-------------------------------------------------------*/
#define checkIfRmRead(minAlnStatsST, samST)({ \
   float percSnpF = 0;\
   float percIndelF = 0;\
   float percDiffF = 0;\
   char retC = 0;\
   \
   percSnpF =\
     (samST)->numSnpUI / ((float) (samST)->readLenUI);\
   \
   percIndelF =\
        ((samST)->numDelUI/((float) (samST)->readLenUI))\
      + ((samST)->numInsUI/((float) (samST)->readLenUI));\
   \
   percDiffF = percIndelF + percSnpF;\
   \
   retC |= ((minAlnStatsST)->minSNPsFlt < percSnpF);\
   retC |= ((minAlnStatsST)->minIndelsFlt < percIndelF);\
   retC |= ((minAlnStatsST)->minDiffFlt < percDiffF);\
   \
   retC;\
}) /*checkIfRmRead*/

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
   struct idStack *scoresAry[maxScoreUS +1];/*Hash table*/

   struct samEntry samST;
   char *buffStr = 0;
   ulong lenBuffUL = 0;

   long lIter;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-02:
   ^   - Get the first line of the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samST);
   errUC= readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);

   if(errUC) return 0;

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
      freeSamEntryStack(&samST);
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
   *    - Check to see if read is worth keeping
   \*****************************************************/

   while(!errUC)
   { /*Loop: Find read ids to subsample in sam file*/
      if(*samST.extraStr == '@') goto nextEntry;
      if(samST.flagUS & (2048|256|4)) goto nextEntry;

      /*Check if the read is to long*/
      if(   samST.readLenUI > minStats->maxReadLenULng
         && minStats->maxReadLenULng > 0
      ) goto nextEntry;

      if(samST.alnReadLenUI < minStats->minReadLenULng)
         goto nextEntry;

      if(samST.mapqUC < minStats->minMapqUInt) 
         goto nextEntry;

      if(checkIfRmRead(minStats, &samST)) goto nextEntry;

      /*Remove the soft masked regions*/
      errUC = trimSamEntry(&samST);

      if(errUC >> 2) goto nextEntry;

      if(samST.medianQF < minStats->minMedianQFlt)
         goto nextEntry;

      if(samST.meanQF < minStats->minMeanQFlt)
         goto nextEntry;

      /**************************************************\
      * Fun-07 Sec-04 Sub-02:
      *   - Score reads
      \**************************************************/

      /*Discard decimal part of the Q-score and build a
      ` score from the mapq and Q-scores
      */
      scoreUS =
         (
              ((ushort) samST.mapqUC << 2)
            & (ushort) useMapqBl
         ) + (ushort) samST.medianQF;

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

      if(scoreUS <= lowScoreUS) goto nextEntry;

      mvIdStackST(
         scoresAry[lowScoreUS],
         scoresAry[scoreUS],
         samST.qryIdStr
      );

      *numIdsKeptUL += numIdsToExtractUL > *numIdsKeptUL;

      while(scoresAry[lowScoreUS] == 0) ++lowScoreUS;
 
      /**************************************************\
      * Fun-04 Sec-05 Sub-04:
      *   - Get the next line in the sam file
      \**************************************************/

      nextEntry:;

      errUC =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Find read ids to subsample in sam file*/

   freeSamEntryStack(&samST);
   return idsST;
} /*getXSamIds*/

#endif
