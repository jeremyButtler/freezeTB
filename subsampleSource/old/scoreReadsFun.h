/*########################################################
# Name: scoreReadsFun
# Use:
#   - Holds functions needed for scoreReads.
# Libraries:
#   - "minAlnStatsStruct.h"
#   - "../samEntryStruct.h"
#   o "../base10StrToNum.h"
#   o "../dataTypeShortHand.h"
# C Standard Libraries:
#   o <stdint.h>
#   o <stdlib.h>
#   o <string.h>
#   o <stdio.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'     - Included libraries and definitions
'  o fun-01: checkSNPs
'     - Check if want to keep or ignore a mismatch
'  o fun-02: checkMatches:
'    - Check if matches should be kept
'  o fun-03: checkInss:
'    - Checks if insertion should be kept
'  o fun-04: checkDels
'    - Checks if deletions should be kept
'  o fun-05: checkSoftMasks
'    - Increments pointer when soft mask is present
'  o fun-06: qHistToMed
'    - Uses a histogram of q-scores to find the median
'      q-score
'  o fun-07: scoreAln
'    - Scores a single alignment in a sam file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and definitions
\-------------------------------------------------------*/

#ifndef SCORE_READS_FUN_H
#define SCORE_READS_FUN_H

#include "minAlnStatsStruct.h"
#include "../samEntryStruct.h"

#define charToQ(inC) ((inC) - Q_ADJUST)

/*-------------------------------------------------------\
| Fun-01: checkSNPs
|   - Find the number of snps that are above the minimum
|     threshold. Also get q-scores
| Input:
|   - numSnpsUI:
|     o Number of snps to check
|   - baseOnUI:
|     o first base of snps
|   - minStats:
|     o minAlnStats structure with settings to use
|   - samST:
|     o Has the sam entry to check
|   - qLineBl:
|     o 1: I have a Q-score line
|     o 0: No Q-score line was given
|   - refQBool
| Output:
|   - Modifies:
|     o seqQAlnHist to have the q-scores
|     o snp counters and aligned length couters in samST
|     o baseOnUI to point to base after the last snp
\-------------------------------------------------------*/
static void checkSNPs(
    uint numSnpsUI,
    uint *baseOnUI,
    struct minAlnStats *minStats,
    struct samEntry *samST,
    uchar qLineBl
){ /*checkSNPs*/
    uchar keepBl = 0; /*Keep or ignore mismatch*/
    uint endUI = *baseOnUI + numSnpsUI;
    int qI = 0;

    if(qLineBl)
    { /*If: I had Q-score entires*/
       for(
          *baseOnUI = *baseOnUI;
          *baseOnUI < endUI; 
          ++(*baseOnUI)
       ){ /*Loop through all bases that are mismatches*/
          qI = charToQ(samST->qCStr[*baseOnUI]);
          keepBl = (qI >= minStats->minQChar);

          ++samST->numSNPUInt;
          samST->numKeptSNPUInt += keepBl;

          /*Get the stats*/
          ++samST->seqQAlnHistUInt[qI];
          samST->totalAlnQScoreULng += qI;
          ++samST->readAligLenUInt;
       } /*Loop through all bases that are mismatches*/
    } /*If: I had Q-score entires*/

    else
    { /*Else: I have no Q-score entires*/
       samST->numSNPUInt += numSnpsUI;
       samST->numKeptSNPUInt += numSnpsUI;
       samST->readAligLenUInt += numSnpsUI;
       *baseOnUI = endUI;
    } /*Else: I have no Q-score entires*/

    return;
} /*checkSNPs*/

/*-------------------------------------------------------\
| Fun-02: checkMatches
|   - Find the number of mathces that are above the
|     minimum threshold. Also get q-scores
| Input:
|   - numMatchesUI:
|     o Number of matches to check
|   - baseOnUI:
|     o first base of matches
|   - minStats:
|     o minAlnStats structure with settings to use
|   - samST:
|     o Has the sam entry to check
|   - qLineBl:
|     o 1: I have a Q-score line
|     o 0: No Q-score line was given
|   - refQBool
| Output:
|   - Modifies:
|     o seqQAlnHist to have the q-scores
|     o mathc counters and aligned length couters in samST
|     o baseOnUI to point to base after the last match
\-------------------------------------------------------*/
void checkMatches(
    uint numMatchesUI,
    uint *baseOnUI,
    struct minAlnStats *minStats,
    struct samEntry *samST,
    uchar qLineBl
){ /*checkMatches*/
    uchar keepBl = 0; /*Keep or ignore mismatch*/
    uint endBaseUI = *baseOnUI + numMatchesUI;
    int qI = 0;

    if(qLineBl)
    { /*If: I have a q-score entry*/
       for(
         *baseOnUI = *baseOnUI;
         *baseOnUI < endBaseUI;
         ++(*baseOnUI)
       ){ /*Loop through all bases that are matches*/
          qI = charToQ(samST->qCStr[*baseOnUI]);
          keepBl = (qI >= minStats->minQChar);

          ++samST->numMatchUInt; /*Total matches*/
          samST->numKeptMatchUInt += keepBl;

          /*Get the stats*/
          ++samST->seqQAlnHistUInt[qI];
          samST->totalAlnQScoreULng += qI;
          ++samST->readAligLenUInt;
       } /*Loop through all bases that are matches*/
    } /*If: I have a q-score entry*/

    else
    { /*Else: I have no q-score line*/
       samST->numMatchUInt += numMatchesUI;
       samST->numKeptMatchUInt += numMatchesUI;
       samST->readAligLenUInt += numMatchesUI;
       *baseOnUI = endBaseUI;
    } /*Else: I have no q-score line*/

    return;
} /*checkMatches*/

