/*#########################################################
# Name: mapReads-fun
#   - Holds functions for mapping reads to an reference
#     with kmer based methods
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o .h st-01: kmerTbl
'     - Holds an kmer table for an single sequence
'   o .h fun-01: kmerTblBlank
'     - blank the kmer counters (set to -1) in an kmerTbl
'       structure
'   o .h fun-02: kmerTblInit
'     - Initialize an kmerTbl structure
'   o fun-03: kmerTblFreeStack(
'     - Frees an stack allocated kmerTbl structer
'   o fun-04: kmerTblFreeHeap
'     - Frees an heap allocated kmerTbl structer
'   o fun-05: kmerTblFreeHeapAry
'     - Frees an heap allocated array of kmerTbl structers
'   o fun-06: kmerTblSetSeq
'     - Set the seqeuence for the kmer table
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "mapReads-err.h"

#include <stdio.h>

#include "../memwater/seqStruct.h"

/*No .c files (only .h)*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/codonTbl.h"
#include "../generalLib/gen-shellSort.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .h ../generalLib/ulCpStr.h
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-03: kmerTblFreeStack
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
){
    uint uiKmer = 0;

    if(! kmerTblSTPtr)
       return; /*Null pointer, nothing to do*/

    free((kmerTblSTPtr->tblSI));
    kmerTblSTPtr->kmerUITbl = 0;

    free(kmerTblSTPtr->kmerIndexAryUI);
    kmerTblSTPtr->numKmerUIAry = 0;

    kmerTblSTPtr->lenKmerUC = 0;
    kmerTblSTPtr->tblLenUI = 0;
    kmerTblSTPtr->indexAryLenUI = 0;
} /*kmerTblFreeStack*/

/*-------------------------------------------------------\
| Fun-04: kmerTblFreeHeap
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
){
   kmerTblFreeStack(kmerTblSTPtr);
   free(kmerTblSTPtr);
   kmerTblSTPtr = 0;
} /*kmerTblFreeHeap*/

/*-------------------------------------------------------\
| Fun-05: kmerTblFreeAryHeap
|   - Frees an heap allocated array of kmerTbl structers
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
   struct kmerTbl *kmerTblSTPtr,
   unsigned int numTblsUI
){
   uint uiTbl = 0;

   for(uiTbl = 0; uiTbl < numTblsUI; ++uiTbl)
      kmerTblFreeStack(&kmerTblSTPtr[uiTbl]);

   free(kmerTblSTPtr);
   kmerTblSTPtr = 0;
} /*kmerTblFreeHeap*/

