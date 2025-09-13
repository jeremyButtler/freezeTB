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
'   o fun04: freeStack_set_freezeTB
'     - frees variables in a set_freezeTB stack struct
'   o fun05: freeHeap_set_freezeTB
'     - frees a set_freezeTB stack struct
'   o fun06: pversion_freezeTB
'     - prints version number for freezeTB and |submodules
'   o .c fun07: phelp_freezeTB
'     - prints help message for freezeTB
'   o .c fun08: input_freezeTB
'     - gets user input
'   o fun09: run_freezeTB:
'     - drives everything, but not fun09 (for tcltk)
'   o .h note01:
'     - windows enviromental variables
'   o license:
'     - licensing for this code (public dofun09 / mit)
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
#include "../genBio/rmHomo.h"
#include "../genBio/tbCon.h"

/* these are needed for read mapping*/
#include "../genLib/inflate.h"
#include "../genBio/gzSeqST.h"
#include "../genAln/mapRead.h"

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
#include "miruTbl.h"

#include "spolFind.h" 
#include "spolST.h"

#include "amrST.h"
#include "checkAmr.h"

/*freezeTB core*/
#include "freezeTBPaths.h" /*getting default file paths*/

/********************************************************\
* Header Sec01 Sub04:
*   - libraries without .c files
\********************************************************/

#include "../genLib/endLine.h"
#include "../ftbVersion.h"

/*default settings*/
#include "freezeTBDefs.h"
#include "tbAmrDefs.h"
#include "tbMiruDefs.h"
#include "tbSpolDefs.h"/*includes defaults*/
#include "../genBio/tbConDefs.h" /*settings/error values*/
#include "../genAln/mapRead.h" /*read mapping settings*/

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
!   o .c  #include "../genLib/fileFun.h"
!   o .c  #include "../genLib/endin.h"
!   o .c  #include "../genLib/checkSum.h"
!   o .c  #include "../genBio/codonFun.h"
!   o .c  #include "../genBio/kmerFun.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/edDist.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/dirMatrix.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../genAln/water.h"
!   o .c  #include "../genAln/needle.h"
!   o .c  #include "../tbAmrSrc/drugAry.h"
!
!   o .h  #include "../genBio/ntTo2bit.h" 
!   o .h  #include "../genBio/revNtTo2bit.h" 
!   o .h  #include "../genBio/ntTo5bit.h" 
!   o .h  #include "../genBio/codonTbl.h"
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
   struct set_mapRead mapSetST;   /*mapRead settings*/
   struct set_clustST clustSetST; /*cluster settings*/
   signed char clustBl;           /*enable clustering*/

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
   init_set_mapRead(&setFTBST->mapSetST);

   setFTBST->clustBl = def_mixedInfect_freezeTBDefs;


   /*copy database paths*/
   amrPath_freezeTBPaths(setFTBST->amrDbFileStr);
   coordPath_freezeTBPaths(setFTBST->coordFileStr);
   miruPath_freezeTBPaths(setFTBST->miruDbFileStr);
   maskPath_freezeTBPaths(setFTBST->maskPrimFileStr);
   refPath_freezeTBPaths(setFTBST->refFileStr);
   spolLineagePath_freezeTBPaths(setFTBST->spolDBFileStr);
   spolSpacerPath_freezeTBPaths(setFTBST->spolRefFileStr);

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
| Fun03: setup_set_freezeTB
|   - allocates memory for ftb settings variables that
|     need memroy
| Input:
|   - setFTBST:
|     o pointer to set_freezeTB struct to set up
| Ouput:
|   - Modifies:
|     o mapSetST to have memory for the alignment matrix
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error
\-------------------------------------------------------*/
signed char
setup_set_freezeTB(
   struct set_freezeTB *setFTBST
){
   if(! setFTBST)
      return 0;
   else if(setup_set_mapRead(&setFTBST->mapSetST))
      return 1;
   else
      return 0;
} /*freeStack_set_freezeTB*/

/*-------------------------------------------------------\
| Fun04: freeStack_set_freezeTB
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
   freeStack_set_mapRead(&setFTBST->mapSetST);
   init_set_freezeTB(setFTBST);
} /*freeStack_set_freezeTB*/

/*-------------------------------------------------------\
| Fun05: freeHeap_set_freezeTB
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
| Fun06: pversion_freezeTB
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
       "freezeTB version: %i-%02i-%02i%s",
       def_year_ftbVersion,
       def_month_ftbVersion,
       def_day_ftbVersion,
       str_endLine
   ); /*Version of freezeTB*/
} /*pversion_freezeTB*/

