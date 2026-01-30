/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' checkAmr SOF: Start Of File
'   - has functions to detect AMRs in sam file entrie
'   o header:
'     - included libraries
'   o .h st01: amrHit_checkAmr
'     - linked list of postive amrs for each amr check
'   o fun01: init_amrHit_checkAmr
'     - initializes amrHit_checkAmr struct (all zeros)
'   o fun02: freeHeap_amrHit_checkAmr
'     - frees a amrHit_checkAmr structure
'   o fun03: freeHeapList_amrHit_checkAmr
'     - frees list of amrHit_checkAmr structures
'   o fun04: mk_marHit_checkAmr
'     - makes heap initialized amrHit_checkAmr structer
'   o fun05: LoFFor_checkAmr
'     - gets number of indels in gene and checks for early
'       stops or missing stops and starts.
'   o fun06: LofRev_checkAmr
'     - gets number of indels and checks for early stops
'       or missing stops/starts in reverse complement gene
'   o fun07: checkAmr
'     - checks if sam entry has amr's
'   o .c fun08: pCrossRes_checkAmr
'     - print out cross resitance (report, not database)
'   o fun09: cmpAmrs_checkAmr
'     - checks if two AMR structs are the same variant
'   o fun10: flagAmrHit_checkAmr
'     - flag all duplicates in a AMR hit list
'   o fun11: pConHead_checkAmr
'     - prints header for a consensus amr check
'   o fun12: pCon_checkAmr
'     - prints out all amr's that were in a consensus
'   o fun13: readGetVarHits_checkAmr
'     - finds non-duplicate AMRs for the target variant ID
'   o fun14: pReadHead_checkAmr
'     - prints header for reads AMR table
'   o fun15: pRead_checkAmr
'     - prints AMRs detected in reads that have min depth
'   o fun16: pIdVarHead_checkAmr
'     - prints header for the read id mapped variant table
'   o fun17: pIdVarTbl_checkAmr
'     - prints table of read ids and detected AMRs
'   o fun18: samFindAmrs_checkAmr
'     - look for AMRs in sam file entries
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

#include "checkAmr.h"

#include <stdio.h>

#include "amrST.h"
#include "drugAry.h"

#include "../genLib/charCp.h"
#include "../genLib/ulCp.h"
#include "../genLib/genMath.h"
#include "../genBio/codonFun.h"
#include "../genBio/samEntry.h"

/*Only .h files*/
#include "../genLib/endLine.h"
#include "../genBio/ntTo2Bit.h"
#include "../genBio/revNtTo2Bit.h"
#include "../genBio/codonTbl.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/base10StrToNum.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: init_amrHit_checkAmr
|   - initializes amrHit_checkAmr struct (all zeros)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
void
init_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
){
   amrHitSTPtr->amrSTPtr = 0;
   amrHitSTPtr->seqPosUI = 0;
   amrHitSTPtr->nextAmr = 0;
   amrHitSTPtr->dupBl = 0;
} /*init_amrHit_checkAmr*/

/*-------------------------------------------------------\
| Fun02: freeHeap_amrHit_checkAmr
|   - frees a amrHit_checkAmr structure
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeap_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
){
   if(amrHitSTPtr)
      free(amrHitSTPtr);
} /*freeHeap_amrHit_checkAmr*/

/*-------------------------------------------------------\
| Fun03: freeHeapList_amrHit_checkAmr
|   - frees list of amrHit_checkAmr structures
| Input:
|   - amrHitListST:
|     o pointer to start of an amrHist struct list to free
| Output:
|   - Frees:
|     o all amrHit_checkAmrs structures in amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeapList_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
){
   struct amrHit_checkAmr *tmpST = amrHitListST;
   struct amrHit_checkAmr *nextHitST = 0;

   while(tmpST)
   { /*Loop: free all amrHit_checkAmr structures in list*/
      nextHitST = tmpST->nextAmr;
      freeHeap_amrHit_checkAmr(tmpST);
      tmpST = nextHitST;
   } /*Loop: free all amrHit_checkAmr structures in list*/
} /*freeHeap_amrHit_checkAmr*/

/*-------------------------------------------------------\
| Fun04: mk_amrHit_checkAmr
|   - makes heap initialized amrHit_checkAmr structer
| Input:
| Output:
|   - Returns:
|     o pointer to the new amrHit_checkAmr structure
|     o 0 for memory error
\-------------------------------------------------------*/
struct amrHit_checkAmr *
mk_amrHit_checkAmr(
   void
){
   struct amrHit_checkAmr *retST = 0;
   retST = malloc(sizeof(struct amrHit_checkAmr));

   if(retST)
      init_amrHit_checkAmr(retST);

   return retST;
} /*mk_amrHit_checkAmr*/

/*-------------------------------------------------------\
| Fun05: LoFFor_checkAmr
|   - gets number of indels in gene and checks for early
|     stops or missing stops and starts.
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       having the gene to check
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o delCntSI to hold the number of deletions
|     o insCntSI to hold the number of insertions
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LoFFor_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry *samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
){/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC: LoFFor_checkAmr
   '   - gets number of indels in gene and checks for
   '     early stops or missing stops and starts.
   '   o fun05 sec01:
   '     - variable declerations
   '   o fun05 sec02:
   '     - move to position and check start codon
   '   o fun05 sec03:
   '     - check reading frame for LoFs & get indel count
   '   o fun05 sec03:
   '     - return answer for LoF entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int extraNtSI = 0; /*extra bases; last entry*/
   signed char aaSC = 0;
   signed char LoFBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - move to position and check start codon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   findRefPos_samEntry(
     samSTPtr,
     &cigPosSI,
     &cigBaseSI,
     startGeneSI,
     &refPosSI,
     &readPosSI
   ); /*find first base in the gene*/

   *delCntSI = 0;
   *insCntSI = 0;

   if(refPosSI == startGeneSI)
   { /*If: have first base in the gene*/
      if((samSTPtr)->cigTypeStr[cigPosSI] == 'D')
         LoFBl = 1;

       else
       { /*Else: not deletion*/
          aaSC =
            codonToAA_codonFun(
               samSTPtr->seqStr[readPosSI],
               samSTPtr->seqStr[readPosSI + 1],
               samSTPtr->seqStr[readPosSI + 2]
            );

          aaSC = aaSC != 'x';

          LoFBl =
             ! (
               bactStart_codonFun(
                 (samSTPtr)->seqStr[readPosSI],
                 (samSTPtr)->seqStr[readPosSI + 1],
                 (samSTPtr)->seqStr[readPosSI + 2]
                )
             ); /*check if I have start codon*/

          LoFBl &= aaSC; /*account for masking*/
       } /*Else: not an deletion*/

   } /*If: have first base in gene*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - check reading frame for LoFs and get indel count
   ^   o fun05 sec03 sub01:
   ^     - start loop and handle insertion/mask cases
   ^   o fun05 sec03 sub02:
   ^     - get counts for deletions
   ^   o fun05 sec03 sub03:
   ^     - check snps/matchs for early stops
   ^   o fun05 sec03 sub04:
   ^     - move to the next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - start loop and handle insertion/mask cases
   \****************************************************/\

   while(refPosSI < endGeneSI)
   { /*Loop: count indels/check for LoFs (foward)*/

      switch((samSTPtr)->cigTypeStr[cigPosSI])
      { /*Switch: Check cigar entry type*/
         case 'S':
         /*Case: softmask; likely from primers/adapter*/
            readPosSI += cigBaseSI;
            ++cigPosSI;
            break;
         /*Case: softmask; likely from primers/adapter*/

         case 'I':
         /*Case: insertions*/
            refPosSI += cigBaseSI;
            *insCntSI += cigBaseSI;
            extraNtSI += cigBaseSI;

            if(extraNtSI < 3)
               break; /*Not enough bases for codon*/

            while(extraNtSI > 2)
            { /*Loop: find early stops*/
               aaSC =
                 codonToAA_codonFun(
                    samSTPtr->seqStr[readPosSI],
                    samSTPtr->seqStr[readPosSI + 1],
                    samSTPtr->seqStr[readPosSI + 2]
                 );

               readPosSI += 3;

               /*check if I had an early stop*/
               LoFBl |= (aaSC == '*');
               extraNtSI -= 3;
            } /*Loop: find early stops*/

            break;
         /*Case: insertions*/

         /***********************************************\
         * Fun05 Sec03 Sub02:\
         *   - get counts for deletions
         \***********************************************/

         case 'D':
         /*Case: deletions*/
            refPosSI += cigBaseSI;

            if(refPosSI > endGeneSI)
            { /*If: overshot gene end*/
                *delCntSI += refPosSI - endGeneSI;
                LoFBl = 1;         /*stop removed*/
            } /*If: overshot gene end*/

            else
               *delCntSI += cigBaseSI;

            break;
         /*Case: deletions*/

         /***********************************************\
         * Fun05 Sec03 Sub03:
         *   - Check snps/matchs for early stops
         \***********************************************/

         case 'M':
         case 'X':
         case '=':
         /*Case: matchs and snps*/
            refPosSI += cigBaseSI;
            extraNtSI += cigBaseSI;

            /*check if overshot gene*/
            if(refPosSI > endGeneSI)
               extraNtSI -= (refPosSI - endGeneSI) - 3;
               /*-3 to account for stop codon*/

            while(extraNtSI > 2)
            { /*Loop: check reading frame*/
               aaSC =
                 codonToAA_codonFun(
                   samSTPtr->seqStr[readPosSI],
                   samSTPtr->seqStr[readPosSI + 1],
                   samSTPtr->seqStr[readPosSI + 2]
                 );

               readPosSI += 3;
               extraNtSI -= 3;

               if(extraNtSI < 3)
                  break;  /*last codon in match/snp*/

               /*check if early stop*/
               LoFBl |= (aaSC == '*');
            } /*Loop: check reading frame*/

            /*check if last base*/
            if(refPosSI >= endGeneSI)
               LoFBl |= ((aaSC != '*') & (aaSC != 'x'));
            else
               LoFBl |= (aaSC == '*');

            break;
         /*Case: matchs and snps*/
      } /*Switch: check cigar entry type*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - move to next cigar entry
      \**************************************************/

      ++cigPosSI;

      if(cigPosSI >= (signed int) samSTPtr->cigLenUI)
         break; /*end of sequence*/

      cigBaseSI = samSTPtr->cigArySI[cigPosSI];
   } /*Loop: count indels/check for LoFs (foward)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return answer for the LoF entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return LoFBl; /*1 = found stop missing start*/
} /*LoFFor_checkAmr*/

