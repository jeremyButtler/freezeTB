/*#######################################################\
# Name: miruTblStruct
#   - 
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Holds included libraries
'   o st-01: miruAmp (.h file only)
'     - A single column in a MIRU lineages table
'   o st-02: miruTbl (.h file only)
'     - Holds a single table for miru primers
'   o fun-01: blankMiruAmp (.h file only)
'     - Sets the non-pointer values in an miruAmp
'       structure to -1 or 0
'   o fun-02: resetCnt_miruAmp (.h only)
'     - Resets the counter variables in an miruAmp
'       structure
'   o fun-03: initMiruAmp (.h file only)
'     - Initializes a miruAmp structure
'   o fun-04: makeMiruAmp
'     - Makes an initialized miruAmp structure on the heap
'   o fun-05: freeMiruAmpStack
'     - Frees all arrays in an mirAmp structure
'   o fun-06: freeMiruAmp
'     - Frees an miruAmp structure
'   o fun-07: blankMiruTbl (.h file only)
'     - Blanks all values in a miruTbl structure. This
'       includes the arrays
'   o fun-08: resetCnt_miruTbl (.h only)
'     - Resets all miruAmp counters in an miru table
'   o fun-09: initMiruTbl (.h file only)
'     - Sets all values in an miruTbl structure to
'       defaults
'   o fun-10: makeMiruTbl
'     - Makes an initialized miruTbl structure on the heap
'   o fun-11: freeMiruTblStack
'     - Frees all variables inside an miruTbl structure
'   o fun-12: freeMiruTbl
'     - Frees an miruTbl structure
'   o fun-13: getStrAryIndex (.h only)
'     - Gets the index of an string in an miruTblST
'       c-string array
'   o fun-14: sortMiruAmp
'     - Sorts the lineages in an miruAmp structure by
'       the expected length of an amplicon
'   o fun-15: swapMiruAmps
'     - Swaps two amplicons in a miruTbl structure
'   o fun-16: cmpLineages
'     - Checks to see if the first lineage should come
'       the second (alphabetically)
'   o fun-17: sortMiruTbl
'     - Sorts a miruTbl structure for quick look ups. This
'       will sort the primer names by starting positoin
'       and the rows by length, so it is not in a
'       printable format. This calls fun-14 in
'       miruTblStruct.c
'   o fun-18: getMiruAmpFromTbl
'     - Get the miruAmp structure for an input genome
'       position from an sorted (fun-15; miruTblStruct.h)
'       miruTbl
'   o fun-19: getMiruLinageFromAmp
'     - Get the first lineage that has the same (fuzzy)
'       length as the input length from an sorted
'       (fun-14; miruTblStruct.c) miruAmp structure
'   o fun-20: findAmpLen
'      - Finds the length of the MIRU amplicon for an
'        sequence in a samEntry struct
'   o fun-21: inc_matching_len_lineages
'     - Scans reads for MIRU amplicons and then incurments
'       the counter for ever lineage(s) in read/consensu
'   o fun-22: miruAmp_unsort
'     - unsorts an miruAmp structure back to the original
'       input format (printable/analysis form). 
'   o fun-23: miruTbl_unsort
'     - unsorts an miruTbl structure back to its original
'       (printable form). Calls mirAmp_unsort (fun-18;
'       miruTblStruct.c)
'   o fun-24: readMiruTblLine
'     - Reads a line from an miruTbl
'   o fun-25: readMiruTbl
'     - Reads in a ts table of MIRU primers and lineages
'       in the tsv. (primers are columns, lineages rows)
'   o fun-26: pMiruTbl
'     - Prints the MIRU table to a file. This prints
'       the number of hits per primer lineage.
'   o fun-27: pLineages
'     - Prints the "best" MIRU lineages for each primer
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
   /*TODO: Figure out plan9 file system*/
#else
   #include <stdlib.h>
#endif

#include "miruTblStruct.h"

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"

#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/numToStr.h"
#include "../generalLib/genMath.h"

/*Hidden libraries
  - has a .c files
  - No .c file (.h only)
*/

/*-------------------------------------------------------\
| Fun-04: makeMiruAmp
|   - Makes an initialized miruAmp structure on the heap
| Input:
| Output:
|   - Returns:
|     o Pointer to an miruAmp structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct miruAmp *
makeMiruAmp(
){
   struct miruAmp *retST = malloc(sizeof(struct miruAmp));

   if(retST != 0) initMiruAmp(retST);
   return retST;
} /*makeMiruAmp*/

/*-------------------------------------------------------\
| Fun-05: freeMiruAmpStack
|   - Frees the variables in an miruAmp structure
| Input:
|   - miruAmpSTPtr:
|     o Pointer to an miruAmp structure to free variables
|       in
| Output:
|   - Frees:
|     o miruAmpSTPtr variables: idStr, linLenIAry,
|       indexIAry, and numHitsIAry
|   - Modifies:
|     o All values in miruAmpSTPtr to be initialized
\-------------------------------------------------------*/
void
freeMiruAmpStack(
   struct miruAmp *miruAmpSTPtr
){ /*freeMiruAmpStack*/
   if(miruAmpSTPtr->linLenIAry)
      free(miruAmpSTPtr->linLenIAry);

   if(miruAmpSTPtr->indexIAry)
      free(miruAmpSTPtr->indexIAry);

   if(miruAmpSTPtr->numHitsIAry)
      free(miruAmpSTPtr->numHitsIAry);

   initMiruAmp(miruAmpSTPtr);
} /*freeMiruAmpStack*/

/*-------------------------------------------------------\
| Fun-06: freeMiruAmp
|   - Frees an miruAmp stucture
| Input:
|   - miruAmpSTPtrPtr:
|     o Pointer to an miruAmp structure pointer to free
| Output:
|   - Frees:
|     o miruAmpSTPtrPtr
|   - Modifies:
|     o miruAmpSTPtrPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeMiruAmp(
   struct miruAmp **miruAmpSTPtrPtr
){ /*freeMiruAmp*/
   if(*miruAmpSTPtrPtr)
      freeMiruAmpStack(*miruAmpSTPtrPtr);

   free(*miruAmpSTPtrPtr);
   *miruAmpSTPtrPtr = 0;
} /*freeMiruAmp*/

/*-------------------------------------------------------\
| Fun-10: makeMiruTbl
|   - Makes and initializes a miruTbl structure on the
|     heap
| Input:
| Output:
|   - Returns:
|     o An initialized miruTbl structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct miruTbl *
makeMiruTbl(
){ /*makeMiruTbl*/
   struct miruTbl *retST = malloc(sizeof(struct miruTbl));

   if(retST) initMiruTbl(retST);

   return retST;
} /*makeMiruTbl*/

/*-------------------------------------------------------\
| Fun-11: freeMiruTblStack
|   - Frees the arrays inside a miruTbl structure
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to free variables
|       in
| Output:
|   - Frees:
|     o miruTblSTPtr variabls: miruAmpSTAry,
|       lingeageIndexIAry, lineageIndexStrAry
|   - Modifies:
|     o Runs initMiruTbl() on all variables
\-------------------------------------------------------*/
void
freeMiruTblStack(
   struct miruTbl *miruTblSTPtr
){ /*freeMiruTblStack*/
   int iAmp = 0;

   if(miruTblSTPtr->lineageStrAry)
      free(miruTblSTPtr->lineageStrAry);

   for(iAmp = 0; iAmp < miruTblSTPtr->numPrimI; ++iAmp)
      freeMiruAmpStack(&miruTblSTPtr->miruAmpSTAry[iAmp]);

   initMiruTbl(miruTblSTPtr);
} /*freeMiruTblStack*/

/*-------------------------------------------------------\
| Fun-12: freeMiruTbl
|   - Frees a miruTbl structure
| Input:
|   - miruTblSTPtrPtr:
|     o Pointer to a pointer to a miruTbl structure to
|       free
| Output:
|   - Frees:
|     o The miruTbl structure in miruTblSTPtrPtr
|   - Modifies:
|     o miruTblSTPtrPtr to point to 0 (null)
\-------------------------------------------------------*/
void
freeMiruTbl(
   struct miruTbl **miruTblSTPtrPtr
){ /*freeMiruTbl*/
   freeMiruTblStack(*miruTblSTPtrPtr);

   free(*miruTblSTPtrPtr);

   *miruTblSTPtrPtr = 0;
} /*freeMiruTbl*/

