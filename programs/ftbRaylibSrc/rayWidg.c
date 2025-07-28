/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' rayWidg SOF: Start Of File
'   - widgets or other functions to manipulate raylib and
'     make the GUI building step eaiser
'   o header:
'     - included libraries
'   o .h st01: widg_rayWidg
'     - has widget ids, coordiantes, and dimensions
'   * widg_rayWidg struct managment functions
'     o fun01: idBlank_widg_rayWidg
'       - blanks a widget id in a widg_rayWidg struct
'     o fun02: blank_widg_rayWidg
'       - blanks a widg_rayWidg struct
'     o fun03: init_widg_rayWidg
'       - initializes a widg_rayWidg struct
'     o fun04: setup_widg_rayWidg
'       - adds the default font to a rayWidg struct and
'         sets it up for use (only call once)
'     o fun05: freeStack_widg_rayWidg
'       - frees arrays in a widg_rayWidg struct
'     o fun06: freeHeap_widg_rayWidg
'       - frees a widg_rayWidg struct
'     o fun07: realloc_widg_rayWidg
'       - reallocates memory for a widg_rayWidg struct
'       - note: this will not downsize or report downsize
'         attempts
'   * change font or font variables in widg_rayWidg struct
'     o fun08: fontMeasure_widg_rayWidg
'       - measures the width of the character "D" for a
'         font
'     o fun09: textMeasure_widg_rayWidg
'       - measures the width of input text
'     o fun10: changeFont_widg_rayWidg
'       - adds a font from a file to a widg_rayWidg struct
'     o fun11: useEmbedFont_widg_rayWidg
'       - uses an embed font
'       - make this using ExprotFontAsCode from raylib
'     o fun12: swapFont_widg_rayWidg
'       - swaps the font out with an input font pointer
'       - this is a swap of pointers only, so only free
'         the input font when you are finished
'     o fun13: setFontSize_widg_rayWidg
'       - changes font size
'     o fun14: setFontSpacing_widg_rayWidg
'       - changes a fonts spacing
'   * widg_rayWidg struct querying functions
'     o fun15: xCoordGet_widg_rayWidg
'       - get the x-coordinate of a widget
'     o fun16: yCoordSet_widg_rayWidg
'       - get the y-coordinate of a widget
'     o fun17: widthGet_widg_rayWidg
'       - get the width of a widget
'     o fun18: heightGet_widg_rayWidg
'       - get the height of a widget
'     o fun19: rowGet_widg_rayWidg
'       - get the row a widget is assigned to
'     o fun20: colGet_widg_rayWidg
'       - get the column a widget is assigned to
'     o fun21: stateGet_widg_rayWidg
'       - get the state of a widget
'     o fun22: pressStateGet_widg_rayWidg
'       - detect if widget is in pressed state
'     o fun23: checkedStateGet_widg_rayWidg
'       - detect if widget is in checked state
'     o fun24: activeStateGet_widg_rayWidg
'       - detect if widget is in active state
'     o fun25: focusStateGet_widg_rayWidg
'       - detect if widget is in focus state
'     o fun26: inacitveStateGet_widg_rayWidg
'       - detect if widget is in inacitve state
'     o fun27: hidenStateGet_widg_rayWidg
'       - detect if widget is in hiden state
'     o fun28: childStateGet_widg_rayWidg
'       - detect if widget is a child widget
'     o fun29: hogStateGet_widg_rayWidg
'       - detect if widget is a hog (high priority) widget
'     o fun30: numWidgetGet_widg_rayWidg
'       - get number of widgets made
'     o fun31: focusGet_widg_rayWidg
'       - get the id (index) of the focused widget
'   * widg_rayWidg struct modify coordiantes and size
'     o fun32: xCoordSet_widg_rayWidg
'       - sets a widgets x coordiante
'     o fun33: yCoordSet_widg_rayWidg
'       - sets a widgets y coordiante
'     o fun34: widthSet_widg_rayWidg
'       - sets a widgets width
'     o fun35: heightSet_widg_rayWidg
'       - sets a widgets height
'     o fun36: rowSet_widg_rayWidg
'       - sets a widgets row number
'     o fun37: colSet_widg_rayWidg
'       - sets a widgets column number
'   * widg_rayWidg struct modify state of widgets
'     o fun38: stateSet_widg_rayWidg
'       - sets a widgets state to input state
'     o fun39: pressAdd_widg_rayWidg
'       - adds the pressed state to a widget
'     o fun40: checkedAdd_widg_rayWidg
'       - adds the checkeded state to a widget
'     o fun41: activeAdd_widg_rayWidg
'       - adds the activeed state to a widget
'     o fun42: focusAdd_widg_rayWidg
'       - adds the activeed state to a widget
'     o fun43: inactiveAdd_widg_rayWidg
'       - adds the inactive state to a widget
'     o fun44: hidenAdd_widg_rayWidg
'       - adds the hiden state to a widget
'     o fun45: childAdd_widg_rayWidg
'       - adds the child state to a widget
'       - only use this funcition if you know what your are
'         are doing and understand how the child/parent
'         system in rayWidg works
'     o fun46: hogAdd_widg_rayWidg
'       - adds the hog state to a widget
'   * widg_rayWidg struct clear state of widgets
'     o fun47: stateClear_widg_rayWidg
'       - remove all states in a widget (set to 0)
'     o fun48: pressClear_widg_rayWidg
'       - remove wigets pressed state
'     o fun49: checkedClear_widg_rayWidg
'       - remove wigets checked state
'     o fun50: activeClear_widg_rayWidg
'       - remove wigets active state
'     o fun51: focusClear_widg_rayWidg
'       - remove wigets focus state
'     o fun52: inactiveClear_widg_rayWidg
'       - remove wigets & its child widgets inactive state
'     o fun53: hidenClear_widg_rayWidg
'       - remove wigets and its child widgets hiden state
'     o fun54: childClear_widg_rayWidg
'       - clears the child state in a widget
'       - only use this function if you know what your are
'         doing and understand how the child/parent system
'         works
'     o fun55: hogClear_widg_rayWidg
'       - clears the hog state in a widget
'   * get OS state
'     o fun56: getDarkModeState_rayWidg
'       - detects if Mac/Windows computer is in dark mode
'     o fun57: checkGuiColorMode_rayWidg
'       - checks is user is using dark or light mode, and
'         sets the color scheme to the correct mode
'   * widget modification or focs/state changes
'     o fun58: addWidget_widg_rayWidg
'       - add a new widget to a widg_rayWidg struct
'     o fun59: tile_widg_rayWidg
'       - find tiling x,y coordiantes for each widget
'         using widths, heights, rows, and columns
'       - note this can only support one level of children
'         widgets
'     o fun60: focusCheck_widg_rayWidg
'       - detect if input key was tab and update focus
'     o fun61: enterCheck_widg_rayWidg
'       - check if key event was an enter key
'     o fun62: getMouseWidg_widg_rayWidg
'       - get widget mouse was on
'     o fun63: blinkGet_rayWidg
'      - returns current state of cursor blink for widgets
'   * string functions
'     o fun64: intStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an integer
'     o fun65: floatStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an float
'     o fun66: fileStrCheck_rayWidg
'       - corrects non-file characters in c-string
'       - spaces/tabes converted to '_', other than that,
'         only A-Z, a-z, 0-9, '-', '_', & '.' are allowed
'     o fun67: addCharToEntry_rayWidg
'       - adds a character to an entry box text
'   * widget drawing
'     o fun68: drawRect_rayWidg
'       - draws a rectangle for rayWidg
'     o fun69: butDraw_rayWidg
'       - draw a button
'     o fun70: entryDraw_rayWidg
'       - draw an entry box and its text
'     o fun71: labDraw_rayWidg
'       - draw a label
'   * complex widgets
'     o fun72: mkMesgBox_rayWidg
'       - makes an emtpy message box widget
'     o fun73: mesgBoxDraw_rayWidg
'       - draws a message box
'     o fun74: mesgBoxEvent_rayWidg
'      - checks the event that was fired in a message box
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
| Fun01: idBlank_widg_rayWidg
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
| Fun02: blank_widg_rayWidg
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

   widgSTPtr->butColSI = def_lightGrey_rayWidg;
   widgSTPtr->butBorderColSI = def_darkGrey_rayWidg;
   widgSTPtr->butPressColSI = def_darkGrey_rayWidg;
   widgSTPtr->butPressBorderColSI = def_lightGrey_rayWidg;

   widgSTPtr->entryColSI = def_darkGrey_rayWidg;
   widgSTPtr->entryBorderColSI = def_lightGrey_rayWidg;
   widgSTPtr->entryActiveColSI = def_lightGrey_rayWidg;
   widgSTPtr->entryActiveBorderColSI =
      def_darkGrey_rayWidg;
} /*blank_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun03: init_widg_rayWidg
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
| Fun04: setup_widg_rayWidg
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
      goto memErr_fun04;
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

   memErr_fun04:;
      return 1;
} /*setup_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun05: freeStack_widg_rayWidg
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
| Fun06: freeHeap_widg_rayWidg
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
| Fun07: realloc_widg_rayWidg
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
   ' Fun07 TOC:
   '   o fun07 sec01:
   '     - variable declarations and initial check
   '   o fun07 sec02:
   '     - deal with no memory (get initial memory)
   '   o fun07 sec03:
   '     - have memory, need to reallocate
   '   o fun07 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
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
         goto noResize_fun07_sec04;
   } /*If: user wants auto resize*/

   if(sizeSI <= widgSTPtr->sizeSI)
      goto noResize_fun07_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - deal with no memory (get initial memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->sizeSI <= 0)
   { /*If: no memory to begin with*/
      widgSTPtr->xArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->xArySI)
         goto memErr_fun07_sec04;
      widgSTPtr->yArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->yArySI)
         goto memErr_fun07_sec04;
      widgSTPtr->widthArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->widthArySI)
         goto memErr_fun07_sec04;
      widgSTPtr->heightArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->heightArySI)
         goto memErr_fun07_sec04;

      widgSTPtr->stateAryUS =
         malloc(sizeSI * sizeof(unsigned short));
      if(! widgSTPtr->stateAryUS)
         goto memErr_fun07_sec04;

      widgSTPtr->rowArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->rowArySI)
         goto memErr_fun07_sec04;
      widgSTPtr->colArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->colArySI)
         goto memErr_fun07_sec04;

      goto done_fun07_sec04;
   } /*If: no memory to begin with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - have memory, need to reallocate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siSwapPtr =
      realloc(
         widgSTPtr->xArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->xArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->yArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->yArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->widthArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->widthArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->heightArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->heightArySI = siSwapPtr;

   ucSwapPtr =
      (unsigned char *)
      realloc(
         widgSTPtr->stateAryUS,
         sizeSI * sizeof(unsigned short)
      );
   if(! ucSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->stateAryUS = (unsigned short *) ucSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->rowArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->rowArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->colArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun07_sec04;
   widgSTPtr->colArySI = siSwapPtr;

   goto done_fun07_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun07_sec04:;
      widgSTPtr->sizeSI = sizeSI;
      return 0;

   noResize_fun07_sec04:;
      return 0;

   memErr_fun07_sec04:;
      return 1;
} /*realloc_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun08: fontMeasure_widg_rayWidg
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
| Fun09: textMeasure_widg_rayWidg
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
| Fun10: changeFont_widg_rayWidg
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
         goto memErr_fun10;
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

   memErr_fun10:;
      return 1;
} /*changeFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun11: useEmbedFont_widg_rayWidg
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
         goto memErr_fun11;
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

   memErr_fun11:;
      return 1;
} /*useEmbedFont_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun12: swapFont_widg_rayWidg
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
| Fun13: setFontSize_widg_rayWidg
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
| Fun14: setFontSpacing_widg_rayWidg
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
| Fun15: xCoordGet_widg_rayWidg
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
| Fun16: yCoordGet_widg_rayWidg
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
| Fun17: widthGet_widg_rayWidg
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
| Fun18: heightGet_widg_rayWidg
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
| Fun19: rowGet_widg_rayWidg
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
| Fun20: colGet_widg_rayWidg
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
| Fun21: stateGet_widg_rayWidg
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
| Fun22: pressStateGet_widg_rayWidg
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
pressStateGet_widg_rayWidg(
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
} /*pressStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun23: checkedStateGet_widg_rayWidg
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
checkedStateGet_widg_rayWidg(
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
} /*checkedStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun24: activeStateGet_widg_rayWidg
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
activeStateGet_widg_rayWidg(
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
} /*activeStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun25: focusStateGet_widg_rayWidg
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
focusStateGet_widg_rayWidg(
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
} /*focusStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun26: inactiveStateGet_widg_rayWidg
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
inactiveStateGet_widg_rayWidg(
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
} /*inactiveStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun27: hidenStateGet_widg_rayWidg
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
hidenStateGet_widg_rayWidg(
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
} /*hidenStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun28: childStateGet_widg_rayWidg
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
childStateGet_widg_rayWidg(
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
} /*childStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun29: hogStateGet_widg_rayWidg
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
hogStateGet_widg_rayWidg(
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
} /*hogStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun30: numWidgetGet_widg_rayWidg
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
| Fun31: focusGet_widg_rayWidg
|   - get the id (index) of the focused widget
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to get focus from
| Output:
|   - Returns:
|     o wigdet in focus (< 0 means nothing in focus)
\-------------------------------------------------------*/
signed int
focusGet_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   return widgSTPtr->focusSI;
} /*focusGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun32: xCoordSet_widg_rayWidg
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
| Fun33: yCoordSet_widg_rayWidg
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
| Fun34: widthSet_widg_rayWidg
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
| Fun35: heightSet_widg_rayWidg
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
| Fun36: rowSet_widg_rayWidg
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
| Fun37: colSet_widg_rayWidg
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
| Fun38: stateSet_widg_rayWidg
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
| Fun39: pressAdd_widg_rayWidg
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
| Fun40: checkedAdd_widg_rayWidg
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
| Fun41: activeAdd_widg_rayWidg
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
| Fun42: focusAdd_widg_rayWidg
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
| Fun43: inactiveAdd_widg_rayWidg
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
| Fun44: hidenAdd_widg_rayWidg
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
| Fun45: childAdd_widg_rayWidg
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
| Fun46: hogAdd_widg_rayWidg
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
| Fun47: stateClear_widg_rayWidg
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
| Fun48: pressClear_widg_rayWidg
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
| Fun49: checkedClear_widg_rayWidg
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
| Fun50: activeClear_widg_rayWidg
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
| Fun51: focusClear_widg_rayWidg
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
| Fun52: inactiveClear_widg_rayWidg
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
| Fun53: hidenClear_widg_rayWidg
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
| Fun54: childClear_widg_rayWidg
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
| Fun55: hogClear_widg_rayWidg
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
| Fun56: getDarkModeState_rayWidg
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
            goto retUnkown_fun01;
         else if (
               [style caseInsensitiveCompare:@"Dark"]
            == NSOrderedSame
         ) goto retDark_fun01;
         else if (
         else
            goto retLight_fun01;
       } /*autoreleasepool block from objective C*/

       goto retUnkown_fun01;
          /*could not get dark/light mode*/

       retUnkown_fun01:;
          return def_unkownMode_rayWidg;
       retLight_fun01:;
          return def_lightMode_rayWidg;
       retDark_fun01:;
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
                 goto retDark_fun01;
              else
                 goto retLight_fun01;
          }  /*If: could get dark mode status*/

          RegCloseKey(hKey);
      } /*If: can open theme registory*/

      goto retUnkown_fun01;
         /*could not get dark/light mode*/

      retUnkown_fun01:;
         return def_unkownMode_rayWidg;
      retLight_fun01:;
         return def_lightMode_rayWidg;
      retDark_fun01:;
         return def_darkMode_rayWidg;
      #endif
   #else
      return def_unkownMode_rayWidg;
   #endif /*windows check*/
   #endif /*mac check*/
} /*getDarkModeState_rayWidg*/

