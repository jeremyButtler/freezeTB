/*########################################################
# Name: tbCon-fun 
#  - Holds functions to make a quick consensus, more like
#    ivar. This is a bit memory intensive,
#    (40 + # insertions * (17) + bases in insertions)
#    bytes . Base length is 5 million
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o st-01: insBase (.h only)
'     - Structure to hold an list of insetions for a
'       single position
'   o st-02: conBase (.h only)
'     - Structure to hold an single base in a sequence
'   o st-03: tbConSet (.h only)
'     - Holds the settings for tbCon
'   o fun-01: initInsBase (.h only)
'     - Initializes an initBase structure
'   o fun-02: makeInsBase
'     - Makes an initializes an initBase structure on the
'   o fun-03: freeInsBase
'     - Frees an heap allocated insBase structure
'   o fun-04: blankConBase (.h only)
'     - Sets all non-pointer values in a conBase structure
'       to 0
'   o fun-05: initConBase (.h only)
'     - Sets all values (including pointers) in a conBase
'       structure to 0
'   o fun-06: makeConBase
'     - Makes an initialized, heap allocated conBase
'       structure
'   o fun-07: freeConBaseStack
'     - Frees the heap allocated variables in a conBase
'       structure and also blanks the conBase structure
'   o fun-08: freeConBase
'     - Frees a conBase structure
'   o fun-09: freeConBaseAry
'     - Frees an array of conBase structures (assumes
'       arrya is on heap)
'   o fun-10: initTbConSet
'     - initialize/set tbConSet settings to defaults
'   o fun-11: addReadToConBaseArray
'     - Adds the read into a conBase array
'   o fun-12: collapseConBaseArray
'     - Collapses a conBase array into an array of
'       samEntry structures
'   o fun-13: pConBaseArray
'     - Print all entries in a conBase structure array
'       that are above the minimum read depth
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

#include "tbCon-fun.h"

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"
#include "../generalLib/numToStr.h"
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/genMath.h"
#include "../generalLib/ulCpStr.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .h #include "../generalLib/base10StrToNum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-02: makeInsBase
|   - Makes an initializes an initBase structure on the
|     heap
| Input:
| Output:
|   - Returns:
|     o An pointer to an initialized insBase structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct insBase *
makeInsBase(
){
   struct insBase *retMacST =
      malloc(sizeof(struct insBase));

   if(retMacST) initInsBase(retMacST);

   return retMacST;
} /*makeInsBase*/

/*-------------------------------------------------------\
| Fun-03: freeInsBase
|   - Frees an heap allocated insBase structure
| Input:
|   - insBaseSTPtr:
|     o A Pointer to an insBase structure to free
| Output:
|   - Frees:
|     o insBaseSTPtr
|   - Sets:
|     o insBaseSTPtr to be 0
|   - Returns:
|     o Next insBase structure (nextIns) in insBase list
\-------------------------------------------------------*/
struct insBase *
freeInsBase(
   struct insBase *insBaseSTPtr
){
  struct insBase *nextIns = (insBaseSTPtr)->nextIns;

  if((insBaseSTPtr)->insStr) free((insBaseSTPtr)->insStr);

  free((insBaseSTPtr));
  (insBaseSTPtr) = 0;

  return nextIns;
} /*freeInsBase*/

/*-------------------------------------------------------\
| Fun-06: makeConBase
|   - Makes an initialized, heap allocated conBase
|     structure
| Input:
| Output:
|   - Returns:
|     o A Pointer to a conBase structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct conBase *
makeConBase(
){
   struct conBase *retMacST =
      malloc(sizeof(struct conBase));
   
   if(retMacST) initConBase(retMacST);
   
   return retMacST;
} /*makeInsBase*/

/*-------------------------------------------------------\
| Fun-07: freeConBaseStack
|   - Frees the heap allocated variables in a conBase
|     structure and also blanks the conBase structure
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure with variables
|      to free
| Output:
|   - Frees:
|     o Frees the conBaseST->insList 
|   - Sets:
|     o All values in conBaseSTPTr to 0
\-------------------------------------------------------*/
void
freeConBaseStack(
   struct conBase *conBaseSTPtr
){
  while((conBaseSTPtr)->insList)
  { /*Loop: Free each insertion for the position*/
     (conBaseSTPtr)->insList =
         freeInsBase((conBaseSTPtr)->insList);
  } /*Loop: Free each insertion for the position*/
  
  blankConBase((conBaseSTPtr));
} /*freeConBaseStack*/

/*-------------------------------------------------------\
| Fun-08: freeConBase
|   - Frees a conBase structure
| Input:
|   - conBaseSTPtr:
|     o A Pointer to pointer to an conBase structure to
|       free
| Output:
|   - Frees:
|     o Frees conBaseST
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
void
freeConBase(
   struct conBase **conBaseSTPtrPtr
){
  freeConBaseStack(*conBaseSTPtrPtr);
  free(*conBaseSTPtrPtr);
  *(conBaseSTPtrPtr) = 0;
} /*freeConBase*/

/*-------------------------------------------------------\
| Fun-09: freeConBaseAry
|   - Frees an array of conBase structures (assumes heap)
| Input:
|   - conBaseSTPtr:
|     o Pointer to an pointer to the start of an conBase
|       structure array to free
|   - lenAryI:
|     o Number of conBase structurs in the array
| Output:
|   - Frees:
|     o The array in conBaseSTPtr
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
void
freeConBaseAry(
   struct conBase **conBaseSTPtrPtr,
   int lenAryI
){
  int iIndexMac = 0;
  
  for(iIndexMac = 0; iIndexMac < (lenAryI); ++iIndexMac)
      freeConBaseStack((*conBaseSTPtrPtr) + iIndexMac);
  
  free(*conBaseSTPtrPtr);
  *conBaseSTPtrPtr = 0;
} /*freeConBase*/

