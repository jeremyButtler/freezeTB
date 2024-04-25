/*########################################################
# Name: tbSpoligo-readDb
#   - reads in an databse of known spoligo types
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o .h st-01: spoligoST
'     - Holds an single lineage for an spoligotype
'   o .h fun-01: blankSpoligoST
'     - Blanks all variables in and spoligoST structure
'   o .h fun-02: initSpoligoST
'      - Sets all pointers to null and other variables
'       (none) to defaults in an spoligoST struct
'   o fun-03: mkSpoligoAryST
'     - Makes array of initialized spoligotype structers
'   o fun-04: freeSpoligoSTStack
'     - Frees an spoligoST struct on the stack
'   o fun-05: freeSpoligoST
'     - Frees an spoligoST struct (on heap)
'   o fun-06: freeSpoligoSTAry
'     - Frees an array of spoligoST struct (on heap)
'   o .h fun-07: swapSpoligos
'     - Swaps values in two spoligoST structures
'   o .h fun-08: sortSpoligoSTAry
'     - Sorts an array of spoligoST structures by least to
'       greatest with shell sort
'   o .h fun-09: findSpoligoBarcode
'     - Finds an spoligo barcode in an array of spoligoST
'       structures using an binary search
'   o fun-10: readSpoligoDb
'     - Reads in an database of spoligotypes and returns
'       an array of spoligoST structures sorted by barcode
'   o fun-11: pSpoligo
'     - Print out an spoligotype and matching lineage
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

#include "tbSpoligo-readDb.h"

#include <stdio.h>

/*.h files only*/
#include "../generalLib/ulCpStr.h"
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/genMath.h"
#include "tbSpoligo-errors.h"

/*-------------------------------------------------------\
| Fun-03: mkSpoligoAryST
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
){
   sint iSpoligo = 0;
   struct spoligoST *retST = 0;

   retST=malloc(numSpoligosST * sizeof(struct spoligoST));

   if(! retST)
      return 0;

   for(iSpoligo = 0; iSpoligo < numSpoligosST; ++iSpoligo)
      initSpoligoST(&retST[iSpoligo]);

   return retST;
} /*mkSpoligoAryST*/

/*-------------------------------------------------------\
| Fun-04: freeSpoligoSTStack
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
){
   if(! spoligoSTPtr)
      return;

   free(spoligoSTPtr->idStr);
   free(spoligoSTPtr->lineageStr);
   free(spoligoSTPtr->sitStr);
   free(spoligoSTPtr->countriesStr);

   initSpoligoST(spoligoSTPtr);
} /*freeSpoligoStack*/

/*-------------------------------------------------------\
| Fun-05: freeSpoligoST
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
){
   freeSpoligoSTStack(spoligoSTPtr);
   free(spoligoSTPtr);
} /*freeSpoligoST*/

/*-------------------------------------------------------\
| Fun-06: freeSpoligoSTAry
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
){
   sint siSpoligo = 0;

   for(siSpoligo=0; siSpoligo <numSpoligosSI; ++siSpoligo)
      freeSpoligoSTStack(&spoligoArySTPtr[siSpoligo]);

   free(spoligoArySTPtr);
} /*freeSpoligoSTAry*/