/*-------------------------------------------------------\
| Fun-03: checkInss
|   - Checks to see if keeping insertions or not
| Input:
|   - numInsUI:
|     o Number of insertions in this part of the sequence
|   - baseOnUI:
|     o first base of inhsedrtions
|   - minStats:
|     o Structure with settings for checking homopolymers
|       with
|   - samST:
|     o samEntry sturct with sequence to check and to
|       record stats in
|   - qLineBl:
|     o 1: This sequence has a q-score entry
|     o 0: This sequence is missing the q-score entry
| Output:
|   - Modifies:
|     o seqQAlnHist to have the q-scores
|     o insertion counters and aligned length couters in
|       samST
|     o baseOnUI to point to base after the last insertion
\-------------------------------------------------------*/
static void checkInss(
    uint numInsUI,
    uint *baseOnUI,
    struct minAlnStats *minStats,
    struct samEntry *samST,
    uchar qLineBl
){  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
    ' Fun-03 TOC: checkInss
    '   - Check if keeping insertions or not
    '   o fun-03 sec-01:
    '     - variable declerations
    '   o fun-03 sec-02:
    '     - Find the homopolymer length
    '   o fun-03 sec-03:
    '     - Check the insertions / get stats
    \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-03 Sec-01:
    ^   - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    int keepBl = 0; /*Keep or ignore mismatch*/
    int homoBl = 0; /*Marks if over homopolymer limit*/
    uint endBaseUI = *baseOnUI + numInsUI;

    char baseC;
    char *seqStr = 0;
    int qI = 0; /*Holds the adjusted q-score*/
    
    uint lenHomoUI = 0;    /*Size of homopolymer*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-03 Sec-02:
    ^   - Find the homopolymer length
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    seqStr = &samST->seqCStr[*baseOnUI];
    baseC = samST->seqCStr[*baseOnUI] & ~33;
      /*~33 converts to lower cases and merges T and U*/

    /*This is kinda hacky, but my logic is to see the
    ` size of the homopolymer around the insertion. This
    ` should give me a good idea of the original
    ` homopolymer
    */

    for(
       lenHomoUI = 0;
       baseC == (seqStr[lenHomoUI] & ~33);
       ++lenHomoUI
    ) {}

    --seqStr;
    ++lenHomoUI; /*Single bases are homopolymer of 1*/

    while(baseC == (*seqStr & ~33))
    { /*Loop: Check the reverse direction*/
        ++lenHomoUI;
        --seqStr;
    } /*Loop: Check the reverse direction*/

    homoBl =
          minStats->maxHomoInsAry[(baseC & ~(64+128)) >>1]
       <= lenHomoUI;
        /* Logic:
        `  - & ~(64 + 128) clears 1st & last 3 bits
        `     - 64 & 128: Ensures array size is under 32
        `     - >> 1 removes 1st bit (reduces to 16 bytes)
        `  - minStats->maxInsHomoUIntAry has the maximum
        `    size of homopolymer to keep a deletion
        `  - maxHomoSize >= lenHomoInt
        `    Goes to 1 if the homopolymer does not go
        `    over the maximum size, else 0.
        */

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-03 Sec-03:
    ^   - Check the insertions / get stats
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    homoBl = -homoBl;

    if(qLineBl)
    { /*If: I have a Q-score entry*/
       for(
          *baseOnUI = *baseOnUI;
          *baseOnUI < endBaseUI;
          ++(*baseOnUI)
       ){ /*Loop through all insertions*/
          qI = charToQ(samST->qCStr[*baseOnUI]);
          keepBl = (qI >= minStats->minQChar);

          ++samST->readAligLenUInt;
          ++samST->seqQAlnHistUInt[qI];
          ++samST->totalAlnQScoreULng;

          ++samST->numInsUInt;
          samST->numKeptInsUInt += (keepBl & homoBl);
       } /*Loop through all insertions*/
    } /*If: I have a Q-score entry*/
 
    else
    { /*Else: I have no q-score entry*/
       samST->readAligLenUInt += (homoBl & numInsUI);
       samST->numInsUInt += (homoBl & numInsUI);
       samST->numKeptInsUInt += (homoBl & numInsUI);
       *baseOnUI = endBaseUI;
    } /*Else: I have no q-score entry*/

    return;
} /*checkInss*/

