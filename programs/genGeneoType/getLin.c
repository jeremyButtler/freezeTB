/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getLin SOF:
'   - get lineages for a genome or group of reads mapped
'     to a reference genome
'   ! fun01 to fun11 do not apply default lineages, but
'     instead return if the lineage was a default lineage
'     function 16 uses this return do assign default
'     lineages when the search is finished
'   o header:
'     - included libraries
'   o .h st01: cnt_getLin:
'     - keeps the number of reads each lineage had
'   * lineage detection functions
'     o fun01: snpNtLineage_getLin
'       - checks if a variant supports a snp lineage for a
'         nucleotide sequence
'     o fun02: snpAALineage_getLin
'       - checks if a variant supports a snp lineage for
'         an amino acid sequence
'     o fun03: delLineage_getLin
'       - checks if variant supports a deletion lineage
'     O fun04: lengthLineage_getLin
'       - scans for a lineage that is set by length
'     o .c fun05: fastFindPatterns_getLin
'       - finds all patterns using kmerFind primer search
'     o .c fun06: startCoordSort_getLin
'       - sorts coordinates from fastFindPatterns by start
'     o .c fun07: filterCoords_getLin
'       - filters overlapping coordinates from
'         fastFindPatterns_getLin by
'     o fun09: insNtLineage_getLin
'       - checks if have a nucleotide insertion lineage
'     o fun10: insNtFastLineage_getLin
'       - checks if have a nucleotide insertion lineage by
'         using a faster kmer search
'     o fun11: insAALineage_getLin
'       - checks if have an amino acid insertion lineage
'     o .c fun12: coordsToTrsLin_getLin
'       - converts alignments coordinates to a trs lineage
'     o fun13: trsNtLineage_getLin
'       - tandam repeat lineage in nucleotide sequences
'     o fun14: trsNtFastLineage_getLin
'       - detect tandam repeat lineage in nucleotide
'         sequences using the faster kmer scan
'     o fun15: trsAALineage_getLin
'       - tandam repeat lineage in amino acid sequences
'     o fun16: countNtLineage_getLin
'       - find lineages by counting number of times a
'         pattern is duplicated
'     o fun17: countNtFastLineage_getLin
'       - detect count lineage in nucleotide sequences
'         using the faster kmer scan
'     o fun18: countAALineage_getLin
'       - detects lineages based on number of times a
'         pattern is duplicated for an amino acid sequence
'     o fun19: simpleLinage_getLin
'       - finds all one variant lineages in a sequence
'     o fun20: complexLineage_getLin
'       - detect simple lineages and returns the index of
'         the detected complex lineages
'       - mLinAryST (multi_linST array) must not be sorted
'         or changed from getComplexLineages_multi_linST
'   * cnt_getLin structure functions
'     o fun21: blank_cnt_getLin
'       - blanks values in a cnt_getLin struct
'     o fun22: init_cnt_getLin
'       - initializes all values in a cnt_getLin struct
'     o fun23: freeStack_cnt_getLin
'       - frees arrays and initialzes a cnt_getLine struct
'     o fun24: freeHeap_cnt_getLin
'       - frees a cnt_getLine struct
'     o fun25: addMem_cnt_getLin
'       - add memory (if memory rellocate) to a cnt_getLin
'     o .c fun26: addLineage_cnt_getLin
'       - adds a lineage to a cnt_getLin structure
'   * lineage print functions
'     o fun27: addReadLineages_cnt_getLin
'       - adds lineages for a read to a cnt_getLin struct
'     o fun28: pReadLineages_getLin
'       - prints the lineage counts for a set of reads and
'         then prints the consensus lineage for the reads
'       !!! warning, this does not have a conistent header
'           system, because the TRS lineages are variable.
'           expect different headers for different input
'     o fun29: pGenomeLineage_getLin
'       - prints lineages found by simpleLinage_getLin &
'         complexLineage_getLin for a single sequence
'       !!! warning, this does not have a conistent header
'           system, because the TRS lineages are variable.
'           expect different headers for different input
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

#include "getLin.h"

#include "../genLib/ulCp.h"
#include "../genLib/numToStr.h"
#include "../genLib/shellSort.h"
#include "../genBio/codonFun.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genAln/indexToCoord.h"
#include "../genAln/alnSet.h"
#include "../genAln/memwater.h"
#include "../genAln/memwaterScan.h"
#include "../genAln/kmerFind.h"
#include "linST.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../genLib/genMath.h" /*only .h macros*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/ntTo2Bit.h"
!   - .h  #include "../genBio/revNtTo2Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: snpNtLineage_getLin
|   - checks if a variant supports a snp lineage for a
|     nucleotide sequence
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type for function to check
|     o 0 if had memory error
\-------------------------------------------------------*/
signed int
snpNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - checks if a variant supports a snp lineage
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - check if can get lineage + move to position
   '   o fun01 sec03:
   '     - allocate memory for the lineage check
   '   o fun01 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char baseSC = 0;
   signed char refMatchBl = 0; /*goes to 1 if have ref*/
   signed int seqPosSI = 0;

   signed int retSI = 0;
   signed char breakBl = 0;

   signed int refPosSI = 0;
   signed int siCig = 0;
   signed int ntLeftSI = 0;

   signed int *diffCntArySI = 0;
      /*differences between lineage and sequence*/
   signed int *patternArySI = 0;
      /*position at for each pattern in the lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - check if can get lineage + move to position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
     linSTPtr->startSI < (signed int) samSTPtr->refStartUI
   ) goto noMatch_fun01_sec05; /*is before read*/

   else if(
      linSTPtr->endSI > (signed int) samSTPtr->refEndUI
   ) goto noMatch_fun01_sec05; /*is before read*/

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun01_sec05;

   else if(linSTPtr->checkTypeSC != def_snpType_linST)
      goto wrongLineage_fun01_sec05;

   if(linSTPtr->defGroupBl)
   { /*If: default group, can not check lineage*/
      refMatchBl = 1;
      goto noMatch_fun01_sec05;
   } /*If: default group, can not check lineage*/

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - allocate memory for the lineage check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(linSTPtr->seqLenSI > 0)
      diffCntArySI =
         calloc(linSTPtr->seqLenSI, sizeof(signed int));
   else
      diffCntArySI = calloc(1, sizeof(signed int));
   if(! diffCntArySI)
      goto memErr_fun01_sec05;

   if(linSTPtr->seqLenSI > 0)
      patternArySI =
         calloc(linSTPtr->seqLenSI, sizeof(signed int));
   else
      patternArySI = calloc(1, sizeof(signed int));
   if(! patternArySI)
      goto memErr_fun01_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - check if lineage is in read
   ^   o fun01 sec04 sub01:
   ^     - setup position vars and start loop
   ^   o fun01 sec04 sub02:
   ^     - deal with insertion/deletion entries
   ^   o fun01 sec04 sub03:
   ^     - deal with match/snp entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec04 Sub01:
   *   - setup position vars and start loop
   \*****************************************************/

   refPosSI = posArySI[0];
   seqPosSI = posArySI[1];
   siCig = posArySI[2];
   ntLeftSI = posArySI[3];

   while(! breakBl)
   { /*Loop: get differences for each lineage*/

      /**************************************************\
      * Fun01 Sec04 Sub02:
      *   - deal with insertion/deletion entries
      \**************************************************/

      if(samSTPtr->cigTypeStr[siCig] == 'X')
         ;
      else if(samSTPtr->cigTypeStr[siCig] == 'M')
         ;
      else if(samSTPtr->cigTypeStr[siCig] == '=')
         ;
      else
      { /*Else: have a indel/masking differnce*/
         retSI = 0;
         breakBl = 1;

         for(retSI=0; retSI < linSTPtr->seqLenSI;++retSI)
         { /*Loop: add difference in*/
            diffCntArySI[retSI] += ntLeftSI;

            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               breakBl = 0;

            if(samSTPtr->cigTypeStr[siCig] == 'D')
               patternArySI[retSI] += ntLeftSI;
         } /*Loop: add difference in*/

         if(samSTPtr->cigTypeStr[siCig] == 'D')
            refPosSI -= ntLeftSI;
         else
            seqPosSI -= ntLeftSI;

         ++siCig;
         ntLeftSI = samSTPtr->cigArySI[siCig];
         continue;
      } /*Else: have a indel/masking differnce*/

      /**************************************************\
      * Fun01 Sec04 Sub03:
      *   - deal with match/snp entries
      *   o fun01 sec04 sub03 cat01:
      *     - check if new base is different then patterns
      *   o fun01 sec04 sub03 cat02:
      *     - move to next base and check if finished
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec04 Sub03 Cat01:
      +   - check if new base is different then patterns
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      retSI = 0;
      breakBl = 1;
      baseSC = samSTPtr->seqStr[seqPosSI];

      for(retSI = 0; retSI < linSTPtr->seqLenSI; ++retSI)
      { /*Loop: find difference*/
         if(! linSTPtr->seqAryStr[patternArySI[retSI]])
         { /*If: finished with this pattern*/
            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               goto match_fun01_sec05;
            else
               continue;
         } /*If: finished with this pattern*/

         if(
               baseSC
            != linSTPtr->seqAryStr[retSI][
                  patternArySI[retSI]
               ]
         ) ++diffCntArySI[retSI];

         ++patternArySI[retSI];
         if(
            ! linSTPtr->seqAryStr[retSI][
              patternArySI[retSI]
            ]
         ){ /*If: finished with this pattern*/
            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               goto match_fun01_sec05;
            else
               continue;
         } /*If: finished with this pattern*/

         else if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
            breakBl = 0;
            /*still have one pattern that works*/
      } /*Loop: find difference*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec04 Sub03 Cat02:
      +   - move to next base and check if finished
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++refPosSI;
      ++seqPosSI;

      if(refPosSI > linSTPtr->endSI)
      { /*If: end of lineage region*/
         if(! breakBl)
            goto match_fun01_sec05;
         else
            goto noMatch_fun01_sec05;
      } /*If: end of lineage region*/

      --ntLeftSI;

      if(ntLeftSI <= 0)
      { /*If: need to move back an entry*/
         ++siCig;
         ntLeftSI = samSTPtr->cigArySI[siCig];
      } /*If: need to move back an entry*/
   } /*Loop: get differences for each lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(breakBl)
      goto noMatch_fun01_sec05;

   match_fun01_sec05:;
      retSI = 1;
      goto ret_fun01_sec05;

   noMatch_fun01_sec05:;
      if(refMatchBl)
         retSI = -1;
      else
         retSI = -2;
      goto ret_fun01_sec05;

   wrongLineage_fun01_sec05:;
      retSI = -3;
      goto ret_fun01_sec05;

   memErr_fun01_sec05:;
      retSI = 0;
      goto ret_fun01_sec05;

   ret_fun01_sec05:;
      if(diffCntArySI)
         free(diffCntArySI);
      diffCntArySI = 0;

      if(patternArySI)
         free(patternArySI);
      patternArySI = 0;

      return retSI;
} /*snpNtLineage_getLin*/

/*-------------------------------------------------------\
| Fun02: snpAALineage_getLin
|   - checks if a variant supports a snp lineage for an
|     amino acid sequence
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type input
|     o 0 if had memory error
\-------------------------------------------------------*/
signed int
snpAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - checks if a variant supports a snp lineage
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - check if can get lineage + move to position
   '   o fun02 sec03:
   '     - allocate memory for the lineage check
   '   o fun02 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char refMatchBl = 0; /*goes to 1 if have ref*/
   signed char aaSC = 0;

   signed int retSI = 0;
   signed char breakBl = 0;

   signed int refPosSI = 0;
   signed int seqPosSI = 0;
   signed int siCig = 0;
   signed int ntLeftSI = 0;

   signed int *diffCntArySI = 0;
      /*differences between lineage and sequence*/
   signed int diffSI = 0; /*number nt different*/
   signed int *patternArySI = 0;
      /*position at for each pattern in the lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - check if can get lineage + move to position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
     linSTPtr->startSI < (signed int) samSTPtr->refStartUI
   ) goto noMatch_fun02_sec05; /*is before read*/

   else if(
      linSTPtr->endSI > (signed int) samSTPtr->refEndUI
   ) goto noMatch_fun02_sec05; /*is before read*/

   else if(linSTPtr->moleculeTypeSC != def_aaSeq_linST)
      goto wrongLineage_fun02_sec05;

   else if(linSTPtr->checkTypeSC != def_snpType_linST)
      goto wrongLineage_fun02_sec05;

   if(linSTPtr->defGroupBl)
   { /*If: default group, can not check lineage*/
      refMatchBl = 1;
      goto noMatch_fun02_sec05;
   } /*If: default group, can not check lineage*/

   if(linSTPtr->revDirBl)
      findRefPos_samEntry(
         samSTPtr,
         &posArySI[2],    /*cigar position*/
         &posArySI[3],   /*bases left at cigar position*/
         linSTPtr->endSI,/*were to end at*/
         &posArySI[0],    /*reference position*/
         &posArySI[1]      /*sequence position*/
      ); /*move to last base in lineage*/
   else
      findRefPos_samEntry(
         samSTPtr,
         &posArySI[2],    /*cigar position*/
         &posArySI[3],   /*bases left at cigar position*/
         linSTPtr->startSI,/*were to end at*/
         &posArySI[0],    /*reference position*/
         &posArySI[1]      /*sequence position*/
      ); /*move to first base in lineage*/

   seqPosSI = posArySI[1];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - allocate memory for the lineage check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(linSTPtr->seqLenSI > 0)
      diffCntArySI =
         calloc(linSTPtr->seqLenSI, sizeof(signed int));
   else
      diffCntArySI = calloc(1, sizeof(signed int));
   if(! diffCntArySI)
      goto memErr_fun02_sec05;

   if(linSTPtr->seqLenSI > 0)
      patternArySI =
         calloc(linSTPtr->seqLenSI, sizeof(signed int));
   else
      patternArySI = calloc(1, sizeof(signed int));
   if(! patternArySI)
      goto memErr_fun02_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - check if lineage is in read
   ^   o fun02 sec04 sub01:
   ^     - setup position vars and start loop
   ^   o fun02 sec04 sub02:
   ^     - find the next three bases
   ^   o fun02 sec04 sub03:
   ^     - apply differences (if have any)
   ^   o fun02 sec04 sub04:
   ^     - check if amino acids match
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - setup position vars and start loop
   \*****************************************************/

   refPosSI = posArySI[0];
   seqPosSI = posArySI[1];
   siCig = posArySI[2];
   ntLeftSI = posArySI[3];

   while(! breakBl)
   { /*Loop: get differences for each lineage*/

      /**************************************************\
      * Fun02 Sec04 Sub02:
      *   - find the next three bases
      *   - my method here is to keep the position in
      *     exact alignment. This does not make the best
      *     biological sense, but it prevents one
      *     insertion or deletion changing the translation
      \**************************************************/

      retSI = 0;

      while(retSI < 2)
      { /*Loop: get cigar type for the next three bases*/
         aaSC = samSTPtr->cigTypeStr[siCig];

         if(linSTPtr->revDirBl)
         { /*If: going backwards*/
            if(aaSC == 'D')
            { /*If: on a deletion*/
               ++diffSI;
               --refPosSI;
               ++retSI;
               goto nextRevNt_fun02_sec04_sub02;
            } /*If: on a deletion*/

            else if(aaSC == 'I')
            { /*Else If: was a insertion*/
               ++diffSI;
               --seqPosSI;
               ++ntLeftSI;
               goto nextRevNt_fun02_sec04_sub02;
            } /*Else If: was a insertion*/

            else
            { /*Else: snp or match*/
               ++retSI;
               --refPosSI;
               --seqPosSI;

               nextRevNt_fun02_sec04_sub02:;
                 ++ntLeftSI;

                 if(ntLeftSI >= samSTPtr->cigArySI[siCig])
                 { /*If: need to move to next base*/
                    --siCig;
                    ntLeftSI = 0;
                 } /*If: need to move to next base*/

                 continue;
            } /*Else: snp or match*/
         } /*If: going backwards*/

         else
         { /*Else: going forwards*/
            if(aaSC == 'D')
            { /*If: on a deletion*/
               ++diffSI;
               ++retSI;
               ++refPosSI;
               goto nextNt_fun02_sec04_sub02;
            } /*If: on a deletion*/

            else if(aaSC == 'I')
            { /*Else If: was a insertion*/
               ++diffSI;
               ++seqPosSI;
               goto nextNt_fun02_sec04_sub02;
            } /*Else If: was a insertion*/

            else
            { /*Else: snp or match*/
               ++retSI;
               ++refPosSI;
               ++seqPosSI;

               nextNt_fun02_sec04_sub02:;
                  --ntLeftSI;

                  if(ntLeftSI <= 0)
                  { /*If: need to move to next entry*/
                     ++siCig;
                     ntLeftSI = samSTPtr->cigArySI[siCig];
                  } /*If: need to move to next entry*/
            } /*Else: snp or match*/
         } /*Else: going forwards*/
      } /*Loop: get cigar type for the next three bases*/

      /**************************************************\
      * Fun02 Sec04 Sub03:
      *   - apply differences (if have any)
      \**************************************************/

      if(diffSI)
      { /*If: had a differnce*/
         diffSI /= 3;

         if(! diffSI)
            diffSI = 1;

         retSI = 0;
         breakBl = 1;

         for(retSI=0; retSI < linSTPtr->seqLenSI;++retSI)
         { /*Loop: add difference in*/
            diffCntArySI[retSI] += diffSI;

            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               breakBl = 0;

            if(linSTPtr->seqAryStr[patternArySI[retSI]])
               ++patternArySI[retSI];
               /*move to next item in pattern*/
         } /*Loop: add difference in*/

         /*move to first base in next codon*/
         if(linSTPtr->revDirBl)
            --seqPosSI;
         else
            ++seqPosSI;

         continue;
      } /*If: had a differnce*/

      /**************************************************\
      * Fun02 Sec04 Sub04:
      *   - check if amino acids match
      \**************************************************/

      if(linSTPtr->revDirBl)
      { /*If: going backwards*/
         aaSC =
            revCodonToAA_codonFun(
               samSTPtr->seqStr[seqPosSI],
               samSTPtr->seqStr[seqPosSI + 1],
               samSTPtr->seqStr[seqPosSI + 2]
            );
         --seqPosSI; /*move to first base in next codon*/
      } /*If: going backwards*/

      else
      { /*Else: going forwards*/
         aaSC =
             codonToAA_codonFun(
                samSTPtr->seqStr[seqPosSI - 2],
                samSTPtr->seqStr[seqPosSI - 1],
                samSTPtr->seqStr[seqPosSI]
             );
         ++seqPosSI; /*move to first base in next codon*/
      } /*Else: going forwards*/

      retSI = 0;
      breakBl = 1;

      for(retSI = 0; retSI < linSTPtr->seqLenSI; ++retSI)
      { /*Loop: find difference*/
         if(
            ! linSTPtr->seqAryStr[retSI][
               patternArySI[retSI]
            ]
         ){ /*If: finished with this pattern*/
            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               goto match_fun02_sec05;
            else
               continue;
         } /*If: finished with this pattern*/

         if(
               (aaSC & ~32)
            != (linSTPtr->seqAryStr[retSI][
                  patternArySI[retSI]
               ] & ~32)
         ) ++diffCntArySI[retSI];

         ++patternArySI[retSI];
         if(
            ! linSTPtr->seqAryStr[retSI][
              patternArySI[retSI]
            ]
         ){ /*If: finished with this pattern*/
            if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
               goto match_fun02_sec05;
            else
               continue;
         } /*If: finished with this pattern*/

         else if(diffCntArySI[retSI] <= linSTPtr->fudgeSI)
            breakBl = 0;
            /*still have one pattern that works*/
      } /*Loop: find difference*/
   } /*Loop: get differences for each lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(breakBl)
      goto noMatch_fun02_sec05;

   match_fun02_sec05:;
      retSI = 1;
      goto ret_fun02_sec05;

   noMatch_fun02_sec05:;
      if(refMatchBl)
         retSI = -1;
      else
         retSI = -2;
      goto ret_fun02_sec05;

   wrongLineage_fun02_sec05:;
      retSI = -3;
      goto ret_fun02_sec05;

   memErr_fun02_sec05:;
      retSI = 0;
      goto ret_fun02_sec05;

   ret_fun02_sec05:;
      if(diffCntArySI)
         free(diffCntArySI);
      diffCntArySI = 0;

      if(patternArySI)
         free(patternArySI);
      patternArySI = 0;

      return retSI;
} /*snpAALineage_getLin*/