/*-------------------------------------------------------\
| Fun-06: kmerTblSetSeq
|   - Set the seqeuence for the kmer table
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-06 TOC:
   '   - Set the sequence in an kmer table
   '   o fun-06 sec-01:
   '     - Variable declerations
   '   o fun-06 sec-02:
   '     - Allocate memory
   '   o fun-06 sec-03:
   '     - Find the number of kmers in the sequence
   '   o fun-06 sec-04:
   '     - Find the index of each kmer
   '   o fun-06 sec-05:
   '     - Add kmers to table
   '   o fun-06 sec-06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiKmer = 0;
   uint uiBase = 0;
   uchar errUC = 0;
   uint kmerUI = 0;

   sint *cntAryHeapSI = 0; /*Number of kmers in index*/

   uchar baseCodeUC = 0; /*Code of base*/
   uchar kmerBaseUC = 0; /*Base on in kmer*/

   struct seqStruct *seqSTPtr= (seqStruct *) seqSTVoidPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-02:
   ^   - Allocate memory
   ^   o fun-06 sec-02 sub-01:
   ^     - Find the number of kmers in the table
   ^   o fun-06 sec-02 sub-02:
   ^     - Kmer index (for table) allocation
   ^   o fun-06 sec-02 sub-03:
   ^     - Kmer table memory allocation
   ^   o fun-06 sec-02 sub-04:
   ^     - Temporary index memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-06 Sec-02 Sub-01:
   *   - Find the number of kmers in the table
   \*****************************************************/

   if(! kmerTblSTPtr)
      return 0;   /*No kmer table input*/

   kmerTblSTPtr->lenKmerUC = lenKmerUC;

   /*Find the number of kmers in the table*/
   kmerUI = 1;
   for(uiKmer = 0; uiKmer < lenKmerUC; ++uiKmer)
      kmerUI << 2;

   /*****************************************************\
   * Fun-06 Sec-02 Sub-02:
   *   - Kmer index (for table) allocation
   \*****************************************************/

   if(! kmerTblSTPtr->kmerIndexAryUI)
   { /*If: I do not have an index entry*/
      kmerTblSTPtr->indexAryLenUI = kmerUI;

      kmerTblSTPtr->kmerIndexAryUI =
         calloc(
            kmerUI,
            sizeof(uint)
         );

      if(! kmerTblSTPtr->kmerIndexAryUI)
         goto memErr_fun06_sec06_sub02;
   } /*If: I do not have an index entry*/

   else
   { /*Else: The user provided an index entry*/
      if(kmerTblSTPtr->indexAryLenUI < kmerUI)
      { /*If: I need to resize the index array*/
         free(kmerTblSTPtr->indexAryLenUI);
         kmerTblSTPtr->indexAryLenUI = 0;
         kmerTblSTPtr->indexAryLenUI = kmerUI;

         kmerTblSTPtr->kmerIndexAryUI =
            calloc(
               kmerUI,
               sizeof(uint)
            );

         if(! kmerTblSTPtr->kmerIndexAryUI)
            goto memErr_fun06_sec06_sub02;
      } /*If: I need to resize the index array*/

      else
      { /*Else: I need to blank the index array*/
         for(uiKmer = 0; uiKmer < kmerUI; ++uiKmer)
            kmerTblSTPtr->kmerIndexAryUI = 0;
      } /*Else: I need to blank the index array*/
   } /*Else: The user provided an index entry*/

   /*****************************************************\
   * Fun-06 Sec-02 Sub-03:
   *   - Kmer table memory allocation
   \*****************************************************/

   kmerTblSTPtr->tblLenUI = seqSTPtr->lenSeqUL -lenKmerUC;

   kmerTblSTPtr->tlbUI =
      malloc(kmerTblSTPtr->tblLenUI * sizeof(sint));

   if(! kmerTblSTPtr->numKmerUIAry)
      goto memErr_fun06_sec06_sub02;

   /*****************************************************\
   * Fun-06 Sec-02 Sub-04:
   *   - Temporary index memory allocation
   \*****************************************************/

   cntAryHeapSI =
      calloc(
         kmerTblSTPtr->indexAryLenUI,
         sizeof(uint)
      );

   if(! cntAryHeapSI)
      goto memErr_fun06_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-03:
   ^   - Find the number of kmers in the sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   kmerUI = 0;
   kmerBaseUC = 0;
   uiBase = 0;

   while(uiBase < seqSTPtr->lenSeqUL)
   { /*Loop: Find the number of kmers*/
      while(kmerBaseUC < lenKmerUC)
      { /*Loop: Fill out the kmer*/
         if(uiBase >= seqSTPtr0>lenSeqUL)
            break;

         baseCodUC =
            baseToCodeLkTbl[seqSTPtr->seqStr[uiBase]];

         kmerUI <<= 1;
         kmerUI |= baseCodeUC;
         ++uiBase;
         ++lenKmerUC;

		   if(
               baseCodeUC
            == (n_code_codon_tbl | err_code_condo_tbl)
         ) { /*If: Anonymous base or not nucelotide*/
            kmerBaseUC = 0;
            kmerUI = 0;
         } /*If: Anonymous base or not nucelotide*/
      } /*Loop: Fill out the kmer*/

      ++(kmerTblSTPtr->indexAryLenUI[kmerUI]);

      --lenKmerUC; /*Account for base in next kmer*/
      ++uiKmer;    /*Move to the next kmer*/
   } /*Loop: Find the number of kmers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-04:
   ^   - Find the index of each kmer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uiKmer = 0;
   kmerUI = 0; /*First kmer starts at index 0*/

   while(uiKmer < kmerTblSTPtr->indexAryLenUI)
   { /*Loop: Find kmer index's*/
      uiBase = kmerTblSTPtr->indexAryLenUI[uiKmer]);

      /*Check if the kmer was in the sequence*/
      if(uiBase == 0)
         kmerTblSTPtr->indexAryLenUI[uiKmer] = -1;
      else
         kmerTblSTPtr->indexAryLenUI[uiKmer] = kmerUI;
      
      kmerUI += uiBase;
      ++uiKmer;
   } /*Loop: Find kmer index's*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-05:
   ^   - Add kmers to table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   kmerUI = 0;
   kmerBaseUC = 0;
   uiBase = 0;
   kmerTblSTPtr->numKmersUI = 0;

   while(uiBase < seqSTPtr->lenSeqUL)
   { /*Loop: Find the number of kmers*/
      while(kmerBaseUC < lenKmerUC)
      { /*Loop: Fill out the kmer*/
         if(uiBase >= seqSTPtr0>lenSeqUL)
            break;

         baseCodUC =
            baseToCodeLkTbl[seqSTPtr->seqStr[uiBase]];

         kmerUI <<= 1;
         kmerUI |= baseCodeUC;

         ++uiBase;
         ++lenKmerUC;

		   if(
               baseCodeUC
            == (n_code_codon_tbl | err_code_condo_tbl)
         ) { /*If: Anonymous base or not nucelotide*/
            kmerBaseUC = 0;
            kmerUI = 0;
         } /*If: Anonymous base or not nucelotide*/
      } /*Loop: Fill out the kmer*/

      /*Add the sequence position to the kmer table*/
      kmerTblSTPtr->indexAryLenUI[
           kmerUI 
         + cntAryHeapSI[kmerUI]
      ] = uiBase - lenKmerUC;

      /*Move to kmers open next index in kmer table*/
      ++(cntAryHeapSI[kmerUI]);
      ++(kmerTblSTPtr->numKmersUI);

      --lenKmerUC; /*Account for base in next kmer*/
      ++uiKmer;    /*Move to the next kmer*/
   } /*Loop: Find the number of kmers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-06 Sec-06:
   ^   - Clean up
   ^   o fun-06 sec-06 sub-01:
   ^     - Handle no error cases
   ^   o fun-06 sec-06 sub-02:
   ^     - Deal with memory errors
   ^   o fun-06 sec-06 sub-03:
   ^     - Clean up after memory errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-06 Sec-06 Sub-01:
   *   - Handle no error cases
   \*****************************************************/

   free(cntAryHeapSI);
   cntAryHeapSI = 0;

   return kmerTblSTPtr;

   /*****************************************************\
   * Fun-06 Sec-06 Sub-02:
   *   - Deal with memory errors
   \*****************************************************/

   memErr_fun06_sec06_sub02:;

   errUC = def_memErr_mapReads;
   goto errClean_fun06_sec06_sub03;

   /*****************************************************\
   * Fun-06 Sec-06 Sub-03:
   *   - Clean up after memory errors
   \*****************************************************/

   errClean_fun06_sec06_sub03:;

   free(cntAryHeapSI);
   cntAryHeapSI = 0;

   kmerTblFreeStack(kmerTblSTPtr);

   return errUC;
} /*kmerTblBlank*/