/*-------------------------------------------------------\
| Fun06: LofRev_checkAmr
|   - gets number of indels and checks for early stops or
|     missing stops and starts in reverse complement gene
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with sequence
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o indelCntSI to hold the number of indels
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LofRev_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry* samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC: LofRev_checkAmr
   '   - gets number of indels and checks for early stops
   '     or missing stops and starts in reverse complement
   '     gene
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - move to position and check start codon
   '   o fun06 sec03:
   '     - check reading frame for LoFs & get indel count
   '   o fun06 sec03:
   '     - return answer for the LoF entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int extraNtSI = 0; /*extra bases; last entry*/
   signed char aaSC = 0;
   signed char LoFBl = 0;

   signed int readStartSI = -1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - move to position and check start codon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   findRefPos_samEntry(
     samSTPtr,
     &cigPosSI,
     &cigBaseSI,
     startGeneSI,
     &refPosSI,
     &readPosSI
   ); /*Find the first base in the gene*/

   *delCntSI = 0;
   *insCntSI = 0;

   if(refPosSI == startGeneSI)
   { /*If: have last base in gene*/

      if(samSTPtr->cigTypeStr[cigPosSI] == 'D')
        LoFBl = 1; /*Is an indel*/

      else
      { /*Else: see if is not an deletion*/
         aaSC =
            revCodonToAA_codonFun(
               samSTPtr->seqStr[readPosSI],
               samSTPtr->seqStr[readPosSI + 1],
               samSTPtr->seqStr[readPosSI + 2]
            ); /*get last codon*/

         LoFBl = ((aaSC != '*') & (aaSC != 'x'));
         readStartSI = readPosSI;
      } /*Else: see if is not deletion*/

   } /*If: have last base in gene*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - check reading frame for LoFs and get indel count
   ^   o fun06 sec03 sub01:
   ^     - start loop and handle insertion/mask cases
   ^   o fun06 sec03 sub02:
   ^     - get counts for deletions
   ^   o fun06 sec03 sub03:
   ^     - check snps/matchs for early stops
   ^   o fun06 sec03 sub04:
   ^     - move to next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec03 Sub01:
   *   - start loop and handle insertion/mask cases
   \*****************************************************/

   while(refPosSI < endGeneSI)
   { /*Loop: count indels/check for LoFs*/

      switch((samSTPtr)->cigTypeStr[cigPosSI])
      { /*Switch: Check cigar entry type*/
         case 'S':
         /*Case: softmask; likely from primers/adapter*/
            readPosSI += cigBaseSI;
            ++cigPosSI;
            break;
         /*Case: softmask; likely from primers/adapter*/

         case 'I':
         /*Case: insertions*/
            refPosSI += cigBaseSI;
            *insCntSI += cigBaseSI;
            extraNtSI += cigBaseSI;

            if(extraNtSI < 3)
               break; /*not enough bases for codon*/

            while(extraNtSI > 2)
            { /*Loop: check for early stops*/
               aaSC =
                 revCodonToAA_codonFun(
                   samSTPtr->seqStr[readPosSI],
                   samSTPtr->seqStr[readPosSI + 1],
                   samSTPtr->seqStr[readPosSI + 2]
                 );

               readPosSI += 3;

               LoFBl |= (aaSC == '*'); /*have stop?*/
               extraNtSI -= 3;
            } /*Loop: check for early stops*/

            break;
         /*Case: insertions*/

         /***********************************************\
         * Fun06 Sec03 Sub02:
         *   - get counts for deletions
         \***********************************************/

         case 'D':
         /*Case: deletions*/
            refPosSI += cigBaseSI;

            /*check if overshot*/
            if(refPosSI > endGeneSI)
            { /*If: overshot gene end*/
               *delCntSI += (refPosSI - endGeneSI);
                LoFBl = 1;  /*stop removed*/
            } /*If: I have overshot the gene end*/

            else
               *delCntSI += cigBaseSI;

            break;
         /*Case: deletions*/

         /***********************************************\
         * Fun06 Sec03 Sub03:
         *   - Check snps/matchs for early stops
         \***********************************************/

         case 'M':
         case 'X':
         case '=':
         /*Case: matchs and snps*/
            refPosSI += cigBaseSI;
            extraNtSI += cigBaseSI;

            /*check if overshot gene*/
            if(refPosSI > endGeneSI)
               extraNtSI -= (refPosSI - endGeneSI) - 3;
               /*-3 to account for stop codon*/

             while(extraNtSI > 2)
             { /*Loop: check indel reading frame*/
                if(extraNtSI == 3)
                   break;  /*last codon for entry*/
                else
                   extraNtSI -= 3;

                aaSC =
                  revCodonToAA_codonFun(
                    samSTPtr->seqStr[readPosSI],
                    samSTPtr->seqStr[readPosSI + 1],
                    samSTPtr->seqStr[readPosSI + 2]
                  );

                /*check if had early stop*/
                if(readPosSI != readStartSI)
                   LoFBl |= (aaSC == '*');

                readPosSI += 3;
               } /*Loop: Check indel reading frame*/

            /*check if is last base*/
            if(extraNtSI < 3)
               ; /*incomplete codon*/
            else if(refPosSI >= endGeneSI)
            { /*If: at end; check if have start codon*/
               aaSC =
                  revCodonToAA_codonFun(
                     samSTPtr->seqStr[readPosSI],
                     samSTPtr->seqStr[readPosSI + 1],
                     samSTPtr->seqStr[readPosSI + 2]
                  );

               aaSC = (aaSC != 'x');
               aaSC &=
                 !(
                   bactRevStart_codonFun(
                      samSTPtr->seqStr[readPosSI],
                      samSTPtr->seqStr[readPosSI + 1],
                      samSTPtr->seqStr[readPosSI + 2]
                   ) /*check if have start codon*/
                 );

               readPosSI += 3;

               LoFBl |= aaSC;
            } /*If: at end; check if have start codon*/

            else
            { /*Else: check if have early stop*/
               extraNtSI = 0;
               aaSC =
                  revCodonToAA_codonFun(
                     samSTPtr->seqStr[readPosSI],
                     samSTPtr->seqStr[readPosSI + 1],
                     samSTPtr->seqStr[readPosSI + 2]
                  );

               LoFBl |= (aaSC == '*');
               readPosSI += 3;
            } /*Else: check if have early stop*/

            break;
         /*Case: matchs and snps*/
      } /*Switch: check cigar entry type*/

      /**************************************************\
      * Fun06 Sec03 Sub04:
      *   - move to next cigar entry
      \**************************************************/

      ++cigPosSI;

      if(cigPosSI >= (signed int) samSTPtr->cigLenUI)
         break; /*end of sequence*/

      cigBaseSI = samSTPtr->cigArySI[cigPosSI];
   } /*Loop: count indels/check for LoFs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - return the answer for the LoF entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return LoFBl;
} /*LofRev_checkAmr*/

