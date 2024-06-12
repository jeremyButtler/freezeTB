/*########################################################
# Name: kmerFind
#   - holds functions for kmer detection of spoligotypes
# Basic usage:
#   - fun11 to set up sequences searching for
#   - fun18 to to find kmers
#     o declare and initialize an tblST_kmerFind
#       - fun02 (for initialize)
#     o decleare and initialize and alnSet structure
#       - ../memwater/alnSetStruct.h
#     o an set of sequences to find (fun11)
#     o an samEntry struct with the sequence
# Basic free at end:
#     o tblST_kmerFind structure (fun03 stack; fun04 heap)
#     o refST_kmerFind structure (heap; fun09)
#     o alnSet structure
#       (fun03/fun04 ../memwater/alnSetStruct.h)
#     o samEntry structure
#       (fun03/fun04 ../generalLib/samEntryStruct.h)
#     o possibily codeStr (if you allocated memory)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o tbl01: alnNtToBit_kmerFind
'     - converts an nucleotide alignment code form
'       alnSetStruct.h to an two bit value, with an 3rd
'       bit being used for anonymous bases and the 4th bit
'       for errors
'   o .h st01: tblST_kmerFind
'     - holds the kmer tables for detecting spoligytpes
'   o .h st02: refST_kmerFind
'     - holds the kmer pattern for the reference
'   o fun01: blank_tblST_kmerFind
'     - blanks all stored values in an tblST_kmerFind
'   o fun02: init_tblST_kmerFind
'     - initializes an tblST_kmerFind structure
'   o fun03: freeStack_tblST_kmerFind
'     - frees all variables in an tblST_kmerFind structure
'   o fun04: freeHeap_tblST_kmerFind
'     - frees and tblST_kmerFind structure
'   o fun05: blank_refST_kmerFind
'     - sets the counting and kmer values in an
'       refST_kmerFind to 0 or def_noKmer_kmerFind
'   o fun06: init_refST_kmerFind
'     - initializes an refST_kmerFind structure
'   o fun07: freeStack_refST_kmerFind
'     - frees the variables in an refST_kmerFind structure
'   o fun08: freeHeap_refST_kmerFind
'     - frees an refST_kmerFind structure
'   o fun09: freeHeapAry_refST_kmerFind
'     - frees an array of refST_kmerFind structure
'   o .c fun10: addNtToKmer_kmerFind
'     - adds an nucleotide to an kmer
'   o fun11: mkAry_refST_kmerFind
'     - makes an array of refST_kmerFind structures
'   o fun13: nextSeqChunk_tblST_kmerFind
'     - adds a new set of kmers from an sequence to an
'       tblST_kmerFind structure
'   o .h fun14: forCntMatchs_kmerFind
'     - finds the number of kmers that are in both the
'       kmer table (query) and the pattern (reference)
'   o .h fun15: revCntMatchs_kmerFind
'     - finds the number of kmers that are shared in the
'       kmer table (query) and the reverse pattern
'       (reference)
'   o fun16: matchCheck_kmerFind
'     - tells if the  match meets the min requirements to
'       do an alignment or not
'   o fun17: findRefInChunk_kmerFind
'     - does an kmer check and alings an single sequence
'       in an refST_kmerFind structure to see if there is
'       an match
'   o fun18: samFindSpoligos_kmerFind
'     - finds spoligotype spacers in an sequence using
'       an faster kmer search followed by an slower
'       waterman to finalize alignments
'   o fun19: fxFindSpoligos_kmerFind
'     - finds spoligotype spacers in an sequence (from
'       fastx file) using an faster kmer search followed
'       by an slower waterman to finalize alignments
'   o license:
'     - Licensing for this code (public domain / mit)
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

#include "kmerFind.h"

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"

#include "tbSpoligo-memwater-fun.h"
#include "../memwater/alnSetStruct.h"
#include "../memwater/seqStruct.h"
#include "../memwater/memwater.h"

/*.h files only (no .c files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/genMath.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/gen-shellSort.h"
#include "tbSpoligo-errors.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .c #include "../memwater/memwater.h"
!   - .c #include "../generalLib/samEntryStruct.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_a_kmerFind 0
#define def_c_kmerFind 1
#define def_g_kmerFind 2
#define def_t_kmerFind 3
#define def_anonNt_kmerFind 4 /*anonmyous nucleotide*/
#define def_noNt_kmerFind 8

/*-------------------------------------------------------\
| Tbl01: alnNtToBit_kmerFind
|   - converts an nucleotide alignment code form
|     alnSetStruct.h to an two bit value, with an 3rd
|     bit being used for anonymous bases and the 4th bit
|     for errors
\-------------------------------------------------------*/
unsigned char alnNtToBit_kmerFind[] =
{
   def_noNt_kmerFind,    /*00 = 64 = ,*/  
   def_a_kmerFind,     /*01 = 65 = A/a*/  
   def_anonNt_kmerFind,  /*02 = 66 = B/b*/  
   def_c_kmerFind,       /*03 = 67 = C/c*/  
   def_anonNt_kmerFind,  /*04 = 68 = D/d*/  
   def_noNt_kmerFind,    /*05 = 69 = E/e*/  
   def_noNt_kmerFind,    /*06 = 70 = F/f*/  
   def_g_kmerFind,       /*07 = 71 = G/g*/  
   def_anonNt_kmerFind,  /*08 = 72 = H/h*/  
   def_noNt_kmerFind,    /*09 = 73 = I/i*/  
   def_noNt_kmerFind,    /*10 = 74 = J/j*/  
   def_anonNt_kmerFind,  /*11 = 75 = K/k*/  
   def_noNt_kmerFind,    /*12 = 76 = L/l*/  
   def_anonNt_kmerFind,  /*13 = 77 = M/m*/  
   def_anonNt_kmerFind,  /*14 = 78 = N/n*/  
   def_noNt_kmerFind,    /*15 = 79 = O/o*/  
   def_noNt_kmerFind,    /*16 = 80 = P/p*/  
   def_noNt_kmerFind,    /*17 = 81 = Q/q*/  
   def_anonNt_kmerFind,  /*18 = 82 = R/r*/  
   def_anonNt_kmerFind,  /*19 = 83 = S/s*/  
   def_t_kmerFind,       /*20 = 84 = T/t*/  
   def_t_kmerFind,       /*21 = 85 = U/u*/  
   def_anonNt_kmerFind,  /*22 = 86 = V/v*/  
   def_anonNt_kmerFind,  /*23 = 87 = W/w*/  
   def_anonNt_kmerFind,  /*24 = 88 = X/x (amino acids)*/  
   def_anonNt_kmerFind,  /*25 = 89 = Y/y*/  
   def_noNt_kmerFind     /*26 = 90 = Z/z*/  
}; /*ntAlnCodeToBitTbl*/

/*-------------------------------------------------------\
| Fun01: blank_tblST_kmerFind
|   - blanks all stored values in an tblST_kmerFind
|     structure
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to blank
|   - blankSeqBl:
|     o 1: blank the seqStruct (sequence) in tblSTPtr
|     o 0: do not blank the seqStruct in tblSTPtr
| Output:
|   - Modifies:
|     o tblSI in tblSTPtr to be full of zeros
|     o kmerArySI to be full of -1's and to end in -2
|     o lenLastKmer to be 0
|     o seqPosUL to be 0
|     o if requested seqST with blaknSeqST from
|       ../memwater/seqStruct.h
\-------------------------------------------------------*/
void
blank_tblST_kmerFind(
   struct tblST_kmerFind *tblSTPtr,
   signed char blankSeqBl
){
   unsigned int uiKmer = 0;

   if(tblSTPtr->tblSI)
   { /*If: I have an kmer table to blank*/
      for(
         uiKmer = 0;
         uiKmer < tblSTPtr->lenTblUI;
         ++uiKmer
      ) tblSTPtr->tblSI[uiKmer] = 0;
   } /*If: I have an kmer table to blank*/

   if(tblSTPtr->kmerArySI)
   { /*If: I have an kmer array to blank*/
      for(
         uiKmer = 0;
         uiKmer < tblSTPtr->numKmerUI;
         ++uiKmer
      ) tblSTPtr->kmerArySI[uiKmer] = def_noKmer_kmerFind;

      tblSTPtr->kmerArySI[uiKmer] = def_endKmers_kmerFind;
   } /*If: I have an kmer array to blank*/

   tblSTPtr->lenLastKmerUL = 0;
   tblSTPtr->seqPosUL = 0;

   if(blankSeqBl && tblSTPtr->seqST)
   { /*If: I am blanking the stored seqeunce*/
      blankSeqST(tblSTPtr->seqST);
   } /*If: I am blanking the stored seqeunce*/
} /*blank_tblST_kmerFind*/

