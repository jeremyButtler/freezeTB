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
'       o stores mouse, keyboard, & scroll wheel events
'     o .h st03: listBox_rayWidg
'       o structure to store values in a list box
'       o also grouped macros for list box states here
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
'       o fun004: setup_widg_rayWidg
'         - adds the default font to a rayWidg struct and
'           sets it up for use (only call once)
'       o fun005: freeStack_widg_rayWidg
'         - frees arrays in a widg_rayWidg struct
'       o fun006: freeHeap_widg_rayWidg
'         - frees a widg_rayWidg struct
'       o fun007: realloc_widg_rayWidg
'         - reallocates memory for a widg_rayWidg struct
'         - note: this will not downsize or report
'           downsize attempts
'     + Sof02 Sec02:
'       - change font or font variables in widg_rayWidg
'       o fun008: fontMeasure_widg_rayWidg
'         - measures the width of the character "D" for a
'           font
'       o fun009: textMeasure_widg_rayWidg
'         - measures the width of input text
'       o fun010: checkTextWidth_rayWidg
'         - check if text meets widgets length, if not
'           shorten
'       o fun011: changeFont_widg_rayWidg
'         - adds a font from a file to a widg_rayWidg
'       o fun012: useEmbedFont_widg_rayWidg
'         - uses an embed font
'         - make this using ExprotFontAsCode from raylib
'       o fun013: swapFont_widg_rayWidg
'         - swaps the font out with an input font pointer
'         - this is a swap of pointers only, so only free
'           the input font when you are finished
'       o fun014: setFontSize_widg_rayWidg
'         - changes font size
'       o fun015: setFontSpacing_widg_rayWidg
'         - changes a fonts spacing
'     + Sof02 Sec03:
'       - widg_rayWidg struct querying functions
'       o fun016: xCoordGet_widg_rayWidg
'         - get the x-coordinate of a widget
'       o fun017: yCoordSet_widg_rayWidg
'         - get the y-coordinate of a widget
'       o fun018: widthGet_widg_rayWidg
'         - get the width of a widget
'       o fun019: heightGet_widg_rayWidg
'         - get the height of a widget
'       o fun020: rowGet_widg_rayWidg
'         - get the row a widget is assigned to
'       o fun021: colGet_widg_rayWidg
'         - get the column a widget is assigned to
'       o fun022: stateGet_widg_rayWidg
'         - get the state of a widget
'       o fun023: pressGet_widg_rayWidg
'         - detect if widget is in pressed state
'       o fun024: checkedGet_widg_rayWidg
'         - detect if widget is in checked state
'       o fun025: activeGet_widg_rayWidg
'         - detect if widget is in active state
'       o fun026: focusGet_widg_rayWidg
'         - detect if widget is in focus state
'       o fun027: inacitveGet_widg_rayWidg
'         - detect if widget is in inacitve state
'       o fun028: hidenGet_widg_rayWidg
'         - detect if widget is in hiden state
'       o fun029: childGet_widg_rayWidg
'         - detect if widget is a child widget
'       o fun030: hogGet_widg_rayWidg
'         - detect if widget is a hog (high priority)
'           widget
'       o fun031: numWidgetGet_widg_rayWidg
'         - get number of widgets made
'       o fun032: focusWidgGet_widg_rayWidg
'         - get the id (index) of the focused widget
'     + Sof02 Sec04:
'       - widg_rayWidg struct modify coordiantes & size
'       o fun033: xCoordSet_widg_rayWidg
'         - sets a widgets x coordiante
'       o fun034: yCoordSet_widg_rayWidg
'         - sets a widgets y coordiante
'       o fun035: widthSet_widg_rayWidg
'         - sets a widgets width
'       o fun036: heightSet_widg_rayWidg
'         - sets a widgets height
'       o fun037: rowSet_widg_rayWidg
'         - sets a widgets row number
'       o fun038: colSet_widg_rayWidg
'         - sets a widgets column number
'     + Sof02 Sec05:
'       - widg_rayWidg struct modify state of widgets
'       o fun039: stateSet_widg_rayWidg
'         - sets a widgets state to input state
'       o fun040: pressAdd_widg_rayWidg
'         - adds the pressed state to a widget
'       o fun041: checkedAdd_widg_rayWidg
'         - adds the checkeded state to a widget
'       o fun042: activeAdd_widg_rayWidg
'         - adds the activeed state to a widget
'       o fun043: focusAdd_widg_rayWidg
'         - adds the activeed state to a widget
'       o fun044: inactiveAdd_widg_rayWidg
'         - adds the inactive state to a widget
'       o fun045: hidenAdd_widg_rayWidg
'         - adds the hiden state to a widget
'       o fun046: childAdd_widg_rayWidg
'         - adds the child state to a widget
'         - only use this funcition if you know what your
'           are are doing and understand how the
'           child/parent system in rayWidg works
'       o fun047: hogAdd_widg_rayWidg
'         - adds the hog state to a widget
'     + Sof07 Sec06:
'       - widg_rayWidg struct clear state of widgets
'       o fun048: stateClear_widg_rayWidg
'         - remove all states in a widget (set to 0)
'       o fun049: pressClear_widg_rayWidg
'         - remove wigets pressed state
'       o fun050: checkedClear_widg_rayWidg
'         - remove wigets checked state
'       o fun051: activeClear_widg_rayWidg
'         - remove wigets active state
'       o fun052: focusClear_widg_rayWidg
'         - remove wigets focus state
'       o fun053: inactiveClear_widg_rayWidg
'         - remove wigets & its child widgets inactive
'           state
'       o fun054: hidenClear_widg_rayWidg
'         - remove wigets and its child widgets hiden
'           state
'       o fun055: childClear_widg_rayWidg
'         - clears the child state in a widget
'         - only use this function if you know what your
'           are doing and understand how the child/parent
'           system works
'       o fun056: hogClear_widg_rayWidg
'         - clears the hog state in a widget
'   * Sof03:
'     - event_rayWidg structure functions
'     o fun057: blank_event_rayWidg
'       - sets all variables in event_rayWidg struct to
'         blank (0 or -1)
'     o fun058: init_event_rayWidg
'       - sets all variables in an event_rayWidg struct to
'         defuautls/null
'     o fun059: freeStack_event_rayWidg
'       - frees variables in an event_rayWidg struct
'     o fun060: freeHeap_event_rayWidg
'       - frees a heap allocated event_rayWidg struct
'     o fun061: get_event_rayWidg
'       - adds a user event to an event_rayWidg struct
'   * Sof04:
'     - get OS state
'     o fun062: getDarkModeState_rayWidg
'       - detects if Mac/Windows computer is in dark mode
'     o fun063: checkGuiColorMode_rayWidg
'       - checks is user is using dark or light mode, and
'         sets the color scheme to the correct mode
'   * Sof05:
'     - widget modification or focus/state changes
'     o fun064: addWidget_widg_rayWidg
'       - add a new widget to a widg_rayWidg struct
'     o fun065: tile_widg_rayWidg
'       - find tiling x,y coordiantes for each widget
'         using widths, heights, rows, and columns
'       - note this can only support one level of children
'         widgets
'     o fun066: focusCheck_widg_rayWidg
'       - detect if input key was tab and update focus
'     o fun067: enterCheck_widg_rayWidg
'       - check if key event was an enter key
'     o fun068: getMouseWidg_widg_rayWidg
'       - get widget mouse was on
'     o fun069: blinkGet_rayWidg
'      - returns current state of cursor blink for widgets
'     o fun070: getLines_rayWidg
'       - get number of lines can fit in a set height
'     o fun071: lineToHeight_rayWidg
'       - convert line count to height in pixels
'   * Sof06:
'     - string functions
'     o fun072: intStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an integer
'     o fun073: floatStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an float
'     o fun074: fileStrCheck_rayWidg
'       - corrects non-file characters in c-string
'       - spaces/tabes converted to '_', other than that,
'         only A-Z, a-z, 0-9, '-', '_', & '.' are allowed
'     o fun075: addCharToEntry_rayWidg
'       - adds a character to an entry box text
'   * Sof07:
'     - file_rayWidg and file functions
'     o fun076: checkExt_rayWidg
'       - checks if file is the correct input extension
'   * Sof08:
'     - widget drawing
'       + s0f08 sec01:
'         - simple widgets
'       + s0f08 sec02:
'         - message box
'       + sof08 sec03:
'         - list box
'     + Sof08 Sec01:
'       - simple widgets
'       o fun077: drawRectByCoord_rayWidg
'         - draws a rectangle with input coordinates
'       o fun078: drawRec_rayWidg
'         - draws a rectangle for rayWidg
'       o fun079: butDraw_rayWidg
'         - draw a button
'       o fun080: entryDraw_rayWidg
'         - draw an entry box and its text
'       o fun081: entryEvent_rayWidg
'         - handels an event for an entry box
'       o fun082: labDraw_rayWidg
'         - draw a label
'     + Sof08 Sec02:
'       - message box
'       o fun084: mkMesgBox_rayWidg
'         - makes an emtpy message box widget
'       o fun085: mesgBoxDraw_rayWidg
'         - draws a message box
'       o fun086: mesgBoxEvent_rayWidg
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
'         o fun087: blank_listBox_rayWidg
'           - blanks a listBox_rayWidg structure
'         o fun088: init_listBox_rayWidg
'           - initializes a listBox_rayWidg structure
'         o fun089: freeStack_listBox_rayWidg
'           - frees variables in a listBox_rayWidg struct
'         o fun091: realloc_listBox_rayWidg
'           - add more memory to listBox_rayWidg structure
'         o fun092: clear_listBox_rayWidg
'           - removes all items from the list box
'         o fun093: clearSelect_listBox_rayWidg
'           - clear all selections from a list box
'       # Sof08 Sec03 Sub02:
'         - listBox_rayWidg set variables functions
'         o fun094: selectMaxSet_listBox_rayWidg
'           - sets maximum number of items that can be
'             selected in a list box
'         o fun095: widthSet_listBox_rayWidg
'           - sets maximum and minimum width of a list box
'         o fun096: heightSet_listBox_rayWidg
'           - sets maximum & minimum height of a list box
'         o fun097: hiddenSet_listBox_rayWidg
'           - sets a list box item to the hiden state
'         o fun098: selectSet_listBox_rayWidg
'           - sets a list box item to the select state
'         o fun099: specialSet_listBox_rayWidg
'           - sets a list box item to the special state
'         o fun100: canSelectSet_listBox_rayWidg
'           - sets a list box item to the can select state
'       # Sof08 Sec03 Sub03:
'         - listBox_rayWidg get variables value functions
'         o fun101: hiddenGet_listBox_rayWidg
'           - gets if a list box item is in hiden state
'         o fun102: selectGet_listBox_rayWidg
'           - sets a list box item to the select state
'         o fun103: specialGet_listBox_rayWidg
'           - sets a list box item to the special state
'         o fun104: canSelectGet_listBox_rayWidg
'           - sets a list box item to the can select state
'       # Sof08 Sec03 Sub04:
'         - list box widget create, draw, and event
'           handeling
'         o fun105: mk_listBox_rayWidg
'           - makes/adds list box to a widg_widgST struct
'         o fun106: addItem_listBox_rayWidg
'           - adds an item to a list box
'         o fun107: draw_listBox_rayWidg
'           - draws a list box widget
'         o fun108: listBoxEvent_rayWidg
'           - runs an event for a list box
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


   /*reset the color scheme to light mode*/
   widgSTPtr->guiColSI = def_white_rayWidg;
   widgSTPtr->focusColSI = def_black_rayWidg;

   widgSTPtr->textColSI = def_black_rayWidg;
   widgSTPtr->textAltColSI = def_white_rayWidg;

   widgSTPtr->widgColSI = def_lightGrey_rayWidg;
   widgSTPtr->borderColSI = def_darkGrey_rayWidg;
   widgSTPtr->pressColSI = def_darkGrey_rayWidg;
   widgSTPtr->pressBorderColSI = def_lightGrey_rayWidg;

   widgSTPtr->widgColSI = def_darkGrey_rayWidg;
   widgSTPtr->borderColSI = def_lightGrey_rayWidg;
   widgSTPtr->activeColSI = def_lightGrey_rayWidg;
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

   widgSTPtr->fontSTPtr = 0;
   widgSTPtr->freeFontBl = 0;
   widgSTPtr->fontSizeSI = 0;
   widgSTPtr->fontWidthF = 0;
   widgSTPtr->fontHeightF = 0;
   widgSTPtr->spacingF = 0;

   widgSTPtr->sizeSI = 0;

   blank_widg_rayWidg(widgSTPtr);
} /*init_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun004: setup_widg_rayWidg
|   - adds the default font to a rayWidg struct and sets
|     it up for use (only call once)
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add default font
|       to
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
   struct widg_rayWidg *widgSTPtr
){
   if(widgSTPtr->fontSTPtr && widgSTPtr->unloadFontBl)
      UnloadFont(*widgSTPtr->fontSTPtr);

   widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
   if(! widgSTPtr->fontSTPtr)
      goto memErr_fun004;
   widgSTPtr->freeFontBl = 1;

   *widgSTPtr->fontSTPtr = LoadFont_IbmPlexMono();
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 0;

   widgSTPtr->fontSizeSI = def_fontSize_rayWidg;
   widgSTPtr->spacingF = 1;
   fontMeasure_widg_rayWidg(widgSTPtr);

   return 0;

   memErr_fun004:;
      return 1;
} /*setup_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun005: freeStack_widg_rayWidg
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
| Fun006: freeHeap_widg_rayWidg
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
| Fun007: realloc_widg_rayWidg
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
   ' Fun007 TOC:
   '   o fun007 sec01:
   '     - variable declarations and initial check
   '   o fun007 sec02:
   '     - deal with no memory (get initial memory)
   '   o fun007 sec03:
   '     - have memory, need to reallocate
   '   o fun007 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun007 Sec01:
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
         goto noResize_fun007_sec04;
   } /*If: user wants auto resize*/

   if(sizeSI <= widgSTPtr->sizeSI)
      goto noResize_fun007_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun007 Sec02:
   ^   - deal with no memory (get initial memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->sizeSI <= 0)
   { /*If: no memory to begin with*/
      widgSTPtr->xArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->xArySI)
         goto memErr_fun007_sec04;
      widgSTPtr->yArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->yArySI)
         goto memErr_fun007_sec04;
      widgSTPtr->widthArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->widthArySI)
         goto memErr_fun007_sec04;
      widgSTPtr->heightArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->heightArySI)
         goto memErr_fun007_sec04;

      widgSTPtr->stateAryUS =
         malloc(sizeSI * sizeof(unsigned short));
      if(! widgSTPtr->stateAryUS)
         goto memErr_fun007_sec04;

      widgSTPtr->rowArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->rowArySI)
         goto memErr_fun007_sec04;
      widgSTPtr->colArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->colArySI)
         goto memErr_fun007_sec04;

      goto done_fun007_sec04;
   } /*If: no memory to begin with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun007 Sec03:
   ^   - have memory, need to reallocate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siSwapPtr =
      realloc(
         widgSTPtr->xArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->xArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->yArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->yArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->widthArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->widthArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->heightArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->heightArySI = siSwapPtr;

   ucSwapPtr =
      (unsigned char *)
      realloc(
         widgSTPtr->stateAryUS,
         sizeSI * sizeof(unsigned short)
      );
   if(! ucSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->stateAryUS = (unsigned short *) ucSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->rowArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->rowArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->colArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun007_sec04;
   widgSTPtr->colArySI = siSwapPtr;

   goto done_fun007_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun007 Sec04:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun007_sec04:;
      widgSTPtr->sizeSI = sizeSI;
      return 0;

   noResize_fun007_sec04:;
      return 0;

   memErr_fun007_sec04:;
      return 1;
} /*realloc_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun008: fontMeasure_widg_rayWidg
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
| Fun009: textMeasure_widg_rayWidg
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
| Fun010: checkTextWidth_rayWidg
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
\-------------------------------------------------------*/
void
checkTextWidth_rayWidg(
   signed char *textStr, /*text to check width of*/
   signed char *outStr,  /*gets modified text*/
   signed int widthSI,   /*width of text*/
   struct widg_rayWidg *widgSTPtr
){
   signed int textWidthSI = 0;
   signed int discardSI = 0;
   signed int indexSI = 0;
   signed char tmpSC = 0;

   while(textStr[indexSI])
   { /*Loop: copy text to outStr*/
      tmpSC = textStr[indexSI + 1];
      textStr[indexSI + 1] = 0;

      textWidthSI +=
         textMeasure_widg_rayWidg(
            &textStr[indexSI],
            &discardSI,
            widgSTPtr
         );
      textStr[indexSI + 1] = tmpSC;

      if(textWidthSI > widthSI)
      { /*If: hit end of text lenth*/
         outStr[indexSI--] = 0;

         if(indexSI >= 0)
            outStr[indexSI--] = '.';
         if(indexSI >= 0)
            outStr[indexSI--] = '.';
         if(indexSI >= 0)
            outStr[indexSI--] = '.';

         break;
      } /*If: hit end of text lenth*/

      else
         outStr[indexSI] = textStr[indexSI];
      ++indexSI;
   } /*Loop: copy text to outStr*/

   if(textWidthSI <= widthSI)
      outStr[indexSI] = 0;
} /*checkTextWidth_rayWidg*/

/*-------------------------------------------------------\
| Fun011: changeFont_widg_rayWidg
|   - adds a font from a file to a widg_rayWidg struct
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fileStr:
|     - file to load font from
|   o textSizeSI:
|     - text size to print in GUI
| Output:
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o fontSizeF in widgSTPtr to have the input font size
|     o unloadFontBl in widgSTPtr to 1 (safe to unload)
|     o freeFontBl in widgSTPtr to 1 (safe to free)
|     o spacingF in widgSTPtr to be 1
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
changeFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed char *fileStr,
   signed int fontSizeSI
){
   if(widgSTPtr->unloadFontBl && widgSTPtr->fontSTPtr)
      UnloadFont(*widgSTPtr->fontSTPtr);

   if(! widgSTPtr->freeFontBl)
   { /*If: need to get memory*/
      widgSTPtr->fontSTPtr = 0;
      widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
      if(! widgSTPtr->fontSTPtr)
         goto memErr_fun010;
      widgSTPtr->freeFontBl = 1;
   } /*If: need to get memory*/

   *widgSTPtr->fontSTPtr = LoadFont((char *) fileStr);
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 1;

   widgSTPtr->fontSizeSI = fontSizeSI;
   widgSTPtr->spacingF = 1;
   fontMeasure_widg_rayWidg(widgSTPtr);

   return 0;

   memErr_fun010:;
      return 1;
} /*changeFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun012: useEmbedFont_widg_rayWidg
|   - uses an embed font
|   - make this using ExprotFontAsCode from raylib
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontFunction:
|     - function generated by raylib's ExportFontAsCode
|   o textSizeSI:
|     - text size to print in GUI
| Output:
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o fontSizeF in widgSTPtr to have the input font size
|     o unloadFontBl in widgSTPtr to 1 (safe to unload)
|     o spacingF in widgSTPtr to be 1
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
useEmbedFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   struct Font (*fontFunction)(void),
      /*raylib auto generates this with ExportFontAsCode*/
   signed int fontSizeSI
){
   if(widgSTPtr->unloadFontBl && widgSTPtr->fontSTPtr)
      UnloadFont(*widgSTPtr->fontSTPtr);

   if(! widgSTPtr->freeFontBl)
   { /*If: need to get memory*/
      widgSTPtr->fontSTPtr = 0;
      widgSTPtr->fontSTPtr = calloc(1, sizeof(Font));
      if(! widgSTPtr->fontSTPtr)
         goto memErr_fun012;
      widgSTPtr->freeFontBl = 1;
   } /*If: need to get memory*/

   *widgSTPtr->fontSTPtr = (*fontFunction)();
      /*sadly raylib always assumes this succeded, so I
      `   have no way to check for failure
      */
   widgSTPtr->unloadFontBl = 0;
      /*this is hardcoded in the .h file*/

   widgSTPtr->fontSizeSI = fontSizeSI;
   widgSTPtr->spacingF = 1;
   fontMeasure_widg_rayWidg(widgSTPtr);

   return 0;

   memErr_fun012:;
      return 1;
} /*useEmbedFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun013: swapFont_widg_rayWidg
|   - swaps the font out with an input font pointer
|   - this is a swap of pointers only, so only free the
|     input font when you are finished
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontSTPtr:
|     - fontSTPtr pointer to swap to default font
|   o fontSizeSI:
|     - text size to print in GUI
| Output:
|   - Frees:
|     o old fontSTPtr (including unload if able) if
|       present and is able to free
|   - Modifies:
|     o fontSTPtr in widgSTPtr to have new font
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
|     o fontSizeF in widgSTPtr to have the input font size
|     o freeFontBl to 0 (unsafe/do not free fontSTPtr)
|     o unloadFontBl to 0 (unsafe/do not unload fontSTPtr)
|     o spacingF in widgSTPtr to be 1
\-------------------------------------------------------*/
void
swapFont_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   struct Font *newFontSTPtr,
   signed int fontSizeSI
){
   if(widgSTPtr->fontSTPtr && widgSTPtr->unloadFontBl)
      UnloadFont(*widgSTPtr->fontSTPtr);
   if(widgSTPtr->fontSTPtr && widgSTPtr->freeFontBl)
      free(widgSTPtr->fontSTPtr);
   widgSTPtr->fontSTPtr = 0;
   widgSTPtr->freeFontBl = 0;
   widgSTPtr->unloadFontBl = 0;

   widgSTPtr->fontSTPtr = newFontSTPtr;
   widgSTPtr->fontSizeSI = fontSizeSI;
   widgSTPtr->spacingF = 1;
   fontMeasure_widg_rayWidg(widgSTPtr);
} /*swapFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun014: setFontSize_widg_rayWidg
|   - changes font size
| Input:
|   o widgSTPtr:
|     - widg_rayWidg struct pointer to add font to
|   o fontSizeSI:
|     - font size to print in GUI
| Output:
|   - Modifies:
|     o fontSizeF in widgSTPtr to have the input font size
|     o fontWidthF in widgSTPtr to have width of "D"
|     o fontHeightF in widgSTptr to have height of "D"
\-------------------------------------------------------*/
void
setFontSize_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed int fontSizeSI
){
   widgSTPtr->fontSizeSI = fontSizeSI;
   fontMeasure_widg_rayWidg(widgSTPtr);
} /*setFontSize_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun015: setFontSpacing_widg_rayWidg
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
| Fun016: xCoordGet_widg_rayWidg
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
| Fun017: yCoordGet_widg_rayWidg
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
| Fun018: widthGet_widg_rayWidg
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
| Fun019: heightGet_widg_rayWidg
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
| Fun020: rowGet_widg_rayWidg
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
| Fun021: colGet_widg_rayWidg
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
| Fun022: stateGet_widg_rayWidg
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
| Fun023: pressGet_widg_rayWidg
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
| Fun024: checkedGet_widg_rayWidg
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
| Fun025: activeGet_widg_rayWidg
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
| Fun026: focusGet_widg_rayWidg
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
| Fun027: inactiveGet_widg_rayWidg
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
| Fun028: hidenGet_widg_rayWidg
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
| Fun029: childGet_widg_rayWidg
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
| Fun030: hogGet_widg_rayWidg
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
| Fun031: numWidgetGet_widg_rayWidg
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
| Fun032: focusWidgGet_widg_rayWidg
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
| Fun033: xCoordSet_widg_rayWidg
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
| Fun034: yCoordSet_widg_rayWidg
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
| Fun035: widthSet_widg_rayWidg
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
| Fun036: heightSet_widg_rayWidg
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
| Fun037: rowSet_widg_rayWidg
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
| Fun038: colSet_widg_rayWidg
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
| Fun039: stateSet_widg_rayWidg
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
| Fun040: pressAdd_widg_rayWidg
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
| Fun041: checkedAdd_widg_rayWidg
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
| Fun042: activeAdd_widg_rayWidg
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
| Fun043: focusAdd_widg_rayWidg
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
| Fun044: inactiveAdd_widg_rayWidg
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
| Fun045: hidenAdd_widg_rayWidg
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
| Fun046: childAdd_widg_rayWidg
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
| Fun047: hogAdd_widg_rayWidg
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
| Fun048: stateClear_widg_rayWidg
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
| Fun049: pressClear_widg_rayWidg
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
| Fun050: checkedClear_widg_rayWidg
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
| Fun051: activeClear_widg_rayWidg
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
| Fun052: focusClear_widg_rayWidg
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
| Fun053: inactiveClear_widg_rayWidg
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
| Fun054: hidenClear_widg_rayWidg
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
| Fun055: childClear_widg_rayWidg
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
| Fun056: hogClear_widg_rayWidg
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
| Fun057: blank_event_rayWidg
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

   eventSTPtr->parIdSI = -1;
   eventSTPtr->childIdSI = -1;

   eventSTPtr->leftPressBl = 0;
   eventSTPtr->leftReleaseBl = 0;

   eventSTPtr->scrollF = 0;
} /*blank_event_rayWidg*/

