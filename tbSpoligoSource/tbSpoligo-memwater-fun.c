/*########################################################
# Name: tbSpoligo-memwater-fun.c
#   - Holds function for detecting spoligotypes using
#     an memory efficent waterman alignment. Slow, but
#     works.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun-01: getSpoligoRefs
'     - gets the sequence for each spoligotypes
'   o fun-02: fxSpoligoSearch
'     - Searches for spoligotypes in fastx sequences using
'       the memory efficent waterman
'   o fun-03: samSpoligoSearch
'     - Searchs for spoligotypes in sam files
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

#include "tbSpoligo-memwater-fun.h"

#include <stdio.h>

#include "../memwater/memwater.h"
#include "../memwater/alnSetStruct.h"
#include "../memwater/seqStruct.h"

#include "../generalLib/samEntryStruct.h"

/*.h only libraries*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "tbSpoligo-errors.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .h #include "../memwater/alnSeqDefaults.h"
!   o .h #include "../generalLib/base10StrToNum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-01: getSpoligoRefs
|   - gets the sequence for each spoligotypes
| Input:
|   - spoligoFileStr:
|     o C-string with spoligotype sequences
|   - numSpoligosSIPtr:
|     o Pointer to an int with the number of spoligotype
|       sequences in the returned spoligotype array
|   - errSCPtr:
|     o Pointer to character to hold the error type
| Output:
|   - Modifies:
|     o numSpoligosSI to hold the number of spoligotypes
|       in the returned array
|     o errSCPtr to hold the error type
|       - 0 for no errors
|       - fileErr_tbSpoligo for file errors
|       - memErr_tbSpoligo for memory errors
|   - Returns:
|     o array of seqStructs for success
|     o 0 for an error
\-------------------------------------------------------*/
void *
getSpoligoRefs(
   signed char *spoligoFileStr,
   signed int *numSpoligosSIPtr,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC:
   '   - gets the sequence for each spoligotypes
   '   o fun-01 sec-01:
   '     - variable declerations
   '   o fun-01 sec-02:
   '     - initalize pointers and open spolitotype file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sshort lenBuffSS = 2 << 10;
   schar buffStr[lenBuffSS + 1];
   schar *tmpStr = 0;
   ulong bytesReadUL = 0;

   schar newLineBl = 1; /*1: On an new line*/

   sint siSpoligo = 0;/*Counter for getting spoligotypes*/
   struct seqStruct *seqHeapAryST = 0;
   uchar errUC = 0;
   uchar cpErrUC = 0;

   FILE *spoligoFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - initalize pointers and open spolitotype file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   *numSpoligosSIPtr = 0;

   spoligoFILE = fopen((char *) spoligoFileStr, "r");

   if(! spoligoFILE)
      goto fileErr_fun01_sec05_sub03;
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - Find the number of sequences in the file
   ^   o fun-01 sec-03 sub-01:
   ^     - Get the first line of file and start loop
   ^   o fun-01 sec-03 sub-02:
   ^     - Start loop & check if have new line in buffer
   ^   o fun-01 sec-03 sub-03:
   ^     - If missing newline, get next part of file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-03 Sub-01:
   *   - Get the first line of file and start loop
   \*****************************************************/

   bytesReadUL =
      fread(
         buffStr,
         sizeof(char),
         lenBuffSS,
         spoligoFILE
      ); /*Read in the file*/

   buffStr[bytesReadUL] = '\0';
   newLineBl = 1;
   siSpoligo = 0;
   tmpStr = buffStr;
   goto checkHead_fun01_sec03_sub02;

   /*****************************************************\
   * Fun-01 Sec-03 Sub-02:
   *   Start loop & check if have new line in buffer
   \*****************************************************/

   while(bytesReadUL)
   { /*Loop: Find the number of sequences*/
      tmpStr += ulEndStrLine(tmpStr);

      if(*tmpStr == '\n')
      { /*If: I found an new line*/
         newLineBl = 1;

         if(*(tmpStr + 1) == '\0')
         { /*If: new line is at the end of the buffer*/
            getBuff_fun01_sec03:;

            if(! bytesReadUL)
               break; /*Make sure not EOF*/

            bytesReadUL =
               fread(
                  buffStr,
                  sizeof(char),
                  lenBuffSS,
                  spoligoFILE
               ); /*Read in the file*/

            buffStr[bytesReadUL] = '\0';
            tmpStr = buffStr;
         } /*If: new line is at the end of the buffer*/

         checkHead_fun01_sec03_sub02:;

         while(*tmpStr < 33) ++tmpStr;

         if(*tmpStr == '\0')
            goto getBuff_fun01_sec03;

         /*Count the number of fastq headers*/
         siSpoligo += ((*tmpStr == '>') & newLineBl);
      } /*If: I found an new line*/

      /**************************************************\
      * Fun-01 Sec-03 Sub-03:
      *   - If missing newline, get next part of file
      \**************************************************/

      else
      { /*Else: I am still on the old line*/
         bytesReadUL =
            fread(
               buffStr,
               sizeof(char),
               lenBuffSS,
               spoligoFILE
            ); /*Read in the file*/

         buffStr[bytesReadUL] = '\0';
         tmpStr = buffStr;
      } /*Else: I am still on the old line*/
   } /*Loop: Find the number of sequences*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-04:
   ^   - Read in the sequences
   ^   o fun-01 sec-04 sub-01:
   ^     - Allocate memory for the sequence arrays
   ^   o fun-01 sec-04 sub-02:
   ^     - Read in the sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-04 Sub-01:
   *   - Allocate memory for the sequence arrays
   \*****************************************************/

   seqHeapAryST = 0;

   siSpoligo <<= 1; /*Acount for having both directions*/

   seqHeapAryST=
      malloc(siSpoligo * sizeof(struct seqStruct));

   if(! seqHeapAryST)
      goto memErr_fun01_sec05_sub02;

   /*Initialize the sequence structure array*/
   for(
      *numSpoligosSIPtr = 0;
      *numSpoligosSIPtr < siSpoligo;
      ++(*numSpoligosSIPtr)
   ) initSeqST(&seqHeapAryST[*numSpoligosSIPtr]);

   /*****************************************************\
   * Fun-01 Sec-04 Sub-02:
   *   - Read in the sequences
   \*****************************************************/

   fseek(spoligoFILE, 0, SEEK_SET);

   for(
      *numSpoligosSIPtr = 0;
      *numSpoligosSIPtr < siSpoligo;
      (*numSpoligosSIPtr) += 2
   ){ /*Loop: Read in the sequences*/
      errUC =
         readFaSeq(
            spoligoFILE,
            &seqHeapAryST[*numSpoligosSIPtr]
         ); /*Read in the first sequence*/

      if(errUC)
      { /*If: I had an error*/
         if(errUC & 64)
            goto memErr_fun01_sec05_sub02;

         if(errUC & 2)
            goto fileErr_fun01_sec05_sub03;
      } /*If: I had an error*/

      cpErrUC =
         cpSeqST(
            &seqHeapAryST[*numSpoligosSIPtr + 1],
            &seqHeapAryST[*numSpoligosSIPtr]
         );

      if(cpErrUC)
         goto memErr_fun01_sec05_sub02;

      reverseComplementSeq(
         &seqHeapAryST[*numSpoligosSIPtr + 1]
      ); /*Make the reverse complement sequence*/

      seqToLookupIndex(
         seqHeapAryST[*numSpoligosSIPtr].seqStr
      ); /*For alignments*/

      seqToLookupIndex(
         seqHeapAryST[*numSpoligosSIPtr + 1].seqStr
      ); /*For alingments*/

      if(errUC & ~1)
      { /*If: I hade fewer spoligotypes than expected*/
         *numSpoligosSIPtr +=  2;
         break;
      } /*If: I hade fewer spoligotypes than expected*/
   }  /*Loop: Read in the sequences*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-05:
   ^   - Clean up
   ^   o fun-01 sec-03 sub-02:
   ^     - Clean up when there are no errors
   ^   o fun-01 sec-03 sub-02:
   ^     - Clean up when there are memory errors
   ^   o fun-01 sec-03 sub-03:
   ^     - Clean up when there are file errors
   ^   o fun-01 sec-03 sub-04:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-05 Sub-01:
   *   - Clean up when there are no errors
   \*****************************************************/

   fclose(spoligoFILE);
   spoligoFILE = 0;

   return seqHeapAryST;

   /*****************************************************\
   * Fun-01 Sec-05 Sub-02:
   *   - Clean up when there are memory errors
   \*****************************************************/

   memErr_fun01_sec05_sub02:;

   *errSCPtr = memErr_tbSpoligo;

   goto errCleanUp_fun01_sec05_sub04;

   /*****************************************************\
   * Fun-01 Sec-05 Sub-03:
   *   - Clean up when there are file errors
   \*****************************************************/

   fileErr_fun01_sec05_sub03:;

   *errSCPtr = fileErr_tbSpoligo;

   goto errCleanUp_fun01_sec05_sub04;

   /*****************************************************\
   * Fun-01 Sec-05 Sub-04:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_fun01_sec05_sub04:;

   freeSeqAryST(seqHeapAryST, *numSpoligosSIPtr);
   seqHeapAryST = 0;

   if(spoligoFILE)
      fclose(spoligoFILE);

   spoligoFILE = 0;

   return 0;
} /*getSpoligoRefs*/

/*-------------------------------------------------------\
| Fun-02: fxSpoligoSearch
|   - Searches for spoligotypes in fastx sequences using
|      the memory efficent waterman
| Input:
|   - fx_seqSTPtr:
|     o Pointer to an seqStruct with the sequence to
|       search (sequence as characters. This function
|       converts)
|   - spoliogAry_SeqSTPtr:
|     o Pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun-01: getSpoligoRefs
|       return)
|   - minPercScoreF:
|     o Float with minimum percentage to count an
|       spoligotype
|   - codeStr:
|     o C-string to hold the spoligotype barcode. This
|       needs to be the length of the number of
|       spoliogtypes
|   - pProgressBl:
|     o 1: Print progress reports to the screen (stderr)
|     o 0: Do not print progress reports
|   - alnSetSTPtr:
|     o Pointer to an alnSet structer with alignment
|       settings
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Prints:
|     o Status to stderr if user requested
|   - Returns:
|     o 0 for success
|     o memErr_tbSpoligo for memory errors
\-------------------------------------------------------*/
signed char
fxSpoligoSearch(
   void *fx_SeqSTPtr,
   void *spoligoAry_SeqSTPtr,
   signed int numSpoligosSI,
   float minPercScoreF,
   signed char *codeStr,
   signed char pProgressBl,
   void *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02:
   '   - Searches for spoligotypes in fastx sequences using
   '      the memory efficent waterman
   '   o fun-02 sec-01:
   '     - Variable declerations
   '   o fun-02 sec-02:
   '     - Check if there are spoligotypes
   '   o fun-02 sec-03:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siSpoligo = 0;
   signed char revBl = 0;    /*Check reverse complement*/

   float percScoreF = 0;
   slong maxScoreSL = 0;
   slong scoreSL = 0;
   ulong spoligoStartUL = 0; /*Start on spoligotype seq*/
   ulong spoligoEndUL = 0;   /*End on spoligotype seq*/
   ulong startAlnUL = 0;     /*First aligned base in seq*/
   ulong endAlnUL = 0;       /*Last alignned base in seq*/

   struct seqStruct *fxSeqST =
     (struct seqStruct *) fx_SeqSTPtr;

   struct seqStruct *spoligoAryST =
     (struct seqStruct *) spoligoAry_SeqSTPtr;

   struct alnSet *settings= (struct alnSet *) alnSetSTPtr;

   *codeStr = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Check if there are spoligotypes
   ^   o fun-02 sec-02 sub-01:
   ^     - Check if an spoligotype is in forward sequence
   ^   o fun-02 sec-02 sub-02:
   ^     - Check if spoligotype is in reverse sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Check if an spoligotype is in forward sequence
   \*****************************************************/
   
   seqToLookupIndex(fxSeqST->seqStr);

   for(
      siSpoligo = 0;
      siSpoligo < numSpoligosSI;
      siSpoligo += 2
   ){ /*Loop: Find the matching spoligotypes*/
      if(pProgressBl)
      { /*If: The user wanted progress reports*/
         if(! revBl)
            fprintf(
               stderr,
               "checking foward spoligotype %i\n",
               1 + (siSpoligo >> 1)
            );

         else
            fprintf(
               stderr,
               "checking reverse spoligotype %i\n",
               1 + (siSpoligo >> 1)
            );
      } /*If the user wanted progress reports*/

      maxScoreSL =
           (float) spoligoAryST[siSpoligo].lenSeqUL
         * def_matchScore_tbSpoligoWater;

      scoreSL =
         memWater(
            &spoligoAryST[siSpoligo + revBl],
            fxSeqST,
            &spoligoStartUL,
            &spoligoEndUL,
            &startAlnUL,
            &endAlnUL,
            settings 
         );

       percScoreF = (float) scoreSL / (float) maxScoreSL;

       if(percScoreF >= minPercScoreF)
       { /*If: I had an match*/
          codeStr[siSpoligo >> 1] = '1';
          continue;
       } /*If: I had an match*/

      /*I am ignoring memory errors*/

      /**************************************************\
      * Fun-02 Sec-02 Sub-02:
      *   - Check if spoligotype is in reverse sequence
      \**************************************************/

      if(pProgressBl)
      { /*If: The user wanted progress reports*/
         if(! revBl)
            fprintf(
               stderr,
               "Trying the reverse spoligotype %i\n",
               1 + (siSpoligo >> 1)
            );

         else
            fprintf(
               stderr,
               "Trying the forward spoligotype %i\n",
               1 + (siSpoligo >> 1)
            );
      } /*If the user wanted progress reports*/

      maxScoreSL =
         (float) spoligoAryST[siSpoligo].lenSeqUL *
         def_matchScore_tbSpoligoWater;

      scoreSL =
         memWater(
            &spoligoAryST[siSpoligo + (! revBl)],
            fxSeqST,
            &spoligoStartUL,
            &spoligoEndUL,
            &startAlnUL,
            &endAlnUL,
            settings 
         );

       percScoreF = (float) scoreSL / (float) maxScoreSL;

       if(percScoreF >= minPercScoreF)
       { /*If: I found an hit*/
          codeStr[siSpoligo >> 1] = '1';
          revBl = !revBl; /*likely future hits are same*/
       } /*If: I found an hit*/

       else
          codeStr[siSpoligo >> 1] = '0';
   } /*Loop: Find the matching spoligotypes*/
      
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   codeStr[siSpoligo >> 1] = '\0';
   lookupIndexToSeq(fxSeqST->seqStr);
   return 0;
} /*fxSpoligoSearch*/

/*-------------------------------------------------------\
| Fun-03: samSpoligoSearch
|   - Searchs for spoligotypes in sam files
| Input:
|   - samSTPtr:
|     o Pointer to an samEntry structure with sequence to
|       search for spoligotypes
|   - spoliogAry_SeqSTPtr:
|     o Pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun-01: getSpoligoRefs
|       return)
|   - drStartSI:
|     o Starting position of the direct repeat region on
|       the reference
|   - drEndSI:
|     o Ending position on the direct repeat region on the
|       reference
|   - minPercScoreF:
|     o Float with minimum percentage to count an
|       spoligotype
|   - codeStr:
|     o C-string to hold the spoligotype barcode. This
|       needs to be the length of the number of
|       spoliogtypes
|   - alnSetSTPtr:
|     o Pointer to an alnSet structer with alignment
|       settings
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Returns:
|     o 0 for success
|     o noSpoligo_tbSpoligo
|     o memErr_tbSpoligo for memory errors
\-------------------------------------------------------*/
signed char
samSpoligoSearch(
   void *samSTPtr,
   void *spoligoAry_SeqSTPtr,
   signed int numSpoligosSI,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   signed char *codeStr,
   void *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-03 TOC:
   '   - Searches for spoligotypes in sam files
   '   o fun-03 sec-01:
   '     - Variable declerations
   '   o fun-03 sec-02:
   '     - Find direct repeat (DR) region coordinates
   '   o fun-03 sec-03:
   '     - Run the spoligo checks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int siCig = 0;
   int siNt = 0;
   uint refPosUI = 0;

   struct samEntry *samST = (struct samEntry *) samSTPtr;
   struct seqStruct seqST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-02:
   ^   - Find direct repeat (DR) region coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samST->refEndUI <= dirStartSI)
      return noSpoligo_tbSpoligo; /*DR Not present*/

   if(samST->refStartUI >= dirEndSI)
      return noSpoligo_tbSpoligo; /*DR Not present*/

   initSeqST(&seqST);

   siNt = samST->cigValAryI[0];
   refPosUI = samST->refStartUI;

   samEntryFindRefPos(
      samST,
      siCig,
      siNt,
      dirStartSI,
      refPosUI,
      seqST.offsetUL
   );

   if(samST->refEndUI > dirEndSI)
   { /*If: the read extends beyond the direct repeat*/
      seqST.endAlnUL = seqST.offsetUL;

      samEntryFindRefPos(
         samST,
         siCig,
         siNt,
         dirEndSI,
         refPosUI,
         seqST.endAlnUL
      );
   } /*If: the read extends beyond the direct repeat*/

   else
      seqST.endAlnUL = samST->readLenUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-03:
   ^   - Run the spoligo checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqST.seqStr = samST->seqStr;

   return
      fxSpoligoSearch(
         &seqST,
         spoligoAry_SeqSTPtr,
         numSpoligosSI,
         minPercScoreF,
         codeStr,
         0,
         alnSetSTPtr
      );
} /*samSpoligoSearch*/

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
