/*########################################################
# Name: tbCon
#  - Holds functions to make a quick consensus, more like
#    ivar. This is a bit memory intensive,
#    (40 + # insertions * (17) + bases in insertions)
#    bytes 
# Libraries
#   - "../generaLib/samEntryStruct.h"     (No .c file)
#   o "../generaLib/base10StrToNum.h"     (No .c file)
#   o "../generaLib/dataTypeShortHand.h"  (No .c file)
#   o "../generaLib/ulCpStr.h"            (No .c file)
#   o "../generaLib/numToStr.h"           (No .c file)
# C Standard Libraries:
#  o <stdlib.h>
#  o <stdio.h>
#  o <limits.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and defines
'   o st-01: insBase
'     - Structure to hold an list of insetions for a
'       single position
'   o st-02: conBase
'     - Structure to hold an single base in a sequence
'   o fun-01: initInsBase
'     - Initializes an initBase structure
'   o fun-02: makeInsBase
'     - Makes an initializes an initBase structure on the
'   o fun-03: freeInsBase
'     - Frees an heap allocated insBase structure
'   o fun-04: blankConBase
'     - Sets all non-pointer values in a conBase structure
'       to 0
'   o fun-05: initConBase
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
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and defines
\-------------------------------------------------------*/

#ifndef TB_CONSENSUS_H
#define TB_CONSENSUS_H

#include "../generalLib/samEntryStruct.h"

#define defMaskBase 'N'
#define defMinBaseQ 7
#define defMinInsQ 7


/*Settings for consensus buildiing*/
#define defMinLen 200     /*min length for consensus*/
#define defMinDepth 20    /*Support to keep a position*/
#define defMinPerSnp 0.5f  /*% support to keep an snp*/
#define defMinPerIns 0.8f  /*% support to keep an ins*/
#define defMinPerDel 0.8f  /*% support to keep an del*/

/*Settings for printing*/
#define defMinPrintDepth 20 
#define defMinPrintPerSnp 0.01f
#define defMinPrintPerIns 0.1f
#define defMinPrintPerDel 0.1f

#define defRefLen 5000000 /*~ Length of TB genome*/
char *defRefNameStr = "Reference";

#define def_tbCon_Memory_Err 64

/*-------------------------------------------------------\
| ST-01: insBase
|   - Structure to hold an list of insetions for a single
|     position
\-------------------------------------------------------*/
typedef struct insBase{
   char *insStr;/*C-string with insertion*/
   int lenInsI; /*Length of insertion*/
   int numHitsI;/*Number reads supporting this insertion*/

   struct insBase *nextIns; /*Next insertion*/
}insBase;

/*-------------------------------------------------------\
| ST-02: conBase
|   - Structure to hold an single base in a sequence
\-------------------------------------------------------*/
typedef struct conBase{
   int numAI;     /*Number of reads supporting an a*/
   int numTI;     /*Number of reads supporting an t*/
   int numCI;     /*Number of reads supporting an c*/
   int numGI;     /*Number of reads supporting an g*/
   int numDelI;   /*Number reads supporting an deletion*/

   int numInsI;   /*Number reads supporting insertions*/
   struct insBase *insList; /*List of insertions*/

   int totalBasesI; /*Reads supporting position*/
   int totalBasesKeptI; /*Reads above min Q-score*/
}conBase;

/*-------------------------------------------------------\
| ST-03: tbConSet
|   - Structure to hold settings for tbCon
\-------------------------------------------------------*/
typedef struct tbConSet{
   int minQI;   /*Min q-score to keep a base*/
   int minInsQI;/*Min Q-score to keep ins*/

   /*Min percent/depth/length to keep an match/mutation*/
   int minLenI;      /*Min fragment length*/
   int minDepthI;    /*Min depth to keep base*/
   float minPercSnpF;/*Min % support to keep a snp/match*/
   float minPercDelF;/*Min % support to keep a insertion*/
   float minPercInsF;/*Min % support to keep an deletion*/

   /*To mask low quality/support cosnesus bases with*/
   char maskC;/*Character to mask with*/

   /*Min percent/depth to print an snp/match,ins,or del*/
   int printMinDepthI;
   float printMinSupSnpF;
   float printMinSupInsF;
   float printMinSupDelF;
}tbConSet;

