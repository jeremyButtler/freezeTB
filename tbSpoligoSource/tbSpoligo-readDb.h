/*########################################################
# Name: tbSpoligo-readDb
#   - reads in an databse of known spoligo types
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards
'   o .h st-01: spoligoST
'     - Holds an single spoligotype lineage
'   o .h fun01: blankSpoligoST
'     - Blanks all variables in and spoligoST structure
'   o .h fun02: initSpoligoST
'      - Sets all pointers to null and other variables
'       (none) to defaults in an spoligoST struct
'   o fun03: mkSpoligoAryST
'     - Makes array of initialized spoligotype structers
'   o fun04: freeSpoligoSTStack
'     - Frees an spoligoST struct on the stack
'   o fun05: freeSpoligoST
'     - Frees an spoligoST struct (on heap)
'   o fun06: freeSpoligoSTAry
'     - Frees an array of spoligoST struct (on heap)
'   o .h fun07: swapSpoligos
'     - Swaps values in two spoligoST structures
'   o .h fun08: sortSpoligoSTAry
'     - Sorts an array of spoligoST structures by least to
'       greatest with shell sort
'   o .h fun09: findSpoligoBarcode
'     - Finds an spoligo barcode in an array of spoligoST
'       structures using an binary search
'   o fun10: readSpoligoDb
'     - Reads in an database of spoligotypes and returns
'       an array of spoligoST structures sorted by barcode
'   o fun11: pSpoligoHead
'     - Print out the header for the spoligotype output
'   o fun12: pSpoligo
'     - Print out an spoligotype and matching lineage
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef TB_SPOLIGO_READ_DATABASE_H
#define TB_SPOLIGO_READ_DATABASE_H

/*-------------------------------------------------------\
| ST-01: spoligoST
|   - Holds an single spoligotype lineage from an database
\-------------------------------------------------------*/
typedef struct
spoligoST
{ /*Single spoligotype lineage*/
   signed char *idStr;   /*Strain id entry*/

   unsigned long codeUL; /*barcode; allows faster lookup*/

   signed char *lineageStr; /*Lineage assigned*/
   signed char *sitStr;     /*SIT entry (lineage??)*/
   signed char *countriesStr;/*Contries seen in*/
}spoligoST; /*spoligoST*/

/*-------------------------------------------------------\
| Fun01: blankSpoligoST
|   - Blanks all variables in and spoligoST structure
| Input:
|   - spoligoSTPtr:
|     o Point to an spoligoST structure to blank
| Output:
|   - Modifies:
|     o All c-strings in spoligoST to start with null
\-------------------------------------------------------*/
#define \
blankSpoligoST(\
   spoligoSTPtr\
){\
   if((spoligoSTPtr)->idStr)\
      *(spoligoSTPtr)->idStr = '\0';\
   \
   (spoligoSTPtr)->codeUL = 0;\
   \
   if((spoligoSTPtr)->lineageStr)\
      *(spoligoSTPtr)->lineageStr  = '\0';\
   \
   if((spoligoSTPtr)->sitStr)\
      *(spoligoSTPtr)->sitStr  = '\0';\
   \
   if((spoligoSTPtr)->countriesStr)\
      *(spoligoSTPtr)->countriesStr  = '\0';\
} /*blankSpoligoST*/

/*-------------------------------------------------------\
| Fun02: initSpoligoST
|   - Sets all pointers to null and other variables
|     (currently none) to defaults in an spoligoST struct
| Input:
|   - spoligoSTPtr:
|     o Point to an spoligoST structure to initialize
| Output:
|   - Modifies:
|     o All points to point to null
|     o All non-pointers (none) to be default values
\-------------------------------------------------------*/
#define \
initSpoligoST(\
   spoligoSTPtr\
){\
   (spoligoSTPtr)->idStr = 0;\
   (spoligoSTPtr)->lineageStr = 0;\
   (spoligoSTPtr)->sitStr = 0;\
   (spoligoSTPtr)->countriesStr = 0;\
   \
   blankSpoligoST((spoligoSTPtr));\
} /*initPoligoST*/

