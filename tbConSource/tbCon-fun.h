/*########################################################
# Name: tbCon-fun
#  - Holds functions to make a quick consensus, more like
#    ivar. This is a bit memory intensive,
#    (40 + # insertions * (17) + bases in insertions)
#    bytes 
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Definitions and header guards
'   o st-01: insBase
'     - Structure to hold an list of insetions for a
'       single position
'   o st-02: conBase
'     - Structure to hold an single base in a sequence
'   o fun-01: initInsBase
'     - Initializes an initBase structure
'   o fun-02: makeInsBase
'     - Makes an initializes an initBase structure on the
'   o fun-03: freeInsBase
'     - Frees an heap allocated insBase structure
'   o fun-04: blankConBase
'     - Sets all non-pointer values in a conBase structure
'       to 0
'   o fun-05: initConBase
'     - Sets all values (including pointers) in a conBase
'       structure to 0
'   o fun-06: makeConBase
'     - Makes an initialized, heap allocated conBase
'       structure
'   o fun-07: freeConBaseStack
'     - Frees the heap allocated variables in a conBase
'       structure and also blanks the conBase structure
'   o fun-08: freeConBase
'     - Frees a conBase structure
'   o fun-09: freeConBaseAry
'     - Frees an array of conBase structures (assumes
'       arrya is on heap)
'   o fun-10: initTbConSet
'     - initialize/set tbConSet settings to defaults
'   o fun-11: addReadToConBaseArray
'     - Adds the read into a conBase array
'   o fun-12: collapseConBaseArray
'     - Collapses a conBase array into an array of
'       samEntry structures
'   o fun-13: pConBaseArray
'     - Print all entries in a conBase structure array
'       that are above the minimum read depth
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Defined variables and header guards
\-------------------------------------------------------*/

#ifndef TB_CONSENSUS_FUNCTIONS_H
#define TB_CONSENSUS_FUNCTIONS_H

#define defMaskBase 'N'
#define defMinBaseQ 7
#define defMinInsQ 7
#define defMinMapQ 15 /*Min mapq to keep an read*/

/*Settings for consensus buildiing*/
#define defMinLen 200      /*min length for consensus*/
#define defMinDepth 20     /*Support to keep a position*/
#define defMinPerSnp 0.5f  /*% support to keep an snp*/
#define defMinPerIns 0.7f  /*% support to keep an ins*/
#define defMinPerDel 0.7f  /*% support to keep an del*/

/*Settings for printing*/
#define defMinPrintDepth 20 
#define defMinPrintPerSnp 0.01f
#define defMinPrintPerIns 0.1f
#define defMinPrintPerDel 0.1f

#define defRefLen 5000000 /*~ Length of TB genome*/

#define def_tbCon_Memory_Err 64
#define def_tbCon_File_Err 2
#define def_tbCon_samNoMap 4
#define def_tbCon_samHead 8
#define def_tbCon_samNoSeq 16
#define def_tbCon_samLowMapq 32

/*-------------------------------------------------------\
| ST-01: insBase
|   - Structure to hold an list of insetions for a single
|     position
\-------------------------------------------------------*/
typedef struct insBase{
   char *insStr;/*C-string with insertion*/
   int lenInsI; /*Length of insertion*/
   int numHitsI;/*Number reads supporting this insertion*/

   struct insBase *nextIns; /*Next insertion*/
}insBase;

/*-------------------------------------------------------\
| ST-02: conBase
|   - Structure to hold an single base in a sequence
\-------------------------------------------------------*/
typedef struct conBase{
   int numAI;     /*Number of reads supporting an a*/
   int numTI;     /*Number of reads supporting an t*/
   int numCI;     /*Number of reads supporting an c*/
   int numGI;     /*Number of reads supporting an g*/
   int numDelI;   /*Number reads supporting an deletion*/

   int numInsI;   /*Number reads supporting insertions*/
   struct insBase *insList; /*List of insertions*/

   int totalBasesI; /*Reads supporting position*/
   int totalBasesKeptI; /*Reads above min Q-score*/
}conBase;

/*-------------------------------------------------------\
| ST-03: tbConSet
|   - Structure to hold settings for tbCon
\-------------------------------------------------------*/
typedef struct tbConSet{
   int minQI;   /*Min q-score to keep a base*/
   int minInsQI;/*Min Q-score to keep ins*/
   unsigned char minMapqUC; /*min mapq to keep a read*/

   /*Min percent/depth/length to keep an match/mutation*/
   int minLenI;      /*Min fragment length*/
   int minDepthI;    /*Min depth to keep base*/
   float minPercSnpF;/*Min % support to keep a snp/match*/
   float minPercDelF;/*Min % support to keep a insertion*/
   float minPercInsF;/*Min % support to keep an deletion*/

   /*To mask low quality/support cosnesus bases with*/
   char maskC;/*Character to mask with*/

   /*Min percent/depth to print an snp/match,ins,or del*/
   int printMinDepthI;
   float printMinSupSnpF;
   float printMinSupInsF;
   float printMinSupDelF;
}tbConSet;

