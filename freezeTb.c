/*########################################################
# Name: freezeTb
#   - Process input reads for AMR(s) and MIRU lineages.
#     This also builds a cosenssu and prints out depths
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   - header:
'     o Included libraries
'   o main:
'     - main function which drives everything
'   o fun-01: getInput_freezeTb
'     - Gets the user input from the arguments array for
'       freezeTb
'   o fun-02: pHelp_freezeTb
'     - Prints the help message for freezeTb
'   o fun-03: pVersion_freezeTb
'     - Prints the version number for freezeTb and its
'   o note-01:
'     - Windows enviromental variables
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
|   o header sec-01:
|     - Included libraries
|   o header sec-02:
|     - Defined variables/default settings
|   o header sec-03:
|     - Function headers
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^    - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

/*Libraries that are modules*/
#include "ampDepthSource/ampDepth-fun.h"
#include "tbConSource/tbCon-fun.h"
#include "tbAmrSource/checkAmr.h"
#include "generalLib/trimSam.h"
#include "tbMiruSource/miruTblStruct.h"
#include "tbSpoligoSource/tbSpoligo-memwater-fun.h"
#include "primMaskSrc/primMask-fun.h"

/*libraries that are not modules (used by modules)*/
#include "generalLib/samEntryStruct.h"
#include "generalLib/geneCoordStruct.h"
#include "tbAmrSource/amrStruct.h"

/*.c libraries for tbSpoligo*/
#include "tbSpoligoSource/tbSpoligo-readDb.h"
#include "memwater/seqStruct.h"
#include "memwater/alnSetStruct.h"

/*No .c files (only a .h file)*/
#include "generalLib/base10StrToNum.h"
#include "generalLib/dataTypeShortHand.h"
#include "generalLib/numToStr.h"
#include "generalLib/ulCpStr.h"
#include "tbSpoligoSource/tbSpoligo-errors.h" 

/*Version numbers (not using print version number)*/
#include "freezeTb-version.h"
#include "tbAmrSource/tbAmr-version.h"
#include "tbConSource/tbCon-version.h"
#include "trimSamSource/trimSam-version.h"
#include "ampDepthSource/ampDepth-version.h"
#include "tbMiruSource/tbMiru-version.h"
#include "tbSpoligoSource/tbSpoligo-version.h"
#include "primMaskSrc/primMask-version.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c #include "tbAmrSource/drug_str_ary.h"
!   o .h #include "generalLib/genMath.h"
!   o .h #include "generalLib/codonTbl.h"
!   o .c #include "../memwater/memwater.h"
!   o .h #include "../memwater/alnSeqDefaults.h"
!   o .h #include "../generalLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^    - Defined variables/default settings unique to
^      freezeTb
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_freezeTb_minMedianQ 7
#define def_freezeTb_minMeanQ 7

#define def_graphBl_freezeTb 0 /*1 is make graphs*/

/*Return values from getting user input*/
#define def_nonNumeric_freezeTb 1
#define def_pHelp_freezeTb 2
#define def_pVersion_freezeTb 4
#define def_conHelp_freezeTb 8
#define def_printHelp_freezeTb 16

char *def_prefix_freezeTb = "Hufflepuff";
   /*This is here to tweak Tara's nose a bit.
   `   She is an Harry Potter Ravenclaw fan. I figure
   `   Ravenclaw is the worst thing other then Slytherin,
   `   which is evil.
   */
char *def_extGraph_freezeTb = "tiff";

/*Setting up some default paths. I will use getenv in
`   the main function to get the full file paths.
` NOTE: These paths should always in an \\ or /
*/
#ifdef WINDOWS
   char *defPathStr = "\\Documents\\freezeTBFiles\\";
   char *defAltPathStr = "\\Documents\\freezeTBFiles\\";
#else
   char *defPathStr = "/share/freezeTBFiles/";
   char *defAltPathStr = "/Documents/freezeTBFiles/";
#endif

char *defAmrDbStr = "who-2023.tsv";
const char *defMiruTblStr = "miruTbl.tsv";
char *defGeneCoordStr = "gene-tbl.tsv";
char *defSpoligoRefsStr = "spoligotype-seq.fa";
char *defSpoligoDbStr = "spoligo-lineages.csv";
char *defMaskPrimStr = "mask.tsv";

char *def_graphFlag_freezeTb = "tb";

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-03:
^   - Function headers
^   o fun-01: getInput_freezeTb
^   o fun-02: pHelp_freezeTb
^   o fun-03: pVersion_freezeTb
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*-------------------------------------------------------\
| Fun-01: getInput_freezeTb
|   - Gets the user input from the arguments array for
|     freezeTb
| Input:
|   User Input:
|     - numArgsI:
|       o Number of arguments the user input
|     - argsStrAry:
|       o Pointer to array of c-strings with the users
|         input
|   File input:
|     - samFileStr:
|       o Pointer to c-string to hold the input sam file
|       o This is modified to point to user input
|       o C-string (char array) to hold the path to the
|         AMR database (for tbAmr)
|         gene coordiantes paf file
|       o This is filled in
|     - coordFileStr:
|       o C-string (char array) to hold the path to the
|         gene coordiantes paf file
|       o This is filled in
|     - miruDbFileStr:
|       o C-string (char array) to hold the path to the
|         MIRU table (database) for lineages
|       o This is filled in
|     - spoligoRefFileStr:
|       o C-string (char array) to hold the path to the
|         fasta file with the spoligotyping internal
|         spaces sequences
|       o This is filled in
|     - spoligoDbFileStr:
|       o C-string (char array) to hold the path to the
|         csv file with the spoligotyping lineages
|       o This is filled in
|     - primMaskFileStr:
|       o C-string (char array) to hold the path to the
|         primer masking tsv file
|       o This is filled in
|     - prefixStr:
|       o Pointer to c-string to hold the users prefix
|       o This is modified to point to user input
|   Read filtering:
|     - minMedianQF:
|       o Pointer to a float to hold the min median Q
|         score
|     - minMeanQF:
|       o Pointer to a float to hold the min mean Q
|         score
|     - minPercMapF:
|       o Min percentage of reads to keep an AMR
|     - settings:
|       o Pointer to a tbCon structure with the tbCon
|         settings for consensus buliding/read filtering
|   Graphing:
|     - graphFlagStr:
|       o C-string with value to add for the run name to
|         the read coverage (ampDepth) tsv file
|     - mkGraphBl:
|       o Pointer to character to set to 1 if the user
|         requested graphs
|     - graphFileTypeStr:
|       o Pointer to C-string that is set to point to the
|         file extension for the R graphs
|   Lineages:
|     - frameshiftBl:
|       o 1: users wants frame shift checking on consensus
|     - fudgeLenI:
|       o Pointer to integer to hold the fudge length
|         for assiging a read to an MIRU lineage
|     - spoligoPercScoreF:
|       o Pointer to an float to hold the mininum percent
|         score to count an spoligotype
|     - drStartSI:
|       o Pointer to an int to hold the first base in the
|         direct repeat region (spoligotyping)
|     - drEndSI:
|       o Pointer to an int to hold the last base in the
|         direct repeat region (spoligotyping)
| Output:
|   - Modifies:
|     o Each input variable the user had an agrument for
|   - Returns:
|     o 0 for success
|     o (iArg << 8) | def_tbCon_input_nonNumErr for
|       non-numeric input. Use iArg >> 8 to get the
|       problematic index.
|     o def_pHelp_freezeTb if printing the help message
|     o def_conHelp_freezeTb if printing the consensus
|       help message
|     o def_printHelp_freezeTb if printing the print AMR
|       and variant help message
|     o def_pVersion_freezeTb if printing the version
|     o iArg << 8 for invalid entries. Use iArg >> 8 to
|       get the problematic entry
\-------------------------------------------------------*/
int
getInput_freezeTb(
   int numArgsI,
   char *argsStrAry[],

   /*File input*/
   char **samFileStr,
   char *amrDbFileStr,
   char *coordFileStr,
   char *miruDbFileStr,
   char *spoligoRefFileStr,/*Spoligotype seqeunces*/
   char *spoligoDbFileStr, /*Lineage database*/
   schar *primMaskFileStr,
   char **prefixStr,

   /*Read filtering*/
   float *minMedianQF,
   float *minMeanQF,
   float *minPercMapF,
   struct tbConSet *settings,

   /*graphing*/
   char **graphFlagStr,
   char *mkGraphBl,
   char **graphFileTypeStr,

   /*Lineages*/
   char *frameshiftBl,
   int *fudgeLenI,
   float *spoligoPercScoreF,
   signed int *drStartSI,
   signed int *drEndSI
); /*getInput_freezeTb*/

