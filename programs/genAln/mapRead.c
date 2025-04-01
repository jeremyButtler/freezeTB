/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mapRead SOF: Start Of File
'   - maps a read to a reference
'   * .h st01: chains_mapRead
'     - holds kmer chains build from kmerChain_mapRead
'      o fun01: blank_chains_mapRead
'        - blanks a chains_mapRead struct
'      o fun02: init_chains_mapRead
'        - initialize a chains_mapRead struct
'      o fun03: freeStack_chains_mapRead
'        - frees all variables in a chains_mapRead struct
'      o fun04: freeHeap_chains_mapRead
'        - frees a chains_mapRead struct
'      o fun05: setup_chains_mapRead
'        - sets up memory for a chains_mapRead struct
'      o fun06: mk_chains_mapRead
'        - makes a heap allocated chains_mapRead struct
'      o fun07: realloc_chains_mapRead
'        - reallocates memroy in a chains_mapRead struct
'   * .h st02: set_mapRead
'     - settings for a aln_mapRead
'     o fun08: blank_set_mapRead
'       - blank a set_mapRead struct
'     o fun09: init_set_mapRead
'       - initializes a set_mapRead struct
'     o fun10: freeStack_set_mapRead
'       - frees variables in a set_mapRead struct
'     o fun11: freeHeap_set_mapRead
'       - frees a set_mapRead struct
'     o fun12: setup_set_mapRead
'       - allocates memory for structs in a set_mapRead
'   * .h st03: ref_mapRead
'     - holds reference variables used in read mapping
'     o fun13: blank_ref_mapRead
'       - blanks (sets to defaults) a ref_mapRead struct
'     o fun14: init_ref_mapRead
'       - initialize a ref_mapRead structure
'     o fun15: freeStack_ref_mapRead
'       - frees variables in a ref_mapRead struct
'     o fun16: freeHeap_ref_mapRead
'       - frees a ref_mapRead struct
'     o fun17: addRef_ref_mapRead
'       - adds a reference sequence to ref_map read struct
'     o fun18: mk_ref_mapRead
'       - makes and initializes a ref_mapRead structure
'     o fun19: getRef_ref_mapRead
'       - gets and stores next reference in ref_mapRead
'   * st04: aln_mapRead
'     - holds non-reference variables used in read mapping
'     o fun20: blank_aln_mapRead
'       - blanks an aln_mapRead struct
'     o fun21: init_aln_mapRead
'       - initializes an aln_mapRead struct
'     o fun22: freeStack_aln_mapRead
'       - frees variables in an aln_mapRead struct
'     o fun23: freeHeap_aln_mapRead
'       - frees an aln_mapRead struct
'     o fun24: setup_aln_mapRead
'       - setups (memory allocate) an aln_mapRead struct
'     o fun25: mk_aln_mapRead
'       - makes and returns heap allocated aln_mapRead
'   o fun26: sortKmerIndex_mapRead
'     - sorts kmer index array by a kmer array
'   o .c fun27: findKmer_mapRead
'     - finds first signed int in a range
'   o .c fun28: findChain_chains_mapRead
'     - finds kmer in range from kmerChain_mapRead return
'   o fun29: kmerChain_mapRead
'     - merge kmers into chains (longer kmers)
'   o .c fun30: numQryChains_mapRead
'     - gets the number of chains assigned to a single
'       query kmer
'   o fun31: mergeChains_mapRead
'     - merges kmer chains from fun29 (kmerChain) into
'       the chain the maximizes base count
'   o fun32: mergeToSam_mapRead
'     - convert output from mergeChains_map read to sam
'       file entry (mapq is set to 0)
'   o fun33: scoreSubAln_mapRead
'     - scores the aligment in samEntry and finds best
'       sub-aligment
'   o fun34: aln_mapRead
'     - maps read to reference
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "mapRead.h"

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"

#include "../genBio/kmerFun.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"

#include "alnSet.h"
#include "dirMatrix.h"
#include "needle.h"
#include "water.h"

/*.h files only*/
#include "defsMapRead.h" /*default settings*/
#include "../genLib/genMath.h" /*.h max/min macros only*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_chains_mapRead
|   - blanks a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead struct to blank
| Output:
|   - Modifies:
|     o refKmerAryUI and qryKmerAryUI in chainsSTPtr to
|       have all values set to -1
|     o chainLenArySI and scoreArySI in chainsSTPtr to
|       have all values set to 0
|     o indexArySI in chainsSTPtr to be filled with -1's
|     o lenChainSI and lenIndexSL in chainsSTPtr to be 0
\-------------------------------------------------------*/
void
blank_chains_mapRead(
   struct chains_mapRead *chainsSTPtr /*to blank*/
){
   signed long slElm = 0;

   if(! chainsSTPtr)
      return; /*nothing to blank*/

   if(chainsSTPtr->refArySI)
   { /*If: need to blank chain arrays*/

      slElm = 0;

      while(slElm < chainsSTPtr->lenChainSI)
      { /*Loop: blank chain arrays*/
         chainsSTPtr->refArySI[slElm] = -1;
         chainsSTPtr->qryArySI[slElm] = -1;
         chainsSTPtr->chainArySI[slElm] = 0;
         chainsSTPtr->scoreArySI[slElm] = 0;
         chainsSTPtr->nextArySI[slElm++] = 0;
      } /*Loop: blank chain arrays*/

   } /*If: need to blank chain arrays*/

   if(chainsSTPtr->indexArySI)
   { /*If: need to blank chain arrays*/

      slElm = 0;

      while(slElm < chainsSTPtr->lenIndexSI)
         chainsSTPtr->indexArySI[slElm++] = -1;

   } /*If: need to blank chain arrays*/

   chainsSTPtr->lenIndexSI = 0;
   chainsSTPtr->lenChainSI = 0;
   chainsSTPtr->lastIndexSI = 0;
} /*blank_chains_mapRead*/

/*-------------------------------------------------------\
| Fun02: init_chains_mapRead
|   - initialize a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead struct to initialize
| Output:
|   - Modifies:
|     o refKmerAryUI, qryKmerAryUI, chainLenArySI,
|       scoreArySI, and indexArySI to be 0 (null)
|     o lenChainSI, sizeChainSI, lenIndexSL, and
|       sizeIndexSI in chainsSTPtr to be 0
\-------------------------------------------------------*/
void
init_chains_mapRead(
   struct chains_mapRead *chainsSTPtr /*to initialize*/
){
   if(! chainsSTPtr)
      return; /*nothing to initialize*/

   chainsSTPtr->refArySI = 0;
   chainsSTPtr->qryArySI = 0;
   chainsSTPtr->chainArySI = 0;
   chainsSTPtr->scoreArySI = 0;
   chainsSTPtr->nextArySI = 0;
   chainsSTPtr->indexArySI = 0;

   chainsSTPtr->sizeChainSI = 0;
   chainsSTPtr->sizeIndexSI = 0;

   blank_chains_mapRead(chainsSTPtr);
} /*init_chains_mapReads*/

/*-------------------------------------------------------\
| Fun03: freeStack_chains_mapRead
|   - frees all variables in a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead with variables to free
| Output:
|   - Modifies:
|     o frees refKmerAryUI, qryKmerAryUI, chainLenArySI,
|       scoreArySI, and indexArySI and sets to 0 (null)
|     o lenChainSI, sizeChainSI, lenIndexSL, and
|       sizeIndexSI in chainsSTPtr to be 0
\-------------------------------------------------------*/
void
freeStack_chains_mapRead(
   struct chains_mapRead *chainsSTPtr /*to free vars*/
){
   if(! chainsSTPtr)
      return;

   if(chainsSTPtr->refArySI)
      free(chainsSTPtr->refArySI);

   if(chainsSTPtr->qryArySI)
      free(chainsSTPtr->qryArySI);

   if(chainsSTPtr->chainArySI)
      free(chainsSTPtr->chainArySI);

   if(chainsSTPtr->scoreArySI)
      free(chainsSTPtr->scoreArySI);

   if(chainsSTPtr->nextArySI)
      free(chainsSTPtr->nextArySI);

   if(chainsSTPtr->indexArySI)
      free(chainsSTPtr->indexArySI);

   init_chains_mapRead(chainsSTPtr);
} /*freeStack_chains_mapRead*/

/*-------------------------------------------------------\
| Fun04: freeHeap_chains_mapRead
|   - frees a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead to free
| Output:
|   - Modifies:
|     o frees chainsSTPtr (you must set to null)
\-------------------------------------------------------*/
void
freeHeap_chains_mapRead(
   struct chains_mapRead *chainsSTPtr /*to free*/
){
   if(! chainsSTPtr)
      return;

   freeStack_chains_mapRead(chainsSTPtr);
   free(chainsSTPtr);
} /*freeHeap_chains_mapRead*/

/*-------------------------------------------------------\
| Fun05: setup_chains_mapRead
|   - sets up memory for a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead to setup memory
|   - lenChainsSL:
|     o length of chains arrays
|   - lenRefUI:
|     o length of reference
| Output:
|   - Modifies:
|     o allocates lenChainsSL items of memory to
|       refCkmerArySI, qryKmerArySI, chainLenArySI,
|       and scoreArySI in chainsSTPtr
|     o allocates lenRefUI memory to indexArySI in
|       chainsSTPtr
|     o sizeChainSI in chainsSTPtr to be lenChainSI 
|     o sizeIndexSI in chainsSTPtr to be lenIndexSL 
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
setup_chains_mapRead(
   struct chains_mapRead *chainsSTPtr, /*to setup*/
   signed long lenChainsSL,         /*chain array length*/
   unsigned int lenRefUI            /*reference length*/
){
   signed long slElm = 0;

   if(! chainsSTPtr)
      return 0;

   /*making sure no old memory hanging around*/
   freeStack_chains_mapRead(chainsSTPtr);

   chainsSTPtr->refArySI =
      malloc(lenChainsSL * sizeof(signed int));
   if(! chainsSTPtr->refArySI)
      goto memErr_fun05;

   chainsSTPtr->qryArySI =
      malloc(lenChainsSL * sizeof(signed int));
   if(! chainsSTPtr->qryArySI)
      goto memErr_fun05;

   chainsSTPtr->chainArySI =
      calloc(
         lenChainsSL,
         sizeof(signed int)
      );
   if(! chainsSTPtr->chainArySI)
      goto memErr_fun05;

   chainsSTPtr->scoreArySI =
      calloc(
         lenChainsSL,
         sizeof(signed int)
      );
   if(! chainsSTPtr->scoreArySI)
      goto memErr_fun05;

   chainsSTPtr->nextArySI =
      calloc(
         lenChainsSL,
         sizeof(signed int)
      );
   if(! chainsSTPtr->nextArySI)
      goto memErr_fun05;


   chainsSTPtr->indexArySI =
      calloc(
         lenRefUI,
         sizeof(signed int)
      );
   if(! chainsSTPtr->indexArySI)
      goto memErr_fun05;


   chainsSTPtr->sizeChainSI = lenChainsSL;
   chainsSTPtr->sizeIndexSI = (signed int) lenRefUI;


   slElm = 0;
   while(slElm < lenChainsSL)
   { /*Loop: blank kmer starting coords arrays*/
      chainsSTPtr->refArySI[slElm] = -1;
      chainsSTPtr->qryArySI[slElm++] = -1;
   } /*Loop: blank kmer starting coords arrays*/

   slElm = 0;
   while(slElm < lenRefUI)
      chainsSTPtr->indexArySI[slElm++] = -1;


   return 0;

   memErr_fun05:;
      return def_memErr_mapRead;
} /*setup_chains_mapRead*/

/*-------------------------------------------------------\
| Fun06: mk_chains_mapRead
|   - makes a heap allocated chains_mapRead struct
| Input:
|   - lenChainsSL:
|     o length of chains arrays
|   - lenRefUI:
|     o length of reference
| Output:
|   - Returns:
|     o pointer to initialized and setup chains_mapRead
|       struct
|     o 0 for memory errors
\-------------------------------------------------------*/
struct chains_mapRead *
mk_chains_mapRead(
   signed long lenChainsSL,         /*chain array length*/
   unsigned int lenRefUI            /*reference length*/
){
   struct chains_mapRead *retSTPtr = 0;

   retSTPtr = malloc(sizeof(struct chains_mapRead));
   if(! retSTPtr)
      goto memErr_fun06;

   init_chains_mapRead(retSTPtr);

   if(
      setup_chains_mapRead(
         retSTPtr,
         lenChainsSL,
         (signed long) lenRefUI
      )
   ) goto memErr_fun06;


   goto ret_fun06;

   memErr_fun06:;
      if(retSTPtr)
         freeHeap_chains_mapRead(retSTPtr);
      retSTPtr = 0;
      goto ret_fun06;
   ret_fun06:;
      return retSTPtr;
} /*mk_chains_mapRead*/

/*-------------------------------------------------------\
| Fun07: realloc_chains_mapRead
|   - reallocates memroy in a chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead to rellocate memory
|   - lenChainsSL:
|     o new length of chains arrays (0 for do not resize)
|   - lenRefUI:
|     o new length of reference (0 for do not resize)
| Output:
|   - Modifies:
|     o refKmerArySI, qryKmerArySI, chainLenArySI,
|       scoreArySI in chainsSTPtr to be reallocated to
|       lenChainSI if lenChainSI >chainsSTPtr->sizeChainSI
|     o indexArySI chainsSTPtr to be reallocated to
|       lenRefUI if lenRefUI  > chainsSTPtr->sizeIndexSI
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
realloc_chains_mapRead(
   struct chains_mapRead *chainsSTPtr, /*reallocate*/
   signed long lenChainsSL,         /*chain array length*/
   unsigned int lenRefUI            /*reference length*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - reallocates memroy in a chains_mapRead struct
   '   o fun07 sec01:
   '     - variable declaration + input check
   '   o fun07 sec02:
   '     - chain arrays reallocs + initialize
   '   o fun07 sec03:
   '     - reference index array rellocs
   '   o fun07 sec04:
   '     - return error or no error
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declaration + input check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *scPtr = 0;
   signed long slElm = 0;

   if(! chainsSTPtr)
      return 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - chain arrays reallocs + initialize
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(lenChainsSL > chainsSTPtr->sizeChainSI)
   { /*If: reallocating chains array memory*/
      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->refArySI,
            lenChainsSL * sizeof(signed int)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->refArySI = (signed int *) scPtr;

      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->qryArySI,
            lenChainsSL * sizeof(signed int)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->qryArySI = (signed int *) scPtr;

      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->chainArySI,
            lenChainsSL * sizeof(signed int)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->chainArySI = (signed int *) scPtr;

      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->scoreArySI,
            lenChainsSL * sizeof(signed int)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->scoreArySI = (signed int *) scPtr;


      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->nextArySI,
            lenChainsSL * sizeof(signed int)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->nextArySI = (signed int *) scPtr;


      slElm = chainsSTPtr->sizeChainSI;
      while(slElm < lenChainsSL)
      { /*Loop: blank kmer starting coords arrays*/
         chainsSTPtr->refArySI[slElm] = -1;
         chainsSTPtr->qryArySI[slElm] = -1;
         chainsSTPtr->chainArySI[slElm] = 0;
         chainsSTPtr->scoreArySI[slElm] = 0;
         chainsSTPtr->nextArySI[slElm++] = 0;
      } /*Loop: blank kmer starting coords arrays*/

      chainsSTPtr->sizeChainSI = lenChainsSL;
   } /*If: reallocating chains array memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - reference index array rellocs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(lenRefUI > (unsigned int) chainsSTPtr->sizeIndexSI)
   { /*If: reallocating chains array memory*/
      scPtr =
         (signed char *)
         realloc(
            chainsSTPtr->indexArySI,
            lenRefUI * sizeof(signed long)
         );
      if(! scPtr)
         goto memErr_fun07_sec04;
      chainsSTPtr->indexArySI = (signed int *) scPtr;

      slElm = chainsSTPtr->sizeIndexSI;
      while(slElm < lenRefUI)
         chainsSTPtr->indexArySI[slElm++] = 0;

      chainsSTPtr->sizeIndexSI = lenRefUI;
   } /*If: reallocating chains array memory*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - return error or no error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun07_sec04:;
      return def_memErr_mapRead;
} /*realloc_chains_mapRead*/

