/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' rayWidg SOF: Start Of File
'   - widgets or other functions to manipulate raylib and
'     make the GUI building step eaiser
'   o header:
'     - guards and defiend variables
'   o Table Of Contents
'     * sof01:
'       - structures
'     * sof02:
'       - widg_rayWidg struct functions
'     * sof03:
'       - event_rayWidg structure functions
'     * sof04:
'       - get OS state
'     * sof05:
'       - widget modification or focs/state changes
'     * sof06:
'       - string functions
'     * sof07:
'       - file_rayWidg struct and file functions
'     * sof08:
'       - widget drawing
'   * Sof01: structures
'     o .h st01: widg_rayWidg
'       - has widget ids, coordiantes, and dimensions
'     o .h st02: event_rayWidg
'       - stores mouse, keyboard, & scroll wheel events
'     o .h st03: listBox_rayWidg
'       - structure to store values in a list box
'       - also grouped macros for list box states here
'     o .h st04: files_rayWidg
'       - structure with files listed in a file browser
'   * Sof02: widg_rayWidg struct functions
'       + sof02 sec01:
'         - widg_rayWidg struct managment functions
'       + sof02 sec02:
'         - change font or font variables in widg_rayWidg
'       + sof02 sec03:
'         - widg_rayWidg struct querying functions
'       + sof02 sec04:
'         - widg_rayWidg struct modify coordiantes & size
'       + sof02 sec05:
'         - widg_rayWidg struct modify state of widgets
'       + sof02 sec06:
'         - widg_rayWidg struct clear state of widgets
'     + Sof02 Sec01:
'       - widg_rayWidg struct managment functions
'       o fun001: idBlank_widg_rayWidg
'         - blanks a widget id in a widg_rayWidg struct
'       o fun002: blank_widg_rayWidg
'         - blanks a widg_rayWidg struct
'       o fun003: init_widg_rayWidg
'         - initializes a widg_rayWidg struct
'       o fun004: setWinDim_rayWidg
'         - set height and width of the window
'       o fun005: getScale_widg_rayWidg
'         - find how much to scale the GUI by, & scale it
'       o fun006: setup_widg_rayWidg
'         - adds the default font to a rayWidg struct and
'           sets it up for use (only call once)
'         - this also starts up the raylib GUI
'       o fun007: freeStack_widg_rayWidg
'         - frees arrays in a widg_rayWidg struct
'       o fun008: freeHeap_widg_rayWidg
'         - frees a widg_rayWidg struct
'       o fun009: realloc_widg_rayWidg
'         - reallocates memory for a widg_rayWidg struct
'         - note: this will not downsize or report
'           downsize attempts
'     + Sof02 Sec02:
'       - change font or font variables in widg_rayWidg
'       o fun010: fontMeasure_widg_rayWidg
'         - measures the width of the character "D" for a
'           font
'       o fun011: textMeasure_widg_rayWidg
'         - measures the width of input text
'       o fun012: checkTextWidth_rayWidg
'         - check if text meets widgets length, if not
'           shorten
'       o fun013: backCheckTextWidth_rayWidg
'         - check if text meets widgets length, if not
'           shortens start of string until is proper
'           length
'       o fun014: changeFont_widg_rayWidg
'         - adds a font from a file to a widg_rayWidg
'       o fun015: useEmbedFont_widg_rayWidg
'         - uses an embed font
'         - make this using ExprotFontAsCode from raylib
'       o fun016: swapFont_widg_rayWidg
'         - swaps the font out with an input font pointer
'         - this is a swap of pointers only, so only free
'           the input font when you are finished
'       o fun017: setFontSize_widg_rayWidg
'         - changes font size
'       o fun018: setFontSpacing_widg_rayWidg
'         - changes a fonts spacing
'     + Sof02 Sec03:
'       - widg_rayWidg struct querying functions
'       o fun019: xCoordGet_widg_rayWidg
'         - get the x-coordinate of a widget
'       o fun020: yCoordSet_widg_rayWidg
'         - get the y-coordinate of a widget
'       o fun021: widthGet_widg_rayWidg
'         - get the width of a widget
'       o fun022: heightGet_widg_rayWidg
'         - get the height of a widget
'       o fun023: rowGet_widg_rayWidg
'         - get the row a widget is assigned to
'       o fun024: colGet_widg_rayWidg
'         - get the column a widget is assigned to
'       o fun025: stateGet_widg_rayWidg
'         - get the state of a widget
'       o fun026: pressGet_widg_rayWidg
'         - detect if widget is in pressed state
'       o fun027: checkedGet_widg_rayWidg
'         - detect if widget is in checked state
'       o fun028: activeGet_widg_rayWidg
'         - detect if widget is in active state
'       o fun029: focusGet_widg_rayWidg
'         - detect if widget is in focus state
'       o fun030: inacitveGet_widg_rayWidg
'         - detect if widget is in inacitve state
'       o fun031: hidenGet_widg_rayWidg
'         - detect if widget is in hiden state
'       o fun032: childGet_widg_rayWidg
'         - detect if widget is a child widget
'       o fun033: hogGet_widg_rayWidg
'         - detect if widget is a hog (high priority)
'           widget
'       o fun034: numWidgetGet_widg_rayWidg
'         - get number of widgets made
'       o fun035: focusWidgGet_widg_rayWidg
'         - get the id (index) of the focused widget
'     + Sof02 Sec04:
'       - widg_rayWidg struct modify coordiantes & size
'       o fun036: xCoordSet_widg_rayWidg
'         - sets a widgets x coordiante
'       o fun037: yCoordSet_widg_rayWidg
'         - sets a widgets y coordiante
'       o fun038: widthSet_widg_rayWidg
'         - sets a widgets width
'       o fun039: heightSet_widg_rayWidg
'         - sets a widgets height
'       o fun040: rowSet_widg_rayWidg
'         - sets a widgets row number
'       o fun041: colSet_widg_rayWidg
'         - sets a widgets column number
'     + Sof02 Sec05:
'       - widg_rayWidg struct modify state of widgets
'       o fun042: stateSet_widg_rayWidg
'         - sets a widgets state to input state
'       o fun043: pressAdd_widg_rayWidg
'         - adds the pressed state to a widget
'       o fun044: checkedAdd_widg_rayWidg
'         - adds the checkeded state to a widget
'       o fun045: activeAdd_widg_rayWidg
'         - adds the activeed state to a widget
'       o fun046: focusAdd_widg_rayWidg
'         - adds the activeed state to a widget
'       o fun047: inactiveAdd_widg_rayWidg
'         - adds the inactive state to a widget
'       o fun048: hidenAdd_widg_rayWidg
'         - adds the hiden state to a widget
'       o fun049: childAdd_widg_rayWidg
'         - adds the child state to a widget
'         - only use this funcition if you know what your
'           are are doing and understand how the
'           child/parent system in rayWidg works
'       o fun050: hogAdd_widg_rayWidg
'         - adds the hog state to a widget
'     + Sof02 Sec06:
'       - widg_rayWidg struct clear state of widgets
'       o fun051: stateClear_widg_rayWidg
'         - remove all states in a widget (set to 0)
'       o fun052: pressClear_widg_rayWidg
'         - remove wigets pressed state
'       o fun053: checkedClear_widg_rayWidg
'         - remove wigets checked state
'       o fun054: activeClear_widg_rayWidg
'         - remove wigets active state
'       o fun055: focusClear_widg_rayWidg
'         - remove wigets focus state
'       o fun056: inactiveClear_widg_rayWidg
'         - remove wigets & its child widgets inactive
'           state
'       o fun057: hidenClear_widg_rayWidg
'         - remove wigets and its child widgets hiden
'           state
'       o fun058: childClear_widg_rayWidg
'         - clears the child state in a widget
'         - only use this function if you know what your
'           are doing and understand how the child/parent
'           system works
'       o fun059: hogClear_widg_rayWidg
'         - clears the hog state in a widget
'   * Sof03:
'     - event_rayWidg structure functions
'     o fun060: blank_event_rayWidg
'       - sets all variables in event_rayWidg struct to
'         blank (0 or -1)
'     o fun061: init_event_rayWidg
'       - sets all variables in an event_rayWidg struct to
'         defuautls/null
'     o fun062: freeStack_event_rayWidg
'       - frees variables in an event_rayWidg struct
'     o fun063: freeHeap_event_rayWidg
'       - frees a heap allocated event_rayWidg struct
'     o fun064: get_event_rayWidg
'       - adds a user event to an event_rayWidg struct
'   * Sof04:
'     - get OS state
'     o fun066: checkGuiColorMode_rayWidg
'       - checks is user is using dark or light mode, and
'         sets the color scheme to the correct mode
'   * Sof05:
'     - widget modification or focus/state changes
'     o fun067: addWidget_widg_rayWidg
'       - add a new widget to a widg_rayWidg struct
'     o fun068: tile_widg_rayWidg
'       - find tiling x,y coordiantes for each widget
'         using widths, heights, rows, and columns
'       - note this can only support one level of children
'         widgets
'     o fun069: focusCheck_widg_rayWidg
'       - detect if input key was tab and update focus
'     o fun070: enterCheck_widg_rayWidg
'       - check if key event was an enter key
'     o fun071: getMouseWidg_widg_rayWidg
'       - get widget mouse was on
'     o fun072: blinkGet_rayWidg
'      - returns current state of cursor blink for widgets
'     o fun073: getLines_rayWidg
'       - get number of lines can fit in a set height
'     o fun074: lineToHeight_rayWidg
'       - convert line count to height in pixels
'   * Sof06:
'     - string functions
'     o fun075: intStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an integer
'     o fun076: floatStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an float
'     o fun077: fileStrCheck_rayWidg
'       - corrects non-file characters in c-string
'       - spaces/tabes converted to '_', other than that,
'         only A-Z, a-z, 0-9, '-', '_', & '.' are allowed
'     o fun078: addCharToEntry_rayWidg
'       - adds a character to an entry box text
'   * Sof07:
'     - file_rayWidg and file functions
'     o fun079: checkExt_rayWidg
'       - checks if file is the correct input extension
'   * Sof08:
'     - widget drawing
'       + s0f08 sec01:
'         - simple widgets
'       + s0f08 sec02:
'         - message box
'       + sof08 sec03:
'         - list box
'       + sof08 sec04:
'         - file browser
'     + Sof08 Sec01:
'       - simple widgets
'       o fun080: textDrawByCoord_rayWidg
'         - draws one line of text by coordinate
'       o fun081: rectDrawByCoord_rayWidg
'         - draws a rectangle with input coordinates
'       o fun083: drawRec_rayWidg
'         - draws a rectangle for rayWidg
'       o fun084: butDraw_rayWidg
'         - draw a button
'       o fun085: entryDraw_rayWidg
'         - draw an entry box and its text
'       o fun086: entryEvent_rayWidg
'         - handels an event for an entry box
'       o fun087: labDraw_rayWidg
'         - draw a label
'     + Sof08 Sec02:
'       - message box
'       o fun088: mkMesgBox_rayWidg
'         - makes an emtpy message box widget
'       o fun089: mesgBoxDraw_rayWidg
'         - draws a message box
'       o fun090: mesgBoxEvent_rayWidg
'        - checks event that was fired in a message box
'     + Sof08 Sec03:
'       - list box
'         o sof08 sec03 sub01:
'           - clear, blank, initialize, realloc, and free
'             listBox_rayWidg functions
'         o sof08 sec03 sub02:
'           - listBox_rayWidg set variables functions
'         o sof08 sec03 sub03:
'           - listBox_rayWidg get variable value functions
'         o sof08 sec03 sub04:
'           - list box widget create, draw, and event
'             handeling
'       # Sof08 Sec03 Sub01:
'         - clear, blank, initialize, realloc, and free
'           listBox_rayWidg functions
'         o fun092: blank_listBox_rayWidg
'           - blanks a listBox_rayWidg structure
'         o fun093: init_listBox_rayWidg
'           - initializes a listBox_rayWidg structure
'         o fun094: freeStack_listBox_rayWidg
'           - frees variables in a listBox_rayWidg struct
'         o fun096: realloc_listBox_rayWidg
'           - add more memory to listBox_rayWidg structure
'         o fun097: clear_listBox_rayWidg
'           - removes all items from the list box
'         o fun098: clearSelect_listBox_rayWidg
'           - clear all selections from a list box
'       # Sof08 Sec03 Sub02:
'         - listBox_rayWidg set variables functions
'         o fun099: selectMaxSet_listBox_rayWidg
'           - sets maximum number of items that can be
'             selected in a list box
'         o fun100: widthSet_listBox_rayWidg
'           - sets maximum and minimum width of a list box
'         o fun101: heightSet_listBox_rayWidg
'           - sets maximum & minimum height of a list box
'         o fun102: hiddenSet_listBox_rayWidg
'           - sets a list box item to the hiden state
'         o fun103: selectSet_listBox_rayWidg
'           - sets a list box item to the select state
'         o fun104: specialSet_listBox_rayWidg
'           - sets a list box item to the special state
'         o fun105: canSelectSet_listBox_rayWidg
'           - sets a list box item to the can select state
'       # Sof08 Sec03 Sub03:
'         - listBox_rayWidg get variables value functions
'         o fun106: hiddenGet_listBox_rayWidg
'           - gets if a list box item is in hiden state
'         o fun107: selectGet_listBox_rayWidg
'           - sets a list box item to the select state
'         o fun108: specialGet_listBox_rayWidg
'           - sets a list box item to the special state
'         o fun109: canSelectGet_listBox_rayWidg
'           - sets a list box item to the can select state
'       # Sof08 Sec03 Sub04:
'         - list box widget create, draw, and event
'           handeling
'         o fun110: mk_listBox_rayWidg
'           - makes/adds list box to a widg_widgST struct
'         o fun111: sort_listBox_rayWidg
'           - sorts a list box naturaly
'             (alphbetical + numeric)
'         o fun112: addItem_listBox_rayWidg
'           - adds an item to a list box
'         o fun113: nextItem_listBox_rayWidg
'           - move to the next item or next item(s) in a
'             listbox
'         o fun114: toIndex_listBox_rayWidg
'           - move list box selected item till at target
'             index
'         o fun115: draw_listBox_rayWidg
'           - draws a list box widget
'         o fun116: listBoxEvent_rayWidg
'           - runs an event for a list box
'     + Sof08 Sec04:
'       - file browser
'         o sof08 sec04 sub01:
'           - general functions (set, get, initialize)
'         o sof08 sec04 sub02:
'           - manipulate file browswer functions
'       # Sof08 Sec04 Sub01:
'         - general functions (set, get, initialize)
'         o fun117: blank_files_rayWidg
'           - set all files in a files_rayWidg struct to
'             no files 
'         o fun118: init_files_rayWidg
'           - initialize a files_rayWidg struct
'         o fun119: freeStack_files_rayWidg
'           - frees variables in a files_rayWidg struct
'         o fun120: freeHeap_files_rayWidg
'           - frees files_rayWidg struct
'         o fun121: setFileLim_files_rayWidg
'           - sets maximum number of files user can choose
'         o fun122: getFile_files_rayWidg
'           - gets next selected file from a files_rayWidg
'         o fun123: swapFiles_files_rayWidg
'           - swap two files in a files_rayWidg struct
'         o fun124: sortFiles_files_rayWidg
'           - sorts files by name and if directory
'         o fun125: scanForFiles_files_rayWidg
'           - get files in a directory
'         o fun126: changeExt_files_rayWidg
'           - updates files for the change extension type
'       # Sof08 Sec04 Sub02:
'         - manipulate file browswer functions
'         o fun125: addExt_files_rayWidg
'           - add an file extension to a files_rayWidg
'         o fun128: mkFileBrowser_rayWidg
'           - makes a file browswer widget
'         o fun129: fileBrowserDraw_rayWidg
'           - draws a file browswer widget
'         o fun130: fileBrowserEvent_rayWidg
'           - process and event for the file browser
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

#include <raylib.h>

#include "darkLight.h"
#include "rayWidg.h"
#include "ibmPlexMono.h" /*default font*/

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"

/*-------------------------------------------------------\
| Fun001: idBlank_widg_rayWidg
|   - blanks a widget id in a widg_rayWidg struct
| Input:
|   - idSI:
|     o widget id (index) to blank
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with id to blank
| Output:
|   - Modifies:
|     o idSI position in xArySI, yArySI, widthArySI,
|       heightArySI, and stateAryUS in widgSTPtr to be 0
|     o id position rowArySI and colArySI in widgSTPtr to
|       be -1
|     o focusSi in widgSTPtr to be -1 if widget is the
|       focused widget
\-------------------------------------------------------*/
void
idBlank_widg_rayWidg(
   signed int idSI,
   struct widg_rayWidg *widgSTPtr
){
   if(! widgSTPtr)
      return;
   else if(idSI >= widgSTPtr->sizeSI)
      return;

   widgSTPtr->xArySI[idSI] = 0;
   widgSTPtr->yArySI[idSI] = 0;
   widgSTPtr->widthArySI[idSI] = 0;
   widgSTPtr->heightArySI[idSI] = 0;

   widgSTPtr->stateAryUS[idSI] = 0;

   widgSTPtr->rowArySI[idSI] = -1;
   widgSTPtr->colArySI[idSI] = -1;

   if(widgSTPtr->focusSI == idSI)
      widgSTPtr->focusSI = -1;
} /*idBlank_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun002: blank_widg_rayWidg
|   - blanks a widg_rayWidg struct
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to blank
| Output:
|   - Modifies:
|     o lenSI in widgSTPtr to be 0 (no widgets)
|     o focusSI to be -1 (not on a widget)
\-------------------------------------------------------*/
void
blank_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   if(! widgSTPtr)
      return;

   widgSTPtr->focusSI = -1;
   widgSTPtr->lenSI = 0; /*lazy blank*/

   widgSTPtr->winOriginalWidthSI = def_winWidth_rayWidg;
   widgSTPtr->winOriginalHeightSI = def_winHeight_rayWidg;

   widgSTPtr->blinkRateSI =
      def_cursorBlinkInvterval_rayWidg;

   /*reset the color scheme to light mode*/
   checkGuiColorMode_rayWidg(widgSTPtr);
} /*blank_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun003: init_widg_rayWidg
|   - initializes a widg_rayWidg struct
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to initialize
| Output:
|   - Modifies:
|     o all variables to be set to 0/null
\-------------------------------------------------------*/
void
init_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   if(! widgSTPtr)
      return;

   widgSTPtr->xArySI = 0;
   widgSTPtr->yArySI = 0;
   widgSTPtr->widthArySI = 0;
   widgSTPtr->heightArySI = 0;
   widgSTPtr->rowArySI = 0;
   widgSTPtr->colArySI = 0;
   widgSTPtr->stateAryUS = 0;
   widgSTPtr->sizeSI = 0;


   widgSTPtr->xTargetWidthSI = def_scaleWidth_rayWidg;
   widgSTPtr->monitorNumSI = -1;
   widgSTPtr->xWidthSI = -1;
   widgSTPtr->xUsrScaleF = 0;
   widgSTPtr->xScaleF = 0;
   widgSTPtr->winWidthSI = 0;
   widgSTPtr->winHeightSI = 0;


   widgSTPtr->fontSTPtr = 0;
   widgSTPtr->freeFontBl = 0;
   widgSTPtr->originalFontSizeSI = def_fontSize_rayWidg;
   widgSTPtr->spacingF = def_spacing_rayWidg;
   widgSTPtr->fontSizeSI = def_fontSize_rayWidg;
   widgSTPtr->fontWidthF = 0;
   widgSTPtr->fontHeightF = 0;


   blank_widg_rayWidg(widgSTPtr);
} /*init_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun004: setWinDim_rayWidg
|   - set height and width of the window
| Input:
|   - widthSI:
|     o width in pixels to set window to 
|   - heightSI:
|     o height in pixels to set window to 
|   - scaleSC:
|     o 0: no scaling
|     o 1: adjust scalling for HDPI screens
|     o 2: adjust scalling for small screens
|       * I often leave this off, because I am assuming
|         the target width was setup for smaller screens
|     o 3: to scale for all screens
|   - widgSTPtr
|     o widg_rayWidg struct pointer with window width and
|       height
| Output:
|   - Modifies:
|     o winOriginalWidthSI in widgSTPtr to be widthSI
|     o winOriginalHeightSI in widgSTPtr to be heightSI
|     o winWidthSI in widgSTPtr to be scaled widthSI
|     o winHeightSI in widgSTPtr to be scaled heightSI
\-------------------------------------------------------*/
void
setWinDim_rayWidg(
   signed int widthSI,
   signed int heightSI,
   signed char scaleSC,
   struct widg_rayWidg *widgSTPtr
){
   widgSTPtr->winOriginalWidthSI = widthSI;
   widgSTPtr->winOriginalHeightSI = heightSI;

   if(widgSTPtr->xScaleF <= 0)
      goto noScale_fun004; /*nothing to scale*/
   else if( widgSTPtr->xScaleF > 1 && ! (scaleSC & 1) )
      goto noScale_fun004; /*not scaling larger screens*/
   else if( widgSTPtr->xScaleF < 1 && ! (scaleSC & 2) )
      goto noScale_fun004; /*not scaling smaller screens*/

   else
   { /*Else: screen needs to be scaled*/
      widgSTPtr->winWidthSI =
           widgSTPtr->winOriginalWidthSI
         * widgSTPtr->xScaleF;
      widgSTPtr->winHeightSI =
           widgSTPtr->winOriginalHeightSI
         * widgSTPtr->xScaleF;
   } /*Else: screen needs to be scaled*/

   SetWindowSize(
      widgSTPtr->winWidthSI,
      widgSTPtr->winHeightSI
   );

   return;

   noScale_fun004:;
      widgSTPtr->winWidthSI =
           widgSTPtr->winOriginalWidthSI;
      widgSTPtr->winHeightSI =
           widgSTPtr->winOriginalHeightSI;
} /*setWinDim_rayWidg*/

/*-------------------------------------------------------\
| Fun005: getScale_widg_rayWidg
|   - find how much to scale the GUI by
| Input:
|   - widgSTPtr:
|     o widg_rayWidg structure to get scale for
|   - scaleSC:
|     o 0: no scaling
|     o 1: adjust scalling for HDPI screens
|     o 2: adjust scalling for small screens
|       * I often leave this off, because I am assuming
|         the target width was setup for smaller screens
|     o 3: to scale for all screens
| Output:
|   - Modifies:
|     o xWidthSI, and xUsrScaleF to have the pixel width
|       and scale of the users screen
|     o xScaleF to have scale factor to use
|     o fontSizeSI, fontWidthF, and fontHeightF to have
|       xScaleF (if changed) to be applied
\-------------------------------------------------------*/
void
getScale_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed char scaleSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun005 TOC:
   '   - find how much to scale the GUI by
   '   o fun005 sec01:
   '     - variable declarations
   '   o fun005 sec02:
   '     - get mointor width, DPI, and scale
   '   o fun005 sec03:
   '     - find scale factor and scale
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun005 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0;
   Vector2 tmpVect2ST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun005 Sec02:
   ^   - get mointor width, DPI, and scale
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpSI = GetCurrentMonitor();

   if(tmpSI == widgSTPtr->monitorNumSI)
      return; /*already resized for this monitor*/

   widgSTPtr->monitorNumSI = tmpSI;
   tmpSI = GetMonitorWidth(tmpSI);
   tmpVect2ST = GetWindowScaleDPI();

   if(tmpSI != widgSTPtr->xWidthSI)
      widgSTPtr->xWidthSI = tmpSI;
   else if(
         (signed int) (100 * tmpVect2ST.x)
      == (signed int) (100 * widgSTPtr->xUsrScaleF)
   ) return; /*same scale factor*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun005 Sec03:
   ^   - find scale factor and scale
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   widgSTPtr->xUsrScaleF = tmpVect2ST.x;

   widgSTPtr->xScaleF = (float) widgSTPtr->xWidthSI;
   widgSTPtr->xScaleF /=(float) widgSTPtr->xTargetWidthSI;

   if(widgSTPtr->xUsrScaleF > 0)
      widgSTPtr->xScaleF /= widgSTPtr->xUsrScaleF;
      /*account for scaling factor the user used*/

   setFontSize_widg_rayWidg(
      widgSTPtr,
      widgSTPtr->originalFontSizeSI,
      scaleSC
   ); /*set the font size*/

   setWinDim_rayWidg(
      widgSTPtr->winOriginalWidthSI,
      widgSTPtr->winOriginalHeightSI,
      scaleSC,
      widgSTPtr
   ); /*set the window width and height by monitor*/
} /*getScale_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun006: setup_widg_rayWidg
|   - adds the default font to a rayWidg struct and sets
|     it up for use (only call once)
|   - this also starts up the raylib GUI
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add default font
|       to
|   - scaleSC:
|     o 0: no scaling
|     o 1: adjust scalling for HDPI screens
|     o 2: adjust scalling for small screens
|       * I often leave this off, because I am assuming
|         the target width was setup for smaller screens
|     o 3: to scale for all screens
| Output:
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o fontSizeF in widgSTPtr to be fontSizeSI
|     o unloadFontBl in widgSTPtr to 1 (safe to unload)
|     o freeFontBl in widgSTPtr to 1 (safe to free)
|     o spacingF in widgSTPtr to be 1
|   - Returns:
|     o 0 for success
|     o 1 for error
\-------------------------------------------------------*/
signed char
setup_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed char *titleStr,
   signed char scaleSC
){
   InitWindow(
      widgSTPtr->winOriginalWidthSI,
      widgSTPtr->winOriginalHeightSI,
      (char *) titleStr
   ); /*needed before I can measure and likely before I
      `  can check screen resolution
      */

   SetTargetFPS(def_FPS_rayWidg);

   /*load initial values so I can set and measure fonts*/
   BeginDrawing();
      ClearBackground( GetColor(widgSTPtr->guiColSI));
   EndDrawing();

   if(widgSTPtr->fontSTPtr && widgSTPtr->unloadFontBl)
      UnloadFont(*widgSTPtr->fontSTPtr);

   widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
   if(! widgSTPtr->fontSTPtr)
      goto memErr_fun006;
   widgSTPtr->freeFontBl = 1;

   *widgSTPtr->fontSTPtr = LoadFont_IbmPlexMono();
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 0;

   getScale_widg_rayWidg(widgSTPtr, scaleSC);
   return 0;

   memErr_fun006:;
      return 1;
} /*setup_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun007: freeStack_widg_rayWidg
|   - frees arrays in a widg_rayWidg struct
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with arrays to free
| Output:
|   - Modifies:
|     o all arrays in widgSTPtr to be set to 0/null
|     o lenSI and sizeSI in widgSTPtr to be 0
\-------------------------------------------------------*/
void
freeStack_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   if(! widgSTPtr)
      return;

   if(widgSTPtr->xArySI)
      free(widgSTPtr->xArySI);
   if(widgSTPtr->yArySI)
      free(widgSTPtr->yArySI);
   if(widgSTPtr->widthArySI)
      free(widgSTPtr->widthArySI);
   if(widgSTPtr->heightArySI)
      free(widgSTPtr->heightArySI);
   if(widgSTPtr->rowArySI)
      free(widgSTPtr->rowArySI);
   if(widgSTPtr->colArySI)
      free(widgSTPtr->colArySI);
   if(widgSTPtr->stateAryUS)
      free(widgSTPtr->stateAryUS);
   if(widgSTPtr->fontSTPtr)
   { /*If: need to free the font*/
      if(widgSTPtr->unloadFontBl)
         UnloadFont(*widgSTPtr->fontSTPtr);
      if(widgSTPtr->freeFontBl)
         free(widgSTPtr->fontSTPtr);
   } /*If: need to free the font*/

   init_widg_rayWidg(widgSTPtr);
} /*freeStack_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun008: freeHeap_widg_rayWidg
|   - frees a widg_rayWidg struct
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to free
| Output:
|   - Modifies:
|     o frees widgSTPtr, you must set to 0
\-------------------------------------------------------*/
void
freeHeap_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   if(! widgSTPtr)
      return;
   freeStack_widg_rayWidg(widgSTPtr);
   free(widgSTPtr);
} /*freeHeap_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun009: realloc_widg_rayWidg
|   - reallocates memory for a widg_rayWidg struct
|   - note, this will not downsize or report downsize
|     attempts
| Input:
|   - sizeSI:
|     o size of final arrays
|     o use 0 (or < 0) for this function to find if need
|       to resize (lenght >= size) and auto resize (+ 50%)
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add memory to
| Output:
|   - Modifies:
|     o all arrays to have more memory
|     o sizeSI in rayWidg to be sizeSI
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
realloc_widg_rayWidg(
   signed int sizeSI,
   struct widg_rayWidg *widgSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun009 TOC:
   '   o fun009 sec01:
   '     - variable declarations and initial check
   '   o fun009 sec02:
   '     - deal with no memory (get initial memory)
   '   o fun009 sec03:
   '     - have memory, need to reallocate
   '   o fun009 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun009 Sec01:
   ^   - variable declarations and initial check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int *siSwapPtr = 0;
   unsigned char *ucSwapPtr = 0;

   if(sizeSI <= 0)
   { /*If: user wants auto resize*/
      if(widgSTPtr->sizeSI <= 0)
         sizeSI = 16;
      else if(widgSTPtr->lenSI >= widgSTPtr->sizeSI)
         sizeSI =
            widgSTPtr->sizeSI + (widgSTPtr->sizeSI >> 1);
      else
         goto noResize_fun009_sec04;
   } /*If: user wants auto resize*/

   if(sizeSI <= widgSTPtr->sizeSI)
      goto noResize_fun009_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun009 Sec02:
   ^   - deal with no memory (get initial memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->sizeSI <= 0)
   { /*If: no memory to begin with*/
      widgSTPtr->xArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->xArySI)
         goto memErr_fun009_sec04;
      widgSTPtr->yArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->yArySI)
         goto memErr_fun009_sec04;
      widgSTPtr->widthArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->widthArySI)
         goto memErr_fun009_sec04;
      widgSTPtr->heightArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->heightArySI)
         goto memErr_fun009_sec04;

      widgSTPtr->stateAryUS =
         malloc(sizeSI * sizeof(unsigned short));
      if(! widgSTPtr->stateAryUS)
         goto memErr_fun009_sec04;

      widgSTPtr->rowArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->rowArySI)
         goto memErr_fun009_sec04;
      widgSTPtr->colArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->colArySI)
         goto memErr_fun009_sec04;

      goto done_fun009_sec04;
   } /*If: no memory to begin with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun009 Sec03:
   ^   - have memory, need to reallocate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siSwapPtr =
      realloc(
         widgSTPtr->xArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->xArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->yArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->yArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->widthArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->widthArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->heightArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->heightArySI = siSwapPtr;

   ucSwapPtr =
      (unsigned char *)
      realloc(
         widgSTPtr->stateAryUS,
         sizeSI * sizeof(unsigned short)
      );
   if(! ucSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->stateAryUS = (unsigned short *) ucSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->rowArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->rowArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->colArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun009_sec04;
   widgSTPtr->colArySI = siSwapPtr;

   goto done_fun009_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun009 Sec04:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun009_sec04:;
      widgSTPtr->sizeSI = sizeSI;
      return 0;

   noResize_fun009_sec04:;
      return 0;

   memErr_fun009_sec04:;
      return 1;
} /*realloc_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun010: fontMeasure_widg_rayWidg
|   - measures the width of the character "D" for a font
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to get font
|       measurements
| Output:
|   - Modifies:
|     o fontWidthF and fontHeightF in widgSTPtr to have
|       the width and hight of the font
\-------------------------------------------------------*/
void
fontMeasure_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   Vector2 tmpVect2 =
      MeasureTextEx(
         *widgSTPtr->fontSTPtr,
         "D",
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF
      );
   widgSTPtr->fontWidthF = tmpVect2.x;
   widgSTPtr->fontHeightF = tmpVect2.y;
} /*fontMeasure_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun011: textMeasure_widg_rayWidg
|   - measures the width of input text
| Input:
|   o textStr:
|     - c-string with text to measure
|   o heightSIPtr:
|     - signed int pionter to get height of text
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to get font
|       measurements
| Output:
|   - Modifies:
|     o heightSIPtr to have height of text
|   - Returns:
|     o size of text in pixels
\-------------------------------------------------------*/
signed int
textMeasure_widg_rayWidg(
   signed char *textStr,
   signed int *heightSIPtr,
   struct widg_rayWidg *widgSTPtr
){
   Vector2 tmpVect2 =
      MeasureTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) textStr,
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF
      );
   *heightSIPtr = tmpVect2.y;
   return tmpVect2.x;
} /*textMeasure_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun012: checkTextWidth_rayWidg
|   - check if text meets widgets length, if not shorten
| Input:
|   - textStr:
|     o c-string with text check
|   - outStr:
|     o c-string to hold ouput text
|   - widthSI:
|     o target width in pixels
|   - widgSTPtr:
|     o has font and font size
| Output:
|   - Modifies:
|     o outStr to have the contents of textStr
|       * if textStr is longer then widthSI, outStr gets
|         is shortned to widthSI and ends in `...`
|   - Returns:
|     o width of output text
\-------------------------------------------------------*/
signed int
checkTextWidth_rayWidg(
   signed char *textStr, /*text to check width of*/
   signed char *outStr,  /*gets modified text*/
   signed int widthSI,   /*width of text*/
   struct widg_rayWidg *widgSTPtr
){
   signed int textWidthSI = 0;
   signed int tmpSI = 0;
   signed int indexSI = 0;
   signed char tmpStr[2] = {0, 0};

   while(textStr[indexSI])
   { /*Loop: copy text to outStr*/
      tmpStr[0] = textStr[indexSI];

      textWidthSI +=
         textMeasure_widg_rayWidg(
            tmpStr,
            &tmpSI,
            widgSTPtr
         );

      if(textWidthSI > widthSI)
      { /*If: hit end of text lenth*/
         if(indexSI > 0)
            outStr[indexSI - 1] = '.';
         if(indexSI > 1)
            outStr[indexSI - 2] = '.';
         if(indexSI > 2)
            outStr[indexSI - 3] = '.';

         break;
      } /*If: hit end of text lenth*/

      else
         outStr[indexSI] = textStr[indexSI];

      ++indexSI;
   } /*Loop: copy text to outStr*/

   outStr[indexSI] = 0;

   /*this is needed to finalize the measurment, otherwise
   `  the width is always off
   */
   textWidthSI =
      textMeasure_widg_rayWidg(outStr, &tmpSI, widgSTPtr);

   while(textWidthSI > widthSI && indexSI)
   { /*Loop: finish finding the width*/
      if(textWidthSI > widthSI)
      { /*If: hit end of text length*/
         if(indexSI >= 0)
            outStr[indexSI--] = 0;
         if(indexSI >= 0)
            outStr[indexSI] = '.';
         if(indexSI >= 0)
            outStr[indexSI - 1] = '.';
         if(indexSI >= 0)
            outStr[indexSI - 2] = '.';
         if(indexSI <= 0)
            break;
      } /*If: hit end of text length*/

      textWidthSI =
        textMeasure_widg_rayWidg(outStr,&tmpSI,widgSTPtr);
   } /*Loop: finish finding the width*/
      
   return textWidthSI;
} /*checkTextWidth_rayWidg*/

/*-------------------------------------------------------\
| Fun013: backCheckTextWidth_rayWidg
|   - check if text meets widgets length, if not shortens
|     start of string until is proper length
| Input:
|   - textStr:
|     o c-string with text check
|   - outStr:
|     o c-string to hold ouput text
|   - widthSI:
|     o target width in pixels
|   - widgSTPtr:
|     o has font and font size
| Output:
|   - Modifies:
|     o outStr to have the contents of textStr
|       * if textStr is longer then widthSI, outStr gets
|         is shortned to widthSI and starts in `...`
|   - Returns:
|     o width of output text
\-------------------------------------------------------*/
signed int
backCheckTextWidth_rayWidg(
   signed char *textStr, /*text to check width of*/
   signed char *outStr,  /*gets modified text*/
   signed int widthSI,   /*width of text*/
   struct widg_rayWidg *widgSTPtr
){
   signed int textWidthSI = 0;
   signed int ignoreSI = 0;
   signed int indexSI = endStr_ulCp(textStr);
   signed char tmpArySC[2] = {0, 0};

   outStr[indexSI--] = 0;

   while(indexSI >= 0)
   { /*Loop: copy text to outStr*/
      tmpArySC[0] = textStr[indexSI];

      textWidthSI +=
         textMeasure_widg_rayWidg(
            tmpArySC,
            &ignoreSI,
            widgSTPtr
         );

      if(textWidthSI > widthSI)
      { /*If: hit end of text lenth*/
         outStr[indexSI] = '.';

         if(outStr[indexSI + 1])
            outStr[indexSI + 1] = '.';
         if(outStr[indexSI + 2])
            outStr[indexSI + 2] = '.';

         cpStr_ulCp(outStr, &outStr[indexSI]);

         textWidthSI = widthSI;
         indexSI = 0;
         break;
      } /*If: hit end of text lenth*/

      else
         outStr[indexSI] = textStr[indexSI];
      --indexSI;
   } /*Loop: copy text to outStr*/

   if(indexSI > 0)
      cpStr_ulCp(outStr, &outStr[indexSI]);
   return
     textMeasure_widg_rayWidg(outStr,&ignoreSI,widgSTPtr);
     /*for some odd reason the text width is off without
     `  measuring the full string
     */
} /*backCheckTextWidth_rayWidg*/

