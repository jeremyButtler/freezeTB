/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' linST SOF:
'   - has structure and functions to read a lineage
'     database
'   o header:
'     - guards, defined variables, & forward declarations
'   * structures
'     o .h st01: one_linST
'       - has data for one lineage
'     o .h st02: simple_linST:
'       - holds a array of one_linST structures, which are
'         simple lineages that are made from single
'         variants and the supporting array for default
'         lineage detection
'     o .h st03: multi_linST
'       - has data for a complex (multi-variant) lineage
'     o .h st04: complex_linST
'       - has a mulit_linST (complex (multi-variant))
'         lineage array
'   * one_linST structures
'     + one_linST general functions
'       o fun01: blank_one_linST
'         - blanks (sets to defaults) a one_linST
'       o fun02: init_one_linST
'         - initializes (all values to 0) a one_linST
'       o fun03: freeStack_one_linST
'         - frees arrays & blanks variables in one_linST
'       o fun04: freeHeap_one_linST
'         - frees a one_linST strucutre
'       o fun05: freeHeapAry_one_linST
'         - frees a one_linST struct array
'     + one_linST sorting and swap functions
'       ! these functions are hidden to avoid users
'         messing things up. Just using them will cause
'         errors for the multi_linST structures.
'       o .c fun06: swap_one_linST
'         - swap values between one_linST structures
'       o .c fun07: sortPosAry_one_linST
'         - sorts one_linST struct array by start position
'       o .c fun08: sortIdAry_one_linST
'         - sorts a one_linST struct array by variant id
'       o .c fun09: sortGroupAry_one_linST
'         - sorts one_linST struct array by variant group
'           and variants in a group by variant id
'       o .c fun10: unsort_one_linST
'         - unsorts a one_linST array using an array of
'           original index's
'     + one_linST find functions (all need a sorted array)
'       ! fun12 and fun 12 are hidden becuase fun39
'         retuns a position sorted array
'       o fun11: posFindAry_one_linST
'         - find the first variant that is between the
'           start and end position
'       o .c fun12: idFind_one_linST
'         - find the variant with the input id
'       o .c fun13: groupFindAry_one_linST
'         - find the first variant that is in the group
'   * simple_linST structure general functions
'     o fun14: blank_simple_linST
'       - does a lazy blank and sets all lengths in a
'         simple_linST struct to 0
'     o fun15: init_simple_linST
'       - sets array addresses in simple_linST to 0/null
'     o fun16: freeStack_simple_linST
'       - frees all arrays in a simple_linST struct
'     o fun17: freeHeap_simple_linST
'       - frees a simple_linST struct
'     o fun18: addMem_simple_linST
'       - adds memory to a simple_linST struct
'     o fun19: clearGroup_simple_linST
'       - clears all detected groups in a simple_linST
'     o fun20: unsort_simple_linST
'       - unsorts a simple_linST struct using an array
'         of original index's
'   * multi_linST structures
'     o fun21: blank_multi_linST
'       - blanks (sets to defaults) a multi_linST struct
'     o fun22: init_multi_linST
'       - initializes a multi_linST structure
'     o fun23: freeStack_multi_linST
'       - frees variables in a multi_linST structure
'     o fun24: freeHeap_multi_linST
'       - frees a multi_linST structure
'     o fun25: freeHeapAry_multi_linST
'       - frees a multi_linST struct array
'   * complex_linST structure general functions
'      o fun26: blank_complex_linST
'        - sets length in a complex_linST to 0;(lazy blank
'      o fun27: init_complex_linST
'        - sets all values complex_linST to 0/null
'      o fun28: freeStack_complex_linST
'        - frees all variabls in a complex_linST struct
'      o fun29: freeHeap_complex_linST
'        - frees a complex_linST struct
'      o fun30: addMem_complex_linST
'        - adds memory to a complex_linST struct
'      ! fun31 to fun36 are only used in getting complex
'        lineages to decrease search time in reading in
'        the database
'      o .c fun31: swap_complex_linST
'        - swap two index in a complex lineage array
'      o .c fun32: addIdSort_complex_linST
'        - add complex id to an lineage by sorting
'      o .c fun33: idFind_complex_linST
'        - finds an id in an id sorted complex_linST
'        - this is only used in the reading in function
'      o .c fun34: groupSort_complex_linST
'        - sorts a mult_linST array in a complex_linST
'          struct by the group id
'      o .c fun35: positionSort_complex_linST
'        - sorts a mult_linST array in a complex_linST
'          struct by the position (end coordinate)
'        - this sort also makes sure that lineages that
'          have more variants are at the end
'        - between end and total at ends, this ensures
'          complex lineages that are depenent on other
'          complex lineages end up at the end
'      o .c fun36: unsort_complex_linST
'        - unsort a mult_linST array in a complex_linST
'          by the original index's
'      o fun37: posFind_complex_linST
'        - find the first complex variant that is between
'          the start and end position in a complex_linST
'   * general functions
'      o .c fun38: flipIndexAry_complex_linST
'        - changes the index array so that the stored
'          index is the position and the old position is
'          the new stored index
'     o fun39: getSimpleLineages_linST
'       - gets the lineages from the variants lineage file
'     o .c fun40: intInsert_linST
'       - inserts an integer into an array at its sorted
'         position (this does not insert duplicates)
'     o fun41: getComplexLineages_linST
'       - gets the lineages from the complex lineage file
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

#include "linST.h"
#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genLib/fileFun.h"
#include "../genAln/kmerFind.h"

/*.h files only*/
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/shellShort.h"
!   - .c  #include "../genBio/seqST.h"
!   - .c  #include "../genAln/alnSet.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/memwater.h"
!   - .h  #include "../genLib/endLine.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_one_linST
|   - blanks (sets to defaults) a one_linST structure
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to blank
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o all arrays to be set to 0/null
\-------------------------------------------------------*/
void
blank_one_linST(
   struct one_linST *linSTPtr
){
   signed int lenSI = 0;

   if(! linSTPtr)
     return;

   if(linSTPtr->geneStr)
      linSTPtr->geneStr[0] = 0;

   if(linSTPtr->groupStr)
      linSTPtr->groupStr[0] = 0;

   if(linSTPtr->idStr)
      linSTPtr->idStr[0] = 0;

   if(linSTPtr->lineageStr)
      linSTPtr->lineageStr[0] = 0;


   if(linSTPtr->seqAryStr)
   { /*If: have length*/
      for(lenSI = 0; lenSI < linSTPtr->seqLenSI; ++lenSI)
      { /*Loop: set all patterns to null*/
         if(linSTPtr->seqAryStr[lenSI])
            linSTPtr->seqAryStr[lenSI][0] = 0;
      } /*Loop: set all patterns to null*/
   } /*If: have length*/

   if(linSTPtr->fastAryST)
   { /*If: using kmer find patterns*/
      for(lenSI = 0; lenSI < linSTPtr->seqLenSI; ++lenSI)
        blank_refST_kmerFind(&linSTPtr->fastAryST[lenSI]);
   } /*If: using kmer find patterns*/

   if(linSTPtr->seqLenArySI)
   { /*If: using kmer find patterns*/
      for(lenSI = 0; lenSI < linSTPtr->seqLenSI; ++lenSI)
         linSTPtr->seqLenArySI[lenSI] = 0;
   } /*If: using kmer find patterns*/


   linSTPtr->seqLenSI = 0;
   linSTPtr->refMatchBl = 0;
   linSTPtr->minScoreSI = 0;
   linSTPtr->fudgeSI = 0;
   linSTPtr->trsGapSI = 0;
   linSTPtr->trsLineageSI = 0;

   linSTPtr->startSI = 0;
   linSTPtr->endSI = 0;
   linSTPtr->moleculeTypeSC = def_unknown_linST;
   linSTPtr->checkTypeSC = def_unknown_linST;

   linSTPtr->defGroupBl = 0;
   linSTPtr->printLinBl = 0;
} /*blank_one_linST*/

/*-------------------------------------------------------\
| Fun02: init_one_linST
|   - initializes (all values to 0) a one_linST structure
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to initialize
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o all arrays to be 0
\-------------------------------------------------------*/
void
init_one_linST(
   struct one_linST *linSTPtr
){
   if(! linSTPtr)
     return;

   linSTPtr->geneStr = 0;
   linSTPtr->groupStr = 0;
   linSTPtr->idStr = 0;
   linSTPtr->lineageStr = 0;
   linSTPtr->seqAryStr = 0;
   linSTPtr->seqLenArySI = 0;
   linSTPtr->seqSizeSI = 0;
   linSTPtr->fastAryST = 0;

   blank_one_linST(linSTPtr);
} /*init_one_linST*/

/*-------------------------------------------------------\
| Fun03: freeStack_one_linST
|   - frees all arrays and blanks variables in a one_linST
| Input:
|   - linSTPtr:
|     o one_linST structure pointer with variables to free
|       and blank
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o frees all arrays (not keeping track)
\-------------------------------------------------------*/
void
freeStack_one_linST(
   struct one_linST *linSTPtr
){
   signed int lenSI = 0;

   if(! linSTPtr)
     return;

   if(linSTPtr->geneStr)
      free(linSTPtr->geneStr);
   if(linSTPtr->groupStr)
      free(linSTPtr->groupStr);
   if(linSTPtr->idStr)
      free(linSTPtr->idStr);
   if(linSTPtr->lineageStr)
      free(linSTPtr->lineageStr);
   if(linSTPtr->seqLenArySI)
      free(linSTPtr->seqLenArySI);

   if(linSTPtr->seqAryStr)
   { /*If: have sequence patterns*/
      for(lenSI = 0; lenSI < linSTPtr->seqSizeSI; ++lenSI)
      { /*Loop: free all patterns*/
         if(linSTPtr->seqAryStr[lenSI])
            free(linSTPtr->seqAryStr[lenSI]);
         linSTPtr->seqAryStr[lenSI] = 0;
      } /*Loop: free all patterns*/
      free(linSTPtr->seqAryStr);
   } /*If: have sequence patterns*/

   if(linSTPtr->fastAryST)
      freeHeapAry_refST_kmerFind(
         linSTPtr->fastAryST,
         linSTPtr->seqLenSI
      );

   init_one_linST(linSTPtr);
} /*freeStack_one_linST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_one_linST
|   - frees a one_linST strucutre
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to free
| Output:
|   - Modifies:
|     o frees linSTPtr, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_one_linST(
   struct one_linST *linSTPtr
){
   if(! linSTPtr)
     return;

   freeStack_one_linST(linSTPtr);
   free(linSTPtr);
} /*freeHeap_one_linST*/

/*-------------------------------------------------------\
| Fun05: freeHeapAry_one_linST
|   - frees a one_linST struct array
| Input:
|   - linAryST:
|     o one_linST structure array to free
|   - lenSI:
|     o number of one_linST structures in the array
| Output:
|   - Modifies:
|     o frees linAryST, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeapAry_one_linST(
   struct one_linST *linAryST,
   signed int lenSI
){
   if(! linAryST)
     return;

   --lenSI;

   while(lenSI >= 0)
      freeStack_one_linST(&linAryST[lenSI--]);
   free(linAryST);
} /*freeHeapAry_one_linST*/

/*-------------------------------------------------------\
| Fun06: swap_one_linST
|   - swap values between one_linST structures
| Input:
|   - firstSTPtr:
|     o one_linST struct pointer to frist struct to swap
|   - secSTPtr:
|     o one_linST struct pointer to second struct to swap
| Output:
|   - Modifies:
|     o firstSTPtr to have values from secSTPtr
|     o secSTPtr to have values from firstSTPtr
\-------------------------------------------------------*/
void
swap_one_linST(
   struct one_linST *firstSTPtr,
   struct one_linST *secSTPtr
){
   signed char *swapStr = 0;
   signed char **swapStrPtr = 0;
   signed int *swapIntPtr = 0;
   struct refST_kmerFind *swapRefPtr = 0;

   swapStr = firstSTPtr->geneStr;
   firstSTPtr->geneStr = secSTPtr->geneStr;
   secSTPtr->geneStr = swapStr;

   swapStr = firstSTPtr->groupStr;
   firstSTPtr->groupStr = secSTPtr->groupStr;
   secSTPtr->groupStr = swapStr;

   swapStr = firstSTPtr->idStr;
   firstSTPtr->idStr = secSTPtr->idStr;
   secSTPtr->idStr = swapStr;


   firstSTPtr->startSI ^= secSTPtr->startSI;
   secSTPtr->startSI ^= firstSTPtr->startSI;
   firstSTPtr->startSI ^= secSTPtr->startSI;

   firstSTPtr->endSI ^= secSTPtr->endSI;
   secSTPtr->endSI ^= firstSTPtr->endSI;
   firstSTPtr->endSI ^= secSTPtr->endSI;


   firstSTPtr->moleculeTypeSC ^= secSTPtr->moleculeTypeSC;
   secSTPtr->moleculeTypeSC ^= firstSTPtr->moleculeTypeSC;
   firstSTPtr->moleculeTypeSC ^= secSTPtr->moleculeTypeSC;

   firstSTPtr->checkTypeSC ^= secSTPtr->checkTypeSC;
   secSTPtr->checkTypeSC ^= firstSTPtr->checkTypeSC;
   firstSTPtr->checkTypeSC ^= secSTPtr->checkTypeSC;


   swapStrPtr = firstSTPtr->seqAryStr;
   firstSTPtr->seqAryStr = secSTPtr->seqAryStr;
   secSTPtr->seqAryStr = swapStrPtr;

   swapIntPtr = firstSTPtr->seqLenArySI;
   firstSTPtr->seqLenArySI = secSTPtr->seqLenArySI;
   secSTPtr->seqLenArySI = swapIntPtr;

   firstSTPtr->seqSizeSI ^= secSTPtr->seqSizeSI;
   secSTPtr->seqSizeSI ^= firstSTPtr->seqSizeSI;
   firstSTPtr->seqSizeSI ^= secSTPtr->seqSizeSI;

   firstSTPtr->seqLenSI ^= secSTPtr->seqLenSI;
   secSTPtr->seqLenSI ^= firstSTPtr->seqLenSI;
   firstSTPtr->seqLenSI ^= secSTPtr->seqLenSI;

   firstSTPtr->refMatchBl ^= secSTPtr->refMatchBl;
   secSTPtr->refMatchBl ^= firstSTPtr->refMatchBl;
   firstSTPtr->refMatchBl ^= secSTPtr->refMatchBl;

   firstSTPtr->defGroupBl ^= secSTPtr->defGroupBl;
   secSTPtr->defGroupBl ^= firstSTPtr->defGroupBl;
   firstSTPtr->defGroupBl ^= secSTPtr->defGroupBl;

   swapRefPtr = firstSTPtr->fastAryST;
   firstSTPtr->fastAryST = secSTPtr->fastAryST;
   secSTPtr->fastAryST = swapRefPtr;


   firstSTPtr->minScoreSI ^= secSTPtr->minScoreSI;
   secSTPtr->minScoreSI ^= firstSTPtr->minScoreSI;
   firstSTPtr->minScoreSI ^= secSTPtr->minScoreSI;

   firstSTPtr->fudgeSI ^= secSTPtr->fudgeSI;
   secSTPtr->fudgeSI ^= firstSTPtr->fudgeSI;
   firstSTPtr->fudgeSI ^= secSTPtr->fudgeSI;

   firstSTPtr->trsGapSI ^= secSTPtr->trsGapSI;
   secSTPtr->trsGapSI ^= firstSTPtr->trsGapSI;
   firstSTPtr->trsGapSI ^= secSTPtr->trsGapSI;

   firstSTPtr->printLinBl ^= secSTPtr->printLinBl;
   secSTPtr->printLinBl ^= firstSTPtr->printLinBl;
   firstSTPtr->printLinBl ^= secSTPtr->printLinBl;


   swapStr = firstSTPtr->lineageStr;
   firstSTPtr->lineageStr = secSTPtr->lineageStr;
   secSTPtr->lineageStr = swapStr;

   firstSTPtr->trsLineageSI ^= secSTPtr->trsLineageSI;
   secSTPtr->trsLineageSI ^= firstSTPtr->trsLineageSI;
   firstSTPtr->trsLineageSI ^= secSTPtr->trsLineageSI;
} /*swap_one_linST*/

