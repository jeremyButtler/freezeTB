/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' test SOF: Start Of File
'   - test script for GUI, will be renameed when becomes
'     the acutal freezeTB gui
'   o header:
'     - included libraries
'   o fun02: wrap_guifreezeTB
'     - wrapper function to run freezeTB
'   o fun03: initTK_guiFreezeTB
'     - initializes (sets up) and launches freezeTB gui
'   o main:
'     - main function to run the freezeTB gui
'   o license:
'     - licensing for this code (public dofun04 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

/*little lower than cmd version because report filters*/
#define def_amrPercSup_guiFreezeTB 0.01

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

/*tcltk libraries*/
#include <tk.h>
#include <tcl.h>

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"

#include "../freezeTBSrc/freezeTB.h"
#include "../freezeTBSrc/freezeTBPaths.h" /*(file paths)*/

/*.h files only (no .c files used)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genBio/tbConDefs.h"
#include "../tbAmrSrc/tbAmrDefs.h"
#include "../tbMiruSrc/tbMiruDefs.h"
#include "../tbSpolSrc/tbSpolDefs.h"
#include "../freezeTBSrc/freezeTBDefs.h"

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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - wrapper function to run freezeTB
   '   o fun01 sec01:
   '     - variable declartions and run freezeTB
   '   o fun01 sec02:
   '     - deal with errors
   '   o fun01 sec03:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declartions and run freezeTB
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *errHeapStr = 0;
   signed char *errMessageHeapStr = 0;
   signed char *tmpStr = 0;
   uint lenUI = 0;
   int retSI = 0;

   errHeapStr =
      run_freezeTB(
         numArgsSI,
         argAryStr
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - deal with errors
   ^   o fun01 sec01 sub01:
   ^     - if had no errors, return
   ^   o fun01 sec01 sub02:
   ^     - else error, allocate memory for message command
   ^   o fun01 sec01 sub03:
   ^     - else error, build and launch error message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec01 Sub01:
   *   - if had no errors, return
   \*****************************************************/

   if(! errHeapStr)
      goto noErr_fun01_sec03_sec03;

   /*****************************************************\
   * Fun01 Sec01 Sub02:
   *   - else error, allocate memory for message command
   \*****************************************************/

   else
   { /*Else: had error*/
      lenUI = endStr_ulCp(errHeapStr);

      errMessageHeapStr =
         malloc((lenUI + 256) * sizeof(schar));

      if(! errMessageHeapStr)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error printing error message %s\n",
            errHeapStr
         );

         Tcl_Eval(
            interpTclSTPtr,
            "tk_messageBox -message \"memory error\""
         );

         goto err_fun01_sec03;
      } /*If: memory error*/

      /**************************************************\
      * Fun01 Sec01 Sub03:
      *   - else error, build and launch error message
      \**************************************************/

      tmpStr = errMessageHeapStr;

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (schar *) "tk_messageBox -message \""
      );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            errHeapStr
      );

      free(errHeapStr);
      errHeapStr = 0;

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (schar *) "\" -title \"ERROR\""
      );

      /*launch the messgage box*/
      if(
         Tcl_Eval(
            interpTclSTPtr,
            (char *) errMessageHeapStr
         )
      ){ /*If: had error launching message box*/
         fprintf(
           stderr,
           "failed to launch message box with %s error\n",
           errMessageHeapStr
         );
      } /*If: had error launching message box*/

      goto err_fun01_sec03;
   } /*Else: had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   noErr_fun01_sec03_sec03:;
      retSI = TCL_OK;
      goto ret_fun01_sec03_sec03;

   err_fun01_sec03:;
      retSI = TCL_ERROR;
      goto ret_fun01_sec03_sec03;

   ret_fun01_sec03_sec03:;
     if(errHeapStr)
        free(errHeapStr);
     errHeapStr = 0;

     if(errMessageHeapStr)
        free(errMessageHeapStr);
     errMessageHeapStr = 0;

     return retSI;
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
   '     - return finished
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint errSI = 0;
   schar *tmpStr = 0;
   schar *cmdHeapStr = 0;
   schar defStr[4096]; /*for setting defaults*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initialize values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if( Tcl_Init(tclInterpSTPtr) != TCL_OK)
   { /*If: failed to initialize tcl*/
      fprintf(
         stderr,
         "Could not initialize interpetor for tcl\n"
      );

      return TCL_ERROR;
   } /*If: failed to initialize tcl*/

   if( Tk_Init(tclInterpSTPtr) != TCL_OK)
   { /*If: failed to initialize tk*/
      fprintf(
         stderr,
         "Could not initialize interpetor for tk\n"
      );

      return TCL_ERROR;
   } /*If: failed to initialize tk*/

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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error setting up reference*/
      fprintf(
         stderr,
         "%s\nreference variable setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)

      );

      return TCL_ERROR;
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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncoordinate paths setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nAMR database paths setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nmasking coordinates paths setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nMIRU-VNTR database path setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nSpoligo spacer fasta path setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nSpoligo lineage databse path setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nmin mapq setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nmin median q-score setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI = 
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nmin mean q-score setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nmin length setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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
      def_amrPercSup_guiFreezeTB,
      3
   );

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nAMR min percent mapped reads setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nMIRU-VNTR fudge length error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nspoligotype min percent score error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nspoligoty DR start setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nspoligoty DR end setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsensus min depth setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsensus min base q-score setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsnesus min ins q-score setuperror\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsnesus min %% snp support setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsnesus min %% ins support setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nconsnesus min %% del support setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nvariant print min depth setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nvariant print min base %% sup setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nvariant print min ins %% sup setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\nvariant print min del %% sup setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster min depth setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster min %% depth setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster min snp q-score setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster min indel length setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster read %% error rate setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster con %% error rate setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster error ration setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster window error ration setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster window length setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster length weight setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster min con %% simularity setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster consensus %% overlap setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster max con %% simularity setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

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

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) defStr
      );

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s\ncluster number con rebuilds setup error\n",
         Tcl_GetStringResult(tclInterpSTPtr)
      );

      return TCL_ERROR;
   } /*If: error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return finished
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(cmdHeapStr)
      free(cmdHeapStr);
   cmdHeapStr = 0;

   return TCL_OK; /*no problems*/
} /*initTK_guiFreezeTB*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Main:
'   - main function to run the freezeTB gui
' Input:
' Output:
'   - outputs files for freezeTB or error messages
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int
main(
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - run freezeTB gui
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - memory allocation for tcltk args
   '   o main sec03:
   '     - run gui
   '   o main sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char **tclArgsHeapStr = 0;
   sint tclArgsSI = 0; /*in case user input something*/
                       /*also for error report at end*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - memory allocation for tcltk args
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tclArgsHeapStr = malloc(2 * sizeof(signed char *));

   if(! tclArgsHeapStr)
      goto memErr_main_sec04;

   tclArgsSI = 2;


   tclArgsHeapStr[0] = malloc(16 * sizeof(signed char *));

   if(! tclArgsHeapStr)
      goto memErr_main_sec04;

   cpStr_ulCp(
      (schar *) tclArgsHeapStr[0],
      (schar *) "freezeTB"
   ); /*copy gui title*/


   tclArgsHeapStr[1]=malloc(2048 * sizeof(signed char *));

   if(! tclArgsHeapStr)
      goto memErr_main_sec04;

   if(
     tclGuiPath_freezeTBPaths((schar *) tclArgsHeapStr[1])
   ){ /*If: had file error*/
      fprintf(
        stderr,
        "could not find tcl gui script for freezeTB gui\n"
      );

      goto guiErr_main_sec04;
   } /*If: had file error*/
    
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - run gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   Tk_Main(
      (int) tclArgsSI, /*so argAryStr is always blank*/
      tclArgsHeapStr,
      initTK_guiFreezeTB
   ); /*TK_Main does not return, so should not happen*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tclArgsSI = 0;
   goto clean_main_sec04;

   memErr_main_sec04:;
      fprintf(
         stderr,
         "memory error setting up tcl arguments\n"
      );
      goto clean_main_sec04;

   guiErr_main_sec04:;
      tclArgsSI = 2;
      goto clean_main_sec04;

   clean_main_sec04:;
      if(tclArgsHeapStr)
      { /*If: need to free tcl input*/
         if(tclArgsHeapStr[0])
            free(tclArgsHeapStr[0]);
         if(tclArgsHeapStr[1])
            free(tclArgsHeapStr[1]);
         free(tclArgsHeapStr);
      } /*If: need to free tcl input*/

      tclArgsHeapStr = 0;

      return tclArgsSI;
} /*main*/

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