/*-------------------------------------------------------\
| Fun08: blank_set_mapRead
|   - blank a set_mapRead struct
| Input:
|   - setSTPtr:
|     o pointer to set_mapRead struct to blank
| Output:
|   - Modifies:
|     o sets values in setSTPtr to defaults
\-------------------------------------------------------*/
void
blank_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   setSTPtr->subBl = def_subAln_defsMapRead;
   setSTPtr->minScoreF = def_minScore_defsMapRead;
   setSTPtr->minPercLenF = def_minPercLen_defsMapRead;

   setSTPtr->lenArySI[0] = def_minLen_defsMapRead;
   setSTPtr->lenArySI[1] = def_secLen_defsMapRead;
   setSTPtr->lenArySI[2] = def_thirdLen_defsMapRead;
   setSTPtr->lenArySI[3] = def_fourthLen_defsMapRead;
   setSTPtr->lenArySI[4] = def_fifthLen_defsMapRead;
   setSTPtr->lenArySI[5] = def_sixthLen_defsMapRead;
   setSTPtr->lenArySI[6] = def_finalLen_defsMapRead;

   setSTPtr->kmerAryUC[0] = def_minKmer_defsMapRead;
   setSTPtr->kmerAryUC[1] = def_secKmer_defsMapRead;
   setSTPtr->kmerAryUC[2] = def_thirdKmer_defsMapRead;
   setSTPtr->kmerAryUC[3] = def_fourthKmer_defsMapRead;
   setSTPtr->kmerAryUC[4] = def_fifthKmer_defsMapRead;
   setSTPtr->kmerAryUC[5] = def_sixthKmer_defsMapRead;
   setSTPtr->kmerAryUC[6] = def_finalKmer_defsMapRead;

   setSTPtr->chainLenArySI[0] = def_minChain_defsMapRead;
   setSTPtr->chainLenArySI[1] = def_secChain_defsMapRead;
   setSTPtr->chainLenArySI[2] =
        def_thirdChain_defsMapRead;
   setSTPtr->chainLenArySI[3] =
       def_fourthChain_defsMapRead;
   setSTPtr->chainLenArySI[4] =
      def_fifthChain_defsMapRead;
   setSTPtr->chainLenArySI[5] =
      def_sixthChain_defsMapRead;
   setSTPtr->chainLenArySI[6] =
      def_finalChain_defsMapRead;

   if(setSTPtr->alnSetST)
      init_alnSet(setSTPtr->alnSetST);
      /*alnSet has no blank function*/
} /*blank_set_mapRead*/

/*-------------------------------------------------------\
| Fun09: init_set_mapRead
|   - initializes a set_mapRead struct
| Input:
|   - setSTPtr:
|     o pointer to set_mapRead struct to initializes
| Output:
|   - Modifies:
|     o sets variables to defaults and pointers to null
\-------------------------------------------------------*/
void
init_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   setSTPtr->lenKmersUC = 7;
   setSTPtr->alnSetST = 0;

   blank_set_mapRead(setSTPtr);
} /*init_set_mapRead*/

/*-------------------------------------------------------\
| Fun10: freeStack_set_mapRead
|   - frees variables in a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all pointers and intitializes; defaults/null
\-------------------------------------------------------*/
void
freeStack_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   if(setSTPtr->alnSetST)
      freeHeap_alnSet(setSTPtr->alnSetST);

   init_set_mapRead(setSTPtr);
} /*freeStack_set_mapRead*/

/*-------------------------------------------------------\
| Fun11: freeHeap_set_mapRead
|   - frees a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer to free
| Output:
|   - Modifies:
|     o frees setSTPtr (you must set to null/0)
\-------------------------------------------------------*/
void
freeHeap_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   freeStack_set_mapRead(setSTPtr);
   free(setSTPtr);
} /*freeHeap_set_mapRead*/

/*-------------------------------------------------------\
| Fun12: setup_set_mapRead
|   - allocates memory for structs in a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer to free
| Output:
|   - Modifies:
|     o alnSetST to be on heap
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
setup_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return 0;

   if(! setSTPtr->alnSetST)
   { /*If: do not have alignment settings*/

      setSTPtr->alnSetST = malloc(sizeof(struct alnSet));
      if(! setSTPtr->alnSetST)
         goto memErr_fun12;
      init_alnSet(setSTPtr->alnSetST);

   } /*If: do not have alignment settings*/

   return 0;

   memErr_fun12:;
      return def_memErr_mapRead;
} /*setup_set_mapRead*/

/*-------------------------------------------------------\
| Fun13: blank_ref_mapRead
|   - blanks (sets to defaults) a ref_mapRead structure
|   - use addRef_ref_mapRead to change references
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer to blank
| Output:
|   - Modifies:
|     o sets all values in refSTPtr to defaults or for
|       arrays null
\-------------------------------------------------------*/
void
blank_ref_mapRead(
   struct ref_mapRead *refSTPtr
){
   signed int iterSI = 0;

   if(! refSTPtr)
      return;

   /*these are not good defaults, but then they also will
   `  be set when the user adds the reference sequence
   */
   refSTPtr->lenKmerUC = def_finalKmer_defsMapRead;
   refSTPtr->minChainLenSI = def_finalChain_defsMapRead;

   /*need to blank all kmers in arrays*/
   for(
      iterSI = 0;
      iterSI < refSTPtr->lenSI;
      ++iterSI
   ){ /*Loop: blank kmer arrays*/
      if(refSTPtr->kmerArySI)
        refSTPtr->kmerArySI[0] = def_endKmers_kmerFun;
      if(refSTPtr->indexArySI)
        refSTPtr->indexArySI[0] = (signed int) iterSI;
   }  /*Loop: blank kmer arrays*/

   refSTPtr->lenSI = 0;

   if(refSTPtr->seqSTPtr)
      blank_seqST(refSTPtr->seqSTPtr);
} /*blank_ref_mapRead*/

/*-------------------------------------------------------\
| Fun14: init_ref_mapRead
|   - initialize a ref_mapRead structure
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer to initialize
| Output:
|   - Modifies:
|     o sets pointers to 0/null & other values to default
\-------------------------------------------------------*/
void
init_ref_mapRead(
   struct ref_mapRead *refSTPtr
){
   if(! refSTPtr)
     return;

   refSTPtr->seqSTPtr = 0;
   refSTPtr->kmerArySI = 0;
   refSTPtr->indexArySI = 0;
   refSTPtr->sizeKmerSI = 0;
   refSTPtr->sizeIndexSI = 0;

   blank_ref_mapRead(refSTPtr);
} /*init_ref_mapRead*/

/*-------------------------------------------------------\
| Fun15: freeStack_ref_mapRead
|   - frees variables in a ref_mapRead struct
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all heap allocated variabes in refSTPtr
|     o all variabes in refSTPtr to be 0/null (free) or
|       defaults
\-------------------------------------------------------*/
void
freeStack_ref_mapRead(
   struct ref_mapRead *refSTPtr
){
   if(! refSTPtr)
      return;

    if(refSTPtr->seqSTPtr)
       freeHeap_seqST(refSTPtr->seqSTPtr);
    refSTPtr->seqSTPtr = 0;

    if(refSTPtr->kmerArySI)
       free(refSTPtr->kmerArySI);
    if(refSTPtr->indexArySI)
       free(refSTPtr->indexArySI);

    init_ref_mapRead(refSTPtr);
} /*freeStack_ref_mapRead*/

/*-------------------------------------------------------\
| Fun16: freeHeap_ref_mapRead
|   - frees a ref_mapRead struct
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer to free
| Output:
|   - Modifies:
|     o frees refSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_ref_mapRead(
   struct ref_mapRead *refSTPtr
){
   if(! refSTPtr)
      return;
   freeStack_ref_mapRead(refSTPtr);
   free(refSTPtr);
} /*freeHeap_ref_mapRead*/

/*-------------------------------------------------------\
| Fun17: addRef_ref_mapRead
|   - adds a reference sequence to a ref_map read struct
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer to add reference to
|   - seqSTPtr:
|     o seqST struct pointer with reference sequence
|     o must be heap allocated; otherwise messes up
|       freeStack_ref_mapRead
|     o on success, you should set this to null, but do
|       not free
|     o on failure, free seqSTPtr; only assigned on succes
|   - setSTPtr:
|     o set_mapRead struct with settings for alignment
| Output:
|   - Modifies:
|     o seqSTPtr in refSTPtr to point to seqSTPtr
|     o on success sequence in seqSTPtr is converted
|       to alnSet lookup index's
|     o kmerArySI to have refernce kmers
|     o sortKmerArySI to have refernce kmers and be sorted
|     o indexArySI to have index's to convert
|       sortKmerArySI index to kmerArySI index
|     o lenKmerUC and minChainLenSI to bet set to their
|       correct values in setSTPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
|     o def_noStruct_mapRead if any input is 0/null
\-------------------------------------------------------*/
signed char
addRef_ref_mapRead(
   struct ref_mapRead *refSTPtr, /*add reference to*/
   struct seqST *seqSTPtr,       /*reference to add*/
   struct set_mapRead *setSTPtr  /*has settings to add*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - adds a reference sequence to ref_map read struct
   '   o fun17 sec01:
   '     - variable declaration and make sure have input
   '   o fun17 sec02:
   '     - find reference alignment settings
   '   o fun17 sec03:
   '     - build kmer arrays
   '   o fun17 sec04:
   '     - allocate memory for sorted and index arrays
   '   o fun17 sec05:
   '     - copy kmers into sorted array and sort kmers
   '   o fun17 sec06:
   '     - set seqsTPtr pointer and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declaration and make sure have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siKmer = 0;

   if(! refSTPtr)
      goto noStruct_fun17_sec06;
   else if(! seqSTPtr)
      goto noStruct_fun17_sec06;
   else if(! seqSTPtr->seqStr)
      goto noStruct_fun17_sec06;
   else if(! seqSTPtr->seqStr[0] == '\0')
      goto noStruct_fun17_sec06;
   else if(! setSTPtr)
      goto noStruct_fun17_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - find reference alignment settings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      siKmer = 0;
      siKmer < setSTPtr->lenKmersUC;
      ++siKmer
   ){ /*Loop: find kmer and chain size*/
      if(
             refSTPtr->seqSTPtr->lenSeqUL
          >= (unsigned long) setSTPtr->lenArySI[siKmer]
      ) break; /*found kmer settings*/
   }  /*Loop: find kmer and chain size*/

   refSTPtr->lenKmerUC = setSTPtr->lenArySI[siKmer];
   refSTPtr->minChainLenSI =
      setSTPtr->chainLenArySI[siKmer];

   /*needed indexed bases for kmer conversion*/
   seqToIndex_alnSet(seqSTPtr->seqStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - build kmer arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refSTPtr->lenSI =
      seqToKmer_kmerFun(
         seqSTPtr->seqStr,
         (signed int) seqSTPtr->lenSeqUL,
         &refSTPtr->kmerArySI,
         &refSTPtr->sizeKmerSI,
         refSTPtr->lenKmerUC
      );

   if(! refSTPtr->lenSI)
      goto memErr_fun17_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - allocate memory for sorted and index arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(refSTPtr->sizeIndexSI < refSTPtr->lenSI)
   { /*If: need more memory*/

      if(refSTPtr->indexArySI)
         free(refSTPtr->indexArySI);
      refSTPtr->indexArySI = 0;

      refSTPtr->indexArySI =
         malloc(
            (refSTPtr->lenSI + 8) * sizeof(signed int)
         );

      if(! refSTPtr->indexArySI)
         goto memErr_fun17_sec06;

      refSTPtr->sizeIndexSI = refSTPtr->lenSI;
   } /*If: need more memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec05:
   ^   - copy kmers into sorted array and sort kmers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortKmerIndex_mapRead(
      refSTPtr->kmerArySI,
      refSTPtr->indexArySI,
      refSTPtr->lenSI
   ); /*makes indexArySI into creates an index array that is
      `  sorted by kmer (uses kmerArySI as a guide [does
      `  nothing to it])
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec06:
   ^   - set seqSTPtr pointer and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refSTPtr->seqSTPtr = seqSTPtr;
   return 0;

   memErr_fun17_sec06:;
     siKmer = def_memErr_mapRead;
     goto errRet_fun17_sec06;

   noStruct_fun17_sec06:;
     siKmer = def_noStruct_mapRead;
     goto errRet_fun17_sec06;

   errRet_fun17_sec06:;
      indexToSeq_alnSet(seqSTPtr->seqStr);
      return (signed char) siKmer;
} /*addRef_ref_mapRead*/

/*-------------------------------------------------------\
| Fun18: mk_ref_mapRead
|   - makes and initializes a ref_mapRead structure
| Input:
|   - seqSTPtr:
|     o seqST struct pointer with reference sequence
|     o must be heap allocated; otherwise messes up
|       freeStack_ref_mapRead
|     o on success, you should set this to null, but do
|       not free
|     o on failure, free seqSTPtr; only assigned on succes
|   - setSTPtr:
|     o set_mapRead struct with settings for alignment
| Output:
|   - Returns:
|     o 0 for errors
|     o pointer to setup ref_mapRead structure
\-------------------------------------------------------*/
struct ref_mapRead *
mk_ref_mapRead(
   struct seqST *seqSTPtr,       /*reference to add*/
   struct set_mapRead *setSTPtr  /*has settings to add*/
){
   struct ref_mapRead *refSTPtr = 0;
  
   if(! seqSTPtr)
      goto err_fun18;
   else if(! seqSTPtr->seqStr)
      goto err_fun18;
   else if(! seqSTPtr->seqStr[0] == '\0')
      goto err_fun18;
   else if(! setSTPtr)
      goto err_fun18;

   refSTPtr = malloc( sizeof(struct ref_mapRead) );
   if(! refSTPtr)
      goto err_fun18;

   init_ref_mapRead(refSTPtr);

   if(
      addRef_ref_mapRead(
         refSTPtr,
         seqSTPtr,
         setSTPtr
      )
   ) goto err_fun18;

   goto ret_fun18;

   err_fun18:;
      if(refSTPtr)
         freeHeap_ref_mapRead(refSTPtr);
      refSTPtr = 0;
      goto ret_fun18;

   ret_fun18:;
      return refSTPtr;
} /*mk_ref_mapRead*/

/*-------------------------------------------------------\
| Fun19: getRef_ref_mapRead
|   - gets and stores next reference in ref_mapRead struct
| Input:
|   - refSTPtr:
|     o ref_mapRead struct pointer to store reference in
|   - setSTPtr:
|     o set_mapRead struct with settings for alignment
|   - fxBl:
|     o tells if file is fastq (1) or regular fasta (0)
|   - gzBl:
|     o tells if file is gziped (1) or regular fasta (0)
|     o TODO: add gzip support
|   - refFILE:
|     o FILE pointer to file with reference to store
| Output:
|   - Modifies:
|     o seqSTPtr in refSTPtr to have reference sequence
|     o kmerArySI in refSTPtr to have refernce kmers
|     o sortKmerArySI to have refernce kmers and be sorted
|     o indexArySI to have index's to convert
|       sortKmerArySI index to kmerArySI index
|     o lenKmerUC and minChainLenSI to bet set to their
|       correct values in setSTPtr
|     o refFILE to be on next entry in fastx file
|   - Returns:
|     o 0 for errors
|     o def_EOF_mapRead if hit end of file
|     o def_memErr_mapRead for memory errors
|     o def_noStruct_mapRead if setSTPtr or refSTPtr is 0
|     o def_badFile_mapRead for invalid fastx entries
|       * also for gz files (currently not supported)
\-------------------------------------------------------*/
signed char
getRef_ref_mapRead(
   struct ref_mapRead *refSTPtr,/*gets reference seq*/
   struct set_mapRead *setSTPtr,/*has settings to add*/
   signed char fxBl,            /*1: fastq; 0: fasta*/
   signed char gzBl,            /*1: gzipped; 0: not*/
   void *refFILE                /*fx file with reference*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - gets and stores next reference in ref_mapRead
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - checks and memory allocation
   '   o fun19 sec03:
   '     - get and add reference sequence
   '   o fun19 sec04:
   '     - setup reference in refSTPtr
   '   o fun19 sec05:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   signed char errSC = 0;
   signed char eofSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - checks and memory allocation
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   if(! refSTPtr)
      goto noStruct_fun19_sec05;
   else if(! setSTPtr)
      goto noStruct_fun19_sec05;
   else if(gzBl)
      goto badFile_fun19_sec05;
      /*currently not supported*/

   if(! refSTPtr->seqSTPtr)
   { /*If: need a sequence structure*/
      refSTPtr->seqSTPtr = malloc( sizeof(struct seqST) );

      if(! refSTPtr->seqSTPtr)
         goto memErr_fun19_sec05;
      init_seqST(refSTPtr->seqSTPtr);
   } /*If: need a sequence structure*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - get and add reference sequence
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*TODO add gzip support*/
   if(fxBl)
   { /*If: fastq file*/
      errSC = 
         getFqSeq_seqST(
            refFILE,
            refSTPtr->seqSTPtr
         );
   } /*If: fastq file*/

   else
   { /*Else: fasta file*/
      errSC = 
         getFaSeq_seqST(
            refFILE,
            refSTPtr->seqSTPtr
         );
   } /*Else: fasta file*/

   if(errSC)
   { /*If: had error*/
      if(errSC & def_fileErr_seqST)
         goto badFile_fun19_sec05;
      else if(errSC & def_memErr_seqST)
         goto memErr_fun19_sec05;
      else if(refSTPtr->seqSTPtr->seqStr[0] == '\0')
         goto eof_fun19_sec05;
         /*end of file*/
      else
         eofSC = 1; /*last sequence in file*/
   } /*If: had error*/
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - setup reference in refSTPtr
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   errSC =
      addRef_ref_mapRead(
         refSTPtr,
         refSTPtr->seqSTPtr,
         setSTPtr
      );

   if(errSC)
      goto memErr_fun19_sec05;
      /*already know structures exist, so must be memory
      `   error
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec05:
   ^   - return result
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   if(eofSC)
      errSC = def_EOF_mapRead;
   else
      errSC = 0;

   goto ret_fun19_sec05;

   memErr_fun19_sec05:;
      errSC = def_memErr_mapRead;
      goto ret_fun19_sec05;

   noStruct_fun19_sec05:;
      errSC = def_noStruct_mapRead;
      goto ret_fun19_sec05;

   badFile_fun19_sec05:;
      errSC = def_badFile_mapRead;
      goto ret_fun19_sec05;

   eof_fun19_sec05:;
      errSC = def_EOF_mapRead;
      goto ret_fun19_sec05;

   ret_fun19_sec05:;
      return errSC;
} /*getRef_ref_mapRead*/

/*-------------------------------------------------------\
| Fun20: blank_aln_mapRead
|   - blanks an aln_mapRead struct
| Input:
|   - alnSTPtr:
|     o aln_mapRead struct pointer to blank
| Output:
|   - Modifies:
|     o first element kmerArySI in alnSTPtr to be
|       def_endKmers_kmerFun 
|     o lenSI in alnSTPtr to be 0
|     o dirMatrix in alnSTPtr with blank_dirMatrix
|     o chainsSTPtr in alnSTPtr with blank_chains_mapRead
\-------------------------------------------------------*/
void
blank_aln_mapRead(
   struct aln_mapRead *alnSTPtr
){
   if(! alnSTPtr)
      return;

   if(alnSTPtr->kmerArySI)
      alnSTPtr->kmerArySI[0] = def_endKmers_kmerFun;
   alnSTPtr->lenSI = 0;

   if(alnSTPtr->matrixSTPtr)
      blank_dirMatrix(alnSTPtr->matrixSTPtr);

   if(alnSTPtr->chainsSTPtr)
      blank_chains_mapRead(alnSTPtr->chainsSTPtr);
} /*blank_aln_mapRead*/

/*-------------------------------------------------------\
| Fun21: init_aln_mapRead
|   - initializes an aln_mapRead struct
| Input:
|   - alnSTPtr:
|     o aln_mapRead struct pointer to initialize
| Output:
|   - Modifies:
|     o kmerArySI, matrixSTPtr, and chainsSTPtr in
|       alnSTPtr to be 0/null
|     o lenSI and sizeSI in alnSTPtr to be 0
\-------------------------------------------------------*/
void
init_aln_mapRead(
   struct aln_mapRead *alnSTPtr
){
   if(! alnSTPtr)
      return;

   alnSTPtr->kmerArySI = 0;
   alnSTPtr->matrixSTPtr = 0;
   alnSTPtr->chainsSTPtr = 0;

   alnSTPtr->sizeSI = 0;
   blank_aln_mapRead(alnSTPtr);
} /*init_aln_mapRead*/

/*-------------------------------------------------------\
| Fun22: freeStack_aln_mapRead
|   - frees variables in an aln_mapRead struct
| Input:
|   - alnSTPtr:
|     o aln_mapRead struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees kmerArySI, matrixSTPtr, and chainsSTPtr
|       in alnSTPtr
|     o sizeSI and lenSI in alnSTPtr to be 0
\-------------------------------------------------------*/
void
freeStack_aln_mapRead(
   struct aln_mapRead *alnSTPtr
){
   if(! alnSTPtr)
      return;

   if(alnSTPtr->kmerArySI)
      free(alnSTPtr->kmerArySI);
   if(alnSTPtr->matrixSTPtr)
      freeHeap_dirMatrix(alnSTPtr->matrixSTPtr);
   if(alnSTPtr->chainsSTPtr)
      freeHeap_chains_mapRead(alnSTPtr->chainsSTPtr);

   init_aln_mapRead(alnSTPtr);
} /*freeStack_aln_mapRead*/

/*-------------------------------------------------------\
| Fun23: freeHeap_aln_mapRead
|   - frees an aln_mapRead struct
| Input:
|   - alnSTPtr:
|     o aln_mapRead struct pointer to free
| Output:
|   - Modifies:
|     o frees alnSTPtr (you must set to null)
\-------------------------------------------------------*/
void
freeHeap_aln_mapRead(
   struct aln_mapRead *alnSTPtr
){
   if(! alnSTPtr)
      return;

   freeStack_aln_mapRead(alnSTPtr);
   free(alnSTPtr);
} /*freeHeap_aln_mapRead*/

/*-------------------------------------------------------\
| Fun24: setup_aln_mapRead
|   - setups (memory allocate) an aln_mapRead struct
| Input:
|   - alnSTPtr:
|     o aln_mapRead struct pointer to setup
| Output:
|   - Modifies:
|     o allocates memory for kmerArySI, matrixSTPtr, and
|       chainsSTPtr in alnSTPtr
|     o sizeSI in alnSTPtr to be size of memory
|     o lenSI in alnSTPtr to be 0
|   - Returns:
|     o 0 for no errors (or no structure input)
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
setup_aln_mapRead(
   struct aln_mapRead *alnSTPtr
){
   if(! alnSTPtr)
      return 0;

   freeStack_aln_mapRead(alnSTPtr);


   alnSTPtr->kmerArySI = malloc(256 * sizeof(signed int));
   if(! alnSTPtr->kmerArySI)
      goto memErr_fun21;
   alnSTPtr->sizeSI = 256;


   alnSTPtr->chainsSTPtr =
      mk_chains_mapRead(
         256, /*number chains, resize later if needed*/
         4096 /*will be resized later if needed*/
      ); /*calling this results in the blanking step
         `  being repeated twice, however,
         `   setup_aln_mapRead should only be called once
         */
   if(! alnSTPtr->chainsSTPtr)
      goto memErr_fun21;


   alnSTPtr->matrixSTPtr =
      malloc(sizeof(struct dirMatrix));
   if(! alnSTPtr->matrixSTPtr)
      goto memErr_fun21;


   blank_aln_mapRead(alnSTPtr);


   return 0;

   memErr_fun21:;
      return def_memErr_mapRead;
} /*setup_aln_mapRead*/

