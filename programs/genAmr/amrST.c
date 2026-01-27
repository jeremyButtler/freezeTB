/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' amrST SOF: Start Of File
'   - holds functions to support the amrST struct used in
'     tbAmr.
'   o header:
'     - include libraries
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
'        - finds index of amrAry assigned to a reference
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
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "amrST.h"

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genLib/fileFun.h"
#include "drugAry.h"

/*.h files only*/
#include "../genLib/64bit.h"
#include "../genLib/endLine.h"

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
){
   signed int siAmr = 0;

   amrSTPtr->refPosUI = 0;
   amrSTPtr->codonPosUI = 0;
   amrSTPtr->codonNumUI = 0;
   amrSTPtr->endCodonNumUI = 0;
   
   amrSTPtr->geneFirstRefUI = 0;
   amrSTPtr->geneLastRefUI = 0;

   amrSTPtr->frameshiftBl = 0;
   amrSTPtr->wholeGeneFlagSC = 0;
   amrSTPtr->unknownBl = 0;
   amrSTPtr->aaMultiDupBl = 0;

   amrSTPtr->dirFlagSC = def_unkownDir_amrST;
   amrSTPtr->mutTypeStr[0] = '\0';

   amrSTPtr->gradeSC = 0;
   amrSTPtr->numSupReadsUI = 0;
   amrSTPtr->numMapReadsUI = 0;

   amrSTPtr->highResBl = 0;
   amrSTPtr->lowResBl = 0;
   amrSTPtr->additiveResBl = 0;

   for(
      siAmr = 0;
      siAmr < def_maxDrugs_amrST;
      ++siAmr
   ){ /*Loop: blank antibiotic flags*/
      amrSTPtr->amrFlagsUL[siAmr] = 0;
      amrSTPtr->crossResFlagsUL[siAmr] = 0;
   } /*Loop: blank antibiotic flags*/
} /*blank_amrST*/

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
){
   amrSTPtr->geneIdStr = 0;
   amrSTPtr->lenGeneIdUI = 0;
   
   amrSTPtr->refSeqStr = 0;
   amrSTPtr->lenRefSeqUI = 0;
    
   amrSTPtr->amrSeqStr = 0;
   amrSTPtr->lenAmrSeqUI = 0;
   
   amrSTPtr->varIdStr = 0;
   amrSTPtr->lenVarIdUI = 0;
   
   amrSTPtr->effectStr = 0;
   amrSTPtr->lenEffectUI = 0;
   
   amrSTPtr->commentStr = 0;
   amrSTPtr->lenCommentUI = 0;
   
   amrSTPtr->refAaStr = 0;
   amrSTPtr->lenRefAaUI = 0;
   
   amrSTPtr->amrAaStr = 0;
   amrSTPtr->lenAmrAaUI = 0;
   
   amrSTPtr->needsGeneStr = 0;
   amrSTPtr->lenNeedsGeneUI = 0;

   blank_amrST(amrSTPtr);
} /*init_amrST*/

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
){
   if(amrSTPtr)
   { /*If: have amrST struct*/
      if(amrSTPtr->geneIdStr != 0)
        free(amrSTPtr->geneIdStr);
      
      if(amrSTPtr->refSeqStr != 0)
        free(amrSTPtr->refSeqStr);
      
      if(amrSTPtr->amrSeqStr != 0)
        free(amrSTPtr->amrSeqStr);
      
      if(amrSTPtr->varIdStr != 0)
        free(amrSTPtr->varIdStr);
      
      if(amrSTPtr->effectStr != 0)
        free(amrSTPtr->effectStr);
      
      if(amrSTPtr->commentStr != 0)
        free(amrSTPtr->commentStr);
      
      if(amrSTPtr->refAaStr != 0)
         free(amrSTPtr->refAaStr);
      
      if(amrSTPtr->amrAaStr != 0)
         free(amrSTPtr->amrAaStr);

      if(amrSTPtr->needsGeneStr != 0)
         free(amrSTPtr->needsGeneStr);
      
      init_amrST(amrSTPtr);
   } /*If: have amrST struct*/
} /*freeStack_amrST*/

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
){
   if(amrSTPtr)
   { /*If: have amrST struct to free*/
      freeStack_amrST(amrSTPtr);
      free(amrSTPtr);
   } /*If: have amrST struct to free*/
} /*freeHeap_amrST*/

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
   unsigned long numElmUL /*number of elements*/
){
   unsigned long ulElm = 0;

   if(amrSTAry)
   { /*If: have array to free*/
      for(ulElm = 0; ulElm < numElmUL; ++ulElm)
         freeStack_amrST(&amrSTAry[ulElm]);
      free(amrSTAry);
   } /*If: have array to free*/
} /*freeHeapAry_amrST*/

/*-------------------------------------------------------\
| Fun06: swap_amrST
|   - swaps values between two amrST structures
| Input:
|   - firstAmrSTPtr:
|     - first amrST to swap values in (not a pointer)
|   - secAmrSTPtr:
|     - second amrST to swap values in (not a pointer)
| Output:
|   - Modifies:
|     o fristAmrSTPtr to have the values of secAmrSTPtr
|     o secAmrSTPtr to have the values of frstAmrSTPtr
\-------------------------------------------------------*/
void
swap_amrST(
   struct amrST *firstAmrST,
   struct amrST *secAmrST
){
   signed char *tmpStr = 0;
   signed int siAmr = 0;
   
   firstAmrST->refPosUI ^= secAmrST->refPosUI;
   secAmrST->refPosUI ^= firstAmrST->refPosUI;
   firstAmrST->refPosUI ^= secAmrST->refPosUI;

   firstAmrST->codonPosUI ^= secAmrST->codonPosUI;
   secAmrST->codonPosUI ^= firstAmrST->codonPosUI;
   firstAmrST->codonPosUI ^= secAmrST->codonPosUI;

   firstAmrST->codonNumUI ^= secAmrST->codonNumUI;
   secAmrST->codonNumUI ^= firstAmrST->codonNumUI;
   firstAmrST->codonNumUI ^= secAmrST->codonNumUI;

   firstAmrST->endCodonNumUI ^= secAmrST->endCodonNumUI;
   secAmrST->endCodonNumUI ^= firstAmrST->endCodonNumUI;
   firstAmrST->endCodonNumUI ^= secAmrST->endCodonNumUI;

   firstAmrST->geneFirstRefUI ^= secAmrST->geneFirstRefUI;
   secAmrST->geneFirstRefUI ^= firstAmrST->geneFirstRefUI;
   firstAmrST->geneFirstRefUI ^= secAmrST->geneFirstRefUI;

   firstAmrST->geneLastRefUI ^= secAmrST->geneLastRefUI;
   secAmrST->geneLastRefUI ^= firstAmrST->geneLastRefUI;
   firstAmrST->geneLastRefUI ^= secAmrST->geneLastRefUI;

   tmpStr = firstAmrST->refAaStr;
   firstAmrST->refAaStr = secAmrST->refAaStr;
   secAmrST->refAaStr = tmpStr;

   firstAmrST->lenRefAaUI ^= secAmrST->lenRefAaUI;
   secAmrST->lenRefAaUI ^= firstAmrST->lenRefAaUI;
   firstAmrST->lenRefAaUI ^= secAmrST->lenRefAaUI;
   
   tmpStr = firstAmrST->amrAaStr;
   firstAmrST->amrAaStr = secAmrST->amrAaStr;
   secAmrST->amrAaStr = tmpStr;

   firstAmrST->lenAmrAaUI ^= secAmrST->lenAmrAaUI;
   secAmrST->lenAmrAaUI ^= firstAmrST->lenAmrAaUI;
   firstAmrST->lenAmrAaUI ^= secAmrST->lenAmrAaUI;
   
   firstAmrST->aaMultiDupBl ^= secAmrST->aaMultiDupBl;
   secAmrST->aaMultiDupBl ^= firstAmrST->aaMultiDupBl;
   firstAmrST->aaMultiDupBl ^= secAmrST->aaMultiDupBl;

   firstAmrST->frameshiftBl ^= secAmrST->frameshiftBl;
   secAmrST->frameshiftBl ^= firstAmrST->frameshiftBl;
   firstAmrST->frameshiftBl ^= secAmrST->frameshiftBl;

   firstAmrST->wholeGeneFlagSC^=secAmrST->wholeGeneFlagSC;
   secAmrST->wholeGeneFlagSC^=firstAmrST->wholeGeneFlagSC;
   firstAmrST->wholeGeneFlagSC^=secAmrST->wholeGeneFlagSC;
   
   firstAmrST->unknownBl ^= secAmrST->unknownBl;
   secAmrST->unknownBl ^= firstAmrST->unknownBl;
   firstAmrST->unknownBl ^= secAmrST->unknownBl;
   
   tmpStr = firstAmrST->geneIdStr;
   firstAmrST->geneIdStr = secAmrST->geneIdStr;
   secAmrST->geneIdStr = tmpStr;

   firstAmrST->lenGeneIdUI ^= secAmrST->lenGeneIdUI;
   secAmrST->lenGeneIdUI ^= firstAmrST->lenGeneIdUI;
   firstAmrST->lenGeneIdUI ^= secAmrST->lenGeneIdUI;
   
   tmpStr = firstAmrST->refSeqStr;
   firstAmrST->refSeqStr = secAmrST->refSeqStr;
   secAmrST->refSeqStr = tmpStr;
   
   firstAmrST->lenRefSeqUI ^= secAmrST->lenRefSeqUI;
   secAmrST->lenRefSeqUI ^= firstAmrST->lenRefSeqUI;
   firstAmrST->lenRefSeqUI ^= secAmrST->lenRefSeqUI;
   
   tmpStr = firstAmrST->amrSeqStr;
   firstAmrST->amrSeqStr = secAmrST->amrSeqStr;
   secAmrST->amrSeqStr = tmpStr;
   
   firstAmrST->lenAmrSeqUI ^= secAmrST->lenAmrSeqUI;
   secAmrST->lenAmrSeqUI ^= firstAmrST->lenAmrSeqUI;
   firstAmrST->lenAmrSeqUI ^= secAmrST->lenAmrSeqUI;
   
   tmpStr = firstAmrST->varIdStr;
   firstAmrST->varIdStr = secAmrST->varIdStr;
   secAmrST->varIdStr = tmpStr;
   
   firstAmrST->lenVarIdUI ^= secAmrST->lenVarIdUI;
   secAmrST->lenVarIdUI ^= firstAmrST->lenVarIdUI;
   firstAmrST->lenVarIdUI ^= secAmrST->lenVarIdUI;
   
   tmpStr = firstAmrST->effectStr;
   firstAmrST->effectStr = secAmrST->effectStr;
   secAmrST->effectStr = tmpStr;
   
   firstAmrST->lenEffectUI ^= secAmrST->lenEffectUI;
   secAmrST->lenEffectUI ^= firstAmrST->lenEffectUI;
   firstAmrST->lenEffectUI ^= secAmrST->lenEffectUI;
   
   tmpStr = firstAmrST->commentStr;
   firstAmrST->commentStr = secAmrST->commentStr;
   secAmrST->commentStr = tmpStr;
   
   firstAmrST->lenCommentUI ^= secAmrST->lenCommentUI;
   secAmrST->lenCommentUI ^= firstAmrST->lenCommentUI;
   firstAmrST->lenCommentUI ^= secAmrST->lenCommentUI;
   
   firstAmrST->dirFlagSC ^= secAmrST->dirFlagSC;
   secAmrST->dirFlagSC ^= firstAmrST->dirFlagSC;
   firstAmrST->dirFlagSC ^= secAmrST->dirFlagSC;
   
   firstAmrST->gradeSC ^= secAmrST->gradeSC;
   secAmrST->gradeSC ^= firstAmrST->gradeSC;
   firstAmrST->gradeSC ^= secAmrST->gradeSC;
   
   firstAmrST->numSupReadsUI ^= secAmrST->numSupReadsUI;
   secAmrST->numSupReadsUI ^= firstAmrST->numSupReadsUI;
   firstAmrST->numSupReadsUI ^= secAmrST->numSupReadsUI;
   
   firstAmrST->numMapReadsUI ^= secAmrST->numMapReadsUI;
   secAmrST->numMapReadsUI ^= firstAmrST->numMapReadsUI;
   firstAmrST->numMapReadsUI ^= secAmrST->numMapReadsUI;
   
   /*swap mutation type (3 chars max)*/
   firstAmrST->mutTypeStr[0] ^= secAmrST->mutTypeStr[0];
   secAmrST->mutTypeStr[0] ^= firstAmrST->mutTypeStr[0];
   firstAmrST->mutTypeStr[0] ^= secAmrST->mutTypeStr[0];
   
   firstAmrST->mutTypeStr[1] ^= secAmrST->mutTypeStr[1];
   secAmrST->mutTypeStr[1] ^= firstAmrST->mutTypeStr[1];
   firstAmrST->mutTypeStr[1] ^= secAmrST->mutTypeStr[1];
   
   firstAmrST->mutTypeStr[2] ^= secAmrST->mutTypeStr[2];
   secAmrST->mutTypeStr[2] ^= firstAmrST->mutTypeStr[2];
   firstAmrST->mutTypeStr[2] ^= secAmrST->mutTypeStr[2];
   
   tmpStr = firstAmrST->needsGeneStr;
   firstAmrST->needsGeneStr = secAmrST->needsGeneStr;
   secAmrST->needsGeneStr = tmpStr;
   
   firstAmrST->lenNeedsGeneUI ^= secAmrST->lenNeedsGeneUI;
   secAmrST->lenNeedsGeneUI ^= firstAmrST->lenNeedsGeneUI;
   firstAmrST->lenNeedsGeneUI ^= secAmrST->lenNeedsGeneUI;
   
   firstAmrST->highResBl ^= secAmrST->highResBl;
   secAmrST->highResBl ^= firstAmrST->highResBl;
   firstAmrST->highResBl ^= secAmrST->highResBl;
   
   firstAmrST->lowResBl ^= secAmrST->lowResBl;
   secAmrST->lowResBl ^= firstAmrST->lowResBl;
   firstAmrST->lowResBl ^= secAmrST->lowResBl;
   
   firstAmrST->additiveResBl ^= secAmrST->additiveResBl;
   secAmrST->additiveResBl ^= firstAmrST->additiveResBl;
   firstAmrST->additiveResBl ^= secAmrST->additiveResBl;
   
   for(
      siAmr = 0;
      siAmr < def_maxDrugs_amrST;
      ++siAmr
   ){ /*Loop: swap antibiotic flags*/
      firstAmrST->amrFlagsUL[siAmr] ^=
         secAmrST->amrFlagsUL[siAmr];

      secAmrST->amrFlagsUL[siAmr] ^=
         firstAmrST->amrFlagsUL[siAmr];

      firstAmrST->amrFlagsUL[siAmr] ^=
         secAmrST->amrFlagsUL[siAmr];
      
      
      firstAmrST->crossResFlagsUL[siAmr] ^=
         secAmrST->crossResFlagsUL[siAmr];

      secAmrST->crossResFlagsUL[siAmr] ^=
         firstAmrST->crossResFlagsUL[siAmr];

      firstAmrST->crossResFlagsUL[siAmr] ^=
         secAmrST->crossResFlagsUL[siAmr];
   } /*Loop: swap antibiotic flags*/
} /*swap_amrST*/

