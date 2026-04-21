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
'   o fun08: getBarcode_spolST
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
| Fun08: getBarcode_spolST
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
|    o -1 for lineage not found
\-------------------------------------------------------*/
signed int
getBarcode_spolST(
   struct spolST *spolSTAryPtr,
   unsigned long codeUL,
   unsigned int lenAryUI
){
   signed int midSI = 0;
   signed int rightSI = (lenAryUI) - 1;
   signed int leftSI = 0;

   while(leftSI <= rightSI)
   { /*Loop: Search for the query lineage*/
      midSI = (leftSI + rightSI) >> 1;

     if((codeUL) > (spolSTAryPtr)[midSI].codeUL)
         leftSI = midSI + 1;

     else if((codeUL)<(spolSTAryPtr)[midSI].codeUL)
         rightSI = midSI - 1;

     else
        return midSI;
   } /*Loop: Search for the query lineage*/

   return -1;
} /*getBarcode_spolST*/

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
| Fun11: pspol_spolST
|   - print out an spoligotype and matching lineage
| Input:
|   - idStr:
|     o c-string with id/name of sequence
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts
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
\-------------------------------------------------------*/
void
pspol_spolST(
   signed char *idStr,
   unsigned int *codeAryUI,
   signed char fragmentBl,
   unsigned int numSupUI,
   struct spolST *spoligoAryST,
   signed int numSpoligosSI,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - Print out an spoligotype and matching lineage
   '   o fun11 sec01:
   '     - Variable declerations
   '   o fun11 sec02:
   '     - Convert barcode to numeric & "octal" formats
   '   o fun11 sec03:
   '     - Find the lineage and print out the entry
   '   o fun11 sec04:
   '     - for fragments, print out counts
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siDig = 0;
   signed char tmpC = 0;

   /*For getting the "octal" number*/
   signed char codeStr[65];
   signed char octalStr[65];
   signed int lenOctalSI = 0;

   signed char cntStr[4096];
   signed int digInCntStr = 0;

   signed int indexSI = 0;/*Index of lineage in database*/

   unsigned long codeUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - Convert the barcode to numeric & "octal" formats
   ^   o fun11 sec02 sub01:
   ^     - see if first spacer mapped (bit 1 in octal)
   ^   o fun11 sec02 sub02:
   ^     - see if second spacer mapped (bit 2 in octal)
   ^   o fun11 sec02 sub03:
   ^     - see if third spacer mapped (bit 3 in octal)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec02 Sub01:
   *   - see if first spacer mapped (bit 1 in octal)
   \*****************************************************/

   while((signed int) codeAryUI[siDig] > -1)
   { /*Loop: Translate the barcode to number and octal*/
      codeUL <<= 1;

      if(codeAryUI[siDig] > 0)
      { /*If: spacer mapped*/
          codeStr[siDig] = 'I';
          codeUL |= 1;
          octalStr[lenOctalSI] = 1;
      } /*If: spacer mapped*/

      else
      { /*Else: no spacer*/
          codeStr[siDig] = 'o';
          octalStr[lenOctalSI] = 0;
      } /*Else: no spacer*/

      ++siDig;

      if((signed int) codeAryUI[siDig] < 0)
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;/*make octal numeric*/
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      /**************************************************\
      * Fun11 Sec02 Sub02:
      *   - see if second spacer mapped (bit 2 in octal)
      \**************************************************/

      codeUL <<= 1;
      octalStr[lenOctalSI] <<= 1;

      if(codeAryUI[siDig] > 0)
      { /*If: spacer mapped*/
          codeStr[siDig] = 'I';
          codeUL |= 1;
          octalStr[lenOctalSI] |= 1;
      } /*If: spacer mapped*/

      else
          codeStr[siDig] = 'o'; /*no spacer*/

      ++siDig;

      if((signed int) codeAryUI[siDig] < 0)
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;/*make octal numeric*/
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      /**************************************************\
      * Fun11 Sec02 Sub03:
      *   - see if third spacer mapped (bit 3 in octal)
      \**************************************************/

      codeUL <<= 1;
      octalStr[lenOctalSI] <<= 1;

      if(codeAryUI[siDig] > 0)
      { /*If: spacer mapped*/
          codeStr[siDig] = 'I';
          codeUL |= 1;
          octalStr[lenOctalSI] |= 1;
      } /*If: spacer mapped*/

      else
          codeStr[siDig] = 'o'; /*no spacer*/

      octalStr[lenOctalSI] += 48; /*make octal numeric*/
      ++lenOctalSI;
      ++siDig;
   } /*Loop: Translate the barcode to number and octal*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - Find the lineage and print out the entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*This is here to remove spaces*/
   octalStr[lenOctalSI] = '\0';
   codeStr[siDig] = '\0';

   siDig = 0;
   while(idStr[siDig++] > 32) ;
   tmpC = idStr[siDig];
   idStr[siDig] = '\0';

   if(spoligoAryST)
   { /*If: I have lineages to check*/
      indexSI =
         getBarcode_spolST(
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
         codeStr,
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
         codeStr,
         octalStr
      );
   } /*Else: There is no lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - for fragments, print out counts
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fragmentBl)
   { /*If: I am checking fragments*/
      for(
         siDig = 0;
         (signed int) codeAryUI[siDig] > -1;
         ++siDig
      ){ /*Loop: get the counts and barcode*/
         codeStr[siDig] = (!!codeAryUI[siDig]) + 48;

         digInCntStr += 
            numToStr(
               &cntStr[digInCntStr],
               codeAryUI[siDig]
         );

         cntStr[digInCntStr++] = '\t';
      } /*Loop: get the counts and barcode*/

      --digInCntStr;
      cntStr[digInCntStr] = '\0';

      codeStr[siDig] = '\0';

      fprintf(
         (FILE *) outFILE,
         "\t%u\t%s",
         numSupUI,
         cntStr
      ); /*print out the counts*/
   } /*If: I am checking fragments*/

   fprintf((FILE *) outFILE, "%s", str_endLine);
   idStr[siDig] = tmpC; 
} /*pspol_spolST*/

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
