/*spdX-License-Identifier: CC0-1.0*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' findCloseBarcodes SOF: Start Of File
'   - has the functions to find the closest barcodes for a
'     spoligotype
'   o .c mac01: setDistIndex_findCloseBarcodes
'     - adds distance & database index into a signed long
'   o .h mac02: distGet_findCloseBarcodes
'     - gets the edit distance from a signed long set with
'       (setDistIndex_findCloseBarcodes)
'   o .h mac03: indexGet_findCloseBarcodes
'     - gets the index from a signed long set with
'       (setDistIndex_findCloseBarcodes)
'   o fun01: barcodeToCode_findCloseBarcode
'     - converts a spoligotype barcode to an unsigned long
'   o .c fun02: sortDistances_findCloseBarcodes
'     - sorts the distance array by closest match to
'       farthest
'   o fun03: getDistances_findCloseBarcodes
'     - get every spoligotype lineage with x distance from
'       the input barcode
'   o fun04: codeToBarcode_findCloseBarcodes
'     - convert a code stored in an unsigned long
'       (returned from barcodeToCode_findCloseBarcodes) to
'       a c-string barcode
'   o fun05: pCloseLineages_findCloseBarcodes
'     - print the detected close lineages found by
'       getDistances_findCloseBarcodes
' This file is released into the Public Domain under
'   CC0-1.0.
' See https://creativecommons.org/publicdomain/zero/1.0/
'   for details (or ../../license-CC-1.0.txt).
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef FIND_CLOSE_BARCODES_FOR_SPOLIGOTYPE_BARCODES_H
#define FIND_CLOSE_BARCODES_FOR_SPOLIGOTYPE_BARCODES_H

struct spolST;

/*-------------------------------------------------------\
| Mac02: distGet_findCloseBarcodes
|   - gets the edit distance from a signed long set with
|     (setDistIndex_findCloseBarcodes)
| Input:
|   - distanceMacOneSL:
|     o signed long with edit distance to get
| Output:
|   - Returns:
|     o signed char with distance
\-------------------------------------------------------*/
#define distGet_findCloseBarcodes(distanceMacOneSL) ( (distanceMacOneSL) & 0xff)

/*-------------------------------------------------------\
| Mac03: indexGet_findCloseBarcodes
|   - gets the index from a signed long set with
|     (setDistIndex_findCloseBarcodes)
| Input:
|   - distanceMacTwoSL:
|     o signed long with index to get
| Output:
|   - Returns:
|     o signed int with index
\-------------------------------------------------------*/
#define indexGet_findCloseBarcodes(distanceMacTwoSL) ( (distanceMacTwoSL) >> 8 )

/*-------------------------------------------------------\
| Fun01: barcodeToCode_findCloseBarcodes
|   - converts a spoligotype barcode to an unsigned long
| Input:
|   - barStr:
|     o c-string with barcode to convert
| Output:
|   - Returns:
|     o unsigned long with barcoded
\-------------------------------------------------------*/
unsigned long
barcodeToCode_findCloseBarcodes(
   signed char *barStr
);

/*-------------------------------------------------------\
| Fun03: getDistances_findCloseBarcodes
|   - get every spoligotype lineage with x distance from
|     the input barcode
| Input:
|   - barStr:
|     o c-string with barcode to convert
|     o valid characters:
|       * spacer persent: I, i, N, n, or 1
|       * spacer missing: any chacter that is not present
|   - maxDistSC:
|     o maximum edit distance to count lineage as close
|   - retLenSIPtr:
|     o signed int pointer to be set to the number of
|       lineages that were close to barStr
|   - spolAryST:
|     o spolST struct array with lineages to compare to
|   - numSpolSI:
|     o number lineages in spolAryST
| Output:
|   - Modifies:
|     o retLenSIPtr to have the number of close lineages
|       * set to -1 for a memory error
|   - Returns:
|     o signed long array with lineage index (in
|       spolAryST) and edit distance
|       * use indexGet_findCloseBarcodes(array[index]) to
|         get the lineage index
|       * use distGet_findCloseBarcodes(array[index]) to
|         get the edit distance from a lineage
|       * length of returned array is set in retLenSIPtr
|     o 0 for no lineages or a memory error
\-------------------------------------------------------*/
signed long *
getDistances_findCloseBarcodes(
   signed char *barStr,      /*barcode to search for*/
   signed char maxDistSC,    /*max distance to keep*/
   signed int *retLenSIPtr,  /*number of hits found*/
   struct spolST *spolAryST, /*spoligotype database*/
   signed int numSpolSI      /*number of spoligotypes*/
);

/*-------------------------------------------------------\
| Fun04: codeToBarcode_findCloseBarcodes
|   - convert a code stored in an unsigned long (returned
|     from barcodeToCode_findCloseBarcodes) to a c-string
|     barcode
| Input:
|   - codeUL:
|     o return value from barcodeToCode_findCloseBarcodes
|       * unsigned long with bits set to 1 or 0 for
|         each spacer
|   - barStr:
|     o c-string with 44 elements to store the barcode in
| Output:
|   - Modifies:
|     o barStr to have 'o' for missing spacers and 'I' for
|       present spacers in codeUL
\-------------------------------------------------------*/
void
codeToBarcode_closetSpol(
   unsigned long codeUL,
   signed char *barStr   /*44 element c-string*/
);

/*-------------------------------------------------------\
| Fun05: pCloseLineages_findCloseBarcodes
|   - print the detected close lineages found by
|     getDistances_findCloseBarcodes
| Input:
|   - barStr:
|     o c-stirng with barcode to print results for
|   - distArySL:
|     o signed long array wit edit distance and lineage
|       distances from getDistances_findCloseBarcodes
|   - distLenSI:
|     o number of distances found
|   - spolAryST:
|     o spolST struct array with lineages that were
|       searched
|   - outFILE:
|     o FILE pointer to file to print output to
| Output:
|   - Prints:
|     o lineages ind distArySL to outFILE
|       * lineage_strain\tlineage\tdistance\tbarcode
|       * first line is a header
|       * for barcode, uppercase characters are matches,
|         lower case characters are missmatches
\-------------------------------------------------------*/
void
pCloseLineages_findCloseBarcodes(
   signed char *barStr,      /*barcode looked for*/
   signed long *distArySL,   /*has distance and index*/
   signed int distLenSI,     /*number of matches*/
   struct spolST *spolAryST, /*spoligotype database*/
   void *outFILE
);

#endif
