/*########################################################
# Name: samEntryStruct
#  - Holds structer to hold a sam file entry. This also
#    includes the functions needed to support this
#    structer.
# Libraries
#  - "base10StrToNum.h"     (No .c file)
#  o "dataTypeShortHand.h"  (No .c file)
#  - "ulCpStr.h"            (No .c file)
#  - "numToStr.h"           (No .c file)
# C Standard Libraries:
#  - <stdlib.h>
#  - <stdio.h>
#  o <limits.h>
# Note:
#  - End of file has some general sam file basics
########################################################*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, definitions and preprocessor
|     checks
\-------------------------------------------------------*/

#ifndef SAMENTRYSTRUCT_H
#define SAMENTRYSTRUCT_H

#include <stdlib.h>
#include <stdio.h>

#include "base10StrToNum.h"
#include "ulCpStr.h"
#include "numToStr.h"

#define defQAdjust 33 /*offest to get q-score of 0*/
#define defMaxQScore 94 /*highest possible Q-score*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o struct-01 samEntry:
'    - Holds a single samfile entry
'  o fun-01 blankSamEntry:
'    - Sets all non-alloacted variables in samEntryST to 0
'  o fun-02 initSamEntry:
'    - Initalize a samEntry struct to 0's
'  o fun-03 freeSamEntryStack:
'    - Frees heap allocations in a stack allocated
'      samEntry struct
'  o fun-04 freeSamEntry:
'    - Frees a samEntry structer (and sets to null)
'  o fun-05: makeSamEntry
'    - Makes an heap allocated samEntry structure
'  o fun-08 processSamEntry:
'    - Sets Q-score, cigar, & sequence pointers and 
'      stats in a sam entry sotred in a samEntry struct.
'  o fun-09 readNextPartOfLine:
'    - Reads in next part of line when fgets did not
'      get a full line
'  o fun-10 findSamCig:
'    - Finds the cigar in same entry & also finds the
'      number of bases in query from cigar
'  o fun-11 readSamLine:
'    - Reads in sam entry into a samEntry struct
'  o fun-12 printSamEntry:
'    - Prints the sam file entry to a file. This does not
'      print any extra stats that were found.
'  o fun-13 samToFq:
'    - Prints sam entry as a fastq entry to a fastq file
'  o fun-14 printStatsHeader:
'    - Prints the stat file header made using a samEntry
'      struct
'  o fun-15 printSamStats
'    - Prints stats in samEntry structure to a tsv file
'  o fun-16 swapSamEntries:
'    - Swaps the values in two sam entries. This includes
'      histograms, so will take a bit of time.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Struct-01: samEntry
|  - Holds a single samfile entry
\-------------------------------------------------------*/
typedef struct samEntry
{ /*samEntry*/
    /*Buffers for storing strings*/
    char qryIdStr[128];/*Holds the query id/name*/
    uchar lenQryIdUC;  /*Length of query id/name*/

    char refIdStr[128];/*Holds the reference id/name*/
    uchar lenRefIdUC;  /*Length of reference id/name*/

    char *cigTypeStr;  /*Holds the cigar type entry*/
    int *cigValAryI;   /*Holds the cigar number*/
    uint lenCigUI;     /*Length of the cigar entry*/
    uint lenCigBuffUI; /*# bytes allocated to cigStr*/

    char rNextStr[128];/*Holds the rNext entry*/
    uchar lenRNextUC;  /*Length of rNextStr*/

    /*The sequence and Q-scores entries have lengths
    ` stored in readLenUI
    */
    char *seqStr;      /*Holds the sequence entry*/
    uint lenSeqBuffUI; /*#bytes allocated to seqStr/qStr*/

    char *qStr;        /*Holds the q-score entry*/
    uint lenQBuffUI; /*#bytes allocated to seqStr/qStr*/

    char *extraStr;    /*Extra entries (columns 12-end)*/
    uint lenExtraUI;   /*Length of c-string in extraStr*/
    uint lenExtraBuffUI;/*# bytes allocated to qStr*/

    /*Flags/single numeric values in the sam entry*/
    uchar mapqUC;      /*Holds mapping quality*/
    ushort flagUS;     /*Holds the flag in samEntryCStr*/
    int pNextI;        /*PNEXT postion of mate/next read*/
    int tLenI;         /*TLEN observed template length*/

    /*Stats*/

    /*The Q-scores; includes the softmasked regions*/
    float medianQF;    /*holds median read Q-score*/
    float meanQF;      /*holds mean read Q-score*/
 
    uint refStartUI;   /*First reference base*/
    uint refEndUI;     /*Last reference base*/
    uint readLenUI;    /*Holds read length*/
    uint alnReadLenUI; /*Number of ref basea aligned to*/

    uint numMatchUI;   /*Holds the number of matches*/
    uint numSnpUI;     /*Holds the number of mismatches*/
    uint numInsUI;     /*Holds the number of insertions*/
    uint numDelUI;     /*the number of deletions*/
    uint numMaskUI;    /*the number of soft masked bases*/

    /*These variables are used in finding the Q-scores*/
    uint qHistUI[defMaxQScore]; /*Histogram for median Q*/
    ulong sumQUL;              /*Total for mean Q*/
}samEntry;

