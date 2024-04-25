/*########################################################
# Name alnSetStruct
# Use:
#  o Holds the settings structures and supporting
#    functions for setting structures for alnSeq's
#    pairwise aligners.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - included libraries
'  o .h st-01 alnSet:
'     o Holds settings for my alignment program
'  o .h fun-01 setBpScore:
'    - Sets the score for a base pair (reference/query)
'  o .h fun-02 setIfBpMatch:
'    - Sets if two bases are a match or mismtach
'  o fun-03 freeAlnSetStack:
'    o Frees variables inside alnSet
'  o fun-04 freeAlnSet:
'    o Frees an alnSet structure (and sets to 0)
'  o .h fun-05 getBaseScore:
'    - Get the score for a pair of bases from an alignment
'  o .h fun-06 matchOrSnp:
'    - Check if two bases were a match or mismatch
'  o fun-07 readInScoreFile
'     - Reads in a file of scores for a scoring matrix
'  o fun-08 readInMatchFile:
'    - Reads in a file of matches
'  o .h fun-09 seqToLookupIndex:
'    - Converts a sequence to a look up table index
'  o .h fun-10 lookupIndexToSeq:
'    - Converts a sequence of lookup indexs back into
'      uppercase characters (a-z)
'  o fun-11 initAlnSet:
'    - Set all values in altSet (alingment settings)
'      structure to defaults
'  o license:
'    - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "alnSetStruct.h"

#include <stdio.h>

#include "alnSeqDefaults.h"
#include "../generalLib/base10StrToNum.h"
#include "../generalLib/dataTypeShortHand.h"

/*-------------------------------------------------------\
| Fun-03: freeAlnSetStack
|  - Does a stack free of an alnSet structer
| Input:
|  - alnSetSTPtr:
|    o alnSetSTPtr to free internal variables in
| Output:
|  - Free:
|    o Nothing, there are no heap allocated variables.
|      Is here in case I decide to have heap allocated
|      variables on day.
\-------------------------------------------------------*/
void
freeAlnSetStack(
   struct alnSet *alnSetSTPtr
){}

/*-------------------------------------------------------\
| Fun-04: freeAlnSet
|  - Frees and alnSet (alignment settings) structure
| Input:
|  - alnSetSTPtr:
|    o Pionter to alnSetST to free
| Output:
|  - Free:
|    o alnSetSTPtr
|  - Set:
|    o alnSetSTPtr to 0 (NULL)
\-------------------------------------------------------*/
void
freeAlnSet(
   struct alnSet *alnSetSTPtr
){
   freeAlnSetStack(alnSetSTPtr);
   free(alnSetSTPtr);
} /*freeAlnSet*/

