/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

#include "checkAmr.h"

#include <stdio.h>
#include <stdlib.h>

#include "amrStruct.h"
#include "drug_str_ary.h"
#include "../generalLib/samEntryStruct.h"

/*Only .h files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/codonTbl.h"
#include "../generalLib/ulCpStr.h"

/*-------------------------------------------------------\
| Fun-05: checkAmrSam
|   - Checks if a sequence in a sam file entry has
|     amr's (antibiotic resitance)
| Input:
|   - samSTPtr:
|     o Pointer to samEntry struct with sequence to check
|       for amr's
|   - amrSTAryPtr:
|     o Pointer to an array of amrStruct structures with
|       amr's to check for.
|   - numArmI:
|     o Number of amrStruct structures in amrAryST
|   - numHitsI:
|     o Updated to hold the number of amr's found in
|       the sequence stored in samST
|   - errC:
|     o Pointer to character to hold the error output
| Output:
|   - Modifies:
|     o numHitsI to hold the number of amr's found in the
|       sequence in samST
|     o errC:
|       - 0 for No errors
|       - 64 for memory errors
|   - Returns:
|     o List of amrHit structures with the detected amr's
|     o 0 for memory errors or when no amr's were found
\-------------------------------------------------------*/
struct amrHit * checkAmrSam(
   void *samSTPtr,     /*Sequence to check*/
   void *amrSTAryPtr,  /*Has amr's to check*/
   int numAmrI,        /*Length of amrAryST*/
   int *numHitsI,      /*Number amr hits for seq*/
   char *errC          /*For error reporting*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-05 TOC: checkAmrSam
   '   - Checks if a sequence in a sam file entry has
   '     amr's (antibiotic resitance)
   '   o Fun-05 Sec-01:
   '     - Variable declerations
   '   o Fun-05 Sec-02:
   '     - Find the nearest amr to the sequence
   '   o Fun-05 Sec-03:
   '     - Check for AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-05 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*For finding the amr position*/
   uint seqPosUI = 0;
   uint refPosUI = 0;

   /*For processing the cigar entries*/
   int numBasesI = 0;  /*Number bases in cigar entry*/
   int cigBaseOnI = 0; /*Number bases left in cig entry*/
   int iCig = 0;       /*Iterate through the cigar*/


   /*For checking amr's*/
   uchar *seqUStr = 0;
   uchar *amrUStr = 0;

   int iAmr = 0;       /*Iterate through the amr's list*/
   int iBase = 0;      /*iterate/compare amr pattern*/

   char resBl = 0;     /*-1 is resitance; 0 is not*/

   /*For aa amr check*/
   uchar base1UC = 0;    /*Base 1 in amr codon*/
   uchar base2UC = 0;    /*Base 2 in amr codon*/
   uchar base3UC = 0;    /*Base 3 in amr codon*/
   char aaC = 0;         /*Holds sequence AA at position*/

   /*Return values/used to build amr list*/
   struct amrHit *amrSTList = 0; /*Amr list to return*/
   struct amrHit *amrST = 0;     /*Used in building list*/

   /*Cast the void variables*/
   struct amrStruct *amrAryST =
      (struct amrStruct *) amrSTAryPtr;

   struct samEntry *samST = (struct samEntry *) samSTPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-05 Sec-02:
   ^   - Find the nearest amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errC = 0;

   iAmr =
      findNearestAmr(
         amrAryST,
         (int) samST->refStartUI,
         numAmrI
      ); /*Find the nearest amr to this sequence*/

   /*Check if this is at the end of the amr list*/
   if(iAmr == -1) return 0;
   
   refPosUI = samST->refStartUI - 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-05 Sec-03:
   ^   - Check for AMRs
   ^   o fun-05 sec-03 sub-01:
   ^     - Find the start of the AMR
   ^   o fun-05 sec-03 sub-02:
   ^     - Move to the first base in the amr pattern
   ^   o fun-05 sec-03 sub-03:
   ^     - Check if the amr has a strict match
   ^   o fun-05 sec-03 sub-04:
   ^     - Handle amino acid amr's
   ^   o fun-05 sec-03 sub-05:
   ^     - Check if resistance was found
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-05 Sec-03 Sub-01:
   *   - Find the start of the AMR
   \*****************************************************/

   iCig = -1;
 
   for(
      iAmr = iAmr;
      iAmr < numAmrI;
      ++iAmr
   ){ /*Loop: Check if is an amr or not*/

      /*Check if I have another amr*/
      if(samST->refEndUI < (amrAryST[iAmr].refPosUI + 1))
         return amrSTList;

      /**************************************************\
      * Fun-05 Sec-03 Sub-02:
      *   - Move to the first base in the amr pattern
      \**************************************************/

      while(refPosUI < amrAryST[iAmr].refPosUI)
      { /*Loop: Find the start of the reference*/

         /*Check if I did a full cigar entry move*/
         if(cigBaseOnI >= numBasesI)
         { /*If: I finshed the last round*/
            ++iCig;
            numBasesI = samST->cigValAryI[iCig];
            cigBaseOnI = numBasesI;
         } /*If: I finshed the last round*/

         switch(samST->cigTypeStr[iCig])
         { /*Switch: check what the next entry is*/
            case 'S':
            case 'I':
            /*Case: Softmasking or insertions*/
               seqPosUI += numBasesI;
               break;
            /*Case: Softmasking or insertions*/

            case 'D':
            /*Case: Deletion*/
               refPosUI += numBasesI;
               cigBaseOnI = numBasesI;

               /*Deletion at amr site means error. There
               ` should be enough sequence around it to
               ` move onto the deletion
               */
               if(amrAryST[iAmr].refPosUI < refPosUI)
                  goto nextArm; /*Move to next amr*/

               break;
            /*Case: Deletion*/

            case 'M':
            case 'X':
            case '=':
            /*Case: match (M or =) or snp (M or X)*/
               refPosUI += cigBaseOnI;
               seqPosUI += cigBaseOnI;

               cigBaseOnI =
                  (int) (
                       refPosUI
                     - amrAryST[iAmr].refPosUI
                  ); /*See if I overshot the target*/

               /*If I did not overshoot the target*/
               if(cigBaseOnI <= 0) cigBaseOnI = numBasesI;

               /*Correct for overshooting*/
               else
               { /*Else: I overshot the amr*/
                  refPosUI -= cigBaseOnI; 
                  seqPosUI -= cigBaseOnI; 
               } /*Else: I overshot the amr*/

               break;
            /*Case: match (M or =) or snp (M or X)*/
         } /*Switch: check what the next entry is*/
      } /*Loop: Find the start of the reference*/

      /**************************************************\
      * Fun-05 Sec-03 Sub-03:
      *   - Check if the amr has a strict match
      \**************************************************/

      ++amrAryST[iAmr].numMapReadsUI;
      seqUStr = (uchar *) &samST->seqStr[seqPosUI] - 1;

      /*check if the amr is from an amino acid change*/
      if(amrAryST[iAmr].amrAaStr != 0) goto  checkAaAmr;

      checkSNP:;

      amrUStr = (uchar *) amrAryST[iAmr].amrSeqStr;

      for(
         iBase = 0;
         (amrUStr[iBase] &~32) == (seqUStr[iBase] &~32);
         ++iBase
      ) {}

      /*If there was an extact match to amrStr*/
      resBl = -(amrUStr[iBase] == '\0');

      /*If this was a deletion*/
      if(  amrAryST[iAmr].lenAmrSeqUI
         < amrAryST[iAmr].lenRefSeqUI
      ) resBl =
           -(   (seqUStr[iBase] & ~32)
             != (uchar)
                (amrAryST[iAmr].refSeqStr[iBase] & ~32)
           ); /*Make sure deletion is present*/

      goto checkIfRes;

      /**************************************************\
      * Fun-05 Sec-03 Sub-04:
      *   - Handle amino acid amr's
      *   o fun-05 sec-03 sub-04 cat-01:
      *     - Check if deletions are resistant
      *   o fun-05 sec-03 sub-04 cat-02:
      *     - Move to the first base in the target codon
      *   o fun-05 sec-03 sub-04 cat-03:
      *     - Check reverse complemnt gene snps/insertions
      *   o fun-05 sec-03 sub-04 cat-04:
      *     - Else I do not know direction, look at snps
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-05 Sec-03 Sub-04 Cat-01: checkAmrSam
      +   - Check if deletions are resistant
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      checkAaAmr:

      /*For deletions, I know the count, so I do not
      `   have to worry about the direction of the
      `   deletion
      */
      if(amrAryST[iAmr].aaDelBl == def_amrST_del)
      { /*If: I have a deletion whithout insertions*/
         if(   samST->cigTypeStr[iCig + 1] != 'D'
            || cigBaseOnI + (iBase * 3) < numBasesI
         ) resBl = 0;

         else if(
              samST->cigValAryI[iCig + 1]
            < amrAryST[iAmr].lenRefAaUI - 2
         ) resBl = 0;

         else resBl = -1;

         goto checkIfRes;
      } /*If: I have a deletion whithout insertions*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-05 Sec-03 Sub-04 Cat-02: checkAmrSam
      +   - Move to the first base in the target codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*This should work out, even when reverse*/
      seqUStr +=
         (int) amrAryST[iAmr].codonPosUI - (int) refPosUI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-05 Sec-03 Sub-04 Cat-03: checkAmrSam
      +   - Check reverse complemnt gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(amrAryST[iAmr].dirFlag == def_amrST_revCompDir)
      { /*If: This is a reverse complement gene*/
         for(
            iBase = 0;
            iBase < amrAryST[iAmr].lenAmrAaUI;
            ++iBase 
         ){ /*Loop: Check the codon reading frame*/
            if(*seqUStr == '\0') goto aaNotResitantRev;

            base1UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0') goto aaNotResitantRev;

            base2UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0') goto aaNotResitantRev;

            base3UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            aaC = codonLkTbl[base1UC][base2UC][base3UC];

            /*The 2024 WHO catalog has ? marks for
            ` non-insertions/deletions; so I am safe here
            ` for now
            */
          
            if(
                  amrAryST[iAmr].amrAaStr[iBase] == '?'
               && aaC != amrAryST[iAmr].refAaStr[iBase]
            ) continue; /*is just not reference amr*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iBase])
            { /*If: this does not match the mutant*/
               aaNotResitantRev:;
               resBl = 0;
               goto checkIfRes;
            } /*If: this does not match the mutant*/
         } /*Loop: Check the codon reading frame*/

         resBl = -1;
         goto checkIfRes;
      } /*If: This is a reverse complement gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-05 Sec-03 Sub-04 Cat-04: checkAmrSam
      +   - Check forward gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         amrAryST[iAmr].dirFlag == def_amrST_forwardDir
      ){ /*Else If: This is a foward gene*/
         for(
            iBase = 0;
            iBase < amrAryST[iAmr].lenAmrAaUI;
            ++iBase 
         ){ /*Loop: Check the codon reading frame*/
            if(*seqUStr == '\0') goto aaNotResitantFor;

            base1UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0') goto aaNotResitantFor;

            base2UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0') goto aaNotResitantFor;

            base3UC = (uchar) baseToCodeLkTbl[*seqUStr++];
            aaC = codonLkTbl[base1UC][base2UC][base3UC];

            /*The 2023 WHO catalog has ? marks for
            ` non-insertions/deletions; so I am safe here
            ` for now
            */
          
            if(
                  amrAryST[iAmr].amrAaStr[iBase] == '?'
               && aaC != amrAryST[iAmr].refAaStr[iBase]
            ) continue; /*is just not reference amr*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iBase])
            { /*If: this does not match the mutant*/
               aaNotResitantFor:;
               resBl = 0;
               goto checkIfRes;
            } /*If: this does not match the mutant*/
         } /*Loop: Check the codon reading frame*/

         resBl = -1;
         goto checkIfRes;
      } /*Else If: This is a foward gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-05 Sec-03 Sub-04 Cat-05: checkAmrSam
      +   - Else I do not know the direction, look at snp
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else  goto checkSNP;

      /**************************************************\
      * Fun-05 Sec-03 Sub-05:
      *   - Check if resistance was found
      \**************************************************/

      checkIfRes:

      if(resBl)
      { /*If: I found a resitance mutation*/
         ++(amrAryST[iAmr].numSupReadsUI);
         ++(*numHitsI);

         if(amrSTList)
         { /*If: This is not the first amr*/
            amrST->nextAmr = makeAmrHit();

            if(!amrST->nextAmr)
            { /*If: I had a memory error*/
               *errC = 64;
               freeAmrHitList(amrSTList);
               return 0;
            } /*If: I had a memory error*/

            amrST->nextAmr->amrST = &amrAryST[iAmr];
            amrST->nextAmr->seqPosUI = seqPosUI;
            amrST = amrST->nextAmr;
         } /*If: This is not the first amr*/

         else
         { /*Else: this is the first amr*/
            amrSTList = makeAmrHit();
            amrST = amrSTList;

            if(!amrST)
            { /*If: I had a memory error*/
               *errC = 64;
               freeAmrHitList(amrSTList);
               return 0;
            } /*If: I had a memory error*/

            amrST->amrST = &amrAryST[iAmr];
            amrST->seqPosUI = seqPosUI;
         } /*Else: this is the first amr*/
      } /*If: I found a resitance mutation*/

      nextArm:;
        /*the ; is to deal with label after compound
        ` statement error in some versions of gcc
        */
   } /*Loop: Check if is an amr or not*/

   return amrSTList;
} /*checkAmr*/