/*-------------------------------------------------------\
| Fun07: cp_amrST
|   - copies (deep copy) values from one amrST struct to
|     another
| Input:
|   - dupAmrST:
|     - amrST struct pointer to copy to
|   - cpAmrST:
|     - amrST struct ponter to copy from
| Output:
|   - Modifies:
|     o dupAmrST to have the values of cpAmrST
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
cp_amrST(
   struct amrST *dupAmrST,
   struct amrST *cpAmrST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - copies (deep copy) values from one amrST struct
   '     to another
   '   o fun07 sec01:
   '     - variable declaration
   '   o fun07 sec02:
   '     - copy coordinate entries
   '   o fun07 sec03:
   '     - copy amino acid sequences and some flags
   '   o fun07 sec04:
   '     - copy gene name, nt sequences, and variant id
   '   o fun07 sec05:
   '     - copy effect, comment, various flags, mutation
   '       type and if needs other genes
   '   o fun07 sec06:
   '     - copy drug resistance
   '   o fun07 sec07:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declaration
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   signed int siAmr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - copy coordinate entries
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   dupAmrST->refPosUI = cpAmrST->refPosUI;

   dupAmrST->codonPosUI = cpAmrST->codonPosUI;
   dupAmrST->codonNumUI = cpAmrST->codonNumUI;
   dupAmrST->endCodonNumUI = cpAmrST->endCodonNumUI;

   dupAmrST->geneFirstRefUI = cpAmrST->geneFirstRefUI;
   dupAmrST->geneLastRefUI = cpAmrST->geneLastRefUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - copy amino acid sequences and some flags
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*____________reference_aa_sequence__________________*/
   if(dupAmrST->refAaStr)
      free(dupAmrST->refAaStr);
   dupAmrST->lenRefAaUI =
      ulAlign_64bit(
         (cpAmrST->lenRefAaUI + 1),
         sizeof(signed char)
      );
   dupAmrST->refAaStr =
      malloc(dupAmrST->lenRefAaUI * sizeof(signed char));
   if(! dupAmrST->refAaStr)
      goto memErr_fun07_sec07;

   if(! cpAmrST->refAaStr)
      dupAmrST->refAaStr[0] = 0;
   else
      cpLen_ulCp(
         dupAmrST->refAaStr,
         cpAmrST->refAaStr,
         cpAmrST->lenAmrAaUI
      );

   dupAmrST->lenRefAaUI = cpAmrST->lenRefAaUI;
   
   /*_______________amr_aa_seqeunce_____________________*/
   if(dupAmrST->amrAaStr)
      free(dupAmrST->amrAaStr);
   dupAmrST->lenAmrAaUI =
      ulAlign_64bit(
         (cpAmrST->lenAmrAaUI + 1),
         sizeof(signed char)
      );
   dupAmrST->amrAaStr =
      malloc(dupAmrST->lenAmrAaUI * sizeof(signed char));
   if(! dupAmrST->amrAaStr)
      goto memErr_fun07_sec07;

   if(! cpAmrST->amrAaStr)
      dupAmrST->amrAaStr[0] = 0;
   else
      cpLen_ulCp(
         dupAmrST->amrAaStr,
         cpAmrST->amrAaStr,
         cpAmrST->lenAmrAaUI
      );
   
   dupAmrST->lenAmrAaUI = cpAmrST->lenAmrAaUI;

   /*_________________various_flags_____________________*/
   dupAmrST->aaMultiDupBl = cpAmrST->aaMultiDupBl;
   dupAmrST->frameshiftBl = cpAmrST->frameshiftBl;
   dupAmrST->wholeGeneFlagSC = cpAmrST->wholeGeneFlagSC;
   dupAmrST->unknownBl = cpAmrST->unknownBl;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - copy gene name, nt sequences, and variant id
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   
   /*_______________gene_name___________________________*/
   if(dupAmrST->geneIdStr)
      free(dupAmrST->geneIdStr);
   dupAmrST->lenGeneIdUI =
      ulAlign_64bit(
         (cpAmrST->lenGeneIdUI + 1),
         sizeof(signed char)
      );
   dupAmrST->geneIdStr =
      malloc(dupAmrST->lenGeneIdUI * sizeof(signed char));
   if(! dupAmrST->geneIdStr)
      goto memErr_fun07_sec07;
   cpLen_ulCp(
      dupAmrST->geneIdStr,
      cpAmrST->geneIdStr,
      cpAmrST->lenGeneIdUI
   );
   
   dupAmrST->lenGeneIdUI = cpAmrST->lenGeneIdUI;

   /*_______________reference_sequence__________________*/
   if(dupAmrST->refSeqStr)
      free(dupAmrST->refSeqStr);
   dupAmrST->lenRefSeqUI =
      ulAlign_64bit(
         (cpAmrST->lenRefSeqUI + 1),
         sizeof(signed char)
      );
   dupAmrST->refSeqStr =
      malloc(dupAmrST->lenRefSeqUI * sizeof(signed char));
   if(! dupAmrST->refSeqStr)
      goto memErr_fun07_sec07;
   cpLen_ulCp(
      dupAmrST->refSeqStr,
      cpAmrST->refSeqStr,
      cpAmrST->lenRefSeqUI
   );

   dupAmrST->lenRefSeqUI = cpAmrST->lenRefSeqUI;
   
   /*_______________amr_sequence________________________*/
   if(dupAmrST->amrSeqStr)
      free(dupAmrST->amrSeqStr);
   dupAmrST->lenAmrSeqUI =
      ulAlign_64bit(
         (cpAmrST->lenAmrSeqUI + 1),
         sizeof(signed char)
      );
   dupAmrST->amrSeqStr =
      malloc(dupAmrST->lenAmrSeqUI * sizeof(signed char));
   if(! dupAmrST->amrSeqStr)
      goto memErr_fun07_sec07;
   cpLen_ulCp(
      dupAmrST->amrSeqStr,
      cpAmrST->amrSeqStr,
      cpAmrST->lenAmrSeqUI
   );

   dupAmrST->lenAmrSeqUI = cpAmrST->lenAmrSeqUI;
   
   /*_______________variant_id__________________________*/
   if(dupAmrST->varIdStr)
      free(dupAmrST->varIdStr);
   dupAmrST->lenVarIdUI =
      ulAlign_64bit(
        (cpAmrST->lenVarIdUI + 1),
        sizeof(signed char)
      );
   dupAmrST->varIdStr =
      malloc(dupAmrST->lenVarIdUI * sizeof(signed char));
   if(! dupAmrST->varIdStr)
      goto memErr_fun07_sec07;
   cpLen_ulCp(
      dupAmrST->varIdStr,
      cpAmrST->varIdStr,
      cpAmrST->lenVarIdUI
   );

   dupAmrST->lenVarIdUI = cpAmrST->lenVarIdUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - copy effect, comment, various flags, mutation
   ^     type and if needs other genes
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   
   /*_______________effect_entry________________________*/
   if(dupAmrST->effectStr)
      free(dupAmrST->effectStr);
   dupAmrST->lenEffectUI =
      ulAlign_64bit(
         (cpAmrST->lenEffectUI + 1),
         sizeof(signed char)
      );
   dupAmrST->effectStr =
      malloc(dupAmrST->lenEffectUI * sizeof(signed char));
   if(! dupAmrST->effectStr)
      goto memErr_fun07_sec07;

   if(! cpAmrST->effectStr)
      dupAmrST->effectStr[0] = 0;
   else
      cpLen_ulCp(
         dupAmrST->effectStr,
         cpAmrST->effectStr,
         cpAmrST->lenEffectUI
      );
   
   dupAmrST->lenEffectUI = cpAmrST->lenEffectUI;

   /*_______________comment_entry_______________________*/
   if(dupAmrST->commentStr)
      free(dupAmrST->commentStr);
   dupAmrST->lenCommentUI =
      ulAlign_64bit(
         (cpAmrST->lenCommentUI + 1),
         sizeof(signed char)
      );
   dupAmrST->commentStr =
      malloc(dupAmrST->lenCommentUI *sizeof(signed char));
   if(! dupAmrST->commentStr)
      goto memErr_fun07_sec07;

   if(! cpAmrST->commentStr)
      dupAmrST->commentStr[0] = 0;
   else
      cpLen_ulCp(
         dupAmrST->commentStr,
         cpAmrST->commentStr,
         cpAmrST->lenCommentUI
      );
   
   dupAmrST->lenCommentUI = cpAmrST->lenCommentUI;

   /*____________various_flags__________________________*/
   dupAmrST->dirFlagSC = cpAmrST->dirFlagSC;
   dupAmrST->gradeSC = cpAmrST->gradeSC;
   dupAmrST->numSupReadsUI = cpAmrST->numSupReadsUI;
   dupAmrST->numMapReadsUI = cpAmrST->numMapReadsUI;
   dupAmrST->highResBl = cpAmrST->highResBl;
   dupAmrST->lowResBl = cpAmrST->lowResBl;
   dupAmrST->additiveResBl = cpAmrST->additiveResBl;
   
   /*____________mutation_type__________________________*/
   dupAmrST->mutTypeStr[0] = cpAmrST->mutTypeStr[0];
   dupAmrST->mutTypeStr[1] = cpAmrST->mutTypeStr[1];
   dupAmrST->mutTypeStr[2] = cpAmrST->mutTypeStr[2];
   dupAmrST->mutTypeStr[3] = 0;
   
   /*_______________if_needs_another_gene_______________*/
   if(dupAmrST->needsGeneStr)
      free(dupAmrST->needsGeneStr);
   dupAmrST->lenNeedsGeneUI =
      ulAlign_64bit(
         (cpAmrST->lenNeedsGeneUI + 1),
         sizeof(signed char)
      );
   dupAmrST->needsGeneStr =
      malloc(
         dupAmrST->lenNeedsGeneUI * sizeof(signed char)
      );
   if(! dupAmrST->needsGeneStr)
      goto memErr_fun07_sec07;

   if(! cpAmrST->needsGeneStr)
      dupAmrST->needsGeneStr[0] = 0;
   else
      cpLen_ulCp(
         dupAmrST->needsGeneStr,
         cpAmrST->needsGeneStr,
         cpAmrST->lenNeedsGeneUI
      );

   dupAmrST->lenNeedsGeneUI = cpAmrST->lenNeedsGeneUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec06:
   ^   - copy drug resistance
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   
   for(
      siAmr = 0;
      siAmr < def_maxDrugs_amrST;
      ++siAmr
   ){ /*Loop: swap antibiotic flags*/
      dupAmrST->amrFlagsUL[siAmr] =
         cpAmrST->amrFlagsUL[siAmr];
      dupAmrST->crossResFlagsUL[siAmr] =
         cpAmrST->crossResFlagsUL[siAmr];
   } /*Loop: swap antibiotic flags*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec07:
   ^   - return
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   return 0;

   memErr_fun07_sec07:;
      return 1;
} /*cp_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - sort an array of amrST structs by reference pos
   '   - Shell sort taken from:
   '     - Adam Drozdek. 2013. Data Structures and
   '       Algorithims in c++. Cengage Leraning. fourth
   '       edition. pages 505-508
   '     - i made some minor changes, but is mostly the
   '       same
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - find number of rounds to sort for
   '   o fun08 sec03:
   '     - sort the amrST array by reference position
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec01:
  ^   - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*get arrays to sort from the matrix (for sanity)*/
  
  /*variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec02:
  ^   - find max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  if(numElmUL <= 1)
     return; /*nothing to sort*/

  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun08 Sec03:
  ^   - sort amrST array by reference position
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*Loop: sub arrays sort the subarrays*/

    for(
       ulIndex = 0;
       ulIndex <= subUL;
      ++ulIndex
    ){ /*Loop: each element in the subarray*/

      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop: swap each nth element of the subarray*/

        nextElmUL = ulElm + subUL;
        
        if(
              amrAryST[ulElm].refPosUI
           > amrAryST[nextElmUL].refPosUI
        ){ /*If: need to swap an element*/
          swap_amrST(
             &amrAryST[ulElm],
             &amrAryST[nextElmUL]
          );
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*Loop: move swapped element back*/
            lastElmUL -= subUL;
            
            if(  amrAryST[elmOnUL].refPosUI
               > amrAryST[lastElmUL].refPosUI
            ) break; /*Positioned the element*/
            
             swap_amrST(
                &amrAryST[elmOnUL],
                &amrAryST[lastElmUL]
             );
            
            elmOnUL = lastElmUL;
          } /*Loop: move swapped element back*/
        } /*If: need to swap elements*/
      } /*Loop: swap each nth element of the subarray*/
    } /*Loop: each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*Move to the next round*/
  } /*Loop: through all sub arrays to sort the subarrays*/
} /*sortPos_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - sort amrST struct array by gene names (ids)
   '   - Shell sort taken from:
   '     - Adam Drozdek. 2013. Data Structures and
   '       Algorithims in c++. Cengage Leraning. fourth
   '       edition. pages 505-508
   '     - I made some minor changes, but is mostly the
   '       same
   '   o fun09 sec01:
   '     - variable declerations
   '   o fun09 sec02:
   '     - find the number of rounds to sort for
   '   o fun09 sec03:
   '     - sort the amrSTs by variant id
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun09 Sec01:
  ^   - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun09 Sec02:
  ^   - find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun09 Sec03:
  ^   - sort the amrSTs by variant id
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*Loop: all sub arrays sort the subarrays*/

    for(
       ulIndex = 0;
       ulIndex <= subUL;
       ++ulIndex
    ){ /*Loop: each element in the subarray*/

      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop: swap each nth element of the subarray*/
        nextElmUL = ulElm + subUL;
        
        if(
            eqlNull_ulCp(
                amrAryST[ulElm].geneIdStr,
                amrAryST[nextElmUL].geneIdStr
            ) > 0
        ){ /*If: need to swap an element*/
          swap_amrST(
             &amrAryST[ulElm],
             &amrAryST[nextElmUL]
          );
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*Loop: move swapped element back*/
            lastElmUL -= subUL;
            
            if(
                eqlNull_ulCp(
                   amrAryST[elmOnUL].geneIdStr,
                   amrAryST[lastElmUL].geneIdStr
                ) > 0
            ) break; /*positioned element*/
            
             swap_amrST(
                &amrAryST[elmOnUL],
                &amrAryST[lastElmUL]
             );
            
            elmOnUL = lastElmUL;
          } /*Loop: move swapped element back*/
        } /*If: need to swap elements*/
      } /*Loop: swap each nth element of the subarray*/
    } /*Loop: each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*move to the next round*/
  } /*Loop: all sub arrays to sort the subarrays*/
} /*sortGeneId_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - sort amrST struct array by variant id
   '   - Shell sort taken from:
   '     - Adam Drozdek. 2013. Data Structures and
   '       Algorithims in c++. Cengage Leraning. fourth
   '       edition. pages 505-508
   '     - I made some minor changes, but is mostly the
   '       same
   '   o fun10 sec01:
   '     - variable declerations
   '   o fun10 sec02:
   '     - find the number of rounds to sort for
   '   o fun10 sec03:
   '     - sort the amrSTs by variant id
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec01:
  ^   - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec02:
  ^   - find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun10 Sec03:
  ^   - sort the amrSTs by variant id
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*Loop: all sub arrays sort the subarrays*/

    for(
       ulIndex = 0;
       ulIndex <= subUL;
       ++ulIndex
    ){ /*Loop: each element in the subarray*/

      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop: swap each nth element of the subarray*/
        nextElmUL = ulElm + subUL;
        
        if(
            eqlNull_ulCp(
                amrAryST[ulElm].varIdStr,
                amrAryST[nextElmUL].varIdStr
            ) > 0
        ){ /*If: need to swap an element*/
          swap_amrST(
             &amrAryST[ulElm],
             &amrAryST[nextElmUL]
          );
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*Loop: move swapped element back*/
            lastElmUL -= subUL;
            
            if(
                eqlNull_ulCp(
                   amrAryST[elmOnUL].varIdStr,
                   amrAryST[lastElmUL].varIdStr
                ) > 0
            ) break; /*positioned element*/
            
             swap_amrST(
                &amrAryST[elmOnUL],
                &amrAryST[lastElmUL]
             );
            
            elmOnUL = lastElmUL;
          } /*Loop: move swapped element back*/
        } /*If: need to swap elements*/
      } /*Loop: swap each nth element of the subarray*/
    } /*Loop: each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*move to the next round*/
  } /*Loop: all sub arrays to sort the subarrays*/
} /*sortVarId_amrST*/

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
){
   int midSI = 0;
   int rightSI = numAmrSI - 1;
   int leftSI = 0;

   while(leftSI <= rightSI)
   { /*Loop: search for starting coordinate*/
      midSI = (leftSI + rightSI) >> 1;

     if(qryUI > amrAryST[midSI].refPosUI)
         leftSI = midSI + 1;

     else if(qryUI < amrAryST[midSI].refPosUI)
        rightSI = midSI - 1;

     else
     { /*Else: found query*/
        while(midSI > 0)
        { /*Loop: make sure on first amr*/
           if(amrAryST[midSI - 1].refPosUI < qryUI)
              break;

           --midSI;
        } /*Loop: make sure on first amr*/

        return midSI;
     } /*Else: found query*/
   } /*Loop: search for the starting coordinate*/

   if(midSI >= numAmrSI)
      return -1;

   /*make sure it is alwasy greater than*/
   midSI += (amrAryST[midSI].refPosUI < qryUI);

   if(midSI >= numAmrSI)
      return -1;

   while(midSI > 0)
   { /*Loop: make sure on first amr*/
      if(amrAryST[midSI - 1].refPosUI < qryUI)
         break;

      --midSI;
   } /*Loop: make sure on first amr*/

   /*make sure not at end of vector*/
   if(midSI >= numAmrSI)
      return -1;

   midSI += (amrAryST[midSI].refPosUI < qryUI);

   if(midSI >= numAmrSI)
      return -1;

   if(amrAryST[midSI].refPosUI < qryUI)
      return -1;

   return midSI;
} /*getAmr_amrST*/

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
){
   if(! refListSTPtr)
      return;
   refListSTPtr->refLenSI = 0;
   refListSTPtr->noRefBl = 0;
   refListSTPtr->drugCntSI = 0;
} /*blank_refList_amrST*/

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
){
   if(! refListSTPtr)
      return;

   refListSTPtr->amrsAryST = 0;
   refListSTPtr->amrLenArySI = 0;
   refListSTPtr->amrSizeArySI = 0;
   refListSTPtr->refAryStr = 0;
   refListSTPtr->refSizeSI = 0;

   refListSTPtr->drugAryStr = 0;
   refListSTPtr->drugSizeSI = 0;

   blank_refList_amrST(refListSTPtr);
} /*init_refList_amrST*/

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
){
   signed int siPos = 0;

   if(! refListSTPtr)
      return;

   for(siPos=0; siPos < refListSTPtr->refSizeSI; ++siPos)
   { /*Loop: free memory*/
      if(refListSTPtr->amrsAryST[siPos])
         freeHeapAry_amrST(
            refListSTPtr->amrsAryST[siPos],
            refListSTPtr->amrSizeArySI[siPos]
         );
      refListSTPtr->amrsAryST[siPos] = 0;

      if(refListSTPtr->refAryStr[siPos])
         free(refListSTPtr->refAryStr[siPos]);
      refListSTPtr->refAryStr[siPos] = 0;
   } /*Loop: free memory*/

   if(refListSTPtr->amrsAryST)
      free(refListSTPtr->amrsAryST);
   if(refListSTPtr->amrLenArySI)
      free(refListSTPtr->amrLenArySI);
   if(refListSTPtr->amrSizeArySI)
      free(refListSTPtr->amrSizeArySI);
   if(refListSTPtr->refAryStr)
      free(refListSTPtr->refAryStr);

   if(refListSTPtr->drugAryStr)
      free(refListSTPtr->drugAryStr);

   init_refList_amrST(refListSTPtr);
} /*freeStack_refList_amrST*/

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
){
   if(! refListSTPtr)
      return;
   freeStack_refList_amrST(refListSTPtr);
   free(refListSTPtr);
} /*freeHeap_refList_amrST*/