/*-------------------------------------------------------\
| Fun02: init_tblST_kmerFind
|   - initializes an tblST_kmerFind structure
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to blank
|   - lenKmerUC:
|     o length of one kmer
| Output:
|   - Modifies:
|     o all varaibles in an tblST_kmerFind find structure
|       to be 0 or for the table to have memory allocated
|   - Returns:
|     o 0 for no errors
|     o memErr_tbSpoligo for an memory error
\-------------------------------------------------------*/
unsigned char
init_tblST_kmerFind(
   struct tblST_kmerFind *tblSTPtr,
   unsigned char lenKmerUC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - initialize an tblST_kmerFind structure
   '   o fun02 sec01:
   '     - allocate memory for the table
   '   o fun02 sec02:
   '     - blank arrays and other values
   '   o fun02 sec03:
   '     - build the kmer mask
   '   o fun02 sec04:
   '     - initialize seqStruct and blank table
   '   o fun02 sec05:
   '     - return any errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - allocate memory for the table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiKmer = 0;

   tblSTPtr->seqST = 0; /*just in case have memory error*/

   tblSTPtr->lenKmerUC = lenKmerUC;

   tblSTPtr->lenTblUI = 1;

   for(
      uiKmer = 0;
      uiKmer < lenKmerUC;
      ++uiKmer
   ) tblSTPtr->lenTblUI <<= 2; /*multiply by 4*/

   tblSTPtr->tblSI = 0;
   tblSTPtr->tblSI =
      malloc((tblSTPtr->lenTblUI + 1) * sizeof(sint));

   if(! tblSTPtr->tblSI)
      goto memErr_fun02_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - blank arrays and other values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tblSTPtr->kmerArySI = 0;
   tblSTPtr->numKmerUI = 0;
   tblSTPtr->ntInWinUI = 0;
   tblSTPtr->rmNtUI = 0;
   tblSTPtr->seqPosUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - build the kmer mask
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tblSTPtr->kmerMaskUL =
      (lenKmerUC * def_bitsPerKmer_kmerFind);

   tblSTPtr->kmerMaskUL =
      (sizeof(ulong) << 3) - tblSTPtr->kmerMaskUL;

   tblSTPtr->kmerMaskUL =
      ((ulong) -1) >> tblSTPtr->kmerMaskUL;

   /*logic
   `   - mask = lenKmerUC << bitsPerKmer:
   `     o gives me the number of bits an single kmer
   `       takes up
   `   - mask = (sizeof(ulong) << 3) - mask
   `     o sizeof(ulong) << 3 gives the number of bits
   `       in an unsigned long
   `     o bits in long - mask gives the number of bits
   `       that need to be masked (not in kmer)
   `   -  mask = -1 >> mask
   `      o removes all the leading ones that should be
   `        masked out (111...11111 to 000...011111)
   */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - initialize seqStruct and blank table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tblSTPtr->seqST = 0;
   tblSTPtr->seqST = malloc(sizeof(struct seqStruct));

   if(! tblSTPtr->seqST)
      goto memErr_fun02_sec06;

   initSeqST(tblSTPtr->seqST);

   blank_tblST_kmerFind(
      tblSTPtr,
      0 /*sequence structure is already blanked*/
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - return any errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun02_sec06:;

   freeStack_tblST_kmerFind(tblSTPtr);

   return memErr_tbSpoligo;
} /*init_tblST_kmerFind*/

/*-------------------------------------------------------\
| Fun03: freeStack_tblST_kmerFind
|   - frees all variables in an tblST_kmerFind structure
|     and sets other values to defaults (calls blank)
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to free
|       variables in
| Output:
|   - Frees:
|     o all allocated varialbes in tblStPtr
\-------------------------------------------------------*/
void
freeStack_tblST_kmerFind(
   struct tblST_kmerFind *tblSTPtr
){
   if(tblSTPtr)
   { /*If: I have an struct to free*/
      free(tblSTPtr->kmerArySI);
      tblSTPtr->kmerArySI = 0;

      freeSeqST(tblSTPtr->seqST); /*checks if have null*/
      tblSTPtr->seqST = 0;

      free(tblSTPtr->tblSI);
      tblSTPtr->tblSI = 0;

      blank_tblST_kmerFind(
         tblSTPtr,
         0 /*no seqST to blank*/
      );
   } /*If: I have an struct to free*/
} /*freeStack_tblST_kmerFind*/

/*-------------------------------------------------------\
| Fun04: freeHeap_tblST_kmerFind
|   - frees and tblST_kmerFind structure
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to free
| Output:
|   - Frees:
|     o tblSTPtr (does not set to null)
\-------------------------------------------------------*/
void
freeHeap_tblST_kmerFind(
   struct tblST_kmerFind *tblSTPtr
){
   freeStack_tblST_kmerFind(tblSTPtr);
   free(tblSTPtr);
} /*freeHeap_tblST_kmerFind*/

/*-------------------------------------------------------\
| Fun05: blank_refST_kmerFind
|   - sets the counting and kmer values in an
|     refST_kmerFind to 0 or def_noKmer_kmerFind
| Input:
|   - refSTPtr:
|     o pointer to an refST_kmerFind structure to blank
|       variables in
| Output:
|   - Modifies:
|     o forKmerArySI to be full of -1's
|     o forRefAryUS to be full of 0's
|     o revKmerArySI to be full of -1's
|     o revRefAryUS to be full of 0's
|     o forSeqST to be blanked
|     o revSeqST to be blanked
\-------------------------------------------------------*/
void
blank_refST_kmerFind(
   struct refST_kmerFind *refSTPtr
){
   ulong ulKmer = 0;

   if(refSTPtr->forKmerArySI)
   { /*If: I have kmer arrays allocated*/
      for(
         ulKmer = 0;
         ulKmer < refSTPtr->lenAryUI;
         ++ulKmer
      ){ /*Loop: blank kmer arrays*/
         refSTPtr->forKmerArySI[ulKmer] =
            def_noKmer_kmerFind;

         refSTPtr->forRepAryUI[ulKmer] = 0;

         refSTPtr->revKmerArySI[ulKmer] =
            def_noKmer_kmerFind;

         refSTPtr->revRepAryUI[ulKmer] = 0;
      } /*Loop: blank kmer arrays*/

      refSTPtr->forKmerArySI[ulKmer] =
         def_endKmers_kmerFind;

      refSTPtr->revKmerArySI[ulKmer] =
         def_endKmers_kmerFind;
   } /*If: I have kmer arrays allocated*/

   if(refSTPtr->forSeqST)
      blankSeqST(refSTPtr->forSeqST);

   if(refSTPtr->revSeqST)
      blankSeqST(refSTPtr->revSeqST);
} /*blank_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun06: init_refST_kmerFind
|   - initializes an refST_kmerFind structure
| Input:
|   - refSTPtr:
|     o pointer to refST_kmerFind structure to initialize
|   - lenKmerUC:
|     o length of one kmer
| Output:
|   - Modifies:
|     o refSTPtr to be initialized
|   - Returns:
|     o 0 for success
|     o memErr_tbSpoligo for memory error
\-------------------------------------------------------*/
signed char
init_refST_kmerFind(
   struct refST_kmerFind *refSTPtr,
   unsigned char lenKmerUC
){
   refSTPtr->lenKmerUC = lenKmerUC;
   refSTPtr->minKmersUI = 0;
   refSTPtr->maxForScoreF = 0;
   refSTPtr->maxRevScoreF = 0;
   refSTPtr->lenAryUI = 0;

   refSTPtr->forKmerArySI = 0;
   refSTPtr->forRepAryUI = 0;

   refSTPtr->revKmerArySI = 0;
   refSTPtr->revRepAryUI = 0;

   refSTPtr->forSeqST = 0;
   refSTPtr->revSeqST = 0;

   refSTPtr->forSeqST = malloc(sizeof(struct seqStruct));

   if(! refSTPtr->forSeqST)
      goto memErr_fun06;

   initSeqST(refSTPtr->forSeqST);

   refSTPtr->revSeqST = malloc(sizeof(struct seqStruct));

   if(! refSTPtr->revSeqST)
      goto memErr_fun06;

   initSeqST(refSTPtr->revSeqST);

   /*not needed, but for future additions*/
   blank_refST_kmerFind(refSTPtr);

   return 0;

   memErr_fun06:;

   freeStack_refST_kmerFind(refSTPtr);
   return memErr_tbSpoligo;
} /*init_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun07: freeStack_refST_kmerFind
|   - frees the variables in an refST_kmerFind structure
| Input:
|   - refSTPtr:
|     o pointer to refST_kmerFind structure to free
|       varialbes in
| Output:
|   - Frees:
|     o all pointers in refSTPtr
|   - Sets
|     o pointers in refSTPtr to 0
\-------------------------------------------------------*/
void
freeStack_refST_kmerFind(
   struct refST_kmerFind *refSTPtr
){
   if(refSTPtr)
   { /*If: I have an structure to free*/
      free(refSTPtr->forKmerArySI);
      refSTPtr->forKmerArySI = 0;

      free(refSTPtr->forRepAryUI);
      refSTPtr->forRepAryUI = 0;

      free(refSTPtr->revKmerArySI);
      refSTPtr->revKmerArySI = 0;

      free(refSTPtr->revRepAryUI);
      refSTPtr->revRepAryUI = 0;

      freeSeqST(refSTPtr->forSeqST);
      refSTPtr->forSeqST = 0;

      freeSeqST(refSTPtr->revSeqST);
      refSTPtr->revSeqST = 0;
   } /*If: I have an structure to free*/

   /*not needed, but for future additions*/
   blank_refST_kmerFind(refSTPtr);
} /*freeStack_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun08: freeHeap_refST_kmerFind
|   - frees an refST_kmerFind structure
| Input:
|   - refSTPtr:
|     o pointer to refST_kmerFind structure to free
| Output:
|   - Frees:
|     o refSTPtr
\-------------------------------------------------------*/
void
freeHeap_refST_kmerFind(
   struct refST_kmerFind *refSTPtr
){
   freeStack_refST_kmerFind(refSTPtr);
   free(refSTPtr);
} /*freeStack_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun09: freeHeapAry_refST_kmerFind
|   - frees an array of refST_kmerFind structure
| Input:
|   - refSTAry:
|     o pointer to refST_kmerFind structure array to free
|   - lenArySI:
|     o number of refST_kmerFind structures in refSTPtr
| Output:
|   - Frees:
|     o all refST_kmerFind structures in refSTAry
|     o refSTAry (does not set to 0)
\-------------------------------------------------------*/
void
freeHeapAry_refST_kmerFind(
   struct refST_kmerFind *refSTAry,
   signed int lenArySI
){
   --lenArySI; /*convert index 1 to index 0*/

   while(lenArySI > -1)
   { /*Loop: free structres in array*/
      freeStack_refST_kmerFind(&refSTAry[lenArySI]);
      --lenArySI;
   } /*Loop: free structres in array*/

   free(refSTAry);
} /*freeHeapAry_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun10: addNtToKmer_kmerFind
|   - adds an nucleotide to an kmer
| Input:
|   - ntUC:
|     o character with nucleotide to add
|   - kmerUL:
|     o unsigned long with current kmer to add an
|       nucleotide to
|   - lenKmerUL:
|     o unsigned long with the number of nuceotdies since
|       the last anonymous nucleotide
|   - kmerMaskUL:
|     o unsigned long to mask out bits not used in the
|       kmer (each nucleotide takes two bits)
| Output:
|   - Modifies:
|     o kmerUL to
|       - have the new nucleotide and masked by kmerMaskUL
|       - be def_noKmer_kmerFind if ntUC is an anonymous
|         base (nucleotide)
|     o lenKmerUL
|       - be incurmented by 1 (+1)
|       - 0 if ntUC is an anonymous base (nucleotide)
\-------------------------------------------------------*/
#define \
addNtToKmer_kmerFind( \
   ntUC,     /*nucleotide to add*/ \
   kmerUL,   /*kmer to add nucleotide to*/ \
   lenKmerUL,/*number bases since last anonymous base*/ \
   kmerMaskUL/*bases to mask at end*/ \
){ \
   unsigned long ntMacUL = 0; \
   \
   ntMacUL = alnNtToBit_kmerFind[(unsigned char) (ntUC)];\
   (kmerUL) <<= def_bitsPerKmer_kmerFind; \
   (kmerUL) |= ntMacUL; \
   \
   if(ntMacUL < 5) \
   { /*If: I have no anonymous baseses*/ \
      (kmerUL) &= kmerMaskUL; \
      ++(lenKmerUL); \
   } /*If: I have no anonymous baseses*/ \
   \
   else \
   { /*Else: I have an anonymous base*/ \
      (kmerUL) = def_noKmer_kmerFind; \
      (lenKmerUL) = 0; \
   } /*Else: I have an anonymous base*/ \
} /*addNtToKmer_kmerFind*/

/*-------------------------------------------------------\
| Fun11: mkAry_refST_kmerFind
|   - makes an array of refST_kmerFind structures
| Input:
|   - faFileStr:
|     o c-string with path to fasta file with sequences
|       to build an refST_kmerFind array from
|   - lenKmerUC:
|     o length of one kmer
|   - lenArySIPtr:
|     o will hold the number of refST_kmerFind structures
|       made
|   - minPercKmersF:
|     o float with minimum percentage of kmers to start
|       considering an window supports an spacer
|   - maxNtUIPtr:
|     o pointer to unsigned int to hold the largest
|       reference sequence
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to link
|       with this reference set
|   - percExtraNtInWinF:
|     o float with percentage of extra nucleotides to
|       store in one window (beyond reference length)
|   - percWinShiftF:
|     o float with percentage of bases to shift for each
|       new window in tblSTPtr
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       settings (for finding max possible score)
|   - errUC:
|     o holds error type
| Output:
|   - Modifies:
|     o lenArySIPtr to hold the number of sequences in
|       th refST_kmerFind array
|     o errUC to hold the error type
|       - 0 for no errors
|       - memErr_tbSpoligo for memory errors
|       - fileErr_tbSpoligo for file errors
|     o ntInWinUI in tblSTPtr to have the number of bases
|       in one window (2 * longest primer length)
|     o rmNtUI in tblSTPtr to have the number of bases
|       to remove with each window shift
|       (percWinShiftF * tblSTPtr->ntINWinUI)
|   - Returns:
|     o an array of refST_kmerFind structures with
|       sequences to check
|     o 0 for an error
\-------------------------------------------------------*/
refST_kmerFind *
mkAry_refST_kmerFind(
   signed char *faFileStr,
   unsigned char lenKmerUC,
   signed int *lenArySIPtr,
   float minPercKmersF,
   struct tblST_kmerFind *tblSTPtr,
   float percExtraNtInWinF,
   float percWinShiftF,
   struct alnSet *alnSetSTPtr,
   unsigned char *errUC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - makes an array of refST_kmerFind structures
   '   o fun11 sec01:
   '     - variable declerations 
   '   o fun11 sec02:
   '     - check if can open input file
   '   o fun11 sec03:
   '     - get number of sequences in file
   '   o fun11 sec04:
   '     - allocate memory
   '   o fun11 sec05:
   '     - read in sequences
   '   o fun11 sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declerations 
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ushort lenBuffUS = 4096;
   signed char buffStr[lenBuffUS];
   ulong bytesUL = 0;
   ulong posUL = 0;
   schar newLineBl = 0;

   uchar ntUC = 0;

   sint  numSeqSI = 0;
   sint siSeq = 0;
   sint siKmer = 0;

   /*for building kmers*/
   ulong forKmerUL = 0;
   ulong lenForKmerUL = 0;

   ulong revKmerUL = 0;
   ulong lenRevKmerUL = 0;

   ulong maskUL = 0;

   uint longestPrimUI = 0; /*length of longest primer*/

   /*structures to work with*/
   struct refST_kmerFind *retRefSTAry = 0;

   FILE *faFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - initialize and check if can open input file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *lenArySIPtr = 0;

   faFILE = fopen((char *) faFileStr, "r");

   if(! faFILE)
      goto fileErr_fun11_sec06_sub03;

   maskUL = tblSTPtr->kmerMaskUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - get number of sequences in file
   ^   o fun11 sec03 sub01:
   ^     - read in first part of file and start loop
   ^   o fun11 sec03 sub02:
   ^     - see if '>' for new line cases
   ^   o fun11 sec03 sub03:
   ^     - find next newline or end of buffer
   ^   o fun11 sec03 sub04:
   ^     - check for errors and move to start of file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - read in first part of file and start loop
   \*****************************************************/

   bytesUL =
      fread(
         (char *) buffStr,
         sizeof(char),
         lenBuffUS - 1,
         faFILE
      );

   buffStr[bytesUL] = '\0';
   posUL = 0;
   newLineBl = 1; /*assume I start on an new line*/

   while(bytesUL)
   { /*Loop: find number of sequences in file*/

      /**************************************************\
      * Fun11 Sec03 Sub02:
      *   - see if '>' for new line cases
      \**************************************************/

      if(newLineBl)
      { /*If: I just finished an newline*/
         while(buffStr[posUL] < 33)
         { /*Loop: Get past white space*/
            if(posUL >= bytesUL)
            { /*If: I need to read in more buffer*/
               bytesUL =
                  fread(
                     (char *) buffStr,
                     sizeof(char),
                     lenBuffUS - 1,
                     faFILE
                  );

               buffStr[bytesUL] = '\0';
               posUL = 0;

               if(! bytesUL)
                  goto EOF_fun11_sec03_sub04;
            } /*If: I need to read in more buffer*/

            else
               ++posUL;
         } /*Loop: Get past white space*/

         if(buffStr[posUL] == '>')
            ++numSeqSI;

         newLineBl = 0;
      } /*If: I just finished an newline*/

      /**************************************************\
      * Fun11 Sec03 Sub03:
      *   - find next newline or end of buffer
      \**************************************************/

      posUL += ulEndStrLine(&buffStr[posUL]);

      if(buffStr[posUL] == '\0')
      { /*If: I need to get more file*/
         bytesUL =
            fread(
               (char *) buffStr,
               sizeof(char),
               lenBuffUS,
               faFILE
            );

         posUL = 0;
      } /*If: I need to get more file*/

      else
         newLineBl = 1;
   } /*Loop: find number of sequences in file*/

   /*****************************************************\
   * Fun11 Sec03 Sub04:
   *   - check for errors and move to start of file
   \*****************************************************/

   EOF_fun11_sec03_sub04:;

   if(*errUC)
   { /*If: I had an error of some kind*/
      if(*errUC & 64)
         goto memErr_fun11_sec06_sub02; /*memory error*/

      goto fileErr_fun11_sec06_sub03;
   } /*If: I had an error of some kind*/

   if(numSeqSI < 1)
      goto fileErr_fun11_sec06_sub03; /*no seq in file*/

   fseek(faFILE, 0, SEEK_SET);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retRefSTAry =
      malloc(numSeqSI * sizeof(struct refST_kmerFind));

   if(! retRefSTAry)
      goto memErr_fun11_sec06_sub02;

   for(
      *lenArySIPtr = 0;
      *lenArySIPtr < numSeqSI;
      ++(*lenArySIPtr)
   ){ /*Loop: initialize the new structures*/
      *errUC |=
         (uchar)
         init_refST_kmerFind(
            &retRefSTAry[*lenArySIPtr],
            lenKmerUC
         );
   } /*Loop: initialize the new structures*/

   if(*errUC)
   { /*If: I had an memroy error*/
      *lenArySIPtr = numSeqSI;
      goto memErr_fun11_sec06_sub02;
   } /*If: I had an memroy error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec05:
   ^   - read in sequences
   ^   o fun11 sec05 sub01:
   ^     - read first sequence and start loop
   ^   o fun11 sec05 sub02:
   ^     - copy forward and reverse sequences
   ^   o fun11 sec05 sub03:
   ^     - find the minimum number of kmers
   ^   o fun11 sec05 sub04:
   ^     - allocate memory for forward kmer arrays
   ^   o fun11 sec05 sub05:
   ^     - allocate reverse kmer array memory
   ^   o fun11 sec05 sub06:
   ^     - initialize kmer counts
   ^   o fun11 sec05 sub07:
   ^     - get kmer counts
   ^   o fun11 sec05 sub08:
   ^     - get forward counts and max score
   ^   o fun11 sec05 sub09:
   ^     - make short kmer arrays
   ^   o fun11 sec05 sub10:
   ^     - deal with errors or no sequences
   ^   o fun11 sec05 sub11:
   ^     - update the table window size values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec05 Sub01:
   *   - read first sequence and start loop
   \*****************************************************/

   for(
      numSeqSI = 0;
      numSeqSI < *lenArySIPtr;
      ++numSeqSI
   ){ /*Loop: read in each sequence in the file*/

       /*************************************************\
       * Fun11 Sec05 Sub02:
       *   - copy forward and reverse sequences
       \*************************************************/

       *errUC = 
          readFaSeq(
            faFILE,
            retRefSTAry[numSeqSI].forSeqST
          );

       if(*errUC > 1)
          goto memErr_fun11_sec06_sub02; /*memory error*/

       longestPrimUI =
          noBranchMax(
             longestPrimUI,
             (uint)
               retRefSTAry[numSeqSI].forSeqST->lenSeqUL
          ); /*find the length of the longest primer*/

       *errUC =
          cpSeqST(
             retRefSTAry[numSeqSI].revSeqST,
             retRefSTAry[numSeqSI].forSeqST
          ); /*copy the reverse complement sequence*/

       reverseComplementSeq(
          retRefSTAry[numSeqSI].revSeqST
       );

       /*convert sequences to the correct format*/
       seqToLookupIndex(
          retRefSTAry[numSeqSI].forSeqST->seqStr
       );

       seqToLookupIndex(
          retRefSTAry[numSeqSI].revSeqST->seqStr
       );
       
       /*I am merging duplicates, so I never expect
       `   more then the maxiumum number of possible kmers
       */
       retRefSTAry[numSeqSI].lenAryUI= tblSTPtr->lenTblUI;

       /*************************************************\
       * Fun11 Sec05 Sub03:
       *   - find the minimum number of kmers
       \*************************************************/

       retRefSTAry[numSeqSI].minKmersUI = 
          (uint) retRefSTAry[numSeqSI].forSeqST->lenSeqUL;

       retRefSTAry[numSeqSI].minKmersUI -= 
          retRefSTAry[numSeqSI].lenKmerUC;

       ++retRefSTAry[numSeqSI].minKmersUI; /*total kmers*/
       retRefSTAry[numSeqSI].minKmersUI *= minPercKmersF;

       /*************************************************\
       * Fun11 Sec05 Sub04:
       *   - allocate memory for forward kmer arrays
       \*************************************************/
       
       retRefSTAry[numSeqSI].forKmerArySI =
          malloc(
             (retRefSTAry[numSeqSI].lenAryUI + 1)
             * sizeof(uint)
       );

       if(! retRefSTAry[numSeqSI].forKmerArySI)
          goto memErr_fun11_sec06_sub02;

       retRefSTAry[numSeqSI].forRepAryUI =
          malloc(
             (retRefSTAry[numSeqSI].lenAryUI + 1)
             * sizeof(uint)
       );

       if(! retRefSTAry[numSeqSI].forRepAryUI)
          goto memErr_fun11_sec06_sub02;

       /*************************************************\
       * Fun11 Sec05 Sub04:
       *   - allocate reverse kmer array memory
       \*************************************************/
       
       retRefSTAry[numSeqSI].revKmerArySI =
          malloc(
             (retRefSTAry[numSeqSI].lenAryUI + 1)
             * sizeof(uint)
       );

       if(! retRefSTAry[numSeqSI].revKmerArySI)
          goto memErr_fun11_sec06_sub02;

       retRefSTAry[numSeqSI].revRepAryUI =
          malloc(
             (retRefSTAry[numSeqSI].lenAryUI + 1)
             * sizeof(uint)
       );

       if(! retRefSTAry[numSeqSI].revRepAryUI)
          goto memErr_fun11_sec06_sub02;
 
       /*************************************************\
       * Fun11 Sec05 Sub05:
       *   - initialize kmer counts
       \*************************************************/

       for(
          siKmer = 0;
          siKmer < retRefSTAry[numSeqSI].lenAryUI;
          ++siKmer
       ){ /*Loop: inititalize kmer counts*/
          retRefSTAry[numSeqSI].forRepAryUI[siKmer] = 0;
          retRefSTAry[numSeqSI].revRepAryUI[siKmer] = 0;

          retRefSTAry[
             numSeqSI
          ].forKmerArySI[siKmer] = def_noKmer_kmerFind;

          retRefSTAry[
             numSeqSI
          ].revKmerArySI[siKmer] = def_noKmer_kmerFind;
       } /*Loop: inititalize kmer counts*/

       /*************************************************\
       * Fun11 Sec05 Sub06:
       *   - get kmer counts
       \*************************************************/

       for(
          siSeq = 0;
           siSeq
              < retRefSTAry[
                  numSeqSI
                ].forSeqST->lenSeqUL;
          ++siSeq
       ){ /*Loop: copy the kmers*/

          /**********************************************\
          * Fun11 Sec05 Sub07:
          *   - get foward counts and max score
          \**********************************************/

          ntUC = 
             (uchar)
             retRefSTAry[
                 numSeqSI
               ].forSeqST->seqStr[siSeq];

          retRefSTAry[numSeqSI].maxForScoreF +=
             getBaseScore(
                ntUC & defClearNonAlph,
                ntUC & defClearNonAlph,
                alnSetSTPtr
             );

          addNtToKmer_kmerFind(
             ntUC,
             forKmerUL,
             lenForKmerUL,
             maskUL
          );

          if(lenForKmerUL >= lenKmerUC)
          { /*If: I have an complete forword kmer*/
             retRefSTAry[
                numSeqSI
             ].forRepAryUI[forKmerUL] += 1;
             
             retRefSTAry[
                numSeqSI
             ].forKmerArySI[forKmerUL] = (uint) forKmerUL;
          } /*If: I have an complete forword kmer*/

          /**********************************************\
          * Fun11 Sec05 Sub08:
          *   - get reverse counts and max score
          \**********************************************/

          ntUC = 
             (uchar)
             retRefSTAry[
                 numSeqSI
               ].revSeqST->seqStr[siSeq];

          retRefSTAry[numSeqSI].maxRevScoreF +=
             getBaseScore(
                ntUC & defClearNonAlph,
                ntUC & defClearNonAlph,
                alnSetSTPtr
             );

          addNtToKmer_kmerFind(
             ntUC,
             revKmerUL,
             lenRevKmerUL,
             maskUL
          );

          if(lenRevKmerUL >= lenKmerUC)
          { /*If: I have an complete reverse kmer*/
             retRefSTAry[
                numSeqSI
             ].revRepAryUI[revKmerUL] += 1;
             
             retRefSTAry[
                numSeqSI
             ].revKmerArySI[revKmerUL] = (uint) revKmerUL;
          } /*If: I have an complete reverse kmer*/
       } /*Loop: copy the kmers*/

       /*************************************************\
       * Fun11 Sec05 Sub09:
       *   - make short kmer arrays
       \*************************************************/

       /*I am converting the kmer numbers to unsigned ints
       `  so that -1's and -2's will be at the ends
       */
       twoArySortNumeric(
           (uint *)
              retRefSTAry[numSeqSI].forKmerArySI,
           retRefSTAry[numSeqSI].forRepAryUI,
           0,
           retRefSTAry[numSeqSI].lenAryUI - 1
       ); /*sort the forward kmers by kmer id*/

        twoArySortNumeric(
           (uint *)
              retRefSTAry[numSeqSI].revKmerArySI,
           retRefSTAry[numSeqSI].revRepAryUI,
           0,
           retRefSTAry[numSeqSI].lenAryUI - 1
       ); /*sort the revers kmers by kmer id*/
   } /*Loop: read in each sequence in the file*/

   /*****************************************************\
   * Fun11 Sec05 Sub10:
   *   - deal with errors or no sequences
   \*****************************************************/

   if(*errUC)
   { /*If: I had an error of some kind*/
      if(*errUC & 64)
         goto memErr_fun11_sec06_sub02; /*memory error*/

      goto fileErr_fun11_sec06_sub03;
   } /*If: I had an error of some kind*/

   if(numSeqSI < 1)
      goto fileErr_fun11_sec06_sub03; /*no seq in file*/

   /*****************************************************\
   * Fun11 Sec05 Sub11:
   *   - update the table window size values
   \*****************************************************/

   /*find window size*/
   tblSTPtr->ntInWinUI = longestPrimUI;
   tblSTPtr->ntInWinUI *= percExtraNtInWinF;
   tblSTPtr->ntInWinUI += longestPrimUI;

   /*find number of kmers in one window*/
   tblSTPtr->numKmerUI = tblSTPtr->ntInWinUI;
   tblSTPtr->numKmerUI -= tblSTPtr->lenKmerUC;
   ++tblSTPtr->numKmerUI;

   /*find the number of kmers to remove per window shift*/
   tblSTPtr->rmNtUI = tblSTPtr->ntInWinUI;
   tblSTPtr->rmNtUI *= percWinShiftF;

   /*set up the kmer window array*/
   tblSTPtr->kmerArySI =
      malloc((tblSTPtr->numKmerUI + 1) * sizeof(sint));

   if(! tblSTPtr->kmerArySI)
      goto memErr_fun11_sec06_sub02; /*memory error*/

   for(
      siSeq = 0;
      siSeq < tblSTPtr->numKmerUI;
      ++siSeq
   ) tblSTPtr->kmerArySI[siSeq] = def_noKmer_kmerFind;
   
   tblSTPtr->kmerArySI[siSeq] = def_endKmers_kmerFind;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec06:
   ^   - clean up
   ^   o fun11 sec06 sub01:
   ^     - success clean up
   ^   o fun11 sec06 sub02:
   ^     - memory error report
   ^   o fun11 sec06 sub03:
   ^     - file error report
   ^   o fun11 sec06 sub04:
   ^     - error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec06 Sub01:
   *   - success clean up
   \*****************************************************/

   *errUC = 0;

   fclose(faFILE);
   faFILE = 0;

   return retRefSTAry;

   /*****************************************************\
   * Fun11 Sec06 Sub02:
   *   - memory error report
   \*****************************************************/

   memErr_fun11_sec06_sub02:;

   *errUC = memErr_tbSpoligo;

   goto errCleanUp_fun11_sec06_sub04;

   /*****************************************************\
   * Fun11 Sec06 Sub03:
   *   - file error report
   \*****************************************************/

   fileErr_fun11_sec06_sub03:;

   *errUC = fileErr_tbSpoligo;

   goto errCleanUp_fun11_sec06_sub04;

   /*****************************************************\
   * Fun11 Sec06 Sub04:
   *   - error clean up
   \*****************************************************/

   errCleanUp_fun11_sec06_sub04:;

   fclose(faFILE);
   faFILE = 0;

   freeHeapAry_refST_kmerFind(
      retRefSTAry,
      *lenArySIPtr
   );

   retRefSTAry = 0;
   *lenArySIPtr = 0;

   return 0;
} /*mkAry_refST_kmerFind*/

/*-------------------------------------------------------\
| Fun13: nextSeqChunk_tblST_kmerFind
|   - adds a new set of kmers from an sequence to an
|     tblST_kmerFind structure
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to add
|       kmers to
|   - firstTimeBl:
|     o 1: first time adding sequence (blank kmer array)
|     o 0: updating the kmer window
| Output:
|   - Modifies:
|     o tblSI and seqAryUS in tblSTPtr to have the old
|       kmers (number specified by rmNtUI in tblSI)
|       remove and the new kmers added in
|       - for end of sequence it sets an index to
|         def_endKmers_kmerFind
|    o firstTimeBl:
|      o to be 0 if it is 1
|   - Returns:
|     o 0 for not end of sequence
|     o 1 for end of sequence
\-------------------------------------------------------*/
signed char
nextSeqChunk_tblST_kmerFind(
   struct tblST_kmerFind *tblSTPtr,/*table to add seq to*/
   signed char *firstTimeBl /*1: first set of kmers*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   o fun13 sec01:
   '     - variable declerations
   '   o fun13 sec02:
   '     - handle first time adding in sequence cases
   '   o fun13 sec03:
   '     - remove the old kmers from the table
   '   o fun13 sec04:
   '     - move keep kmers to start of kmer array
   '   o fun13 sec05:
   '     - add new kmers to table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiNtMac = 0;
   uint endWindowUI = 0;
   uint dupNtMacUI = 0;
   ulong kmerMacUL = 0;
   schar lastWinBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - handle first time adding in sequence cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*firstTimeBl)
   { /*If: this is the first window*/
      *firstTimeBl = 0; /*no longer first time*/
      kmerMacUL = 0;    /*no kmers in window*/

      dupNtMacUI = tblSTPtr->seqPosUL;

      endWindowUI = tblSTPtr->seqPosUL;
      endWindowUI += tblSTPtr->lenKmerUC;
      --endWindowUI; /*I want to be 1 base off the kmer*/

      while(tblSTPtr->seqPosUL < tblSTPtr->lenKmerUC)
      { /*Loop: build the first kmer*/
         addNtToKmer_kmerFind(
            tblSTPtr->seqST->seqStr[tblSTPtr->seqPosUL],
            kmerMacUL,
            tblSTPtr->lenLastKmerUL,
            tblSTPtr->kmerMaskUL
         );

         ++tblSTPtr->seqPosUL;
      } /*Loop: build the first kmer*/

      endWindowUI = dupNtMacUI;
      endWindowUI += tblSTPtr->ntInWinUI;

      dupNtMacUI = 0;   /*first base in window*/

      goto firstKmers_fun13_sec04;
   } /*If: this is the first window*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - remove the old kmers from the table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      uiNtMac = 0;
      uiNtMac < (tblSTPtr)->rmNtUI;
      ++uiNtMac
   ){ /*Loop: remove discarded kmers from table*/
      kmerMacUL = tblSTPtr->kmerArySI[uiNtMac];

      if((slong) kmerMacUL < 0)
      { /*If: I have an invalid kmer*/
         if((slong) kmerMacUL == def_endKmers_kmerFind)
            break; /*was an end of kmers*/

         continue; /*was an no kmer*/
      } /*If: I have an invalid kmer*/

      --tblSTPtr->tblSI[kmerMacUL];
   } /*Loop: remove discarded kmers from table*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec04:
   ^   - move keep kmers to start of kmer array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   dupNtMacUI = 0;

   for(
      uiNtMac = (tblSTPtr)->rmNtUI;
      uiNtMac < (tblSTPtr)->numKmerUI;
      ++uiNtMac
   ){ /*Loop: copy keep kmers over*/
      tblSTPtr->kmerArySI[ dupNtMacUI ] =
         tblSTPtr->kmerArySI[ uiNtMac ];

      if(
            tblSTPtr->kmerArySI[ uiNtMac ]
         == def_endKmers_kmerFind
      ) break; /*finished copying*/

      ++dupNtMacUI;
   } /*Loop: copy keep kmers over*/

   kmerMacUL = tblSTPtr->kmerArySI[ dupNtMacUI - 1 ];

   endWindowUI = tblSTPtr->ntInWinUI;
   endWindowUI -= tblSTPtr->rmNtUI;
   endWindowUI += tblSTPtr->seqPosUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec05:
   ^   - add new kmers to table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   firstKmers_fun13_sec04:; /*first time for sequence*/

   if(endWindowUI > tblSTPtr->seqST->lenSeqUL)
   { /*If: this is the last window*/
      endWindowUI = tblSTPtr->seqST->lenSeqUL;
      lastWinBl = 1;
   } /*If: this is the last window*/

   while(tblSTPtr->seqPosUL < endWindowUI)
   { /*Loop: add kmers in*/
      addNtToKmer_kmerFind(
         tblSTPtr->seqST->seqStr[tblSTPtr->seqPosUL],
         kmerMacUL,
         tblSTPtr->lenLastKmerUL,
         tblSTPtr->kmerMaskUL
      );

      tblSTPtr->kmerArySI[ dupNtMacUI ] =
         (signed int) kmerMacUL;

      /*add kmer to kmer table counts if is complete*/
      if(
            (slong) tblSTPtr->lenLastKmerUL
         >= tblSTPtr->lenKmerUC
      ) ++tblSTPtr->tblSI[ kmerMacUL ];

      ++(tblSTPtr->seqPosUL); /*move to next base in seq*/
      ++dupNtMacUI; /*add the new kmer in*/
   } /*Loop: add kmers in*/

   if(lastWinBl)
   { /*If: this is the last window*/
      /*mark end of sequence*/
      tblSTPtr->kmerArySI[dupNtMacUI] =
         def_endKmers_kmerFind;

      return 1;
   } /*If: this is the last window*/

   return 0;
} /*nextSeqChunk_tblST_kmerFind*/

