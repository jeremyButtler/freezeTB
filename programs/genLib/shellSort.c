/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' shellSort SOF: Start Of File
'   - holds functions for doing shell sorts on arrays
'   o note:
'     - most of this code is from
'        o Adam Drozdek. 2013. Data Structures and
'          Algorithims in c++. Cengage Leraning. fourth
'          edition. pages 505-508
'   o note: this is long, but is really a set of three
'     functions per data type
'     o xxSearch_shellSort
'       - search for query in a sorted array
'     o xxRange_shellSort
'       - search for start of range in a sorted array
'     o xx_shellSort
'       - sort an array by lowest value first
'   o header:
'     - included libraries
'   o fun01: ulSearch_shellSort
'     - search for query in unsigned long array
'   o fun02: ulRange_shellSort
'     - searches for range in unsinged long array
'   o fun03: ul_shellSort
'     - sorts a unsigned long array from least to greatest
'   o fun04: uiSearch_shellSort
'     - search for query in unsigned int array
'   o fun05: uiRange_shellSort
'     - searches for range in unsinged int array
'   o fun06: ui_shellSort
'     - sorts a unsigned int array from least to greatest
'   o fun07: us_Search_shellSort
'     - search for query in unsigned short array
'   o fun08: us_Range_shellSort
'     - searches for range in unsinged short array
'   o fun09: us_shellSort
'     - sorts unsigned short array from least to greatest
'   o fun10: uc_Search_shellSort
'     - search for query in unsigned char array
'   o fun11: uc_Range_shellSort
'     - searches for range in unsinged char array
'   o fun12: sl_shellSort
'     - sorts signed long array from least to greatest
'   o fun13: sl_Search_shellSort
'     - search for query in signed long array
'   o fun14: sl_Range_shellSort
'     - searches for range in singed long array
'   o fun15: sl_shellSort
'     - sorts signed long array from least to greatest
'   o fun16: si_Search_shellSort
'     - search for query in signed int array
'   o fun17: si_Range_shellSort
'     - searches for range in singed int array
'   o fun18: si_shellSort
'     - sorts signed int array from least to greatest
'   o fun19: ss_Search_shellSort
'     - search for query in signed short array
'   o fun20: ss_Range_shellSort
'     - searches for range in singed short array
'   o fun21: ss_shellSort
'     - sorts signed short array from least to greatest
'   o fun22: sc_Search_shellSort
'     - search for query in signed char array
'   o fun23: sc_Range_shellSort
'     - searches for range in singed char array
'   o fun24: sc_shellSort
'     - sorts signed char array from least to greatest
'   o fun25: dbl_shellSort
'     - sorts a double array from least to greatest
'   o fun26: uiTwinShort_kmerFind
'     - sorts a unsigned int array from least to greatest
'       and keeps it linked to a second unsigned int array
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "shellSort.h"

/*-------------------------------------------------------\
| Fun01: ulSearch_shellSort
|  - search for query in unsigned long array
| Input:
|  - ulAry:
|    o to unsigned int array
|  - qryUL:
|    o number to find
|  - lenArySL:
|    o length of ulAry (index 1)
| Output:
|  - Returns:
|    o index of qryUL in ulAry
|    o -1 if qryUL is not in ulAry
\-------------------------------------------------------*/
signed long
ulSearch_shellSort(
   unsigned long *ulAry,
   unsigned long qryUL,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qryUL > ulAry[midSL])
         leftSL = midSL + 1;

     else if(qryUL < ulAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*ulSearch_shellSort*/

/*-------------------------------------------------------\
| Fun02: ulRange_shellSort
|   - searches for range in unsinged long array
| Input:
|   - ulAry:
|     o unsigned long array
|   - startUL:
|     o start of the range to look for
|   - endUL:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ulRange_shellSort(
   unsigned long *ulAry,
   unsigned long startUL,
   unsigned long endUL,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startUL > ulAry[midSL])
         leftSL = midSL + 1;

     else
         rightSL = midSL - 1;
         /*value was in range greater than range
         `   - if in range; find start of range
         `   - if out of range; get higher number
         */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startUL > ulAry[midSL])
      return -1;
   if(endUL < ulAry[midSL])
      return -1;

   return midSL;
} /*ulRange_shellSort*/

