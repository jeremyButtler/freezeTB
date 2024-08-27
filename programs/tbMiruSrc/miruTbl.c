/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' miruTbl SOF: Start Of File
'   - holds the miruTbl struct and its supporting
'     functions and structrues
'   o header:
'     - included libraries
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
|   - included libraries
\-------------------------------------------------------*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "miruTbl.h"

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/samEntry.h"

/*only .h files used*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*using .h max macro only*/
#include "tbMiruDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .h #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_amp_miruTbl
|   - sets non-pointer vars in a amp_miruTbl struc to 0/-1
| Input:
|   - ampSTPtr:
|     o pointer to an amp_miruTbl structure to blank
| Output:
|   - Modifies:
|     o ampSTPtr variables: forPosSI, forLenSI,
|       revPosSI, and readLenSI to be -1
|     o ampSTPtr arrays: ampLenArySI and
|       indexArySI to have all elements set to -1
|     o amp_miruTblStPtr hitsArySI to have all elements
|       set to 0
\-------------------------------------------------------*/
void
blank_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
){
   sint ssiLin = 0;

   ampSTPtr->forPosSI = -1;
   ampSTPtr->forLenSI = -1;

   ampSTPtr->revPosSI = -1;
   ampSTPtr->revLenSI = -1;
   ampSTPtr->mapReadsSI = 0;
   ampSTPtr->noLinSI = 0;

   for(
      ssiLin = 0;
      ssiLin < ampSTPtr->numLinSI;
      ++ssiLin
   ){ /*Loop: blank all arrays values*/
      ampSTPtr->ampLenArySI[ssiLin] = -1;
      ampSTPtr->hitsArySI[ssiLin] = 0;
      ampSTPtr->indexArySI[ssiLin] = -1;
   } /*Loop: blank all array values*/
} /*blank_amp_miruTbl*/

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
){
   sint ssiLin = 0; /*lineage on in MIRU amplicon*/

   ampSTPtr->mapReadsSI = 0;
   ampSTPtr->noLinSI = 0;

   for(
      ssiLin = 0;
      ssiLin < ampSTPtr->numLinSI;
      ++ssiLin
   ) ampSTPtr->hitsArySI[ssiLin] = 0;
} /*resetCnt_amp_miruTbl*/

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
|       revPosSI, and readLenSI to be -1
\-------------------------------------------------------*/
void
init_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
){
   ampSTPtr->idStr[0] = '\0';
   ampSTPtr->numLinSI = 0;
   ampSTPtr->ampLenArySI = 0;
   ampSTPtr->indexArySI = 0;
   ampSTPtr->hitsArySI = 0;

   blank_amp_miruTbl(ampSTPtr);
} /*init_amp_miruTbl*/

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
){
   struct amp_miruTbl *retST = 0;
   retST = malloc(sizeof(struct amp_miruTbl));

   if(retST != 0)
      init_amp_miruTbl(retST);

   return retST;
} /*mk_amp_miruTbl*/

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
){
   if(amp_miruTblSTPtr->ampLenArySI)
      free(amp_miruTblSTPtr->ampLenArySI);

   if(amp_miruTblSTPtr->indexArySI)
      free(amp_miruTblSTPtr->indexArySI);

   if(amp_miruTblSTPtr->hitsArySI)
      free(amp_miruTblSTPtr->hitsArySI);

   init_amp_miruTbl(amp_miruTblSTPtr);
} /*freeStack_amp_miruTbl*/

/*-------------------------------------------------------\
| Fun06: freeHeap_amp_miruTbl
|   - frees an amp_miruTbl stucture
| Input:
|   - amp_miruTblSTPtr:
|     o pointer to amp_miruTbl structure to free
| Output:
|   - Frees:
|     o amp_miruTblSTPtr
|   - Modifies:
|     o amp_miruTblSTPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeHeap_amp_miruTbl(
   struct amp_miruTbl *ampSTPtr
){
   if(ampSTPtr)
   { /*If: have structure to free*/
      freeStack_amp_miruTbl(ampSTPtr);
      free(ampSTPtr);
   } /*If: have structure to free*/
} /*freeHeap_amp_miruTbl*/

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
){
   int siAmp = 0;
   int indexSI = 0;

   for(
      siAmp = 0;
      siAmp < miruTblSTPtr->numPrimSI;
      ++siAmp
   ) blank_amp_miruTbl(&miruTblSTPtr->ampAryST[siAmp]);

   miruTblSTPtr->numPrimSI = 0;

   for(
      siAmp = 0;
      siAmp < miruTblSTPtr->numLinSI;
      ++siAmp
   ){ /*Loop: set all lineages to null*/
      indexSI = strIndex_miruTbl(siAmp);
      (miruTblSTPtr)->lineageStrAry[indexSI] = '\0';
   } /*Loop: set all lineages to null*/

   miruTblSTPtr->numLinSI = 0;
} /*blank_miruTbl*/

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
){
   sint siAmp = 0;

   for(
      siAmp = 0;
      siAmp < miruTblSTPtr->numPrimSI;
      ++siAmp
   ) resetCnt_amp_miruTbl(&miruTblSTPtr->ampAryST[siAmp]);
} /*resetCnt_miruTbl*/

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
){
   miruTblSTPtr->lineageStrAry = 0;
   miruTblSTPtr->numLinSI = 0;

   miruTblSTPtr->ampAryST = 0;
   miruTblSTPtr->indexArySI = 0;
   miruTblSTPtr->numPrimSI = 0;

   blank_miruTbl(miruTblSTPtr);
} /*init_miruTbl*/

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
){
   struct miruTbl *retST = 0;
   retST = malloc(sizeof(struct miruTbl));

   if(retST)
      init_miruTbl(retST);

   return retST;
} /*mk_miruTbl*/

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
){
   sint siAmp = 0;

   if(miruTblSTPtr)
   { /*If: have structure to free*/
      if(miruTblSTPtr->lineageStrAry)
         free(miruTblSTPtr->lineageStrAry);

      miruTblSTPtr->lineageStrAry = 0;

      if(miruTblSTPtr->indexArySI)
         free(miruTblSTPtr->indexArySI);

      miruTblSTPtr->indexArySI = 0;

      if(miruTblSTPtr->ampAryST)
      { /*If: have amplicons to free*/
         for(
            siAmp = 0;
            siAmp < miruTblSTPtr->numPrimSI;
            ++siAmp
         ){ /*Loop: free amp_miruTbl struct array*/
             freeStack_amp_miruTbl(
                &miruTblSTPtr->ampAryST[siAmp]
             );
         } /*Loop: free amp_miruTbl struct array*/

         free(miruTblSTPtr->ampAryST);
         miruTblSTPtr->ampAryST = 0;
      } /*If: have amplicons to free*/

      init_miruTbl(miruTblSTPtr);
   } /*If: have structure to free*/
} /*freeStack_miruTbl*/

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
){
   if(miruTblSTPtr)
   { /*If: have a structure to free*/
      freeStack_miruTbl(miruTblSTPtr);
      free(miruTblSTPtr);
   } /*If: have a structure to free*/
} /*freeHeap_miruTbl*/