/*-------------------------------------------------------\
| Fun-07: readInScoreFile
|  - Reads in a file of scores for a scoring matrix
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the score matrix to modify
|  - scoreFILE:
|    o File to get scores from
| Output:
|  - Modifies:
|    o Score matrix in alngSetST to hold the scores from
|      the file (scoreFILE)
|    o scoreFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readInScoreFile(
    struct alnSet *alnSetSTPtr, /*score matrix to change*/
    FILE *scoreFILE  /*File of scores for scoring matrix*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-07 TOC: readInScoreFile
   '  o fun-07 sec-01:
   '    - Variable declerations & set up
   '  o fun-07 sec-02:
   '    - Blank the scoring matrix
   '  o fun-07 sec-03:
   '    - Read in line and check if comment
   '  o fun-07 sec-04:
   '    - Convert score & add to matrix
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-01:
   ^  - Variable declerations and set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort lenBuffUS = 1024;
   char buffCStr[lenBuffUS];
   char *tmpCStr = 0;
   char scoreC = 0;

   uchar colUC = 0;
   uchar rowUC = 0;

   buffCStr[lenBuffUS - 1] = '\0';
   buffCStr[lenBuffUS - 2] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-02:
   ^  - Blank the scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*Loop: blank all values in the scoring matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetSTPtr->scoreMatrixC[colUC][rowUC] = 0;
   } /*Loop: blank all values in the scoring matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-03:
   ^  - Read in line and check if comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(fgets(buffCStr, 1024, scoreFILE))
   { /*While I have scores to read in*/
       
       if(buffCStr[0] == '/' && buffCStr[1] == '/')
       { /*On a comment, move onto the next line*/
           while(
               buffCStr[lenBuffUS - 2] != '\0' &&
               buffCStr[lenBuffUS - 2] != '\n'
           ) { /*While have more buffer to read in*/
               buffCStr[lenBuffUS - 2] = '\0';
               fgets(buffCStr, 1024, scoreFILE);
           } /*While have more buffer to read in*/

           /*Reset the buffer*/
           buffCStr[lenBuffUS - 2] = '\0';

           continue;
       } /*On a comment, move onto the next line*/

       /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
       ^ Fun-07 Sec-04:
       ^  - Convert score & add to matrix
       \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

       if(buffCStr[0] == '\n')
           continue;                        /*Blank line*/

       if(buffCStr[0] < 64 && buffCStr[2] < 64)
           return ftell(scoreFILE);  /*Invalid character*/
       
       tmpCStr = base10StrToSC(&buffCStr[4], scoreC);

       setBpScore(
         buffCStr[0],
         buffCStr[2],
         scoreC,
         alnSetSTPtr
       ); /*Add the score to the matrix*/

       if(tmpCStr == &buffCStr[3])
           return ftell(scoreFILE);         /*No score*/

       while(
           buffCStr[lenBuffUS - 2] != '\0' &&
           buffCStr[lenBuffUS - 2] != '\n'
       ){ /*While have more buffer to read in*/
           buffCStr[lenBuffUS - 2] = '\0';
           fgets(buffCStr, 1024, scoreFILE);
       } /*While have more buffer to read in*/

       /*Reset the buffer*/
       buffCStr[lenBuffUS - 2] = '\0';
   } /*While I have scores to read in*/

   return 0;
} /*readInScoreFile*/

/*-------------------------------------------------------\
| Fun-08: readInMatchFile
|  - Reads in a file of matches
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the match matrix to modify
|  - matchFILE:
|    o File to get matchs from
| Output:
|  - Modifies:
|    o Match matrix in alngSetST to hold the matchs from
|      the file (matchFILE)
|    o matchFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readInMatchFile(
    struct alnSet *alnSetSTPtr,
    void *matchFILE  /*File of matchs for scoring matrix*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: readInMatchFile
   '  o fun-08 sec-01:
   '    - Variable declerations & set up
   '  o fun-08 sec-02:
   '    - Blank the match matrix
   '  o fun-08 sec-03:
   '    - Read in line and check if comment
   '  o fun-08 sec-04:
   '    - Add match/snp (mismatch) to match matrix
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^  - Variable declerations and set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort lenBuffUS = 1024;
   char buffCStr[lenBuffUS];

   uchar colUC = 0;
   uchar rowUC = 0;

   buffCStr[lenBuffUS - 1] = '\0';
   buffCStr[lenBuffUS - 2] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^  - Blank the scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*Loop: blank all values in the scoring matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetSTPtr->matchMatrixBl[colUC][rowUC] = 0;
   } /*Loop: blank all values in the scoring matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^  - Read in line and check if comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(fgets(buffCStr, 1024, (FILE *) matchFILE))
   { /*While I have matchs to read in*/
       
       if(buffCStr[0] == '/' && buffCStr[1] == '/')
       { /*On a comment, move onto the next line*/
           while(
               buffCStr[lenBuffUS - 2] != '\0' &&
               buffCStr[lenBuffUS - 2] != '\n'
           ) { /*While have more buffer to read in*/
               buffCStr[lenBuffUS - 2] = '\0';
               fgets(buffCStr, 1024, (FILE *) matchFILE);
           } /*While have more buffer to read in*/

           /*Reset the buffer*/
           buffCStr[lenBuffUS - 2] = '\0';

           continue;
       } /*On a comment, move onto the next line*/

       /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
       ^ Fun-08 Sec-04:
       ^  - Convert match & add to matrix
       \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

       if(buffCStr[0] == '\n')
           continue;                        /*Blank line*/

       if(buffCStr[4] != '1' && buffCStr[4] != '0')
           return ftell((FILE *) matchFILE);
           /*This error means I do not know if match/snp*/

       setIfBpMatch(
         buffCStr[0],
         buffCStr[2],
         buffCStr[4],
         alnSetSTPtr
       ); /*Add the match to the matrix*/

       while(
           buffCStr[lenBuffUS - 2] != '\0' &&
           buffCStr[lenBuffUS - 2] != '\n'
       ){ /*While have more buffer to read in*/
           buffCStr[lenBuffUS - 2] = '\0';
           fgets(buffCStr, 1024, (FILE *) matchFILE);
       } /*While have more buffer to read in*/

       /*Reset the buffer*/
       buffCStr[lenBuffUS - 2] = '\0';
   } /*While I have matchs to read in*/

   return 0;
} /*readInMatchFile*/