/*-------------------------------------------------------\
| Fun-01: initInsBase
|   - Initializes an initBase structure
| Input:
|   - insBaseSTPtr:
|     o A Pointer to an insBase structure to initialize
| Output:
|   - Modifies:
|     o All values in insBase to be 0
\-------------------------------------------------------*/
#define initInsBase(insBaseSTPtr){\
   (insBaseSTPtr)->insStr = 0;\
   (insBaseSTPtr)->lenInsI = 0;\
   (insBaseSTPtr)->numHitsI = 0;\
   (insBaseSTPtr)->nextIns = 0;\
} /*initInsBase*/

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
#define makeInsBase()({\
   struct insBase *retMacST =\
      malloc(sizeof(struct insBase));\
   \
   if(retMacST) initInsBase(retMacST);\
   \
   retMacST;\
}) /*makeInsBase*/

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
#define freeInsBase(insBaseSTPtr)({\
  struct insBase *nextIns = (insBaseSTPtr)->nextIns;\
  \
  if((insBaseSTPtr)->insStr)\
     free((insBaseSTPtr)->insStr);\
  \
  free((insBaseSTPtr));\
  (insBaseSTPtr) = 0;\
  nextIns;\
}) /*freeInsBase*/

/*-------------------------------------------------------\
| Fun-04: blankConBase
|   - Sets all non-pointer values in a conBase structure
|     to 0
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure to blank
| Output:
|   - Modifies:
|     o All non-pointer values in conBaseSTPtr to be 0
\-------------------------------------------------------*/
#define blankConBase(conBaseSTPtr){\
   (conBaseSTPtr)->numAI = 0;\
   (conBaseSTPtr)->numTI = 0;\
   (conBaseSTPtr)->numGI = 0;\
   (conBaseSTPtr)->numCI = 0;\
   (conBaseSTPtr)->numDelI = 0;\
   \
   (conBaseSTPtr)->numInsI = 0;\
   \
   (conBaseSTPtr)->totalBasesI = 0;\
   (conBaseSTPtr)->totalBasesKeptI = 0;\
} /*blankConBase*/

/*-------------------------------------------------------\
| Fun-05: initConBase
|   - Sets all values (including pointers) in a conBase
|     structure to 0
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure to initialize
| Output:
|   - Modifies:
|     o All values in conBaseSTPtr to be 0
\-------------------------------------------------------*/
#define initConBase(conBaseSTPtr){\
   blankConBase((conBaseSTPtr));\
   (conBaseSTPtr)->insList = 0;\
} /*initConBase*/

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
#define makeConBase()({\
   struct conBase *retMacST =\
      malloc(sizeof(struct conBase));\
   \
   if(retMacST) initConBase(retMacST);\
   \
   retMacST;\
}) /*makeInsBase*/

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
#define freeConBaseStack(conBaseSTPtr){\
  while((conBaseSTPtr)->insList)\
  { /*Loop: Free each insertion for the position*/\
     (conBaseSTPtr)->insList =\
         freeInsBase((conBaseSTPtr)->insList);\
  } /*Loop: Free each insertion for the position*/\
  \
  blankConBase((conBaseSTPtr);\
} /*freeConBaseStack*/

/*-------------------------------------------------------\
| Fun-08: freeConBase
|   - Frees a conBase structure
| Input:
|   - conBaseSTPtr:
|     o A Pointer to an conBase structure to free
| Output:
|   - Frees:
|     o Frees conBaseST
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
#define freeConBase(conBaseSTPtr){\
  freeConBaseStacvk((conBaseSTPtr));\
  free((conBaseSTPtr));\
  (conBaseSTPtr) = 0;\
} /*freeConBase*/

/*-------------------------------------------------------\
| Fun-09: freeConBaseAry
|   - Frees an array of conBase structures (assumes heap)
| Input:
|   - conBaseSTPtr:
|     o Pointer to the start of an conBase structure array
|       to free
|   - lenAryI:
|     o Number of conBase structurs in the array
| Output:
|   - Frees:
|     o The array in conBaseSTPtr
|   - Sets:
|     o conBaseST to 0
\-------------------------------------------------------*/
#define freeConBaseAry(conBaseSTPtr, lenAryI){\
  int iIndexMac = 0;\
  \
  for(iIndexMac = 0; iIndexMac < (lenAryI); ++iIndexMac)\
      freeConBaseStack(&(conBaseSTPtr)[iIndexMac]);\
  \
  free((conBaseSTPtr));\
  (conBaseSTPtr) = 0;\
} /*freeConBase*/

