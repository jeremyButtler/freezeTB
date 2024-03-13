/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

#define defMinDepth 20

#define defMinPerReadsMap 0.45f
    /*At least 45% of reads with region support the amr*/
#define defMinPerReadsTotal 0.00f
    /*0% (turn off) of all kept reads mapped to this amr
    `   region
    */

typedef struct amrHit{
   struct amrStruct *amrST;
   unsigned seqPosUI;
   struct amrHit *nextAmr;
}amrHit;

/*-------------------------------------------------------\
| Fun-01: initAmrHit
|   - Initializes an amrHit structuer to all zeros
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
#define initAmrHit(amrHitSTPtr){\
   (amrHitSTPtr)->amrST = 0;\
   (amrHitSTPtr)->seqPosUI = 0;\
   (amrHitSTPtr)->nextAmr = 0;\
} /*initAmrHit*/

/*-------------------------------------------------------\
| Fun-02: freeAmrHit
|   - Frees a single amrHit structure. This does not free
|     a list. For lists, use freeAmrHitList (fun-03)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
|   - Sets:
|     o amrHitSTPtr to 0
\-------------------------------------------------------*/
#define freeAmrHit(amrHitSTPtr){\
   free((amrHitSTPtr));\
   (amrHitSTPtr) = 0;\
} /*freeAmrHit*/

/*-------------------------------------------------------\
| Fun-03: freeAmrHitList
|   - Frees a list of amrHit structures
| Input:
|   - amrHitSTListPtr:
|     o pointer to start of an amrHist structure list to
|       free
| Output:
|   - Frees:
|     o All amrHits structures in amrHitSTPtr
|   - Sets:
|     o amrHitSTPtr to 0
\-------------------------------------------------------*/
#define freeAmrHitList(amrHitSTListPtr){\
   struct amrHit *tmpST = (amrHitSTListPtr)->nextAmr;\
   \
   while(tmpST != 0)\
   { /*Loop: Free all amrHit structures in list*/\
      freeAmrHit((amrHitSTListPtr));\
      (amrHitSTListPtr) = tmpST;\
      tmpST = (amrHitSTListPtr)->nextAmr;\
   } /*Loop: Free all amrHit structures in list*/\
   \
   freeAmrHit((amrHitSTListPtr));\
} /*freeAmrHit*/

/*-------------------------------------------------------\
| Fun-04:
|   - Makes a new, initialized amrHit structer on heap
| Input:
| Output:
|   - Returns:
|     o A pointer to the new amrHit structure
|     o 0 for memory error
\-------------------------------------------------------*/
#define makeAmrHit() ({\
   struct amrHit *retST = malloc(sizeof(struct amrHit));\
   if(retST) initAmrHit(retST);\
   retST;\
}) /*makeAmrHit*/


/*-------------------------------------------------------\
| Fun-05: checkAmrSam
|   - Checks if a sequence in a sam file entry has
|     amr's (antibiotic resitance)
| Input:
|   - samSTPtr:
|     o Pointer to samEntry struct with sequence to check
|       for amr's
|   - amrSTAryPtr:
|     o Pointer to an array of amrStruct structures with
|       amr's to check for.
|   - numArmI:
|     o Number of amrStruct structures in amrAryST
|   - numHitsI:
|     o Updated to hold the number of amr's found in
|       the sequence stored in samST
|   - errC:
|     o Pointer to character to hold the error output
| Output:
|   - Modifies:
|     o numHitsI to hold the number of amr's found in the
|       sequence in samST
|     o errC:
|       - 0 for No errors
|       - 64 for memory errors
|   - Returns:
|     o List of amrHit structures with the detected amr's
|     o 0 for memory errors or when no amr's were found
\-------------------------------------------------------*/
struct amrHit * checkAmrSam(
   void *samSTPtr,     /*Sequence to check*/
   void *amrSTAryPtr,  /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   int *numHitsI,              /*Number amr hits for seq*/
   char *errC                  /*For error reporting*/
);


/*-------------------------------------------------------\
| Fun-14: lookForAmrsSam
|   - Look for anti-microbial (antibiotic) genes in the
|     reads in a sam file
| Input:
|   - buffStr:
|     o Temporary buffer (c-string) to use when reading in
|       the sam file. This is resized as needed.
|   - lenBuffUL:
|     o Current length of buffStr. This is updated when
|       buffStr is resized
|   - readsBl:
|     o 1: Printing out read stats (use pAmr)
|     o 0: Printing out consensus stats (use pAmrHitList)
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|     o This is applied to read checks only
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally) 
|     o This is applied to read checks only
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|     o This is applied to read checks only
|   - samFILE:
|     o Pointer to sam file to read  from
|   - outFILE:
|     o Pointer to file to print the amr results to
|   - idPrefStr:
|     o Prefix to name the read id file(s). Input 0/null
|       to not print out any ids
| Output:
|   - Prints:
|     o Stats about AMRs to outFILE
|     o Read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o 2 for file open errors (when printing ids)
|     o 64 for memory errors
\-------------------------------------------------------*/
char lookForAmrsSam(
   void *amrSTAryPtr, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   char *drugAryStr, /*Has antibiotic names*/
   char readsBl,     /*1: processing reads not cons*/
   unsigned int minDepthUI,  /*Min depth keep amr (read)*/
   float minPercMapF,/*Min % support to keep amr (read)*/
   float minPercTotalF, /*Min % mapped reads to keep*/
   char *samStr,    /*Sam file with reads to check*/
   char *outStr,
   char *idPrefStr    /*Prefix for id files*/
);

#endif
