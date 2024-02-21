/*########################################################
# Name: samEntryStruct
#  - Holds structer to hold a sam file entry. This also
#    includes the functions needed to support this
#    structer.
# Libraries
#  - "base10StrToNum.h"
#  o "dataTypeShortHand.h"
# C Standard Libraries:
#  - <string.h>
#  - <stdlib.h>
#  - <stdio.h>
# Note:
#  - End of file has some general sam file basics
########################################################*/

#ifndef SAMENTRYSTRUCT_H
#define SAMENTRYSTRUCT_H

#include <stdlib.h>          /*memory allocation*/
#include <string.h>
#include <stdio.h>

#include "base10StrToNum.h"

#define Q_ADJUST 33 /*offest to get q-score of 0*/
#define MAX_Q_SCORE 94 /*highest possible Q-score*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' samEntryStruct TOC: Table Of Contents
'  o struct-01 samEntry:
'    - Holds a single samfile entry
'  o struct-02 readStat:
'    - Holds query id, reference id, & stats for a read
'  o fun-01 blankSamEntry:
'    - Sets all non-alloacted variables in samEntryST to 0
'  o fun-02 initSamEntry:
'    - Initalize a samEntry struct to 0's
'  o fun-03 blankReadStats:
'    - modifes samEntryST to have every non-pointer
'      variable set to 0
'  o fun-04 deepCpSamReadStats:
'    - Copies the contents and buffer of one samEntry
'      struct to another samEntry struct.
'  o fun-05 cpSamEntry:
'    - Copies address of q-score & sequence pointers from
'      an old samEntry to an new SamEntry
'  o fun-06 freeStackSamEntry:
'    - Frees heap allocations in a stack allocated
'      samEntry struct
'  o fun-07 freeHeapSamEntry:
'    - Frees a samEntry structer (and sets to null)
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
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Struct-01: samEntry
|  - Holds a single samfile entry
\-------------------------------------------------------*/
typedef struct samEntry
{ /*samEntry*/
    char *samEntryCStr;/*Holds c-string (entire sam entry)*/

    char *queryCStr;/*points to query id in samEntryCStr*/
    char *refCStr;  /*points to reference in samEntryCStr*/
    char *cigarCStr;/*points to cigar in samEntryCStr*/
    char *seqCStr;  /*points to seqeuence in samEntryCStr*/
    char *qCStr;    /*points to q-score in samEntryCStr*/

    uchar mapqUChar; /*Holds mapping quality*/

    ushort flagUSht; /*Holds the flag in samEntryCStr*/

    float medianQFlt;     /*holds median read Q-score*/
    float meanQFlt;       /*holds mean read Q-score*/
 
    /*The aligned Q-scores ignore low quality bases*/
    float medianAligQFlt; /*holds aligned median Q-score*/
    float meanAligQFlt;   /*holds mean aligned Q-score*/

    uint posOnRefUInt;  /*First reference base*/
    uint unTrimReadLenUInt;/*length of untrimmed read*/
    uint readLenUInt;     /*Holds read length*/
    uint readAligLenUInt; /*Aligned read length*/

    uint numMatchUInt;    /*Holds number of matches*/
    uint numSnpUInt;     /*number of insertions*/
    uint numInsUInt;     /*number of insertions*/
    uint numDelUInt;     /*number of deletions*/

    uint seqQHistUInt[MAX_Q_SCORE]; 
       /*Histogram of base Q-scores*/
    uint seqQAlnHistUInt[MAX_Q_SCORE];
        /*Histogram of kept base Q-score (Qscore > minQ)*/

    ulong totalQScoreULng;
       /*Q-score of all bases added together*/
    ulong totalAlnQScoreULng;
       /*Q-score of kept bases added together*/

    unsigned long lenBuffULng;
       /*# bytes allocated to samEntryCStr*/
}samEntry;

/*-------------------------------------------------------\
| Fun-01: blankSamEntry
| Use:
|  - Sets all non-alloacted variables in samEntryST to 0
| Input:
|  - samEntryST:
|    - Pointer to samEntry structure to blank
| Output:
|  - Modifies:
|    o Sets every variable but samEntryCStr to 0
|    o the first character in samEntryCStr to be '\0'
\-------------------------------------------------------*/
#define blankSamEntry(samSTPtr){\
    ushort usQMac = 0;\
    \
    (samSTPtr)->queryCStr = 0;\
    (samSTPtr)->refCStr = 0;\
    (samSTPtr)->cigarCStr = 0;\
    (samSTPtr)->seqCStr = 0;\
    (samSTPtr)->qCStr = 0; \
    \
    /*Make sure the c-strings are blanked*/\
    if((samSTPtr)->samEntryCStr != 0)\
        *(samSTPtr)->samEntryCStr = '\0';\
    \
    (samSTPtr)->flagUSht = 0;\
    (samSTPtr)->mapqUChar = 0;\
    \
    (samSTPtr)->medianQFlt = 0;\
    (samSTPtr)->medianAligQFlt = 0;\
    (samSTPtr)->meanQFlt = 0;\
    (samSTPtr)->meanAligQFlt = 0;\
    \
    (samSTPtr)->posOnRefUInt = 0;\
    \
    (samSTPtr)->unTrimReadLenUInt = 0;\
    (samSTPtr)->readLenUInt = 0;\
    (samSTPtr)->readAligLenUInt = 0;\
    \
    (samSTPtr)->numMatchUInt = 0;\
    (samSTPtr)->numSnpUInt = 0;\
    (samSTPtr)->numInsUInt = 0;\
    (samSTPtr)->numDelUInt = 0;\
    \
    (samSTPtr)->totalQScoreULng = 0;\
    (samSTPtr)->totalAlnQScoreULng = 0;\
    \
    /*Reset the q-score histogram*/\
    for(usQMac = 0; usQMac < MAX_Q_SCORE; ++usQMac)\
    { /*Set all values in histograms to 0*/\
        (samSTPtr)->seqQHistUInt[usQMac] = 0;\
        (samSTPtr)->seqQAlnHistUInt[usQMac] = 0;\
    } /*Set all values in histograms to 0*/\
} /*blankSamEntry*/