/*-------------------------------------------------------\
| Fun014: changeFont_widg_rayWidg
|   - adds a font from a file to a widg_rayWidg struct
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fileStr:
|     - file to load font from
| Output:
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o unloadFontBl in widgSTPtr to 1 (safe to unload)
|     o freeFontBl in widgSTPtr to 1 (safe to free)
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
changeFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed char *fileStr
){
   if(widgSTPtr->unloadFontBl && widgSTPtr->fontSTPtr)
      UnloadFont(*widgSTPtr->fontSTPtr);

   if(! widgSTPtr->freeFontBl)
   { /*If: need to get memory*/
      widgSTPtr->fontSTPtr = 0;
      widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
      if(! widgSTPtr->fontSTPtr)
         goto memErr_fun014;
      widgSTPtr->freeFontBl = 1;
   } /*If: need to get memory*/

   *widgSTPtr->fontSTPtr = LoadFont((char *) fileStr);
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 1;

   widgSTPtr->spacingF = def_spacing_rayWidg;
   fontMeasure_widg_rayWidg(widgSTPtr);

   return 0;

   memErr_fun014:;
      return 1;
} /*changeFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun015: useEmbedFont_widg_rayWidg
|   - uses an embed font
|   - make this using ExprotFontAsCode from raylib
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontFunction:
|     - function generated by raylib's ExportFontAsCode
| Output:
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o unloadFontBl in widgSTPtr to 1 (safe to unload)
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
useEmbedFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   struct Font (*fontFunction)(void)
      /*raylib auto generates this with ExportFontAsCode*/
){
   if(widgSTPtr->unloadFontBl && widgSTPtr->fontSTPtr)
      UnloadFont(*widgSTPtr->fontSTPtr);

   if(! widgSTPtr->freeFontBl)
   { /*If: need to get memory*/
      widgSTPtr->fontSTPtr = 0;
      widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
      if(! widgSTPtr->fontSTPtr)
         goto memErr_fun015;
      widgSTPtr->freeFontBl = 1;
   } /*If: need to get memory*/

   *widgSTPtr->fontSTPtr = (*fontFunction)();
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 0;
      /*this is hardcoded in the .h file*/
   fontMeasure_widg_rayWidg(widgSTPtr);

   return 0;

   memErr_fun015:;
      return 1;
} /*useEmbedFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun016: swapFont_widg_rayWidg
|   - swaps the font out with an input font pointer
|   - this is a swap of pointers only, so only free the
|     input font when you are finished
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontSTPtr:
|     - fontSTPtr pointer to swap to default font
| Output:
|   - Frees:
|     o old fontSTPtr (including unload if able) if
|       present and is able to free
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o freeFontBl to 0 (unsafe/do not free fontSTPtr)
|     o unloadFontBl to 0 (unsafe/do not unload fontSTPtr)
\-------------------------------------------------------*/
void
swapFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   struct Font *newFontSTPtr
){
   if(widgSTPtr->fontSTPtr && widgSTPtr->unloadFontBl)
      UnloadFont(*widgSTPtr->fontSTPtr);
   if(widgSTPtr->fontSTPtr && widgSTPtr->freeFontBl)
      free(widgSTPtr->fontSTPtr);
   widgSTPtr->fontSTPtr = 0;
   widgSTPtr->freeFontBl = 0;
   widgSTPtr->unloadFontBl = 0;

   widgSTPtr->fontSTPtr = newFontSTPtr;
   fontMeasure_widg_rayWidg(widgSTPtr);
} /*swapFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun017: setFontSize_widg_rayWidg
|   - changes font size
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontSizeSI:
|     - font size to print in GUI
|   - scaleSC:
|     o 0: no scaling
|     o 1: adjust scalling for HDPI screens
|     o 2: adjust scalling for small screens
|       * I often leave this off, because I am assuming
|         the target width was setup for smaller screens
|     o 3: to scale for all screens
| Output:
|   - Modifies:
|     o fontSizeF in widgSTPtr to have the input font size
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
\-------------------------------------------------------*/
void
setFontSize_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed int fontSizeSI,
   signed char scaleSC
){
   widgSTPtr->originalFontSizeSI = fontSizeSI;

   if(widgSTPtr->xScaleF <= 0)
      goto noScale_fun017; /*nothing to scale*/
   else if( widgSTPtr->xScaleF > 1 && ! (scaleSC & 1) )
      goto noScale_fun017; /*not scaling larger screens*/
   else if( widgSTPtr->xScaleF < 1 && ! (scaleSC & 2) )
      goto noScale_fun017; /*not scaling smaller screens*/
   else
   { /*Else: font needs to be scaled*/
      widgSTPtr->fontSizeSI =
           widgSTPtr->originalFontSizeSI
         * widgSTPtr->xScaleF;
   } /*Else: font needs to be scaled*/

   goto ret_fun017;

   noScale_fun017:;
      widgSTPtr->fontSizeSI =
         widgSTPtr->originalFontSizeSI;
      goto ret_fun017;

   ret_fun017:;
      fontMeasure_widg_rayWidg(widgSTPtr);
} /*setFontSize_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun018: setFontSpacing_widg_rayWidg
|   - changes a fonts spacing
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o spacingF:
|     - spacing to change to (float)
| Output:
|   - Modifies:
|     o spacingF in widgSTPtr to have the input spacing
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
\-------------------------------------------------------*/
void
setFontSpacing_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   float spacingF
){
   widgSTPtr->spacingF = spacingF;
   fontMeasure_widg_rayWidg(widgSTPtr);
} /*setFontSpacing_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun019: xCoordGet_widg_rayWidg
|   - get the x-coordinate of a widget
| Input:
|   - idSI:
|     o id (index) of widget to get x coordinate from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get x coordinate
|       from
| Output:
|   - Returns:
|     o x coordiante of widget
|     o def_noValue_rayWidg if widget uses tile
|       coordinates (not x,y)
|       * in this case, x and y are auto calculated, so
|         I am not returning
|       * you can get this with widgSTPtr->xArySI[idSI]
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
xCoordGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->xArySI[idSI];
} /*xCoordGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun020: yCoordGet_widg_rayWidg
|   - get the y-coordinate of a widget
| Input:
|   - idSI:
|     o id (index) of widget to get y coordinate from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get y coordinate
|       from
| Output:
|   - Returns:
|     o y coordiante of widget
|     o def_noValue_rayWidg if widget uses tile
|       coordinates (not x,y)
|       * in this case, x and y are auto calculated, so
|         I am not returning
|       * you can get this with widgSTPtr->yArySI[idSI]
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
yCoordGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->yArySI[idSI];
} /*yCoordGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun021: widthGet_widg_rayWidg
|   - get the width of a widget
| Input:
|   - idSI:
|     o id (index) of widget to get width from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get width from
| Output:
|   - Returns:
|     o width of widget
|     o def_noValue_rayWidg if widget does not have width
|       assigned
|       * in this case 0 is always the reall value
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
widthGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->widthArySI[idSI] <= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->widthArySI[idSI];
} /*widthGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun022: heightGet_widg_rayWidg
|   - get the height of a widget
| Input:
|   - idSI:
|     o id (index) of widget to get height from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get height from
| Output:
|   - Returns:
|     o height of widget
|     o def_noValue_rayWidg if widget has no height
|       * in this case 0 is always the reall value
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
heightGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->heightArySI[idSI] <= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->heightArySI[idSI];
} /*heightGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun023: rowGet_widg_rayWidg
|   - get the row a widget is assigned to
| Input:
|   - idSI:
|     o id (index) of widget to get row assignment from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get row number from
| Output:
|   - Returns:
|     o row number widget is on
|     o def_noValue_rayWidg if widget is not using the
|       tile system
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
rowGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->rowArySI[idSI] < 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->rowArySI[idSI];
} /*rowGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun024: colGet_widg_rayWidg
|   - get the column a widget is assigned to
| Input:
|   - idSI:
|     o id (index) of widget to get column number from
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get column number
| Output:
|   - Returns:
|     o column number widget is on
|     o def_noValue_rayWidg if widget is not using the
|       tile system
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
colGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else if(widgSTPtr->colArySI[idSI] < 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->colArySI[idSI];
} /*colGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun025: stateGet_widg_rayWidg
|   - get the state of a widget
| Input:
|   - idSI:
|     o id (index) of widget to get state of
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get state from
| Output:
|   - Returns:
|     o state of widget; can be any combination of the
|       following:
|       * def_press_rayWidg for mouse/enter press event
|       * def_checked_rayWidg for checked state
|       * def_active_rayWidg if widget is being used
|       * def_focus_rayWidg if widget is in focus
|       * def_inactive_rayWidg if widget is inactive
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
stateGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   else if(idSI < 0)
      return def_noWidget_rayWidg;
   else
      return widgSTPtr->stateAryUS[idSI];
} /*stateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun026: pressGet_widg_rayWidg
|   - detect if widget is in pressed state
| Input:
|   - idSI:
|     o id (index) of widget to check pressed state status
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in press state
|     o 0: if widget is not in press state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
pressGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_press_rayWidg
   ) return 1;

   else
      return 0;
} /*pressGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun027: checkedGet_widg_rayWidg
|   - detect if widget is in checked state
| Input:
|   - idSI:
|     o id (index) of widget to check checked state status
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in checked state
|     o 0: if widget is not in checked state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
checkedGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_checked_rayWidg
   ) return 1;

   else
      return 0;
} /*checkedGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun028: activeGet_widg_rayWidg
|   - detect if widget is in active state
| Input:
|   - idSI:
|     o id (index) of widget to check active state status
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in active state
|     o 0: if widget is not in active state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
activeGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_active_rayWidg
   ) return 1;

   else
      return 0;
} /*activeGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun029: focusGet_widg_rayWidg
|   - detect if widget is in focus state
| Input:
|   - idSI:
|     o id (index) of widget to check focus state status
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in focus state
|     o 0: if widget is not in focus state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
focusGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg
   ) return 1;

   else
      return 0;
} /*focusGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun030: inactiveGet_widg_rayWidg
|   - detect if widget is in inactive state
| Input:
|   - idSI:
|     o id (index) of widget to check if inactive state
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in inactive state
|     o 0: if widget is not in inactive state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
inactiveGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ) return 1;

   else
      return 0;
} /*inactiveGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun031: hidenGet_widg_rayWidg
|   - detect if widget is in hiden state
| Input:
|   - idSI:
|     o id (index) of widget to check if hiden state
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1: if widget is in hiden state
|     o 0: if widget is not in hiden state
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
unsigned char
hidenGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg
   ) return 1;

   else
      return 0;
} /*hidenGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun032: childGet_widg_rayWidg
|   - detect if widget is a child widget
| Input:
|   - idSI:
|     o id (index) of widget to check if child widget
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o index of parent widget, if widget is child widget
|     o -1: if widget is not a child widget
|     o def_noWidget_rayWidg if widget does not exist
|     o -3: if wiget is a child, but has no parent
\-------------------------------------------------------*/
signed int
childGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      ! (widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
   ) return -1;

   for( ; idSI > -1; --idSI)
   { /*Loop: find parent widget*/
      if(
        !(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
      ) return idSI; /*found parent widget*/
   } /*Loop: find parent widget*/

  return -3;/*widget assigned child state, but no parent*/
} /*childGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun033: hogGet_widg_rayWidg
|   - detect if widget is a hog (high priority) widget
| Input:
|   - idSI:
|     o id (index) of widget to check if child widget
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to check
| Output:
|   - Returns:
|     o 1 if is a hog widget
|     o 0 if not a hog (or out of bounds)
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
hogGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_hog_rayWidg
   ) return 1;

   else
      return 0;
} /*hogGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun034: numWidgetGet_widg_rayWidg
|   - get number of widgets made
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get widget count
|       from
| Output:
|   - Returns:
|     o number of widgets made
\-------------------------------------------------------*/
signed int
numWidgetGet_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   return widgSTPtr->lenSI;
} /*numWidgetGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun035: focusWidgGet_widg_rayWidg
|   - get the id (index) of the focused widget
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get focus from
| Output:
|   - Returns:
|     o wigdet in focus (< 0 means nothing in focus)
\-------------------------------------------------------*/
signed int
focusWidgGet_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   return widgSTPtr->focusSI;
} /*focusWidgGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun036: xCoordSet_widg_rayWidg
|   - sets a widgets x coordiante
| Input:
|   - xSI:
|     o x-coordiante to set widget to
|   - idSI:
|     o id (index) of widget to set x-coordinate
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noValue_rayWidg if wiget is tile system
|       * does not set coordiante
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
xCoordSet_widg_rayWidg(
   signed int xCoordSI,           /*x-coordiante*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;

  widgSTPtr->xArySI[idSI] ^= xCoordSI;
  xCoordSI ^= widgSTPtr->xArySI[idSI];
  widgSTPtr->xArySI[idSI] ^= xCoordSI;

  if(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
     return 0; /*no children to adjust*/

  xCoordSI -= widgSTPtr->xArySI[idSI];
  ++idSI;

  /*adjust the childrens x coordiantes*/
  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ){ /*Loop: adjust child widgets x-coordinates*/
     if(widgSTPtr->rowArySI[idSI] < 0)
        widgSTPtr->xArySI[idSI] -= xCoordSI;
        /*looks confusing, but if xCoordSI has (old - new)
        `   so it is negative if the new coordiante is
        `   > the old coordinate
        */
     ++idSI;
  }  /*Loop: adjust child widgets x-coordinates*/

  return 0;
} /*xCoordSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun037: yCoordSet_widg_rayWidg
|   - sets a widgets y coordiante
| Input:
|   - ySI:
|     o y-coordiante to set widget to
|   - idSI:
|     o id (indey) of widget to set y-coordinate
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noValue_rayWidg if wiget is tile system
|       * does not set coordiante
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
yCoordSet_widg_rayWidg(
   signed int yCoordSI,           /*y-coordiante*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;

  widgSTPtr->yArySI[idSI] ^= yCoordSI;
  yCoordSI ^= widgSTPtr->yArySI[idSI];
  widgSTPtr->yArySI[idSI] ^= yCoordSI;

  if(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
     return 0; /*no children to adjust*/

  yCoordSI -= widgSTPtr->yArySI[idSI];
  ++idSI;

  /*adjust the childrens y coordiantes*/
  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ){ /*Loop: adjust all y coordiantes in widgets*/
     if(widgSTPtr->rowArySI[idSI] < 0)
        widgSTPtr->yArySI[idSI] -= yCoordSI;
        /*looks confusing, but if yCoordSI has (old - new)
        `   so it is negative if the new coordiante is
        `   > the old coordinate
        */
     ++idSI;
  }  /*Loop: adjust all y coordiantes in widgets*/
  
  return 0;
} /*yCoordSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun038: widthSet_widg_rayWidg
|   - sets a widgets width
| Input:
|   - widthSI:
|     o width to set widget to set width
|   - idSI:
|     o id (index) of widget to change width of
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
widthSet_widg_rayWidg(
   signed int widthSI,            /*width*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->widthArySI[idSI] = widthSI;
  return 0;
} /*widthSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun039: heightSet_widg_rayWidg
|   - sets a widgets height
| Input:
|   - heightSI:
|     o height to set widget to set height
|   - idSI:
|     o id (index) of widget to change height of
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
heightSet_widg_rayWidg(
   signed int heightSI,           /*height*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->heightArySI[idSI] = heightSI;
  return 0;
} /*heightSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun040: rowSet_widg_rayWidg
|   - sets a widgets row number
| Input:
|   - rowSI:
|     o row number to set widget to
|     o use -1 to remove tile state (non-tile widget)
|   - idSI:
|     o id (indey) of widget to set row number of
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
rowSet_widg_rayWidg(
   signed int rowSI,              /*new row number*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->rowArySI[idSI] = rowSI;

  if(rowSI < 0)
     widgSTPtr->colArySI[idSI] = -1;
     /*removing tile state*/

  return 0;
} /*rowSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun041: colSet_widg_rayWidg
|   - sets a widgets column number
| Input:
|   - colSI:
|     o column number to set widget to
|     o use -1 to remove tile state (non-tile widget)
|   - idSI:
|     o id (indey) of widget to set column number of
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
|     have their own functions to deal with these
\-------------------------------------------------------*/
signed char
colSet_widg_rayWidg(
   signed int colSI,              /*new col number*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->colArySI[idSI] = colSI;

  if(colSI < 0)
     widgSTPtr->rowArySI[idSI] = -1;
     /*removing tile state*/

  return 0;
} /*colSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun042: stateSet_widg_rayWidg
|   - sets a widgets state to input state
| Input:
|   - stateUC:
|     o state or series of states
|       (state_1 | state_2 | ... | state_n) to set widget
|       to
|       * 0 for no/normal state
|       * def_press_rayWidg for mouse/enter press event
|       * def_checked_rayWidg for checked state
|       * def_active_rayWidg if widget is being used
|       * def_focus_rayWidg if widget is in focus
|       * def_inactive_rayWidg if widget is inactive
|   - idSI:
|     o id (indey) of widget to set state
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
| Note:
|   - assuming all states are valid
|     o currently only (state & 64) are the only possible
|       invalid states
\-------------------------------------------------------*/
signed char
stateSet_widg_rayWidg(
   unsigned char stateUC,         /*state of widget*/
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] = stateUC;

  stateUC &= def_inactive_rayWidg & def_hiden_rayWidg;

  if(! stateUC)
     return 0; /*no states to pass down to children*/
  ++idSI;

  if(idSI >= widgSTPtr->lenSI)
     return 0;

  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUS[idSI++] |= stateUC;

  return 0;
} /*stateSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun043: pressAdd_widg_rayWidg
|   - adds the pressed state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add press state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
pressAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] |= def_press_rayWidg;

  return 0;
} /*pressAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun044: checkedAdd_widg_rayWidg
|   - adds the checkeded state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add checked state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
checkedAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] |= def_checked_rayWidg;

  return 0;
} /*checkedAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun045: activeAdd_widg_rayWidg
|   - adds the activeed state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add active state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
activeAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] |= def_active_rayWidg;

  return 0;
} /*activeAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun046: focusAdd_widg_rayWidg
|   - adds the focus state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add focus state to
|     o use -1 to clear focus
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
focusAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  
  if(widgSTPtr->focusSI < 0)
     ;
  else if(widgSTPtr->focusSI > widgSTPtr->lenSI)
     ;
  else
     widgSTPtr->stateAryUS[widgSTPtr->focusSI] &=
        ~def_focus_rayWidg;

  if(idSI >= 0)
  { /*If: setting focus*/
     widgSTPtr->stateAryUS[idSI] |= def_focus_rayWidg;
     widgSTPtr->focusSI = idSI;
  } /*If: setting focus*/

  else
     widgSTPtr->focusSI = -1; /*clearing focus*/

  return 0;
} /*focusAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun047: inactiveAdd_widg_rayWidg
|   - adds the inactive state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add inactive state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
inactiveAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] |= def_inactive_rayWidg;
  if(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
     return 0; /*this is a child, so no other children*/
  ++idSI;


  if(idSI >= widgSTPtr->lenSI)
     return 0;

  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUS[idSI++] |= def_inactive_rayWidg;

  return 0;
} /*inactiveAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun048: hidenAdd_widg_rayWidg
|   - adds the hiden state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add hiden state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
hidenAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] |= def_hiden_rayWidg;
  if(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
     return 0; /*this is a child, so no other children*/
  ++idSI;

  if(idSI >= widgSTPtr->lenSI)
     return 0;

  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUS[idSI++] |= def_hiden_rayWidg;

  return 0;
} /*hidenAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun049: childAdd_widg_rayWidg
|   - adds the child state to a widget
|   - only use this funcition if you know what your are
|     doing and understand how the child/parent system
|     works
|   - rayWidg only has two levels, the parent level or
|     the child level.
|   - every child after a parent is assumed to belong to
|     the same parent
|   - a parents children end when the next parent widget
|     is found
|   * p-p-c-c-c-c-p-c-c-c (p = parent, c = child)
|       |_______| |_____|
|          1st      2nd
|     in this case there is parent 1 and its children
|     widgets and parent 2 and its children widgets,
|     each set is a different group
| Input:
|   - idSI:
|     o id (indey) of widget to add child state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
childAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] |= def_child_rayWidg;
  return 0;
} /*hidenAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun050: hogAdd_widg_rayWidg
|   - adds the hog state to a widget
| Input:
|   - idSI:
|     o id (indey) of widget to add child state to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
hogAdd_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] |= def_hog_rayWidg;
  return 0;
} /*hogAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun051: stateClear_widg_rayWidg
|   - remove all states in a widget (set to 0)
| Input:
|   - idSI:
|     o id (indey) of widget clear states
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
stateClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] = 0;
  return 0;
} /*stateClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun052: pressClear_widg_rayWidg
|   - remove wigets pressed state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
pressClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_press_rayWidg;
  return 0;
} /*pressClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun053: checkedClear_widg_rayWidg
|   - remove wigets checked state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
checkedClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_checked_rayWidg;
  return 0;
} /*checkedClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun054: activeClear_widg_rayWidg
|   - remove wigets active state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
activeClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_active_rayWidg;
  return 0;
} /*activeClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun055: focusClear_widg_rayWidg
|   - remove wigets focus state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Modifies:
|     o focusSI in widgSTPtr to be -1 if is set to idSI
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
focusClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
   if(widgSTPtr->focusSI == idSI)
      widgSTPtr->focusSI = -1;

  widgSTPtr->stateAryUS[idSI] &= ~def_focus_rayWidg;
  return 0;
} /*focusClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun056: inactiveClear_widg_rayWidg
|   - remove wigets and its child widgets inactive state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
inactiveClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;

  widgSTPtr->stateAryUS[idSI] &= ~def_inactive_rayWidg;
  if(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
     return 0;
  ++idSI;

  if(idSI >= widgSTPtr->lenSI)
     return 0;

  while(
         idSI < widgSTPtr->lenSI
      && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUS[idSI++]&= ~def_inactive_rayWidg;

  return 0;
} /*inactiveClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun057: hidenClear_widg_rayWidg
|   - remove wigets and its child widgets hiden state
| Input:
|   - idSI:
|     o id (indey) of widget to clear
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
hidenClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_hiden_rayWidg;
  ++idSI;

  if(idSI >= widgSTPtr->lenSI)
     return 0;

  while(
         idSI < widgSTPtr->lenSI
      && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUS[idSI++] &= ~def_hiden_rayWidg;

  return 0;
} /*hidenClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun058: childClear_widg_rayWidg
|   - clears the child state in a widget
|   - only use this function if you know what your are
|     doing and understand how the child/parent system
|     works
|   - rayWidg only has two levels, the parent level or
|     the child level.
|   - every child after a parent is assumed to belong to
|     the same parent
|   - a parents children end when the next parent widget
|     is found
|   * p-p-c-c-c-c-p-c-c-c (p = parent, c = child)
|       |_______| |_____|
|          1st      2nd
|     in this case there is parent 1 and its children
|     widgets and parent 2 and its children widgets,
|     each set is a different group
| Input:
|   - idSI:
|     o id (indey) of widget to clear child state
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
childClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_child_rayWidg;
  return 0;   
} /*childClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun059: hogClear_widg_rayWidg
|   - clears the hog state in a widget
| Input:
|   - idSI:
|     o id (indey) of widget to clear child state
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widget to change
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed char
hogClear_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;
   if(idSI < 0)
      return def_noWidget_rayWidg;
  widgSTPtr->stateAryUS[idSI] &= ~def_hog_rayWidg;
  return 0;   
} /*hogClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun060: blank_event_rayWidg
|   - sets all variables in an event_rayWidg struct to
|     blank (0 or -1)
| Input:
|   - eventSTPtr:
|     o pointer to event_rayWidg struct to blank
| Output:
|   - Mofidifies:
|     o all varialbes in eventSTPtr to be 0 or -1
\-------------------------------------------------------*/
void
blank_event_rayWidg(
   struct event_rayWidg *eventSTPtr
){
   if(! eventSTPtr)
      return;

   eventSTPtr->keySI = 0;
   eventSTPtr->shiftBl = 0;
   eventSTPtr->altBl = 0;
   eventSTPtr->ctrlBl = 0;
   eventSTPtr->superBl = 0;
   eventSTPtr->menuBl = 0;

   eventSTPtr->xSI = -1;
   eventSTPtr->ySI = -1;

   eventSTPtr->idSI = -1;
   eventSTPtr->parIdSI = -1;
   eventSTPtr->childIdSI = -1;

   eventSTPtr->leftPressBl = 0;
   eventSTPtr->leftReleaseBl = 0;

   eventSTPtr->scrollF = 0;

   eventSTPtr->cursorBlinkBl = 0;
} /*blank_event_rayWidg*/

/*-------------------------------------------------------\
| Fun061: init_event_rayWidg
|   - sets all variables in an event_rayWidg struct to
|     defuautls/null
| Input:
|   - eventSTPtr:
|     o pointer to event_rayWidg struct to initialize
| Output:
|   - Mofidifies:
|     o all varialbes in eventSTPtr to be defaults or null
\-------------------------------------------------------*/
void
init_event_rayWidg(
   struct event_rayWidg *eventSTPtr
){
   if(! eventSTPtr)
      return;
   blank_event_rayWidg(eventSTPtr);
   eventSTPtr->blinkCntSI = 0;
} /*init_event_rayWidg*/

/*-------------------------------------------------------\
| Fun062: freeStack_event_rayWidg
|   - frees variables in an event_rayWidg struct
| Input:
|   - eventSTPtr:
|     o pointer to event_rayWidg struct to clear
| Output:
|   - Mofidifies:
|     o all varialbes in eventSTPtr to be to an
|       initialized state
\-------------------------------------------------------*/
void
freeStack_event_rayWidg(
   struct event_rayWidg *eventSTPtr
){
   if(! eventSTPtr)
      return;
   init_event_rayWidg(eventSTPtr);
} /*freeStack_event_rayWidg*/

/*-------------------------------------------------------\
| Fun063: freeHeap_event_rayWidg
|   - frees a heap allocated event_rayWidg struct
| Input:
|   - eventSTPtr:
|     o pointer to event_rayWidg struct to free
| Output:
|   - Frees:
|     o eventSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_event_rayWidg(
   struct event_rayWidg *eventSTPtr
){
   if(! eventSTPtr)
      return;
   freeStack_event_rayWidg(eventSTPtr);
   free(eventSTPtr);
} /*freeHeap_event_rayWidg*/