/*-------------------------------------------------------\
| Fun07: sortPosAry_one_linST
|   - sorts a one_linST struct array by start position
| Input:
|   - linSTPtr:
|     o one_linST struct array to sort
|   - lenSI:
|     o number of items in the array (index 1)
| Output:
|   - Modifies:
|     o linSTPtr to be sorted by starting position
\-------------------------------------------------------*/
void
sortPosAry_one_linST(
   struct one_linST *linSTPtr,
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - sorts a one_linST struct array by start position
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find the number of rounds to sort for
   '   o fun07 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int curSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   --lenSI;

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         curSI = indexSI;

         for(
            curSI = indexSI;
            curSI + subSI <= lenSI;
            curSI += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = curSI + subSI;

            if(
                 linSTPtr[curSI].startSI
               < linSTPtr[nextSI].startSI
            ) ;

            else if(
                     linSTPtr[curSI].startSI
                  == linSTPtr[nextSI].startSI
               &&
                     linSTPtr[curSI].endSI
                  < linSTPtr[nextSI].endSI
            ) ;

            else
            { /*Else: I need to swap an element*/
               swap_one_linST(
                  &linSTPtr[curSI],
                  &linSTPtr[nextSI]
               );

               lastSI = curSI;
               onSI = curSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(
                       linSTPtr[onSI].startSI
                     > linSTPtr[lastSI].startSI
                  ) break;

                  else if(
                           linSTPtr[onSI].startSI
                        == linSTPtr[lastSI].startSI
                     &&
                           linSTPtr[curSI].endSI
                        > linSTPtr[nextSI].endSI
                  ) break;
   
                  swap_one_linST(
                     &linSTPtr[onSI],
                     &linSTPtr[lastSI]
                  );

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*Else: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortPosAry_one_linST*/

/*-------------------------------------------------------\
| Fun08: sortIdAry_one_linST
|   - sorts a one_linST struct array by variant id
| Input:
|   - linSTPtr:
|     o one_linST struct array to sort
|   - indexArySI:
|     o singed int array with 0 to lenSI - 1
|     o is kept in sync with linSTPtr so that the acutal
|       index's are kept
|   - lenSI:
|     o number of items in the array (index 1)
| Output:
|   - Modifies:
|     o linSTPtr to be sorted by variant id
|     o indexArySI to be kept in sync with linSTPtr
\-------------------------------------------------------*/
void
sortIdAry_one_linST(
   struct one_linST *linSTPtr,
   signed int *indexArySI,    /*index's to keep track of*/
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - sorts a one_linST struct array by variant id
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - find the number of rounds to sort for
   '   o fun08 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int curSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   --lenSI;

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         curSI = indexSI;

         for(
            curSI = indexSI;
            curSI + subSI <= lenSI;
            curSI += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = curSI + subSI;

            if(
                 eqlNull_ulCp(
                    linSTPtr[curSI].idStr,
                    linSTPtr[nextSI].idStr
                 ) > 0 
            ){ /*If: I need to swap an element*/
               swap_one_linST(
                  &linSTPtr[curSI],
                  &linSTPtr[nextSI]
               );

               indexArySI[curSI] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[curSI];
               indexArySI[curSI] ^= indexArySI[nextSI];

               lastSI = curSI;
               onSI = curSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(
                     eqlNull_ulCp(
                        linSTPtr[onSI].idStr,
                        linSTPtr[lastSI].idStr
                     ) > 0 
                  ) break;

                  swap_one_linST(
                     &linSTPtr[onSI],
                     &linSTPtr[lastSI]
                  );

                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            }  /*If: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortIdAry_one_linST*/

/*-------------------------------------------------------\
| Fun09: sortGroupAry_one_linST
|   - sorts a one_linST struct array by variant group and
|     variants in a group by variant id
| Input:
|   - linSTPtr:
|     o one_linST struct array to sort
|   - indexArySI:
|     o singed int array with 0 to lenSI - 1
|     o is kept in sync with linSTPtr so that the acutal
|       index's are kept
|   - lenSI:
|     o number of items in the array (index 1)
| Output:
|   - Modifies:
|     o linSTPtr to be sorted by variant id
|     o indexArySI to be kept in sync with linSTPtr
\-------------------------------------------------------*/
void
sortGroupAry_one_linST(
   struct one_linST *linSTPtr,
   signed int *indexArySI,    /*index's to keep track of*/
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - sorts a one_linST struct array by variant group
   '     and variants in a group by variant id
   '   o fun09 sec01:
   '     - variable declerations
   '   o fun09 sec02:
   '     - find the number of rounds to sort for
   '   o fun09 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int curSI = 0;

   signed long cmpSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   --lenSI;

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         curSI = indexSI;

         for(
            curSI = indexSI;
            curSI + subSI <= lenSI;
            curSI += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = curSI + subSI;

            cmpSL = 
               eqlNull_ulCp(
                  linSTPtr[curSI].groupStr,
                  linSTPtr[nextSI].groupStr
               );
            if(! cmpSL)
               cmpSL = 
                  eqlNull_ulCp(
                     linSTPtr[curSI].idStr,
                     linSTPtr[nextSI].idStr
                  );
            if(cmpSL > 0)
            { /*If: I need to swap an element*/
               swap_one_linST(
                  &linSTPtr[curSI],
                  &linSTPtr[nextSI]
               );

               indexArySI[curSI] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[curSI];
               indexArySI[curSI] ^= indexArySI[nextSI];

               lastSI = curSI;
               onSI = curSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  cmpSL = 
                     eqlNull_ulCp(
                        linSTPtr[onSI].groupStr,
                        linSTPtr[lastSI].groupStr
                     );
                  if(! cmpSL)
                     cmpSL = 
                        eqlNull_ulCp(
                           linSTPtr[onSI].idStr,
                           linSTPtr[lastSI].idStr
                        );
                  if(cmpSL > 0)
                     break;

                  swap_one_linST(
                     &linSTPtr[onSI],
                     &linSTPtr[lastSI]
                  );

                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            }  /*If: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortGroupAry_one_linST*/

/*-------------------------------------------------------\
| Fun10: unsort_one_linST
|   - unsorts a one_linST array using an array of original
|     index's
| Input:
|   - linSTPtr:
|     o one_linST struct array to unsort
|   - indexArySI:
|     o singed int array with original index's to unsort
|       to
|   - lenSI:
|     o number of items in the array (index 1)
| Output:
|   - Modifies:
|     o linSTPtr to be in unsorted order
|     o indexArySI to be 0 to lenSI - 1
\-------------------------------------------------------*/
void
unsort_one_linST(
   struct one_linST *linSTPtr, /*to unsort*/
   signed int *indexArySI,     /*has unsorted index's*/
   signed int lenSI
){
   signed int posSI = 0;
   signed int swapSI = 0;

   while(posSI < lenSI)
   { /*Loop: unsort the lineages*/
      if(indexArySI[posSI] == posSI)
         ++posSI; /*item is at the correct position*/
      else
      { /*Else: put current index in its right position*/
         swapSI = indexArySI[posSI];
         swap_one_linST(
            &linSTPtr[posSI],
            &linSTPtr[swapSI]
         );

         indexArySI[posSI] ^= indexArySI[swapSI];
         indexArySI[swapSI] ^= indexArySI[posSI];
         indexArySI[posSI] ^= indexArySI[swapSI];
      } /*Else: put current index in its right position*/
   } /*Loop: unsort the lineages*/
} /*unsort_one_linST*/

/*-------------------------------------------------------\
| Fun11: posFindAry_one_linST
|   - find the first variant that is between the start and
|     end position
| Input:
|   - startSI:
|     o start of range to find
|   - linAryST:
|     o one_linST struct array to find variant
|   - lenSI:
|     o number of variants in linAryST
| Output:
|   - Returns:
|     o index of first possible variant
|     o -1 if no variant is after the start position
\-------------------------------------------------------*/
signed int
posFindAry_one_linST(
   signed int startSI, /*first base variant can have*/
   struct one_linST *linAryST, /*array of lineages*/
   signed int lenSI    /*number of variants in array*/
){
   signed int midSI = 0;
   signed int rightSI = lenSI - 1;
   signed int leftSI = 0;

   while(leftSI <= rightSI)
   { /*Loop: find the first variant in range*/
      midSI = (leftSI + rightSI) >> 1;

     if(startSI > linAryST[midSI].startSI)
         leftSI = midSI + 1;
     else if(startSI < linAryST[midSI].startSI)
         rightSI = midSI - 1;
     else
     { /*Else: is in range*/
         if(! midSI)
            return midSI;
         else if(startSI < linAryST[midSI - 1].startSI)
            return midSI; /*found end of range*/
         else
            rightSI = midSI - 1;
     } /*Else: is in range*/
   } /*Loop: find the first variant in range*/

   /*check if found range*/
   if(
         midSI == lenSI - 1
      && startSI > linAryST[midSI].startSI
   ) return -1;

   return midSI;
} /*posFindAry_one_linST*/

/*-------------------------------------------------------\
| Fun12: idFind_one_linST
|   - find the variant with the input id
| Input:
|   - idStr:
|     o c-string with the id of the variant to find
|   - linAryST:
|     o one_linST struct array to find variant
|   - lenSI:
|     o number of variants in linAryST
| Output:
|   - Returns:
|     o index of variant with the id in idStr
|     o -1 if no variant is in the input range
\-------------------------------------------------------*/
signed int
idFind_one_linST(
   signed char *idStr,         /*variant to find*/
   struct one_linST *linAryST, /*array of lineages*/
   signed int lenSI        /*number of variants in array*/
){
   signed int midSI = 0;
   signed int rightSI = lenSI - 1;
   signed int leftSI = 0;
   signed long cmpSL = 0;

   while(leftSI <= rightSI)
   { /*Loop: find the first variant in range*/
      midSI = (leftSI + rightSI) >> 1;

     cmpSL = eqlNull_ulCp(idStr, linAryST[midSI].idStr);

     if(cmpSL > 0)
         leftSI = midSI + 1;
     else if(cmpSL < 0)
         rightSI = midSI - 1;
     else
        return midSI;
   } /*Loop: find the first variant in range*/

   return -1;
} /*idFind_one_linST*/

/*-------------------------------------------------------\
| Fun13: groupFindAry_one_linST
|   - find the first variant that is in the group
| Input:
|   - groupStr:
|     o c-string with group to find
|   - linAryST:
|     o one_linST struct array to find variant
|   - lenSI:
|     o number of variants in linAryST
| Output:
|   - Returns:
|     o index of first variant in group in groupStr
|     o -1 if no variant is in the input range
\-------------------------------------------------------*/
signed int
groupFindAry_one_linST(
   signed char *groupStr,      /*group to find*/
   struct one_linST *linAryST, /*array of lineages*/
   signed int lenSI        /*number of variants in array*/
){
   signed int midSI = 0;
   signed int rightSI = lenSI - 1;
   signed int leftSI = 0;
   signed long cmpSL = 0;

   while(leftSI <= rightSI)
   { /*Loop: find the first variant in range*/
      midSI = (leftSI + rightSI) >> 1;

     cmpSL =
        eqlNull_ulCp(groupStr, linAryST[midSI].groupStr);

     if(cmpSL > 0)
         leftSI = midSI + 1;
     else if(cmpSL < 0)
         rightSI = midSI - 1;
     else
     { /*Else: found the group*/
         if(! midSI)
            return midSI;

         cmpSL =
           eqlNull_ulCp(
              groupStr,
              linAryST[midSI - 1].groupStr
           );
         if(cmpSL < 0)
            return midSI; /*found end of range*/
         else
            rightSI = midSI - 1;
     } /*Else: found the group*/
   } /*Loop: find the first variant in range*/

   cmpSL =
     eqlNull_ulCp(groupStr, linAryST[midSI].groupStr);

   /*check if found range*/
   if(! cmpSL)
      return midSI;
   else
      return -1;
} /*groupFindAry_one_linST*/

/*-------------------------------------------------------\
| Fun14: blank_simple_linST
|   - does a lazy blank and sets all lengths in a
|     simple_linST struct to 0
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to blank
| Output:
|   - Modifies:
|     o lenSI in simpleSTPtr to be 0
|     o groupLenSI in simpleSTPtr to be 0
\-------------------------------------------------------*/
void
blank_simple_linST(
   struct simple_linST *simpleSTPtr
){
   if(! simpleSTPtr)
      return;

   simpleSTPtr->lenSI = 0;
   simpleSTPtr->groupLenSI = 0;
} /*blank_simple_linST*/

/*-------------------------------------------------------\
| Fun15: init_simple_linST
|   - sets all array addresses in simple_linST to 0/null
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to initialize
| Output:
|   - Modifies:
|     o all variables to be 0/null
\-------------------------------------------------------*/
void
init_simple_linST(
   struct simple_linST *simpleSTPtr
){
   if(! simpleSTPtr)
      return;

   simpleSTPtr->linAryST = 0;
   simpleSTPtr->sizeSI = 0;
   simpleSTPtr->groupArySI = 0;
   simpleSTPtr->foundGroupAryBl = 0;
   simpleSTPtr->defGroupArySI = 0;
   blank_simple_linST(simpleSTPtr);
} /*init_simple_linST*/

/*-------------------------------------------------------\
| Fun16: freeStack_simple_linST
|   - frees all arrays in a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees all variables and initializes
\-------------------------------------------------------*/
void
freeStack_simple_linST(
   struct simple_linST *simpleSTPtr
){
   if(! simpleSTPtr)
      return;

   if(simpleSTPtr->groupArySI)
      free(simpleSTPtr->groupArySI);
   if(simpleSTPtr->foundGroupAryBl)
      free(simpleSTPtr->foundGroupAryBl);
   if(simpleSTPtr->defGroupArySI)
      free(simpleSTPtr->defGroupArySI);

   if(simpleSTPtr->linAryST)
      freeHeapAry_one_linST(
         simpleSTPtr->linAryST,
         simpleSTPtr->sizeSI
      );

   init_simple_linST(simpleSTPtr);
} /*freeStack_simple_linST*/

/*-------------------------------------------------------\
| Fun17: freeHeap_simple_linST
|   - frees a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to free
| Output:
|   - Modifies:
|     o frees simpleSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_simple_linST(
   struct simple_linST *simpleSTPtr
){
   if(! simpleSTPtr)
      return;

   freeStack_simple_linST(simpleSTPtr);
   free(simpleSTPtr);
} /*freeHeap_simple_linST*/

/*-------------------------------------------------------\
| Fun18: addMem_simple_linST
|   - adds memory to a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to add memory to
|   - sizeSI:
|     o new size of arrays in simpleSTPtr
| Output:
|   - Modifies:
|     o linAryST and groupArySI arrays in simpleSTPtr to
|       be the size of sizeSI if sizeSI is larger then
|       sizeSI in simpleSTPtr
|       * uses malloc if no arrays
|       * uses reallic if already  have arrays
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addMem_simple_linST(
   struct simple_linST *simpleSTPtr,
   signed int sizeSI
){
   signed int siPos = 0;
   struct one_linST *swapLinPtr = 0;
   signed int *swapSIPtr = 0;

   if(! simpleSTPtr)
      goto ret_fun18;
   if(sizeSI < simpleSTPtr->sizeSI)
      goto ret_fun18;

   if(! simpleSTPtr->linAryST)
   { /*If: need to setup initial memory*/
      simpleSTPtr->groupArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! simpleSTPtr->groupArySI)
         goto memErr_fun18;

      simpleSTPtr->linAryST =
         malloc(sizeSI * sizeof(struct one_linST));
      if(! simpleSTPtr->linAryST)
         goto memErr_fun18;

      simpleSTPtr->lenSI = 0;
      simpleSTPtr->sizeSI = 0;
   } /*If: need to setup initial memory*/

   else
   { /*Else: need to add more memory*/
      swapSIPtr =
         realloc(
            simpleSTPtr->groupArySI,
            sizeSI * sizeof (signed int)
         );
      if(! swapSIPtr)
         goto memErr_fun18;
      simpleSTPtr->groupArySI = swapSIPtr;

      swapLinPtr =
         realloc(
            simpleSTPtr->linAryST,
            sizeSI * sizeof (struct one_linST)
         );
      if(! swapLinPtr)
         goto memErr_fun18;
      simpleSTPtr->linAryST = swapLinPtr;
   } /*Else: need to add more memory*/

   for(
      siPos = simpleSTPtr->sizeSI;
      siPos < sizeSI;
      ++siPos
   ){ /*Loop: initialze values*/
      init_one_linST(&simpleSTPtr->linAryST[siPos]);
      simpleSTPtr->groupArySI[siPos] = -1;
   } /*Loop: initialze values*/

   simpleSTPtr->sizeSI = sizeSI;

   ret_fun18:;
      return 0;
   memErr_fun18:;
      return 1;
} /*addMem_simple_linST*/

/*-------------------------------------------------------\
| Fun19: clearGroup_simple_linST
|   - clears all detected groups in a simple_linST struct
| Input
|   - simpleSTPtr:
|     o simple_linST struct array to clear found groups
| Output:
|   - Modifies:
|     o foundGroupAryBl in simpleSTPtr to be filled with
|       -1's (no lineage supported)
\-------------------------------------------------------*/
void
clearGroup_simple_linST(
   struct simple_linST *simpleSTPtr
){
   signed int siPos = 0;

   if(! simpleSTPtr)
      return;
   for(siPos=0; siPos < simpleSTPtr->groupLenSI; ++siPos)
      simpleSTPtr->foundGroupAryBl[siPos] = -1;
} /*clearGroup_simple_linST*/

/*-------------------------------------------------------\
| Fun20: unsort_simple_linST
|   - unsorts a simple_linST struct using an array of
|     original index's
| Input:
|   - simpleSTPtr:
|     o simple_linST struct array to unsort
|   - indexArySI:
|     o singed int array with original index's to unsort
|       to
| Output:
|   - Modifies:
|     o linSTPtr to be in unsorted order
|     o indexArySI to be 0 to lenSI - 1
\-------------------------------------------------------*/
void
unsort_simple_linST(
   struct simple_linST *simpleSTPtr,
   signed int *indexArySI
){
   signed int posSI = 0;
   signed int swapSI = 0;
   struct one_linST *linSTPtr = simpleSTPtr->linAryST;

   while(posSI < simpleSTPtr->lenSI)
   { /*Loop: unsort the lineages*/
      if(indexArySI[posSI] == posSI)
         ++posSI; /*item is at the correct position*/
      else
      { /*Else: put current index in its right position*/
         swapSI = indexArySI[posSI];
         swap_one_linST(
            &linSTPtr[posSI],
            &linSTPtr[swapSI]
         );

         simpleSTPtr->groupArySI[posSI] ^=
            simpleSTPtr->groupArySI[swapSI];
         simpleSTPtr->groupArySI[swapSI] ^=
            simpleSTPtr->groupArySI[posSI];
         simpleSTPtr->groupArySI[posSI] ^=
            simpleSTPtr->groupArySI[swapSI];

         indexArySI[posSI] ^= indexArySI[swapSI];
         indexArySI[swapSI] ^= indexArySI[posSI];
         indexArySI[posSI] ^= indexArySI[swapSI];
      } /*Else: put current index in its right position*/
   } /*Loop: unsort the lineages*/
} /*unsort_simple_linST*/

/*-------------------------------------------------------\
| Fun21: blank_multi_linST
|   - blanks (sets to defaults) a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to blank
| Output:
|   - Modifies:
|     o all non-array variables in mLinSTPtr to be 0/null
|     o all arrays to be set to 0/null
\-------------------------------------------------------*/
void
blank_multi_linST(
   struct multi_linST *mLinSTPtr
){
   signed int lenSI = 0;

   if(! mLinSTPtr)
      return;

   if(mLinSTPtr->idStr)
      mLinSTPtr->idStr[0] = 0;
   if(mLinSTPtr->groupIdStr)
      mLinSTPtr->groupIdStr[0] = 0;
   if(mLinSTPtr->geneStr)
      mLinSTPtr->geneStr[0] = 0;
   if(mLinSTPtr->lineageStr)
      mLinSTPtr->lineageStr[0] = 0;

   if(mLinSTPtr->linIndexArySI)
   { /*If: have variants in a lineage*/
      for(lenSI = 0; lenSI < mLinSTPtr->linLenSI; ++lenSI)
      { /*Loop: set variant lineages to no lineage*/
         mLinSTPtr->linIndexArySI[lenSI] = -1;
         mLinSTPtr->linNeedAryBl[lenSI] = 0;
         mLinSTPtr->linTrsArySI[lenSI] = -1024;
         mLinSTPtr->defGroupArySI[lenSI] = -1;
      } /*Loop: set variant lineages to no lineage*/
   } /*If: have variants in a lineage*/

   if(mLinSTPtr->mLinIndexArySI)
   { /*If: have variants in a lineage*/
      for(lenSI=0; lenSI < mLinSTPtr->mLinLenSI; ++lenSI)
      { /*Loop: blank the multi lineages*/
         mLinSTPtr->mLinIndexArySI[lenSI] = 0;
         mLinSTPtr->mLinNeedAryBl[lenSI] = 0;
      } /*Loop: blank the multi lineages*/
   } /*If: have variants in a lineage*/

   mLinSTPtr->defGroupLenSI = 0;
   mLinSTPtr->typeSC = def_matchComplexType_linST;
   mLinSTPtr->overwriteBl = 0;
   mLinSTPtr->printLinBl = 0;
   mLinSTPtr->fudgeSI = 0;
   mLinSTPtr->startSI = 0;
   mLinSTPtr->endSI = 0;
   mLinSTPtr->totalOneLinSI = 0;
   mLinSTPtr->totalLinSI = 0;
} /*blank_multi_linST*/

/*-------------------------------------------------------\
| Fun22: init_multi_linST
|   - initializes a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to initialize
| Output:
|   - Modifies:
|     o all arrays and variables to be set to 0/null
\-------------------------------------------------------*/
void
init_multi_linST(
   struct multi_linST *mLinSTPtr
){
   if(! mLinSTPtr)
      return;

   mLinSTPtr->idStr = 0;
   mLinSTPtr->groupIdStr = 0;
   mLinSTPtr->geneStr = 0;
   mLinSTPtr->lineageStr = 0;

   mLinSTPtr->linIndexArySI = 0;
   mLinSTPtr->linTrsArySI = 0;
   mLinSTPtr->linNeedAryBl = 0;
   mLinSTPtr->linNotAryBl = 0;
   mLinSTPtr->defGroupArySI = 0;
   mLinSTPtr->linLenSI = 0;
   mLinSTPtr->mLinIndexArySI = 0;
   mLinSTPtr->mLinNeedAryBl = 0;
   mLinSTPtr->mLinNotAryBl = 0;
   mLinSTPtr->mLinLenSI = 0;

   blank_multi_linST(mLinSTPtr);
} /*init_multi_linST*/

/*-------------------------------------------------------\
| Fun23: freeStack_multi_linST
|   - frees variables in a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees the arrays and initializes
| Note:
|   - does not free individual pointers in the varArySTPtr
|     or multiArySTPtr arrays
\-------------------------------------------------------*/
void
freeStack_multi_linST(
   struct multi_linST *mLinSTPtr
){
   if(! mLinSTPtr)
      return;

   if(mLinSTPtr->idStr)
      free(mLinSTPtr->idStr);
   if(mLinSTPtr->groupIdStr)
      free(mLinSTPtr->groupIdStr);
   if(mLinSTPtr->geneStr)
      free(mLinSTPtr->geneStr);
   if(mLinSTPtr->lineageStr)
      free(mLinSTPtr->lineageStr);


   /*the varArySTPtr and multiArySTPtr are pointer arrays
   `  that should be a second pointer, so they will cause
   `  a memory error if freeded
   */
   if(mLinSTPtr->linIndexArySI)
      free(mLinSTPtr->linIndexArySI);
   if(mLinSTPtr->linNeedAryBl)
      free(mLinSTPtr->linNeedAryBl);
   if(mLinSTPtr->linNotAryBl)
      free(mLinSTPtr->linNotAryBl);
   if(mLinSTPtr->linTrsArySI)
      free(mLinSTPtr->linTrsArySI);
   if(mLinSTPtr->defGroupArySI)
      free(mLinSTPtr->defGroupArySI);

   if(mLinSTPtr->mLinIndexArySI)
      free(mLinSTPtr->mLinIndexArySI);
   if(mLinSTPtr->mLinNeedAryBl)
      free(mLinSTPtr->mLinNeedAryBl);
   if(mLinSTPtr->mLinNotAryBl)
      free(mLinSTPtr->mLinNotAryBl);

   init_multi_linST(mLinSTPtr);
} /*freeStack_multi_linST*/

/*-------------------------------------------------------\
| Fun24: freeHeap_multi_linST
|   - frees a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to free
| Output:
|   - Modifies:
|     o frees the mLinSTPtr structure and its variables
| Note:
|   - does not free the varArySTPtr or multiArySTPtr
|     arrays
\-------------------------------------------------------*/
void
freeHeap_multi_linST(
   struct multi_linST *mLinSTPtr
){
   if(! mLinSTPtr)
      return;
   freeStack_multi_linST(mLinSTPtr);
   free(mLinSTPtr);
} /*freeHeap_multi_linST*/

/*-------------------------------------------------------\
| Fun25: freeHeapAry_multi_linST
|   - frees a multi_linST struct array
| Input:
|   - mLinAryST:
|     o multi_linST structure array to free
|   - lenSI:
|     o number of multi_linST structures in the array
| Output:
|   - Modifies:
|     o frees mLinAryST, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeapAry_multi_linST(
   struct multi_linST *mLinAryST,
   signed int lenSI
){
   if(! mLinAryST)
     return;

   while(--lenSI >= 0)
      freeStack_multi_linST(&mLinAryST[lenSI]);
   free(mLinAryST);
} /*freeHeapAry_multi_linST*/

/*-------------------------------------------------------\
| Fun26: blank_complex_linST
|   - sets length in a complex_linST to 0 (lazy blank)
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to blank
| Output:
|   - Modifies:
|     o lenSI in complexSTPtr to be 0 (lazy blank)
\-------------------------------------------------------*/
void
blank_complex_linST(
   struct complex_linST *complexSTPtr
){
   if(! complexSTPtr)
      return;
   complexSTPtr->lenSI = 0;
} /*blank_complex_linST*/

/*-------------------------------------------------------\
| Fun27: init_complex_linST
|   - sets all values complex_linST to 0/null
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to initialize
| Output:
|   - Modifies:
|     o all values/arrays in complexSTPtr to be 0/null
\-------------------------------------------------------*/
void
init_complex_linST(
   struct complex_linST *complexSTPtr
){
   if(! complexSTPtr)
      return;

   complexSTPtr->linAryST = 0;
   complexSTPtr->sizeSI = 0;
   complexSTPtr->groupArySI = 0;
   complexSTPtr->cntArySI = 0;

   blank_complex_linST(complexSTPtr);
} /*init_complex_linST*/

/*-------------------------------------------------------\
| Fun28: freeStack_complex_linST
|   - frees all variabls in a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees all values/arrays in complexSTPtr and then
|       initializes
\-------------------------------------------------------*/
void
freeStack_complex_linST(
   struct complex_linST *complexSTPtr
){
   if(! complexSTPtr)
      return;

   if(complexSTPtr->groupArySI)
      free(complexSTPtr->groupArySI);
   if(complexSTPtr->cntArySI)
      free(complexSTPtr->cntArySI);

   if(complexSTPtr->linAryST)
      freeHeapAry_multi_linST(
         complexSTPtr->linAryST,
         complexSTPtr->sizeSI
      );
   init_complex_linST(complexSTPtr);
} /*freeStack_complex_linST*/

/*-------------------------------------------------------\
| Fun29: freeHeap_complex_linST
|   - frees a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to free
| Output:
|   - Modifies:
|     o frees complexSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_complex_linST(
   struct complex_linST *complexSTPtr
){
   if(! complexSTPtr)
      return;
   freeStack_complex_linST(complexSTPtr);
   free(complexSTPtr);
} /*freeHeap_complex_linST*/

/*-------------------------------------------------------\
| Fun30: addMem_complex_linST
|   - adds memory to a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to expand lineage
|       array
|   - sizeSI:
|     o size to resize to
| Output:
|   - Modifies:
|     o if linAryST in complexSTPtr is 0/null uses malloc
|     o if sizeSI is greater then sizeSI in complexSTPtr
|       used realloc to resize
|     o initializes all new array elements
|   - Returns:
|     o 0 for no errors
|     o 1 for memory erros
\-------------------------------------------------------*/
signed char
addMem_complex_linST(
   struct complex_linST *complexSTPtr,
   signed int sizeSI
){
   signed int siPos = 0;
   struct multi_linST *swapPtr = 0;

   if(! complexSTPtr)
      goto noErr_fun29;
   if(sizeSI < complexSTPtr->sizeSI)
      goto noErr_fun29;

   if(! complexSTPtr->linAryST)
   { /*If: need initial memory*/
      complexSTPtr->linAryST =
         malloc(sizeSI * sizeof(struct multi_linST));
      if(! complexSTPtr->linAryST)
         goto err_fun29;

      complexSTPtr->sizeSI = 0;
      complexSTPtr->lenSI = 0;
   } /*If: need initial memory*/

   else
   { /*Else: increasing memory*/
      swapPtr =
         realloc(
            complexSTPtr->linAryST,
            sizeSI * sizeof(struct multi_linST)
         );
      if(! swapPtr)
         goto err_fun29;
      complexSTPtr->linAryST = swapPtr;
   } /*Else: increasing memory*/

   for(
      siPos = complexSTPtr->sizeSI;
      siPos < sizeSI;
      ++siPos
   ) init_multi_linST(&complexSTPtr->linAryST[siPos]);

   complexSTPtr->sizeSI = sizeSI;

   noErr_fun29:;
      return 0;
   err_fun29:;
      return 1;
} /*addMem_complex_linST*/

/*-------------------------------------------------------\
| Fun31: swap_complex_linST
|   - swap two index in a complex lineage array
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to swap lineages in
|   - oneSI:
|     o frist index to swap
|   - twoSI:
|     o second index to swap
| Output:
|   - Modifies:
|     o complexSTPtr to have index's at oneSI and twoSI
|       swapped
\-------------------------------------------------------*/
void
swap_complex_linST(
   struct complex_linST *complexSTPtr,
   signed int oneSI,
   signed int twoSI
){
   signed char *swapStrPtr = 0;
   signed int *swapSIPtr = 0;

   swapStrPtr = complexSTPtr->linAryST[oneSI].idStr;
   complexSTPtr->linAryST[oneSI].idStr =
      complexSTPtr->linAryST[twoSI].idStr;
   complexSTPtr->linAryST[twoSI].idStr = swapStrPtr;

   swapStrPtr = complexSTPtr->linAryST[oneSI].groupIdStr;
   complexSTPtr->linAryST[oneSI].groupIdStr =
      complexSTPtr->linAryST[twoSI].groupIdStr;
   complexSTPtr->linAryST[twoSI].groupIdStr = swapStrPtr;

   swapStrPtr = complexSTPtr->linAryST[oneSI].geneStr;
   complexSTPtr->linAryST[oneSI].geneStr =
      complexSTPtr->linAryST[twoSI].geneStr;
   complexSTPtr->linAryST[twoSI].geneStr = swapStrPtr;

   swapStrPtr = complexSTPtr->linAryST[oneSI].lineageStr;
   complexSTPtr->linAryST[oneSI].lineageStr =
      complexSTPtr->linAryST[twoSI].lineageStr;
   complexSTPtr->linAryST[twoSI].lineageStr = swapStrPtr;

   complexSTPtr->linAryST[oneSI].typeSC ^=
      complexSTPtr->linAryST[twoSI].typeSC;
   complexSTPtr->linAryST[twoSI].typeSC ^=
      complexSTPtr->linAryST[oneSI].typeSC;
   complexSTPtr->linAryST[oneSI].typeSC ^=
      complexSTPtr->linAryST[twoSI].typeSC;

   complexSTPtr->linAryST[oneSI].fudgeSI ^=
      complexSTPtr->linAryST[twoSI].fudgeSI;
   complexSTPtr->linAryST[twoSI].fudgeSI ^=
      complexSTPtr->linAryST[oneSI].fudgeSI;
   complexSTPtr->linAryST[oneSI].fudgeSI ^=
      complexSTPtr->linAryST[twoSI].fudgeSI;

   complexSTPtr->linAryST[oneSI].overwriteBl ^=
      complexSTPtr->linAryST[twoSI].overwriteBl;
   complexSTPtr->linAryST[twoSI].overwriteBl ^=
      complexSTPtr->linAryST[oneSI].overwriteBl;
   complexSTPtr->linAryST[oneSI].overwriteBl ^=
      complexSTPtr->linAryST[twoSI].overwriteBl;

   complexSTPtr->linAryST[oneSI].printLinBl ^=
      complexSTPtr->linAryST[twoSI].printLinBl;
   complexSTPtr->linAryST[twoSI].printLinBl ^=
      complexSTPtr->linAryST[oneSI].printLinBl;
   complexSTPtr->linAryST[oneSI].printLinBl ^=
      complexSTPtr->linAryST[twoSI].printLinBl;

   swapSIPtr=complexSTPtr->linAryST[oneSI].linIndexArySI;
   complexSTPtr->linAryST[oneSI].linIndexArySI =
      complexSTPtr->linAryST[twoSI].linIndexArySI;
   complexSTPtr->linAryST[twoSI].linIndexArySI=swapSIPtr;

   swapSIPtr = complexSTPtr->linAryST[oneSI].linTrsArySI;
   complexSTPtr->linAryST[oneSI].linTrsArySI =
      complexSTPtr->linAryST[twoSI].linTrsArySI;
   complexSTPtr->linAryST[twoSI].linTrsArySI = swapSIPtr;

   swapStrPtr =complexSTPtr->linAryST[oneSI].linNeedAryBl;
   complexSTPtr->linAryST[oneSI].linNeedAryBl =
      complexSTPtr->linAryST[twoSI].linNeedAryBl;
   complexSTPtr->linAryST[twoSI].linNeedAryBl =swapStrPtr;

   swapStrPtr = complexSTPtr->linAryST[oneSI].linNotAryBl;
   complexSTPtr->linAryST[oneSI].linNotAryBl =
      complexSTPtr->linAryST[twoSI].linNotAryBl;
   complexSTPtr->linAryST[twoSI].linNotAryBl = swapStrPtr;

   complexSTPtr->linAryST[oneSI].linLenSI ^=
      complexSTPtr->linAryST[twoSI].linLenSI;
   complexSTPtr->linAryST[twoSI].linLenSI ^=
      complexSTPtr->linAryST[oneSI].linLenSI;
   complexSTPtr->linAryST[oneSI].linLenSI ^=
      complexSTPtr->linAryST[twoSI].linLenSI;

   swapSIPtr=complexSTPtr->linAryST[oneSI].defGroupArySI;
   complexSTPtr->linAryST[oneSI].defGroupArySI =
      complexSTPtr->linAryST[twoSI].defGroupArySI;
   complexSTPtr->linAryST[twoSI].defGroupArySI=swapSIPtr;

   complexSTPtr->linAryST[oneSI].defGroupLenSI ^=
      complexSTPtr->linAryST[twoSI].defGroupLenSI;
   complexSTPtr->linAryST[twoSI].defGroupLenSI ^=
      complexSTPtr->linAryST[oneSI].defGroupLenSI;
   complexSTPtr->linAryST[oneSI].defGroupLenSI ^=
      complexSTPtr->linAryST[twoSI].defGroupLenSI;

   swapSIPtr=complexSTPtr->linAryST[oneSI].mLinIndexArySI;
   complexSTPtr->linAryST[oneSI].mLinIndexArySI =
      complexSTPtr->linAryST[twoSI].mLinIndexArySI;
   complexSTPtr->linAryST[twoSI].mLinIndexArySI=swapSIPtr;

   swapStrPtr =
      complexSTPtr->linAryST[oneSI].mLinNeedAryBl;
   complexSTPtr->linAryST[oneSI].mLinNeedAryBl =
      complexSTPtr->linAryST[twoSI].mLinNeedAryBl;
   complexSTPtr->linAryST[twoSI].mLinNeedAryBl =
      swapStrPtr;

   swapStrPtr =
      complexSTPtr->linAryST[oneSI].mLinNotAryBl;
   complexSTPtr->linAryST[oneSI].mLinNotAryBl =
      complexSTPtr->linAryST[twoSI].mLinNotAryBl;
   complexSTPtr->linAryST[twoSI].mLinNotAryBl =
      swapStrPtr;

   complexSTPtr->linAryST[oneSI].mLinLenSI ^=
      complexSTPtr->linAryST[twoSI].mLinLenSI;
   complexSTPtr->linAryST[twoSI].mLinLenSI ^=
      complexSTPtr->linAryST[oneSI].mLinLenSI;
   complexSTPtr->linAryST[oneSI].mLinLenSI ^=
      complexSTPtr->linAryST[twoSI].mLinLenSI;

   complexSTPtr->linAryST[oneSI].totalOneLinSI ^=
      complexSTPtr->linAryST[twoSI].totalOneLinSI;
   complexSTPtr->linAryST[twoSI].totalOneLinSI ^=
      complexSTPtr->linAryST[oneSI].totalOneLinSI;
   complexSTPtr->linAryST[oneSI].totalOneLinSI ^=
      complexSTPtr->linAryST[twoSI].totalOneLinSI;

   complexSTPtr->linAryST[oneSI].startSI ^=
      complexSTPtr->linAryST[twoSI].startSI;
   complexSTPtr->linAryST[twoSI].startSI ^=
      complexSTPtr->linAryST[oneSI].startSI;
   complexSTPtr->linAryST[oneSI].startSI ^=
      complexSTPtr->linAryST[twoSI].startSI;

   complexSTPtr->linAryST[oneSI].endSI ^=
      complexSTPtr->linAryST[twoSI].endSI;
   complexSTPtr->linAryST[twoSI].endSI ^=
      complexSTPtr->linAryST[oneSI].endSI;
   complexSTPtr->linAryST[oneSI].endSI ^=
      complexSTPtr->linAryST[twoSI].endSI;
} /*swap_complex_linST*/

/*-------------------------------------------------------\
| Fun32: addIdSort_complex_linST
|   - add complex id to an lineage by sorting
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to add an id to by
|       order
|   - idStr:
|     o c-string with id to insert
|   - indexArySI:
|     o signed int array with indexs to unsort to the
|       original input later
| Output:
|   - Modifies:
|     o linAryST in complexSTPtr to add the id to
|     o indexArySI to get the original order
|   - Returns:
|     o index of inserted id
|     o -1 if id is already inserted
|     o -2 for memory errors
| Warning:
|   - you need to incurment complexSTPtr->lenSI, this
|     function does not do this for you
\-------------------------------------------------------*/
signed int
addIdSort_complex_linST(
   complex_linST *complexSTPtr, /*lineages to add ids to*/
   signed char *idStr,          /*id to insert*/
   signed int *indexArySI       /*original index array*/
){
   signed int midSI = 0;
   signed int leftSI = 0;
   signed int rightSI = complexSTPtr->lenSI - 1;

   signed int lenSI = 0;
   signed char cpIdStr[256];

   signed long cmpSL = 0;

   lenSI = cpWhite_ulCp(cpIdStr, idStr);
     /*do not want white space in the id name*/

   if(rightSI < 0)
      goto addId_fun32; /*first string to add in*/

   while(leftSI <= rightSI)
   { /*Loop: find the insert position*/
      midSI = (rightSI + leftSI) >> 1;
      cmpSL =
         eqlNull_ulCp(
            cpIdStr,
            complexSTPtr->linAryST[midSI].idStr
         );

      if(cmpSL < 0)
         rightSI = midSI - 1; /*id is less then midSI*/
      else if(cmpSL > 0)
         leftSI = midSI + 1; /*id is greater then midSI*/
      else
         return -1;
   } /*Loop: find the insert position*/

   if(cmpSL > 0)
      ++midSI; /*insert point is after found index*/


   for(
      leftSI = complexSTPtr->lenSI;
      leftSI > midSI;
      --leftSI
   ){ /*Loop: move old values up one*/
       swap_complex_linST(complexSTPtr, leftSI, leftSI-1);
       indexArySI[leftSI] ^= indexArySI[leftSI - 1];
       indexArySI[leftSI - 1] ^= indexArySI[leftSI];
       indexArySI[leftSI] ^= indexArySI[leftSI - 1];
   }  /*Loop: move old values up one*/

   addId_fun32:;
      complexSTPtr->linAryST[midSI].idStr =
         malloc((lenSI + 8) * sizeof(signed char));
      if(! complexSTPtr->linAryST[midSI].idStr)
         return -2;
      cpLen_ulCp(
         complexSTPtr->linAryST[midSI].idStr,
         cpIdStr,
         lenSI
      );
      indexArySI[midSI] = complexSTPtr->lenSI;

      return midSI;
} /*addIdSort_complex_linST*/

/*-------------------------------------------------------\
| Fun33: idFind_complex_linST
|   - finds an id in an id sorted complex_linST structure
|   - this is only used in the reading in function
| Input:
|   - idStr:
|     o c-string with id to find
|   - complexSTPtr:
|     o complex_linST struct pointer to add an id to by
|       order
| Output:
|   - Returns:
|     o index of id
|     o -1 if id is not in the array
\-------------------------------------------------------*/
signed int
idFind_complex_linST(
   signed char *idStr,          /*id to find*/
   complex_linST *complexSTPtr  /*lineages to search*/
){
   signed int midSI = 0;
   signed int leftSI = 0;
   signed int rightSI = complexSTPtr->lenSI;
   signed long cmpSL = 0;

   while(leftSI <= rightSI)
   { /*Loop: find the insert position*/
      midSI = (rightSI + leftSI) >> 1;
      cmpSL =
         eqlNull_ulCp(
            idStr,
            complexSTPtr->linAryST[midSI].idStr
         );

      if(cmpSL < 0)
         rightSI = midSI - 1; /*id is less then midSI*/
      else if(cmpSL > 0)
         leftSI = midSI + 1; /*id is greater then midSI*/
      else
         return midSI;
   } /*Loop: find the insert position*/

   return -1;
} /*addIdSort_complex_linST*/

/*-------------------------------------------------------\
| Fun34: groupSort_complex_linST
|   - sorts a mult_linST array in a complex_linST struct
|     by the group id
| Input:
|   - cLinSTPtr:
|     o complex_linST struct pointer to un34rt
|   - indexArySI:
|     o signed int array with original index's
|     o this is kept in sync for unsorting later
| Output:
|   - Modifies:
|     o linAryST in cLinSTPtr to be sorted by group
\-------------------------------------------------------*/
void
groupSort_complex_linST(
   struct complex_linST *cLinSTPtr,
   signed int *indexArySI /*for unsorting later*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun34 TOC:
   '   - sorts a mult_linST array in a complex_linST
   '     struct by the group id
   '   - sorts a one_linST struct array by variant id
   '   o fun34 sec01:
   '     - variable declerations
   '   o fun34 sec02:
   '     - find the number of roun34 to sort for
   '   o fun34 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting roun34*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;
   signed int lenSI = cLinSTPtr->lenSI;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int curSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec02:
   ^   - find the max search value (number roun34 to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   --lenSI;

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all roun34*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         curSI = indexSI;

         for(
            curSI = indexSI;
            curSI + subSI <= lenSI;
            curSI += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = curSI + subSI;

            if(
               eqlNull_ulCp(
                  cLinSTPtr->linAryST[curSI].groupIdStr,
                  cLinSTPtr->linAryST[nextSI].groupIdStr
               ) > 0 
            ){ /*If: I need to swap an element*/
               swap_complex_linST(cLinSTPtr,curSI,nextSI);

               cLinSTPtr->groupArySI[curSI] ^=
                  cLinSTPtr->groupArySI[nextSI];
               cLinSTPtr->groupArySI[nextSI] ^=
                  cLinSTPtr->groupArySI[curSI];
               cLinSTPtr->groupArySI[curSI] ^=
                  cLinSTPtr->groupArySI[nextSI];

               indexArySI[curSI] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[curSI];
               indexArySI[curSI] ^= indexArySI[nextSI];

               lastSI = curSI;
               onSI = curSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(
                   eqlNull_ulCp(
                    cLinSTPtr->linAryST[curSI].groupIdStr,
                    cLinSTPtr->linAryST[nextSI].groupIdStr
                   ) > 0 
                  ) break;

                  swap_complex_linST(
                     cLinSTPtr,
                     lastSI,
                     onSI
                  );

                  cLinSTPtr->groupArySI[onSI] ^=
                     cLinSTPtr->groupArySI[lastSI];
                  cLinSTPtr->groupArySI[lastSI] ^=
                     cLinSTPtr->groupArySI[onSI];
                  cLinSTPtr->groupArySI[onSI] ^=
                     cLinSTPtr->groupArySI[lastSI];

                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            }  /*If: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all roun34*/
} /*groupSort_complex_linST*/

/*-------------------------------------------------------\
| Fun35: positionSort_complex_linST
|   - sorts a mult_linST array in a complex_linST struct
|     by the position (end coordinate)
|   - this sort also makes sure that lineages that have
|     more variants are at the end
|   - between end and total at ends, this ensures complex
|     lineages that are depenent on other complex lineages
|     end up at the end
| Input:
|   - cLinSTPtr:
|     o complex_linST struct pointer to un35rt
|   - indexArySI:
|     o signed int array with original index's
|     o this is kept in sync for unsorting later
| Output:
|   - Modifies:
|     o linAryST in cLinSTPtr to be sorted by group
\-------------------------------------------------------*/
void
positionSort_complex_linST(
   struct complex_linST *cLinSTPtr,
   signed int *indexArySI /*for unsorting later*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun35 TOC:
   '   - sorts a mult_linST array in a complex_linST
   '     struct by the position (end coordinate)
   '   o fun35 sec01:
   '     - variable declerations
   '   o fun35 sec02:
   '     - find the number of roun35 to sort for
   '   o fun35 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun35 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting roun35*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;
   signed int lenSI = cLinSTPtr->lenSI;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int curSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun35 Sec02:
   ^   - find the max search value (number roun35 to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   --lenSI;

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun35 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all roun35*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         curSI = indexSI;

         for(
            curSI = indexSI;
            curSI + subSI <= lenSI;
            curSI += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = curSI + subSI;

            if(
                 cLinSTPtr->linAryST[curSI].endSI
               > cLinSTPtr->linAryST[nextSI].endSI
            ) goto swap_fun35_sec03;

            else if(
                 cLinSTPtr->linAryST[curSI].endSI
               < cLinSTPtr->linAryST[nextSI].endSI
            ) goto swap_fun35_sec03;

            else if(
                 cLinSTPtr->linAryST[curSI].totalLinSI
               > cLinSTPtr->linAryST[nextSI].totalLinSI
            ){ /*Else If: I need to swap an element*/
               swap_fun35_sec03:;

               swap_complex_linST(cLinSTPtr,curSI,nextSI);

               cLinSTPtr->groupArySI[curSI] ^=
                  cLinSTPtr->groupArySI[nextSI];
               cLinSTPtr->groupArySI[nextSI] ^=
                  cLinSTPtr->groupArySI[curSI];
               cLinSTPtr->groupArySI[curSI] ^=
                  cLinSTPtr->groupArySI[nextSI];

               indexArySI[curSI] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[curSI];
               indexArySI[curSI] ^= indexArySI[nextSI];

               lastSI = curSI;
               onSI = curSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(
                       cLinSTPtr->linAryST[onSI].endSI
                     > cLinSTPtr->linAryST[lastSI].endSI
                  ) break;

                  else if(
                       cLinSTPtr->linAryST[onSI].endSI
                     < cLinSTPtr->linAryST[lastSI].endSI
                  ) ;

                  else if(
                    cLinSTPtr->linAryST[onSI].totalLinSI
                   >cLinSTPtr->linAryST[lastSI].totalLinSI
                  ) break;

                  swap_complex_linST(
                     cLinSTPtr,
                     lastSI,
                     onSI
                  );

                  cLinSTPtr->groupArySI[onSI] ^=
                     cLinSTPtr->groupArySI[lastSI];
                  cLinSTPtr->groupArySI[lastSI] ^=
                     cLinSTPtr->groupArySI[onSI];
                  cLinSTPtr->groupArySI[onSI] ^=
                     cLinSTPtr->groupArySI[lastSI];

                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            }  /*If: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*positionSort_complex_linST*/

/*-------------------------------------------------------\
| Fun36: unsort_complex_linST
|   - unsort a mult_linST array in a complex_linST struct
|     by the original index's
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to unsort
|   - indexArySI:
|     o signed int array with index to unsort with
| Output:
|   - Modifies:
|     o linAryST in complexSTPtr to be unsorted
|     o indexArySI to be orded from least to greatest
\-------------------------------------------------------*/
void
unsort_complex_linST(
   complex_linST *complexSTPtr, /*lineages to unsort*/
   signed int *indexArySI       /*use in unsorting*/
){
   signed int siPos = 0;
   signed int swapSI = 0;

   while(siPos < complexSTPtr->lenSI)
   { /*Loop: unsort the array*/
      if(indexArySI[siPos] != siPos)
      { /*If: need to swap*/
         swapSI = indexArySI[siPos];
         swap_complex_linST(
           complexSTPtr,
           siPos,
           indexArySI[siPos]
         );

         indexArySI[siPos] ^= indexArySI[swapSI];
         indexArySI[swapSI] ^= indexArySI[siPos];
         indexArySI[siPos] ^= indexArySI[swapSI];
      } /*If: need to swap*/

      else
         ++siPos;
   } /*Loop: unsort the array*/
} /*unsort_complex_linST*/

/*-------------------------------------------------------\
| Fun37: posFind_complex_linST
|   - find the first complex variant that is between the
|     start and end position in a complex_linST struct
| Input:
|   - endSI:
|     o last base in range
|   - complexSTPtr:
|     o complex_linST struct array to find variant index
| Output:
|   - Returns:
|     o index of first variant in between the start and
|       end
|     o -1 if no variant is in the input range
\-------------------------------------------------------*/
signed int
posFind_complex_linST(
   signed int endSI,   /*last base variant can have*/
   struct complex_linST *complexSTPtr /*complex lineages*/
){
   signed int midSI = 0;
   signed int rightSI = complexSTPtr->lenSI - 1;
   signed int leftSI = 0;

   while(leftSI <= rightSI)
   { /*Loop: find the first variant in range*/
      midSI = (leftSI + rightSI) >> 1;

     if(endSI > complexSTPtr->linAryST[midSI].endSI)
         leftSI = midSI + 1;
     else if(endSI < complexSTPtr->linAryST[midSI].endSI)
         rightSI = midSI - 1;
     else
     { /*Else: is in range*/
         if(! midSI)
            return midSI;

         else if(
            endSI > complexSTPtr->linAryST[midSI -1].endSI
         ) return midSI; /*found end of range*/

         else
            rightSI = midSI - 1;
     } /*Else: is in range*/
   } /*Loop: find the first variant in range*/

   /*check if found range*/
   if(endSI > complexSTPtr->linAryST[midSI].endSI)
      return -1;

   return midSI;
} /*posFind_complex_linST*/

/*-------------------------------------------------------\
| Fun38: flipIndexAry_complex_linST
|   - changes the index array so that the stored index is
|     the position and the old position is the new stored
|     index
| Input:
|   - indexArySI:
|     o signed int array with indexs to flip
|   - lenSI:
|     o number of index in indexArySI
| Output:
|   - Modifies:
|     o indexArySI to have index's as position and old
|       positions as index's
|   - Returns:
|     o 0 for no errors
|     o 1 if have a memory error
\-------------------------------------------------------*/
signed char
flipIndexAry_complex_linST(
   signed int *indexArySI,
   signed int lenSI
){
   signed int siPos = 0;
   signed int *tmpHeapArySI =
      calloc(lenSI, sizeof(signed int));

   if(! tmpHeapArySI)
      return 1;

   /*I need to first flip and then copy to avoid cases
   `  were an index is moved ahead and can build and
   `  infinite loop or is processed again
   */

   /*flip the index array*/
   for(siPos = 0; siPos < lenSI; ++siPos)
      tmpHeapArySI[indexArySI[siPos]] = siPos;

   /*copy the flipped array*/
   for(siPos = 0; siPos < lenSI; ++siPos)
      indexArySI[siPos] = tmpHeapArySI[siPos];

   free(tmpHeapArySI);
   tmpHeapArySI = 0;

   return 0;
} /*flipIndexAry_complex_linST*/

/*-------------------------------------------------------\
| Fun39: getSimpleLineages_linST
|   - gets the lineages from the variants lineage file
| Input:
|   - noFastBl
|     o 1: ignore fast alignment types (ftrs and fins)
|     o 0: allow fast alignment types
|   - tblSTPtr:
|     o tblST_kmerFind struct pointer with settings for
|       fast alignment (only used if noFastBl == 0)
|   - errSL:
|     o signed long pointer to get the line the error was
|       on or number lines in file
|   - linFILE:
|     o FILE pionter to variant lineage file
| Output:
|   - Modifies:
|     o errSL to have line the error was on
|       * is negative if error was no variant error
|       * is positive if error was file or memory error
|       * if no error, number of lines in file
|   - Returns:
|     o a simple_linST struct with the lineages
|     o 0 for memory or file errors
\-------------------------------------------------------*/
struct simple_linST *
getSimpleLineages_linST(
   signed char noFastBl,   /*1: ignore fast types*/
   struct tblST_kmerFind *tblSTPtr,
   signed long *errSL,     /*gets line error was on*/
   void *linFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun39 TOC:
   '   - gets the lineages from the variants lineage file
   '   o fun39 sec01:
   '     - variable declarations
   '   o fun39 sec05:
   '     - clean up (errors only) and return
   '   o fun39 sec02:
   '     - initialize and set up
   '   o fun39 sec03:
   '     - read entries in the file
   '   o fun39 sec04:
   '     - assign group index's to each index
   '   o fun39 sec05:
   '     - clean up (errors only) and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun39 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define fun39_lenBuffer 4096
   signed char buffStr[fun39_lenBuffer + 8];
   signed int strLenSI = 0;
   signed int posSI = 0;

   signed int linLenSI = 0;
   signed int longestSeqSI = 0;/*for kmerFind adding seq*/

   float tmpF = 0;

   signed long buffLenSL = 0;  /*bytes in the buffer*/
   struct one_linST *linSTPtr = 0;
   struct simple_linST *lineagesHeapST = 0;
   signed int *indexHeapArySI = 0;
   signed int sizeSI = 0;

   signed char defLinBl = 0;
   ulong_ulCp orLinUL = mkDelim_ulCp(def_orLineage_linST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun39 Sec02:
   ^   - initialize and set up
   ^   o fun39 sec02 sub01:
   ^     - allocate initial memory and initialize array
   ^   o fun39 sec02 sub02:
   ^     - move past header to first entry in file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun39 Sec02 Sub01:
   *   - allocate initial memory and initialize array
   \*****************************************************/

   *errSL = 1;
   linLenSI = 0;

   lineagesHeapST = malloc(sizeof(struct simple_linST));
   if(! lineagesHeapST)
      goto memErr_fun39_sec05;
   init_simple_linST(lineagesHeapST);

   if( addMem_simple_linST(lineagesHeapST, 16) )
      goto memErr_fun39_sec05;
   linSTPtr = lineagesHeapST->linAryST;

   /*****************************************************\
   * Fun39 Sec02 Sub02:
   *   - move past header to first entry in file
   \*****************************************************/

   buffLenSL =
      getLine_fileFun(
         linFILE,
         buffStr,
         fun39_lenBuffer,
         &buffLenSL /*ignoreing this*/
      );
   if(! buffLenSL)
      goto fileErr_fun39_sec05;

   (*errSL)++;
   buffLenSL =
      getLine_fileFun(
         linFILE,
         buffStr,
         fun39_lenBuffer,
         &buffLenSL /*ignoreing this*/
      );
   if(! buffLenSL)
      goto fileErr_fun39_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun39 Sec03:
   ^   - read entries in the file
   ^   o fun39 sec03 sub01:
   ^     - check if need more memory + start loop
   ^   o fun39 sec03 sub02:
   ^     - get past empty space at start
   ^   o fun39 sec03 sub03:
   ^     - get gene name
   ^   o fun39 sec03 sub04:
   ^     - get group name
   ^   o fun39 sec03 sub05:
   ^     - get lineage id
   ^   o fun39 sec03 sub06:
   ^     - get start position of variant
   ^   o fun39 sec03 sub07:
   ^     - get end position of variant
   ^   o fun39 sec03 sub08:
   ^     - get molecule type
   ^   o fun39 sec03 sub09:
   ^     - get variant type
   ^   o fun39 sec03 sub10:
   ^     - get direction of the pattern
   ^   o fun39 sec03 sub11:
   ^     - get the pattern(s)
   ^   o fun39 sec03 sub12:
   ^     - get lineage to assign
   ^   o fun39 sec03 sub13:
   ^     - mark if printing the lineage
   ^   o fun39 sec03 sub14:
   ^     - get fudge amount for lineage assingnment
   ^   o fun39 sec03 sub15:
   ^     - get minimum score for trs or ins alignments
   ^   o fun39 sec03 sub16:
   ^     - get maximum gap between TRS sequences
   ^   o fun39 sec03 sub17:
   ^     - move to the next line/entry in the file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun39 Sec03 Sub01:
   *   - check if need more memory + start loop
   \*****************************************************/

   while(buffLenSL)
   { /*Loop: read each line from the file*/

      if(linLenSI >= lineagesHeapST->sizeSI)
      { /*If: need more memory*/
         sizeSI = lineagesHeapST->sizeSI;
         sizeSI += (sizeSI >> 1);
         if( addMem_simple_linST(lineagesHeapST, sizeSI) )
            goto memErr_fun39_sec05;
      } /*If: need more memory*/

      linSTPtr = &lineagesHeapST->linAryST[linLenSI];

      /**************************************************\
      * Fun39 Sec03 Sub02:
      *   - get past empty space at start
      \**************************************************/

      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto nextLine_fun39_sec03_sub13; /*empty line*/

      /**************************************************\
      * Fun39 Sec03 Sub03:
      *   - get gene name
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
         && buffStr[posSI + 2] < 33
      ){ /*If: no gene name*/
         linSTPtr->geneStr = malloc(sizeof(signed char));
         if(! linSTPtr->geneStr)
            goto memErr_fun39_sec05;
         linSTPtr->geneStr[0] = 0;
      }  /*If: no gene name*/

      else
      { /*Else: have a gene name*/
         linSTPtr->geneStr =
            malloc((strLenSI + 8) * sizeof(signed char));
         if(! linSTPtr->geneStr)
            goto memErr_fun39_sec05;
         cpWhite_ulCp(linSTPtr->geneStr, &buffStr[posSI]);
      } /*Else: have a gene name*/


      /*move to the next entery*/
      posSI += strLenSI; 
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub04:
      *   - get group name
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
         && buffStr[posSI + 2] < 33
      ){ /*If: no group name*/
         linSTPtr->groupStr = malloc(sizeof(signed char));
         if(! linSTPtr->groupStr)
            goto memErr_fun39_sec05;
         linSTPtr->groupStr[0] = 0;
      }  /*If: no group name*/

      else
      { /*Else: have a group name*/
         linSTPtr->groupStr =
            malloc((strLenSI + 8) * sizeof(signed char));
         if(! linSTPtr->groupStr)
            goto memErr_fun39_sec05;
         cpWhite_ulCp(linSTPtr->groupStr,&buffStr[posSI]);
      } /*Else: have a group name*/


      /*move to the next entery*/
      posSI += strLenSI; 
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub05:
      *   - get lineage id
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);

      linSTPtr->idStr =
         malloc((strLenSI + 8) * sizeof(signed char));
      if(! linSTPtr->idStr)
         goto memErr_fun39_sec05;
      cpWhite_ulCp(linSTPtr->idStr, &buffStr[posSI]);

      /*move to the next entery*/
      posSI += strLenSI; 
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub06:
      *   - get start position of variant
      \**************************************************/

      posSI += strToF_base10str(&buffStr[posSI], &tmpF);

      if(buffStr[posSI] == ' ')
         ;
      else if(buffStr[posSI] == '\t')
         ;
      else
         goto fileErr_fun39_sec05;

      linSTPtr->startSI = (signed int) tmpF - 1;

      /*move to the next entery*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub07:
      *   - get end position of variant
      \**************************************************/

      posSI += strToF_base10str(&buffStr[posSI], &tmpF);

      if(buffStr[posSI] == ' ')
         ;
      else if(buffStr[posSI] == '\t')
         ;
      else
         goto fileErr_fun39_sec05;

      linSTPtr->endSI = (signed int) tmpF - 1;

      /*move to the next entery*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub08:
      *   - get molecule type
      \**************************************************/

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'T'
      ) linSTPtr->moleculeTypeSC = def_ntSeq_linST;

      else if(
            (buffStr[posSI] & ~32) == 'A'
         && (buffStr[posSI + 1] & ~32) == 'A'
      ) linSTPtr->moleculeTypeSC = def_aaSeq_linST;

      else
         goto fileErr_fun39_sec05; /*no idea on type*/


      /*move to the next entery*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub09:
      *   - get variant type
      \**************************************************/

      if(
            (buffStr[posSI] & ~32) == 'S'
         && (buffStr[posSI + 1] & ~32) == 'N'
         && (buffStr[posSI + 2] & ~32) == 'P'
      ) linSTPtr->checkTypeSC = def_snpType_linST;

      else if(
            (buffStr[posSI] & ~32) == 'D'
         && (buffStr[posSI + 1] & ~32) == 'E'
         && (buffStr[posSI + 2] & ~32) == 'L'
      ){ /*Else If: deletion check*/
         linSTPtr->checkTypeSC = def_delType_linST;

         if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
            goto fastAAErr_fun39_sec05;
           /*deletion method is nuceotide only*/
      }  /*Else If: deletion check*/

      else if(
            ! noFastBl
         && (buffStr[posSI] & ~32) == 'T'
         && (buffStr[posSI + 1] & ~32) == 'R'
         && (buffStr[posSI + 2] & ~32) == 'S'
         && (buffStr[posSI + 3] & ~32) == 'F'
      ){ /*Else If: checking trs in fast mode*/
         linSTPtr->checkTypeSC = def_trsFastType_linST;
         if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
            goto fastAAErr_fun39_sec05;
            /*fast methods are nuceotide only*/
      }  /*Else If: checking trs in fast mode*/

      else if(
            (buffStr[posSI] & ~32) == 'T'
         && (buffStr[posSI + 1] & ~32) == 'R'
         && (buffStr[posSI + 2] & ~32) == 'S'
      ) linSTPtr->checkTypeSC = def_trsType_linST;

      else if(
            ! noFastBl
         && (buffStr[posSI] & ~32) == 'I'
         && (buffStr[posSI + 1] & ~32) == 'N'
         && (buffStr[posSI + 2] & ~32) == 'S'
         && (buffStr[posSI + 3] & ~32) == 'F'
      ){ /*Else If: checking ins in fast mode*/
         linSTPtr->checkTypeSC = def_insFastType_linST;

         if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
            goto fastAAErr_fun39_sec05;
            /*fast methods are nuceotide only*/
      }  /*Else If: checking ins in fast mode*/

      else if(
            (buffStr[posSI] & ~32) == 'I'
         && (buffStr[posSI + 1] & ~32) == 'N'
         && (buffStr[posSI + 2] & ~32) == 'S'
      ) linSTPtr->checkTypeSC = def_insType_linST;

      else if(
            (buffStr[posSI] & ~32) == 'L'
         && (buffStr[posSI + 1] & ~32) == 'E'
         && (buffStr[posSI + 2] & ~32) == 'N'
      ){ /*Else If: length check*/
         linSTPtr->checkTypeSC = def_lenType_linST;
         if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
            goto fastAAErr_fun39_sec05;
            /*length method does not convert AA, so
            `  nuceotide only
            */
      }  /*Else If: length check*/

      else if(
            (buffStr[posSI] & ~32) == 'C'
         && (buffStr[posSI + 1] & ~32) == 'O'
         && (buffStr[posSI + 2] & ~32) == 'U'
         && (buffStr[posSI + 3] & ~32) == 'N'
         && (buffStr[posSI + 4] & ~32) == 'T'
         && (buffStr[posSI + 5] & ~32) == 'F'
      ){ /*Else If: length check*/
         linSTPtr->checkTypeSC = def_countFastType_linST;
         if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
            goto fastAAErr_fun39_sec05;
            /*length method does not convert AA, so
            `  nuceotide only
            */
      }  /*Else If: length check*/

      else if(
            (buffStr[posSI] & ~32) == 'C'
         && (buffStr[posSI + 1] & ~32) == 'O'
         && (buffStr[posSI + 2] & ~32) == 'U'
         && (buffStr[posSI + 3] & ~32) == 'N'
         && (buffStr[posSI + 4] & ~32) == 'T'
      ) linSTPtr->checkTypeSC = def_countType_linST;

      else
         goto fileErr_fun39_sec05; /*no idea on type*/


      /*move to the next entery*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub10:
      *   - get direction of the pattern
      \**************************************************/

      if((buffStr[posSI] & ~32) == 'F')
         linSTPtr->revDirBl = 0;
      else if(buffStr[posSI] == '+')
         linSTPtr->revDirBl = 0;
      else if((buffStr[posSI] & ~32) == 'R')
         linSTPtr->revDirBl = 1;
      else if(buffStr[posSI] == '-')
         linSTPtr->revDirBl = 1;
      else
         goto fileErr_fun39_sec05; /*no idea on type*/


      /*move to the next entery*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub11:
      *   - get the pattern(s)
      *   o fun39 sec03 sub11 cat01:
      *     - find the number of patterns
      *   o fun39 sec03 sub11 cat02:
      *     - allocate memory for storing patterns
      *   o fun39 sec03 sub11 cat03:
      *     - add patterns to the lineage
      *   o fun39 sec03 sub11 cat03:
      *     - find patterns length; loop for all patterns
      *   o fun39 sec03 sub11 cat04:
      *     - add deletion/length pattern; coords only
      *   o fun39 sec03 sub11 cat05:
      *     - add fast check (ins/trs) pattern
      *   o fun39 sec03 sub11 cat06:
      *     - add pattern (regular) to the array
      *   o fun39 sec03 sub11 cat07:
      *     - move to next pattern (or end of patterns)
      *   o fun39 sec03 sub11 cat08:
      *     - move to next entry (off white space)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun39 Sec03 Sub11 Cat01:
      +   - find the number of patterns
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      strLenSI = posSI;
      buffLenSL = 0;
      defLinBl = 0;

      if(buffStr[strLenSI] == def_defLineage_linST)
      { /*If: have the default lineage*/
         defLinBl = 1;
         ++strLenSI;
      } /*If: have the default lineage*/

      while(buffStr[strLenSI] >= 33)
      { /*Loop: find number of patterns*/
         if(buffStr[strLenSI] == def_orLineage_linST)
         { /*If: have multiple lineages*/
            if(defLinBl)
               goto fileErr_fun39_sec05;
            ++buffLenSL;
            ++strLenSI;
         } /*If: have multiple lineages*/

         if(defLinBl)
            ;
         else if(buffStr[strLenSI]==def_defLineage_linST)
            goto fileErr_fun39_sec05;
            /*multi-lineage system for defualt or invalid
            `  entry
            */
         strLenSI +=
            endWhiteDelim_ulCp(
               &buffStr[strLenSI],
               orLinUL,
               def_orLineage_linST
            );
      } /*Loop: find number of patterns*/

      ++strLenSI;  /*convert to index 1*/
      ++buffLenSL; /*convert to index 1*/

      if(defLinBl)
         linSTPtr->defGroupBl = 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun39 Sec03 Sub11 Cat02:
      +   - allocate memory for storing patterns
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
            linSTPtr->checkTypeSC == def_insFastType_linST
         || linSTPtr->checkTypeSC == def_trsFastType_linST
         || linSTPtr->checkTypeSC==def_countFastType_linST
      ){ /*If: doing fast checking (no sequence)*/
         linSTPtr->fastAryST =
            malloc(
              buffLenSL * sizeof(struct refST_kmerFind)
            );
         if(! linSTPtr->fastAryST)
            goto memErr_fun39_sec05;

         for(tmpF = 0; tmpF < buffLenSL; ++tmpF)
         { /*Loop: initialize structs*/
             init_refST_kmerFind(
                &linSTPtr->fastAryST[(signed int) tmpF]
             );
         } /*Loop: initialize structs*/

         for(tmpF = 0; tmpF < buffLenSL; ++tmpF)
         { /*Loop: setup pattern structure*/
             if(
                setup_refST_kmerFind(
                  &linSTPtr->fastAryST[(signed int) tmpF],
                  def_lenKmer_kmerFind
                )
             ) goto memErr_fun39_sec05;
         } /*Loop: setup pattern structure*/
      }  /*If: doing fast checking (no sequence)*/

      else if(
            linSTPtr->checkTypeSC != def_delType_linST
         && linSTPtr->checkTypeSC != def_lenType_linST
      ){ /*Else If: lineage method has a pattern*/
         linSTPtr->seqAryStr =
            calloc(buffLenSL, sizeof(signed char *));
         if(! linSTPtr->seqAryStr)
            goto memErr_fun39_sec05;
      }  /*Else If: lineage method has a pattern*/

      if(linSTPtr->checkTypeSC != def_delType_linST)
      { /*If: lineage used sequence length somehow*/
         linSTPtr->seqLenArySI =
            calloc(buffLenSL, sizeof(signed int));
         if(! linSTPtr->seqLenArySI)
            goto memErr_fun39_sec05;
         linSTPtr->seqSizeSI = buffLenSL;
      }  /*If: lineage used sequence length somehow*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun39 Sec03 Sub11 Cat03:
      +   - find a patterns length (loop for all patterns)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      buffLenSL = 0;
      while(buffStr[posSI] >= 33)
      { /*Loop: copy the patterns*/
         strLenSI =
            endWhiteDelim_ulCp(
               &buffStr[posSI],
               orLinUL,
               def_orLineage_linST
            );

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun39 Sec03 Sub11 Cat04:
         +   - add deletion/length pattern; coords only
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(linSTPtr->checkTypeSC == def_lenType_linST)
         { /*If: lineage is length lineage (no pattern)*/
            posSI +=
               strToSI_base10str(
                  &buffStr[posSI],
                  &linSTPtr->seqLenArySI[buffLenSL]
               );
            if(buffStr[posSI] == def_orLineage_linST)
               ;
            else if(buffStr[posSI] > 32)
               goto fileErr_fun39_sec05;
         }  /*If: lineage is length lineage (no pattern)*/

         else if(
            linSTPtr->checkTypeSC == def_delType_linST
         ){ /*Else If: deletion lineage (no pattern)*/
            posSI += endWhite_ulCp(&buffStr[posSI]);
            break; /*sequence entry not used for dels*/
         }  /*Else If: deletion lineage (no pattern)*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun39 Sec03 Sub11 Cat05:
         +   - add fast check (ins/trs) pattern
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(
             linSTPtr->checkTypeSC==def_trsFastType_linST
          || linSTPtr->checkTypeSC==def_insFastType_linST
          || linSTPtr->checkTypeSC==def_countFastType_linST
         ){ /*Else If: fast check*/
            if(strLenSI < 12)
               goto fastShortSeqErr_fun39_sec05;
               /*sequence is to short to reliably detect*/

            tmpF = buffStr[posSI + strLenSI];
            buffStr[posSI + strLenSI] = 0;
            longestSeqSI =
               addNoIndexSeqToRefST_kmerFind(
                  tblSTPtr,
                  &linSTPtr->fastAryST[buffLenSL],
                  &buffStr[posSI],
                  strLenSI,
                  0,       /*auto add id in*/
                  def_minKmerPerc_kmerFind,
                  longestSeqSI
               );
            if(! longestSeqSI)
               goto memErr_fun39_sec05;
            posSI += strLenSI;
            buffStr[posSI] = (signed char) tmpF;
         }  /*Else If: fast check*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun39 Sec03 Sub11 Cat06:
         +   - add pattern (regular) to the array
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: is a pattern lineage*/
            linSTPtr->seqAryStr[buffLenSL] =
               malloc(
                    (strLenSI +
                     (
                      (8 & -(strLenSI % 8))
                       - (strLenSI % 8)
                    )) * sizeof(signed char)
               );
            if(! linSTPtr->seqAryStr[buffLenSL])
               goto memErr_fun39_sec05;

            cpLen_ulCp(
              linSTPtr->seqAryStr[buffLenSL],
              &buffStr[posSI],
              strLenSI
            );

            linSTPtr->seqLenArySI[buffLenSL] = strLenSI;
            posSI += strLenSI;
         } /*Else: is a pattern lineage*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun39 Sec03 Sub11 Cat07:
         +   - move to next pattern (or end of patterns)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         ++linSTPtr->seqLenSI;
         posSI += (buffStr[posSI] > 32);
         ++buffLenSL;
      } /*Loop: copy the patterns*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun39 Sec03 Sub11 Cat08:
      +   - move to next entry (off white space)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub12:
      *   - get lineage to assign
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
         && buffStr[posSI + 2] < 33
      ){ /*If: no lineage to assign*/
         if(
              linSTPtr->checkTypeSC == def_trsType_linST
           || linSTPtr->checkTypeSC == def_countType_linST
           || linSTPtr->checkTypeSC==def_trsFastType_linST
           ||    linSTPtr->checkTypeSC
              == def_countFastType_linST
         ) linSTPtr[linLenSI].trsLineageSI = 0;

         else
         { /*Else: not trs lineage (no assignment)*/
            linSTPtr->lineageStr =
               malloc(sizeof(signed char));
            if(! linSTPtr->lineageStr)
               goto memErr_fun39_sec05;
            linSTPtr->lineageStr[0] = 0;
         } /*Else: not trs lineage (no assignment)*/
      }  /*If: no lineage to assign*/

      else
      { /*Else: have a lineage to assign*/
         if(
              linSTPtr->checkTypeSC == def_trsType_linST
           || linSTPtr->checkTypeSC == def_countType_linST
           || linSTPtr->checkTypeSC==def_trsFastType_linST
           ||    linSTPtr->checkTypeSC
              == def_countFastType_linST
         ){ /*If: trs lineage (number input)*/
           strLenSI =
              strToF_base10str(&buffStr[posSI], &tmpF);

           if(buffStr[posSI + strLenSI] > 32)
              goto fileErr_fun39_sec05;
           linSTPtr->trsLineageSI = tmpF - 1;
             /*needs to be index 0*/
         } /*If: trs lineage (number input)*/

         else
         { /*Else: standared lineage, anything goes*/
            linSTPtr->lineageStr =
               malloc((strLenSI+8) * sizeof(signed char));
            if(! linSTPtr->lineageStr)
               goto memErr_fun39_sec05;
            cpWhite_ulCp(
               linSTPtr->lineageStr,
               &buffStr[posSI]
            );
         } /*Else: standared lineage, anything goes*/
      } /*Else: have a lineage to assign*/


      /*move to the next entery*/
      posSI += strLenSI; 
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub13:
      *   - mark if printing the lineage
      \**************************************************/

      if( (buffStr[posSI] & ~32) == 'T' )
         linSTPtr->printLinBl = 1;
      else if(buffStr[posSI] == '+')
         linSTPtr->printLinBl = 1;
      else if(buffStr[posSI] == '1')
         linSTPtr->printLinBl = 1;
      else if( (buffStr[posSI] & ~32) == 'F' )
         linSTPtr->printLinBl = 0;
      else if(buffStr[posSI] == '-')
         linSTPtr->printLinBl = 0;
      else if(buffStr[posSI] == '0')
         linSTPtr->printLinBl = 0;
      else
         goto fileErr_fun39_sec05;

      /*move to the next entery*/
      while(buffStr[posSI] > 32)
         ++posSI;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub14:
      *   - get fudge amount for lineage assingnment
      \**************************************************/

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
      ){ /*If: no fudge amount*/
         linSTPtr->fudgeSI = 0;

         while(buffStr[posSI] > 32)
            ++posSI;
      }  /*If: no fudge amount*/

      else
      { /*Else: user provided a fudge number*/
         posSI += strToF_base10str(&buffStr[posSI],&tmpF);

         if(buffStr[posSI] == ' ')
            ;
         else if(buffStr[posSI] == '\t')
            ;
         else
            goto fileErr_fun39_sec05;

         linSTPtr->fudgeSI = tmpF;
      } /*Else: user provided a fudge number*/


      /*move to the next entery*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub15:
      *   - get minimum score for trs or ins alignments
      \**************************************************/

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
      ){  /*If: no min score input*/
         linSTPtr->minScoreSI = 0;

         while(buffStr[posSI] > 32)
            ++posSI;
      }  /*If: no min score input*/

      else
      { /*Else: user provided a min score*/
         posSI += strToF_base10str(&buffStr[posSI],&tmpF);

         if(buffStr[posSI] == ' ')
            ;
         else if(buffStr[posSI] == '\t')
            ;
         else
            goto fileErr_fun39_sec05;

         linSTPtr->minScoreSI = tmpF;
      } /*Else: user provided a min score*/


      /*move to the next entery*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun39_sec05;

      /**************************************************\
      * Fun39 Sec03 Sub16:
      *   - get maximum gap between TRS sequences
      \**************************************************/

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
      ){ /*If: no trs gap amount*/
         linSTPtr->trsGapSI = 0;

         while(buffStr[posSI] > 32)
            ++posSI;
      }  /*If: no trs gap amount*/

      else
      { /*Else: user provided a trs gap size*/
         posSI += strToF_base10str(&buffStr[posSI],&tmpF);

         if(buffStr[posSI] < 33)
            ; /*white space, ok to end*/
         else
            goto fileErr_fun39_sec05;

         linSTPtr->trsGapSI = tmpF;
      } /*Else: user provided a trs gap size*/

      /*at end of entry, everything else is future data,
      `  user notes, or input for another program, which
      `  is very unlikely, since this program will likely
      `  to nowere
      */

      /**************************************************\
      * Fun39 Sec03 Sub17:
      *   - move to the next line/entry in the file
      \**************************************************/

      ++linLenSI;

      nextLine_fun39_sec03_sub13:;
         buffLenSL =
            getLine_fileFun(
               linFILE,
               buffStr,
               fun39_lenBuffer,
               &buffLenSL /*ignoreing this*/
            );
         (*errSL)++;
   } /*Loop: read each line from the file*/

   lineagesHeapST->lenSI = linLenSI;

   if(longestSeqSI)
   { /*If: user is using a fast method*/
      if(
         prep_tblST_kmerFind(
            tblSTPtr,
            def_extraNtInWin_kmerFind,
            def_percShift_kmerFind,
            longestSeqSI
         ) /*set up for fast trs scanning*/
      ) goto memErr_fun39_sec05;
   } /*If: user is using a fast method*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun39 Sec04:
   ^   - assign group index's to each index
   ^   o fun39 sec04 sub01:
   ^     - initialize memory and sort by group id
   ^   o fun39 sec04 sub02:
   ^     - find number of groups in the lineages
   ^   o fun39 sec04 sub03:
   ^     - setup group id index's
   ^   o fun39 sec04 sub04:
   ^     - unsort and add default group start and ends
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun39 Sec04 Sub01:
   *   - initialize memory and sort by group id
   \*****************************************************/

   /*final sorting pattern, will use unsort later*/
   sortPosAry_one_linST(
      lineagesHeapST->linAryST,
      lineagesHeapST->lenSI
   );

   indexHeapArySI =
      malloc(lineagesHeapST->lenSI * sizeof(signed int));
   if(! indexHeapArySI)
      goto memErr_fun39_sec05;

   /*____set up index's to allow easy unsorting later___*/
   for(posSI = 0; posSI < lineagesHeapST->lenSI; ++posSI)
      indexHeapArySI[posSI] = posSI;

   sortGroupAry_one_linST(
      lineagesHeapST->linAryST,
      indexHeapArySI,
      lineagesHeapST->lenSI
   ); /*allows easier loging of group ids*/

   /*****************************************************\
   * Fun39 Sec04 Sub02:
   *   - find number of groups in the lineages
   \*****************************************************/

   /*______________find number of groups________________*/
   linLenSI = 1;
   for(posSI = 1; posSI < lineagesHeapST->lenSI; ++posSI)
   { /*Loop: find number of groups*/
      if(
         eqlNull_ulCp(
            lineagesHeapST->linAryST[posSI - 1].groupStr,
            lineagesHeapST->linAryST[posSI].groupStr
         )
      ) ++linLenSI; /*starting a new group*/
   } /*Loop: find number of groups*/

   lineagesHeapST->foundGroupAryBl =
      calloc(linLenSI, sizeof(signed char));
   if(! lineagesHeapST->foundGroupAryBl)
      goto memErr_fun39_sec05;

   lineagesHeapST->defGroupArySI =
      malloc(linLenSI * sizeof(signed int));
   if(! lineagesHeapST->defGroupArySI)
      goto memErr_fun39_sec05;
   for(posSI = 0; posSI < linLenSI; ++posSI)
      lineagesHeapST->defGroupArySI[posSI] = -1;

   lineagesHeapST->groupLenSI = linLenSI;

   /*****************************************************\
   * Fun39 Sec04 Sub03:
   *   - setup group id index's
   \*****************************************************/

   /*________________assign group ids___________________*/
   linLenSI = 0; /*reusing for group ids*/
   lineagesHeapST->groupArySI[0] = linLenSI;

   if(lineagesHeapST->linAryST[0].defGroupBl)
      lineagesHeapST->defGroupArySI[0]=indexHeapArySI[0];

   /*_____________assign_default_groups_________________*/
   for(posSI = 1; posSI < lineagesHeapST->lenSI; ++posSI)
   { /*Loop: assign group ids to a group index*/
      if(
         eqlNull_ulCp(
            lineagesHeapST->linAryST[posSI - 1].groupStr,
            lineagesHeapST->linAryST[posSI].groupStr
         )
      ) ++linLenSI; /*starting a new group*/

      if(lineagesHeapST->linAryST[posSI].defGroupBl)
         lineagesHeapST->defGroupArySI[linLenSI] =
           indexHeapArySI[posSI];
           /*set default goup index for faster lookups*/

      lineagesHeapST->groupArySI[posSI] = linLenSI;
   } /*Loop: assign group ids to a group index*/

   /*****************************************************\
   * Fun39 Sec04 Sub04:
   *   - unsort and add default group start and ends
   \*****************************************************/

   unsort_simple_linST(lineagesHeapST, indexHeapArySI);
     /*need to revert back to the original sorting order*/

   for(posSI = 0; posSI < lineagesHeapST->lenSI; ++posSI)
   { /*Loop: find default group start and end*/
      /*get the index of the default groups index*/
      linLenSI = lineagesHeapST->groupArySI[posSI];
      if(linLenSI < 0)
         continue; /*no default lineage for this group*/

      /*get the index of the default groups*/
      linLenSI = lineagesHeapST->defGroupArySI[linLenSI];
      if(linLenSI < 0)
         continue; /*no default lineage for this group*/

      if(
           lineagesHeapST->linAryST[linLenSI].startSI
        >  lineagesHeapST->linAryST[posSI].startSI
      ) lineagesHeapST->linAryST[linLenSI].startSI =
          lineagesHeapST->linAryST[posSI].startSI;

      if(
           lineagesHeapST->linAryST[linLenSI].endSI
        <  lineagesHeapST->linAryST[posSI].endSI
      ) lineagesHeapST->linAryST[linLenSI].endSI =
          lineagesHeapST->linAryST[posSI].endSI;
   } /*Loop: find default group start and end*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun39 Sec05:
   ^   - clean up (errors only) and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun39_sec05;

   fileErr_fun39_sec05:;
      goto errClean_fun39_sec05;

   fastAAErr_fun39_sec05:;
      *errSL *= -1;
      goto errClean_fun39_sec05;

   fastShortSeqErr_fun39_sec05:;
      *errSL *= -1;
      goto errClean_fun39_sec05;

   memErr_fun39_sec05:;
      goto errClean_fun39_sec05;

   errClean_fun39_sec05:;
      if(lineagesHeapST)
        freeHeap_simple_linST(lineagesHeapST);
      lineagesHeapST = 0;
      goto ret_fun39_sec05;
      
   ret_fun39_sec05:;
      if(indexHeapArySI)
         free(indexHeapArySI);
      indexHeapArySI = 0;

      return lineagesHeapST;
} /*getSimpleLineages_linST*/

