/*########################################################
# Name: spolST
#   - holds the spolST (spoligotype structure) and its
#     supporting functions
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o .h st01: spolST
'     - holds an single lineage for an spoligotype
'   o fun01: blank_spolST
'     - blanks all variables in and spolST structure
'   o fun02: init_spolST
'     - sets all pointers to null and other variables
'      (none) to defaults in an spolST struct
'   o fun03: mkAry_spolST
'     - makes array of initialized spoligotype structers
'   o fun04: freeStack_spolST
'     - frees an spolST struct on the stack
'   o fun05: freeHeap_spolST
'     - frees an spolST struct (on heap)
'   o fun06: freeHeapAry_spolST
'     - frees an array of spolST struct (on heap)
'   o fun07: sortAry_spolST
'     - sorts an array of spolST structures by least to
'       greatest with shell sort
'   o fun08: codeToLineage_spolST
'     - finds an spoligo barcode in an array of spolST
'       structures using an binary search
'   o fun09: readDb_spolST
'     - reads in an database of spoligotypes and returns
'       an array of spolST structures sorted by barcode
'   o fun10: phead_spolST
'     - print out the header for the spoligotype output
'   o fun11: pspol_spolST
'     - print out an spoligotype and matching lineage
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
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "spolST.h"

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/fileFun.h"

/*.h files only*/
#include "../genLib/genMath.h" /*.h macros only*/
#include "../genLib/endLine.h"
#include "../genLib/64bit.h"
#include "tbSpolDefs.h"

/*-------------------------------------------------------\
| Fun01: blank_spolST
|   - blanks all variables in and spolST structure
| Input:
|   - spolSTPtr:
|     o point to an spolST structure to blank
| Output:
|   - Modifies:
|     o all c-strings in spolST to start with null
\-------------------------------------------------------*/
void
blank_spolST(
   struct spolST *spolSTPtr
){
   if((spolSTPtr)->idStr)
      *(spolSTPtr)->idStr = '\0';

   (spolSTPtr)->codeUL = 0;

   if((spolSTPtr)->lineageStr)
      *(spolSTPtr)->lineageStr  = '\0';

   if((spolSTPtr)->sitStr)
      *(spolSTPtr)->sitStr  = '\0';

   if((spolSTPtr)->countriesStr)
      *(spolSTPtr)->countriesStr  = '\0';
} /*blank_spolST*/

/*-------------------------------------------------------\
| Fun02: init_spolST
|   - sets all pointers to null and other variables
|     (currently none) to defaults in an spolST struct
| Input:
|   - spolSTPtr:
|     o point to an spolST structure to initialize
| Output:
|   - Modifies:
|     o all points to point to null
|     o all non-pointers (none) to be default values
\-------------------------------------------------------*/
void
init_spolST(
   struct spolST *spolSTPtr
){
   (spolSTPtr)->idStr = 0;
   (spolSTPtr)->lineageStr = 0;
   (spolSTPtr)->sitStr = 0;
   (spolSTPtr)->countriesStr = 0;

   blank_spolST((spolSTPtr));
} /*initPoligoST*/

/*-------------------------------------------------------\
| Fun03: mkAry_spolST
|   - makes an array of initialized spoligotype structers
| Input:
|   - numSpoligosSI:
|     o number of spolST structures to make
| Output:
|   - Returns:
|     o pointer to spolST array
|     o 0 for memory errors
\-------------------------------------------------------*/
struct spolST *
mkAry_spolST(
   signed int numSpoligosST
){
   signed int siSpoligo = 0;
   struct spolST *retST = 0;

   retST = malloc(numSpoligosST * sizeof(struct spolST));

   if(! retST)
      return 0;

   for(
      siSpoligo = 0;
      siSpoligo < numSpoligosST;
      ++siSpoligo
   ) init_spolST(&retST[siSpoligo]);

   return retST;
} /*mkAry_spolST*/

/*-------------------------------------------------------\
| Fun04: freeStack_spolST
|   - frees an spolST struct on the stack
| Input:
|   - spolSTPtr:
|     o pointer to an spolST structure to free
| Output:
|   - Frees:
|     o all variables in spolST and sets to null
\-------------------------------------------------------*/
void
freeStack_spolST(
   struct spolST *spolSTPtr
){
   if(spolSTPtr)
   { /*If: have structure to free*/
      if(spolSTPtr->idStr)
         free(spolSTPtr->idStr);

      spolSTPtr->idStr = 0;

      if(spolSTPtr->lineageStr)
         free(spolSTPtr->lineageStr);

      spolSTPtr->lineageStr = 0;

      if(spolSTPtr->sitStr)
         free(spolSTPtr->sitStr);

      spolSTPtr->sitStr = 0;


      if(spolSTPtr->countriesStr)
         free(spolSTPtr->countriesStr);

      spolSTPtr->countriesStr = 0;

      init_spolST(spolSTPtr);
   } /*If: have structure to free*/
} /*freeSpoligoStack*/

/*-------------------------------------------------------\
| Fun05: freeHeap_spolST
|   - frees an spolST struct (on heap)
| Input:
|   - spolSTPtr:
|     o pointer to an spolST structure to free
| Output:
|   - Frees:
|     o spoligoST
\-------------------------------------------------------*/
void
freeHeap_spolST(
   struct spolST *spolSTPtr
){
   if(spolSTPtr)
   { /*If: have structure to free*/
      freeStack_spolST(spolSTPtr);
      free(spolSTPtr);
   } /*If: have structure to free*/
} /*freeHeap_spolST*/

/*-------------------------------------------------------\
| Fun06: freeHeapAry_spolST
|   - frees an array of spolST struct (on heap)
| Input:
|   - spoligoArySTPtr:
|     o pointer to an spolST array structure to free
|   - numSpoligosSI:
|     o number of spolST structers in spoligoArySTPtr
| Output:
|   - Frees:
|     o spoligoArySTPtr and all its elements
\-------------------------------------------------------*/
void
freeHeapAry_spolST(
   struct spolST *spoligoArySTPtr,
   signed int numSpoligosSI
){
   signed int siSpoligo = 0;

   if(! spoligoArySTPtr)
      return;

   for(
      siSpoligo = 0;
      siSpoligo < numSpoligosSI;
      ++siSpoligo
   ) freeStack_spolST(&spoligoArySTPtr[siSpoligo]);

   free(spoligoArySTPtr);
} /*freeHeapAry_spolST*/

