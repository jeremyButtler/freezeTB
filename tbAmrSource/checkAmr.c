/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o st-01: amrHit (.h only)
'     - Holds a linked list of postive amrs for each amr
'       check
'   o fun-01: initAmrHit (.h only)
'     - Initializes an amrHit structuer to all zeros
'   o fun-02: freeAmrHit (.h only)
'     - Frees a single amrHit structure. This does not
'       free a list. For lists, use freeAmrHitList; fun-03
'   o fun-03: freeAmrHitList (.h only)
'     - Frees a list of amrHit structures
'   o fun-04: makeAmr (.h only)
'     - Makes a new, initialized amrHit structer on heap
'   o fun-05: getCigMutCount
'     - Updates the snp (or match)/ins/del counts for a
'   o fun-06: incCigCnt
'     - Incurments the cigar counter when all values for
'   o fun-07: isBactStartCodon
'      - Checks to is if an input codon is an bacterial
'        start codon
'   o fun-08: checkAmrSam
'     - Checks if a sequence in a sam file entry has
'       amr's (antibiotic resitance)
'   o fun-09: pCrossRes
'     - Print out cross resitance
'   o fun-10: pHeadAmrHitList
'     - Prints the header for an amrHitList table
'   o fun-11: pAmrHitList
'     - Prints out all amr's that were in a sequence
'   o fun-12: pHeadAmrs
'     - Prints the header for an amr table (reads instead
'       of consensuses)
'   o fun-13: pAmrs
'     - Prints out all amr's that meant the min depth
'   o fun-14: lookForAmrsSam
'     - Look for anti-microbial (antibiotic) genes in the
'       reads in a sam file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "checkAmr.h"

#include <stdio.h>

#include "amrStruct.h"
#include "drug_str_ary.h"
#include "../generalLib/samEntryStruct.h"

/*Only .h files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/codonTbl.h"
#include "../generalLib/ulCpStr.h"

/*Hidden files
#include "../generalLib/numToStr.h" (.h only)
*/

/*-------------------------------------------------------\
| Fun-05: getCigMutCount
|   - Updates the snp (or match)/ins/del counts for a
|     single cigar entry
| Input:
|   - snpCntI:
|     o snp counter to update
|   - delCntI:
|     o deleition counter to update
|   - insCntI:
|     o insertion counter to update
|   - cigC:
|     o Cigar entry to update for
| Output:
|   - Modifies:
|     o Incurments snpCntI if there is an snp/match
|     o Incurments insCntI if there is an insertion
|     o Incurments delCntI if there is an deletion
\-------------------------------------------------------*/
#define \
getCigMutCount(\
   snpCntI,\
   insCntI,\
   delCntI,\
   cigC\
){\
   (snpCntI) += ((cigC) == 'M');\
   (snpCntI) += ((cigC) == '=');\
   (snpCntI) += ((cigC) == 'X');\
   \
   (insCntI) += ((cigC) == 'I');\
   (delCntI) += ((cigC) == 'D');\
} /*getCigMutCount*/

/*-------------------------------------------------------\
| Fun-06: incCigCnt
|   - Incurments the cigar counter when all values for
|     an cigar entry have been reached
| Input:
|   - cigCntUI:
|     o A counter with the number of bases I have checked
|       for this entry. This is updated with each call.
|   - cigPosI:
|     o Position at in the cigar. This is updated when
|       cigCntUI is > the cigar entry.
|   - samSTPtr:
|     o samEntry structure with the cigar to get an
|       updated position for.
| Output:
|   - Modifies:
|     o Incurments cigCntUI if cigCntUI <  current cigar
|       entry
|     o Sets cigCntUI to 0 if I moved to a new cigar
|       entery (cigCntUI will be > current cigar entry)
|     o Incurments cigPosI if cigCntUI will be > the
|       current cigar entry
\-------------------------------------------------------*/
#define \
incCigCnt(\
   cigCntI,\
   cigPosI,\
   samSTPtr\
){\
   char tmpBl = 0;\
   \
   tmpBl = (cigCntI) < (samST)->cigValAryI[(cigPosI)];\
   (cigPosI) += (! tmpBl);\
   (cigCntI) = ((cigCntI) & tmpBl) + tmpBl;\
}