/*-------------------------------------------------------\
| Fun57: checkGuiColorMode_rayWidg
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

      widgSTPtr->butColSI = def_darkGrey_rayWidg;
      widgSTPtr->butBorderColSI = def_lightGrey_rayWidg;
      widgSTPtr->butPressColSI = def_darkGrey_rayWidg;
      widgSTPtr->butPressBorderColSI =
         def_lightGrey_rayWidg;

      widgSTPtr->entryColSI = def_darkGrey_rayWidg;
      widgSTPtr->entryBorderColSI= def_lightGrey_rayWidg;
      widgSTPtr->entryActiveColSI= def_black_rayWidg;
      widgSTPtr->entryActiveBorderColSI =
         def_lightGrey_rayWidg;
   } /*If: dark mode is set*/

   else
   { /*Else: light mode is set*/
      widgSTPtr->guiColSI = def_white_rayWidg;
      widgSTPtr->focusColSI = def_black_rayWidg;

      widgSTPtr->textColSI = def_black_rayWidg;
      widgSTPtr->textAltColSI = def_white_rayWidg;

      widgSTPtr->butColSI = def_lightGrey_rayWidg;
      widgSTPtr->butBorderColSI = def_darkGrey_rayWidg;
      widgSTPtr->butPressColSI = def_darkGrey_rayWidg;
      widgSTPtr->butPressBorderColSI =
         def_lightGrey_rayWidg;

      widgSTPtr->entryColSI = def_lightGrey_rayWidg;
      widgSTPtr->entryBorderColSI = def_darkGrey_rayWidg;
      widgSTPtr->entryBorderColSI = def_lightGrey_rayWidg;
      widgSTPtr->entryActiveColSI = def_darkGrey_rayWidg;
   } /*Else: light mode is set*/
} /*checkGuiColorMode_rayWidg*/