/*-------------------------------------------------------\
| Fun14: sort_amp_miruTbl
|   - sorts lineages in a amp_miruTbl struct by amp length
| Input:
|   - amp_miruTblSTPtr: 
|     o pointer to an amp_miruTblSTPtr structure to sort
| Output:
|   - Modifies:
|     o amp_miruTblSTPtr variables: ampLenArySI,
|       hitsArySI, and indexArySI to be sorted by values
|       in ampLenArySI (least to greatest)
\-------------------------------------------------------*/
void
sort_amp_miruTbl(
   struct amp_miruTbl *amp_miruTblSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC: sort_amp_miruTbl
   '   - sorts the lineages in a amp_miruTbl structure by the
   '     expected amplicon length (least to greatest)
   '   o fun14 sec01:
   '     - variable declerations
   '   o fun14 sec02:
   '     - find the first subset (largest gap)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Some quick pointers to save my sanity*/
   sint *ampLenArySI = amp_miruTblSTPtr->ampLenArySI;
   sint *hitsArySI = amp_miruTblSTPtr->hitsArySI;
   sint *indexArySI = amp_miruTblSTPtr->indexArySI;
   sint numLinSI = amp_miruTblSTPtr->numLinSI;

   sint distSI = 0; /*distance between elements in subset*/
   sint nextSI = 0; /*Next element in subset*/
   sint lastSI = 0; /*Previous element in subset*/
   sint onSI = 0;   /*Element currently sorting*/

   sint siSub = 0;  /*Subset of index's on in round*/
   sint siElm = 0;     /*index on*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - find the first subset (largest gap)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Adam (2013) mentioned that shell sort needed to have
   `   a subarray of indexs. I took there idea, but
   `   instead of using an array I find the final value &
   `   then backwards calulate the next index in the loop.
   `   This should save some memory.
   ` Adam Drozdek, 2013; Data Structures and
   `   algorithims in c++, Congage Learning, fourth
   `   edition, pages 505 to 508.
   ` Recursion formula: H0 = 1; Hn = 3 * Hn-1 + 1
   */

   distSI = 1;

   while(distSI < numLinSI - 1)
   { /*Loop: Find the first subset I am using*/
      distSI *= 3; 
      ++distSI;
   } /*Loop: Find the first subset I am using*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - sort my arrays
   ^   o fun14 sec03 sub01:
   ^     - set up the shell short triple loop
   ^   o fun14 sec03 sub02:
   ^     - check if I need to swap elements in a subset
   ^   o fun14 sec03 sub03:
   ^     - check if need to swap previous elements
   ^   o fun14 sec03 sub04:
   ^     - move to the next distance/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec03 Sub01:
   *   - set up the shell short triple loop
   \*****************************************************/

   while(distSI > 0)
   { /*Loop: sort till distance between subsets is 0*/

      for(siSub = 0; siSub < distSI; ++siSub)
      { /*Loop: Go through each subset in distSI*/

         for(
            siElm = 0;
            siElm + distSI < numLinSI;
            siElm += distSI
         ){ /*Loop: sort all elements in each subset*/

            /********************************************\
            * Fun14 Sec03 Sub02:
            *   - check if swap elements in subset
            \********************************************/

            /*get the next index in the subset*/
            nextSI = siElm + distSI;

            if(ampLenArySI[siElm] > ampLenArySI[nextSI])
            { /*If: need to swap elements in a subset*/
               ampLenArySI[siElm] ^= ampLenArySI[nextSI];
               ampLenArySI[nextSI] ^= ampLenArySI[siElm];
               ampLenArySI[siElm] ^= ampLenArySI[nextSI];

               hitsArySI[siElm] ^= hitsArySI[nextSI];
               hitsArySI[nextSI] ^= hitsArySI[siElm];
               hitsArySI[siElm] ^= hitsArySI[nextSI];

               indexArySI[siElm] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[siElm];
               indexArySI[siElm] ^= indexArySI[nextSI];

               /*****************************************\
               * Fun14 Sec03 Sub03:
               *   - check if swap previous elements
               \*****************************************/

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= distSI)
               { /*Loop: Check the previous elements*/
                  lastSI -= distSI;

                  if(   ampLenArySI[lastSI]
                      < ampLenArySI[onSI]
                  ) break;/*Finshed; element in position*/

                  ampLenArySI[onSI] ^=ampLenArySI[lastSI];
                  ampLenArySI[lastSI] ^=ampLenArySI[onSI];
                  ampLenArySI[onSI] ^=ampLenArySI[lastSI];

                  hitsArySI[onSI] ^= hitsArySI[lastSI];
                  hitsArySI[lastSI] ^= hitsArySI[onSI];
                  hitsArySI[onSI] ^= hitsArySI[lastSI];

                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: check the previous elements*/
            } /*If: i need to swap elements in a subset*/
         } /*Loop: sort all elements in each subset*/
      } /*Loop: go through each subset in distSI*/

      /**************************************************\
      * Fun14 Sec03 Sub04:
      *   - move to next distance/round
      \**************************************************/

       --distSI;
       distSI /= 3;
   } /*Loop: sort till distance between subsets is 0*/
} /*sort_amp_miruTbl*/

/*-------------------------------------------------------\
| Fun15: swap_amp_miruTbl
|   - swaps two amp_miruTbl structs in a miruTbl struct
| Input:
|   - firstIndexI:
|     o index of first lineage to swap
|   - secLineageStr
|     o index of the second lineage to swap
|   - mirTblSTPtr:
|     o pointer to miruTbl struct with lineages to swap
| Output:
|   - Modifies:
|     o lineageStrAry to have the two indexs swaped
|     o lineageIndexIAry to have the original indexs for
|       the two lineages swapped
\-------------------------------------------------------*/
void
swap_amp_miruTbl(
   signed int firstElmSI,
   signed int secElmSI,
   struct miruTbl *miruTblSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC: swap_amp_miruTbl
   '   - swaps two lineages around in a miruTbl structure
   '   o fun15 sec01:
   '     - variable declerations
   '   o fun15 sec02:
   '     - swap primer coordinates
   '   o fun15 sec03:
   '     - swap arrays/values associated with lineages
   '   o fun15 sec04:
   '     - swap primer name
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *firstStr = 0;
   schar *secStr = 0;

   int *ptrSwapSI = 0; /*for swapping interger pionts*/

   struct amp_miruTbl *firstAmpST =
      &miruTblSTPtr->ampAryST[firstElmSI];

   struct amp_miruTbl *secAmpST =
      &miruTblSTPtr->ampAryST[secElmSI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - swap primer coordiantes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   firstAmpST->forPosSI ^= secAmpST->forPosSI;
   secAmpST->forPosSI ^= firstAmpST->forPosSI;
   firstAmpST->forPosSI ^= secAmpST->forPosSI;

   firstAmpST->forLenSI ^= secAmpST->forLenSI;
   secAmpST->forLenSI ^= firstAmpST->forLenSI;
   firstAmpST->forLenSI ^= secAmpST->forLenSI;

   firstAmpST->revPosSI ^= secAmpST->revPosSI;
   secAmpST->revPosSI ^= firstAmpST->revPosSI;
   firstAmpST->revPosSI ^= secAmpST->revPosSI;

   firstAmpST->revLenSI ^= secAmpST->revLenSI;
   secAmpST->revLenSI ^= firstAmpST->revLenSI;
   firstAmpST->revLenSI ^= secAmpST->revLenSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - swap arrays/values associated with lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ptrSwapSI = firstAmpST->ampLenArySI;
   firstAmpST->ampLenArySI = secAmpST->ampLenArySI;
   secAmpST->ampLenArySI = ptrSwapSI;

   ptrSwapSI = firstAmpST->hitsArySI;
   firstAmpST->hitsArySI = secAmpST->hitsArySI;
   secAmpST->hitsArySI = ptrSwapSI;

   ptrSwapSI = firstAmpST->indexArySI;
   firstAmpST->indexArySI = secAmpST->indexArySI;
   secAmpST->indexArySI = ptrSwapSI;

   firstAmpST->numLinSI ^= secAmpST->numLinSI;
   secAmpST->numLinSI ^= firstAmpST->numLinSI;
   firstAmpST->numLinSI ^= secAmpST->numLinSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - swap primer name
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   firstStr = firstAmpST->idStr;
   secStr = secAmpST->idStr;

   while(
         *firstStr > 31
      && *secStr > 31
   ){ /*Loop: Swap the primer names*/
      *firstStr ^= *secStr;
      *secStr ^= *firstStr;
      *firstStr ^= *secStr;
   } /*Loop: Swap the primer names*/

   *firstStr = '\0';
   *secStr = '\0';
} /*swap_amp_miruTbl*/

/*-------------------------------------------------------\
| Fun16: cmpLin_miruTbl
|   - checks to see if the first lineage comes before
|     second lineage on reference
| Input:
|   - firstIndexI:
|     o integer with index of first lineage to swap
|   - secLineageStr
|     o integer with index of the second lineage to swap
|   - mirTblSTPtr:
|     o pointer to a miruTbl structure with lineages to
|       swap
| Output:
|   - Returns:
|     o < 0 if the first linage is less than the second
|     o 0 if the first lineage matches the second
|     o > 0 if th e first lineage is greater than second
\-------------------------------------------------------*/
#define cmpLin_miruTbl(firstElmSI, secElmSI, miruTblSTPtr) ( (miruTblSTPtr)->ampAryST[(firstElmSI)].forPosSI - (miruTblSTPtr)->ampAryST[(secElmSI)].forPosSI )

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC: sort_miruTbl
   '   - sorts miruTbl struct lineages by amp position
   '   o fun17 sec01:
   '     - variable declerations
   '   o fun17 sec02:
   '     - find the first subset (largest gap)
   '   o fun17 Sec03:
   '     - sort my arrays
   '   o fun17 sec04:
   '     - sort amplicon array in miruTblSTPtr
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*some quick pointers to save my sanity*/
   sint numPrimSI = miruTblSTPtr->numPrimSI; 

   uchar lenSwapUC = 0;/*number of characters swaped*/

   sint distSI = 0;/*distance between elements in subset*/
   sint nextSI = 0;/*next element in subset*/
   sint lastSI = 0;/*previous element in subset*/
   sint onSI = 0;  /*element currently sorting*/

   sint siSub = 0; /*subset of index's on in round*/
   sint siElm = 0; /*index on*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - Find the first subset (largest gap)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Adam (2013) mentioned that shell sort needed to have
   `   a subarray of indexs. I took there idea, but
   `   instead of using an array I find the final value &
   `   then backwards calulate the next index in the loop.
   `   This should save some memory.
   ` Adam Drozdek, 2013; Data Structures and
   `   algorithims in c++, Congage Learning, fourth
   `   edition, pages 505 to 508.
   ` Recursion formula: H0 = 1; Hn = 3 * Hn-1 + 1
   */

   distSI = 1;

   while(distSI < numPrimSI - 1)
   { /*Loop: find first subset I am using*/
      distSI *= 3; 
      ++distSI;
   } /*Loop: find first subset I am using*/

   siElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - sort my arrays
   ^   o fun17 sec03 sub01:
   ^     - set up shell short triple loop
   ^   o fun17 sec03 sub02:
   ^     - check if swap elements in a subset
   ^   o fun17 sec03 sub03:
   ^     - check if swap previous elements
   ^   o fun17 sec03 sub04:
   ^     - move to the next distance/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun17 Sec03 Sub01:
   *   - set up shell short triple loop
   \*****************************************************/

   while(distSI > 0)
   { /*Loop: sort till distance between subsets is 0*/

      for(siSub = 0; siSub < distSI; ++siSub)
      { /*Loop: go through each subset in distSI*/

         for(
            siElm = 0;
            siElm + distSI < numPrimSI;
            siElm += distSI
         ){ /*Loop: sort all elements in each subset*/

            /********************************************\
            * Fun17 Sec03 Sub02:
            *   - check if swap elements in a subset
            \********************************************/

            /*get the next index in the subset*/
            nextSI = siElm + distSI;

            if(
                 cmpLin_miruTbl(siElm,nextSI,miruTblSTPtr)
               > 0
            ){ /*If: swap elements in a subset*/

               swap_amp_miruTbl(
                  siElm,
                  nextSI,
                  miruTblSTPtr
               );

               /*****************************************\
               * Fun17 Sec03 Sub03:
               *   - check if to swap previous elements
               \*****************************************/

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= distSI)
               { /*Loop: check the previous elements*/
                  lastSI -= distSI;

                   if(
                      cmpLin_miruTbl(
                         onSI,
                         lastSI,
                         miruTblSTPtr
                      ) > 0
                    ) break;

                    swap_amp_miruTbl(
                       onSI,
                       lastSI,
                       miruTblSTPtr
                    ); /*move the amplicons around*/

                  if(lenSwapUC >= def_lenStr_miruTbl - 1)
                     return 1; /*array out of bound*/

                  onSI = lastSI;
               } /*Loop: check the previous elements*/
            } /*If: swap elements in a subset*/
         } /*Loop: sort all elements in each subset*/
      } /*Loop: go through each subset in distSI*/

      /**************************************************\
      * Fun17 Sec03 Sub04:
      *   - move to the next distance/round
      \**************************************************/

       --distSI;
       distSI /= 3;
   } /*Loop: sort till distance between subsets is 0*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - sort amplicon array in miruTblSTPtr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      siElm = 0;
      siElm < numPrimSI;
      ++siElm
   ) sort_amp_miruTbl(&miruTblSTPtr->ampAryST[siElm]);

   return 0;
} /*sort_miruTbl*/