/*-------------------------------------------------------\
| Fun-14: sortMiruAmp
|   - Sorts the lineages in a miruAmp structure be the
|     expected amplicon length
| Input:
|   - miruAmpSTPtr: 
|     o A pointer to an miruAmpSTPtr structure to sort
| Output:
|   - Modifies:
|     o miruAmpSTPtr variables: linLenIAry,
|       numHitsIAry, and indexIAry to be sorted by values
|       in linLenIAry (least to greatest)
\-------------------------------------------------------*/
void
sortMiruAmp(
   struct miruAmp *miruAmpSTPtr /*Has lineages to sort*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-14 TOC: sortMiruAmp
   '   - Sorts the lineages in a miruAmp structure by the
   '     expected amplicon length (least to greatest)
   '   o fun-14 sec-01:
   '     - Variable declerations
   '   o fun-14 sec-02:
   '     - Find the first subset (largest gap)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Some quick pointers to save my sanity*/
   int *ampLenIAry = miruAmpSTPtr->linLenIAry;
   int *hitsIAry = miruAmpSTPtr->numHitsIAry;
   int *indexIAry = miruAmpSTPtr->indexIAry;
   int numLineagesI = miruAmpSTPtr->numLinI;

   int swapI = 0;  /*For swaping elements*/

   int distI = 0; /*distance between elements in subset*/
   int nextElmI = 0; /*Next element in subset*/
   int lastElmI = 0; /*Previous element in subset*/
   int elmOnI = 0;   /*Element currently sorting*/

   int iSubset = 0;  /*Subset of index's on in round*/
   int iElm = 0;     /*index on*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-02:
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

   distI = 1;

   while(distI < numLineagesI - 1)
   { /*Loop: Find the first subset I am using*/
      distI *= 3; 
      ++distI;
   } /*Loop: Find the first subset I am using*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-03:
   ^   - Sort my arrays
   ^   o fun-14 sec-03 sub-01:
   ^     - Set up the shell short triple loop
   ^   o fun-14 sec-03 sub-02:
   ^     - Check if I need to swap elements in a
   ^       subset
   ^   o fun-14 sec-03 sub-03:
   ^     - Check to see if I need to swap
   ^       previous elements in a subset
   ^   o fun-14 sec-03 sub-04:
   ^     - Move to the next distance/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-14 Sec-03 Sub-01:
   *   - Set up the shell short triple loop
   \*****************************************************/

   while(distI > 0)
   { /*Loop: sort till distance between subsets is 0*/

      for(iSubset = 0; iSubset < distI; ++iSubset)
      { /*Loop: Go through each subset in distI*/

         for(
            iElm = 0;
            iElm + distI < numLineagesI;
            iElm += distI
         ){ /*Loop: Sort all elements in each subset*/

            /********************************************\
            * Fun-14 Sec-03 Sub-02:
            *   - Check if I need to swap elements in a
            *     subset
            \********************************************/

            /*Get the next index in the subset*/
            nextElmI = iElm + distI;

            if(ampLenIAry[iElm] > ampLenIAry[nextElmI])
            { /*If: I need to swap elements in a subset*/
               swapI = ampLenIAry[iElm];
               ampLenIAry[iElm] = ampLenIAry[nextElmI];
               ampLenIAry[nextElmI] = swapI;

               swapI = hitsIAry[iElm];
               hitsIAry[iElm] = hitsIAry[nextElmI];
               hitsIAry[nextElmI] = swapI;

               swapI = indexIAry[iElm];
               indexIAry[iElm] = indexIAry[nextElmI];
               indexIAry[nextElmI] = swapI;

               /*****************************************\
               * Fun-14 Sec-03 Sub-03:
               *   - Check to see if I need to swap
               *     previous elements in a subset
               \*****************************************/

               lastElmI = iElm;
               elmOnI = iElm;

               while(lastElmI >= distI)
               { /*Loop: Check the previous elements*/
                  lastElmI -= distI;

                  if(   ampLenIAry[lastElmI]
                      < ampLenIAry[elmOnI]
                  ) break;/*Finshed; element in position*/

                  swapI = ampLenIAry[elmOnI];
                  ampLenIAry[elmOnI]=ampLenIAry[lastElmI];
                  ampLenIAry[lastElmI] = swapI;

                  swapI = hitsIAry[elmOnI];
                  hitsIAry[elmOnI] = hitsIAry[lastElmI];
                  hitsIAry[lastElmI] = swapI;

                  swapI = indexIAry[elmOnI];
                  indexIAry[elmOnI] = indexIAry[lastElmI];
                  indexIAry[lastElmI] = swapI;

                  elmOnI = lastElmI;
               } /*Loop: Check the previous elements*/
            } /*If: I need to swap elements in a subset*/
         } /*Loop: Sort all elements in each subset*/
      } /*Loop: Go through each subset in distI*/

      /**************************************************\
      * Fun-14 Sec-03 Sub-04:
      *   - Move to the next distance/round
      \**************************************************/

       --distI;
       distI /= 3;
   } /*Loop: sort till distance between subsets is 0*/
} /*sortMiruAmp*/

/*-------------------------------------------------------\
| Fun-15: swapMiruAmps
|   - Swaps two miruAmp structures in an miruTbl structure
| Input:
|   - firstIndexI:
|     o Integer with index of first lineage to swap
|   - secLineageStr
|     o Integer with index of the second lineage to swap
|   - mirTblSTPtr:
|     o Pointer to a miruTbl structure with lineages to
|       swap
| Output:
|   - Modifies:
|     o lineageStrAry to have the two indexs swaped
|     o lineageIndexIAry to have the original indexs for
|       the two lineages swapped
\-------------------------------------------------------*/
#define \
swapMiruAmps(\
   firstIndexI,\
   secIndexI,\
   miruTblSTPtr\
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-15 TOC: swapMiruAmps
   '   - Swaps two lineages around in a miruTbl structure
   '   o fun-15 sec-01:
   '     - Variable declerations
   '   o fun-15 sec-02:
   '     - Swap the primer coordinates
   '   o fun-15 sec-03:
   '     - Swap the index for each lineages original
   '       position in the table around
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-15 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   char swapMacC = 0;\
   char *firstMacStr = 0;\
   char *secMacStr = 0;\
   \
   int swapMacI = 0; /*For swapping indexs*/\
   int *ptrSwapMacI = 0; /*for swapping interger pionts*/\
   \
   struct miruAmp *firstAmpST =\
      &(miruTblSTPtr)->miruAmpSTAry[firstIndexI];\
   \
   struct miruAmp *secAmpST =\
      &(miruTblSTPtr)->miruAmpSTAry[secIndexI];\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-15 Sec-02:
   ^   - Swap the primer coordiantes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   swapMacI = firstAmpST->forPosI;\
   firstAmpST->forPosI = secAmpST->forPosI;\
   secAmpST->forPosI = swapMacI;\
   \
   swapMacI = firstAmpST->forLenI;\
   firstAmpST->forLenI = secAmpST->forLenI;\
   secAmpST->forLenI = swapMacI;\
   \
   swapMacI = firstAmpST->revPosI;\
   firstAmpST->revPosI = secAmpST->revPosI;\
   secAmpST->revPosI = swapMacI;\
   \
   swapMacI = firstAmpST->revLenI;\
   firstAmpST->revLenI = secAmpST->revLenI;\
   secAmpST->revLenI = swapMacI;\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-15 Sec-03:
   ^   - swap the arrays/values associated with lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   ptrSwapMacI = firstAmpST->linLenIAry;\
   firstAmpST->linLenIAry = secAmpST->linLenIAry;\
   secAmpST->linLenIAry = ptrSwapMacI;\
   \
   ptrSwapMacI = firstAmpST->numHitsIAry;\
   firstAmpST->numHitsIAry = secAmpST->numHitsIAry;\
   secAmpST->numHitsIAry = ptrSwapMacI;\
   \
   ptrSwapMacI = firstAmpST->indexIAry;\
   firstAmpST->indexIAry = secAmpST->indexIAry;\
   secAmpST->indexIAry = ptrSwapMacI;\
   \
   swapMacI = firstAmpST->numLinI;\
   firstAmpST->numLinI = secAmpST->numLinI;\
   secAmpST->numLinI = swapMacI;\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-15 Sec-04:
   ^   - swap the primer name
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   firstMacStr = firstAmpST->idStr;\
   secMacStr = secAmpST->idStr;\
   \
   while(*firstMacStr > 31 && *secMacStr > 31)\
   { /*Loop: Swap the primer names*/\
      swapMacC = *firstMacStr;\
      *firstMacStr++ = *secMacStr;\
      *secMacStr++ = swapMacC;\
   } /*Loop: Swap the primer names*/\
   \
   *firstMacStr = '\0';\
   *secMacStr = '\0';\
} /*swapMiruAmps*/

/*-------------------------------------------------------\
| Fun-16: cmpLineages
|   - Checks to see if the first lineage should come
|     the second (alphabetically)
| Input:
|   - firstIndexI:
|     o Integer with index of first lineage to swap
|   - secLineageStr
|     o Integer with index of the second lineage to swap
|   - mirTblSTPtr:
|     o Pointer to a miruTbl structure with lineages to
|       swap
| Output:
|   - Returns:
|     o < 0 if the first linage is less than the second
|     o 0 if the first lineage matches the second
|     o > 0 if th e first lineage is greater than second
\-------------------------------------------------------*/
#define \
cmpLineages(\
   firstIndexI,\
   secIndexI,\
   miruTblSTPtr\
)(\
    (miruTblSTPtr)->miruAmpSTAry[(firstIndexI)].forPosI\
  - (miruTblSTPtr)->miruAmpSTAry[(secIndexI)].forPosI\
) /*cmpLineages*/