/*-------------------------------------------------------\
| Fun40: intInsert_linST
|   - inserts an integer into an array at its sorted
|     position (this does not insert duplicates)
| Input:
|   - inSI:
|     o integer to insert
|   - arySI:
|     o signed int array to insert element into
|   - lenSI:
|     o length of the array (index 1)
| Output:
|   - Modifies:
|     o arySI to have inSI at its sorted position
|   - Returns;
|     o new index of inSI
|     o negative index if was a duplicate (not inserted)
|     o (signed int) ~((unsigned int) -1 >> 1) if
|       duplicate as at the 0th index
\-------------------------------------------------------*/
signed int
intInsert_linST(
   signed int inSI,  /*to insert*/
   signed int *arySI, /*array to insert into*/
   signed int lenSI   /*length of array*/
){
   signed int leftSI = 0;
   signed int midSI = 0;
   signed int rightSI = lenSI - 1;

   if(rightSI < 0)
   { /*If: this is the first item in the array*/
      arySI[0] = inSI; /*nothing in the array*/
      return 0;
   } /*If: this is the first item in the array*/

   while(leftSI <= rightSI)
   { /*Loop: find the insert position*/
      midSI = (leftSI + rightSI) >> 1;

      if(inSI > arySI[midSI])
         leftSI = midSI + 1;
      else if(inSI < arySI[midSI])
         rightSI = midSI - 1;
      else
      { /*Else: integer is already in the array*/
         if(midSI)
            return midSI * -1; /*already inserted*/
         else
            return (signed int) ~((unsigned int) -1 >> 1);
              /*for 0; just set negative bit*/
      } /*Else: integer is already in the array*/
   } /*Loop: find the insert position*/

   if(midSI >= lenSI)
      ; /*inserting at end*/
   else if(inSI > arySI[midSI])
      ++midSI;

   for(rightSI = lenSI - 1; rightSI >= midSI; --rightSI)
      arySI[rightSI + 1] = arySI[rightSI];
   arySI[midSI] = inSI;
   return midSI;
} /*intInsert_linST*/