/*-------------------------------------------------------\
| Fun-11: addReadToConBaseArray
|   - Adds the read into a conBase array
| Input:
|   - sameEntrySTPtr:
|     o Pointer to a samEntry structure with the read
|       to use
|   - conBaseAry:
|     o Point to a conBase structure array to add the read
|       to.  This is resized if it is to small.
|   - lenRefUI:
|     o Length of the reference sequence (use 0 for
|       unkown), is updated as conBaseSTAry expands.
|   - settings:
|     o Poinnter to a tbConSet structure with the min
|       Q-scores to keep an base or insertion
| Output:
|   - Modifies:
|     o conBaseSTAry to have bases in samSTPtr
|       consensus
|   - Returns:
|     o for no errors
|     o def_tbCon_samHead for sam file header entry
|     o def_tbCon_samNoMap for an unmapped read
|     o def_tbCon_samNoSeq for a read missing a sequence
|     o def_tbCon_Memory_Err for memory errors
\-------------------------------------------------------*/
char
addReadToConBaseArray(
   void *samEntrySTPtr,       /*read to add to consensus*/
   struct conBase *conBaseAry[], /*Consensus array*/
   unsigned int *lenRefUI,       /*Length of reference*/
   struct tbConSet *settings
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-11 TOC: addReadToConBaseArray
   '   - Adds the read into a conBase array
   '   o fun-11 sec-01:
   '     - Variable declerations
   '   o fun-11 sec-02:
   '     - Make sure I have enough bases
   '   o fun-11 sec-03:
   '     - Add bases to the consensus
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*For loops*/
   uint uiCig = 0;  /*For iterating throug cigars*/
   uint uiBase = 0; /*For iterating through bases*/
   uint uiRef = 0;  /*Position at in the reference*/
   uint endCigUI = 0; /*End of cigar entry*/

   /*For adding insertions to the consensus*/
   int iIns = 0;     /*Position at in insertion*/
   int lenInsI = 0;  /*Length of found ins*/
   char *insStr  = 0;  /*Memory to hold insertion*/
   struct insBase *insST = 0;
   struct insBase *lastInsST = 0;
 
   struct conBase *baseST = 0; /*For reallocs*/

   /*Set up the void return*/
   struct samEntry *samST =
      (struct samEntry *) samEntrySTPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^   - Make sure I have enough bases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samST->qryIdStr[0] =='\0') return def_tbCon_samHead;
   if(samST->flagUS & 4) return def_tbCon_samNoMap;
   if(samST->seqStr == 0) return def_tbCon_samNoSeq;
   if(samST->seqStr[0] == '*') return def_tbCon_samNoSeq;

   if(samST->mapqUC < settings->minMapqUC)
      return def_tbCon_samLowMapq;

   /*First time*/
   if(! (*conBaseAry))
   { /*If: First round*/
      if(*lenRefUI < samST->refEndUI)
         *lenRefUI = samST->refEndUI + 128;

      *conBaseAry =
         malloc(*lenRefUI * sizeof(struct conBase));

      if(! (*conBaseAry)) return def_tbCon_Memory_Err;

      /*Initialize all the strutures*/
      for(uiBase = 0; uiBase < *lenRefUI; ++uiBase)
          initConBase(&(*conBaseAry)[uiBase]);
   } /*If: First round*/

   /*Check if I am adding more bases*/
   else if(*lenRefUI < samST->refEndUI)
   { /*If: I need to add more bases*/
      uiBase = *lenRefUI;

      /*Make sure I am not doing small reallocs*/
      if(*lenRefUI >= samST->refEndUI - 128)
         *lenRefUI = samST->refEndUI + 128;

      else *lenRefUI = samST->refEndUI;

      baseST =
         realloc(
           *conBaseAry,
           *lenRefUI * sizeof(struct conBase)
         ); /*Add more memory for the bases*/

      if(! baseST) return def_tbCon_Memory_Err;

      * conBaseAry = baseST; 

      while(uiBase < *lenRefUI)
      { /*Loop: Initialize the new structures*/
          initConBase(conBaseAry[uiBase]);
          ++uiBase;
      } /*Loop: Initialize the new structures*/
   } /*If: I need to add more bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-03:
   ^   - Add bases to the consensus
   ^   o fun-11 sec-03 sub-01:
   ^     - setup, loop through the cigar, and switch case
   ^   o fun-11 sec-03 sub-02:
   ^     - Check the matches and snp cases
   ^   o fun-11 sec-03 sub-03:
   ^     - Check the deletion cases
   ^   o fun-11 sec-03 sub-04:
   ^     - Check the insertion cases
   ^   o fun-11 sec-03 sub-05:
   ^     - Handle softmasking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-11 Sec-03 Sub-01:
   *   - setup, loop through the cigar, and switch case
   \*****************************************************/

   uiBase = 0;
   uiRef = samST->refStartUI;

   for(uiCig = 0; uiCig < samST->lenCigUI; ++uiCig)
   { /*Loop: Add bases to the consensus*/

      /*`& ~32` converts to uppercase, but does not
      ` change '=' or '\t'
      */
      switch(samST->cigTypeStr[uiCig])
      { /*Switch: Check the cigar entry type*/

         /***********************************************\
         * Fun-11 Sec-03 Sub-02:
         *   - Check the matches and snp cases
         \***********************************************/

         case 'M':
         case 'X':
         case '=':
         /*Case: match or snp*/
            endCigUI = uiBase + samST->cigValAryI[uiCig];

            while(uiBase < endCigUI)
            { /*Loop: Add matchs/snps to consensus*/
               ++((*conBaseAry)[uiRef].totalBasesI);

               if   ((samST->qStr[0] != '*')
                  & (samST->qStr[1] != '\0')
               ){ /*If: I have a q-score entry*/
                  if(  samST->qStr[uiBase] - defQAdjust
                     < settings->minQI
                  ){ /*If: the base is beneath min Q*/
                     ++uiBase;
                     ++uiRef;
                     continue; /*Base under min Q-score*/
                  } /*If: the base is beneath min Q*/
               } /*If: I have a q-score entry*/

               /*Incurmenting total base count here so
               `  masked positions are still kept
               */
               ++((*conBaseAry)[uiRef].totalBasesKeptI);

               switch(samST->seqStr[uiBase] & ~32)
               { /*Switch: Check the base type*/
                  case 'A':
                     ++((*conBaseAry)[uiRef].numAI);
                     break;

                  case 'T':
                     ++((*conBaseAry)[uiRef].numTI);
                     break;

                  case 'C':
                     ++((*conBaseAry)[uiRef].numCI);
                     break;

                  case 'G':
                     ++((*conBaseAry)[uiRef].numGI);
                     break;
               } /*Switch: Check the base type*/

               ++uiBase;
               ++uiRef;
            } /*Loop: Add matchs/snps to consensus*/

            break;
         /*Case: match or snp*/

         /***********************************************\
         * Fun-11 Sec-03 Sub-03:
         *   - Check the deletion cases
         \***********************************************/

         case 'D':
         /*Case: Deletion*/ 
            endCigUI = uiRef + samST->cigValAryI[uiCig];

            while(uiRef < endCigUI)
            { /*Loop: Add in the deletions*/
               ++((*conBaseAry)[uiRef].totalBasesI);
               ++((*conBaseAry)[uiRef].totalBasesKeptI);
               ++((*conBaseAry)[uiRef].numDelI);
               ++uiRef;
            } /*Loop: Add in the deletions*/

            break;
         /*Case: Deletion*/ 

         /***********************************************\
         * Fun-11 Sec-03 Sub-04:
         *   - Check the insertion cases
         \***********************************************/

         case 'I':
         /*Case: Insertions*/
            lenInsI = samST->cigValAryI[uiCig];
            endCigUI = uiBase + samST->cigValAryI[uiCig];
            insStr = malloc((lenInsI +1) * sizeof(char));
            iIns = 0;

            if(! insStr) return def_tbCon_Memory_Err;

            /*Get the total Q-score for the insertion*/
            while(uiBase < endCigUI)
            { /*Loop: Copy the insertion*/

              if   ((samST->qStr[0] != '*')
                  & (samST->qStr[1] != '\0')
              ){ /*If: I have a q-score entry*/
                 if(  samST->qStr[uiBase] - defQAdjust
                    < settings->minInsQI
                 ){ /*If: This insertion is low quality*/
                    ++uiBase;
                    continue;
                 } /*If: This insertion is low quality*/
              } /*If: I have a q-score entry*/

              insStr[iIns] = samST->seqStr[uiBase];

              ++uiBase;
              ++iIns;
            } /*Loop: Copy the insertion*/
 
            if(iIns == 0)
            { /*If: I discarded the insertion*/
               free(insStr);
               insStr = 0;
               break;
            } /*If: I discarded the insertion*/

            lenInsI = iIns;
            insStr[lenInsI] = '\0';

            insST = (*conBaseAry)[uiRef].insList;
            lastInsST = insST;

            while(insST)
            { /*Loop:check if insertion is a match*/
               if(lenInsI != insST->lenInsI)
               { /*If: lengths are differnt (not match)*/
                  lastInsST = insST;
                  insST = insST->nextIns;
                  continue;
               } /*If: lengths are differnt (not match)*/
 
               /*Check if I have an match*/
               for(
                  iIns = 0;
                  insStr[iIns] == insST->insStr[iIns];
                  ++iIns
               ) if(insStr[iIns] =='\0') break;

               if(insStr[iIns] =='\0')
               { /*If: I found a match*/
                  free(insStr);
                  insStr = 0;
                  ++(insST->numHitsI);
                  break;
               } /*If: I found a match*/

               lastInsST = insST;
               insST = insST->nextIns;
            } /*Loop:check if insertion is a match*/
 
            if(! insST)
            { /*If: I did not find a match*/

               if(lastInsST)
               { /*If: there are previous insertions*/
                  lastInsST->nextIns = makeInsBase();

                  if(! (lastInsST->nextIns))
                  { /*If: I had a memory error*/
                     free(insStr);
                     return def_tbCon_Memory_Err;
                  } /*If: I had a memory error*/

                  lastInsST = lastInsST->nextIns;
               } /*If: there are previous insertions*/

               else
               { /*Else: This is the first insertion*/
                  (*conBaseAry)[uiRef].insList =
                     makeInsBase();

                  lastInsST= (*conBaseAry)[uiRef].insList;
               } /*Else: This is the first insertion*/
 
               lastInsST->insStr = insStr;
               lastInsST->lenInsI = lenInsI;
               lastInsST->numHitsI = 1;

               insStr = 0;
               lenInsI = 0;
            } /*If: I did not find a match*/

            break;
         /*Case: Insertions*/

         /***********************************************\
         * Fun-11 Sec-03 Sub-05:
         *   - Handle softmasking
         \***********************************************/

         case 'S':
         /*Case: soft masking*/
            endCigUI = uiBase + samST->cigValAryI[uiCig];
            while(uiBase < endCigUI) ++uiBase;
            break;
         /*Case: soft masking*/

         /*Other cases invovle hard masking*/
      } /*Switch: Check the cigar entry type*/
   } /*Loop: Add bases to the consensus*/

   return 0;
}/*addReadToConBaseArray*/