/*-------------------------------------------------------\
| Fun058: init_event_rayWidg
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
} /*init_event_rayWidg*/

/*-------------------------------------------------------\
| Fun059: freeStack_event_rayWidg
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
| Fun060: freeHeap_event_rayWidg
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
| Fun061: get_event_rayWidg
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
|   - eventSTPtr:
|     o event_rayWidg struct pointer to get user event
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with widgets
| Output:
|   - Modifies:
|     o all variables in eventSTPtr to hold a list of
|       events
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
   void (drawFunPtr(void *)),  /*function to redraw GUI*/
   void *guiStructPtr,          /*passed to drawFunPtr*/
   struct event_rayWidg *eventSTPtr, /*gets events*/
   struct widg_rayWidg *widgSTPtr    /*has widgets*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun061 TOC:
   '   - adds a user event to an event_rayWidg struct
   '   o fun061 sec01:
   '     - variable declare and blank events function
   '   o fun061 sec02:
   '     - get any key events
   '   o fun061 sec03:
   '     - get mouse events
   '   o fun061 sec04:
   '     - get scroll wheel events
   '   o fun061 sec05:
   '     - return event type
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun061 Sec01:
   ^   - variable declare and blank events function
   ^   o fun061 sec01:
   ^     - variable declarations and blanking
   ^   o fun061 sec02:
   ^     - get events and focused widget
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun061 Sec01:
   *   - variable declarations and blanking
   \*****************************************************/

   signed char eventBl = 0;
   signed int tmpSI = 0;
   signed int discardSI = 0;

   blank_event_rayWidg(eventSTPtr);

   /*****************************************************\
   * Fun061 Sec02:
   *   - get events and focused widget
   \*****************************************************/

   eventSTPtr->keySI = GetKeyPressed();

   eventSTPtr->xSI = GetMouseX();
   eventSTPtr->ySI = GetMouseY();

   eventSTPtr->leftReleaseBl =
      IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
   eventSTPtr->leftPressBl =
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

   eventSTPtr->scrollF = GetMouseWheelMove();


   eventSTPtr->parIdSI =
      childGet_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);
   if(eventSTPtr->parIdSI < 0)
      eventSTPtr->parIdSI = widgSTPtr->focusSI;
   else
      eventSTPtr->childIdSI = widgSTPtr->focusSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun061 Sec02:
   ^   - get any key events
   ^   o fun061 sec02 sub01:
   ^     - check if had key event + check for modifier key
   ^   o fun061 sec02 sub02:
   ^     - check tab keys
   ^   o fun061 sec02 sub03:
   ^     - check if enter key pressed and if setting state
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun061 Sec02 Sub02:
   *   - check if had key event + check for modifier keys
   \*****************************************************/

   if( IsKeyDown(KEY_LEFT_SHIFT) )
      eventSTPtr->shiftBl = 1;
   if( IsKeyDown(KEY_RIGHT_SHIFT) )
      eventSTPtr->shiftBl |= 2;
   if( IsKeyDown(KEY_CAPS_LOCK) )
   { /*If: user hit caps lock*/
      if(eventSTPtr->shiftBl)
         eventSTPtr->shiftBl = 0;
      else
         eventSTPtr->shiftBl |= 4;
   } /*If: user hit caps lock*/

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
      goto leftClick_fun061_sec03_sub01;

   /*****************************************************\
   * Fun061 Sec02 Sub02:
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

         goto focusKeyEvent_fun061_sec05;
      } /*If: user switched focus*/
   } /*If: user wanted tab switching*/

   /*****************************************************\
   * Fun061 Sec02 Sub03:
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

         goto enterKeyEvent_fun061_sec05;
      } /*If: widget is pressable*/
   } /*If: redrawing GUI on enter key presses*/

   goto retKeyEvent_fun061_sec05; /*key event*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun061 Sec03:
   ^   - get mouse events
   ^   o fun061 sec03 sub01:
   ^     - get mouse event and check if was mouse event
   ^   o fun061 sec03 sub02:
   ^     - find location of click & redraw GUI; if wanted
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun061 Sec03 Sub01:
   *   - get mouse event and check if was mouse event
   \*****************************************************/

   leftClick_fun061_sec03_sub01:;

   if(eventSTPtr->leftReleaseBl)
      eventBl = 1;

   if(eventSTPtr->leftPressBl)
      eventBl = 1;

   if(eventSTPtr->leftPressBl)
      ;
   else if(eventSTPtr->leftReleaseBl)
      ;
   else
      goto scrollCheck_fun061_sec04;

   /*****************************************************\
   * Fun061 Sec03 Sub02:
   *   - find location of click and redraw GUI (if wanted)
   \*****************************************************/

   eventSTPtr->childIdSI =
      getMouseWidg_widg_rayWidg(
         eventSTPtr->xSI,
         eventSTPtr->ySI,
         &eventSTPtr->parIdSI,
         widgSTPtr
      ); /*find widget that was clicked/released*/

   tmpSI = eventSTPtr->childIdSI;/*so can check if match*/

   if(eventSTPtr->childIdSI < 0)
      goto noEvent_fun061_sec05; /*can not click widget*/
   else if(eventSTPtr->parIdSI < 0)
   { /*Else If: not child*/
      eventSTPtr->parIdSI = eventSTPtr->childIdSI;
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
         == getMouseWidg_widg_rayWidg(
            GetMouseX(),
            GetMouseY(),
            &discardSI,
            widgSTPtr
         )
      ) goto clickReleaseEvent_fun061_sec05;

      else
         goto noEvent_fun061_sec05;
         /*user moved mouse of of the button*/
   } /*If: user wants to redraw until a release*/


   if(eventSTPtr->leftPressBl)
      goto clickPressEvent_fun061_sec05;
   else
      goto clickReleaseEvent_fun061_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun061 Sec04:
   ^   - get scroll wheel events
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   scrollCheck_fun061_sec04:;

   if(! ((signed int) eventSTPtr->scrollF) )
      goto noEvent_fun061_sec05;
   else
      goto scrollEvent_fun061_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun061 Sec05:
   ^   - return event type
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
   
   noEvent_fun061_sec05:; /*also event that does nothing*/
      eventBl = 0;
      goto ret_fun061_sec05;

   retKeyEvent_fun061_sec05:;
      eventBl = def_keyEvent_rayWidg;
      goto ret_fun061_sec05;

   enterKeyEvent_fun061_sec05:;
      eventBl =
          def_releaseEvent_rayWidg | def_keyEvent_rayWidg;
      goto ret_fun061_sec05;

   focusKeyEvent_fun061_sec05:;
      eventBl = def_focus_rayWidg | def_keyEvent_rayWidg;
      goto ret_fun061_sec05;

   clickPressEvent_fun061_sec05:;
      eventBl =
            def_press_rayWidg | def_clickEvent_rayWidg;
      goto ret_fun061_sec05;

   clickReleaseEvent_fun061_sec05:;
      eventBl =
            def_releaseEvent_rayWidg
          | def_clickEvent_rayWidg;
      goto ret_fun061_sec05;

   scrollEvent_fun061_sec05:;
      eventBl = def_scrollEvent_rayWidg;
      goto ret_fun061_sec05;

   ret_fun061_sec05:;
      return eventBl;
} /*get_event_rayWidg*/

/*-------------------------------------------------------\
| Fun062: getDarkModeState_rayWidg
|   - detects if Mac or Windows computer is in dark mode
| Input:
| Output:
|   - Returns:
|     o def_darkMode_rayWidg for dark mode
|     o def_lightMode_rayWidg for light mode
|     o def_unkownMode_rayWidg if could not decide
| Note:
|   - this is from github co-pilot (chat gpt)
\-------------------------------------------------------*/
signed char
getDarkModeState_rayWidg(
   void
){
   #ifdef MAC
      #ifdef NO_DARK_MODE_CHECK
         return def_unkownMode_rayWidg;
      #else
      @autoreleasepool {
         NSString *style =
            [
               [NSUserDefaults standardUserDefaults]
               stringForKey:@"AppleInterfaceStyle"
            ];
         if(! style)
            goto retUnkown_fun001;
         else if (
               [style caseInsensitiveCompare:@"Dark"]
            == NSOrderedSame
         ) goto retDark_fun001;
         else if (
         else
            goto retLight_fun001;
       } /*autoreleasepool block from objective C*/

       goto retUnkown_fun001;
          /*could not get dark/light mode*/

       retUnkown_fun001:;
          return def_unkownMode_rayWidg;
       retLight_fun001:;
          return def_lightMode_rayWidg;
       retDark_fun001:;
          return def_darkMode_rayWidg;
      #endif
   #else
   #ifdef WINDOWS
      #ifdef NO_DARK_MODE_CHECK
         return def_unkownMode_rayWidg;
      #else
      HKEY hKey;
      DWORD value = 1;
      DWORD valueSize = sizeof(value);

      /*Open the registry key*/
      if(
         RegOpenKeyExA(
            HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
            0,
            KEY_READ,
            &hKey
         ) == ERROR_SUCCESS
      ){ /*If: can open theme registory*/
          if(
             RegQueryValueExA(
                hKey,
                "AppsUseLightTheme",
                NULL,
                NULL,
                (LPBYTE)&value,
                &valueSize
              ) == ERROR_SUCCESS
          ){ /*If: could get dark mode status*/
              RegCloseKey(hKey);

              if (value == 0)
                 goto retDark_fun001;
              else
                 goto retLight_fun001;
          }  /*If: could get dark mode status*/

          RegCloseKey(hKey);
      } /*If: can open theme registory*/

      goto retUnkown_fun001;
         /*could not get dark/light mode*/

      retUnkown_fun001:;
         return def_unkownMode_rayWidg;
      retLight_fun001:;
         return def_lightMode_rayWidg;
      retDark_fun001:;
         return def_darkMode_rayWidg;
      #endif
   #else
      return def_unkownMode_rayWidg;
   #endif /*windows check*/
   #endif /*mac check*/
} /*getDarkModeState_rayWidg*/

/*-------------------------------------------------------\
| Fun063: checkGuiColorMode_rayWidg
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
   /*TODO: need elevation change and border shadow*/
   if(getDarkModeState_rayWidg() == def_darkMode_rayWidg)
   { /*If: dark mode is set*/
      widgSTPtr->guiColSI = def_black_rayWidg;
      widgSTPtr->focusColSI = def_white_rayWidg;

      widgSTPtr->textColSI = def_white_rayWidg;
      widgSTPtr->textAltColSI = def_lightGrey_rayWidg;

      widgSTPtr->widgColSI = def_darkGrey_rayWidg;
      widgSTPtr->borderColSI = def_lightGrey_rayWidg;
      widgSTPtr->pressColSI = def_darkGrey_rayWidg;
      widgSTPtr->pressBorderColSI =
         def_lightGrey_rayWidg;

      widgSTPtr->widgColSI = def_darkGrey_rayWidg;
      widgSTPtr->borderColSI= def_lightGrey_rayWidg;
      widgSTPtr->activeColSI= def_black_rayWidg;
      widgSTPtr->pressBorderColSI =
         def_lightGrey_rayWidg;
   } /*If: dark mode is set*/

   else
   { /*Else: light mode is set*/
      widgSTPtr->guiColSI = def_white_rayWidg;
      widgSTPtr->focusColSI = def_black_rayWidg;

      widgSTPtr->textColSI = def_black_rayWidg;
      widgSTPtr->textAltColSI = def_white_rayWidg;

      widgSTPtr->widgColSI = def_lightGrey_rayWidg;
      widgSTPtr->borderColSI = def_darkGrey_rayWidg;
      widgSTPtr->pressColSI = def_darkGrey_rayWidg;
      widgSTPtr->pressBorderColSI =
         def_lightGrey_rayWidg;

      widgSTPtr->widgColSI = def_lightGrey_rayWidg;
      widgSTPtr->borderColSI = def_darkGrey_rayWidg;
      widgSTPtr->pressBorderColSI = def_lightGrey_rayWidg;
      widgSTPtr->activeColSI = def_darkGrey_rayWidg;
   } /*Else: light mode is set*/
} /*checkGuiColorMode_rayWidg*/