/*-------------------------------------------------------\
| Fun-01: blankSamEntry
| Use:
|  - Sets all values to 0, or for c-strings to '\0'
| Input:
|  - samSTPtr:
|    - Pointer to samEntry structure to blank
| Output:
|  - Modifies:
|    o Sets every variable to 0
|    o the first character in every c-string to be '\0'
\-------------------------------------------------------*/
#define blankSamEntry(samSTPtr){\
    int iIter = 0;\
    \
    (samSTPtr)->qryIdStr[0] = '\0';/*query id/name*/\
    (samSTPtr)->lenQryIdUC = 0;    /*Length of query id*/\
    \
    (samSTPtr)->refIdStr[0] = '\0';/*reference id/name*/\
    (samSTPtr)->lenRefIdUC = 0;  /*Length; reference id*/\
    \
    (samSTPtr)->lenCigUI = 0;     /*Length; cigar entry*/\
    \
    for(iIter=0; iIter <(samSTPtr)->lenCigBuffUI;++iIter)\
    { /*Loop: Clear all cigar entries*/\
       (samSTPtr)->cigTypeStr[iIter] = '\0';\
       (samSTPtr)->cigValAryI[iIter] = 0;\
    } /*Loop: Clear all cigar entries*/\
    \
    (samSTPtr)->rNextStr[0] = '\0'; /*rNext entry*/\
    (samSTPtr)->lenRNextUC = 0;     /*Length of rNext*/\
    \
    (samSTPtr)->seqStr[0] = '\0';   /*sequence entry*/\
    (samSTPtr)->qStr[0] = '\0';     /*q-score entry*/\
    \
    (samSTPtr)->extraStr[0] = '\0';    /*Extra entries*/\
    (samSTPtr)->lenExtraUI = 0;\
    \
    /*Flags/single numeric values in the sam entry*/\
    (samSTPtr)->mapqUC = 0;      /*mapping quality*/\
    (samSTPtr)->flagUS = 0;      /*flag in samEntryCStr*/\
    (samSTPtr)->pNextI = 0;      /*PNEXT postion*/\
    (samSTPtr)->tLenI = 0;       /*TLEN*/\
    \
    /*Stats*/\
    \
    /*The Q-scores excluded the softmasked regions*/\
    (samSTPtr)->medianQF = 0;    /*median read Q-score*/\
    (samSTPtr)->meanQF = 0;      /*mean read Q-score*/\
    \
    (samSTPtr)->refStartUI = 0;  /*First reference base*/\
    (samSTPtr)->refEndUI = 0;    /*Last reference base*/\
    (samSTPtr)->readLenUI = 0;   /*Holds read length*/\
    (samSTPtr)->alnReadLenUI = 0;/*Aligned length*/\
    \
    (samSTPtr)->numMatchUI = 0;  /*number of matches*/\
    (samSTPtr)->numSnpUI = 0;    /*number of mismatches*/\
    (samSTPtr)->numInsUI = 0;    /*number of insertions*/\
    (samSTPtr)->numDelUI = 0;    /*number of deletions*/\
    (samSTPtr)->numMaskUI = 0;   /*number soft masked*/\
    \
    /*These variables are used in finding the Q-scores*/\
    for(iIter = 0; iIter < defMaxQScore; ++iIter)\
       (samSTPtr)->qHistUI[iIter] = 0;\
    \
    (samSTPtr)->sumQUL = 0;\
} /*blankSamEntry*/

/*-------------------------------------------------------\
| Fun-03: initSamEntry
|  - Initializes a samEntry structure for use. This 
|    function should only ever be called once per
|    structure or after freeSamEntryStack has been used.
|    Use blankSamEntry to reset the values in a samEntry
|    struct after initialization.
| Input:
|  - samSTPtr:
|    o pointer to samEntry struct to initialize
| Output:
|  - Modifies:
|    o samEntry to have buffers and all values set to 0
|  - Returns:
|    o 0 for success
|    o 1 for memory error
\-------------------------------------------------------*/
#define initSamEntry(samSTPtr)({\
    uchar errUC = 0;\
    \
    (samSTPtr)->seqStr = calloc(1024, sizeof(char));\
    errUC |= (!(samSTPtr)->seqStr);\
    (samSTPtr)->lenSeqBuffUI = 1024 & ((int) errUC - 1);\
    \
    (samSTPtr)->qStr = calloc(1024, sizeof(char));\
    errUC |= (!(samSTPtr)->qStr);\
    (samSTPtr)->lenQBuffUI = 1024 & ((int) errUC - 1);\
    \
    (samSTPtr)->cigTypeStr = calloc(256, sizeof(char));\
    errUC |= (!(samSTPtr)->cigTypeStr);\
    (samSTPtr)->lenCigBuffUI = 256 & ((int) errUC - 1);\
    \
    (samSTPtr)->cigValAryI = calloc(256, sizeof(int));\
    errUC |= (!(samSTPtr)->cigValAryI);\
    \
    (samSTPtr)->extraStr = calloc(256, sizeof(char));\
    errUC |= (!(samSTPtr)->extraStr);\
    (samSTPtr)->lenExtraBuffUI = 256 & ((int) errUC - 1);\
    \
    /*Handle no memory errors*/\
    if(!errUC) blankSamEntry((samSTPtr));\
    \
    errUC;\
}) /*initSamEntry*/

/*-------------------------------------------------------\
| Fun-06: freeSamEntryStack
| Use:
|  - Frees all variables in samEntry, but not samEntry
| Input:
|  - samSTPtr
|    o Pointer to samEntry struct to free the interal
|      memory of
| Output:
|  - Frees:
|    o allocated memory in samSTPtr
\-------------------------------------------------------*/
#define freeSamEntryStack(samSTPtr){\
    blankSamEntry((samSTPtr));\
    \
    free((samSTPtr)->seqStr);\
    (samSTPtr)->seqStr = 0;\
    (samSTPtr)->lenSeqBuffUI = 0;\
    \
    free((samSTPtr)->qStr);\
    (samSTPtr)->qStr = 0;\
    (samSTPtr)->lenQBuffUI = 0;\
    \
    free((samSTPtr)->cigTypeStr);\
    (samSTPtr)->cigTypeStr = 0;\
    (samSTPtr)->lenCigBuffUI = 0;\
    \
    free((samSTPtr)->cigValAryI);\
    (samSTPtr)->cigValAryI = 0;\
    \
    free((samSTPtr)->extraStr);\
    (samSTPtr)->extraStr = 0;\
    (samSTPtr)->lenExtraBuffUI = 0;\
} /*freeStackSamEntry*/

