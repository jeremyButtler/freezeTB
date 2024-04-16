/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o .h st-01: amrHit
'     - Holds a linked list of postive amrs for each amr
'       check
'   o .h fun-01: initAmrHit
'     - Initializes an amrHit structuer to all zeros
'   o fun-02: freeAmrHit
'     - Frees a single amrHit structure. This does not
'       free a list. For lists, use freeAmrHitList; fun-03
'   o fun-03: freeAmrHitList
'     - Frees a list of amrHit structures
'   o fun-04: makeAmr
'     - Makes a new, initialized amrHit structer on heap
'   o .c fun-05: getCigMutCount
'     - Updates the snp (or match)/ins/del counts for a
'   o .c fun-06: incCigCnt
'     - Incurments the cigar counter when all values for
'   o .c fun-07: LoFForwardCheck
'     - Gets the number of indels in an gene and checks
'       for early stops or missing stops and starts.
'   o .c fun-08: LoFReverseCheck
'     - Gets the number of indels in an gene and checks
'       for early stops or missing stops and starts. For
'       an reverse complement gene
'   o fun-09: checkAmrSam
'     - Checks if a sequence in a sam file entry has
'       amr's (antibiotic resitance)
'   o .c fun-10: pCrossRes
'     - Print out cross resitance
'   o fun-11: pHeadAmrHitList
'     - Prints the header for an amrHitList table
'   o fun-12: pAmrHitList
'     - Prints out all amr's that were in a sequence
'   o fun-13: pHeadAmrs
'     - Prints the header for an amr table (reads instead
'       of consensuses)
'   o fun-14: pAmrs
'     - Prints out all amr's that meant the min depth
'   o fun-15: pHeadAmrReadIds
'     - Prints out the header for the read id mapped
'       variant file
'   o fun-16: pAmrReadIds
'     - Prints out the read id and the ARMs variant ids
'       that it supported
'   o fun-17: lookForAmrsSam
'     - Look for anti-microbial (antibiotic) genes in the
'       reads in a sam file
'   o license:
'     - Licensing for this code (public domain / mit)
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
`  - has an .c file
`  - has .h file only
`    o #include "../generalLib/numToStr.h"
`    o #include "../generalLib/base10StrToNum.h"
*/

/*-------------------------------------------------------\
| Fun-02: freeAmrHit
|   - Frees a single amrHit structure. This does not free
|     a list. For lists, use freeAmrHitList (fun-03)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
\-------------------------------------------------------*/
void
freeAmrHit(
   struct amrHit *amrHitSTPtr
){
   free(amrHitSTPtr);
} /*freeAmrHit*/

/*-------------------------------------------------------\
| Fun-03: freeAmrHitList
|   - Frees a list of amrHit structures
| Input:
|   - amrHitSTListPtr:
|     o pointer to start of an amrHist structure list to
|       free
| Output:
|   - Frees:
|     o All amrHits structures in amrHitSTPtr
\-------------------------------------------------------*/
void
freeAmrHitList(
   struct amrHit *amrHitSTListPtr
){
   struct amrHit *tmpST = amrHitSTListPtr;
   struct amrHit *nextHitST = 0;

   while(tmpST)
   { /*Loop: Free all amrHit structures in list*/
      nextHitST = tmpST->nextAmr;
      freeAmrHit(tmpST);
      tmpST = nextHitST;
   } /*Loop: Free all amrHit structures in list*/
} /*freeAmrHit*/

/*-------------------------------------------------------\
| Fun-04: makeAmr
|   - Makes a new, initialized amrHit structer on heap
| Input:
| Output:
|   - Returns:
|     o A pointer to the new amrHit structure
|     o 0 for memory error
\-------------------------------------------------------*/
struct amrHit *
makeAmrHit(
){
   struct amrHit *retST = malloc(sizeof(struct amrHit));
   if(retST) initAmrHit(retST);
   return retST;
} /*makeAmrHit*/

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
   tmpBl = (cigCntI) > 0;\
   cigCntI -= tmpBl;\
   cigPosI += (! cigCntI); /*Only incurment at 0*/\
}

