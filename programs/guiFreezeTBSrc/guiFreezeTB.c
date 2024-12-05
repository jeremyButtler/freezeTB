/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' test SOF: Start Of File
'   - test script for GUI, will be renameed when becomes
'     the acutal freezeTB gui
'   o header:
'     - included libraries and tcltk scripts (variables)
'   o tcl01: glob_tcl_isInt_test
'     - checks if input key is numeric and if string is
'       between max and min
'   o gui:
'     - tcltk script to build gui (glob_guiScript_gui)
'   o fun02: wrap_guifreezeTB
'     - wrapper function to run freezeTB
'   o fun03: initTK_guiFreezeTB
'     - initializes (sets up) and launches freezeTB gui
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

/*tcltk libraries*/
#include <tcl.h>
#include <tk.h>

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genBuild/rmBlocks.h"

#include "../freezeTBSrc/freezeTB.h"
#include "../freezeTBSrc/freezeTBPaths.h" /*(file paths)*/

/*.h files only (no .c files used)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genBio/tbConDefs.h"
#include "../tbAmrSrc/tbAmrDefs.h"
#include "../tbMiruSrc/tbMiruDefs.h"
#include "../tbSpolSrc/tbSpolDefs.h"
#include "../freezeTBSrc/freezeTBDefs.h"
#include "gui.h" /*has script for gui*/

#include "../genClust/clustST.h" /*for default values*/
    /*edClust uses .c, so need both*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   o std #include <string.h>
!   o std #include <stdio.h>
!
!   > task specific libraries
!
!   o .c  #include "../genBio/trimSam.h"
!   o .c  #include "../genBio/maskPrim.h"
!   o .c  #include "../genBio/ampDepth.h"
!   o .c  #include "../genBio/adjCoords.h"
!   o .c  #include "../tbSpolSrc/spolFind.h" 
!   o .c  #include "../tbMiruSrc/miruTbl.h"
!   o .c  #include "../tbAmrSrc/checkAmr.h"
!   o .c  #include "../genBio/tbCon.h"
!   o .c  #include "../genClust/edClust.h"
!
!   > general library dependencies for tasks
!
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/charCp.h"
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/strAry.h"
!
!   > general biology dependencys for task
!
!   o .c  #include "../genBio/geneCoord.h"
!   o .c  #include "../genBio/codonTbl.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/samEntry.h"
!   o .c  #include "../genBio/edDist.h"
!
!   > alignemnt library dependencies for tasks
!
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/alnSet.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../genAln/kmerFind.h"
!
!   > other library dependencys needed for a spefic task
!
!   o .c  #include "../tbSpolSrc/spolST.h"
!   o .c  #include "../tbAmrSrc/amrST.h"
!   o .c  #include "../tbAmrSrc/drugAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Gui:
|   - tcltk script to build gui
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Fun01: wrap_guifreezeTB
|   - wrapper function to run freezeTB
| Input:
|   - dataTclST:
|     o ClientData structure for tcl (not used)
|   - interpTclSTPtr:
|     o pionter to a Tlc_Interp structure
|   - numArgsSI:
|     o number of arguments input (not used)
|   - argAryStr:
|     o c-string array with user input (not used)
| Output:
|   - Prints:
|     o errors to stderr
|   - Returns:
|     o TCL_OK for success
|     o TCL_ERROR for error
\-------------------------------------------------------*/
int
wrap_guiFreezeTB(
   ClientData dataTclST,
   Tcl_Interp *interpTclSTPtr,
   int numArgsSI,
   const char *argAryStr[]
){
   if(
      ! run_freezeTB(
         numArgsSI,
         argAryStr
      )
   ) return TCL_OK;

   else
      return TCL_ERROR;
} /*wrap_guiFreezeTB*/


