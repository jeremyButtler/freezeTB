/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Definitions/defaults and header guards
'   o st-01: amrHit
'     - Holds a linked list of postive amrs for each amr
'       check
'   o fun-01: initAmrHit
'     - Initializes an amrHit structuer to all zeros
'   o fun-02: freeAmrHit
'     - Frees a single amrHit structure. This does not
'       free a list. For lists, use freeAmrHitList; fun-03
'   o fun-03: freeAmrHitList
'     - Frees a list of amrHit structures
'   o fun-04: makeAmr
'     - Makes a new, initialized amrHit structer on heap
'   o fun-05: getCigMutCount (.c only)
'     - Updates the snp (or match)/ins/del counts for a
'   o fun-06: incCigCnt (.c only)
'     - Incurments the cigar counter when all values for
'   o fun-07: isBactStartCodon (.c only)
'      - Checks to is if an input codon is an bacterial
'        start codon
'   o fun-08: checkAmrSam
'     - Checks if a sequence in a sam file entry has
'       amr's (antibiotic resitance)
'   o fun-09: pCrossRes (.c only)
'     - Print out cross resitance
'   o fun-10: pHeadAmrHitList
'     - Prints the header for an amrHitList table
'   o fun-11: pAmrHitList
'     - Prints out all amr's that were in a sequence
'   o fun-12: pHeadAmrs
'     - Prints the header for an amr table (reads instead
'       of consensuses)
'   o fun-13: pAmrs
'     - Prints out all amr's that meant the min depth
'   o fun-14: lookForAmrsSam
'     - Look for anti-microbial (antibiotic) genes in the
'       reads in a sam file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*-------------------------------------------------------\
| Header:
|   - Definitions/defaults and header guards
\-------------------------------------------------------*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

#define defMinDepth 20

#define defMinPerReadsMap 0.45f
    /*At least 45% of reads with region support the amr*/
#define defMinPerReadsTotal 0.00f
    /*0% (turn off) of all kept reads mapped to this amr
    `   region
    */
/*-------------------------------------------------------\
| ST-01: amrHit
|   - Holds a linked list of postive amrs for each amr
|     check
\-------------------------------------------------------*/
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
#define \
initAmrHit(amrHitSTPtr){\
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
#define \
freeAmrHit(amrHitSTPtr){\
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
#define \
freeAmrHitList(amrHitSTListPtr){\
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
| Fun-04: makeAmr
|   - Makes a new, initialized amrHit structer on heap
| Input:
| Output:
|   - Returns:
|     o A pointer to the new amrHit structure
|     o 0 for memory error
\-------------------------------------------------------*/
#define \
makeAmrHit() ({\
   struct amrHit *retST = malloc(sizeof(struct amrHit));\
   if(retST) initAmrHit(retST);\
   retST;\
}) /*makeAmrHit*/

/*-------------------------------------------------------\
| Fun-08: checkAmrSam
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
struct amrHit *
checkAmrSam(
   void *samSTPtr,     /*Sequence to check*/
   void *amrSTAryPtr,  /*Has amr's to check*/
   int numAmrI,        /*Length of amrAryST*/
   int *numHitsI,      /*Number amr hits for seq*/
   char *errC          /*For error reporting*/
);

/*-------------------------------------------------------\
| Fun-10: pHeadAmrHitList
|   - Prints the header for an amrHitList table
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrHitList table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrHitList(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-11: pAmrHitList
|   - Prints out all amr's that were in a sequence
| Input:
|   - seqIdStr:
|     o C-string with name of sequence that had the amr
|   - amrHitSTListPtr:
|     o Pointer to a List of amrHit structures with amr's
|       to print out
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's in amrHitSTListPtr to outFILE
|     o The header if pHeadBl is 1
\-------------------------------------------------------*/
void
pAmrHitList(
   char *seqIdStr,
   struct amrHit *amrHitSTListPtr,
   char *drugAryStr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-12: pHeadAmrs
|   - Prints the header for an amr table (reads instead
|     of consensuses)
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrs(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-13: pAmrs
|   - Prints out all amr's that meant the min depth
| Input:
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally)
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|   - totalReadsUI:
|     o total number of reads input
|   - amrSTAry:
|     o Pointer to an array of amrStruct structures to
|       check and print
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's that meet the min stats and have at
|       least one mapped read
|     o The header if pHeadBl is 1
\-------------------------------------------------------*/
void
pAmrs(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   unsigned int totalReadsUI,
   struct amrStruct *amrSTAry,
   unsigned int numAmrsUI,
   char *drugAryStr,
   void *outFILE
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
|     o 2 for file open errors
|     o 64 for memory errors
\-------------------------------------------------------*/
char
lookForAmrsSam(
   void *amrSTAryPtr, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   char *drugAryStr, /*Has antibiotic names*/
   char readsBl,     /*1: processing reads not cons*/
   unsigned int minDepthUI,  /*Min depth to keep amr*/
   float minPercMapF,/*Min % support to keep amr (read)*/
   float minPercTotalF, /*Min % mapped reads to keep*/
   char *samStr,    /*Sam file with reads to check*/
   char *outStr,
   char *idPrefStr    /*Prefix for id files*/
);

#endif