/*-------------------------------------------------------\
| Fun03: delLineage_getLin
|   - checks if the variant supports a deletion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type input
\-------------------------------------------------------*/
signed int
delLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - checks if a variant supports a snp lineage
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if can get lineage + move to position
   '   o fun03 sec03:
   '     - check if lineage is in read
   '   o fun03 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int lenDelSI =
      linSTPtr->endSI - linSTPtr->startSI + 1;
   signed int delCntSI = 0;
   signed int diffSI = 0;

   signed int refPosSI = 0;
   signed int seqPosSI = 0;
   signed int siCig = 0;
   signed int ntLeftSI = 0;

   signed char refMatchBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if can get lineage + move to position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
     linSTPtr->startSI < (signed int) samSTPtr->refStartUI
   ) goto noMatch_fun03_sec04; /*is before read*/

   else if(
      linSTPtr->endSI > (signed int) samSTPtr->refEndUI
   ) goto noMatch_fun03_sec04; /*is before read*/

   else if(linSTPtr->checkTypeSC != def_delType_linST)
      goto wrongLineage_fun03_sec04;

   if(linSTPtr->defGroupBl)
   { /*If: default group, can not check lineage*/
      refMatchBl = 1;
      goto noMatch_fun03_sec04;
   } /*If: default group, can not check lineage*/


   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],    /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - check if lineage is in read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refPosSI = posArySI[0];
   seqPosSI = posArySI[1];
   siCig = posArySI[2];
   ntLeftSI = posArySI[3];

   while(refPosSI <= linSTPtr->endSI)
   { /*Loop: get differences for each lineage*/
      if(samSTPtr->cigTypeStr[siCig] == 'D')
      { /*If: deletion*/
         delCntSI += ntLeftSI;
         refPosSI += ntLeftSI;
         /*If I overshoot with deletions it means that the
         `  deletion is larger in the read then the
         `  reference, so it is still different
         */
      } /*If: deletion*/

      else if(samSTPtr->cigTypeStr[siCig] == 'I')
      { /*Else If: insertion*/
         seqPosSI += ntLeftSI;
         diffSI += ntLeftSI;
      } /*Else If: insertion*/

      else
      { /*Else: snp or match*/
         /*check if overshot the target end*/
         if(refPosSI + ntLeftSI > linSTPtr->endSI)
            diffSI += linSTPtr->endSI - refPosSI;
         else
            diffSI += ntLeftSI;

         refPosSI += ntLeftSI;
         seqPosSI += ntLeftSI;
      } /*Else: snp or match*/

      if(diffSI > linSTPtr->fudgeSI)
         break;
      ++siCig;
      ntLeftSI += samSTPtr->cigArySI[siCig];
   } /*Loop: get differences for each lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(delCntSI > lenDelSI)
      diffSI += (delCntSI - lenDelSI);
   else
      diffSI += (lenDelSI - delCntSI);


   if(diffSI <= linSTPtr->fudgeSI)
      return 1;
   else
      goto noMatch_fun03_sec04;

   noMatch_fun03_sec04:;
      if(refMatchBl)
         return -1;
      else
         return -2;

   wrongLineage_fun03_sec04:;
      return -3;
} /*delLineage_getLin*/

/*-------------------------------------------------------\
| Fun04: lengthLineage_getLin
|   - scans for a lineage that is set by length
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
\-------------------------------------------------------*/
signed char
lengthLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   / Fun04 TOC:
   '   - scans for a lineage that is set by length
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - check if can get lineage + move to position
   '   o fun04 sec03:
   '     - find the lineage to assign
   '   o fun04 sec04:
   '     - return the found lineage
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int refPosSI = 0;
   signed int seqPosSI = 0;
   signed int siCig = 0;
   signed int ntLeftSI = 0;

   signed char refMatchBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - check if can get lineage + move to position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
     linSTPtr->startSI < (signed int) samSTPtr->refStartUI
   ) goto noMatch_fun04_sec04; /*is before read*/

   else if(
      linSTPtr->endSI > (signed int) samSTPtr->refEndUI
   ) goto noMatch_fun04_sec04; /*is before read*/

   else if(linSTPtr->checkTypeSC != def_lenType_linST)
      goto noMatch_fun04_sec04; /*is before read*/

   if(linSTPtr->defGroupBl)
   { /*If: default group, can not check lineage*/
      refMatchBl = 1;
      goto noMatch_fun04_sec04;
   } /*If: default group, can not check lineage*/

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],    /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   refPosSI = posArySI[0];
   seqPosSI = posArySI[1];
   siCig = posArySI[2];
   ntLeftSI = posArySI[3];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,          /*cigar position*/
      &ntLeftSI,      /*bases left at cigar position*/
      linSTPtr->endSI,/*were to end at*/
      &refPosSI,     /*reference position*/
      &seqPosSI      /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - find the lineage to assign
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqPosSI -= posArySI[1];
   ++seqPosSI; /*convert index 0 to index 1*/

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find matching lineage*/
      refPosSI =
         linSTPtr->seqLenArySI[siCig] + linSTPtr->fudgeSI;
      ntLeftSI =
         linSTPtr->seqLenArySI[siCig] - linSTPtr->fudgeSI;
 
      if(seqPosSI < ntLeftSI)
         continue;
      else if(seqPosSI > refPosSI)
         continue;
      else
         goto foundLineage_fun04_sec04;
   } /*Loop: find matching lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - return the found lineage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return -1; /*no lineage matched*/

   noMatch_fun04_sec04:;
      if(refMatchBl)
         return -1;
      else
         return -2; /*do not have complete position*/
   foundLineage_fun04_sec04:;
      return 1; /*found the lineage*/
} /*lengthLineagee_getLin*/

/*-------------------------------------------------------\
| Fun05: fastFindPatterns_getLin
|   - finds all patterns using kmerFind's primer search
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings and to hold the query sequence to check
|   - linSTPtr:
|     o linage with fast lineage to search for
|   - seqStr:
|     o c-string with sequence to scan
|   - seqLenSI:
|     o length of sequence (seqStr)
|   - startArySLPtr:
|     o signed long array pionter to get starting
|       coordinates
|   - endArySLPtr:
|     o signed long array pionter to get end coordinates
|   - scoreArySLPtr:
|     o signed long array pointer to get scores
|   - arySizeSIPtr:
|     o signed int pionter with the current size of the
|       arrays
| Output:
|   - Modifies:
|     o startArySLPtr to have non-overlapping starting
|       coordinates
|     o endArySLPtr to have non-overlapping ending
|       coordinates
|     o scoreArySLPtr score of each alignment
|     o resizes startArySLPtr, endArySLPtr, and
|       scoreArySLPtr if needed
|     o sets arySizeSIPtr to new array sizes (if resized)
|   - Returns:
|     o number of alignments found
|     o 0 if no alignments were found
|     o -1 for memory errors
\-------------------------------------------------------*/
signed int
fastFindPatterns_getLin(
   struct tblST_kmerFind *tblSTPtr,/*has settings*/
   struct one_linST *linSTPtr,    /*has patterns*/
   signed char *seqStr,           /*sequence to scan*/
   signed int seqLenSI,           /*sequence length*/
   signed long **startArySLPtr,   /*gets alinment start*/
   signed long **endArySLPtr,     /*gets alinment end*/
   signed long **scoreArySLPtr,   /*gets alinment scores*/
   signed int *arySizeSIPtr       /*size of arrays*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ^ Fun05 TOC:
   '   o fun05 sec01:
   '     - varaible declerations
   '   o fun05 sec02:
   '     - assign sequence to table and memory
   '   o fun05 sec03:
   '     - check sequence for spacers
   '   o fun05 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - varaible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0; /*error messages*/
   signed int lenSI = 0;
   signed long *swapSLPtr = 0;

   unsigned char matchBl = 0;
   signed char firstTimeBl = 1;

   unsigned int uiPrim = 0;

   /*alignemnt variables; I keep*/
   signed long scoreSL = 0;

   /*to keep the old assigned sequence*/
   struct seqST *oldSeqST = 0;

   signed int coordArySI[4];
   struct seqST seqDoNotFree;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - assign sequence to table and memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_seqST(&seqDoNotFree);
   seqDoNotFree.seqStr = seqStr;
   seqDoNotFree.seqLenSL = seqLenSI;

   oldSeqST = tblSTPtr->seqSTPtr;
   tblSTPtr->seqSTPtr = &seqDoNotFree;
   qckBlank_tblST_kmerFind(tblSTPtr, 0);

   if(! *scoreArySLPtr)
   { /*If: need memory*/
      lenSI = linSTPtr->seqLenSI << 1;

      if(*startArySLPtr)
         free(*startArySLPtr);
      *startArySLPtr = 0;
      *startArySLPtr=malloc(lenSI * sizeof(signed long));
      if(! *startArySLPtr)
         goto memErr_fun05_sec04;

      if(*endArySLPtr)
         free(*endArySLPtr);
      *endArySLPtr = 0;
      *endArySLPtr = malloc(lenSI * sizeof(signed long));
      if(! *endArySLPtr)
         goto memErr_fun05_sec04;

      if(*scoreArySLPtr)
         free(*scoreArySLPtr);
      *scoreArySLPtr = 0;
      *scoreArySLPtr=malloc(lenSI * sizeof(signed long));
      if(! *scoreArySLPtr)
         goto memErr_fun05_sec04;

      *arySizeSIPtr = lenSI;
      lenSI = 0;
   } /*If: need memory*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - check sequence for patterns
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   do{
      errSC =
         nextNoIndexSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         );

      for(
         uiPrim = 0;
         uiPrim < (unsigned int) linSTPtr->seqLenSI;
         ++uiPrim
      ){ /*Loop: detect primers in each chunk*/

         /***********************************************\
         * Fun05 Sec03 Sub0x:
         *   - resize arrays if needed
         \***********************************************/

         if(lenSI >= *arySizeSIPtr)
         { /*If: need more memory*/
            *arySizeSIPtr += (*arySizeSIPtr << 1);

            swapSLPtr =
               realloc(
                  *startArySLPtr,
                  (*arySizeSIPtr<<1) * sizeof(signed long)
               );
            if(! swapSLPtr)
               goto memErr_fun05_sec04;
            *startArySLPtr = swapSLPtr;

            swapSLPtr =
               realloc(
                  *endArySLPtr,
                  (*arySizeSIPtr<<1) * sizeof(signed long)
               );
            if(! swapSLPtr)
               goto memErr_fun05_sec04;
            *endArySLPtr = swapSLPtr;

            swapSLPtr =
               realloc(
                  *scoreArySLPtr,
                  (*arySizeSIPtr<<1) * sizeof(signed long)
               );
            if(! swapSLPtr)
               goto memErr_fun05_sec04;
            *scoreArySLPtr = swapSLPtr;
         } /*If: need more memory*/

         /***********************************************\
         * Fun05 Sec03 Sub0y:
         *   - see if have any matches
         \***********************************************/

         matchBl =
            findNoIndexRefInChunk_kmerFind(
               tblSTPtr,
               &linSTPtr->fastAryST[uiPrim],
               0.8, /*at least an 80% score in alingment*/
               &scoreSL,
               coordArySI
            );

         if(matchBl)
         { /*If: found a hit*/
             (*startArySLPtr)[lenSI] = coordArySI[0];
             (*endArySLPtr)[lenSI] = coordArySI[1];
             (*scoreArySLPtr)[lenSI] = scoreSL;
             ++lenSI; 
         } /*If: found a hit*/
      } /*Loop: detect primers in each chunk*/
   } while(! errSC);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun05_sec04;

   memErr_fun05_sec04:;
      lenSI = -1;
      goto ret_fun05_sec04;
      
   ret_fun05_sec04:;
      qckBlank_tblST_kmerFind(tblSTPtr, 0);
         /*using quick blank here, since all non-filled
	   	`   kmers will already be blanked
	   	*/

      /*reset table to original sequences*/
      tblSTPtr->seqSTPtr = oldSeqST;

      return lenSI;
} /*fastFindPatterns_getLin*/