/*-------------------------------------------------------\
| Fun-03: initSamEntry
| Use:
|  - Sets all variables in a samEntry struct to 0 or NULL
|  - This should only be used after malloc or decleration.
| Input:
|  - samEntryST:
|    o pointer to samEntry struct to initialize
| Output:
|  - Modifies:
|    - samEntry to have all variables set to 0
| Warning:
|  - This sets samEntryCStr to 0 without freeing.
\-------------------------------------------------------*/
#define initSamEntry(samSTPtr){\
    (samSTPtr)->samEntryCStr = 0;\
    (samSTPtr)->lenBuffULng = 0;\
    blankSamEntry((samSTPtr));\
} /*initSamEntry*/

/*-------------------------------------------------------\
| Fun-06: freeStackSamEntry
| Use:
|  - Frees all variables in samEntry, but not samEntry
| Input:
|  - samEntryST
|    o Pointer to samEntry struct to free the interal
|      memory of
| Output:
|  - Frees:
|    o allocated memory in samEntryST
\-------------------------------------------------------*/
#define freeStackSamEntry(samSTPtr){\
    if((samSTPtr)->samEntryCStr != 0)\
    { /*If I have a sam enty to free*/\
      if((samSTPtr)->queryCStr==(samSTPtr)->samEntryCStr)\
      { /*If: I need to set values to 0*/\
          (samSTPtr)->queryCStr = 0;\
          (samSTPtr)->refCStr = 0;\
          (samSTPtr)->cigarCStr = 0;\
          (samSTPtr)->seqCStr = 0;\
          (samSTPtr)->qCStr = 0;\
      } /*If: I need to set values to 0*/\
        \
        free((samSTPtr)->samEntryCStr);\
        (samSTPtr)->samEntryCStr = 0;\
    } /*If I have a sam enty to free*/\
} /*freeStackSamEntry*/

/*-------------------------------------------------------\
| Fun-07: freeHeapSamEntry
| Use:
|  - Frees all a samEntry struct
| Input:
|  - samEntryST
|    o Pointer to Sam entry struct to free
| Output:
|  - Frees:
|    o samEntryST and its variables with allocated memory
\-------------------------------------------------------*/
#define freeHeapSamEntry(samSTPtr){\
    freeStackSamEntry(samSTPtr);\
    free(samSTPtr);\
    samSTPtr = 0;\
} /*freeStackSamEntry*/

/*-------------------------------------------------------\
| Name: processSamEntry (Fun-08:)
| Use:
|  - Extracts the data from the sam entry from the buffer
|    stored in samEntryST
| Input:
|  - samEntryST:
|    o Hs the sam entry with buffer to extract data from
| Output:
|  - Modifies:
|    o read id, reference id, q-score, cigar, & sequence
|      pointers to piont to their entires in the sam entry.
|      This also grabs the flag & mapq.
\-------------------------------------------------------*/
static void processSamEntry(
    struct samEntry *samEntryST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: processSamEntry
   '  - Sets Q-score, cigar, & sequence pointers and 
   '    stats in a sam entry sotred in a samEntry struct.
   '  o fun-08 sec-01:
   '    - Variable declerations
   '  o fun-08 sec-02:
   '    - Check if their is a sam entry
   '  o fun-08 sec-03:
   '    - Extract data from the sam entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-08 Sec-01:
    ^  - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *samIterUC = samEntryST->samEntryCStr;
    unsigned int cigEntryUI = 0;
    uchar uCharCnt = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-08 Sec-02:
    ^  - Check if their is a sam entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(samIterUC == 0 || *samIterUC == '@')
    { /*If their is nothing to do or is header entry*/
        samEntryST->queryCStr = 0;
        samEntryST->refCStr = 0;
        samEntryST->cigarCStr = 0;
        samEntryST->seqCStr = 0;
        samEntryST->mapqUChar = 0;
        samEntryST->qCStr = 0;
        samEntryST->flagUSht = 0;

        return;
    } /*If their is nothing to do or is header entry*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-08 Sec-03:
    ^  - Extract data from the sam entry
    ^  o fun-08 sec-03 sub-01:
    ^    - Find non-sequence & q-score entries
    ^  o fun-08 sec-03 sub-02:
    ^    - Find sequence length from the cigar entry
    ^  o fun-08 sec-03 sub-03:
    ^    - Find sequence & q-score entries
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*****************************************************\
    * Fun-08 Sec-03 Sub-01:
    *  - Find non-sequence & q-score entries
    \*****************************************************/

    /*Store query id address*/
    samEntryST->queryCStr = samIterUC;

    /*Get the flag*/
    while(*samIterUC > 32) samIterUC++;
    samIterUC++;             /*Move of tab after query id*/
    samIterUC =
       base10StrToU16(samIterUC, samEntryST->flagUSht);

    /*Find the reference id*/
    samIterUC++;           /*Move off tab after flag*/
    samEntryST->refCStr = samIterUC; 

    while(*samIterUC > 32) samIterUC++;
    samIterUC++;   /*Move of tab after reference id*/

    /*Find the starting position on the reference*/
    samIterUC =
       base10StrToU32(samIterUC, samEntryST->posOnRefUInt);

    samIterUC++;   /*Move of tab after position*/

    /*Find the mapq entry*/
    samIterUC =
       base10StrToU8(samIterUC, samEntryST->mapqUChar);

    /*Find the cigar entry*/
    samIterUC++;    /*Move of tab after mapq entry*/
    samEntryST->cigarCStr = samIterUC;

    /*****************************************************\
    * Fun-08 Sec-03 Sub-02:
    *  - Find sequence length from the cigar entry
    \*****************************************************/

     samEntryST->unTrimReadLenUInt = 0;

     while(*samIterUC > 32)
     { /*Loop: Get the stats from the cigar entry*/
        samIterUC = base10StrToU32(samIterUC, cigEntryUI);

        switch(*samIterUC)
        { /*Switch: Count number of cigar entries*/
           case '=':
           case 'M':
           case '\t':
              samEntryST->unTrimReadLenUInt += cigEntryUI;
              samEntryST->numMatchUInt += cigEntryUI;
              break;
           case 'X':
              samEntryST->unTrimReadLenUInt += cigEntryUI;
              samEntryST->numSnpUInt += cigEntryUI;
              break;
           case 'I':
              samEntryST->unTrimReadLenUInt += cigEntryUI;
              samEntryST->numInsUInt += cigEntryUI;
              break;
           case 'D':
              samEntryST->numDelUInt += cigEntryUI;
              break;
           case 'S':
              samEntryST->unTrimReadLenUInt += cigEntryUI;
              break;
           default: break;
        } /*Switch: Count number of cigar entries*/

        cigEntryUI = 0;
        ++samIterUC; /*Get off entry character type*/
     } /*Loop: Get the stats from the cigar entry*/

     samEntryST->readLenUInt=samEntryST->unTrimReadLenUInt;
     ++samIterUC; /*Get off the tab*/

    /*****************************************************\
    * Fun-08 Sec-03 Sub-02:
    *  - Find sequence & q-score entries
    \*****************************************************/


    /*Find the sequence entry*/
    for(uCharCnt = 0; uCharCnt < 3; ++uCharCnt)
    { /*Loop past RNEXT, PNEXT, & TLEN*/
        while(*samIterUC > 32) samIterUC++;

        samIterUC++;      /*Move of tab after last entry*/
    } /*Loop past RNEXT, PNEXT, & TLEN*/

    /*Recored sequence address*/
    samEntryST->seqCStr = samIterUC;

    /*Check if have a sequence entry*/
    if(*samIterUC != '*')
       samIterUC += samEntryST->readLenUInt + 1;
    else samIterUC += 2;

    samEntryST->qCStr = samIterUC; /*Set q-score pionter*/

    return;
} /*processSamEntry*/

