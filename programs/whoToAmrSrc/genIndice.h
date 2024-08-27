/*#######################################################\
# Name: geneIndiceStruct
#   - structer and functions for reading the genome
#     indice (tab 2) of the WHO's 2023 TB catalog
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header: 
'     - guards
'   o .h st01: genIndice
'     - Structure to hold a single genome indice from
'       the who 2023 catalog
'   o fun01: blankGeneIndice
'     - blanks a genIndice struct
'   o fun02: init_genIndice
'     - initialize a genIndice
'   o fun03: mk_genIndice
'     - makes a new genIndice struct (on heap)
'   o fun04: freestack_genindice
'     - frees the variables in an geneIndice structure
'   o fun05: freeHeap_geneIndice
'     - frees a single geneIndiceST structure.
'   o fun06: freeHeapAry_genIndice
'     - frees an array of geneIndiceST structures
'   o fun07: swap_genIndice
'     - swaps two genIndice structures in an array
'   o fun08: sort_genIndice
'     - sorts array of genIndice structures by variant ids
'   o fun09: findVariant_genIndice
'     - finds nearest amr to input query coordiante
'   o fun10: read2023Db_genIndice
'     - reads in the genome coordinates and variants from
'       the 2023 WHO TB catalog genome indice tab (saved
'       as a tsv)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header: 
|   - guards
\-------------------------------------------------------*/

#ifndef GENOME_INDICIE_STRUCT_H
#define GENOME_INDICIE_STRUCT_H

#define def_memErr_genIndice 1
#define def_fileErr_genIndice 2

/*-------------------------------------------------------\
| ST01: genIndice
|   - Structure to hold a single genome indice from
|     the who 2023 catalog
\-------------------------------------------------------*/
typedef struct genIndice
{ /*genIndice*/
   unsigned int posUI;

   signed char *varIdStr;
   unsigned int lenVarIdUI;

   signed char *refSeqStr;
   unsigned int lenRefSeqUI;

   signed char *amrSeqStr;
   unsigned int lenAmrSeqUI;
}genIndice;

/*-------------------------------------------------------\
| Fun01: blankGeneIndice
|   - blanks a genIndice struct
| Input:
|   - geneIndiceSTPtr:
|     o pointer to genIndice to blank
| Output:
|   - Modifies:
|     o posUI in geneIndiceSTPtr to be 0
\-------------------------------------------------------*/
void
blank_genIndice(
   struct genIndice *genIndiceSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_genIndice
|   - initialize a genIndice
| Input:
|   - geneIndiceStPtr:
|     o pointer to genIndice to initialize
| Output:
|   - Modifies:
|     o geneIndice to have all values set to 0
\-------------------------------------------------------*/
void
init_genIndice(
   struct genIndice *genIndiceSTPtr
);

/*-------------------------------------------------------\
| Fun03: mk_genIndice
|   - makes a new genIndice struct (on heap)
| Input:
| Output:
|   - Returns:
|     o genIndice on heap
\-------------------------------------------------------*/
struct genIndice *
mk_genIndice(
);

/*-------------------------------------------------------\
| fun04: freestack_genindice
|   - frees the variables in an geneIndice structure
| Input:
|   - geneIndiceST:
|     o pointer to genIndice sturct with varialbes to free
| Output:
|   - Frees:
|     o strings in the geneIndiceST structure
|   - Sets:
|     o everything to 0
\-------------------------------------------------------*/
void
freeStack_genIndice(
   struct genIndice *geneIndiceST
);

/*-------------------------------------------------------\
| Fun05: freeHeap_geneIndice
|   - frees a single geneIndiceST structure.
| Input:
|   - geneIndiceST:
|     o pointer to geneIndiceST structure to free
| Output:
|   - Frees:
|     o geneIndiceST
\-------------------------------------------------------*/
void
freeHeap_geneIndice(
   struct genIndice *geneIndiceST
);

/*-------------------------------------------------------\
| Fun06: freeHeapAry_genIndice
|   - frees an array of geneIndiceST structures
| Input:
|   - genIndiceST:
|     o pointer to genIndiceST structure arrray to free
|  - numElmUI:
|     o number of elements in genIndiceST
| Output:
|   - Frees:
|     o genIndiceST
|   - Sets:
|     o genIndiceST to 0
\-------------------------------------------------------*/
void
freeHeapAry_genIndice(
   struct genIndice *genIndiceST,
   unsigned int numElmUI
);

/*-------------------------------------------------------\
| Fun07: swap_genIndice
|  - swaps two genIndice structures in an array
| Input:
|  - genIndiceAry:
|    o pointer to geneIndice struct array
|  - posOne:
|    o first position (index) to swap
|  - posTwo:
|    o second position (index) to swap
| Output:
|  - Modifies:
|    o swaps first and second positions in genIndicAry
\-------------------------------------------------------*/
void
swap_genIndice(
   struct genIndice *genAry,
   unsigned int posOne,
   unsigned int posTwo
);

/*-------------------------------------------------------\
| Fun08: sort_genIndice
|  - sorts an array of genIndice structures by variant ids
| Input:
|  - genIndiceST:
|    o pointer to genIndice structure array to sort
|  - startUI:
|    o first element sort
|  - endUI:
|    o last element to sort (index 0)
| Output:
|  - Modifies:
|    o arrays in genIndiceST to be sorted by varint id
\-------------------------------------------------------*/
void sort_genIndice(
   struct genIndice *genIndiceST,
   unsigned int startUI,
   unsigned int endUI
);


/*-------------------------------------------------------\
| Fun09: findVariant_genIndice
|  - finds nearest amr to input query coordiante
| Input:
|  - genIndiceAry:
|    o genIndice struct array to find variant in
|  - qryStr:
|    o c-string with variant to search for
|  - lenArySI:
|    o lenth of the genIndice structure array (index 1)
| Output:
|  - Returns:
|    o index of the variant in the array
|    o -1 if the variant is not in the array
\-------------------------------------------------------*/
int findVariant_genIndice(
   struct genIndice *genIndiceAry,
   signed char *qryStr,
   signed int lenArySI
);

/*-------------------------------------------------------\
| Fun10: read2023Db_genIndice
|   - reads in the genome coordinates and variants from
|     the 2023 WHO TB catalog genome indice tab (saved
|     as a tsv)
| Input:
|   - indiceStr:
|     o c-string with the path to the tsv with the
|       genome indice tab (tab 2) of the who 2023 catalog
|   - numIndicesUI:
|     o pointer to unsigned int to hold number of AMRs
|   - errSC:
|     o pointer to signed char to hold error
| Output:
|   - Modifies:
|     o numIndicesUI to have number of extracted genome
|       indices
|     o errSC to have error:
|       - 0 for no errors
|       - def_memErr_genIndice for memory errors
|       - def_fileErr_genIndice for file errors
|   - Returns:
|     o 0 for memory error or file errors
|     o pointer to an array of genIndice structures
\-------------------------------------------------------*/
struct genIndice *
read2023Db_genIndice(
   signed char *indiceStr,
   unsigned int *numIndicesUI,
   signed char *errSC
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