/*-------------------------------------------------------\
| Fun06: startCoordSort_getLin
|   - sorts coordinates from fastFindPatterns_getLin by
|     starting coordinates
| Input:
|   - startArySL:
|     o starting coordinate array to sort
|   - endArySL:
|     o ending coordinate array to keep in sync with
|       startArySL
|   - scoreArySL:
|     o score array to keep in sync with startArySL
|   - lenSI:
|     o number of elements in the arrays (index 1)
| Output:
|   - Modifies:
|     o startArySL to be sorted by the starting coordinate
|     o endArySL and scoreArySL to be kept in synce with
|       startArySL
\-------------------------------------------------------*/
void
startCoordSort_getLin(
   signed long *startArySL, /*start coordinates to sort*/
   signed long *endArySL,   /*ending coordinates*/
   signed long *scoreArySL, /*alignment scores*/
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - sorts the mapped barcodes by starting coordinates
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - find the number of rounds to sort for
   '   o fun06 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int siIndex = 0;
   signed int siElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   --lenSI;
   if(! lenSI)
      return;

   /*Recursion fromula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(
         siIndex = 0;
         siIndex <= subSI;
         ++siIndex
      ){ /*Loop: though sub array*/
         siElm = siIndex;

         for(
            siElm = siIndex;
            siElm + subSI <= lenSI;
            siElm += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = siElm + subSI;

            if(startArySL[siElm] >  startArySL[nextSI])
            { /*If: I need to swap an element*/
               startArySL[siElm] ^= startArySL[nextSI];
               startArySL[nextSI] ^= startArySL[siElm];
               startArySL[siElm] ^= startArySL[nextSI];

               endArySL[siElm] ^= endArySL[nextSI];
               endArySL[nextSI] ^= endArySL[siElm];
               endArySL[siElm] ^= endArySL[nextSI];

               scoreArySL[siElm] ^= scoreArySL[nextSI];
               scoreArySL[nextSI] ^= scoreArySL[siElm];
               scoreArySL[siElm] ^= scoreArySL[nextSI];

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(startArySL[onSI] >startArySL[lastSI])
                     break; /*Positioned the element*/
   
                  startArySL[onSI] ^= startArySL[lastSI];
                  startArySL[lastSI] ^= startArySL[onSI];
                  startArySL[onSI] ^= startArySL[lastSI];

                  endArySL[onSI] ^= endArySL[lastSI];
                  endArySL[lastSI] ^= endArySL[onSI];
                  endArySL[onSI] ^= endArySL[lastSI];

                  scoreArySL[onSI] ^= scoreArySL[lastSI];
                  scoreArySL[lastSI] ^= scoreArySL[onSI];
                  scoreArySL[onSI] ^= scoreArySL[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*If: I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*startCoordSort_getLin*/

/*-------------------------------------------------------\
| Fun07: filterCoords_getLin
|   - filters overlapping coordinates from
|     fastFindPatterns_getLin by
| Input:
|   - minScoreSL:
|     o minimum score to keep an alignment
|   - startArySL:
|     o starting coordinate array to filter
|   - endArySL:
|     o ending coordinate array to filter
|   - scoreArySL:
|     o score array to filter
|   - lenSI:
|     o number of elements in the arrays (index 1)
| Output:
|   - Modifies:
|     o startArySL endArySL, and scoreArySL to have
|       overlapping alignments filtered out
\-------------------------------------------------------*/
signed int
filterCoords_getLin(
   signed long minScoreSL,  /*minimum score to keep aln*/
   signed long *startArySL, /*start coordinates to sort*/
   signed long *endArySL,   /*ending coordinates*/
   signed long *scoreArySL, /*alignment scores*/
   signed int lenSI
){
   signed int siAln = 0;
   signed int endSI = 0;
   signed int cmpSI = 0;
   #define def_maxSL_fun07_getLin (((unsigned long) -1)>>1)

   if(minScoreSL > 0)
   { /*If: have a min score to filter by*/
      endSI = 0;

      for(siAln = 0; siAln < lenSI; ++siAln)
      { /*Loop: remove low scores (non-percent check)*/
         if(scoreArySL[siAln] < minScoreSL)
            startArySL[siAln] = def_maxSL_fun07_getLin;
         else
            ++endSI;
      } /*Loop: remove low scores (non-percent check)*/
   } /*If: have a min score to filter by*/

   else
      endSI = lenSI;


   lenSI = endSI;
   endSI = 0;
   cmpSI = 0;

   if(startArySL[0] != def_maxSL_fun07_getLin)
      endSI = 1;
   else
      endSI = 0;

   for(siAln = 1; siAln < lenSI; ++siAln)
   { /*Loop: mark alns*/
      if(startArySL[siAln] == def_maxSL_fun07_getLin)
         continue; /*already flagged this alignment*/

      if(endArySL[cmpSI] > startArySL[siAln])
      { /*If: have overlapped alignment*/
         if(scoreArySL[cmpSI] >= scoreArySL[siAln])
            startArySL[siAln] = def_maxSL_fun07_getLin;

         else
         { /*Else: remove last alignment*/
            startArySL[cmpSI] = def_maxSL_fun07_getLin;
            cmpSI = siAln;
         } /*Else: remove last alignment*/
      } /*If: have overlapped alignment*/

      else
      { /*Else: keeping both alignments*/
         ++endSI;
         cmpSI = siAln;
      } /*Else: keeping both alignments*/
   } /*Loop: mark alns*/

   startCoordSort_getLin(
      startArySL,
      endArySL,
      scoreArySL,
      lenSI
   ); /*sort by starting position*/

   return endSI;
} /*filterCoords_getLin*/

/*-------------------------------------------------------\
| Fun09: insNtLineage_getLin
|   - checks if have a nucleotide insertion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
\-------------------------------------------------------*/
signed int
insNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - checks if variant supports a insertion lineage
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - find start and end position of insertion
   '   o fun09 sec03:
   '     - check if have match and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int seqPosSI = 0;
   signed int seqLenSI = 0;
   signed int refEndSI = 0;
   signed int endSI = 0; /*target to end on*/

   signed int alnCoordsSI[4] = {0, 0, 0, 0};

   signed int siCig = 0;
   signed int ntOnSI = 0;

   signed long scoreSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find start and end position of insertion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto noLineageCoverage_fun09_sec03;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto noLineageCoverage_fun09_sec03;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun09_sec03;

   else if(linSTPtr->checkTypeSC != def_insType_linST)
      goto wrongLineage_fun09_sec03;

   if(linSTPtr->defGroupBl)
      goto defaultLineage_fun09_sec03;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqPosSI = posArySI[1];

   if(linSTPtr->startSI + 1 <= linSTPtr->endSI)
   { /*If: user is targeting a region*/
      if(samSTPtr->cigTypeStr[posArySI[2]] == 'I')
         ;
      else if(samSTPtr->cigTypeStr[posArySI[2] +1] != 'I')
         goto noIns_fun09_sec03;

      endSI = posArySI[0] + 1; /*move past insertion*/
   } /*If: user is targeting a region*/

   else
      endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - check if have match and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];
   ++seqLenSI;

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find matching patterns*/
      scoreSL =
         simple_memwater(
            &samSTPtr->seqStr[seqPosSI],
            seqLenSI,
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            alnCoordsSI
         );
      if(scoreSL < 0)
         goto memErr_fun09_sec03;

      if(scoreSL > linSTPtr->minScoreSI)
         goto foundMatch_fun09_sec03;
   } /*Loop: find matching patterns*/

   goto noIns_fun09_sec03;

   foundMatch_fun09_sec03:;
      return 1;
   defaultLineage_fun09_sec03:;
      return -1;
   noIns_fun09_sec03:;
      return -2;
   noLineageCoverage_fun09_sec03:;
      return -3;
   wrongLineage_fun09_sec03:;
      return -4;
   memErr_fun09_sec03:;
      return 0;
} /*insNtLineage_getLin*/

/*-------------------------------------------------------\
| Fun10: insNtFastLineage_getLin
|   - checks if have a nucleotide insertion lineage by
|     using a faster kmer search
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
|     o 1 if match found
\-------------------------------------------------------*/
signed int
insNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - checks if have a nucleotide insertion lineage by
   '     using a faster kmer search
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun10 sec02:
   '     - find start and end position of insertion
   '   o fun10 sec03:
   '     - check if have match and return
   '   o fun10 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int seqLenSI = 0;
   signed int refEndSI = 0;
   signed int endSI = 0; /*target end position*/

   signed int siCig = 0;
   signed int ntOnSI = 0;

   signed long scoreSL = 0;

   signed long *startHeapArySL = 0;
   signed long *endHeapArySL = 0;
   signed long *scoreHeapArySL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - find start and end position of insertion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto noLineageCoverage_fun10_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto noLineageCoverage_fun10_sec04;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun10_sec04;

   else if(linSTPtr->checkTypeSC != def_insFastType_linST)
      goto wrongLineage_fun10_sec04;

   if(linSTPtr->defGroupBl)
      goto defaultLineage_fun10_sec04;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   if(linSTPtr->startSI + 1 <= linSTPtr->endSI)
   { /*If: user is targeting a region*/
      if(samSTPtr->cigTypeStr[posArySI[2]] == 'I')
         ;
      else if(samSTPtr->cigTypeStr[posArySI[2] +1] != 'I')
         goto noIns_fun10_sec04;

      endSI = posArySI[0] + 1; /*move past insertion*/
   } /*If: user is targeting a region*/

   else
      endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - check if have match and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];

   scoreSL =
      fastFindPatterns_getLin(
         tblSTPtr,
         linSTPtr,
         &samSTPtr->seqStr[posArySI[1]],
         seqLenSI,
         &startHeapArySL,
         &endHeapArySL,
         &scoreHeapArySL,
         &endSI           /*can ignore this*/
      ); /*bit inefficent, but did not want to write
         `  a second function
         */
   if(scoreSL < 0)
      goto memErr_fun10_sec04;
   else if(! scoreSL)
      goto noIns_fun10_sec04;

   for(seqLenSI = 0; seqLenSI < scoreSL; ++seqLenSI)
   { /*Loop: detect if have any hits with a good score*/
      if(scoreHeapArySL[seqLenSI] >= linSTPtr->minScoreSI)
         goto foundMatch_fun10_sec04;
         /*at least 1 match found*/
   } /*Loop: detect if have any hits with a good score*/

   goto noIns_fun10_sec04; /*no good scoring matches*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   foundMatch_fun10_sec04:;
      scoreSL = 1;
      goto ret_fun10_sec04;

   defaultLineage_fun10_sec04:;
      scoreSL = -1;
      goto ret_fun10_sec04;

   noIns_fun10_sec04:;
      scoreSL = -2;
      goto ret_fun10_sec04;

   noLineageCoverage_fun10_sec04:;
      scoreSL = -3;
      goto ret_fun10_sec04;

   wrongLineage_fun10_sec04:;
      scoreSL = -4;
      goto ret_fun10_sec04;

   memErr_fun10_sec04:;
      scoreSL = 0;
      goto ret_fun10_sec04;

   ret_fun10_sec04:;
      if(startHeapArySL)
         free(startHeapArySL);
      startHeapArySL = 0;

      if(endHeapArySL)
         free(endHeapArySL);
      endHeapArySL = 0;

      if(scoreHeapArySL)
         free(scoreHeapArySL);
      scoreHeapArySL = 0;

      return scoreSL;
} /*insNtLineage_getLin*/

/*-------------------------------------------------------\
| Fun11: insAALineage_getLin
|   - checks if have an amino acid insertion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
\-------------------------------------------------------*/
signed char
insAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - checks if variant supports a insertion lineage
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - find start and end position of insertion
   '   o fun11 sec03:
   '     - get aa sequence and check if have match
   '   o fun11 sec04:
   '     - return the answer
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *aaSeqHeapStr = 0;
   signed int seqLenSI = 0;
   signed int refEndSI = 0;
   signed int endSI = 0;

   signed int alnCoordsSI[4] = {0, 0, 0, 0};

   signed int siCig = 0;
   signed int ntOnSI = 0;

   signed long scoreSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - find start and end position of insertion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto noLineageCoverage_fun11_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto noLineageCoverage_fun11_sec04;

   else if(linSTPtr->moleculeTypeSC != def_aaSeq_linST)
      goto wrongLineage_fun11_sec04;

   else if(linSTPtr->checkTypeSC != def_insType_linST)
      goto wrongLineage_fun11_sec04;

   if(linSTPtr->defGroupBl)
      goto defaultLineage_fun11_sec04;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   if(linSTPtr->startSI + 1 <= linSTPtr->endSI)
   { /*If: user is targeting a region*/
      if(samSTPtr->cigTypeStr[posArySI[2]] == 'I')
         ;
      else if(samSTPtr->cigTypeStr[posArySI[2] +1] != 'I')
         goto noIns_fun11_sec04;

      endSI = posArySI[0] + 1; /*move past insertion*/
   } /*If: user is targeting a region*/

   else
      endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - get aa sequence and check if have match
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   aaSeqHeapStr =
       malloc(((seqLenSI / 3) +8)  * sizeof(signed char));
   if(! aaSeqHeapStr)
      goto memErr_fun11_sec04;

   if(linSTPtr->revDirBl)
      seqLenSI =
         revSeqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );
   else
      seqLenSI =
         seqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find matching patterns*/
      scoreSL =
         simple_memwater(
            aaSeqHeapStr,
            seqLenSI,
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            alnCoordsSI
         );
      if(scoreSL < 0)
         goto memErr_fun11_sec04;

      if(scoreSL > linSTPtr->minScoreSI)
         goto foundMatch_fun11_sec04;
   } /*Loop: find matching patterns*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - return the answer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto noIns_fun11_sec04;

   foundMatch_fun11_sec04:;
      scoreSL = 1;
      goto ret_fun11_sec04;

   defaultLineage_fun11_sec04:;
      scoreSL = -1;
      goto ret_fun11_sec04;

   noIns_fun11_sec04:;
      scoreSL = -2;
      goto ret_fun11_sec04;

   noLineageCoverage_fun11_sec04:;
      scoreSL = -3;
      goto ret_fun11_sec04;

   wrongLineage_fun11_sec04:;
      scoreSL = -4;
      goto ret_fun11_sec04;

   memErr_fun11_sec04:;
      scoreSL = 0;
      goto ret_fun11_sec04;

   ret_fun11_sec04:;
      if(aaSeqHeapStr)
         free(aaSeqHeapStr);
      aaSeqHeapStr = 0;

      return (signed char) scoreSL;
} /*insAALineage_getLin*/

/*-------------------------------------------------------\
| Fun12: coordsToTrsLin_getLin
|   - converts alignments coordinates to a trs lineage
| Input:
|   - startArySL:
|     o signed long array with reference starting
|       coorindates
|   - endArySL:
|     o signed long array with reference ending
|       coorindates
|   - cntArySL:
|     o signed long array to use in keeping track of the
|       scores (this functions sets to 0 and then scores)
|   - coordLenSI:
|     o number of coordinates in startArySL and endArySL
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
| Output:
|   - Returns:
|     o trs lineage
|     o -1024 if no coordinates (unable to find lineage)
\-------------------------------------------------------*/
signed int
coordsToTrsLin_getLin(
   signed long *startArySL,   /*ref starting coordinates*/
   signed long *endArySL,     /*ref ending coordinates*/
   signed long *cntArySL,     /*ref ending coordinates*/
   signed int coordLenSI,     /*number of coordinates*/
   struct one_linST *linSTPtr /*lineage to check*/
){
   signed int siStart = 0;
   signed int siEnd = 0;
   signed int gapSI = 0;
   signed int trsLineageSI = 0;

   if(coordLenSI <= 0)
      goto noCoords_fun12;

   for(siStart = 0; siStart < coordLenSI; ++siStart)
      cntArySL[siStart] = 0; /*initialize*/

   /*worst case n^2, but often will be less*/
   for(siStart = 0; siStart < coordLenSI; ++siStart)
   { /*Loop: find longest alignment for any repeat*/
      for(siEnd = siStart +1; siEnd < coordLenSI; ++siEnd)
      { /*Loop: find longest length for siStart*/
         gapSI = startArySL[siEnd] - endArySL[siStart];

         if(gapSI - 1 > linSTPtr->trsGapSI)
            break; /*overshot limit*/

         else if(gapSI < 0)
            continue;

         else if(cntArySL[siEnd] > cntArySL[siStart])
            continue;
            /*previous repeat set as higher score, I need
            `  this check to avoid overlapping TRS's
            `  increasing the same next score, logic is
            `  that each TRS can only incurment the TRS
            `  ahead of it.
            `I do not need to save the path, so I only
            `  returnt the score
            */

        else
        { /*Else: can assign new/higher score*/
           cntArySL[siEnd] = cntArySL[siStart] + 1;

           if(cntArySL[siEnd] > trsLineageSI)
              trsLineageSI = cntArySL[siEnd];
        } /*Else: can assign new/higher score*/
      } /*Loop: find longest length for siStart*/
   } /*Loop: find longest alignment for any repeat*/

   return trsLineageSI - linSTPtr->trsLineageSI + 1;
      /*account for frist lineage sometimes being > 0;
      `  + 1 is to account for lineage being index 0
      */

   noCoords_fun12:;
      return -1024;
} /*coordsToTrsLin_getLin*/

