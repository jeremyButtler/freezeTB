/*#######################################################\
# Name: checkAmr
#   - has functions to detect AMRs in sam file entrie
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
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
'   o fun09: pConHead_checkAmr
'     - prints header for a consensus amr check
'   o fun10: pCon_checkAmr
'     - prints out all amr's that were in a consensus
'   o fun11: pReadHead_checkAmr
'     - prints header for reads AMR table
'   o fun12: pRead_checkAmr
'     - prints AMRs detected in reads that have min depth
'   o fun13: pIdVarHead_checkAmr
'     - prints header for the read id mapped variant table
'   o fun14: pIdVarTbl_checkAmr
'     - prints table of read ids and detected AMRs
'   o fun15: samFindAmrs_checkAmr
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

#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/genMath.h"
#include "../genBio/codonTbl.h"
#include "../genBio/samEntry.h"

/*Only .h files*/
#include "../genLib/dataTypeShortHand.h"
#include "../genBio/ntTo2Bit.h"
#include "../genBio/revNtTo2Bit.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c  #include "../genLib/base10StrToNum.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/strAry.h"
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

   sint extraNtSI = 0; /*extra bases from last entry*/
   schar aaC = 0;
   schar LoFBl = 0;
   schar haveStartBl = 0;

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
          aaC =
            codonToAA_codonTbl(
               samSTPtr->seqStr[readPosSI],
               samSTPtr->seqStr[readPosSI + 1],
               samSTPtr->seqStr[readPosSI + 2]
            );

          aaC = aaC != 'x';

          LoFBl =
             ! (
               bactStart_codonTbl(
                 (samSTPtr)->seqStr[readPosSI],
                 (samSTPtr)->seqStr[readPosSI + 1],
                 (samSTPtr)->seqStr[readPosSI + 2]
                )
             ); /*check if I have start codon*/

          LoFBl &= aaC; /*account for masking*/
       } /*Else: not an deletion*/

       haveStartBl = 1;
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

            if(haveStartBl)
            { /*If: had an start, check for LoFs*/
               while(extraNtSI > 2)
               { /*Loop: find early stops*/
                  aaC =
                    codonToAA_codonTbl(
                       samSTPtr->seqStr[readPosSI],
                       samSTPtr->seqStr[readPosSI + 1],
                       samSTPtr->seqStr[readPosSI + 2]
                    );

                  readPosSI += 3;

                  /*check if I had an early stop*/
                  LoFBl |= (aaC == '*');
                  extraNtSI -= 3;
               } /*Loop: find early stops*/
            } /*If: had an start, check for LoFs*/

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

            if(haveStartBl)
            { /*If: start, check for LoFs*/
               while(extraNtSI > 2)
               { /*Loop: check reading frame*/
                  aaC =
                    codonToAA_codonTbl(
                      samSTPtr->seqStr[readPosSI],
                      samSTPtr->seqStr[readPosSI + 1],
                      samSTPtr->seqStr[readPosSI + 2]
                    );

                  readPosSI += 3;
                  extraNtSI -= 3;

                  if(extraNtSI < 3)
                     break;  /*last codon in match/snp*/

                  /*check if early stop*/
                  LoFBl |= (aaC == '*');
               } /*Loop: check reading frame*/
            } /*If: start, check for LoFs*/

            /*check if last base*/
            if(refPosSI >= endGeneSI)
               LoFBl |= ((aaC != '*') & (aaC != 'x'));
            else
               LoFBl |= (aaC == '*');

            break;
         /*Case: matchs and snps*/
      } /*Switch: check cigar entry type*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - move to next cigar entry
      \**************************************************/

      ++cigPosSI;

      if(cigPosSI >= (sint) samSTPtr->lenCigUI)
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

   int extraNtSI = 0; /*extra bases from last entry*/
   schar aaC = 0;
   schar LoFBl = 0;
   schar haveEndBl = 0; /*marks have end of gene*/

   sint readStartSI = -1;

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
         aaC =
            revCodonToAA_codonTbl(
               samSTPtr->seqStr[readPosSI],
               samSTPtr->seqStr[readPosSI + 1],
               samSTPtr->seqStr[readPosSI + 2]
            ); /*get last codon*/

         LoFBl = ((aaC != '*') & (aaC != 'x'));
         readStartSI = readPosSI;
      } /*Else: see if is not deletion*/

      haveEndBl = 1;
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

            if(haveEndBl)
            { /*If: had an end, check for LoFs*/
               while(extraNtSI > 2)
               { /*Loop: check for early stops*/
                  aaC =
                    revCodonToAA_codonTbl(
                      samSTPtr->seqStr[readPosSI],
                      samSTPtr->seqStr[readPosSI + 1],
                      samSTPtr->seqStr[readPosSI + 2]
                    );

                  readPosSI += 3;

                  LoFBl |= (aaC == '*'); /*have stop?*/
                  extraNtSI -= 3;
               } /*Loop: check for early stops*/
            } /*If: had an end, check for LoFs*/

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

            if(haveEndBl)
            { /*If: had an end, check for LoFs*/
               while(extraNtSI > 2)
               { /*Loop: check indel reading frame*/
                  extraNtSI -= 3;

                  if(extraNtSI < 3)
                     break;  /*last codon for entry*/

                  aaC =
                    revCodonToAA_codonTbl(
                      samSTPtr->seqStr[readPosSI],
                      samSTPtr->seqStr[readPosSI + 1],
                      samSTPtr->seqStr[readPosSI + 2]
                    );

                  /*check if had early stop*/
                  if(readPosSI != readStartSI)
                     LoFBl |= (aaC == '*');

                  readPosSI += 3;
               } /*Loop: Check indel reading frame*/
            } /*If: had an end, check for LoFs*/

            /*check if is last base*/
            if(refPosSI >= endGeneSI)
            { /*If: at end; check if have start codon*/
               aaC =
                  revCodonToAA_codonTbl(
                     samSTPtr->seqStr[readPosSI],
                     samSTPtr->seqStr[readPosSI + 1],
                     samSTPtr->seqStr[readPosSI + 2]
                  );

               aaC = (aaC != 'x');
               aaC &=
                 !(
                   bactRevStart_codonTbl(
                      samSTPtr->seqStr[readPosSI],
                      samSTPtr->seqStr[readPosSI + 1],
                      samSTPtr->seqStr[readPosSI + 2]
                   ) /*check if have start codon*/
                 );

               readPosSI += 3;

               LoFBl |= aaC;
            } /*If: at end; check if have start codon*/

            else
            { /*Else: check if have early stop*/
               aaC =
                  revCodonToAA_codonTbl(
                     samSTPtr->seqStr[readPosSI],
                     samSTPtr->seqStr[readPosSI + 1],
                     samSTPtr->seqStr[readPosSI + 2]
                  );

               LoFBl |= (aaC == '*');
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

      if(cigPosSI >= (sint) samSTPtr->lenCigUI)
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
   uint seqPosUI = 0;
   uint refPosUI = 0;

   /*For processing the cigar entries*/
   sint cigNtSI = 0;   /*number bases left in cig entry*/
   sint siCig = 0;     /*Iterate through the cigar*/

   /*for recording the number of snps/ins/dels when
   `   comparing AMRs
   */
   sint snpSI = 0;
   sint insSI = 0;
   sint delSI = 0;

   /*for keeping track of the cigar positions when
   `   comparing AMRs
   */
   sint cigPosSI = 0;
   sint cigCountSI = 0;

   /*For checking amr's*/
   uchar *seqUStr = 0;
   uchar *amrUStr = 0;

   sint siAmr = 0;     /*iterate through the amr's list*/
   sint siBase = 0;    /*iterate/compare amr pattern*/
   sint siAa = 0;      /*i base for amino acid amrs*/
   sint siMatch = 0;   /*holds the last checked base*/
   sint amrEndSI = 0;  /*length of AMR*/

   schar resBl = 0;    /*-1 is resitance; 0 is not*/

   /*for gene wide frameshift processing*/
   sint numDelSI = 0; /*number deletions in frameshift*/
   sint numInsSI = 0; /*number insertions in frameshift*/
   schar lofBl = 0;   /*1: had mising/early start/stop*/

   /*for aa amr check*/
   uchar base1UC = 0;    /*base 1 in amr codon*/
   uchar base2UC = 0;    /*base 2 in amr codon*/
   uchar base3UC = 0;    /*base 3 in amr codon*/
   char aaC = 0;         /*holds sequence AA at position*/

   /*return values/used to build amr list*/
   struct amrHit_checkAmr *amrSTList = 0;/*amr list*/
   struct amrHit_checkAmr *amrST = 0;    /*to build list*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - Find the nearest amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSC = 0;

   siAmr =
      getAmr_amrST(
         amrAryST,
         (int) samSTPtr->refStartUI,
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
      if(
           samSTPtr->refEndUI
         < amrAryST[siAmr].refPosUI + 1
      ) goto finished_fun07_sec04_sub03;
        /*no more AMRs*/

      /*make sure read covers full AMR*/
      amrEndSI =
           amrAryST[siAmr].refPosUI
         + amrAryST[siAmr].lenRefSeqUI;

      if(samSTPtr->refEndUI < (uint) amrEndSI)
         goto nextAmr_fun07_sec03_sub08;

      if(samSTPtr->readLenUI < amrAryST[siAmr].lenAmrAaUI)
         goto nextAmr_fun07_sec03_sub08;

      if(frameshiftBl & siAmr)
      { /*If: frameshift checking is set up*/
         if(
            eql_charCp(
               amrAryST[siAmr].geneIdStr,
               amrAryST[siAmr - 1].geneIdStr,
               (schar) '\0'
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
         (sint *) &refPosUI,
         (sint *) &seqPosUI
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
      *     - check if non-amino acid AMR is match
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat01:
      +   - get first amr base and set up
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++amrAryST[siAmr].numMapReadsUI;
      seqUStr = (uchar *) &samSTPtr->seqStr[seqPosUI];

      /*amrUStr is needed for the final checks for both
      `   nucleotide and amino acid checks
      */
      amrUStr = (uchar *) amrAryST[siAmr].amrSeqStr;

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
         && amrAryST[siAmr].frameshiftBl)
      { /*If: is frameshift pattern*/
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
         } /*If: deletion or insertion*/

         while(
               samSTPtr->cigTypeStr[cigPosSI] == 'D' 
            || samSTPtr->cigTypeStr[cigPosSI] == 'I' 
         ){ /*Loop: count number of indels*/
            --cigCountSI;

            if(cigCountSI <= 0)
            { /*If: next cigar entry*/
               ++cigPosSI;
               cigCountSI = samSTPtr->cigArySI[cigPosSI];
            } /*If: next cigar entry*/

            if(samSTPtr->cigTypeStr[cigPosSI] == 'D')
               ++delSI;
            else
               ++insSI;
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
      if(amrAryST[siAmr].amrAaStr[0] != '0')
         goto checkAA_fun07_sec03_sub05_cat01;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat04:
      +   - check if non-amino acid AMR is match
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
      
      checkSnp_fun07_sec03_sub04_cat04:;

      for(
         siBase = 0;
         (amrUStr[siBase] &~32) == (seqUStr[siBase] &~32);
         ++siBase
      ){ /*Loop : see if have extact match to the amr*/
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
            cigCountSI = samSTPtr->cigArySI[cigPosSI];
         } /*If: next cigar entry*/

         if(amrUStr[siBase] == '\0')
            break;

         if(seqUStr[siBase] == '\0')
            break;
      } /*Loop : See if have extact match to the amr*/

      siMatch = siBase; /*for comparison step*/

      if(amrUStr[siBase] != '\0')
         goto nextAmr_fun07_sec03_sub08; /*not AMR*/

      /*may be an AMR; make sure not false positive*/
      goto falsePositiveCheck_fun07_sec04_sub06_cat01;

      /**************************************************\
      * Fun07 Sec03 Sub05:
      *   - handle amino acid amr's
      *   o fun07 sec03 sub05 cat01:
      *     - check if deletions are resistant
      *   o fun07 sec03 sub05 cat02:
      *     - move to the first base in target codon
      *   o fun07 sec03 sub05 cat03:
      *     - check reverse complemnt gene snps/insertions
      *   o fun07 sec03 sub05 cat04:
      *     - else I do not know direction, look at snps
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat01:
      +   - check if deletions are resistant
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      checkAA_fun07_sec03_sub05_cat01:;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat02:
      +   - move to first base in target codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*should work out, even when reverse complement*/
      seqUStr +=
         (sint)
         amrAryST[siAmr].codonPosUI - (int) refPosUI;

      siBase = 0; /*for finding the number of
                 ` matches/snps, inss, and dels in
                 ` sequence after AA checks (sub05)
                 */

      if((schar *) seqUStr < samSTPtr->seqStr)
         goto nextAmr_fun07_sec03_sub08;
         /*some cases go one base off, so incomplete*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat03:
      +   - check reverse complemnt gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
         amrAryST[siAmr].dirFlagSC == def_revCompDir_amrST
      ){ /*If: reverse complement gene*/
         for(
            siAa = 0;
				siAa < (sint) amrAryST[siAmr].lenAmrAaUI;
            ++siAa 
         ){ /*Loop: check codon reading frame*/
            if((schar *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base1UC =
               (uchar)
               revNtTo2Bit[*seqUStr--];

            if((schar *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base2UC =
               (uchar)
               revNtTo2Bit[*seqUStr--];

            if((schar *) seqUStr < samSTPtr->seqStr)
               goto nextAmr_fun07_sec03_sub08;

            base3UC =
               (uchar)
               revNtTo2Bit[*seqUStr--];

            aaC = codonTbl[base1UC][base2UC][base3UC];
            /*2024 WHO catalog has ? marks for
            ` non-insertions/deletions; so safe for now
            */
          
            if(aaC == 'x') /*any aa*/
               goto nextAmr_fun07_sec03_sub08;

            else if(
                  amrAryST[siAmr].amrAaStr[siAa] == '?'
               && aaC != amrAryST[siAmr].refAaStr[siAa]
            ){ /*Else If: amr pattern has any codon*/

               if(
                     (amrAryST[siAmr].refAaStr[siAa] | 32)
                  == 'm'
               ){ /*If: ref (non-amr) pattern has start*/
                  resBl =
                     bactStartCode_codonTbl(
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

            else if(aaC != amrAryST[siAmr].amrAaStr[siAa])
               goto nextAmr_fun07_sec03_sub08;
         } /*Loop: check codon reading frame*/

         siMatch = amrAryST[siAmr].lenAmrSeqUI;
      } /*If: reverse complement gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat04:
      +   - check forward gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         amrAryST[siAmr].dirFlagSC == def_forwardDir_amrST
      ){ /*Else If: foward gene*/
         for(
            siAa = 0;
            siAa < (sint) amrAryST[siAmr].lenAmrAaUI;
            ++siAa 
         ){ /*Loop: check codon reading frame*/
            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base1UC = (uchar) ntTo2Bit[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base2UC = (uchar) ntTo2Bit[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub08;

            base3UC = (uchar) ntTo2Bit[*seqUStr++];

            aaC = codonTbl[base1UC][base2UC][base3UC];

            /*2023 WHO catalog has ? marks for
            ` non-insertions/deletions; so am safe for now
            */
            if(aaC == 'x') /*any aa*/
               goto nextAmr_fun07_sec03_sub08;

            else if(
                  amrAryST[siAmr].amrAaStr[siAa] == '?'
               && aaC != amrAryST[siAmr].refAaStr[siAa]
            ){ /*Else If: pattern supports any codon*/
               if(
                      (amrAryST[siAmr].refAaStr[siAa] |32)
                   == 'm'
               ){ /*If: ref (non-amr) pattern has start*/
                  resBl =
                     bactStartCode_codonTbl(
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

            else if(aaC != amrAryST[siAmr].amrAaStr[siAa])
               goto nextAmr_fun07_sec03_sub08;
         } /*Loop: check codon reading frame*/

         siMatch = amrAryST[siAmr].lenAmrSeqUI;
      } /*Else If: foward gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat05:
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

      falsePositiveCheck_fun07_sec04_sub06_cat01:;

      while(siBase < (sint) amrAryST[siAmr].lenAmrSeqUI)
      { /*Loop: to end of amr sequence*/
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
            cigCountSI = samSTPtr->cigArySI[cigPosSI];
         } /*If: next cigar entry*/

         ++siBase;
      } /*Loop: to end of the amr sequence*/

      while(siBase < (sint) amrAryST[siAmr].lenRefSeqUI)
      { /*Loop: to end of reference sequence*/
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
              == (sint) amrAryST[siAmr].lenAmrSeqUI
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
            amrST->nextAmr = mk_amrHit_checkAmr();

            if(!amrST->nextAmr)
                goto memErr_fun07_sec04_sub02;

            amrST->nextAmr->amrSTPtr = &amrAryST[siAmr];
            amrST->nextAmr->seqPosUI = seqPosUI;
            amrST = amrST->nextAmr;
         } /*If: not first amr*/

         else
         { /*Else: is first amr*/
            amrSTList = mk_amrHit_checkAmr();
            amrST = amrSTList;

            if(!amrST)
                goto memErr_fun07_sec04_sub02;

            amrST->amrSTPtr = &amrAryST[siAmr];
            amrST->seqPosUI = seqPosUI;
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
   sint siAmr = 0; /*index of amr on*/
   sint siIndex = 0;
   schar firstPrintBl = 1;
   schar *drugStr = 0;
   ulong flagsUL = 0;

   for(
      siAmr=0;
      siAmr < (sint) def_maxDrugs_amrST;
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

         drugStr =
            get_drugAry(
               drugAryStr,
               siIndex
            );

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
| Fun09: pConHead_checkAmr
|   - prints header for a consensus amr check
| Input:
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrHit_checkAmrList table header to outFILE
\-------------------------------------------------------*/
void
pConHead_checkAmr(
   void *outFILE
){
   fprintf((FILE *) outFILE, "Id\tgene\tdrug");
   fprintf((FILE *) outFILE, "\tcross_resistance");
   fprintf((FILE *) outFILE, "\tvariant_id\ttype\tgrade");
   fprintf((FILE *) outFILE, "\tref_pos\tseq_pos");
   fprintf((FILE *) outFILE, "\tresitance_level");
   fprintf((FILE *) outFILE, "\tresistance_additive");
   fprintf((FILE *) outFILE, "\tneeds_gene");
   fprintf((FILE *) outFILE, "\teffect\twho_comment");
   fprintf((FILE *) outFILE, "\twhole_gene\n");
} /*pConHead_checkAmr*/

/*-------------------------------------------------------\
| Fun10: pCon_checkAmr
|   - prints out all amr's that were in a consensus
| Input:
|   - seqIdStr:
|     o c-string with name of sequence
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs with
|       detected AMRs
|   - drugAryStr:
|     o c-string array (drugAry.c) with antibiotic
|     o needs to be same order as flags in
|       amrHitListST->nextAmr (AMR array)
|   - outFILE:
|     o file to print amr's to
| Output: 
|   - Prints:
|     o amr's in amrHitListST to outFILE
\-------------------------------------------------------*/
void
pCon_checkAmr(
   signed char *seqIdStr,
   struct amrHit_checkAmr *amrHitListST,
   signed char *drugAryStr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - prints out all amr's that were in a consensus
   '   o fun10 sec01:
   '     - variable decerations
   '   o fun10 sec02:
   '     - print AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable decerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct amrHit_checkAmr *tmpST = (amrHitListST);
   struct amrHit_checkAmr *lastST = 0;
   schar matchBl = 0;

   ulong amrFlagUL = 0;
   sint flagOnSI = 0;
   schar *drugStr = 0;
   schar firstPrintBl = 0;
   sint siAmr = 0;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - print AMRs
   ^   o fun10 sec02 sub01:
   ^     - print AMRs/check if already printed variant
   ^   o fun10 sec02 sub02:
   ^     - print read/gene ids
   ^   o fun10 sec02 sub03:
   ^     - print antibiotics resitant to
   ^   o fun10 sec02 sub04:
   ^     - print cross resistance
   ^   o fun10 sec02 sub05:
   ^     - print variant id, mutation type, and positions
   ^   o fun10 sec02 sub06:
   ^     - print high, unkown (normal?), or low resitance
   ^   o fun10 sec02 sub07:
   ^     - print if low resitance is additive
   ^   o fun10 sec02 sub08:
   ^     - print if restance needs a functional gene
   ^   o fun10 sec02 sub09:
   ^     - print effect of mutation
   ^   o fun10 sec02 sub10:
   ^     - print WHOs comment and if whole gene effect
   ^   o fun10 sec02 sub11:
   ^     - move onto next AMR
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec02 Sub01:
   *   - print AMRs/check if already printed variant
   \*****************************************************/

   while(tmpST)
   { /*Loop: print all amr's*/

      if(lastST)
      { /*If: need to check previous results*/
         matchBl =
            eql_charCp(
               tmpST->amrSTPtr->varIdStr,
               lastST->amrSTPtr->varIdStr,
               (schar) '\0'
            );

         lastST = tmpST;
         tmpST = tmpST->nextAmr;

         if(! tmpST)
            break; /*no more AMRs (finished)*/

         if(! matchBl)
            continue;
      } /*If: need to check previous results*/

      else
         lastST = tmpST;

      /**************************************************\
      * Fun10 Sec02 Sub02:
      *   - print read/gene ids
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "%s\t%s\t",
        (seqIdStr),             /*name of the seq*/
        tmpST->amrSTPtr->geneIdStr /*gene name*/
      );
      
      /**************************************************\
      * Fun10 Sec02 Sub03:
      *   - print antibiotics resitant to
      \**************************************************/

      flagOnSI = 0;
      firstPrintBl = 1;
      
      for(
         siAmr = 0;
         siAmr < (sint) def_maxDrugs_amrST;
         ++siAmr
      ){ /*Loop: run through all antibiotic flags*/
         amrFlagUL = tmpST->amrSTPtr->amrFlagsUL[siAmr];
         
         if(! amrFlagUL)
            continue; /*no resitance*/

         while(amrFlagUL)
         { /*Loop: check each flag in a flag set*/
            if(! (amrFlagUL & 1) )
               goto nextAmrFlag_fun10_sec02_sub03;
               /*not resistance to this antibiotic*/
            
            drugStr =
               get_drugAry(
                  drugAryStr,
                  flagOnSI
               );
            
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
            
             nextAmrFlag_fun10_sec02_sub03:;
            ++flagOnSI;
            amrFlagUL >>= 1;
         } /*Loop: check each flag in a flag set*/
      } /*Loop: run through all antibiotic flags*/
      
      /**************************************************\
      * Fun10 Sec02 Sub04:
      *   - print cross resistance
      \**************************************************/

      fprintf(
         (FILE *) outFILE,
         "\t"
      );

      pCrossRes_checkAmr(
         tmpST->amrSTPtr,(drugAryStr),
         (FILE *) outFILE
      );
      
      /**************************************************\
      * Fun10 Sec02 Sub05:
      *   - print variant id, mutation type, and positions
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i\t%i",
        tmpST->amrSTPtr->varIdStr,         /*variant id*/
        tmpST->amrSTPtr->mutTypeStr,       /*snp/del/ins/LoF*/
        tmpST->amrSTPtr->gradeSC,
        (int) tmpST->amrSTPtr->refPosUI,  /*position on ref*/
        (int) tmpST->seqPosUI          /*position on seq*/
      );

      /**************************************************\
      * Fun10 Sec02 Sub06:
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
      * Fun10 Sec02 Sub07:
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
      * Fun10 Sec02 Sub08:
      *   - print if restance needs a functional gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         tmpST->amrSTPtr->needsGeneStr
      );

      /**************************************************\
      * Fun10 Sec02 Sub09:
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
      * Fun10 Sec02 Sub10:
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
            "\tTrue\n"
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tFalse\n"
         );

      /**************************************************\
      * Fun10 Sec02 Sub11:
      *   - move to next AMR
      \**************************************************/
      
      tmpST = tmpST->nextAmr;
   } /*Loop: print all amr's*/
} /*pCon_checkAmr*/

/*-------------------------------------------------------\
| Fun11: pReadHead_checkAmr
|   - prints header for reads AMR table
| Input:
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pReadHead_checkAmr(
   void *outFILE
){
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
   fprintf((FILE *) outFILE,"\twhole_gene\n");
} /*pReadHead_checkAmr*/

/*-------------------------------------------------------\
| Fun12: pRead_checkAmr
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
|   - totalReadsUI:
|     o total number of reads input
|   - amrAryST:
|     o pointer to an array of amrST structures to print
|   - drugAryStr:
|     o c-string array (drugAry.c) of antibiotic drugs.
|     o needs to have same order as flags in amrAryST
|   - outFILE:
|     o file to print the amr's to
| Output: 
|   - Prints:
|     o AMRs with >= min stats to outFILE
\-------------------------------------------------------*/
void
pRead_checkAmr(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   unsigned int totalReadsUI,
   struct amrST *amrAryST,
   unsigned int numAmrsUI,
   signed char *drugAryStr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC: pRead_checkAmr
   '   o fun12 sec01:
   '     - variable declerations
   '   o fun12 sec02:
   '     - start loop and filter AMRs
   '   o fun12 sec03:
   '     -  print out the entry (passed filters)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint indexUI = 0;
   uint lastPrintIndexUI = 0;
   float percSupF = 0;
   ulong amrFlagUL = 0;
   sint flagOnSI = 0;
   schar *drugStr = 0;
   sint siAmr = 0;
   schar firstPrintBl = 1;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - start loop and filter AMRs
   ^   o fun12 sec02 Sub01:
   ^     - start loop and remove low depth AMRs
   ^   o fun12 sec02 Sub02:
   ^     - remove AMRs with a lower % of mapped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec02 Sub01:
   *   - start loop and remove low depth AMRs
   \*****************************************************/

   for(indexUI = 0; indexUI < (numAmrsUI); ++indexUI)
   { /*Loop: check and print out amrs*/

      if(amrAryST[indexUI].numSupReadsUI < minDepthUI)
         continue; /*to few reads support the AMR*/
      
      if(amrAryST[indexUI].numMapReadsUI == 0)
         continue; /*to few reads mapped to AMR region*/

      /**************************************************\
      * Fun12 Sec02 Sub02:
      *   - remove AMRs with a lower % of mapped reads
      \**************************************************/
      
      percSupF =
              (float) amrAryST[indexUI].numMapReadsUI
            / (float) totalReadsUI;
      
      if(percSupF < minPercTotalF)
         continue; /*not enough support*/
      
      percSupF =
           (float) amrAryST[indexUI].numSupReadsUI
         / (float) amrAryST[indexUI].numMapReadsUI;
      
      if(percSupF < minPercMapF)
         continue; /*support for AMR under minimum %*/

       if(indexUI > 0)
       { /*If: need to check indexs*/
          if( !
              eql_charCp(
                 amrAryST[indexUI].varIdStr,
                 amrAryST[lastPrintIndexUI].varIdStr,
                 (schar) '\0'
              )
          ) continue; /*id already printed*/
       } /*If: need to check indexs*/
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun12 Sec03:
      ^   -  print out entry (passed filters)
      ^   o fun12 sec03 Sub01:
      ^     - print gene id
      ^   o fun12 sec03 Sub02:
      ^     - print antibiotics resitant to
      ^   o fun12 sec03 Sub03:
      ^     - print antibiotics cross resistance to
      ^   o fun12 sec03 Sub04:
      ^     - print variant id, mutation, reference
      ^       position, number reads supporting AMR and
      ^       total reads at reference position.
      ^   o fun12 sec03 Sub05:
      ^     - print high, unkown/normal, or low resitance
      ^   o fun12 sec03 Sub06:
      ^     - print if low resitance is additive
      ^   o fun12 sec03 Sub07:
      ^     - Print if restance needs a functional gene
      ^   o fun12 sec03 Sub08:
      ^     - print mutation effect
      ^   o fun12 sec03 Sub09:
      ^     - print WHOs comment and if whole gene effect
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      
      /**************************************************\
      * Fun12 Sec03 Sub01:
      *   - print gene id
      \**************************************************/

      lastPrintIndexUI = indexUI;

      fprintf(
        (FILE *) outFILE,
        "%s\t",
        (amrAryST)[indexUI].geneIdStr /*gene id*/
      );
      
      /**************************************************\
      * Fun12 Sec03 Sub02:
      *   - print antibiotics resitant to
      \**************************************************/

      flagOnSI = 0;
      firstPrintBl = 1;
      
      for(
         siAmr = 0;
         siAmr < def_maxDrugs_amrST;
         ++siAmr
      ){ /*Loop: run through all drug flags*/
         amrFlagUL = amrAryST[indexUI].amrFlagsUL[siAmr];
         
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
                  drugAryStr,
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
         
      /**************************************************\
      * Fun12 Sec03 Sub03:
      *   - print antibiotics cross resistanct to
      \**************************************************/

      fprintf(
         (FILE *) outFILE,
         "\t"
      ); /*separate resistance & cross resitance entries*/
      
      pCrossRes_checkAmr(
         &(amrAryST)[indexUI],
        drugAryStr,
        (FILE *) outFILE
      );

      /**************************************************\
      * Fun12 Sec03 Sub04:
      *   - print variant id, mutation, reference
      *     position, number reads supporting AMR and
      *     total reads at reference position.
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i\t%i\t%.2f\t%i",
        amrAryST[indexUI].varIdStr,   /*variant id*/
        amrAryST[indexUI].mutTypeStr, /*snp/del/ins/LoF*/
        amrAryST[indexUI].gradeSC,
        (int) amrAryST[indexUI].refPosUI,
        (int) amrAryST[indexUI].numSupReadsUI,
        percSupF * 100,
        (int) amrAryST[indexUI].numMapReadsUI
      );

      /**************************************************\
      * Fun12 Sec03 Sub05:
      *   - print high, unkown/normal, or low resitance
      \**************************************************/

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

      /**************************************************\
      * Fun12 Sec03 Sub06:
      *   - print if low resitance is additive
      \**************************************************/

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

      /**************************************************\
      * Fun12 Sec03 Sub07:
      *   - print if restance needs a functional gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         amrAryST[indexUI].needsGeneStr
      );

      /**************************************************\
      * Fun12 Sec03 Sub08:
      *   - print mutation effect
      \**************************************************/
      
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

      /**************************************************\
      * Fun12 Sec03 Sub09:
      *   - print WHOs comment and if whole gene effect
      \**************************************************/
      
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
            "\tTrue\n"
         );

      else
         fprintf(
            (FILE *) outFILE,
            "\tFalse\n"
         );

   } /*Loop:Check and print out amrs*/
} /*pRead_checkAmr*/

/*-------------------------------------------------------\
| Fun13: pIdVarHead_checkAmr
|   - prints header for the read id mapped variant table
| Input:
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o header for read id mapped variant table to outFILE
\-------------------------------------------------------*/
void
pIdVarHead_checkAmr(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "readId\tvariantId\n"
   );
} /*pIdVarHead_checkAmr*/

/*-------------------------------------------------------\
| Fun14: pIdVarTbl_checkAmr
|   - prints table of read ids and detected AMRs
| Input:
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
   signed char *idStr,
   struct amrHit_checkAmr *amrHitListST,
   void *outFILE
){
   schar uniqVarBl = 0;
   struct amrHit_checkAmr *tmpHitST = amrHitListST;
   struct amrHit_checkAmr *lastHitST = tmpHitST;
   
   if(tmpHitST)
   { /*If: have AMR(s) to print out*/
      fprintf(
         (FILE *) outFILE,
         "%s\t%s\n",
         idStr,
         tmpHitST->amrSTPtr->varIdStr
      );

      tmpHitST = tmpHitST->nextAmr;

      while(tmpHitST)
      { /*Loop: print each amr the read mapped to*/
         uniqVarBl =
            eql_charCp(
               lastHitST->amrSTPtr->varIdStr,
               tmpHitST->amrSTPtr->varIdStr,
               (schar) '\0'
            );

         if(uniqVarBl)
         { /*If: unique variant (print AMR)*/
            fprintf(
               (FILE *) outFILE,
               "%s\t%s\n",
               idStr,
               tmpHitST->amrSTPtr->varIdStr
            );
         } /*If: unique variant (print AMR)*/

         lastHitST = tmpHitST;
         tmpHitST = tmpHitST->nextAmr;
      } /*Loop: print each amr the read mapped to*/
   } /*If: have AMR(s) to print out*/
} /*pIdVarTbl_checkAmr*/

/*-------------------------------------------------------\
| Fun15: samFindAmrs_checkAmr
|   - look for AMRs in sam file entries
| Input:
|   - amrAryST
|     o pointer to an array of amrST structs with AMRs
|   - numAmrSI:
|     o number of AMRs in amrAryST
|   - drugAryStr:
|     o c-string array (drugAry.c) with antibiotic names
|   - readsBl:
|     o 1: print read AMRs (pRead_checkAmry [fun12])
|     o 0: print consensus AMRs (pCon_checkAmr [fun10])
|   - framshiftBl:
|     o 1: check for framshifts (LoF/frameshift AMRs)
|     o 0: ingore frameshifts (are exact matches)
|   - minDepthUI:
|     o minumum depth to keep an amr (reads only)
|   - minPercMapF:
|     o mininimum percent of supporting reads to keep an
|       amr (reads only) 
|   - minPercTotalF:
|     o mininimum percent of mapped reads needed to keep
|       an amr (all possible mapped reads; reads only)
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
   struct amrST *amrAryST,          /*AMRs to check*/
   signed int numAmrSI,     /*number of AMRS in amrAryST*/
   signed char *drugAryStr, /*antibiotic names*/
   signed char readsBl,     /*1: checking reads not cons*/
   signed char frameshiftBl,/*1: check frameshifts*/
   unsigned int minDepthUI, /*min depth to keep amr*/
   float minPercMapF,       /*% support to keep amr*/
   float minPercTotalF,     /*% mapped reads to keep amr*/
   signed char *samFileStr, /*sam file to check*/
   signed char *outFileStr, /*output file (main)*/
   signed char *idFileStr   /*output file (ids)*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:P samFindAmrs_checkAmr
   '   - Look for anti-microbial (antibiotic) genes in the
   '     reads in a sam file
   '   o fun15 sec01:
   '     - Variable declerations
   '   o fun15 sec02:
   '     - Get the first sam entry
   '   o fun15 sec03:
   '     - Check for AMRs
   '   o fun15 sec04:
   '     - Print out read AMR stats
   '   o fun15 sec05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   schar errSC = 0;
   sint numHitsSI = 0;    /*number of amrs hits/con*/
   uint totalReadsUI = 0; /*number of kept reads*/

   struct samEntry samStackST;
   struct amrHit_checkAmr *amrHitHeapList = 0;

   FILE *idFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - Get the first sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStackST);

   errSC = setup_samEntry(&samStackST);

   if(errSC)
      goto memErr_fun15_sec05_sub03;

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
         goto fileErr_fun15_sec05_sub03;
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
            "r"
      );

      if(! outFILE)
         goto fileErr_fun15_sec05_sub03;
   } /*Else: given output file*/

   if(idFileStr)
   { /*If: I given read id file*/
      idFILE =
         fopen(
            (char *) idFileStr,
            "w"
         );

      if(! idFILE)
         goto fileErr_fun15_sec05_sub03;
      
      pIdVarHead_checkAmr(idFILE);
   } /*If: I given read id file*/

   errSC =
      get_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      ); /*read in first sam file entry*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - check for AMRs
   ^   o fun15 sec03 sub01:
   ^     - filter out less usefull entries
   ^   o fun15 sec03 sub02:
   ^     - check for amrs
   ^   o fun15 sec03 sub03:
   ^     - print consensus sequence AMRS
   ^   o fun15 sec03 sub04:
   ^     - deal with read amrs; print ids if requested/
   ^       free consensus structuerrs
   ^   o fun15 sec03 sub05:
   ^     - move to next sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec03 Sub01:
   *   - filter out less usefull entries/start loop
   \*****************************************************/

   if(! readsBl)
      pConHead_checkAmr(outFILE);

   else
      pReadHead_checkAmr(outFILE);

   while(!errSC)
   { /*Loop: check if have antibiotic resitance*/
      if(samStackST.extraStr[0] == '@')
      { /*If: header*/
         errSC =
            get_samEntry(
               &samStackST,
               &buffHeapStr,
               &lenBuffUL,
               samFILE
            );

         continue;
      } /*If: header*/

      if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If: umapped read, secondary, supplemental*/
         /*4=unmapped, 256=secondary, 2048=supplemental*/

         errSC =
            get_samEntry(
               &samStackST,
               &buffHeapStr,
               &lenBuffUL,
               samFILE
            );

         continue;
      } /*If: umapped read, secondary, supplemental*/

      /**************************************************\
      * Fun15 Sec03 Sub02:
      *   - check for amrs
      \**************************************************/

      ++totalReadsUI;

      amrHitHeapList = 
         checkAmr(
            &samStackST,  /*sequence to check*/
            amrAryST,     /*has amr's to check*/
            numAmrSI,     /*length of amrAryST*/
            &numHitsSI,   /*number amr hits for seq*/
            frameshiftBl, /*1: check frameshifts*/
            &errSC        /*for error reporting*/
         );

      if(errSC)
         goto memErr_fun15_sec05_sub03;

      /**************************************************\
      * Fun15 Sec03 Sub03:
      *   - print consensus sequence AMRS
      \**************************************************/

      if(! readsBl)
      { /*If: am printing out consensus AMRs*/
          pCon_checkAmr(
             samStackST.qryIdStr,
             amrHitHeapList,
             drugAryStr,
             outFILE
          ); /*print amr entry*/
      } /*If: am printing out consensus AMRs*/

      /**************************************************\
      * Fun15 Sec03 Sub04:
      *   - print ids/amr table (for detected AMRs)
      \**************************************************/

      else if(idFileStr)
      { /*Else If: printing out read ids*/
         pIdVarTbl_checkAmr(
            samStackST.qryIdStr,
            amrHitHeapList,
            idFILE
         );
      } /*Else If: printing out read ids*/

      /**************************************************\
      * Fun15 Sec03 Sub05:
      *   - move to next sam entry
      \**************************************************/

      freeHeapList_amrHit_checkAmr(amrHitHeapList);
      amrHitHeapList = 0;

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: check if have antibiotic resitance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - print read AMR stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(errSC > 1)
      goto memErr_fun15_sec05_sub03;

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   freeStack_samEntry(&samStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   if(readsBl)
   { /*If: printing reads AMRs*/
      pRead_checkAmr(
         minDepthUI,
         minPercMapF,
         minPercTotalF,
         totalReadsUI,
         amrAryST,
         numAmrSI,
         drugAryStr,
         outFILE
      );
   } /*If: printing reads AMRs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec05:
   ^   - clean up
   ^   o fun15 sec05 sub01:
   ^     - no error clean up
   ^   o fun15 sec05 sub02:
   ^     - memory error clean up
   ^   o fun15 sec05 sub03:
   ^     - file error clean up
   ^   o fun15 sec05 sub04:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_fun14_sec05_sub04;

   /*****************************************************\
   * Fun15 Sec05 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun15_sec05_sub03:;
   errSC = def_memErr_amrST;
   goto cleanUp_fun14_sec05_sub04;

   /*****************************************************\
   * Fun15 Sec05 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_fun15_sec05_sub03:;
   errSC = def_fileErr_amrST;
   goto cleanUp_fun14_sec05_sub04;

   /*****************************************************\
   * Fun15 Sec05 Sub04:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun14_sec05_sub04:;

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   outFILE = 0;

   if(
         idFILE
      && idFILE != stdin
      && idFILE != stdout
   ) fclose(idFILE);

   idFILE = 0;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

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