/*-------------------------------------------------------\
| Fun07: sortAry_spolST
|  - sorts an array of spolST structures by least to
|    greatest with shell sort
| Input:
|  - spolArySTPtr:
|    o point to an array of spolST structures to sort
|  - startUL:
|    o first element to start sorting at
|  - endUL:
|    o last element to sort (index 0)
| Output:
|  - Modifies:
|    o spoligoArySTPtr array to be sorted form least to
|       greatest
\-------------------------------------------------------*/
void
sortAry_spolST(
   struct spolST *spolArySTPtr,
   unsigned long startUL,
   unsigned long endUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '  - sorts an array of spolST structures by least to
   '    greatest with shell sort
   '  - shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - i made some minor changes, but is mostly the
   '      same
   '  o fun07 sec01:
   '    - Variable declerations
   '  o fun07 sec02:
   '    - Find the number of rounds to sort for
   '  o fun07 sec03:
   '    - Sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  signed char *swapStr = 0;

  /*Number of elements to sort*/
  unsigned long numElmUL = (endUL) - (startUL);

  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextUL = 0;
  unsigned long lastUL = 0;
  unsigned long curUL = 0;

  /*Get arrays to sort from the matrix (for sanity)*/

  /*Variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long onUL = 0;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/

  while(subUL < numElmUL - 1)
     subUL = (3 * subUL) + 1;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  while(subUL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(
       ulIndex = 0;
       ulIndex <= subUL;
       ++ulIndex
    ){ /*For each element in the subarray*/
      for(onUL = ulIndex;
          onUL + subUL <= endUL;
          onUL += subUL
      ){ /*Loop; swap each nth element of the subarray*/
        nextUL = onUL + subUL;

        if(
             (spolArySTPtr)[onUL].codeUL
           > (spolArySTPtr)[nextUL].codeUL
        ){ /*If I need to swap an element*/

           swapStr = spolArySTPtr[onUL].idStr;
           spolArySTPtr[onUL].idStr =
              spolArySTPtr[nextUL].idStr;
           spolArySTPtr[nextUL].idStr = swapStr;

           spolArySTPtr[onUL].codeUL ^=
              spolArySTPtr[nextUL].codeUL;
           spolArySTPtr[nextUL].codeUL ^=
              spolArySTPtr[onUL].codeUL;
           spolArySTPtr[onUL].codeUL ^=
              spolArySTPtr[nextUL].codeUL;

           swapStr = spolArySTPtr[onUL].lineageStr;
           spolArySTPtr[onUL].lineageStr =
              spolArySTPtr[nextUL].lineageStr;
           spolArySTPtr[nextUL].lineageStr = swapStr;

           swapStr = spolArySTPtr[onUL].sitStr;
           spolArySTPtr[onUL].sitStr =
              spolArySTPtr[nextUL].sitStr;
           spolArySTPtr[nextUL].sitStr = swapStr;

           swapStr = spolArySTPtr[onUL].countriesStr;
           spolArySTPtr[onUL].countriesStr =
              spolArySTPtr[nextUL].countriesStr;
           spolArySTPtr[nextUL].countriesStr = swapStr;

          lastUL = onUL;
          curUL = onUL;

          while(lastUL >= subUL)
          { /*loop; move swapped element back*/
            lastUL -= subUL;

            if(
                 (spolArySTPtr)[curUL].codeUL
               > (spolArySTPtr)[lastUL].codeUL
            ) break; /*Positioned the element*/

            swapStr = spolArySTPtr[curUL].idStr;
            spolArySTPtr[curUL].idStr =
               spolArySTPtr[lastUL].idStr;
            spolArySTPtr[lastUL].idStr = swapStr;

            spolArySTPtr[curUL].codeUL ^=
               spolArySTPtr[lastUL].codeUL;
            spolArySTPtr[lastUL].codeUL ^=
               spolArySTPtr[curUL].codeUL;
            spolArySTPtr[curUL].codeUL ^=
               spolArySTPtr[lastUL].codeUL;

            swapStr = spolArySTPtr[curUL].lineageStr;
            spolArySTPtr[curUL].lineageStr =
               spolArySTPtr[lastUL].lineageStr;
            spolArySTPtr[lastUL].lineageStr = swapStr;

            swapStr = spolArySTPtr[curUL].sitStr;
            spolArySTPtr[curUL].sitStr =
               spolArySTPtr[lastUL].sitStr;
            spolArySTPtr[lastUL].sitStr = swapStr;

            swapStr = spolArySTPtr[curUL].countriesStr;
            spolArySTPtr[curUL].countriesStr =
               spolArySTPtr[lastUL].countriesStr;
            spolArySTPtr[lastUL].countriesStr = swapStr;

            curUL = lastUL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/

    subUL = (subUL - 1) / 3; /*Move to next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sortAry_spolST*/

/*-------------------------------------------------------\
| Fun08: codeToLineage_spolST
|  - finds an spoligo barcode in an array of spolST
|    structures using an binary search
| Input:
|  - spolSTAryPtr:
|    o pointer to an array of spolST structures to
|      search
|  - codeUL:
|    o barcode to look up
|  - lenAryUI:
|    o number of spoligo type lineages (index 1)
|      (length of spolSTAryPtr)
| Output:
|  - Returns:
|    o nearest index at or after codeUL
|    o (index + 1) * -1 for lineage not found
\-------------------------------------------------------*/
signed long
codeToLineage_spolST(
   struct spolST *spolSTAryPtr,
   unsigned long codeUL,
   unsigned int lenAryUI
){
   signed long midSL = 0;
   signed long stepSL = lenAryUI;

   while(stepSL > 1)
   { /*Loop: find insert position*/
      stepSL >>= 1;

      if(codeUL >= spolSTAryPtr[midSL + stepSL].codeUL)
         midSL += stepSL;
   } /*Loop: find insert position*/

   while(
         midSL < lenAryUI
      && codeUL > spolSTAryPtr[midSL].codeUL
   ) ++midSL;

   if(
         midSL < lenAryUI
      && codeUL == spolSTAryPtr[midSL].codeUL
   ) return midSL;

   else
      return (midSL + 1) * -1;
} /*codeToLineage_spolST*/

/*-------------------------------------------------------\
| Fun09: readDb_spolST
|   - reads in an database of spoligotypes and returns
|     an array of spolST structures sorted by barcodes
| Input:
|   - dbFileStr:
|     o c-string with name of database to read in
|   - numSpoligosSIPtr:
|     o pointer to int to hold the the number of
|       spoligotypes in the database
|   - errSC:
|     o pointer to an char to hold any errors
| Output:
|   - Modifies:
|     o numSpoligosSIPtr to hold the number of extracted
|       spoligotypes
|     o errSC to hold the error
|       - 0 for no errors
|       - def_fileErr_tbSpolDefs for file errors
|       - def_memErr_tbSpolDefs for memory errors
|   - Returns:
|     o pointer to an array of spolST structures with
|       the database
|     o 0 for errors
\-------------------------------------------------------*/
struct spolST *
readDb_spolST(
   signed char *dbFileStr,
   signed int *numElmSIPtr,
   signed char *errSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09: TOC
   '   o fun07 sec01:
   '     - Variable declerations
   '   o fun07 sec02:
   '     - open database, and allocate memory
   '   o fun07 sec03:
   '     - Find the number of lines (lineages) in database
   '   o fun07 sec04:
   '     - Allocate memory for the database
   '   o fun07 sec05:
   '     - Read in the database
   '   o fun07 sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buffHeapStr = 0;

   signed int numLinesSI = 0;
   signed long maxLineSL = 0;
   unsigned int tmpUI = 0;

   unsigned long ulComma = mkDelim_ulCp(',');
   unsigned int posUI = 0;

   struct spolST *retHeapST = 0;
   FILE *dbFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - open database, and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   dbFILE = fopen((char*) dbFileStr, "r");
   if(! dbFILE)
      goto fileErr_fun09_sec04_sub02;
   numLinesSI = lineCnt_fileFun(dbFILE, &maxLineSL);

   buffHeapStr =
         malloc((maxLineSL + 11) * sizeof(signed char));
   if(! buffHeapStr)
      goto memErr_fun09_sec04_sub02;

   retHeapST = mkAry_spolST(numLinesSI);
   if(! retHeapST)
      goto memErr_fun09_sec04_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - Read in the database
   ^   o fun09 sec03 sub01:
   ^     - Get past the header, initialize, and start loop
   ^   o fun09 sec03 sub02:
   ^     - Copy the strain id
   ^   o fun09 sec03 sub03:
   ^     - Get the barcode (binary number)
   ^   o fun09 sec03 sub04:
   ^     - Skip the octal entry
   ^   o fun09 sec03 sub05:
   ^     - Get the lineage entry
   ^   o fun09 sec03 sub06:
   ^     - Get the SIT entry
   ^   o fun09 sec03 sub07:
   ^     - Get the countries seen in entry
   ^   o fun09 sec03 sub08:
   ^     - Move to the next lineage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - Get past the header, initialize, and start loop
   \*****************************************************/

   /*This will never happen, but it also avoids an
   `   compliler warning
   */
   if(! fgets((char *) buffHeapStr, maxLineSL +3, dbFILE))
      goto fileErr_fun09_sec04_sub02;

   *numElmSIPtr = 0;

   while(
      fgets((char *) buffHeapStr, maxLineSL + 3, dbFILE)
   ){ /*Loop: Read in the database*/
      posUI = 0;

      /**************************************************\
      * Fun09 Sec03 Sub02:
      *   - Copy the strain id
      \**************************************************/

      tmpUI = 
         lenStr_charCp(
            &buffHeapStr[posUI],
            ','
         );

      retHeapST[*numElmSIPtr].idStr =
         malloc((tmpUI + 1) * sizeof(signed char));

      if(! retHeapST[*numElmSIPtr].idStr)
         goto memErr_fun09_sec04_sub02;

      cpLen_ulCp(
         retHeapST[*numElmSIPtr].idStr,
         &buffHeapStr[posUI],
         tmpUI
      ); /*Copy the strain id*/

      retHeapST[*numElmSIPtr].idStr[tmpUI] = '\0';

      posUI += tmpUI + 1; /*+1 gets off the comma*/

      tmpUI = 0;

      while(retHeapST[*numElmSIPtr].idStr[tmpUI] != '\0')
      { /*Loop: Remove white space*/
        if(retHeapST[*numElmSIPtr].idStr[tmpUI] < 33)
          retHeapST[*numElmSIPtr].idStr[tmpUI] = '_';

        ++tmpUI;
      } /*Loop: Remove white space*/

      /**************************************************\
      * Fun09 Sec03 Sub03:
      *   - Get the barcode (binary number)
      \**************************************************/

      while(buffHeapStr[posUI] != ',')
      { /*Loop: convert the barcode to long*/
         retHeapST[*numElmSIPtr].codeUL <<= 1;

         if((buffHeapStr[posUI] & ~32) == ('n' & ~32))
            retHeapST[*numElmSIPtr].codeUL |= 1;
         else if(buffHeapStr[posUI] == '1')
            retHeapST[*numElmSIPtr].codeUL |= 1;
         else if((buffHeapStr[posUI] & ~32) ==('i' & ~32))
            retHeapST[*numElmSIPtr].codeUL |= 1;

         ++posUI;
      } /*Loop: convert the barcode to long*/

      ++posUI;

      /**************************************************\
      * Fun09 Sec03 Sub04:
      *   - Skip the octal entry
      \**************************************************/

      posUI +=
         lenStr_ulCp(
            &buffHeapStr[posUI],
            ulComma,
            ','
         );

      ++posUI; /*Get off the comma*/

      /**************************************************\
      * Fun09 Sec03 Sub05:
      *   - Get the lineage entry
      \**************************************************/

      tmpUI =
         lenStr_ulCp(&buffHeapStr[posUI], ulComma, ',');

      retHeapST[*numElmSIPtr].lineageStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].lineageStr)
         goto memErr_fun09_sec04_sub02;

      cpLen_ulCp(
         retHeapST[*numElmSIPtr].lineageStr,
         &buffHeapStr[posUI],
         tmpUI
      ); /*Copy the strain id*/

      retHeapST[*numElmSIPtr].lineageStr[tmpUI] = '\0';

      posUI += tmpUI + 1; /*+1 gets off the comma*/

      tmpUI = 0;

      while(
         retHeapST[*numElmSIPtr].lineageStr[tmpUI] != '\0'
      ){ /*Loop: Remove white space*/
        if(retHeapST[*numElmSIPtr].lineageStr[tmpUI] < 33)
          retHeapST[*numElmSIPtr].lineageStr[tmpUI] = '_';

        ++tmpUI;
      } /*Loop: Remove white space*/

      /**************************************************\
      * Fun09 Sec03 Sub06:
      *   - Get the SIT entry
      \**************************************************/

      tmpUI =
         lenStr_ulCp(
            &buffHeapStr[posUI],
            ulComma,
            ','
         );

      retHeapST[*numElmSIPtr].sitStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].sitStr)
         goto memErr_fun09_sec04_sub02;

      cpLen_ulCp(
         retHeapST[*numElmSIPtr].sitStr,
         &buffHeapStr[posUI],
         tmpUI
      ); /*Copy the strain id*/

      retHeapST[*numElmSIPtr].sitStr[tmpUI] = '\0';

      posUI += tmpUI + 1; /*+1 gets off the comma*/

      tmpUI = 0;

      while(retHeapST[*numElmSIPtr].sitStr[tmpUI] != '\0')
      { /*Loop: Remove white space*/
        if(retHeapST[*numElmSIPtr].sitStr[tmpUI] < 33)
          retHeapST[*numElmSIPtr].sitStr[tmpUI] = '_';

        ++tmpUI;
      } /*Loop: Remove white space*/

      /**************************************************\
      * Fun09 Sec03 Sub07:
      *   - Get the countries seen in entry
      \**************************************************/

      tmpUI = posUI;

      while(
            buffHeapStr[tmpUI] != ','
         && buffHeapStr[tmpUI] > 31
      ) ++tmpUI; /*Find the end of the string*/

      tmpUI -= posUI;

      retHeapST[*numElmSIPtr].countriesStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].countriesStr)
         goto memErr_fun09_sec04_sub02;

      cpLen_ulCp(
         retHeapST[*numElmSIPtr].countriesStr,
         &buffHeapStr[posUI],
         tmpUI
      ); /*Copy the strain id*/

      retHeapST[*numElmSIPtr].countriesStr[tmpUI] = '\0';

      tmpUI = 0;

      while(
         retHeapST[*numElmSIPtr].countriesStr[tmpUI]!='\0'
      ){ /*Loop: Remove white space*/
        if(retHeapST[*numElmSIPtr].countriesStr[tmpUI]<33)
          retHeapST[*numElmSIPtr].countriesStr[tmpUI]='_';

        ++tmpUI;
      } /*Loop: Remove white space*/

      /**************************************************\
      * Fun09 Sec03 Sub08:
      *   - Move to the next lineage entry
      \**************************************************/

      ++(*numElmSIPtr);
   } /*Loop: Read in the database*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - clean up
   ^   o fun09 sec04 sub01:
   ^     - clean up after success
   ^   o fun09 sec04 sub02:
   ^     - deal with memory errors
   ^   o fun09 sec04 sub03:
   ^     - deal with file errors
   ^   o fun09 sec04 sub04:
   ^     - clean up after errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec04 Sub01:
   *   - clean up after success
   \*****************************************************/

   sortAry_spolST(retHeapST, 0, (*numElmSIPtr - 1));

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

   fclose(dbFILE);
   dbFILE = 0;

   return retHeapST;

   /*****************************************************\
   * Fun09 Sec04 Sub02:
   *   - deal with memory errors
   \*****************************************************/

   memErr_fun09_sec04_sub02:;
   *errSC = def_memErr_tbSpolDefs;
   goto errCleanUp_fun09_sec04_sub02;

   /*****************************************************\
   * Fun09 Sec04 Sub03:
   *   - deal with file errors
   \*****************************************************/

   fileErr_fun09_sec04_sub02:;
   *errSC = def_fileErr_tbSpolDefs;
   goto errCleanUp_fun09_sec04_sub02;

   /*****************************************************\
   * Fun09 Sec04 Sub04:
   *   - clean up after errors
   \*****************************************************/

   errCleanUp_fun09_sec04_sub02:;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

   freeHeapAry_spolST(
      retHeapST,
      *numElmSIPtr
   );

   retHeapST = 0;

   if(dbFILE)
      fclose(dbFILE);

   dbFILE = 0;

   return 0;
} /*readSpoligoDB*/