/*-------------------------------------------------------\
| Fun25: mk_aln_mapRead
|   - makes and returns heap allocated aln_mapRead struct
| Input:
| Output:
|   - Returns:
|     o pointer to new initialized and setup aln_mapRead
|       struct
|     o 0 for memory errors
\-------------------------------------------------------*/
struct aln_mapRead *
mk_aln_mapRead(
){
   struct aln_mapRead *retSTPtr = 0;

   retSTPtr = malloc(sizeof(struct aln_mapRead));
   if(! retSTPtr)
      goto memErr_fun25;

   init_aln_mapRead(retSTPtr);
   if( setup_aln_mapRead(retSTPtr) )
      goto memErr_fun25;

   goto ret_fun25;

   memErr_fun25:;
      if(retSTPtr)
         freeHeap_aln_mapRead(retSTPtr);
      retSTPtr = 0;
      goto ret_fun25;

   ret_fun25:;
      return retSTPtr;
} /*mk_aln_mapRead*/

/*-------------------------------------------------------\
| Fun26: sortKmerIndex_mapRead
|   - sorts kmer index array by a kmer array
| Input:
|   - refKmerArySI:
|     o signed int array of reference kmers to use for
|       sorting
|   - indexArySI:
|     o signed int array to have sorted kmer index's
|     o needs to be same length as lenSI
|   - lenSI:
|     o length of kmer array (index 1)
| Output:
|   - Modifies:
|     o indexArySI to have index's sorted by refKmerArySI
|       - also sets index's for indexArySI
\-------------------------------------------------------*/
void
sortKmerIndex_mapRead(
   signed int *kmerArySI, /*kmers to make sorted index's*/
   signed int *indexArySI,/*get kmerArySI sorted index's*/
   signed int lenSI       /*array length (index 1)*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun26 TOC:
   '   - sorts kmer index array by a kmer array
   '   o fun26 sec01:
   '     - variable declerations
   '   o fun26 sec02:
   '     - find the number of rounds to sort for
   '   o fun26 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec01:
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
   ^ Fun26 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   --lenSI; /*convert index 1 to index 0*/

   indexArySI[0] = 0;

   if(lenSI <= 0)
      return;

   /*set up index's*/
   for(
      subSI = 1;
      subSI < lenSI;
      ++subSI
   ) indexArySI[subSI] = (signed int) subSI;


   /*Recursion formsia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec03:
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

            if(
                 (unsigned int)
                    kmerArySI[ indexArySI[siElm] ]
               > (unsigned int)
                    kmerArySI[ indexArySI[nextSI] ]
            ){ /*If I need to swap an element*/
               indexArySI[siElm] ^= indexArySI[nextSI];
               indexArySI[nextSI] ^= indexArySI[siElm];
               indexArySI[siElm] ^= indexArySI[nextSI];

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(
                       (unsigned int)
                          kmerArySI[ indexArySI[onSI] ]
                     > (unsigned int)
                          kmerArySI[ indexArySI[lastSI] ]
                  ) break; /*Positioned the element*/
   
                  indexArySI[onSI] ^= indexArySI[lastSI];
                  indexArySI[lastSI] ^= indexArySI[onSI];
                  indexArySI[onSI] ^= indexArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortKmerIndex_mapRead*/

/*-------------------------------------------------------\
| Fun27: findKmer_mapRead
|   - finds first unsigned int in a range
| Input:
|   - indexArySI:
|     o signed int array of sorted index's to search
|     o make sure is sorted by sortKmerIndex_mapRead
|   - refKmerArySI:
|     o signed int array of reference kmers to search
|   - qrySI:
|     o query to find
|   - lenSI:
|     o length of array (index 1)
| Output:
|   - Returns:
|     o index of first item in array
|     o -1 if item not in array
\-------------------------------------------------------*/
signed long
findKmer_mapRead(
   signed int *indexArySI,   /*ref kmer index's*/
   signed int *refKmerArySI, /*reference kmers*/
   signed int qrySI,         /*query to search for*/
   signed int lenSI          /*array length (index 1)*/
){
   signed long midSL = 0;
   signed long rightSL = lenSI - 1;
   signed long leftSL = 0;

   if(lenSI <= 0)
      return -1;

   while(leftSL <= rightSL)
   { /*Loop: find query*/
      midSL = (leftSL + rightSL) >> 1;

      if(
          ( (unsigned int) qrySI )
        > ((unsigned int) refKmerArySI[indexArySI[midSL]])
      ) leftSL = midSL + 1;

      else if(
          ( (unsigned int) qrySI )
        < ((unsigned int) refKmerArySI[indexArySI[midSL]])
      ) rightSL = midSL - 1;

      else
      { /*Else: is match*/
         if(! midSL)
            return midSL; /*at start of array*/

         if(
               ( (unsigned int) qrySI )
            == (  (unsigned int)
                  refKmerArySI[ indexArySI[midSL - 1] ]
               )
         ) rightSL = midSL - 1; /*still more matches*/

         else
            return midSL;
      } /*Else: is match*/
   } /*Loop: find query*/

   return -1; /*no matches found*/
} /*findKmer_mapRead*/

/*--------------------------------------------------------\
| Fun28: findChain_chains_mapRead
|   - finds chain with input kmer coordiantes in a
|     chains_mapRead struct
| Input:
|   - chainsSTPtr:
|     o pointer to chains_mapRead struct to search
|   - refSI:
|     o reference starting position of kmer
|   - qrySI:
|     o query starting position of kmer
| Output:
|   - Returns:
|     o index of at least one kmer chain with both
|       refSI and qrySI kmer coordaintes
|     o -1 if did not find kmer
| Note:
|   - data must be sorted by reference start kmer position,
|     then by query start kmer positions
\--------------------------------------------------------*/
signed long
findChain_chains_mapRead(
   struct chains_mapRead *chainsSTPtr, /*has chains*/
   signed int refSI,        /*reference kmer start*/
   signed int qrySI         /*query kmer start*/
){
   signed long midSL = 0;
   signed long rightSL = chainsSTPtr->lenChainSI - 1;
   signed long leftSL = 0;
   signed long refEndSL = 0;
   signed long qryEndSL = 0;

   if(chainsSTPtr->lenChainSI <= 0)
      return -1;

   while(leftSL <= rightSL)
   { /*Loop: find kmers*/
      midSL = (leftSL + rightSL) >> 1;

      refEndSL = chainsSTPtr->refArySI[midSL];
      refEndSL += chainsSTPtr->chainArySI[midSL];

      qryEndSL = chainsSTPtr->qryArySI[midSL];
      qryEndSL += chainsSTPtr->chainArySI[midSL];

      if(refSI > refEndSL)
         leftSL = midSL + 1;
      else if(qrySI > qryEndSL)
         leftSL = midSL + 1;
      else if(refSI < chainsSTPtr->refArySI[midSL])
         rightSL = midSL - 1;
      else if(qrySI < chainsSTPtr->qryArySI[midSL])
         rightSL = midSL - 1;
      else
         return midSL;
         /*chain includes reference and query kmers*/
   } /*Loop: find kmers*/

   return -1; /*nothing found*/
} /*findChain_chains_mapRead*/

/*-------------------------------------------------------\
| Fun29: kmerChain_mapRead
|   - merge kmers into chains (longer kmers)
| Input:
|   - chainsSTPtr:
|     o pointer to chainsSTPtr struct to add kmer chains
|       to
|   - minChainLenSI:
|     o minimum number of kmers to keep a chain
|   - qryKmerArySI:
|     o signed int array with query kmers (by position)
|   - lenQrySI:
|     o number kmers in qryKmerArySI
|   - refKmerArySI:
|     o signed int array with reference kmers to chain
|   - refIndexArySI:
|     o signed int array with refKmerArySI index's sorted
|       by kmer
|     o create with fun26 sortKmerIndex_mapRead
|   - lenRefSI:
|     o number kmers in refKmerArySI
|   - refStartSI:
|     o first refrence base in window
| Output:
|   - Modifies:
|     o refKmerArySI, qryKmerArySI, and chainLenArySI in
|       chainsSTPtr to have chain starting coordinates and
|       length
|     o scoreArySI and nextArySI in chainsSTPtr to have
|       all detected kmer positions filled with 0's
|     o indexArySI in chainsSTPtr to have index or -1 for
|       each query kmer start position in qryKmerArySI
|     o lenChainSI and lenIndexSI to have length of
|       indexArySI (num ref kmers) and number of chains
|     o sizeChainSI and sizeIndexSI in chainsSTPtr if
|       needed to rellocate memory for chain/index arrays
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
|   - Note:
|     o sorting order of chains is query coordinate and
|       then reference coordinate
\-------------------------------------------------------*/
signed char
kmerChain_mapRead(
   struct chains_mapRead *chainsSTPtr,/*gets kmer chains*/
   signed int minChainLenSI,          /*min chain length*/
   signed int *qryKmerArySI,          /*query kmers*/
   signed int lenQrySI,               /*query length*/
   signed int *refKmerArySI,          /*reference kmers*/
   signed int *refIndexArySI,
     /*sorted refernce kmer index's (fun26)*/
   signed int lenRefSI,               /*number ref kmers*/
   signed int refStartSI              /*first ref base*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun29 TOC:
   '   - merge kmers to get longer kmers
   '   o fun29 sec01:
   '     - variable declarations
   '   o fun29 sec02:
   '     - initial memory allocation
   '   o fun29 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;          /*for returing errors*/

   signed long indexSL = 0;      /*index of kmer matches*/
   signed int qryIndexSI = 0;      /*qry kmer on*/
   signed int refIndexSI = 0;      /*table kmer on*/

   signed int lenChainSI = 0;      /*length of 1 chain*/
   signed int siKmer = 0;          /*reference kmer iter*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec02:
   ^   - initial memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   blank_chains_mapRead(chainsSTPtr);

   if(chainsSTPtr->sizeIndexSI < lenQrySI)
   { /*If: need to resize index*/
      errSC =
         realloc_chains_mapRead(
            chainsSTPtr,
            0, /*not resizing chains array*/
            lenQrySI
         );
      if(errSC)
         goto memErr_fun29_sec04;
   } /*If: need to resize index*/

   chainsSTPtr->lenIndexSI = lenQrySI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec03:
   ^   - find kmer chains
   ^   o fun29 sec03 sub01:
   ^     - find if have kmer chain(s) + start detect loop
   ^   o fun29 sec03 sub02:
   ^     - build all kmer chains for one ref kmer
   ^   o fun29 sec03 sub03:
   ^     - find next possible kmer match to ref kmer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun29 Sec03 Sub01:
   *   - find if have kmer chain(s) + start detect loop
   \*****************************************************/

   /* very hard to estimate O/theta (finding worst case
   `   is hard); this is an off the`cuff guess
   `  O[  q * log(r) qry-find * O[r * log(hits) * O[r|q
   `    + log(r) next-ref
   `   ]
   `   = O[q * r * log(2r + hits) * (r|q)]
   `      but r|q worst case is many duplicate kmers
   `      that can never meet the minimum chain
   `      length
   `   o no chain case, never extend chain beyond
   `     minimum chain length (mChain)
   `   = O[q * r * log(2r + hits) * mChain]
   `   o however, this assumes that for each q, we
   `     are iterating over the entireity of r, but
   `     this would require duplicate kmers, which
   `     would mean the minimum chain length,
   `     so there must be enough kmers that non-mathcing
   `     patterns form.
   `     * for 1 kmer only, it is dulpicate, everything
   `       is merged, so [r * log(hits) * mChain] becomes
   `       [1 * log(1) + mChain]
   `     * for 2 kmers, query is divided by 2, inorder to
   `       be worst case the reference must have a
   `       different kmer at every mChain interval. This
   `       is highly unlikey in biological data. The max
   `       length before duplication is (2 ^ mChain)
   `     * howver, in thuis case we have no hits, so
   `       log(2r + hits) becomes log(2q)
   `   = O[q * (r / 2) * log(2r + hits) * mChain]
   `   o the worst case is highly unlikely, also, it is
   `     very likely kmer diversity will be high, so this
   `     more likely scales closer, but not quite to
   `     kmer-space [r / min(length(r), 4 ^ kmer-length]
   `   = theta[q * kmer-space * log(2r + hits) * mChain]
   `   o for most cases the kmer space will be larger
   `     (7mer to 11mer; 16000 kmers to 4000000 kmers)
   `     than the read length, so only duplications
   `     will have an impact on the theta case, so
   `     log(2r + hits) might scale to log(r + hits)
   `   o the kmer scan step should reduce the chance
   `     of duplications, by limiting windows size
   `   o based on this it may often, very roughly scale to
   `   = theta[q * log(r + hits] * mChain]
   `     * probably my overally optimisitic guess
   */
   for(
      siKmer = 0;
      siKmer < lenQrySI;
      ++siKmer
   ){ /*Loop: find matching kmers*/

      chainsSTPtr->indexArySI[siKmer] = -1;

      if(qryKmerArySI[siKmer] < 0)
      { /*If: anonymous kmer or end of kmers*/
         if(qryKmerArySI[siKmer] != def_endKmers_kmerFun)
            continue; /*anonymous base*/
         else
            break; /*no more query kmers*/
      } /*If: anonymous kmer or end of kmers*/

      indexSL =
         findKmer_mapRead(
            refIndexArySI,
            refKmerArySI,
            (unsigned int) qryKmerArySI[siKmer],
            lenRefSI
         ); /*find first query kmer for ref*/
            /*O[log(r)] ref-find*/
            /*unsigned int forces so negative values
            `  to be at end
            */

      if(indexSL < 0)
         continue; /*no match*/

      /**************************************************\
      * Fun29 Sec03 Sub02:
      *   - build all kmer chains for one ref kmer
      \**************************************************/

      /*  O[r] ref-loop
      ` * O[log(hits)] ref-find
      ` * O[r | q] extend
      */
      while(
            qryKmerArySI[siKmer]
         == refKmerArySI[ refIndexArySI[indexSL] ]
      ){ /*Loop: make kmer chains*/

         qryIndexSI = siKmer;
         refIndexSI = refIndexArySI[indexSL];
         lenChainSI = 0;

         if(
            findChain_chains_mapRead(
               chainsSTPtr,
               refIndexSI + refStartSI,
               qryIndexSI
            ) >= 0
         ) goto nextMatch_fun29_sec03_sub03;
           /*already have this kmer chain*/
           /*O[log(hits)] hit-find*/


         while(
                qryKmerArySI[qryIndexSI++]
             == refKmerArySI[refIndexSI++]
         ) ++lenChainSI; /*find kmer chain length*/
           /*O[r | q] extend*/


         if(lenChainSI >= minChainLenSI)
         { /*If: keeping kmer chain*/

             if(
                   chainsSTPtr->lenChainSI
                >= chainsSTPtr->sizeChainSI
             ){ /*If: need more memory*/
                errSC =
                   realloc_chains_mapRead(
                      chainsSTPtr,
                      chainsSTPtr->lenChainSI << 1,
                      0 /*not rellocating index arrray*/
                   );
                if(errSC)
                   goto memErr_fun29_sec04;
             } /*If: need more memory*/

            if(chainsSTPtr->indexArySI[siKmer] < 0)
            { /*If: first chain for this query base*/
               chainsSTPtr->indexArySI[siKmer] =
                  chainsSTPtr->lenChainSI;

               chainsSTPtr->lastIndexSI = siKmer;
            } /*If: first chain for this query base*/

             /*reference kmer starting point*/
             chainsSTPtr->refArySI[
                chainsSTPtr->lenChainSI
             ] =  refIndexArySI[indexSL] + refStartSI;

             /*query starting point*/
             chainsSTPtr->qryArySI[
                chainsSTPtr->lenChainSI
             ] = siKmer;

             /*kmer chain length*/
             chainsSTPtr->chainArySI[
                chainsSTPtr->lenChainSI++
             ] = lenChainSI;
               
         } /*If: keeping kmer chain*/

         /***********************************************\
         * Fun29 Sec03 Sub03:
         *   - find next possible kmer match to ref kmer
         \***********************************************/

         nextMatch_fun29_sec03_sub03:;
            ++indexSL; /*get off last match*/
      } /*Loop: make kmer chains*/
   }  /*Loop: find matching kmers*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun29_sec04;

   memErr_fun29_sec04:;
      errSC = def_memErr_mapRead;
      goto ret_fun29_sec04;

   ret_fun29_sec04:
      return errSC;
} /*kmerChain_mapRead*/

/*-------------------------------------------------------\
| Fun30: numQryChains_mapRead
|   - gets the number of chains assigned to a single
|     query kmer
| Input:
|   - chainsSTPtr:
|     o pointer to a chains_mapRead struct with reference
|       chaines
|   - index:
|     o index of reference kmer (kmer number) to find
|       number of chains for
| Output:
|   - Returns:
|     o number of chains for reference kmer
\-------------------------------------------------------*/
#define numQryChains_mapRead(chainsSTPtr, index) ( ((chainsSTPtr)->indexArySI[(indexSL)] < 0) ? -1 : ( (index) < (chainsSTPtr)->lenIndexSI ) ? ( (chainsSTPtr)->indexArySI[(index) + 1] - (chainsSTPtr)->indexArySI[(index)] ) : ( (chainsSTPtr)->lenChainSI - (chainsSTPtr)->indexArySI[(index)] ) )
/*Logic:
`   - checks to see if reference kmer has any assigned
`     assigned chains
`   - turnary checks if have more index's (ref chains)
`     after the current index
`     (index < chainsSTPtr->lenIndexSI)
`   - if have more index's; length is found by
`     subtracing next reference starting index
`     (indexArySI[index + 1]) from the current index
`     (indexArySI[index])
`   - if at end; length is found by
`     subtracing the array length (chainsSTPtr->lenChainSI)
`     minus the references starting index
`     (chainsSTPtr->indexArySI[index])
*/

/*-------------------------------------------------------\
| Fun31: mergeChains_mapRead
|   - merges kmer chains from fun29 (kmerChain) into
|     the chain the maximizes base count
|   - not optimal solution, but also reduce complexity
|     by not counting for gaps
| Input:
|   - chainAryUI:
|     o unsigned int array with kmer chains to merge into
|       a "best" chain
| Output:
|   - Modifies:
|     o nextArySI in chains_mapRead to have best scoring
|       trail for each chain
|     o scoreArySI to have most bases possible in a chain
|   - Returns:
|     o index of best merged chain
|     o def_noChains_mapRead if no chains in array
\-------------------------------------------------------*/ 
signed int
mergeChains_mapRead(
   struct chains_mapRead *chainsSTPtr  /*chains to merge*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun31 TOC:
   '   - merges kmer chains from fun29 (kmerChain)
   '   o fun31 sec01:
   '     - variable declarations
   '   o fun31 Sec02:
   '     - memory allocation
   '   o fun31 sec03:
   '     - check few chain cases
   '   o fun31 sec04:
   '     - merge chains
   '   o fun31 sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun31 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siChain = 0;    /*main chain on*/
   signed int siSub = 0;      /*subchain on*/
   signed int lastKmerSI = 0; /*index of last ref kmer*/
   signed int lastIndexSI = 0;/*index of last refkmer*/
   signed int subIndexSI = 0; /*index of subchain ref*/

   signed int scoreSI = 0;    /*score of merge*/
   signed int refUniqSI = 0;  /*number uniq bases in ref*/
   signed int refEndSI = 0;   /*ref ending coordinate*/

   signed int qryUniqSI = 0; /*number unique query bases*/
   signed int qryEndSI = 0;  /*query ending coordinate*/

   signed int maxNtSI = 0;    /*most bases in chain*/
   signed int curMaxNtSI = 0; /*current best base count*/
   signed int maxIndexSI = 0; /*index of chain maximized*/
   signed int curMaxIndexSI = 0;

   signed int secMaxNtSI = 0; /*2nd most bases in chain*/
   signed int curSecMaxNtSI = 0; /*current rounds score*/
      /*used as a quick check to reduces chances of a full
      `  scan. The logic is if
      `     (nt in chain) + secMaxNtSI
      `   < maxNtSI + (nt in chain) - overlap
      `  then I can safely say that I can not maximize
      `  the score any more for overlap cases
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun31 Sec02:
   ^   - memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun31 Sec03:
   ^   - check few chain cases
   ^   o fun31 sec03 sub01:
   ^     - no chains and only one chain case
   ^   o fun31 sec03 sub02:
   ^     - only two chain case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun31 Sec03 Sub01:
   *   - no chains and only one chain case
   \*****************************************************/

   if(chainsSTPtr->lenChainSI <= 0)
      goto noChains_fun31_sec05;

   if(chainsSTPtr->lenChainSI == 1)
   { /*If: only one chain*/
      maxIndexSI = 0;

      chainsSTPtr->scoreArySI[0] =
         chainsSTPtr->chainArySI[0];

      chainsSTPtr->nextArySI[0] = 0; /*nothing after*/

      goto ret_fun31_sec05;
   } /*If: only one chain*/

   /*****************************************************\
   * Fun31 Sec03 Sub02:
   *   - two chain case
   *   o fun31 sec03 sub02 cat01:
   *     - two chains, setup + find ending coordinats
   *   o fun31 sec03 sub02 Cat02:
   *     - two chains, handle non-merge cases
   *   o fun31 sec03 sub02 Cat03:
   *     - two chains, handle merge cases
   *   o fun31 sec03 sub02 Cat04:
   *     - goto return section
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun31 Sec03 Sub02 Cat01:
   +   - two chains, setup + find ending coordinates
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(chainsSTPtr->lenChainSI == 2)
   { /*Else If: might be quick merge*/

      chainsSTPtr->scoreArySI[0] =
         chainsSTPtr->chainArySI[0];
      chainsSTPtr->scoreArySI[1] =
         chainsSTPtr->chainArySI[1];
      maxNtSI = 0;

      /*find end of chains*/
      refUniqSI =
           chainsSTPtr->refArySI[0]
         + chainsSTPtr->chainArySI[0];
         /*ending coordinate of first reference chain*/
      refUniqSI =
           chainsSTPtr->refArySI[1]
         + chainsSTPtr->chainArySI[1];
         /*ending coordinate of second reference chain*/

      qryUniqSI =
           chainsSTPtr->qryArySI[0]
         + chainsSTPtr->chainArySI[0];
         /*ending coordiante of frist query chain*/
      qryEndSI =
           chainsSTPtr->qryArySI[1]
         + chainsSTPtr->chainArySI[1];
         /*ending coordiante of second query chain*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec03 Sub02 Cat02:
      +   - two chains, handle non-merge cases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*the chians are always sorted by reference and then
      `  query kmer starting position
      ` first check is checking if one chain enguffled the
      `   other or if second chain is before first
      */
      if(
            qryUniqSI >= qryEndSI
         || refUniqSI >= refEndSI
         ||
               chainsSTPtr->refArySI[0]
            >= chainsSTPtr->refArySI[1]
         ||   
               chainsSTPtr->qryArySI[0]
            >= chainsSTPtr->qryArySI[1]
      ){ /*If: chains can not be merged*/
         if(
              chainsSTPtr->chainArySI[0]
            > chainsSTPtr->chainArySI[1]
         ) maxIndexSI = 0; /*first chain is best*/

         else
            maxIndexSI = 1; /*second chain is best*/
      }  /*If: chains can not be merged*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec03 Sub02 Cat03:
      +   - two chains, handle merge cases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: can merge chains*/
         maxIndexSI = 0;

         /*find if have gap between chains*/
         refUniqSI = chainsSTPtr->refArySI[1] - refUniqSI;
         qryUniqSI = chainsSTPtr->qryArySI[1] - qryUniqSI;

         /*check if have overlap (gap)*/
         if(
               refUniqSI < 0
            || qryUniqSI < 0
         ){ /*If: have overlap*/
            maxNtSI =
               min_genMath(
                  refUniqSI,
                  qryUniqSI
               ); /*find most overlap (most negative)*/

            maxNtSI += chainsSTPtr->chainArySI[0];
            /*find length minus overlap*/
         }  /*If: have overlap*/

         else
            maxNtSI = chainsSTPtr->chainArySI[0];

         /*get chain length*/
         maxNtSI += chainsSTPtr->chainArySI[1];
         chainsSTPtr->scoreArySI[0] = maxNtSI;
         chainsSTPtr->nextArySI[0] = 1;
      } /*Else: can merge chains*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec03 Sub02 Cat04:
      +   - goto return section
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      goto ret_fun31_sec05;
   } /*Else If: might be quick merge*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun31 Sec04:
   ^   - merge chains
   ^   o fun31 sec04 sub01:
   ^     - find last mapped reference kmer
   ^   o fun31 sec04 sub02:
   ^     - merge kmers
   ^   o fun31 sec04 sub03:
   ^     - score last reference kmer chains
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun31 Sec04 Sub01:
   *   - find last mapped reference kmer
   \*****************************************************/

   lastKmerSI = chainsSTPtr->lastIndexSI;

   if(lastKmerSI < 0)
      goto noChains_fun31_sec05;
   else
   { /*Else: have chains to merge*/
      lastIndexSI = lastKmerSI;
      lastKmerSI = chainsSTPtr->indexArySI[lastKmerSI];
   } /*Else: have chains to merge*/

   /*****************************************************\
   * Fun31 Sec04 Sub02:
   *   - score last reference kmer chains
   \*****************************************************/

   siChain = chainsSTPtr->lenChainSI - 1;

   while(
         chainsSTPtr->qryArySI[siChain]
      >= chainsSTPtr->qryArySI[lastKmerSI]
   ){ /*Loop: merge kmer chains*/
      chainsSTPtr->scoreArySI[siChain] =
         chainsSTPtr->chainArySI[siChain];
      chainsSTPtr->nextArySI[siChain] = 0;/*end of merge*/

      /*want best score on first reference kmer, that
      `  way I avoid long gaps (hopefully)
      */
      if(chainsSTPtr->chainArySI[siChain] >= maxNtSI)
      { /*If: new best score*/
         secMaxNtSI = maxNtSI;
         maxNtSI = chainsSTPtr->chainArySI[siChain];
         maxIndexSI = siChain;   
      } /*If: new best score*/

      else if(
         chainsSTPtr->chainArySI[siChain] > secMaxNtSI
      ) secMaxNtSI = chainsSTPtr->chainArySI[siChain];
         /*so always have second best case*/

      --siChain;

      if(siChain < 0)
         goto ret_fun31_sec05;
         /*no chains can be merged*/
   } /*Loop: merge kmer chains*/

   /*****************************************************\
   * Fun31 Sec04 Sub03:
   *   - score chains
   *   o fun31 sec04 sub03 cat01:
   *     - loop though chains & find end coords of chain
   *   o fun31 sec04 sub03 cat02:
   *     - handel no overlap cases (use highest score)
   *   o fun31 sec04 sub03 cat03:
   *     - handel overlap, but still best score cases
   *   o fun31 sec04 sub03 cat04:
   *     - handle cases were need to find best score
   *   o fun31 sec04 sub03 cat05:
   *     - merge valid subchains
   *   o fun31 sec04 sub03 cat06:
   *     - move to next sub chain
   *   o fun31 sec04 sub03 cat07:
   *     - move back one chain
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun31 Sec04 Sub03 Cat01:
   +   - loop though chains & find end coords of chain
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   curMaxNtSI = maxNtSI;
   curSecMaxNtSI = secMaxNtSI;

   /*this is kinda like the pheramon trail for the ant
   `   traveling salesman problem.
   */
   while(siChain >= 0)
   { /*Loop: merge chains*/
      qryEndSI =
           chainsSTPtr->qryArySI[siChain]
         + chainsSTPtr->chainArySI[siChain];

      refEndSI =
           chainsSTPtr->qryArySI[siChain]
         + chainsSTPtr->chainArySI[siChain];

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec04 Sub03 Cat02:
      +   - handel no overlap cases (use highest score)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      qryUniqSI =
         qryEndSI - chainsSTPtr->qryArySI[maxIndexSI];
      refUniqSI =
         refEndSI - chainsSTPtr->refArySI[maxIndexSI];

      if(
            qryUniqSI < 0
         && refUniqSI < 0
      ){ /*If: no overlap*/
         chainsSTPtr->scoreArySI[siChain] =
            maxNtSI + chainsSTPtr->chainArySI[siChain];
         chainsSTPtr->nextArySI[siChain] = maxIndexSI;

         if(
               chainsSTPtr->scoreArySI[siChain]
            >= curMaxNtSI
         ){ /*If: new best score*/
            curSecMaxNtSI = curMaxNtSI;
            curMaxNtSI = chainsSTPtr->scoreArySI[siChain];
            curMaxIndexSI = siChain;
         } /*If: new best score*/

         else if(
              chainsSTPtr->scoreArySI[siChain]
            > curSecMaxNtSI
         ) curSecMaxNtSI=chainsSTPtr->scoreArySI[siChain];

         goto mvBack_fun31_sec04_sub03_cat07;
      }  /*If: no overlap*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec04 Sub03 Cat03:
      +   - handel overlap, but still best score cases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
              chainsSTPtr->chainArySI[siChain]
           +  min_genMath(refUniqSI, qryUniqSI)
           +  maxNtSI
         > secMaxNtSI
      ){ /*Else If: with overlap still is best score*/
         chainsSTPtr->scoreArySI[siChain] =
               chainsSTPtr->chainArySI[siChain]
            +  min_genMath(refUniqSI, qryUniqSI)
            + maxNtSI; /*find score*/

         chainsSTPtr->nextArySI[siChain] = maxIndexSI;

         if(
              chainsSTPtr->scoreArySI[siChain]
            >= curMaxNtSI
         ){ /*If: new best score*/
            curSecMaxNtSI = curMaxNtSI;
            curMaxNtSI = chainsSTPtr->scoreArySI[siChain];
            curMaxIndexSI = siChain;
         } /*If: new best score*/

         else if(
              chainsSTPtr->scoreArySI[siChain]
            > curSecMaxNtSI
         ) curSecMaxNtSI=chainsSTPtr->scoreArySI[siChain];

         goto mvBack_fun31_sec04_sub03_cat07;
      }  /*Else If: with overlap still is best score*/
         
      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec04 Sub03 Cat04:
      +   - handle cases were need to find best score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*this is the n^2 part, however, I am reallying
      `  on kmer space to be scarce enough that this step
      `  will be rare
      */

      siSub = lastKmerSI; /*last chain was on*/
      subIndexSI = lastIndexSI;/*qry index of last chain*/

      while(siSub < chainsSTPtr->lenChainSI)
      { /*Loop: scan next query kmer*/

         /*skipping chains I can not merge*/
         if(
                refEndSI
            >=  
                chainsSTPtr->refArySI[siSub]
              + chainsSTPtr->chainArySI[siSub]
         ) ;
           /*reference is before/engulfs chain*/

         else if(
              qryEndSI
            >=
                chainsSTPtr->qryArySI[siSub]
              + chainsSTPtr->chainArySI[siSub]
         ){ /*Else If: complete query overlap*/
             ++subIndexSI;
             siSub = chainsSTPtr->indexArySI[subIndexSI];
         }  /*Else If: complete query overlap*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun31 Sec04 Sub03 Cat05:
         +   - merge valid subchains
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: have gap or overlap (ref and query)*/
            qryUniqSI =
               chainsSTPtr->qryArySI[siSub] - qryEndSI;
            refUniqSI =
               chainsSTPtr->refArySI[siSub] - refEndSI;

            if(
                  refUniqSI < 0
               || qryUniqSI < 0
            ){ /*If: have overlap*/
               scoreSI = 
                  min_genMath( 
                     refUniqSI,
                     qryUniqSI
                  ); /*compinsate for overlap*/
            }  /*If: have overlap*/

            scoreSI = chainsSTPtr->chainArySI[siSub];
            scoreSI += chainsSTPtr->chainArySI[siChain];

            if(scoreSI > chainsSTPtr->scoreArySI[siChain])
            { /*If: new local best score*/
               chainsSTPtr->scoreArySI[siChain] = scoreSI;
               chainsSTPtr->nextArySI[siChain] = siSub;

               if(scoreSI > curMaxNtSI)
               { /*If: new best score*/
                  curSecMaxNtSI = maxNtSI;
                  curMaxNtSI = scoreSI;
                  curMaxIndexSI = siChain;
               } /*If: new best score*/

               else if(scoreSI > curSecMaxNtSI)
                  curSecMaxNtSI = scoreSI;
            } /*If: new local best score*/

            else if(scoreSI > curSecMaxNtSI)
               curSecMaxNtSI = scoreSI;
         } /*Else: have gap or overlap (ref and query)*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun31 Sec04 Sub03 Cat06:
         +   - move to next sub chain
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         ++siSub; /*move to next hit*/

      } /*Loop: scan next query kmer*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun31 Sec04 Sub03 Cat07:
      +   - move back one chain
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      mvBack_fun31_sec04_sub03_cat07:;
         --siChain;

         if(lastKmerSI <= 0)
            ; /*If: on last chain*/
         else if(siChain < 0)
            ; /*If: on last chain*/
         else if(
             chainsSTPtr->qryArySI[siChain]
           < chainsSTPtr->qryArySI[lastKmerSI - 1]
             /*lastKmerSI -1 lands on current kmer*/
         ){ /*If: on next reference kmer*/
             /*update to add in new maximum values for the
             `  next reference kmer (round)
             */
             maxNtSI = curMaxNtSI;
             maxIndexSI = curMaxIndexSI;
             secMaxNtSI = curSecMaxNtSI;


            if(lastIndexSI > 0)
               --lastIndexSI;
            else
               goto ret_fun31_sec05; /*finished*/

            lastKmerSI =
               chainsSTPtr->indexArySI[lastIndexSI];

            while(lastKmerSI < 0)
            { /*Loop: find previous kmer*/
               if(lastIndexSI < 0)
                  goto ret_fun31_sec05; /*finished*/

               --lastIndexSI;

               lastKmerSI =
                  chainsSTPtr->indexArySI[lastIndexSI];
            } /*Loop: find previous kmer*/
         } /*If: on next reference kmer*/
   } /*Loop: merge chains*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun31 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   maxNtSI = curMaxNtSI;
   maxIndexSI = curMaxIndexSI;
   secMaxNtSI = curSecMaxNtSI;

   goto ret_fun31_sec05;

   noChains_fun31_sec05:;
      maxIndexSI = def_noChains_mapRead;
      goto ret_fun31_sec05;

   ret_fun31_sec05:;
      return maxIndexSI;
} /*mergeChains_mapRead*/