/*-------------------------------------------------------\
| Fun-10: readSpoligoDb
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-10: TOC
   '   o fun-07 sec-01:
   '     - Variable declerations
   '   o fun-07 sec-02:
   '     - Open the database file
   '   o fun-07 sec-03:
   '     - Find the number of lines (lineages) in database
   '   o fun-07 sec-04:
   '     - Allocate memory for the database
   '   o fun-07 sec-05:
   '     - Read in the database
   '   o fun-07 sec-06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-10 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint lenBuffUI = 0;
   schar *buffHeapStr = 0;

   sint numLinesSI = 0;
   uint lenLongestLineUI = 0;
   uint lenLineUI = 0;
   uint tmpUI = 0;
   ulong bytesUL = 0;

   ulong ulComma = ulCpMakeDelim(',');
   uint posUI = 0;

   struct spoligoST *retHeapST = 0;
   FILE *dbFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-10 Sec-02:
   ^   - Open the database file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   dbFILE = fopen((char*) dbFileStr, "r");

   if(! dbFILE)
      goto fileErr_fun10_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-10 Sec-03:
   ^   - Find the number of lines (lineages) in database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    lenBuffUI = 1 << 10;
    buffHeapStr = malloc((lenBuffUI + 1) * sizeof(char));

    if(! buffHeapStr)
      goto memErr_fun10_sec06_sub02;

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
        tmpUI = ulEndStrLine(&buffHeapStr[posUI]);
        posUI += tmpUI;
        lenLineUI += tmpUI;

        if(buffHeapStr[posUI] == '\n')
        { /*If: I have an new line*/
           ++numLinesSI;

           lenLongestLineUI = 
              noBranchMax(
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
   ^ Fun-10 Sec-04:
   ^   - Allocate memory for the database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(lenBuffUI < lenLongestLineUI)
   { /*If: I need to resize the buffer*/
      lenBuffUI = lenLongestLineUI + 1; /*+1 for null*/
      free(buffHeapStr);
      buffHeapStr = malloc((lenBuffUI +1) * sizeof(char));

      if(! buffHeapStr)
         goto memErr_fun10_sec06_sub02;
   } /*If: I need to resize the buffer*/

   retHeapST = mkSpoligoAryST(numLinesSI);

   if(! retHeapST)
         goto memErr_fun10_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-10 Sec-05:
   ^   - Read in the database
   ^   o fun-10 sec-05 sub-01:
   ^     - Get past the header, initialize, and start loop
   ^   o fun-10 sec-05 sub-02:
   ^     - Copy the strain id
   ^   o fun-10 sec-05 sub-03:
   ^     - Get the barcode (binary number)
   ^   o fun-10 sec-05 sub-04:
   ^     - Skip the octal entry
   ^   o fun-10 sec-05 sub-05:
   ^     - Get the lineage entry
   ^   o fun-10 sec-05 sub-06:
   ^     - Get the SIT entry
   ^   o fun-10 sec-05 sub-07:
   ^     - Get the countries seen in entry
   ^   o fun-10 sec-05 sub-08:
   ^     - Move to the next lineage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-10 Sec-05 Sub-01:
   *   - Get past the header, initialize, and start loop
   \*****************************************************/

   /*This will never happen, but it also avoids an
   `   compliler warning
   */
   if(! fgets((char *) buffHeapStr, lenBuffUI, dbFILE))
      goto fileErr_fun10_sec06_sub02;

   *numElmSIPtr = 0;

   while(fgets((char *) buffHeapStr, lenBuffUI, dbFILE))
   { /*Loop: Read in the database*/
      posUI = 0;

      /**************************************************\
      * Fun-10 Sec-05 Sub-02:
      *   - Copy the strain id
      \**************************************************/

      tmpUI = cLenStr((char *) &buffHeapStr[posUI], ',');

      retHeapST[*numElmSIPtr].idStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].idStr)
         goto memErr_fun10_sec06_sub02;

      ulCpStr(
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
      * Fun-10 Sec-05 Sub-03:
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
      * Fun-10 Sec-05 Sub-04:
      *   - Skip the octal entry
      \**************************************************/

      posUI += ulLenStr(&buffHeapStr[posUI],ulComma,',');
      ++posUI; /*Get off the comma*/

      /**************************************************\
      * Fun-10 Sec-05 Sub-05:
      *   - Get the lineage entry
      \**************************************************/

      tmpUI = ulLenStr(&buffHeapStr[posUI], ulComma, ',');

      retHeapST[*numElmSIPtr].lineageStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].lineageStr)
         goto memErr_fun10_sec06_sub02;

      ulCpStr(
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
      * Fun-10 Sec-05 Sub-06:
      *   - Get the SIT entry
      \**************************************************/

      tmpUI = ulLenStr(&buffHeapStr[posUI], ulComma, ',');

      retHeapST[*numElmSIPtr].sitStr =
         malloc((tmpUI + 1) * sizeof(char));

      if(! retHeapST[*numElmSIPtr].sitStr)
         goto memErr_fun10_sec06_sub02;

      ulCpStr(
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
      * Fun-10 Sec-05 Sub-07:
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
         goto memErr_fun10_sec06_sub02;

      ulCpStr(
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
      * Fun-10 Sec-05 Sub-08:
      *   - Move to the next lineage entry
      \**************************************************/

      ++(*numElmSIPtr);
   } /*Loop: Read in the database*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-10 Sec-06:
   ^   - clean up
   ^   o fun-10 sec-06 sub-01:
   ^     - clean up after success
   ^   o fun-10 sec-06 sub-02:
   ^     - deal with memory errors
   ^   o fun-10 sec-06 sub-03:
   ^     - deal with file errors
   ^   o fun-10 sec-06 sub-04:
   ^     - clean up after errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-10 Sec-06 Sub-01:
   *   - clean up after success
   \*****************************************************/

   sortSpoligoSTAry(retHeapST, 0, (*numElmSIPtr - 1));

   free(buffHeapStr);
   buffHeapStr = 0;

   fclose(dbFILE);
   dbFILE = 0;

   return retHeapST;

   /*****************************************************\
   * Fun-10 Sec-06 Sub-02:
   *   - deal with memory errors
   \*****************************************************/

   memErr_fun10_sec06_sub02:;

   *errSC = memErr_tbSpoligo;

   goto errCleanUp_fun10_sec06_sub02;

   /*****************************************************\
   * Fun-10 Sec-06 Sub-03:
   *   - deal with file errors
   \*****************************************************/

   fileErr_fun10_sec06_sub02:;

   *errSC = fileErr_tbSpoligo;

   goto errCleanUp_fun10_sec06_sub02;

   /*****************************************************\
   * Fun-10 Sec-06 Sub-04:
   *   - clean up after errors
   \*****************************************************/

   errCleanUp_fun10_sec06_sub02:;

   free(buffHeapStr);
   buffHeapStr = 0;

   freeSpoligoSTAry(retHeapST, *numElmSIPtr);
   retHeapST = 0;

   if(dbFILE)
      fclose(dbFILE);

   dbFILE = 0;

   return 0;
} /*readSpoligoDB*/

/*-------------------------------------------------------\
| Fun-11: pSpoligo
|   - Print out an spoligotype and matching lineage
| Input:
|   - idStr:
|     o C-string with id/name of sequence
|   - codeStr:
|     o C-string with the spoligotype barcode for idStr
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
   signed char *codeStr,
   struct spoligoST *spoligoAryST,
   signed int numSpoligosSI,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC:
   '   - Print out an spoligotype and matching lineage
   '   o fun-11 sec-01:
   '     - Variable declerations
   '   o fun-11 sec-02:
   '     - Convert barcode to numeric & "octal" formats
   '   o fun-11 sec-03:
   '     - Find the lineage and print out the entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siDig = 0;
   schar tmpC = 0;

   /*For getting the "octal" number*/
   schar octalStr[65];
   sint lenOctalSI = 0;

   sint indexSI = 0; /*Index of lineage in database*/

   ulong codeUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^   - Convert the barcode to numeric & "octal" formats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(codeStr[siDig] != '\0')
   { /*Loop: Translate the barcode to number and octal*/
      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] = codeStr[siDig] - 48;
      
      ++siDig;

      if(codeStr[siDig] == '\0')
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] <<= 1;
      octalStr[lenOctalSI] |= (codeStr[siDig] - 48);

      ++siDig;

      if(codeStr[siDig] == '\0')
      { /*If: I have converted the barcode*/
         octalStr[lenOctalSI] += 48;
         ++lenOctalSI;
         break;
      } /*If: I have converted the barcode*/

      codeUL <<= 1;
      codeUL |= (codeStr[siDig] - 48);

      octalStr[lenOctalSI] <<= 1;
      octalStr[lenOctalSI] |= (codeStr[siDig] - 48);
      octalStr[lenOctalSI] += 48;

      ++lenOctalSI;
      ++siDig;
   } /*Loop: Translate the barcode to number and octal*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-03:
   ^   - Find the lineage and print out the entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*This is here to remove spaces*/
   octalStr[lenOctalSI] = '\0';

   siDig = 0;
   while(idStr[siDig++] > 32) ;
   tmpC = idStr[siDig];
   idStr[siDig] = '\0';

   if(spoligoAryST)
   { /*If: I have lineages to check*/
      indexSI =
         findSpoligoBarcode(
            spoligoAryST,
            codeUL,
            numSpoligosSI           
         ); /*Find the lineage*/

      if(indexSI < 0)
         goto noLineage_fun11_sec04;

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
      noLineage_fun11_sec04:;

      fprintf(
         (FILE *) outFILE,
         "%s\tNA\t%s\t\'%s\tNA\tNA\tNA\n",
         idStr,
         codeStr,
         octalStr
      );
   } /*Else: There is no lineage*/

   idStr[siDig] = tmpC; 
} /*pSpoligo*/

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
