/*########################################################
# Name memWater
# Use:
#  o Holds functions doing a memory efficent Smith
#    Waterman pairwise alignments.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of Functions
'   o header:
'     - Included libraries
'   o .c fun-01: macroMax
'     - Find the maximum value (branchless)
'   o .c fun-02: macroIfMax
'     - Set a value (ret) to a value based on which value
'       is greater.
'   o .c fun-03: indexToQry
'     - Gets the query coordinates of the query sequence
'       in an matrix.
'   o .c fun-04: indexToRef
'     - Gets the coordinates of the reference sequence in
'       in an matrix.
'   o .c fun-05: indexToCoord
'     - Gets the coordinates of the reference and query
'       sequence in an matrix.
'   o .c fun-06: waterMaxScore
'     - Maximizes the score for a single base pair in an
'       waterman query reference scan
'   o .c fun-07: waterMaxEndRowScore
'     - Maximizes the score for a single waterman
'       alignment when at the end of an row
'   o fun-08 memWater:
'     - Run a memory efficent Waterman Smith alignment on
'       input sequences
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "memWater.h"

#include "alnSetStruct.h"
#include "seqStruct.h"

/*.h files only*/
#include "../../generalLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .c #include <stdio.h>
!   o .h #include "alnSeqDefaults.h"
!   o .h #include "../../generalLib/base10StrToNum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-01: macroMax
|  - Find the maximum value (branchless)
| Input:
|  o ret:
|    - Value to hold the maximum value
|  o x:
|    - First value to compare, ret is set to x if x >= y
|  o y:
|    - second value to compare, ret is set to y if y > x
| Output:
|  - Sets:
|    - Sets ret to x if x is greater than or equal to y
|    - Sets ret to y if y is greather than x
\-------------------------------------------------------*/
#define macroMax(x, y)(\
   (x) ^ (((x) ^ (y)) & (-((x) < (y))))\
   /*Logic:
   `  x < y:
   `    if x < y I get 0 (x > y); else I get 1
   `  -(x < y):
   `    If x < y I get -1 (111...)
   `    If x >= y I get 0
   `  x ^ ((x ^ y) & 111...): (only when x < y)
   `    This gives me x
   `  x ^ (x ^ y) & 0: (only when x >= y)
   `    This gives me y
   */\
)

/*-------------------------------------------------------\
| Fun-02: macroIfMax
|  - Set a value (ret) to a value based on which value
|    is greater.
| Input:
|  o ret:
|    - This will hold the return value
|  o x:
|    - First value to compare, (if x >= y)
|  o y:
|    - second value to compare, (if y > x)
|  o xRet:
|    - Value to set ret of x is >= y
|  o yRet:
|    - Value to set ret of y is > x
| Output:
|  - Sets:
|    - ret to xRet if x is greater than or equal to y
|    - ret to yRet if y is greater than x
\-------------------------------------------------------*/
#define macroIfMax(x, y, xRet, yRet)(\
   (xRet) ^ (((xRet) ^ (yRet)) & (-((x) < (y))))\
   /*This follows the same logic as macroMax(ret, x, y),
   ' except instead of setting ret to the highest value, I
   ' set ret to xRet if x is >= to y or yRet if y > x.
   */ \
)

/*-------------------------------------------------------\
| Fun-03: indexToQry
|   - Gets the query coordinates of the query sequence in
|     an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - qryCoord:
|     o Will hold the coordinate of the query sequence
| Output:
|  - Returns:
|    o The query coordinate that was in the index
\-------------------------------------------------------*/
#define indexToQry(refLen, index)(\
   ((index) / ((refLen) + 1)) - ((index) >= (refLen))\
   /* Find query coordinates from an index:
   `  - refLen + 1:
   `    o gives the length of each row in the matrix
   `    o The + 1 accounts for the gap column
   `  - index / (refLen + 1)
   `    o The number of rows down, which is the number of
   `      query bases + the gap row
   `  - index >= refLen
   `    o Is 1 if the index is divisible by the reference
   `      This means I have an index 1 value
   `    o else index is in the gap row (index 0)
   `  - position - (index 1 or 0)
   `    o Removes the gap row from the query if it is
   `      present. This makes sure I get an index 0 result
   */\
) /*indexToQry*/

