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

#include "../genLib/charCp.h"
#include "../genBio/seqST.h"

#include "../genAln/alnSet.h"
#include "../genAln/kmerFind.h"

#include "miruTbl.h"

/*no .c files*/
#include "tbMiruDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genBio/samEntry.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/memwater.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
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
   signed int *siSwapPtr = 0;
   unsigned int *uiSwapPtr = 0;
   float fSwap = 0;
   struct seqST *seqSwapPtr = 0;


   refAryST[firstSI].lenKmerUC ^=
      refAryST[secSI].lenKmerUC;
   refAryST[secSI].lenKmerUC ^=
      refAryST[firstSI].lenKmerUC;
   refAryST[firstSI].lenKmerUC ^=
      refAryST[secSI].lenKmerUC;


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

   signed int siAmp = 0;
   signed int siRef = 0;

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

   signed char firstTimeBl = 0;
   signed int siPrim = 0;
   signed char matchBl = 0;
   signed char errSC = 0;

   /*need for alignment*/
   signed long scoreSL = 0;
   unsigned long startUL = 0;
   unsigned long endUL = 0;
   unsigned long ignoreStartUL = 0;
   unsigned long ignoreEndUL = 0;

   /*stores best hit*/
   signed long *scoreHeapArySL = 0;

   unsigned long *startHeapAryUL = 0;
   unsigned long *endHeapAryUL = 0;

   signed char *dirHeapArySC = 0;

   /*for lineages*/
   signed int readLenSI = 0;
   signed int linIndexSI = 0;
   signed int *lenArySI = 0;
   signed int *hitsArySI = 0;

   /*other variables*/
   signed char retLinSC = 1; /*return if found lineages*/
   struct seqST *oldSeqST = 0;


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   scoreHeapArySL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(signed long)
   );

   if(! scoreHeapArySL)
      goto memErr_fun03_sec0x_sub02;


   startHeapAryUL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(signed long)
   );

   if(! startHeapAryUL)
      goto memErr_fun03_sec0x_sub02;


   endHeapAryUL =
      calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(signed long)
   );

   if(! endHeapAryUL)
      goto memErr_fun03_sec0x_sub02;


   dirHeapArySC = calloc(
         miruTblSTPtr->numPrimSI << 1,
         sizeof(signed long)
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