/*-------------------------------------------------------\
| Fun-04: checkDels
|   - Checks to see if keeping deletions or not
| Input:
|   - numDelUI:
|     o Number of deletions in this part of the sequence
|   - baseOnUI:
|     o first base of deletions
|   - minStats:
|     o Structure with settings for checking homopolymers
|       with
|   - samST:
|     o samEntry sturct with sequence to check and to
|       record stats in
| Output:
|   - Modifies:
|     o seqQAlnHist to have the q-scores
|     o deletion counters and aligned length couters in
|       samST
\-------------------------------------------------------*/
static void checkDels(
    uint numDelUI,
    uint baseOnUI,
    struct minAlnStats *minStats,
    struct samEntry *samST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ^ Fun-05: checkDeletions
   '   o fun-04 sec-01:
   '     - Variable declerations
   '   o fun-04 sec-02:
   '     - Find if is a valid deletion
   '   o fun-04 sec-03:
   '     - Check if deletion should be kept
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *baseStr = 0;
   char revBaseC = *(samST->seqCStr + baseOnUI - 1) & ~33;
   char forBaseC = samST->seqCStr[baseOnUI] & ~33;

   int keepBl = 0;

   /*Variables for finding the homopolymer length*/
   int revHomoI = 0;
   int forHomoI = 0;
   int lenHomoI = 0;

   int baseEqBl = 0;
   int minI = 0;
   int diffI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-02: Find if is a valid deletion
   ^   o fun-04 sec-02 sub-01:
   ^     - Find reverse and forward homopolymer length
   ^   o fun-04 sec-02 sub-02:
   ^     - Find the largest homopolymer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-04 Sec-02 Sub-01:
   *   - Find the reverse and foward homopolymer lengths
   \*****************************************************/

   baseStr = &samST->seqCStr[baseOnUI] - 1;
   revHomoI = 1;

   while(revBaseC != (*baseStr & ~(33)))
   {/*Loop: get number of previous bases in homopolymer*/
       ++revHomoI;
       --baseStr;
   }/*Loop: get number of previous bases in homopolymer*/
     /*& ~(33) converts to uppercase and merges U and T*/

   baseStr = &samST->seqCStr[baseOnUI];

   for(
      forHomoI = 1;
      forBaseC == (baseStr[forHomoI] & ~33);
      ++forHomoI
   ){}

   /*****************************************************\
   * Fun-04 Sec-02 Sub-02:
   *   - Find the largest homopolymer
   \*****************************************************/

   /*Not sure if faster, but from my expedrince with
   ` alnSeq I have found that max and min are often
   ` random and faster without ifs
   */

   /*Maximize function (assuming for + rev < max int)*/
   diffI = forHomoI - revHomoI;
   lenHomoI = (diffI & (forHomoI > revHomoI)) + revHomoI;
   minI = (diffI & (forHomoI <= revHomoI)) + revHomoI;

   /*Logic:
   ` lenForHomo - lenRevHomo:
   `   - Gives me the difference between the two lengths,
   `     so that difference + lenRevHomo = lenForHomo
   ` (diff & (forHomo > revHomo) + revHomo
   `   - Becomes diff + revHomo (= forHomo) if the
   `     foward homopolymer is greater
   `   - Becomes revHomo if the foward homopolymer is not
   `     geater
   ` (diff & (forHomo <= revHomo) + revHomo
   `   - Gives the minimum
   */

   /*See if forward and reverse base are the same*/
   baseEqBl = -((int) forBaseC == revBaseC);
   lenHomoI += (baseEqBl & minI);

   /*Logic:
   `  At this point I have the maximum, now I need to
   `    find if I need to combine both directions
   `  - -(int) forBaseC == revBaseC:
   `    o Gives -1 if forBaseC == revBaseC
   `    o Gives 0 if forBaseC != revBaseC
   `  - baseEqBl & minI
   `    o Gives minI if both bases were same (-1)
   `    o Gives 0 if bases were differnt
   `  - lenHomoI + (minI or 0)
   `    o Gives the final value
   */ 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-04 Sec-03:
   ^   - Check if deletion should be kept
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   keepBl =
      minStats->maxHomoDelAry[
         (*samST->seqCStr & ~(1+32+64+128)) >> 1
      ] <= lenHomoI;
       /*& ~(1 + 32 + 64 + 128) clears 1st & last 3 bits
       `     - 1: Merges T and U
       `     - 32: converts to upper
       `     - 64 & 128: Ensures array size is under 32
       `     - >> 1 removes 1st bit (reduces to 16 bytes)
       `  - minStats->maxInsHomoUIntAry has the maximum
       `    size of homopolymer to keep a deletion
       `  - maxHomoSize >= lenHomoI
       `    Goes to 1 if the homopolymer does not go
       `    over the maximum size, else 0.
       */

   /*Record how many deletions were present*/
   samST->numDelUInt += numDelUI;

   samST->numKeptDelUInt +=
      numDelUI & (-((int) keepBl));

   return;
} /*checkDeletions*/