/*-------------------------------------------------------\
| Fun07: phelp_freezeTB
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
   ' Fun07 TOC:
   '   - print help message
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find default file paths
   '   o fun07 sec03:
   '     - usage
   '   o fun07 sec04:
   '     - input
   '   o fun07 sec05:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
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
   ^ Fun07 Sec02:
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
   ^ Fun07 Sec03:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   fprintf(
      (FILE *) outFILE,
      "fastx input: freezeTB -prefix %s ...",
      def_prefix_freezeTB
   );
   fprintf(
      (FILE *) outFILE,
      " /path/to/fastq_pass/barcodexx/*.fastq*%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "sam file input: freezeTB -sam reads.sam -prefix"
   );
   fprintf(
      (FILE *) outFILE, " %s%s",
      def_prefix_freezeTB,
      str_endLine
   );


   fprintf(
     (FILE *) outFILE,
     "  - tuberculosis AMR, spoligotype, and MIRU VNTR%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    analysis pipeline (for ONT Nanopore reads)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - NOT CLIA CERTIFIED; RESEARCH USE ONLY%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - input entry
   ^   o fun07 sec04 sub01:
   ^     - input header/File IO input
   ^   o fun07 sec04 sub02:
   ^     - fitering settings
   ^   o fun07 sec04 sub03:
   ^     - indel clean up (rmHomo) settings
   ^   o fun07 sec04 sub04:
   ^     - consensus settings
   ^   o fun07 sec04 sub05:
   ^     - clustering settings
   ^   o fun07 sec04 sub06:
   ^     - variant/read AMR printing settings
   ^   o fun07 sec04 sub07:
   ^     - AMR settings
   ^   o fun07 sec04 sub08:
   ^     - lineage (non-database) settings
   ^   o fun07 sec04 sub09:
   ^     - other settings and help message/version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *   - input header/File IO input
   *   o fun07 sec04 sub01 cat01:
   *     - input header
   *   o fun07 sec04 sub01 cat02:
   *     - sam file input
   *   o fun07 sec04 sub01 cat03:
   *     - prefix to name output files
   *   o fun07 sec04 sub01 cat04:
   *     - AMR table input
   *   o fun07 sec04 sub01 cat05:
   *     - gene coordinates
   *   o fun07 sec04 sub01 cat06:
   *     - MIRU table input
   *   o fun07 sec04 sub01 cat07:
   *     - spoligotype spacer sequence input
   *   o fun07 sec04 sub01 cat08:
   *     - spoligotype linage database
   *   o fun07 sec04 sub01 cat09:
   *     - primer masking database
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat01:
   +   - input header and FILE IO header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf((FILE *) outFILE,"  File IO: %s",str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat02:
   +   - sam file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "    -sam reads.sam: [Replaces fastq file input]:%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o sam file with reads mapped to NC000962%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat03:
   +   - prefix to name output files
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -prefix %s: [Optional; %s]%s",
      def_prefix_freezeTB,
      def_prefix_freezeTB,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix to name all output files with%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat04:
   +   - AMR table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(amrDbFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -amr-tbl amr-catalog.tsv: [Required]%s",
         str_endLine
      );

   else
      fprintf(
         (FILE *) outFILE,
         "    -amr-tbl amr-catalog.tsv: [Using %s]%s",
         amrDbFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o AMR mutation database for AMRs detection%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat05:
   +   - gene coordinates
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(coordFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -gene-coords coordinates.tsv: [Required]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -gene-coords coordinates.tsv: [Using %s]%s",
         coordFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o gene coordinate table (for NC000962.3)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat06:
   +   - MIRU table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(miruTblFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -miru-tbl MIRU-tbl.tsv: [Required]%s",
      str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -miru-tbl MIRU-tbl.tsv: [Using %s]%s",
         miruTblFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o miru table (tbMiru) to detect lineages"
   );
   fprintf((FILE *) outFILE, " with%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat07:
   +   - spoligotype spacer sequence input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spolRefFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Required]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Using %s]%s",
         spolRefFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o fasta file with direct repeat spacer%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        sequences for spoligotyping%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat08:
   +   - spoligotype linage database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spolDBFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Required]%s",
         str_endLine
      );

   else
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Using %s]%s",
         spolDBFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o csv file with spoligotype lineages%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o format: strain, barcode, ignore,%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        lineage, SIT, countries%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat09:
   +   - primer masking database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(maskPrimFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -mask-prim primer-mask.tsv: [Optional]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -mask-prim primer-mask.tsv: [Using %s]%s",
         maskPrimFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o tsv file with primer positions to mask%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-prim-mask -\" to disable%s",
      str_endLine
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
      "\treverse_start\treverse_end%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub01 Cat10:
   +   - reference for indel clean up
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(maskPrimFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -ref ref.fasta: [Required for fastq]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -ref reference.fasta: [Using %s]%s",
         refFileStr,
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o reference to use in indel clean up%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o required for fastq input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec04 Sub02:
   *   - fitering settings
   *   o fun07 sec04 sub02 cat01:
   *     - filter header
   *   o fun07 sec04 sub02 cat02:
   *     - mapping quality filter
   *   o fun07 sec04 sub02 cat03:
   *     - min length filter
   *   o fun07 sec04 sub02 cat04:
   *     - mean Q-score filter
   *   o fun07 sec04 sub02 cat05:
   *     - median Q-score filter
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub02 Cat01:
   +   - fitering header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pFiltBl)
      goto skipReadFilt_fun07_sec04_sub04;

   fprintf(
      (FILE *) outFILE,
      "  Read Filtering:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub02 Cat02:
   +   - mapping quality filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mapq %u: [Optional; %u]%s",
      def_minMapq_tbConDefs,
      def_minMapq_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mapping quality to keep a read%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub02 Cat03:
   +   - min length filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]%s",
      def_minLen_tbConDefs,
      def_minLen_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum alinged length keep a read%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub02 Cat04:
   +   - mean q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mean-q %i: [Optional; %i]%s",
      def_minMeanQ_freezeTBDefs,
      def_minMeanQ_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mean q-score to keep a read%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub02 Cat05:
   +   - median Q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-median-q %i: [Optional; %i]%s",
      def_minMedianQ_freezeTBDefs,
      def_minMedianQ_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum median q-score to keep a read%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec04 Sub03:
   *   - indel clean up (rmHomo) settings
   *   o fun07 sec04 sub03 cat01:
   *     - rmHomo header
   *   o fun07 sec04 sub03 cat02:
   *     - indel clean up boolean
   *   o fun07 sec04 sub03 cat03:
   *     - indel clean up minimum homopolymer size
   *   o fun07 sec04 sub03 cat04:
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub03 Cat01:
   +   - rmHomo header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  Indel cleanup:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub03 Cat02:
   +   - indel clean up boolean
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_indelClean_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -rmHomo: [enabled]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -rmHomo: [disabled]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o remove indels in homoplyers%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-rmHomo\"%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub03 Cat03:
   +   - indel clean up minimum homopolymer size
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -rmHomo-homo %i: [%i]%s",
      def_minHomo_freezeTBDefs,
      def_minHomo_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum homopolymer size to remove indel"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub03 Cat04:
   +   - indel clean up maximum indel size
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -rmHomo-indel %i: [%i]%s",
      def_maxIndel_freezeTBDefs,
      def_maxIndel_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum indel size to remove%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec04 Sub04:
   *   - consensus settings
   *   o fun07 sec04 sub04 cat01:
   *     - consensus setting header
   *   o fun07 sec04 sub04 cat02:
   *     - min depth filter
   *   o fun07 sec04 sub04 cat03:
   *     - min length to keep a fragment
   *   o fun07 sec04 sub04 cat04:
   *     - min snp/match Q-score
   *   o fun07 sec04 sub04 cat05:
   *     - min insertion Q-score
   *   o fun07 sec04 sub04 cat06:
   *     - min percdent snp/match support
   *   o fun07 sec04 sub04 cat07:
   *     - min percdent insertion support
   *   o fun07 sec04 sub04 cat08:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat01:
   +   - consensus setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipReadFilt_fun07_sec04_sub04:;

   if(! pConBl)
       goto skipConSet_fun07_sec04_sub05_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Consensus settings:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat02:
   +   - min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-depth %i: [Optional; %i]%s",
      def_minDepth_tbConDefs,
      def_minDepth_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to keep a base%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat03:
   +   - min length to keep a fragment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]%s",
      def_minLen_tbConDefs,
      def_minLen_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum length to keep consensus fragment"
   );

   fprintf(
      (FILE *) outFILE,
      "%s",
      str_endLine
   );


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat04:
   +   - min snp/match Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-q %i: [Optional; %i]%s",
      def_minNtQ_tbConDefs,
      def_minNtQ_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum Q-score to keep an match or snp%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat05:
   +   - min insertion Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-q-ins %i: [Optional; %i]%s",
      def_minInsQ_tbConDefs,
      def_minInsQ_tbConDefs,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum Q-score to keep an inserted base%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat06:
   +   - min percdent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-snp-sup %.2f: [Optional; %.2f]%s",
      def_minSnpPerc_tbConDefs,
      def_minSnpPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% mapped reads to keep a snp%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat07:
   +   - min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-ins-sup %.2f: [Optional; %.2f]%s",
      def_minInsPerc_tbConDefs,
      def_minInsPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% mapped reads to keep an ins%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub04 Cat08:
   +   - min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-del-sup %.2f: [Optional; %.2f]%s",
      def_minDelPerc_tbConDefs,
      def_minDelPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum %% of mapped reads to keep a del%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec04 Sub05:
   *   - clustering settings
   *   o fun07 sec04 sub05 cat01:
   *     - print clustering setting header
   *   o fun07 sec04 sub05 cat02:
   *     - read scoring settings
   *   o fun07 sec04 sub05 cat03:
   *     - minimum cluster depth
   *   o fun07 sec04 sub05 cat04:
   *     - minimum cluster percent depth
   *   o fun07 sec04 sub05 cat05:
   *     - minimum percent difference between reads
   *   o fun07 sec04 sub05 cat06:
   *     - minimum consensus to read percent difference
   *   o fun07 sec04 sub05 cat07:
   *     - maximum percent similarity between consensuses
   *   o fun07 sec04 sub05 cat08:
   *     - minimum percent overlap between consensuses
   *   o fun07 sec04 sub05 cat09:
   *     - maximum percent maksing in consensus
   *   o fun07 sec04 sub05 cat10:
   *     - number of consensus rebuilds
   *   o fun07 sec04 sub05 cat11:
   *     - depth profiling (edit distance setting start)
   *   o fun07 sec04 sub05 cat12:
   *     - minimum depth profile depth (edDist)
   *   o fun07 sec04 sub05 cat13:
   *     - minimum error:variant ratio to be different
   *   o fun07 sec04 sub05 cat14:
   *     - length of window (edDist)
   *   o fun07 sec04 sub05 cat15:
   *     - window variant:error ratio (for difference)
   *   o fun07 sec04 sub05 cat16:
   *     - minimum indel length to keep indel in edDist
   *   o fun07 sec04 sub05 cat17:
   *     - minimum q-score to count snp as different
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat01:
   +   - print clustering setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipConSet_fun07_sec04_sub05_cat01:;

   if(! pClustBl)
      goto skipClustSet_fun07_sec04_sub06_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Clustering Settings:%s",
      str_endLine
   );

   if(def_mixedInfect_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -clust: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -clust: [Optional; No]%s",
         str_endLine
      );
 
   fprintf(
      (FILE *) outFILE,
      "      o detect mixed infections; no stdin input%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-clust\"%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat02:
   +   - read scoring settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    Read Scoring:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      -len-weight %0.2f: [Optional; %0.2f]%s",
      def_lenWeight_clustST,
      def_lenWeight_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o how much to increase lengths impact on"
   );

   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "          read scoring%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "          o log_length = floor[log10(length)]%s",
      str_endLine
   );

   fprintf( (FILE *) outFILE, "        o length_score =");

   fprintf(
      (FILE *) outFILE,
      " floor[weigth * log_length]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        o score = length_score + floor[median Q]%s",
     str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat03:
   +   - minimum cluster depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    General Clustering:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      -clust-depth %i: [Optional; %i]%s",
      def_minDepth_clustST,
      def_minDepth_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o min cluster read depth to keep cluster%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat04:
   +   - minimum cluster percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "      -clust-perc-depth %.04f: [Optional; %.04f]%s",
     def_minPercDepth_clustST,
     def_minPercDepth_clustST,
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        o minimum percent read depth need to keep"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "          a cluster%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat05:
   +   - minimum percent difference between reads
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -read-err %0.3f: [Optional; %0.3f]%s",
      def_readErrRate_clustST,
      def_readErrRate_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o error rate for read to read mapping%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat06:
   +   - minimum consensus to read percent difference
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-err %0.3f: [Optional; %0.3f]%s",
      def_conErrRate_clustST,
      def_conErrRate_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o consensus to read mapping error rate%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat07:
   +   - maximum percent similarity between consensuses
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-sim %0.4f: [Optional; %0.4f]%s",
      def_maxConSim_clustST,
      def_maxConSim_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum simularity between consensuses%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "          to not merge clusters%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat08:
   +   - minimum percent overlap between consensuses
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -overlap %0.2f: [Optional; %0.2f]%s",
      def_percOverlap_clustST,
      def_percOverlap_clustST,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        o minimum percentage of overlap needed to"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "       merge clusters (start comparision)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o used in final consensus comparison%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat09:
   +   - maximum percent maksing in consensus
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -perc-n %0.4f: [Optional; %0.4f]%s",
      def_maxNPerc_clustST,
      def_maxNPerc_clustST,
      str_endLine
    );

   fprintf(
      (FILE *) outFILE,
      "      o maximum percent masked consnesus bases%s",
      str_endLine
   ); /*both consensus and edit distance*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat10:
   +   - number of consensus rebuilds
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -con-iter %i: [Optional; %i]%s",
      def_conRebuild_clustST,
      def_conRebuild_clustST,
      str_endLine
    );

   fprintf(
      (FILE *) outFILE,
      "      o number times to rebuild cluster (con)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat11:
   +   - depth profiling (edit distance setting start)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    Edit Distance:%s",
      str_endLine
   );

   if(def_depthProfile_clustST)
      fprintf(
         (FILE *) outFILE,
         "      -depth-prof: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "      -depth-prof: [Optional; No]%s",
         str_endLine
      );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat12:
   +   - minimum depth profile depth (edDist)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "        o -clust-depth to remove low read depth%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "          variants from edit distance%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o disable with \"no-depth-prof\"%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat13:
   +   - minimum error:variant ratio to be different
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -err-to-var %i: [Optional; %i]%s",
      def_maxVarErrRatio_clustST,
      def_maxVarErrRatio_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum variant to error ratio to add%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "          read to a cluster (whole number)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o 100*(# variants)/(error rate * length)%s",
      str_endLine
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat14:
   +   - length of window (edDist)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -win-len %i: [Optional; %i]%s",
      def_window_clustST,
      def_window_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o size of window for window scanning%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o idea is to detect regions of high%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "          difference in very long reads%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat15:
   +   - window variant:error ratio (for difference)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -win-err %i: [Optional; %i]%s",
      def_windowError_clustST,
      def_windowError_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o maximum window variant to error ratio%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat16:
   +   - minimum indel length to keep indel in edDist
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -indel-len %i: [Optional; %i]%s",
      def_indelLen_clustST,
      def_indelLen_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o min indel length to count as different"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub05 Cat17:
   +   - minimum q-score to count snp as different
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "      -clust-q-snp %i: [Optional; %i]%s",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        o minimum q-score to keep snp; edit dist"
   ); /*edit distance min snp q-score*/
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*****************************************************\
   * Fun07 Sec04 Sub06:
   *   - variant printing settings (depth is AMR)
   *   o fun07 sec04 sub06 cat01:
   *     - variant/readAMR print setting header
   *   o fun07 sec04 sub06 cat02:
   *     - min depth filter
   *   o fun07 sec04 sub06 cat03:
   +     - min percent of reads to keep read AMR
   *   o fun07 sec04 sub06 cat04:
   *     - min perccent snp/match support
   *   o fun07 sec04 sub06 cat05:
   *     - min percent insertion support
   *   o fun07 sec04 sub06 cat06:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub06 Cat01:
   +   - variant/readAMR print setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipClustSet_fun07_sec04_sub06_cat01:;

   if(! pPrintBl)
      goto skipPrintSet_fun07_sec04_sub07_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Print settings:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub06 Cat02:
   +   - print min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-min-depth %i: [Optional; %i]%s",
      def_minPrintDepth_tbConDefs,
      def_minPrintDepth_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o for variant tsv; mininum read depth to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        print variant%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub06 Cat04:
   +   - min percent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-snp-sup %.2f: [Optional; %.2f]%s",
      def_minSnpPrintPerc_tbConDefs,
      def_minSnpPrintPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv; min %% of reads to print%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        snp/match%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub06 Cat05:
   +   - min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-ins-sup %.2f: [Optional; %.2f]%s",
      def_minInsPrintPerc_tbConDefs,
      def_minInsPrintPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv;min %% of reads to print in%s",
      str_endLine
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub06 Cat06:
   +   - min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-del-sup %.2f: [Optional: %.2f]%s",
      def_minDelPrintPerc_tbConDefs,
      def_minDelPrintPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o variant tsv;min %% of reads to print del"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*****************************************************\
   * Fun07 Sec04 Sub07:
   *   - AMR settings
   *   o fun07 sec04 sub07 cat01:
   *     - min percent of reads to keep read AMR
   *   o fun07 sec04 sub07 cat02:
   *     - min percent of reads to keep indel
   *   o fun07 sec04 sub07 cat03:
   *     - checking frameshifts
   *   o fun07 sec04 sub07 cat04:
   *     - frameshift support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub07 Cat01:
   +   - min percent of reads to keep read AMR
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-amr-map-perc %.2f: [Optional; %.2f]%s",
      def_minPercMapped_freezeTBDefs,
      def_minPercMapped_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o read AMRs; min %% of mapped reads to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        print an AMR%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub07 Cat02:
   +   - min percent of reads to keep indel
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -amr-indel-sup %.2f: [Optional; %.2f]%s",
      def_amrIndelSup_freezeTBDefs,
      def_amrIndelSup_freezeTBDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent to keep indel AMR%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub07 Cat03:
   +   - checking frameshifts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_frameshift_freezeTBDefs)
      fprintf(
         (FILE *) outFILE,
         "    -frameshift: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -frameshift: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o off; frame shift AMRs are extact matches"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "      o disable: with -no-frameshift (recommend)"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub07 Cat04:
   +   - frameshift support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -frameshift-sup %0.2f: [Optional; %0.2f]%s",
       def_supFrameshift_freezeTBDefs,
       def_supFrameshift_freezeTBDefs,
       str_endLine
    );

   fprintf(
      (FILE *) outFILE,
      "      o minimum support to keep a frameshift AMR"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*****************************************************\
   * Fun07 Sec04 Sub08:
   *   - lineage settings
   *   o fun07 sec04 sub08 cat01:
   *     - lineage setting block
   *   o fun07 sec04 sub08 cat02:
   *     - MIRU fudge setting
   *   o fun07 sec04 sub08 cat03:
   *     - spoligtyping min perc score
   *   o fun07 sec04 sub08 cat04:
   *     - spoligtyping dr-start
   *   o fun07 sec04 sub08 cat05:
   *     - spoligtyping dr-end
   \*****************************************************/

   skipPrintSet_fun07_sec04_sub07_cat01:;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub08 Cat01:
   +   - lineage setting block
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pLinBl)
      goto skipLineageSet_fun07_sec04_sub09;

   fprintf(
      (FILE *) outFILE,
      "  Lineage settings:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub08 Cat02:
   +   - MIRU fudge setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fudge %i: [Optional; %i]%s",
      def_fudgeLen_tbMiruDefs,
      def_fudgeLen_tbMiruDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o fudge read lengths by to match a lineage"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "      o read length range is + or - fudge%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub08 Cat03:
   +   - spoligtyping min perc score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -spoligo-min-score %.2f: [Optional; %.2f]%s",
      def_minPercScore_tbSpolDefs,
      def_minPercScore_tbSpolDefs,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum percent score needed to count a%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        spoligo spaceer as mapped%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub08 Cat04:
   +   - spoligtyping dr-start
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -dr-start %i: [Optional; %i]%s",
      def_DRStart_tbSpolDefs,
      def_DRStart_tbSpolDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o start of reference direct repeat region%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub08 Cat05:
   +   - spoligtyping dr-end
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -dr-end %i: [Optional; %i]%s",
      def_DREnd_tbSpolDefs,
      def_DREnd_tbSpolDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o end of reference direct repeat region%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec04 Sub09:
   *   - other settings and help message/version number
   *   o fun07 sec04 sub09 cat01:
   *     - other settings header
   *   o fun07 sec04 sub09 cat02:
   *     - flag for graph
   *   o fun07 sec04 sub09 cat03:
   *     - default help message
   *   o fun07 sec04 sub09 cat04:
   *     - addons for help message
   *   o fun07 sec04 sub09 cat05:
   *     - version number
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub09 Cat01:
   +   - Other settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipLineageSet_fun07_sec04_sub09:;

   fprintf(
      (FILE *) outFILE,
      "  Other settings:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub09 Cat02:
   +   - flag for graph
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pOtherBl)
      goto skipOtherSet_fun07_sec04_sub09_cat04;
   
   fprintf(
      (FILE *) outFILE,
      "    -depth-flag %s: [Optional; %s]%s",
      def_depthFlag_freezeTB,
      def_depthFlag_freezeTB,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o label for experiment in depth tsv/graphs%s",
      str_endLine
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "      o do not use spaces or tabs%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub09 Cat03:
   +   - default help message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipOtherSet_fun07_sec04_sub09_cat04:;

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
         "    -h: default help message and exit%s",
         str_endLine
      );
   } /*If: not default help message*/

   else
   { /*Else: not default help message*/
      fprintf(
         (FILE *) outFILE,
         "    -h: this help message and exit%s",
         str_endLine
      );
   } /*Else: not default help message*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub09 Cat04:
   +   - addons for help message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pFiltBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-filt: add consensus settings to -h%s",
         str_endLine
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-filt: print this help message and exit%s",
        str_endLine
      );

   if(pConBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-con: add consensus settings to -h%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -h-con: print this help message and exit%s",
         str_endLine
      );

   if(pLinBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-lin: add lineage settings to -h%s",
         str_endLine
      );
   else
      fprintf(
        (FILE *) outFILE,
        "    -h-lin: print this help message and exit%s",
        str_endLine
      );


   if(pPrintBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-print: add print settings to -h%s",
         str_endLine
      );
   else
     fprintf(
       (FILE *) outFILE,
       "    -h-print: print this help message and exit%s",
      str_endLine
     );

   if(pClustBl)
      fprintf(
         (FILE *) outFILE,
         "    -h-clust: add cluster settings to -h%s",
         str_endLine
      );
   else
     fprintf(
       (FILE *) outFILE,
       "    -h-clust: print this help message and exit%s",
       str_endLine
     );

   if(pOtherBl)
     fprintf(
       (FILE *) outFILE,
       "    -h-other: add uncatogorized settings to -h%s",
       str_endLine
     );
   else
     fprintf(
       (FILE *) outFILE,
       "    -h-other: print this help message and exit%s",
       str_endLine
     );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec04 Sub09 Cat07:
   +   - version number
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
    (FILE *) outFILE,
    "    -v: print version number of freezeTB and exit%s",
    str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - output
   ^   o fun07 sec05 sub01:
   ^     - output header
   ^   o fun07 sec05 sub02:
   ^     - consensus amrs file
   ^   o fun07 sec05 sub04:
   ^     - consensus mirufile
   ^   o fun07 sec05 sub05:
   ^     - consensus file
   ^   o fun07 sec05 sub06:
   ^     - variants file
   ^   o fun07 sec05 sub07:
   ^     - reads amr table
   ^   o fun07 sec05 sub08:
   ^     - reads ids of AMR reads
   ^   o fun07 sec05 sub09:
   ^     - reads MIRU table
   ^   o fun07 sec05 sub09:
   ^     - filtered histogram file
   ^   o fun07 sec05 sub10:
   ^     - unfiltered histogram file
   ^   o fun07 sec05 sub11:
   ^     - clusters with reads
   ^   o fun07 sec05 sub12:
   ^     - log (for mixed infect)
   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec05 Sub01:
   *   - output header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub02:
   *   - consensus amrs file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-con-amrs.tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with detected consensus AMRs%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub04:
   *   - consensus mirufile
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-con-miru.tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with detected consensus MIRU lineage%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub05:
   *   - consensus file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-consensuses.sam %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with consensus sequences%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub06:
   *   - variants file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-consensuses.tsv %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with variants for each consensus base%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub07:
   *   - reads amr table
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-read-amrs.tsv %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with AMRs found in the reads%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub08:
   *   - reads ids of AMR reads
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-id-amrs.tsv %s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o tsv file with ids of reads that have AMRs%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub09:
   *   - reads MIRU table
   \*****************************************************/


   fprintf(
      (FILE *) outFILE,
      "  - prefix-miru-read-tbl.tsv %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with MIRU read count table%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub09:
   *   - filtered histogram file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-depths-filter.tsv %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with mean read depths per gene%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      after read filtering step%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub10:
   *   - unfiltered histogram file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-depths.tsv %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv with mean read depths per gene%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      before read filtering step%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub11:
   *   - clusters with reads
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-reference-number.sam %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with clusters (mixed infect)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun07 Sec05 Sub12:
   *   - log (for mixed infect)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - prefix-log.txt %s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o log for mixed infection detection step%s",
      str_endLine
   );
} /*phelp_freezeTB*/