/*-------------------------------------------------------\
| Fun16: matchCheck_kmerFind
|   - tells if the  match meets the min requirements to
|     do an alignment or not
| Input:
|   - tblST_kmerFindPtr:
|     o pointer to an tblST_kmerFind structure with the
|       chunk of query (kmer table) to check
|   - refST_kmerFindPtr:
|     o pointer to an refST_kmerFind structure with the
|       reference (primers) kmers to check
| Output:
|   - Returns:
|     o 0 (foward) or 1 (best) if there is no match
|     o 0 | 1 if match and the forward direction is best
|     o 2 | 1 if match and the reverse direction is best
\-------------------------------------------------------*/
unsigned char
matchCheck_kmerFind(  
   struct tblST_kmerFind *tblST_kmerFindPtr, 
   struct refST_kmerFind *refST_kmerFindPtr  
){ 
   unsigned char retMacBl = 0; 
   unsigned char revBestMacBl = 0; 
   unsigned long forKmerCntMacUL = 0; 
   unsigned long revKmerCntMacUL = 0; 
   unsigned long bestKmerCntMacUL = 0; 
   
   forKmerCntMacUL = 
      forCntMatchs_kmerFind( 
         (tblST_kmerFindPtr), 
         (refST_kmerFindPtr) 
      ); 
   
   revKmerCntMacUL = 
      revCntMatchs_kmerFind( 
         (tblST_kmerFindPtr), 
         (refST_kmerFindPtr) 
      ); 
   
   bestKmerCntMacUL = 
      noBranchMax( 
         forKmerCntMacUL, 
         revKmerCntMacUL 
      ); 
   
   revBestMacBl = (revKmerCntMacUL == bestKmerCntMacUL); 
   revBestMacBl <<= 1; 
   
   retMacBl=( 
      bestKmerCntMacUL >= (refST_kmerFindPtr)->minKmersUI
   ); 
   
   return retMacBl | revBestMacBl;
} /*matchCheck_kmerFind*/

