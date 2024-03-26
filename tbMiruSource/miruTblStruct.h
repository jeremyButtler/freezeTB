/*#######################################################\
# Name: miruTblStruct
#   - 
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Holds definitions and file guards
'   o st-01: miruAmp
'     - A single column in a MIRU lineages table
'   o st-02: miruTbl
'     - Holds a single table for miru primers
'   o fun-01: blankMiruAmp
'     - Sets the non-pointer values in an miruAmp
'       structure to -1 or 0
'   o fun-02: resetCnt_miruAm
'     - Resets the counter variables in an miruAmp
'       structure
'   o fun-03: initMiruAmp
'     - Initializes a miruAmp structure
'   o fun-04: makeMiruAmp
'     - Makes an initialized miruAmp structure on heap
'   o fun-05: freeMiruAmpStack
'     - Frees all arrays in an mirAmp structure
'   o fun-06: freeMiruAmp
'     - Frees an miruAmp structure
'   o fun-07: blankMiruTbl
'     - Blanks all values in a miruTbl structure. This
'       includes the arrays
'   o fun-08: resetCnt_miruTb
'     - Resets all miruAmp counters in an miru table
'   o fun-09: initMiruTbl
'     - Sets all values in an miruTbl structure to
'       defaults
'   o fun-10: makeMiruTbl
'     - Makes an initialized miruTbl structure on heap
'   o fun-11: freeMiruTblStack
'     - Frees all variables inside an miruTbl structure
'   o fun-12: freeMiruTbl
'     - Frees an miruTbl structure
'   o fun-13: getStrAryIndex
'     - Gets the index of an string in an miruTblST
'       c-string array
'   o fun-14: sortMiruAmp (.c only; called by sortMiruTbl)
'     - Sorts the lineages in an miruAmp structure by
'       the expected length of an amplicon
'   o fun-15: swapMiruAmps (.c only)
'     - Swaps two amplicons in a miruTbl structure
'   o fun-16: cmpLineages (.c only)
'     - Checks to see if the first lineage should come
'       the second (alphabetically)
'   o fun-17: sortMiruTbl
'     - Sorts a miruTbl structure for quick look ups. This
'       will sort the primer names by starting positoin
'       and the rows by length, so it is not in a
'       printable format. This calls fun-11 in
'       miruTblStruct.c
'   o fun-18: get_miruAmpIndex_fromMiruAmpTbl (.c only)
'     - Get the miruAmp structure for an input genome
'       position from an sorted (fun-12; miruTblStruct.h)
'       miruTbl
'   o fun-19: get_linCountIndex_fromMiruAmpTbl (.c only)
'     - Get the first lineage that has the same (fuzzy)
'       length as the input length from an sorted
'       (fun-11; miruTblStruct.c) miruAmp structure
'   o fun-20: findAmpLen (.c only)
'      - Finds the length of the MIRU amplicon for an
'        sequence in a samEntry struct
'   o fun-21: inc_matching_len_lineages
'     - Scans reads for MIRU amplicons and then incurments
'       the counter for ever lineage(s) in read/consensus
'   o fun-22: miruAmp_unsort (.c file only)
'     - unsorts an miruAmp structure back to the original
'       input format (printable/analysis form). 
'   o fun-23: miruTbl_unsort
'     - unsorts an miruTbl structure back to its original
'       (printable form). Calls unsortMiruAmp (fun-16;
'       miruTblStruct.c)
'   o fun-24: readMiruTblLine (.c only)
'     - Reads a line from an miruTbl
'   o fun-25: readMiruTbl
'     - Reads in a ts table of MIRU primers and lineages
'       in the tsv. (primers are columns, lineages rows)
'   o fun-26: pMiruTbl
'     - Prints the MIRU table to a file. This prints
'       the number of hits per primer lineage.
'   o fun-27: pLineages
'     - Prints the "best" MIRU lineages for each primer
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Definitions
\-------------------------------------------------------*/