/*-------------------------------------------------------\
| Fun064: get_event_rayWidg
|   - adds a user event to an event_rayWidg struct
| Input:
|   - tabBl:
|     o process tab events automatically
|   - drawFunPtr:
|     o function to draw the GUI on press and enter
|       events
|       - this function must include the BeginDrawing(),
|         ClearBackground(), and EndDrawing() functions
|         BeginDrawin();
|         ClearBackground(<raylib_color>);
|            <commands to draw GUI go here>
|         EndDrawing();
|     o 0/null: does not handle or redraw press/enter
|       events
|   - guiStructPtr:
|     o void pointer to variable/strucutre to pass to
|       drawFunPtr
|   - scaleSC:
|     o 0: no scaling
|     o 1: adjust scalling for HDPI screens
|     o 2: adjust scalling for small screens
|       * I often leave this off, because I am assuming
|         the target width was setup for smaller screens
|     o 3: to scale for all screens
|   - eventSTPtr:
|     o event_rayWidg struct pointer to get user event
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widgets
| Output:
|   - Modifies:
|     o all variables in eventSTPtr to hold a list of
|       events
|     o modifies colors in widgSTPtr to be dark or light
|       depening on current OS (Mac/Windows) light/dark
|       state
|     o uses getScale_widg_rayWidg to modify all window
|       window variables in widgSTPtr
|       * this sets the scale
|   - Returns:
|     o 0 if no events happened
|     o def_keyEvent_rayWidg if a keyboard event happened
|     o def_clickEvent_rayWidg if a click event happened
|     o def_press_rayWidg if a press (enter/left click)
|       event happened
|       * def_press_rayWidg | def_keyEvent_rayWidg is for
|         an enter
|       * def_press_rayWidg | def_clickEvent_rayWidg is
|         for an left click
|     o def_releaseEvent_rayWidg if a release event
|       for enter or a left click happened
|       * def_releaseEvent_rayWidg | def_keyEvent_rayWidg
|         if enter key was released
|       * def_releaseEvent_rayWidg |def_clickEvent_rayWidg
|         if left mouse button was released
|     o def_focus_rayWidg if a focus change event happened
|       * this is often combined with the event type
\-------------------------------------------------------*/
signed char
get_event_rayWidg(
   signed char tabBl,           /*1: handel tab events*/
   void (drawFunPtr(void *)),   /*function to redraw GUI*/
   void *guiStructPtr,          /*passed to drawFunPtr*/
   signed char scaleSC,         /*0: no scale:*/
   struct event_rayWidg *eventSTPtr, /*gets events*/
   struct widg_rayWidg *widgSTPtr    /*has widgets*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun064 TOC:
   '   - adds a user event to an event_rayWidg struct
   '   o fun064 sec01:
   '     - variable declare and blank events function
   '   o fun064 sec02:
   '     - get any key events
   '   o fun064 sec03:
   '     - get mouse events
   '   o fun064 sec04:
   '     - get scroll wheel events
   '   o fun064 sec05:
   '     - return event type
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun064 Sec01:
   ^   - variable declare and blank events function
   ^   o fun064 sec01:
   ^     - variable declarations and blanking
   ^   o fun064 sec02:
   ^     - get events and focused widget
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun064 Sec01:
   *   - variable declarations and blanking
   \*****************************************************/

   signed char eventBl = 0;
   signed int tmpSI = 0;
   signed int discardSI = 0;

   blank_event_rayWidg(eventSTPtr);

   /*****************************************************\
   * Fun064 Sec02:
   *   - get events and focused widget
   \*****************************************************/

   ++eventSTPtr->blinkCntSI;

   if(eventSTPtr->blinkCntSI >= widgSTPtr->blinkRateSI)
   { /*If: need to change the cursor state*/
      eventSTPtr->blinkCntSI = 0; /*reset*/

      eventSTPtr->cursorBlinkBl =
         ! eventSTPtr->cursorBlinkBl;
   } /*If: need to change the cursor state*/

   getScale_widg_rayWidg(widgSTPtr, scaleSC);

   /*check if user changed the light/dark mode state*/
   checkGuiColorMode_rayWidg(widgSTPtr);

   /*get pressed keys*/
   eventSTPtr->keySI = GetKeyPressed();

   eventSTPtr->xSI = GetMouseX();
   eventSTPtr->ySI = GetMouseY();

   eventSTPtr->leftReleaseBl =
      IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
   eventSTPtr->leftPressBl =
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

   eventSTPtr->scrollF = GetMouseWheelMove();


   if(eventSTPtr->keySI || eventSTPtr->scrollF != 0)
   { /*If: event using a focused widget*/
      /*need to reduce to one movement because Mac does
      `  not return integers. With raylib the scroll
      `  wheel resigtars for every click, so it is always
      `  one movement
      */
      if(eventSTPtr->scrollF <0 &&eventSTPtr->scrollF >-1)
         eventSTPtr->scrollF = -1;
      else if(
         eventSTPtr->scrollF >0 && eventSTPtr->scrollF <1
      ) eventSTPtr->scrollF = 1;

      eventSTPtr->idSI = widgSTPtr->focusSI;

      eventSTPtr->parIdSI =
         childGet_widg_rayWidg(
            widgSTPtr->focusSI,
            widgSTPtr
         );

      if(eventSTPtr->parIdSI < 0)
         eventSTPtr->parIdSI = widgSTPtr->focusSI;
      else
         eventSTPtr->childIdSI = widgSTPtr->focusSI;
   } /*If: event using a focused widget*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun064 Sec02:
   ^   - get any key events
   ^   o fun064 sec02 sub01:
   ^     - check if had key event + check for modifier key
   ^   o fun064 sec02 sub02:
   ^     - check tab keys
   ^   o fun064 sec02 sub03:
   ^     - check if enter key pressed and if setting state
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun064 Sec02 Sub02:
   *   - check if had key event + check for modifier keys
   \*****************************************************/

   if( IsKeyDown(KEY_LEFT_SHIFT) )
      eventSTPtr->shiftBl = 1;
   if( IsKeyDown(KEY_RIGHT_SHIFT) )
      eventSTPtr->shiftBl |= 2;

   /*ray widg does not detect when caps lock was
   `   released, at least not on my Linux
   */
   /*if( IsKeyDown(KEY_CAPS_LOCK) )
   {*/ /*If: user hit caps lock*/
   /*   if(eventSTPtr->shiftBl)
         eventSTPtr->shiftBl = 0;
      else
         eventSTPtr->shiftBl |= 4;
   }*/ /*If: user hit caps lock*/

   if( IsKeyDown(KEY_LEFT_ALT) )
      eventSTPtr->altBl |= 1;
   if( IsKeyDown(KEY_RIGHT_ALT) )
      eventSTPtr->altBl |= 2;

   if( IsKeyDown(KEY_LEFT_CONTROL) )
      eventSTPtr->ctrlBl |= 1;
   if( IsKeyDown(KEY_RIGHT_CONTROL) )
      eventSTPtr->ctrlBl |= 2;

   if( IsKeyDown(KEY_LEFT_SUPER) )
      eventSTPtr->superBl |= 1;
   if( IsKeyDown(KEY_RIGHT_SUPER) )
      eventSTPtr->superBl |= 2;

   if( IsKeyDown(KEY_KB_MENU) )
      eventSTPtr->menuBl |= 1;


   if(! eventSTPtr->keySI)
      goto leftClick_fun064_sec03_sub01;

   /*****************************************************\
   * Fun064 Sec02 Sub02:
   *   - check tab keys
   \*****************************************************/

   if(tabBl)
   { /*If: user wanted tab switching*/
      tmpSI =
         focusCheck_widg_rayWidg(
            eventSTPtr->keySI,
            0,
            widgSTPtr
         );

      if(tmpSI != -2)
      { /*If: user switched focus*/
         eventSTPtr->parIdSI =
            childGet_widg_rayWidg(
               tmpSI,
               widgSTPtr
            );

         if(eventSTPtr->parIdSI < 0)
         { /*If: not a child widget*/
            eventSTPtr->parIdSI = tmpSI;
            eventSTPtr->childIdSI = -1;
         } /*If: not a child widget*/

         else
            eventSTPtr->childIdSI = tmpSI;

         eventSTPtr->idSI = tmpSI;

         goto focusKeyEvent_fun064_sec05;
      } /*If: user switched focus*/
   } /*If: user wanted tab switching*/

   /*****************************************************\
   * Fun064 Sec02 Sub03:
   *   - check if enter key pressed and if setting state
   \*****************************************************/

   if(drawFunPtr)
   { /*If: redrawing GUI on enter key presses*/
      tmpSI =
         enterCheck_widg_rayWidg(
            eventSTPtr->keySI,
            0,
            &tmpSI,  /*discarding value, already know*/
            widgSTPtr
         );

      if(tmpSI != -2)
      { /*If: widget is pressable*/
         pressAdd_widg_rayWidg(
            widgSTPtr->focusSI,
            widgSTPtr
         ); /*remove press state from widget*/

         while( IsKeyDown(KEY_ENTER) )
            drawFunPtr(guiStructPtr);

         pressClear_widg_rayWidg(
            widgSTPtr->focusSI,
            widgSTPtr
         ); /*remove press state from widget*/

         goto enterKeyEvent_fun064_sec05;
      } /*If: widget is pressable*/
   } /*If: redrawing GUI on enter key presses*/

   goto retKeyEvent_fun064_sec05; /*key event*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun064 Sec03:
   ^   - get mouse events
   ^   o fun064 sec03 sub01:
   ^     - get mouse event and check if was mouse event
   ^   o fun064 sec03 sub02:
   ^     - find location of click & redraw GUI; if wanted
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun064 Sec03 Sub01:
   *   - get mouse event and check if was mouse event
   \*****************************************************/

   leftClick_fun064_sec03_sub01:;

   if(eventSTPtr->leftReleaseBl)
      eventBl = 1;

   if(eventSTPtr->leftPressBl)
      eventBl = 1;

   if(eventSTPtr->leftPressBl)
      ;
   else if(eventSTPtr->leftReleaseBl)
      ;
   else
      goto scrollCheck_fun064_sec04;

   /*****************************************************\
   * Fun064 Sec03 Sub02:
   *   - find location of click and redraw GUI (if wanted)
   \*****************************************************/

   eventSTPtr->childIdSI =
      getMouseWidg_widg_rayWidg(
         eventSTPtr->xSI,
         eventSTPtr->ySI,
         &eventSTPtr->parIdSI,
         widgSTPtr
      ); /*find widget that was clicked/released*/

   eventSTPtr->idSI = eventSTPtr->childIdSI;
   tmpSI = eventSTPtr->childIdSI;/*so can check if match*/

   if(eventSTPtr->childIdSI < 0)
      goto noEvent_fun064_sec05; /*can not click widget*/
   else if(eventSTPtr->parIdSI < 0)
   { /*Else If: not child*/
      eventSTPtr->parIdSI = eventSTPtr->childIdSI;
      eventSTPtr->idSI = eventSTPtr->parIdSI;
      eventSTPtr->childIdSI = -1;
   } /*Else If: not child*/

   if(drawFunPtr && eventSTPtr->leftPressBl)
   { /*If: user wants to redraw until a release*/
      pressAdd_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*remove press state from widget*/

      while(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
         drawFunPtr(guiStructPtr);

      pressClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*remove press state from widget*/

      eventSTPtr->leftPressBl = 0;
      eventSTPtr->leftReleaseBl = 1;

      if(
            tmpSI
         ==
            getMouseWidg_widg_rayWidg(
               GetMouseX(),
               GetMouseY(),
               &discardSI,
               widgSTPtr
            )
      ) goto clickReleaseEvent_fun064_sec05;

      else
         goto noEvent_fun064_sec05;
         /*user moved mouse of of the button*/
   } /*If: user wants to redraw until a release*/


   if(eventSTPtr->leftPressBl)
      goto clickPressEvent_fun064_sec05;
   else
      goto clickReleaseEvent_fun064_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun064 Sec04:
   ^   - get scroll wheel events
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   scrollCheck_fun064_sec04:;

   if(! ((signed int) eventSTPtr->scrollF) )
      goto noEvent_fun064_sec05;
   else
      goto scrollEvent_fun064_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun064 Sec05:
   ^   - return event type
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   
   noEvent_fun064_sec05:; /*also event that does nothing*/
      eventBl = 0;
      goto ret_fun064_sec05;

   retKeyEvent_fun064_sec05:;
      eventBl = def_keyEvent_rayWidg;
      goto ret_fun064_sec05;

   enterKeyEvent_fun064_sec05:;
      eventBl =
          def_releaseEvent_rayWidg | def_keyEvent_rayWidg;
      goto ret_fun064_sec05;

   focusKeyEvent_fun064_sec05:;
      eventBl = def_focus_rayWidg | def_keyEvent_rayWidg;
      goto ret_fun064_sec05;

   clickPressEvent_fun064_sec05:;
      eventBl =
            def_press_rayWidg | def_clickEvent_rayWidg;
      goto ret_fun064_sec05;

   clickReleaseEvent_fun064_sec05:;
      eventBl =
            def_releaseEvent_rayWidg
          | def_clickEvent_rayWidg;
      goto ret_fun064_sec05;

   scrollEvent_fun064_sec05:;
      eventBl = def_scrollEvent_rayWidg;
      goto ret_fun064_sec05;

   ret_fun064_sec05:;
      return eventBl;
} /*get_event_rayWidg*/

/*-------------------------------------------------------\
| Fun066: checkGuiColorMode_rayWidg
|   - checks is user is using dark or light mode, and
|     sets the color scheme to the correct mode
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with color scheme
| Output:
|   - Modifies:
|     o all color variables in widg_rayWidg to match the
|       detected color scheme
\-------------------------------------------------------*/
void
checkGuiColorMode_rayWidg(
   widg_rayWidg *widgSTPtr
){
   if(getMode_darkLight() & 2)
   { /*If: dark mode is set*/
      if( IsWindowFocused() )
      { /*If: user is interacting with window*/
         widgSTPtr->guiColSI =
            def_backFocusDarkCol_rayWidg;

         widgSTPtr->textColSI =
            def_forFocusDarkCol_rayWidg;
         widgSTPtr->textAltColSI =
            def_backFocusDarkCol_rayWidg;

         widgSTPtr->borderColSI =
            def_forFocusDarkCol_rayWidg;
         widgSTPtr->activeColSI =
            def_forFocusDarkCol_rayWidg;
      } /*If: user is interacting with window*/

      else
      { /*Else: window is in the background*/
         widgSTPtr->guiColSI = def_backDarkCol_rayWidg;

         widgSTPtr->textColSI = def_forDarkCol_rayWidg;
         widgSTPtr->textAltColSI=def_backDarkCol_rayWidg;

         widgSTPtr->borderColSI = def_forDarkCol_rayWidg;
         widgSTPtr->activeColSI = def_forDarkCol_rayWidg;
      } /*Else: window is in the background*/
   } /*If: dark mode is set*/

   else
   { /*Else: light mode is set*/
      if( IsWindowFocused() )
      { /*If: user is interacting with window*/
         widgSTPtr->guiColSI =
            def_backFocusLightCol_rayWidg;

         widgSTPtr->textColSI =
            def_forFocusLightCol_rayWidg;
         widgSTPtr->textAltColSI =
            def_backFocusLightCol_rayWidg;

         widgSTPtr->borderColSI =
            def_forFocusLightCol_rayWidg;
         widgSTPtr->activeColSI =
            def_forFocusLightCol_rayWidg;
      } /*If: user is interacting with window*/

      else
      { /*Else: window is in the background*/
         widgSTPtr->guiColSI = def_backLightCol_rayWidg;

         widgSTPtr->textColSI = def_forLightCol_rayWidg;
         widgSTPtr->textAltColSI=def_backLightCol_rayWidg;

         widgSTPtr->borderColSI = def_forLightCol_rayWidg;
         widgSTPtr->activeColSI = def_forLightCol_rayWidg;
      } /*Else: window is in the background*/
   } /*Else: light mode is set*/
} /*checkGuiColorMode_rayWidg*/

/*-------------------------------------------------------\
| Fun067: addWidget_widg_rayWidg
|   - add a new widget to a widg_rayWidg struct
| Input:
|   - xSI:
|     o x coordiante of widget or column if tileBl is 1
|   - ySI:
|     o y coordiante of widget or row if tileBl is 1
|   - tileBl:
|     o is input a x,y coordiante, or tileing row,column
|       coordiante
|       * 1: is tileing coordinate system
|       * 0: is x,y coordinate system
|   - widthSI:
|     o width of the widget
|     o -1 for program to figure out width from widget
|       function calls you make
|   - heightSI:
|     o height of the widget
|     o -1 for program to figure out height from widget
|       function calls you make
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add widget to
| Output:
|   - Modifies:
|     o all variables in widgSTPtr to have input values
|   - Returns:
|     o wiget id, which is thel index of the widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed int
addWidget_widg_rayWidg(
   signed int xSI, /*x coordinate or if tiling column #*/
   signed int ySI, /*y coordinate or if tiling row #*/
   signed char tileBl, /*1: tiling (program figures x,y)*/
   signed int widthSI,/*width of the widget or -1*/
   signed int heightSI, /*height of widget or -1*/
   struct widg_rayWidg *widgSTPtr
){
   signed int idSI = 0;

   if( realloc_widg_rayWidg(0, widgSTPtr) )
      goto memErr_fun066; /*memory error while resizing*/
         /*realloc_widg_rayWig resizes the arrays by 50%
         `  if no more elements can be added to the arrays
         `  (or 16 for no elements)
         */

   idSI = widgSTPtr->lenSI;
   ++widgSTPtr->lenSI;

   idBlank_widg_rayWidg(idSI, widgSTPtr);
   widgSTPtr->widthArySI[idSI] = widthSI;
   widgSTPtr->heightArySI[idSI] = heightSI;

   if(tileBl)
   { /*If: using tiling coordinates*/
      widgSTPtr->xArySI[idSI] = 0; /*tile fun will find*/
      widgSTPtr->yArySI[idSI] = 0; /*tile fun will find*/

      widgSTPtr->rowArySI[idSI] = xSI;
      widgSTPtr->colArySI[idSI] = ySI;
   } /*If: using tiling coordinates*/

   else
   { /*If: using x,y coordinates*/
      widgSTPtr->xArySI[idSI] = xSI;
      widgSTPtr->yArySI[idSI] = ySI;

      widgSTPtr->rowArySI[idSI] = -1;
      widgSTPtr->colArySI[idSI] = -1;
   } /*If: using x,y coordinates*/

   return idSI;

   memErr_fun066:;
      return 1;
} /*addWidget_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun068: tile_widg_rayWidg
|   - find tiling x,y coordiantes for each widget using
|     widths, heights, rows, and columns
|   - note this can only support one level of children
|     widgets
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to tile
|   - rowOffsetSI:
|     o offset of first row (y-axis)
|   - colOffsetSI:
|     o offset of the first column (x-axis)
| Output:
|   - Modifies:
|     o xArySI and yArySI in widgSTPtr to have the x and
|       y coordiantes for each widget
|   - Returns:
|     o 0 for no errors or if nothing to tile 
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
tile_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr, /*has widgets to tile*/
   signed int rowOffsetSI,         /*offset for 1st row*/
   signed int colOffsetSI          /*1st column offset*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun068 TOC:
   '   - find tiling x,y coordiantes for each widget using
   '     widths, heights, rows, and columns
   '   o fun068 sec01:
   '     - variable declarations
   '   o fun068 sec02:
   '     - get maximum rows and column (and get memory)
   '   o fun068 sec03:
   '     - find parent widget row and column coordinates
   '   o fun068 sec04:
   '     - add x,y to tiled parents and update non-tiled
   '       child widgets
   '   o fun068 sec05:
   '     - find tile coordinates for tiled child widgets
   '   o fu10 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0;
   signed int idSI = 0;

   signed int *rowHeapArySI = 0;
   signed int rowLenSI = 0; /*number rows in tile set*/
   signed int rowSizeSI = 0;
   signed int siRow = 0;    /*row of widget*/
   signed int yChangeSI = 0;/*how much tile changes y*/

   signed int *colHeapArySI = 0;
   signed int colLenSI = 0; /*number columns in tile set*/
   signed int colSizeSI = 0;
   signed int siCol = 0;    /*column of widget*/
   signed int xChangeSI = 0;/*how much tile changes x*/

   signed char tileBl = 0;

   signed int rowPadSI = 0;
   signed int colPadSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec02:
   ^   - get maximum rows and column (and get memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rowPadSI =
        widgSTPtr->fontHeightF
      / def_widgHeightGap_rayWidg;
   colPadSI = widgSTPtr->fontWidthF;

   if(rowOffsetSI < 0)
      rowOffsetSI = rowPadSI; /*offset first row*/

   if(colOffsetSI < 0)
      colOffsetSI = colPadSI; /*offset first column*/

   rowSizeSI = -1;
   colSizeSI = -1;

   for(idSI = 0; idSI < widgSTPtr->lenSI; ++idSI)
   { /*Loop: find number of rows and columns*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         continue; /*is non-tile widget*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         continue; /*is non-tile widget*/
      else if(
         widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg
      ) continue;

      if(rowSizeSI < widgSTPtr->rowArySI[idSI])
         rowSizeSI = widgSTPtr->rowArySI[idSI];
      if(colSizeSI < widgSTPtr->colArySI[idSI])
         colSizeSI = widgSTPtr->colArySI[idSI];
      tileBl = 1;
   } /*Loop: find number of rows and columns*/

   if(! tileBl)
      goto noTile_fun068_sec06;

   ++rowSizeSI;
   ++colSizeSI;

   rowHeapArySI = calloc(rowSizeSI, sizeof(signed int));
   if(! rowHeapArySI)
      goto memErr_fun068_sec06;
   colHeapArySI = calloc(colSizeSI, sizeof(signed int));
   if(! colHeapArySI)
      goto memErr_fun068_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec03:
   ^   - find parent widget row and column coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rowLenSI = 0;
   colLenSI = 0;

   for(idSI = 0; idSI < widgSTPtr->lenSI; ++idSI)
   { /*Loop: find tile dimensions*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         continue; /*is non-tile widget*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         continue; /*is non-tile widget*/
      else if(
         widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ) continue; /*this is a child*/
      else if(
         widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg
      ) continue;

      siRow = widgSTPtr->rowArySI[idSI];
      siCol = widgSTPtr->colArySI[idSI];

      if(widgSTPtr->widthArySI[idSI] >colHeapArySI[siCol])
         colHeapArySI[siCol]=widgSTPtr->widthArySI[idSI];
      if(widgSTPtr->heightArySI[idSI]>rowHeapArySI[siRow])
         rowHeapArySI[siRow]=widgSTPtr->heightArySI[idSI];
 
      if(siRow > rowLenSI)
         rowLenSI = siRow;
      if(siCol > colLenSI)
         colLenSI = siCol;
   } /*Loop: find tile dimensions*/

   /*add offsets for rows (y) and columns (x)*/

   yChangeSI = rowHeapArySI[0];
   rowHeapArySI[0] = rowOffsetSI;

   for(siRow = 1; siRow <= rowLenSI; ++siRow)
   { /*Loop: find y coordinate of each row*/
      rowHeapArySI[siRow] ^= yChangeSI;
      yChangeSI ^= rowHeapArySI[siRow];
      rowHeapArySI[siRow] ^= yChangeSI;

      rowHeapArySI[siRow] += rowHeapArySI[siRow - 1];
      rowHeapArySI[siRow] += rowPadSI;
   } /*Loop: find y coordinate of each row*/

   xChangeSI = colHeapArySI[0];
   colHeapArySI[0] = colOffsetSI;

   for(siCol = 1; siCol <= colLenSI; ++siCol)
   { /*Loop: find x coordinate of each row*/
      colHeapArySI[siCol] ^= xChangeSI;
      xChangeSI ^= colHeapArySI[siCol];
      colHeapArySI[siCol] ^= xChangeSI;

      colHeapArySI[siCol] += colHeapArySI[siCol - 1];
      colHeapArySI[siCol] += colPadSI;
   } /*Loop: find x coordinate of each row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ fun068 sec04:
   ^   - add x,y to tiled parents and update non-tiled
   ^     child widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: add in x,y coordaintes*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto nextWidg_fun068_sec04;
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto nextWidg_fun068_sec04;
      else if(
         widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ) goto nextWidg_fun068_sec04;
      else if(
         widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg
      ) goto nextWidg_fun068_sec04;

      siRow = widgSTPtr->rowArySI[idSI];
      siCol = widgSTPtr->colArySI[idSI];

      yChangeSI = rowHeapArySI[siRow];
      yChangeSI -= widgSTPtr->yArySI[idSI];

      xChangeSI = colHeapArySI[siCol];
      xChangeSI -= widgSTPtr->xArySI[idSI];

      widgSTPtr->yArySI[idSI] = rowHeapArySI[siRow];
      widgSTPtr->xArySI[idSI] = colHeapArySI[siCol];

      ++idSI;

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: update x,y coord child coordinates*/

          /*check if children were tiled in the widget*/
          if(widgSTPtr->rowArySI[idSI] >= 0)
             goto nextChild_fun068_sec04;
          else if(widgSTPtr->colArySI[idSI] >= 0)
             goto nextChild_fun068_sec04;
          else if(
               widgSTPtr->stateAryUS[idSI]
             & def_hiden_rayWidg
          ) goto nextChild_fun068_sec04;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] += yChangeSI;
         widgSTPtr->xArySI[idSI] += xChangeSI;

         nextChild_fun068_sec04:;
            ++idSI;
      }  /*Loop: update x,y coord child coordinates*/

      continue; /*already on next parent wiget*/

      nextWidg_fun068_sec04:;
         ++idSI;
   } /*Loop: add in x,y coordaintes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec05:
   ^   - find tile coordinates for tiled child widgets
   ^   o fun068 sec05 sub01:
   ^     - find and setup for found child group
   ^   o fun068 sec05 sub02:
   ^     - find column and row sizes in child group
   ^   o fun068 sec05 sub03:
   ^     - add x,y coordinates in child group
   ^   o fun068 sec05 sub04:
   ^     - move to next widget
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun068 Sec05 Sub01:
   *   - find and setup for found child group
   \*****************************************************/

   rowLenSI = 0;
   colLenSI = 0;
   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: find tile dimensions*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto childNextWidg_fun068_sec05_sub05;
         /*non-tiled*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto childNextWidg_fun068_sec05_sub05;
         /*non-tiled*/
      else if(
        !(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
      ) goto childNextWidg_fun068_sec05_sub05;
        /*this is a parent*/
      else if(
         widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg
      ) goto childNextWidg_fun068_sec05_sub05;


      for(siRow = 1; siRow < rowSizeSI; ++siRow)
         rowHeapArySI[siRow] = 0;
      for(siCol = 1; siCol < colSizeSI; ++siCol)
         colHeapArySI[siCol] = 0;

      /*get x,y coordinates of parent*/
      rowHeapArySI[0] = widgSTPtr->yArySI[idSI - 1];
      colHeapArySI[0] = widgSTPtr->xArySI[idSI - 1];

      tmpSI = idSI;
      rowLenSI = 0;
      colLenSI = 0;

      /**************************************************\
      * Fun068 Sec05 Sub02:
      *   - find column and row sizes in child group
      \**************************************************/

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: find child widget row and column sizes*/

         if(widgSTPtr->rowArySI[idSI] < 0)
            goto nextChild_fun068_sec05_sub02;/*non-tiled*/
         else if(widgSTPtr->colArySI[idSI] < 0)
            goto nextChild_fun068_sec05_sub02;/*non-tiled*/
         else if(
              widgSTPtr->stateAryUS[idSI]
            & def_hiden_rayWidg
         ) goto nextChild_fun068_sec05_sub02;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];

         if(
              widgSTPtr->widthArySI[idSI]
            > rowHeapArySI[siRow]
         ) rowHeapArySI[siRow] =
               widgSTPtr->widthArySI[idSI];
         if(
              widgSTPtr->heightArySI[idSI]
            > colHeapArySI[siRow]
         ) colHeapArySI[siRow] =
               widgSTPtr->heightArySI[idSI];
 
         if(siRow > rowLenSI)
            rowLenSI = siRow;
         if(siCol > colLenSI)
            colLenSI = siCol;

         nextChild_fun068_sec05_sub02:;
            ++idSI;
      }  /*Loop: find child widget row and column sizes*/

      /*add offsets for rows (y) and columns (x)*/
      rowHeapArySI[0] += rowOffsetSI;
      colHeapArySI[0] += colOffsetSI;

      for(siRow = 1; siRow <= rowLenSI; ++siRow)
         rowHeapArySI[siRow] += rowHeapArySI[siRow - 1];
         /*find coordinate of each row; y coordiante*/
      for(siCol = 1; siCol <= colLenSI; ++siCol)
         colHeapArySI[siCol] += colHeapArySI[siCol - 1];
         /*find coordinate of each col; x coordiante*/

      /**************************************************\
      * Fun068 Sec05 Sub03:
      *   - add x,y coordinates in child group
      \**************************************************/

      idSI = tmpSI;

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: set tiled child wiget x,y coordinates*/

          /*check if children were non-tile widgets*/
          if(widgSTPtr->rowArySI[idSI] < 0)
             goto addCoordNextChild_fun068_sec05_sub03;
          else if(widgSTPtr->colArySI[idSI] < 0)
             goto addCoordNextChild_fun068_sec05_sub03;
           else if(
                widgSTPtr->stateAryUS[idSI]
              & def_hiden_rayWidg
           ) goto addCoordNextChild_fun068_sec05_sub03;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] = rowHeapArySI[siRow];
         widgSTPtr->xArySI[idSI] = colHeapArySI[siCol];

         addCoordNextChild_fun068_sec05_sub03:;
            ++idSI;
      }  /*Loop: set tiled child wiget x,y coordinates*/

      /**************************************************\
      * Fun068 Sec05 Sub04:
      *   - move to next widget
      \**************************************************/

      continue; /*on next parent widget*/

      childNextWidg_fun068_sec05_sub05:;
         ++idSI;
   } /*Loop: find tile dimensions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fu10 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;
   goto ret_fun068_sec06;

   noTile_fun068_sec06:;
      idSI = 0;
      goto ret_fun068_sec06;

   memErr_fun068_sec06:;
      idSI = 1;
      goto ret_fun068_sec06;

   ret_fun068_sec06:;
      if(rowHeapArySI)
         free(rowHeapArySI);
      rowHeapArySI = 0;

      if(colHeapArySI)
         free(colHeapArySI);
      colHeapArySI = 0;

      return idSI;
} /*tile_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun069: focusCheck_widg_rayWidg
|   - detect if input key was tab and update focus
| Input:
|   - keySI:
|     o raylib key returned from GetKeyPressed() 
|   - charBl:
|     o 1: keySC is char from GetChar()
|       * this can only detect tab
|     o 0: keySC is key from GetKeyPressed()
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to set focus on
| Output:
|   - Modifies:
|     o focusSI in widgSTPtr to be set to focused widget
|     o stateAryUS[focus] in widgSTPtr to be set to
|       def_focus_rayWidg
|     o stateAryUS[old_focus] in widgSTPtr to have the
|       def_focus_rayWidg removed
|   - Returns:
|     - index of new focused widget (widgSTPtr->focusSI)
|     - -1 if all widgets were hiden or inactive
|       * there was nothing to focus
|     - -2 if focus was not changed (key was not tab)
\-------------------------------------------------------*/
signed int
focusCheck_widg_rayWidg(
   signed int keySI,   /*key from GetKeyPressed()*/
   signed char charBl, /*1: key was from GetChar()*/
   struct widg_rayWidg *widgSTPtr /*has focus to change*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun069 TOC:
   '   - detect if input key was tab and update focus
   '   o fun069 sec01:
   '     - find if focus and set focus
   '   o fun069 sec02:
   '     - check if next widget can be focused, if not
   '       find the next widget that can be focused
   '   o fun069 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun069 Sec01:
  ^   - find if focus and set focus
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  signed char endBl = 0;   /*mark if hit end of widgets*/
  signed char shiftBl = 0; /*if shift key was pressed*/
     /*not using alt tab, because alt tab is often a
     `  system hotkey for reverse cycle
     */
  signed char hogBl = 0;
  signed int tmpSI = 0;

  shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
  shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

  if(charBl)
  { /*If: character input*/
     if(keySI == '\t')
        charBl = shiftBl;
     else
        goto nonFocusKey_fun069_sec04;
  } /*If: character input*/

  else
  { /*Else: key event input*/
     if(keySI == KEY_TAB)
        charBl = shiftBl;
     else
        goto nonFocusKey_fun069_sec04;
  } /*Else: key event input*/

  /*quickly wipe old focus; less efficent, but bit neater
  `   code to handle in two steps
  */
  if(widgSTPtr->focusSI < 0)
     ;
  else if(widgSTPtr->focusSI < widgSTPtr->lenSI)
  { /*Else If: have a widget in focus*/
     widgSTPtr->stateAryUS[widgSTPtr->focusSI] &=
          ~def_focus_rayWidg;

     /*need to clear states that can only be on a focused
     `  widget
     */
     pressClear_widg_rayWidg(
        widgSTPtr->focusSI,
        widgSTPtr
     );
     activeClear_widg_rayWidg(
        widgSTPtr->focusSI,
        widgSTPtr
     );
  } /*Else If: have a widget in focus*/

  /*make sure I am on a widget that can accept focus*/
  if(charBl)
  { /*If: moving focus to last widget*/
     if(widgSTPtr->focusSI >= widgSTPtr->lenSI)
        widgSTPtr->focusSI = widgSTPtr->lenSI - 1;
     else if(widgSTPtr->focusSI <= 0)
        widgSTPtr->focusSI = widgSTPtr->lenSI - 1;
  } /*If: moving focus to last widget*/

  else
  { /*Else: moving focus to next widget*/
     if(widgSTPtr->focusSI >= widgSTPtr->lenSI)
        widgSTPtr->focusSI = 0;
     else if(widgSTPtr->focusSI < 0)
        widgSTPtr->focusSI = 0;
  } /*Else: moving focus to next widget*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun069 Sec02:
  ^   - check if next widget can be focused, if not find
  ^     the next widget that can be focused
  ^   o fun069 sec02 sub01:
  ^     - check if changing focus on a hog widget
  ^   o fun069 sec02 sub02:
  ^     - change focus on non-hog widget
  ^   o fun069 sec02 sub03:
  ^     - reset state on focuse widget to focused
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  /******************************************************\
  * Fun069 Sec02 Sub01:
  *   - check if changing focus on a hog widget
  + Fun069 Sec02 Sub01 Cat01:
  +   - find next hog widget in focus
        + Fun069 Sec02 Sub01 Cat02:
        +   - if no next widget, see if can move to end
  \******************************************************/

  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++\
  + Fun069 Sec02 Sub01 Cat01:
  +   - find next hog widget in focus
  \+++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  /*check if next widget can be focused*/
  keySI = widgSTPtr->focusSI;

  if(
       hogGet_widg_rayWidg(
          widgSTPtr->focusSI,
          widgSTPtr
       ) == 1
  ){ /*If: on a hog widget*/
     hogBl = 1;

     if(charBl && keySI <= 0)
        goto resetState_fun069_sec02_sub03;
        /*on parent widget; can  not go back*/
     else if(! charBl && keySI >= widgSTPtr->lenSI)
        goto resetState_fun069_sec02_sub03;
        /*on last widget; can not go forward*/
     else if(
           ! charBl
        && childGet_widg_rayWidg(keySI - 1, widgSTPtr) < 0
     ) ; /*moving foward, next is not a child*/
     else if(
            charBl
         && childGet_widg_rayWidg(keySI, widgSTPtr) < 0
     ) ; /*moveing backwards, but on parent widget*/
     else
     { /*Else: can move backwards or fowards*/
        tmpSI = keySI;

        while(
            childGet_widg_rayWidg(tmpSI, widgSTPtr) >= 0
        ){ /*Loop: see if can focus a different widget*/
           if(charBl)
           { /*If: moving focus up*/
              --tmpSI;
              if(tmpSI < 0)
                 break;
           } /*If: moving focus up*/

           else
           { /*Else: moving focus down*/
              ++tmpSI;
              if(tmpSI >= widgSTPtr->lenSI)
                 break;
              else if(
                 childGet_widg_rayWidg(tmpSI,widgSTPtr) <0
              ) break; /*end of widget group*/
           } /*Else: moving focus down*/


           if( hidenGet_widg_rayWidg(tmpSI, widgSTPtr) )
              ;
           else if(
              inactiveGet_widg_rayWidg(tmpSI, widgSTPtr)
           ) ;
           else if(tmpSI != keySI)
           { /*Else: found next widget to focus*/
              keySI = tmpSI;
              goto resetState_fun069_sec02_sub03;
           } /*Else: found next widget to focus*/
        }  /*Loop: see if can focus a different widget*/

        /*+++++++++++++++++++++++++++++++++++++++++++++++\
        + Fun069 Sec02 Sub01 Cat02:
        +   - if no next widget, see if can move to end
        \+++++++++++++++++++++++++++++++++++++++++++++++*/

        if(
              charBl
           && childGet_widg_rayWidg(keySI+1,widgSTPtr) >=0
        ){ /*If: I need to shift focus to end*/
           tmpSI = keySI + 1;
           while(
                 tmpSI < widgSTPtr->lenSI
              &&    childGet_widg_rayWidg(tmpSI,widgSTPtr)
                 >= 0
           ) ++tmpSI;

           --tmpSI;

           while(
                 tmpSI > keySI
              && (
                    inactiveGet_widg_rayWidg(
                        tmpSI,
                        widgSTPtr
                    )
                  ||hidenGet_widg_rayWidg(tmpSI,widgSTPtr)
                 )
           ) --tmpSI;

           if(tmpSI != keySI)
           { /*If: found the last widget*/
              keySI = tmpSI;
              goto resetState_fun069_sec02_sub03;
           } /*If: found the last widget*/
        }  /*If: I need to shift focus to end*/

        /*+++++++++++++++++++++++++++++++++++++++++++++++\
        + Fun069 Sec02 Sub01 Cat03:
        +   - if no next widget, see if can move to start
        \+++++++++++++++++++++++++++++++++++++++++++++++*/

        else if(
              ! charBl
           && childGet_widg_rayWidg(keySI-1,widgSTPtr) >=0
        ){ /*Else: shift focus to start*/
           tmpSI = keySI - 1;
           while(
                 tmpSI >= 0
              &&    childGet_widg_rayWidg(tmpSI,widgSTPtr)
                 >= 0
           ) --tmpSI;

           while(
                 tmpSI < keySI
              && (
                    inactiveGet_widg_rayWidg(
                        tmpSI,
                        widgSTPtr
                    )
                  ||hidenGet_widg_rayWidg(tmpSI,widgSTPtr)
                 )
           ) ++tmpSI;

           if(tmpSI != keySI)
           { /*If: found the first widget*/
              keySI = tmpSI;
              goto resetState_fun069_sec02_sub03;
           } /*If: found the first widget*/
        } /*Else: shift focus to start*/
     } /*Else: can move backwards or fowards*/
  }  /*If: on a hog widget*/

  /******************************************************\
  * Fun069 Sec02 Sub02:
  *   - change focus on non-hog widget
  \******************************************************/

  do { /*Loop: find next widget in focus*/
     if(charBl)
     { /*If: moving focus up*/
        --keySI;

        if(keySI < 0)
        { /*If: need to move to start*/
           if(endBl)
              goto noFocus_fun069_sec04;
           keySI = widgSTPtr->lenSI - 1;
           endBl = 1;
        } /*If: need to move to start*/
     } /*If: moving focus up*/

     else
     { /*Else: moving down*/
        ++keySI;

        if(keySI >= widgSTPtr->lenSI)
        { /*If: need to move to start*/
           if(endBl)
              goto noFocus_fun069_sec04;
           endBl = 1;
           keySI = 0;
        } /*If: need to move to start*/
     } /*Else: moving down*/

     if(! hogBl)
        ;
     else if(
        !
        (widgSTPtr->stateAryUS[keySI] & def_child_rayWidg)
     ){ /*If: out of child widget range*/
        keySI = widgSTPtr->focusSI;
        goto resetState_fun069_sec02_sub03;
     }  /*If: out of child widget range*/
  } while(
    (  widgSTPtr->stateAryUS[keySI] & def_hiden_rayWidg
      ||
       widgSTPtr->stateAryUS[keySI] & def_inactive_rayWidg
    )
    && (
          ! hogBl
       || widgSTPtr->stateAryUS[keySI] & def_hog_rayWidg
    )
  ); /*Loop: find next widget in focus*/

  /******************************************************\
  * Fun069 Sec02 Sub03:
  *   - reset state on focuse widget to focused
  \******************************************************/

  resetState_fun069_sec02_sub03:;
     widgSTPtr->focusSI = keySI;
     widgSTPtr->stateAryUS[keySI] |= def_focus_rayWidg;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun069 Sec03:
  ^   - return
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  if(widgSTPtr->focusSI >= 0)
     activeAdd_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);

  return keySI;

  noFocus_fun069_sec04:;
     widgSTPtr->focusSI = -1;
     return -1;
  nonFocusKey_fun069_sec04:;
      return -2;
} /*focusCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun070: enterCheck_widg_rayWidg
|   - check if key event was an enter key
| Input:
|   - keySI:
|     o raylib key returned from GetKeyPressed() 
|   - charBl:
|     o 1: keySC is char from GetChar()
|     o 0: keySC is key from GetKeyPressed()
|   - parSIPtr:
|     o signed int pionter to get the parent widget (if
|       on a complex widget)
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with focused widget
|       to set press state
| Output:
|   - Modifies:
|     o stateAryUS[widgSTPtr->focusSI] in widgSTPtr to be
|       set to def_press_rayWidg
|     o parSIPtr to have the parent widget id or -1 if
|       this was a simple widget (no children)
|   - Returns:
|     - index of the pressed widget (widgSTPtr->focusSI)
|     - -1 if focused widget was hiden or inactive
|       * no press event can be done
|     - -2 if input key was not enter '\n'
|     - -3 if no parent widget exists for the child widget
\-------------------------------------------------------*/
signed int
enterCheck_widg_rayWidg(
   signed int keySI,   /*key from GetKeyPressed()*/
   signed char charBl, /*1: key was from GetChar()*/
   signed int *parSIPtr, /*gets parent widget id*/
   struct widg_rayWidg *widgSTPtr /*has focus to change*/
){
   *parSIPtr = -1;

   if(charBl)
   { /*If: character input*/
      if(keySI == '\n')
         charBl = 1;
      else
         goto noPress_fun070;
   } /*If: character input*/

   else
   { /*Else: key event input*/
      if(keySI == KEY_ENTER)
         charBl = 1;
      else
         goto noPress_fun070;
   } /*Else: key event input*/


   /*check if widget is pressable*/
   keySI = widgSTPtr->focusSI;
   if(keySI < 0)
      goto noFocus_fun070;
   else if(keySI >= widgSTPtr->lenSI)
      goto noFocus_fun070;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_hiden_rayWidg
   ) goto noFocus_fun070;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_inactive_rayWidg
   ) goto noFocus_fun070;


   pressAdd_widg_rayWidg(keySI, widgSTPtr);

   *parSIPtr =
      childGet_widg_rayWidg(keySI, widgSTPtr);

   if(*parSIPtr == -3)
      goto widgetErr_fun070;
   else if(*parSIPtr < 0)
      *parSIPtr = keySI; /*not a child widget*/

   return keySI;

   noFocus_fun070:;
      return -1;
   noPress_fun070:;
      return -2;
   widgetErr_fun070:;
      return -3;
} /*enterCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun071: getMouseWidg_widg_rayWidg
|   - get widget mouse was on
| Input:
|   - xSI:
|     o x coordiante of mouse
|   - ySI:
|     o y coordiante of mouse
|   - parSIPtr:
|     o signed int pointer to hold parent widget id, if
|       the returned widget was a child, else is set to -1
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to widgets to check
| Output:
|   - Modifies:
|     o parSIPtr to the child widgets parent id (index)
|       * or -1 if returned widget was a parent
|   - Returns:
|     o id (index) of the widget the mouse was over
|     o -1 if the mouse was on no widget or if have hog
|       widget and clicked widget was not a hog widget
|       * negative length (past widget size)
| Note:
|   - this only finds the first possible match
\-------------------------------------------------------*/
signed int
getMouseWidg_widg_rayWidg(
   signed int xSI, /*x coordinate of mouse*/
   signed int ySI, /*y coordinate of mouse*/
   signed int *parSIPtr, /*will get parent if on child*/
   struct widg_rayWidg *widgSTPtr /*widgets to search*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun071 TOC:
   '   - get widget mouse was on
   '   o fun071 sec01:
   '     - variable declarations
   '   o fun071 sec02:
   '     - find widget on
   '   o fun071 sec03:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun071 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int retSI = -1;
   signed int tmpSI = 0;
   signed int hogSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun071 Sec02:
   ^   - find widget on
   ^   o fun071 sec02 sub01:
   ^     - check if widget was clicked on
   ^   o fun071 sec02 sub02:
   ^     - handel clicks on visible hog cases
   ^   o fun071 sec02 sub03:
   ^     - handel first time events
   ^   o fun071 sec02 sub04:
   ^     - check if picked a widget and find child status
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun071 Sec02 Sub01:
   *   - check if widget was clicked on
   \*****************************************************/

   for(
      tmpSI = 0;
      tmpSI < widgSTPtr->lenSI;
      ++tmpSI
   ){ /*Loop: find first widget that could match*/
      if(xSI < widgSTPtr->xArySI[tmpSI])
         continue;

      else if(ySI < widgSTPtr->yArySI[tmpSI])
         continue;

      else if(
           widgSTPtr->stateAryUS[tmpSI]
         & def_hiden_rayWidg
      ) continue;

      else if(
           widgSTPtr->stateAryUS[tmpSI]
         & def_inactive_rayWidg
      ) continue;

      else if(
           xSI
         > widgSTPtr->xArySI[tmpSI]
           + widgSTPtr->widthArySI[tmpSI]
      ) continue;

      else if(
           ySI
         > widgSTPtr->yArySI[tmpSI]
           + widgSTPtr->heightArySI[tmpSI]
      ) continue;

      /**************************************************\
      * Fun071 Sec02 Sub02:
      *   - handel clicks on visible hog cases
      \**************************************************/

      else if( hogGet_widg_rayWidg(tmpSI, widgSTPtr) )
      { /*Else If: have a hog state widget*/
         hogSI = tmpSI;

         if(hogSI > widgSTPtr->focusSI)
         { /*If: user clicked on a next widget*/
            while(hogSI > widgSTPtr->focusSI)
            { /*Loop: check if have child widget path*/
                if(childGet_widg_rayWidg(hogSI,widgSTPtr))
                   --hogSI;
                else
                   break;
            } /*Loop: check if have child widget path*/

            if(tmpSI == widgSTPtr->focusSI)
               retSI = tmpSI;
            else if(hogSI == widgSTPtr->focusSI)
               retSI = tmpSI;
            else
               retSI = -2; /*not in current hog event*/
         } /*If: user clicked on a next widget*/

         else
         { /*Else: moving backwards*/
            if(childGet_widg_rayWidg(hogSI,widgSTPtr))
               ++hogSI;

            while(hogSI <= widgSTPtr->focusSI)
            { /*Loop: check if have child widget path*/
                if(childGet_widg_rayWidg(hogSI,widgSTPtr))
                   ++hogSI;
                else
                   break;
            } /*Loop: check if have child widget path*/

            if(tmpSI == widgSTPtr->focusSI)
               retSI = tmpSI;
            else if(hogSI > widgSTPtr->focusSI)
               retSI = tmpSI;
            else
               retSI = -2; /*not in current hog event*/
         } /*Else: moving backwards*/
      } /*Else If: have a hog state widget*/

      /**************************************************\
      * Fun071 Sec02 Sub03:
      *   - handel first time events
      \**************************************************/

      else if(retSI < 0)
         retSI = tmpSI;
   }  /*Loop: find first widget that could match*/

   /*****************************************************\
   * Fun071 Sec02 Sub04:
   *   - check if picked a widget and find child status
   \*****************************************************/

   if(retSI < -1)
      goto hogNoMove_fun071_sec03_sub03;
   else if(retSI < 0)
      goto noWidg_fun071_sec03_sub03;

   *parSIPtr =
      childGet_widg_rayWidg(retSI, widgSTPtr);

   if(*parSIPtr < -1)
      goto noWidg_fun071_sec03_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun071 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto setRet_fun071_sec03_sub03;

   setRet_fun071_sec03_sub03:;
      pressClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear a press event (might be one)*/
      activeClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear the old active event*/

      focusClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear old focus; also clears focus item*/

      widgSTPtr->focusSI = retSI;
      focusAdd_widg_rayWidg(retSI, widgSTPtr);
         /*set clicked widget to focused widget*/
      activeAdd_widg_rayWidg(retSI, widgSTPtr);
         /*make into an active widget*/

      return retSI;

   hogNoMove_fun071_sec03_sub03:;
      retSI = widgSTPtr->focusSI;
      focusAdd_widg_rayWidg(retSI, widgSTPtr);
         /*set clicked widget to focused widget*/
      activeAdd_widg_rayWidg(retSI, widgSTPtr);
         /*make into an active widget*/
      *parSIPtr = -1;
      return -1;

   noWidg_fun071_sec03_sub03:;
      *parSIPtr = -1;
      return -1;
} /*getMouseWidg_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun072: blinkGet_rayWidg
|   - returns current state of cursor blink for widgets
| Input:
|   - blinkSI:
|     o current interval blink is on
|   - intervalSI:
|     o how many cycles in one blink
|       * ex: 10 frame redraws
|   - timeSI:
|     o time interval between one blink
|       * ex: 5 frame redraws (out of 10 frames)
| Output:
|   - Returns:
|     o 1 if blinking cursor
|     o 0 if printing cursor
\-------------------------------------------------------*/
signed char
blinkGet_rayWidg(
   signed int blinkSI,    /*current interval blink is on*/
   signed int intervalSI, /*redraws for a blink cycle*/
   signed int timeSI      /*number cycles between blinks*/
){
   timeSI = intervalSI - timeSI; /*get number of cycles*/

   if(blinkSI % intervalSI >= timeSI)
      return 1;
   else
      return 0;
} /*blinkGet_rayWidg*/

/*-------------------------------------------------------\
| Fun073: getLines_rayWidg
|   - get number of lines can fit in a set height
| Input:
|   - heightSI:
|     o height (in pixels) to get line count for
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with text height
| Output:
|   - Returns:
|     o number of lines you can print
\-------------------------------------------------------*/
signed int
getLines_rayWidg(
   signed int heightSI, /*height in pixels*/
   struct widg_rayWidg *widgSTPtr
){
   return
      (signed int)
      (
          heightSI
        - widgSTPtr->fontHeightF
        / def_widgHeightGap_rayWidg
     ) / widgSTPtr->fontHeightF;
} /*getLines_rayWidg*/

/*-------------------------------------------------------\
| Fun074: lineToHeight_rayWidg
|   - convert line count to height in pixels
| Input:
|   - linesSI:
|     o number of lines
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with text height
| Output:
|   - Returns:
|     o number height of lines
\-------------------------------------------------------*/
signed int
linesToHeight_rayWidg(
   signed int linesSI, /*number lines*/
   struct widg_rayWidg *widgSTPtr
){
   return (linesSI * widgSTPtr->fontHeightF)
     + widgSTPtr->fontHeightF / def_widgHeightGap_rayWidg;
} /*linesToHeight_rayWidg*/