/*-------------------------------------------------------\
| Fun17: findRefInChunk_kmerFind
|   - does an kmer check and alings an single sequence
|     in an refST_kmerFind structure to see if there is
|     an match
| Input:
|   - tblST_kmerFindPtr:
|     o pointer to an tblST_kmerFind structure with the
|       chunk of query (kmer table) to check
|     o the stored sequence must be converted with
|       seqToLookupIndex from alnSetStruct.h
|   - refST_kmerFindPtr:
|     o pointer to an refST_kmerFind structure with the
|       reference (primers) kmers to check
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       settings
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - scoreSL:
|     o pointer to an signed long to hold the alingment
|       score
|   - qryStartUL:
|     o pointer to an unsigned long to hold the first base
|       in the query alingment
|   - qryEndUL:
|     o pointer to an unsigned long to hold the last base
|       in the query alingment
|   - refStartUL:
|     o pointer to an unsigned long to hold the first base
|       in the reference alingment
|   - refEndUL:
|     o pointer to an unsigned long to hold the last base
|       in the reference alingment
| Output:
|   - Modifies:
|     o scoreSL
|       - 0 if no alignment done
|       - score if an alignment was done
|     o qryStartUL
|       - 0 if no alignment done
|       - first aligned query base if alignment done
|     o qryEndtUL
|       - 0 if no alignment done
|       - last aligned query base if alignment done
|     o refStartUL
|       - 0 if no alignment done
|       - first aligned reference base if alignment done
|     o refEndtUL
|       - 0 if no alignment done
|       - last aligned reference base if alignment done
|   - Returns:
|     o 1 if the reference sequence was found in the
|       kmer table (query) sequence
|     o 0 if reference sequence not found
\-------------------------------------------------------*/
unsigned char
findRefInChunk_kmerFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTPtr,
   struct alnSet *alnSetSTPtr,
   float minPercScoreF,
   signed long *scoreSL,
   unsigned long *qryStartUL,
   unsigned long *qryEndUL,
   unsigned long *refStartUL,
   unsigned long *refEndUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - finds an sequence pattern in an sam entry
   '   o fun17 sec01:
   '     - variable declerations
   '   o fun17 sec02:
   '     - initialize & see if enough kmers for alignment
   '   o fun17 sec03:
   '     - prepare for alignemnt (if passed kmer check)
   '   o fun17 sec04:
   '     - do alignment and check if passes min score
   '   o fun17 sec05:
   '     - return the answer
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char matchBl = 0;
   float percScoreF = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - initialize and see if enough kmers for alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *(qryStartUL) = 0;
   *(qryEndUL) = 0;

   *(refStartUL) = 0;
   *(refEndUL) = 0;

   *(scoreSL) = 0;

   matchBl =
      matchCheck_kmerFind(
         (tblSTPtr),
         (refSTPtr)
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - prepare for alignemnt (if passed kmer check)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(matchBl & 1)
   { /*If: I had enough kmers to do an alignment*/
      /*find the max score possible*/
      /*start of alignment region*/
      (tblSTPtr)->seqST->offsetUL = (tblSTPtr)->seqPosUL;
      (tblSTPtr)->seqST->offsetUL -=(tblSTPtr)->ntInWinUI;

      /*find end of alignment region*/
      (tblSTPtr)->seqST->endAlnUL = (tblSTPtr)->seqPosUL;

      /*convert index 1 to index 0*/
      --(tblSTPtr)->seqST->endAlnUL;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun17 Sec04:
      ^   - do alignment and check if passes min score
      ^   o fun17 sec04 sub01:
      ^     - do the alignment
      ^   o fun17 sec04 sub02:
      ^     - check if it passes the alignment
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      /**************************************************\
      * Fun17 Sec04 Sub01:
      *   - do the alignment
      \**************************************************/

      if(matchBl & 2)
      { /*If: this was an reverse alignment*/
         *(scoreSL) =
            memWater(
              (tblSTPtr)->seqST,
              (refSTPtr)->revSeqST,
              (refStartUL),
              (refEndUL),
              (qryStartUL),
              (qryEndUL),
              (alnSetSTPtr)
            ); /*align primer to region*/

         percScoreF = (float) (*scoreSL);
         percScoreF /= (refSTPtr)->maxRevScoreF;
      } /*If: this was an reverse alignment*/

      else
      { /*Else: this is an foward alignment*/
         *(scoreSL) =
            memWater(
              (tblSTPtr)->seqST,
              (refSTPtr)->forSeqST,
              (refStartUL),
              (refEndUL),
              (qryStartUL),
              (qryEndUL),
              (alnSetSTPtr)
            ); /*align primer to region*/

         percScoreF = (float) (*scoreSL);
         percScoreF /= (refSTPtr)->maxForScoreF;
      } /*Else: this is an foward alignment*/

      /**************************************************\
      * Fun17 Sec04 Sub02:
      *   - check if it passes the alignment
      \**************************************************/

      matchBl = ( percScoreF >= (minPercScoreF) );
   } /*If: I had enough kmers to do an alignment*/

   else
     matchBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec05:
   ^   - return the answer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return matchBl;
} /*findRefInChunk_kmerFind*/