/*-------------------------------------------------------\
| Fun58: addWidget_widg_rayWidg
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
      goto memErr_fun57; /*memory error while resizing*/
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

   memErr_fun57:;
      return 1;
} /*addWidget_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun59: tile_widg_rayWidg
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
   ' Fun59 TOC:
   '   - find tiling x,y coordiantes for each widget using
   '     widths, heights, rows, and columns
   '   o fun59 sec01:
   '     - variable declarations
   '   o fun59 sec02:
   '     - get maximum rows and column (and get memory)
   '   o fun59 sec03:
   '     - find parent widget row and column coordinates
   '   o fun59 sec04:
   '     - add x,y to tiled parents and update non-tiled
   '       child widgets
   '   o fun59 sec05:
   '     - find tile coordinates for tiled child widgets
   '   o fu10 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec01:
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
   ^ Fun59 Sec02:
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
      goto noTile_fun59_sec06;

   ++rowSizeSI;
   ++colSizeSI;

   rowHeapArySI = calloc(rowSizeSI, sizeof(signed int));
   if(! rowHeapArySI)
      goto memErr_fun59_sec06;
   colHeapArySI = calloc(colSizeSI, sizeof(signed int));
   if(! colHeapArySI)
      goto memErr_fun59_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec03:
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
   ^ fun59 sec04:
   ^   - add x,y to tiled parents and update non-tiled
   ^     child widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: add in x,y coordaintes*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto nextWidg_fun59_sec04;
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto nextWidg_fun59_sec04;
      else if(
         widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ) goto nextWidg_fun59_sec04;

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
             goto nextChild_fun59_sec04;
          else if(widgSTPtr->colArySI[idSI] >= 0)
             goto nextChild_fun59_sec04;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] += yChangeSI;
         widgSTPtr->xArySI[idSI] += xChangeSI;

         nextChild_fun59_sec04:;
            ++idSI;
      }  /*Loop: update x,y coord child coordinates*/

      continue; /*already on next parent wiget*/

      nextWidg_fun59_sec04:;
         ++idSI;
   } /*Loop: add in x,y coordaintes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec05:
   ^   - find tile coordinates for tiled child widgets
   ^   o fun59 sec05 sub01:
   ^     - find and setup for found child group
   ^   o fun59 sec05 sub02:
   ^     - find column and row sizes in child group
   ^   o fun59 sec05 sub03:
   ^     - add x,y coordinates in child group
   ^   o fun59 sec05 sub04:
   ^     - move to next widget
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun59 Sec05 Sub01:
   *   - find and setup for found child group
   \*****************************************************/

   rowLenSI = 0;
   colLenSI = 0;
   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: find tile dimensions*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto childNextWidg_fun59_sec05_sub05;
         /*non-tiled*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto childNextWidg_fun59_sec05_sub05;
         /*non-tiled*/
      else if(
        !(widgSTPtr->stateAryUS[idSI] & def_child_rayWidg)
      ) goto childNextWidg_fun59_sec05_sub05;
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
      * Fun59 Sec05 Sub02:
      *   - find column and row sizes in child group
      \**************************************************/

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: find child widget row and column sizes*/

         if(widgSTPtr->rowArySI[idSI] < 0)
            goto nextChild_fun59_sec05_sub02;/*non-tiled*/
         else if(widgSTPtr->colArySI[idSI] < 0)
            goto nextChild_fun59_sec05_sub02;/*non-tiled*/

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

         nextChild_fun59_sec05_sub02:;
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
      * Fun59 Sec05 Sub03:
      *   - add x,y coordinates in child group
      \**************************************************/

      idSI = tmpSI;

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUS[idSI] & def_child_rayWidg
      ){ /*Loop: set tiled child wiget x,y coordinates*/

          /*check if children were non-tile widgets*/
          if(widgSTPtr->rowArySI[idSI] < 0)
             goto addCoordNextChild_fun59_sec05_sub03;
          else if(widgSTPtr->colArySI[idSI] < 0)
             goto addCoordNextChild_fun59_sec05_sub03;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] = rowHeapArySI[siRow];
         widgSTPtr->xArySI[idSI] = colHeapArySI[siCol];

         addCoordNextChild_fun59_sec05_sub03:;
            ++idSI;
      }  /*Loop: set tiled child wiget x,y coordinates*/

      /**************************************************\
      * Fun59 Sec05 Sub04:
      *   - move to next widget
      \**************************************************/

      continue; /*on next parent widget*/

      childNextWidg_fun59_sec05_sub05:;
         ++idSI;
   } /*Loop: find tile dimensions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fu10 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;
   goto ret_fun59_sec06;

   noTile_fun59_sec06:;
      idSI = 0;
      goto ret_fun59_sec06;

   memErr_fun59_sec06:;
      idSI = 1;
      goto ret_fun59_sec06;

   ret_fun59_sec06:;
      if(rowHeapArySI)
         free(rowHeapArySI);
      rowHeapArySI = 0;

      if(colHeapArySI)
         free(colHeapArySI);
      colHeapArySI = 0;

      return idSI;
} /*tile_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun60: focusCheck_widg_rayWidg
|   - detect if input key was tab and update focus
| Input:
|   - keySI:
|     o raylib key returned from GetKey() 
|   - charBl:
|     o 1: keySC is char from GetChar()
|       * this can only detect tab
|     o 0: keySC is key from GetKey()
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
   signed int keySI,   /*key from GetKey()*/
   signed char charBl, /*1: key was from GetChar()*/
   struct widg_rayWidg *widgSTPtr /*has focus to change*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun60 TOC:
   '   - detect if input key was tab and update focus
   '   o fun60 sec01:
   '     - find if focus and set focus
   '   o fun60 sec02:
   '     - check if next widget can be focused, if not
   '       find the next widget that can be focused
   '   o fun60 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun60 Sec01:
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
        goto nonFocusKey_fun60_sec04;
  } /*If: character input*/

  else
  { /*Else: key event input*/
     if(keySI == KEY_TAB)
        charBl = shiftBl;
     else
        goto nonFocusKey_fun60_sec04;
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
  ^ Fun60 Sec02:
  ^   - check if next widget can be focused, if not find
  ^     the next widget that can be focused
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  /*check if next widget can be focused*/
  keySI = widgSTPtr->focusSI;

  if(
       hogStateGet_widg_rayWidg(
          widgSTPtr->focusSI,
          widgSTPtr
       ) == 1
  ){ /*If: on a hog widget*/
     hogBl = 1;

     if(widgSTPtr->stateAryUS[keySI] & def_child_rayWidg)
     { /*If: is a parent widget*/
        if(charBl)
           goto resetState_fun60_sec02;
           /*going backwards to other widgets*/
        else if(
            widgSTPtr->stateAryUS[keySI + 1]
          & def_child_rayWidg
        ) goto resetState_fun60_sec02; /*no children*/
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
              goto noFocus_fun60_sec04;
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
              goto noFocus_fun60_sec04;
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
        goto resetState_fun60_sec02;
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

  resetState_fun60_sec02:;
     widgSTPtr->focusSI = keySI;
     widgSTPtr->stateAryUS[keySI] |= def_focus_rayWidg;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun60 Sec03:
  ^   - return
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  return keySI;

  noFocus_fun60_sec04:;
     widgSTPtr->focusSI = -1;
     return -1;
  nonFocusKey_fun60_sec04:;
      return -2;
} /*focusCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun61: enterCheck_widg_rayWidg
|   - check if key event was an enter key
| Input:
|   - keySI:
|     o raylib key returned from GetKey() 
|   - charBl:
|     o 1: keySC is char from GetChar()
|     o 0: keySC is key from GetKey()
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
   signed int keySI,   /*key from GetKey()*/
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
         goto noPress_fun61;
   } /*If: character input*/

   else
   { /*Else: key event input*/
      if(keySI == KEY_ENTER)
         charBl = 1;
      else
         goto noPress_fun61;
   } /*Else: key event input*/


   /*check if widget is pressable*/
   keySI = widgSTPtr->focusSI;
   if(keySI < 0)
      goto noFocus_fun61;
   else if(keySI >= widgSTPtr->lenSI)
      goto noFocus_fun61;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_hiden_rayWidg
   ) goto noFocus_fun61;
   else if(
      widgSTPtr->stateAryUS[keySI] & def_inactive_rayWidg
   ) goto noFocus_fun61;


   pressAdd_widg_rayWidg(keySI, widgSTPtr);

   *parSIPtr =
      childStateGet_widg_rayWidg(keySI, widgSTPtr);

   if(*parSIPtr == -3)
      goto widgetErr_fun61;

   return keySI;

   noFocus_fun61:;
      return -1;
   noPress_fun61:;
      return -2;
   widgetErr_fun61:;
      return -3;
} /*enterCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun62: getMouseWidg_widg_rayWidg
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
   ' Fun62 TOC:
   '   - get widget mouse was on
   '   o fun62 sec01:
   '     - find first parent widget the mouse coud be on
   '   o fun62 sec02:
   '     - check if there is a child widget the mouse
   '       could be on
   '   o fun62 sec03:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun62 Sec01:
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
      goto noWidg_fun62_sec03_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun62 Sec02:
   ^   - check if there is a child widget the mouse could
   ^     be on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *parSIPtr =
      childStateGet_widg_rayWidg(retSI, widgSTPtr);

   if(*parSIPtr < -1)
      goto noWidg_fun62_sec03_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun62 Sec03:
   ^   - return results
   ^   o fun62 sec03 sub01:
   ^     - clear old focus state
   ^   o fun62 sec03 sub02:
   ^     - handle hog widget state checks
   ^   o fun62 sec03 sub03:
   ^     - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun62 Sec03 Sub01:
   *   - clear old focus state
   \*****************************************************/

   goto retWidg_fun62_sec03_sub01;

   retWidg_fun62_sec03_sub01:;
      pressClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear a press event (might be one)*/
      activeClear_widg_rayWidg(
         widgSTPtr->focusSI,
         widgSTPtr
      ); /*clear the old active event*/

      /**************************************************\
      * Fun62 Sec03 Sub02:
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
         ) goto setRet_fun62_sec03_sub03;

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
               goto setRet_fun62_sec03_sub03;
            else
               goto hogNoMove_fun62_sec03_sub03;
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
               goto setRet_fun62_sec03_sub03;
            else
               goto hogNoMove_fun62_sec03_sub03;
         } /*Else: moving backwards*/
      }  /*Else If: moving from a hog state widget*/

      /**************************************************\
      * Fun62 Sec03 Sub03:
      *   - return results
      \**************************************************/

      setRet_fun62_sec03_sub03:;
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

      hogNoMove_fun62_sec03_sub03:;
         retSI = widgSTPtr->focusSI;
         focusAdd_widg_rayWidg(retSI, widgSTPtr);
            /*set clicked widget to focused widget*/
         activeAdd_widg_rayWidg(retSI, widgSTPtr);
            /*make into an active widget*/
         *parSIPtr = -1;
         return -1;

   noWidg_fun62_sec03_sub03:;
      *parSIPtr = -1;
      return -1;
} /*getMouseWidg_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun63: blinkGet_rayWidg
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
| Fun64: intStrCheck_rayWidg
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
      goto ret_fun65;

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

   ret_fun65:;
      return dupStr - textStr;
} /*intStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun65: floatStrCheck_rayWidg
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
      goto ret_fun65;

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

   ret_fun65:;
      return dupStr - textStr;
} /*floatStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun66: fileStrCheck_rayWidg
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
| Fun67: addCharToEntry_rayWidg
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
   ' Fun67 TOC:
   '   - adds a character to an entry box text
   '   o fun67 sec01:
   '     - variable declaration and movement keys
   '   o fun67 sec02:
   '     - delete key
   '   o fun67 sec03:
   '     - backspace key
   '   o fun67 sec04:
   '     - keys that need a conversion
   '   o fun67 sec05:
   '     - normal keys
   '   o fun67 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec01:
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
      goto cursorMove_fun67_sec06;
   } /*If: move left*/

   else if(keySI == KEY_RIGHT)
   { /*Else If: move left*/
      if(posArySI[1] < *lenSIPtr)
         ++posArySI[1];
      goto cursorMove_fun67_sec06;
   } /*Else If: move left*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec02:
   ^   - delete key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_DELETE)
   { /*Else If: deleting the next character*/
      if(*lenSIPtr <= 0)
         goto cursorMove_fun67_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] >= *lenSIPtr)
         goto cursorMove_fun67_sec06;
         /*at end of string; nothing to delete*/

      else if(posArySI[1] == *lenSIPtr - 1)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1]] = 0;
         --*lenSIPtr;
         goto delete_fun67_sec06;
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
         goto delete_fun67_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: deleting the next character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec03:
   ^   - backspace key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_BACKSPACE)
   { /*Else If: backspace to delete last character*/
      if(posArySI[1] <= 0)
         goto cursorMove_fun67_sec06;
         /*at start of string; nothing to delete*/

      else if(*lenSIPtr <= 0)
         goto cursorMove_fun67_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] == *lenSIPtr)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1] - 1] = 0;
         --posArySI[1];
         --*lenSIPtr;
         goto delete_fun67_sec06;
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
         goto delete_fun67_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: backspace to delete last character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec04:
   ^   - keys that need a conversion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_END)
   { /*Else If: move cursor to end of text*/
      posArySI[1] = *lenSIPtr;
      goto cursorMove_fun67_sec06;
   } /*Else If: move cursor to end of text*/

   else if(keySI == KEY_HOME)
   { /*Else If: move cursor to start of text*/
      posArySI[0] = 0;
      posArySI[1] = 0;
      goto cursorMove_fun67_sec06;
   } /*Else If: move cursor to start of text*/

   else if(keySI == KEY_KP_0)
   { /*Else If: key pad 0*/
      keySI = '0';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 0*/

   else if(keySI == KEY_KP_1)
   { /*Else If: key pad 1*/
      keySI = '1';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 1*/

   else if(keySI == KEY_KP_2)
   { /*Else If: key pad 2*/
      keySI = '2';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 2*/

   else if(keySI == KEY_KP_3)
   { /*Else If: key pad 3*/
      keySI = '3';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 3*/

   else if(keySI == KEY_KP_4)
   { /*Else If: key pad 4*/
      keySI = '4';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 4*/

   else if(keySI == KEY_KP_5)
   { /*Else If: key pad 5*/
      keySI = '5';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 5*/

   else if(keySI == KEY_KP_6)
   { /*Else If: key pad 6*/
      keySI = '6';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 6*/

   else if(keySI == KEY_KP_7)
   { /*Else If: key pad 7*/
      keySI = '7';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 7*/

   else if(keySI == KEY_KP_8)
   { /*Else If: key pad 8*/
      keySI = '8';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 8*/

   else if(keySI == KEY_KP_9)
   { /*Else If: key pad 9*/
      keySI = '9';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad 9*/

   else if(keySI == KEY_KP_DECIMAL)
   { /*Else If: key pad .*/
      keySI = '.';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad .*/

   else if(keySI == KEY_KP_DIVIDE)
   { /*Else If: key pad /*/
      keySI = '/';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad /*/

   else if(keySI == KEY_KP_MULTIPLY)
   { /*Else If: key pad **/
      keySI = '*';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad **/

   else if(keySI == KEY_KP_SUBTRACT)
   { /*Else If: key pad -*/
      keySI = '-';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad -*/

   else if(keySI == KEY_KP_ADD)
   { /*Else If: key pad +*/
      keySI = '+';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad +*/

   else if(keySI == KEY_KP_EQUAL)
   { /*Else If: key pad =*/
      keySI = '=';
      goto addNormalKey_fun67_sec05;
   } /*Else If: key pad =*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec05:
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
         
   
      addNormalKey_fun67_sec05:;

      if(posArySI[1] >= *lenSIPtr)
      { /*If: at end of string*/
         textStr[posArySI[1]] = keySI;
         ++posArySI[1];
         ++*lenSIPtr;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun67_sec06;
      } /*If: at end of string*/

      else
      { /*Else: need to insert the new character*/
         for(posSI=*lenSIPtr;posSI > posArySI[1];--posSI)
            textStr[posSI] = textStr[posSI - 1];

         ++posArySI[1];
         ++*lenSIPtr;
         textStr[posSI] = keySI;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun67_sec06;
      } /*Else: need to insert the new character*/
   } /*Else If: normal key pressed*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun67 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cursorMove_fun67_sec06;
      /*no idea what the key event is*/

   delete_fun67_sec06:;
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      posSI = -1;
      goto ret_fun67_sec06;

   addChar_fun67_sec06:;
      posSI = 1;
      goto adjustPos_fun67_sec06;

   cursorMove_fun67_sec06:;
      posSI = 0;
      goto adjustPos_fun67_sec06;

   adjustPos_fun67_sec06:;
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

      goto ret_fun67_sec06;

   ret_fun67_sec06:;
      textStr[*lenSIPtr] = 0;
      return (signed char) posSI;
} /*addCharToEntry_rayWidg*/

/*-------------------------------------------------------\
| Fun68: drawRect_rayWidg
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun68 TOC:
   '   - draws a rectangle for rayWidg
   '   o fun68 sec01:
   '     - variable declarations
   '   o fun68 sec02:
   '     - draw the rectangle
   '   o fun68 sec03:
   '     - draw border around rectangle (if requested)
   '   o fun68 sec04:
   '     - draw focus border (if requested)
   '   o fun68 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun68 Sec01:
   ^   - variable declarations and initial checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Rectangle coordsST;
   struct Color colorST;

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun68_sec05;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun68_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun68 Sec02:
   ^   - draw the rectangle
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*need to convert hex color code to raylib color*/
   colorST.r = colorHexUI >> 24 & 0xff;   /*red*/
   colorST.g = (colorHexUI >> 16) & 0xff; /*green*/
   colorST.b = (colorHexUI >> 8) & 0xff;  /*blue*/
   colorST.a = colorHexUI & 0xff;         /*alpha*/

   coordsST.x = widgSTPtr->xArySI[idSI];
   coordsST.y = widgSTPtr->yArySI[idSI];
   coordsST.width = widgSTPtr->widthArySI[idSI];
   coordsST.height = widgSTPtr->heightArySI[idSI];

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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun68 Sec03:
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
   ^ Fun68 Sec04:
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun68 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun68_sec05:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun68_sec05:;
      return 0;
} /*drawRect_rayWidg*/

/*-------------------------------------------------------\
| Fun69: butDraw_rayWidg
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
   ' Fun69 TOC:
   '   - draw a button
   '   o fun69 sec01:
   '     - variable declarations
   '   o fun69 sec02:
   '     - get color of button (state in)
   '   o fun69 sec03:
   '     - get width of button
   '   o fun69 sec04:
   '     - draw button
   '   o fun69 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun69 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = widgSTPtr->focusColSI;
   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxButText_fun69 32
      /*max number of characters in button*/
   signed char writeStr[def_maxButText_fun69 + 8];

   /*these are for shorting strings*/
   signed char histUC = 0;
   signed int histIndexSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun69 Sec02:
   ^   - get color of button (state in)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun69_sec05;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun69_sec05;

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: button is in a disabled state*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->butBorderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a disabled state*/

   else if(widgSTPtr->stateAryUS[idSI] &def_press_rayWidg)
   { /*Else If: button is in a press state*/
      colorUI = widgSTPtr->butPressColSI;
      borderColorUI = widgSTPtr->butPressBorderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: button is in a press state*/

   else
   { /*Else If: button is in a normal state*/
      colorUI = widgSTPtr->butColSI;
      borderColorUI = widgSTPtr->butBorderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a normal state*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun69 Sec03:
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
        widgSTPtr->widthArySI[idSI] > def_maxButText_fun69
      )widgSTPtr->widthArySI[idSI] = def_maxButText_fun69;

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
      goto done_fun69_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun69 Sec04:
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
   ^ Fun69 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun69_sec05:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun69_sec05:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun69_sec05:;
      return 0;
} /*butDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun70: entryDraw_rayWidg
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
   ' Fun70 TOC:
   '   - draw an entry box, prevents typing if maxium is
   '     exceded or wrong entry type
   '   o fun70 sec01:
   '     - variable declarations
   '   o fun70 sec02:
   '     - check state of entry box
   '   o fun70 sec03:
   '     - copy string & check/set scroll & cursor values
   '   o fun70 sec04:
   '     - find text size and print cursor
   '   o fun70 sec05:
   '     - draw entry box
   '   o fun70 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun70 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int colorUI = 0;
   unsigned int borderColorUI = 0;
   unsigned int focusColorUI = widgSTPtr->focusColSI;
   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxChar_fun70 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun70 + 8];
   signed char *cpStr = 0;
   signed char *dupStr = 0;
   signed char rmStr[2] = {0, 0}; /*for adjusting length*/

   signed int tmpSI = 0;
   signed int outLenSI = 0;
   signed int cursorPosSI = posArySI[1];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun70 Sec02:
   ^   - check state of entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun70_sec06;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun70_sec06;
      /*entry box is hidden*/

   else if(
      widgSTPtr->stateAryUS[idSI] & def_inactive_rayWidg
   ){ /*Else If: entry box is being used*/
      colorUI = widgSTPtr->guiColSI;
      borderColorUI = widgSTPtr->entryBorderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else If: entry box is being used*/

   /*will likely remove, or use lighter shade difference*/
   else if(
      widgSTPtr->stateAryUS[idSI] & def_active_rayWidg
   ){ /*Else If: entry box is being used*/
      colorUI = widgSTPtr->entryActiveColSI;
      borderColorUI = widgSTPtr->entryActiveBorderColSI;
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: entry box is being used*/

   else
   { /*Else: not being used*/
      colorUI = widgSTPtr->entryColSI;
      borderColorUI = widgSTPtr->entryBorderColSI;
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else: not being used*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun70 Sec03:
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


   if(outLenSI >= def_maxChar_fun70)
      outLenSI = def_maxChar_fun70 - 1;
         /*need one character for the cursor*/

   cpLen_ulCp(outStr, &textStr[posArySI[0]], outLenSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun70 Sec04:
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
      goto getDimensions_fun70_sec04;

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

   getDimensions_fun70_sec04:;

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] =
        widgSTPtr->fontHeightF
      + widgSTPtr->fontHeightF
      / (def_widgHeightGap_rayWidg / 2);
   widgSTPtr->widthArySI[idSI] = widthSI;
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun70_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun70 Sec05:
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
   ^ Fun70 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun70_sec06:;
      return outLenSI;

   noWidget_fun70_sec06:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun70_sec06:;
      return 0;
} /*entryDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun71: labDraw_rayWidg
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
   ' Fun71 TOC:
   '   - draw a label
   '   o fun71 sec01:
   '     - variable declarations
   '   o fun71 sec02:
   '     - check state of label and copy string
   '   o fun71 sec03:
   '     - find text size and pad if needed
   '   o fun71 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun71 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color textCol;
   struct Vector2 textDimVect2;

   #define def_maxChar_fun71 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun71 + 8];
   signed int outLenSI = 0;
   signed int oldLenSI = 0;
   signed int tmpSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun71 Sec02:
   ^   - check state of label and copy string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun71_sec04;

   if(widgSTPtr->stateAryUS[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun71_sec04;
      /*entry box is hidden*/

   if(minWidthSI > def_maxChar_fun71)
      minWidthSI = def_maxChar_fun71;

   textCol = GetColor(widgSTPtr->textColSI);
   outLenSI = endStr_ulCp(textStr);

   if(outLenSI >= def_maxChar_fun71)
   { /*If: user input to long of string*/
      outLenSI = def_maxChar_fun71 - 4;
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
   ^ Fun71 Sec03:
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
      goto done_fun71_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun71 Sec04:
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

   done_fun71_sec04:;
      return outLenSI;

   noWidget_fun71_sec04:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun71_sec04:;
      return 0;
} /*labDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun72: mkMesgBox_rayWidg
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
         goto memErr_fun72;
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

   memErr_fun72:;
      return -1;
} /*mkMesgBox_rayWidg*/

/*-------------------------------------------------------\
| Fun73: mesgBoxDraw_rayWidg
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
   ' Fun73 TOC:
   '   - draws a message box
   '   o fun73 sec01:
   '     - variable declarations
   '   o fun73 sec02:
   '     - initalize and copy/edit string
   '   o fun73 sec03:
   '     - find widget widths
   '   o fun73 sec04:
   '     - find widget coordinates
   '   o fun73 sec05:
   '     - draw message box
   '   o fun73 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun73 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   Vector2 textDimVect2;

   #define def_lenOut_fun73 128
   signed char outStr[def_lenOut_fun73 + 8];
   signed int textLenSI = endStr_ulCp(mesgStr);
   signed int butLenSI = 0;
   signed int maxLenSI = 0;

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun73 Sec02:
   ^   - initalize and copy/edit string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun73_sec0x;
   if( hidenStateGet_widg_rayWidg(idSI, widgSTPtr) )
      goto hidden_fun73_sec0x;
   if(textLenSI > def_lenOut_fun73)
      goto mesgErr_fun73_sec0x;

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
   ^ Fun73 Sec03:
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
   ^ Fun73 Sec04:
   ^   - find widget coordinates
   ^   o fun73 sec04 sub01:
   ^     - find x coordinates
   ^   o fun73 sec04 sub02:
   ^     - find y coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun73 Sec04 Sub01:
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
   * Fun73 Sec04 Sub02:
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
   ^ Fun73 Sec05:
   ^   - draw message box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI,
      1,       /*only draw the inner border*/
      widgSTPtr->guiColSI,
      widgSTPtr->butBorderColSI,
      widgSTPtr->focusColSI
   ); /*rectangle holding message box*/

   drawRect_rayWidg(
      widgSTPtr,
      idSI + 1,
      1,       /*only draw the inner border*/
      widgSTPtr->butColSI,
      widgSTPtr->butBorderColSI,
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
   ^ Fun73 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*this widget's button is in hog state, so it should
   `  take over the GUI
   */
   activeClear_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);
   pressClear_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);
   focusClear_widg_rayWidg(widgSTPtr->focusSI,widgSTPtr);

   widgSTPtr->focusSI = idSI + 3;
   focusAdd_widg_rayWidg(idSI + 3, widgSTPtr);

   return widgSTPtr->widthArySI[idSI];

   noWidget_fun73_sec0x:;
      return def_noWidget_rayWidg;

   hidden_fun73_sec0x:;
      return 0;

   mesgErr_fun73_sec0x:;
      return -1;
} /*mesgBoxDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun74: mesgBoxEvent_rayWidg
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
      goto noEvent_fun74;
   } /*If: clearing button press*/

   else if(childIdSI - parIdSI != 3)
      goto emptyClick_fun74;

   else if(eventSC == 1)
   { /*Else If: press event*/
      pressAdd_widg_rayWidg(childIdSI, widgSTPtr);
      goto noEvent_fun74;
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

   noEvent_fun74:;
      return 0;
   emptyClick_fun74:;
      return 0;
} /*mesgBoxEvent_rayWidg*/