/*-------------------------------------------------------\
| Fun064: addWidget_widg_rayWidg
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
      goto memErr_fun063; /*memory error while resizing*/
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

   memErr_fun063:;
      return 1;
} /*addWidget_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun065: tile_widg_rayWidg
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
   ' Fun065 TOC:
   '   - find tiling x,y coordiantes for each widget using
   '     widths, heights, rows, and columns
   '   o fun065 sec01:
   '     - variable declarations
   '   o fun065 sec02:
   '     - get maximum rows and column (and get memory)
   '   o fun065 sec03:
   '     - find parent widget row and column coordinates
   '   o fun065 sec04:
   '     - add x,y to tiled parents and update non-tiled
   '       child widgets
   '   o fun065 sec05:
   '     - find tile coordinates for tiled child widgets
   '   o fu10 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun065 Sec01:
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun065 Sec02:
   ^   - get maximum rows and column (and get memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(rowOffsetSI < 0)
   { /*If: need to offset off first row*/
      rowOffsetSI =
          widgSTPtr->fontHeightF
        / def_widgHeightGap_rayWidg;
      /*rowOffsetSI += widgSTPtr->fontHeightF / 4;*/
   } /*If: need to offset off first row*/

   if(colOffsetSI < 0)
      colOffsetSI = widgSTPtr->fontWidthF;

   rowSizeSI = -1;
   colSizeSI = -1;

   for(idSI = 0; idSI < widgSTPtr->lenSI; ++idSI)
   { /*Loop: find number of rows and columns*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         continue; /*is non-tile widget*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         continue; /*is non-tile widget*/

      if(rowSizeSI < widgSTPtr->rowArySI[idSI])
         rowSizeSI = widgSTPtr->rowArySI[idSI];
      if(colSizeSI < widgSTPtr->colArySI[idSI])
         colSizeSI = widgSTPtr->colArySI[idSI];
      tileBl = 1;
   } /*Loop: find number of rows and columns*/

   if(! tileBl)
      goto noTile_fun065_sec06;

   ++rowSizeSI;
   ++colSizeSI;

   rowHeapArySI = calloc(rowSizeSI, sizeof(signed int));
   if(! rowHeapArySI)
      goto memErr_fun065_sec06;
   colHeapArySI = calloc(colSizeSI, sizeof(signed int));
   if(! colHeapArySI)
      goto memErr_fun065_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun065 Sec03:
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
      rowHeapArySI[siRow] += rowOffsetSI;
   } /*Loop: find y coordinate of each row*/

   xChangeSI = colHeapArySI[0];
   colHeapArySI[0] = colOffsetSI;

   for(siCol = 1; siCol <= colLenSI; ++siCol)
   { /*Loop: find x coordinate of each row*/
      colHeapArySI[siCol] ^= xChangeSI;
      xChangeSI ^= colHeapArySI[siCol];
      colHeapArySI[siCol] ^= xChangeSI;

      colHeapArySI[siCol] += colHeapArySI[siCol - 1];
      colHeapArySI[siCol] += colOffsetSI;
   } /*Loop: find x coordinate of each row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ fun065 sec04:
   ^   - add x,y to tiled parents and update non-tiled
   ^     child widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: add in x,y coordaintes*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto nextWidg_fun065_sec04;
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto nextWidg_fun065_sec04;
      else if(
         widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ) goto nextWidg_fun065_sec04;

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
             goto nextChild_fun065_sec04;
          else if(widgSTPtr->colArySI[idSI] >= 0)
             goto nextChild_fun065_sec04;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] += yChangeSI;
         widgSTPtr->xArySI[idSI] += xChangeSI;

         nextChild_fun065_sec04:;
            ++idSI;
      }  /*Loop: update x,y coord child coordinates*/

      continue; /*already on next parent wiget*/

      nextWidg_fun065_sec04:;
         ++idSI;
   } /*Loop: add in x,y coordaintes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun065 Sec05:
   ^   - find tile coordinates for tiled child widgets
   ^   o fun065 sec05 sub01:
   ^     - find and setup for found child group
   ^   o fun065 sec05 sub02:
   ^     - find column and row sizes in child group
   ^   o fun065 sec05 sub03:
   ^     - add x,y coordinates in child group
   ^   o fun065 sec05 sub04:
   ^     - move to next widget
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun065 Sec05 Sub01:
   *   - find and setup for found child group
   \*****************************************************/

   rowLenSI = 0;
   colLenSI = 0;
   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: find tile dimensions*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto childNextWidg_fun065_sec05_sub05;
         /*non-tiled*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto childNextWidg_fun065_sec05_sub05;
         /*non-tiled*/
      else if(
        !(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
      ) goto childNextWidg_fun065_sec05_sub05;
        /*this is a parent*/

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
      * Fun065 Sec05 Sub02:
      *   - find column and row sizes in child group
      \**************************************************/

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: find child widget row and column sizes*/

         if(widgSTPtr->rowArySI[idSI] < 0)
            goto nextChild_fun065_sec05_sub02;/*non-tiled*/
         else if(widgSTPtr->colArySI[idSI] < 0)
            goto nextChild_fun065_sec05_sub02;/*non-tiled*/

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

         nextChild_fun065_sec05_sub02:;
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
      * Fun065 Sec05 Sub03:
      *   - add x,y coordinates in child group
      \**************************************************/

      idSI = tmpSI;

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: set tiled child wiget x,y coordinates*/

          /*check if children were non-tile widgets*/
          if(widgSTPtr->rowArySI[idSI] < 0)
             goto addCoordNextChild_fun065_sec05_sub03;
          else if(widgSTPtr->colArySI[idSI] < 0)
             goto addCoordNextChild_fun065_sec05_sub03;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] = rowHeapArySI[siRow];
         widgSTPtr->xArySI[idSI] = colHeapArySI[siCol];

         addCoordNextChild_fun065_sec05_sub03:;
            ++idSI;
      }  /*Loop: set tiled child wiget x,y coordinates*/

      /**************************************************\
      * Fun065 Sec05 Sub04:
      *   - move to next widget
      \**************************************************/

      continue; /*on next parent widget*/

      childNextWidg_fun065_sec05_sub05:;
         ++idSI;
   } /*Loop: find tile dimensions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fu10 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;
   goto ret_fun065_sec06;

   noTile_fun065_sec06:;
      idSI = 0;
      goto ret_fun065_sec06;

   memErr_fun065_sec06:;
      idSI = 1;
      goto ret_fun065_sec06;

   ret_fun065_sec06:;
      if(rowHeapArySI)
         free(rowHeapArySI);
      rowHeapArySI = 0;

      if(colHeapArySI)
         free(colHeapArySI);
      colHeapArySI = 0;

      return idSI;
} /*tile_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun066: focusCheck_widg_rayWidg
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
   ' Fun066 TOC:
   '   - detect if input key was tab and update focus
   '   o fun066 sec01:
   '     - find if focus and set focus
   '   o fun066 sec02:
   '     - check if next widget can be focused, if not
   '       find the next widget that can be focused
   '   o fun066 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun066 Sec01:
  ^   - find if focus and set focus
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  signed char endBl = 0;   /*mark if hit end of widgets*/
  signed char shiftBl = 0; /*if shift key was pressed*/
     /*not using alt tab, because alt tab is often a
     `  system hotkey for reverse cycle
     */
  signed char hogBl = 0;

  shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
  shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

  if(charBl)
  { /*If: character input*/
     if(keySI == '\t')
        charBl = shiftBl;
     else
        goto nonFocusKey_fun066_sec04;
  } /*If: character input*/

  else
  { /*Else: key event input*/
     if(keySI == KEY_TAB)
        charBl = shiftBl;
     else
        goto nonFocusKey_fun066_sec04;
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
  ^ Fun066 Sec02:
  ^   - check if next widget can be focused, if not find
  ^     the next widget that can be focused
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  /*check if next widget can be focused*/
  keySI = widgSTPtr->focusSI;

  if(
       hogGet_widg_rayWidg(
          widgSTPtr->focusSI,
          widgSTPtr
       ) == 1
  ){ /*If: on a hog widget*/
     hogBl = 1;

     if(widgSTPtr->stateAryUS[keySI] & def_child_rayWidg)
     { /*If: is a parent widget*/
        if(charBl)
           goto resetState_fun066_sec02;
           /*going backwards to other widgets*/
        else if(
            widgSTPtr->stateAryUS[keySI + 1]
          & def_child_rayWidg
        ) goto resetState_fun066_sec02; /*no children*/
        else
           ++keySI; /*children to check*/
     }  /*If: is a parent widget*/
  }  /*If: on a hog widget*/

  do { /*Loop: find next widget in focus*/
     if(charBl)
     { /*If: moving focus up*/
        --keySI;

        if(keySI < 0)
        { /*If: need to move to start*/
           if(endBl)
              goto noFocus_fun066_sec04;
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
              goto noFocus_fun066_sec04;
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
        goto resetState_fun066_sec02;
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

  resetState_fun066_sec02:;
     widgSTPtr->focusSI = keySI;
     widgSTPtr->stateAryUS[keySI] |= def_focus_rayWidg;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun066 Sec03:
  ^   - return
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  if(widgSTPtr->focusSI >= 0)
     activeAdd_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);

  return keySI;

  noFocus_fun066_sec04:;
     widgSTPtr->focusSI = -1;
     return -1;
  nonFocusKey_fun066_sec04:;
      return -2;
} /*focusCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun067: enterCheck_widg_rayWidg
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
         goto noPress_fun067;
   } /*If: character input*/

   else
   { /*Else: key event input*/
      if(keySI == KEY_ENTER)
         charBl = 1;
      else
         goto noPress_fun067;
   } /*Else: key event input*/


   /*check if widget is pressable*/
   keySI = widgSTPtr->focusSI;
   if(keySI < 0)
      goto noFocus_fun067;
   else if(keySI >= widgSTPtr->lenSI)
      goto noFocus_fun067;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_hiden_rayWidg
   ) goto noFocus_fun067;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_inactive_rayWidg
   ) goto noFocus_fun067;


   pressAdd_widg_rayWidg(keySI, widgSTPtr);

   *parSIPtr =
      childGet_widg_rayWidg(keySI, widgSTPtr);

   if(*parSIPtr == -3)
      goto widgetErr_fun067;
   else if(*parSIPtr < 0)
      *parSIPtr = keySI; /*not a child widget*/

   return keySI;

   noFocus_fun067:;
      return -1;
   noPress_fun067:;
      return -2;
   widgetErr_fun067:;
      return -3;
} /*enterCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun068: getMouseWidg_widg_rayWidg
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
   ' Fun068 TOC:
   '   - get widget mouse was on
   '   o fun068 sec01:
   '     - find first parent widget the mouse coud be on
   '   o fun068 sec02:
   '     - check if there is a child widget the mouse
   '       could be on
   '   o fun068 sec03:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec01:
   ^   - find first parent widget the mouse coud be on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int retSI = 0;
   signed int tmpSI = 0;

   for(
      retSI = 0;
      retSI < widgSTPtr->lenSI;
      ++retSI
   ){ /*Loop: find first widget that could match*/
      if(xSI < widgSTPtr->xArySI[retSI])
         continue;

      else if(ySI < widgSTPtr->yArySI[retSI])
         continue;

      else if(
           widgSTPtr->stateAryUS[retSI]
         & def_hiden_rayWidg
      ) continue;

      else if(
           widgSTPtr->stateAryUS[retSI]
         & def_inactive_rayWidg
      ) continue;

      else if(
           xSI
         > widgSTPtr->xArySI[retSI]
           + widgSTPtr->widthArySI[retSI]
      ) continue;

      else if(
           ySI
         > widgSTPtr->yArySI[retSI]
           + widgSTPtr->heightArySI[retSI]
      ) continue;

      break; /*found widget*/
   }  /*Loop: find first widget that could match*/

   if(retSI >= widgSTPtr->lenSI)
      goto noWidg_fun068_sec03_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec02:
   ^   - check if there is a child widget the mouse could
   ^     be on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *parSIPtr =
      childGet_widg_rayWidg(retSI, widgSTPtr);

   if(*parSIPtr < -1)
      goto noWidg_fun068_sec03_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun068 Sec03:
   ^   - return results
   ^   o fun068 sec03 sub01:
   ^     - clear old focus state
   ^   o fun068 sec03 sub02:
   ^     - handle hog widget state checks
   ^   o fun068 sec03 sub03:
   ^     - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun068 Sec03 Sub01:
   *   - clear old focus state
   \*****************************************************/

   goto retWidg_fun068_sec03_sub01;

   retWidg_fun068_sec03_sub01:;
      pressClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear a press event (might be one)*/
      activeClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear the old active event*/

      /**************************************************\
      * Fun068 Sec03 Sub02:
      *   - handle hog widget state checks
      \**************************************************/

      if(widgSTPtr->focusSI < 0)
         ; /*nothing is in focus*/
      else if(
           widgSTPtr->stateAryUS[widgSTPtr->focusSI]
         & def_hog_rayWidg
      ){ /*Else If: moving from a hog state widget*/
         if(
             widgSTPtr->stateAryUS[widgSTPtr->focusSI]
           & def_hiden_rayWidg
         ) goto setRet_fun068_sec03_sub03;

         tmpSI = retSI;

         if(tmpSI > widgSTPtr->focusSI)
         { /*If: user clicked on a next widget*/
            while(tmpSI > widgSTPtr->focusSI)
            { /*Loop: check if have child widget path*/
                if(
                    widgSTPtr->stateAryUS[tmpSI]
                  & def_child_rayWidg
                ) --tmpSI;

                else
                   break;
            } /*Loop: check if have child widget path*/

            if(tmpSI == widgSTPtr->focusSI)
               goto setRet_fun068_sec03_sub03;
            else
               goto hogNoMove_fun068_sec03_sub03;
         } /*If: user clicked on a next widget*/

         else
         { /*Else: moving backwards*/
            if(
              !(
                  widgSTPtr->stateAryUS[tmpSI]
                & def_child_rayWidg
              )
            ) ++tmpSI;

            while(tmpSI < widgSTPtr->focusSI)
            { /*Loop: check if have child widget path*/
                if(
                    widgSTPtr->stateAryUS[tmpSI]
                  & def_child_rayWidg
                ) ++tmpSI;

                else
                   break;
            } /*Loop: check if have child widget path*/

            if(tmpSI == widgSTPtr->focusSI)
               goto setRet_fun068_sec03_sub03;
            else
               goto hogNoMove_fun068_sec03_sub03;
         } /*Else: moving backwards*/
      }  /*Else If: moving from a hog state widget*/

      /**************************************************\
      * Fun068 Sec03 Sub03:
      *   - return results
      \**************************************************/

      setRet_fun068_sec03_sub03:;
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

      hogNoMove_fun068_sec03_sub03:;
         retSI = widgSTPtr->focusSI;
         focusAdd_widg_rayWidg(retSI, widgSTPtr);
            /*set clicked widget to focused widget*/
         activeAdd_widg_rayWidg(retSI, widgSTPtr);
            /*make into an active widget*/
         *parSIPtr = -1;
         return -1;

   noWidg_fun068_sec03_sub03:;
      *parSIPtr = -1;
      return -1;
} /*getMouseWidg_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun069: blinkGet_rayWidg
|   - returns current state of cursor blink for widgets
| Input:
|   - blinkSI:
|     o current interval blink is on
|   - intervalSI:
|     o how many cycles in one blink
|       * ex: 10 frame redraws
|   - timeSI:
|     o how many cycles the blink will last
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
   signed int timeSI      /*number cycles to blink*/
){
   timeSI = intervalSI - timeSI; /*get number of cycles*/

   if(blinkSI % intervalSI >= timeSI)
      return 1;
   else
      return 0;
} /*blinkGet_rayWidg*/