/*-------------------------------------------------------\
| Fun-07: isBactStartCodon
|   - Checks to is if an input codon is an bacterial start
|     codon
| Input:
|   - firstBaseC:
|     o first base to check; it needs to be the output of
|       an base to code look up table in
|      ../generalLib/codonTbl.h
|   - secBaseC:
|     o second base to check; it needs to be the output
|       of an base to code look up table in
|      ../generalLib/codonTbl.h
|   - thirdBaseC:
|     o third base to check; it needs to be the output
|       of an base to code look up table in
|      ../generalLib/codonTbl.h
| Output:
|   - Returns:
|     o 1 if this is an bacterial start codon
|     o 0 if this is not an bacterial start codon
\-------------------------------------------------------*/
#define \
isBactStartCodon(\
   firstBaseC,\
   secBaseC,\
   thirdBaseC\
)(\
     (  (   (firstBaseC == g_code_codon_tbl)\
          | (firstBaseC == t_code_codon_tbl)\
        )\
      | (firstBaseC == a_code_codon_tbl)\
     ) \
     & ( secBaseC  == t_code_codon_tbl)\
     & (thirdBaseC == g_code_codon_tbl)\
     \
     /*Logic:
     `   - x = firstBaseC == g_code_codon_tbl:
     `     o 1 if the first base is a G
     `     o 0 if not a g
     `   - x |= (firstBaseC == t_code_codon_tbl):
     `     o 1 if the first base is an "G" or "T"
     `     o 0 if not an "G" or "T"
     `   - x |= (firstBaseC == a_code_codon_tbl):
     `     o 1 if the first base is an "G", "T", or "A"
     `     o 0 if not an "G", "T", or "A"
     `     o This covers the first codon for all posible
     `       start codons (ATG, GTG, and TTG)
     `   - The second and thrid base comparisions clear
     `     the bit (set to 0) if I do not have an
     `     TTG, GTG, or ATG codon
     */\
) /*isBackStartCodon*/

