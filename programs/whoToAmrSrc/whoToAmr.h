/*########################################################
# Name: whoToTbAmr
#   - holds functions to convert 2021/2023 WHO database to
#     tbAmr format.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o .c fun01: amrIsRes_whoToAmr
'     - dectects if an amr is classified as resistant
'   o .c fun02: addOneAa_whoToAmr
'     - add single amino acid variant to an amrST struct
'   o fun03: WHO2021Catalog_whoToAmr
'     - gets amrs from the genome indice tab (as csv) of
'       the WHO TB antibiotic resistance 2021 catalog
'   o c. fun04: crossRes_whoToAmr
'     - check if have cross resitance (2023 catalog)
'   o fun05: WHO2023Catalog_whoToAmr
'     - reads in both tabs of the 2023 WHO TB catalog (as
'       separate tsv's) and extracts AMRs
'   o fun06: parseVarId_whoToAmr
'     - parse variant idea from the WHO 2023 TB catalog
'   o .c fun07: sortCoords_whoToAmr
'     - sorts the gene start, end, and direction arrays by
'       the starting coordiantes
'   o fun08: addCodonPos_whoToAmr
'       - Adds the amino acid sequences for deletions and
'         large duplications, reading frame orientation
'         (forward/reverse) to the, and the first
'         reference base in the codon to an amrST that
'         has been processed with parseVarId_whoToAmr.
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef CONVERT_WHO_TO_TBAMR_DATABASE_H
#define CONVERT_WHO_TO_TBAMR_DATABASE_H

struct amrST;

#define def_noRes_whoToAmr 1

/*-------------------------------------------------------\
| Fun03: WHO2021Catalog_whoToAmr
|   - gets amrs from the genome indice tab (as csv) of the
|     WHO TB antibiotic resistance 2021 catalog
| Input:
|   - whoCsvStr:
|     o c-string with the path to the csv file for the
|       WHO's genome indience tab of the TB antibiotic
|       catalog spread sheet
|   - numArmUL:
|     o modified to hold the number of catalog entriex
|   - drugAryStr:
|     o c-string that will hold the name of each drug
|   - numDrugsSI:
|     o will have the number of druges in drugAryStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - errSC:
|     o pointer to signed char to hold error message
| Output:
|   - Modifies:
|     o numArmUL to hold the number of extracted amr
|       mutations.
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC to hold error message
|       - 0 if no error
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST's (free with freeHeapAry_amrST)
\-------------------------------------------------------*/
struct amrST *
WHO2021Catalog_whoToAmr(
   signed char *whoCsvStr,
   unsigned long *numAmrUL,
   signed char **drugAryStr,
   signed int *numDrugsSI,
   signed char keepNonResBl,   /*1: to keep everything*/
   signed char *errSC
);

/*-------------------------------------------------------\
| Fun05: WHO2023Catalog_whoToAmr
|   - reads in both tabs of the 2023 WHO TB catalog (as
|     separate tsv's) and extracts AMRs
| Input:
|   - whoMasterStr:
|     o c-string with the path to the tsv file with 
|       tab one (master) of the 2023 who TB catalog
|   - whoIndiceFILE:
|     o c-string with the path to the tsv with tab two
|       (genome indices) of the 2023 who catalog
|   - numAmrUL:
|     o modifed to hold the number of kept amr genes
|   - drugStr:
|     o modified to hold the antibiotic names. Each name
|       is at most 31 characters long. To get to the next
|       name add 32.
|   - numDrugsSI:
|     o modfied to hold the number of drugs in drugStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - rmFullGeneVarBl, 
|     o 1: ignore all variants that apply to the full gene
|     o 0: keep all variants
|   - errSC:
|     o holds error type
| Output:
|   - Modifies:
|     o numAmrUL to hold the number of used amrSTs in
|       the returned amrST structure array
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST structures with catalog AMRs
\-------------------------------------------------------*/
struct amrST *
WHO2023Catalog_whoToAmr(
   signed char *whoMasterStr,  /*master tab tsv path*/
   signed char *whoIndiceStr,  /*genome indice tab path*/
   unsigned long *numAmrUL,    /*number of amrs kept*/
   signed char **drugAryStr,   /*holds antibiotic names*/
   signed int *numDrugsSI,     /*number drugs in drugAry*/
   signed char keepNonResBl,   /*1: keep all AMR grades*/
   signed char rmFullGeneVarBl,/*1:no entire gene events*/
   signed char *errSC          /*reports errors*/
);

/*-------------------------------------------------------\
| Fun06: parseVarId_whoToAmr
|   - parse variant idea from the WHO 2023 TB catalog
| Input:
|   - amrST:
|     - pointer to array of amrST structures to update and
|       process variants for
|   - numAmrSI:
|     - number of amrSTs in amrST
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence whe possible. for
|       large deletions/duplications you will need to call
|       addCodonPos_whoToAmr
|   - Returns
|     o 0 for success
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
parseVarId_whoToAmr(
   struct amrST *amrSTPtr, /*has amr variants to update*/
   signed int numAmrSI     /*number of amrs*/
);

/*-------------------------------------------------------\
| Fun08: addCodonPos_whoToAmr
|   - adds amino acid sequences for deletions and
|     large duplications, reading frame orientation
|     (forward/reverse), and the first reference
|     base in the codon to an amrST that has been
|     processed with parseVarId_whoToAmr.
| Input:
|   - amrST:
|     o pointer to an array of amrST structs to update
|   - numAmrSI:
|     o number of amrSTs in amrST
|   - geneTblFileStr:
|     o c-string with the path to the gene coordinates
|       file with gene mappings 
|       - column 3: + for foward gene; - for reverse gene
|       - column 4: frist mapped base in reference
|       - column 5: last mapped base in reference
|   - refFileStr:
|     o c-string with path to fasta file with reference
|       sequence (should only have one sequence)
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence and starting
|       position of codon on the reference
|     o buffStr to be larger if needed
|     o lenBuffUL if buffStr is resized
|     o samFILE to point to the end of the file
|   - Returns
|     o 0 for success
|     o def_fileErr_amrST for an file error
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
addCodonPos_whoToAmr(
   struct amrST *amrSTPtr,     /*amr variants to update*/
   signed int numAmrSI,        /*number of amrs*/
   signed char *geneTblFileStr,/*gene coordiantes path*/
   signed char *refFileStr     /*reference file path*/
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
