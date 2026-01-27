/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' amrST SOF: Start Of File
'   - holds functions to support the amrST struct used in
'     tbAmr.
'   o header:
'     - defined variables and guards
'   o .h st01: amrST
'     - holds a single amr mutation + supporting info
'   o .h st02: refList_amrST
'     - has amrST arrays organized by reference
'   * amrST structure general functions
'      o fun01: blank_amrST
'        - sets all non-pointer values in amrSTPtr to 0
'      o fun02: init_AmrST
'        - sets all values/pointers in amrSTPtr to 0
'      o fun03: freeStack_amrST
'        - frees internal variables in an amrST structure
'      o fun04: freeHeap_amrST
'        - frees heap allocated amrST structure
'      o fun05: freeHeapAry_amrST
'        - frees an heap allocated array of amrST structs
'      o .c fun06: swap_amrST
'        - swaps values between two amrST structures
'      o .c fun07: cp_amrST
'        - copies (deep copy) values from one amrST struct
'          to another
'      o fun08: sortPos_amrST
'        - Sort on an amrST array structures by reference
'          coordiante (uses shell sort)
'      o fun09: sortGeneId_amrST
'        - sort amrST struct array by gene names (ids)
'      o fun10: sortVarId_amrST
'        - sort amrST struct array by variant id
'      o fun11: getAmr_amrST
'        - finds nearest amr to input query coordiante
'   * refList_amrST structure general functions
'      o fun12: blank_refList_amrST
'        - blanks a refList_amrST structure
'      o fun13: init_refList_amrST
'        - initializes a refList_amrST structure
'      o fun14: freeStack_refList_amrST
'        - frees variables in a refList_amrST structure
'      o fun15: freeHeap_refList_amrST
'        - frees refList_amrST struct, you must set to 0
'      o .c fun16: swap_refList_amrST
'        - swap to index's ina refList_amrST structure
'      o fun17: refFind_refList_amrST
'        - finds the amrAry assigned to a reference
'      o fun18: getAmrList_refList_amrST
'        - get the amr list for a single reference
'      o fun19: refAdd_refList_amrST
'        - adds a reference to its correct possition in a
'          refList_amrST structure
'      o fun20: amrAdd_refList_amrST
'        - adds a amrST struct to the end of its amr list
'        - this is an unsorted list
'      o fun21: sortPos_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by position
'      o fun22: sortGeneId_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by gene id
'      o fun23: sortVarId_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by variant id
'   * input/output functions
'      o fun24: p_amrST
'        - Print out the amr database used
'      o fun25: readTbl_amrST
'        - gets amr data from a getAmr database
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef AMR_STRUCT_H
#define AMR_STRUCT_H

/*This is the number of longs to hold the resistance
`   flags. Each long holds 64 drugs on a 64 bit system
`   or 32 drugs on an 32 bit system
*/
#define def_maxDrugs_amrST 5
#define def_firstDrugCol_amrST 19
   /*first antibiotic column is 19 columns in*/

/*Reading frame direction*/
#define def_forwardDir_amrST 0
#define def_revCompDir_amrST 1
#define def_unkownDir_amrST 2

/*Whole gene event flags*/
#define def_geneFrameshift_amrST 1
#define def_geneDel_amrST 2

/*This is the flags for each resistance type when printed
`   out
*/
#define def_resFlag_amrST 1
#define def_crossResFlag_amrST 2
#define def_crossToResShift_amrST 1
   /*for converting cross restence to one bit*/
#define def_maxFlags_amrST (sizeof(unsigned long) << 3)

/*Error values*/
#define def_memErr_amrST 64
#define def_fileErr_amrST 2
#define def_missingIndice_amrST 4