/*-------------------------------------------------------\
| Fun-18: pCrossRes
|   - Print out cross resitance (for report, not database)
| Input:
|   - amrSTPtr:
|     o Pionter to amrStruct structure to print out cross
|       resistance for
|   - oufFILE:
|     o FIle to print the cross restance to
| Output:
|   - Prints:
|     o NA if no cross resitance
|     o drug1-drug2-...-drugn if there is any cross
|       resistance
\-------------------------------------------------------*/
#define pCrossRes(amrSTPtr, drugAryStr, outFILE){\
   int iAmrMac = 0; /*Index of amr on*/\
   int iIndexMac = 0;\
   char firstPrintMacBl = 1;\
   char *drugMacStr = 0;\
   ulong flagsMacUL = 0;\
   \
   for(iAmrMac=0; iAmrMac < def_amrST_maxDrugs;++iAmrMac)\
   { /*Loop: Go though all amr elements*/\
      flagsMacUL = (amrSTPtr)->crossResFlagsUL[iAmrMac];\
      \
      while(flagsMacUL)\
      { /*Loop: Check if flags support an antibiotic*/\
         if(! (flagsMacUL & 1))\
         { /*If: this position is not an antibiotic*/\
            ++iIndexMac;\
            flagsMacUL >>= 1;\
            continue;\
         } /*If: this position is not an antibiotic*/\
         \
         drugMacStr =\
            getDrugFromDrugAry((drugAryStr), iIndexMac);\
         \
         if(! firstPrintMacBl)\
            fprintf((outFILE), "-%s", drugMacStr);\
         else \
         { /*Else: This is the first drug I am printing*/\
            firstPrintMacBl = 0;\
            fprintf((outFILE), "%s", drugMacStr);\
         } /*Else: This is the first drug I am printing*/\
      } /*Loop: Check if flags support an antibiotic*/\
      \
   } /*Loop: Go though all amr elements*/\
   \
   /*Check if there was any cross resitance*/\
   if(firstPrintMacBl) fprintf((outFILE), "NA");\
} /*pCrossRes*/