/*-------------------------------------------------------\
| Fun32: mergeToSam_mapRead
|   - convert output from mergeChains_map read to sam
|     file entry (mapq is set to 0)
| Input:
|   - chainsSTPtr:
|     o chains_mapRead struct pointer with chains to use
|       in making the alignment
|   - indexSI:
|     o starting index of merged chain to align
|   - qrySTPtr:
|     o seqST struct with query sequence
|   - refSTPtr:
|     o seqST struct pointer with reference sequence
|   - lenKmerUC:
|     o length of one kmer in reference/query
|   - chainLenSI:
|     o minimum length of a chain
|     o used to align ends before chains, alignment size
|       is 1.5 chains (for chainLenSI = 20, is 30 bases)
|       * 1.5 is off the top of my head
|   - settings:
|     o settings for alignment
|   - matrixSTPtr:
|     o directional matrix to use in alignment step
|   - samSTPtr:
|     o samEntry struct pointer to add alignment to
| Output:
|   - Modifies:
|     o samSTPtr to have alignment; everything in samSTPtr
|       is modified
|     o matrixSTPtr (nothing usefull, here to reduce
|       memory allocations in alignment step)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
mergeToSam_mapRead(
   struct chains_mapRead *chainsSTPtr,/*chains to use*/
   signed int indexSI,            /*chain index to start*/
   struct seqST *qrySTPtr,     /*query sequence*/
   struct seqST *refSTPtr,     /*reference sequence*/
   unsigned char lenKmerUC,    /*length of one kmer*/
   signed int chainLenSI,      /*min length of one chain*/
   struct alnSet *settings,      /*aligment settings*/
   struct dirMatrix *matrixSTPtr,/*for alignment step*/
   struct samEntry *samSTPtr     /*gets final alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun32 TOC:
   '   - convert output form mergeChains_map read to sam
   '     file entry (mapq is set to 0)
   '   o fun32 sec01:
   '     - variable declarations
   '   o fun32 sec02:
   '     - initialize and setup
   '   o fun32 sec03:
   '     - align ending soft masked bases
   '   o fun32 sec04:
   '     - align ending soft masked bases
   '   o fun32 sec05:
   '     - finish up samEntry values (no mapq)
   '   o fun32 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int lastChainBl = 0;
      /*tells if have done last chain yet*/

   /*coordiantes for aligning chunks*/
   signed int refStartSI = 0;
   signed int refEndSI = 0;
   signed int refLastEndSI = 0;

   signed int qryStartSI = 0;
   signed int qryEndSI = 0;
   signed int qryLastEndSI = 0;
   signed int qryLenSI = 0; /*for overlap indel detect*/
   signed int refLenSI = 0; /*for overlap indel detect*/

   /*for alignments*/
   unsigned int maskUI = 0; /*throw away*/
   unsigned int anonUI = 0; /*throw away*/
   unsigned int tmpUI = 0;  /*cigar lengths (throw away)*/

   /*for sam entry editing*/
   signed char *swapSCPtr = 0; /*for cigar reallocs*/
   signed int siCig = 0;
   signed int newCigPosSI = 0;

   /*for dealing with soft masked bases at start*/
   signed int *cigHeapArySI = 0;
   signed char *cigHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec02:
   ^   - initialize and setup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   chainLenSI += (chainLenSI >> 1); /*softmasks at end*/

   cigHeapArySI =
      malloc((chainLenSI +1) * sizeof(signed int));
   if(! cigHeapArySI)
      goto memErr_fun32_sec06;

   cigHeapStr =
      malloc((chainLenSI + 1) * sizeof(signed char));
   if(! cigHeapStr)
      goto memErr_fun32_sec06;


   /*for merged chain gap filing*/
   blank_samEntry(samSTPtr);
   samSTPtr->flagUS = 0; /*mark forward map*/


   /*setup reference start coords*/
   samSTPtr->refStartUI =
      (unsigned int) chainsSTPtr->refArySI[indexSI];

   if(chainsSTPtr->qryArySI[indexSI] > 0)
   { /*If: have starting softmasking*/
      samSTPtr->cigTypeStr[0] = 'S';

      samSTPtr->cigArySI[0] =
         chainsSTPtr->qryArySI[indexSI];
         /*coordinates in index 0, but cigar index 1*/

      siCig = 1;
      samSTPtr->lenCigUI = 1;
   } /*If: have starting softmasking*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec03:
   ^   - align gaps in merged chains
   ^   o fun32 sec03 sub01:
   ^     - expand cigar array memory + start loop
   ^   o fun32 sec03 sub02:
   ^     - find first and last base in current chain
   ^   o fun32 sec03 sub03:
   ^     - detect if have overlap in chains
   ^   o fun32 sec03 sub04:
   ^     - handel easy insertion/deletion cases
   ^   o fun32 sec03 sub05:
   ^     - handel easy (single) snp cases
   ^   o fun32 sec03 sub06:
   ^     - handel gaps that need to be aligned
   ^   o fun32 sec03 sub07:
   ^     - add chain length to matches
   ^   o fun32 sec03 sub08:
   ^     - move to next chain in merge
   ^   o fun32 sec03 sub09:
   ^     - move to next chain in merge
   ^     - check if have last chain to merge
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun32 Sec03 Sub01:
   *   - expand cigar array memory (if need) + start loop
   \*****************************************************/

   samSTPtr->cigTypeStr[siCig] = '=';
   samSTPtr->cigArySI[siCig] =
      chainsSTPtr->chainArySI[indexSI];
   samSTPtr->cigArySI[siCig] += (lenKmerUC - 1);
        /*last kmer is always two bases off*/
   ++siCig;

   qryLastEndSI = chainsSTPtr->qryArySI[indexSI];
   qryLastEndSI += chainsSTPtr->chainArySI[indexSI];
   qryLastEndSI += (lenKmerUC - 2);
     /*last kmer is always one base off (-1)
     `  and in index one (-1; total -2)
     */

   refLastEndSI = chainsSTPtr->refArySI[indexSI];
   refLastEndSI += chainsSTPtr->chainArySI[indexSI];
   refLastEndSI += (lenKmerUC - 2);
     /*last kmer is always one base off (-1)
     `  and in index one (-1; total -2)
     */

   goto nextChain_fun32_sec03_sub07;
   /*first index will often be 0*/

   while(chainsSTPtr->nextArySI[indexSI] > 0)
   { /*Loop: while have chains to merge*/
      mergeChains_fun32_sec03_sub07:;

      /*doing + 3 because max for this loop is two
      `   entries, the thrid is for a softmask ending
      */
      if(
        (siCig +3) >= (signed int) samSTPtr->lenCigBuffUI)
      { /*If: need more cigar memory*/
         newCigPosSI= (signed int) samSTPtr->lenCigBuffUI;
         newCigPosSI += (newCigPosSI >> 1);

         swapSCPtr =
            realloc(
               samSTPtr->cigTypeStr,
               (newCigPosSI + 1) * sizeof(signed char)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigTypeStr = swapSCPtr;

         swapSCPtr =
            (signed char *)
            realloc(
               samSTPtr->cigArySI,
               (newCigPosSI + 1) * sizeof(signed int)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigArySI = (signed int *) swapSCPtr;

         samSTPtr->lenCigBuffUI =
            (unsigned int) newCigPosSI;
      } /*If: need more cigar memory*/

      /**************************************************\
      * Fun32 Sec03 Sub02:
      *   - find first and last base in current chain
      \**************************************************/

      /*find start and end of kmer chain*/
      qryStartSI = chainsSTPtr->qryArySI[indexSI];
      qryEndSI = qryStartSI;
      qryEndSI += chainsSTPtr->chainArySI[indexSI];
      qryEndSI += (lenKmerUC - 2);
        /*last kmer is always one base off (-1)
        `  and in index one (-1; total -2)
        */

      refStartSI = chainsSTPtr->refArySI[indexSI];
      refEndSI = refStartSI;
      refEndSI += chainsSTPtr->chainArySI[indexSI];
      refEndSI += (lenKmerUC - 2);
        /*last kmer is always one base off (-1)
        `  and in index one (-1; total -2)
        */

      /**************************************************\
      * Fun32 Sec03 Sub03:
      *   - detect if have overlap in chains
      \**************************************************/

      /*account for overlaps in kmer chains*/
      if(qryStartSI < qryLastEndSI)
      { /*If: have overlap*/
         chainOverlap_fun32_sec02_sub03:;

         qryLenSI = qryEndSI - qryLastEndSI;
         refLenSI = refEndSI - refLastEndSI;

         if(qryLenSI < refLenSI)
         { /*If: overlap caused a deletion*/
            samSTPtr->cigTypeStr[siCig] = 'D';
            samSTPtr->cigArySI[siCig] = refLenSI;
            samSTPtr->cigArySI[siCig] -= qryLenSI;

            ++siCig;
            qryStartSI = qryLastEndSI + 1;
               /*account for overlap*/
         } /*If: overlap caused a deletion*/

         else if(qryLenSI > refLenSI)
         { /*If: overlap caused an indel*/
            samSTPtr->cigTypeStr[siCig] = 'I';
            samSTPtr->cigArySI[siCig] = qryLenSI;
            samSTPtr->cigArySI[siCig] -= refLenSI;

            ++siCig;
            refStartSI = refLastEndSI + 1;
               /*account for overlap*/
         } /*If: overlap caused an deletion*/

         else
         { /*Else: overlap in bases (no indels)*/
            qryStartSI = qryLastEndSI + 1;
            refStartSI = qryLastEndSI + 1;
         } /*Else: overlap in bases (no indels)*/
         /*this case should never happen*/
      } /*If: have overlap*/

      else if(refStartSI < refLastEndSI)
         goto chainOverlap_fun32_sec02_sub03;
         /*means overlap between chains, this is most
         `  likely to happen for indels in homopolymers
         */

      /**************************************************\
      * Fun32 Sec03 Sub04:
      *   - handel easy insertion/deletion cases
      \**************************************************/

      /*check if have easy call*/
      else if((refStartSI - refLastEndSI) == 0)
      { /*If: no reference gap (means query gap)*/
         samSTPtr->cigTypeStr[siCig] = 'I';
         samSTPtr->cigArySI[siCig] =
            qryStartSI - qryLastEndSI; /*gap size*/

         ++siCig;
      } /*If: no reference gap (means query gap)*/

      else if((qryStartSI - qryLastEndSI) == 0)
      { /*If: no query gap (means reference gap)*/
         samSTPtr->cigTypeStr[siCig] = 'D';
         samSTPtr->cigArySI[siCig] =
            refStartSI - refLastEndSI; /*gap size*/

         ++siCig;
      } /*If: no query gap (means reference gap)*/

      /**************************************************\
      * Fun32 Sec03 Sub05:
      *   - handel easy (single) snp cases
      \**************************************************/

      else if(
            qryStartSI - qryLastEndSI == 2
         && refStartSI - refLastEndSI == 2
      ){ /*Else If: mismatch*/
         samSTPtr->cigTypeStr[siCig] = 'X';
         samSTPtr->cigArySI[siCig] = 1;
         ++siCig;
      }  /*Else If: mismatch*/

      /**************************************************\
      * Fun32 Sec03 Sub06:
      *   - handel gaps that need to be aligned
      \**************************************************/

      else
      { /*Else: need to align*/
         ++qryLastEndSI;  /*move into gap between chains*/
         qrySTPtr->offsetUL = qryLastEndSI;
         qrySTPtr->endAlnUL = (unsigned long) qryStartSI;
         --qrySTPtr->endAlnUL;

         ++refLastEndSI;  /*move into gap between chains*/
         refSTPtr->offsetUL = refLastEndSI;
         refSTPtr->endAlnUL = (unsigned long) refStartSI;
         --refSTPtr->endAlnUL;

         needle(
            qrySTPtr,
            refSTPtr,
            matrixSTPtr,
            settings
         );

         if(matrixSTPtr->errSC == def_memErr_needle)
            goto memErr_fun32_sec06;

         newCigPosSI =
            getCig_dirMatrix(
              matrixSTPtr,           /*has alignment*/
              0,                     /*use matrix index*/
              0,                     /*always forward*/
              qrySTPtr,              /*query sequence*/
              refSTPtr,              /*reference sequence*/
              &samSTPtr->cigTypeStr, /*cigar type array*/
              &samSTPtr->cigArySI,  /*has cigar lengths*/
              samSTPtr->lenCigUI,   /*position in cigar*/
              &samSTPtr->lenCigUI,  /*length of cigar*/
              (unsigned int *) &refLastEndSI,/*aln start*/
              &anonUI,               /*ignoring*/
              &maskUI,               /*ignoring*/
              1,                     /*keep ending dels*/
              settings               /*settings for aln*/
            ); /*add aligment to cigar*/

         siCig = newCigPosSI + 1;

         if( newCigPosSI < 0)
            goto memErr_fun32_sec06;

      } /*Else: need to align*/

      /**************************************************\
      * Fun32 Sec03 Sub07:
      *   - add chain length to matches
      \**************************************************/

      samSTPtr->refEndUI = (unsigned int) refEndSI;
         /*mark end of alignment*/
      samSTPtr->readLenUI = (unsigned int) qryEndSI;
         /*so know last query base*/

      /*setup for next round*/
      qryLastEndSI = qryEndSI;
      refLastEndSI = refEndSI;

      /*add chain lengths in*/
      qryEndSI -= qryStartSI; /*find query length*/
      refEndSI -= refStartSI; /*find reference length*/
         /*already accounted for extra bases in last
         `  kmer when found qryEndSI and refEndSI
         */

      samSTPtr->cigArySI[siCig] =
         min_genMath(
            qryEndSI,
            refEndSI
         );
      ++samSTPtr->cigArySI[siCig];/*convert to index 1*/

      samSTPtr->cigTypeStr[siCig++] = '=';
      samSTPtr->lenCigUI = (unsigned int) siCig;

      /**************************************************\
      * Fun32 Sec03 Sub08:
      *   - move to next chain in merge
      \**************************************************/

      nextChain_fun32_sec03_sub07:;
         if(chainsSTPtr->nextArySI[indexSI])
            indexSI = chainsSTPtr->nextArySI[indexSI];
            /*move to next chain in merge*/
   } /*Loop: while have chains to merge*/

   /*****************************************************\
   * Fun32 Sec03 Sub09:
   *   - check if have last chain to merge
   \*****************************************************/

   if(! lastChainBl)
   { /*If: still need to finish the last chain*/
      lastChainBl = 1;
      goto mergeChains_fun32_sec03_sub07;
   } /*If: still need to finish the last chain*/

   if(
        qryLastEndSI
      < ( (signed int) (qrySTPtr->lenSeqUL - 1) )
   ){ /*If: have softmasking at end*/
      /*cig softmasking length at end*/
      samSTPtr->cigArySI[siCig] =
        (signed int) qrySTPtr->lenSeqUL;

      samSTPtr->cigArySI[siCig] -= qryLastEndSI;
      --samSTPtr->cigArySI[siCig];
         /*lenSeqUL is index 1, but qryLastEndSI is index
         `  0, so need to account for diference
         */

      samSTPtr->cigTypeStr[siCig++] = 'S';
      samSTPtr->cigTypeStr[siCig] = '\0';
      ++samSTPtr->lenCigUI;

      samSTPtr->readLenUI =
         (signed int) qrySTPtr->lenSeqUL;
   } /*If: have softmasking at end*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec04:
   ^   - align starting soft masked bases
   ^   o fun32 sec04 sub01:
   ^     - deal with starting softmasking
   ^   o fun32 sec04 sub02:
   ^     - deal with ending softmasking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun32 Sec04 Sub01:
   *   - deal with starting softmasking
   *   o fun32 sec04 sub01 cat01:
   *     - find softamsking coordinates to align
   *   o fun32 sec04 sub01 cat02:
   *     - do waterman alignment8
   *   o fun32 sec04 sub01 cat03:
   *     - get cigar and check if keeping
   *   o fun32 sec04 sub01 cat04:
   *     - check if keeping cigar entries
   *   o fun32 sec04 sub01 cat05:
   *     - realloc main memory (if more needed)
   *   o fun32 sec04 sub01 cat06:
   *     - add new cigar to samEntry
   *   o fun32 sec04 sub01 cat07:
   *     - adjust softmasking at start
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun32 Sec04 Sub01 Cat01:
   +   - find softamsking coordinates to align
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(samSTPtr->cigTypeStr[0] == 'S')
   { /*If: have softmasking at start*/
      /*adding to end so can do swap*/

      if(samSTPtr->refStartUI < (unsigned int) chainLenSI)
         refStartSI = 0;
      else
         refStartSI =
            (signed int)
            samSTPtr->refStartUI - chainLenSI;

      if(samSTPtr->cigArySI[0] < chainLenSI)
         qryStartSI = 0;
      else
         qryStartSI =
            (signed int)
            samSTPtr->cigArySI[0] - chainLenSI;

      if(samSTPtr->refStartUI > 0)
         refEndSI = samSTPtr->refStartUI - 1;
      else
         refEndSI = 0;

      qryEndSI = samSTPtr->cigArySI[0];


      /*check if both sequences have softmasking*/
      if(refEndSI - refStartSI <= 0)
         goto skipStartMask_fun32_sec04_sub02;
      else if (qryEndSI - qryStartSI <= 0)
         goto skipStartMask_fun32_sec04_sub02;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub01 Cat02:
      +   - do waterman alignment
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      qrySTPtr->offsetUL = (unsigned long) qryStartSI;
      qrySTPtr->endAlnUL = (unsigned long) qryEndSI;

      refSTPtr->offsetUL = (unsigned long) refStartSI;
      refSTPtr->endAlnUL = (unsigned long) refEndSI;

       water(
         qrySTPtr,
         refSTPtr,
         matrixSTPtr,
         settings
      );

      if(matrixSTPtr->errSC == def_memErr_water)
         goto memErr_fun32_sec06;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub01 Cat03:
      +   - get cigar and check if keeping
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      refLastEndSI = refStartSI;
      tmpUI = chainLenSI;
      cigHeapStr[0] = '\0';

      newCigPosSI =
         getCig_dirMatrix(
           matrixSTPtr,           /*has alignment*/
           0,                     /*use matrix index*/
           0,                     /*always forward*/
           qrySTPtr,              /*query sequence*/
           refSTPtr,              /*reference sequence*/
           &cigHeapStr,           /*cigar type array*/
           &cigHeapArySI,         /*has cigar lengths*/
           0,                     /*first cigar pos (0)*/
           &tmpUI,                /*cigar buffer size*/
           (unsigned int *) &refLastEndSI,/*start of aln*/
           &anonUI,               /*ignoring*/
           &maskUI,               /*ignoring*/
           0,                     /*keep ending dels*/
           settings               /*settings for aln*/
         ); /*add aligment to cigar*/
      
      if(newCigPosSI < 0)
         goto memErr_fun32_sec06;

      if(cigHeapStr[newCigPosSI] == 'S')
         goto skipStartMask_fun32_sec04_sub02;

      siCig = (unsigned int) (cigHeapStr[0] == 'S');
      ++newCigPosSI; /*convert to index 1*/
      maskUI = (unsigned int) (newCigPosSI - siCig);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub01 Cat05:
      +   - realloc main memory (if more needed)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      indexSI = (signed int) samSTPtr->lenCigUI + maskUI;

      if(indexSI >= (signed int) samSTPtr->lenCigBuffUI)
      { /*If: need more memory*/

         swapSCPtr =
            realloc(
               samSTPtr->cigTypeStr,
               (indexSI + 1) * sizeof(signed char)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigTypeStr = swapSCPtr;

         swapSCPtr =
            (signed char *)
            realloc(
               samSTPtr->cigArySI,
               (indexSI + 1) * sizeof(signed int)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigArySI = (signed int *) swapSCPtr;

         samSTPtr->lenCigBuffUI = (unsigned int) indexSI;
         
      } /*If: need more memory*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub01 Cat06:
      +   - add new cigar to samEntry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      indexSI = (signed int) samSTPtr->lenCigUI - 1;
      indexSI += maskUI;
      /*removes softmasking at start if needed*/

      siCig = samSTPtr->lenCigUI;
      samSTPtr->lenCigUI = indexSI + 1;

      while(siCig > 0)
      { /*Loop: move cigar up*/
         samSTPtr->cigTypeStr[indexSI] ^=
            samSTPtr->cigTypeStr[siCig];
         samSTPtr->cigTypeStr[siCig] ^=
            samSTPtr->cigTypeStr[indexSI];
         samSTPtr->cigTypeStr[indexSI] ^=
            samSTPtr->cigTypeStr[siCig];

         samSTPtr->cigArySI[indexSI] ^=
            samSTPtr->cigArySI[siCig];
         samSTPtr->cigArySI[siCig] ^=
            samSTPtr->cigArySI[indexSI];
         samSTPtr->cigArySI[indexSI] ^=
            samSTPtr->cigArySI[siCig];

         --siCig;
         --indexSI;
      } /*Loop: move cigar up*/

      siCig = 1;
      indexSI = newCigPosSI - (signed int) maskUI;
         /*find first cigar entry to copy*/
 
      while(indexSI < (signed int) maskUI)
      { /*Loop: add new cigar entries in*/
         samSTPtr->cigTypeStr[siCig]= cigHeapStr[indexSI];
         samSTPtr->cigArySI[siCig]=cigHeapArySI[indexSI];

         if(cigHeapStr[indexSI] != 'I')
            samSTPtr->refStartUI -= cigHeapArySI[indexSI];
            /*adjust reference starting position*/

         ++siCig;
         ++indexSI;
      } /*Loop: add new cigar entries in*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub01 Cat07:
      +   - adjust softmasking at start
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*removed aligned bases from softmasked entry*/

      if(samSTPtr->cigArySI[0] >= chainLenSI)
         samSTPtr->cigArySI[0] -= chainLenSI;
      else
         samSTPtr->cigArySI[0] = 0;

      if(cigHeapStr[0] == 'S')
         samSTPtr->cigArySI[0] += cigHeapStr[0];
         /*added aligned softmasking to cigar*/

      if(samSTPtr->cigArySI[0] <= 0)
      { /*If: no softmasking at start*/

         /*bit inefficent, but works*/

         siCig = 0;
         while(siCig < (signed int) samSTPtr->lenCigUI)
         { /*Loop: move cigar entries back one*/
            samSTPtr->cigTypeStr[siCig] ^=
               samSTPtr->cigTypeStr[siCig + 1];
            samSTPtr->cigTypeStr[siCig + 1] ^=
               samSTPtr->cigTypeStr[siCig];
            samSTPtr->cigTypeStr[siCig] ^=
               samSTPtr->cigTypeStr[siCig + 1];

            samSTPtr->cigArySI[siCig] ^=
               samSTPtr->cigArySI[siCig + 1];
            samSTPtr->cigArySI[siCig + 1] ^=
               samSTPtr->cigArySI[siCig];
            samSTPtr->cigArySI[siCig] ^=
               samSTPtr->cigArySI[siCig + 1];

            ++siCig;
         } /*Loop: move cigar entries back one*/

         --samSTPtr->lenCigUI;
      } /*If: no softmasking at start*/
   } /*If: have softmasking at start*/

   /*****************************************************\
   * Fun32 Sec04 Sub02:
   *   - deal with ending softmasking
   *   o fun32 sec04 sub02 cat01:
   *     - find softamsking coordinates to align
   *   o fun32 sec04 sub02 cat02:
   *     - do waterman alignment8
   *   o fun32 sec04 sub02 cat03:
   *     - get cigar and check if keeping
   *   o fun32 sec04 sub02 cat04:
   *     - realloc main memory (if more needed)
   *   o fun32 sec04 sub02 cat05:
   *     - add new cigar to samEntry
   *   o fun32 sec04 sub02 cat06:
   *     - adjust softmasking at start
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun32 Sec04 Sub02 Cat01:
   +   - find softamsking coordinates to align
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipStartMask_fun32_sec04_sub02:;

   siCig = samSTPtr->lenCigUI - 1;

   if(samSTPtr->cigTypeStr[siCig] == 'S')
   { /*If: have ending softmasking*/
      /*adding to end so can do swap*/

      refStartSI = (signed int) samSTPtr->refEndUI;
      refEndSI = (signed int) refSTPtr->lenSeqUL;
      --refEndSI; /*convert index one to index zero*/

      qryStartSI = (signed int) qrySTPtr->lenSeqUL;
      qryEndSI = qryStartSI;
      qryEndSI -= (signed int) samSTPtr->readLenUI;
      --qryEndSI; /*convert index 1 to index 0*/


      if(
            refEndSI < 1
         || qryEndSI < 1
      ) goto skipEndMask_fun32_sec05;
        /*at least on position has no softmasking*/


      if(refEndSI < chainLenSI)
         refEndSI = (signed int) refSTPtr->lenSeqUL - 1;
      else
         refEndSI = refStartSI + chainLenSI - 1;
            

      if(qryEndSI < chainLenSI)
         qryEndSI = (signed int) qrySTPtr->lenSeqUL - 1;
      else
         qryEndSI = qryStartSI + chainLenSI - 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub02 Cat02:
      +   - do waterman alignment
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      qrySTPtr->offsetUL = (unsigned long) qryStartSI;
      qrySTPtr->endAlnUL = (unsigned long) qryEndSI;

      refSTPtr->offsetUL = (unsigned long) refStartSI;
      refSTPtr->endAlnUL = (unsigned long) refEndSI;

      water(
         qrySTPtr,
         refSTPtr,
         matrixSTPtr,
         settings
      );

      if(matrixSTPtr->errSC == def_memErr_water)
         goto memErr_fun32_sec06;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub02 Cat03:
      +   - get cigar and check if keeping
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      refLastEndSI = refStartSI;
      tmpUI = chainLenSI;
      cigHeapStr[0] = '\0';

      newCigPosSI =
         getCig_dirMatrix(
            matrixSTPtr,         /*has alignment*/
            0,                   /*use matrix index*/
            0,                   /*always forward*/
            qrySTPtr,            /*query sequence*/
            refSTPtr,            /*reference sequence*/
            &cigHeapStr,         /*cigar type array*/
            &cigHeapArySI,       /*has cigar lengths*/
            0,                     /*first cigar pos (0)*/
            &tmpUI,                /*cigar buffer size*/
            (unsigned int *) &refStartSI,/*start of aln*/
            &anonUI,             /*ignoring*/
            &maskUI,             /*ignoring*/
            1,                   /*keep ending dels*/
            settings             /*settings for aln*/
         ); /*add aligment to cigar*/
      
      if(newCigPosSI < 0)
         goto memErr_fun32_sec06;


      if(cigHeapStr[0] == 'S')
         goto skipEndMask_fun32_sec05;
         /*softmasking between alignment end and aligned*/

      ++newCigPosSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub02 Cat04:
      +   - realloc main memory (if more needed)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      indexSI =
         (signed int) samSTPtr->lenCigUI + newCigPosSI;

      if(indexSI >= (signed int) samSTPtr->lenCigBuffUI)
      { /*If: need more memory*/

         swapSCPtr =
            realloc(
               samSTPtr->cigTypeStr,
               (indexSI + 2) * sizeof(signed char)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigTypeStr = swapSCPtr;

         swapSCPtr =
            (signed char *)
            realloc(
               samSTPtr->cigArySI,
               (indexSI + 2) * sizeof(signed int)
            );
         if(! swapSCPtr)
            goto memErr_fun32_sec06;
         samSTPtr->cigArySI = (signed int *) swapSCPtr;

         samSTPtr->lenCigBuffUI = (unsigned int) indexSI;
         
      } /*If: need more memory*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun32 Sec04 Sub02 Cat05:
      +   - add new cigar to samEntry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      siCig = samSTPtr->lenCigUI;
      samSTPtr->lenCigUI = indexSI;
      indexSI = 0;

      while(siCig < (signed int) samSTPtr->lenCigUI)
      { /*Loop: move cigar up*/
         if(cigHeapStr[indexSI] == 'S')
            break;
         else if(cigHeapStr[indexSI] == '\0')
            break;

         if(cigHeapStr[siCig] != 'I')
            samSTPtr->refEndUI += cigHeapArySI[siCig];
            /*reference length*/


         samSTPtr->cigTypeStr[siCig] =
            cigHeapStr[indexSI];

         samSTPtr->cigArySI[siCig] =
            cigHeapArySI[indexSI];


         ++siCig;
         ++indexSI;
      } /*Loop: move cigar up*/

      samSTPtr->cigTypeStr[siCig] = '\0';
      samSTPtr->lenCigUI = siCig;
   } /*If: have ending softmasking*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec05:
   ^   - finish up samEntry values (no mapq)
   ^   o fun32 sec05 sub01:
   ^     - add ending softmasking in
   ^   o fun32 sec05 sub02:
   ^     - add read lengths
   ^   o fun32 sec05 sub03:
   ^     - copy read ids
   ^   o fun32 sec05 sub04:
   ^     - copy sequence
   ^   o fun32 sec05 sub05:
   ^     - copy q-score
   ^   o fun32 sec05 sub06:
   ^     - find mutation counts
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun32 Sec05 Sub01:
   *   - add ending softmasking in
   \*****************************************************/

   skipEndMask_fun32_sec05:;

   /*add ending softmasking*/
   if(samSTPtr->readLenUI < qrySTPtr->lenSeqUL)
   { /*If: have softmasking at end*/

      samSTPtr->cigTypeStr[samSTPtr->lenCigUI] = 'S';

      samSTPtr->cigArySI[samSTPtr->lenCigUI] =
         qrySTPtr->lenSeqUL - samSTPtr->readLenUI;

      ++samSTPtr->lenCigUI;
   } /*If: have softmasking at end*/

   /*****************************************************\
   * Fun32 Sec05 Sub02:
   *   - add read lengths
   \*****************************************************/

   /*add in read lengths*/
   samSTPtr->readLenUI =
      (unsigned int) qrySTPtr->lenSeqUL;

   samSTPtr->alnReadLenUI =
      samSTPtr->refEndUI - samSTPtr->refStartUI;

   /*****************************************************\
   * Fun32 Sec05 Sub03:
   *   - copy read ids
   \*****************************************************/

   if(refSTPtr->idStr[0] == '@')
      indexSI = 1;
   else if(refSTPtr->idStr[0] == '>')
      indexSI = 1;
   else
      indexSI = 0;

   /*copy read ids*/
   samSTPtr->lenRefIdUC =
      (unsigned int)
      cpWhite_ulCp(
         samSTPtr->refIdStr,
         &refSTPtr->idStr[indexSI]
      );


   if(qrySTPtr->idStr[0] == '@')
      indexSI = 1;
   else if(qrySTPtr->idStr[0] == '>')
      indexSI = 1;
   else
      indexSI = 0;

   samSTPtr->lenQryIdUC =
      (unsigned int)
      cpWhite_ulCp(
         samSTPtr->qryIdStr,
         &qrySTPtr->idStr[indexSI]
      );

   /*****************************************************\
   * Fun32 Sec05 Sub04:
   *   - copy sequence
   \*****************************************************/

   if(samSTPtr->lenSeqBuffUI < qrySTPtr->lenSeqUL)
   { /*If: need more memory for sequence*/
      free(samSTPtr->seqStr);
      samSTPtr->seqStr = 0;

      samSTPtr->seqStr =
         malloc(
              (qrySTPtr->lenSeqUL + 9)
            * sizeof(unsigned char)
         );

      if(! samSTPtr)
         goto memErr_fun32_sec06;

      samSTPtr->lenSeqBuffUI =
        (unsigned int) qrySTPtr->lenSeqUL;
   } /*If: need more memory for sequence*/

   samSTPtr->readLenUI =
      cpStr_ulCp(
         samSTPtr->seqStr,
         qrySTPtr->seqStr
      );

   indexToSeq_alnSet(samSTPtr->seqStr);
     /*sequence is in index format, but I need a human
     `   readable format
     */

   /*****************************************************\
   * Fun32 Sec05 Sub05:
   *   - copy q-score
   \*****************************************************/

   if(
         qrySTPtr->qStr
      && qrySTPtr->qStr[0] != '\0'
   ){ /*If: have q-score entry*/

      if(samSTPtr->lenQBuffUI < qrySTPtr->lenSeqUL)
      { /*If: need more memory*/
         free(samSTPtr->qStr);
         samSTPtr->qStr = 0;

         samSTPtr->qStr =
            malloc(
                 (qrySTPtr->lenSeqUL + 9)
               * sizeof(unsigned char)
            );

         if(! samSTPtr)
            goto memErr_fun32_sec06;

         samSTPtr->lenSeqBuffUI =
           (unsigned int) qrySTPtr->lenSeqUL;
      } /*If: need more memory*/

      cpQEntry_samEntry(
         samSTPtr,
         qrySTPtr->qStr,
         0 /*histgram and q-score values already blanked*/
      ); /*copy q-score entries*/
   } /*If: have q-score entry*/

   else
   { /*Else: mark no q-score entry*/
      samSTPtr->qStr[0] = '*';
      samSTPtr->qStr[1] = '\0';
   } /*Else: mark no q-score entry*/

   /*****************************************************\
   * Fun32 Sec05 Sub06:
   *   - find mutation counts
   \*****************************************************/

   /*this would be faster if I did it as I aligned,
   `   however, this is eaiser. It should also add little
   `   overhead
   */

   for(
      siCig = 0;
      siCig < (signed int) samSTPtr->lenCigUI;
      ++siCig
   ){ /*Loop: get mutation counts*/

      switch(samSTPtr->cigTypeStr[siCig])
      { /*Switch: find mutation type*/
         case '=':
         /*Case: match*/
            samSTPtr->numMatchUI +=
               samSTPtr->cigArySI[siCig];
            break;
         /*Case: match*/

         case 'X':
         /*Case: mismatch*/
            samSTPtr->numSnpUI +=
               samSTPtr->cigArySI[siCig];
            break;
         /*Case: mismatch*/

         case 'I':
         /*Case: insertion*/
            samSTPtr->numInsUI +=
               samSTPtr->cigArySI[siCig];
            break;
         /*Case: insertion*/

         case 'D':
         /*Case: deletion*/
            samSTPtr->numDelUI +=
               samSTPtr->cigArySI[siCig];
            break;
         /*Case: deletion*/

         case 'S':
         /*Case: soft mask*/
            samSTPtr->numMaskUI +=
               samSTPtr->cigArySI[siCig];
            break;
         /*Case: soft mask*/
      } /*Switch: find mutation type*/

   }  /*Loop: get mutation counts*/

   samSTPtr->alnReadLenUI = samSTPtr->numMatchUI;
   samSTPtr->alnReadLenUI += samSTPtr->numSnpUI;
   samSTPtr->alnReadLenUI += samSTPtr->numDelUI;

   samSTPtr->refEndUI = samSTPtr->refStartUI;
   samSTPtr->refEndUI += samSTPtr->alnReadLenUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun32 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun32_sec06;

   memErr_fun32_sec06:;
      errSC = def_memErr_mapRead;
      goto ret_fun32_sec06;
   ret_fun32_sec06:;
      qrySTPtr->offsetUL = 0;
      qrySTPtr->endAlnUL = qrySTPtr->lenSeqUL - 1;

      refSTPtr->offsetUL = 0;
      refSTPtr->endAlnUL = refSTPtr->lenSeqUL - 1;

      if(cigHeapArySI)
         free(cigHeapArySI);
      cigHeapArySI = 0;

      if(cigHeapStr)
         free(cigHeapStr);
      cigHeapStr = 0;

      return errSC;
} /*mergeToSam_mapRead*/

/*-------------------------------------------------------\
| Fun33: scoreSubAln_mapRead
|   - scores the aligment in samEntry and finds best
|     sub-aligment
| Input:
|   - samSTPtr:
|     o samEntry struct pointer with alignment to score
|     o do not trim
|   - subBl:
|     o modifiy alignment to best sub-alignment
|       * 1: look for best sub-alignment
|       * 0: do not look for sub-alignments
|   - refSTPtr:
|     o seqST struct pointer with reference sequence used
|       in alignment
|     o seqStr must be as lookup index (seqToIndex_alnSet)
|   - qrySTPtr:
|     o seqST struct pointer with query sequence used in
|       alignment
|     o seqStr must be as lookup index (seqToIndex_alnSet)
|   - alnSetSTPTr:
|     o alnSet struct pointer with scoring settings
|   - maxScoreSLPtr:
|     o signed long pointer to hold maximum score possible
|       for the aligned part of the query
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o extraStr in samSTPtr to have score "AS:i:score"
|       entry
|     o cigTypeStr and cigArySI in samSTPtr to be setup
|       for best sub-alignment (if subBl is 1)
|     o lenCigUI in samSTPtr to be new cigar length if
|       looking for best sub-alignment
|     o maxScoreSLPtr to have maximum possible score for
|       returned alignment (exlcudes unaligned bases)
|     o errSCPtr:
|       * 0 for no errors
|       * def_noScore_mapRead if no sub-score greater
|         than 0 (unlikley)
|       * def_memErr_mapRead for memory errors
|   - Returns:
|     o score for best sub-alignment or if subSl is 0, for
|       the entire samEntry
\-------------------------------------------------------*/
signed long
scoreSub_mapRead(
   struct samEntry *samSTPtr, /*alignment to score*/
   signed char subBl,         /*1:look for sub-alingment*/
   struct seqST *refSTPtr,    /*reference sequence*/
   struct seqST *qrySTPtr,    /*query sequence*/
   struct alnSet *alnSetSTPtr,/*scoring settings*/
   signed long *maxScoreSLPtr,/*gets max possible score*/
   signed char *errSCPtr      /*errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun33 TOC:
   '   - scores the aligment in samEntry and finds best
   '     sub-aligment
   '   o fun33 sec01:
   '     - variable declartions
   '   o fun33 sec02:
   '     - allocate memory for assembler score (if need)
   '   o fun33 sec03:
   '     - find score and best sub-score
   '   o fun33 sec04:
   '     - convert cigar to sub-alingment cigar; if subaln
   '   o fun33 sec05:
   '     - get extra entries and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun33 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *refStr = refSTPtr->seqStr;
   signed char *qryStr = qrySTPtr->seqStr;

   signed char *swapSCPtr = 0;
   unsigned int uiCig = 0;
   unsigned int uiPos = 0;
   signed long scoreSL = 0;

   signed long subScoreSL = 0;
   unsigned int subStartUI = 0;
   signed long maxSubScoreSL = 0;

   signed long bestScoreSL = 0;
   unsigned int bestStartUI = 0;
   unsigned int bestEndUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun33 Sec02:
   ^   - allocate memory for assembler score (if need)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
      samSTPtr->lenExtraUI >= samSTPtr->lenExtraBuffUI -32
   ){ /*If: need more memory*/
      /*32 is more then enough for two numeric entries*/

      swapSCPtr =
         realloc(
            samSTPtr->extraStr,
              (samSTPtr->lenExtraUI + 41)
            * sizeof(signed char)
         );
      if(! swapSCPtr)
         goto memErr_fun33_sec05;
      samSTPtr->extraStr = swapSCPtr;

      samSTPtr->lenExtraBuffUI += 32;
   }  /*If: need more memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun33 Sec03:
   ^   - find score and best sub-score
   ^   o fun33 sec03 sub01:
   ^     - get to first aligned reference base
   ^   o fun33 sec03 sub02:
   ^     - score matches/snps + start switch + start loop
   ^   o fun33 sec03 sub03:
   ^     - score insertions
   ^   o fun33 sec03 sub04:
   ^     - score deletions
   ^   o fun33 sec03 sub05:
   ^     - masking, move to next query entry
   ^   o fun33 sec03 sub06:
   ^     - add final sub-score to total score
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun33 Sec03 Sub01:
   *   - get to first aligned reference base
   \*****************************************************/

   refStr += samSTPtr->refStartUI;
   *maxScoreSLPtr = 0;

   /*****************************************************\
   * Fun33 Sec03 Sub02:
   *   - score matches/snps + start switch + start loop
   \*****************************************************/

   for(
      uiCig = 0;
      uiCig < samSTPtr->lenCigUI;
      ++uiCig
   ){ /*Loop: score cigar*/

      switch(samSTPtr->cigTypeStr[uiCig])
      { /*Switch: find cigar type*/

         case '=':
         case 'X':
         case 'M':
         /*Case: match or snp*/
            for(
               uiPos = 0;
               uiPos <
                 (unsigned int) samSTPtr->cigArySI[uiCig];
               ++uiPos
            ){ /*Loop: find score*/
               subScoreSL +=
                  getScore_alnSet(
                     *qryStr,
                     *refStr,
                     alnSetSTPtr
                  );

               maxSubScoreSL +=
                  getScore_alnSet(
                     *qryStr,
                     *qryStr,
                     alnSetSTPtr
                  );

               ++refStr;
               ++qryStr;
            }  /*Loop: find score*/

            if(subScoreSL <= 0)
            { /*If: end of sub-alignment*/
               scoreSL += subScoreSL;
               *maxScoreSLPtr += maxSubScoreSL;

               subScoreSL = 0;
               maxSubScoreSL = 0;

               subStartUI = uiCig;
            } /*If: end of sub-alignment*/

            else if(subScoreSL > bestScoreSL)
            { /*Else If: new best score*/
               bestScoreSL = subScoreSL;
               bestStartUI = subStartUI;
               bestEndUI = uiCig;
            } /*Else If: new best score*/

            break;
         /*Case: match or snp*/

         /***********************************************\
         * Fun33 Sec03 Sub03:
         *   - score insertions
         \************************************************/

         case 'I':
         /*Case: insertion*/

            /*get score and subscore*/
            subScoreSL += alnSetSTPtr->gapSS;

            subScoreSL +=
               (
                    (samSTPtr->cigArySI[uiCig] - 1)
                  * alnSetSTPtr->extendSS
               );

            /*get the maximum possible score*/
            for(
               uiPos = 0;
               uiPos <
                 (unsigned int) samSTPtr->cigArySI[uiCig];
               ++uiPos
            ){ /*Loop: find maximum possible score*/
               maxSubScoreSL +=
                  getScore_alnSet(
                     *qryStr,
                     *qryStr,
                     alnSetSTPtr
                  );

               ++qryStr;
            }  /*Loop: find maximum possible score*/


            /*check if end of sub-alignment*/
            if(subScoreSL <= 0)
            { /*If: end of sub-alignment*/
               scoreSL += subScoreSL;
               *maxScoreSLPtr += maxSubScoreSL;

               subScoreSL = 0;
               maxSubScoreSL = 0;

               subStartUI = uiCig;
            } /*If: end of sub-alignment*/


            break;
         /*Case: insertion*/

         /***********************************************\
         * Fun33 Sec03 Sub04:
         *   - score deletions
         \************************************************/

         case 'D':
         /*Case: deletion*/
            refStr += samSTPtr->cigArySI[uiCig];
            subScoreSL += alnSetSTPtr->gapSS;

            subScoreSL +=
               (
                    (samSTPtr->cigArySI[uiCig] - 1)
                  * alnSetSTPtr->extendSS
               );

            if(subScoreSL <= 0)
            { /*If: end of sub-alignment*/
               scoreSL += subScoreSL;
               *maxScoreSLPtr += maxSubScoreSL;

               subScoreSL = 0;
               maxSubScoreSL = 0;

               subStartUI = uiCig;
            } /*If: end of sub-alignment*/

            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun33 Sec03 Sub05:
         *   - masking, move to next query entry
         \************************************************/

         default:
            qryStr += samSTPtr->cigArySI[uiCig];
      } /*Switch: find cigar type*/

   } /*Loop: score cigar*/

   /*****************************************************\
   * Fun33 Sec03 Sub06:
   *   - add final sub-score to total score
   \*****************************************************/

   scoreSL += subScoreSL;
   *maxScoreSLPtr += maxSubScoreSL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun33 Sec04:
   ^   - convert cigar to sub-alingment cigar (if subaln)
   ^   o fun33 sec04 sub01:
   ^     - check if have subalinment
   ^   o fun33 sec04 sub02:
   ^     - handel adjusting start for sub-alignment
   ^   o fun33 sec04 sub03:
   ^     - adjusting cigar end for sub-alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun33 Sec04 Sub01:
   *   - check if have subalinment
   \*****************************************************/

   if(samSTPtr->cigTypeStr[bestEndUI + 1] == 'S')
      ++bestEndUI;
      /*at aligment end*/

   if(! subBl)
      goto ret_fun33_sec05;
   else if(bestStartUI == bestEndUI)
      goto noScore_fun33_sec05;
   else if(bestStartUI >= samSTPtr->lenCigUI)
      goto noScore_fun33_sec05;

   /*****************************************************\
   * Fun33 Sec04 Sub02:
   *   - handel adjusting start for sub-alignment
   *   o fun33 sec04 sub02 cat01:
   *     - start removal; convert first cigar entry to S
   *   o fun33 sec04 sub02 cat02:
   *     - start removal; remove targed cigar entries
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun33 Sec04 Sub02 Cat01:
   +   - start removal; convert first cigar entry to S
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(bestStartUI)
   { /*If: need to shift cigar*/
      if(
            samSTPtr->cigTypeStr[0] != 'S'
         && samSTPtr->cigTypeStr[0] != 'H'
      ){ /*If: start not masked*/
         samSTPtr->numMaskUI += samSTPtr->cigArySI[0];

         switch(samSTPtr->cigTypeStr[0])
         { /*Switch: find cigar type*/

            case 'M':
            case '=':
            /*Case: match or match/snp*/
               samSTPtr->numMatchUI -=
                 samSTPtr->cigArySI[0];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[0];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[0];
               break;
            /*Case: match or match/snp*/

            case 'X':
            /*Case: snp*/
               samSTPtr->numSnpUI -=
                 samSTPtr->cigArySI[0];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[0];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[0];
               break;
            /*Case: snp*/

            case 'D':
            /*Case: deletion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[0];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[0];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[0];
               break;
            /*Case: deletion*/

            case 'I':
            /*Case: insertion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[0];
               break;
            /*Case: insertion*/
         } /*Switch: find cigar type*/

      } /*If: start not masked*/

      samSTPtr->cigTypeStr[0] = 'S';
      uiCig = 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun33 Sec04 Sub02 Cat02:
      +   - start removal; remove targed cigar entries
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
     
      while(bestStartUI < samSTPtr->lenCigUI)
      { /*Loop: shift cigar*/
         if(uiCig < bestStartUI)
         { /*If: masking*/
            samSTPtr->cigArySI[0] +=
               samSTPtr->cigArySI[uiCig];
            samSTPtr->numMaskUI +=
               samSTPtr->cigArySI[uiCig];
         } /*If: masking*/

         switch(samSTPtr->cigTypeStr[uiCig])
         { /*Switch: find cigar type*/

            case 'M':
            case '=':
            /*Case: match or match/snp*/
               samSTPtr->numMatchUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[uiCig];
               break;
            /*Case: match or match/snp*/

            case 'X':
            /*Case: snp*/
               samSTPtr->numSnpUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[uiCig];
               break;
            /*Case: snp*/

            case 'D':
            /*Case: deletion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refStartUI +=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                 samSTPtr->cigArySI[uiCig];
               break;
            /*Case: deletion*/

            case 'I':
            /*Case: insertion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[uiCig];
               break;
            /*Case: insertion*/
         } /*Switch: find cigar type*/

         samSTPtr->cigTypeStr[uiCig] =
            samSTPtr->cigTypeStr[bestStartUI];
         samSTPtr->cigArySI[uiCig] =
            samSTPtr->cigArySI[bestStartUI];

         ++uiCig;
         ++bestStartUI;
         --bestEndUI; /*account for shift in length*/
      }  /*Loop: shift cigar*/

      samSTPtr->lenCigUI = uiCig;
      samSTPtr->cigTypeStr[uiCig] = '\0';
   } /*If: need to shift cigar*/

   /*****************************************************\
   * Fun33 Sec04 Sub03:
   *   - adjusting cigar end for sub-alignment
   *   o fun33 sec04 sub03 cat01:
   *     - end removal; convert 1st cigar removal to S
   *   o fun33 sec04 sub03 cat01:
   *     - end removal; adjust mutation counts
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun33 Sec04 Sub03 Cat01:
   +   - end removal; convert 1st cigar removal to S
   \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(bestEndUI < (samSTPtr->lenCigUI - 1) )
   { /*If: removing ending cigar entries*/
      ++bestEndUI;
      samSTPtr->numMaskUI+= samSTPtr->cigArySI[bestEndUI];

      switch(samSTPtr->cigTypeStr[bestEndUI])
      { /*Switch: find cigar type*/

         case 'M':
         case '=':
         /*Case: match or match/snp*/
            samSTPtr->numMatchUI -=
              samSTPtr->cigArySI[bestEndUI];
             samSTPtr->refEndUI -=
                samSTPtr->cigArySI[bestEndUI];
             samSTPtr->alnReadLenUI -=
                samSTPtr->cigArySI[bestEndUI];
            break;
         /*Case: match or match/snp*/

         case 'X':
         /*Case: snp*/
            samSTPtr->numSnpUI -=
              samSTPtr->cigArySI[bestEndUI];
            samSTPtr->refEndUI -=
               samSTPtr->cigArySI[bestEndUI];
            samSTPtr->alnReadLenUI -=
               samSTPtr->cigArySI[bestEndUI];
            break;
         /*Case: snp*/

         case 'D':
         /*Case: deletion*/
            samSTPtr->numDelUI -=
              samSTPtr->cigArySI[bestEndUI];
            samSTPtr->refEndUI -=
               samSTPtr->cigArySI[bestEndUI];
            samSTPtr->alnReadLenUI -=
               samSTPtr->cigArySI[bestEndUI];
            break;
         /*Case: deletion*/

         case 'I':
         /*Case: insertion*/
            samSTPtr->numDelUI -=
              samSTPtr->cigArySI[bestEndUI];
            break;
         /*Case: insertion*/
      } /*Switch: find cigar type*/

      samSTPtr->cigTypeStr[bestEndUI] = 'S';

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun33 Sec04 Sub03 Cat02:
      +   - end removal; adjust mutation counts
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      uiCig = bestEndUI + 1;
     
      while(uiCig < samSTPtr->lenCigUI)
      { /*Loop: remove tailing entries*/

         switch(samSTPtr->cigTypeStr[uiCig])
         { /*Switch: find cigar type*/

            case 'M':
            case '=':
            /*Case: match or match/snp*/
               samSTPtr->numMatchUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refEndUI -=
                  samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                  samSTPtr->cigArySI[uiCig];
               break;
            /*Case: match or match/snp*/

            case 'X':
            /*Case: snp*/
               samSTPtr->numSnpUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refEndUI -=
                  samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                  samSTPtr->cigArySI[uiCig];
               break;
            /*Case: snp*/

            case 'D':
            /*Case: deletion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[uiCig];
               samSTPtr->refEndUI -=
                  samSTPtr->cigArySI[uiCig];
               samSTPtr->alnReadLenUI -=
                  samSTPtr->cigArySI[uiCig];
               break;
            /*Case: deletion*/

            case 'I':
            /*Case: insertion*/
               samSTPtr->numDelUI -=
                 samSTPtr->cigArySI[uiCig];
               break;
            /*Case: insertion*/
         } /*Switch: find cigar type*/

         samSTPtr->cigArySI[bestEndUI] +=
               samSTPtr->cigArySI[uiCig];
         ++uiCig;

      } /*Loop: remove tailing entries*/

      ++bestEndUI;
      samSTPtr->lenCigUI = bestEndUI;
      samSTPtr->cigTypeStr[bestEndUI] = '\0';
   } /*If: removing ending cigar entries*/

   /*****************************************************\
   * Fun33 Sec04 Sub04:
   *   - adjusting alignment lengths and coordiantes
   \*****************************************************/

   samSTPtr->alnReadLenUI = samSTPtr->numMatchUI;
   samSTPtr->alnReadLenUI += samSTPtr->numSnpUI;
   samSTPtr->alnReadLenUI += samSTPtr->numDelUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun33 Sec05:
   ^   - get extra entries and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ret_fun33_sec05:;
      *errSCPtr = 0;

      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'N';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'M';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = ':';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'i';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = ':';

      samSTPtr->lenExtraUI +=
         numToStr(
            &samSTPtr->extraStr[samSTPtr->lenExtraUI],
            (unsigned long)
            (
                samSTPtr->numSnpUI
              + samSTPtr->numDelUI
              + samSTPtr->numInsUI
            )
         );

      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = '\t';

      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'A';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'S';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = ':';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = 'i';
      samSTPtr->extraStr[samSTPtr->lenExtraUI++] = ':';

      bestScoreSL = scoreSL; /*so can  update AS:i score*/

      if(scoreSL < 0)
      { /*If: had negative score*/
         samSTPtr->extraStr[samSTPtr->lenExtraUI++] = '-';
         bestScoreSL *= -1;
      } /*If: had negative score*/
      
      samSTPtr->lenExtraUI +=
         numToStr(
            &samSTPtr->extraStr[samSTPtr->lenExtraUI],
            (unsigned long) bestScoreSL
         );

      return scoreSL;

   noScore_fun33_sec05:;
      *errSCPtr = def_noScore_mapRead;
      return scoreSL;

   memErr_fun33_sec05:;
      *errSCPtr = def_memErr_mapRead;
      return scoreSL;
} /*scoreSubAln_mapRead*/

