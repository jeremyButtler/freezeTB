/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' miruTbl SOF: Start Of File
'   - holds the miruTbl struct and its supporting
'     functions and structrues
'   o header:
'     - forward declartions, defined variables, and guards
'   o .h st01: amp_miruTbl
'     - A single column in a MIRU lineages table
'   o .h st02: miruTbl
'     - Holds a single table for miru primers
'   o fun01: blank_amp_miruTbl
'     - sets non-pointer vars in amp_miruTbl struc to 0/-1
'   o fun02: resetCnt_amp_miruTbl
'     - resets counter variables in an amp_miruTbl struc
'   o fun03: init_amp_miruTbl
'     - initializes a amp_miruTbl structure
'   o fun04: mk_amp_miruTbl
'     - makes initialized amp_miruTbl structure on heap
'   o fun05: freeStack_amp_miruTbl
'     - frees variables in an amp_miruTbl structure
'   o fun06: freeHeap_amp_miruTbl
'     - frees an amp_miruTbl stucture
'   o fun07: blank_miruTbl
'     - blanks a miruTbl structure to default values
'   o fun08: resetCnt_miruTbl
'     - resets all amp_miruTbl counters in an miru table
'   o fun09:
'     - sets all values in miruTbl structure to defaults
'   o fun10: mk_miruTbl
'     - makes and initializes miruTbl struct on the heap
'   o fun11: freeStack_miruTbl
'     - frees arrays inside a miruTbl structure
'   o fun12: freeHeap_miruTbl
'     - frees a miruTbl structure
'   o .h fun13: strIndex_miruTbl
'     - Gets the index of an string in an miruTblST
'       c-string array
'   o .c fun14: sort_amp_miruTbl
'     - sorts lineages in amp_miruTbl struct by amp length
'   o .c fun15: swap_amp_miruTbl
'     - swaps two amp_miruTbl structs in a miruTbl struct
'   o fun16: cmpLin_miruTbl
'     - checks to see if the first lineage comes before
'       second lineage on reference
'   o fun17: sort_miruTbl
'     - Sorts a miruTbl structure for quick look ups. This
'       will sort the primer names by starting positoin
'       and the rows by length, so it is not in a
'       printable format. This calls fun14 in
'       miruTblStruct.c
'   o .c fun18: getAmpIndex_miruTbl
'     - get index of amp_miruTbl struct in input range
'       from an miruTbl struct
'   o fun19: getLinIndex_miruTbl
'      - finds index of counter entry for a MIRU linage of
'        input length (allows for fudging)
'   o fun20: getAmpLen_miruTbl
'     - finds length of MIRU region in a sam file entry
'   o fun21: incLineage_miruTbl
'     - scans reads for MIRU amplicons and incurments
'       counter for every found lineage
'   o .c fun22: unsort_amp_miruTbl
'     - unsorts amp_miruTbl struct to original input order
'   o fun23: unsort_miruTbl
'     - unsorts miruTbl struct to its original order
'   o .c fun24: readLine_miruTbl
'      - reads a line from an miruTbl
'   o fun25: get_miruTbl
'     - read in a MIRU table from a tsv file
'   o fun26: p_miruTbl
'     - prints MIRU table to file; number hits per lineage
'   o fun27: plineages_miruTbl
'     - prints the "best" MIRU lineages for each primer
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declartions, defined variables, and guards
\-------------------------------------------------------*/

#ifndef MIRU_TABLE_STRUCTURE_H
#define MIRU_TABLE_STRUCTURE_H

struct samEntry;

/*This is the number to shift to get to the next string
`   in an array
*/
#define def_lenStr_miruTbl 64

/*-------------------------------------------------------\
| ST01: amp_miruTbl
|   - A single column in a MIRU lineages table
\-------------------------------------------------------*/
typedef struct amp_miruTbl{
   signed char idStr[def_lenStr_miruTbl]; /*primer name*/
   signed int mapReadsSI; /*number reads maped to primer*/

   signed int forPosSI;   /*ref position; forward primer*/
   signed int forLenSI;   /*length of foward primer*/

   signed int revPosSI;   /*ref position; reverse primer*/
   signed int revLenSI;   /*length of the reverse primer*/

   signed int *ampLenArySI; /*amp length for lineages*/
   signed int *hitsArySI;   /*number hits for lineages*/
   signed int numLinSI;     /*defensive (array lengths)*/
   signed int *indexArySI;  /*index for lineages*/
      /*indexArySI is here so I can sort the primers by
      `   length, but still be able find the lineage
      `   they represent later
      */

   /*Number of hits were the lineage could not be found*/
   signed int noLinSI;
}amp_miruTbl;

/*-------------------------------------------------------\
| ST02: miruTbl
|   - Holds all entries in a single table of MIRU primers
\-------------------------------------------------------*/
typedef struct miruTbl{
   signed char *lineageStrAry;/*lineage names*/
   signed int numLinSI;       /*number lineages in table*/

   struct amp_miruTbl *ampAryST; /*array of primers*/
   signed int *indexArySI;     /*original index of table*/

   signed int numPrimSI;       /*number primers in table*/
}miruTbl;