/*-------------------------------------------------------\
| Fun13: trsNtLineage_getLin
|   - detect tandam repeat lineage in nucleotide sequences
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - detect tandam repeat lineage in a nucleotide
   '     sequence
   '   o fun13 sec01:
   '     - variable declarations
   '   o fun13 sec02:
   '     - initialize and find sequence positions
   '   o fun13 sec03:
   '     - get the alignments
   '   o fun13 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long scoreSL = 0;
   signed int trsLenSI = 0;

   unsigned long lastRefCoordAryUL[2];/*0=start; 1=end*/
   unsigned long lastQryCoordAryUL[2];/*0=start; 1=end*/
 
   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   struct aln_memwaterScan tmpAlnStackST;
   struct aln_memwaterScan mergeAlnStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_aln_memwaterScan(&tmpAlnStackST);
   init_aln_memwaterScan(&mergeAlnStackST);

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun13_sec05;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun13_sec05;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun13_sec05;

   else if(linSTPtr->checkTypeSC != def_trsType_linST)
      goto wrongLineage_fun13_sec05;


   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - get the alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];
   ++seqLenSI; /*convert to index 1*/

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find all tandum repeat alignments*/
      scoreSL =
         simple_memwaterScan(
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            &samSTPtr->seqStr[posArySI[1]],
            seqLenSI,
            &tmpAlnStackST
         );

      if(scoreSL < 0)
         goto memErr_fun13_sec05;

      filter_memwaterScan(
         &tmpAlnStackST,
         linSTPtr->minScoreSI,
         0
      ); /*remove nested alignments, doing here and after
         `  all alignments are merged to make sure all
         `  nested alignmetns are removed
         */

      if(
         merge_aln_memwaterScan(
            &tmpAlnStackST,
            &mergeAlnStackST,
            0 /*already filtered by min score*/
         )
      ) goto memErr_fun13_sec05;
   } /*Loop: find all tandum repeat alignments*/

   freeStack_aln_memwaterScan(&tmpAlnStackST);

   if(siCig > 1)
     seqLenSI = filter_memwaterScan(&mergeAlnStackST,0,0);
     /*using 0 for min scores since already filtered by
     `  this in the merge step
     */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec04:
   ^   - find the longest repeat
   ^   o fun13 sec04 sub01:
   ^     - check if have repeat and get first coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun13 Sec04 Sub01:
   *   - check if have repeat and get first coordinates
   \*****************************************************/

   if(! mergeAlnStackST.outLenSL)
   { /*If: not alignments (no repeat lineage)*/
      trsLenSI = 0;
      goto done_fun13_sec05;
   } /*If: not alignments (no repeat lineage)*/


   /*at this piont I no longer care about the query
   `  mapping coordinates since the alignment passed the
   `  minimum score, so just find the reference mapping
   `  coordinates.
   `I also do not care about the scores (all passed), so
   `  convert the scores into the repeat count
   */
   for(siCig=0; siCig < mergeAlnStackST.outLenSL; ++siCig)
   { /*Loop: find starting coordinates*/
      indexToCoord(
         mergeAlnStackST.refLenSI,
         mergeAlnStackST.startArySL[siCig],
         &lastRefCoordAryUL[0],
         &lastQryCoordAryUL[0]
      );

      indexToCoord(
         mergeAlnStackST.refLenSI,
         mergeAlnStackST.endArySL[siCig],
         &lastRefCoordAryUL[1],
         &lastQryCoordAryUL[1]
      );

      mergeAlnStackST.startArySL[siCig] =
         lastRefCoordAryUL[0];
      mergeAlnStackST.endArySL[siCig] =
         lastRefCoordAryUL[1];
   } /*Loop: find starting coordinates*/

   /*****************************************************\
   * Fun13 Sec04 Sub02:
   *   - find the longest lineage in the list
   \*****************************************************/

   trsLenSI =
      coordsToTrsLin_getLin(
         mergeAlnStackST.startArySL,
         mergeAlnStackST.endArySL,
         mergeAlnStackST.scoreArySL, /*for counts*/
         mergeAlnStackST.outLenSL,
         linSTPtr
      );

   if(trsLenSI == -1024)
      goto incompleteRange_fun13_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun13_sec05:;
      goto ret_fun13_sec05;

   incompleteRange_fun13_sec05:;
      trsLenSI = -1024;
      goto ret_fun13_sec05;

   wrongLineage_fun13_sec05:;
      scoreSL = -2048;
      goto ret_fun13_sec05;

   memErr_fun13_sec05:;
      trsLenSI = -4096;
      goto ret_fun13_sec05;

   ret_fun13_sec05:;
      freeStack_aln_memwaterScan(&tmpAlnStackST);
      freeStack_aln_memwaterScan(&mergeAlnStackST);
      return trsLenSI;
} /*trsNtLineage_getLin*/

/*-------------------------------------------------------\
| Fun14: trsNtFastLineage_getLin
|   - detect tandam repeat lineage in nucleotide sequences
|     using the faster kmer scan
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage type was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - detect tandam repeat lineage in a nucleotide
   '     sequence
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - initialize and find sequence positions
   '   o fun14 sec03:
   '     - get the alignments
   '   o fun14 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   /*variables for finding trs lineage*/
   signed int trsLenSI = 0;
   signed long *startHeapArySL = 0;
   signed long *endHeapArySL = 0;
   signed long *scoreHeapArySL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun14_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun14_sec04;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun14_sec04;

   else if(linSTPtr->checkTypeSC != def_trsFastType_linST)
      goto wrongLineage_fun14_sec04;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - get the alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];
   ++seqLenSI; /*convert to index 1*/

   seqLenSI =
      fastFindPatterns_getLin(
         tblSTPtr,
         linSTPtr,
         &samSTPtr->seqStr[posArySI[1]],
         seqLenSI,
         &startHeapArySL,
         &endHeapArySL,
         &scoreHeapArySL,
         &endSI           /*safe to discard this*/
      ); /*get the alignments*/

   if(seqLenSI < 0)
      goto memErr_fun14_sec04;
   else if(! seqLenSI)
   { /*Else If: no mappings*/
      trsLenSI = 0;
      goto done_fun14_sec04;
   } /*Else If: no mappings*/

   seqLenSI =
      filterCoords_getLin(
         linSTPtr->minScoreSI,
         startHeapArySL,
         endHeapArySL,
         scoreHeapArySL,
         seqLenSI
      ); /*remove overlapping alignments*/


   trsLenSI =
      coordsToTrsLin_getLin(
         startHeapArySL,
         endHeapArySL,
         scoreHeapArySL,
         seqLenSI,
         linSTPtr
      ); /*get the lineage*/

   if(trsLenSI == -1024)
      goto incompleteRange_fun14_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun14_sec04:;
      goto ret_fun14_sec04;

   incompleteRange_fun14_sec04:;
      trsLenSI = -1024;
      goto ret_fun14_sec04;

   wrongLineage_fun14_sec04:;
      trsLenSI = -2048;
      goto ret_fun14_sec04;

   memErr_fun14_sec04:;
      trsLenSI = -4096;
      goto ret_fun14_sec04;

   ret_fun14_sec04:;
      if(startHeapArySL)
         free(startHeapArySL);
      startHeapArySL = 0;

      if(endHeapArySL)
         free(endHeapArySL);
      endHeapArySL = 0;

      if(scoreHeapArySL)
         free(scoreHeapArySL);
      scoreHeapArySL = 0;

      return trsLenSI;
} /*trsNtFastLineage_getLin*/

/*-------------------------------------------------------\
| Fun15: trsAALineage_getLin
|   - detect tandam repeat lineage in amino acid sequences
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|     o AA conversion is done on the flye
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - detect tandam repeat lineage in an amino acid
   '     sequence
   '   o fun15 sec01:
   '     - variable declarations
   '   o fun15 sec02:
   '     - initialize and find sequence positions
   '   o fun15 sec03:
   '     - get the alignments
   '   o fun15 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long scoreSL = 0;

   unsigned long lastRefCoordAryUL[2];/*0=start; 1=end*/
   unsigned long lastQryCoordAryUL[2];/*0=start; 1=end*/
 
   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   signed int trsLenSI = 0;
   struct aln_memwaterScan tmpAlnStackST;
   struct aln_memwaterScan mergeAlnStackST;

   signed char *aaSeqHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_aln_memwaterScan(&tmpAlnStackST);
   init_aln_memwaterScan(&mergeAlnStackST);

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun15_sec05;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun15_sec05;

   else if(linSTPtr->moleculeTypeSC != def_aaSeq_linST)
      goto wrongLineage_fun15_sec05;

   else if(linSTPtr->checkTypeSC != def_trsType_linST)
      goto wrongLineage_fun15_sec05;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - get the alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*convert the sequence to amino acids*/
   aaSeqHeapStr =
       malloc(((seqLenSI / 3) +8)  * sizeof(signed char));
   if(! aaSeqHeapStr)
      goto memErr_fun15_sec05;

   if(linSTPtr->revDirBl)
      seqLenSI =
         revSeqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );
   else
      seqLenSI =
         seqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );

   /*get the aligments*/
   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find all tandum repeat alignments*/
      scoreSL =
         simple_memwaterScan(
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            aaSeqHeapStr,
            seqLenSI,
            &tmpAlnStackST
         );

      if(scoreSL < 0)
         goto memErr_fun15_sec05;

      filter_memwaterScan(
         &tmpAlnStackST,
         linSTPtr->minScoreSI,
         0
      ); /*remove nested alignments, doing here and after
         `  all alignments are merged to make sure all
         `  nested alignmetns are removed
         */

      if(
         merge_aln_memwaterScan(
            &tmpAlnStackST,
            &mergeAlnStackST,
            0 /*already filtered by min score*/
         )
      ) goto memErr_fun15_sec05;
   } /*Loop: find all tandum repeat alignments*/

   free(aaSeqHeapStr);
   aaSeqHeapStr = 0;
   freeStack_aln_memwaterScan(&tmpAlnStackST);

   if(siCig > 1)
     seqLenSI = filter_memwaterScan(&mergeAlnStackST,0,0);
     /*using 0 for min scores since already filtered by
     `  this in the merge step
     */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - find the longest repeat
   ^   o fun15 sec04 sub01:
   ^     - check if have repeat and get first coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec04 Sub01:
   *   - check if have repeat and get first coordinates
   \*****************************************************/

   if(! mergeAlnStackST.outLenSL)
   { /*If: not alignments (no repeat lineage)*/
      trsLenSI = 0;
      goto done_fun15_sec05;
   } /*If: not alignments (no repeat lineage)*/


   /*at this piont I no longer care about the query
   `  mapping coordinates since the alignment passed the
   `  minimum score, so just find the reference mapping
   `  coordinates.
   `I also do not care about the scores (all passed), so
   `  convert the scores into the repeat count
   */
   for(siCig=0; siCig < mergeAlnStackST.outLenSL; ++siCig)
   { /*Loop: find starting coordinates*/
      indexToCoord(
         mergeAlnStackST.refLenSI,
         mergeAlnStackST.startArySL[siCig],
         &lastRefCoordAryUL[0],
         &lastQryCoordAryUL[0]
      );

      indexToCoord(
         mergeAlnStackST.refLenSI,
         mergeAlnStackST.endArySL[siCig],
         &lastRefCoordAryUL[1],
         &lastQryCoordAryUL[1]
      );

      mergeAlnStackST.startArySL[siCig] =
         lastRefCoordAryUL[0];
      mergeAlnStackST.endArySL[siCig] =
         lastRefCoordAryUL[1];
   } /*Loop: find starting coordinates*/

   /*****************************************************\
   * Fun15 Sec04 Sub02:
   *   - find the longest lineage in the list
   \*****************************************************/


   trsLenSI =
      coordsToTrsLin_getLin(
         mergeAlnStackST.startArySL,
         mergeAlnStackST.endArySL,
         mergeAlnStackST.scoreArySL, /*for counts*/
         mergeAlnStackST.outLenSL,
         linSTPtr
      );

   if(trsLenSI == -1024)
      goto incompleteRange_fun15_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun15_sec05:;
      goto ret_fun15_sec05;

   incompleteRange_fun15_sec05:;
      trsLenSI = -1024;
      goto ret_fun15_sec05;

   wrongLineage_fun15_sec05:;
      scoreSL = -2048;
      goto ret_fun15_sec05;

   memErr_fun15_sec05:;
      trsLenSI = -4096;
      goto ret_fun15_sec05;

   ret_fun15_sec05:;
      if(aaSeqHeapStr)
         free(aaSeqHeapStr);
      aaSeqHeapStr = 0;

      freeStack_aln_memwaterScan(&tmpAlnStackST);
      freeStack_aln_memwaterScan(&mergeAlnStackST);
      return trsLenSI;
} /*trsAALineage_getLin*/

/*-------------------------------------------------------\
| Fun16: countNtLineage_getLin
|   - find lineages by counting number of times a pattern 
|     is duplicated
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun16 TOC:
   '   - find lineages by counting number of times a
   '     pattern is duplicated
   '   o fun16 sec01:
   '     - variable declarations
   '   o fun16 sec02:
   '     - initialize and find sequence positions
   '   o fun16 sec03:
   '     - get the alignments, filter, and get counts
   '   o fun16 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long scoreSL = 0;
   signed int trsLenSI = 0;

   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   struct aln_memwaterScan tmpAlnStackST;
   struct aln_memwaterScan mergeAlnStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_aln_memwaterScan(&tmpAlnStackST);
   init_aln_memwaterScan(&mergeAlnStackST);

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun16_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun16_sec04;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun16_sec04;

   else if(linSTPtr->checkTypeSC != def_countType_linST)
      goto wrongLineage_fun16_sec04;


   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec03:
   ^   - get the alignments, filter, and get counts
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];
   ++seqLenSI; /*convert to index 1*/

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find all tandum repeat alignments*/
      scoreSL =
         simple_memwaterScan(
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            &samSTPtr->seqStr[posArySI[1]],
            seqLenSI,
            &tmpAlnStackST
         );

      if(scoreSL < 0)
         goto memErr_fun16_sec04;

      trsLenSI =
         filter_memwaterScan(
            &tmpAlnStackST,
            linSTPtr->minScoreSI,
            0
         ); /*remove nested alignments, doing here and
            `  after all alignments are merged to make
            `  sure all nested alignmetns are removed
            */

      if(
         merge_aln_memwaterScan(
            &tmpAlnStackST,
            &mergeAlnStackST,
            0 /*already filtered by min score*/
         )
      ) goto memErr_fun16_sec04;
   } /*Loop: find all tandum repeat alignments*/

   freeStack_aln_memwaterScan(&tmpAlnStackST);

   if(siCig > 1)
     trsLenSI = filter_memwaterScan(&mergeAlnStackST,0,0);
     /*using 0 for min scores since already filtered by
     `  this in the merge step
     */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   trsLenSI -= linSTPtr->trsLineageSI;
      /*not all TRS lineages start at 0 (user errors)
      `  so I need to adjust the TRS count to the first
      `  lineage
      */
   goto ret_fun16_sec04;

   incompleteRange_fun16_sec04:;
      trsLenSI = -1024;
      goto ret_fun16_sec04;

   wrongLineage_fun16_sec04:;
      scoreSL = -2048;
      goto ret_fun16_sec04;

   memErr_fun16_sec04:;
      trsLenSI = -4096;
      goto ret_fun16_sec04;

   ret_fun16_sec04:;
      freeStack_aln_memwaterScan(&tmpAlnStackST);
      freeStack_aln_memwaterScan(&mergeAlnStackST);
      return trsLenSI;
} /*countNtLineage_getLin*/