/*-------------------------------------------------------\
| Fun075: intStrCheck_rayWidg
|   - corrects mistakes in a c-string that is supposed to
|     be an integer
| Input:
|   - textStr:
|     o c-string with mnumber to correct
|   - cursorSI:
|     o were the cursor is at in the c-string
|       * first digit removed, then goes from end to start
|     o -1 to set to no cursor, in this case it works
|       from end to start
|   - maxSI:
|     o maximum value for c-string
|     o 0 to set to no maximum
| Output:
|   - Modifies:
|     o removes non-numeric values from textStr
|     o removes values that cause from textStr to be to
|       large
|   - Returns:
|     o length of string
\-------------------------------------------------------*/
signed int
intStrCheck_rayWidg(
   signed char *textStr,/*text with number*/
   signed int cursorSI, /*position of cursor*/
   signed int maxSI     /*maximum value allowed*/
){
   signed char rmCursorBl = 0;
   signed char *cpStr = textStr;
   signed char *dupStr = textStr;
   signed int tmpSI = 0;

   while(*cpStr)
   { /*Loop: remove non-numeric characters*/
      if(*cpStr == '-' && cpStr == textStr)
         *dupStr++ = *cpStr++;
      else if(*cpStr > 47 && *cpStr < 58)
         *dupStr++ = *cpStr++;
      else
      { /*Else: non-numeric value*/
         if(dupStr - textStr == cursorSI)
            rmCursorBl = 1;
         else if(rmCursorBl)
            ;
         else
            --cursorSI;
         ++cpStr;
      } /*Else: non-numeric value*/
   } /*Loop: remove non-numeric characters*/

   *dupStr = 0;

   if(! maxSI)
      goto ret_fun076;

   strToSI_base10str(textStr, &tmpSI);

   if(cursorSI >= 0 && ! rmCursorBl)
   { /*If: cursor position input*/
      if(tmpSI <= maxSI)
         ;
      else if(dupStr - textStr > cursorSI)
      { /*If: need to remove digit at cursor position*/
         *cpStr = textStr[cursorSI + 1];
         *dupStr = textStr[cursorSI];

         while(*cpStr)
            *dupStr++ = *cpStr++;
         *dupStr = 0;

         strToSI_base10str(textStr, &tmpSI);
      } /*If: need to remove digit at cursor position*/
   } /*If: cursor position input*/

   while(tmpSI > maxSI)
   { /*Loop: remove digits till number is less then*/
      tmpSI = tmpSI / 10;
      --dupStr;
      *dupStr = 0;
   } /*Loop: remove digits till number is less then*/

   ret_fun076:;
      return dupStr - textStr;
} /*intStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun076: floatStrCheck_rayWidg
|   - corrects mistakes in a c-string that is supposed to
|     be an float
| Input:
|   - textStr:
|     o c-string with mnumber to correct
|   - cursorSI:
|     o were the cursor is at in the c-string
|       * first digit removed, then goes from end to start
|     o -1 to set to no cursor, in this case it works
|       from end to start
|   - maxSI:
|     o maximum value for c-string
|       * this divides maxSI by 100 to get two decimals
|     o 0 to set to no maximum
| Output:
|   - Modifies:
|     o removes non-numeric values from textStr
|     o removes values that cause from textStr to be to
|       large
|   - Returns:
|     o length of string
\-------------------------------------------------------*/
signed int
floatStrCheck_rayWidg(
   signed char *textStr,/*text with number*/
   signed int cursorSI, /*position of cursor*/
   signed int maxSI     /*maximum value allowed*/
){
   signed char *cpStr = textStr;
   signed char *dupStr = textStr;
   signed char rmCursorBl = 0;
   signed char decBl = 0;
   float tmpF = 0;
   float maxF = ((float) maxSI) / 100;

   while(*cpStr)
   { /*Loop: remove non-numeric characters*/
      if(*cpStr == '-' && cpStr == textStr)
         *dupStr++ = *cpStr++;

      else if(*cpStr > 47 && *cpStr < 58)
         *dupStr++ = *cpStr++;

      else if(*cpStr == '.' && ! decBl)
      { /*Else If: have a decimal number*/
         *dupStr++ = *cpStr++;
         decBl = 1;
      } /*Else If: have a decimal number*/

      else
      { /*Else: non-numeric value*/
         if(dupStr - textStr == cursorSI)
            rmCursorBl = 1;
         else if(rmCursorBl)
            ;
         else
            --cursorSI;
         ++cpStr;
      } /*Else: non-numeric value*/
   } /*Loop: remove non-numeric characters*/

   *dupStr = 0;

   if(! maxF)
      goto ret_fun076;

   strToF_base10str(textStr, &tmpF);

   if(cursorSI >= 0 && ! rmCursorBl)
   { /*If: cursor position input*/
      if(tmpF <= maxF)
         ;
      else if(dupStr - textStr > cursorSI)
      { /*If: need to remove digit at cursor position*/
         *cpStr = textStr[cursorSI + 1];
         *dupStr = textStr[cursorSI];

         while(*cpStr)
            *dupStr++ = *cpStr++;
         *dupStr = 0;

         strToF_base10str(textStr, &tmpF);
      } /*If: need to remove digit at cursor position*/
   } /*If: cursor position input*/

   while(tmpF > maxF)
   { /*Loop: remove digits till number is less then*/
      --dupStr;
      *dupStr = 0;
      strToF_base10str(textStr, &tmpF);
   } /*Loop: remove digits till number is less then*/

   ret_fun076:;
      return dupStr - textStr;
} /*floatStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun077: fileStrCheck_rayWidg
|   - corrects non-file characters in c-string
|   - spaces/tabes converted to '_', other than that,
|     only A-Z, a-z, 0-9, '-', '_', and '.' are allowed
| Input:
|   - textStr:
|     o c-string with mnumber to correct
|   - cursorSI:
|     o were the cursor is at in the c-string
|       * first digit removed, then goes from end to start
|     o -1 to set to no cursor, in this case it works
|       from end to start
|   - maxSI:
|     o maximum length of string
|     o 0 to set to no maximum
| Output:
|   - Modifies:
|     o replaces spaces an tabs in textStr with '_'
|     o removes non-file name values
|     o trims extra characters
|   - Returns:
|     o length of string
\-------------------------------------------------------*/
signed int
fileStrCheck_rayWidg(
   signed char *textStr,/*text with number*/
   signed int cursorSI, /*position of cursor*/
   signed int maxSI     /*maximum string length*/
){
   signed char rmCursorBl = 0;
   signed char *cpStr = textStr;
   signed char *dupStr = textStr;

   while(*cpStr)
   { /*Loop: check characters in string*/
      if(*cpStr == '.')
         *dupStr++ = *cpStr++; /*dot for extension*/
      if(*cpStr > 47 && *cpStr < 58)
         *dupStr++ = *cpStr++; /*number*/
      else if(*cpStr == '-')
         *dupStr++ = *cpStr++; /*dash*/
      else if(*cpStr == '_')
         *dupStr++ = *cpStr++; /*underscore*/
      else if(*cpStr > 64 && *cpStr < 91)
         *dupStr++ = *cpStr++; /*capital letters*/
      else if(*cpStr > 96 && *cpStr < 122)
         *dupStr++ = *cpStr++; /*lowercase letters*/
      else if(*cpStr == ' ' || *cpStr == '\t')
      { /*Else If: space or tab*/
         *dupStr++ = '_';
         ++cpStr;
      } /*Else If: space or tab*/
      
      else
      { /*Else: invalid entry*/
         if(dupStr - textStr == cursorSI)
            rmCursorBl = 1;
         else if(rmCursorBl)
            ;
         else
            --cursorSI;
         ++cpStr;
      } /*Else: invalid entry*/
   } /*Loop: check characters in string*/

   *dupStr = 0;

   if(cursorSI >= 0 && ! rmCursorBl)
   { /*If: can remove character at cursor*/
      if(dupStr - textStr > maxSI)
      { /*If: string is to long*/
         *cpStr = textStr[cursorSI + 1];
         *dupStr = textStr[cursorSI];

         while(*cpStr)
            *dupStr++ = *cpStr++;
         *dupStr = 0;
      } /*If: string is to long*/
   } /*If: can remove character at cursor*/

   while(dupStr - textStr > maxSI)
      *dupStr-- = 0;

   return dupStr - textStr;
} /*fileStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun078: addCharToEntry_rayWidg
|   - adds a character to an entry box text
| Input:
|   - keySI:
|     o key from raylib's GetKeyPressed() function
|       * GetChar() should also work for ascii keys
|   - textStr:
|     o c-string to add key to
|   - lenSIPtr:
|     o signed int pionter with length of current c-string
|   - widthSI:
|     o width of entry box; used to make sure cursor and
|       scroll positions are in sync
|   - posArySI:
|     o signed int array with cursor position at index 1
|       and scroll position at index 0
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font and font
|       variables
| Output:
|   - Modifies:
|     o textStr to have keySI applied
|     o lenSIPtr to have the new textStr length
|     o cusrorSIPtr to have the new cursor position
|   - Returns:
|     o -1 for delete event (if could be done)
|     o 1 for insert/append event
|     o 0 for cursor move event or no event
|       * or deletion event with nothing to delete
| WARNING:
|   - this function assumes you always have at least one
|     character left in textStr
\-------------------------------------------------------*/
signed int
addCharToEntry_rayWidg(
   signed int keySI,       /*key to event to add*/
   signed char textStr[],  /*text to add event to*/
   signed int *lenSIPtr,   /*length of c-string*/
   signed int widthSI,     /*maximum width of entry*/
   signed int *posArySI,   /*cursor + scroll position*/
   struct widg_rayWidg *widgSTPtr /*has font*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun078 TOC:
   '   - adds a character to an entry box text
   '   o fun078 sec01:
   '     - variable declaration and movement keys
   '   o fun078 sec02:
   '     - delete key
   '   o fun078 sec03:
   '     - backspace key
   '   o fun078 sec04:
   '     - keys that need a conversion
   '   o fun078 sec05:
   '     - normal keys
   '   o fun078 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec01:
   ^   - variable declaration and movement keys
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int posSI = posArySI[1];
   signed int lenSI = 0;   /*find final output length*/
   signed char shiftBl = 0;
   signed char tmpStr[256];

   /*for measuring final string width*/
   signed char swapArySC[3] = {0, 0, 0};

   shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
   shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

   if(keySI == KEY_LEFT)
   { /*If: move left*/
      if(posArySI[1] > 0)
         --posArySI[1];
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      goto cursorMove_fun078_sec06;
   } /*If: move left*/

   else if(keySI == KEY_RIGHT)
   { /*Else If: move left*/
      if(posArySI[1] < *lenSIPtr)
         ++posArySI[1];
      goto cursorMove_fun078_sec06;
   } /*Else If: move left*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec02:
   ^   - delete key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_DELETE)
   { /*Else If: deleting the next character*/
      if(*lenSIPtr <= 0)
         goto cursorMove_fun078_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] >= *lenSIPtr)
         goto cursorMove_fun078_sec06;
         /*at end of string; nothing to delete*/

      else if(posArySI[1] == *lenSIPtr - 1)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1]] = 0;
         --*lenSIPtr;
         goto delete_fun078_sec06;
      } /*Else If: only one character to delete*/

      else
      { /*Else: need to shift items around*/
         posSI = posArySI[1];
            /*cursor position is on deleted character*/

         while(posSI < *lenSIPtr)
         { /*Loop: delete cursor position*/
            textStr[posSI] = textStr[posSI + 1];
            ++posSI;
         } /*Loop: delete cursor position*/

         textStr[posSI] = 0;
         --*lenSIPtr;
         goto delete_fun078_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: deleting the next character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec03:
   ^   - backspace key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_BACKSPACE)
   { /*Else If: backspace to delete last character*/
      if(posArySI[1] <= 0)
         goto cursorMove_fun078_sec06;
         /*at start of string; nothing to delete*/

      else if(*lenSIPtr <= 0)
         goto cursorMove_fun078_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] == *lenSIPtr)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1] - 1] = 0;
         --posArySI[1];
         --*lenSIPtr;
         goto delete_fun078_sec06;
      } /*Else If: only one character to delete*/

      else
      { /*Else: need to shift items around*/
         posSI = posArySI[1] - 1;
            /*-1 to get on delete character*/

         while(posSI < *lenSIPtr)
         { /*Loop: delete cursor position*/
            textStr[posSI] = textStr[posSI + 1];
            ++posSI;
         } /*Loop: delete cursor position*/

         textStr[posSI] = 0;
         --*lenSIPtr;
         --posArySI[1];
         goto delete_fun078_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: backspace to delete last character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec04:
   ^   - keys that need a conversion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_END)
   { /*Else If: move cursor to end of text*/
      posArySI[1] = *lenSIPtr;
      goto cursorMove_fun078_sec06;
   } /*Else If: move cursor to end of text*/

   else if(keySI == KEY_HOME)
   { /*Else If: move cursor to start of text*/
      posArySI[0] = 0;
      posArySI[1] = 0;
      goto cursorMove_fun078_sec06;
   } /*Else If: move cursor to start of text*/

   else if(keySI == KEY_KP_0)
   { /*Else If: key pad 0*/
      keySI = '0';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 0*/

   else if(keySI == KEY_KP_1)
   { /*Else If: key pad 1*/
      keySI = '1';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 1*/

   else if(keySI == KEY_KP_2)
   { /*Else If: key pad 2*/
      keySI = '2';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 2*/

   else if(keySI == KEY_KP_3)
   { /*Else If: key pad 3*/
      keySI = '3';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 3*/

   else if(keySI == KEY_KP_4)
   { /*Else If: key pad 4*/
      keySI = '4';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 4*/

   else if(keySI == KEY_KP_5)
   { /*Else If: key pad 5*/
      keySI = '5';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 5*/

   else if(keySI == KEY_KP_6)
   { /*Else If: key pad 6*/
      keySI = '6';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 6*/

   else if(keySI == KEY_KP_7)
   { /*Else If: key pad 7*/
      keySI = '7';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 7*/

   else if(keySI == KEY_KP_8)
   { /*Else If: key pad 8*/
      keySI = '8';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 8*/

   else if(keySI == KEY_KP_9)
   { /*Else If: key pad 9*/
      keySI = '9';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad 9*/

   else if(keySI == KEY_KP_DECIMAL)
   { /*Else If: key pad .*/
      keySI = '.';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad .*/

   else if(keySI == KEY_KP_DIVIDE)
   { /*Else If: key pad /*/
      keySI = '/';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad /*/

   else if(keySI == KEY_KP_MULTIPLY)
   { /*Else If: key pad **/
      keySI = '*';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad **/

   else if(keySI == KEY_KP_SUBTRACT)
   { /*Else If: key pad -*/
      keySI = '-';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad -*/

   else if(keySI == KEY_KP_ADD)
   { /*Else If: key pad +*/
      keySI = '+';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad +*/

   else if(keySI == KEY_KP_EQUAL)
   { /*Else If: key pad =*/
      keySI = '=';
      goto addNormalKey_fun078_sec05;
   } /*Else If: key pad =*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec05:
   ^   - normal keys
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI >= 32 && keySI <= 96)
   { /*Else If: normal key pressed*/
      if(keySI < KEY_A)
         ;
      else if(keySI <= KEY_Z)
      { /*Else If: alphabet key input*/
         if(shiftBl)
            keySI &= ~32; /*make sure upper case*/
         else
            keySI |= 32;  /*make sure lower case*/
      } /*Else If: alphabet key input*/
         
   
      addNormalKey_fun078_sec05:;

      if(posArySI[1] >= *lenSIPtr)
      { /*If: at end of string*/
         textStr[posArySI[1]] = keySI;
         ++posArySI[1];
         ++*lenSIPtr;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun078_sec06;
      } /*If: at end of string*/

      else
      { /*Else: need to insert the new character*/
         for(posSI=*lenSIPtr;posSI > posArySI[1];--posSI)
            textStr[posSI] = textStr[posSI - 1];

         ++posArySI[1];
         ++*lenSIPtr;
         textStr[posSI] = keySI;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun078_sec06;
      } /*Else: need to insert the new character*/
   } /*Else If: normal key pressed*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun078 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cursorMove_fun078_sec06;
      /*no idea what the key event is*/

   delete_fun078_sec06:;
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      posSI = -1;
      goto ret_fun078_sec06;

   addChar_fun078_sec06:;
      posSI = 1;
      goto adjustPos_fun078_sec06;

   cursorMove_fun078_sec06:;
      posSI = 0;
      goto adjustPos_fun078_sec06;

   adjustPos_fun078_sec06:;
      /*need to make sure the scroll and cursor position
      `  do not overextend the entry box
      */

      lenSI = *lenSIPtr;

      if(posArySI[1] < lenSI)
         ;
      else
         posArySI[1] = lenSI;
      
      if(posArySI[0] < lenSI)
         lenSI -= posArySI[0];
      else
         posArySI[0] = lenSI;

      /*build printed string*/
      swapArySC[0] = textStr[posArySI[1]];
      textStr[posArySI[1]] = def_cursor_rayWidg;

      swapArySC[1] = textStr[posArySI[1] + 1];
      textStr[posArySI[1] + 1] = swapArySC[0];

      swapArySC[2] = textStr[posArySI[1] + 2];
      textStr[posArySI[1] + 2] = 0;

      checkTextWidth_rayWidg(
         textStr,
         tmpStr,
         widthSI,
         widgSTPtr
      );

      lenSI = endStr_ulCp(tmpStr);
      if(lenSI + posArySI[0] < posArySI[1])
         posArySI[0] = posArySI[1] - lenSI;

      textStr[posArySI[1]] = swapArySC[0];
      textStr[posArySI[1] + 1] = swapArySC[1];
      textStr[posArySI[1] + 2] = swapArySC[2];

      goto ret_fun078_sec06;

   ret_fun078_sec06:;
      textStr[*lenSIPtr] = 0;
      return (signed char) posSI;
} /*addCharToEntry_rayWidg*/

/*-------------------------------------------------------\
| Fun079: checkExt_rayWidg
|   - checks if file is the correct input extension
| Input:
|   - fileStr:
|     o c-string with file to check
|   - fileExtAryStr:
|     o c-string array with file extensions to look for
|     o `*d` stands for all files and directories
|     o `*` stands for all files (not directories)
|     o `+` stands for all files with an extension in
|       the list (including directories)
|     o `dir` stands for allow selection of directories
|   - stateArySC:
|     o tells if the file extension is selected or is not
|       selected
|       * def_listSelect_rayWidg for selected
|   - extLenSI:
|     o number of file extensions looking for
| Output:
|   - Returns:
|     o 2 if was directory and is supported
|     o 1 if fileStr has a supported extension
|     o 0 if fileStr does not have the extension
|     o -1 if not a file or directory
\-------------------------------------------------------*/
signed char
checkExt_rayWidg(
   signed char *fileStr,    /*file to check extension of*/
   signed char *extAryStr[], /*file extensions*/
   signed char *stateArySC,  /*is file extension used*/
   signed int extLenSI       /*number of extensions*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun079 TOC:
   '   - checks if file is the correct input extension
   '   o fun079 sec01:
   '     - variable declarations & check if have file/dir
   '   o fun079 sec02:
   '     - handle any file case
   '   o fun079 sec03:
   '     - handle directory case
   '   o fun079 sec04:
   '     - handle a single extension case
   '   o fun079 sec05:
   '     - handle all possible file extensions case
   '   o fun079 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec01:
   ^   - variable declarations and check if have file/dir
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char fileBl = 0;
   signed int lenSI = 0;
   signed int extPosSI = 0;
   signed int extIndexSI = 0;

   if( DirectoryExists((char *) fileStr) )
      fileBl = 0;
   else if( FileExists((char *) fileStr) )
      fileBl = 1; /*will count directory as file*/
   else
      goto noFile_fun079_sec06;

   if(! extAryStr || ! extLenSI)
   { /*If: no extensions input; assume all files*/
      if( ! fileBl)
         goto noMatch_fun079_sec06;
      else
         goto match_fun079_sec06;
      /*no extensions, assume everything matches*/
   } /*If: no extensions input; assume all files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec02:
   ^   - handle any file case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      extIndexSI = 0;
      extIndexSI < extLenSI;
      ++extIndexSI
   ){ /*Loop: check if file matches any extensions*/
      if(! stateArySC)
         ; /*assuming all extensions are valid*/

      else if(
         stateArySC[extIndexSI] & def_listSelect_rayWidg
      ) ;

      else
         continue; /*user has not selected this*/

      if(extAryStr[extIndexSI][0] == '+')
         goto matchDir_fun079_sec06;

      if(extAryStr[extIndexSI][0] == '*' && fileBl)
         goto matchDir_fun079_sec06;

      else if(
            extAryStr[extIndexSI][0] == 'd'
         && extAryStr[extIndexSI][1] == 'i'
         && extAryStr[extIndexSI][2] == 'r'
         && ! extAryStr[extIndexSI][3]
      ){ /*Else If: directories are supported*/
         if(! fileBl)
            goto matchDir_fun079_sec06;
         else
            continue; /*is file not a directory*/
      }  /*Else If: directories are supported*/

      else if(! fileBl)
         continue; /*is directory not file*/

      else
      { /*Else: specific file extension*/
         lenSI = endStr_ulCp(fileStr) - 1;
         extPosSI = endStr_ulCp(extAryStr[extIndexSI]) -1;

         while(
             lenSI >= 0
          && extPosSI >= 0
          &&
                fileStr[lenSI]
             == extAryStr[extIndexSI][extPosSI]
         ){ /*Loop: see if extension matches*/
            --lenSI;
            --extPosSI;
         }  /*Loop: see if extension matches*/

         if(extPosSI < 0)
            goto match_fun079_sec06;
            /*extensions matched*/
      } /*Else: specific file extension*/
   }  /*Loop: check if file matches any extensions*/

   /*if I hit this point, then no extensions matched*/
   goto noMatch_fun079_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   match_fun079_sec06:;
      return 1;
   matchDir_fun079_sec06:;
      return 2;
   noMatch_fun079_sec06:;
      return 0;
   noFile_fun079_sec06:;
      return -1;
} /*checkExt_rayWidg*/

/*-------------------------------------------------------\
| Fun080: textDrawByCoord_rayWidg
|   - draws one line of text by coordinate
| Input:
|   - textStr:
|     o c-string with text to draw
|   - xSI:
|     o x coordinate to draw at
|   - ySI:
|     o y coordinate to draw at
|   - maxWidthSI:
|     o maximum width of text (in pixels) (use 0 for any)
|     o if x padding, the x pad is taken of maxWidthSI
|   - colUI:
|     o unsignd int with hex code of color to draw
|   - padSC:
|     o tells if to apply padding to text coordinates
|       * 0 for no padding
|       * 1 for x padding
|         - padding on both ends, included in width
|       * 2 for y padding
|       * 3 for x and y padding
|         - x padding on both ends, included in width
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font to use
| Output:
|   - Draws:
|     o text or shortened text to coordinates
|   - Returns:
|     o width of drawn text
\-------------------------------------------------------*/
signed int
textDrawByCoord_rayWidg(
   signed char *textStr,  /*text to draw*/
   signed int xSI,        /*x coordinate*/
   signed int ySI,        /*y coordinate*/
   signed int maxWidthSI, /*max width of text*/
   unsigned int colUI,    /*color to draw*/
   signed char padSC,     /*tells if adding padding*/
   struct widg_rayWidg *widgSTPtr /*has font*/
){
   signed char drawStr[512 + 8];
   signed int lenSI = 0;
   struct Color colorST;

   /*need to convert hex color code to raylib color*/
   colorST.r = colUI >> 24 & 0xff;   /*red*/
   colorST.g = (colUI >> 16) & 0xff; /*green*/
   colorST.b = (colUI >> 8) & 0xff;  /*blue*/
   colorST.a = colUI & 0xff;         /*alpha*/

   if(padSC & 1)
      maxWidthSI -= (widgSTPtr->fontWidthF * 2);

   if(maxWidthSI > 0)
      checkTextWidth_rayWidg(
         textStr,
         drawStr,
         maxWidthSI,
         widgSTPtr
      );
   else
   { /*Else: copy full text*/
      lenSI = endStr_ulCp(textStr);
      if(lenSI > 509)
      { /*If: buffer overflow*/
         lenSI = 509;
         cpLen_ulCp(drawStr, textStr, lenSI);
         drawStr[lenSI++] = '.';
         drawStr[lenSI++] = '.';
         drawStr[lenSI++] = '.';
         drawStr[lenSI] = 0;
      } /*If: buffer overflow*/

      else
         cpLen_ulCp(drawStr, textStr, lenSI);
   } /*Else: copy full text*/

   maxWidthSI =
     textMeasure_widg_rayWidg(drawStr, &lenSI, widgSTPtr);

   if(padSC & 1)
   { /*If: padding x coordinate*/
      xSI += widgSTPtr->fontWidthF;
      maxWidthSI += widgSTPtr->fontWidthF * 2;
   } /*If: padding x coordinate*/

   if(padSC & 2)
      ySI +=
        widgSTPtr->fontHeightF /def_widgHeightGap_rayWidg;

   DrawTextEx(
       *widgSTPtr->fontSTPtr,
       (char *) drawStr,
       (Vector2) {xSI, ySI},
       widgSTPtr->fontSizeSI,
       widgSTPtr->spacingF,
       colorST
   );

   return maxWidthSI;
} /*textDrawByCoord_rayWidg*/

/*-------------------------------------------------------\
| Fun081: rectDrawByCoord_rayWidg
|   - draws a rectangle for rayWidg by input coordinates
| Input:
|   - xSI:
|     o x coordinate to draw rectangle at
|   - ySI:
|     o y coordinate to draw rectangle at
|   - widthSI:
|     o width of rectangle
|   - heightSI:
|     o height of rectangle
|   - idSI:
|     o id of recganle to draw widget for
|   - typeSC:
|     o 0: for just rectangle
|     o 1: for rectangle with a border
|     o 2: for rectangle with a focus border
|     o 3: for rectangle with a border and focus border
|     o 4: to not draw rectangle
|     o 5: for just border
|     o 6: for just focus border
|     o 7: for border and focus border
|   - colorHexUI:
|     o has hex color code for the rectangles color
|   - borderHexUI:
|     o has hex color code for the borders color
|   - focusHexUI:
|     o has hex color code for the focus borders color
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with scale factor
|       for output (for border)
| Output:
\-------------------------------------------------------*/
void
rectDrawByCoord_rayWidg(
   signed int xSI,           /*x coordinate*/
   signed int ySI,           /*y coordinate*/
   signed int widthSI,       /*width of rectangle*/
   signed int heightSI,      /*height of rectangle*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI,  /*hex color of focus border*/
   struct widg_rayWidg *widgSTPtr /*has scale*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun081 TOC:
   '   - draws a rectangle for with input coordinates
   '   o fun081 sec01:
   '     - variable declarations
   '   o fun081 sec02:
   '     - draw focus border (if requested)
   '   o fun081 sec03:
   '     - draw border around rectangle (if requested)
   '   o fun081 sec04:
   '     - draw the rectangle
   '   o fun081 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun081 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   float offsetF = 0;
   signed int xOffsetSI = 0;
   signed int yOffsetSI = 0;

   struct Rectangle coordsST;
   struct Color colorST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun081 Sec02:
   ^   - draw focus border around rectangle (if requested)
   ^   o fun081 sec02 sub01:
   ^     - find the offset for the focus border
   ^   o fun081 sec02 sub02:
   ^     - draw the focus border
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun081 Sec02 Sub01:
   *   - find the offset for the focus border
   \*****************************************************/

   if(typeSC & 2)
   { /*If: drawing a focus border*/
      coordsST.x = xSI;
      coordsST.y = ySI;
      coordsST.width = widthSI;
      coordsST.height = heightSI;

      offsetF = def_focusBorder_rayWidg;
      if(widgSTPtr->xScaleF > 1)
         offsetF = offsetF * widgSTPtr->xScaleF;

      if(coordsST.x - offsetF < 0)
      { /*If: need to adjust border*/
         xOffsetSI = coordsST.x;
         coordsST.x = 0;
      } /*If: need to adjust border*/

      else
      { /*Else: full offset*/
         xOffsetSI = offsetF;
         coordsST.x -= offsetF;
      } /*Else: full offset*/

      if(coordsST.y - offsetF < 0)
      { /*If: need to adjust border*/
         yOffsetSI = coordsST.y;
         coordsST.y = 0;
      } /*If: need to adjust border*/

      else
      { /*Else: full offset*/
         yOffsetSI = offsetF;
         coordsST.y -= offsetF;
      } /*Else: full offset*/

      coordsST.width += xOffsetSI + offsetF;
      coordsST.height += yOffsetSI + offsetF;

      /**************************************************\
      * Fun081 Sec02 Sub02:
      *   - draw the focus border
      \**************************************************/

      colorST.r = focusHexUI >> 24 & 0xff;   /*red*/
      colorST.g = (focusHexUI >> 16) & 0xff; /*green*/
      colorST.b = (focusHexUI >> 8) & 0xff;  /*blue*/
      colorST.a = focusHexUI & 0xff;         /*alpha*/

      #ifdef MAC
         DrawRectangleRounded(
            coordsST,
            def_macRoundness_rayWidg,
            def_macSegments_rayWidg,
            colorST
         );
      #else
         DrawRectangle(
            coordsST.x,
            coordsST.y,
            coordsST.width,
            coordsST.height,
            colorST
         );
      #endif
   } /*If: drawing a focus border*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun081 Sec03:
   ^   - draw border around rectangle (if requested)
   ^   o fun081 sec03 sub01:
   ^     - find the offset for the border
   ^   o fun081 sec03 sub02:
   ^     - draw the border
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun081 Sec03 Sub01:
   *   - find the offset for the border
   \*****************************************************/

   if(typeSC & 1)
   { /*If: rectangle with border*/
      coordsST.x = xSI;
      coordsST.y = ySI;
      coordsST.width = widthSI;
      coordsST.height = heightSI;

      offsetF = def_border_rayWidg;
      if(widgSTPtr->xScaleF > 1)
         offsetF = offsetF * widgSTPtr->xScaleF;

      if(coordsST.x - offsetF < 0)
      { /*If: need to adjust border*/
         xOffsetSI = coordsST.x;
         coordsST.x = 0;
      } /*If: need to adjust border*/

      else
      { /*Else: full offset*/
         xOffsetSI = offsetF;
         coordsST.x -= offsetF;
      } /*Else: full offset*/

      if(coordsST.y - offsetF < 0)
      { /*If: need to adjust border*/
         yOffsetSI = coordsST.y;
         coordsST.y = 0;
      } /*If: need to adjust border*/

      else
      { /*Else: full offset*/
         yOffsetSI = offsetF;
         coordsST.y -= offsetF;
      } /*Else: full offset*/

      coordsST.width += xOffsetSI + offsetF;
      coordsST.height += yOffsetSI + offsetF;

      /**************************************************\
      * Fun081 Sec03 Sub02:
      *   - draw the border
      \**************************************************/

      colorST.r = borderHexUI >> 24 & 0xff;   /*red*/
      colorST.g = (borderHexUI >> 16) & 0xff; /*green*/
      colorST.b = (borderHexUI >> 8) & 0xff;  /*blue*/
      colorST.a = borderHexUI & 0xff;         /*alpha*/

      #ifdef MAC
         DrawRectangleRounded(
            coordsST,
            def_macRoundness_rayWidg,
            def_macSegments_rayWidg,
            colorST
         );
      #else
         DrawRectangle(
            coordsST.x,
            coordsST.y,
            coordsST.width,
            coordsST.height,
            colorST
         );
      #endif
   } /*If: rectangle with border*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun081 Sec04:
   ^   - draw the rectangle
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*need to convert hex color code to raylib color*/
   colorST.r = colorHexUI >> 24 & 0xff;   /*red*/
   colorST.g = (colorHexUI >> 16) & 0xff; /*green*/
   colorST.b = (colorHexUI >> 8) & 0xff;  /*blue*/
   colorST.a = colorHexUI & 0xff;         /*alpha*/

   coordsST.x = xSI;
   coordsST.y = ySI;
   coordsST.width = widthSI;
   coordsST.height = heightSI;

   if(! (typeSC & 4))
   { /*If: can draw a rectangle*/
      #ifdef MAC
         DrawRectangleRounded(
            coordsST, /*coordinates for rectangle*/
            def_macRoundness_rayWidg,
               /*roundness of rectangle*/
            def_macSegments_rayWidg,
                /*no idea what segments are for*/
            colorST
         );
      #else
         DrawRectangle(
            coordsST.x,
            coordsST.y,
            coordsST.width,
            coordsST.height,
            colorST
         );
      #endif
   } /*If: can draw a rectangle*/
} /*rectDrawByCoord_rayWidg*/

/*-------------------------------------------------------\
| Fun082: rectTextDrawByCoord_rayWidg
|   - draws a rectangle with text by coordinate
| Input:
|   - textStr:
|     o c-string to draw
|   - xSI:
|     o x coordinate to draw rectangle at
|   - ySI:
|     o y coordinate to draw rectangle at
|   - maxWidthSI:
|     o max width to draw
|   - minWidthSI:
|     o min width to draw
|   - idSI:
|     o id of recganle to draw widget for
|   - typeSC:
|     o 0: for just rectangle
|     o 1: for rectangle with a border
|     o 2: for rectangle with a focus border
|     o 3: for rectangle with a border and focus border
|     o 4: to not draw rectangle
|     o 5: for just border
|     o 6: for just focus border
|     o 7: for border and focus border
|   - textHexUI:
|     o has hex color code for the text
|   - colorHexUI:
|     o has hex color code for the rectangles color
|   - borderHexUI:
|     o has hex color code for the borders color
|   - focusHexUI:
|     o has hex color code for the focus borders color
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font to use
| Output:
|   - Returns:
|     o width of the drawn rectangle
\-------------------------------------------------------*/
signed int
rectTextDrawByCoord_rayWidg(
   signed char *textStr,     /*c-string to draw*/
   signed int xSI,           /*x coordinate*/
   signed int ySI,           /*y coordinate*/
   signed int maxWidthSI,    /*max width of rectangle*/
   signed int minWidthSI,    /*min width of rectangle*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int textHexUI,   /*hex color of text*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI,  /*hex color of focus border*/
   struct widg_rayWidg *widgSTPtr /*has font*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun082 TOC:
   '   - draws a rectangle for with input coordinates
   '   o fun082 sec01:
   '     - variable declarations
   '   o fun082 sec02:
   '     - draw the rectangle
   '   o fun082 sec03:
   '     - draw border around rectangle (if requested)
   '   o fun082 sec04:
   '     - draw focus border (if requested)
   '   o fun082 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char outTextStr[1024] = {0};
   signed int heightSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec02:
   ^   - draw the rectangle
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   heightSI =
        def_getPad_rayWidg(widgSTPtr->fontHeightF) * 2
      + widgSTPtr->fontHeightF;

   maxWidthSI -= widgSTPtr->fontWidthF * 2;
      /*account for padding around text*/

   maxWidthSI =
      checkTextWidth_rayWidg(
         textStr,
         outTextStr,
         maxWidthSI,
         widgSTPtr
      ); /*find the text width*/

   maxWidthSI += widgSTPtr->fontWidthF * 2;
      /*actual width*/

   if(maxWidthSI < minWidthSI)
      maxWidthSI = minWidthSI;

   rectDrawByCoord_rayWidg(
      xSI,           /*x coordinate*/
      ySI,           /*y coordinate*/
      maxWidthSI,    /*width of rectangle*/
      heightSI,      /*height of rectangle*/
      typeSC,        /*type of rectangle*/
      colorHexUI,    /*hex color of rectangle*/
      borderHexUI,   /*hex color of border*/
      focusHexUI,    /*hex color of focus border*/
      widgSTPtr
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec03:
   ^   - draw the text
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   heightSI = def_getPad_rayWidg(widgSTPtr->fontHeightF);

   textDrawByCoord_rayWidg(
      outTextStr,                  /*text to draw*/
      xSI + widgSTPtr->fontWidthF, /*x coordinate*/
      ySI + heightSI,              /*y coordinate*/
      0,                           /*width already setup*/
      textHexUI,                   /*text color*/
      0,                           /*no padding*/
      widgSTPtr                    /*has font*/
   );

   return maxWidthSI;
} /*rectTextDrawByCoord_rayWidg*/

/*-------------------------------------------------------\
| Fun083: rectDraw_rayWidg
|   - draws a rectangle for rayWidg
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct poitner with coordinates and
|       size of rectangle
|   - idSI:
|     o id of recganle to draw widget for
|   - typeSC:
|     o 0: for just rectangle
|     o 1: for rectangle with a border
|     o 2: for rectangle with a focus border
|     o 3: for rectangle with a border and focus border
|     o 4: to not draw rectangle
|     o 5: for just border
|     o 6: for just focus border
|     o 7: for border and focus border
|   - colorHexUI:
|     o has hex color code for the rectangles color
|   - borderHexUI:
|     o has hex color code for the borders color
|   - focusHexUI:
|     o has hex color code for the focus borders color
| Output:
|   - Returns:
|     o width of widget (without borders) for success
|     o 0 if widget is hidden
|     o def_noWidget_rayWidg if widget does not exits
\-------------------------------------------------------*/
signed int
rectDraw_rayWidg(
   struct widg_rayWidg *widgSTPtr, /*has coordinates*/
   signed int idSI,          /*id of widget to draw*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI   /*hex color of focus border*/
){
   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun083;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun083;

   rectDrawByCoord_rayWidg(
      widgSTPtr->xArySI[idSI],  /*x coordinate*/
      widgSTPtr->yArySI[idSI],  /*y coordinate*/
      widgSTPtr->widthArySI[idSI],/*width fof rectangle*/
      widgSTPtr->heightArySI[idSI],/*height of rectangle*/
      typeSC,                   /*type of rectangle*/
      colorHexUI,               /*hex color of rectangle*/
      borderHexUI,              /*hex color of border*/
      focusHexUI,               /*hex color of focus border*/
      widgSTPtr
   );

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun083:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun083:;
      return 0;
} /*rectDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun084: butDraw_rayWidg
|   - draw a button
| Input:
|   - maxWidthSI:
|     o maximum width of button
|   - minWidthSI:
|     o miniumum width of button (fixed if min == max)
|   - idSI:
|     o id (index) of button to draw/get tile coordinates
|   - textStr:
|     o c-string with text to print on button
|   - noDrawBl:
|     o 1: do not draw, but find button width and height
|       * x and y coordiantes are found with
|        tile_widg_rayWidg (needs width/height)
|     o 0: draw button
|       * needs x and y coordinates
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add widget to
|     o x coordinate for button
|     o y coordinate for button
|     o state button is in
|       * def_press_rayWidg for press
|       * def_inactive_rayWidg for inactive
| Output:
|   - Draws:
|     o button (with text) to GUI if noDrawBl is 0
|   - Modifies:
|     o widthArySI in widgSTPtr to have the button width
|     o heightArySI in widgSTPtr to have the button height
|   - Returns:
|     o width of the button
|     o 0 if button wigdet is hidden
|     o def_noWidget_rayWidg if could not find widget
| Note
|   - checks: hiden, inactive, focus, and press states
\-------------------------------------------------------*/
signed int
butDraw_rayWidg(
   signed int maxWidthSI, /*maximum width of button*/
   signed int minWidthSI, /*maximum width of button*/
   signed int idSI,       /*id of widget to add*/
   signed char *textStr,  /*text to print on button*/
   signed char noDrawBl,  /*1:find size, but do not draw*/
   struct widg_rayWidg *widgSTPtr  /*has button widget*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun084 TOC:
   '   - draw a button
   '   o fun084 sec01:
   '     - variable declarations
   '   o fun084 sec02:
   '     - get color of button (state in)
   '   o fun084 sec03:
   '     - get width of button
   '   o fun084 sec04:
   '     - draw button
   '   o fun084 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = 0;
   struct Color textCol;

   #define def_maxButText_fun084 32
      /*max number of characters in button*/
   signed char writeStr[def_maxButText_fun084 + 8];
   signed char typeSC = 0; /*type of rectangle to draw*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec02:
   ^   - get color of button (state in)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun084_sec05;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun084_sec05;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: button is in a disabled state*/
      typeSC = 0; /*want both borders*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      focusColorUI = widgSTPtr->textAltColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a disabled state*/

   else if(
         widgSTPtr->stateAryUS[idSI] & def_press_rayWidg
      || widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg
   ){ /*Else If: button is in a press state*/
      typeSC = 0; /*want border*/
      colorUI = widgSTPtr->activeColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: button is in a press state*/

   else
   { /*Else If: button is in a normal state*/
      typeSC = 1; /*want border*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a normal state*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec03:
   ^   - get width of button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(minWidthSI < 0)
      minWidthSI = 0;

   if(! textStr)
   { /*If: no text input*/
      widgSTPtr->widthArySI[idSI] = minWidthSI;
      writeStr[0] = 0;
   } /*If: no text input*/

   else
   { /*Else: have text to use*/
      widgSTPtr->widthArySI[idSI] =
         checkTextWidth_rayWidg(
            textStr,
            writeStr,
            maxWidthSI - widgSTPtr->fontWidthF * 2,
            widgSTPtr
         );
      if(widgSTPtr->widthArySI[idSI] < minWidthSI)
         widgSTPtr->widthArySI[idSI] = minWidthSI;
   } /*Else: have text to use*/

   if(! widgSTPtr->widthArySI[idSI])
      widgSTPtr->widthArySI[idSI] = widgSTPtr->fontWidthF;
         /*button needs to be at least 1 char*/

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg);

   widgSTPtr->widthArySI[idSI] +=
      widgSTPtr->fontWidthF * 2;
      /*want a one character pad around the button*/

   if(noDrawBl)
   { /*If: getting coordinates only*/
      widgSTPtr->heightArySI[idSI] +=
         + widgSTPtr->fontHeightF
         / (def_widgHeightGap_rayWidg);

      goto done_fun084_sec05;
   } /*If: getting coordinates only*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec04:
   ^   - draw button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI,
      typeSC,
      colorUI,
      borderColorUI,
      focusColorUI
   );

   widgSTPtr->heightArySI[idSI] +=
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg);


   if(textStr)
      DrawTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) writeStr,
         (Vector2) {
             widgSTPtr->xArySI[idSI] +
               widgSTPtr->fontWidthF,
               /*center text in x*/
            widgSTPtr->yArySI[idSI]
              + widgSTPtr->fontHeightF
              / (def_widgHeightGap_rayWidg * 2)
               /*center text on y*/
         },
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         textCol
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun084_sec05:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun084_sec05:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun084_sec05:;
      return 0;
} /*butDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun085: entryDraw_rayWidg
|   - draw an entry box and its text
| Input:
|   - widthSI:
|     o width of entry box without padding
|     o the returned width will be a little larger
|   - idSI:
|     o widget id to draw
|   - posArySI:
|     o 2 element int array with position of the cursor
|       and the scroll positon
|       * if both elements are < 0 (ex -1), then both
|         elements are set to 0
|       * first element is scroll position
|         - first character in textStr to dispaly
|         - use posArySI[0] = def_maxStrLen_rayWidg; to
|           always get end of string
|         - use -1 to set to cursor position
|       * 2nd element is cursor position
|         - how many characters away from scroll
|         - use -1 to set to scroll position
|         - use posArySI[0] = def_maxStrLen_rayWidg; to
|           always get end of string
|   - blinkBl:
|     o 1: to print a blink (space) instead of a cursor 
|     o 0: to print cursor
|   - textStr:
|     o text currenty in entry box
|     o maximum limit for printed characters is 256
|   - noDrawBl:
|     o 1: update height, but do not draw entry box
|     o 0: update height and draw entry box
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with width,
|       coordinates (tile/x,y), and state of entry box
| Output
|   - Draws:
|     o text entry box (with text) to GUI if noDrawBl is 0
|   - Modifies:
|     o heightArySI in widgSTPtr to have the button height
|     o both elements of posArySI to be on position in box
|   - Returns
|     o width of the entry box
|     o 0 if entry box wigdet is hidden
|     o def_noWidget_rayWidg if could not find widget
| Note:
|   - checks, hiden, inactive, focus, and active states,
|     everything else is considered normal
|   - maximum number of characters is 256, this is not
|     a text box, which could span multipe lines, so 256
|     is way longer then needed
\-------------------------------------------------------*/
signed int
entryDraw_rayWidg(
   signed int widthSI,    /*width of entry box*/
   signed int idSI,       /*id of widget to draw*/
   signed int posArySI[], /*0 is cursor; 1 is scroll pos*/
   signed char blinkBl,   /*1: print blink cursor state*/
   signed char *textStr,  /*text to print in entry box*/
   signed char noDrawBl,  /*1: do not draw entry box*/
   struct widg_rayWidg *widgSTPtr /*has widget to draw*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun085 TOC:
   '   - draw an entry box, prevents typing if maxium is
   '     exceded or wrong entry type
   '   o fun085 sec01:
   '     - variable declarations
   '   o fun085 sec02:
   '     - check state of entry box
   '   o fun085 sec03:
   '     - copy string & check/set scroll & cursor values
   '   o fun085 sec04:
   '     - find text size and print cursor
   '   o fun085 sec05:
   '     - draw entry box
   '   o fun085 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = 0;
   struct Color textCol;

   #define def_maxChar_fun085 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun085 + 8];
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   signed int outLenSI = 0;
   signed int cursorPosSI = posArySI[1];

   signed char typeSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec02:
   ^   - check state of entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun085_sec06;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun085_sec06;
      /*entry box is hidden*/

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: entry box is being used*/
      typeSC = 0; /*want no borders*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      focusColorUI = widgSTPtr->textAltColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else If: entry box is being used*/

   /*will likely remove, or use lighter shade difference*/
   else if(
         widgSTPtr->stateAryUS[idSI] & def_active_rayWidg
      || widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg
   ){ /*Else If: entry box is being used*/
      typeSC = 0; /*want border*/
      colorUI = widgSTPtr->activeColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: entry box is being used*/

   else
   { /*Else: not being used*/
      typeSC = 1; /*want border*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else: not being used*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec03:
   ^   - copy string & check/set scroll & cursor values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   outLenSI = endStr_ulCp(textStr);

   if(posArySI[0] > outLenSI)
      posArySI[0] = outLenSI;
   else if(posArySI[0] < 0)
      posArySI[0] = 0;
      /*scroll position is past end of string*/

   /*cursor position is past end of string*/
   if(posArySI[1] > outLenSI)
      posArySI[1] = outLenSI;
   else if(posArySI[1] < 0)
      posArySI[1] = 0;

   outLenSI = posArySI[1] - posArySI[0];

   if(outLenSI < 0)
      posArySI[0] = posArySI[1];

   checkTextWidth_rayWidg(
       &textStr[posArySI[0]],
       outStr,
       widthSI,
       widgSTPtr
   );

   outLenSI = endStr_ulCp(outStr);

   if(outLenSI + posArySI[0] < posArySI[1])
   { /*If: position is out of bounds*/
      posArySI[0] = posArySI[1] - outLenSI;

      checkTextWidth_rayWidg(
          outStr,
          outStr,
          widthSI,
          widgSTPtr
      );

      outLenSI = endStr_ulCp(outStr);
   } /*If: position is out of bounds*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec04:
   ^   - find text size and print cursor
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(cursorPosSI >= 0)
   { /*If: adding in a cursor*/
      dupStr = &outStr[posArySI[1] - posArySI[0]];
      cpStr = &outStr[outLenSI];
      cpStr[2] = 0;

      while(cpStr > dupStr)
      { /*Loop: add cursor in*/
         cpStr[1] = cpStr[0];
         --cpStr;
      } /*Loop: add cursor in*/

      cpStr[1] = cpStr[0];

      if(blinkBl)
         *cpStr = def_blinkCursor_rayWidg;
      else
         *cpStr = def_cursor_rayWidg;

      checkTextWidth_rayWidg(
          outStr,
          outStr,
          widthSI,
          widgSTPtr
      );
   } /*If: adding in a cursor*/

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);
   widgSTPtr->widthArySI[idSI] = widthSI;
      /*want a one character pad around the entry box*/

   if(noDrawBl)
      goto done_fun085_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec05:
   ^   - draw entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI,
      typeSC,
      colorUI,
      borderColorUI,
      focusColorUI
   );

   if(*outStr)
      DrawTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) outStr,
         (Vector2) {
            widgSTPtr->xArySI[idSI]
               + widgSTPtr->fontWidthF,
            widgSTPtr->yArySI[idSI]
               + widgSTPtr->fontHeightF
              / def_widgHeightGap_rayWidg
         }, /*keeping x/y centered with box*/
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         textCol
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun085 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun085_sec06:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun085_sec06:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun085_sec06:;
      return 0;
} /*entryDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun086: entryEvent_rayWidg
|   - handels an event for an entry box
| Input:
|   - idSI:
|     o id of entry box
|   - posArySI:
|     o 2 element array
|       * index 0: is scroll position; 1st character shown
|       * index 1: is cursor position
|   - textStr:
|     o c-string with text to add character to
|   - textLenSI:
|     o current length of textStr
|   - maxLenSI:
|     o maximum string length
|   - textCheckFun:
|     o function to check if new chracter is valid
|     o use 0/null for no checks
|     o the function has three inputs and returns the
|       length of the string (signed int)
|       * c-string (signed char *) with text to check
|       * signed int with the cursor position
|       * signed int with the maximum length
|     o for examples see fun075, fun076, or fun077
|       * fun075: intStrCheck_rayWidg
|       * fun076: floatStrCheck_rayWidg
|       * fun077: fileStrCheck_rayWidg
|   - eventSTPtr:
|     o event_rayWidg sturct pointer with event to add
|   - widgSTPtr:
|     o has entry box widget
| Output:
|   - Modifies:
|     o posArySI to have updated scroll/cursor position if
|       event changed them (arrow, scroll, ect ...)
|     o textStr to have new character (if char event)
|   - Returns:
|     o length of c-string (textStr)
|     o -1 if no event happened
|     o -2 if idSI did not match the event id and the
|       entry box (form idSI) is not active
\-------------------------------------------------------*/
signed int
entryEvent_rayWidg(
   signed int idSI,       /*id of entry box widget*/
   signed int posArySI[], /*0: scroll pos; 1: cursor*/
   signed char *textStr,  /*c-string to update*/
   signed int textLenSI,  /*length of textStr*/
   signed int maxLenSI,   /*maximum allowed length*/
   signed int (
      textCheckFun(
         signed char *, /*text to check*/
         signed int,    /*cursor position*/
         signed int     /*maximum length*/
      )
   ), /*function to check if input is valid*/
   struct event_rayWidg *eventSTPtr, /*has event to add*/
   struct widg_rayWidg *widgSTPtr /*has entry box widget*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun086 TOC:
   '   - handels an event for an entry box
   '   o fun086 sec01:
   '     - variable declarations
   '   o fun086 sec02:
   '     - check if have inuput
   '   o fun086 sec03:
   '     - handel scroll wheel events
   '   o fun086 sec05:
   '     - handel key input event
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun086 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int cpSI = 0;
   signed int tmpSI = 0;
   signed int maxCharInBoxSI = 0;
   signed char tmpStr[257];
   signed char checkStr[257];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun086 Sec02:
   ^   - check if have inuput
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI == eventSTPtr->parIdSI)
      ;
   else if(idSI == eventSTPtr->childIdSI)
      ;
   else if( activeGet_widg_rayWidg(idSI, widgSTPtr) )
      ; /*the entry box is active, so taking input*/
   else
      goto wrongWidget_fun086;

   if(
         ! eventSTPtr->scrollF
      && ! eventSTPtr->leftReleaseBl
      && ! eventSTPtr->keySI
   ) goto noEvent_fun086;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun086 Sec03:
   ^   - handel scroll wheel events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(eventSTPtr->scrollF != 0)
   { /*If: have scroll wheel input*/
      checkTextWidth_rayWidg(
         &textStr[posArySI[0]],
         tmpStr,
         maxLenSI,
         widgSTPtr
      );
      maxLenSI = endStr_ulCp(tmpStr);
      maxCharInBoxSI = maxLenSI;

      if(! eventSTPtr->shiftBl)
        maxLenSI = 1;

      if(eventSTPtr->scrollF < 0)
        maxLenSI *= -1;

      posArySI[1] += maxLenSI;

      if(posArySI[1] < 0)
         posArySI[1] = 0;
      else if(posArySI[1] > textLenSI)
         posArySI[1] = textLenSI; /*scroll is at end*/

      if(posArySI[1] < posArySI[0])
         posArySI[0] = posArySI[1];
      else if(posArySI[1] > posArySI[0] + maxCharInBoxSI)
         posArySI[0] = posArySI[1] - maxCharInBoxSI;

      if(posArySI[0] < 0)
         posArySI[0] = 0;
      if(posArySI[1] >= textLenSI)
         posArySI[0] = textLenSI - 1;
   } /*If: have scroll wheel input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun086 Sec04:
   ^   - handel mouse click
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(eventSTPtr->leftReleaseBl)
   { /*Else If: left click event*/
      /*get width to selected item*/
      maxCharInBoxSI =
         eventSTPtr->xSI -widgSTPtr->xArySI[idSI];

      if(maxCharInBoxSI < widgSTPtr->fontWidthF)
         posArySI[1] = 0;

      else
      { /*Else: need to find position of characters*/
         tmpSI = 0;
         cpSI = 0;

         while(textStr[cpSI])
         { /*Loop: copy c-string in entry box*/
            if(cpSI == posArySI[1])
               tmpStr[tmpSI++] = def_cursor_rayWidg;
            tmpStr[tmpSI++] = textStr[cpSI++];
         } /*Loop: copy c-string in entry box*/

         checkTextWidth_rayWidg(
            &tmpStr[posArySI[0]],
            checkStr,
            maxCharInBoxSI,
            widgSTPtr
         );

         tmpSI = endStr_ulCp(checkStr);
         posArySI[1] = tmpSI - 1;
            /*-1 to convert index 1 to index 0*/
      } /*Else: need to find position of characters*/
   } /*Else If: left click event*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun086 Sec05:
   ^   - handel key input event
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(eventSTPtr->keySI)
   { /*If: had a keyboard event*/
      addCharToEntry_rayWidg(
         eventSTPtr->keySI,
         textStr,
         &textLenSI,
         widgSTPtr->widthArySI[idSI],
         posArySI,
         widgSTPtr
      ); /*add input key into c-string*/

      if(textCheckFun)
      { /*If: have function pointer*/
         textLenSI =
            textCheckFun(textStr, posArySI[1], maxLenSI);
      } /*If: have function pointer*/
   } /*If: had a keyboard event*/

   return textLenSI;

   noEvent_fun086:;
      return -1;
   wrongWidget_fun086:;
      return -2;
} /*entryEvent_rayWidg*/

/*-------------------------------------------------------\
| Fun087: labDraw_rayWidg
|   - draw a label
| Input:
|   - maxWidthSI:
|     o maximum width of label
|   - minWidthSI:
|     o minimum width of label
|   - idSI:
|     o widget id to draw
|   - textStr:
|     o text to draw (in label)
|   - padSC:
|     o character to pad with, use 0 for space
|   - padFlagSC:
|     o 0: to ignore padding
|     o 1: if beneath minimum width, left pad till at min
|     o 2: if beneath minimum width, right pad till at min
|     o 3: if beneath min width, pad both sides equaly
|     o this always adds one character of pad at the end
|       to separate it from other widgets
|   - noDrawBl:
|     o 1: update height, but do not draw entry box
|     o 0: update height and draw entry box
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with width,
|       coordinates (tile/x,y), and state of entry box
| Output
|   - Draws:
|     o text as label
|   - Modifies:
|     o heightArySI in widgSTPtr to have the button height
|     o both elements of posArySI to be on position in box
|   - Returns
|     o width of the entry box
|     o 0 if entry box wigdet is hidden
|     o def_noWidget_rayWidg if could not find widget
| Note:
|   - checks, hiden state everything else is normal
|   - maximum number of characters is 256
\-------------------------------------------------------*/
signed int
labDraw_rayWidg(
   signed int maxWidthSI, /*maximum width of entry box*/
   signed int minWidthSI, /*minimum width of entry box*/
   signed int idSI,       /*id of widget to draw*/
   signed char *textStr,  /*text to print in entry box*/
   signed char padSC,     /*character to pad with*/
   signed char padFlagSC, /*type of padding (if needed)*/
   signed char noDrawBl,  /*1: do not draw entry box*/
   struct widg_rayWidg *widgSTPtr /*has widget to draw*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun087 TOC:
   '   - draw a label
   '   o fun087 sec01:
   '     - variable declarations
   '   o fun087 sec02:
   '     - check state of label and copy string
   '   o fun087 sec03:
   '     - find text size and pad if needed
   '   o fun087 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun087 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxChar_fun087 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun087 + 8];
   signed int outLenSI = 0;
   signed int oldLenSI = 0;
   signed int tmpSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun087 Sec02:
   ^   - check state of label and copy string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun087_sec04;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun087_sec04;
      /*entry box is hidden*/

   if(minWidthSI > def_maxChar_fun087)
      minWidthSI = def_maxChar_fun087;

   textCol = GetColor(widgSTPtr->textColSI);
   outLenSI = endStr_ulCp(textStr);

   if(outLenSI >= def_maxChar_fun087)
   { /*If: user input to long of string*/
      outLenSI = def_maxChar_fun087 - 4;
         /*need on character for the cursor*/
      cpLen_ulCp(outStr, textStr, outLenSI);
      outStr[outLenSI++] = '.';
      outStr[outLenSI++] = '.';
      outStr[outLenSI++] = '.';
      outStr[outLenSI] = 0;
   } /*If: user input to long of string*/

   else
      cpLen_ulCp(outStr, textStr, outLenSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun087 Sec03:
   ^   - find text size and pad if needed
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   textDimVect2 =
      MeasureTextEx(
           *widgSTPtr->fontSTPtr,
           (char *)outStr,
           widgSTPtr->fontSizeSI,
           widgSTPtr->spacingF
        );
   tmpSI = textDimVect2.x;

   --outLenSI;

   while(tmpSI > maxWidthSI)
   { /*Loop: shorten string down*/
      outStr[outLenSI--] = 0;

      textDimVect2 =
        MeasureTextEx(
           *widgSTPtr->fontSTPtr,
           (char *)outStr,
           widgSTPtr->fontSizeSI,
           widgSTPtr->spacingF
        );
      tmpSI = textDimVect2.x;
   } /*Loop: shorten string down*/

   ++outLenSI;

   if(padFlagSC)
   { /*If: padding*/
      oldLenSI = outLenSI;

      while(tmpSI < minWidthSI)
      { /*Loop: add initial padding to end*/
         outStr[outLenSI++] = padSC;
         outStr[outLenSI] = 0;

         textDimVect2 =
           MeasureTextEx(
              *widgSTPtr->fontSTPtr,
              (char *)outStr,
              widgSTPtr->fontSizeSI,
              widgSTPtr->spacingF
           );
         tmpSI = textDimVect2.x;
      } /*Loop: add initial padding to end*/

      if(padFlagSC == 3)
         oldLenSI += (outLenSI - oldLenSI) >> 1;
         /*padding in both directions, add half of right
         `   pad to string length
         */

      if(padFlagSC & 1)
      { /*If: left padding*/
         cpLen_ulCp(
            &outStr[outLenSI - oldLenSI],
            outStr,
            oldLenSI
         );

         oldLenSI = outLenSI - oldLenSI - 1;

         while(oldLenSI >= 0)
            outStr[oldLenSI--] = padSC;
      } /*If: left padding*/
   } /*If: padding*/

   textDimVect2 =
      MeasureTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) outStr,
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF
     );
   outLenSI = textDimVect2.x;

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);
   widgSTPtr-> widthArySI[idSI] =
        outLenSI
      + widgSTPtr->fontWidthF
      + widgSTPtr->fontWidthF;
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun087_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun087 Sec04:
   ^   - draw label and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*outStr)
      DrawTextEx(
          *widgSTPtr->fontSTPtr,
          (char *) outStr,
          (Vector2) {
             widgSTPtr->xArySI[idSI],
             widgSTPtr->yArySI[idSI]
                + widgSTPtr->fontHeightF
                / def_widgHeightGap_rayWidg
          }, /*keeping x and y centered with box*/
          widgSTPtr->fontSizeSI,
          widgSTPtr->spacingF,
          textCol
      );

   done_fun087_sec04:;
      return outLenSI;

   noWidget_fun087_sec04:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun087_sec04:;
      return 0;
} /*labDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun088: mkMesgBox_rayWidg
|   - makes an emtpy message box widget
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add message box
|       and its children widgets to
| Output:
|   - Modifies:
|     o resizes arrays in widgSTPtr if not enough memory
|       to hold the message box and its children widgets
|   - Returns:
|     o parent id (index) of the message box widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed int
mkMesgBox_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   signed int idSI = 0;
   idSI = widgSTPtr->lenSI;

   if(widgSTPtr->lenSI + 3 >= widgSTPtr->sizeSI)
   { /*If: need more memory*/
      idSI += (idSI >> 1);

      if( realloc_widg_rayWidg(idSI, widgSTPtr) )
         goto memErr_fun089;
   } /*If: need more memory*/

   idSI =
     addWidget_widg_rayWidg(0, 0, 0, 200, 100, widgSTPtr);
   inactiveAdd_widg_rayWidg(idSI, widgSTPtr);

   addWidget_widg_rayWidg(
      0, /*colomn*/
      0, /*row*/
      1, /*use tile*/
      200, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   ); /*add the top border*/
   childAdd_widg_rayWidg(idSI + 1, widgSTPtr);
   inactiveAdd_widg_rayWidg(idSI + 1, widgSTPtr);

   addWidget_widg_rayWidg(
      0, /*colomn*/
      1, /*row*/
      1, /*use tile*/
      200, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   ); /*add the message*/
   childAdd_widg_rayWidg(idSI + 2, widgSTPtr);
   inactiveAdd_widg_rayWidg(idSI + 2, widgSTPtr);

   addWidget_widg_rayWidg(
      0,  /*colomn*/
      2,  /*row*/
      1,  /*use tile*/
      20, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   ); /*add the ok button*/
   childAdd_widg_rayWidg(idSI + 3, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 3, widgSTPtr);

   hidenAdd_widg_rayWidg(idSI, widgSTPtr);
      /*hide the message box*/
   return idSI;

   memErr_fun089:;
      return -1;
} /*mkMesgBox_rayWidg*/

/*-------------------------------------------------------\
| Fun089: mesgBoxDraw_rayWidg
|   - draws a message box
| Input:
|   - idSI:
|     o id of message boxes parent widget
|       * a message box is a complex widget, with three
|         child widgets
|   - mesgStr:
|     o text to print in message box
|       * one line, with a max of 128 characters
|       * a message box should be a breif warning
|   - butTextStr:
|     o text to print on the button
|       * use 0/null for "ok"
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with message box
|       widget
| Output:
|   - Draws:
|     o message box to screen (if no errors)
|   - Modifies:
|     o message box and its child widgets to be in a
|       normal state
|   - Returns:
|     o width of message box if drawn
|     o def_noWidge_rayWidg if widget does not exit
|     o 0 if widget is hidden (not drawn)
|     o -1 if message was to long
\-------------------------------------------------------*/
signed int
mesgBoxDraw_rayWidg(
   signed int idSI,       /*id of message box widget*/
   signed char *mesgStr,  /*message to show*/
   signed char *butTextStr, /*text on button*/
   struct widg_rayWidg *widgSTPtr /*has widgets*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun089 TOC:
   '   - draws a message box
   '   o fun089 sec01:
   '     - variable declarations
   '   o fun089 sec02:
   '     - initalize and copy/edit string
   '   o fun089 sec03:
   '     - find widget widths
   '   o fun089 sec04:
   '     - find widget coordinates
   '   o fun089 sec05:
   '     - draw message box
   '   o fun089 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   Vector2 textDimVect2;

   #define def_lenOut_fun089 128
   signed char outStr[def_lenOut_fun089 + 8];
   signed int textLenSI = endStr_ulCp(mesgStr);
   signed int butLenSI = 0;
   signed int maxLenSI = 0;

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   signed int widthSI = widgSTPtr->winWidthSI;
   signed int heightSI = widgSTPtr->winHeightSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec02:
   ^   - initalize and copy/edit string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun089_sec06;
   if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hidden_fun089_sec06;
   if(textLenSI > def_lenOut_fun089)
      goto mesgErr_fun089_sec06;

   cpStr = mesgStr;
   dupStr = outStr;

   while(*cpStr)
   { /*Loop: copy message over*/
      if(*cpStr == '\n' || *cpStr == '\r')
         ++cpStr; /*do not want new lines*/
      else
         *dupStr++ = *cpStr++;
   } /*Loop: copy message over*/

   *dupStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec03:
   ^   - find widget widths
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   textDimVect2 =
      MeasureTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) mesgStr,
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF
      );
   textLenSI = textDimVect2.x;
   hidenClear_widg_rayWidg(idSI + 2, widgSTPtr);
   textLenSI =
      labDraw_rayWidg(
         textLenSI << 1,
         textLenSI,
         idSI + 2,   /*second child is label*/
         mesgStr,
         ' ',  /*pad with spaces*/
         0,    /*do not apply padding*/
         1,    /*do not draw yet*/
         widgSTPtr
      );

   textDimVect2 =
      MeasureTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) butTextStr,
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF
      );
   butLenSI = textDimVect2.x;
   hidenClear_widg_rayWidg(idSI + 3, widgSTPtr);
   butLenSI =
      butDraw_rayWidg(
         butLenSI << 1,
         butLenSI,
         idSI + 3,   /*third child is label*/
         butTextStr,
         1,          /*do not draw yet*/
         widgSTPtr
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec04:
   ^   - find widget coordinates
   ^   o fun089 sec04 sub01:
   ^     - find x coordinates
   ^   o fun089 sec04 sub02:
   ^     - find y coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun089 Sec04 Sub01:
   *   - find x coordinates
   \*****************************************************/

   if(textLenSI > butLenSI)
      maxLenSI = textLenSI;
   else
      maxLenSI = butLenSI;


   widgSTPtr->widthArySI[idSI] =
      maxLenSI + (widgSTPtr->fontWidthF * 2);
   /*find center of message box*/
   if(widthSI < widgSTPtr->widthArySI[idSI])
      widgSTPtr->xArySI[idSI] = 0;
      /*no center can be found, message to long*/
   else
      widgSTPtr->xArySI[idSI] =
         (widthSI - widgSTPtr->widthArySI[idSI]) >> 1;


   /*top border*/
   widgSTPtr->widthArySI[idSI + 1] =
      widgSTPtr->widthArySI[idSI];
   widgSTPtr->xArySI[idSI + 1] =
      widgSTPtr->xArySI[idSI];

   /*message*/
   widgSTPtr->xArySI[idSI + 2] =
      (maxLenSI - textLenSI) >> 1;
      /*center the message*/
   widgSTPtr->xArySI[idSI + 2] +=
      widgSTPtr->xArySI[idSI];

   /*button*/
   widgSTPtr->xArySI[idSI + 3] =
      (maxLenSI - butLenSI) >> 1;
      /*center the button*/
   widgSTPtr->xArySI[idSI + 3] +=
      widgSTPtr->xArySI[idSI];

   /*****************************************************\
   * Fun089 Sec04 Sub02:
   *   - find y coordinates
   \*****************************************************/

   /*find y-coordinates*/
   widgSTPtr->yArySI[idSI + 1] = 0;
   widgSTPtr->heightArySI[idSI + 1] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);

   widgSTPtr->yArySI[idSI + 2] =
        widgSTPtr->yArySI[idSI + 1]
      + widgSTPtr->heightArySI[idSI + 1]
      + widgSTPtr->fontHeightF
      / def_widgHeightGap_rayWidg;

   widgSTPtr->yArySI[idSI + 3] =
        widgSTPtr->yArySI[idSI + 2]
      + widgSTPtr->heightArySI[idSI + 2]
      + widgSTPtr->fontHeightF
      / def_widgHeightGap_rayWidg;

   /*mesage box height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->yArySI[idSI + 3]
      + widgSTPtr->heightArySI[idSI + 3]
      + widgSTPtr->fontHeightF
      / def_widgHeightGap_rayWidg;


   /*find center of message box*/
   if(heightSI < widgSTPtr->heightArySI[idSI])
      widgSTPtr->yArySI[idSI] = 0;
      /*no center can be found, programer messed up*/
   else
      widgSTPtr->yArySI[idSI] =
         (heightSI - widgSTPtr->heightArySI[idSI]) >> 1;

   widgSTPtr->yArySI[idSI + 1] = widgSTPtr->yArySI[idSI];
   widgSTPtr->yArySI[idSI + 2] += widgSTPtr->yArySI[idSI];
   widgSTPtr->yArySI[idSI + 3] += widgSTPtr->yArySI[idSI];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec05:
   ^   - draw message box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*this widget's button is in hog state, so it should
   `  take over the GUI
   */
   if(widgSTPtr->focusSI != idSI + 3)
   { /*If: message box is not the priority*/
      activeClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      );
      pressClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      );
      focusClear_widg_rayWidg(
        widgSTPtr->focusSI,
        widgSTPtr
      );

      widgSTPtr->focusSI = idSI + 3;
      focusAdd_widg_rayWidg(idSI + 3, widgSTPtr);
   } /*If: message box is not the priority*/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI,
      1,       /*only draw the inner border*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      0
   ); /*rectangle holding message box*/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI + 1,
      1,       /*only draw the inner border*/
      widgSTPtr->activeColSI,
      widgSTPtr->borderColSI,
      0
   ); /*top border of message box*/

   widgSTPtr->widthArySI[idSI + 2] =
      labDraw_rayWidg(
         textLenSI << 1,
         textLenSI,
         idSI + 2,   /*second child is label*/
         mesgStr,
         ' ',  /*pad with spaces*/
         0,    /*do not apply padding*/
         0,    /*draw widget*/
         widgSTPtr
      ); /*message user provided*/

   widgSTPtr->widthArySI[idSI + 3] =
      butDraw_rayWidg(
         butLenSI << 1,
         butLenSI,
         idSI + 3,   /*third child is label*/
         butTextStr,
         0,          /*draw*/
         widgSTPtr
      ); /*button to accept message*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun089 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun089_sec06:;
      return def_noWidget_rayWidg;

   hidden_fun089_sec06:;
      return 0;

   mesgErr_fun089_sec06:;
      return -1;
} /*mesgBoxDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun090: mesgBoxEvent_rayWidg
|   - checks the event that was fired in a message box
| Input:
|   - eventSC:
|     o event type that happened
|       * 0: for no event, clear press state on button
|       * 1: for press event
|       * 2: for release event
|   - parIdSI:
|     o id of message boxes parent widget
|       * a message box is a complex widget, with three
|         child widgets
|   - childIdSI:
|     o id of the child widget clicked in the message
|       box
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with message box
|       widget
| Output:
|   - Modifies:
|     o if button was clicked, sets the message box and
|       its child widgets to the hidden state
|   - Returns:
|     o 0 if message box button was not clicked or if was
|       a press event or clear event
|     o 1 if message box button was clicked
\-------------------------------------------------------*/
signed int
mesgBoxEvent_rayWidg(
   signed char eventSC,  /*event that happened*/
   signed int parIdSI,   /*parent id of message box*/
   signed int childIdSI, /*id of child widget clicked*/
   struct widg_rayWidg *widgSTPtr  /*has widgets*/
){
   signed int focusSI = 0;
   signed int firstFocusSI = -1;

   if(! eventSC)
   { /*If: clearing button press*/
      pressClear_widg_rayWidg(parIdSI + 3, widgSTPtr);
      goto noEvent_fun091;
   } /*If: clearing button press*/

   else if(childIdSI - parIdSI != 3)
      goto emptyClick_fun091;

   else if(eventSC == 1)
   { /*Else If: press event*/
      pressAdd_widg_rayWidg(childIdSI, widgSTPtr);
      goto noEvent_fun091;
   } /*Else If: press event*/

   else if(eventSC == 2)
   { /*If: releasing widget*/
      pressClear_widg_rayWidg(childIdSI, widgSTPtr);
      hidenAdd_widg_rayWidg(parIdSI, widgSTPtr);

      focusSI = widgSTPtr->focusSI + 1;

      if(focusSI >= widgSTPtr->lenSI)
         focusSI = 0;

      while(firstFocusSI != widgSTPtr->focusSI)
      { /*Loop: find next widget*/
         if(focusSI >= widgSTPtr->lenSI)
            focusSI = 0;

         else if(focusSI == widgSTPtr->focusSI)
         { /*Else If: nothing to focus*/
            if(firstFocusSI < 0)
            { /*If: nothing can be focused*/
               widgSTPtr->focusSI = 0;
               break;
            } /*If: nothing can be focused*/

            else
            { /*Else: have new focus*/
               widgSTPtr->focusSI = firstFocusSI;
               focusAdd_widg_rayWidg(
                  firstFocusSI,
                  widgSTPtr
               );
               activeAdd_widg_rayWidg(
                  firstFocusSI,
                  widgSTPtr
               );
               break;
            } /*Else: have new focus*/
         } /*Else If: nothing to focus*/

         else if(
              widgSTPtr->stateAryUS[focusSI]
            & def_hog_rayWidg
         ){ /*Else If: found the next hog widget*/
            widgSTPtr->focusSI = firstFocusSI;
            focusAdd_widg_rayWidg(
               firstFocusSI,
               widgSTPtr
            );
            activeAdd_widg_rayWidg(
               firstFocusSI,
               widgSTPtr
            );
            break;
         }  /*Else If: found the next hog widget*/

         else if(
              widgSTPtr->stateAryUS[focusSI]
            & def_hiden_rayWidg
         ) ;

         else if(
              widgSTPtr->stateAryUS[focusSI]
            & def_inactive_rayWidg
         ) ;

         else if(firstFocusSI < 0)
            firstFocusSI = focusSI;

         ++focusSI;
      }  /*Loop: find next widget*/
   } /*If: releasing widget*/

   return 1;

   noEvent_fun091:;
      return 0;
   emptyClick_fun091:;
      return 0;
} /*mesgBoxEvent_rayWidg*/

/*-------------------------------------------------------\
| Fun092: blank_listBox_rayWidg
|   - blanks a listBox_rayWidg structure
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure to blank
| Output:
|   - Modifies:
|     o listSTPtr to have all values set to 0
|       * this is lazy, so it reducles lengths to 0 when
|         possible instead of looping
\-------------------------------------------------------*/
void
blank_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   if(! listSTPtr)
      return;

   listSTPtr->maxWidthSI = 0;
   listSTPtr->minWidthSI = 0;
   listSTPtr->lineWidthSI = 0;
   listSTPtr->lineLenSI = 0;

   
   listSTPtr->maxHeightSI = 0;
   listSTPtr->minHeightSI = 0;

   listSTPtr->lenSI = 0;
   listSTPtr->maxSelectSI = -1;
   listSTPtr->numSelectSI = 0;

   listSTPtr->onSI = 0;
   listSTPtr->lastSelectSI = -1;

   listSTPtr->scrollSI = 0;
   listSTPtr->lastScrollSI = 0;
   listSTPtr->clickSI = -1;

   listSTPtr->sideScrollSI = 0;
} /*blank_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun093: init_listBox_rayWidg
|   - initializes a listBox_rayWidg structure
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure to initialize
| Output:
|   - Modifies:
|     o listSTPtr to have all values set to 0 and all
|       arrays set to 0/null
\-------------------------------------------------------*/
void
init_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   if(! listSTPtr)
      return;

   listSTPtr->textAryStr = 0;
   listSTPtr->sizeArySI = 0;
   listSTPtr->stateArySC = 0;
   listSTPtr->sizeSI = 0;
   listSTPtr->lineWidthSI = 0;
   listSTPtr->lastScaleF = 0;

   blank_listBox_rayWidg(listSTPtr);
} /*init_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun094: freeStack_listBox_rayWidg
|   - frees variables in a listBox_rayWidg structure
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure with variables to free
| Output:
|   - Frees:
|     o textAryStr, stateArySI, and stateArySC in
|       listSTPtr
|  - Modifies:
|     o initializes all varaibles to 0/null
\-------------------------------------------------------*/
void
freeStack_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   if(! listSTPtr)
      return;

   if(listSTPtr->sizeArySI)
      free(listSTPtr->sizeArySI);
   if(listSTPtr->stateArySC)
      free(listSTPtr->stateArySC);

   for(
      listSTPtr->lenSI = 0; 
      listSTPtr->lenSI < listSTPtr->sizeSI;
      ++listSTPtr->lenSI
   ){ /*Loop: free text in list box*/
      if(listSTPtr->textAryStr[listSTPtr->lenSI])
         free(listSTPtr->textAryStr[listSTPtr->lenSI]);
      listSTPtr->textAryStr[listSTPtr->lenSI] = 0;
   }  /*Loop: free text in list box*/

   free(listSTPtr->textAryStr);
   init_listBox_rayWidg(listSTPtr);
} /*freeStack_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun095: freeHeap_listBox_rayWidg
|   - frees a heap allocated listBox_rayWidg structure
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure to free
| Output:
|   - Frees:
|     o listSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   if(! listSTPtr)
      return;
   freeStack_listBox_rayWidg(listSTPtr);
   free(listSTPtr);
} /*freeHeap_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun096: realloc_listBox_rayWidg
|   - add more memory to listBox_rayWidg structure
| Input:
|   - sizeSI:
|     o new size of arrays; must be >= sizeSI in listSTPtr
|     o input 0 for auto expand (1.5 times)
|   - listSTPtr:
|     o listBox_rayWidg structure to increase size of
| Output:
|   - Modifies:
|     o textAryStr, sizeArySI, and stateArySC in listSTPtr
|       to have more memory
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
realloc_listBox_rayWidg(
   signed int sizeSI,             /*new array size*/
   struct listBox_rayWidg *listSTPtr
      /*has arrays to expand*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun096 TOC:
   '   - add more memory to listBox_rayWidg structure
   '   o fun096 sec0x:
   '     - return results
   '   o fun096 sec01:
   '     - variable declarations and initial checks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun096 Sec01:
   ^   - variable declarations and initial checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char **swapAryStr = 0;
   signed int *swapSIPtr = 0;
   signed char *swapStr = 0;
   signed int tmpSI = 0;

   if(! listSTPtr)
      goto noExpand_fun096_sec03;

   else if(sizeSI <= 0)
   { /*Else If: auto expanding*/
      if(listSTPtr->sizeSI >= 8)
         sizeSI =
            listSTPtr->sizeSI + (listSTPtr->sizeSI >> 1);
      else
         sizeSI = 8;
   } /*Else If: auto expanding*/

   else if(sizeSI <= listSTPtr->sizeSI)
      goto noExpand_fun096_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun096 Sec02:
   ^   - expand memory
   ^   o fun096 sec02 sub01:
   ^     - c-string array (list box entries) expansion
   ^   o fun096 sec02 sub02:
   ^     - c-string size array expansion
   ^   o fun096 sec02 sub03:
   ^     - state array expansion
   ^   o fun096 sec02 sub04:
   ^     - initialize memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun096 Sec02 Sub01:
   *   - c-string array (list box entries) expansion
   \*****************************************************/

   if(! listSTPtr->textAryStr)
   { /*If: first memory allocation*/
      listSTPtr->textAryStr =
         malloc(sizeSI * sizeof(char *));
      if(! listSTPtr->textAryStr)
         goto memErr_fun096_sec03;
      listSTPtr->lenSI = 0;
   } /*If: first memory allocation*/

   else
   { /*Else: memory reallocation*/
      swapAryStr =
         realloc(
            listSTPtr->textAryStr,
            sizeSI * sizeof(char *)
         );

      if(! swapAryStr)
         goto memErr_fun096_sec03;
      listSTPtr->textAryStr = swapAryStr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun096 Sec02 Sub02:
   *   - c-string size array expansion
   \*****************************************************/

   if(! listSTPtr->sizeArySI)
   { /*If: first memory allocation*/
      listSTPtr->sizeArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! listSTPtr->sizeArySI)
         goto memErr_fun096_sec03;
      listSTPtr->lenSI = 0;
   } /*If: first memory allocation*/

   else
   { /*Else: memory reallocation*/
      swapSIPtr =
         realloc(
            listSTPtr->sizeArySI,
            sizeSI * sizeof(signed int)
         );

      if(! swapSIPtr)
         goto memErr_fun096_sec03;
      listSTPtr->sizeArySI = swapSIPtr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun096 Sec02 Sub03:
   *   - state array expansion
   \*****************************************************/

   if(! listSTPtr->stateArySC)
   { /*If: first memory allocation*/
      listSTPtr->stateArySC =
         malloc(sizeSI * sizeof(signed int));
      if(! listSTPtr->stateArySC)
         goto memErr_fun096_sec03;
      listSTPtr->lenSI = 0;
   } /*If: first memory allocation*/

   else
   { /*Else: memory reallocation*/
      swapStr =
         realloc(
            listSTPtr->stateArySC,
            sizeSI * sizeof(signed char)
         );

      if(! swapStr)
         goto memErr_fun096_sec03;
      listSTPtr->stateArySC = swapStr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun096 Sec02 Sub04:
   *   - initialize memory
   \*****************************************************/

   for(
      tmpSI = listSTPtr->lenSI;
      tmpSI < sizeSI;
      ++tmpSI
   ){ /*Loop: inititialize new memory*/
      listSTPtr->textAryStr[tmpSI] = 0;
      listSTPtr->sizeArySI[tmpSI] = 0;
      listSTPtr->stateArySC[tmpSI] = 0;
   }  /*Loop: inititialize new memory*/

   listSTPtr->sizeSI = sizeSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun096 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   noExpand_fun096_sec03:;
      return 0;
   memErr_fun096_sec03:;
      return 1;
} /*realloc_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun097: clear_listBox_rayWidg
|   - removes all items from the list box
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure to clear items
| Output:
|   - Modifies:
|     o lenSI, onSI, and scrollSI in listSTPtr to be 0
|       (lazy delete)
\-------------------------------------------------------*/
void
clear_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   listSTPtr->lenSI = 0;
   listSTPtr->lineWidthSI = 0;
   listSTPtr->lineLenSI = 0;

   listSTPtr->onSI = 0;
   listSTPtr->lastSelectSI = -1;

   listSTPtr->scrollSI = 0;
   listSTPtr->lastScrollSI = 0;
   listSTPtr->clickSI = -1;

   listSTPtr->sideScrollSI = 0;

   listSTPtr->numSelectSI = 0;
} /*clear_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun098: clearSelect_listBox_rayWidg
|   - clear all selections from a list box
| Input:
|   - listSTPtr:
|     o listBox_rayWidg structure to clear
| Output:
|   - Modifies:
|     o stateArySC to have no selected items
\-------------------------------------------------------*/
void
clearSelect_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){
   signed int siItem = 0;

   for(siItem = 0; siItem < listSTPtr->lenSI; ++siItem)
      listSTPtr->stateArySC[siItem] &=
         ~def_listSelect_rayWidg;
         /*remove select state from all items*/

   listSTPtr->numSelectSI = 0;
   listSTPtr->lastSelectSI = -1;
} /*clearSelect_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun099: selectMaxSet_listBox_rayWidg
|   - sets maximum number of items that can be selected in
|     a list box
| Input:
|   - maxSI:
|     o maximum number of items to select
|   - listSTPtr:
|     o listBox_rayWidg struct pointer to set max select
| Output:
|   - Modifies:
|     o maxSelectSI in listSTPtr to be maxSI
\-------------------------------------------------------*/
void
selectMaxSet_listBox_rayWidg(
   signed int maxSI,
   struct listBox_rayWidg *listSTPtr
){
   listSTPtr->maxSelectSI = maxSI;
} /*selectMaxSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun100: widthSet_listBox_rayWidg
|   - sets maximum and minimum width of a list box
| Input:
|   - maxSI:
|     o maximum width (in pixels) of list box
|   - minSI:
|     o minimum width (in pixels) of list box
|   - listSTPtr:
|     o listBox_rayWidg struct pointer to set max width
| Output:
|   - Modifies:
|     o maxWidthSI in listSTPtr to be maxSI
|     o minWidthSI in listSTPtr to be minSI
\-------------------------------------------------------*/
void
widthSet_listBox_rayWidg(
   signed int maxSI,
   signed int minSI,
   struct listBox_rayWidg *listSTPtr
){
   listSTPtr->maxWidthSI = maxSI;
   listSTPtr->minWidthSI = minSI;
} /*widthSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun101: heightSet_listBox_rayWidg
|   - sets maximum and minimum height of a list box
| Input:
|   - maxSI:
|     o maximum height (in pixels) of list box
|   - minSI:
|     o minimum height (in pixels) of list box
|   - listSTPtr:
|     o listBox_rayWidg struct pointer to set max height
| Output:
|   - Modifies:
|     o maxHeightSI in listSTPtr to be maxSI
|     o minHeightSI in listSTPtr to be minSI
\-------------------------------------------------------*/
void
heightSet_listBox_rayWidg(
   signed int maxSI,
   signed int minSI,
   struct listBox_rayWidg *listSTPtr
){
   listSTPtr->maxHeightSI = maxSI;
   listSTPtr->minHeightSI = minSI;
} /*heightSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun102: hiddenSet_listBox_rayWidg
|   - sets a list box item to the hiden state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - addBl:
|     1: add state in
|     0: remove state
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the hidden state to a list box item
\-------------------------------------------------------*/
void
hiddenSet_listBox_rayWidg(
   signed int itemSI,
   signed char addBl,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return;
   else if(addBl)
      listSTPtr->stateArySC[itemSI] |=
         def_listHidden_rayWidg;
   else
      listSTPtr->stateArySC[itemSI] &=
         ~def_listHidden_rayWidg;
} /*hiddenSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun103: selectSet_listBox_rayWidg
|   - sets a list box item to the select state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - addBl:
|     1: add state in
|     0: remove state
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the select state to a list box item
\-------------------------------------------------------*/
void
selectSet_listBox_rayWidg(
   signed int itemSI,
   signed char addBl,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return;
   else if(addBl)
      listSTPtr->stateArySC[itemSI] |=
         def_listSelect_rayWidg;
   else
      listSTPtr->stateArySC[itemSI] &=
         ~def_listSelect_rayWidg;
} /*selectSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun104: specialSet_listBox_rayWidg
|   - sets a list box item to the special state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - addBl:
|     1: add state in
|     0: remove state
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the special state to a list box item
\-------------------------------------------------------*/
void
specialSet_listBox_rayWidg(
   signed int itemSI,
   signed char addBl,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return;
   else if(addBl)
      listSTPtr->stateArySC[itemSI] |=
         def_listSpecial_rayWidg;
   else
      listSTPtr->stateArySC[itemSI] &=
         ~def_listSpecial_rayWidg;
} /*specialSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun105: canSelectSet_listBox_rayWidg
|   - sets a list box item to the can select state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - addBl:
|     1: add state in
|     0: remove state
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the can select state to a list box item
\-------------------------------------------------------*/
void
canSelectSet_listBox_rayWidg(
   signed int itemSI,
   signed char addBl,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return;
   else if(addBl)
      listSTPtr->stateArySC[itemSI] |=
         def_listCanSelect_rayWidg;
   else
      listSTPtr->stateArySC[itemSI] &=
         ~def_listCanSelect_rayWidg;
} /*canSelectSet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun106: hiddenGet_listBox_rayWidg
|   - gets if a list box item is in the hiden state
| Input:
|   - itemSI:
|     o index of item to get state of
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Returns:
|     o 0 if item is not hidden
|     o 1 if item is not hidden
\-------------------------------------------------------*/
signed char
hiddenGet_listBox_rayWidg(
   signed int itemSI,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return 0;

   return
        listSTPtr->stateArySC[itemSI]
      & def_listHidden_rayWidg;
} /*hiddenGet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun107: selectGet_listBox_rayWidg
|   - sets a list box item to the select state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the select state to a list box item
\-------------------------------------------------------*/
signed char
selectGet_listBox_rayWidg(
   signed int itemSI,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return 0;

   return
        listSTPtr->stateArySC[itemSI]
      & def_listSelect_rayWidg;
} /*selectGet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun108: specialGet_listBox_rayWidg
|   - sets a list box item to the special state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Returns:
|     o 1 if item has special state
|     o 0 if item does not have special state
\-------------------------------------------------------*/
signed char
specialGet_listBox_rayWidg(
   signed int itemSI,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return 0;

   return
        listSTPtr->stateArySC[itemSI]
      & def_listSpecial_rayWidg;
} /*specialGet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun109: canSelectGet_listBox_rayWidg
|   - sets a list box item to the can select state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Returns:
|     o 1 if item has "can select" state
|     o 0 if item does not have "can select" state
\-------------------------------------------------------*/
signed char
canSelectGet_listBox_rayWidg(
   signed int itemSI,
   struct listBox_rayWidg *listSTPtr
){
   if(itemSI >= listSTPtr->lenSI)
      return 0;

   return
        listSTPtr->stateArySC[itemSI]
      & def_listCanSelect_rayWidg;
} /*canSelectGet_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun110: mk_listBox_rayWidg
|   - makes/adds a list box to a widg_widgST structure
| Input:
|   - xSI:
|     o x coordinate of list box, or the column is in
|   - ySI:
|     o y coordinate of list box, or the row is in
|   - tileBl
|     o 1: xSI and ySI are tiling coordinates
|     o 0: xSI and ySI are x,y pixel coordinates
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add list box to
| Output:
|   - Mofidies:
|     o widgSTPtr to have the list box widgets
|       * 0 is parent widget
|       * 1 is list box
|       * 2 is reserved (maybe vertical scroll bar)
|       * 3 is reserved (maybe horizontal scroll bar)
|   - Returns:
|     o parent id (index) of list box widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed char
mk_listBox_rayWidg(
   signed int xSI,            /*x coordinate of list box*/
   signed int ySI,            /*y coordinate of list box*/
   signed char tileBl,        /*1: using tileing*/
   struct widg_rayWidg *widgSTPtr  /*add list box to*/
){
   signed int idSI = 0;
   idSI = widgSTPtr->lenSI;

   if(idSI >= widgSTPtr->sizeSI)
   { /*If: need more room*/
      idSI += 1 + (idSI >> 1);

      if( realloc_widg_rayWidg(idSI, widgSTPtr) )
         goto memErr_fun100;
   } /*If: need more room*/

   idSI =
      addWidget_widg_rayWidg(
         xSI,       /*x coordinate*/
         ySI,       /*y coordinate*/
         !! tileBl, /*using exact or tile coords*/
         0,   /*width is added in later*/
         0,   /*height is added in later*/
         widgSTPtr  /*add list box to*/
      );

   return idSI;

   memErr_fun100:;
      return -1;
} /*mk_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun111: sort_listBox_rayWidg
|   - sorts a list box naturaly (aphabetically + numbers)
| Input:
|   - listSTPtr:
|     o listBox_rayWidg struct pointer to sort
| Output:
|   - Modifies:
|     o textAryStr in listSTPtr to be sorted
|     o sizeArySI and stateArySC in listSTPtr to be kept
|       in sync with textAryStr
\-------------------------------------------------------*/
void
sort_listBox_rayWidg(
   struct listBox_rayWidg *listSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun111 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun111 sec01:
   '     - variable declerations
   '   o fun111 sec02:
   '     - find the number of rounds to sort for
   '   o fun111 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun111 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *swapStr = 0;

   /*Number of elements to sort*/
   signed long numElmSL = listSTPtr->lenSI - 1;
   signed long endSL = listSTPtr->lenSI - 1;

   /*Number of sorting rounds*/
   signed long subSL = 0;
   signed long nextSL = 0;
   signed long lastSL = 0;
   signed long onSL = 0;

   /*Variables to incurment loops*/
   signed long slIndex = 0;
   signed long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun111 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numElmSL < 2)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < numElmSL - 1)
      subSL = (3 * subSL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun111 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSL > 0)
   { /*Loop: all rounds*/
      for(
         slIndex = 1;
         slIndex <= subSL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subSL <= endSL;
            slElm += subSL
         ){ /*Loop: swap elements in subarray*/
            nextSL = slElm + subSL;

            if(
               eqlNull_ulCp(
                  listSTPtr->textAryStr[slElm],
                  listSTPtr->textAryStr[nextSL]
               ) > 0
            ){ /*Else If: I need to swap an element*/
               swapStr = listSTPtr->textAryStr[slElm];
               listSTPtr->textAryStr[slElm] =
                  listSTPtr->textAryStr[nextSL];
               listSTPtr->textAryStr[nextSL] = swapStr;

               listSTPtr->sizeArySI[slElm] ^=
                  listSTPtr->sizeArySI[nextSL];
               listSTPtr->sizeArySI[nextSL] ^=
                  listSTPtr->sizeArySI[slElm];
               listSTPtr->sizeArySI[slElm] ^=
                  listSTPtr->sizeArySI[nextSL];

               listSTPtr->stateArySC[slElm] ^=
                  listSTPtr->stateArySC[nextSL];
               listSTPtr->stateArySC[nextSL] ^=
                  listSTPtr->stateArySC[slElm];
               listSTPtr->stateArySC[slElm] ^=
                  listSTPtr->stateArySC[nextSL];

               lastSL = slElm;
               onSL = slElm;

               while(lastSL >= subSL)
               { /*loop: move swapped element back*/
                  lastSL -= subSL;
                  if(! lastSL)
                     break;

                  else if(
                     eqlNull_ulCp(
                        listSTPtr->textAryStr[onSL],
                        listSTPtr->textAryStr[lastSL]
                     ) >= 0
                  ) break;

                  swapStr = listSTPtr->textAryStr[onSL];
                  listSTPtr->textAryStr[onSL] =
                     listSTPtr->textAryStr[lastSL];
                  listSTPtr->textAryStr[lastSL] = swapStr;

                  listSTPtr->sizeArySI[onSL] ^=
                     listSTPtr->sizeArySI[lastSL];
                  listSTPtr->sizeArySI[lastSL] ^=
                     listSTPtr->sizeArySI[onSL];
                  listSTPtr->sizeArySI[onSL] ^=
                     listSTPtr->sizeArySI[lastSL];

                  listSTPtr->stateArySC[onSL] ^=
                     listSTPtr->stateArySC[lastSL];
                  listSTPtr->stateArySC[lastSL] ^=
                     listSTPtr->stateArySC[onSL];
                  listSTPtr->stateArySC[onSL] ^=
                     listSTPtr->stateArySC[lastSL];

                  onSL = lastSL;
               } /*Loop: move swapped element back*/
            } /*Else If: I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSL = (subSL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortFiles_files_rayWidg*/

/*-------------------------------------------------------\
| Fun112: addItem_listBox_rayWidg
|   - adds an item to a list box
| Input:
|   - textStr:
|     o c-string with text to add
|   - stateSC:
|     o state of item in list box (can be any combo of)
|       * 0 for default (canSelect state, but no other)
|       * def_listHidden_rayWidg for hidden item
|       * def_listCanSelect_rayWidg to tell is visible
|         and that the item can be selected
|       * def_listSpecial_rayWidg for a special item
|       * def_listSelect_rayWidg for a selected item
|   - listSTPtr:
|     o listBox_rayWidg structure to free
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font to use
| Output:
|   - Mofidies:
|     o textAryStr, sizeArySI
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addItem_listBox_rayWidg(
   signed char *textStr,             /*text to add in*/
   signed char stateSC,              /*state of new item*/
   struct listBox_rayWidg *listSTPtr,/*add item to*/
   struct widg_rayWidg *widgSTPtr    /*has font*/
){
   signed int lenSI = endStr_ulCp(textStr);
   signed int discardSI = 0;

   if(! stateSC)
      stateSC = def_listCanSelect_rayWidg;

   if(listSTPtr->lenSI >= listSTPtr->sizeSI)
   { /*If: need more room*/
      if( realloc_listBox_rayWidg(0, listSTPtr) )
         goto memErr_fun00h;
   } /*If: need more room*/

   if(lenSI >= listSTPtr->sizeArySI[listSTPtr->lenSI])
   { /*If: need memory to store the string*/
      if(listSTPtr->textAryStr[listSTPtr->lenSI])
         free(listSTPtr->textAryStr[listSTPtr->lenSI]);
      listSTPtr->textAryStr[listSTPtr->lenSI] = 0;
 
      listSTPtr->textAryStr[listSTPtr->lenSI] =
         malloc( (lenSI + 8) * sizeof(signed char));
      if(! listSTPtr->textAryStr[listSTPtr->lenSI])
         goto memErr_fun00h;
      listSTPtr->sizeArySI[listSTPtr->lenSI] = lenSI;
   } /*If: need memory to store the string*/

   cpLen_ulCp(
      listSTPtr->textAryStr[listSTPtr->lenSI],
      textStr,
      lenSI
   );
   if(lenSI > listSTPtr->lineLenSI)
      listSTPtr->lineLenSI = lenSI;

   listSTPtr->stateArySC[listSTPtr->lenSI] = stateSC;

   lenSI =
      textMeasure_widg_rayWidg(
         listSTPtr->textAryStr[listSTPtr->lenSI],
         &discardSI, /*do not care about height*/
         widgSTPtr
      );

   lenSI += widgSTPtr->fontWidthF * 4;
     /*times 4 because my measure text does a times 4*/

   if(lenSI > listSTPtr->lineWidthSI)
      listSTPtr->lineWidthSI = lenSI;
   ++listSTPtr->lenSI;
   return 0;

   memErr_fun00h:;
      return 1;
} /*addItem_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun113: nextItem_listBox_rayWidg
|   - move to the next item or next item(s) in a listbox
| Input:
|   - changeSI:
|     o how many items to move up or down by
|       * + is move down
|       * - is move up
|   - maxItemsSI:
|     o maximum number of items to display
|   - onSI:
|     o item to start count from
|       * -1 means use and update onSI in listSTPtr
|       * >= 0 means do not modify or use onSI in
|         listSTPtr
|         @ for thread safty, this still uses the
|           variables in listSTPtr, but it resets them
|           back at the end
|   - listSTPtr:
|     o listBox_rayWidg struct pointer with items
| Output:
|   - Modifies:
|     o onSI in listSTPtr to be on the input target
|     o lastScrollSI in listSTPtr in listSTPtr to have the
|       number of dispalyed items moved down
|     o scrollSI to be advanced if lastScrollSI said
|       user moved out of the window
|   - Returns:
|     o -1 if did not move
|     o index moved to
\-------------------------------------------------------*/
signed int
nextItem_listBox_rayWidg(
   signed int changeSI,     /*how many items to move*/
   signed int maxItemsSI,   /*maximum items in window*/
   signed int onSI,         /*use -1 to do real change*/
   struct listBox_rayWidg *listSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun113: TOC
   '   - move to next item or next item(s) in a listbox
   '   o fun113 sec01:
   '     - variable declrations
   '   o fun113 sec02:
   '     - move down list box (+ change)
   '   o fun113 sec03:
   '     - move up list box (negative change)
   '   o fun113 sec04:
   '     - no change
   '   o fun113 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun113 Sec01:
   ^   - variable declrations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int cntSI = 0;
   signed int tmpSI = 0;
   signed int lastSI = 0;

   signed int oldScrollSI = listSTPtr->scrollSI;
   signed int oldLastSI = listSTPtr->lastScrollSI;
   signed int oldOnSI = listSTPtr->onSI;

   if(onSI >= 0)
      listSTPtr->onSI = onSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun113 Sec02:
   ^   - move down list box (+ change)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(changeSI > 0)
   { /*If: moving down*/
      tmpSI = listSTPtr->onSI + 1;
      cntSI = 0;

      if(listSTPtr->onSI >= listSTPtr->lenSI)
         goto moveRet_fun113_sec05;

      while(tmpSI < listSTPtr->lenSI && cntSI < changeSI)
      { /*Loop: find ending file*/
         if( hiddenGet_listBox_rayWidg(tmpSI, listSTPtr) )
            ;

         else
         { /*Else: file can be moved to*/
            ++cntSI;
            listSTPtr->onSI = tmpSI;

            ++listSTPtr->lastScrollSI;

            if(listSTPtr->lastScrollSI >= maxItemsSI)
            { /*If: need to move scroll down one item*/
               --listSTPtr->lastScrollSI;
               lastSI = listSTPtr->scrollSI;
               ++listSTPtr->scrollSI;

               if(listSTPtr->scrollSI >= listSTPtr->lenSI)
               { /*If: went past end of list*/
                  --listSTPtr->scrollSI;
                  goto moveRet_fun113_sec05;
               } /*If: went past end of list*/

               while(
                 hiddenGet_listBox_rayWidg(
                    listSTPtr->scrollSI,
                    listSTPtr
                 )
               ){ /*Loop: find next scroll position*/
                 ++listSTPtr->scrollSI;

                  if(
                   listSTPtr->scrollSI >= listSTPtr->lenSI
                  ){ /*If: at end of list*/
                     listSTPtr->scrollSI = lastSI;
                     goto moveRet_fun113_sec05;
                  }  /*If: at end of list*/
               }  /*Loop: find next scroll position*/
            }  /*If: need to move scroll down one item*/
         } /*Else: file can be moved to*/

         ++tmpSI;
      } /*Loop: find ending file*/

      if(! cntSI)
         goto noMoveRet_fun113_sec05;
         /*on last visible item*/
      else
         goto moveRet_fun113_sec05;
   } /*If: moving down*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun113 Sec03:
   ^   - move up list box (negative change)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(changeSI < 0)
   { /*Else If: moving down*/
      if(listSTPtr->onSI <= 0)
         goto moveRet_fun113_sec05;

      tmpSI = listSTPtr->onSI - 1;
      cntSI = 0;
      changeSI *= -1;

      while(tmpSI >= 0 && cntSI < changeSI)
      { /*Loop: find ending file*/
         if( hiddenGet_listBox_rayWidg(tmpSI, listSTPtr) )
            ;

         else
         { /*Else: file can be moved to*/
            --listSTPtr->lastScrollSI;
            ++cntSI;
            listSTPtr->onSI = tmpSI;

            if(listSTPtr->lastScrollSI < 0)
            { /*If: need to move scroll one item up*/
               ++listSTPtr->lastScrollSI;
               lastSI = listSTPtr->scrollSI;
               --listSTPtr->scrollSI;


               if(listSTPtr->scrollSI < 0)
               { /*If: went past end of list*/
                  ++listSTPtr->scrollSI;
                  goto moveRet_fun113_sec05;
               } /*If: went past end of list*/


               while(
                 hiddenGet_listBox_rayWidg(
                    listSTPtr->scrollSI,
                    listSTPtr
                 )
               ){ /*Loop: find next scroll position*/
                  --listSTPtr->scrollSI;

                  if(listSTPtr->scrollSI < 0)
                  { /*If: at end of list*/
                     listSTPtr->scrollSI = lastSI;
                     goto moveRet_fun113_sec05;
                  }  /*If: at end of list*/
               }  /*Loop: find next scroll position*/
            } /*If: need to move scroll one item up*/
         } /*Else: file can be moved to*/

         --tmpSI;
      } /*Loop: find ending file*/

      if(! cntSI)
         goto noMoveRet_fun113_sec05;
         /*on first visible item*/
      else
         goto moveRet_fun113_sec05;
   } /*Else If: moving down*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun113 Sec04:
   ^   - no change
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
      goto noMoveRet_fun113_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun113 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    moveRet_fun113_sec05:;
       tmpSI = listSTPtr->onSI;
       goto ret_fun113_sec05;

    noMoveRet_fun113_sec05:;
       return -1;

    ret_fun113_sec05:;
       if(onSI >= 0)
       { /*If: need to reset list box values*/
          if(tmpSI >= 0)
             tmpSI = listSTPtr->onSI;

          listSTPtr->scrollSI = oldScrollSI;
          listSTPtr->lastScrollSI = oldLastSI;
          listSTPtr->onSI = oldOnSI;
       } /*If: need to reset list box values*/

       return tmpSI;
} /*nextItem_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun114: toIndex_listBox_rayWidg
|   - move list box selected item till at target index
| Input:
|   - indexSI:
|     o index to move to
|   - maxItemsSI:
|     o maximum number of items to display
|   - listSTPtr:
|     o listBox_rayWidg struct pointer with items
| Output:
|   - Modifies:
|     o onSI in listSTPtr to be on the input target
|     o lastScrollSI in listSTPtr in listSTPtr to have the
|       number of dispalyed items moved down
|     o scrollSI to be advanced if lastScrollSI said
|       user moved out of the window
\-------------------------------------------------------*/
void
toIndex_listBox_rayWidg(
   signed int indexSI,     /*index to move to*/
   signed int maxItemsSI,  /*maximum items in window*/
   struct listBox_rayWidg *listSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun114: TOC
   '   - move list box selected item till at target index
   '   o fun114 sec01:
   '     - variable declrations
   '   o fun114 sec02:
   '     - move down list box (+ change)
   '   o fun114 sec03:
   '     - move up list box (negative change)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun114 Sec01:
   ^   - variable declrations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0;
   signed int lastSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun114 Sec02:
   ^   - move down list box (+ change)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSI > listSTPtr->onSI)
   { /*If: moving down*/
      tmpSI = listSTPtr->onSI + 1;

      while(tmpSI < listSTPtr->lenSI && tmpSI <= indexSI)
      { /*Loop: find ending file*/
         if( hiddenGet_listBox_rayWidg(tmpSI, listSTPtr) )
            ;

         else
         { /*Else: file can be moved to*/
            listSTPtr->onSI = tmpSI;
            ++listSTPtr->lastScrollSI;

            if(listSTPtr->lastScrollSI >= maxItemsSI)
            { /*If: need to move scroll down one item*/
               --listSTPtr->lastScrollSI;
               lastSI = listSTPtr->scrollSI;
               ++listSTPtr->scrollSI;

               if(listSTPtr->scrollSI >= listSTPtr->lenSI)
               { /*If: went past end of list*/
                  --listSTPtr->scrollSI;
                  return;
               } /*If: went past end of list*/

               while(
                 hiddenGet_listBox_rayWidg(
                    listSTPtr->scrollSI,
                    listSTPtr
                 )
               ){ /*Loop: find next scroll position*/
                 ++listSTPtr->scrollSI;

                  if(
                   listSTPtr->scrollSI >= listSTPtr->lenSI
                  ){ /*If: at end of list*/
                     listSTPtr->scrollSI = lastSI;
                     return;
                  }  /*If: at end of list*/
               }  /*Loop: find next scroll position*/
            }  /*If: need to move scroll down one item*/
         } /*Else: file can be moved to*/

         ++tmpSI;
      } /*Loop: find ending file*/
   } /*If: moving down*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun114 Sec03:
   ^   - move up list box (negative change)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(indexSI < listSTPtr->onSI)
   { /*Else If: moving down*/
      tmpSI = listSTPtr->onSI - 1;

      while(tmpSI >= 0 && tmpSI >= indexSI)
      { /*Loop: find ending file*/
         if( hiddenGet_listBox_rayWidg(tmpSI, listSTPtr) )
            ;

         else
         { /*Else: file can be moved to*/
            --listSTPtr->lastScrollSI;
            listSTPtr->onSI = tmpSI;

            if(listSTPtr->lastScrollSI < 0)
            { /*If: need to move scroll one item up*/
               ++listSTPtr->lastScrollSI;
               lastSI = listSTPtr->scrollSI;
               --listSTPtr->scrollSI;


               if(listSTPtr->scrollSI < 0)
               { /*If: went past end of list*/
                  ++listSTPtr->scrollSI;
                  return;
               } /*If: went past end of list*/


               while(
                 hiddenGet_listBox_rayWidg(
                    listSTPtr->scrollSI,
                    listSTPtr
                 )
               ){ /*Loop: find next scroll position*/
                  --listSTPtr->scrollSI;

                  if(listSTPtr->scrollSI < 0)
                  { /*If: at end of list*/
                     listSTPtr->scrollSI = lastSI;
                     return;
                  }  /*If: at end of list*/
               }  /*Loop: find next scroll position*/
            } /*If: need to move scroll one item up*/
         } /*Else: file can be moved to*/

         --tmpSI;
      } /*Loop: find ending file*/
   } /*Else If: moving down*/
} /*toIndex_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun115: draw_listBox_rayWidg
|   - draws a list box widget
| Input:
|   - idSI:
|     o id of the parent list box widget
|     o return value from mk_listBox_rayWidg()
|   - noDrawBl:
|     o 1: find dimensions, but do not draw the list box
|     o 0: find dimensions and draw the list box
|   - listSTPtr:
|     o listBox_rayWidg struct pointer with list box
|       attributes
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with list box
| Output:
|   - Modifies:
|     o widthArySI in widgSTPtr to have current list box
|       width
|     o heightArySI in widgSTPtr to have current list box
|       height
|   - Draws:
|     o list box in GUI
|   - Returns:
|     o width of list box for success
|     o 0 for list box is hidden
|     o def_noWidget_rayWidg if could not find list box
\-------------------------------------------------------*/
signed int
draw_listBox_rayWidg(
   signed int idSI, /*id of parent widget*/
   signed char noDrawBl, /*1: just find dimensions*/
   struct listBox_rayWidg *listSTPtr, /*has list box*/
   struct widg_rayWidg *widgSTPtr /*has widgets*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun115 TOC:
   '   - draws a list box widget
   '   o fun115 sec01:
   '     - variable declarations
   '   o fun115 sec02:
   '     - check if can draw and get color scheme
   '   o fun115 sec03:
   '     - get dimensions
   '   o fun115 sec04:
   '     - draw the list box
   '   o fun115 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun115 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = 0;
   unsigned int selectColUI = 0;

   signed int siLine = 0;   /*line on*/
   signed int maxLinesSI = 0;
   signed int maxCharInLineSI = 0;
   signed int sidePosSI = 0; /*side scroll position*/

   signed int itemSI = 0;   /*item on in list box*/
   signed int ySI = 0;      /*y position to draw item*/

   #define lenStr_fun115 512 /*max c-string length*/
   signed char textStr[lenStr_fun115 + 8];
   signed char tmpTextStr[lenStr_fun115 + 8];
   signed char selectBl = 0;
   unsigned int onColUI = 0;
   unsigned int onBorderUI = 0;

   signed char typeSC = 0;

   struct Color textCol;
   struct Color selectTextCol;
   struct Color *textPickColPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun115 Sec02:
   ^   - check if can draw and get color scheme
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun115_sec0x;

   else if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hidenWidget_fun115_sec0x;

   else if( inactiveGet_widg_rayWidg(idSI, widgSTPtr) )
   { /*Else If: list box is in an inactive state*/
       typeSC = 0;

       colorUI = widgSTPtr->guiColSI;
       borderColorUI = widgSTPtr->borderColSI;
       textCol = GetColor(widgSTPtr->textColSI);

       selectColUI = widgSTPtr->textAltColSI;
       selectTextCol = GetColor(widgSTPtr->guiColSI);

       focusColorUI = widgSTPtr->textAltColSI;
   } /*Else If: list box is in an inactive state*/

   else if(
         widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg
      || widgSTPtr->stateAryUS[idSI] & def_active_rayWidg
   ){ /*Else If: user is unsing list box*/
       typeSC = 0;

       colorUI = widgSTPtr->activeColSI;
       borderColorUI = widgSTPtr->borderColSI;
       textCol = GetColor(widgSTPtr->textAltColSI);

       selectColUI = widgSTPtr->guiColSI;
       selectTextCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: user is unsing list box*/

   else
   { /*Else: not working with list box*/
       typeSC = 1;

       colorUI = widgSTPtr->guiColSI;
       borderColorUI = widgSTPtr->borderColSI;
       textCol = GetColor(widgSTPtr->textColSI);

       selectColUI = widgSTPtr->activeColSI;
       selectTextCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else: not working with list box*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun115 Sec03:
   ^   - get dimensions
   ^   o fun115 sec03 sub01:
   ^     - find the width of the longest line
   ^   o fun115 sec03 sub02:
   ^     - find max lines and characters per line; to show
   ^   o fun115 sec03 sub03:
   ^     - find width and height of list box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun115 Sec03 Sub01:
   *   - find the width of the longest line
   \*****************************************************/

   if(
         (signed int) (listSTPtr->lastScaleF * 100)
      != (signed int) (widgSTPtr->xScaleF * 100)
   ){ /*If: need to reget the longest lines width*/
      listSTPtr->lineWidthSI = 0;

      for(itemSI=0; itemSI < listSTPtr->lenSI; ++itemSI)
      { /*Loop: remeasure each line*/
         siLine =
            textMeasure_widg_rayWidg(
               listSTPtr->textAryStr[itemSI],
               &sidePosSI,
               widgSTPtr
            );

         if(siLine > listSTPtr->lineWidthSI)
            listSTPtr->lineWidthSI = siLine;
      } /*Loop: remeasure each line*/

      itemSI = 0;
      siLine = 0;
      sidePosSI = 0;
   }  /*If: need to reget the longest lines width*/

   /*****************************************************\
   * Fun115 Sec03 Sub02:
   *   - find max lines and characters per line (to show)
   \*****************************************************/

   maxCharInLineSI =
      (
           widgSTPtr->widthArySI[idSI]
         - widgSTPtr->fontWidthF * 4
      ) / widgSTPtr->fontWidthF;
      /*max number of characters in one list box line*/

   maxLinesSI =
     getLines_rayWidg(
        listSTPtr->maxHeightSI,
        widgSTPtr
     );
   itemSI =
      getLines_rayWidg(
        listSTPtr->minHeightSI,
        widgSTPtr
     ); /*minimum number of lines (resuing itemSI)*/

   /*****************************************************\
   * Fun115 Sec03 Sub03:
   *   - find width and height of list box
   \*****************************************************/

   /*find the height of the list box*/
   if(listSTPtr->lenSI < 0)
      maxLinesSI = itemSI; /*no minimum input*/
   else if(maxLinesSI > listSTPtr->lenSI)
      maxLinesSI = listSTPtr->lenSI;

   if(maxLinesSI < itemSI)
      maxLinesSI = itemSI; /*beneath min line count*/

   widgSTPtr->heightArySI[idSI] =
      linesToHeight_rayWidg(maxLinesSI, widgSTPtr);
   widgSTPtr->heightArySI[idSI] +=
      widgSTPtr->fontHeightF / def_widgHeightGap_rayWidg;

   /*find the width of the list box*/
   itemSI =
        listSTPtr->lineWidthSI
      + widgSTPtr->fontWidthF * 3; /**3 for # and >*/

   if(listSTPtr->maxWidthSI <= 0)
      widgSTPtr->widthArySI[idSI] = itemSI;
   else if(itemSI > listSTPtr->maxWidthSI)
      widgSTPtr->widthArySI[idSI] = listSTPtr->maxWidthSI;
   else if(itemSI < listSTPtr->minWidthSI)
      widgSTPtr->widthArySI[idSI] = listSTPtr->minWidthSI;
   else
      widgSTPtr->widthArySI[idSI] = itemSI;

   if(noDrawBl)
      goto noDraw_fun115_sec0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun115 Sec04:
   ^   - draw the list box
   ^   o fun115 sec04 sub01:
   ^     - draw rectangle for list box
   ^   o fun115 sec04 sub02:
   ^     - draw each item currently shown in list box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun115 Sec04 Sub01:
   *   - draw rectangle for list box
   \*****************************************************/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI,
      typeSC,
      colorUI,
      borderColorUI,
      focusColorUI
   ); /*rectangle for widget*/

   /*****************************************************\
   * Fun115 Sec04 Sub02:
   *   - draw each item currently shown in list box
   *   o fun115 sec04 sub02 cat01:
   *     - setup and detect if can draw item (not hidden)
   *   o fun115 sec04 sub02 cat02:
   *     - check if item was selected
   *   o fun115 sec04 sub02 cat03:
   *     - check if user is on item
   *   o fun115 sec04 sub02 cat04:
   *     - draw item
   *   o fun115 sec04 sub02 cat05:
   *     - move to next item
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun115 Sec04 Sub02 Cat01:
   +   - setup and detect if can draw item (not hidden)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   siLine = 0;
   itemSI = listSTPtr->scrollSI;
   ySI = widgSTPtr->yArySI[idSI];

   while(siLine < maxLinesSI)
   { /*Loop: draw the labels*/
      if(itemSI >= listSTPtr->lenSI)
         break; /*no more items to draw*/

      selectBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun115 Sec04 Sub02 Cat02:
      +   - check if item was selected
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if( hiddenGet_listBox_rayWidg(itemSI, listSTPtr) )
          ; /*item is hiden; nothing to do*/

      else
      { /*Else: I can draw the item*/
         if(selectGet_listBox_rayWidg(itemSI, listSTPtr))
         { /*If: item was selected*/
            rectDrawByCoord_rayWidg(
               (widgSTPtr->xArySI[idSI] + 1)
                  + widgSTPtr->fontWidthF,
               ySI + widgSTPtr->fontHeightF
                  / (def_widgHeightGap_rayWidg / 1.25),
               widgSTPtr->widthArySI[idSI] - 2
                  - widgSTPtr->fontWidthF * 1.5,
               widgSTPtr->fontHeightF
                  - (widgSTPtr->fontHeightF * 0.1),
                  /*color half of gap with select color*/
               0, /*just rectangle, no borders*/
               selectColUI,
               selectColUI, /*border color*/
               selectColUI, /*no focus color*/
               widgSTPtr
            );

            selectBl = 1;
            onColUI = selectColUI;
            textPickColPtr = &selectTextCol;
            onBorderUI = colorUI;
            typeSC = 3;
         } /*If: item was selected*/

         else
         { /*Else: item not selected*/
            onBorderUI = selectColUI;
            textPickColPtr = &textCol;
            onColUI = colorUI;
            typeSC = 1;
         } /*Else: item not selected*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun115 Sec04 Sub02 Cat03:
         +   - check if user is on item
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(listSTPtr->onSI == itemSI)
         { /*If: need to draw box around item on*/
            rectDrawByCoord_rayWidg(
               widgSTPtr->xArySI[idSI]
                  + widgSTPtr->fontWidthF,
               ySI + widgSTPtr->fontHeightF
                  / (def_widgHeightGap_rayWidg / 1.25),
               widgSTPtr->widthArySI[idSI]
                  - widgSTPtr->fontWidthF * 1.5,
               widgSTPtr->fontHeightF
                  - (widgSTPtr->fontHeightF * 0.1),
                  /*color half of gap with select color*/
               typeSC, /*both border and rectangle*/
               onColUI,  /*make same color as background*/
               onBorderUI, /*border color*/
               onColUI,  /*outer border color*/
               widgSTPtr
            );
         } /*If: need to draw box around item on*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun115 Sec04 Sub02 Cat04:
         +   - draw item
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         sidePosSI =
            endStr_ulCp(listSTPtr->textAryStr[itemSI]);

         if(sidePosSI < listSTPtr->sideScrollSI)
         { /*If: need to find the correct items*/
            if(sidePosSI < maxCharInLineSI)
               sidePosSI = 0;
               /*item fully shown in list box*/
            else
               sidePosSI -= maxCharInLineSI;
         } /*If: need to find the correct items*/

         else
            sidePosSI = listSTPtr->sideScrollSI;

         cpStr_ulCp(
            &tmpTextStr[1], 
            &listSTPtr->textAryStr[itemSI][sidePosSI]
         );

         if(listSTPtr->onSI == itemSI && ! selectBl)
            tmpTextStr[0] = '>'; /*mark item on*/
         else if(listSTPtr->onSI == itemSI && selectBl)
            tmpTextStr[0] = '#'; /*mark selection is on*/
         else if(selectBl)
            tmpTextStr[0] = '+'; /*mark on an selected*/
         else
            tmpTextStr[0] = ' ';


         /*get text to draw*/
         checkTextWidth_rayWidg(
           tmpTextStr,
           textStr,
           widgSTPtr->widthArySI[idSI]
              - widgSTPtr->fontWidthF * 2,
           widgSTPtr
         );

         DrawTextEx(
            *widgSTPtr->fontSTPtr,
            (char *) textStr,
            (Vector2){
               widgSTPtr->xArySI[idSI] +
                 widgSTPtr->fontWidthF,
                 /*center text in x*/
               ySI
                  + widgSTPtr->fontHeightF
                  / def_widgHeightGap_rayWidg
                  /*center text on y*/
            }, /*dimensions*/
            widgSTPtr->fontSizeSI,
            widgSTPtr->spacingF,
            *textPickColPtr
         ); /*draw the item*/

         ySI += widgSTPtr->fontHeightF;
         ++siLine;
      } /*Else: I can draw the item*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun115 Sec04 Sub02 Cat05:
      +   - move to next item
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++itemSI;
   } /*Loop: draw the labels*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun115 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgSTPtr->widthArySI[idSI];

   noDraw_fun115_sec0x:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun115_sec0x:;
      return def_noWidget_rayWidg;

   hidenWidget_fun115_sec0x:;
      return 0;
} /*draw_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun116: listBoxEvent_rayWidg
|   - runs an event for a list box
| Input:
|   - idSI:
|     o id of listbox, this is used to check if the
|       listbox was really selected (avoids ifs in
|       your code)
|   - listSTPtr:
|     o listBox_rayWidg struct pointer with text (items)
|       to print out and other list box needed tracking
|   - eventSTPtr:
|     o event_rayWidg structure pointer with events user
|       input
|   - widgSTPtr:
|     o widg_rayWidg pointer with target widget
| Output:
|   - Modifies:
|     o selectArySI to have the next selected item, if
|       the user selected an item
|   - Returns:
|     o -2 if no widget in widgSTPtr was selected
|     o -1 if list box was not selected
|     o 0 for no event
|     o 1 for movement event
|     o 2 for selection event
|     o 4 for enter/double click event
\-------------------------------------------------------*/
signed int
listBoxEvent_rayWidg(
   signed int idSI,                  /*id of list box*/
   struct listBox_rayWidg *listSTPtr,/*list box data*/
   struct event_rayWidg *eventSTPtr, /*events user did*/
   struct widg_rayWidg *widgSTPtr    /*widgets user made*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun116 TOC:
   '   - runs an event for a list box
   '   o fun116 sec01:
   '     - variable declarations
   '   o fun116 sec02:
   '     - check if have event + find select/display limit
   '   o fun116 sec03:
   '     - deal with keyboard input
   '   o fun116 sec04:
   '     - deal with scroll wheel events
   '   o fun116 sec05:
   '     - deal with mouse clicks
   '   o fun116 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int maxLinesSI = 0;   /*maximum lines to show*/
   signed int maxSelectSI = listSTPtr->maxSelectSI;

   signed int changeSI = 0; /*for up/down key movements*/
   signed int cntSI = 0;    /*for up/down key movements*/
   signed int tmpSI = 0;

   signed int ySI = 0;      /*for getting item click on*/
   signed int lastSelectSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec02:
   ^   - check if have event + find select/display limits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(eventSTPtr->idSI != idSI)
      goto notListBox_fun116_sec06;
      /*selected widget was not a list box*/

   else if(eventSTPtr->idSI < 0)
      goto noWidget_fun116_sec06;
      /*no widget was selected*/
      
   else if(
      hidenGet_widg_rayWidg(eventSTPtr->idSI, widgSTPtr)
   ) goto hidenWidget_fun116_sec06;

   else if(
      inactiveGet_widg_rayWidg(eventSTPtr->idSI,widgSTPtr)
   ) goto inactiveWidget_fun116_sec06;

   /*find max nubmer of files to select (select limit)*/
   if(maxSelectSI < 0)
      maxSelectSI = listSTPtr->lenSI;

   /*find maximum number of lines that can be shown*/
   maxLinesSI =
      getLines_rayWidg(
         widgSTPtr->heightArySI[eventSTPtr->idSI],
         widgSTPtr
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec03:
   ^   - deal with keyboard input
   ^   o fun116 sec03 sub01:
   ^     - move down + start keyboard event if
   ^   o fun116 sec03 sub02:
   ^     - move up + start keyboard event if
   ^   o fun116 sec03 sub03:
   ^     - move left
   ^   o fun116 sec03 sub04:
   ^     - move right
   ^   o fun116 sec03 sub05:
   ^     - move to start or end of list
   ^   o fun116 sec03 sub06:
   ^     - enter key select item or trigger event
   ^   o fun116 sec03 sub07:
   ^     - space key (add selection items)
   ^   o fun116 sec03 sub08:
   ^     - ctrl-a if user wanted everything
   ^   o fun116 sec03 sub09:
   ^     - ctrl-r delesect (refresh)
   ^   o fun116 sec03 sub10:
   ^     - unknown key or no key event
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun116 Sec03 Sub01:
   *   - move down + start keyboard event if
   \*****************************************************/

   switch(eventSTPtr->keySI)
   { /*If: had a key board event*/
      case KEY_J:
      case KEY_DOWN:
      case KEY_PAGE_DOWN:
      /*Case: moving down one item or page*/
         moveDown_fun116_sec03_sub01:;
            listSTPtr->clickSI = -1;

            if(
                  eventSTPtr->shiftBl
               || eventSTPtr->keySI == KEY_PAGE_DOWN
            ) changeSI = maxLinesSI;
            else
               changeSI = 1;


            if(
               nextItem_listBox_rayWidg(
                  changeSI,
                  maxLinesSI,
                  -1, /*use internal variables*/
                  listSTPtr
               ) < 0
            ) goto moveEvent_fun116_sec06;

            else
               goto noEvent_fun116_sec06;
      /*Case: moving down one item or page*/

      /**************************************************\
      * Fun116 Sec03 Sub02:
      *   - move up
      \**************************************************/

      case KEY_K:
      case KEY_UP:
      case KEY_PAGE_UP:
      /*Case: moving up one item or page*/
         moveUp_fun116_sec03_sub02:;
            listSTPtr->clickSI = -1;

            if(
                  eventSTPtr->shiftBl
               || eventSTPtr->keySI == KEY_PAGE_UP
            ) changeSI = maxLinesSI * -1;
            else
               changeSI = -1;


            if(
               nextItem_listBox_rayWidg(
                  changeSI,
                  maxLinesSI,
                  -1, /*use internal variables*/
                  listSTPtr
               ) < 0
            ) goto moveEvent_fun116_sec06;

            else
               goto noEvent_fun116_sec06;
      /*Case: moving up one item or page*/

      /**************************************************\
      * Fun116 Sec03 Sub03:
      *   - move left
      \**************************************************/

      case KEY_H:
      case KEY_LEFT:
      /*Case: moving left*/
         moveLeft_fun116_sec03_sub03:;
            listSTPtr->clickSI = -1;

            if(eventSTPtr->shiftBl)
            { /*If: jumping by width*/
               listSTPtr->sideScrollSI -=
                  (
                       widgSTPtr->widthArySI[idSI]
                     - widgSTPtr->fontWidthF * 2
                  ) / widgSTPtr->fontWidthF;
            } /*If: jumping by width*/

            else
               listSTPtr->sideScrollSI -= 1;

            if(listSTPtr->sideScrollSI < 0)
               listSTPtr->sideScrollSI = 0;
            goto sideMoveEvent_fun116_sec06;
      /*Case: moving left*/

      case KEY_S:
      case KEY_ZERO:
      case KEY_SIX: /*with shift is ^*/
      /*Case: moving to start*/
         listSTPtr->clickSI = -1;

         if(
               eventSTPtr->keySI == KEY_SIX
            && ! eventSTPtr->shiftBl
         ) goto noEvent_fun116_sec06;

         listSTPtr->sideScrollSI = 0;
         goto sideMoveEvent_fun116_sec06;
      /*Case: moving to start*/

      /**************************************************\
      * Fun116 Sec03 Sub04:
      *   - move right
      \**************************************************/

      case KEY_L:
      case KEY_RIGHT:
      /*Case: moving right*/
         moveRight_fun116_sec03_sub04:;
            listSTPtr->clickSI = -1;

            tmpSI = listSTPtr->lineLenSI;
            tmpSI -=
               (
                    widgSTPtr->widthArySI[idSI]
                  - widgSTPtr->fontWidthF * 8
               ) / widgSTPtr->fontWidthF;
               /*last full window*/

            if(eventSTPtr->shiftBl)
            { /*If: jumping by width*/
               listSTPtr->sideScrollSI +=
                  (
                       widgSTPtr->widthArySI[idSI]
                     - widgSTPtr->fontWidthF * 2
                  ) / widgSTPtr->fontWidthF;
            } /*If: jumping by width*/

            else
               listSTPtr->sideScrollSI += 1;

            if(listSTPtr->sideScrollSI > tmpSI)
               listSTPtr->sideScrollSI = tmpSI;

            goto sideMoveEvent_fun116_sec06;
      /*Case: moving right*/

      case KEY_E:
      case KEY_FOUR: /*with shift is $*/
      /*Case: moving to start*/
         listSTPtr->clickSI = -1;

         if(
               eventSTPtr->keySI == KEY_FOUR
            && ! eventSTPtr->shiftBl
         ) goto noEvent_fun116_sec06;

         listSTPtr->sideScrollSI = listSTPtr->lineLenSI;
         listSTPtr->sideScrollSI -=
            (
                 widgSTPtr->widthArySI[idSI]
               - widgSTPtr->fontWidthF * 8
            ) / widgSTPtr->fontWidthF;
            /*account for number of characters in a
            `  screen
            */

         goto sideMoveEvent_fun116_sec06;
      /*Case: moving to start*/

      /**************************************************\
      * Fun116 Sec03 Sub05:
      *   - move to start or end of list
      \**************************************************/

      case KEY_HOME:
      case KEY_END:
      case KEY_G:
      /*Case: move to start or end of list*/
         listSTPtr->clickSI = -1;

         if(
               eventSTPtr->keySI == KEY_HOME
            || (
                     ! eventSTPtr->shiftBl
                  && eventSTPtr->keySI == KEY_G
               )
         ) changeSI = listSTPtr->onSI * -1;/*go to start*/

         else
            changeSI = listSTPtr->lenSI - listSTPtr->onSI;
            /*go to end*/

         nextItem_listBox_rayWidg(
            changeSI,
            maxLinesSI,
            -1, /*use internal variables*/
            listSTPtr
         );

         goto moveEvent_fun116_sec06;
      /*Case: move to start or end of list*/

      /**************************************************\
      * Fun116 Sec03 Sub06:
      *   - enter key select item or trigger event
      \**************************************************/

      case KEY_ENTER:
      /*Case: enter key was hit*/
         listSTPtr->clickSI = -1;

         clearSelect_listBox_rayWidg(listSTPtr);

         if(
              listSTPtr->stateArySC[listSTPtr->onSI]
            & def_listCanSelect_rayWidg
         ) listSTPtr->stateArySC[listSTPtr->onSI] |= 
               def_listSelect_rayWidg;
               /*user can select this item*/

         if(
              listSTPtr->stateArySC[listSTPtr->onSI]
            & def_listSpecial_rayWidg
         ) goto specialSelectEvent_fun116_sec06;
           /*user needs to handel this one*/

         else
            goto selectEvent_fun116_sec06;
      /*Case: enter key was hit*/

      /**************************************************\
      * Fun116 Sec03 Sub07:
      *   - space key (add selection items)
      *   o fun116 sec03 sub07 cat01:
      *     - set selection to only one item + case
      *   o fun116 sec03 sub07 cat02:
      *     - add or remove item from selection
      *   o fun116 sec03 sub07 cat03:
      *     - add range to selection
      *   o fun116 sec03 sub07 cat04:
      *     - go to selection return
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun116 Sec03 Sub07 Cat01:
      +   - set selection to only one item + case
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case KEY_SPACE:
      /*Case: space key was hit*/
         listSTPtr->clickSI = -1;

         if(! eventSTPtr->shiftBl && ! eventSTPtr->ctrlBl)
         { /*If: reseting selection to the current item*/
            clearSelect_listBox_rayWidg(listSTPtr);
            /*selecting one file; so clear old files*/

            if(
                 listSTPtr->stateArySC[listSTPtr->onSI]
               & def_listCanSelect_rayWidg
            ){ /*If: can select the item*/
               listSTPtr->numSelectSI = 1;
               listSTPtr->stateArySC[listSTPtr->onSI] |=
                   def_listSelect_rayWidg;
            }  /*If: can select the item*/

            listSTPtr->lastSelectSI = listSTPtr->onSI;
         } /*If: reseting selection to the current item*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun116 Sec03 Sub07 Cat02:
         +   - add or remove item from selection
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(eventSTPtr->ctrlBl)
         { /*Else If: adding or removing item*/
            if(
                 listSTPtr->stateArySC[listSTPtr->onSI]
               & def_listCanSelect_rayWidg
            ){ /*If: can select the item*/
               if(
                    listSTPtr->stateArySC[listSTPtr->onSI]
                  & def_listSelect_rayWidg
               ){ /*If: deselecting the item*/
                  if(
                        listSTPtr->lastSelectSI
                     == listSTPtr->onSI
                  ) listSTPtr->lastSelectSI = -1;

                  --listSTPtr->numSelectSI;
                  listSTPtr->stateArySC[listSTPtr->onSI]^=
                     def_listSelect_rayWidg;
               }  /*If: deselecting the item*/

               else if(
                  listSTPtr->numSelectSI < maxSelectSI
               ){ /*Else If: selecting the item*/
                 ++listSTPtr->numSelectSI;
                 listSTPtr->lastSelectSI =listSTPtr->onSI;

                 listSTPtr->stateArySC[listSTPtr->onSI] ^=
                     def_listSelect_rayWidg;
               }  /*Else If: selecting the item*/
            }  /*If: can select the item*/
         } /*Else If: adding or removing item*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun116 Sec03 Sub07 Cat03:
         +   - add range to selection
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: adding a range*/
            if(listSTPtr->lastSelectSI > listSTPtr->onSI)
            { /*If: moving down the list*/
               cntSI = listSTPtr->onSI;
               tmpSI = listSTPtr->lastSelectSI;
            } /*If: moving down the list*/

            else if(listSTPtr->lastSelectSI < 0)
            { /*Else If: nothing selected*/
               tmpSI = listSTPtr->onSI;
               cntSI = tmpSI;
            } /*Else If: nothing selected*/

            else
            { /*Else: moving up the list*/
               tmpSI = listSTPtr->onSI;
               cntSI = listSTPtr->lastSelectSI;
            } /*Else: moving up the list*/

            while(cntSI <= tmpSI)
            { /*Loop: select all files in range*/
               if( 
                    listSTPtr->stateArySC[cntSI]
                  & def_listHidden_rayWidg
               ) break;

               else if(
                  listSTPtr->numSelectSI >= maxSelectSI
               ) break; /*hit selection limit*/

               else if(
                    listSTPtr->stateArySC[cntSI]
                  & def_listCanSelect_rayWidg
               ){ /*Else If: adding item to selection*/
                  if(
                     selectGet_listBox_rayWidg(
                        cntSI,
                        listSTPtr
                     )
                  ) ; /*already selected*/

                  else
                  { /*Else: need to select*/
                     listSTPtr->stateArySC[cntSI] |=
                        def_listSelect_rayWidg;

                     listSTPtr->lastSelectSI = cntSI;
                     ++listSTPtr->numSelectSI;
                  } /*Else: need to select*/
               }  /*Else If: adding item to selection*/

               ++cntSI;
            } /*Loop: select all files in range*/
         } /*Else: adding a range*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun116 Sec03 Sub07 Cat04:
         +   - go to selection return
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         goto selectEvent_fun116_sec06;
      /*Case: space key was hit*/

      /**************************************************\
      * Fun116 Sec03 Sub08:
      *   - ctrl-a if user wanted everything
      \**************************************************/

      case KEY_A:
      /*Case: a key*/
         listSTPtr->clickSI = -1;

         if(eventSTPtr->ctrlBl)
            ;
         else
            goto noEvent_fun116_sec06;

         if(listSTPtr->numSelectSI >= maxSelectSI)
            goto noEvent_fun116_sec06;
            /*already selected everything possible*/

         tmpSI = 0;

         while(tmpSI < listSTPtr->lenSI)
         { /*Loop: select as  many files as possible*/
            if( 
                 listSTPtr->stateArySC[tmpSI]
               & def_listHidden_rayWidg
            ) ;

            else if(listSTPtr->numSelectSI >= maxSelectSI)
               break;

            else if(
               selectGet_listBox_rayWidg(
                  tmpSI,
                  listSTPtr
               )
            ) ; /*already selected this*/

            else if(
               canSelectGet_listBox_rayWidg(
                  tmpSI,
                  listSTPtr
               )
            ){ /*Else: file can be selected*/
               ++listSTPtr->numSelectSI;

               listSTPtr->stateArySC[tmpSI] |=
                  def_listSelect_rayWidg;
            }  /*Else: file can be selected*/

            ++tmpSI;
         } /*Loop: select as  many files as possible*/

         goto selectEvent_fun116_sec06;
      /*Case: a key*/

      /**************************************************\
      * Fun116 Sec03 Sub09:
      *   - ctrl-r delesect (refresh)
      \**************************************************/

      case KEY_R:
      /*Case: r key*/
         listSTPtr->clickSI = -1;

         if(eventSTPtr->ctrlBl)
         { /*If: clearing all selected files*/
            clearSelect_listBox_rayWidg(listSTPtr);
            goto selectEvent_fun116_sec06;
         } /*If: clearing all selected files*/

         else
            goto noEvent_fun116_sec06;
      /*Case: r key*/

      /**************************************************\
      * Fun116 Sec03 Sub10:
      *   - unknown key or no key event
      \**************************************************/

      case 0:
          goto checkWheelEvent_fun116_sec04_sub01;

      default:
          goto noEvent_fun116_sec06;
          /*unreconginzed key input*/
   } /*If: was a key event*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec04:
   ^   - deal with scroll wheel events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   checkWheelEvent_fun116_sec04_sub01:;

   cntSI = (signed int) eventSTPtr->scrollF;

   if(cntSI != 0)
   { /*If: moving by scroll wheel*/
      if(eventSTPtr->ctrlBl)
      { /*If: controll used with scroll*/
         if(cntSI > 0)
            goto moveLeft_fun116_sec03_sub03;
         else
            goto moveRight_fun116_sec03_sub04;
      } /*If: controll used with scroll*/

      else if(eventSTPtr->scrollF < 0)
         goto moveUp_fun116_sec03_sub02;
      else
         goto moveDown_fun116_sec03_sub01;
   } /*If: moving by scroll wheel*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec05:
   ^   - deal with mouse clicks
   ^   o fun116 sec05 sub01:
   ^     - find the clicked on item in list box
   ^   o fun116 sec05 sub02:
   ^     - deal with click event
   ^   o fun116 sec05 sub03:
   ^     - check for right clicks; event does not capture
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun116 Sec05 Sub01:
   *   - find the clicked on item in list box
   \*****************************************************/

   if(eventSTPtr->leftPressBl)
      listSTPtr->clickSI = -1;

   /*****************************************************\
   * Fun116 Sec05 Sub02:
   *   - deal with click release event
   *   o fun116 sec05 sub02 cat01:
   *     - left release; find change to get to item
   *   o fun116 sec05 sub02 cat01:
   *     - control + click; add/remove file from selection
   *   o fun116 sec05 sub02 cat03:
   *     - shift + click select a range
   *   o fun116 sec05 sub02 cat04:
   *     - single click; set item as only selected item
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun116 Sec05 Sub02 Cat01:
   +   - left release; find change to get to item
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(eventSTPtr->leftReleaseBl)
   { /*Else If: release event*/

      /*find how many items down from scroll positoin the
      `   user clicked
      */
      ySI = eventSTPtr->ySI;
      ySI -= widgSTPtr->yArySI[eventSTPtr->idSI];
      ySI = getLines_rayWidg(ySI, widgSTPtr);

      if(ySI + listSTPtr->scrollSI >= listSTPtr->lenSI)
      { /*If: empty space was clicked*/
         listSTPtr->clickSI = -1;
         goto noEvent_fun116_sec06;
      } /*If: empty space was clicked*/

      lastSelectSI = listSTPtr->onSI;
      if(ySI)
         ySI =
            nextItem_listBox_rayWidg(
               ySI,
               maxLinesSI,
               listSTPtr->scrollSI,
               listSTPtr
            );
      else
         ySI = listSTPtr->scrollSI;
         /*in this case, it will appear to move no items
         `  and nextItem returns -1 as a result,
         `  so this is needed to keep scroll in position
         */

      if(ySI >= 0)
      { /*If: chaning my position*/
         toIndex_listBox_rayWidg(
           ySI,
           maxLinesSI,
           listSTPtr
         );
      } /*If: chaning my position*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun116 Sec05 Sub02 Cat02:
      +   - control+click; add/remove file from selection
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(listSTPtr->onSI != listSTPtr->clickSI)
         listSTPtr->clickSI = -1;
         /*different item being clicked on*/

      if(eventSTPtr->ctrlBl)
      { /*If: user wanted to add item to selection list*/
         listSTPtr->clickSI = -1;

         if(
              listSTPtr->stateArySC[listSTPtr->onSI]
            & def_listCanSelect_rayWidg
         ){ /*If: can select the item*/
            if(
                 listSTPtr->stateArySC[listSTPtr->onSI]
               & def_listSelect_rayWidg
            ){ /*If: deselecting the item*/
               --listSTPtr->numSelectSI;
               listSTPtr->stateArySC[listSTPtr->onSI]^=
                  def_listSelect_rayWidg;

               if(
                     listSTPtr->lastSelectSI
                  == listSTPtr->onSI
               ) listSTPtr->lastSelectSI = -1;
            }  /*If: deselecting the item*/

            else if(
               listSTPtr->numSelectSI < maxSelectSI
            ){ /*Else If: selecting the item*/
               ++listSTPtr->numSelectSI;
               listSTPtr->lastSelectSI = lastSelectSI;

               listSTPtr->stateArySC[listSTPtr->onSI] ^=
                   def_listSelect_rayWidg;
            }  /*Else If: selecting the item*/
         }  /*If: can select the item*/

         goto selectEvent_fun116_sec06;
      } /*If: user wanted to add item to selection list*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun116 Sec05 Sub02 Cat03:
      +   - shift + click select a range
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(eventSTPtr->shiftBl)
      { /*Else If: selecting a range of items*/
         listSTPtr->clickSI = -1;

         if(listSTPtr->onSI > listSTPtr->lastSelectSI)
         { /*If: going fowards*/
            tmpSI = listSTPtr->lastSelectSI;
            cntSI = listSTPtr->onSI;
         } /*If: going fowards*/

         else
         { /*Else: going backwards*/
            cntSI = listSTPtr->lastSelectSI;
            tmpSI = listSTPtr->onSI;
         } /*Else: going backwards*/

         while(tmpSI <= cntSI)
         { /*Loop: add files to selection*/
            if( 
                 listSTPtr->stateArySC[tmpSI]
               & def_listHidden_rayWidg
            ) ;

            else if(
               listSTPtr->numSelectSI >= maxSelectSI
            ) break; /*hit selection limit*/

            else if(
                 listSTPtr->stateArySC[tmpSI]
               & def_listCanSelect_rayWidg
            ){ /*Else If: adding item to selection*/
               if(
                  selectGet_listBox_rayWidg(
                     tmpSI,
                     listSTPtr
                  )
               ) ; /*alreadly selected the item*/

               else
               { /*Else: need to select the item*/
                  listSTPtr->stateArySC[tmpSI] |=
                     def_listSelect_rayWidg;
                  listSTPtr->lastSelectSI = lastSelectSI;
                  lastSelectSI = tmpSI;
                  ++listSTPtr->numSelectSI;
               } /*Else: need to select the item*/
            }  /*Else If: adding item to selection*/

            ++tmpSI;
         } /*Loop: add files to selection*/

         goto selectEvent_fun116_sec06;
      } /*Else If: selecting a range of items*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun116 Sec05 Sub02 Cat04:
      +     - single click; set item as only selected item
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: user is selecting one item*/
         clearSelect_listBox_rayWidg(listSTPtr);

         if(
              listSTPtr->stateArySC[listSTPtr->onSI]
            & def_listCanSelect_rayWidg
         ){ /*If: can select the item*/
            if(listSTPtr->numSelectSI < maxSelectSI)
            { /*Else If: selecting the item*/
               ++listSTPtr->numSelectSI;
               listSTPtr->lastSelectSI = lastSelectSI;

               listSTPtr->stateArySC[listSTPtr->onSI] ^=
                   def_listSelect_rayWidg;
            }  /*Else If: selecting the item*/
         }  /*If: can select the item*/

         if(listSTPtr->clickSI == listSTPtr->onSI)
         { /*If: was a double click*/
            listSTPtr->clickSI = -1;/*clear double click*/

            if(
                 listSTPtr->stateArySC[listSTPtr->onSI]
               & def_listSpecial_rayWidg
            ) goto specialSelectEvent_fun116_sec06;
              /*selected a special item the user needs
              `  to do something with
              */
         } /*If: was a double click*/

         else
         { /*Else: single click*/
            listSTPtr->clickSI = listSTPtr->onSI;
            goto selectEvent_fun116_sec06;
         } /*Else: single click*/
      } /*Else: user is selecting one item*/
   } /*Else If: release event*/

   /*****************************************************\
   * Fun116 Sec05 Sub03:
   *   - check for right clicks (eventST does not capture)
   \*****************************************************/

   else if( IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) )
      listSTPtr->clickSI = -1;

   else if( IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) )
      listSTPtr->clickSI = -1;

   goto noEvent_fun116_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun116 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   moveEvent_fun116_sec06:;
      return 1;

   selectEvent_fun116_sec06:;
      return 2;

   specialSelectEvent_fun116_sec06:;
      return 4; /*is really enter on special event*/

   sideMoveEvent_fun116_sec06:;
      goto noEvent_fun116_sec06;

   /*these are states were the widget can not be
   `  interacted with or were the widget was not selected
   */

   notListBox_fun116_sec06:;
      return -1;

   noWidget_fun116_sec06:;
      return -2;

   noEvent_fun116_sec06:;
      return 0;

   hidenWidget_fun116_sec06:;
      return 0;

   inactiveWidget_fun116_sec06:;
      return 0;
} /*listBoxEvent_rayWidg*/