/*-------------------------------------------------------\
| Fun18: getAmpIndex_miruTbl
|   - get index of amp_miruTbl struct in input range from
|     an miruTbl struct
| Input:
|   - startSI:
|     o first base in the amplicon to search for (index 0)
|   - endSI:
|     o last base in the amplicon to search for (index 0)
|   - miruTblSTPtr:
|     o pointer to miruTbl structure to search
| Output:
|   - Returns:
|     o index of matching lineage
|     o -1 if there were no MIRU amplicons in the input
|       range (startSI to endSI)
\-------------------------------------------------------*/
signed int
getAmpIndex_miruTbl(
   signed int startSI,   /*start of amplicon*/
   signed int endSI,     /*end of amplicon*/
   struct miruTbl *miruTblSTPtr
){
   sint midSI = 0;
   sint rightSI = 0;
   sint leftSI = 0;

   struct amp_miruTbl *tmpST = miruTblSTPtr->ampAryST;

   rightSI = miruTblSTPtr->numPrimSI - 1;

   while(leftSI <= rightSI)
   { /*Loop: Find the target index*/
      midSI = (leftSI + rightSI) >> 1;

      if(tmpST[midSI].forPosSI > endSI)
         rightSI = midSI - 1;

      else if(tmpST[midSI].revPosSI < startSI)
         leftSI = midSI + 1;

      else break;
   } /*Loop: Find the target index*/

   if(tmpST[midSI].forPosSI > endSI)
      return -1;

   if(tmpST[midSI].revPosSI < startSI)
      return -1;

   while(
         midSI > 0
      && tmpST[midSI - 1].forPosSI >= startSI
   ) --midSI;

   return midSI;
} /*getAmpIndex_miruTbl*/

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
){
   sint midSI = 0;
   sint rightSI = 0;
   sint leftSI = 0;

   sint startSI = lenAmpI - fudgeSI;
   sint endSI = lenAmpI + fudgeSI;

   sint *ampArySI =
     miruTblSTPtr->ampAryST[ampIndexSI].ampLenArySI;

   rightSI = miruTblSTPtr->numLinSI - 1;

   while(leftSI <= rightSI)
   { /*Loop: find target index*/
      midSI = (leftSI + rightSI) >> 1;

      if(ampArySI[midSI] > endSI)
         rightSI = midSI - 1;

      else if(ampArySI[midSI] < startSI)
         leftSI = midSI + 1;

      else
      { /*Else: found amplicon length*/
         /*find first match in length range*/

         while(
               midSI > 0
            && ampArySI[midSI - 1] >= startSI
         ) --midSI;

         return midSI; /*Found my index*/
     } /*Else: found amplicon length*/
   } /*Loop: find target index*/

   if(ampArySI[midSI] > endSI)
      return -1;

   if(ampArySI[midSI] < startSI)
      return -1;

   /*find first match in length range*/
   while(
         midSI > 0
      && ampArySI[midSI - 1] >= startSI
   ) --midSI;

   return midSI;
} /*getLinIndex_miruTbl*/

/*-------------------------------------------------------\
| Fun20: getAmpLen_miruTbl
|   - finds length of MIRU region in a sam file entry
| Input:
|   - startSI:
|     o starting position of MIRU amplicon
|   - endAmpSI:
|     o ending position of MIRU amplicon
|   - refPosSIPtr:
|     o pointer to integer with current reference base
|   - cigPosIPtr:
|     o pionter to integer with bases left in current
|       cigar entry
|   - cigIndexUIPtr:
|     o pointer to unsigned integer with current cigar
|       entry index
|   - samSTPtr:
|     o pointer to a samEntry struct with amplicon
| Output:
|   - Returns:
|     o read length
|     o -1 if the range (startSI to endAmpSI) was longer
|       than the reads (samSTPtr) reference range.
\-------------------------------------------------------*/
signed int
getAmpLen_miruTbl(
   signed int startSI,     /*starting of read*/
   signed int endAmpSI,    /*end of read*/
   signed int *refPosSIPtr, /*position in reference*/
   signed int *cigPosIPtr, /*num cigar entry bases left*/
   unsigned int *cigIndexUIPtr, /*cigar entry on*/
   struct samEntry *samSTPtr
){ 
   sint seqPosSI = 0;   /*temporary variable*/
   sint seqStartSI = 0; /*temporary variable*/

   findRefPos_samEntry(
      samSTPtr,
      (sint *) cigIndexUIPtr,
      cigPosIPtr,
      startSI,
      refPosSIPtr,
      &seqPosSI
   ); /*find start of amplicon*/

   /*If: there is an incomplete mapping*/
   if(*refPosSIPtr != startSI)
      return -1;

   seqStartSI = seqPosSI;

   findRefPos_samEntry(
      samSTPtr,
      (sint *) cigIndexUIPtr,
      cigPosIPtr,
      endAmpSI,
      refPosSIPtr,
      &seqPosSI
   ); /*find length of amplicon/read*/

   if(*refPosSIPtr != endAmpSI)
      return -1;

   return seqPosSI - seqStartSI;
} /*getAmpLen_miruTbl*/