/*-------------------------------------------------------\
| Fun17: countNtFastLineage_getLin
|   - detect count lineage in nucleotide sequences using
|     the faster kmer scan
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage type was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - detect count lineage in nucleotide sequences
   '     using the faster kmer scan
   '   o fun17 sec01:
   '     - variable declarations
   '   o fun17 sec02:
   '     - initialize and find sequence positions
   '   o fun17 sec03:
   '     - get the alignments
   '   o fun17 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   /*variables for finding trs lineage*/
   signed int trsLenSI = 0;
   signed long *startHeapArySL = 0;
   signed long *endHeapArySL = 0;
   signed long *scoreHeapArySL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun17_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun17_sec04;

   else if(linSTPtr->moleculeTypeSC != def_ntSeq_linST)
      goto wrongLineage_fun17_sec04;

   else if(
      linSTPtr->checkTypeSC != def_countFastType_linST
   ) goto wrongLineage_fun17_sec04;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - get the alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqLenSI -= posArySI[1];
   ++seqLenSI; /*convert to index 1*/

   seqLenSI =
      fastFindPatterns_getLin(
         tblSTPtr,
         linSTPtr,
         &samSTPtr->seqStr[posArySI[1]],
         seqLenSI,
         &startHeapArySL,
         &endHeapArySL,
         &scoreHeapArySL,
         &endSI           /*safe to discard this*/
      ); /*get the alignments*/

   if(seqLenSI < 0)
      goto memErr_fun17_sec04;
   else if(! seqLenSI)
   { /*Else If: no mappings*/
      trsLenSI = 0;
      goto done_fun17_sec04;
   } /*Else If: no mappings*/

   trsLenSI =
      filterCoords_getLin(
         linSTPtr->minScoreSI,
         startHeapArySL,
         endHeapArySL,
         scoreHeapArySL,
         seqLenSI
      ); /*remove overlapping alignments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun17_sec04:;
      trsLenSI -= linSTPtr->trsLineageSI;
         /*not all TRS lineages start at 0 (user errors)
         `  so I need to adjust the TRS count to the first
         `  lineage
         */
      goto ret_fun17_sec04;

   incompleteRange_fun17_sec04:;
      trsLenSI = -1024;
      goto ret_fun17_sec04;

   wrongLineage_fun17_sec04:;
      trsLenSI = -2048;
      goto ret_fun17_sec04;

   memErr_fun17_sec04:;
      trsLenSI = -4096;
      goto ret_fun17_sec04;

   ret_fun17_sec04:;
      if(startHeapArySL)
         free(startHeapArySL);
      startHeapArySL = 0;

      if(endHeapArySL)
         free(endHeapArySL);
      endHeapArySL = 0;

      if(scoreHeapArySL)
         free(scoreHeapArySL);
      scoreHeapArySL = 0;

      return trsLenSI;
} /*countNtFastLineage_getLin*/

/*-------------------------------------------------------\
| Fun18: countAALineage_getLin
|   - detects lineages based on number of times a pattern
|     is duplicated for an amino acid sequence
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|     o AA conversion is done on the flye
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - detects lineages based on number of times a
   '     pattern is duplicated for amino acid sequqnces
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec02:
   '     - initialize and find sequence positions
   '   o fun18 sec03:
   '     - get the alignments
   '   o fun18 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long scoreSL = 0;

   /*variables for finding position*/
   signed int siCig = 0;
   signed int ntOnSI = 0;
   signed int refEndSI = 0;
   signed int seqLenSI = 0;
   signed int endSI = 0;

   signed int trsLenSI = 0;
   struct aln_memwaterScan tmpAlnStackST;
   struct aln_memwaterScan mergeAlnStackST;

   signed char *aaSeqHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - initialize and find sequence positions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_aln_memwaterScan(&tmpAlnStackST);
   init_aln_memwaterScan(&mergeAlnStackST);

   if(samSTPtr->refEndUI < (unsigned int) linSTPtr->endSI)
      goto incompleteRange_fun18_sec04;

   else if(
        samSTPtr->refStartUI
      > (unsigned int) linSTPtr->startSI
   ) goto incompleteRange_fun18_sec04;

   else if(linSTPtr->moleculeTypeSC != def_aaSeq_linST)
      goto wrongLineage_fun18_sec04;

   else if(linSTPtr->checkTypeSC != def_countType_linST)
      goto wrongLineage_fun18_sec04;

   findRefPos_samEntry(
      samSTPtr,
      &posArySI[2],     /*cigar position*/
      &posArySI[3],     /*bases left at cigar position*/
      linSTPtr->startSI,/*were to end at*/
      &posArySI[0],     /*reference position*/
      &posArySI[1]      /*sequence position*/
   ); /*move to first base in lineage*/

   seqLenSI = posArySI[1];
   endSI = linSTPtr->endSI;

   siCig = posArySI[2];
   ntOnSI = posArySI[3];
   refEndSI = posArySI[0];

   findRefPos_samEntry(
      samSTPtr,
      &siCig,     /*cigar position*/
      &ntOnSI,    /*bases left at cigar position*/
      endSI,      /*were to end at*/
      &refEndSI,  /*reference position*/
      &seqLenSI   /*sequence position*/
   ); /*move to last base in lineage*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - get the alignments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*convert the sequence to amino acids*/
   aaSeqHeapStr =
       malloc(((seqLenSI / 3) +8)  * sizeof(signed char));
   if(! aaSeqHeapStr)
      goto memErr_fun18_sec04;

   if(linSTPtr->revDirBl)
      seqLenSI =
         revSeqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );
   else
      seqLenSI =
         seqToAA_codonFun(
            samSTPtr->seqStr,
            aaSeqHeapStr,
            posArySI[1],
            seqLenSI
         );

   /*get the aligments*/
   seqLenSI -= posArySI[1];
   ++seqLenSI; /*convert to index 1*/

   for(siCig = 0; siCig < linSTPtr->seqLenSI; ++siCig)
   { /*Loop: find all tandum repeat alignments*/
      scoreSL =
         simple_memwaterScan(
            linSTPtr->seqAryStr[siCig],
            linSTPtr->seqLenArySI[siCig],
            aaSeqHeapStr,
            seqLenSI,
            &tmpAlnStackST
         );

      if(scoreSL < 0)
         goto memErr_fun18_sec04;

      trsLenSI =
         filter_memwaterScan(
            &tmpAlnStackST,
            linSTPtr->minScoreSI,
            0
         ); /*remove nested alignments, doing here and
            `  after all alignments are merged to make
            `  sure all nested alignmetns are removed
            */

      if(
         merge_aln_memwaterScan(
            &tmpAlnStackST,
            &mergeAlnStackST,
            0 /*already filtered by min score*/
         )
      ) goto memErr_fun18_sec04;
   } /*Loop: find all tandum repeat alignments*/

   free(aaSeqHeapStr);
   aaSeqHeapStr = 0;
   freeStack_aln_memwaterScan(&tmpAlnStackST);

   if(siCig > 1)
     trsLenSI = filter_memwaterScan(&mergeAlnStackST,0,0);
     /*using 0 for min scores since already filtered by
     `  this in the merge step
     */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   trsLenSI -= linSTPtr->trsLineageSI;
      /*not all TRS lineages start at 0 (user errors)
      `  so I need to adjust the TRS count to the first
      `  lineage
      */
   goto ret_fun18_sec04;

   incompleteRange_fun18_sec04:;
      trsLenSI = -1024;
      goto ret_fun18_sec04;

   wrongLineage_fun18_sec04:;
      scoreSL = -2048;
      goto ret_fun18_sec04;

   memErr_fun18_sec04:;
      trsLenSI = -4096;
      goto ret_fun18_sec04;

   ret_fun18_sec04:;
      if(aaSeqHeapStr)
         free(aaSeqHeapStr);
      aaSeqHeapStr = 0;

      freeStack_aln_memwaterScan(&tmpAlnStackST);
      freeStack_aln_memwaterScan(&mergeAlnStackST);
      return trsLenSI;
} /*countAALineage_getLin*/

/*-------------------------------------------------------\
| Fun19: simpleLinage_getLin
|   - finds all one variant lineages in a sequence
| Input:
|   - samSTPtr:
|     o samEntry struct pointer with read to get lineages
|       for
|   - simpleSTPtr:
|     o simple_linST struct with lineages to search
|   - tblSTPtr:
|     o tblST_kmerFind struct to search with, it should
|       be setup with the `def_lenKmer_kmerFind` variable
|   - trsLinArySIPtr:
|     o signed int pionter to array to get the trs
|       lineages detected (otherwise, value set to -1024)
|   - outLenSIPtr:
|     o signed int pointer to get returned list length
| Output:
|   - Modifies:
|     o outLenSIPtr:
|       * have returned list length
|       * 0 if no errors
|       * -1 if memory error
|     o trsLinArySIPtr to have returned trs lineages
|       * freeded and then resized as needed
|       * freeded and set to 0 on memory error
|   - Returns:
|     o signed int array with the list of index's of the
|       detected lineages
|     o 0 for memory errors or no lineages
\-------------------------------------------------------*/
signed int *
simpleLineage_getLin(
   struct samEntry *samSTPtr,    /*sequence to search*/
   struct simple_linST *simpleSTPtr,/*lineages to search*/
   struct tblST_kmerFind *tblSTPtr, /*for fast searches*/
   signed int **trsLinArySIPtr,  /*gets trs lineages*/
   signed int *outLenSIPtr       /*number lineages found*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - finds all one variant lineages in a sequence
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - initialize and allocate memory
   '   o fun19 sec03:
   '     - find lineages
   '   o fun19 sec04:
   '     - assign default lineages to groups
   '   o fun19 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int errSI = 0;
   signed int siLin = 0;
   signed int groupSI = 0;
   signed int *swapSIPtr = 0;
   signed int *retLinArySI = 0;
   signed int retSizeSI = 0;
   signed int coordArySI[4] = {0, 0, 0, 0};

   struct one_linST *linSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - initilialze and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*make sure all group found lineages flags are set to
   `  -1's (allows default lineage assignment)
   */
   coordArySI[3] = samSTPtr->cigArySI[0];
   clearGroup_simple_linST(simpleSTPtr);

   if(*trsLinArySIPtr)
      free(*trsLinArySIPtr);
   *trsLinArySIPtr = 0;
   *outLenSIPtr = 0;

   retLinArySI = malloc(16 * sizeof(signed int));
   if(! retLinArySI)
      goto memErr_fun19_sec05;
   retSizeSI = 16;

   *trsLinArySIPtr = malloc(16 * sizeof(signed int));
   if(! *trsLinArySIPtr)
      goto memErr_fun19_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - find lineages
   ^   o fun19 sec03 sub01:
   ^     - start loop and resize arrays as needed
   ^   o fun19 sec03 sub02:
   ^     - switch start + no lineage type
   ^   o fun19 sec03 sub03:
   ^     - snp lineage detection
   ^   o fun19 sec03 sub04:
   ^     - deletion lineage detection
   ^   o fun19 sec03 sub05:
   ^     - insertion lineage detection
   ^   o fun19 sec03 sub06:
   ^     - insertion faster lineage detection
   ^   o fun19 sec03 sub07:
   ^     - trs lineage detection (slower)
   ^   o fun19 sec03 sub08:
   ^     - trs lineage detection (faster)
   ^   o fun19 sec03 sub09:
   ^     - count lineage detection (slower)
   ^   o fun19 sec03 sub10:
   ^     - count lineage detection (faster)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec03 Sub01:
   *   - start loop and resize arrays as needed
   \*****************************************************/

   siLin =
      posFindAry_one_linST(
         samSTPtr->refStartUI,
         simpleSTPtr->linAryST,
         simpleSTPtr->lenSI
      ); /*find the first possible variant*/

   if(siLin < 0)
      goto noLineages_fun19_sec05;

   for( ; siLin < simpleSTPtr->lenSI; ++siLin)
   { /*Loop: find lineages*/
      if(*outLenSIPtr >= retSizeSI)
      { /*If: need more memory*/
         retSizeSI += (retSizeSI >> 1);
         swapSIPtr =
            realloc(
               retLinArySI,
               retSizeSI * sizeof(signed int)
            );
         if(! swapSIPtr)
            goto memErr_fun19_sec05;
         retLinArySI = swapSIPtr;

         swapSIPtr =
            realloc(
               *trsLinArySIPtr,
               retSizeSI * sizeof(signed int)
            );
         if(! swapSIPtr)
            goto memErr_fun19_sec05;
         *trsLinArySIPtr = swapSIPtr;
      } /*If: need more memory*/

      /**************************************************\
      * Fun19 Sec03 Sub02:
      *   - switch start + no lineage type
      \**************************************************/

      linSTPtr = &simpleSTPtr->linAryST[siLin];
      groupSI = simpleSTPtr->groupArySI[siLin];

      switch(linSTPtr->checkTypeSC)
      { /*Switch: check the lineage type*/
         case def_noType_linST:
            errSI = 0; /*no lineage assigned*/
            break;

         /***********************************************\
         * Fun19 Sec03 Sub03:
         *   - snp lineage detection
         \***********************************************/

         case def_snpType_linST:
         /*Case: SNP lineage*/
            if(
              linSTPtr->moleculeTypeSC==def_unknown_linST
            ) errSI = -2;

            else if(
              linSTPtr->moleculeTypeSC == def_ntSeq_linST
            ){ /*Else If: nucleotide sequence*/
               errSI =
                  snpNtLineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: nucleotide sequence*/

            else
            { /*Else: amino acid sequence*/
               errSI =
                  snpAALineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: amino acid sequence*/

            if(! errSI)
               goto memErr_fun19_sec05;

            else if(errSI > 0)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = -1024;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
            } /*Else If: found the lineage*/

            else if(
               simpleSTPtr->foundGroupAryBl[groupSI] >= 0
            ) ; /*already marked group as in read*/

            else if(errSI == -1)
               simpleSTPtr->foundGroupAryBl[groupSI] = 0;
               /*mark group; in read, but no assignment*/

            break;
         /*Case: SNP lineage*/

         /***********************************************\
         * Fun19 Sec03 Sub04:
         *   - deletion lineage detection
         \***********************************************/

         case def_delType_linST:
         /*Case: deletion lineage*/
            errSI =
               delLineage_getLin(
                  linSTPtr,
                  samSTPtr,
                  coordArySI
               );

            if(! errSI)
               goto memErr_fun19_sec05;

            else if(errSI > 0)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = -1024;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
            } /*Else If: found the lineage*/

            else if(
               simpleSTPtr->foundGroupAryBl[groupSI] >=0
            ) ; /*already marked group as in read*/
            else if(errSI == -1)
               simpleSTPtr->foundGroupAryBl[groupSI] = 0;
               /*mark group; in read, but no assignment*/

            break;
         /*Case: deletion lineage*/

         /***********************************************\
         * Fun19 Sec03 Sub05:
         *   - insertion lineage detection
         \***********************************************/

         case def_insType_linST:
         /*Case: insertion lineage (slower)*/
            if(
              linSTPtr->moleculeTypeSC==def_unknown_linST
            ) errSI = -2;

            else if(
               linSTPtr->moleculeTypeSC == def_ntSeq_linST
            ){ /*Else If: nucleotide sequence*/
               errSI =
                  insNtLineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: nucleotide sequence*/

            else
            { /*Else: amino acid sequence*/
               errSI =
                  insAALineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: amino acid sequence*/

            if(! errSI)
               goto memErr_fun19_sec05;

            else if(errSI > 0)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = -1024;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
            } /*Else If: found the lineage*/

            else if(
               simpleSTPtr->foundGroupAryBl[groupSI] >=0
            ) ; /*already marked group as in read*/
            else if(errSI == -1)
               simpleSTPtr->foundGroupAryBl[groupSI] = 0;
               /*mark group; in read, but no assignment*/

            break;
         /*Case: insertion lineage (slower)*/

         /***********************************************\
         * Fun19 Sec03 Sub06:
         *   - insertion faster lineage detection
         \***********************************************/

         case def_insFastType_linST:
         /*Case: detection insertion by kmerFind (fast)*/
            errSI =
               insNtFastLineage_getLin(
                  linSTPtr,
                  samSTPtr,
                  tblSTPtr,
                  coordArySI
               );

            if(! errSI)
               goto memErr_fun19_sec05;

            else if(errSI > 0)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = -1024;
               (*outLenSIPtr)++;

               simpleSTPtr->foundGroupAryBl[groupSI] = 1;
            } /*Else If: found the lineage*/

            else if(
               simpleSTPtr->foundGroupAryBl[groupSI] >=0
            ) ; /*already marked group as in read*/

            else if(errSI == -1)
               simpleSTPtr->foundGroupAryBl[groupSI] = 0;
               /*mark group; in read, but no assignment*/

            break;
         /*Case: detection insertion by kmerFind (fast)*/

         /***********************************************\
         * Fun19 Sec03 Sub07:
         *   - trs lineage detection (slower)
         \***********************************************/

         case def_trsType_linST:
         /*Case: trs lineage (slower method)*/
            if(
              linSTPtr->moleculeTypeSC==def_unknown_linST
            ) errSI = -2;

            else if(
               linSTPtr->moleculeTypeSC == def_ntSeq_linST
            ){ /*Else If: nucleotide sequence*/
               errSI =
                  trsNtLineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: nucleotide sequence*/

            else
            { /*Else: amino acid sequence*/
               errSI =
                  trsAALineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: amino acid sequence*/

            if(errSI == -4096)
               goto memErr_fun19_sec05;

            else if(errSI > -1024)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = errSI;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
                  /*this system always returns a lineage
                  `   or the regions is incomplete
                  */
            } /*Else If: found the lineage*/

            break;
         /*Case: trs lineage (slower method)*/

         /***********************************************\
         * Fun19 Sec03 Sub08:
         *   - trs lineage detection (faster)
         \***********************************************/

         case def_trsFastType_linST:
         /*Case: trs lineage (faster method)*/
            errSI =
               trsNtFastLineage_getLin(
                  linSTPtr,
                  samSTPtr,
                  tblSTPtr,
                  coordArySI
               );

            if(errSI == -4096)
               goto memErr_fun19_sec05;

            else if(errSI > -1024)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = errSI;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
                  /*this system always returns a lineage
                  `   or the regions is incomplete
                  */
            } /*Else If: found the lineage*/

            break;
         /*Case: trs lineage (faster method)*/

         /***********************************************\
         * Fun19 Sec03 Sub09:
         *   - count lineage detection (slower)
         \***********************************************/

         case def_countType_linST:
         /*Case: count lineage (slower method)*/
            if(
              linSTPtr->moleculeTypeSC==def_unknown_linST
            ) errSI = -2;

            else if(
               linSTPtr->moleculeTypeSC == def_ntSeq_linST
            ){ /*Else If: nucleotide sequence*/
               errSI =
                  countNtLineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: nucleotide sequence*/

            else
            { /*Else: amino acid sequence*/
               errSI =
                  countAALineage_getLin(
                     linSTPtr,
                     samSTPtr,
                     coordArySI
                  );
            }  /*Else If: amino acid sequence*/

            if(errSI == -4096)
               goto memErr_fun19_sec05;

            else if(errSI > -1024)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = errSI;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
                  /*this system always returns a lineage
                  `   or the regions is incomplete
                  */
            } /*Else If: found the lineage*/

            break;
         /*Case: trs lineage (slower method)*/

         /***********************************************\
         * Fun19 Sec03 Sub10:
         *   - count lineage detection (faster)
         \***********************************************/

         case def_countFastType_linST:
         /*Case: count lineage (faster method)*/
            errSI =
               countNtFastLineage_getLin(
                  linSTPtr,
                  samSTPtr,
                  tblSTPtr,
                  coordArySI
               );

            if(errSI == -4096)
               goto memErr_fun19_sec05;

            else if(errSI > -1024)
            { /*Else If: found the lineage*/
               retLinArySI[*outLenSIPtr] = siLin;
               (*trsLinArySIPtr)[*outLenSIPtr] = errSI;
               (*outLenSIPtr)++;

               if(linSTPtr->printLinBl)
                  simpleSTPtr->foundGroupAryBl[groupSI]=1;
                  /*this system always returns a lineage
                  `   or the regions is incomplete
                  */
            } /*Else If: found the lineage*/

            break;
         /*Case: count lineage (faster method)*/
      } /*Switch: check the lineage type*/
   } /*Loop: find lineages*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - assign default lineages to groups
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(siLin=0; siLin < simpleSTPtr->groupLenSI; ++siLin)
   { /*Loop: check for groups I can assign as default*/
      if(*outLenSIPtr >= retSizeSI)
      { /*If: need more memory*/
         retSizeSI += (retSizeSI >> 1);
         swapSIPtr =
            realloc(
               retLinArySI,
               retSizeSI * sizeof(signed int)
            );
         if(! swapSIPtr)
            goto memErr_fun19_sec05;
         retLinArySI = swapSIPtr;

         swapSIPtr =
            realloc(
               *trsLinArySIPtr,
               retSizeSI * sizeof(signed int)
            );
         if(! swapSIPtr)
            goto memErr_fun19_sec05;
         *trsLinArySIPtr = swapSIPtr;
      } /*If: need more memory*/

      if(simpleSTPtr->foundGroupAryBl[siLin])
         continue; /*no default or assigned non-default*/

      else
      { /*Else: no other linage was found; do default*/
         retLinArySI[*outLenSIPtr] =
            simpleSTPtr->defGroupArySI[siLin];
         (*trsLinArySIPtr)[*outLenSIPtr] = -1024;
         (*outLenSIPtr)++;
      } /*Else: no other linage was found; do default*/
   } /*Loop: check for groups I can assign as default*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *outLenSIPtr)
      goto noLineages_fun19_sec05; /*no lineages found*/

   goto ret_fun19_sec05;

   memErr_fun19_sec05:;
      *outLenSIPtr = -1;
      goto errClean_fun19_sec05;

   noLineages_fun19_sec05:;
      *outLenSIPtr = 0;
      goto errClean_fun19_sec05;

   errClean_fun19_sec05:;
      if(retLinArySI)
         free(retLinArySI);
      retLinArySI = 0;

      if(*trsLinArySIPtr)
         free(*trsLinArySIPtr);
      *trsLinArySIPtr = 0;

      goto ret_fun19_sec05;

   ret_fun19_sec05:;
      return retLinArySI;
} /*simpleLinage_getLin*/