/*-------------------------------------------------------\
| Fun-07: LoFForwardCheck
|   - Gets the number of indels in an gene and checks for
|     early stops or missing stops and starts.
| Input:
|   - startGeneSI:
|     o Index (0) of the first reference base in the gene
|   - endGeneSI:
|     o Index (0) of the last reference base in the gene
|   - refPosSI:
|     o Position at in the reference genome
|   - readPosSI:
|     o Position at in the reads sequence
|   - samSTPtr:
|     o Pointer to an samEntry structure with the sequence
|       having the gene to check
|   - cigPosSI:
|     o Position at in the cigar
|   - cigBaseSI:
|     o Base on in the current cigar entry
|   - LoFMacBl:
|     o Marks if an start codon or stop codon was lost.
|       This does not mark if there is one afterwards
|   - indelCntI:
|     o Integer/long to hold the number of indels
| Output:
|   - Modifies:
|     o indelCntI to hold the number of indels
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
#define \
LoFForwardCheck(\
   startGeneSI,\
   endGeneSI,\
   refPosSI,\
   readPosSI,\
   samSTPtr,\
   cigPosSI,\
   cigBaseSI,\
   indelCntSI\
)({/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-07 TOC: LoFForwardCheck\
   '   - Gets the number of indels in an gene and checks\
   '     for early stops or missing stops and starts.\
   '   o fun-07 sec-01:\
   '     - Variable declerations\
   '   o fun-07 sec-02:\
   '     - Move to position and check start codon\
   '   o fun-07 sec-03:\
   '     - Check reading frame for LoFs & get indel count\
   '   o fun-07 sec-03:\
   '     - Return the answer for the LoF entry\
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-01:\
   ^   - Variable declerations\
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   uint refPosMacUI = (uint) (refPosSI);\
   uint readPosMacUI = (uint) (readPosSI);\
   int siCigMac = (cigPosSI);\
   int baseOnMacSI = (cigBaseSI);\
   int extraNtMacSI = 0; /*Extra bases from last entry*/\
   \
   char aaC = 0;\
   char LoFMacBl = 0;\
   char haveStartBl = 0;\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-02:\
   ^   - Move to position and check start codon\
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   samEntryFindRefPos(\
     (samSTPtr),\
     siCigMac,\
     baseOnMacSI,\
     (startGeneSI),\
     refPosMacUI,\
     readPosMacUI\
   ); /*Find the first base in the gene\
            ` (fun-10 samEntryStruct.h)\
            */\
   \
   (indelCntSI) = 0;\
   \
   if(refPosMacUI == (startGeneSI))\
   { /*If: I have the first base in the gene*/\
      if((samSTPtr)->cigTypeStr[siCigMac] == 'D')\
         LoFMacBl = 1;\
       \
       else\
       { /*Else: This is not an deletion*/\
          LoFMacBl =\
             ! (\
               bacteriaStart_check(\
                 (samSTPtr)->seqStr[readPosMacUI],\
                 (samSTPtr)->seqStr[readPosMacUI + 1],\
                 (samSTPtr)->seqStr[readPosMacUI + 2]\
                )\
             ); /*Check if I have an start codon*/\
       } /*Else: This is not an deletion*/\
       \
       haveStartBl = 1;\
   } /*If: I have the first base in the gene*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-03:\
   ^   - Check reading frame for LoFs and get indel count\
   ^   o fun-07 sec-03 sub-01:\
   ^     - Start loop and handle insertion/mask cases\
   ^   o fun-07 sec-03 sub-02:\
   ^     - Get counts for the deletions\
   ^   o fun-07 sec-03 sub-03:\
   ^     - Check snps/matchs for early stops\
   ^   o fun-07 sec-03 sub-04:\
   ^     - Move to the next cigar entry\
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   /*****************************************************\
   * Fun-07 Sec-03 Sub-01:\
   *   - Start loop and handle insertion/mask cases\
   \****************************************************/\
   \
   while(refPosMacUI < (endGeneSI))\
   { /*Loop: Count indels/check for LoFs (foward)*/\
      switch((samSTPtr)->cigTypeStr[siCigMac])\
      { /*Switch: Check cigar entry type*/\
         case 'S':\
         /*Case: softmask; likely from primers/adapter*/\
            readPosMacUI += baseOnMacSI;\
            ++siCigMac;\
            break;\
         /*Case: softmask; likely from primers/adapter*/\
         \
         case 'I':\
         /*Case: Insertions*/\
            refPosMacUI += baseOnMacSI;\
            (indelCntSI) += baseOnMacSI;\
            extraNtMacSI += baseOnMacSI;\
            \
            if(extraNtMacSI < 3)\
               break; /*Not enough bases for codon*/\
            \
            if(haveStartBl)\
            { /*If: had an start, check for LoFs*/\
               while(extraNtMacSI > 2)\
               { /*Loop: Check indel reading frame*/\
                  aaC =\
                    codonToAA(\
                       samSTPtr->seqStr[readPosMacUI],\
                       samSTPtr->seqStr[readPosMacUI +1],\
                       samSTPtr->seqStr[readPosMacUI +2]\
                    );\
                  \
                  readPosMacUI += 3;\
                  \
                  /*Check if I had an early stop*/\
                  LoFMacBl |= (aaC== '*');\
                  extraNtMacSI -= 3;\
               } /*Loop: Check indel reading frame*/\
            } /*If: had an start, check for LoFs*/\
            \
            ++siCigMac;\
            break;\
         /*Case: Insertions*/\
         \
         /***********************************************\
         * Fun-07 Sec-03 Sub-02:\
         *   - Get counts for the deletions\
         \**********************************************/\
         \
         case 'D':\
         /*Case: Deletions*/\
            refPosMacUI += baseOnMacSI;\
            \
            /*Check if I have overshot*/\
            if(refPosMacUI > (endGeneSI))\
            { /*If: I have overshot the gene end*/\
               (indelCntSI) += refPosMacUI - (endGeneSI);\
                LoFMacBl = 1;/*Stop removed*/\
            } /*If: I have overshot the gene end*/\
            \
            else\
               (indelCntSI) += baseOnMacSI;\
            \
            ++siCigMac;\
            break;\
         /*Case: Deletions*/\
         \
         /***********************************************\
         * Fun-07 Sec-03 Sub-03:\
         *   - Check snps/matchs for early stops\
         \**********************************************/\
         \
         case 'M':\
         case 'X':\
         case '=':\
         /*Case: Matchs and snps*/\
            refPosMacUI += baseOnMacSI;\
            extraNtMacSI += baseOnMacSI;\
            \
            /*Check if I overshot the gene*/\
            if(refPosMacUI > (endGeneSI))\
               extraNtMacSI -= (refPosMacUI-(endGeneSI));\
            \
            if(haveStartBl)\
            { /*If: had an start, check for LoFs*/\
               while(extraNtMacSI > 2)\
               { /*Loop: Check indel reading frame*/\
                  aaC =\
                    codonToAA(\
                      samSTPtr->seqStr[readPosMacUI],\
                      samSTPtr->seqStr[readPosMacUI + 1],\
                      samSTPtr->seqStr[readPosMacUI + 2]\
                    );\
                  \
                  readPosMacUI += 3;\
                  extraNtMacSI -= 3;\
                  \
                  if(extraNtMacSI < 3) /*last codon*/\
                     break; /*Check outside the loop*/\
                  \
                  /*Check if I had an early stop*/\
                  LoFMacBl |= (aaC== '*');\
               } /*Loop: Check indel reading frame*/\
            } /*If: had an start, check for LoFs*/\
            \
            /*Check if this is the last base*/\
            if(refPosMacUI >= (endGeneSI))\
               LoFMacBl |= (aaC != '*');\
            else\
               LoFMacBl |= (aaC== '*');\
            \
            ++siCigMac;\
            break;\
         /*Case: Matchs and snps*/\
      } /*Switch: Check cigar entry type*/\
      \
      /**************************************************\
      * Fun-07 Sec-03 Sub-04:\
      *   - Move to the next cigar entry\
      \*************************************************/\
      \
      if(siCigMac >= (samSTPtr)->lenCigUI)\
         break; /*End of sequence*/\
      \
      baseOnMacSI = (samSTPtr)->cigValAryI[siCigMac];\
   } /*Loop: Count indels/check for LoFs (foward)*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-07 Sec-04:\
   ^   - Return the answer for the LoF entry\
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   LoFMacBl;\
}) /*LoFForwardCheck*/