/*-------------------------------------------------------\
| Fun21: incLineage_miruTbl
|   - scans reads for MIRU amplicons and incurments
|     counter for every found lineage
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with amplicon
|   - fudgeSI:
|     o offset length by when comparing to lingeage length
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
   signed int fudgeSI,        /*offset length by*/
   struct miruTbl *miruTblSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun21 TOC: incLineage_miruTbl
   '   - scans reads for MIRU amplicons and incurments
   '     counter for every found lineage
   '   o fun21 sec01:
   '     - variable declerations
   '   o fun21 sec02:
   '     - find first MIRU amplicon in read/consensus
   '   o fun21 sec03:
   '     - find mapped lineages, update hits for mapped,
   '       lineages, and see if any more lineages in read
   '   o fun21 sec04:
   '     - let user know lineage(s) were found
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint ampIndexSI = 0;
   sint linIndexSI = 0;
   sint *hitArySI = 0;
   sint *lenArySI = 0;
   sint numLinSI = miruTblSTPtr->numLinSI;

   /*Start and end of the amplicon lengths*/
   sint startSI = 0;
   sint endAmpSI = 0;

   /*Stats on the reads*/
   sint refPosSI = 0;
   sint readLenSI = 0;

   /*Sam entry variables*/
   uint cigIndexUI = 0;
   sint cigPosOnSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec02:
   ^   - find first MIRU amplicon in read/consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refPosSI = (int) samSTPtr->refStartUI;
   
   ampIndexSI =
      getAmpIndex_miruTbl(
         refPosSI,
         (sint) samSTPtr->refEndUI,
         miruTblSTPtr
      );

   if(ampIndexSI < 0)
      goto noLineage_fun21_sec04;

   cigPosOnSI = samSTPtr->cigArySI[0];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec03:
   ^   - find mapped lineages, update hits for mapped,
   ^     lineages, and see if any more lineages in read
   ^   o fun21 sec03 sub01:
   ^      - find the length of the amplicon in the read
   ^   o fun21 sec03 sub02:
   ^     - find lineage index and incrument the hits for
   ^       all lineages having the reads amplicon length
   ^   o fun21 sec03 sub03:
   ^     - see if there are more MIRU amplicons in read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun21 Sec03 Sub01:
   *   - Find the length of the amplicon in the read
   \*****************************************************/

   endAmpSI= miruTblSTPtr->ampAryST[ampIndexSI].revPosSI;

   while(endAmpSI <= (sint) samSTPtr->refEndUI)
   { /*Loop: update table for each MIRU amp in read*/
      startSI =
         miruTblSTPtr->ampAryST[ampIndexSI].forPosSI;

      readLenSI = 
         getAmpLen_miruTbl(
            startSI,     /*start position of MIRU amp*/
            endAmpSI,    /*endi position of MIRU am*/
            &refPosSI,   /*current reference position*/
            &cigPosOnSI, /*bases in current cigar entry*/
            &cigIndexUI, /*index on in cigar*/
            samSTPtr
         ); /*Find the MIRU amplicons length in the read*/

      /**************************************************\
      * Fun21 Sec03 Sub02:
      *   - find lineage index and incrument the hits for
      *     all lineages having the reads amplicon length
      \**************************************************/

      if(readLenSI > -1)
      { /*If: have the read length, find lineage*/
         linIndexSI =
            getLinIndex_miruTbl(
               readLenSI,    /*amplicon length*/
               fudgeSI,      /*offset length by*/
               ampIndexSI,  /*ampicon index to search*/
               miruTblSTPtr
            ); /*Get index of matching lineage length*/
      } /*If: have the read length, find lineage*/

      else
         goto nextAmp_fun21_sec03_sub03;

      /*incurment read counter for lineage*/
      ++(miruTblSTPtr->ampAryST[ampIndexSI].mapReadsSI);
 
      if(linIndexSI >= 0)
      { /*If: have lineages to incurment*/
         lenArySI =
            miruTblSTPtr->ampAryST[
               ampIndexSI
            ].ampLenArySI; /*get the length array*/

         /*get hit counts array*/
         hitArySI =
            miruTblSTPtr->ampAryST[ampIndexSI].hitsArySI;

         while(
               lenArySI[linIndexSI] - fudgeSI <= readLenSI
            && lenArySI[linIndexSI] + fudgeSI >= readLenSI
         ){ /*Loop: incurment each lineage hit*/
            ++hitArySI[linIndexSI];
            ++linIndexSI;

            if(linIndexSI >= numLinSI)
               break; /*hit out of bounds*/
         } /*Loop: incurment each lineage hit*/
      } /*If: have lineages to incurment*/

      else
        ++(miruTblSTPtr->ampAryST[ampIndexSI].noLinSI);
        /*no lineage found*/

      /**************************************************\
      * Fun21 Sec03 Sub03:
      *   - see if there are more MIRU amplicons in read
      \**************************************************/

      nextAmp_fun21_sec03_sub03:;

      ampIndexSI++;

      endAmpSI =
         miruTblSTPtr->ampAryST[ampIndexSI].revPosSI;

      if(ampIndexSI >= miruTblSTPtr->numPrimSI)
         break; /*checked all possible amplicons*/
   } /*Loop: update table for each MIRU amp in read*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec04:
   ^   - let user know lineage(s) were found
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   noLineage_fun21_sec04:;
   return 1; /*lineage not in table*/
} /*incLineage_miruTbl*/

/*-------------------------------------------------------\
| Fun22: unsort_amp_miruTbl
|   - unsorts a amp_miruTbl struct to original input order
| Input:
|   - amp_miruTblSTPtr:
|     o pointer to an amp_miruTbl structure to unsort
|   - numLinSI:
|     o number of lineages in amp_miruTblSTPtr
| Output:
|   - Modifies:
|     o amp_miruTblSTPtr to be unsorted
\-------------------------------------------------------*/
void
unsort_amp_miruTbl(
   struct amp_miruTbl *amp_miruTblSTPtr /*To be unsorted*/
){
   /*These are so I have an easier work flow*/
   sint *linArySI = amp_miruTblSTPtr->ampLenArySI;
   sint *hitsArySI = amp_miruTblSTPtr->hitsArySI;
   sint *indexArySI = amp_miruTblSTPtr->indexArySI;

   sint siElm = 0; /*lineage on*/
   sint swapElmSI = 0; /*lineage on*/

   while(siElm < (amp_miruTblSTPtr)->numLinSI)
   { /*Loop: Unsort the lineage array*/
      if(siElm != indexArySI[siElm])
      { /*If: need to swap entries around*/
         swapElmSI = indexArySI[siElm];

         linArySI[siElm] ^= linArySI[swapElmSI];
         linArySI[swapElmSI] ^= linArySI[siElm];
         linArySI[siElm] ^= linArySI[swapElmSI];

         hitsArySI[siElm] ^= hitsArySI[swapElmSI];
         hitsArySI[swapElmSI] ^= hitsArySI[siElm];
         hitsArySI[siElm] ^= hitsArySI[swapElmSI];

         indexArySI[siElm] ^= indexArySI[swapElmSI];
         indexArySI[swapElmSI] ^= indexArySI[siElm];
         indexArySI[siElm] ^= indexArySI[swapElmSI];
      } /*If: need to swap entries around*/

      else
         ++siElm;
   } /*Loop: unsort the lineage array*/
} /*unsort_amp_miruTbl*/

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
){
   sint numLinSI = miruTblSTPtr->numPrimSI;
   sint *linArySI = miruTblSTPtr->indexArySI;

   /*for easier work flow*/
   struct amp_miruTbl *ampSTAry = miruTblSTPtr->ampAryST;

   sint ssiLin = 0; /*lineage on*/
   sint swapElmSI = 0;

   while(ssiLin < numLinSI)
   { /*Loop: unsort lineage array*/
      if(ssiLin != linArySI[ssiLin])
      { /*If: need to swap entries around*/
         swapElmSI = linArySI[ssiLin];

         swap_amp_miruTbl(
            ssiLin,
            swapElmSI,
            miruTblSTPtr
         );
      } /*If: need to swap entries around*/

      else ++ssiLin;
   } /*Loop: unsort lineage array*/

   /*unsort amplicons*/
   for(
      ssiLin = 0;
      ssiLin < numLinSI;
      ++ssiLin
   ) unsort_amp_miruTbl(&ampSTAry[ssiLin]);
} /*unsort_miruTbl*/