/*-------------------------------------------------------\
| ST01: amrST
|   - holds a single amr mutation + supporting info
\-------------------------------------------------------*/
typedef struct amrST{
   unsigned int refPosUI;  /*start position on reference*/
   unsigned int codonPosUI;/*codon start position on ref*/
   unsigned int codonNumUI;/*codon number*/
   unsigned int endCodonNumUI; /*end of an indel*/

   /*this might allow my custom database to be a bit more
   ` usefull to others. It will at least help with mapping
   ` AMR's in my graphAmpDepth.r script
   */
   unsigned int geneFirstRefUI; /*1st ref base in gene*/
   unsigned int geneLastRefUI;  /*last ref base in gene*/

   /*Amino acid sequence stuff*/
   signed char *refAaStr;   /*reference amino acid seq*/
   unsigned int lenRefAaUI; /*length of reference aa seq*/

   signed char *amrAaStr;   /*mutant amino acid seq*/
   unsigned int lenAmrAaUI; /*length of mutant aa seq*/

   signed char frameshiftBl; /*1: is a frame shift*/
   signed char wholeGeneFlagSC;/*1: whole gene event*/
   signed char unknownBl;    /*1: have no idea what is*/
   signed char aaMultiDupBl; /*1: range duplicated*/

   /*general info/ mutation*/
   signed char *geneIdStr;          /*gene name*/
   unsigned int lenGeneIdUI;/*length of gene id*/

   signed char *refSeqStr; /*reference sequence for AMR*/
   unsigned int lenRefSeqUI;/*length of reference seq*/

   signed char *amrSeqStr; /*AMR sequence*/
   unsigned int lenAmrSeqUI;/*length of amr sequence*/

   signed char *varIdStr;  /*variant nomincalture/id*/
   unsigned int lenVarIdUI;

   signed char *effectStr;/*WHO comment; mutation effect*/
   unsigned int lenEffectUI;/*length of effect*/

   signed char *commentStr;/*WHO comment (has epistatic)*/
   unsigned int lenCommentUI; /*length of comment*/

   signed char gradeSC;    /*grade of resitance*/
   signed char mutTypeStr[4]; /*mutation type*/
   signed char dirFlagSC;   /*1: gene reverse, else 0*/
      /*this is marked in the gene_locus column as a c
      ' at the end
      */
   
   /*flags for antibiotic resistance*/
   unsigned long amrFlagsUL[def_maxDrugs_amrST];
   unsigned long crossResFlagsUL[def_maxDrugs_amrST];
      /*5 items should allow for ~ 150 antibiotics on a
      `  32 bit system. ~ 300 on a 64 bit
      */

   unsigned int numSupReadsUI; /*number of read support*/
   unsigned int numMapReadsUI; /*number of reads mapping*/

   /*flags/variables for special who comments*/
   signed char highResBl; /*1: high resitance primary*/
   signed char lowResBl;  /*1: low resitance primary*/
   signed char additiveResBl;/*1:low resitance addivitve*/
   signed char *needsGeneStr; /*gene needed for resitance*/
   unsigned int lenNeedsGeneUI; /*length of needsGeneStr*/
}amrST;

/*-------------------------------------------------------\
| ST02: refList_amrST
|   - has amrST arrays organized by reference
\-------------------------------------------------------*/
typedef struct refList_amrST{
   struct amrST **amrsAryST;/*arrays of AMRs by refrence*/
   signed int *amrLenArySI; /*amr array lengths*/
   signed int *amrSizeArySI; /*amr array sizes*/
   signed char **refAryStr; /*references found*/
   signed int refLenSI;     /*number of references*/
   signed int refSizeSI;    /*number of references*/

   signed char noRefBl;     /*1: means no reference in
                            `    the database
                            */
   signed char *drugAryStr; /*has drugs in database*/
   signed int drugCntSI;    /*number drugs in drugAryStr*/
   signed int drugSizeSI;   /*max drugs array can hold*/
}refList_amrST;

