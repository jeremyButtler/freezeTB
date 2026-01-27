/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hardFtbWdig SOF: Start Of File
'   - has the hardcoded parts of the rayWidg that is
'     needed for ftbRaylib
'     * initization for raylib
'     * check and run events (click/keyboard)
'     * rayWidg gui structure for drawing function
'   o header:
'     - included libraries and global variables
'   o .h st01: gui_ftbRayST
'     - has widgets used in the ftbRaylib GUI
'     - TODO: add output gui
'   o fun01: blank_gui_ftbRayST
'     - blanks a gui_ftbRayST structure to default values
'   o fun02: init_gui_ftBrayST
'     - initialize a gui_ftbrayst structure to default
'       values (really just blank)
'   o fun03: freeStack_gui_ftbRayST
'     - free variables in a gui_ftbRayST
'   o fun04: freeHeap_gui_ftbRayST
'     - frees a gui_ftbRayST structure
'   o .c fun05: drawGUI_ftbRayST
'     - draws the gui for a gui_ftbRayST structure
'   o fun06: initRayGui_ftbRayST
'     - initialize raylibs GUI
'   o .c fun07: hideInput_ftbRayST
'     - hides the input menu
'   o .c fun08: hideOutput_ftbRayST
'     - hides the ouput menu
'   o .c fun09: hideReport_ftbRayST
'     - hides the report menu
'   o .c fun10: hideTable_ftbRayST
'     - hides the amr table menu
'   o .c fun11: hideHsp65_ftbRayST
'     - hides the hsp65 and user lineage table menu
'   o .c fun12: hideGeneCover_ftbRayST
'     - hides the gene coverage table
'   o .c fun13: spoligoLinGet_ftbRayST
'     - gets the spoligotype lineage and sets the spoligo
'   o .c fun14: miruLinGet_ftbRayST
'     - gets the MIRU-VNTR lineage & sets miru text output
'   o .c fun15: checkDrugs_ftbRayST
'     - builds the drug resistance part of the ftb report
'   o .c fun16: getDatabases_ftbRayST
'     - get database files for freezeTB (currently Mac)
'   o .c fun17: mkCoverageTbl_ftbRayST
'     - makes the gene percent coverage table
'   o .c fun18: getHsp65Lin_ftbRayST
'     - get the getLin hsp65 lineages (an others)
'   o fun19: checkRunEvent_ftbRayST
'     - checks for an event, and if can runs found event
'     - also redraws the GUI
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and global variables
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - global and defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include <raylib.h>

#include "../genLib/numToStr.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genLib/ptrAry.h"
#include "../genLib/fileFun.h"

#include "ftbRayST.h"
#include "rayWidg.h"

#include "../genFreezeTB/freezeTB.h"
#include "../genFreezeTB/freezeTBPaths.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../genFreezeTB/freezeTBDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .h  #include "ibmPlexMono.h"
!   - everything in freezeTB
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - global and defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*main window settings*/
signed int glob_maxWidgWidthSI = 240;

/*cases when to scale*/

#ifdef NORESIZE
   #define def_resizeState_ftbRayST 0
   /*do not resize based on scale/resolution*/
#else
   #ifdef ALLRESIZE
         #define def_resizeState_ftbRayST 3
         /*downsize and upsize for all resolutions*/
   #else
      #ifdef NOUPSIZE
         #define def_resizeState_ftbRayST 2
         /*do only downsize for low resolution*/
      #else
         #define def_resizeState_ftbRayST 1
         /*only upsize for HDPI screens*/
     #endif
   #endif
#endif

/*entry box dimensions*/
#define def_entryWidth_ftbRayST 200
signed int glob_widthPrefixEntrySI =
   def_entryWidth_ftbRayST;


/*do not mess with drug tables. If you are adding
`  or removing something, then make sure to also change
`  the gene coverage step in checkDrugs_ftbRayST (fun15)
*/
signed char
   glob_drugFullStrAry[def_numDrugs_ftbRayST][32] =
   {
      "amikacin",
      "bedaquiline",
      "capreomycin",
      "clofazimine",
      "delamanid",
      "ethambutol",
      "ethionamide",
      "fluoroquine",
      "isoniazid",
      "kanamycin",
      "levofloxacin",
      "linezolid",
      "moxifloxacin",
      "protonimid",
      "pyrazinamide",
      "rifampicin",
      "streptomycin"
   }; /*the full names of each drug*/

signed char glob_drugStrAry[def_numDrugs_ftbRayST][32] =
   {
     "Amk",/*0 amikacin*/
     "Bdq",/*1 bedaquiline*/
     "Cap",/*2 capreomycin*/
     "Cfz",/*3 clorfazimine*/
     "Dlm",/*4 delamanid*/
     "Emb",/*5 ethambutol*/
     "Eto",/*6 ethionamide*/
     "Flq",/*7 fluoroquine*/
     "Inh",/*8 isoniazid;Iso,Inz,Inh*/
     "Kan",/*9 kanamycin*/
     "Lfx",/*10 levofloxacin*/
     "Lzd",/*11 linezolid*/
     "Mfx",/*12 moxifloxacin*/
     "Pmd",/*13 protonimid*/
     "Pza",/*14 pyrazinamide;Pyz,Pza,Pyr*/
     "Rif",/*15 rifampicin;Rif,Rmp,Rfm*/
     "Stm",/*16 streptomycin;Str,Stp,Stm*/
   }; /*the shorthand name for each drug*/
   /*should be sorted alphabetically*/

/*colors of drug resistnace*/
#define def_noAmrBackCol_ftbRayST 0x000004ff
#define def_noAmrTextCol_ftbRayST 0xFDE725ff

#define def_amrBackCol_ftbRayST 0xF1605Dff
#define def_amrTextCol_ftbRayST 0x000004ff

#define def_lowDepthBackCol_ftbRayST 0xFDE725ff
#define def_lowDepthTextCol_ftbRayST 0x000004ff

/*cursor blink settings*/
#define def_blinkInterval_ftbRayST 55
   /*how many frames for blink*/
#define def_blinkTime_ftbRayST (def_blinkInterval_ftbRayST>>1)
   /*how long a blink lasts; using
   `  (counter % def_interval) > def_blinkTime to find how
   `  long a blink lasts
   */

/*widget rows*/
#define def_xFqBut_ftbRayST 0
#define def_xPrefixLab_ftbRayST 1
#define def_xPrefixEntry_ftbRayST 1
#define def_xOutBut_ftbRayST 2
#define def_xConfigBut_ftbRayST 3
#define def_xIllumina_ftbRayST 4
#define def_xRunBut_ftbRayST 5

/*widget columns*/
#define def_yFqBut_ftbRayST 0
#define def_yPrefixLab_ftbRayST 0
#define def_yPrefixEntry_ftbRayST 1
#define def_yOutBut_ftbRayST 0
#define def_yConfigBut_ftbRayST 0
#define def_yIllumina_ftbRayST 0
#define def_yRunBut_ftbRayST 0

/*labels for each widget*/
static signed char *textFqButStr =
   (signed char *) "fastq files:";
static signed char *guiTitleStr =
   (signed char *) "FreezeTB";
static signed char *textConfigButStr =
   (signed char *) "Config File:";
/*static signed char *textIlluminaStr =
   (signed char *) "Short reads:"; TODO: add in later*/
static signed char *textPrefixLabStr =
   (signed char *) "Output Prefix:";
static signed char *textOutButStr =
   (signed char *) "Output Folder:";
static signed char *textRunButStr =
   (signed char *) "Run FTB:";

