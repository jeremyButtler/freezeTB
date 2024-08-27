/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' miruFx SOF: Start Of File
'   - has functios to use kmerFind to identify MIRU
'     lineages in reads
'   o header:
'     - included libraries
'   o .c fun01: swap_refST_kmerFind_miruFx
'     - swaps two refST_kmerFind structures around
'   o fun02: readTbl_miruFx
'     - reads in a miru table for fastq processing
'   o fun03: kmerFind_miruFx
'     - scans an seqST structure for MIRU-VNTR lineages
'       using kmerFind
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

#include "../genLib/charCp.h"
#include "../genLib/seqST.h"

#include "../genAln/alnSet.h"
#include "../genAln/kmerFind.h"

#include "miruTbl.h"

/*no .c files*/
#include "../genLib/dataTypeShortHand.h"
#include "tbMiruDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/samEntry.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/memwater.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "../genLib/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: swap_refST_kmerFind_miruFx
|   - swaps two refST_kmerFind structures around
| Input:
|   - refAryST:
|     o array of refST_kmerFind structs
|   - firstSI:
|     o index of first refST_kmerFind struct to swap
|   - secSI:
|     o index of second refST_kmerFind struct to swap
| Output:
|   - Mofdifies:
|     o refAryST to have firstSI and secSI index's swapped
\-------------------------------------------------------*/
void
swap_refST_kmerFind_miruFx(
   struct refST_kmerFind *refAryST,
   signed int firstSI,
   signed int secSI
){
   sint *siSwapPtr = 0;
   uint *uiSwapPtr = 0;
   float fSwap = 0;
   struct seqST *seqSwapPtr = 0;

   refAryST[firstSI].minKmersUI ^=
      refAryST[secSI].minKmersUI;
   refAryST[secSI].minKmersUI ^=
      refAryST[firstSI].minKmersUI;
   refAryST[firstSI].minKmersUI ^=
      refAryST[secSI].minKmersUI;


   refAryST[firstSI].mateSI ^=
      refAryST[secSI].mateSI;
   refAryST[secSI].mateSI ^=
      refAryST[firstSI].mateSI;
   refAryST[firstSI].mateSI ^=
      refAryST[secSI].mateSI;


   fSwap = refAryST[firstSI].maxForScoreF;
   refAryST[firstSI].maxForScoreF =
      refAryST[secSI].maxForScoreF;
   refAryST[secSI].maxForScoreF = fSwap;


   fSwap = refAryST[firstSI].maxRevScoreF;
   refAryST[firstSI].maxRevScoreF =
      refAryST[secSI].maxRevScoreF;
   refAryST[secSI].maxRevScoreF = fSwap;


   refAryST[firstSI].lenAryUI ^=
      refAryST[secSI].lenAryUI;
   refAryST[secSI].lenAryUI ^=
      refAryST[firstSI].lenAryUI;
   refAryST[firstSI].lenAryUI ^=
      refAryST[secSI].lenAryUI;


   siSwapPtr = refAryST[firstSI].forKmerArySI;
   refAryST[firstSI].forKmerArySI =
      refAryST[secSI].forKmerArySI;
   refAryST[secSI].forKmerArySI = siSwapPtr;


   siSwapPtr = refAryST[firstSI].revKmerArySI;
   refAryST[firstSI].revKmerArySI =
      refAryST[secSI].revKmerArySI;
   refAryST[secSI].revKmerArySI = siSwapPtr;


   uiSwapPtr = refAryST[firstSI].forRepAryUI;
   refAryST[firstSI].forRepAryUI =
      refAryST[secSI].forRepAryUI;
   refAryST[secSI].forRepAryUI = uiSwapPtr;


   uiSwapPtr = refAryST[firstSI].revRepAryUI;
   refAryST[firstSI].revRepAryUI =
      refAryST[secSI].revRepAryUI;
   refAryST[secSI].revRepAryUI = uiSwapPtr;


   seqSwapPtr = refAryST[firstSI].forSeqST;
   refAryST[firstSI].forSeqST =
      refAryST[secSI].forSeqST;
   refAryST[secSI].forSeqST = seqSwapPtr;

   seqSwapPtr = refAryST[firstSI].revSeqST;
   refAryST[firstSI].revSeqST =
      refAryST[secSI].revSeqST;
   refAryST[secSI].revSeqST = seqSwapPtr;
} /*swap_refST_kmerFind_miruFx*/