/*-------------------------------------------------------\
| Fun-04: freeSamEntry
|  - Frees a samEntry struct
| Input:
|  - samSTPtr
|    o Pointer to Sam entry struct to free
| Output:
|  - Frees:
|    o samSTPtr and its variables with allocated memory
\-------------------------------------------------------*/
#define freeSamEntry(samSTPtr){\
    freeStackSamEntry((samSTPtr));\
    free((samSTPtr));\
    (samSTPtr) = 0;\
} /*freeSamEntry*/

/*-------------------------------------------------------\
| Fun-05: makeSamEntry
|  - Makes an heap allocated samEntry structure
| Input:
| Output:
|  - Returns:
|    o Pointer to a samEntry structure
|    o 0 if had an memory error
\-------------------------------------------------------*/
#define makeSamEntry()({\
  struct samEntry *samST=malloc(sizeof(struct samEntry));\
  uchar errUC = 0;\
  \
  if(samST)\
  { /*If: I did not have a memory error*/\
     errUC = initSamEntry(samST);\
     if(errUC) freeSamEntry(samST);\
  } /*If: I did not have a memory error*/\
  \
  samST;\
}) /*makeSamEntry*/

/*-------------------------------------------------------\
| Fun-06: samEntryQHistToMed
|   - Gets the median Q-score for an histogram of q-scores
|     in a samStruct
| Input:
|   - samSTPTr:
|     o Pointer to samEntry struct with a Q-score
|       histogram to find the median Q-score for
| Output:
|   - Modifies:
|     o samSTPtr->medianQF to have the median Q-score
\-------------------------------------------------------*/
#define samEntryQHistToMed(samSTPtr){\
    uint numBasesUI = 0;\
    uint midPointUL = (samSTPtr)->readLenUI >> 1;\
    uint uiQ = 0;\
    \
    for(uiQ = 0; uiQ < defMaxQScore; uiQ++)\
    { /*Loop: through Q-score histogram for midpoint*/\
        numBasesUI += (samSTPtr)->qHistUI[uiQ];\
        \
        if(numBasesUI >= midPointUL)\
        { /*if found the midpoint, then find the median*/\
            if(numBasesUI > midPointUL || numBasesUI & 1)\
                (samSTPtr)->medianQF = (float) uiQ;\
            \
            else\
            { /*Else: even & 2 differnt Q's at midpoint*/\
                numBasesUI = uiQ;\
                ++uiQ;\
                \
                while((samSTPtr)->qHistUI[uiQ]==0) ++uiQ;\
                \
                (samSTPtr)->medianQF =\
                    (numBasesUI + uiQ) / ((float) 2);\
            } /*Else: even & 2 differnt Q's at midpoint*/\
            \
            break;\
        } /*if found the midpoint, then find the median*/\
    } /*Loop: through Q-score histogram for midpoint*/\
} /*samEntryQHistToMed*/

/*-------------------------------------------------------\
| Fun-07: samEntryFindQScores
|   - Gets the median and mean Q-scores from a samEntry
|     Structure.
| Input:
|   - samSTPTr:
|     o Pointer to samEntry struct to find the median and
|       mean q-scores for
| Output:
|   - Modifies:
|     o samSTPtr->medianQF to have the median Q-score
|     o samSTPtr->meanQF to have the mean Q-score
|     o samSTPtr->qHistUI to have a histogram of Q-scores
|     o samSTPtr->sumQUL to have the sum of all Q-scores
\-------------------------------------------------------*/
#define samEntryFindQScores(samSTPtr){\
    ulong macQAdjustUL = ulCpMakeDelim(defQAdjust);\
    ulong macQScoresUL = 0;\
    ulong *macQPtrUL = (samSTPtr)->qStr;\
    int uiQMac = 0;\\
    uchar *macAryUC = 0;\
    int iCMac = 0;\
    \
    /*Find the number of Q-score characters in buffer*/\
    for(\
       iQMac = 0;\
       iQMac < ((samSTPtr)->readLenUI >> defShiftByUL);\
       ++uiQMac\
    ) { /*Loop: Update the q-score historgram and sum*/\
       macQScoresUL = macQPtrUL[iQMacMac] - qAdjustUL;\
       macAryUC = (uchar *) macQScoresUL;\
       \
       for(iCMac = 0; iCMac < defCharPerUL; ++iCMac)\
       { /*Loop: Get the q-score entries*/\
          (samSTPtr)->qHistUI[(uchar) macAryUC[iCMac]];\
          (samSTPtr)->sumQUL += (uchar) macAryUC[iCMac];\
       } /*Loop: Get the q-score entries*/\
    } /*Loop: Update the q-score historgram and sum*/\
    \
    iQMac = (samSTPtr)->readLenUI;\
    macAryUC = (uchar *) (samSTPtr)->qStr;\
    \
    for(\
       iQMac -= ((samSTPtr)->readLenUI & defModByUL);\
       iQMac < (samSTPtr)->readLenUI;\
       ++iQMac\
    ) { /*Loop: Copy the Q-score entries*/\
       (samSTPtr)->qStr[iQMac] = macAryUC[iQMac];\
       macQScoresUL= (uchar) macAryUC[iQMac] - defQAdjust;\
       \
       ++(samSTPtr)->qHistUI[macQScoresUL];\
       (samSTPtr)->sumQUL += macQScoresUL;\
    } /*Loop: Copy the Q-score entries*/\
    \
    (samSTPtr)->meanQF =\
       (float) (samSTPtr)->sumQUL /(samSTPtr)->readLenUI;\
    \
    samEntryQHistToMed((samSTPtr));\
} /*samEntryFindQScores*/