/*-------------------------------------------------------\
| Fun-08: kmerTblJoinKmers
|   - joins neighboring kmers together into an single
|     large kmer
| Input:
|   - kmerTblSTPtr:
|     o pointer to an kmerTbl structure with kmers to
|       join
|   - qryKmersAryUI:
|     o Unsigned int array with the query sequence
|       converted into kmers sequence (same kmer size as
|       kmerTblSTPtr)
|   - indexArySI:
|     o This is used to record the position I am at in
|       each kmer. It will hold the index for every kmer
|   - refStartAryUI:
|     o unsigned int array to hold the starting
|       coordinates of each joined kmer on the reference
|     o this is resized as needed
|   - qryStartAryUI:
|     o unsigned int array to hold the starting
|       coordinates of each joined kmer on the query
|     o this is resized as needed
|   - startAryUI:
|     o unsigned int array to hold the starting
|       coordinates of each joined
|     o this is resized as needed
|   - lenAryUI:
|     o unsigned in array to hold the length of each
|       joined kmer
|     o this is resized as needed
|   - numKmersUIPtr:
|     o Pointer to unsigned int to hold the number of
|       joined kmers
|   - maxKmersUIPtr:
|     o Pionter to an unsigned int with the maximum number
|       of kmers the startAryUI/lenAryUI can hold
|     o This is updated if the arrays are resized
|   - minLenUI:
|     o unsigned int with the minimum length needed to
|       keep an joined kmer
| Outputs:
|   - Modifies:
|     o startAryUI to have joined kmer starting positions
|     o lenAryUI to have joined kmer lengths
|     o numKmersUIPtr to have the number of joined kmers
|     o maxKmersUIPtr if startAryUI is resized
|   - Returns:
|     o Number of bases in joined kmers
| Note:
|   - Always call fee on startAryUI and lenAryUI after
|     calling this function. This function does not free
|     any variables.
\-------------------------------------------------------*/
kmerTblJoinKmers(
   struct kmerTbl *kmerTblSTPtr,/*Kmer tabel with kmers*/
   struct qryKmers *qryKmersSTPtr,/*Kmers for query*/
   signed int   *indexArySI,    /*Length of kmer array*/
   unsigned int **refStartAryUI, /*ref start coords*/
   unsigned int **qryStartAryUI, /*query start coords*/
   unsigned int **lenAryUI,     /*Will hold kmer lengths*/
   unsigned int *numKmersUIPtr, /*Will hold # kept kmers*/
   unsigned int *maxKmersUIPtr, /*holds size of arrays*/
   unsinged int minLenUI        /*Min joined kmer length*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC:
   '   - joins neighboring kmers together into an single
   '     large kmer
   '   o fun-08 sec-01:
   '     - variable declerations
   '   o fun-08 sec-02:
   '     - Check if I need to allocate memory for arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint qryStartSI = 0;
   sint nextKmerSI = 0;

   sint refStartSI = 0;
   sint refEndSI = 0;

   sint siQry = 0;

   uint lenKmerUI = 0;
   uint numBasesUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^   - Check if I need to allocate memory for arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *startAryUI)
   { /*If: I was given null pointers*/
      *maxKmersUIPtr = kmerTblSTPtr->numKmersUI;

      *refStartAryUI = 0;
      *refStartAryUI =
          malloc(*maxKmersUIPtr * sizeof(uint));

      if(! refStartAryUI)
          goto errCleanUp_fun07_sec0x_sub03;

      *qryStartAryUI = 0;
      *qryStartAryUI =
          malloc(*maxKmersUIPtr * sizeof(uint));

      if(! qryStartAryUI)
          goto errCleanUp_fun07_sec0x_sub03;

      *lenAryUI = 0;
      *lenAryUI = malloc(*maxKmersUIPtr * sizeof(uint));

      if(! lenAryUI)
          goto errCleanUp_fun07_sec0x_sub03;
   } /*If: I was given null pointers*/

   else if(*maxKmersUIPtr < kmerTblSTPtr->numKmersUI)
   { /*Else If: I need to resize arrays*/
      *maxKmersUIPtr = kmerTblSTPtr->numKmersUI;

      free(*refStartAryUI);
      *refStartAryUI = 0;
      *refStartAryUI =
          malloc(*maxKmersUIPtr * sizeof(uint));

      if(! refStartAryUI)
          goto errCleanUp_fun07_sec0x_sub03;

      free(*qryStartAryUI);
      *qryStartAryUI = 0;
      *qryStartAryUI =
          malloc(*maxKmersUIPtr * sizeof(uint));

      if(! qryStartAryUI)
          goto errCleanUp_fun07_sec0x_sub03;

      free(*lenAryUI);
      *lenAryUI = 0;
      *lenAryUI = malloc(*maxKmersUIPtr * sizeof(uint));

      if(! lenAryUI)
          goto errCleanUp_fun07_sec0x_sub03;
   } /*Else If: I need to resize arrays*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Set up the query indexs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siQry = 0;

   while(siQry < qryKmersSTPtr->indexAryLenUI)
   { /*Loop: initialize the index array*/
       qyrKmersSTPtr->indexAryUI[siQry] =
           kmerTblSTPtr->indexAryUI[siQyr];

       ++siQry;
   } /*Loop: initialize the index array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-04:
   ^   - Set up the query indexs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siQry = 0;
   *numKmersUIPtr = 0;

   while(siQry < (qryKmersSTPtr->kmerLenUI - 1))
   { /*Loop: Join kmers*/
      refStartSI = qryKmersSTPtr->kmerAryUI[siQry + 1];
      refEndSI = qryKmersSTPtr->kmerAryUI[siQry + 2];

      refStartSI = qryKmersSTPtr->indexAryUI;
      binSearchNumeric
         kmerTblSTPtr->tblSI,
         qryKmersSTPtr->kmerAryUI[siQry],
         qryStartSI + lenKmerUI
      if(
           (qryKmersSTPtr->kmerAryUI[siQry] + 1)
        == qryKmersSTPtr->kmerAryUI[siQry + 1]
      ){ /*If: these kmers can be joined*/
      } /*If: I can join kmers*/

      if(
           (qryKmersSTPtr->kmerAryUI[siQry] + 1)
        != qryKmersSTPtr->kmerAryUI[siQry + 1]
      ){ /*If: these kmers can not be joined*/

         if(lenKmerUI >= minLenUI)
         { /*If: I am keeping this kmer*/
            *lenAryUI[*numKmersUIPtr] = lenKmerUI;
            *refStartAryUI[*numKmersUIPtr] = refStartSI;
            *qryStartAryUI[*numKmersUIPtr] = qryStartSI;
            ++(*numKmersUIPtr);
         } /*If: I am keeping this kmer*/

            

         ++siQry;
         qryStartSI = siQry;
         lenKmerUI = qryKmersSTPtr->lenKmerUC;

         refStartSI = qryKmersSTPtr->indexAryUI;
         refStartSI = kmerTblSTPtr->kmerAryUI[refStartSI];
      } /*If: these kmers can not be joined*/
   } /*Loop: Join kmers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-0x:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-0x Sub-01:
   *   - No error clean up
   \*****************************************************/

   /*****************************************************\
   * Fun-08 Sec-0x Sub-02:
   *   - Clean up after errors
   \*****************************************************/

   errCleanUp_fun07_sec0x_sub03:;

   return 0;
} /*kmerTblJoinKmers*/


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