/*-------------------------------------------------------\
| Fun18: samFindSpoligos_kmerFind
|   - finds spoligotype spacers in an sequence using
|     an faster kmer search followed by an slower waterman
|     to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       to check for spoligotypes in
|   - dirStartSI:
|     o position of first base in the direct repeat region
|   - dirEndSI:
|     o position of last base in the direct repeat region
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       settings
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - noSpoligo_tbSpoligo if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o noSpoligo_tbSpoligo if no spoligotypes were found
\-------------------------------------------------------*/
unsigned char
samFindSpoligos_kmerFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct samEntry *samSTPtr,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ^ Fun18 TOC:
   '   o fun18 sec01:
   '     - varaible declerations
   '   o fun18 sec02:
   '     - assign sequence to table
   '   o fun18 sec03:
   '     - check sequence for spacers
   '   o fun18 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - varaible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar errUC = 0; /*error messages / return*/
   uchar retUC = noSpoligo_tbSpoligo; /*return*/

   uchar swapBl = 0; /*swapped in sequence into table*/

   uchar matchBl = 0;
   signed char firstTimeBl = 1;

   /*to keep the old assigned sequence*/
   signed char *oldSeqStr = 0;
   ulong oldLenSeqUL = 0;

   uint uiSpoligo = 0;

   /*alignemnt variables; i ingore*/
   slong scoreSL = 0;
   ulong qryStartUL = 0;
   ulong qryEndUL = 0;
   ulong refStartUL = 0;
   ulong refEndUL = 0;

   /*variables for finding dr region*/
   sint siCig = 0;
   sint cigBaseOnSI = 0;
   sint refPosSI = 0;
   sint stopPosSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - check positions and assign sequence to table
   ^   o fun18 sec02 sub01:
   ^     - see if i have an direct repeat region
   ^   o fun18 sec02 sub02:
   ^     - add the sequence to the kmer table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec02 Sub01:
   *   - see if i have an direct repeat region
   \*****************************************************/

   if(fragCheckBl)
   { /*If: I am checking fragments*/
      if(samSTPtr->refEndUI < dirStartSI)
         goto noSpoligo_fun18_sec04_sub02;

      if(samSTPtr->refStartUI > dirEndSI)
         goto noSpoligo_fun18_sec04_sub02;
   } /*If: I am checking fragments*/

   else
   { /*Else: I am checking full direct repeat regions*/
      if(samSTPtr->refStartUI > dirStartSI)
         goto noSpoligo_fun18_sec04_sub02;

      if(samSTPtr->refEndUI < dirEndSI)
         goto noSpoligo_fun18_sec04_sub02;

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] = 0; /*blank for new seq*/
   } /*Else: I am checking full direct repeat regions*/

   codeAryUI[lenRefAryUI] = -1;

   /*****************************************************\
   * Fun18 Sec02 Sub02:
   *   - add the sequence to the kmer table
   \*****************************************************/

   swapBl = 1;
   oldSeqStr = (schar *) tblSTPtr->seqST->seqStr;
   oldLenSeqUL = tblSTPtr->seqST->lenSeqUL;

   blank_tblST_kmerFind(
      tblSTPtr,
      0          /*I do not want to blank the sequence*/
   );

   tblSTPtr->seqST->seqStr = samSTPtr->seqStr;
   tblSTPtr->seqST->lenSeqUL = samSTPtr->readLenUI;
   
   cigBaseOnSI = samSTPtr->cigValAryI[0];
   refPosSI = (sint) samSTPtr->refStartUI;

   /*find the direct repeat start and end coordiantes*/
   samEntryFindRefPos(
      samSTPtr,
      siCig,
      cigBaseOnSI,
      dirStartSI,
      refPosSI,
      stopPosSI
   );

   tblSTPtr->seqPosUL = (ulong) stopPosSI;

   samEntryFindRefPos(
      samSTPtr,
      siCig,
      cigBaseOnSI,
      dirEndSI,
      refPosSI,
      stopPosSI
   );

   /*prepare for waterman alignment*/
   seqToLookupIndex(tblSTPtr->seqST->seqStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - check sequence for spacers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   do{
      errUC =
         (uchar)
         nextSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         );

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ){ /*Loop: detect spoligotypes in each chunk*/
         matchBl =
            findRefInChunk_kmerFind(
               tblSTPtr,
               &(refSTAry[uiSpoligo]),
               alnSetSTPtr,
               minPercScoreF,
               &scoreSL,
               &qryStartUL,
               &qryEndUL,
               &refStartUL,
               &refEndUL
            );

         codeAryUI[uiSpoligo] += matchBl;
         retUC &= (!matchBl);
      } /*Loop: detect spoligotypes in each chunk*/

      if(tblSTPtr->seqPosUL >= stopPosSI)
         break;
   } while(! errUC);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - clean up
   ^   o fun18 sec04 sub01:
   ^     - no error
   ^   o fun18 sec04 sub02:
   ^     - memory error clean up
   ^   o fun18 sec04 sub03:
   ^     - general clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec04 Sub01:
   *   - no error
   \*****************************************************/

   goto cleanUp_fun18_sec04_sub04;

   /*****************************************************\
   * Fun18 Sec04 Sub02:
   *   - sequene out of bounds
   \*****************************************************/

   noSpoligo_fun18_sec04_sub02:;

   retUC = noSpoligo_tbSpoligo;

   goto cleanUp_fun18_sec04_sub04;

   /*****************************************************\
   * Fun18 Sec04 Sub03:
   *   - memory error
   \*****************************************************/

   /*
   memErr_fun18_sec04_sub03:;

   retUC = memErr_tbSpoligo;

   goto cleanUp_fun18_sec04_sub04;
   */

   /*****************************************************\
   * Fun18 Sec04 Sub04:
   *   - general clean up
   \*****************************************************/

   cleanUp_fun18_sec04_sub04:;

   /*reset table to original sequences*/

   /*convert back to sequence*/
   if(swapBl)
   { /*If: I tried to detect spoligotypes*/
      lookupIndexToSeq(tblSTPtr->seqST->seqStr);

      tblSTPtr->seqST->seqStr = (char *) oldSeqStr;
      tblSTPtr->seqST->lenSeqUL = oldLenSeqUL;
   } /*If: I tried to detect spoligotypes*/

   return retUC;
} /*samFindSpoligos_kmerFind*/