/*-------------------------------------------------------\
| Fun-11: initAlnSet
|  - Set values in altSet (alingment settings) structure
|    to default values
| Input:
|  - alnSetSTPtr:
|    o poineter to an alnSet (settings) structure to
|      initialize
| Output:
|  o Modifies:
|    - alnSetST to have default alignment settings values
\-------------------------------------------------------*/
void
initAlnSet(
    struct alnSet *alnSetST /*Has settings to initialize*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC: initAlnSet
   '  - Set values in altSet (alingment settings)
   '    structure to defaults
   '  o fun-11 sec-01:
   '    - Set non-matrix variables
   '  o fun-11 sec-02:
   '    - Initialize scoring matrix
   '  o fun-11 sec-03:
   '    - Initialize match matrix
   '  o fun-11 sec-04:
   '    - Handle special DNA scoring cases
   '  o fun-11 sec-05:
   '    - Handle special DNA match cases
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-01:
   ^  - Set non-matrix variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Variables for my for loop*/
   uchar colUC = 0;
   uchar rowUC = 0;

   alnSetST->gapOpenC = defGapOpen;
   alnSetST->gapExtendC = defGapExtend;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^  - Initialize scoring matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(colUC = 0; colUC < defMatrixCol; ++colUC)
   { /*loop for all columns in the comparison matrix*/
       for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
           alnSetST->scoreMatrixC[colUC][rowUC] = 0;
           /*Most of these cells will never be used*/
           /*But are needed to build the table*/
   } /*loop for all columns in the comparison matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-03:
   ^  - Initialize match matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Both words, DNA, and AA all are the same when both
    ` characters/bases/amino acids are the same.
    */
    for(colUC = 0; colUC < defMatrixCol; ++colUC)
    { /*loop for all columns in the comparison matrix*/
        for(rowUC = 0; rowUC < defMatrixCol; ++rowUC)
        { /*Loop: Fill in the entire matrix*/ 
           if(colUC == rowUC)
             alnSetST->matchMatrixBl[colUC][rowUC] =
                defBaseMatch;
           else
             alnSetST->matchMatrixBl[colUC][rowUC] =
                defBaseSnp;
        } /*Loop: Fill in the entire matrix*/ 
    } /*loop for all columns in the comparison matrix*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-04:
   ^  - Handle special DNA scoring cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Set up scores for non-anonmyous base pairs*/
   setBpScore('a', 'a', defAToA, alnSetST);
   setBpScore('a', 't', defAToT, alnSetST);
   setBpScore('a', 'u', defAToT, alnSetST);
   setBpScore('a', 'g', defAToG, alnSetST);
   setBpScore('a', 'c', defAToC, alnSetST);

   setBpScore('t', 'a', defTToA, alnSetST);
   setBpScore('t', 't', defTToT, alnSetST);
   setBpScore('t', 'u', defTToT, alnSetST);
   setBpScore('t', 'g', defTToG, alnSetST);
   setBpScore('t', 'c', defTToC, alnSetST);

   setBpScore('u', 'a', defTToA, alnSetST);
   setBpScore('u', 'u', defTToT, alnSetST);
   setBpScore('u', 'g', defTToG, alnSetST);
   setBpScore('u', 'c', defTToC, alnSetST);
   setBpScore('u', 't', defTToT, alnSetST);

   setBpScore('g', 'a', defGToA, alnSetST);
   setBpScore('g', 't', defGToT, alnSetST);
   setBpScore('g', 'u', defGToT, alnSetST);
   setBpScore('g', 'g', defGToG, alnSetST);
   setBpScore('g', 'c', defGToC, alnSetST);

   setBpScore('c', 'a', defCToA, alnSetST);
   setBpScore('c', 't', defCToT, alnSetST);
   setBpScore('c', 'u', defCToT, alnSetST);
   setBpScore('c', 'g', defCToG, alnSetST);
   setBpScore('c', 'c', defCToC, alnSetST);

   /*non-anonymous base and anonymous base pairs*/
   setBpScore('a', 'w', defAToW, alnSetST);
   setBpScore('a', 's', defAToS, alnSetST);
   setBpScore('a', 'm', defAToM, alnSetST);
   setBpScore('a', 'k', defAToK, alnSetST);
   setBpScore('a', 'r', defAToR, alnSetST);
   setBpScore('a', 'y', defAToY, alnSetST);
   setBpScore('a', 'b', defAToB, alnSetST);
   setBpScore('a', 'd', defAToD, alnSetST);
   setBpScore('a', 'h', defAToH, alnSetST);
   setBpScore('a', 'v', defAToV, alnSetST);
   setBpScore('a', 'n', defAToN, alnSetST);
   setBpScore('a', 'x', defAToX, alnSetST);

   setBpScore('w', 'a', defWToA, alnSetST);
   setBpScore('s', 'a', defSToA, alnSetST);
   setBpScore('m', 'a', defMToA, alnSetST);
   setBpScore('k', 'a', defKToA, alnSetST);
   setBpScore('r', 'a', defRToA, alnSetST);
   setBpScore('y', 'a', defYToA, alnSetST);
   setBpScore('b', 'a', defBToA, alnSetST);
   setBpScore('d', 'a', defDToA, alnSetST);
   setBpScore('h', 'a', defHToA, alnSetST);
   setBpScore('v', 'a', defVToA, alnSetST);
   setBpScore('n', 'a', defNToA, alnSetST);
   setBpScore('x', 'a', defXToA, alnSetST);

   setBpScore('c', 'w', defCToW, alnSetST);
   setBpScore('c', 's', defCToS, alnSetST);
   setBpScore('c', 'm', defCToM, alnSetST);
   setBpScore('c', 'k', defCToK, alnSetST);
   setBpScore('c', 'r', defCToR, alnSetST);
   setBpScore('c', 'y', defCToY, alnSetST);
   setBpScore('c', 'b', defCToB, alnSetST);
   setBpScore('c', 'd', defCToD, alnSetST);
   setBpScore('c', 'h', defCToH, alnSetST);
   setBpScore('c', 'v', defCToV, alnSetST);
   setBpScore('c', 'n', defCToN, alnSetST);
   setBpScore('c', 'x', defCToX, alnSetST);

   setBpScore('w', 'c', defWToC, alnSetST);
   setBpScore('s', 'c', defSToC, alnSetST);
   setBpScore('m', 'c', defMToC, alnSetST);
   setBpScore('k', 'c', defKToC, alnSetST);
   setBpScore('r', 'c', defRToC, alnSetST);
   setBpScore('y', 'c', defYToC, alnSetST);
   setBpScore('b', 'c', defBToC, alnSetST);
   setBpScore('d', 'c', defDToC, alnSetST);
   setBpScore('h', 'c', defHToC, alnSetST);
   setBpScore('v', 'c', defVToC, alnSetST);
   setBpScore('n', 'c', defNToC, alnSetST);
   setBpScore('x', 'c', defXToC, alnSetST);

   setBpScore('g', 'w', defGToW, alnSetST);
   setBpScore('g', 's', defGToS, alnSetST);
   setBpScore('g', 'm', defGToM, alnSetST);
   setBpScore('g', 'k', defGToK, alnSetST);
   setBpScore('g', 'r', defGToR, alnSetST);
   setBpScore('g', 'y', defGToY, alnSetST);
   setBpScore('g', 'b', defGToB, alnSetST);
   setBpScore('g', 'd', defGToD, alnSetST);
   setBpScore('g', 'h', defGToH, alnSetST);
   setBpScore('g', 'v', defGToV, alnSetST);
   setBpScore('g', 'n', defGToN, alnSetST);
   setBpScore('g', 'x', defGToX, alnSetST);

   setBpScore('w', 'g', defWToG, alnSetST);
   setBpScore('s', 'g', defSToG, alnSetST);
   setBpScore('m', 'g', defMToG, alnSetST);
   setBpScore('k', 'g', defKToG, alnSetST);
   setBpScore('r', 'g', defRToG, alnSetST);
   setBpScore('y', 'g', defYToG, alnSetST);
   setBpScore('b', 'g', defBToG, alnSetST);
   setBpScore('d', 'g', defDToG, alnSetST);
   setBpScore('h', 'g', defHToG, alnSetST);
   setBpScore('v', 'g', defVToG, alnSetST);
   setBpScore('n', 'g', defNToG, alnSetST);
   setBpScore('x', 'g', defXToG, alnSetST);

   setBpScore('t', 'w', defTToW, alnSetST);
   setBpScore('t', 's', defTToS, alnSetST);
   setBpScore('t', 'm', defTToM, alnSetST);
   setBpScore('t', 'k', defTToK, alnSetST);
   setBpScore('t', 'r', defTToR, alnSetST);
   setBpScore('t', 'y', defTToY, alnSetST);
   setBpScore('t', 'b', defTToB, alnSetST);
   setBpScore('t', 'd', defTToD, alnSetST);
   setBpScore('t', 'h', defTToH, alnSetST);
   setBpScore('t', 'v', defTToV, alnSetST);
   setBpScore('t', 'n', defTToN, alnSetST);
   setBpScore('t', 'x', defTToX, alnSetST);

   setBpScore('w', 't', defWToT, alnSetST);
   setBpScore('s', 't', defSToT, alnSetST);
   setBpScore('m', 't', defMToT, alnSetST);
   setBpScore('k', 't', defKToT, alnSetST);
   setBpScore('r', 't', defRToT, alnSetST);
   setBpScore('y', 't', defYToT, alnSetST);
   setBpScore('b', 't', defBToT, alnSetST);
   setBpScore('d', 't', defDToT, alnSetST);
   setBpScore('h', 't', defHToT, alnSetST);
   setBpScore('v', 't', defVToT, alnSetST);
   setBpScore('n', 't', defNToT, alnSetST);
   setBpScore('x', 't', defXToT, alnSetST);

   /*Set u & t to same scores (U is RNA version of T)*/
   setBpScore('u', 'w', defTToW, alnSetST);
   setBpScore('u', 's', defTToS, alnSetST);
   setBpScore('u', 'm', defTToM, alnSetST);
   setBpScore('u', 'k', defTToK, alnSetST);
   setBpScore('u', 'r', defTToR, alnSetST);
   setBpScore('u', 'y', defTToY, alnSetST);
   setBpScore('u', 'b', defTToB, alnSetST);
   setBpScore('u', 'd', defTToD, alnSetST);
   setBpScore('u', 'h', defTToH, alnSetST);
   setBpScore('u', 'v', defTToV, alnSetST);
   setBpScore('u', 'n', defTToN, alnSetST);
   setBpScore('u', 'x', defTToX, alnSetST);

   setBpScore('w', 'u', defWToT, alnSetST);
   setBpScore('s', 'u', defSToT, alnSetST);
   setBpScore('m', 'u', defMToT, alnSetST);
   setBpScore('k', 'u', defKToT, alnSetST);
   setBpScore('r', 'u', defRToT, alnSetST);
   setBpScore('y', 'u', defYToT, alnSetST);
   setBpScore('b', 'u', defBToT, alnSetST);
   setBpScore('d', 'u', defDToT, alnSetST);
   setBpScore('h', 'u', defHToT, alnSetST);
   setBpScore('v', 'u', defVToT, alnSetST);
   setBpScore('n', 'u', defNToT, alnSetST);
   setBpScore('x', 'u', defXToT, alnSetST);

   /*anonymous base and anonymous base pairs*/
   setBpScore('w', 'w', defWToW, alnSetST);
   setBpScore('w', 's', defWToS, alnSetST);
   setBpScore('w', 'm', defWToM, alnSetST);
   setBpScore('w', 'k', defWToK, alnSetST);
   setBpScore('w', 'r', defWToR, alnSetST);
   setBpScore('w', 'y', defWToY, alnSetST);
   setBpScore('w', 'b', defWToB, alnSetST);
   setBpScore('w', 'd', defWToD, alnSetST);
   setBpScore('w', 'h', defWToH, alnSetST);
   setBpScore('w', 'v', defWToV, alnSetST);
   setBpScore('w', 'n', defWToN, alnSetST);
   setBpScore('w', 'x', defWToX, alnSetST);

   setBpScore('s', 'w', defSToW, alnSetST);
   setBpScore('s', 's', defSToS, alnSetST);
   setBpScore('s', 'm', defSToM, alnSetST);
   setBpScore('s', 'k', defSToK, alnSetST);
   setBpScore('s', 'r', defSToR, alnSetST);
   setBpScore('s', 'y', defSToY, alnSetST);
   setBpScore('s', 'b', defSToB, alnSetST);
   setBpScore('s', 'd', defSToD, alnSetST);
   setBpScore('s', 'h', defSToH, alnSetST);
   setBpScore('s', 'v', defSToV, alnSetST);
   setBpScore('s', 'n', defSToN, alnSetST);
   setBpScore('s', 'x', defSToX, alnSetST);

   setBpScore('m', 'w', defMToW, alnSetST);
   setBpScore('m', 's', defMToS, alnSetST);
   setBpScore('m', 'm', defMToM, alnSetST);
   setBpScore('m', 'k', defMToK, alnSetST);
   setBpScore('m', 'r', defMToR, alnSetST);
   setBpScore('m', 'y', defMToY, alnSetST);
   setBpScore('m', 'b', defMToB, alnSetST);
   setBpScore('m', 'd', defMToD, alnSetST);
   setBpScore('m', 'h', defMToH, alnSetST);
   setBpScore('m', 'v', defMToV, alnSetST);
   setBpScore('m', 'n', defMToN, alnSetST);
   setBpScore('m', 'x', defMToX, alnSetST);

   setBpScore('k', 'w', defKToW, alnSetST);
   setBpScore('k', 's', defKToS, alnSetST);
   setBpScore('k', 'm', defKToM, alnSetST);
   setBpScore('k', 'k', defKToK, alnSetST);
   setBpScore('k', 'r', defKToR, alnSetST);
   setBpScore('k', 'y', defKToY, alnSetST);
   setBpScore('k', 'b', defKToB, alnSetST);
   setBpScore('k', 'd', defKToD, alnSetST);
   setBpScore('k', 'h', defKToH, alnSetST);
   setBpScore('k', 'v', defKToV, alnSetST);
   setBpScore('k', 'n', defKToN, alnSetST);
   setBpScore('k', 'x', defKToX, alnSetST);

   setBpScore('r', 'w', defRToW, alnSetST);
   setBpScore('r', 's', defRToS, alnSetST);
   setBpScore('r', 'm', defRToM, alnSetST);
   setBpScore('r', 'k', defRToK, alnSetST);
   setBpScore('r', 'r', defRToR, alnSetST);
   setBpScore('r', 'y', defRToY, alnSetST);
   setBpScore('r', 'b', defRToB, alnSetST);
   setBpScore('r', 'd', defRToD, alnSetST);
   setBpScore('r', 'h', defRToH, alnSetST);
   setBpScore('r', 'v', defRToV, alnSetST);
   setBpScore('r', 'n', defRToN, alnSetST);
   setBpScore('r', 'x', defRToX, alnSetST);

   setBpScore('y', 'w', defYToW, alnSetST);
   setBpScore('y', 's', defYToS, alnSetST);
   setBpScore('y', 'm', defYToM, alnSetST);
   setBpScore('y', 'k', defYToK, alnSetST);
   setBpScore('y', 'r', defYToR, alnSetST);
   setBpScore('y', 'y', defYToY, alnSetST);
   setBpScore('y', 'b', defYToB, alnSetST);
   setBpScore('y', 'd', defYToD, alnSetST);
   setBpScore('y', 'h', defYToH, alnSetST);
   setBpScore('y', 'v', defYToV, alnSetST);
   setBpScore('y', 'n', defYToN, alnSetST);
   setBpScore('y', 'x', defYToX, alnSetST);

   setBpScore('b', 'w', defBToW, alnSetST);
   setBpScore('b', 's', defBToS, alnSetST);
   setBpScore('b', 'm', defBToM, alnSetST);
   setBpScore('b', 'k', defBToK, alnSetST);
   setBpScore('b', 'r', defBToR, alnSetST);
   setBpScore('b', 'y', defBToY, alnSetST);
   setBpScore('b', 'b', defBToB, alnSetST);
   setBpScore('b', 'd', defBToD, alnSetST);
   setBpScore('b', 'h', defBToH, alnSetST);
   setBpScore('b', 'v', defBToV, alnSetST);
   setBpScore('b', 'n', defBToN, alnSetST);
   setBpScore('b', 'x', defBToX, alnSetST);

   setBpScore('d', 'w', defDToW, alnSetST);
   setBpScore('d', 's', defDToS, alnSetST);
   setBpScore('d', 'm', defDToM, alnSetST);
   setBpScore('d', 'k', defDToK, alnSetST);
   setBpScore('d', 'r', defDToR, alnSetST);
   setBpScore('d', 'y', defDToY, alnSetST);
   setBpScore('d', 'b', defDToB, alnSetST);
   setBpScore('d', 'd', defDToD, alnSetST);
   setBpScore('d', 'h', defDToH, alnSetST);
   setBpScore('d', 'v', defDToV, alnSetST);
   setBpScore('d', 'n', defDToN, alnSetST);
   setBpScore('d', 'x', defDToX, alnSetST);

   setBpScore('h', 'w', defHToW, alnSetST);
   setBpScore('h', 's', defHToS, alnSetST);
   setBpScore('h', 'm', defHToM, alnSetST);
   setBpScore('h', 'k', defHToK, alnSetST);
   setBpScore('h', 'r', defHToR, alnSetST);
   setBpScore('h', 'y', defHToY, alnSetST);
   setBpScore('h', 'b', defHToB, alnSetST);
   setBpScore('h', 'd', defHToD, alnSetST);
   setBpScore('h', 'h', defHToH, alnSetST);
   setBpScore('h', 'v', defHToV, alnSetST);
   setBpScore('h', 'n', defHToN, alnSetST);
   setBpScore('h', 'x', defHToX, alnSetST);

   setBpScore('v', 'w', defVToW, alnSetST);
   setBpScore('v', 's', defVToS, alnSetST);
   setBpScore('v', 'm', defVToM, alnSetST);
   setBpScore('v', 'k', defVToK, alnSetST);
   setBpScore('v', 'r', defVToR, alnSetST);
   setBpScore('v', 'y', defVToY, alnSetST);
   setBpScore('v', 'b', defVToB, alnSetST);
   setBpScore('v', 'd', defVToD, alnSetST);
   setBpScore('v', 'h', defVToH, alnSetST);
   setBpScore('v', 'v', defVToV, alnSetST);
   setBpScore('v', 'n', defVToN, alnSetST);
   setBpScore('v', 'x', defVToX, alnSetST);

   setBpScore('n', 'w', defNToW, alnSetST);
   setBpScore('n', 's', defNToS, alnSetST);
   setBpScore('n', 'm', defNToM, alnSetST);
   setBpScore('n', 'k', defNToK, alnSetST);
   setBpScore('n', 'r', defNToR, alnSetST);
   setBpScore('n', 'y', defNToY, alnSetST);
   setBpScore('n', 'b', defNToB, alnSetST);
   setBpScore('n', 'd', defNToD, alnSetST);
   setBpScore('n', 'h', defNToH, alnSetST);
   setBpScore('n', 'v', defNToV, alnSetST);
   setBpScore('n', 'n', defNToN, alnSetST);
   setBpScore('n', 'x', defNToX, alnSetST);

   setBpScore('x', 'w', defXToW, alnSetST);
   setBpScore('x', 's', defXToS, alnSetST);
   setBpScore('x', 'm', defXToM, alnSetST);
   setBpScore('x', 'k', defXToK, alnSetST);
   setBpScore('x', 'r', defXToR, alnSetST);
   setBpScore('x', 'y', defXToY, alnSetST);
   setBpScore('x', 'b', defXToB, alnSetST);
   setBpScore('x', 'd', defXToD, alnSetST);
   setBpScore('x', 'h', defXToH, alnSetST);
   setBpScore('x', 'v', defXToV, alnSetST);
   setBpScore('x', 'n', defXToN, alnSetST);
   setBpScore('x', 'x', defXToX, alnSetST);

   return;
} /*initAlnSet*/

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
