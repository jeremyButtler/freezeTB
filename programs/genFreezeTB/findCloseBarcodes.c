/*SPDX-License-Identifier: CC0-1.0*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' findCloseBarcodes SOF: Start Of File
'   - has the functions to find the closest barcodes for a
'     spoligotype
'   o header:
'     - included libraries
'   o .c mac01: setDistIndex_findCloseBarcode
'     - adds distance & database index into a signed long
'   o .h mac02: distGet_findCloseBarcode
'     - gets the edit distance from a signed long set with
'       (setDistIndex_findCloseBarcode)
'   o .h mac03: indexGet_findCloseBarcode
'     - gets the index from a signed long set with
'       (setDistIndex_findCloseBarcode)
'   o fun01: barcodeToCode_findCloseBarcode
'     - converts a spoligotype barcode to an unsigned long
'   o .c fun02: sortDistances_findCloseBarcodes
'     - sorts the distance array by closest match to
'       farthest
'   o fun03: getDistances_findCloseBarcode
'     - get every spoligotype lineage with x distance from
'       the input barcode
'   o fun04: codeToBarcode_findCloseBarcode
'     - convert a code stored in an unsigned long
'       (returned from barcodeToCode_findCloseBarcode) to
'       a c-string barcode
'   o fun05: pCloseLineages_findCloseBarcode
'     - print the detected close lineages found by
'       getDistances_findCloseBarcode
' This file is released into the Public Domain under
'   CC0-1.0.
' See https://creativecommons.org/publicdomain/zero/1.0/
'   for details (or ../../license-CC-1.0.txt).
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

#include "findCloseBarcodes.h"
#include "../genFreezeTB/spolST.h"

/*these files are only .h files or only .h file used*/
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/64bit.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "../genFreezeTB/tbSpolDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: barcodeToCode_findCloseBarcodes
|   - converts a spoligotype barcode to an unsigned long
| Input:
|   - barStr:
|     o c-string with barcode to convert
| Output:
|   - Returns:
|     o unsigned long with barcoded
\-------------------------------------------------------*/
unsigned long
barcodeToCode_findCloseBarcodes(
   signed char *barStr
){
   unsigned char ucSpace = 0;
   unsigned long barcodeUL = 0;

   for(ucSpace = 0; ucSpace < 43; ++ucSpace)
   { /*Loop: convert barcode to code*/
      barcodeUL <<= 1;
      if( (barStr[ucSpace] | 32) == 'i')
         barcodeUL |= 1;
      else if( (barStr[ucSpace] | 32) == 'n')
         barcodeUL |= 1;
      else if(barStr[ucSpace] == '1')
         barcodeUL |= 1;
   } /*Loop: convert barcode to code*/

   return barcodeUL;
} /*barcodeToCode_findCloseBarcodes*/

/*-------------------------------------------------------\
| Mac01: setDistIndex_findCloseBarcodes
|   - adds distance and database index into a signed long
| Input:
|   - indexMacSI:
|     o index to add to the signed long
|   - distanceMacSC:
|     o distance (signed char) to add to the signed long
| Output:
|   - Returns:
|     o signed long with index and distance
\-------------------------------------------------------*/
#define setDistIndex_findCloseBarcodes(indexMacSI, distanceMacSC) ( (signed long) ((((signed long) (indexMacSI)) << 8) | ((distanceMacSC) & 0xff)) )
   /*Logic:
   `   - movedIndex: indexSIMac << 8:
   `     o moves index up 8 bits, adds room for distance
   `   - distance: distanceMacSC & 0xff
   `     o converts distance to a signed char
   `   - movedIndex | distance:
   `     o sets up signed long to have distance and index
   */

