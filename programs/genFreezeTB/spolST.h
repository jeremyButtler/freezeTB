/*SPDX-License-Identifier: CC0-1.0*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' spolST SOF: Start Of File
'   - holds the spolST (spoligotype structure) and its
'     supporting functions
'   o header:
'     - guards
'   o .h st01: spolST
'     - holds an single spoligotype lineage
'   o .h st02: fuzzy_spolST
'     - holds all fuzzy spoligotype lineages in a databse
'   o table of contents:
'     *_sof01: spolST general functions
'     *_sof02: spolST barcode and database functions
'     *_sof03: spolST print functions
'     *_sof04: fuzzy spolST general functions
'     *_sof05: fuzzy spolST lineage and database functions
'   *_Sof01:___spolST_general_functions___________________
'     o fun01: blank_spolST
'       - blanks all variables in and spolST structure
'     o fun02: init_spolST
'       - sets all pointers to null and other variables
'        (none) to defaults in an spolST struct
'     o fun03: mkAry_spolST
'       - makes array of initialized spoligotype structers
'     o fun04: freeStack_spolST
'       - frees an spolST struct on the stack
'     o fun05: freeHeap_spolST
'       - frees an spolST struct (on heap)
'     o fun06: freeHeapAry_spolST
'       - frees an array of spolST struct (on heap)
'   *_Sof02:___spolST_barcode_and_database_functions______
'     o fun07: sortAry_spolST
'       - sorts an array of spolST structures by least to
'         greatest with shell sort
'     o fun08: codeToLineage_spolST
'       - finds an spoligo barcode in an array of spolST
'         structures using an binary search
'     o fun09: readDb_spolST
'       - reads in an database of spoligotypes and returns
'         an array of spolST structures sorted by barcode
'     o fun10: depthToBarcode_spolST
'       - converts a read depth array to a barcode
'     o fun11: barcodeToOctal_spolST
'       - convert barcode from depthToBarcode_spolST to an
'         octal
'   *_Sof03:___spolST_print_functions_____________________
'     o fun12: phead_spolST
'       - print out the header for the spoligotype output
'     o fun13: pspol_spolST
'       - print out an spoligotype and matching lineage
'   *_Sof04:___fuzzy_spolST_general_functions_____________
'     o fun14: blank_fuzzy_spolST
'       - blank a fuzzy_spolST struct
'     o fun15: init_fuzzy_spolST
'       - initializes a fuzzy_spolST structure
'     o fun16: freeStack_fuzzy_spolST
'       - frees variables in a fuzzy_spolST structure
'     o fun17: freeHeap_fuzzy_spolST
'       - frees a fuzzy_spolST structure
'     o fun18: memAdd_fuzzy_spolST
'       - check if can add one more item to a fuzzy_spolST
'         struct, if not expand memory
'   *_Sof05:___fuzzy_spolST_lineage_and_database_functions
'     o fun19: fuzzyDbGet_spolST
'       - read in a database for general lineages
'     o fun20: fuzzyLineageSearch_spolST
'       - searches the spoligotype for fuzzy (generalized)
'         lineages
' This file is released into the Public Domain under
'   CC0-1.0.
' See https://creativecommons.org/publicdomain/zero/1.0/
'   for details (or ../../LICENSE).
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef SPOLIGOTYPE_STRUCTURE_H
#define SPOLIGOTYPE_STRUCTURE_H

/*-------------------------------------------------------\
| ST01: spolST
|   - Holds an single spoligotype lineage from an database
\-------------------------------------------------------*/
typedef struct
spolST
{ /*Single spoligotype lineage*/
   signed char *idStr;   /*Strain id entry*/

   unsigned long codeUL; /*barcode; allows faster lookup*/

   signed char *lineageStr; /*Lineage assigned*/
   signed char *sitStr;     /*SIT entry (lineage??)*/
   signed char *countriesStr;/*Contries seen in*/
}spolST; /*spolST*/

/*-------------------------------------------------------\
| ST02: fuzzy_spolST
|   - holds all fuzzy spoligotype lineages in a databse
\-------------------------------------------------------*/
typedef struct
fuzzy_spolST{
   signed char **idAryStr;      /*id for each lineage*/
   signed int lenSI;            /*number of lineages*/
   signed int sizeSI;           /*max lineages*/
   signed char **barAryStr;     /*lineage barcodes*/
     /*
     ` - non-set values in barcodes
     `   * o = no spacer (spacer is missing)
     `   * I = inserted (present) spacer
     `   * x = can be absent or present
     ` for sets: a set is a set of spacers were at least
     `   one spacer must be present (s+p) or absent (d-m).
     `   Each used set must be present before a match is
     `   made
     `   * d = at least one spacer is deleted (set 1)
     `   * a = at least one spacer is absent  (set 2)
     `   * m = at least one spacer is missing (set 3)
     `   * s = at least one spacer is set (set 1)
     `   * f = at least one spacer is found (set 2)
     `   * p = at least one spacer is present (set 3)
     */
}fuzzy_spolST;