/*-------------------------------------------------------\
| Fun03: initTK_guiFreezeTB
|   - initializes (sets up) and launches freezeTB gui
| Input:
|   - tclInterpSTPtr:
|     o pointer to Tcl_Interp structuer
| Output:
|   - initializes and launches freezeTB gui
\-------------------------------------------------------*/
int
initTK_guiFreezeTB(
   Tcl_Interp *tclInterpSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - initializes (sets up) and launches freezeTB gui
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - initialize values
   '   o fun03 sec03:
   '     - set up global variables
   '   o fun03 sec04:
   '     - set up the gui
   '   o fun03 sec05:
   '     - return finished
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *cmdHeapStr = 0;
   schar defStr[4096]; /*for setting defaults*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initialize values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*initialize the interperator for tcl and tk*/
   if( Tcl_Init(tclInterpSTPtr) == TCL_ERROR)
      return TCL_ERROR;

   if( Tk_Init(tclInterpSTPtr) == TCL_ERROR)
      return TCL_ERROR;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - set up global variables
   ^   o fun03 sec03 sub01:
   ^     - register functions
   ^   o fun03 sec03 sub02:
   ^     - requied input global variables
   ^   o fun03 sec03 sub03:
   ^     - read filtering variables
   ^   o fun03 sec03 sub04:
   ^     - amr settings
   ^   o fun03 sec03 sub05:
   ^     - lineage settings
   ^   o fun03 sec03 sub06:
   ^     - consensus settings
   ^   o fun03 sec03 sub07:
   ^     - consensus variant print settings
   ^   o fun03 sec03 sub08:
   ^     - cluster settings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - register functions
   \*****************************************************/

   Tcl_CreateCommand(
      tclInterpSTPtr,
      "freezeTB",
      wrap_guiFreezeTB,
      (ClientData) NULL,
      (Tcl_CmdDeleteProc *) NULL
   );

   /*****************************************************\
   * Fun03 Sec03 Sub02:
   *   - databases and files
   *   o fun03 sec03 sub02 cat01:
   *     - reference variable setup
   *   o fun03 sec03 sub02 cat02:
   *     - gene coordiants variable setup
   *   o fun03 sec03 sub02 cat03:
   *     - amr database variable setup
   *   o fun03 sec03 sub02 cat04:
   *     - masking coordinates database variable setup
   *   o fun03 sec03 sub02 cat05:
   *     - miru table database variable setup
   *   o fun03 sec03 sub02 cat06:
   *     - spoligo spacer database setup
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat01:
   +   - reference variable setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_refFa "
      );
      
   refPath_freezeTBPaths(tmpStr);

   if(
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      ) == TCL_ERROR
   ){ /*If: error setting up reference*/
      fprintf(
         stderr,
         "reference variable setup error\n"
      );
   } /*If: error setting up reference*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat02:
   +   - gene coordiants variable setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_coordsTsv "
      );
      
   coordPath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat03:
   +   - amr database variable setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_amrDb "
      );
      
   amrPath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat04:
   +   - masking coordinates database variable setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_maskCoords "
      );
      
   maskPath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat05:
   +   - miru table database variable setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_miruDb "
      );
      
   miruPath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat06:
   +   - spoligo spacer database setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_spacer "
      );
      
   spolSpacerPath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub02 Cat07:
   +   - spoligo lineage database setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_spolDb "
      );
      
   spolLineagePath_freezeTBPaths(tmpStr);

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*****************************************************\
   * Fun03 Sec03 Sub03:
   *   - read filtering variables
   *   o fun03 sec03 sub03 cat01:
   *     - mapping quality
   *   o fun03 sec03 sub03 cat02:
   *     - median q-score
   *   o fun03 sec03 sub03 cat03:
   *     - mean q-score
   *   o fun03 sec03 sub03 cat04:
   *     - min length
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub03 Cat01:
   +   - mapping quality
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_mapq "
      );
      
   numToStr(
      tmpStr,
      def_minMapq_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub03 Cat02:
   +   - median q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_medQ "
      );
      
   numToStr(
      tmpStr,
      def_minMedianQ_freezeTBDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub03 Cat03:
   +   - mean q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_meanQ "
      );
      
   numToStr(
      tmpStr,
      def_minMeanQ_freezeTBDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up reference fasta path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub03 Cat04:
   +   - min length
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minLen "
      );

   numToStr(
      tmpStr,
      def_minLen_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*****************************************************\
   * Fun03 Sec03 Sub04:
   *   - amr settings
   *   o fun03 sec03 sub04 cat01:
   *     - minimum perc support
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub04 Cat01:
   +   - AMR read minimum percent support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_amrPercSup "
      );

   double_numToStr(
      tmpStr,
      def_minPercMapped_tbAmrDefs,
      3
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*****************************************************\
   * Fun03 Sec03 Sub05:
   *   - lineage settings
   *   o fun03 sec03 sub05 cat01:
   *     - fudge factor for miru
   *   o fun03 sec03 sub05 cat02:
   *     - spoligotype minimum similarty
   *   o fun03 sec03 sub05 cat03:
   *     - direct repeat start setup
   *   o fun03 sec03 sub05 cat04:
   *     - direct repeat end setup
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat01:
   +   - fudge factor for miru
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_miruFudge "
      );

   numToStr(
      tmpStr,
      def_fudgeLen_tbMiruDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat02:
   +   - spoligotype minimum similarty
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_spolSim "
      );

   double_numToStr(
      tmpStr,
      def_minPercScore_tbSpolDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat03:
   +   - direct repeat start setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_drStart "
      );

   numToStr(
      tmpStr,
      def_DRStart_tbSpolDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat04:
   +   - direct repeat end setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_drEnd "
      );

   numToStr(
      tmpStr,
      def_DREnd_tbSpolDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*****************************************************\
   * Fun03 Sec03 Sub06:
   *   - consensus settings
   *   o fun03 sec03 sub06 cat01:
   *     - minimum read depth
   *   o fun03 sec03 sub06 cat02:
   *     - snp/math minimum q-score
   *   o fun03 sec03 sub06 cat03:
   *     - insertion minimum q-score
   *   o fun03 sec03 sub06 cat04:
   *     - snp/match min percent depth
   *   o fun03 sec03 sub06 cat05:
   *     - insertion min percent depth
   *   o fun03 sec03 sub06 cat06:
   *     - deletion min percent depth
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat01:
   +   - minimum read depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_depth "
      );

   numToStr(
      tmpStr,
      def_minDepth_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat02:
   +   - snp/math minimum q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_snpq "
      );

   numToStr(
      tmpStr,
      def_minNtQ_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat03:
   +   - insertion minimum q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_insq "
      );

   numToStr(
      tmpStr,
      def_minInsQ_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat04:
   +   - snp/match min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_basePerc "
      );

   double_numToStr(
      tmpStr,
      def_minSnpPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat05:
   +   - insertion min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_insPerc "
      );

   double_numToStr(
      tmpStr,
      def_minInsPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat06:
   +   - deletion min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_delPerc "
      );

   double_numToStr(
      tmpStr,
      def_minDelPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*****************************************************\
   * Fun03 Sec03 Sub07:
   *   - consensus variant print settings
   *   o fun03 sec03 sub07 cat01:
   *     - read depth variant minimum
   *   o fun03 sec03 sub07 cat02:
   *     - snp/match variant min percent depth
   *   o fun03 sec03 sub07 cat03:
   *     - insertion variant min percent depth
   *   o fun03 sec03 sub07 cat04:
   *     - deletion varaint min percent depth
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat01:
   +   - read depth variant minimum
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minVarDepth "
      );

   numToStr(
      tmpStr,
      def_minPrintDepth_tbConDefs
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat03:
   +   - snp/match variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_baseVarPerc "
      );

   double_numToStr(
      tmpStr,
      def_minSnpPrintPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat05:
   +   - insertion variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_insVarPerc "
      );

   double_numToStr(
      tmpStr,
      def_minInsPrintPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat06:
   +   - deletion variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_delVarPerc "
      );

   double_numToStr(
      tmpStr,
      def_minDelPrintPerc_tbConDefs,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*****************************************************\
   * Fun03 Sec03 Sub08:
   *   - cluster settings
   *   o fun03 sec03 sub08 cat01:
   *     - min depth clustering
   *   o fun03 sec03 sub08 cat02:
   *     - min percent depth clustering
   *   o fun03 sec03 sub08 cat03:
   *     - min base (snp/match) q clustering
   *   o fun03 sec03 sub08 cat04:
   *     - indel length min clustering
   *   o fun03 sec03 sub08 cat05:
   *     - read error rate min clustering
   *   o fun03 sec03 sub08 cat06:
   *     - consensus error rate min clustering
   *   o fun03 sec03 sub08 cat07:
   *     - variant to error ration (max) cluster
   *   o fun03 sec03 sub08 cat08:
   *     - window variant to error ratio (max) cluster
   *   o fun03 sec03 sub08 cat09:
   *     - window length cluster
   *   o fun03 sec03 sub08 cat10:
   *     - read scoring length weight (cluster)
   *   o fun03 sec03 sub08 cat11:
   *     - consensus similarity (max) cluster
   *   o fun03 sec03 sub08 cat12:
   *     - consensus overlap (min) cluster
   *   o fun03 sec03 sub08 cat13:
   *     - consensus mask perc (max) cluster
   *   o fun03 sec03 sub08 cat14:
   *     - consensus max rebuilds cluster
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat01:
   +   - min depth clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minClustDepth "
      );

   numToStr(
      tmpStr,
      def_minDepth_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat02:
   +   - min percent depth clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minClustPercDepth "
      );

   double_numToStr(
      tmpStr,
      def_minPercDepth_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat03:
   +   - min base (snp/match) q clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minClustSnpQ "
      );

   numToStr(
      tmpStr,
      def_minSnpQ_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat04:
   +   - indel length min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minClustIndelLen "
      );

   numToStr(
      tmpStr,
      def_indelLen_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat05:
   +   - read error rate min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_clustReadErr "
      );

   double_numToStr(
      tmpStr,
      def_readErrRate_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat06:
   +   - consensus error rate min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_clustConErr "
      );

   double_numToStr(
      tmpStr,
      def_conErrRate_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat07:
   +   - variant to error ratio (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_clustErrRatio "
      );

   numToStr(
      tmpStr,
      def_maxVarErrRatio_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat08:
   +   - window variant to error ratio (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_clustWinErrRatio "
      );

   numToStr(
      tmpStr,
      def_windowError_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat09:
   +   - window length cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_clustWinLen "
      );

   numToStr(
      tmpStr,
      def_window_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat10:
   +   - read scoring length weight (cluster)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_lenWeight "
      );

   numToStr(
      tmpStr,
      def_lenWeight_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat11:
   +   - consensus similarity (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_maxClustSim "
      );

   double_numToStr(
      tmpStr,
      def_maxConSim_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat12:
   +   - consensus overlap (min) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_minClustOverlap "
      );

   double_numToStr(
      tmpStr,
      def_percOverlap_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat13:
   +   - consensus mask perc (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_maxClustMask "
      );

   double_numToStr(
      tmpStr,
      def_maxNPerc_clustST,
      2
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat14:
   +   - consensus max rebuilds cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (schar *) "variable glob_numRebuilds "
      );

   numToStr(
      tmpStr,
      def_conRebuild_clustST
   );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) defStr
   ); /*set up global output directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - set up the gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cmdHeapStr =
      str_rmBlocks(
         glob_guiScript_gui,
         endStr_ulCp(glob_guiScript_gui),
         0, /*do not care about length*/
         0  /*want to remove comments*/
      );

   Tcl_Eval(
      tclInterpSTPtr,
      (char *) cmdHeapStr
   );

   free(cmdHeapStr);
   cmdHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - return finished
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(cmdHeapStr)
      free(cmdHeapStr);
   cmdHeapStr = 0;

   return TCL_OK; /*no problems*/
} /*initTK_guiFreezeTB*/

int
main(
   int numArgsSI,
   char *argAryStr[]
){
   sint tclArgsSI = 1; /*in case user input something*/
                       /*also for error report at end*/

   tclArgsSI = numArgsSI;

   if(numArgsSI > 1)
      numArgsSI = 1;

   Tk_Main(
      tclArgsSI, /*so argAryStr is always blank*/
      argAryStr,
      initTK_guiFreezeTB
   );
   /*TK_Main does not return, so should not happen*/

   numArgsSI = tclArgsSI;

   tclArgsSI = 0;
   /*
   goto cleanUp_main_sec0x;

   err_main_sec0x:;
      tclArgsSI = 1;
      goto cleanUp_main_sec0x;

   cleanUp_main_sec0x:;
   */
     return tclArgsSI;
} /*main*/

