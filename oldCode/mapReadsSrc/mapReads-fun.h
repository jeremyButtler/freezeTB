/*########################################################
# Name: mapReads-fun
#   - Holds functions for mapping reads to an reference
#     with kmer based methods
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables (?) and guards
'   o .h st-01: kmerTbl
'     - Holds an kmer table for an single sequence
'   o .h fun01: kmerTblBlank
'     - blank the kmer counters (set to -1) in an kmerTbl
'       structure
'   o .h fun02: kmerTblInit
'     - Initialize an kmerTbl structure
'   o fun03: kmerTblFreeStack(
'     - Frees an stack allocated kmerTbl structer
'   o fun04: kmerTblFreeHeap
'     - Frees an heap allocated kmerTbl structer
'   o fun05: kmerTblFreeHeapAry
'     - Frees an heap allocated array of kmerTbl structers
'   o fun06: kmerTblSetSeq
'     - Set the seqeuence for the kmer table
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables (?) and guards
\-------------------------------------------------------*/

#ifndef MAP_READS_FUNCTIONS_H
#define MAP_READS_FUNCTIONS_H

/*-------------------------------------------------------\
| ST01: kmerTbl
|   - Holds an kmer table for an single sequence
\-------------------------------------------------------*/
typedef
kmerTbl
   signed int *tblSI;          /*kmer coordinates table*/
   signed int *indexAryUI;     /*Index of each kmer*/
   unsigned char lenKmerUC;    /*length of one kmer*/
   unsigned int tblLenUI;      /*Length of kmerTbl buff*/
   unsigned int indexAryLenUI; /*Number of total kmers*/
   unsigned int numKmersUI;    /*Number kmers in table*/
}kmerTbl;

/*-------------------------------------------------------\
| ST01: kmerSearch
|   - Holds search array for an kmer join step
\-------------------------------------------------------*/
typedef
kmerSearch
   signed int *searchTblSI;   /*Has kmer patterns*/
   signed int *tblIndxArySI;  /*Index of each kmer*/
   signed int *refPosArySI;   /*ref coords for searchTbl*/
   signed int *kmerIndxArySI; /*Kmer index for tblIndx*/
      /* How these arrays/tables interact
      `    - searchTblSI has the patterns and branches
      `      o Length is length of reference genome
      `      o negative numbers mark branches
      `        - Remove negative flag to get the branch
      `          number
      `    - tblIndxAry has the index of every kmer in
      `      searchTblSI (as blocks)
      `      o Length is length of reference genome
      `    - refPosArySI:
      `      o Has the starting position for each branch
      `        in searchTblSI. This is the index from
      `        tblIndexAry
      `      o Each entry starts with the number of
      `        entries supporting this position and the
      `        coordinates
      `    - kmerIndxAry has the location and length of
      `      every kmer block in tblIndxAry
      `      o Each kmer has two values; the index and
      `        the length (use kmer << 1) to find kmer.
      */
      
   unsigned char lenKmerUC; /*length of one kmer*/
   signed int maxKmersSI;   /*Max number kmers possible*/
   signed int lenRefSI;     /*length of reference; same
                              `   as length of searchTbl
                              `   and tblIndx
                              */
   unsigned int lenKmerIndxUI;/*length of kmerIndxArySI*/
}kmerSearch;

/*-------------------------------------------------------\
| ST02: seqStruct
|   - Forward decleration of seqStruct in
|     ../memwater/seqStruct.h
|   - I am trying to avoid having included files in .h
|     files
\-------------------------------------------------------*/
struct seqStruct;
typedef struct seqStruct seqStuct;

/*-------------------------------------------------------\
| ST03: qyrKmers
|   - Kmer sequence for an single query
\-------------------------------------------------------*/
typedef
qryKmers
   struct seqStruct *seqSTPtr; /*seq struct with query*/
   unsigned int kmerAryUI;     /*Kmers in the query*/
   unsigned int kmerLenUI;     /*Length of kmerAryUI*/

   signed int *indexAryUI;     /*Index of each kmer*/
   unsigned int indexAryLenUI; /*Number of total kmers*/

   unsigned char lenKmerUC;    /*length of one kmer*/
}qryKmer;

/*-------------------------------------------------------\
| Fun01: kmerTblBlank
|   - blank the kmer counters (set to -1) in an kmerTbl
|     strucuter
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure to blank
| Output:
|   - Modifies:
|     o kmerTblSTPtr->tblSI to be full of -1's
|     o kmerTblSTPtr->indexAryUI to be full of zeros
|     o kmerTblSTPtr->numKmersUI be 0
\-------------------------------------------------------*/
#define \
kmerTblBlank(\
   kmerTblSTPtr, /*kmerTbl struct to blank*/
){\
   unsigned int uiKmerMac = 0;\
   \
   if((kmerTblSTPtr))\
      (kmerTblSTPtr)->numKmersUI = 0; \
   \
   if((kmerTblSTPtr) && (kmerTbSTPtr)->tblSI)\
   { /*If: I have kmer lengths to blank*/\
      while(uiKmerMac < (kmerTblSTPtr)->indexAryLenUI) \
      { /*Loop: blank all index values*/ \
         (kmerTblSTPtr)->indexAryUI[uiKmerMac] = 0; \
         ++uiKmerMac; \
      } /*Loop: blank all index values*/ \
      \
      uiKmerMac = 0; \
      \
      while(uiKmerMac <  (kmerTblSTPtr)->tblLenUI)\
      { /*Loop: Set all kmer counter to negative one*/\
         (kmerTblSTPtr)->tblSI[uiKmerMac] = -1;\
         ++uiKmerMac;
      } /*Loop: Set all kmer counters to negative one*/\
   } /*If: I have kmer lengths to blank*/\
} /*kmerTblBlank*/