/*-------------------------------------------------------\
| Fun01: blank_gui_ftbRayST
|   - blanks a gui_ftbRayST structure to default values
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to blank
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
blank_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed int siDrug = 0;

   if(! guiSTPtr)
      return;

   for(siDrug=0; siDrug < def_numDrugs_ftbRayST; ++siDrug)
   { /*Loop: blank the drug colors*/
      guiSTPtr->drugBackColUI[siDrug] =
         def_noAmrBackCol_ftbRayST;
      guiSTPtr->drugTextColUI[siDrug] =
         def_noAmrTextCol_ftbRayST;
   } /*Loop: blank the drug colors*/

   guiSTPtr->mesgStr[0] = 0;

   guiSTPtr->screenIndexSC = 0;
   guiSTPtr->blinkSC = 0;
   guiSTPtr->numFramesSI = 0;

   /*prefix button defaults*/
   guiSTPtr->prefixLenSI =
      cpStr_ulCp(
         guiSTPtr->inPrefixStr,
         (signed char *) "FTB_OUT"
      );
   guiSTPtr->prefixPosArySI[0] = 0; /*scroll*/
   guiSTPtr->prefixPosArySI[1] = 0; /*cusor position*/

   /*output button*/
   guiSTPtr->outDirStr[0] = 0;

   /*file browser message*/
   guiSTPtr->fileMesgStr[0] = 0;

   /*configuration file button*/
   guiSTPtr->configFileStr[0] = 0;

   /*___________________output GUI______________________*/

   guiSTPtr->amrSupLenSI =
      double_numToStr(
         guiSTPtr->amrSupStr,
         def_minPercMapped_freezeTBDefs,
         2
      );
   guiSTPtr->amrSupPosArySI[0] = 0;
   guiSTPtr->amrSupPosArySI[1] = 0;

   guiSTPtr->indelSupLenSI =
      double_numToStr(
         guiSTPtr->indelSupStr,
         def_amrIndelSup_freezeTBDefs,
         2
      );
   guiSTPtr->indelSupPosArySI[0] = 0;
   guiSTPtr->indelSupPosArySI[1] = 0;

   guiSTPtr->filePrefixStr[0] = 0;

   /*____________________report_GUI_____________________*/
   guiSTPtr->spoligoStr[0] = 0;
   guiSTPtr->spoligoStrainStr[0] = 0;
   guiSTPtr->spoligoLineageStr[0] = 0;
   guiSTPtr->miruStr[0] = 0;

   /*____________________table GUI______________________*/
   if(guiSTPtr->amrListSTPtr)
      clear_listBox_rayWidg(guiSTPtr->amrListSTPtr);

   /*____________________hsp65 GUI______________________*/
   if(guiSTPtr->hsp65ListSTPtr)
      clear_listBox_rayWidg(guiSTPtr->hsp65ListSTPtr);

   /*_________________gene_coverage_table_______________*/
   if(guiSTPtr->geneCoverSTPtr)
      clear_listBox_rayWidg(guiSTPtr->geneCoverSTPtr);
} /*blank_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun02: init_gui_ftbRayST
|   - initialize a gui_ftbRaySt structure to default
|     values (really just blank)
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to initialize
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
init_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   guiSTPtr->fqStrSTPtr = 0;
   guiSTPtr->widgSTPtr = 0;

   guiSTPtr->inputGuiIdSI = 0;
   guiSTPtr->outGuiIdSI = 0;
   guiSTPtr->reportGuiIdSI = 0;
   guiSTPtr->amrsGuiIdSI = 0;
   guiSTPtr->coverGuiIdSI = 0;

   guiSTPtr->mesgBoxIdSI = 0;
   guiSTPtr->fqButIdSI = 0;
   guiSTPtr->fqLabIdSI = 0;
   guiSTPtr->prefixLabIdSI = 0;
   guiSTPtr->prefixEntryIdSI = 0;
   guiSTPtr->outDirIdSI = 0;
   guiSTPtr->outDirLabIdSI = 0;
   guiSTPtr->configIdSI = 0;
   guiSTPtr->configLabIdSI = 0;
   guiSTPtr->runIdSI = 0;
   guiSTPtr->fileBrowserIdSI = 0;

   guiSTPtr->fqFileSTPtr = 0;
   guiSTPtr->outDirSTPtr = 0;
   guiSTPtr->configFileSTPtr = 0;
   guiSTPtr->browserSC = -1;

   guiSTPtr->oldFtbFileSTPtr = 0;
   guiSTPtr->getPrefixButIdSI = 0;
   guiSTPtr->minAmrPercEntryIdSI = 0;
   guiSTPtr->minAmrIndelPercEntryIdSI = 0;

   guiSTPtr->miruLabIdSI = 0;
   guiSTPtr->reportPrefixLabIdSI = 0;
   guiSTPtr->spoligoLabIdSI = 0;
   guiSTPtr->drugResRectIdSI = 0;

   guiSTPtr->amrTblIdSI = 0;
   guiSTPtr->amrLabIdSI = 0;
   guiSTPtr->amrListSTPtr = 0;

   guiSTPtr->hsp65TblIdSI = 0;
   guiSTPtr->hsp65LabIdSI = 0;
   guiSTPtr->hsp65ListSTPtr = 0;

   guiSTPtr->geneCoverTblIdSI = 0;
   guiSTPtr->geneCoverLabIdSI = 0;
   guiSTPtr->geneCoverSTPtr = 0;

   blank_gui_ftbRayST(guiSTPtr);
} /*init_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun03: freeStack_gui_ftbRayST
|   - free variables in a gui_ftbRayST
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with variables to free
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
freeStack_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   if(guiSTPtr->fqStrSTPtr)
      freeHeap_str_ptrAry(guiSTPtr->fqStrSTPtr);
   if(guiSTPtr->widgSTPtr)
      freeHeap_widg_rayWidg(guiSTPtr->widgSTPtr);
   if(guiSTPtr->fqFileSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->fqFileSTPtr);
   if(guiSTPtr->outDirSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->outDirSTPtr);
   if(guiSTPtr->configFileSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->configFileSTPtr);
   if(guiSTPtr->oldFtbFileSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->oldFtbFileSTPtr);
   if(guiSTPtr->amrListSTPtr)
      freeHeap_listBox_rayWidg(guiSTPtr->amrListSTPtr);
   if(guiSTPtr->hsp65ListSTPtr)
      freeHeap_listBox_rayWidg(guiSTPtr->hsp65ListSTPtr);
   if(guiSTPtr->geneCoverSTPtr)
      freeHeap_listBox_rayWidg(guiSTPtr->geneCoverSTPtr);

   init_gui_ftbRayST(guiSTPtr);
} /*freeStack_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_gui_ftbRayST
|   - frees a gui_ftbRayST structure
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to free
| Output:
|   - Modifies:
|     o frees guiSTPtr, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   freeStack_gui_ftbRayST(guiSTPtr);
   free(guiSTPtr);
} /*freeStack_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun05: drawGUI_ftbRayST
|   - draws the gui for a gui_ftbRayST structure
| Input:
|   - voidGuiSTPtr:
|     o gui_ftbRayST struct pointer with gui to draw
|       sent in as void
| Output:
|   - Draws:
|     o current GUI state to screen
\-------------------------------------------------------*/
void
draw_gui_ftbRayST(
   void *voidGuiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - draws the gui for a gui_ftbRayST structure
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - get file browser and set width and height
   '   o fun05 sec03:
   '     - start drawing + draw buttons and entry boxes
   '   o fun05 sec04:
   '     - draw labels for file paths
   '   o fun05 sec05:
   '     - draw message box and file browser
   '   o fun05 sec06:
   '     - end drawing
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char fqFileStr[1024];
   signed char pathsStr[1024];
   signed int lenSI = 0;
   signed char tileBl = 1;

   struct Color guiCol;
   signed int heightSI = 0;
   signed int widthSI = 0;
   signed int padSI = 0;
   signed int oneRowSI = 0;
   signed int ySI = 0;

   struct gui_ftbRayST *guiSTPtr =
      (gui_ftbRayST *) voidGuiSTPtr;
   struct files_rayWidg *fileSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - get file browser and set width and height
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   switch(guiSTPtr->browserSC)
   { /*Switch: find which browser using*/
      case 0: fileSTPtr = guiSTPtr->fqFileSTPtr; break;
      case 1: fileSTPtr = guiSTPtr->outDirSTPtr; break;
      case 2: fileSTPtr =guiSTPtr->configFileSTPtr; break;
      case 3: fileSTPtr =guiSTPtr->oldFtbFileSTPtr; break;

      default: fileSTPtr = guiSTPtr->fqFileSTPtr; break;
         /*file browser is hidden, just give something*/
   } /*Switch: find which browser using*/

   guiCol = GetColor(guiSTPtr->widgSTPtr->guiColSI);

   widthSI = guiSTPtr->widgSTPtr->winWidthSI;
   heightSI = guiSTPtr->widgSTPtr->winHeightSI;

   SetWindowSize(widthSI, heightSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - start drawing + draw buttons and entry boxes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ++guiSTPtr->numFramesSI;

   /*check if blinking the cursor*/
   guiSTPtr->blinkSC =
      blinkGet_rayWidg(
         guiSTPtr->numFramesSI,
         def_blinkInterval_ftbRayST,
         def_blinkTime_ftbRayST
      );

   if(guiSTPtr->fqStrSTPtr->lenSL)
   { /*If: have fastq files to draw*/
      if(guiSTPtr->fqStrSTPtr->lenSL > 1)
      { /*If: have more than one file*/
         lenSI =
            cpStr_ulCp(
               fqFileStr,
               guiSTPtr->fqFileSTPtr->pwdStr
            );
         fqFileStr[lenSI++] = '*';
         fqFileStr[lenSI] = 0;
      } /*If: have more than one file*/

      else
      { /*Else: only one file*/
         cpStr_ulCp(
            fqFileStr,
            guiSTPtr->fqStrSTPtr->strAry[0]
         );
      } /*Else: only one file*/
   } /*If: have fastq files to draw*/

   else
      fqFileStr[0] = 0;

   ySI = 
      def_getTotalPad_rayWidg(
         guiSTPtr->widgSTPtr->fontHeightF
      ) / 2;
   padSI =
      def_getTotalPad_rayWidg(
         guiSTPtr->widgSTPtr->fontHeightF
      ) * 2 + guiSTPtr->widgSTPtr->fontHeightF;
   oneRowSI = padSI;
   padSI += ySI;

   widthSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winWidthSI
         - guiSTPtr->widgSTPtr->fontWidthF * 2,
      20,
      guiSTPtr->amrListSTPtr
   );

   heightSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winHeightSI - oneRowSI * 2,
      20,
      guiSTPtr->amrListSTPtr
   );

   widthSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winWidthSI
         - guiSTPtr->widgSTPtr->fontWidthF * 2,
      20,
      guiSTPtr->hsp65ListSTPtr
   );

   heightSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winHeightSI - oneRowSI * 2,
      20,
      guiSTPtr->hsp65ListSTPtr
   );

   widthSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winWidthSI
         - guiSTPtr->widgSTPtr->fontWidthF * 2,
      20,
      guiSTPtr->geneCoverSTPtr
   );

   heightSet_listBox_rayWidg(
      guiSTPtr->widgSTPtr->winHeightSI - oneRowSI * 2,
      20,
      guiSTPtr->geneCoverSTPtr
   );

   BeginDrawing();
      ClearBackground(guiCol);

      /*______________menu_buttons___________________*/

      widthSI = guiSTPtr->widgSTPtr->fontWidthF;

      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->inputGuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->inputGuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->inputGuiIdSI,
            (signed char *) "in",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      widthSI += guiSTPtr->widgSTPtr->fontWidthF;
      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->outGuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->outGuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->outGuiIdSI,
            (signed char *) "out",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      widthSI += guiSTPtr->widgSTPtr->fontWidthF;
      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->reportGuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->reportGuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->reportGuiIdSI,
            (signed char *) "report",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      widthSI += guiSTPtr->widgSTPtr->fontWidthF;
      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->amrsGuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->amrsGuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->amrsGuiIdSI,
            (signed char *) "AMR",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      widthSI += guiSTPtr->widgSTPtr->fontWidthF;
      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->hsp65GuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->hsp65GuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->hsp65GuiIdSI,
            (signed char *) "hsp65",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      widthSI += guiSTPtr->widgSTPtr->fontWidthF;
      guiSTPtr->widgSTPtr->xArySI[
         guiSTPtr->coverGuiIdSI
      ] = widthSI;
      guiSTPtr->widgSTPtr->yArySI[
         guiSTPtr->coverGuiIdSI
      ] = ySI;
      widthSI +=
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->coverGuiIdSI,
            (signed char *) "gene",
            0,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

      /*_______________report_amr_resistance____________*/
      if(
         ! hidenGet_widg_rayWidg(
            guiSTPtr->drugResRectIdSI,
            guiSTPtr->widgSTPtr
         )
      ){ /*If: drawing the report page*/
         widthSI = guiSTPtr->widgSTPtr->fontWidthF;
         heightSI = oneRowSI + oneRowSI / 8;

         guiSTPtr->widgSTPtr->xArySI[
            guiSTPtr->reportPrefixLabIdSI
         ] = widthSI;
         guiSTPtr->widgSTPtr->yArySI[
            guiSTPtr->reportPrefixLabIdSI
         ] = heightSI;

         backCheckTextWidth_rayWidg(
            guiSTPtr->filePrefixStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->reportPrefixLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            0,                    /*right pad if needed*/
            0,
            guiSTPtr->widgSTPtr
         );

         heightSI += oneRowSI * 0.8;
         padSI += oneRowSI * 0.8; /*add another row in*/

         for(
            lenSI = 0;
            lenSI < def_numDrugs_ftbRayST;
            ++lenSI
         ){ /*Loop: draw the amr report*/
            if(lenSI && ! (lenSI % 9) )
            { /*If: need another row (8 drugs per row)*/
               heightSI += oneRowSI;
               padSI += oneRowSI;
               widthSI = guiSTPtr->widgSTPtr->fontWidthF;
            } /*If: need another row (8 drugs per row)*/

            widthSI +=
               rectTextDrawByCoord_rayWidg(
                  glob_drugStrAry[lenSI],
                  widthSI, /*has x coordinate*/
                  heightSI, /*y cooradinate*/
                  glob_maxWidgWidthSI,
                  20,  /*at least 20 pixels*/
                  0,   /*no borders*/
                  guiSTPtr->drugTextColUI[lenSI],
                  guiSTPtr->drugBackColUI[lenSI],
                  0, /*not drawing the border*/
                  0, /*not drawing the focus border*/
                  guiSTPtr->widgSTPtr
               );
            widthSI += guiSTPtr->widgSTPtr->fontWidthF;
         }  /*Loop: draw the amr report*/

         heightSI += oneRowSI;
         padSI += oneRowSI;
         widthSI = guiSTPtr->widgSTPtr->fontWidthF * 4;

         widthSI +=
            rectTextDrawByCoord_rayWidg(
               (signed char *) "no resistance",
               widthSI, /*has x coordinate*/
               heightSI, /*y cooradinate*/
               glob_maxWidgWidthSI,
               20,  /*at least 20 pixels*/
               0,   /*no borders*/
               def_noAmrTextCol_ftbRayST,
               def_noAmrBackCol_ftbRayST,
               0, /*not drawing the border*/
               0, /*not drawing the focus border*/
               guiSTPtr->widgSTPtr
            );
         widthSI += guiSTPtr->widgSTPtr->fontWidthF;

         widthSI +=
            rectTextDrawByCoord_rayWidg(
               (signed char *) "low depth",
               widthSI, /*has x coordinate*/
               heightSI, /*y cooradinate*/
               glob_maxWidgWidthSI,
               20,  /*at least 20 pixels*/
               0,   /*no borders*/
               def_lowDepthTextCol_ftbRayST,
               def_lowDepthBackCol_ftbRayST,
               0, /*not drawing the border*/
               0, /*not drawing the focus border*/
               guiSTPtr->widgSTPtr
            );
         widthSI += guiSTPtr->widgSTPtr->fontWidthF;

         widthSI +=
            rectTextDrawByCoord_rayWidg(
               (signed char *) "resistance",
               widthSI, /*has x coordinate*/
               heightSI, /*y cooradinate*/
               glob_maxWidgWidthSI,
               20,  /*at least 20 pixels*/
               0,   /*no borders*/
               def_amrTextCol_ftbRayST,
               def_amrBackCol_ftbRayST,
               0, /*not drawing the border*/
               0, /*not drawing the focus border*/
               guiSTPtr->widgSTPtr
            );
         widthSI += guiSTPtr->widgSTPtr->fontWidthF;


         padSI += oneRowSI; /*add another row in*/
         padSI +=
             (oneRowSI - guiSTPtr->widgSTPtr->fontHeightF)
           / 4;
      }  /*If: drawing the report page*/

      /*for most of the draw functions, the return value
      `  is either, width, 0 for hidden buton, or
      `  def_noWidget_rayWidg if no widget is present.
      `  I already know all widgets exist and I do not
      `  care about  hidden widgets, so I am ignoring the
      `  return values
      */

      tileBl = 2;

      while(tileBl)
      { /*Loop: get tiles and draw widgets*/
         --tileBl;

         /*fastq file input*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->fqButIdSI,  /*button id in array*/
            textFqButStr,
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );
         backCheckTextWidth_rayWidg(
            fqFileStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->fqLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            0,                    /*no pading*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/

         /*prefix entry box*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->prefixLabIdSI,
            textPrefixLabStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/

         entryDraw_rayWidg(
            glob_widthPrefixEntrySI,
            guiSTPtr->prefixEntryIdSI,
            guiSTPtr->prefixPosArySI,/*cursor+scroll pos*/
            guiSTPtr->blinkSC,       /*blink cursor*/
            guiSTPtr->inPrefixStr,   /*current input*/
            tileBl,
            guiSTPtr->widgSTPtr
         );

         /*output directory button*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->outDirIdSI,  /*button id in array*/
            textOutButStr,
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );
         backCheckTextWidth_rayWidg(
            guiSTPtr->outDirStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->outDirLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            0,                    /*do not pad*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/
         pathsStr[0] = 0;

         /*TODO: add illumina check box*/

         /*confituration button*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->configIdSI,  /*button id in array*/
            textConfigButStr,
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );
         backCheckTextWidth_rayWidg(
            guiSTPtr->configFileStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->configLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            0,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/

         /*run button*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->runIdSI,  /*button id in array*/
            textRunButStr,
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

         /*____________output_menu______________________*/

         /*get the file prefix button*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->getPrefixButIdSI,
            (signed char *) "get prefix",
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );
         backCheckTextWidth_rayWidg(
            guiSTPtr->filePrefixStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->getPrefixLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         );

         /*mininum AMR percent support*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->minAmrPercLabIdSI,
            (signed char *) "AMR support %:",
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         );
         entryDraw_rayWidg(
            glob_widthPrefixEntrySI
               - 50 * guiSTPtr->widgSTPtr->xScaleF,
            guiSTPtr->minAmrPercEntryIdSI,
            guiSTPtr->amrSupPosArySI,/*cursor+scroll pos*/
            guiSTPtr->blinkSC,       /*blink cursor*/
            guiSTPtr->amrSupStr,     /*current input*/
            tileBl,
            guiSTPtr->widgSTPtr
         );


         /*mininum AMR indel percent support*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->minAmrIndelPercLabIdSI,
            (signed char *) "AMR indel support %:",
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/
         entryDraw_rayWidg(
            glob_widthPrefixEntrySI
               - 50 * guiSTPtr->widgSTPtr->xScaleF,
            guiSTPtr->minAmrIndelPercEntryIdSI,
            guiSTPtr->indelSupPosArySI,
            guiSTPtr->blinkSC,       /*blink cursor*/
            guiSTPtr->indelSupStr,   /*current input*/
            tileBl,
            guiSTPtr->widgSTPtr
         );

         /*get report button*/
         butDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                /*minumum width = any size*/
            guiSTPtr->getOutButSI,
            (signed char *) "build report",
            tileBl,
            guiSTPtr->widgSTPtr    /*has widgets to draw*/
         );

         /*_____________report_menu_lineages____________*/
         labDraw_rayWidg(
            guiSTPtr->widgSTPtr->winWidthSI,
            0,                    /*no min width*/
            guiSTPtr->spoligoLabIdSI,
            guiSTPtr->spoligoStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/

         labDraw_rayWidg(
            guiSTPtr->widgSTPtr->winWidthSI,
            0,                    /*no min width*/
            guiSTPtr->spoligoLineageLabIdSI,
            guiSTPtr->spoligoLineageStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/


         labDraw_rayWidg(
            guiSTPtr->widgSTPtr->winWidthSI,
            0,                    /*no min width*/
            guiSTPtr->spoligoStrainLabIdSI,
            guiSTPtr->spoligoStrainStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/


         /*_____________table_menu______________________*/
         backCheckTextWidth_rayWidg(
            guiSTPtr->filePrefixStr,
            pathsStr,
            glob_maxWidgWidthSI,
            guiSTPtr->widgSTPtr
         );
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->amrLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/
         draw_listBox_rayWidg(
            guiSTPtr->amrTblIdSI,
            tileBl,
            guiSTPtr->amrListSTPtr,
            guiSTPtr->widgSTPtr
         );

         /*_____________hsp65_menu______________________*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->hsp65LabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/
         draw_listBox_rayWidg(
            guiSTPtr->hsp65TblIdSI,
            tileBl,
            guiSTPtr->hsp65ListSTPtr,
            guiSTPtr->widgSTPtr
         );

         /*___________gene_coverage_menu________________*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->geneCoverLabIdSI,
            pathsStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/
         draw_listBox_rayWidg(
            guiSTPtr->geneCoverTblIdSI,
            tileBl,
            guiSTPtr->geneCoverSTPtr,
            guiSTPtr->widgSTPtr
         );

         /*_____________get_tile_coordinates____________*/
         if(tileBl)
         { /*If: need to apply tiling coordinates*/
            tile_widg_rayWidg(
               guiSTPtr->widgSTPtr,
               padSI,
               guiSTPtr->widgSTPtr->fontWidthF
             );
         } /*If: need to apply tiling coordinates*/
      } /*Loop: get tiles and draw widgets*/

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec05:
      ^   - draw message box and file browser
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      mesgBoxDraw_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         guiSTPtr->mesgStr,
         (signed char *) "Ok",
         guiSTPtr->widgSTPtr
      );

      fileBrowserDraw_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->fileMesgStr,
         fileSTPtr,
         guiSTPtr->widgSTPtr
      );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec06:
      ^   - end drawing
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   EndDrawing();
} /*draw_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun06: initRayGui_ftbRayST
|   - makes a heap allocated gui_ftbRayST struct and draws
|     the initial GUI
| Input:
| Output:
|   - Draws:
|     o initial gui to screen
|   - Returns:
|     o heap allocated gui_ftbRayST struct with widgets
|     o 0 for memory errors
\-------------------------------------------------------*/
struct gui_ftbRayST *
mk_gui_ftbRayST(
   void
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '  - makes a heap allocated gui_ftbRayST struct and
   '    draws the initial GUI
   '  o fun06 sec01:
   '    - variable declarations
   '  o fun06 sec02:
   '    - add widgets
   '   o fun06 sec06:
   '     - add file broswer structures and extensions
   '   o fun06 sec07:
   '     - draw gui and return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siWidg = 0;
   signed int tmpSI = 0;

   struct widg_rayWidg *widgSTPtr = 0;
   struct gui_ftbRayST *retHeapGUI = 0;
   /*Image ftbIconImg = 0;*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - initialize and add widgets
   ^   o fun06 sec02 sub01:
   ^     - initialize the gui
   ^   o fun06 sec02 sub02:
   ^     - add menu buttons
   ^   o fun06 sec02 sub03:
   ^     - add input buttons and entry boxes
   ^   o fun06 sec02 sub04:
   ^     - add output screen widgets
   ^   o fun06 sec02 sub05:
   ^     - add amr table widgets
   ^   o fun06 sec02 sub06:
   ^     - add hsp65 table widgets
   ^   o fun06 sec02 sub07:
   ^     - add report screen widgets (last for rectangles)
   ^   o fun06 sec02 sub08:
   ^     - add report screen widgets (last for rectangles)
   ^     - last because requires creating a large number
   ^       of untracked widgets for the drugs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec02 Sub01:
   *   - initialize the gui
   \*****************************************************/

   retHeapGUI = malloc(sizeof(struct gui_ftbRayST));
   if(! retHeapGUI)
      goto memErr_fun06_sec07;
   init_gui_ftbRayST(retHeapGUI);

   retHeapGUI->fqStrSTPtr = mk_str_ptrAry(32);
   if(! retHeapGUI->fqStrSTPtr)
      goto memErr_fun06_sec07;

   retHeapGUI->amrListSTPtr =
      malloc(sizeof(struct listBox_rayWidg));
   if(! retHeapGUI->amrListSTPtr)
      goto memErr_fun06_sec07;
   init_listBox_rayWidg(retHeapGUI->amrListSTPtr);

   retHeapGUI->hsp65ListSTPtr =
      malloc(sizeof(struct listBox_rayWidg));
   if(! retHeapGUI->hsp65ListSTPtr)
      goto memErr_fun06_sec07;
   init_listBox_rayWidg(retHeapGUI->hsp65ListSTPtr);

   retHeapGUI->geneCoverSTPtr =
      malloc(sizeof(struct listBox_rayWidg));
   if(! retHeapGUI->geneCoverSTPtr)
      goto memErr_fun06_sec07;
   init_listBox_rayWidg(retHeapGUI->geneCoverSTPtr);

   retHeapGUI->widgSTPtr =
      malloc(sizeof(struct widg_rayWidg));
   if(! retHeapGUI->widgSTPtr)
      goto memErr_fun06_sec07;
   init_widg_rayWidg(retHeapGUI->widgSTPtr);
   widgSTPtr = retHeapGUI->widgSTPtr;


   if(
     setup_widg_rayWidg(
        widgSTPtr,
        guiTitleStr,
        def_resizeState_ftbRayST
      )
   ) goto memErr_fun06_sec07;
      /*1 is for scaling for HDPI screens*/

   /*SetWindowIcon(ftbIconImg)*/

   /*****************************************************\
   * Fun06 Sec02 Sub02:
   *   - add menu buttons
   \*****************************************************/

   /*menu buttons*/
   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   inactiveAdd_widg_rayWidg(tmpSI, widgSTPtr);
   retHeapGUI->inputGuiIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->outGuiIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->reportGuiIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->amrsGuiIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->hsp65GuiIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->coverGuiIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub03:
   *   - add input buttons and entry boxes
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
      /*0 column and row 0, 1 = tile, -1's are for later
      `  function to get width and height
      */
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->fqButIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->fqLabIdSI = tmpSI;

   /*prefix entry box*/
   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->prefixLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->prefixEntryIdSI = tmpSI;

   /*output directory button*/
   tmpSI = addWidget_widg_rayWidg(2,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->outDirIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(2,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->outDirLabIdSI = tmpSI;

   /*configuration file button*/
   tmpSI = addWidget_widg_rayWidg(3,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->configIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(3,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->configLabIdSI = tmpSI;

   /*TODO: add illumin check box*/

   /*run freezeTB button*/
   tmpSI = addWidget_widg_rayWidg(4,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->runIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub04:
   *   - add output screen widgets
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(retHeapGUI->getPrefixButIdSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->getPrefixButIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->getPrefixLabIdSI = tmpSI;


   tmpSI = addWidget_widg_rayWidg(1,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->minAmrPercEntryIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->minAmrPercLabIdSI = tmpSI;


   tmpSI = addWidget_widg_rayWidg(2,1,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->minAmrIndelPercEntryIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(2,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->minAmrIndelPercLabIdSI = tmpSI;


   tmpSI = addWidget_widg_rayWidg(3,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->getOutButSI = tmpSI;

   /*make file browser and message box widget*/
   tmpSI = mkMesgBox_rayWidg(retHeapGUI->widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->mesgBoxIdSI = tmpSI;

   tmpSI = mkFileBrowser_rayWidg(retHeapGUI->widgSTPtr);
   if(retHeapGUI->fileBrowserIdSI < 0)
      goto memErr_fun06_sec07;
   retHeapGUI->fileBrowserIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub05:
   *   - add amr table widgets
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->amrLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->amrTblIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub06:
   *   - add hsp65 table widgets
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->hsp65LabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->hsp65TblIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub07:
   *   - add gene coverage table
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->geneCoverLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->geneCoverTblIdSI = tmpSI;

   /*****************************************************\
   * Fun06 Sec02 Sub08:
   *   - add report screen widgets (last for rectangles)
   *   - last because requires creating a large number of
   *     untracked widgets for the drugs
   \*****************************************************/

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->reportPrefixLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->spoligoLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(1,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->spoligoStrainLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(2,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->spoligoLineageLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(3,0,1,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->miruLabIdSI = tmpSI;

   tmpSI = addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
   if(tmpSI < 0)
      goto memErr_fun06_sec07;
   hidenAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   inactiveAdd_widg_rayWidg(tmpSI, retHeapGUI->widgSTPtr);
   retHeapGUI->drugResRectIdSI = tmpSI;

   for(siWidg=1; siWidg < def_numDrugs_ftbRayST; ++siWidg)
   { /*Loop: add the drug resistance rectangles*/
      tmpSI=addWidget_widg_rayWidg(0,0,0,-1,-1,widgSTPtr);
      if(tmpSI < 0)
         goto memErr_fun06_sec07;
      hidenAdd_widg_rayWidg(tmpSI,retHeapGUI->widgSTPtr);
      inactiveAdd_widg_rayWidg(
         tmpSI,
         retHeapGUI->widgSTPtr
      );
   } /*Loop: add the drug resistance rectangles*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec06:
   ^   - add file broswer structures and extensions
   ^   o fun06 sec06 sub01:
   ^     - add fastq file browser structure
   ^   o fun06 sec06 sub02:
   ^     - add output directory file browser structure
   ^   o fun06 sec06 sub03:
   ^     - add configuration file file browser structure
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec06 Sub01:
   +   - add fastq file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->fqFileSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->fqFileSTPtr)
      goto memErr_fun06_sec07;
   init_files_rayWidg(retHeapGUI->fqFileSTPtr);

   if(
      addExt_files_rayWidg(
         (signed char *) ".fq",
         1, /*cloear old extensions*/
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fastq",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fastq.gz",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fq.gz",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec06 Sub02:
   +   - add output directory file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->outDirSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->outDirSTPtr)
      goto memErr_fun06_sec07;
   init_files_rayWidg(retHeapGUI->outDirSTPtr);
   setFileLimit_files_rayWidg(1, retHeapGUI->outDirSTPtr);

   if(
      addExt_files_rayWidg(
         (signed char *) "dir",
         1, /*clear old extensions*/
         1, /*select this extension*/
         retHeapGUI->outDirSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   cpStr_ulCp(
      retHeapGUI->outDirStr,
      retHeapGUI->outDirSTPtr->pwdStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec06 Sub03:
   +   - add configuration file file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->configFileSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->configFileSTPtr)
      goto memErr_fun06_sec07;
   init_files_rayWidg(retHeapGUI->configFileSTPtr);

   setFileLimit_files_rayWidg(
      1,
      retHeapGUI->configFileSTPtr
   );

   if(
      addExt_files_rayWidg(
         (signed char *) "*",
         1, /*cloear old extensions*/
         1, /*select this extension*/
         retHeapGUI->configFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) ".txt",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->configFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec06 Sub04:
   +   - add old results prefix (for checking old reports)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->oldFtbFileSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->oldFtbFileSTPtr)
      goto memErr_fun06_sec07;
   init_files_rayWidg(retHeapGUI->oldFtbFileSTPtr);

   setFileLimit_files_rayWidg(
      1,
      retHeapGUI->oldFtbFileSTPtr
   );

   if(
      addExt_files_rayWidg(
         (signed char *) "coverage.tsv",
         0, /*do not clear old extensions*/
         1, /*select this extension*/
         retHeapGUI->oldFtbFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) "amrs.tsv",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->oldFtbFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) "miru.tsv",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->oldFtbFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) "spoligo.tsv",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->oldFtbFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   if(
      addExt_files_rayWidg(
         (signed char *) "hsp65.tsv",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->oldFtbFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec07:
   ^   - draw gui and return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpStr_ulCp(
      retHeapGUI->appDirStr,
      (signed char *) GetApplicationDirectory()
   );

   draw_gui_ftbRayST(retHeapGUI);
   return retHeapGUI;

   memErr_fun06_sec07:;
      if(retHeapGUI)
         freeHeap_gui_ftbRayST(retHeapGUI);
      retHeapGUI = 0;
      return 0;
} /*initRayGui_ftbRayST*/

/*-------------------------------------------------------\
| Fun07: hideInput_ftbRayST
|   - hides the input menu
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all input GUI widgets to be hidden and the input
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideInput_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
      hidenAdd_widg_rayWidg(
         guiSTPtr->fqButIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->fqLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->prefixEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->prefixLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->outDirIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->outDirLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->configIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->configLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->runIdSI,
         guiSTPtr->widgSTPtr
      );

      inactiveClear_widg_rayWidg(
         guiSTPtr->inputGuiIdSI,
         guiSTPtr->widgSTPtr
      );
} /*hideInput_ftbRayST*/

/*-------------------------------------------------------\
| Fun08: hideOutput_ftbRayST
|   - hides the ouput menu
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all output GUI widgets to be hidden and the output
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideOutput_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
      hidenAdd_widg_rayWidg(
         guiSTPtr->getPrefixButIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->getPrefixLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->minAmrPercEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->minAmrPercLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->minAmrIndelPercEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->minAmrIndelPercLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->getOutButSI,
         guiSTPtr->widgSTPtr
      );

      inactiveClear_widg_rayWidg(
         guiSTPtr->outGuiIdSI,
         guiSTPtr->widgSTPtr
      );
} /*hideOutput_ftbRayST*/

/*-------------------------------------------------------\
| Fun09: hideReport_ftbRayST
|   - hides the report menu
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all output GUI widgets to be hidden and the output
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideReport_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed int lenSI = 0;

   inactiveClear_widg_rayWidg(
      guiSTPtr->reportGuiIdSI,
      guiSTPtr->widgSTPtr
   );

   hidenAdd_widg_rayWidg(
      guiSTPtr->reportPrefixLabIdSI,
      guiSTPtr->widgSTPtr
   );
   hidenAdd_widg_rayWidg(
      guiSTPtr->miruLabIdSI,
      guiSTPtr->widgSTPtr
   );
   hidenAdd_widg_rayWidg(
      guiSTPtr->spoligoLabIdSI,
      guiSTPtr->widgSTPtr
   );

   hidenAdd_widg_rayWidg(
      guiSTPtr->spoligoLineageLabIdSI,
      guiSTPtr->widgSTPtr
   );
   hidenAdd_widg_rayWidg(
      guiSTPtr->spoligoStrainLabIdSI,
      guiSTPtr->widgSTPtr
   );

   for(lenSI = 0; lenSI < def_numDrugs_ftbRayST; ++lenSI)
      hidenAdd_widg_rayWidg(
         guiSTPtr->drugResRectIdSI + lenSI,
         guiSTPtr->widgSTPtr
      );
} /*hideReport_ftbRayST*/

/*-------------------------------------------------------\
| Fun10: hideTable_ftbRayST
|   - hides the amr table menu
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all output GUI widgets to be hidden and the output
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideTable_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
      inactiveClear_widg_rayWidg(
         guiSTPtr->amrsGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenAdd_widg_rayWidg(
         guiSTPtr->amrTblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->amrLabIdSI,
         guiSTPtr->widgSTPtr
      );
} /*hideTable_ftbRayST*/

/*-------------------------------------------------------\
| Fun11: hideHsp65_ftbRayST
|   - hides the hsp65 and user lineage table menu
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all output GUI widgets to be hidden and the output
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideHsp65_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
      inactiveClear_widg_rayWidg(
         guiSTPtr->hsp65GuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenAdd_widg_rayWidg(
         guiSTPtr->hsp65TblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->hsp65LabIdSI,
         guiSTPtr->widgSTPtr
      );
} /*hideHsp65_ftbRayST*/

/*-------------------------------------------------------\
| Fun12: hideGeneCover_ftbRayST
|   - hides the gene coverage table
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Mofidies:
|     o all output GUI widgets to be hidden and the output
|       menu button to have the inactive state removed
\-------------------------------------------------------*/
void
hideGeneCover_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
      inactiveClear_widg_rayWidg(
         guiSTPtr->coverGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenAdd_widg_rayWidg(
         guiSTPtr->geneCoverTblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenAdd_widg_rayWidg(
         guiSTPtr->geneCoverLabIdSI,
         guiSTPtr->widgSTPtr
      );
} /*hideGeneCover_ftbRayST*/

/*-------------------------------------------------------\
| Fun13: spoligoLinGet_ftbRayST
|   - gets the spoligotype lineage and sets the spoligo
|     output text (for report)
| Input:
|   - guiSTPtr
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Modifies:
|     o spoligoStr in guiSTPtr to have the spoligotype
\-------------------------------------------------------*/
void
spoligoLinGet_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed char fileStr[256];
   signed char lineStr[1024];
   signed int lenSI = 0;
   signed int outLenSI = 0;

   FILE *inFILE = 0;

   cpStr_ulCp(
      guiSTPtr->spoligoStr,
      (signed char *) "spoligotype:"
   );
   cpStr_ulCp(
      guiSTPtr->spoligoLineageStr,
      (signed char *) "   lineage:"
   );
   cpStr_ulCp(
      guiSTPtr->spoligoStrainStr,
      (signed char *) "   strain:"
   ); /*strain*/

   lenSI = cpStr_ulCp(fileStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &fileStr[lenSI],
      (signed char *) "-read-spoligo.tsv"
   );
   
   inFILE = fopen((char *) fileStr, "r");
   if(! inFILE)
     return;

   lenSI = 0;

   if(! fgets((char *) lineStr, 1000, inFILE) )
      return; /*no header*/
   if(! fgets((char *) lineStr, 1000, inFILE) )
      return; /*no lineage*/

   while(lineStr[lenSI++] > 32) ; /*skip file path*/

   outLenSI =
      cpStr_ulCp(
         guiSTPtr->spoligoStrainStr,
         (signed char *) "   strain:"
      ); /*strain*/
   lenSI +=
      cpWhite_ulCp(
         &guiSTPtr->spoligoStrainStr[outLenSI],
         &lineStr[lenSI]
      ); /*strain*/
   ++lenSI;

   while(lineStr[lenSI++] > 32) ; /*skip barcode*/

   ++lenSI; /*get off "'"*/
   outLenSI =
      cpStr_ulCp(
         guiSTPtr->spoligoStr,
         (signed char *) "spoligotype: "
      );
   lenSI +=
      cpWhite_ulCp(
         &guiSTPtr->spoligoStr[outLenSI],
         &lineStr[lenSI]
      ); /*octal*/
   ++lenSI;

   outLenSI =
      cpStr_ulCp(
         guiSTPtr->spoligoLineageStr,
         (signed char *) "   lineage: "
      );
   lenSI +=
      cpWhite_ulCp(
         &guiSTPtr->spoligoLineageStr[outLenSI],
         &lineStr[lenSI]
      ); /*lineage*/
   ++lenSI;

   fclose(inFILE);
   inFILE = 0;

   /*want nothing more*/
   return;
} /*spoligoLinGet_ftbRayST*/

/*-------------------------------------------------------\
| Fun14: miruLinGet_ftbRayST
|   - gets the MIRU-VNTR lineage and sets miru text output
| Input:
|   - guiSTPtr
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Modifies:
|     o miruStr in guiSTPtr to have the spoligotype
\-------------------------------------------------------*/
void
miruLinGet_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed char fileStr[256];
   signed char lineStr[1024];
   signed int lenSI = 0;
   signed int outLenSI = 0;
   signed int tmpSI = 0;

   FILE *inFILE = 0;

   outLenSI =
      cpStr_ulCp(
         guiSTPtr->miruStr,
         (signed char *) "MIRU-VNTR: "
      );

   lenSI = cpStr_ulCp(fileStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &fileStr[lenSI],
      (signed char *) "-read-mirulin.tsv"
   );
   
   inFILE = fopen((char *) fileStr, "r");
   if(! inFILE)
     return;

   lenSI = 0;
   if(! fgets((char *) lineStr, 1000, inFILE) )
      return; /*no header*/
   if(! fgets((char *) lineStr, 1000, inFILE) )
      return; /*no lineage*/

   while(lineStr[lenSI++] > 32) ; /*skip lineage col*/

   while(lineStr[lenSI])
   { /*Loop: copy the lineages*/
      tmpSI =
         cpWhite_ulCp(
            &guiSTPtr->miruStr[outLenSI],
            &lineStr[lenSI]
         ); /*strain*/
      outLenSI += tmpSI;
      lenSI += tmpSI + 1;
      guiSTPtr->miruStr[outLenSI++] = '_';
   } /*Loop: copy the lineages*/

   fclose(inFILE);
   inFILE = 0;
   return;
} /*miruLinGet_ftbRayST*/

/*-------------------------------------------------------\
| Fun15: checkDrugs_ftbRayST
|   - builds the drug resistance part of the ftb report
| Input:
|   - guiSTPtr
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Draws:
|     o drug resistance to screen as colored rectangles
\-------------------------------------------------------*/
void
checkDrugs_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - builds the drug resistance part of the ftb report
   '   o fun15 sec01:
   '     - variable declarations
   '   o fun15 sec02:
   '     - initialize
   '   o fun15 sec03:
   '     - check if have enough coverage for no amr call
   '   o fun15 sec04:
   '     - set the drug resistance colors
   '   o fun15 sec05:
   '     - find AMRs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char fileStr[256];
   signed char lineStr[1024];
   signed int lenSI = 0;
   signed char *tmpStr = 0;

   signed char geneStr[128];
   signed int drugSI = 0;

   signed char resAryBl[def_numDrugs_ftbRayST];

   /*has drugs resistance to (first is always primary*/
   float minPerSupF = def_minPercMapped_freezeTBDefs;
   float minIndelPerSupF = def_amrIndelSup_freezeTBDefs;

   /*these are for recording the hits in the table*/
   signed char colStr[128]; /*holds one column*/
   signed char typeStr[32]; /*holds one column*/
   signed char varIdStr[128];
   signed char resLevelStr[16];
   signed char resAdditiveStr[16];
   signed char needsGeneStr[64];
   signed int gradeSI = 0;
   signed int refPosSI = 0;
   signed int numSupReadsSI = 0;
   signed int numMapReadsSI = 0;
   float percSupReadsF = 0;

   FILE *inFILE = 0;

   /*for building the drug resistance part of the report*/
   signed char drugAryStr[def_numDrugs_ftbRayST * 4];
   signed int drugLenSI = 0;
   signed int crossStartSI = 0;

   signed char depthAryBl[17];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - initialize
   ^   o fun15 sec02 sub01:
   ^     - get default filters and clear AMRs
   ^   o fun15 sec02 sub02:
   ^     - add the header to the amr table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec02 Sub01:
   *   - get default filters and clear AMRs
   \*****************************************************/
   
   if(! guiSTPtr->filePrefixStr[0])
      return;

   for(drugSI = 0; drugSI < 17; ++drugSI)
      depthAryBl[drugSI] = 0;
      /*records if I have enough depth to make a no call*/

   if(guiSTPtr->amrSupStr[0])
      strToF_base10str(guiSTPtr->amrSupStr, &minPerSupF);
   if(guiSTPtr->indelSupStr[0])
      strToF_base10str(
         guiSTPtr->indelSupStr,
         &minIndelPerSupF
      );

   clear_listBox_rayWidg(guiSTPtr->amrListSTPtr);

   for(lenSI = 0; lenSI < def_numDrugs_ftbRayST; ++lenSI)
   { /*Loop: blank the drug colors*/
      guiSTPtr->drugBackColUI[lenSI] =
         def_lowDepthBackCol_ftbRayST;
      guiSTPtr->drugTextColUI[lenSI] =
         def_lowDepthTextCol_ftbRayST;
   } /*Loop: blank the drug colors*/

   /*****************************************************\
   * Fun15 Sec02 Sub02:
   *   - add the header to the amr table
   \*****************************************************/

   /*build header for list box*/
   lenSI = 0;
   lenSI +=
      cpStr_ulCp(&lineStr[lenSI], (signed char *)" drug");
        /*space is frist alphabetcially, so I can use it
        `  to keep the header at the top during the sort
        */
   lineStr[lenSI++] = ' '; /*set lenSI to 5*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "variant"
      );
   while(lenSI < 24)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 25*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "other_drugs"
      );
   while(lenSI < 36)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 37*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "%_support"
      );
   while(lenSI < 47)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 58*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "read_depth"
      );
   while(lenSI < 56)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 57*/

   lenSI +=
      cpStr_ulCp(&lineStr[lenSI],(signed char *) "needs");

   /*add header to list box*/
   if(
      addItem_listBox_rayWidg(
         lineStr,
         def_listSpecial_rayWidg, /*does nothing*/
         guiSTPtr->amrListSTPtr,
         guiSTPtr->widgSTPtr
      )
   ) return; /*memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - check if have enough coverage for no amr call
   ^   o fun15 sec03 sub01:
   ^     - open coverage file
   ^   o fun15 sec03 sub02:
   ^     - start loop, get percent coverage, move to drugs
   ^   o fun15 sec03 sub03:
   ^     - find the drugs in each gene/target
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec03 Sub01:
   *   - open coverage file
   \*****************************************************/

   lenSI = cpStr_ulCp(fileStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &fileStr[lenSI],
      (signed char *) "-coverage.tsv"
   );

   inFILE = fopen((char *) fileStr, "r");
   if(! inFILE)
     return;

   /*****************************************************\
   * Fun15 Sec03 Sub02:
   *   - start loop, get percent coverage, move to drugs
   \*****************************************************/

   if(fgets((char *) lineStr, 1000, inFILE))
   { /*If: have header*/
      while( fgets((char *) lineStr, 1000, inFILE) )
      { /*Loop: get depths for each gene*/
         tmpStr = lineStr;
         lenSI = 0;

         /*___________get_past_the_gene_name____________*/
         tmpStr += endWhite_ulCp(tmpStr);
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            continue;

         /*___________get_perc_coverage_________________*/
         tmpStr +=
            strToF_base10str(tmpStr, &percSupReadsF);
         if(*tmpStr > 32)
            continue; /*bad line, ignore*/
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            continue;

         /*___________get_past_mean_read_depth__________*/
         tmpStr += endWhite_ulCp(tmpStr);
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            continue;

         /*___________get_past_mean_read_depth_2________*/
         tmpStr += endWhite_ulCp(tmpStr);
         while(*tmpStr && *tmpStr < 33)
            ++tmpStr;
         if(! *tmpStr)
            continue;

         /***********************************************\
         * Fun15 Sec03 Sub03:
         *   - find the drugs in each gene/target
         \***********************************************/

         nextDrug_fun15_sec03_sub03:;

         for(drugSI = 0; drugSI < 17; ++drugSI)
         { /*Loop: find drug*/
            /*_________check_if_have_a_drug_____________*/
            if(*tmpStr == '*')
                break; /*done*/

            if(
                  (tmpStr[0] | 32) =='n'
               && (tmpStr[1] | 32) =='a'
               && tmpStr[2] < 33
            ) break; /*no drug in this column*/

            /*_________check_if_have_this_drug__________*/
            if(
                  (tmpStr[0] | 32)
               != (glob_drugStrAry[drugSI][0] | 32)
            ) continue;

            else if(
                  (tmpStr[1] | 32)
               != (glob_drugStrAry[drugSI][1] | 32)
            ) continue;

            else if(
                  (tmpStr[2] | 32)
               != (glob_drugStrAry[drugSI][2] | 32)
            ) continue;

            /*______________check_if_have_depth_________*/
            if(percSupReadsF < 0.95)
               depthAryBl[drugSI] = -1;
            else
               depthAryBl[drugSI] = 1;
         } /*Loop: find drug*/

         if(*tmpStr && *tmpStr != '*')
         { /*If: I have more drugs to find*/
            tmpStr += endWhite_ulCp(tmpStr);
            while(*tmpStr && *tmpStr < 33)
               ++tmpStr;
            goto nextDrug_fun15_sec03_sub03;
         } /*If: I have more drugs to find*/
      } /*Loop: get depths for each gene*/
   } /*If: have header*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - set the drug resistance colors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(drugSI = 0; drugSI < 17; ++drugSI)
   { /*Loop: set the colors*/
      if(depthAryBl[drugSI] > 0)
      { /*If: enough read depth to make no amr call*/
         guiSTPtr->drugBackColUI[drugSI] =
            def_noAmrBackCol_ftbRayST;
         guiSTPtr->drugTextColUI[drugSI] =
            def_noAmrTextCol_ftbRayST;
      } /*If: enough read depth to make no amr call*/

      else
      { /*Else: not enough depth for no amr call*/
         guiSTPtr->drugBackColUI[drugSI] =
            def_lowDepthBackCol_ftbRayST;
         guiSTPtr->drugTextColUI[drugSI] =
            def_lowDepthTextCol_ftbRayST;
      } /*Else: not enough depth for no amr call*/
   } /*Loop: set the colors*/

   drugSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec05:
   ^   - find AMRs
   ^   o fun15 sec05 sub01:
   ^     - open amr file (reads)
   ^   o fun15 sec05 sub02:
   ^     - get gene name + start loop
   ^   o fun15 sec05 sub03:
   ^     - check for resistance
   ^   o fun15 sec05 sub04:
   ^     - check for cross resistance (loops to sub03)
   ^   o fun15 sec05 sub05:
   ^     - get variant id, grade, and support
   ^   o fun15 sec05 sub06:
   ^     - check if amr has enough support
   ^   o fun15 sec05 sub07:
   ^     - if enough support, set resistance colors
   ^   o fun15 sec05 sub08:
   ^     - get high/low res, additive res, and genes
   ^   o fun15 sec05 sub09:
   ^     - build amr table entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec05 Sub01:
   *   - open amr file (reads)
   \*****************************************************/

   lenSI = cpStr_ulCp(fileStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &fileStr[lenSI],
      (signed char *) "-read-amrs.tsv"
   );

   inFILE = fopen((char *) fileStr, "r");
   if(! inFILE)
      return;

   /*****************************************************\
   * Fun15 Sec05 Sub02:
   *   - get gene name + start loop
   \*****************************************************/

   if(fgets((char *) lineStr, 1000, inFILE))
   { /*If: have header*/
      while( fgets((char *) lineStr, 1000, inFILE) )
      { /*Loop: get drug resistance*/
         crossStartSI = 0;
         drugLenSI = 0;
         drugAryStr[0] = 0;

         tmpStr = lineStr;

         tmpStr += cpWhite_ulCp(geneStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         lenSI = 0;
         while(*tmpStr > 32)
            colStr[lenSI++] = ((*tmpStr++) | 32);
         colStr[lenSI] = 0;
         ++tmpStr;

         /***********************************************\
         * Fun15 Sec05 Sub03:
         *   - check for resistance
         \***********************************************/

         if(
               colStr[0] == 'n'
            && colStr[1] == 'a'
            && ! colStr[2]
         ) tmpStr += 2; /*no drug to check*/

         else
         { /*Else: check what drug am resistant to*/
            checkDrug_fun15_sec05_sub03:;
               for(
                  lenSI = 0;
                  lenSI < def_numDrugs_ftbRayST;
                  ++lenSI
               ){ /*Loop: check all drugs*/
                  resAryBl[lenSI] = 0;
                  drugAryStr[drugLenSI] = 0;

                  if(
                        ! eqlNull_ulCp(
                           colStr,
                           glob_drugFullStrAry[lenSI]
                        ) /*check if full drug name used*/
                     || ! eqlNull_ulCp(
                           colStr,
                           glob_drugStrAry[lenSI]
                        ) /*check if shorthand instead*/
                  ){ /*If: found resistance*/
                     resAryBl[lenSI] = 1;

                     drugAryStr[drugLenSI++] =
                        glob_drugStrAry[lenSI][0];
                     drugAryStr[drugLenSI++] =
                        glob_drugStrAry[lenSI][1];
                     drugAryStr[drugLenSI++] =
                        glob_drugStrAry[lenSI][2];
                     drugAryStr[drugLenSI++] = '_';
                  }  /*If: found resistance*/
               }  /*Loop: check all drugs*/
         } /*Else: check what drug am resistant to*/

         /***********************************************\
         * Fun15 Sec05 Sub04:
         *   - check for cross resistance (loops to sub03)
         \***********************************************/

         drugAryStr[drugLenSI] = 0;
         if(! crossStartSI)
         { /*If: need to mark end of primary drug*/
            drugAryStr[drugLenSI - 1] = 0;
            crossStartSI = drugLenSI;

            /*assume no cross resistance, this will be
            `  overwritten if there is cross resitstance
            */
            drugAryStr[drugLenSI] = 'N';
            drugAryStr[drugLenSI + 1] = 'A';
            drugAryStr[drugLenSI + 2] = 0;
         } /*If: need to mark end of primary drug*/

         if(
               tmpStr[0] == 'N'
            && tmpStr[1] == 'A'
            && tmpStr[2] < 33
         ) tmpStr += 2; /*no cross resistance*/

         else if(*tmpStr > 32)
         { /*Else If: have cross resistance*/

            if(*tmpStr == '_')
               ++tmpStr;

            lenSI = 0;
            while(*tmpStr > 32 && *tmpStr != '_')
               colStr[lenSI++] = ((*tmpStr++) | 32);
            goto checkDrug_fun15_sec05_sub03;
         } /*Else If: have cross resistance*/

         if(! *tmpStr)
            continue;
         ++tmpStr;

         /***********************************************\
         * Fun15 Sec05 Sub05:
         *   - get variant id, grade, and support
         \***********************************************/

         /*get the variant id*/
         tmpStr += cpWhite_ulCp(varIdStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get mutation type entry*/
         tmpStr += cpWhite_ulCp(typeStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get the grade of the AMR*/
         tmpStr += strToSI_base10str(tmpStr, &gradeSI);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get the reference position*/
         tmpStr += strToSI_base10str(tmpStr, &refPosSI);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get number of supporting reads for the AMR*/
         tmpStr+=strToSI_base10str(tmpStr,&numSupReadsSI);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get percentage of reads supported AMR*/
         tmpStr+=strToF_base10str(tmpStr, &percSupReadsF);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /***********************************************\
         * Fun15 Sec05 Sub06:
         *   - check if amr has enough support
         \***********************************************/

         /*check if keeping AMR*/
         if(
               (typeStr[0] | 32) == 's'
            && (typeStr[1] | 32) == 'n'
            && (typeStr[2] | 32) == 'p'
            && typeStr[3] < 33
         ){ /*If: snp variant*/
            if(percSupReadsF < minPerSupF * 100)
               continue;
         }  /*If: snp variant*/

         else
         { /*Else: frameshift or indel*/
            if(percSupReadsF < minIndelPerSupF * 100)
               continue;
         } /*Else: frameshift or indel*/

         /***********************************************\
         * Fun15 Sec05 Sub07:
         *   - if enough support, set resistance colors
         \***********************************************/

         for(
            lenSI = 0;
            lenSI < def_numDrugs_ftbRayST;
            ++lenSI
         ){ /*Loop: set resistance*/
            if(resAryBl[lenSI])
            { /*If: had resistance*/
               guiSTPtr->drugBackColUI[lenSI] =
                  def_amrBackCol_ftbRayST;
               guiSTPtr->drugTextColUI[lenSI] =
                  def_amrTextCol_ftbRayST;
            } /*If: had resistance*/
         }  /*Loop: set resistance*/

         /*get the number of mapped reads*/
         tmpStr+=strToSI_base10str(tmpStr,&numMapReadsSI);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /***********************************************\
         * Fun15 Sec05 Sub08:
         *   - get high/low res, additive res, and genes
         \***********************************************/

         /*get if high or low resistance*/
         tmpStr+=cpWhite_ulCp(resLevelStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get if resistance is additive or not*/
         tmpStr+=cpWhite_ulCp(resAdditiveStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /*get if needs another gene to support*/
         tmpStr += cpWhite_ulCp(needsGeneStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         /***********************************************\
         * Fun15 Sec05 Sub09:
         *   - build amr table entry
         \***********************************************/

         lenSI = 0;

         /*copy the drug name*/
         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], &drugAryStr[0]);
         lineStr[lenSI++] = ' '; /*set lenSI to 4*/
         lineStr[lenSI++] = ' '; /*set lenSI to 5*/

         /*copy variant id*/
         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], varIdStr);

         while(lenSI < 23)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*make 24*/

         /*copy cross resistance*/
         if(drugAryStr[drugLenSI - 1] == '_')
            drugAryStr[drugLenSI - 1] = 0;
            /*cross resitistance will add a _ to the end*/

         lenSI +=
            cpStr_ulCp(
               &lineStr[lenSI],
               &drugAryStr[crossStartSI]
            );
         while(lenSI < 35)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 36*/

         /*copy read support*/
         lenSI +=
            double_numToStr(
               &lineStr[lenSI],
               percSupReadsF,
               2
            );
         while(lenSI < 46)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 47*/

         lenSI += numToStr(&lineStr[lenSI],numSupReadsSI);
         while(lenSI < 57)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 58*/

         /*add needed genes*/
         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], needsGeneStr);

         /*add entry to the list box and move on*/
         if(
            addItem_listBox_rayWidg(
               lineStr,
               def_listSpecial_rayWidg, /*does nothing*/
               guiSTPtr->amrListSTPtr,
               guiSTPtr->widgSTPtr
            )
         ) return; /*memory error*/
      } /*Loop: get drug resistance*/

      sort_listBox_rayWidg(guiSTPtr->amrListSTPtr);

      cpStr_ulCp(
         guiSTPtr->amrListSTPtr->textAryStr[0],
         &guiSTPtr->amrListSTPtr->textAryStr[0][1]
      ); /*remove the space keeping header at top*/
   } /*If: have header*/

   fclose(inFILE);
   inFILE = 0;
} /*checkDrugs_ftbRayST*/

/*-------------------------------------------------------\
| Fun16: getDatabases_ftbRayST
|   - get database files for freezeTB (currently Mac)
| Input:
|   - appPathStr:
|     o c-string with path to ftbRay binarary
|   - refStr:
|     o c-string to get path to the reference sequence
|   - coordsStr:
|     o c-string to get path to the coordinates file,
|       which also might have the drug resistance for
|       each gene/amplicon
|   - minimap2Str:
|     o c-string to get path for minimap2
|   - argAryStr:
|     o c-string array (as pointers) to add databases to
|   - argLenSIPtr:
|     o number of arguments in argAryStr (index 1)
| Output:
|   - Modifies:
|     o argAryStr to have databases as input arguments
|     o argLenSIPtr to be on next open argument pointer
|     o minimapStr to have path to minimap2 or start with
|       null/0/'\0'
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
getDatabases_ftbRayST(
   signed char *appPathStr, /*path to ftbRay binary*/
   signed char *refStr,     /*gets reference sequence*/
   signed char *coordsStr,  /*gets coordinates file*/
   signed char *minimap2Str,/*gets minimap2 location*/
   signed char **argAryStr, /*gets database paths*/
   signed int *argLenSIPtr  /*number of arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun16 TOC: getDatabases_ftbRayST
   '   - get database files for freezeTB (currently Mac)
   '   o fun16 sec01:
   '     - variable declarations
   '   o fun16 sec02:
   '     - get path to databases
   '   o fun16 sec03:
   '     - get reference file
   '   o fun16 sec02:
   '     - get amrs table (amrs database)
   '   o fun16 sec05:
   '     - get gene coordinates file
   '   o fun16 sec06:
   '     - get MIRU-VNTR lineage table
   '   o fun16 sec07:
   '     - get spoligotype spacer sequences
   '   o fun16 sec08:
   '     - get spoligotype lineage database
   '   o fun16 sec09:
   '     - get hsp65 simple speices database
   '   o fun16 sec10:
   '     - get hsp65 complex speices database
   '   o fun16 sec11:
   '     - find path to minimap2
   '   o fun16 sec12:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int lenSI = 0;
   signed char pathStr[1024];
   signed char *tmpStr = 0;

   #ifdef WINDOWS
      signed char *mapVersionCmdStr =
         (signed char *) "minimap2.exe --version";
      signed char *mapCallCmdStr =
         (signed char *) "minimap2.exe";
   #else
      signed char *mapVersionCmdStr =
         (signed char *) "minimap2 --version";
      signed char *mapCallCmdStr =
         (signed char *) "minimap2";
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec02:
   ^   - get path to databases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef MAC
      lenSI = cpStr_ulCp(pathStr, appPathStr);
      pathStr[lenSI++] = def_pathSep_rayWidg;
      pathStr[lenSI++] = '.';
      pathStr[lenSI++] = '.';
      pathStr[lenSI++] = def_pathSep_rayWidg;
      lenSI +=
         cpStr_ulCp(
            &pathStr[lenSI],
            (signed char *) "Resources"
         );
      pathStr[lenSI++] = def_pathSep_rayWidg;
      pathStr[lenSI] = 0;
   #else
      lenSI = cpStr_ulCp(pathStr, appPathStr);
      pathStr[lenSI++] = def_pathSep_rayWidg;
      lenSI +=
         cpStr_ulCp(
            &pathStr[lenSI],
            (signed char *) "ftbResources"
         );
      pathStr[lenSI++] = def_pathSep_rayWidg;
      pathStr[lenSI] = 0;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec03:
   ^   - get reference file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lenSI = cpStr_ulCp(refStr, pathStr);
   cpStr_ulCp(
      &refStr[lenSI],
      (signed char *) "NC000962.fa"
   );

   argAryStr[*argLenSIPtr] =
      malloc(8 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'r';
   argAryStr[*argLenSIPtr][2] = 'e';
   argAryStr[*argLenSIPtr][3] = 'f';
   argAryStr[*argLenSIPtr][4] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   refPath_freezeTBPaths(argAryStr[*argLenSIPtr]);

   if(! argAryStr[*argLenSIPtr][0])
      cpStr_ulCp(argAryStr[*argLenSIPtr], refStr);
   else
      cpStr_ulCp(refStr, argAryStr[*argLenSIPtr]);

   ++*argLenSIPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec04:
   ^   - get amr table (amrs database)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'a';
   argAryStr[*argLenSIPtr][2] = 'm';
   argAryStr[*argLenSIPtr][3] = 'r';
   argAryStr[*argLenSIPtr][4] = '-';
   argAryStr[*argLenSIPtr][5] = 't';
   argAryStr[*argLenSIPtr][6] = 'b';
   argAryStr[*argLenSIPtr][7] = 'l';
   argAryStr[*argLenSIPtr][8] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;

   amrPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find amr database path*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "amrDb.tsv"
      );
   } /*If: need to find amr database path*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec05:
   ^   - get gene coordinates file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'g';
   argAryStr[*argLenSIPtr][2] = 'e';
   argAryStr[*argLenSIPtr][3] = 'n';
   argAryStr[*argLenSIPtr][4] = 'e';
   argAryStr[*argLenSIPtr][5] = '-';
   argAryStr[*argLenSIPtr][6] = 'c';
   argAryStr[*argLenSIPtr][7] = 'o';
   argAryStr[*argLenSIPtr][8] = 'o';
   argAryStr[*argLenSIPtr][9] = 'r';
   argAryStr[*argLenSIPtr][10] = 'd';
   argAryStr[*argLenSIPtr][11] = 's';
   argAryStr[*argLenSIPtr][12] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;

   coordPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find coordinates file path*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "coords.tsv"
      );
   } /*If: need to find coordinates file path*/

   cpStr_ulCp(coordsStr, argAryStr[*argLenSIPtr]);
   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec06:
   ^   - get MIRU-VNTR lineage table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'm';
   argAryStr[*argLenSIPtr][2] = 'i';
   argAryStr[*argLenSIPtr][3] = 'r';
   argAryStr[*argLenSIPtr][4] = 'u';
   argAryStr[*argLenSIPtr][5] = '-';
   argAryStr[*argLenSIPtr][6] = 't';
   argAryStr[*argLenSIPtr][7] = 'b';
   argAryStr[*argLenSIPtr][8] = 'l';
   argAryStr[*argLenSIPtr][9] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
     goto err_fun16_sec11;

   miruPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find miru lineage*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "miruTbl.tsv"
      );
   } /*If: need to find miru lineage*/
   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec07:
   ^   - get spoligotype spacer sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 's';
   argAryStr[*argLenSIPtr][2] = 'p';
   argAryStr[*argLenSIPtr][3] = 'o';
   argAryStr[*argLenSIPtr][4] = 'l';
   argAryStr[*argLenSIPtr][5] = 'i';
   argAryStr[*argLenSIPtr][6] = 'g';
   argAryStr[*argLenSIPtr][7] = 'o';
   argAryStr[*argLenSIPtr][8] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;

   spolSpacerPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find spoligotype spacer sequences*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "spoligo-spacers.fa"
      );
   } /*If: need to find spoligotype spacer sequences*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec08:
   ^   - get spoligotype lineage database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'd';
   argAryStr[*argLenSIPtr][2] = 'b';
   argAryStr[*argLenSIPtr][3] = '-';
   argAryStr[*argLenSIPtr][4] = 's';
   argAryStr[*argLenSIPtr][5] = 'p';
   argAryStr[*argLenSIPtr][6] = 'o';
   argAryStr[*argLenSIPtr][7] = 'l';
   argAryStr[*argLenSIPtr][8] = 'i';
   argAryStr[*argLenSIPtr][9] = 'g';
   argAryStr[*argLenSIPtr][10] = 'o';
   argAryStr[*argLenSIPtr][11] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
     malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
     goto err_fun16_sec11;

   spolLineagePath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find spoligotype lineage database*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "spoligo-lineages.csv"
      );
   } /*If: need to find spoligotype lineage database*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec09:
   ^   - get hsp65 simple speices database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'h';
   argAryStr[*argLenSIPtr][2] = 's';
   argAryStr[*argLenSIPtr][3] = 'p';
   argAryStr[*argLenSIPtr][4] = '6';
   argAryStr[*argLenSIPtr][5] = '5';
   argAryStr[*argLenSIPtr][6] = '-';
   argAryStr[*argLenSIPtr][7] = 's';
   argAryStr[*argLenSIPtr][8] = 'i';
   argAryStr[*argLenSIPtr][9] = 'm';
   argAryStr[*argLenSIPtr][10] = 'p';
   argAryStr[*argLenSIPtr][11] = 'l';
   argAryStr[*argLenSIPtr][12] = 'e';
   argAryStr[*argLenSIPtr][13] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
     malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
     goto err_fun16_sec11;

   hsp65SimpleDbPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find hsp65 simple lineage database*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "hsp65-db-simple.tsv"
      );
   } /*If: need to find hsp65 simple lineage database*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec10:
   ^   - get hsp65 complex speices database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun16_sec11;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'h';
   argAryStr[*argLenSIPtr][2] = 's';
   argAryStr[*argLenSIPtr][3] = 'p';
   argAryStr[*argLenSIPtr][4] = '6';
   argAryStr[*argLenSIPtr][5] = '5';
   argAryStr[*argLenSIPtr][6] = '-';
   argAryStr[*argLenSIPtr][7] = 'c';
   argAryStr[*argLenSIPtr][8] = 'o';
   argAryStr[*argLenSIPtr][9] = 'm';
   argAryStr[*argLenSIPtr][10] = 'p';
   argAryStr[*argLenSIPtr][11] = 'l';
   argAryStr[*argLenSIPtr][12] = 'e';
   argAryStr[*argLenSIPtr][13] = 'x';
   argAryStr[*argLenSIPtr][14] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
     malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
     goto err_fun16_sec11;

   hsp65ComplexDbPath_freezeTBPaths(
      argAryStr[*argLenSIPtr]
   );

   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find hsp65 complex lineage database*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "hsp65-db-complex.tsv"
      );
   } /*If: need to find hsp65 complex lineage database*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec11:
   ^   - find path to minimap2
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpStr_ulCp(minimap2Str, mapVersionCmdStr);

   if( ! system((char *) minimap2Str) )
      cpStr_ulCp(minimap2Str, mapCallCmdStr);
   else
   { /*Else: minimap2 is not in the path*/
      lenSI = cpStr_ulCp(minimap2Str, pathStr);
      cpStr_ulCp(&minimap2Str[lenSI], mapVersionCmdStr);

      if( system((char *) minimap2Str) )
      { /*If: failed to find minimap2*/
         checkPaths_freezeTBPaths(
            minimap2Str,
            mapCallCmdStr
         );

         if(! minimap2Str[0])
            minimap2Str[0] = 0;
         else
         { /*Else: found minimap2 file; check if runs*/
            tmpStr = minimap2Str;
            while(*tmpStr)
               ++tmpStr;
            cpStr_ulCp(
               tmpStr,
               (signed char *) "--version"
            );

            if( system((char *) minimap2Str) )
               minimap2Str[0] = 0;/*minimap2 did not run*/
            else
               *tmpStr = 0; /*remove --version flag*/
         } /*Else: found minimap2 file; check if runs*/
      } /*If: failed to find minimap2*/

      else
      { /*Else: found minimap2*/
         /*find space between minimap2 & --version*/
         while(minimap2Str[lenSI] > 32)
            ++lenSI;
         minimap2Str[lenSI] = 0;
      } /*Else: found minimap2*/
   } /*Else: minimap2 is not in the path*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec12:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   err_fun16_sec11:;
      return 1;
} /*getDatbases_ftbRayST*/

/*-------------------------------------------------------\
| Fun17: mkCoverageTbl_ftbRayST
|   - makes the gene percent coverage table
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui settings and
|       prefix of output files
| Output:
|   - Modifies:
|     o gui to have the gene percent coverage table
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
|     o 2 if could not open the coordinates file
\-------------------------------------------------------*/
signed int
mkCoverageTbl_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - makes the gene percent coverage table
   '   o fun17 sec01:
   '     - variable declarations
   '   o fun17 sec02:
   '     - get maximum column sizes
   '   o fun17 sec03:
   '     - build the header for the table
   '   o fun17 sec04:
   '     - build the table
   '   o fun17 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char lineStr[4096];
   signed int linePosSI = 0;

   signed char outStr[4096];
   signed int outPosSI = 0;
   signed int tmpSI = 0;

   signed int colArySI[256]; /*main column sizes*/
   signed int colPosSI = 0;
   signed int lastColSI = 0;

   FILE *inFILE = 0;

   signed int lineSI = 0;

   clear_listBox_rayWidg(guiSTPtr->geneCoverSTPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - get maximum column sizes
   ^   o fun17 sec02 sub01:
   ^     - open the amr coverage file
   ^   o fun17 sec02 sub02:
   ^     - find column sizes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun17 Sec02 Sub01:
   *   - open the amr coverage file
   \*****************************************************/

   for(tmpSI = 0; tmpSI < 256; ++tmpSI)
      colArySI[tmpSI] = 0;
   tmpSI = 0;

   linePosSI =
      cpStr_ulCp(lineStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &lineStr[linePosSI],
      (signed char *) "-coverage.tsv"
   );

   inFILE = fopen((char *) lineStr, "r");
   if(! inFILE)
      goto fileErr_fun17_sec05;
   if(! fgets((char *) lineStr, 4088, inFILE) )
      goto fileErr_fun17_sec05;

   /*set the header sizes*/
   colArySI[0] = 4; /*gene*/
   colArySI[1] = 10; /*perc_cover*/
   colArySI[2] = 5; /*depth*/

   /*****************************************************\
   * Fun17 Sec02 Sub02:
   *   - find column sizes
   \*****************************************************/

   lineSI = 0;
   if(! fgets((char *) lineStr, 4088, inFILE))
      goto blankFile_fun17_sec05;

   while(fgets((char *) lineStr, 4088, inFILE))
   { /*Loop: find column sizes*/
      linePosSI = 0;
      colPosSI = 0;

      /*____________get_gene_id_size____________________*/
      outPosSI = endWhite_ulCp(&lineStr[linePosSI]);
      if(outPosSI > colArySI[colPosSI])
         colArySI[colPosSI] = outPosSI;
      ++colPosSI;

      linePosSI += outPosSI;
      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      if(! linePosSI)
         continue; /*end of line*/

      /*____________get_gene_percent_coverage___________*/
      outPosSI = endWhite_ulCp(&lineStr[linePosSI]);
      if(outPosSI > colArySI[colPosSI])
         colArySI[colPosSI] = outPosSI;
      ++colPosSI;

      linePosSI += outPosSI;
      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      if(! linePosSI)
         goto fileErr_fun17_sec05;

      /*____________get_gene_mean_coverage_depth________*/
      outPosSI = endWhite_ulCp(&lineStr[linePosSI]);
      if(outPosSI > colArySI[colPosSI])
         colArySI[colPosSI] = outPosSI;

      ++colPosSI;

      linePosSI += outPosSI;
      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      if(! linePosSI)
         goto fileErr_fun17_sec05;

      /*____________skip_gene_mean_target_depth_________*/
      linePosSI += endWhite_ulCp(&lineStr[linePosSI]);

      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      if(! linePosSI)
         goto fileErr_fun17_sec05;

      /*____________get_drug_names______________________*/
      while(
         lineStr[linePosSI] && lineStr[linePosSI] != '*'
      ){ /*Loop: find drug column lengths*/
         outPosSI = endWhite_ulCp(&lineStr[linePosSI]);
         if(outPosSI > colArySI[colPosSI])
            colArySI[colPosSI] = outPosSI;
         if(colArySI[colPosSI] < 6)
            colArySI[colPosSI] = 6;
         ++colPosSI;

         linePosSI += outPosSI;
         while(
            lineStr[linePosSI] && lineStr[linePosSI] < 33
         ) ++linePosSI;
      }  /*Loop: find drug column lengths*/

      if(colPosSI > lastColSI - 1)
         lastColSI = colPosSI - 1;
      ++lineSI;
   } /*Loop: find column sizes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - build the header for the table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add in the padding*/
   colArySI[0] += 1;
   colArySI[1] += 1;
   colArySI[2] += 1;

   colPosSI = 0;
   outPosSI = 0;

   /*________________gene_name__________________________*/
   lineSI = cpStr_ulCp(outStr, (signed char *) "gene");
   outPosSI += lineSI;
   while(lineSI <= colArySI[0])
   { /*Loop: add in spaces*/
      outStr[outPosSI++] = ' ';
      ++lineSI;
   } /*Loop: add in spaces*/

   /*________________percent_coverage___________________*/
   lineSI =
      cpStr_ulCp(
         &outStr[outPosSI],
         (signed char *) "perc_cover"
      );
   outPosSI += lineSI;
   while(lineSI <= colArySI[1])
   { /*Loop: add in spaces*/
      outStr[outPosSI++] = ' ';
      ++lineSI;
   } /*Loop: add in spaces*/

   /*________________mean_depth_________________________*/
   lineSI =
      cpStr_ulCp(
         &outStr[outPosSI],
         (signed char *) "depth"
      );
   outPosSI += lineSI;
   while(lineSI <= colArySI[2])
   { /*Loop: add in spaces*/
      outStr[outPosSI++] = ' ';
      ++lineSI;
   } /*Loop: add in spaces*/

   
   /*________________drug_columns_______________________*/
   for(colPosSI = 3; colPosSI <= lastColSI; ++colPosSI)
   { /*Loop: print 'na' for no drug columns*/
      tmpSI = 0;

      outStr[outPosSI++] = 'd';
      outStr[outPosSI++] = 'r';
      outStr[outPosSI++] = 'u';
      outStr[outPosSI++] = 'g';
      outStr[outPosSI++] = '_';
      tmpSI += numToStr(&outStr[outPosSI], colPosSI - 3);
      outPosSI += tmpSI;
      tmpSI += 5; /*account for "drug_"*/

      while(tmpSI <= colArySI[colPosSI])
      { /*Loop: pad column if is to short*/
         outStr[outPosSI++] = ' ';
         ++tmpSI;
      } /*Loop: pad column if is to short*/
   } /*Loop: print 'na' for no drug columns*/

   outStr[outPosSI] = 0;
   if(
      addItem_listBox_rayWidg(
         outStr,
         def_listSpecial_rayWidg, /*so does nothing*/
         guiSTPtr->geneCoverSTPtr,
         guiSTPtr->widgSTPtr
      )
   ) goto memErr_fun17_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - build the table
   ^   o fun17 sec04 sub01:
   ^     - move to the start of coverage table and start
   ^       read loop
   ^   o fun17 sec04 sub02:
   ^     - get the gene name
   ^   o fun17 sec04 sub03:
   ^     - get percent coverage of the gene
   ^   o fun17 sec04 sub04:
   ^     - get mean covered region read depth
   ^   o fun17 sec04 sub05:
   ^     - skip gene mean read depth column
   ^     - this is the mean read depth for the entire
   ^       gene/target instead of the region that had
   ^       good read depth
   ^   o fun17 sec04 sub06:
   ^     - get the name for each drug
   ^   o fun17 sec04 sub07:
   ^     - add row to table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun17 Sec04 Sub01:
   *   - move to the start of coverage table and start
   *     read loop
   \*****************************************************/

   fseek(inFILE, 0, SEEK_SET);
   lineSI = 0;

   if( ! fgets((char *) lineStr, 4088, inFILE) )
      lineSI = 0;
        /*this should never happen since I already
        `  checked for this
        */

   while( fgets((char *) lineStr, 4088, inFILE) )
   { /*Loop: read in the gene/amplicons*/
      linePosSI = 0;
      outPosSI = 0;
      colPosSI = 0;

      /**************************************************\
      * Fun17 Sec04 Sub02:
      *   - get the gene name
      \**************************************************/

      tmpSI =
         cpWhite_ulCp(
            &outStr[outPosSI],
            &lineStr[linePosSI]
         );
      outPosSI += tmpSI;
      linePosSI += tmpSI;

      while(tmpSI <= colArySI[colPosSI])
      { /*Loop: pad column if is to short*/
         outStr[outPosSI++] = ' ';
         ++tmpSI;
      } /*Loop: pad column if is to short*/

      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      ++colPosSI;

      /**************************************************\
      * Fun17 Sec04 Sub03:
      *   - get percent coverage of the gene
      \**************************************************/

      tmpSI =
         cpWhite_ulCp(
            &outStr[outPosSI],
            &lineStr[linePosSI]
         );
      outPosSI += tmpSI;
      linePosSI += tmpSI;

      while(tmpSI <= colArySI[colPosSI])
      { /*Loop: pad column if is to short*/
         outStr[outPosSI++] = ' ';
         ++tmpSI;
      } /*Loop: pad column if is to short*/

      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      ++colPosSI;

      /**************************************************\
      * Fun17 Sec04 Sub04:
      *   - get mean covered region read depth
      \**************************************************/

      tmpSI =
         cpWhite_ulCp(
            &outStr[outPosSI],
            &lineStr[linePosSI]
         );
      linePosSI += tmpSI;

      outPosSI += tmpSI;

      while(tmpSI <= colArySI[colPosSI])
      { /*Loop: pad column if is to short*/
         outStr[outPosSI++] = ' ';
         ++tmpSI;
      } /*Loop: pad column if is to short*/

      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;
      ++colPosSI;

      /**************************************************\
      * Fun17 Sec04 Sub05:
      *   - skip gene mean read depth column
      *   - this is the mean read depth for the entire
      *     gene/target instead of the region that had
      *     good read depth
      \**************************************************/

      linePosSI += endWhite_ulCp(&lineStr[linePosSI]);
      while(lineStr[linePosSI] && lineStr[linePosSI] < 33)
         ++linePosSI;

      /**************************************************\
      * Fun17 Sec04 Sub06:
      *   - get the name for each drug
      \**************************************************/

      while(
        lineStr[linePosSI] && lineStr[linePosSI] != '*'
      ){ /*Loop: find drug column lengths*/
         tmpSI =
            cpWhite_ulCp(
               &outStr[outPosSI],
               &lineStr[linePosSI]
            );
         outPosSI += tmpSI;
         linePosSI += tmpSI;

         while(tmpSI <= colArySI[colPosSI])
         { /*Loop: pad column if is to short*/
            outStr[outPosSI++] = ' ';
            ++tmpSI;
         } /*Loop: pad column if is to short*/

         while(
               lineStr[linePosSI]
            && lineStr[linePosSI] < 33
         ) ++linePosSI;

         ++colPosSI;
      }  /*Loop: find drug column lengths*/

      /*__________add_na's_for_empty_columns_________*/
      while(colPosSI <= lastColSI)
      { /*Loop: print 'na' for no drug columns*/
         ++colPosSI;
         outStr[outPosSI++] = 'n';
         outStr[outPosSI++] = 'a';

         tmpSI = 2;

         while(tmpSI <= colArySI[colPosSI])
         { /*Loop: pad column if is to short*/
            outStr[outPosSI++] = ' ';
            ++tmpSI;
         } /*Loop: pad column if is to short*/
      } /*Loop: print 'na' for no drug columns*/

      /**************************************************\
      * Fun17 Sec04 Sub07:
      *   - add row to table
      \**************************************************/

      outStr[outPosSI] = 0;

      if(
         addItem_listBox_rayWidg(
            outStr,
            def_listSpecial_rayWidg, /*so does nothing*/
            guiSTPtr->geneCoverSTPtr,
            guiSTPtr->widgSTPtr
         )
      ) goto memErr_fun17_sec05;
 
      ++lineSI;
   } /*Loop: read in the gene/amplicons*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineSI = 0;
   goto ret_fun17_sec05;

   memErr_fun17_sec05:;
      lineSI = 1;
      goto ret_fun17_sec05;

   fileErr_fun17_sec05:;
      lineSI = 2;
      goto ret_fun17_sec05;

   blankFile_fun17_sec05:;
      lineSI = 0;

      /*add in the header quickly*/
      cpStr_ulCp(
         lineStr,
         (signed char *) "gene perc_cover depth drug_1"
      );
      if(
         addItem_listBox_rayWidg(
            lineStr,
            def_listSpecial_rayWidg, /*so does nothing*/
            guiSTPtr->geneCoverSTPtr,
            guiSTPtr->widgSTPtr
         )
      ) goto memErr_fun17_sec05;

      else
         goto ret_fun17_sec05;

   ret_fun17_sec05:;
      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      return (signed char) lineSI;
} /*mkCoverageTbl_ftbRayST*/

/*-------------------------------------------------------\
| Fun18: getHsp65Lin_ftbRayST
|   - get the getLin hsp65 lineages (an others)
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct with lineage list box
| Output:
|   - Modifies:
|     - hsp65ListBox list box widget to have the lineages
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
getHsp65Lin_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - get the getLin hsp65 lineages (an others)
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec02:
   '     - open the hsp65 read lineage file output and
   '       read the three lines in the file
   '   o fun18 sec03:
   '     - get id and move past id and type column
   '   o fun18 sec04:
   '     - extract the lineages from the file and add to
   '       the table
   '   o fun18 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*for reading in the database*/
   #define def_lenLine_fun18 (2 << 10) /*about 4kb*/

   /*variables for processing the three lines*/
   signed char firstLineStr[def_lenLine_fun18];
   signed char *firstStr = 0;
   signed int firstMaxLenSI = 0;
   signed char *firstEndStr = 0;

   signed char secLineStr[def_lenLine_fun18];
   signed char *secStr = 0;
   signed int secMaxLenSI = 0;
   signed char *secEndStr = 0;
 
   signed char thirdLineStr[def_lenLine_fun18];
   signed char *thirdStr = 0;

   /*variables for printing to table*/
   signed char outLineStr[256];
   signed char *outStr = 0;

   /*temporary or general variables*/
   signed int lenSI = 0;
   signed long ignoreSL = 0;

   FILE *hsp65FILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - open the hsp65 read lineage file output and
   ^     read the three lines in the file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   clear_listBox_rayWidg(guiSTPtr->hsp65ListSTPtr);

   if(! guiSTPtr->filePrefixStr[0])
      goto done_fun18_sec05;

   secStr = firstLineStr;
   secStr += cpStr_ulCp(secStr, guiSTPtr->filePrefixStr);
   secStr +=
     cpStr_ulCp(secStr,(signed char *) "-read-hsp65.tsv");

   hsp65FILE = fopen((char *) firstLineStr, "r");
   if(! hsp65FILE)
      goto done_fun18_sec05;

   lenSI =
      getLine_fileFun(
         hsp65FILE,
         firstLineStr,
         def_lenLine_fun18,
         &ignoreSL
      );
   if(lenSI <= 0)
      goto done_fun18_sec05;
   firstStr = firstLineStr;

   lenSI =
      getLine_fileFun(
         hsp65FILE,
         secLineStr,
         def_lenLine_fun18,
         &ignoreSL
      );
   if(lenSI <= 0)
      goto done_fun18_sec05;
   secStr = secLineStr;

   lenSI =
      getLine_fileFun(
         hsp65FILE,
         thirdLineStr,
         def_lenLine_fun18,
         &ignoreSL
      );
   if(lenSI <= 0)
      goto done_fun18_sec05;
   thirdStr = thirdLineStr;

   fclose(hsp65FILE);
   hsp65FILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - move past id and type column
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   outStr = outLineStr;

   /*get past id column*/
   firstStr += endWhite_ulCp(firstStr);
   secStr += endWhite_ulCp(secStr);
   thirdStr += endWhite_ulCp(thirdStr);
   if(! *firstStr || ! *secStr || ! *thirdStr)
      goto done_fun18_sec05;

   /*get off the tab and white space*/
   while(*firstStr && *firstStr < 33)
      ++firstStr;
   while(*secStr && *secStr < 33)
      ++secStr;
   while(*thirdStr && *thirdStr < 33)
      ++thirdStr;

   /*get of the type column*/
   firstStr += endWhite_ulCp(firstStr);
   secStr += endWhite_ulCp(secStr);
   thirdStr += endWhite_ulCp(thirdStr);
   if(! *firstStr || ! *secStr || ! *thirdStr)
      goto done_fun18_sec05;

   /*get off the tab and white space*/
   while(*firstStr && *firstStr < 33)
      ++firstStr;
   while(*secStr && *secStr < 33)
      ++secStr;
   while(*thirdStr && *thirdStr < 33)
      ++thirdStr;
   if(! *firstStr || ! *secStr || ! *thirdStr)
      goto done_fun18_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - extract the lineages from the file and add to
   ^     the table
   ^   o fun18 sec04 sub01:
   ^     - find the maximum length of each column and
   ^       build the header
   ^   o fun18 sec04 sub02:
   ^     - start lineage loop + get next lineage, counts,
   ^       and status from the
   ^   o fun18 sec04 sub03:
   ^     - add in call for the lineage
   ^   o fun18 sec04 sub04:
   ^     - add lineage to the list box and move off white
   ^       space
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec04 Sub01:
   *   - find the maximum length of each column and build
   *     the header
   \*****************************************************/

   /*___________find_column_lengths_____________________*/
   firstEndStr = firstStr;
   secEndStr = secStr;

   while(*secEndStr && *secEndStr != '*')
   { /*Loop: find longest entry for each column*/
      lenSI = endWhite_ulCp(firstEndStr);
      if(lenSI > firstMaxLenSI)
         firstMaxLenSI = lenSI;
      firstEndStr += lenSI;

      lenSI = endWhite_ulCp(secEndStr);
      if(lenSI > secMaxLenSI)
         secMaxLenSI = lenSI;
      secEndStr += lenSI;

      while(*firstEndStr && *firstEndStr < 33)
         ++firstEndStr;
      while(*secEndStr && *secEndStr < 33)
         ++secEndStr;
   } /*Loop: find longest entry for each column*/

   outStr = outLineStr;

   /*___________build_the_header________________________*/
   if(firstMaxLenSI < 9)
      firstMaxLenSI = 9;
   else
      firstMaxLenSI += 2;
   lenSI = cpStr_ulCp(outStr, (signed char *) "lineage");
   outStr += lenSI;
   for( ; lenSI <= firstMaxLenSI; ++lenSI)
      *outStr++ = ' ';
   *outStr = 0;

   if(secMaxLenSI < 7)
      secMaxLenSI = 7;
   else
      secMaxLenSI += 2;
   lenSI = cpStr_ulCp(outStr, (signed char *) "depth");
   outStr += lenSI;
   for( ; lenSI <= secMaxLenSI; ++lenSI)
      *outStr++ = ' ';
   *outStr = 0;

   lenSI = cpStr_ulCp(outStr, (signed char *) "call");
   outStr += lenSI;

   /*___________add_header_to_the_table_________________*/
   if(
      addItem_listBox_rayWidg(
         outLineStr,
         def_listSpecial_rayWidg, /*so does nothing*/
         guiSTPtr->hsp65ListSTPtr,
         guiSTPtr->widgSTPtr
      )
   ) goto memErr_fun18_sec05;

   /*****************************************************\
   * Fun18 Sec04 Sub02:
   *   - start lineage loop + get next lineage, counts,
   *     and status from the
   \*****************************************************/

   while(secStr != secEndStr)
   { /*Loop: build the table*/
      outStr = outLineStr;

      /*______________add_lineage_to_entry______________*/
      lenSI = cpWhite_ulCp(outStr, firstStr);
      firstStr += lenSI;
      outStr += lenSI;

      while(lenSI <= firstMaxLenSI)
      { /*Loop: add padding for the lineage*/
         *outStr++ = ' ';
         ++lenSI;
      } /*Loop: add padding for the lineage*/
      *outStr = 0;


      /*______________add_depth_to_entry________________*/
      lenSI = cpWhite_ulCp(outStr, secStr);
      secStr += lenSI;
      outStr += lenSI;
      while(lenSI <= secMaxLenSI)
      { /*Loop: add padding for the number*/
         *outStr++ = ' ';
         ++lenSI;
      } /*Loop: add padding for the number*/
      lenSI = 0;
      *outStr = 0;

      /**************************************************\
      * Fun18 Sec04 Sub03:
      *   - add in call for the lineage
      \**************************************************/

      if(thirdStr[0] == 'L')
         cpStr_ulCp(
            outStr,
            (signed char *) "low_depth"
         );

      else if(thirdStr[0] == 'N')
         cpStr_ulCp(
            outStr,
            (signed char *) "alternate"
         );

      else if(thirdStr[0] == 'M')
         cpStr_ulCp(
            outStr,
            (signed char *) "mixed_lineage"
         );

      else
         cpStr_ulCp(
            outStr,
            (signed char *) "supported"
         );

      /**************************************************\
      * Fun18 Sec04 Sub04:
      *   - add lineage to the list box and move off white
      *     space
      \**************************************************/

      if(
         addItem_listBox_rayWidg(
            outLineStr,
            def_listSpecial_rayWidg, /*so does nothing*/
            guiSTPtr->hsp65ListSTPtr,
            guiSTPtr->widgSTPtr
         )
      ) goto memErr_fun18_sec05;

      /*get off the tab and white space*/
      while(*firstStr && *firstStr < 33)
         ++firstStr;
      while(*secStr && *secStr < 33)
         ++secStr;
      while(*thirdStr && *thirdStr < 33)
         ++thirdStr;
   } /*Loop: build the table*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sort_listBox_rayWidg(guiSTPtr->hsp65ListSTPtr);

   done_fun18_sec05:;
      lenSI = 0;
      goto ret_fun18_sec05;

   memErr_fun18_sec05:;
      lenSI = 1;
      goto ret_fun18_sec05;

   ret_fun18_sec05:;
      if(! hsp65FILE)
         ;
      else
         fclose(hsp65FILE);
      hsp65FILE = 0;

      return (signed char) lenSI;
} /*getHsp65Lin_ftbRayST*/

/*-------------------------------------------------------\
| Fun19: checkRunEvent_ftbRayST
|   - checks for an event, and if can runs the found event
|   - also redraws the GUI
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Draws:
|     o current GUI state during and after event to screen
|   - Modifies:
|     o state and other widget related variables in
|       guiSTPtr
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
checkRunEvent_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - checks for an event, and if can runs found event
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - get and check events
   '   o fun19 sec06:
   '     - handle running button events
   '   o fun19 sec07:
   '     - return results and redraw gui
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;
   signed char *tmpHeapStr = 0;
   signed char buildReportBl = 0;

   /*for reading the config file*/
   #define def_lineLen_fun19 1024
   signed char lineStr[def_lineLen_fun19 + 8];
   signed char logFileStr[def_lineLen_fun19 + 8];
   FILE *inFILE = 0;
   signed long discardSL = 0; /*for reading files*/

   signed char refStr[def_lineLen_fun19 + 8];
   signed char coordsStr[def_lineLen_fun19 + 8];
   signed char minimap2Str[def_lineLen_fun19 + 8];

   /*for buiding freezeTB run command*/
   signed char *argAryStr[1024];
      /*just give more memory then I will ever need*/
   signed int argLenSI = 0;

   struct event_rayWidg eventStackST;
   signed int indexSI = 0;
   signed int tmpSI = 0;
   signed int siCnt = 0;

   FILE *tmpFILE = 0;
   struct files_rayWidg *fileSTPtr = 0;

   for(tmpSI = 0; tmpSI < 1024; ++tmpSI)
      argAryStr[tmpSI] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - get and check events
   ^   o fun19 sec02 sub01:
   ^     - get event and check entery event
   ^   o fun19 sec02 sub02:
   ^     - check which event I am running
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec02 Sub01:
   *   - get event and check entery event
   \*****************************************************/

   init_event_rayWidg(&eventStackST);

   indexSI =
      get_event_rayWidg(
         1,                 /*handel focus changes*/
         draw_gui_ftbRayST, /*redraw GUI for presses*/
         guiSTPtr,          /*has GUI to redraw*/
         def_resizeState_ftbRayST, /*when to resize*/
         &eventStackST,     /*has events*/
         guiSTPtr->widgSTPtr/*has widgets*/
      );

   glob_maxWidgWidthSI = 240;
   glob_widthPrefixEntrySI = def_entryWidth_ftbRayST;

   if(guiSTPtr->widgSTPtr->xScaleF > 1)
   { /*If: need to scale widget size*/
      glob_maxWidgWidthSI *= guiSTPtr->widgSTPtr->xScaleF;
      glob_widthPrefixEntrySI *=
         guiSTPtr->widgSTPtr->xScaleF;
   } /*If: need to scale widget size*/

   /*not worried, bucause this will check if the entry
   `  box id matches the found id or is not active. So, it
   `  does nothing if not on an entry box
   */
   tmpSI =
      entryEvent_rayWidg(
         guiSTPtr->prefixEntryIdSI,/*id of entry box*/
         guiSTPtr->prefixPosArySI,/*cursor/scroll pos*/
         guiSTPtr->inPrefixStr,   /*text in entry box*/
         guiSTPtr->prefixLenSI,   /*string length*/
         127,                     /*max string length*/
         fileStrCheck_rayWidg,    /*check function*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->prefixLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/


   /*entry box's for getting AMR support settings for
   `  buildind the output
   */
   tmpSI =
      entryEvent_rayWidg(
         guiSTPtr->minAmrPercEntryIdSI,/*id of entry box*/
         guiSTPtr->amrSupPosArySI,/*cursor/scroll pos*/
         guiSTPtr->amrSupStr,   /*text in entry box*/
         guiSTPtr->amrSupLenSI,   /*string length*/
         100,                   /*max percent allowd*/
         floatStrCheck_rayWidg,    /*check function*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->amrSupLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/

   tmpSI =
      entryEvent_rayWidg(
         guiSTPtr->minAmrIndelPercEntryIdSI,
         guiSTPtr->indelSupPosArySI,/*cursor/scroll pos*/
         guiSTPtr->indelSupStr,   /*text in entry box*/
         guiSTPtr->indelSupLenSI,   /*string length*/
         100,                   /*max percent allowd*/
         floatStrCheck_rayWidg,    /*check function*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->indelSupLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/

   tmpSI =
      listBoxEvent_rayWidg(
         guiSTPtr->amrTblIdSI,   /*id of list box*/
         guiSTPtr->amrListSTPtr, /*has list box items*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->prefixLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/

   tmpSI =
      listBoxEvent_rayWidg(
         guiSTPtr->hsp65TblIdSI,   /*id of list box*/
         guiSTPtr->hsp65ListSTPtr, /*has list box items*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->prefixLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/

   tmpSI =
      listBoxEvent_rayWidg(
         guiSTPtr->geneCoverTblIdSI,   /*id of list box*/
         guiSTPtr->geneCoverSTPtr, /*has list box items*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->prefixLenSI = tmpSI;
      goto done_fun19_sec07;
   } /*If: was a entry box input event*/

   /*****************************************************\
   * Fun19 Sec02 Sub02:
   *   - check which event I am running
   \*****************************************************/

   if(eventStackST.idSI == guiSTPtr->fqButIdSI)
      goto getFqFiles_fun19_sec06_sub02;

   else if(eventStackST.idSI == guiSTPtr->outDirIdSI)
      goto getOutDir_fun19_sec06_sub03;

   else if(eventStackST.idSI == guiSTPtr->configIdSI)
      goto getConfigFile_fun19_sec06_sub04;

   else if(eventStackST.idSI == guiSTPtr->runIdSI)
      goto runFtb_fun19_sec06_sub06;

   else if(
      eventStackST.idSI == guiSTPtr->reportGuiIdSI
   ) goto reportMenu_fun19_sec06_sub10;

   else if(eventStackST.idSI==guiSTPtr->inputGuiIdSI)
      goto inputMenu_fun19_sec06_sub07;

   else if(eventStackST.idSI == guiSTPtr->outGuiIdSI)
      goto outputMenu_fun19_sec06_sub08;

   else if(eventStackST.idSI == guiSTPtr->amrsGuiIdSI)
      goto amrTblMenu_fun19_sec06_sub12;

   else if(eventStackST.idSI == guiSTPtr->getOutButSI)
      goto buildOutReport_fun19_sec06_sub0x;

   else if(eventStackST.idSI == guiSTPtr->hsp65GuiIdSI)
      goto hsp65TblMenu_fun19_sec06_sub12;

   else if(eventStackST.idSI == guiSTPtr->coverGuiIdSI)
      goto geneCoverMenu_fun19_sec06_sub13;

   else if(eventStackST.idSI==guiSTPtr->getPrefixButIdSI)
       goto getFtbPrefix_fun19_sec06_sub11;

   else if(eventStackST.idSI == guiSTPtr->getOutButSI)
      goto buildOutReport_fun19_sec06_sub0x;

   else if(eventStackST.parIdSI == guiSTPtr->mesgBoxIdSI)
      goto mesgBox_fun19_sec06_sub01;

   else if(
      eventStackST.parIdSI == guiSTPtr->fileBrowserIdSI
   ) goto fileBrowser_fun19_sec06_sub05;

   goto done_fun19_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec06:
   ^   - handle running button events
   ^   o fun19 sec06 sub01:
   ^     - message box event
   ^   o fun19 sec06 sub02:
   ^     - get fastq files event
   ^   o fun19 sec06 sub03:
   ^     - get output directory event
   ^   o fun19 sec06 sub04:
   ^     - get configuration file event
   ^   o fun19 sec06 sub05:
   ^     - file browser event actions
   ^   o fun19 sec06 sub06:
   ^     - run event actions
   ^   o fun19 sec06 sub07:
   ^     - button pressed to build the output report
   ^   o fun19 sec06 sub08:
   ^     - goto output menu
   ^   o fun15 sec04 sub09:
   ^     - build amr table entry
   ^   o fun19 sec06 sub10:
   ^     - got to report
   ^   o fun19 sec06 sub11:
   ^     - get ftb prefix
   ^   o fun19 sec06 sub12:
   ^     - goto to the amr table
   ^   o fun19 sec06 sub13:
   ^     - goto to the hsp65 species/lineage table
   ^   o fun19 sec06 sub14:
   ^     - goto to gene coverage table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec06 Sub01:
   *   - message box event
   \*****************************************************/

   mesgBox_fun19_sec06_sub01:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      mesgBoxEvent_rayWidg(
         2, /*rease key event*/
         eventStackST.parIdSI,
         eventStackST.idSI,
         guiSTPtr->widgSTPtr
      );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub02:
   *   - get fastq files event
   \*****************************************************/

   getFqFiles_fun19_sec06_sub02:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 0;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select fastq files to run"
      );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub03:
   *   - get output directory event
   \*****************************************************/

   getOutDir_fun19_sec06_sub03:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 1;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select output folder"
      );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub04:
   *   - get configuration file event
   \*****************************************************/

   getConfigFile_fun19_sec06_sub04:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 2;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select FTB configuration file"
      );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub05:
   *   - file browser event actions
   *   o fun19 sec06 sub05 cat01:
   *     - run file brower event
   *   o fun19 sec06 sub05 cat02:
   *     - cancel event
   *   o fun19 sec06 sub05 cat03:
   *     - selected or cleared files
   *   o fun19 sec06 sub05 cat04:
   *     - selected fastq files
   *   o fun19 sec06 sub05 cat05:
   *     - get file and find browser used
   *   o fun19 sec06 sub05 cat06:
   *     - add file to the browswer (or clear)
   *   o fun19 sec06 sub05 cat07:
   *     - error or no event
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec06 Sub05 Cat01:
   +   - run file brower event
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fileBrowser_fun19_sec06_sub05:;
      switch(guiSTPtr->browserSC)
      { /*Switch: find which browser using*/
         case 0: fileSTPtr = guiSTPtr->fqFileSTPtr;
                 break;
         case 1: fileSTPtr = guiSTPtr->outDirSTPtr;
                 break;
         case 2: fileSTPtr = guiSTPtr->configFileSTPtr;
                 break;
         case 3: fileSTPtr = guiSTPtr->oldFtbFileSTPtr;
                 break;

         default: goto done_fun19_sec07;
           /*invalid option*/
      } /*Switch: find which browser using*/

      tmpSI =
         fileBrowserEvent_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            &eventStackST,
            fileSTPtr,
            guiSTPtr->widgSTPtr
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub05 Cat02:
      +   - cancel event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpSI == 2)
      { /*If: hit cancel*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
          ); /*use hit cancel*/

          goto done_fun19_sec07;
      } /*If: hit cancel*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub05 Cat03:
      +   - selected or clear files
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI > 0)
      { /*Else If: files were selected or cleared*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
         ); /*user hit select*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub05 Cat04:
         +   - selected fastq files
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(! guiSTPtr->browserSC)
         { /*If: fastq files input*/
            if(tmpSI == 3)
               blank_str_ptrAry(guiSTPtr->fqStrSTPtr);
               /*clearning all fastq files*/

            else
            { /*Else: getting fastq files*/
               tmpSI = 0;

               while(tmpSI >= 0)
               { /*Loop: get fastq files*/
                  tmpHeapStr =
                     getFile_files_rayWidg(
                        &tmpSI,
                        0, /*only selected items*/
                        fileSTPtr
                     );
                  if(tmpSI < 0)
                      break;
                  else if(! tmpHeapStr)
                     goto err_fun19_sec07;

                  if(
                     add_str_ptrAry(
                        tmpHeapStr,
                        guiSTPtr->fqStrSTPtr,
                        guiSTPtr->fqStrSTPtr->lenSL
                     )
                  ) goto err_fun19_sec07;

                  free(tmpHeapStr);
                  tmpHeapStr = 0;
               } /*Loop: get fastq files*/
            } /*Else: getting fastq files*/

            goto done_fun19_sec07;
         } /*If: fastq files input*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub05 Cat05:
         +   - get file and find browser used
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(tmpSI == 1)
         { /*If: files were selected*/
            tmpHeapStr =
               getFile_files_rayWidg(
                  &tmpSI,
                  0, /*only selected items*/
                  fileSTPtr
               );
            if(! tmpHeapStr)
               goto err_fun19_sec07;
         } /*If: files were selected*/

         switch(guiSTPtr->browserSC)
         { /*Switch: find which browser using*/
            case 1: /*Case: output directory selected*/
               tmpStr = guiSTPtr->outDirStr;
               break;

            case 2: /*Case: configuration file selected*/
               tmpStr = guiSTPtr->configFileStr;
               break;

            case 3: /*Case: select old ftb prefix*/
               tmpStr = guiSTPtr->filePrefixStr;
               break;
         } /*Switch: find which browser using*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub05 Cat06:
         +   - add file to browser or clear
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(tmpSI == 3)
            tmpStr[0] = 0;
         else
            cpStr_ulCp(tmpStr, tmpHeapStr);

         if(tmpHeapStr)
            free(tmpHeapStr);
         tmpHeapStr = 0;

         if(guiSTPtr->browserSC == 3)
         { /*If: need to remove the suffix*/
            tmpHeapStr = guiSTPtr->filePrefixStr;
            while(*tmpHeapStr != '-')
               ++tmpHeapStr;
            *tmpHeapStr = 0;
            tmpHeapStr = 0;
         } /*If: need to remove the suffix*/

         goto done_fun19_sec07;
      } /*Else If: files were selected or cleared*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub05 Cat07:
      +   - error or no event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI < -2)
         goto err_fun19_sec07;

      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub06:
   *   - run event actions
   *   o fun19 sec06 sub06 cat01:
   *     - check if everything was input
   *   o fun19 sec06 sub06 cat02:
   *     - read in the config file
   *   o fun19 sec06 sub06 cat03:
   *     - build prefix & make output directory
   *   o fun19 sec06 sub06 cat04:
   *     - build the log file
   *   o fun19 sec06 sub06 cat05:
   *     - check if can run minimap2
   *   o fun19 sec06 sub06 cat06:
   *     - find length of minimap2 command and get memory
   *   o fun19 sec06 sub06 cat07:
   *     - build minimap2 command
   *   o fun19 sec06 sub06 cat08:
   *     - add the -sam <file>.sam entry to ftb
   *   o fun19 sec06 sub06 cat09:
   *     - run minimap2
   *   o fun19 sec06 sub06 cat10:
   *     - if cannot, copy fastq files to ftb command
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec06 Sub06 Cat01:
   +   - check if everything was input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   runFtb_fun19_sec06_sub06:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      if(guiSTPtr->fqStrSTPtr->lenSL <= 0)
      { /*If: no fastq files input*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(
            guiSTPtr->mesgStr,
            (signed char *) "no fastq files input"
         );

         goto done_fun19_sec07;
      } /*If: no fastq files input*/

      else if(guiSTPtr->prefixLenSI <= 0)
      { /*Else If: no prefix input*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(
            guiSTPtr->mesgStr,
            (signed char *) "no prefix input"
         );

         guiSTPtr->prefixLenSI =
            cpStr_ulCp(
               guiSTPtr->inPrefixStr,
               (signed char *) "FTB_OUT"
            );
         goto done_fun19_sec07;
      } /*Else If: no prefix input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat02:
      +   - read in the config file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      argLenSI = 1; /*0 is always reserved*/

      /*get database paths and check for minimap2*/
      if(
         getDatabases_ftbRayST(
            (signed char *) GetApplicationDirectory(),
            refStr,
            coordsStr,
            minimap2Str,
            argAryStr,
            &argLenSI
         )
      ) goto err_fun19_sec07;

      if(guiSTPtr->configFileStr[0])
      { /*If: user provided a configuration file*/
         inFILE =
            fopen((char *) guiSTPtr->configFileStr, "r");

         while(
            getLine_fileFun(
               inFILE,
               lineStr,
               def_lineLen_fun19,
               &discardSL
            )
         ){ /*Loop: read in configuration file*/
            tmpHeapStr = lineStr;

            while(*tmpHeapStr && *tmpHeapStr < 33)
               ++tmpHeapStr;
            if(! *tmpHeapStr)
               continue;
            tmpSI = endWhite_ulCp(tmpHeapStr);

            argAryStr[argLenSI] =
               malloc((tmpSI + 8) * sizeof(signed char));
            if(! argAryStr[argLenSI])
               goto err_fun19_sec07;

            tmpHeapStr +=
               cpWhite_ulCp(
                 argAryStr[argLenSI],
                 tmpHeapStr
               );
            ++argLenSI;

            /*copy the second entry (if is one)*/
            while(*tmpHeapStr && *tmpHeapStr < 33)
               ++tmpHeapStr;
            if(! *tmpHeapStr)
            { /*If: no more arguments on this line*/
               tmpHeapStr = 0;
               continue;
            } /*If: no more arguments on this line*/

            tmpSI = endStr_ulCp(tmpHeapStr);

            argAryStr[argLenSI] =
               malloc((tmpSI + 8) * sizeof(signed char));
            if(! argAryStr[argLenSI])
            { /*If: memory error*/
               tmpHeapStr = 0;
               goto err_fun19_sec07;
            } /*If: memory error*/

            cpWhite_ulCp(argAryStr[argLenSI], tmpHeapStr);

            /*make sure user is not changing reference*/
            if(argAryStr[argLenSI - 1][0] != '-') ;
            else if(argAryStr[argLenSI - 1][1] != 'r') ;
            else if(argAryStr[argLenSI - 1][2] != 'e') ;
            else if(argAryStr[argLenSI - 1][3] != 'f') ;
            else
               cpStr_ulCp(refStr, argAryStr[argLenSI]);
               /*user input reference sequence*/

            /*make sure user is not changing reference*/
            if(argAryStr[argLenSI - 1][0] != '-') ;
            else if(argAryStr[argLenSI - 1][1] != 'r') ;
            else if(argAryStr[argLenSI - 1][2] != 'e') ;
            else if(argAryStr[argLenSI - 1][3] != 'f') ;
            else
               cpStr_ulCp(refStr, argAryStr[argLenSI]);
               /*user input reference sequence*/

            if(
               ! eqlWhite_ulCp(
                  (signed char *) "-gene-coords",
                  argAryStr[argLenSI - 1]
               )
            ) cpStr_ulCp(coordsStr, argAryStr[argLenSI]);
               /*user input a coordinates database; these
               `  likely means a change in drug targets
               */

            ++argLenSI;
            tmpHeapStr = 0;
         } /*Loop: read in configuration file*/

         tmpHeapStr = 0;

         fclose(inFILE);
         inFILE = 0;
      } /*If: user provided a configuration file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat03:
      +   - build prefix and make output directory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*prefix tag*/
      argAryStr[argLenSI] =
         malloc((7 + 8) * sizeof(signed char));

      if(! argAryStr[argLenSI])
         goto err_fun19_sec07;
      cpStr_ulCp(
         argAryStr[argLenSI],
         (signed char *) "-prefix"
      );
      ++argLenSI;

      /*prefix path*/
      tmpSI = endStr_ulCp(guiSTPtr->inPrefixStr) * 2;
      tmpSI += endStr_ulCp(guiSTPtr->outDirStr) + 1;
      tmpSI += 2;
      argAryStr[argLenSI] =
         malloc((tmpSI + 8) * sizeof(signed char));
      if(! argAryStr[argLenSI])
         goto err_fun19_sec07;
      tmpSI =
         cpStr_ulCp(
            argAryStr[argLenSI],
            guiSTPtr->outDirStr
         );
      argAryStr[argLenSI][tmpSI++] = def_pathSep_rayWidg;
      tmpSI +=
         cpStr_ulCp(
            &argAryStr[argLenSI][tmpSI],
            guiSTPtr->inPrefixStr
         );
      
      if( MakeDirectory((char *) argAryStr[argLenSI]) )
      { /*If: could not make the output directory*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         tmpSI =
            cpStr_ulCp(
               guiSTPtr->mesgStr,
               (signed char *) "could not make folder: "
            );
         cpStr_ulCp(
            &guiSTPtr->mesgStr[tmpSI],
            argAryStr[argLenSI]
         );
         goto done_fun19_sec07;
      } /*If: could not make the output directory*/

      argAryStr[argLenSI][tmpSI++] = def_pathSep_rayWidg;
      tmpSI +=
         cpStr_ulCp(
            &argAryStr[argLenSI][tmpSI],
            guiSTPtr->inPrefixStr
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat04:
      +   - build the log file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSI = cpStr_ulCp(logFileStr, argAryStr[argLenSI]);
      logFileStr[tmpSI++] = '-';
      logFileStr[tmpSI++] = 'l';
      logFileStr[tmpSI++] = 'o';
      logFileStr[tmpSI++] = 'g';
      logFileStr[tmpSI++] = '.';
      logFileStr[tmpSI++] = 't';
      logFileStr[tmpSI++] = 'x';
      logFileStr[tmpSI++] = 't';
      logFileStr[tmpSI] = 0;

      tmpFILE = fopen((char *) logFileStr, "w");
      if(! tmpFILE)
         goto err_fun19_sec07;
      pcitation_freezeTB(tmpFILE);
      fclose(tmpFILE);
      tmpFILE = 0;

      ++argLenSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat05:
      +   - check if can run minimap2
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(minimap2Str[0])
      { /*If: have minimap2*/
         tmpFILE = fopen((char *) logFileStr, "a");
         pMinimap2Citation_freezeTB(tmpFILE);
         fclose(tmpFILE);
         tmpFILE = 0;

         tmpSI = cpStr_ulCp(lineStr, minimap2Str);
         tmpSI +=
            cpStr_ulCp(
               &lineStr[tmpSI],
               (signed char *) " --version >> "
            );
         tmpSI += cpStr_ulCp(&lineStr[tmpSI], logFileStr);
         system((char *) lineStr);

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub06 Cat06:
         +   - find length of minimap2 command
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpSI = endStr_ulCp(minimap2Str);
         tmpSI += 16; /*length of " -a -x map-ont "*/
         tmpSI += endStr_ulCp(refStr) + 2; /*+2 for "'s*/
         tmpSI += endStr_ulCp(argAryStr[argLenSI - 1]);
         tmpSI += 5; /*" > \"<out_file>.sam\""*/

         for(
            siCnt = 0;
            siCnt < guiSTPtr->fqStrSTPtr->lenSL;
            ++siCnt
         ) tmpSI += 
             guiSTPtr->fqStrSTPtr->lenAryUI[siCnt] + 3;
             /*length for adding fastq files; + 1 for
             `  space between files; + 2 for "'s
             */
         tmpHeapStr =
            malloc((tmpSI + 8) * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun19_sec07;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub06 Cat07:
         +   - build minimap2 command
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpSI = cpStr_ulCp(tmpHeapStr, minimap2Str);
         tmpSI +=
            cpStr_ulCp(
               &tmpHeapStr[tmpSI],
               (signed char *) " -a -x map-ont \""
            );
         tmpSI += cpStr_ulCp(&tmpHeapStr[tmpSI], refStr);
         tmpHeapStr[tmpSI++] = '"';
         tmpHeapStr[tmpSI++] = ' ';
         tmpHeapStr[tmpSI] = 0;

         for(
            siCnt = 0;
            siCnt < guiSTPtr->fqStrSTPtr->lenSL;
            ++siCnt
         ){ /*Loop: copy fastq sequences*/
            tmpHeapStr[tmpSI++] = '"';

            cpLen_ulCp(
               &tmpHeapStr[tmpSI],
               guiSTPtr->fqStrSTPtr->strAry[siCnt],
               guiSTPtr->fqStrSTPtr->lenAryUI[siCnt]
            ); /*copy fastq file name*/

            tmpSI +=
               guiSTPtr->fqStrSTPtr->lenAryUI[siCnt];
            tmpHeapStr[tmpSI++] = '"';
            tmpHeapStr[tmpSI++] = ' ';
         }  /*Loop: copy fastq sequences*/

         tmpHeapStr[tmpSI++] = '>';
         tmpHeapStr[tmpSI++] = ' ';
         tmpHeapStr[tmpSI++] = '"';
         tmpSI +=
            cpStr_ulCp(
               &tmpHeapStr[tmpSI],
               argAryStr[argLenSI - 1]
            ); /*copy output name*/

         tmpHeapStr[tmpSI++] = '.';
         tmpHeapStr[tmpSI++] = 's';
         tmpHeapStr[tmpSI++] = 'a';
         tmpHeapStr[tmpSI++] = 'm';
         tmpHeapStr[tmpSI++] = '"';
         tmpHeapStr[tmpSI] = 0;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub06 Cat08:
         +   - add the -sam <file>.sam entry to ftb
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         argAryStr[argLenSI] =
            malloc(11 * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun19_sec07;
         argAryStr[argLenSI][0] = '-';
         argAryStr[argLenSI][1] = 's';
         argAryStr[argLenSI][2] = 'a';
         argAryStr[argLenSI][3] = 'm';
         argAryStr[argLenSI][4] = 0;
         ++argLenSI;

         tmpSI = endStr_ulCp(argAryStr[argLenSI - 2]);
         argAryStr[argLenSI] =
            malloc((tmpSI + 13) * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun19_sec07;
         cpLen_ulCp(
            argAryStr[argLenSI],
            argAryStr[argLenSI - 2],
            tmpSI
         );
         argAryStr[argLenSI][tmpSI++] = '.';
         argAryStr[argLenSI][tmpSI++] = 's';
         argAryStr[argLenSI][tmpSI++] = 'a';
         argAryStr[argLenSI][tmpSI++] = 'm';
         argAryStr[argLenSI][tmpSI] = 0;
         ++argLenSI;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec06 Sub06 Cat09:
         +   - run minimap2
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         inFILE = fopen((char *) logFileStr, "a");
         fprintf(
            inFILE,
            "^^^^minimap2 version^^^^%s",
           str_endLine
         );
         fprintf(
            inFILE,
            "minimap2 cmd: %s%s",
            tmpHeapStr,
            str_endLine
         );
         fclose(inFILE);
         inFILE = 0;

         if(system((char *) tmpHeapStr))
         { /*If: minimap2 errored out*/
            hidenClear_widg_rayWidg(
               guiSTPtr->mesgBoxIdSI,
               guiSTPtr->widgSTPtr
            );
            tmpSI =
               cpStr_ulCp(
                  guiSTPtr->mesgStr,
                  (signed char *) "minimap2 errored out"
               );
            cpStr_ulCp(
               &guiSTPtr->mesgStr[tmpSI],
               argAryStr[argLenSI]
            );
            goto done_fun19_sec07;
         } /*If: minimap2 errored out*/

         free(tmpHeapStr);
         tmpHeapStr = 0;
      } /*If: have minimap2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat10:
      +   - if cannot, copy fastq files to ftb command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: no minimap2*/
         for(
            siCnt = 0;
            siCnt < guiSTPtr->fqStrSTPtr->lenSL;
            ++siCnt
         ){ /*Loop: add fastq files to ftb command*/
            argAryStr[argLenSI] =
               malloc(
                 ( guiSTPtr->fqStrSTPtr->lenAryUI[siCnt]
                  + 8 
                 ) * sizeof(signed char)
               );

            if(! argAryStr[argLenSI])
               goto err_fun19_sec07;

            cpLen_ulCp(
               argAryStr[argLenSI],
               guiSTPtr->fqStrSTPtr->strAry[siCnt],
               guiSTPtr->fqStrSTPtr->lenAryUI[siCnt]
            ); /*copy fastq file name*/

            ++argLenSI;
         }  /*Loop: add fastq files to ftb command*/
      } /*Else: no minimap2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec06 Sub06 Cat11:
      +   - run freezeTB
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpHeapStr)
         free(tmpHeapStr);
      tmpHeapStr = 0;

      if(minimap2Str[0])
         inFILE = fopen((char *) logFileStr, "a");
      else
         inFILE = fopen((char *) logFileStr, "w");

      pversion_freezeTB(inFILE);
      fprintf(inFILE, "FreezeTB cmd:%s", str_endLine);
      fprintf(inFILE, "  freezeTB \\%s", str_endLine);

      for(siCnt = 1; siCnt < argLenSI; siCnt += 2)
      { /*Loop: print freezeTB command*/
         if(siCnt + 2 < argLenSI)
            fprintf(inFILE,
               "    %s %s \\%s",
               argAryStr[siCnt],
               argAryStr[siCnt + 1],
               str_endLine
            );
         else if(siCnt + 1 < argLenSI)
            fprintf(inFILE,
               "    %s %s%s",
               argAryStr[siCnt],
               argAryStr[siCnt + 1],
               str_endLine
            );
         else
            fprintf(inFILE,
               "    %s%s",
               argAryStr[siCnt],
               str_endLine
            );
      } /*Loop: print freezeTB command*/

      fclose(inFILE);
      inFILE = 0;

      tmpHeapStr =
         run_freezeTB(argLenSI, (char **) argAryStr);

      if(tmpHeapStr)
      { /*If: had an error*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(guiSTPtr->mesgStr, tmpHeapStr);

         inFILE = fopen((char *) logFileStr, "a");
         fprintf(
            inFILE,
            "%s%s",
            guiSTPtr->mesgStr,
            str_endLine
         );
         fclose(inFILE);
         inFILE = 0;
         goto done_fun19_sec07;
      } /*If: had an error*/

      /*remove run fastq files*/
      blank_str_ptrAry(guiSTPtr->fqStrSTPtr);
      siCnt =
         cpStr_ulCp(
            guiSTPtr->filePrefixStr,
            guiSTPtr->outDirStr
         );
      guiSTPtr->filePrefixStr[siCnt++] =
         def_pathSep_rayWidg;
      siCnt +=
         cpStr_ulCp(
            &guiSTPtr->filePrefixStr[siCnt],
            guiSTPtr->inPrefixStr
         );
      guiSTPtr->filePrefixStr[siCnt++] =
         def_pathSep_rayWidg;
      cpStr_ulCp(
         &guiSTPtr->filePrefixStr[siCnt],
         guiSTPtr->inPrefixStr
      );
      guiSTPtr->inPrefixStr[0] = 0;
      goto buildOutReport_fun19_sec06_sub0x;

   /*****************************************************\
   * Fun19 Sec06 Sub07:
   *   - got to input menu
   \*****************************************************/

   inputMenu_fun19_sec06_sub07:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
      hideHsp65_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->inputGuiIdSI,
         guiSTPtr->widgSTPtr
      );
      
      hidenClear_widg_rayWidg(
         guiSTPtr->fqButIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->fqLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->prefixEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->prefixLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->outDirIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->outDirLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->configIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->configLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->runIdSI,
         guiSTPtr->widgSTPtr
      );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub08:
   *   - goto output menu
   \*****************************************************/

   outputMenu_fun19_sec06_sub08:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;
      hideInput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
      hideHsp65_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->outGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      if(buildReportBl)
         goto buildOutReport_fun19_sec06_sub0x;

      hidenClear_widg_rayWidg(
         guiSTPtr->getPrefixButIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->getPrefixLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->minAmrPercEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->minAmrPercLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->minAmrIndelPercEntryIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->minAmrIndelPercLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->getOutButSI,
         guiSTPtr->widgSTPtr
      );

      goto done_fun19_sec07;

   buildOutReport_fun19_sec06_sub0x:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;
      if(! guiSTPtr->filePrefixStr[0])
         goto done_fun19_sec07;

      /*build drug resistance report*/
      checkDrugs_ftbRayST(guiSTPtr);
      spoligoLinGet_ftbRayST(guiSTPtr);
      miruLinGet_ftbRayST(guiSTPtr);
      getHsp65Lin_ftbRayST(guiSTPtr);
      mkCoverageTbl_ftbRayST(guiSTPtr);

      goto reportMenu_fun19_sec06_sub10;

   /*****************************************************\
   * Fun19 Sec06 Sub10:
   *   - got to report
   \*****************************************************/

   reportMenu_fun19_sec06_sub10:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
      hideHsp65_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->reportGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenClear_widg_rayWidg(
         guiSTPtr->reportPrefixLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->miruLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->spoligoLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->spoligoLineageLabIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->spoligoStrainLabIdSI,
         guiSTPtr->widgSTPtr
      );

      for(tmpSI=0; tmpSI < def_numDrugs_ftbRayST; ++tmpSI)
         hidenClear_widg_rayWidg(
            guiSTPtr->drugResRectIdSI + tmpSI,
            guiSTPtr->widgSTPtr
         );
      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub11:
   *   - get ftb prefix
   \*****************************************************/

   getFtbPrefix_fun19_sec06_sub11:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select a FTB output file"
      );
      guiSTPtr->browserSC = 3;
      goto fileBrowser_fun19_sec06_sub05;

   /*****************************************************\
   * Fun19 Sec06 Sub12:
   *   - goto to the amr table
   \*****************************************************/

   amrTblMenu_fun19_sec06_sub12:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideHsp65_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->amrsGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenClear_widg_rayWidg(
         guiSTPtr->amrTblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->amrLabIdSI,
         guiSTPtr->widgSTPtr
      );

      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub13:
   *   - goto to the hsp65 species/lineage table
   \*****************************************************/

   hsp65TblMenu_fun19_sec06_sub12:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);


      inactiveAdd_widg_rayWidg(
         guiSTPtr->hsp65GuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenClear_widg_rayWidg(
         guiSTPtr->hsp65TblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->hsp65LabIdSI,
         guiSTPtr->widgSTPtr
      );

      goto done_fun19_sec07;

   /*****************************************************\
   * Fun19 Sec06 Sub14:
   *   - goto to gene coverage table
   \*****************************************************/

   geneCoverMenu_fun19_sec06_sub13:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun19_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideHsp65_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->coverGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      hidenClear_widg_rayWidg(
         guiSTPtr->geneCoverTblIdSI,
         guiSTPtr->widgSTPtr
      );
      hidenClear_widg_rayWidg(
         guiSTPtr->geneCoverLabIdSI,
         guiSTPtr->widgSTPtr
      );

      goto done_fun19_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec07:
   ^   - return results and redraw gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto done_fun19_sec07;

   err_fun19_sec07:;
      tmpSI = 1;
      goto ret_fun19_sec07;

   done_fun19_sec07:;
      draw_gui_ftbRayST(guiSTPtr);
      tmpSI = 0;
      goto ret_fun19_sec07;

   ret_fun19_sec07:;
      if(inFILE)
         fclose(inFILE); /*never will be stdout/in/err*/
      inFILE = 0;

      if(tmpFILE)
         fclose(tmpFILE); /*never will be stdout/in/err*/
      tmpFILE = 0;

      freeStack_event_rayWidg(&eventStackST);

      if(tmpHeapStr)
         free(tmpHeapStr);
      tmpHeapStr = 0;

      for(indexSI = 0; indexSI < argLenSI; ++indexSI)
      { /*Loop: free c-string array*/
         if(argAryStr[indexSI])
            free(argAryStr[indexSI]);
         argAryStr[indexSI] = 0;
      } /*Loop: free c-string array*/

      argLenSI = 0;

      return (signed char) tmpSI;
} /*checkRunEvent_ftbRayST*/