/*-------------------------------------------------------\
| Fun-08: checkAmrSam
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
struct amrHit *
checkAmrSam(
   void *samSTPtr,     /*Sequence to check*/
   void *amrSTAryPtr,  /*Has amr's to check*/
   int numAmrI,        /*Length of amrAryST*/
   int *numHitsI,      /*Number amr hits for seq*/
   char *errC          /*For error reporting*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: checkAmrSam
   '   - Checks if a sequence in a sam file entry has
   '     amr's (antibiotic resitance)
   '   o Fun-08 Sec-01:
   '     - Variable declerations
   '   o Fun-08 Sec-02:
   '     - Find the nearest amr to the sequence
   '   o Fun-08 Sec-03:
   '     - Check for AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*For finding the amr position*/
   uint seqPosUI = 0;
   uint refPosUI = 0;

   /*For processing the cigar entries*/
   int numBasesI = 0;  /*Number bases in cigar entry*/
   int cigBaseOnI = 0; /*Number bases left in cig entry*/
   int iCig = 0;       /*Iterate through the cigar*/

   /*For recording the number of snps/ins/dels when
   `   comparing AMRs
   */
   int snpInAmrI = 0;
   int insInAmrI = 0;
   int delInAmrI = 0;

   /*For keeping track of the cigar positions when
   `   comparing AMRs
   */
   int cigPosI = 0;
   int cigCountI = 0;

   /*For checking amr's*/
   uchar *seqUStr = 0;
   uchar *amrUStr = 0;

   int iAmr = 0;       /*Iterate through the amr's list*/
   int iBase = 0;      /*iterate/compare amr pattern*/
   int iAa = 0;        /*I base for amino acid amrs*/
   int iMatch = 0;     /*Holds the last checked base*/

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
   ^ Fun-08 Sec-02:
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
   
   refPosUI = samST->refStartUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Check for AMRs
   ^   o fun-08 sec-03 sub-01:
   ^     - Find the start of the AMR
   ^   o fun-08 sec-03 sub-02:
   ^     - Move to the first base in the amr pattern
   ^   o fun-08 sec-03 sub-03:
   ^     - Check if the amr has a strict match
   ^   o fun-08 sec-03 sub-04:
   ^     - Handle amino acid amr's
   ^   o fun-08 sec-03 sub-05:
   ^     - Check if resistance was found
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-08 Sec-03 Sub-01:
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
         break; /*Finished with the read*/

      /**************************************************\
      * Fun-08 Sec-03 Sub-02:
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
                  goto nextAmr_fun07_sec03_sub07;
                 /*Move to next amr*/

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
      * Fun-08 Sec-03 Sub-03:
      *   - Check if the amr has a strict match
      \**************************************************/

      ++amrAryST[iAmr].numMapReadsUI;
      seqUStr = (uchar *) &samST->seqStr[seqPosUI];

      /*amrUStr is needed for the final checks for both
      `   nucleotide and amino acid checks
      */
      amrUStr = (uchar *) amrAryST[iAmr].amrSeqStr;

      /*Set variables for keeping track of cigar entries*/
      cigPosI = iCig;
      cigCountI = cigBaseOnI;

      /*Set up counters*/
      snpInAmrI = 0;
      insInAmrI = 0;
      delInAmrI = 0;
      iMatch = 0;

      /*check if the amr is from an amino acid change*/
      if(amrAryST[iAmr].amrAaStr != 0)
         goto checkAmr_tbAmr_fun07_sec03_sub04;

      checkSnp_tbAmr_fun07_sec03_sub03:;

      for(
         iBase = 0;
         (amrUStr[iBase] &~32) == (seqUStr[iBase] &~32);
         ++iBase
      ){ /*Loop : See if have extact match to the amr*/
         getCigMutCount(
            snpInAmrI,
            insInAmrI,
            delInAmrI,
            samST->cigTypeStr[cigPosI]
         ); /*Update number of bases in the cigar*/

         /*See if I need to move to next cigar entry*/
         incCigCnt(cigCountI, cigPosI, samST);
      } /*Loop : See if have extact match to the amr*/

      iMatch = iBase; /*This is for the comparison step*/

      /*Check if this is denfinently not an amr*/
      if(amrUStr[iBase] != '\0')
         goto nextAmr_fun07_sec03_sub07;

      goto finishChecks_tbAmr_fun07_sec04_sub05;

      /**************************************************\
      * Fun-08 Sec-03 Sub-04:
      *   - Handle amino acid amr's
      *   o fun-08 sec-03 sub-04 cat-01:
      *     - Check if deletions are resistant
      *   o fun-08 sec-03 sub-04 cat-02:
      *     - Move to the first base in the target codon
      *   o fun-08 sec-03 sub-04 cat-03:
      *     - Check reverse complemnt gene snps/insertions
      *   o fun-08 sec-03 sub-04 cat-04:
      *     - Else I do not know direction, look at snps
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-08 Sec-03 Sub-04 Cat-01: checkAmrSam
      +   - Check if deletions are resistant
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      checkAmr_tbAmr_fun07_sec03_sub04:;

      /*For a full deletion; I have no way of checking
      `   an amino acid sequence. So, I just treat it as
      `   an snp
      */
      if(amrAryST[iAmr].aaDelBl == def_amrST_del)
         goto checkSnp_tbAmr_fun07_sec03_sub03;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-08 Sec-03 Sub-04 Cat-02: checkAmrSam
      +   - Move to the first base in the target codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*This should work out, even when reverse*/
      seqUStr +=
         (int) amrAryST[iAmr].codonPosUI - (int) refPosUI;

      iBase = 0; /*For getting the sequence position*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-08 Sec-03 Sub-04 Cat-03: checkAmrSam
      +   - Check reverse complemnt gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(amrAryST[iAmr].dirFlag == def_amrST_revCompDir)
      { /*If: This is a reverse complement gene*/
         for(
            iAa = 0;
            iAa < amrAryST[iAmr].lenAmrAaUI;
            ++iAa 
         ){ /*Loop: Check the codon reading frame*/

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base1UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base2UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base3UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            aaC = codonLkTbl[base1UC][base2UC][base3UC];
            /*The 2024 WHO catalog has ? marks for
            ` non-insertions/deletions; so I am safe here
            ` for now
            */
          
            if(
                  amrAryST[iAmr].amrAaStr[iAa] == '?'
               && aaC != amrAryST[iAmr].refAaStr[iAa]
            ){ /*If: this could be any codon*/
               if((amrAryST[iAmr].refAaStr[iAa] |32)=='m')
               { /*If: this is a "start" codon*/
                  resBl =
                     isBactStartCodon(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  /*If this was not a start codon*/;
                  if(resBl)
                  { /*If: this was a bacterial start*/
                     resBl = 0;
                     goto nextAmr_fun07_sec03_sub07;
                  } /*If: this was a bacterial start*/
               } /*If: this is a "start" codon*/
            } /*If: this could be any codon*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iAa])
               goto nextAmr_fun07_sec03_sub07;
         } /*Loop: Check the codon reading frame*/

         iMatch = amrAryST[iAmr].lenAmrSeqUI;
      } /*If: This is a reverse complement gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-08 Sec-03 Sub-04 Cat-04: checkAmrSam
      +   - Check forward gene snps/insertions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         amrAryST[iAmr].dirFlag == def_amrST_forwardDir
      ){ /*Else If: This is a foward gene*/
         for(
            iAa = 0;
            iAa < amrAryST[iAmr].lenAmrAaUI;
            ++iAa 
         ){ /*Loop: Check the codon reading frame*/
            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base1UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base2UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun07_sec03_sub07;

            base3UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            aaC = codonLkTbl[base1UC][base2UC][base3UC];

            /*The 2023 WHO catalog has ? marks for
            ` non-insertions/deletions; so I am safe here
            ` for now
            */
            if(
                  amrAryST[iAmr].amrAaStr[iAa] == '?'
               && aaC != amrAryST[iAmr].refAaStr[iAa]
            ){ /*If: this could be any codon*/
               if((amrAryST[iAmr].refAaStr[iAa] |32)=='m')
               { /*If: this is a "start" codon*/
                  resBl =
                     isBactStartCodon(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  /*If this was not a start codon*/;
                  if(resBl)
                  { /*If: this was a bacterial start*/
                     resBl = 0;
                     goto nextAmr_fun07_sec03_sub07;
                  } /*If: this was a bacterial start*/
               } /*If: this is a "start" codon*/
            } /*If: this could be any codon*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iAa])
               goto nextAmr_fun07_sec03_sub07;
         } /*Loop: Check the codon reading frame*/

         iMatch = amrAryST[iAmr].lenAmrSeqUI;
      } /*Else If: This is a foward gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-08 Sec-03 Sub-04 Cat-05: checkAmrSam
      +   - Else I do not know the direction, look at snp
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else  goto checkSnp_tbAmr_fun07_sec03_sub03;

      /**************************************************\
      * Fun-08 Sec-03 Sub-05:
      *   - Make sure there is really resistance
      \**************************************************/

      finishChecks_tbAmr_fun07_sec04_sub05:;

      while(iBase < amrAryST[iAmr].lenAmrSeqUI)
      { /*Loop: to the end of the amr sequence*/
         getCigMutCount(
            snpInAmrI,
            insInAmrI,
            delInAmrI,
            samST->cigTypeStr[cigPosI]
         ); /*Update number of bases in the cigar*/

         /*See if I need to move to next cigar entry*/
         incCigCnt(cigCountI, cigPosI, samST);

         ++iBase;
      } /*Loop: to the end of the amr sequence*/

      while(iBase < amrAryST[iAmr].lenRefSeqUI)
      { /*Loop: to the end of the reference sequence*/
         getCigMutCount(
            snpInAmrI,
            insInAmrI,
            delInAmrI,
            samST->cigTypeStr[cigPosI]
         ); /*Update number of bases in the cigar*/

         /*See if I need to move to next cigar entry*/
         incCigCnt(cigCountI, cigPosI, samST);

         ++iBase;
      } /*Loop: to the end of the reference sequence*/

      /*This if statement will often be false*/
      if(amrUStr[iMatch] == '\0')
      { /*If: I may have an match*/
         /*Check if I have a deletion*/
         resBl =
            (
                 (   amrAryST[iAmr].lenAmrSeqUI
                   < amrAryST[iAmr].lenRefSeqUI
                 )
               & (delInAmrI > 0)
           );

         /*Check if I have an insertion*/
         resBl |=
            (
                 (   amrAryST[iAmr].lenAmrSeqUI
                   > amrAryST[iAmr].lenRefSeqUI
                 )
               & (insInAmrI > 0)
           );

         /*Check if this was an snp*/
         resBl |=
           (
                amrAryST[iAmr].lenAmrSeqUI
              == amrAryST[iAmr].lenRefSeqUI
           );

         resBl &=
            (
                 (insInAmrI + snpInAmrI - delInAmrI)
              == amrAryST[iAmr].lenAmrSeqUI
            );
      } /*If: I may have a match*/

      /**************************************************\
      * Fun-08 Sec-03 Sub-06:
      *   - Check if resistance was found
      \**************************************************/

      if(resBl)
      { /*If: I found a resitance mutation*/
         resBl = 0;
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

      /**************************************************\
      * Fun-08 Sec-03 Sub-07:
      *   - Move to the next amr
      \**************************************************/

      nextAmr_fun07_sec03_sub07:;
   } /*Loop: Check if is an amr or not*/

   return amrSTList;
} /*checkAmr*/

/*-------------------------------------------------------\
| Fun-09: pCrossRes
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
#define \
pCrossRes(amrSTPtr, drugAryStr, outFILE){\
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
| Fun-10: pHeadAmrHitList
|   - Prints the header for an amrHitList table
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrHitList table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrHitList(
   void *outFILE
){
   fprintf((FILE *) outFILE, "Id\tgene\tdrug");
   fprintf((FILE *) outFILE, "\tcrossResitance");
   fprintf((FILE *) outFILE, "\tvariantId\ttype");
   fprintf((FILE *) outFILE, "\trefPos\tseqPos");
   fprintf((FILE *) outFILE, "\tresitanceLevel");
   fprintf((FILE *) outFILE, "\tresistanceAdditive");
   fprintf((FILE *) outFILE, "\tneedsGene");
   fprintf((FILE *) outFILE, "\teffect\twhoComment\n");
} /*pHeadAmrHitList*/

/*-------------------------------------------------------\
| Fun-11: pAmrHitList
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
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's in amrHitSTListPtr to outFILE
\-------------------------------------------------------*/
void
pAmrHitList(
   char *seqIdStr,
   struct amrHit *amrHitSTListPtr,
   char *drugAryStr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC: pAmrHitList
   '   - Prints out the AMRs that were in a sequence. This
   '     is for consensuses
   '   o fun-11 sec-01:
   '     - variable decerations
   '   o fun-11 sec-02:
   '     - Print AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-01:
   ^   - variable decerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct amrHit *tmpST = (amrHitSTListPtr);
   struct amrHit *lastST = 0;
   char matchBl = 0;

   ulong amrFlagUL = 0;
   int flagOnI = 0;
   char *drugStr = 0;
   char firstPrintMacBl = 0;
   int iAmrMac = 0;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^   - Print AMRs
   ^   o fun-11 sec-02 sub-01:
   ^     - Print AMRs/check if already printed the variant
   ^   o fun-11 sec-02 sub-02:
   ^     - Print the read/gene ids
   ^   o fun-11 sec-02 sub-03:
   ^     - Print out the Antibiotics resitant to
   ^   o fun-11 sec-02 sub-04:
   ^     - Print out the cross resistance
   ^   o fun-11 sec-02 sub-05:
   ^     - Print out the variant id, mutation type,
   ^       and positions
   ^   o fun-11 sec-02 sub-06:
   ^     - Print out if I have high, unkown (normal?),
   ^       or low resitance
   ^   o fun-11 sec-02 sub-07:
   ^     - Print out if the low resitance is additive
   ^   o fun-11 sec-02 sub-08:
   ^     - Print out if the restance needs a functional
   ^       gene
   ^   o fun-11 sec-02 sub-09:
   ^     - Print out the effect of the  mutation
   ^   o fun-11 sec-02 sub-10:
   ^     - Print out the WHO's comment
   ^   o fun-11 sec-02 sub-11:
   ^     - Move onto the next AMR
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-11 Sec-02 Sub-01:
   *   - Print AMRs/check if already printed the variant
   \*****************************************************/

   while(tmpST)
   { /*Loop: Print out all amr's*/

      if(lastST)
      { /*If: I need to check previous results*/
         matchBl =
            cStrEql(
               tmpST->amrST->varIdStr,
               lastST->amrST->varIdStr,
               '\0'
            );

         lastST = tmpST;
         tmpST = tmpST->nextAmr;

         if(! matchBl) continue;
      } /*If: I need to check previous results*/

      else lastST = tmpST;

      /**************************************************\
      * Fun-11 Sec-02 Sub-02:
      *   - Print the read/gene ids
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "%s\t%s\t",
        (seqIdStr),              /*Name of the seq*/
        tmpST->amrST->geneIdStr /*Gene name*/
      ); /*Pirnt out gene id and drug*/
      
      /**************************************************\
      * Fun-11 Sec-02 Sub-03:
      *   - Print out the Antibiotics resitant to
      \**************************************************/

      flagOnI = 0;
      firstPrintMacBl = 1;
      
      for(
         iAmrMac = 0;
         iAmrMac < def_amrST_maxDrugs;
         ++iAmrMac
      ){ /*Loop: Run through each set of flags*/
         amrFlagUL = tmpST->amrST->amrFlagsUL[iAmrMac];
         
         if(! amrFlagUL) continue; /*no resitance*/

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
               fprintf((FILE *) outFILE, "%s", drugStr);
               firstPrintMacBl = 0;
            } /*If: this is the frist printout*/
            
            else fprintf((FILE *) outFILE, "_%s", drugStr);
            
            ++flagOnI;
            amrFlagUL >>= 1;
         } /*Loop: Checn each flag in a set*/
      } /*Loop: Run through each set of flags*/
      
      /**************************************************\
      * Fun-11 Sec-02 Sub-04:
      *   - Print out the cross resistance
      \**************************************************/

      fprintf((FILE *) outFILE, "\t");

      pCrossRes(
         tmpST->amrST,(drugAryStr),
         (FILE *) outFILE
      );
      
      /**************************************************\
      * Fun-11 Sec-02 Sub-05:
      *   - Print out the variant id, mutation type,
      *     and positions
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i",
        tmpST->amrST->varIdStr,  /*Variant id*/
        tmpST->amrST->mutTypeStr, /*snp/del/ins/LoF*/
        (int) tmpST->amrST->refPosUI,  /*Position on ref*/
        (int) tmpST->seqPosUI          /*Position on seq*/
      ); /*Print out the variant id and type*/

         /*I am printing the reference and sequence
         `   position as signed integers because they
         `   will never go over 2 billion and this will
         `   avoid compatibilty issues with plan9's
         `   fprintf function. I just need to assign
         `   fprintf to printf
         */

      /**************************************************\
      * Fun-11 Sec-02 Sub-06:
      *   - Print out if I have high, unkown (normal?),
      *     or low resitance
      \**************************************************/

      if(tmpST->amrST->highResBl)
         fprintf((FILE *) outFILE, "\thigh");

      else if(tmpST->amrST->lowResBl)
         fprintf((FILE *) outFILE, "\tlow");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-11 Sec-02 Sub-07:
      *   - Print out if the low resitance is additive
      \**************************************************/

      if(tmpST->amrST->additiveResBl)
         fprintf((FILE *) outFILE, "\tAdditive");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-11 Sec-02 Sub-08:
      *   - Print out if the restance needs a functional
      *     gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         tmpST->amrST->needsGeneStr
      ); /*Print out if needs a functional gene*/

      /**************************************************\
      * Fun-11 Sec-02 Sub-09:
      *   - Print out the effect of the  mutation
      \**************************************************/

      if(tmpST->amrST->effectStr)
         fprintf(
            (FILE *) outFILE,
            "\t%s",
            tmpST->amrST->effectStr
         );
      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-11 Sec-02 Sub-10:
      *   - Print out the WHO's comment
      \**************************************************/
      
      if(tmpST->amrST->commentStr)
         fprintf(
            (FILE *) outFILE,
            "\t%s\n",
            tmpST->amrST->commentStr
         );
      else fprintf((FILE *) outFILE, "\tNA\n");

      /**************************************************\
      * Fun-11 Sec-02 Sub-11:
      *   - Move onto the next AMR
      \**************************************************/
      
      tmpST = tmpST->nextAmr;
   } /*Loop: Print out all amr's*/
} /*pAmrHitList*/