/*-------------------------------------------------------\
| Fun-17: sortMiruTbl
|   - Sorts the lineages in a miruTbl structure be the
|     expected amplicon length
| Input:
|   - miruTblSTPtr: 
|     o A pointer to an miruTblSTPtr structure to sort
| Output:
|   - Modifies:
|     o miruTblSTPtr variables: linLenIAry,
|       numHitsIAry, and indexIAry to be sorted by values
|       in linLenIAry (least to greatest)
|   - Returns:
|     o 0 for no errors
|     o 1 for an array out of bounds error
\-------------------------------------------------------*/
char
sortMiruTbl(
   struct miruTbl *miruTblSTPtr /*Has lineages to sort*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-17 TOC: sortMiruTbl
   '   - Sorts the lineages in a miruTbl structure by the
   '     lineage name
   '   o fun-17 sec-01:
   '     - Variable declerations
   '   o fun-17 sec-02:
   '     - Find the first subset (largest gap)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Some quick pointers to save my sanity*/
   int numPrimI = miruTblSTPtr->numPrimI; 

   uchar lenSwapC = 0;/*Number of characters swaped*/

   int distI = 0; /*distance between elements in subset*/
   int nextElmI = 0; /*Next element in subset*/
   int lastElmI = 0; /*Previous element in subset*/
   int elmOnI = 0;   /*Element currently sorting*/

   int iSubset = 0;  /*Subset of index's on in round*/
   int iElm = 0;     /*index on*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-02:
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

   distI = 1;

   while(distI < numPrimI - 1)
   { /*Loop: Find the first subset I am using*/
      distI *= 3; 
      ++distI;
   } /*Loop: Find the first subset I am using*/

   iElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-03:
   ^   - Sort my arrays
   ^   o fun-17 sec-03 sub-01:
   ^     - Set up the shell short triple loop
   ^   o fun-17 sec-03 sub-02:
   ^     - Check if I need to swap elements in a
   ^       subset
   ^   o fun-17 sec-03 sub-03:
   ^     - Check to see if I need to swap
   ^       previous elements in a subset
   ^   o fun-17 sec-03 sub-04:
   ^     - Move to the next distance/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-17 Sec-03 Sub-01:
   *   - Set up the shell short triple loop
   \*****************************************************/

   while(distI > 0)
   { /*Loop: sort till distance between subsets is 0*/

      for(iSubset = 0; iSubset < distI; ++iSubset)
      { /*Loop: Go through each subset in distI*/

         for(
            iElm = 0;
            iElm + distI < numPrimI;
            iElm += distI
         ){ /*Loop: Sort all elements in each subset*/

            /********************************************\
            * Fun-17 Sec-03 Sub-02:
            *   - Check if I need to swap elements in a
            *     subset
            \********************************************/

            /*Get the next index in the subset*/
            nextElmI = iElm + distI;

            if(cmpLineages(iElm,nextElmI,miruTblSTPtr) >0)
            { /*If: I need to swap elements in a subset*/

               swapMiruAmps(iElm,nextElmI,miruTblSTPtr);

               /*****************************************\
               * Fun-17 Sec-03 Sub-03:
               *   - Check to see if I need to swap
               *     previous elements in a subset
               \*****************************************/

               lastElmI = iElm;
               elmOnI = iElm;

               while(lastElmI >= distI)
               { /*Loop: Check the previous elements*/
                  lastElmI -= distI;

                   if(
                      cmpLineages(
                         elmOnI,
                         lastElmI,
                         miruTblSTPtr
                      ) > 0
                    ) break;

                    swapMiruAmps(
                       elmOnI,
                       lastElmI,
                       miruTblSTPtr
                    ); /*Move the amplicons around*/

                  if(lenSwapC >= def_lenStr_miruTblST - 1)
                     return 1; /*array out of bound*/

                  elmOnI = lastElmI;
               } /*Loop: Check the previous elements*/
            } /*If: I need to swap elements in a subset*/
         } /*Loop: Sort all elements in each subset*/
      } /*Loop: Go through each subset in distI*/

      /**************************************************\
      * Fun-17 Sec-03 Sub-04:
      *   - Move to the next distance/round
      \**************************************************/

       --distI;
       distI /= 3;
   } /*Loop: sort till distance between subsets is 0*/

   for(iElm = 0; iElm < numPrimI; ++iElm)
      sortMiruAmp(&miruTblSTPtr->miruAmpSTAry[iElm]);

   return 0;
} /*sortMiruTbl*/

/*-------------------------------------------------------\
| Fun-18: get_miruAmpIndex_fromMiruTbl
|   - Get an miruAmp structure from an miruTbl structure
|     for the input lineage
| Input:
|   - ampStartI:
|     o Integer with the first base in the amplicon that
|       I am getting the MIRU table entry for
|   - ampEndI:
|     o Integer with the last base in the amplicon that I
|       am getting the MIRU table entry for
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure with the MIRU
|       table to search
| Output:
|   - Returns:
|     o An index (integer) of the matching lineage
|     o -1 if there were no MIRU amplicons in the input
|       range (ampStartI to ampEndI)
\-------------------------------------------------------*/
int
get_miruAmpIndex_FromMiruTbl(
   int ampStartI, /*First base in amplicon to look for*/
   int ampEndI,   /*Last base in amplicon to look for*/
   struct miruTbl *miruTblSTPtr /*Has table to look in*/
){
   int midI = 0;   /*Midpoint/found table*/
   int rightI = 0; /*Right (upper) half of search range*/
   int leftI = 0;  /*left (lower) half of search range*/
   struct miruAmp *tmpST = miruTblSTPtr->miruAmpSTAry;

   rightI = miruTblSTPtr->numPrimI - 1;

   while(leftI <= rightI)
   { /*Loop: Find the target index*/
      midI = (leftI + rightI) >> 1;

      if(tmpST[midI].forPosI > ampEndI) rightI = midI - 1;

      else if(tmpST[midI].revPosI < ampStartI)
         leftI = midI + 1;

      else break;
   } /*Loop: Find the target index*/

   if(tmpST[midI].forPosI > ampEndI) return -1;
   if(tmpST[midI].revPosI < ampStartI) return -1;

   while(midI > 0 && tmpST[midI - 1].forPosI >= ampStartI)
      --midI;

   return midI;
} /*get_miruAmpIndex_fromMiruTbl*/

/*-------------------------------------------------------\
| Fun-19: get_linCountIndex_fromMiruTbl
|   - Finds the counter entry for a MIRU linage of the
|     input length. This is a bit fuzzy, in that you can
|     provide a fudge factor.
| Input:
|   - lenAmpI:
|     o Integer with the length of the amplicon to compare
|   - fudgeI:
|     o How much to ofset the length by when comparing to
|       the lengths in the MIRU table (converts lenAmpI
|       into a range)
|   - ampIndexI:
|     o Integer with index of amplicon to search in the
|       MIRU Table
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
| Output:
|   - Returns:
|     o An index (integer) to the first matching length
|       in mirAmpSTPtr. There may be dupicate lengths
|     o -1 if there were no MIRU lineages with the target
|       length for this amplicon
\-------------------------------------------------------*/
int
get_linCountIndex_fromMiruAmp(
   int lenAmpI, /*Length of amplicon*/
   int fudgeI,  /*How much to offset length by*/
   int ampIndexI, /*Index of amplicon to search*/
   struct miruTbl *miruTblSTPtr /*Has a miruTbl column*/
){
   int midI = 0;   /*Midpoint/found table*/
   int rightI = 0; /*Right (upper) half of search range*/
   int leftI = 0;  /*left (lower) half of search range*/

   int ampStartI = lenAmpI - fudgeI;
   int ampEndI = lenAmpI + fudgeI;

   int *ampIAry =
     miruTblSTPtr->miruAmpSTAry[ampIndexI].linLenIAry;

   rightI = miruTblSTPtr->numLineagesI - 1;

   while(leftI <= rightI)
   { /*Loop: Find the target index*/
      midI = (leftI + rightI) >> 1;

      if(ampIAry[midI] > ampEndI) rightI = midI - 1;

      else if(ampIAry[midI] < ampStartI) leftI = midI + 1;

      else
      { /*Else: I found the amplicon length*/
         /*I need to find the first match that was in this
         `   length range. There may be multiple matches
         */
         while(midI > 0 && ampIAry[midI - 1] >= ampStartI)
            --midI;

         return midI; /*Found my index*/
     } /*Else: I found the amplicon length*/
   } /*Loop: Find the target index*/

   if(ampIAry[midI] > ampEndI) return -1;
   if(ampIAry[midI] < ampStartI) return -1;

   /*I need to find the first match that was in this
   `   length range. There may be multiple matches
   */
   while(midI > 0 && ampIAry[midI - 1] >= ampStartI)
      --midI;

   return midI;
} /*get_linCountIndex_fromMiruAmp*/