/*-------------------------------------------------------\
| Fun07: checkAmr
|   - checks if sam entry has amr's
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with sequence to check
|   - amrAryST:
|     o pointer to an amrST struct array with amr's
|   - numAmrSI:
|     o number of amrST structures in amrAryST
|   - numHitsSI:
|     o pointer to signed int to hold number detected amrs
|   - frameshiftBl:
|     o 1: check for LoFs in frameshift
|     o 0: treat frameshifts as exact matches
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - errSC:
|     o pointer to signed char to hold the error output
| Output:
|   - Modifies:
|     o numHitsSI to have number of amrs found in samSTPtr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o list of amrHit_checkAmr structs with detected amrs
|     o 0 for no amrs or memory errors
\-------------------------------------------------------*/
struct amrHit_checkAmr *
checkAmr(
   struct samEntry *samSTPtr, /*sequence to check*/
   struct amrST *amrAryST,    /*AMRs to search*/
   signed int numAmrSI,       /*length of amrAryST*/
   signed int *numHitsSI,     /*holds number amr hits*/
   signed char frameshiftBl,  /*1: frameshift/indel mode*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   signed char *errSC         /*for error reporting*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC: checkAmr
   '   - checks if sam entry has amr's
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find nearest amr coordinate to sequence
   '   o fun07 sec03:
   '     - check for AMRs
   '   o fun07 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*For finding the amr position*/
   unsigned int seqPosUI = 0;
   unsigned int refPosUI = 0;
   signed int aaSeqPosSI = 0;
   signed int aaRefPosSI = 0;

   /*For processing the cigar entries*/
   signed int cigNtSI = 0;
      /*number bases left in cigar entry*/
   signed int siCig = 0;  /*Iterate through the cigar*/

   /*for recording the number of snps/ins/dels when
   `   comparing AMRs
   */
   signed int snpSI = 0;
   signed int insSI = 0;
   signed int delSI = 0;

   /*for keeping track of the cigar positions when
   `   comparing AMRs
   */
   signed int cigPosSI = 0;
   signed int cigCountSI = 0;

   /*For checking amr's*/
   unsigned char *seqUStr = 0;
   unsigned char *amrUStr = 0;

   signed int siAmr = 0;   /*iterate through amr's list*/
   signed int siBase = 0;  /*iterate/compare amr pattern*/
   signed int siAa = 0;    /*i base for amino acid amrs*/
   signed int siMatch = 0; /*holds the last checked base*/
   signed int amrEndSI = 0;/*length of AMR*/

   signed char resBl = 0;    /*-1 is resitance; 0 is not*/

   /*for gene wide frameshift processing*/
   signed int numDelSI = 0;
      /*number deletions in frameshift*/
   signed int numInsSI = 0;
      /*number insertions in frameshift*/
   signed char lofBl = 0;
     /*1: had mising/early start/stop*/

   /*for aa amr check*/
   unsigned char base1UC = 0;    /*base 1 in amr codon*/
   unsigned char base2UC = 0;    /*base 2 in amr codon*/
   unsigned char base3UC = 0;    /*base 3 in amr codon*/
   signed char aaSC = 0; /*holds sequence AA at position*/

   /*return values/used to build amr list*/
   struct amrHit_checkAmr *amrSTList = 0;/*amr list*/
   struct amrHit_checkAmr *amrSTPtr = 0; /*to build list*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - Find the nearest amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSC = 0;

   siAmr =
      getAmr_amrST(
         amrAryST,
         (signed int) samSTPtr->refStartUI,
         numAmrSI
      ); /*Find the nearest amr to this sequence*/

   /*Check if this is at the end of the amr list*/
   if(siAmr == -1)
      goto finished_fun07_sec04_sub03; /*no AMRs*/
   
   refPosUI = samSTPtr->refStartUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - Check for AMRs
   ^   o fun07 sec03 sub01:
   ^     - check if read has full AMR
   ^   o fun07 sec03 sub02:
   ^     - check for missing starts/stops, early stops
   ^        and get indel counts (framshift mode only)
   ^   o fun07 sec03 sub03:
   ^     - move to first base in the amr pattern
   ^   o fun07 sec03 sub04:
   ^     - check if the amr has a strict match
   ^   o fun07 sec03 sub05:
   ^     - handle amino acid amr's
   ^   o fun07 sec03 sub06:
   ^     - make sure there is really resistance
   ^   o fun07 sec03 sub07:
   ^     - check if detected resistance
   ^   o fun07 sec03 sub08:
   ^     - move to next amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec03 Sub01:
   *   - check if read has an full AMR
   \*****************************************************/

   siCig = 0;
   cigNtSI = samSTPtr->cigArySI[0];

   /*Mark that this is an new frameshift*/
   if(frameshiftBl)
      frameshiftBl = 3; /*2 and 1 bit set*/
 
   while(siAmr < numAmrSI)
   { /*Loop: check if have any AMR mutations*/
      if(samSTPtr->refEndUI < amrAryST[siAmr].refPosUI +1)
         goto finished_fun07_sec04_sub03;
         /*no more AMRs*/

      /*make sure read covers full AMR*/
      amrEndSI =
           amrAryST[siAmr].refPosUI
         + amrAryST[siAmr].lenRefSeqUI;

      if(samSTPtr->refEndUI < (unsigned int) amrEndSI)
         goto nextAmr_fun07_sec03_sub08;

      if(samSTPtr->readLenUI < amrAryST[siAmr].lenAmrAaUI)
         goto nextAmr_fun07_sec03_sub08;

      if(frameshiftBl & siAmr)
      { /*If: frameshift checking is set up*/
         if(
            eql_charCp(
               amrAryST[siAmr].geneIdStr,
               amrAryST[siAmr - 1].geneIdStr,
               (signed char) '\0'
             )
         ) frameshiftBl |= 2; /*Changing genes*/
      } /*If: frameshift checking is set up*/

      /**************************************************\
      * Fun07 Sec03 Sub02:
      *   - check for missing starts/stops, early stops
      *     and get indel counts (framshift mode only)
      \**************************************************/

      if(frameshiftBl & 2)
      { /*If: have new gene to check*/
         /*check missing starts/stops and early stops*/
         if(
               amrAryST[siAmr].dirFlagSC
            == def_forwardDir_amrST
         ){ /*If: have foward gene*/
            lofBl =
               LoFFor_checkAmr(
                  amrAryST[siAmr].geneFirstRefUI,
                  amrAryST[siAmr].geneLastRefUI,
                  refPosUI,
                  seqPosUI,
                  samSTPtr,
                  siCig,
                  cigNtSI,
                  &numDelSI,
                  &numInsSI
               );
         } /*If: have foward gene*/

         else if(
               amrAryST[siAmr].dirFlagSC
            == def_revCompDir_amrST
         ){ /*Else If: have reverse complement gene*/
            lofBl =
               LofRev_checkAmr(
                  amrAryST[siAmr].geneFirstRefUI,
                  amrAryST[siAmr].geneLastRefUI,
                  refPosUI,
                  seqPosUI,
                  samSTPtr,
                  siCig,
                  cigNtSI,
                  &numDelSI,
                  &numInsSI
               );
         } /*Else If: have reverse complement gene*/

         else {}; /*unkown direction*/

         frameshiftBl &= (~2); /*remove gene flag*/
      } /*If: have a new gene to check*/

      /**************************************************\
      * Fun07 Sec03 Sub03:
      *   - move to first base in amr pattern
      \**************************************************/

      findRefPos_samEntry(
         samSTPtr,
         &siCig,
         &cigNtSI,
         amrAryST[siAmr].refPosUI,
         (signed int *) &refPosUI,
         (signed int *) &seqPosUI
      );

      /*deletion at amr site means error, except for frame
      `   shifts. there should be enough sequence around
      `   it to move onto the deletion
      */
      if(samSTPtr->cigTypeStr[siCig] == 'D')
      { /*If: entry is a deletion*/
         if(
               ! frameshiftBl
            || ! amrAryST[siAmr].frameshiftBl
         ) goto nextAmr_fun07_sec03_sub08;
      } /*If: entry is a deletion*/

      /**************************************************\
      * Fun07 Sec03 Sub04:
      *   - check if amr is a strict match
      *   o fun07 sec03 sub04 cat01:
      *     - get first amr base and set up
      *   o fun07 sec03 sub04 cat02:
      *     - check if may have a frame shift
      *   o fun07 sec03 sub04 cat03:
      *     - check if AMR pattern is amino acid sequence
      *   o fun07 sec03 sub04 cat04:
      *     - check if snp AMR is present
      *   o fun07 sec03 sub04 cat05:
      *     - check if deletion AMR is present
      *   o fun07 sec03 sub04 cat06:
      *     - check if insertion AMR is present
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat01:
      +   - get first amr base and set up
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++amrAryST[siAmr].numMapReadsUI;
      seqUStr =
          (unsigned char *) &samSTPtr->seqStr[seqPosUI];

      /*amrUStr is needed for the final checks for both
      `   nucleotide and amino acid checks
      */
      amrUStr =
         (unsigned char *) amrAryST[siAmr].amrSeqStr;

      /*set variables for keeping track of cigar entries*/
      cigPosSI = siCig;
      cigCountSI = cigNtSI;

      /*set up counters*/
      snpSI = 0;
      insSI = 0;
      delSI = 0;
      siMatch = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat02:
      +   - check if may have a frame shift
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
            frameshiftBl
         && amrAryST[siAmr].frameshiftBl
      ){ /*If: is frameshift pattern*/
         if(
              amrAryST[siAmr].wholeGeneFlagSC
            & def_geneFrameshift_amrST
         ){ /*If: is LoF entry (gene wide event)*/
            if(lofBl)
               goto isRes_fun06_sec03__sub07; /*LoF*/

            else if(ab_genMath(numInsSI - numDelSI) % 3)
               goto isRes_fun06_sec03__sub07; /*LoF*/
               /*logic
               `   - shift: numInsSi - numDelSI
               `     o gives the offset for the region
               `   - ab_shift: ab_genMath(shift):
               `     o absolute value (not negative)
               `   - ab_shift % 3:
               `     - multiples of three got to 0
               `     - non-multiples of three (always
               `       frame shifts) are > 0
               `   may miss smaller events
               */
         } /*If: is LoF entry (gene wide event)*/

         /*i am likely one base off, so I start on the
         `  next base, unless there is an deletion here
         */
         if(
               samSTPtr->cigTypeStr[cigPosSI] != 'D' 
            && samSTPtr->cigTypeStr[cigPosSI] != 'I' 
         ){ /*If: deletion or insertion*/
            --cigCountSI;

            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

            if(
                  samSTPtr->cigTypeStr[cigPosSI] != 'I'
               && amrAryST[siAmr].mutTypeStr[0] == 'i'
            ) goto skipFrame_fun07_sec03_sub04_cat03;
              /*not insertion AMR*/

            else if(
                  samSTPtr->cigTypeStr[cigPosSI] != 'D'
               && amrAryST[siAmr].mutTypeStr[0] == 'd'
            ) goto skipFrame_fun07_sec03_sub04_cat03;
              /*not deletion AMR*/
         } /*If: deletion or insertion*/

         while(
               samSTPtr->cigTypeStr[cigPosSI] == 'D' 
            || samSTPtr->cigTypeStr[cigPosSI] == 'I' 
         ){ /*Loop: count number of indels*/

            if(samSTPtr->cigTypeStr[cigPosSI] == 'D')
               ++delSI;
            else
               ++insSI;


            --cigCountSI;
	
            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

         } /*Loop: count number of indels*/

         if(ab_genMath(insSI - delSI) % 3)
            goto isRes_fun06_sec03__sub07; /*AMR detected*/
            /*same logic as `if: is LoF entry`*/

         delSI = 0;
         insSI = 0;
         cigPosSI = siCig;
         cigCountSI = cigNtSI;

         /*Else: treat as nucleotide/amnio acid pattern*/
      } /*If: is frameshift pattern*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat03:
      +   - check if AMR pattern is an amino acid sequence
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*this treats full deletions (no insertions) as
      `   snps
      */

      skipFrame_fun07_sec03_sub04_cat03:;

      if(amrAryST[siAmr].amrAaStr[0] != '0')
         goto checkAA_fun07_sec03_sub05_cat01;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat04:
      +   - check if snp AMR is present
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
      
      checkSnp_fun07_sec03_sub04_cat04:;

      if(amrAryST[siAmr].mutTypeStr[0] == 's')
      { /*If: snp mutation*/
         for(
            siBase = 0;
            (amrUStr[siBase] & ~32)
               == (seqUStr[siBase] & ~32);
            ++siBase
         ){ /*Loop : see if have extact match to the amr*/
            switch(samSTPtr->cigTypeStr[cigPosSI])
            { /*Switch: update cigar mutation coutner*/
               case 'M':
               case '=':
               case 'X':
                  break;
               default:
                  goto nextAmr_fun07_sec03_sub08;
            } /*Switch: update cigar mutation coutner*/

            /*see if I need to move to next cigar entry*/
            --cigCountSI;

            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

            if(amrUStr[siBase] == '\0')
               break;

            if(seqUStr[siBase] == '\0')
               goto nextAmr_fun07_sec03_sub08;
               /*not enough bases to tell if AMR*/
         } /*Loop : See if have extact match to the amr*/

         if(amrUStr[siBase] != '\0')
            goto nextAmr_fun07_sec03_sub08; /*not AMR*/

         goto isRes_fun06_sec03__sub07; /*is AMR*/
      } /*If: snp mutation*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat05:
      +   - check if deletion AMR is present
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(amrAryST[siAmr].mutTypeStr[0] == 'd')
      { /*Else If: deletion*/
         siBase = 0;

         if(! amrUStr) ;
         else if(amrUStr[0]) /*0 means no sequence*/
         { /*Else If: included non-deleted amr bases*/
            for(
               ;
               (amrUStr[siBase] & ~32)
                  == (seqUStr[siBase] & ~32);
               ++siBase
            ){ /*Loop : move past non-deletion bases*/
               switch(samSTPtr->cigTypeStr[cigPosSI])
               { /*Switch: update cigar mutation coutner*/
                  case 'M':
                  case '=':
                  case 'X':
                     break;
                  default:
                     goto nextAmr_fun07_sec03_sub08;
               } /*Switch: update cigar mutation coutner*/

               /*check if need move to next cigar entry*/
               --cigCountSI;

               if(cigCountSI <= 0)
               { /*If: next cigar entry*/
                  ++cigPosSI;
                  cigCountSI =
                     samSTPtr->cigArySI[cigPosSI];
               } /*If: next cigar entry*/

               if(amrUStr[siBase] == '\0')
                  break; /*end of AMR part of seqeunce*/

               if(seqUStr[siBase] == '\0')
                  goto nextAmr_fun07_sec03_sub08;
                  /*not enough bases to tell if AMR*/
            }  /*Loop : move past non-deletion bases*/
         } /*Else If: included non-deleted amr bases*/

         while(amrAryST[siAmr].refSeqStr[siBase])
         { /*Loop: find number of deletions*/
            if(samSTPtr->cigTypeStr[cigPosSI] != 'D')
               goto nextAmr_fun07_sec03_sub08;

            /*see if I need to move to next cigar entry*/
            --cigCountSI;

            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

            if(! samSTPtr->cigTypeStr[cigPosSI])
               break; /*end of read*/
         } /*Loop: find number of deletions*/

         siMatch = siBase; /*for comparison step*/

         if(amrAryST[siAmr].refSeqStr[siBase])
            goto nextAmr_fun07_sec03_sub08;
            /*not enough deletions*/

         goto isRes_fun06_sec03__sub07; /*is AMR*/
      } /*Else If: deletion*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat06:
      +   - check if insertion AMR is present
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(amrAryST[siAmr].mutTypeStr[0] == 'i')
      { /*Else If: insertion*/
         siBase = 0;

         if(! amrAryST[siAmr].refSeqStr) ;
         else if(amrAryST[siAmr].refSeqStr[0])
         { /*If: included non-inserted reference bases*/
            for(
               ;
               (amrAryST[siAmr].refSeqStr[siBase] & ~32)
                  == (seqUStr[siBase] & ~32);
               ++siBase
            ){ /*Loop : move past non-inserted bases*/
               switch(samSTPtr->cigTypeStr[cigPosSI])
               { /*Switch: update cigar mutation coutner*/
                  case 'M':
                  case '=':
                  case 'X':
                     break;
                  default:
                     goto nextAmr_fun07_sec03_sub08;
               } /*Switch: update cigar mutation coutner*/

               /*check if need move to next cigar entry*/
               --cigCountSI;

               if(cigCountSI <= 0)
               { /*If: next cigar entry*/
                  ++cigPosSI;
                  cigCountSI =
                     samSTPtr->cigArySI[cigPosSI];
               } /*If: next cigar entry*/

               if(! amrAryST[siAmr].refSeqStr[siBase])
                  break;
                  /*end of non-inserted bases*/

               if(seqUStr[siBase] == '\0')
                  goto nextAmr_fun07_sec03_sub08;
                  /*not enough bases to tell if AMR*/
            }  /*Loop : move past non-inserted bases*/
         } /*If: included non-inserted reference bases*/

         while(amrAryST[siAmr].amrSeqStr[siBase])
         { /*Loop: find number of insertions*/
            if(samSTPtr->cigTypeStr[cigPosSI] != 'I')
               goto nextAmr_fun07_sec03_sub08;
               /*needs to be an insertion*/

            if(
                 (seqUStr[siBase] & ~32)
              != (amrAryST[siAmr].amrSeqStr[siBase] & ~32)
            ) goto nextAmr_fun07_sec03_sub08;
              /*does not match profile*/

            /*see if I need to move to next cigar entry*/
            --cigCountSI;

            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

            if(! samSTPtr->cigTypeStr[cigPosSI])
               break; /*end of read*/
         } /*Loop: find number of insertions*/

         siMatch = siBase; /*for comparison step*/

         if(amrAryST[siAmr].amrSeqStr[siBase])
            goto nextAmr_fun07_sec03_sub08;
            /*not enough deletions*/

         goto isRes_fun06_sec03__sub07; /*is AMR*/
      } /*Else If: insertion*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat07:
      +   - skip lof cases (already checked)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
         goto nextAmr_fun07_sec03_sub08;

      /**************************************************\
      * Fun07 Sec03 Sub05:
      *   - handle amino acid amr's
      *   o fun07 sec03 sub05 cat01:
      *     - check if cover codon + find base offset
      *   o fun07 sec03 sub05 cat02:
      *     - check if have indel in codon
      *   o fun07 sec03 sub05 cat03:
      *     - move to the first base in target codon
      *   o fun07 sec03 sub05 cat04:
      *     - check reverse complemnt gene snps/insertions
      *   o fun07 sec03 sub05 cat05:
      *     - check forward gene snps/insertions
      *   o fun07 sec03 sub05 cat06:
      *     - else I do not know direction, look at snps
      *   o fun07 sec03 sub04 cat07:
      *     - skip lof cases (already checked)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat01:
      +   - check if cover codon + find base offset
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      checkAA_fun07_sec03_sub05_cat01:;

      /*check if have full codon coverage*/
      if(
           samSTPtr->refStartUI
         > amrAryST[siAmr].codonPosUI
      ) goto checkSnp_fun07_sec03_sub04_cat04;

      else if(
           samSTPtr->refEndUI
         < amrAryST[siAmr].codonPosUI + 2
      ) goto checkSnp_fun07_sec03_sub04_cat04;


      /*find codon start*/
      siBase = amrAryST[siAmr].codonPosUI - refPosUI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat02:
      +   - check if have indel in codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(aaIndelBl & 1) ;
      else if(*amrAryST[siAmr].mutTypeStr =='s')
      { /*If: want to ignore codon indels*/

         /*check the current base on*/
         if(samSTPtr->cigTypeStr[siCig] == 'D')
            goto nextAmr_fun07_sec03_sub08;
         else if(samSTPtr->cigTypeStr[siCig] == 'I')
            goto nextAmr_fun07_sec03_sub08;

         /*check the next bases*/
         if(siBase > -2)
         { /*If: have at least on forward base*/
            siMatch = siCig;
            snpSI = cigNtSI;

            if(snpSI == 0)
            { /*If: need to move forward*/
               ++siMatch;
               ++snpSI;
            } /*If: need to move forward*/

            else
               ++snpSI;

            if(samSTPtr->cigTypeStr[siMatch] == 'D')
               goto nextAmr_fun07_sec03_sub08;
            else if(samSTPtr->cigTypeStr[siMatch] == 'I')
               goto nextAmr_fun07_sec03_sub08;

            if(siBase >= 0)
            { /*If: have two forward bases*/
               if(snpSI == 0)
               { /*If: need to move forward*/
                  ++siMatch;
                  ++snpSI;
               } /*If: need to move forward*/

               else
                  ++snpSI;

               if(samSTPtr->cigTypeStr[siMatch] == 'D')
                  goto nextAmr_fun07_sec03_sub08;
               if(samSTPtr->cigTypeStr[siMatch] == 'I')
                  goto nextAmr_fun07_sec03_sub08;
            } /*If: have two forward bases*/
         } /*If: have at least on forward base*/

         /*check previous bases*/
         if(siBase < 0)
         { /*If: moving backwards*/

            siMatch = siCig;
            snpSI = samSTPtr->cigArySI[siCig] - cigNtSI;
               /*snps left to move back for*/

            if(snpSI <= 0)
            { /*If: need to move back*/
               --siMatch;
               snpSI = samSTPtr->cigArySI[siMatch];
            } /*If: need to move back*/

            else
               --snpSI;

            if(samSTPtr->cigTypeStr[siMatch] == 'D')
               goto nextAmr_fun07_sec03_sub08;
            else if(samSTPtr->cigTypeStr[siMatch] == 'I')
               goto nextAmr_fun07_sec03_sub08;

            if(siBase < -1)
            { /*If: moving back two bases*/
               if(snpSI <= 0)
               { /*If: need to move back*/
                  --siMatch;
                  snpSI = samSTPtr->cigArySI[siMatch];
               } /*If: need to move back*/

               else
                  --snpSI;

               if(samSTPtr->cigTypeStr[siMatch] == 'D')
                  goto nextAmr_fun07_sec03_sub08;
               if(samSTPtr->cigTypeStr[siMatch] == 'I')
                  goto nextAmr_fun07_sec03_sub08;
            } /*If: moving back two bases*/
         } /*If: moving backwards*/

         snpSI = 0;
         siMatch = 0;
      } /*If: want to ignore codon indels*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat03:
      +   - move to first base in target codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      cigPosSI = siCig;
      cigCountSI = cigNtSI;
      aaRefPosSI = refPosUI;
      aaSeqPosSI = seqPosUI;
       
      findRefPos_samEntry(
         samSTPtr,
         &cigPosSI,
         &cigCountSI,
         amrAryST[siAmr].codonPosUI,
         &aaRefPosSI,
         &aaSeqPosSI
      );

      if(   aaRefPosSI
         != (signed int) amrAryST[siAmr].codonPosUI
      ) goto nextAmr_fun07_sec03_sub08;
         /*do not have reference position*/

      /*should work out, even when reverse complement*/
      /*seqUStr += siBase;*/
      seqUStr =
         (unsigned char *) &samSTPtr->seqStr[aaSeqPosSI];

      siBase = 0; /*for finding the number of
                 ` matches/snps, inss, and dels in
                 ` sequence after AA checks (sub05)
                 */

      if((signed char *) seqUStr < samSTPtr->seqStr)
         goto nextAmr_fun07_sec03_sub08;
         /*some cases go one base off, so incomplete*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat04:
      +   - check reverse complemnt gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
         amrAryST[siAmr].dirFlagSC == def_revCompDir_amrST
      ){ /*If: reverse complement gene*/
         for(
            siAa = 0;
				siAa <
               (signed int) amrAryST[siAmr].lenAmrAaUI;
            ++siAa 
         ){ /*Loop: check codon reading frame*/
            if((signed char *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base1UC =
               (unsigned char)
               revNtTo2Bit[*seqUStr--];

            if((signed char *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base2UC =
               (unsigned char)
               revNtTo2Bit[*seqUStr--];

            if((signed char *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base3UC =
               (unsigned char)
               revNtTo2Bit[*seqUStr--];

            aaSC = codonTbl[base1UC][base2UC][base3UC];
            /*2024 WHO catalog has ? marks for
            ` non-insertions/deletions; so safe for now
            */
          
            if(aaSC == 'x') /*any aa*/
               goto nextAmr_fun07_sec03_sub08;

            else if(
                  amrAryST[siAmr].amrAaStr[siAa] == '?'
               && aaSC != amrAryST[siAmr].refAaStr[siAa]
            ){ /*Else If: amr pattern has any codon*/

               if(
                     (amrAryST[siAmr].refAaStr[siAa] | 32)
                  == 'm'
               ){ /*If: ref (non-amr) pattern has start*/
                  resBl =
                     bactStartCode_codonFun(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  if(resBl)
                  { /*If: sequence has bacterial start*/
                     resBl = 0;
                     goto nextAmr_fun07_sec03_sub08;
                  } /*If: sequence has bacterial start*/
               } /*If: ref (non-amr) pattern has start*/
            } /*Else If: amr pattern has any codon*/

            else if(aaSC != amrAryST[siAmr].amrAaStr[siAa])
               goto nextAmr_fun07_sec03_sub08;
         } /*Loop: check codon reading frame*/

         if(aaIndelBl & 2)
            goto isRes_fun06_sec03__sub07;
         else
            siMatch = amrAryST[siAmr].lenAmrSeqUI;
      } /*If: reverse complement gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat05:
      +   - check forward gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         amrAryST[siAmr].dirFlagSC == def_forwardDir_amrST
      ){ /*Else If: foward gene*/
         for(
            siAa = 0;
            siAa <
               (signed int) amrAryST[siAmr].lenAmrAaUI;
            ++siAa 
         ){ /*Loop: check codon reading frame*/
            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base1UC =
              (unsigned char) ntTo2Bit[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base2UC =
               (unsigned char) ntTo2Bit[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base3UC =
              (unsigned char) ntTo2Bit[*seqUStr++];

            aaSC = codonTbl[base1UC][base2UC][base3UC];

            /*2023 WHO catalog has ? marks for
            ` non-insertions/deletions; so am safe for now
            */
            if(aaSC == 'x') /*any aa*/
               goto nextAmr_fun07_sec03_sub08;

            else if(
                  amrAryST[siAmr].amrAaStr[siAa] == '?'
               && aaSC != amrAryST[siAmr].refAaStr[siAa]
            ){ /*Else If: pattern supports any codon*/
               if(
                      (amrAryST[siAmr].refAaStr[siAa] |32)
                   == 'm'
               ){ /*If: ref (non-amr) pattern has start*/
                  resBl =
                     bactStartCode_codonFun(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  if(resBl)
                  { /*If: bacterial start codon*/
                     resBl = 0;
                     goto nextAmr_fun07_sec03_sub08;
                  } /*If: bacterial start codon*/
               } /*If: ref (non-amr) pattern has start*/
            } /*Else If: pattern supports any codon*/

            else if(aaSC != amrAryST[siAmr].amrAaStr[siAa])
               goto nextAmr_fun07_sec03_sub08;
         } /*Loop: check codon reading frame*/

         if(aaIndelBl & 2)
            goto isRes_fun06_sec03__sub07;
         else
            siMatch = amrAryST[siAmr].lenAmrSeqUI;
      } /*Else If: foward gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat06:
      +   - else I do not know direction, look for snps
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
         goto checkSnp_fun07_sec03_sub04_cat04;

      /**************************************************\
      * Fun07 Sec03 Sub06:
      *   - make sure there is really resistance
      *   o fun07 sec03 sub06 cat01:
      *     - finish getting snps/matches, dels, and inss
      *       in amr region
      *   o fun07 sec03 sub06 cat02:
      *     - check if region & pattern have same length
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub06 Cat01:
      +   - finish getting snps/matches, dels, and inss
      +     in amr region
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*falsePositiveCheck_fun07_sec04_sub06_cat01:;*/

      while(
         siBase < (signed int) amrAryST[siAmr].lenAmrSeqUI
      ){ /*Loop: to end of amr sequence*/
         switch(samSTPtr->cigTypeStr[cigPosSI])
         { /*Switch: update cigar mutation coutner*/
            case 'M': ++snpSI; break;
            case '=': ++snpSI; break;
            case 'X': ++snpSI; break;
            case 'I': ++insSI; break;
            case 'D': ++delSI; break;
         } /*Switch: update cigar mutation coutner*/

         /*see if I need to move to next cigar entry*/
         --cigCountSI;

         if(cigCountSI <= 0)
         { /*If: next cigar entry*/
            ++cigPosSI;

            if(cigPosSI >=(signed int) samSTPtr->cigLenUI)
               goto nextAmr_fun07_sec03_sub08;
               /*not enough bases to validate*/

            cigCountSI = samSTPtr->cigArySI[cigPosSI];
         } /*If: next cigar entry*/

         ++siBase;
      } /*Loop: to end of the amr sequence*/

      while(
         siBase < (signed int) amrAryST[siAmr].lenRefSeqUI
      ){ /*Loop: to end of reference sequence*/
         switch(samSTPtr->cigTypeStr[cigPosSI])
         { /*Switch: update cigar mutation coutner*/
            case 'M': ++snpSI; break;
            case '=': ++snpSI; break;
            case 'X': ++snpSI; break;
            case 'I': ++insSI; break;
            case 'D': ++delSI; break;
         } /*Switch: update cigar mutation coutner*/

         /*see if I need to move to next cigar entry*/
         --cigCountSI;

         if(cigCountSI <= 0)
         { /*If: next cigar entry*/
            ++cigPosSI;

            if(cigPosSI < (signed int) samSTPtr->cigLenUI)
               ;
            else if(
                 siBase
              >= (signed int) amrAryST[siAmr].lenRefSeqUI
            ) break;

            else
               goto nextAmr_fun07_sec03_sub08;
               /*not enough bases to validate AMR*/

            cigCountSI = samSTPtr->cigArySI[cigPosSI];
         } /*If: next cigar entry*/

         ++siBase;
      } /*Loop: to end of reference sequence*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub06 Cat02:
      +   - check if amr region & pattern have same length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(amrUStr[siMatch] == '\0')
      { /*If: may have an match*/
         /*check if amr pattern is deletion*/
         resBl =
            (
                 (   amrAryST[siAmr].lenAmrSeqUI
                   < amrAryST[siAmr].lenRefSeqUI
                 )
               & (delSI > 0)
           );

         /*check if amr pattern is insertion*/
         resBl |=
            (
                 (   amrAryST[siAmr].lenAmrSeqUI
                   > amrAryST[siAmr].lenRefSeqUI
                 )
               & (insSI > 0)
           );

         /*check if amr pattern is snp*/
         resBl |=
           (
                amrAryST[siAmr].lenAmrSeqUI
              == amrAryST[siAmr].lenRefSeqUI
           );

         resBl &=
            (
                 (insSI + snpSI - delSI)
              == (signed int) amrAryST[siAmr].lenAmrSeqUI
            ); /*make sure lengths are equal*/
      } /*If: may have an match*/

      /**************************************************\
      * Fun07 Sec03 Sub07:
      *   - check if detected resistance
      \**************************************************/

      if(resBl)
      { /*If: found resitance mutation*/
         isRes_fun06_sec03__sub07:;

         resBl = 0;
         ++amrAryST[siAmr].numSupReadsUI;
         ++(*numHitsSI);

         if(amrSTList)
         { /*If: not first amr*/
            amrSTPtr->nextAmr = mk_amrHit_checkAmr();

            if(!amrSTPtr->nextAmr)
                goto memErr_fun07_sec04_sub02;

            amrSTPtr->nextAmr->amrSTPtr =
               &amrAryST[siAmr];
            amrSTPtr->nextAmr->seqPosUI = seqPosUI;
            amrSTPtr = amrSTPtr->nextAmr;
         } /*If: not first amr*/

         else
         { /*Else: is first amr*/
            amrSTList = mk_amrHit_checkAmr();
            amrSTPtr = amrSTList;

            if(!amrSTPtr)
                goto memErr_fun07_sec04_sub02;

            amrSTPtr->amrSTPtr = &amrAryST[siAmr];
            amrSTPtr->seqPosUI = seqPosUI;
         } /*Else: is first amr*/
      } /*If: found resitance mutation*/

      /**************************************************\
      * Fun07 Sec03 Sub08:
      *   - move to next amr
      \**************************************************/

      nextAmr_fun07_sec03_sub08:;
         ++siAmr;
   } /*Loop: check if have any AMR mutations*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - clean up and return
   ^   o fun07 sec04 sub01:
   ^     - no errors clean up
   ^   o fun07 sec04 sub02:
   ^     - memory error clean up
   ^   o fun07 sec04 sub03:
   ^     - clean up after errors (all errors)
   ^   o fun07 sec04 sub04:
   ^     - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *   - no error clean up
   \*****************************************************/

   finished_fun07_sec04_sub03:;
   *errSC = 0;
   goto ret_fun07_sec04_sub04;

   /*****************************************************\
   * Fun07 Sec04 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun07_sec04_sub02:;
   *errSC = def_memErr_amrST;
   goto errCleanUp_fun07_sec04_sub03;

   /*****************************************************\
   * Fun07 Sec04 Sub03:
   *   - clean up after errors (all errors)
   \*****************************************************/

   errCleanUp_fun07_sec04_sub03:;

   if(amrSTList)
      freeHeapList_amrHit_checkAmr(amrSTList);

   amrSTList = 0;

   goto ret_fun07_sec04_sub04;

   /*****************************************************\
   * Fun07 Sec04 Sub04:
   *   - return result
   \*****************************************************/

   ret_fun07_sec04_sub04:;
   return amrSTList;
} /*checkAmr*/

/*-------------------------------------------------------\
| Fun08: pCrossRes_checkAmr
|   - print out cross resitance (for report, not database)
| Input:
|   - amrSTPtr:
|     o pionter to amrST struct with cross resistance
|   - oufFILE:
|     o file to print cross restance to
| Output:
|   - Prints:
|     o NA if no cross resitance
|     o drug1-drug2-...-drugn if there is cross resistance
\-------------------------------------------------------*/
void
pCrossRes_checkAmr(
   struct amrST *amrSTPtr,
   signed char *drugAryStr,
   void *outFILE
){
   signed int siAmr = 0; /*index of amr on*/
   signed int siIndex = 0;
   signed char firstPrintBl = 1;
   signed char *drugStr = 0;
   unsigned long flagsUL = 0;

   for(
      siAmr=0;
      siAmr < (signed int) def_maxDrugs_amrST;
      ++siAmr
   ){ /*Loop: go though all amr elements*/
      flagsUL = amrSTPtr->crossResFlagsUL[siAmr];

      while(flagsUL)
      { /*Loop: check if flags support an antibiotic*/
         if(! (flagsUL & 1))
         { /*If: this position is not an antibiotic*/
            ++siIndex;
            flagsUL >>= 1;
            continue;
         } /*If: this position is not an antibiotic*/

         drugStr = get_drugAry(drugAryStr, siIndex);

         if(! firstPrintBl)
            fprintf(
               (FILE *) outFILE,
               "_%s",
               drugStr
            );

         else
         { /*Else: is first drug am printing*/
            firstPrintBl = 0;

            fprintf(
               (FILE *) outFILE,
               "%s",
               drugStr
            );
         } /*Else: is first drug am printing*/

         ++siIndex;
         flagsUL >>= 1;
      } /*Loop: check if flags support an antibiotic*/

   } /*Loop: go though all amr elements*/

   /*check if there was cross resitance*/
   if(firstPrintBl)
      fprintf(
         (FILE *) outFILE,
         "NA"
      ); /*no cross resistance*/
} /*pCrossRes_checkAmr*/

/*-------------------------------------------------------\
| Fun09: cmpAmrs_checkAmr
|   - checks if two AMR structs are the same variant
| Input:
|   - oneST:
|     o pointer to first amrST structures to compare
|   - twoST:
|     o pointer to second amrST structures to compare
| Output:
|   - Returns:
|     o -1 if are different by variant ID
|     o -2 if same variant ID, but different attribute
|     o 0 if are the same to checkAmr
|     o 1 if first AMR has more support
|     o 2 if second AMR has more support
\-------------------------------------------------------*/
signed char
cmpAmrs_checkAmr(
   struct amrST *oneST,
   struct amrST *twoST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - checks if two AMR structs are the same variant
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - check if have same AMRs to checkAmr
   '   o fun09 sec03:
   '     - find which AMR has the most support
   '   o fun09 sec04:
   '     - find which AMR has the most support
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siFlag = 0;
   unsigned long onePercUL = 0;
   unsigned long twoPercUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - check if have same AMRs to checkAmr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(eqlNull_ulCp(oneST->varIdStr, twoST->varIdStr))
      goto noMatch_fun09_sec04;

   for(siFlag = 0; siFlag < def_maxDrugs_amrST; ++siFlag)
   { /*Loop: check if different resistance*/
      if(
            oneST->amrFlagsUL[siFlag]
         != twoST->amrFlagsUL[siFlag]
      ) goto sameIdDiffEntry_fun09_sec04;

      if(
            oneST->crossResFlagsUL[siFlag]
         != twoST->crossResFlagsUL[siFlag]
      ) goto sameIdDiffEntry_fun09_sec04;
   }  /*Loop: check if different resistance*/

   if(oneST->highResBl != twoST->highResBl)
      goto sameIdDiffEntry_fun09_sec04;

   else if(oneST->lowResBl != twoST->lowResBl)
      goto sameIdDiffEntry_fun09_sec04;

   else if(oneST->additiveResBl != twoST->additiveResBl)
      goto sameIdDiffEntry_fun09_sec04;

   else if(oneST->gradeSC != twoST->gradeSC)
      goto sameIdDiffEntry_fun09_sec04;

   else if(
      eqlNull_ulCp(
         oneST->needsGeneStr,
         twoST->needsGeneStr
      )
   ) goto sameIdDiffEntry_fun09_sec04;

   else if(
      eqlNull_ulCp(oneST->mutTypeStr, twoST->mutTypeStr)
   ) goto sameIdDiffEntry_fun09_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - find which AMR has the most support
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! oneST->numMapReadsUI && ! twoST->numMapReadsUI)
      goto sameSup_fun09_sec04;
   else if(! oneST->numMapReadsUI)
      goto secSup_fun09_sec04;
   else if(! twoST->numMapReadsUI)
      goto firstSup_fun09_sec04;

   /*if the differences are by a fraction of a percent,
   `  then I will compare the number of supporting
   `  reads
   */
   onePercUL = oneST->numSupReadsUI;
   onePercUL /= oneST->numMapReadsUI;

   twoPercUL = twoST->numSupReadsUI;
   twoPercUL /= twoST->numMapReadsUI;

   if(onePercUL > twoPercUL)
      goto firstSup_fun09_sec04;
   else if(twoPercUL > onePercUL)
      goto secSup_fun09_sec04;
   else if(oneST->numSupReadsUI > twoST->numSupReadsUI)
      goto firstSup_fun09_sec04;
   else if(oneST->numSupReadsUI < twoST->numSupReadsUI)
      goto secSup_fun09_sec04;
   else
      goto sameSup_fun09_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - find which AMR has the most support
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   firstSup_fun09_sec04:;
      return 1; /*first AMR has more support*/

   secSup_fun09_sec04:;
      return 2; /*second AMR has more support*/

   sameSup_fun09_sec04:;
      return 0; /*AMRs are a match*/

   sameIdDiffEntry_fun09_sec04:;
      return -2; /*AMRs are a match*/

   noMatch_fun09_sec04:;
      return -1; /*AMRs are different*/
} /*cmpAmrs_checkAmr*/

/*-------------------------------------------------------\
| Fun10: flagAmrHit_checkAmr
|   - flag all duplicates in a AMR hit list
| Input:
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs to
|       flag duplicate AMRs
| Output:
|    - Modifies:
|      o dupBl in each amrHit_checkAmr struct in
|        amrHitListST to be 1 (if duplicates) or 0 if
|        unique/best AMR
\-------------------------------------------------------*/
void
flagAmrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
){
   signed char resSC = 0;
   struct amrHit_checkAmr *lastAmrST = 0;
   struct amrHit_checkAmr *scanAmrST = 0;

   while(amrHitListST->nextAmr)
   { /*Loop: find duplicate AMRs*/
      resSC =
         cmpAmrs_checkAmr(
           amrHitListST->amrSTPtr,
           amrHitListST->nextAmr->amrSTPtr
         );

      if(resSC == -1)
      { /*If: different variant IDs*/
         lastAmrST = amrHitListST->nextAmr;
         goto nextAmr_fun10;
      } /*If: different variant IDs*/

      else if(resSC == 1 || ! resSC)
      { /*Else If: next AMR is a duplicate*/
         if(! lastAmrST)
            lastAmrST = amrHitListST;
         amrHitListST->nextAmr->dupBl = 1;
         goto nextAmr_fun10;
      } /*Else If: next AMR is a duplicate*/

      else
      { /*Else: need to scan older variant ids*/
         if(! lastAmrST)
         { /*If: first variant ID match*/
            lastAmrST = amrHitListST;
            goto nextAmr_fun10;
         } /*If: first variant ID match*/

         scanAmrST = lastAmrST;

         while(scanAmrST < amrHitListST)
         { /*Loop: see if AMR is a duplicate*/
            if(scanAmrST->dupBl)
            { /*If: this AMR was a duplicate*/
               scanAmrST = scanAmrST->nextAmr;
               continue;
            } /*If: this AMR was a duplicate*/

            resSC =
               cmpAmrs_checkAmr(
                 scanAmrST->amrSTPtr,
                 amrHitListST->nextAmr->amrSTPtr
               );

            if(resSC < 0)
            { /*If: different variant IDs*/
               scanAmrST = scanAmrST->nextAmr;
               continue;
            } /*If: different variant IDs*/

            else if(resSC < 2)
            { /*Else If: equal AMRs or first is better*/
               amrHitListST->nextAmr->dupBl = 1;
               goto nextAmr_fun10;
            } /*Else If: equal AMRs or first is better*/

            else
            { /*Else: new AMR is better*/
               scanAmrST->dupBl = 1;
               goto nextAmr_fun10;
            } /*Else: new AMR is better*/
         } /*Loop: see if AMR is a duplicate*/
      } /*Else: need to scan older variant ids*/

      nextAmr_fun10:;
         amrHitListST = amrHitListST->nextAmr;
   } /*Loop: find duplicate AMRs*/
} /*flagAmrHit_checkAmr*/

/*-------------------------------------------------------\
| Fun11: pConHead_checkAmr
|   - prints header for a consensus amr check
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrHit_checkAmrList table header to outFILE
\-------------------------------------------------------*/
void
pConHead_checkAmr(
   signed char refBl,
   void *outFILE
){
   if(refBl)
      fprintf((FILE *) outFILE, "ref");

   fprintf((FILE *) outFILE, "Id\tgene\tdrug");
   fprintf((FILE *) outFILE, "Id\tgene\tdrug");
   fprintf((FILE *) outFILE, "\tcross_resistance");
   fprintf((FILE *) outFILE, "\tvariant_id\ttype\tgrade");
   fprintf((FILE *) outFILE, "\tref_pos\tseq_pos");
   fprintf((FILE *) outFILE, "\tresitance_level");
   fprintf((FILE *) outFILE, "\tresistance_additive");
   fprintf((FILE *) outFILE, "\tneeds_gene");
   fprintf((FILE *) outFILE, "\teffect\twho_comment");
   fprintf((FILE *) outFILE, "\twhole_gene");
   fprintf((FILE *) outFILE, "%s", str_endLine);
} /*pConHead_checkAmr*/

/*-------------------------------------------------------\
| Fun12: pCon_checkAmr
|   - prints out all amr's that were in a consensus
| Input:
|   - refStr:
|     o c-string with the reference id
|     o or 0/null if no reference column
|   - seqIdStr:
|     o c-string with name of sequence
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs with
|       detected AMRs
|   - refSTPtr:
|     o refList_amrST struct pointer with drugs used in
|       the database
|   - outFILE:
|     o file to print amr's to
| Output: 
|   - Prints:
|     o amr's in amrHitListST to outFILE
|   - Modifies:
|     o dupBl in amrHitListST to be 1 for duplicate
|       variant ids
\-------------------------------------------------------*/
void
pCon_checkAmr(
   signed char *refStr, /*reference id to print*/
   signed char *seqIdStr,
   struct amrHit_checkAmr *amrHitListST,
   struct refList_amrST *refSTPtr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - prints out all amr's that were in a consensus
   '   o fun12 sec01:
   '     - variable decerations
   '   o fun12 sec02:
   '     - print AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable decerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct amrHit_checkAmr *tmpST = amrHitListST;

   unsigned long amrFlagUL = 0;
   signed int flagOnSI = 0;
   signed char *drugStr = 0;
   signed char firstPrintBl = 0;
   signed int siAmr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - print AMRs
   ^   o fun12 sec02 sub01:
   ^     - print AMRs/check if already printed variant
   ^   o fun12 sec02 sub02:
   ^     - print read/gene ids
   ^   o fun12 sec02 sub03:
   ^     - print antibiotics resitant to
   ^   o fun12 sec02 sub04:
   ^     - print cross resistance
   ^   o fun12 sec02 sub05:
   ^     - print variant id, mutation type, and positions
   ^   o fun12 sec02 sub06:
   ^     - print high, unkown (normal?), or low resitance
   ^   o fun12 sec02 sub07:
   ^     - print if low resitance is additive
   ^   o fun12 sec02 sub08:
   ^     - print if restance needs a functional gene
   ^   o fun12 sec02 sub09:
   ^     - print effect of mutation
   ^   o fun12 sec02 sub10:
   ^     - print WHOs comment and if whole gene effect
   ^   o fun12 sec02 sub11:
   ^     - move onto next AMR
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec02 Sub01:
   *   - print AMRs/check if already printed variant
   \*****************************************************/

   if(! amrHitListST)
      return; /*nothing to print; avoids infinite loops*/

   /*flag duplicate AMRs (so only best printed*/
   flagAmrHit_checkAmr(amrHitListST);

   while(tmpST)
   { /*Loop: print all amr's*/

      if(tmpST->dupBl)
         goto nextAmr_fun12_sec02_sub11;

      /**************************************************\
      * Fun12 Sec02 Sub02:
      *   - print read/gene ids
      \**************************************************/

      if(refStr)
         fprintf((FILE *) outFILE, "%s\t", refStr);
      fprintf(
        (FILE *) outFILE,
        "%s\t%s\t",
        (seqIdStr),             /*name of the seq*/
        tmpST->amrSTPtr->geneIdStr /*gene name*/
      );
      
      /**************************************************\
      * Fun12 Sec02 Sub03:
      *   - print antibiotics resitant to
      \**************************************************/

      flagOnSI = 0;
      firstPrintBl = 1;
      
      for(
         siAmr = 0;
         siAmr < (signed int) def_maxDrugs_amrST;
         ++siAmr
      ){ /*Loop: run through all antibiotic flags*/
         amrFlagUL = tmpST->amrSTPtr->amrFlagsUL[siAmr];
         
         if(! amrFlagUL)
            continue; /*no resitance*/

         while(amrFlagUL)
         { /*Loop: check each flag in a flag set*/
            if(! (amrFlagUL & 1) )
               goto nextAmrFlag_fun12_sec02_sub03;
               /*not resistance to this antibiotic*/
            
            drugStr =
               get_drugAry(refSTPtr->drugAryStr,flagOnSI);
            
            if(firstPrintBl)
            { /*If: is frist drug printed for this AMR*/
               fprintf(
                  (FILE *) outFILE,
                  "%s",
                  drugStr
               );

               firstPrintBl = 0;
            } /*If: is frist drug printed for this AMR*/
            
            else fprintf((FILE *) outFILE, "_%s", drugStr);
            
             nextAmrFlag_fun12_sec02_sub03:;
            ++flagOnSI;
            amrFlagUL >>= 1;
         } /*Loop: check each flag in a flag set*/
      } /*Loop: run through all antibiotic flags*/
      
      /**************************************************\
      * Fun12 Sec02 Sub04:
      *   - print cross resistance
      \**************************************************/

      fprintf(
         (FILE *) outFILE,
         "\t"
      );

      pCrossRes_checkAmr(
         tmpST->amrSTPtr,
         refSTPtr->drugAryStr,
         (FILE *) outFILE
      );
      
      /**************************************************\
      * Fun12 Sec02 Sub05:
      *   - print variant id, mutation type, and positions
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i\t%i",
        tmpST->amrSTPtr->varIdStr,     /*variant id*/
        tmpST->amrSTPtr->mutTypeStr,   /*snp/del/ins/LoF*/
        tmpST->amrSTPtr->gradeSC,
        (int) tmpST->amrSTPtr->refPosUI + 1,
        (int) tmpST->seqPosUI + 1      /*position on seq*/
      );

      /**************************************************\
      * Fun12 Sec02 Sub06:
      *   - print high, unkown (normal?), or low resitance
      \**************************************************/

      if(tmpST->amrSTPtr->highResBl)
         fprintf(
            (FILE *) outFILE,
            "\thigh"
         );

      else if(tmpST->amrSTPtr->lowResBl)
         fprintf(
            (FILE *) outFILE,
            "\tlow"
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tNA"
         );

      /**************************************************\
      * Fun12 Sec02 Sub07:
      *   - print if low resitance is additive
      \**************************************************/

      if(tmpST->amrSTPtr->additiveResBl)
         fprintf(
            (FILE *) outFILE,
            "\tAdditive"
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tNA"
         );

      /**************************************************\
      * Fun12 Sec02 Sub08:
      *   - print if restance needs a functional gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         tmpST->amrSTPtr->needsGeneStr
      );

      /**************************************************\
      * Fun12 Sec02 Sub09:
      *   - print effect of mutation
      \**************************************************/

      if(tmpST->amrSTPtr->effectStr)
         fprintf(
            (FILE *) outFILE,
            "\t%s",
            tmpST->amrSTPtr->effectStr
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tNA"
         );

      /**************************************************\
      * Fun12 Sec02 Sub10:
      *   - print WHOs comment and if whole gene effect
      \**************************************************/
      
      if(tmpST->amrSTPtr->commentStr)
         fprintf(
            (FILE *) outFILE,
            "\t%s",
            tmpST->amrSTPtr->commentStr
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tNA"
         );

      if(tmpST->amrSTPtr->wholeGeneFlagSC)
         fprintf(
            (FILE *) outFILE,
            "\tTrue%s",
            str_endLine
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tFalse%s",
            str_endLine
         );

      /**************************************************\
      * Fun12 Sec02 Sub11:
      *   - move to next AMR
      \**************************************************/
      
      nextAmr_fun12_sec02_sub11:;
         tmpST = tmpST->nextAmr;
   } /*Loop: print all amr's*/
} /*pCon_checkAmr*/

/*-------------------------------------------------------\
| Fun13: readGetVarHits_checkAmr
|   - finds non-duplicate AMRs for the target variant ID
| Input:
|   - amrAryST:
|     o pointer to an array of amrST structures to print
|   - indexSI:
|     o index of AMR in amrAryST
|   - numAmrsUI:
|     o number AMRs in amrAryST
| Output:
|   - Returns:
|     o 0 for memory errors
|     o signed int array ending in -1 with the index of
|       each different AMR
|       * the index after -1 has the next variant ID to
|         scan
| Note:
|   - Requires AMRs to be sorted by variant (gene) id
\-------------------------------------------------------*/
signed int *
readGetVarHits_checkAmr(
   struct amrST *amrAryST,
   signed int indexSI,      /*index at in AMR array*/
   unsigned int numAmrsUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - finds non-duplicate AMRs for target variant ID
   '   o fun13 sec01:
   '     - variable declarations and initial memory alloc
   '   o fun13 sec02:
   '     - find unique AMRs for this variant id
   '   o fun13 sec03:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declarations and initial memory alloc
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int *retHeapArySI = 0;
   signed int *tmpSIPtr = 0;
   signed int retSizeSI = 16;
   signed int retLenSI = 0;
   signed int posSI = indexSI + 1;
   signed int checkSI = 0;
   signed char resSC = 0;

   retHeapArySI = malloc(retSizeSI * sizeof(signed int));
   if(! retHeapArySI)
      goto memErr_fun13_sec03;
   retHeapArySI[retLenSI++] = indexSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - find unique AMRs for this variant id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(resSC != -1)
   { /*Loop: check matching variant IDs*/
      if(posSI >= (signed int) numAmrsUI)
         break; /*at end of AMR list*/

      resSC =
         cmpAmrs_checkAmr(
            &amrAryST[ retHeapArySI[0] ],
            &amrAryST[posSI]
         );

      if(resSC == -1)
         break; /*finished*/

      else if(resSC == 1 || ! resSC)
         goto nextAmr_fun13_sec02;

      else if(resSC == 2)
      { /*Else If: second AMR is better*/
         retHeapArySI[0] = posSI;
         goto nextAmr_fun13_sec02;
      } /*Else If: second AMR is better*/

      else if(resSC == -2)
      { /*Else If: need to scan for duplicate*/
         for(checkSI = 1; checkSI < retLenSI; ++checkSI)
         { /*Loop: check if have duplicate entry*/
            resSC =
               cmpAmrs_checkAmr(
                  &amrAryST[ retHeapArySI[checkSI] ],
                  &amrAryST[posSI]
               );

            if(resSC == 1 || ! resSC)
               break; /*duplicate, can ignore*/

            else if(resSC == 2)
            { /*Else If: this is a better match*/
               retHeapArySI[checkSI] = posSI;
               break;
            } /*Else If: this is a better match*/
         } /*Loop: check if have duplicate entry*/

         if(checkSI >= retLenSI)
         { /*If: this was not a duplicate*/
            if(retLenSI >= retSizeSI - 2)
            { /*If: need to resize array*/
               retSizeSI <<= 1;
               tmpSIPtr =
                  realloc(
                     retHeapArySI,
                     retSizeSI * sizeof(signed int)
                  );
               if(! tmpSIPtr)
                  goto memErr_fun13_sec03;
               retHeapArySI = tmpSIPtr;
            } /*If: need to resize array*/

            retHeapArySI[retLenSI++] = posSI;
         } /*If: this was not a duplicate*/
      } /*Else If: need to scan for duplicate*/

      nextAmr_fun13_sec02:;
         ++posSI;
   }  /*Loop: check matching variant IDs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retHeapArySI[retLenSI++] = -1; /*mark end of array*/
   retHeapArySI[retLenSI] = posSI;/*next variant to scan*/
   goto ret_fun13_sec03;

   memErr_fun13_sec03:;
      if(retHeapArySI)
         free(retHeapArySI);
      goto ret_fun13_sec03;

   ret_fun13_sec03:;
      return retHeapArySI;
} /*readGetVarHitks_checkAmr*/

/*-------------------------------------------------------\
| Fun14: pReadHead_checkAmr
|   - prints header for reads AMR table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pReadHead_checkAmr(
   signed char refBl,
   void *outFILE
){
   if(refBl)
      fprintf((FILE *) outFILE, "ref\t");
   fprintf((FILE *) outFILE,"gene\tdrug");
   fprintf((FILE *) outFILE,"\tcross_resistance");
   fprintf((FILE *) outFILE,"\tvariant_id\ttype\tgrade");
   fprintf((FILE *) outFILE,"\tref_pos");
   fprintf((FILE *) outFILE,"\tnum_supporting_reads");
   fprintf((FILE *) outFILE,"\tperc_support_reads");
   fprintf((FILE *) outFILE, "\tnum_map_reads");
   fprintf((FILE *) outFILE, "\tresitance_level");
   fprintf((FILE *) outFILE, "\tresistance_additive");
   fprintf((FILE *) outFILE, "\tneeds_gene");
   fprintf((FILE *) outFILE,"\teffect\twho_comment");
   fprintf((FILE *) outFILE,"\twhole_gene");
   fprintf((FILE *) outFILE,"%s", str_endLine);
} /*pReadHead_checkAmr*/

/*-------------------------------------------------------\
| Fun15: pRead_checkAmr
|   - prints AMRs detected in reads that have min depth
| Input:
|   - minDepthUI:
|     o minumum depth to keep an amr
|   - minPercMapF:
|     o mininmum percent of supporting reads to keep an
|       amr (only compared locally)
|   - minPercTotalF:
|     o min percent of mapped reads needed to keep an amr
|       (compared to all reads [total depth])
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - framShiftBl:
|     o 1: looked for frameshifts in data
|   - totalReadsUI:
|     o total number of reads input
|   - refSTPtr:
|     o pointer to a refList_amrST struct with amrs to
|       check if detected
|   - outFILE:
|     o file to print the amr's to
| Output: 
|   - Modifies:
|     o amrAryST to be sorted by position
|   - Prints:
|     o AMRs with >= min stats to outFILE
\-------------------------------------------------------*/
void
pRead_checkAmr(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   float minIndelSupF,      /*% support to keep indel*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char frameShiftBl,/*looked for frameshifts*/
   unsigned int totalReadsUI,
   struct refList_amrST *refSTPtr, /*has amrs to check*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC: pRead_checkAmr
   '   o fun15 sec01:
   '     - variable declerations
   '   o fun15 sec02:
   '     - start loop and filter AMRs
   '   o fun15 sec03:
   '     -  print out the entry (passed filters)
   '   o fun15 sec04:
   '     -  move to next variant ID in list
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int indexUI = 0;

   float percSupF = 0;
   unsigned long amrFlagUL = 0;
   signed int flagOnSI = 0;
   signed char *drugStr = 0;
   signed int siAmr = 0;
   signed char firstPrintBl = 1;
   
   /*for duplicate filtering*/
   signed int *hitsHeapArySI = 0;
   signed int hitPosSI = 0;
   signed int errArySI[3]; /*if had memory errors*/

   struct amrST *amrAryST = 0;
   unsigned int numAmrsUI = 0;
   signed int siRef = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - start loop and filter AMRs
   ^   o fun15 sec02 Sub01:
   ^     - start loop and remove low depth AMRs
   ^   o fun15 sec02 Sub02:
   ^     - remove AMRs with a lower % of mapped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec02 Sub01:
   *   - start loop and remove low depth AMRs
   \*****************************************************/

   for(siRef = 0; siRef < refSTPtr->refLenSI; ++siRef)
   { /*Loop: print amrs*/
      amrAryST = refSTPtr->amrsAryST[siRef];

      numAmrsUI = refSTPtr->amrLenArySI[siRef];
      sortVarId_amrST(amrAryST, 0, numAmrsUI - 1);
      indexUI = 0;

      while(indexUI < numAmrsUI)
      { /*Loop: check and print out amrs*/
         hitsHeapArySI =
            readGetVarHits_checkAmr(
               amrAryST,
               (signed int) indexUI,
               numAmrsUI
            );

         if(! hitsHeapArySI)
         { /*If: had memory error*/
            errArySI[0] = (signed int) indexUI;
            errArySI[1] = -1;
            errArySI[2] = (signed int) indexUI + 1;

            /*bad way of handeling, but should work*/
         } /*If: had memory error*/

         for(
            hitPosSI = 0;
            hitsHeapArySI[hitPosSI] != -1;
            ++hitPosSI
         ){ /*Loop: print non-duplicate AMRs*/

            indexUI = hitsHeapArySI[hitPosSI];

            if(
              amrAryST[indexUI].numSupReadsUI < minDepthUI
            ) continue; /*to few reads support the AMR*/
            
            else if(amrAryST[indexUI].numMapReadsUI == 0)
               continue; /*to few reads mapped to AMR*/
      
            /********************************************\
            * Fun15 Sec02 Sub02:
            *   - remove AMRs with a low % of mapped reads
            \********************************************/
            
            percSupF =
                 (float) amrAryST[indexUI].numMapReadsUI
               / (float) totalReadsUI;
            
            if(percSupF < minPercTotalF)
               continue; /*not enough support*/
            
            percSupF =
                 (float) amrAryST[indexUI].numSupReadsUI
               / (float) amrAryST[indexUI].numMapReadsUI;
            
            if(percSupF < minPercMapF)
               continue; /*support for AMR under min %*/
      
            else if(
                  amrAryST[indexUI].mutTypeStr[0] == 'd'
               && percSupF < minIndelSupF
            ) continue; /*removing indel*/
            
            else if(! frameShiftBl)
               ; /*did not check frameshift events*/
      
            else if(! amrAryST[indexUI].frameshiftBl)
               ; /*not checking frameshift mutations*/
      
            else if(percSupF < minFrameshiftF)
               continue; /*beneath min support*/
      
            /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
            ^ Fun15 Sec03:
            ^   -  print out entry (passed filters)
            ^   o fun15 sec03 Sub01:
            ^     - print gene id
            ^   o fun15 sec03 Sub02:
            ^     - print antibiotics resitant to
            ^   o fun15 sec03 Sub03:
            ^     - print antibiotics cross resistance to
            ^   o fun15 sec03 Sub04:
            ^     - print variant id, mutation, reference
            ^       position, number reads supporting AMR
            ^       and total reads at reference position.
            ^   o fun15 sec03 Sub05:
            ^     - print high, normal, or low resitance
            ^   o fun15 sec03 Sub06:
            ^     - print if low resitance is additive
            ^   o fun15 sec03 Sub07:
            ^     - print if restance needs a functional
            ^       gene
            ^   o fun15 sec03 Sub08:
            ^     - print mutation effect
            ^   o fun15 sec03 Sub09:
            ^     - WHO comment and if whole gene effect
            \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
            
            /********************************************\
            * Fun15 Sec03 Sub01:
            *   - print gene id
            \********************************************/
      
            if(! refSTPtr->noRefBl)
               fprintf(
                  (FILE *) outFILE,
                  "%s\t",
                  refSTPtr->refAryStr[siRef]
               );
            fprintf(
              (FILE *) outFILE,
              "%s\t",
              amrAryST[indexUI].geneIdStr /*gene id*/
            );
            
            /********************************************\
            * Fun15 Sec03 Sub02:
            *   - print antibiotics resitant to
            \********************************************/
      
            flagOnSI = 0;
            firstPrintBl = 1;
            
            for(
               siAmr = 0;
               siAmr < def_maxDrugs_amrST;
               ++siAmr
            ){ /*Loop: run through all drug flags*/
               amrFlagUL =
                  amrAryST[indexUI].amrFlagsUL[siAmr];
               
               if(! amrFlagUL)
                  continue; /*no resitance*/
      
               while(amrFlagUL)
               { /*Loop: check each flag in a drug set*/
                  if(!(amrFlagUL & 1))
                  { /*If: There is no amr to this drug*/
                     ++flagOnSI;
                     amrFlagUL >>= 1;
                     continue; /*No resitance*/
                  } /*If: There is no amr to this drug*/
                  
                  drugStr =
                     get_drugAry(
                        refSTPtr->drugAryStr,
                        flagOnSI
                     );
                  
                  if(firstPrintBl)
                  { /*If: is frist print out*/
                     fprintf(
                        (FILE *) outFILE,
                        "%s",
                        drugStr
                     );
      
                     firstPrintBl = 0;
                  } /*If: is frist print out*/
                  
                  else
                     fprintf(
                        (FILE *) outFILE,
                        "_%s",
                        drugStr
                     );
                  
                  ++flagOnSI;
                  amrFlagUL >>= 1;
               } /*Loop: check each flag in a drug set*/
            } /*Loop: run through all drug flags*/
               
            /********************************************\
            * Fun15 Sec03 Sub03:
            *   - print antibiotics cross resistanct to
            \********************************************/
      
            fprintf(
               (FILE *) outFILE,
               "\t"
            ); /*separate resistance &
               `  cross resitance entries
               */
            
            pCrossRes_checkAmr(
               &(amrAryST)[indexUI],
               refSTPtr->drugAryStr,
               (FILE *) outFILE
            );
      
            /********************************************\
            * Fun15 Sec03 Sub04:
            *   - print variant id, mutation, reference
            *     position, number reads supporting AMR and
            *     total reads at reference position.
            \********************************************/
      
            fprintf(
              (FILE *) outFILE,
              "\t%s\t%s\t%i\t%i\t%i\t%.2f\t%i",
              amrAryST[indexUI].varIdStr,   /*variant id*/
              amrAryST[indexUI].mutTypeStr,
                 /*snp/del/ins/LoF*/
              amrAryST[indexUI].gradeSC,
              (int) amrAryST[indexUI].refPosUI + 1,
              (int) amrAryST[indexUI].numSupReadsUI,
              percSupF * 100,
              (int) amrAryST[indexUI].numMapReadsUI
            );
      
            /********************************************\
            * Fun15 Sec03 Sub05:
            *   - print high, unkown/normal, or low
            *     resitance
            \********************************************/
      
            if(amrAryST[indexUI].highResBl)
               fprintf(
                  (FILE *) outFILE,
                  "\thigh"
               );
      
            else if(amrAryST[indexUI].lowResBl)
               fprintf(
                  (FILE *) outFILE,
                  "\tlow"
               );
      
            else
               fprintf(
                  (FILE *) outFILE,
                  "\tNA"
               );
      
            /********************************************\
            * Fun15 Sec03 Sub06:
            *   - print if low resitance is additive
            \********************************************/
      
            if(amrAryST[indexUI].additiveResBl)
               fprintf(
                  (FILE *) outFILE,
                  "\tAdditive"
               );
      
            else
               fprintf(
                  (FILE *) outFILE,
                  "\tNA"
               );
      
            /********************************************\
            * Fun15 Sec03 Sub07:
            *   - print if restance needs a functional gene
            \********************************************/
      
            fprintf(
               (FILE *) outFILE, "\t%s",
               amrAryST[indexUI].needsGeneStr
            );
      
            /********************************************\
            * Fun15 Sec03 Sub08:
            *   - print mutation effect
            \********************************************/
            
            if((amrAryST)[indexUI].effectStr)
               fprintf(
                  (FILE *) outFILE,
                  "\t%s",
                  amrAryST[indexUI].effectStr
               );
            else
               fprintf(
                  (FILE *) outFILE,
                  "\tNA"
               );
      
            /********************************************\
            * Fun15 Sec03 Sub09:
            *   - print WHOs comment & if whole gene
            *     effect
            \********************************************/
            
            if(amrAryST[indexUI].commentStr)
            { /*If: have a who comment*/
               fprintf(
                  (FILE *) outFILE,
                  "\t%s",
                  amrAryST[indexUI].commentStr
               );
            } /*If: have a who comment*/
            
            else
               fprintf(
                  (FILE *) outFILE,
                  "\tNA"
               );
      
            if(amrAryST[indexUI].wholeGeneFlagSC)
               fprintf(
                  (FILE *) outFILE,
                  "\tTrue%s",
                  str_endLine
               );
      
            else
               fprintf(
                  (FILE *) outFILE,
                  "\tFalse%s",
                  str_endLine
               );
      
         }  /*Loop: print non-duplicate AMRs*/

         /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
         ^ Fun15 Sec04:
         ^   -  move to next variant ID in list
         \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

         indexUI = hitsHeapArySI[hitPosSI + 1];

         if(hitsHeapArySI != errArySI)
            free(hitsHeapArySI);
         hitsHeapArySI = 0;
      } /*Loop:Check and print out amrs*/

      sortPos_amrST(amrAryST, 0, numAmrsUI - 1);
   } /*Loop: print amrs*/
} /*pRead_checkAmr*/

/*-------------------------------------------------------\
| Fun16: pIdVarHead_checkAmr
|   - prints header for the read id mapped variant table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o header for read id mapped variant table to outFILE
\-------------------------------------------------------*/
void
pIdVarHead_checkAmr(
   signed char pRefBl,
   void *outFILE
){
   if(pRefBl)
      fprintf((FILE *) outFILE, "ref\t");
   fprintf(
      (FILE *) outFILE,
      "readId\tvariantId%s",
      str_endLine
   );
} /*pIdVarHead_checkAmr*/

/*-------------------------------------------------------\
| Fun17: pIdVarTbl_checkAmr
|   - prints table of read ids and detected AMRs
| Input:
|   - refStr:
|     o c-string with reference used
|     o use 0/null to not print the reference column
|   - idStr:
|     o c-string with read id to print out
|   - amrHitListST:
|     o pointer to an list of amrHit_checkAmr structures
|       having AMRs detected in the read
|   - outFILE:
|     o pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o read id and detected AMRs to outFILE
\-------------------------------------------------------*/
void
pIdVarTbl_checkAmr(
   signed char *refStr,
   signed char *idStr,
   struct amrHit_checkAmr *amrHitListST,
   void *outFILE
){
   signed char uniqVarBl = 0;
   struct amrHit_checkAmr *tmpHitST = amrHitListST;
   struct amrHit_checkAmr *lastHitST = tmpHitST;
   
   if(tmpHitST)
   { /*If: have AMR(s) to print out*/
      if(refStr)
         fprintf((FILE *) outFILE, "%s\t", refStr);
      fprintf(
         (FILE *) outFILE,
         "%s\t%s%s",
         idStr,
         tmpHitST->amrSTPtr->varIdStr,
         str_endLine
      );

      tmpHitST = tmpHitST->nextAmr;

      while(tmpHitST)
      { /*Loop: print each amr the read mapped to*/
         uniqVarBl =
            eql_charCp(
               lastHitST->amrSTPtr->varIdStr,
               tmpHitST->amrSTPtr->varIdStr,
               (signed char) '\0'
            );

         if(uniqVarBl)
         { /*If: unique variant (print AMR)*/
            fprintf(
               (FILE *) outFILE,
               "%s\t%s%s",
               idStr,
               tmpHitST->amrSTPtr->varIdStr,
               str_endLine
            );
         } /*If: unique variant (print AMR)*/

         lastHitST = tmpHitST;
         tmpHitST = tmpHitST->nextAmr;
      } /*Loop: print each amr the read mapped to*/
   } /*If: have AMR(s) to print out*/
} /*pIdVarTbl_checkAmr*/

/*-------------------------------------------------------\
| Fun18: samFindAmrs_checkAmr
|   - look for AMRs in sam file entries
| Input:
|   - refSTPtr:
|     o pointer to refList_amrST struct with the amrs for
|       each reference
|   - readsBl:
|     o 1: print read AMRs (pRead_checkAmry [fun15])
|     o 0: print consensus AMRs (pCon_checkAmr [fun13])
|   - framshiftBl:
|     o 1: check for framshifts (LoF/frameshift AMRs)
|     o 0: ingore frameshifts (are exact matches)
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - minDepthUI:
|     o minumum depth to keep an amr (reads only)
|   - minPercMapF:
|     o mininimum percent of supporting reads to keep an
|       amr (reads only) 
|   - minPercTotalF:
|     o mininimum percent of mapped reads needed to keep
|       an amr (all possible mapped reads; reads only)
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - samFileStr:
|     o c-string with sam file to check for AMRs
|   - outFileStr:
|     o c-string with name to print AMR hits to
|   - idFileStr:
|     o c-sring with name of file to print read ids to
| Output:
|   - Prints:
|     o stats about AMRs to outFILE
|     o read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o def_fileErr_amrST for file open errors
|     o def_memErr_amrST for memory errors
\-------------------------------------------------------*/
signed char
samFindAmrs_checkAmr(
   struct refList_amrST *refSTPtr,/*has amrs and ref ids*/
   signed char readsBl,     /*1: checking reads not cons*/
   signed char frameshiftBl,/*1: check frameshifts*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   unsigned int minDepthUI, /*min depth to keep amr*/
   float minPercMapF,       /*% support to keep amr*/
   float minPercTotalF,     /*% mapped reads to keep amr*/
   float minIndelSupF,      /*%support to keep indel amr*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char *samFileStr, /*sam file to check*/
   signed char *outFileStr, /*output file (main)*/
   signed char *idFileStr   /*output file (ids)*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:P samFindAmrs_checkAmr
   '   - Look for anti-microbial (antibiotic) genes in the
   '     reads in a sam file
   '   o fun18 sec01:
   '     - Variable declerations
   '   o fun18 sec02:
   '     - Get the first sam entry
   '   o fun18 sec03:
   '     - Check for AMRs
   '   o fun18 sec04:
   '     - Print out read AMR stats
   '   o fun18 sec05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int numHitsSI = 0;   /*number of amrs hits/con*/
   unsigned int totalReadsUI = 0; /*number of kept reads*/

   struct samEntry samStackST;
   struct amrHit_checkAmr *amrHitHeapList = 0;

   signed int refIndexSI = 0;
   signed char *tmpStr = 0;

   FILE *idFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - Get the first sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStackST);

   errSC = setup_samEntry(&samStackST);

   if(errSC)
      goto memErr_fun18_sec05;

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;
   
   else
   { /*Else: sam file provided*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
         goto fileErr_fun18_sec05;
   } /*Else: sam file provided*/

   if(
         ! outFileStr
      || *outFileStr =='-'
   ) outFILE = stdout;

   else
   { /*Else: given output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
      );

      if(! outFILE)
         goto fileErr_fun18_sec05;
   } /*Else: given output file*/

   if(idFileStr)
   { /*If: I given read id file*/
      idFILE =
         fopen(
            (char *) idFileStr,
            "w"
         );

      if(! idFILE)
         goto fileErr_fun18_sec05;
      
      pIdVarHead_checkAmr(! refSTPtr->noRefBl, idFILE);
   } /*If: I given read id file*/

   /*read in first sam file entry*/
   errSC = get_samEntry(&samStackST, samFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - check for AMRs
   ^   o fun18 sec03 sub01:
   ^     - filter out less usefull entries
   ^   o fun18 sec03 sub02:
   ^     - check for amrs
   ^   o fun18 sec03 sub03:
   ^     - print consensus sequence AMRS
   ^   o fun18 sec03 sub04:
   ^     - deal with read amrs; print ids if requested/
   ^       free consensus structuerrs
   ^   o fun18 sec03 sub05:
   ^     - move to next sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec03 Sub01:
   *   - filter out less usefull entries/start loop
   \*****************************************************/

   if(! readsBl)
      pConHead_checkAmr(! refSTPtr->noRefBl, outFILE);
   else
      pReadHead_checkAmr(! refSTPtr->noRefBl, outFILE);

   while(!errSC)
   { /*Loop: check if have antibiotic resitance*/
      if(samStackST.extraStr[0] == '@')
      { /*If: header*/
         errSC = get_samEntry(&samStackST, samFILE);
         continue;
      } /*If: header*/

      if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If: umapped read, secondary, supplemental*/
         /*4=unmapped, 256=secondary, 2048=supplemental*/
         errSC = get_samEntry(&samStackST, samFILE);
         continue;
      } /*If: umapped read, secondary, supplemental*/

      if(! refSTPtr->noRefBl)
         refIndexSI =
            refFind_refList_amrST(
               samStackST.refIdStr,
               refSTPtr
            );
      else
         refIndexSI = 0;

      if(refIndexSI < 0)
      { /*If: no matching reference for this read*/
         errSC = get_samEntry(&samStackST, samFILE);
         continue;
      } /*If: no matching reference for this read*/

      /**************************************************\
      * Fun18 Sec03 Sub02:
      *   - check for amrs
      \**************************************************/

      ++totalReadsUI;

      amrHitHeapList = 
         checkAmr(
            &samStackST,  /*sequence to check*/
            refSTPtr->amrsAryST[refIndexSI], /*has amrs*/
            refSTPtr->amrLenArySI[refIndexSI],
            &numHitsSI,   /*number amr hits for seq*/
            frameshiftBl, /*1: check frameshifts*/
            aaIndelBl,    /*1: allow indel in snp aa AMR*/
            &errSC        /*for error reporting*/
         );

      if(errSC)
         goto memErr_fun18_sec05;

      /**************************************************\
      * Fun18 Sec03 Sub03:
      *   - print consensus sequence AMRS
      \**************************************************/

      if(! readsBl)
      { /*If: am printing out consensus AMRs*/
          if(! refSTPtr->noRefBl)
             tmpStr = refSTPtr->refAryStr[refIndexSI];
          else
             tmpStr = 0;

          pCon_checkAmr(
             tmpStr,
             samStackST.qryIdStr,
             amrHitHeapList,
             refSTPtr,
             outFILE
          ); /*print amr entry*/
      } /*If: am printing out consensus AMRs*/

      /**************************************************\
      * Fun18 Sec03 Sub04:
      *   - print ids/amr table (for detected AMRs)
      \**************************************************/

      else if(idFileStr)
      { /*Else If: printing out read ids*/
          if(! refSTPtr->noRefBl)
             tmpStr = refSTPtr->refAryStr[refIndexSI];
          else
             tmpStr = 0;
          pIdVarTbl_checkAmr(
             tmpStr,
             samStackST.qryIdStr,
             amrHitHeapList,
             idFILE
          );
      } /*Else If: printing out read ids*/

      /**************************************************\
      * Fun18 Sec03 Sub05:
      *   - move to next sam entry
      \**************************************************/

      freeHeapList_amrHit_checkAmr(amrHitHeapList);
      amrHitHeapList = 0;

      errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: check if have antibiotic resitance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - print read AMR stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(errSC > 1)
      goto memErr_fun18_sec05;

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   freeStack_samEntry(&samStackST);

   if(readsBl)
   { /*If: printing reads AMRs*/
      pRead_checkAmr(
         minDepthUI,
         minPercMapF,
         minPercTotalF,
         minIndelSupF,
         minFrameshiftF,
         frameshiftBl,
         totalReadsUI,
         refSTPtr,
         outFILE
      );
   } /*If: printing reads AMRs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_fun14_sec05;

   memErr_fun18_sec05:;
      errSC = def_memErr_amrST;
      goto cleanUp_fun14_sec05;

   fileErr_fun18_sec05:;
      errSC = def_fileErr_amrST;
      goto cleanUp_fun14_sec05;

   cleanUp_fun14_sec05:;
      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else if(samFILE == stderr) ;
      else
         fclose(samFILE);
      samFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);

      if(! idFILE) ;
      else if(idFILE == stdin) ;
      else if(idFILE == stdout) ;
      else if(idFILE == stderr) ;
      else
         fclose(idFILE);
      idFILE = 0;


      freeStack_samEntry(&samStackST);

      if(amrHitHeapList)
         freeHeapList_amrHit_checkAmr(amrHitHeapList);

      amrHitHeapList = 0;

      return errSC;
} /*samFindAmrs_checkAmr*/

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