/*-------------------------------------------------------\
| Fun02: sortDistances_findCloseBarcodes
|   - sorts the distance array by closest match to
|     farthest
| Input:
|   - distArySL:
|     o distance array to sort
|   - lenSI:
|     o length of distArySL (index 1)
| Output:
|   - Modifies:
|     o distArySL to be sorted form least distance to
|       greatest distance
\-------------------------------------------------------*/
void
sortDistances_findCloseBarcodes(
   signed long *distArySL,
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - sorts the distance array by closest match to
   '     farthest
   '   o fun02 sec01:
   '     - variable declerations
   '   o fun02 sec02:
   '     - find the number of rounds to sort for
   '   o fun02 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char distOneSC = 0;
   signed char distTwoSC = 0;

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int indexSI = 0;
   signed int elmSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! lenSI)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(indexSI = 0; indexSI <= subSI; ++indexSI)
      { /*Loop: though sub array*/
         elmSI = indexSI;

         for( ; elmSI + subSI < lenSI; elmSI += subSI)
         { /*Loop: swap elements in subarray*/
            nextSI = elmSI + subSI;

            distOneSC =
               distGet_findCloseBarcodes(
                  distArySL[elmSI]
               );
            distTwoSC =
               distGet_findCloseBarcodes(
                  distArySL[nextSI]
               );
            if(distOneSC > distTwoSC)
            { /*If I need to swap an element*/
               distArySL[elmSI] ^= distArySL[nextSI];
               distArySL[nextSI] ^= distArySL[elmSI];
               distArySL[elmSI] ^= distArySL[nextSI];

               lastSI = elmSI;
               onSI = elmSI;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;
                  distOneSC =
                     distGet_findCloseBarcodes(
                        distArySL[onSI]
                     );
                  distTwoSC =
                     distGet_findCloseBarcodes(
                        distArySL[lastSI]
                     );

                  if(distOneSC > distTwoSC)
                     break; /*Positioned the element*/
   
                  distArySL[onSI] ^= distArySL[lastSI];
                  distArySL[lastSI] ^= distArySL[onSI];
                  distArySL[onSI] ^= distArySL[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortDistances_findCloseBarcodes*/

/*-------------------------------------------------------\
| Fun03: getDistances_findCloseBarcodes
|   - get every spoligotype lineage with x distance from
|     the input barcode
| Input:
|   - barStr:
|     o c-string with barcode to convert
|     o valid characters:
|       * spacer persent: I, i, N, n, or 1
|       * spacer missing: any chacter that is not present
|   - maxDistSC:
|     o maximum edit distance to count lineage as close
|   - retLenSIPtr:
|     o signed int pointer to be set to the number of
|       lineages that were close to barStr
|   - spolAryST:
|     o spolST struct array with lineages to compare to
|   - numSpolSI:
|     o number lineages in spolAryST
| Output:
|   - Modifies:
|     o retLenSIPtr to have the number of close lineages
|       * set to -1 for a memory error
|   - Returns:
|     o signed long array with lineage index (in
|       spolAryST) and edit distance
|       * use indexGet_findCloseBarcodes(array[index]) to
|         get the lineage index
|       * use distGet_findCloseBarcodes(array[index]) to
|         get the edit distance from a lineage
|       * length of returned array is set in retLenSIPtr
|     o 0 for no lineages or a memory error
\-------------------------------------------------------*/
signed long *
getDistances_findCloseBarcodes(
   signed char *barStr,      /*barcode to search for*/
   signed char maxDistSC,    /*max distance to keep*/
   signed int *retLenSIPtr,  /*number of hits found*/
   struct spolST *spolAryST, /*spoligotype database*/
   signed int numSpolSI      /*number of spoligotypes*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - get every spoligotype lineage with x distance
   '     from the input barcode
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - find the distances
   '   o fun03 sec03:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned long codeUL = 0;
   unsigned long cmpUL = 0;
   unsigned long barcodeUL = 0;

   signed int siSpol = 0;
   unsigned char ucSpace = 0;
   signed char distSC = 0;

   signed long *tmpArySL = 0;
   signed long *distHeapArySL = 0;
   signed int distSizeSI = 16;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - find the distances
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *retLenSIPtr = 0;

   distHeapArySL =
      malloc(distSizeSI * sizeof(signed long));
   if(! distHeapArySL)
      goto memErr_fun03_sec0x;

   barcodeUL = barcodeToCode_findCloseBarcodes(barStr);
   for(siSpol = 0; siSpol < numSpolSI; ++siSpol)
   { /*Loop: find edit distance*/
      distSC = 0;

      codeUL = spolAryST[siSpol].codeUL;
      cmpUL = barcodeUL;

      for(ucSpace = 0; ucSpace < 43; ++ucSpace)
      { /*Loop: compare to spoligotype*/
         distSC += ((codeUL & 1) ^ (cmpUL & 1));
         codeUL >>= 1;
         cmpUL >>= 1;
      } /*Loop: compare to spoligotype*/

      if(distSC <= maxDistSC)
      { /*If: distance within limits*/
         if(*retLenSIPtr >= distSizeSI)
         { /*If: need more memory*/
            distSizeSI += (distSizeSI >> 2);
            tmpArySL =
               realloc(
                  distHeapArySL,
                  distSizeSI * sizeof(signed long)
               );
            if(! tmpArySL)
               goto memErr_fun03_sec0x;
            distHeapArySL = tmpArySL;
            tmpArySL = 0;
         } /*If: need more memory*/

         distHeapArySL[*retLenSIPtr] =
            setDistIndex_findCloseBarcodes(siSpol,distSC);
         ++(*retLenSIPtr);
      } /*If: distance within limits*/
   } /*Loop: find edit distance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortDistances_findCloseBarcodes(
      distHeapArySL,
      *retLenSIPtr
   );

   return distHeapArySL;

   memErr_fun03_sec0x:;
      if(distHeapArySL)
         free(distHeapArySL);
      distHeapArySL = 0;
      *retLenSIPtr = -1;
      return 0;
} /*getDistances_findCloseBarcodes*/

/*-------------------------------------------------------\
| Fun04: codeToBarcode_findCloseBarcodes
|   - convert a code stored in an unsigned long (returned
|     from barcodeToCode_findCloseBarcodes) to a c-string
|     barcode
| Input:
|   - codeUL:
|     o return value from barcodeToCode_findCloseBarcodes
|       * unsigned long with bits set to 1 or 0 for
|         each spacer
|   - barStr:
|     o c-string with 44 elements to store the barcode in
| Output:
|   - Modifies:
|     o barStr to have 'o' for missing spacers and 'I' for
|       present spacers in codeUL
\-------------------------------------------------------*/
void
codeToBarcode_closetSpol(
   unsigned long codeUL,
   signed char *barStr   /*44 element c-string*/
){
   signed char scSpacer = 0;

   barStr[43] = 0;
   for(scSpacer = 42; scSpacer >= 0; --scSpacer)
   { /*Loop: build the barcode*/
      if(codeUL & 1)
         barStr[scSpacer] = 'I';
      else
         barStr[scSpacer] = 'o';
      codeUL >>= 1;
   } /*Loop: build the barcode*/
} /*codeToBarcode_cloestSpol*/

/*-------------------------------------------------------\
| Fun05: pCloseLineages_findCloseBarcodes
|   - print the detected close lineages found by
|     getDistances_findCloseBarcodes
| Input:
|   - barStr:
|     o c-stirng with barcode to print results for
|   - distArySL:
|     o signed long array wit edit distance and lineage
|       distances from getDistances_findCloseBarcodes
|   - distLenSI:
|     o number of distances found
|   - spolAryST:
|     o spolST struct array with lineages that were
|       searched
|   - outFILE:
|     o FILE pointer to file to print output to
| Output:
|   - Prints:
|     o lineages ind distArySL to outFILE
|       * lineage_strain\tlineage\tdistance\tbarcode
|       * first line is a header
|       * for barcode, uppercase characters are matches,
|         lower case characters are missmatches
\-------------------------------------------------------*/
void
pCloseLineages_findCloseBarcodes(
   signed char *barStr,      /*barcode looked for*/
   signed long *distArySL,   /*has distance and index*/
   signed int distLenSI,     /*number of matches*/
   struct spolST *spolAryST, /*spoligotype database*/
   void *outFILE
){
   signed int indexSI = 0;
   signed int siDist = 0;
   signed char distSC = 0;
   signed char outBarStr[44];
   unsigned long codeUL = 0;
   unsigned long barCodeUL = 0;
   unsigned long cmpCodeUL = 0;

   signed int setMissSI = 0;
   signed int delMissSI = 0;

   codeUL = barcodeToCode_findCloseBarcodes(barStr);

   fprintf(
      (FILE *) outFILE,
      "id\tlineage\tdistance\tdelMiss\tsetMiss"
   );
   fprintf((FILE *) outFILE, "\tbarcode%s", str_endLine);

   if(! distLenSI)
      return ;
   else
   { /*Else: print all close matches*/
      for(siDist = 0; siDist < distLenSI; ++siDist)
      { /*Loop: print close hits*/
         indexSI =
            indexGet_findCloseBarcodes(distArySL[siDist]);
         cmpCodeUL = spolAryST[indexSI].codeUL;
         barCodeUL = codeUL;

         setMissSI = 0;
         delMissSI = 0;
 
         for(distSC = 42; distSC >= 0; --distSC)
         { /*Loop: convert output barcode to show diffs*/
            switch((barCodeUL &1) | ((cmpCodeUL &1) <<1))
            { /*Switch: check if match*/
               case 0:
                  outBarStr[distSC] = 'O';
                  break;
               case 1:
                   outBarStr[distSC] = 'i';
                   ++delMissSI;
                   break;
               case 2:
                   outBarStr[distSC] = 'o';
                   ++setMissSI;
                   break;
               case 3:
                   outBarStr[distSC] = 'I';
                   break;
            } /*Switch: check if match*/

            barCodeUL >>= 1;
            cmpCodeUL >>= 1;
         } /*Loop: convert output barcode to show diffs*/

         outBarStr[43] = 0;
         distSC =
            distGet_findCloseBarcodes(distArySL[siDist]);

         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%i\t%i\t%i\t%s%s",
            spolAryST[indexSI].idStr,
            spolAryST[indexSI].lineageStr,
            distSC,
            setMissSI,
            delMissSI,
            outBarStr,
            str_endLine
         );
      } /*Loop: print close hits*/
   } /*Else: print all close matches*/
} /*pCloseLineages_findCloseBarcodes*/