/*-------------------------------------------------------\
| Fun-04: indexToRef
|   - Gets the coordinates of the reference sequence in
|     an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - refCoord:
|     o Will hold the coordinate of the reference sequence
| Output:
|  - Returns
|    o The reference coordinate that was in the index
\-------------------------------------------------------*/
#define indexToRef(refLen, index)(\
     ((index) % ((refLen) + 1))\
   - (((index) / ((refLen) + 1)) > 0)\
   /* Find reference coordinates from an index:
   `  - refLen + 1:
   `    o gives the length of each row in the matrix
   `    o The + 1 accounts for the gap column
   `  - index % (refLen + 1)
   `    o The number of columns, which is the number of
   `      reference bases + the gap row
   `  - index / (refLen + 1)
   `    o Gets the query position
   `  - (index / (refLen + 1)) > 0
   `    o Is 0 if I am in the gap column, 1 if on a real
   `      reference base
   `  - position - (index 1 or 0)
   `    o Removes the gap column from the reference if it
   `      is present. This makes sure I get an index 0
   `      result.
   */\
) /*indexToRef*/

/*-------------------------------------------------------\
| Fun-05: indexToCoord
|   - Gets the coordinates of the reference and query
|     sequence in an matrix.
| Input
|   - refLen:
|     o Length of the reference sequence
|   - index:
|     o Index to convert to cooridnates
|   - refCoord:
|     o Will hold the coordinate of the reference sequence
|   - qryCoord:
|     o Will hold the coordinate of the query sequence
| Output:
|  - Sets
|    o refCoord to the reference coordinate in index
|    o qryCoord to the query coordinate in index
\-------------------------------------------------------*/
#define indexToCoord(refLen, index, refCoord, qryCoord){\
   (refCoord) = indexToRef((refLen), (index));\
   (qryCoord) = indexToQry((refLen), (index));\
} /*indexToCoord*/


/*-------------------------------------------------------\
| Fun-06: waterMaxScore
|   - Maximizes the score for a single base pair in an
|     waterman query reference scan
| Input;
|   - refBase;
|     o Reference base to find score for
|   - qryBase;
|     o Query base to find score for
|   - gapDiff:
|     o Gap Exentsion score - gap open score.
|     o This is used to find the indel scores
|   - scoreOn:
|     o Score to update/score for an insertion
|   - dirOn;
|     o Direction to update/direction did for last ins
|   - nextSnpScore:
|     o Score to use in finding the snp score
|   - delScore:
|     o Score for making a deletion (is updated)
|   - alnSetPtr:
|     o Pointer to alnSet structure with the settings
|       for the alignment
| Output:
|   - Modifies:
|     o socreOn to hold the maximum score
|     o dirOn to hold the direction of the max score
|     o nextSnpScore to hold the score used to find the
|       next snp score
|     o delScore to hold the score for the next deletion
\-------------------------------------------------------*/
#define waterMaxScore(\
   refBase,     /*Reference Base*/\
   qryBase,     /*Query Base*/\
   gapDiff,     /*GapExtend - gapOpen*/\
   scoreOn,     /*Score to update*/\
   dirOn,       /*Direction to update*/\
   index,       /*Start of alignment for kept dir*/\
   nextSnpScore,/*Gets score to use for next snp*/\
   delScore,    /*Score for an deletion*/\
   snpPos,      /*Index for snp*/\
   insPos,      /*Indes for ins*/\
   delPos,      /*Index for del*/\
   curIndex,    /*Current index (for stops)*/\
   alnSetPtr    /*Pointer to alnSet with settings*/\
){ /*waterMaxScore*/\
   long macroSnpScoreSL =\
        (nextSnpScore)\
      + getBaseScore((qryBase), (refBase), (alnSetPtr));\
    \
    long macroInsScoreSL =\
         (scoreOn)\
       + ((gapDiff) & (-((dirOn) != defMvSnp)))\
       + (alnSetPtr)->gapOpenC;\
    \
    ulong keepDirUL = 0;\
    \
    (nextSnpScore) = (scoreOn);/*Score to find next snp*/\
    \
    macroMax((scoreOn), (delScore), macroInsScoreSL);\
    (dirOn) = macroSnpScoreSL > (scoreOn);\
     \
     macroIfMax(\
        (index),\
        (delScore),\
        macroInsScoreSL,\
        (delPos),\
        (insPos)\
     );\
     \
   macroIfMax(\
      (index),\
      (scoreOn),\
      macroSnpScoreL,\
      (maxPos),\
      (snpPos)\
   );\
   \
   macroMax((scoreOn), (scoreOn), macroSnpScoreL);\
   (dirOn) += ( (delScore) != (scoreOn) ) + defMvDel;\
   /*Logic:
   ` maxDir = snp > max
   `   maxDir is 1 if snp was selected, else 0
   ` defMvDel = 1
   ` del != maxSc
   `   1 if a deletion (ins/snp) was selected
   ` For an snp 1 += (1) + 1 = 3
   ` For an ins 0 += (1) + 1 = 2
   ` For an del 0 += 0 + 1 = 1
   */\
   \
   /*Find if keeping score of if alignment stops*/\
   keepDirUL = -((scoreOn) > 0);\
   (dirOn) &= keepDirUL;\
   (scoreOn) &= keepDirUL;\
   (index) =\
        ((index) & keepDirUL)\
      + ((curIndex) & (~keepDirUL));\
   \
   (delScore) =\
        (scoreOn)\
      + ((gapDiff) & (-(dirOn) != defMvSnp))\
      + (alnSetPtr)->gapOpenC;\
} /*waterMaxScore*/

