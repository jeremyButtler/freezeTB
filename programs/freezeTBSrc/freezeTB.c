/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTB SOF: Start Of File
'   - Process input reads for AMR(s) and MIRU lineages.
'     This also builds a cosenssu and prints out depths
'   o header:
'     o Included libraries
'   o .c st01: set_freezeTB
'     - has loose (not in structure) settings for freezeTB
'   o fun01: blank_set_freezeTB
'     - blanks a set_freezeTB stucture
'   o fun02: init_set_freezeTB
'     - initializes a set_freezeTB stucture
'   o fun03: freeStack_set_freezeTB
'     - frees variables in a set_freezeTB stack struct
'   o fun04: freeHeap_set_freezeTB
'     - frees a set_freezeTB stack struct
'   o .c fun05: pversion_freezeTB
'     - prints version number for freezeTB and |submodules
'   o .c fun06: phelp_freezeTB
'     - prints help message for freezeTB
'   o .c fun07: input_freezeTB
'     - gets user input
'   o fun08: run_freezeTB:
'     - drives everything, but not fun08 (for tcltk)
'   o .h note01:
'     - windows enviromental variables
'   o license:
'     - licensing for this code (public dofun08 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - defined variables/default settings
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
^   o header sec01 sub01:
^     - standard libraries
^   o header sec01 sub02:
^     - general libraries (lib/bio/aln)
^   o header sec01 sub03:
^     - libraries unique to tb analysis
^   o header sec01 sub04:
^     - libraries without .c files
^   o header sec01 sub05:
^     - hidden libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/********************************************************\
* Header Sec01 Sub01:
*   - standard libraries
\********************************************************/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

/********************************************************\
* Header Sec01 Sub02:
*   - general libraries (lib/bio/aln)
\********************************************************/

/*non-biology c libraries*/
#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"

/*general biology c libraries*/
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genBio/geneCoord.h"
#include "../genBio/trimSam.h"
#include "../genBio/maskPrim.h"
#include "../genBio/ampDepth.h"
#include "../genBio/adjCoords.h"
#include "../genBio/rmHomo.h"
#include "../genBio/tbCon.h"

/*for clustering (mixed infection detection)*/
#include "../genClust/clustST.h"
#include "../genClust/edClust.h"

#include "../genAln/alnSet.h"
#include "../genAln/kmerFind.h"

/********************************************************\
* Header Sec01 Sub03:
*   - libraries unique to tb analysis
\********************************************************/

#include "../genLib/genMath.h"

/*libraries that are modules*/
#include "../tbMiruSrc/miruTbl.h"

#include "../tbSpolSrc/spolFind.h" 
#include "../tbSpolSrc/spolST.h"

#include "../tbAmrSrc/checkAmr.h"
#include "../tbAmrSrc/amrST.h"

/*freezeTB core*/
#include "freezeTBPaths.h" /*getting default file paths*/

/********************************************************\
* Header Sec01 Sub04:
*   - libraries without .c files
\********************************************************/

#include "../genBio/tbConDefs.h" /*settings/error values*/

/*default settings*/
#include "freezeTBDefs.h"
#include "../tbAmrSrc/tbAmrDefs.h"
#include "../tbMiruSrc/tbMiruDefs.h"
#include "../tbSpolSrc/tbSpolDefs.h"/*includes defaults*/

/********************************************************\
* Header Sec01 Sub05:
*   - libraries without .c files
\********************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Header Sec01 Sub05:
!   - hidden libraries
!   o std #include <string.h>
!
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genBio/codonFun.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/edDist.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../tbAmrSrc/drugAry.h"
!
!   o .h  #include "../genLib/dataTypeShortHand.h"
!   o .h  #include "../genBio/ntTo2bit.h" 
!   o .h  #include "../genBio/revNtTo2bit.h" 
!   o .h  #include "../genBio/ntTo5bit.h" 
!   o .c  #include "../genBio/codonTbl.h"
!   o .h  #include "../genBio/kmerBit.h" 
!   o .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^    - defined variables/default settings (freezeTB only)
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_lenFileName_freezeTB 1024
signed char *def_prefix_freezeTB =
    (signed char *) "FTB_output";
signed char *def_depthFlag_freezeTB =
    (signed char *) "tb";

/*-------------------------------------------------------\
| ST01: set_freezeTB
|   - has loose (not in structure) settings for freezeTB
\-------------------------------------------------------*/
typedef struct set_freezeTB
{
   /*read filtering or modification*/
   float minMedianQF;
   float minMeanQF;
   signed char adjCoordBl; /*adjust read map coordinates*/
   
   /*AMR filtering*/
   signed char frameshiftBl;/*enable AMR frameshift scan*/
   float minPercMapF;  /*percent support for all AMRs*/
   float amrIndelSupF; /*percent support for indel AMRs*/
   float amrFrameshiftSupF; /*% support; frameshift AMRs*/
    
   /*homopolyer clean up*/
   signed char indelCleanBl;
      /*enable indel cleanup (in homopolymers)*/
   signed int minHomoLenSI;
      /*min homopolymer length to remove indel*/
   signed int maxIndelLenSI;
      /*max indel length to not remove indel*/
   signed char homoMaskSC; /*base to mask with*/

   /*lineage settings*/
   signed int fudgeSI; /*amount of fudge on MIRU linage*/
   float spolPercScoreF; /*min percent for spoligotye*/
   signed int drStartSI; /*direct repeat start; spoligo*/
   signed int drEndSI;   /*direct repeat end; spoligo*/

   /*spoligotype scan settings (user unable to set)*/
   unsigned char lenKmerUC;/*length of one kmer in scan*/
   float minKmerPercF;     /*min % kmers to align qeury*/
   float percShiftF;       /*% bases to shift window by*/
   float percExtraNtInWinF;/*% extra bases in window*/
   struct alnSet alnSetST; /*waterman settings*/

   /*consensus building*/
   struct set_tbCon tbConSet;

   /*structers with settings*/
   struct set_clustST clustSetST;   /*cluster settings*/
   signed char clustBl;    /*enable clustering*/

   /*file paths*/
   signed char amrDbFileStr[def_lenFileName_freezeTB];
      /*amr database path*/
   signed char coordFileStr[def_lenFileName_freezeTB];
      /*coordinate file path*/
   signed char miruDbFileStr[def_lenFileName_freezeTB];
      /*miru database path*/
   signed char maskPrimFileStr[def_lenFileName_freezeTB];
      /*file with masking coordinates*/
   signed char spolRefFileStr[def_lenFileName_freezeTB];
      /*references (spacers) for spoligotyping*/
   signed char spolDBFileStr[def_lenFileName_freezeTB];
      /*spoligotype lineage database*/
   signed char refFileStr[def_lenFileName_freezeTB];
      /*fasta with reference sequence*/
   signed char prefixStr[64];     /*what to name output*/
   signed char depthFlagStr[64];  /*graph prefix*/

}set_freezeTB;

/*-------------------------------------------------------\
| Fun01: blank_set_freezeTB
|   - blanks a set_freezeTB stucture
| Input:
|   - setFTBST:
|     o pointer to set_freezeTB struct to blank
| Ouput:
|   - Modifies:
|     o calls blanks functions for all internal structs
\-------------------------------------------------------*/
void
blank_set_freezeTB(
   struct set_freezeTB *setFTBST
){
   if(! setFTBST)
      return;

   blank_set_clustST(&setFTBST->clustSetST);
} /*blank_set_freezeTB*/

/*-------------------------------------------------------\
| Fun02: init_set_freezeTB
|   - initializes a set_freezeTB stucture
| Input:
|   - setFTBST:
|     o pointer to set_freezeTB struct to initialize
| Ouput:
|   - Modifies:
|     o intitializes everythning, then blanks
\-------------------------------------------------------*/
void
init_set_freezeTB(
   struct set_freezeTB *setFTBST
){
   if(! setFTBST)
      return;

   setFTBST->minMedianQF = def_minMedianQ_freezeTBDefs;
   setFTBST->minMeanQF = def_minMeanQ_freezeTBDefs;
   setFTBST->adjCoordBl = def_adjCoord_freezeTBDefs;

   /*AMR detection variables*/
   setFTBST->minPercMapF = def_minPercMapped_freezeTBDefs;
   setFTBST->frameshiftBl = def_frameshift_freezeTBDefs;
   setFTBST->amrIndelSupF = def_amrIndelSup_freezeTBDefs;
   setFTBST->amrFrameshiftSupF =
      def_supFrameshift_freezeTBDefs;

   /*initialize homopolymer clean up settings*/
   setFTBST->indelCleanBl = def_indelClean_freezeTBDefs;
   setFTBST->minHomoLenSI = def_minHomo_freezeTBDefs;
   setFTBST->maxIndelLenSI = def_maxIndel_freezeTBDefs;
   setFTBST->homoMaskSC = def_homoMask_freezeTBDefs;

   /*initialize lineage settings*/
   setFTBST->fudgeSI = def_fudgeLen_tbMiruDefs;
   setFTBST->spolPercScoreF = def_minPercScore_tbSpolDefs;
   setFTBST->drStartSI = def_DRStart_tbSpolDefs;
   setFTBST->drEndSI = def_DREnd_tbSpolDefs;

   setFTBST->lenKmerUC = def_lenKmer_kmerFind;
   setFTBST->minKmerPercF = def_minKmerPerc_kmerFind;
   setFTBST->percShiftF = def_percShift_kmerFind;
   setFTBST->percExtraNtInWinF =
      def_extraNtInWin_kmerFind;

   /*initialize settings structures*/
   init_alnSet(&setFTBST->alnSetST);
   init_set_tbCon(&setFTBST->tbConSet);
   init_set_clustST(&setFTBST->clustSetST);

   setFTBST->clustBl = def_mixedInfect_freezeTBDefs;


   /*copy database paths*/
   amrPath_freezeTBPaths(setFTBST->amrDbFileStr);
   coordPath_freezeTBPaths(setFTBST->coordFileStr);
   miruPath_freezeTBPaths(setFTBST->miruDbFileStr);
   maskPath_freezeTBPaths(setFTBST->maskPrimFileStr);
   refPath_freezeTBPaths(setFTBST->refFileStr);
   spolLineagePath_freezeTBPaths(setFTBST->spolDBFileStr);
   spolSpacerPath_freezeTBPaths(
      setFTBST->spolRefFileStr
   );

   cpStr_ulCp(
      setFTBST->depthFlagStr,
      def_depthFlag_freezeTB
   );

   cpStr_ulCp(
      setFTBST->prefixStr,
      def_prefix_freezeTB
   );


   /*there is some duplication here, but this should only
   `  be called once
   */
   blank_set_freezeTB(setFTBST);
} /*init_set_freezeTB*/

/*-------------------------------------------------------\
| Fun03: freeStack_set_freezeTB
|   - frees variables in a set_freezeTB stack struct
| Input:
|   - setFTBST:
|     o pointer to set_freezeTB struct with variables to
|       free
| Ouput:
|   - Frees:
|     o all variables in setFTBST and initializes
\-------------------------------------------------------*/
void
freeStack_set_freezeTB(
   struct set_freezeTB *setFTBST
){
   if(! setFTBST)
      return;

   freeStack_alnSet(&setFTBST->alnSetST);
   freeStack_set_tbCon(&setFTBST->tbConSet);
   freeStack_set_clustST(&setFTBST->clustSetST);
   init_set_freezeTB(setFTBST);
} /*freeStack_set_freezeTB*/

/*-------------------------------------------------------\
| Fun04: freeHeap_set_freezeTB
|   - frees a set_freezeTB stack struct
| Input:
|   - setFTBST:
|     o pointer to set_freezeTB struct to free
| Ouput:
|   - Frees:
|     o setFTBST, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_set_freezeTB(
   struct set_freezeTB *setFTBST
){
   if(! setFTBST)
      return;
   freeStack_set_freezeTB(setFTBST);
   free(setFTBST);
} /*freeHeap_set_freezeTB*/

/*-------------------------------------------------------\
| Fun05: pversion_freezeTB
|   - prints version number for freezeTB and |submodules
| Input:
|   - outFILE:
|     o file to print version numbers to
| Ouput:
|   - Prints:
|     o version numbers to outFILE
\-------------------------------------------------------*/
void
pversion_freezeTB(
   void *outFILE
){
   fprintf(
       (FILE *) outFILE,
       "freezeTB version: %i-%02i-%02i\n",
       def_year_freezeTBDefs,
       def_month_freezeTBDefs,
       def_day_freezeTBDefs
   ); /*Version of freezeTB*/
} /*pversion_freezeTB*/