/*-------------------------------------------------------\
| Fun-10: initTbConSet
|   - initialize/set tbConSet settings to defaults
| Input:
|   - tmpConSetSTPtr:
|     o Pointer to the tbConSet structure with settings
|       for tbCon to set to defaults
| Output:
|   - Modifies:
|     o tbConSetSTPtr to default settings
\-------------------------------------------------------*/
#define initTbConSet(tbConSetSTPtr){\
   (tbConSetSTPtr)->minQI = defMinBaseQ;\
   (tbConSetSTPtr)->minInsQI = defMinInsQ;\
   \
   /*Min percent/depth/length to keep an match/mutation*/\
   (tbConSetSTPtr)->minLenI = defMinLen;\
   (tbConSetSTPtr)->minDepthI = defMinDepth;\
   (tbConSetSTPtr)->minPercSnpF = defMinPerSnp;\
   (tbConSetSTPtr)->minPercDelF = defMinPerDel;\
   (tbConSetSTPtr)->minPercInsF = defMinPerIns;\
   \
   /*To mask low quality/support cosnesus bases with*/\
   (tbConSetSTPtr)->maskC = defMaskBase;\
   \
   (tbConSetSTPtr)->printMinDepthI = defMinPrintDepth;\
   (tbConSetSTPtr)->printMinSupSnpF = defMinPrintPerSnp;\
   (tbConSetSTPtr)->printMinSupInsF = defMinPrintPerIns;\
   (tbConSetSTPtr)->printMinSupDelF = defMinPrintPerDel;\
} /*initTbConSet*/