/*--------------------------------------------------------\
| Name: readNextPartOfLine (Fun-09:)
| Use:
|  - Reads in the next part of the line.
|  - Marks the samStruct->lenBuffULng - 2 position to be
|    '\0\' or '\n' if a complete line was read in.
| Input:
|  - inFILE:
|    o File to read from
|  - samStruct:
|    o samEntry struct to copy buffer to
|  - buffChangeULng:
|    o Amount to increase buffer by when full
| Output:
|  - Returns:
|    o 0 for eof or memory error (no data read in)
|    o pointer to start of buffer with characters read in
|  - Sets:
|    o the second to last entry (samStruct->lenBuffULng-2)
|      in samStruct->samEntryCStr to '\0'.
\--------------------------------------------------------*/
static char * readNextPartOfLine(
    FILE *inFILE,
    struct samEntry *samStruct,
    ulong buffChangeULng 
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-09 TOC: readNextPartOfLine
   '  - Reads in next part of line when fgets did not
   '    get a full line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    char *tmpUCStr = samStruct->samEntryCStr;

    tmpUCStr =
        realloc(
            samStruct->samEntryCStr, 
              sizeof(uchar)
            * (samStruct->lenBuffULng + buffChangeULng)
        ); /*Rellocate memory*/

    if(tmpUCStr == 0)
    { /*If: I had a memory error*/
       freeStackSamEntry(samStruct);
       return 0;
    } /*If: I had a memory error*/

    samStruct->samEntryCStr = tmpUCStr;

    tmpUCStr =
       samStruct->samEntryCStr + samStruct->lenBuffULng -1;

    samStruct->lenBuffULng += buffChangeULng;

    while(*tmpUCStr == '\0') /*Make sure on first null*/
    { /*Loop: till not on null*/
        ++buffChangeULng;
        --tmpUCStr;
    } /*Loop: till not on null*/

    ++tmpUCStr;
      /*Not adjusting buff change because fgets reads in
      ` one off
      */

    if(samStruct->samEntryCStr == 0)
        return 0;

    /*Set my marker for non-entire line read in*/
    *(  samStruct->samEntryCStr
      + samStruct->lenBuffULng
      - 2
    ) = '\0';

    return fgets(tmpUCStr, buffChangeULng, inFILE);
} /*readNextPartOfLine*/

