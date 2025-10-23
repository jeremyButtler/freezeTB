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
'   o .c fun11: hideGeneCover_ftbRayST
'     - hides the gene coverage table
'   o .c fun12: spoligoLinGet_ftbRayST
'     - gets the spoligotype lineage and sets the spoligo
'   o .c fun13: miruLinGet_ftbRayST
'     - gets the MIRU-VNTR lineage & sets miru text output
'   o .c fun14: checkDrugs_ftbRayST
'     - builds the drug resistance part of the ftb report
'   o .c fun15: getDatabases_ftbRayST
'     - get database files for freezeTB (currently Mac)
'   o .c fun16: mkCoverageTbl_ftbRayST
'     - makes the gene percent coverage table
'   o fun17: checkRunEvent_ftbRayST
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
`  the gene coverage step in checkDrugs_ftbRayST (fun14)
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
   guiSTPtr->spoligoLabIdSI = 0;
   guiSTPtr->drugResRectIdSI = 0;

   guiSTPtr->amrTblIdSI = 0;
   guiSTPtr->amrLabIdSI = 0;
   guiSTPtr->amrListSTPtr = 0;

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
            (signed char *) "input",
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
            (signed char *) "output",
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
            (signed char *) "AMRs",
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
            (signed char *) "% gene",
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

         heightSI =
              padSI
            + def_getTotalPad_rayWidg(
                 guiSTPtr->widgSTPtr->fontHeightF
              );

         for(
            lenSI = 0;
            lenSI < def_numDrugs_ftbRayST;
            ++lenSI
         ){ /*Loop: draw the amr report*/
            if(lenSI && ! (lenSI % 7) )
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
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->fqLabIdSI,
            fqFileStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
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
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->outDirLabIdSI,
            guiSTPtr->outDirStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
            tileBl,
            guiSTPtr->widgSTPtr
         ); /*label for entry box*/

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
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->configLabIdSI,
            guiSTPtr->configFileStr,
            ' ',                  /*padding with spaces*/
            2,                    /*right pad if needed*/
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
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->getPrefixLabIdSI,
            guiSTPtr->filePrefixStr,
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


         /*labDraw_rayWidg(
            guiSTPtr->widgSTPtr->winWidthSI,
            0,
            guiSTPtr->miruLabIdSI,
            guiSTPtr->miruStr,
            ' ',
            2,
            tileBl,
            guiSTPtr->widgSTPtr
         );*/ /*label for entry box*/

         /*_____________table_menu______________________*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->amrLabIdSI,
            guiSTPtr->filePrefixStr,
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

         /*___________gene_coverage_menu________________*/
         labDraw_rayWidg(
            glob_maxWidgWidthSI,
            0,                    /*no min width*/
            guiSTPtr->geneCoverLabIdSI,
            guiSTPtr->filePrefixStr,
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
   ^   o fun06 sec02 sub07:
   ^     - add report screen widgets (last for rectangles)
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
         (signed char *) "depths.tsv",
         0, /*do not clear old extensions*/
         1, /*select this extension*/
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
| Fun11: hideGeneCover_ftbRayST
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
| Fun12: spoligoLinGet_ftbRayST
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
| Fun13: miruLinGet_ftbRayST
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
| Fun14: checkDrugs_ftbRayST
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
   ' Fun14 TOC:
   '   - builds the drug resistance part of the ftb report
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - initialize
   '   o fun14 sec03:
   '     - get coverage of reads
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char fileStr[256];
   signed char lineStr[1024];
   signed int lenSI = 0;
   signed char *tmpStr = 0;

   signed char geneStr[128];
   signed int startSI = 0;
   signed int endSI = 0;

   signed int refStartSI = 0;
   signed int refEndSI = 0;

   unsigned int backColUI = 0;
   unsigned int textColUI = 0;

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
   signed char drugAryStr[def_numDrugs_ftbRayST * 4];
   signed int drugLenSI = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - initialize
   ^   o fun14 sec02 sub01:
   ^     - get default filters and clear AMRs
   ^   o fun14 sec02 sub02:
   ^     - add the header to the amr table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec02 Sub01:
   *   - get default filters and clear AMRs
   \*****************************************************/
   
   if(! guiSTPtr->filePrefixStr[0])
      return;

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
   * Fun14 Sec02 Sub02:
   *   - add the header to the amr table
   \*****************************************************/

   /*build header for list box*/
   lenSI = 0;
   lenSI +=
      cpStr_ulCp(&lineStr[lenSI], (signed char *)" drug");
        /*space is frist alphabetcially, so I can use it
        `  to keep the header at the top during the sort
        */
   lineStr[lenSI++] = ' '; /*set lenSI to 4*/

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
         (signed char *) "read_depth"
      );
   while(lenSI < 47)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 48*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "%_support"
      );
   while(lenSI < 56)
      lineStr[lenSI++] = ' ';
   lineStr[lenSI++] = ' '; /*make 57*/

   lenSI +=
      cpStr_ulCp(
         &lineStr[lenSI],
         (signed char *) "needs"
      );

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
   ^ Fun14 Sec03:
   ^   - get coverage of reads
   ^   o fun14 sec03 sub01:
   ^     - open depths file
   ^   o fun14 sec03 sub02:
   ^     - get the coverage, gene name, & expected length
   ^   o fun14 sec03 sub03:
   ^     - check if AMR gene & flag if low coverage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec03 Sub01:
   *   - open depths file
   \*****************************************************/

   lenSI = cpStr_ulCp(fileStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &fileStr[lenSI],
      (signed char *) "-depths.tsv"
   );

   
   inFILE = fopen((char *) fileStr, "r");
   if(! inFILE)
     return;

   /*****************************************************\
   * Fun14 Sec03 Sub02:
   *   - get the coverage, gene name, and expected length
   \*****************************************************/

   if(fgets((char *) lineStr, 1000, inFILE))
   { /*If: have header*/
      while( fgets((char *) lineStr, 1000, inFILE) )
      { /*Loop: get depths for each gene*/
         tmpStr = lineStr;
         lenSI = 0;

         while(*tmpStr && lenSI < 4)
         { /*Loop: get to amplicon start*/
            if(*tmpStr == '\t')
               ++lenSI;
            ++tmpStr;
         } /*Loop: get to amplicon start*/

         /*get start and end of amplicon*/
         tmpStr += strToSI_base10str(tmpStr, &startSI);
         if(! *tmpStr) break;
         ++tmpStr;

         tmpStr += strToSI_base10str(tmpStr, &endSI);
         if(! *tmpStr) break;
         ++tmpStr;

         lenSI = 0;
         while(*tmpStr && lenSI < 3)
         { /*Loop: get to gene name*/
            if(*tmpStr == '\t')
               ++lenSI;
            ++tmpStr;
         } /*Loop: get to amplicon start*/

         tmpStr += cpWhite_ulCp(geneStr, tmpStr) ;
         if(! *tmpStr) break;
         ++tmpStr;

       
         /*get start and end of amplicon*/
         tmpStr += strToSI_base10str(tmpStr, &refStartSI);
         if(! *tmpStr) break;
         ++tmpStr;

         tmpStr += strToSI_base10str(tmpStr, &refEndSI);
         if(! *tmpStr) break;
         ++tmpStr;

         if(startSI > refStartSI || endSI < refEndSI)
         { /*If: incomplete gene coverage*/
            backColUI = def_lowDepthBackCol_ftbRayST;
            textColUI = def_lowDepthTextCol_ftbRayST;
         } /*If: incomplete gene coverage*/

         else
         { /*If: incomplete gene coverage*/
            backColUI = def_noAmrBackCol_ftbRayST;
            textColUI = def_noAmrTextCol_ftbRayST;
         } /*If: incomplete gene coverage*/

         /***********************************************\
         * Fun14 Sec03 Sub03:
         *   - check if gene is an AMR gene and flag if
         *     low coverage
         \***********************************************/

         if(!eqlNull_ulCp((signed char *) "atpE",geneStr))
         { /*If: is atpE gene (Bdq)*/
            if(startSI > 1461125 || endSI < 1461242)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[1] = backColUI;
            guiSTPtr->drugTextColUI[1] = textColUI;
         } /*If: is atpE gene (Bdq)*/

         else if(
            ! eqlNull_ulCp((signed char *) "ddn", geneStr)
         ){ /*Else If: is ddn gene (Dlm and Pmd)*/
            if(startSI > 3986737 || endSI < 3987298)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[4] = backColUI;
            guiSTPtr->drugTextColUI[4] = textColUI;
            guiSTPtr->drugBackColUI[13] = backColUI;
            guiSTPtr->drugTextColUI[13] = textColUI;
         } /*Else If: is ddn gene (Dlm and Pmd)*/

         else if(
            ! eqlNull_ulCp((signed char *) "eis", geneStr)
         ){ /*Else If: is eis gene (Amk and Kan)*/
            if(startSI > 2715339 || endSI < 2715369)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[0] = backColUI;
            guiSTPtr->drugTextColUI[0] = textColUI;
            guiSTPtr->drugBackColUI[9] = backColUI;
            guiSTPtr->drugTextColUI[9] = textColUI;
         } /*Else If: is eis gene (Amk and Kan)*/

         else if(
            ! eqlNull_ulCp((signed char *) "embA",geneStr)
         ) ; /*not in WHO 2023: embA eis gene (Emb)*/

         else if(
            ! eqlNull_ulCp((signed char *) "embB",geneStr)
         ){ /*Else If: embB eis gene (Emb)*/
            if(startSI > 4247429 || endSI < 4248003)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[5] = backColUI;
            guiSTPtr->drugTextColUI[5] = textColUI;
         } /*Else If: embB eis gene (Emb)*/

         else if(
            ! eqlNull_ulCp((signed char *) "ethA",geneStr)
         ){ /*Else If: ethA eis gene (Eto)*/
            if(startSI > 4325970 || endSI < 4327480)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[6] = backColUI;
            guiSTPtr->drugTextColUI[6] = textColUI;
         } /*Else If: ethA eis gene (Eto)*/

         else if(
           ! eqlNull_ulCp((signed char *) "fabG1",geneStr)
         ){ /*Else If: fabG1 eis gene (Eto Inh)*/
            if(startSI > 1673423 || endSI < 1674048)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[6] = backColUI;
            guiSTPtr->drugTextColUI[6] = textColUI;

            guiSTPtr->drugBackColUI[8] = backColUI;
            guiSTPtr->drugTextColUI[8] = textColUI;
         } /*Else If: fabG1 eis gene (Eto Inh)*/

         else if(
           ! eqlNull_ulCp((signed char *) "fbiA", geneStr)
         ){ /*Else If: fbiA eis gene (Cfz Dlm Pmd)*/
            if(startSI > 3640145 || endSI < 3641537)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[3] = backColUI;
            guiSTPtr->drugTextColUI[3] = textColUI;

            guiSTPtr->drugBackColUI[4] = backColUI;
            guiSTPtr->drugTextColUI[4] = textColUI;

            guiSTPtr->drugBackColUI[13] = backColUI;
            guiSTPtr->drugTextColUI[13] = textColUI;
         } /*Else If: fbiA eis gene (Cfz Dlm Pmd)*/

         else if(
           ! eqlNull_ulCp((signed char *) "fbiB", geneStr)
         ){ /*Else If: fbiB eis gene (Cfz Dlm Pmd)*/
            if(startSI > 3641538 || endSI < 3642880)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[3] = backColUI;
            guiSTPtr->drugTextColUI[3] = textColUI;

            guiSTPtr->drugBackColUI[4] = backColUI;
            guiSTPtr->drugTextColUI[4] = textColUI;

            guiSTPtr->drugBackColUI[13] = backColUI;
            guiSTPtr->drugTextColUI[13] = textColUI;
         } /*Else If: fbiB eis gene (Cfz Dlm Pmd)*/

         else if(
           ! eqlNull_ulCp((signed char *) "fbiC", geneStr)
         ){ /*Else If: fbiC eis gene (Cfz Dlm Pmd)*/
            if(startSI > 1302695 || endSI < 1305501)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[3] = backColUI;
            guiSTPtr->drugTextColUI[3] = textColUI;

            guiSTPtr->drugBackColUI[4] = backColUI;
            guiSTPtr->drugTextColUI[4] = textColUI;

            guiSTPtr->drugBackColUI[13] = backColUI;
            guiSTPtr->drugTextColUI[13] = textColUI;
         } /*Else If: fbiC eis gene (Cfz Dlm Pmd)*/

         else if(
           ! eqlNull_ulCp((signed char *) "fgd1", geneStr)
         ) ; /*not in WHO 2023 or at least not as  fgd1
             `  gene (Cfz Dlm Pmd)
             */

         else if(
           ! eqlNull_ulCp((signed char *) "gidB", geneStr)
         ) ; /*not in WHO 2023 gidB gene (Stm)*/

         else if(
           ! eqlNull_ulCp((signed char *) "gyrA", geneStr)
         ){ /*Else If: gyrA gene (Flq Lfx Mfx)*/
            if(startSI > 7563 || endSI < 7582)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[7] = backColUI;
            guiSTPtr->drugTextColUI[7] = textColUI;

            guiSTPtr->drugBackColUI[10] = backColUI;
            guiSTPtr->drugTextColUI[10] = textColUI;

            guiSTPtr->drugBackColUI[12] = backColUI;
            guiSTPtr->drugTextColUI[12] = textColUI;
         } /*Else If: gyrA gene (Flq Lfx Mfx)*/

         else if(
           ! eqlNull_ulCp((signed char *) "gyrB", geneStr)
         ){ /*Else If: gyrB gene (Flq Lfx Mfx)*/
            if(startSI > 6579 || endSI < 6750)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[7] = backColUI;
            guiSTPtr->drugTextColUI[7] = textColUI;

            guiSTPtr->drugBackColUI[10] = backColUI;
            guiSTPtr->drugTextColUI[10] = textColUI;

            guiSTPtr->drugBackColUI[12] = backColUI;
            guiSTPtr->drugTextColUI[12] = textColUI;
         } /*Else If: gyrB gene (Flq Lfx Mfx)*/

         else if(
           ! eqlNull_ulCp((signed char *) "inhA", geneStr)
         ){ /*Else If: inhA gene (Eto Inh)*/
            if(startSI > 1673423 || endSI < 1674481)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[6] = backColUI;
            guiSTPtr->drugTextColUI[6] = textColUI;

            guiSTPtr->drugBackColUI[8] = backColUI;
            guiSTPtr->drugTextColUI[8] = textColUI;
         } /*Else If: inhA gene (Eto Inh)*/

         else if(
           ! eqlNull_ulCp((signed char *) "katG", geneStr)
         ){ /*Else If: katG gene (Inh)*/
            if(startSI > 2153926 || endSI < 2156110)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[8] = backColUI;
            guiSTPtr->drugTextColUI[8] = textColUI;
         } /*Else If: katG gene (Inh)*/

         else if(
           ! eqlNull_ulCp((signed char *) "pncA", geneStr)
         ){ /*Else If: pncA gene (Pza)*/
            if(startSI > 2288680 || endSI < 228953)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[14] = backColUI;
            guiSTPtr->drugTextColUI[14] = textColUI;
         } /*Else If: pncA gene (Pza)*/

         else if(
           ! eqlNull_ulCp((signed char *) "pepQ", geneStr)
         ){ /*Else If: pepQ gene (Bdq Cfz)*/
            if(startSI > 2859292 || endSI < 2860417)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*If: incomplete gene coverage*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            guiSTPtr->drugBackColUI[1] = backColUI;
            guiSTPtr->drugTextColUI[1] = textColUI;

            guiSTPtr->drugBackColUI[3] = backColUI;
            guiSTPtr->drugTextColUI[3] = textColUI;
         } /*Else If: pepQ gene (Bdq Cfz)*/

         else if(
           ! eqlNull_ulCp((signed char *) "rpoB", geneStr)
         ){ /*Else If: rpoB gene (Rif)*/
            if(startSI > 760314 || endSI < 761277)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[15] = backColUI;
            guiSTPtr->drugTextColUI[15] = textColUI;
         } /*Else If: rpoB gene (Rif)*/

         else if(
           ! eqlNull_ulCp((signed char *) "rpsL", geneStr)
         ){ /*Else If: rpsL gene (Stm)*/
            if(startSI > 781682 || endSI < 781822)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[16] = backColUI;
            guiSTPtr->drugTextColUI[16] = textColUI;
         } /*Else If: rpsL gene (Stm)*/

         else if(
           ! eqlNull_ulCp((signed char *) "rrs", geneStr)
         ){ /*Else If: rrs gene (Amk Cap Kan Stm)*/
            if(startSI > 1472359 || endSI < 1473329)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[0] = backColUI;
            guiSTPtr->drugTextColUI[0] = textColUI;

            guiSTPtr->drugBackColUI[2] = backColUI;
            guiSTPtr->drugTextColUI[2] = textColUI;

            guiSTPtr->drugBackColUI[9] = backColUI;
            guiSTPtr->drugTextColUI[9] = textColUI;

            guiSTPtr->drugBackColUI[16] = backColUI;
            guiSTPtr->drugTextColUI[16] = textColUI;
         } /*Else If: rrs gene (Amk Cap Kan Stm)*/

         else if(
           ! eqlNull_ulCp((signed char *) "rrl", geneStr)
         ){ /*Else If: rrl gene (Cap Lzd)*/
            if(startSI > 1475926 || endSI < 1476471)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[2] = backColUI;
            guiSTPtr->drugTextColUI[2] = textColUI;

            guiSTPtr->drugBackColUI[11] = backColUI;
            guiSTPtr->drugTextColUI[11] = textColUI;
         } /*Else If: rrl gene (Cap Lzd)*/

         else if(
           ! eqlNull_ulCp((signed char *) "rplC", geneStr)
         ){ /*Else If: rplC gene (Lzd)*/
            if(startSI > 801266 || endSI < 801268)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[11] = backColUI;
            guiSTPtr->drugTextColUI[11] = textColUI;
         } /*Else If: rplC gene (Lzd)*/

         else if(
           !eqlNull_ulCp((signed char *) "rv0678",geneStr)
         ){ /*Else If: rv0678 gene (Bdq Cfz)*/
            /*likely different name in 2023 catalog*/
            /*guiSTPtr->drugBackColUI[1] = backColUI;
            guiSTPtr->drugTextColUI[1] = textColUI;

            guiSTPtr->drugBackColUI[3] = backColUI;
            guiSTPtr->drugTextColUI[3] = textColUI;*/
         } /*Else If: rv0678 gene (Bdq Cfz)*/

         else if(
           !eqlNull_ulCp((signed char *) "rv2983",geneStr)
         ){ /*Else If: rv2983 gene (Dlm Pmd)*/
            /*likely different name in 2023 catalog*/
            /*guiSTPtr->drugBackColUI[4] = backColUI;
            guiSTPtr->drugTextColUI[4] = textColUI;

            guiSTPtr->drugBackColUI[13] = backColUI;
            guiSTPtr->drugTextColUI[13] = textColUI;*/
         } /*Else If: rv2983 gene (Dlm Pmd)*/

         else if(
           ! eqlNull_ulCp((signed char *) "tlyA", geneStr)
         ){ /*Else If: tlyA gene (Cap)*/
            if(startSI > 1917933 || endSI < 1918741)
            { /*If: incomplete gene coverage*/
               backColUI = def_lowDepthBackCol_ftbRayST;
               textColUI = def_lowDepthTextCol_ftbRayST;
            } /*If: incomplete gene coverage*/

            else
            { /*Else: covered amr region*/
               backColUI = def_noAmrBackCol_ftbRayST;
               textColUI = def_noAmrTextCol_ftbRayST;
            } /*Else: covered amr region*/

            guiSTPtr->drugBackColUI[2] = backColUI;
            guiSTPtr->drugTextColUI[2] = textColUI;
         }  /*Else If: tlyA gene (Cap)*/
      } /*Loop: get depths for each gene*/
   } /*If: have header*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec04:
   ^   - find AMRs
   ^   o fun14 sec04 sub01:
   ^     - open amr file (reads)
   ^   o fun14 sec04 sub02:
   ^     - get gene name + start loop
   ^   o fun14 sec04 sub03:
   ^     - check for resistance
   ^   o fun14 sec04 sub04:
   ^     - check for cross resistance (loops to sub03)
   ^   o fun14 sec04 sub05:
   ^     - get variant id, grade, and support
   ^   o fun14 sec04 sub06:
   ^     - check if amr has enough support
   ^   o fun14 sec04 sub07:
   ^     - if enough support, set resistance colors
   ^   o fun14 sec04 sub08:
   ^     - get high/low res, additive res, and genes
   ^   o fun14 sec04 sub09:
   ^     - build amr table entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec04 Sub01:
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
   * Fun14 Sec04 Sub02:
   *   - get gene name + start loop
   \*****************************************************/

   if(fgets((char *) lineStr, 1000, inFILE))
   { /*If: have header*/
      while( fgets((char *) lineStr, 1000, inFILE) )
      { /*Loop: get drug resistance*/
         tmpStr = lineStr;

         tmpStr += cpWhite_ulCp(geneStr, tmpStr);
         if(! *tmpStr)
            continue;
         ++tmpStr;

         tmpStr += cpWhite_ulCp(colStr, tmpStr);
         ++tmpStr;
         drugLenSI = 0;

         /***********************************************\
         * Fun14 Sec04 Sub03:
         *   - check for resistance
         \***********************************************/

         if(
               colStr[0] == 'N'
            && colStr[1] == 'A'
            && ! colStr[2]
         ) tmpStr += 2; /*no drug to check*/

         else
         { /*Else: check what drug am resistant to*/
            checkDrug_fun14_sec04:;
               drugLenSI = 0;
               while(colStr[drugLenSI])
                  colStr[drugLenSI++] |= 32;/*lower case*/
               drugLenSI = 0;

               for(
                  lenSI = 0;
                  lenSI < def_numDrugs_ftbRayST;
                  ++lenSI
               ){ /*Loop: check all drugs*/
                  resAryBl[lenSI] = 0;
                  drugAryStr[lenSI] = 0;

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

                     drugAryStr[drugLenSI * 4] =
                        glob_drugStrAry[lenSI][0];
                     drugAryStr[drugLenSI * 4 + 1] =
                        glob_drugStrAry[lenSI][1];
                     drugAryStr[drugLenSI * 4 + 2] =
                        glob_drugStrAry[lenSI][2];
                     drugAryStr[drugLenSI * 4 + 3] = '_';
                     ++drugLenSI;
                  }  /*If: found resistance*/
               }  /*Loop: check all drugs*/
         } /*Else: check what drug am resistant to*/

         /***********************************************\
         * Fun14 Sec04 Sub04:
         *   - check for cross resistance (loops to sub03)
         \***********************************************/

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
               colStr[lenSI++] = *tmpStr++;
            goto checkDrug_fun14_sec04;
         } /*Else If: have cross resistance*/

         if(! *tmpStr)
            continue;
         ++tmpStr;

         if(drugLenSI > 0)
         { /*If: resistance, break into two c-strings*/
            if(drugLenSI <= 3)
            { /*If: not cross resistance*/
               drugAryStr[3] = 0;
               drugAryStr[4] = 'N';
               drugAryStr[5] = 'A';
               drugAryStr[6] = 0;
            } /*If: not cross resistance*/

            else
            { /*Else: had cross resistance*/
               drugAryStr[(drugLenSI - 1) * 4 + 3] = 0;
               drugAryStr[3] = 0; /*store two c-strings*/
            } /*Else: had cross resistance*/
         } /*If: resistance, break into two c-strings*/

         /***********************************************\
         * Fun14 Sec04 Sub05:
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
         * Fun14 Sec04 Sub06:
         *   - check if amr has enough support
         \***********************************************/

         /*check if keeping AMR*/
         if(
               (typeStr[0] & ~32) == 's'
            && (typeStr[1] & ~32) == 'n'
            && (typeStr[2] & ~32) == 'p'
            && typeStr[3] < 33
         ){ /*If: snp variant*/
            if(percSupReadsF < minPerSupF)
               continue;
         }  /*If: snp variant*/

         else
         { /*Else: frameshift or indel*/
            if(percSupReadsF < minIndelPerSupF)
               continue;
         } /*Else: frameshift or indel*/

         /***********************************************\
         * Fun14 Sec04 Sub07:
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
         * Fun14 Sec04 Sub08:
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
         * Fun14 Sec04 Sub09:
         *   - build amr table entry
         \***********************************************/

         lenSI = 0;

         /*copy the drug name*/
         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], &drugAryStr[0]);
         lineStr[lenSI++] = ' '; /*set lenSI to 4*/
         lineStr[lenSI++] = ' '; /*set lenSI to 5*/

         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], varIdStr);

         while(lenSI < 25)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*make 26*/

         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], &drugAryStr[4]);
         while(lenSI < 37)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 38*/

         lenSI += numToStr(&lineStr[lenSI],numSupReadsSI);
         while(lenSI < 48)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 49*/

         lenSI +=
            double_numToStr(
               &lineStr[lenSI],
               percSupReadsF,
               2
            );
         while(lenSI < 57)
            lineStr[lenSI++] = ' ';
         lineStr[lenSI++] = ' '; /*sets to 58*/

         lenSI +=
            cpStr_ulCp(&lineStr[lenSI], needsGeneStr);

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
| Fun15: getDatabases_ftbRayST
|   - get database files for freezeTB (currently Mac)
| Input:
|   - appPathStr:
|     o c-string with path to ftbRay binarary
|   - refStr:
|     o c-string to get path to the reference sequence
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
   signed char *minimap2Str,/*gets minimap2 location*/
   signed char **argAryStr, /*gets database paths*/
   signed int *argLenSIPtr  /*number of arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC: getDatabases_ftbRayST
   '   - get database files for freezeTB (currently Mac)
   '   o fun15 sec01:
   '     - variable declarations
   '   o fun15 sec02:
   '     - get path to databases
   '   o fun15 sec03:
   '     - get reference file
   '   o fun15 sec04:
   '     - get gene coordinates file
   '   o fun15 sec05:
   '     - get MIRU-VNTR lineage table
   '   o fun15 sec06:
   '     - get spoligotype spacer sequences
   '   o fun15 sec07:
   '     - get spoligotype lineage database
   '   o fun15 sec08:
   '     - find path to minimap2
   '   o fun15 sec09:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
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
   ^ Fun15 Sec02:
   ^   - get path to databases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*TODO this needs to be defined for every OS*/
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
   ^ Fun15 Sec03:
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
      goto err_fun15_sec09;
   argAryStr[*argLenSIPtr][0] = '-';
   argAryStr[*argLenSIPtr][1] = 'r';
   argAryStr[*argLenSIPtr][2] = 'e';
   argAryStr[*argLenSIPtr][3] = 'f';
   argAryStr[*argLenSIPtr][4] = 0;
   ++(*argLenSIPtr);

   argAryStr[*argLenSIPtr] =
      malloc(256 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
   refPath_freezeTBPaths(argAryStr[*argLenSIPtr]);

   if(! argAryStr[*argLenSIPtr][0])
      cpStr_ulCp(argAryStr[*argLenSIPtr], refStr);
   else
      cpStr_ulCp(refStr, argAryStr[*argLenSIPtr]);

   ++*argLenSIPtr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - get reference file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
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
      goto err_fun15_sec09;

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
   ^ Fun15 Sec04:
   ^   - get gene coordinates file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
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
      goto err_fun15_sec09;

   coordPath_freezeTBPaths(argAryStr[*argLenSIPtr]);
   if(! argAryStr[*argLenSIPtr][0])
   { /*If: need to find coordinates file path*/
      lenSI = cpStr_ulCp(argAryStr[*argLenSIPtr],pathStr);
      cpStr_ulCp(
         &argAryStr[*argLenSIPtr][lenSI],
         (signed char *) "coords.tsv"
      );
   } /*If: need to find coordinates file path*/

   ++(*argLenSIPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec05:
   ^   - get MIRU-VNTR lineage table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
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
     goto err_fun15_sec09;

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
   ^ Fun15 Sec06:
   ^   - get spoligotype spacer sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
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
      goto err_fun15_sec09;

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
   ^ Fun15 Sec07:
   ^   - get spoligotype lineage database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   argAryStr[*argLenSIPtr] =
      malloc(16 * sizeof(signed char));
   if(! argAryStr[*argLenSIPtr])
      goto err_fun15_sec09;
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
     goto err_fun15_sec09;

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
   ^ Fun15 Sec08:
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
   ^ Fun15 Sec09:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   err_fun15_sec09:;
      return 1;
} /*getDatbases_ftbRayST*/

/*-------------------------------------------------------\
| Fun16: mkCoverageTbl_ftbRayST
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
\-------------------------------------------------------*/
signed int
mkCoverageTbl_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed char lineStr[4096];
   signed int lenSI = 0;
   signed int posSI = 0;
   float percF = 0;
   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec02:
   ^   - open gene coverage file and get past header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   clear_listBox_rayWidg(guiSTPtr->geneCoverSTPtr);

   lenSI = cpStr_ulCp(lineStr, guiSTPtr->filePrefixStr);
   cpStr_ulCp(
      &lineStr[lenSI],
      (signed char *) "-geneCoverage.tsv"
   );

   inFILE = fopen((char *) lineStr, "r");
   if(! inFILE)
     return 0;


   if( ! fgets((char *) lineStr, 4088, inFILE) )
      return 0; /*empty file*/

   if(
      addItem_listBox_rayWidg(
         (signed char *) "gene    %_cover  drugs...",
         def_listSpecial_rayWidg, /*does nothing*/
         guiSTPtr->geneCoverSTPtr,
         guiSTPtr->widgSTPtr
      )
   ) return 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun16 Sec03:
   ^   - read in gene coverage file entries
   ^   o fun16 sec03 sub01:
   ^     - get gene name and percent coverage
   ^   o fun16 sec03 sub02:
   ^     - add drug resistance a gene mutation can cause
   ^   o fun16 sec03 sub03:
   ^     - add gene entry to list box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   /*****************************************************\
   * Fun16 Sec03 Sub01:
   *   - get gene name and percent coverage
   \*****************************************************/

   
   while( fgets((char *) lineStr, 4088, inFILE) )
   { /*Loop: get the gene coverage and gene*/
      lenSI = 0;
      while(lineStr[lenSI] > 32)
         ++lenSI;

      lineStr[lenSI++] = ' ';
      posSI = lenSI;

      while(lineStr[lenSI] && lineStr[lenSI] < 33)
         ++lenSI;

      lenSI += strToF_base10str(&lineStr[lenSI], &percF);
      if(lineStr[lenSI] > 32)
         continue; /*bad line*/

      /*add in padding and column spacer*/
      while(posSI < 8)
         lineStr[posSI++] = ' ';

      /*add the percent coverage back into the line at the
      `   correct position
      */
      lenSI = posSI + 1;
      posSI += double_numToStr(&lineStr[posSI], percF, 2);

      if(lenSI == posSI)
      { /*If: added 0*/
         lineStr[posSI++] = '.';
         lineStr[posSI++] = '0';
         lineStr[posSI++] = '0';
      } /*If: added 0*/

      lineStr[posSI++] = ' ';
      lineStr[posSI++] = ' ';
      lineStr[posSI++] = ' ';
      lineStr[posSI++] = ' ';
      lineStr[posSI++] = ' ';

      /**************************************************\
      * Fun16 Sec03 Sub02:
      *   - add drug resistance a gene mutation can cause
      \**************************************************/

      if(! eqlWhite_ulCp(lineStr, (signed char *) "atpE"))
         posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Bdq  na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "ahpc")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Inh  Pmd  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "ddn")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Bdq  Pmd  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "eis")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Amk  Kan  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "embA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Emb  na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "embB")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Emb  na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "ethA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Eto  na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fabG1")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Eto  Inh  Pmd  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fbiA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cfz  Dlm  Pmd  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fbiB")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cfz  Dlm  Pmd  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fbiC")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cfz  Dlm  Pmd  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fbiD")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Dlm	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "fgd1")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cfz  Dlm  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "gidB")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Stm  na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "gyrA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Flq	 Lfx  Mfx  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "gyrB")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Flq	 Lfx  Mfx  na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "inhA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Eto	 Inh  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "katG")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Inh	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "pepQ")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Bdq	 Cfz  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "pncA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Pza	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "rplC")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Lzd	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "rpoB")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Rif	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "rpsL")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Stm	 na   na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "rrl")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cap	 Lzd  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "rrs")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Amk	 Cap  Kan  Stm"
            );

      else if(
         ! eqlWhite_ulCp(lineStr,(signed char *) "Rv0678")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Bdq	 Cfz  na   na"
            );

      else if(
         ! eqlWhite_ulCp(lineStr, (signed char *) "tlyA")
      ) posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "Cap	 na   na   na"
            );

      else
         posSI +=
            cpStr_ulCp(
               &lineStr[posSI],
               (signed char *) "na 	 na   na   na"
            );

      /**************************************************\
      * Fun16 Sec03 Sub03:
      *   - add gene entry to list box
      \**************************************************/

      lineStr[posSI] = 0;

      if(
         addItem_listBox_rayWidg(
             lineStr,
             def_listSpecial_rayWidg, /*does nothing*/
             guiSTPtr->geneCoverSTPtr,
             guiSTPtr->widgSTPtr
         )
      ) return 1;
   } /*Loop: get the gene coverage and gene*/

   return 0;
} /*mkCoverageTbl_ftbRayST*/

