/*########################################################
# Name: minAlnStatsStruct
# Use:
#   o Holds the minAlnStats struct and its default
#     settings 
# Libraries:
# C Standard Libraries
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o Header:
'     - Indcludes and defined variables
'   o st-01: minAlnStats
'      - Holds the minimum stats for keeping a base while
'        scoring and keeping a read while extracting
'   o fun-01: blankMinStats
'     - Sets the values in a minAlnStats structure to
'       default settings
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Indcludes and defined variables
\-------------------------------------------------------*/

#ifndef SUBSAMPLE_SETTINGS_H
#define SUBSAMPLE_SETTINGS_H

#define defMinPercSNPs 0.07 /*Min % of SNPs to keep read*/
#define defMinPercDiff 1 /*Min % difference to keep read*/
#define defMinPercDels 1 /*Min % deletions to keep read*/
#define defMinPercInss 1 /*Min % insertions to keep read*/
#define defMinPercIndels 1 /*Min % indels to keep read*/

#define defMapq 20         /*Min mapq to keep a read*/
#define defMinBaseQ 7     /*Min Q-score to keep a base*/

/*read to reference mapping quality thresholds*/
#define defMinMedQ 10
#define defMinMeanQ 10
#define defMinAlnMedQ 10
#define defMinAlnMeanQ 10

/*Maximum homopolymer size to keep an insertion*/
#define defMaxInsAHomo 1
#define defMaxInsTHomo 1
#define defMaxInsCHomo 1
#define defMaxInsGHomo 1

/*Maximum homopolymer size to keep an deletion*/
#define defMaxDelAHomo 0
#define defMaxDelTHomo 0
#define defMaxDelCHomo 0
#define defMaxDelGHomo 0

/*Not used in readExtract*/
#define defMinReadLen 200   /*Min length to keep a read*/
#define defMaxReadLen 3000 /*Maximum length to keep read*/

/*-------------------------------------------------------\
| ST-01: minAlnStats
|   - Holds the minimum stats for keeping a base while
|     scoring and keeping a read while extracting
\-------------------------------------------------------*/
typedef struct minAlnStats
{ /*minAlnStats structer*/
   uint minQChar; /*Min Q-score to keep mismatch / indel*/
   uint minMapqUInt;     /*min mapping quality*/
   
   float minMedianQFlt;       /*min median Q-score*/
   float minMeanQFlt;         /*min mean Q-score*/
   float minAlignedMedianQFlt;/*median aligend Q-score*/
   float minAlignedMeanQFlt;  /*min mean aligend Q-score*/

   uint maxReadLenULng;    /*max read length (entire)*/
   uint minReadLenULng;    /*min read length (aligned)*/
   uint maxHomoInsAry[16]; /*limits for insertions*/
   uint maxHomoDelAry[16]; /*limits for deletions*/
       /*A->0, C->1, G->3, T/U->10;
         find with: (baseChar & ~(1+32+64+138)) >> 1*/

    /*not used in scoreReads, but used in readExtract*/
    float minSNPsFlt;
    float minDelsFlt;
    float minInssFlt;
    float minIndelsFlt;
    float minDiffFlt;
}minAlnStats; /*minAlnStats*/

/*-------------------------------------------------------\
| Fun-01: blankMinStats
|   - Sets the values in a minAlnStats structure to
|     default settings
| Input:
|   - minStats:
|     o minStats structure to blank
| Output:
|   - Modifies:
|     o All values in minStats to be defaults
\-------------------------------------------------------*/
static void blankMinStats(struct minAlnStats *minStats)
{ /*blankMinStats*/
    uchar uCharCnt = 0;

    /*Minimum mapping quality to keep a read*/
    minStats->minMapqUInt = defMapq;

    /*Minimum Q-score to keep a base*/
    minStats->minQChar = defMinBaseQ;

    /*Q-score of read*/
    minStats->minMedianQFlt = defMinMedQ;
    minStats->minMeanQFlt = defMinMeanQ;

    /*Q-scores of aligned regions*/
    minStats->minAlignedMedianQFlt = defMinAlnMedQ;
    minStats->minAlignedMeanQFlt = defMinAlnMeanQ;

    /*Variables dealing with read length*/
    minStats->maxReadLenULng = defMaxReadLen; /*max*/
    minStats->minReadLenULng = defMinReadLen; /*min*/
    
    for(uCharCnt = 0; uCharCnt < 16; ++uCharCnt)
    { /*loop: Initialize homopolymer arrays*/
        minStats->maxHomoInsAry[uCharCnt] = 0;
        minStats->maxHomoDelAry[uCharCnt] = 0;
    } /*loop: Initialize homopolymer arrays*/

    /*Settings for choosing when to discard insertions
    ` in homopolymers (by size)
    */
    minStats->maxHomoInsAry[0] = defMaxInsAHomo;
    minStats->maxHomoInsAry[10] = defMaxInsTHomo;
    minStats->maxHomoInsAry[1] = defMaxInsCHomo;
    minStats->maxHomoInsAry[3] = defMaxInsGHomo;

    /*Settings for choosing when to discard deletions
    ` in homopolymers (by size)
    */
    minStats->maxHomoDelAry[0] = defMaxDelAHomo;
    minStats->maxHomoDelAry[10] = defMaxDelTHomo;
    minStats->maxHomoDelAry[1] = defMaxDelCHomo;
    minStats->maxHomoDelAry[3] = defMaxDelGHomo;

    /*Settings for deciding to keep or discard a read*/
    minStats->minSNPsFlt = defMinPercSNPs;
    minStats->minDelsFlt = defMinPercDels;
    minStats->minInssFlt = defMinPercInss;
    minStats->minIndelsFlt = defMinPercIndels;
    minStats->minDiffFlt = defMinPercDiff;

    return;
} /*blankMinStats*/

#endif