#ifndef MIRU_TABLE_STRUCTURE_H
#define MIRU_TABLE_STRUCTURE_H

/*This is the number to shift to get to the next string
`   in an array
*/
#define def_lenStr_miruTblST 64
#define def_fudgeLen_miruTblST 15
   /*How much to fudge on the lengths when I am comparing
   `   an amplicon length to a MIRU column
   */

/*Error return values*/
#define def_memErr_miruTblST 64
#define def_fileErr_miruTblST 2
#define def_EOF_miruTblST 1
#define def_primNameInvalid_miruTblST 4

/*-------------------------------------------------------\
| ST-01: miruAmp
|   - A single column in a MIRU lineages table
\-------------------------------------------------------*/
typedef struct miruAmp{
   char idStr[def_lenStr_miruTblST]; /*MIRU primer name*/
   int mapReadsI; /*Number of reads mapping to primer*/

   int forPosI;    /*Reference position; forward primer*/
   int forLenI;    /*Length of the foward primer*/

   int revPosI;    /*Reference position; reverse primer*/
   int revLenI;    /*Length of the reverse primer*/

   int *linLenIAry; /*Amp length for each lineage*/
   int *numHitsIAry;/*Number of hits for each lineage*/
   int numLinI;     /*Defensive coding (array lengths)*/
   int *indexIAry;  /*Index for each lineage miruTbl)*/

      /*indexIAry is here so I can sort the primers by
      `   length, but still be able find the lineage
      `   they represent later
      */
}miruAmp;

/*-------------------------------------------------------\
| ST-02: miruTbl
|   - Holds all entries in a single table of MIRU primers
\-------------------------------------------------------*/
typedef struct miruTbl{
   char *lineageStrAry; /*Names of for lineage*/
   int numLineagesI;/*Number of lineages in table*/

   struct miruAmp *miruAmpSTAry; /*MIRU primers*/
   int *ampIndexIAry; /*Original index of table*/

   int numPrimI;    /*Number of primers in the table*/
}miruTbl;

/*-------------------------------------------------------\
| Fun-01: blankMiruAmp
|   - Sets the non-pointer values in an miruAmp structure
|     to -1 or 0
| Input:
|   - miruAmpSTPtr:
|     o Pointer to an miruAmp structure to blank
| Output:
|   - Modifies:
|     o miruAmpSTPtr variables: forPosI, forLenI,
|       revPosI, and refLenI to be -1
|     o miruAmpSTPtr arrays: linLenIAry and indexIAry
|       to have all elements set to -1
|     o miruAmpStPtr numHitsIAry to have all elements set
|       to 0
\-------------------------------------------------------*/
#define \
blankMiruAmp(\
   miruAmpSTPtr \
){\
   int iLinMac = 0;\
   \
   (miruAmpSTPtr)->forPosI = -1;\
   (miruAmpSTPtr)->forLenI = -1;\
   \
   (miruAmpSTPtr)->revPosI = -1;\
   (miruAmpSTPtr)->revLenI = -1;\
   (miruAmpSTPtr)->mapReadsI = 0;\
   \
   for(\
      iLinMac = 0;\
      iLinMac < (miruAmpSTPtr)->numLinI;\
      ++iLinMac\
   ){ /*Loop: Blank all arrays values*/\
      (miruAmpSTPtr)->linLenIAry[iLinMac] = -1;\
      (miruAmpSTPtr)->numHitsIAry[iLinMac] = 0;\
      (miruAmpSTPtr)->indexIAry[iLinMac] = -1;\
   } /*Loop: Blank all array values*/\
} /*blankMiruAmp*/