/*-------------------------------------------------------\
| Fun02: readTbl_miruFx
|   - reads in a miru table for fastq processing
| Input:
|   - miruTblStr:
|     o c-string with path to miru table to use
|   - primStr:
|     o path to primers to align
|   - lenKmerUC:
|     o length of one kmer
|   - refAryST:
|     o pointer to pointer to refST_kmerFind structure
|       to hold reference sequences (should be null)
|   - lenRefUI:
|     o pinter to signed int to hold number primers in
|       refAryST
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to link
|       with this reference set
|   - minPercKmersF:
|     o float with minimum percentage of kmers to start
|       considering an window supports an spacer
|   - percExtraNtInWinF:
|     o float with percentage of extra nucleotides to
|       store in one window (beyond reference length)
|   - percWinShiftF:
|     o float with percentage of bases to shift for each
|       new window in tblSTPtr
|   - alnSetPtr:
|     o pointer to an alnSet structure with the alignment
|       settings (for finding max possible score)
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o refAryST: to be an array of primer sequences
|     o lenRefSI: to have number of primers read in
|     o tblSTPtr: to be setup for refAryST
|     o errSCPtr:
|       - 0 for no errors
|       - def_fileErr_tbMiruDefs for file error
|       - def_memErr_tbMiruDefs for memory error
|   - Returns:
|     o pointer to a set up miruTbl structure
\-------------------------------------------------------*/
struct miruTbl *
kmerFindSetup_miruTbl(
   signed char *miruTblStr,  /*tsv with path to table*/
   signed char *primTsvStr,  /*tsv with primers*/
   unsigned int lenKmerUC,   /*length of one kmer*/
   struct refST_kmerFind **refAryST,
   signed int *lenRefSI,     /*number of primers*/
   struct tblST_kmerFind *tblSTPtr,
   float minPercKmersF,
   float percExtraNtInWinF,
   float percWinShiftF,
   struct alnSet *alnSetPtr,
   signed char *errSCPtr     /*holds error message*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - reads in a miru table for fastq processing
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - set up MIRU table
   '   o fun02 sec03:
   '     - read in primers
   '   o fun02 sec04:
   '     - organize primer ids by MIRU table
   '     - this is not a very efficent method, but then I
   '       expect very few primers to be input
   '   o fun02 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siAmp = 0;
   sint siRef = 0;

   struct miruTbl *miruHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - set up MIRU table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   miruHeapST =
      get_miruTbl(
        miruTblStr,
        errSCPtr
      ); /*read in miru table*/

   if(*errSCPtr)
      goto errCleanUpfun02_sec05; /*failed to get table*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - read in primers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*refAryST)
   { /*If: reference array is not blank*/
      freeHeapAry_refST_kmerFind(
         *refAryST,
         *lenRefSI
      );

      *lenRefSI = 0;
      *refAryST = 0;
   } /*If: reference array is not blank*/

   *refAryST =
      tsvToAry_refST_kmerFind(
         primTsvStr,
         lenKmerUC,
         lenRefSI,
         minPercKmersF,
         tblSTPtr,
         percExtraNtInWinF,
         percWinShiftF,
         alnSetPtr,
         errSCPtr
      ); /*read in primer sequences + set up table*/

   if(! *refAryST)
      goto memErrfun02_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - organize primer ids by MIRU table
   ^   - this is not a very efficent method, but then I
   ^     expect very few primers to be input
   ^   o fun02 sec04 sub01:
   ^     - find reference primer position in MIRU table
   ^   o fun02 sec04 sub02:
   ^     - swap references around
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - find reference primer position in MIRU table
   \*****************************************************/

   while(siRef < *lenRefSI)
   { /*Loop: organize primers*/
      if(
         ! eql_charCp(
            (*refAryST)[siRef].forSeqST->idStr,
            miruHeapST->ampAryST[siRef >> 1].idStr,
            '\0'
         )
      ){ /*If: primers are in the correct location*/
         siRef += 2;
         continue;
      } /*If: primers are in the correct location*/

      for(
         siAmp = 0;
         siAmp < miruHeapST->numPrimSI;
         ++siAmp
      ){ /*Loop: make sure ids in correct order*/
         if(
            ! eql_charCp(
               (*refAryST)[siRef].forSeqST->idStr,
               miruHeapST->ampAryST[siAmp].idStr,
               '\0'
            )
         ) break; /*found primer location*/
      } /*Loop: make sure ids in correct order*/

      /**************************************************\
      * Fun02 Sec04 Sub02:
      *   - swap references around
      \**************************************************/

      swap_refST_kmerFind_miruFx(
         *refAryST,
         siAmp << 1,
         siRef
      );

      swap_refST_kmerFind_miruFx(
         *refAryST,
         (siAmp << 1) + 1,
         siRef + 1
      );
   } /*Loop: organize primers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto retfun02_sec05;

   memErrfun02_sec05:;
   *errSCPtr = def_memErr_tbMiruDefs;
   goto errCleanUpfun02_sec05;

   errCleanUpfun02_sec05:;
   if(miruHeapST)
      freeHeap_miruTbl(miruHeapST);
   miruHeapST = 0;
   goto retfun02_sec05;

   retfun02_sec05:;
   return miruHeapST;
} /*kmerFindSetup_miruTbl*/