/*-------------------------------------------------------\
| Fun117: blank_files_rayWidg
|   - set all files in a files_rayWidg struct to no files 
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer to blank
| Output:
|   - calls blank function for listBox_rayWidg structures
|   - sets first item in pwdStr to null
\-------------------------------------------------------*/
void
blank_files_rayWidg(
  struct files_rayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   fileSTPtr->pwdStr[0] = 0;
   fileSTPtr->showHidenSC = 0;/*do not show hidden files*/
   fileSTPtr->lastWidgSI = -1;/*no previous widgets*/

   #ifdef WINDOWS
      cpStr_ulCp(
         fileSTPtr->pwdStr,
         (signed char *) getenv("HOMEPATH")
      );
   #else
      cpStr_ulCp(
         fileSTPtr->pwdStr,
         (signed char *) getenv("HOME")
      );
   #endif

   blank_listBox_rayWidg(&fileSTPtr->fileListST);
   blank_listBox_rayWidg(&fileSTPtr->extListST);
} /*blank_files_rayWidg*/

/*-------------------------------------------------------\
| Fun118: init_files_rayWidg
|   - initialize a files_rayWidg struct
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer to initialize
| Output:
|   - calls init function for listBox_rayWidg structures
|   - sets first item in pwdStr to null
\-------------------------------------------------------*/
void
init_files_rayWidg(
  struct files_rayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   init_listBox_rayWidg(&fileSTPtr->fileListST);
   init_listBox_rayWidg(&fileSTPtr->extListST);

   blank_files_rayWidg(fileSTPtr);
   selectMaxSet_listBox_rayWidg(-1,&fileSTPtr->extListST);
} /*init_files_rayWidg*/