/*-------------------------------------------------------\
| Fun-02: resetCnt_miruAmp
|   - Resets the counter variables in an miruAmp structure
| Input:
|   - miruAmpSTPtr:
|     o Pointer to an miruAmp structure to reset the
|       counters in
| Output:
|   - Modifies:
|     o All values in numHitsIAry in miruAmpSTPtr to be
|       set to 0
\-------------------------------------------------------*/
#define \
resetCnt_miruAmp(\
   miruAmpSTPtr\
){\
   int iLinMac = 0; /*Lineage on in MIRU amplicon*/\
   \
   (miruAmpSTPtr)->mapReadsI = 0;\
   \
   for(\
      iLinMac = 0;\
      iLinMac < (miruAmpSTPtr)->numLinI;\
      ++iLinMac\
   ) (miruAmpSTPtr)->numHitsIAry[iLinMac] = 0;\
} /*resetCnt_miruAmp*/
   
/*-------------------------------------------------------\
| Fun-03: initMiruAmp
|   - Initializes a miruAmp structure
| Input:
|   - miruAmpSTPtr
|     o Pointer to an miruAmp structure to initialize
| Output:
|   - Modifies:
|     o miruAmpSTPtr variables: idStr, linLenIAry,
|       and indexIAry to point to be 0 (null).
|     o miruAmpSTPtr variables: forPosI, forLenI,
|       revPosI, and refLenI to be -1
\-------------------------------------------------------*/
#define \
initMiruAmp(\
   miruAmpSTPtr\
){\
   (miruAmpSTPtr)->idStr[0] = '\0';\
   \
   (miruAmpSTPtr)->numLinI = 0;\
      /*Number of lineages in amplicon.*/\
   \
   (miruAmpSTPtr)->linLenIAry = 0;\
   (miruAmpSTPtr)->indexIAry = 0;\
   \
   (miruAmpSTPtr)->numHitsIAry = 0;\
   \
   blankMiruAmp((miruAmpSTPtr));\
} /*initMiryAmp*/

/*-------------------------------------------------------\
| Fun-04: makeMiruAmp
|   - Makes an initialized miruAmp structure on the heap
| Input:
| Output:
|   - Returns:
|     o Pointer to an miruAmp structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct miruAmp *
makeMiruAmp();

/*-------------------------------------------------------\
| Fun-05: freeMiruAmpStack
|   - Frees the variables in an miruAmp structure
| Input:
|   - miruAmpSTPtr:
|     o Pointer to an miruAmp structure to free variables
|       in
| Output:
|   - Frees:
|     o miruAmpSTPtr variables: idStr, linLenIAry,
|       indexIAry, and numHitsIAry
|   - Modifies:
|     o All values in miruAmpSTPtr to be initialized
\-------------------------------------------------------*/
void
freeMiruAmpStack(
   struct miruAmp *miruAmpSTPtr
); /*freeMiruAmpStack*/

/*-------------------------------------------------------\
| Fun-06: freeMiruAmp
|   - Frees an miruAmp stucture
| Input:
|   - miruAmpSTPtrPtr:
|     o Pointer to an miruAmp structure pointer to free
| Output:
|   - Frees:
|     o miruAmpSTPtrPtr
|   - Modifies:
|     o miruAmpSTPtrPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeMiruAmp(
   struct miruAmp **miruAmpSTPtrPtr
); /*freeMiruAmp*/

/*-------------------------------------------------------\
| Fun-07: blankMiruTbl
|   - Blanks a miruTbl structure to default values
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to blank
| Output:
|   - Modifies:
|     o Runs blankMiruAmp on all miruAmp structures in the
|       miruTbl structure
\-------------------------------------------------------*/
#define \
blankMiruTbl(\
   miruTblSTPtr\
){\
   int iAmpMac = 0;\
   int indexMacI = 0;\
   \
   for(\
      iAmpMac = 0;\
      iAmpMac < (miruTblSTPtr)->numPrimI;\
      ++iAmpMac\
   ) blankMiruAmp(\
        &(miruTblSTPtr)->miruAmpSTAry[iAmpMac]\
     );\
   \
   (miruTblSTPtr)->numPrimI = 0;\
   \
   for(\
      iAmpMac = 0;\
      iAmpMac < (miruTblSTPtr)->numLineagesI;\
      ++iAmpMac\
   ){ /*Loop: Set all lineages to null*/\
      indexMacI = getStrAryIndex(iAmpMac);\
      (miruTblSTPtr)->lineageStrAry[indexMacI] = '\0';\
   } /*Loop: Set all lineages to null*/\
   \
   (miruTblSTPtr)->numLineagesI = 0;\
} /*blankMiruTbl*/