/*-------------------------------------------------------\
| Fun02: kmerTblInit
|   - initialize an kmerTbl structure
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure to initialize
| Output:
|   - Modifies:
|     o kmerTblSTPtr to have all values set to 0/null
\-------------------------------------------------------*/
#define \
kmerTblInit( \
   struct kmerTbl *kmerTblSTPtr \
){ \
   if(kmerTblSTPtr) \
   { /*If: The user provided an kmer table*/ \
       kmerTblSTPtr->lenKmerUC = 0; \
       kmerTblSTPtr->indexAryLenUI = 0; \
       kmerTblSTPtr->kmerIndexAryUI = 0; \
       kmerTblSTPtr->tblLenUI = 0; \
       kmerTblSTPtr->tlbUI = 0; \
       \
       kmerBlBlank(kmerTblSTPtr);/*for future additions*/\
   } /*If: The user provided an kmer table*/ \
} /*kmerTblInit*/


/*-------------------------------------------------------\
| Fun03: kmerTblFreeStack
|   - Frees an stack allocated kmerTbl structer
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure free variables from
| Output:
|   - frees:
|     o kmerUITbl and numKmerUIAry pointers in
|       kmerTblSTPtr
|   - sets:
|     o kmerUITbl and numKmerUIAry pointers to 0 (null)
|       kmerTblSTPtr
|   - Modifies:
|     o tblLenUI and lenMkerUC in kmerTblSTPtr to be 0
\-------------------------------------------------------*/
void
kmerTblFreeStack(
   struct kmerTbl *kmerTblSTPtr
);

/*-------------------------------------------------------\
| Fun04: kmerTblFreeHeap
|   - Frees an heap allocated kmerTbl structer
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure free
| Output:
|   - frees:
|     o kmerTblSTPtr and its associated variables
\-------------------------------------------------------*/
void
kmerTblFreeHeap(
   struct kmerTbl *kmerTblSTPtr
);

/*-------------------------------------------------------\
| Fun05: kmerTblFreeAryHeap
|   - frees an heap allocated array of kmerTbl structers
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure array to free
|   - numTblsUI:
|     o Number of kmerTbl structures in array
| Output:
|   - frees:
|     o All kmerTbl structures in kmerTblSTPtr
|     o kmerTblSTPtr
\-------------------------------------------------------*/
void
kmerTblFreeAryHeap(
   struct kmerTbl *kmerTblSTPtr
   unsigned int numTblsUI
);

/*-------------------------------------------------------\
| Fun06: kmerTblSetSeq
|   - set the seqeuence for the kmer table
| Input:
|   - kmerTblSTPtr:
|     o kermTbl structure to add the sequence to
|   - lenKmerUC:
|     o number of bases in an kmer
|   - seqSTVoidPtr:
|     o pointer to an sequence structure
|       (st-01 ../memwater/seqStruct.h) to add to the kmer
|       table
| Output:
|   - Modifies:
|     o kmerTblSTPtr to have the initialize kmer table
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapReads for memory errors
\-------------------------------------------------------*/
signed char
kmerTblInit(
   struct kmerTbl *kmerTblSTPtr,
   unsigned char lenKmerUC,
   void *seqSTVoidPtr
);

/*-------------------------------------------------------\
| Fun07: kmerSearch_getTblIndx
|   - Gets the kmer index and block size for tblIndxArySI
|     in an kmerSearch structure.
| Input:
|   - kmerSI:
|     o int with kmer to look up
|   - indxSI:
|     o int to hold index of first kmer in kmer block
|   - lenKmerBlockSI:
|     o int to hold length of kmer block in tblIndexArySI
|   - kmerSearchSTPtr:
|     o Pointer to an kmerSearch structure to get the
|       kmer index from
| Output:
|   - Modifies:
|     o indxSI to hold index of first matching kmer in
|       tblIndxArySI
|     o lenKmerBlockSI to hold number of times the kmer
|       is repeated (block size)
\-------------------------------------------------------*/
#define \
kmerSearch_getTblIndx(
   kmerSI,
   indxSI,
   lenKmerBlockSI,
   kmerSearchSTPtr
){ \
   unsinged int posMacUI = (kmerSI) << 1;
   \
   (indexSI) = kmerSearchSTPtr->kmerIndxArySI[posMacUI]; \
   \
   (lenKmerBlockSI) = \
      kmerSearchSTPtr->kmerIndxArySI[posMacUI]; \
} /*kmerSearch_getTblIndx*/

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
