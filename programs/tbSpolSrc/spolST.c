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

#include "spolST.h"

#include "../genLib/ulCp.h"
#include "../genLib/numToStr.h"
#include "../genLib/charCp.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*.h macros only*/
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
   sint siSpoligo = 0;
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
   sint siSpoligo = 0;

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

  schar *swapStr = 0;

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
   sint midSI = 0;
   sint rightSI = (lenAryUI) - 1;
   sint leftSI = 0;

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
   '     - Open the database file
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

   uint lenBuffUI = 0;
   schar *buffHeapStr = 0;

   sint numLinesSI = 0;
   uint lenLongestLineUI = 0;
   uint lenLineUI = 0;
   uint tmpUI = 0;
   ulong bytesUL = 0;

   ulong ulComma = mkDelim_ulCp(',');
   uint posUI = 0;

   struct spolST *retHeapST = 0;
   FILE *dbFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - Open the database file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   dbFILE = fopen((char*) dbFileStr, "r");

   if(! dbFILE)
      goto fileErr_fun09_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - Find the number of lines (lineages) in database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    lenBuffUI = 1 << 10;
    buffHeapStr = malloc((lenBuffUI + 9) * sizeof(schar));

    if(! buffHeapStr)
      goto memErr_fun09_sec06_sub02;

    bytesUL =
       fread(
          buffHeapStr,
          sizeof(char),
          lenBuffUI,
          dbFILE
       ); /*Get the first part of the file*/

    buffHeapStr[bytesUL] = '\0';

    while(bytesUL)
    { /*Loop: Find the number of lines in the file*/
        tmpUI = endLine_ulCp(&buffHeapStr[posUI]);
        posUI += tmpUI;
        lenLineUI += tmpUI;

        if(buffHeapStr[posUI] == '\n')
        { /*If: I have an new line*/
           ++numLinesSI;

           lenLongestLineUI = 
              max_genMath(
                 lenLongestLineUI,
                 lenLineUI
              ); /*See if I have an longest line*/

           lenLineUI = 0;
           ++posUI;
        } /*If: I have an new line*/

        else
        { /*Else: I need to read in more file*/
           bytesUL =
              fread(
                 buffHeapStr,
                 sizeof(char),
                 lenBuffUI,
                 dbFILE
              ); /*Get the first part of the file*/

           posUI = 0;
           buffHeapStr[bytesUL] = '\0';
        } /*Else: I need to read in more file*/
    } /*Loop: Find the number of lines in the file*/

   fseek(dbFILE, 0, SEEK_SET); /*Get back to first line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - Allocate memory for the database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(lenBuffUI < lenLongestLineUI)
   { /*If: I need to resize the buffer*/
      lenBuffUI = lenLongestLineUI + 1; /*+1 for null*/

      free(buffHeapStr);
      buffHeapStr = 0;

      buffHeapStr =
         malloc((lenBuffUI + 9) * sizeof(schar));

      if(! buffHeapStr)
         goto memErr_fun09_sec06_sub02;
   } /*If: I need to resize the buffer*/

   retHeapST = mkAry_spolST(numLinesSI);

   if(! retHeapST)
         goto memErr_fun09_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - Read in the database
   ^   o fun09 sec05 sub01:
   ^     - Get past the header, initialize, and start loop
   ^   o fun09 sec05 sub02:
   ^     - Copy the strain id
   ^   o fun09 sec05 sub03:
   ^     - Get the barcode (binary number)
   ^   o fun09 sec05 sub04:
   ^     - Skip the octal entry
   ^   o fun09 sec05 sub05:
   ^     - Get the lineage entry
   ^   o fun09 sec05 sub06:
   ^     - Get the SIT entry
   ^   o fun09 sec05 sub07:
   ^     - Get the countries seen in entry
   ^   o fun09 sec05 sub08:
   ^     - Move to the next lineage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec05 Sub01:
   *   - Get past the header, initialize, and start loop
   \*****************************************************/

   /*This will never happen, but it also avoids an
   `   compliler warning
   */
   if(! fgets((char *) buffHeapStr, lenBuffUI, dbFILE))
      goto fileErr_fun09_sec06_sub02;

   *numElmSIPtr = 0;

   while(fgets((char *) buffHeapStr, lenBuffUI, dbFILE))
   { /*Loop: Read in the database*/
      posUI = 0;

      /**************************************************\
      * Fun09 Sec05 Sub02:
      *   - Copy the strain id
      \**************************************************/

      tmpUI = 
         lenStr_charCp(
            &buffHeapStr[posUI],
            ','
         );

      retHeapST[*numElmSIPtr].idStr =
         malloc((tmpUI + 1) * sizeof(schar));

      if(! retHeapST[*numElmSIPtr].idStr)
         goto memErr_fun09_sec06_sub02;

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
      * Fun09 Sec05 Sub03:
      *   - Get the barcode (binary number)
      \**************************************************/

      while(buffHeapStr[posUI] != ',')
      { /*Loop: convert the barcode to long*/
         retHeapST[*numElmSIPtr].codeUL <<= 1;

         if(buffHeapStr[posUI] == 'n')
            retHeapST[*numElmSIPtr].codeUL |= 1;
         else if(buffHeapStr[posUI] == '1')
            retHeapST[*numElmSIPtr].codeUL |= 1;

         ++posUI;
      } /*Loop: convert the barcode to long*/

      ++posUI;

      /**************************************************\
      * Fun09 Sec05 Sub04:
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
      * Fun09 Sec05 Sub05:
      *   - Get the lineage entry
      \**************************************************/

      tmpUI =
         lenStr_ulCp(
            &buffHeapStr[posUI],
            ulComma,
            ','
         );

      retHeapST[*numElmSIPtr].lineageStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].lineageStr)
         goto memErr_fun09_sec06_sub02;

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
      * Fun09 Sec05 Sub06:
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
         goto memErr_fun09_sec06_sub02;

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
      * Fun09 Sec05 Sub07:
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
         goto memErr_fun09_sec06_sub02;

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
      * Fun09 Sec05 Sub08:
      *   - Move to the next lineage entry
      \**************************************************/

      ++(*numElmSIPtr);
   } /*Loop: Read in the database*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec06:
   ^   - clean up
   ^   o fun09 sec06 sub01:
   ^     - clean up after success
   ^   o fun09 sec06 sub02:
   ^     - deal with memory errors
   ^   o fun09 sec06 sub03:
   ^     - deal with file errors
   ^   o fun09 sec06 sub04:
   ^     - clean up after errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec06 Sub01:
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
   * Fun09 Sec06 Sub02:
   *   - deal with memory errors
   \*****************************************************/

   memErr_fun09_sec06_sub02:;
   *errSC = def_memErr_tbSpolDefs;
   goto errCleanUp_fun09_sec06_sub02;

   /*****************************************************\
   * Fun09 Sec06 Sub03:
   *   - deal with file errors
   \*****************************************************/

   fileErr_fun09_sec06_sub02:;
   *errSC = def_fileErr_tbSpolDefs;
   goto errCleanUp_fun09_sec06_sub02;

   /*****************************************************\
   * Fun09 Sec06 Sub04:
   *   - clean up after errors
   \*****************************************************/

   errCleanUp_fun09_sec06_sub02:;

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

   if(fragBl)
   { /*If: I am doing fragment checks on reads*/
      fprintf(
       (FILE *) outFILE,
       "ref\tbarcode\tnumReads"
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

      fprintf(
         (FILE *) outFILE,
         "\n"
      );
   } /*If: I am doing fragment checks on reads*/

   else
   { /*Else: I am using non-fragment mode*/
      fprintf(
         (FILE *) outFILE,
         "ref\tstrain\tbarcode\toctal\tlineage\tSIT"
      );

      fprintf(
         (FILE *) outFILE,
         "\tcountries\n"
      );
   } /*Else: I am using non-fragment mode*/
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
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siDig = 0;
   schar tmpC = 0;

   /*For getting the "octal" number*/
   schar codeStr[65];
   schar octalStr[65];
   sint lenOctalSI = 0;

   schar cntStr[4096];
   sint digInCntStr = 0;

   sint indexSI = 0; /*Index of lineage in database*/

   ulong codeUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - Convert the barcode to numeric & "octal" formats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fragmentBl)
   { /*If: I am checking fragments*/
      for(
         siDig = 0;
         (sint) codeAryUI[siDig] > -1;
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
         "%s\t%s\t%u\t%s\n",
         idStr,
         codeStr,
         numSupUI,
         cntStr
      ); /*print out the counts*/

      return;
   } /*If: I am checking fragments*/

   while((sint) codeAryUI[siDig] > -1)
   { /*Loop: Translate the barcode to number and octal*/
      codeStr[siDig] = (!!codeAryUI[siDig]) + 48;

      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] = codeStr[siDig] - 48;
      
      ++siDig;

      if((sint) codeAryUI[siDig] < 0)
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      codeStr[siDig] = (!!codeAryUI[siDig]) + 48;

      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] <<= 1;
      octalStr[lenOctalSI] |= (codeStr[siDig] - 48);

      ++siDig;

      if((sint) codeAryUI[siDig] < 0)
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      codeStr[siDig] = (!!codeAryUI[siDig]) + 48;

      codeUL <<= 1;
      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] <<= 1;
      octalStr[lenOctalSI] |= (codeStr[siDig] - 48);
      octalStr[lenOctalSI] += 48;

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
         "%s\t%s\t%s\t\'%s\t%s\t%s\t%s\n",
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
         "%s\tNA\t%s\t\'%s\tNA\tNA\tNA\n",
         idStr,
         codeStr,
         octalStr
      );
   } /*Else: There is no lineage*/

   idStr[siDig] = tmpC; 
} /*pspol_spolST*/

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