/*-------------------------------------------------------\
| Fun16: swap_refList_amrST
|   - swap to index's ina refList_amrST structure
| Input:
|   - firstSI:
|     o first index to swap
|   - secSI:
|     o second index to swap
|   - refListSTPtr:
|     o refList_amrST structure to swap index's
| Output:
|   - Modifies:
|     o the first index to have the second index values
|     o the second index to have the first index values
\-------------------------------------------------------*/
void
swap_refList_amrST(
   signed int firstSI,
   signed int secSI,
   struct refList_amrST *refListSTPtr
){
   struct amrST **amrSwapPtr = 0;
   signed int *intSwapPtr = 0;
   signed char **strSwapPtr = 0;

   amrSwapPtr = refListSTPtr[firstSI].amrsAryST;
   refListSTPtr[firstSI].amrsAryST =
      refListSTPtr[secSI].amrsAryST;
   refListSTPtr[secSI].amrsAryST = amrSwapPtr;

   intSwapPtr = refListSTPtr[firstSI].amrLenArySI;
   refListSTPtr[firstSI].amrLenArySI =
      refListSTPtr[secSI].amrLenArySI;
   refListSTPtr[secSI].amrLenArySI = intSwapPtr;

   intSwapPtr = refListSTPtr[firstSI].amrSizeArySI;
   refListSTPtr[firstSI].amrSizeArySI =
      refListSTPtr[secSI].amrSizeArySI;
   refListSTPtr[secSI].amrSizeArySI = intSwapPtr;

   strSwapPtr = refListSTPtr[firstSI].refAryStr;
   refListSTPtr[firstSI].refAryStr =
      refListSTPtr[secSI].refAryStr;
   refListSTPtr[secSI].refAryStr = strSwapPtr;
} /*swap_refList_amrST*/

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
){
   signed int leftSI = 0;
   signed int midSI = 0;
   signed int rightSI = refListSTPtr->refLenSI - 1;
   signed long cmpSL = 0;

   if(refListSTPtr->refLenSI >= 2)
      ; /*have references to search*/
   else if(refListSTPtr->noRefBl)
      return 0;
      /*every AMR belongs to the first reference*/
   else if(! refListSTPtr->refLenSI)
      return -0;

   while(leftSI >= rightSI)
   { /*Loop: find reference*/
      midSI = (leftSI + rightSI) >> 1;

      cmpSL =
         eqlNull_ulCp(
            refStr,
            refListSTPtr->refAryStr[midSI]
         );

      if(cmpSL > 0)
         leftSI = midSI + 1;  /*query is greater*/
      else if(cmpSL < 0)
         rightSI = midSI - 1; /*reference is greater*/
      else
         return midSI;
   } /*Loop: find reference*/

   if(cmpSL < 0)
      ; /*insert would be before the found item*/
   else if(midSI >= refListSTPtr->refLenSI - 1)
      ++midSI; /*insert is at end*/
   else
      ++midSI; /*can move index up one*/

   return midSI * -1;
} /*refFind_refList_amrST*/

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
){
   signed int
      indexSI=refFind_refList_amrST(refStr, refListSTPtr);

   if(indexSI < 0)
   { /*If: reference not in the reference list*/
      *amrLenSIPtr = indexSI;
      return 0;
   } /*If: reference not in the reference list*/

   else
   { /*Else: found the reference*/
      *amrLenSIPtr = refListSTPtr->amrLenArySI[indexSI];
      return refListSTPtr->amrsAryST[indexSI];
   } /*Else: found the reference*/
} /*getAmrList_refList_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - adds a reference to its correct possition in a
   '     refList_amrST structure
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - allocate memory for more pointers (if needed)
   '   o fun19 sec03:
   '     - find and insert reference
   '   o fun19 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define initSize_fun19 16
   signed int indexSI = 0;
   signed int posSI = 0;

   struct amrST **amrSTSwapPtr = 0;
   signed char **strSwapPtr = 0;
   signed int *siSwapPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - allocate memory for more pointers (if needed)
   ^   o fun19 sec02 sub01:
   ^     - deal with no memory (initial memory assign)
   ^   o fun19 sec02 sub02:
   ^     - add more memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec02 Sub01:
   *   - deal with no memory (initial memory assign)
   \*****************************************************/

   if(! refListSTPtr->refSizeSI)
   { /*If: need initial memory*/
      refListSTPtr->amrsAryST =
         calloc(initSize_fun19, sizeof(struct amrST *));
      if(! refListSTPtr->amrsAryST)
         goto memErr_fun19_sec04;

      refListSTPtr->amrLenArySI =
         calloc(initSize_fun19, sizeof(signed int));
      if(! refListSTPtr->amrLenArySI)
         goto memErr_fun19_sec04;

      refListSTPtr->amrSizeArySI =
         calloc(initSize_fun19, sizeof(signed int));
      if(! refListSTPtr->amrSizeArySI)
         goto memErr_fun19_sec04;

      refListSTPtr->refAryStr =
         calloc(initSize_fun19, sizeof(signed char *));
      if(! refListSTPtr->refAryStr)
         goto memErr_fun19_sec04;

      refListSTPtr->refSizeSI = initSize_fun19;
   } /*If: need initial memory*/

   /*****************************************************\
   * Fun19 Sec02 Sub02:
   *   - add more memory
   \*****************************************************/

   else if(
      refListSTPtr->refLenSI >= refListSTPtr->refSizeSI
   ){ /*Else If: need more memory*/
      posSI = refListSTPtr->refSizeSI;
      posSI += (refListSTPtr->refSizeSI >> 1);

      amrSTSwapPtr =
         realloc(
            refListSTPtr->amrsAryST,
            posSI * sizeof(struct amrST *)
         );
      if(! amrSTSwapPtr)
         goto memErr_fun19_sec04;
      refListSTPtr->amrsAryST = amrSTSwapPtr;

      siSwapPtr =
         realloc(
            refListSTPtr->amrLenArySI,
            posSI * sizeof(signed char *)
         );
      if(! siSwapPtr)
         goto memErr_fun19_sec04;
      refListSTPtr->amrLenArySI = siSwapPtr;

      siSwapPtr = realloc(
            refListSTPtr->amrSizeArySI,
            posSI * sizeof(signed char *)
         );
      if(! siSwapPtr)
         goto memErr_fun19_sec04;
      refListSTPtr->amrSizeArySI = siSwapPtr;

      strSwapPtr =
         realloc(
            refListSTPtr->refAryStr,
            posSI * sizeof(signed char *)
         );
      if(! strSwapPtr)
         goto memErr_fun19_sec04;
      refListSTPtr->refAryStr = strSwapPtr;

      refListSTPtr->refSizeSI = posSI;

      for(
         posSI = refListSTPtr->refLenSI;
         posSI < refListSTPtr->refSizeSI;
         ++posSI
      ){ /*Loop: initialize all values*/
         refListSTPtr->amrsAryST[posSI] = 0;
         refListSTPtr->amrLenArySI[posSI] = 0;
         refListSTPtr->amrSizeArySI[posSI] = 0;
         refListSTPtr->refAryStr[posSI] = 0;
      }  /*Loop: initialize all values*/

      posSI = 0;
   }  /*Else If: need more memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - find and insert reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*____________find_insert_position___________________*/
   if(! refStr || ! refStr[0])
      goto blankRef_fun19_sec04; /*nothing to insert*/
   else if(! refListSTPtr->refLenSI)
      indexSI = 0;
   else
   { /*Else If: need to find the index to insert at*/
      indexSI=refFind_refList_amrST(refStr, refListSTPtr);

      if(indexSI >= 0)
         goto done_fun19_sec04; /*nothing to insert*/
   } /*Else If: need to find the index to insert at*/

   /*____________move_old_items_up______________________*/
   if(refListSTPtr->refLenSI)
   { /*If: can insert and have at least one reference*/
      for(
         posSI = refListSTPtr->refLenSI;
         posSI >= indexSI;
         --posSI
      ) swap_refList_amrST(posSI, posSI -1, refListSTPtr);
   } /*If: can insert and have at least one reference*/

   ++refListSTPtr->refLenSI; /*account for new reference*/
   posSI = endStr_ulCp(refStr); /*get length for malloc*/

   /*____________add_reference_to_its_index_____________*/
   if(refListSTPtr->refAryStr[indexSI])
      free(refListSTPtr->refAryStr[indexSI]);
   refListSTPtr->refAryStr[indexSI] =
     malloc(
          ulAlign_64bit(posSI, sizeof(signed char))
        * sizeof(signed char)
     );
   if(! refListSTPtr->refAryStr[indexSI])
      goto memErr_fun19_sec04;
   cpLen_ulCp(
      refListSTPtr->refAryStr[indexSI],
      refStr,
      posSI
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun19_sec04:;
      return indexSI;
   blankRef_fun19_sec04:;
      return -1;
   memErr_fun19_sec04:;
      return -2;
} /*refAdd_refList_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - adds a amrST struct to the end of its amr list
   '   o fun20 sec01:
   '     - variable declarations
   '   o fun20 sec02:
   '     - find reference and allocate memory
   '   o fun20 sec03:
   '     - add the new amr entry in
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define initLen_fun20 16
   signed int indexSI = 0;   /*index of amr list*/
   signed int lastAmrSI = 0; /*index to put new amr at*/

   struct amrST *amrSwapSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec02:
   ^   - find reference and allocate memory
   ^   o fun20 sec02 sub01:
   ^     - find/add reference
   ^   o fun20 sec02 sub02:
   ^     - if have no memor for the amr array; setup
   ^       initial memory
   ^   o fun20 sec02 sub03:
   ^     - add more memory if needed
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun20 Sec02 Sub01:
   *   - find/add reference
   \*****************************************************/

   if(refListSTPtr->noRefBl)
   { /*If: not using the reference system*/
      indexSI = 0; /*not using the reference system*/

      if(refListSTPtr->refSizeSI == 0)
      { /*If: first time for this structure*/
         refListSTPtr->refLenSI = 1;

         refListSTPtr->amrsAryST =
            calloc(1, sizeof(struct amrST *));
         if(! refListSTPtr->amrsAryST)
            goto memErr_fun20_sec04;

         refListSTPtr->amrLenArySI =
            calloc(1, sizeof(signed int));
         if(! refListSTPtr->amrLenArySI)
            goto memErr_fun20_sec04;

         refListSTPtr->amrSizeArySI =
            calloc(1, sizeof(signed int));
         if(! refListSTPtr->amrSizeArySI)
            goto memErr_fun20_sec04;

         refListSTPtr->refAryStr =
            calloc(1, sizeof(signed char *));
         if(! refListSTPtr->refAryStr)
            goto memErr_fun20_sec04;

         refListSTPtr->refSizeSI = 1;
      } /*If: first time for this structure*/

      if(! refListSTPtr->refLenSI)
         refListSTPtr->refLenSI = 1;
   } /*If: not using the reference system*/

   else
   { /*Else: using a reference system*/
      refAdd_refList_amrST(refStr, refListSTPtr);

      if(! indexSI)
         ;
      else if(indexSI == -1)
         goto blankValue_fun20_sec04;
      else
         goto memErr_fun20_sec04;
   } /*Else: using a reference system*/

   /*****************************************************\
   * Fun20 Sec02 Sub02:
   *   - if have no memor for the amr array; setup initial
   *     memory
   \*****************************************************/

   if(! refListSTPtr->amrsAryST[indexSI])
   { /*If: no amrs have been added yet*/
      refListSTPtr->amrLenArySI[indexSI] = 0;

      refListSTPtr->amrsAryST[indexSI] =
         malloc(initLen_fun20 * sizeof(struct amrST));
      if(! refListSTPtr->amrsAryST[indexSI])
         goto memErr_fun20_sec04;

      for(
         lastAmrSI = 0;
         lastAmrSI < initLen_fun20;
         ++lastAmrSI
      ){ /*Loop: initialize the amr structs*/
         init_amrST(
            &refListSTPtr->amrsAryST[indexSI][lastAmrSI]
         );
      }  /*Loop: initialize the amr structs*/

      refListSTPtr->amrSizeArySI[indexSI]=initLen_fun20;
   } /*If: no amrs have been added yet*/

   /*****************************************************\
   * Fun20 Sec02 Sub03:
   *   - add more memory if needed
   \*****************************************************/

   else if(
         refListSTPtr->amrLenArySI[indexSI]
      >= refListSTPtr->amrSizeArySI[indexSI]
   ){ /*Else If: need more memory*/
      lastAmrSI = refListSTPtr->amrSizeArySI[indexSI];
      lastAmrSI += (lastAmrSI >> 1);

      amrSwapSTPtr =
         realloc(
            refListSTPtr->amrsAryST[indexSI],
            lastAmrSI * sizeof(struct amrST)
         );
      if(! amrSwapSTPtr)
         goto memErr_fun20_sec04;
      refListSTPtr->amrsAryST[indexSI] = amrSwapSTPtr;

      refListSTPtr->amrSizeArySI[indexSI] = lastAmrSI;

      for(
         lastAmrSI = refListSTPtr->amrLenArySI[indexSI];
         lastAmrSI < refListSTPtr->amrSizeArySI[indexSI];
         ++lastAmrSI
      ){ /*Loop: initialize the amr structs*/
         init_amrST(
            &refListSTPtr->amrsAryST[indexSI][lastAmrSI]
         );
      }  /*Loop: initialize the amr structs*/
   }  /*Else If: need more memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec03:
   ^   - add the new amr entry in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lastAmrSI = refListSTPtr->amrLenArySI[indexSI];

   if(
      cp_amrST(
         &refListSTPtr->amrsAryST[indexSI][lastAmrSI],
         amrSTPtr
      )
   ) goto memErr_fun20_sec04;
   ++refListSTPtr->amrLenArySI[indexSI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   blankValue_fun20_sec04:;
      return 1;
   memErr_fun20_sec04:;
      return 2;
} /*amrAdd_refList_amrST*/

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
){
   signed int siAmr = 0;

   for(siAmr = 0; siAmr < refSTPtr->refLenSI; ++siAmr)
   { /*Loop: sort all amr arrays*/
      if(refSTPtr->amrsAryST[siAmr])
         sortPos_amrST(
            refSTPtr->amrsAryST[siAmr],
            0,
            refSTPtr->refLenSI
         );
   } /*Loop: sort all amr arrays*/
} /*sortPos_refList_amrST*/

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
){
   signed int siAmr = 0;

   for(siAmr = 0; siAmr < refSTPtr->refLenSI; ++siAmr)
   { /*Loop: sort all amr arrays*/
      if(refSTPtr->amrsAryST[siAmr])
         sortGeneId_amrST(
            refSTPtr->amrsAryST[siAmr],
            0,
            refSTPtr->refLenSI
         );
   } /*Loop: sort all amr arrays*/
} /*sortGeneId_refList_amrST*/

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
){
   signed int siAmr = 0;

   for(siAmr = 0; siAmr < refSTPtr->refLenSI; ++siAmr)
   { /*Loop: sort all amr arrays*/
      if(refSTPtr->amrsAryST[siAmr])
         sortVarId_amrST(
            refSTPtr->amrsAryST[siAmr],
            0,
            refSTPtr->refLenSI
         );
   } /*Loop: sort all amr arrays*/
} /*sortVarId_refList_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun24 TOC: p_amrST
   '   - print out amr database used
   '   o fun24 sec01:
   '     - variable declerations
   '   o fun24 sec02:
   '     - open output file
   '   o fun24 sec03:
   '     - print header
   '   o fun24 sec04:
   '     - print AMRs
   '   o fun24 sec05:
   '     - close file and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siAmr = 0; /*looping through refSTPtr*/

   signed int siDrug = 0;   /*looping through drugAryStr*/
   signed char *drugStr = 0;/*printing out the drug id's*/

   unsigned long ulRes = 0; /*checking amr's resitance*/
   unsigned long ulCross = 0;
      /*check amr's cross resitance*/

   signed int siAmrSet = 0;  /*flag set on*/
   signed int siFlag = 0;    /*flag on in a set*/
   signed int siDrugOn = 0;  /*drug on*/
   signed char resBl = 0;

   signed char *tmpStr = 0;
   struct amrST *amrSTAry = 0;
   signed char siRef = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec02:
   ^   - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outStr)
      outFILE = stdout;

   else if(*outStr == '-')
      outFILE = stdout;
   else
      outFILE = fopen((char *) outStr, "w");

   if(outFILE == 0)
      return def_fileErr_amrST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec03:
   ^   - print header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! refSTPtr->noRefBl)
      fprintf((FILE *) outFILE, "ref\t");

   fprintf(outFILE,"gene\tvariantID\trefPosition");
   fprintf(outFILE,"\tdirection\tmutation\tframeshift");
   fprintf(outFILE,"\trefSeq\tamrSeq\tcodonRefPos");
   fprintf(outFILE,"\tcodonNumber\tlastCodon\trefAA");
   fprintf(outFILE, "\tamrAA\tgeneStart\tgeneEnd");
   fprintf(outFILE, "\thighResistance\tlowResitance");
   fprintf(outFILE, "\tadditiveResitance\tneedsGene");

   for(siDrug = 0; siDrug < refSTPtr->drugCntSI; ++siDrug)
   { /*Loop: print antibiotic names*/
      drugStr = get_drugAry(refSTPtr->drugAryStr, siDrug);
      fprintf(outFILE, "\t%s", drugStr);
   } /*Loop: print antibiotic names*/

   fprintf(outFILE,"\tendAntibiotics\teffect\tcomment");
   fprintf(outFILE, "\tgrade\tentireGene\tunkownEntry");
   fprintf(outFILE, "%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec04:
   ^   - print AMRs
   ^   o fun24 sec04 sub01:
   ^     - start loop; print ids & reference position
   ^   o fun24 sec04 sub02:
   ^     - print direction and AMR type/sequence
   ^   o fun24 sec04 sub03:
   ^     - print amino acid sequence and coordinates
   ^   o fun24 sec04 sub04:
   ^     - print gene coordinates
   ^   o fun24 sec04 sub05:
   ^     - print resistance level/if additive
   ^   o fun24 sec04 sub06:
   ^     - print drugs resistant to
   ^   o fun24 sec04 sub07:
   ^     - print effect entry
   ^   o fun24 sec04 sub08:
   ^     - print comment entry
   ^   o fun24 sec04 sub09:
   ^     - print grade (main drug), if effect appies to
   ^       entire gene, and if unknown entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun24 Sec04 Sub01:
   *   - start loop; print ids and reference position
   \*****************************************************/

   for(siRef = 0; siRef < refSTPtr->refLenSI; ++siRef)
   { /*Loop: print out each reference*/
      amrSTAry = refSTPtr->amrsAryST[siRef];

      for(
         siAmr = 0;
         siAmr < refSTPtr->amrLenArySI[siRef];
         ++siAmr
      ){ /*Loop: print out each  amr*/
         if(! refSTPtr->noRefBl)
         { /*If: have a reference column*/
            fprintf(
               outFILE,
               "%s\t",
               refSTPtr->refAryStr[siRef]
            );
         } /*If: have a reference column*/

         fprintf(
            outFILE,
            "%s\t%s\t%u",
            amrSTAry[siAmr].geneIdStr,   /*gene name*/
            amrSTAry[siAmr].varIdStr,    /*variant name*/
            amrSTAry[siAmr].refPosUI + 1 /*ref position*/
         ); /*print first few entries*/

         /***********************************************\
         * Fun24 Sec04 Sub02:
         *   - print direction and AMR type/sequence
         \***********************************************/

         /*print reading frame of the gene*/
         if(
               amrSTAry[siAmr].dirFlagSC
            == def_forwardDir_amrST
         ) fprintf(outFILE, "\tF");

         else if(
               amrSTAry[siAmr].dirFlagSC
            == def_revCompDir_amrST
         ) fprintf(outFILE, "\tR");

         else
            fprintf(outFILE, "\tNA");

         /*print mutation type,if frame shift, sequences*/
         fprintf(
            outFILE,
            "\t%s\t%u\t%s\t%s",
            amrSTAry[siAmr].mutTypeStr,
            amrSTAry[siAmr].frameshiftBl,
            amrSTAry[siAmr].refSeqStr,
            amrSTAry[siAmr].amrSeqStr
         );

         /***********************************************\
         * Fun24 Sec04 Sub03:
         *   - print amino acid sequence and coordinates
         \***********************************************/

         if(
            amrSTAry[siAmr].refAaStr != 0 &&
            amrSTAry[siAmr].refAaStr[0] > 0 &&
            amrSTAry[siAmr].refAaStr[0] != '0'
         ){ /*If: had amino acid change*/
            fprintf(
               outFILE,
               "\t%u\t%u\t%u",
               amrSTAry[siAmr].codonPosUI + 1,
               amrSTAry[siAmr].codonNumUI,
               amrSTAry[siAmr].endCodonNumUI
            );

            /*check if have reference aa sequence*/
            if(amrSTAry[siAmr].refAaStr != 0)
               fprintf(
                   outFILE,
                   "\t%s",
                   amrSTAry[siAmr].refAaStr
               );

            else
               fprintf(outFILE, "\t0");

            /*check if have amr aa sequence*/
            if(amrSTAry[siAmr].amrAaStr != 0)
               fprintf(
                   outFILE,
                   "\t%s",
                   amrSTAry[siAmr].amrAaStr
               );

            else
               fprintf(outFILE, "\t0");
         } /*If: had an amino acid change*/

         else
            fprintf(outFILE, "\tNA\tNA\tNA\t0\t0");

         /***********************************************\
         * Fun24 Sec04 Sub04:
         *   - print gene coordinates
         \***********************************************/

         /*print gene position*/
         if(amrSTAry[siAmr].geneLastRefUI > 0)
            fprintf(
               outFILE,
               "\t%u\t%u",
               amrSTAry[siAmr].geneFirstRefUI + 1,
               amrSTAry[siAmr].geneLastRefUI + 1
            );

         else
            fprintf(outFILE, "\tNA\tNA");
               
         /***********************************************\
         * Fun24 Sec04 Sub05:
         *   - print resistance level/if additive
         \***********************************************/

         fprintf(
            outFILE,
           "\t%i\t%i\t%i",
           amrSTAry[siAmr].highResBl,
           amrSTAry[siAmr].lowResBl,
           amrSTAry[siAmr].additiveResBl
         ); /*print who comment parsed data*/
            
         /*should not need to do this, but for some
         `    odd reason I get blanks/(null) if I do not
         */
         if(! amrSTAry[siAmr].needsGeneStr)
            fprintf(outFILE, "\tNA");
         else if(amrSTAry[siAmr].needsGeneStr[0] < 32)
            fprintf(outFILE, "\tNA");
         else
           fprintf(
              outFILE,
              "\t%s",
              amrSTAry[siAmr].needsGeneStr
           );

         /***********************************************\
         * Fun24 Sec04 Sub06:
         *   - print drugs resistant to
         \***********************************************/

         siDrugOn = 0;

         for(
            siAmrSet = 0;
            siAmrSet
               < (signed int)
                 (
                     1 + refSTPtr->drugCntSI
                   / (sizeof(unsigned long) << 3)
                 );
            ++siAmrSet
         ){ /*Loop: print AMR's resitant to*/
            ulRes = amrSTAry[siAmr].amrFlagsUL[siAmrSet];

            ulCross =
               amrSTAry[siAmr].crossResFlagsUL[siAmrSet];

            for(
               siFlag=0;
               siFlag < (signed int) def_maxFlags_amrST;
               ++siFlag
            ){ /*Loop: print antibiotic resitance*/
               /*check if buffer was completely filled*/
               ++siDrugOn;
               if(siDrugOn > refSTPtr->drugCntSI)
                  break;

               /*add resitance entry*/
               resBl =
                  (signed char)
                  ( (-(ulRes & 1) ) & def_resFlag_amrST );

               /*add cross resistance entry*/
               resBl |=
                  (signed char)
                  (
                      ( -(ulCross & 1) )
                    & def_crossResFlag_amrST
                  );
               /*Logic (both lines)
               `  - ulRes & 1 or ulCross & 1:
               `    o 1 if there is resitance (bit set)
               `    o 0 if there is no resitance
               `  - -(ulRes & 1) or -(ulCross & 1):
               `    o -1 if have resitance (all bits set)
               `    o 0 if there is no resitance
               `  - -(ulRes & 1) & def_resFlag:_amrST
               `    o Changes -1 to def_resFlag_amrST
               `      - for cross resitance I use the
               `        def_crossResFlag_amrST
               `    o Stays 0 if there is no resitance
               `  - resBl |=
               `    o Meges the two (restiance and cross
               `      resitance) result together
               */

               fprintf(outFILE, "\t%i", resBl);

               /*move to next antibiotic flag*/
               ulRes >>= 1;
               ulCross >>= 1;
            } /*Loop: print antibiotic restance*/

            /*mark end of antibiotics*/
            fprintf(outFILE, "\t*");

            /********************************************\
            * Fun24 Sec04 Sub07:
            *   - print effect entry
            \********************************************/

            if(! amrSTAry[siAmr].effectStr)
               fprintf(outFILE, "\tNA");
            else
            { /*Else: may have effect entry*/

               /*for some odd reason there are some blank
               `  effect entries in the catalog that
               `  escape me
               */
               tmpStr = amrSTAry[siAmr].effectStr;

               while(*tmpStr && *tmpStr < 33)
                  ++tmpStr; /*move past white space*/

               if(*tmpStr == '\0')
                  fprintf(outFILE, "\tNA");

               else
                  fprintf(
                     outFILE,
                     "\t%s",
                     amrSTAry[siAmr].effectStr
                  );
            } /*Else: may have effect entry*/
 
            /********************************************\
            * Fun24 Sec04 Sub08:
            *   - print comment entry
            \********************************************/

            if(! amrSTAry[siAmr].commentStr)
               fprintf(outFILE, "\tNA");
            else
            { /*Else: may have comment entry*/
               /*for some odd reason there are some blank
               `  comment entries in the catalog that
               `  escape me
               */
               tmpStr = amrSTAry[siAmr].commentStr;

               while(*tmpStr && *tmpStr < 33)
                  ++tmpStr; /*move past white space*/

               if(*tmpStr == '\0')
                  fprintf(outFILE, "\tNA");

               else
                  fprintf(
                     outFILE,
                     "\t%s",
                     amrSTAry[siAmr].commentStr
                  );
            } /*Else: may have comment entry*/
 
            /********************************************\
            * Fun24 Sec04 Sub09:
            *   - print grade (main drug), if effect
            *     applies to entire gene, and if unknown
            *     entry
            \********************************************/

            fprintf(
               outFILE,
               "\t%i\t%i\t%i",
               amrSTAry[siAmr].gradeSC,
               amrSTAry[siAmr].wholeGeneFlagSC,
               amrSTAry[siAmr].unknownBl
            ); /*print out the final columns*/

            fprintf(outFILE, "%s", str_endLine);
         } /*Loop: print AMR's resitant to*/
      } /*Loop: print each  amr*/
   } /*Loop: print out each reference*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec05:
   ^   - close file and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outFILE != stdout)
      fclose(outFILE);

   return 0;
} /*p_amrST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun25 TOC: readTbl_amrST
   '   - gets amr data from tbAmr tsv (p_amrST; fun24)
   '   o fun25 sec01:
   '     - variable declarations
   '   o fun25 sec02:
   '     - process header
   '   o fun25 sec03:
   '     - process header and get number of lines
   '   o fun25 sec04:
   '     - prepare buffers for extracting AMRs
   '   o fun25 sec05:
   '     - extract information from the file
   '   o fun25 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int lenBuffUI = 1 << 11; /*around 4096 bytes*/
   signed char *buffHeapStr = 0;
      /*buffer to read file into*/
   signed char *tmpStr = 0;
      /*for temporary operations*/

   signed long maxLineLenSL = 0;

   signed int siEndDrug = 0;  /*mark end of drug entry*/
   signed int siStartDrug = 0;/*mark start of drug entry*/
   signed int siDrug = 0;     /*iterator for loops*/

   struct amrST amrStackST;   /*for adding amrs*/
   signed char refIdStr[128]; /*name of reference*/

   struct refList_amrST *refHeapSTPtr = 0; /*return*/

   unsigned long *drugResUL = 0;
   unsigned long *drugCrossResUL = 0;

   FILE *amrFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec02:
   ^   - check if file eixists
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_amrST(&amrStackST);
   amrFILE = fopen((char *) tbAmrTblStr, "r");
   if(amrFILE == 0)
      goto fileErr_fun25_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec03:
   ^   - process header and get number of lines
   ^   o fun25 sec03 sub01:
   ^     - get number of lines in file
   ^   o fun25 sec03 sub02:
   ^     - read in header
   ^   o fun25 sec03 sub03:
   ^     - get number of antibiotics
   ^   o fun25 sec03 sub04:
   ^     - copy the antibiotics to the drug array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec03 Sub01:
   *   - find the longest line in the file
   \*****************************************************/

   if(! lineCnt_fileFun(amrFILE, &maxLineLenSL) )
      goto fileErr_fun25_sec06; /*no AMRs in file*/
   lenBuffUI = (unsigned int) maxLineLenSL + 3;

   buffHeapStr =
      malloc((lenBuffUI + 9) * sizeof(signed char));
   if(buffHeapStr == 0)
      goto memErr_fun25_sec06;

   refHeapSTPtr = malloc(sizeof(refList_amrST));
   if(! refHeapSTPtr)
       goto memErr_fun25_sec06;
   init_refList_amrST(refHeapSTPtr);

   /*****************************************************\
   * Fun25 Sec03 Sub02:
   *   - read in header
   \*****************************************************/

   tmpStr =
      (signed char *)
      fgets((char *) buffHeapStr, lenBuffUI, amrFILE);
      /*file set to start by lineCnt_fileFun*/

   if(! tmpStr)
      goto fileErr_fun25_sec06;
  
   /*****************************************************\
   * Fun25 Sec03 Sub03:
   *   - get number of antibiotics
   \*****************************************************/

   /*detect if database includes a reference sequence*/
   if(
          (buffHeapStr[0] | 32) == 'r'
       && (buffHeapStr[1] | 32) == 'e'
       && (buffHeapStr[2] | 32) == 'f'
       && buffHeapStr[3] < 32
   ) refHeapSTPtr->noRefBl = 0;

   else
      refHeapSTPtr->noRefBl = 1;

   siStartDrug = 0; /*gets moved to first drug column*/
   for(
      siDrug = 0;
      siDrug < def_firstDrugCol_amrST
               + (! refHeapSTPtr->noRefBl);
      ++siDrug
   ){ /*Loop: get to drug resistance columns*/
      while(buffHeapStr[siStartDrug++] > 31)
         ;
   }  /*Loop: get to drug resistance columns*/

   siEndDrug = siStartDrug;

   while(
      eqlWhite_ulCp(
         &buffHeapStr[siEndDrug],
         (signed char *) "endAntibiotics"
      )
   ){ /*Loop: find end of antibiotics columns*/
      ++refHeapSTPtr->drugSizeSI;
      siEndDrug += endWhite_ulCp(&buffHeapStr[siEndDrug]);

      while(buffHeapStr[siEndDrug] < 33)
      { /*Loop: find the next drug name*/
         if(! buffHeapStr[siEndDrug])
            goto fileErr_fun25_sec06;
         ++siEndDrug;
      } /*Loop: find the next drug name*/
   } /*Loop: find end of antibiotics columns*/

   /*****************************************************\
   * Fun25 Sec03 Sub04:
   *   - copy antibiotics to drug array
   \*****************************************************/

   refHeapSTPtr->drugAryStr =
      alloc_drugAry(refHeapSTPtr->drugSizeSI);
   if(! refHeapSTPtr->drugAryStr)
      goto memErr_fun25_sec06;

   refHeapSTPtr->drugCntSI = 0;

   while(
      refHeapSTPtr->drugCntSI < refHeapSTPtr->drugSizeSI
   ){ /*Loop: read in antibiotic entries*/
      siStartDrug +=
         cpDrug_drugAry(
            refHeapSTPtr->drugAryStr, 
            &buffHeapStr[siStartDrug],
            refHeapSTPtr->drugCntSI,
            -1 /*any white space*/
         ); /*copy drug to drug array*/

      while(buffHeapStr[siStartDrug] < 33)
         ++siStartDrug; /*move to the next drug*/
      ++refHeapSTPtr->drugCntSI;
   } /*Loop: read in antibiotic entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec05:
   ^   - extract information from file
   ^   o fun25 sec05 sub02:
   ^     - get first entry, start loop, & initialize
   ^   o fun25 sec05 sub02:
   ^     - get reference id if have reference column
   ^   o fun25 sec05 sub03:
   ^     - read in gene id
   ^   o fun25 sec05 sub04:
   ^     - read in variaint id
   ^   o fun25 sec05 sub05:
   ^     - read in refernce positon
   ^   o fun25 sec05 sub06:
   ^     - read in direction
   ^   o fun25 sec05 sub07:
   ^     - read in mutation type
   ^   o fun25 sec05 sub08:
   ^     - read in frame shift entry
   ^   o fun25 sec05 sub09:
   ^     - read in reference sequence
   ^   o fun25 sec05 sub10:
   ^     - read in amr sequence
   ^   o fun25 sec05 sub11:
   ^     - read in frist codon base in reference
   ^   o fun25 sec05 sub12:
   ^     - read in starting codon number
   ^   o fun25 sec05 sub13:
   ^     - read in ending codon number
   ^   o fun25 sec05 sub14:
   ^     - read in reference amino acid sequence
   ^   o fun25 sec05 sub15:
   ^     - read in amr amino acid sequence
   ^   o fun25 sec05 sub16:
   ^     - read in gene starting position
   ^   o fun25 sec05 sub17:
   ^     - read in gene ending position
   ^   o fun25 sec05 sub18:
   ^     - check if is a high resistance gene
   ^   o fun25 sec05 sub19:
   ^     - check if is a low resistance gene
   ^   o fun25 sec05 sub20:
   ^     - check if resistance is additive
   ^   o fun25 sec05 sub21:
   ^     - read in "if it needs a functional gene"
   ^   o fun25 sec05 sub22:
   ^     - read in antibiotic flags
   ^   o fun25 sec05 sub23:
   ^     - read in effect entry
   ^   o fun25 sec05 sub24:
   ^     - read in comment entry
   ^   o fun25 sec05 sub25:
   ^     - read in grade entry
   ^   o fun25 sec05 sub26:
   ^     - read in "if entry effects the entire gene"
   ^   o fun25 sec05 sub27:
   ^     - read in unkown type entry
   ^   o fun25 sec05 sub28:
   ^     - get the next line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec05 Sub01:
   *   - get first entry, Start read in loop, & initailze
   \*****************************************************/

   while(fgets((char *) buffHeapStr, lenBuffUI, amrFILE))
   { /*Loop: read in file*/

      freeStack_amrST(&amrStackST);
      tmpStr = buffHeapStr;

      /**************************************************\
      * Fun25 Sec05 Sub02:
      *   - get reference id if have reference column
      \**************************************************/

      if(! refHeapSTPtr->noRefBl)
      { /*If: reference column was used*/
         tmpStr += cpWhite_ulCp(refIdStr, tmpStr);

         while(*tmpStr < 33)
         { /*Loop: get past white space*/
            if(! *tmpStr)
               goto fileErr_fun25_sec06;
            ++tmpStr;
         } /*Loop: get past white space*/
      } /*If: reference column was used*/

      /**************************************************\
      * Fun25 Sec05 Sub03:
      *   - read in gene id
      \**************************************************/

      amrStackST.lenGeneIdUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenGeneIdUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenGeneIdUI] != ' ')
         goto fileErr_fun25_sec06; /*end of line*/

      amrStackST.geneIdStr =
         malloc(
              (amrStackST.lenGeneIdUI + 1) * sizeof(char)
         ); /*Allocate memory for the gene name*/

      if(! amrStackST.geneIdStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.geneIdStr,
         tmpStr,
         amrStackST.lenGeneIdUI
      );

      tmpStr += amrStackST.lenGeneIdUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub04:
      *   - read in variant id
      \**************************************************/

      amrStackST.lenVarIdUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenVarIdUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenVarIdUI] != ' ')
         goto fileErr_fun25_sec06; /*end of line*/

      amrStackST.varIdStr =
         malloc(
              (amrStackST.lenVarIdUI + 1) * sizeof(char)
         ); /*Allocate memory for the variant id*/

      if(! amrStackST.varIdStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.varIdStr,
         tmpStr,
         amrStackST.lenVarIdUI
      );

      tmpStr += amrStackST.lenVarIdUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub05:
      *   - read in refernce positon
      \**************************************************/

      tmpStr +=
         strToUI_base10str(tmpStr, &amrStackST.refPosUI);
      --amrStackST.refPosUI; /*convert to index 0*/

      if(*tmpStr == ' ')
         ;
      else if(*tmpStr == '\t')
         ;
      else
         goto fileErr_fun25_sec06;
         /*end of line or invalid character*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub06:
      *   - read in direction
      \**************************************************/

      if( (*tmpStr & ~32) == 'F' ) 
         amrStackST.dirFlagSC = def_forwardDir_amrST;
      else if( (*tmpStr & ~32) == 'R' ) 
         amrStackST.dirFlagSC = def_revCompDir_amrST;
      else
         amrStackST.dirFlagSC = def_unkownDir_amrST;
     
      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub07:
      *   - read in the mutation type
      \**************************************************/

      if(*tmpStr == '\t')
      { /*If: blank entry*/
         amrStackST.mutTypeStr[0] = 'N';
         amrStackST.mutTypeStr[1] = 'A';
         amrStackST.mutTypeStr[2] = '\0';

         goto endMut_fuh11_sec05_sub06;
      } /*If: blank entry*/

      amrStackST.mutTypeStr[0] = *tmpStr++;

      if(*tmpStr == '\t')
      { /*If: short entry*/
         amrStackST.mutTypeStr[1] = '\0';
         goto endMut_fuh11_sec05_sub06;
      } /*If: short entry*/

      amrStackST.mutTypeStr[1] = *tmpStr++;

      if(*tmpStr == '\t')
      { /*If: short entry*/
         amrStackST.mutTypeStr[2] = '\0';
         goto endMut_fuh11_sec05_sub06;
      } /*If: short entry*/

      /*full length entry*/
      amrStackST.mutTypeStr[2] = *tmpStr++;
      amrStackST.mutTypeStr[3] = '\0';

      /*move to next entry*/
      endMut_fuh11_sec05_sub06:;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub08:
      *   - read in frame shift entry
      \**************************************************/

      amrStackST.frameshiftBl = (*tmpStr == '1');
      
      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub09:
      *   - read in reference sequence
      \**************************************************/

      amrStackST.lenRefSeqUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenRefSeqUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenRefSeqUI] != ' ')
         goto fileErr_fun25_sec06; /*end of line*/

      amrStackST.refSeqStr =
         malloc(
            (amrStackST.lenRefSeqUI + 1) * sizeof(char)
         );

      if(! amrStackST.refSeqStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.refSeqStr,
         tmpStr,
         amrStackST.lenRefSeqUI
      );

      tmpStr += amrStackST.lenRefSeqUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub10:
      *   - read in amr sequence
      \**************************************************/

      amrStackST.lenAmrSeqUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenAmrSeqUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenAmrSeqUI] != ' ')
         goto fileErr_fun25_sec06; /*end of line*/

      amrStackST.amrSeqStr =
         malloc(
            (amrStackST.lenAmrSeqUI + 1) * sizeof(char)
         );

      if(! amrStackST.amrSeqStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.amrSeqStr,
         tmpStr,
         amrStackST.lenAmrSeqUI
      );

      tmpStr += amrStackST.lenAmrSeqUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub11:
      *   - read in frist codon base in reference
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrStackST.codonPosUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;
      } /*If: is mising*/

      else
      { /*Else: have number*/
         ++amrStackST.codonPosUI; /*to index 1*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrStackST.codonPosUI
            );
         --amrStackST.codonPosUI; /*to index 0*/

         if(*tmpStr == '\t')
            ;
         else if(*tmpStr != ' ')
            goto fileErr_fun25_sec06;/*non-numeric*/

         ++tmpStr;
      } /*Else: have number*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub12:
      *   - read in starting codon number
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrStackST.codonNumUI = 0;
         while(*tmpStr > 32)
            ++tmpStr;
      } /*If: is mising*/

      else
      { /*Else: have number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrStackST.codonNumUI
            );
         if(*tmpStr == '\t')
            ;
         else if(*tmpStr != ' ')
            goto fileErr_fun25_sec06;

         ++tmpStr;
      } /*Else: have number*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub13:
      *   - read in ending codon number
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrStackST.endCodonNumUI = 0;
         while(*tmpStr > 32)
            ++tmpStr;
      } /*If: is mising*/

      else
      { /*Else: have number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrStackST.endCodonNumUI
            );
         if(*tmpStr == '\t')
            ;
         else if(*tmpStr != ' ')
            goto fileErr_fun25_sec06;

         ++tmpStr;
      } /*Else: have number*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub14:
      *   - read in reference amino acid sequence
      \**************************************************/

      amrStackST.lenRefAaUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenRefAaUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenRefAaUI] != ' ')
         goto fileErr_fun25_sec06; /*end of line*/

      amrStackST.refAaStr =
         malloc(
            (amrStackST.lenRefAaUI + 1) * sizeof(char)
         );
   
      if(! amrStackST.refAaStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.refAaStr,
         tmpStr,
         amrStackST.lenRefAaUI
      );
      tmpStr += amrStackST.lenRefAaUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub15:
      *   - read in the amr amino acid sequence
      \**************************************************/

      amrStackST.lenAmrAaUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenAmrAaUI] == '\t')
          ;
      else if(tmpStr[amrStackST.lenAmrAaUI] != ' ')
         goto fileErr_fun25_sec06;/*end of line*/

      amrStackST.amrAaStr =
         malloc(
            (amrStackST.lenAmrAaUI + 1) * sizeof(char)
         );
      if(! amrStackST.amrAaStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.amrAaStr,
         tmpStr,
         amrStackST.lenAmrAaUI
      );
      tmpStr += amrStackST.lenAmrAaUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/
         
      /**************************************************\
      * Fun25 Sec05 Sub16:
      *   - read in gene starting position
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: this is mising*/
         amrStackST.geneFirstRefUI = 0;
         while(*tmpStr > 32)
            ++tmpStr;
      } /*If: this is mising*/

      else
      { /*Else: I have a number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrStackST.geneFirstRefUI
            );
         if(*tmpStr == '\t')
            ;
         else if(*tmpStr != ' ')
            goto fileErr_fun25_sec06;

         --amrStackST.geneFirstRefUI; /*to index 0*/
         ++tmpStr;
      } /*Else: I have a number*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub17:
      *   - read in gene ending position
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: this is mising*/
         amrStackST.geneLastRefUI = 0;
         while(*tmpStr > 32)
            ++tmpStr;
      } /*If: this is mising*/

      else
      { /*Else: I have a number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrStackST.geneLastRefUI
            );
         if(*tmpStr == '\t')
            ;
         else if(*tmpStr != ' ')
            goto fileErr_fun25_sec06;

         --amrStackST.geneLastRefUI; /*to index 0*/
         ++tmpStr;
      } /*Else: I have a number*/

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub18:
      *   - check if is a high resistance gene
      \**************************************************/
      
      amrStackST.highResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub19:
      *   - check if is a low resistance gene
      \**************************************************/
      
      amrStackST.lowResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub20:
      *   - check if resistance is additive
      \**************************************************/
      
      amrStackST.additiveResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub21:
      *   - Read in "if it needs a functional gene"
      \**************************************************/

      amrStackST.lenNeedsGeneUI = endWhite_ulCp(tmpStr);
      if(tmpStr[amrStackST.lenNeedsGeneUI] =='\t')
         ;
      else if(
         tmpStr[amrStackST.lenNeedsGeneUI] != ' '
      ) goto fileErr_fun25_sec06;/*end of line*/

      amrStackST.needsGeneStr =
         malloc(
            (amrStackST.lenNeedsGeneUI + 1) * sizeof(char)
         );
      if(! amrStackST.needsGeneStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.needsGeneStr,
         tmpStr,
         amrStackST.lenNeedsGeneUI
      );
      tmpStr += amrStackST.lenNeedsGeneUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub22:
      *   - read in antibiotic flags
      \**************************************************/

      siDrug = 0;
      drugResUL = amrStackST.amrFlagsUL;
      drugCrossResUL = amrStackST.crossResFlagsUL;

      while(*tmpStr != '*')
      { /*Loop: check for antibiotic resistance*/
         if(siDrug >= (signed int) def_maxFlags_amrST)
         { /*If: I need to start a new array*/
            siDrug = 0;
            ++drugResUL;
            ++drugCrossResUL;
         } /*If: I need to start a new array*/

         *drugResUL |=
              ((*tmpStr - 48) & def_resFlag_amrST)
            << siDrug;

         *drugCrossResUL |=
               ((
                  (*tmpStr - 48) & def_crossResFlag_amrST
                ) >> def_crossToResShift_amrST
               ) << siDrug;
         ++siDrug;

         while(*tmpStr > 32)
            ++tmpStr;
         while(*tmpStr < 33)
         { /*Loop: get past white space*/
            if(! *tmpStr)
               goto fileErr_fun25_sec06;
            ++tmpStr;
         } /*Loop: get past white space*/
      } /*Loop: check for antibiotic resistance*/
     
      while(*tmpStr > 32)
         ++tmpStr;
      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub23:
      *   - read in effect entry
      \**************************************************/

      amrStackST.lenEffectUI = endWhite_ulCp(tmpStr);

      if(tmpStr[amrStackST.lenEffectUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenEffectUI] != ' ')
         goto fileErr_fun25_sec06;/*end of line*/

      amrStackST.effectStr =
         malloc(
              (amrStackST.lenEffectUI + 1)
            * sizeof(signed char)
         );
      if(! amrStackST.effectStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.effectStr,
         tmpStr,
         amrStackST.lenEffectUI
      );
      tmpStr += amrStackST.lenEffectUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub24:
      *   - read in comment entry
      \**************************************************/

      amrStackST.lenCommentUI = 0;

      while(tmpStr[amrStackST.lenCommentUI] > 32)
         ++amrStackST.lenCommentUI;

      if(tmpStr[amrStackST.lenCommentUI] == '\t')
         ;
      else if(tmpStr[amrStackST.lenCommentUI] != ' ')
         goto fileErr_fun25_sec06;/*end of line*/

      amrStackST.commentStr =
         malloc(
            (amrStackST.lenCommentUI + 1) * sizeof(char)
         );

      if(! amrStackST.commentStr)
         goto memErr_fun25_sec06;

      cpLen_ulCp(
         amrStackST.commentStr,
         tmpStr,
         amrStackST.lenCommentUI
      );
      tmpStr += amrStackST.lenCommentUI + 1;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub25:
      *   - read in grade entry
      \**************************************************/

      amrStackST.gradeSC = *tmpStr - 48;

      while(*tmpStr > 32)
         ++tmpStr;
      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub26:
      *   - Read in if the entry effects the entire gene
      \**************************************************/

      amrStackST.wholeGeneFlagSC = *tmpStr - 48;

      while(*tmpStr > 32)
         ++tmpStr;
      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         if(! *tmpStr)
            goto fileErr_fun25_sec06;/*end of line*/
         ++tmpStr;
      } /*Loop: get past white space*/

      /**************************************************\
      * Fun25 Sec05 Sub27:
      *   - read in unkown type entry
      \**************************************************/

      amrStackST.unknownBl = *tmpStr - 48;

      /**************************************************\
      * Fun25 Sec05 Sub28:
      *   - get next line
      \**************************************************/

      if(
         amrAdd_refList_amrST(
            &amrStackST,
            refIdStr,
            refHeapSTPtr
         )
      ) goto memErr_fun25_sec06; /*memory error*/
   } /*Loop: read in file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec06:
   ^   - clean up and return
   ^   o fun25 sec06 sub01:
   ^     - no error clean up
   ^   o fun25 sec06 sub02:
   ^     - memory error clean up
   ^   o fun25 sec06 sub03:
   ^     - file error clean up
   ^   o fun25 sec06 sub04:
   ^     - error clean up (all errors jump to)
   ^   o fun25 sec06 sub05:
   ^     - general clean up (everything jumps to)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   sortPos_refList_amrST(refHeapSTPtr);
   *errSC = 0;
   goto cleanUp_fun25_sec06;

   memErr_fun25_sec06:;
      *errSC = def_memErr_amrST;
      goto errCleanUp_fun25_sec06;

   fileErr_fun25_sec06:;
      *errSC = def_fileErr_amrST;
      goto errCleanUp_fun25_sec06;

   errCleanUp_fun25_sec06:;
      if(refHeapSTPtr)
         freeHeap_refList_amrST(refHeapSTPtr);
      refHeapSTPtr = 0;

      goto cleanUp_fun25_sec06;

   cleanUp_fun25_sec06:;
      freeStack_amrST(&amrStackST);

      if(buffHeapStr != 0)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(! amrFILE) ;
      else fclose(amrFILE);
      amrFILE = 0;

      return refHeapSTPtr;
} /*readTbl_amrST*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not posible, this code is under the
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
: Permision is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permision notice
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