/*-------------------------------------------------------\
| Fun-01: initInsBase
|   - Initializes an initBase structure
| Input:
|   - insBaseSTPtr:
|     o A Pointer to an insBase structure to initialize
| Output:
|   - Modifies:
|     o All values in insBase to be 0
\-------------------------------------------------------*/
#define initInsBase(insBaseSTPtr){\
   (insBaseSTPtr)->insStr = 0;\
   (insBaseSTPtr)->lenInsI = 0;\
   (insBaseSTPtr)->numHitsI = 0;\
   (insBaseSTPtr)->nextIns = 0;\
} /*initInsBase*/

/*-------------------------------------------------------\
| Fun-02: makeInsBase
|   - Makes an initializes an initBase structure on the
|     heap
| Input:
| Output:
|   - Returns:
|     o An pointer to an initialized insBase structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct insBase *
makeInsBase(
);

/*-------------------------------------------------------\
| Fun-03: freeInsBase
|   - Frees an heap allocated insBase structure
| Input:
|   - insBaseSTPtr:
|     o A Pointer to an insBase structure to free
| Output:
|   - Frees:
|     o insBaseSTPtr
|   - Sets:
|     o insBaseSTPtr to be 0
|   - Returns:
|     o Next insBase structure (nextIns) in insBase list
\-------------------------------------------------------*/
struct insBase *
freeInsBase(
   struct insBase *insBaseSTPtr
);

/*-------------------------------------------------------\
| Fun-04: blankConBase
|   - Sets all non-pointer values in a conBase structure
|     to 0
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure to blank
| Output:
|   - Modifies:
|     o All non-pointer values in conBaseSTPtr to be 0
\-------------------------------------------------------*/
#define blankConBase(conBaseSTPtr){\
   (conBaseSTPtr)->numAI = 0;\
   (conBaseSTPtr)->numTI = 0;\
   (conBaseSTPtr)->numGI = 0;\
   (conBaseSTPtr)->numCI = 0;\
   (conBaseSTPtr)->numDelI = 0;\
   \
   (conBaseSTPtr)->numInsI = 0;\
   \
   (conBaseSTPtr)->totalBasesI = 0;\
   (conBaseSTPtr)->totalBasesKeptI = 0;\
} /*blankConBase*/

/*-------------------------------------------------------\
| Fun-05: initConBase
|   - Sets all values (including pointers) in a conBase
|     structure to 0
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure to initialize
| Output:
|   - Modifies:
|     o All values in conBaseSTPtr to be 0
\-------------------------------------------------------*/
#define initConBase(conBaseSTPtr){\
   blankConBase((conBaseSTPtr));\
   (conBaseSTPtr)->insList = 0;\
} /*initConBase*/

/*-------------------------------------------------------\
| Fun-06: makeConBase
|   - Makes an initialized, heap allocated conBase
|     structure
| Input:
| Output:
|   - Returns:
|     o A Pointer to a conBase structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct conBase *
makeConBase(
);

/*-------------------------------------------------------\
| Fun-07: freeConBaseStack
|   - Frees the heap allocated variables in a conBase
|     structure and also blanks the conBase structure
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure with variables
|      to free
| Output:
|   - Frees:
|     o Frees the conBaseST->insList 
|   - Sets:
|     o All values in conBaseSTPTr to 0
\-------------------------------------------------------*/
void
freeConBaseStack(
   struct conBase *conBaseSTPtr
);


/*-------------------------------------------------------\
| Fun-08: freeConBase
|   - Frees a conBase structure
| Input:
|   - conBaseSTPtr:
|     o A Pointer to pointer to an conBase structure to
|       free
| Output:
|   - Frees:
|     o Frees conBaseST
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
void
freeConBase(
   struct conBase **conBaseSTPtrPtr
);

/*-------------------------------------------------------\
| Fun-09: freeConBaseAry
|   - Frees an array of conBase structures (assumes heap)
| Input:
|   - conBaseSTPtr:
|     o Pointer to an pointer to the start of an conBase
|       structure array to free
|   - lenAryI:
|     o Number of conBase structurs in the array
| Output:
|   - Frees:
|     o The array in conBaseSTPtr
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
void
freeConBaseAry(
   struct conBase **conBaseSTPtrPtr,
   int lenAryI
);

/*-------------------------------------------------------\
| Fun-10: initTbConSet
|   - initialize/set tbConSet settings to defaults
| Input:
|   - tmpConSetSTPtr:
|     o Pointer to the tbConSet structure with settings
|       for tbCon to set to defaults
| Output:
|   - Modifies:
|     o tbConSetSTPtr to default settings
\-------------------------------------------------------*/
#define initTbConSet(tbConSetSTPtr){\
   (tbConSetSTPtr)->minQI = defMinBaseQ;\
   (tbConSetSTPtr)->minInsQI = defMinInsQ;\
   (tbConSetSTPtr)->minMapqUC = defMinMapQ;\
   \
   /*Min percent/depth/length to keep an match/mutation*/\
   (tbConSetSTPtr)->minLenI = defMinLen;\
   (tbConSetSTPtr)->minDepthI = defMinDepth;\
   (tbConSetSTPtr)->minPercSnpF = defMinPerSnp;\
   (tbConSetSTPtr)->minPercDelF = defMinPerDel;\
   (tbConSetSTPtr)->minPercInsF = defMinPerIns;\
   \
   /*To mask low quality/support cosnesus bases with*/\
   (tbConSetSTPtr)->maskC = defMaskBase;\
   \
   (tbConSetSTPtr)->printMinDepthI = defMinPrintDepth;\
   (tbConSetSTPtr)->printMinSupSnpF = defMinPrintPerSnp;\
   (tbConSetSTPtr)->printMinSupInsF = defMinPrintPerIns;\
   (tbConSetSTPtr)->printMinSupDelF = defMinPrintPerDel;\
} /*initTbConSet*/