/*-------------------------------------------------------\
| Fun-02: pHelp_freezeTb
|   - Prints the help message for freezeTb
| Input:
|   - outFILE:
|     o File to print the help message to
| Ouput:
|   - Prints:
|     o The help message to outFILE
\-------------------------------------------------------*/
void
pHelp_freezeTb(
   char pConBl,   /*Print the consensus unqiue settings*/
   char pPrintBl, /*Print the print (amr/con) settings*/
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-03: pVersion_freezeTb
|   - Prints the version number for freezeTb and its
|     submodules
| Input:
|   - outFILE:
|     o File to print the version numbers to
| Ouput:
|   - Prints:
|     o The version numbers to outFILE
\-------------------------------------------------------*/
void
pVersion_freezeTb(
   void *outFILE
);

/*-------------------------------------------------------\
| Main:
|    - Analyze ONT sequenced TB reads
| Input:
|    - numArgsI:
|      o Integer with number of argument the user input
|    - argsStrAry:
|      o C-string array with the input arguments
| Output:
|    - 
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   int numArgsI,
   char *argsStrAry[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - Run freezeTB on user input
   '   o main sec-01:
   '     - Variable declerations
   '   o main sec-02:
   '     -  Set up program default values not assigned
   '   o main sec-03:
   '     - Get and check user input
   '   o main sec-04:
   '     - Check files and set up the output file names
   '   o main sec-06:
   '     - Get reference name and length + get past header
   '   o main sec-07:
   '     - Read in the masking primer coordinates
   '   o main sec-08:
   '     - Do read analysis
   '   o main sec-09:
   '     - Print out the data collected for the reads
   '   o main sec-10:
   '     - Collapse consensus and do consensus analysis
   '   o main sec-11:
   '     - Run R to build graphs
   '   o main sec-12:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - Variable declerations
   ^   o main sec-01 sub-01:
   ^     - General IO variables (applies to multple subs)
   ^   o main sec-01 sub-02:
   ^     - Temporay and error reporting variables
   ^   o main sec-01 sub-03:
   ^     - Filtering and sam file variables
   ^   o main sec-01 sub-04:
   ^     - Read depth and coverage stats variables
   ^   o main sec-01 sub-05:
   ^     - AMR detection variables
   ^   o main sec-01 sub-06:
   ^     - Miru lineage unique variables
   ^   o main sec-01 sub-07:
   ^     - Spoligotyping unique variables
   ^   o main sec-01 sub-08:
   ^     - Consensus building unique variables
   ^   o main sec-01 sub-09:
   ^     - Masking unique variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-01 Sub-01:
   *   - General IO variables (applies to multple subs)
   \*****************************************************/

   char *prefixStr = def_prefix_freezeTb;


   #ifdef WINDOWS
      char *sharePathStr = getenv("PUBLIC");
      char *homePathStr = getenv("HOMEPATH");
   #else
      char *sharePathStr = "/usr/local";
      char *homePathStr = getenv("HOME");
   #endif

   FILE *outFILE = 0;

   /*****************************************************\
   * Main Sec-01 Sub-02:
   *   - Temporay and error reporting variables
   \*****************************************************/

   char spareStr[1024]; /*When I need an extra buffer*/
   char *tmpStr = 0;

   char errC = 0;
   short errS = 0;
   int errI = 0;
   slong errSL = 0;

   /*****************************************************\
   * Main Sec-01 Sub-03:
   *   - Filtering and sam file variables
   \*****************************************************/

   char *samFileStr = 0;

   /*For sam file processing*/
   struct samEntry samStackST;
   char *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   FILE *samFILE = 0;

   char multiRefBl = 0; /*1: means multiple refs/exit*/
   int lenRefI = 0;
   ulong tabUL = ulCpMakeDelim('\t'); /*Only used in ref*/
   char refIdStr[256];

   /*For filtering reads*/
   float minMedianQF = def_freezeTb_minMedianQ;
   float minMeanQF = def_freezeTb_minMeanQ;

   /*****************************************************\
   * Main Sec-01 Sub-04:
   *   - Read depth and coverage stats variables
   \*****************************************************/

   char coordFileStr[256]; /*Input*/
   char readStatsStr[256]; /*Output file name*/

   /*Graphing output*/
   char *graphFlagStr = def_graphFlag_freezeTb;
   char *graphFileTypeStr = def_extGraph_freezeTb;
   char mkGraphBl = def_graphBl_freezeTb;

   struct geneCoords *coordsST = 0; /*From function*/
   int numCoordsI = 0;

   int *readMapIAry = 0;
   int *filt_readMapIAry = 0;
   int offTargReadsI = 0;
   int oldOfTargI = 0;
   int umapReadCntI = 0;

   /*****************************************************\
   * Main Sec-01 Sub-05:
   *   - AMR detection variables
   \*****************************************************/

   char idFileStr[256]; /*For read id printing*/
   char conAmrStr[256];
   char amrDbFileStr[256]; 
   char readAmrStr[256];

   FILE *idFILE = 0; /*For read id printing*/
   char frameshiftBl = def_checkFrameshift_checkAmr;
   struct amrStruct *amrSTAry = 0;
   struct amrHit *amrHitSTList = 0;
   int numAmrI = 0;
   int numHitsI = 0;

   char *drugStrAry = 0;
   int numDrugsI = 0;
   int maxDrugsI = 0;
   uint totalReadsUI = 0;

   float minPercMapF = defMinPerReadsMap;
       /*Min percenatge of mapped reads to keep an AMR
       `  when checking against reads
       */

   /*For the miru lineages*/
   struct miruTbl *miruST = 0;
   int fudgeLenI = def_fudgeLen_miruTblST;

   /*****************************************************\
   * Main Sec-01 Sub-06:
   *   - Miru lineage unique variables
   \*****************************************************/

   /*Input or output files for MIRU lineages*/
   char miruTblFileStr[256]; /*Default flie path*/
   char *miruDbFileStr = miruTblFileStr; /*file path*/
   char readMiruStr[256];
   char conMiruStr[256];

   /*****************************************************\
   * Main Sec-01 Sub-07:
   *   - Spoligotyping unique variables
   \*****************************************************/

   char spoligoRefFileStr[256];
   char spoligoDbFileStr[256];
   schar checkSpoligoLinBl = 1;
   char barcodeStr[1024];
   char outSpoligoFileStr[256];

   FILE *spoligoOutFILE = 0;

   float spoligoPercScoreF =
      def_minPercScore_tbSpoligoWater;

   signed int drStartSI =  def_DRStart_tbSpoligo;
   signed int drEndSI =  def_DREnd_tbSpoligo;
   struct alnSet alnSetStackST; /*Alingment settings*/

   struct spoligoST *lineageHeapAryST = 0;
   sint numLineagesSI = 0;

   struct seqStruct *spoligoHeapAryST = 0;
   sint numSpoligosSI = 0;

   /*****************************************************\
   * Main Sec-01 Sub-08:
   *   - Consensus building unique variables
   \*****************************************************/

   char samConStr[256];
   char conTsvStr[256];

   struct tbConSet tbConSettings;
   struct conBase *conBaseSTAry = 0;
   FILE *samConFILE = 0;

   struct samEntry *samConSTAry = 0;
   int numFragmentsI = 0;
   int iCon = 0; /*Iterator for consensus step*/

   /*****************************************************\
   * Main Sec-01 Sub-09:
   *   - Masking unique variables
   \*****************************************************/

   schar primMaskFileStr[256];
   uint *maskStartHeapAryUI = 0;
   uint *maskEndHeapAryUI = 0;
   uint *maskFlagHeapAryUI = 0;
   uint maskNumPrimUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   -  Set up the default values not already set up
   ^   o main sec-02 sub-01:
   ^     - Initialize the structures
   ^   o main sec-02 sub-02:
   ^     - Set the path to the amr database
   ^   o main sec-02 sub-03:
   ^     - Set the path to the MIRU lineages
   ^   o main sec-02 sub-04:
   ^     - Set the path to the gene coordinates file
   ^   o main sec-02 sub-05:
   ^     - Set the path to the spoligo reference file
   ^   o main sec-02 sub-06:
   ^     - Set the path to the spoligo lineages database
   ^   o main sec-02 sub-07:
   ^     - Set primer masking file path
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-02 Sub-01:
   *   - Initialzie the structures
   \*****************************************************/

   initTbConSet(&tbConSettings);
   initAlnSet(&alnSetStackST);
   refIdStr[0] = '\0';
   primMaskFileStr[0] = '\0';
   initSamEntry(&samStackST);

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Set the path to the amr database
   \*****************************************************/

   tmpStr = amrDbFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defAmrDbStr, 0, 0);
   outFILE = fopen(amrDbFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = amrDbFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr, defAmrDbStr, 0, 0);
      outFILE = fopen(amrDbFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = amrDbFileStr;
         tmpStr += ulCpStrDelim(tmpStr,defAmrDbStr,0,0);
         outFILE = fopen(amrDbFileStr, "r");

         if(! outFILE)
            amrDbFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE) fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Set the path to the MIRU lineages
   \*****************************************************/

   tmpStr = miruTblFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defMiruTblStr, 0, 0);
   outFILE = fopen(miruTblFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = miruTblFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr, defMiruTblStr, 0, 0);
      outFILE = fopen(miruTblFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = miruTblFileStr;
         tmpStr += ulCpStrDelim(tmpStr,defMiruTblStr,0,0);
         outFILE = fopen(miruTblFileStr, "r");

         if(! outFILE)
            miruTblFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE) fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-04:
   *   - Set the path to the gene coordinates file
   \*****************************************************/

   tmpStr = coordFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defGeneCoordStr, 0, 0);
   outFILE = fopen(coordFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = coordFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defGeneCoordStr,0, 0);
      outFILE = fopen(coordFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = coordFileStr;
         tmpStr+=ulCpStrDelim(tmpStr,defGeneCoordStr,0,0);
         outFILE = fopen(coordFileStr, "r");

         if(! outFILE)
            coordFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE) fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-05:
   *   - Set the path to the spoligo reference file
   \*****************************************************/

   tmpStr = spoligoRefFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defSpoligoRefsStr, 0, 0);

   outFILE = fopen(spoligoRefFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = spoligoRefFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defSpoligoRefsStr,0,0);

      outFILE = fopen(spoligoRefFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = spoligoRefFileStr;

         tmpStr +=
            ulCpStrDelim(
               tmpStr,
               defSpoligoRefsStr,
               0,
               0
            ); /*Fasta with spoligotype reference seqs*/

         outFILE = fopen(spoligoRefFileStr, "r");

         if(! outFILE)
            spoligoRefFileStr[0] = '\0'; /*No idea*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE) fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-06:
   *   - Set the path to the spoligo iinages database
   \*****************************************************/

   tmpStr = spoligoDbFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');

   tmpStr +=
       ulCpStrDelim(
          tmpStr,
          defSpoligoDbStr,
          0,
          0
      );

   outFILE = fopen(spoligoDbFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = spoligoDbFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');


      tmpStr +=
          ulCpStrDelim(
             tmpStr,
             defSpoligoDbStr,
             0,
             0
         );

      outFILE = fopen(spoligoDbFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = spoligoRefFileStr;

         tmpStr +=
             ulCpStrDelim(
                tmpStr,
                defSpoligoDbStr,
                0,
                0
            );

         outFILE = fopen(spoligoDbFileStr, "r");

         if(! outFILE)
            spoligoDbFileStr[0] = '\0'; /*No idea*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE) fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-02 Sub-07:
   *   - Set primer masking file path
   \*****************************************************/

   tmpStr = (char *) primMaskFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defMaskPrimStr, 0, 0);
   outFILE = fopen((char *) primMaskFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the database*/
      tmpStr = (char *) primMaskFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defMaskPrimStr,0,0);
      outFILE = fopen((char *) primMaskFileStr, "r");

      if(! outFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = (char *) primMaskFileStr;
         tmpStr+= ulCpStrDelim(tmpStr,defMaskPrimStr,0,0);
         outFILE = fopen((char *) primMaskFileStr, "r");

         if(! outFILE)
            primMaskFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(outFILE)
      fclose(outFILE);

   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Get and check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errI =
      getInput_freezeTb(
         numArgsI,
         argsStrAry,
         &samFileStr,      /*reads*/
         amrDbFileStr,     /*AMR database tbAmr*/
         coordFileStr,     /*Gene coordianges ampDepth*/
         miruDbFileStr,    /*Miru database file*/
         spoligoRefFileStr,/*Spoligotype seqeunces*/
         spoligoDbFileStr, /*Lineage database*/
         primMaskFileStr,  /*Masking file for primers*/
         &prefixStr,       /*output file names*/
         &minMedianQF,     /*median Q-score to filter*/
         &minMeanQF,       /*mean Q-score to filter with*/
         &minPercMapF,     /*pitch AMRs beneath; tbAMR*/
         &tbConSettings,   /*Consensus/variants settings*/
         &graphFlagStr,    /*ampDepth column 1*/
         &mkGraphBl,       /*Make a graph*/
         &graphFileTypeStr,/*graph extension*/
         &frameshiftBl,    /*AMR frame shift checking*/
         &fudgeLenI,       /*Fudge size for MIRU tables*/
         &spoligoPercScoreF, /*% score to count map*/
         &drStartSI,       /*Start of direct repeat*/
         &drEndSI          /*End of direct repeat*/
   ); /*getInput_freezeTb*/

   if(errI)
   { /*If: I had an error*/
      if(errI & def_conHelp_freezeTb)
      { /*If: I need to print the consensus help message*/
         pHelp_freezeTb(1, 0, stdout);
         exit(0);
      } /*If: I need to print the consensus help message*/

      if(errI & def_printHelp_freezeTb)
      { /*If: I need to print the consensus help message*/
         pHelp_freezeTb(0, 1, stdout);
         exit(0);
      } /*If: I need to print the consensus help message*/

      if(errI & def_pHelp_freezeTb)
      { /*If: I need to print the consensus help message*/
         pHelp_freezeTb(0, 0, stdout);
         exit(0);
      } /*If: I need to print the consensus help message*/

      if(errI & def_pVersion_freezeTb)
      { /*If: I need to print the consensus help message*/
         pVersion_freezeTb(stdout);
         exit(0);
      } /*If: I need to print the consensus help message*/

      if(errI & def_nonNumeric_freezeTb)
      { /*If: I need to print the consensus help message*/
         errI >>= 8;

         fprintf(
            stderr,
            "%s %s is a non-numeric value\n",
            argsStrAry[errI],
            argsStrAry[errI + 1]
         ); /*If there was a non-numeric entry*/;

         exit(-1);
      } /*If: I need to print the consensus help message*/

      errI >>= 8;

      pHelp_freezeTb(1, 1, stderr);

      fprintf(
         stderr,
         "%s is not a recongnized value\n",
         argsStrAry[errI]
      ); /*If there was a non-numeric entry*/;

      exit(-1);
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Check files and set up the output file names
   ^   o main sec-04 sub-01:
   ^     - Output file for read stats
   ^   o main sec-04 sub-02:
   ^     - Set up the name for tsv consensus output file
   ^   o main sec-04 sub-03:
   ^     - output file for the AMRs found in reads
   ^   o main sec-04 sub-04:
   ^     - Set up read id AMR hit table
   ^   o main sec-04 sub-05:
   ^     - output file for the AMRs found in consensus
   ^   o main sec-04 sub-06:
   ^     - output file for MIRU lineages (reads)
   ^   o main sec-04 sub-07:
   ^     - output file for MIRU lineages (consensus)
   ^   o main sec-04 sub-08:
   ^     - Build spoligotyping output file name
   ^   o main sec-04 sub-09:
   ^     - Check if the MIRU table exists
   ^   o main sec-04 sub-10:
   ^     - Check if spoligotyping lineage database
   ^   o main sec-04 sub-11:
   ^     - Check if spoligotyping lineage database
   ^   o main sec-04 sub-12:
   ^     - Check if amr table exists
   ^   o main sec-04 sub-13:
   ^     - Open the sam file
   ^   o main sec-04 sub-14:
   ^     - Set up name for and open consensus output file
   ^   o main sec-04 sub-15:
   ^     - Open the paf (gene coordinates) file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-04 Sub-01:
   *   - Set up the name for the read stats file
   \*****************************************************/

   tmpStr = readStatsStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, "-depths.tsv", 0, '\0');

   outFILE = fopen(readStatsStr, "w");

   if(! outFILE)
   { /*If: I could not open the read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readStatsStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the read stats file*/
 
   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-03:
   *   - Set up the name for the tsv consensus output file
   \*****************************************************/

   tmpStr = conTsvStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-consensuses.tsv", 0, '\0');

   outFILE = fopen(conTsvStr, "w");

   if(! outFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conTsvStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-03:
   *   - Set up the name for the read AMRs table
   \*****************************************************/

   tmpStr = readAmrStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-read-amrs.tsv", 0, '\0');

   outFILE = fopen(readAmrStr, "w");

   if(! outFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readAmrStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-04:
   *   - Set up read id AMR hit table
   \*****************************************************/

   tmpStr = idFileStr;

   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-id-amrs.tsv", 0, '\0');

   outFILE = fopen(idFileStr, "w");

   if(! outFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         idFileStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-05:
   *   - Set up the name for the consensus AMRs table
   \*****************************************************/

   tmpStr = conAmrStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-con-amrs.tsv", 0, '\0');

   outFILE = fopen(conAmrStr, "w");

   if(! outFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conAmrStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-06:
   *   - Set up the name for the MIRU reads table
   \*****************************************************/

   tmpStr = readMiruStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-miru-read-tbl.tsv", 0, '\0');

   outFILE = fopen(readMiruStr, "w");

   if(errC)
   { /*If: I could not open the file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         readMiruStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-07:
   *   - Set up the name for the MIRU consensus table
   \*****************************************************/

   tmpStr = conMiruStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-con-miru.tsv", 0, '\0');

   outFILE = fopen(conMiruStr, "w");

   if(errC)
   { /*If: I could not open the file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         conMiruStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-08:
   *   - Build spoligotyping output file name
   \*****************************************************/

   tmpStr = outSpoligoFileStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-con-spoligo.tsv", 0, '\0');

   outFILE = fopen(outSpoligoFileStr, "w");

   if(! outFILE)
   { /*If: I could not open the spoligo output file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         outSpoligoFileStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the spoligo output file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-09:
   *   - Check if the MIRU table exists
   \*****************************************************/

   outFILE = fopen(miruDbFileStr, "r");

   if(! outFILE)
   { /*If I could not open the MIRU table*/
      fprintf(
         stderr,
         "unable to open -miru-tbl %s\n",
         miruDbFileStr
      );

      goto err_main_sec12_sub02;
   } /*If I could not open the MIRU table*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-10:
   *   - Check if spoligotyping reference sequences exists
   \*****************************************************/

   outFILE = fopen(spoligoRefFileStr, "r");

   if(! outFILE)
   { /*If I could not open the spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -spoligo %s\n",
         spoligoRefFileStr
      );

      goto err_main_sec12_sub02;
   } /*If I could not open the spoligo spacer sequences*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-11:
   *   - Check if spoligotyping lineage database
   \*****************************************************/

   outFILE = fopen(spoligoDbFileStr, "r");

   if(! outFILE)
   { /*If I could not open the spoligo spacer sequences*/
      fprintf(
         stderr,
         "unable to open -db-spoligo %s\n",
         spoligoDbFileStr
      );

      fprintf(
         stderr,
         "Spoligotype lineages will be ignored\n"
      );

      checkSpoligoLinBl = 0;
   } /*If I could not open the spoligo spacer sequences*/

   else
   { /*Else: have valid spoligotype lineage database*/
      checkSpoligoLinBl = 0;
      fclose(outFILE);
   } /*Else: have valid spoligotype lineage database*/

   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-12:
   *   - Check if amr table exists
   \*****************************************************/

   
   outFILE = fopen(amrDbFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open -amr-tbl %s\n",
         amrDbFileStr
      );

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-04 Sub-13:
   *   - Open the sam file
   \*****************************************************/
   
   if(! samFileStr || *samFileStr == '-') samFILE = stdin;

   else
   { /*Else: I need to open the sam file*/
      samFILE = fopen(samFileStr, "r");

      if(! samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
            stderr,
            "Could not open -sam %s\n",
            samFileStr 
         );

         exit(-1);
      } /*If: I could not open the sam file*/
   } /*Else: I need to open the sam file*/

   /*****************************************************\
   * Main Sec-04 Sub-14:
   *   - Set up name for and open consensus output file
   \*****************************************************/

   tmpStr = samConStr;
   tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');

   tmpStr +=
      ulCpStrDelim(tmpStr, "-consensuses.sam", 0, '\0');

   samConFILE = fopen(samConStr, "w");

   if(! samConFILE)
   { /*If: I could not open the filtered read stats file*/
      fprintf(
         stderr,
         "unable to open %s for output\n",
         samConStr
      );

      if(samFILE != stdin) fclose(samFILE);

      goto err_main_sec12_sub02;
   } /*If: I could not open the filtered read stats file*/

   /*****************************************************\
   * Main Sec-04 Sub-15:
   *   - Open the paf (gene coordinates) file
   \*****************************************************/
   
   outFILE = fopen(coordFileStr, "r");

   if(! outFILE)
   { /*If: I could not open the gene coordinates file*/
      fprintf(
         stderr,
         "Could not open gene coordiantes flie (-paf %s)",
         samFileStr 
      );

      fprintf(stderr, "\n");

      if(samFILE != stdin) fclose(samFILE);

      fclose(samConFILE);

      goto err_main_sec12_sub02;
   } /*If: I could not open the gene coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Read in the supporting files
   ^   o main sec-05 sub-01:
   ^     - Read in the gene mapping coodiantes
   ^   o main sec-05 sub-02:
   ^     - Read in the amr table
   ^   o main sec-05 sub-03:
   ^     - Read in the MIRU lineage table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-05 Sub-01:
   *   - Read in the gene mapping coodiantes
   \*****************************************************/

   coordsST =
      getGeneCoords(coordFileStr,&numCoordsI,&lenBuffUL);

   if(! coordsST)
   { /*If: I had a memory error*/
      if(samFILE != stdin) fclose(samFILE);

      fclose(samConFILE);

      goto err_main_sec12_sub02;
   } /*If: I had a memory error*/

   lenBuffUL = 0;

   /*****************************************************\
   * Main Sec-05 Sub-02:
   *   - Read in the amr table
   \*****************************************************/

   amrSTAry =
      readTbAmrTbl(
         amrDbFileStr,
         (uint *) &numAmrI,
         &drugStrAry,
         &numDrugsI,
         &maxDrugsI,
         &errC
      ); /*Read in the amr database*/

   if(errC)
   { /*If: I had an error of some kind*/
      if(errC == def_amrST_invalidFILE)
         fprintf(
           stderr,
           "%s is not in tbAmr format or has no data\n",
           amrDbFileStr
         );

      if(errC == def_amrST_memError)
         fprintf(
           stderr,
           "(mem) Err; when processing variant id's\n"
         );

      goto err_main_sec12_sub02;
   } /*If: I had an error of some kind*/

   /*****************************************************\
   * Main Sec-05 Sub-03:
   *   - Read in the MIRU lineage table
   \*****************************************************/
 
   miruST = readMiruTbl(miruDbFileStr, &errC);

   if(errC)
   { /*If: I had an error with the MIRU database*/
      if(errC == def_fileErr_miruTblST)
         fprintf(
            stderr,
            "Could not open -miru -tbl %s\n",
            miruDbFileStr
          ); /*Let the user know the error type*/

      else if(errC == def_memErr_miruTblST)
         fprintf(stderr, "Ran out of memory\n");

      else
         fprintf(
            stderr,
            "Error while reading -miru-tbl %s\n",
               miruDbFileStr
         ); /*Let the user know the error*/

      goto err_main_sec12_sub02;
   } /*If: I had an error with the MIRU database*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-06:
   ^   - Get reference name and length + get past header
   ^   o main sec-06 sub-01:
   ^     - Get the reference name/length from header
   ^   o main sec-06 sub-02:
   ^     - print tbCon cosensus (same file) settings
   ^   o main sec-06 sub-03:
   ^     - print tbCon tsv file settings
   ^   o main sec-06 sub-04:
   ^     - Check if I could extract reference name/length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-06 Sub-01:
   *   - Get the reference length from the header
   \*****************************************************/

   errS =
      readSamLine(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errS)
   { /*Loop: read in the header*/

      /*Check if I have a comment*/
      if(*samStackST.extraStr != '@') break;

      fprintf(samConFILE, "%s\n", samStackST.extraStr);

      /*This is not perfect because it assumes the
      `  sam file @SQ entry is in the order of
      `  @SQ\tSN:reference-name\tLN:reference-length
      */

      if(! cStrEql("@SQ\t", samStackST.extraStr, '\t'))
      { /*If: this has length information*/
         if(multiRefBl)
         { /*If: sam file has multiple references*/
            fprintf(
               stderr,
               "-sam %s has multiple references\n",
               samFileStr
            ); /*Let user know the problem*/

            goto err_main_sec12_sub02;
         } /*If: sam file has multiple references*/

         multiRefBl = 1;

         /*Get past "@SQ\t"*/
         tmpStr = samStackST.extraStr + 4; 

         while(*tmpStr++ !=':') if(*tmpStr <31) break;

         /*Check if I have a reference id*/
         if(*(tmpStr - 1) != ':') goto nextHeader_feezeTb;

         tmpStr +=
            ulCpStrDelim(refIdStr,tmpStr,tabUL,'\t');

         ++tmpStr;
        
         if(*tmpStr < 31 ) goto nextHeader_feezeTb;

         /*Move past the LN: flag*/
         while(*tmpStr++ !=':') if(*tmpStr <31) break;

         /*Check if I have a read length*/
         if(*(tmpStr - 1) != ':') goto nextHeader_feezeTb;

         /*Get the reference length*/
         tmpStr = base10StrToUI(tmpStr, lenRefI);

         /*Check if I had a conversion error*/
         if(*tmpStr > 31) lenRefI = defRefLen;
      } /*If: this has length information*/

      nextHeader_feezeTb:;

      errS =
         readSamLine(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next line*/
   } /*Loop: read in the header*/

   /*****************************************************\
   * Main Sec-06 Sub-02:
   *   - print tbCon cosensus (same file) settings
   \*****************************************************/

   fprintf(samConFILE, "@consensus: tbCon");

   /*Check if getting sam file from stdin*/
   if(! samFileStr || *samFileStr == '-')
     fprintf(samConFILE, " -sam -");
   else
     fprintf(samConFILE, " -sam %s", samFileStr);

   fprintf(
      samConFILE,
      " -min-mapq %i -min-q %i -min-q-ins %i",
      tbConSettings.minMapqUC,
      tbConSettings.minQI,
      tbConSettings.minInsQI
   );

   fprintf(
      samConFILE,
      " -min-len %i -min-depth %i -perc-snp-sup %f",
      tbConSettings.minLenI,
      tbConSettings.minDepthI,
      tbConSettings.minPercSnpF
   );

   fprintf(
      samConFILE,
      " -perc-ins-sup %f -min-del-sup %f -out %s\n",
      tbConSettings.minPercInsF,
      tbConSettings.minPercDelF,
      samConStr
   );

   /*****************************************************\
   * Main Sec-06 Sub-03:
   *   - print tbCon tsv file settings
   \*****************************************************/

   fprintf(samConFILE, "@tsv: tbCon");

   /*Check if getting sam file from stdin*/
   if(! samFileStr || *samFileStr == '-')
     fprintf(samConFILE, " -sam -");
   else
     fprintf(samConFILE, " -sam %s", samFileStr);

   fprintf(
      samConFILE,
      " -min-mapq %i -min-q %i -min-q-ins %i",
      tbConSettings.minMapqUC,
      tbConSettings.minQI,
      tbConSettings.minInsQI
   );

   fprintf(
      samConFILE,
      " -p-min-depth %i -p-perc-snp-sup %f",
      tbConSettings.printMinDepthI,
      tbConSettings.printMinSupSnpF
   );

   fprintf(
      samConFILE,
      " -p-perc-ins-sup %f -p-min-del-sup %f",
      tbConSettings.printMinSupInsF,
      tbConSettings.printMinSupDelF
   );

   fprintf(
      samConFILE,
       " -out-tsv %s\n",
       conTsvStr
   );

   /*****************************************************\
   * Main Sec-06 Sub-04:
   *   - Check if I could extract reference name/length
   \*****************************************************/

   /*This is from tbConSource/tbCon-fun.h*/
   if(! lenRefI) lenRefI = defRefLen;

   if(refIdStr[0] == '\0')
   { /*If: I could not get the reference name*/
      refIdStr[0] = 'u';
      refIdStr[1] = 'n';
      refIdStr[2] = 'k';
      refIdStr[3] = 'o';
      refIdStr[4] = 'w';
      refIdStr[5] = 'n';
      refIdStr[6] = '\0';
   } /*If: I could not get the reference name*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-07:
   ^   - Read in the masking primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         primMaskFileStr[0] != '-' 
      && primMaskFileStr[0] != '\0'
   ){ /*If: an primer masking file was input*/
      maskNumPrimUI =
         readPrimCoords(
            primMaskFileStr,
            &maskStartHeapAryUI,
            &maskEndHeapAryUI,
            &maskFlagHeapAryUI,
            &errSL
         );

      if(! maskNumPrimUI)
      { /*If: I had an error*/
         if(errSL & def_emptyFileErr_primMask)
         { /*If: I had an empty file*/
            fprintf(
               stderr,
               "-mask-prim %s is emtpy\n",
               primMaskFileStr
            );
         } /*If: I had an empty file*/

         else if(errSL & def_emptyFileErr_primMask)
         { /*Else If: I had an invalid line*/
            fprintf(
               stderr,
               "line %lu in -mask-prim %s is invalid\n",
               (errSL >> 8),
               primMaskFileStr
            );
         } /*Else If: I had an invalid line*/
         
         else
         { /*Else: I had an memory error*/
            fprintf(
               stderr,
               "Memory error reading -mask-prim %s\n",
               primMaskFileStr
            );
         } /*Else: I had an memory error*/

         goto err_main_sec12_sub02;
      } /*If: I had an error*/
   } /*If: an primer masking file was input*/

   else
      maskNumPrimUI = 0; /*Just making sure*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-08:
   ^   - Do read analysis
   ^   o main sec-06 sub-01:
   ^     - Allocate memory for the read stats arrays
   ^   o main sec-06 sub-02:
   ^     - Filter sam entries
   ^   o main sec-06 sub-03:
   ^     - Mask primers in reads
   ^   o main sec-06 sub-04:
   ^     - Build the filtered histogram
   ^   o main sec-06 sub-05:
   ^     - Build the consensus
   ^   o main sec-06 sub-06:
   ^     - Check for AMRs
   ^   o main sec-06 sub-07:
   ^     - Check for MIRU lineages
   ^   o main sec-06 sub-08:
   ^     - Move to the next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-08 Sub-01:
   *   - Allocate memory for the read stats arrays
   \*****************************************************/

   readMapIAry = calloc(lenRefI, sizeof(int));

   if(! readMapIAry)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory\n");
      goto err_main_sec12_sub02;
   } /*If: I had an memory error*/

   filt_readMapIAry = calloc(lenRefI, sizeof(int));

   if(! filt_readMapIAry)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory\n");
      goto err_main_sec12_sub02;
   } /*If: I had an memory error*/

   /*****************************************************\
   * Main Sec-08 Sub-02:
   *   - filter the reads
   \*****************************************************/

   idFILE = fopen(idFileStr, "w");

   while(! errS)
   { /*Loop: Process each read in the sam file*/
      if(samStackST.flagUS & (4 | 256 | 2048))
      { /*If:umapped 4, secondary 256, or suplemtal 2048*/
         /*If this was an unmapped read*/
         if(samStackST.flagUS & 4) ++umapReadCntI;

         goto nextSamRead_freezeTb;
      } /*If:umapped 4, secondary 256, or suplemtal 2048*/

      /*Remove soft masked bases*/
      trimSamEntry(&samStackST);

      oldOfTargI = offTargReadsI;

      /*Build up the histogram of stats*/
      addBaseToAmpDepth(
         &samStackST,
         coordsST,
         numCoordsI,
         readMapIAry,
         offTargReadsI
      );

      /*If this read was off target*/
      if(oldOfTargI < offTargReadsI)
         goto nextSamRead_freezeTb;

      /*Check the Q-scores*/
      if(samStackST.medianQF < minMedianQF)
         goto nextSamRead_freezeTb;

      if(samStackST.medianQF < minMedianQF)
         goto nextSamRead_freezeTb;

      /*Check the mapping quality*/
      if(samStackST.mapqUC < tbConSettings.minMapqUC)
         goto nextSamRead_freezeTb;

      /*Check if the read is to short*/
      if(samStackST.alnReadLenUI < tbConSettings.minLenI)
         goto nextSamRead_freezeTb;

      /**************************************************\
      * Main Sec-08 Sub-03:
      *   - Mask primers in reads
      \**************************************************/

      if(maskNumPrimUI)
      { /*If: We are masking primers*/
         errSL =
            maskPrimers(
               &samStackST,
               maskStartHeapAryUI,
               maskEndHeapAryUI,
               maskFlagHeapAryUI,
               maskNumPrimUI,
               -1, /*Mask any found primer sites*/
               'N' /*Add masking*/
            );
      } /*If: We are masking primers*/

      /**************************************************\
      * Main Sec-08 Sub-04:
      *   - Build the filtered histogram
      \**************************************************/

      ++totalReadsUI;

      addBaseToAmpDepth(
         &samStackST,
         coordsST,
         numCoordsI,
         filt_readMapIAry,
         oldOfTargI         /*Never used*/
      );

      /**************************************************\
      * Main Sec-08 Sub-05:
      *   - Build the consensus
      \**************************************************/

      /*Add the read to the consensus*/
      errS =
         addReadToConBaseArray(
            &samStackST,
            &conBaseSTAry,
            (uint *) &lenRefI,
            &tbConSettings
         ); /*Add the read to the consnesus*/
            
      if(errS & def_tbCon_Memory_Err)
      { /*If: memory error for consensus*/
         fprintf(stderr, "Memory error\n");
         goto err_main_sec12_sub02;
      } /*If: memory error for consensus*/

      /**************************************************\
      * Main Sec-08 Sub-06:
      *   - Check for AMRs
      \**************************************************/

      amrHitSTList =
         checkAmrSam(
            &samStackST,
            amrSTAry,
            numAmrI,
            &numHitsI,
            0,
            &errC
         ); /*Check if read has antibiotic resistance*/

      if(errC)
      { /*If: memory error for AMR*/
         fprintf(stderr, "Memory error\n");
         goto err_main_sec12_sub02;
      } /*If: memory error for AMR*/

      /*I only care about this for consensuses*/
      if(amrHitSTList)
      { /*If: I had AMRs*/
         pAmrReadIds(
            samStackST.qryIdStr,
            amrHitSTList,
            idFILE
         );

         freeAmrHitList(amrHitSTList);
      } /*If: I had AMRs*/

      /**************************************************\
      * Main Sec-08 Sub-07:
      *   - Check for MIRU lineages 
      \**************************************************/

      inc_matching_len_lineages(
         &samStackST,
         fudgeLenI,
         miruST
      );

      /**************************************************\
      * Main Sec-08 Sub-08:
      *   - Move to the next read
      \**************************************************/

      nextSamRead_freezeTb:;

      errS =
         readSamLine(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next line*/
   } /*Loop: Process each read in the sam file*/

   freeSamEntryStack(&samStackST);

   if(samFILE != stdin) fclose(samFILE);

   samFILE = 0;

   fclose(idFILE);
   idFILE = 0;

   /*Memory for masking primers*/
   free(maskStartHeapAryUI);
   maskStartHeapAryUI = 0;

   free(maskEndHeapAryUI);
   maskEndHeapAryUI = 0;

   free(maskFlagHeapAryUI);
   maskFlagHeapAryUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-09:
   ^   - Print out the data collected for the reads
   ^   o main sec-09 sub-01:
   ^     - Print out the unfiltered read stats
   ^   o main sec-09 sub-02:
   ^     - Print out the filtered read stats
   ^   o main sec-09 sub-03:
   ^     - Print out the AMR hits for the reads
   ^   o main sec-09 sub-04:
   ^     - Print out read MIRU table
   ^   o main sec-09 sub-05:
   ^     - Print out the tsv file of variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-09 Sub-01:
   *   - Print out the unfiltered read stats
   \*****************************************************/

   /*I already checked if I could open this file*/
   outFILE = fopen(readStatsStr, "w");

   pHeaderAmpDepth(outFILE);

   pAmpDepthHistogram(
      readMapIAry,
      tbConSettings.printMinDepthI,      
      coordsST,
      numCoordsI,
      offTargReadsI,
      umapReadCntI,
      graphFlagStr,
      outFILE
    ); /*Print out the unfilterd read stats*/

   free(readMapIAry);
   readMapIAry = 0;

   /*****************************************************\
   * Main Sec-09 Sub-02:
   *   - Print out the filtered read stats
   \*****************************************************/

   /*Adding a unique column name so I can compare
   `  filtered and unfiltered reads
   */
   tmpStr = spareStr;
   tmpStr += ulCpStrDelim(tmpStr, graphFlagStr, 0, '\0');
   ulCpStrDelim(tmpStr, "-filt", 0, '\0');

   pAmpDepthHistogram(
      filt_readMapIAry,
      tbConSettings.printMinDepthI,      
      coordsST,
      numCoordsI,
      0,
      0,
      spareStr,
      outFILE
    ); /*Print out the filterd read stats*/

   fclose(outFILE);
   outFILE = 0;

   free(filt_readMapIAry);
   filt_readMapIAry = 0;

   freeGeneCoords(coordsST);
   coordsST = 0;

   /*****************************************************\
   * Main Sec-09 Sub-03:
   *   - Print out the AMR hits for the reads
   \*****************************************************/

   /*I already checked if I could open this file*/
   outFILE = fopen(readAmrStr, "w");

   pHeadAmrs(outFILE);

   pAmrs(
      (uint) tbConSettings.minDepthI,
      minPercMapF,
      (float) 0, /*Not really usefull, so leaving out*/
      totalReadsUI,
      amrSTAry,
      (uint) numAmrI,
      drugStrAry,
      outFILE
   ); /*Print out the AMRs for the reads*/
      
   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-09 Sub-04:
   *   - Print out read MIRU table
   \*****************************************************/

   errC = pMiruTbl(miruST, readMiruStr);

   /*this should never happen*/
   if(errC)
   { /*If: I could not open the output file*/
      fprintf(
         stderr,
         "Could not write read MIRU table to %s\n",
         readMiruStr
      ); /*Let the user know the file problem*/
   } /*If: I could not open the output file*/

   /*****************************************************\
   * Main Sec-09 Sub-05:
   *   - Print out the tsv file of variants
   \*****************************************************/

   /*Build the tsv of variants table*/
   errC =
      pConBaseArray( conBaseSTAry,
         lenRefI,
         refIdStr,
         &tbConSettings,
         conTsvStr
      ); /*Print out the variants (not a vcf)*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-10:
   ^   - Collapse consensus and do consensus analysis
   ^   o main sec-10 sub-01:
   ^     - Collapse the consensus
   ^   o main sec-10 sub-02:
   ^     - Read in spoligotyping spacer sequences
   ^   o main sec-10 sub-03:
   ^     - Read in spoligotyping lineage database
   ^   o main sec-10 sub-04:
   ^     - print consensus and detect/print AMRs and
   ^       lineages for consensus
   ^   o main sec-10 sub-05:
   ^     - Print out consensus MIRU lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-10 Sub-01:
   *   - Collapse the consensus
   \*****************************************************/

   samConSTAry =
      (struct samEntry *)
      collapseConBaseArray(
         conBaseSTAry,
         lenRefI,
         &numFragmentsI,
         refIdStr,
         &tbConSettings,
         &errC
      ); /*Collapse the consesus*/

   freeConBaseAry(&conBaseSTAry, lenRefI);

   if(! samConSTAry)
   { /*If: I could not build the consensus*/
      fprintf(stderr, "Could not build consensus\n");
      goto err_main_sec12_sub02;
   } /*If: I could not build the consensus*/
   
   /*****************************************************\
   * Main Sec-10 Sub-02:
   *   - Read in spoligotyping spacer sequences
   \*****************************************************/

   if(checkSpoligoLinBl)
   { /*If: I could open the spoligo lineage database*/
      spoligoHeapAryST =
        getSpoligoRefs(
           (schar *) spoligoRefFileStr,
           &numSpoligosSI,
           (schar *) &errC
        );

      if(errC)
      { /*If: I had an error*/
        if(errC == fileErr_tbSpoligo)
        { /*If: I had an file error*/
           fprintf(
              stderr,
              "-db-spoligo %s is not valid\n",
              spoligoRefFileStr
           );

           fprintf(
              stderr,
              "Skipping spoligotype lineage detection\n"
           );
        } /*If: I had an file error*/

        else
        { /*Else: I had an memory error*/
           fprintf(
              stderr,
              "Ran out of memory getting spoligo seqs\n"
           );

            goto err_main_sec12_sub02;
        } /*Else: I had an memory error*/
      } /*If: I had an error*/
   } /*If: I could open the spoligo lineage database*/

   /*****************************************************\
   * Main Sec-10 Sub-03:
   *   - Read in spoligotyping lineage database
   \*****************************************************/

   lineageHeapAryST =
      readSpoligoDb(
         (schar *) spoligoDbFileStr,
         &numLineagesSI,
         (schar *) &errC
   );

   if(errC)
   { /*If: I had an error*/
      if(errC == fileErr_tbSpoligo)
      { /*If: I had an file error*/
         fprintf(
            stderr,
            "Could not open -db-spoligo %s\n",
            spoligoDbFileStr
         );

         fprintf(stderr, "Skipping spoligotype checks\n");
      } /*If: I had an file error*/

      else
      { /*Else: I had an memory error*/
         fprintf(
            stderr,
            "Ran out of memory reading -db %s\n",
            spoligoDbFileStr
         );

         goto err_main_sec12_sub02;
      } /*Else: I had an memory error*/

   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec-10 Sub-04:
   *   - print consensus and detect/print AMRs and
   *     lineages for consensus
   *   o main sec-10 sub-04 cat-01:
   *     - Open files and run consensus fragment loop
   *   o main sec-10 sub-04 cat-02:
   *     - Consensus printing
   *   o main sec-10 sub-04 cat-03:
   *     - AMR detection and printing
   *   o main sec-10 sub-04 cat-04:
   *     - MIRU-VNTR lineage detection and printing
   *   o main sec-10 sub-04 cat-05:
   *     - Detect spoligotypes
   *   o main sec-10 sub-04 cat-06:
   *     - Close output files and free memory
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-10 Sub-04 Cat-01:
   +   - Open files and run consensus fragment loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   outFILE = fopen(conAmrStr, "w");
   spoligoOutFILE = fopen(outSpoligoFileStr, "w");

   pSpoligoHead(spoligoOutFILE);

   errC = 0;

   /*Remove all the read counters*/
   resetCnt_miruTbl(miruST);

   pHeadAmrHitList(outFILE);

   for(iCon = 0; iCon < numFragmentsI; ++iCon)
   { /*Loop: Process each consensus*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec-10 Sub-04 Cat-02:
     +   - Consensus printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errC)
      { /*If: I had an error*/
         freeSamEntryStack(&samConSTAry[iCon]);
         continue;
      } /*If: I had an error*/

      if(samConSTAry[iCon].qryIdStr[0] == '\0')
      { /*If: This is a blank structure*/
         freeSamEntryStack(&samConSTAry[iCon]);
         continue;
      } /*If: This is a blank structure*/

      pSamEntry(
         &samConSTAry[iCon],
         &buffHeapStr,
         &lenBuffUL,
         samConFILE
      );

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec-10 Sub-04 Cat-03:
     +   - AMR detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      amrHitSTList =
         checkAmrSam(
            &samConSTAry[iCon],
            amrSTAry,
            numAmrI,
            &numHitsI,
            frameshiftBl,
            &errC
         ); /*Check if read has antibiotic resistance*/

      if(errC)
         continue; /*At this piont easier to keep moving*/

      if(amrHitSTList)
      { /*If: I had AMRs*/
         pAmrHitList(
            samConSTAry[iCon].qryIdStr,
            amrHitSTList,
            drugStrAry,
            outFILE
         ); /*Print out the AMRs*/

         freeAmrHitList(amrHitSTList);
      } /*If: I had AMRs*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec-10 Sub-04 Cat-04:
     +   - MIRU-VNTR lineage detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

      inc_matching_len_lineages(
         &samConSTAry[iCon],
         fudgeLenI,
         miruST
      ); /*Get the MIRU lineages in this consensus*/

     /*++++++++++++++++++++++++++++++++++++++++++++++++++\
     + Main Sec-10 Sub-04 Cat-05:
     +   - Spoligotype detection and printing
     \++++++++++++++++++++++++++++++++++++++++++++++++++*/

     errSL =
        samSpoligoSearch(
           &samConSTAry[iCon].qryIdStr,
           spoligoHeapAryST,
           numSpoligosSI,
           drStartSI,
           drEndSI,
           spoligoPercScoreF,
           (schar *) barcodeStr,
           &alnSetStackST
        ); /*Detect spoligotypes*/

     if(errSL == memErr_tbSpoligo)
        goto err_main_sec12_sub02;

     if(! errSL)
     { /*If: I could detect spoligotypes*/
        pSpoligo(
           (schar *) samConSTAry[iCon].qryIdStr,
           (schar *) barcodeStr,
           lineageHeapAryST,
           numLineagesSI,
           spoligoOutFILE
        );
     } /*If: I could detect spoligotypes*/
   } /*Loop: Process each consensus*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec-10 Sub-04 Cat-06:
   +   - Close output files and free memory
   \++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fclose(spoligoOutFILE);
   spoligoOutFILE = 0;

   fclose(outFILE);
   outFILE = 0;

   /*Consensus memory*/

   free(buffHeapStr);
   buffHeapStr = 0;

   fclose(samConFILE);
   samConFILE = 0;

   free(samConSTAry);

   /*AMR memory*/
   freeAmrStructArray(&amrSTAry, numAmrI);
   free(drugStrAry);

   /*Spoligotyping memory*/
   freeAlnSetStack(&alnSetStackST);

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSeqAryST(spoligoHeapAryST, numSpoligosSI);
   spoligoHeapAryST = 0;

   /*****************************************************\
   * Main Sec-10 Sub-05:
   *   - Print out consensus MIRU lineages
   \*****************************************************/

   errC = pLineages(miruST, conMiruStr);

   if(errC)
      fprintf(
         stderr,
         "Could not write consensuses MIRU lineages %s\n",
         conMiruStr
      ); /*Let the user know the file problem*/

   freeMiruTbl(&miruST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-11:
   ^   - Run R to build graphs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(mkGraphBl)
   { /*If: The user wanted graphs*/
      tmpStr = spareStr;

      tmpStr +=
         ulCpStrDelim(
            tmpStr,
            "graphAmpDepth.r -stats ",
            0,
            '\0'
         ); /*Copy the Rscript name*/

      tmpStr += ulCpStrDelim(tmpStr,readStatsStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr," -prefix ",0,'\0');
      tmpStr += ulCpStrDelim(tmpStr, prefixStr, 0, '\0');
      tmpStr += cCpStrDelim(tmpStr, " -ext ", '\0');
      tmpStr += cCpStrDelim(tmpStr,graphFileTypeStr,'\0');
      tmpStr += cCpStrDelim(tmpStr, " -who ", '\0');
      tmpStr += ulCpStrDelim(tmpStr,amrDbFileStr,0,'\0');

      /*Run R*/
      system(spareStr);
   } /*If: The user wanted graphs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-12:
   ^   - Clean up
   ^   o main sec-12 sub-01:
   ^     - Clean up for no errors
   ^   o main sec-12 sub-02:
   ^     - Error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-12 Sub-01:
   *   - Clean up for no errors
   \*****************************************************/

   exit(0);

   /*****************************************************\
   * Main Sec-12 Sub-02:
   *   - Error clean up
   \*****************************************************/

   err_main_sec12_sub02:;

   freeSamEntryStack(&samStackST);

   freeGeneCoords(coordsST);
   coordsST = 0;

   if(numAmrI > 0)
      freeAmrStructArray(&amrSTAry, numAmrI);

   amrSTAry = 0;

   freeMiruTbl(&miruST);
   miruST = 0;

   freeConBaseAry(&conBaseSTAry, lenRefI);
   conBaseSTAry = 0;

   freeAlnSetStack(&alnSetStackST);

   freeSpoligoSTAry(lineageHeapAryST, numLineagesSI);
   lineageHeapAryST = 0;

   freeSeqAryST(spoligoHeapAryST, numSpoligosSI);
   spoligoHeapAryST = 0;

   free(buffHeapStr);
   buffHeapStr = 0;

   free(readMapIAry);
   readMapIAry = 0;

   free(filt_readMapIAry);
   filt_readMapIAry = 0;

   free(drugStrAry);
   drugStrAry = 0;

   free(maskStartHeapAryUI);
   maskStartHeapAryUI = 0;

   free(maskEndHeapAryUI);
   maskEndHeapAryUI = 0;

   free(maskFlagHeapAryUI);
   maskFlagHeapAryUI = 0;

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   if(samConFILE) fclose(samConFILE);

   if(idFILE)
      fclose(idFILE);

   idFILE = 0;

   if(spoligoOutFILE)
      fclose(spoligoOutFILE);

   spoligoOutFILE = 0;

   exit(-1);
} /*main*/

/*-------------------------------------------------------\
| Fun-01: getInput_freezeTb
|   - Gets the user input from the arguments array for
|     freezeTb
| Input:
|   User Input:
|     - numArgsI:
|       o Number of arguments the user input
|     - argsStrAry:
|       o Pointer to array of c-strings with the users
|         input
|   File input:
|     - samFileStr:
|       o Pointer to c-string to hold the input sam file
|       o This is modified to point to user input
|     - amrDbFileStr:
|       o C-string (char array) to hold the path to the
|         AMR database (for tbAmr)
|         gene coordiantes paf file
|       o This is filled in
|     - coordFileStr:
|       o C-string (char array) to hold the path to the
|         gene coordiantes paf file
|       o This is filled in
|     - miruDbFileStr:
|       o C-string (char array) to hold the path to the
|         MIRU table (database) for lineages
|       o This is filled in
|     - spoligoRefFileStr:
|       o C-string (char array) to hold the path to the
|         fasta file with the spoligotyping internal
|         spaces sequences
|       o This is filled in
|     - spoligoDbFileStr:
|       o C-string (char array) to hold the path to the
|         csv file with the spoligotyping lineages
|       o This is filled in
|     - primMaskFileStr:
|       o C-string (char array) to hold the path to the
|         primer masking tsv file
|       o This is filled in
|     - prefixStr:
|       o Pointer to c-string to hold the users prefix
|       o This is modified to point to user input
|   Read filtering:
|     - minMedianQF:
|       o Pointer to a float to hold the min median Q
|         score
|     - minMeanQF:
|       o Pointer to a float to hold the min mean Q
|         score
|     - minPercMapF:
|       o Min percentage of reads to keep an AMR
|     - settings:
|       o Pointer to a tbCon structure with the tbCon
|         settings for consensus buliding/read filtering
|   Graphing:
|     - graphFlagStr:
|       o C-string with value to add for the run name to
|         the read coverage (ampDepth) tsv file
|     - mkGraphBl:
|       o Pointer to character to set to 1 if the user
|         requested graphs
|     - graphFileTypeStr:
|       o Pointer to C-string that is set to point to the
|         file extension for the R graphs
|   Misc:
|     - frameshiftBl:
|       o 1: users wants frame shift checking on consensus
|     - fudgeLenI:
|       o Pointer to integer to hold the fudge length
|         for assiging a read to an MIRU lineage
|     - spoligoPercScoreF:
|       o Pointer to an float to hold the mininum percent
|         score to count an spoligotype
|     - drStartSI:
|       o Pointer to an int to hold the first base in the
|         direct repeat region (spoligotyping)
|     - drEndSI:
|       o Pointer to an int to hold the last base in the
|         direct repeat region (spoligotyping)
| Output:
|   - Modifies:
|     o Each input variable the user had an agrument for
|   - Returns:
|     o 0 for success
|     o (iArg << 8) | def_tbCon_input_nonNumErr for
|       non-numeric input. Use iArg >> 8 to get the
|       problematic index.
|     o def_pHelp_freezeTb if printing the help message
|     o def_conHelp_freezeTb if printing the consensus
|       help message
|     o def_printHelp_freezeTb if printing the print AMR
|       and variant help message
|     o def_pVersion_freezeTb if printing the version
|     o iArg << 8 for invalid entries. Use iArg >> 8 to
|       get the problematic entry
\-------------------------------------------------------*/
int
getInput_freezeTb(
   int numArgsI,
   char *argsStrAry[],

   /*File input*/
   char **samFileStr,
   char *amrDbFileStr,
   char *coordFileStr,
   char *miruDbFileStr,
   char *spoligoRefFileStr,/*Spoligotype seqeunces*/
   char *spoligoDbFileStr, /*Lineage database*/
   schar *primMaskFileStr,
   char **prefixStr,

   /*Read filtering*/
   float *minMedianQF,
   float *minMeanQF,
   float *minPercMapF,
   struct tbConSet *settings,

   /*graphing*/
   char **graphFlagStr,
   char *mkGraphBl,
   char **graphFileTypeStr,

   /*misc*/
   char *frameshiftBl,
   int *fudgeLenI,
   float *spoligoPercScoreF,
   signed int *drStartSI,
   signed int *drEndSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: getInput_freezeTb
   '   - Gets the user commandline input for freezeTb
   '   o fun-01 sec-01:
   '     - Variable declerations
   '   o fun-01 sec-03:
   '     - Get user input
   '   o fun-01 sec-03:
   '     - Return success
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   int iArg = 1;
   char *argStr = 0;
   char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-02:
   ^   - Get user input
   ^   o fun-01 sec-02 sub-01:
   ^     - Start loop/check file input
   ^   o fun-01 sec-02 sub-02:
   ^     - Consensus options
   ^   o fun-01 sec-02 sub-03:
   ^     - Printing settings (for consensus tsv)
   ^   o fun-01 sec-02 sub-04:
   ^     - tbAmr settings
   ^   o fun-01 sec-02 sub-05:
   ^     - Read filterin
   ^   o fun-01 sec-02 sub-06:
   ^     - Lineages and graph output
   ^   o fun-01 sec-02 sub-07:
   ^     - Check for help message requests (normal)
   ^   o fun-01 sec-02 sub-08:
   ^     - Check for cosensus help message requests
   ^   o fun-01 sec-02 sub-09:
   ^     - Check for print help message requests
   ^   o fun-01 sec-02 sub-10:
   ^     - Check for version number requets
   ^   o fun-01 sec-02 sub-11:
   ^     - Handle invalid argument/move to next parameter
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-01 Sec-02 Sub-01:
   *   - Start loop/check file input
   \*****************************************************/

   while(iArg < numArgsI)
   { /*Loop: Read in the user input*/
      argStr = argsStrAry[iArg];


      if(! cStrEql("-sam", argStr, '\0'))
         *samFileStr = argsStrAry[iArg + 1];

      else if(! cStrEql("-graph", argStr, '\0'))
      { /*Else If: the user wanted graphs*/
         *mkGraphBl = 1;
         --iArg;
      } /*Else If: the user wanted graphs*/

      else if(! cStrEql("-graph-ext", argStr, '\0'))
      { /*Else If: the user supplied a graph extension*/
         *mkGraphBl = 1;
         *graphFileTypeStr = argsStrAry[iArg + 1];
      } /*Else If: the user supplied a graph extension*/

      else if(! cStrEql("-amr-tbl", argStr, '\0'))
         ulCpStrDelim(
            amrDbFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-gene-coords", argStr, '\0'))
         ulCpStrDelim(
            coordFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-miru-tbl", argStr, '\0'))
         ulCpStrDelim(
            miruDbFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-spoligo", argStr, '\0'))
         ulCpStrDelim(
            spoligoRefFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-db-spoligo", argStr, '\0'))
         ulCpStrDelim(
            spoligoDbFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-mask-prim", argStr, '\0'))
         ulCpStrDelim(
            (char *) primMaskFileStr,
            argsStrAry[iArg + 1],
            0,
            '\0'
         );

      else if(! cStrEql("-prefix", argStr, '\0'))
         *prefixStr = argsStrAry[iArg + 1];

      /**************************************************\
      * Fun-01 Sec-02 Sub-02:
      *   - Consensus options
      *   o fun-01 sec-02 sub-02 cat-01:
      *     - Consensus Q-score settinngs
      *   o fun-01 sec-02 sub-02 cat-02:
      *     - Consensus min depth/length settings
      *   o fun-01 sec-02 sub-02 cat-03:
      *     - Consensus min base percent support settings
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-01 Sec-02 Sub-02 Cat-01:
      +    - Consensus Q-score settinngs
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(! cStrEql("-min-q", argStr, '\0'))
      { /*If: The user provided the minimum Q-score*/
         argStr = argsStrAry[iArg + 1];
         tmpStr = base10StrToSI(argStr, settings->minQI);

         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum Q-score*/


      else if(! cStrEql("-min-q-ins", argStr, '\0'))
      { /*If: user provided the minimum inserion Q-score*/
         argStr = argsStrAry[iArg + 1];
         tmpStr= base10StrToSI(argStr,settings->minInsQI);

         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: user provided the minimum inserion Q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-01 Sec-02 Sub-02 Cat-02:
      +    - Consensus min depth/length settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(! cStrEql("-min-depth", argStr, '\0'))
      { /*If: The user provided the minimum depth*/
         argStr = argsStrAry[iArg + 1];
         tmpStr=base10StrToUI(argStr,settings->minDepthI);

         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum depth*/


      else if(! cStrEql("-min-len", argStr, '\0'))
      { /*If: minimum fragment length*/
         argStr = argsStrAry[iArg + 1];
         tmpStr= base10StrToUI(argStr,settings->minLenI);
   
         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: minimum fragment length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun-01 Sec-02 Sub-02 Cat-03:
      +    - Consensus min base percent support settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*atof reurns 0 for failure*/
      else if(! cStrEql("-perc-snp-sup", argStr, '\0'))
      { /*If: The user provided the minimum % snp support*/
         argStr = argsStrAry[iArg + 1];
         settings->minPercSnpF = atof(argStr);

         if(settings->minPercSnpF == 0)
            return def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum % snp support*/

      else if(! cStrEql("-perc-ins-sup", argStr, '\0'))
      { /*If: The user provided the minimum % ins support*/
         argStr = argsStrAry[iArg + 1];
         settings->minPercInsF = atof(argStr);

         if(settings->minPercInsF == 0)
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum % ins support*/

      else if(! cStrEql("-perc-del-sup", argStr, '\0'))
      { /*If: The user provided the minimum % del support*/
         argStr = argsStrAry[iArg + 1];
         settings->minPercDelF = atof(argStr);

         if(settings->minPercDelF == 0)
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum % del support*/

      /**************************************************\
      * Fun-01 Sec-02 Sub-03:
      *   - Printing settings (for consensus tsv)
      \**************************************************/

      /*Settings for printing out variations*/
      else if(! cStrEql("-p-min-depth", argStr, '\0'))
      { /*If: The user provided the minimum depth*/
         argStr = argsStrAry[iArg + 1];

         tmpStr =
           base10StrToUI(argStr,settings->printMinDepthI);

         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return def_nonNumeric_freezeTb;
      } /*If: The user provided the minimum depth*/

      /*The print percentage depths*/
      else if(! cStrEql("-p-perc-snp-sup", argStr, '\0'))
      { /*If: user provided the minimum % snp support*/
         argStr = argsStrAry[iArg + 1];
         settings->printMinSupSnpF = atof(argStr);
      } /*If: user provided the minimum % snp support*/

      else if(! cStrEql("-p-perc-ins-sup", argStr, '\0'))
      { /*If: user provided the minimum % ins support*/
         argStr = argsStrAry[iArg + 1];
         settings->printMinSupInsF = atof(argStr);
      } /*If: user provided the minimum % ins support*/

      else if(! cStrEql("-p-perc-del-sup", argStr, '\0'))
      { /*If: user provided the minimum % snp support*/
         argStr = argsStrAry[iArg + 1];
         settings->printMinSupDelF = atof(argStr);
      } /*If: user provided the minimum % snp support*/

      /**************************************************\
      * Fun-01 Sec-02 Sub-04:
      *   - AMR settings
      \**************************************************/

      else if(! cStrEql(argStr,"-min-amr-map-perc",'\0'))
         *minPercMapF= atof(argsStrAry[iArg + 1]);

      else if(! cStrEql(argStr,"-frameshift",'\0'))
      { /*Else if: The user wanted frameshift checking*/
         *frameshiftBl = 1;
         --iArg;
      } /*Else if: The user wanted frameshift checking*/

      else if(! cStrEql(argStr,"-no-frameshift",'\0'))
      { /*Else if: user did not want frameshift checking*/
         *frameshiftBl = 0;
         --iArg;
      } /*Else if: user did not want frameshift checking*/

      /**************************************************\
      * Fun-01 Sec-02 Sub-05:
      *   - Filterting (mapq/mean/median)
      \**************************************************/

      else if(! cStrEql("-min-mapq", argStr, '\0'))
      { /*If: user provided the minimum mapping quality*/
         argStr = argsStrAry[iArg + 1];
         tmpStr=base10StrToUC(argStr,settings->minMapqUC);

         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*If: user provided the minimum mapping quality*/

      else if(! cStrEql(argStr,"-min-median-q",'\0'))
         *minMedianQF = atof(argsStrAry[iArg + 1]);

      else if(! cStrEql(argStr,"-min-mean-q",'\0'))
         *minMeanQF = atof(argsStrAry[iArg + 1]);

      /**************************************************\
      * Fun-01 Sec-02 Sub-06:
      *   - Lineages and graph output
      \**************************************************/

      else if(! cStrEql(argStr,"-graph-flag",'\0'))
         *graphFlagStr = argsStrAry[iArg + 1];

      else if(! cStrEql(argStr,"-fudge",'\0'))
      { /*Else If: Is the MIRU table fudge length*/
         argStr = argsStrAry[iArg + 1];
         tmpStr= base10StrToUI(argStr, *fudgeLenI);
   
         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*Else If: Is the MIRU table fudge length*/

      else if(! cStrEql("-spoligo-min-score", argStr, 0))
         *spoligoPercScoreF = atof((char *) argStr);

      else if(! cStrEql(argStr,"-dr-start",'\0'))
      { /*Else If: Start of direct repeat region*/
         argStr = argsStrAry[iArg + 1];
         tmpStr= base10StrToSI(argStr, *drStartSI);
   
         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*Else If: Start of direct repeat region*/

      else if(! cStrEql(argStr,"-dr-end",'\0'))
      { /*Else If: End of direct repeat region*/
         argStr = argsStrAry[iArg + 1];
         tmpStr= base10StrToSI(argStr, *drStartSI);
   
         /*Check for errors*/
         if(tmpStr[0] != '\0')
            return (iArg<<8) | def_nonNumeric_freezeTb;
      } /*Else If: End of direct repeat region*/

      /**************************************************\
      * Fun-01 Sec-02 Sub-07:
      *   - Check for help message requests (normal)
      \**************************************************/

      else if(! cStrEql(argStr,"-h",'\0'))
         return def_pHelp_freezeTb;

      else if(! cStrEql(argStr,"--h",'\0'))
         return def_pHelp_freezeTb;

      else if(! cStrEql(argStr,"-help",'\0'))
         return def_pHelp_freezeTb;

      else if(! cStrEql(argStr,"--help",'\0'))
         return def_pHelp_freezeTb;

      else if(! cStrEql(argStr,"help",'\0'))
         return def_pHelp_freezeTb;

      /**************************************************\
      * Fun-01 Sec-02 Sub-08:
      *   - Check for cosensus help message requests
      \**************************************************/

      else if(! cStrEql(argStr,"-h-con",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"--h-con",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"-help-con",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"--help-con",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"help-con",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"-h-consensus",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"--h-consensus",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"-help-consensus",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"--help-consensus",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      else if(! cStrEql(argStr,"help-consensus",'\0'))
         return def_pHelp_freezeTb | def_conHelp_freezeTb;

      /**************************************************\
      * Fun-01 Sec-02 Sub-09:
      *   - Check for print help message requests
      \**************************************************/

      else if(! cStrEql(argStr,"-h-p",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"--h-p",'\0'))
         return
             def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"-help-p",'\0'))
         return
           def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"--help-p",'\0'))
         return
           def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"help-p",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"-h-print",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"--h-print",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"-help-print",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"--help-print",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      else if(! cStrEql(argStr,"help-print",'\0'))
         return
            def_pHelp_freezeTb | def_printHelp_freezeTb;

      /**************************************************\
      * Fun-01 Sec-02 Sub-10:
      *   - Check for version number requets
      \**************************************************/

      else if(! cStrEql(argStr,"-v",'\0'))
         return def_pVersion_freezeTb;

      else if(! cStrEql(argStr,"--v",'\0'))
         return def_pVersion_freezeTb;

      else if(! cStrEql(argStr,"-version",'\0'))
         return def_pVersion_freezeTb;

      else if(! cStrEql(argStr,"--version",'\0'))
         return def_pVersion_freezeTb;

      else if(! cStrEql(argStr,"version",'\0'))
         return def_pVersion_freezeTb;

      /**************************************************\
      * Fun-01 Sec-02 Sub-11:
      *   - Handle invalid argument/move to next parameter
      \**************************************************/

      else return (iArg << 8);

      iArg += 2;
   } /*Loop: Read in the user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-01 Sec-03:
   ^   - Return success
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;
} /*getInput_freezeTb*/

/*-------------------------------------------------------\
| Fun-02: pHelp_freezeTb
|   - Prints the help message for freezeTb
| Input:
|   - outFILE:
|     o File to print the help message to
| Ouput:
|   - Prints:
|     o The help message to outFILE
\-------------------------------------------------------*/
void
pHelp_freezeTb(
   char pConBl,   /*Print the consensus unqiue settings*/
   char pPrintBl, /*Print the print (amr/con) settings*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun-02 sec-01:
   '     - Variable declerations
   '   o fun-02 Sec-02:
   '     - Find default file paths
   '   o fun-02 sec-03:
   '     - Build the usage entry
   '   o fun-02 sec-04:
   '     - Build the input entry
   '   o fun-02 sec-05:
   '     - Build the output entry
   '   o fun-02 sec-06:
   '     - Print the help message
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char helpStr[4096];
   int lenHelpI = 0;

   /*For finding default database locations*/
   #ifdef WINDOWS
      char *sharePathStr = getenv("PUBLIC");
      char *homePathStr = getenv("HOMEPATH");
   #else
      char *sharePathStr = "/usr/local";
      char *homePathStr = getenv("HOME");
   #endif

   char *tmpStr = 0;
   char amrDbFileStr[256];
   char miruTblFileStr[256];
   char coordFileStr[256];

   char spoligoRefFileStr[256];
   char spoligoDbFileStr[256];
   char maskPrimFileStr[256];

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Find default file paths
   ^   o fun-02 sec-02 sub-01:
   ^     - Set the path to the amr database
   ^   o fun-02 sec-02 sub-02:
   ^     - Set the path to the MIRU lineages
   ^   o fun-02 sec-02 sub-03:
   ^     - Set the path to the gene coordinates file
   ^   o fun-02 sec-02 sub-04:
   ^     - Set the path to the spoligo reference file
   ^   o fun-02 sec-02 sub-05:
   ^     - Set the path to the spoligo lineages database
   ^   o fun-02 sec-02 sub-06:
   ^     - Set up primer masking path
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Set the path to the amr database
   \*****************************************************/

   tmpStr = amrDbFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defAmrDbStr, 0, 0);
   testFILE = fopen(amrDbFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = amrDbFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr, defAmrDbStr, 0, 0);
      testFILE = fopen(amrDbFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = amrDbFileStr;
         tmpStr += ulCpStrDelim(tmpStr,defAmrDbStr,0,0);
         testFILE = fopen(amrDbFileStr, "r");

         if(! testFILE)
            amrDbFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE) fclose(testFILE);
   testFILE = 0;

   /*****************************************************\
   * Fun-02 Sec-02 Sub-02:
   *   - Set the path to the MIRU lineages
   \*****************************************************/

   tmpStr = miruTblFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defMiruTblStr, 0, 0);
   testFILE = fopen(miruTblFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = miruTblFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr, defMiruTblStr, 0, 0);
      testFILE = fopen(miruTblFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = miruTblFileStr;
         tmpStr += ulCpStrDelim(tmpStr,defMiruTblStr,0,0);
         testFILE = fopen(miruTblFileStr, "r");

         if(! testFILE)
            miruTblFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE) fclose(testFILE);
   testFILE = 0;

   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Set the path to the gene coordinates file
   \*****************************************************/

   tmpStr = coordFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defGeneCoordStr, 0, 0);
   testFILE = fopen(coordFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = coordFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defGeneCoordStr,0, 0);
      testFILE = fopen(coordFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = coordFileStr;
         tmpStr+=ulCpStrDelim(tmpStr,defGeneCoordStr,0,0);
         testFILE = fopen(coordFileStr, "r");

         if(! testFILE)
            coordFileStr[0] = '\0'; /*No idea were at*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE) fclose(testFILE);
   testFILE = 0;

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Set the path to the spoligo reference file
   \*****************************************************/

   tmpStr = spoligoRefFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defSpoligoRefsStr, 0, 0);

   testFILE = fopen(spoligoRefFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = spoligoRefFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defSpoligoRefsStr,0,0);

      testFILE = fopen(spoligoRefFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = spoligoRefFileStr;

         tmpStr +=
            ulCpStrDelim(
               tmpStr,
               defSpoligoRefsStr,
               0,
               0
            ); /*Fasta with spoligotype reference seqs*/

         testFILE = fopen(spoligoRefFileStr, "r");

         if(! testFILE)
            spoligoRefFileStr[0] = '\0'; /*No idea*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE) fclose(testFILE);
   testFILE = 0;

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Set the path to the spoligo lineages database
   \*****************************************************/

   tmpStr = spoligoDbFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');

   tmpStr +=
       ulCpStrDelim(
          tmpStr,
          defSpoligoDbStr,
          0,
          0
      );

   testFILE = fopen(spoligoDbFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = spoligoDbFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');


      tmpStr +=
          ulCpStrDelim(
             tmpStr,
             defSpoligoDbStr,
             0,
             0
         );

      testFILE = fopen(spoligoDbFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = spoligoDbFileStr;

         tmpStr +=
             ulCpStrDelim(
                tmpStr,
                defSpoligoDbStr,
                0,
                0
            );

         testFILE = fopen(spoligoDbFileStr, "r");

         if(! testFILE)
            spoligoDbFileStr[0] = '\0'; /*No idea*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;


   /*****************************************************\
   * Fun-02 Sec-02 Sub-06:
   *   - Set up primer masking path
   \*****************************************************/

   tmpStr = maskPrimFileStr;
   tmpStr += ulCpStrDelim(tmpStr, sharePathStr, 0, '\0');
   tmpStr += ulCpStrDelim(tmpStr, defPathStr, 0, '\0');

   tmpStr +=
       ulCpStrDelim(
          tmpStr,
          defMaskPrimStr,
          0,
          0
      );

   testFILE = fopen(maskPrimFileStr, "r");

   if(! testFILE)
   { /*If: I could not open the database*/
      tmpStr = maskPrimFileStr;
      tmpStr += ulCpStrDelim(tmpStr,homePathStr,0,'\0');
      tmpStr += ulCpStrDelim(tmpStr,defAltPathStr,0,'\0');


      tmpStr +=
          ulCpStrDelim(
             tmpStr,
             defMaskPrimStr,
             0,
             0
         );

      testFILE = fopen(maskPrimFileStr, "r");

      if(! testFILE)
      { /*If: I could not open the alternate path*/
         tmpStr = maskPrimFileStr;

         tmpStr +=
             ulCpStrDelim(
                tmpStr,
                defMaskPrimStr,
                0,
                0
            );

         testFILE = fopen(maskPrimFileStr, "r");

         if(! testFILE)
            maskPrimFileStr[0] = '\0'; /*No idea*/ 
      } /*If: I could not open the alternate path*/
   } /*If: I could not open the database*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Build the usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "freezeTb -sam reads.sam\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - Detects AMR(s) in ONT sequence reads\n",
      0,
      '\0'         
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-04:
   ^   - Build the input entry
   ^   o fun-02 sec-04 sub-01:
   ^     - Build the input header/File IO input
   ^   o fun-02 sec-04 sub-02:
   ^     - Fitering settings
   ^   o fun-02 sec-04 sub-03:
   ^     - Consensus settings
   ^   o fun-02 sec-04 sub-04:
   ^     - Print out the print settings
   ^   o fun-02 sec-04 sub-05:
   ^     - Print out the graph settings
   ^   o fun-02 sec-04 sub-06:
   ^     - Print out the lineage (non-database) settings
   ^   o fun-02 sec-04 sub-07:
   ^     - Print out the help message and version numbers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-04 Sub-01:
   *   - Build the input header/File IO input
   *   o fun-02 sec-04 sub-01 cat-01:
   *     - Build the input header
   *   o fun-02 sec-04 sub-01 cat-02:
   *     - Sam file input
   *   o fun-02 sec-04 sub-01 cat-03:
   *     - Prefix to name output files
   *   o fun-02 sec-04 sub-01 cat-04:
   *     - AMR table input
   *   o fun-02 sec-04 sub-01 cat-05:
   *     - Gene coordinates (paf) input
   *   o fun-02 sec-04 sub-01 cat-06:
   *     - MIRU table input
   *   o fun-02 sec-04 sub-01 cat-07:
   *     - spoligotype spacer sequence input
   *   o fun-02 sec-04 sub-01 cat-08:
   *     - spoligotype linage database
   *   o fun-02 sec-04 sub-01 cat-09:
   *     - primer masking database
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-01:
   +   - Build the input header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "Input:\n  Files: \n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-02:
   +   - Sam file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -sam: [Required]\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Sam file with reads mapped to NC000962\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-03:
   +   - Prefix to name output files
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -prefix: [",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      def_prefix_freezeTb,
      0,
      '\0'         
   );

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Prefix to name all output files with\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-04:
   +   - AMR table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -amr-tbl: [",
      0,
      '\0'         
   );

   if(amrDbFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         amrDbFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "Required",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o AMR database to detect AMRs with\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-05:
   +   - Gene coordinates (paf) input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -gene-coords: [",
      0,
      '\0'         
   );

   if(coordFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         coordFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "Required",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Gene coordinate table (for NC000962.3)\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-06:
   +   - MIRU table input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -miru-tbl: [",
      0,
      '\0'         
   );

   if(miruTblFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         miruTblFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "Required",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Miru table (tbMiru) to detect lineages",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      " with\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-07:
   +   - spoligotype spacer sequence input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -spoligo: [",
      0,
      '\0'         
   );

   if(spoligoRefFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         spoligoRefFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "Required",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Fasta file with direct repeat spacer",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      " sequences\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "        for spoligotyping\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-08:
   +   - spoligotype linage database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -db-spoligo: [",
      0,
      '\0'         
   );

   if(spoligoDbFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         spoligoDbFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "Required",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Csv file with spoligotype lineages\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o strain,barcode,ignore,lineage,SIT,",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "countries\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-01 Cat-09:
   +   - primer masking database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -mask-prim: [",
      0,
      '\0'         
   );

   if(maskPrimFileStr[0] != '\0')
   { /*If: I have a path*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         maskPrimFileStr,
         0,
         '\0'         
      );
   } /*If: I have a path*/

   else
   { /*Else: I could not find the database*/
      lenHelpI += ulCpStrDelim(
         &helpStr[lenHelpI],
         "No masking",
         0,
         '\0'         
      );
   } /*Else: I could not find the database*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Tsv file with primer positions to mask\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o ignored\tigonored\t0\tforward_start",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "\tforward_end\treverse_start\treverse_end\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Do `-prim-mask -` to disable\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-02:
   *   - Fitering settings
   *   o fun-02 sec-04 sub-02 cat-01:
   *     - Print the header
   *   o fun-02 sec-04 sub-02 cat-02:
   *     - Print mapping quality filter
   *   o fun-02 sec-04 sub-02 cat-03:
   *     - Print out the min length filter
   *   o fun-02 sec-04 sub-02 cat-04:
   *     - Print out the mean Q-score filter
   *   o fun-02 sec-04 sub-02 cat-05:
   *     - Print out the median Q-score filter
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-02 Cat-01:
   +   - Print the read fitering header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Read Filtering:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-02 Cat-02:
   +   - Print mapping quality filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-mapq: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinMapQ);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum mapping quality to keep a read\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-02 Cat-03:
   +   - Print out the min length filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-len: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinLen);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum alinged length keep a read\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-02 Cat-04:
   +   - Print out the mean Q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-mean-q: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(
          &helpStr[lenHelpI],
          def_freezeTb_minMeanQ
       );

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum mean q-score to keep a read\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-02 Cat-05:
   +   - Print out the median Q-score filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-median-q: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(
          &helpStr[lenHelpI],
          def_freezeTb_minMedianQ
       );

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum median q-score to keep a read\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-03:
   *   - Consensus settings
   *   o fun-02 sec-04 sub-03 cat-01:
   *     - Print out the consensus setting header
   *   o fun-02 sec-04 sub-03 cat-02:
   *     - Print out the min depth filter
   *   o fun-02 sec-04 sub-03 cat-03:
   *     - Print out the min length to keep a fragment
   *   o fun-02 sec-04 sub-03 cat-04:
   *     - Print out the min snp/match Q-score
   *   o fun-02 sec-04 sub-03 cat-05:
   *     - Print out the min insertion Q-score
   *   o fun-02 sec-04 sub-03 cat-06:
   *     - Print out the min percdent snp/match support
   *   o fun-02 sec-04 sub-03 cat-07:
   *     - Print out the min percdent insertion support
   *   o fun-02 sec-04 sub-03 cat-08:
   *     - Print out the min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-01:
   +   - Print out the consensus setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! pConBl) goto printSet_freezeTb;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Consensus settings:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-02:
   +   - Print out the min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-depth: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinDepth);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum read depth to keep a base\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-03:
   +   - Print out the min length to keep a fragment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-len: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinLen);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
    "      o Minimum length to keep a amplicon consensus",
     0,
     '\0'         
   );

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-04:
   +   - Print out the min snp/match Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-q: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinBaseQ);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Minimum Q-score to keep an match or snp\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-05:
   +   - Print out the min insertion Q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-q-ins: [",
      0,
      '\0'         
   );

   lenHelpI += numToStr(&helpStr[lenHelpI], defMinInsQ);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Minimum Q-score to keep an inserted base\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-06:
   +   - Print out the min percdent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -perc-snp-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI++] = '0';

   helpStr[lenHelpI++] = '.';

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPerSnp)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Minimum % of mapped reads to keep a base\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-07:
   +   - Print out the min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -perc-ins-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI] = '0';
   ++lenHelpI;

   helpStr[lenHelpI] = '.';
   ++lenHelpI;

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPerIns)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Minimum % of mapped reads to keep a ins\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-03 Cat-08:
   +   - Print out the min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -perc-del-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI] = '0';
   ++lenHelpI;

   helpStr[lenHelpI] = '.';
   ++lenHelpI;

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPerDel)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Minimum % of mapped reads to keep a del\n",
     0,
     '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-04:
   *   - Print out the print settings
   *   o fun-02 sec-04 sub-04 cat-01:
   *     - Print out the print setting header
   *   o fun-02 sec-04 sub-04 cat-02:
   *     - Print out the print min depth filter
   *   o fun-02 sec-04 sub-04 cat-03:
   +     - Print out min percent of reads to keep read AMR
   *   o fun-02 sec-04 sub-04 cat-04:
   *     - Print out the min perccent snp/match support
   *   o fun-02 sec-04 sub-04 cat-05:
   *     - Print out the min percent insertion support
   *   o fun-02 sec-04 sub-04 cat-06:
   *     - Print out the min percent deletion support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-01:
   +   - Print out the print setting header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   printSet_freezeTb:;

   if(! pPrintBl) goto finalHelp_freezeTb;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Print (read AMRs / variants) settings:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-02:
   +   - Print out the print min depth filter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -p-min-depth: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(&helpStr[lenHelpI], defMinPrintDepth);

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Variant tsv; Min read depth to print",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      " variant\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-03:
   +   - Print out min percent of reads to keep read AMR
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -min-amr-map-perc: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(
         &helpStr[lenHelpI],
         (ulong) defMinPerReadsMap
      );

   helpStr[lenHelpI++] = '.';

   lenHelpI +=
      numToStr(
         &helpStr[lenHelpI],
         (ulong) (
              (defMinPerReadsMap * 100) -
            - (((ulong) defMinPerReadsMap) * 100)
         ) /*Get the decimal digits*/
      );

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Read AMRs; Min % of reads to keep an AMR",
      0,
      '\0'         
   );

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-04:
   +   - Print out the min percent snp/match support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -p-perc-snp-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI] = '0';
   ++lenHelpI;

   helpStr[lenHelpI] = '.';
   ++lenHelpI;

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPrintPerSnp)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Variant tsv; Min % of reads to print",
     0,
     '\0'         
   );

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     " snp/match\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-05:
   +   - Print out the min percent insertion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -p-perc-ins-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI] = '0';
   ++lenHelpI;

   helpStr[lenHelpI] = '.';
   ++lenHelpI;

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPrintPerIns)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Variant tsv; Min % of reads to print ins\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-04 Cat-06:
   +   - Print out the min percent deletion support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -p-perc-del-sup",
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ':';
   helpStr[lenHelpI++] = ' ';
   helpStr[lenHelpI++] = '[';

   /*In this case I know the decimal is <= 1 and +*/
   helpStr[lenHelpI] = '0';
   ++lenHelpI;

   helpStr[lenHelpI] = '.';
   ++lenHelpI;

   lenHelpI +=
     numToStr(
        &helpStr[lenHelpI],
        (int) (10000 * defMinPrintPerDel)
     ); /*Convert the decimal to integer with 2 places*/

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Variant tsv; Min % of reads to print del\n",
     0,
     '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-05:
   *   - Print out the final settings
   *   o fun-02 sec-04 sub-04 cat-01:
   *     - Graph settings header
   *   o fun-02 sec-04 sub-04 cat-02:
   *     - -graph-flag setting
   *   o fun-02 sec-04 sub-04 cat-03:
   *     - Make graphs flag
   *   o fun-02 sec-04 sub-04 cat-04:
   *     - Graph extension flag
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-05 Cat-01:
   +   - Graph settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   finalHelp_freezeTb:;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Graphing settings:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-05 Cat-02:
   +   - -graph-flag setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -graph-flag: [",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      def_graphFlag_freezeTb,
      0,
      '\0'         
   );

   helpStr[lenHelpI] = ']';
   ++lenHelpI;

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;


   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o graphAmpDepth.r uses this to split tests\n",
     0,
     '\0'         
   );


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-05 Cat-03:
   +   - Make graphs flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -graph: [",
      0,
      '\0'         
   );

   if(def_graphBl_freezeTb)
   { /*If: I am outputing graphs*/
      helpStr[lenHelpI++] = 'Y';
      helpStr[lenHelpI++] = 'e';
      helpStr[lenHelpI++] = 's';
   } /*If: I am outputing graphs*/

   else
   { /*Else: I am not outputing graphs*/
      helpStr[lenHelpI++] = 'N';
      helpStr[lenHelpI++] = 'o';
   } /*Else: I am not outputing graphs*/

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Use R to build mean read depth &",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      " coverage graphs\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-05 Cat-04:
   +   - Graph extension flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -graph-ext: [",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      def_extGraph_freezeTb,
      0,
      '\0'         
   );

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o File type to save R graphs as",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      " (sets -graph to true)\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-06:
   *   - Print lineage settings
   *   o fun-02 sec-04 sub-06 cat-01:
   *     - Lineage setting block
   *   o fun-02 sec-04 sub-06 cat-02:
   *     - MIRU fudge setting
   *   o fun-02 sec-04 sub-06 cat-03:
   *     - Spoligtyping min perc score
   *   o fun-02 sec-04 sub-06 cat-04:
   *     - Spoligtyping dr-start
   *   o fun-02 sec-04 sub-06 cat-05:
   *     - Spoligtyping dr-end
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-01:
   +   - Lineage setting block
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Lineage settings:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-02:
   +   - MIRU fudge setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -fudge: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(&helpStr[lenHelpI],def_fudgeLen_miruTblST);

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o Range to fudge lengths by to call lineage",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "\n      o Lineage length range is + or - fudge\n",
       0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-03:
   +   - Spoligtyping min perc score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -spoligo-min-score: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(
         &helpStr[lenHelpI],
         def_minPercScore_tbSpoligoWater * 100
   );

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Minimum percent score needed to count an\n",
     0,
     '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      o spoligo space as mapped\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-04:
   +   - Spoligtyping dr-start
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -dr-start: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(&helpStr[lenHelpI], def_DRStart_tbSpoligo);

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o Start of reference direct repeat region\n",
     0,
     '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-05:
   +   - Spoligtyping dr-end
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -dr-end: [",
      0,
      '\0'         
   );

   lenHelpI +=
      numToStr(&helpStr[lenHelpI], def_DREnd_tbSpoligo);

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "      o End of reference direct repeat region\n",
     0,
     '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-04 Sub-06:
   *   - Print out the help message and version numbers
   *   o fun-02 sec-04 sub-06 cat-01:
   *     - Other settings header
   *   o fun-02 sec-04 sub-06 cat-02:
   *     - Checking frameshifts
   *   o fun-02 sec-04 sub-06 cat-03:
   *     - help messages/version
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-01:
   +   - Other settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  Other settings:\n",
      0,
      '\0'         
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-02:
   +   - Checking frameshifts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI +=
      ulCpStrDelim(
         &helpStr[lenHelpI],
         "    -frameshift: [",
         0,
         '\0'
      );

   if(! def_checkFrameshift_checkAmr)
   { /*If: I am not doing frameshift checks*/
      helpStr[lenHelpI++] = 'N';
      helpStr[lenHelpI++] = 'o';
   } /*If: I am not doing frameshift checks*/

   else
   { /*Else: I am doing frameshift checking*/
      helpStr[lenHelpI++] = 'Y';
      helpStr[lenHelpI++] = 'e';
      helpStr[lenHelpI++] = 's';
   } /*Else: I am doing frameshift checking*/

   helpStr[lenHelpI++] = ']';
   helpStr[lenHelpI++] = '\n';

   lenHelpI +=
      ulCpStrDelim(
         &helpStr[lenHelpI],
         "      o On: checks for AMRs in frameshifts",
         0,
         '\0'
       );
   lenHelpI +=
      ulCpStrDelim(
         &helpStr[lenHelpI],
         " (consensus only)\n",
         0,
         '\0'
       );

   lenHelpI +=
      ulCpStrDelim(
         &helpStr[lenHelpI],
         "      o Off: frameshifts are exact matches\n",
         0,
         '\0'
       );

   lenHelpI +=
     ulCpStrDelim(
       &helpStr[lenHelpI],
       "      o Disable: with -no-frameshift (recommend)",
       0,
       '\0'
      );

   helpStr[lenHelpI++] = '\n';

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-04 Sub-06 Cat-03:
   +   - help messages/version
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -h: Print help message\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -h-con: help message with consnesus settings",
      0,
      '\0'         
   );

   helpStr[lenHelpI] = '\n';
   ++lenHelpI;

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -h-print: help message with print settings\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    -v: Version number of freezeTb/submodules\n",
      0,
      '\0'         
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-05:
   ^   - Print the output message
   ^   o fun-02 sec-05 sub-01:
   ^     - Print out the output header
   ^   o fun-02 sec-05 sub-02:
   ^     - Print out the consensus amrs file
   ^   o fun-02 sec-05 sub-03:
   ^     - Print consensus mirufile
   ^   o fun-02 sec-05 sub-04:
   ^     - Print out the consensus file
   ^   o fun-02 sec-05 sub-05:
   ^     - Print out the variants file
   ^   o fun-02 sec-05 sub-06:
   ^     - Print out the reads amr table
   ^   o fun-02 sec-05 sub-07:
   ^     - Print out the reads ids of AMR reads
   ^   o fun-02 sec-05 sub-08:
   ^     - Print out the reads MIRU table
   ^   o fun-02 sec-05 sub-09:
   ^     - Print out the filtered histogram file
   ^   o fun-02 sec-05 sub-10:
   ^     - Print out the unfiltered histogram file
   ^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-05 Sub-01:
   *   - Print out the output header
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "Output:\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-02:
   *   - Print out the consensus amrs file
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-con-amrs.tsv\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv file with AMRs in the consensus\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-03:
   *   - Print consensus mirufile
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-con-miru.tsv\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv file with MIRU lineages\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-04:
   *   - Print out the consensus file
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-consensuses.sam \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o Sam file with the consensus sequences\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-05:
   *   - Print out the variants file
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-consensuses.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o Tsv with variants for each consensus base\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-06:
   *   - Print out the reads amr table
   \*****************************************************/


   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-read-amrs.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv file with AMRs found in the reads\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-07:
   *   - Print out the reads ids of AMR reads
   \*****************************************************/


   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-id-amrs.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
     &helpStr[lenHelpI],
     "    o tsv file with ids of reads that had an AMR\n",
     0,
     '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-08:
   *   - Print out the reads MIRU table
   \*****************************************************/


   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-miru-read-tbl.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv file with the MIRU read count table\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-09:
   *   - Print out the filtered histogram file
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-depths-filter.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv with the mean read depths per gene\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      after the filtering step\n",
      0,
      '\0'         
   );

   /*****************************************************\
   * Fun-02 Sec-05 Sub-10:
   *   - Print out the unfiltered histogram file
   \*****************************************************/

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "  - prefix-depths.tsv \n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "    o tsv with the mean read depths per gene\n",
      0,
      '\0'         
   );

   lenHelpI += ulCpStrDelim(
      &helpStr[lenHelpI],
      "      before the filtering step\n",
      0,
      '\0'         
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-06:
   ^   - Print the help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fwrite(helpStr,lenHelpI,sizeof(char),(FILE *) outFILE);
} /*pHelp_freezeTb*/

/*-------------------------------------------------------\
| Fun-03: pVersion_freezeTb
|   - Prints the version number for freezeTb and its
|     submodules
| Input:
|   - outFILE:
|     o File to print the version numbers to
| Ouput:
|   - Prints:
|     o The version numbers to outFILE
\-------------------------------------------------------*/
void
pVersion_freezeTb(
   void *outFILE
){
   fprintf(
       (FILE *) outFILE,
       "freezeTb version: %i-%02i-%02i\n",
       def_year_freezeTb,
       def_month_freezeTb,
       def_day_freezeTb
   ); /*Version of freezeTb*/

   fprintf(
       (FILE *) outFILE,
       "   compliled with tbAmr Version: %i-%02i-%02i\n",
       def_year_tbAmr,
       def_month_tbAmr,
       def_day_tbAmr
   ); /*Version of tbAmr*/

   fprintf(
       (FILE *) outFILE,
       "   compliled with tbMiru Version: %i-%02i-%02i\n",
       def_year_tbMiru,
       def_month_tbMiru,
       def_day_tbMiru
   ); /*Version of tbAmr*/

   fprintf(
       (FILE *) outFILE,
       "   complied with tbCon version: %i-%02i-%02i\n",
       def_tbCon_input_year,
       def_tbCon_input_month,
       def_tbCon_input_day
   ); /*Version of tbCon*/


   fprintf(
      stdout,
      "   compiled with ampDepth version: %i-%02i-%02i\n",
      def_year_ampDepth,
      def_month_ampDepth,
      def_day_ampDepth
   );

   fprintf(
       stdout,
       "   complied with trimSam version: %i-%02i-%02i\n",
       def_year_trimSam,
       def_month_trimSam,
       def_day_trimSam
   );

   fprintf(
     stdout,
     "   complied with tbSpoligo version: %i-%02i-%02i\n",
     def_year_tbSpoligo,
     def_month_tbSpoligo,
     def_day_tbSpoligo
   );

   fprintf(
     stdout,
     "   complied with primMask version: %i-%02i-%02i\n",
     def_year_primMask,
     def_month_primMask,
     def_day_primMask
   );
} /*pVersion_freezeTb*/

/* Note-01:
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