/*-------------------------------------------------------\
| Fun06: phelp_freezeTB
|   - prints help message for freezeTB
| Input:
|   - pFiltBl:
|     - 1: print read filtering settings
|     - 0: do not print read flitering settings
|   - pConBl:
|     - 1: print settings for consensus building
|     - 0: do not print settings for consensus building
|   - pPrintBl:
|     - 1: print settings for variant/amr id printing
|     - 0: do not print variant/amr id print settings
|   - pClustBl:
|     - 1: print settings for clustering
|     - 0: do not print settings for clustering
|   - pLinBl:
|     - 1: print lineage settings
|     - 0: do not print lineage settings
|   - pOtherBl:
|     - 1: print other settings
|     - 0: do not print other settings
|   - outFILE:
|     o file to print help message to
| Ouput:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_freezeTB(
   signed char pFiltBl,  /*print read filtering settigs*/
   signed char pConBl,   /*consensus unqiue settings*/
   signed char pPrintBl, /*print (amr/con) settings*/
   signed char pClustBl, /*print cluster settings*/
   signed char pLinBl,   /*print lineage settings*/
   signed char pOtherBl,  /*print cluster settings*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - print help message
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - find default file paths
   '   o fun06 sec03:
   '     - usage
   '   o fun06 sec04:
   '     - input
   '   o fun06 sec05:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*for default file locations*/
   signed char amrDbFileStr[def_lenFileName_freezeTB];
   signed char miruTblFileStr[def_lenFileName_freezeTB];
   signed char coordFileStr[def_lenFileName_freezeTB];

   signed char spolRefFileStr[def_lenFileName_freezeTB];
   signed char spolDBFileStr[def_lenFileName_freezeTB];
   signed char maskPrimFileStr[def_lenFileName_freezeTB];

   signed char refFileStr[def_lenFileName_freezeTB];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - find default file paths
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrPath_freezeTBPaths(amrDbFileStr);
   miruPath_freezeTBPaths(miruTblFileStr);
   coordPath_freezeTBPaths(coordFileStr);
   spolSpacerPath_freezeTBPaths(spolRefFileStr);
   spolLineagePath_freezeTBPaths(spolDBFileStr);
   maskPath_freezeTBPaths(maskPrimFileStr);
   refPath_freezeTBPaths(refFileStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   fprintf(
      (FILE *) outFILE,
      "freezeTB -sam reads.sam -prefix %s\n",
      def_prefix_freezeTB
   );

   fprintf(
      (FILE *) outFILE,
      "  - tuberculosis AMR, spoligotype, and MIRU VNTR\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    analysis pipeline (for ONT Nanopore reads)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - NOT CLIA CERTIFIED; RESEARCH USE ONLY\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - input entry
   ^   o fun06 sec04 sub01:
   ^     - input header/File IO input
   ^   o fun06 sec04 sub02:
   ^     - fitering settings
   ^   o fun06 sec04 sub03:
   ^     - indel clean up (rmHomo) settings
   ^   o fun06 sec04 sub04:
   ^     - consensus settings
   ^   o fun06 sec04 sub05:
   ^     - clustering settings
   ^   o fun06 sec04 sub06:
   ^     - variant/read AMR printing settings
   ^   o fun06 sec04 sub07:
   ^     - AMR settings
   ^   o fun06 sec04 sub08:
   ^     - lineage (non-database) settings
   ^   o fun06 sec04 sub09:
   ^     - other settings and help message/version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec04 Sub01:
   *   - input header/File IO input
   *   o fun06 sec04 sub01 cat01:
   *     - input header
   *   o fun06 sec04 sub01 cat02:
   *     - sam file input
   *   o fun06 sec04 sub01 cat03:
   *     - prefix to name output files
   *   o fun06 sec04 sub01 cat04:
   *     - AMR table input
   *   o fun06 sec04 sub01 cat05:
   *     - gene coordinates
   *   o fun06 sec04 sub01 cat06:
   *     - MIRU table input
   *   o fun06 sec04 sub01 cat07:
   *     - spoligotype spacer sequence input
   *   o fun06 sec04 sub01 cat08:
   *     - spoligotype linage database
   *   o fun06 sec04 sub01 cat09:
   *     - primer masking database
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat01:
   +   - input header and FILE IO header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  File IO: \n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat02:
   +   - sam file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -sam reads.sam: [Required; stdin]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o sam file with reads mapped to NC000962\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat03:
   +   - prefix to name output files
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -prefix %s: [Optional; %s]\n",
      def_prefix_freezeTB,
      def_prefix_freezeTB
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix to name all output files with\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat04:
   +   - AMR table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(amrDbFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -amr-tbl amr-catalog.tsv: [Required]\n"
      );

   else
      fprintf(
         (FILE *) outFILE,
         "    -amr-tbl amr-catalog.tsv: [Using %s]\n",
         amrDbFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o AMR mutation database for AMRs detection\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat05:
   +   - gene coordinates
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(coordFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -gene-coords coordinates.tsv: [Required]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -gene-coords coordinates.tsv: [Using %s]\n",
         coordFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o gene coordinate table (for NC000962.3)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat06:
   +   - MIRU table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(miruTblFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -miru-tbl MIRU-tbl.tsv: [Required]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -miru-tbl MIRU-tbl.tsv: [Using %s]\n",
         miruTblFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o miru table (tbMiru) to detect lineages"
   );

   fprintf(
      (FILE *) outFILE,
      " with\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat07:
   +   - spoligotype spacer sequence input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spolRefFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Required]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Using %s]\n",
         spolRefFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o fasta file with direct repeat spacer\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        sequences for spoligotyping\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat08:
   +   - spoligotype linage database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spolDBFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Required]\n"
      );

   else
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Using %s]\n",
         spolDBFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o csv file with spoligotype lineages\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o format: strain, barcode, ignore,\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        lineage, SIT, countries\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat09:
   +   - primer masking database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(maskPrimFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -mask-prim primer-mask.tsv: [Optional]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -mask-prim primer-mask.tsv: [Using %s]\n",
         maskPrimFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o tsv file with primer positions to mask\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-prim-mask -\" to disable\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o format: ignored\tigonored\t0"
   );

   fprintf(
      (FILE *) outFILE,
      "\tforward_start\tforward_end"
   );

   fprintf(
      (FILE *) outFILE,
      "\treverse_start\treverse_end\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub01 Cat10:
   +   - reference for indel clean up
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(maskPrimFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -ref reference.fasta: [Optional]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -ref reference.fasta: [using %s]\n",
         refFileStr
      );

   fprintf(
      (FILE *) outFILE,
      "      o reference to use in indel clean up\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub02:
   *   - fitering settings
   *   o fun06 sec04 sub02 cat01:
   *     - filter header
   *   o fun06 sec04 sub02 cat02:
   *     - mapping quality filter
   *   o fun06 sec04 sub02 cat03:
   *     - min length filter
   *   o fun06 sec04 sub02 cat04:
   *     - mean Q-score filter
   *   o fun06 sec04 sub02 cat05:
   *     - median Q-score filter
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat01:
   +   - fitering header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pFiltBl)
      goto skipReadFilt_fun06_sec04_sub04;

   fprintf(
      (FILE *) outFILE,
      "  Read Filtering:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat02:
   +   - mapping quality filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mapq %u: [Optional; %u]\n",
      def_minMapq_tbConDefs,
      def_minMapq_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mapping quality to keep a read\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat03:
   +   - min length filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]\n",
      def_minLen_tbConDefs,
      def_minLen_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum alinged length keep a read\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat04:
   +   - mean q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mean-q %i: [Optional; %i]\n",
      def_minMeanQ_freezeTBDefs,
      def_minMeanQ_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mean q-score to keep a read\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat05:
   +   - median Q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-median-q %i: [Optional; %i]\n",
      def_minMedianQ_freezeTBDefs,
      def_minMedianQ_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum median q-score to keep a read\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub02 Cat06:
   +   - adjust coordinates
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_adjCoord_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -adj-coords: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -adj-coords: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "      o adjust reads to reference coordinates in\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        \"-gene-coords coords.tsv\"\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-no-adj-coords\" to disable\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub03:
   *   - indel clean up (rmHomo) settings
   *   o fun06 sec04 sub03 cat01:
   *     - rmHomo header
   *   o fun06 sec04 sub03 cat02:
   *     - indel clean up boolean
   *   o fun06 sec04 sub03 cat03:
   *     - indel clean up minimum homopolymer size
   *   o fun06 sec04 sub03 cat04:
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub03 Cat01:
   +   - rmHomo header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  Indel cleanup:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub03 Cat02:
   +   - indel clean up boolean
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_indelClean_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -rmHomo: [enabled]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -rmHomo: [disabled]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "      o remove indels in homoplyers\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-rmHomo\"\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub03 Cat03:
   +   - indel clean up minimum homopolymer size
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -rmHomo-homo %i: [%i]\n",
      def_minHomo_freezeTBDefs,
      def_minHomo_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum homopolymer size to remove indel\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub03 Cat04:
   +   - indel clean up maximum indel size
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -rmHomo-indel %i: [%i]\n",
      def_maxIndel_freezeTBDefs,
      def_maxIndel_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum indel size to remove\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub04:
   *   - consensus settings
   *   o fun06 sec04 sub04 cat01:
   *     - consensus setting header
   *   o fun06 sec04 sub04 cat02:
   *     - min depth filter
   *   o fun06 sec04 sub04 cat03:
   *     - min length to keep a fragment
   *   o fun06 sec04 sub04 cat04:
   *     - min snp/match Q-score
   *   o fun06 sec04 sub04 cat05:
   *     - min insertion Q-score
   *   o fun06 sec04 sub04 cat06:
   *     - min percdent snp/match support
   *   o fun06 sec04 sub04 cat07:
   *     - min percdent insertion support
   *   o fun06 sec04 sub04 cat08:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat01:
   +   - consensus setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipReadFilt_fun06_sec04_sub04:;

   if(! pConBl)
       goto skipConSet_fun06_sec04_sub05_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Consensus settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat02:
   +   - min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-depth %i: [Optional; %i]\n",
      def_minDepth_tbConDefs,
      def_minDepth_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to keep a base\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat03:
   +   - min length to keep a fragment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]\n",
      def_minLen_tbConDefs,
      def_minLen_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum length to keep consensus fragment"
   );

   fprintf(
      (FILE *) outFILE,
      "\n"
   );


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat04:
   +   - min snp/match Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-q %i: [Optional; %i]\n",
      def_minNtQ_tbConDefs,
      def_minNtQ_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum Q-score to keep an match or snp\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat05:
   +   - min insertion Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-q-ins %i: [Optional; %i]\n",
      def_minInsQ_tbConDefs,
      def_minInsQ_tbConDefs
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum Q-score to keep an inserted base\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat06:
   +   - min percdent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-snp-sup %.2f: [Optional; %.2f]\n",
      def_minSnpPerc_tbConDefs,
      def_minSnpPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% mapped reads to keep a snp\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat07:
   +   - min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-ins-sup %.2f: [Optional; %.2f]\n",
      def_minInsPerc_tbConDefs,
      def_minInsPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% mapped reads to keep an ins\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub04 Cat08:
   +   - min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-del-sup %.2f: [Optional; %.2f]\n",
      def_minDelPerc_tbConDefs,
      def_minDelPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% of mapped reads to keep a del\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub05:
   *   - clustering settings
   *   o fun06 sec04 sub05 cat01:
   *     - print clustering setting header
   *   o fun06 sec04 sub05 cat02:
   *     - read scoring settings
   *   o fun06 sec04 sub05 cat03:
   *     - minimum cluster depth
   *   o fun06 sec04 sub05 cat04:
   *     - minimum cluster percent depth
   *   o fun06 sec04 sub05 cat05:
   *     - minimum percent difference between reads
   *   o fun06 sec04 sub05 cat06:
   *     - minimum consensus to read percent difference
   *   o fun06 sec04 sub05 cat07:
   *     - maximum percent similarity between consensuses
   *   o fun06 sec04 sub05 cat08:
   *     - minimum percent overlap between consensuses
   *   o fun06 sec04 sub05 cat09:
   *     - maximum percent maksing in consensus
   *   o fun06 sec04 sub05 cat10:
   *     - number of consensus rebuilds
   *   o fun06 sec04 sub05 cat11:
   *     - depth profiling (edit distance setting start)
   *   o fun06 sec04 sub05 cat12:
   *     - minimum depth profile depth (edDist)
   *   o fun06 sec04 sub05 cat13:
   *     - minimum error:variant ratio to be different
   *   o fun06 sec04 sub05 cat14:
   *     - length of window (edDist)
   *   o fun06 sec04 sub05 cat15:
   *     - window variant:error ratio (for difference)
   *   o fun06 sec04 sub05 cat16:
   *     - minimum indel length to keep indel in edDist
   *   o fun06 sec04 sub05 cat17:
   *     - minimum q-score to count snp as different
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat01:
   +   - print clustering setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipConSet_fun06_sec04_sub05_cat01:;

   if(! pClustBl)
      goto skipClustSet_fun06_sec04_sub06_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Clustering Settings:\n"
   );

   if(def_mixedInfect_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -clust: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -clust: [Optional; No]\n"
      );
 
   fprintf(
      (FILE *) outFILE,
      "      o detect mixed infections; no stdin input\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-clust\"\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat02:
   +   - read scoring settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    Read Scoring:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      -len-weight %0.2f: [Optional; %0.2f]\n",
      def_lenWeight_clustST,
      def_lenWeight_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o how much to increase lengths impact on\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          read scoring\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          o log_length = floor[log10(length)]\n"
   );

   fprintf(
    (FILE *) outFILE,
    "        o length_score= floor[weigth * log_length]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        o score = length_score + floor[median Q]\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat03:
   +   - minimum cluster depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    General Clustering:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      -clust-depth %i: [Optional; %i]\n",
      def_minDepth_clustST,
      def_minDepth_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o min cluster read depth to keep cluster\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat04:
   +   - minimum cluster percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "      -clust-perc-depth %.04f: [Optional; %.04f]\n",
     def_minPercDepth_clustST,
     def_minPercDepth_clustST
   );

   fprintf(
     (FILE *) outFILE,
     "        o minimum percent read depth need to keep\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          a cluster\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat05:
   +   - minimum percent difference between reads
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -read-err %0.3f: [Optional; %0.3f]\n",
      def_readErrRate_clustST,
      def_readErrRate_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o error rate for read to read mapping\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat06:
   +   - minimum consensus to read percent difference
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-err %0.3f: [Optional; %0.3f]\n",
      def_conErrRate_clustST,
      def_conErrRate_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o consensus to read mapping error rate\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat07:
   +   - maximum percent similarity between consensuses
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-sim %0.4f: [Optional; %0.4f]\n",
      def_maxConSim_clustST,
      def_maxConSim_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum simularity between consensuses\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          to not merge clusters\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat08:
   +   - minimum percent overlap between consensuses
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -overlap %0.2f: [Optional; %0.2f]\n",
      def_percOverlap_clustST,
      def_percOverlap_clustST
   );

   fprintf(
     (FILE *) outFILE,
     "        o minimum percentage of overlap needed to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "       merge clusters (start comparision)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        o used in final consensus comparison\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat09:
   +   - maximum percent maksing in consensus
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -perc-n %0.4f: [Optional; %0.4f]\n",
      def_maxNPerc_clustST,
      def_maxNPerc_clustST
    );

   fprintf(
      (FILE *) outFILE,
      "      o maximum percent masked consnesus bases\n"
   ); /*both consensus and edit distance*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat10:
   +   - number of consensus rebuilds
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-iter %i: [Optional; %i]\n",
      def_conRebuild_clustST,
      def_conRebuild_clustST
    );

   fprintf(
      (FILE *) outFILE,
      "      o number times to rebuild cluster (con)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat11:
   +   - depth profiling (edit distance setting start)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    Edit Distance:\n"
   );

   if(def_depthProfile_clustST)
      fprintf(
         (FILE *) outFILE,
         "      -depth-prof: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "      -depth-prof: [Optional; No]\n"
      );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat12:
   +   - minimum depth profile depth (edDist)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "        o -clust-depth to remove low read depth\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          variants from edit distance\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        o disable with \"no-depth-prof\"\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat13:
   +   - minimum error:variant ratio to be different
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -err-to-var %i: [Optional; %i]\n",
      def_maxVarErrRatio_clustST,
      def_maxVarErrRatio_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum variant to error ratio to add\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          read to a cluster (whole number)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        o 100*(# variants)/(error rate * length)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat14:
   +   - length of window (edDist)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -win-len %i: [Optional; %i]\n",
      def_window_clustST,
      def_window_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o size of window for window scanning\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        o idea is to detect regions of high\n"
   );

   fprintf(
      (FILE *) outFILE,
      "          difference in very long reads\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat15:
   +   - window variant:error ratio (for difference)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -win-err %i: [Optional; %i]\n",
      def_windowError_clustST,
      def_windowError_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum window variant to error ratio\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat16:
   +   - minimum indel length to keep indel in edDist
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -indel-len %i: [Optional; %i]\n",
      def_indelLen_clustST,
      def_indelLen_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o min indel length to count as different\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub05 Cat17:
   +   - minimum q-score to count snp as different
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -clust-q-snp %i: [Optional; %i]\n",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "        o minimum q-score to keep snp; edit dist\n"
   ); /*edit distance min snp q-score*/

   /*****************************************************\
   * Fun06 Sec04 Sub06:
   *   - variant printing settings (depth is AMR)
   *   o fun06 sec04 sub06 cat01:
   *     - variant/readAMR print setting header
   *   o fun06 sec04 sub06 cat02:
   *     - min depth filter
   *   o fun06 sec04 sub06 cat03:
   +     - min percent of reads to keep read AMR
   *   o fun06 sec04 sub06 cat04:
   *     - min perccent snp/match support
   *   o fun06 sec04 sub06 cat05:
   *     - min percent insertion support
   *   o fun06 sec04 sub06 cat06:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub06 Cat01:
   +   - variant/readAMR print setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipClustSet_fun06_sec04_sub06_cat01:;

   if(! pPrintBl)
      goto skipPrintSet_fun06_sec04_sub07_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Print settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub06 Cat02:
   +   - print min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-min-depth %i: [Optional; %i]\n",
      def_minPrintDepth_tbConDefs,
      def_minPrintDepth_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o for variant tsv; mininum read depth to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        print variant\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub06 Cat04:
   +   - min percent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-snp-sup %.2f: [Optional; %.2f]\n",
      def_minSnpPrintPerc_tbConDefs,
      def_minSnpPrintPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv; min %% of reads to print\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        snp/match\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub06 Cat05:
   +   - min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-ins-sup %.2f: [Optional; %.2f]\n",
      def_minInsPrintPerc_tbConDefs,
      def_minInsPrintPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv;min %% of reads to print ins\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub06 Cat06:
   +   - min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-del-sup %.2f: [Optional: %.2f]\n",
      def_minDelPrintPerc_tbConDefs,
      def_minDelPrintPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv;min %% of reads to print del\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub07:
   *   - AMR settings
   *   o fun06 sec04 sub07 cat01:
   *     - min percent of reads to keep read AMR
   *   o fun06 sec04 sub07 cat02:
   *     - min percent of reads to keep indel
   *   o fun06 sec04 sub07 cat03:
   *     - checking frameshifts
   *   o fun06 sec04 sub07 cat04:
   *     - frameshift support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub07 Cat01:
   +   - min percent of reads to keep read AMR
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-amr-map-perc %.2f: [Optional; %.2f]\n",
      def_minPercMapped_freezeTBDefs,
      def_minPercMapped_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o read AMRs; min %% of mapped reads to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        print an AMR\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub07 Cat02:
   +   - min percent of reads to keep indel
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -amr-indel-sup %.2f: [Optional; %.2f]\n",
      def_amrIndelSup_freezeTBDefs,
      def_amrIndelSup_freezeTBDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent to keep indel AMR\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub07 Cat03:
   +   - checking frameshifts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_frameshift_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -frameshift: [Optional; Yes]\n"
         );
   else
      fprintf(
         (FILE *) outFILE,
         "    -frameshift: [Optional; No]\n"
         );

   fprintf(
      (FILE *) outFILE,
      "      o off; frame shift AMRs are extact matches\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable: with -no-frameshift (recommend)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub07 Cat04:
   +   - frameshift support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -frameshift-sup %0.2f: [Optional; %0.2f]\n",
       def_supFrameshift_freezeTBDefs,
       def_supFrameshift_freezeTBDefs
    );

   fprintf(
      (FILE *) outFILE,
      "      o minimum support to keep a frameshift AMR\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub08:
   *   - lineage settings
   *   o fun06 sec04 sub08 cat01:
   *     - lineage setting block
   *   o fun06 sec04 sub08 cat02:
   *     - MIRU fudge setting
   *   o fun06 sec04 sub08 cat03:
   *     - spoligtyping min perc score
   *   o fun06 sec04 sub08 cat04:
   *     - spoligtyping dr-start
   *   o fun06 sec04 sub08 cat05:
   *     - spoligtyping dr-end
   \*****************************************************/

   skipPrintSet_fun06_sec04_sub07_cat01:;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub08 Cat01:
   +   - lineage setting block
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pLinBl)
      goto skipLineageSet_fun06_sec04_sub09;

   fprintf(
      (FILE *) outFILE,
      "  Lineage settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub08 Cat02:
   +   - MIRU fudge setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fudge %i: [Optional; %i]\n",
      def_fudgeLen_tbMiruDefs,
      def_fudgeLen_tbMiruDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o fudge read lengths by to match a lineage\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o read length range is + or - fudge\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub08 Cat03:
   +   - spoligtyping min perc score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -spoligo-min-score %.2f: [Optional; %.2f]\n",
      def_minPercScore_tbSpolDefs,
      def_minPercScore_tbSpolDefs
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum percent score needed to count a\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        spoligo spaceer as mapped\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub08 Cat04:
   +   - spoligtyping dr-start
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -dr-start %i: [Optional; %i]\n",
      def_DRStart_tbSpolDefs,
      def_DRStart_tbSpolDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o start of reference direct repeat region\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub08 Cat05:
   +   - spoligtyping dr-end
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -dr-end %i: [Optional; %i]\n",
      def_DREnd_tbSpolDefs,
      def_DREnd_tbSpolDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o end of reference direct repeat region\n"
   );

   /*****************************************************\
   * Fun06 Sec04 Sub09:
   *   - other settings and help message/version number
   *   o fun06 sec04 sub09 cat01:
   *     - other settings header
   *   o fun06 sec04 sub09 cat02:
   *     - flag for graph
   *   o fun06 sec04 sub09 cat03:
   *     - default help message
   *   o fun06 sec04 sub09 cat04:
   *     - addons for help message
   *   o fun06 sec04 sub09 cat05:
   *     - version number
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub09 Cat01:
   +   - Other settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipLineageSet_fun06_sec04_sub09:;

   fprintf(
      (FILE *) outFILE,
      "  Other settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub09 Cat02:
   +   - flag for graph
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pOtherBl)
      goto skipOtherSet_fun06_sec04_sub09_cat04;
   
   fprintf(
      (FILE *) outFILE,
      "    -depth-flag %s: [Optional; %s]\n",
      def_depthFlag_freezeTB,
      def_depthFlag_freezeTB
   );

   fprintf(
      (FILE *) outFILE,
      "      o label for experiment in depth tsv/graphs\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o do not use spaces or tabs\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub09 Cat03:
   +   - default help message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipOtherSet_fun06_sec04_sub09_cat04:;

   if(
         pFiltBl
      || pConBl
      || pPrintBl
      || pClustBl
      || pLinBl
      || pOtherBl
   ){ /*If: not default help message*/
      fprintf(
         (FILE *) outFILE,
         "    -h: default help message and exit\n"
      );
   } /*If: not default help message*/

   else
   { /*Else: not default help message*/
      fprintf(
         (FILE *) outFILE,
         "    -h: this help message and exit\n"
      );
   } /*Else: not default help message*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub09 Cat04:
   +   - addons for help message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pFiltBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-filt: add consensus settings to -h\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -h-filt: print this help message and exit\n"
      );

   if(pConBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-con: add consensus settings to -h\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -h-con: print this help message and exit\n"
      );

   if(pLinBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-lin: add lineage settings to -h\n"
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-lin: print this help message and exit\n"
      );


   if(pPrintBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-print: add print settings to -h\n"
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-print: print this help message and exit\n"
      );

   if(pClustBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-clust: add cluster settings to -h\n"
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-clust: print this help message and exit\n"
      );

   if(pOtherBl)
      fprintf(
        (FILE *) outFILE,
        "    -h-other: add uncatogorized settings to -h\n"
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-other: print this help message and exit\n"
      );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec04 Sub09 Cat07:
   +   - version number
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "    -v: print version number of freezeTB and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec05:
   ^   - output
   ^   o fun06 sec05 sub01:
   ^     - output header
   ^   o fun06 sec05 sub02:
   ^     - consensus amrs file
   ^   o fun06 sec05 sub04:
   ^     - consensus mirufile
   ^   o fun06 sec05 sub05:
   ^     - consensus file
   ^   o fun06 sec05 sub06:
   ^     - variants file
   ^   o fun06 sec05 sub07:
   ^     - reads amr table
   ^   o fun06 sec05 sub08:
   ^     - reads ids of AMR reads
   ^   o fun06 sec05 sub09:
   ^     - reads MIRU table
   ^   o fun06 sec05 sub09:
   ^     - filtered histogram file
   ^   o fun06 sec05 sub10:
   ^     - unfiltered histogram file
   ^   o fun06 sec05 sub11:
   ^     - clusters with reads
   ^   o fun06 sec05 sub12:
   ^     - log (for mixed infect)
   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec05 Sub01:
   *   - output header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub02:
   *   - consensus amrs file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-con-amrs.tsv\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with detected consensus AMRs\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub04:
   *   - consensus mirufile
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-con-miru.tsv\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with detected consensus MIRU lineage\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub05:
   *   - consensus file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-consensuses.sam \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with consensus sequences\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub06:
   *   - variants file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-consensuses.tsv \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with variants for each consensus base\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub07:
   *   - reads amr table
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-read-amrs.tsv \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with AMRs found in the reads\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub08:
   *   - reads ids of AMR reads
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-id-amrs.tsv \n"
   );

   fprintf(
     (FILE *) outFILE,
     "    o tsv file with ids of reads that have AMRs\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub09:
   *   - reads MIRU table
   \*****************************************************/


   fprintf(
      (FILE *) outFILE,
      "  - prefix-miru-read-tbl.tsv \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with MIRU read count table\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub09:
   *   - filtered histogram file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-depths-filter.tsv \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with mean read depths per gene\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      after read filtering step\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub10:
   *   - unfiltered histogram file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-depths.tsv \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with mean read depths per gene\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      before read filtering step\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub11:
   *   - clusters with reads
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-reference-number.sam \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with clusters (mixed infect)\n"
   );

   /*****************************************************\
   * Fun06 Sec05 Sub12:
   *   - log (for mixed infect)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-log.txt \n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o log for mixed infection detection step\n"
   );
} /*phelp_freezeTB*/