/*-------------------------------------------------------\
| Fun-08: LoFReverseCheck
|   - Gets the number of indels in an gene and checks for
|     early stops or missing stops and starts. This is for
|     an reverse complement gene
| Input:
|   - startGeneSI:
|     o Index (0) of the first reference base in the gene
|   - endGeneSI:
|     o Index (0) of the last reference base in the gene
|   - refPosSI:
|     o Position at in the reference genome
|   - readPosSI:
|     o Position at in the reads sequence
|   - samSTPtr:
|     o Pointer to an samEntry structure with the sequence
|       having the gene to check
|   - cigPosSI:
|     o Position at in the cigar
|   - cigBaseSI:
|     o Base on in the current cigar entry
|   - LoFMacBl:
|     o Marks if an start codon or stop codon was lost.
|       This does not mark if there is one afterwards
|   - indelCntI:
|     o Integer/long to hold the number of indels
| Output:
|   - Modifies:
|     o indelCntI to hold the number of indels
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
#define \
LoFReverseCheck(\
   startGeneSI,\
   endGeneSI,\
   refPosSI,\
   readPosSI,\
   samSTPtr,\
   cigPosSI,\
   cigBaseSI,\
   indelCntSI\
)({\
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: LoFReverseCheck
   '   - Gets the number of indels in an gene and checks
   '     for early stops or missing stops and starts.
   '   - This is for an reverse complement gene
   '   o fun-08 sec-01:
   '     - Variable declerations
   '   o fun-08 sec-02:
   '     - Move to position and check start codon
   '   o fun-08 sec-03:
   '     - Check reading frame for LoFs & get indel count
   '   o fun-08 sec-03:
   '     - Return the answer for the LoF entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   uint refPosMacUI = (uint) (refPosSI);\
   uint readPosMacUI = (uint) (readPosSI);\
   int siCigMac = (cigPosSI);\
   int baseOnMacSI = (cigBaseSI);\
   int extraNtMacSI = 0; /*Extra bases from last entry*/\
   \
   char aaC = 0;\
   char LoFMacBl = 0;\
   char haveEndBl = 0; /*Marks I have end of the gene*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-02:
   ^   - Move to position and check start codon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   samEntryFindRefPos(\
     (samSTPtr),\
     siCigMac,\
     baseOnMacSI,\
     (startGeneSI),\
     refPosMacUI,\
     readPosMacUI\
   ); /*Find the first base in the gene
            ` (fun-10 samEntryStruct.h)
            */\
   \
   (indelCntSI) = 0;\
   \
   if(refPosMacUI == (startGeneSI))\
   { /*If: I have the last base in the gene*/\
      if((samSTPtr)->cigTypeStr[siCigMac] == 'D')\
        LoFMacBl = 1; /*Is an indel*/\
      \
      else\
      { /*Else: This is not an deletion*/\
         aaC =\
            revCompCodonToAA(\
               (samSTPtr)->seqStr[readPosMacUI],\
               (samSTPtr)->seqStr[readPosMacUI + 1],\
               (samSTPtr)->seqStr[readPosMacUI + 2]\
            ); /*Get the last codon*/\
         \
         /*See if the last codon is an stop*/\
         LoFMacBl = aaC != '*';\
      } /*Else: This is not an deletion*/\
      \
      haveEndBl = 1;\
   } /*If: I have the last base in the gene*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-03:
   ^   - Check reading frame for LoFs and get indel count
   ^   o fun-08 sec-03 sub-01:
   ^     - Start loop and handle insertion/mask cases
   ^   o fun-08 sec-03 sub-02:
   ^     - Get counts for the deletions
   ^   o fun-08 sec-03 sub-03:
   ^     - Check snps/matchs for early stops
   ^   o fun-08 sec-03 sub-04:
   ^     - Move to the next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   /*****************************************************\
   * Fun-08 Sec-03 Sub-01:
   *   - Start loop and handle insertion/mask cases
   \****************************************************/\
   \
   while(refPosMacUI < (endGeneSI))\
   { /*Loop: Count indels/check for LoFs (foward)*/\
      switch((samSTPtr)->cigTypeStr[siCigMac])\
      { /*Switch: Check cigar entry type*/\
         case 'S':\
         /*Case: softmask; likely from primers/adapter*/\
            readPosMacUI += baseOnMacSI;\
            ++siCigMac;\
            break;\
         /*Case: softmask; likely from primers/adapter*/\
         \
         case 'I':\
         /*Case: Insertions*/\
            refPosMacUI += baseOnMacSI;\
            (indelCntSI) += baseOnMacSI;\
            extraNtMacSI += baseOnMacSI;\
            \
            if(extraNtMacSI < 3)\
               break; /*Not enough bases for codon*/\
            \
            if(haveEndBl)\
            { /*If: had an end, check for LoFs*/\
               while(extraNtMacSI > 2)\
               { /*Loop: Check indel reading frame*/\
                  aaC =\
                    revCompCodonToAA(\
                      samSTPtr->seqStr[readPosMacUI],\
                      samSTPtr->seqStr[readPosMacUI + 1],\
                      samSTPtr->seqStr[readPosMacUI + 2]\
                    );\
                  \
                  readPosMacUI += 3;\
                  \
                  /*Check if I had an early stop*/\
                  LoFMacBl |= (aaC== '*');\
                  extraNtMacSI -= 3;\
               } /*Loop: Check indel reading frame*/\
            } /*If: had an end, check for LoFs*/\
            \
            ++siCigMac;\
            break;\
         /*Case: Insertions*/\
         \
         /***********************************************\
         * Fun-08 Sec-03 Sub-02:
         *   - Get counts for the deletions
         \**********************************************/\
         \
         case 'D':\
         /*Case: Deletions*/\
            refPosMacUI += baseOnMacSI;\
            \
            /*Check if I have overshot*/\
            if(refPosMacUI > (endGeneSI))\
            { /*If: I have overshot the gene end*/\
               (indelCntSI) += refPosMacUI - (endGeneSI);\
                LoFMacBl = 1;/*Stop removed*/\
            } /*If: I have overshot the gene end*/\
            \
            else\
               (indelCntSI) += baseOnMacSI;\
            \
            ++siCigMac;\
            break;\
         /*Case: Deletions*/\
         \
         /***********************************************\
         * Fun-08 Sec-03 Sub-03:
         *   - Check snps/matchs for early stops
         \**********************************************/\
         \
         case 'M':\
         case 'X':\
         case '=':\
         /*Case: Matchs and snps*/\
            refPosMacUI += baseOnMacSI;\
            extraNtMacSI += baseOnMacSI;\
            \
            /*Check if I overshot the gene*/\
            if(refPosMacUI > (endGeneSI))\
               extraNtMacSI -= (refPosMacUI-(endGeneSI));\
            \
            if(haveEndBl)\
            { /*If: had an end, check for LoFs*/\
               while(extraNtMacSI > 2)\
               { /*Loop: Check indel reading frame*/\
                  extraNtMacSI -= 3;\
                  \
                  if(extraNtMacSI < 3) /*last codon*/\
                     break; /*Check outside the loop*/\
                  \
                  aaC =\
                    revCompCodonToAA(\
                      samSTPtr->seqStr[readPosMacUI],\
                      samSTPtr->seqStr[readPosMacUI + 1],\
                      samSTPtr->seqStr[readPosMacUI + 2]\
                    );\
                  \
                  readPosMacUI += 3;\
                  \
                  /*Check if I had an early stop*/\
                  LoFMacBl |= (aaC== '*');\
               } /*Loop: Check indel reading frame*/\
            } /*If: had an end, check for LoFs*/\
            \
            /*Check if this is the last base*/\
            if(refPosMacUI >= (endGeneSI))\
            { /*If: At end; check if have start codon*/\
                LoFMacBl =\
                   bacteriaReverseStart_check(\
                      samSTPtr->seqStr[readPosMacUI],\
                      samSTPtr->seqStr[readPosMacUI + 1],\
                      samSTPtr->seqStr[readPosMacUI + 2]\
                   ); /*Check if I have an start codon*/\
               \
               readPosMacUI += 3;\
               \
               LoFMacBl = !LoFMacBl;\
            } /*If: At end; check if have start codon*/\
            \
            else\
            { /*Else: check if have early stop*/\
               aaC =\
                  revCompCodonToAA(\
                     samSTPtr->seqStr[readPosMacUI],\
                     samSTPtr->seqStr[readPosMacUI + 1],\
                     samSTPtr->seqStr[readPosMacUI + 2]\
                  );\
               \
               readPosMacUI += 3;\
               \
               LoFMacBl |= (aaC== '*');\
            } /*Else: check if have early stop*/\
            \
            ++siCigMac;\
            break;\
         /*Case: Matchs and snps*/\
      } /*Switch: Check cigar entry type*/\
      \
      /**************************************************\
      * Fun-08 Sec-03 Sub-04:
      *   - Move to the next cigar entry
      \*************************************************/\
      \
      if(siCigMac >= (samSTPtr)->lenCigUI)\
         break; /*End of sequence*/\
      \
      baseOnMacSI = (samSTPtr)->cigValAryI[siCigMac];\
   } /*Loop: Count indels/check for LoFs (foward)*/\
   \
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-08 Sec-04:
   ^   - Return the answer for the LoF entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/\
   \
   LoFMacBl;\
}) /*LoFReverseCheck*/