/*-------------------------------------------------------\
| Fun-12: pHeadAmrs
|   - Prints the header for an amr table (reads instead
|     of consensuses)
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrs(
   void *outFILE
){
   fprintf((FILE *) outFILE,"\tgene\tDrug");
   fprintf((FILE *) outFILE,"\tcrossResistance");
   fprintf((FILE *) outFILE,"\tvariantId\ttype");
   fprintf((FILE *) outFILE,"\trefPos");
   fprintf((FILE *) outFILE,"\tnumSupportingRead");
   fprintf((FILE *) outFILE, "\tnumMappedReads");
   fprintf((FILE *) outFILE, "\tresitanceLevel");
   fprintf((FILE *) outFILE, "\tresistanceAdditive");
   fprintf((FILE *) outFILE, "\tneedsGene");
   fprintf((FILE *) outFILE,"\teffect\twhoComment\n");
} /*pHeadAmrs*/

/*-------------------------------------------------------\
| Fun-13: pAmrs
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
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's that meet the min stats and have at
|       least one mapped read
\-------------------------------------------------------*/
void
pAmrs(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   unsigned int totalReadsUI,
   struct amrStruct *amrSTAry,
   unsigned int numAmrsUI,
   char *drugAryStr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-13 TOC: pAmrs
   '   o fun-13 sec-01:
   '     - Variable declerations
   ^   - Start loop and filter AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint indexUI = 0;
   uint lastPrintIndexUI = 0;
   float percSupF = 0;
   ulong amrFlagUL = 0;
   int flagOnI = 0;
   char *drugStr = 0;
   int iAmrMac = 0;
   char firstPrintMacBl = 1;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-02:
   ^   - Start loop and filter AMRs
   ^   o fun-13 sec-02 Sub-01:
   ^     - Start loop and remove low depth AMRs
   ^   o fun-13 sec-02 Sub-02:
   ^     - Remove AMRs with a lower % of mapped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-13 Sec-02 Sub-01:
   *   - Startloop and remove low depth AMRs
   \*****************************************************/

   for(indexUI = 0; indexUI < (numAmrsUI); ++indexUI)
   { /*Loop:Check and print out amrs*/

      if((amrSTAry)[indexUI].numSupReadsUI <(minDepthUI))
         continue;
      
      if((amrSTAry)[indexUI].numMapReadsUI ==0) continue;

      /**************************************************\
      * Fun-13 Sec-02 Sub-02:
      *   - Remove AMRs with a lower % of mapped reads
      \**************************************************/
      
      percSupF =
           ((float) (amrSTAry)[indexUI].numMapReadsUI)
         / (totalReadsUI);
      
      if(percSupF < (minPercTotalF)) continue;
      
      percSupF =
           ((float) (amrSTAry)[indexUI].numSupReadsUI)
         / ((float)(amrSTAry)[indexUI].numMapReadsUI);
      
      if(percSupF < (minPercMapF)) continue;

       if(indexUI > 0)
       { /*If: I need to check indexs*/
          if( !
              cStrEql(
                 amrSTAry[indexUI].varIdStr,
                 amrSTAry[lastPrintIndexUI].varIdStr,
                 '\0'
              ) /*Check if I have printed this id*/
          ) continue; /*Id already printed*/
       } /*If: I need to check indexs*/
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun-13 Sec-03:
      ^   - 
      ^   o fun-13 sec-03 Sub-01:
      ^     - Print out the gene id
      ^   o fun-13 sec-03 Sub-02:
      ^     - Print out the antibiotics resitant to
      ^   o fun-13 sec-03 Sub-03:
      ^     - Print out antibiotics cross resistanct to
      ^   o fun-13 sec-03 Sub-04:
      ^     - Print out variant id, mutation, reference
      ^       position, number reads supporting AMR and
      ^       total reads at reference position.
      ^   o fun-13 sec-03 Sub-05:
      ^     - Print out if I have high, unkown (normal?),
      ^       or low resitance
      ^   o fun-13 sec-03 Sub-06:
      ^     - Print out if the low resitance is additive
      ^   o fun-13 sec-03 Sub-07:
      ^     - Print out if the restance needs a functional
      ^       gene
      ^   o fun-13 sec-03 Sub-08:
      ^     - Print out the mutation effect
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      
      /**************************************************\
      * Fun-13 Sec-03 Sub-01:
      *   - Print out the gene id
      \**************************************************/

      lastPrintIndexUI = indexUI;

      fprintf(
        (FILE *) outFILE,
        "%s\t",
        (amrSTAry)[indexUI].geneIdStr /*gene id*/
      ); /*Pirnt out gene id and drug*/
      
      /**************************************************\
      * Fun-13 Sec-03 Sub-02:
      *   - Print out the antibiotics resitant to
      \**************************************************/

      flagOnI = 0;
      firstPrintMacBl = 1;
      
      for(
         iAmrMac = 0;
         iAmrMac < def_amrST_maxDrugs;
         ++iAmrMac
      ){ /*Loop: Run through each set of flags*/
         amrFlagUL =
            (amrSTAry)[indexUI].amrFlagsUL[iAmrMac];
         
         if(! amrFlagUL) continue; /*no resitance*/

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
               fprintf((FILE *) outFILE, "%s", drugStr);
               firstPrintMacBl = 0;
            } /*If: this is the frist printout*/
            
            else fprintf((FILE *) outFILE,"_%s",drugStr);
            
            ++flagOnI;
            amrFlagUL >>= 1;
         } /*Loop: Checn each flag in a set*/
      } /*Loop: Run through each set of flags*/
         
      /**************************************************\
      * Fun-13 Sec-03 Sub-03:
      *   - Print out the antibiotics cross resistanct to
      \**************************************************/

      /*There is resistance, print it out*/
      fprintf((FILE *) outFILE, "\t");
      
      pCrossRes(
         &(amrSTAry)[indexUI],
        (drugAryStr),
        (FILE *) outFILE
      ); /*Print out cross-resistance drugs*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-04:
      *   - Print out variant id, mutation, reference
      *     position, number reads supporting AMR and
      *     total reads at reference position.
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i\t%i",
        amrSTAry[indexUI].varIdStr,   /*Variant id*/
        amrSTAry[indexUI].mutTypeStr, /*snp/del/ins/LoF*/
        (int) amrSTAry[indexUI].refPosUI,
        (int) amrSTAry[indexUI].numSupReadsUI,
        (int) amrSTAry[indexUI].numMapReadsUI
      ); /*Print out the variant id and type*/

         /*I am printing the reference position as an
         `   signed integer because it will never go
         `   over 2 billion and this will avoid
         `   compatibilty issues with plan9's
         `   fprintf function. I just need to assign
         `   fprintf to printf
         */
      
      /**************************************************\
      * Fun-13 Sec-03 Sub-05:
      *   - Print out if I have high, unkown (normal?),
      *     or low resitance
      \**************************************************/

      if(amrSTAry[indexUI].highResBl)
         fprintf((FILE *) outFILE, "\thigh");

      else if(amrSTAry[indexUI].lowResBl)
         fprintf((FILE *) outFILE, "\tlow");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-13 Sec-03 Sub-06:
      *   - Print out if the low resitance is additive
      \**************************************************/

      if(amrSTAry[indexUI].additiveResBl)
         fprintf((FILE *) outFILE, "\tAdditive");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-13 Sec-03 Sub-07:
      *   - Print out if the restance needs a functional
      *     gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         amrSTAry[indexUI].needsGeneStr
      ); /*Print out if needs a functional gene*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-08:
      *   - Print out the mutation effect
      \**************************************************/
      
      if((amrSTAry)[indexUI].effectStr)
         fprintf(
            (FILE *) outFILE,
            "\t%s",
            (amrSTAry)[indexUI].effectStr
         );
      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-13 Sec-03 Sub-09:
      *   - Print out the WHOs comment
      \**************************************************/
      
      if((amrSTAry)[indexUI].commentStr)
      { /*If: I have a who comment*/
         fprintf(
            (FILE *) outFILE,
            "\t%s\n",
            (amrSTAry)[indexUI].commentStr
         );
      } /*If: I have a who comment*/
      
      else fprintf((FILE *) outFILE, "\tNA");
   } /*Loop:Check and print out amrs*/\
} /*pAmrs*/

