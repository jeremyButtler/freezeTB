/*########################################################
# Name: amrST
#   - holds functions to support the amrST struct used in
#     tbAmr.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - include libraries
'   o .h st01: amrST
'     - holds a single amr mutation + supporting info
'   o fun01: blank_amrST
'     - sets all non-pointer values in amrSTPtr to 0
'   o fun02: init_AmrST
'     - sets all values/pointers in amrSTPtr to 0
'   o fun03: freeStack_amrST
'     - frees internal variables in an amrST structure
'   o fun04: freeHeap_amrST
'     - frees heap allocated amrST structure
'   o fun05: freeHeapAry_amrST
'     - frees an heap allocated array of amrST structs
'   o fun06: swap_amrST
'     - swaps values between two amrST structures
'   o fun07: sortPos_amrST
'     - Sort on an amrST array structures by reference
'       coordiante (uses shell sort)
'   o fun08: sortGeneId_amrST
'     - sort amrST struct array by gene names (ids)
'   o fun09: getAmr_amrST
'     - finds nearest amr to input query coordiante
'   o fun10: p_amrST
'     - Print out the amr database used
'   o fun11: readTbl_amrST
'     - gets amr data from tbAmr tsv file (p_amrST;fun 10)
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
#include "../genLib/charCp.h"
#include "drugAry.h"

/*Only .h files*/
#include "../genLib/dataTypeShortHand.h"

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
   sint siAmr = 0;

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
| Fun02: init_AmrST
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
   ulong ulElm = 0;

   if(amrSTAry)
   { /*If: have array to free*/
      for(
         ulElm = 0;
         ulElm < numElmUL;
         ++ulElm
      ) freeStack_amrST(&amrSTAry[ulElm]);
      
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
   schar *tmpStr = 0;
   sint siAmr = 0;
   
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
| Fun07: sortPos_amrST
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
   ' Fun07 TOC:
   '   - sort an array of amrST structs by reference pos
   '   - Shell sort taken from:
   '     - Adam Drozdek. 2013. Data Structures and
   '       Algorithims in c++. Cengage Leraning. fourth
   '       edition. pages 505-508
   '     - i made some minor changes, but is mostly the
   '       same
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find number of rounds to sort for
   '   o fun07 sec03:
   '     - sort the amrST array by reference position
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec01:
  ^   - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*number of elements to sort*/
  ulong numElmUL = (endUI) - (startUI);
  
  /*number of sorting rounds*/
  ulong subUL = 0;
  ulong nextElmUL = 0;
  ulong lastElmUL = 0;
  ulong elmOnUL = 0;
  
  /*get arrays to sort from the matrix (for sanity)*/
  
  /*variables to incurment loops*/
  ulong ulIndex = 0;
  ulong ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec02:
  ^   - find max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun07 Sec03:
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
| Fun08: sortGeneId_amrST
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
   ' Fun08 TOC:
   '   - sort amrST struct array by gene names (ids)
   '   - Shell sort taken from:
   '     - Adam Drozdek. 2013. Data Structures and
   '       Algorithims in c++. Cengage Leraning. fourth
   '       edition. pages 505-508
   '     - I made some minor changes, but is mostly the
   '       same
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - find the number of rounds to sort for
   '   o fun08 sec03:
   '     - sort the amrSTs by variant id
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec01:
  ^   - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  ulong numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  ulong subUL = 0;
  ulong nextElmUL = 0;
  ulong lastElmUL = 0;
  ulong elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  ulong ulIndex = 0;
  ulong ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec02:
  ^   - find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun08 Sec03:
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
            eql_charCp(
                amrAryST[ulElm].geneIdStr,
                amrAryST[nextElmUL].geneIdStr,
                '\0'
            )
          > 0
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
                eql_charCp(
                   amrAryST[elmOnUL].geneIdStr,
                   amrAryST[lastElmUL].geneIdStr,
                   '\0'
                )
              > 0
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
| Fun09: getAmr_amrST
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
| Fun10: p_amrST
|  - print out amr database used
| Input:
|  - amrAryST:
|    o pointer to amrST struct array to print out
|  - numAmrUL:
|    o number of amrST's in amrAryST
|  - drugAryStr:
|    o c-string array with drug names (from drugAry.h)
|  - numDrugsSI:
|    o number of antibiotics in drugAryStr
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
   struct amrST *amrSTAry,
   unsigned long numAmrUL,
   signed char *drugAryStr,
   signed int numDrugsSI,
   signed char *outStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC: p_amrST
   '   - print out amr database used
   '   o fun10 sec01:
   '     - variable declerations
   '   o fun10 sec02:
   '     - open output file
   '   o fun10 sec03:
   '     - print header
   '   o fun10 sec04:
   '     - print AMRs
   '   o fun10 sec05:
   '     - close file and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong ulAmr = 0;    /*looping through amrSTAry*/

   sint siDrug = 0;    /*looping through drugAryStr*/
   schar *drugStr = 0; /*printing out the drug id's*/
   ulong ulRes = 0;    /*checking amr's resitance*/
   ulong ulCross = 0;  /*check amr's cross resitance*/
   sint siAmrSet = 0;  /*flag set on*/
   sint siFlag = 0;    /*flag on in a set*/
   sint siDrugOn = 0;  /*drug on*/
   schar resBl = 0;

   schar *tmpStr = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
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
   ^ Fun10 Sec03:
   ^   - print header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE,"gene\tvariantID\trefPosition");
   fprintf(outFILE,"\tdirection\tmutation\tframeshift");
   fprintf(outFILE,"\trefSeq\tamrSeq\tcodonRefPos");
   fprintf(outFILE,"\tcodonNumber\tlastCodon\trefAA");
   fprintf(outFILE, "\tamrAA\tgeneStart\tgeneEnd");
   fprintf(outFILE, "\thighResistance\tlowResitance");
   fprintf(outFILE, "\tadditiveResitance\tneedsGene");

   for(siDrug = 0; siDrug < numDrugsSI; ++siDrug)
   { /*Loop: print antibiotic names*/
      drugStr = get_drugAry(drugAryStr, siDrug);
      fprintf(outFILE, "\t%s", drugStr);
   } /*Loop: print antibiotic names*/

   fprintf(outFILE,"\tendAntibiotics\teffect\tcomment");
   fprintf(outFILE, "\tgrade\tentireGene\tunkownEntry");
   fprintf(outFILE, "\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - print AMRs
   ^   o fun10 sec04 sub01:
   ^     - start loop; print ids & reference position
   ^   o fun10 sec04 sub02:
   ^     - print direction and AMR type/sequence
   ^   o fun10 sec04 sub03:
   ^     - print amino acid sequence and coordinates
   ^   o fun10 sec04 sub04:
   ^     - print gene coordinates
   ^   o fun10 sec04 sub05:
   ^     - print resistance level/if additive
   ^   o fun10 sec04 sub06:
   ^     - print drugs resistant to
   ^   o fun10 sec04 sub07:
   ^     - print effect entry
   ^   o fun10 sec04 sub08:
   ^     - print comment entry
   ^   o fun10 sec04 sub09:
   ^     - print grade (main drug), if effect appies to
   ^       entire gene, and if unknown entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec04 Sub01:
   *   - start loop; print ids and reference position
   \*****************************************************/

   for(
      ulAmr = 0;
      ulAmr < numAmrUL;
      ++ulAmr
   ){ /*Loop: print out each  amr*/
      fprintf(
         outFILE,
         "%s\t%s\t%u",
         amrSTAry[ulAmr].geneIdStr,   /*gene name*/
         amrSTAry[ulAmr].varIdStr,    /*variant name*/
         amrSTAry[ulAmr].refPosUI + 1 /*ref position*/
      ); /*print first few entries*/

      /**************************************************\
      * Fun10 Sec04 Sub02:
      *   - print direction and AMR type/sequence
      \**************************************************/

      /*print reading frame of the gene*/
      if(
         amrSTAry[ulAmr].dirFlagSC == def_forwardDir_amrST
      ) fprintf(outFILE, "\tF");

      else if(
         amrSTAry[ulAmr].dirFlagSC == def_revCompDir_amrST
      ) fprintf(outFILE, "\tR");

      else
         fprintf(outFILE, "\tNA");

      /*print mutation type, if frame shift, sequences*/
      fprintf(
         outFILE,
         "\t%s\t%u\t%s\t%s",
         amrSTAry[ulAmr].mutTypeStr,
         amrSTAry[ulAmr].frameshiftBl,
         amrSTAry[ulAmr].refSeqStr,
         amrSTAry[ulAmr].amrSeqStr
      );

      /**************************************************\
      * Fun10 Sec04 Sub03:
      *   - print amino acid sequence and coordinates
      \**************************************************/

      if(
         amrSTAry[ulAmr].refAaStr != 0 &&
         amrSTAry[ulAmr].refAaStr[0] > 0 &&
         amrSTAry[ulAmr].refAaStr[0] != '0'
      ){ /*If: had amino acid change*/
         fprintf(
            outFILE,
            "\t%u\t%u\t%u",
            amrSTAry[ulAmr].codonPosUI + 1,
            amrSTAry[ulAmr].codonNumUI,
            amrSTAry[ulAmr].endCodonNumUI
         );

         /*check if have reference aa sequence*/
         if(amrSTAry[ulAmr].refAaStr != 0)
            fprintf(
                outFILE,
                "\t%s",
                amrSTAry[ulAmr].refAaStr
            );

         else
            fprintf(outFILE, "\t0");

         /*check if have amr aa sequence*/
         if(amrSTAry[ulAmr].amrAaStr != 0)
            fprintf(
                outFILE,
                "\t%s",
                amrSTAry[ulAmr].amrAaStr
            );

         else
            fprintf(outFILE, "\t0");
      } /*If: had an amino acid change*/

      else
         fprintf(outFILE, "\tNA\tNA\tNA\t0\t0");

      /**************************************************\
      * Fun10 Sec04 Sub04:
      *   - print gene coordinates
      \**************************************************/

      /*print gene position*/
      if(amrSTAry[ulAmr].geneLastRefUI > 0)
         fprintf(
            outFILE,
            "\t%u\t%u",
            amrSTAry[ulAmr].geneFirstRefUI + 1,
            amrSTAry[ulAmr].geneLastRefUI + 1
         );

      else
         fprintf(outFILE, "\tNA\tNA");
            
      /**************************************************\
      * Fun10 Sec04 Sub05:
      *   - print resistance level/if additive
      \**************************************************/

      fprintf(
         outFILE,
        "\t%i\t%i\t%i",
        amrSTAry[ulAmr].highResBl,
        amrSTAry[ulAmr].lowResBl,
        amrSTAry[ulAmr].additiveResBl
      ); /*print who comment parsed data*/
         
      /*should not need to do this, but for some
      `    odd reason I get blanks/(null) if I do not
      */
      if(! amrSTAry[ulAmr].needsGeneStr)
         fprintf(outFILE, "\tNA");
      else if(amrSTAry[ulAmr].needsGeneStr[0] < 32)
         fprintf(outFILE, "\tNA");
      else
        fprintf(
           outFILE,
           "\t%s",
           amrSTAry[ulAmr].needsGeneStr
        );

      /**************************************************\
      * Fun10 Sec04 Sub06:
      *   - print drugs resistant to
      \**************************************************/

      siDrugOn = 0;

      for(
         siAmrSet = 0;
         siAmrSet
            < (sint)
              (
                  1 + numDrugsSI
                / (sizeof(unsigned long) << 3)
              );
         ++siAmrSet
      ){ /*Loop: print AMR's resitant to*/
         ulRes = amrSTAry[ulAmr].amrFlagsUL[siAmrSet];

         ulCross =
            amrSTAry[ulAmr].crossResFlagsUL[siAmrSet];

         for(
            siFlag=0;
            siFlag < (sint) def_maxFlags_amrST;
            ++siFlag
         ){ /*Loop: print antibiotic resitance*/
            /*check if this buffer was completely filled*/
            ++siDrugOn;
            if(siDrugOn > numDrugsSI)
               break;

            /*add resitance entry*/
            resBl =
               (schar)
               ( (-(ulRes & 1) ) & def_resFlag_amrST );

            /*add cross resistance entry*/
            resBl |=
               (schar)
               ((-(ulCross &1)) & def_crossResFlag_amrST);

            /*Logic (both lines)
            `  - ulRes & 1 or ulCross & 1:
            `    o 1 if there is resitance (bit set)
            `    o 0 if there is no resitance
            `  - -(ulRes & 1) or -(ulCross & 1):
            `    o -1 if there is resitance (all bits set)
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

         /***********************************************\
         * Fun10 Sec04 Sub07:
         *   - print effect entry
         \***********************************************/

         if(! amrSTAry[ulAmr].effectStr)
            fprintf(outFILE, "\tNA");
         else
         { /*Else: may have effect entry*/

            /*for some odd reason there are some blank
            `  effect entries in the catalog that escape
            `  me
            */
            tmpStr = amrSTAry[ulAmr].effectStr;

            while(*tmpStr++ < 32)
               if(*tmpStr == '\0')
                  break;

            if(*tmpStr == '\0')
               fprintf(outFILE, "\tNA");

            else
               fprintf(
                  outFILE,
                  "\t%s",
                  amrSTAry[ulAmr].effectStr
               );
         } /*Else: may have effect entry*/
 
         /***********************************************\
         * Fun10 Sec04 Sub08:
         *   - print comment entry
         \***********************************************/

         if(! amrSTAry[ulAmr].commentStr)
            fprintf(outFILE, "\tNA");
         else
         { /*Else: may have comment entry*/
            /*for some odd reason there are some blank
            `  comment entries in the catalog that escape
            `  me
            */
            tmpStr = amrSTAry[ulAmr].commentStr;

            while(*tmpStr < 32)
            { /*Loop: see if have something in comment*/
               if(*tmpStr == '\0')
                  break;

               ++tmpStr;
            } /*Loop: see if have something in comment*/

            if(*tmpStr == '\0')
               fprintf(outFILE, "\tNA");

            else
               fprintf(
                  outFILE,
                  "\t%s",
                  amrSTAry[ulAmr].commentStr
               );
         } /*Else: may have comment entry*/
 
         /***********************************************\
         * Fun10 Sec04 Sub09:
         *   - print grade (main drug), if effect applies
         *     to entire gene, and if unknown entry
         \***********************************************/

         fprintf(
            outFILE,
            "\t%i\t%i\t%i",
            amrSTAry[ulAmr].gradeSC,
            amrSTAry[ulAmr].wholeGeneFlagSC,
            amrSTAry[ulAmr].unknownBl
         ); /*print out the final columns*/

         fprintf(outFILE, "\n"); /*Print newline*/
      } /*Loop: print AMR's resitant to*/
   } /*Loop: print each  amr*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec05:
   ^   - close file and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outFILE != stdout)
      fclose(outFILE);

   return 0;
} /*p_amrST*/

/*-------------------------------------------------------\
| Fun11: readTbl_amrST
|   - gets amr data from tbAmr tsv file (p_amrST; fun 10)
| Input:
|   - tbAmrTblStr:
|     o c-string with path to AMR database/table
|   - numAmrUI:
|     o pointer to unisgined to to hold hold number of
|       AMRs in tbAmrTblStr
|   - durgStrAry:
|     o pointer to a c-string to hold antbiotic names
|   - numDrugsSI
|     o pointer to uint to hold number of drugs in
|       drugAryStr
|   - maxDrugsI:
|     o maximum number of drugs drugAryStr can hold
|     o this is changed when drugAryStr is resized
|   - errSC:
|     o signed char ponter to hold error message
| Output:
|   - Modifies:
|     o numAmrUI to hold the number of extracted amrs
|     o drugAryStr to hold the name of each drug
|     o numDrugsSI to hold number of drugs in drugAryStr
|     o maxDrugsI to hold maximum number of drugs
|       drugAryStr can hold
|     o errSC to hold the error message
|       - 0 for no errors
|       - def_fileErr_amrST if could not open tbAmrTblStr
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o array of amrST structures with AMRs
|     o 0 for errors; see errSC for specific error
\-------------------------------------------------------*/
struct amrST *
readTbl_amrST(
   signed char *tbAmrTblStr, /*path to tsv with AMR(s)*/
   unsigned int *numAmrUI,   /*number of AMR(s) in tsv*/
   signed char **drugAryStr, /*will get drug names*/
   signed int *numDrugsSI,   /*gets number drugs*/
   signed int *maxDrugsI,    /*max drugs for drugAryStr*/
   signed char *errSC        /*holds errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC: readTbl_amrST
   '   - gets amr data from tbAmr tsv (p_amrST; fun 10)
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - process header
   '   o fun11 sec03:
   '     - process header and get number of lines
   '   o fun11 sec04:
   '     - prepare buffers for extracting AMRs
   '   o fun11 sec05:
   '     - extract information from the file
   '   o fun11 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint lenBuffUI = 1 << 11; /*around 4096 bytes*/
   schar *buffHeapStr = 0;   /*buffer to read file into*/
   schar *tmpStr = 0;        /*for temporary operations*/

   sint longestLineSI = 0;
   sint tmpSI = 0;
   sint posSI = 0;

   /*boolean to check if I need to make a new drug array*/
   schar newDrugAryBl = (*drugAryStr == 0);

   sint siEndDrug = 0;   /*marks end of the drug entry*/
   sint siStartDrug = 0; /*marks start of the drug entry*/
   sint siDrug = 0;      /*iterator for loops*/

   struct amrST *amrSTAry = 0;
   uint uiAmr = 0;
   ulong *drugResUL = 0;
   ulong *drugCrossResUL = 0;

   FILE *amrFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - check if file eixists and sets up buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrFILE =
      fopen(
         (char *) tbAmrTblStr,
         "r"
      );

   if(amrFILE == 0)
      goto fileErr_fun11_sec06_sub03;

   /*set up buffer*/
   buffHeapStr = malloc((lenBuffUI + 9) * sizeof(schar));
     /*+9 for null + 8 bytes for ulCp overshoot*/

   if(buffHeapStr == 0)
      goto memErr_fun11_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - process header and get number of lines
   ^   o fun11 sec03 sub01:
   ^     - get number of lines in file
   ^   o fun11 sec03 sub02:
   ^     - read in header
   ^   o fun11 sec03 sub03:
   ^     - get number of antibiotics
   ^   o fun11 sec03 sub04:
   ^     - allocate memory for antibiotics
   ^   o fun11 sec03 sub05:
   ^     - copy the antibiotics to the drug array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - get number of lines in file
   \*****************************************************/

   *numAmrUI = 0;

   while(
      fgets((char *) buffHeapStr, lenBuffUI, amrFILE)
   ){ /*Loop: get number of amrs from file*/
      tmpStr = buffHeapStr;
      posSI = endLine_ulCp(buffHeapStr);

      tmpSI += posSI;
      tmpStr += posSI;

      if(tmpSI > longestLineSI)
         longestLineSI = tmpSI;

      if(*tmpStr == '\n')
      { /*If: new line*/
         tmpSI = 0;
         ++(*numAmrUI);
         ++tmpStr;
      } /*If: new line*/
   } /*Loop: get number of amrs from file*/

   ++longestLineSI; /*account for EOF (no '\n')*/

   if(! *numAmrUI)
      goto fileErr_fun11_sec06_sub03; /*no AMRs in file*/

   fseek(
      amrFILE,
      0,
      SEEK_SET
   );

   free(buffHeapStr);
   buffHeapStr = 0;
   lenBuffUI = longestLineSI + 1;
   buffHeapStr = malloc((lenBuffUI + 9) * sizeof(schar));

   if(buffHeapStr == 0)
      goto memErr_fun11_sec06_sub02;

   /*****************************************************\
   * Fun11 Sec03 Sub02:
   *   - read in header
   \*****************************************************/

   tmpStr =
      (schar *)
      fgets(
         (char *) buffHeapStr,
         lenBuffUI,
         amrFILE
      );

   if(! tmpStr)
      goto fileErr_fun11_sec06_sub03;
  
   /*****************************************************\
   * Fun11 Sec03 Sub03:
   *   - get number of antibiotics
   \*****************************************************/

   /*moving to first antibiotic in header*/
   for(
      siDrug = 0;
      siDrug < def_firstDrugCol_amrST;
      ++siDrug
   ) while(buffHeapStr[siStartDrug++] > 31) ;

   siDrug = *numDrugsSI;
   siEndDrug = siStartDrug;

   while(
      match_charCp(
         &buffHeapStr[siEndDrug],
         (schar *) "endAntibiotics",
         '\t',
         &siEndDrug
      )
   ){ /*Loop: find end of antibiotics columns*/
      ++(*numDrugsSI);

      while(buffHeapStr[siEndDrug++] != '\t') ;
   } /*Loop: find end of antibiotics columns*/

   /*****************************************************\
   * Fun11 Sec03 Sub04:
   *   - allocate memory for antibiotics
   \*****************************************************/

   if(newDrugAryBl)
   { /*If: is new drug array*/
      *drugAryStr = alloc_drugAry(*numDrugsSI);

      if(! *drugAryStr)
         goto memErr_fun11_sec06_sub02;
   } /*If: is new drug array*/

   else if(*numDrugsSI > *maxDrugsI)
   { /*Else If: need to add more memory to drug array*/
      tmpStr =
         realloc_drugAry(
            *drugAryStr,
            *numDrugsSI
         );

      if(! tmpStr)
         goto memErr_fun11_sec06_sub02;

      *drugAryStr = tmpStr;
   } /*Else If: need to add more memory to drug array*/

   /*****************************************************\
   * Fun11 Sec03 Sub05:
   *   - copy antibiotics to drug array
   \*****************************************************/

   while(siDrug < *numDrugsSI)
   { /*Loop: read in antibiotic entries*/
      siStartDrug +=
         cpDrug_drugAry(
            *drugAryStr, 
            &buffHeapStr[siStartDrug],
            siDrug,
            '\t'
         ); /*copy drug to drug array*/

      ++siStartDrug; /*get off tab*/
      ++siDrug;
   } /*Loop: read in antibiotic entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - prepare buffers for extracting AMRs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrSTAry =
      malloc(*numAmrUI * sizeof(struct amrST));

   if(! amrSTAry)
      goto memErr_fun11_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec05:
   ^   - extract information from file
   ^   o fun11 sec05 sub02:
   ^     - get first entry, start loop, & initialize
   ^   o fun11 sec05 sub02:
   ^     - read in gene id
   ^   o fun11 sec05 sub03:
   ^     - read in variaint id
   ^   o fun11 sec05 sub04:
   ^     - read in refernce positon
   ^   o fun11 sec05 sub05:
   ^     - read in direction
   ^   o fun11 sec05 sub06:
   ^     - read in mutation type
   ^   o fun11 sec05 sub07:
   ^     - read in frame shift entry
   ^   o fun11 sec05 sub08:
   ^     - read in reference sequence
   ^   o fun11 sec05 sub09:
   ^     - read in amr sequence
   ^   o fun11 sec05 sub10:
   ^     - read in frist codon base in reference
   ^   o fun11 sec05 sub11:
   ^     - read in starting codon number
   ^   o fun11 sec05 sub12:
   ^     - read in ending codon number
   ^   o fun11 sec05 sub13:
   ^     - read in reference amino acid sequence
   ^   o fun11 sec05 sub14:
   ^     - read in amr amino acid sequence
   ^   o fun11 sec05 sub15:
   ^     - read in gene starting position
   ^   o fun11 sec05 sub16:
   ^     - read in gene ending position
   ^   o fun11 sec05 sub17:
   ^     - check if is a high resistance gene
   ^   o fun11 sec05 sub18:
   ^     - check if is a low resistance gene
   ^   o fun11 sec05 sub19:
   ^     - check if resistance is additive
   ^   o fun11 sec05 sub20:
   ^     - read in "if it needs a functional gene"
   ^   o fun11 sec05 sub21:
   ^     - read in antibiotic flags
   ^   o fun11 sec05 sub22:
   ^     - read in effect entry
   ^   o fun11 sec05 sub23:
   ^     - read in comment entry
   ^   o fun11 sec05 sub24:
   ^     - read in grade entry
   ^   o fun11 sec05 sub25:
   ^     - read in "if entry effects the entire gene"
   ^   o fun11 sec05 sub26:
   ^     - read in unkown type entry
   ^   o fun11 sec05 sub27:
   ^     - get the next line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec05 Sub01:
   *   - get first entry, Start read in loop, & initailze
   \*****************************************************/

   /*get past header*/
   tmpStr =
      (schar *)
      fgets(
         (char *) buffHeapStr,
         lenBuffUI,
         amrFILE
      );

   for(
      uiAmr = 0;
      uiAmr < *numAmrUI;
      ++uiAmr
   ) { /*Loop: read in file*/

      init_amrST(&amrSTAry[uiAmr]);
      tmpStr = buffHeapStr;

      /**************************************************\
      * Fun11 Sec05 Sub02:
      *   - read in gene id
      \**************************************************/

      amrSTAry[uiAmr].lenGeneIdUI =
        lenStrNull_charCp(
           tmpStr,
           '\t'
        );

      if(tmpStr[amrSTAry[uiAmr].lenGeneIdUI] != '\t')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      amrSTAry[uiAmr].geneIdStr =
         malloc(
              (amrSTAry[uiAmr].lenGeneIdUI + 1)
            * sizeof(char)
         ); /*Allocate memory for the gene name*/

      if(! amrSTAry[uiAmr].geneIdStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_charCp(
         amrSTAry[uiAmr].geneIdStr,
         tmpStr,
         amrSTAry[uiAmr].lenGeneIdUI
      );

      tmpStr += amrSTAry[uiAmr].lenGeneIdUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub03:
      *   - read in variaint id
      \**************************************************/

      amrSTAry[uiAmr].lenVarIdUI =
         lenStrNull_ulCp(
            tmpStr,
            def_tab_ulCp,
            '\t'
         );

      if(tmpStr[amrSTAry[uiAmr].lenVarIdUI] != '\t')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      amrSTAry[uiAmr].varIdStr =
         malloc(
              (amrSTAry[uiAmr].lenVarIdUI + 1)
            * sizeof(char)
         ); /*Allocate memory for the variant id*/

      if(! amrSTAry[uiAmr].varIdStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_ulCp(
         amrSTAry[uiAmr].varIdStr,
         tmpStr,
         amrSTAry[uiAmr].lenVarIdUI
      );

      tmpStr += amrSTAry[uiAmr].lenVarIdUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub04:
      *   - read in refernce positon
      \**************************************************/

      tmpStr +=
         strToUI_base10str(
            tmpStr,
            &amrSTAry[uiAmr].refPosUI
         );

      --amrSTAry[uiAmr].refPosUI; /*convert to index 0*/

      if(*tmpStr > 32)
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      ++tmpStr; /*get off the tab*/

      /**************************************************\
      * Fun11 Sec05 Sub05:
      *   - read in direction
      \**************************************************/

      if(*tmpStr == 'F') 
         amrSTAry[uiAmr].dirFlagSC = def_forwardDir_amrST;
      else if(*tmpStr == 'R') 
         amrSTAry[uiAmr].dirFlagSC = def_revCompDir_amrST;
      else
         amrSTAry[uiAmr].dirFlagSC = def_unkownDir_amrST;
     
      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub06:
      *   - read in the mutation type
      \**************************************************/

      if(*tmpStr == '\t')
      { /*If: blank entry*/
         amrSTAry[uiAmr].mutTypeStr[0] = 'N';
         amrSTAry[uiAmr].mutTypeStr[1] = 'A';
         amrSTAry[uiAmr].mutTypeStr[2] = '\0';

         goto endMut_fuh11_sec05_sub06;
      } /*If: blank entry*/

      amrSTAry[uiAmr].mutTypeStr[0] = *tmpStr++;

      if(*tmpStr == '\t')
      { /*If: short entry*/
         amrSTAry[uiAmr].mutTypeStr[1] = '\0';
         goto endMut_fuh11_sec05_sub06;
      } /*If: short entry*/

      amrSTAry[uiAmr].mutTypeStr[1] = *tmpStr++;

      if(*tmpStr == '\t')
      { /*If: short entry*/
         amrSTAry[uiAmr].mutTypeStr[2] = '\0';
         goto endMut_fuh11_sec05_sub06;
      } /*If: short entry*/

      /*full length entry*/
      amrSTAry[uiAmr].mutTypeStr[2] = *tmpStr++;
      amrSTAry[uiAmr].mutTypeStr[3] = '\0';

      /*move to next entry*/
      endMut_fuh11_sec05_sub06:;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub07:
      *   - read in frame shift entry
      \**************************************************/

      amrSTAry[uiAmr].frameshiftBl = (*tmpStr == '1');
      
      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;/*end of line*/

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub08:
      *   - read in reference sequence
      \**************************************************/

      amrSTAry[uiAmr].lenRefSeqUI =
         lenStrNull_charCp(
            tmpStr,
            '\t'
         );

      if(tmpStr[amrSTAry[uiAmr].lenRefSeqUI] != '\t')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      amrSTAry[uiAmr].refSeqStr =
         malloc(
              (amrSTAry[uiAmr].lenRefSeqUI + 1)
            * sizeof(char)
         );

      if(! amrSTAry[uiAmr].refSeqStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_charCp(
         amrSTAry[uiAmr].refSeqStr,
         tmpStr,
         amrSTAry[uiAmr].lenRefSeqUI
      );

      tmpStr += amrSTAry[uiAmr].lenRefSeqUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub09:
      *   - read in amr sequence
      \**************************************************/

      amrSTAry[uiAmr].lenAmrSeqUI =
         lenStrNull_charCp(
            tmpStr,
            '\t'
         );

      if(tmpStr[amrSTAry[uiAmr].lenAmrSeqUI] != '\t')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      amrSTAry[uiAmr].amrSeqStr =
         malloc(
              (amrSTAry[uiAmr].lenAmrSeqUI + 1)
            * sizeof(char)
         );

      if(! amrSTAry[uiAmr].amrSeqStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_charCp(
         amrSTAry[uiAmr].amrSeqStr,
         tmpStr,
         amrSTAry[uiAmr].lenAmrSeqUI
      );

      tmpStr += amrSTAry[uiAmr].lenAmrSeqUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub10:
      *   - read in frist codon base in reference
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrSTAry[uiAmr].codonPosUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*If: is mising*/

      else
      { /*Else: have number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrSTAry[uiAmr].codonPosUI
            );

         --amrSTAry[uiAmr].codonPosUI; /*to index 0*/

         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;/*non-numeric*/

         ++tmpStr;
      } /*Else: have number*/

      /**************************************************\
      * Fun11 Sec05 Sub11:
      *   - read in starting codon number
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrSTAry[uiAmr].codonNumUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*If: is mising*/

      else
      { /*Else: have number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrSTAry[uiAmr].codonNumUI
            );

         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Else: have number*/

      /**************************************************\
      * Fun11 Sec05 Sub12:
      *   - read in ending codon number
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: is mising*/
         amrSTAry[uiAmr].endCodonNumUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*If: is mising*/

      else
      { /*Else: have number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrSTAry[uiAmr].endCodonNumUI
            );

         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Else: have number*/

      /**************************************************\
      * Fun11 Sec05 Sub13:
      *   - read in reference amino acid sequence
      \**************************************************/

      amrSTAry[uiAmr].lenRefAaUI =
         lenStrNull_charCp(
            tmpStr,
            '\t'
          );

      if(tmpStr[amrSTAry[uiAmr].lenRefAaUI] != '\t')
         goto fileErr_fun11_sec06_sub03; /*end of line*/

      amrSTAry[uiAmr].refAaStr =
         malloc(
              (amrSTAry[uiAmr].lenRefAaUI + 1)
            * sizeof(char)
         );
   
      if(! amrSTAry[uiAmr].refAaStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_charCp(
         amrSTAry[uiAmr].refAaStr,
         tmpStr,
         amrSTAry[uiAmr].lenRefAaUI
      );

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec06_sub03;/*end of line*/

      tmpStr += amrSTAry[uiAmr].lenRefAaUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub14:
      *   - read in the amr amino acid sequence
      \**************************************************/

      amrSTAry[uiAmr].lenAmrAaUI =
         lenStrNull_charCp(
            tmpStr,
            '\t'
         );

      if(tmpStr[amrSTAry[uiAmr].lenAmrAaUI] != '\t')
         goto fileErr_fun11_sec06_sub03;/*end of line*/

      amrSTAry[uiAmr].amrAaStr =
         malloc(
              (amrSTAry[uiAmr].lenAmrAaUI + 1)
            * sizeof(char)
         );
   
      if(! amrSTAry[uiAmr].amrAaStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_charCp(
         amrSTAry[uiAmr].amrAaStr,
         tmpStr,
         amrSTAry[uiAmr].lenAmrAaUI
      );

      tmpStr += amrSTAry[uiAmr].lenAmrAaUI + 1;
         
      /**************************************************\
      * Fun11 Sec05 Sub15:
      *   - read in gene starting position
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: this is mising*/
         amrSTAry[uiAmr].geneFirstRefUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*If: this is mising*/

      else
      { /*Else: I have a number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrSTAry[uiAmr].geneFirstRefUI
            );

         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         --amrSTAry[uiAmr].geneFirstRefUI; /*to index 0*/
         ++tmpStr;
      } /*Else: I have a number*/

      /**************************************************\
      * Fun11 Sec05 Sub16:
      *   - read in gene ending position
      \**************************************************/

      if(*tmpStr == 'N')
      { /*If: this is mising*/
         amrSTAry[uiAmr].geneLastRefUI = 0;

         while(*tmpStr > 32)
            ++tmpStr;

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*If: this is mising*/

      else
      { /*Else: I have a number*/
         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &amrSTAry[uiAmr].geneLastRefUI
            );

         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         --amrSTAry[uiAmr].geneLastRefUI; /*to index 0*/
         ++tmpStr;
      } /*Else: I have a number*/

      /**************************************************\
      * Fun11 Sec05 Sub17:
      *   - check if is a high resistance gene
      \**************************************************/
      
      amrSTAry[uiAmr].highResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub18:
      *   - check if is a low resistance gene
      \**************************************************/
      
      amrSTAry[uiAmr].lowResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub19:
      *   - check if resistance is additive
      \**************************************************/
      
      amrSTAry[uiAmr].additiveResBl = (*tmpStr == '1');

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub20:
      *   - Read in "if it needs a functional gene"
      \**************************************************/

      /*nothing in the column; should not happen*/
      if(*tmpStr == '\t')
      { /*If: there is no entry*/
         
         amrSTAry[uiAmr].lenNeedsGeneUI = 2;

         amrSTAry[uiAmr].needsGeneStr =
            malloc( 3 * sizeof(char));

         if(! amrSTAry[uiAmr].needsGeneStr)
            goto memErr_fun11_sec06_sub02;

        amrSTAry[uiAmr].needsGeneStr[0] = 'N';
        amrSTAry[uiAmr].needsGeneStr[1] = 'A';
        amrSTAry[uiAmr].needsGeneStr[2] = '\0';

        ++tmpStr;
      } /*If: there is no entry*/

      else
      { /*Else if: entry (may be NA)*/
         amrSTAry[uiAmr].lenNeedsGeneUI =
            lenStrNull_charCp(
               tmpStr,
               '\t'
            );

         if(tmpStr[amrSTAry[uiAmr].lenNeedsGeneUI] !='\t')
            goto fileErr_fun11_sec06_sub03;/*end of line*/

         amrSTAry[uiAmr].needsGeneStr =
            malloc(
                 (amrSTAry[uiAmr].lenNeedsGeneUI + 1)
               * sizeof(char)
            );

         if(! amrSTAry[uiAmr].needsGeneStr)
            goto memErr_fun11_sec06_sub02;

         cpLen_charCp(
            amrSTAry[uiAmr].needsGeneStr,
            tmpStr,
            amrSTAry[uiAmr].lenNeedsGeneUI
         );

         tmpStr += amrSTAry[uiAmr].lenNeedsGeneUI + 1;

         if(*(tmpStr - 1) != '\t')
            goto fileErr_fun11_sec06_sub03;/*end of line*/
      } /*Else if: entry (may be NA)*/

      /**************************************************\
      * Fun11 Sec05 Sub21:
      *   - read in antibiotic flags
      \**************************************************/

      siDrug = 0;
      drugResUL = amrSTAry[uiAmr].amrFlagsUL;
      drugCrossResUL = amrSTAry[uiAmr].crossResFlagsUL;

      while(*tmpStr != '*')
      { /*Loop: check for antibiotic resistance*/
         if(siDrug >= (sint) def_maxFlags_amrST)
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

         while(*tmpStr < 31)
         { /*Loop: get past tab*/
            if(*tmpStr != '\t')
               goto fileErr_fun11_sec06_sub03;

            ++tmpStr;
         } /*Loop: get past tab*/
      } /*Loop: check for antibiotic resistance*/
     
      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub22:
      *   - read in effect entry
      \**************************************************/

      amrSTAry[uiAmr].lenEffectUI =
         lenStrNull_ulCp(
            tmpStr,
            def_tab_ulCp,
            '\t'
         );

      if(tmpStr[amrSTAry[uiAmr].lenEffectUI] != '\t')
         goto fileErr_fun11_sec06_sub03;/*end of line*/

      amrSTAry[uiAmr].effectStr =
         malloc(
              (amrSTAry[uiAmr].lenEffectUI + 1)
            * sizeof(char)
         );

      if(! amrSTAry[uiAmr].effectStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_ulCp(
         amrSTAry[uiAmr].effectStr,
         tmpStr,
         amrSTAry[uiAmr].lenEffectUI
      );

      tmpStr += amrSTAry[uiAmr].lenEffectUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub23:
      *   - read in comment entry
      \**************************************************/

      amrSTAry[uiAmr].lenCommentUI = 0;

      while(tmpStr[amrSTAry[uiAmr].lenCommentUI] > 31)
         ++amrSTAry[uiAmr].lenCommentUI;

      if(tmpStr[amrSTAry[uiAmr].lenCommentUI] != '\t')
         goto fileErr_fun11_sec06_sub03;/*end of line*/

      amrSTAry[uiAmr].commentStr =
         malloc(
              (amrSTAry[uiAmr].lenCommentUI + 1)
            * sizeof(char)
         );

      if(! amrSTAry[uiAmr].commentStr)
         goto memErr_fun11_sec06_sub02;

      cpLen_ulCp(
         amrSTAry[uiAmr].commentStr,
         tmpStr,
         amrSTAry[uiAmr].lenCommentUI
      );

      tmpStr += amrSTAry[uiAmr].lenCommentUI + 1;

      /**************************************************\
      * Fun11 Sec05 Sub24:
      *   - read in grade entry
      \**************************************************/

      if(*(tmpStr - 1) != '\t')
         goto nextLine_readAmrST;

      amrSTAry[uiAmr].gradeSC = *tmpStr - 48;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub25:
      *   - Read in if the entry effects the entire gene
      \**************************************************/

      amrSTAry[uiAmr].wholeGeneFlagSC = *tmpStr - 48;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 31)
      { /*Loop: get past tab*/
         if(*tmpStr != '\t')
            goto fileErr_fun11_sec06_sub03;

         ++tmpStr;
      } /*Loop: get past tab*/

      /**************************************************\
      * Fun11 Sec05 Sub26:
      *   - read in unkown type entry
      \**************************************************/

      amrSTAry[uiAmr].unknownBl = *tmpStr - 48;

      /**************************************************\
      * Fun11 Sec05 Sub27:
      *   - get next line
      \**************************************************/

      nextLine_readAmrST:;

      tmpStr =
         (schar *)
         fgets(
            (char *) buffHeapStr,
            lenBuffUI,
            amrFILE
         );
   } /*Loop: read in file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec06:
   ^   - clean up and return
   ^   o fun11 sec06 sub01:
   ^     - no error clean up
   ^   o fun11 sec06 sub02:
   ^     - memory error clean up
   ^   o fun11 sec06 sub03:
   ^     - file error clean up
   ^   o fun11 sec06 sub04:
   ^     - error clean up (all errors jump to)
   ^   o fun11 sec06 sub05:
   ^     - general clean up (everything jumps to)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   ++(*numDrugsSI); /*convert index 0 to index 1*/

   *errSC = 0;

   sortPos_amrST(
      amrSTAry,
      0,
     *numAmrUI - 1
   );

   goto cleanUp_fun11_sec06_sub05;

   /*****************************************************\
   * Fun11 Sec06 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun11_sec06_sub02:;
   *errSC = def_memErr_amrST;
   goto errCleanUp_fun11_sec06_sub04;

   /*****************************************************\
   * Fun11 Sec06 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_fun11_sec06_sub03:;
   *errSC = def_fileErr_amrST;
   goto errCleanUp_fun11_sec06_sub04;

   /*****************************************************\
   * Fun11 Sec06 Sub04:
   *   - error clean up (all errors jump to)
   \*****************************************************/

   errCleanUp_fun11_sec06_sub04:;

   if(newDrugAryBl)
   { /*If: I made a new drug array*/
      if(*drugAryStr != 0)
         free(*drugAryStr);

      *drugAryStr = 0;
      *numDrugsSI = 0;
      *maxDrugsI = 0;
   } /*If: I made a new drug array*/

   if(amrSTAry)
      freeHeapAry_amrST(
         amrSTAry,
         *numAmrUI
      );

   *numAmrUI = 0;
   amrSTAry = 0;

   goto cleanUp_fun11_sec06_sub05;

   /*****************************************************\
   * Fun11 Sec06 Sub05:
   *   - general clean up (everything jumps to)
   \*****************************************************/

   cleanUp_fun11_sec06_sub05:;

   if(buffHeapStr != 0)
      free(buffHeapStr);

   buffHeapStr = 0;

   if(
        amrFILE
      && amrFILE != stdin
      && amrFILE != stdout
   ) fclose(amrFILE);

   amrFILE = 0;

   return amrSTAry;
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