/*-------------------------------------------------------\
| Fun119: freeStack_files_rayWidg
|   - frees variables in a files_rayWidg struct
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer to free variables in
| Output:
|   - calls freeStack function for listBox_rayWidg structs
|   - sets first item in pwdStr to null
\-------------------------------------------------------*/
void
freeStack_files_rayWidg(
  struct files_rayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   freeStack_listBox_rayWidg(&fileSTPtr->fileListST);
   freeStack_listBox_rayWidg(&fileSTPtr->extListST);

   init_files_rayWidg(fileSTPtr);
} /*freeStack_files_rayWidg*/

/*-------------------------------------------------------\
| Fun120: freeHeap_files_rayWidg
|   - frees files_rayWidg struct
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer to free
| Output:
|   - Frees: fileSTPtr, but you must set to null
\-------------------------------------------------------*/
void
freeHeap_files_rayWidg(
  struct files_rayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   freeStack_files_rayWidg(fileSTPtr);
   free(fileSTPtr);
} /*freeHeap_files_rayWidg*/

/*-------------------------------------------------------\
| Fun121: setFileLim_files_rayWidg
|   - sets the maximum number of files the user can choose
| Input:
|   - maxFilesSI:
|     o maximum number of files the user can select
|     o use -1 for any number
|   - fileSTPtr:
|     o files_rayWidg struct pointer to set file limit for
| Output:
|   - Modifies:
|     o fileListST in fileSTPtr to have maxFilesSI
\-------------------------------------------------------*/
void
setFileLimit_files_rayWidg(
   signed int maxFilesSI, /*file limit*/
   struct files_rayWidg *fileSTPtr
){
   selectMaxSet_listBox_rayWidg(
      maxFilesSI,
      &fileSTPtr->fileListST
   );
} /*setFileLimit_files_rayWidg*/