/*-------------------------------------------------------\
| Fun03: mkSpoligoAryST
|   - Makes an array of initialized spoligotype structers
| Input:
|   - numSpoligosSI:
|     o Number of spoligoST structures to make
| Output:
|   - Returns:
|     o Pointer to spoligoST array
|     o 0 for memory errors
\-------------------------------------------------------*/
struct spoligoST *
mkSpoligoAryST(
   signed int numSpoligosST
);

/*-------------------------------------------------------\
| Fun04: freeSpoligoSTStack
|   - Frees an spoligoST struct on the stack
| Input:
|   - spoligoSTPtr:
|     o Pointer to an spoligoST structure to free
| Output:
|   - Frees:
|     o All variables in spoligoST and sets to null
\-------------------------------------------------------*/
void
freeSpoligoSTStack(
   struct spoligoST *spoligoSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeSpoligoST
|   - Frees an spoligoST struct (on heap)
| Input:
|   - spoligoSTPtr:
|     o Pointer to an spoligoST structure to free
| Output:
|   - Frees:
|     o SpoligoST
\-------------------------------------------------------*/
void
freeSpoligoST(
   struct spoligoST *spoligoSTPtr
);

/*-------------------------------------------------------\
| Fun06: freeSpoligoSTAry
|   - Frees an array of spoligoST struct (on heap)
| Input:
|   - spoligoArySTPtr:
|     o Pointer to an spoligoST array structure to free
|   - numSpoligosSI:
|     o Number of spoligoST structers in spoligoArySTPtr
| Output:
|   - Frees:
|     o spoligoArySTPtr and all its elements
\-------------------------------------------------------*/
void
freeSpoligoSTAry(
   struct spoligoST *spoligoArySTPtr,
   signed int numSpoligosSI
);

/*-------------------------------------------------------\
| Fun07: swapSpoligos
|   - Swaps values in two spoligoST structures
| Input:
|   - firstSpoligoSTPtr
|     o Pointer to first spoligoST structure to swap
|   - secSpoligoSTPtr
|     o Pointer to second spoligoST structure to swap
| Output:
|   - Modifies:
|     o Values in firstSpoligoSTPtr to be values in
|       secSpoligoSTPtr
|     o Values in secSpoligoSTPtr to be values in
|       firstSpoligoSTPtr
\-------------------------------------------------------*/
#define \
swapSpoligos(\
   firstSpoligoSTPtr,\
   secSpoligoSTPtr\
){\
   signed char *tmpMacStr = 0;\
   unsigned long tmpMacUL = 0;\
   \
   tmpMacStr = (firstSpoligoSTPtr)->idStr;\
   (firstSpoligoSTPtr)->idStr = (secSpoligoSTPtr)->idStr;\
   (secSpoligoSTPtr)->idStr = tmpMacStr;\
   \
   tmpMacUL = (firstSpoligoSTPtr)->codeUL;\
   (firstSpoligoSTPtr)->codeUL=(secSpoligoSTPtr)->codeUL;\
   (secSpoligoSTPtr)->codeUL = tmpMacUL;\
   \
   tmpMacStr = (firstSpoligoSTPtr)->lineageStr;\
   (firstSpoligoSTPtr)->lineageStr =\
      (secSpoligoSTPtr)->lineageStr;\
   (secSpoligoSTPtr)->lineageStr = tmpMacStr;\
   \
   tmpMacStr = (firstSpoligoSTPtr)->sitStr;\
   (firstSpoligoSTPtr)->sitStr=(secSpoligoSTPtr)->sitStr;\
   (secSpoligoSTPtr)->sitStr = tmpMacStr;\
   \
   tmpMacStr = (firstSpoligoSTPtr)->countriesStr;\
   (firstSpoligoSTPtr)->countriesStr =\
      (secSpoligoSTPtr)->countriesStr;\
   (secSpoligoSTPtr)->countriesStr = tmpMacStr;\
} /*swapSpoligos*/

/*-------------------------------------------------------\
| Fun08: sortSpoligoSTAry
|  - Sorts an array of spoligoST structures by least to
|    greatest with shell sort
| Input:
|  - spoligoArySTPtr:
|    o Point to an array of spoligoST structures to sort
|  - startUL:
|    o First element to start sorting at
|  - endUL:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o spoligoArySTPtr array to be sorted form least to
|       greatest
\-------------------------------------------------------*/
#define \
sortSpoligoSTAry(\
   spoligoArySTPtr,\
   startUL,\
   endUL\
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '  - Sorts an array of spoligoST structures by least to
   '    greatest with shell sort
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun08 sec01:
   '    - Variable declerations
   '  o fun08 sec02:
   '    - Find the number of rounds to sort for
   '  o fun08 sec03:
   '    - Sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Number of elements to sort*/\
  unsigned long numElmMacUL = (endUL) - (startUL);\
  \
  /*Number of sorting rounds*/\
  unsigned long subMacUL = 0;\
  unsigned long nextElmMacUL = 0;\
  unsigned long lastElmMacUL = 0;\
  unsigned long elmOnMacUL = 0;\
  \
  /*Get arrays to sort from the matrix (for sanity)*/\
  \
  /*Variables to incurment loops*/\
  unsigned long ulIndexMac = 0;\
  unsigned long ulElmMac = 0;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/\
  subMacUL = 1; /*Initialzie first array*/\
  \
  while(subMacUL < numElmMacUL - 1)\
     subMacUL = (3 * subMacUL) + 1;\
  \
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
  \
  while(subMacUL > 0)\
  { /*loop trhough all sub arrays sort the subarrays*/\
    for( \
       ulIndexMac = 0;\
       ulIndexMac <= subMacUL;\
       ++ulIndexMac\
    ){ /*For each element in the subarray*/\
      for(ulElmMac = ulIndexMac;\
          ulElmMac + subMacUL <= endUL;\
          ulElmMac += subMacUL\
      ){ /*Loop; swap each nth element of the subarray*/\
        nextElmMacUL = ulElmMac + subMacUL;\
        \
        if(\
             (spoligoArySTPtr)[ulElmMac].codeUL\
           > (spoligoArySTPtr)[nextElmMacUL].codeUL\
        ){ /*If I need to swap an element*/\
          swapSpoligos(\
             &(spoligoArySTPtr)[ulElmMac],\
             &(spoligoArySTPtr)[nextElmMacUL]\
          ); /*Swap values*/\
          \
          lastElmMacUL = ulElmMac;\
          elmOnMacUL = ulElmMac;\
          \
          while(lastElmMacUL >= subMacUL)\
          { /*loop; move swapped element back*/\
            lastElmMacUL -= subMacUL;\
            \
            if(\
                 (spoligoArySTPtr)[elmOnMacUL].codeUL\
               > (spoligoArySTPtr)[lastElmMacUL].codeUL\
            ) break; /*Positioned the element*/\
            \
            \
            swapSpoligos(\
               &(spoligoArySTPtr)[elmOnMacUL],\
               &(spoligoArySTPtr)[lastElmMacUL]\
            ); /*Swap values*/\
            \
            elmOnMacUL = lastElmMacUL;\
          } /*loop; move swapped element back*/\
        } /*If I need to swap elements*/\
      } /*Loop; swap each nth element of the subarray*/\
    } /*For each element in the subarray*/\
    \
    subMacUL = (subMacUL - 1) / 3; /*Move to next round*/\
  } /*loop through all sub arrays to sort the subarrays*/\
} /*sortSpoligoSTAry*/

/*-------------------------------------------------------\
| Fun09: findSpoligoBarcode
|  - Finds an spoligo barcode in an array of spoligoST
|    structures using an binary search
| Input:
|  - spoligoSTAryPtr:
|    o Pointer to an array of spoligoST structures to
|      search
|  - codeUL:
|    o Barcode to look up
|  - lenAryUI:
|    o Number of spoligo type lineages (index 1)
|      (length of spoligoSTAryPtr)
| Output:
|  - Returns:
|    o The nearest index at or after codeUL
|    o -1 for lineage not found
\-------------------------------------------------------*/
#define \
findSpoligoBarcode(\
   spoligoSTAryPtr,\
   codeUL,\
   lenAryUI\
)({\
   signed int midMacSI = 0;\
   signed int rightHalfMacSI = (lenAryUI) - 1;\
   signed int leftHalfMacSI = 0;\
   \
   while(leftHalfMacSI <= rightHalfMacSI)\
   { /*Loop: Search for the query lineage*/\
      midMacSI = (leftHalfMacSI + rightHalfMacSI) >> 1;\
     \
     if((codeUL) > (spoligoSTAryPtr)[midMacSI].codeUL)\
         leftHalfMacSI = midMacSI + 1;\
     \
     else if((codeUL)<(spoligoSTAryPtr)[midMacSI].codeUL)\
         rightHalfMacSI = midMacSI - 1;\
     \
     else\
        break; /*Found lineage*/\
   } /*Loop: Search for the query lineage*/\
   \
   /*See if the lineage was found*/\
   if(midMacSI < lenAryUI) \
   { /*If: the lineage may have been found*/ \
      midMacSI |= \
         (signed int) \
         -( \
               (codeUL) \
            != (spoligoSTAryPtr)[midMacSI].codeUL \
          ); \
      /*goes to -1 if the codes are not equal*/ \
   } /*If: the lineage may have been found*/ \
   \
   else \
      midMacSI = -1; /*lineage not found*/ \
   \
   midMacSI;\
}) /*findSpoligoBarcode*/

/*-------------------------------------------------------\
| Fun10: readSpoligoDb
|   - Reads in an database of spoligotypes and returns
|     an array of spoligoST structures sorted by barcodes
| Input:
|   - dbFileStr:
|     o C-string with name of database to read in
|   - numSpoligosSIPtr:
|     o Pointer to int to hold the the number of
|       spoligotypes in the database
|   - errSC:
|     o Pointer to an char to hold any errors
| Output:
|   - Modifies:
|     o numSpoligosSIPtr to hold the number of extracted
|       spoligotypes
|     o errSC to hold the error
|       - 0 for no errors
|       - fileErr_tbSpoligo for file errors
|       - memErr_tbSpoligo for memory errors
|   - Returns:
|     o pointer to an array of spoligoST structures with
|       the database
|     o 0 for errors
\-------------------------------------------------------*/
struct spoligoST *
readSpoligoDb(
   signed char *dbFileStr,
   signed int *numElmSIPtr,
   signed char *errSC
);

/*-------------------------------------------------------\
| Fun11: pSpoligoHead
|   - Print out the header for the spoligotype output
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
|     o the header to the output file
\-------------------------------------------------------*/
void
pSpoligoHead(
   signed char fragBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: pSpoligo
|   - Print out an spoligotype and matching lineage
| Input:
|   - idStr:
|     o C-string with id/name of sequence
|   - codeAryUI:
|     o unsigned int array with the spoligotype counts
|   - fragmentBl:
|     o 1: fragment mode, do not find lineages
|     o 0: complete reads find lineages
|   - numSupUI:
|     o unsigned int with the number of reads that had at
|       least one spacer (for fragment mode)
|   - spoligoAryST:
|     o Array of spoligotype lineages to get lineage from
|   - numSpoligosSI:
|     o Length of spoligoAryST (index 1)
|   - outFILE:
|     o FILE pointer with file to print to
\-------------------------------------------------------*/
void
pSpoligo(
   signed char *idStr,
   unsigned int *codeAryUI,
   signed char fragmentBl,
   unsigned int numSupUI,
   struct spoligoST *spoligoAryST,
   signed int numSpoligosSI,
   void *outFILE
);

#endif

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