/*-------------------------------------------------------\
| Fun10: phead_spolST
|   - print out the header for the spoligotype output
| Input:
|   - fragBl:
|     o 1: printing out header for sequence fragments
|          that  not have the entire direct repeat region.
|     o 0: header for sequences with full DR. An consensus
|          with fragments will use this header as well.
|   - outFILE:
|     o FILE pointer with file to print to
| Output:
|   - Prints:
|     o header to the output file
\-------------------------------------------------------*/
void
phead_spolST(
   signed char fragBl,
   void *outFILE
){

   fprintf(
      (FILE *) outFILE,
      "input\tstrain\tbarcode\toctal\tlineage\tSIT"
   );

   fprintf(
      (FILE *) outFILE,
      "\tcountries"
   );

   if(fragBl)
   { /*If: I am doing fragment checks on reads*/

      fprintf(
         (FILE *) outFILE,
         "\tmapped_reads"
      );

      for(
         fragBl = 0;
         fragBl < 43;
         ++fragBl
      ){ /*Loop: print out counter header*/
         fprintf(
            (FILE *) outFILE,
            "\t%i",
            fragBl + 1
         );
      } /*Loop: print out counter header*/
   } /*If: I am doing fragment checks on reads*/

   fprintf((FILE *) outFILE, "%s", str_endLine);
} /*phead_spolST*/