/*-------------------------------------------------------\
| Fun-07: waterMaxEndRowScore
|   - Maximizes the score for a single waterman alignment
|   - This for the end of a row, were you do not want the
|     next snp or deletion score
| Input;
|   - refBase;
|     o Reference base to find score for
|   - qryBase;
|     o Query base to find score for
|   - gapDiff:
|     o Gap Exentsion score - gap open score.
|     o This is used to find the indel scores
|   - scoreOn:
|     o Score to update/score for an insertion
|   - dirOn;
|     o Direction to update/direction did for last ins
|   - nextSnpScore:
|     o Score to use in finding the snp score
|   - alnSetPtr:
|     o Pointer to alnSet structure with the settings
|       for the alignment
| Output:
|   - Modifies:
|     o socreOn to hold the maximum score
|     o dirOn to hold the direction of the max score
\-------------------------------------------------------*/
#define waterMaxEndRowScore(\
   refBase,     /*Reference Base*/\
   qryBase,     /*Query Base*/\
   gapDiff,    /*GapExtend - gapOpen*/\
   scoreOn,     /*Score to update*/\
   dirOn,       /*Direction to update*/\
   index,       /*Start of alignment for kept dir*/\
   nextSnpScore,/*Gets score to use for next snp*/\
   delScore,    /*Score for an deletion*/\
   snpPos,      /*Index for snp*/\
   insPos,      /*Indes for ins*/\
   delPos,      /*Index for del*/\
   curIndex,    /*Current index (for stops)*/\
   alnSetPtr    /*Pointer to alnSet with settings*/\
){ /*waterMaxEndRowScore*/\
   long macroSnpScoreL =\
      (nextSnpScore)\
      + getBaseScore((qryBase), (refBase), (alnSetPtr));\
   \
   long macroInsScoreL =\
        (scoreOn)\
      + ((gapDiff) & (-((dirOn) != defMvSnp)))\
      + (alnSetPtr)->gapOpenC;\
   \
   ulong keepDirUL = 0;\
   \
   macroMax((scoreOn), (delScore), macroInsScoreSL);\
   (dirOn) = macroSnpScoreSL > (scoreOn);\
    \
   macroIfMax(\
      (index),\
      (delScore),\
      macroInsScoreSL,\
      (delPos),\
      (insPos)\
   );\
   \
   macroIfMax(\
      (index),\
      (scoreOn),\
      macroSnpScoreL,\
      (maxPos),\
      (snpPos)\
   );\
   \
   macroMax((scoreOn), (scoreOn), macroSnpScoreL);\
   (dirOn) += ( (delScore) != (scoreOn) ) + defMvDel;\
   \
   /*Find if keeping score of if alignment stops*/\
   keepDirUL = -((scoreOn) > 0);\
   (dirOn) &= keepDirUL;\
   (scoreOn) &= keepDirUL;\
   (index) =\
        ((index) & keepDirUL)\
      + ((curIndex) & (~keepDirUL));\
} /*waterMaxEndRowScore*/