/*-------------------------------------------------------\
| Fun122: getFile_files_rayWidg
|   - gets the next selected files from a files_rayWidg
|     struct
| Input:
|   - fileOnSIPtr:
|     o signed int pionter with current file on and to
|       advance to the next file
|   - specialBl:
|     o 1: return items that are special, but not selected
|     o 0: only return a special item if it has been
|          selected
|   - fileSTPtr:
|     o files_rayWidg struct pointer to free
| Output:
|   - Modifies:
|     o fileOnSIPtr to the next file after the returned
|       file
|   - Returns:
|     o heap allocated signed char array with the next
|       file name (you must free it)
|       * max size is 511 bytes
|     o 0 for an error
|       * for no files fileOnSIPtr is set to -1
|       * for memory errors fileOnSIPtr is set to -2
\-------------------------------------------------------*/
signed char *
getFile_files_rayWidg(
   signed int *fileOnSIPtr, /*1st file to check if
                            `  selected
                            */
   signed char specialBl,   /*1: ret non-select special*/
   struct files_rayWidg *fileSTPtr /*has selected files*/
){
   signed char *retHeapStr = 0;
   signed int lenSI = 0;

   retHeapStr = malloc(512 * sizeof(signed char));
   if(! retHeapStr)
      goto memErr_fun00e;

   if(*fileOnSIPtr < 0)
      fileOnSIPtr = 0;

   while(*fileOnSIPtr < fileSTPtr->fileListST.lenSI)
   { /*Loop: find next file*/
      if(
          selectGet_listBox_rayWidg(
             *fileOnSIPtr,
             &fileSTPtr->fileListST
          )
          || (
                   specialBl
                && specialGet_listBox_rayWidg(
                   *fileOnSIPtr,
                   &fileSTPtr->fileListST
                )
             )
      ){ /*If: found a selected file*/
         lenSI = cpStr_ulCp(retHeapStr,fileSTPtr->pwdStr);
         retHeapStr[lenSI++] = def_pathSep_rayWidg;

         cpStr_ulCp(
           &retHeapStr[lenSI],
           &fileSTPtr->fileListST.textAryStr[
              *fileOnSIPtr
           ][4] /*4 skips "[d] " and "    " part*/
         );

         ++*fileOnSIPtr;
         goto retFile_fun00e;
      }  /*If: found a selected file*/

      else
         ++*fileOnSIPtr;
   }  /*Loop: find next file*/

   /*no files were found*/
   *fileOnSIPtr = -1;
   goto errClean_fun00e;

   memErr_fun00e:;
      *fileOnSIPtr = -2;
      goto errClean_fun00e;

   errClean_fun00e:;
      if(retHeapStr)
         free(retHeapStr);
      retHeapStr = 0;

      goto retFile_fun00e;

   retFile_fun00e:;
      return retHeapStr;
} /*getFile_files_rayWidg*/

/*-------------------------------------------------------\
| Fun123: swapFiles_files_rayWidg
|   - swap two files in a files_rayWidg struct
| Input:
|   - firstSI:
|     o index of first file to swap
|   - secSI:
|     o index of second file to swap
|   - fileSTPtr:
|     o files_rayWidg struct pionter with files to swap
| Output:
|   - Modifies:
|     o textAryStr, stateArySC, and sizeArySI in fileSTPtr
|       to have firstSI and secSI index's swapped
\-------------------------------------------------------*/
void
swapFiles_files_rayWidg(
   signed int firstSI,             /*1st file to swap*/
   signed int secSI,               /*second file to swap*/
   struct files_rayWidg *fileSTPtr /*has files to swap*/
){

   signed char *swapStr = 0;

   swapStr = fileSTPtr->fileListST.textAryStr[firstSI];
   fileSTPtr->fileListST.textAryStr[firstSI] =
      fileSTPtr->fileListST.textAryStr[secSI];
   fileSTPtr->fileListST.textAryStr[secSI] = swapStr;

   fileSTPtr->fileListST.stateArySC[firstSI] ^=
      fileSTPtr->fileListST.stateArySC[secSI];
   fileSTPtr->fileListST.stateArySC[secSI] ^=
      fileSTPtr->fileListST.stateArySC[firstSI];
   fileSTPtr->fileListST.stateArySC[firstSI] ^=
      fileSTPtr->fileListST.stateArySC[secSI];

   fileSTPtr->fileListST.sizeArySI[firstSI] ^=
      fileSTPtr->fileListST.sizeArySI[secSI];
   fileSTPtr->fileListST.sizeArySI[secSI] ^=
      fileSTPtr->fileListST.sizeArySI[firstSI];
   fileSTPtr->fileListST.sizeArySI[firstSI] ^=
      fileSTPtr->fileListST.sizeArySI[secSI];
} /*swapFiles_files_rayWidg*/

