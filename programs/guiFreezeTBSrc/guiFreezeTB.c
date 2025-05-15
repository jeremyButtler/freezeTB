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

#define def_amrOutPercSup_guiFreezeTB 0.1
#define def_mkGraph_guiFreezeTB 1 /*1 is yes*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

/*tcltk libraries*/
#include <tk.h>
#include <tcl.h>

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"

#include "../genFreezeTB/freezeTB.h"
#include "../genFreezeTB/freezeTBPaths.h" /*(file paths)*/

/*.h files only (no .c files used)*/
#include "../genLib/endLine.h"
#include "../genBio/tbConDefs.h"
#include "../genFreezeTB/tbMiruDefs.h"
#include "../genFreezeTB/tbSpolDefs.h"
#include "../genFreezeTB/freezeTBDefs.h"
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
!   o .c  #include "../genBio/rmHomo.h"
!   o .c  #include "../genFreezeTB/spolFind.h" 
!   o .c  #include "../genFreezeTB/miruTbl.h"
!   o .c  #include "../genFreezeTB/checkAmr.h"
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
!   o .c  #include "../genLib/fileFun.h"
!   o .c  #include "../genLib/endin.h"
!   o .c  #include "../genLib/checkSum.h"
!   o .c  #include "../genLib/inflate.h"
!
!   > general biology dependencys for task
!
!   o .c  #include "../genBio/geneCoord.h"
!   o .c  #include "../genBio/codonFun.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genBio/samEntry.h"
!   o .c  #include "../genBio/edDist.h"
!
!   > alignemnt library dependencies for tasks
!
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/alnSet.h"
!   o .c  #include "../genAln/dirMatrix.h"
!   o .c  #include "../genAln/memwater.h"
!   o .c  #include "../genAln/needle.h"
!   o .c  #include "../genAln/water.h"
!   o .c  #include "../genAln/kmerFind.h"
!   o .c  #include "../genAln/mapRead.h"
!
!   > other library dependencys needed for a spefic task
!
!   o .c  #include "../genFreezeTB/spolST.h"
!   o .c  #include "../genFreezeTB/amrST.h"
!   o .c  #include "../genFreezeTB/drugAry.h"
!
!   > .h files only
!   o .c  #include "../genBio/codonTbl.h"
!   o .c  #include "../genBio/kmerBit.h" from kmerFind
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
   unsigned int lenUI = 0;
   int retSI = 0;

   /*this is here to silence unused variable errors*/
   if(dataTclST)
      errHeapStr = run_freezeTB(numArgsSI, argAryStr);
   else
      errHeapStr = run_freezeTB(numArgsSI, argAryStr);

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
         malloc((lenUI + 256) * sizeof(signed char));

      if(! errMessageHeapStr)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error printing error message %s%s",
            errHeapStr,
            str_endLine
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
            (signed char *) "tk_messageBox -message \""
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
            (signed char *) "\" -title \"ERROR\""
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
           "failed to launch message box with %s error%s",
           errMessageHeapStr,
           str_endLine
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

   signed int errSI = 0;
   signed char *tmpStr = 0;
   signed char *cmdHeapStr = 0;
   signed char defStr[4096]; /*for setting defaults*/

	signed char *cpStr = 0;
   signed char fileStr[4096]; /*for setting defaults*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initialize values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if( Tcl_Init(tclInterpSTPtr) != TCL_OK)
   { /*If: failed to initialize tcl*/
      fprintf(
         stderr,
         "Could not initialize interpetor for tcl%s",
         str_endLine
      );

      return TCL_ERROR;
   } /*If: failed to initialize tcl*/

   if( Tk_Init(tclInterpSTPtr) != TCL_OK)
   { /*If: failed to initialize tk*/
      fprintf(
         stderr,
         "Could not initialize interpetor for tk%s",
         str_endLine
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
   ^     - indel clean up
   ^   o fun03 sec03 sub05:
   ^     - amr settings
   ^   o fun03 sec03 sub06:
   ^     - lineage settings
   ^   o fun03 sec03 sub07:
   ^     - consensus settings
   ^   o fun03 sec03 sub08:
   ^     - consensus variant print settings
   ^   o fun03 sec03 sub09:
   ^     - cluster settings
   ^   o fun03 sec03 sub10:
   ^     - output settings
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
         (signed char *) "variable glob_refFa "
      );
      
   refPath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error setting up reference*/
      fprintf(
         stderr,
         "%s%sreference variable setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_coordsTsv "
      );
      
   coordPath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%scoordinate paths setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_amrDb "
      );
      
   amrPath_freezeTBPaths(tmpStr);

   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
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
         "%s%sAMR database paths setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_maskCoords "
      );
      
   maskPath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%smasking coordinates paths setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_miruDb "
      );
      
   miruPath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sMIRU-VNTR database path setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_spacer "
      );
      
   spolSpacerPath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sSpoligo spacer fasta path setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_spolDb "
      );
      
   spolLineagePath_freezeTBPaths(tmpStr);

   if(tmpStr[0] == '\0')
   { /*If: file not found*/
      tmpStr[0] = '"';
      tmpStr[1] = '"';
      tmpStr[2] = '\0';
   } /*If: file not found*/


   /*needed because windows uses \*/
   cpStr = defStr;
   tmpStr = fileStr;

   while(*cpStr != '\0')
   { /*Loop: esacape \*/
      if(*cpStr == '\\')
         *tmpStr++ = '\\';

      *tmpStr++ = *cpStr++;
   } /*Loop: esacape \*/

	*tmpStr = '\0';

   errSI =
      Tcl_Eval(
         tclInterpSTPtr,
         (char *) fileStr
      );


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sSpoligo lineage databse path setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_mapq "
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
         "%s%smin mapq setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_medQ "
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
         "%s%smin median q-score setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_meanQ "
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
         "%s%smin mean q-score setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
         (signed char *) "variable glob_minLen "
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
         "%s%smin length setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub04:
   *   - indel clean up
   *   o fun03 sec03 sub04 cat01:
   *     - indel clean up check box
   *   o fun03 sec03 sub04 cat02:
   *     - indel clean up homopolymer length
   *   o fun03 sec03 sub04 cat03:
   *     - indel cleanup indel size
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub04 Cat01:
   +   - indel clean up check box
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_rmHomoBl "
      );

   numToStr(
      tmpStr,
      def_indelClean_freezeTBDefs
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
         "%s%serror setting up indel clean up checkbox%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub04 Cat02:
   +   - indel clean up homopolymer length
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_rmHomo_homoSI "
      );

   numToStr(
      tmpStr,
      def_minHomo_freezeTBDefs
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
         "%s%serror setting indel cleanup homopolymer%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub04 Cat03:
   +   - indel cleanup indel size
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_rmHomo_indelSI "
      );

   numToStr(
      tmpStr,
      def_maxIndel_freezeTBDefs
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
         "%s%serror setting indel cleanup indel size%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub05:
   *   - amr settings
   *   o fun03 sec03 sub05 cat01:
   *     - minimum perc support
   *   o fun03 sec03 sub05 cat02:
   *     - minimum indel perc support
   *   o fun03 sec03 sub05 cat03:
   *     - frameshift support
   *   o fun03 sec03 sub05 cat04:
   *     - detect frameshifts
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat01:
   +   - AMR read minimum percent support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_amrPercSup "
      );

   double_numToStr(
      tmpStr,
      def_minPercMapped_freezeTBDefs,
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
         "%s%sAMR min percent mapped reads setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat02:
   +   - AMR indel min percent support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_amrIndelSupF "
      );

   double_numToStr(
      tmpStr,
      def_amrIndelSup_freezeTBDefs,
      3
   );

   errSI = Tcl_Eval(tclInterpSTPtr, (char *) defStr);


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sAMR min percent indel setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat03:
   +   - AMR frameshift support
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_amrFrameSupF "
      );

   double_numToStr(
      tmpStr,
      def_supFrameshift_freezeTBDefs,
      3
   );

   errSI = Tcl_Eval(tclInterpSTPtr, (char *) defStr);

   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sAMR min percent frameshift setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub05 Cat04:
   +   - detect frameshifts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_frameshift "
      );

   if(def_frameshift_freezeTBDefs)
      *tmpStr++ = '1';
   else
      *tmpStr++ = '0';
   errSI = Tcl_Eval(tclInterpSTPtr, (char *) defStr);


   if( errSI != TCL_OK)
   { /*If: error*/
      fprintf(
         stderr,
         "%s%sAMR check frameshift setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/


   /*****************************************************\
   * Fun03 Sec03 Sub06:
   *   - lineage settings
   *   o fun03 sec03 sub06 cat01:
   *     - fudge factor for miru
   *   o fun03 sec03 sub06 cat02:
   *     - spoligotype minimum similarty
   *   o fun03 sec03 sub06 cat03:
   *     - direct repeat start setup
   *   o fun03 sec03 sub06 cat04:
   *     - direct repeat end setup
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat01:
   +   - fudge factor for miru
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_miruFudge "
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
         "%s%sMIRU-VNTR fudge length error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat02:
   +   - spoligotype minimum similarty
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_spolSim "
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
         "%s%sspoligotype min percent score error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat03:
   +   - direct repeat start setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_drStart "
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
         "%s%sspoligoty DR start setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub06 Cat04:
   +   - direct repeat end setup
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_drEnd "
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
         "%s%sspoligoty DR end setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub07:
   *   - consensus settings
   *   o fun03 sec03 sub07 cat01:
   *     - minimum read depth
   *   o fun03 sec03 sub07 cat02:
   *     - snp/math minimum q-score
   *   o fun03 sec03 sub07 cat03:
   *     - insertion minimum q-score
   *   o fun03 sec03 sub07 cat04:
   *     - snp/match min percent depth
   *   o fun03 sec03 sub07 cat05:
   *     - insertion min percent depth
   *   o fun03 sec03 sub07 cat06:
   *     - deletion min percent depth
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat01:
   +   - minimum read depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_depth "
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
         "%s%sconsensus min depth setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat02:
   +   - snp/math minimum q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_snpq "
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
         "%s%sconsensus min base q-score setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat03:
   +   - insertion minimum q-score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_insq "
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
         "%s%sconsnesus min ins q-score setuperror%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat04:
   +   - snp/match min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_basePerc "
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
         "%s%sconsnesus min %% snp support setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat05:
   +   - insertion min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_insPerc "
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
         "%s%sconsnesus min %% ins support setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub07 Cat06:
   +   - deletion min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_delPerc "
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
         "%s%sconsnesus min %% del support setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub08:
   *   - consensus variant print settings
   *   o fun03 sec03 sub08 cat01:
   *     - read depth variant minimum
   *   o fun03 sec03 sub08 cat02:
   *     - snp/match variant min percent depth
   *   o fun03 sec03 sub08 cat03:
   *     - insertion variant min percent depth
   *   o fun03 sec03 sub08 cat04:
   *     - deletion varaint min percent depth
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat01:
   +   - read depth variant minimum
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minVarDepth "
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
         "%s%svariant print min depth setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat03:
   +   - snp/match variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_baseVarPerc "
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
        "%s%svariant print min base %% sup setup error%s",
        Tcl_GetStringResult(tclInterpSTPtr),
        str_endLine,
        str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat05:
   +   - insertion variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_insVarPerc "
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
         "%s%svariant print min ins %% sup setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub08 Cat06:
   +   - deletion variant min percent depth
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_delVarPerc "
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
         "%s%svariant print min del %% sup setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub09:
   *   - cluster settings
   *   o fun03 sec03 sub09 cat01:
   *     - min depth clustering
   *   o fun03 sec03 sub09 cat02:
   *     - min percent depth clustering
   *   o fun03 sec03 sub09 cat03:
   *     - min base (snp/match) q clustering
   *   o fun03 sec03 sub09 cat04:
   *     - indel length min clustering
   *   o fun03 sec03 sub09 cat05:
   *     - read error rate min clustering
   *   o fun03 sec03 sub09 cat06:
   *     - consensus error rate min clustering
   *   o fun03 sec03 sub09 cat07:
   *     - variant to error ration (max) cluster
   *   o fun03 sec03 sub09 cat08:
   *     - window variant to error ratio (max) cluster
   *   o fun03 sec03 sub09 cat09:
   *     - window length cluster
   *   o fun03 sec03 sub09 cat10:
   *     - read scoring length weight (cluster)
   *   o fun03 sec03 sub09 cat11:
   *     - consensus similarity (max) cluster
   *   o fun03 sec03 sub09 cat12:
   *     - consensus overlap (min) cluster
   *   o fun03 sec03 sub09 cat13:
   *     - consensus mask perc (max) cluster
   *   o fun03 sec03 sub09 cat14:
   *     - consensus max rebuilds cluster
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat01:
   +   - min depth clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minClustDepth "
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
         "%s%scluster min depth setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat02:
   +   - min percent depth clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minClustPercDepth "
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
         "%s%scluster min %% depth setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat03:
   +   - min base (snp/match) q clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minClustSnpQ "
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
         "%s%scluster min snp q-score setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat04:
   +   - indel length min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minClustIndelLen "
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
         "%s%scluster min indel length setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat05:
   +   - read error rate min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_clustReadErr "
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
         "%s%scluster read %% error rate setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat06:
   +   - consensus error rate min clustering
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_clustConErr "
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
         "%s%scluster con %% error rate setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat07:
   +   - variant to error ratio (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_clustErrRatio "
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
         "%s%scluster error ration setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat08:
   +   - window variant to error ratio (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_clustWinErrRatio "
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
         "%s%scluster window error ration setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat09:
   +   - window length cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_clustWinLen "
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
         "%s%scluster window length setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat10:
   +   - read scoring length weight (cluster)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_lenWeight "
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
         "%s%scluster length weight setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat11:
   +   - consensus similarity (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_maxClustSim "
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
         "%s%scluster min con %% simularity setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat12:
   +   - consensus overlap (min) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_minClustOverlap "
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
         "%s%scluster consensus %% overlap setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat13:
   +   - consensus mask perc (max) cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_maxClustMask "
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
        "%s%scluster max con %% simularity setup error%s",
        Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub09 Cat14:
   +   - consensus max rebuilds cluster
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_numRebuilds "
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
         "%s%scluster number con rebuilds setup error%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*****************************************************\
   * Fun03 Sec03 Sub09:
   *   - cluster settings
   *   o fun03 sec03 sub10 cat01:
   *     - min % ARM support for output
   *   o fun03 sec03 sub10 cat02:
   *     - min % ARM indel support for output
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub10 Cat01:
   +   - min % ARM support for output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_outSnpSup "
      );

   double_numToStr(
      tmpStr,
      def_amrOutPercSup_guiFreezeTB,
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
         "%s%serror setting up output AMR support%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub10 Cat02:
   +   - min % ARM indel support for output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_outIndelSup "
      );

   double_numToStr(
      tmpStr,
      def_amrIndelSup_freezeTBDefs,
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
         "%s%serror set up output AMR indel support%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
      );

      return TCL_ERROR;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec03 Sub10 Cat03:
   +   - build graph setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpStr = defStr;

   tmpStr +=
      cpLine_ulCp(
         tmpStr,
         (signed char *) "variable glob_mkGraphBl "
      );

   numToStr(
      tmpStr,
      def_mkGraph_guiFreezeTB
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
         "%s%serror set up output AMR indel support%s",
         Tcl_GetStringResult(tclInterpSTPtr),
         str_endLine,
         str_endLine
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
   void
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
   signed int tclArgsSI = 0;
      /*in case user input something
      `  also for error report at end
      */

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
      (signed char *) tclArgsHeapStr[0],
      (signed char *) "freezeTB"
   ); /*copy gui title*/


   tclArgsHeapStr[1]=malloc(2048 * sizeof(signed char *));

   if(! tclArgsHeapStr)
      goto memErr_main_sec04;

   if(
     tclGuiPath_freezeTBPaths(
        (signed char *) tclArgsHeapStr[1]
     )
   ){ /*If: had file error*/
      fprintf(
       stderr,
       "could not find tcl gui script for freezeTB gui%s",
       str_endLine
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
   ); /*TK_Main does not return*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tclArgsSI = 0;
   goto clean_main_sec04;

   memErr_main_sec04:;
      fprintf(
         stderr,
         "memory error setting up tcl arguments%s",
         str_endLine
      );
      goto clean_main_sec04;

   guiErr_main_sec04:;
      tclArgsSI = 2;
      goto clean_main_sec04;

   clean_main_sec04:;
      Tcl_Finalize();
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