/*--------------------------------------------------------\
| Name findSamCig (Fun-10:)
| Use:
|  - Finds the cigar entry in a sam entry and used the
|    cigar to find the sequence length
| Input:
|  - inFILE:
|    o File that has the remaing part of the sam file entry
|  - samStruct:
|    o samEntry struct with buffer to hold cigar and its
|      related stats
| Output:
|  - Modifies:
|    o samStruct->samEntryCStr, samStruct->lenBuffULng, &
|      samStruct->unTrimedLenULng
|    o the cigar pointer in samStruct to point to the cigar
|  - Returns:
|    o 1 if succeded
|    o 4 if end of file or memory allocation error
| Note:
|  - This requires that samStruct->samEntryCStr has read
|    in at least part of the sam entry
\--------------------------------------------------------*/
static uchar findSamCig(
    FILE *inFILE, /*File to search for cigar in*/
    struct samEntry *samStruct
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-10 TOC: findSamCig
   '  - Finds the cigar in same entry & also finds the
   '    number of bases in query from cigar
   '  o fun-10 sec-01:
   '    - Variable declerations
   '  o fun-10 sec-02:
   '    - Set query pointer & read to start of flag
   '  o fun-10 sec-03:
   '    - Read in flag & move to start of reference id
   '  o fun-10 sec-04:
   '    - Set reference id pointer & move to position
   '  o fun-10 sec-05:
   '    - Read position & move to start of mapq
   '  o fun-10 sec-06:
   '    - Read in mapq & move to start of cigar
   '  o fun-10 sec-07:
   '    - Read in cigar
   '  o fun-10 sec-08:
   '    - Find length of sequence in the cigar
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-01:
    ^  - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *oldBufAddUChar = 0;/*Used to refind pointers*/
    char *samIterUChar = samStruct->samEntryCStr;
    uint cigEntryUInt = 0;
    uint intIter = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-02:
    ^  - Set query pointer & read to start of flag
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samStruct->queryCStr = samIterUChar;

    while(*samIterUChar != '\t')
    { /*Loop: till off the query id*/
        if(*samIterUChar == '\0')
        { /*If: I did not get a full line*/
            samIterUChar =
               readNextPartOfLine(
                 inFILE,
                 samStruct,
                 samStruct->lenBuffULng/*Double buff size*/
            ); /*Read in next section of line*/

            /*Check if I have an incomplete sam aligment*/
            if(samIterUChar == 0) return 4; 

            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
        } /*If: I did not get a full line*/

        ++samIterUChar;
    } /*Loop: till off the query id*/

    ++samIterUChar;   /*Get off tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-03:
    ^  - Read in flag & move to start of reference id
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Get how many characters mapq is in the sam entry*/
    cigEntryUInt = samIterUChar - samStruct->samEntryCStr;

    /*Need to make sure entire flag is read in*/ 
    for(
        intIter = 0;
       *(samIterUChar+intIter) != '\t';
       ++intIter
    ) { /*Loop: till at end of number*/
        if(*(samIterUChar + intIter) == '\0')
        { /*If: at end of buffer*/
            samIterUChar =
               readNextPartOfLine(
                 inFILE,
                 samStruct,
                 samStruct->lenBuffULng/*Double buff size*/
            ); /*Read in next section of line*/

            /*check if is an incomplete sam aligment*/
            if(samIterUChar == 0) return 4;

            /*Make sure pionter is set start of flag*/
            samIterUChar =
               samStruct->samEntryCStr + cigEntryUInt;
            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
        } /*If: at end of buffer*/
    } /*Loop: till at end of number*/

    samIterUChar =
       base10StrToU16(samIterUChar, samStruct->flagUSht);
    ++samIterUChar;   /*Get off tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-04:
    ^  - Set reference id pointer & move to position
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samStruct->refCStr = samIterUChar;

    while(*samIterUChar != '\t')
    { /*While on the reference id*/
        if(*samIterUChar == '\0')
        { /*If at end of buffer*/
            oldBufAddUChar = samStruct->samEntryCStr;

            samIterUChar =
              readNextPartOfLine(
                inFILE,
                samStruct,
                samStruct->lenBuffULng /*Double buff size*/
            ); /*Read in next section of line*/

            /*Check if is an incomplete sam aligment*/
            if(samIterUChar == 0) return 4;

            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
    
            samStruct->refCStr =
                samStruct->samEntryCStr +
                (samStruct->refCStr - oldBufAddUChar);
        } /*If at end of buffer*/

        ++samIterUChar;
    } /*While on the reference id*/

    ++samIterUChar;   /*Get off tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-05:
    ^  - Get off position & move to start of mapq
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    for(
       intIter = 0;
       *(samIterUChar+intIter) != '\t';
       ++intIter
    ){ /*Loop: till at start of of mapq*/
        if(*samIterUChar == '\0')
        { /*If: at end of buffer*/
            oldBufAddUChar = samStruct->samEntryCStr;

            samIterUChar =
              readNextPartOfLine(
                inFILE,
                samStruct,
                samStruct->lenBuffULng /*Double buff size*/
            ); /*Read in next section of line*/

            /*Check if is an incomplete sam aligment*/
            if(samIterUChar == 0) return 4;

            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
    
            samStruct->refCStr =
                samStruct->samEntryCStr +
                (samStruct->refCStr - oldBufAddUChar);
        } /*If: at end of buffer*/
    } /*Loop: till at start of of mapq*/

    /*Record the position entry*/
    samIterUChar =
       base10StrToU32(
          samIterUChar,
          samStruct->posOnRefUInt
       );

    ++samIterUChar;   /*Get off tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-06:
    ^  - Read in mapq & move to start of cigar
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Get how many characters mapq is in the sam entry*/
    cigEntryUInt = samIterUChar - samStruct->samEntryCStr;

    /*Need to make sure entire mapq is read in*/ 
    for(
       intIter = 0;
       *(samIterUChar+intIter) != '\t';
       ++intIter
    ){ /*Loop: till at end of mapq (is number)*/
        if(*(samIterUChar + intIter) == '\0')
        { /*If: at end of buffer*/
            oldBufAddUChar = samStruct->samEntryCStr;

            samIterUChar =
              readNextPartOfLine(
                inFILE,
                samStruct,
                samStruct->lenBuffULng /*Double buff size*/
            ); /*Read in next section of line*/

            /*Check if is an incomplete sam aligment*/
            if(samIterUChar == 0) return 4;

            /*Make sure pionter points to start of mapq*/
            samIterUChar =
               samStruct->samEntryCStr + cigEntryUInt;

            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
    
            samStruct->refCStr =
                samStruct->samEntryCStr +
                (samStruct->refCStr - oldBufAddUChar);
        } /*If: at end of buffer*/
    } /*Loop: till at end of mapq (is number)*/

    samIterUChar =
       base10StrToU8(samIterUChar, samStruct->mapqUChar);
    ++samIterUChar;   /*Get off tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-07:
    ^  - Read in cigar (make sure I have a full entry)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samStruct->cigarCStr = samIterUChar;

    while(*samIterUChar != '\t')
    { /*Loop: Till at end of cigar*/
        if(*samIterUChar == '\0')
        { /*If: at end of buffer*/
            oldBufAddUChar = samStruct->samEntryCStr;

            samIterUChar =
              readNextPartOfLine(
                inFILE,
                samStruct,
                samStruct->lenBuffULng /*Double buff size*/
            ); /*Read in next section of line*/

            /*Check if is an incomplete sam aligment*/
            if(samIterUChar == 0) return 4;

            /*Readjust previous pointers*/
            samStruct->queryCStr = samStruct->samEntryCStr;
    
            samStruct->refCStr =
                samStruct->samEntryCStr +
                (samStruct->refCStr - oldBufAddUChar);

            samStruct->cigarCStr =
                samStruct->samEntryCStr +
                (samStruct->cigarCStr - oldBufAddUChar);
        } /*If: at end of buffer*/

        ++samIterUChar;
    } /*Loop: Till at end of cigar*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-10 Sec-08:
    ^  - Find length of sequence in the cigar
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samIterUChar = samStruct->cigarCStr;

    while(*samIterUChar != '\t')
    { /*Loop: though all cigar entries to read in*/
        samIterUChar =
           base10StrToU32(samIterUChar, cigEntryUInt);

        switch(*samIterUChar)
        { /*Switch: Count number of cigar entries*/
           case '=':
           case 'M':
           case '\t':
              samStruct->unTrimReadLenUInt += cigEntryUInt;
              samStruct->numMatchUInt += cigEntryUInt;
              break;
           case 'X':
              samStruct->unTrimReadLenUInt += cigEntryUInt;
              samStruct->numSnpUInt += cigEntryUInt;
              break;
           case 'I':
              samStruct->unTrimReadLenUInt += cigEntryUInt;
              samStruct->numInsUInt += cigEntryUInt;
              break;
           case 'D':
              samStruct->numDelUInt += cigEntryUInt;
              break;
           case 'S':
              samStruct->unTrimReadLenUInt += cigEntryUInt;
              break;
           default: break;
        } /*Switch: Count number of cigar entries*/

        cigEntryUInt = 0;
        ++samIterUChar; /*Get off entry character type*/
    } /*Loop: though all cigar entries to read in*/

    samStruct->readLenUInt = samStruct->unTrimReadLenUInt;
    return 1;
} /*findSamCig*/