#if defined NO_UL_CP
   #define cpQScores(samSTPtr, cpQStr)({\
      int iQMac = 0;\
      uint qScoreMacUI = 0;\
      \
      for(iQMac=0; iQMac < (samSTPtr)->readLenUI;++iQMac)\
      { /*Loop: Copy the Q-score entries*/\
         (samSTPtr)->qStr[iQMac] = iterStr[iQMac];\
          qScoreMacUI=\
            (uchar) (cpQStr)[iQMac] - defQAdjust;\
         \
         ++(samSTPtr)->qHistUI[qScoreMacUI];\
         (samSTPtr)->sumQUL += qScoreMacUI;\
      } /*Loop: Copy the Q-score entries*/\
      \
      (samSTPtr)->qStr[iQMac] = '\0';\
      \
      /**************************************************\
      * Fun-03 Sec-13 Sub-04:
      *   - Find the mean and median Q-scores
      \*************************************************/\
      \
      (samSTPtr)->meanQF =\
         (float)(samSTPtr)->sumQUL/(samSTPtr)->readLenUI;\
      \
      samEntryQHistToMed((samSTPtr));\
      iQMac;\
}) /*cpQScores*/

#else
   #define cpQScores(samSTPtr, cpQStr)({\
      char *tmpMacStr = 0;\
      int iQMac = 0;\
      int iCharMac = 0;\
      ulong qAdjustMacUL = ulCpMakeDelim(defQAdjust);\
      ulong *cpMacPtrUL = (ulong *) (cpQStr);\
      ulong *dupMacPtrUL = (ulong *) samSTPtr->qStr;\
      ulong qScoreMacUL = 0;\
      \
      for(\
         iQMac = 0;\
         iQMac < ((samSTPtr)->readLenUI >> defShiftByUL);\
         ++iQMac\
      ) { /*Loop: Copy the Q-score entries*/\
         dupMacPtrUL[iQMac] = cpMacPtrUL[iQMac];\
         qScoreMacUL = dupMacPtrUL[iQMac] - qAdjustMacUL;\
         tmpMacStr = (char *) &qScoreMacUL;\
         \
         for(\
            iCharMac = 0;\
            iCharMac < defCharPerUL;\
            ++iCharMac\
         ) { /*Loop: Get the q-score entries*/\
            ++(samSTPtr)->qHistUI[\
               (uchar) tmpMacStr[iCharMac]\
            ];\
            \
            (samSTPtr)->sumQUL +=\
               (uchar) tmpMacStr[iCharMac];\
         } /*Loop: Get the q-score entries*/\
      } /*Loop: Copy the Q-score entries*/\
      \
      /**************************************************\
      * Fun-03 Sec-13 Sub-03:
      *   - Finish copying the end of the Q-score entry
      \*************************************************/\
      \
      iQMac = (samSTPtr)->readLenUI;\
      \
      for(\
         iQMac -= ((samSTPtr)->readLenUI & defModByUL);\
         iQMac < (samSTPtr)->readLenUI;\
         ++iQMac\
      ) { /*Loop: Copy the Q-score entries*/\
         (samSTPtr)->qStr[iQMac] = (cpQStr)[iQMac];\
         qScoreMacUL =\
            (uchar) (cpQStr)[iQMac] - defQAdjust;\
         \
         ++(samSTPtr)->qHistUI[qScoreMacUL];\
         (samSTPtr)->sumQUL += qScoreMacUL;\
      } /*Loop: Copy the Q-score entries*/\
      \
      (samSTPtr)->qStr[iQMac] = '\0';\
      \
      /**************************************************\
      * Fun-03 Sec-13 Sub-04:
      *   - Find the mean and median Q-scores
      \*************************************************/\
      \
      (samSTPtr)->meanQF =\
         (float)(samSTPtr)->sumQUL/(samSTPtr)->readLenUI;\
      \
      samEntryQHistToMed((samSTPtr));\
      iQMac;\
}) /*cpQScores*/

#endif