/*-------------------------------------------------------\
| Fun03: ul_shellSort
|   - sorts a unsigned long array from least to greatest
| Input:
|   - ulAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ulAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ul_shellSort(
   unsigned long *ulAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - find the number of rounds to sort for
   '   o fun03 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ulIndex = 0;
   unsigned long ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            if(ulAry[ulElm] > ulAry[nextUL])
            { /*If I need to swap an element*/
               ulAry[ulElm] ^= ulAry[nextUL];
               ulAry[nextUL] ^= ulAry[ulElm];
               ulAry[ulElm] ^= ulAry[nextUL];

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(ulAry[onUL] > ulAry[lastUL])
                     break; /*Positioned the element*/
   
                  ulAry[onUL] ^= ulAry[lastUL];
                  ulAry[lastUL] ^= ulAry[onUL];
                  ulAry[onUL] ^= ulAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*ul_shellSort*/

/*-------------------------------------------------------\
| Fun04: uiSearch_shellSort
|  - search for query in unsigned int array
| Input:
|  - uiAry:
|    o unsigned int array
|  - qryUI:
|    o number to find
|  - lenArySL:
|    o length of uiAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in uiAry
|    o -1 if qryUI is not in uiAry
\-------------------------------------------------------*/
signed long
uiSearch_shellSort(
   unsigned int *uiAry,
   unsigned int qryUI,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qryUI > uiAry[midSL])
         leftSL = midSL + 1;

     else if(qryUI < uiAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*uiSearch_shellSort*/

/*-------------------------------------------------------\
| Fun05: uiRange_shellSort
|   - searches for range in unsinged int array
| Input:
|   - uiAry:
|     o unsigned int array
|   - startUI:
|     o start of the range to look for
|   - endUI:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
uiRange_shellSort(
   unsigned int *uiAry,
   unsigned int startUI,
   unsigned int endUI,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startUI > uiAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startUI > uiAry[midSL])
      return -1;
   if(endUI < uiAry[midSL])
      return -1;

   return midSL;
} /*uiRange_shellSort*/

/*-------------------------------------------------------\
| Fun06: ui_shellSort
|   - sorts a unsigned int array from least to greatest
| Input:
|   - uiAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o uiAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ui_shellSort(
   unsigned int *uiAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - find the number of rounds to sort for
   '   o fun06 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long uiIndex = 0;
   unsigned long uiElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formuia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         uiIndex = 0;
         uiIndex <= subUL;
         ++uiIndex
      ){ /*Loop: though sub array*/
         uiElm = uiIndex;

         for(
            uiElm = uiIndex;
            uiElm + subUL <= endUL;
            uiElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = uiElm + subUL;

            if(uiAry[uiElm] > uiAry[nextUL])
            { /*If I need to swap an element*/
               uiAry[uiElm] ^= uiAry[nextUL];
               uiAry[nextUL] ^= uiAry[uiElm];
               uiAry[uiElm] ^= uiAry[nextUL];

               lastUL = uiElm;
               onUL = uiElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(uiAry[onUL] > uiAry[lastUL])
                     break; /*Positioned the element*/
   
                  uiAry[onUL] ^= uiAry[lastUL];
                  uiAry[lastUL] ^= uiAry[onUL];
                  uiAry[onUL] ^= uiAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*ui_shellSort*/

/*-------------------------------------------------------\
| Fun07: usSearch_shellSort
|  - search for query in unsigned short array
| Input:
|  - usAry:
|    o unsigned short array
|  - qryUS:
|    o number to find
|  - lenArySL:
|    o length of usAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in usAry
|    o -1 if qryUI is not in usAry
\-------------------------------------------------------*/
signed long
usSearch_shellSort(
   unsigned short *usAry,
   unsigned short qryUS,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qryUS > usAry[midSL])
         leftSL = midSL + 1;

     else if(qryUS < usAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*usSearch_shellSort*/

/*-------------------------------------------------------\
| Fun08: usRange_shellSort
|   - searches for range in unsinged short array
| Input:
|   - usAry:
|     o unsigned short array
|   - startUS:
|     o start of the range to look for
|   - endUS:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
usRange_shellSort(
   unsigned short *usAry,
   unsigned short startUS,
   unsigned short endUS,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startUS > usAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startUS > usAry[midSL])
      return -1;
   if(endUS < usAry[midSL])
      return -1;

   return midSL;
} /*usRange_shellSort*/

/*-------------------------------------------------------\
| Fun09: us_shellSort
|   - sorts a unsigned short array from least to greatest
| Input:
|   - usAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o usAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
us_shellSort(
   unsigned short *usAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - sorts signed short array from least to greatest
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

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long usIndex = 0;
   unsigned long usElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formusa: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         usIndex = 0;
         usIndex <= subUL;
         ++usIndex
      ){ /*Loop: though sub array*/
         usElm = usIndex;

         for(
            usElm = usIndex;
            usElm + subUL <= endUL;
            usElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = usElm + subUL;

            if(usAry[usElm] > usAry[nextUL])
            { /*If I need to swap an element*/
               usAry[usElm] ^= usAry[nextUL];
               usAry[nextUL] ^= usAry[usElm];
               usAry[usElm] ^= usAry[nextUL];

               lastUL = usElm;
               onUL = usElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(usAry[onUL] > usAry[lastUL])
                     break; /*Positioned the element*/
   
                  usAry[onUL] ^= usAry[lastUL];
                  usAry[lastUL] ^= usAry[onUL];
                  usAry[onUL] ^= usAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*us_shellSort*/

/*-------------------------------------------------------\
| Fun10: ucSearch_shellSort
|  - search for query in unsigned char array
| Input:
|  - ucAry:
|    o unsigned char array
|  - qryUC:
|    o number to find
|  - lenArySL:
|    o length of ucAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in ucAry
|    o -1 if qryUI is not in ucAry
\-------------------------------------------------------*/
signed long
ucSearch_shellSort(
   unsigned char *ucAry,
   unsigned char qryUC,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qryUC > ucAry[midSL])
         leftSL = midSL + 1;

     else if(qryUC < ucAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*ucSearch_shellSort*/

/*-------------------------------------------------------\
| Fun11: ucRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - ucAry:
|     o unsigned char array
|   - startUC:
|     o start of the range to look for
|   - endUC:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ucRange_shellSort(
   unsigned char *ucAry,
   unsigned char startUC,
   unsigned char endUC,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startUC > ucAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startUC > ucAry[midSL])
      return -1;
   if(endUC < ucAry[midSL])
      return -1;

   return midSL;
} /*ucRange_shellSort*/

/*-------------------------------------------------------\
| Fun12: uc_shellSort
|   - sorts a unsigned char array from least to greatest
| Input:
|   - ucAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ucAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
uc_shellSort(
   unsigned char *ucAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun12 sec01:
   '     - variable declerations
   '   o fun12 sec02:
   '     - find the number of rounds to sort for
   '   o fun12 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ucIndex = 0;
   unsigned long ucElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formuca: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ucIndex = 0;
         ucIndex <= subUL;
         ++ucIndex
      ){ /*Loop: though sub array*/
         ucElm = ucIndex;

         for(
            ucElm = ucIndex;
            ucElm + subUL <= endUL;
            ucElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ucElm + subUL;

            if(ucAry[ucElm] > ucAry[nextUL])
            { /*If I need to swap an element*/
               ucAry[ucElm] ^= ucAry[nextUL];
               ucAry[nextUL] ^= ucAry[ucElm];
               ucAry[ucElm] ^= ucAry[nextUL];

               lastUL = ucElm;
               onUL = ucElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(ucAry[onUL] > ucAry[lastUL])
                     break; /*Positioned the element*/
   
                  ucAry[onUL] ^= ucAry[lastUL];
                  ucAry[lastUL] ^= ucAry[onUL];
                  ucAry[onUL] ^= ucAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*uc_shellSort*/

/*-------------------------------------------------------\
| Fun13: slSearch_shellSort
|  - search for query in signed long array
| Input:
|  - slAry:
|    o signed long array
|  - qrySL:
|    o number to find
|  - lenArySL:
|    o length of slAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in slAry
|    o -1 if qryUI is not in slAry
\-------------------------------------------------------*/
signed long
slSearch_shellSort(
   signed long *slAry,
   signed long qrySL,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qrySL > slAry[midSL])
         leftSL = midSL + 1;

     else if(qrySL < slAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*slSearch_shellSort*/

/*-------------------------------------------------------\
| Fun14: slRange_shellSort
|   - searches for range in singed long array
| Input:
|   - slAry:
|     o signed long array
|   - startSL:
|     o start of the range to look for
|   - endSL:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
slRange_shellSort(
   signed long *slAry,
   signed long startSL,
   signed long endSL,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startSL > slAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startSL > slAry[midSL])
      return -1;
   if(endSL < slAry[midSL])
      return -1;

   return midSL;
} /*slRange_shellSort*/

/*-------------------------------------------------------\
| Fun15: sl_shellSort
|   - sorts a signed long array from least to greatest
| Input:
|   - slAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o slAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
sl_shellSort(
   signed long *slAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun15 sec01:
   '     - variable declerations
   '   o fun15 sec02:
   '     - find the number of rounds to sort for
   '   o fun15 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long slIndex = 0;
   unsigned long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formsla: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         slIndex = 0;
         slIndex <= subUL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subUL <= endUL;
            slElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = slElm + subUL;

            if(slAry[slElm] > slAry[nextUL])
            { /*If I need to swap an element*/
               slAry[slElm] ^= slAry[nextUL];
               slAry[nextUL] ^= slAry[slElm];
               slAry[slElm] ^= slAry[nextUL];

               lastUL = slElm;
               onUL = slElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(slAry[onUL] > slAry[lastUL])
                     break; /*Positioned the element*/
   
                  slAry[onUL] ^= slAry[lastUL];
                  slAry[lastUL] ^= slAry[onUL];
                  slAry[onUL] ^= slAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sl_shellSort*/

/*-------------------------------------------------------\
| Fun16: siSearch_shellSort
|  - search for query in signed int array
| Input:
|  - siAry:
|    o signed int array
|  - qrySI:
|    o number to find
|  - lenArySL:
|    o length of siAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in siAry
|    o -1 if qryUI is not in siAry
\-------------------------------------------------------*/
signed long
siSearch_shellSort(
   signed int *siAry,
   signed int qrySI,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qrySI > siAry[midSL])
         leftSL = midSL + 1;

     else if(qrySI < siAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*siSearch_shellSort*/

/*-------------------------------------------------------\
| Fun17: siRange_shellSort
|   - searches for range in singed int array
| Input:
|   - siAry:
|     o signed int array
|   - startSI:
|     o start of the range to look for
|   - endSI:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
siRange_shellSort(
   signed int *siAry,
   signed int startSI,
   signed int endSI,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startSI > siAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startSI > siAry[midSL])
      return -1;
   if(endSI < siAry[midSL])
      return -1;

   return midSL;
} /*siRange_shellSort*/

/*-------------------------------------------------------\
| Fun18: si_shellSort
|   - sorts a signed int array from least to greatest
| Input:
|   - siAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o siAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
si_shellSort(
   signed int *siAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun18 sec01:
   '     - variable declerations
   '   o fun18 sec02:
   '     - find the number of rounds to sort for
   '   o fun18 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long siIndex = 0;
   unsigned long siElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formsia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         siIndex = 0;
         siIndex <= subUL;
         ++siIndex
      ){ /*Loop: though sub array*/
         siElm = siIndex;

         for(
            siElm = siIndex;
            siElm + subUL <= endUL;
            siElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = siElm + subUL;

            if(siAry[siElm] > siAry[nextUL])
            { /*If I need to swap an element*/
               siAry[siElm] ^= siAry[nextUL];
               siAry[nextUL] ^= siAry[siElm];
               siAry[siElm] ^= siAry[nextUL];

               lastUL = siElm;
               onUL = siElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(siAry[onUL] > siAry[lastUL])
                     break; /*Positioned the element*/
   
                  siAry[onUL] ^= siAry[lastUL];
                  siAry[lastUL] ^= siAry[onUL];
                  siAry[onUL] ^= siAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*si_shellSort*/

/*-------------------------------------------------------\
| Fun19: ssSearch_shellSort
|  - search for query in signed short array
| Input:
|  - ssAry:
|    o signed short array
|  - qrySS:
|    o number to find
|  - lenArySL:
|    o length of ssAry (index 1)
| Output:
|  - Returns:
|    o index of qrySS in ssAry
|    o -1 if qrySS is not in ssAry
\-------------------------------------------------------*/
signed long
ssSearch_shellSort(
   signed short *ssAry,
   signed short qrySS,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qrySS > ssAry[midSL])
         leftSL = midSL + 1;

     else if(qrySS < ssAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*ssSearch_shellSort*/

/*-------------------------------------------------------\
| Fun20: ssRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - ssAry:
|     o signed short array
|   - startSS:
|     o start of the range to look for
|   - endSS:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ssRange_shellSort(
   signed short *ssAry,
   signed short startSS,
   signed short endSS,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startSS > ssAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startSS > ssAry[midSL])
      return -1;
   if(endSS < ssAry[midSL])
      return -1;

   return midSL;
} /*ssRange_shellSort*/

/*-------------------------------------------------------\
| Fun21: ss_shellSort
|   - sorts a signed short array from least to greatest
| Input:
|   - ssAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ssAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ss_shellSort(
   signed short *ssAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun21 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun21 sec01:
   '     - variable declerations
   '   o fun21 sec02:
   '     - find the number of rounds to sort for
   '   o fun21 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ssIndex = 0;
   unsigned long ssElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formssa: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ssIndex = 0;
         ssIndex <= subUL;
         ++ssIndex
      ){ /*Loop: though sub array*/
         ssElm = ssIndex;

         for(
            ssElm = ssIndex;
            ssElm + subUL <= endUL;
            ssElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ssElm + subUL;

            if(ssAry[ssElm] > ssAry[nextUL])
            { /*If I need to swap an element*/
               ssAry[ssElm] ^= ssAry[nextUL];
               ssAry[nextUL] ^= ssAry[ssElm];
               ssAry[ssElm] ^= ssAry[nextUL];

               lastUL = ssElm;
               onUL = ssElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(ssAry[onUL] > ssAry[lastUL])
                     break; /*Positioned the element*/
   
                  ssAry[onUL] ^= ssAry[lastUL];
                  ssAry[lastUL] ^= ssAry[onUL];
                  ssAry[onUL] ^= ssAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*ss_shellSort*/

/*-------------------------------------------------------\
| Fun22: scSearch_shellSort
|  - search for query in signed char array
| Input:
|  - scAry:
|    o signed char array
|  - qrySC:
|    o number to find
|  - lenArySL:
|    o length of scAry (index 1)
| Output:
|  - Returns:
|    o index of qrySC in scAry
|    o -1 if qrySC is not in scAry
\-------------------------------------------------------*/
signed long
scSearch_shellSort(
   signed char *scAry,
   signed char qrySC,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(qrySC > scAry[midSL])
         leftSL = midSL + 1;

     else if(qrySC < scAry[midSL])
         rightSL = midSL - 1;

     else
        return midSL;
   } /*Loop: Search for the querys index*/

   return -1; /*query not found*/
} /*scSearch_shellSort*/

/*-------------------------------------------------------\
| Fun23: scRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - scAry:
|     o signed char array
|   - startSC:
|     o start of the range to look for
|   - endSC:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
scRange_shellSort(
   signed char *scAry,
   signed char startSC,
   signed char endSC,
   signed long lenArySL
){
   signed long midSL = 0;
   signed long rightSL = lenArySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(startSC > scAry[midSL])
        leftSL = midSL + 1;

     else
        rightSL = midSL - 1;
        /*value was in range greater than range
        `   - if in range; find start of range
        `   - if out of range; get higher number
        */
   } /*Loop: Search for the querys index*/

   /*check if found range*/
   if(startSC > scAry[midSL])
      return -1;
   if(endSC < scAry[midSL])
      return -1;

   return midSL;
} /*scRange_shellSort*/

/*-------------------------------------------------------\
| Fun24: sc_shellSort
|   - sorts a signed char array from least to greatest
| Input:
|   - scAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o scAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
sc_shellSort(
   signed char *scAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun24 TOC:
   '   - sorts signed char array from least to greatest
   '   o fun24 sec01:
   '     - variable declerations
   '   o fun24 sec02:
   '     - find the number of rounds to sort for
   '   o fun24 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long scIndex = 0;
   unsigned long scElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun24 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         scIndex = 0;
         scIndex <= subUL;
         ++scIndex
      ){ /*Loop: though sub array*/
         scElm = scIndex;

         for(
            scElm = scIndex;
            scElm + subUL <= endUL;
            scElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = scElm + subUL;

            if(scAry[scElm] > scAry[nextUL])
            { /*If I need to swap an element*/
               scAry[scElm] ^= scAry[nextUL];
               scAry[nextUL] ^= scAry[scElm];
               scAry[scElm] ^= scAry[nextUL];

               lastUL = scElm;
               onUL = scElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(scAry[onUL] > scAry[lastUL])
                     break; /*Positioned the element*/
   
                  scAry[onUL] ^= scAry[lastUL];
                  scAry[lastUL] ^= scAry[onUL];
                  scAry[onUL] ^= scAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sc_shellSort*/

/*-------------------------------------------------------\
| Fun25: dbl_shellSort
|   - sorts a double array from least to greatest
| Input:
|   - dblAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o scAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
dbl_shellSort(
   double *dblAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun25 TOC:
   '   - sorts a double array from least to greatest
   '   o fun24 sec01:
   '     - variable declerations
   '   o fun24 sec02:
   '     - find the number of rounds to sort for
   '   o fun24 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;
   double swapDbl = 0;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ulIndex = 0;
   unsigned long ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            if(dblAry[ulElm] > dblAry[nextUL])
            { /*If I need to swap an element*/
               swapDbl = dblAry[ulElm];
               dblAry[ulElm] = dblAry[nextUL];
               dblAry[ulElm] = swapDbl;

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(dblAry[onUL] > dblAry[lastUL])
                     break; /*Positioned the element*/

                  swapDbl = dblAry[onUL];
                  dblAry[onUL] = dblAry[lastUL];
                  dblAry[onUL] = swapDbl;
   
                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*dbl_shellSort*/

/*-------------------------------------------------------\
| Fun26: uiTwinSort_shellSort
|   - sorts a unsigned int array from least to greatest
|     and keeps it linked to a second unsigned int array
| Input:
|   - uiAry:
|     o array to sort
|   - uiSecAry:
|     o second array to keep in order with uiAry
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o uiAry to be sorted form least to greatest
|     o uiSecAry to be sorted by uiAry
\-------------------------------------------------------*/
void
uiTwinSort_shellSort(
   unsigned int *uiAry,
   unsigned int *uiSecAry,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun26 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun26 sec01:
   '     - variable declerations
   '   o fun26 sec02:
   '     - find the number of rounds to sort for
   '   o fun26 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = endUL - startUL;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long uiIndex = 0;
   unsigned long uiElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numElmUL <= 1)
      return; /*nothing to sort*/

   /*Recursion formuia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         uiIndex = 0;
         uiIndex <= subUL;
         ++uiIndex
      ){ /*Loop: though sub array*/
         uiElm = uiIndex;

         for(
            uiElm = uiIndex;
            uiElm + subUL <= endUL;
            uiElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = uiElm + subUL;

            if(uiAry[uiElm] > uiAry[nextUL])
            { /*If I need to swap an element*/
               uiAry[uiElm] ^= uiAry[nextUL];
               uiAry[nextUL] ^= uiAry[uiElm];
               uiAry[uiElm] ^= uiAry[nextUL];


               uiSecAry[uiElm] ^= uiSecAry[nextUL];
               uiSecAry[nextUL] ^= uiSecAry[uiElm];
               uiSecAry[uiElm] ^= uiSecAry[nextUL];

               lastUL = uiElm;
               onUL = uiElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(uiAry[onUL] > uiAry[lastUL])
                     break; /*Positioned the element*/
   
                  uiAry[onUL] ^= uiAry[lastUL];
                  uiAry[lastUL] ^= uiAry[onUL];
                  uiAry[onUL] ^= uiAry[lastUL];

                  uiSecAry[onUL] ^= uiSecAry[lastUL];
                  uiSecAry[lastUL] ^= uiSecAry[onUL];
                  uiSecAry[onUL] ^= uiSecAry[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*uiTwinSort_shellSort*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