/*-------------------------------------------------------\
| Fun-09: checkAmrSam
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
|   - frameshiftBl:
|     o 1: Check for LoFs in frameshift
|     o 0: Treat frameshifts as exact matches
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
   char frameshiftBl,  /*1: Handle frameshifts*/
   char *errC          /*For error reporting*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-09 TOC: checkAmrSam
   '   - Checks if a sequence in a sam file entry has
   '     amr's (antibiotic resitance)
   '   o fun-09 sec-01:
   '     - Variable declerations
   '   o fun-09 sec-02:
   '     - Find the nearest amr to the sequence
   '   o fun-09 sec-03:
   '     - Check for AMRs
   '   o fun-09 sec-04:
   '     - Clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-09 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*For finding the amr position*/
   uint seqPosUI = 0;
   uint refPosUI = 0;

   /*For processing the cigar entries*/
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
   int amrEndI = 0;    /*Length of AMR*/

   char resBl = 0;     /*-1 is resitance; 0 is not*/

   /*For frameshift processing*/
   int numIndelSI = 0; /*Number indels in an frameshift*/
   char lofBl = 0;     /*1: had mising/early start/stop*/

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
   ^ Fun-09 Sec-02:
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
   if(iAmr == -1)
      goto noAmrErr_fun09_sec04_sub03;
   
   refPosUI = samST->refStartUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-09 Sec-03:
   ^   - Check for AMRs
   ^   o fun-09 sec-03 sub-01:
   ^     - Check if the read has the full AMR
   ^   o fun-09 sec-03 sub-02:
   ^     - Move to the first base in the amr pattern
   ^   o fun-09 sec-03 sub-03:
   ^     - Check for missing starts/stops, check for
   ^       early stops, and get indel counts
   ^       (frameshift checking must be enabled)
   ^   o fun-09 sec-03 sub-04:
   ^     - Check if the amr has a strict match
   ^   o fun-09 sec-03 sub-05:
   ^     - Handle amino acid amr's
   ^   o fun-09 sec-03 sub-06:
   ^     - Make sure there is really resistance
   ^   o fun-09 sec-03 sub-07:
   ^     - Check if resistance was found
   ^   o fun-09 sec-03 sub-08:
   ^     - Move to the next amr
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-09 Sec-03 Sub-01:
   *   - Check if the read has the full AMR
   \*****************************************************/

   iCig = 0;
   cigBaseOnI = samST->cigValAryI[0];

   /*Mark that this is an new frameshift*/
   if(frameshiftBl) frameshiftBl = 3; /*2 and 1 bit set*/
 
   for(
      iAmr = iAmr;
      iAmr < numAmrI;
      ++iAmr
   ){ /*Loop: Check if is an amr or not*/

      /*Check if I have another amr*/
      if(samST->refEndUI < (amrAryST[iAmr].refPosUI + 1))
         break; /*Finished with the read*/

      /*Make sure the read covers the full AMR*/
      amrEndI =
           amrAryST[iAmr].refPosUI
         + amrAryST[iAmr].lenRefSeqUI;

      if(samST->refEndUI < amrEndI)
         continue; /*read does not cover whole pattern*/

      if(samST->readLenUI < amrAryST[iAmr].lenAmrAaUI)
         continue; /*The AMR is longer then the read*/

      /**************************************************\
      * Fun-09 Sec-03 Sub-03:
      *   - Check for missing starts/stops, check for
      *     early stops, and get indel counts
      *     (frameshift checking must be enabled)
      \**************************************************/

      if(frameshiftBl & 2)
      { /*If: I have a new gene to check*/
         /*Check for missing starts/stops and early stops
         `   Also get the indel counts
         */
         if(amrAryST[iAmr].dirFlag== def_amrST_forwardDir)
         { /*If: I have an foward gene*/
            lofBl =
               LoFForwardCheck(
                  amrAryST[iAmr].geneFirstRefUI,
                  amrAryST[iAmr].geneLastRefUI,
                  refPosUI,
                  seqPosUI,
                  samST,
                  iCig,
                  cigBaseOnI,
                  numIndelSI
               );
         } /*If: I have an foward gene*/

         else if(
            amrAryST[iAmr].dirFlag == def_amrST_revCompDir
         ){ /*Else If: I have an reverse complement gene*/
            lofBl =
               LoFReverseCheck(
                  amrAryST[iAmr].geneFirstRefUI,
                  amrAryST[iAmr].geneLastRefUI,
                  refPosUI,
                  seqPosUI,
                  samST,
                  iCig,
                  cigBaseOnI,
                  numIndelSI
               );
         } /*Else If: I have an reverse complement gene*/

         else ; /*Unkown direction*/

         frameshiftBl &= (~2); /*Remove flag*/
      } /*If: I have a new gene to check*/

      /**************************************************\
      * Fun-09 Sec-03 Sub-02:
      *   - Move to the first base in the amr pattern
      \**************************************************/

      samEntryFindRefPos(
         samST,
         iCig,
         cigBaseOnI,
         amrAryST[iAmr].refPosUI,
         refPosUI,
         seqPosUI
      ); /*Find sequence position
         ` (fun-09 samEntryStruct.h)
         */

      /*Deletion at amr site means error, except for frame
      `   shifts. There should be enough sequence around
      `   it to move onto the deletion
      */
      if(samST->cigTypeStr[iCig] == 'D')
      { /*If: this entry is an deletion*/
         if(!frameshiftBl || !amrAryST[iAmr].frameshiftBl)
            goto nextAmr_fun09_sec03_sub07;
      } /*If: this entry is an deletion*/

      /**************************************************\
      * Fun-09 Sec-03 Sub-04:
      *   - Check if the amr has a strict match
      *   o fun-09 sec-03 sub-04 cat-01:
      *     - Get to first base and set up check variables
      *   o fun-09 sec-03 sub-04 cat-02:
      *     - Check if I might have an frame shift
      *   o fun-09 sec-03 sub-04 cat-03:
      *     - Check if I this is an amino acid sequence
      *   o fun-09 sec-03 sub-04 cat-04:
      *     - Check if this is an extact nucleotide match
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-04 Cat-01:
      +   - Get to first base and set up check variables
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

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

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-04 Cat-02:
      +   - Check if I might have an frame shift
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(frameshiftBl && amrAryST[iAmr].frameshiftBl)
      { /*If: This is an frameshift pattern*/
         if(
              amrAryST[iAmr].wholeGeneFlag
            & def_geneFrameshift_amrST
         ){ /*If: This is an LoF entry*/
            if(lofBl)
               goto isRes_fun08_sec03_sub06; /*LoF*/

            else if(numIndelSI % 3)
               goto isRes_fun08_sec03_sub06; /*LoF*/
               /*This is quick and dirty; the logic is if
               `   it is not an multiple of three, then it
               `   an is frame shift. However, this is not
               `   an garuntee
               */
         } /*If: This is an LoF entry*/

         /*I am likely one base off, so I start on the
         `  next base, unless there is an deletion here
         */
         if(
               samST->cigTypeStr[cigPosI] != 'D' 
            && samST->cigTypeStr[cigPosI] != 'I' 
         ) incCigCnt(cigCountI, cigPosI, samST);

         while(
               samST->cigTypeStr[cigPosI] == 'D' 
            || samST->cigTypeStr[cigPosI] == 'I' 
         ){ /*Loop: Count the number of indels*/
            incCigCnt(cigCountI, cigPosI, samST);
            ++delInAmrI;
         } /*Loop: Count the number of indels*/

         if(delInAmrI % 3)
            goto isRes_fun08_sec03_sub06; /*AMR detected*/

         delInAmrI = 0;
         cigPosI = iCig;
         cigCountI = cigBaseOnI;

         /*Else: I will treat this as an nucleotide
         `  pattern (no amino acid AMR sequence)
         */
      } /*If: This is an frameshift pattern*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-04 Cat-03:
      +   - Check if I have an amino acid sequence
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*This treats full deletions (no insertions) as
      `   snps
      */
      if(amrAryST[iAmr].amrAaStr[0] != '0')
         goto checkAmr_tbAmr_fun09_sec03_sub04;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-04 Cat-04:
      +   - Check if this is an extact nucleotide match
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
      
      checkSnp_tbAmr_fun09_sec03_sub03:;

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

      if(amrUStr[iBase] != '\0')
         goto nextAmr_fun09_sec03_sub07; /*Not an AMR*/

      /*This may be an AMR; need to make sure not a
      `   false positive
      */
      goto finishChecks_tbAmr_fun09_sec04_sub05;

      /**************************************************\
      * Fun-09 Sec-03 Sub-05:
      *   - Handle amino acid amr's
      *   o fun-09 sec-03 sub-05 cat-01:
      *     - Check if deletions are resistant
      *   o fun-09 sec-03 sub-05 cat-02:
      *     - Move to the first base in the target codon
      *   o fun-09 sec-03 sub-05 cat-03:
      *     - Check reverse complemnt gene snps/insertions
      *   o fun-09 sec-03 sub-05 cat-04:
      *     - Else I do not know direction, look at snps
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-05 Cat-01: checkAmrSam
      +   - Check if deletions are resistant
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      checkAmr_tbAmr_fun09_sec03_sub04:;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-05 Cat-02: checkAmrSam
      +   - Move to the first base in the target codon
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*This should work out, even when reverse*/
      seqUStr +=
         (int) amrAryST[iAmr].codonPosUI - (int) refPosUI;

      iBase = 0; /*For finding the number of matches/snps,
                 `  inss, and dels from the sequence after
                 `  I checked the AMR sequence (sub-05)
                 */

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-05 Cat-03: checkAmrSam
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
               goto nextAmr_fun09_sec03_sub07;

            base1UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0')
               goto nextAmr_fun09_sec03_sub07;

            base2UC =
               (uchar)
               compBaseToCodeLkTbl[*seqUStr--];

            if(*seqUStr == '\0')
               goto nextAmr_fun09_sec03_sub07;

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
                     bacteriaStartCode_check(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  /*If this was not a start codon*/;
                  if(resBl)
                  { /*If: this was a bacterial start*/
                     resBl = 0;
                     goto nextAmr_fun09_sec03_sub07;
                  } /*If: this was a bacterial start*/
               } /*If: this is a "start" codon*/
            } /*If: this could be any codon*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iAa])
               goto nextAmr_fun09_sec03_sub07;
         } /*Loop: Check the codon reading frame*/

         iMatch = amrAryST[iAmr].lenAmrSeqUI;
      } /*If: This is a reverse complement gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-05 Cat-04: checkAmrSam
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
               goto nextAmr_fun09_sec03_sub07;

            base1UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun09_sec03_sub07;

            base2UC = (uchar) baseToCodeLkTbl[*seqUStr++];

            if(*seqUStr == '\0')
               goto nextAmr_fun09_sec03_sub07;

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
                     bacteriaStartCode_check(
                        base1UC,
                        base2UC,
                        base3UC
                     );

                  /*If this was not a start codon*/;
                  if(resBl)
                  { /*If: this was a bacterial start*/
                     resBl = 0;
                     goto nextAmr_fun09_sec03_sub07;
                  } /*If: this was a bacterial start*/
               } /*If: this is a "start" codon*/
            } /*If: this could be any codon*/

            else if(aaC != amrAryST[iAmr].amrAaStr[iAa])
               goto nextAmr_fun09_sec03_sub07;
         } /*Loop: Check the codon reading frame*/

         iMatch = amrAryST[iAmr].lenAmrSeqUI;
      } /*Else If: This is a foward gene*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-09 Sec-03 Sub-05 Cat-05: checkAmrSam
      +   - Else I do not know the direction, look at snp
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else  goto checkSnp_tbAmr_fun09_sec03_sub03;

      /**************************************************\
      * Fun-09 Sec-03 Sub-06:
      *   - Make sure there is really resistance
      \**************************************************/

      finishChecks_tbAmr_fun09_sec04_sub05:;

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
      * Fun-09 Sec-03 Sub-07:
      *   - Check if resistance was found
      \**************************************************/

      if(resBl)
      { /*If: I found a resitance mutation*/
         isRes_fun08_sec03_sub06:;

         resBl = 0;
         ++(amrAryST[iAmr].numSupReadsUI);
         ++(*numHitsI);

         if(amrSTList)
         { /*If: This is not the first amr*/
            amrST->nextAmr = makeAmrHit();

            if(!amrST->nextAmr)
                goto memErr_fun09_sec04_sub02;

            amrST->nextAmr->amrST = &amrAryST[iAmr];
            amrST->nextAmr->seqPosUI = seqPosUI;
            amrST = amrST->nextAmr;
         } /*If: This is not the first amr*/

         else
         { /*Else: this is the first amr*/
            amrSTList = makeAmrHit();
            amrST = amrSTList;

            if(!amrST)
                goto memErr_fun09_sec04_sub02;

            amrST->amrST = &amrAryST[iAmr];
            amrST->seqPosUI = seqPosUI;
         } /*Else: this is the first amr*/
      } /*If: I found a resitance mutation*/

      /**************************************************\
      * Fun-09 Sec-03 Sub-08:
      *   - Move to the next amr
      \**************************************************/

      nextAmr_fun09_sec03_sub07:;

      if(frameshiftBl & iAmr)
      { /*If: frameshift checking is set up*/
         if(
            cStrEql(
               amrAryST[iAmr].geneIdStr,
               amrAryST[iAmr - 1].geneIdStr,
               '\0'
             )
         ) frameshiftBl |= 2; /*Changing genes*/
      } /*If: frameshift checking is set up*/
   } /*Loop: Check if is an amr or not*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-09 Sec-04:
   ^   - Clean up and return
   ^   o fun-09 sec-04 sub-01:
   ^     - No error clean up
   ^   o fun-09 sec-04 sub-02:
   ^     - Memory error clean up
   ^   o fun-09 sec-04 sub-03:
   ^     - No AMRs error
   ^   o fun-09 sec-04 sub-04:
   ^     - Clean up after errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-09 Sec-04 Sub-01:
   *   - No error clean up
   \*****************************************************/

   return amrSTList;

   /*****************************************************\
   * Fun-09 Sec-04 Sub-02:
   *   - Memory error clean up
   \*****************************************************/

   memErr_fun09_sec04_sub02:;

   *errC = 64;

   goto errCleanUp_fun09_sec04_sub04;

   /*****************************************************\
   * Fun-09 Sec-04 Sub-03:
   *   - No AMRs error
   \*****************************************************/

   noAmrErr_fun09_sec04_sub03:;

   *errC = 0;

   goto errCleanUp_fun09_sec04_sub04;

   /*****************************************************\
   * Fun-09 Sec-04 Sub-04:
   *   - Clean up after errors
   \*****************************************************/

   errCleanUp_fun09_sec04_sub04:;

   if(amrSTList) freeAmrHitList(amrSTList);
   amrSTList = 0;

   return 0;
} /*checkAmr*/