/*-------------------------------------------------------\
| Fun-08: readSamLine
|  - Reads in a single line from a sam file
| Input:
|  - samSTPtr:
|    o Pionter to samEntry structure to store the sam file
|      line in. This structure should be initialized
|  - buffStr:
|    o Buffer to read the sam file line temporarly into.
|      This is resized if needed. You can input NULL to
|      create a new buffer.
|  - lenBuffUL:
|    o Length of buffStr (updated if buffStr is resized)
|  - samFILE:
|    o Sam file to read a line from
| Output:
|  - Modifies:
|    o samSTPtr to have the next line
|    o samFILE to be on the next line
|    o buffStr to hold a sam file line (resized if needed)
|    o lenBuffUL to hold the resized length of buffStr
|  - Returns:
|    o 0 for success
|    o 1 for EOF (End Of File)
|    o 64 for memory errors
\-------------------------------------------------------*/
static char readSamLine(
   struct samEntry *samSTPtr,
   char **buffStr,
   ulong *lenBuffUL,
   FILE *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: readSamLine
   '   - Reads a single line from a sam file into samSTPtr
   '   o fun-08 Sec-01:
   '     - Variable declerations
   '   o fun-08 Sec-02:
   '     - Read in one line from the sam file
   '   o fun-08 Sec-03:
   '     - Get the query id from the buffer
   '   o fun-08 Sec-04:
   '     - Get the flag
   '   o fun-08 Sec-05:
   '     - REad in the reference name/id
   '   o fun-08 Sec-06:
   '     - Get the reference position
   '   o fun-08 Sec-07:
   '     - Get the mapping quality
   '   o fun-08 Sec-08:
   '     - Get the cigar entry
   '   o fun-08 Sec-09:
   '     - Get the RNEXT entry
   '   o fun-08 Sec-10:
   '     - Get the PNEXT entry
   '   o fun-08 Sec-11:
   '     - Get the TLEN entry
   '   o fun-08 Sec-12:
   '     - Get the sequence entry
   '   o fun-08 Sec-13:
   '     - Get the Q-score entry
   '   o fun-08 Sec-14:
   '     - Copy the extra entry; after strict sam entries
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort extraBuffUS = 4096;

   ulong tabUL = ulCpMakeDelim('\t');
   ulong newLineUL = ulCpMakeDelim('\n');

   ulong oldLenUL = 0;
   char *tmpStr = 0;
   char *iterStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^   - Read in one line from the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   blankSamEntry(samSTPtr);

   if(*buffStr == 0)
   { /*If: I need to create a buffer*/
      *lenBuffUL = 0;
      *buffStr = malloc(extraBuffUS * sizeof(char));

      if(*buffStr == 0) return 64;

      *lenBuffUL = extraBuffUS;
   } /*If: I need to create a buffer*/

   /*My marker to make sure I read in a full line*/
   (*buffStr)[*lenBuffUL - 3] = '\0';

   tmpStr = fgets(*buffStr, *lenBuffUL, samFILE);

   if(!tmpStr) return 1; /*End of file*/

   /*This is not the most efficent system for memory, but
   ` it makes the code less complex and easier to deal
   ` with
   */
   while((*buffStr)[*lenBuffUL - 3] != '\0')
   { /*Loop: Read in the full sam entry line*/
      oldLenUL = *lenBuffUL;
      *lenBuffUL += extraBuffUS;
      tmpStr =realloc(*buffStr,*lenBuffUL * sizeof(char));

      if(tmpStr == 0) return 64;

      *buffStr = tmpStr;
      (*buffStr)[*lenBuffUL - 3] = '\0';
      tmpStr = *buffStr + oldLenUL - 1;

      /*Make sure on the first null; the other method can
      ` mess up
      */
      while(*(tmpStr - 1) == '\0') --tmpStr;

        /*-1 to account for extraBuffUS being index 1*/

      tmpStr = fgets(tmpStr, extraBuffUS, samFILE);
      if(!tmpStr) break; /*End of file*/

   } /*Loop: Read in the full sam entry line*/

   iterStr = *buffStr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Get the query id from the buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*This is a comment*/
   if(iterStr[0] == '@') goto extraEntry;
 
   /*Query id, single byte copy may be better here,
    ` hard to tell
   */
   samSTPtr->lenQryIdUC = (uchar)
      cCpStrDelim(
         samSTPtr->qryIdStr,
         iterStr,
         '\t'
      ); /*Copy the query id/name*/

   iterStr += samSTPtr->lenQryIdUC + 1; /*+1 get off tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-04:
   ^   - Get the flag for the alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iterStr =
      base10StrToUS(
         iterStr,
         samSTPtr->flagUS
      ); /*Get the flag for the alignment*/

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-05:
   ^   - REad in the reference name/id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Not clear wich version is faster here*/
   samSTPtr->lenRefIdUC = (uchar)
      ulCpStrDelim(
         samSTPtr->refIdStr,
         iterStr,
         defTabDelim,
         '\t'
      ); /*Copy the reference id/name*/
   
   iterStr += samSTPtr->lenRefIdUC + 1; /*+1 get off tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-06:
   ^   - Get the reference position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iterStr =
      base10StrToUI(
         iterStr,
         samSTPtr->refStartUI
      ); /*Get the starting base in the reference*/

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-07:
   ^   - Get the mapping quality
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iterStr =
      base10StrToUC(
         iterStr,
         samSTPtr->mapqUC
      ); /*Get the mapping quality of the alignment*/

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-08:
   ^   - Get the cigar entry
   ^   o fun-08 sec-08 sub-01:
   ^     - Check if there is a cigar entry
   ^   o fun-08 sec-08 sub-02:
   ^     - Read in the cigar entry
   ^   o fun-08 sec-08 sub-03:
   ^     - Count number of matchs/snps/dels/inss/masks in
   ^       the cigar entry
   ^   o fun-08 sec-08 sub-04:
   ^     - Get the read lengths from the cigar entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-08 Sub-01:
   *   - Check if there is a cigar entry
   \*****************************************************/

   if(iterStr[0] == '*')
   { /*If: the cigar entry was not present*/
      samSTPtr->cigTypeStr[0] = '*';
      samSTPtr->cigValAryI[0] = 0;
      iterStr += 2;
      (samSTPtr)->lenCigUI = 1;

      goto rNextEntry;
   } /*If: the cigar entry was not present*/

   /*****************************************************\
   * Fun-08 Sec-08 Sub-02:
   *   - Read in the cigar entry
   \*****************************************************/
   
   while(iterStr[0] > 32)
   { /*Loop: Read in the cigar entry*/

      /*Using -1 to account for the null I will add at the
      `end
      */
      if((samSTPtr)->lenCigUI >= samSTPtr->lenCigBuffUI-1)
      { /*If: I need to increase the cigar buff size*/
         samSTPtr->lenCigBuffUI <<= 1;

         tmpStr =
            realloc(
               samSTPtr->cigTypeStr,
               samSTPtr->lenCigBuffUI * sizeof(char)
            ); /*Resize the type cigar buffer*/

          if(tmpStr == 0) return 64;
          samSTPtr->cigTypeStr = tmpStr;

         tmpStr = (char *)
            realloc(
               samSTPtr->cigValAryI,
               samSTPtr->lenCigBuffUI * sizeof(int)
            ); /*Resize the value cigar buffer*/

          if(tmpStr == 0) return 64;
          samSTPtr->cigValAryI = (int *) tmpStr;
      } /*If: I need to increase the cigar buff size*/

      /*Record the cigar entry*/
      iterStr =
         base10StrToSI(
            iterStr,
            samSTPtr->cigValAryI[samSTPtr->lenCigUI]
          ); /*Get the number of bases for this type*/

      samSTPtr->cigTypeStr[samSTPtr->lenCigUI]=iterStr[0];

      /**************************************************\
      * Fun-08 Sec-08 Sub-03:
      *   - Count number of matchs/snps/dels/inss/masks in
      *     the cigar entry
      \**************************************************/

      switch(iterStr[0])
      { /*Switch: Check the cigar entry type*/
         case '=':
         /*Case: This was a match*/
             samSTPtr->numMatchUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This was a match*/

         case 'M':
         /*Case: This is an snp or match*/
             samSTPtr->numMatchUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This is an snp or match*/

         case 'X':
         /*Case: This is an snp*/
             samSTPtr->numSnpUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This is an snp*/

         case 'I':
         /*Case: This is an insertion*/
             samSTPtr->numInsUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This is an insertion*/

         case 'D':
         /*Case: This is an deletion*/
             samSTPtr->numDelUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This is an deletion*/

         case 'S':
         /*Case: This is an softmasked region*/
             samSTPtr->numMaskUI +=
                samSTPtr->cigValAryI[samSTPtr->lenCigUI];

             break;
         /*Case: This is an softmasked region*/
      } /*Switch: Check the cigar entry type*/

      ++iterStr; /*Get off the tab*/
      ++(samSTPtr)->lenCigUI;
   } /*Loop: Read in the cigar entry*/

   /*****************************************************\
   * Fun-08 Sec-08 Sub-04:
   *   - Get the read lengths from the cigar entries
   \*****************************************************/

    samSTPtr->cigValAryI[samSTPtr->lenCigUI] = 0;
    samSTPtr->cigTypeStr[samSTPtr->lenCigUI] = '\0';

   samSTPtr->readLenUI =
        samSTPtr->numMatchUI
      + samSTPtr->numSnpUI
      + samSTPtr->numInsUI
      + samSTPtr->numMaskUI;

   samSTPtr->alnReadLenUI =
        samSTPtr->numMatchUI
      + samSTPtr->numSnpUI
      + samSTPtr->numDelUI;

   samSTPtr->refEndUI = samSTPtr->refStartUI;
   samSTPtr->refEndUI += samSTPtr->alnReadLenUI;

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-09:
   ^   - Get the RNEXT entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rNextEntry:

   /*Not sure which is better here*/
   samSTPtr->lenRNextUC = (uchar)
      ulCpStrDelim(
         samSTPtr->rNextStr,
         iterStr,
         defTabDelim,
         '\t'
      ); /*Copy the query id/name*/

   iterStr += samSTPtr->lenRNextUC + 1; /*+1 get off tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-10:
   ^   - Get the PNEXT entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iterStr =
      base10StrToSI(
         iterStr,
         samSTPtr->pNextI
       ); /*Get the number of bases for this type*/

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-11:
   ^   - Get the TLEN entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iterStr =
      base10StrToSI(
         iterStr,
         samSTPtr->tLenI
       ); /*Get the number of bases for this type*/

   ++iterStr; /*Get off the tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-12:
   ^   - Get the sequence entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->readLenUI == 0 && iterStr[0] != '*')
      samSTPtr->readLenUI= ulLenStr(iterStr, tabUL, '\t');

   else if(iterStr[0] == '*')
   { /*Else If: There  is no sequence entry*/
      samSTPtr->seqStr[0] = '*';
      samSTPtr->seqStr[1] = '\0';
      iterStr += 2;

      goto noQEntry;
   } /*Else If: There  is no sequence entry*/

   if(samSTPtr->readLenUI > samSTPtr->lenSeqBuffUI)
   { /*If: I need to resize sequence & Q-score buffers*/
      free(samSTPtr->seqStr);
      samSTPtr->seqStr = 0;

      samSTPtr->seqStr =
         malloc((samSTPtr->readLenUI + 1) * sizeof(char));

      if(!samSTPtr->seqStr) return 64;

      free(samSTPtr->qStr);
      samSTPtr->qStr = 0;

      samSTPtr->qStr =
         malloc((samSTPtr->readLenUI + 1) * sizeof(char));

      if(!samSTPtr->qStr) return 64;

      samSTPtr->lenSeqBuffUI = samSTPtr->readLenUI;
      samSTPtr->lenQBuffUI = samSTPtr->readLenUI;
   } /*If: I need to resize sequence & Q-score buffers*/

   ulCpStr(samSTPtr->seqStr,iterStr,samSTPtr->readLenUI);
   iterStr += samSTPtr->readLenUI + 1; /*+1 gets off tab*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-13:
   ^   - Get the Q-score entry
   ^   o fun-03 sec-13 sub-01:
   ^     - Check if I have a Q-score entry
   ^   o fun-03 sec-13 sub-02:
   ^     - Copy the q-score entry in blocks of 8
   ^   o fun-03 sec-13 sub-03:
   ^     - Finish copying the end of the Q-score entry
   ^   o fun-03 sec-13 sub-04:
   ^     - Find the mean and median Q-scores
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-03 Sec-13 Sub-01:
   *   - Check if I have a Q-score entry
   \*****************************************************/

   if(iterStr[0] == '*' && iterStr[1] == '\t')
   { /*If: there is no Q-score entry*/
      noQEntry:

      samSTPtr->qStr[0] = '*';
      samSTPtr->qStr[1] = '\0';
      iterStr += 2;
   } /*If: there is no Q-score entry*/

   else iterStr += cpQScores(samSTPtr, iterStr) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-14:
   ^   - Copy the extra entry; after strict sam entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   extraEntry:

   /*not sure if char or ul copy better here*/
   samSTPtr->lenExtraUI= ulLenStr(iterStr,newLineUL,'\n');

   if(samSTPtr->lenExtraUI > samSTPtr->lenExtraBuffUI)
   { /*If: I need to resize the buffer*/
      free(samSTPtr->extraStr);
      samSTPtr->extraStr = 0;

      samSTPtr->extraStr =
         malloc((samSTPtr->lenExtraUI +1)* sizeof(char));
      
      if(samSTPtr->extraStr == 0) return 64;
   } /*If: I need to resize the buffer*/

   ulCpStr(
      samSTPtr->extraStr,
      iterStr,
      samSTPtr->lenExtraUI
   ); /*Copy the extra entry*/

   return 0;
} /*readSamLine*/