/*-------------------------------------------------------\
| Name: readSamLine (Fun-11:)
| Use:
|  - Reads in a sam file entry into a samEntry struct
| Input:
|  - samStruct:
|    o Pointer to samEntry struct to store the sam entry in
|  - inFILE:
|    o File with sam entry to get
| Output:
|  - Returns:
|    o 1 if succeded
|    o 2 if end of file
|    o 64 if memory allocation error
|  - Modifies:
|    o All pointers in samStruct & variables holding read
|      Lenths
\-------------------------------------------------------*/
static uchar readSamLine(
    struct samEntry *samST,
    FILE *inFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC: readSamLine
   '  - Reads in sam entry into a samEntry struct
   '  o fun-11 sec-01:
   '    - Variable declerations
   '  o fun-11 sec-02:
   '    - Check if have buffer to read into/add buffer
   '  o fun-11 sec-03:
   '    - Check if can read line in one go
   '  o fun-11 sec-04:
   '    - Check if is header & need to read in more
   '  o fun-11 sec-05:
   '    - If not read in cigar & find sequence length
   '  o fun-11 sec-06:
   '    - Check if have full sequence from cigar read in
   '  o fun-11 sec-07:
   '    - If not find sequence length
   '  o fun-11 sec-08:
   '    - Use sequence length to get sequence & q-score
   '  o fun-11 sec-09:
   '    - If not, read rest of cigar by adding 1kb chunks
   '  o fun-11 sec-10:
   '    - Set sequence & q-score pointers
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-01:
    ^  - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *oldBufAddUChar = 0;
    char *samIterUChar = 0; /*fget error or end of file*/
    char checkC = 0;        /*Used for single char checks*/
    ulong newBuffSizeULng = 0;
    uint intTab = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-02:
    ^  - Check if have buffer to read into & prepare buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(samST->samEntryCStr == 0)
    { /*If: I have no buffer*/
        samST->lenBuffULng = 1024;
        samST->samEntryCStr = malloc(sizeof(char) * 1024);
    } /*If: I have no buffer*/

    /*Set second to last character in buffer to null.
    ' This tells me if I read in an entire line
    */
    samST->samEntryCStr[samST->lenBuffULng - 2] = '\0';

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-03:
    ^  - Check if can read line in one go
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samIterUChar =
       fgets(
            samST->samEntryCStr,
            samST->lenBuffULng,
            inFILE
    ); /*See if can read in a single line*/

    if(samIterUChar == 0)
        return 2;           /*At end of the file*/

    checkC = samST->samEntryCStr[samST->lenBuffULng - 2];

    if(checkC =='\0' || checkC == '\n')
    { /*If: I grabed the entire line*/
        processSamEntry(samST);
        return 1;
    } /*If: I grabed the entire line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-04:
    ^  - Check if is header & need to read in more
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(*samST->samEntryCStr == '@')
    { /*If: this is a header line*/
        newBuffSizeULng = 1024; /*Expand buffer by*/
            /*I should be close to the full entry, so 
            ` expand the buffer by in small chunks only
            */
    
        while(!(checkC == '\0' || checkC == '\n'))
        { /*Loop: Get the next part of the line*/
            samIterUChar =
               readNextPartOfLine(
                  inFILE,
                  samST,
                  newBuffSizeULng
            ); /*Get the next or remnaing part of line*/

            if(samIterUChar == 0) return 64;

            /*Find the ending character*/
            checkC =
              samST->samEntryCStr[samST->lenBuffULng -2];
        } /*Loop: Get the next part of the line*/

        return 1;
    } /*If: this is a header line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-05:
    ^  - Read in cigar & get sequence length from cigar
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Read in cigar (findSamCig) & check for memory error*/
    if(findSamCig(inFILE, samST) == 4)
        if(samST->samEntryCStr == 0) return 64;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-06:
    ^  - Check if have full sequence from cigar read in
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*Check if have full sequence & set pionters*/
    checkC = samST->samEntryCStr[samST->lenBuffULng - 2];

    if(checkC =='\0' || checkC == '\n')
    { /*If: I now have the entire line*/
        samIterUChar = samST->cigarCStr;

        /*I have to get past the cigar, RNEXT, PNEXT, &
        ` TLEN entires
        */
        for(intTab = 0; intTab < 4; ++intTab)
        { /*Loop: till on sequence entry*/
            while(*samIterUChar != '\t')
                ++samIterUChar;
            ++samIterUChar; /*get off the tab*/
        } /*Loop: till on sequence entry*/

        samST->seqCStr = samIterUChar;
        samST->qCStr =
           samIterUChar + samST->readLenUInt + 1;
            /*reaLenUint to move past sequence; +1 to get
            ` off the tab
            */
        return 1;
    } /*If: I now have the entire line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-07:
    ^  - If not find sequence length
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    samIterUChar = samST->cigarCStr;

    while(*samIterUChar != '\t')
    { /*Loop: till at end of cigar*/
        ++samIterUChar;
        ++newBuffSizeULng;
        /*Find length of the cigar (avoid guessing)*/
    } /*Loop: till at end of cigar*/

    /*MD entries will be bit longer than cigars, but I can
    ` double the cigar to get a rough size. This way I
    ` only have to read in once or twice
    */
    newBuffSizeULng = newBuffSizeULng << 1;

    /*Figure out buffer size with sequence & q-score*/
    newBuffSizeULng +=         /*newBuff has Cigar length*/
          (samST->readLenUInt << 1)  /*seq + q-score*/
        + samST->cigarCStr
        - samST->samEntryCStr /*pre cigar*/
        + 2048;                            /*Extra buffer*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-07:
    ^  - Use sequence length to get sequence & q-score
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(newBuffSizeULng > samST->lenBuffULng)
    { /*If: new buffer size with the sequence is bigger*/
        newBuffSizeULng -= samST->lenBuffULng;
        oldBufAddUChar = samST->samEntryCStr;

        samIterUChar =
           readNextPartOfLine(
              inFILE,
              samST,
              newBuffSizeULng
        ); /*See if I can finish the line*/

        if(samST->samEntryCStr == 0) return 64;

        /*Readjust previous pointers*/
        samST->queryCStr = samST->samEntryCStr;
    
        samST->refCStr =
            samST->samEntryCStr +
            (samST->refCStr - oldBufAddUChar);

        samST->cigarCStr =
            samST->samEntryCStr +
            (samST->cigarCStr - oldBufAddUChar);
    } /*If: new buffer size with the sequence is bigger*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-09:
    ^  - If not, read rest of cigar by adding 1kb chunks
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    newBuffSizeULng = 1024;
    /*I Should be close to the full line length.
    ` So I am going to read in small chunks at a time
    */
    checkC =
      *(samST->samEntryCStr +samST->lenBuffULng-2);

    while(!(checkC == '\0' || checkC == '\n'))
    { /*Loop: Finish reading in the sam entry*/
        oldBufAddUChar = samST->samEntryCStr;
        samIterUChar =
            readNextPartOfLine(
               inFILE,
               samST,
               newBuffSizeULng
        ); /*Read in the next part of the line*/

        if(samST->samEntryCStr == 0) return 64;

        checkC =
          *(
               samST->samEntryCStr
             + samST->lenBuffULng
             - 2
        ); /*Find the ending character*/

        /*Readjust previous pointers*/
        samST->queryCStr = samST->samEntryCStr;
    
        samST->refCStr =
            samST->samEntryCStr +
            (samST->refCStr - oldBufAddUChar);

        samST->cigarCStr =
            samST->samEntryCStr +
            (samST->cigarCStr - oldBufAddUChar);
    } /*Loop: Finish reading in the sam entry*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-11 Sec-10:
    ^  - Set sequence & q-score pointers
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*At this point I have the entire entry, but I still
    `  need to set some pointers
    */
    samIterUChar = samST->cigarCStr;

    /*TODO: SEQ IS ON Q-SCORE, FIX go though cigar entry
    ` I need to go through the cigar, RNEXT, PNEXT, & TLEN
    ` entires. This should not be the case and needs to be
    ` fixed.
    */
    for(intTab = 0; intTab < 4; ++intTab)
    { /*Loop: Go to seq entry start*/
        while(*samIterUChar != '\t')
            ++samIterUChar;
        ++samIterUChar; /*get off the tab*/
    } /*Loop: Go to seq entry start*/

    samST->seqCStr = samIterUChar;

    if(*samST->seqCStr != '*')
    { /*If: I have a sequence entry*/
        if(*samST->cigarCStr != '*')
            samST->qCStr = samIterUChar + samST->readLenUInt +1;
        else
        { /*Else: I have no cigar entry*/
            while(*samIterUChar > 32)
            { /*Loop: Find lenght of sequence*/
                ++samST->unTrimReadLenUInt;
                ++samIterUChar;
            } /*Loop: Find lenght of sequence*/

            samST->readLenUInt =
               samST->unTrimReadLenUInt;
            ++samIterUChar;  /*Get off tab after sequence*/
            samST->qCStr = samIterUChar;
        } /*Else: I have no cigar entry*/
    } /*If: I have a sequence entry*/

    else samST->qCStr = samIterUChar +2;/*No sequence*/
   
    return 1;
} /*readSamLine*/