/*-------------------------------------------------------\
| Fun20: complexLineage_getLin
|   - detect simple lineages and returns the index of the
|     detected complex lineages
|   - mLinAryST (multi_linST array) must not be sorted
|     or changed from getComplexLineages_multi_linST
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer with the complex
|       lineages to check
|     o should be in format of user input (do not sort)
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       lineages (allows narrowing down of complex
|       lineages needed to be searched)
|   - simpleLinArySI:
|     o signed int array that has the index of the
|       lineages from simpleLinage_getLin; returned array
|     o this is assumed to be sorted by index (value),
|       which is the default from simpleLinage_getLin
|   - trsLinArySI:
|     o signed int array that has the detected trs lineage
|     o this is input five of simpleLinage_getLin
|   - simpleLenSIPtr:
|     o signed int pointer with the number of lineages in
|       simpLinArySI and trsLinArySI
|     o this is intput six of simpleLinage_getLin
|   - outLenSIPtr:
|     o signed int pionter to get the length of the
|       returned signed int array
| Output:
|   - Modifies:
|     o simpLinArySI and trsLinArySI to have the index of
|       the simple lineages that the complex lineages did
|       not replace (so, removes lineages as needed)
|     o simpleLenSIPtr to have the new length of
|       simpLineArySI and trsLinArySI (same lengths)
|     o outLenSIPtr
|       * have the returned arrays length
|       * 0 if no complex lineages were found
|       * -1 if no simple lineages were found
|       * -2 for memory errors
|   - Returns
|     o signed int array with the index's of the detected
|       complex lineages
|     o 0 for no lineages or errrors
\-------------------------------------------------------*/
signed int *
complexLineage_getLin(
   struct complex_linST *complexSTPtr,/*complex lineages*/
   struct simple_linST *simpleSTPtr,  /*simple lineages*/
   signed int *simpleLinArySI, /*found simple lineages*/
   signed int *trsLinArySI,    /*found TRS lineages*/
   signed int *simpleLenSIPtr, /*# found simple lineages*/
   signed int *outLenSIPtr     /*returned array length*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - detect simple lineages and returns the index of
   '     the detected complex lineages
   '   o fun20 sec01:
   '     - variable declarations
   '   o fun20 sec02:
   '     - initialze and memory alloaction
   '   o fun20 sec03:
   '     - find lineages
   '   o fun20 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int *retMLinArySI = 0;
   signed int siMLin = 0;
   signed int siLin = 0;
   signed int firstLinSI = 0;

   signed int indexSI = 0;
   signed int missSI = 0;

   signed int *histHeapArySI = 0;
      /*keeps track of lineages hit so I can clear later*/

   struct multi_linST *mLinSTPtr = 0;
      /*for my own sanity later on*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec02:
   ^   - initialze and memory alloaction
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *simpleLenSIPtr)
      goto noSimpleLinErr_fun20_sec04;

   *outLenSIPtr = 0;

   retMLinArySI = malloc(16 * sizeof(signed int));
   if(! retMLinArySI)
      goto memErr_fun20_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec03:
   ^   - find lineages
   ^   o fun20 sec03 sub01:
   ^     - start lineage loop + initialize and resize
   ^   o fun20 sec03 sub02:
   ^     - scan simple lineages for hits and misses
   ^   o fun20 sec03 sub03:
   ^     - scan complex lineages for hits and misses
   ^   o fun20 sec03 sub04:
   ^     - add found complex lineage to the hits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun20 Sec03 Sub01:
   *   - start lineage loop + initialize and resize
   \*****************************************************/

   if(simpleLinArySI && *simpleLenSIPtr)
      firstLinSI = 
         posFind_complex_linST(
           simpleSTPtr->linAryST[simpleLinArySI[0]].endSI,
           complexSTPtr
         ); /*find the first possible complex lineage
            `  given the simple lineages. This array is
            `  sorted by index, which in turn is sorted
            `  by the starting position and then ending
            `  position. The complex array is sorted by
            `  the ending position.
            */
   else
      goto noSimpleLinErr_fun20_sec04;

   if(firstLinSI < 0)
      goto noSimpleLinErr_fun20_sec04;

   for(
      siMLin = firstLinSI;
      siMLin < complexSTPtr->lenSI;
      ++siMLin
   ){ /*Loop: find complex lineages*/
      complexSTPtr->cntArySI[siMLin] = 0;
      missSI = 0;
      mLinSTPtr = &complexSTPtr->linAryST[siMLin];
         /*for sanity*/

      /**************************************************\
      * Fun20 Sec03 Sub02:
      *   - scan simple lineages for hits and misses
      \**************************************************/

      for(siLin = 0; siLin < mLinSTPtr->linLenSI; ++siLin)
      { /*Loop: find simple lineage matches*/
         if(missSI > mLinSTPtr->fudgeSI)
            break; /*not a valid lineage*/

         indexSI = mLinSTPtr->linIndexArySI[siLin];

         indexSI =
            siSearch_shellSort(
               simpleLinArySI,
               indexSI,
               *simpleLenSIPtr
            ); /*see if have a match*/

         if(indexSI < 0)
         { /*If: lineage was not found*/
            if(mLinSTPtr->linNotAryBl[siLin])
               ;
            else if(mLinSTPtr->linNeedAryBl[siLin])
               missSI = mLinSTPtr->fudgeSI + 1;
               /*this was a required lineage*/
            else
               ++missSI;
         } /*If: lineage was not found*/

         else if(
               mLinSTPtr->linTrsArySI[siLin] > -1024
            &&    mLinSTPtr->linTrsArySI[siLin]
               != trsLinArySI[indexSI]
         ){ /*Else If: the lineage was not found*/
            if(mLinSTPtr->linNotAryBl[siLin])
               ;
            else if(mLinSTPtr->linNeedAryBl[siLin])
               missSI = mLinSTPtr->fudgeSI + 1;
               /*this was a required lineage*/
            else
               ++missSI;
         }  /*Else If: the lineage was not found*/

         else if(mLinSTPtr->linNotAryBl[siLin])
         { /*Else If: this lineage should be ignored*/
            if(mLinSTPtr->linNeedAryBl[siLin])
               missSI = mLinSTPtr->fudgeSI + 1;
               /*this lineage must not be present ever*/
            else
               ++missSI; /*lineage counts as one miss*/
         } /*Else If: this lineage should be ignored*/

         else
            ++complexSTPtr->cntArySI[siMLin];
      } /*Loop: find simple lineage matches*/

      if(missSI > mLinSTPtr->fudgeSI)
      { /*If: this lineage is not a valid lineage*/
         complexSTPtr->cntArySI[siMLin] = 0;
         continue;
      } /*If: this lineage is not a valid lineage*/

      /**************************************************\
      * Fun20 Sec03 Sub03:
      *   - scan complex lineages for hits and misses
      \**************************************************/

      for(siLin=0; siLin < mLinSTPtr->mLinLenSI; ++siLin)
      { /*Loop: find complex lineage matches*/
         if(missSI > mLinSTPtr->fudgeSI)
            break;

         indexSI =
            siSearch_shellSort(
               retMLinArySI,
               mLinSTPtr->mLinIndexArySI[siLin],
               *outLenSIPtr
            ); /*see if have the complex lineage*/

         if(indexSI < 0)
         { /*If: did not find the lineage*/
            ++missSI;

            if(mLinSTPtr->mLinNeedAryBl[siLin])
               missSI = mLinSTPtr->fudgeSI + 1;
               /*this was a required lineage*/
         } /*If: did not find the lineage*/

         else
            ++complexSTPtr->cntArySI[siMLin];
      } /*Loop: find complex lineage matches*/

      if(missSI > mLinSTPtr->fudgeSI)
      { /*If: this lineage is not a valid lineage*/
         complexSTPtr->cntArySI[siMLin] = 0;
         continue;
      } /*If: this lineage is not a valid lineage*/

      /**************************************************\
      * Fun20 Sec03 Sub04:
      *   - add found complex lineage to the hits
      \**************************************************/

      retMLinArySI[*outLenSIPtr] = siMLin;
      ++(*outLenSIPtr);
   } /*Loop: find complex lineages*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - for closest lineages, find the best lineage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      siMLin = 0;
      siMLin < *outLenSIPtr;
      ++siMLin
   ){ /*Loop: find the best complex lineages*/
      indexSI = retMLinArySI[siMLin];

      if(complexSTPtr->cntArySI[indexSI] == -2)
         continue;/*already checked this lineage/species*/

      mLinSTPtr = &complexSTPtr->linAryST[indexSI];
         /*for sanity*/

      if(mLinSTPtr->typeSC !=def_closestComplexType_linST)
         continue; /*this is not a count lineage*/
      else if(complexSTPtr->cntArySI[indexSI] < 0)
         continue;
         /*this count lineage has been eliminated*/

      missSI = complexSTPtr->cntArySI[indexSI];
      firstLinSI = indexSI;
      siLin = complexSTPtr->groupArySI[indexSI];

      while(siLin != firstLinSI)
      { /*Loop: find the closest lineage*/
         if(complexSTPtr->cntArySI[siLin] > missSI)
         { /*If: found a new best lineage*/
            complexSTPtr->cntArySI[indexSI] = -2;

            missSI = complexSTPtr->cntArySI[siLin];
            indexSI = siLin;
         } /*If: found a new best lineage*/

         else
            complexSTPtr->cntArySI[siLin] = -2;
            /*mark lineage for removal*/

         siLin = complexSTPtr->groupArySI[siLin];
      }  /*Loop: find the closest lineage*/
   } /*Loop: find the best complex lineages*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec05:
   ^   - mark and then remove overwrite lineages
   ^   o fun20 sec05 sub01:
   ^     - mark lineages for removal
   ^   o fun20 sec05 sub02:
   ^     - remove lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun20 Sec05 Sub01:
   *   - mark lineages for removal
   *   o fun20 sec05 sub01 cat01:
   *     - setup for marking lineages
   *   o fun20 sec05 sub01 cat02:
   *     - start loop + check if can remove any lineages
   *   o fun20 sec05 sub01 cat03:
   *     - mark simple lineages to remove
   *   o fun20 sec05 sub01 cat04:
   *     - mark complex lineages to remove
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun20 Sec05 Sub01 Cat01:
   +   - setup for marking lineages
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   histHeapArySI =
      calloc(*simpleLenSIPtr, sizeof(signed int));
   if(! histHeapArySI)
      goto memErr_fun20_sec04;

   for(siLin = 0; siLin < *simpleLenSIPtr; ++siLin)
      histHeapArySI[siLin] = 0;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun20 Sec05 Sub01 Cat02:
   +   - start loop + check if can remove any lineages
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   for(siMLin = 0; siMLin < *outLenSIPtr; ++siMLin)
   { /*Loop: find the best complex lineages*/
       mLinSTPtr = &complexSTPtr->linAryST[siMLin];

       if(! mLinSTPtr->overwriteBl)
          continue;
       else if(complexSTPtr->cntArySI[siMLin] == -2)
          continue;
          /*this lineage was closest lineage that was not
          `  used or has already been checked
          */

      if(mLinSTPtr->overwriteBl & 2)
      { /*If: overwriting the default groups*/
         for(
            siLin = 0;
            siLin < mLinSTPtr->defGroupLenSI;
            ++siLin
         ){ /*Loop: add the default groups to history*/
            indexSI =
               siSearch_shellSort(
                  simpleLinArySI,
                  mLinSTPtr->defGroupArySI[siLin],
                  *simpleLenSIPtr
               ); /*see if have a match*/

            if(indexSI >= 0)
               histHeapArySI[indexSI] = -1;
         }  /*Loop: add the default groups to history*/

         if( ! (mLinSTPtr->overwriteBl & 1) )
            continue; /*only removing default groups*/
      } /*If: overwriting the default groups*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun20 Sec05 Sub01 Cat03:
      +   - mark simple lineages to remove
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

       for(siLin=0; siLin < mLinSTPtr->linLenSI; ++siLin)
       { /*Loop: mark simple lineages as hit*/
          indexSI =
             siSearch_shellSort(
                simpleLinArySI,
                mLinSTPtr->linIndexArySI[siLin],
                *simpleLenSIPtr
             ); /*see if have the simple lineage*/
          
          if(indexSI >= 0)
             histHeapArySI[indexSI] = -1;
       } /*Loop: mark simple lineages as hit*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun20 Sec05 Sub01 Cat04:
      +   - mark complex lineages to remove
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

       for(siLin=0; siLin < mLinSTPtr->mLinLenSI; ++siLin)
       { /*Loop: mark complex lineages as hit*/
          complexSTPtr->cntArySI[
             mLinSTPtr->mLinIndexArySI[siLin]
          ] = -1;
       } /*Loop: mark complex lineages as hit*/

      if(complexSTPtr->cntArySI[siMLin] < 0)
         complexSTPtr->cntArySI[siMLin] = -2;
         /*aleady marked for removal. At this piont I have
         `  marked all its invidiual lineages for removal
         */
   } /*Loop: find the best complex lineages*/

   /*****************************************************\
   * Fun20 Sec05 Sub02:
   *   - remove lineages
   \*****************************************************/

   siLin = 0;
   indexSI = 0;

   while(siLin < *simpleLenSIPtr)
   { /*Loop: remove simple lineages*/
       if(histHeapArySI[siLin] >= 0)
       { /*If: not removing the lineage*/
          trsLinArySI[indexSI] = trsLinArySI[siLin];
          simpleLinArySI[indexSI] = simpleLinArySI[siLin];
          ++indexSI;
       } /*If: not removing the lineage*/

       ++siLin; /*move to next lineage to keep or remove*/
   } /*Loop: remove simple lineages*/

   *simpleLenSIPtr = indexSI;


   siLin = 0;
   indexSI = 0;

   while(siLin < *outLenSIPtr)
   { /*Loop: remove complex lineages*/
       missSI = retMLinArySI[siLin];

       if(complexSTPtr->cntArySI[missSI] >= 0)
       { /*If: not removing the lineage*/
          retMLinArySI[indexSI] = retMLinArySI[siLin];
          ++indexSI;
       } /*If: not removing the lineage*/

       ++siLin; /*move to next lineage to keep or remove*/
   } /*Loop: remove complex lineages*/

   *outLenSIPtr = indexSI;
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun20_sec04;

   noSimpleLinErr_fun20_sec04:;
      *outLenSIPtr = -1;
      goto errClean_fun20_sec04;

   memErr_fun20_sec04:;
      *outLenSIPtr = -2;
      goto errClean_fun20_sec04;

   errClean_fun20_sec04:;
      if(retMLinArySI)
        free(retMLinArySI);
      retMLinArySI = 0;

      goto ret_fun20_sec04;

   ret_fun20_sec04:;
      if(histHeapArySI)
         free(histHeapArySI);
      histHeapArySI = 0;

      return retMLinArySI;
} /*complexLineage_getLin*/