#define def_del_spolST 'o'
#define def_set_spolST 'i'
#define def_unkown_spolST 'x'
#define def_delOne_spolST 'd'
#define def_delTwo_spolST 'a'
#define def_delThree_spolST 'm'
#define def_setOne_spolST 's'
#define def_setTwo_spolST 'f'
#define def_setThree_spolST 'p'

/*-------------------------------------------------------\
| Fun01: blank_spolST
|   - blanks all variables in and spolST structure
| Input:
|   - spolSTPtr:
|     o point to an spolST structure to blank
| Output:
|   - Modifies:
|     o all c-strings in spolST to start with null
\-------------------------------------------------------*/
void
blank_spolST(
   struct spolST *spolSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_spolST
|   - sets all pointers to null and other variables
|     (currently none) to defaults in an spolST struct
| Input:
|   - spolSTPtr:
|     o point to an spolST structure to initialize
| Output:
|   - Modifies:
|     o all points to point to null
|     o all non-pointers (none) to be default values
\-------------------------------------------------------*/
void
init_spolST(
   struct spolST *spolSTPtr
);

/*-------------------------------------------------------\
| Fun03: mkAry_spolST
|   - makes an array of initialized spoligotype structers
| Input:
|   - numSpoligosSI:
|     o number of spolST structures to make
| Output:
|   - Returns:
|     o pointer to spolST array
|     o 0 for memory errors
\-------------------------------------------------------*/
struct spolST *
mkAry_spolST(
   signed int numSpoligosST
);

/*-------------------------------------------------------\
| Fun04: freeStack_spolST
|   - frees an spolST struct on the stack
| Input:
|   - spolSTPtr:
|     o pointer to an spolST structure to free
| Output:
|   - Frees:
|     o all variables in spolST and sets to null
\-------------------------------------------------------*/
void
freeStack_spolST(
   struct spolST *spolSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeap_spolST
|   - frees an spolST struct (on heap)
| Input:
|   - spolSTPtr:
|     o pointer to an spolST structure to free
| Output:
|   - Frees:
|     o spoligoST
\-------------------------------------------------------*/
void
freeHeap_spolST(
   struct spolST *spolSTPtr
);

/*-------------------------------------------------------\
| Fun06: freeHeapAry_spolST
|   - frees an array of spolST struct (on heap)
| Input:
|   - spoligoArySTPtr:
|     o pointer to an spolST array structure to free
|   - numSpoligosSI:
|     o number of spolST structers in spoligoArySTPtr
| Output:
|   - Frees:
|     o spoligoArySTPtr and all its elements
\-------------------------------------------------------*/
void
freeHeapAry_spolST(
   struct spolST *spoligoArySTPtr,
   signed int numSpoligosSI
);

/*-------------------------------------------------------\
| Fun07: sortAry_spolST
|  - sorts an array of spolST structures by least to
|    greatest with shell sort
| Input:
|  - spolArySTPtr:
|    o point to an array of spolST structures to sort
|  - startUL:
|    o first element to start sorting at
|  - endUL:
|    o last element to sort (index 0)
| Output:
|  - Modifies:
|    o spoligoArySTPtr array to be sorted form least to
|       greatest
\-------------------------------------------------------*/
void
sortAry_spolST(
   struct spolST *spolArySTPtr,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun08: codeToLineage_spolST
|  - finds an spoligo barcode in an array of spolST
|    structures using an binary search
| Input:
|  - spolSTAryPtr:
|    o pointer to an array of spolST structures to
|      search
|  - codeUL:
|    o barcode to look up
|  - lenAryUI:
|    o number of spoligo type lineages (index 1)
|      (length of spolSTAryPtr)
| Output:
|  - Returns:
|    o nearest index at or after codeUL
|    o (index + 1) * -1 for lineage not found
\-------------------------------------------------------*/
signed long
codeToLineage_spolST(
   struct spolST *spolSTAryPtr,
   unsigned long codeUL,
   unsigned int lenAryUI
);

/*-------------------------------------------------------\
| Fun09: readDb_spolST
|   - reads in an database of spoligotypes and returns
|     an array of spolST structures sorted by barcodes
| Input:
|   - dbFileStr:
|     o c-string with name of database to read in
|   - numSpoligosSIPtr:
|     o pointer to int to hold the the number of
|       spoligotypes in the database
|   - errSC:
|     o pointer to an char to hold any errors
| Output:
|   - Modifies:
|     o numSpoligosSIPtr to hold the number of extracted
|       spoligotypes
|     o errSC to hold the error
|       - 0 for no errors
|       - def_fileErr_tbSpolDefs for file errors
|       - def_memErr_tbSpolDefs for memory errors
|   - Returns:
|     o pointer to an array of spolST structures with
|       the database
|     o 0 for errors
\-------------------------------------------------------*/
struct spolST *
readDb_spolST(
   signed char *dbFileStr,
   signed int *numElmSIPtr,
   signed char *errSC
);

/*-------------------------------------------------------\
| Fun10: depthToBarcode_spolST
|   - converts a read depth array to a barcode
| Input:
|   - barStr:
|     o c-string of 65 bytes to store the barcode
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts,
|       ends with (unsigned int) -1
|   - minDepthUI:
|     o mininum read depth to keep a spacer
|   - minPercDepthF:
|     o mininum percent read depth to keep a spacer
| Output:
|   - Modifies:
|     o barStr to have the barcode
|       * `o` for no barcode
|       * `I` for barcode present
|       * `x` if positon had low depth
\-------------------------------------------------------*/
void
depthToBarcode_spolST(
   signed char *barStr,
   unsigned int *codeAryUI,
   unsigned int minDepthUI,
   float minPercDepthF
);

/*-------------------------------------------------------\
| Fun11: barcodeToOctal_spolST
|   - convert barcode from depthToBarcode_spolST to an
|     octal
| Input:
|   - octalStr:
|     o c-string to get octal number (at least 32 bytes)
|   - barStr:
|     o c-string with barcode to convert to an octal code
|     o barStr from depthToBarcode_spolST()
| Output:
|   - Modifies:
|     o octalStr to have the octal code (x's got to o)
|   - Returns:
|     o unsigned long with index of barcode for quick look
|       up
\-------------------------------------------------------*/
unsigned long
barcodeToOctal_spolST(
   signed char *octalStr,
   signed char *barStr
);

/*-------------------------------------------------------\
| Fun12: phead_spolST
|   - Print out the header for the spoligotype output
| Input:
|   - fragBl:
|     o 1: printing out header for sequence fragments
|          that  not have the entire direct repeat region.
|     o 0: header for sequences with full DR. An consensus
|          with fragments will use this header as well.
|   - outFILE:
|     o FILE pointer with file to print to
| Output:
|   - Prints:
|     o the header to the output file
\-------------------------------------------------------*/
void
phead_spolST(
   signed char fragBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun13: pspol_spolST
|   - print out an spoligotype and matching lineage
| Input:
|   - idStr:
|     o c-string with id/name of sequence
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts
|   - minDepthUI:
|     o mininum read depth to keep a spacer
|   - minPercDepthF:
|     o minimum percent read depth (0 to 1)
|   - fragmentBl:
|     o 1: fragment mode, do not find lineages
|     o 0: complete reads find lineages
|   - numSupUI:
|     o unsigned int with the number of reads that had at
|       least one spacer (for fragment mode)
|   - spoligoAryST:
|     o array of spoligotype lineages to get lineage from
|   - numSpoligosSI:
|     o length of spoligoAryST (index 1)
|   - outFILE:
|     o FILE pointer with file to print to
| Output:
|   - Prints:
|     o spoligotype entry to the outFILE
\-------------------------------------------------------*/
void
pspol_spolST(
   signed char *idStr,
   unsigned int *codeAryUI,
   unsigned int minDepthUI,
   float minPercDepthF,
   signed char fragmentBl,
   unsigned int numSupUI,
   struct spolST *spoligoAryST,
   signed int numSpoligosSI,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun14: blank_fuzzy_spolST
|   - blank a fuzzy_spolST struct
| Input:
|   - fuzzySTPtr:
|     o fuzzy_spolST struct to blank
| Output:
|   - Modifies:
|     o frees all c-strings in idArySTr and barAryStr
|     o sets lenSI to 0
\-------------------------------------------------------*/
void
blank_fuzzy_spolST(
   struct fuzzy_spolST *fuzzySTPtr
);

/*-------------------------------------------------------\
| Fun15: init_fuzzy_spolST
|   - initializes a fuzzy_spolST structure
| Input:
|   - fuzzySTPtr:
|     o fuzzy_spolST struct pointer to initialize
| Output:
|   - Modifies:
|     o sets all values and arrays to 0/null
\-------------------------------------------------------*/
void
init_fuzzy_spolST(
   struct fuzzy_spolST *fuzzySTPtr
);

/*-------------------------------------------------------\
| Fun16: freeStack_fuzzy_spolST
|   - frees variables in a fuzzy_spolST structure
| Input:
|   - fuzzySTPtr:
|     o fuzzy_spolST struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all heap variables and initializes
\-------------------------------------------------------*/
void
freeStack_fuzzy_spolST(
   struct fuzzy_spolST *fuzzySTPtr
);

/*-------------------------------------------------------\
| Fun17: freeHeap_fuzzy_spolST
|   - frees a fuzzy_spolST structure
| Input:
|   - fuzzySTPtr:
|     o fuzzy_spolST struct pointer to free
| Output:
|   - Modifies:
|     o frees fuzzySTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_fuzzy_spolST(
   struct fuzzy_spolST *fuzzySTPtr
);

/*-------------------------------------------------------\
| Fun18: memAdd_fuzzy_spolST
|   - check if can add one more item to a fuzzy_spolST
|     struct, if not expand memory
| Input:
|   - fuzzySTPtr:
|     o fuzzy_spolST struct to check
| Output:
|   - Modifies:
|     o if needed; expands idAryStr in fuzzySTPtr
|     o if needed; expands barAryStr in fuzzySTPtr
|     o if needed; increases sizeSI by 25%
|   - Returns:
|     o 0 for no errors
|     o 1 for a memory error
\-------------------------------------------------------*/
signed char
memAdd_fuzzy_spolST(
   struct fuzzy_spolST *fuzzySTPtr
);

/*-------------------------------------------------------\
| Fun19: fuzzyDbGet_spolST
|   - read in a database for general lineages
| Input:
|   - dbFILE:
|     o FILE pointer to database to read in
|   - errSLPtr:
|     o signed long pointer to get the line of the error
|       or error type
| Output:
|   - Modifies:
|     o errSCPtr:
|       * 0 for no errors
|       * line error was on (> 0) for file error
|       * -1 for empty file
|       * -2 for memory errors
|   - Returns:
|     o fuzzy_spolST array with the fuzzy lineages
|     o 0 for no input, file errors, or memory errors
| dbFILE database format (1st row header, rest lineages):
|  - spaces and tabs separat columns
|  - format
|    lineage	barcode
|    1	Iooooooooooxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
|    2	ooooooooooooooooooooooooooooooooooxxxxxxxxx
|    3	xxxxxxxxxxxxxxxxxxxxxxooooooooooooxxxxxxxxx
|    4	0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxDDDDxxxxxxxxx
|  - possible values used in the barcode
|    * I = always present
|    * o = always absent
|    * x = not used (set or deleted)
|    * D = at least one spacer delete (delete; set 1)
|    * a = at least one spacer delete (absent; set 2)
|    * M = at least one spacer delete (missing; set 3)
|    * S = at least one spacer set (set; set 1)
|    * F = at least one spacer set (found; set 2)
|    * P = at least one spacer set (present; set 3)
\-------------------------------------------------------*/
struct fuzzy_spolST *
fuzzyDbGet_spolST(
   void *dbFILE,
   signed long *errSLPtr
);

/*-------------------------------------------------------\
| Fun20: fuzzyLineageSearch_spolST
|   - searches the spoligotype for fuzzy (generalized)
|     lineages
| Input:
|   - barStr:
|     o c-string with barcode
|     o use depthToBarcode_spolST to build this
|   - fuzzySTPtr:
|     o fuzzy_spolST struct pointer with fuzzy lineages
|       to look for
|   - lenSIPtr:
|     o signed int pointer with number of fuzzy lineages
|       found
| Output:
|   - Modifies:
|     o lenSIPtr:
|       * to have the number of fuzzy lineages found
|       * -1 if barStr did not have 43 spacers
|       * -1 for memory errors
|   - Returns:
|     o index of lineage in 
|     o 0 for no input or memory errors
\-------------------------------------------------------*/
signed int *
fuzzyLineageSearch_spolST(
   signed char *barStr,
   struct fuzzy_spolST *fuzzySTPtr,
   signed int *lenSIPtr
);

#endif