/*-------------------------------------------------------\
| Fun03: kmerFind_miruFx
|   - scans an seqST structure for MIRU-VNTR lineages
|     using kmerFind
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings
|   - refAryST
|     o array of refST_kmerFind structures with reference
|       (primer) sequences to search for
|     o needs to be same length as
|       miruTblSTPtr->numPrimSI * 2
|   - fudgeSI:
|     o offset length by when comparing to lingeage length
|   - tblSTPtr:
|     o pointer to tblST_kmerFind structure with MIRU-VNTR
|       primers to scan for and settings
|   - seqSTPtr:
|     o pointer to seqST struct with sequence to scan
|   - minPercScoreF:
|     o minimum percent score to keep an alignment
|   - maxAmpLenSI:
|     o maximum amplicon length before discarding
|       a match
|   - alnSetPtr:
|     o pointer to alnSet sturct with alignment settings
| Output:
|   - Modifies:
|     o miruTblST to have detected lineages incurmented
|   - Returns:
|     o 0 if found lineage
|     o 1 if could not find lineage
|     o 2 for an error
\-------------------------------------------------------*/
signed char
kmerFind_miruFx(
   struct miruTbl *miruTblSTPtr, /*MIRU lineage table*/
   struct tblST_kmerFind *tblSTPtr,/*settings*/
   struct refST_kmerFind *refAryST,/*primer sequences*/
   signed int fudgeSI,           /*lineage match range*/
   struct seqST *seqSTPtr,       /*query sequence*/
   float minPercScoreF,          /*min score to keep map*/
   signed int maxAmpLenSI,       /*max amplicon length*/
   struct alnSet *alnSetPtr      /*alignment settings*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - scans an seqST structure for MIRU-VNTR lineages
   '     using kmerFind
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '   o fun03 sec0x:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar firstTimeBl = 0;
   sint siPrim = 0;
   schar matchBl = 0;
   schar errSC = 0;

   /*need for alignment*/
   slong scoreSL = 0;
   ulong startUL = 0;
   ulong endUL = 0;
   ulong ignoreStartUL = 0;
   ulong ignoreEndUL = 0;

   /*stores best hit*/
   slong *scoreHeapArySL = 0;

   ulong *startHeapAryUL = 0;
   ulong *endHeapAryUL = 0;

   schar *dirHeapArySC = 0;

   /*for lineages*/
   sint readLenSI = 0;
   sint linIndexSI = 0;
   sint *lenArySI = 0;
   sint *hitsArySI = 0;

   /*other variables*/
   schar retLinSC = 1; /*return if found lineages*/
   struct seqST *oldSeqST = 0;


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   scoreHeapArySL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(slong)
   );

   if(! scoreHeapArySL)
      goto memErr_fun03_sec0x_sub02;


   startHeapAryUL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(slong)
   );

   if(! startHeapAryUL)
      goto memErr_fun03_sec0x_sub02;


   endHeapAryUL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(slong)
   );

   if(! endHeapAryUL)
      goto memErr_fun03_sec0x_sub02;


   dirHeapArySC = calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(slong)
   );

   if(! dirHeapArySC)
      goto memErr_fun03_sec0x_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - find primer sites
   ^   o fun03 sec03 sub01:
   ^     - set up structures for kmerFind
   ^   o fun03 sec03 sub02:
   ^     - find MIRU-VNTR sites in read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - set up structures for kmerFind
   \*****************************************************/

   oldSeqST = tblSTPtr->seqSTPtr;
   tblSTPtr->seqSTPtr = seqSTPtr;

   blank_tblST_kmerFind(
      tblSTPtr,
      0
   );

   seqToIndex_alnSet(tblSTPtr->seqSTPtr->seqStr);

   /*****************************************************\
   * Fun03 Sec03 Sub02:
   *   - find MIRU-VNTR sites in read
   \*****************************************************/

   do{ /*Loop: find primers*/
      errSC =
         nextSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         ); /*move to next window in read*/

      for(
         siPrim = 0;
         siPrim < miruTblSTPtr->numPrimSI << 1;
         ++siPrim
      ){ /*Loop: check if primers in chunk*/
         matchBl =
            findRefInChunk_kmerFind(
               tblSTPtr,
               &refAryST[siPrim],
               alnSetPtr,
               minPercScoreF,
               &scoreSL,
               &startUL,
               &endUL,
               &ignoreStartUL,
               &ignoreEndUL
            ); /*See if have match*/

         if(scoreSL > scoreHeapArySL[siPrim])
            goto newBest_fun03_sec03_sub02;

         else if(scoreSL < scoreHeapArySL[siPrim]) ;

         else if(
               (siPrim & 1)
            && (matchBl & 2)
         ) goto newBest_fun03_sec03_sub02;

         else if(
               ! (siPrim & 1)
            && ! (matchBl & 2)
         ){ /*Else If: forward primer on reverse read*/
            newBest_fun03_sec03_sub02:;

            scoreHeapArySL[siPrim] = scoreSL;
            startHeapAryUL[siPrim] = startUL;
            endHeapAryUL[siPrim] = endUL;

            if(matchBl & 2)
               dirHeapArySC[siPrim] = 1; /*reverse*/
            else
               dirHeapArySC[siPrim] = 0; /*foward*/
         } /*Else If: forward primer on reverse read*/
      } /*Loop: check if primers in chunk*/

   } while(! errSC);
     /*Loop: find primers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - find lineages
   ^   o fun03 sec03 sub01:
   ^     - find length of MIRU-VNTR site + start loop
   ^   o fun03 sec03 sub02:
   ^     - assign lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - find length of MIRU-VNTR site + start loop
   \*****************************************************/

   for(
      siPrim = 0;
      siPrim < miruTblSTPtr->numPrimSI << 1;
      siPrim += 2
   ){ /*Loop: find lineages*/
      if(dirHeapArySC[siPrim] == dirHeapArySC[siPrim + 1])
         continue; /*primers in same direction*/

      if(endHeapAryUL[siPrim +1] > startHeapAryUL[siPrim])
      { /*If: forward read*/
         readLenSI =
              endHeapAryUL[siPrim + 1]
            - startHeapAryUL[siPrim];

         if(readLenSI < 0)
            continue; /*reverse primer before forward*/
      } /*If: forward read*/

      else
      { /*Else: reverse read*/
         readLenSI =
              endHeapAryUL[siPrim]
            - startHeapAryUL[siPrim + 1];

         if(readLenSI < 0)
            continue; /*forward primer beofre reverse*/
      } /*Else: reverse read*/

      if(readLenSI > maxAmpLenSI)
         continue; /*amplicon is to long*/

      /**************************************************\
      * Fun03 Sec04 Sub02:
      *   - assign lineages
      \**************************************************/

      linIndexSI =
         getLinIndex_miruTbl(
            readLenSI,
            fudgeSI,
            siPrim >> 1, /*index of primer in table*/
            miruTblSTPtr
         ); /*see if have a lineage*/

      /*add read to tally for its sight*/
      ++(miruTblSTPtr->ampAryST[siPrim >> 1].mapReadsSI);

      if(linIndexSI < 0)
      { /*If: no lineages*/
         ++(miruTblSTPtr->ampAryST[siPrim << 1].noLinSI);
         continue;
      } /*If: no lineages*/

      retLinSC = 0; /*at least one lineage matched*/

      lenArySI =
         miruTblSTPtr->ampAryST[siPrim >> 1].ampLenArySI;

      hitsArySI =
         miruTblSTPtr->ampAryST[siPrim >> 1].hitsArySI;

      while(
            lenArySI[linIndexSI] - fudgeSI <= readLenSI
         && lenArySI[linIndexSI] + fudgeSI >= readLenSI
      ){ /*Loop: incurment all matching lineages*/
         ++hitsArySI[linIndexSI];
         ++linIndexSI;

         if(linIndexSI >= miruTblSTPtr->numLinSI)
            break; /*no more lineages in range*/
      } /*Loop: incurment all matching lineages*/
   } /*Loop: find lineages*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec0x:
   ^   - clean up
   ^   o fun03 sec0x sub01:
   ^     - no error clean up
   ^   o fun03 sec0x sub02:
   ^     - memory error clean up
   ^   o fun03 sec0x sub03:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec0x Sub01:
   *   - no error clean up
   \*****************************************************/

   indexToSeq_alnSet(tblSTPtr->seqSTPtr->seqStr);
   goto cleanUp_fun03_sec0x_sub03;

   /*****************************************************\
   * Fun03 Sec0x Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun03_sec0x_sub02:;
   retLinSC = 2;
   goto cleanUp_fun03_sec0x_sub03;

   /*****************************************************\
   * Fun03 Sec0x Sub03:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun03_sec0x_sub03:;

   /*transfer orignal sequence structure back*/
   tblSTPtr->seqSTPtr = oldSeqST;
   oldSeqST = 0;
   
   if(scoreHeapArySL)
      free(scoreHeapArySL);

   scoreHeapArySL = 0;

   if(startHeapAryUL)
      free(startHeapAryUL);

   startHeapAryUL = 0;

   if(endHeapAryUL)
      free(endHeapAryUL);

   endHeapAryUL = 0;

   if(dirHeapArySC)
      free(dirHeapArySC);

   dirHeapArySC = 0;


   return retLinSC;
} /*kmerFind_miruFx*/

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