/*-------------------------------------------------------\
| Fun-09: pSamEntry
| Use:
|  - Prints the sam file entry to a file. This does not
|    print any extra stats that were found.
| Input:
|  - samST
|    o Pointer to samEntry struct with sam entry to print
|  - buffStr:
|    o Pointer to c-string buffer to temporarly hold the
|      cigar entry (speeds things up)
|  - lenBuffUL:
|    o Current length of buffer, adjusted if buffStr is
|      expanded
|  - outFILE:
|    o File to print the sam entry to
| Output:
|  - Prints:
|    o Sam file entry in samST to outFILE.
|  - Modifies:
|    o inceases the size of buffStr, if is not 8x the
|      cigar length
|    o Sets lenBuffUL to the new buffStr size when buffStr
|      is resized
|  - Returns:
|    o 0 for no problems
|    o 64 for memory errors
\-------------------------------------------------------*/
#define pSamEntry(samSTPtr,buffStr,lenBuffUL,outFILE)({\
   uint uiCig = 0;\
   char *tmpStr = buffStr;\
   char errC = 0;\
   \
   if((lenBuffUL) < (samSTPtr)->lenCigUI << 3)\
   { /*If: I need to add more buffer*/\
      free(buffStr);\
      (lenBuffUL) = (samSTPtr)->lenCigUI << 3;\
      buffStr = malloc((lenBuffUL) * sizeof(char));\
   } /*If: I need to add more buffer*/\
   \
   if(!buffStr) errC = 64;/*If I had a memory error*/\
   \
   else if(\
         (samSTPtr)->extraStr[0] == '@'\
      && (samSTPtr)->qryIdStr[0] == '\0'\
   ) fprintf((outFILE), "%s\n", (samSTPtr)->extraStr);\
   \
   else\
   { /*Else: This is an entry, not a comment*/\
      \
      fprintf(\
         (outFILE),\
         "%s\t%u\t%s\t%u\t%u\t",\
         (samSTPtr)->qryIdStr,\
         (samSTPtr)->flagUS,\
         (samSTPtr)->refIdStr,\
         (samSTPtr)->refStartUI,\
         (samSTPtr)->mapqUC\
      );\
      \
      if((samSTPtr)->cigTypeStr[0] != '*')\
      { /*If: I need to print out the cigar entry*/\
         for(\
            uiCig=0;\
            uiCig < (samSTPtr)->lenCigUI;\
            ++uiCig\
         ){\
            tmpStr +=\
               numToStr(\
                  tmpStr,\
                  (samSTPtr)->cigValAryI[uiCig]\
               );\
            tmpStr[0] = (samSTPtr)->cigTypeStr[uiCig];\
            ++tmpStr;\
          }\
          tmpStr[0] = '\0';\
          fprintf((outFILE), buffStr);\
      } /*If: I need to print out the cigar entry*/\
      \
      else fprintf((outFILE), "*");\
      \
      fprintf(\
         (outFILE),\
         "\t%s\t%u\t%u\t%s\t%s\t%s\n",\
         (samSTPtr)->rNextStr,\
         (samSTPtr)->pNextI,\
         (samSTPtr)->tLenI,\
         (samSTPtr)->seqStr,\
         (samSTPtr)->qStr,\
         (samSTPtr)->extraStr\
      ); /*Print out the final entries*/\
   } /*Else: This is an entry, not a comment*/\
   \
   errC;\
}) /*printSamEntry*/