/*-------------------------------------------------------\
| Fun0x: depthToBarcode_spolST
|   - converts a read depth array to a barcode
| Input:
|   - barStr:
|     o c-string of 65 bytes to store the barcode
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts,
|       ends with (unsigned int) -1
|   - minDepthUI:
|     o mininum read depth to keep a spacer
|   - minPercDepthF:
|     o mininum percent read depth to keep a spacer
| Output:
|   - Modifies:
|     o barStr to have the barcode
|       * `o` for no barcode
|       * `I` for barcode present
|       * `x` if positon had low depth
\-------------------------------------------------------*/
void
depthToBarcode_spolST(
   signed char *barStr,
   unsigned int *codeAryUI,
   unsigned int minDepthUI,
   float minPercDepthF
){
   unsigned int maxDepthUI = 0;
   unsigned int siDig = 0;
   signed int *codeArySI = (signed int *) codeAryUI;
      /*this is for my sanity, it is not needed*/

   /*_find_minimum_percent_depth________________________*/
   if(minPercDepthF > 0)
   { /*If: have a minimum percent read depth*/
      for(siDig = 0; codeArySI[siDig] > -1; ++siDig)
      { /*Loop: find maximum read depth*/
         if(siDig >= 43)
            break;
         maxDepthUI =
            max_genMath(maxDepthUI, codeAryUI[siDig]);
      } /*Loop: find maximum read depth*/

      minPercDepthF = (float) maxDepthUI * minPercDepthF;

      if((unsigned int) minPercDepthF > minDepthUI)
         minDepthUI = minPercDepthF;
         /*truncating (flooring) makes more sense then
         `  rounding up
         */
   } /*If: have a minimum percent read depth*/

   /*_build_the_barcode_________________________________*/
   siDig = 0;
   for(siDig = 0; codeArySI[siDig] > -1; ++siDig)
   { /*Loop: convert depths to barcode*/
      if(siDig >= 43)
         break;
      if(codeAryUI[siDig] >= minDepthUI)
         barStr[siDig] = 'I';
      else if(codeAryUI[siDig])
         barStr[siDig] = 'x';
      else
         barStr[siDig] = 'o';
   } /*Loop: convert depths to barcode*/

   barStr[siDig] = 0;
} /*depthToBarcode_spolST*/