/*-------------------------------------------------------\
| Fun-11: addReadToConBaseArray
|   - Adds the read into a conBase array
| Input:
|   - sameEntrySTPtr:
|     o Pointer to a samEntry structure with the read
|       to use
|   - conBaseAry:
|     o Point to a conBase structure array to add the read
|       to.  This is resized if it is to small.
|   - lenRefUI:
|     o Length of the reference sequence (use 0 for
|       unkown), is updated as conBaseSTAry expands.
|   - settings:
|     o Poinnter to a tbConSet structure with the min
|       Q-scores to keep an base or insertion
| Output:
|   - Modifies:
|     o conBaseSTAry to have bases in samSTPtr
|       consensus
|   - Returns:
|     o for no errors
|     o def_tbCon_Memory_Err for memory errors
\-------------------------------------------------------*/
char
addReadToConBaseArray(
   void *samEntrySTPtr,       /*read to add to consensus*/
   struct conBase *conBaseAry[], /*Consensus array*/
   unsigned int *lenRefUI,       /*Length of reference*/
   struct tbConSet *settings
);

/*-------------------------------------------------------\
| Fun-12: collapseConBaseArray
|   - Collapses a conBase array into an array of samEntry
|     structures
| Input:
|   - conBaseAry:
|     o Point to a conBase structure array to collapse
|   - lenConAryUI:
|     o Length of the conBase array
|   - lenSamAryI:
|     o Pointer to integer to hold number of samEntry
|       structures made
|   - minLenI:
|     o Minimum length of a consensus (otherwise pitch)
|   - minDepthI:
|     o Minimum read depth to not mask an snp/match, and
|       not remove an deltion or insertion
|   - minPercSnpF:
|     o Minimum percentage of bases needed to keep an snp
|   - minPerDelF:
|     o Minimum percentage of bases needed to keep (not
|       mask) a deletion
|   - minPerInsF:
|     o Minimum percentage of bases needed to keep an ins.
|       If min depth is present and I have the percenate
|       support for an insertion, but there is not enough
|       support in percentage for a single ins, then mask
|       most supported.
|   - refIdStr:
|     o c-string with referernce sequence name
|   - maskC:
|     o Character to mask with
|   - errC:
|     o Pointer to character to hold error type
| Output:
|   - Modifies:
|     o lenSamST to hold the returned samEntry arrray
|       length
|     o errC set to 0 for no errors
|     o errC set to 1 for nothing meets mean stats
|     o errC set to 64 for memory errors
|   - Returns:
|     o An array of sam entry structures with the
|       consensus. Idealy this would be one structure,
|       but for amplicons it will be an array.
|     o 0 for memory erors
\-------------------------------------------------------*/
void *
collapseConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   unsigned int lenConAryUI,    /*Length of consensus*/
   int *lenSamAryI,          /*Holds output array length*/
   char *refIdStr,             /*Name of reference seq*/
   struct tbConSet *settings,  /*settings for collapsing*/
   char *errC                  /*Error reports*/
);

/*-------------------------------------------------------\
| Fun-13: pConBaseArray
|   - Print all entries in a conBase structure array that
|     are above the minimum read depth
| Input:
|   - conBaseAry:
|     o Point to a conBase structure array to print
|   - lenConAryUI:
|     o Length of the conBase array
|   - minDepthI:
|     o Minimum read depth to print out a position
|   - refIdStr:
|     o c-string with referernce sequence name
|   - outFILE:
|     o C-string with name of file to print everything to
| Output:
|   - Prints:
|     o Entries in conBaseAry to outFILE
|       length
|   - Returns:
|     o 0 for success
|     o def_tbCon_File_Err for file errors
\-------------------------------------------------------*/
char
pConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   unsigned int lenConAryUI,    /*Length of conBaseAry*/
   char *refIdStr,           /*Name of reference seq*/
   struct tbConSet *settings,/*Settings for printing*/
   char *outStr              /*File to print to*/
);

#endif