/*-------------------------------------------------------\
| Fun-10: pCrossRes
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
| Fun-11: pHeadAmrHitList
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
| Fun-12: pAmrHitList
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
   ' Fun-12 TOC: pAmrHitList
   '   - Prints out the AMRs that were in a sequence. This
   '     is for consensuses
   '   o fun-12 sec-01:
   '     - variable decerations
   '   o fun-12 sec-02:
   '     - Print AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-01:
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
   ^ Fun-12 Sec-02:
   ^   - Print AMRs
   ^   o fun-12 sec-02 sub-01:
   ^     - Print AMRs/check if already printed the variant
   ^   o fun-12 sec-02 sub-02:
   ^     - Print the read/gene ids
   ^   o fun-12 sec-02 sub-03:
   ^     - Print out the Antibiotics resitant to
   ^   o fun-12 sec-02 sub-04:
   ^     - Print out the cross resistance
   ^   o fun-12 sec-02 sub-05:
   ^     - Print out the variant id, mutation type,
   ^       and positions
   ^   o fun-12 sec-02 sub-06:
   ^     - Print out if I have high, unkown (normal?),
   ^       or low resitance
   ^   o fun-12 sec-02 sub-07:
   ^     - Print out if the low resitance is additive
   ^   o fun-12 sec-02 sub-08:
   ^     - Print out if the restance needs a functional
   ^       gene
   ^   o fun-12 sec-02 sub-09:
   ^     - Print out the effect of the  mutation
   ^   o fun-12 sec-02 sub-10:
   ^     - Print out the WHO's comment
   ^   o fun-12 sec-02 sub-11:
   ^     - Move onto the next AMR
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-12 Sec-02 Sub-01:
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

         if(! tmpST) break; /*Finished*/
         if(! matchBl) continue;
      } /*If: I need to check previous results*/

      else lastST = tmpST;

      /**************************************************\
      * Fun-12 Sec-02 Sub-02:
      *   - Print the read/gene ids
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "%s\t%s\t",
        (seqIdStr),              /*Name of the seq*/
        tmpST->amrST->geneIdStr /*Gene name*/
      ); /*Pirnt out gene id and drug*/
      
      /**************************************************\
      * Fun-12 Sec-02 Sub-03:
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
      * Fun-12 Sec-02 Sub-04:
      *   - Print out the cross resistance
      \**************************************************/

      fprintf((FILE *) outFILE, "\t");

      pCrossRes(
         tmpST->amrST,(drugAryStr),
         (FILE *) outFILE
      );
      
      /**************************************************\
      * Fun-12 Sec-02 Sub-05:
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
      * Fun-12 Sec-02 Sub-06:
      *   - Print out if I have high, unkown (normal?),
      *     or low resitance
      \**************************************************/

      if(tmpST->amrST->highResBl)
         fprintf((FILE *) outFILE, "\thigh");

      else if(tmpST->amrST->lowResBl)
         fprintf((FILE *) outFILE, "\tlow");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-12 Sec-02 Sub-07:
      *   - Print out if the low resitance is additive
      \**************************************************/

      if(tmpST->amrST->additiveResBl)
         fprintf((FILE *) outFILE, "\tAdditive");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-12 Sec-02 Sub-08:
      *   - Print out if the restance needs a functional
      *     gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         tmpST->amrST->needsGeneStr
      ); /*Print out if needs a functional gene*/

      /**************************************************\
      * Fun-12 Sec-02 Sub-09:
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
      * Fun-12 Sec-02 Sub-10:
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
      * Fun-12 Sec-02 Sub-11:
      *   - Move onto the next AMR
      \**************************************************/
      
      tmpST = tmpST->nextAmr;
   } /*Loop: Print out all amr's*/
} /*pAmrHitList*/