/*-------------------------------------------------------\
| Name: printSamEntry (Fun-12:)
| Use:
|  - Prints the sam file entry to a file. This does not
|    print any extra stats that were found.
| Input:
|  - samST
|    o Pointer to samEntry struct with sam entry to print
|  - outFILE:
|    o File to print the sam entry to
| Output:
|  - Prints:
|    o Sam file entry in samST to outFILE.
\-------------------------------------------------------*/
#define printSamEntry(samPtrST, outFILE){\
    /*
    ` printing out query name since it is the start of the
    ` sam entry. This allows me to store the sam entry in a
    ` different samEntry structure, but still have a handle
    ` on the entire entry with the query. I do this when
    ` their might be multiple entries for a single read.
    ` In this case only the first entry has the sequence.
    */\
    if((samPtrST)->queryCStr != 0)\
        fprintf((outFILE), "%s", (samPtrST)->queryCStr);\
    else\
       fprintf((outFILE),"%s",(samPtrST)->samEntryCStr);\
} /*printSamEntry*/

/*--------------------------------------------------------\
| Fun-13: samToFq
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
\--------------------------------------------------------*/
#define samToFq(samST, outFILE){\
    char *macQryStr = (samST)->queryCStr;\
    uint macLenIdUI = 0;\
    \
    /*Find the end of the query id*/\
    for(\
       macLenIdUI = 0;\
       macQryStr[macLenIdUI] > 32;\
       ++macLenIdUI\
    ){}\
    \
    /*Check If I need to print out the @ for the header*/\
    if((samST)->queryCStr[0] != '@')\
        fwrite("@", sizeof(char), 1, (outFILE));\
    \
    fwrite(\
       (samST)->queryCStr,\
       sizeof(char),\
       macLenIdUI,\
       (outFILE)\
    ); /*print out the query id*/\
    \
    fwrite("\n", sizeof(char), 1, (outFILE));\
    \
    fwrite(\
        (samST)->seqCStr,\
        sizeof(char),\
        (samST)->readLenUInt,\
        (outFILE)\
    ); /*Write sequence entry*/\
    \
    /*Print out the spacer entry*/\
    fwrite("\n+\n", sizeof(char), 3, (outFILE));\
    \
    fwrite(\
        (samST)->qCStr,\
        sizeof(char),\
        (samST)->readLenUInt,\
        (outFILE)\
    ); /*Write Q-score entry*/\
    \
    /*Write new line for next entry*/\
    fwrite("\n", sizeof(char), 1, (outFILE));\
} /*samToFq*/