/*-------------------------------------------------------\
| Fun01: blank_amrST
|   - sets all non-pointer values in amrSTPtr to 0
| Input:
|   - ampStructPtr:
|     - pointer to amrST to blank
| Output:
|   - Modifies
|     o all non-pointer values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
blank_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_amrST
|   - sets all values/pointers in amrSTPtr to 0
| Input:
|   - ampSTPtr:
|     - pointer to amrST to initialize
| Output:
|   - Modifies
|     o all values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
init_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_amrST
|   - Frees the geneIdStr, refSeqStr, and amrSeqStr arrays
|     in amrSTPtr and sets all values to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrST to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrSTPtr to be 0
\-------------------------------------------------------*/
void freeStack_amrST(struct amrST *amrSTPtr);

/*-------------------------------------------------------\
| Fun03: freeStack_amrST
|   - frees internal variables in an amrST structure
| Input:
|   - amrSTPtr:
|     - Pointer to amrST to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
freeStack_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_amrST
|   - frees heap allocated amrST structure
| Input:
|   - amrSTPtr:
|     - pointer to amrST to free
| Output:
|   - Frees:
|     o amrSTPtr and the variables inside amrSTPtr
\-------------------------------------------------------*/
void
freeHeap_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeapAry_amrST
|   - frees an heap allocated array of amrST structs
| Input:
|   - amrSTAry:
|     - pointer to start of amrST array to free
| Output:
|   - Frees:
|     o amrSTPtr array (includes internal variables)
\-------------------------------------------------------*/
void
freeHeapAry_amrST(
   struct amrST *amrSTAry, /*array to free*/
   unsigned long numElmUL  /*number of elements*/
);

/*-------------------------------------------------------\
| Fun08: sortPos_amrST
|   - sort an array of amrST structs by reference position
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by start reference position
\-------------------------------------------------------*/
void
sortPos_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI      /*last element (index 0)*/
);

/*-------------------------------------------------------\
| Fun09: sortGeneId_amrST
|   - sort amrST struct array by gene names (ids)
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by starting gene names
\-------------------------------------------------------*/
void
sortGeneId_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI
);

/*-------------------------------------------------------\
| Fun10: sortVarId_amrST
|   - sort amrST struct array by variant id
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by variant ids
\-------------------------------------------------------*/
void
sortVarId_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI
);

/*-------------------------------------------------------\
| Fun11: getAmr_amrST
|  - finds nearest amr to input query coordiante
| Input:
|  - amrAryST:
|    o pointer to an amrST array to search
|  - qryUI:
|    o coordinate (starting) to search for
|  - numGenesUI:
|    o number of amrST's in amrAryST (index 0)
| Output:
|  - Returns:
|    o nearest index at or after qryUI
|    o -1 for no amr genes after qryUI
\-------------------------------------------------------*/
signed int
getAmr_amrST(
   struct amrST *amrAryST,
   unsigned int qryUI,
   signed int numAmrSI
);