/*-------------------------------------------------------\
| Fun-10: pSamEntryAsFastq
| Use:
|  - Prints the sam entry as a fastq entry to a fastq file
| Input:
|  - samST:
|    o Pointer to samEntry structure with fastq entry to
|      print out
|  - outFILE:
|    o Fastq file to print the new fastq entry to
| Output:
|  - Prints:
|    o fastq entry from samST to outFILE
\-------------------------------------------------------*/
#define pSamEntryAsFastq(samSTPtr, outFILE){\
   if(   (samSTPtr)->seqStr[1] != '\0'\
      && (samSTPtr)->qStr[1] != '\0'\
   ){ /*If: This entry can be printed out*/\
      fprintf(\
        (outFILE),\
        "@%s ref=%s start=%u len=%u refAlnLen=%u",\
        (samSTPtr)->qryIdStr,\
        (samSTPtr)->refIdStr,\
        (samSTPtr)->refStartUI,\
        (samSTPtr)->readLenUI,\
        (samSTPtr)->alnReadLenUI\
      );\
      \
      fprintf(\
        (outFILE),\
        "  flag=%u mapq=%u match=%u snp=%u ins=%u",\
        (samSTPtr)->flagUS,\
        (samSTPtr)->mapqUC,\
        (samSTPtr)->numMatchUI,\
        (samSTPtr)->numSnpUI,\
        (samSTPtr)->numInsUI\
      );\
      \
      fprintf(\
        (outFILE),\
        "  del=%u softMasked=%u meanQ=%f medianQ=%f\n",\
        (samSTPtr)->numDelUI,\
        (samSTPtr)->numMaskUI,\
        (samSTPtr)->meanQF,\
        (samSTPtr)->medianQF\
      );\
      fprintf(\
        (outFILE),\
        "%s\n+\n%s\n",\
        (samSTPtr)->seqStr,\
        (samSTPtr)->qStr\
      );\
   } /*If: This entry can be printed out*/\
} /*pSamEntryAsFq*/


