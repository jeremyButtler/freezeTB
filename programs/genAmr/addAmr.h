/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' addAmr SOF: Start Of File
'   - has functions to get tbAmr database information from
'     variant ids
'   o header;
'     - type defs, defined variables, and guards
'   o .c fun01: aaToSeq_addAmr
'     - change amino acids to a valid nucleotide sequence
'   o .c fun02: revCmp_addAmr
'     - reverse complement a sequence
'   o .c fun03: addOneAa_addAmr
'     - add single amino acid variant to an amrST struct
'   o .c fun04: parseVarId_addAmr
'     - parse variant id to its correct format
'   o .c fun05: addNtSeq_addAmr
'     - adds nucleotide sequence to AMR
'   o .c fun06: addCodonPos_addAmr
'     - adds amino acid sequences for deletions and
'       large duplications, reading frame orientation
'       (forward/reverse), and the first reference
'       base in the codon to an amrST that has been
'       processed with addNtSeq_addAmr
'   o fun07: var_addAmr
'     - adds a variant to an AMR struct, but does not
'       complete (call addCodonPos_addAmr)
'   o fun08: addDrugs_addAmr:
'     - add anti-microbials (antibiotics) to an AMR
'   o fun09: lineToVar_addAmr
'     - convert line from variant file to tbAmr entry
'   o fun10: getVar_addAmr
'     - get a variant id from a file
'   o license:
'     - licensing for this code (public dofun09 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header;
|   - type defs, defined variables, and guards
\-------------------------------------------------------*/

#ifndef ADD_ANTI_MICROBIAL_RESISTANT_VARIANT_TO_DATABASE_H
#define ADD_ANTI_MICROBIAL_RESISTANT_VARIANT_TO_DATABASE_H

/*some quick default settings*/
#define def_grade_addAmr 2
#define def_highRes_addAmr 0
#define def_lowRes_addAmr 0
#define def_addRes_addAmr 0

#define def_noVar_addAmr 1
#define def_lof_addAmr 2
#define def_noGene_addAmr 3
#define def_del_addAmr 4
#define def_memErr_addAmr 5
#define def_fileErr_addAmr 6
#define def_varErr_addAmr 7
#define def_overflow_addAmr 8
#define def_eof_addAmr 8
#define def_noRef_addAmr 9

struct amrST;
struct seqST;
struct geneCoord;

/*-------------------------------------------------------\
| Fun07: var_addAmr
|   - adds a variant to an AMR struct, but does not
|     complete (call addCodonPos_addAmr)
| Input:
|   - varStr:
|     o variant id to process, should be in form of
|       * "gene_p.refAAnumberAMRAA" (ex gyrB_p.Ser447Phe)
|       * "gene_LoF"
|       * "gene_deletion"
|   - geneStr:
|     o c-string with name of gene to add
|   - coordsSTPtr:
|     o geneCoord struct pointer with gene coordinates
|     o needs to be sorted by gene name
|       * use sortName_geneCoord()
|   - refIdStr:
|     o c-string with reference id
|   - refSeqStr:
|     o c-string with the reference sequence
|   - errSCPtr:
|     o pointer to signed char to get returned error type
| Output:
|   - Modifies:
|     o errSCPtr to have error types:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_noVar_addAmr if variant id is empty
|   - Returns:
|     o pointer to amrST struct with added variant
|     o 0 for memory errors
\-------------------------------------------------------*/
struct amrST *
var_addAmr(
   signed char *varStr,      /*variant to add*/
   signed char *geneStr,     /*name of gene to add*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   signed char *refIdStr,      /*reference id*/
   signed char *refSeqStr,     /*reference sequence*/
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun08: addDrugs_addAmr:
|   - add anti-microbials (antibiotics) to an AMR
| Input:
|   - amrSTPtr:
|     o amrST struct pointer to add drugs to
|   - newDrugStr:
|     o c-string with new drug to add to armSTPtr
|   - crossResBl:
|     o 1: set drug as cross resistant in amrSTPtr
|     o 0: set drug as primrary resistant in amrSTPtr
|   - drugAryStr:
|     o c-string pointer with antiobitics names
|     o this is a an array, so only update with functions
|       in drugAry.c/h
|     o you can set drugAryStr to 0 to have this function
|       allocate memory for you
|       * signed char *drugAryStr = 0;
|       * signed int drugLenSI = 0;
|       * signed int drugSizeSI = 0;
|       * sigend char errorSC =
|            addDrugs_addAmr(
|              amrSTPtr,
|              (signed char *) "PMD",
|              0,
|              &drugAryStr,
|              &drugLenSI,
|              &drugSizeSI
|            );
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
| Output:
|   - Modifies:
|     o amrSTPtr to have the new drug
|     o drugAryStr to have new drug if not already in it
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|   - Returns:
|     o 0 for no errors
|     o def_memErr_addAmr for memory errors
|     o def_overflow_addAmr if went past the maximum drug
|       limit
\-------------------------------------------------------*/
signed char
addDrugs_addAmr(
   struct amrST *amrSTPtr,  /*add drug to*/
   signed char *newDrugStr, /*drug to add*/
   signed char crossResBl,  /*1: is cross resistance*/
   signed char **drugAryStr,/*has drugs already added*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr/*max drugs in drugAryStr*/
);

/*-------------------------------------------------------\
| Fun09: lineToVar_addAmr
|   - convert line from variant file to tbAmr entry
| Input:
|   - lineStr:
|     o c-string with line to convert
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if variant id is empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|       * def_noRef_addAmr if no reference found
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
lineToVar_addAmr(
   signed char *lineStr,         /*has variant id*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr,/*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
);

/*-------------------------------------------------------\
| Fun10: getVar_addAmr
|   - get a variant id from a file
| Input:
|   - varFILE:
|     o FILE pointer with variant line to get
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if blank line or variant id is
|         empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
getVar_addAmr(
   void *varFILE,            /*file with variant ids*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr, /*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
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
: Copyright (c) 2025 jeremyButtler
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