/*-------------------------------------------------------\
| Fun12: blank_refList_amrST
|   - blanks a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to blank
| Output:
|   - Modifies:
|     o all values in refList_amrST to be defaults (blank)
|       - this does a lazy blank (length to 0)
\-------------------------------------------------------*/
void
blank_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun13: init_refList_amrST
|   - initializes a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to initialize
| Output:
|   - Modifies:
|     o all values to be 0 (includes pointers)
\-------------------------------------------------------*/
void
init_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun14: freeStack_refList_amrST
|   - frees variables in a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure with variables to free
| Output:
|   - Modifies:
|     o frees amrsAryST and refAryStr and initializes
\-------------------------------------------------------*/
void
freeStack_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun15: freeHeap_refList_amrST
|   - frees a refList_amrST structure, you must set to 0
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to free
| Output:
|   - Modifies:
|     o frees refListSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun17: refFind_refList_amrST
|   - finds the index of amrAry assigned to a reference
| Input:
|   - refStr:
|     o c-string with reference to find
|   - refListSTPtr:
|     o refList_amrST structure to with refernces
| Output:
|   - Returns:
|     o index of the amrST struct with the matching
|       reference
|     o < 0 if no reference matched
|       * returned value is index to insert at times -1
|         + either elements after or greater or at end
\-------------------------------------------------------*/
signed int
refFind_refList_amrST(
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun18: getAmrList_refList_amrST
|   - get the amr list for a single reference
| Input:
|   - refStr:
|     o c-string with reference the amr list is assigned
|       to
|   - amrLenSIPtr:
|     o signed int pointer with the AMRs length
|   - refListSTPtr:
|     o refList_amrST structure with the amr lists
| Output:
|   - Modifies:
|     o amrLenSIPtr to be
|       * < 0 if reference was not found
|       * number of AMRs (>= 0) in the amr array
|   - Returns:
|     o amrST struct array pointer with references AMRs
|     o 0 if reference could not be found or if no AMRs
|       in the list
\-------------------------------------------------------*/
struct amrST *
getAmrList_refList_amrST(
   signed char *refStr,
   signed int *amrLenSIPtr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun19: refAdd_refList_amrST
|   - adds a reference to its correct possition in a
|     refList_amrST structure
| Input:
|   - refStr:
|     o c-string with the reference the amr belongs to
|     o use 0/null for if not using a reference
|   - refListSTPtr:
|     o refList_amrST structure to sort
| Output:
|   - Modifies:
|     o if refStr is not in refAryStr; then inserts
|       refStr into its sorted position in refAryStr
|   - Returns:
|     o index (>=0) of reference
|     o -1 if reference is blank
|     o -2 for memory errors
\-------------------------------------------------------*/
signed int
refAdd_refList_amrST(
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun20: amrAdd_refList_amrST
|   - adds a amrST struct to the end of its amr list
|   - this is an unsorted list
| Input:
|   - amrSTPtr:
|     o amrST struct pointer with settings to transfer
|     o this is a deep copy, so you can resuse the struct
|   - refStr:
|     o c-string with the reference the amr belongs to
|     o use 0/null for if not using a reference
|   - refListSTPtr:
|     o refList_amrST structure to sort
| Output:
|   - Modifies:
|     o amrsAryST in refListSTPtr to have the new AMR
|       * if needed will resize and set amrSizeArySI to
|         the new length
|     o amrLenArySI in refListSTPtr for the AMR list to
|       have one more AMR
|     o refAryStr to have reference (if it is a new ref)
|       * refLenSI and refSizeSI may also be resized
|   - Returns:
|     o 0 for no errors
|     o 1 if amr or reference is blank
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
amrAdd_refList_amrST(
   struct amrST *amrSTPtr,
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun21: sortPos_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortPos_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun22: sortGeneId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortGeneId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun23: sortVarId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortVarId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun21: sortPos_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortPos_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun22: sortGeneId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     gene id
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortGeneId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun23: sortVarId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     variant id
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortVarId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun24: p_amrST
|  - print out amr database used
| Input:
|  - refSTPtr:
|    o pointer to refList_amrST struct with a AMR database
|      to print out
|  - outStr:
|    o c-string with path to output to.
|    o a "-" or null (0) is stdout
| Output:
|  - Prints:
|    o amrSTAry array to outStr as a tsv file
|  - Returns:
|    o 0 for success
|    o def_fileErr_amrST if outStr could not be opend
\-------------------------------------------------------*/
signed char
p_amrST(
   struct refList_amrST *refSTPtr,
   signed char *outStr
);

/*-------------------------------------------------------\
| Fun25: readTbl_amrST
|   - gets amr data from tbAmr tsv file (p_amrST; fun24)
| Input:
|   - tbAmrTblStr:
|     o c-string with path to AMR database/table
|   - errSC:
|     o signed char ponter to hold error message
| Output:
|   - Modifies:
|     o errSC to hold the error message
|       - 0 for no errors
|       - def_fileErr_amrST if could not open tbAmrTblStr
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o refList_amrST struct pointer with the amrs and
|       drugs in the database
|     o 0 for errors; see errSC for specific error
\-------------------------------------------------------*/
struct refList_amrST *
readTbl_amrST(
   signed char *tbAmrTblStr, /*path to tsv with AMR(s)*/
   signed char *errSC        /*holds errors*/
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
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
