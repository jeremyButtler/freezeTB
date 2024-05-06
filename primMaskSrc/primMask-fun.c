/*########################################################
# Name: primMask-fun
#   - holds functions for primer masking or trimming with
#     sam files
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o .h fun-01: blankPrimFlag
'     - blanks all flags in an primFlag uint
'   o .h fun-02: setDirPrimFlag
'     - sets the direction flag for an primer
'   o .h fun-03: setPairPrimFlag
'     - sets the paired flag for an primer
'   o .h fun-04: setMateIndexPrimIndex
'     - sets the index of the other primer
'   o .h fun-05: getDirPrimFlag
'     - gets the primer direction from an primer flag uint
'   o .h fun-06: getPairPrimFlag
'     - gets if the primer is to be treated as an pair
'   o .h fun-07: getMateIndexPrimFlag
'     - gets the other primers index in the pair
'   o fun-08: maskPrimers
'     - masks the primers in the sam sequence
'   o .c fun-09: swapPrimCoords
'     - swaps two primer coordinates values around and
'       updates the index's
'   o fun-10: sortPrimCoords
'     - Sorts the primer coordiantes of a primer flag
'       array setup. Order is least to greates by first
'       starting positon, then ending position
'   o fun-11: readPrimCoords
'     - gets primer coordinates and flags from an file
'   o license:
'     - Licensing for this code (public domain / mit)
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

#include "primMask-fun.h"

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"

/*No .c files (only .h)*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"
#include "../generalLib/gen-shellSort.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .h #include "../generalLib/numToStr.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-08: maskPrimers
|   - masks the primers in the sam sequence
| Input:
|   - samSTVoidPtr:
|     o pointer to an sam structure to mask primers in
|   - primStartAryUI:
|     o uint array of primer starting coordiantes
|     o This is the starting position on the reference
|       sequence
|   - primEndAryUI:
|     o uint array of primer ending coordiantes
|     o This is the starting position on the reference
|       sequence
|   - primFlagAryUI:
|     o Int array with bits filled for the flags and
|       coordinates
|       - the first two bits have the direction,
|         o def_reverse_primMask (1) if is an reverse
|           primer
|         o def_pair_primMask (2) if you want to only
|           mask primers that have pairs instead off any
|           primer
|       - the next 15 bits (2 bytes - 1) has the index of
|         the forward primer
|       - the last 15 bits has the index of the reverse
|         primer
|   - numPrimSI:
|     o number of primers in primStartAryUI and primEndUI
|   - fudgeSI:
|     o sint with the number of bases before (start) or
|       after (end) to 
|       - -1 mask any detected primer
|       - 0 is no bases before start or after end
|       - > 0 Allow 
|   - maskSC:
|     o character to mask with
| Output:
|   - Modifies:
|     o samSTVoidPtr to have any detected primers masked
|   - Returns:
|     o 0 if no problems
|     o -1 for no detected primers
|     o (primer index << 1) | 1 if primer found but not
|        masked. This can only happen for fudge lengths
|        out of bounds or incomplete primer pairs
\-------------------------------------------------------*/
signed int
maskPrimers(
   void *samSTVoidPtr, /*samEntry struct to mask primer*/
   unsigned int *primStartAryUI,
   unsigned int *primEndAryUI,
   unsigned int *primFlagAryUI,
   signed int numPrimSI,
   signed int fudgeSI, /*Max fudge size*/
   signed char maskSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC:
   '   - Mask the primers in an sam sequence
   '   o fun-08 sec-01:
   '     - Variable declerations
   '   o fun-08 sec-02:
   '     - Mask regions in the read with primers
   '   o fun-08 sec-03:
   '     - Return back the answer
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar revPrimBl = 0;
   schar pairPrimBl = 0;
   sint primIndexSI = 0;
   sint revIndexSI = -1;

   uint forStartUI = 0;
   uint forEndUI = 0;
   uint revStartUI = 0;
   uint revEndUI = 0;

   char firstMaskBl = 1; /*No primers masked yet*/

   /*Values for finding start position*/
   uint uiCig = 0;
   uint cigBaseLeftUI = 0;

   uint refPosUI = 0;
   uint seqPosUI = 0;

   /*Are for finding end of mask region*/
   uint uiEndCig = 0;
   uint cigEndBaseLeftUI = 0;
   uint refEndPosUI = 0;
   uint seqEndPosUI = 0;

   sint retIndexSI = 0; /*For error returing*/

   struct samEntry *samSTPtr =
      (struct samEntry *) samSTVoidPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^   - Mask regions in the read with primers
   ^   o fun-08 sec-02 sub-01:
   ^     - find 1st primers index, start loop, check range
   ^   o fun-08 sec-02 sub-02:
   ^     - Check primers paired, get reverse primer
   ^   o fun-08 sec-02 sub-03:
   ^     - Check if the primer(s) are in fudge length
   ^   o fun-08 sec-02 sub-04:
   ^     - Find start of primer and mask
   ^   o fun-08 sec-02 sub-05:
   ^     - Mask the primer position
   ^   o fun-08 sec-02 sub-06:
   ^     - For paired primers, set up reverse primer mask
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun-08 Sec-02 Sub-01:
   *   - Find first primers index, start loop, check range
   \*****************************************************/

   cigBaseLeftUI = samSTPtr->cigValAryI[0];
   refPosUI = (uint) samSTPtr->refStartUI;

   /*Account for some primer positions not being
   `  complete. I can always loop to the start later if
   `  needed
   */
   if(samSTPtr->refStartUI > 20)
      forStartUI = samSTPtr->refStartUI - 20;
   else
      forStartUI = 0;

   primIndexSI =
      binSearchRange(
         primStartAryUI,
         forStartUI,
         samSTPtr->refEndUI,
         numPrimSI
      );

   if(primIndexSI < 0)
      goto noMask_fun08_sec03_sub02;

   while(primIndexSI < numPrimSI)
   { /*Loop: Mask primers*/
      forStartUI = primStartAryUI[primIndexSI];
      forEndUI = primEndAryUI[primIndexSI];

      /*Only fires after the first case*/
      if(forStartUI > samSTPtr->refEndUI)
      { /*If: the primer is outside of my range*/
         if(firstMaskBl)
            goto noMask_fun08_sec03_sub02;

         break;
      } /*If: the primer is outside of my range*/

      /*If primer is before sequence*/
      if(forEndUI < samSTPtr->refStartUI)
         goto nextPrim_fun08_sec02_sub07;
      
      /**************************************************\
      * Fun-08 Sec-02 Sub-02:
      *   - Check primers paired, get reverse primer
      \**************************************************/

      pairPrimBl =
         getPairPrimFlag(primFlagAryUI[primIndexSI]);

      revPrimBl =
         getDirPrimFlag(primFlagAryUI[primIndexSI]);

      if(pairPrimBl)
      { /*If: These primers are paired*/
         /*If this is an pair, then I have already checked
         `  the reverse complement or will check the
         `  the reverse complement primer
         */
         if(revPrimBl)
            goto nextPrim_fun08_sec02_sub07;

         revIndexSI =
            getMateIndexPrimFlag(
               primFlagAryUI[primIndexSI]
            ); /*Get the index of the reverse primer*/

         revStartUI = primStartAryUI[revIndexSI];
         revEndUI = primEndAryUI[revIndexSI];

         if(revStartUI > samSTPtr->refEndUI)
         { /*If: reverse primer is outside of range*/
            if(firstMaskBl)
               goto noMask_fun08_sec03_sub02;

            break;
         } /*If: reverse primer is outside of range*/
      } /*If: These primers are paired*/

      /**************************************************\
      * Fun-08 Sec-02 Sub-03:
      *   - Check if the primer(s) are in fudge length
      \**************************************************/

      if(fudgeSI > -1)
      { /*If: The user only wants end primers*/
         /*I need to make sure this primer is at the end
         `  of the read + or - users fudge length
         */
         if(pairPrimBl)
         { /*If: This is an paired primer*/
            if(
                 (int) forStartUI - (int) fudgeSI
               > (int) samSTPtr->refStartUI
            ) goto nextPrim_fun08_sec02_sub07;

            if(revEndUI + fudgeSI < samSTPtr->refEndUI)
               goto nextPrim_fun08_sec02_sub07;
         } /*If: This is an paired primer*/

         else if(revPrimBl)
         { /*Else this is an unpaired reverse primer*/
            if(forEndUI + fudgeSI < samSTPtr->refEndUI)
               goto nextPrim_fun08_sec02_sub07;
         } /*Else this is an unpaired reverse primer*/

         else if(
                 (int) forStartUI - (int) fudgeSI
               > (int) samSTPtr->refStartUI
         ) goto nextPrim_fun08_sec02_sub07;
      } /*If: The user only wants end primers*/

      /**************************************************\
      * Fun-08 Sec-02 Sub-04:
      *   - Find the start of the primer
      \**************************************************/

      firstMaskBl = 0; /*At this point masking happens*/

      samEntryFindRefPos(
         samSTPtr, 
         uiCig,
         cigBaseLeftUI,
         forStartUI,
         refPosUI,
         seqPosUI
      );

      uiEndCig = uiCig;
      cigEndBaseLeftUI = cigBaseLeftUI;
      refEndPosUI = refPosUI;
      seqEndPosUI = seqPosUI;

      /**************************************************\
      * Fun-08 Sec-02 Sub-05:
      *   - Mask the primer position
      \**************************************************/

      maskLoop_fun08_sec02_sub05:;

      while(refEndPosUI <= forEndUI)
      { /*Loop: Mask bases and adjust cigar*/
         if(uiEndCig >= samSTPtr->lenCigUI)
            break; /*sequence had part of primer only*/

         switch(samSTPtr->cigTypeStr[uiEndCig])
         { /*Switch: Find the cigar entry type*/
            case 'M':
            case 'X':
            case '=':
            /*Case: match/snp*/
               while(cigEndBaseLeftUI > 0)
               { /*Loop: Mask the bases*/
                  if(refEndPosUI > forEndUI)
                     break; /*Finished masking*/

                  samSTPtr->seqStr[seqEndPosUI] = maskSC;
                  ++seqEndPosUI;
                  ++refEndPosUI;
                  --cigEndBaseLeftUI;
               } /*Loop: Mask the bases*/

               if(cigEndBaseLeftUI < 1)
               { /*If: I used up all the bases*/
                  ++uiEndCig;

                  cigEndBaseLeftUI =
                     samSTPtr->cigValAryI[uiEndCig];
               } /*If: I used up all the bases*/

               break;
            /*Case: match/snp*/

            case 'I':
            /*Case: insertions*/
               while(cigEndBaseLeftUI > 0)
               { /*Loop: Mask the bases*/
                  samSTPtr->seqStr[seqEndPosUI] = maskSC;
                  ++seqEndPosUI;
                  --cigEndBaseLeftUI;
               } /*Loop: Mask the bases*/

               ++uiEndCig;

               cigEndBaseLeftUI =
                     samSTPtr->cigValAryI[uiEndCig];

               break;
            /*Case: insertions*/

            case 'D':
            /*Case: Deletions*/
               refEndPosUI += cigEndBaseLeftUI;

               if(refEndPosUI > (forEndUI + 1))
               { /*If: I have overshot*/
                   cigEndBaseLeftUI =
                      refEndPosUI - forEndUI - 1;

                   refEndPosUI = forEndUI + 1;
               } /*If: I have overshot*/

               else
               { /*Else: I still have more masking*/
                  ++uiEndCig;

                  cigEndBaseLeftUI =
                     samSTPtr->cigValAryI[uiEndCig];
               } /*Else: I still have more masking*/

               break;
            /*Case: Deletions*/
         } /*Switch: Find the cigar entry type*/
      } /*Loop: Mask bases and adjust cigar*/

      /**************************************************\
      * Fun-08 Sec-02 Sub-06:
      *   - For paired primers, set up reverse primer mask
      \**************************************************/

      if(pairPrimBl)
      { /*If: I have primer pairs to check*/
         forStartUI = revStartUI;
         forEndUI = revEndUI;

         samEntryFindRefPos(
            samSTPtr, 
            uiEndCig,
            cigEndBaseLeftUI,
            forStartUI,
            refEndPosUI,
            seqEndPosUI
         );

         pairPrimBl = 0;

         goto maskLoop_fun08_sec02_sub05;
      } /*If: I have primer pairs to check*/

      /**************************************************\
      * Fun-08 Sec-02 Sub-07:
      *   - Move to the next set of primers
      \**************************************************/

      nextPrim_fun08_sec02_sub07:;

      ++primIndexSI;
   } /*Loop: Mask primers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Return back the answer
   ^   o fun-08 sec-03 sub-01:
   ^     - Return success
   ^   o fun-08 sec-03 sub-02:
   ^     - Return no primers masked
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-03 Sub-01:
   *   - Return success
   \*****************************************************/

   return -firstMaskBl; /*Masking was done*/
   /*-1 if firstMaskBl is 1; else 0*/

   /*****************************************************\
   * Fun-08 Sec-03 Sub-02:
   *   - Return no primers masked
   \*****************************************************/

   noMask_fun08_sec03_sub02:;

   if(primIndexSI < 0) 
      retIndexSI = (sint) -1;
   else
      retIndexSI = (((sint) primIndexSI) << 1) | 1;

   return retIndexSI;
} /*maskPrimers*/


