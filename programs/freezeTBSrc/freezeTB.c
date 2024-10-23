/*########################################################
# Name: freezeTB
#   - Process input reads for AMR(s) and MIRU lineages.
#     This also builds a cosenssu and prints out depths
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   - header:
'     o Included libraries
'   o fun01: pversion_freezeTB
'     - prints version number for freezeTB and |submodules
'   o fun02: phelp_freezeTB
'     - prints help message for freezeTB
'   o fun03: input_freezeTB
'     - gets user input
'   o main:
'     - main function which drives everything
'   o note01:
'     - windows enviromental variables
'   o license:
'     - licensing for this code (public domain / mit)
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

#include "../genLib/dataTypeShortHand.h"
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
!   o .c  #include "../genBio/codonTbl.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/edDist.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../tbAmrSrc/drugAry.h"
!
!   o .h  #include "../genBio/ntTo2bit.h" 
!   o .h  #include "../genBio/revNtTo2bit.h" 
!   o .h  #include "../genBio/ntTo5bit.h" 
!   o .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^    - defined variables/default settings (freezeTB only)
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_lenFileName_freezeTB 1024

signed char *def_prefix_freezeTB = (schar *) "Hufflepuff";
   /*this is here to tweak Tara's nose a bit.
   `   She is an Harry Potter Ravenclaw fan. I figure
   `   Ravenclaw is the worst thing other then Slytherin,
   `   which is evil.
   */


signed char *def_depthFlag_freezeTB = (schar *) "tb";