/*-------------------------------------------------------\
| Fun41: getComplexLineages_linST
|   - gets the lineages from the complex lineage file
| Input:
|   - simpleSTPtr:
|     o simple_linST struct array with variant lineages
|   - errSL:
|     o signed long pointer to get the line the error was
|       on or number lines in file
|   - linFILE:
|     o FILE pionter to variant lineage file
| Output:
|   - Modifies:
|     o errSL to have line the error was on
|       * is negative if error was no variant error
|       * is positive if error was file or memory error
|       * if no error, number of lines in file
|   - Returns:
|     o multi_linST struct array with the complex lineages
|     o 0 for memory or file errors
\-------------------------------------------------------*/
struct complex_linST *
getComplexLineages_linST(
   struct simple_linST *simpleSTPtr,
   signed long *errSL,
   void *linFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun41 TOC:
   '   - gets the lineages from the complex lineage file
   '   o fun41 sec01:
   '     - variable declarations
   '   o fun41 sec02:
   '     - memory allocate, initialization, & get 1st line
   '   o fun41 sec03:
   '     - read in lineages in the file
   '   o fun41 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun41 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define fun41_lenBuff 4096
   signed char buffStr[fun41_lenBuff + 8];
   signed long buffLenSL = 0;
   signed int posSI = 0;
   signed int tmpSI = 0;
   float tmpF = 0;

   signed char *trsStr = 0;
   signed char tmpSC = 0;   /*so can set trs maker to \0*/
   signed int trsLinSI = 0;

   signed int strLenSI = 0;
   signed int *indexHeapArySI = 0;
   signed char varStr[32];
   signed char *tmpStr = 0;
   signed char needBl = 0;/*marks in lineage is required*/
   signed char notBl = 0; /*mark if lineage can not be
                          `  present
                          */

   struct complex_linST *complexHeapST = 0;
   struct multi_linST *mLinSTPtr = 0;

   /*for organizing the complex array*/
   signed int *complexIndexHeapArySI = 0;
   signed int *swapSIPtr = 0;

   ulong_ulCp trsDelimUL =
      mkDelim_ulCp(def_trsLinMark_linST);

   ulong_ulCp variantDelimUL =
      mkDelim_ulCp(def_startVariantsComplex_linST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun41 Sec02:
   ^   - memory allocate, initialization, & get 1st line
   ^   o fun41 sec02 sub01:
   ^     - allocate mememory and initialize
   ^   o fun41 sec02 sub02:
   ^     - get the first entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun41 Sec02 Sub01:
   *   - allocate mememory and initialize
   \*****************************************************/

   *errSL = 1;

   indexHeapArySI =
      malloc(simpleSTPtr->lenSI * sizeof(signed int));
   if(! indexHeapArySI)
      goto memErr_fun41_sec05;
   for(posSI = 0; posSI < simpleSTPtr->lenSI; ++posSI)
      indexHeapArySI[posSI] = posSI;
   sortIdAry_one_linST(
      simpleSTPtr->linAryST,
      indexHeapArySI,
      simpleSTPtr->lenSI
   ); /*this is needed for quick id lookups*/

   complexHeapST = malloc(sizeof(struct complex_linST));
   if(! complexHeapST)
      goto memErr_fun41_sec05;
   init_complex_linST(complexHeapST); 

   if( addMem_complex_linST(complexHeapST, 16) )
      goto memErr_fun41_sec05;
   complexIndexHeapArySI=malloc(16 * sizeof(signed int));
   if(! complexIndexHeapArySI)
      goto memErr_fun41_sec05;

   /*****************************************************\
   * Fun41 Sec02 Sub02:
   *   - get the first entry
   \*****************************************************/

   /*get the header*/
   buffLenSL =
      getLine_fileFun(
         linFILE,
         buffStr,
         fun41_lenBuff,
         &buffLenSL
      );
   if(! buffLenSL)
      goto fileErr_fun41_sec05; /*empty file*/

   /*get the first entry*/
   (*errSL)++;
   buffLenSL =
      getLine_fileFun(
         linFILE,
         buffStr,
         fun41_lenBuff,
         &buffLenSL
      );
   if(! buffLenSL)
      goto fileErr_fun41_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun41 Sec03:
   ^   - read in lineages in the file
   ^   o fun41 sec03 sub01:
   ^     - expand memory if needed + start loop
   ^   o fun41 sec03 sub02:
   ^     - get past white space at start
   ^   o fun41 sec03 sub03:
   ^     - get id of lineage
   ^   o fun41 sec03 sub04:
   ^     - get group id of lineage
   ^   o fun41 sec03 sub05:
   ^     - get gene of the lineage
   ^   o fun41 sec03 sub06:
   ^     - get name of lineage
   ^   o fun41 sec03 sub07:
   ^     - fudge for number of simple lineages can miss
   ^   o fun41 sec03 sub08:
   ^     - get if overwriting past lineage assignments
   ^   o fun41 sec03 sub09:
   ^     - get if can print this lineage
   ^   o fun41 sec03 sub10:
   ^     - get method used to determine lineage
   ^   o fun41 sec03 sub11:
   ^     - move to variant start column
   ^   o fun41 sec03 sub12:
   ^     - get variants in lineage
   ^   o fun41 sec03 sub13:
   ^     - get next line in file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun41 Sec03 Sub01:
   *   - expand memory if needed + start loop
   \*****************************************************/

   while(buffLenSL)
   { /*Loop: read in each lineage*/
      if(complexHeapST->lenSI >= complexHeapST->sizeSI)
      { /*If: need more memory*/
         tmpSI = complexHeapST->sizeSI;
         tmpSI += (tmpSI >> 1);

         if( addMem_complex_linST(complexHeapST, tmpSI) )
            goto memErr_fun41_sec05;
         swapSIPtr =
            realloc(
               complexIndexHeapArySI,
               tmpSI * sizeof(signed int)
            );
         if(! swapSIPtr)
            goto memErr_fun41_sec05;
         complexIndexHeapArySI = swapSIPtr;
      } /*If: need more memory*/

      /**************************************************\
      * Fun41 Sec03 Sub02:
      *   - get past white space at start
      \**************************************************/

      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto nextLine_fun41_sec03_sub11; /*empty line*/

      /**************************************************\
      * Fun41 Sec03 Sub03:
      *   - get id of lineage
      \**************************************************/

      strLenSI =
         addIdSort_complex_linST(
            complexHeapST,
            &buffStr[posSI],
            complexIndexHeapArySI
         );
      if(strLenSI == -1)
         goto fileErr_fun41_sec05;
      else if(strLenSI == -2)
         goto memErr_fun41_sec05;
      mLinSTPtr = &complexHeapST->linAryST[strLenSI];

      /*move to next entry*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub04:
      *   - get group id of lineage
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);
      mLinSTPtr->groupIdStr =
         malloc((strLenSI + 8) * sizeof(signed char));
      if(! mLinSTPtr->groupIdStr)
         goto memErr_fun41_sec05;
      cpLen_ulCp(
         mLinSTPtr->groupIdStr,
         &buffStr[posSI],
         strLenSI
      );

      /*move to next entry*/
      posSI += strLenSI;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub05:
      *   - get gene of the lineage
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);

      if(
            (buffStr[posSI] & ~32) == 'N'
         && (buffStr[posSI + 1] & ~32) == 'A'
         && buffStr[posSI + 2] < 33
      ){ /*If: no gene assigned*/
         mLinSTPtr->geneStr =
            malloc(1 * sizeof(signed char));
         if(! mLinSTPtr->geneStr)
            goto memErr_fun41_sec05;
         mLinSTPtr->geneStr[0] = 0;
      }  /*If: no gene assigned*/

      else
      { /*Else: have a gene name*/
         mLinSTPtr->geneStr =
            malloc((strLenSI + 8) * sizeof(signed char));
         if(! mLinSTPtr->geneStr)
            goto memErr_fun41_sec05;
         cpLen_ulCp(
            mLinSTPtr->geneStr,
            &buffStr[posSI],
            strLenSI
         );
      } /*Else: have a gene name*/


      /*move to next entry*/
      posSI += strLenSI;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub06:
      *   - get name of lineage
      \**************************************************/

      strLenSI = endWhite_ulCp(&buffStr[posSI]);
      mLinSTPtr->lineageStr =
         malloc((strLenSI + 8) * sizeof(signed char));
      if(! mLinSTPtr->lineageStr)
         goto memErr_fun41_sec05;
      cpLen_ulCp(
         mLinSTPtr->lineageStr,
         &buffStr[posSI],
         strLenSI
      );

      /*move to next entry*/
      posSI += strLenSI;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub07:
      *   - fudge for number of simple lineages can miss
      \**************************************************/

      if(
            buffStr[posSI] == 'N'
         && buffStr[posSI + 1] == 'A'
         && buffStr[posSI + 2] < 33
      ){ /*If: no fudge*/
         mLinSTPtr->fudgeSI = 0;
         posSI += 2;
      }  /*If: no fudge*/

      else
      { /*Else: have a fudge value*/
         posSI += strToF_base10str(&buffStr[posSI],&tmpF);
         mLinSTPtr->fudgeSI = tmpF;
         if(buffStr[posSI] > 32)
            goto fileErr_fun41_sec05;/*non-numeric input*/
      } /*Else: have a fudge value*/

      /*move to next entry*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub08:
      *   - get if overwriting past lineage assignments
      \**************************************************/

      if((buffStr[posSI] & ~32) == 'Y')
         mLinSTPtr->overwriteBl = 1;
      else if((buffStr[posSI] & ~32) == 'T')
         mLinSTPtr->overwriteBl = 1;
      else if((buffStr[posSI] & ~32) == 'D')
         mLinSTPtr->overwriteBl = 2;
      else if((buffStr[posSI] & ~32) == 'A')
         mLinSTPtr->overwriteBl = 3;
      else if((buffStr[posSI] & ~32) == 'N')
         mLinSTPtr->overwriteBl = 0;
      else if((buffStr[posSI] & ~32) == 'F')
         mLinSTPtr->overwriteBl = 0;
      else
         mLinSTPtr->overwriteBl = 0;


      /*move to next entry*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub09:
      *   - get if can print this lineage
      \**************************************************/

      if( (buffStr[posSI] & ~32) == 'T' )
         mLinSTPtr->printLinBl = 1;
      else if(buffStr[posSI] == '+')
         mLinSTPtr->printLinBl = 1;
      else if(buffStr[posSI] == '1')
         mLinSTPtr->printLinBl = 1;
      else if( (buffStr[posSI] & ~32) == 'F' )
         mLinSTPtr->printLinBl = 0;
      else if(buffStr[posSI] == '-')
         mLinSTPtr->printLinBl = 0;
      else if(buffStr[posSI] == '0')
         mLinSTPtr->printLinBl = 0;
      else
         goto fileErr_fun41_sec05;

      /*move to next entry*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub10:
      *   - get method used to determine lineage
      \**************************************************/

      if(
            buffStr[posSI]==def_startVariantsComplex_linST
         && buffStr[posSI + 1] < 32
      ){ /*If: column is not present, assume defalut*/
         mLinSTPtr->typeSC = def_matchComplexType_linST;
         goto variantColumn_fun41_sec03_sub11;
      } /*If: column is not present, assume defalut*/

      else if(
            buffStr[posSI]==def_startIgnoreComplex_linST
         && buffStr[posSI] < 32)
      { /*Else If: user wants to ingore everything else*/
         mLinSTPtr->typeSC = def_matchComplexType_linST;
         goto variantColumn_fun41_sec03_sub11;
      } /*Else If: user wants to ingore everything else*/

      else if(
            (buffStr[posSI] &~32) == 'M'
         && (buffStr[posSI + 1] &~32) == 'A'
         && (buffStr[posSI + 2] &~32) == 'T'
         && (buffStr[posSI + 3] &~32) == 'C'
         && (buffStr[posSI + 4] &~32) == 'H'
      ) mLinSTPtr->typeSC = def_matchComplexType_linST;

      else if(
            (buffStr[posSI] &~32) == 'C'
         && (buffStr[posSI + 1] &~32) == 'L'
         && (buffStr[posSI + 2] &~32) == 'O'
         && (buffStr[posSI + 3] &~32) == 'S'
         && (buffStr[posSI + 4] &~32) == 'E'
         && (buffStr[posSI + 5] &~32) == 'S'
         && (buffStr[posSI + 6] &~32) == 'T'
      ) mLinSTPtr->typeSC = def_closestComplexType_linST;

      else if(
            (buffStr[posSI] &~32) == 'N'
         && (buffStr[posSI + 1] &~32) == 'A'
      ) mLinSTPtr->typeSC = def_matchComplexType_linST;
        /*user did not supply a type, so assume match*/

      else
         goto fileErr_fun41_sec05;/*unkown type*/

      /*move to next entry*/
      posSI += endWhite_ulCp(&buffStr[posSI]);
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub11:
      *   - move to variant start column
      \**************************************************/

      variantColumn_fun41_sec03_sub11:;

      posSI +=
         lenStrNull_ulCp(
            &buffStr[posSI],
            variantDelimUL,
            def_startVariantsComplex_linST
         );

      while(buffStr[posSI])
      { /*Loop: find variant column start*/
         if(buffStr[posSI - 1] > 32)
            ;
         else if(buffStr[posSI + 1] < 33)
            break; /*found the variant column*/

         posSI +=
            lenStrNull_ulCp(
               &buffStr[posSI],
               variantDelimUL,
               def_startVariantsComplex_linST
            );
      }  /*Loop: find variant column start*/

      posSI += 2; /*get off "*\t"*/
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI;
      if(! buffStr[posSI])
         goto fileErr_fun41_sec05;

      /**************************************************\
      * Fun41 Sec03 Sub12:
      *   - get variants in lineage
      *   o fun41 sec03 sub12 cat01:
      *     - find the memory needed for the variants
      *   o fun41 sec03 sub12 cat02:
      *     - assign memory for variant pointers
      *   o fun41 sec03 sub12 cat03:
      *     - add the variants to the lineage
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun41 Sec03 Sub12 Cat01:
      +   - find the memory needed for the variants
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSI = posSI;

      while(buffStr[posSI])
      { /*Loop: find variants*/
         if(buffStr[posSI]==def_startIgnoreComplex_linST)
            break; /*end of variants*/

         if(
               (buffStr[posSI] & ~32) == 'N'
            && (buffStr[posSI + 1] & ~32) == 'A'
            && buffStr[posSI + 2] < 33
         ){ /*If: no lineage*/
            posSI += 3;
            while(buffStr[posSI] && buffStr[posSI] < 33)
               ++posSI;
            continue;
         }  /*If: no lineage*/

         tmpStr = varStr;
         posSI += cpWhite_ulCp(tmpStr, &buffStr[posSI]);

         if(*tmpStr == def_needLin_linST)
         { /*If: need this lineage*/
            ++tmpStr; /*get off '!' for required lineage*/

            if(*tmpStr == def_notLin_linST)
               ++tmpStr; /*get off '&' for not lineage*/
         } /*If: need this lineage*/

         else if(*tmpStr == def_notLin_linST)
         { /*If: not this lineage*/
            ++tmpStr; /*get off '!' for not lineage*/

            if(*tmpStr == def_needLin_linST)
               ++tmpStr; /*get off '&' for required lin*/
         } /*If: not this lineage*/

         trsStr = tmpStr;

         trsStr +=
            endWhiteDelim_ulCp(
               trsStr,
               trsDelimUL,
               def_trsLinMark_linST
            );
         tmpSC = *trsStr;
         if(*trsStr == def_trsLinMark_linST)
            *trsStr = 0; /*have trs lineage in header*/
        
         strLenSI =
            idFind_one_linST(
               tmpStr,
               simpleSTPtr->linAryST,
               simpleSTPtr->lenSI
            );

         if(strLenSI >= 0)
            ++mLinSTPtr->linLenSI;

         else
         { /*Else: is likely a complex (muti) lineage*/
            strLenSI =
               idFind_complex_linST(tmpStr,complexHeapST);
            if(strLenSI < 0)
               goto noLinErr_fun41_sec05;

            ++mLinSTPtr->mLinLenSI;
         } /*Else: is likely a complex (muti) lineage*/

         *trsStr = tmpSC;
         while(buffStr[posSI] && buffStr[posSI] < 33)
            ++posSI;
         if(! buffStr[posSI])
            break;
      } /*Loop: find variants*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun41 Sec03 Sub12 Cat02:
      +   - assign memory for variant pointers
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      mLinSTPtr->linIndexArySI =
         calloc(mLinSTPtr->linLenSI, sizeof(signed int));
      if(! mLinSTPtr->linIndexArySI)
         goto memErr_fun41_sec05;

      mLinSTPtr->defGroupArySI =
         calloc(mLinSTPtr->linLenSI, sizeof(signed int));
      if(! mLinSTPtr->linIndexArySI)
         goto memErr_fun41_sec05;

      mLinSTPtr->linTrsArySI =
         calloc(mLinSTPtr->linLenSI, sizeof(signed int));
      if(! mLinSTPtr->linTrsArySI)
         goto memErr_fun41_sec05;

      mLinSTPtr->linNeedAryBl =
         calloc(mLinSTPtr->linLenSI, sizeof(signed char));
      if(! mLinSTPtr->linNeedAryBl)
         goto memErr_fun41_sec05;

      mLinSTPtr->linNotAryBl =
         calloc(mLinSTPtr->linLenSI, sizeof(signed char));
      if(! mLinSTPtr->linNotAryBl)
         goto memErr_fun41_sec05;


      mLinSTPtr->mLinIndexArySI =
         calloc(mLinSTPtr->mLinLenSI,sizeof(signed int));
      if(! mLinSTPtr->mLinIndexArySI)
         goto memErr_fun41_sec05;

      mLinSTPtr->mLinNeedAryBl =
         calloc(mLinSTPtr->mLinLenSI,sizeof(signed char));
      if(! mLinSTPtr->mLinNeedAryBl)
         goto memErr_fun41_sec05;

      mLinSTPtr->mLinNotAryBl =
         calloc(mLinSTPtr->mLinLenSI,sizeof(signed char));
      if(! mLinSTPtr->mLinNotAryBl)
         goto memErr_fun41_sec05;

      mLinSTPtr->linLenSI = 0;
      mLinSTPtr->mLinLenSI = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun41 Sec03 Sub12 Cat03:
      +   - add the variants to the lineage
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      posSI = tmpSI;

      while(buffStr[posSI])
      { /*Loop: find variants*/
         notBl = 0;
         needBl = 0;

         if(buffStr[posSI]==def_startIgnoreComplex_linST)
            break; /*end of variants*/

         /*____________check_if_have_lineage____________*/
         if(
               (buffStr[posSI] & ~32) == 'N'
            && (buffStr[posSI + 1] & ~32) == 'A'
            && buffStr[posSI + 2] < 33
         ){ /*If: no lineage*/
            posSI += 3;
            while(buffStr[posSI] && buffStr[posSI] < 33)
               ++posSI;
            continue;
         }  /*If: no lineage*/

         /*___________find_if_is_trs_lineage____________*/
         tmpStr = varStr;
         posSI += cpWhite_ulCp(tmpStr, &buffStr[posSI]);
         trsStr = tmpStr;

         if(*trsStr == def_needLin_linST)
         { /*If: this is a required lineage*/
            needBl = 1;
            ++trsStr; /*get off '&'*/

            if(*trsStr == def_notLin_linST)
            { /*If: also a not lineage*/
               notBl = 1;
               ++trsStr; /*get off '!'*/
            } /*If: also a not lineage*/
         } /*If: this is a required lineage*/

         else if(*trsStr == def_notLin_linST)
         { /*Else If: this is a not lineage*/
            notBl = 1;
            ++trsStr; /*get off '!'*/

            if(*trsStr == def_needLin_linST)
            { /*If: need this lineage*/
               needBl = 1;
               ++trsStr; /*get off &*/
            } /*If: need this lineage*/
         } /*Else If: this is a not lineage*/

         tmpStr = trsStr;
         trsStr +=
            lenStrNull_ulCp(
               trsStr,
               trsDelimUL,
               def_trsLinMark_linST
            );
         tmpSC = *trsStr;

         if(*trsStr == def_trsLinMark_linST)
         { /*If: have a trs lineage*/
            *trsStr = 0; /*have trs lineage in header*/
            ++trsStr;
            trsStr += strToSI_base10str(trsStr,&trsLinSI);

            if(*trsStr)
               goto fileErr_fun41_sec05;
         } /*If: have a trs lineage*/

         else
            trsLinSI = -1024;

         /*_____________add_variant_lineage_____________*/
         strLenSI =
            idFind_one_linST(
               tmpStr,
               simpleSTPtr->linAryST,
               simpleSTPtr->lenSI
            );

         if(strLenSI >= 0)
         { /*If: is a variant lineage*/
            mLinSTPtr->linIndexArySI[mLinSTPtr->linLenSI]= 
               indexHeapArySI[strLenSI];
            mLinSTPtr->linTrsArySI[mLinSTPtr->linLenSI] =
               trsLinSI;
            mLinSTPtr->linNeedAryBl[mLinSTPtr->linLenSI] =
               needBl;
            mLinSTPtr->linNotAryBl[mLinSTPtr->linLenSI] =
               notBl;
            ++mLinSTPtr->linLenSI;

            /*to get the default group id, I first need to
            `  get the index of the position sorted
            `  lineages (currently the lineage array is
            `  sorted by id) since the group arrays in
            `  simpleSTPtr are sorted by position still
            `  and not id
            */
            trsLinSI = indexHeapArySI[strLenSI];
            trsLinSI =
               simpleSTPtr->defGroupArySI[
                  simpleSTPtr->groupArySI[trsLinSI]
               ];
            if(trsLinSI >= 0)
            { /*If: simple lineage has a default group*/
               trsLinSI =
                  intInsert_linST(
                     trsLinSI,
                     mLinSTPtr->defGroupArySI,
                     mLinSTPtr->defGroupLenSI
                  ); /*insert group id*/
               if(trsLinSI >= 0)
                  ++mLinSTPtr->defGroupLenSI;
                  /*group id was new, so increase length*/
            } /*If: simple lineage has a default group*/

            if(
                 simpleSTPtr->linAryST[strLenSI].startSI
               < mLinSTPtr->startSI
            ) mLinSTPtr->startSI =
                 simpleSTPtr->linAryST[strLenSI].startSI;

            if(
                 simpleSTPtr->linAryST[strLenSI].endSI
               > mLinSTPtr->endSI
            ) mLinSTPtr->endSI =
                 simpleSTPtr->linAryST[strLenSI].endSI;

            ++mLinSTPtr->totalOneLinSI;
            ++mLinSTPtr->totalLinSI;
         } /*If: is a variant lineage*/

         /*_____________add_complex_lineage_____________*/
         else
         { /*Else: is a complex (muti) lineage*/
            strLenSI =
               idFind_complex_linST(tmpStr,complexHeapST);

            mLinSTPtr->mLinIndexArySI[
               mLinSTPtr->mLinLenSI
            ] = strLenSI;

            mLinSTPtr->mLinNeedAryBl[
               mLinSTPtr->mLinLenSI
            ] = needBl;

            mLinSTPtr->mLinNotAryBl[
               mLinSTPtr->mLinLenSI
            ] = notBl;


            ++mLinSTPtr->mLinLenSI;

            if(
                 complexHeapST->linAryST[strLenSI].startSI
               < mLinSTPtr->startSI
            ) mLinSTPtr->startSI =
                complexHeapST->linAryST[strLenSI].startSI;

            if(
                 complexHeapST->linAryST[strLenSI].endSI
               > mLinSTPtr->endSI
            ) mLinSTPtr->endSI =
                 complexHeapST->linAryST[strLenSI].endSI;

            /*keep track of all one lineages*/
            mLinSTPtr->totalOneLinSI +=
               complexHeapST->linAryST[
                  strLenSI
               ].totalOneLinSI;

            /*keep track of all lineages; inlcuding
            `  duplicates
            */
            mLinSTPtr->totalLinSI +=
               complexHeapST->linAryST[
                  strLenSI
               ].totalLinSI + 1;
               /*+1 for added complex lineage*/
         } /*Else: is a complex (muti) lineage*/

         /*_____________move_to_next_lineage____________*/
         *trsStr = tmpSC;
         while(buffStr[posSI] && buffStr[posSI] < 33)
            ++posSI;
         if(! buffStr[posSI])
            break;
      } /*Loop: find variants*/

      /**************************************************\
      * Fun41 Sec03 Sub13:
      *   - get next line in file
      \**************************************************/
      
      mLinSTPtr = 0; /*no longer need*/
      ++complexHeapST->lenSI;

      nextLine_fun41_sec03_sub11:;
         buffLenSL =
            getLine_fileFun(
               linFILE,
               buffStr,
               fun41_lenBuff,
               &buffLenSL
            );
         (*errSL)++;
   } /*Loop: read in each lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun41 Sec04:
   ^   - build group id skip list and unsort to user input
   ^   o fun41 sec04 sub01:
   ^     - assign the group ids
   ^   o fun41 sec04 sub02:
   ^     - sort by end position and ajust the child
   ^       multi_linST indexs in each multi_linST array
   ^   o fun41 sec04 sub03:
   ^     - convert the group ids list into a circular skip
   ^       list
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun41 Sec04 Sub01:
   *   - assign the group ids
   \*****************************************************/

   complexHeapST->groupArySI =
      malloc(complexHeapST->lenSI * sizeof(signed int));
   if(! complexHeapST->groupArySI)
      goto memErr_fun41_sec05;

   complexHeapST->cntArySI =
      calloc(complexHeapST->lenSI, sizeof(signed int));
   if(! complexHeapST->groupArySI)
      goto memErr_fun41_sec05;

   groupSort_complex_linST(
      complexHeapST,
      complexIndexHeapArySI
   );

   trsLinSI = 0;
   complexHeapST->groupArySI[0] = 0;

   for(posSI = 1; posSI < complexHeapST->lenSI; ++posSI)
   { /*Loop: add the group index's*/
      if(
         eqlNull_ulCp(
           complexHeapST->linAryST[posSI].groupIdStr,
           complexHeapST->linAryST[posSI - 1].groupIdStr
         )
      ) ++trsLinSI; /*moving to the next group*/
 
      complexHeapST->groupArySI[posSI] = trsLinSI;
   } /*Loop: add the group index's*/

   /*****************************************************\
   * Fun41 Sec04 Sub02:
   *   - sort by end position and ajust the child
   *     multi_linST indexs in each multi_linST array
   \*****************************************************/

   positionSort_complex_linST(
      complexHeapST,
      complexIndexHeapArySI
   ); /*I used to unsort this, because sorting will mess
      `  up future checks since some multi-lineages can
      `  included other multi-lineages, which complicates
      `  things. `  Best leave in order so the simplest
      `  multi-lineages are first
      `This position sort works because it sorts by the
      `  ending coordinate (complex lineages always have
      `  max range) and also puts the complex lineages
      `  with the most dependencies at the end
      */

   /*this allows me to convert the multi lineage index's
   `  into the new sorted index's
   */
   flipIndexAry_complex_linST(
      complexIndexHeapArySI,
      complexHeapST->lenSI
   );

   for(
      trsLinSI = 1;
      trsLinSI < complexHeapST->lenSI;
      ++trsLinSI
   ){ /*Loop: reset index's for the complex lineages*/
      mLinSTPtr = &complexHeapST->linAryST[trsLinSI];

      for(posSI=0; posSI < mLinSTPtr->mLinLenSI; ++posSI)
      { /*Loop: reset index to match the positon sort*/
         mLinSTPtr->mLinIndexArySI[posSI] =
            complexIndexHeapArySI[
               mLinSTPtr->mLinIndexArySI[posSI]
            ];
      } /*Loop: reset index to match the positon sort*/
   }  /*Loop: reset index's for the complex lineages*/

   /*****************************************************\
   * Fun41 Sec04 Sub03:
   *   - convert the group ids list into a circular skip
   *     list
   \*****************************************************/

   /*at this piont I can convert the group ids into a
   `  circular skip list
   */
   for(posSI = 0; posSI < trsLinSI + 1; ++posSI)
      complexIndexHeapArySI[posSI] = -1;
      /*initialize for making a skip list. I am not
      `  worred about out of bounds errors because at
      `  worst case I have one group per complex lineage
      */

   for(posSI = 0; posSI < complexHeapST->lenSI; ++posSI)
   { /*Loop: add the group index's*/
      trsLinSI =
         complexIndexHeapArySI[
            complexHeapST->groupArySI[posSI]
         ];

      if(trsLinSI >= 0)
      { /*If: already found first index for the group*/
         complexHeapST->groupArySI[posSI] =
            complexHeapST->groupArySI[trsLinSI];
            /*I need to make sure the last index always
            `  points to the first. This makes the group
            `  index list circular
            */
         complexHeapST->groupArySI[trsLinSI] = posSI;
         /*have previous memember in group access this
         `  position*/
      } /*If: already found first index for the group*/

      else
         complexHeapST->groupArySI[posSI] = posSI;
         /*first group index, set equal to its self*/

      complexIndexHeapArySI[
            complexHeapST->groupArySI[posSI]
      ] = posSI;

   } /*Loop: add the group index's*/

   free(complexIndexHeapArySI);
   complexIndexHeapArySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun41 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun41_sec05;

   fileErr_fun41_sec05:;
     goto errClean_fun41_sec05;

   memErr_fun41_sec05:;
     goto errClean_fun41_sec05;

   noLinErr_fun41_sec05:;
     *errSL *= -1;
     goto errClean_fun41_sec05;

   errClean_fun41_sec05:;
      if(complexHeapST)
        freeHeap_complex_linST(complexHeapST);
      complexHeapST = 0;
      goto ret_fun41_sec05;

   ret_fun41_sec05:;
      if(indexHeapArySI)
      { /*If: resorted the one_linST array*/
         unsort_one_linST(
            simpleSTPtr->linAryST,
            indexHeapArySI,
            simpleSTPtr->lenSI
         );
         free(indexHeapArySI);
         indexHeapArySI = 0;
      } /*If: resorted the one_linST array*/

      if(complexIndexHeapArySI)
         free(complexIndexHeapArySI);
      complexIndexHeapArySI = 0;

      return complexHeapST;
} /*getComplexLineages_linST*/

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