/*-------------------------------------------------------\
| Fun0y: barcodeToOctal_spolST
|   - convert barcode from depthToBarcode_spolST to an
|     octal
| Input:
|   - octalStr:
|     o c-string to get octal number (at least 32 bytes)
|   - barStr:
|     o c-string with barcode to convert to an octal code
|     o barStr from depthToBarcode_spolST()
| Output:
|   - Modifies:
|     o octalStr to have the octal code (x's got to o)
|   - Returns:
|     o unsigned long with index of barcode for quick look
|       up
\-------------------------------------------------------*/
unsigned long
barcodeToOctal_spolST(
   signed char *octalStr,
   signed char *barStr
){
   signed int siBar = 0;
   signed int siOctal = 0;
   signed int shiftSC = 0; 
   unsigned long codeUL = 0;

   octalStr[0] = 0;

   for(siBar = 0; barStr[siBar]; ++siBar)
   { /*Loop: barcode to octal*/
      codeUL <<= 1;

      if(shiftSC < 3)
         octalStr[siOctal] <<= 1;
      else
      { /*Else: moving onto the next number*/
         shiftSC = 0;
         octalStr[siOctal++] += 48; /*make numeric*/
         octalStr[siOctal] = 0;
      } /*Else: moving onto the next number*/

      ++shiftSC;

      if(barStr[siBar] == 'I')
      { /*If: spacer mapped*/
          codeUL |= 1;
          octalStr[siOctal] |= 1;
      } /*If: spacer mapped*/
   } /*Loop: Translate the barcode to number and octal*/


   if(shiftSC > 0)
      octalStr[siOctal++] += 48;
   octalStr[siOctal] = 0;

   return codeUL;
} /*barcodeToOctal_spolST*/