/*-------------------------------------------------------\
| Fun-08: resetCnt_miruTbl
|   - Resets all miruAmp counters in an miru table
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to reset the hit
|       counters in
| Output:
|   - Modifies:
|     o Sets all values to 0 in numHitsIAry in all
|       miruAmpSTPtr in all miruAmpSTAry variables in
|       miruTblSTPtr.
\-------------------------------------------------------*/
#define \
resetCnt_miruTbl(\
   miruTblSTPtr\
){\
   int iAmpMac = 0;\
   \
   for(\
      iAmpMac = 0;\
      iAmpMac < (miruTblSTPtr)->numPrimI;\
      ++iAmpMac\
   ) resetCnt_miruAmp(\
        &(miruTblSTPtr)->miruAmpSTAry[iAmpMac]\
     ); /*Blank all the amplicons*/\
} /*resetCnt_miruTbl*/

/*-------------------------------------------------------\
| Fun-09:
|   - Sets all values in an miruTbl structure to defaults
| Input:
|   - miruTblSTPtr:
|     o Pointer to a miruTbl structure to initialize
| Output:
|   - Modifies:
|     o Runs Sets all values and pointers to 0
\-------------------------------------------------------*/
#define initMiruTbl(miruTblSTPtr){\
   (miruTblSTPtr)->lineageStrAry = 0;\
   (miruTblSTPtr)->numLineagesI = 0;\
   \
   (miruTblSTPtr)->miruAmpSTAry = 0;\
   (miruTblSTPtr)->ampIndexIAry = 0;\
   (miruTblSTPtr)->numPrimI = 0;\
   \
   blankMiruTbl((miruTblSTPtr));\
} /*initMiruTbl*/