/*-------------------------------------------------------\
| Fun-20: findAmpLen
|   - Finds the length of the MIRU amplicon for a sequence
|     in a samEntry struct
| Input:
|   - startAmpI:
|     o Integer with starting position of MIRU amplicon
|   - endAmpI:
|     o Integer with ending position of MIRU amplicon
|   - refPosIPtr:
|     o Pointer to an integer with the reference base that
|       I am on
|   - cigPosIPtr:
|     o Pionter to an integer with the number of bases
|       left in the current cigar entry
|   - cigIndexUIPtr:
|     o Pointer to an unsigned integer with the cigar
|       index I am currently on
|   - samSTPtr:
|     o Pointer to a samEntry structure with a sequence to
|       find the  amplicon length for
| Output:
|   - Returns:
|     o Integer with the read length
|     o -1 if the range (startAmpI to endAmpI) was longer
|       than the reads (samSTPtr) reference range.
\-------------------------------------------------------*/
int
findAmpLen(
   int startAmpI, /*Starting position of MIRU amplicon*/
   int endAmpI,   /*ending position of MIRU ampllicon*/
   int *refPosIPtr, /*Position on in reference*/
   int *cigPosIPtr, /*Num of bases left in cigar entry*/
   unsigned int *cigIndexUIPtr, /*index on in cigar*/
   void *samSTPtr   /*sequence I am finding length of*/
){ 
   sint seqPosSI = 0; /*Temporary variable*/
   sint seqStartSI = 0; /*Temporary variable*/
   struct samEntry *samST = (struct samEntry *) samSTPtr;

   /*Find the start of the amplicon*/

   samEntryFindRefPos(
      samST,
      *cigIndexUIPtr,
      *cigPosIPtr,
      startAmpI,
      *refPosIPtr,
      seqPosSI
   );

   /*If: there is an incomplete mapping*/
   if(*refPosIPtr != startAmpI)
      return -1;

   seqStartSI = seqPosSI;

   /*Find the length of an amplicon/read*/

   samEntryFindRefPos(
      samST,
      *cigIndexUIPtr,
      *cigPosIPtr,
      endAmpI,
      *refPosIPtr,
      seqPosSI
   );

   if(*refPosIPtr != endAmpI)
      return -1;

   return seqPosSI - seqStartSI;
} /*findAmpLen*/