/*-------------------------------------------------------\
| Fun-06: pAmrHitList
|   - Prints out all amr's that were in a sequence
| Input:
|   - seqIdStr:
|     o C-string with name of sequence that had the amr
|   - amrHitSTListPtr:
|     o Pointer to a List of amrHit structures with amr's
|       to print out
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - pHeadBl:
|     o 1: Print out the header
|     o 0: Do not print the header
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's in amrHitSTListPtr to outFILE
|     o The header if pHeadBl is 1
|   - Sets:
|     o pHeadBl to 0 if it is set to 1
\-------------------------------------------------------*/
void pAmrHitList(
   char *seqIdStr,
   struct amrHit *amrHitSTListPtr,
   char *drugAryStr,
   char pHeadBl,
   FILE *outFILE
){
   struct amrHit *tmpST = (amrHitSTListPtr);
   ulong amrFlagUL = 0;
   int flagOnI = 0;
   char *drugStr = 0;
   char firstPrintMacBl = 0;
   int iAmrMac = 0;
   
   if((pHeadBl))
   { /*If: I am printing the header*/
      (pHeadBl) = 0;
      fprintf((outFILE), "Id\tGene\tDrug");
      fprintf((outFILE), "\tCrossResitance\tVariantId");
      fprintf((outFILE), "\tType\t\trefPos\tseqPos");
      fprintf((outFILE), "\trefSeq\tamrSeq\teffect");
      fprintf((outFILE), "\twhoComment\n");
   } /*If: I am printing the header*/
   
   while(tmpST)
   { /*Loop: Print out all amr's*/
      /*There is resistance, print it out*/
      fprintf(
        (outFILE),
        "\t%s\t%s\t",
        (seqIdStr),              /*Name of the seq*/
        tmpST->amrST->geneIdStr /*Gene name*/
      ); /*Pirnt out gene id and drug*/
      
      
      flagOnI = 0;
      firstPrintMacBl = 1;
      
      for(
         iAmrMac = 0;
         iAmrMac < def_amrST_maxDrugs;
         ++iAmrMac
      ){ /*Loop: Run through each set of flags*/
         amrFlagUL = tmpST->amrST->amrFlagsUL[iAmrMac];
         
         while(amrFlagUL)
         { /*Loop: Checn each flag in a set*/
            if(!(amrFlagUL & 1))
            { /*If: There is no amr to this drug*/
               ++flagOnI;
               amrFlagUL >>= 1;
               continue; /*No resitance*/
            } /*If: There is no amr to this drug*/
            
            drugStr =
               getDrugFromDrugAry((drugAryStr), flagOnI);
            
            if(firstPrintMacBl)
            { /*If: this is the frist printout*/
               fprintf((outFILE), "%s", drugStr);
               firstPrintMacBl = 0;
            } /*If: this is the frist printout*/
            
            else fprintf((outFILE), "_%s", drugStr);
            
            ++flagOnI;
            amrFlagUL >>= 1;
         } /*Loop: Checn each flag in a set*/
      } /*Loop: Run through each set of flags*/
      
      
      fprintf((outFILE), "\t");
      pCrossRes(tmpST->amrST,(drugAryStr),(outFILE));
      
      fprintf(
        (outFILE),
        "\t%s\t%s",
        tmpST->amrST->varIdStr,  /*Variant id*/
        tmpST->amrST->mutTypeStr /*snp/del/ins/LoF*/
      ); /*Print out the variant id and type*/
      
      fprintf(
        (outFILE),
        "\t%u\t%u\t%s\t%s",
        tmpST->amrST->refPosUI,  /*Position on ref*/
        tmpST->seqPosUI,         /*Position on seq*/
        tmpST->amrST->refSeqStr, /*Reference pattern*/
        tmpST->amrST->amrSeqStr  /*amr pattern*/
      );
      
      if(tmpST->amrST->effectStr)
         fprintf(
            (outFILE),
            "\t%s",
            tmpST->amrST->effectStr
         );
      else fprintf((outFILE), "\tNA");
      
      if(tmpST->amrST->commentStr)
         fprintf(
            (outFILE),
            "\t%s\n",
            tmpST->amrST->commentStr
         );
      else fprintf((outFILE), "\tNA\n");
      
      tmpST = tmpST->nextAmr;
   } /*Loop: Print out all amr's*/
} /*pAmrHitList*/