/*--------------------------------------------------------\
| Fun-14: printStatHeader
| Use:
|  - Prints the header for the stats file
| Input:
|  - statFILE:
|    o File to print the header to
| Output:
|  - Prints:
|    o stat file header to the input file
\--------------------------------------------------------*/
#define printStatHeader(statFILE){\
    if((statFILE) == 0)\ return;\
    \
    fprintf((statFILE), "Read\tRef\tMAPQ\treadLength");\
    fprintf((statFILE), "\talignedLength\tmatches"); \
    fprintf((statFILE), "\tInss\tDels\tSNPs");\
    fprintf((statFILE), "ttmedianQ\tmeanQ"); \
    \
    fprintf(\
       (statFILE),\
        "\talignedMedianQ\talignedMeanQ\n"\
    );\
} /*printStatHeader*/

/*--------------------------------------------------------\
| Fun-15: printSamStats
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
\--------------------------------------------------------*/
#define printSamStats(samSTPtr, pHeadBl, outFILE){\
    char *tmpQueryCStr = (samSTPtr)->queryCStr;\
    char *tmpRefCStr = (samSTPtr)->refCStr;\
    \
    if((pHeadBl) == 1)\
    { /*If printing out the header entry*/\
        printStatHeader((outFILE));\
        (pHeadBl) = 0;\
    } /*If printing out the header entry*/\
    \
    /*Convert query to c-string*/\
    while(*tmpQueryCStr != '\t') tmpQueryCStr++;\
    *tmpQueryCStr = '\0';\
    \
    /*Convert reference to c-string*/\
    while(*tmpRefCStr != '\t')\ tmpRefCStr++;\
    *tmpRefCStr = '\0';\
    \
    /*Print out the sam entry stats*/\
    fprintf(\
        (outFILE),\
        "%s\t%s\t%u\t%u\t%u",\
        (samSTPtr)->queryCStr, \
        (samSTPtr)->refCStr,\
        (samSTPtr)->mapqUChar,\
        (samSTPtr)->readLenUInt,\
        (samSTPtr)->readAligLenUInt\
    ); /*General stats*/\
    \
    fprintf(\
        (outFILE),\
        "\t%u\t%u\t%u\t%u",\
        (samSTPtr)->numMatchUInt,\
        (samSTPtr)->numSnpUInt,\
        (samSTPtr)->numInsUInt,\
        (samSTPtr)->numDelUInt\
    ); /*read stats*/\
    \
    fprintf(\
        (outFILE),\
        "\t%f\t%f\t%f\t%f\t%u\t%u\t%u\n",\
        (samSTPtr)->medianQFlt,\
        (samSTPtr)->meanQFlt,\
        (samSTPtr)->medianAligQFlt,\
        (samSTPtr)->meanAligQFlt\
    ); /*Quality score stats*/\
    \
    /*revert back into sam format*/\
    *tmpQueryCStr = '\t';\
    *tmpRefCStr = '\t'; \
} /*printSamStats*/