/*-------------------------------------------------------\
| Fun-10: makeMiruTbl
|   - Makes and initializes a miruTbl structure on the
|     heap
| Input:
| Output:
|   - Returns:
|     o An initialized miruTbl structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct miruTbl *
makeMiruTbl();

/*-------------------------------------------------------\
| Fun-11: freeMiruTblStack
|   - Frees the arrays inside a miruTbl structure
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to free variables
|       in
| Output:
|   - Frees:
|     o miruTblSTPtr variabls: miruAmpSTAry,
|       lingeageIndexIAry, lineageIndexStrAry
|   - Modifies:
|     o Runs initMiruTbl() on all variables
\-------------------------------------------------------*/
void
freeMiruTblStack(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun-12: freeMiruTbl
|   - Frees a miruTbl structure
| Input:
|   - miruTblSTPtrPtr:
|     o Pointer to a pointer to a miruTbl structure to
|       free
| Output:
|   - Frees:
|     o The miruTbl structure in miruTblSTPtrPtr
|   - Modifies:
|     o miruTblSTPtrPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeMiruTbl(
   struct miruTbl **miruTblSTPtrPtr
);

/*-------------------------------------------------------\
| Fun-13: getStrAryIndex
|   - Gets the index of an string in an miruTblST c-string
|     array
| Input:
|   - indexI:
|     o Integer with index of string you want
| Output:
|   - Returns:
|     o Starting index of string
\-------------------------------------------------------*/
#define \
getStrAryIndex(\
   indexI\
)(\
   def_lenStr_miruTblST * indexI\
)

/*-------------------------------------------------------\
| Fun-17: sortMiruTbl
|   - Sorts the lineages in a miruTbl structure be the
|     expected amplicon length
| Input:
|   - miruTblSTPtr: 
|     o A pointer to an miruTblSTPtr structure to sort
| Output:
|   - Modifies:
|     o miruTblSTPtr variables: linLenIAry,
|       numHitsIAry, and indexIAry to be sorted by values
|       in linLenIAry (least to greatest)
|   - Returns:
|     o 0 for no errors
|     o 1 for an array out of bounds error
\-------------------------------------------------------*/
char
sortMiruTbl(
   struct miruTbl *miruTblSTPtr /*Has lineages to sort*/
);

/*-------------------------------------------------------\
| Fun-21: inc_matching_len_lineages
|   - Scans reads for MIRU amplicons and then incurments
|     the counter for ever lineage(s) in read/consensus
| Input:
|   - samSTPtr:
|     o Pointer to an samEntry structure with the amplicon
|       coordinates.
|   - fudgeI:
|     o How much to ofset the length by when comparing to
|       the lengths in the MIRU table (converts lenAmpI
|       into a range)
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
| Output:
|   - Returns:
|     o 0 for could incurment
|     o 1 if the sam entry is not an lineage
\-------------------------------------------------------*/
char
inc_matching_len_lineages(
   void *samSTPtr, /*Sam entry to look at*/
   int fudgeI,     /*Range for amplicons lengths*/
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun-22: miruTbl_unsort
|   - Unsorts a miruTbl structure to its original input
|     order
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to unsort
| Output:
|   - Modifies:
|     o miruTblSTPtr to be unsorted
\-------------------------------------------------------*/
void
miruTbl_unsort(
   struct miruTbl *miruTblSTPtr /*To be unsorted*/
);

/*-------------------------------------------------------\
| Fun-25: readMiruTbl
|   - Read in an MIRU table from a tsv file and store it
|     in an miruTbl structure
| Input:
|   - miruTblStr:
|     o C-string with the path/name of the MIRU amp table
|       to read in
|   - errCPtr:
|     o Character ponter to store the error message
| Output:
|   - Modifies:
|     o errCPtr to be 0 for no errors
|     o errCPtr to be def_fileErr_miruTblST for an file
|       error
|     o errCPtr to be def_memErr_miruTblST for an memory
|       error
|   - Returns:
|     o An pointer to a miruTbl structure
|     o 0 for an error
\-------------------------------------------------------*/
struct miruTbl *
readMiruTbl(
   char *miruTblStr, /*tsv with path to MIRU table read*/
   char *errCPtr     /*Holds error message*/
);

/*-------------------------------------------------------\
| Fun-26: pMiruTbl
|   - Prints the MIRU table to a file. This prints
|     the number of hits per primer lineage.
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to print out
|   - outFileStr:
|     o C-string with path of file to print to. Null (0)
|       and '-' are treated as stdout
| Output:
|   - Prints:
|     o The miruTbl and the number of sequences that
|       support each position to outFileStr.
|   - Returns:
|     o 0 for success
|     o def_fileErr_miruTblST if the file could not be
|       opened
\-------------------------------------------------------*/
char
pMiruTbl(
   struct miruTbl *miruTblSTPtr,
   char *outFileStr
);

/*-------------------------------------------------------\
| Fun-27: pLineages
|   - Prints the "best" MIRU lineages for each primer
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to get lineages
|       from
|   - outFileStr:
|     o C-string with path of file to print to. Null (0)
|       and '-' are treated as stdout
| Output:
|   - Prints:
|     o The lineage for each primer to outFileStr
|   - Returns:
|     o 0 for success
|     o def_fileErr_miruTblST if the file could not be
|       opened
\-------------------------------------------------------*/
char
pLineages(
   struct miruTbl *miruTblSTPtr,
   char *outFileStr
);

#endif