/*-------------------------------------------------------\
| Fun-11: pSamEntryAsFasta
| Use:
|  - Prints the sam entry as a fasta entry to a fasta file
| Input:
|  - samST:
|    o Pointer to samEntry structure with fastq entry to
|      print out
|  - outFILE:
|    o Fasta file to print the new fasta entry to
| Output:
|  - Prints:
|    o fasta entry from samST to outFILE
\-------------------------------------------------------*/
#define pSamEntryAsFasta(samSTPtr, outFILE){\
   if((samSTPtr)->seqStr[1] != '\0')\
   { /*If: This entry can be printed out*/\
      fprintf(\
        (outFILE),\
        ">%s ref=%s start=%u len=%u refAlnLen=%u",\
        (samSTPtr)->qryIdStr,\
        (samSTPtr)->refIdStr,\
        (samSTPtr)->refStartUI,\
        (samSTPtr)->readLenUI,\
        (samSTPtr)->alnReadLenUI\
      );\
      \
      fprintf(\
        (outFILE),\
        "  flag=%u mapq=%u match=%u snp=%u ins=%u",\
        (samSTPtr)->flagUS,\
        (samSTPtr)->mapqUC,\
        (samSTPtr)->numMatchUI,\
        (samSTPtr)->numSnpUI,\
        (samSTPtr)->numInsUI\
      );\
      \
      fprintf(\
        (outFILE),\
        "  del=%u softMasked=%u meanQ=%f medianQ=%f\n",\
        (samSTPtr)->numDelUI,\
        (samSTPtr)->numMaskUI,\
        (samSTPtr)->meanQF,\
        (samSTPtr)->medianQF\
      );\
      fprintf((outFILE), "%s\n", (samSTPtr)->seqStr);\
   } /*If: This entry can be printed out*/\
} /*pSamEntryAsFq*/

/*-------------------------------------------------------\
| Fun-11: pSamEntryStats
| Use:
|  - Prints out the stats in a samEntry struct to a file
| Input:
|  - samEntryST:
|    o Pointer to samEntry struct to print stats for
|  - pHeadBl:
|    o 1: Print the header for the stats tsv file
|    o 0: Do not print the header
|  - outFILE:
|    o TSV (tab deliminated) file to print stats to
| Output:
|  - Prints:
|    o line with stats from samEntryStruct to file (tsv)
|  - Modifies:
|    o printHeaderChar to be 0 if set to 1
\-------------------------------------------------------*/
#define pSamEntryStats(samSTPtr, pHeadBl, outFILE){\
   if(   (samSTPtr)->extraStr[0] != '@'\
      && (samSTPtr)->qryIdStr[0] != '\0'\
   ){ /*If: This is not a comment*/\
      if((pHeadBl))\
      { /*If: I need to print the header*/\
        fprintf((outFILE), "Read\tRef\tflag\tMAPQ");\
        fprintf((outFILE), "\trefPos\treadLength"); \
        fprintf((outFILE), "\trefAlnLength\tmatches");\
        fprintf((outFILE), "\tInss\tDels\tSNPs");\
        fprintf((outFILE), "\tmedianQ\tmeanQ\n");\
        \
        (pHeadBl) = 0;\
      } /*If: I need to print the header*/\
      \
      fprintf(\
        (outFILE),\
        "%s\t%s\t%u\t%u\t%u\t%u",\
        (samSTPtr)->qryIdStr,\
        (samSTPtr)->refIdStr,\
        (samSTPtr)->flagUS,\
        (samSTPtr)->mapqUC,\
        (samSTPtr)->refStartUI,\
        (samSTPtr)->readLenUI\
      );\
      \
      fprintf(\
        (outFILE),\
        "\t%u\t%u\t%u\t%u\t%u\t%f\t%f\n",\
        (samSTPtr)->alnReadLenUI,\
        (samSTPtr)->numMatchUI,\
        (samSTPtr)->numSnpUI,\
        (samSTPtr)->numInsUI,\
        (samSTPtr)->numDelUI,\
        (samSTPtr)->meanQF,\
        (samSTPtr)->medianQF\
      );\
   } /*If: This is not a comment*/\
} /*pSamEntryStats*/

#endif

/*
Sam file table for first 11 columns (all sam files have)
| Col | Field |  Type  |        Brief description              |
|:---:|:-----:|:------:|:-------------------------------------:|
|  1  | QNAME | String |       Query template NAME             |
|  2  | FLAG  |  Int   |          bitwise FLAG                 |
|  3  | RNAME | String |     Reference sequence NAME           |
|  4  |  POS  |  Int   |  1-based leftmost mapping POSition    |
|  5  | MAPQ  |  Int   |          MAPping Quality              |
|  6  | CIGAR | String |            CIGAR string               |
|  7  | RNEXT | String | Reference name of the mate/next read  |
|  8  | PNEXT |  Int   |   Position of the mate/next read      |
|  9  | TLEN  |  Int   |      observed Template LENgth         |
| 10  |  SEQ  | String |          segment SEQuence             |
| 11  | QUAL  | String | ASCII of Phred-scaled base Quality+33 |
*/

/*eqx cigar entry
`   matches are '='
`   mimsatches are 'X'
'   non-eqx merges matches and mismatches into 'M'
`   deletions are 'D'
`   insertions are 'I'
`   soft masks are 'S'
`   Numbers come before entry (=, X, D, I, or S) & show
`     number of times the entry is repeated
`   Everything else is a hard mask (was removed) or
'      removed the bases from the sequence
`   EX: 10S5=1X701 (10 soft masked bases, 5 matches,
`       1 SNP, 701 matches)
*/

/* Sam file flag values tables
| Bit  | FLAG  |                        Description                                 |
|:----:|:-----:|:------------------------------------------------------------------:|
| 1    | 0x1   | template having multiple segments in sequencing                    |
| 2    | 0x2   | each segment properly aligned according to the aligner             |
| 4    | 0x4   | segment unmapped                                                   |
| 8    | 0x8   | next segment in the template unmapped                              |
| 16   | 0x10  | SEQ being reverse complemented                                     |
| 32   | 0x20  | SEQ of the next segment in the template being reverse complemented |
| 64   | 0x40  | the first segment in the template                                  |
| 128  | 0x80  | the last segment in the template                                   |
| 256  | 0x100 | secondary alignment                                                |
| 512  | 0x200 | not passing filters, such as platform/vendor quality controls      |
| 1024 | 0x400 | PCR or optical duplicate                                           |
| 2048 | 0x800 | supplementary alignment                                            |
*/