/*-------------------------------------------------------\
| Fun24: readLine_miruTbl
|   - reads a line from an miruTbl
| Input:
|   - buffStrPtr:
|     o pointer to a c-string buffer to add the line to
|   - posSIPtr:
|     o pionter to integer with position at in buffer
|   - lenBuffSIPtr:
|     o pointer to integer with length of buffer
|   - bytesSIPtr:
|     o pointer to integer to hold number of characters
|       currently in the buffer
|   - tblFILE:
|     o file to get the next line from
| Output:
|   - Modifies:
|     o posSIPttr to be 0
|     o buffStr to hold at least the next line
|     o bytesSIPtr to hold number of characters in
|       the buffer (index 1)
|   - Returns:
|     o 0 for no errors
|     o def_EOF_tbMiruDefs if at end of file
|     o def_memErr_tbMiruDefs for memory errors
\-------------------------------------------------------*/
unsigned long
readLine_miruTbl(
   signed char **buffStrPtr,
   signed int *posSIPtr,
   signed int *lenBuffSIPtr,
   signed int *bytesSIPtr,
   void *tblFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun24 TOC: readLine_miruTbl
   '   - reads a line from an miruTbl
   '   o fun24 sec01:
   '     - variable declerations
   '   o fun24 sec02:
   '     - check if I have an open buffer
   '   o fun24 sec03:
   '     - read in the next line
   '   o fun24 sec04:
   '     - finish setting variables and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   schar *tmpStr = *buffStrPtr;
   sint buffSpaceSI = 0;

   ulong totalBytesUL = 0;
   ulong charReadUL = 0;
   ulong checkUL = 0;
   
   ulong *ulStr = 0;
   sint byteOnSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec02:
   ^   - check if I have an open buffer
   ^   o fun24 sec02 sub01:
   ^     - check if have data in the buffer already
   ^   o fun24 sec02 sub02:
   ^     - see if there is a new line already in buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun24 Sec02 Sub01:
   *   - check if have data in the buffer already
   \*****************************************************/

   if(*(posSIPtr))
   { /*If: there is extra space in the buffer*/
      buffSpaceSI = *(posSIPtr);
      totalBytesUL = *(lenBuffSIPtr) - buffSpaceSI;

      cpLen_ulCp(
         *buffStrPtr,
         tmpStr,
         totalBytesUL
      ); /*Copy the unused part of the buffer over*/

      *(posSIPtr) = 0;
      tmpStr = *(buffStrPtr) + totalBytesUL;
   } /*If: there is extra space in the buffer*/

   else
      buffSpaceSI = *(lenBuffSIPtr);

   /*****************************************************\
   * Fun24 Sec02 Sub02:
   *   - see if there is a new line already in buffer
   \*****************************************************/

    ulStr = (ulong *) tmpStr;

    for(
       byteOnSI = 0;
       byteOnSI
          < (sint) (totalBytesUL >> def_shiftULBy_ulCp);
       ++byteOnSI
    ){ /*Loop: See if I have a new line*/
       checkUL = *ulStr ^ def_newline_ulCp;
       checkUL -= def_one_ulCp;
       checkUL = checkUL & def_highBit_ulCp;

       if(checkUL)
          break;

       ++ulStr;
    } /*Loop: See if I have a new line*/

   tmpStr = (schar *) ulStr;
   byteOnSI = 0;

   while(byteOnSI < (sint)(totalBytesUL & def_modUL_ulCp))
   { /*Loop: Check the last few bytes*/
      if(*tmpStr == '\n')
         break;

      ++tmpStr;
      ++byteOnSI;
   } /*Loop: Check the last few bytes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec03:
   ^   - read in the next line
   ^   o fun24 sec03 sub01:
   ^     - start loop and get input from file
   ^   o fun24 sec03 sub02:
   ^     - see if there is a new line in the buffer
   ^   o fun24 sec03 sub03:
   ^     - resize the buffer to get more input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun24 Sec03 Sub01:
   *   - Start loop and get input from file
   \*****************************************************/

   while(*tmpStr != '\n')
   { /*Loop: read in first line*/
      charReadUL =
         fread(
            (char *) tmpStr,
            sizeof(char),
            buffSpaceSI - 1,
            (FILE *) (tblFILE)
         ); /*read in next chunk of file*/

       totalBytesUL += charReadUL;

       if(charReadUL < (ulong) buffSpaceSI - 1)
       { /*If: could not read any lines from the file*/
          errSC = def_EOF_tbMiruDefs;
          break;
       } /*If: could not read any lines from the file*/

      /**************************************************\
      * Fun24 Sec03 Sub02:
      *   - check for new line in buffer
      \**************************************************/

       ulStr = (ulong *) tmpStr;

       for(
          byteOnSI = 0;
          byteOnSI
            < (sint) (totalBytesUL >> def_shiftULBy_ulCp);
          ++byteOnSI
       ){ /*Loop: see if I have a new line*/
          checkUL = *ulStr ^ def_newline_ulCp;
          checkUL -= def_one_ulCp;
          checkUL = checkUL & def_highBit_ulCp;

          if(checkUL)
             break;

          ++ulStr;
       } /*Loop: see if I have a new line*/

      tmpStr = (schar *) ulStr;
      byteOnSI = 0;

      while(
         byteOnSI < (sint) (totalBytesUL & def_modUL_ulCp)
      ){ /*Loop: check last few bytes*/
         if(*tmpStr == '\n')
            break;

         ++tmpStr;
         ++byteOnSI;
      } /*Loop: check last few bytes*/

      if(*tmpStr == '\n')
         break; /*end of line*/

      /**************************************************\
      * Fun24 Sec03 Sub03:
      *   - resize buffer to get more input
      \**************************************************/

      *(lenBuffSIPtr) <<= 1;

      tmpStr =
         realloc(
            *buffStrPtr,
            (*lenBuffSIPtr + 1) * sizeof(schar)
         ); /*get larger buffer*/

      if(! tmpStr)
      { /*If: I had a memory error*/
         errSC = def_memErr_tbMiruDefs;
         break;
      } /*If: I had a memory error*/

      *buffStrPtr = tmpStr;
      tmpStr += totalBytesUL;
      buffSpaceSI = *lenBuffSIPtr;
      (*lenBuffSIPtr) <<= 1;
   } /*Loop: Read in the first line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec04:
   ^   - Finish setting variables and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *(*buffStrPtr + totalBytesUL) = '\0';

   *(posSIPtr) = 0;
   *(bytesSIPtr) = (sint) totalBytesUL;

   return errSC;
} /*readLine_miruTbl*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun25 TOC: get_miruTbl
   '   - read in a MIRU table from a tsv file
   '   o fun25 sec01:
   '     - variable declerations
   '   o fun25 sec02:
   '     - allocate memory and get the first line
   '   o fun25 sec03:
   '     - find number of lines (lineages) in file
   '   o fun25 sec04:  
   '     - get postion and length of each pimer
   '   o fun25 sec05:
   '     - get each lineages length for each primer
   '   o fun25 sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint lenBuffSI = 1 << 11;
   schar *buffHeapStr = 0;
   sint posSI = 0;
   schar errSC = 0;

   schar *tmpStr = 0;
   sint totalBytesSI = 0;

   sint siAmp = 0; /*Amplicon I am currently on*/
   sint siLin = 0; /*Lineage on in loop*/

   struct miruTbl *tblHeapST = 0;

   FILE *tblFILE =
      fopen(
         (char *) miruTblStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec02:
   ^   - allocate memory and get the first line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! tblFILE)
      goto fileErr_get_miruTbl;

   buffHeapStr = calloc((lenBuffSI + 1), sizeof(schar));

   if(! buffHeapStr)
      goto memErr_get_miruTbl;

   tblHeapST = malloc(sizeof(struct miruTbl));

   if(! tblHeapST)
      goto memErr_get_miruTbl;

   init_miruTbl(tblHeapST);

   errSC =
      readLine_miruTbl(
         &buffHeapStr,
         &posSI,
         &lenBuffSI,
         &totalBytesSI,
         tblFILE
      );

   if(errSC & def_memErr_tbMiruDefs)
      goto memErr_get_miruTbl;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec03:
   ^   - find number of lines (lineages) in file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = buffHeapStr;

   while(! errSC)
   { /*Loop: read in each line*/
      tmpStr += endLine_ulCp(tmpStr);
      tblHeapST->numLinSI += (*tmpStr == '\n');
      ++tmpStr; /*get off new line*/

      /*check if I have more characters in the buffer*/
      if(tmpStr < buffHeapStr + totalBytesSI)
         continue;

      errSC =
         readLine_miruTbl(
            &buffHeapStr,
            &posSI,
            &lenBuffSI,
            &totalBytesSI,
            tblFILE
         );

      tmpStr = buffHeapStr;
   } /*Loop: read in each line*/

   if(errSC & def_memErr_tbMiruDefs)
      goto memErr_get_miruTbl;

   /*need to finsh reading the file*/
   while(posSI < totalBytesSI)
   { /*Loop: find the end of the file*/
      posSI += endLine_ulCp(&buffHeapStr[posSI]);
      tblHeapST->numLinSI += (buffHeapStr[posSI] =='\n');
      ++posSI; /*get off the new line*/
   } /*Loop: find the end of the file*/

   /*account for overcounting files that end in newline*/
   tblHeapST->numLinSI -=
      (buffHeapStr[posSI - 1] == '\n');

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec04:
   ^   - get postion and length of each pimer
   ^   o fun25 sec04 sub01:
   ^     - find the number of amplicons in the header
   ^   o fun25 sec04 sub02:
   ^     - assign memory for the amplicons
   ^   o fun25 sec04 sub03:
   ^     - extract stats for each amplicon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec04 Sub01:
   *   - find the number of amplicons in the header
   \*****************************************************/

   fseek(
      tblFILE,
      0,
      SEEK_SET
   );

   if(tblHeapST->numLinSI < 2)
      goto fileErr_get_miruTbl;

   /*I have already know this line exists*/
   tmpStr =
      (schar *)
      fgets(
         (char *) buffHeapStr,
         lenBuffSI,
         tblFILE
      );

   /*count number of amplicons. last column ends in '\n'
   `  (no '\t') so am safe counting first '\t'
   */
   while(*tmpStr++ != '\n') 
      tblHeapST->numPrimSI += (*tmpStr == '\t');

   /*****************************************************\
   * Fun25 Sec04 Sub02:
   *   - assign memory for amplicons
   \*****************************************************/

   tblHeapST->lineageStrAry =
      malloc(
           (tblHeapST->numLinSI * def_lenStr_miruTbl)
         * sizeof(schar)
      ); /*Assign memory for the lineage name array*/

   if(! tblHeapST->lineageStrAry)
      goto memErr_get_miruTbl;

   tblHeapST->ampAryST =
      calloc(
         tblHeapST->numPrimSI,
         sizeof(struct amp_miruTbl)
      );

   if(! tblHeapST->ampAryST)
      goto memErr_get_miruTbl;

   tblHeapST->indexArySI =
      calloc(
         tblHeapST->numPrimSI,
         sizeof(sint)
      );

   if(! tblHeapST->indexArySI)
      goto memErr_get_miruTbl;

   for(
      siAmp = 0;
      siAmp < tblHeapST->numPrimSI;
      ++siAmp
   ){ /*Loop: allocate memory for lineages in primer*/
      tblHeapST->ampAryST[siAmp].ampLenArySI =
         malloc(tblHeapST->numLinSI * sizeof(sint));

      if(! tblHeapST->ampAryST[siAmp].ampLenArySI)
         goto memErr_get_miruTbl;

      tblHeapST->ampAryST[siAmp].hitsArySI =
         malloc(tblHeapST->numLinSI * sizeof(sint));

      if(! tblHeapST->ampAryST[siAmp].hitsArySI)
         goto memErr_get_miruTbl;

      tblHeapST->ampAryST[siAmp].indexArySI =
         malloc(tblHeapST->numLinSI * sizeof(sint));

      if(! tblHeapST->ampAryST[siAmp].indexArySI)
         goto memErr_get_miruTbl;

      tblHeapST->ampAryST[siAmp].numLinSI =
         tblHeapST->numLinSI;

      tblHeapST->indexArySI[siAmp] = siAmp;
   } /*Loop: allocate memroy for lineages in primer*/

   /*****************************************************\
   * Fun25 Sec04 Sub03:
   *   - extract stats for each amplicon
   *   o fun25 sec04 sub03 cat01:
   *     - get past lineage column & start primer name
   *       extraction loop
   *   o fun25 sec04 sub03 cat02:
   *     - get primer name
   *   o fun25 sec04 sub03 cat03:
   *     - get foward starting position
   *   o fun25 sec04 sub03 cat04:
   *     - get foward primers length
   *   o fun25 sec04 sub03 cat05:
   *     - get reverse primers starting position
   *   o fun25 sec04 sub03 cat06:
   *     - get reverse primers starting position
   *   o fun25 sec04 sub03 cat07:
   *     - get reverse primers length
   *   o fun25 sec04 sub03 cat08:
   *     - move to next primers entry
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun25 Sec04 Sub03 Cat01:
   +   - get past lineage column & start primer name loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = buffHeapStr;
   while(*tmpStr++ != '\t') ;

   for(
      siAmp = 0;
      siAmp < tblHeapST->numPrimSI;
      ++siAmp
   ){ /*Loop: extract each primers stats*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat02:
      +   - get primer name
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpStr +=
         cpDelim_charCp(
            tblHeapST->ampAryST[siAmp].idStr,
            tmpStr,
            '.'
         ); /*Copy the primer id*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat03:
      +   - get foward starting position
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.')
         goto primNameErr_get_miruTbl;

      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_get_miruTbl;
  
      tmpStr +=
         strToSI_base10str(
            tmpStr,
            &tblHeapST->ampAryST[siAmp].forPosSI
         );

      /*convert to index 0*/
      --tblHeapST->ampAryST[siAmp].forPosSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat04:
      +   - get foward primers length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.')
         goto primNameErr_get_miruTbl;

      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_get_miruTbl;
  
      tmpStr +=
         strToSI_base10str(
            tmpStr,
            &tblHeapST->ampAryST[siAmp].forLenSI
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat05:
      +   - get reverse primers starting position
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.')
         goto primNameErr_get_miruTbl;

      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_get_miruTbl;
  
      tmpStr +=
         strToSI_base10str(
            tmpStr,
            &tblHeapST->ampAryST[siAmp].revPosSI
         );

      /*Convert to index 0*/
      --tblHeapST->ampAryST[siAmp].revPosSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat06:
      +   - get reverse primers length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.')
         goto primNameErr_get_miruTbl;

      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_get_miruTbl;
  
      tmpStr +=
         strToSI_base10str(
            tmpStr,
            &tblHeapST->ampAryST[siAmp].revLenSI
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub03 Cat07:
      +   - move to next primers entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(*tmpStr++ > 31) ;
   } /*Loop: Extract each primers stats*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec05:
   ^   - get each lineages length for each primer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siLin = 0; /*Lineage on*/

   while(fgets((char *) buffHeapStr, lenBuffSI, tblFILE))
   { /*Loop: get the row for each linage from the file*/
      siAmp = 0;
      tmpStr = buffHeapStr;

      /*get lineages name*/
      tmpStr +=
         cpDelim_charCp(
            &tblHeapST->lineageStrAry[
               strIndex_miruTbl(siLin)
            ],
            tmpStr,
            '\t'
         ); /*copy lineages name*/

      while(*tmpStr & (~ '\n'))
      { /*Loop: get lineages length for each primer*/
         ++tmpStr;

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &tblHeapST->ampAryST[
                  siAmp
               ].ampLenArySI[siLin]
            );

         /*record the original index*/
         tblHeapST->ampAryST[siAmp].indexArySI[siLin] =
            siLin;

         tblHeapST->ampAryST[siAmp].hitsArySI[siLin] = 0;

         ++siAmp;
      } /*Loop: get lineages length for each primer*/

      ++siLin; /*move to next lineage*/
   } /*Loop: get row for each linage from the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec06:
   ^   - clean up
   ^   o fun25 sec06 sub01:
   ^     - clean up for success
   ^   o fun25 sec06 sub02:
   ^     - clean up for memory errors
   ^   o fun25 sec06 sub03:
   ^     - clean up for file errors
   ^   o fun25 sec06 sub04:
   ^     - clean up for invalid primer names in table
   ^   o fun25 sec06 sub05:
   ^     - general error clean up (all errors)
   ^   o fun25 sec06 sub06:
   ^     - general clean up (always runs)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec06 Sub01:
   *   - clean up for success
   \*****************************************************/

   sort_miruTbl(tblHeapST);
   *errSCPtr = 0;
   goto cleanUp_fun25_sec06_sub06;

   /*****************************************************\
   * Fun25 Sec06 Sub02:
   *   - clean up for memory errors
   \*****************************************************/

   memErr_get_miruTbl:;
   *errSCPtr = def_memErr_tbMiruDefs;
   goto errSCleanUp_fun25_sec06_sub05;

   /*****************************************************\
   * Fun25 Sec06 Sub03:
   *   - clean up for file errors
   \*****************************************************/

   fileErr_get_miruTbl:;
   *errSCPtr = def_fileErr_tbMiruDefs;
   goto errSCleanUp_fun25_sec06_sub05;

   /*****************************************************\
   * Fun25 Sec06 Sub04:
   *   - clean up for invalid primer names in table
   \*****************************************************/

   primNameErr_get_miruTbl:;
   *errSCPtr = def_idInvalid_tbMiruDefs;
   goto errSCleanUp_fun25_sec06_sub05;

   /*****************************************************\
   * Fun25 Sec06 Sub05:
   *   - general error clean up (all errors)
   \*****************************************************/

   errSCleanUp_fun25_sec06_sub05:;

   if(tblHeapST)
      freeHeap_miruTbl(tblHeapST);

   tblHeapST = 0;

   goto cleanUp_fun25_sec06_sub06;

   /*****************************************************\
   * Fun25 Sec06 Sub06:
   *   - general clean up (always runs)
   \*****************************************************/

   cleanUp_fun25_sec06_sub06:;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

   if(tblFILE)
      fclose(tblFILE);

   tblFILE = 0;
 
   return tblHeapST;
} /*get_miruTbl*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun26 TOC: p_miruTbl
   '   - prints MIRU table to file; number hits per lineage
   '   o fun26 sec01:
   '     - variable declerations
   '   o fun26 sec02:
   '     - open output file
   '   o fun26 sec03:
   '     - print header (MIRU primers)
   '   o fun26 sec04:
   '     - print table of hits and lineage names
   '   o fun26 sec05:
   '     - print final parts and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuffSI (2 << 11) /*around 4kb*/
   schar buffStr[lenBuffSI]; /*around 4kb*/
   sint posSI = 0;
   sint siLin = 0;
   sint siAmp = 0;
   sint linIndexSI = 0; /*for sanity*/
   ulong percMappedUL = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec02:
   ^   - Check I I can open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: user supplied an file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
         return def_fileErr_tbMiruDefs;
   } /*Else: user supplied an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec03:
   ^   - print header (MIRU primers)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*need to unsort the table, otherwise the output
   `   table will be garbage
   */
   unsort_miruTbl(miruTblSTPtr);

   posSI +=
      cpDelim_ulCp(
         &buffStr[posSI],
         (schar *) "Lineages",
         0,
         '\0'
      );

   for(
      siAmp = 0;
      siAmp < miruTblSTPtr->numPrimSI;
      ++siAmp
   ){ /*Loop: print primer names*/
      if(lenBuffSI - posSI < (def_lenStr_miruTbl << 2))
      { /*If: need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: need to print out the buffer*/

      buffStr[posSI++] = '\t';

      posSI +=
         cpDelim_ulCp(
            &buffStr[posSI],
            miruTblSTPtr->ampAryST[siAmp].idStr,
            0,
            '\0'
         ); /*copy amplicons name*/

      buffStr[posSI++] = 'H';
      buffStr[posSI++] = 'i';
      buffStr[posSI++] = 't';

      buffStr[posSI++] = '\t';

      posSI +=
         cpDelim_ulCp(
            &buffStr[posSI],
            miruTblSTPtr->ampAryST[siAmp].idStr,
            0,
            '\0'
         ); /*copy amplicons name*/

      buffStr[posSI++] = 'P';
      buffStr[posSI++] = 'e';
      buffStr[posSI++] = 'r';
      buffStr[posSI++] = 'c';

      buffStr[posSI++] = '\t';

      posSI +=
         cpDelim_ulCp(
            &buffStr[posSI],
            miruTblSTPtr->ampAryST[siAmp].idStr,
            0,
            '\0'
         ); /*copy amplicons name*/

      buffStr[posSI++] = 'T';
      buffStr[posSI++] = 'o';
      buffStr[posSI++] = 't';
      buffStr[posSI++] = 'a';
      buffStr[posSI++] = 'l';
   } /*Loop: print primer names*/

   if(lenBuffSI - posSI < def_lenStr_miruTbl)
   { /*If: need to print out the buffer*/
      fwrite(
         (char *) buffStr,
         sizeof(char),
         posSI,
         outFILE
      );

      posSI = 0;
   } /*If: need to print out the buffer*/

   buffStr[posSI++] = '\n';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec04:
   ^   - print table of hits and lineage names
   ^   o fun26 sec04 sub01:
   ^     - start lineage loop and copy lineage name
   ^   o fun26 sec04 sub02:
   ^     - start primer loop & print # hits per lineage
   ^   o fun26 sec04 sub03:
   ^     - end lineages row with an newline
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun26 Sec04 Sub01:
   *   - start lineage loop and copy lineage name
   \*****************************************************/

   for(
      siLin=0;
      siLin < miruTblSTPtr->numLinSI;
      ++siLin
   ){ /*Loop: copy entries for each lineage*/

      if(lenBuffSI - posSI < def_lenStr_miruTbl)
      { /*If: need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: need to print out the buffer*/

      /*copy lineage name, they should be sort*/
      linIndexSI = strIndex_miruTbl(siLin);

      posSI +=
         cpDelim_charCp(
            &buffStr[posSI],
            &miruTblSTPtr->lineageStrAry[linIndexSI],
            '\0'
         );

      /**************************************************\
      * Fun26 Sec04 Sub02:
      *   - start primer loop & print # hits per lineage
      *   o fun26 sec04 sub02 cat01:
      *     - copy number of hits to the buffer
      *   o fun26 sec04 sub02 cat02:
      *     - find and copy the percentage of mapped reads
      *   o fun26 sec04 sub02 cat03:
      *     - copy the total number of reads to buffer
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun26 Sec04 Sub02 Cat01:
      +   - copy number of hits to the buffer
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      for(
         siAmp=0;
         siAmp < miruTblSTPtr->numPrimSI;
         ++siAmp
      ){ /*Loop: Print the number of hits per primer*/

         if(lenBuffSI - posSI < def_lenStr_miruTbl + 32)
         { /*If: need to print out the buffer*/
            fwrite(
               (char *) buffStr,
               sizeof(char),
               posSI,
               outFILE
            );

            posSI = 0;
         } /*If: need to print out the buffer*/

         buffStr[posSI++] = '\t';

         posSI +=
            numToStr(
               &buffStr[posSI],
               miruTblSTPtr->ampAryST[
                  siAmp
               ].hitsArySI[siLin]
            ); /*copy number of hits to lineage*/

         buffStr[posSI++] = '\t';

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun26 Sec04 Sub02 Cat02:
         +   - find and copy percentage of mapped reads
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(lenBuffSI - posSI < def_lenStr_miruTbl)
         { /*If: I need to print out the buffer*/
            fwrite(
               (char *) buffStr,
               sizeof(char),
               posSI,
               outFILE
            );

            posSI = 0;
         } /*If: I need to print out the buffer*/

         if(! miruTblSTPtr->ampAryST[siAmp].mapReadsSI)
         { /*If: no reads mapped to this primer*/
            buffStr[posSI++] = '0';
            buffStr[posSI++] = '\t';
            buffStr[posSI++] = '0';

            goto linCpTotal_fun26_sec04_sub02_cat03;
         } /*If: no reads mapped to this primer*/

         /*allows me to keep the percentage as integer*/
         percMappedUL =
              10000
            * miruTblSTPtr->ampAryST[
                 siAmp
              ].hitsArySI[siLin];
           
         percMappedUL /=
            miruTblSTPtr->ampAryST[siAmp].mapReadsSI;

         /*copy non-decimal part of percentage*/
         posSI +=
            numToStr(&buffStr[posSI], percMappedUL / 100);

         buffStr[posSI++] = '.';
         
         /*copy decimal part of percentage*/
         posSI +=
            numToStr(&buffStr[posSI], percMappedUL % 100);

         buffStr[posSI++] = '\t';

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun26 Sec04 Sub02 Cat03:
         +   - copy total number of reads to buffer
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         linCpTotal_fun26_sec04_sub02_cat03:;

         if(lenBuffSI - posSI < def_lenStr_miruTbl)
         { /*If: I need to print out the buffer*/
            fwrite(
               (char *) buffStr,
               sizeof(char),
               posSI,
               outFILE
            );

            posSI = 0;
         } /*If: I need to print out the buffer*/

         posSI +=
            numToStr(
               &buffStr[posSI],
               miruTblSTPtr->ampAryST[siAmp].mapReadsSI
            ); /*copy total number mapped reads*/
      } /*Loop: print number of hits per primer*/

      if(lenBuffSI - posSI < def_lenStr_miruTbl)
      { /*If: need to print out the buffer*/
            fwrite(
               (char *) buffStr,
               sizeof(char),
               posSI,
               outFILE
            );

         posSI = 0;
      } /*If: need to print out the buffer*/

      /**************************************************\
      * Fun26 Sec04 Sub03:
      *   - end lineages row with an newline
      \**************************************************/

      buffStr[posSI++] = '\n';
   } /*Loop: copy entries for each lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec05:
   ^   - copy number of missed reads
   ^   o fun26 sec05 sub01:
   ^     - copy unkown lineage id and start print loop
   ^   o fun26 sec05 sub02:
   ^     - copy number of no lineage detected reads
   ^   o fun26 sec05 sub03:
   ^     - get and copy percentage of no lineage reads
   ^   o fun26 sec05 sub04:
   ^     - copy total number of reads to buffer
   ^   o fun26 sec05 sub05:
   ^     - add new line to end of unkown lineage line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun26 Sec05 Sub01:
   *   - copy unkown lineage id and start print loop
   \*****************************************************/

   if(lenBuffSI - posSI < def_lenStr_miruTbl)
   { /*If: I need to print out the buffer*/
      fwrite(
         (char *) buffStr,
         sizeof(char),
         posSI,
         outFILE
      );

      posSI = 0;
   } /*If: I need to print out the buffer*/

   posSI +=
      cpDelim_charCp(
         &buffStr[posSI],
         (schar *) "NA",
         '\0'
      );

   for(
      siAmp = 0;
      siAmp < miruTblSTPtr->numPrimSI;
      ++siAmp
   ){ /*Loop: copy unmapped counts*/

      if(lenBuffSI - posSI < def_lenStr_miruTbl)
      { /*If: need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: need to print out the buffer*/

      /**************************************************\
      * Fun26 Sec05 Sub02:
      *   - copy number of no lineage detected reads
      \**************************************************/

      if(lenBuffSI - posSI < def_lenStr_miruTbl + 32)
      { /*If: need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: need to print out the buffer*/

      buffStr[posSI++] = '\t';

      posSI +=
         numToStr(
            &buffStr[posSI],
            miruTblSTPtr->ampAryST[siAmp].noLinSI
         ); /*copy number of hits to lineage*/

      buffStr[posSI++] = '\t';

      /**************************************************\
      * Fun26 Sec05 Sub03:
      *   - get and copy percentage of no lineage reads
      \**************************************************/

      if(lenBuffSI - posSI < def_lenStr_miruTbl + 32)
      { /*If: need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: need to print out the buffer*/

      if(! miruTblSTPtr->ampAryST[siAmp].noLinSI)
      { /*If: no reads mapped to this primer*/
         buffStr[posSI++] = '0';
         buffStr[posSI++] = '\t';
         buffStr[posSI++] = '0';

         goto noLinCpTotal_fun26_sec05_sub03;
      } /*If: no reads mapped to this primer*/

      /*allows me to keep the percentage as integer*/
      percMappedUL =
           10000
         * (miruTblSTPtr->ampAryST[siAmp].noLinSI);
        
      percMappedUL /=
         miruTblSTPtr->ampAryST[siAmp].mapReadsSI;

      /*copy non-decimal part of the percentage*/
      posSI +=
         numToStr(
            &buffStr[posSI],
            percMappedUL / 100
         );

      buffStr[posSI++] = '.';
      
      /*copy decimal part of the percentage*/
      posSI +=
         numToStr(
            &buffStr[posSI],
            percMappedUL % 100
         );

      buffStr[posSI++] = '\t';

      /**************************************************\
      * Fun26 Sec05 Sub04:
      *   - copy total number of reads to buffer
      \**************************************************/

      noLinCpTotal_fun26_sec05_sub03:;

      if(lenBuffSI - posSI < def_lenStr_miruTbl)
      { /*If: I need to print out the buffer*/
         fwrite(
            (char *) buffStr,
            sizeof(char),
            posSI,
            outFILE
         );

         posSI = 0;
      } /*If: I need to print out the buffer*/

      posSI +=
         numToStr(
            &buffStr[posSI],
            miruTblSTPtr->ampAryST[siAmp].mapReadsSI
         ); /*copy total number mapped reads*/

   } /*Loop: copy the unmapped counts*/

   /*****************************************************\
   * Fun26 Sec05 Sub05:
   *   - add new line to end of unkown lineage line
   \*****************************************************/

   if(lenBuffSI - posSI < def_lenStr_miruTbl)
   { /*If: need to print out the buffer*/
      fwrite(
         (char *) buffStr,
         sizeof(char),
         posSI,
         outFILE
      );

      posSI = 0;
   } /*If: need to print out the buffer*/

   buffStr[posSI++] = '\n';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec05:
   ^   - print final parts and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fwrite(
      (char *) buffStr,
      sizeof(char),
      posSI,
      outFILE
   );

   if(outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   sort_miruTbl(miruTblSTPtr); /*so can be re-used*/

   return 0;
} /*p_miruTbl*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun27 TOC: plineages_miruTbl
   '   - prints the "best" MIRU lineages for each primer
   '   o fun27 sec01:
   '     - variable declerations
   '   o fun27 sec02:
   '     - check if I can open the output file
   '   o fun27 sec03:
   '     - unsort table and print out the MIRU lineages
   '   o fun27 sec04:
   '     - resort the table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siPrim = 0;
   sint siLin = 0;
   sint bestLinSI = 0;

   sint *hitAryI = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec02:
   ^   - check if I can open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: user supplied an file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
         return def_fileErr_tbMiruDefs;
   } /*Else: user supplied an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec03:
   ^   - unsort table and print out the MIRU lineages
   ^   o fun27 sec03 sub01:
   ^     - unsort table/print out header; has primer names
   ^   o fun27 sec03 sub02:
   ^     - print "best" lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun27 Sec03 Sub01:
   *   - unsort table & print out header; has primer names
   \*****************************************************/

   unsort_miruTbl(miruTblSTPtr);

   fprintf(
      outFILE,
      "Primer"
   );

   for(
      siPrim = 0;
      siPrim < miruTblSTPtr->numPrimSI;
      ++siPrim
   ){ /*Loop: print primer names*/
      fprintf(
         outFILE,
         "\t%s",
          miruTblSTPtr->ampAryST[siPrim].idStr
      );
   } /*Loop: print primer names*/

   /*****************************************************\
   * Fun27 Sec03 Sub02:
   *   - print "best" lineages
   \*****************************************************/

   fprintf(
      outFILE,
      "\nLineage"
   );

   for(
      siPrim = 0;
      siPrim < miruTblSTPtr->numPrimSI;
      ++siPrim
   ){ /*Loop: print linage for each primer*/
      hitAryI = miruTblSTPtr->ampAryST[siPrim].hitsArySI;
      bestLinSI = 0;

      for(
         siLin = 0;
         siLin < miruTblSTPtr->numLinSI;
         ++siLin
      ){ /*Loop: find most supported lineage*/
         bestLinSI = 
            ifmax_genMath(
               hitAryI[siLin],
               hitAryI[bestLinSI],
               siLin,
               bestLinSI
            ); /*get best index*/
      } /*Loop: find most supported lineage*/

      if(hitAryI[bestLinSI] > 0)
      { /*If: have lineage to print out*/
         bestLinSI = strIndex_miruTbl(bestLinSI);

         fprintf(
            outFILE,
            "\t%s",
            &miruTblSTPtr->lineageStrAry[bestLinSI]
         );
      } /*If: have lineage to print out*/

      else
         fprintf(
            outFILE,
            "\tNA"
         );
   } /*Loop: print linage for each primer*/

   fprintf(
      outFILE,
      "\n"
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec04:
   ^   - resort table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sort_miruTbl(miruTblSTPtr); /*So can be re-used*/

   if(outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   return 0;
} /*plineages_miruTbl*/

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