/*-------------------------------------------------------\
| Fun-08: memWater
|   - Performs a memory efficent Smith Waterman alignment
|     on a pair of sequences
| Input;
|   - qryST:
|     o SeqStruct with the query sequence and index 0
|       coordinates to start (offsetUL)/end (endAlnUL) the
|       alignment.
|   - refST:
|     o SeqStruct with the reference sequence and index 0
|       coordinates to start (offsetUL)/end (endAlnUL) the
|       alignment
|   - refStartUL:
|     o Pointer to unsigned long to hold the frist
|       reference base in the alignment
|   - refEndUL:
|     o Pointer to unsigned long to hold the last
|       reference base in the alignment
|   - qryStartUL:
|     o Pointer to unsigned long to hold the frist query 
|       base in the alignment
|   - qryEndUL:
|     o Pointer to unsigned long to hold the last query
|       base in the alignment
|   - settings:
|     o alnSet structure with the setttings, such as
|       gap open, gap extend, scoring matrix, and
|       preffered direction.
| Output:
|  - Modifies:
|    o refStartUL to have 1st reference base in alignment
|    o refEndUL to have last reference base in alignment
|    o qryStartUL to have first query base in alignment
|    o qryEndUL to have last query base in alignment
|  - Returns:
|    o Score for aligment
|    o 0 for memory errors
\-------------------------------------------------------*/
long scoreSL
memWater(
    struct seqStruct *qryST, /*query sequence*/
    struct seqStruct *refST, /*ref sequence*/
    ulong *refStartUL,
    ulong *refEndUL,
    ulong *qryStartUL,
    ulong *qryEndUL,
    struct alnSet *settings  /*Settings for alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: memWaterAln
   '  - Run a memory efficent Waterman Smith alignment on
   '    input sequences
   '  o fun-08 sec-01:
   '    - Variable declerations
   '  o fun-08 sec-02:
   '    - Allocate memory for alignment
   '  o fun-08 sec-03:
   '    - Fill in initial negatives for ref
   '  o fun0 sec-04:
   '    - Fill the matrix with scores
   '  o fun-08 sec-05:
   '    - Set up for returing matrix (clean up/wrap up)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01: Variable declerations
   ^  o fun-08 sec-01 sub-01:
   ^    - Variables dealing with the query and reference
   ^      starting positions
   ^  o fun-08 sec-01 sub-02:
   ^    - Variables holding the scores (only two rows)
   ^  o fun-08 sec-01 sub-03:
   ^    - Directinol matrix variables
   ^  o fun-08 sec-01 sub-04:
   ^    - Variables for building returend alignment array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-01 Sub-01:
   *  - Variables dealing with the query and reference
   *    starting positions
   \*****************************************************/

   long scoreSL = 0; /*Score to return*/
   ulong bestStartUL = 0; /*Records best starting index*/
   ulong bestEndUL = 0;   /*Records best ending index*/

   /*Get start & end of query and reference sequences*/
   char *refSeqStr = 0;
   char *qrySeqStr = 0;

   ulong lenRefUL = refST->endAlnUL - refST->offsetUL + 1;
   ulong lenQryUL = qryST->endAlnUL - qryST->offsetUL + 1;

   /*Iterators for loops*/
   ulong ulRefBase = 0;
   ulong ulQryBase = 0;

   /*****************************************************\
   * Fun-08 Sec-01 Sub-02:
   *  - Variables holding the scores (only two rows)
   \*****************************************************/

   long delScoreL = 0;    /*Score for doing an deletion*/
   long nextSnpScoreL = 0;/*Score for the next match/snp*/
   long *scoreHeapSL = 0;  /*matrix to use in alignment*/

   /*Used in finding if useing gap extension*/
   short gapDiffS =
      settings->gapExtendC - settings->gapOpenC;

   /*****************************************************\
   * Fun-08 Sec-01 Sub-03:
   *  - Directional matrix variables
   \*****************************************************/

   /*Direction matrix (1 cell holds a single direction)*/
   char *dirRowHeapSC = 0;  /*Holds directions*/

   /*Keeping track of alignment starting positions*/
   ulong indexUL = 0;      /*Index I am at in the matrix*/
   ulong *indexHeapUL=0;   /*Row of starting indexes*/
   ulong *oldIndexHeapUL=0;/*Last round starting indexes*/
   ulong *swapPtrUL = 0;   /*For swapping ulongs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^  - Allocate memory for alignment
   ^  o fun-08 sec-02 sub-01:
   ^    - Allocate memory for the alignment
   ^  o fun-08 sec-02 sub-02:
   ^    - Allocate memory for keeping track of indexes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-02 Sub-01:
   *  - Allocate memory for the alignment
   \****************************************************/

   dirRowHeapSC = malloc((lenRefUL + 1) * sizeof(char));

   if(dirRowHeapSC == 0)
      goto memErr_fun08_sec05_sub01;

   scoreHeapSL = malloc((lenRefUL + 1) * sizeof(long));
   /*+ 1 is for the indel column*/

   if(scoreHeapSL == 0)
      goto memErr_fun08_sec05_sub01;

   /*****************************************************\
   * Fun-08 Sec-02 Sub-02:
   *  - Get memory for keeping track of starting indexes
   \*****************************************************/

   /*Set up the first row of starting indexes*/
   indexHeapUL = malloc((lenRefUL + 1) * sizeof(ulong));

   if(indexHeapUL == 0)
      goto memErr_fun08_sec05_sub01;

   /*Set up the second row of indexs (so have two rows)*/
   oldIndexHeapUL = malloc((lenRefUL +1) * sizeof(ulong));

   if(oldIndexHeapUL == 0)
      goto memErr_fun08_sec05_sub01;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^  - Fill in initial negatives for reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(indexUL = 0; indexUL <= lenRefUL; ++indexUL)
   { /*loop; till have initalized the first row*/
      dirRowHeapSC[indexUL] = defMvStop;
      indexHeapUL[indexUL] = indexUL;
      scoreHeapSL[indexUL] = 0;
   } /*loop; till have initalized the first row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-04:
   ^  - Fill the matrix with scores
   ^  o fun-08 sec-04 sub-01:
   ^    - Final set up before scoring the matrix
   ^  o fun-08 sec-04 sub-02:
   ^    - Start loops and get each score
   ^  o fun-08 sec-04 sub-03:
   ^    - Check if is an alternative base best score
   ^  o fun-08 sec-04 sub-04:
   ^    - Find the best score for the last base
   ^  o fun-08 sec-04 sub-05:
   ^    - Is last base in row an alternative alignment?
   ^  o fun-08 sec-04 sub-06:
   ^    - Prepare to score the next row in the matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-04 Sub-01:
   *  - Final set up before scoring the matrix
   \*****************************************************/

   /*Move the row of starting indexes to the last row*/
   swapPtrUL = indexHeapUL;
   indexHeapUL = oldIndexHeapUL;
   oldIndexHeapUL = swapPtrUL;

   nextSnpScoreL = scoreHeapSL[0];

   /*These are always negative*/
   delScoreL = 0;
   indexHeapUL[0] = indexUL;
   dirRowHeapSC[0] = defMvStop;

   /*Incurment to the frist base*/
   ++indexUL;
   refSeqStr = refST->seqCStr + refST->offsetUL - 1;
   qrySeqStr = qryST->seqCStr + qryST->offsetUL;

   /*****************************************************\
   * Fun-08 Sec-04 Sub-02:
   *  - Start loops and get each score
   \*****************************************************/

   /*Starting on the first sequence row*/
   for(
      ulQryBase = 0;
      ulQryBase <= qryST->endAlnUL - qryST->offsetUL;
      ++ulQryBase
   ){ /*loop; compare query base against all ref bases*/

      for(
         ulRefBase = 1;
         ulRefBase < lenRefUL;
         ++ulRefBase
      ){ /*loop; compare one query to one reference base*/
         waterMaxScore(
            refSeqStr[ulRefBase],
            qrySeqStr[ulQryBase],
            gapDiffS,
            scoreHeapSL[ulRefBase],
            dirRowHeapSC[ulRefBase],
            indexHeapUL[ulRefBase],
            nextSnpScoreL,
            delScoreL,
            oldIndexHeapUL[ulRefBase - 1], 
            oldIndexHeapUL[ulRefBase], 
            indexHeapUL[ulRefBase - 1],
            indexUL,
            settings
         );

         /***********************************************\
         * Fun-08 Sec-04 Sub-03:
         *  - Determine if is best score (keep as primary)
         \***********************************************/

         if(scoreSL < scoreHeapSL[ulRefBase])
         { /*If: this is the best score*/
            scoreSL = scoreHeapSL[ulRefBase];
            bestStartUL = indexHeapUL[ulRefBase];
            bestEndUL = indexUL;
         } /*If: this was an snp or match*/

         ++indexUL;
      } /*loop; compare one query to one reference base*/

      /***************************************************\
      * Fun-08 Sec-04 Sub-04:
      *  - Find the best score for the last base
      \***************************************************/

      waterMaxEndRowScore(
         refSeqStr[ulRefBase],
         qrySeqStr[ulQryBase],
         gapDiffS,
         scoreHeapSL[ulRefBase],
         dirRowHeapSC[ulRefBase],
         indexHeapUL[ulRefBase],
         nextSnpScoreL,
         delScoreL,
         oldIndexHeapUL[ulRefBase - 1], 
         oldIndexHeapUL[ulRefBase], 
         indexHeapUL[ulRefBase - 1],
         indexUL,
         settings
      );

     /***************************************************\
     * Fun-08 Sec-04 Sub-05:
     *  - Is last base in row an alternative alignment?
     \***************************************************/

     if(scoreSL < scoreHeapSL[ulRefBase])
     { /*If: this is the best score*/
        scoreSL = scoreHeapSL[ulRefBase];
        bestStartUL = indexHeapUL[ulRefBase];
        bestEndUL = indexUL;
     } /*If: this was an snp or match*/

     ++indexUL;

     /***************************************************\
     *  Fun-08 Sec-04 Sub-06:
     *   - Prepare for the next round
     \***************************************************/

      /*Get scores set up*/
      nextSnpScoreL = scoreHeapSL[0];

      dirRowHeapSC[0] = defMvStop;
      scoreHeapSL[0] += settings->gapExtendC;
      scoreHeapSL[0] &= -(scoreHeapSL[0] > 0);

      delScoreL = scoreHeapSL[0] + settings->gapExtendC;
      delScoreL &= -(delScoreL > 0);

      /*Swap index arrays so the current is last*/
      swapPtrUL = indexHeapUL;
      indexHeapUL = oldIndexHeapUL;
      oldIndexHeapUL = swapPtrUL;

      ++indexUL; /*Set index for the next base pair*/
   } /*loop; compare query base against all ref bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-05:
   ^  - Set up for returing the matrix (clean up/wrap up)
   ^  o fun-08 sec-05 sub-01:
   ^    - clean up
   ^  o fun-08 sec-05 sub-02:
   ^    - find the best score
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-05 Sub-01:
   *  - clean up
   \*****************************************************/

   indexToCoord(
      lenRefUL,
      bestStartUL,
      *refStartUL,
      *qryStartUL
   ); /*Convert the starting index to coordinates*/

   *refStartUL += refST->offsetUL;
   *qryStartUL += qryST->offsetUL;

   indexToCoord(
      lenRefUL,
      bestEndUL,
      *refEndUL,
      *qryEndUL
   ); /*Convert the ending index to coordinates*/

   *refEndUL += refST->offsetUL;
   *qryEndUL += qryST->offsetUL;

   free(dirRowHeapSC);
   dirRowHeapSC = 0;

   free(scoreHeapSL);
   scoreHeapSL = 0;

   free(indexHeapUL);
   indexHeapUL = 0;

   free(oldIndexHeapUL);
   oldIndexHeapUL = 0;

   return scoreSL;

   memErr_fun08_sec05_sub01:;

   free(dirRowHeapSC);
   dirRowHeapSC = 0;

   free(scoreHeapSL);
   scoreHeapSL = 0;

   free(indexHeapUL);
   indexHeapUL = 0;

   free(oldIndexHeapUL);
   oldIndexHeapUL = 0;

   return 0;
} /*memWaterAln*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
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