/*--------------------------------------------------------\
| Name: swapSamEntries (Fun-16:)
| Use:
|  - Swaps the values in two samEntry structs
| Input:
|  - samOneST:
|    o First samEntry structure to swap values
|  - samTwoST:
|    o Second samEntry structure to swap values
| Output:
|  - Modifies:
|    o samOnST to have samTwoST's values
|    o samTwoST to have samOneST's values
| Note:
|  - This does swap the histograms. So, there is a bit of
|    work in this swap.
\--------------------------------------------------------*/
#define void swapSamEntries(\
   *samOneST, /*First entry to swap*/\
   *samTwoST  /*Second entry to swap*/\
){\
   char *macSwapStr = 0;\
   uchar macSwapUC = 0;\
   ushort macSwapUS = 0;\
   uint macSwapUI = 0;\
   ulong macSwapUL = 0;\
   float macSwapFlt = 0;\
   uint iQMac = 0;\
   \
   macSwapStr = samOneST->samEntryCStr;\
   samOneST->samEntryCStr = samTwoST->samEntryCStr;\
   samTwoST->samEntryCStr = macSwapStr;\
   \
   macSwapStr = samOneST->queryCStr;\
   samOneST->queryCStr = samTwoST->queryCStr;\
   samTwoST->queryCStr = macSwapStr;\
   \
   macSwapStr = samOneST->refCStr;\
   samOneST->refCStr = samTwoST->refCStr;\
   samTwoST->refCStr = macSwapStr;\
   \
   macSwapStr = samOneST->cigarCStr;\
   samOneST->cigarCStr = samTwoST->cigarCStr;\
   samTwoST->cigarCStr = macSwapStr;\
   \
   macSwapStr = samOneST->seqCStr;\
   samOneST->seqCStr = samTwoST->seqCStr;\
   samTwoST->seqCStr = macSwapStr;\
   \
   macSwapStr = samOneST->qCStr;\
   samOneST->qCStr = samTwoST->qCStr;\
   samTwoST->qCStr = macSwapStr;\
   \
   macSwapUC = samOneST->mapqUChar;\
   samOneST->mapqUChar = samTwoST->mapqUChar;\
   samTwoST->mapqUChar = macSwapUC;\
   \
   macSwapUS = samOneST->flagUSht;\
   samOneST->flagUSht = samTwoST->flagUSht;\
   samTwoST->flagUSht = macSwapUS;\
   \
   macSwapFlt = samOneST->medianQFlt;\
   samOneST->medianQFlt = samTwoST->medianQFlt;\
   samTwoST->medianQFlt = macSwapFlt;\
   \
   macSwapFlt = samOneST->meanQFlt;\
   samOneST->meanQFlt = samTwoST->meanQFlt;\
   samTwoST->meanQFlt = macSwapFlt;\
   \
   macSwapFlt = samOneST->medianAligQFlt;\
   samOneST->medianAligQFlt = samTwoST->medianAligQFlt;\
   samTwoST->medianAligQFlt = macSwapFlt;\
   \
   macSwapFlt = samOneST->meanAligQFlt;\
   samOneST->meanAligQFlt = samTwoST->meanAligQFlt;\
   samTwoST->meanAligQFlt = macSwapFlt;\
   \
   macSwapUI = samOneST->posOnRefUInt;\
   samOneST->posOnRefUInt = samTwoST->posOnRefUInt;\
   samTwoST->posOnRefUInt = macSwapUI;\
   \
   macSwapUI = samOneST->unTrimReadLenUInt;\
   \
   samOneST->unTrimReadLenUInt =\
      samTwoST->unTrimReadLenUInt;\
   \
   samTwoST->unTrimReadLenUInt = macSwapUI;\
   \
   macSwapUI = samOneST->readLenUInt;\
   samOneST->readLenUInt = samTwoST->readLenUInt;\
   samTwoST->readLenUInt = macSwapUI;\
   \
   macSwapUI = samOneST->readAligLenUInt;\
   samOneST->readAligLenUInt = samTwoST->readAligLenUInt;\
   samTwoST->readAligLenUInt = macSwapUI;\
   \
   macSwapUI = samOneST->numMatchUInt;\
   samOneST->numMatchUInt = samTwoST->numMatchUInt;\
   samTwoST->numMatchUInt = macSwapUI;\
   \
   macSwapUI = samOneST->numSnpUInt;\
   samOneST->numSnpUInt = samTwoST->numSnpUInt;\
   samTwoST->numSnpUInt = macSwapUI;\
   \
   macSwapUI = samOneST->numDelUInt;\
   samOneST->numDelUInt = samTwoST->numDelUInt;\
   samTwoST->numDelUInt = macSwapUI;\
   \
   macSwapUI = samOneST->numInsUInt;\
   samOneST->numInsUInt = samTwoST->numInsUInt;\
   samTwoST->numInsUInt = macSwapUI;\
   \
   macSwapUI = samOneST->numInsUInt;\
   samOneST->numInsUInt = samTwoST->numInsUInt;\
   samTwoST->numInsUInt = macSwapUL;\
   \
   macSwapUL = samOneST->totalAlnQScoreULng;\
   samOneST->totalAlnQScoreULng =\
      samTwoST->totalAlnQScoreULng;\
   samTwoST->totalAlnQScoreULng = macSwapUL;\
   \
   macSwapUL = samOneST->lenBuffULng;\
   samOneST->lenBuffULng = samTwoST->lenBuffULng;\
   samTwoST->lenBuffULng = macSwapUL;\
   \
   for(iQMac = 0; iQMac < MAX_Q_SCORE; ++iQMac)\
   { /*Loop: Swap Q-score histogram entries*/\
     macSwapUI = samOneST->seqQHistUInt[iQMac];\
     \
     samOneST->seqQHistUInt[iQMac] =\
        samTwoST->seqQHistUInt[iQMac];\
     \
     samTwoST->seqQHistUInt[iQMac] = macSwapUI;\
     \
     /*Deal with the aligned histogram*/\
     macSwapUI = samOneST->seqQAlnHistUInt[iQMac];\
     \
     samOneST->seqQAlnHistUInt[iQMac] =\
        samTwoST->seqQAlnHistUInt[iQMac];\
     \
     samTwoST->seqQAlnHistUInt[iQMac] = macSwapUI;\
   } /*Loop: Swap Q-score histogram entries*/\
} /*swapSamEntries*/

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