/*-------------------------------------------------------\
| Fun11: pspol_spolST
|   - print out an spoligotype and matching lineage
| Input:
|   - idStr:
|     o c-string with id/name of sequence
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts,
|       ends with (unsigned int) -1
|   - minDepthUI:
|     o mininum read depth to keep a spacer
|   - minPercDepthF:
|     o minimum percent read depth (0 to 1)
|   - fragmentBl:
|     o 1: fragment mode, do not find lineages
|     o 0: complete reads find lineages
|   - numSupUI:
|     o unsigned int with the number of reads that had at
|       least one spacer (for fragment mode)
|   - spoligoAryST:
|     o array of spoligotype lineages to get lineage from
|   - numSpoligosSI:
|     o length of spoligoAryST (index 1)
|   - outFILE:
|     o FILE pointer with file to print to
| Output:
|   - Prints:
|     o spoligotype entry to the outFILE
\-------------------------------------------------------*/
void
pspol_spolST(
   signed char *idStr,
   unsigned int *codeAryUI,
   unsigned int minDepthUI,
   float minPercDepthF,
   signed char fragmentBl,
   unsigned int numSupUI,
   struct spolST *spoligoAryST,
   signed int numSpoligosSI,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - Print out an spoligotype and matching lineage
   '   o fun11 sec01:
   '     - variable declerations
   '   o fun11 sec02:
   '     - get barcode and octal + clean up id
   '   o fun11 sec03:
   '     - print spoligotype entry
   '   o fun11 sec04:
   '     - print read counts (fragment mode)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siDig = 0;
   signed char tmpC = 0;

   signed char buffStr[512];
   signed char *barStr = &buffStr[0];
   signed char *octalStr = &buffStr[128];

   signed int indexSI = 0;/*Index of lineage in database*/
   unsigned long codeUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - get barcode and octal + clean up id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   depthToBarcode_spolST(
      barStr,
      codeAryUI,
      minDepthUI,
      minPercDepthF
   );
   codeUL = barcodeToOctal_spolST(octalStr, barStr);

   /*_remove_spaces_from_input_id_______________________*/
   siDig = endWhite_ulCp(idStr);
   tmpC = idStr[siDig];
   idStr[siDig] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - print spoligotype entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(spoligoAryST)
   { /*If: I have lineages to check*/
      indexSI =
         codeToLineage_spolST(
            spoligoAryST,
            codeUL,
            numSpoligosSI           
         ); /*Find the lineage*/

      if(indexSI < 0)
         goto noLineage_fun10_sec04;

      fprintf(
         (FILE *) outFILE,
         "%s\t%s\t%s\t\'%s\t%s\t%s\t%s",
         idStr,
         spoligoAryST[indexSI].idStr,
         barStr,
         octalStr,
         spoligoAryST[indexSI].lineageStr,
         spoligoAryST[indexSI].sitStr,
         spoligoAryST[indexSI].countriesStr
      );
   } /*If: I have lineages to check*/

   else
   { /*Else: There is no lineage*/
      noLineage_fun10_sec04:;

      fprintf(
         (FILE *) outFILE,
         "%s\tNA\t%s\t\'%s\tNA\tNA\tNA",
         idStr,
         barStr,
         octalStr
      );
   } /*Else: There is no lineage*/

   idStr[siDig] = tmpC; 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - print read counts (fragment mode)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fragmentBl)
   { /*If: I am checking fragments*/
      indexSI = 0;

      for(
         siDig = 0;
         (signed int) codeAryUI[siDig] > -1;
         ++siDig
      ){ /*Loop: get the counts and barcode*/
         if(siDig >= 43)
            break;
         indexSI += 
            numToStr(&buffStr[indexSI], codeAryUI[siDig]);
         buffStr[indexSI++] = '\t';
      } /*Loop: get the counts and barcode*/

      --indexSI;
      buffStr[indexSI] = '\0';

      fprintf(
         (FILE *) outFILE,
         "\t%u\t%s",
         numSupUI,
         buffStr
      ); /*print out the counts*/
   } /*If: I am checking fragments*/

   fprintf((FILE *) outFILE, "%s", str_endLine);
} /*pspol_spolST*/

#ifdef NEW
/*-------------------------------------------------------\
| Fun12: fuzzyDbGet_spolST
|   - read in a database for general lineages
| Input:
|   - dbFILE:
|     o FILE pointer to database to read in
|   - errSLPtr:
|     o signed long pointer to get the line of the error
|       or error type
| Output:
|   - Modifies:
|     o errSCPtr:
|       * 0 for no errors
|       * line error was on (> 0) for file error
|       * -1 for empty file
|       * -2 for memory errors
|   - Returns:
|     o fuzzy_spolST array with the fuzzy lineages
|     o 0 for no input, file errors, or memory errors
| dbFILE database format (1st row header, rest lineages):
|  - spaces and tabs separat columns
|  - format
|    lineage	barcode
|    1	Iooooooooooxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
|    2	ooooooooooooooooooooooooooooooooooxxxxxxxxx
|    3	xxxxxxxxxxxxxxxxxxxxxxooooooooooooxxxxxxxxx
|    4	0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxDDDDxxxxxxxxx
|  - possible values used in the barcode
|    * I = always present
|    * o = always absent
|    * x = not used (set or deleted)
|    * D = at least one spacer delete (delete; set 1)
|    * a = at least one spacer delete (absent; set 2)
|    * M = at least one spacer delete (missing; set 3)
|    * S = at least one spacer set (set; set 1)
|    * F = at least one spacer set (found; set 2)
|    * P = at least one spacer set (present; set 3)
\-------------------------------------------------------*/
struct fuzzy_spolST *
fuzzyDbGet_spolST(
   void *dbFILE,
   signed long *errSLPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' fun12: fuzzyDbGet_spolST
   '   - read in a database for general lineages
   '   o fun12 sec01:
   '     - variable declarations
   '   o fun12 sec02:
   '     - read in the fuzzy barcodes
   '   o fun12 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_buffSize_fun12_spolST 1024
   signed char buffStr[def_buffSize_fun13_spolST + 1]

   signed int idStartSI = 0; 
   signed int idLenSI = 0; 
   signed int barStartSI = 0; 
   signed int barLenSI = 0; 

   struct fuzzy_spolST *fuzzyHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - read in the fuzzy barcodes
   ^   o fun12 sec02 sub01:
   ^     - get past the header
   ^   o fun12 sec02 sub02:
   ^     - get row and find id length and barcode start
   ^   o fun12 sec02 sub03:
   ^     - make sure the barcode entry is valid
   ^   o fun12 sec02 sub04:
   ^     - copy lineage id
   ^   o fun12 sec02 sub05:
   ^     - copy barcode
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec02 Sub01:
   *   - get past the header
   \*****************************************************/

   fuzzyHeapST = malloc(sizeof(struct fuzzy_spolST));
   if(! fuzzyHeapST)
      goto memErr_fun12_sec0x;
   init_fuzzy_spolST(fuzzyHeapST);
       /*TODO: add init*/

   *errSLPtr = 1;
   if(
      ! fgets(
         buffStr,
         def_buffSize_fun12_spolST,
         (FILE *) dbFILE
      )
   ) goto emptyFile_fun12_sec0x;

   /*****************************************************\
   * Fun12 Sec02 Sub02:
   *   - get row and find id length and barcode start
   \*****************************************************/

   while(
      ! fgets(
         buffStr,
         def_buffSize_fun12_spolST,
         (FILE *) dbFILE
      )
   ){ /*Loop: get fuzzy lineages*/
      ++(*errSLPtr);

      while(buffStr[idStartSI] && buffStr[idStartSI] < 33)
         ++idStartSI;
      if(! buffStr[idLenSI])
         continue; /*empty line*/

      idLenSI = endWhite_ulCp(&buffStr[idStartSI]);
      if(buffStr[idLenSI] == ' ')
         ;
      else if(buffStr[idLenSI] == '\t')
         ;
      else
         goto fileErr_fun12_sec0x;

      barStartSI = idLenSI + idStartSI;
      while(buffStr[barStartSI] && buffStr[barStartSI]<33)
         ++barStartSI;
      if(! buffStr[barStartSI])
         goto fileErr_fun12_sec0x;

      /**************************************************\
      * Fun12 Sec02 Sub03:
      *   - make sure the barcode entry is valid
      \**************************************************/

      barLenSI = barStartSI;
      while(buffStr[barLenSI] && buffStr[barLenSI] > 32)
      { /*Loop: check barcode*/
         buffStr[barLenSI] |= 32;
         if(buffStr[barLenSI] == def_del_spolST)
            ;
         else if(buffStr[barLenSI] == def_set_spolST)
            ;
         else if(buffStr[barLenSI] == def_unkown_spolST)
            ;
         else if(buffStr[barLenSI] == def_delOne_spolST)
            ;
         else if(buffStr[barLenSI] == def_delTwo_spolST)
            ;
         else if(buffStr[barLenSI] == def_delThree_spolST)
            ;
         else if(buffStr[barLenSI] == def_setOne_spolST)
            ;
         else if(buffStr[barLenSI] == def_setTwo_spolST)
            ;
         else if(buffStr[barLenSI] == def_setThree_spolST)
            ;
         else
            goto fileErr_fun12_sec0x;

         ++barLenSI;
      } /*Loop: check barcode*/

      barLenSI -= barStartSI;

      /**************************************************\
      * Fun12 Sec02 Sub04:
      *   - copy lineage id
      \**************************************************/

      /*TODO: make memAdd*/
      if(memAdd_fuzzy_spolST(fuzzyHeapST))
         goto memErr_fun12_sec0x;

      fuzzyHeapST->idAryStr[fuzzyHeapST->lenSI] =
         malloc(
            ulAlign_64bit(idLenSI +1, sizeof(signed char))
               * sizeof(signed char)
         );
      if(! fuzzyHeapST->idAryStr[fuzzyHeapST->lenSI])
         goto memErr_fun12_sec0x;
      cpLen_ulCp(
         fuzzyHeapST->idAryStr[fuzzyHeapST->lenSI],
         &buffStr[idStartSI],
         idLenSI
      );

      /**************************************************\
      * Fun12 Sec02 Sub05:
      *   - copy barcode
      \**************************************************/

      fuzzyHeapST->barAryStr[fuzzyHeapST->lenSI] =
         malloc(
            ulAlign_64bit(barLenSI+1, sizeof(signed char))
               * sizeof(signed char)
         );
      if(! fuzzyHeapST->barAryStr[fuzzyHeapST->lenSI])
         goto memErr_fun12_sec0x;
      cpLen_ulCp(
         fuzzyHeapST->idAryStr[fuzzyHeapST->lenSI],
         &buffStr[barStartSI],
         barLenSI
      );

      ++fuzzyHeapST->lenSI;
   }  /*Loop: get fuzzy lineages*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSLPtr = 0;
   goto ret_fun12_sec0x;

   emptyFile_fun12_sec0x:;
      *errSLPtr = -1;
      goto errClean_fun12_sec0x;

   fileErr_fun12_sec0x:;
      goto errClean_fun12_sec0x;

   memErr_fun12_sec0x:;
      *errSLPtr = -2;
      goto errClean_fun12_sec0x;

   errClean_fun12_sec0x:;
      /*TODO: make freeHeap*/
      if(fuzzyHeapST)
         freeHeap_fuzzy_spolST(fuzzyHeapST);
      fuzzyHeapST = 0;
      goto ret_fun12_sec0x;

   ret_fun12_sec0x:;
      return fuzzyHeapST;
} /*fuzzyDbGet_spolST*/