/*-------------------------------------------------------\
| Fun01: blank_amp_miruTbl
|   - sets non-pointer vars in a amp_miruTbl struc to 0/-1
| Input:
|   - ampSTPtr:
|     o pointer to an amp_miruTbl structure to blank
| Output:
|   - Modifies:
|     o ampSTPtr variables: forPosSI, forLenSI,
|       revPosSI, and refLenI to be -1
|     o ampSTPtr arrays: ampLenArySI and
|       indexArySI to have all elements set to -1
|     o amp_miruTblStPtr hitsArySI to have all elements
|       set to 0
\-------------------------------------------------------*/
void
blank_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
);

/*-------------------------------------------------------\
| Fun02: resetCnt_amp_miruTbl
|   - resets counter variables in an amp_miruTbl struc
| Input:
|   - ampSTPtr:
|     o pointer to amp_miruTbl structure to reset
| Output:
|   - Modifies:
|     o all values in hitsArySI in ampSTPtr to be 0
\-------------------------------------------------------*/
void
resetCnt_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
);

/*-------------------------------------------------------\
| Fun03: init_amp_miruTbl
|   - initializes a amp_miruTbl structure
| Input:
|   - ampSTPtr
|     o pointer to an amp_miruTbl structure to initialize
| Output:
|   - Modifies:
|     o ampSTPtr variables: idStr, ampLenArySI,
|       and indexArySI to point to be 0 (null).
|     o ampSTPtr variables: forPosSI, forLenSI,
|       revPosSI, and refLenI to be -1
\-------------------------------------------------------*/
void
init_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
);

/*-------------------------------------------------------\
| Fun04: mk_amp_miruTbl
|   - makes initialized amp_miruTbl structure on heap
| Input:
| Output:
|   - Returns:
|     o pointer to an amp_miruTbl structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct amp_miruTbl *
mk_amp_miruTbl(
);

/*-------------------------------------------------------\
| Fun05: freeStack_amp_miruTbl
|   - frees variables in an amp_miruTbl structure
| Input:
|   - amp_miruTblSTPtr:
|     o pointer to amp_miruTbl struct to free variables in
| Output:
|   - Frees:
|     o amp_miruTblSTPtr variables: idStr, ampLenArySI,
|       indexArySI, and hitsArySI
|   - Modifies:
|     o all values in amp_miruTblSTPtr to be initialized
\-------------------------------------------------------*/
void
freeStack_amp_miruTbl(
   struct amp_miruTbl *amp_miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun06: freeHeap_amp_miruTbl
|   - frees an amp_miruTbl stucture
| Input:
|   - amp_miruTblSTPtrPtr:
|     o pointer to amp_miruTbl structure to free
| Output:
|   - Frees:
|     o amp_miruTblSTPtrPtr
|   - Modifies:
|     o amp_miruTblSTPtrPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeHeap_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
);

/*-------------------------------------------------------\
| Fun07: blank_miruTbl
|   - blanks a miruTbl structure to default values
| Input:
|   - miruTblSTPtr:
|     o pointer to a miruTbl structure to blank
| Output:
|   - Modifies:
|     o all variables to be defaults
\-------------------------------------------------------*/
void
blank_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun08: resetCnt_miruTbl
|   - resets all amp_miruTbl counters in an miru table
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure to reset the hit
|       counters in
| Output:
|   - Modifies:
|     o sets all values to 0 in hitsArySI in all
|       ampSTPtr in all ampAryST variables in
|       miruTblSTPtr.
\-------------------------------------------------------*/
void
resetCnt_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun09:
|   - sets all values in an miruTbl structure to defaults
| Input:
|   - miruTblSTPtr:
|     o pointer to a miruTbl structure to initialize
| Output:
|   - Modifies:
|     o runs Sets all values and pointers to 0
\-------------------------------------------------------*/
void
init_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun10: mk_miruTbl
|   - makes and initializes miruTbl struct on the heap
| Input:
| Output:
|   - Returns:
|     o initialized miruTbl structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct miruTbl *
mk_miruTbl(
);

/*-------------------------------------------------------\
| Fun11: freeStack_miruTbl
|   - frees arrays inside a miruTbl structure
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure to free arrays in
| Output:
|   - Frees:
|     o miruTblSTPtr variabls: ampAryST,
|       lingeageIndexIAry, lineageIndexStrAry
|   - Modifies:
|     o runs init_miruTbl() on all variables
\-------------------------------------------------------*/
void
freeStack_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun12: freeHeap_miruTbl
|   - frees a miruTbl structure
| Input:
|   - miruTblSTPtr:
|     o pointer a miruTbl structure to free
| Output:
|   - Frees:
|     o miruTbl structure in miruTblSTPtr
|   - Modifies:
|     o miruTblSTPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeHeap_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun13: strIndex_miruTbl
|   - gets index of amp_miruTbl struct lineage id
| Input:
|   - indexSI:
|     o integer with index (normal array) of string to get
| Output:
|   - Returns:
|     o starting index of string
\-------------------------------------------------------*/
#define strIndex_miruTbl(indexSI) (def_lenStr_miruTbl * (indexSI))