/*-------------------------------------------------------\
| Fun34: aln_mapRead
|   - maps read to reference
| Input:
|   - samSTPtr:
|     o samEntry struct pointer to hold alignment
|   - qrySTPtr:
|     o seqST struct pointer with query sequence and id
|   - refSTPtr:
|     o ref_mapRead struct pointer with reference
|       sequence, settings, and kmer arrays for alignment
|     o setup with init_ref_mapRead, setup_ref_mapRead,
|       and addRef_ref_mapRead
|   - mapIndexSIPtr:
|     o signed int pointer to hold index of best chain
|       (used to make mapping)
|   - alnSTPtr:
|     o aln_mapRead struct pionter with allocated memory to
|       use/allocate/resize in mapping steps
|   - setSTPtr:
|     o set_mapRead structure with non-reference specific
|       alignment settings
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o samSTPtr to have alignment (everything modified)
|       - blanked if *errSCPtr != 0
|     o seqStr in qrySTPtr to be lookup index's, you can
|       undo this with indexToSeq_alnSet(qrySTPtr->seqStr)
|     o mapIndexSLPtr to have index of chain used to make
|       alignment
|     o kmerArySI in alnSTPtr to have query kmer sequence
|     o lenSI in alnSTPtr to be length of kmerArySI
|     o sizeSI in alnSTPtr if kmerArySI is resized
|     o chainsSTPtr alnSTPtr to have found chains
|     o errSCPtr to have error message
|       * 0 for no errors
|       * def_memErr_mapRead for no errors
|       * def_noChains_mapRead if chould not find any
|         chains
|       * def_noAln_mapRead if aligment was under
|         minimum percent query length or score
|     o matrixSTPtr in alnSTPtr to nothing usefull. Here
|       to avoid memory allocations in alignment step
|   - Returns:
|     o score for alignment
\-------------------------------------------------------*/
signed long
align_mapRead(
   struct samEntry *samSTPtr,   /*gets aligment*/
   struct seqST *qrySTPtr,      /*has query sequence*/
   struct ref_mapRead *refSTPtr,/*ref kmers/settings*/
   signed long *mapIndexSIPtr,  /*index of best chain*/
   struct aln_mapRead *alnSTPtr,/*memory for query*/
   struct set_mapRead *setSTPtr,/*alignment settings*/
   signed char *errSCPtr        /*gets errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun34 TOC:
   '   - maps a read to a reference
   '   o fun29 sec01:
   '     - variable declarations
   '   o fun34 sec02:
   '     - setup and get query kmer sequence
   '   o fun34 sec03:
   '     - map query to reference
   '   o fun34 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;

   signed long scoreSL = 0;
   signed long maxPossibleScoreSL = 0;

   float percF = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec02:
   ^   - setup and get query kmer sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   blank_samEntry(samSTPtr);

   seqToIndex_alnSet(qrySTPtr->seqStr);

   alnSTPtr->lenSI =
      seqToKmer_kmerFun(
         qrySTPtr->seqStr,
         qrySTPtr->lenSeqUL,
         &alnSTPtr->kmerArySI,
         &alnSTPtr->sizeSI,
         refSTPtr->lenKmerUC
      );

   if(! alnSTPtr->lenSI)
      goto memErr_fun34_sec04;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec03:
   ^   - map query to reference
   ^   o fun34 sec03 sub01:
   ^     - get all possible chains
   ^   o fun34 sec03 sub02:
   ^     - fill in merged chain gaps (best chain)
   ^   o fun34 sec03 sub03:
   ^     - adjust coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /**************************************************\
   * Fun34 Sec03 Sub01:
   *   - get all possible chains
   \**************************************************/

   if(
      kmerChain_mapRead(
         alnSTPtr->chainsSTPtr,
         refSTPtr->minChainLenSI,
         alnSTPtr->kmerArySI,
         alnSTPtr->lenSI,
         refSTPtr->kmerArySI,
         refSTPtr->indexArySI,
         refSTPtr->lenSI,
         0
      )
   ) goto memErr_fun34_sec04;

   *mapIndexSIPtr =
       mergeChains_mapRead(alnSTPtr->chainsSTPtr);
       /*merges chains to get an alignment guide*/

   if(scoreSL == def_noChains_mapRead)
      goto noChains_fun34_sec04;

   /**************************************************\
   * Fun34 Sec03 Sub02:
   *   - fill in merged chain gaps (best chain)
   \**************************************************/
   
   if(
      mergeToSam_mapRead(
         alnSTPtr->chainsSTPtr,
         (signed int) *mapIndexSIPtr,
         qrySTPtr,
         refSTPtr->seqSTPtr,
         refSTPtr->lenKmerUC,
         refSTPtr->minChainLenSI,
         setSTPtr->alnSetST,
         alnSTPtr->matrixSTPtr,
         samSTPtr
      ) /*make merged chains into an alignment*/
   ) goto memErr_fun34_sec04;

   /**************************************************\
   * Fun34 Sec03 Sub03:
   *   - adjust coordinates
   \**************************************************/

   scoreSL =
      scoreSub_mapRead(
         samSTPtr,
         setSTPtr->subBl,  /*if doing sub-alignment*/
         qrySTPtr,
         refSTPtr->seqSTPtr,
         setSTPtr->alnSetST,
         &maxPossibleScoreSL,
         &errSC
      );

   /*check if had the minimum percent score*/
   percF = (float) scoreSL;
   percF /= (float) maxPossibleScoreSL;

   if(percF < setSTPtr->minScoreF)
      goto noAln_fun34_sec04;

   /*check if had the minimum percent length*/
   percF = (float) samSTPtr->numMatchUI;
   percF += (float) samSTPtr->numSnpUI;
   percF += (float) samSTPtr->numInsUI;
   percF /= (float) qrySTPtr->lenSeqUL;

   if(percF < setSTPtr->minPercLenF)
      goto noAln_fun34_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun34 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun34_sec04;

   memErr_fun34_sec04:;
      *errSCPtr = def_memErr_mapRead;
      goto err_fun34_sec04;

   noAln_fun34_sec04:;
      *errSCPtr = def_noAln_mapRead;
      goto err_fun34_sec04;

   noChains_fun34_sec04:;
      *errSCPtr = def_noChains_mapRead;
      goto err_fun34_sec04;

   err_fun34_sec04:;
      blank_samEntry(samSTPtr);
      goto ret_fun34_sec04;

   ret_fun34_sec04:;
      return scoreSL;
} /*aln_mapRead*/

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