/*-------------------------------------------------------\
| Fun-07: pAmrs
|   - Prints out all amr's that meant the min depth
| Input:
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally)
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|   - totalReadsUI:
|     o total number of reads input
|   - amrSTAry:
|     o Pointer to an array of amrStruct structures to
|       check and print
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - pHeadBl:
|     o 1: Print out the header
|     o 0: Do not print the header
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's that meet the min stats and have at
|       least one mapped read
|     o The header if pHeadBl is 1
|   - Sets:
|     o pHeadBl to 0 if it is set to 1
\-------------------------------------------------------*/
void pAmrs(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   unsigned int totalReadsUI,
   struct amrStruct *amrSTAry,
   unsigned int numAmrsUI,
   char *drugAryStr,
   char pHeadBl,
   FILE *outFILE
){
   uint indexUI = 0;
   float percSupF = 0;
   ulong amrFlagUL = 0;
   int flagOnI = 0;
   char *drugStr = 0;
   int iAmrMac = 0;
   char firstPrintMacBl = 1;
   
   if((pHeadBl))
   { /*If: I am printing the header*/
      (pHeadBl) = 0;
      fprintf((outFILE),"\tGene\tDrug\tcrossResistance");
      fprintf((outFILE),"\tVariantId\tType\trefPos");
      fprintf((outFILE),"\trefSeq\tamrSeq\tmappedReads");
      fprintf((outFILE),"\tsupportingReads");
      fprintf((outFILE),"\teffect\twhoComment\n");
   } /*If: I am printing the header*/
   
   for(indexUI = 0; indexUI < (numAmrsUI); ++indexUI)
   { /*Loop:Check and print out amrs*/
      if((amrSTAry)[indexUI].numSupReadsUI <(minDepthUI))
         continue;
      
      if((amrSTAry)[indexUI].numMapReadsUI ==0) continue;
      
      percSupF =
           ((float) (amrSTAry)[indexUI].numMapReadsUI)
         / (totalReadsUI);
      
      if(percSupF < (minPercTotalF)) continue;
      
      percSupF =
           ((float) (amrSTAry)[indexUI].numSupReadsUI)
         / ((float)(amrSTAry)[indexUI].numMapReadsUI);
      
      if(percSupF < (minPercMapF)) continue;
      
      
      fprintf(
        (outFILE),
        "%s\t",
        (amrSTAry)[indexUI].geneIdStr /*gene id*/
      ); /*Pirnt out gene id and drug*/
      
      
      flagOnI = 0;
      firstPrintMacBl = 1;
      
      for(
         iAmrMac = 0;
         iAmrMac < def_amrST_maxDrugs;
         ++iAmrMac
      ){ /*Loop: Run through each set of flags*/
         amrFlagUL =
            (amrSTAry)[indexUI].amrFlagsUL[iAmrMac];
         
         while(amrFlagUL)
         { /*Loop: Checn each flag in a set*/
            if(!(amrFlagUL & 1))
            { /*If: There is no amr to this drug*/
               ++flagOnI;
               amrFlagUL >>= 1;
               continue; /*No resitance*/
            } /*If: There is no amr to this drug*/
            
            drugStr =
               getDrugFromDrugAry((drugAryStr), flagOnI);
            
            if(firstPrintMacBl)
            { /*If: this is the frist printout*/
               fprintf((outFILE), "%s", drugStr);
               firstPrintMacBl = 0;
            } /*If: this is the frist printout*/
            
            else fprintf((outFILE), "_%s", drugStr);
            
            ++flagOnI;
            amrFlagUL >>= 1;
         } /*Loop: Checn each flag in a set*/
         
         /*There is resistance, print it out*/
         fprintf((outFILE), "\t");
         
         pCrossRes(
            &(amrSTAry)[indexUI],
           (drugAryStr),
           (outFILE)
         ); /*Print out cross-resistance drugs*/
         
         fprintf(
           (outFILE),
           "\t%s\t%s",
           (amrSTAry)[indexUI].varIdStr, /*variant id*/
           (amrSTAry)[indexUI].mutTypeStr/*mutationType*/
         ); /*Print out the gene id and drug name*/
         
         fprintf(
           (outFILE),
           "\t%u\t%s\t%s\t%u\t%u",
           (amrSTAry)[indexUI].refPosUI, /*ref Position*/
           (amrSTAry)[indexUI].refSeqStr,/*Ref pattern*/
           (amrSTAry)[indexUI].amrSeqStr, /*amr pattern*/
           (amrSTAry)[indexUI].numMapReadsUI,
           (amrSTAry)[indexUI].numSupReadsUI
         );
         
         if((amrSTAry)[indexUI].effectStr)
            fprintf(
               (outFILE),
               "\t%s",
               (amrSTAry)[indexUI].effectStr
            );
         else fprintf((outFILE), "\tNA");
         
         if((amrSTAry)[indexUI].commentStr)
         { /*If: I have a who comment*/
            /*This is to handle blanks left by WHO. I
            ` have not been able to catch this in my
            ` readin scripts
            */
            if((amrSTAry)[indexUI].commentStr[0] < 33)
               fprintf((outFILE), "\tNA\n");
            else
               fprintf(
                  (outFILE),
                  "\t%s\n",
                  (amrSTAry)[indexUI].commentStr
               );
         } /*If: I have a who comment*/
         
         else fprintf((outFILE), "\tNA\n");
        
         ++flagOnI;
         amrFlagUL >>= 1;
      } /*Loop: Checn each flag*/
   } /*Loop:Check and print out amrs*/\
} /*pAmrs*/