/*-------------------------------------------------------\
| Fun19: fxFindSpoligos_kmerFind
|   - finds spoligotype spacers in an sequence (from
|     fastx file) using an faster kmer search followed by
|     an slower waterman to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - seqSTPtr:
|     o pointer to an seqStruct structure with the
|       sequence to check for spoligotypes in
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       settings
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - noSpoligo_tbSpoligo if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o noSpoligo_tbSpoligo if no spoligotypes were found
\-------------------------------------------------------*/
unsigned char
fxFindSpoligos_kmerFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct seqStruct *seqSTPtr,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ^ Fun19 TOC:
   '   o fun19 sec01:
   '     - varaible declerations
   '   o fun19 sec02:
   '     - assign sequence to table
   '   o fun19 sec03:
   '     - check sequence for spacers
   '   o fun19 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - varaible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar errUC = 0; /*error messages*/
   uchar retUC = noSpoligo_tbSpoligo; /*return*/

   uchar matchBl = 0;
   signed char firstTimeBl = 1;

   uint uiSpoligo = 0;

   /*alignemnt variables; i ingore*/
   slong scoreSL = 0;
   ulong qryStartUL = 0;
   ulong qryEndUL = 0;
   ulong refStartUL = 0;
   ulong refEndUL = 0;

   /*to keep the old assigned sequence*/
   struct seqStruct *oldSeqST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - check positions and assign sequence to table
   ^   o fun19 sec02 sub01:
   ^     - see if i have an direct repeat region
   ^   o fun19 sec02 sub02:
   ^     - add the sequence to the kmer table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec02 Sub01:
   *   - see if i have an direct repeat region
   \*****************************************************/

   if(! fragCheckBl)
   { /*If: I am checking full direct repeat regions*/
      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ) codeAryUI[uiSpoligo] = 0; /*blank for new seq*/
   } /*If: I am checking full direct repeat regions*/

   codeAryUI[lenRefAryUI] = -1;

   /*****************************************************\
   * Fun19 Sec02 Sub02:
   *   - add the sequence to the kmer table
   \*****************************************************/

   retUC = noSpoligo_tbSpoligo; /*allows detection*/

   oldSeqST = tblSTPtr->seqST;
   tblSTPtr->seqST = seqSTPtr;

   blank_tblST_kmerFind(
      tblSTPtr,
      0          /*I do not want to blank the sequence*/
   );

   /*prepare for waterman alignment*/
   seqToLookupIndex(tblSTPtr->seqST->seqStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - check sequence for spacers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   do{
      errUC =
         (uchar)
         nextSeqChunk_tblST_kmerFind(
            tblSTPtr,
            &firstTimeBl
         );

      for(
         uiSpoligo = 0;
         uiSpoligo < lenRefAryUI;
         ++uiSpoligo
      ){ /*Loop: detect spoligotypes in each chunk*/
         matchBl =
            findRefInChunk_kmerFind(
               tblSTPtr,
               &(refSTAry[uiSpoligo]),
               alnSetSTPtr,
               minPercScoreF,
               &scoreSL,
               &qryStartUL,
               &qryEndUL,
               &refStartUL,
               &refEndUL
            );

         codeAryUI[uiSpoligo] += matchBl;
         retUC &= (!matchBl);
      } /*Loop: detect spoligotypes in each chunk*/
   } while(! errUC);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*reset table to original sequences*/
   lookupIndexToSeq(tblSTPtr->seqST->seqStr);
   tblSTPtr->seqST = oldSeqST;

   return retUC;
} /*fxFindSpoligos_kmerFind*/

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
