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
'   o fun07: checkRunEvent_ftbRayST
'    - checks for an event, and if can runs found event
'    - also redraws the GUI
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards, declare widg_rayWidg, max entry box width
\-------------------------------------------------------*/

#ifndef FREEZE_TB_RAYLIB_STRUCT_H
#define FREEZE_TB_RAYLIB_STRUCT_H

struct widg_rayWidg;

/*entry box dimensions*/
#define def_widthPrefixEntry_ftbRayST 200

/*-------------------------------------------------------\
| ST01: gui_ftbRayST
|   - has widgets used in the ftbRaylib GUI
|   - TODO: add output gui
\-------------------------------------------------------*/
typedef struct gui_ftbRayST
{
   struct widg_rayWidg *widgSTPtr; /*has widgets*/

   signed char mesgStr[128];
      /*message for the message box*/

   signed char inputBl; /*on input screen*/
   signed char curDirStr[4096]; /*current location*/

   signed int inWinWidthSI;  /*GUI input window width*/
   signed int inWinHeightSI; /*GUI input window height*/
   signed int outWinWidthSI;  /*GUI output window width*/
   signed int outWinHeightSI; /*GUI output window height*/

   signed int focusSI;        /*widget focus is on*/
   signed char blinkSC;       /*status of cursor blink*/

   /*for fastq input button*/
   signed char *fileAryStr;   /*array of c-strings*/
   signed int *fileIndexArySI; /*each files array index*/
   signed int fileCntSI;       /*number of files*/

   /*prefix entry box*/
   signed char inPrefixStr[128]; /*input prefix name*/
   signed int prefixLenSI;       /*length of text*/
   signed int prefixPosArySI[2]; /*scroll and cursor pos*/
      /*0 is scroll position, 1 is cursor position*/

   /*output button*/
   signed char outDirStr[1024];

   /*for configuration file button*/
   signed char configFileStr[1024];

   /*widget ids*/
   signed int mesgBoxIdSI;
   signed int fqButIdSI;
   signed int prefixLabIdSI;
   signed int prefixEntryIdSI;
   signed int outDirIdSI;
   signed int configIdSI;
   signed int runIdSI;
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
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui to draw
| Output:
|   - Draws:
|     o current GUI state to screen
\-------------------------------------------------------*/
void
draw_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
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
| Fun07: checkRunEvent_ftbRayST
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