/*-------------------------------------------------------\
| Fun17: checkRunEvent_ftbRayST
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
   ' Fun17 TOC:
   '   - checks for an event, and if can runs found event
   '   o fun17 sec01:
   '     - variable declarations
   '   o fun17 sec02:
   '     - get and check events
   '   o fun17 sec06:
   '     - handle running button events
   '   o fun17 sec07:
   '     - return results and redraw gui
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpHeapStr = 0;
   signed char buildReportBl = 0;

   /*for reading the config file*/
   #define def_lineLen_fun17 1024
   signed char lineStr[def_lineLen_fun17 + 8];
   signed char logFileStr[def_lineLen_fun17 + 8];
   FILE *inFILE = 0;
   signed long discardSL = 0; /*for reading files*/

   signed char refStr[def_lineLen_fun17 + 8];
   signed char minimap2Str[def_lineLen_fun17 + 8];

   /*for buiding freezeTB run command*/
   signed char *argAryStr[1024];
      /*just give more memory then I will ever need*/
   signed int argLenSI = 0;

   struct event_rayWidg eventStackST;
   signed int indexSI = 0;
   signed int tmpSI = 0;
   signed int siCnt = 0;

   struct files_rayWidg *fileSTPtr = 0;

   for(tmpSI = 0; tmpSI < 1024; ++tmpSI)
      argAryStr[tmpSI] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - get and check events
   ^   o fun17 sec02 sub01:
   ^     - get event and check entery event
   ^   o fun17 sec02 sub02:
   ^     - check which event I am running
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun17 Sec02 Sub01:
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
      goto done_fun17_sec07;
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
      goto done_fun17_sec07;
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
      goto done_fun17_sec07;
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
      goto done_fun17_sec07;
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
      goto done_fun17_sec07;
   } /*If: was a entry box input event*/

   /*****************************************************\
   * Fun17 Sec02 Sub02:
   *   - check which event I am running
   \*****************************************************/

   if(eventStackST.idSI == guiSTPtr->fqButIdSI)
      goto getFqFiles_fun17_sec06_sub02;

   else if(eventStackST.idSI == guiSTPtr->outDirIdSI)
      goto getOutDir_fun17_sec06_sub03;

   else if(eventStackST.idSI == guiSTPtr->configIdSI)
      goto getConfigFile_fun17_sec06_sub04;

   else if(eventStackST.idSI == guiSTPtr->runIdSI)
      goto runFtb_fun17_sec06_sub06;

   else if(
      eventStackST.idSI == guiSTPtr->reportGuiIdSI
   ) goto reportMenu_fun17_sec06_sub10;

   else if(eventStackST.idSI==guiSTPtr->inputGuiIdSI)
      goto inputMenu_fun17_sec06_sub07;

   else if(eventStackST.idSI == guiSTPtr->outGuiIdSI)
      goto outputMenu_fun17_sec06_sub08;

   else if(eventStackST.idSI == guiSTPtr->amrsGuiIdSI)
      goto amrTblMenu_fun17_sec06_sub12;

   else if(eventStackST.idSI == guiSTPtr->getOutButSI)
      goto buildOutReport_fun17_sec06_sub0x;

   else if(eventStackST.idSI == guiSTPtr->coverGuiIdSI)
      goto geneCoverMenu_fun17_sec06_sub13;

   else if(eventStackST.idSI==guiSTPtr->getPrefixButIdSI)
       goto getFtbPrefix_fun17_sec06_sub11;

   else if(eventStackST.idSI == guiSTPtr->getOutButSI)
      goto buildOutReport_fun17_sec06_sub0x;

   else if(eventStackST.parIdSI == guiSTPtr->mesgBoxIdSI)
      goto mesgBox_fun17_sec06_sub01;

   else if(
      eventStackST.parIdSI == guiSTPtr->fileBrowserIdSI
   ) goto fileBrowser_fun17_sec06_sub05;

   goto done_fun17_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec06:
   ^   - handle running button events
   ^   o fun17 sec06 sub01:
   ^     - message box event
   ^   o fun17 sec06 sub02:
   ^     - get fastq files event
   ^   o fun17 sec06 sub03:
   ^     - get output directory event
   ^   o fun17 sec06 sub04:
   ^     - get configuration file event
   ^   o fun17 sec06 sub05:
   ^     - file browser event actions
   ^   o fun17 sec06 sub06:
   ^     - run event actions
   ^   o fun17 sec06 sub07:
   ^     - button pressed to build the output report
   ^   o fun17 sec06 sub08:
   ^     - goto output menu
   ^   o fun14 sec04 sub09:
   ^     - build amr table entry
   ^   o fun17 sec06 sub10:
   ^     - got to report
   ^   o fun17 sec06 sub11:
   ^     - get ftb prefix
   ^   o fun17 sec06 sub12:
   ^     - goto to the amr table
   ^   o fun17 sec06 sub13:
   ^     - goto to gene coverage table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun17 Sec06 Sub01:
   *   - message box event
   \*****************************************************/

   mesgBox_fun17_sec06_sub01:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      mesgBoxEvent_rayWidg(
         2, /*rease key event*/
         eventStackST.parIdSI,
         eventStackST.idSI,
         guiSTPtr->widgSTPtr
      );
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub02:
   *   - get fastq files event
   \*****************************************************/

   getFqFiles_fun17_sec06_sub02:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 0;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select fastq files to run"
      );
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub03:
   *   - get output directory event
   \*****************************************************/

   getOutDir_fun17_sec06_sub03:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 1;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select output folder"
      );
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub04:
   *   - get configuration file event
   \*****************************************************/

   getConfigFile_fun17_sec06_sub04:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 2;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select FTB configuration file"
      );
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub05:
   *   - file browser event actions
   *   o fun17 sec06 sub05 cat01:
   *     - run file brower event
   *   o fun17 sec06 sub05 cat02:
   *     - cancel event
   *   o fun17 sec06 sub05 cat03:
   *     - selected output directory
   *   o fun17 sec06 sub05 cat04:
   *     - selected configuration file
   *   o fun17 sec06 sub05 cat05:
   *     - selected fastq files
   *   o fun17 sec06 sub05 cat05:
   *     - error or no event
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun17 Sec06 Sub05 Cat01:
   +   - run file brower event
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fileBrowser_fun17_sec06_sub05:;
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

         default: goto done_fun17_sec07;
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
      + Fun17 Sec06 Sub05 Cat02:
      +   - cancel event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpSI == 2)
      { /*If: hit cancel*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
          ); /*use hit cancel*/

          goto done_fun17_sec07;
      } /*If: hit cancel*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub05 Cat03:
      +   - selected output directory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI == 1)
      { /*Else If: files were selected*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
         ); /*user hit select*/

         switch(guiSTPtr->browserSC)
         { /*Switch: find which browser using*/
            case 1:
            /*Case: output directory selected*/
               tmpSI = 0;
               tmpHeapStr =
                  getFile_files_rayWidg(
                     &tmpSI,
                     0, /*only selected items*/
                     fileSTPtr
                  );
               if(! tmpHeapStr)
                  goto err_fun17_sec07;

               cpStr_ulCp(guiSTPtr->outDirStr,tmpHeapStr);
               free(tmpHeapStr);
               tmpHeapStr = 0;
               goto done_fun17_sec07;
            /*Case: output directory selected*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun17 Sec06 Sub05 Cat04:
            +   - selected configuration file
            \+++++++++++++++++++++++++++++++++++++++++++*/

            case 2:
            /*Case: configuration file selected*/
               tmpSI = 0;
               tmpHeapStr =
                  getFile_files_rayWidg(
                     &tmpSI,
                     0, /*only selected items*/
                     fileSTPtr
                  );
               if(! tmpHeapStr)
                  goto err_fun17_sec07;

               cpStr_ulCp(
                  guiSTPtr->configFileStr,
                  tmpHeapStr
               );
               free(tmpHeapStr);
               tmpHeapStr = 0;
               goto done_fun17_sec07;
            /*Case: configuration file selected*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun17 Sec06 Sub05 Cat05:
            +   - get old ftb prefix
            \+++++++++++++++++++++++++++++++++++++++++++*/

            case 3:
            /*Case: select old ftb prefix*/
               tmpSI = 0;
               tmpHeapStr =
                  getFile_files_rayWidg(
                     &tmpSI,
                     0, /*only selected items*/
                     fileSTPtr
                  );
               if(! tmpHeapStr)
                  goto err_fun17_sec07;

               cpStr_ulCp(
                  guiSTPtr->filePrefixStr,
                  tmpHeapStr
               );
               free(tmpHeapStr);
               tmpHeapStr = 0;

               /*remove suffix ("-depths.tsv") ftb adds*/
               tmpHeapStr = guiSTPtr->filePrefixStr;
               tmpHeapStr += endStr_ulCp(tmpHeapStr) - 11;
               *tmpHeapStr = 0;
               tmpHeapStr = 0;

               goto done_fun17_sec07;
            /*Case: select old ftb prefix*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun17 Sec06 Sub05 Cat06:
            +   - selected fastq files
            \+++++++++++++++++++++++++++++++++++++++++++*/

            case 0:
            /*Case: fastq files selected*/
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
                     goto err_fun17_sec07;

                  if(
                     add_str_ptrAry(
                        tmpHeapStr,
                        guiSTPtr->fqStrSTPtr,
                        guiSTPtr->fqStrSTPtr->lenSL
                     )
                  ) goto err_fun17_sec07;

                  free(tmpHeapStr);
                  tmpHeapStr = 0;
               } /*Loop: get fastq files*/

               goto done_fun17_sec07;
            /*Case: fastq files selected*/
         } /*Switch: find which browser using*/
      } /*Else If: files were selected*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub05 Cat05:
      +   - error or no event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI < -2)
         goto err_fun17_sec07;

      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub06:
   *   - run event actions
   *   o fun17 sec06 sub06 cat01:
   *     - check if everything was input
   *   o fun17 sec06 sub06 cat02:
   *     - read in the config file
   *   o fun17 sec06 sub06 cat03:
   *     - build prefix & make output directory
   *   o fun17 sec06 sub06 cat04:
   *     - build the log file
   *   o fun17 sec06 sub06 cat05:
   *     - check if can run minimap2
   *   o fun17 sec06 sub06 cat06:
   *     - find length of minimap2 command and get memory
   *   o fun17 sec06 sub06 cat07:
   *     - build minimap2 command
   *   o fun17 sec06 sub06 cat08:
   *     - add the -sam <file>.sam entry to ftb
   *   o fun17 sec06 sub06 cat09:
   *     - run minimap2
   *   o fun17 sec06 sub06 cat10:
   *     - if cannot, copy fastq files to ftb command
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun17 Sec06 Sub06 Cat01:
   +   - check if everything was input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   runFtb_fun17_sec06_sub06:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

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

         goto done_fun17_sec07;
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
         goto done_fun17_sec07;
      } /*Else If: no prefix input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat02:
      +   - read in the config file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      argLenSI = 1; /*0 is always reserved*/

      /*get database paths and check for minimap2*/
      if(
         getDatabases_ftbRayST(
            (signed char *) GetApplicationDirectory(),
            refStr,
            minimap2Str,
            argAryStr,
            &argLenSI
         )
      ) goto err_fun17_sec07;

      if(guiSTPtr->configFileStr[0])
      { /*If: user provided a configuration file*/
         inFILE =
            fopen((char *) guiSTPtr->configFileStr, "r");

         while(
            getLine_fileFun(
               inFILE,
               lineStr,
               def_lineLen_fun17,
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
               goto err_fun17_sec07;

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
               goto err_fun17_sec07;
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

            ++argLenSI;
            tmpHeapStr = 0;
         } /*Loop: read in configuration file*/

         tmpHeapStr = 0;

         fclose(inFILE);
         inFILE = 0;
      } /*If: user provided a configuration file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat03:
      +   - build prefix and make output directory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*prefix tag*/
      argAryStr[argLenSI] =
         malloc((7 + 8) * sizeof(signed char));

      if(! argAryStr[argLenSI])
         goto err_fun17_sec07;
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
         goto err_fun17_sec07;
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
         goto done_fun17_sec07;
      } /*If: could not make the output directory*/

      argAryStr[argLenSI][tmpSI++] = def_pathSep_rayWidg;
      tmpSI +=
         cpStr_ulCp(
            &argAryStr[argLenSI][tmpSI],
            guiSTPtr->inPrefixStr
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat04:
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

      ++argLenSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat05:
      +   - check if can run minimap2
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(minimap2Str[0])
      { /*If: have minimap2*/
         tmpSI = cpStr_ulCp(lineStr, minimap2Str);
         tmpSI +=
            cpStr_ulCp(
               &lineStr[tmpSI],
               (signed char *) " --version > "
            );
         tmpSI += cpStr_ulCp(&lineStr[tmpSI], logFileStr);
         system((char *) lineStr);

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun17 Sec06 Sub06 Cat06:
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
            goto err_fun17_sec07;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun17 Sec06 Sub06 Cat07:
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
         + Fun17 Sec06 Sub06 Cat08:
         +   - add the -sam <file>.sam entry to ftb
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         argAryStr[argLenSI] =
            malloc(11 * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun17_sec07;
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
            goto err_fun17_sec07;
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
         + Fun17 Sec06 Sub06 Cat09:
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
            goto done_fun17_sec07;
         } /*If: minimap2 errored out*/

         free(tmpHeapStr);
         tmpHeapStr = 0;
      } /*If: have minimap2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat10:
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
               goto err_fun17_sec07;

            cpLen_ulCp(
               argAryStr[argLenSI],
               guiSTPtr->fqStrSTPtr->strAry[siCnt],
               guiSTPtr->fqStrSTPtr->lenAryUI[siCnt]
            ); /*copy fastq file name*/

            ++argLenSI;
         }  /*Loop: add fastq files to ftb command*/
      } /*Else: no minimap2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun17 Sec06 Sub06 Cat11:
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
         goto done_fun17_sec07;
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
      goto buildOutReport_fun17_sec06_sub0x;

   /*****************************************************\
   * Fun17 Sec06 Sub07:
   *   - got to input menu
   \*****************************************************/

   inputMenu_fun17_sec06_sub07:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
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
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub08:
   *   - goto output menu
   \*****************************************************/

   outputMenu_fun17_sec06_sub08:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;
      hideInput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->outGuiIdSI,
         guiSTPtr->widgSTPtr
      );

      if(buildReportBl)
         goto buildOutReport_fun17_sec06_sub0x;

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

      goto done_fun17_sec07;

   buildOutReport_fun17_sec06_sub0x:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;
      if(! guiSTPtr->filePrefixStr[0])
         goto done_fun17_sec07;

      /*build drug resistance report*/
      checkDrugs_ftbRayST(guiSTPtr);
      spoligoLinGet_ftbRayST(guiSTPtr);
      miruLinGet_ftbRayST(guiSTPtr);
      mkCoverageTbl_ftbRayST(guiSTPtr);

      goto reportMenu_fun17_sec06_sub10;

   /*****************************************************\
   * Fun17 Sec06 Sub10:
   *   - got to report
   \*****************************************************/

   reportMenu_fun17_sec06_sub10:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideTable_ftbRayST(guiSTPtr);
      hideGeneCover_ftbRayST(guiSTPtr);

      inactiveAdd_widg_rayWidg(
         guiSTPtr->reportGuiIdSI,
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
      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub11:
   *   - get ftb prefix
   \*****************************************************/

   getFtbPrefix_fun17_sec06_sub11:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select a FTB output file"
      );
      guiSTPtr->browserSC = 3;
      goto fileBrowser_fun17_sec06_sub05;

   /*****************************************************\
   * Fun17 Sec06 Sub12:
   *   - goto to the amr table
   \*****************************************************/

   amrTblMenu_fun17_sec06_sub12:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
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

      goto done_fun17_sec07;

   /*****************************************************\
   * Fun17 Sec06 Sub13:
   *   - goto to gene coverage table
   \*****************************************************/

   geneCoverMenu_fun17_sec06_sub13:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun17_sec07;

      hideInput_ftbRayST(guiSTPtr);
      hideOutput_ftbRayST(guiSTPtr);
      hideReport_ftbRayST(guiSTPtr);
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

      goto done_fun17_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec07:
   ^   - return results and redraw gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto done_fun17_sec07;

   err_fun17_sec07:;
      tmpSI = 1;
      goto ret_fun17_sec07;

   done_fun17_sec07:;
      draw_gui_ftbRayST(guiSTPtr);
      tmpSI = 0;
      goto ret_fun17_sec07;

   ret_fun17_sec07:;
      if(inFILE)
         fclose(inFILE); /*never will be stdout/in/err*/
      inFILE = 0;

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