/*-------------------------------------------------------\
| Fun070: getLines_rayWidg
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
| Fun071: lineToHeight_rayWidg
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
| Fun072: intStrCheck_rayWidg
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
      goto ret_fun073;

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

   ret_fun073:;
      return dupStr - textStr;
} /*intStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun073: floatStrCheck_rayWidg
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
|   - maxF:
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
floatStrCheck_rayWidg(
   signed char *textStr,/*text with number*/
   signed int cursorSI, /*position of cursor*/
   float maxF           /*maximum value allowed*/
){
   signed char *cpStr = textStr;
   signed char *dupStr = textStr;
   signed char rmCursorBl = 0;
   signed char decBl = 0;
   float tmpF = 0;

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
      goto ret_fun073;

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

   ret_fun073:;
      return dupStr - textStr;
} /*floatStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun074: fileStrCheck_rayWidg
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
| Fun075: addCharToEntry_rayWidg
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
   ' Fun075 TOC:
   '   - adds a character to an entry box text
   '   o fun075 sec01:
   '     - variable declaration and movement keys
   '   o fun075 sec02:
   '     - delete key
   '   o fun075 sec03:
   '     - backspace key
   '   o fun075 sec04:
   '     - keys that need a conversion
   '   o fun075 sec05:
   '     - normal keys
   '   o fun075 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec01:
   ^   - variable declaration and movement keys
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int posSI = posArySI[1];
   Vector2 textDimVect2;
   signed int lenSI = 0;   /*find final output length*/
   signed char shiftBl = 0;

   /*for measuring final string width*/
   signed char swapArySC[3] = {0, 0, 0};
   signed char rmArySC[2] = {0, 0};
   signed int textWidthSI = 0;

   shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
   shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

   if(keySI == KEY_LEFT)
   { /*If: move left*/
      if(posArySI[1] > 0)
         --posArySI[1];
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      goto cursorMove_fun075_sec06;
   } /*If: move left*/

   else if(keySI == KEY_RIGHT)
   { /*Else If: move left*/
      if(posArySI[1] < *lenSIPtr)
         ++posArySI[1];
      goto cursorMove_fun075_sec06;
   } /*Else If: move left*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec02:
   ^   - delete key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_DELETE)
   { /*Else If: deleting the next character*/
      if(*lenSIPtr <= 0)
         goto cursorMove_fun075_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] >= *lenSIPtr)
         goto cursorMove_fun075_sec06;
         /*at end of string; nothing to delete*/

      else if(posArySI[1] == *lenSIPtr - 1)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1]] = 0;
         --*lenSIPtr;
         goto delete_fun075_sec06;
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
         goto delete_fun075_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: deleting the next character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec03:
   ^   - backspace key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_BACKSPACE)
   { /*Else If: backspace to delete last character*/
      if(posArySI[1] <= 0)
         goto cursorMove_fun075_sec06;
         /*at start of string; nothing to delete*/

      else if(*lenSIPtr <= 0)
         goto cursorMove_fun075_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] == *lenSIPtr)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1] - 1] = 0;
         --posArySI[1];
         --*lenSIPtr;
         goto delete_fun075_sec06;
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
         goto delete_fun075_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: backspace to delete last character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec04:
   ^   - keys that need a conversion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_END)
   { /*Else If: move cursor to end of text*/
      posArySI[1] = *lenSIPtr;
      goto cursorMove_fun075_sec06;
   } /*Else If: move cursor to end of text*/

   else if(keySI == KEY_HOME)
   { /*Else If: move cursor to start of text*/
      posArySI[0] = 0;
      posArySI[1] = 0;
      goto cursorMove_fun075_sec06;
   } /*Else If: move cursor to start of text*/

   else if(keySI == KEY_KP_0)
   { /*Else If: key pad 0*/
      keySI = '0';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 0*/

   else if(keySI == KEY_KP_1)
   { /*Else If: key pad 1*/
      keySI = '1';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 1*/

   else if(keySI == KEY_KP_2)
   { /*Else If: key pad 2*/
      keySI = '2';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 2*/

   else if(keySI == KEY_KP_3)
   { /*Else If: key pad 3*/
      keySI = '3';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 3*/

   else if(keySI == KEY_KP_4)
   { /*Else If: key pad 4*/
      keySI = '4';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 4*/

   else if(keySI == KEY_KP_5)
   { /*Else If: key pad 5*/
      keySI = '5';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 5*/

   else if(keySI == KEY_KP_6)
   { /*Else If: key pad 6*/
      keySI = '6';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 6*/

   else if(keySI == KEY_KP_7)
   { /*Else If: key pad 7*/
      keySI = '7';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 7*/

   else if(keySI == KEY_KP_8)
   { /*Else If: key pad 8*/
      keySI = '8';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 8*/

   else if(keySI == KEY_KP_9)
   { /*Else If: key pad 9*/
      keySI = '9';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad 9*/

   else if(keySI == KEY_KP_DECIMAL)
   { /*Else If: key pad .*/
      keySI = '.';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad .*/

   else if(keySI == KEY_KP_DIVIDE)
   { /*Else If: key pad /*/
      keySI = '/';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad /*/

   else if(keySI == KEY_KP_MULTIPLY)
   { /*Else If: key pad **/
      keySI = '*';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad **/

   else if(keySI == KEY_KP_SUBTRACT)
   { /*Else If: key pad -*/
      keySI = '-';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad -*/

   else if(keySI == KEY_KP_ADD)
   { /*Else If: key pad +*/
      keySI = '+';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad +*/

   else if(keySI == KEY_KP_EQUAL)
   { /*Else If: key pad =*/
      keySI = '=';
      goto addNormalKey_fun075_sec05;
   } /*Else If: key pad =*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec05:
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
         
   
      addNormalKey_fun075_sec05:;

      if(posArySI[1] >= *lenSIPtr)
      { /*If: at end of string*/
         textStr[posArySI[1]] = keySI;
         ++posArySI[1];
         ++*lenSIPtr;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun075_sec06;
      } /*If: at end of string*/

      else
      { /*Else: need to insert the new character*/
         for(posSI=*lenSIPtr;posSI > posArySI[1];--posSI)
            textStr[posSI] = textStr[posSI - 1];

         ++posArySI[1];
         ++*lenSIPtr;
         textStr[posSI] = keySI;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun075_sec06;
      } /*Else: need to insert the new character*/
   } /*Else If: normal key pressed*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun075 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cursorMove_fun075_sec06;
      /*no idea what the key event is*/

   delete_fun075_sec06:;
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      posSI = -1;
      goto ret_fun075_sec06;

   addChar_fun075_sec06:;
      posSI = 1;
      goto adjustPos_fun075_sec06;

   cursorMove_fun075_sec06:;
      posSI = 0;
      goto adjustPos_fun075_sec06;

   adjustPos_fun075_sec06:;
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

      textDimVect2 =
         MeasureTextEx(
            *widgSTPtr->fontSTPtr,
            (char *) &textStr[posArySI[0]],
            widgSTPtr->fontSizeSI,
            widgSTPtr->spacingF
         );

      textWidthSI = textDimVect2.x;

      while(textWidthSI > widthSI)
      { /*Loop: find text length*/
         if(posArySI[0] == posArySI[1])
            break; /*width only allows one character*/

         rmArySC[0] = textStr[posArySI[0]];
         ++posArySI[0];
            /*cursor moved out of window, so need to
            `  move window one forward
            */
         textDimVect2 =
            MeasureTextEx(
               *widgSTPtr->fontSTPtr,
               (char *) rmArySC,
               widgSTPtr->fontSizeSI,
               widgSTPtr->spacingF
            );
         textWidthSI -= textDimVect2.x;
      } /*Loop: find text length*/

      textStr[posArySI[1]] = swapArySC[0];
      textStr[posArySI[1] + 1] = swapArySC[1];
      textStr[posArySI[1] + 2] = swapArySC[2];

      goto ret_fun075_sec06;

   ret_fun075_sec06:;
      textStr[*lenSIPtr] = 0;
      return (signed char) posSI;
} /*addCharToEntry_rayWidg*/

/*-------------------------------------------------------\
| Fun076: checkExt_rayWidg
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
|   - extLenSI:
|     o number of file extensions looking for
|   - extIndexSI:
|     o index of extension using
| Output:
|   - Returns:
|     o 2 if was directory and is supported
|     o 1 if fileStr has a supported extension
|     o 0 if fileStr does not have the extension
|     o -1 if not a file or directory
\-------------------------------------------------------*/
signed char
checkExt_rayWidg(
   signed char *fileStr, /*file to check extension of*/
   signed char *extAryStr[], /*file extensions*/
   signed int extLenSI,  /*number of extensions*/
   signed int extIndexSI /*index of extension using*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun076 TOC:
   '   - checks if file is the correct input extension
   '   o fun076 sec01:
   '     - variable declarations & check if have file/dir
   '   o fun076 sec02:
   '     - handle any file case
   '   o fun076 sec03:
   '     - handle directory case
   '   o fun076 sec04:
   '     - handle a single extension case
   '   o fun076 sec05:
   '     - handle all possible file extensions case
   '   o fun076 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec01:
   ^   - variable declarations and check if have file/dir
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char fileBl = 0;
   signed int lenSI = 0;
   signed int extPosSI = 0;

   if( FileExists((char *) fileStr) )
      fileBl = 1;
   else if( DirectoryExists((char *) fileStr) )
      fileBl = 0;
   else
      goto noFile_fun076_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec02:
   ^   - handle any file case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(extAryStr[extIndexSI][0] == '*')
   { /*If: could be any file*/
      if(fileBl)
         goto match_fun076_sec06;
         /*is a file, not a directory*/
      else if(extAryStr[extIndexSI][1] == 'd')
         goto matchDir_fun076_sec06;
         /*any file/directory works*/
      else
         goto noMatch_fun076_sec06; /*is directory*/
   } /*If: could be any file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec03:
   ^   - handle directory case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(
         extAryStr[extIndexSI][0] == 'd'
      && extAryStr[extIndexSI][1] == 'i'
      && extAryStr[extIndexSI][2] == 'r'
      && ! extAryStr[extIndexSI][3]
   ){ /*Else If: user wanted a directory*/
      if(fileBl)
         goto noMatch_fun076_sec06; /*is a file*/
      else
         goto matchDir_fun076_sec06; /*is a directory*/
   }  /*Else If: user wanted a directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec04:
   ^   - handle a single extension case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(extAryStr[extIndexSI][0] != '+')
   { /*Else If: this is a spefic file type*/
      lenSI = endStr_ulCp(fileStr);
      --lenSI;
      extPosSI = endStr_ulCp(fileStr) - 1;

      while(
          lenSI >= 0
       && extPosSI >= 0
       && fileStr[lenSI] ==extAryStr[extIndexSI][extPosSI]
     ){ /*Loop: see if extension matches*/
        --lenSI;
        --extPosSI;
     }  /*Loop: see if extension matches*/

     if(extPosSI < 0)
        goto match_fun076_sec06; /*file extensions matched*/
     else
        goto noMatch_fun076_sec06;
        /*file extensions did not matched*/
   } /*Else If: this is a spefic file type*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec05:
   ^   - handle all possible file extensions case
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
   { /*Else: have multiple extension to check*/
      for(
         extIndexSI = 0;
         extIndexSI < extLenSI;
         ++extIndexSI
      ){ /*Loop: check if file matches any extensions*/
         if(extAryStr[extIndexSI][0] == '*')
            continue; /*not a specific file extension*/

         else if(extAryStr[extIndexSI][0] == '+')
            continue; /*not a specific file extension*/

         else if(
               extAryStr[extIndexSI][0] == 'd'
            && extAryStr[extIndexSI][1] == 'i'
            && extAryStr[extIndexSI][2] == 'r'
            && ! extAryStr[extIndexSI][3]
         ){ /*Else If: directories are supported*/
            if(! fileBl)
               goto matchDir_fun076_sec06;
            else
               continue; /*is file not a directory*/
         }  /*Else If: directories are supported*/

         else if(! fileBl)
            continue; /*is directory not file*/

         else
         { /*Else: specific file extension*/
            lenSI = endStr_ulCp(fileStr) - 1;
            extPosSI = endStr_ulCp(fileStr) - 1;

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
               goto match_fun076_sec06;
               /*extensions matched*/
         } /*Else: specific file extension*/
      }  /*Loop: check if file matches any extensions*/

      /*if I hit this point, then no extensions matched*/
      goto noMatch_fun076_sec06;
   } /*Else: have multiple extension to check*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun076 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   match_fun076_sec06:;
      return 1;
   matchDir_fun076_sec06:;
      return 2;
   noMatch_fun076_sec06:;
      return 0;
   noFile_fun076_sec06:;
      return -1;
} /*checkExt_rayWidg*/

/*-------------------------------------------------------\
| Fun077: drawRectByCoord_rayWidg
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
| Output:
|   - Returns:
|     o width of widget (without borders) for success
|     o 0 if widget is hidden
|     o def_noWidget_rayWidg if widget does not exits
\-------------------------------------------------------*/
void
drawRectByCoord_rayWidg(
   signed int xSI,           /*x coordinate*/
   signed int ySI,           /*y coordinate*/
   signed int widthSI,       /*width of rectangle*/
   signed int heightSI,      /*height of rectangle*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI   /*hex color of focus border*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun077 TOC:
   '   - draws a rectangle for with input coordinates
   '   o fun077 sec01:
   '     - variable declarations
   '   o fun077 sec02:
   '     - draw the rectangle
   '   o fun077 sec03:
   '     - draw border around rectangle (if requested)
   '   o fun077 sec04:
   '     - draw focus border (if requested)
   '   o fun077 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun077 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Rectangle coordsST;
   struct Color colorST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun077 Sec02:
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun077 Sec03:
   ^   - draw border around rectangle (if requested)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(typeSC & 1)
   { /*If: rectangle with border*/
      if(coordsST.x <= 0)
         coordsST.x = 0;
      else
         coordsST.x -= def_border_rayWidg;

      if(coordsST.y <= 0)
         coordsST.y = 0;
      else
         coordsST.y -= def_border_rayWidg;

      coordsST.width += (def_border_rayWidg << 1);
      coordsST.height += (def_border_rayWidg << 1);

      colorST.r = borderHexUI >> 24 & 0xff;   /*red*/
      colorST.g = (borderHexUI >> 16) & 0xff; /*green*/
      colorST.b = (borderHexUI >> 8) & 0xff;  /*blue*/
      colorST.a = borderHexUI & 0xff;         /*alpha*/

      #ifdef MAC
         DrawRectangleRoundedLines(
            coordsST,
            def_macRoundness_rayWidg,
            def_macSegments_rayWidg,
            colorST
         );
      #else
         DrawRectangleLines(
            coordsST.x,
            coordsST.y,
            coordsST.width,
            coordsST.height,
            colorST
         );
      #endif
   } /*If: rectangle with border*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun077 Sec04:
   ^   - draw focus border around rectangle (if requested)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(typeSC & 2)
   { /*If: drawing a focus border*/
      if(coordsST.x <= 0)
         coordsST.x = 0;
      else
         coordsST.x -= def_focusBorder_rayWidg;

      if(coordsST.y <= 0)
         coordsST.y = 0;
      else
         coordsST.y -= def_focusBorder_rayWidg;

      coordsST.width += (def_focusBorder_rayWidg << 1);
      coordsST.height += (def_focusBorder_rayWidg << 1);

      colorST.r = focusHexUI >> 24 & 0xff;   /*red*/
      colorST.g = (focusHexUI >> 16) & 0xff; /*green*/
      colorST.b = (focusHexUI >> 8) & 0xff;  /*blue*/
      colorST.a = focusHexUI & 0xff;         /*alpha*/

      #ifdef MAC
         DrawRectangleRoundedLines(
            coordsST,
            def_macRoundness_rayWidg,
            def_macSegments_rayWidg,
            colorST
         );
      #else
         DrawRectangleLines(
            coordsST.x,
            coordsST.y,
            coordsST.width,
            coordsST.height,
            colorST
         );
      #endif
   } /*If: drawing a focus border*/
} /*drawRectByCoord_rayWidg*/

/*-------------------------------------------------------\
| Fun078: drawRect_rayWidg
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
drawRect_rayWidg(
   struct widg_rayWidg *widgSTPtr, /*has coordinates*/
   signed int idSI,          /*id of widget to draw*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI   /*hex color of focus border*/
){
   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun078;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun078;

   drawRectByCoord_rayWidg(
      widgSTPtr->xArySI[idSI],  /*x coordinate*/
      widgSTPtr->yArySI[idSI],  /*y coordinate*/
      widgSTPtr->widthArySI[idSI],/*width fof rectangle*/
      widgSTPtr->heightArySI[idSI],/*height of rectangle*/
      typeSC,                   /*type of rectangle*/
      colorHexUI,               /*hex color of rectangle*/
      borderHexUI,              /*hex color of border*/
      focusHexUI                /*hex color of focus border*/
   );

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun078:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun078:;
      return 0;
} /*drawRect_rayWidg*/

/*-------------------------------------------------------\
| Fun079: butDraw_rayWidg
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
   ' Fun079 TOC:
   '   - draw a button
   '   o fun079 sec01:
   '     - variable declarations
   '   o fun079 sec02:
   '     - get color of button (state in)
   '   o fun079 sec03:
   '     - get width of button
   '   o fun079 sec04:
   '     - draw button
   '   o fun079 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = widgSTPtr->focusColSI;
   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxButText_fun079 32
      /*max number of characters in button*/
   signed char writeStr[def_maxButText_fun079 + 8];

   /*these are for shorting strings*/
   signed char histUC = 0;
   signed int histIndexSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec02:
   ^   - get color of button (state in)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun079_sec05;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun079_sec05;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: button is in a disabled state*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a disabled state*/

   else if(widgSTPtr->stateAryUS[idSI] &def_press_rayWidg)
   { /*Else If: button is in a press state*/
      colorUI = widgSTPtr->pressColSI;
      borderColorUI = widgSTPtr->pressBorderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: button is in a press state*/

   else
   { /*Else If: button is in a normal state*/
      colorUI = widgSTPtr->widgColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a normal state*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec03:
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
      widgSTPtr->widthArySI[idSI] = endStr_ulCp(textStr);

      if(
        widgSTPtr->widthArySI[idSI] > def_maxButText_fun079
      )widgSTPtr->widthArySI[idSI] = def_maxButText_fun079;

      cpLen_ulCp(
         writeStr,
         textStr,
         widgSTPtr->widthArySI[idSI]
      );

      histIndexSI = widgSTPtr->widthArySI[idSI];
      textDimVect2 =
         MeasureTextEx(
            *widgSTPtr->fontSTPtr,
            (char *) writeStr,
            widgSTPtr->fontSizeSI,
            widgSTPtr->spacingF
         );
      widgSTPtr->widthArySI[idSI] = textDimVect2.x;

      if(widgSTPtr->widthArySI[idSI] < minWidthSI)
         widgSTPtr->widthArySI[idSI] = minWidthSI;
   } /*Else: have text to use*/

   if(widgSTPtr->widthArySI[idSI] > maxWidthSI)
   { /*Else If: I can not draw the full button*/
      histUC = 0;

      while(widgSTPtr->widthArySI[idSI] > maxWidthSI)
      { /*Loop: find number of bytes*/
         writeStr[histIndexSI] = histUC;

         --histIndexSI;
         histUC = writeStr[histIndexSI];
         writeStr[histIndexSI] = 0;

         textDimVect2 =
            MeasureTextEx(
               *widgSTPtr->fontSTPtr,
               (char *) writeStr,
               widgSTPtr->fontSizeSI,
               widgSTPtr->spacingF
            );

         widgSTPtr->widthArySI[idSI] = textDimVect2.x;
      } /*Loop: find number of bytes*/

      if(histIndexSI >= 3)
      { /*If: can copy in ...*/
         writeStr[histIndexSI - 1] = '.';
         writeStr[histIndexSI - 2] = '.';
         writeStr[histIndexSI - 3] = '.';
      } /*If: can copy in ...*/
   } /*Else If: I can not draw the full button*/

   if(! widgSTPtr->widthArySI[idSI])
      widgSTPtr->widthArySI[idSI] = widgSTPtr->fontWidthF;
         /*button needs to be at least 1 char*/

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);

   widgSTPtr->widthArySI[idSI] +=
      widgSTPtr->fontWidthF + widgSTPtr->fontWidthF;
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun079_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec04:
   ^   - draw button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg)
      histUC = 3;
   else
      histUC = 1;

   drawRect_rayWidg(
      widgSTPtr,
      idSI,
      (signed char) histUC,
      colorUI,
      borderColorUI,
      focusColorUI
   );

   if(textStr)
      DrawTextEx(
         *widgSTPtr->fontSTPtr,
         (char *) writeStr,
         (Vector2) {
             widgSTPtr->xArySI[idSI] +
               widgSTPtr->fontWidthF, /*center text in x*/
            widgSTPtr->yArySI[idSI]
              + widgSTPtr->fontHeightF
              / def_widgHeightGap_rayWidg
               /*center text on y*/
         },
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         textCol
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun079 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun079_sec05:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun079_sec05:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun079_sec05:;
      return 0;
} /*butDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun080: entryDraw_rayWidg
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
   ' Fun080 TOC:
   '   - draw an entry box, prevents typing if maxium is
   '     exceded or wrong entry type
   '   o fun080 sec01:
   '     - variable declarations
   '   o fun080 sec02:
   '     - check state of entry box
   '   o fun080 sec03:
   '     - copy string & check/set scroll & cursor values
   '   o fun080 sec04:
   '     - find text size and print cursor
   '   o fun080 sec05:
   '     - draw entry box
   '   o fun080 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun080 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = widgSTPtr->focusColSI;
   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxChar_fun080 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun080 + 8];
   signed char *cpStr = 0;
   signed char *dupStr = 0;
   signed char rmStr[2] = {0, 0}; /*for adjusting length*/

   signed int tmpSI = 0;
   signed int outLenSI = 0;
   signed int cursorPosSI = posArySI[1];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun080 Sec02:
   ^   - check state of entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun080_sec06;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun080_sec06;
      /*entry box is hidden*/

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: entry box is being used*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else If: entry box is being used*/

   /*will likely remove, or use lighter shade difference*/
   else if(
      widgSTPtr->stateAryUS[idSI] & def_active_rayWidg
   ){ /*Else If: entry box is being used*/
      colorUI = widgSTPtr->activeColSI;
      borderColorUI = widgSTPtr->pressBorderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: entry box is being used*/

   else
   { /*Else: not being used*/
      colorUI = widgSTPtr->widgColSI;
      borderColorUI = widgSTPtr->borderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else: not being used*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun080 Sec03:
   ^   - copy string & check/set scroll & cursor values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   outLenSI = endStr_ulCp(textStr);

   if(posArySI[0] > outLenSI)
      posArySI[0] = outLenSI;
      /*scroll position is past end of string*/
   if(posArySI[1] > outLenSI)
      posArySI[1] = outLenSI;
      /*cursor position is past end of string*/

   outLenSI -= posArySI[0];

   if(posArySI[1] < 0 && posArySI[0] < 0)
   { /*If: user wants position 0*/
      posArySI[1] = 0; /*cursor position*/
      posArySI[0] = 0; /*scroll position*/
   } /*If: user wants position 0*/

   else if(posArySI[1] < 0)
      posArySI[1] = posArySI[0];
      /*set cursor position to scroll position*/
   else if(posArySI[0] < 0)
      posArySI[0] = posArySI[1];
      /*set scroll position to cursor position*/


   if(outLenSI >= def_maxChar_fun080)
      outLenSI = def_maxChar_fun080 - 1;
         /*need one character for the cursor*/

   cpLen_ulCp(outStr, &textStr[posArySI[0]], outLenSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun080 Sec04:
   ^   - find text size and print cursor
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*get cursor in measurment*/
   if(cursorPosSI >= 0)
   { /*If: adding in a cursor*/
      cursorPosSI = posArySI[1];
      outStr[outLenSI] = def_cursor_rayWidg;
      outStr[outLenSI + 1] = 0;

      textDimVect2 =
         MeasureTextEx(
            *widgSTPtr->fontSTPtr,
            (char *) outStr,
            widgSTPtr->fontSizeSI,
            widgSTPtr->spacingF
         );
      tmpSI = textDimVect2.x;

      outStr[outLenSI] = 0;
         /*will add cursor back in later*/
      --outLenSI;
      cpStr = outStr;
   } /*If: adding in a cursor*/

   else
   { /*Else: not adding a cursor*/
      textDimVect2 =
         MeasureTextEx(
            *widgSTPtr->fontSTPtr,
            (char *) outStr,
            widgSTPtr->fontSizeSI,
            widgSTPtr->spacingF
         );
      tmpSI = textDimVect2.x;

      --outLenSI;
      cpStr = outStr;
   } /*Else: not adding a cursor*/

   while(tmpSI > widthSI)
   { /*Loop: shorten string down*/
      if(outLenSI <= 1)
         break; /*width must be at least one character*/

      if(cursorPosSI < outLenSI)
      { /*If: removing last character*/
         rmStr[0] = cpStr[outLenSI];
         cpStr[outLenSI] = 0;
      } /*If: removing last character*/

      else
      { /*Else: need to adjust scroll by one character*/
         rmStr[0] = cpStr[0];
         ++cpStr;
         --cursorPosSI;
      } /*Else: need to adjust scroll by one character*/

      --outLenSI;

      textDimVect2 =
        MeasureTextEx(
           *widgSTPtr->fontSTPtr,
           (char *) rmStr,
           widgSTPtr->fontSizeSI,
           widgSTPtr->spacingF
        );

      tmpSI -= textDimVect2.x;
   } /*Loop: shorten string down*/

   ++outLenSI;
   cpLen_ulCp(outStr, cpStr, outLenSI);

   if(cursorPosSI < 0)
      goto getDimensions_fun080_sec04;

   if(cursorPosSI > outLenSI)
      cursorPosSI = outLenSI;

   if(cursorPosSI == outLenSI)
      outStr[outLenSI + 1] = 0;
      /*cursor comes at end*/

   else
   { /*Else: cursor is not at end of string*/
      dupStr = &outStr[outLenSI];
      dupStr[1] = 0;
      cpStr = &outStr[outLenSI - 1];

      while(cpStr >= outStr + cursorPosSI)
         *dupStr-- = *cpStr--;
   } /*Else: cursor is not at end of string*/

   
   if(blinkBl)
      outStr[cursorPosSI] = def_blinkCursor_rayWidg;
   else
      outStr[cursorPosSI] = def_cursor_rayWidg;

   ++outLenSI;

   getDimensions_fun080_sec04:;

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);
   widgSTPtr->widthArySI[idSI] = widthSI;
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun080_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun080 Sec05:
   ^   - draw entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->stateAryUS[idSI] & def_focus_rayWidg)
      tmpSI = 3;
   else
      tmpSI = 1;

   drawRect_rayWidg(
      widgSTPtr,
      idSI,
      (signed char) tmpSI,
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
   ^ Fun080 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun080_sec06:;
      return outLenSI;

   noWidget_fun080_sec06:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun080_sec06:;
      return 0;
} /*entryDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun081: entryEvent_rayWidg
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
|     o for examples see fun072, fun073, or fun074
|       * fun072: intStrCheck_rayWidg
|       * fun073: floatStrCheck_rayWidg
|       * fun074: fileStrCheck_rayWidg
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
){

   if(idSI == eventSTPtr->parIdSI)
      ;
   else if(idSI == eventSTPtr->childIdSI)
      ;
   else if( activeGet_widg_rayWidg(idSI, widgSTPtr) )
      ; /*the entry box is active, so taking input*/
   else
      goto wrongWidget_fun081;

   if(eventSTPtr->scrollF == 0 && ! eventSTPtr->keySI)
      goto noEvent_fun081;
   
   if(eventSTPtr->scrollF != 0)
      posArySI[0] += (signed int) eventSTPtr->scrollF;

   if(eventSTPtr->keySI)
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

   noEvent_fun081:;
      return -1;
   wrongWidget_fun081:;
      return -2;
} /*entryEvent_rayWidg*/

/*-------------------------------------------------------\
| Fun082: labDraw_rayWidg
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
   ' Fun082 TOC:
   '   - draw a label
   '   o fun082 sec01:
   '     - variable declarations
   '   o fun082 sec02:
   '     - check state of label and copy string
   '   o fun082 sec03:
   '     - find text size and pad if needed
   '   o fun082 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxChar_fun082 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun082 + 8];
   signed int outLenSI = 0;
   signed int oldLenSI = 0;
   signed int tmpSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec02:
   ^   - check state of label and copy string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun082_sec04;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun082_sec04;
      /*entry box is hidden*/

   if(minWidthSI > def_maxChar_fun082)
      minWidthSI = def_maxChar_fun082;

   textCol = GetColor(widgSTPtr->textColSI);
   outLenSI = endStr_ulCp(textStr);

   if(outLenSI >= def_maxChar_fun082)
   { /*If: user input to long of string*/
      outLenSI = def_maxChar_fun082 - 4;
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
   ^ Fun082 Sec03:
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
   widgSTPtr->widthArySI[idSI] +=
      widgSTPtr->fontWidthF + widgSTPtr->fontWidthF;
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun082_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun082 Sec04:
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

   done_fun082_sec04:;
      return outLenSI;

   noWidget_fun082_sec04:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun082_sec04:;
      return 0;
} /*labDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun083: mkMesgBox_rayWidg
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
         goto memErr_fun084;
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

   memErr_fun084:;
      return -1;
} /*mkMesgBox_rayWidg*/

/*-------------------------------------------------------\
| Fun084: mesgBoxDraw_rayWidg
|   - draws a message box
| Input:
|   - idSI:
|     o id of message boxes parent widget
|       * a message box is a complex widget, with three
|         child widgets
|   - widthSI:
|     o width of GUI or draw region
|   - heightSI:
|     o height of GUI or draw region
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
   signed int widthSI,    /*width of GUI or draw region*/
   signed int heightSI,   /*height of GUI or draw region*/
   signed char *mesgStr,  /*message to show*/
   signed char *butTextStr, /*text on button*/
   struct widg_rayWidg *widgSTPtr /*has widgets*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun084 TOC:
   '   - draws a message box
   '   o fun084 sec01:
   '     - variable declarations
   '   o fun084 sec02:
   '     - initalize and copy/edit string
   '   o fun084 sec03:
   '     - find widget widths
   '   o fun084 sec04:
   '     - find widget coordinates
   '   o fun084 sec05:
   '     - draw message box
   '   o fun084 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   Vector2 textDimVect2;

   #define def_lenOut_fun084 128
   signed char outStr[def_lenOut_fun084 + 8];
   signed int textLenSI = endStr_ulCp(mesgStr);
   signed int butLenSI = 0;
   signed int maxLenSI = 0;

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun084 Sec02:
   ^   - initalize and copy/edit string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun084_sec06;
   if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hidden_fun084_sec06;
   if(textLenSI > def_lenOut_fun084)
      goto mesgErr_fun084_sec06;

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
   ^ Fun084 Sec03:
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
   ^ Fun084 Sec04:
   ^   - find widget coordinates
   ^   o fun084 sec04 sub01:
   ^     - find x coordinates
   ^   o fun084 sec04 sub02:
   ^     - find y coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun084 Sec04 Sub01:
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
   * Fun084 Sec04 Sub02:
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
   ^ Fun084 Sec05:
   ^   - draw message box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI,
      1,       /*only draw the inner border*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
   ); /*rectangle holding message box*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 1,
      1,       /*only draw the inner border*/
      widgSTPtr->widgColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
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
   ^ Fun084 Sec06:
   ^   - return
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

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun084_sec06:;
      return def_noWidget_rayWidg;

   hidden_fun084_sec06:;
      return 0;

   mesgErr_fun084_sec06:;
      return -1;
} /*mesgBoxDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun085: mesgBoxEvent_rayWidg
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
      goto noEvent_fun086;
   } /*If: clearing button press*/

   else if(childIdSI - parIdSI != 3)
      goto emptyClick_fun086;

   else if(eventSC == 1)
   { /*Else If: press event*/
      pressAdd_widg_rayWidg(childIdSI, widgSTPtr);
      goto noEvent_fun086;
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

   noEvent_fun086:;
      return 0;
   emptyClick_fun086:;
      return 0;
} /*mesgBoxEvent_rayWidg*/

/*-------------------------------------------------------\
| Fun087: blank_listBox_rayWidg
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

   
   listSTPtr->maxHeightSI = 0;
   listSTPtr->minHeightSI = 0;

   listSTPtr->lenSI = 0;
   listSTPtr->maxSelectSI = 0;
   listSTPtr->numSelectSI = 0;

   listSTPtr->onSI = 0;
   listSTPtr->lastSelectSI = -1;

   listSTPtr->scrollSI = 0;
   listSTPtr->clickSI = -1;
} /*blank_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun088: init_listBox_rayWidg
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

   blank_listBox_rayWidg(listSTPtr);
} /*init_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun089: freeStack_listBox_rayWidg
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
| Fun090: freeHeap_listBox_rayWidg
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
| Fun091: realloc_listBox_rayWidg
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
   ' Fun091 TOC:
   '   - add more memory to listBox_rayWidg structure
   '   o fun091 sec0x:
   '     - return results
   '   o fun091 sec01:
   '     - variable declarations and initial checks
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun091 Sec01:
   ^   - variable declarations and initial checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char **swapAryStr = 0;
   signed int *swapSIPtr = 0;
   signed char *swapStr = 0;
   signed int tmpSI = 0;

   if(! listSTPtr)
      goto noExpand_fun091_sec03;

   else if(sizeSI <= 0)
      sizeSI =
         listSTPtr->sizeSI + (listSTPtr->sizeSI >> 1);

   else if(sizeSI <= listSTPtr->sizeSI)
      goto noExpand_fun091_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun091 Sec02:
   ^   - expand memory
   ^   o fun091 sec02 sub01:
   ^     - c-string array (list box entries) expansion
   ^   o fun091 sec02 sub02:
   ^     - c-string size array expansion
   ^   o fun091 sec02 sub03:
   ^     - state array expansion
   ^   o fun091 sec02 sub04:
   ^     - initialize memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun091 Sec02 Sub01:
   *   - c-string array (list box entries) expansion
   \*****************************************************/

   if(! listSTPtr->textAryStr)
   { /*If: first memory allocation*/
      listSTPtr->textAryStr =
         malloc(sizeSI * sizeof(char *));
      if(! listSTPtr->textAryStr)
         goto memErr_fun091_sec03;
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
         goto memErr_fun091_sec03;
      listSTPtr->textAryStr = swapAryStr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun091 Sec02 Sub02:
   *   - c-string size array expansion
   \*****************************************************/

   if(! listSTPtr->sizeArySI)
   { /*If: first memory allocation*/
      listSTPtr->sizeArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! listSTPtr->sizeArySI)
         goto memErr_fun091_sec03;
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
         goto memErr_fun091_sec03;
      listSTPtr->sizeArySI = swapSIPtr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun091 Sec02 Sub03:
   *   - state array expansion
   \*****************************************************/

   if(! listSTPtr->stateArySC)
   { /*If: first memory allocation*/
      listSTPtr->stateArySC =
         malloc(sizeSI * sizeof(signed int));
      if(! listSTPtr->stateArySC)
         goto memErr_fun091_sec03;
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
         goto memErr_fun091_sec03;
      listSTPtr->stateArySC = swapStr;
   } /*Else: memory reallocation*/

   /*****************************************************\
   * Fun091 Sec02 Sub04:
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
   ^ Fun091 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   noExpand_fun091_sec03:;
      return 0;
   memErr_fun091_sec03:;
      return 1;
} /*realloc_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun092: clear_listBox_rayWidg
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

   listSTPtr->onSI = 0;
   listSTPtr->lastSelectSI = -1;

   listSTPtr->scrollSI = 0;
   listSTPtr->clickSI = -1;

   listSTPtr->numSelectSI = 0;
} /*clear_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun093: clearSelect_listBox_rayWidg
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
| Fun094: selectMaxSet_listBox_rayWidg
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
| Fun095: widthSet_listBox_rayWidg
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
| Fun096: heightSet_listBox_rayWidg
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
| Fun097: hiddenSet_listBox_rayWidg
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
| Fun098: selectSet_listBox_rayWidg
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
| Fun099: specialSet_listBox_rayWidg
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
| Fun100: canSelectSet_listBox_rayWidg
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
| Fun101: hiddenGet_listBox_rayWidg
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
| Fun102: selectGet_listBox_rayWidg
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
| Fun103: specialGet_listBox_rayWidg
|   - sets a list box item to the special state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the special state to a list box item
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
| Fun104: canSelectGet_listBox_rayWidg
|   - sets a list box item to the can select state
| Input:
|   - itemSI:
|     o index of item to set state for
|   - listSTPtr:
|     o listBox_rayWidg struct pointer
| Output:
|   - Modifies:
|     o adds the can select state to a list box item
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
| Fun105: mk_listBox_rayWidg
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

   if(idSI + 4 >= widgSTPtr->sizeSI)
   { /*If: need more room*/
      idSI += (idSI >> 1);

      if( realloc_widg_rayWidg(idSI, widgSTPtr) )
         goto memErr_fun095;
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
   inactiveAdd_widg_rayWidg(idSI, widgSTPtr);
      /*parent is always inactive*/

   addWidget_widg_rayWidg(
      xSI, /*first tile column*/
      ySI, /*first tile row*/
      0, /*use tile system to organize*/
      0, /*width is added in later*/
      0, /*height is added in later*/
      widgSTPtr  /*add list box to*/
   ); /*actual list box*/
   childAdd_widg_rayWidg(idSI + 1, widgSTPtr);

  /*would be vertical scroll bar if add in*/
  addWidget_widg_rayWidg(1, 0, 1, 0, 0, widgSTPtr);
  childAdd_widg_rayWidg(idSI + 2, widgSTPtr);
  inactiveAdd_widg_rayWidg(idSI + 2, widgSTPtr);
  hidenAdd_widg_rayWidg(idSI + 2, widgSTPtr);

  /*would be horizontal scroll bar if add in*/
  addWidget_widg_rayWidg(0, 1, 1, 0, 0, widgSTPtr);
  childAdd_widg_rayWidg(idSI + 3, widgSTPtr);
  inactiveAdd_widg_rayWidg(idSI + 3, widgSTPtr);
  hidenAdd_widg_rayWidg(idSI + 3, widgSTPtr);

   return idSI;

   memErr_fun095:;
      return -1;
} /*mk_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun106: addItem_listBox_rayWidg
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

   if(listSTPtr->sizeSI <= listSTPtr->lenSI)
   { /*If: need more room*/
      if(
         realloc_listBox_rayWidg(
            lenSI + 8,
            listSTPtr
         )
      ) goto memErr_fun00h;
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

   listSTPtr->stateArySC[listSTPtr->lenSI] = stateSC;

   lenSI =
      textMeasure_widg_rayWidg(
         listSTPtr->textAryStr[listSTPtr->lenSI],
         &discardSI, /*do not care about height*/
         widgSTPtr
      );

   lenSI += widgSTPtr->fontWidthF * 2;

   if(lenSI > listSTPtr->lineWidthSI)
      listSTPtr->lineWidthSI = lenSI;

   ++listSTPtr->lenSI;
   return 0;

   memErr_fun00h:;
      return 1;
} /*addItem_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun107: draw_listBox_rayWidg
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
   ' Fun107 TOC:
   '   - draws a list box widget
   '   o fun107 sec01:
   '     - variable declarations
   '   o fun107 sec02:
   '     - check if can draw and get color scheme
   '   o fun107 sec03:
   '     - get dimensions
   '   o fun107 sec04:
   '     - draw the list box
   '   o fun107 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun107 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = 0;
   unsigned int selectColUI = 0;

   signed int siLine = 0;   /*line on*/
   signed int maxLinesSI = 0;

   signed int itemSI = 0;   /*item on in list box*/
   signed int ySI = 0;      /*y position to draw item*/

   #define lenStr_fun107 512 /*max c-string length*/
   signed char textStr[lenStr_fun107 + 8];

   struct Color textCol;
   struct Color selectTextCol;
   struct Color *textPickColPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun107 Sec02:
   ^   - check if can draw and get color scheme
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun107_sec0x;

   else if( hidenGet_widg_rayWidg(idSI, widgSTPtr ) )
      goto hidenWidget_fun107_sec0x;

   else if( inactiveGet_widg_rayWidg(idSI, widgSTPtr ) )
   { /*Else If: list box is in an inactive state*/
       colorUI = (unsigned int) widgSTPtr->guiColSI;
       borderColorUI = (unsigned int)
          widgSTPtr->borderColSI;
       textCol = GetColor(widgSTPtr->textColSI);

       selectColUI =(unsigned int) widgSTPtr->activeColSI;
       selectTextCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: list box is in an inactive state*/

   else
   { /*Else: user is unsing list box*/
       colorUI = (unsigned int) widgSTPtr->widgColSI;
       borderColorUI = (unsigned int)
          widgSTPtr->borderColSI;
       textCol = GetColor(widgSTPtr->textColSI);

       selectColUI =(unsigned int) widgSTPtr->activeColSI;
       selectTextCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else: user is unsing list box*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun107 Sec03:
   ^   - get dimensions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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

   /*find the height of the list box*/
   if(maxLinesSI > listSTPtr->lenSI)
      maxLinesSI = listSTPtr->lenSI;
   else if(maxLinesSI < itemSI)
      maxLinesSI = itemSI; /*beneath min line count*/

   widgSTPtr->heightArySI[idSI + 1] =
      linesToHeight_rayWidg(maxLinesSI, widgSTPtr);
   widgSTPtr->heightArySI[idSI + 1] +=
      widgSTPtr->fontHeightF / def_widgHeightGap_rayWidg;

   /*find the width of the list box*/
   if(listSTPtr->lineWidthSI > listSTPtr->maxWidthSI)
      widgSTPtr->widthArySI[idSI + 1] =
         listSTPtr->maxWidthSI;

   else if(
      listSTPtr->lineWidthSI < listSTPtr->minWidthSI
   ) widgSTPtr->widthArySI[idSI + 1] =
         listSTPtr->minWidthSI;

   else
      widgSTPtr->widthArySI[idSI + 1] =
         listSTPtr->lineWidthSI;

   widgSTPtr->widthArySI[idSI] =
      widgSTPtr->widthArySI[idSI + 1];
   widgSTPtr->heightArySI[idSI] =
      widgSTPtr->heightArySI[idSI + 1];

   widgSTPtr->xArySI[idSI + 1] = widgSTPtr->xArySI[idSI];
   widgSTPtr->yArySI[idSI + 1] = widgSTPtr->yArySI[idSI];

   if(noDrawBl)
      goto noDraw_fun107_sec0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun107 Sec04:
   ^   - draw the list box
   ^   o fun107 sec04 sub01:
   ^     - draw rectangle for list box
   ^   o fun107 sec04 sub02:
   ^     - draw each item currently shown in list box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun107 Sec04 Sub01:
   *   - draw rectangle for list box
   \*****************************************************/

   if( focusGet_widg_rayWidg(idSI + 1, widgSTPtr) )
      siLine = 3; /*with all borders (focus/regualr)*/
   else
      siLine = 1; /*only with regular border*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 1,
      (signed char) siLine,      
      colorUI,
      borderColorUI,
      focusColorUI
   ); /*rectangle for widget*/

   /*****************************************************\
   * Fun107 Sec04 Sub02:
   *   - draw each item currently shown in list box
   *   o fun107 sec04 sub02 cat01:
   *     - setup and detect if can draw item (not hidden)
   *   o fun107 sec04 sub02 cat02:
   *     - check if item was selected
   *   o fun107 sec04 sub02 cat03:
   *     - check if user is on item
   *   o fun107 sec04 sub02 cat04:
   *     - draw item
   *   o fun107 sec04 sub02 cat05:
   *     - move to next item
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun107 Sec04 Sub02 Cat01:
   +   - setup and detect if can draw item (not hidden)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   siLine = 0;
   itemSI = listSTPtr->scrollSI;
   ySI = widgSTPtr->yArySI[idSI + 1];

   while(siLine < maxLinesSI)
   { /*Loop: draw the labels*/
      if(itemSI >= listSTPtr->lenSI)
         break; /*no more items to draw*/

      if( hiddenGet_listBox_rayWidg(itemSI, listSTPtr) )
          ; /*item is hiden; nothing to do*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun107 Sec04 Sub02 Cat02:
      +   - check if item was selected
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: I can draw the item*/
         if(selectGet_listBox_rayWidg(itemSI, listSTPtr))
         { /*If: item was selected*/
            drawRectByCoord_rayWidg(
               (widgSTPtr->xArySI[idSI] + 1)
                  + widgSTPtr->fontWidthF / 4,
               (ySI + 1)
                  + widgSTPtr->fontHeightF
                  / def_widgHeightGap_rayWidg,
               (widgSTPtr->widthArySI[idSI] - 2)
                  - widgSTPtr->fontWidthF / 4,
               (widgSTPtr->fontHeightF - 2)
                  + widgSTPtr->fontHeightF
                  / (def_widgHeightGap_rayWidg * 2),
                  /*color half of gap with select color*/
               0, /*just rectangle, no borders*/
               selectColUI,
               selectColUI, /*border color*/
               selectColUI  /*no focus color*/
            );

            textPickColPtr = &selectTextCol;
         } /*If: item was selected*/

         else
            textPickColPtr = &textCol;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun107 Sec04 Sub02 Cat03:
         +   - check if user is on item
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(listSTPtr->onSI == itemSI)
         { /*If: need to draw box around item on*/
            drawRectByCoord_rayWidg(
               widgSTPtr->xArySI[idSI]
                  + widgSTPtr->fontWidthF / 4,
               ySI
                  + widgSTPtr->fontHeightF
                  / def_widgHeightGap_rayWidg,
               widgSTPtr->widthArySI[idSI]
                  - widgSTPtr->fontWidthF / 4,
               widgSTPtr->fontHeightF
                  + widgSTPtr->fontHeightF
                  / (def_widgHeightGap_rayWidg * 2),
                  /*color half of gap with select color*/
               5, /*just border (no rectangle)*/
               selectColUI, /*box color (not used)*/
               (unsigned int) widgSTPtr->borderColSI,
                   /*border color*/
               selectColUI  /*no focus color*/
            );
         } /*If: need to draw box around item on*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun107 Sec04 Sub02 Cat04:
         +   - draw item
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         /*get text to draw*/
         checkTextWidth_rayWidg(
           listSTPtr->textAryStr[itemSI],
           textStr,
           widgSTPtr->widthArySI[idSI]
              - (widgSTPtr->fontWidthF * 2),
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
      + Fun107 Sec04 Sub02 Cat05:
      +   - move to next item
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      ++itemSI;
   } /*Loop: draw the labels*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun107 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgSTPtr->widthArySI[idSI];

   noDraw_fun107_sec0x:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun107_sec0x:;
      return def_noWidget_rayWidg;

   hidenWidget_fun107_sec0x:;
      return 0;
} /*draw_listBox_rayWidg*/

/*-------------------------------------------------------\
| Fun108: listBoxEvent_rayWidg
|   - runs an event for a list box
| Input:
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
|     o 0 for no event
|     o 1 for movement event
|     o 2 for selection event
|     o 4 for enter/double click event
\-------------------------------------------------------*/
signed int
listBoxEvent_rayWidg(
   struct listBox_rayWidg *listSTPtr,/*list box data*/
   struct event_rayWidg *eventSTPtr, /*events user did*/
   struct widg_rayWidg *widgSTPtr    /*widgets user made*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun108 TOC:
   '   - runs an event for a list box
   '   o fun108 sec01:
   '     - variable declarations
   '   o fun108 sec02:
   '     - check if have event + find select/display limit
   '   o fun108 sec03:
   '     - deal with keyboard input
   '   o fun108 sec04:
   '     - deal with scroll wheel events
   '   o fun108 sec05:
   '     - deal with mouse clicks
   '   o fun108 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int maxLinesSI = 0;   /*maximum lines to show*/
   signed int maxSelectSI = listSTPtr->maxSelectSI;

   signed int changeSI = 0; /*for up/down key movements*/
   signed int cntSI = 0;    /*for up/down key movements*/
   signed int tmpSI = 0;

   signed int ySI = 0;      /*for getting item click on*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec02:
   ^   - check if have event + find select/display limits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*check for event*/
   if( eventSTPtr->childIdSI != eventSTPtr->parIdSI + 1 )
      goto noWidget_fun108_sec06;
     /*not on select box, on extra widgets*/

   else if(
      eventSTPtr->childIdSI != eventSTPtr->parIdSI + 1
   ) goto noWidget_fun108_sec06;
      /*not on select box, on extra widgets*/

   else if(
      hidenGet_widg_rayWidg(
         eventSTPtr->parIdSI + 1,
         widgSTPtr
      )
   ) goto hidenWidget_fun108_sec06;

   else if(
      inactiveGet_widg_rayWidg(
         eventSTPtr->parIdSI + 1,
         widgSTPtr
      )
   ) goto inactiveWidget_fun108_sec06;

   /*find max nubmer of files to select (select limit)*/
   if(maxSelectSI < 0)
      maxSelectSI = listSTPtr->lenSI;

   /*find maximum number of lines that can be shown*/
   maxLinesSI =
      getLines_rayWidg(
         widgSTPtr->heightArySI[eventSTPtr->parIdSI + 1],
         widgSTPtr
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec03:
   ^   - deal with keyboard input
   ^   o fun108 sec03 sub01:
   ^     - move down + start keyboard event if
   ^   o fun108 sec03 sub02:
   ^     - move up + start keyboard event if
   ^   o fun108 sec03 sub03:
   ^     - move to start or end of list
   ^   o fun108 sec03 sub04:
   ^     - enter key select item or trigger event
   ^   o fun108 sec03 sub05:
   ^     - space key (add selection items)
   ^   o fun108 sec03 sub06:
   ^     - ctrl-a if user wanted everything
   ^   o fun108 sec03 sub07:
   ^     - ctrl-r delesect (refresh)
   ^   o fun108 sec03 sub08:
   ^     - unknown key or no key event
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun108 Sec03 Sub01:
   *   - move down + start keyboard event if
   \*****************************************************/

   switch(eventSTPtr->keySI)
   { /*If: had a key board event*/
      case KEY_J:
      case KEY_DOWN:
      case KEY_PAGE_DOWN:
      /*Case: moving down one item or page*/
         listSTPtr->clickSI = -1;

         if(
               eventSTPtr->shiftBl
            || eventSTPtr->keySI == KEY_PAGE_DOWN
         ) changeSI = maxLinesSI;
         else
            changeSI = 1;

         tmpSI = listSTPtr->onSI + 1;
         cntSI = 0;

         while(
              tmpSI < listSTPtr->lenSI
           && cntSI < changeSI
         ){ /*Loop: find ending file*/
            if( 
                 listSTPtr->stateArySC[tmpSI]
               & def_listHidden_rayWidg
            ) ;

            else
            { /*Else: file can be moved to*/
               ++cntSI;
               listSTPtr->onSI = tmpSI;

               if(
                     tmpSI
                  >= listSTPtr->scrollSI + maxLinesSI
               ) listSTPtr->scrollSI = tmpSI;
                 /*next item is not in list box window*/
            } /*Else: file can be moved to*/

            ++tmpSI;
         } /*Loop: find ending file*/

         if(! cntSI)
            goto noEvent_fun108_sec06;
            /*on last visible item*/
         else
            goto moveEvent_fun108_sec06;
      /*Case: moving down one item or page*/

      /**************************************************\
      * Fun108 Sec03 Sub02:
      *   - move up
      \**************************************************/

      case KEY_K:
      case KEY_UP:
      case KEY_PAGE_UP:
      /*Case: moving up one item or page*/
         listSTPtr->clickSI = -1;

         if(
               eventSTPtr->shiftBl
            || eventSTPtr->keySI == KEY_PAGE_UP
         ) changeSI = maxLinesSI;
         else
            changeSI = 1;

         tmpSI = listSTPtr->onSI - 1;
         cntSI = 0;

         while(tmpSI >= 0 && cntSI < changeSI)
         { /*Loop: find ending file*/
            if( 
                 listSTPtr->stateArySC[tmpSI]
               & def_listHidden_rayWidg
            ) ;

            else
            { /*Else: file can be moved to*/
               ++cntSI;
               listSTPtr->onSI = tmpSI;

               if(tmpSI < listSTPtr->scrollSI)
                  listSTPtr->scrollSI = tmpSI;
                  /*next item is not in list box window*/
            } /*Else: file can be moved to*/

            --tmpSI;
         } /*Loop: find ending file*/

         if(! cntSI)
            goto noEvent_fun108_sec06;
            /*on first visible item*/
         else
            goto moveEvent_fun108_sec06;
      /*Case: moving up one item or page*/

      /**************************************************\
      * Fun108 Sec03 Sub03:
      *   - move to start or end of list
      \**************************************************/

      case KEY_HOME:
         listSTPtr->clickSI = -1;
         listSTPtr->scrollSI = 0;
         listSTPtr->lastSelectSI = -1;
         listSTPtr->onSI = 0;
         goto moveEvent_fun108_sec06;

      case KEY_END:
      case KEY_G:
      /*Case: move to start or end of list*/
         listSTPtr->clickSI = -1;

         if(
               ! eventSTPtr->shiftBl
            && eventSTPtr->keySI == KEY_G
         ){ /*If: user wanted top of list*/
            listSTPtr->scrollSI = 0;
            listSTPtr->onSI = 0;
            goto moveEvent_fun108_sec06;
         } /*If: user wanted top of list*/

         /*moving to last file in list*/
         tmpSI = listSTPtr->lenSI - 1;

         while(tmpSI > -1)
         { /*Loop: find last file*/
            if(
               ! hiddenGet_listBox_rayWidg(
                  tmpSI,
                  listSTPtr
               ) 
            ) break;

            --tmpSI;
         } /*Loop: find last file*/

         if(tmpSI >= 0)
         { /*If: have file to show*/
            listSTPtr->scrollSI = tmpSI;
            listSTPtr->onSI = tmpSI;
         } /*If: have file to show*/

         goto moveEvent_fun108_sec06;
      /*Case: move to start or end of list*/

      /**************************************************\
      * Fun108 Sec03 Sub04:
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
         ) goto specialSelectEvent_fun108_sec06;
           /*user needs to handel this one*/

         else
            goto selectEvent_fun108_sec06;
      /*Case: enter key was hit*/

      /**************************************************\
      * Fun108 Sec03 Sub05:
      *   - space key (add selection items)
      *   o fun108 sec03 sub05 cat01:
      *     - set selection to only one item + case
      *   o fun108 sec03 sub05 cat02:
      *     - add or remove item from selection
      *   o fun108 sec03 sub05 cat03:
      *     - add range to selection
      *   o fun108 sec03 sub05 cat04:
      *     - go to selection return
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun108 Sec03 Sub05 Cat01:
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
         + Fun108 Sec03 Sub05 Cat02:
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
         + Fun108 Sec03 Sub05 Cat03:
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
         + Fun108 Sec03 Sub05 Cat04:
         +   - go to selection return
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         goto selectEvent_fun108_sec06;
      /*Case: space key was hit*/

      /**************************************************\
      * Fun108 Sec03 Sub06:
      *   - ctrl-a if user wanted everything
      \**************************************************/

      case KEY_A:
      /*Case: a key*/
         listSTPtr->clickSI = -1;

         if(eventSTPtr->ctrlBl)
            ;
         else
            goto noEvent_fun108_sec06;

         if(listSTPtr->numSelectSI >= maxSelectSI)
            goto noEvent_fun108_sec06;
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

         goto selectEvent_fun108_sec06;
      /*Case: a key*/

      /**************************************************\
      * Fun108 Sec03 Sub07:
      *   - ctrl-r delesect (refresh)
      \**************************************************/

      case KEY_R:
      /*Case: r key*/
         listSTPtr->clickSI = -1;

         if(eventSTPtr->ctrlBl)
         { /*If: clearing all selected files*/
            clearSelect_listBox_rayWidg(listSTPtr);
            goto selectEvent_fun108_sec06;
         } /*If: clearing all selected files*/

         else
            goto noEvent_fun108_sec06;
      /*Case: r key*/

      /**************************************************\
      * Fun108 Sec03 Sub08:
      *   - unknown key or no key event
      \**************************************************/

      case 0:
          goto checkWheelEvent_fun108_sec04_sub01;

      default:
          goto noEvent_fun108_sec06;
          /*unreconginzed key input*/
   } /*If: was a key event*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec04:
   ^   - deal with scroll wheel events
   ^   o fun108 sec04 sub01:
   ^     - get scroll wheel movement & handel forward move
   ^   o fun108 sec04 sub02:
   ^     - handel backwards scroll wheel move
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun108 Sec04 Sub01:
   *   - get scroll wheel movement & handel forward move
   \*****************************************************/

   checkWheelEvent_fun108_sec04_sub01:;

   cntSI = (signed int) eventSTPtr->scrollF;

   if(cntSI > 0)
   { /*If: moving down*/
      listSTPtr->clickSI = -1;

      tmpSI = listSTPtr->onSI - 1;

      while(tmpSI >= 0 && cntSI > 0)
      { /*Loop: find ending file*/
         if( 
              listSTPtr->stateArySC[tmpSI]
            & def_listHidden_rayWidg
         ) ;

         else
         { /*Else: file can be moved to*/
            --cntSI;
            listSTPtr->onSI = tmpSI;

            if(tmpSI < listSTPtr->scrollSI)
               listSTPtr->scrollSI = tmpSI;
               /*next item is not in list box window*/
         } /*Else: file can be moved to*/

         --tmpSI;
      } /*Loop: find ending file*/

      goto moveEvent_fun108_sec06;
   } /*If: moving down*/

   /*****************************************************\
   * Fun108 Sec04 Sub02:
   *   - handel backwards scroll wheel move
   \*****************************************************/

   else if(cntSI < 0)
   { /*Else If: moving up*/
      listSTPtr->clickSI = -1;

      tmpSI = listSTPtr->onSI + 1;

      while(tmpSI < listSTPtr->lenSI && cntSI < 0)
      { /*Loop: find ending file*/
         if( 
              listSTPtr->stateArySC[tmpSI]
            & def_listHidden_rayWidg
         ) ;

         else
         { /*Else: file can be moved to*/
            ++cntSI;
            listSTPtr->onSI = tmpSI;

            if(tmpSI > listSTPtr->scrollSI + maxLinesSI)
               listSTPtr->scrollSI = tmpSI;
               /*next item is not in list box window*/
         } /*Else: file can be moved to*/

         ++tmpSI;
      } /*Loop: find ending file*/

      goto moveEvent_fun108_sec06;
   } /*Else If: moving up*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec05:
   ^   - deal with mouse clicks
   ^   o fun108 sec05 sub01:
   ^     - find the clicked on item in list box
   ^   o fun108 sec05 sub02:
   ^     - deal with click event
   ^   o fun108 sec05 sub03:
   ^     - check for right clicks; event does not capture
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun108 Sec05 Sub01:
   *   - find the clicked on item in list box
   \*****************************************************/

   ySI = eventSTPtr->ySI;
   ySI -= widgSTPtr->yArySI[eventSTPtr->childIdSI];

   if(
        ySI
     <= widgSTPtr->fontHeightF / def_widgHeightGap_rayWidg
    ) goto noEvent_fun108_sec06;
      /*nothing was selected*/

   ySI = getLines_rayWidg(ySI, widgSTPtr);
   ySI += listSTPtr->scrollSI; /*from first position*/

   if(ySI >= listSTPtr->lenSI)
   { /*If: empty space was clicked*/
      listSTPtr->clickSI = -1;
      goto noEvent_fun108_sec06;
   } /*If: empty space was clicked*/

   if(listSTPtr->clickSI != ySI)
      listSTPtr->clickSI = -1;
      /*different item being clicked on*/

   /*****************************************************\
   * Fun108 Sec05 Sub02:
   *   - deal with click event
   *   o fun108 sec05 sub02 cat01:
   *     - control + click; add/remove file from selection
   *   o fun108 sec05 sub02 cat01:
   *     - shift + click select a range
   *   o fun108 sec05 sub02 cat03:
   *     - single click; set item as only selected item
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun108 Sec05 Sub02 Cat01:
   +   - control + click (add/remove file from selection)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(eventSTPtr->leftPressBl)
      ; /*do nothing for press events*/

   else if(eventSTPtr->leftReleaseBl)
   { /*Else If: release event*/

      if(eventSTPtr->ctrlBl)
      { /*If: user wanted to add item to selection list*/
         listSTPtr->clickSI = -1;
         listSTPtr->onSI = ySI;

         if(
              listSTPtr->stateArySC[ySI]
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
               listSTPtr->lastSelectSI = listSTPtr->onSI;

               listSTPtr->stateArySC[listSTPtr->onSI] ^=
                   def_listSelect_rayWidg;
            }  /*Else If: selecting the item*/
         }  /*If: can select the item*/
      } /*If: user wanted to add item to selection list*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun108 Sec05 Sub02 Cat01:
      +   - shift + click select a range
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(eventSTPtr->shiftBl)
      { /*Else If: selecting a range of items*/
         listSTPtr->clickSI = -1;

         if(ySI > listSTPtr->lastSelectSI)
         { /*If: going fowards*/
            tmpSI = listSTPtr->lastSelectSI;
            cntSI = ySI + listSTPtr->scrollSI;
         } /*If: going fowards*/

         else
         { /*Else: going backwards*/
            cntSI = listSTPtr->lastSelectSI;
            tmpSI = ySI + listSTPtr->scrollSI;
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
                  listSTPtr->lastSelectSI =
                     listSTPtr->onSI;
                  ++listSTPtr->numSelectSI;
               } /*Else: need to select the item*/
            }  /*Else If: adding item to selection*/

            ++tmpSI;
         } /*Loop: add files to selection*/

         listSTPtr->onSI = ySI;
      } /*Else If: selecting a range of items*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun108 Sec05 Sub02 Cat03:
      +     - single click; set item as only selected item
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: user is selecting one item*/
         clearSelect_listBox_rayWidg(listSTPtr);
         listSTPtr->onSI = ySI;

         if(
              listSTPtr->stateArySC[ySI]
            & def_listCanSelect_rayWidg
         ){ /*If: can select the item*/
            if(listSTPtr->numSelectSI < maxSelectSI)
            { /*Else If: selecting the item*/
               ++listSTPtr->numSelectSI;
               listSTPtr->lastSelectSI = listSTPtr->onSI;

               listSTPtr->stateArySC[listSTPtr->onSI] ^=
                   def_listSelect_rayWidg;
            }  /*Else If: selecting the item*/
         }  /*If: can select the item*/

         if(listSTPtr->clickSI == ySI)
         { /*If: was a double click*/
            listSTPtr->clickSI = -1;/*clear double click*/

            if(
                 listSTPtr->stateArySC[ySI]
               & def_listSpecial_rayWidg
            ) goto specialSelectEvent_fun108_sec06;
              /*selected a special item the user needs
              `  to do something with
              */
         } /*If: was a double click*/

         else
         { /*Else: single click*/
            listSTPtr->clickSI = ySI;
            goto selectEvent_fun108_sec06;
         } /*Else: single click*/
      } /*Else: user is selecting one item*/
   } /*Else If: release event*/

   /*****************************************************\
   * Fun108 Sec05 Sub03:
   *   - check for right clicks (eventST does not capture)
   \*****************************************************/

   else if( IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) )
      listSTPtr->clickSI = -1;

   else if( IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) )
      listSTPtr->clickSI = -1;

   goto noEvent_fun108_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun108 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   moveEvent_fun108_sec06:;
      return 1;

   selectEvent_fun108_sec06:;
      return 2;

   specialSelectEvent_fun108_sec06:;
      return 4; /*is really enter on special event*/

   /*these are states were the widget can not be
   `  interacted with or were the widget was not selected
   */
   noWidget_fun108_sec06:;
      return -1;

   noEvent_fun108_sec06:;
      return 0;

   hidenWidget_fun108_sec06:;
      return 0;

   inactiveWidget_fun108_sec06:;
      return 0;
} /*listBoxEvent_rayWidg*/