/*-------------------------------------------------------\
| Fun17: sort_miruTbl
|   - sorts miruTbl struct lineages by amplicon position
| Input:
|   - miruTblSTPtr: 
|     o pointer to miruTbl structure to sort
| Output:
|   - Modifies:
|     o miruTblSTPtr variables: ampLenArySI,
|       hitsArySI, and indexArySI to be sorted by values
|       in ampLenArySI (least to greatest)
|   - Returns:
|     o 0 for no errors
|     o 1 for an array out of bounds error
\-------------------------------------------------------*/
signed char
sort_miruTbl(
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun19: getLinIndex_miruTbl
|   - finds index of counter entry for a MIRU linage of
|     input length (allows for fudging)
| Input:
|   - lenAmpI:
|     o length of the amplicon to compare
|   - fudgeSI:
|     o How much to offset length by when comparing to
|       amplcon lengths
|   - ampIndexSI:
|     o index of amplicon to search
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
| Output:
|   - Returns:
|     o index with first matching length in mirAmpSTPtr
|       - there could be dupicate lengths
|     o -1 if there were no MIRU lineages with the target
|       length for this amplicon
\-------------------------------------------------------*/
signed int
getLinIndex_miruTbl(
   signed int lenAmpI,          /*length of amplicon*/
   signed int fudgeSI,           /*offset length by*/
   signed int ampIndexSI,        /*index of amplicon*/
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun21: incLineage_miruTbl
|   - scans reads for MIRU amplicons and incurments
|     counter for every found lineage
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with amplicon
|   - fudgeI:
|     o ofsset the length by when comparing to lingeage
|       length
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
| Output:
|   - Returns:
|     o 0 for could incurment
|     o 1 if the sam entry is not an lineage
\-------------------------------------------------------*/
signed char
incLineage_miruTbl(
   struct samEntry *samSTPtr, /*read/sequence to search*/
   signed int fudgeI,         /*offset length by*/
   struct miruTbl *miruTblSTPtr
);

/*-------------------------------------------------------\
| Fun23: unsort_miruTbl
|   - unsorts miruTbl struct to its original order
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure to unsort
| Output:
|   - Modifies:
|     o miruTblSTPtr to be unsorted
\-------------------------------------------------------*/
void
unsort_miruTbl(
   struct miruTbl *miruTblSTPtr /*To be unsorted*/
);

/*-------------------------------------------------------\
| Fun25: get_miruTbl
|   - read in a MIRU table from a tsv file
| Input:
|   - miruTblStr:
|     o c-string with path/name of MIRU table to read in
|   - errSCSPtr:
|     o character ponter to store the error message
| Output:
|   - Modifies:
|     o errSCPtr to be 0 for no errors
|     o errSCPtr to be def_fileErr_tbMiruDefs for an file
|       error
|     o errSCPtr to be def_memErr_tbMiruDefs for an memory
|       error
|   - Returns:
|     o An pointer to a miruTbl structure
|     o 0 for an error
\-------------------------------------------------------*/
struct miruTbl *
get_miruTbl(
   signed char *miruTblStr,  /*tsv with path to table*/
   signed char *errSCPtr     /*holds error message*/
);

/*-------------------------------------------------------\
| Fun26: p_miruTbl
|   - prints MIRU table to file; number hits per lineage
| Input:
|   - miruTblSTPtr:
|     o pointer to miruTbl structure to print
|   - outFileStr:
|     o c-string with path of file to print to
|     o null (0) and '-' are treated as stdout
| Output:
|   - Prints:
|     o miruTbl and the number of sequences that
|       support each position to outFileStr.
|   - Returns:
|     o 0 for success
|     o def_fileErr_tbMiruDefs if the file could not be
|       opened
\-------------------------------------------------------*/
signed char
p_miruTbl(
   struct miruTbl *miruTblSTPtr,
   signed char *outFileStr
);

/*-------------------------------------------------------\
| Fun27: plineages_miruTbl
|   - prints the "best" MIRU lineages for each primer
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl struct with lineages
|   - outFileStr:
|     o c-string with path of file to print to.
|     o null (0) and '-' are treated as stdout
| Output:
|   - Prints:
|     o lineage for each primer to outFileStr
|   - Returns:
|     o 0 for success
|     o def_fileErr_tbMiruDefs if the file could not be
|       opened
\-------------------------------------------------------*/
signed char
plineages_miruTbl(
   struct miruTbl *miruTblSTPtr,
   signed char *outFileStr
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
