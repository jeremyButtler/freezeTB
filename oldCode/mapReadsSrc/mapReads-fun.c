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
'   o .c fun-08: buildKmerMask
'     - builds an mask for removing unneeded bases from an
'       kmer
'   o .c fun09: getNextKmer
'     - Adds the next base to the kmer and adjusts kmer
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
#include "ntToBit.h"/*Convert nucleotides to xbit values*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .h ../generalLib/ulCpStr.h
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*For building the kmer mask*/
#define def_ntOnePos 0x3 /*Equivlanet to 11 or two bits*/
#define def_kmerShift 2

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
){
   kmerTblFreeStack(kmerTblSTPtr);
   free(kmerTblSTPtr);
   kmerTblSTPtr = 0;
} /*kmerTblFreeHeap*/

/*-------------------------------------------------------\
| Fun05: kmerTblFreeAryHeap
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
| Fun06: kmerTblSetSeq
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
   ' Fun06 TOC:
   '   - Set the sequence in an kmer table
   '   o fun06 sec01:
   '     - Variable declerations
   '   o fun06 sec02:
   '     - Allocate memory
   '   o fun06 sec03:
   '     - Find the number of kmers in the sequence
   '   o fun06 sec04:
   '     - Find the index of each kmer
   '   o fun06 sec05:
   '     - Add kmers to table
   '   o fun06 sec06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
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
   ^ Fun06 Sec02:
   ^   - Allocate memory
   ^   o fun06 sec02 sub01:
   ^     - Find the number of kmers in the table
   ^   o fun06 sec02 sub02:
   ^     - Kmer index (for table) allocation
   ^   o fun06 sec02 sub03:
   ^     - Kmer table memory allocation
   ^   o fun06 sec02 sub04:
   ^     - Temporary index memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec02 Sub01:
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
   * Fun06 Sec02 Sub02:
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
   * Fun06 Sec02 Sub03:
   *   - Kmer table memory allocation
   \*****************************************************/

   kmerTblSTPtr->tblLenUI = seqSTPtr->lenSeqUL -lenKmerUC;

   kmerTblSTPtr->tlbUI =
      malloc(kmerTblSTPtr->tblLenUI * sizeof(sint));

   if(! kmerTblSTPtr->numKmerUIAry)
      goto memErr_fun06_sec06_sub02;

   /*****************************************************\
   * Fun06 Sec02 Sub04:
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
   ^ Fun06 Sec03:
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
   ^ Fun06 Sec04:
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
   ^ Fun06 Sec05:
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
   ^ Fun06 Sec06:
   ^   - Clean up
   ^   o fun06 sec06 sub01:
   ^     - Handle no error cases
   ^   o fun06 sec06 sub02:
   ^     - Deal with memory errors
   ^   o fun06 sec06 sub03:
   ^     - Clean up after memory errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec06 Sub01:
   *   - Handle no error cases
   \*****************************************************/

   free(cntAryHeapSI);
   cntAryHeapSI = 0;

   return kmerTblSTPtr;

   /*****************************************************\
   * Fun06 Sec06 Sub02:
   *   - Deal with memory errors
   \*****************************************************/

   memErr_fun06_sec06_sub02:;

   errUC = def_memErr_mapReads;
   goto errClean_fun06_sec06_sub03;

   /*****************************************************\
   * Fun06 Sec06 Sub03:
   *   - Clean up after memory errors
   \*****************************************************/

   errClean_fun06_sec06_sub03:;

   free(cntAryHeapSI);
   cntAryHeapSI = 0;

   kmerTblFreeStack(kmerTblSTPtr);

   return errUC;
} /*kmerTblBlank*/

/*-------------------------------------------------------\
| Fun08: kmerTblJoinKmers
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
   ' Fun08 TOC:
   '   - joins neighboring kmers together into an single
   '     large kmer
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - Check if I need to allocate memory for arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
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
   ^ Fun08 Sec02:
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
   ^ Fun08 Sec03:
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
   ^ Fun08 Sec04:
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
   ^ Fun08 Sec0x:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec0x Sub01:
   *   - No error clean up
   \*****************************************************/

   /*****************************************************\
   * Fun08 Sec0x Sub02:
   *   - Clean up after errors
   \*****************************************************/

   errCleanUp_fun07_sec0x_sub03:;

   return 0;
} /*kmerTblJoinKmers*/