/*-------------------------------------------------------\
| Fun-12: collapseConBaseArray
|   - Collapses a conBase array into an array of samEntry
|     structures
| Input:
|   - conBaseAry:
|     o Point to a conBase structure array to collapse
|   - lenConAryUI:
|     o Length of the conBase array
|   - lenSamAryI:
|     o Pointer to integer to hold number of samEntry
|       structures made
|   - minLenI:
|     o Minimum length of a consensus (otherwise pitch)
|   - minDepthI:
|     o Minimum read depth to not mask an snp/match, and
|       not remove an deltion or insertion
|   - minPercSnpF:
|     o Minimum percentage of bases needed to keep an snp
|   - minPerDelF:
|     o Minimum percentage of bases needed to keep (not
|       mask) a deletion
|   - minPerInsF:
|     o Minimum percentage of bases needed to keep an ins.
|       If min depth is present and I have the percenate
|       support for an insertion, but there is not enough
|       support in percentage for a single ins, then mask
|       most supported.
|   - refIdStr:
|     o c-string with referernce sequence name
|   - maskC:
|     o Character to mask with
|   - errC:
|     o Pointer to character to hold error type
| Output:
|   - Modifies:
|     o lenSamST to hold the returned samEntry arrray
|       length
|     o errC set to 0 for no errors
|     o errC set to 1 for nothing meets mean stats
|     o errC set to 64 for memory errors
|   - Returns:
|     o An array of sam entry structures with the
|       consensus. Idealy this would be one structure,
|       but for amplicons it will be an array.
|     o 0 for memory erors
\-------------------------------------------------------*/
void *
collapseConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   unsigned int lenConAryUI,    /*Length of consensus*/
   int *lenSamAryI,          /*Holds output array length*/
   char *refIdStr,             /*Name of reference seq*/
   struct tbConSet *settings,  /*settings for collapsing*/
   char *errC                  /*Error reports*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-12 TOC: collapseConBaseAry
   '   - 
   '   o fun-12 sec-01:
   '     - Variable declerations
   '   o fun-12 sec-02:
   '     - Count the number of fragments I have
   '   o fun-12 sec-03:
   '     - Allocate memory for each fragment
   '   o fun-12 sec-04:
   '     - Collapse each fragment into a samEntry struct
   '   o fun-12 sec-05:
   '     - Collapse the consensus fragment
   '   o fun-12 sec-06:
   '     - Handle clean up for memory errors
   '   o fun-12 sec-07:
   '     - Handle clean up for successful collapse
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int iFrag = 0;
   uint uiRef = 0;     /*Ref base on*/
   uint uiEndRef = 0;  /*Last base in amplicon/fragment*/
   uint lenFragUI = 0; /*Length of fragment/amplicon*/
   uint uiBase = 0;    /*Position on fragment (base on)*/

   uint uiCig = 0;     /*Cigar entry on*/
   uint lenCigUI = 0;
   char lastCigC = '0';
   uint extraInsNtI = 0; /*max extra bases from ins's*/

   /*General support*/
   float snpPerSupF = 0;
   float insPerSupF = 0;
   float delPerSupF = 0;

   /*Support for a particler snp/match*/
   uint nonMaskBaseUI = 0; /*number of non-anonymous*/
   float aPercSupF = 0;
   float tPercSupF = 0;
   float cPercSupF = 0;
   float gPercSupF = 0;

   /*Find the most supported insertion*/
   int keptReadsI = 0;
   struct insBase *curInsST = 0;
   struct insBase *bestInsST = 0;

   struct samEntry *retSamST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-02:
   ^   - Count the number of fragments I have
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *lenSamAryI = 0;
   *errC = 0;
   uiRef = 0;

   while(uiRef < lenConAryUI)
   { /*Loop: Find the number of fragments*/

      /*Find the starting position for this fragment*/
      while(
           conBaseAry[uiRef].totalBasesKeptI
         < settings->minDepthI
      ){ /*Loop: Find the first base with enough depth*/
         ++uiRef;
         if(uiRef >= lenConAryUI) goto collapseFragments;
      } /*Loop: Find the first base with enough depth*/

      /*Find the length of this fragment*/
      for(
         uiEndRef = uiRef;
           conBaseAry[uiEndRef].totalBasesKeptI
         >= settings->minDepthI;
         ++uiEndRef
      ) if(uiEndRef >= lenConAryUI) break;

      /*Find the length*/
      lenFragUI = uiRef - uiEndRef;

      if(lenFragUI < settings->minLenI)
      { /*If: This is beneath the min length to keep*/
         uiRef = uiEndRef;
         continue;
      } /*If: This is beneath the min length to keep*/

      ++(*lenSamAryI);

      uiRef = uiEndRef;
   } /*Loop: Find the number of fragments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-03:
   ^   - Allocate memory for each fragment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   collapseFragments:;

   if(*lenSamAryI == 0)
   { /*If: not enough depth to build a consensus*/
      *errC = 1;
      return 0;
   } /*If: not enough depth to build a consensus*/

   retSamST=malloc(*lenSamAryI * sizeof(struct samEntry));

   if(!retSamST)
   { /*If: I had a memory error*/
      *errC = 64;
      return 0;
   } /*If: I had a memory error*/

   /*Initialize the structures*/
   for(iFrag = 0; iFrag < *lenSamAryI; ++iFrag)
      if(initSamEntry(&retSamST[iFrag])) goto memoryError;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-04:
   ^   - Collapse each fragment into a samEntry struct
   ^   o fun-12 sec-01 sub-01:
   ^     - Get length & check if fragment meets min length
   ^   o fun-12 sec-01 sub-02:
   ^     - Find the length and memory I need to allocate
   ^   o fun-12 sec-01 sub-03:
   ^     - Set up the sam entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-12 Sec-01 Sub-01:
   *   - Get length & check if fragment meets min length
   \*****************************************************/

   *errC = 0;
   uiRef = 0;
   iFrag = 0;

   while(uiRef < lenConAryUI)
   { /*Loop: Find the number of fragments*/

      /*Find the starting position for this fragment*/
      while(
           conBaseAry[uiRef].totalBasesKeptI
         < settings->minDepthI
      ){ /*Loop: Find the first base with enough depth*/
         ++uiRef;
         if(uiRef >= lenConAryUI) goto cleanUp;
      } /*Loop: Find the first base with enough depth*/

      /**************************************************\
      * Fun-12 Sec-01 Sub-02:
      *   - Find the length and memory I need to allocate
      *   o fun-12 sec-04 sub-02 cat-01:
      *     - Find the length/start memory count loop
      *   o fun-12 sec-04 sub-02 cat-02:
      *     - Find the percent support for each option
      *   o fun-12 sec-04 sub-02 cat-03:
      *     - Check if have ins (if so get extra bases)
      *   o fun-12 sec-04 sub-02 cat-04:
      *     - Check if an snp/match, del, or mask will
      *       be choosen
      *   o fun-12 sec-04 sub-02 cat-05:
      *     - Find fragement length/check if long enough
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-02 Cat-01:
      +   - Find the length/start memory count loop
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      extraInsNtI = 0;

      for(
         uiEndRef = uiRef;
            conBaseAry[uiEndRef].totalBasesKeptI
         >= settings->minDepthI;
         ++uiEndRef
      ){ /*Loop: Find the memory for the fragment*/
         if(uiEndRef >= lenConAryUI) break;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-12 Sec-04 Sub-02 Cat-02:
         +   - Find the percent support for each option
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         /*The insertion is inbetween bases, which makes
         ` it hard to gauge. So, I am taking the worst
         ` case
         */
         keptReadsI =
           noBranchMax(
             conBaseAry[uiEndRef].totalBasesKeptI,
             conBaseAry[
                uiEndRef - (uiEndRef > 0)
             ].totalBasesKeptI
           );
            /*uiEndRef - (uiEndRef > 0) turns into
            `  uiEndRef - 1, when uiEndRef is > 0; else is
            `  uiEndRef
            */

         insPerSupF =
              (float) conBaseAry[uiEndRef].numInsI
            / (float) keptReadsI;

         delPerSupF =
              (float) conBaseAry[uiEndRef].numDelI
            / (float) conBaseAry[uiEndRef].totalBasesKeptI;

         snpPerSupF = 1 - delPerSupF;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-12 Sec-04 Sub-02 Cat-03:
         +   - Check if have ins (if so get extra bases)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(insPerSupF >= settings->minPercInsF)
         { /*If: I have an insertion*/
            lenCigUI += (lastCigC != 'I');
            lastCigC = 'I';

            bestInsST = conBaseAry[uiEndRef].insList;
            curInsST = bestInsST;

            while(curInsST)
            { /*Loop: Find the most supported insertion*/
              if(bestInsST->numHitsI < curInsST->numHitsI)
                 bestInsST = curInsST;

              curInsST = curInsST->nextIns;
            } /*Loop: Find the most supported insertion*/

            /*How many bases the insertions could add*/
            extraInsNtI += bestInsST->lenInsI;
         } /*If: I have an insertion*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-12 Sec-04 Sub-02 Cat-04:
         +   - Check if an snp/match, del, or mask will
         +     be choosen
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(snpPerSupF >= settings->minPercSnpF)
         { /*If: I am keeping an mathc/snp*/
            lenCigUI += (lastCigC != 'M');
            lastCigC = 'M';
         } /*If: I am keeping an mathc/snp*/

         else if(delPerSupF >= settings->minPercDelF)
         { /*Else If: I am keeping an deletion*/
            lenCigUI += (lastCigC != 'D');
            lastCigC = 'D';
         } /*Else If: I am keeping an deletion*/

         /*For the cigar a mask is same as an snp/match*/
         else
         { /*Else: I am masking (snp/match case)*/
            lenCigUI += (lastCigC != 'M');
            lastCigC = 'M';
         } /*Else: I am masking (snp/match case)*/
      } /*Loop: Find the memory for the fragment*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-02 Cat-05:
      +   - Find the fragement length/check if long enough
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*Find the length*/
      lenFragUI = uiEndRef - uiRef;

      if(lenFragUI < settings->minLenI)
      { /*If: This is beneath the min length to keep*/
         uiRef = uiEndRef;
         continue;
      } /*If: This is beneath the min length to keep*/

      /**************************************************\
      * Fun-12 Sec-04 Sub-03:
      *   - Set up the sam entry
      *   o fun-12 sec-04 sub-03 cat-01:
      *     - Make the query id
      *   o fun-12 sec-04 sub-03 cat-02:
      *     - Make the reference id
      *   o fun-12 sec-04 sub-03 cat-03:
      *     - Set up the RNEXT entry
      *   o fun-12 sec-04 sub-03 cat-04:
      *     - Set up reference positions/alinged length
      *   o fun-12 sec-04 sub-03 cat-05:
      *     - Allocate memory for the sequence
      *   o fun-12 sec-04 sub-03 cat-06:
      *     - Set up the Q-score entry
      *   o fun-12 sec-04 sub-03 cat-07:
      *     - Allocate memory for the cigar types buffer
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-01:
      +   - Make the query id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      retSamST[iFrag].lenQryIdUC =
         numToStr(retSamST[iFrag].qryIdStr, uiRef);

      retSamST[iFrag].qryIdStr[retSamST[iFrag].lenQryIdUC]
         = '-';

      ++retSamST[iFrag].lenQryIdUC;

      retSamST[iFrag].lenQryIdUC += (uchar)
         numToStr(
            &(retSamST[iFrag].qryIdStr[
               retSamST[iFrag].lenQryIdUC
            ]),
            uiEndRef
         ); /*Copy the ending position*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-02:
      +   - Make the reference id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*Copy the reference id*/
      retSamST[iFrag].lenRefIdUC =
         ulCpStrDelim(
            retSamST[iFrag].refIdStr,
            refIdStr,
            0, /*'\0' = 0*/
            '\0'
         ); /*Copy the reference name*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-03:
      +   - Set up the RNEXT entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      retSamST[iFrag].rNextStr[0] = '*';
      retSamST[iFrag].rNextStr[1] = '\0';
      retSamST[iFrag].lenRNextUC = 1;

      /*The flag is already set up*/
      /*Not sure what to put for mapping quality, so using
      ` 0 (default)
      */

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-04:
      +   - Set up the reference positions/alinged length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*Get the reference lengths*/
      retSamST[iFrag].refStartUI = uiRef;
      retSamST[iFrag].alnReadLenUI = lenFragUI;
      retSamST[iFrag].refEndUI = uiEndRef;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-05:
      +   - Allocate memory for the sequence
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      lenFragUI += extraInsNtI;

      /*Make the sequence buffer*/
      if(retSamST[iFrag].lenSeqBuffUI < lenFragUI + 1)
      { /*If: I need to expand the memory*/
         free(retSamST[iFrag].seqStr);

         retSamST[iFrag].seqStr =
            malloc((lenFragUI + 1) * (sizeof(char)));

         if(!retSamST[iFrag].seqStr) goto memoryError;

         retSamST[iFrag].lenSeqBuffUI = lenFragUI;
      } /*If: I need to expand the memory*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-06:
      +   - Set up the Q-score entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*initSamEntry allocates over 2 bytes, so I am
      ` safe here
      */
      retSamST[iFrag].qStr[0] = '*';
      retSamST[iFrag].qStr[1] = '\0';
      retSamST[iFrag].lenQBuffUI = 2;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-12 Sec-04 Sub-03 Cat-07:
      +   - Allocate memory for the cigar types buffer
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(retSamST[iFrag].lenCigBuffUI < lenCigUI + 1)
      { /*If: I need to allocate memory for cigar buff*/
      /*Make the cigar types buffer*/
          retSamST[iFrag].cigTypeStr =
             malloc((lenCigUI + 1) * sizeof(char));

          if(!retSamST[iFrag].cigTypeStr)
             goto memoryError;

          retSamST[iFrag].lenCigBuffUI = lenCigUI;

          retSamST[iFrag].cigValAryI =
             malloc((lenCigUI + 1) * sizeof(int));

          if(!retSamST[iFrag].cigValAryI)
             goto memoryError;
      } /*If: I need to allocate memory for cigar buff*/
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun-12 Sec-05:
      ^   - Collapse the consensus fragment
      ^   o fun-12 sec-05 sub-01:
      ^     - Get the amount of support for each position
      ^   o fun-12 sec-05 sub-02:
      ^     - Check if I have an insertion. In this case
      ^       the insertion is always one base behind
      ^   o fun-12 sec-05 sub-03:
      ^     - Check if I have support for an snp/match
      ^   o fun-12 sec-05 sub-04:
      ^     - Check if this was a deletion instead of snp
      ^   o fun-12 sec-05 sub-05:
      ^     - Handle Not enough support cases; likely
      ^       mixed infections
      ^   o fun-12 sec-05 sub-06:
      ^     - Check if I need to resize sequence buffer
      ^   o fun-12 sec-05 sub-07:
      ^     - Check if I need to resize the cigar buffer
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      /**************************************************\
      * Fun-12 Sec-05 Sub-01:
      *   - Get the amount of support for each position
      \**************************************************/

      uiBase = 0;
      uiCig = 0;

      retSamST[iFrag].cigTypeStr[uiCig] = '\0';

      while(uiRef < uiEndRef)
      { /*Loop: Collapse the fragment*/

         /*The insertion is inbetween bases, which makes
         ` it hard to gauge. So, I am taking the worst
         ` case. Here I want all positoins
         */
         keptReadsI =
           noBranchMax(
             conBaseAry[uiRef].totalBasesKeptI,
             conBaseAry[uiRef -(uiRef >0)].totalBasesKeptI
           );
            /*uiRef - (uiRef > 0) turns into
            `  uiRef - 1, when uiRef is > 0; else is
            `  uiRef
            */

         insPerSupF =
              (float) conBaseAry[uiRef].numInsI
            / (float) keptReadsI;

         /*For deletions an masked base is equivlent to no
         `   support
         */
         delPerSupF = (float) conBaseAry[uiRef].numDelI;

         delPerSupF /=
            (float) conBaseAry[uiRef].totalBasesKeptI;

         /*Find the number of non-anonymous bases
         `   For tbCon all anonymous bases are N's (masked)
         */
         nonMaskBaseUI = conBaseAry[uiRef].numAI;
         nonMaskBaseUI += conBaseAry[uiRef].numTI;
         nonMaskBaseUI += conBaseAry[uiRef].numGI;
         nonMaskBaseUI += conBaseAry[uiRef].numCI;

         snpPerSupF = nonMaskBaseUI;
         nonMaskBaseUI += conBaseAry[uiRef].numDelI;
         snpPerSupF /= (float) nonMaskBaseUI;

         /***********************************************\
         * Fun-12 Sec-05 Sub-02:
         *   - Check if I have an insertion. In this case
         *     the insertion is always one base behind
         *   o fun-12 sec-05 sub-02 cat-01:
         *     - Check if need new cigar entry + find
         *       insertion with the most support
         *   o fun-12 sec-05 sub-02 cat-01:
         *     - Check if keeping inse / copy insertion
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-12 Sec-05 Sub-02 Cat-01:
         +   - Check if need new cigar entry + find
         +     insertion with the most support
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(insPerSupF >= settings->minPercInsF)
         { /*If: An insertions is supported here*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'I')
            { /*If: This is a new cigar entry*/
               ++uiCig;
               retSamST[iFrag].cigTypeStr[uiCig] = 'I';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            bestInsST = conBaseAry[uiRef].insList;
            curInsST = bestInsST;

            while(curInsST)
            { /*Loop: Find the most supported insertion*/
              if(bestInsST->numHitsI < curInsST->numHitsI)
                 bestInsST = curInsST;

              curInsST = curInsST->nextIns;
            } /*Loop: Find the most supported insertion*/

            keptReadsI = curInsST->numHitsI;

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun-12 Sec-05 Sub-02 Cat-02:
            +   - Check if keeping ins / copy insertion
            \+++++++++++++++++++++++++++++++++++++++++++*/

            insPerSupF =
                 (float) bestInsST->numHitsI
               / (float) keptReadsI;

            if(insPerSupF >= settings->minPercInsF)
            { /*If: I am keeping the insertion*/
               ulCpStr(
                  &(retSamST[iFrag].seqStr[uiBase]),
                  bestInsST->insStr,
                  bestInsST->lenInsI
               );

               uiBase += bestInsST->lenInsI;

               retSamST[iFrag].cigValAryI[uiCig] +=
                  bestInsST->lenInsI;

               retSamST[iFrag].numInsUI +=
                  bestInsST->lenInsI;
            } /*If: I am keeping the insertion*/
         } /*If: an Insertion is supported here*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-03:
         *   - Check if I have support for an snp/match
         *   o fun-12 sec-05 sub-03 cat-01:
         *     - Check if need new cigar entry + find
         *       support for each base type
         *   o fun-12 sec-05 sub-03 cat-01:
         *     - Find the base with the most support and
         *       see if it had enough support to keep
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun-12 Sec-05 Sub-03 Cat-01:
         +   - Check if need new cigar entry + find
         +     support for each base type
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(nonMaskBaseUI < settings->minDepthI)
            goto maskPos_fun12_sec05_sub05;

         else if(snpPerSupF >= settings->minPercSnpF)
         { /*If: snps were the best choice*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'M')
            { /*If: This is a new cigar entry*/
               uiCig +=
                  (retSamST[iFrag].cigTypeStr[uiCig]
                   != '\0'
                  );

               retSamST[iFrag].cigTypeStr[uiCig] = 'M';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

             aPercSupF = (float) conBaseAry[uiRef].numAI;
             aPercSupF /= (float) nonMaskBaseUI;

             tPercSupF = (float) conBaseAry[uiRef].numTI;
             tPercSupF /= (float) nonMaskBaseUI;

             gPercSupF = (float) conBaseAry[uiRef].numGI;
             gPercSupF /= (float) nonMaskBaseUI;

             cPercSupF = (float) conBaseAry[uiRef].numCI;
             cPercSupF /= (float) nonMaskBaseUI;

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun-12 Sec-05 Sub-03 Cat-02:
            +   - Find the base with the most support and
            +     see if it had enough support to keep
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(   aPercSupF > tPercSupF
               && aPercSupF > cPercSupF
               && aPercSupF > gPercSupF
            ) { /*If: A has the most support*/
               if(aPercSupF >= settings->minPercSnpF)
               { /*If: this was an A*/
                  retSamST[iFrag].seqStr[uiBase] = 'A';
                  ++retSamST[iFrag].numMatchUI;
               } /*If: this was an A*/

               else
               { /*Else: not enough support to call*/
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;

                  ++retSamST[iFrag].numMaskUI;
               } /*Else: not enough support to call*/
            } /*If: A has the most support*/

            else if(
                  tPercSupF > cPercSupF
               && tPercSupF > gPercSupF
            ) { /*Else If: T has the most support*/

               if(tPercSupF >= settings->minPercSnpF)
               { /*If: this was an T*/
                  retSamST[iFrag].seqStr[uiBase] = 'T';
                  ++retSamST[iFrag].numMatchUI;
               } /*If: this was an T*/

               else
               { /*Else: not enough support to call*/
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;

                  ++retSamST[iFrag].numMaskUI;
               } /*Else: not enough support to call*/
            } /*Else If: T has the most support*/

            else if(cPercSupF > gPercSupF)
            { /*Else If: C has the most support*/
               if(cPercSupF >= settings->minPercSnpF)
               { /*If: this was an C*/
                  retSamST[iFrag].seqStr[uiBase] = 'C';
                  ++retSamST[iFrag].numMatchUI;
               } /*If: this was an C*/

               else
               { /*Else: not enough support to call*/
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;

                  ++retSamST[iFrag].numMaskUI;
               } /*Else: not enough support to call*/
            } /*Else If: C has the most support*/

            else
            { /*Else If: G has the most support*/
               if(gPercSupF >= settings->minPercSnpF)
               { /*If: this was an G*/
                  retSamST[iFrag].seqStr[uiBase] = 'G';
                  ++retSamST[iFrag].numMatchUI;
               } /*If: this was an G*/

               else
               { /*Else: not enough support to call*/
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;

                  ++retSamST[iFrag].numMaskUI;
               } /*Else: not enough support to call*/
            } /*Else If: G has the most support*/

            ++uiBase;
            ++(retSamST[iFrag].cigValAryI[uiCig]);
         } /*If: snps were the best choice*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-04:
         *   - Check if this was a deletion instead of snp
         \***********************************************/

         /*Do nothing if deletion was selected*/
         else if(delPerSupF >= settings->minPercDelF)
         { /*Else if: there was a deletion*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'D')
            { /*If: This is a new cigar entry*/
               uiCig +=
                  (retSamST[iFrag].cigTypeStr[uiCig]
                   != '\0'
                  );

               retSamST[iFrag].cigTypeStr[uiCig] = 'D';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            ++(retSamST[iFrag].cigValAryI[uiCig]);
            ++retSamST[iFrag].numDelUI;
         } /*Else if: there was a deletion*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-05:
         *   - Handle Not enough support cases; likely
         *     mixed infections
         \***********************************************/

         else
         { /*Else: I have no support, assume mask snp*/
            maskPos_fun12_sec05_sub05:;

            if(retSamST[iFrag].cigTypeStr[uiCig] != 'M')
            { /*If: This is a new cigar entry*/
               uiCig +=
                  (retSamST[iFrag].cigTypeStr[uiCig]
                   != '\0'
                  );

               retSamST[iFrag].cigTypeStr[uiCig] = 'M';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            retSamST[iFrag].seqStr[uiBase] =
               settings->maskC;

            ++uiBase;
            ++(retSamST[iFrag].cigValAryI[uiCig]);
            ++retSamST[iFrag].numMaskUI;
         } /*Else: I have no support, assume mask snp*/

         ++uiRef;
      } /*Loop: Collapse the fragment*/

      retSamST[iFrag].readLenUI = uiBase;
      retSamST[iFrag].lenCigUI = uiCig + 1;
         /*The read length is at index 1, but the cigar is
         `   at index 0, so needs a + 1
         */

      ++iFrag;
   } /*Loop: Find the number of fragments*/

   goto cleanUp;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-06:
   ^   - Handle clean up for memory errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   memoryError:;

   while(iFrag > -1)
   { /*Loop: Free fragments*/
     freeSamEntryStack(&(retSamST[iFrag]));
     --iFrag;
   } /*Loop: Free fragments*/

   free(retSamST);

   *errC = 64;
   return 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-07:
   ^   - Handle clean up for successful collapse
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp:;

   return retSamST;
} /*collapseConBaseArray*/