/*-------------------------------------------------------\
| Fun-14: lookForAmrsSam
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
char
lookForAmrsSam(
   void *amrSTAryPtr, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   char *drugAryStr, /*Has antibiotic names*/
   char readsBl,     /*1: processing reads not cons*/
   unsigned int minDepthUI,  /*Min depth to keep amr*/
   float minPercMapF,/*Min % support to keep amr (read)*/
   float minPercTotalF, /*Min % mapped reads to keep*/
   char *samStr,    /*Sam file with reads to check*/
   char *outStr,
   char *idPrefStr    /*Prefix for id files*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-14 TOC:P lookForAmrsSam
   '   - Look for anti-microbial (antibiotic) genes in the
   '     reads in a sam file
   '   o fun-14 sec-01:
   '     - Variable declerations
   '   o fun-14 sec-02:
   '     - Get the first sam entry
   '   o fun-14 sec-03:
   '     - Check for AMRs
   '   o fun-14 sec-04:
   '     - Print out read AMR stats
   '   o fun-14 sec-05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *buffStr = 0;   /*Buffer for readSamLine*/
   ulong lenBuffUL = 0; /*Current length of buffStr*/

   char errC = 0;         /*For error checking*/
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
   ^ Fun-14 Sec-02:
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

   if(! outStr || *outStr == '-') outFILE = stdout;

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
   ^ Fun-14 Sec-03:
   ^   - Check for AMRs
   ^   o fun-14 sec-03 sub-01:
   ^     - Filter out less usefull entries
   ^   o fun-14 sec-03 sub-02:
   ^     - Check for amrs
   ^   o fun-14 sec-03 sub-03:
   ^     - Print out consensus sequence AMRS
   ^   o fun-14 sec-03 sub-04:
   ^     - Deal with read amrs; print ids if requested/
   ^       free consensus structuerrs
   ^   o fun-14 sec-03 sub-05:
   ^     - Move to the next sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-14 Sec-03 Sub-01:
   *   - Filter out less usefull entries/start loop
   \*****************************************************/

   if(! readsBl) pHeadAmrHitList(outFILE);
   else pHeadAmrs(outFILE);

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
      if(samST.flagUS & (4 | 256 | 2048))
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
      * Fun-14 Sec-03 Sub-02:
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
      * Fun-14 Sec-03 Sub-03:
      *   - Print out consensus sequence AMRS
      \**************************************************/

      /*Check if I can print out the amr's*/
      if( (!! amrHitSTList) & (! readsBl) )
      { /*If: There were amr's*/
          pAmrHitList(
             samST.qryIdStr,
             amrHitSTList,
             (drugAryStr),
             (outFILE)
          ); /*Print the amr entry*/

          freeAmrHitList(amrHitSTList);
      } /*If: There were amr's*/

      /**************************************************\
      * Fun-14 Sec-03 Sub-04:
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
      * Fun-14 Sec-03 Sub-05:
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
   ^ Fun-14 Sec-04:
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
         outFILE
      ); /*Print out the AMRs*/
   } /*If: I mapped reads, not consensuses*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-05:
   ^   - Clean up
   ^   o fun-14 sec-05 sub-01:
   ^     - Clean up after a successful run
   ^   o fun-14 sec-05 sub-02:
   ^     - Clean up after a memory error
   ^   o fun-14 sec-05 sub-03:
   ^     - Clean up after an file error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-14 Sec-05 Sub-01:
   *   - Clean up after a successful run
   \*****************************************************/

   if(outFILE != stdout) fclose(outFILE);

   if(idFILE) fclose(idFILE);

   return 0;

   /*****************************************************\
   * Fun-14 Sec-05 Sub-02:
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
   * Fun-14 Sec-05 Sub-03:
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