/*-------------------------------------------------------\
| Fun13: genLineageSearch_spolST
|   - searches the spoligotype for generalized lineages
| Input:
|   - barStr:
|     o c-string with barcode
|     o use depthToBarcode_spolST to build this
|   - fuzzySTPtr:
|     o fuzzy_spolST struct pointer with fuzzy lineages
|       to look for
|   - lenSIPtr:
|     o signed int pointer with number of fuzzy lineages
|       found
| Output:
|   - Modifies:
|     o lenSIPtr:
|       * to have the number of fuzzy lineages found
|       * -1 if barStr did not have 43 spacers
|       * -1 for memory errors
|   - Returns:
|     o index of lineage in 
|     o 0 for no input or memory errors
\-------------------------------------------------------*/
signed int *
lineageDetect_tbSpol(
   signed char *barStr,
   struct fuzzy_spolST *fuzzySTPtr,
   signed int *lenSIPtr
){
   #define def_numSpoligo_xxx 43
   signed int siPos = 0;
   signed int siLin = 0;

   #define def_delOne_fun13 (1 << 1)
   #define def_delOneMatch_fun13 (1 << 2)

   #define def_delTwo_fun13 (1 << 3)
   #define def_delTwoMatch_fun13 (1 << 4)

   #define def_delThree_fun13 (1 << 5)
   #define def_delThreeMatch_fun13 (1 << 6)


   #define def_setOne_fun13 (1 << 7)
   #define def_setOneMatch_fun13 (1 << 8)

   #define def_setTwo_fun13 (1 << 9)
   #define def_setTwoMatch_fun13 (1 << 10)

   #define def_setThree_fun13 (1 << 11)
   #define def_setThreeMatch_fun13 (1 << 12)

   signed short *hitHeapArySS = 0;
   signed int *retHeapArySI = 0;

   /*for my own sanity*/
   signed char **linAryStr = fuzzySTPtr->barAryStr;

   *lenSIPtr = 0;

   hitHeapArySS =
      malloc(fuzzySTPtr->lenSI * sizeof(signed short));
   if(! hitHeapArySS)
      goto memErr_Fun13_sec0x;

   for(siLin = 0; siLin < fuzzySTPtr->lenSI; ++siLin)
      hitHeapArySC[siLin] = 1;
   
   for(siPos = 0; siPos < def_numSpoligo_xxx; ++siPos)
   { /*Loop: map spacers*/
      if(barStr[siPos])
         goto noBarcode_fun13_sec0x;

      for(siLin = 0; siLin < fuzzySTPtr->lenSI; ++siLin)
      { /*Loop: check if have a match*/
         if(linAryStr[siLin][siPos] == def_unkown_spolST)
            ;

         else if(linAryStr[siLin][siPos]==def_del_spolST)
         { /*ElseI If: have a deletion*/
            if( (barStr[siPos] | 32) != def_del_spolST )
               hitHeapArySS[siPos] = 0;
         } /*ElseI If: have a deletion*/

         else if(linAryStr[siLin][siPos]==def_set_spolST)
         { /*ElseI If: spacer must be set*/
            if( (barStr[siPos] | 32) != def_set_spolST )
               hitHeapArySS[siPos] = 0;
         } /*ElseI If: spacer must be set*/

         else if(
            linAryStr[siLin][siPos] == def_delOne_spolST
         ){ /*Else If: at least one spacer must be a del*/
            hitHeapArySS[siPos] |= def_delOne_fun13;

            if( (barStr[siPos] | 32) == def_del_spolST )
               hitHeapArySS[siPos]|=def_delOneMatch_fun13;
         }  /*Else If: at least one spacer must be a del*/

         else if(
            linAryStr[siLin][siPos] == def_delTwo_spolST
         ){ /*Else If: at least one spacer must be a del*/
            hitHeapArySS[siPos] |= def_delTwo_fun13;

            if( (barStr[siPos] | 32) == def_del_spolST )
               hitHeapArySS[siPos]|=def_delTwoMatch_fun13;
         }  /*Else If: at least one spacer must be a del*/

         else if(
            linAryStr[siLin][siPos] == def_delThree_spolST
         ){ /*Else If: at least one spacer must be a del*/
            hitHeapArySS[siPos] |= def_delThree_fun13;

            if( (barStr[siPos] | 32) == def_del_spolST )
               hitHeapArySS[siPos] |=
                  def_delThreeMatch_fun13;
         }  /*Else If: at least one spacer must be a del*/

         else if(
            linAryStr[siLin][siPos] == def_setOne_spolST
         ){ /*Else If: at least one spacer must be set*/
            hitHeapArySS[siPos] |= def_setOne_fun13;

            if( (barStr[siPos] | 32) == def_set_spolST )
               hitHeapArySS[siPos]|=def_setOneMatch_fun13;
         }  /*Else If: at least one spacer must be set*/

         else if(
            linAryStr[siLin][siPos] == def_setTwo_spolST
         ){ /*Else If: at least one spacer must be set*/
            hitHeapArySS[siPos] |= def_setTwo_fun13;

            if( (barStr[siPos] | 32) == def_set_spolST )
               hitHeapArySS[siPos]|=def_setTwoMatch_fun13;
         }  /*Else If: at least one spacer must be set*/

         else if(
            linAryStr[siLin][siPos] == def_setThree_spolST
         ){ /*Else If: at least one spacer must be set*/
            hitHeapArySS[siPos] |= def_setThree_fun13;

            if( (barStr[siPos] | 32) == def_set_spolST )
               hitHeapArySS[siPos] |=
                  def_setThreeMatch_fun13;
         }  /*Else If: at least one spacer must be set*/
      } /*Loop: check if have a match*/
   } /*Loop: map spacers*/

   for(siLin = 0; siLin < fuzzySTPtr->lenSI; ++siLin)
   { /*Loop: find number of detected lineages*/
      if(hitHeapArySS[siPos] == 1)
         ++(*lenSIPtr);

      else if(! hitHeapArySS[siPos])
         continue;

      else if(
           hitHeapArySS[siPos] & def_delOne_spolST
        && !(hitHeapArySS[siPos] & def_delOneMatch_spolST)
      ) hitHeapArySS[siPos] = 0;

      else if(
           hitHeapArySS[siPos] & def_delTwo_spolST
        && !(hitHeapArySS[siPos] & def_delTwoMatch_spolST)
      ) hitHeapArySS[siPos] = 0;

      else if(
           hitHeapArySS[siPos] & def_delThree_spolST
        && !(
            hitHeapArySS[siPos] & def_delThreeMatch_spolST
           )
      ) hitHeapArySS[siPos] = 0;

      else if(
           hitHeapArySS[siPos] & def_setOne_spolST
        && !(hitHeapArySS[siPos] & def_setOneMatch_spolST)
      ) hitHeapArySS[siPos] = 0;

      else if(
           hitHeapArySS[siPos] & def_setTwo_spolST
        && !(hitHeapArySS[siPos] & def_setTwoMatch_spolST)
      ) hitHeapArySS[siPos] = 0;

      else if(
           hitHeapArySS[siPos] & def_setThree_spolST
        && !(
            hitHeapArySS[siPos] & def_setThreeMatch_spolST
           )
      ) hitHeapArySS[siPos] = 0;

      else
      { /*Else: all partial sets and absent met*/
         hitHeapArySS[siPos] = 1;
         ++(*lenSIPtr);
      } /*Else: all partial sets and absent met*/
   } /*Loop: find number of detected lineages*/

   retHeapArySI = malloc(*lenSIPtr * sizeof(signed int));
   if(! retHeapArySI)
      goto memErr_fun13_sec0x;

   *lenSIPtr = 0;
   for(siLin = 0; siLin < fuzzySTPtr->lenSI; ++siLin)
   { /*Loop: recored match index's*/
      if(hitHeapArySS[siPos] == 1)
         retHeapArySI[*lenSIPtr++] = siLin;
   } /*Loop: recored match index's*/

   goto ret_fun13_sec0x;

   noBarcode_fun13_sec0x:;
      *lenSIPtr = -1;
      goto errClean_fun13_sec0x;

   memErr_fun13_sec0x:;
      *lenSIPtr = -2;
      goto errClean_fun13_sec0x;

   errClean_fun13_sec0x:;
      if(retHeapArySI)
         free(retHeapArySI);
      retHeapArySI = 0;

      goto ret_fun13_sec0x;

   ret_fun13_sec0x:;
      if(hitHeapArySS)
         free(hitHeapArySS);
      hitHeapArySS = 0;

      return retHeapArySI;
} /*lineageDetect_tbSpol*/
#endif

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