#ifdef NEW

/*-------------------------------------------------------\
| Fun00a: blank_files_rayWidg
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
  struct files_fayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   fileSTPtr->pwdStr[0] = 0;
   blank_listBox_rayWidg(&fileSTPtr->fileListST);
   blank_listBox_rayWidg(&fileSTPtr->extListST);
} /*blank_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00b: init_files_rayWidg
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
  struct files_fayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   init_listBox_rayWidg(&fileSTPtr->fileListST);
   init_listBox_rayWidg(&fileSTPtr->extListST);

   blank_files_rayWidg(filesSTPtr);
} /*init_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00c: freeStack_files_rayWidg
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
  struct files_fayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   freeStack_listBox_rayWidg(&fileSTPtr->fileListST);
   freeStack_listBox_rayWidg(&fileSTPtr->extListST);

   init_files_rayWidg(filesSTPtr);
} /*freeStack_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00d: freeHeap_files_rayWidg
|   - frees files_rayWidg struct
| Input:
|   - fileSTPtr:
|     o files_rayWidg struct pointer to free
| Output:
|   - Frees: fileSTPtr, but you must set to null
\-------------------------------------------------------*/
void
freeHeap_files_rayWidg(
  struct files_fayWidg *fileSTPtr
){
   if(! fileSTPtr)
      return;

   freeStack_files_rayWidg(fileSTPtr);
   free(fileSTPtr);
} /*freeHeap_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00e: getFile_files_rayWidg
|   - gets the next selected files from a files_rayWidg
|     struct
| Input:
|   - fileOnSIPtr:
|     o signed int pionter with current file on and to
|       advance to the next file
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
   struct files_rayWidg *fileSTPtr /*has selected files*/
){
   signed char *retHeapStr = 0;

   retHeapStr = malloc(512 * sizeof(signed char));
   if(! retHeapStr)
      goto memErr_fun0e;

   if(*fileOnSIPtr < 0)
      *fileOnSIPtr = 0;

   while(*fileOnSIPtr < filesSTPtr->fileListST.lenSI)
   { /*Loop: find next file*/
      if(
          selectGet_listBox_rayWidg(
             *fileOneSIPtr,
             &filesSTPtr->fileListST
          )
      ){ /*If: found a selected file*/
         cpStr_ulCp(
           retHeapStr,
           fileSTPtr->fileListST.textAryStr[*fileOneSIPtr]
         );

         ++*fileOnSIPtr;
         goto retFile_fun0e;
      }  /*If: found a selected file*/

      else
         ++*fileOnSIPtr
   }  /*Loop: find next file*/

   /*no files were found*/
   *fileOnSIPtr = -1;
   goto errClean_fun0e;

   memErr_fun0e:;
      *fileOnSIPtr = -2;
      goto errClean_fun0e;

   errClean_fun0e:;
      if(retHeapStr)
         free(retHeapStr);
      retHeapStr = 0;

      goto retFile_fun0e;

   retFile_fun0e:;
      return retHeapStr;
} /*getFile_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00f: addExt_files_rayWidg
|   - add an file extension to a files_rayWidg struct
| Input:
|   - extStr:
|     o c-string with extension to add (can be 0/null)
|   - clearBl:
|     o 1: remove all old extensions
|     o 0: append extension to the list
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
   struct files_rayWidg *fileSTPtr,/*add extension to*/
   struct widg_rayWidg *widgSTPtr /*has font*/
){
   unsigned char stateUC = 0;

   if(clearBl)
      clear_listBox_rayWidg(&fileSTPtr->extListST);

   if(! extStr)
      ; /*null input*/
   else if(*extStr)
      ; /*empty string*/
   else
   { /*Else: have an extension to add*/
      if(fileSTPtr->extListST.lenSI <= 0)
      { /*If: first extension added in*/
         stateUC =
              def_listCanSelect_rayWidg
            | def_listSelect_rayWidg;
      } /*If: first extension added in*/

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
| Fun0g: scanForFiles_files_rayWidg
|   - get files in a directory
| Input:
|   - dirStr:
|     o c-string with the new working directory to get
|       files from
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
   struct files_rayWidg *fileSTPtr /*add files to*/
   struct widg_rayWidg *widgSTPtr  /*has font size*/
){
   unsigned char stateUC = 0;
   signed char tmpStr[512];
   signed int lenSI = 0;
   signed char errSC = 0;
   signed char freeBl = 0;
   struct FilePathList stFiles;

   if(! cpEql_ulCp(fileSTPtr->pwdStr, dirStr))
      goto noChange_fun0g_sec0x;
   else if(FileExists(dirStr) )
      goto fileNotDir_fun0g_sec0x;
   else if(! DirectoryExists(dirStr) )
      goto noDirOrFile_fun0g_sec0x;

   stFiles = LoadDirectoryFiles(dirStr);
   freeBl = 1;

   clear_listBox_rayWidg(&fileSTPtr->fileListST);
   if(
      addItem_listBox_rayWidg(
         (signed char *) "[d] <--- (..)", /*back one dir*/
         0,
         &fileSTPtr->fileListST,
         widgSTPtr
      )
   ) goto memErr_fun0g_sec0x;

   for(
      fileSTPtr->fileListST.lenSI = 0;
      fileSTPtr->fileListST.lenSI < stFiles.count;
      ++fileSTPtr->fileListST.lenSI;
   ){ /*Loop: copy files*/
      stateUC = def_listCanSelect_rayWidg;

      /*TODO:
      `  need to check if file extension is a match
      */
      cpStr_ulCp(
         &tmpStr[4],
         stFiles.paths[fileSTPtr->fileListST.lenSI],
      );

      if(
         DirecotryExists(
            stFiles.paths[fileSTPtr->fileListST.lenSI]
         )
      ){ /*If: file was a directory*/
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

      if(
         addItem_listBox_rayWidg(
            stFiles.paths[fileSTPtr->fileListST.lenSI],
            stateUC,
            &fileSTPtr->fileListST,
            widgSTPtr
         )
      ) goto memErr_fun0g_sec0x;
   }  /*Loop: copy files*/

   errSC = 0;

   fileNotDir_fun0g_sec0x:;
      errSC = -1;
   noDirOrFile_fun0g_sec0x:;
      errSC = -2;
   noChange_fun0g_sec0x:;
      errSC = -3;
   memErr_fun0g_sec0x:;
      errSC = -4;

   ret_fun0g_sec0x:;
      if(freeBl)
         UnloadDirectoryFiles(stFiles);
      return errSC;
} /*scanForFiles_files_rayWidg*/

/*-------------------------------------------------------\
| Fun00x: mkFileBrowser_rayWidg
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

   /*path to file label/rectangle*/
   addWidget_widg_rayWidg(
      0, /*colomn*/
      0, /*row*/
      0, /*use x,y*/
      200, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   );
   childAdd_widg_rayWidg(idSI + 1, widgSTPtr);
   inactiveAdd_widg_rayWidg(idSI + 1, widgSTPtr);

   /*pick file list box (adds 4 children widgets)*/
   mk_listBox_rayWidg(0, 0, 0, widgSTPtr);
   childAdd_widg_rayWidg(idSI + 2, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 3, widgSTPtr);
      /*actual widget*/

   /*ok button*/
   addWidget_widg_rayWidg(
      0, /*colomn*/
      0, /*row*/
      0, /*use x,y*/
      20, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   );
   childAdd_widg_rayWidg(idSI + 6, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 6, widgSTPtr);

   /*cancel button*/
   addWidget_widg_rayWidg(
      1, /*colomn*/
      0, /*row*/
      0, /*use x,y*/
      20, /*width*/
      widgSTPtr->fontHeightF, /*height*/
      widgSTPtr
   );
   childAdd_widg_rayWidg(idSI + 7, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 7, widgSTPtr);

   /*file extension list box (adds 4 children widgets)*/
   mk_listBox_rayWidg(0, 0, 0, widgSTPtr);
   childAdd_widg_rayWidg(idSI + 8, widgSTPtr);
   hogAdd_widg_rayWidg(idSI + 9, widgSTPtr);
      /*actual widget*/

   hidenAdd_widg_rayWidg(idSI, widgSTPtr);
      /*hide the message box*/
   return idSI;

   memErr_fun00a:;
      return -1;
} /*mkFileBrowser_rayWidg*/

/*-------------------------------------------------------\
| Fun00y: fileBrowserDraw_rayWidg
|   - draws a file browswer widget
| Input:
|   - idSI:
|     o parent id of file browser widget
|   - mesgStr:
|     o c-string with message to print for user
|   - pwdStr:
|     o present working directory
|   - fileAryStr:
|     o c-string with files to list
|     o each file has def_maxFileLen_rayWidg - 1 bytes
|   - fileLenSI:
|     o with number of files in filesStrPtr
|   - filePosSI:
|     o file on in file indexArySI
|   - fileExtAryStr:
|     o c-string array with file extensions to look for
|     o the first character in the file extension on
|       should be negative (let event handle this),
|       otherwise the first option is used
|     o `dir` stands for allow selection of directories
|   - extLenSI:
|     o number of file extensions looking for
|   - fileExtPosSI:
|     o position at in file extension box
|   - widthSI:
|     o width of the GUI
|   - heightSI:
|     o height of the GUI
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with file browser
|       to draw
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
   signed int idSI,          /*id of file browser*/
   signed char *mesgStr,     /*message for user*/
   signed char *pwdStr,      /*present working directory*/
   signed char *fileAryStr,  /*has files in directory*/
   signed int *fileIndexArySI,/*index of each file*/
   signed int fileLenSI,
   signed int filePosSI,    /*position in fileIndexArySI*/
   signed char *fileExtAryStr[],/*file extensions*/
   signed int extLenSI,       /*number extensions*/
   signed int fileExtPosSI,   /*file extension on*/
   signed int widthSI,        /*width of GUI*/
   signed int heightSI,       /*height of GUI*/
   struct widg_rayWidg *widgSTPtr /*has file browser*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun00y TOC:
   '   - draws a file browswer widget
   '   o fun00y sec01:
   '     - variable declarations
   '   o fun00y sec02:
   '     - check if can draw file browswer
   '   o fun00y sec03:
   '     - get coordinates and dimensions for all widgets
   '   o fun00y sec04:
   '     - draw file browswer
   '   o fun00y sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00y Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int indexSI = 0;
   signed int tmpSI = 0;
   signed char selectBl = 0;

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   #define def_maxStrLen_fun00y 512

   signed char pwdOutStr[def_maxStrLen_fun00y + 8];
      /*holds modified pwdStr*/
   signed int pwdWidthSI = 0;  /*pwd print width*/

   signed char mesgOutStr[def_maxStrLen_fun00y + 8];
      /*holds modified message*/
   signed int mesgWidthSI = 0;  /*width of message*/
   signed int mesgHeightSI = 0; /*height of message*/
   signed int mesgLinesSI = 0; /*number lines in message*/

   signed int extWidthSI = 0;

   struct Color textColST; /*for drawing file names*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00y Sec02:
   ^   - check if can draw file browswer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidg_fun00y_sec05;
   else if(idSI < 0)
      goto noWidg_fun00y_sec05;
   else if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hiden_fun00y_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00y Sec03:
   ^   - get coordinates and dimensions for all widgets
   ^   o fun00y sec03 sub01:
   ^     - find file browswer rectangle dimensions
   ^   o fun00y sec03 sub02:
   ^     - find present working directory label dimensions
   ^   o fun00y sec03 sub03:
   ^     - find file list box dimensions
   ^   o fun00y sec03 sub04:
   ^     - extension list box dimensions
   ^   o fun00y sec03 sub05:
   ^     - message box dimensions
   ^   o fun00y sec03 sub06:
   ^     - get button coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun00y Sec03 Sub01:
   *   - find file browswer rectangle dimensions
   \*****************************************************/
   /*center the file browswer and make take up most of the
   `  GUI
   */
   widgSTPtr->heightArySI[idSI] =
      heightSI - (widgSTPtr->fontHeightSI << 1);
   widgSTPtr->widthArySI[idSI] =
      widthSI - (widgSTPtr->fontWidthSI << 1);

   widgSTPtr->yArySI[idSI] = widgSTPtr->fontHeightSI;
   widgSTPtr->xArySI[idSI] = widgSTPtr->fontWidthSI;

   widgSTPtr->xArySI[idSI] = widgSTPtr->fontWidthSI;
   widgSTPtr->yArySI[idSI] = widgSTPtr->fontHeightSI;

   /*****************************************************\
   * Fun00y Sec03 Sub02:
   *   - find present working directory label dimensions
   *   o fun00y sec03 sub02 cat01:
   *     - pwd general dimensions
   *   o fun00y sec03 sub02 cat02:
   *     - copy pwd string and shorten (if needed)
   *   o fun00y sec03 sub02 cat03:
   *     - make sure pwd string fits in file browser
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub02 Cat01:
   +   - pwd rectangle general dimensions
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   widgSTPtr->xArySI[idSI + 1] = widgSTPtr->xArySI[idSI];
   widgSTPtr->yArySI[idSI + 1] = widgSTPtr->yArySI[idSI];

   widgSTPtr->heightArySI[idSI + 1] =
        widgSTPtr->fontHeightSI
      + (
          widgSTPtr->fontHeightSI
         / (def_widgHeightGap_rayWidg / 2)
        );

   widgSTPtr->widthArySI[idSI + 1] =
        widgSTPtr->widthArySI[idSI]
      - (widgSTPtr->fontWidthSI << 1);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub02 Cat02:
   +   - copy pwd string and shorten (if needed)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpSI = def_maxStrLen_fun00y - 1;
   pwdOutStr[def_maxStrLen_fun00y] = 0;
   cpStr = pwdStr;
   widthSI = endStr_ulCp(pwdStr);
   cpStr += widthSI - 1;

   while(cpStr != pwdStr && tmpSI)
      pwdOutStr[tmpSI--] = *cpStr--;

   if(! tmpSI && cpStr != pwdStr)
   { /*If: need to shorten the file path*/
      pwdOutStr[0] = '.';
      pwdOutStr[1] = '.';
      pwdOutStr[2] = '.';
      pwdOutStr[3] = def_pathSep_rayWidg;

      tmpSI = 4;
      while(
            pwdOutStr[tmpSI]
         && pwdOutStr[tmpSI] != def_pathSep_rayWidg
      ) ++tmpSI;

      if(! pwdOutStr[tmpSI])
         pwdOutStr[5] = 0;
      else
      { /*Else: can shorten file path*/
         cpLen_ulCp(
            &pwdOutStr[4],
            &pwdOutStr[tmpSI + 1],
            widthSI - tmpSI - 1
         );
      } /*Else: can shorten file path*/
   } /*If: need to shorten the file path*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub02 Cat03:
   +   - make sure pwd string fits in file browser
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   widthSI =
      textMeasure_widg_rayWidg(
         pwdOutStr,
         &heightSI,
         widgSTPtr
      );

   if(widthSI < widgSTPtr->widthArySI[idSI + 1])
   { /*If: directory path is to long*/
      tmpSI = 0;
      indexSI = 0;

      do{
         while(
               pwdOutStr[tmpSI]
            && pwdOutStr[tmpSI] != def_pathSep_rayWidg
         ) ++tmpSI;

         pwdOutStr[tmpSI] = 0;

         widthSI =
            textMeasure_widg_rayWidg(
               &pwdOutStr[indexSI],
               &heightSI,
               widgSTPtr
            );
         indexSI = tmpSI + 1;
      } while(widthSI < widgSTPtr->widthArySI[idSI + 1]);

      pwdOutStr[0] = '.';
      pwdOutStr[1] = '.';
      pwdOutStr[2] = '.';
      pwdOutStr[3] = def_pathSep_rayWidg;

      cpLen_ulCp(
         &pwdOutStr[4],
         &pwdOutStr[indexSI],
         widthSI - indexSI
      );
   } /*If: directory path is to long*/

   /*****************************************************\
   * Fun00y Sec03 Sub03:
   *   - find file list box dimensions
   \*****************************************************/

   widgSTPtr->heightArySI[idSI + 2] =
        ((widgSTPtr->heightArySI[idSI] / 3) * 2)
      - widgSTPtr->heightArySI[idSI + 1]
      / (def_widgHeightGap_rayWidg / 2);

   widgSTPtr->widthArySI[idSI + 2] =
        widgSTPtr->widthArySI[idSI]
      - (widgSTPtr->fontWidthSI << 1);

   widgSTPtr->xArySI[idSI + 2] = widgSTPtr->xArySI[idSI]

   widgSTPtr->yArySI[idSI + 2] =
      widgSTPtr->heightArySI[idSI + 1];

   /*****************************************************\
   * Fun00y Sec03 Sub04:
   *   - extension list box dimensions
   \*****************************************************/

   /*find longest file extension length*/
   for(indexSI = 0; indexSI < extLenSI; ++indexSI)
   { /*Loop: find the length of the longest extension*/
      
      if(fileExtAryStr[indexSI][0] < 0)
      { /*If: file extenion is current extentsion*/
         selectBl = 1;
         fileExtAryStr[indexSI][0] *= -1;
      } /*If: file extenion is current extentsion*/

      tmpSI =
         textMeasure_widg_rayWidg(
            fileExtAryStr[indexSI],
            &heightSI,
            widgSTPtr
         );

      if(selectBl)
      { /*If: file extenion is current extentsion*/
         selectBl = 0;
         fileExtAryStr[indexSI][0] *= -1;
      } /*If: file extenion is current extentsion*/

      if(tmpSI > extWidthSI)
         extWidthSI = tmpSI;
   } /*Loop: find the length of the longest extension*/

   extWidthSI += (widgSTPtr->fontWidthSI << 1);
   widgSTPtr->widthArySI[idSI + 5] = extWidthSI;

   widgSTPtr->heightArySI[idSI + 5] =
        widgSTPtr->heightArySI[idSI + 1]
      + widgSTPtr->heightArySI[idSI + 2]
      + widgSTPtr->fontHeightSI
      / (def_widgHeightGap_rayWidg / 2);

   widgSTPtr->xArySI[idSI + 5] =
      widgSTPtr->widthArySI[idSI] - extWidthSI;
   widgSTPtr->yArySI[idSI + 5] =
        widgSTPtr->yArySI[idSI + 2]
      + widgSTPtr->heightArySI[idSI + 2]
      + widgSTPtr->fontHeightSI
      / (def_widgHeightGap_rayWidg / 2);

   /*****************************************************\
   * Fun00y Sec03 Sub05:
   *   - message box dimensions
   *   o fun00y sec03 sub05 cat01:
   *     - find space avaliable for the message
   *   o fun00y sec03 sub05 cat02:
   *     - copy message and break into lines as needed
   *   o fun00y sec03 sub05 cat03:
   *     - shorten message if needed
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub05 Cat01:
   +   - find space avaliable for the message
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   mesgWidthSI =
        widgSTPtr->widthArySI[idSI]
      - (widgSTPtr->fontWidthSI << 1);
      - extWidthSI;

   heightSI =
        widgSTPtr->yArySI[idSI + 3]
      + widgSTPtr->heightArySI[idSI + 3]
      + widgSTPtr->fontHeightSI
      / (def_widgHeightGap_rayWidg / 2);

   /*account for buttons at botton*/
   heightSI -= widgSTPtr->fontHeightSI;
   heightSI -= 
      (
          widgSTPtr->fontHeightSI
        / (def_widgHeightGap_rayWidg / 2)
      ) * 2;
      /**2 is to account for the buttons internal
      `  padding and the external padding between text
      `  and button
      */

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub05 Cat02:
   +   - copy message and break into lines as needed
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      
   /*copy message for file browser*/
   cpStr = mesgStr;
   dupStr = mesgOutStr;
   tmpSI = 0;
   messageLinesSI = 1;

   while(*cpStr)
   { /*Loop: copy message to display*/
      if(*cpStr == '\n' || *cpStr == '\r')
      { /*If: new line or carriage return*/
         ++cpStr;
         continue;
      } /*If: new line or carriage return*/

      tmpSI +=
         textMeasure_widg_rayWidg(
            mesgStr,
            &heightSI,
            widgSTPtr
         );

      if(tmpSI >= mesgWidthSI)
      { /*If: need a new line*/
         *dupStr++ = '\n';

         tmpSI =
            textMeasure_widg_rayWidg(
               mesgStr,
               &indexSI, /*throwing away*/
               widgSTPtr
            );

         ++mesgLinesSI;
      } /*If: need a new line*/

      *dupStr++ = *cpStr++;
   } /*Loop: copy message to display*/

   *dupStr = 0;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec03 Sub05 Cat03:
   +   - shorten message if needed
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpSI =
        mesgLinesSI
      * widgSTPtr->fontHeightSI
      * widgSTPtr->spacingF;

   if(tmpSI > heightSI)
   { /*If: need to reduce the message height*/
      while(dupStr != mesgOutStr)
      { /*Loop: remove lines*/
         if(*dupStr == '\n')
         { /*If: found a line to remove*/
            --mesgLinesSI;
            tmpSI =
                 mesgLinesSI
               * widgSTPtr->fontHeightSI
               * widgSTPtr->spacingF;
         } /*If: found a line to remove*/

         *dupStr-- = 0; 
      } /*Loop: remove lines*/

      dupStr[1] = 0; /*make sure*/
      *dupStr-- = '.';
      *dupStr-- = '.';
      *dupStr-- = '.';
      *dupStr-- = ' ';
   } /*If: need to reduce the message height*/

   mesgWidthSI =
      textMeasure_widg_rayWidg(
         mesgOutStr,
         &mesgHeightSI,
         widgSTPtr
      ); /*get final message dimensions*/

   /*get height when accounting for padding*/
   mesgHeightSI +=
        widgSTPtr->fontHeightSI
      / (def_widgHeightGap_rayWidg / 2);

   /*****************************************************\
   * Fun00y Sec03 Sub06:
   *   - get button coordinates
   \*****************************************************/

   tmpSI =
      butDraw_rayWidg(
         200, /*maximum width*/
         20,  /*minimum width*/
         idSI + 3,
         (signed char *) "Select",
         1, /*do not draw the button yet*/
         widgSTPtr
      );

   widthSI = tmpSI + widgSTPtr->fontWidthSI;

   widthSI +=
      butDraw_rayWidg(
         200, /*maximum width*/
         20,  /*minimum width*/
         idSI + 4,
         (signed char *) "Cancel",
         1, /*do not draw the button yet*/
         widgSTPtr
      );
   

   widgSTPtr->xArySI[idSI + 3] =
      (widgSTPtr->widthArySI[idSI] - widthSI) >> 1;
      /*gives center point for button*/

   widgSTPtr->xArySI[idSI + 3] -= (widthSI >> 1);
      /*this is the position needed to center buttons*/

   widgSTPtr->xArySI[idSI + 4] = 
        widgSTPtr->xArySI[idSI + 3]
      + tmpSI
      + widgSTPtr->fontWidthSI;

   widgSTPtr->yArySI[idSI + 3] = 
        widgSTPtr->yArySI[idSI + 2]
      + widgSTPtr->heightArySI[idSI + 2]
      + mesgHeightSI;
      
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00y Sec04:
   ^   - draw file browswer
   ^   o fun00y sec04 sub01:
   ^     - draw in the intial shapes
   ^   o fun00y sec04 sub02:
   ^     - add the message
   ^   o fun00y sec04 sub03:
   ^     - add the extensions
   ^   o fun00y sec04 sub04:
   ^     - add the files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun00y Sec04 Sub01:
   *   - draw in the intial shapes
   \*****************************************************/

   drawRect_rayWidg(
      widgSTPtr,
      idSI,
      1, /*has border, but not in focus*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
   ); /*background rectangle to hold file browser*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 1,
      1, /*has border, but not in focus*/
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
   ); /*rectangle for the present working directory*/

   if(foucsGet_widg_rayWidg(idSI + 2, widgSTPtr)
      tmpSI = 3;
   else
      tmpSI = 1;

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 2,
      tmpSI,
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
   ); /*rectangle for the listed files*/

   if(foucsGet_widg_rayWidg(idSI + 5, widgSTPtr)
      tmpSI = 3;
   else
      tmpSI = 1;

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 5,
      tmpSI,
      widgSTPtr->guiColSI,
      widgSTPtr->borderColSI,
      widgSTPtr->focusColSI
   ); /*rectangle for pick file extensions box*/

   butDraw_rayWidg(
      200, /*maximum width*/
      20,  /*minimum width*/
      idSI + 3,
      (signed char *) "Select",
      0,
      widgSTPtr
   );

   butDraw_rayWidg(
      200, /*maximum width*/
      20,  /*minimum width*/
      idSI + 4,
      (signed char *) "Cancel",
      0,
      widgSTPtr
   );

   /*****************************************************\
   * Fun00y Sec04 Sub02:
   *   - add the message
   \*****************************************************/

   drawTextEx(
      widgSTPtr->fontSTPtr,
      (char *) mesgOutStr,
      (Vector2) {
         widgSTPtr->xArySI[idSI] + widgSTPtr->fontWidthSI,
         widgSTPtr->yArySI[idSI + 3]
           + widgSTPtr->heightArySI[idSI + 3]
           + widgSTPtr->fontHeightSI
           / (def_widgHeightGap_rayWidg / 2)
      ),
      widgSTPtr->fontSizeSI,
      widgSTPtr->spacingF,
      GetColor(widgSTPtr->textColSI)
   );

   /*****************************************************\
   * Fun00y Sec04 Sub03:
   *   - add the extensions
   *   o fun00y sec04 sub03 cat01:
   *     - draw the active extension
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec04 Sub03 Cat01:
   +   - draw the active extension
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   for(indexSI = 0; indexSI < extLenSI; ++indexSI)
   { /*Loop: find current extension on*/
      if(fileExtAryStr[indexSI][0] < 0)
         break; /*found the extension*/
   } /*Loop: find current extension on*/

   if(indexSI >= extLenSI)
   { /*If: user did not specify an extension*/
      indexSI = 0;
      fileExtAryStr[indexSI][0] *= -1;
   } /*If: user did not specify an extension*/

   heightSI = widgSTPtr->heightArySI[idSI + 5];
   widgSTPtr->heightArySI[idSI + 5] =
        widgSTPtr->fontHeightSI
      + widgSTPtr->fontHeightSI
      / def_widgHeightGap_rayWidg;
 
   DrawRect_rayWidg(
      widgSTPtr,
      idSI + 5,
      0,        /*do not want a border*/
      widgSTPtr->activeColSI,
      0, /*no border*/
      0  /*not drawing focus*/
   );

   fileExtAryStr[indexSI][0] *= -1;

   drawTextEx(
      widgSTPtr->fontSTPtr,
      (char *) fileExtAryStr[indexSI],
      (Vector2) {
         widgSTPtr->xArySI[idSI + 5]
           + widgSTPtr->fontWidthSI,
         widgSTPtr->heightArySI[idSI + 5]
           + widgSTPtr->fontHeightSI
           / def_widgHeightGap_rayWidg;
      ),
      widgSTPtr->fontSizeSI,
      widgSTPtr->spacingF,
      GetColor(widgSTPtr->textAltColSI)
   );

   fileExtAryStr[indexSI][0] *= -1;
   widgSTPtr->heightArySI[idSI + 5] ^= heightSI;
   heightSI ^= widgSTPtr->heightArySI[idSI + 5];
   widgSTPtr->heightArySI[idSI + 5] ^= heightSI;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec04 Sub03 Cat02:
   +   - draw visible extensions
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   heightSI += 
      widgSTPtr->fontHeightSI / def_widgHeightGap_rayWidg;

   while(
        heightSI
           + widgSTPtr->fontHeightSI
           + widgSTPtr->fontHeightSI
          / def_widgHeightGap_rayWidg;
      < widgSTPtr->heighArySI[idSI + 5]
   { /*Loop: draw remaing extensions*/
      if(fileExtPosSI >= extLenSI)
         break; /*no more extensions to draw*/

      else if(fileExtPosSI == indexSI)
      { /*Else If: is used extension*/
          ++fileExtPosSI;
          continue;
      } /*Else If: is used extension*/
      

      drawTextEx(
         widgSTPtr->fontSTPtr,
         (char *) fileExtAryStr[fileExtPosSI],
         (Vector2) {
            widgSTPtr->xArySI[idSI + 5]
              + widgSTPtr->fontWidthSI,
            heightSI
         ),
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         GetColor(widgSTPtr->textColSI)
      );

      heightSI += 
          widgSTPtr->fontHeightSI
        + widgSTPtr->fontHeightSI
        / def_widgHeightGap_rayWidg;
      ++fileExtPosSI;
   } /*Loop: draw remaing extensions*/

   /*****************************************************\
   * Fun00y Sec04 Sub04:
   *   - add the files
   *   o fun00y sec04 sub04 cat01:
   *     - add up one directory (if at file 0)
   *   o fun00y sec04 sub04 cat02:
   *     - start file loop & check if valid file type
   *   o fun00y sec04 sub04 cat03:
   *     - check if file/dir was selected by user
   *   o fun00y sec04 sub04 cat04:
   *     - get base name of file/dir and resize (if need)
   *   o fun00y sec04 sub04 cat05:
   *     - draw file/dir name and move to next file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec04 Sub04 Cat01:
   +   - add up one directory (if at file 0)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   heightSI =
        widgSTPtr->heightArySI[idSI + 2]
      + widgSTPtr->fontHeightSI 
      / def_widgHeightGap_rayWidg;

   if(! filePosSI)
   { /*If: on first file*/
      drawTextEx(
         widgSTPtr->fontSTPtr,
         "[-] <---",
         (Vector2) {
            widgSTPtr->xArySI[idSI + 2]
              + widgSTPtr->fontWidthSI,
            heightSI
         ),
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         GetColor(widgSTPtr->textColSI)
      );

      heightSI += 
          widgSTPtr->fontHeightSI
        + widgSTPtr->fontHeightSI
        / def_widgHeightGap_rayWidg;
   } /*If: on first file*/

   heightSI += 
      widgSTPtr->fontHeightSI / def_widgHeightGap_rayWidg;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00y Sec04 Sub04 Cat02:
   +   - start file loop & check if valid file type
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(
        heightSI
           + widgSTPtr->fontHeightSI
           + widgSTPtr->fontHeightSI
          / def_widgHeightGap_rayWidg;
      < widgSTPtr->heighArySI[idSI + 2]
   { /*Loop: draw remaing files*/
      if(filePosSI >= fileLenSI)
         break; /*no more files to draw*/

      tmpSI =
         checkExt_rayWidg(
            &fileAryStr[
               filePosSI * def_maxFileLen_rayWidg
            ],
            fileExtAryStr,
            extLenSI,
            indexSI
         );

      if(tmpSI <= 0)
      ){ /*If: file is wrong extension*/
         ++filePosSI;
         continue;
      }  /*If: file is wrong extension*/

      else if(tmpSI == 1)
      ){ /*Else If: is a file*/
         pwdOutStr[0] = ' ';
         pwdOutStr[1] = '-';
         pwdOutStr[2] = ' ';
         pwdOutStr[3] = ' ';
      }  /*Else If: is a file*/

      else
      ){ /*Else: is a directory*/
         pwdOutStr[0] = '[';
         pwdOutStr[1] = '-';
         pwdOutStr[2] = ']';
         pwdOutStr[3] = ' ';
      }  /*Else: is a directory*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00y Sec04 Sub04 Cat03:
      +   - check if file/dir was selected by user
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      cpStr =
         &fileAryStr[filePosSI * def_maxFileLen_rayWidg];

      if(*cpStr < 0)
      { /*If: this file is selected*/
         textColST = GetColor(widgSTPtr->textAltColSI);

         widgSTPtr->heightArySI[idSI + 2] ^= heightSI;
         heightSI ^= widgSTPtr->heightArySI[idSI + 2];
         widgSTPtr->heightArySI[idSI + 2] ^= heightSI;

 
         DrawRect_rayWidg(
            widgSTPtr,
            idSI + 2,
            0,        /*do not want a border*/
            widgSTPtr->activeColSI,
            0, /*no border*/
            0  /*not drawing focus*/
         );

         heightSI ^= widgSTPtr->heightArySI[idSI + 2];
         widgSTPtr->heightArySI[idSI + 2] ^= heightSI;
         heightSI ^= widgSTPtr->heightArySI[idSI + 2];
         *cpStr *= -1;

         pwdOutStr[1] = '*';
      } /*If: this file is selected*/

      else
         textColST = GetColor(widgSTPtr->textColSI);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00y Sec04 Sub04 Cat04:
      +   - get base name of file/dir and resize (if need)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*get the basename of the file or directory*/
      dupStr = &pwdOutStr[4];

      while(*cpStr)
      { /*Loop: get the base name of the file*/
         if(cpStr[tmpSI] == def_pathSep_rayWidg)
         { /*If: found file separator*/
            if(! cpStr[1])
               ; /*end of string*/
            else
               dupStr = &pwdOutStr[4];
               /*need to restart copy*/
            ++cpStr;
            continue;
         } /*If: found file separator*/

         *dupStr++ = *cpStr++;
      } /*Loop: get the base name of the file*/

      *dupStr = 0;

      checkTextWidth_rayWidg(
         pwdOutStr,
         pwdOutStr,
         widgSTPtr->widthArySI[idSI]
            - (widgSTPtr->fontWidthSI << 1),
         widgSTPtr
      ); /*shorten down the name if needed*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00y Sec04 Sub04 Cat05:
      +   - draw file/dir name and move to next file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/


      drawTextEx(
         widgSTPtr->fontSTPtr,
         (char *) pwdOutStr,
         (Vector2) {
            widgSTPtr->xArySI[idSI + 2]
              + widgSTPtr->fontWidthSI,
            heightSI
         },
         widgSTPtr->fontSizeSI,
         widgSTPtr->spacingF,
         textColST
      );

      if(
           fileAryStr[filePosSI * def_maxFileLen_rayWidg]
         < 0
      )fileAryStr[filePosSI * def_maxFileLen_rayWidg]*=-1;

      heightSI += 
          widgSTPtr->fontHeightSI
        + widgSTPtr->fontHeightSI
        / def_widgHeightGap_rayWidg;

      ++fileExtPosSI;
   } /*Loop: draw remaing files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00y Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   noWidg_fun00y_sec05:;
      return def_noWidget_rayWidg;
   hiden_fun00y_sec05:;
      return 0;
} /*fileBrowserDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun00z: fileBrowserEvent_rayWidg
|   - process and event for the file browser
| Input:
|   - retFileAryStr:
|     o signed char pointer to get the selected file
|       list if the user selected the ok event
|   - childSI:
|     o id of child widget on
|   - parIdSI:
|     o parent id of file browser widget
|   - pwdStr:
|     o present working directory
|   - filesAryStrPtr:
|     o c-string array pointer with files to list
|     o each file has def_maxFileLen_rayWidg - 1 bytes
|   - fileLenSIPtr:
|     o signed int pointer with number of files in
|       filesStrPtr
|   - filePosSIPtr:
|     o signed int pointer with file users is currently on
|   - extAryStr:
|     o c-string array with file extensions to look for
|     o `dir` stands for allow selection of directories
|   - extLenSI:
|     o number of file extensions looking for
|   - extPosSIPtr:
|     o signed int with the position at in file extension
|       box
|   - keySI:
|     o key input into file browser or 0 if none
|     o is from raylib's `GetKeyPressed()` function
|     o do not include tabs (use focusCheck_widg_rayWidg
|       for tab events)
|   - scrollF:
|     o number of scroll units moved (0 for none)
|   - ySI:
|     o y-axis mouse position
|   - eventSC:
|     o 0 for no click event
|     o 1 for left press event
|     o 2 for left release event
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with file browser
|       to draw
| Output:
|   - Modifies:
|     - retFileAryStr to have selected files if user hit
|       select
|     - pwdStr to have the working directory the user
|       changed to
|     - resizes filesStrPtr if more spaces is needed for
|       files
|     - sets frist character in selected file to -1
|       extAryStr to negative (* -1)
|     - changes fileSizeSIPtr to have resized
|       filesAryStrPtr size
|     - filePosSIPtr to have new file position (if change)
|     - sets frist character in used extension in
|       extAryStr to negative (* -1)
|     - extPosSIPtr to have new extension position
|   - Returns:
|     o 2 for cancel event
|     o 1 for ok event
|       - modifies: fileStrPtr to ony have selected files
|       - modifies: fileIndexArySI to ony have selected
|         files
|     o 0 for no major event
|     o -1 for memory errors
\-------------------------------------------------------*/
signed char
fileBrowserEvent_rayWidg(
   signed int childIdSI,          /*id of child widget*/
   signed int parIdSI,            /*id of parent widget*/
   struct files_rayWidg *fileSTPtr,/*has files & pwd*/
   signed int keySI,              /*key user input*/
   float scrollF,                 /*amount scroll moved*/
   signed int ySI,                /*y coordinate*/
   signed char eventSC,           /*1 press; 2 release*/
   struct widg_rayWidg *widgSTPtr
){
   ^ Fun00z Sec01:
   ^   - variable declarations
   ^ Fun00z Sec02:
   ^   - check if widget can take events
   ^ Fun00z Sec0x:
   ^   - return

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00z Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int tmpSI = 0; 
   signed int maxLinesSI = 0;
   signed int changeSI = 0;
   signed int lastSI = 0;
   signed int cntSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00z Sec02:
   ^   - check if widget can take events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidg_fun00z_sec0x;
   else if(parIdSI < 0)
      goto noWidg_fun00z_sec0x;
   else if( hidenGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hiden_fun00z_sec0x;

   if(childIdSI < 0)
      goto noEvent_fun00z_sec0x;
   else if(inactiveGet_widg_rayWidg(childIdSI))
      goto noEvent_fun00z_sec0x;

   if(scrollF > 0)
      ;
   else if(keySI)
      ;
   else if(! evenSC)
      goto noEvent_fun00z_sec0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00z Sec03:
   ^   - find the event
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(childIdIS == parIdSI + 4)
      goto cancle_fun00z_sec0x;

   else if(childIdIS == parIdSI + 3)
      goto seletectedFiles_fun00z_sec0x;

   /*****************************************************\
   * Fun00z Sec03 Sub02:
   *   - handel extension box events
   *   o fun00z sec03 sub02 cat01:
   *     - if extension box event
   *   o fun00z sec03 sub02 cat02:
   *     - if down key * check if key event
   *   o fun00z sec03 sub02 cat03:
   *     - if up key
   *   o fun00z sec03 sub02 cat04:
   *     - if home/end (go to start/end) key
   *   o fun00z sec03 sub02 cat05:
   *     - if enter key
   *   o fun00z sec03 sub02 cat07:
   *     - if scroll wheel event
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00z Sec03 Sub02 Cat01:
   +   - if extension box event
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(childIdIS == parIdSI + 5)
   { /*Else If: extension event*/
      maxLinesSI = widgSTPtr->heightArySI[childIdSI];
      maxLinesSI /=
         (
              widgSTPtr->fontHeightSI;
            + widgSTPtr->fontHeightSI
            / (def_widgHeightGap_rayWidg / 2);
         ); /*space per entry*/
      --maxLinesSI; /*account for extension used*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub02 Cat02:
      +   - if down key + check if key event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(keySI)
      { /*If: was a key event*/
         if(keySI == KEY_J || keySI == KEY_DOWN)
         { /*If: going down one entry*/
            if(
                  IsKeyPressed(KEY_LEFT_SHIFT)
               || IsKeyPressed(KEY_RIGHT_SHIFT)
            ){ /*If: moving one page down*/
               if(
                    fileSTPtr->extScrollSI + maxLinesSI
                  < fileSTPtr->extLenSI
               ){ /*If: not at end of list*/
                  fileSTPtr->extScrollSI += maxLinesSI;
                  fileSTPtr->extOnSI =
                     fileSTPtr->extScrollSI;
               }  /*If: not at end of list*/

               else
               { /*Else: at end of list*/
                  fileSTPtr->extScrollSI =
                     fileSTPtr->extLenSI - 1;
                  fileSTPtr->extOnSI =
                     fileSTPtr->extScrollSI;
               } /*Else: at end of list*/
            }  /*If: moving one page down*/

            else
            { /*Else: moving one extension*/
               if(
                    fileSTPtr->extOnSI + 1
                  < fileSTPtr->extLenSI
               ){ /*If: can move down one extension*/
                  ++fileSTPtr->extOnSI;
                  tmpSI = fileSTPtr->extOnSI;
                  tmpSI -= maxLinesSI;

                  if(tmpSI > fileSTPtr->extScrollSI)
                     fileSTPtr->extScrollSI =
                        fileSTPtr->extOnSI;
                        /*moved past down*/
               }  /*If: can move down one extension*/
            } /*Else: moving one extension*/
         } /*If: going down one entry*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub02 Cat03:
         +   - if up key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(keySI == KEY_K || keySI == KEY_UP)
         { /*Else If: going up one entry*/
            if(
                  IsKeyPressed(KEY_LEFT_SHIFT)
               || IsKeyPressed(KEY_RIGHT_SHIFT)
            ) fileSTPtr->extOnSI -= maxLinesSI;
              /*move down a page*/

            else
               --fileSTPtr->extOnSI;
               /*move down an extension*/

            if(fileSTPtr->extOnSI < 0)
               fileSTPtr->extOnSI = 0;
            if(fileSTPtr->extOnSI <fileSTPtr->extScrollSI)
               fileSTPtr->extScrollSI =fileSTPtr->extOnSI;
         } /*Else If: going up one entry*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub02 Cat04:
         +   - if home/end (go to start/end) key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(
               (
                     keySI == KEY_G
                  && ! IsKeyPressed(KEY_LEFT_SHIFT)
                  && ! IsKeyPressed(KEY_RIGHT_SHIFT)
               )
            || keySI == KEY_HOME
         ){ /*Else If: go to start*/
            fileSTPtr->extScrollSI = 0;
            fileSTPtr->extOnSI = 0;
         }  /*Else If: go to start*/

         else if(
               (
                     keySI == KEY_G
                  && (
                         IsKeyPressed(KEY_LEFT_SHIFT)
                      || IsKeyPressed(KEY_RIGHT_SHIFT)
                     )
               ) /*capital G (shift+g)*/
            || keySI == KEY_END
         ){ /*If: go to end*/
            fileSTPtr->extScrollSI =
               fileSTPtr->extLenSI - 1;
            fileSTPtr->extOnSI = fileSTPtr->extLenSI - 1;
         }  /*If: go to end*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub02 Cat05:
         +   - if enter key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(keySI == KEY_ENTER)
            fileSTPtr->extSelectSI = fileSTPtr->extOnSI;

         else
             goto noEvent_fun00z_sec0x;
             /*unreconginzed key input*/
      } /*If: was a key event*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub02 Cat06:
      +   - if click event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(eventSC & 1)
         ; /*do nothing for press events*/

      else if(eventSC & 2)
      { /*Else If: release event*/
         ySI -= widgSTPtr->yArySI[childIdSI];

         if(ySI <= widgSTPtr->fontHeightSI)
             goto noEvent_fun00z_sec0x;
             /*nothing was selected*/

         ySI /=
            (
                 widgSTPtr->fontHeightSI;
               + widgSTPtr->fontHeightSI
               / (def_widgHeightGap_rayWidg / 2);
            ); /*space per entry*/

         --ySI; /*account for first item being selected*/

         if(ySI >= fileSTPtr->extLenSI)
             goto noEvent_fun00z_sec0x;
             /*empty space was clicked*/

         else
         { /*Else: extension selected*/
            fileSTPtr->extSelectSI = ySI;
            fileSTPtr->extOnSI = ySI;
         } /*Else: extension selected*/
      } /*Else If: release event*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub02 Cat07:
      +   - if scroll wheel event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(scrollF > 0)
      { /*Else If: scroll wheel was done*/
         fileSTPtr->extScrollSI += (signed int) scrollF;

         if(fileSTPtr->extScrollSI < 0)
            fileSTPtr->extScrollSI = 0;
         else if(
            fileSTPtr->extScrollSI > fileSTPtr->extLenSI
         ) fileSTPtr->extScrollSI = fileSTPtr->extLenSI-1;

         if(fileSTPtr->extOnSI < fileSTPtr->extScrollSI)
            fileSTPtr->extOnSI = fileSTPtr->extScrollSI;

         else if(
              fileSTPtr->extOnSI
            > fileSTPtr->extScrollSI +  maxLinesSI
         ) fileSTPtr->extOnSI = fileSTPtr->extScrollSI;
      } /*Else If: scroll wheel was done*/
   } /*Else If: extension event*/

   /*****************************************************\
   * Fun00z Sec03 Sub03:
   *   - handel file selection box events
   *   o fun00z sec03 sub03 cat01:
   *     - if file seletion box event
   *   o fun00z sec03 sub03 cat02:
   *     - if down key * check if key event
   *   o fun00z sec03 sub03 cat03:
   *     - if up key
   *   o fun00z sec03 sub03 cat04:
   *     - if home/end (go to start/end) key
   *   o fun00z sec03 sub03 cat05:
   *     - if enter key
   *   o fun00z sec03 sub03 cat06:
   *     - if space key
   *   o fun00z sec03 sub03 cat07:
   *     - if scroll wheel event
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun00z Sec03 Sub03 Cat01:
   +   - if file selection box event
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(childIdIS == parIdSI + 2)
   { /*Else If: file window event*/
      maxLinesSI = widgSTPtr->heightArySI[childIdSI];
      maxLinesSI /=
         (
              widgSTPtr->fontHeightSI;
            + widgSTPtr->fontHeightSI
            / (def_widgHeightGap_rayWidg / 2);
         ); /*space per entry*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub03 Cat02:
      +   - if down key + check if key event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(keySI)
      { /*If: was a key event*/
         if(keySI == KEY_J || keySI == KEY_DOWN)
         { /*If: going down one entry*/
            if(
                  IsKeyPressed(KEY_LEFT_SHIFT)
               || IsKeyPressed(KEY_RIGHT_SHIFT)
            ) changeSI = maxLinesSI;

            else
               changeSI = 1;

            lastSI = fileSTPtr->fileOnSI;
            tmpSI = fileSTPtr->fileOnSI + 1;
            cntSI = 0;

            while(cntSI < changeSI)
            { /*Loop: find ending file*/
               if(tmpSI >= fileSTPtr->fileLenSI)
                  break;

               if( 
                  checkExt_rayWidg(
                     fileSTPtr->fileAryStr[
                        fileSTPtr->indexArySI[tmpSI]
                     ],
                     fileSTPtr->extAryStr,
                     fileSTPtr->extLenSI,
                     fileSTPtr->extSelectSI
                  ) > 0
               ){ /*If: found a file that can be used*/
                  --cntSI;
                  lastSI = tmpSI;
               }  /*If: found a file that can be used*/

               ++tmpSI
            } /*Loop: find ending file*/

            if(lastSI == fileSTPtr->fileOnSI)
               goto noEvent_fun00z_sec0x; /*on last file*/

            fileSTPtr->fileOnSI = lastSI;

            if(
                 lastSI
               > fileSTPtr->fileScrollSI + maxLinesSI
            ) fileSTPtr->fileScrollSI = lastSI;
              /*moved past current widnow*/
         } /*If: going down one entry*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub03 Cat03:
         +   - if up key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(keySI == KEY_K || keySI == KEY_UP)
         { /*Else If: going up one entry*/
            if(
                  IsKeyPressed(KEY_LEFT_SHIFT)
               || IsKeyPressed(KEY_RIGHT_SHIFT)
            ) changeSI = maxLinesSI;

            else
               changeSI = 1;

            lastSI = fileSTPtr->fileOnSI;
            tmpSI = fileSTPtr->fileOnSI - 1;
            cntSI = 0;

            while(cntSI < changeSI)
            { /*Loop: find ending file*/
               if(tmpSI < 0)
                  break;

               if( 
                  checkExt_rayWidg(
                     fileSTPtr->fileAryStr[
                        fileSTPtr->indexArySI[tmpSI]
                     ],
                     fileSTPtr->extAryStr,
                     fileSTPtr->extLenSI,
                     fileSTPtr->extSelectSI
                  ) > 0
               ){ /*If: found a file that can be used*/
                  --cntSI;
                  lastSI = tmpSI;
               }  /*If: found a file that can be used*/

               --tmpSI
            } /*Loop: find ending file*/

            if(lastSI == fileSTPtr->fileOnSI)
               goto noEvent_fun00z_sec0x; /*on last file*/

            fileSTPtr->fileOnSI = lastSI;

            if(lastSI < fileSTPtr->fileScrollSI)
               fileSTPtr->fileScrollSI = lastSI;
              /*moved before current widnow*/
         } /*Else If: going up one entry*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub03 Cat04:
         +   - if home/end (go to start/end) key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(
               (
                     keySI == KEY_G
                  && ! IsKeyPressed(KEY_LEFT_SHIFT)
                  && ! IsKeyPressed(KEY_RIGHT_SHIFT)
               )
            || keySI == KEY_HOME
         ){ /*Else If: go to start*/
            fileSTPtr->fileScrollSI = 0;
            fileSTPtr->fileOnSI = 0;
         }  /*Else If: go to start*/

         else if(
               (
                     keySI == KEY_G
                  && (
                         IsKeyPressed(KEY_LEFT_SHIFT)
                      || IsKeyPressed(KEY_RIGHT_SHIFT)
                     )
               ) /*capital G (shift+g)*/
            || keySI == KEY_END
         ){ /*If: go to end*/

            tmpSI = fileSTPtr->fileLenSI - 1;

            while(tmpSI > -1)
            { /*Loop: find last file*/
               if( 
                  checkExt_rayWidg(
                     fileSTPtr->fileAryStr[
                        fileSTPtr->indexArySI[tmpSI]
                     ],
                     fileSTPtr->extAryStr,
                     fileSTPtr->extLenSI,
                     fileSTPtr->extSelectSI
                  ) > 0
               ) break;

               --tmpSI
            } /*Loop: find last file*/

            if(tmpSI >= 0)
            { /*If: have file to show*/
               fileSTPtr->fileScrollSI = tmpSI;
               fileSTPtr->fileOnSI = tmpSI;
            } /*If: have file to show*/
         }  /*If: go to end*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub03 Cat05:
         +   - if enter key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(keySI == KEY_ENTER)
         { /*Else If: user hit enter key*/
            tmpSI = fileSTPtr->fileOnSI;

            if(
               DirectoryExists(
                  (char *) &fileSTPtr->fileAryStr[
                    fileSTPtr->indexArySI[tmpSI]
                  ]
               )
            ){ /*If: this was a direcotry not a file*/
               if(
                  get_files_rayWidg(
                     fileSTPtr->fileOnSI,
                     fileSTPtr
                  ) /*TODO: make this function*/
               ) goto memErr_fun00z_sec0x;
            }  /*If: this was a direcotry not a file*/

            else
            { /*Else: file was selected*/
               if(
                  addFile_files_rayWidg(
                     fileSTPtr->fileOnSI,
                     1, /*set this as the only file*/
                     fileSTPtr
                  ) /*TODO: add this function*/
               ) goto memErr_fun00z_sec0x;
            } /*Else: file was selected*/
         } /*Else If: user hit enter key*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun00z Sec03 Sub03 Cat06:
         +   - if space key
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(keySI == KEY_SPACE)
         { /*Else If: space key*/
            tmpSI =
               findSelect_files_rayWidg(
                  fileSTPtr->fileOnSI,
                  fileSTPtr
               ); /*TODO: add this function*/

            if(tmpSI < 0)
            { /*If: file was not selected*/
               if(
                     IsKeyDown(KEY_LEFT_SHIFT)
                  || IsKeyDown(KEY_RIGHT_SHIFT)
               ){ /*If: adding a range*/
                  if(
                     addFileRange_files_rayWidg(
                        fileSTPtr->fileOnSI,
                        fileSTPtr
                     ) /*TODO: add this function; should
                       `  add after last selected file
                       */
                  ) goto memErr_fun00z_sec0x;
               }  /*If: adding a range*/

               else
               { /*Else: adding a single file*/
                  if(
                     addFile_files_rayWidg(
                        fileSTPtr->fileOnSI,
                        0, /*not resting file selection*/
                        fileSTPtr
                     ) /*TODO: add this function*/
                  ) goto memErr_fun00z_sec0x;
               } /*Else: adding a single file*/
            } /*If: file was not selected*/

            else
            { /*Else: file was selected (remove)*/

               if(
                     IsKeyDown(KEY_LEFT_SHIFT)
                  || IsKeyDown(KEY_RIGHT_SHIFT)
               ){ /*If: removing a range*/
                  rmFileRange_files_rayWidg(
                     tmpSI,
                     fileSTPtr
                  ); /*TODO: add this function
                     `  should remove form last selected
                     `  file till tmpSI
                     */
               }  /*If: removing a range*/

               else
               { /*Else: removign a single file*/
                  rmFile_files_rayWidg(
                     tmpSI,
                     fileSTPtr
                  ); /*TODO: add this function*/
               } /*Else: removign a single file*/
            } /*Else: file was selected (remove)*/
         } /*Else If: space key*/

         else
             goto noEvent_fun00z_sec0x;
             /*unreconginzed key input*/
      } /*If: was a key event*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub03 Cat06:
      +   - if click event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(eventSC & 1)
         ; /*do nothing for press events*/

      else if(eventSC & 2)
      { /*Else If: release event*/
         ySI -= widgSTPtr->yArySI[childIdSI];

         if(ySI <= widgSTPtr->fontHeightSI)
             goto noEvent_fun00z_sec0x;
             /*nothing was selected*/

         ySI /=
            (
                 widgSTPtr->fontHeightSI;
               + widgSTPtr->fontHeightSI
               / (def_widgHeightGap_rayWidg / 2);
            ); /*space per entry*/

         --ySI; /*account for first item being selected*/

         if(ySI >= fileSTPtr->fileLenSI)
             goto noEvent_fun00z_sec0x;
             /*empty space was clicked*/


         tmpSI =
            findSelect_files_rayWidg(ySI, fileSTPtr);
            /*TODO: add this function*/

         if(tmpSI < 0)
         { /*If: file was not selected*/
            if(
                  IsKeyDown(KEY_LEFT_SHIFT)
               || IsKeyDown(KEY_RIGHT_SHIFT)
            ){ /*If: adding a range*/
               if(
                  addFileRange_files_rayWidg(
                     ySI,
                     fileSTPtr
                  ) /*TODO: add this function; should
                    `  add after last selected file
                    */
               ) goto memErr_fun00z_sec0x;
            }  /*If: adding a range*/

            else if(
                  IsKeyDown(KEY_LEFT_CTRL)
               || IsKeyDown(KEY_RIGHT_CTRL)
            ){ /*Else If: adding file to a range*/
               if(
                  addFile_files_rayWidg(
                     ySI,
                     0, /*not resting file selection*/
                     fileSTPtr
                  ) /*TODO: add this function*/
               ) goto memErr_fun00z_sec0x;
            }  /*Else If: adding file to a range*/

            else
            { /*Else: adding a single file*/
               if(
                  addFile_files_rayWidg(
                     ySI,
                     1, /*only selecting one file*/
                     fileSTPtr
                  ) /*TODO: add this function*/
               ) goto memErr_fun00z_sec0x;
            } /*Else: adding a single file*/
         } /*If: file was not selected*/

      } /*Else If: release event*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun00z Sec03 Sub03 Cat07:
      +   - if scroll wheel event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(scrollF > 0)
      { /*Else If: scroll wheel was done*/
         fileSTPtr->fileScrollSI += (signed int) scrollF;

         if(fileSTPtr->fileScrollSI < 0)
            fileSTPtr->fileScrollSI = 0;
         else if(
            fileSTPtr->fileScrollSI > fileSTPtr->fileLenSI
         ) fileSTPtr->fileScrollSI = fileSTPtr->fileLenSI-1;

         if(fileSTPtr->fileOnSI < fileSTPtr->fileScrollSI)
            fileSTPtr->fileOnSI = fileSTPtr->fileScrollSI;

         else if(
              fileSTPtr->fileOnSI
            > fileSTPtr->fileScrollSI +  maxLinesSI
         ) fileSTPtr->fileOnSI = fileSTPtr->fileScrollSI;
      } /*Else If: scroll wheel was done*/
      
   } /*Else If: file window event*/

   else
      goto noEvent_fun00z_sec0x;
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun00z Sec0x:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto noEvent_fun00z_sec0x;
      /*user did an event, but it was not a button*/

   seletectedFiles_fun00z_sec0x:;
      tmpSI = 1;
      goto ret_fun00z_sec0x;

   cancle_fun00z_sec0x:;
      tmpSI = 2;
      selectClear_files_rayWidg(fileSTPtr);
      extClear_files_rayWidg(fileSTPtr);
      goto ret_fun00z_sec0x;

   hiden_fun00z_sec0x:;
      tmpSI = 0;
      goto ret_fun00z_sec0x;

   noWidg_fun00z_sec0x:;
      tmpSI = 0;
      goto ret_fun00z_sec0x;

   noEvent_fun00z_sec0x:;
      tmpSI = 0;
      goto ret_fun00z_sec0x;

   memErr_fun00z_sec0x:;
      tmpSI = -1;
      goto ret_fun00z_sec0x;

   ret_fun00z_sec0x:;
      return (signed char) tmpSI;
} /*fileBrowserEvent_rayWidg*/

#endif