/*-------------------------------------------------------\
| Fun21: blank_cnt_getLin
|   - blanks values in a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to blank
| Output:
|   - Modifies:
|     o lenSI and numReadsSL to be 0 (lazy blank)
\-------------------------------------------------------*/
void
blank_cnt_getLin(
   struct cnt_getLin *cntSTPtr
){
   if(! cntSTPtr)
      return;
   cntSTPtr->lenSI = 0;
   cntSTPtr->numReadsSL = 0;
} /*blank_cnt_getLin*/

/*-------------------------------------------------------\
| Fun22: init_cnt_getLin
|   - initializes all values in a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to initialze
| Output:
|   - Modifies:
|     o all values (including pointers) to be 0
\-------------------------------------------------------*/
void
init_cnt_getLin(
   struct cnt_getLin *cntSTPtr
){
   if(! cntSTPtr)
      return;

   cntSTPtr->linStrAry = 0;
   cntSTPtr->idSizeArySI = 0;
   cntSTPtr->linCntArySI = 0;
   cntSTPtr->sizeSI = 0;

   blank_cnt_getLin(cntSTPtr);
} /*init_cnt_getLin*/

/*-------------------------------------------------------\
| Fun23: freeStack_cnt_getLin
|   - frees arrays and initialzes a cnt_getLine struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter with variables to free
| Output:
|   - Modifies:
|     o frees all arrays and initializes
\-------------------------------------------------------*/
void
freeStack_cnt_getLin(
   struct cnt_getLin *cntSTPtr
){
   signed int siPos = 0;

   if(! cntSTPtr)
      return;

   if(cntSTPtr->idSizeArySI)
      free(cntSTPtr->idSizeArySI);
   if(cntSTPtr->linCntArySI)
      free(cntSTPtr->linCntArySI);

   if(cntSTPtr->linStrAry)
   { /*Loop: free c-string pointers*/
      for(siPos = 0; siPos < cntSTPtr->sizeSI; ++siPos)
      { /*Loop: free individual c-string pointers*/
         if(cntSTPtr->linStrAry[siPos])
            free(cntSTPtr->linStrAry[siPos]);
         cntSTPtr->linStrAry[siPos] = 0;
      } /*Loop: free individual c-string pointers*/

      free(cntSTPtr->linStrAry);
   } /*Loop: free c-string pointers*/

   init_cnt_getLin(cntSTPtr);
} /*freeStack_cnt_getLin*/

/*-------------------------------------------------------\
| Fun24: freeHeap_cnt_getLin
|   - frees a cnt_getLine struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to free
| Output:
|   - Modifies:
|     o frees cntSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_cnt_getLin(
   struct cnt_getLin *cntSTPtr
){
   if(! cntSTPtr)
      return;

   freeStack_cnt_getLin(cntSTPtr);
   free(cntSTPtr);
} /*freeStack_cnt_getLin*/

/*-------------------------------------------------------\
| Fun25: addMem_cnt_getLin
|   - add memory (or if memory rellocate) to a cnt_getLin
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to add memory to
|   - lenSI:
|     o new size of arrays (must be >= sizeSI in cntSTPtr)
| Output:
|   - Modifies:
|     o uses malloc to allocate memory to arrays in
|       cntSTPtr if the arrays in cntSTPtr are set to
|       0/null
|     o uses realloc to add memory to arrays in cntSTPtr
|       if the arrays have something
|     o sizeSI in cntStPtr is set to lenSI; if an increase
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors (you must clean up)
\-------------------------------------------------------*/
signed char
addMem_cnt_getLin(
   struct cnt_getLin *cntSTPtr,
   signed int lenSI
){
   signed char **swapStrPtr = 0;
   signed int *swapSI = 0;
   signed int siPos = 0;

   if(! cntSTPtr)
      goto noErr_fun25;
   if(cntSTPtr->linStrAry && cntSTPtr->sizeSI > lenSI)
      goto noErr_fun25; /*arrays already have that much*/

   if(! cntSTPtr->linStrAry)
   { /*If: first allcation*/
      cntSTPtr->linStrAry =
         calloc(lenSI, sizeof(signed char *));
      if(! cntSTPtr->linStrAry)
         goto err_fun25;

      cntSTPtr->idSizeArySI =
         calloc(lenSI, sizeof(signed int));
      if(! cntSTPtr->idSizeArySI)
         goto err_fun25;

      cntSTPtr->linCntArySI =
         calloc(lenSI, sizeof(signed int));
      if(! cntSTPtr->linCntArySI)
         goto err_fun25;
   } /*If: first allcation*/

   else
   { /*Else: expanding memory*/
      swapStrPtr =
         realloc(
            cntSTPtr->linStrAry,
            lenSI * sizeof(signed char *)
         );
      if(! swapStrPtr)
         goto err_fun25;
      cntSTPtr->linStrAry = swapStrPtr;

      swapSI =
         realloc(
            cntSTPtr->idSizeArySI,
            lenSI * sizeof(signed char *)
         );
      if(! swapSI)
         goto err_fun25;
      cntSTPtr->idSizeArySI = swapSI;

      swapSI =
         realloc(
            cntSTPtr->linCntArySI,
            lenSI * sizeof(signed char *)
         );
      if(! swapSI)
         goto err_fun25;
      cntSTPtr->linCntArySI = swapSI;

      for(siPos = cntSTPtr->lenSI; siPos < lenSI; ++siPos)
      { /*Loop: initialize memory*/
         cntSTPtr->linStrAry[siPos] = 0;
         cntSTPtr->idSizeArySI[siPos] = 0;
         cntSTPtr->linCntArySI[siPos] = 0;
      } /*Loop: initialize memory*/
   } /*Else: expanding memory*/

   cntSTPtr->sizeSI = lenSI;

   noErr_fun25:;
      return 0;
   err_fun25:;
      return 1;
} /*addMem_cnt_getLin*/

/*-------------------------------------------------------\
| Fun26: addLineage_cnt_getLin
|   - adds a lineage to a cnt_getLin structure
| Input:
|   - linStr:
|     o c-string with the name of the lineage to add
|   - cntSTPtr:
|     o cnt_getLin struct pionter to add the lineage to
| Output:
|   - Modifies:
|     o linStrAry in cntSTPtr to have linStr if the
|       lineage it is not already in it
|     o idSizeArySI in cntSTPtr if needed to add more
|       memory to a string pionter in linStrAry
|     o linCntArySI in cntSTPtr to have the lineage added
|       if the arrays have something
|     o lenSI in cntSTPtr to have the new lineage (if new)
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors (you must clean up)
\-------------------------------------------------------*/
signed char
addLineage_cnt_getLin(
   signed char *linStr,
   struct cnt_getLin *cntSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun26 TOC:
   '   - adds a lineage to a cnt_getLin structure
   '   o fun26 sec01:
   '     - variable declarations
   '   o fun26 sec02:
   '     - check if need more memory
   '   o fun26 sec03:
   '     - find lineage or insert position
   '   o fun26 sec04:
   '     - increase lineage count or if new add lineage
   '   o fun26 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0;
   signed int leftSI = 0;
   signed int rightSI = 0;
   signed int midSI = 0;

   signed char *swapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec02:
   ^   - check if need more memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(cntSTPtr->lenSI >= cntSTPtr->sizeSI)
   { /*If: need more memory*/
      tmpSI = cntSTPtr->sizeSI;

      if(tmpSI <= 0)
         tmpSI = 16;
      else
         tmpSI += (tmpSI >> 1);

      if( addMem_cnt_getLin(cntSTPtr, tmpSI) )
         goto memErr_fun26_sec05;
   } /*If: need more memory*/

   if(cntSTPtr->lenSI <= 0)
      goto addLineage_fun26_sec04; /*first lineage*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec03:
   ^   - find lineage or insert position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rightSI = cntSTPtr->lenSI - 1;

   while(leftSI <= rightSI)
   { /*Loop: find lineage or insert position*/
      midSI = (leftSI + rightSI) >> 1;

      tmpSI =
         eqlNull_ulCp(linStr, cntSTPtr->linStrAry[midSI]);

      if(tmpSI > 0)
         leftSI = midSI + 1; /*linStr is greater*/
      else if(tmpSI < 0)
         rightSI = midSI - 1; /*linStr is lesser*/
      else
         break; /*found the insert position*/
   } /*Loop: find lineage or insert position*/

   if(midSI < cntSTPtr->lenSI && tmpSI > 0)
      ++midSI; /*lineage is after the insert position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec04:
   ^   - increase lineage count or if new add lineage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(tmpSI)
   { /*If: is a new lineage*/
      /*not the best method, but allows for ordered array
      `  on the flye
      */
      for(
         tmpSI = cntSTPtr->lenSI - 1;
         tmpSI >= midSI;
         --tmpSI
      ){ /*Loop: shift lineages to make room*/
         swapStr = cntSTPtr->linStrAry[tmpSI + 1];
         cntSTPtr->linStrAry[tmpSI + 1] =
            cntSTPtr->linStrAry[tmpSI];
         cntSTPtr->linStrAry[tmpSI] = swapStr;

         cntSTPtr->idSizeArySI[tmpSI + 1] ^=
            cntSTPtr->idSizeArySI[tmpSI];
         cntSTPtr->idSizeArySI[tmpSI] ^=
            cntSTPtr->idSizeArySI[tmpSI + 1];
         cntSTPtr->idSizeArySI[tmpSI + 1] ^=
            cntSTPtr->idSizeArySI[tmpSI];


         cntSTPtr->linCntArySI[tmpSI + 1] ^=
            cntSTPtr->linCntArySI[tmpSI];
         cntSTPtr->linCntArySI[tmpSI] ^=
            cntSTPtr->linCntArySI[tmpSI + 1];
         cntSTPtr->linCntArySI[tmpSI + 1] ^=
            cntSTPtr->linCntArySI[tmpSI];
      } /*Loop: shift lineages to make room*/

      addLineage_fun26_sec04:;
         cntSTPtr->linCntArySI[midSI] = 1;
         tmpSI = endStr_ulCp(linStr);

         if(tmpSI >= cntSTPtr->idSizeArySI[midSI])
         { /*If: need more memory*/
            if(cntSTPtr->linStrAry[midSI])
               free(cntSTPtr->linStrAry[midSI]);
            cntSTPtr->linStrAry[midSI] =
               malloc((tmpSI + 8) * sizeof(signed char));
            if(! cntSTPtr->linStrAry[midSI])
                goto memErr_fun26_sec05;
            cntSTPtr->idSizeArySI[midSI] = tmpSI;
         } /*If: need more memory*/

         cpLen_ulCp(
            cntSTPtr->linStrAry[midSI],
            linStr,
            tmpSI
         );
         ++cntSTPtr->lenSI;
   } /*If: is a new lineage*/

   else
      ++cntSTPtr->linCntArySI[midSI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun26_sec05:;
      return 1;
} /*addLineage_cnt_getLin*/