/*-------------------------------------------------------\
| Fun08: input_freezeTB
|   - gets user input
| Input:
|   User Input:
|     - numArgsSI:
|       o number of arguments user input
|     - argAryStr:
|       o pointer to array of c-strings with users input
|     - samFileStr:
|       o pointer to c-string to point to input sam file
|     - fqIndexSIPtr:
|       o pointer to signed in to get index of first
|         fastq file in user input
|       o set to 0 if no fastq files were input
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
   char *argAryStr[],
   signed char **samFileStr,
   signed int *fqIndexSIPtr,
   struct set_freezeTB *ftbSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC: input_freezeTB
   '   - gets user input
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - check if have user input
   '   o fun08 sec03:
   '     - get user input
   '   o fun08 sec04:
   '     - return error type
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;
   signed char errSC = 0;
   signed char helpBl = 0; /*print help message at end*/
   FILE *testFILE = 0;

   /*booleans for extra help message entries*/
   signed char pFiltHelpBl = 0;
   signed char pConHelpBl = 0;
   signed char pPrintHelpBl = 0;
   signed char pClustHelpBl = 0;
   signed char pLinHelpBl = 0;
   signed char pOtherHelpBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - check if have user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *fqIndexSIPtr = 0;

   if(numArgsSI <= 1)
   { /*If: nothing was input*/
      phelp_fun08_sec02:;

      phelp_freezeTB(
         pFiltHelpBl,
         pConHelpBl,
         pPrintHelpBl,
         pClustHelpBl,
         pLinHelpBl,
         pOtherHelpBl,
         stdout
      );

      goto help_fun08_sec04;
   } /*If: nothing was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - get user input
   ^   o fun08 sec03 sub01:
   ^     - file input + start loop
   ^   o fun08 sec03 sub02:
   ^     - consensus options
   ^   o fun08 sec03 sub03:
   ^     - printing settings (for consensus tsv)
   ^   o fun08 sec03 sub04:
   ^     - indel clean up settings
   ^   o fun08 sec03 sub05:
   ^     - tbAmr settings
   ^   o fun08 sec03 sub06:
   ^     - read filtering
   ^   o fun08 sec03 sub07:
   ^     - lineages and graph output
   ^   o fun08 sec03 sub08:
   ^     - cluster settings
   ^   o fun08 sec03 sub09:
   ^     - check for help message requests (normal)
   ^   o fun08 sec03 sub10:
   ^     - check for add on help message requests
   ^   o fun08 sec03 sub11:
   ^     - check for version number requests
   ^   o fun08 sec03 sub12:
   ^     - set fastq file index
   ^   o fun08 sec03 sub13:
   ^     - invalid input
   ^   o fun08 sec03 sub14:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec03 Sub01:
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
         if(*fqIndexSIPtr)
         { /*If: using both sam file and fastq file*/
            fprintf(
                stderr,
                "can not read both sam and fastq files%s",
                str_endLine
            );
            goto err_fun08_sec04;
         } /*If: using both sam file and fastq file*/

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
      * Fun08 Sec03 Sub02:
      *   - consensus options
      *   o fun08 sec03 sub02 cat01:
      *     - consensus q-score settinngs
      *   o fun08 sec03 sub02 cat02:
      *     - consensus min depth/length settings
      *   o fun08 sec03 sub02 cat03:
      *     - consensus min base percent support settings
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat01:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-q %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: had an error*/

         if(
               ftbSetSTPtr->tbConSet.minQSI > 93
            || ftbSetSTPtr->tbConSet.minQSI < 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-q %s; not between 0 and 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-q-ins %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(
               ftbSetSTPtr->tbConSet.minInsQSI > 93
            || ftbSetSTPtr->tbConSet.minInsQSI < 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-q-ins %s; not between 0 and 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum insertion q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat02:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minDepthSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-depth; must be greater than 0%s",
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-len %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minLenSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-len; must be greater than 0%s",
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum fragment length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat03:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-snp-sup %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-ins-sup %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-del-sup %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum % del support*/

      /**************************************************\
      * Fun08 Sec03 Sub03:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-p-min-depth %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minPrintDepthSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-p-min-depth %s; must be at least 0%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
              stderr,
              "-p-perc-snp-sup %s; not between 0 and 1%s",
              argAryStr[siArg],
              str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
              stderr,
              "-p-perc-ins-sup %s; not between 0 and 1%s",
              argAryStr[siArg],
              str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
              stderr,
              "-p-perc-del-sup %s; not between 0 and 1%s",
              argAryStr[siArg],
              str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: print minimum % snp support*/

      /**************************************************\
      * Fun08 Sec03 Sub04:
      *   - indel clean up
      *   o fun08 sec03 sub04 cat01:
      *     - do indel clean up
      *   o fun08 sec03 sub04 cat02:
      *     - indel clean up min hompolymer length
      *   o fun08 sec03 sub04 cat03:
      *     - indel clean up maximum indel length
      *   o fun08 sec03 sub04 cat04:
      *     - indel clean up reference
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub04 Cat02:
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
      + Fun08 Sec03 Sub04 Cat02:
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
               "-rmHomo-homo %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun08_sec04;
         } /*If: non-numeric*/

         else if(
            ftbSetSTPtr->minHomoLenSI <= 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-rmHomo-homo %s must be greater than 0%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      }  /*Else If: minimum homopolymer length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub04 Cat03:
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
               "-rmHomo-indel %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun08_sec04;
         } /*If: non-numeric*/

         else if(
            ftbSetSTPtr->maxIndelLenSI <= 0
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-rmHomo-indel %s must be at least 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      }  /*Else If: maximum indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub04 Cat04:
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
      * Fun08 Sec03 Sub05:
      *   - AMR settings
      *   o fun08 sec03 sub05 cat01:
      *     - minimum AMR mapping percent
      *   o fun08 sec03 sub05 cat02:
      *     - minimum indel percent support
      *   o fun08 sec03 sub05 cat03:
      *     - frameshift setting/filtering
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub05 Cat01:
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
               goto phelp_fun08_sec02;

            fprintf(
             stderr,
             "-min-amr-map-per %s; not between 0 and 1%s",
             argAryStr[siArg],
             str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: AMR min percent mapped reads*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub05 Cat02:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-amr-indel-sup %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      }  /*Else If: min indel % support*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub05 Cat03:
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
               goto phelp_fun08_sec02;

            fprintf(
              stderr,
              "-frameshift-sup %s; not between 0 and 1%s",
              argAryStr[siArg],
              str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      }  /*Else If: min % frameshift support*/


      /**************************************************\
      * Fun08 Sec03 Sub06:
      *   - read filterting
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->tbConSet.minMapqUC > 93)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-mapq %s; not between 0 and 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-median-q %s; not between 0 and 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-min-mean-q %s; not between 0 and 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: min mean q-score*/

      /**************************************************\
      * Fun08 Sec03 Sub07:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-fudge %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(
               ftbSetSTPtr->fudgeSI < 0
            || ftbSetSTPtr->fudgeSI > 100
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-fudge %s; must be between 0 and 100%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
            stderr,
            "-spoligo-min-score %s not between 0 and 1%s",
            argAryStr[siArg],
            str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-dr-start %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->drStartSI < 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-dr-start %s must be at least 0%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-dr-end %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: error*/

         if(ftbSetSTPtr->drEndSI <= 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-dr-end %s must be greater than 0%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: end of direct repeat region*/

      /**************************************************\
      * Fun08 Sec03 Sub08:
      *   - clustering settings
      *   o fun08 sec03 sub08 cat01:
      *     - user requested clustering?
      *   o fun08 sec03 sub08 cat02:
      *     - minimum length weight for read scoring
      *   o fun08 sec03 sub08 cat03:
      *     - depth profiling
      *   o fun08 sec03 sub08 cat04:
      *     - minium cluster depth
      *   o fun08 sec03 sub08 cat05:
      *     - minium cluster percent depth
      *   o fun08 sec03 sub08 cat06:
      *     - minium read:read percent distance
      *   o fun08 sec03 sub08 cat07:
      *     - minium consensus:read percent distance
      *   o fun08 sec03 sub08 cat08:
      *     - maximum consensus similarity (before merge)
      *   o fun08 sec03 sub08 cat09:
      *     - maximum error to variant percent (edDist)
      *   o fun08 sec03 sub08 cat10:
      *     - minimum overlap between consensuses
      *   o fun08 sec03 sub08 cat11:
      *     - window size for window scan
      *   o fun08 sec03 sub08 cat12:
      *     - window variant:error ratio
      *   o fun08 sec03 sub08 cat13:
      *     - mimimum indel length to count as difference
      *   o fun08 sec03 sub08 cat14:
      *     - mimimum q-score to keep snp
      *   o fun08 sec03 sub08 cat15:
      *     - maximum percent of consensus maksed
      *   o fun08 sec03 sub08 cat16:
      *     - number of times to rebuild consensus
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat01:
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
      + Fun08 Sec03 Sub08 Cat02:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-len-weigth %s must be at least 0%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: how much length influces score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat03:
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
      + Fun08 Sec03 Sub08 Cat04:
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
               "-clust-depth %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/

         if(ftbSetSTPtr->clustSetST.minDepthUI <= 0)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-clust-depth %s must be at least 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: min cluster read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat05:
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
               goto phelp_fun08_sec02;

            fprintf(
             stderr,
             "-clust-perc-depth %s not between 0 and 1%s",
             argAryStr[siArg],
             str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: cluster min percent read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat06:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-read-err %s not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: error rate for read to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat07:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-con-err %s not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
     } /*Else If: error rate for con to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat08:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-con-sim %s not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: maximum similarity between cons*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat09:
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
               "-err-to-var %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum error to variant ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat10:
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
               "-overlap %s goes to 0; invalid input%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/

         if(
               ftbSetSTPtr->clustSetST.percOverlapF < 0
            || ftbSetSTPtr->clustSetST.percOverlapF > 1
         ){ /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-overlap %s not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: minimum overlap*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat11:
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
               "-win-len %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/
      } /*Else If: window length for window scanning*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat12:
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
               "-win-err %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/
      } /*Else If: window error to varaint ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat13:
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
               "-indel-len %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: invalid input*/
      } /*Else If: min indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat14:
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
             "-clust-q-snp %s; non-numeric or to large%s",
             argAryStr[siArg],
             str_endLine
            );

           goto err_fun08_sec04;
         } /*If: invalid input*/

         if(ftbSetSTPtr->clustSetST.minSnpQUC > 93)
         { /*If: value out of range*/
            if(helpBl)
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-clust-q-snp %s not between 0 to 93%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: min snp q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat15:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-perc-n %s not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: value out of range*/
      } /*Else If: maximum % masking*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub08 Cat16:
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
               goto phelp_fun08_sec02;

            fprintf(
               stderr,
               "-con-iter %s; not numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun08_sec04;
         } /*If: had error*/
      } /*Else If: number of cosensus rebuilds*/


      /**************************************************\
      * Fun08 Sec03 Sub09:
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
      * Fun08 Sec03 Sub10:
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
      * Fun08 Sec03 Sub11:
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
         goto pversion_fun08_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun08_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun08_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun08_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun08_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun08 Sec03 Sub12:
      *   - set fastq file index
      \**************************************************/

      else if(argAryStr[siArg][0] != '-')
      { /*Else If: likely query fastx files*/
         if(! *fqIndexSIPtr)
            *fqIndexSIPtr = siArg;

         if(argAryStr[siArg][0] == '>')
            goto done_fun08_sec04;
            /*at stdout redirect to file*/

         testFILE = fopen(argAryStr[siArg], "r");

         if(! testFILE)
         { /*If: could not open file*/
            fprintf(
               stderr,
               "could not open file %s%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun08_sec04;
         } /*If: could not open file*/

         fclose(testFILE);
         testFILE = 0;

         if(*samFileStr)
         { /*If: using both sam file and fastq file*/
            fprintf(
                stderr,
                "can not read both sam and fastq files%s",
                str_endLine
            );
            goto err_fun08_sec04;
         } /*If: using both sam file and fastq file*/
      } /*Else If: likely query fastx files*/

      else if(! argAryStr[siArg][1])
      { /*Else If: standard input*/
         if(*samFileStr)
         { /*If: using both sam file and fastq file*/
            fprintf(
                stderr,
                "can not read both sam and fastq files%s",
                str_endLine
            );
            goto err_fun08_sec04;
         } /*If: using both sam file and fastq file*/

         if(! *fqIndexSIPtr)
            *fqIndexSIPtr = siArg; /*stdin input*/
      } /*Else If: standard input*/

      /**************************************************\
      * Fun08 Sec03 Sub13:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         if(helpBl)
            goto phelp_fun08_sec02;

         fprintf(
            stderr,
            "%s is not recognized%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun08_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun08 Sec03 Sub14:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: read in user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - return errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun08_sec04:;
      errSC = 0;
      if(helpBl)
         goto phelp_fun08_sec02;
         /*user wanted help message*/
         /*this allows for full help message printing*/
      goto ret_fun09_sec04;
   
   help_fun08_sec04:;
   pversion_fun08_sec04:;
      errSC = 1;
      goto ret_fun09_sec04;

   err_fun08_sec04:;
      errSC = 2;
      goto ret_fun09_sec04;

   ret_fun09_sec04:;
      return errSC;
} /*input_freezeTB*/

/*-------------------------------------------------------\
| Fun09: run_freezeTB
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
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - Run freezeTB on user input
   '   o fun09 sec01:
   '     - Variable declerations
   '   o fun09 sec02:
   '     - initialize, get input, and set up memory
   '   o fun09 sec03:
   '     - check user input database (if can open)
   '   o fun09 sec04:
   '     - check output files (can I open?)
   '   o fun09 sec05:
   '     - read in databases
   '   o fun09 sec06:
   '     - get reference stats and print consensus header
   '   o fun09 sec07:
   '     - Do read analysis
   '   o fun09 sec08:
   '     - print read data
   '   o fun09 sec09:
   '     - collapse consensus and consensus analysis
   '   o fun09 sec10:
   '     - run mixed infection detection (if requested)
   '   o fun09 sec11:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - Variable declerations
   ^   o fun09 sec01 sub01:
   ^     - general IO variables (applies to multple subs)
   ^   o fun09 sec01 sub02:
   ^     - temporay and error reporting variables
   ^   o fun09 sec01 sub03:
   ^     - filtering and sam file variables (adjust coord)
   ^   o fun09 sec01 sub04:
   ^     - read depth and coverage stats variables
   ^   o fun09 sec01 sub05:
   ^     - AMR detection variables
   ^   o fun09 sec01 sub06:
   ^     - miru lineage unique variables
   ^   o fun09 sec01 sub07:
   ^     - spoligotyping unique variables
   ^   o fun09 sec01 sub08:
   ^     - consensus building/mixed infection variables
   ^   o fun09 sec01 sub09:
   ^     - masking unique variables
   ^   o fun09 sec01 sub10:
   ^     - indel clean up (rmHomo)
   ^   o fun09 sec01 sub11:
   ^     - read mapping (mapRead)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec01 Sub01:
   *   - general IO variables (applies to multple subs)
   \*****************************************************/

   FILE *outFILE = 0;
   struct set_freezeTB ftbSetStackST; /*has settings*/

   /*****************************************************\
   * Fun09 Sec01 Sub02:
   *   - Temporay and error reporting variables
   \*****************************************************/

   signed char *tmpStr = 0;

   signed char errSC = 0;
   signed long errSL = 0;

   signed char *errHeapStr = 0;

   /*****************************************************\
   * Fun09 Sec01 Sub03:
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
   * Fun09 Sec01 Sub04:
   *   - read depth and coverage stats variables
   \*****************************************************/

   signed char readStatsStr[def_lenFileName_freezeTB];
      /*output file name*/

   struct geneCoord *coordsHeapST = 0;
   signed int numCoordsSI = 0;
   unsigned int lastBaseUI = 0;

   signed int *readMapArySI = 0;
   signed int offTargReadsSI = 0;
   signed int noMapReadSI = 0;

   /*****************************************************\
   * Fun09 Sec01 Sub05:
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
   * Fun09 Sec01 Sub06:
   *   - miru lineage unique variables
   \*****************************************************/

   struct miruTbl *miruHeapST = 0;

   signed char readMiruStr[def_lenFileName_freezeTB];
       /*read results output*/
   signed char conMiruStr[def_lenFileName_freezeTB];
      /*consensus results output*/

   /*****************************************************\
   * Fun09 Sec01 Sub07:
   *   - Spoligotyping unique variables
   \*****************************************************/

   signed char checkSpoligoLinBl = 1;
      /*set to 0 if failed to load linage database*/

   #define def_lenSpolAry_fun09 128
   unsigned int spoligoAryUI[def_lenSpolAry_fun09 + 1];
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
   * Fun09 Sec01 Sub08:
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
   * Fun09 Sec01 Sub09:
   *   - masking unique variables
   \*****************************************************/

   unsigned int *maskStartHeapAryUI = 0;
   unsigned int *maskEndHeapAryUI = 0;
   unsigned int *maskFlagHeapAryUI = 0;
   unsigned int maskNumPrimUI = 0;

   /*****************************************************\
   * Fun09 Sec01 Sub10:
   *   - indel clean up variables
   \*****************************************************/

   /*for indel clean up step*/
   signed char *rmHomoRefStr = 0;
   unsigned int tmpSeqUI = 0;
   unsigned int tmpQUI = 0;
   struct seqST refStackST;

   /*****************************************************\
   * Fun09 Sec01 Sub11:
   *   - read mapping
   \*****************************************************/

   signed char mapSamStr[def_lenFileName_freezeTB];

   signed char mapErrSC = def_EOF_seqST;
   signed char forErrSC = 0;
   signed char revErrSC = 0;

   signed int ignoreSI = 0; /*needed to fill input value*/
   signed int fqIndexSI = 0; /*index of 1st fastq file*/
   signed long forScoreSL = 0; /*forward alignment score*/
   signed long revScoreSL = 0; /*reverse alignment score*/
   signed char stdinBl = 0;    /*read stdin already*/
   signed char fileTypeSC = 0; /*current file type*/

   struct file_inflate gzFileStackST;/*for uncompression*/
   struct ref_mapRead mapRefStackST;
   struct aln_mapRead mapAlnStackST;

   struct seqST readSeqStackST;
   struct samEntry revSamStackST;

   FILE *fqFILE = 0; 

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - initialize, get input, and set up memory
   ^   o fun09 sec02 sub01:
   ^     - initialize variables
   ^   o fun09 sec02 sub02:
   ^     - get input
   ^   o fun09 sec02 sub03:
   ^     - set up memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec02 Sub01:
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

   /*for mapRead*/
   init_file_inflate(&gzFileStackST);
   init_ref_mapRead(&mapRefStackST);
   init_aln_mapRead(&mapAlnStackST);
   init_seqST(&readSeqStackST);
   init_samEntry(&revSamStackST);

   /*initialize spoligotyping array*/
   for(
      numLineagesSI = 0;
      numLineagesSI < def_lenSpolAry_fun09;
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
   * Fun09 Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_freezeTB(
         numArgsSI,
         argAryStr,
         &samFileStr,      /*reads*/
         &fqIndexSI,       /*first fastq file*/ 
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

         goto err_fun09_sec11_sub02;
      } /*If: had input error*/

      else
         goto ret_fun09_sec11;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec02 Sub03:
   *   - set up memory
   *   o fun09 sec02 sub03 cat01:
   *     - general setup
   *   o fun09 sec02 sub03 cat02:
   *     - mapRead setup
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec02 Sub03 Cat01:
   +   - general setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! errHeapStr)
   { /*If: hadd memory error*/
      fprintf(
         stderr,
         "memory error setting error report string%s",
         str_endLine
      );

      goto err_fun09_sec11_sub02;
   } /*If: hadd memory error*/


   if(setup_set_freezeTB(&ftbSetStackST))
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
             "memory error freezeTB setting struct setup"
      );

      goto err_fun09_sec11_sub02;
   } /*If: memory error*/

   if( setup_samEntry(&samStackST) )
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
             "memory error samEntry struct setup"
      );

      goto err_fun09_sec11_sub02;
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

      goto err_fun09_sec11_sub02;
   } /*If: memory error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec02 Sub03 Cat02:
   +   - mapRead setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(fqIndexSI)
   { /*If: mapping reads*/
      ftbSetStackST.tbConSet.minMapqUC = 0;
      ftbSetStackST.clustSetST.minMapqUC = 0;

      if( setup_aln_mapRead(&mapAlnStackST) )
      { /*If: memory error*/
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memroy error setting map read structs"
         );

         goto err_fun09_sec11_sub02;
      } /*If: memory error*/

      if( setup_samEntry(&revSamStackST) )
      { /*If: memory error*/
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memroy error setting map read structs"
         );

         goto err_fun09_sec11_sub02;
      } /*If: memory error*/
   } /*If: mapping reads*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - check user input database (if can open)
   ^   o fun09 sec03 sub01:
   ^     - check if MIRU table exists
   ^   o fun09 sec03 sub02:
   ^     - check if spoligotyping spacer sequences exists
   ^   o fun09 sec03 sub03:
   ^     - check if spoligotyping lineage database
   ^   o fun09 sec03 sub04:
   ^     - check if amr table exists
   ^   o fun09 sec03 sub05:
   ^     - open the sam file
   ^   o fun09 sec03 sub06:
   ^     - check if gene coordinates file exits
   ^   o fun09 sec03 sub07:
   ^     - read in reference sequence
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - check if MIRU table exists
   \*****************************************************/

   outFILE =
      fopen((char *) ftbSetStackST.miruDbFileStr, "r");

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

      goto err_fun09_sec11_sub02;
   } /*If: I could not open the MIRU table*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - check if spoligotyping spacer sequences exists
   \*****************************************************/

   outFILE =
      fopen((char *) ftbSetStackST.spolRefFileStr, "r");

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

      goto err_fun09_sec11_sub02;
   } /*If: could not open spoligo spacer sequences*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun09 Sec03 Sub03:
   *   - check if spoligotyping lineage database
   \*****************************************************/

   outFILE =
      fopen((char *) ftbSetStackST.spolDBFileStr, "r");

   if(! outFILE)
   { /*If: could not open the spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -db-spoligo %s%s",
         ftbSetStackST.spolDBFileStr,
         str_endLine
      );

      fprintf(
         stderr,
         "spoligotype lineages will be ignored%s",
         str_endLine
      );

      checkSpoligoLinBl = 0;
   } /*If: could not open the spoligo spacer sequences*/

   else
      fclose(outFILE); /*valid lineage database*/

   outFILE = 0;

   /*****************************************************\
   * Fun09 Sec03 Sub04:
   *   - check if amr table exists
   \*****************************************************/

   
   outFILE =
      fopen((char *) ftbSetStackST.amrDbFileStr, "r");

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

      goto err_fun09_sec11_sub02;
   } /*If: could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun09 Sec03 Sub05:
   *   - open sam file
   \*****************************************************/
   
   if(fqIndexSI)
      ;
   else if(! samFileStr)
      samFILE = stdin;
   else if(samFileStr[0] == '-')
      samFILE = stdin;
   else
   { /*Else: need to open sam file*/
      samFILE = fopen((char *) samFileStr, "r");

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

         goto err_fun09_sec11_sub02;
      } /*If: could not open the sam file*/
   } /*Else: need to open sam file*/

   /*****************************************************\
   * Fun09 Sec03 Sub06:
   *   - open gene coordinates file
   \*****************************************************/
   
   outFILE = 
      fopen((char *) ftbSetStackST.coordFileStr, "r");

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

      goto err_fun09_sec11_sub02;
   } /*If: could not open gene coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Fun09 Sec03 Sub07
   *   - read in reference sequence
   *   o fun09 sec03 sub07 cat01:
   *     - open reference file
   *   o fun09 sec03 sub07 cat02:
   *     - get reference sequence for rmHomo only
   *   o fun09 sec03 sub07 cat03:
   *     - get reference sequence for mapRead and rmHomo
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec03 Sub07 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! ftbSetStackST.indelCleanBl && samFileStr) ;
   else
   { /*Else: need reference (rmHomo or mapRead)*/
      outFILE =
         fopen((char *) ftbSetStackST.refFileStr, "r");

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

         goto err_fun09_sec11_sub02;
      } /*If: unable to open reference file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec03 Sub07 Cat02:
      +   - get reference sequence for rmHomo only
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(! fqIndexSI)
      { /*If: not using mapRead*/
         errSC = getFa_seqST(outFILE, &refStackST);
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

            cpStr_ulCp(tmpStr, ftbSetStackST.refFileStr);
            goto err_fun09_sec11_sub02;
         } /*Else: error reading reference*/
      } /*If: not using mapRead*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec03 Sub07 Cat03:
      +   - get reference sequence for rmHomo and mapRead
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: mapping reads, could be using rmHomo to*/
         errSC =
            getRef_ref_mapRead(
               &mapRefStackST,
               &ftbSetStackST.mapSetST,
               0, /*input is fasta file*/
               0, /*not used (no gz support)*/
               outFILE
            );

          fclose(outFILE);
          outFILE = 0;

          if(errSC)
          { /*Else: error reading reference*/
             tmpStr = errHeapStr;

             tmpStr +=
                cpStr_ulCp(
                   errHeapStr,
                   (signed char *)
                   "could not get reference from -ref "
                );

             cpStr_ulCp(tmpStr, ftbSetStackST.refFileStr);
             goto err_fun09_sec11_sub02;
          } /*Else: error reading reference*/
      } /*Else: mapping reads, could be using rmHomo to*/
   } /*Else: need reference (rmHomo or mapRead)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - check output files (can I open?)
   ^   o fun09 sec04 sub01:
   ^     - output file for read stats
   ^   o fun09 sec04 sub02:
   ^     - set up cosensus fragments output file
   ^   o fun09 sec04 sub03:
   ^     - set up read AMRs table outp file name
   ^   o fun09 sec04 sub04:
   ^     - set up read id AMR hit table
   ^   o fun09 sec04 sub05:
   ^     - output file for the AMRs found in consensus
   ^   o fun09 sec04 sub06:
   ^     - set up MIRU reads table output name
   ^   o fun09 sec04 sub07:
   ^     - set up MIRU consensus table output name
   ^   o fun09 sec04 sub08:
   ^     - set up consensus spoligotyping output file
   ^   o fun09 sec04 sub09:
   ^     - set up read spoligotyping output file name
   ^   o fun09 sec04 sub10:
   ^     - set up open consensus output file name
   ^   o fun09 sec04 sub12:
   ^     - set up sam file name (if mapping reads)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec04 Sub01:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/
 
   /*****************************************************\
   * Fun09 Sec04 Sub02:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub03:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub04:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub05:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub06:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub07:
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

      goto err_fun09_sec11_sub02;
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub08:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub09:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Fun09 Sec04 Sub10:
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

      goto err_fun09_sec11_sub02;
   } /*If: could not open file*/

   samConFILE = fopen((char *) samConStr, "w");

   /*****************************************************\
   * Fun09 Sec04 Sub11:
   *   - set up sam file name (if mapping reads)
   \*****************************************************/

   if(fqIndexSI)
   { /*If: mapping reads*/
      errSC =
         outputPath_freezeTBPaths(
            ftbSetStackST.prefixStr,
            (signed char *) ".sam",
            mapSamStr
         );

      if(errSC)
      { /*If: could not open file*/
         tmpStr = errHeapStr;

         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *)
                  "unable to open sam file output: "
            );

         cpStr_ulCp(
            tmpStr,
            mapSamStr
         );

         goto err_fun09_sec11_sub02;
      } /*If: could not open file*/

      samFILE = fopen((char *) mapSamStr, "w");
   } /*If: mapping reads*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - read in databases
   ^   o fun09 sec05 sub01:
   ^     - get gene mapping coodiantes
   ^   o fun09 sec05 sub02:
   ^     - get amr table
   ^   o fun09 sec05 sub03:
   ^     - get MIRU lineage table
   ^   o fun09 sec05 sub04:
   ^     - get spoligotyping spacer sequences
   ^   o fun09 sec05 sub05:
   ^     - get spoligotyping lineages
   ^   o fun09 sec05 sub06:
   ^     - get masking primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec05 Sub01:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   lenBuffUL = 0;
   lastBaseUI = coordsHeapST->endAryUI[numCoordsSI];

   /*****************************************************\
   * Fun09 Sec05 Sub02:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec05 Sub03:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec05 Sub04:
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
 
         goto err_fun09_sec11_sub02;
      } /*If: file error*/
 
      else
      { /*Else: memory error*/
         cpStr_ulCp(
            errHeapStr,
            (signed char *)
               "memory error getting spoligo seqs"
         );
 
          goto err_fun09_sec11_sub02;
      } /*Else: memory error*/
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec05 Sub05:
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
               "could not open -db-spoligo %s%s",
               ftbSetStackST.spolDBFileStr,
               str_endLine
            );

            fprintf(
               stderr,
               "skipping spoligotype checks%s",
               str_endLine
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

            goto err_fun09_sec11_sub02;
         } /*Else: memory error*/
      } /*If: error*/
   } /*If: have lineage database*/

   /*****************************************************\
   * Fun09 Sec05 Sub06:
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

         goto err_fun09_sec11_sub02;
      } /*If: error*/
   } /*If: primer masking file was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec06:
   ^   - get reference stats and print consensus header
   ^   o fun09 sec06 sub01:
   ^     - get reference name/length from header
   ^   o fun09 sec06 sub02:
   ^     - print tbCon header for sam file
   ^   o fun09 sec06 sub03:
   ^     - check if have reference name/length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec06 Sub01:
   *   - get reference length from header
   *   o fun09 sec06 sub01 cat01:
   *     - get first sam file entry + start loop
   *   o fun09 sec06 sub01 cat02:
   *     - print comment entry (end loop if not comment)
   *   o fun09 sec06 sub01 cat03:
   *     - if sequence entry; get id and length
   *   o fun09 sec06 sub01 cat04:
   *     - move to next entry
   *   o fun09 sec06 sub01 cat05:
   *     - check for errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec06 Sub01 Cat01:
   +   - get first sam file entry + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(fqIndexSI)
      goto pFTBHeader_fun09_sec06_sub02_cat01;

   errSC = get_samEntry(&samStackST, samFILE);

   while(! errSC)
   { /*Loop: read in header*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec06 Sub01 Cat02:
      +   - print comment entry (end loop if not comment)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(samStackST.extraStr[0] != '@')
         break; /*off header*/

      fprintf(
         samConFILE,
         "%s%s",
         samStackST.extraStr,
         str_endLine
      );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec06 Sub01 Cat03:
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

            goto err_fun09_sec11_sub02;
         } /*If: sam file has multiple references*/

         multiRefBl = 1;

         /*get past "@SQ\t"*/
         tmpStr = samStackST.extraStr + 4; 

         while(*tmpStr++ != ':')
            if(*tmpStr < 31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_fun09_sec06_sub01_cat04;

         tmpStr +=
            cpDelim_ulCp(
               refIdStr,
               tmpStr,
               def_tab_ulCp,
               '\t'
            ); /*copy reference id*/

         ++tmpStr;
        
         if(*tmpStr < 31 )
            goto nextHeader_fun09_sec06_sub01_cat04;

         /*move past LN: flag*/
         while(*tmpStr++ != ':')
            if(*tmpStr <31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_fun09_sec06_sub01_cat04;

         /*get reference length*/
         tmpStr += strToSI_base10str(tmpStr, &lenRefSI);

         if(*tmpStr > 31)
            lenRefSI = def_refLen_tbConDefs;
            /*could not get reference length*/
      } /*If: sequence entry*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun09 Sec06 Sub01 Cat04:
      +   - move to next entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      nextHeader_fun09_sec06_sub01_cat04:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: read in header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec06 Sub01 Cat05:
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

      goto err_fun09_sec11_sub02;
   } /*If: error reading sam file header*/

   /*****************************************************\
   * Fun09 Sec06 Sub02:
   *   - print tbCon header for sam file
   *   o fun09 sec06 sub02 cat01:
   *     - mapRead sam file header settings
   *   o fun09 sec06 sub02 cat02:
   *     - tbCon cosensus settings
   *   o fun09 sec06 sub02 cat03:
   *     - tbCon variant print (tsv file) settings
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec06 Sub02 Cat01:
   +   - mapRead sam file header settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   pFTBHeader_fun09_sec06_sub02_cat01:;

   if(fqIndexSI)
   { /*If: need a full header*/
      fprintf(samFILE, "@HD\tVN:1.6\tSO:unsorted");
      fprintf(samFILE, "\tGO:query%s", str_endLine);

      fprintf(samConFILE, "@HD\tVN:1.6\tSO:unsorted");
      fprintf(samConFILE, "\tGO:query%s", str_endLine);

      tmpStr = mapRefStackST.seqSTPtr->idStr;
      errSL = 0;

      while(*tmpStr > 32)
         refIdStr[errSL++] = *tmpStr++;

      errSC = *tmpStr;
      *tmpStr = 0;
      refIdStr[errSL] = 0;
      errSL = 0;

      fprintf(
         samFILE,
         "@SQ\tSN:%s\tLN:%li%s",
         mapRefStackST.seqSTPtr->idStr,
         mapRefStackST.seqSTPtr->seqLenSL + 1,
         str_endLine
      );
      fprintf(
         samConFILE,
         "@SQ\tSN:%s\tLN%li%s",
         mapRefStackST.seqSTPtr->idStr,
         mapRefStackST.seqSTPtr->seqLenSL + 1,
         str_endLine
      );

      *tmpStr = errSC;
      tmpStr = 0;
      errSC = 0;

      fprintf(
         samFILE,
         "@PG\tID:freezeTB\tPN:freezeTB"
      );

      fprintf(
         samFILE,
         "\tVN:%i-%02i-%02i\tCL:freezeTB -prefix %s",
         def_year_ftbVersion,
         def_month_ftbVersion,
         def_day_ftbVersion,
         ftbSetStackST.prefixStr
      );

      for(errSL = 1; errSL < numArgsSI; ++errSL)
         fprintf(samFILE, " %s", argAryStr[errSL]);
      errSL = 0;
      fprintf(samFILE, "%s", str_endLine);
   } /*If: need a full header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec06 Sub02 Cat02:
   +   - tbCon cosensus settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! ftbSetStackST.clustBl || fqIndexSI)
   { /*If: not doing mixed infection step*/
      fprintf(
         samConFILE,
         "@PG\tID:freezeTB\tPN:freezeTB"
      );

      fprintf(
         samConFILE,
         "\tVN:%i-%02i-%02i\tCL:freezeTB",
         def_year_ftbVersion,
         def_month_ftbVersion,
         def_day_ftbVersion
      );

      /*Check if getting sam file from stdin*/
      if(! samFileStr || *samFileStr == '-')
        fprintf(samConFILE, " -sam -");
      else
        fprintf(samConFILE, " -sam %s", samFileStr);

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
      + Fun09 Sec06 Sub02 Cat03:
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
          " -out-tsv %s -out %s%s",
          conTsvStr,
          samConStr,
          str_endLine
      );
   } /*If: not doing mixed infection step*/

   fclose(samConFILE);
   samConFILE = 0;

   /*****************************************************\
   * Fun09 Sec06 Sub03:
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
   ^ Fun09 Sec07:
   ^   - read analysis
   ^   o fun09 sec06 sub01:
   ^     - allocate memory for the read stats arrays
   ^   o fun09 sec06 sub02:
   ^     - map reads (if needed) + start loop
   ^   o fun09 sec06 sub03:
   ^     - filter reads (sam entries)
   ^   o fun09 sec06 sub04:
   ^     - mask primers in reads
   ^   o fun09 sec06 sub05:
   ^     - build filtered histogram
   ^   o fun09 sec06 sub06:
   ^     - indel clean up
   ^   o fun09 sec06 sub07:
   ^     - build consensus
   ^   o fun09 sec06 sub08:
   ^     - check for AMRs
   ^   o fun09 sec06 sub09:
   ^     - check for MIRU lineages
   ^   o fun09 sec06 sub10:
   ^     - check for spoligotypes
   ^   o fun09 sec06 sub11:
   ^     - move to next read
   ^   o fun09 sec06 sub12:
   ^     - minor clean up (variables unique to sec07)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec07 Sub01:
   *   - allocate memory for the read stats arrays
   \*****************************************************/

   if(fqIndexSI)
      lastBaseUI = mapRefStackST.seqSTPtr->seqLenSL;
      /*mapping to input reference*/

   readMapArySI =
      calloc((lastBaseUI + 8), sizeof(signed int));

   if(! readMapArySI)
   { /*If: memory error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "memory error (read histogram malloc)"
      );

      goto err_fun09_sec11_sub02;
   } /*If: memory error*/

   /*****************************************************\
   * Fun09 Sec07 Sub02:
   *   - map reads (if needed) + start loop
   *   o fun09 sec07 sub02 cat01:
   *     - open fastq(s) file for mapping + start loop
   *   o fun09 sec07 sub02 cat02:
   *     - map reads
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec07 Sub02 Cat01:
   +   - open fastq(s) file for mapping + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   idFILE = fopen((char *) idFileStr, "w");
      /*printing ids for reads with amrs*/

   while(! errSC)
   { /*Loop: read anaylsis*/

      if(fqIndexSI)
      { /*If: need to map reads*/
         if(mapErrSC == def_EOF_seqST)
         { /*If: finished last file*/
            mapErrSC = 0;
  
            if(fqIndexSI >= numArgsSI)
               break; /*finished*/

            if(argAryStr[fqIndexSI][0] == '-')
            { /*If: standard input*/
               if(stdinBl)
               { /*If: already read from stdin*/
                  mapErrSC = def_EOF_seqST;
                  continue;
               } /*If: already read from stdin*/

               else
                  fqFILE = stdin;
            } /*If: standard input*/

            else
            { /*Else: fastq file input*/
               fqFILE = fopen(argAryStr[fqIndexSI], "rb");

               if(! fqFILE)
               { /*If: could not open fastq file*/
                  tmpStr = errHeapStr;

                  tmpStr +=
                     cpStr_ulCp(
                        errHeapStr,
                        (signed char *) "could not open "
                     );
                  tmpStr +=
                     cpStr_ulCp(
                        tmpStr,
                        (signed char *)
                            argAryStr[fqIndexSI]
                     );

                  goto err_fun09_sec11_sub02;
               } /*If: could not open fastq file*/
            } /*Else: fastq file input*/

            ++fqIndexSI;
         } /*If: finished last file*/

        /*+++++++++++++++++++++++++++++++++++++++++++++++\
        + Fun09 Sec07 Sub02 Cat02:
        +   - map reads
        \+++++++++++++++++++++++++++++++++++++++++++++++*/

         mapErrSC =
            get_gzSeqST(
               &gzFileStackST,
               &fileTypeSC,
               &readSeqStackST,
               fqFILE
            );

         if(fqFILE)
            fqFILE = 0;

         if(mapErrSC == def_EOF_seqST)
            continue;

         else if(mapErrSC)
         { /*Else If: error (likely file)*/
               tmpStr = errHeapStr;

               tmpStr +=
                  cpStr_ulCp(
                     errHeapStr,
                     (signed char *) "error reading "
                  );
               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     (signed char *) argAryStr[fqIndexSI]
                  );

               goto err_fun09_sec11_sub02;
         } /*Else If: error (likely file)*/

         forScoreSL =
            align_mapRead(
               &readSeqStackST,
               &mapRefStackST,
               &ignoreSI,
               &samStackST,
               &mapAlnStackST,
               &ftbSetStackST.mapSetST,
               &forErrSC
            ); /*get forward alignment*/

         if(forErrSC == def_memErr_mapRead)
         { /*If: mapRead had memory error*/
            mapMemErr_fun09_sec07_sub02_cat02:;
               tmpStr = errHeapStr;

               tmpStr +=
                  cpStr_ulCp(
                     errHeapStr,
                     (signed char *)
                        "memory error mapping reads in"
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     (signed char *) argAryStr[fqIndexSI]
                  );

               goto err_fun09_sec11_sub02;
         } /*If: mapRead had memory error*/

         revCmpIndex_alnSet(
            readSeqStackST.seqStr,
            readSeqStackST.qStr,
            readSeqStackST.seqLenSL
         );

         revScoreSL =
            align_mapRead(
               &readSeqStackST,
               &mapRefStackST,
               &ignoreSI,
               &revSamStackST,
               &mapAlnStackST,
               &ftbSetStackST.mapSetST,
               &revErrSC
            ); /*get forward alignment*/

         if(revErrSC == def_memErr_mapRead)
            goto mapMemErr_fun09_sec07_sub02_cat02;
         revSamStackST.flagUS = 16;

         if(revScoreSL > forScoreSL)
         { /*If: reverse alignment is better*/
            swap_samEntry(&samStackST, &revSamStackST);

            if(revErrSC)
               continue; /*read did not map*/
         } /*If: reverse alignment is better*/

         else if(forErrSC)
            continue; /*read did not map*/

         /*left align homopolymer indels*/
         seqToIndex_alnSet(samStackST.seqStr);
         leftAlnIndel_rmHomo(
            &samStackST,
            mapRefStackST.seqSTPtr->seqStr
         );
         indexToSeq_alnSet(samStackST.seqStr);

         p_samEntry(&samStackST, 0, samFILE);
      } /*If: need to map reads*/ 

      /**************************************************\
      * Fun09 Sec07 Sub03:
      *   - filter reads (sam entries)
      \**************************************************/

      /*these flags are only set by minimap2*/
      else if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If:umapped 4, secondary 256, or suplemtal 2048*/
         /*If this was an unmapped read*/
         if(samStackST.flagUS & 4)
            ++noMapReadSI;

         goto nextRead_fun09_sec07_sub09;
      } /*If:umapped 4, secondary 256, or suplemtal 2048*/

      /*remove soft masked bases*/
      seq_trimSam(&samStackST);
         /*already removed reads that could cause errors*/

      if(samStackST.medianQF < ftbSetStackST.minMedianQF)
         goto nextRead_fun09_sec07_sub09;
         /*low mean q-score*/

      if(samStackST.medianQF < ftbSetStackST.minMedianQF)
         goto nextRead_fun09_sec07_sub09;
         /*low median q-score*/

      if(
           samStackST.mapqUC
         < ftbSetStackST.tbConSet.minMapqUC
      ) goto nextRead_fun09_sec07_sub09;
         /*low mapping quality*/

      if(
           samStackST.alnReadLenUI
         < (unsigned int) ftbSetStackST.tbConSet.minLenSI
      ) goto nextRead_fun09_sec07_sub09; /*short read*/

      /**************************************************\
      * Fun09 Sec07 Sub04:
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
      * Fun09 Sec07 Sub05:
      *   - build filtered histogram
      \**************************************************/

      ++totalReadsUI;

      addRead_ampDepth(
         &samStackST,
         coordsHeapST,
         numCoordsSI,
         readMapArySI,
         &offTargReadsSI
      );

      /**************************************************\
      * Fun09 Sec07 Sub06:
      *   - indel clean up
      \**************************************************/

      if(ftbSetStackST.indelCleanBl)
      { /*If: cleaning up indels in reads*/
         tmpSeqUI = samStackST.readLenUI;
         tmpSeqUI += samStackST.delCntUI;
         tmpSeqUI += 8;

         if( lenBuffUL < (tmpSeqUI << 1) )
         { /*If: need larger buffer*/
            lenBuffUL = tmpSeqUI << 1;
            free(buffHeapStr);
            buffHeapStr = 0;
            buffHeapStr =
               malloc(
                  (lenBuffUL + 8) * sizeof(signed char)
               );

            if(! buffHeapStr)
            { /*If: had memory error*/
               tmpStr = errHeapStr;
               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     (signed char *)
                       "memory error setting up buffer"
                  );

               *tmpStr++ = str_endLine[0];
               if(str_endLine[1])
                  *tmpStr++ = str_endLine[1];
               *tmpStr = 0;

               goto err_fun09_sec11_sub02;
            } /*If: had memory error*/

         } /*If: need larger buffer*/
         /*the resized buffer is set up so that rmHomo
         `  will never have to resize it. This should
         `  avoid all memory errors
         */
 
         tmpQUI = tmpSeqUI;
         tmpStr = &buffHeapStr[tmpSeqUI];

         /*handling reference sequence this way because
         `  the mapRead processing step adds a good amount
         `  of memory and time
         */
         if(! fqIndexSI)
            rmHomoRefStr = refStackST.seqStr;
         else
            rmHomoRefStr =
               mapRefStackST.seqSTPtr->seqStr;

         indel_rmHomo(
            &samStackST,
            rmHomoRefStr,
            ftbSetStackST.minHomoLenSI,
            ftbSetStackST.maxIndelLenSI,
            ftbSetStackST.homoMaskSC,
            1, /*scan for neighbor homopolymers*/
            &buffHeapStr,
            &tmpSeqUI,
            &tmpStr,
            &tmpQUI
         );

         indexToSeq_alnSet(samStackST.seqStr);
            /*the reference is in index form, so need to
            `  convert any added indels from index to
            `  nucleotide
            */
      } /*If: cleaning up indels in reads*/

      /**************************************************\
      * Fun09 Sec07 Sub07:
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

            goto err_fun09_sec11_sub02;
         } /*If: memory error*/
      } /*If: not doing mixed infection detection*/

      /**************************************************\
      * Fun09 Sec07 Sub08:
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

         goto err_fun09_sec11_sub02;
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
      * Fun09 Sec07 Sub09:
      *   - check for MIRU lineages 
      \**************************************************/

      incLineage_miruTbl(
         &samStackST,
         ftbSetStackST.fudgeSI,
         miruHeapST
      );

      /**************************************************\
      * Fun09 Sec07 Sub10:
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

         goto err_fun09_sec11_sub02;
      } /*If: memory error*/
      
      /**************************************************\
      * Fun09 Sec07 Sub11:
      *   - move to next read
      \**************************************************/

      nextRead_fun09_sec07_sub09:;
         if(! fqIndexSI)
            errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: read anaylsis*/

   /*****************************************************\
   * Fun09 Sec07 Sub12:
   *   - minor clean up (variables unique to sec07)
   \*****************************************************/

   if(! ftbSetStackST.clustBl)
   { /*If: not doing mixed infection detection*/
      freeStack_samEntry(&samStackST);

      if(samFILE != stdin)
         fclose(samFILE);

      samFILE = 0;
   } /*If: not doing mixed infection detection*/

   else if(fqIndexSI)
   { /*Else If: mapped reads for clustering*/
      fclose(samFILE);
      samFILE = 0;
      samFILE = fopen((char *) mapSamStr, "r");
   } /*Else If: mapped reads for clustering*/

   fclose(idFILE);
   idFILE = 0;

   freeStack_file_inflate(&gzFileStackST);

   /*memory for masking primers*/
   free(maskStartHeapAryUI);
   maskStartHeapAryUI = 0;

   free(maskEndHeapAryUI);
   maskEndHeapAryUI = 0;

   free(maskFlagHeapAryUI);
   maskFlagHeapAryUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec08:
   ^   - print read data
   ^   o fun09 sec08 sub01:
   ^     - print filtered read stats
   ^   o fun09 sec08 sub02:
   ^     - print AMR hits for reads
   ^   o fun09 sec08 sub03:
   ^     - print read MIRU table
   ^   o fun09 sec08 sub04:
   ^     - print read spoligotype entry
   ^   o fun09 sec08 sub05:
   ^     - print tsv file of variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec08 Sub01:
   *   - print filtered read stats
   \*****************************************************/

   sort_geneCoord(
      coordsHeapST,
      0,
      (unsigned int) numCoordsSI
   );

   outFILE = fopen((char *) readStatsStr, "w");
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
    ); /*print filterd read stats*/

   fclose(outFILE);
   outFILE = 0;

   free(readMapArySI);
   readMapArySI = 0;

   freeHeap_geneCoord(coordsHeapST);
   coordsHeapST = 0;

   /*****************************************************\
   * Fun09 Sec08 Sub02:
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
   * Fun09 Sec08 Sub03:
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
   * Fun09 Sec08 Sub04:
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
      errSL < def_lenSpolAry_fun09;
      ++errSL
   ) spoligoAryUI[errSL] = 0;

   spoligoAryUI[errSL] = -1;
   errSL = 0;
   
   /*****************************************************\
   * Fun09 Sec08 Sub05:
   *   - print tsv file of variants
   \*****************************************************/

   /*Build the tsv of variants table*/
   if(ftbSetStackST.clustBl)
      goto mixedInfect_fun09_sec11;

   errSC =
      pvar_tbCon(
         conNtHeapAryST,
         lenRefSI,
         refIdStr,
         &ftbSetStackST.tbConSet,
         conTsvStr
      ); /*print variants (not a vcf)*/

    if(errSC == def_noMap_tbConDefs)
    { /*If: nothing mapped*/
       cpStr_ulCp(
          errHeapStr,
          (signed char *) "unable to build consensus"
       );

       goto err_fun09_sec11_sub02;
    } /*If: nothing mapped*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec09:
   ^   - collapse consensus and consensus analysis
   ^   o fun09 sec09 sub01:
   ^     - collapse consensus
   ^   o fun09 sec09 sub02:
   ^     - print consensus and do ananlysis
   ^   o fun09 sec09 sub03:
   ^     - close output files and free uneeded variables
   ^   o fun09 sec09 sub04:
   ^     - print consensus MIRU lineages
   ^   o fun09 sec09 sub05:
   ^     - print detected spoligotype (consensus)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec09 Sub01:
   *   - collapse consensus
   \*****************************************************/

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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec09 Sub02:
   *   - print consensus and do ananlysis
   *   o fun09 sec09 sub04 cat01:
   *     - open files + run consensus fragment loop
   *   o fun09 sec09 sub04 cat02:
   *     - print consensus fragments
   *   o fun09 sec09 sub04 cat03:
   *     - AMR detection and printing
   *   o fun09 sec09 sub04 cat04:
   *     - MIRU-VNTR lineage detection and printing
   *   o fun09 sec09 sub04 cat05:
   *     - detect spoligotypes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec09 Sub04 Cat01:
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
     + Fun09 Sec09 Sub04 Cat02:
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

      /*print fragment*/
      p_samEntry(&samConSTAry[siCon], 0, samConFILE);

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun09 Sec09 Sub04 Cat03:
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
     + Fun09 Sec09 Sub04 Cat04:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         &samConSTAry[siCon],
         ftbSetStackST.fudgeSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun09 Sec09 Sub04 Cat05:
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
   * Fun09 Sec09 Sub03:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec09 Sub04:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec09 Sub05:
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

   goto ret_fun09_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec10:
   ^   - run mixed infection detection (if requested)
   ^   o fun09 sec10 sub01:
   ^     - check if can open log files
   ^   o fun09 sec10 sub02:
   ^     - run mixed infection detection
   ^   o fun09 sec10 sub03:
   ^     - print clusters for mixed infection
   ^   o fun09 sec10 sub04:
   ^     - amr/miru/spoligotype detection on clusters
   ^   o fun09 sec10 sub05:
   ^     - print consensus MIRU lineages
   ^   o fun09 sec10 sub06:
   ^     - print detected spoligotype
   \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec10 Sub01:
   *   - check if can open log files
   \*****************************************************/

   mixedInfect_fun09_sec11:;

   if(samFILE == stdin)
   { /*If: piping file to mixed infection detect*/
      tmpStr = errHeapStr;

      tmpStr += 
         cpStr_ulCp(
          errHeapStr,
          (signed char *)
             "do not use -sam stdin for clustering"
         );

      goto err_fun09_sec11_sub02;
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
      logFILE = fopen((char *) logFileStr, "w");

   if(! logFILE)
   { /*If: could not open log flie*/
      fprintf(
         stderr,
         "could not open log file %s%s",
         logFileStr,
         str_endLine
      );

      goto err_fun09_sec11_sub02;
   } /*If: could not open log flie*/

   /*****************************************************\
   * Fun09 Sec10 Sub02:
   *   - run mixed infection detection
   \*****************************************************/

   fseek(samFILE, 0, SEEK_SET);

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
      tmpStr = errHeapStr;

      if(errSC == def_memErr_edClust)
         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "clustering memory error"
            );

      else if(errSC == def_fileErr_edClust)
         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "clustering file error"
            );

      else
         tmpStr +=
            cpStr_ulCp(
               errHeapStr,
               (signed char *) "no reads in sam file"
            );

      *tmpStr++ = str_endLine[0];
      if(str_endLine[1])
         *tmpStr++ = str_endLine[1];
      *tmpStr = '\0';

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   if(logFILE != stderr)
      fclose(logFILE);
   logFILE = 0;

   /*****************************************************\
   * Fun09 Sec10 Sub03:
   *   - print clusters for mixed infection
   \*****************************************************/

   samConFILE =
      fopen(
         (char *) samConStr,
          "a"
      ); /*open consensus output file*/

   errSC =
      plist_con_clustST(conListHeapST, 0, 0, samConFILE);
      /*print consensus for clusters*/

   fclose(samConFILE);
   samConFILE = 0;

   if(errSC)
   { /*If: had error*/
      cpStr_ulCp(
         errHeapStr,
         (signed char *)
            "cluster memory error consensus print"
      );

      goto err_fun09_sec11_sub02;
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

      goto err_fun09_sec11_sub02;
   } /*If: had error*/

   fclose(samFILE);
   samFILE = 0;

   if(indexHeapST)
      freeHeap_index_clustST(indexHeapST);
   indexHeapST = 0;

   /*****************************************************\
   * Fun09 Sec10 Sub04:
   *   - amr/miru/spoligotype detection on clusters
   *   o fun09 sec10 sub04 cat01:
   *     - amr detection + start loop
   *   o fun09 sec10 sub04 cat02:
   *     - MIRU-VNTR lineage detection and printing
   *   o fun09 sec10 sub04 cat03:
   *     - spoligotype detection and printing
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun09 Sec10 Sub04 Cat01:
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
     + Fun09 Sec10 Sub04 Cat02:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         conNodeST->samSTPtr,
         ftbSetStackST.fudgeSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Fun09 Sec10 Sub04 Cat03:
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
     + Fun09 Sec10 Sub04 Cat04:
     +   - move to next cluster
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     conNodeST = conNodeST->nextST;
   } /*Loop: find amrs and lineages for consensuses*/

   if(conListHeapST)
      freeHeapList_con_clustST(conListHeapST);
   conListHeapST = 0;
   conNodeST = 0;
         
   /*****************************************************\
   * Fun09 Se109 Sub05:
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

      goto err_fun09_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Fun09 Sec10 Sub06:
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

   goto ret_fun09_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec11:
   ^   - clean up
   ^   o fun09 sec11 sub01:
   ^     - no error clean up
   ^   o fun09 sec11 sub02:
   ^     - error clean up
   ^   o fun09 sec11 sub03:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec11 Sub01:
   *   - no error clean up
   \*****************************************************/

   ret_fun09_sec11:;
      errSC = 0;
      free(errHeapStr);
      errHeapStr = 0;
      goto cleanUp_fun09_sec11_sub03;

   /*****************************************************\
   * Fun09 Sec11 Sub02:
   *   - error clean up
   \*****************************************************/

   err_fun09_sec11_sub02:;
      errSC = 1;
      goto cleanUp_fun09_sec11_sub03;

   /*****************************************************\
   * Fun09 Sec11 Sub03:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun09_sec11_sub03:;
      freeStack_seqST(&refStackST);
      freeStack_set_freezeTB(&ftbSetStackST);
      freeStack_samEntry(&samStackST);

      freeStack_file_inflate(&gzFileStackST);
      freeStack_ref_mapRead(&mapRefStackST);
      freeStack_aln_mapRead(&mapAlnStackST);
      freeStack_samEntry(&revSamStackST);
      freeStack_seqST(&readSeqStackST);

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

      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else if(samFILE == stderr) ;
      else
         fclose(samFILE);
      samFILE = 0;

      if(! fqFILE) ;
      else if(fqFILE == stdin) ;
      else if(fqFILE == stdout) ;
      else if(fqFILE == stderr) ;
      else
         fclose(fqFILE);
      fqFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);
      outFILE = 0;

      if(! samConFILE) ;
      else if(samConFILE == stdin) ;
      else if(samConFILE == stdout) ;
      else if(samConFILE == stderr) ;
      else
         fclose(samConFILE);
      samConFILE = 0;

      if(! idFILE) ;
      else if(idFILE == stdin) ;
      else if(idFILE == stdout) ;
      else if(idFILE == stderr) ;
      else
         fclose(idFILE);
      idFILE = 0;

      if(! spoligoOutFILE) ;
      else if(spoligoOutFILE == stdin) ;
      else if(spoligoOutFILE == stdout) ;
      else if(spoligoOutFILE == stderr) ;
      else
         fclose(spoligoOutFILE);
      spoligoOutFILE = 0;

      if(! logFILE) ;
      else if(logFILE == stdin) ;
      else if(logFILE == stdout) ;
      else if(logFILE == stderr) ;
      else
         fclose(logFILE);
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