/*-------------------------------------------------------\
| Fun-11: addReadToConBaseArray
|   - Adds the read into a conBase array
| Input:
|   - samST:
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
|     o def_tbCon_Memory_Err for memory errors
\-------------------------------------------------------*/
static char addReadToConBaseArray(
   struct samEntry *samST,    /*read to add to consensus*/
   struct conBase *conBaseAry[], /*Consensus array*/
   uint *lenRefUI,               /*Length of reference*/
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-11 Sec-02:
   ^   - Make sure I have enough bases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*First time*/
   if(!(*conBaseAry))
   { /*If: First round*/
      if(*lenRefUI < samST->refEndUI)
         *lenRefUI = samST->refEndUI + 100;

      *conBaseAry =
         malloc(*lenRefUI * sizeof(struct conBase));

      if(!(*conBaseAry)) return def_tbCon_Memory_Err;

      /*Initialize all the strutures*/
      for(uiBase = 0; uiBase < *lenRefUI; ++uiBase)
          initConBase(conBaseAry[uiBase]);
   } /*If: First round*/

   /*Check if I am adding more bases*/
   else if(*lenRefUI < samST->refEndUI)
   { /*If: I need to add more bases*/
      uiBase = *lenRefUI;

      /*Make sure I am not doing small reallocs*/
      if(*lenRefUI >= samST->refEndUI - 100)
         *lenRefUI = samST->refEndUI + 100;

      else *lenRefUI = samST->refEndUI;

      baseST =
         realloc(
           *conBaseAry,
           *lenRefUI * sizeof(struct conBase)
         ); /*Add more memory for the bases*/

      if(!baseST) return def_tbCon_Memory_Err;

      *conBaseAry = baseST; 

      /*Initialize the new strutures*/
      for(uiBase = uiBase; uiBase < *lenRefUI; ++uiBase)
          &initConBase(&conBaseAry[uiBase]);
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
   uiRef = 0;

   for(uiCig = 0; uiCig < samST->lenCigUI; ++uiCig)
   { /*Loop: Add bases to the consensus*/

      /*`& ~32` converts to uppercase, but does not
      ` change '=' or '\t'
      */
      switch(samST->cigTypeStr[uiCig] & ~32)
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
               ++(conBaseAry[uiRef]->totalBasesI);

               if(  samST->qStr[uiBase] - defQAdjust
                  < settings->minQI
               ){ /*If: the base is beneath the min Q*/
                  ++uiBase;
                  ++uiRef;
                  continue; /*Base beneath min Q-score*/
               } /*If: the base is beneath the min Q*/

               ++(conBaseAry[uiRef]->totalBasesKeptI);

               switch(samST->seqStr[uiBase] & ~32)
               { /*Switch: Check the base type*/
                  case 'A':
                     ++(conBaseAry[uiRef]->numAI);
                     break;

                  case 'T':
                     ++(conBaseAry[uiRef]->numTI);
                     break;

                  case 'C':
                     ++(conBaseAry[uiRef]->numCI);
                     break;

                  case 'G':
                     ++(conBaseAry[uiRef]->numGI);
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
               ++(conBaseAry[uiRef]->totalBasesI);
               ++(conBaseAry[uiRef]->totalBasesKeptI);
               ++(conBaseAry[uiRef]->numDelI);
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
            endCigUI = uiRef + samST->cigValAryI[uiCig];
            insStr = malloc((lenInsI +1) * sizeof(char));

            if(!insStr) return def_tbCon_Memory_Err;

            /*Get the total Q-score for the insertion*/
            while(uiBase < endCigUI)
            { /*Loop: Copy the insertion*/

              if(  samST->seqStr[uiBase] - defQAdjust
                 < settings->minInsQI
              ){ /*If: This insertion is low quality*/
                 ++uiBase;
                 continue;
              } /*If: This insertion is low quality*/

              insStr[iIns] = samST->seqStr[iIns];

              ++uiBase;
              ++iIns;
            } /*Loop: Copy the insertion*/
 
            lenInsI = iIns;
            insStr[lenInsI] == '\0';

            insST = conBaseAry[uiRef]->insList;

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
 
            if(!insST)
            { /*If: I did not find a match*/
               lastInsST->nextIns = makeInsBase();

               if(!(lastInsST->nextIns))
               { /*If: I had a memory error*/
                  free(insStr);
                  return def_tbCon_Memory_Err;
               } /*If: I had a memory error*/
 
               lastInsST->nextIns->insStr = insStr;
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
|       but for amplicons it will be an array
|     o 0 for memory erors
\-------------------------------------------------------*/
static struct samEntry * collapseConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   uint lenConAryUI,               /*Length of consensus*/
   int *lenSamAryI,         /*Holds output array length*/
   char *refIdStr,          /*Name of reference seq*/
   struct tbConSet *settings, /*settings for collapsing*/
   char *errC               /*Error reports*/
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
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiRef = 0;     /*Ref base on*/
   uint uiEndRef = 0;  /*Last base in amplicon/fragment*/
   uint lenFragUI = 0; /*Length of fragment/amplicon*/
   uint uiBase = 0;    /*Position on fragment (base on)*/

   uint uiCig = 0;     /*Cigar entry on*/

   /*General support*/
   float percSupSnpF = 0;
   float percSupInsF = 0;
   float percSupDelF = 0;

   /*Support for a particler snp/match*/
   float percSupAF = 0;
   float percSupTF = 0;
   float percSupCF = 0;
   float percSupGF = 0;

   /*Find the most supported insertion*/
   int keptReadsI = 0;
   char *tmpStr = 0; /*For resizing the buffers*/
   struct insBase *insIterST = 0;
   struct insBase *bestInsST = 0;

   int iFrag;          /*Fragment I am working on*/
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
         ++uiRef
      ) if(uiRef >= lenConAryUI) break;

      /*Find the length*/
      lenFragUI = uiRef - uiEndRef;

      if(lenFragUI < settings->minLenI)
      { /*If: This is beneath the min length to keep*/
         uiRef = uiEndRef;
         continue;
      } /*If: This is beneath the min length to keep*/

      ++(*lenSamAryI);
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-04:
   ^   - Collapse each fragment into a samEntry struct
   ^   o fun-12 sec-01 sub-01:
   ^     - Get length & check if fragment meets min length
   ^   o fun-12 sec-01 sub-01:
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-12 Sec-01 Sub-02:
   *   - Get length & check if fragment meets min length
   \*****************************************************/

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

      /*Find the length of this fragment*/
      for(
         uiEndRef = uiRef;
         conBaseAry[uiEndRef].totalBasesKeptI
            >= settings->minDepthI;
         ++uiRef
      ) if(uiRef >= lenConAryUI) break;

      /*Find the length*/
      lenFragUI = uiRef - uiEndRef;

      if(lenFragUI < settings->minLenI)
      { /*If: This is beneath the min length to keep*/
         uiRef = uiEndRef;
         continue;
      } /*If: This is beneath the min length to keep*/

      /**************************************************\
      * Fun-12 Sec-04 Sub-03:
      *   - Set up the samEntry meta data
      \**************************************************/

      blankSamEntry(&retSamST[iFrag]);

      retSamST[iFrag].extraStr = 0;

      retSamST[iFrag].seqStr = 0;
      retSamST[iFrag].qStr = 0;


      retSamST[iFrag].cigTypeStr = 0;
      retSamST[iFrag].cigValAryI = 0;

      /*Set up the query id (start-end)*/
      retSamST[iFrag].lenQryIdUC =
         numToStr(retSamST[iFrag].qryIdStr, uiRef);

      retSamST[iFrag].qryIdStr[retSamST[iFrag].lenQryIdUC] =
         '-';

      ++retSamST[iFrag].lenQryIdUC;

      retSamST[iFrag].lenQryIdUC += (uchar)
         numToStr(
            &(retSamST[iFrag].qryIdStr[
               retSamST[iFrag].lenQryIdUC
            ]),
            uiEndRef
         ); /*Copy the ending position*/

      /*Copy the reference id*/
      retSamST[iFrag].lenRefIdUC =
         ulCpStrDelim(
            retSamST[iFrag].refIdStr,
            refIdStr,
            defNullDelim
         ); /*Copy the reference name*/

      retSamST[iFrag].rNextStr[0] = '*';
      retSamST[iFrag].rNextStr[1] = '\0';
      retSamST[iFrag].lenRNextUC = 1;

      /*The flag is already set up*/
      /*Not sure what to put for mapping quality, so using
      ` 0 (default)
      */

      /*Get the reference lengths*/
      retSamST[iFrag].refStartUI = uiRef;
      retSamST[iFrag].alnReadLenUI = lenFragUI;
      retSamST[iFrag].refEndUI = uiEndRef;

      /**************************************************\
      * Fun-12 Sec-04 Sub-04:
      *   - Allocate memory for cigar and sequence buffers
      \**************************************************/

      /*Make the sequence buffer*/
      retSamST[iFrag].seqStr =
         malloc((lenFragUI << 1) * (sizeof(char)));

      retSamST[iFrag].lenSeqBuffUI = lenFragUI << 1;

      if(!retSamST[iFrag].seqStr)
      { /*If: I had a memroy error*/
         --iFrag;
         goto memoryError;
      } /*If: I had a memroy error*/

      /*Make the q-score buffer*/
      retSamST[iFrag].qStr = malloc(2 * sizeof(char));

      if(!retSamST[iFrag].qStr)
      { /*If: I had a memroy error*/
         free(retSamST[iFrag].seqStr);
         --iFrag;
         goto memoryError;
      } /*If: I had a memroy error*/

      retSamST[iFrag].qStr[0] = '*';
      retSamST[iFrag].qStr[1] = '\0';
      retSamST[iFrag].lenQBuffUI = 2;

      /*Make the cigar types buffer*/
      retSamST[iFrag].cigTypeStr =
         malloc((lenFragUI << 5) * sizeof(char));
      retSamST[iFrag].lenCigBuffUI = lenFragUI << 4;
      
      if(!retSamST[iFrag].cigTypeStr)
      { /*If: I had a memroy error*/
         free(retSamST[iFrag].seqStr);
         free(retSamST[iFrag].qStr);
         --iFrag;
         goto memoryError;
      } /*If: I had a memroy error*/

      /*Make the cigar values buffer*/
      retSamST[iFrag].cigValAryI =
         malloc((lenFragUI << 3) * sizeof(char));
      
      if(!retSamST[iFrag].cigValAryI)
      { /*If: I had a memroy error*/
         free(retSamST[iFrag].seqStr);
         free(retSamST[iFrag].qStr);
         free(retSamST[iFrag].cigTypeStr);
         --iFrag;
         goto memoryError;
      } /*If: I had a memroy error*/

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

      while(uiRef < uiEndRef)
      { /*Loop: Collapse the fragment*/
         percSupInsF =
             (float) conBaseAry[uiRef].numInsI
            / (float) conBaseAry[uiRef].totalBasesKeptI;

         /*The insertion is inbetween bases, which makes
         ` it hard to gauge. So, I am taking the worst
         ` case
         */
         if(uiRef > 0)
         { /*If: this is not the first reference base*/
            percSupDelF =
                (float) conBaseAry[uiRef].numInsI
               / conBaseAry[uiRef - 1].totalBasesKeptI;

            if(percSupInsF > percSupDelF)
            { /*If: the previous base had less support*/
               percSupInsF = percSupDelF;
               keptReadsI =
                  conBaseAry[uiRef - 1].totalBasesKeptI;
            } /*If: the previous base had less support*/

            else
               keptReadsI =
                  conBaseAry[uiRef].totalBasesKeptI;
         } /*If: this is not the first reference base*/

         percSupDelF =
              (float) conBaseAry[uiRef].numDelI
            / (float) conBaseAry[uiRef].totalBasesKeptI;

         percSupSnpF = 1 - percSupDelF;

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

         if(percSupInsF >= settings->minPercInsF)
         { /*If: An insertions is supported here*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'I')
            { /*If: This is a new cigar entry*/
               ++uiCig;
               retSamST[iFrag].cigTypeStr[uiCig] = 'I';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            keptReadsI;
            bestInsST = conBaseAry[uiRef].insList;
            insIterST = bestInsST;

            while(insIterST)
            { /*Loop: Find the most supported insertion*/
               if(bestInsST->numHitsI<insIterST->numHitsI)
                  bestInsST = insIterST;

               insIterST = insIterST->nextIns;
            } /*Loop: Find the most supported insertion*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun-12 Sec-05 Sub-02 Cat-02:
            +   - Check if keeping inse / copy insertion
            \+++++++++++++++++++++++++++++++++++++++++++*/

            percSupInsF =
                 (float) bestInsST->numHitsI
               / (float) keptReadsI;

            if(percSupInsF < settings->minPercInsF)
               goto ignoreIns;

            if(
                 retSamST[iFrag].lenSeqBuffUI
               < uiBase + bestInsST->lenInsI
            ){ /*If: I need a larger buffer*/
               retSamST[iFrag].lenSeqBuffUI <<= 1;

               tmpStr =
                   realloc(
                      retSamST[iFrag].seqStr,
                        retSamST[iFrag].lenSeqBuffUI
                      * sizeof(char)
                  );

               if(!tmpStr) goto memoryError;

               retSamST[iFrag].seqStr = tmpStr;
            } /*If: I need a larger buffer*/

            ulCpStr(
               &(retSamST[iFrag].seqStr[uiBase]),
               bestInsST->insStr,
               bestInsST->lenInsI
            );

            uiBase += bestInsST->lenInsI;

            retSamST[iFrag].cigValAryI[uiCig] +=
               bestInsST->lenInsI;

             ignoreIns:; /*No ins has enough support*/
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

         if(percSupSnpF >= settings->minPercSnpF)
         { /*If: snps were the best choice*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'M')
            { /*If: This is a new cigar entry*/
               ++uiCig;
               retSamST[iFrag].cigTypeStr[uiCig] = 'M';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

             percSupAF =
                (float) conBaseAry[uiRef].numAI
              / (float) conBaseAry[uiRef].totalBasesKeptI;

             percSupTF =
                (float) conBaseAry[uiRef].numTI
              / (float) conBaseAry[uiRef].totalBasesKeptI;

             percSupCF =
                (float) conBaseAry[uiRef].numCI
              / (float) conBaseAry[uiRef].totalBasesKeptI;

             percSupGF =
                (float) conBaseAry[uiRef].numCI
              / (float) conBaseAry[uiRef].totalBasesKeptI;

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun-12 Sec-05 Sub-03 Cat-02:
            +   - Find the base with the most support and
            +     see if it had enough support to keep
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(   percSupAF > percSupTF
               && percSupAF > percSupCF
               && percSupAF > percSupGF
            ) { /*If: A has the most support*/
               if(percSupAF >= settings->minPercSnpF)
                  retSamST[iFrag].seqStr[uiBase] = 'A';

               else
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;
            } /*If: A has the most support*/

            else if(
                  percSupTF > percSupCF
               && percSupTF > percSupGF
            ) { /*Else If: T has the most support*/
               if(percSupTF >= settings->minPercSnpF)
                  retSamST[iFrag].seqStr[uiBase] = 'T';

               else
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;
            } /*Else If: T has the most support*/

            else if(percSupCF > percSupGF)
            { /*Else If: C has the most support*/
               if(percSupCF >= settings->minPercSnpF)
                  retSamST[iFrag].seqStr[uiBase] = 'C';

               else
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;
            } /*Else If: C has the most support*/

            else
            { /*Else If: G has the most support*/
               if(percSupCF >= settings->minPercSnpF)
                  retSamST[iFrag].seqStr[uiBase] = 'G';

               else
                  retSamST[iFrag].seqStr[uiBase] =
                     settings->maskC;
            } /*Else If: G has the most support*/

            ++uiBase;
            ++(retSamST[iFrag].cigValAryI[uiCig]);
         } /*If: snps were the best choice*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-04:
         *   - Check if this was a deletion instead of snp
         \***********************************************/

         /*Do nothing if deletion was selected*/
         else if(percSupDelF >= settings->minPercDelF)
         { /*Else if: there was a deletion*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'D')
            { /*If: This is a new cigar entry*/
               ++uiCig;
               retSamST[iFrag].cigTypeStr[uiCig] = 'D';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            ++(retSamST[iFrag].cigValAryI[uiCig]);
         } /*Else if: there was a deletion*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-05:
         *   - Handle Not enough support cases; likely
         *     mixed infections
         \***********************************************/

         else
         { /*Else: I have no support, assume mask snp*/
            if(retSamST[iFrag].cigTypeStr[uiCig] != 'M')
            { /*If: This is a new cigar entry*/
               ++uiCig;
               retSamST[iFrag].cigTypeStr[uiCig] = 'M';
               retSamST[iFrag].cigValAryI[uiCig] = 0;
            } /*If: This is a new cigar entry*/

            retSamST[iFrag].seqStr[uiBase] =
               settings->maskC;

            ++uiBase;
            ++(retSamST[iFrag].cigValAryI[uiCig]);
         } /*Else: I have no support, assume mask snp*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-06:
         *   - Check if I need to resize sequence buffer
         \***********************************************/

         if(retSamST[iFrag].lenSeqBuffUI <= uiBase + 10)
         { /*If: I need a larger buffer*/
            retSamST[iFrag].lenSeqBuffUI <<= 1;

            tmpStr =
                realloc(
                  retSamST[iFrag].seqStr,
                  retSamST[iFrag].lenSeqBuffUI
                    * sizeof(char)
               );

            if(!tmpStr) goto memoryError;

            retSamST[iFrag].seqStr = tmpStr;
         } /*If: I need a larger buffer*/

         /***********************************************\
         * Fun-12 Sec-05 Sub-07:
         *   - Check if I need to resize the cigar buffer
         \***********************************************/

         if(retSamST[iFrag].lenCigBuffUI <= uiCig + 10)
         { /*If: I need a larger buffer*/
            retSamST[iFrag].lenCigBuffUI <<= 1;

            tmpStr =
                realloc(
                  retSamST[iFrag].cigTypeStr,
                  retSamST[iFrag].lenCigBuffUI
                    * sizeof(char)
               );

            if(!tmpStr) goto memoryError;

            retSamST[iFrag].cigTypeStr = tmpStr;

            tmpStr = (char *)
                realloc(
                  retSamST[iFrag].cigValAryI,
                  retSamST[iFrag].lenCigBuffUI
                     * sizeof(int)
               );

            if(!tmpStr) goto memoryError;

            retSamST[iFrag].cigValAryI = (int *) tmpStr;
         } /*If: I need a larger buffer*/

         ++uiRef;
      } /*Loop: Collapse the fragment*/

      retSamST[iFrag].readLenUI = uiBase;
      retSamST[iFrag].lenCigUI = uiCig;

      ++iFrag;
   } /*Loop: Find the number of fragments*/

   goto cleanUp;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-12 Sec-0?:
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
   ^ Fun-12 Sec-0?:
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
|     o File to print everything to
| Output:
|   - Prints:
|     o Entries in conBaseAry to outFILE
|       length
\-------------------------------------------------------*/
static void pConBaseArray(
   struct conBase conBaseAry[], /*Consensus array*/
   uint lenConAryUI,         /*Length of conBaseAry*/
   char *refIdStr,           /*Name of reference seq*/
   struct tbConSet *settings,/*Settings for printing*/
   FILE *outFILE             /*File to print to*/
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
   float percSupF = 0;
   uint worstInsSupUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-13 Sec-02:
   ^   - Print out the header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Print the header*/
   fprintf(
      outFILE,
      "refId\tposition\ttype\tsequence\treadsSupporting"
   );

   fprintf(
     outFILE,
     "\tkeptBasesAtPosition\ttotalMappedBases"
   );

   fprintf(outFILE, "\tpercSuport\n");

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

      worstInsSupUI =
        noBranchMax(
           conBaseAry[uiBase].totalBasesKeptI,
           conBaseAry[uiBase - 1].totalBasesKeptI
        );

      while(insST)
      { /*Loop: Check insertions*/
         percSupF =
          (float) insST->numHitsI/(float) worstInsSupUI;

         if(percSupF < settings->printMinSupInsF)
         { /*If: I am not printing this ins*/
            insST = insST->nextIns;
             continue;
         } /*If: I am not printing this ins*/

         fprintf(
            outFILE,
            "%s\t%u\tins\t%s\t%u\t%f\t%u\t%u\n",
            refIdStr,
            uiBase - 1,
            insST->insStr,
            insST->numHitsI,
            percSupF,
            conBaseAry[uiBase].totalBasesKeptI,
            "NA"
         );
      } /*Loop: Check insertions*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-02:
      *   - Print out the snp/match entry for A
      \**************************************************/

      firstBaseNoIns:;

      if(  conBaseAry[uiBase].numAI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF =
              (float) conBaseAry[uiBase].numAI
            / (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupSnpF)
            fprintf(
               outFILE,
               "%s\t%u\tsnp\tA\t%u\t%f\t%u\t%u\n",
               refIdStr,
               uiBase,
               conBaseAry[uiBase].numAI,
               percSupF,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-03:
      *   - Print out the snp/match entry for T
      \**************************************************/

      if(  conBaseAry[uiBase].numTI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF =
              (float) conBaseAry[uiBase].numTI
            / (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupSnpF)
            fprintf(
               outFILE,
               "%s\t%u\tsnp\tT\t%u\t%f\t%u\t%u\n",
               refIdStr,
               uiBase,
               conBaseAry[uiBase].numTI,
               percSupF,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-04:
      *   - Print out the snp/match entry for C
      \**************************************************/

      if(  conBaseAry[uiBase].numCI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF =
              (float) conBaseAry[uiBase].numCI
            / (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupSnpF)
            fprintf(
               outFILE,
               "%s\t%u\tsnp\tC\t%u\t%f\t%u\t%u\n",
               refIdStr,
               uiBase,
               conBaseAry[uiBase].numCI,
               percSupF,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-05:
      *   - Print out the snp/match entry for G
      \**************************************************/

      if(  conBaseAry[uiBase].numGI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF =
              (float) conBaseAry[uiBase].numGI
            / (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupSnpF)
            fprintf(
               outFILE,
               "%s\t%u\tsnp\tG\t%u\t%f\t%u\t%u\n",
               refIdStr,
               uiBase,
               conBaseAry[uiBase].numGI,
               percSupF,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/

      /**************************************************\
      * Fun-13 Sec-03 Sub-06:
      *   - Print out the deletion entry
      \**************************************************/

      if(  conBaseAry[uiBase].numDelI
         > settings->printMinDepthI
      ){ /*If: The a base has enough depth*/
         percSupF =
              (float) conBaseAry[uiBase].numDelI
            / (float) conBaseAry[uiBase].totalBasesKeptI;

         if(percSupF >= settings->printMinSupDelF)
            fprintf(
               outFILE,
               "%s\t%u\tdel\t-\t%u\t%f\t%u\t%u\n",
               refIdStr,
               uiBase,
               conBaseAry[uiBase].numDelI,
               percSupF,
               conBaseAry[uiBase].totalBasesKeptI,
               conBaseAry[uiBase].totalBasesI
            );
      } /*If: The a base has enough depth*/
   } /*Loop: Print out the consensus*/

   return;
} /*pConBasAry*/

#endif