/*-------------------------------------------------------\
| Fun01: pversion_freezeTB
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
| Fun02: phelp_freezeTB
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
   ' Fun02 TOC:
   '   - print help message
   '   o fun02 sec01:
   '     - variable declerations
   '   o fun02 sec02:
   '     - find default file paths
   '   o fun02 sec03:
   '     - usage
   '   o fun02 sec04:
   '     - input
   '   o fun02 sec05:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*for default file locations*/
   schar amrDbFileStr[def_lenFileName_freezeTB];
   schar miruTblFileStr[def_lenFileName_freezeTB];
   schar coordFileStr[def_lenFileName_freezeTB];

   schar spoligoRefFileStr[def_lenFileName_freezeTB];
   schar spoligoDbFileStr[def_lenFileName_freezeTB];
   schar maskPrimFileStr[def_lenFileName_freezeTB];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - find default file paths
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrPath_freezeTBPaths(amrDbFileStr);
   miruPath_freezeTBPaths(miruTblFileStr);
   coordPath_freezeTBPaths(coordFileStr);
   spolSpacerPath_freezeTBPaths(spoligoRefFileStr);
   spolLineagePath_freezeTBPaths(spoligoDbFileStr);
   maskPath_freezeTBPaths(maskPrimFileStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
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
   ^ Fun02 Sec04:
   ^   - input entry
   ^   o fun02 sec04 sub01:
   ^     - input header/File IO input
   ^   o fun02 sec04 sub02:
   ^     - fitering settings
   ^   o fun02 sec04 sub03:
   ^     - consensus settings
   ^   o fun02 sec04 sub04:
   ^     - clustering settings
   ^   o fun02 sec04 sub05:
   ^     - variant/read AMR printing settings
   ^   o fun02 sec04 sub06:
   ^     - depth settings
   ^   o fun02 sec04 sub07:
   ^     - lineage (non-database) settings
   ^   o fun02 sec04 sub08:
   ^     - other settings and help message/version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - input header/File IO input
   *   o fun02 sec04 sub01 cat01:
   *     - input header
   *   o fun02 sec04 sub01 cat02:
   *     - sam file input
   *   o fun02 sec04 sub01 cat03:
   *     - prefix to name output files
   *   o fun02 sec04 sub01 cat04:
   *     - AMR table input
   *   o fun02 sec04 sub01 cat05:
   *     - gene coordinates
   *   o fun02 sec04 sub01 cat06:
   *     - MIRU table input
   *   o fun02 sec04 sub01 cat07:
   *     - spoligotype spacer sequence input
   *   o fun02 sec04 sub01 cat08:
   *     - spoligotype linage database
   *   o fun02 sec04 sub01 cat09:
   *     - primer masking database
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub01 Cat01:
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
   + Fun02 Sec04 Sub01 Cat02:
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
   + Fun02 Sec04 Sub01 Cat03:
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
   + Fun02 Sec04 Sub01 Cat04:
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
   + Fun02 Sec04 Sub01 Cat05:
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
   + Fun02 Sec04 Sub01 Cat06:
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
   + Fun02 Sec04 Sub01 Cat07:
   +   - spoligotype spacer sequence input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spoligoRefFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Required]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -spoligo spoligo-spacers.fa: [Using %s]\n",
         spoligoRefFileStr
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
   + Fun02 Sec04 Sub01 Cat08:
   +   - spoligotype linage database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(spoligoDbFileStr[0] == '\0')
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Required]\n"
      );

   else
      fprintf(
         (FILE *) outFILE,
         "    -db-spoligo lineages.csv: [Using %s]\n",
         spoligoDbFileStr
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
   + Fun02 Sec04 Sub01 Cat09:
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

   /*****************************************************\
   * Fun02 Sec04 Sub02:
   *   - fitering settings
   *   o fun02 sec04 sub02 cat01:
   *     - filter header
   *   o fun02 sec04 sub02 cat02:
   *     - mapping quality filter
   *   o fun02 sec04 sub02 cat03:
   *     - min length filter
   *   o fun02 sec04 sub02 cat04:
   *     - mean Q-score filter
   *   o fun02 sec04 sub02 cat05:
   *     - median Q-score filter
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub02 Cat01:
   +   - fitering header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pFiltBl)
      goto skipReadFilt_fun02_sec04_sub03;

   fprintf(
      (FILE *) outFILE,
      "  Read Filtering:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub02 Cat02:
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
   + Fun02 Sec04 Sub02 Cat03:
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
   + Fun02 Sec04 Sub02 Cat04:
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
   + Fun02 Sec04 Sub02 Cat05:
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
   + Fun02 Sec04 Sub02 Cat06:
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
   * Fun02 Sec04 Sub03:
   *   - consensus settings
   *   o fun02 sec04 sub03 cat01:
   *     - consensus setting header
   *   o fun02 sec04 sub03 cat02:
   *     - min depth filter
   *   o fun02 sec04 sub03 cat03:
   *     - min length to keep a fragment
   *   o fun02 sec04 sub03 cat04:
   *     - min snp/match Q-score
   *   o fun02 sec04 sub03 cat05:
   *     - min insertion Q-score
   *   o fun02 sec04 sub03 cat06:
   *     - min percdent snp/match support
   *   o fun02 sec04 sub03 cat07:
   *     - min percdent insertion support
   *   o fun02 sec04 sub03 cat08:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub03 Cat01:
   +   - consensus setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipReadFilt_fun02_sec04_sub03:;

   if(! pConBl)
       goto skipConSet_fun02_sec04_sub04_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Consensus settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub03 Cat02:
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
   + Fun02 Sec04 Sub03 Cat03:
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
   + Fun02 Sec04 Sub03 Cat04:
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
   + Fun02 Sec04 Sub03 Cat05:
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
   + Fun02 Sec04 Sub03 Cat06:
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
   + Fun02 Sec04 Sub03 Cat07:
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
   + Fun02 Sec04 Sub03 Cat08:
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
   * Fun02 Sec04 Sub04:
   *   - clustering settings
   *   o fun02 sec04 sub04 cat01:
   *     - print clustering setting header
   *   o fun02 sec04 sub04 cat02:
   *     - read scoring settings
   *   o fun02 sec04 sub04 cat03:
   *     - minimum cluster depth
   *   o fun02 sec04 sub04 cat04:
   *     - minimum cluster percent depth
   *   o fun02 sec04 sub04 cat05:
   *     - minimum percent difference between reads
   *   o fun02 sec04 sub04 cat06:
   *     - minimum consensus to read percent difference
   *   o fun02 sec04 sub04 cat07:
   *     - maximum percent similarity between consensuses
   *   o fun02 sec04 sub04 cat08:
   *     - minimum percent overlap between consensuses
   *   o fun02 sec04 sub04 cat09:
   *     - maximum percent maksing in consensus
   *   o fun02 sec04 sub04 cat10:
   *     - depth profiling (edit distance setting start)
   *   o fun02 sec04 sub04 cat11:
   *     - minimum depth profile depth (edDist)
   *   o fun02 sec04 sub04 cat12:
   *     - minimum error:variant ratio to be different
   *   o fun02 sec04 sub04 cat13:
   *     - length of window (edDist)
   *   o fun02 sec04 sub04 cat14:
   *     - window variant:error ratio (for difference)
   *   o fun02 sec04 sub04 cat15:
   *     - minimum indel length to keep indel in edDist
   *   o fun02 sec04 sub04 cat16:
   *     - minimum q-score to count snp as different
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub04 Cat01:
   +   - print clustering setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipConSet_fun02_sec04_sub04_cat01:;

   if(! pClustBl)
      goto skipClustSet_fun02_sec04_sub05_cat01;

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
   + Fun02 Sec04 Sub04 Cat02:
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
   + Fun02 Sec04 Sub04 Cat03:
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
   + Fun02 Sec04 Sub04 Cat04:
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
   + Fun02 Sec04 Sub04 Cat05:
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
   + Fun02 Sec04 Sub04 Cat06:
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
   + Fun02 Sec04 Sub04 Cat07:
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
   + Fun02 Sec04 Sub04 Cat08:
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
   + Fun02 Sec04 Sub04 Cat09:
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
   + Fun02 Sec04 Sub04 Cat10:
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
   + Fun02 Sec04 Sub04 Cat11:
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
   + Fun02 Sec04 Sub04 Cat12:
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
   + Fun02 Sec04 Sub04 Cat13:
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
   + Fun02 Sec04 Sub04 Cat14:
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
   + Fun02 Sec04 Sub04 Cat15:
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
   + Fun02 Sec04 Sub04 Cat16:
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
   * Fun02 Sec04 Sub05:
   *   - variant/read AMR printing settings
   *   o fun02 sec04 sub05 cat01:
   *     - variant/readAMR print setting header
   *   o fun02 sec04 sub05 cat02:
   *     - min depth filter
   *   o fun02 sec04 sub05 cat03:
   +     - min percent of reads to keep read AMR
   *   o fun02 sec04 sub05 cat04:
   *     - min perccent snp/match support
   *   o fun02 sec04 sub05 cat05:
   *     - min percent insertion support
   *   o fun02 sec04 sub05 cat06:
   *     - min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub05 Cat01:
   +   - variant/readAMR print setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipClustSet_fun02_sec04_sub05_cat01:;

   if(! pPrintBl)
      goto skipPrintSet_fun02_sec04_sub06_cat01;

   fprintf(
      (FILE *) outFILE,
      "  Print settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub05 Cat02:
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
   + Fun02 Sec04 Sub05 Cat03:
   +   - min percent of reads to keep read AMR
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-amr-map-perc %.2f: [Optional; %.2f]\n",
      def_minPercMapped_tbAmrDefs,
      def_minPercMapped_tbAmrDefs
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
   + Fun02 Sec04 Sub05 Cat04:
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
   + Fun02 Sec04 Sub05 Cat05:
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
   + Fun02 Sec04 Sub05 Cat06:
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
   * Fun02 Sec04 Sub06:
   *   - depth settings
   \*****************************************************/

   skipPrintSet_fun02_sec04_sub06_cat01:;

   /*****************************************************\
   * Fun02 Sec04 Sub07:
   *   - lineage settings
   *   o fun02 sec04 sub07 cat01:
   *     - lineage setting block
   *   o fun02 sec04 sub07 cat02:
   *     - MIRU fudge setting
   *   o fun02 sec04 sub07 cat03:
   *     - spoligtyping min perc score
   *   o fun02 sec04 sub07 cat04:
   *     - spoligtyping dr-start
   *   o fun02 sec04 sub07 cat05:
   *     - spoligtyping dr-end
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub07 Cat01:
   +   - lineage setting block
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pLinBl)
      goto skipLineageSet_fun02_sec04_sub08;

   fprintf(
      (FILE *) outFILE,
      "  Lineage settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub07 Cat02:
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
   + Fun02 Sec04 Sub07 Cat03:
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
   + Fun02 Sec04 Sub07 Cat04:
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
   + Fun02 Sec04 Sub07 Cat05:
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
   * Fun02 Sec04 Sub08:
   *   - other settings and help message/version number
   *   o fun02 sec04 sub08 cat01:
   *     - other settings header
   *   o fun02 sec04 sub08 cat02:
   *     - flag for graph
   *   o fun02 sec04 sub08 cat03:
   *     - checking frameshifts
   *   o fun02 sec04 sub08 cat04:
   *     - default help message
   *   o fun02 sec04 sub08 cat05:
   *     - addons for help message
   *   o fun02 sec04 sub08 cat06:
   *     - version number
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub08 Cat01:
   +   - Other settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipLineageSet_fun02_sec04_sub08:;

   fprintf(
      (FILE *) outFILE,
      "  Other settings:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec04 Sub08 Cat02:
   +   - flag for graph
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pOtherBl)
      goto skipOtherSet_fun02_sec04_sub08_cat04;
   
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
   + Fun02 Sec04 Sub08 Cat03:
   +   - Checking frameshifts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_checkFramshift_tbAmrDefs)
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
      "      o consensus only; checks frame shift AMRs\n"
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
   + Fun02 Sec04 Sub08 Cat04:
   +   - default help message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   skipOtherSet_fun02_sec04_sub08_cat04:;

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
   + Fun02 Sec04 Sub08 Cat05:
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
   + Fun02 Sec04 Sub08 Cat06:
   +   - version number
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "    -v: print version number of freezeTB and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - output
   ^   o fun02 sec05 sub01:
   ^     - output header
   ^   o fun02 sec05 sub02:
   ^     - consensus amrs file
   ^   o fun02 sec05 sub03:
   ^     - consensus mirufile
   ^   o fun02 sec05 sub04:
   ^     - consensus file
   ^   o fun02 sec05 sub05:
   ^     - variants file
   ^   o fun02 sec05 sub06:
   ^     - reads amr table
   ^   o fun02 sec05 sub07:
   ^     - reads ids of AMR reads
   ^   o fun02 sec05 sub08:
   ^     - reads MIRU table
   ^   o fun02 sec05 sub09:
   ^     - filtered histogram file
   ^   o fun02 sec05 sub10:
   ^     - unfiltered histogram file
   ^   o fun02 sec05 sub11:
   ^     - clusters with reads
   ^   o fun02 sec05 sub12:
   ^     - log (for mixed infect)
   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec05 Sub01:
   *   - output header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   /*****************************************************\
   * Fun02 Sec05 Sub02:
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
   * Fun02 Sec05 Sub03:
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
   * Fun02 Sec05 Sub04:
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
   * Fun02 Sec05 Sub05:
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
   * Fun02 Sec05 Sub06:
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
   * Fun02 Sec05 Sub07:
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
   * Fun02 Sec05 Sub08:
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
   * Fun02 Sec05 Sub09:
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
   * Fun02 Sec05 Sub10:
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
   * Fun02 Sec05 Sub11:
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
   * Fun02 Sec05 Sub12:
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
| Fun03: input_freezeTB
|   - gets user input
| Input:
|   User Input:
|     - numArgsSI:
|       o number of arguments user input
|     - argAryStr:
|       o pointer to array of c-strings with users input
|   File input:
|     - samFileStr:
|       o pointer to c-string to point to input sam file
|     - amrDbFileStr:
|       o c-string to hold path to AMR database
|     - coordFileStr:
|       o c-string to hold path to gene coordiantes
|     - miruDbFileStr:
|       o c-string to hold path to MIRU table (database)
|     - spoligoRefFileStr:
|       o c-string to hold path to fasta file with
|        spoligotyping internal spaces sequences
|     - spoligoDbFileStr:
|       o c-string to hold path to csv file with the
|         spoligotyping lineages
|     - maskPrimFileStr:
|       o c-string to hold the path to primer masking tsv
|     - prefixStr:
|       o pointer to c-string to point to output prefix
|   Read filtering:
|     - minMedianQF:
|       o pointer to float to hold min median Q score
|     - minMeanQF:
|       o pointer to float to hold min mean Q score
|     - minPercMapF:
|       o pointer to float to hold min percentage of
|         mapped reads needed to keep an AMR
|     - settings:
|       o pointer to set_tbCon struct with the tbCon
|         settings for consensus buliding/read filtering
|   Graphing:
|     - depthFlagStr:
|       o pointer to c-string to point to experiment id
|     - mkGraphBl:
|       o set to 1: user wants graphs
|       o set to 0: user does not want graphs
|     - graphFileTypeStr:
|       o pointer to c-string to point to graph file
|         extension
|   Clustering:
|     - clustBlPtr:
|       o pointer to signed char
|       o set to 1: if clustering
|       o set to 0: if not clustering
|     - clustSetSTPtr:
|       o holds settings for the clustering step
|   Misc:
|     - frameshiftBl:
|       o 1: users wants frame shift checking on consensus
|     - fudgeLenSI:
|       o pointer to integer to hold fudge length for
|         MIRU lineage detection
|     - spoligoPercScoreF:
|       o pointer to float to hold mininum percent score
|         to count spoligotype spacer as present
|     - drStartSI:
|       o pointer to signed int to hold first base in
|         direct repeat region (spoligotyping)
|     - drEndSI:
|       o pointer to int to hold last base in direct
|         repeat region (spoligotyping)
| Output:
|   - Modifies:
|     o each input variable the user had user input set
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

   /*file input*/
   signed char **samFileStr,
   signed char *amrDbFileStr,
   signed char *coordFileStr,
   signed char *miruDbFileStr,
   signed char *spoligoRefFileStr,
   signed char *spoligoDbFileStr,
   signed char *maskPrimFileStr,
   signed char **prefixStr,

   /*read filtering*/
   float *minMedianQF,
   float *minMeanQF,
   float *minPercMapF,
   struct set_tbCon *settings,
   signed char *adjCoordBlPtr,

   /*graphing*/
   signed char **depthFlagStr,

   /*clustering*/
   signed char *clustBlPtr,
   struct set_clustST *clustSetSTPtr,

   /*misc*/
   signed char *frameshiftBl,
   signed int *fudgeLenSI,
   float *spoligoPercScoreF,
   signed int *drStartSI,
   signed int *drEndSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: input_freezeTB
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if have user input
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return error type
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar *tmpStr = 0;
   schar errSC = 0;
   schar helpBl = 0; /*print help message at end*/

   /*booleans for extra help message entries*/
   schar pFiltHelpBl = 0;
   schar pConHelpBl = 0;
   schar pPrintHelpBl = 0;
   schar pClustHelpBl = 0;
   schar pLinHelpBl = 0;
   schar pOtherHelpBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing was input*/
      phelp_fun03_sec02:;

      phelp_freezeTB(
         pFiltHelpBl,
         pConHelpBl,
         pPrintHelpBl,
         pClustHelpBl,
         pLinHelpBl,
         pOtherHelpBl,
         stdout
      );

      goto phelp_fun03_sec04;
   } /*If: nothing was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - file input + start loop
   ^   o fun03 sec03 sub02:
   ^     - consensus options
   ^   o fun03 sec03 sub03:
   ^     - printing settings (for consensus tsv)
   ^   o fun03 sec03 sub04:
   ^     - tbAmr settings
   ^   o fun03 sec03 sub05:
   ^     - read filtering + adjust coordinates
   ^   o fun03 sec03 sub06:
   ^     - lineages and graph output
   ^   o fun03 sec03 sub07:
   ^     - cluster settings
   ^   o fun03 sec03 sub08:
   ^     - check for help message requests (normal)
   ^   o fun03 sec03 sub09:
   ^     - check for add on help message requests
   ^   o fun03 sec03 sub10:
   ^     - check for version number requests
   ^   o fun03 sec03 sub11:
   ^     - invalid input
   ^   o fun03 sec03 sub12:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file input + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: read in user input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStr = (schar *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
         ! eql_charCp(
            (schar *) "-amr-tbl",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: amr database input*/
         ++siArg;

         cpDelim_ulCp(
            amrDbFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: amr database input*/

      else if(
         ! eql_charCp(
            (schar *) "-gene-coords",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: gene coordiantes table input*/
         ++siArg;

         cpDelim_ulCp(
            coordFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: gene coordiantes table input*/

      else if(
         ! eql_charCp(
            (schar *) "-miru-tbl",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: miru table input*/
         ++siArg;

         cpDelim_ulCp(
            miruDbFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: miru table input*/

      else if(
         ! eql_charCp(
            (schar *) "-spoligo",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: spoligotype spacers input*/
         ++siArg;

         cpDelim_ulCp(
            spoligoRefFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: spoligotype spacers input*/

      else if(
         ! eql_charCp(
            (schar *) "-db-spoligo",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: spoligo lineage database input*/
         ++siArg;

         cpDelim_ulCp(
            spoligoDbFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: spoligo lineage database input*/

      else if(
         ! eql_charCp(
            (schar *) "-mask-prim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: primer masking file input*/
         ++siArg;

         cpDelim_ulCp(
            maskPrimFileStr,
            (schar *) argAryStr[siArg],
            0,
            '\0'
         );
      } /*Else If: primer masking file input*/

      else if(
         ! eql_charCp(
            (schar *) "-prefix",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: prefix input*/
         ++siArg;
         *prefixStr = (schar *) argAryStr[siArg];
      } /*Else If: prefix input*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - consensus options
      *   o fun03 sec03 sub02 cat01:
      *     - consensus q-score settinngs
      *   o fun03 sec03 sub02 cat02:
      *     - consensus min depth/length settings
      *   o fun03 sec03 sub02 cat03:
      *     - consensus min base percent support settings
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat01:
      +    - consensus q-score settinngs
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum q-score input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &settings->minQSI
            );

         if(tmpStr[0] != '\0')
         { /*If: had an error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-min-q %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: had an error*/
      } /*Else If: minimum q-score input*/


      else if(
         ! eql_charCp(
            (schar *) "-min-q-ins",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum insertion q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &settings->minInsQSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-min-q-ins %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: minimum insertion q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat02:
      +    - consensus min depth/length settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &settings->minDepthSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: minimum depth*/


      else if(
         ! eql_charCp(
            (schar *) "-min-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum fragment length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &settings->minLenSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-min-len %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: minimum fragment length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat03:
      +    - consensus min base percent support settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*atof reurns 0 for failure*/
      else if(
         ! eql_charCp(
            (schar *) "-perc-snp-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      )
      { /*If: minimum % snp support*/
         ++siArg;
         settings->minPercSnpF = atof(argAryStr[siArg]);
      } /*If: minimum % snp support*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-ins-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum % ins support*/
         ++siArg;
         settings->minPercInsF = atof(argAryStr[siArg]);
      } /*Else If: minimum % ins support*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-del-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum % del support*/
         ++siArg;
         settings->minPercDelF = atof(argAryStr[siArg]);
      } /*Else If: minimum % del support*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - printing settings (for consensus tsv)
      \**************************************************/

      /*settings for printing out variations*/
      else if(
         ! eql_charCp(
            (schar *) "-p-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: print minimum depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               &settings->minPrintDepthSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-p-min-depth %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: print minimum depth*/

      /*print percentage depths*/
      else if(
         ! eql_charCp(
            (schar *) "-p-perc-snp-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: print minimum % snp support*/
         ++siArg;

         settings->printMinSupSnpF =
            atof(argAryStr[siArg]);
      } /*Else If: print minimum % snp support*/

      else if(
         ! eql_charCp(
            (schar *) "-p-perc-ins-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: print minimum % ins support*/
         ++siArg;

         settings->printMinSupInsF =
            atof(argAryStr[siArg]);
      } /*Else If: print minimum % ins support*/

      else if(
         ! eql_charCp(
            (schar *) "-p-perc-del-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: print minimum % snp support*/
         ++siArg;

         settings->printMinSupDelF =
            atof(argAryStr[siArg]);
      } /*Else If: print minimum % snp support*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - AMR settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-amr-map-perc",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: AMR min percent mapped reads*/
         ++siArg;
         *minPercMapF= atof(argAryStr[siArg]);
      } /*Else If: AMR min percent mapped reads*/

      else if(
         ! eql_charCp(
            (schar *) "-frameshift",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *frameshiftBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-frameshift",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *frameshiftBl = 0;

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - read filterting + adjust coordinates
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-mapq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      )
      { /*Else If: minimum mapping quality*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
               tmpStr,
               &settings->minMapqUC
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: minimum mapping quality*/

      else if(
         ! eql_charCp(
            (schar *) "-min-median-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min median q-score*/
         ++siArg;
         *minMedianQF = atof(argAryStr[siArg]);
      } /*Else If: min median q-score*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mean-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min mean q-score*/
         ++siArg;
         *minMeanQF = atof(argAryStr[siArg]);
      } /*Else If: min mean q-score*/

      else if(
         ! eql_charCp(
            (schar *) "-adj-coords",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *adjCoordBlPtr = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-adj-coords",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *adjCoordBlPtr = 0;


      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - lineages and graph output
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-depth-flag",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: graph flag (experiment name)*/
         ++siArg;
         *depthFlagStr = (schar *) argAryStr[siArg];
      } /*Else If: graph flag (experiment name)*/

      else if(
         ! eql_charCp(
            (schar *) "-fudge",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: fudge length (MIRU VNTR)*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               fudgeLenSI
            );
   

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-fudge %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: fudge length (MIRU VNTR)*/

      else if(
         ! eql_charCp(
            (schar *) "-spoligo-min-score",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min % score for spacer to map*/
         ++siArg;
         *spoligoPercScoreF = atof(argAryStr[siArg]);
      } /*Else If: min % score for spacer to map*/

      else if(
         ! eql_charCp(
            (schar *) "-dr-start",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: Start of direct repeat region*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               drStartSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-dr-start %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: Start of direct repeat region*/

      else if(
         ! eql_charCp(
            (schar *) "-dr-end",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: end of direct repeat region*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               drEndSI
            );

         if(tmpStr[0] != '\0')
         { /*If: error*/
            if(helpBl)
               goto phelp_fun03_sec02;

            fprintf(
               stderr,
               "-dr-end %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: end of direct repeat region*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - clustering settings
      *   o fun03 sec03 sub07 cat01:
      *     - user requested clustering?
      *   o fun03 sec03 sub07 cat02:
      *     - minimum length weight for read scoring
      *   o fun03 sec03 sub07 cat03:
      *     - depth profiling
      *   o fun03 sec03 sub07 cat04:
      *     - minium cluster depth
      *   o fun03 sec03 sub07 cat05:
      *     - minium cluster percent depth
      *   o fun03 sec03 sub07 cat06:
      *     - minium read:read percent distance
      *   o fun03 sec03 sub07 cat07:
      *     - minium consensus:read percent distance
      *   o fun03 sec03 sub07 cat08:
      *     - maximum consensus similarity (before merge)
      *   o fun03 sec03 sub07 cat09:
      *     - maximum error to variant percent (edDist)
      *   o fun03 sec03 sub07 cat10:
      *     - minimum overlap between consensuses
      *   o fun03 sec03 sub07 cat11:
      *     - window size for window scan
      *   o fun03 sec03 sub07 cat12:
      *     - window variant:error ratio
      *   o fun03 sec03 sub07 cat13:
      *     - mimimum indel length to count as difference
      *   o fun03 sec03 sub07 cat14:
      *     - mimimum q-score to keep snp
      *   o fun03 sec03 sub07 cat15:
      *     - maximum percent of consensus maksed
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat01:
      +   - user requested clustering
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-clust",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *clustBlPtr = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-clust",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *clustBlPtr = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat02:
      +   - minimum length weight for read scoring
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-len-weight",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: how much length influces score*/
         ++siArg;
         clustSetSTPtr->lenWeightF=atof(argAryStr[siArg]);
      } /*Else If: how much length influces score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat03:
      +   - depth profiling
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-depth-prof",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) clustSetSTPtr->depthProfBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-depth-prof",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) clustSetSTPtr->depthProfBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat04:
      +   - minium cluster depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-clust-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min cluster read depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &clustSetSTPtr->minDepthUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-clust-depth %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: min cluster read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat05:
      +   - minium cluster percent depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-clust-perc-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: cluster min percent read depth*/
         ++siArg;

         clustSetSTPtr->minPercDepthF =
            atof(argAryStr[siArg]);
      } /*Else If: cluster min percent read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat06:
      +   - minium read:read percent distance
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-read-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: error rate for read to read map*/
         ++siArg;

         clustSetSTPtr->readErrRateF =
            atof(argAryStr[siArg]);
      } /*Else If: error rate for read to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat07:
      +   - minium consensus:read percent distance
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-con-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
        )
     ){ /*Else If: error rate for con to read map*/
        ++siArg;
        clustSetSTPtr->conErrRateF=atof(argAryStr[siArg]);
     } /*Else If: error rate for con to read map*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat08:
      +   - maximum consensus similarity (before merge)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-con-sim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum similarity between cons*/
         ++siArg;

         clustSetSTPtr->maxConSimF=atof(argAryStr[siArg]);
      } /*Else If: maximum similarity between cons*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat09:
      +   - maximum error to variant percent (edDist)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-err-to-var",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum error to variant ratio*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &clustSetSTPtr->varToErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-err-to-var %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum error to variant ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat10:
      +   - minimum overlap between consensuses
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-overlap",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum overlap*/
         ++siArg;

         clustSetSTPtr->percOverlapF =
            atof(argAryStr[siArg]);

         if(clustSetSTPtr->percOverlapF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-overlap %s goes to 0; invalid input\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: minimum overlap*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat11:
      +   - window size for window scan
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-win-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: window length for window scanning*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &clustSetSTPtr->winSizeUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: window length for window scanning*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat12:
      +   - window variant:error ratio
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-win-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: window error to varaint ratio*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &clustSetSTPtr->winErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-err %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: window error to varaint ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat13:
      +   - mimimum indel length to count as difference
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-indel-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &clustSetSTPtr->indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: min indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat14:
      +   - mimimum q-score to keep snp
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-clust-q-snp",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &clustSetSTPtr->minSnpQUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
             stderr,
             "-clust-q-snp %s; non-numeric or to large\n",
             argAryStr[siArg]
            );

           goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: min snp q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub07 Cat15:
      +   - maximum percent of consensus maksed
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-n",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum % masking*/
        ++siArg;
        clustSetSTPtr->maxNPercF = atof(argAryStr[siArg]);
      } /*Else If: maximum % masking*/

      /**************************************************\
      * Fun03 Sec03 Sub08:
      *   - check for help message requests (normal)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) helpBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) helpBl = 1;

      /**************************************************\
      * Fun03 Sec03 Sub09:
      *   - check for extra entry help messages
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h-filt",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add filtering settings to help*/
         helpBl = 1;
         pFiltHelpBl = 1;
      } /*Else If: add filtering settings to help*/

      else if(
         ! eql_charCp(
            (schar *) "-h-con",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add consensus settings to help*/
         helpBl = 1;
         pConHelpBl = 1;
      } /*Else If: add consensus settings to help*/

      else if(
         ! eql_charCp(
            (schar *) "-h-lin",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add lineage settings to help*/
         helpBl = 1;
         pLinHelpBl = 1;
      } /*Else If: add lineage settings to help*/

      else if(
         ! eql_charCp(
            (schar *) "-h-print",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add print settings to help*/
         helpBl = 1;
         pPrintHelpBl = 1;
      } /*Else If: add print settings to help*/

      else if(
         ! eql_charCp(
            (schar *) "-h-clust",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add cluster settings to help*/
         helpBl = 1;
         pClustHelpBl = 1;
      } /*Else If: add cluster settings to help*/

      else if(
         ! eql_charCp(
            (schar *) "-h-other",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: add other settings to help*/
         helpBl = 1;
         pOtherHelpBl = 1;
      } /*Else If: add other settings to help*/

      /**************************************************\
      * Fun03 Sec03 Sub10:
      *   - check for version number requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_freezeTB(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun03 Sec03 Sub11:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         if(helpBl)
            goto phelp_fun03_sec02;

         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub12:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: read in user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(helpBl)
      goto phelp_fun03_sec02;
      /*user wanted help message*/
      /*this trick allows for full help message printing*/
   
   errSC = 0;
   goto ret_fun04_sec04;

   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
      errSC = 1;
      goto ret_fun04_sec04;

   err_fun03_sec04:;
      errSC = 2;
      goto ret_fun04_sec04;

   ret_fun04_sec04:;
      return errSC;
} /*input_freezeTB*/

/*-------------------------------------------------------\
| Main:
|    - Analyze ONT sequenced TB reads
| Input:
|    - numArgsSI:
|      o Integer with number of argument the user input
|    - argAryStr:
|      o C-string array with the input arguments
| Output:
|    - 
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - Run freezeTB on user input
   '   o main sec01:
   '     - Variable declerations
   '   o main sec02:
   '     - initialize, get input, and set up memory
   '   o main sec03:
   '     - check user input database (if can open)
   '   o main sec04:
   '     - check output files (can I open?)
   '   o main sec05:
   '     - read in databases
   '   o main sec06:
   '     - get reference stats and print consensus header
   '   o main sec07:
   '     - Do read analysis
   '   o main sec08:
   '     - print read data
   '   o main sec09:
   '     - collapse consensus and consensus analysis
   '   o main sec10:
   '     - run mixed infection detection (if requested)
   '   o main sec11:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - Variable declerations
   ^   o main sec01 sub01:
   ^     - general IO variables (applies to multple subs)
   ^   o main sec01 sub02:
   ^     - temporay and error reporting variables
   ^   o main sec01 sub03:
   ^     - filtering and sam file variables (adjust coord)
   ^   o main sec01 sub04:
   ^     - read depth and coverage stats variables
   ^   o main sec01 sub05:
   ^     - AMR detection variables
   ^   o main sec01 sub06:
   ^     - miru lineage unique variables
   ^   o main sec01 sub07:
   ^     - spoligotyping unique variables
   ^   o main sec01 sub08:
   ^     - consensus building/mixed infection variables
   ^   o main sec01 sub09:
   ^     - masking unique variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec01 Sub01:
   *   - general IO variables (applies to multple subs)
   \*****************************************************/

   schar *prefixStr = def_prefix_freezeTB;
   FILE *outFILE = 0;

   /*****************************************************\
   * Main Sec01 Sub02:
   *   - Temporay and error reporting variables
   \*****************************************************/

   schar spareStr[1024]; /*extra buffer*/
   schar *tmpStr = 0;

   schar errSC = 0;
   slong errSL = 0;

   /*****************************************************\
   * Main Sec01 Sub03:
   *   - filtering and sam file variables (adjust coord)
   \*****************************************************/

   schar *samFileStr = 0;
   schar adjCoordBl = def_adjCoord_freezeTBDefs;

   /*for sam file processing*/
   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   FILE *samFILE = 0;

   schar multiRefBl = 0; /*1: means multiple refs/exit*/
   sint lenRefSI = 0;
   ulong tabUL = mkDelim_ulCp('\t'); /*only used in ref*/
   schar refIdStr[def_lenFileName_freezeTB];

   /*for filtering reads*/
   float minMedianQF = def_minMedianQ_freezeTBDefs;
   float minMeanQF = def_minMeanQ_freezeTBDefs;

   /*****************************************************\
   * Main Sec01 Sub04:
   *   - read depth and coverage stats variables
   \*****************************************************/

   schar coordFileStr[def_lenFileName_freezeTB]; /*input*/
   schar readStatsStr[def_lenFileName_freezeTB];
      /*output file name*/

   /*tsv file output*/
   schar *depthFlagStr = def_depthFlag_freezeTB;

   struct geneCoord *coordsHeapST = 0;
   sint numCoordsSI = 0;
   uint lastBaseUI = 0;
   uint firstBaseUI = 0;

   sint *readMapArySI = 0;
   sint *filt_readMapArySI = 0;
   sint offTargReadsSI = 0;
   sint oldOffTargSI = 0;
   sint noMapReadSI = 0;

   /*****************************************************\
   * Main Sec01 Sub05:
   *   - AMR detection variables
   \*****************************************************/

   schar idFileStr[def_lenFileName_freezeTB];
      /*read id printing*/

   schar conAmrStr[def_lenFileName_freezeTB];
   schar amrDbFileStr[def_lenFileName_freezeTB]; 
   schar readAmrStr[def_lenFileName_freezeTB];
   schar logFileStr[def_lenFileName_freezeTB];

   FILE *idFILE = 0; /*For read id printing*/
   FILE *logFILE = 0; /*for mixed infection currently*/

   schar frameshiftBl = def_checkFramshift_tbAmrDefs;
   struct amrST *amrHeapAryST = 0;
   struct amrHit_checkAmr *amrHitHeapSTList = 0;
   sint numAmrSI = 0;
   sint numHitsSI = 0;

   schar *drugHeapAryStr = 0;
   sint numDrugsSI = 0;
   sint maxDrugsSI = 0;
   uint totalReadsUI = 0;

   float minPercMapF = def_minPercMapped_tbAmrDefs;
       /*min percenatge of mapped reads to keep AMRs
       `  when checking against reads
       */

   /*****************************************************\
   * Main Sec01 Sub06:
   *   - miru lineage unique variables
   \*****************************************************/

   struct miruTbl *miruHeapST = 0;
   sint fudgeLenSI = def_fudgeLen_tbMiruDefs;

   /*Input or output files for MIRU lineages*/
   schar miruDbFileStr[def_lenFileName_freezeTB];
      /*table of lineages*/

   schar readMiruStr[def_lenFileName_freezeTB];
       /*read results output*/

   schar conMiruStr[def_lenFileName_freezeTB];
      /*consensus results output*/

   /*****************************************************\
   * Main Sec01 Sub07:
   *   - Spoligotyping unique variables
   \*****************************************************/

   schar spoligoRefFileStr[def_lenFileName_freezeTB];
   schar spoligoDbFileStr[def_lenFileName_freezeTB];

   schar checkSpoligoLinBl = 1;
   #define def_lenSpolAry_main 128
   uint spoligoAryUI[def_lenSpolAry_main + 1];
   schar outSpoligoFileStr[def_lenFileName_freezeTB];
   schar outReadSpoligoFileStr[def_lenFileName_freezeTB];

   schar spolErrSC = 0;

   FILE *spoligoOutFILE = 0;

   /*variables for kmer search*/
   uchar lenKmerUC = def_lenKmer_kmerFind;
   float minKmerPercF = def_minKmerPerc_kmerFind;
   float percShiftF = def_percShift_kmerFind;
   float percExtraNtInWinF = def_extraNtInWin_kmerFind;

   float spoligoPercScoreF = def_minPercScore_tbSpolDefs;

   sint drStartSI =  def_DRStart_tbSpolDefs;
   sint drEndSI =  def_DREnd_tbSpolDefs;
   struct alnSet alnSetStackST; /*Alingment settings*/

   struct spolST *lineageHeapAryST = 0;
   sint numLineagesSI = 0;
   uint spoligoNumReadsUI = 0;

   /*kmer finding of spoligotype variables*/
   struct tblST_kmerFind kmerTblStackST;
   struct refST_kmerFind *kmerRefAryST = 0;
   sint numSpoligosSI = 0;

   /*****************************************************\
   * Main Sec01 Sub08:
   *   - consensus building/mixed infection variables
   \*****************************************************/

   schar samConStr[def_lenFileName_freezeTB];
   schar conTsvStr[def_lenFileName_freezeTB];

   struct set_tbCon tbConSettings;
   struct conNt_tbCon *conNtHeapAryST = 0;
   FILE *samConFILE = 0;

   struct samEntry *samConSTAry = 0;
   sint numFragSI = 0;
   sint siCon = 0; /*iterator for consensus step*/

   schar mixedInfectBl = def_mixedInfect_freezeTBDefs;
   struct index_clustST *indexHeapST = 0;
   struct con_clustST *conListHeapST = 0;
   struct con_clustST *conNodeST = 0;
   struct set_clustST clustSetStackST;

   /*****************************************************\
   * Main Sec01 Sub09:
   *   - masking unique variables
   \*****************************************************/

   schar maskPrimFileStr[def_lenFileName_freezeTB];
   uint *maskStartHeapAryUI = 0;
   uint *maskEndHeapAryUI = 0;
   uint *maskFlagHeapAryUI = 0;
   uint maskNumPrimUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and set up memory
   ^   o main sec02 sub01:
   ^     - set up default file paths
   ^   o main sec02 sub02:
   ^     - initialize variables
   ^   o main sec02 sub03:
   ^     - get input
   ^   o main sec02 sub04:
   ^     - set up memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - set up default file paths
   \*****************************************************/

   amrPath_freezeTBPaths(amrDbFileStr);
   miruPath_freezeTBPaths(miruDbFileStr);
   coordPath_freezeTBPaths(coordFileStr);
   spolSpacerPath_freezeTBPaths(spoligoRefFileStr);
   spolLineagePath_freezeTBPaths(spoligoDbFileStr);
   maskPath_freezeTBPaths(maskPrimFileStr);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - initialize variables
   \*****************************************************/

   refIdStr[0] = '\0';
   maskPrimFileStr[0] = '\0';

   init_set_tbCon(&tbConSettings);
   init_samEntry(&samStackST);
   init_alnSet(&alnSetStackST);
   init_set_clustST(&clustSetStackST);

   init_tblST_kmerFind(&kmerTblStackST);

   /*initialize spoligotyping array*/
   for(
      numLineagesSI = 0;
      numLineagesSI < def_lenSpolAry_main;
      ++numLineagesSI
   ) spoligoAryUI[numLineagesSI] = 0;

   spoligoAryUI[numLineagesSI] = -1;

   numLineagesSI = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - get input
   \*****************************************************/

   errSC =
      input_freezeTB(
         numArgsSI,
         argAryStr,
         &samFileStr,      /*reads*/
         amrDbFileStr,     /*AMR database tbAmr*/
         coordFileStr,     /*gene coordianges ampDepth*/
         miruDbFileStr,    /*miru database file*/
         spoligoRefFileStr,/*spoligotype seqeunces*/
         spoligoDbFileStr, /*lineage database*/
         maskPrimFileStr,  /*masking file for primers*/
         &prefixStr,       /*output file names*/
         &minMedianQF,     /*median Q-score to filter*/
         &minMeanQF,       /*mean Q-score to filter with*/
         &minPercMapF,     /*pitch AMRs beneath; tbAMR*/
         &tbConSettings,   /*consensus/variants settings*/
         &adjCoordBl,      /*do coordinate adjusting?*/
         &depthFlagStr,    /*ampDepth column 1*/
         &mixedInfectBl,   /*tells if mixed infct detect*/
         &clustSetStackST, /*settings for mixed infect*/
         &frameshiftBl,    /*AMR frame shift checking*/
         &fudgeLenSI,      /*fudge size for MIRU tables*/
         &spoligoPercScoreF, /*% score to count map*/
         &drStartSI,       /*start of direct repeat*/
         &drEndSI          /*end of direct repeat*/
   );

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version print to no error*/
      goto cleanUp_main_sec11_sub03;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - set up memory
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );

      goto err_main_sec11_sub02;
   } /*If: memory error*/


   errSC =
      setup_tblST_kmerFind(
         &kmerTblStackST,
         lenKmerUC
      );

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up tblST_kmerFind struct\n"
      );

      goto err_main_sec11_sub02;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - check user input database (if can open)
   ^   o main sec03 sub01:
   ^     - check if MIRU table exists
   ^   o main sec03 sub02:
   ^     - check if spoligotyping spacer sequences exists
   ^   o main sec03 sub03:
   ^     - check if spoligotyping lineage database
   ^   o main sec03 sub04:
   ^     - check if amr table exists
   ^   o main sec03 sub05:
   ^     - open the sam file
   ^   o main sec03 sub06:
   ^     - check if gene coordinates file exits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - check if MIRU table exists
   \*****************************************************/

   outFILE =
      fopen(
         (char *) miruDbFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: I could not open the MIRU table*/
      fprintf(
         stderr,
         "unable to open -miru-tbl %s\n",
         miruDbFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: I could not open the MIRU table*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - check if spoligotyping spacer sequences exists
   \*****************************************************/

   outFILE =
      fopen(
         (char *) spoligoRefFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -spoligo %s\n",
         spoligoRefFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open spoligo spacer sequences*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - check if spoligotyping lineage database
   \*****************************************************/

   outFILE =
      fopen(
         (char *) spoligoDbFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open the spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -db-spoligo %s\n",
         spoligoDbFileStr
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
   * Main Sec03 Sub04:
   *   - check if amr table exists
   \*****************************************************/

   
   outFILE =
      fopen(
         (char *) amrDbFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open -amr-tbl %s\n",
         amrDbFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec03 Sub05:
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
         fprintf(
            stderr,
            "could not open -sam %s\n",
            samFileStr 
         );

         goto err_main_sec11_sub02;
      } /*If: could not open the sam file*/
   } /*Else: need to open sam file*/

   /*****************************************************\
   * Main Sec03 Sub06:
   *   - open gene coordinates file
   \*****************************************************/
   
   outFILE = 
      fopen(
         (char *) coordFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open gene coordinates file*/
      fprintf(
         stderr,
         "could not open -gene-coords %s\n",
         samFileStr 
      );

      goto err_main_sec11_sub02;
   } /*If: could not open gene coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - check output files (can I open?)
   ^   o main sec04 sub01:
   ^     - output file for read stats
   ^   o main sec04 sub02:
   ^     - set up cosensus fragments output file
   ^   o main sec04 sub03:
   ^     - set up read AMRs table outp file name
   ^   o main sec04 sub04:
   ^     - set up read id AMR hit table
   ^   o main sec04 sub05:
   ^     - output file for the AMRs found in consensus
   ^   o main sec04 sub06:
   ^     - set up MIRU reads table output name
   ^   o main sec04 sub07:
   ^     - set up MIRU consensus table output name
   ^   o main sec04 sub08:
   ^     - set up consensus spoligotyping output file
   ^   o main sec04 sub09:
   ^     - set up read spoligotyping output file name
   ^   o main sec04 sub10:
   ^     - set up open consensus output file name
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - set up read stats file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-depths.tsv",
        readStatsStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readStatsStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/
 
   /*****************************************************\
   * Main Sec04 Sub02:
   *   - set up cosensus fragments output file
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-con-variants.tsv",
        conTsvStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conTsvStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - set up read AMRs table outp file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-read-amrs.tsv",
        readAmrStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readAmrStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub04:
   *   - set up read id AMR hit table
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-id-amrs.tsv",
        idFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         idFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub05:
   *   - Set up the name for the consensus AMRs table
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-con-amrs.tsv",
        conAmrStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conAmrStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub06:
   *   - set up MIRU reads table output name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-read-miru.tsv",
        readMiruStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readMiruStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub07:
   *   - set up MIRU consensus table output name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
        prefixStr,
        (schar *) "-con-miru.tsv",
        conMiruStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conMiruStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub08:
   *   - set up consensus spoligotyping output file
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         prefixStr,
         (schar *) "-con-spoligo.tsv",
         outSpoligoFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         outSpoligoFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub09:
   *   - set up read spoligotyping output file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         prefixStr,
         (schar *) "-read-spoligo.tsv",
         outReadSpoligoFileStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         outReadSpoligoFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec04 Sub10:
   *   - set up open consensus output file name
   \*****************************************************/

   errSC =
      outputPath_freezeTBPaths(
         prefixStr,
         (schar *) "-cons.sam",
         samConStr
      );

   if(errSC)
   { /*If: could not open file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         outReadSpoligoFileStr
      );

      goto err_main_sec11_sub02;
   } /*If: could not open file*/

   samConFILE =
      fopen(
         (char *) samConStr,
          "w"
      ); /*already checked if could open*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - read in databases
   ^   o main sec05 sub01:
   ^     - get gene mapping coodiantes
   ^   o main sec05 sub02:
   ^     - get amr table
   ^   o main sec05 sub03:
   ^     - get MIRU lineage table
   ^   o main sec05 sub04:
   ^     - get spoligotyping spacer sequences
   ^   o main sec05 sub05:
   ^     - get spoligotyping lineages
   ^   o main sec05 sub06:
   ^     - get masking primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - get gene mapping coodiantes
   \*****************************************************/

   coordsHeapST =
      getCoords_geneCoord(
         coordFileStr,
         &numCoordsSI,
         &lenBuffUL    /*reusing to hold error output*/
      );

   if(! coordsHeapST)
   { /*If: error*/
      if(lenBuffUL & def_memErr_geneCoord)
         fprintf(
            stderr,
            "memory error reading -gene-coords %s\n",
            coordFileStr
         );

      else
         fprintf(
            stderr,
            "line %lu is wrong in -gene-coords %s\n",
            lenBuffUL >> 8,
            coordFileStr
         );

      goto err_main_sec11_sub02;
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
   * Main Sec05 Sub02:
   *   - get amr table
   \*****************************************************/

   amrHeapAryST =
      readTbl_amrST(
         amrDbFileStr,
         (uint *) &numAmrSI,
         &drugHeapAryStr,
         &numDrugsSI,
         &maxDrugsSI,
         &errSC
      ); /*Read in the amr database*/

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_amrST)
         fprintf(
           stderr,
           "%s is not in tbAmr format or has no data\n",
           amrDbFileStr
         );

      if(errSC == def_memErr_amrST)
         fprintf(
           stderr,
           "memory error; when processing variant id's\n"
         );

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - get MIRU lineage table
   \*****************************************************/
 
   miruHeapST =
      get_miruTbl(
         miruDbFileStr,
         &errSC
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_tbMiruDefs)
         fprintf(
            stderr,
            "Could not open -miru-tbl %s\n",
            miruDbFileStr
          ); /*Let the user know the error type*/

      else
         fprintf(
            stderr,
            "memory error reading -miru-tbl %s\n",
            miruDbFileStr
         );

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - get spoligotyping spacer sequences
   \*****************************************************/

   kmerRefAryST =
      faToAry_refST_kmerFind(
        spoligoRefFileStr,
        lenKmerUC,
        &numSpoligosSI,
        minKmerPercF,
        &kmerTblStackST,
        percExtraNtInWinF,
        percShiftF,
        &alnSetStackST,
        &errSC
   );

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_kmerFind)
      { /*If: file error*/
         fprintf(
            stderr,
            "-spoligo %s is not valid\n",
            spoligoRefFileStr
         );
 
         goto err_main_sec11_sub02;
      } /*If: file error*/
 
      else
      { /*Else: memory error*/
         fprintf(
            stderr,
            "Ran out of memory getting spoligo seqs\n"
         );
 
          goto err_main_sec11_sub02;
      } /*Else: memory error*/
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub05:
   *   - get spoligotyping lineage database
   \*****************************************************/

   if(checkSpoligoLinBl)
   { /*If: have lineage database*/
      lineageHeapAryST =
         readDb_spolST(
            spoligoDbFileStr,
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
               spoligoDbFileStr
            );

            fprintf(
               stderr,
               "skipping spoligotype checks\n"
            );
         } /*If: file error*/

         else
         { /*Else: memory error*/
            fprintf(
               stderr,
               "memory error -db %s\n",
               spoligoDbFileStr
            );

            goto err_main_sec11_sub02;
         } /*Else: memory error*/
      } /*If: error*/
   } /*If: have lineage database*/

   /*****************************************************\
   * Main Sec05 Sub06:
   *   - get masking primer coordinates
   \*****************************************************/

   maskNumPrimUI = 0;

   if(
         maskPrimFileStr[0] != '-' 
      && maskPrimFileStr[0] != '\0'
   ){ /*If: primer masking file input*/
      maskNumPrimUI =
         (uint)
         getCoords_maskPrim(
            maskPrimFileStr,
            &maskStartHeapAryUI,
            &maskEndHeapAryUI,
            &maskFlagHeapAryUI,
            &errSL
         );

      if(! maskNumPrimUI)
      { /*If: error*/
         if(errSL == def_emptyFileErr_maskPrim)
         { /*If: empty file*/
            fprintf(
               stderr,
               "-mask-prim %s is emtpy\n",
               maskPrimFileStr
            );
         } /*If: empty file*/

         else if(errSL & def_fileErr_maskPrim)
         { /*Else If: invalid line*/
            fprintf(
               stderr,
               "line %lu in -mask-prim %s is invalid\n",
               (errSL >> 8),
               maskPrimFileStr
            );
         } /*Else If: invalid line*/
         
         else
         { /*Else: memory error*/
            fprintf(
               stderr,
               "memory error reading -mask-prim %s\n",
               maskPrimFileStr
            );
         } /*Else: memory error*/

         goto err_main_sec11_sub02;
      } /*If: error*/
   } /*If: primer masking file was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - get reference stats and print consensus header
   ^   o main sec06 sub01:
   ^     - get reference name/length from header
   ^   o main sec06 sub02:
   ^     - print tbCon header for sam file
   ^   o main sec06 sub03:
   ^     - check if have reference name/length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - get reference length from header
   *   o main sec06 sub01 cat01:
   *     - get first sam file entry + start loop
   *   o main sec06 sub01 cat02:
   *     - print comment entry (end loop if not comment)
   *   o main sec06 sub01 cat03:
   *     - if sequence entry; get id and length
   *   o main sec06 sub01 cat04:
   *     - move to next entry
   *   o main sec06 sub01 cat05:
   *     - check for errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec06 Sub01 Cat01:
   +   - get first sam file entry + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(adjCoordBl)
   { /*If: adjusting coordinates*/
      lenRefSI = (sint) lastBaseUI;
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
      + Main Sec06 Sub01 Cat02:
      +   - print comment entry (end loop if not comment)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(*samStackST.extraStr != '@')
         break; /*off header*/

      if(
            adjCoordBl
         &&
            ! eql_charCp(
               (schar *) "@SQ\t",
               samStackST.extraStr,
               '\t'
            )
      ) goto nextHeader_main_sec06_sub01_cat04;
        /*do not print sequence entries for header*/

      fprintf(
         samConFILE,
         "%s\n",
         samStackST.extraStr
      );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec06 Sub01 Cat03:
      +   - if sequence entry; get id and length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
         ! eql_charCp(
            (schar *) "@SQ\t",
            samStackST.extraStr,
            '\t'
         )
      ){ /*If: sequence entry*/
         if(multiRefBl)
         { /*If: sam file has multiple references*/
            fprintf(
               stderr,
               "-sam %s has multiple references\n",
               samFileStr
            ); 

            goto err_main_sec11_sub02;
         } /*If: sam file has multiple references*/

         multiRefBl = 1;

         /*get past "@SQ\t"*/
         tmpStr = samStackST.extraStr + 4; 

         while(*tmpStr++ != ':')
            if(*tmpStr < 31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_main_sec06_sub01_cat04;

         tmpStr +=
            cpDelim_ulCp(
               refIdStr,
               tmpStr,
               tabUL,
               '\t'
            ); /*copy reference id*/

         ++tmpStr;
        
         if(*tmpStr < 31 )
            goto nextHeader_main_sec06_sub01_cat04;

         /*move past LN: flag*/
         while(*tmpStr++ != ':')
            if(*tmpStr <31)
               break;

         if(*(tmpStr - 1) != ':')
            goto nextHeader_main_sec06_sub01_cat04;

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
      + Main Sec06 Sub01 Cat04:
      +   - move to next entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      nextHeader_main_sec06_sub01_cat04:;

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: read in header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec06 Sub01 Cat05:
   +   - check for errors
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(errSC)
   { /*If: error reading sam file header*/
      if(errSC == def_EOF_samEntry)
         fprintf(
            stderr,
            "-sam %s only has header or is empty\n",
            samFileStr
         );

      else
         fprintf(
            stderr,
            "memory error reading header of -sam %s\n",
            samFileStr
         );

      goto err_main_sec11_sub02;
   } /*If: error reading sam file header*/

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - print tbCon header for sam file
   *   o main sec06 sub02 cat01:
   *     - tbCon cosensus settings
   *   o main sec06 sub02 cat02:
   *     - tbCon variant print (tsv file) settings
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec06 Sub02 Cat01:
   +   - tbCon cosensus settings
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! mixedInfectBl)
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
         tbConSettings.minMapqUC,
         tbConSettings.minQSI,
         tbConSettings.minInsQSI
      );

      fprintf(
         samConFILE,
         " -min-len %i -min-depth %i -perc-snp-sup %.2f",
         tbConSettings.minLenSI,
         tbConSettings.minDepthSI,
         tbConSettings.minPercSnpF
      );

      fprintf(
         samConFILE,
         " -perc-ins-sup %.2f -min-del-sup %.2f",
         tbConSettings.minPercInsF,
         tbConSettings.minPercDelF
      );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec06 Sub02 Cat02:
      +   - tbCon variant print (tsv file) settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      fprintf(
         samConFILE,
         " -min-mapq %i -min-q %i -min-q-ins %i",
         tbConSettings.minMapqUC,
         tbConSettings.minQSI,
         tbConSettings.minInsQSI
      );

      fprintf(
         samConFILE,
         " -p-min-depth %i -p-perc-snp-sup %.2f",
         tbConSettings.minPrintDepthSI,
         tbConSettings.printMinSupSnpF
      );

      fprintf(
         samConFILE,
         " -p-perc-ins-sup %.2f -p-min-del-sup %.2f",
         tbConSettings.printMinSupInsF,
         tbConSettings.printMinSupDelF
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
   * Main Sec06 Sub03:
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
   ^ Main Sec07:
   ^   - read analysis
   ^   o main sec06 sub01:
   ^     - allocate memory for the read stats arrays
   ^   o main sec06 sub02:
   ^     - filter reads (sam entries)
   ^   o main sec06 sub03:
   ^     - mask primers in reads
   ^   o main sec06 sub04:
   ^     - build filtered histogram
   ^   o main sec06 sub05:
   ^     - build consensus
   ^   o main sec06 sub06:
   ^     - check for AMRs
   ^   o main sec06 sub07:
   ^     - check for MIRU lineages
   ^   o main sec06 sub08:
   ^     - check for spoligotypes
   ^   o main sec06 sub09:
   ^     - move to next read
   ^   o main sec06 sub10:
   ^     - minor clean up (variables unique to sec07)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec07 Sub01:
   *   - allocate memory for the read stats arrays
   \*****************************************************/

   readMapArySI =
      calloc(
         lenRefSI,
         sizeof(sint)
      );

   if(! readMapArySI)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error (read histogram allocation)\n"
      );

      goto err_main_sec11_sub02;
   } /*If: memory error*/

   filt_readMapArySI =
      calloc(
         lenRefSI,
         sizeof(sint)
      );

   if(! filt_readMapArySI)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error (read histogram allocation)\n"
      );

      goto err_main_sec11_sub02;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec07 Sub02:
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

         goto nextRead_main_sec07_sub09;
      } /*If:umapped 4, secondary 256, or suplemtal 2048*/

      if(adjCoordBl)
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
            goto nextRead_main_sec07_sub09;
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
         goto nextRead_main_sec07_sub09;
      */ /*off target*/

      if(samStackST.medianQF < minMedianQF)
         goto nextRead_main_sec07_sub09;
         /*low mean q-score*/

      if(samStackST.medianQF < minMedianQF)
         goto nextRead_main_sec07_sub09;
         /*low median q-score*/

      if(samStackST.mapqUC < tbConSettings.minMapqUC)
         goto nextRead_main_sec07_sub09;
         /*low mapping quality*/

      if(
           samStackST.alnReadLenUI
         < (uint) tbConSettings.minLenSI
      ) goto nextRead_main_sec07_sub09; /*short read*/

      /**************************************************\
      * Main Sec07 Sub03:
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
      * Main Sec07 Sub04:
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
      * Main Sec07 Sub05:
      *   - build consensus
      \**************************************************/

      if(! mixedInfectBl)
      { /*If: not doing mixed infection detection*/
         errSC =
            addRead_tbCon(
               &samStackST,
               &conNtHeapAryST,
               (uint *) &lenRefSI,
               &tbConSettings
            ); /*add read to consnesus*/
               
         if(errSC & def_memErr_tbConDefs)
         { /*If: memory error*/
            fprintf(
                stderr,
                "memory error add read %u to consensus\n",
                totalReadsUI
            );

            goto err_main_sec11_sub02;
         } /*If: memory error*/
      } /*If: not doing mixed infection detection*/

      /**************************************************\
      * Main Sec07 Sub06:
      *   - check for AMRs
      \**************************************************/

      amrHitHeapSTList =
         checkAmr(
            &samStackST,
            amrHeapAryST,
            numAmrSI,
            &numHitsSI,
            0,          /*do not do frame shifts*/
            &errSC
         ); /*Check if read has antibiotic resistance*/

      if(errSC)
      { /*If: memory error*/
         fprintf(
             stderr,
             "memory error on AMR check for  read %u\n",
             totalReadsUI
         );

         goto err_main_sec11_sub02;
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
      * Main Sec07 Sub07:
      *   - check for MIRU lineages 
      \**************************************************/

      incLineage_miruTbl(
         &samStackST,
         fudgeLenSI,
         miruHeapST
      );

      /**************************************************\
      * Main Sec07 Sub08:
      *   - Check for spoligotypes
      \**************************************************/

      spolErrSC =
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            &samStackST,
            drStartSI,
            drEndSI,
            spoligoPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &alnSetStackST
         ); /*find spoligotype with kmer search*/

      spoligoNumReadsUI += (! spolErrSC);

      if(spolErrSC & def_memErr_tbConDefs)
      { /*If: memory error*/
         fprintf(
             stderr,
             "memory error detecting spoligo (read %u)\n",
             totalReadsUI
         );

         goto err_main_sec11_sub02;
      } /*If: memory error*/

      /**************************************************\
      * Main Sec07 Sub09:
      *   - move to next read
      \**************************************************/

      nextRead_main_sec07_sub09:;

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: read anaylsis*/

   /*****************************************************\
   * Main Sec07 Sub10:
   *   - minor clean up (variables unique to sec07)
   \*****************************************************/

   if(! mixedInfectBl)
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
   ^ Main Sec08:
   ^   - print read data
   ^   o main sec08 sub01:
   ^     - print unfiltered read stats
   ^   o main sec08 sub02:
   ^     - print filtered read stats
   ^   o main sec08 sub03:
   ^     - print AMR hits for reads
   ^   o main sec08 sub04:
   ^     - print read MIRU table
   ^   o main sec08 sub05:
   ^     - print read spoligotype entry
   ^   o main sec08 sub06:
   ^     - print tsv file of variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec08 Sub01:
   *   - print unfiltered read stats
   \*****************************************************/

   sort_geneCoord(
      coordsHeapST,
      0,
      (uint) numCoordsSI
   );

   outFILE =
      fopen(
         (char *) readStatsStr,
         "w"
      ); /*already check if could open*/

   phead_ampDepth(outFILE);

   phist_ampDepth(
      readMapArySI,
      tbConSettings.minPrintDepthSI,      
      coordsHeapST,
      numCoordsSI,
      offTargReadsSI,
      noMapReadSI,
      depthFlagStr,
      outFILE
    ); /*print the unfilterd read stats*/

   free(readMapArySI);
   readMapArySI = 0;

   /*****************************************************\
   * Main Sec08 Sub02:
   *   - print filtered read stats
   \*****************************************************/

   /*adding a unique column name so I can compare
   `  filtered and unfiltered reads
   */
   tmpStr = spareStr;

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         depthFlagStr,
         0,
         '\0'
      );

   cpDelim_ulCp(
      tmpStr,
      (schar *) "-filt",
      0,
      '\0'
   ); /*marker for filtered reads*/

   phist_ampDepth(
      filt_readMapArySI,
      tbConSettings.minPrintDepthSI,      
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
   * Main Sec08 Sub03:
   *   - print AMR hits for reads
   \*****************************************************/

   outFILE =
      fopen(
         (char *) readAmrStr,
         "w"
      ); /*already checked if could open*/

   pReadHead_checkAmr(outFILE);

   pRead_checkAmr(
      (uint) tbConSettings.minDepthSI,
      minPercMapF,
      (float) 0, /*not really usefull, so leaving out*/
      totalReadsUI,
      amrHeapAryST,
      (uint) numAmrSI,
      drugHeapAryStr,
      outFILE
   ); /*print AMRs detected in reads*/
      
   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec08 Sub04:
   *   - print read MIRU table
   \*****************************************************/

   errSC =
      p_miruTbl(
         miruHeapST,
         readMiruStr
      ); /*print AMRs detected in reads*/

   if(errSC)
   { /*If: impossible case*/
      fprintf(
         stderr,
         "could not write read MIRU table to %s\n",
         readMiruStr
      );
   } /*If: impossible case*/

   /*****************************************************\
   * Main Sec08 Sub05:
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
         prefixStr,
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
      lenKmerUC = 0;
      lenKmerUC < def_lenSpolAry_main;
      ++lenKmerUC
   ) spoligoAryUI[lenKmerUC] = 0;

   spoligoAryUI[lenKmerUC] = -1;

   lenKmerUC = def_lenKmer_kmerFind;
   
   /*****************************************************\
   * Main Sec08 Sub06:
   *   - print tsv file of variants
   \*****************************************************/

   /*Build the tsv of variants table*/
   if(! mixedInfectBl)
      errSC =
         pvar_tbCon(
            conNtHeapAryST,
            lenRefSI,
            refIdStr,
            &tbConSettings,
            conTsvStr
         ); /*print variants (not a vcf)*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec09:
   ^   - collapse consensus and consensus analysis
   ^   o main sec09 sub01:
   ^     - collapse consensus
   ^   o main sec09 sub02:
   ^     - print consensus and do ananlysis
   ^   o main sec09 sub03:
   ^     - close output files and free uneeded variables
   ^   o main sec09 sub04:
   ^     - print consensus MIRU lineages
   ^   o main sec09 sub05:
   ^     - print detected spoligotype (consensus)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec09 Sub01:
   *   - collapse consensus
   \*****************************************************/

   if(mixedInfectBl)
      goto mixedInfect_main_sec11;

   samConSTAry =
      collapse_tbCon(
         conNtHeapAryST,
         lenRefSI,
         &numFragSI,
         refIdStr,
         &tbConSettings,
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
      fprintf(
         stderr,
         "could not collapse consensus\n"
      );

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec09 Sub02:
   *   - print consensus and do ananlysis
   *   o main sec09 sub04 cat01:
   *     - open files + run consensus fragment loop
   *   o main sec09 sub04 cat02:
   *     - print consensus fragments
   *   o main sec09 sub04 cat03:
   *     - AMR detection and printing
   *   o main sec09 sub04 cat04:
   *     - MIRU-VNTR lineage detection and printing
   *   o main sec09 sub04 cat05:
   *     - detect spoligotypes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec09 Sub04 Cat01:
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
     + Main Sec09 Sub04 Cat02:
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
     + Main Sec09 Sub04 Cat03:
     +   - AMR detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      amrHitHeapSTList =
         checkAmr(
            &samConSTAry[siCon],
            amrHeapAryST,
            numAmrSI,
            &numHitsSI,
            frameshiftBl,
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
     + Main Sec09 Sub04 Cat04:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         &samConSTAry[siCon],
         fudgeLenSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec09 Sub04 Cat05:
     +   - spoligotype detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     spolErrSC &=
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            &samConSTAry[siCon],
            drStartSI,
            drEndSI,
            spoligoPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &alnSetStackST
         ); /*find spoligotype with kmer search*/
   } /*Loop: print and analyize consensuse*/

   /*****************************************************\
   * Main Sec09 Sub03:
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
      fprintf(
         stderr,
         "error during consensus analyisis step\n"
      );

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec09 Sub04:
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
      fprintf(
         stderr,
         "could not write consensuses MIRU lineages %s\n",
         conMiruStr
      ); 

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec09 Sub05:
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
         prefixStr,
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

   freeStack_alnSet(&alnSetStackST); /*good pratice*/

   fclose(spoligoOutFILE);
   spoligoOutFILE = 0;

   goto ret_main_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec10:
   ^   - run mixed infection detection (if requested)
   ^   o main sec10 sub01:
   ^     - check if can open log files
   ^   o main sec10 sub02:
   ^     - run mixed infection detection
   ^   o main sec10 sub03:
   ^     - print clusters for mixed infection
   ^   o main sec10 sub04:
   ^     - amr/miru/spoligotype detection on clusters
   ^   o main sec10 sub05:
   ^     - print consensus MIRU lineages
   ^   o main sec10 sub06:
   ^     - print detected spoligotype
   \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec10 Sub01:
   *   - check if can open log files
   \*****************************************************/

   mixedInfect_main_sec11:;

   if(samFILE == stdin)
   { /*If: piping file to mixed infection detect*/
      fprintf(
         stderr,
         "-sam is stdin; not allowed for mixed infection"
      );

      fprintf(
         stderr,
         " mode\n"
      );

      goto err_main_sec11_sub02;
   } /*If: piping file to mixed infection detect*/

   errSC =
      outputPath_freezeTBPaths(
         prefixStr,
         (schar *) "-mixed-infect-log.txt",
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

      goto err_main_sec11_sub02;
   } /*If: could not open log flie*/

   /*****************************************************\
   * Main Sec10 Sub02:
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
         &clustSetStackST,
			&tbConSettings,
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
         fprintf(
            stderr,
            "memory error during mixed infection step\n"
         );

      else if(errSC == def_fileErr_edClust)
         fprintf(
            stderr,
            "file error during mixed infection step\n"
         );

      else
         fprintf(
            stderr,
            "no reads for mixed infection step\n"
         );

      goto err_main_sec11_sub02;
   } /*If: error*/

   if(logFILE != stderr)
      fclose(logFILE);
   logFILE = 0;

   /*****************************************************\
   * Main Sec10 Sub03:
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
      fprintf(
         stderr,
         "memory error printing cluster consensuses\n"
      );

      goto err_main_sec11_sub02;
   } /*If: had error*/

   errSC =
      pbins_clustST(
         prefixStr,
         clustSetStackST.clustSI,
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
         fprintf(
            stderr,
            "memory error printing clusters\n"
         );
      else
         fprintf(
            stderr,
            "file error printing clusters\n"
         );

      goto err_main_sec11_sub02;
   } /*If: had error*/

   fclose(samFILE);
   samFILE = 0;

   if(indexHeapST)
      freeHeap_index_clustST(indexHeapST);
   indexHeapST = 0;

   /*****************************************************\
   * Main Sec10 Sub04:
   *   - amr/miru/spoligotype detection on clusters
   *   o main sec10 sub04 cat01:
   *     - amr detection + start loop
   *   o main sec10 sub04 cat02:
   *     - MIRU-VNTR lineage detection and printing
   *   o main sec10 sub04 cat03:
   *     - spoligotype detection and printing
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec10 Sub04 Cat01:
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
            frameshiftBl,
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
     + Main Sec10 Sub04 Cat02:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      incLineage_miruTbl(
         conNodeST->samSTPtr,
         fudgeLenSI,
         miruHeapST
      ); /*find MIRU lineages in consensus fragment*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec10 Sub04 Cat03:
     +   - spoligotype detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     spolErrSC &=
         sam_spolFind(
            &kmerTblStackST,
            kmerRefAryST,
            numSpoligosSI,
            conNodeST->samSTPtr,
            drStartSI,
            drEndSI,
            spoligoPercScoreF,
            spoligoAryUI,
            1,             /*always assume fragments*/
            &alnSetStackST
         ); /*find spoligotype with kmer search*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec10 Sub04 Cat04:
     +   - move to next cluster
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     conNodeST = conNodeST->nextST;
   } /*Loop: find amrs and lineages for consensuses*/

   if(conListHeapST)
      freeHeapList_con_clustST(conListHeapST);
   conListHeapST = 0;
   conNodeST = 0;
         
   /*****************************************************\
   * Main Se109 Sub05:
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
      fprintf(
         stderr,
         "could not write consensuses MIRU lineages %s\n",
         conMiruStr
      ); 

      goto err_main_sec11_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec10 Sub06:
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
         prefixStr,
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

   goto ret_main_sec11;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec11:
   ^   - clean up
   ^   o main sec11 sub01:
   ^     - no error clean up
   ^   o main sec11 sub02:
   ^     - error clean up
   ^   o main sec11 sub03:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec11 Sub01:
   *   - no error clean up
   \*****************************************************/

   ret_main_sec11:;
      errSC = 0;
      goto cleanUp_main_sec11_sub03;

   /*****************************************************\
   * Main Sec11 Sub02:
   *   - error clean up
   \*****************************************************/

   err_main_sec11_sub02:;
      errSC = 1;
      goto cleanUp_main_sec11_sub03;

   /*****************************************************\
   * Main Sec11 Sub03:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_main_sec11_sub03:;
      freeStack_samEntry(&samStackST);

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

      freeStack_set_clustST(&clustSetStackST);
      freeStack_alnSet(&alnSetStackST);

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

      return errSC;
} /*main*/

/* Note01:
     - Windows enviromental variables
` From: https://pureinfotech.com/list-environment-variables-windows-10/
`  %ALLUSERSPROFILE%           C:\ProgramData
`  %APPDATA%                   C:\Users\{username}\AppData\Roaming
`  %COMMONPROGRAMFILES%        C:\Program Files\Common Files
`  %COMMONPROGRAMFILES(x86)%   C:\Program Files (x86)\Common Files
`  %CommonProgramW6432%        C:\Program Files\Common Files
`  %COMSPEC%                   C:\Windows\System32\cmd.exe
`  %HOMEDRIVE%                 C:\
`  %HOMEPATH%                  C:\Users\{username}
`  %LOCALAPPDATA%              C:\Users\{username}\AppData\Local
`  %LOGONSERVER%               \\{domain_logon_server}
`  %PATH%                      C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem
`  %PathExt%                   .com;.exe;.bat;.cmd;.vbs;.vbe;.js;.jse;.wsf;.wsh;.msc
`  %PROGRAMDATA%               C:\ProgramData
`  %PROGRAMFILES%              C:\Program Files
`  %ProgramW6432%              C:\Program Files
`  %PROGRAMFILES(X86)%         C:\Program Files (x86)
`  %PROMPT%                    $P$G
`  %SystemDrive%               C:
`  %SystemRoot%                C:\Windows
`  %TEMP%                      C:\Users\{username}\AppData\Local\Temp
`  %TMP%                       C:\Users\{username}\AppData\Local\Temp
`  %USERDOMAIN%                Userdomain associated with current user.
`  %USERDOMAIN_ROAMINGPROFILE% Userdomain associated with roaming profile.
`  %USERNAME%                  {username}
`  %USERPROFILE%               C:\Users\{username}
`  %WINDIR%                    C:\Windows
`  %PUBLIC%                    C:\Users\Public
`  %PSModulePath%              %SystemRoot%\system32\WindowsPowerShell\v1.0\Modules\
`  %OneDrive%                  C:\Users\{username}\OneDrive
`  %DriverData%                C:\Windows\System32\Drivers\DriverData
`  %CD%                        Outputs current directory path. (Command Prompt.)
`  %CMDCMDLINE%                Outputs command line used to launch current Command Prompt session. (Command Prompt.)
`  %CMDEXTVERSION%             Outputs the number of current command processor extensions. (Command Prompt.)
`  %COMPUTERNAME%              Outputs the system name.
`  %DATE%                      Outputs current date. (Command Prompt.)
`  %TIME%                      Outputs time. (Command Prompt.)
`  %ERRORLEVEL%                Outputs the number of defining exit status of previous command. (Command Prompt.)
`  %PROCESSOR_IDENTIFIER%      Outputs processor identifier.
`  %PROCESSOR_LEVEL%           Outputs processor level.
`  %PROCESSOR_REVISION%        Outputs processor revision.
`  %NUMBER_OF_PROCESSORS%      Outputs the number of physical and virtual cores.
`  %RANDOM%                    Outputs random number from 0 through 32767.
`  %OS%                        Windows_NT
*/

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