/*-------------------------------------------------------\
| Fun07: input_freezeTB
|   - gets user input
| Input:
|   User Input:
|     - numArgsSI:
|       o number of arguments user input
|     - argAryStr:
|       o pointer to array of c-strings with users input
|     - samFileStr:
|       o pointer to c-string to point to input sam file
|     - ftbSetSTPtr:
|       o pointer to set_freezeTB structure with settings
|         to modify
| Output:
|   - Modifies:
|     o each input variable in ftbSetSTPtr the user input
|   - Prints:
|     o help message anc verson number requests to stdout
|     o errors to stderr
|   - Returns:
|     o 0 for success
|     o 1 if printed the help message or verion number
|     o 2 for errors
\-------------------------------------------------------*/
int
input_freezeTB(
   int numArgsSI,
   const char *argAryStr[],
   signed char **samFileStr,
   struct set_freezeTB *ftbSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC: input_freezeTB
   '   - gets user input
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - check if have user input
   '   o fun07 sec03:
   '     - get user input
   '   o fun07 sec04:
   '     - return error type
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;
   signed char errSC = 0;
   signed char helpBl = 0; /*print help message at end*/

   /*booleans for extra help message entries*/
   signed char pFiltHelpBl = 0;
   signed char pConHelpBl = 0;
   signed char pPrintHelpBl = 0;
   signed char pClustHelpBl = 0;
   signed char pLinHelpBl = 0;
   signed char pOtherHelpBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - check if have user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing was input*/
      phelp_fun07_sec02:;

      phelp_freezeTB(
         pFiltHelpBl,
         pConHelpBl,
         pPrintHelpBl,
         pClustHelpBl,
         pLinHelpBl,
         pOtherHelpBl,
         stdout
      );

      goto phelp_fun07_sec04;
   } /*If: nothing was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - get user input
   ^   o fun07 sec03 sub01:
   ^     - file input + start loop
   ^   o fun07 sec03 sub02:
   ^     - consensus options
   ^   o fun07 sec03 sub03:
   ^     - printing settings (for consensus tsv)
   ^   o fun07 sec03 sub04:
   ^     - indel clean up settings
   ^   o fun07 sec03 sub05:
   ^     - tbAmr settings
   ^   o fun07 sec03 sub06:
   ^     - read filtering + adjust coordinates
   ^   o fun07 sec03 sub07:
   ^     - lineages and graph output
   ^   o fun07 sec03 sub08:
   ^     - cluster settings
   ^   o fun07 sec03 sub09:
   ^     - check for help message requests (normal)
   ^   o fun07 sec03 sub10:
   ^     - check for add on help message requests
   ^   o fun07 sec03 sub11:
   ^     - check for version number requests
   ^   o fun07 sec03 sub12:
   ^     - invalid input
   ^   o fun07 sec03 sub13:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec03 Sub01:
   *   - file input + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: read in user input*/
      if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-amr-tbl",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: amr database input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->amrDbFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: amr database input*/

      else if(
         ! eql_charCp(
            (signed char *) "-gene-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: gene coordiantes table input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->coordFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: gene coordiantes table input*/

      else if(
         ! eql_charCp(
            (signed char *) "-miru-tbl",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: miru table input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->miruDbFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: miru table input*/

      else if(
         ! eql_charCp(
            (signed char *) "-spoligo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: spoligotype spacers input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->spolRefFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: spoligotype spacers input*/

      else if(
         ! eql_charCp(
            (signed char *) "-db-spoligo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: spoligo lineage database input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->spolDBFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: spoligo lineage database input*/

      else if(
         ! eql_charCp(
            (signed char *) "-mask-prim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: primer masking file input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->maskPrimFileStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: primer masking file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-prefix",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: prefix input*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->prefixStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: prefix input*/

      /**************************************************\
      * Fun07 Sec03 Sub02:
      *   - consensus options
      *   o fun07 sec03 sub02 cat01:
      *     - consensus q-score settinngs
      *   o fun07 sec03 sub02 cat02:
      *     - consensus min depth/length settings
      *   o fun07 sec03 sub02 cat03:
      *     - consensus min base percent support settings
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub02 Cat01:
      +    - consensus q-score settinngs
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum q-score input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minQSI
            );

         if(tmpStr[0] != '\0')
         { /*If: had an error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-q %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: had an error*/

         if(
               ftbSetSTPtr->tbConSet.minQSI > 93
            || ftbSetSTPtr->tbConSet.minQSI < 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-q %s; not between 0 and 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum q-score input*/


      else if(
         ! eql_charCp(
            (signed char *) "-min-q-ins",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum insertion q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minInsQSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-q-ins %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(
               ftbSetSTPtr->tbConSet.minInsQSI > 93
            || ftbSetSTPtr->tbConSet.minInsQSI < 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-q-ins %s; not between 0 and 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum insertion q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub02 Cat02:
      +    - consensus min depth/length settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minDepthSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minDepthSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-depth; must be greater than 0\n"
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum depth*/


      else if(
         ! eql_charCp(
            (signed char *) "-min-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum fragment length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minLenSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-len %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minLenSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-len; must be greater than 0\n"
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum fragment length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub02 Cat03:
      +    - consensus min base percent support settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*atof reurns 0 for failure*/
      else if(
         ! eql_charCp(
            (signed char *) "-perc-snp-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      )
      { /*If: minimum % snp support*/
         ++siArg;
         ftbSetSTPtr->tbConSet.minPercSnpF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.minPercSnpF < 0
            || ftbSetSTPtr->tbConSet.minPercSnpF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-snp-sup %s; not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*If: minimum % snp support*/

      else if(
         ! eql_charCp(
            (signed char *) "-perc-ins-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum % ins support*/
         ++siArg;
         ftbSetSTPtr->tbConSet.minPercInsF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.minPercInsF < 0
            || ftbSetSTPtr->tbConSet.minPercInsF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-ins-sup %s; not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum % ins support*/

      else if(
         ! eql_charCp(
            (signed char *) "-perc-del-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum % del support*/
         ++siArg;
         ftbSetSTPtr->tbConSet.minPercDelF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.minPercDelF < 0
            || ftbSetSTPtr->tbConSet.minPercDelF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-del-sup %s; not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum % del support*/

      /**************************************************\
      * Fun07 Sec03 Sub03:
      *   - printing settings (for consensus tsv)
      \**************************************************/

      /*settings for printing out variations*/
      else if(
         ! eql_charCp(
            (signed char *) "-p-min-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: print minimum depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minPrintDepthSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-p-min-depth %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minPrintDepthSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-p-min-depth %s; must be at least 0\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: print minimum depth*/

      /*print percentage depths*/
      else if(
         ! eql_charCp(
            (signed char *) "-p-perc-snp-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: print minimum % snp support*/
         ++siArg;

         ftbSetSTPtr->tbConSet.printMinSupSnpF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.printMinSupSnpF < 0
            || ftbSetSTPtr->tbConSet.printMinSupSnpF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
              stderr,
              "-p-perc-snp-sup %s; not between 0 and 1\n",
              argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: print minimum % snp support*/

      else if(
         ! eql_charCp(
            (signed char *) "-p-perc-ins-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: print minimum % ins support*/
         ++siArg;

         ftbSetSTPtr->tbConSet.printMinSupInsF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.printMinSupInsF < 0
            || ftbSetSTPtr->tbConSet.printMinSupInsF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
              stderr,
              "-p-perc-ins-sup %s; not between 0 and 1\n",
              argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: print minimum % ins support*/

      else if(
         ! eql_charCp(
            (signed char *) "-p-perc-del-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: print minimum % snp support*/
         ++siArg;

         ftbSetSTPtr->tbConSet.printMinSupDelF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->tbConSet.printMinSupDelF < 0
            || ftbSetSTPtr->tbConSet.printMinSupDelF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
              stderr,
              "-p-perc-del-sup %s; not between 0 and 1\n",
              argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: print minimum % snp support*/

      /**************************************************\
      * Fun07 Sec03 Sub04:
      *   - indel clean up
      *   o fun07 sec03 sub04 cat01:
      *     - do indel clean up
      *   o fun07 sec03 sub04 cat02:
      *     - indel clean up min hompolymer length
      *   o fun07 sec03 sub04 cat03:
      *     - indel clean up maximum indel length
      *   o fun07 sec03 sub04 cat04:
      *     - indel clean up reference
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat02:
      +   - do indel clean up
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-rmHomo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->indelCleanBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-rmHomo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->indelCleanBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat02:
      +   - indel clean up min hompolymer length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-rmHomo-homo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum homopolymer length*/
         ftbSetSTPtr->indelCleanBl = 1;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->minHomoLenSI
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-rmHomo-homo %s; non-numeric/to large\n",
               argAryStr[siArg]
            );
            goto err_fun07_sec04;
         } /*If: non-numeric*/

         else if(
            ftbSetSTPtr->minHomoLenSI <= 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-rmHomo-homo %s must be greater than 0\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      }  /*Else If: minimum homopolymer length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat03:
      +   - indel clean up maximum indel length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-rmHomo-indel",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: maximum indel length*/
         ftbSetSTPtr->indelCleanBl = 1;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->maxIndelLenSI
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-rmHomo-indel %s; non-numeric/to large\n",
               argAryStr[siArg]
            );
            goto err_fun07_sec04;
         } /*If: non-numeric*/

         else if(
            ftbSetSTPtr->maxIndelLenSI <= 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-rmHomo-indel %s must be at least 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      }  /*Else If: maximum indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub04 Cat04:
      +   - indel clean up reference
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: reference for clean up*/
         ++siArg;
         cpStr_ulCp(
            ftbSetSTPtr->refFileStr,
            (signed char *) argAryStr[siArg]
         );
      }  /*Else If: reference for clean up*/

      /**************************************************\
      * Fun07 Sec03 Sub05:
      *   - AMR settings
      *   o fun07 sec03 sub05 cat01:
      *     - minimum AMR mapping percent
      *   o fun07 sec03 sub05 cat02:
      *     - minimum indel percent support
      *   o fun07 sec03 sub05 cat03:
      *     - frameshift setting/filtering
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat01:
      +   - minimum AMR mapping percent
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-amr-map-perc",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: AMR min percent mapped reads*/
         ++siArg;
         ftbSetSTPtr->minPercMapF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->minPercMapF < 0
            || ftbSetSTPtr->minPercMapF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
             stderr,
             "-min-amr-map-per %s; not between 0 and 1\n",
             argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: AMR min percent mapped reads*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat02:
      +   - minimum indel percent support
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-amr-indel-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min indel % support*/
         ++siArg;
         ftbSetSTPtr->amrIndelSupF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->amrIndelSupF < 0
            || ftbSetSTPtr->amrIndelSupF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
             stderr,
             "-amr-indel-sup %s; not between 0 and 1\n",
             argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      }  /*Else If: min indel % support*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub05 Cat03:
      +   - frameshift setting/filtering
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-frameshift",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->frameshiftBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-frameshift",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->frameshiftBl = 0;

      else if(
         ! eql_charCp(
            (signed char *) "-frameshift-sup",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min % frameshift support*/
         ftbSetSTPtr->frameshiftBl = 1;
         ++siArg;
         ftbSetSTPtr->amrFrameshiftSupF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->amrFrameshiftSupF < 0
            || ftbSetSTPtr->amrFrameshiftSupF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
             stderr,
             "-frameshift-sup %s; not between 0 and 1\n",
             argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      }  /*Else If: min % frameshift support*/


      /**************************************************\
      * Fun07 Sec03 Sub06:
      *   - read filterting + adjust coordinates
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-min-mapq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      )
      { /*Else If: minimum mapping quality*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
               tmpStr,
               &ftbSetSTPtr->tbConSet.minMapqUC
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minMapqUC > 93)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-mapq %s; not between 0 and 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum mapping quality*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-median-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min median q-score*/
         ++siArg;
         ftbSetSTPtr->minMedianQF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->minMedianQF < 0
            || ftbSetSTPtr->minMedianQF > 93
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-median-q %s; not between 0 and 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: min median q-score*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-mean-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min mean q-score*/
         ++siArg;
         ftbSetSTPtr->minMeanQF = atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->minMeanQF < 0
            || ftbSetSTPtr->minMeanQF > 93
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-min-mean-q %s; not between 0 and 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: min mean q-score*/

      else if(
         ! eql_charCp(
            (signed char *) "-adj-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->adjCoordBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-adj-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->adjCoordBl = 0;

      /**************************************************\
      * Fun07 Sec03 Sub07:
      *   - lineages and graph output
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-depth-flag",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: graph flag (experiment name)*/
         ++siArg;

         cpStr_ulCp(
            ftbSetSTPtr->depthFlagStr,
            (signed char *) argAryStr[siArg]
         );
      } /*Else If: graph flag (experiment name)*/

      else if(
         ! eql_charCp(
            (signed char *) "-fudge",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: fudge length (MIRU VNTR)*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->fudgeSI
            );
   
         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-fudge %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(
               ftbSetSTPtr->fudgeSI < 0
            || ftbSetSTPtr->fudgeSI > 100
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-fudge %s; must be between 0 and 100\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: fudge length (MIRU VNTR)*/

      else if(
         ! eql_charCp(
            (signed char *) "-spoligo-min-score",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min % score for spacer to map*/
         ++siArg;
         ftbSetSTPtr->spolPercScoreF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->spolPercScoreF < 0
            || ftbSetSTPtr->spolPercScoreF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
            stderr,
            "-spoligo-min-score %s not between 0 and 1\n",
            argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: min % score for spacer to map*/

      else if(
         ! eql_charCp(
            (signed char *) "-dr-start",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: Start of direct repeat region*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->drStartSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-dr-start %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->drStartSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-dr-start %s must be at least 0\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: Start of direct repeat region*/

      else if(
         ! eql_charCp(
            (signed char *) "-dr-end",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: end of direct repeat region*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &ftbSetSTPtr->drEndSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-dr-end %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->drEndSI <= 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-dr-end %s must be greater than 0\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: end of direct repeat region*/

      /**************************************************\
      * Fun07 Sec03 Sub08:
      *   - clustering settings
      *   o fun07 sec03 sub08 cat01:
      *     - user requested clustering?
      *   o fun07 sec03 sub08 cat02:
      *     - minimum length weight for read scoring
      *   o fun07 sec03 sub08 cat03:
      *     - depth profiling
      *   o fun07 sec03 sub08 cat04:
      *     - minium cluster depth
      *   o fun07 sec03 sub08 cat05:
      *     - minium cluster percent depth
      *   o fun07 sec03 sub08 cat06:
      *     - minium read:read percent distance
      *   o fun07 sec03 sub08 cat07:
      *     - minium consensus:read percent distance
      *   o fun07 sec03 sub08 cat08:
      *     - maximum consensus similarity (before merge)
      *   o fun07 sec03 sub08 cat09:
      *     - maximum error to variant percent (edDist)
      *   o fun07 sec03 sub08 cat10:
      *     - minimum overlap between consensuses
      *   o fun07 sec03 sub08 cat11:
      *     - window size for window scan
      *   o fun07 sec03 sub08 cat12:
      *     - window variant:error ratio
      *   o fun07 sec03 sub08 cat13:
      *     - mimimum indel length to count as difference
      *   o fun07 sec03 sub08 cat14:
      *     - mimimum q-score to keep snp
      *   o fun07 sec03 sub08 cat15:
      *     - maximum percent of consensus maksed
      *   o fun07 sec03 sub08 cat16:
      *     - number of times to rebuild consensus
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat01:
      +   - user requested clustering
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-clust",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->clustBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-clust",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->clustBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat02:
      +   - minimum length weight for read scoring
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-len-weight",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: how much length influces score*/
         ++siArg;
         ftbSetSTPtr->clustSetST.lenWeightF =
            atof(argAryStr[siArg]);

         if(ftbSetSTPtr->clustSetST.lenWeightF < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-len-weigth %s must be at least 0\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: how much length influces score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat03:
      +   - depth profiling
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-depth-prof",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->clustSetST.depthProfBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-depth-prof",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) ftbSetSTPtr->clustSetST.depthProfBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat04:
      +   - minium cluster depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-clust-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min cluster read depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.minDepthUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-clust-depth %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/

         if(ftbSetSTPtr->clustSetST.minDepthUI <= 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-clust-depth %s must be at least 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: min cluster read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat05:
      +   - minium cluster percent depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-clust-perc-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: cluster min percent read depth*/
         ++siArg;

         ftbSetSTPtr->clustSetST.minPercDepthF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->clustSetST.minPercDepthF < 0
            || ftbSetSTPtr->clustSetST.minPercDepthF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
             stderr,
             "-clust-perc-depth %s not between 0 and 1\n",
             argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: cluster min percent read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat06:
      +   - minium read:read percent distance
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-read-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: error rate for read to read map*/
         ++siArg;

         ftbSetSTPtr->clustSetST.readErrRateF =
            atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->clustSetST.readErrRateF < 0
            || ftbSetSTPtr->clustSetST.readErrRateF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-read-err %s not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: error rate for read to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat07:
      +   - minium consensus:read percent distance
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-con-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
        )
     ){ /*Else If: error rate for con to read map*/
        ++siArg;
        ftbSetSTPtr->clustSetST.conErrRateF =
           atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->clustSetST.conErrRateF < 0
            || ftbSetSTPtr->clustSetST.conErrRateF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-con-err %s not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
     } /*Else If: error rate for con to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat08:
      +   - maximum consensus similarity (before merge)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-con-sim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: maximum similarity between cons*/
         ++siArg;

         ftbSetSTPtr->clustSetST.maxConSimF =
           atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->clustSetST.maxConSimF < 0
            || ftbSetSTPtr->clustSetST.maxConSimF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-con-sim %s not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: maximum similarity between cons*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat09:
      +   - maximum error to variant percent (edDist)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-err-to-var",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: maximum error to variant ratio*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.varToErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-err-to-var %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum error to variant ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat10:
      +   - minimum overlap between consensuses
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-overlap",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum overlap*/
         ++siArg;

         ftbSetSTPtr->clustSetST.percOverlapF =
            atof(argAryStr[siArg]);

         if(ftbSetSTPtr->clustSetST.percOverlapF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-overlap %s goes to 0; invalid input\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/

         if(
               ftbSetSTPtr->clustSetST.percOverlapF < 0
            || ftbSetSTPtr->clustSetST.percOverlapF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-overlap %s not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum overlap*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat11:
      +   - window size for window scan
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-win-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: window length for window scanning*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.winSizeUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/
      } /*Else If: window length for window scanning*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat12:
      +   - window variant:error ratio
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-win-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: window error to varaint ratio*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.winErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-err %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/
      } /*Else If: window error to varaint ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat13:
      +   - mimimum indel length to count as difference
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-indel-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: invalid input*/
      } /*Else If: min indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat14:
      +   - mimimum q-score to keep snp
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-clust-q-snp",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &ftbSetSTPtr->clustSetST.minSnpQUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
             stderr,
             "-clust-q-snp %s; non-numeric or to large\n",
             argAryStr[siArg]
            );

           goto err_fun07_sec04;
         } /*If: invalid input*/

         if(ftbSetSTPtr->clustSetST.minSnpQUC > 93)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-clust-q-snp %s not between 0 to 93\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: min snp q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat15:
      +   - maximum percent of consensus maksed
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-perc-n",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: maximum % masking*/
        ++siArg;
        ftbSetSTPtr->clustSetST.maxNPercF =
           atof(argAryStr[siArg]);

         if(
               ftbSetSTPtr->clustSetST.maxNPercF < 0
            || ftbSetSTPtr->clustSetST.maxNPercF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-perc-n %s not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: value out of range*/
      } /*Else If: maximum % masking*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec03 Sub08 Cat16:
      +   - number times to rebuild consensus
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-con-iter",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: number of cosensus rebuilds*/
        ++siArg;
        tmpStr = (signed char *) argAryStr[siArg];

        tmpStr +=
           strToUC_base10str(
              tmpStr,
              &ftbSetSTPtr->clustSetST.conRebuildUC
           );

         if(*tmpStr != '\0')
         { /*If: had error*/
            if(helpBl)
               goto phelp_fun07_sec02;

            fprintf(
               stderr,
               "-con-iter %s; not numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun07_sec04;
         } /*If: had error*/
      } /*Else If: number of cosensus rebuilds*/


      /**************************************************\
      * Fun07 Sec03 Sub09:
      *   - check for help message requests (normal)
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) helpBl = 1;

      /**************************************************\
      * Fun07 Sec03 Sub10:
      *   - check for extra entry help messages
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h-filt",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add filtering settings to help*/
         helpBl = 1;
         pFiltHelpBl = 1;
      } /*Else If: add filtering settings to help*/

      else if(
         ! eql_charCp(
            (signed char *) "-h-con",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add consensus settings to help*/
         helpBl = 1;
         pConHelpBl = 1;
      } /*Else If: add consensus settings to help*/

      else if(
         ! eql_charCp(
            (signed char *) "-h-lin",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add lineage settings to help*/
         helpBl = 1;
         pLinHelpBl = 1;
      } /*Else If: add lineage settings to help*/

      else if(
         ! eql_charCp(
            (signed char *) "-h-print",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add print settings to help*/
         helpBl = 1;
         pPrintHelpBl = 1;
      } /*Else If: add print settings to help*/

      else if(
         ! eql_charCp(
            (signed char *) "-h-clust",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add cluster settings to help*/
         helpBl = 1;
         pClustHelpBl = 1;
      } /*Else If: add cluster settings to help*/

      else if(
         ! eql_charCp(
            (signed char *) "-h-other",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: add other settings to help*/
         helpBl = 1;
         pOtherHelpBl = 1;
      } /*Else If: add other settings to help*/

      /**************************************************\
      * Fun07 Sec03 Sub11:
      *   - check for version number requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun07_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun07_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun07_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun07_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun07_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun07 Sec03 Sub12:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         if(helpBl)
            goto phelp_fun07_sec02;

         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun07_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun07 Sec03 Sub13:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: read in user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - return errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(helpBl)
      goto phelp_fun07_sec02;
      /*user wanted help message*/
      /*this trick allows for full help message printing*/
   
   errSC = 0;
   goto ret_fun08_sec04;

   phelp_fun07_sec04:;
   pversion_fun07_sec04:;
      errSC = 1;
      goto ret_fun08_sec04;

   err_fun07_sec04:;
      errSC = 2;
      goto ret_fun08_sec04;

   ret_fun08_sec04:;
      return errSC;
} /*input_freezeTB*/

/*-------------------------------------------------------\
| Fun08: run_freezeTB
|    - Analyze ONT sequenced TB reads
| Input:
|    - numArgsSI:
|      o Integer with number of argument the user input
|    - argAryStr:
|      o C-string array with the input arguments
| Output:
|    - Prints:
|      o all files for freezeTB command line program
|      o errors to stderr
|      o clustering progress to stderr
|    - Returns:
|      o 0 for no errors
|      o c-string with error message
\-------------------------------------------------------*/
signed char *
run_freezeTB(
   int numArgsSI,
   const char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - Run freezeTB on user input
   '   o fun08 sec01:
   '     - Variable declerations
   '   o fun08 sec02:
   '     - initialize, get input, and set up memory
   '   o fun08 sec03:
   '     - check user input database (if can open)
   '   o fun08 sec04:
   '     - check output files (can I open?)
   '   o fun08 sec05:
   '     - read in databases
   '   o fun08 sec06:
   '     - get reference stats and print consensus header
   '   o fun08 sec07:
   '     - Do read analysis
   '   o fun08 sec08:
   '     - print read data
   '   o fun08 sec09:
   '     - collapse consensus and consensus analysis
   '   o fun08 sec10:
   '     - run mixed infection detection (if requested)
   '   o fun08 sec11:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - Variable declerations
   ^   o fun08 sec01 sub01:
   ^     - general IO variables (applies to multple subs)
   ^   o fun08 sec01 sub02:
   ^     - temporay and error reporting variables
   ^   o fun08 sec01 sub03:
   ^     - filtering and sam file variables (adjust coord)
   ^   o fun08 sec01 sub04:
   ^     - read depth and coverage stats variables
   ^   o fun08 sec01 sub05:
   ^     - AMR detection variables
   ^   o fun08 sec01 sub06:
   ^     - miru lineage unique variables
   ^   o fun08 sec01 sub07:
   ^     - spoligotyping unique variables
   ^   o fun08 sec01 sub08:
   ^     - consensus building/mixed infection variables
   ^   o fun08 sec01 sub09:
   ^     - masking unique variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec01 Sub01:
   *   - general IO variables (applies to multple subs)
   \*****************************************************/

   FILE *outFILE = 0;
   struct set_freezeTB ftbSetStackST; /*has settings*/

   /*****************************************************\
   * Fun08 Sec01 Sub02:
   *   - Temporay and error reporting variables
   \*****************************************************/

   signed char spareStr[1024]; /*extra buffer*/
   signed char *tmpStr = 0;

   signed char errSC = 0;
   signed long errSL = 0;

   signed char *errHeapStr = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub03:
   *   - filtering and sam file variables (adjust coord)
   \*****************************************************/

   signed char *samFileStr = 0;

   /*for sam file processing*/
   struct samEntry samStackST;
   signed char *buffHeapStr = 0;
   unsigned long lenBuffUL = 0;
   FILE *samFILE = 0;

   signed char multiRefBl = 0;
      /*set to 1; means have multiple error refrences;*/
   signed int lenRefSI = 0;
   signed char refIdStr[def_lenFileName_freezeTB];

   /*****************************************************\
   * Fun08 Sec01 Sub04:
   *   - read depth and coverage stats variables
   \*****************************************************/

   signed char readStatsStr[def_lenFileName_freezeTB];
      /*output file name*/

   struct geneCoord *coordsHeapST = 0;
   signed int numCoordsSI = 0;
   unsigned int lastBaseUI = 0;
   unsigned int firstBaseUI = 0;

   signed int *readMapArySI = 0;
   signed int *filt_readMapArySI = 0;
   signed int offTargReadsSI = 0;
   signed int oldOffTargSI = 0;
   signed int noMapReadSI = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub05:
   *   - AMR detection variables
   \*****************************************************/

   signed char idFileStr[def_lenFileName_freezeTB];
      /*read id printing*/

   signed char conAmrStr[def_lenFileName_freezeTB];
   signed char readAmrStr[def_lenFileName_freezeTB];
   signed char logFileStr[def_lenFileName_freezeTB];

   FILE *idFILE = 0; /*For read id printing*/
   FILE *logFILE = 0; /*for mixed infection currently*/

   struct amrST *amrHeapAryST = 0;
   struct amrHit_checkAmr *amrHitHeapSTList = 0;
   signed int numAmrSI = 0;
   signed int numHitsSI = 0;

   signed char *drugHeapAryStr = 0;
   signed int numDrugsSI = 0;
   signed int maxDrugsSI = 0;
   unsigned int totalReadsUI = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub06:
   *   - miru lineage unique variables
   \*****************************************************/

   struct miruTbl *miruHeapST = 0;

   signed char readMiruStr[def_lenFileName_freezeTB];
       /*read results output*/
   signed char conMiruStr[def_lenFileName_freezeTB];
      /*consensus results output*/

   /*****************************************************\
   * Fun08 Sec01 Sub07:
   *   - Spoligotyping unique variables
   \*****************************************************/

   signed char checkSpoligoLinBl = 1;
      /*set to 0 if failed to load linage database*/

   #define def_lenSpolAry_fun08 128
   unsigned int spoligoAryUI[def_lenSpolAry_fun08 + 1];
   signed char
      outSpoligoFileStr[def_lenFileName_freezeTB];
   signed char
      outReadSpoligoFileStr[def_lenFileName_freezeTB];

   signed char spolErrSC = 0;

   FILE *spoligoOutFILE = 0;

   struct spolST *lineageHeapAryST = 0;
   signed int numLineagesSI = 0;
   unsigned int spoligoNumReadsUI = 0;

   /*kmer finding of spoligotype variables*/
   struct tblST_kmerFind kmerTblStackST;
   struct refST_kmerFind *kmerRefAryST = 0;
   signed int numSpoligosSI = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub08:
   *   - consensus building/mixed infection variables
   \*****************************************************/

   signed char samConStr[def_lenFileName_freezeTB];
   signed char conTsvStr[def_lenFileName_freezeTB];

   struct conNt_tbCon *conNtHeapAryST = 0;
   FILE *samConFILE = 0;

   struct samEntry *samConSTAry = 0;
   signed int numFragSI = 0;
   signed int siCon = 0; /*iterator for consensus step*/

   struct index_clustST *indexHeapST = 0;
   struct con_clustST *conListHeapST = 0;
   struct con_clustST *conNodeST = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub09:
   *   - masking unique variables
   \*****************************************************/

   unsigned int *maskStartHeapAryUI = 0;
   unsigned int *maskEndHeapAryUI = 0;
   unsigned int *maskFlagHeapAryUI = 0;
   unsigned int maskNumPrimUI = 0;

   /*****************************************************\
   * Fun08 Sec01 Sub10:
   *   - indel clean up variables
   \*****************************************************/

   /*for indel clean up step*/
   unsigned int tmpSeqUI = 0;
   unsigned int tmpQUI = 0;
   struct seqST refStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - initialize, get input, and set up memory
   ^   o fun08 sec02 sub01:
   ^     - initialize variables
   ^   o fun08 sec02 sub02:
   ^     - get input
   ^   o fun08 sec02 sub03:
   ^     - set up memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_set_freezeTB(&ftbSetStackST);
   ftbSetStackST.maskPrimFileStr[0] = '\0';
      /*disable masking; force user to provide a file*/

   refIdStr[0] = '\0';

   init_seqST(&refStackST);
   init_samEntry(&samStackST);
   init_alnSet(&ftbSetStackST.alnSetST);
   init_tblST_kmerFind(&kmerTblStackST);

   /*initialize spoligotyping array*/
   for(
      numLineagesSI = 0;
      numLineagesSI < def_lenSpolAry_fun08;
      ++numLineagesSI
   ) spoligoAryUI[numLineagesSI] = 0;

   spoligoAryUI[numLineagesSI] = -1;

   numLineagesSI = 0;

   /*will always assume works*/
   errHeapStr = malloc(128 * sizeof(signed char *));
      /*I have to assume this will always work, otherwise
      `  I have nothing to return
      */

   /*****************************************************\
   * Fun08 Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_freezeTB(
         numArgsSI,
         argAryStr,
         &samFileStr,      /*reads*/
         &ftbSetStackST    /*has settings*/
   );

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version print to no error*/

      if(errSC)
      { /*If: had error with input*/
         tmpStr = errHeapStr;

         *tmpStr++ = 'i';
         *tmpStr++ = 'n';
         *tmpStr++ = 'p';
         *tmpStr++ = 'u';
         *tmpStr++ = 't';
         *tmpStr++ = ' ';
         *tmpStr++ = 'e';
         *tmpStr++ = 'r';
         *tmpStr++ = 'r';
         *tmpStr++ = 'o';
         *tmpStr++ = 'r';
         *tmpStr++ = '\0';

         goto err_fun08_sec11_sub02;
      } /*If: had input error*/

      else
         goto ret_fun08_sec11;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec02 Sub03:
   *   - set up memory
   \*****************************************************/

   if(! errHeapStr)
   { /*If: hadd memory error*/
      fprintf(
         stderr,
         "memory error setting error report string\n"
      );

      goto err_fun08_sec11_sub02;
   } /*If: hadd memory error*/


   if( setup_samEntry(&samStackST) )
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
             "memory error samEntry struct setup"
      );

      goto err_fun08_sec11_sub02;
   } /*If: memory error*/


   errSC =
      setup_tblST_kmerFind(
         &kmerTblStackST,
         ftbSetStackST.lenKmerUC
      );

   if(
      setup_tblST_kmerFind(
         &kmerTblStackST,
         ftbSetStackST.lenKmerUC
      )
   ){ /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "memory error tblST_kmerFind struct setup"
      );

      goto err_fun08_sec11_sub02;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - check user input database (if can open)
   ^   o fun08 sec03 sub01:
   ^     - check if MIRU table exists
   ^   o fun08 sec03 sub02:
   ^     - check if spoligotyping spacer sequences exists
   ^   o fun08 sec03 sub03:
   ^     - check if spoligotyping lineage database
   ^   o fun08 sec03 sub04:
   ^     - check if amr table exists
   ^   o fun08 sec03 sub05:
   ^     - open the sam file
   ^   o fun08 sec03 sub06:
   ^     - check if gene coordinates file exits
   ^   o fun08 sec03 sub07:
   ^     - read in reference sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec03 Sub01:
   *   - check if MIRU table exists
   \*****************************************************/

   outFILE =
      fopen(
         (char *) ftbSetStackST.miruDbFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: I could not open the MIRU table*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (signed char *) "unable to open -miru-tbl "
         );

      cpStr_ulCp(
         tmpStr,
         ftbSetStackST.miruDbFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: I could not open the MIRU table*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec03 Sub02:
   *   - check if spoligotyping spacer sequences exists
   \*****************************************************/

   outFILE =
      fopen(
         (char *) ftbSetStackST.spolRefFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open spoligo spacer sequences*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (signed char *) "unable to open -spoligo "
         );

      cpStr_ulCp(
         tmpStr,
         ftbSetStackST.spolRefFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open spoligo spacer sequences*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec03 Sub03:
   *   - check if spoligotyping lineage database
   \*****************************************************/

   outFILE =
      fopen(
         (char *) ftbSetStackST.spolDBFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open the spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -db-spoligo %s\n",
         ftbSetStackST.spolDBFileStr
      );

      fprintf(
         stderr,
         "spoligotype lineages will be ignored\n"
      );

      checkSpoligoLinBl = 0;
   } /*If: could not open the spoligo spacer sequences*/

   else
      fclose(outFILE); /*valid lineage database*/

   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec03 Sub04:
   *   - check if amr table exists
   \*****************************************************/

   
   outFILE =
      fopen(
         (char *) ftbSetStackST.amrDbFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open the filtered read stats file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *) "unable to open -amr-tbl "
         );

      cpStr_ulCp(
         tmpStr,
         ftbSetStackST.amrDbFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec03 Sub05:
   *   - open sam file
   \*****************************************************/
   
   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: need to open sam file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open the sam file*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "could not open -sam "
            );

         cpStr_ulCp(
            tmpStr,
            samFileStr
         );

         goto err_fun08_sec11_sub02;
      } /*If: could not open the sam file*/
   } /*Else: need to open sam file*/

   /*****************************************************\
   * Fun08 Sec03 Sub06:
   *   - open gene coordinates file
   \*****************************************************/
   
   outFILE = 
      fopen(
         (char *) ftbSetStackST.coordFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open gene coordinates file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *) "could not open -gene-coords "
         );

      cpStr_ulCp(
         tmpStr,
         ftbSetStackST.coordFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open gene coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec03 Sub07
   *   - read in reference sequence
   *   o fun08 sec03 sub07 cat01:
   *     - open reference file
   *   o fun08 sec03 sub07 cat02:
   *     - get reference sequence from file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec03 Sub07 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(ftbSetStackST.indelCleanBl)
   { /*If: doing indel clean up*/
      outFILE =
         fopen(
            (char *) ftbSetStackST.refFileStr,
            "r"
         );

      if(! outFILE)
      { /*If: unable to open reference file*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "could not open -ref "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.refFileStr
         );

         goto err_fun08_sec11_sub02;
      } /*If: unable to open reference file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub07 Cat02:
      +   - get reference sequence from file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      errSC =
         getFaSeq_seqST(
            outFILE,
            &refStackST
         );

      fclose(outFILE);
      outFILE = 0;

      if(! errSC)
         ; /*no error*/
      else if(
            errSC == def_EOF_seqST
         && refStackST.seqStr[0] != '\0'
      ) ; /*only one sequence in file (no error)*/

      else
      { /*Else: error reading reference*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
               "could not get reference from -ref "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.refFileStr
         );

         goto err_fun08_sec11_sub02;
      } /*Else: error reading reference*/
   } /*If: doing indel clean up*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - check output files (can I open?)
   ^   o fun08 sec04 sub01:
   ^     - output file for read stats
   ^   o fun08 sec04 sub02:
   ^     - set up cosensus fragments output file
   ^   o fun08 sec04 sub03:
   ^     - set up read AMRs table outp file name
   ^   o fun08 sec04 sub04:
   ^     - set up read id AMR hit table
   ^   o fun08 sec04 sub05:
   ^     - output file for the AMRs found in consensus
   ^   o fun08 sec04 sub06:
   ^     - set up MIRU reads table output name
   ^   o fun08 sec04 sub07:
   ^     - set up MIRU consensus table output name
   ^   o fun08 sec04 sub08:
   ^     - set up consensus spoligotyping output file
   ^   o fun08 sec04 sub09:
   ^     - set up read spoligotyping output file name
   ^   o fun08 sec04 sub10:
   ^     - set up open consensus output file name
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec04 Sub01:
   *   - set up read stats file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-depths.tsv",
        readStatsStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "unable to open depths ouput file: "
         );

      cpStr_ulCp(
         tmpStr,
         readStatsStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/
 
   /*****************************************************\
   * Fun08 Sec04 Sub02:
   *   - set up cosensus fragments output file
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-con-variants.tsv",
        conTsvStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
          errHeapStr,
          (signed char *)
             "unable to open con tsv output file: "
        );

      cpStr_ulCp(
         tmpStr,
         conTsvStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub03:
   *   - set up read AMRs table outp file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-read-amrs.tsv",
        readAmrStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "unable to open read AMR output file: "
        );

      cpStr_ulCp(
         tmpStr,
         readAmrStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub04:
   *   - set up read id AMR hit table
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-id-amrs.tsv",
        idFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
           errHeapStr,
           (signed char *)
              "unable to open AMR id output file: "
        );

      cpStr_ulCp(
         tmpStr,
         idFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub05:
   *   - Set up the name for the consensus AMRs table
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-con-amrs.tsv",
        conAmrStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
          errHeapStr,
          (signed char *)
             "unable to open con AMR output file: "
        );

      cpStr_ulCp(
         tmpStr,
         conAmrStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub06:
   *   - set up MIRU reads table output name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        ftbSetStackST.prefixStr,
        (signed char *) "-read-miru.tsv",
        readMiruStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "unable to open read miru output: "
         );

      cpStr_ulCp(
         tmpStr,
         readMiruStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub07:
   *   - set up MIRU consensus table output name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-read-miru.tsv",
         readMiruStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "unable to open read miru output: "
         );

      cpStr_ulCp(
         tmpStr,
         conMiruStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-con-miru.tsv",
         conMiruStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "unable to open con miru output: "
         );

      cpStr_ulCp(
         tmpStr,
         conMiruStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub08:
   *   - set up consensus spoligotyping output file
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-con-spoligo.tsv",
         outSpoligoFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
           errHeapStr,
           (signed char *)
              "unable to open con spoligo output: "
        );

      cpStr_ulCp(
         tmpStr,
         outSpoligoFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub09:
   *   - set up read spoligotyping output file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-read-spoligo.tsv",
         outReadSpoligoFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
        cpStr_ulCp(
          errHeapStr,
          (signed char *)
             "unable to open read spoligo output: "
        );

      cpStr_ulCp(
         tmpStr,
         outReadSpoligoFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun08 Sec04 Sub10:
   *   - set up open consensus output file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-cons.sam",
         samConStr
      );

   if(errSC)
   { /*If: could not open file*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "unable to open consensus output: "
         );

      cpStr_ulCp(
         tmpStr,
         samConStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open file*/

   samConFILE =
      fopen(
         (char *) samConStr,
          "w"
      ); /*already checked if could open*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - read in databases
   ^   o fun08 sec05 sub01:
   ^     - get gene mapping coodiantes
   ^   o fun08 sec05 sub02:
   ^     - get amr table
   ^   o fun08 sec05 sub03:
   ^     - get MIRU lineage table
   ^   o fun08 sec05 sub04:
   ^     - get spoligotyping spacer sequences
   ^   o fun08 sec05 sub05:
   ^     - get spoligotyping lineages
   ^   o fun08 sec05 sub06:
   ^     - get masking primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec05 Sub01:
   *   - get gene mapping coodiantes
   \*****************************************************/

   coordsHeapST =
      getCoords_geneCoord(
         ftbSetStackST.coordFileStr,
         &numCoordsSI,
         &lenBuffUL    /*reusing to hold error output*/
      );

   if(! coordsHeapST)
   { /*If: error*/
      if(lenBuffUL & def_memErr_geneCoord)
      { /*If: had memory error*/
         tmpStr = errHeapStr;

         tmpStr +=
          cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memory error reading -gene-coords "
          );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.coordFileStr
         );
      } /*If: had memory error*/

      else
      { /*Else: wrong coordinates*/
         tmpStr = errHeapStr;

         *tmpStr++ = 'l';
         *tmpStr++ = 'i';
         *tmpStr++ = 'n';
         *tmpStr++ = 'e';
         *tmpStr++ = ' ';

         tmpStr +=
            numToStr(
               tmpStr,
               lenBuffUL >> 8
            );
 
         tmpStr +=
            cpStr_ulCp(
               tmpStr,
               (signed char *)
                  " is wrong in -gene-coords "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.coordFileStr
         );
      } /*Else: wrong coordinates*/

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   lenBuffUL = 0;

   firstBaseUI = coordsHeapST->startAryUI[0];
   lastBaseUI = coordsHeapST->endAryUI[numCoordsSI];

   sortName_geneCoord(
      coordsHeapST,
      0,
      numCoordsSI
   );

   /*****************************************************\
   * Fun08 Sec05 Sub02:
   *   - get amr table
   \*****************************************************/

   amrHeapAryST =
      readTbl_amrST(
         ftbSetStackST.amrDbFileStr,
         (unsigned int *) &numAmrSI,
         &drugHeapAryStr,
         &numDrugsSI,
         &maxDrugsSI,
         &errSC
      ); /*Read in the amr database*/

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_amrST)
      { /*If: file error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               ftbSetStackST.amrDbFileStr
            );

         cpStr_ulCp(
            tmpStr,
            (signed char *)
               " not in tbAmr format or empty"
         );
      } /*If: file error*/

      if(errSC == def_memErr_amrST)
      { /*If: memory error*/
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memory error processing variant id"
         );
      } /*If: memory error*/

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec05 Sub03:
   *   - get MIRU lineage table
   \*****************************************************/
 
   miruHeapST =
      get_miruTbl(
         ftbSetStackST.miruDbFileStr,
         &errSC
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_tbMiruDefs)
      { /*If: had file error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "Could not open -miru-tbl "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.miruDbFileStr
         );
      } /*If: had file error*/

      else
      { /*Else: memory error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
                  "memory error reading -miru-tbl "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.miruDbFileStr
         );
      } /*Else: memory error*/

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec05 Sub04:
   *   - get spoligotyping spacer sequences
   \*****************************************************/

   kmerRefAryST =
      faToAry_refST_kmerFind(
        ftbSetStackST.spolRefFileStr,
        ftbSetStackST.lenKmerUC,
        &numSpoligosSI,
        ftbSetStackST.minKmerPercF,
        &kmerTblStackST,
        ftbSetStackST.percExtraNtInWinF,
        ftbSetStackST.percShiftF,
        &ftbSetStackST.alnSetST,
        &errSC
   );

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_kmerFind)
      { /*If: file error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "invalid file: -spoligo "
            );

         cpStr_ulCp(
            tmpStr,
            ftbSetStackST.spolRefFileStr
         );
 
         goto err_fun08_sec11_sub02;
      } /*If: file error*/
 
      else
      { /*Else: memory error*/
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memory error getting spoligo seqs"
         );
 
          goto err_fun08_sec11_sub02;
      } /*Else: memory error*/
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec05 Sub05:
   *   - get spoligotyping lineage database
   \*****************************************************/

   if(checkSpoligoLinBl)
   { /*If: have lineage database*/
      lineageHeapAryST =
         readDb_spolST(
            ftbSetStackST.spolDBFileStr,
            &numLineagesSI,
            &errSC
      );

      if(errSC)
      { /*If: error*/
         if(errSC == def_fileErr_tbSpolDefs)
         { /*If: file error*/
            fprintf(
               stderr,
               "could not open -db-spoligo %s\n",
               ftbSetStackST.spolDBFileStr
            );

            fprintf(
               stderr,
               "skipping spoligotype checks\n"
            );
         } /*If: file error*/

         else
         { /*Else: memory error*/
            tmpStr = errHeapStr;

            tmpStr +=
               cpStr_ulCp(
                  errHeapStr,
                  (signed char *)
                     "memory error -db-spoligo "
               );

            cpStr_ulCp(
               tmpStr,
               ftbSetStackST.spolDBFileStr
            );

            goto err_fun08_sec11_sub02;
         } /*Else: memory error*/
      } /*If: error*/
   } /*If: have lineage database*/

   /*****************************************************\
   * Fun08 Sec05 Sub06:
   *   - get masking primer coordinates
   \*****************************************************/

   maskNumPrimUI = 0;

   if(
         ftbSetStackST.maskPrimFileStr[0] != '-' 
      && ftbSetStackST.maskPrimFileStr[0] != '\0'
   ){ /*If: primer masking file input*/
      maskNumPrimUI =
         (unsigned int)
         getCoords_maskPrim(
            ftbSetStackST.maskPrimFileStr,
            &maskStartHeapAryUI,
            &maskEndHeapAryUI,
            &maskFlagHeapAryUI,
            &errSL
         );

      if(! maskNumPrimUI)
      { /*If: error*/
         if(errSL == def_emptyFileErr_maskPrim)
         { /*If: empty file*/
            tmpStr = errHeapStr;

            tmpStr +=
               cpStr_ulCp(
                  errHeapStr,
                  (signed char *) "empty file -mask-prim "
               );

            cpStr_ulCp(
               tmpStr,
               ftbSetStackST.maskPrimFileStr
            );
         } /*If: empty file*/

         else if(errSL & def_fileErr_maskPrim)
         { /*Else If: invalid line*/
            tmpStr = errHeapStr;

            *tmpStr++ = 'l';
            *tmpStr++ = 'i';
            *tmpStr++ = 'n';
            *tmpStr++ = 'e';
            *tmpStr++ = ' ';

            tmpStr +=
               numToStr(
                  tmpStr,
                  errSL >> 8
               );
 
            tmpStr +=
               cpStr_ulCp(
                  tmpStr,
                  (signed char *)
                     " is wrong in -mask-prim "
               );

            tmpStr +=
               cpStr_ulCp(
                  tmpStr,
                  ftbSetStackST.maskPrimFileStr

               );

            cpStr_ulCp(
               tmpStr,
               (signed char *) " (or invalid file)"
            );
         } /*Else If: invalid line*/
         
         else
         { /*Else: memory error*/
            tmpStr = errHeapStr;

            tmpStr +=
              cpStr_ulCp(
                 errHeapStr,
                 (signed char *)
                    "mem error reading -mask-prim "
              );

            cpStr_ulCp(
               tmpStr,
               ftbSetStackST.maskPrimFileStr
            );
         } /*Else: memory error*/

         goto err_fun08_sec11_sub02;
      } /*If: error*/
   } /*If: primer masking file was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec06:
   ^   - get reference stats and print consensus header
   ^   o fun08 sec06 sub01:
   ^     - get reference name/length from header
   ^   o fun08 sec06 sub02:
   ^     - print tbCon header for sam file
   ^   o fun08 sec06 sub03:
   ^     - check if have reference name/length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec06 Sub01:
   *   - get reference length from header
   *   o fun08 sec06 sub01 cat01:
   *     - get first sam file entry + start loop
   *   o fun08 sec06 sub01 cat02:
   *     - print comment entry (end loop if not comment)
   *   o fun08 sec06 sub01 cat03:
   *     - if sequence entry; get id and length
   *   o fun08 sec06 sub01 cat04:
   *     - move to next entry
   *   o fun08 sec06 sub01 cat05:
   *     - check for errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec06 Sub01 Cat01:
   +   - get first sam file entry + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(ftbSetStackST.adjCoordBl)
   { /*If: adjusting coordinates*/
      lenRefSI = (signed int) lastBaseUI;
      ++lenRefSI; /*convert to index 1*/
   } /*If: adjusting coordinates*/

   errSC =
      get_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errSC)
   { /*Loop: read in header*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec06 Sub01 Cat02:
      +   - print comment entry (end loop if not comment)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(samStackST.extraStr[0] != '@')
         break; /*off header*/

      if(
            ftbSetStackST.adjCoordBl
         &&
            ! eql_charCp(
               (signed char *) "@SQ\t",
               samStackST.extraStr,
               '\t'
            )
      ) goto nextHeader_fun08_sec06_sub01_cat04;
        /*do not print sequence entries for header*/

      fprintf(
         samConFILE,
         "%s\n",
         samStackST.extraStr
      );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec06 Sub01 Cat03:
      +   - if sequence entry; get id and length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
         ! eql_charCp(
            (signed char *) "@SQ\t",
            samStackST.extraStr,
            '\t'
         )
      ){ /*If: sequence entry*/
         if(multiRefBl)
         { /*If: sam file has multiple references*/
            tmpStr = errHeapStr;

            tmpStr +=
               cpStr_ulCp(
                  errHeapStr,
                  (signed char *)
                     "multiple references in -sam "
               );

            cpStr_ulCp(
               tmpStr,
               samFileStr
            );

            goto err_fun08_sec11_sub02;
         } /*If: sam file has multiple references*/

         multiRefBl = 1;

         /*get past "@SQ\t"*/
         tmpStr = samStackST.extraStr + 4; 

         while(*tmpStr++ != ':')
            if(*tmpStr < 31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_fun08_sec06_sub01_cat04;

         tmpStr +=
            cpDelim_ulCp(
               refIdStr,
               tmpStr,
               def_tab_ulCp,
               '\t'
            ); /*copy reference id*/

         ++tmpStr;
        
         if(*tmpStr < 31 )
            goto nextHeader_fun08_sec06_sub01_cat04;

         /*move past LN: flag*/
         while(*tmpStr++ != ':')
            if(*tmpStr <31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_fun08_sec06_sub01_cat04;

         /*get reference length*/
         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &lenRefSI
            );

         if(*tmpStr > 31)
            lenRefSI = def_refLen_tbConDefs;
            /*could not get reference length*/
      } /*If: sequence entry*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec06 Sub01 Cat04:
      +   - move to next entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      nextHeader_fun08_sec06_sub01_cat04:;

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: read in header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec06 Sub01 Cat05:
   +   - check for errors
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(errSC)
   { /*If: error reading sam file header*/
      if(errSC == def_EOF_samEntry)
      { /*If: empty sam file*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "no sequences in -sam "
            );

         cpStr_ulCp(
            tmpStr,
            samFileStr
         );
      } /*If: empty sam file*/

      else
      { /*Else: memory error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
                  "memory error reading -sam "
            );

         cpStr_ulCp(
            tmpStr,
            samFileStr
         );
      } /*Else: memory error*/

      goto err_fun08_sec11_sub02;
   } /*If: error reading sam file header*/

   /*****************************************************\
   * Fun08 Sec06 Sub02:
   *   - print tbCon header for sam file
   *   o fun08 sec06 sub02 cat01:
   *     - tbCon cosensus settings
   *   o fun08 sec06 sub02 cat02:
   *     - tbCon variant print (tsv file) settings
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec06 Sub02 Cat01:
   +   - tbCon cosensus settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! ftbSetStackST.clustBl)
   { /*If: not doing mixed infection step*/
      fprintf(
         samConFILE,
         "@PG\tID:tbCon\tPN:tbCon"
      );

      fprintf(
         samConFILE,
         "\tVN:%i-%02i-%02i\tCL:tbCon",
         def_year_tbConDefs,
         def_month_tbConDefs,
         def_day_tbConDefs
      );

      /*Check if getting sam file from stdin*/
      if(! samFileStr || *samFileStr == '-')
        fprintf(
           samConFILE,
           " -sam -"
        );

      else
        fprintf(
           samConFILE,
              " -sam %s",
              samFileStr
        );

      fprintf(
         samConFILE,
         " -min-mapq %i -min-q %i -min-q-ins %i",
         ftbSetStackST.tbConSet.minMapqUC,
         ftbSetStackST.tbConSet.minQSI,
         ftbSetStackST.tbConSet.minInsQSI
      );

      fprintf(
         samConFILE,
         " -min-len %i -min-depth %i -perc-snp-sup %.2f",
         ftbSetStackST.tbConSet.minLenSI,
         ftbSetStackST.tbConSet.minDepthSI,
         ftbSetStackST.tbConSet.minPercSnpF
      );

      fprintf(
         samConFILE,
         " -perc-ins-sup %.2f -min-del-sup %.2f",
         ftbSetStackST.tbConSet.minPercInsF,
         ftbSetStackST.tbConSet.minPercDelF
      );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec06 Sub02 Cat02:
      +   - tbCon variant print (tsv file) settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      fprintf(
         samConFILE,
         " -min-mapq %i -min-q %i -min-q-ins %i",
         ftbSetStackST.tbConSet.minMapqUC,
         ftbSetStackST.tbConSet.minQSI,
         ftbSetStackST.tbConSet.minInsQSI
      );

      fprintf(
         samConFILE,
         " -p-min-depth %i -p-perc-snp-sup %.2f",
         ftbSetStackST.tbConSet.minPrintDepthSI,
         ftbSetStackST.tbConSet.printMinSupSnpF
      );

      fprintf(
         samConFILE,
         " -p-perc-ins-sup %.2f -p-min-del-sup %.2f",
         ftbSetStackST.tbConSet.printMinSupInsF,
         ftbSetStackST.tbConSet.printMinSupDelF
      );

      fprintf(
         samConFILE,
          " -out-tsv %s -out %s\n",
          conTsvStr,
          samConStr
      );
   } /*If: not doing mixed infection step*/

   fclose(samConFILE);
   samConFILE = 0;

   /*****************************************************\
   * Fun08 Sec06 Sub03:
   *   - check if have reference name/length
   \*****************************************************/

   if(! lenRefSI)
      lenRefSI = def_refLen_tbConDefs;

   if(refIdStr[0] == '\0')
   { /*If: reference name is missing*/
      refIdStr[0] = 'u';
      refIdStr[1] = 'n';
      refIdStr[2] = 'k';
      refIdStr[3] = 'o';
      refIdStr[4] = 'w';
      refIdStr[5] = 'n';
      refIdStr[6] = '\0';
   } /*If: reference name is missing*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec07:
   ^   - read analysis
   ^   o fun08 sec06 sub01:
   ^     - allocate memory for the read stats arrays
   ^   o fun08 sec06 sub02:
   ^     - filter reads (sam entries)
   ^   o fun08 sec06 sub03:
   ^     - mask primers in reads
   ^   o fun08 sec06 sub04:
   ^     - build filtered histogram
   ^   o fun08 sec06 sub05:
   ^     - indel clean up
   ^   o fun08 sec06 sub06:
   ^     - build consensus
   ^   o fun08 sec06 sub07:
   ^     - check for AMRs
   ^   o fun08 sec06 sub08:
   ^     - check for MIRU lineages
   ^   o fun08 sec06 sub09:
   ^     - check for spoligotypes
   ^   o fun08 sec06 sub10:
   ^     - move to next read
   ^   o fun08 sec06 sub11:
   ^     - minor clean up (variables unique to sec07)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec07 Sub01:
   *   - allocate memory for the read stats arrays
   \*****************************************************/

   readMapArySI =
      calloc(
         (lenRefSI + 8),
         sizeof(signed int)
      );

   if(! readMapArySI)
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "memory error (read histogram malloc)"
      );

      goto err_fun08_sec11_sub02;
   } /*If: memory error*/

   filt_readMapArySI =
      calloc(
         (lenRefSI + 8),
         sizeof(signed int)
      );

   if(! filt_readMapArySI)
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "memory error (read histogram malloc)"
      );

      goto err_fun08_sec11_sub02;
   } /*If: memory error*/

   /*****************************************************\
   * Fun08 Sec07 Sub02:
   *   - filter reads (sam entries)
   \*****************************************************/

   idFILE =
      fopen(
         (char *) idFileStr,
         "w"
      ); /*printing ids for reads with amrs*/

   while(! errSC)
   { /*Loop: read anaylsis*/

      if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If:umapped 4, secondary 256, or suplemtal 2048*/
         /*If this was an unmapped read*/
         if(samStackST.flagUS & 4)
            ++noMapReadSI;

         goto nextRead_fun08_sec07_sub09;
      } /*If:umapped 4, secondary 256, or suplemtal 2048*/

      if(ftbSetStackST.adjCoordBl)
      { /*If: need to adjust coordinates*/
         errSC =
            adjCoords(
               &samStackST,
               coordsHeapST,
               numCoordsSI
            );
               
         if(errSC)
         { /*If: read is not in coordinates list*/
            ++offTargReadsSI;
            errSC = 0;
            goto nextRead_fun08_sec07_sub09;
         } /*If: read is not in coordinates list*/
      } /*If: need to adjust coordinates*/

      /*remove soft masked bases*/
      trimSeq_trimSam(&samStackST);
         /*already removed reads that could cause errors*/

      oldOffTargSI = offTargReadsSI;

      addRead_ampDepth(
         &samStackST,
         firstBaseUI,
         lastBaseUI,
         readMapArySI,
         &offTargReadsSI
      ); /*build up histogram of stats*/

      /*limits full genome, really on gene coord adjust
      if(oldOffTargSI < offTargReadsSI)
         goto nextRead_fun08_sec07_sub09;
      */ /*off target*/

      if(samStackST.medianQF < ftbSetStackST.minMedianQF)
         goto nextRead_fun08_sec07_sub09;
         /*low mean q-score*/

      if(samStackST.medianQF < ftbSetStackST.minMedianQF)
         goto nextRead_fun08_sec07_sub09;
         /*low median q-score*/

      if(
           samStackST.mapqUC
         < ftbSetStackST.tbConSet.minMapqUC
      ) goto nextRead_fun08_sec07_sub09;
         /*low mapping quality*/

      if(
           samStackST.alnReadLenUI
         < (unsigned int) ftbSetStackST.tbConSet.minLenSI
      ) goto nextRead_fun08_sec07_sub09; /*short read*/

      /**************************************************\
      * Fun08 Sec07 Sub03:
      *   - mask primers in reads
      \**************************************************/

      if(maskNumPrimUI)
      { /*If: masking primers*/
         errSL =
            maskPrim(
               &samStackST,
               maskStartHeapAryUI,
               maskEndHeapAryUI,
               maskFlagHeapAryUI,
               maskNumPrimUI,
               -1, /*mask any found primer sites*/
               'N' /*Add masking*/
            );
      } /*If: masking primers*/

      /**************************************************\
      * Fun08 Sec07 Sub04:
      *   - build filtered histogram
      \**************************************************/

      ++totalReadsUI;

      addRead_ampDepth(
         &samStackST,
         firstBaseUI,
         lastBaseUI,
         filt_readMapArySI,
         &oldOffTargSI         /*never used*/
      );

      /**************************************************\
      * Fun08 Sec07 Sub05:
      *   - indel clean up
      \**************************************************/

      if(ftbSetStackST.indelCleanBl)
      { /*If: cleaning up indels in reads*/
         tmpSeqUI = samStackST.readLenUI;
         tmpSeqUI += samStackST.numDelUI;
         tmpSeqUI += 8;

         if( lenBuffUL < (tmpSeqUI << 1) )
         { /*If: need larger buffer*/
            free(buffHeapStr);
            buffHeapStr = 0;
            buffHeapStr =
               malloc(
                  (lenBuffUL << 1) * sizeof(signed char)
               );

            if(! buffHeapStr)
            { /*If: had memory error*/
               tmpStr = errHeapStr;
               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     (signed char *)
                        "memory error setting up buffer\n"
                  );
               goto err_fun08_sec11_sub02;
            } /*If: had memory error*/

         } /*If: need larger buffer*/
 
         tmpQUI = tmpSeqUI;
         tmpStr = &buffHeapStr[tmpSeqUI];

         indel_rmHomo(
            &samStackST,
            refStackST.seqStr,
            ftbSetStackST.minHomoLenSI,
            ftbSetStackST.maxIndelLenSI,
            ftbSetStackST.homoMaskSC,
            &buffHeapStr,
            &tmpSeqUI,
            &tmpStr,
            &tmpQUI
         );
            /*I have resized my buffer, so that
            `  indel_rmHomo will never need to resize
            `  my buffers. So, a memory error will never
            `  happen
            */
      } /*If: cleaning up indels in reads*/

      /**************************************************\
      * Fun08 Sec07 Sub06:
      *   - build consensus
      \**************************************************/

      if(! ftbSetStackST.clustBl)
      { /*If: not doing mixed infection detection*/
         errSC =
            addRead_tbCon(
               &samStackST,
               &conNtHeapAryST,
               (unsigned int *) &lenRefSI,
               &ftbSetStackST.tbConSet
            ); /*add read to consnesus*/
               
         if(errSC & def_memErr_tbConDefs)
         { /*If: memory error*/
            tmpStr = errHeapStr;

            tmpStr +=
               cpStr_ulCp(
                  errHeapStr,
                  (signed char *)
                     "con memory error on read "
               );

          
            numToStr(
               tmpStr,
               totalReadsUI
            );

            goto err_fun08_sec11_sub02;
         } /*If: memory error*/
      } /*If: not doing mixed infection detection*/

      /**************************************************\
      * Fun08 Sec07 Sub07:
      *   - check for AMRs
      \**************************************************/

      amrHitHeapSTList =
         checkAmr(
            &samStackST,
            amrHeapAryST,
            numAmrSI,
            &numHitsSI,
            ftbSetStackST.frameshiftBl,
               /*scan for frameshifts*/
            0,              /*ignore aa indels in snp*/
            &errSC
         ); /*Check if read has antibiotic resistance*/

      if(errSC)
      { /*If: memory error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
                  "AMR check memory error on read "
            );

         numToStr(
            tmpStr,
            totalReadsUI
         );

         goto err_fun08_sec11_sub02;
      } /*If: memory error*/

      if(amrHitHeapSTList)
      { /*If: read had AMR(s)*/
         pIdVarTbl_checkAmr(
            samStackST.qryIdStr,
            amrHitHeapSTList,
            idFILE
         );

         freeHeapList_amrHit_checkAmr(amrHitHeapSTList);
         amrHitHeapSTList = 0;
      } /*If: read had AMR(s)*/

      /**************************************************\
      * Fun08 Sec07 Sub08:
      *   - check for MIRU lineages 
      \**************************************************/

      incLineage_miruTbl(
         &samStackST,
         ftbSetStackST.fudgeSI,
         miruHeapST
      );

      /**************************************************\
      * Fun08 Sec07 Sub09:
      *   - Check for spoligotypes
      \**************************************************/

      spolErrSC =
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            &samStackST,
            ftbSetStackST.drStartSI,
            ftbSetStackST.drEndSI,
            ftbSetStackST.spolPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &ftbSetStackST.alnSetST
         ); /*find spoligotype with kmer search*/

      spoligoNumReadsUI += (! spolErrSC);

      if(spolErrSC & def_memErr_tbConDefs)
      { /*If: memory error*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
                  "spoligo memory error on read "
            );

         numToStr(
            tmpStr,
            totalReadsUI
         );

         goto err_fun08_sec11_sub02;
      } /*If: memory error*/

      /**************************************************\
      * Fun08 Sec07 Sub10:
      *   - move to next read
      \**************************************************/

      nextRead_fun08_sec07_sub09:;

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: read anaylsis*/

   /*****************************************************\
   * Fun08 Sec07 Sub11:
   *   - minor clean up (variables unique to sec07)
   \*****************************************************/

   if(! ftbSetStackST.clustBl)
   { /*If: not doing mixed infection detection*/
      freeStack_samEntry(&samStackST);

      if(samFILE != stdin)
         fclose(samFILE);

      samFILE = 0;
   } /*If: not doing mixed infection detection*/

   fclose(idFILE);
   idFILE = 0;

   /*memory for masking primers*/
   free(maskStartHeapAryUI);
   maskStartHeapAryUI = 0;

   free(maskEndHeapAryUI);
   maskEndHeapAryUI = 0;

   free(maskFlagHeapAryUI);
   maskFlagHeapAryUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec08:
   ^   - print read data
   ^   o fun08 sec08 sub01:
   ^     - print unfiltered read stats
   ^   o fun08 sec08 sub02:
   ^     - print filtered read stats
   ^   o fun08 sec08 sub03:
   ^     - print AMR hits for reads
   ^   o fun08 sec08 sub04:
   ^     - print read MIRU table
   ^   o fun08 sec08 sub05:
   ^     - print read spoligotype entry
   ^   o fun08 sec08 sub06:
   ^     - print tsv file of variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec08 Sub01:
   *   - print unfiltered read stats
   \*****************************************************/

   sort_geneCoord(
      coordsHeapST,
      0,
      (unsigned int) numCoordsSI
   );

   outFILE =
      fopen(
         (char *) readStatsStr,
         "w"
      ); /*already check if could open*/

   phead_ampDepth(outFILE);

   phist_ampDepth(
      readMapArySI,
      1,            /*using 1 so all data is printed*/      
      coordsHeapST,
      numCoordsSI,
      offTargReadsSI,
      noMapReadSI,
      ftbSetStackST.depthFlagStr,
      outFILE
    ); /*print the unfilterd read stats*/

   free(readMapArySI);
   readMapArySI = 0;

   /*****************************************************\
   * Fun08 Sec08 Sub02:
   *   - print filtered read stats
   \*****************************************************/

   /*adding a unique column name so I can compare
   `  filtered and unfiltered reads
   */
   tmpStr = spareStr;

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         ftbSetStackST.depthFlagStr,
         0,
         '\0'
      );

   cpDelim_ulCp(
      tmpStr,
      (signed char *) "-filt",
      0,
      '\0'
   ); /*marker for filtered reads*/

   phist_ampDepth(
      filt_readMapArySI,
      1,            /*using 1 so all data is printed*/      
      coordsHeapST,
      numCoordsSI,
      0,
      0,
      spareStr,
      outFILE
    ); /*print filterd read stats*/

   fclose(outFILE);
   outFILE = 0;

   free(filt_readMapArySI);
   filt_readMapArySI = 0;

   freeHeap_geneCoord(coordsHeapST);
   coordsHeapST = 0;

   /*****************************************************\
   * Fun08 Sec08 Sub03:
   *   - print AMR hits for reads
   \*****************************************************/

   outFILE =
      fopen(
         (char *) readAmrStr,
         "w"
      ); /*already checked if could open*/

   pReadHead_checkAmr(outFILE);

   pRead_checkAmr(
      (unsigned int) ftbSetStackST.tbConSet.minDepthSI,
      ftbSetStackST.minPercMapF,
         /*min % reads supporting AMR needed to keep*/
      (float) 0,
         /*not really usefull, so leaving out*/
      ftbSetStackST.amrIndelSupF,
         /*% support to keep indel AMR*/
      ftbSetStackST.amrFrameshiftSupF,
         /*% support to keep frameshift AMR*/
      ftbSetStackST.frameshiftBl,
         /*1: scanned for frameshifts; 0 no*/
      totalReadsUI,
         /*number reads mapped*/
      amrHeapAryST,
         /*has detected AMRs*/
      (unsigned int) numAmrSI,
         /*number AMRs in amrHeapAryST*/
      drugHeapAryStr,
         /*has drug names*/
      outFILE
         /*file to print to*/
   ); /*print AMRs detected in reads*/
      
   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun08 Sec08 Sub04:
   *   - print read MIRU table
   \*****************************************************/

   errSC =
      p_miruTbl(
         miruHeapST,
         readMiruStr
      ); /*print AMRs detected in reads*/

   if(errSC)
   { /*If: impossible case*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (signed char *) "miru memory error on read "
         );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            readMiruStr
         );
   } /*If: impossible case*/

   /*print out "best" lineage*/
   tmpStr = readMiruStr;

   while(*tmpStr != '\0')
      ++tmpStr;

   while(*tmpStr != '.')
      --tmpStr;

   *tmpStr++ = 'l';
   *tmpStr++ = 'i';
   *tmpStr++ = 'n';
   *tmpStr++ = '.';
   *tmpStr++ = 't';
   *tmpStr++ = 's';
   *tmpStr++ = 'v';
   *tmpStr++ = '\0';

    errSC =
      plineages_miruTbl(
         miruHeapST,
         readMiruStr
      );

   /*****************************************************\
   * Fun08 Sec08 Sub05:
   *   - print read spoligotype entry
   \*****************************************************/

   spoligoOutFILE =
      fopen(
         (char *) outReadSpoligoFileStr,
         "w"
      );

   phead_spolST(
      1,             /*print fragment header*/
      spoligoOutFILE
   );

   if(spoligoNumReadsUI > 0)
   { /*If: detected spoligotypes*/
      pspol_spolST(
         ftbSetStackST.prefixStr,
         spoligoAryUI,
         1,                /*fragment print*/
         spoligoNumReadsUI,/*reads with 1 or more spacer*/
         lineageHeapAryST,
         numLineagesSI,
         spoligoOutFILE
      );
   } /*If: detected spoligotypes*/

   fclose(spoligoOutFILE);
   spoligoOutFILE = 0;
   spolErrSC = 0;

   for(
      errSL = 0;
      errSL < def_lenSpolAry_fun08;
      ++errSL
   ) spoligoAryUI[errSL] = 0;

   spoligoAryUI[errSL] = -1;
   errSL = 0;
   
   /*****************************************************\
   * Fun08 Sec08 Sub06:
   *   - print tsv file of variants
   \*****************************************************/

   /*Build the tsv of variants table*/
   if(! ftbSetStackST.clustBl)
      errSC =
         pvar_tbCon(
            conNtHeapAryST,
            lenRefSI,
            refIdStr,
            &ftbSetStackST.tbConSet,
            conTsvStr
         ); /*print variants (not a vcf)*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec09:
   ^   - collapse consensus and consensus analysis
   ^   o fun08 sec09 sub01:
   ^     - collapse consensus
   ^   o fun08 sec09 sub02:
   ^     - print consensus and do ananlysis
   ^   o fun08 sec09 sub03:
   ^     - close output files and free uneeded variables
   ^   o fun08 sec09 sub04:
   ^     - print consensus MIRU lineages
   ^   o fun08 sec09 sub05:
   ^     - print detected spoligotype (consensus)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec09 Sub01:
   *   - collapse consensus
   \*****************************************************/

   if(ftbSetStackST.clustBl)
      goto mixedInfect_fun08_sec11;

   samConSTAry =
      collapse_tbCon(
         conNtHeapAryST,
         lenRefSI,
         &numFragSI,
         refIdStr,
         &ftbSetStackST.tbConSet,
         &errSC
      ); /*collapse consesus*/

   if(conNtHeapAryST)
      freeHeapAry_conNt_tbCon(
         conNtHeapAryST,
         lenRefSI
      );

   conNtHeapAryST = 0;

   if(! samConSTAry)
   { /*If: error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *) "could not collapse consensus"
      );

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec09 Sub02:
   *   - print consensus and do ananlysis
   *   o fun08 sec09 sub04 cat01:
   *     - open files + run consensus fragment loop
   *   o fun08 sec09 sub04 cat02:
   *     - print consensus fragments
   *   o fun08 sec09 sub04 cat03:
   *     - AMR detection and printing
   *   o fun08 sec09 sub04 cat04:
   *     - MIRU-VNTR lineage detection and printing
   *   o fun08 sec09 sub04 cat05:
   *     - detect spoligotypes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec09 Sub04 Cat01:
   +   - open files + run consensus fragment loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   samConFILE =
      fopen(
         (char *) samConStr,
          "a"
      ); /*open consensus output file*/

   outFILE =
      fopen(
         (char *) conAmrStr,
         "w"
      );

   errSC = 0;

   /*remove all the read counters*/
   resetCnt_miruTbl(miruHeapST);

   pConHead_checkAmr(outFILE);

   spolErrSC = def_noSpol_tbSpolDefs;

   for(
      siCon = 0;
      siCon < numFragSI;
      ++siCon
   ){ /*Loop: print and analyize consensuses*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec09 Sub04 Cat02:
     +   - print consensus fragments
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errSC)
      { /*If: error*/
         freeStack_samEntry(&samConSTAry[siCon]);
         continue;
      } /*If: error*/

      if(samConSTAry[siCon].qryIdStr[0] == '\0')
      { /*If: blank structure*/
         freeStack_samEntry(&samConSTAry[siCon]);
         continue;
      } /*If: blank structure*/

      p_samEntry(
         &samConSTAry[siCon],
         &buffHeapStr,
         &lenBuffUL,
         0,          /*want a new line at end*/
         samConFILE
      ); /*print fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec09 Sub04 Cat03:
     +   - AMR detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      amrHitHeapSTList =
         checkAmr(
            &samConSTAry[siCon],
            amrHeapAryST,
            numAmrSI,
            &numHitsSI,
            ftbSetStackST.frameshiftBl,
            0,          /*ignore indels in aa snp AMRs*/
            &errSC
         ); /*check if fragments has AMRs*/

      if(errSC)
         continue; /*just ignore all other errors*/

      if(amrHitHeapSTList)
      { /*If: have AMRs*/
         pCon_checkAmr(
            samConSTAry[siCon].qryIdStr,
            amrHitHeapSTList,
            drugHeapAryStr,
            outFILE
         ); /*print detected AMRs*/

         freeHeapList_amrHit_checkAmr(amrHitHeapSTList);
         amrHitHeapSTList = 0;
      } /*If: have AMRs*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec09 Sub04 Cat04:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         &samConSTAry[siCon],
         ftbSetStackST.fudgeSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec09 Sub04 Cat05:
     +   - spoligotype detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     spolErrSC &=
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            &samConSTAry[siCon],
            ftbSetStackST.drStartSI,
            ftbSetStackST.drEndSI,
            ftbSetStackST.spolPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &ftbSetStackST.alnSetST
         ); /*find spoligotype with kmer search*/

      freeStack_samEntry(&samConSTAry[siCon]);
   } /*Loop: print and analyize consensuse*/

   /*****************************************************\
   * Fun08 Sec09 Sub03:
   *   - close output files and free uneeded variables
   \*****************************************************/

   fclose(outFILE);
   outFILE = 0;

   /*consensus memory*/
   free(buffHeapStr);
   buffHeapStr = 0;

   fclose(samConFILE);
   samConFILE = 0;

   free(samConSTAry); /*all structs are freeded*/
   numFragSI = 0;

   /*AMR memory*/
   freeHeapAry_amrST(
      amrHeapAryST,
      numAmrSI
   );

   amrHeapAryST = 0 ;

   free(drugHeapAryStr);
   drugHeapAryStr = 0;

   if(errSC)
   { /*If: error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "error during consensus analyisis step"
      );

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec09 Sub04:
   *   - print consensus MIRU lineages
   \*****************************************************/

   errSC =
      plineages_miruTbl(
         miruHeapST,
         conMiruStr
      );

   freeHeap_miruTbl(miruHeapST);
   miruHeapST = 0;

   if(errSC)
   { /*If: error*/
      tmpStr = errHeapStr;

      tmpStr += 
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "could not write con MIRU lineages "
         );

      cpStr_ulCp(
         tmpStr,
         conMiruStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec09 Sub05:
   *   - print detected spoligotype
   \*****************************************************/

   spoligoOutFILE =
      fopen(
         (char *) outSpoligoFileStr,
         "w"
      );

   phead_spolST(
      0,             /*print normal header*/
      spoligoOutFILE
   );

   if(! spolErrSC)
   { /*If: detected spoligotype*/
      pspol_spolST(
         ftbSetStackST.prefixStr,
         spoligoAryUI,
         0,                       /*non-fragment print*/
         1,                       /*1 supporting read*/
         lineageHeapAryST,
         numLineagesSI,
         spoligoOutFILE
      );
   } /*If: detected spoligotype*/

   freeHeapAry_spolST(
      lineageHeapAryST,
      numLineagesSI
   );

   lineageHeapAryST = 0;

   freeStack_tblST_kmerFind(&kmerTblStackST);

   freeHeapAry_refST_kmerFind(
      kmerRefAryST,
      numSpoligosSI
   );

   kmerRefAryST = 0;

   fclose(spoligoOutFILE);
   spoligoOutFILE = 0;

   goto ret_fun08_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec10:
   ^   - run mixed infection detection (if requested)
   ^   o fun08 sec10 sub01:
   ^     - check if can open log files
   ^   o fun08 sec10 sub02:
   ^     - run mixed infection detection
   ^   o fun08 sec10 sub03:
   ^     - print clusters for mixed infection
   ^   o fun08 sec10 sub04:
   ^     - amr/miru/spoligotype detection on clusters
   ^   o fun08 sec10 sub05:
   ^     - print consensus MIRU lineages
   ^   o fun08 sec10 sub06:
   ^     - print detected spoligotype
   \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec10 Sub01:
   *   - check if can open log files
   \*****************************************************/

   mixedInfect_fun08_sec11:;

   if(samFILE == stdin)
   { /*If: piping file to mixed infection detect*/
      tmpStr = errHeapStr;

      tmpStr += 
         cpStr_ulCp(
          errHeapStr,
          (signed char *)
             "do not use -sam stdin for clustering"
         );

      goto err_fun08_sec11_sub02;
   } /*If: piping file to mixed infection detect*/

   errSC =
      outputPath_freezeTBPaths(
         ftbSetStackST.prefixStr,
         (signed char *) "-mixed-infect-log.txt",
         logFileStr
      );

   if(errSC)
      logFILE = stderr;
   else
      logFILE =
         fopen(
            (char *) logFileStr,
            "w"
         );

   if(! logFILE)
   { /*If: could not open log flie*/
      fprintf(
         stderr,
         "could not open log file %s\n",
         logFileStr
      );

      goto err_fun08_sec11_sub02;
   } /*If: could not open log flie*/

   /*****************************************************\
   * Fun08 Sec10 Sub02:
   *   - run mixed infection detection
   \*****************************************************/

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   conListHeapST =
      cluster_edClust(
         &indexHeapST,
         &ftbSetStackST.clustSetST,
			&ftbSetStackST.tbConSet,
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE,
         logFILE, /*log file (re-using)*/
         &errSC
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_edClust)
         cpStr_ulCp(
            errHeapStr,
            (signed char *) "clustering memory error\n"
         );

      else if(errSC == def_fileErr_edClust)
         cpStr_ulCp(
            errHeapStr,
            (signed char *) "clustering file error\n"
         );

      else
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "clustering no reads in sam file\n"
         );

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   if(logFILE != stderr)
      fclose(logFILE);
   logFILE = 0;

   /*****************************************************\
   * Fun08 Sec10 Sub03:
   *   - print clusters for mixed infection
   \*****************************************************/

   samConFILE =
      fopen(
         (char *) samConStr,
          "a"
      ); /*open consensus output file*/

   errSC =
      plist_con_clustST(
         conListHeapST,
         0,
         0,
         &buffHeapStr,
         &lenBuffUL,
         samConFILE
      ); /*print consensus for clusters*/

   fclose(samConFILE);
   samConFILE = 0;

   if(errSC)
   { /*If: had error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "cluster memory error consensus print"
      );

      goto err_fun08_sec11_sub02;
   } /*If: had error*/

   errSC =
      pbins_clustST(
         ftbSetStackST.prefixStr,
         ftbSetStackST.clustSetST.clustSI,
         indexHeapST,
         0,            /*currently no program header*/
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE       /*has reads*/
      ); /*print consensus for clusters*/

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_clustST)
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "cluster memory error cluster print"
         );
      else
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "cluster file error cluster print"
         );

      goto err_fun08_sec11_sub02;
   } /*If: had error*/

   fclose(samFILE);
   samFILE = 0;

   if(indexHeapST)
      freeHeap_index_clustST(indexHeapST);
   indexHeapST = 0;

   /*****************************************************\
   * Fun08 Sec10 Sub04:
   *   - amr/miru/spoligotype detection on clusters
   *   o fun08 sec10 sub04 cat01:
   *     - amr detection + start loop
   *   o fun08 sec10 sub04 cat02:
   *     - MIRU-VNTR lineage detection and printing
   *   o fun08 sec10 sub04 cat03:
   *     - spoligotype detection and printing
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec10 Sub04 Cat01:
   +   - amr detection + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   conNodeST = conListHeapST;

   outFILE =
      fopen(
         (char *) conAmrStr,
         "w"
      ); /*file to print AMR results to*/

   pConHead_checkAmr(outFILE);

   errSC = 0;

   /*remove all the read counters*/
   resetCnt_miruTbl(miruHeapST);
   spolErrSC = def_noSpol_tbSpolDefs;


   while(conNodeST)
   { /*Loop: find amrs and lineages for consensuses*/
      amrHitHeapSTList =
         checkAmr(
            conNodeST->samSTPtr,
            amrHeapAryST,
            numAmrSI,
            &numHitsSI,
            ftbSetStackST.frameshiftBl,
            0,  /*skip AMRs with indels in snp AMRs*/
            &errSC
         ); /*check if fragments has AMRs*/

      if(errSC)
         continue; /*just ignore all other errors*/

      if(amrHitHeapSTList)
      { /*If: have AMRs*/
         pCon_checkAmr(
            conNodeST->samSTPtr->qryIdStr,
            amrHitHeapSTList,
            drugHeapAryStr,
            outFILE
         ); /*print detected AMRs*/

         freeHeapList_amrHit_checkAmr(amrHitHeapSTList);
         amrHitHeapSTList = 0;
      } /*If: have AMRs*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec10 Sub04 Cat02:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         conNodeST->samSTPtr,
         ftbSetStackST.fudgeSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec10 Sub04 Cat03:
     +   - spoligotype detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     spolErrSC &=
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            conNodeST->samSTPtr,
            ftbSetStackST.drStartSI,
            ftbSetStackST.drEndSI,
            ftbSetStackST.spolPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &ftbSetStackST.alnSetST
         ); /*find spoligotype with kmer search*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun08 Sec10 Sub04 Cat04:
     +   - move to next cluster
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     conNodeST = conNodeST->nextST;
   } /*Loop: find amrs and lineages for consensuses*/

   if(conListHeapST)
      freeHeapList_con_clustST(conListHeapST);
   conListHeapST = 0;
   conNodeST = 0;
         
   /*****************************************************\
   * Fun08 Se109 Sub05:
   *   - print consensus MIRU lineages
   \*****************************************************/

   errSC =
      plineages_miruTbl(
         miruHeapST,
         conMiruStr
      );

   freeHeap_miruTbl(miruHeapST);
   miruHeapST = 0;

   if(errSC)
   { /*If: error*/
      tmpStr = errHeapStr;

      tmpStr +=
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "consensus MIRU print error: "
         );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            conMiruStr
         );

      goto err_fun08_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun08 Sec10 Sub06:
   *   - print detected spoligotype
   \*****************************************************/

   spoligoOutFILE =
      fopen(
         (char *) outSpoligoFileStr,
         "w"
      );

   phead_spolST(
      0,             /*print normal header*/
      spoligoOutFILE
   );

   if(! spolErrSC)
   { /*If: detected spoligotype*/
      pspol_spolST(
         ftbSetStackST.prefixStr,
         spoligoAryUI,
         0,                       /*non-fragment print*/
         1,                       /*1 supporting read*/
         lineageHeapAryST,
         numLineagesSI,
         spoligoOutFILE
      );
   } /*If: detected spoligotype*/

   fclose(spoligoOutFILE);
   spoligoOutFILE = 0;

   goto ret_fun08_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec11:
   ^   - clean up
   ^   o fun08 sec11 sub01:
   ^     - no error clean up
   ^   o fun08 sec11 sub02:
   ^     - error clean up
   ^   o fun08 sec11 sub03:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec11 Sub01:
   *   - no error clean up
   \*****************************************************/

   ret_fun08_sec11:;
      errSC = 0;
      free(errHeapStr);
      errHeapStr = 0;
      goto cleanUp_fun08_sec11_sub03;

   /*****************************************************\
   * Fun08 Sec11 Sub02:
   *   - error clean up
   \*****************************************************/

   err_fun08_sec11_sub02:;
      errSC = 1;
      goto cleanUp_fun08_sec11_sub03;

   /*****************************************************\
   * Fun08 Sec11 Sub03:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun08_sec11_sub03:;
      freeStack_seqST(&refStackST);
      freeStack_set_freezeTB(&ftbSetStackST);
      freeStack_samEntry(&samStackST);

      for(
         siCon = 0;
         siCon < numFragSI;
         ++siCon
      ) freeStack_samEntry(&samConSTAry[siCon]);
        /*make sure all consenuses are freeded*/

      if(coordsHeapST)
         freeHeap_geneCoord(coordsHeapST);

      coordsHeapST = 0;

      if(amrHeapAryST)
         freeHeapAry_amrST(
            amrHeapAryST,
            numAmrSI
         );

      amrHeapAryST = 0;

      if(amrHitHeapSTList)
         freeHeapList_amrHit_checkAmr(amrHitHeapSTList);

      amrHitHeapSTList = 0;

      if(miruHeapST)
         freeHeap_miruTbl(miruHeapST);

      miruHeapST = 0;

      if(conNtHeapAryST)
         freeHeapAry_conNt_tbCon(
            conNtHeapAryST,
            lenRefSI
         );

      conNtHeapAryST = 0;

      if(indexHeapST)
         freeHeap_index_clustST(indexHeapST);
      indexHeapST = 0;

      if(conListHeapST)
         freeHeapList_con_clustST(conListHeapST);
      conListHeapST = 0;

      if(lineageHeapAryST)
         freeHeapAry_spolST(
            lineageHeapAryST,
            numLineagesSI
         );

      lineageHeapAryST = 0;

      freeStack_tblST_kmerFind(&kmerTblStackST);

      if(kmerRefAryST)
         freeHeapAry_refST_kmerFind(
            kmerRefAryST,
            numSpoligosSI
         );

      kmerRefAryST = 0;

      if(buffHeapStr)
         free(buffHeapStr);

      buffHeapStr = 0;

      if(readMapArySI)
         free(readMapArySI);

      readMapArySI = 0;

      if(filt_readMapArySI)
         free(filt_readMapArySI);

      filt_readMapArySI = 0;

      if(drugHeapAryStr)
         free(drugHeapAryStr);

      drugHeapAryStr = 0;

      if(maskStartHeapAryUI)
         free(maskStartHeapAryUI);

      maskStartHeapAryUI = 0;

      if(maskEndHeapAryUI)
         free(maskEndHeapAryUI);

      maskEndHeapAryUI = 0;

      if(maskFlagHeapAryUI)
         free(maskFlagHeapAryUI);

      maskFlagHeapAryUI = 0;

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
            samConFILE
         && samConFILE != stdin
         && samConFILE != stdout
      ) fclose(samConFILE);

      samConFILE = 0;

      if(
            idFILE
         && idFILE != stdin
         && idFILE != stdout
      ) fclose(idFILE);

      idFILE = 0;

      if(
            spoligoOutFILE
         && spoligoOutFILE != stdin
         && spoligoOutFILE != stdout
      ) fclose(spoligoOutFILE);

      spoligoOutFILE = 0;

      if(
            logFILE
         && logFILE != stdin
         && logFILE != stdout
         && logFILE != stderr
      ) fclose(logFILE);

      logFILE = 0;

      return errHeapStr;
} /*run_freezeTB*/

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