/*-------------------------------------------------------\
| Fun124: sortFiles_files_rayWidg
|   - sorts files by name and if directory
| Input:
|   - fileSTPtr:
|     o files_rayWidg sruct pionter to sort
| Ouput:
|   - Modifies:
|     o textAryStr in fileListST in fileSTPtr to be sorted
|       by if file/directory and then name
|     o stateArySC in fileListST in fileSTPtr to be sorted
|       by textAryStr in fileListST in fileSTPtr (kept in
|       sync)
\-------------------------------------------------------*/
void
sortFiles_files_rayWidg(
   struct files_rayWidg *fileSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun124 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun124 sec01:
   '     - variable declerations
   '   o fun124 sec02:
   '     - find the number of rounds to sort for
   '   o fun124 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun124 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct listBox_rayWidg *listSTPtr =
      &fileSTPtr->fileListST;

   /*Number of elements to sort*/
   signed long numElmSL =
      fileSTPtr->fileListST.lenSI - 2;
   signed long endSL = fileSTPtr->fileListST.lenSI - 1;

   /*Number of sorting rounds*/
   signed long subSL = 0;
   signed long nextSL = 0;
   signed long lastSL = 0;
   signed long onSL = 0;

   /*Variables to incurment loops*/
   signed long slIndex = 0;
   signed long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun124 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numElmSL < 3)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < numElmSL - 1)
      subSL = (3 * subSL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun124 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSL > 0)
   { /*Loop: all rounds*/
      for(
         slIndex = 1;
         slIndex <= subSL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subSL <= endSL;
            slElm += subSL
         ){ /*Loop: swap elements in subarray*/
            nextSL = slElm + subSL;

            if(! slElm)
               ; /*do not want to sort the first element*/
            else if(
               eqlNull_ulCp(
                  listSTPtr->textAryStr[slElm],
                  listSTPtr->textAryStr[nextSL]
               ) > 0
            ){ /*Else If: I need to swap an element*/
               swapFiles_files_rayWidg(
                  slElm,
                  nextSL,
                  fileSTPtr
               );

               lastSL = slElm;
               onSL = slElm;

               while(lastSL >= subSL)
               { /*loop: move swapped element back*/
                  lastSL -= subSL;
                  if(! lastSL)
                     break;

                  else if(
                     eqlNull_ulCp(
                        listSTPtr->textAryStr[onSL],
                        listSTPtr->textAryStr[lastSL]
                     ) >= 0
                  ) break;

                  swapFiles_files_rayWidg(
                     onSL,
                     lastSL,
                     fileSTPtr
                  );

                  onSL = lastSL;
               } /*Loop: move swapped element back*/
            } /*Else If: I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSL = (subSL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortFiles_files_rayWidg*/

/*-------------------------------------------------------\
| Fun125: scanForFiles_files_rayWidg
|   - get files in a directory
| Input:
|   - dirStr:
|     o c-string with the new working directory to get
|       files from
|   - firstBl:
|     o 1: is the first round, so get files even though
|          dirStr is same as pwdStr 
|     o 0: do not get files if dirStr is same as pwdStr 
|   - fileSTPtr:
|     o file_rayWidg struct pointer to add files in
|       directory to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font size
| Output:
|   - Modifies:
|     o fileListST in fileSTPtr to have files in directory
|   - Returns:
|     o 0 for no errors
|     o -1 if input directory was a file
|     o -2 if not a file or directory
|     o -3 for same directory (no change)
|     o -4 for memory errors
\-------------------------------------------------------*/
signed char
scanForFiles_files_rayWidg(
   signed char *dirStr, /*directory to get files for*/
   signed char firstBl, /*1: first round, overwrite*/
   struct files_rayWidg *fileSTPtr,/*add files to*/
   struct widg_rayWidg *widgSTPtr  /*has font size*/
){
   signed int siFile = 0; /*file on*/
   unsigned char stateUC = 0;
   signed char tmpStr[512];

   signed char *dupStr = 0;
   signed char *cpStr = 0;

   signed char errSC = 0;
   signed char freeBl = 0;
   struct FilePathList stFiles;

   if(firstBl)
      ; /*do not check for match between dirStr and pwd*/
   else if( ! eqlNull_ulCp(fileSTPtr->pwdStr, dirStr) )
      goto noChange_fun00g_sec0x;

   if(! DirectoryExists((char *) dirStr) )
      goto noDirOrFile_fun00g_sec0x;

   stFiles = LoadDirectoryFiles((char *) dirStr);
   freeBl = 1;

   clear_listBox_rayWidg(&fileSTPtr->fileListST);
   if(
      addItem_listBox_rayWidg(
         (signed char *) "[d] <--- (..)", /*back one dir*/
         def_listSpecial_rayWidg,
         &fileSTPtr->fileListST,
         widgSTPtr
      )
   ) goto memErr_fun00g_sec0x;

   for(
       siFile = 0;
       siFile < (signed int) stFiles.count;
      ++siFile
   ){ /*Loop: copy files*/
      if(
         checkExt_rayWidg(
            (signed char *) stFiles.paths[siFile],
            fileSTPtr->extListST.textAryStr,
            fileSTPtr->extListST.stateArySC,
            fileSTPtr->extListST.lenSI
         ) > 0
      ) stateUC = def_listCanSelect_rayWidg;

      else
         stateUC = def_listHidden_rayWidg;

      cpStr_ulCp(
         &tmpStr[4],
         (signed char *) stFiles.paths[siFile]
      );

      if( DirectoryExists(stFiles.paths[siFile]) )
      { /*If: file was a directory*/
         stateUC &= ~def_listHidden_rayWidg;
            /*directories should never be hidden*/
         stateUC |= def_listSpecial_rayWidg;

         tmpStr[0] = '[';
         tmpStr[1] = 'd';
         tmpStr[2] = ']';
      }  /*If: file was a directory*/

      else
      { /*Else : is a file*/
         tmpStr[0] = ' ';
         tmpStr[1] = ' ';
         tmpStr[2] = ' ';
      } /*Else : is a file*/

      tmpStr[3] = ' ';

      cpStr = &tmpStr[4];
      dupStr = cpStr;

      while(*cpStr)
      { /*Loop: get file or directory name*/
         if(*cpStr == def_pathSep_rayWidg)
            dupStr = &tmpStr[4];
         else
            *dupStr++ = *cpStr;
         ++cpStr;
      } /*Loop: get file or directory name*/

      *dupStr = 0;

      if(tmpStr[4] == '.' && ! fileSTPtr->showHidenSC)
         stateUC |= def_listHidden_rayWidg;

      if(
         addItem_listBox_rayWidg(
            tmpStr,
            stateUC,
            &fileSTPtr->fileListST,
            widgSTPtr
         )
      ) goto memErr_fun00g_sec0x;
   }  /*Loop: copy files*/

   sortFiles_files_rayWidg(fileSTPtr);

   errSC = 0;
   goto ret_fun00g_sec0x;

   noDirOrFile_fun00g_sec0x:;
      errSC = -2;
      goto ret_fun00g_sec0x;
   noChange_fun00g_sec0x:;
      errSC = -3;
      goto ret_fun00g_sec0x;
   memErr_fun00g_sec0x:;
      errSC = -4;
      goto ret_fun00g_sec0x;

   ret_fun00g_sec0x:;
      if(freeBl)
         UnloadDirectoryFiles(stFiles);
      return errSC;
} /*scanForFiles_files_rayWidg*/

/*-------------------------------------------------------\
| Fun126: changeExt_files_rayWidg
|   - updates files for the change extension type
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer with files to update
| Output:
|   - Modifies:
|     o fileListST in fileSTPtr to have new extensions
|       applied
\-------------------------------------------------------*/
void
changeExt_files_rayWidg(
   struct files_rayWidg *fileSTPtr
){
   signed char tmpFileStr[4096];
   signed int lenSI = 0;
   signed int siIndex = 0;

   fileSTPtr->fileListST.onSI = 0;
   fileSTPtr->fileListST.scrollSI = 0;
   fileSTPtr->fileListST.lastScrollSI = 0;

   for(
      siIndex = 1; /*0 is always back one dir*/
      siIndex < fileSTPtr->fileListST.lenSI;
      ++siIndex
   ){ /*Loop: check if should be hidden or not*/
      if(
              fileSTPtr->fileListST.textAryStr[siIndex][4]
            == '.'
         && ! fileSTPtr->showHidenSC
      ){ /*If: is a hidden file & ignoring hidden files*/
         fileSTPtr->fileListST.stateArySC[siIndex] |=
            def_listHidden_rayWidg;
         fileSTPtr->fileListST.stateArySC[siIndex] &=
            ~def_listCanSelect_rayWidg;
         continue;
      }  /*If: is a hidden file & ignoring hidden files*/

      lenSI = cpStr_ulCp(tmpFileStr, fileSTPtr->pwdStr);
      tmpFileStr[lenSI++] = def_pathSep_rayWidg;

      lenSI =
         cpStr_ulCp(
            &tmpFileStr[lenSI],
            &fileSTPtr->fileListST.textAryStr[siIndex][4]
         );

      if(
         checkExt_rayWidg(
            tmpFileStr,
            fileSTPtr->extListST.textAryStr,
            fileSTPtr->extListST.stateArySC,
            fileSTPtr->extListST.lenSI
         ) > 0
      ){ /*If: file matched the extension*/
         fileSTPtr->fileListST.stateArySC[siIndex] &=
            ~def_listHidden_rayWidg;
         fileSTPtr->fileListST.stateArySC[siIndex] |=
            def_listCanSelect_rayWidg;
      }  /*If: file matched the extension*/

      else if(
         specialGet_listBox_rayWidg(
            siIndex,
            &fileSTPtr->fileListST
         )
      ){ /*Else If: directory, but not selecting dirs*/
         fileSTPtr->fileListST.stateArySC[siIndex] &=
            ~def_listHidden_rayWidg;
         fileSTPtr->fileListST.stateArySC[siIndex] &=
            ~def_listCanSelect_rayWidg;
      }  /*Else If: directory, but not selecting dirs*/

      else
      { /*Else: not relavent to the extension*/
         fileSTPtr->fileListST.stateArySC[siIndex] |=
            def_listHidden_rayWidg;
         fileSTPtr->fileListST.stateArySC[siIndex] &=
            ~def_listCanSelect_rayWidg;
      } /*Else: not relavent to the extension*/
   }  /*Loop: check if should be hidden or not*/
} /*changeExt_files_rayWidg*/

/*-------------------------------------------------------\
| Fun127: addExt_files_rayWidg
|   - add an file extension to a files_rayWidg struct
| Input:
|   - extStr:
|     o c-string with extension to add (can be 0/null)
|   - clearBl:
|     o 1: remove all old extensions
|     o 0: append extension to the list
|   - selectBl:
|     o 1: select the extension for use
|     o 0: do not select the extension
|   - fileSTPtr:
|     o files_rayWidg struct pointer to add extension to
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with font size
| Output:
|   - Modifies:
|     o extListST in fileSTPtr to have the extensions
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addExt_files_rayWidg(
   signed char *extStr, /*file extension to add in*/
   signed char clearBl, /*1: clear old extensions*/
   signed char selectBl,/*1: select the extension*/
   struct files_rayWidg *fileSTPtr,/*add extension to*/
   struct widg_rayWidg *widgSTPtr /*has font*/
){
   unsigned char stateUC = 0;

   if(clearBl)
      clear_listBox_rayWidg(&fileSTPtr->extListST);

   if(! extStr)
      ; /*null input*/
   else if(! *extStr)
      ; /*empty string*/
   else
   { /*Else: have an extension to add*/
      if(selectBl)
      { /*If: first extension added in*/
         stateUC =
              def_listCanSelect_rayWidg
            | def_listSelect_rayWidg;
      } /*If: first extension added in*/

      else
         stateUC = def_listCanSelect_rayWidg;

      if(
         addItem_listBox_rayWidg(
            extStr,
            stateUC,
            &fileSTPtr->extListST,
            widgSTPtr
         )
      ) goto memErr_fun00f;
   } /*Else: have an extension to add*/

   return 0;

   memErr_fun00f:;
      return 1;
} /*addExt_files_rayWidg*/

/*-------------------------------------------------------\
| Fun128: mkFileBrowser_rayWidg
|   - makes a file browswer widget
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add file browser
|       and its children widgets to
| Output:
|   - Modifies:
|     o resizes arrays in widgSTPtr if not enough memory
|       to hold the message box and its children widgets
|   - Returns:
|     o parent id (index) of the file browser widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed char
mkFileBrowser_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   signed int idSI = 0;
   idSI = widgSTPtr->lenSI;

   if(widgSTPtr->lenSI + 12 >= widgSTPtr->sizeSI)
   { /*If: need more memory*/
      idSI += (idSI >> 1) + 12;

      if( realloc_widg_rayWidg(idSI, widgSTPtr) )
         goto memErr_fun00a;
   } /*If: need more memory*/

   idSI =
     addWidget_widg_rayWidg(0, 0, 0, 200, 100, widgSTPtr);
   inactiveAdd_widg_rayWidg(idSI, widgSTPtr);

   /*pick file list box*/
   mk_listBox_rayWidg(0, 0, 0, widgSTPtr);
   childAdd_widg_rayWidg(idSI + 1, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 1, widgSTPtr);

   /*ok button*/
   addWidget_widg_rayWidg(
      0, /*colomn*/
      0, /*row*/
      0, /*use x,y*/
      20, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   );
   childAdd_widg_rayWidg(idSI + 2, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 2, widgSTPtr);

   /*cancel button*/
   addWidget_widg_rayWidg(
      1, /*colomn*/
      0, /*row*/
      0, /*use x,y*/
      20, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   );
   childAdd_widg_rayWidg(idSI + 3, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 3, widgSTPtr);

   /*file extension list box*/
   mk_listBox_rayWidg(0, 0, 0, widgSTPtr);
   childAdd_widg_rayWidg(idSI + 4, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 4, widgSTPtr);

   hidenAdd_widg_rayWidg(idSI, widgSTPtr);
      /*hide the file browser*/
   return idSI;

   memErr_fun00a:;
      return -1;
} /*mkFileBrowser_rayWidg*/

/*-------------------------------------------------------\
| Fun129: fileBrowserDraw_rayWidg
|   - draws a file browswer widget
| Input:
|   - idSI:
|     o parent id of file browser widget
|   - mesgStr:
|     o c-string with message to print for user
|   - fileSTPtr:
|     o files_rayWidg struct pointer with files, current
|       working directory, and extensions
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with coordinates and
|       dimensions of file browser to draw
| Output:
|   - Modifies:
|     - file browswer widgets widthArySI, heightArySI,
|       xArySI, and yArySI in widgSTPtr to have file
|       browswer position and dimensions
|       changed to
|     - will set first extension to active
|       (fileExtAryStr[0][0] *= -1) if no extension is
|       setup as default
|   - Returns:
|     o width of file browser if drawn
|     o 0 if file browser is not drawn
|     o def_noWidget_rayWidg if id is invalid
\-------------------------------------------------------*/
signed char
fileBrowserDraw_rayWidg(
   signed int idSI,                /*id of file browser*/
   signed char *mesgStr,           /*message for the usr*/
   struct files_rayWidg *fileSTPtr,/*has files*/
   struct widg_rayWidg *widgSTPtr  /*has browser widget*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun129 TOC:
   '   - draws a file browswer widget
   '   o fun129 sec01:
   '     - variable declarations
   '   o fun129 sec02:
   '     - check if can draw file browswer
   '   o fun129 sec03:
   '     - get coordinates and dimensions for all widgets
   '   o fun129 sec04:
   '     - draw file browswer
   '   o fun129 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun129 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   float tmpF = 0;
   signed int tmpSI = 0;

   #define def_maxStrLen_fun129 512
   signed char pwdOutStr[def_maxStrLen_fun129 + 8];
      /*holds modified pwdStr*/
   signed char mesgOutStr[def_maxStrLen_fun129 + 8];
      /*holds modified message*/

   signed int mesgWidthSI = 0;  /*width of message*/
   signed int mesgHeightSI = 0; /*height of message*/

   signed int heightSI = widgSTPtr->winHeightSI;
   signed int widthSI = widgSTPtr->winWidthSI;

   signed int pwdHeightSI = 0;
   signed int pwdWidthSI = 0;
   signed int pwdXSI = 0;
   signed int pwdYSI = 0;

   signed int widgGapSI = 
        widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);
   signed int widgPadSI = widgSTPtr->fontWidthF * 2;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun129 Sec02:
   ^   - check if can draw file browswer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidg_fun129_sec05;
   else if(idSI < 0)
      goto noWidg_fun129_sec05;
   else if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hiden_fun129_sec05;

   if(widgSTPtr->focusSI < idSI)
   { /*If: file browser is not in focus*/
      fileSTPtr->lastWidgSI = widgSTPtr->focusSI;
      widgSTPtr->focusSI = idSI + 1;
      focusAdd_widg_rayWidg(idSI + 1, widgSTPtr);
   } /*If: file browser is not in focus*/

   else if(widgSTPtr->focusSI > idSI + 4)
   { /*Else If: file browser is not in focus*/
      fileSTPtr->lastWidgSI = widgSTPtr->focusSI;
      widgSTPtr->focusSI = idSI + 1;
      focusAdd_widg_rayWidg(idSI + 1, widgSTPtr);
   } /*Else If: file browser is not in focus*/

   if(! fileSTPtr->fileListST.lenSI && *fileSTPtr->pwdStr)
   { /*If: no files were loaded*/
      if(
         scanForFiles_files_rayWidg(
            fileSTPtr->pwdStr,
            1, /*get files*/
            fileSTPtr,
            widgSTPtr
         )
      ) goto noWidg_fun129_sec05;
   }  /*If: no files were loaded*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun129 Sec03:
   ^   - get coordinates and dimensions for all widgets
   ^   o fun129 sec03 sub01:
   ^     - find file browswer rectangle dimensions
   ^   o fun129 sec03 sub02:
   ^     - find present working directory label dimensions
   ^   o fun129 sec03 sub03:
   ^     - find file list box dimensions
   ^   o fun129 sec03 sub04:
   ^     - extension list box dimensions
   ^   o fun129 sec03 sub05:
   ^     - message box dimensions
   ^   o fun129 sec03 sub06:
   ^     - get button coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun129 Sec03 Sub01:
   *   - find file browswer rectangle dimensions
   \*****************************************************/

   /*center the file browswer and make take up most of the
   `  GUI
   */
   widgSTPtr->heightArySI[idSI] = heightSI - widgGapSI;
   widgSTPtr->widthArySI[idSI] = widthSI - widgPadSI;

   widgSTPtr->yArySI[idSI] = widgGapSI / 2;
   widgSTPtr->xArySI[idSI] = widgPadSI / 2;

   /*****************************************************\
   * Fun129 Sec03 Sub02:
   *   - find present working directory label dimensions
   *   o fun129 sec03 sub02 cat01:
   *     - pwd general dimensions
   *   o fun129 sec03 sub02 cat02:
   *     - copy pwd string and shorten (if needed)
   *   o fun129 sec03 sub02 cat03:
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun129 Sec03 Sub02 Cat01:
   +   - pwd rectangle general dimensions
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   pwdXSI = widgSTPtr->xArySI[idSI];
   pwdYSI = widgSTPtr->yArySI[idSI];

   pwdHeightSI =
        widgSTPtr->fontHeightF
      + (widgSTPtr->fontHeightF / widgGapSI);

   pwdWidthSI = widgSTPtr->widthArySI[idSI];

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun129 Sec03 Sub02 Cat02:
   +   - copy pwd string and shorten (if needed)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   backCheckTextWidth_rayWidg(
      fileSTPtr->pwdStr,
      pwdOutStr,
      pwdWidthSI - (widgPadSI << 2),
      widgSTPtr
   );
 
   if(pwdOutStr[0] != '.')
      ;
   else if(pwdOutStr[1] != '.')
      ;
   else if(pwdOutStr[2] == '.')
   { /*Else If: shortned the file path*/
      tmpSI = 3;

      while(pwdOutStr[tmpSI])
      { /*Loop: find file separator*/
         if(pwdOutStr[tmpSI] == def_pathSep_rayWidg)
            break;
         ++tmpSI;
      } /*Loop: find file separator*/

      if(! pwdOutStr[tmpSI])
         ; /*end of string*/
      else if(! pwdOutStr[tmpSI + 1])
         ; /*end of string (slash at end)*/
      else if(tmpSI <= 3)
         ; /*perfect trim*/
      else
      { /*Else: have part of file name*/
         ++tmpSI;
         pwdOutStr[3] = def_pathSep_rayWidg;
         cpStr_ulCp(&pwdOutStr[4], &pwdOutStr[tmpSI]);
      } /*Else: have part of file name*/

      tmpSI = 0;
   } /*Else If: shortned the file path*/

   /*****************************************************\
   * Fun129 Sec03 Sub03:
   *   - find file list box dimensions
   \*****************************************************/

   fileSTPtr->fileListST.minHeightSI =
        ( (widgSTPtr->heightArySI[idSI] / 3) * 2 )
      - pwdHeightSI
      - widgGapSI;

   fileSTPtr->fileListST.maxHeightSI =
     fileSTPtr->fileListST.minHeightSI;

   fileSTPtr->fileListST.minWidthSI =
      widgSTPtr->widthArySI[idSI] - widgPadSI;
   fileSTPtr->fileListST.maxWidthSI =
      fileSTPtr->fileListST.minWidthSI;

   widgSTPtr->xArySI[idSI + 1] =
      widgSTPtr->xArySI[idSI] + (widgPadSI / 2);
   widgSTPtr->yArySI[idSI + 1] = pwdHeightSI + widgGapSI;

   /*****************************************************\
   * Fun129 Sec03 Sub04:
   *   - extension list box dimensions
   \*****************************************************/

   fileSTPtr->extListST.minWidthSI =
          fileSTPtr->extListST.lineWidthSI
        + 3
        + widgSTPtr->fontWidthF * 3; /*+ 3 for # and >*/
   fileSTPtr->extListST.maxWidthSI =
     fileSTPtr->extListST.minWidthSI;

   widgSTPtr->xArySI[idSI + 4] =
        widgSTPtr->widthArySI[idSI]
      - fileSTPtr->extListST.minWidthSI;

   widgSTPtr->yArySI[idSI + 4] =
        widgSTPtr->yArySI[idSI + 1]
      + widgSTPtr->heightArySI[idSI + 1]
      + widgGapSI;

   fileSTPtr->extListST.minHeightSI =
        widgSTPtr->heightArySI[idSI]
      + widgSTPtr->yArySI[idSI]
      - widgSTPtr->yArySI[idSI + 4]
      - widgGapSI;

   fileSTPtr->extListST.maxHeightSI =
      fileSTPtr->extListST.minHeightSI;

   /*****************************************************\
   * Fun129 Sec03 Sub05:
   *   - message dimensions
   *   o fun129 sec03 sub05 cat01:
   *     - find space avaliable for the message
   *   o fun129 sec03 sub05 cat02:
   *     - copy message and break into lines as needed
   *   o fun129 sec03 sub05 cat03:
   *     - shorten message if needed
   \*****************************************************/

   mesgWidthSI =
        widgSTPtr->widthArySI[idSI]
      - widgPadSI
      - fileSTPtr->extListST.minWidthSI;

   mesgHeightSI = widgSTPtr->fontHeightF + widgGapSI;

   checkTextWidth_rayWidg(
      mesgStr,
      mesgOutStr,
      mesgWidthSI,
      widgSTPtr
   );

   /*****************************************************\
   * Fun129 Sec03 Sub06:
   *   - get button coordinates
   \*****************************************************/

   tmpSI =
      butDraw_rayWidg(
         200, /*maximum width*/
         20,  /*minimum width*/
         idSI + 2,
         (signed char *) "Select",
         1, /*do not draw the button yet*/
         widgSTPtr
      );

   widgSTPtr->xArySI[idSI + 2] = widgPadSI;

   widgSTPtr->xArySI[idSI + 3] = 
        widgSTPtr->xArySI[idSI + 2]
      + tmpSI
      + widgPadSI;

   widgSTPtr->yArySI[idSI + 2] = 
        widgSTPtr->yArySI[idSI + 1]
      + widgSTPtr->heightArySI[idSI + 1]
      + mesgHeightSI
      + widgGapSI;
   widgSTPtr->yArySI[idSI +3] = widgSTPtr->yArySI[idSI+2];
      
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun129 Sec04:
   ^   - draw file browswer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   rectDraw_rayWidg(
      widgSTPtr,
      idSI,
      1, /*has border, but not in focus*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      0
   ); /*background rectangle to hold file browser*/

   rectDrawByCoord_rayWidg(
      pwdXSI,
      pwdYSI,
      pwdWidthSI,
      pwdHeightSI,
      1, /*has border, but not in focus*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      0,
      widgSTPtr
   ); /*rectangle for the present working directory*/

   /*add in the message*/
   DrawTextEx(
      *widgSTPtr->fontSTPtr,
      (char *) pwdOutStr,
      (Vector2) {
         pwdXSI + (widgPadSI / 2),
         pwdYSI + (widgGapSI / 8)
      },
      widgSTPtr->fontSizeSI,
      widgSTPtr->spacingF,
      GetColor(widgSTPtr->textColSI)
   );

   draw_listBox_rayWidg(
      idSI + 1,
      0, /*drawing*/
      &fileSTPtr->fileListST,
      widgSTPtr
   );

   draw_listBox_rayWidg(
      idSI + 4,
      0, /*drawing*/
      &fileSTPtr->extListST,
      widgSTPtr
   );

   if(widgSTPtr->xScaleF >= 1)
      tmpF = widgSTPtr->xScaleF;
   else
      tmpF = 1;

   butDraw_rayWidg(
      200 * tmpF, /*maximum width*/
      20,  /*minimum width*/
      idSI + 2,
      (signed char *) "Select",
      0,
      widgSTPtr
   );

   butDraw_rayWidg(
      200 * tmpF, /*maximum width*/
      20,  /*minimum width*/
      idSI + 3,
      (signed char *) "Cancel",
      0,
      widgSTPtr
   );

   /*add in the message*/
   DrawTextEx(
      *widgSTPtr->fontSTPtr,
      (char *) mesgOutStr,
      (Vector2) {
         widgSTPtr->xArySI[idSI] + widgPadSI,
         widgSTPtr->yArySI[idSI + 1]
           + widgSTPtr->heightArySI[idSI + 1]
           + widgGapSI
      },
      widgSTPtr->fontSizeSI,
      widgSTPtr->spacingF,
      GetColor(widgSTPtr->textColSI)
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun129 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgSTPtr->widthArySI[idSI];

   noWidg_fun129_sec05:;
     tmpSI = def_noWidget_rayWidg;
     goto retNoDraw_fun129_sec05;
   hiden_fun129_sec05:;
     tmpSI = 0;
     goto retNoDraw_fun129_sec05;

   retNoDraw_fun129_sec05:;
     if(widgSTPtr->focusSI < idSI)
        ;
     else if(widgSTPtr->focusSI > idSI + 4)
        ;
     else
        widgSTPtr->focusSI = 0;
        /*make sure focus is not on invisible hog state*/

     return tmpSI;
} /*fileBrowserDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun130: fileBrowserEvent_rayWidg
|   - process and event for the file browser
| Input:
|   - idSI:
|     o id of file browser, this is used to check if the
|       file browser was really selected (avoids ifs in
|       your code)
|   - eventSTPtr:
|     o event_rayWidg struct pointer with the event to run
|   - fileSTPtr:
|     o files_rayWidg struct pointer with files and
|       extensions
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with file browser
|       to draw
| Output:
|   - Modifies:
|     - fileSTPtr to have changes in directory (pwdStr),
|       files selection (fileListST), and extenon changes
|       (extListST)
|   - Returns:
|     o 2 for cancel event
|     o 1 for ok event
|       - get files with the `getFile_files_rayWidg()`
|         function
|     o 0 for no major event
|     o -1 if file browser widget no selected (different
|       widget)
|     o -2 if no widget was selected
|     o -3 for errors (memory, file, ect ...)
\-------------------------------------------------------*/
signed char
fileBrowserEvent_rayWidg(
   signed int idSI,                 /*id of file browser*/
   struct event_rayWidg *eventSTPtr,/*has click event*/
   struct files_rayWidg *fileSTPtr, /*has files/extions*/
   struct widg_rayWidg *widgSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun130:
   '   - process and event for the file browser
   '   o fun130 sec01:
   '     - variable declarations
   '   o fun130 sec02:
   '     - check if widget can take events
   '   o fun130 sec03:
   '     - find the event
   '   o fun130 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun130 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0; 
   signed char tmpSC = 0;
   signed char *dirHeapStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun130 Sec02:
   ^   - check if widget can take events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(eventSTPtr->parIdSI != idSI)
   { /*If: not file browser event*/
      if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
         ;
      else
      { /*Else: make sure have file list to show*/
         if(
               ! fileSTPtr->fileListST.lenSI
            && *fileSTPtr->pwdStr
         ){ /*If: no files were loaded*/
            if(
               scanForFiles_files_rayWidg(
                  fileSTPtr->pwdStr,
                  1, /*get files*/
                  fileSTPtr,
                  widgSTPtr
               )
            ) goto err_fun130_sec04;
         } /*If: no files were loaded*/
      } /*Else: make sure have file list to show*/

      goto notFileBrowser_fun130_sec04;
   } /*If: not file browser event*/

   else if(eventSTPtr->idSI < 0)
      goto noEvent_fun130_sec04;

   else if(eventSTPtr->parIdSI < 0)
      goto noWidg_fun130_sec04;

   else if(
      hidenGet_widg_rayWidg(eventSTPtr->idSI, widgSTPtr)
   ) goto hiden_fun130_sec04;

   else if(eventSTPtr->parIdSI >= widgSTPtr->lenSI)
      goto noWidg_fun130_sec04;

   else if(
      hidenGet_widg_rayWidg(eventSTPtr->idSI, widgSTPtr)
   ) goto hiden_fun130_sec04;

   else if(
      inactiveGet_widg_rayWidg(eventSTPtr->idSI,widgSTPtr)
   ) goto noEvent_fun130_sec04;

   else if(
         ! fileSTPtr->fileListST.lenSI
      && *fileSTPtr->pwdStr
   ){ /*Else If: no files were loaded*/
      if(
         scanForFiles_files_rayWidg(
            fileSTPtr->pwdStr,
            1, /*get files*/
            fileSTPtr,
            widgSTPtr
         )
      ) goto err_fun130_sec04;

      goto noEvent_fun130_sec04;
   }  /*Else If: no files were loaded*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun130 Sec03:
   ^   - find the event
   ^   o fun130 sec03 sub01:
   ^     - check for button events
   ^   o fun130 sec03 sub02:
   ^     - check for file list box events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun130 Sec03 Sub01:
   *   - check for button events
   \*****************************************************/

   if(eventSTPtr->idSI == eventSTPtr->parIdSI + 2)
   { /*If: select files button hit*/
      if(
            eventSTPtr->leftReleaseBl
         || eventSTPtr->keySI == KEY_ENTER
      ) goto seletectedFiles_fun130_sec04;

      else
         goto noEvent_fun130_sec04;
   } /*If: select files button hit*/

   else if(eventSTPtr->idSI == eventSTPtr->parIdSI + 3)
   { /*Else If: cancel files button hit*/
      if(
            eventSTPtr->leftReleaseBl
         || eventSTPtr->keySI == KEY_ENTER
      ) goto cancle_fun130_sec04;

      else
         goto noEvent_fun130_sec04;
   } /*Else If: cancel files button hit*/

   /*****************************************************\
   * Fun130 Sec03 Sub02:
   *   - check for file list box events
   *   o fun130 sec03 sub02 cat01:
   *     - get file list box event type
   *   o fun130 sec03 sub02 cat02:
   *     - one directory up case
   *   o fun130 sec03 sub02 cat03:
   *     - moving to next directory
   *   o fun130 sec03 sub02 cat04:
   *     - get files in new directory
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun130 Sec03 Sub02 Cat01:
   +   - get file list box event type
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(eventSTPtr->idSI == eventSTPtr->parIdSI + 1)
   { /*Else If: user did something with a file*/
      tmpSI =
         listBoxEvent_rayWidg(
            idSI + 1,
            &fileSTPtr->fileListST,
            eventSTPtr,
            widgSTPtr
         );

      if(! tmpSI)
         goto noEvent_fun130_sec04;
      else if(tmpSI == 1)
         goto noEvent_fun130_sec04;
      else if(tmpSI == 2)
      { /*Else If: user selected something*/
         if(
              eventSTPtr->keySI == KEY_A
           && eventSTPtr->ctrlBl
         ){ /*If: all files selected*/
            selectSet_listBox_rayWidg(
               0,
               0,
               &fileSTPtr->fileListST
            );
            /*user likely did not intend to select the
            `  back one directory option
            */
         }  /*If: all files selected*/

         goto noEvent_fun130_sec04;
      } /*Else If: user selected something*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun130 Sec03 Sub02 Cat02:
      +   - one directory up case
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: directory selected*/
         if(! fileSTPtr->fileListST.onSI)
         { /*If: going back one directory*/
            tmpSI = endStr_ulCp(fileSTPtr->pwdStr) - 1;
            while(
                  tmpSI > 0
               &&    fileSTPtr->pwdStr[tmpSI]
                  != def_pathSep_rayWidg
            ) --tmpSI;

            if(
                 ! tmpSI
              && *fileSTPtr->pwdStr == def_pathSep_rayWidg
            ) fileSTPtr->pwdStr[tmpSI + 1] = 0;
              /*If: Uinux root directory*/

            else if(! tmpSI)
               goto noEvent_fun130_sec04;
               /*can  not go further back in history*/

            else
            { /*Else: windows root or not root*/
               tmpSC = fileSTPtr->pwdStr[tmpSI];
               fileSTPtr->pwdStr[tmpSI] = 0;

               if(
                  DirectoryExists(
                     (char *)fileSTPtr->pwdStr)
               ) ;

               else
               { /*Else: hit end of directory list*/
                  fileSTPtr->pwdStr[tmpSI] = tmpSC;
                  goto noEvent_fun130_sec04;
               } /*Else: hit end of directory list*/
            } /*Else: windows root or not root*/

         } /*If: going back one directory*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun130 Sec03 Sub02 Cat03:
         +   - moving to next directory
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: adding another directory*/

            tmpSI = fileSTPtr->fileListST.onSI;

            if(
                  tmpSI < 0
               || tmpSI > fileSTPtr->fileListST.lenSI
            ) goto noEvent_fun130_sec04;

            dirHeapStr =
               getFile_files_rayWidg(
                  &tmpSI,
                  1, /*want even if not selected*/
                  fileSTPtr
               );
            cpStr_ulCp(fileSTPtr->pwdStr, dirHeapStr);
            free(dirHeapStr);
            dirHeapStr = 0;
         } /*Else: adding another directory*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun130 Sec03 Sub02 Cat04:
         +   - get files in new directory
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(
            scanForFiles_files_rayWidg(
               fileSTPtr->pwdStr,
               1, /*run despite pwdStr's are the same*/
               fileSTPtr,
               widgSTPtr
            )
         ) goto err_fun130_sec04;

         goto noEvent_fun130_sec04;
      } /*Else: directory selected*/
   } /*Else If: user did something with a file*/

   /*****************************************************\
   * Fun130 Sec03 Sub03:
   *   - check for extension list box events
   \*****************************************************/

   else if(eventSTPtr->idSI == eventSTPtr->parIdSI + 4)
   { /*Else If: is the file extension box*/
      fileSTPtr->fileListST.clickSI = -1;

      tmpSI =
         listBoxEvent_rayWidg(
            idSI + 4,
            &fileSTPtr->extListST,
            eventSTPtr,
            widgSTPtr
         );

      if(! tmpSI)
         goto noEvent_fun130_sec04;
      else if(tmpSI == 1)
         goto noEvent_fun130_sec04;
      else
      { /*Else: change in selection*/
         changeExt_files_rayWidg(fileSTPtr);
         goto noEvent_fun130_sec04;
      } /*Else: change in selection*/
   } /*Else If: is the file extension box*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun130 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto noEvent_fun130_sec04;
      /*user did an event, but it was not a button*/

   seletectedFiles_fun130_sec04:;
      tmpSI = 1;
      hidenAdd_widg_rayWidg(
         eventSTPtr->parIdSI,
         widgSTPtr
      );
      widgSTPtr->focusSI = fileSTPtr->lastWidgSI;
      fileSTPtr->lastWidgSI = -1;
      goto ret_fun130_sec04;

   cancle_fun130_sec04:;
      tmpSI = 3;
      clearSelect_listBox_rayWidg(&fileSTPtr->fileListST);
      hidenAdd_widg_rayWidg(
         eventSTPtr->parIdSI,
         widgSTPtr
      );
      widgSTPtr->focusSI = fileSTPtr->lastWidgSI;
      fileSTPtr->lastWidgSI = -1;
      goto ret_fun130_sec04;

   hiden_fun130_sec04:;
      tmpSI = 0;
      goto ret_fun130_sec04;

   noEvent_fun130_sec04:;
      tmpSI = 0;
      goto ret_fun130_sec04;

   notFileBrowser_fun130_sec04:;
      tmpSI = -1;
      goto ret_fun130_sec04;

   noWidg_fun130_sec04:;
      tmpSI = -2;
      goto ret_fun130_sec04;

   err_fun130_sec04:;
      tmpSI = -3;
      goto ret_fun130_sec04;

   ret_fun130_sec04:;
      if(dirHeapStr)
         free(dirHeapStr);
      dirHeapStr = 0;

      return (signed char) tmpSI;
} /*fileBrowserEvent_rayWidg*/
