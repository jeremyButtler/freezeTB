/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
# Libraries:
#   - "../gereralLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   o "../generalLib/ulCpStr.h"
#   - "amrStruct.h"
#   o "../generalLib/codonTbl.h"
#   o "../generalLib/genMath.h"
# C Stanadrd Libraries
#   o <stdio.h>
#   o <stdlib.h>
#   o <limits.h>
\#######################################################*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

#include "../generalLib/samEntryStruct.h"
#include "amrStruct.h"

/*TODO: Get these working*/
/*#include "../generalLib/trimSam.h"*/
/*#include "../generalLib/rmHomo.h"*/

typedef struct amrHit{
   struct amrStruct *amrST;
   uint seqPosUI;
   struct amrHit *nextAmr;
}amrHit;

/*-------------------------------------------------------\
| Fun-01: initAmrHit
|   - Initializes an amrHit structuer to all zeros
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
#define initAmrHit(amrHitSTPtr){\
   (amrHitSTPtr)->amrST = 0;\
   (amrHitSTPtr)->seqPosUI = 0;\
   (amrHitSTPtr)->nextAmr = 0;\
} /*initAmrHit*/

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
|   - Sets:
|     o amrHitSTPtr to 0
\-------------------------------------------------------*/
#define freeAmrHit(amrHitSTPtr){\
   free((amrHitSTPtr));\
   (amrHitSTPtr) = 0;\
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
|   - Sets:
|     o amrHitSTPtr to 0
\-------------------------------------------------------*/
#define freeAmrHitList(amrHitSTListPtr){\
   struct amrHit *tmpST = (amrHitSTListPtr)->nextAmr;\
   \
   while(tmpST != 0)\
   { /*Loop: Free all amrHit structures in list*/\
      freeAmrHit((amrHitSTListPtr));\
      (amrHitSTListPtr) = tmpST;\
      tmpST = (amrHitSTListPtr)->nextAmr;\
   } /*Loop: Free all amrHit structures in list*/\
   \
   freeAmrHit((amrHitSTListPtr));\
} /*freeAmrHit*/

/*-------------------------------------------------------\
| Fun-04:
|   - Makes a new, initialized amrHit structer on heap
| Input:
| Output:
|   - Returns:
|     o A pointer to the new amrHit structure
|     o 0 for memory error
\-------------------------------------------------------*/
#define makeAmrHit() ({\
   struct amrHit *retST = malloc(sizeof(struct amrHit));\
   if(retST) initAmrHit(retST);\
   retST;\
}) /*makeAmrHit*/

/*-------------------------------------------------------\
| Fun-05: checkAmrSam
|   - Checks if a sequence in a sam file entry has
|     amr's (antibiotic resitance)
| Input:
|   - samST:
|     o Pointer to samEntry struct with sequence to check
|       for amr's
|   - amrAryST:
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
static struct amrHit * checkAmrSam(
   struct samEntry *samST,     /*Sequence to check*/
   struct amrStruct *amrAryST, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   int *numHitsI,              /*Number amr hits for seq*/
   char *errC                  /*For error reporting*/
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
   
   refPosUI = samST->refStartUI;

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
      if(samST->refEndUI < amrAryST[iAmr].refPosUI)
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

      seqUStr = (uchar *) &samST->seqStr[seqPosUI];

      /*check if the amr is from an amino acid change*/
      if(amrAryST[iAmr].amrAaC != '\0')
         goto  checkAaAmr;

      amrUStr = (uchar *) amrAryST[iAmr].amrSeqStr;

      for(
         iBase = 0;
         (amrUStr[iBase] &~32) == (seqUStr[iBase] &~32);
         ++iBase
      ) {}

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
      \**************************************************/

      checkAaAmr:

      /*Check if I have an aa to check*/
      /*This should work out, even when reverse*/
      seqUStr +=
         (int) amrAryST[iAmr].codonPosUI - (int) refPosUI;

      if(!amrAryST[iAmr].revBl)
      { /*If: This is forward entry*/
         base1UC = (uchar) baseToCodeLkTbl[seqUStr[0]];
         base2UC = (uchar) baseToCodeLkTbl[seqUStr[1]];
         base3UC = (uchar) baseToCodeLkTbl[seqUStr[2]];
      } /*If: This is forward entry*/

      else
      { /*If: This is a reverse complement entry*/
         seqUStr -= 2; /*Move to the last base*/
         base1UC = (uchar)compBaseToCodeLkTbl[seqUStr[2]];
         base2UC = (uchar)compBaseToCodeLkTbl[seqUStr[1]];
         base3UC = (uchar)compBaseToCodeLkTbl[seqUStr[0]];
      } /*If: This is a reverse complement entry*/

      aaC = codonLkTbl[base1UC][base2UC][base3UC];
      resBl= -(aaC == amrAryST[iAmr].amrAaC);

      /**************************************************\
      * Fun-05 Sec-03 Sub-05:
      *   - Check if resistance was found
      \**************************************************/

      checkIfRes:

      if(resBl)
      { /*If: I found a resitance mutation*/
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
| Fun-06: pAmrs
|   - Prints out all amr's that were in a sequence
| Input:
|   - samST:
|     o Sam file entry
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
#define pAmrs(amrHitSTListPtr,drugAryStr,outFILE){\
   struct amrHit *tmpST = (amrHitSTListPtr);\
   ulong amrFlagUL = 0;\
   int flagOnI = 0;\
   \
   while(tmpST)\
   { /*Loop: Print out all amr's*/\
      amrFlagUL = tmpST->amrST->amrFlagsUL;\
      flagOnI = 0;\
      \
      while(amrFlagUL)\
      { /*Loop: Checn each flag*/\
         /*Check if there is resitance to an antibiotic*/\
         if(!(amrFlagUL & 1))\
         { /*If: There is no amr to this drug*/\
            ++flagOnI;\
            amrFlagUL >>= 1;\
            continue; /*No resitance*/\
         } /*If: There is no amr to this drug*/\
         \
         /*There is resistance, print it out*/\
         fprintf(\
           (outFILE),\
           "%s\t%s",\
           tmpST->amrST->geneIdStr, /*Gene name*/\
           (drugAryStr)[flagOnI] /*Amr drug*/\
         ); /*Print out the gene id and drug name*/\
         \
         if(  tmpST->amrST->lenAmrSeqUI\
            < tmpST->amrST->lenRefSeqUI\
         ) fprintf((outFILE), "\tdel");\
         \
         else if(\
              tmpST->amrST->lenAmrSeqUI\
            > tmpST->amrST->lenRefSeqUI\
         ) fprintf((outFILE), "\tins");\
         \
         else if(tmpST->amrST->amrAaC)\
           fprintf((outFILE), "\tsnp-aa");\
         \
         else fprintf((outFILE), "\tsnp");\
         \
         fprintf(\
           (outFILE),\
           "\t%u\t%u\t%s\t%s",\
           tmpST->amrST->refPosUI,  /*Position on ref*/\
           tmpST->seqPosUI,         /*Position on seq*/\
           tmpST->amrST->refSeqStr, /*Reference pattern*/\
           tmpST->amrST->amrSeqStr  /*amr pattern*/\
         );\
         \
         if(tmpST->amrST->amrAaC)\
            fprintf(\
               (outFILE),\
               "\t%c\n",\
               tmpST->amrST->amrAaC\
            );\
         else fprintf((outFILE), "\t-\n");\
         \
         ++flagOnI;\
         amrFlagUL >>= 1;\
      } /*Loop: Checn each flag*/\
      \
      tmpST = tmpST->nextAmr;\
   } /*Loop: Print out all amr's*/\
} /*pAmrs*/