/*-------------------------------------------------------\
| Fun-21: inc_matching_len_lineages
|   - Scans reads for MIRU amplicons and then incurments
|     the counter for ever lineage(s) in read/consensus
| Input:
|   - samSTPtr:
|     o Pointer to an samEntry structure with the amplicon
|       coordinates.
|   - fudgeI:
|     o How much to ofset the length by when comparing to
|       the lengths in the MIRU table (converts lenAmpI
|       into a range)
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
| Output:
|   - Returns:
|     o 0 for could incurment
|     o 1 if the sam entry is not an lineage
\-------------------------------------------------------*/
char
inc_matching_len_lineages(
   void *samSTPtr, /*Sam entry to look at*/
   int fudgeI,     /*Range for amplicons lengths*/
   struct miruTbl *miruTblSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-21 TOC: inc_matching_len_lineages
   '   - Scans reads for MIRU amplicons and then
   '     incurments the counter for ever lineage(s) in the
   '     read/consensus
   '   o fun-01 sec-01:
   '     - Variable declerations
   '   o fun-01 sec-02:
   '     - Find the first MIRU amplicon in read/consensus
   '   o fun-01 sec-03:
   '     - Find mapped lineages, update hits for mapped,
   '       lineages, and see if any more lineages in read
   '   o fun-01 sec-04:
   '     - Let user know lineage(s) were found
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-21 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int ampIndexI = 0;
   int linIndexI = 0;
   int *hitIAry = 0;
   int *lenIAry = 0;
   int numLinI = miruTblSTPtr->numLineagesI;

   /*Start and end of the amplicon lengths*/
   int startAmpI = 0;
   int endAmpI = 0;

   /*Stats on the reads*/
   int refPosI = 0;
   int readLenI = 0;

   /*Sam entry variables*/
   uint cigIndexUI = 0;
   int cigPosOnI = 0;
   struct samEntry *samST = (struct samEntry *) samSTPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-21 Sec-02:
   ^   - Find the first MIRU amplicon in read/consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refPosI = (int) samST->refStartUI;
   
   ampIndexI =
      get_miruAmpIndex_FromMiruTbl(
         refPosI,
         (int) samST->refEndUI,
         miruTblSTPtr
      );

   if(ampIndexI < 0) return 1; /*lineage not in table*/

   cigPosOnI = samST->cigValAryI[0];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-21 Sec-03:
   ^   - Find mapped lineages, update hits for mapped,
   ^     lineages, and see if any more lineages in read
   ^   o fun-21 sec-03 sub-01:
   ^      - Find the length of the amplicon in the read
   ^   o fun-21 sec-03 sub-02:
   ^     - Find lineage index and incrument the hits for
   ^       all lineages having the reads amplicon length
   ^   o fun-21 sec-03 sub-03:
   ^     - See if there are more MIRU amplicons in read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-21 Sec-03 Sub-01:
   *   - Find the length of the amplicon in the read
   \*****************************************************/

   endAmpI= miruTblSTPtr->miruAmpSTAry[ampIndexI].revPosI;
   /*--endAmpI;*/ /*Get off the last reverse primer base*/

   while(endAmpI <= samST->refEndUI)
   { /*Loop: Update table for each MIRU amp in read*/
      startAmpI =
         miruTblSTPtr->miruAmpSTAry[ampIndexI].forPosI;

      readLenI = 
         findAmpLen(
            startAmpI, /*Starting position of MIRU amp*/
            endAmpI,   /*ending position of MIRU am*/
            &refPosI, /*Position on in reference*/
            &cigPosOnI,/*Position in current cigar entry*/
            &cigIndexUI, /*index on in cigar*/
            samSTPtr  /*sequence I am finding length of*/
         ); /*Find the MIRU amplicons length in the read*/

      /**************************************************\
      * Fun-21 Sec-03 Sub-02:
      *   - Find lineage index and incrument the hits for
      *     all lineages having the reads amplicon length
      \**************************************************/

      if(readLenI > -1)
      { /*If: I have the read length, find lineage*/
         linIndexI =
            get_linCountIndex_fromMiruAmp(
               readLenI, /*Length of amplicon*/
               fudgeI,   /*How much to offset length by*/
               ampIndexI, /*Index of amplicon to search*/
               miruTblSTPtr /*Has a miruTbl column*/
            ); /*Get the index of the first length*/
      } /*If: I have the read length, find lineage*/

      else
         goto nextAmp_fun21_sec03_sub03;

      ++(
         miruTblSTPtr->miruAmpSTAry[
            ampIndexI
         ].mapReadsI
      ); /*Incurment the read counter*/
 
      if(linIndexI >= 0)
      { /*If: I have lineages to incurment*/
         lenIAry =
            miruTblSTPtr->miruAmpSTAry[
               ampIndexI
            ].linLenIAry; /*Get the length array*/

         hitIAry =
            miruTblSTPtr->miruAmpSTAry[
               ampIndexI
            ].numHitsIAry; /*Get the hit counts array*/

         while(
               lenIAry[linIndexI] - fudgeI <= readLenI
            && lenIAry[linIndexI] + fudgeI >= readLenI
         ){ /*Loop: Incurment each lineage hit*/
            ++hitIAry[linIndexI];
            ++linIndexI;

            /*check if I hit array out of bounds*/
            if(linIndexI >= numLinI) break;
         } /*Loop: Incurment each lineage hit*/
      } /*If: I have lineages to incurment*/

      else
      { /*Else: I could not find the lineage*/
        ++(miruTblSTPtr->miruAmpSTAry[ampIndexI].noLinSI);
      } /*Else: I could not find the lineage*/

      /**************************************************\
      * Fun-21 Sec-03 Sub-03:
      *   - See if there are more MIRU amplicons in read
      \**************************************************/

      nextAmp_fun21_sec03_sub03:;

      ampIndexI++;

      endAmpI =
         miruTblSTPtr->miruAmpSTAry[ampIndexI].revPosI;

      if(ampIndexI >= miruTblSTPtr->numPrimI) break;
   } /*Loop: Update table for each MIRU amp in read*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-21 Sec-04:
   ^   - Let user know lineage(s) were found
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;
} /*inc_matching_len_lineages*/

/*-------------------------------------------------------\
| Fun-22: miruAmp_unsort
|   - Unsorts a miruAmp structure to its original input
|     order
| Input:
|   - miruAmpSTPtr:
|     o Pointer to an miruAmp structure to unsort
|   - numLinI:
|     o Integer with number of lineages in miruAmpSTPtr
| Output:
|   - Modifies:
|     o miruAmpSTPtr to be unsorted
\-------------------------------------------------------*/
void
miruAmp_unsort(
   struct miruAmp *miruAmpSTPtr /*To be unsorted*/
){
   /*These are so I have an easier work flow*/
   int *linIAry = miruAmpSTPtr->linLenIAry;
   int *hitsIAry = miruAmpSTPtr->numHitsIAry;
   int *indexIAry = miruAmpSTPtr->indexIAry;

   /*For swaping array items around*/
   int swapLinI = 0;
   int swapHitsI = 0;
   int swapIndexI = 0;
   
   int iIndex = 0; /*lineage on*/

   while(iIndex < (miruAmpSTPtr)->numLinI)
   { /*Loop: Unsort the lineage array*/
      if(iIndex != indexIAry[iIndex])
      { /*If: I need to swap entries around*/
         swapLinI = linIAry[iIndex];
         swapHitsI = hitsIAry[iIndex];
         swapIndexI = indexIAry[iIndex];

         linIAry[iIndex] = linIAry[swapIndexI];
         linIAry[swapIndexI] = swapLinI;

         hitsIAry[iIndex] = hitsIAry[swapIndexI];
         hitsIAry[swapIndexI] = swapHitsI;

         indexIAry[iIndex] = indexIAry[swapIndexI];
         indexIAry[swapIndexI] = swapIndexI; 
      } /*If: I need to swap entries around*/

      else ++iIndex;
   } /*Loop: Unsort the lineage array*/
} /*miruAmp_unsort*/

/*-------------------------------------------------------\
| Fun-23: miruTbl_unsort
|   - Unsorts a miruTbl structure to its original input
|     order
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to unsort
| Output:
|   - Modifies:
|     o miruTblSTPtr to be unsorted
\-------------------------------------------------------*/
void
miruTbl_unsort(
   struct miruTbl *miruTblSTPtr /*To be unsorted*/
){
   int numLinI = miruTblSTPtr->numPrimI;
   int *linIAry = miruTblSTPtr->ampIndexIAry;

   /*These are so I have an easier work flow*/
   struct miruAmp *ampSTAry = miruTblSTPtr->miruAmpSTAry;

   int iLin = 0; /*lineage on*/
   int swapIndexI = 0;

   while(iLin < numLinI)
   { /*Loop: Unsort the lineage array*/
      if(iLin != linIAry[iLin])
      { /*If: I need to swap entries around*/
         swapIndexI = linIAry[iLin];
         swapMiruAmps(iLin, swapIndexI, miruTblSTPtr);
      } /*If: I need to swap entries around*/

      else ++iLin;
   } /*Loop: Unsort the lineage array*/

   /*now I need to unsort the amplicons*/
   for(iLin = 0; iLin < numLinI; ++iLin)
      miruAmp_unsort(&ampSTAry[iLin]);
} /*miruTbl_unsort*/

/*-------------------------------------------------------\
| Fun-24: readMiruTblLine
|   - Reads a line from an miruTbl
| Input:
|   - buffStrPtr:
|     o Pointer to a c-string buffer to add the line to
|   - posIPtr:
|     o Pionter to an integer with the position at in
|       the buffer
|   - lenBuffIPtr:
|     o Pointer to an integer with the length of the
|       buffer
|   - bytesReadInIPtr:
|     o Pointer to integer to hold the number of
|       characters currently in the buffer
|   - tblFILE:
|     o Pointer to FILE to get the next line from
| Output:
|   - Modifies:
|     o posIPttr to be 0
|     o buffStr to hold at least the next line
|     o bytesReadInIPtr to hold number of characters in
|       the buffer (index 1)
|   - Returns:
|     o 0 for no errors
|     o def_EOF_miruTblST if at end of file
|     o def_memErr_miruTblST for memory errors
\-------------------------------------------------------*/
unsigned long
readMiruTblLine(
   char **buffStrPtr,
   int *posIPtr,
   int *lenBuffIPtr,
   int *bytesReadInIPtr,
   void *tblFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-24 TOC: readMiruTblLine
   '   - Reads a line from an miruTbl
   '   o fun-24 sec-01:
   '     - Variable declerations
   '   o fun-24 sec-02:
   '     - Check if I have an open buffer
   '   o fun-24 sec-03:
   '     - Read in the next line
   '   o fun-24 sec-04:
   '     - Finish setting variables and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-24 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char errMacC = 0;
   char *tmpMacStr = *buffStrPtr;
   int spareBuffMacI = 0;

   ulong totalBytesMacUL = 0;
   ulong charReadMacUL = 0;
   
   ulong *ulMacStr = 0;
   int byteOnMacI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-24 Sec-02:
   ^   - Check if I have an open buffer
   ^   o fun-24 sec-02 sub-01:
   ^      - Check if have data in the buffer already
   ^   o fun-24 sec-02 sub-02:
   ^     - See if there is a new line already in buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-24 Sec-02 Sub-01:
   *   - Check if have data in the buffer already
   \*****************************************************/

   if(*(posIPtr))
   { /*If: there is extra space in the buffer*/
      spareBuffMacI = *(posIPtr);
      totalBytesMacUL = *(lenBuffIPtr) - spareBuffMacI;

      ulCpStr(
         *(buffStrPtr),
         tmpMacStr,
         totalBytesMacUL
      ); /*Copy the unused part of the buffer over*/

      *(posIPtr) = 0;
      tmpMacStr = *(buffStrPtr) + totalBytesMacUL;
   } /*If: there is extra space in the buffer*/

   else spareBuffMacI = *(lenBuffIPtr);

   /*****************************************************\
   * Fun-24 Sec-02 Sub-02:
   *   - See if there is a new line already in the buffer
   \*****************************************************/

    ulMacStr = (ulong *) tmpMacStr;

    for(
       byteOnMacI = 0;
       byteOnMacI < (totalBytesMacUL >> defShiftByUL);
       ++byteOnMacI
    ){ /*Loop: See if I have a new line*/
       if(ulIfDelim(*ulMacStr, def_samEntry_newLine))
          break;

       ++ulMacStr;
    } /*Loop: See if I have a new line*/

   tmpMacStr = (char *) ulMacStr;
   byteOnMacI = 0;

   while(byteOnMacI < (totalBytesMacUL & defModByUL))
   { /*Loop: Check the last few bytes*/
      if(*tmpMacStr == '\n') break;

      ++tmpMacStr;
      ++byteOnMacI;
   } /*Loop: Check the last few bytes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-24 Sec-03:
   ^   - Read in the next line
   ^   o fun-24 sec-03 sub-01:
   ^     - Start loop and get input from file
   ^   o fun-24 sec-03 sub-02:
   ^     - See if there is a new line in the buffer
   ^   o fun-24 sec-03 sub-03:
   ^     - Resize the buffer to get more input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-24 Sec-03 Sub-01:
   *   - Start loop and get input from file
   \*****************************************************/

   while(*tmpMacStr != '\n')
   { /*Loop: Read in the first line*/
      charReadMacUL =
         fread(
            tmpMacStr,
            sizeof(char),
            spareBuffMacI - 1,
            (FILE *) (tblFILE)
         ); /*Read in the next chunk in the file*/

       totalBytesMacUL += charReadMacUL;

       if(charReadMacUL < spareBuffMacI - 1)
       { /*If: I could not read any lines from the file*/
          errMacC = def_EOF_miruTblST;
          break;
       } /*If: I could not read any lines from the file*/

      /**************************************************\
      * Fun-24 Sec-03 Sub-02:
      *   - See if there is a new line in the buffer
      \**************************************************/

       ulMacStr = (ulong *) tmpMacStr;

       for(
          byteOnMacI = 0;
          byteOnMacI < (charReadMacUL >> defShiftByUL);
          ++byteOnMacI
       ){ /*Loop: See if I have a new line*/
          if(ulIfDelim(*ulMacStr, def_samEntry_newLine))
             break;

          ++ulMacStr;
       } /*Loop: See if I have a new line*/

      tmpMacStr = (char *) ulMacStr;
      byteOnMacI = 0;

      while(byteOnMacI < (charReadMacUL & defModByUL))
      { /*Loop: Check the last few bytes*/
         if(*tmpMacStr == '\n') break;

         ++tmpMacStr;
         ++byteOnMacI;
      } /*Loop: Check the last few bytes*/

      if(*tmpMacStr == '\n') break;

      /**************************************************\
      * Fun-24 Sec-03 Sub-03:
      *   - Resize the buffer to get more input
      \**************************************************/

      *(lenBuffIPtr) <<= 1;

      tmpMacStr =
         realloc(
            *(buffStrPtr),
            (*(lenBuffIPtr) + 1) * sizeof(char)
         ); /*Get a larger buffer*/

      if(! tmpMacStr)
      { /*If: I had a memory error*/
         errMacC = def_memErr_miruTblST;
         break;
      } /*If: I had a memory error*/

      *(buffStrPtr) = tmpMacStr;
      tmpMacStr += totalBytesMacUL;
      spareBuffMacI = *(lenBuffIPtr);
      *(lenBuffIPtr) <<= 1;
   } /*Loop: Read in the first line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-24 Sec-04:
   ^   - Finish setting variables and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *(*(buffStrPtr) + totalBytesMacUL) = '\0';

   *(posIPtr) = 0;
   *(bytesReadInIPtr) = (int) totalBytesMacUL;

   return errMacC;
} /*readMiruTblLine*/

/*-------------------------------------------------------\
| Fun-25: readMiruTbl
|   - Read in an MIRU table from a tsv file and store it
|     in an miruTbl structure
| Input:
|   - miruTblStr:
|     o C-string with the path/name of the MIRU amp table
|       to read in
|   - errCPtr:
|     o Character ponter to store the error message
| Output:
|   - Modifies:
|     o errCPtr to be 0 for no errors
|     o errCPtr to be def_fileErr_miruTblST for an file
|       error
|     o errCPtr to be def_memErr_miruTblST for an memory
|       error
|   - Returns:
|     o An pointer to a miruTbl structure
|     o 0 for an error
\-------------------------------------------------------*/
struct miruTbl *
readMiruTbl(
   char *miruTblStr, /*tsv with path to MIRU table read*/
   char *errCPtr     /*Holds error message*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-25 TOC: readMiruTbl
   '   - Read in an MIRU table from a tsv file and store
   '     it in an miruTbl structure
   '   o fun-25 sec-01:
   '     - Variable declerations
   '   o fun-25 sec-02:
   '     - Allocate memory and get the first line
   '   o fun-25 sec-03:
   '     - Find the number of lines (lineages) in the file
   '   o fun-25 sec-04:  
   '   - Get the postion and length of each pimer
   '   o fun-25 sec-05:
   '     - Get each lineages length for each primer
   '   o fun-25 sec-06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *buffStr = 0;
   int lenBuffI = 1 << 11;
   int posInBuffI = 0;
   char errC = 0;

   char *tmpStr = 0;
   int totalBytesI = 0;

   struct miruTbl *tblST = 0;
   int iAmp = 0; /*Amplicon I am currently on*/
   int iLin = 0; /*Lineage on in loop*/

   FILE *tblFILE = fopen(miruTblStr, "r");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-02:
   ^   - Allocate memory and get the first line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! tblFILE) goto fileErr_readMiruTbl;

   buffStr = malloc((lenBuffI + 1) * sizeof(char));

   if(! buffStr) goto memErr_readMiruTbl;

   tblST = malloc(sizeof(struct miruTbl));

   if(! tblST) goto memErr_readMiruTbl;

   initMiruTbl(tblST);

   errC =
      readMiruTblLine(
         &buffStr,
         &posInBuffI,
         &lenBuffI,
         &totalBytesI,
         tblFILE
      );

   if(errC & def_memErr_miruTblST)
      goto memErr_readMiruTbl;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-03:
   ^   - Find the number of lines (lineages) in the file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = buffStr;

   while(! errC)
   { /*Loop: Read in each line*/
      tmpStr += ulEndStrLine(tmpStr);

      tblST->numLineagesI += (*tmpStr == '\n');

      ++tmpStr; /*Get off the new line*/

      /*Check if I have more characters in the buffer*/
      if(tmpStr < buffStr + totalBytesI) continue;

      errC =
         readMiruTblLine(
            &buffStr,
            &posInBuffI,
            &lenBuffI,
            &totalBytesI,
            tblFILE
         );

      tmpStr = buffStr;
   } /*Loop: Read in each line*/

   if(errC &def_memErr_miruTblST) goto memErr_readMiruTbl;

   /* I need to finsh of the file*/
   while(posInBuffI < totalBytesI)
   { /*Loop: find the end of the file*/
      posInBuffI += ulEndStrLine(&buffStr[posInBuffI]);
      tblST->numLineagesI += (buffStr[posInBuffI] =='\n');
      ++posInBuffI; /*Get off the new line*/
   } /*Loop: find the end of the file*/

   /*Account for overcounting files that end in newline*/
   tblST->numLineagesI -=
      (buffStr[posInBuffI - 1] == '\n');

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-04:
   ^   - Get the postion and length of each pimer
   ^   o fun-25 sec-04 sub-01:
   ^     - Find the number of amplicons in the header
   ^   o fun-25 sec-04 sub-02:
   ^     - Assign memory for the amplicons
   ^   o fun-25 sec-04 sub-03:
   ^     - Extract stats for each amplicon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-25 Sec-04 Sub-01:
   *   - Find the number of amplicons in the header
   \*****************************************************/

   fseek(tblFILE, 0, SEEK_SET);

   if(tblST->numLineagesI < 2) goto fileErr_readMiruTbl;

   /*I have already know this line exists*/
   tmpStr = fgets(buffStr, lenBuffI, tblFILE);

   /*Count the number of amplicons. The last column ends
   `   in '\n' (no '\t') so I am safe counting the 
   `   first '\t'
   */
   while(*tmpStr++ != '\n') 
      tblST->numPrimI += (*tmpStr == '\t');

   /*****************************************************\
   * Fun-25 Sec-04 Sub-02:
   *   - Assign memory for the amplicons
   \*****************************************************/

   tblST->lineageStrAry =
      malloc(
           (tblST->numLineagesI * def_lenStr_miruTblST)
         * sizeof(char)
      ); /*Assign memory for the lineage name array*/

   if(! tblST->lineageStrAry) goto memErr_readMiruTbl;

   tblST->miruAmpSTAry =
      malloc(tblST->numPrimI * sizeof(struct miruAmp));

   if(! tblST->miruAmpSTAry) goto memErr_readMiruTbl;

   tblST->ampIndexIAry =
      malloc(tblST->numPrimI * sizeof(int));

   if(! tblST->ampIndexIAry) goto memErr_readMiruTbl;

   for(iAmp = 0; iAmp < tblST->numPrimI; ++iAmp)
   { /*Loop: Allocate memory for lineages in primer*/
      tblST->miruAmpSTAry[iAmp].linLenIAry =
         malloc(tblST->numLineagesI * sizeof(int));

      if(! tblST->miruAmpSTAry[iAmp].linLenIAry)
         goto memErr_readMiruTbl;

      tblST->miruAmpSTAry[iAmp].numHitsIAry =
         malloc(tblST->numLineagesI * sizeof(int));

      if(! tblST->miruAmpSTAry[iAmp].numHitsIAry)
         goto memErr_readMiruTbl;

      tblST->miruAmpSTAry[iAmp].indexIAry =
         malloc(tblST->numLineagesI * sizeof(int));

      if(! tblST->miruAmpSTAry[iAmp].indexIAry)
         goto memErr_readMiruTbl;

      tblST->miruAmpSTAry[iAmp].numLinI =
         tblST->numLineagesI;

      tblST->ampIndexIAry[iAmp] = iAmp;
   } /*Loop: Allocate memroy for lineages in primer*/

   /*****************************************************\
   * Fun-25 Sec-04 Sub-03:
   *   - Extract stats for each amplicon
   *   o fun-25 sec-04 sub-03 cat-01:
   *     - Get past lineage column & start primer name
   *       extraction loop
   *   o fun-25 sec-04 sub-03 cat-02:
   *     - Get the primer name
   *   o fun-25 sec-04 sub-03 cat-03:
   *     - Get the foward starting position
   *   o fun-25 sec-04 sub-03 cat-04:
   *     - Get the foward primers length
   *   o fun-25 sec-04 sub-03 cat-05:
   *     - Get the reverse primers starting position
   *   o fun-25 sec-04 sub-03 cat-06:
   *     - Get the reverse primers starting position
   *   o fun-25 sec-04 sub-03 cat-07:
   *     - Get the reverse primers length
   *   o fun-25 sec-04 sub-03 cat-08:
   *     - Move to the next primers entry
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-25 Sec-04 Sub-03 Cat-01:
   +   - Get past lineage column & start primer name loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = buffStr;
   while(*tmpStr++ != '\t') {};

   for(iAmp = 0; iAmp < tblST->numPrimI; ++iAmp)
   { /*Loop: Extract each primers stats*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-02:
      +   - Get the primer name
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpStr +=
         cCpStrDelim(
            tblST->miruAmpSTAry[iAmp].idStr,
            tmpStr,
            '.'
         ); /*Copy the primer id*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-03:
      +   - Get the foward starting position
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.') goto primNameErr_readMiruTbl;

      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_readMiruTbl;
  
      tmpStr =
         base10StrToSI(
            tmpStr,
            tblST->miruAmpSTAry[iAmp].forPosI
         );

      /*Convert to index 0*/
      --(tblST->miruAmpSTAry[iAmp].forPosI);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-04:
      +   - Get the foward primers length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.') goto primNameErr_readMiruTbl;
      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_readMiruTbl;
  
      tmpStr =
         base10StrToSI(
            tmpStr,
            tblST->miruAmpSTAry[iAmp].forLenI
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-05:
      +   - Get the reverse primers starting position
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.') goto primNameErr_readMiruTbl;
      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_readMiruTbl;
  
      tmpStr =
         base10StrToSI(
            tmpStr,
            tblST->miruAmpSTAry[iAmp].revPosI
         );

      /*Convert to index 0*/
      --(tblST->miruAmpSTAry[iAmp].revPosI);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-06:
      +   - Get the reverse primers length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*tmpStr != '.') goto primNameErr_readMiruTbl;
      ++tmpStr;

      if(*tmpStr < 48 && *tmpStr > 57)
         goto  primNameErr_readMiruTbl;
  
      tmpStr =
         base10StrToSI(
            tmpStr,
            tblST->miruAmpSTAry[iAmp].revLenI
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-25 Sec-04 Sub-03 Cat-07:
      +   - Move to the next primers entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*Move to the next entry*/
      while(*tmpStr++ > 31) {}
   } /*Loop: Extract each primers stats*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-05:
   ^   - Get each lineages length for each primer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   iLin = 0; /*Lineage on*/

   while(fgets(buffStr, lenBuffI, tblFILE))
   { /*Loop: Get the row for each linage from the file*/
      iAmp = 0;
      tmpStr = buffStr;

      /*Get the lineages name*/
      tmpStr +=
         cCpStrDelim(
            &tblST->lineageStrAry[getStrAryIndex(iLin)],
            tmpStr,
            '\t'
         ); /*Copy over the lineages name*/

      while(*tmpStr & (~ '\n'))
      { /*Loop: Get the lineages length for each primer*/
         ++tmpStr;

         tmpStr =
            base10StrToSI(
               tmpStr,
               tblST->miruAmpSTAry[iAmp].linLenIAry[iLin]
            );

         /*Record the original index*/
         tblST->miruAmpSTAry[iAmp].indexIAry[iLin]= iLin;

         tblST->miruAmpSTAry[iAmp].numHitsIAry[iLin] = 0;
         ++iAmp;
      } /*Loop: Get the lineages length for each primer*/

      ++iLin; /*Move to the next lineage*/
   } /*Loop: Get the row for each linage from the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-25 Sec-06:
   ^   - Clean up
   ^   o fun-25 sec-06 sub-01:
   ^     - Clean up for success
   ^   o fun-25 sec-06 sub-02:
   ^     - Clean up for memory errors
   ^   o fun-25 sec-06 sub-03:
   ^     - Clean up for file errors
   ^   o fun-25 sec-06 sub-04:
   ^     - Clean up for invalid primer names in table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-25 Sec-06 Sub-01:
   *   - Clean up for success
   \*****************************************************/

   *errCPtr = 0;
   free(buffStr);
   fclose(tblFILE);

   sortMiruTbl(tblST);

   return tblST;

   /*****************************************************\
   * Fun-25 Sec-06 Sub-02:
   *   - Clean up for memory errors
   \*****************************************************/

   memErr_readMiruTbl:;

   *errCPtr = def_memErr_miruTblST;

   if(tblST) freeMiruTbl(&tblST);
   if(buffStr) free(buffStr);
   if(tblFILE) fclose(tblFILE);
 
   return 0;

   /*****************************************************\
   * Fun-25 Sec-06 Sub-03:
   *   - Clean up for file errors
   \*****************************************************/

   fileErr_readMiruTbl:;

   *errCPtr = def_fileErr_miruTblST;

   if(tblST) freeMiruTbl(&tblST);
   if(buffStr) free(buffStr);
   if(tblFILE) fclose(tblFILE);
 
   return 0;

   /*****************************************************\
   * Fun-25 Sec-06 Sub-04:
   *   - Clean up for invalid primer names in table
   \*****************************************************/

   primNameErr_readMiruTbl:;

   *errCPtr = def_primNameInvalid_miruTblST;

   if(tblST) freeMiruTbl(&tblST);
   if(buffStr) free(buffStr);
   if(tblFILE) fclose(tblFILE);
 
   return 0;
} /*readMiruTbl*/

/*-------------------------------------------------------\
| Fun-26: pMiruTbl
|   - Prints the MIRU table to a file. This prints
|     the number of hits per primer lineage.
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to print out
|   - outFileStr:
|     o C-string with path of file to print to. Null (0)
|       and '-' are treated as stdout
| Output:
|   - Prints:
|     o The miruTbl and the number of sequences that
|       support each position to outFileStr.
|   - Returns:
|     o 0 for success
|     o def_fileErr_miruTblST if the file could not be
|       opened
\-------------------------------------------------------*/
char
pMiruTbl(
   struct miruTbl *miruTblSTPtr,
   char *outFileStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-26 TOC: pMiruTbl
   '   - Prints the MIRU table to a file. This prints
   '     the number of hits per primer lineage.
   '   o fun-26 sec-01:
   '     - Variable declerations
   '   o fun-26 sec-02:
   '     - Check I I can open the output file
   '   o fun-26 sec-03:
   '     - Print out the header (MIRU primers)
   '   o fun-26 sec-04:
   '     - Print out the table of hits and lineage names
   '   o fun-26 sec-05:
   '     - Print out final parts and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int lenBuffI = 2 << 11; /*Around 4kb*/
   char buffStr[lenBuffI]; /*Around 4kb*/
   int posI = 0;
   int iLin = 0;
   int iAmp = 0;
   int linIndexI = 0; /*For sanity*/
   ulong percMappedUL = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-02:
   ^   - Check I I can open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: The user supplied an file*/
      outFILE = fopen(outFileStr, "w");

      if(! outFILE) return def_fileErr_miruTblST;
   } /*Else: The user supplied an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-03:
   ^   - Print out the header (MIRU primers)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I need to unsort the table, otherwise the output
   `   table will be garbage
   */
   miruTbl_unsort(miruTblSTPtr);

   posI +=
      ulCpStrDelim(&buffStr[posI],"Lineages",0,'\0');

   for(iAmp = 0; iAmp < miruTblSTPtr->numPrimI; ++iAmp)
   { /*Loop: Print out the primer names*/
      if(lenBuffI - posI < (def_lenStr_miruTblST << 2))
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      buffStr[posI++] = '\t';

      posI +=
         ulCpStrDelim(
            &buffStr[posI],
            miruTblSTPtr->miruAmpSTAry[iAmp].idStr,
            0,
            '\0'
         ); /*Copy the amplicons name*/

      buffStr[posI++] = 'H';
      buffStr[posI++] = 'i';
      buffStr[posI++] = 't';

      buffStr[posI++] = '\t';

      posI +=
         ulCpStrDelim(
            &buffStr[posI],
            miruTblSTPtr->miruAmpSTAry[iAmp].idStr,
            0,
            '\0'
         ); /*Copy the amplicons name*/

      buffStr[posI++] = 'P';
      buffStr[posI++] = 'e';
      buffStr[posI++] = 'r';
      buffStr[posI++] = 'c';

      buffStr[posI++] = '\t';

      posI +=
         ulCpStrDelim(
            &buffStr[posI],
            miruTblSTPtr->miruAmpSTAry[iAmp].idStr,
            0,
            '\0'
         ); /*Copy the amplicons name*/

      buffStr[posI++] = 'T';
      buffStr[posI++] = 'o';
      buffStr[posI++] = 't';
      buffStr[posI++] = 'a';
      buffStr[posI++] = 'l';
   } /*Loop: Print out the primer names*/

   if(lenBuffI - posI < def_lenStr_miruTblST)
   { /*If: I need to print out the buffer*/
      fwrite(buffStr, sizeof(char), posI, outFILE);
      posI = 0;
   } /*If: I need to print out the buffer*/

   buffStr[posI++] = '\n';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-04:
   ^   - Print out the table of hits and lineage names
   ^   o fun-26 sec-04 sub-01:
   ^     - Start lineage loop and copy lineage name
   ^   o fun-26 sec-04 sub-02:
   ^     - Start primer loop & print # hits per lineage
   ^   o fun-26 sec-04 sub-03:
   ^     - End the lineages row with an newline
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-26 Sec-04 Sub-01:
   *   - Start lineage loop and copy lineage name
   \*****************************************************/

   for(iLin=0; iLin < miruTblSTPtr->numLineagesI; ++iLin)
   { /*Loop: Copy the entries for each lineage*/

      if(lenBuffI - posI < def_lenStr_miruTblST)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      /*Copyt the lineage name, they should be sort*/
      linIndexI = getStrAryIndex(iLin);

      posI +=
         cCpStrDelim(
            &buffStr[posI],
            &miruTblSTPtr->lineageStrAry[linIndexI],
            '\0'
         );

      /**************************************************\
      * Fun-26 Sec-04 Sub-02:
      *   - Start primer loop & print # hits per lineage
      *   o fun-26 sec-04 sub-02 cat-01:
      *     - Copy the number of hits to the buffer
      *   o fun-26 sec-04 sub-02 cat-02:
      *     - Find and copy the percentage of mapped reads
      *   o fun-26 sec-04 sub-02 cat-03:
      *     - copy the total number of reads to buffer
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-26 Sec-04 Sub-02 Cat-01:
      +   - Copy the number of hits to the buffer
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      for(iAmp=0; iAmp < miruTblSTPtr->numPrimI; ++iAmp)
      { /*Loop: Print the number of hits per primer*/

         if(lenBuffI - posI < def_lenStr_miruTblST + 32)
         { /*If: I need to print out the buffer*/
            fwrite(buffStr, sizeof(char), posI, outFILE);
            posI = 0;
         } /*If: I need to print out the buffer*/

         buffStr[posI++] = '\t';

         posI +=
            numToStr(
               &buffStr[posI],
               miruTblSTPtr->miruAmpSTAry[
                  iAmp
               ].numHitsIAry[iLin]
            ); /*Copy the number of hits to lineage*/

         buffStr[posI++] = '\t';

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-26 Sec-04 Sub-02 Cat-02:
         +   - Find and copy percentage of mapped reads
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(lenBuffI - posI < def_lenStr_miruTblST)
         { /*If: I need to print out the buffer*/
            fwrite(buffStr, sizeof(char), posI, outFILE);
            posI = 0;
         } /*If: I need to print out the buffer*/

         if(! miruTblSTPtr->miruAmpSTAry[iAmp].mapReadsI)
         { /*If: no reads mapped to this primer*/
            buffStr[posI++] = '0';
            buffStr[posI++] = '\t';
            buffStr[posI++] = '0';

            goto linCpTotal_fun26_sec04_sub02_cat03;
         } /*If: no reads mapped to this primer*/

         /*This is to allow me to keep the percentage as
         `    an integer
         */
         percMappedUL =
              10000
            * miruTblSTPtr->miruAmpSTAry[
                 iAmp
              ].numHitsIAry[iLin];
           
         percMappedUL /=
            miruTblSTPtr->miruAmpSTAry[iAmp].mapReadsI;

         /*Copy the non-decimal part of the percentage*/
         posI +=
            numToStr(&buffStr[posI], percMappedUL / 100);

         buffStr[posI++] = '.';
         
         /*Copy the decimal part of the percentage*/
         posI +=
            numToStr(&buffStr[posI], percMappedUL % 100);

         buffStr[posI++] = '\t';

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-26 Sec-04 Sub-02 Cat-03:
         +   - copy the total number of reads to buffer
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         linCpTotal_fun26_sec04_sub02_cat03:;

         if(lenBuffI - posI < def_lenStr_miruTblST)
         { /*If: I need to print out the buffer*/
            fwrite(buffStr, sizeof(char), posI, outFILE);
            posI = 0;
         } /*If: I need to print out the buffer*/

         posI +=
            numToStr(
               &buffStr[posI],
               miruTblSTPtr->miruAmpSTAry[iAmp].mapReadsI
            ); /*Copy total number mapped reads*/
      } /*Loop: Print the number of hits per primer*/

      if(lenBuffI - posI < def_lenStr_miruTblST)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      /**************************************************\
      * Fun-26 Sec-04 Sub-03:
      *   - End the lineages row with an newline
      \**************************************************/

      buffStr[posI++] = '\n';
   } /*Loop: Copy the entries for each lineage*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-05:
   ^   - Copy the number of missed reads
   ^   o fun-26 sec-05 sub-01:
   ^     - Copy unkown lineage id and start print loop
   ^   o fun-26 sec-05 sub-02:
   ^     - Copy number of no lineage detected reads
   ^   o fun-26 sec-05 sub-03:
   ^     - Get and copy percentage of no lineage reads
   ^   o fun-26 sec-05 sub-04:
   ^     - copy total number of reads to buffer
   ^   o fun-26 sec-05 sub-05:
   ^     - Add new line to end of unkown lineage line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-26 Sec-05 Sub-01:
   *   - Copy unkown lineage id and start print loop
   \*****************************************************/

   if(lenBuffI - posI < def_lenStr_miruTblST)
   { /*If: I need to print out the buffer*/
      fwrite(buffStr, sizeof(char), posI, outFILE);
      posI = 0;
   } /*If: I need to print out the buffer*/

   posI +=
      cCpStrDelim(
         &buffStr[posI],
         "NA",
         '\0'
      );

   for(iAmp = 0; iAmp < miruTblSTPtr->numPrimI; ++iAmp)
   { /*Loop: Copy the unmapped counts*/

      if(lenBuffI - posI < def_lenStr_miruTblST)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      /**************************************************\
      * Fun-26 Sec-05 Sub-02:
      *   - Copy number of no lineage detected reads
      \**************************************************/

      if(lenBuffI - posI < def_lenStr_miruTblST + 32)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      buffStr[posI++] = '\t';

      posI +=
         numToStr(
            &buffStr[posI],
            miruTblSTPtr->miruAmpSTAry[iAmp].noLinSI
         ); /*Copy the number of hits to lineage*/

      buffStr[posI++] = '\t';

      /**************************************************\
      * Fun-26 Sec-05 Sub-03:
      *   - Get and copy percentage of no lineage reads
      \**************************************************/

      if(lenBuffI - posI < def_lenStr_miruTblST + 32)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      if(! miruTblSTPtr->miruAmpSTAry[iAmp].noLinSI)
      { /*If: no reads mapped to this primer*/
         buffStr[posI++] = '0';
         buffStr[posI++] = '\t';
         buffStr[posI++] = '0';

         goto noLinCpTotal_fun26_sec05_sub03;
      } /*If: no reads mapped to this primer*/

      /*This is to allow me to keep the percentage as
      `    an integer
      */
      percMappedUL =
           10000
         * (miruTblSTPtr->miruAmpSTAry[iAmp].noLinSI);
        
      percMappedUL /=
         miruTblSTPtr->miruAmpSTAry[iAmp].mapReadsI;

      /*Copy the non-decimal part of the percentage*/
      posI +=
         numToStr(
            &buffStr[posI],
            percMappedUL / 100
         );

      buffStr[posI++] = '.';
      
      /*Copy the decimal part of the percentage*/
      posI +=
         numToStr(
            &buffStr[posI],
            percMappedUL % 100
         );

      buffStr[posI++] = '\t';

      /**************************************************\
      * Fun-26 Sec-05 Sub-04:
      *   - copy total number of reads to buffer
      \**************************************************/

      noLinCpTotal_fun26_sec05_sub03:;

      if(lenBuffI - posI < def_lenStr_miruTblST)
      { /*If: I need to print out the buffer*/
         fwrite(buffStr, sizeof(char), posI, outFILE);
         posI = 0;
      } /*If: I need to print out the buffer*/

      posI +=
         numToStr(
            &buffStr[posI],
            miruTblSTPtr->miruAmpSTAry[iAmp].mapReadsI
         ); /*Copy total number mapped reads*/

   } /*Loop: Copy the unmapped counts*/

   /*****************************************************\
   * Fun-26 Sec-05 Sub-05:
   *   - Add new line to end of unkown lineage line
   \*****************************************************/

   if(lenBuffI - posI < def_lenStr_miruTblST)
   { /*If: I need to print out the buffer*/
      fwrite(buffStr, sizeof(char), posI, outFILE);
      posI = 0;
   } /*If: I need to print out the buffer*/

   buffStr[posI++] = '\n';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-26 Sec-05:
   ^   - Print out final parts and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fwrite(buffStr, sizeof(char), posI, outFILE);

   if(outFILE != stdout) fclose(outFILE);
   sortMiruTbl(miruTblSTPtr); /*So can be re-used*/

   return 0;
} /*pMiruTbl*/

