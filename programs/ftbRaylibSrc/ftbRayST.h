/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hardFtbWdig SOF: Start Of File
'   - has the hardcoded parts of the rayWidg that is
'     needed for ftbRaylib
'     * drawing function
'     * check if click event function
'     * rayWidg gui structure for drawing function
'   o header:
'     - guards, declare widg_rayWidg, max entry box width
'   o .h st01: gui_ftbRayST
'     - has widgets used in the ftbRaylib GUI
'     - TODO: add output gui
'   o fun01: blank_gui_ftbRayST
'     - blanks a gui_ftbRayST structure to default values
'   o fun02: init_gui_ftbRayST
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
|   - guards, declare widg_rayWidg, max entry box width
\-------------------------------------------------------*/

#ifndef FREEZE_TB_RAYLIB_STRUCT_H
#define FREEZE_TB_RAYLIB_STRUCT_H

struct widg_rayWidg;
struct str_ptrAry;

#define def_numDrugs_ftbRayST 17

/*-------------------------------------------------------\
| ST01: gui_ftbRayST
|   - has widgets used in the ftbRaylib GUI
|   - TODO: add output gui
\-------------------------------------------------------*/
typedef struct gui_ftbRayST
{
   /*_________________general GUI_______________________*/

   struct widg_rayWidg *widgSTPtr; /*has widgets*/

   signed char mesgStr[128];
      /*message for the message box*/

   signed char screenIndexSC;
      /*0: on input screen
      ` 1: on output file load screen
      ` 2: on output report screen
      ` 3: on output amr table screen
      ` 4: on output graph screen (does not exist)
      */
   signed char appDirStr[1024]; /*directory of ftbRay*/
   signed char curDirStr[4096]; /*current location*/

   signed char blinkSC;       /*status of cursor blink*/
   signed int numFramesSI;    /*number of frames*/

   /*_________________menu buttons______________________*/

   signed int inputGuiIdSI; /*input GUI button*/
   signed int outGuiIdSI;   /*output GUI button*/
   signed int reportGuiIdSI;/*report GUI button*/
   signed int amrsGuiIdSI;  /*amr list GUI button*/
   signed int coverGuiIdSI; /*gene % coverage GUI button*/

   /*_________________input GUI_________________________*/

   /*for fastq input button*/
   struct str_ptrAry *fqStrSTPtr;
      /*c-string array with fastq files*/

   /*prefix entry box*/
   signed char inPrefixStr[128]; /*input prefix name*/
   signed int prefixLenSI;       /*length of text*/
   signed int prefixPosArySI[2]; /*scroll and cursor pos*/
      /*0 is scroll position, 1 is cursor position*/

   /*output button*/
   signed char outDirStr[1024];

   /*for configuration file button*/
   signed char configFileStr[1024];

   /*input widget ids*/
   signed int mesgBoxIdSI;
   signed int fileBrowserIdSI;
   signed int fqButIdSI;
   signed int fqLabIdSI;
   signed int prefixLabIdSI;
   signed int prefixEntryIdSI;
   signed int outDirIdSI;
   signed int outDirLabIdSI;
   signed int configIdSI;
   signed int configLabIdSI;
   signed int runIdSI;

   signed char browserSC;
      /*0 for fq, 1 for output directory, 3 for config*/
   struct files_rayWidg *fqFileSTPtr;
      /*for fastq file browser*/
   struct files_rayWidg *outDirSTPtr;
      /*for getting output directory*/
   struct files_rayWidg *configFileSTPtr;
      /*for getting configuration directory*/

   signed char fileMesgStr[128];

   /*_________________output GUI________________________*/

   /*output widget ids*/
   signed int getPrefixButIdSI;
   signed int getPrefixLabIdSI;
      /*gets file to get prefix from*/
   signed int minAmrPercEntryIdSI;
   signed int minAmrPercLabIdSI;
      /*minimum AMR % support*/
   signed int minAmrIndelPercEntryIdSI;
   signed int minAmrIndelPercLabIdSI;
      /*minimum AMR indel % support*/
   signed int getOutButSI;
      /*get output and build report*/

   struct files_rayWidg *oldFtbFileSTPtr;
      /*for FTB output file browser*/
   signed char filePrefixStr[128]; /*prefix for files*/

   /*minimum AMR percent text*/
   signed char amrSupStr[128]; /*minimum amr % support*/
   signed int amrSupLenSI;       /*length of text*/
   signed int amrSupPosArySI[2]; /*scroll and cursor pos*/
      /*0 is scroll position, 1 is cursor position*/

   /*minimum AMR indel percent text*/
   signed char indelSupStr[128]; /*minimum amr % support*/
   signed int indelSupLenSI;       /*length of text*/
   signed int indelSupPosArySI[2]; /*scroll & cursor pos*/
      /*0 is scroll position, 1 is cursor position*/
   
   /*_________________report_GUI________________________*/
   signed int spoligoLabIdSI;  /*spoligotype reporting*/
   signed int spoligoStrainLabIdSI;
   signed int spoligoLineageLabIdSI;
   signed int miruLabIdSI;     /*miru lineage reporting*/
   signed int drugResRectIdSI; /*first item in list*/

   unsigned int drugBackColUI[def_numDrugs_ftbRayST];
   unsigned int drugTextColUI[def_numDrugs_ftbRayST];

   signed char spoligoStr[256]; /*gets spoligotype*/
   signed char spoligoStrainStr[256];
   signed char spoligoLineageStr[256];

   signed char miruStr[256];    /*gets MIRU-VNTR lineage*/

   /*__________________table_GUI________________________*/
   signed int amrTblIdSI;
   signed int amrLabIdSI;
   struct listBox_rayWidg *amrListSTPtr;

   /*_______________gene_coverage_GUI___________________*/
   signed int geneCoverTblIdSI;
   signed int geneCoverLabIdSI;
   struct listBox_rayWidg *geneCoverSTPtr;
}gui_ftbRayST;

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
);

/*-------------------------------------------------------\
| Fun02: init_gui_ftbRayST
|   - initialize a gui_ftbRayST structure to default
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
);

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
);

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
);

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
);

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
);

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
);

#endif