/*-------------------------------------------------------\
| Fun-07: lookForAmrsSam
|   - Look for anti-microbial (antibiotic) genes in the
|     reads in a sam file
| Input:
|   - buffStr:
|     o Temporary buffer (c-string) to use when reading in
|       the sam file. This is resized as needed.
|   - lenBuffUL:
|     o Current length of buffStr. This is updated when
|       buffStr is resized
|   - samFILE:
|     o Pointer to sam file to read  from
|   - outFILE:
|     o Pointer to file to print the amr results to
| Output:
|   - Returns:
|     o 0 for no problems
|     o 64 for memory errors
\-------------------------------------------------------*/
static char lookForAmrsSam(
   struct amrStruct *amrAryST, /*Has amr's to check*/
   int numAmrI,                /*Length of amrAryST*/
   char *drugAryStr[], /*Has antibiotic names*/
   char **buffStr,   /*Buffer for readSamLine*/
   ulong *lenBuffUL, /*Current length of buffStr*/
   FILE *samFILE,    /*Sam file with reads to check*/
   FILE *outFILE
){
   char errC = 0;
   int numHitsI = 0;
   struct samEntry samST;
   struct amrHit *amrHitSTList = 0;

   initSamEntry(&samST);

   errC =
      readSamLine(
         &samST,
         (buffStr),
         (lenBuffUL),
         (samFILE)
      ); /*Read in the first line*/

   while(!errC)
   { /*Loop: Check if have antibiotic resitance*/
      if(samST.extraStr[0] == '@')
      { /*If: this is a header*/
         errC =
            readSamLine(
               &samST,
               (buffStr),
               (lenBuffUL),
               (samFILE)
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is a header*/

      /*4=unmapped, 256=secondary, 2048=supplemental*/
      if(samST.flagUS & (4 + 256))
      { /*If: this is an umapped read, 2ndary*/
         errC =
            readSamLine(
               &samST,
               (buffStr),
               (lenBuffUL),
               (samFILE)
            ); /*Get the next sam entry*/

         continue;
      } /*If: this is an umapped read, 2ndary, sup aln*/

      /*rmHomo call would be here*/

      amrHitSTList = 
         checkAmrSam(
            &samST,   /*Sequence to check*/
            (amrAryST), /*Has amr's to check*/
            (numAmrI),  /*Length of amrAryST*/
            &numHitsI,/*Number amr hits for seq*/
            &errC     /*For error reporting*/
         ); /*Check if there are any amr's*/

      if(errC) return 64; /*Memory error*/

      /*Check if I can print out the amr's*/
      if(amrHitSTList)
      { /*If: There were amr's*/
          pAmrs(amrHitSTList, (drugAryStr), (outFILE));
          freeAmrHitList(amrHitSTList);
      } /*If: There were amr's*/

      errC =
         readSamLine(
            &samST,
            (buffStr),
            (lenBuffUL),
            (samFILE)
         ); /*Get the next sam file entry*/
   } /*Loop: Check if have antibiotic resitance*/

   return 0;
} /*lookForAmrsSam*/

#endif