/*-------------------------------------------------------\
| Fun27: addReadLineages_cnt_getLin
|   - adds the lineages for a read to a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pointer to hold the counts for
|       each lineage
|   - simpLinArySI:
|     o signed int array with the simple lineages found by
|       simpleLinage_getLin to print
|   - trsLinArySI:
|     o signed int array with the trs lineage assigned to
|       each simple lineage (from simpleLinage_getLin)
|   - simpLenSI:
|     o number of simple lineages in simpLinArySI and
|       trsLinArySI
|   - complexLinArySI:
|     o signed int array with the complex lineages found
|       by complexLineage_getLin
|   - complexLenSI:
|     o number of complese lineages found
|   - pAllVarsBl:
|     o 1: print variants that are set to no print
|     o 0: ingore variants set to no print
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       (one variant) lineage names
|   - complexSTPtr:
|     o complex_linST struct array with the complex
|       (multiple variant) lineages names
| Output:
|   - Modifies:
|     o linStrAry, idSizeArySI, and linCntAry in cntSTPtr
|       to have all lineages in the list
|     o numReadsSL in cntSTPtr to be incurmented by 1
|     o if needed lenSI and sizeSI in cntSTPtr to account
|       for the new lineage or resize
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addReadLineages_cnt_getLin(
   struct cnt_getLin *cntSTPtr,/*holds lineage counts*/
   signed int *simpLinArySI,   /*simple lineages found*/
   signed int *trsLinArySI,    /*simple lineage TRS #s*/
   signed int simpLenSI,       /*# found simple lineages*/
   signed int *complexLinArySI,/*complex lineages found*/
   signed int complexLenSI,   /*# found complex lineages*/
   signed char pAllVarsBl,     /*print no-print lineages*/
   struct simple_linST *simpleSTPtr,/*1VariantLineages*/
   struct complex_linST *complexSTPtr /*complex lineages*/
){
   signed int siLin = 0;
   signed int idLenSI = 0;
   signed char idStr[1024]; /*gets lineage name*/

   struct one_linST *linSTPtr = 0; /*for sanity*/
   struct multi_linST *mLinSTPtr = 0; /*for sanity*/

   ++cntSTPtr->numReadsSL; /*user input one read*/
  
   /*______________add_simple_lineages__________________*/
   for(siLin = 0; siLin < simpLenSI; ++siLin)
   { /*Loop: add simple lineages to the count*/
      linSTPtr =
         &simpleSTPtr->linAryST[simpLinArySI[siLin]];
      if(! pAllVarsBl && ! linSTPtr->printLinBl)
         continue; /*this lineage is not printed*/

      idLenSI =
         cpStr_ulCp(
            idStr,
            linSTPtr->groupStr
         );
      idStr[idLenSI++] = def_trsLinMark_linST;

      if(
            linSTPtr->checkTypeSC != def_trsType_linST
         && linSTPtr->checkTypeSC != def_trsFastType_linST
      ){ /*If: non-trs lineage; add lineage id*/
         if(linSTPtr->lineageStr[0])
            idLenSI +=
               cpStr_ulCp(
                 &idStr[idLenSI],
                 linSTPtr->lineageStr
               );
         else
         { /*Else: no lineage id*/
            idStr[idLenSI++] = 'N';
            idStr[idLenSI++] = 'A';
            idStr[idLenSI++] = 0;
         } /*Else: no lineage id*/
      }  /*If: non-trs lineage; add lineage id*/

      else
         idLenSI +=
            numToStr(&idStr[idLenSI], trsLinArySI[siLin]);

      if( addLineage_cnt_getLin(idStr, cntSTPtr) )
         goto memErr_fun27;
   } /*Loop: add simple lineages to the count*/

   /*______________add_complex_lineages_________________*/
   for(siLin = 0; siLin < complexLenSI; ++siLin)
   { /*Loop: add complex lineages to the count*/
      mLinSTPtr =
         &complexSTPtr->linAryST[complexLinArySI[siLin]];
      if(! pAllVarsBl && ! mLinSTPtr->printLinBl)
         continue; /*this lineage is not printed*/

      idLenSI = cpStr_ulCp(idStr, mLinSTPtr->groupIdStr);
      idStr[idLenSI++] = def_trsLinMark_linST;
      idLenSI +=
        cpStr_ulCp(&idStr[idLenSI],mLinSTPtr->lineageStr);

      if( addLineage_cnt_getLin(idStr, cntSTPtr) )
         goto memErr_fun27;
   } /*Loop: add complex lineages to the count*/

   return 0;
   memErr_fun27:;
      return 1;
} /*addReadLineages_cnt_getLin*/

/*-------------------------------------------------------\
| Fun28: pReadLineages_getLin
|   - prints the lineage counts for a set of reads and
|     then prints the consensus lineage for the reads
|   !!! warning, this does not have a conistent header
|       system, because the TRS lineages are variable.
|       expect different headers for different input
| Input:
|   - idStr:
|     o c-string with id to print for the read
|     o printed until the first white space
|   - cntSTPtr:
|     o cnt_getLin struct pointer with lineage counts to
|       print
|   - minDepthSI:
|     o minimum depth to assign a lineage to the consensus
|   - maxMixedPercF:
|     o maximum percent read support for a minor variant
|       in mixed infection
|       * minor_variant / (major_variant + minor_variant)
|       * 50% (max) means can have an equal ratio
|       * 40% (max) means the minor variant can only take
|         up 40% of the reads
|   - outFILE:
|     o FILE pointer to file to print lineage to
| Output:
|   - Prints:
|     o header, then lineage counts, then the consensus
|       lineage
|       * format: id\ttype\tlineage_1\tlineage_2\t...
|         + id: is the read name
|         + type: marks if the count row (`count`) or if
|                 the consensus row (`consensus`)
|         + lineage_*: name of lineage for this row
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
pReadLineages_getLin(
   signed char *idStr,         /*id for sample to  print*/
   struct cnt_getLin *cntSTPtr,/*counts for each lineage*/
   signed int minDepthSI,    /*min depth to keep lineage*/
   float maxMixedPercF,        /*max minor var in mixed*/
   void *outFILE               /*file to print to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun28 TOC:
   '   - prints the lineage counts for a set of reads and
   '     then prints the consensus lineage for the reads
   '   !!! warning, this does not have a conistent header
   '       system, because the TRS lineages are variable.
   '       expect different headers for different input
   '   o fun28 sec01:
   '     - variable declarations
   '   o fun28 sec02:
   '     - print header and setup memory 
   '   o fun28 sec03:
   '     - number reads per lineage row + find consensus
   '   o fun28 sec04:
   '     - print consensus row
   '   o fun28 sec05:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char useIdStr[64];
   signed int siPos = 0;
   signed int siCon = 0;
   float tmpF = 0;

   signed int *conHeapArySI = 0;
   signed char *mixedHeapArySC = 0;
   signed int conLenSI = 0;

   ulong_ulCp ulDelim = mkDelim_ulCp(def_trsType_linST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec02:
   ^   - print header and setup memory 
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "id\ttype");

   if(cntSTPtr->lenSI)
      fprintf(
         (FILE *) outFILE,
         "\t%s",
         cntSTPtr->linStrAry[0]
      );
   conLenSI = 1;

   for(siPos = 1; siPos < cntSTPtr->lenSI; ++siPos)
   { /*Loop: print the header*/
      fprintf(
         (FILE *) outFILE,
         "\t%s",
         cntSTPtr->linStrAry[siPos]
      );

      if(
         eqlNullDelim_ulCp(
            cntSTPtr->linStrAry[siPos],
            cntSTPtr->linStrAry[siPos - 1],
            ulDelim,
            def_trsLinMark_linST
         )
      ) ++conLenSI; /*count number of groups*/
   } /*Loop: print the header*/

   fprintf((FILE *) outFILE, "\tend%s", str_endLine);


   conHeapArySI = calloc(conLenSI, sizeof(signed int));
   if(! conHeapArySI)
      goto memErr_fun28_sec05;

   mixedHeapArySC = calloc(conLenSI, sizeof(signed char));
   if(! mixedHeapArySC)
      goto memErr_fun28_sec05;

   conLenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec03:
   ^   - number reads per lineage row + find consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpWhite_ulCp(useIdStr, idStr);

   fprintf((FILE *) outFILE, "%s\tcount", useIdStr);

   for(siPos = 0; siPos < cntSTPtr->lenSI; ++siPos)
   { /*Loop: print the count line and find consensus*/
      fprintf(
         (FILE *) outFILE,
         "\t%i",
         cntSTPtr->linCntArySI[siPos]
      );

      if(cntSTPtr->linCntArySI[siPos] >= minDepthSI)
      { /*If: I have enough read depth*/
         if(! conLenSI)
            conHeapArySI[conLenSI++] = siPos;
         else
         { /*Else: check if have better lineage*/
            /*the cnt array is orded by name, which starts
            `  with the goup ID first, so all I need to
            `  check is the lineage before this one
            `eql_ulCp assumes that the strings always
            `  have an ':' (def_trsLinMark_linST) in them,
            `  otherwise it will go out of bounds. This is
            `  ok since I set up my lineages functions to
            `  do this
            */

            if(
               ! eql_ulCp(
                  cntSTPtr->linStrAry[siPos],
                  cntSTPtr->linStrAry[siPos - 1],
                  ulDelim,
                  def_trsLinMark_linST
               )
            ){ /*If: lineages are in the same group*/
               tmpF =
                  (float)
                  min_genMath(
                    cntSTPtr->linCntArySI[siPos],
                    cntSTPtr->linCntArySI[siPos - 1]
                 );

               tmpF /=
                  (float) (
                       cntSTPtr->linCntArySI[siPos]
                     + cntSTPtr->linCntArySI[siPos - 1]
                  );

               if(tmpF < maxMixedPercF)
                  mixedHeapArySC[conLenSI - 1] = 1;
                  /*hit users mixed infection threshold*/

               else if(
                    cntSTPtr->linCntArySI[siPos]
                  > cntSTPtr->linCntArySI[siPos] - 1
               ){ /*Else If: have better read depth*/
                  conHeapArySI[conLenSI - 1] = siPos;
                  mixedHeapArySC[conLenSI - 1] = 0;
               }  /*Else If: have better read depth*/
            }  /*If: lineages are in the same group*/

            else
               conHeapArySI[conLenSI++] = siPos;
         } /*Else: check if have better lineage*/
      } /*If: I have enough read depth*/
   } /*Loop: print the count line and find consensus*/

   fprintf((FILE *) outFILE, "\t*%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec04:
   ^   - print consensus row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   fprintf((FILE *) outFILE, "%s\tconsensus", useIdStr);

   siCon = 0;
   for(siPos = 0; siPos < cntSTPtr->lenSI; ++siPos)
   { /*Loop: print the consensus row*/
      if(siCon >= conLenSI || siPos < conHeapArySI[siCon])
      { /*If: not part of the consensus*/
         if(cntSTPtr->linCntArySI[siPos] < minDepthSI)
            fprintf((FILE *) outFILE, "\tLOW_DEPTH");
         else
            fprintf((FILE *) outFILE, "\tNOT_SELECTED");
      } /*If: not part of the consensus*/

      else
      { /*Else: have consensus entry to print*/
         if(mixedHeapArySC[siCon])
            fprintf((FILE *) outFILE, "\tMIXED_LINEAGE");
         else
            fprintf(
               (FILE *) outFILE,
               "\t%s",
               cntSTPtr->linStrAry[siPos]
            );
         ++siCon;
      } /*Else: have consensus entry to print*/
   } /*Loop: print the consensus row*/

   fprintf((FILE *) outFILE, "\t*%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec05:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siPos = 0;
   goto ret_fun28_sec05;

   memErr_fun28_sec05:;
      siPos = 1;
      goto ret_fun28_sec05;

   ret_fun28_sec05:;
      if(conHeapArySI)
         free(conHeapArySI);
      conHeapArySI = 0;

      if(mixedHeapArySC)
         free(mixedHeapArySC);
      mixedHeapArySC = 0;

      return (signed char) siPos;
} /*pReadLineages_getLin*/

/*-------------------------------------------------------\
| Fun29: pGenomeLineage_getLin
|   - prints lineages found by simpleLinage_getLin and
|     complexLineage_getLin for a single sequence
|   !!! warning, this does not have a conistent header
|       system, because the TRS lineages are variable.
|       expect different headers for different input
| Input:
|   - idStr:
|     o c-string with sequence id/name to print out
|   - simpleLinArySI:
|     o signed int array with the simple lineages found by
|       simpleLinage_getLin to print
|   - trsLinArySI:
|     o signed int array with the trs lineage assigned to
|       each simple lineage (from simpleLinage_getLin)
|   - simpleLenSI:
|     o number of simple lineages in simpleLinArySI and
|       trsLinArySI
|   - complexLinArySI:
|     o signed int array with the complex lineages found
|       by complexLineage_getLin
|   - complexLenSI:
|     o number of complese lineages found
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       lineage names
|   - complexSTPtr:
|     o complex_linST struct array with the complex
|       lineage names
|   - pAllVarsBl:
|     o 1: print variants that are set to no print
|     o 0: ingore variants set to no print
|   - pHeadBlPtr:
|     o signed char pointer telling if to print the header
|       * 1: print header and set bool to 0
|       * 0: do not print the header
|   - outFILE:
|     o FILE pointer to print lineages to
| Output:
|   - Modifies:
|     o pHeadBlPtr to be 0 if it is set to 1
|   - Prints:
|     o header to outFILE (output file) if pHeadBlPtr is 1
|       * format:  "id\ttype\tgenome\tlineage..."
|     o lineage row to outFILE
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
plineages_getLin(
   signed char *idStr,         /*id to print out*/
   signed int *simpleLinArySI, /*simple lineages*/
   signed int *trsLinArySI,    /*tsr simple lineages*/
   signed int simpleLenSI,     /*number simple lineages*/
   signed int *complexLinArySI,/*complex lineages*/
   signed int complexLenSI,    /*number complex lineages*/
   struct simple_linST *simpleSTPtr, /*simple lineages*/
   struct complex_linST *complexSTPtr, /*complexLineages*/
   signed char pAllVarsBl,     /*print no-print lineages*/
   signed char *pHeadBlPtr,    /*1: print header*/
   void *outFILE               /*file to print to*/
){
   struct cnt_getLin linStackST;
   init_cnt_getLin(&linStackST);

   /*______________SETUP_LINEAGES_FOR_PRINT_____________*/
   if(
      addReadLineages_cnt_getLin(
         &linStackST,
         simpleLinArySI,
         trsLinArySI,
         simpleLenSI,
         complexLinArySI,
         complexLenSI,
         pAllVarsBl,
         simpleSTPtr,
         complexSTPtr
     ) /*gets lineage output formant and sorts by group*/
   ) goto memErr_fun29;

   /*___________________PRINT_HEADER____________________*/
   if(*pHeadBlPtr)
   { /*If: printing the header*/
      fprintf((FILE *) outFILE, "id\ttype\tlineages...");
      fprintf((FILE *) outFILE, "%s", str_endLine);
      *pHeadBlPtr = 0;
   } /*If: printing the header*/

   /*___________________PRINT_LINEAGES__________________*/
   fprintf((FILE *) outFILE, "%s\tgenome", idStr);
   for(
      simpleLenSI = 0;
      simpleLenSI < linStackST.lenSI;
      ++simpleLenSI
   ) fprintf(
        (FILE *) outFILE,
        "\t%s",
        linStackST.linStrAry[simpleLenSI]
     );
   fprintf((FILE *) outFILE, "\t*%s", str_endLine);

   /*_______________CLEAN_UP_AND_RETURN_________________*/
   complexLenSI = 0;
   goto ret_fun29;

   memErr_fun29:;
      complexLenSI = 1;
      goto ret_fun29;
   ret_fun29:;
      freeStack_cnt_getLin(&linStackST);
      return (signed char) complexLenSI;
} /*pGenomeLineage_getLin*/

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
: Copyright (c) 2025 jeremyButtler
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