/*-------------------------------------------------------\
| Fun-08: lookForAmrsSam
|   - Look for anti-microbial (antibiotic) genes in the
|     reads in a sam file
| Input:
|   - buffStr:
|     o Temporary buffer (c-string) to use when reading in
|       the sam file. This is resized as needed.
|   - lenBuffUL:
|     o Current length of buffStr. This is updated when
|       buffStr is resized
|   - readsBl:
|     o 1: Printing out read stats (use pAmr)
|     o 0: Printing out consensus stats (use pAmrHitList)
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|     o This is applied to read checks only
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally) 
|     o This is applied to read checks only
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|     o This is applied to read checks only
|   - samFILE:
|     o Pointer to sam file to read  from
|   - outFILE:
|     o Pointer to file to print the amr results to
|   - idPrefStr:
|     o Prefix to name the read id file(s). Input 0/null
|       to not print out any ids
| Output:
|   - Prints:
|     o Stats about AMRs to outFILE
|     o Read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o 2 for file open errors
|     o 64 for memory errors
\-------------------------------------------------------*/
char lookForAmrsSam(
   void *amrSTAryPtr, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   char *drugAryStr, /*Has antibiotic names*/
   char readsBl,     /*1: processing reads not cons*/
   uint minDepthUI,  /*Min depth to keep amr (read only)*/
   float minPercMapF,/*Min % support to keep amr (read)*/
   float minPercTotalF, /*Min % mapped reads to keep*/
   char *samStr,    /*Sam file with reads to check*/
   char *outStr,
   char *idPrefStr    /*Prefix for id files*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC:P lookForAmrsSam
   '   - Look for anti-microbial (antibiotic) genes in the
   '     reads in a sam file
   '   o fun-08 sec-01:
   '     - Variable declerations
   '   o fun-08 sec-02:
   '     - Get the first sam entry
   '   o fun-08 sec-03:
   '     - Check for AMRs
   '   o fun-08 sec-04:
   '     - Print out read AMR stats
   '   o fun-08 sec-05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *buffStr = 0;   /*Buffer for readSamLine*/
   ulong lenBuffUL = 0; /*Current length of buffStr*/

   char errC = 0;         /*For error checking*/
   char pHeadBl = 1;      /*To print header*/
   int numHitsI = 0;      /*Number of amrs hits/con*/
   uint totalReadsUI = 0; /*Number of kept reads*/

   struct samEntry samST;
   struct amrHit *amrHitSTList = 0;
   struct amrHit *tmpHitST = 0;
   uint lenFileNameUI = 0;
   uint lenPrefUI = 0;

   FILE *idFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = 0;


   struct amrStruct *amrAryST =
      (struct amrStruct *) amrSTAryPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^   - Get the first sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samST);

   if(! samStr || *samStr == '-') samFILE = stdin;
   
   else
   { /*Else: I am given a sam file*/
      samFILE = fopen(samStr, "r");

      if(! samFILE)
         goto fileErr_sec05_sub02_lookForAmrsSam;
   } /*Else: I am given a sam file*/

   if(! outStr || *outStr == '-') outFILE = stdin;

   else
   { /*Else: I am given an output file*/
      outFILE = fopen(outStr, "r");

      if(! outFILE)
         goto fileErr_sec05_sub02_lookForAmrsSam;
   } /*Else: I am given an output file*/

   if(idPrefStr) lenPrefUI = cLenStr(idPrefStr, '\0');

   errC =
      readSamLine(
         &samST,
         &buffStr,
         &lenBuffUL,
         samFILE
      ); /*Read in the first line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Check for AMRs
   ^   o fun-08 sec-03 sub-01:
   ^     - Filter out less usefull entries
   ^   o fun-08 sec-03 sub-02:
   ^     - Check for amrs
   ^   o fun-08 sec-03 sub-03:
   ^     - Print out consensus sequence AMRS
   ^   o fun-08 sec-03 sub-04:
   ^     - Deal with read amrs; print ids if requested/
   ^       free consensus structuerrs
   ^   o fun-08 sec-03 sub-05:
   ^     - Move to the next sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-03 Sub-01:
   *   - Filter out less usefull entries/start loop
   \*****************************************************/

   while(!errC)
   { /*Loop: Check if have antibiotic resitance*/
      if(samST.extraStr[0] == '@')
      { /*If: this is a header*/
         errC =
            readSamLine(
               &samST,
               &buffStr,
               &lenBuffUL,
               samFILE
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is a header*/

      /*4=unmapped, 256=secondary, 2048=supplemental*/
      if(samST.flagUS & (4 + 256))
      { /*If: this is an umapped read, 2ndary*/
         errC =
            readSamLine(
               &samST,
               &buffStr,
               &lenBuffUL,
               samFILE
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is an umapped read, 2ndary, sup aln*/

      /**************************************************\
      * Fun-08 Sec-03 Sub-02:
      *   - Check for amrs
      \**************************************************/

      ++totalReadsUI;

      amrHitSTList = 
         checkAmrSam(
            &samST,   /*Sequence to check*/
            amrAryST, /*Has amr's to check*/
            numAmrI,  /*Length of amrAryST*/
            &numHitsI,/*Number amr hits for seq*/
            &errC     /*For error reporting*/
         ); /*Check if there are any amr's*/

      if(errC)
         goto memErr_sec05_sub02_lookForAmrsSam;

      /**************************************************\
      * Fun-08 Sec-03 Sub-03:
      *   - Print out consensus sequence AMRS
      \**************************************************/

      /*Check if I can print out the amr's*/
      if( (!! amrHitSTList) & (! readsBl) )
      { /*If: There were amr's*/
          pAmrHitList(
             samST.qryIdStr,
             amrHitSTList,
             (drugAryStr),
             pHeadBl,
             (outFILE)
          ); /*Print the amr entry*/

          freeAmrHitList(amrHitSTList);
      } /*If: There were amr's*/

      /**************************************************\
      * Fun-08 Sec-03 Sub-04:
      *   - Deal with read amrs; print ids if requested/
      *     free consensus structuerrs
      \**************************************************/

      else if( (!! readsBl) & (!! amrHitSTList) )
      { /*Else If; I am processing reads*/
          tmpHitST = amrHitSTList;
          
          if(idPrefStr)
          { /*If: I am printing the read ids*/
             while(tmpHitST)
             { /*Loop: Print each amr the read mapped to*/
                /*Make the file name; add prefix*/
                lenFileNameUI = lenPrefUI;
                cCpStr(buffStr, idPrefStr, lenPrefUI);

                buffStr[lenFileNameUI] = '-';
                ++lenFileNameUI;

                /*Add the gene id*/
                cCpStr(
                   &buffStr[lenFileNameUI],
                   tmpHitST->amrST->geneIdStr,
                   tmpHitST->amrST->lenGeneIdUI
                ); /*Make the file name*/

                lenFileNameUI +=
                   tmpHitST->amrST->lenGeneIdUI;

                buffStr[lenFileNameUI] = '-';
                ++lenFileNameUI;

                /*Add the variant id*/
                cCpStr(
                   &buffStr[lenFileNameUI],
                   tmpHitST->amrST->varIdStr,
                   tmpHitST->amrST->lenVarIdUI
                ); /*Make the file name*/

                lenFileNameUI +=
                   tmpHitST->amrST->lenVarIdUI;

                /*Add the file extension*/
                buffStr[lenFileNameUI] = '.';
                buffStr[lenFileNameUI + 1] = 'i';
                buffStr[lenFileNameUI + 2] = 'd';
                buffStr[lenFileNameUI + 3] = 's';
                buffStr[lenFileNameUI + 4] = '\0';

                idFILE = fopen(buffStr, "a");

                if(!idFILE)
                  goto fileErr_sec05_sub02_lookForAmrsSam;

                fprintf(idFILE, "%s\n", samST.qryIdStr);
                fclose(idFILE);
                idFILE = 0;
                tmpHitST = tmpHitST->nextAmr;
             } /*Loop: Print each amr the read mapped to*/
          } /*If: I am printing the read ids*/

          freeAmrHitList(amrHitSTList);
      } /*Else If; I am processing reads*/

      /**************************************************\
      * Fun-08 Sec-03 Sub-05:
      *   - Move to the next sam entry
      \**************************************************/

      errC =
         readSamLine(
            &samST,
            &buffStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next sam file entry*/
   } /*Loop: Check if have antibiotic resitance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-04:
   ^   - Print out read AMR stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(errC > 1) goto memErr_sec05_sub02_lookForAmrsSam;

   if(samFILE != stdin) fclose(samFILE);
   freeSamEntryStack(&samST);
   free(buffStr);
   buffStr = 0;

   if(readsBl)
   { /*If: I mapped reads, not consensuses*/
      pAmrs(
         minDepthUI,
         minPercMapF,
         minPercTotalF,
         totalReadsUI,
         amrAryST,
         numAmrI,
         drugAryStr,
         pHeadBl,
         outFILE
      ); /*Print out the AMRs*/
   } /*If: I mapped reads, not consensuses*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-05:
   ^   - Clean up
   ^   o fun-08 sec-05 sub-01:
   ^     - Clean up after a successful run
   ^   o fun-08 sec-05 sub-02:
   ^     - Clean up after a memory error
   ^   o fun-08 sec-05 sub-03:
   ^     - Clean up after an file error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-05 Sub-01:
   *   - Clean up after a successful run
   \*****************************************************/

   if(outFILE != stdout) fclose(outFILE);

   if(idFILE) fclose(idFILE);

   return 0;

   /*****************************************************\
   * Fun-08 Sec-05 Sub-02:
   *   - Clean up after a memory error
   \*****************************************************/

   memErr_sec05_sub02_lookForAmrsSam:;

      if(buffStr) free(buffStr);
      if(samFILE != stdin) fclose(samFILE);
      if(outFILE != stdout) fclose(outFILE);

      if(idFILE) fclose(idFILE);

      freeSamEntryStack(&samST);

      if(amrHitSTList) freeAmrHitList(amrHitSTList);

      return 64;

   /*****************************************************\
   * Fun-08 Sec-05 Sub-03:
   *   - Clean up after an file error
   \*****************************************************/

   fileErr_sec05_sub02_lookForAmrsSam:;

      if(buffStr) free(buffStr);

      if(samFILE && samFILE != stdin) fclose(samFILE);
      if(outFILE && outFILE != stdout) fclose(outFILE);

      if(idFILE) fclose(idFILE);

      freeSamEntryStack(&samST);

      if(amrHitSTList) freeAmrHitList(amrHitSTList);

      return 2;
} /*lookForAmrsSam*/