/*-------------------------------------------------------\
| Fun-13: pHeadAmrs
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
   fprintf((FILE *) outFILE,"gene\tDrug");
   fprintf((FILE *) outFILE,"\tcrossResistance");
   fprintf((FILE *) outFILE,"\tvariantId\ttype");
   fprintf((FILE *) outFILE,"\trefPos");
   fprintf((FILE *) outFILE,"\tnumSupportingReads");
   fprintf((FILE *) outFILE,"\tpercSupportReads");
   fprintf((FILE *) outFILE, "\tnumMappedReads");
   fprintf((FILE *) outFILE, "\tresitanceLevel");
   fprintf((FILE *) outFILE, "\tresistanceAdditive");
   fprintf((FILE *) outFILE, "\tneedsGene");
   fprintf((FILE *) outFILE,"\teffect\twhoComment\n");
} /*pHeadAmrs*/

/*-------------------------------------------------------\
| Fun-14: pAmrs
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
   ' Fun-14 TOC: pAmrs
   '   o fun-14 sec-01:
   '     - Variable declerations
   '   o fun-14 sec-02:
   '     - Start loop and filter AMRs
   '   o fun-14 sec-03:
   '     -  Print out the entry (passed filters)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-01:
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
   ^ Fun-14 Sec-02:
   ^   - Start loop and filter AMRs
   ^   o fun-14 sec-02 Sub-01:
   ^     - Start loop and remove low depth AMRs
   ^   o fun-14 sec-02 Sub-02:
   ^     - Remove AMRs with a lower % of mapped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-14 Sec-02 Sub-01:
   *   - Startloop and remove low depth AMRs
   \*****************************************************/

   for(indexUI = 0; indexUI < (numAmrsUI); ++indexUI)
   { /*Loop:Check and print out amrs*/

      if((amrSTAry)[indexUI].numSupReadsUI <(minDepthUI))
         continue;
      
      if((amrSTAry)[indexUI].numMapReadsUI ==0) continue;

      /**************************************************\
      * Fun-14 Sec-02 Sub-02:
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
      ^ Fun-14 Sec-03:
      ^   -  Print out the entry (passed filters)
      ^   o fun-14 sec-03 Sub-01:
      ^     - Print out the gene id
      ^   o fun-14 sec-03 Sub-02:
      ^     - Print out the antibiotics resitant to
      ^   o fun-14 sec-03 Sub-03:
      ^     - Print out antibiotics cross resistanct to
      ^   o fun-14 sec-03 Sub-04:
      ^     - Print out variant id, mutation, reference
      ^       position, number reads supporting AMR and
      ^       total reads at reference position.
      ^   o fun-14 sec-03 Sub-05:
      ^     - Print out if I have high, unkown (normal?),
      ^       or low resitance
      ^   o fun-14 sec-03 Sub-06:
      ^     - Print out if the low resitance is additive
      ^   o fun-14 sec-03 Sub-07:
      ^     - Print out if the restance needs a functional
      ^       gene
      ^   o fun-14 sec-03 Sub-08:
      ^     - Print out the mutation effect
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      
      /**************************************************\
      * Fun-14 Sec-03 Sub-01:
      *   - Print out the gene id
      \**************************************************/

      lastPrintIndexUI = indexUI;

      fprintf(
        (FILE *) outFILE,
        "%s\t",
        (amrSTAry)[indexUI].geneIdStr /*gene id*/
      ); /*Pirnt out gene id and drug*/
      
      /**************************************************\
      * Fun-14 Sec-03 Sub-02:
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
      * Fun-14 Sec-03 Sub-03:
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
      * Fun-14 Sec-03 Sub-04:
      *   - Print out variant id, mutation, reference
      *     position, number reads supporting AMR and
      *     total reads at reference position.
      \**************************************************/

      fprintf(
        (FILE *) outFILE,
        "\t%s\t%s\t%i\t%i\t%.2f\t%i",
        amrSTAry[indexUI].varIdStr,   /*Variant id*/
        amrSTAry[indexUI].mutTypeStr, /*snp/del/ins/LoF*/
        (int) amrSTAry[indexUI].refPosUI,
        (int) amrSTAry[indexUI].numSupReadsUI,
        percSupF * 100,
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
      * Fun-14 Sec-03 Sub-05:
      *   - Print out if I have high, unkown (normal?),
      *     or low resitance
      \**************************************************/

      if(amrSTAry[indexUI].highResBl)
         fprintf((FILE *) outFILE, "\thigh");

      else if(amrSTAry[indexUI].lowResBl)
         fprintf((FILE *) outFILE, "\tlow");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-14 Sec-03 Sub-06:
      *   - Print out if the low resitance is additive
      \**************************************************/

      if(amrSTAry[indexUI].additiveResBl)
         fprintf((FILE *) outFILE, "\tAdditive");

      else fprintf((FILE *) outFILE, "\tNA");

      /**************************************************\
      * Fun-14 Sec-03 Sub-07:
      *   - Print out if the restance needs a functional
      *     gene
      \**************************************************/

      fprintf(
         (FILE *) outFILE, "\t%s",
         amrSTAry[indexUI].needsGeneStr
      ); /*Print out if needs a functional gene*/

      /**************************************************\
      * Fun-14 Sec-03 Sub-08:
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
      * Fun-14 Sec-03 Sub-09:
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
| Fun-15: pHeadAmrReadIds
|   - Prints out the header for the read id mapped variant
|     file
| Input:
|   - outFILE:
|     o Pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o The header for the read id mapped variant table
\-------------------------------------------------------*/
void
pHeadAmrReadIds(
   void *outFILE
){
   fprintf((FILE *) outFILE, "readId\tvariantId\n");
} /*pHeadAmrReadIds*/

/*-------------------------------------------------------\
| Fun-16: pAmrReadIds
|   - Prints out the read id and the ARMs variant ids that
|     it supported
| Input:
|   - idStr:
|     o C-string with read id to print out
|   - amrHitSTListPtr:
|     o Pointer to an list of amrHit structures having the
|       AMRs that the read supported
|   - outFILE:
|     o Pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o The read id and any AMRs that it supported (tsv)
\-------------------------------------------------------*/
void
pAmrReadIds(
   char *idStr,
   struct amrHit *amrHitSTListPtr,
   void *outFILE
){
   char uniqVarBl = 0;
   struct amrHit *tmpHitST = amrHitSTListPtr;
   struct amrHit *lastHitST = tmpHitST;
   
   if(tmpHitST)
   { /*If: I have AMR(s) to print out*/
      fprintf(
         (FILE *) outFILE,
         "%s\t%s\n",
         idStr,
         tmpHitST->amrST->varIdStr
      );

      tmpHitST = tmpHitST->nextAmr;

      while(tmpHitST)
      { /*Loop: Print each amr the read mapped to*/
         uniqVarBl =
            cStrEql(
               lastHitST->amrST->varIdStr,
               tmpHitST->amrST->varIdStr,
               '\0'
            );

         if(uniqVarBl)
         { /*If: This is an unique variant (print AMR)*/
            fprintf(
               (FILE *) outFILE,
               "%s\t%s\n",
               idStr,
               tmpHitST->amrST->varIdStr
            );
         } /*If: This is an unique variant (print AMR)*/

         lastHitST = tmpHitST;
         tmpHitST = tmpHitST->nextAmr;
      } /*Loop: Print each amr the read mapped to*/
   } /*If: I have AMR(s) to print out*/
} /*pAmrReadIds*/

/*-------------------------------------------------------\
| Fun-17: lookForAmrsSam
|   - Look for anti-microbial (antibiotic) genes in the
|     reads in a sam file
| Input:
|   - amrSTAryPtr
|     o Pointer to an array of amrStructs with AMRs to
|       check
|   - numAmrI:
|     o Number of AMRs in amrSTAryPtr
|   - drugAryStr:
|     o C-string array (see drug_str_ary.c/h) with
|       antibiotic names
|   - readsBl:
|     o 1: Printing out read stats (use pAmr)
|     o 0: Printing out consensus stats (use pAmrHitList)
|   - framshiftBl:
|     o 1: Check for framshifts if LoF and frameshift AMRs
|     o 0: Treat all frameshifts and LoFs as exact matches
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
|   - samFileStr:
|     o C-string with sam file to check for AMRs
|   - outFileStr:
|     o C-string with name to print AMR hits to
|   - idFileStr:
|     o C-sring with name of file to print read ids to
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
   void *amrSTAryPtr,     /*Has amr's to check*/
   int numAmrI,           /*Length of amrAryST*/
   char *drugAryStr,      /*Has antibiotic names*/
   char readsBl,          /*1: processing reads not cons*/
   char frameshiftBl,     /*1: check frameshitfs AMRs*/
   unsigned int minDepthUI,/*Min depth to keep amr*/
   float minPercMapF,     /*% support to keep amr; read*/
   float minPercTotalF,   /*% mapped reads to keep read*/
   char *samFileStr,      /*Sam file with reads to check*/
   char *outFileStr,      /*File to output results to*/
   char *idFileStr        /*File to print read ids to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-17 TOC:P lookForAmrsSam
   '   - Look for anti-microbial (antibiotic) genes in the
   '     reads in a sam file
   '   o fun-17 sec-01:
   '     - Variable declerations
   '   o fun-17 sec-02:
   '     - Get the first sam entry
   '   o fun-17 sec-03:
   '     - Check for AMRs
   '   o fun-17 sec-04:
   '     - Print out read AMR stats
   '   o fun-17 sec-05:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *buffHeapStr = 0;   /*Buffer for readSamLine*/
   ulong lenBuffUL = 0; /*Current length of buffHeapStr*/

   char errC = 0;         /*For error checking*/
   int numHitsI = 0;      /*Number of amrs hits/con*/
   uint totalReadsUI = 0; /*Number of kept reads*/

   struct samEntry samStackST;
   struct amrHit *amrHitListHeapST = 0;

   FILE *idFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*For dealing with void*/
   struct amrStruct *amrAryST =
      (struct amrStruct *) amrSTAryPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-02:
   ^   - Get the first sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samStackST);

   if(! samFileStr || *samFileStr == '-') samFILE = stdin;
   
   else
   { /*Else: I am given a sam file*/
      samFILE = fopen(samFileStr, "r");

      if(! samFILE)
         goto fileErr_sec05_sub02_lookForAmrsSam;
   } /*Else: I am given a sam file*/

   if(! outFileStr || *outFileStr =='-') outFILE = stdout;

   else
   { /*Else: I am given an output file*/
      outFILE = fopen(outFileStr, "r");

      if(! outFILE)
         goto fileErr_sec05_sub02_lookForAmrsSam;
   } /*Else: I am given an output file*/

   if(idFileStr)
   { /*If: I was given an read id file*/
      idFILE = fopen(idFileStr, "w");

      if(! idFILE)
         goto fileErr_sec05_sub02_lookForAmrsSam;
      
      pHeadAmrReadIds(idFILE);
   } /*If: I was given an read id file*/

   errC =
      readSamLine(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      ); /*Read in the first line*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-03:
   ^   - Check for AMRs
   ^   o fun-17 sec-03 sub-01:
   ^     - Filter out less usefull entries
   ^   o fun-17 sec-03 sub-02:
   ^     - Check for amrs
   ^   o fun-17 sec-03 sub-03:
   ^     - Print out consensus sequence AMRS
   ^   o fun-17 sec-03 sub-04:
   ^     - Deal with read amrs; print ids if requested/
   ^       free consensus structuerrs
   ^   o fun-17 sec-03 sub-05:
   ^     - Move to the next sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-17 Sec-03 Sub-01:
   *   - Filter out less usefull entries/start loop
   \*****************************************************/

   if(! readsBl) pHeadAmrHitList(outFILE);
   else pHeadAmrs(outFILE);

   while(!errC)
   { /*Loop: Check if have antibiotic resitance*/
      if(samStackST.extraStr[0] == '@')
      { /*If: this is a header*/
         errC =
            readSamLine(
               &samStackST,
               &buffHeapStr,
               &lenBuffUL,
               samFILE
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is a header*/

      /*4=unmapped, 256=secondary, 2048=supplemental*/
      if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If: this is an umapped read, 2ndary*/
         errC =
            readSamLine(
               &samStackST,
               &buffHeapStr,
               &lenBuffUL,
               samFILE
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is an umapped read, 2ndary, sup aln*/

      /**************************************************\
      * Fun-17 Sec-03 Sub-02:
      *   - Check for amrs
      \**************************************************/

      ++totalReadsUI;

      amrHitListHeapST = 
         checkAmrSam(
            &samStackST,  /*Sequence to check*/
            amrAryST,     /*Has amr's to check*/
            numAmrI,      /*Length of amrAryST*/
            &numHitsI,    /*Number amr hits for seq*/
            frameshiftBl, /*1: check frameshifts*/
            &errC         /*For error reporting*/
         ); /*Check if there are any amr's*/

      if(errC)
         goto memErr_sec05_sub02_lookForAmrsSam;

      /**************************************************\
      * Fun-17 Sec-03 Sub-03:
      *   - Print out consensus sequence AMRS
      \**************************************************/

      /*Check if I can print out the amr's*/
      if(! readsBl)
      { /*If: I am printing out consensus AMRs*/
          pAmrHitList(
             samStackST.qryIdStr,
             amrHitListHeapST,
             (drugAryStr),
             (outFILE)
          ); /*Print the amr entry*/
      } /*If: I am printing out consensus AMRs*/

      /**************************************************\
      * Fun-17 Sec-03 Sub-04:
      *   - Deal with read amrs; print ids if requested/
      *     free consensus structuerrs
      \**************************************************/

      else if(idFileStr)
      { /*Else If; I am printing out read ids*/
         pAmrReadIds(
            samStackST.qryIdStr,
            amrHitListHeapST,
            idFILE
         );
      } /*Else If; I am printing out read ids*/

      /**************************************************\
      * Fun-17 Sec-03 Sub-05:
      *   - Move to the next sam entry
      \**************************************************/

      freeAmrHitList(amrHitListHeapST);
      amrHitListHeapST = 0;

      errC =
         readSamLine(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next sam file entry*/
   } /*Loop: Check if have antibiotic resitance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-17 Sec-04:
   ^   - Print out read AMR stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(errC > 1) goto memErr_sec05_sub02_lookForAmrsSam;

   if(samFILE != stdin) fclose(samFILE);
   samFILE = 0;

   freeSamEntryStack(&samStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

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
   ^ Fun-17 Sec-05:
   ^   - Clean up
   ^   o fun-17 sec-05 sub-01:
   ^     - Clean up after a successful run
   ^   o fun-17 sec-05 sub-02:
   ^     - Clean up after a memory error
   ^   o fun-17 sec-05 sub-03:
   ^     - Clean up after an file error
   ^   o fun-17 sec-05 sub-03:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-17 Sec-05 Sub-01:
   *   - Clean up after a successful run
   \*****************************************************/

   /*Make sure I cleaned up everything; Some of this
   `  has already been dealt with, but this also ensures
   `  no memory loss
   */
   if(samFILE && samFILE != stdin) fclose(samFILE);
   if(outFILE && outFILE != stdout) fclose(outFILE);
   if(idFILE) fclose(idFILE);

   freeSamEntryStack(&samStackST);
   freeAmrHitList(amrHitListHeapST);
   amrHitListHeapST = 0;

   return 0;

   /*****************************************************\
   * Fun-17 Sec-05 Sub-02:
   *   - Clean up after a memory error
   \*****************************************************/

   memErr_sec05_sub02_lookForAmrsSam:;
   errC = 64;
   goto errCleanUp_fun16_sec05_sub04;

   /*****************************************************\
   * Fun-17 Sec-05 Sub-03:
   *   - Clean up after an file error
   \*****************************************************/

   fileErr_sec05_sub02_lookForAmrsSam:;
   errC = 2;
   goto errCleanUp_fun16_sec05_sub04;

   /*****************************************************\
   * Fun-17 Sec-05 Sub-04:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_fun16_sec05_sub04:;

   if(samFILE != stdin) fclose(samFILE);
   if(outFILE != stdout) fclose(outFILE);
   fclose(idFILE);

   free(buffHeapStr);
   buffHeapStr = 0;

   freeSamEntryStack(&samStackST);

   freeAmrHitList(amrHitListHeapST);
   amrHitListHeapST = 0;

   return errC;
} /*lookForAmrsSam*/

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