/*-------------------------------------------------------\
| Fun-09: swapPrimCoords
|   - swaps two primer coordinates values around and
|     updates the index's
| Input:
|   - firstIndex:
|     o first (new second) index to swap
|   - secIndex:
|     o second (new first) index to swap
|   - startAry:
|     o array of starting coordinates
|   - endAry:
|     o array of ending coordinates
|   - flagAry:
|     o array of primFlags with flags and the mate index
| Ouput:
|   - Modifies:
|     o startAry and endAry to hold the swapped values
|     o flagAry to hold the swapped values and adjusts
|       their mates index
\-------------------------------------------------------*/
#define \
swapPrimCoords( \
   firstIndex, \
   secIndex, \
   startAry, \
   endAry, \
   flagAry \
){ \
   uint swapMacUI = 0; \
   \
   swapVal( \
      (startAry)[(firstIndex)], \
      (startAry)[(secIndex)] \
   ); /*swapVal is from gen-shellShort.h*/ \
   \
   swapVal( \
      (endAry)[(firstIndex)], \
      (endAry)[(secIndex)] \
   ); \
   \
   /*Changing the paired index. By doing this step before\
   `  the swap I can keep self pointers pointing to self.\
   `  use self pointers (index's) for single sequence\
   `  entries\
   */\
   swapMacUI = \
      getMateIndexPrimFlag( \
         (flagAry)[(firstIndex)] \
      ); \
     \
   setMateIndexPrimFlag( \
      (flagAry)[swapMacUI], \
      (secIndex) \
   ); /*Update the new mate index*/ \
   \
   swapMacUI = \
      getMateIndexPrimFlag( \
         (flagAry)[(secIndex)] \
      ); \
   \
   setMateIndexPrimFlag( \
      (flagAry)[swapMacUI], \
      (firstIndex) \
   ); /*Update the new mate index*/ \
   \
   swapVal( \
      (flagAry)[(firstIndex)], \
      (flagAry)[(secIndex)] \
   ); \
} /*swapPrimCoords*/