/*-------------------------------------------------------\
| Fun-13: pConBaseArray
|   - Print all entries in a conBase structure array that
|     are above the minimum read depth
| Input:
|   - conBaseAry:
|     o Point to a conBase structure array to print
|   - lenConAryUI:
|     o Length of the conBase array
|   - minDepthI:
|     o Minimum read depth to print out a position
|   - refIdStr:
|     o c-string with referernce sequence name
|   - outFILE:
|     o C-string with name of file to print everything to
| Output:
|   - Prints:
|     o Entries in conBaseAry to outFILE
|       length
|   - Returns:
|     o 0 for success
|     o def_tbCon_File_Err for file errors
\-------------------------------------------------------*/
char
pConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   unsigned int lenConAryUI,    /*Length of conBaseAry*/
   char *refIdStr,           /*Name of reference seq*/
   struct tbConSet *settings,/*Settings for printing*/
   char *outStr              /*File to print to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-13 TOC: pConBaseArray
   '   o fun-13 sec-01:
   '     - Variable declerations
   '   o fun-13 sec-02:
   '     - Print out the header
   '   o fun-13 sec-03:
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiBase = 0;
   struct insBase *insST = 0;

   uint nonMaskBaseUI = 0; /*number of non-anonymous*/
   uint maskedBasesUI = 0; /*number of non-anonymous*/
   float percSupF = 0;

   int insLeastSupI = 0;
   int insTotalI = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-02:
   ^   - Print out the header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outStr || *outStr == '*') outFILE = stdout;

   else
   { /*Else: I need to open a file*/
      outFILE = fopen(outStr, "w");
      if(! outFILE) return def_tbCon_File_Err;
   } /*Else: I need to open a file*/

   /*Print the header*/
   fprintf(
      outFILE,
      "refId\tposition\ttype\tsequence\tsupport"
   );

   fprintf(
     outFILE,
     "\tpercSupport\tmasked\tkeptBases\ttotalBases\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-03:
   ^   - Print out each base
   ^   o fun-13 sec-03 sub-01:
   ^     - Start loop and print insertion entries
   ^   o fun-13 sec-03 sub-02:
   ^     - Print out the snp/match entry for A
   ^   o fun-13 sec-03 sub-03:
   ^     - Print out the snp/match entry for T
   ^   o fun-13 sec-03 sub-04:
   ^     - Print out the snp/match entry for C
   ^   o fun-13 sec-03 sub-05:
   ^     - Print out the snp/match entry for G
   ^   o fun-13 sec-03 sub-06:
   ^     - Print out the deletion entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-13 Sec-03 Sub-01:
   *   - Start loop and print insertion entries
   \*****************************************************/

   for(uiBase = 0; uiBase < lenConAryUI; ++uiBase)
   { /*Loop: Print out the consensus*/

      if(!uiBase) goto firstBaseNoIns;

      insST = conBaseAry[uiBase].insList;

      insLeastSupI =
        noBranchMax(
           conBaseAry[uiBase].totalBasesKeptI,
           conBaseAry[uiBase - 1].totalBasesKeptI
        );

     insTotalI =
        noBranchMax(
           conBaseAry[uiBase].totalBasesI,
           conBaseAry[uiBase - 1].totalBasesI
        );

      while(insST)
      { /*Loop: Check insertions*/
         if(insST->numHitsI < settings->printMinDepthI)
         { /*If: There is not enough read depth*/
            insST = insST->nextIns;
            continue;
         } /*If: There is not enough read depth*/

         percSupF =
          (float) insST->numHitsI/(float) insLeastSupI;

         if(percSupF < settings->printMinSupInsF)
         { /*If: I am not printing this ins*/
            insST = insST->nextIns;
            continue;
         } /*If: I am not printing this ins*/

         fprintf(
            outFILE,
            "%s\t%i\tins\t%s\t%i\t%f\tNA\t%i\t%i\n",
            refIdStr,
            uiBase,
            insST->insStr,
            insST->numHitsI,
            percSupF,
            insLeastSupI,
            insTotalI
         );

         insST = insST->nextIns;
      } /*Loop: Check insertions*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-02:
      *   - Print out the snp/match entry for A
      \**************************************************/

      firstBaseNoIns:;

      if(
           conBaseAry[uiBase].totalBasesKeptI
         < settings->printMinDepthI
      ) continue; /*Not enough support to print out*/

      nonMaskBaseUI = conBaseAry[uiBase].numAI;
      nonMaskBaseUI += conBaseAry[uiBase].numTI;
      nonMaskBaseUI += conBaseAry[uiBase].numGI;
      nonMaskBaseUI += conBaseAry[uiBase].numCI;
      nonMaskBaseUI += conBaseAry[uiBase].numDelI;

      maskedBasesUI = conBaseAry[uiBase].totalBasesKeptI;
      maskedBasesUI -= nonMaskBaseUI;

      percSupF = (float) conBaseAry[uiBase].numAI;
      percSupF /= (float) nonMaskBaseUI;


      if(
           conBaseAry[uiBase].numAI
         < settings->printMinDepthI
      ) ;

      else if(percSupF >= settings->printMinSupSnpF)
      { /*If: I had enough support to print out A*/
         fprintf(
            outFILE,
            "%s\t%i\tbase\tA\t%i\t%f\t%u\t%i\t%i\n",
            refIdStr,
            uiBase + 1,
            conBaseAry[uiBase].numAI,
            percSupF,
            maskedBasesUI,
            conBaseAry[uiBase].totalBasesKeptI,
            conBaseAry[uiBase].totalBasesI
         );
      } /*If: I had enough support to print out A*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-03:
      *   - Print out the snp/match entry for T
      \**************************************************/

      percSupF = (float) conBaseAry[uiBase].numTI;
      percSupF /= (float) nonMaskBaseUI;

      if(
           conBaseAry[uiBase].numTI
         < settings->printMinDepthI
      ) ;

      else if(percSupF >= settings->printMinSupSnpF)
      { /*If: I had enough support to print an T*/
         fprintf(
            outFILE,
            "%s\t%i\tbase\tT\t%i\t%f\t%u\t%i\t%i\n",
            refIdStr,
            uiBase + 1,
            conBaseAry[uiBase].numTI,
            percSupF,
            maskedBasesUI,
            conBaseAry[uiBase].totalBasesKeptI,
            conBaseAry[uiBase].totalBasesI
         );
      } /*If: I had enough support to print an T*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-04:
      *   - Print out the snp/match entry for C
      \**************************************************/

      percSupF = (float) conBaseAry[uiBase].numCI;
      percSupF /= (float) nonMaskBaseUI;

      if(
           conBaseAry[uiBase].numCI
         < settings->printMinDepthI
      ) ;

      else if(percSupF >= settings->printMinSupSnpF)
      { /*If: I had enough support to print an C*/
         fprintf(
            outFILE,
            "%s\t%i\tbase\tC\t%i\t%f\t%u\t%i\t%u\n",
            refIdStr,
            uiBase + 1,
            conBaseAry[uiBase].numCI,
            percSupF,
            maskedBasesUI,
            conBaseAry[uiBase].totalBasesKeptI,
            conBaseAry[uiBase].totalBasesI
         );
      } /*If: I had enough support to print an C*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-05:
      *   - Print out the snp/match entry for G
      \**************************************************/

      percSupF = (float) conBaseAry[uiBase].numGI;
      percSupF /= (float) nonMaskBaseUI;

      if(
           conBaseAry[uiBase].numGI
         < settings->printMinDepthI
      ) ;

      else if(percSupF >= settings->printMinSupSnpF)
      { /*If: I had enough support to print an G*/
         fprintf(
            outFILE,
            "%s\t%i\tbase\tG\t%i\t%f\t%u\t%i\t%u\n",
            refIdStr,
            uiBase + 1,
            conBaseAry[uiBase].numGI,
            percSupF,
            maskedBasesUI,
            conBaseAry[uiBase].totalBasesKeptI,
            conBaseAry[uiBase].totalBasesI
         );
      } /*If: I had enough support to print an G*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-06:
      *   - Print out the deletion entry
      \**************************************************/

      if(  conBaseAry[uiBase].numDelI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF = (float) conBaseAry[uiBase].numDelI;

         percSupF /=
            (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupDelF)
            fprintf(
               outFILE,
               "%s\t%i\tdel\tdel\t%i\t%f\t%u\t%i\t%u\n",
               refIdStr,
               uiBase + 1,
               conBaseAry[uiBase].numDelI,
               percSupF,
               maskedBasesUI,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/
   } /*Loop: Print out the consensus*/

   if(outFILE != stdout) fclose(outFILE);

   return 0;
} /*pConBasAry*/

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