/*-------------------------------------------------------\
| Fun-05: checkSoftMasks
|   - Incurments past soft masked regions
| Input:
|   - numMaskUI:
|     o Number of bases softmasked
|   - baseOnUI:
|     o first base of soft masked region
| Output:
|   - Modifies:
|     o baseOnUI to point to base after the last insertion
\-------------------------------------------------------*/
static void checkSoftMasks(
    uint numMaskUI,
    uint *baseOnUI
){ /*checkSoftMasks*/
    *baseOnUI += numMaskUI;
    return;
} /*checkSoftMask*/

/*-------------------------------------------------------\
| Fun-06: qHistToMed
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
| Fun-07: scoreAln
|   - Scores a single samEntry struct
| Input:
|   - minStats:
|     o Minimum stats to keep a base
|   - samST:
|     o samEntry struct with line from sam file to score
| Output:
|   - Modifies:
|     o samST to have the scores
\-------------------------------------------------------*/
static void scoreAln(
    struct minAlnStats *minStats,
    struct samEntry *samST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-07 TOC: scoreAln
   '  - Scores a single aligment in a sam file
   '  o fun-07 sec-01:
   '    - declare variables
   '  o fun-07 sec-02:
   '    - Check if have a q-score entry
   '  o fun-07 sec-03:
   '    - Process each cigar entry
   '  o fun-07 sec-04:
   '    - Find aligned median & means
   '  o fun-07 sec-05:
   '    - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-01:
   ^   - declare variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar qLineBl = 0;

   char *tmpCigStr = 0;
   char *cigarStr = samST->cigarCStr;

   uint cigEntryUI = 0;
   uint baseOnUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-02:
   ^   - Check if have q-score entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Check if read has a q-score entry*/
   if(samST->qCStr[0] == '*' && samST->qCStr[1] == '\t')
      qLineBl = 0;

   else qLineBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-03:
   ^   - Process each cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(*cigarStr != '\t')
   { /*loop through all entries in the Cigar*/
       cigarStr = base10StrToUI(cigarStr, cigEntryUI);
       tmpCigStr = cigarStr;
       ++cigarStr; /*move to next cigar eqx entry*/

       switch(*tmpCigStr)
       { /*switch, check what kind of cigar entry*/
           case '=':
               checkMatches(
                   cigEntryUI,
                   &baseOnUI,
                   minStats,
                   samST,
                   qLineBl
               ); /*Check if keeping the matches*/
               break;

           case 'X':
               checkSNPs(
                   cigEntryUI,
                   &baseOnUI,
                   minStats,
                   samST,
                   qLineBl
               ); /*Check if keeping the SNPs*/
               break;

           case 'I':
               checkInss(
                   cigEntryUI,
                   &baseOnUI,
                   minStats,
                   samST,
                   qLineBl
               ); /*Check if should keep the insertions*/
               break;

           case 'D':
               checkDels(
                  cigEntryUI,
                  baseOnUI,
                  minStats,
                  samST
               ); /*Check if I am keeping deletions*/
               break;

           case '\t':
               checkMatches(
                   cigEntryUI,
                   &baseOnUI,
                   minStats,
                   samST,
                   qLineBl
               ); /*Check if keeping the matches*/
               break;

           case 'S':  /*was soft mask*/
               checkSoftMasks(cigEntryUI, &baseOnUI);
               break;
       } /*switch, check what kind of cigar entry*/

       /*else is a hard mask (auto removed, can ignore*/
   } /*loop through all entries in the Cigar*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-04:
   ^   - Find aligned median & means
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qLineBl != 0)
   { /*If need to find Q-scores*/
       samST->meanAligQFlt =
           samST->totalAlnQScoreULng /
           ((float) samST->readAligLenUInt);

       samST->medianAligQFlt =
           qHistToMed(
               samST->seqQAlnHistUInt,
               samST->readAligLenUInt
       ); /*Get median Q-score*/
   } /*If need to find Q-scores*/

   return;
} /*scoreAln*/

#endif