/*-------------------------------------------------------\
| Fun-10: sortPrimCoords
|  - Sorts the primer coordiantes of a primer flag array
|    setup. Order is least to greates by first starting
|    positon, then ending position
| Input:
|  - startAryUI:
|    o starting coordinates for the primers
|  - endAryUI:
|    o ending coordinates for the primers
|  - flagAryUI:
|    o array full of primFlags with indexs to keep up to
|      date
|  - lenAryUI:
|    o Length of the array
| Output:
|  - Modifies:
|    o startAry array to be sorted form least to greatest
|    o endAry to be kept in order with firstAry
|    o flagAry to be kept in order with firstAry and have
|      any index's updated
\-------------------------------------------------------*/
void
sortPrimCoords(
   unsigned int *startAryUI,
   unsigned int *endAryUI,
   unsigned int *flagAryUI,
   unsigned int lenAryUI 
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-10 TOC:
   '  - Sorts the primer coordiantes of a primer flag
   '    array setup. Order is least to greates by first
   '    starting positon, then ending position
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun-10 sec-01:
   '    - Variable declerations
   '  o fun-10 sec-02:
   '    - Find the number of rounds to sort for
   '  o fun-10 sec-03:
   '    - Sort the coordiantes
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun-10 Sec-01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of sorting rounds*/
  uint subUI = 0;
  uint nextElmUI = 0;
  uint lastElmUI = 0;
  uint elmOnUI = 0;
  
  /*Variables to incurment loops*/
  uint uiIndex = 0;
  uint uiElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun-10 Sec-02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUI = 1; /*Initialzie first array*/
  
  while(subUI < lenAryUI)
     subUI = (3 * subUI) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun-10 Sec-03:
  ^  - Sort the coordinates
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUI > 0)
  { /*loop trhough all sub arrays sort the subarrays*/

    for( 
       uiIndex = 0;
       uiIndex <= subUI;
       ++uiIndex
    ){ /*For each element in the subarray*/

      for(uiElm = uiIndex;
          uiElm + subUI < lenAryUI;
          uiElm += subUI
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmUI = uiElm + subUI;
        
        if(
              startAryUI[uiElm] > startAryUI[nextElmUI]
           || endAryUI[uiElm] > endAryUI[nextElmUI]
        ){ /*If I need to swap an element*/
           swapPrimCoords(
              uiElm,
              nextElmUI,
              startAryUI,
              endAryUI,
              flagAryUI
           ); /*Swap the coorindates around*/

           lastElmUI = uiElm;
           elmOnUI = uiElm;
          
           while(lastElmUI >= subUI)
           { /*loop; move swapped element back*/
              lastElmUI -= subUI;
              
              if(
                startAryUI[elmOnUI] >startAryUI[lastElmUI]
               || endAryUI[elmOnUI] > endAryUI[lastElmUI]
              ) break; /*Positioned the element*/
              
              swapPrimCoords(
                 elmOnUI,
                 lastElmUI,
                 startAryUI,
                 endAryUI,
                 flagAryUI
              ); /*Swap the coorindates around*/
              
              elmOnUI = lastElmUI;
           } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subUI = (subUI - 1) / 3; /*Move to next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sortPrimCoords*/

/*-------------------------------------------------------\
| Fun-11: readPrimCoords
|   - gets primer coordinates and flags from an file
| Input:
|   o primFileStr:
|     - c-string with name of file to get primer
|       coordinates from
|       o ignored\tignored\tpair\tstart\tend\tstart\tend
|         - pair: 1, P, T, or + if primers paired
|         - start: start coordinate of foward/reverse
|           primer
|         - end: end coordinate of foward/reverse primer
|   o startAryUI:
|     - pointer to an unsigned int pointer to hold the
|       starting coordinates for each primer
|   o endAryUI:
|     - pointer to an unsigned int pointer to hold the
|       ending coordinates for each primer
|   o flagAryUI:
|     - pointer to an unsigned int pointer to hold the
|       flags and pair index for each primer
|   o errSL:
|     - pointer to unsigned long to hold the error
|       message
| Output:
|   - Modifies:
|     o startAryUI to have the starting coordiantes
|     o endAryUI to have the ending coordiantes
|     o flagAryUI to have the direction and pair flags
|       and the index of the other primer
|     o errSL to hold the error
|       - 0 for no errors
|       - def_memErr_primMask for an memory error
|       - (siLine << 8) | def_fileErr_primMask for file
|         errors
|       - def_emptyFileErr_primMask for empty file
|  - Returns:
|    o 0 for errors
|    o number of primers extracted
\-------------------------------------------------------*/
signed int
readPrimCoords(
   signed char *primFileStr,
   unsigned int **startAryUI,
   unsigned int **endAryUI,
   unsigned int **flagAryUI,
   signed long *errSL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC:
   '   - gets primer coordinates and flags from an file
   '   o fun-11 sec-01:
   '     - Variable declerations
   '   o fun-11 sec-02:
   '     - Get the number of primers (lines * 2) in file
   '   o fun-11 sec-03:
   '     - Allocate memory for the primers
   '   o fun-11 sec-04:
   '     - Read in the primers
   '   o fun-11 sec-05:
   '     - Clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned short lenBuffUS = 1024;
   signed char buffStr[lenBuffUS];
   signed char *tmpStr = 0;

   signed int numPrimSI = 0;
   signed int siLine = 0;

   schar revBl = 0; /*On the reverse primer*/

   FILE *primFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^   - Get the number of primers (lines * 2) in file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   primFILE = fopen((char *) primFileStr, "r");

   if(!primFILE)
       goto fileErr_fun11_sec05_sub03;

   /*I do not expect line lengths to go beyond 1024 char*/
   while(fgets((char *) buffStr, lenBuffUS, primFILE))
      numPrimSI += 2;

   numPrimSI -= 2; /*Account for the header*/

   fseek(primFILE, 0, SEEK_SET);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-03:
   ^   - Allocate memory for the primers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numPrimSI < 1)
       goto emptyFileErr_fun11_sec05_sub04;
      
   if(*startAryUI)
      free(*startAryUI);

   *startAryUI = 0;

   *startAryUI = malloc(numPrimSI * sizeof(uint));

   if(! *startAryUI)
       goto memErr_fun11_sec05_sub02;

   if(*endAryUI)
      free(*endAryUI);

   *endAryUI = 0;

   *endAryUI = malloc(numPrimSI * sizeof(uint));

   if(! *endAryUI)
       goto memErr_fun11_sec05_sub02;

   if(*flagAryUI)
      free(*flagAryUI);

   *flagAryUI = 0;

   *flagAryUI = malloc(numPrimSI * sizeof(uint));

   if(! *flagAryUI)
       goto memErr_fun11_sec05_sub02;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-04:
   ^   - Read in the primers
   ^   o fun-11 sec-04 sub-01:
   ^     - Get past header and start primer read loop
   ^   o fun-11 sec-04 sub-02:
   ^     - get past reference name
   ^   o fun-11 sec-04 sub-03:
   ^     - get past primer name
   ^   o fun-11 sec-04 sub-04:
   ^     - find if this is an paired primer
   ^   o fun-11 sec-04 sub-05:
   ^     - get primers starting coordinate
   ^   o fun-11 sec-04 sub-06:
   ^     - get primers ending coordinate
   ^   o fun-11 sec-04 sub-07:
   ^     - Move to second primer index and restart loop
   ^   o fun-11 sec-04 sub-08:
   ^     - Get the next line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-11 Sec-04 Sub-01:
   *   - Get past header and start primer read loop
   \*****************************************************/

   tmpStr =
      (schar *)
      fgets((char *) buffStr, lenBuffUS, primFILE);

   numPrimSI = 0;
   siLine = 1;

   while(fgets((char *) buffStr, lenBuffUS, primFILE))
   { /*Loop: Read in each primer coordinate*/
      ++siLine;
      tmpStr = buffStr;

      /**************************************************\
      * Fun-11 Sec-04 Sub-02:
      *   - get past reference name
      \**************************************************/

      while(*tmpStr > 32)
        ++tmpStr; /*Move past the reference name*/

      if(*tmpStr == '\n')
         goto fileErr_fun11_sec05_sub03;

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec05_sub03;

      while(*tmpStr < 33)
      { /*Loop: Find the start of the next entry*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun11_sec05_sub03;
      } /*Loop: Find the start of the next entry*/

      /**************************************************\
      * Fun-11 Sec-04 Sub-03:
      *   - get past primer name
      \**************************************************/

      while(*tmpStr > 32)
        ++tmpStr; /*Move past the primer name*/

      if(*tmpStr == '\n')
         goto fileErr_fun11_sec05_sub03;

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec05_sub03;

      while(*tmpStr < 33)
      { /*Loop: Find the start of the next entry*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun11_sec05_sub03;
      } /*Loop: Find the start of the next entry*/

      /**************************************************\
      * Fun-11 Sec-04 Sub-04:
      *   - find if this is an paired primer
      \**************************************************/

      if(*tmpStr == '1')
         {setPairPrimFlag((*flagAryUI)[numPrimSI], 1);}
      else if( ((*tmpStr) & ~32) == 'P')
         {setPairPrimFlag((*flagAryUI)[numPrimSI], 1);}
      else if( ((*tmpStr) & ~32) == 'T')
         {setPairPrimFlag((*flagAryUI)[numPrimSI], 1);}
      else if( *tmpStr == '+')
         {setPairPrimFlag((*flagAryUI)[numPrimSI], 1);}
      else
         {setPairPrimFlag((*flagAryUI)[numPrimSI], 0);}

      /*Should always be an forward primer*/
      setDirPrimFlag((*flagAryUI)[numPrimSI], 0);

      while(*tmpStr > 32)
        ++tmpStr; /*Move to end of entry*/

      if(*tmpStr == '\n')
         goto fileErr_fun11_sec05_sub03;

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec05_sub03;

      while(*tmpStr < 33)
      { /*Loop: Find the start of the next entry*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun11_sec05_sub03;
      } /*Loop: Find the start of the next entry*/

      /**************************************************\
      * Fun-11 Sec-04 Sub-05:
      *   - get primers starting coordinate
      \**************************************************/

      getPrimInfo_fun11_sec04_sub05:;

      tmpStr =
         (schar *)
         base10StrToUI(
            (char *) tmpStr,
            (*startAryUI)[numPrimSI]
         );

      if(*tmpStr > 32)
         goto fileErr_fun11_sec05_sub03; /*not an entry*/
         
      --((*startAryUI)[numPrimSI]); /*convert to index 0*/

      if(*tmpStr == '\n')
         goto fileErr_fun11_sec05_sub03;

      if(*tmpStr == '\0')
         goto fileErr_fun11_sec05_sub03;

      while(*tmpStr < 33)
      { /*Loop: Find the start of the next entry*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun11_sec05_sub03;
      } /*Loop: Find the start of the next entry*/

      /**************************************************\
      * Fun-11 Sec-04 Sub-06:
      *   - get primers ending coordinate
      \**************************************************/

      tmpStr =
         (schar *)
         base10StrToUI(
           (char *) tmpStr,
           (*endAryUI)[numPrimSI]
         );

      if(*tmpStr > 32)
         goto fileErr_fun11_sec05_sub03; /*Not an entry*/
         
      --((*endAryUI)[numPrimSI]); /*convert to index 0*/

      if((*startAryUI)[numPrimSI] >(*endAryUI)[numPrimSI])
      { /*If: I need to swap start and end*/
         swapVal(
            (*startAryUI)[numPrimSI],
            (*endAryUI)[numPrimSI]
         ); /*from gen-shellSort.h*/
      } /*If: I need to swap start and end*/

      while(*tmpStr < 33)
      { /*Loop: Find the start of the next entry*/
         ++tmpStr;

         if(*tmpStr == '\0')
         { /*If: There is only one primer*/
           
            if(! revBl)
            { /*If: This is not an reverse primer*/
               setMateIndexPrimFlag(
                  (*flagAryUI)[numPrimSI],
                  numPrimSI
               ); /*This allows sorting to never have an
                  `   odd index. It either pionts to self
                  `   or the other paired primer
                  */
            } /*If: This is not an reverse primer*/

            goto nextLine_fun11_sec04_sub08;
         } /*If: There is only one primer*/
      } /*Loop: Find the start of the next entry*/

      /**************************************************\
      * Fun-11 Sec-04 Sub-07:
      *   - Move to second primer index and restart loop
      \**************************************************/

      /*If there was more than two primers*/
      if(revBl)
         goto fileErr_fun11_sec05_sub03;

      ++numPrimSI;

      /*At this point this is not the end of the line*/
      setMateIndexPrimFlag(
         (*flagAryUI)[numPrimSI - 1],
         numPrimSI
      );

      setPairPrimFlag(
         (*flagAryUI)[numPrimSI],
         getPairPrimFlag((*flagAryUI)[numPrimSI - 1])
      );

      setDirPrimFlag(
         (*flagAryUI)[numPrimSI],
         1
      );

      setMateIndexPrimFlag(
         (*flagAryUI)[numPrimSI],
         numPrimSI - 1
      );

      revBl = 1;

      goto getPrimInfo_fun11_sec04_sub05;

      /**************************************************\
      * Fun-11 Sec-04 Sub-08:
      *   - Get the next line
      \**************************************************/

      nextLine_fun11_sec04_sub08:;

      revBl = 0;
      ++numPrimSI;
   } /*Loop: Read in each primer coordinate*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-05:
   ^   - Clean up and return
   ^   o fun-11 sec-05 sub-01:
   ^     - Clean up after no errors
   ^   o fun-11 sec-05 sub-02:
   ^     - Set up the memory error return
   ^   o fun-11 sec-05 sub-03:
   ^     - Set up the file error return
   ^   o fun-11 sec-05 sub-04:
   ^     - Clean up after an error
   \*****************************************************/

   /*****************************************************\
   * Fun-11 Sec-05 Sub-01:
   *   - Clean up after no errors
   \*****************************************************/

   if(primFILE)
      fclose(primFILE);

   primFILE = 0;

   sortPrimCoords(
      *startAryUI,
      *endAryUI,
      *flagAryUI,
      numPrimSI
   ); /*Sort the indexes*/ 

   return numPrimSI;

   /*****************************************************\
   * Fun-11 Sec-05 Sub-02:
   *   - Set up the memory error return
   \*****************************************************/

   memErr_fun11_sec05_sub02:;

   *errSL = def_memErr_primMask;

   goto cleanUp_fun11_sec05_sub05;

   /*****************************************************\
   * Fun-11 Sec-05 Sub-03:
   *   - Set up the file error return
   \*****************************************************/

   fileErr_fun11_sec05_sub03:;

   *errSL = (siLine << 8) | def_fileErr_primMask;

   goto cleanUp_fun11_sec05_sub05;

   /*****************************************************\
   * Fun-11 Sec-05 Sub-04:
   *   - Set up to return empty file error
   \*****************************************************/

   emptyFileErr_fun11_sec05_sub04:;

   *errSL = def_emptyFileErr_primMask;

   goto cleanUp_fun11_sec05_sub05;

   /*****************************************************\
   * Fun-11 Sec-05 Sub-05:
   *   - Clean up after an error
   \*****************************************************/

   cleanUp_fun11_sec05_sub05:;

   if(primFILE)
      fclose(primFILE);

   primFILE = 0;

   free(*startAryUI);
   *startAryUI = 0;

   free(*endAryUI);
   *endAryUI = 0;

   free(*flagAryUI);
   *flagAryUI = 0;

   return 0;
} /*readPrimCoords*/

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