/*-------------------------------------------------------\
| Fun-27: pLineages
|   - Prints the "best" MIRU lineages for each primer
| Input:
|   - miruTblSTPtr:
|     o Pointer to an miruTbl structure to get lineages
|       from
|   - outFileStr:
|     o C-string with path of file to print to. Null (0)
|       and '-' are treated as stdout
| Output:
|   - Prints:
|     o The lineage for each primer to outFileStr
|   - Returns:
|     o 0 for success
|     o def_fileErr_miruTblST if the file could not be
|       opened
\-------------------------------------------------------*/
char
pLineages(
   struct miruTbl *miruTblSTPtr,
   char *outFileStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-27 TOC: pLineages
   '   - Prints the "best" MIRU lineages for each primer
   '   o fun-27 sec-01:
   '     - Variable declerations
   '   o fun-27 sec-02:
   '     - Check if I can open the output file
   '   o fun-27 sec-03:
   '     - unsort table and print out the MIRU lineages
   '   o fun-27 sec-04:
   '     - Resort the table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-27 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int iPrim = 0;
   int iLin = 0;
   int bestLinI = 0;

   int *hitAryI = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-27 Sec-02:
   ^   - Check if I can open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: The user supplied an file*/
      outFILE = fopen(outFileStr, "w");

      if(! outFILE) return def_fileErr_miruTblST;
   } /*Else: The user supplied an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-27 Sec-03:
   ^   - unsort table and print out the MIRU lineages
   ^   o fun-27 sec-03 sub-01:
   ^     - unsort table/print out header; has primer names
   ^   o fun-27 sec-03 sub-02:
   ^     - Print out the "best" lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-27 Sec-03 Sub-01:
   *   - unsort table & print out header; has primer names
   \*****************************************************/

   miruTbl_unsort(miruTblSTPtr);

   fprintf(outFILE, "Primer");

   for(iPrim = 0; iPrim < miruTblSTPtr->numPrimI; ++iPrim)
      fprintf(
         outFILE,
         "\t%s",
          miruTblSTPtr->miruAmpSTAry[iPrim].idStr
      ); /*Print out the primer name*/

   /*****************************************************\
   * Fun-27 Sec-03 Sub-02:
   *   - Print out the "best" lineages
   \*****************************************************/

   fprintf(outFILE, "\nLineage");

   for(iPrim = 0; iPrim < miruTblSTPtr->numPrimI; ++iPrim)
   { /*Loop: Print out the linage for each primer*/
      hitAryI =
         miruTblSTPtr->miruAmpSTAry[iPrim].numHitsIAry;

      bestLinI = 0;

      for(
         iLin = 0;
         iLin < miruTblSTPtr->numLineagesI;
         ++iLin
      ){ /*Loop: Find the most supported lineage*/
         /*This is a little faster than an ifstatment
         `   for random cases with branch prediction
         */
         bestLinI = 
            noBrachIfMax(
               hitAryI[iLin],
               hitAryI[bestLinI],
               iLin,
               bestLinI
            ); /*Get the best index*/
      } /*Loop: Find the most supported lineage*/

      if(hitAryI[bestLinI] > 0)
      { /*If: I have an lineage to print out*/
         bestLinI = getStrAryIndex(bestLinI);

         fprintf(
            outFILE,
            "\t%s",
            &miruTblSTPtr->lineageStrAry[bestLinI]
         );
      } /*If: I have an lineage to print out*/

      else fprintf(outFILE, "\tNA");
   } /*Loop: Print out the linage for each primer*/

   fprintf(outFILE, "\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-27 Sec-04:
   ^   - Resort the table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortMiruTbl(miruTblSTPtr); /*So can be re-used*/
   if(outFILE != stdout) fclose(outFILE);
   return 0;
} /*pLineages*/

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