/*-------------------------------------------------------\
| Fun-08: buildKmerMask
|   - builds an mask for removing unneeded bases from an
|     kmer
| Input:
|   - maskSI:
|     o made into be the kmer mask
|   - lenKmerUC:
|     o length of one kmer
| Output:
|   - Modifies:
|     o maskSI to hold the new mask
\-------------------------------------------------------*/
#define \
buildKmerMask( \
   maskSI,
   lenKmerUC \
){ \
   unsigned char ucNtMac = 0; \
   maskSI = 0; \
   \
   for(ucNtMac = 0; ucNtMac <  (lenKmerUC); ++ucNtMac) \
   { /*Loop: build the kmer mask*/ \
      (maskSI <<= def_kmerShift); \
      (maskSI) |= (def_ntOnePos); \
   } /*Loop: build the kmer mask*/ \
} /*buildKmerMask*/


/*-------------------------------------------------------\
| Fun09: getNextKmer
|   - Adds the next base to the kmer and adjusts kmer
| Input:
|   - ntUC:
|     o nuceotide to add to the kmer
|   - kmerSI:
|     o kmer to add nucleotide to
|   - maskSI:
|     o mask to remove the old kmer (once shifted out)
|   - ntInKmerSI:
|     o Number of nucleotides in the current kmer
| Output:
|   - Modifies:
|     o kmerSI to have the old base remove an ntUC added
|       in
|     o ntInKmerSI:
|       - 0 if I had an anonymous base or an error
|       - number if no anonymous base. This value can be
|         geater then the kmer size. It is just a way to
|         make sure I did not reset after an anonymous
|         base
\-------------------------------------------------------*/
#define \
getNextKmer( \
   ntUC, \
   kmerSI, \
   maskSI, \
   ntInKmerSI \
){ \
   unsigned char codeMacUC = 0; \
   \
   codeMacUC = ntToTwoBit[refStr[(unsigned char) (ntUC)];\
   \
   (kmerSI) <<= 2; \
   (kmerSI) |= (codeMacUC); \
   \
   (kmerSI) &= (maskSI); /*remove old base in kmer*/ \
   \
   codeMacUC = !( ntUC & (err_sixBit | n_fiveBit) ); \
   (ntInKmerSI) &= (-codeMacUC); \
   (ntInKmerSI) += codeMacUC; \
      /*Logic
      `   - ntCU = !( ntUC & (err | n) )
      `     o 1 if no error or anonymous base
      `     o 0 if error or anonymous base
      `   - ntInKmerSI &= (-ntUC)
      `     o For no error or anonymous base case; goes
      `       to ntInKmerSI & -1 = ntInKmersSI
      `     o For error or anonymous base case; goes to
      `       ntInKmerSI & 0 = 0
      `   - ntInKmerSI += ntUC
      `     o For no error or anonymous base case; goes
      `       to ntInKmerSI + 1
      `     o For error or anonymous base case; goes to
      `       ntInKmerSI + 0
      */ \ 
} /*getNextKmer*/

struct kmerSearch *
kmerSearch_mk(
   signed char *refStr,
   signed int lenRefSI,
   signed char lenKmerSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   o fun10 Sec01:
   '     - variable declerations
   '   o fun10 Sec02:
   '     - make and allocate memrory for kmerSearch struct
   '   o fun10 sec03:
   '     - get kmer counts
   '   o fun10 Sec04:
   '     - build the search table
   '   o fun10 Sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar ntUC = 0;

   sint siKmer = 0;
   sint ntInKmerSI = 0;
   sint siNt = 0;
   sint kmerIndxSI = 0;
   sint lastKmerSI = 0;

   sint maskSI = 0; /*Mask to remove old kmers*/
   struct kmerSearch *retHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - make and allocate memrory for kmerSearch struct
   ^   o fun10 sec02 sub01:
   ^     - make and initialize kmerSearch structure
   ^   o fun10 sec02 sub02:
   ^     - allocate memory for ref positions and tables
   ^   o fun10 sec02 sub03:
   ^     - get max kmers in table & allocate kmerIndxArySI
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec02 Sub01:
   *   - make and initialize kmerSearch structure
   \*****************************************************/

   retHeapST = malloc(sizeof(struct kmerSearch));

   if(! retHeapST)
      errCleanUp_fun10_sec0x_sub02;

   if(! kmerTblSTPtr)
      return 0;   /*No kmer table input*/

   kmerSearch_init(retHeapST);
   retHeapST->lenRefSI = lenRefSI;

   /*****************************************************\
   * Fun10 Sec02 Sub02:
   *   - allocate memory for ref positions and tables
   \*****************************************************/

   retHeapST->refPosArySI =
      malloc(lenRefSI * sizeof(sint));

   if(! retHeapST->refPosArySI)
      goto errCleanUp_fun10_sec0x_sub02;

   retHeapST->tblIndexArySI =
      malloc(lenRefSI * sizeof(sint));

   if(! retHeapST->tblIndexArySI)
      goto errCleanUp_fun10_sec0x_sub02;

   retHeapST->searchTblSI =
      malloc(lenRefSI * sizeof(sint));

   if(! retHeapST->searchTblSI)
      goto errCleanUp_fun10_sec0x_sub02;

   /*****************************************************\
   * Fun10 Sec02 Sub03:
   *   - find max kmers in table & allocate kmerIndxArySI
   \*****************************************************/

   retHeapST->lenKmerUC = lenKmerUC;

   /*Find the number of kmers in the table*/
   retHeapST->maxKmersSI = 1;

   for(siKmer = 0; siKmer < lenKmerUC; ++siKmer)
      (retHeapST->maxKmersSI) <<= 2;

   retHeapST->lenKmerIndxUI = (retHeapST->maxKmersSI) <<2;

   retHeapST->kmerIndxArySI =
      malloc((retHeapST)->kmerInxUI * sizeof(sint));

   if(! retHeapST->kmerIndxArySI)
      goto errCleanUp_fun10_sec0x_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - get kmer counts (sets index for tblIndxArySI)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   buildKmerMask(maskSI, lenKmerUC);
   ntInKmerSI = 0;

   for(siNt = 0; siNt < refLenSI; ++siNt)
   { /*Loop: Count how number of times each kmer repeats*/
      getNextKmer(
         refStr[siNt],
         siKmer,
         maskSI,     /*built in fun10 sec02 sub03*/
         ntInKmerSI
      );

      retHeapST->kmerIndexArySI[siKmer << 1] +=
         (ntInKmerSI >= lenKmerUC);
         /* ntInKmerSI > lenKmerUC means that its been an
         `    bit since I had an error or anonymous base
         */
   } /*Loop: Count how number of times each kmer repeats*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - build the search table
   ^   o fun10 sec02 sub01:
   ^   o fun10 sec02 sub02:
   ^   o fun10 sec02 sub03:
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ntInKmerSI = 0;

   for(siNt = 0; siNt < refLenSI; ++siNt)
   { /*Loop: Count how number of times each kmer repeats*/
      getNextKmer(
         refStr[siNt],
         siKmer,
         maskSI,     /*built in fun10 sec02 sub03*/
         ntInKmerSI
      );

      if(ntInKmerSI < lenKmerUC)
         continue; /*Anoymous base in kmer*/

      kmerIndxSI = siKmer << 1;

      siKmer = retHeapST->kmerIndexArySI[kmerIndxSI];

      ++kmerIndexSI;
      ++(retHeapST->kmerIndexArySI[kmerIndxSI]);

      lastKmerSI = kmerIndexSI;

      siKmer += retHeapST->kmerIndexArySI[kmerIndxSI];
   } /*Loop: Count how number of times each kmer repeats*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec05:
   ^   - clean up and return
   ^   o fun10 sec05 sub01:
   ^     - no error clean up
   ^   o fun10 sec05 sub02:
   ^     - error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   return retHeapST;

   /*****************************************************\
   * Fun10 Sec05 Sub02:
   *   - error clean up
   \*****************************************************/

   errCleanUp_fun10_sec0x_sub02:;

   kmerSearch_freeHeap(retHeapST);
   retHeapST = 0;

   return 0;
} /*kmerSearch_addRef*/


/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the publikc domain
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
