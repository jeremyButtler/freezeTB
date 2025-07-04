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
'     o fun04: freeStack_widg_rayWidg
'       - frees arrays in a widg_rayWidg struct
'     o fun05: freeHeap_widg_rayWidg
'       - frees a widg_rayWidg struct
'     o fun06: realloc_widg_rayWidg
'       - reallocates memory for a widg_rayWidg struct
'       - note: this will not downsize or report downsize
'         attempts
'   * widg_rayWidg struct querying functions
'     o fun07: xCoordGet_widg_rayWidg
'       - get the x-coordinate of a widget
'     o fun08: yCoordSet_widg_rayWidg
'       - get the y-coordinate of a widget
'     o fun09: widthGet_widg_rayWidg
'       - get the width of a widget
'     o fun10: heightGet_widg_rayWidg
'       - get the height of a widget
'     o fun11: rowGet_widg_rayWidg
'       - get the row a widget is assigned to
'     o fun12: colGet_widg_rayWidg
'       - get the column a widget is assigned to
'     o fun13: stateGet_widg_rayWidg
'       - get the state of a widget
'     o fun14: pressStateGet_widg_rayWidg
'       - detect if widget is in pressed state
'     o fun15: checkedStateGet_widg_rayWidg
'       - detect if widget is in checked state
'     o fun16: activeStateGet_widg_rayWidg
'       - detect if widget is in active state
'     o fun17: focusStateGet_widg_rayWidg
'       - detect if widget is in focus state
'     o fun18: inacitveStateGet_widg_rayWidg
'       - detect if widget is in inacitve state
'     o fun19: hidenStateGet_widg_rayWidg
'       - detect if widget is in hiden state
'     o fun20: childStateGet_widg_rayWidg
'       - detect if widget is a child widget
'     o fun21: numWidgetGet_widg_rayWidg
'       - get number of widgets made
'     o fun22: focusGet_widg_rayWidg
'       - get the id (index) of the focused widget
'   * widg_rayWidg struct modify coordiantes and size
'     o fun23: xCoordSet_widg_rayWidg
'       - sets a widgets x coordiante
'     o fun24: yCoordSet_widg_rayWidg
'       - sets a widgets y coordiante
'     o fun25: widthSet_widg_rayWidg
'       - sets a widgets width
'     o fun26: heightSet_widg_rayWidg
'       - sets a widgets height
'     o fun27: rowSet_widg_rayWidg
'       - sets a widgets row number
'     o fun28: colSet_widg_rayWidg
'       - sets a widgets column number
'   * widg_rayWidg struct modify state of widgets
'     o fun29: stateSet_widg_rayWidg
'       - sets a widgets state to input state
'     o fun30: pressAdd_widg_rayWidg
'       - adds the pressed state to a widget
'     o fun31: checkedAdd_widg_rayWidg
'       - adds the checkeded state to a widget
'     o fun32: activeAdd_widg_rayWidg
'       - adds the activeed state to a widget
'     o fun33: focusAdd_widg_rayWidg
'       - adds the activeed state to a widget
'     o fun34: inactiveAdd_widg_rayWidg
'       - adds the inactive state to a widget
'     o fun35: hidenAdd_widg_rayWidg
'       - adds the hiden state to a widget
'     o fun36: childAdd_widg_rayWidg
'       - adds the child state to a widget
'       - only use this funcition if you know what your are
'         are doing and understand how the child/parent
'         system in rayWidg works
'   * widg_rayWidg struct clear state of widgets
'     o fun37: stateClear_widg_rayWidg
'       - remove all states in a widget (set to 0)
'     o fun38: pressClear_widg_rayWidg
'       - remove wigets pressed state
'     o fun39: checkedClear_widg_rayWidg
'       - remove wigets checked state
'     o fun40: activeClear_widg_rayWidg
'       - remove wigets active state
'     o fun41: focusClear_widg_rayWidg
'       - remove wigets focus state
'     o fun42: inactiveClear_widg_rayWidg
'       - remove wigets & its child widgets inactive state
'     o fun43: hidenClear_widg_rayWidg
'       - remove wigets and its child widgets hiden state
'     o fun44: childClear_widg_rayWidg
'       - clears the child state in a widget
'       - only use this function if you know what your are
'         doing and understand how the child/parent system
'         works
'   * get OS state
'     o fun45: getDarkModeState_rayWidg
'       - detects if Mac/Windows computer is in dark mode
'     o fun46: checkGuiColorMode_rayWidg
'       - checks is user is using dark or light mode, and
'         sets the color scheme to the correct mode
'   * widget modification or focs/state changes
'     o fun47: addWidget_widg_rayWidg
'       - add a new widget to a widg_rayWidg struct
'     o fun48: tile_widg_rayWidg
'       - find tiling x,y coordiantes for each widget
'         using widths, heights, rows, and columns
'       - note this can only support one level of children
'         widgets
'     o fun49: focusCheck_widg_rayWidg
'       - detect if input key was tab and update focus
'     o fun50: enterCheck_widg_rayWidg
'       - check if key event was an enter key
'     o fun51: getMouseWidg_widg_rayWidg
'       - get widget mouse was on
'     o fun52: blinkGet_rayWidg
'      - returns current state of cursor blink for widgets
'   * string functions
'     o fun53: intStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an integer
'     o fun54: floatStrCheck_rayWidg
'       - corrects mistakes in a c-string that is supposed
'         to be an float
'     o fun55: fileStrCheck_rayWidg
'       - corrects non-file characters in c-string
'       - spaces/tabes converted to '_', other than that,
'         only A-Z, a-z, 0-9, '-', '_', & '.' are allowed
'     o fun56: addCharToEntry_rayWidg
'       - adds a character to an entry box text
'   * widget drawing
'     o fun57: butDraw_rayWidg
'       - draw a button
'     o fun58: entryDraw_rayWidg
'       - draw an entry box and its text
'     o fun59: labDraw_rayWidg
'       - draw a label
'     o fun60: mkMesgBox_rayWidg
'       - makes an emtpy message box widget
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
|       heightArySI, and stateAryUC in widgSTPtr to be 0
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

   widgSTPtr->stateAryUC[idSI] = 0;

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


   /*font variables*/
   widgSTPtr->fontSizeSI = def_fontSizeSI_rayWidg;
   widgSTPtr->charHeightSI = def_charHeightSI_rayWidg;
   widgSTPtr->charPadSI = def_height_rayWidg;


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

   widgSTPtr->stateAryUC = 0;

   widgSTPtr->sizeSI = 0;

   blank_widg_rayWidg(widgSTPtr);
} /*init_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun04: freeStack_widg_rayWidg
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
   if(widgSTPtr->stateAryUC)
      free(widgSTPtr->stateAryUC);

   init_widg_rayWidg(widgSTPtr);
} /*freeStack_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun05: freeHeap_widg_rayWidg
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
| Fun06: realloc_widg_rayWidg
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
   ' Fun06 TOC:
   '   o fun06 sec01:
   '     - variable declarations and initial check
   '   o fun06 sec02:
   '     - deal with no memory (get initial memory)
   '   o fun06 sec03:
   '     - have memory, need to reallocate
   '   o fun06 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
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
         goto noResize_fun06_sec04;
   } /*If: user wants auto resize*/

   if(sizeSI <= widgSTPtr->sizeSI)
      goto noResize_fun06_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - deal with no memory (get initial memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widgSTPtr->sizeSI <= 0)
   { /*If: no memory to begin with*/
      freeStack_widg_rayWidg(widgSTPtr);
 
      widgSTPtr->xArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->xArySI)
         goto memErr_fun06_sec04;
      widgSTPtr->yArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->yArySI)
         goto memErr_fun06_sec04;
      widgSTPtr->widthArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->widthArySI)
         goto memErr_fun06_sec04;
      widgSTPtr->heightArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->heightArySI)
         goto memErr_fun06_sec04;

      widgSTPtr->stateAryUC =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->stateAryUC)
         goto memErr_fun06_sec04;

      widgSTPtr->rowArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->rowArySI)
         goto memErr_fun06_sec04;
      widgSTPtr->colArySI =
         malloc(sizeSI * sizeof(signed int));
      if(! widgSTPtr->colArySI)
         goto memErr_fun06_sec04;

      goto done_fun06_sec04;
   } /*If: no memory to begin with*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - have memory, need to reallocate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siSwapPtr =
      realloc(
         widgSTPtr->xArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->xArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->yArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->yArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->widthArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->widthArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->heightArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->heightArySI = siSwapPtr;

   ucSwapPtr =
      realloc(
         widgSTPtr->stateAryUC,
         sizeSI * sizeof(unsigned char *)
      );
   if(! ucSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->stateAryUC = ucSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->rowArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->rowArySI = siSwapPtr;

   siSwapPtr =
      realloc(
         widgSTPtr->colArySI,
         sizeSI * sizeof(signed int)
      );
   if(! siSwapPtr)
      goto memErr_fun06_sec04;
   widgSTPtr->colArySI = siSwapPtr;

   goto done_fun06_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun06_sec04:;
      widgSTPtr->sizeSI = sizeSI;
      return 0;

   noResize_fun06_sec04:;
      return 0;

   memErr_fun06_sec04:;
      return 1;
} /*realloc_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun07: xCoordGet_widg_rayWidg
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
   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->xArySI[idSI];
} /*xCoordGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun08: yCoordGet_widg_rayWidg
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
   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->yArySI[idSI];
} /*yCoordGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun09: widthGet_widg_rayWidg
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
   else if(widgSTPtr->widthArySI[idSI] <= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->widthArySI[idSI];
} /*widthGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun10: heightGet_widg_rayWidg
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
   else if(widgSTPtr->heightArySI[idSI] <= 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->heightArySI[idSI];
} /*heightGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun11: rowGet_widg_rayWidg
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
   else if(widgSTPtr->rowArySI[idSI] < 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->rowArySI[idSI];
} /*rowGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun12: colGet_widg_rayWidg
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
   else if(widgSTPtr->colArySI[idSI] < 0)
      return def_noValue_rayWidg;
   else
      return widgSTPtr->colArySI[idSI];
} /*colGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun13: stateGet_widg_rayWidg
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
   else
      return widgSTPtr->stateAryUC[idSI];
} /*stateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun14: pressStateGet_widg_rayWidg
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

   else if(
      widgSTPtr->stateAryUC[idSI] & def_press_rayWidg
   ) return 1;

   else
      return 0;
} /*pressStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun15: checkedStateGet_widg_rayWidg
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

   else if(
      widgSTPtr->stateAryUC[idSI] & def_checked_rayWidg
   ) return 1;

   else
      return 0;
} /*checkedStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun16: activeStateGet_widg_rayWidg
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
      widgSTPtr->stateAryUC[idSI] & def_active_rayWidg
   ) return 1;

   else
      return 0;
} /*activeStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun17: focusStateGet_widg_rayWidg
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

   else if(
      widgSTPtr->stateAryUC[idSI] & def_focus_rayWidg
   ) return 1;

   else
      return 0;
} /*focusStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun18: inactiveStateGet_widg_rayWidg
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

   else if(
      widgSTPtr->stateAryUC[idSI] & def_inactive_rayWidg
   ) return 1;

   else
      return 0;
} /*inactiveStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun19: hidenStateGet_widg_rayWidg
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

   else if(
      widgSTPtr->stateAryUC[idSI] & def_hiden_rayWidg
   ) return 1;

   else
      return 0;
} /*hidenStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun20: childStateGet_widg_rayWidg
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
unsigned char
childStateGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
){
   if(idSI >= widgSTPtr->lenSI)
      return def_noWidget_rayWidg;

   else if(
      ! (widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
   ) return 1;

   for( ; idSI > -1; --idSI)
   { /*Loop: find parent widget*/
      if(
        !(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
      ) return idSI; /*found parent widget*/
   } /*Loop: find parent widget*/

  return -3;/*widget assigned child state, but no parent*/
} /*childStateGet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun21: numWidgetGet_widg_rayWidg
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
| Fun22: focusGet_widg_rayWidg
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
| Fun23: xCoordSet_widg_rayWidg
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

   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;

  widgSTPtr->xArySI[idSI] ^= xCoordSI;
  xCoordSI ^= widgSTPtr->xArySI[idSI];
  widgSTPtr->xArySI[idSI] ^= xCoordSI;

  if(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
     return 0; /*no children to adjust*/

  xCoordSI -= widgSTPtr->xArySI[idSI];
  ++idSI;

  /*adjust the childrens x coordiantes*/
  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
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
| Fun24: yCoordSet_widg_rayWidg
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

   else if(widgSTPtr->rowArySI[idSI] >= 0)
      return def_noValue_rayWidg;

  widgSTPtr->yArySI[idSI] ^= yCoordSI;
  yCoordSI ^= widgSTPtr->yArySI[idSI];
  widgSTPtr->yArySI[idSI] ^= yCoordSI;

  if(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
     return 0; /*no children to adjust*/

  yCoordSI -= widgSTPtr->yArySI[idSI];
  ++idSI;

  /*adjust the childrens y coordiantes*/
  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
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
| Fun25: widthSet_widg_rayWidg
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

  widgSTPtr->widthArySI[idSI] = widthSI;
  return 0;
} /*widthSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun26: heightSet_widg_rayWidg
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

  widgSTPtr->heightArySI[idSI] = heightSI;
  return 0;
} /*heightSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun27: rowSet_widg_rayWidg
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

  widgSTPtr->rowArySI[idSI] = rowSI;

  if(rowSI < 0)
     widgSTPtr->colArySI[idSI] = -1;
     /*removing tile state*/

  return 0;
} /*rowSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun28: colSet_widg_rayWidg
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

  widgSTPtr->colArySI[idSI] = colSI;

  if(colSI < 0)
     widgSTPtr->rowArySI[idSI] = -1;
     /*removing tile state*/

  return 0;
} /*colSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun29: stateSet_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] = stateUC;

  stateUC &= def_inactive_rayWidg & def_hiden_rayWidg;

  if(! stateUC)
     return 0; /*no states to pass down to children*/
  ++idSI;
  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUC[idSI++] |= stateUC;

  return 0;
} /*stateSet_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun30: pressAdd_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] |= def_press_rayWidg;

  return 0;
} /*pressAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun31: checkedAdd_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] |= def_checked_rayWidg;

  return 0;
} /*checkedAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun32: activeAdd_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] |= def_active_rayWidg;

  return 0;
} /*activeAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun33: focusAdd_widg_rayWidg
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
  
  if(widgSTPtr->focusSI < 0)
     ;
  else if(widgSTPtr->focusSI > widgSTPtr->lenSI)
     ;
  else
     widgSTPtr->stateAryUC[widgSTPtr->focusSI] &=
        ~def_focus_rayWidg;

  if(idSI >= 0)
  { /*If: setting focus*/
     widgSTPtr->stateAryUC[idSI] |= def_focus_rayWidg;
     widgSTPtr->focusSI = idSI;
  } /*If: setting focus*/

  else
     widgSTPtr->focusSI = -1; /*clearing focus*/

  return 0;
} /*focusAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun34: inactiveAdd_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] |= def_inactive_rayWidg;
  if(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
     return 0; /*this is a child, so no other children*/
  ++idSI;


  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUC[idSI++] |= def_inactive_rayWidg;

  return 0;
} /*inactiveAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun35: hidenAdd_widg_rayWidg
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

  widgSTPtr->stateAryUC[idSI] |= def_hiden_rayWidg;
  if(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
     return 0; /*this is a child, so no other children*/
  ++idSI;

  while(
        idSI < widgSTPtr->lenSI
     && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUC[idSI++] |= def_hiden_rayWidg;

  return 0;
} /*hidenAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun36: childAdd_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] |= def_child_rayWidg;
  return 0;
} /*hidenAdd_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun37: stateClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] = 0;
  return 0;
} /*stateClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun38: pressClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_press_rayWidg;
  return 0;
} /*pressClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun39: checkedClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_checked_rayWidg;
  return 0;
} /*checkedClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun40: activeClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_active_rayWidg;
  return 0;
} /*activeClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun41: focusClear_widg_rayWidg
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
   if(widgSTPtr->focusSI == idSI)
      widgSTPtr->focusSI = -1;

  widgSTPtr->stateAryUC[idSI] &= ~def_focus_rayWidg;
  return 0;
} /*focusClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun42: inactiveClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_inactive_rayWidg;
  if(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
     return 0;
  ++idSI;

  while(
         idSI < widgSTPtr->lenSI
      && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUC[idSI++]&= ~def_inactive_rayWidg;

  return 0;
} /*inactiveClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun43: hidenClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_hiden_rayWidg;
  ++idSI;

  while(
         idSI < widgSTPtr->lenSI
      && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
  ) widgSTPtr->stateAryUC[idSI++] &= ~def_hiden_rayWidg;

  return 0;
} /*hidenClear_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun44: childClear_widg_rayWidg
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
  widgSTPtr->stateAryUC[idSI] &= ~def_child_rayWidg;
  return 0;   
} /*childClear_widg_rayWidg*/


/*-------------------------------------------------------\
| Fun45: getDarkModeState_rayWidg
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
| Fun46: checkGuiColorMode_rayWidg
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
| Fun47: addWidget_widg_rayWidg
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
      goto memErr_fun46; /*memory error while resizing*/
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

   memErr_fun46:;
      return 1;
} /*addWidget_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun48: tile_widg_rayWidg
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
   ' Fun48 TOC:
   '   - find tiling x,y coordiantes for each widget using
   '     widths, heights, rows, and columns
   '   o fun48 sec01:
   '     - variable declarations
   '   o fun48 sec02:
   '     - get maximum rows and column (and get memory)
   '   o fun48 sec03:
   '     - find parent widget row and column coordinates
   '   o fun48 sec04:
   '     - add x,y to tiled parents and update non-tiled
   '       child widgets
   '   o fun48 sec05:
   '     - find tile coordinates for tiled child widgets
   '   o fu10 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun48 Sec01:
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
   ^ Fun48 Sec02:
   ^   - get maximum rows and column (and get memory)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(rowOffsetSI < 0)
      rowOffsetSI = widgSTPtr->charPadSI >> 3;
   if(colOffsetSI < 0)
      colOffsetSI =
         MeasureText("a", widgSTPtr->fontSizeSI) >> 1;

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
      goto noTile_fun48_sec06;

   ++rowSizeSI;
   ++colSizeSI;

   rowHeapArySI = calloc(rowSizeSI, sizeof(signed int));
   if(! rowHeapArySI)
      goto memErr_fun48_sec06;
   colHeapArySI = calloc(colSizeSI, sizeof(signed int));
   if(! colHeapArySI)
      goto memErr_fun48_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun48 Sec03:
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
         widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
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
   ^ fun48 sec04:
   ^   - add x,y to tiled parents and update non-tiled
   ^     child widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: add in x,y coordaintes*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto nextWidg_fun48_sec04;
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto nextWidg_fun48_sec04;
      else if(
         widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
      ) goto nextWidg_fun48_sec04;

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
        && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
      ){ /*Loop: update x,y coord child coordinates*/

          /*check if children were tiled in the widget*/
          if(widgSTPtr->rowArySI[idSI] >= 0)
             goto nextChild_fun48_sec04;
          else if(widgSTPtr->colArySI[idSI] >= 0)
             goto nextChild_fun48_sec04;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] += yChangeSI;
         widgSTPtr->xArySI[idSI] += xChangeSI;

         nextChild_fun48_sec04:;
            ++idSI;
      }  /*Loop: update x,y coord child coordinates*/

      continue; /*already on next parent wiget*/

      nextWidg_fun48_sec04:;
         ++idSI;
   } /*Loop: add in x,y coordaintes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun48 Sec05:
   ^   - find tile coordinates for tiled child widgets
   ^   o fun48 sec05 sub01:
   ^     - find and setup for found child group
   ^   o fun48 sec05 sub02:
   ^     - find column and row sizes in child group
   ^   o fun48 sec05 sub03:
   ^     - add x,y coordinates in child group
   ^   o fun48 sec05 sub04:
   ^     - move to next widget
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun48 Sec05 Sub01:
   *   - find and setup for found child group
   \*****************************************************/

   rowLenSI = 0;
   colLenSI = 0;
   idSI = 0;

   while(idSI < widgSTPtr->lenSI)
   { /*Loop: find tile dimensions*/
      if(widgSTPtr->rowArySI[idSI] < 0)
         goto childNextWidg_fun48_sec05_sub05;
         /*non-tiled*/
      else if(widgSTPtr->colArySI[idSI] < 0)
         goto childNextWidg_fun48_sec05_sub05;
         /*non-tiled*/
      else if(
        !(widgSTPtr->stateAryUC[idSI] & def_child_rayWidg)
      ) goto childNextWidg_fun48_sec05_sub05;
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
      * Fun48 Sec05 Sub02:
      *   - find column and row sizes in child group
      \**************************************************/

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
      ){ /*Loop: find child widget row and column sizes*/

         if(widgSTPtr->rowArySI[idSI] < 0)
            goto nextChild_fun48_sec05_sub02;/*non-tiled*/
         else if(widgSTPtr->colArySI[idSI] < 0)
            goto nextChild_fun48_sec05_sub02;/*non-tiled*/

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

         nextChild_fun48_sec05_sub02:;
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
      * Fun48 Sec05 Sub03:
      *   - add x,y coordinates in child group
      \**************************************************/

      idSI = tmpSI;

      while(
           idSI < widgSTPtr->lenSI
        && widgSTPtr->stateAryUC[idSI] & def_child_rayWidg
      ){ /*Loop: set tiled child wiget x,y coordinates*/

          /*check if children were non-tile widgets*/
          if(widgSTPtr->rowArySI[idSI] < 0)
             goto addCoordNextChild_fun48_sec05_sub03;
          else if(widgSTPtr->colArySI[idSI] < 0)
             goto addCoordNextChild_fun48_sec05_sub03;

         siRow = widgSTPtr->rowArySI[idSI];
         siCol = widgSTPtr->colArySI[idSI];
         
         widgSTPtr->yArySI[idSI] = rowHeapArySI[siRow];
         widgSTPtr->xArySI[idSI] = colHeapArySI[siCol];

         addCoordNextChild_fun48_sec05_sub03:;
            ++idSI;
      }  /*Loop: set tiled child wiget x,y coordinates*/

      /**************************************************\
      * Fun48 Sec05 Sub04:
      *   - move to next widget
      \**************************************************/

      continue; /*on next parent widget*/

      childNextWidg_fun48_sec05_sub05:;
         ++idSI;
   } /*Loop: find tile dimensions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fu10 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idSI = 0;
   goto ret_fun48_sec06;

   noTile_fun48_sec06:;
      idSI = 0;
      goto ret_fun48_sec06;

   memErr_fun48_sec06:;
      idSI = 1;
      goto ret_fun48_sec06;

   ret_fun48_sec06:;
      if(rowHeapArySI)
         free(rowHeapArySI);
      rowHeapArySI = 0;

      if(colHeapArySI)
         free(colHeapArySI);
      colHeapArySI = 0;

      return idSI;
} /*tile_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun49: focusCheck_widg_rayWidg
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
|     o stateAryUC[focus] in widgSTPtr to be set to
|       def_focus_rayWidg
|     o stateAryUC[old_focus] in widgSTPtr to have the
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
   ' Fun49 TOC:
   '   - detect if input key was tab and update focus
   '   o fun49 sec01:
   '     - find if focus and set focus
   '   o fun49 sec02:
   '     - check if next widget can be focused, if not
   '       find the next widget that can be focused
   '   o fun49 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun49 Sec01:
  ^   - find if focus and set focus
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  signed char endBl = 0;   /*mark if hit end of widgets*/
  signed char shiftBl = 0; /*if shift key was pressed*/
     /*not using alt tab, because alt tab is often a
     `  system hotkey for reverse cycle
     */

  shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
  shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

  if(charBl)
  { /*If: character input*/
     if(keySI == '\t')
        charBl = shiftBl;
     else
        goto nonFocusKey_fun49_sec04;
  } /*If: character input*/

  else
  { /*Else: key event input*/
     if(keySI == KEY_TAB)
        charBl = shiftBl;
     else
        goto nonFocusKey_fun49_sec04;
  } /*Else: key event input*/

  /*quickly wipe old focus; less efficent, but bit neater
  `   code to handle in two steps
  */
  if(widgSTPtr->focusSI < 0)
     ;
  else if(widgSTPtr->focusSI < widgSTPtr->lenSI)
  { /*Else If: have a widget in focus*/
     widgSTPtr->stateAryUC[widgSTPtr->focusSI] &=
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

  /*move focus to next widget*/
  if(charBl)
  { /*If: moving focus to last widget*/
     if(widgSTPtr->focusSI <= 0)
        widgSTPtr->focusSI = widgSTPtr->lenSI - 1;
     else
        --widgSTPtr->focusSI;
  } /*If: moving focus to last widget*/

  else
  { /*Else: moving focus to next widget*/
     if(widgSTPtr->focusSI >= widgSTPtr->lenSI)
        widgSTPtr->focusSI = 0;
     else if(widgSTPtr->focusSI < 0)
        widgSTPtr->focusSI = 0;
     else
        ++widgSTPtr->focusSI;
  } /*Else: moving focus to next widget*/

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun49 Sec02:
  ^   - check if next widget can be focused, if not find
  ^     the next widget that can be focused
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  /*check if next widget can be focused*/
  keySI = widgSTPtr->focusSI;
  while(
       widgSTPtr->stateAryUC[keySI] & def_hiden_rayWidg
    || widgSTPtr->stateAryUC[keySI] & def_inactive_rayWidg
  ){ /*Loop: find next widget in focus*/
     if(charBl)
     { /*If: moving focus up*/
        --keySI;

        if(keySI < 0)
        { /*If: need to move to start*/
           if(endBl)
              goto noFocus_fun49_sec04;
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
              goto noFocus_fun49_sec04;
           endBl = 1;
           keySI = 0;
        } /*If: need to move to start*/
     } /*Else: moving down*/
  }  /*Loop: find next widget in focus*/

  widgSTPtr->focusSI = keySI;
  widgSTPtr->stateAryUC[keySI] |= def_focus_rayWidg;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun49 Sec03:
  ^   - return
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  return keySI;

  noFocus_fun49_sec04:;
     widgSTPtr->focusSI = -1;
     return -1;
  nonFocusKey_fun49_sec04:;
      return -2;
} /*focusCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun50: enterCheck_widg_rayWidg
|   - check if key event was an enter key
| Input:
|   - keySI:
|     o raylib key returned from GetKey() 
|   - charBl:
|     o 1: keySC is char from GetChar()
|     o 0: keySC is key from GetKey()
|   - widgSTPtr:
|     o widg_rayWidg struct pointer with focused widget
|       to set press state
| Output:
|   - Modifies:
|     o stateAryUC[widgSTPtr->focusSI] in widgSTPtr to be
|       set to def_press_rayWidg
|   - Returns:
|     - index of the pressed widget (widgSTPtr->focusSI)
|     - -1 if focused widget was hiden or inactive
|       * no press event can be done
|     - -2 if input key was not enter '\n'
\-------------------------------------------------------*/
signed int
enterCheck_widg_rayWidg(
   signed int keySI,   /*key from GetKey()*/
   signed char charBl, /*1: key was from GetChar()*/
   struct widg_rayWidg *widgSTPtr /*has focus to change*/
){
   if(charBl)
   { /*If: character input*/
      if(keySI == '\n')
         charBl = 1;
      else
         goto noPress_fun50;
   } /*If: character input*/

   else
   { /*Else: key event input*/
      if(keySI == KEY_ENTER)
         charBl = 1;
      else
         goto noPress_fun50;
   } /*Else: key event input*/


   /*check if widget is pressable*/
   keySI = widgSTPtr->focusSI;
   if(keySI < 0)
      goto noFocus_fun50;
   else if(keySI >= widgSTPtr->lenSI)
      goto noFocus_fun50;
   else if(
      widgSTPtr->stateAryUC[keySI] & def_hiden_rayWidg
   ) goto noFocus_fun50;
   else if(
      widgSTPtr->stateAryUC[keySI] & def_inactive_rayWidg
   ) goto noFocus_fun50;


   pressAdd_widg_rayWidg(keySI, widgSTPtr);
   return keySI;

   noFocus_fun50:;
      return -1;
   noPress_fun50:;
      return -2;
} /*enterCheck_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun51: getMouseWidg_widg_rayWidg
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
|     o -1 if the mouse was on no widget
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
   ' Fun51 TOC:
   '   - get widget mouse was on
   '   o fun51 sec01:
   '     - find first parent widget the mouse coud be on
   '   o fun51 sec02:
   '     - check if there is a child widget the mouse
   '       could be on
   '   o fun51 sec03:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun51 Sec01:
   ^   - find first parent widget the mouse coud be on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int retSI = 0;

   for(
      *parSIPtr = 0;
      *parSIPtr < widgSTPtr->lenSI;
      ++*parSIPtr
   ){ /*Loop: find first widget that could match*/
      if(
           widgSTPtr->stateAryUC[*parSIPtr]
         & def_child_rayWidg
      ) continue;

      else if(xSI < widgSTPtr->xArySI[*parSIPtr])
         continue;

      else if(ySI < widgSTPtr->yArySI[*parSIPtr])
         continue;

      else if(
           widgSTPtr->stateAryUC[*parSIPtr]
         & def_hiden_rayWidg
      ) continue;

      else if(
           widgSTPtr->stateAryUC[*parSIPtr]
         & def_inactive_rayWidg
      ) continue;

      else if(
           xSI
         > widgSTPtr->xArySI[*parSIPtr]
           + widgSTPtr->widthArySI[*parSIPtr]
      ) continue;

      else if(
           ySI
         > widgSTPtr->yArySI[*parSIPtr]
           + widgSTPtr->heightArySI[*parSIPtr]
      ) continue;

      break; /*found widget*/
   }  /*Loop: find first widget that could match*/

   if(*parSIPtr >= widgSTPtr->lenSI)
      goto noWidg_fun51_sec0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun51 Sec02:
   ^   - check if there is a child widget the mouse could
   ^     be on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retSI = *parSIPtr;

   ++retSI;
   while(
         widgSTPtr->stateAryUC[retSI] & def_child_rayWidg
      && retSI < widgSTPtr->lenSI
   ){ /*Loop: see if can find the child widget*/
      if(xSI < widgSTPtr->xArySI[retSI])
         continue;

      else if(ySI < widgSTPtr->yArySI[retSI])
         continue;

      else if(
         widgSTPtr->stateAryUC[retSI] & def_hiden_rayWidg
      ) continue;

      else if(
           widgSTPtr->stateAryUC[retSI]
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
   }  /*Loop: see if can find the child widget*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun51 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*parSIPtr >= widgSTPtr->lenSI)
      goto noChild_fun51_sec0x;

   if(widgSTPtr->stateAryUC[retSI] & def_child_rayWidg)
      ;
   else
      goto noChild_fun51_sec0x;


   goto retWidg_run51_sec0x;

   noChild_fun51_sec0x:;
      retSI = *parSIPtr;
      *parSIPtr = -1;
      goto retWidg_run51_sec0x;

   retWidg_run51_sec0x:;
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
      ); /*clear the old focus; also clears focus item*/

      widgSTPtr->focusSI = retSI;
      focusAdd_widg_rayWidg(retSI, widgSTPtr);
         /*set clicked widget to focused widget*/
      activeAdd_widg_rayWidg(retSI, widgSTPtr);
         /*make into an active widget*/

      return retSI;

   noWidg_fun51_sec0x:;
      *parSIPtr = -1;
      return -1;
} /*getMouseWidg_widg_rayWidg*/

/*-------------------------------------------------------\
| Fun52: blinkGet_rayWidg
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
| Fun53: intStrCheck_rayWidg
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
      goto ret_fun54;

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

   ret_fun54:;
      return dupStr - textStr;
} /*intStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun54: floatStrCheck_rayWidg
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
      goto ret_fun54;

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

   ret_fun54:;
      return dupStr - textStr;
} /*floatStrCheck_rayWidg*/

/*-------------------------------------------------------\
| Fun55: fileStrCheck_rayWidg
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
| Fun56: addCharToEntry_rayWidg
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
|     o widg_rayWidg struct pointer with font size for GUI
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
   struct widg_rayWidg *widgSTPtr /*has font size*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun56 TOC:
   '   - adds a character to an entry box text
   '   o fun56 sec01:
   '     - variable declaration and movement keys
   '   o fun56 sec02:
   '     - delete key
   '   o fun56 sec03:
   '     - backspace key
   '   o fun56 sec04:
   '     - keys that need a conversion
   '   o fun56 sec05:
   '     - normal keys
   '   o fun56 sec06:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec01:
   ^   - variable declaration and movement keys
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int posSI = posArySI[1];
   signed int textWidthSI = 0;
   signed int lenSI = 0;   /*find final output length*/
   signed char shiftBl = 0;

   /*for measuring final string width*/
   signed char swapArySC[3] = {0, 0, 0};
   signed char rmArySC[2] = {0, 0};


   shiftBl = IsKeyDown(KEY_LEFT_SHIFT);
   shiftBl |= IsKeyDown(KEY_RIGHT_SHIFT);

   if(keySI == KEY_LEFT)
   { /*If: move left*/
      if(posArySI[1] > 0)
         --posArySI[1];
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      goto cursorMove_fun56_sec06;
   } /*If: move left*/

   else if(keySI == KEY_RIGHT)
   { /*Else If: move left*/
      if(posArySI[1] < *lenSIPtr)
         ++posArySI[1];
      goto cursorMove_fun56_sec06;
   } /*Else If: move left*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec02:
   ^   - delete key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_DELETE)
   { /*Else If: deleting the next character*/
      if(*lenSIPtr <= 0)
         goto cursorMove_fun56_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] >= *lenSIPtr)
         goto cursorMove_fun56_sec06;
         /*at end of string; nothing to delete*/

      else if(posArySI[1] == *lenSIPtr - 1)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1]] = 0;
         --*lenSIPtr;
         goto delete_fun56_sec06;
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
         goto delete_fun56_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: deleting the next character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec03:
   ^   - backspace key
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_BACKSPACE)
   { /*Else If: backspace to delete last character*/
      if(posArySI[1] <= 0)
         goto cursorMove_fun56_sec06;
         /*at start of string; nothing to delete*/

      else if(*lenSIPtr <= 0)
         goto cursorMove_fun56_sec06;
         /*delete, but nothing to delete*/

      else if(posArySI[1] == *lenSIPtr)
      { /*Else If: only one character to delete*/
         textStr[posArySI[1] - 1] = 0;
         --posArySI[1];
         --*lenSIPtr;
         goto delete_fun56_sec06;
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
         goto delete_fun56_sec06;
      } /*Else: need to shift items around*/
   } /*Else If: backspace to delete last character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec04:
   ^   - keys that need a conversion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(keySI == KEY_END)
   { /*Else If: move cursor to end of text*/
      posArySI[1] = *lenSIPtr;
      goto cursorMove_fun56_sec06;
   } /*Else If: move cursor to end of text*/

   else if(keySI == KEY_HOME)
   { /*Else If: move cursor to start of text*/
      posArySI[0] = 0;
      posArySI[1] = 0;
      goto cursorMove_fun56_sec06;
   } /*Else If: move cursor to start of text*/

   else if(keySI == KEY_KP_0)
   { /*Else If: key pad 0*/
      keySI = '0';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 0*/

   else if(keySI == KEY_KP_1)
   { /*Else If: key pad 1*/
      keySI = '1';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 1*/

   else if(keySI == KEY_KP_2)
   { /*Else If: key pad 2*/
      keySI = '2';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 2*/

   else if(keySI == KEY_KP_3)
   { /*Else If: key pad 3*/
      keySI = '3';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 3*/

   else if(keySI == KEY_KP_4)
   { /*Else If: key pad 4*/
      keySI = '4';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 4*/

   else if(keySI == KEY_KP_5)
   { /*Else If: key pad 5*/
      keySI = '5';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 5*/

   else if(keySI == KEY_KP_6)
   { /*Else If: key pad 6*/
      keySI = '6';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 6*/

   else if(keySI == KEY_KP_7)
   { /*Else If: key pad 7*/
      keySI = '7';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 7*/

   else if(keySI == KEY_KP_8)
   { /*Else If: key pad 8*/
      keySI = '8';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 8*/

   else if(keySI == KEY_KP_9)
   { /*Else If: key pad 9*/
      keySI = '9';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad 9*/

   else if(keySI == KEY_KP_DECIMAL)
   { /*Else If: key pad .*/
      keySI = '.';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad .*/

   else if(keySI == KEY_KP_DIVIDE)
   { /*Else If: key pad /*/
      keySI = '/';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad /*/

   else if(keySI == KEY_KP_MULTIPLY)
   { /*Else If: key pad **/
      keySI = '*';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad **/

   else if(keySI == KEY_KP_SUBTRACT)
   { /*Else If: key pad -*/
      keySI = '-';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad -*/

   else if(keySI == KEY_KP_ADD)
   { /*Else If: key pad +*/
      keySI = '+';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad +*/

   else if(keySI == KEY_KP_EQUAL)
   { /*Else If: key pad =*/
      keySI = '=';
      goto addNormalKey_fun56_sec05;
   } /*Else If: key pad =*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec05:
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
         
   
      addNormalKey_fun56_sec05:;

      if(posArySI[1] >= *lenSIPtr)
      { /*If: at end of string*/
         textStr[posArySI[1]] = keySI;
         ++posArySI[1];
         ++*lenSIPtr;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun56_sec06;
      } /*If: at end of string*/

      else
      { /*Else: need to insert the new character*/
         for(posSI=*lenSIPtr;posSI > posArySI[1];--posSI)
            textStr[posSI] = textStr[posSI - 1];

         ++posArySI[1];
         ++*lenSIPtr;
         textStr[posSI] = keySI;
         textStr[*lenSIPtr] = 0;
         goto addChar_fun56_sec06;
      } /*Else: need to insert the new character*/
   } /*Else If: normal key pressed*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun56 Sec06:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cursorMove_fun56_sec06;
      /*no idea what the key event is*/

   delete_fun56_sec06:;
      if(posArySI[0] > posArySI[1])
         posArySI[0] = posArySI[1];
      posSI = -1;
      goto ret_fun56_sec06;

   addChar_fun56_sec06:;
      posSI = 1;
      goto adjustPos_fun56_sec06;

   cursorMove_fun56_sec06:;
      posSI = 0;
      goto adjustPos_fun56_sec06;

   adjustPos_fun56_sec06:;
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

      textWidthSI =
         MeasureText(
            (char *) &textStr[posArySI[0]],
            widgSTPtr->fontSizeSI
         );

      while(textWidthSI > widthSI)
      { /*Loop: find text length*/
         if(posArySI[0] == posArySI[1])
            break; /*width only allows one character*/

         rmArySC[0] = textStr[posArySI[0]];
         ++posArySI[0];
            /*cursor moved out of window, so need to
            `  move window one forward
            */
         textWidthSI -=
            MeasureText(
               (char *) rmArySC,
               widgSTPtr->fontSizeSI
            );
      } /*Loop: find text length*/

      textStr[posArySI[1]] = swapArySC[0];
      textStr[posArySI[1] + 1] = swapArySC[1];
      textStr[posArySI[1] + 2] = swapArySC[2];

      goto ret_fun56_sec06;

   ret_fun56_sec06:;
      textStr[*lenSIPtr] = 0;
      return (signed char) posSI;
} /*addCharToEntry_rayWidg*/

/*-------------------------------------------------------\
| Fun57: butDraw_rayWidg
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
   ' Fun57 TOC:
   '   - draw a button
   '   o fun57 sec01:
   '     - variable declarations
   '   o fun57 sec02:
   '     - get color of button (state in)
   '   o fun57 sec03:
   '     - get width of button
   '   o fun57 sec04:
   '     - draw button
   '   o fun57 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun57 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color butCol;
   struct Color borderCol;
   struct Color textCol;

   #define def_maxButText_fun57 32
      /*max number of characters in button*/
   signed char writeStr[def_maxButText_fun57 + 8];
   signed int oneCharSI =
      MeasureText("a", widgSTPtr->fontSizeSI);

   /*these are for shorting strings*/
   signed char histUC = 0;
   signed int histIndexSI = 0;

   /*for drawing focus borders*/
   signed int xSI = 0;
   signed int ySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun57 Sec02:
   ^   - get color of button (state in)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI > widgSTPtr->lenSI)
      goto noWidget_fun57_sec05;

   if(widgSTPtr->stateAryUC[idSI] & def_hiden_rayWidg)
       goto hiddenWidget_fun57_sec05;

   else if(
      widgSTPtr->stateAryUC[idSI] & def_inactive_rayWidg
   ){ /*Else If: button is in a disabled state*/
      butCol = GetColor(widgSTPtr->guiColSI);
      borderCol=GetColor(widgSTPtr->butBorderColSI);
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a disabled state*/

   else if(widgSTPtr->stateAryUC[idSI] &def_press_rayWidg)
   { /*Else If: button is in a press state*/
      butCol = GetColor(widgSTPtr->butPressColSI);
      borderCol=GetColor(widgSTPtr->butPressBorderColSI);
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: button is in a press state*/

   else
   { /*Else If: button is in a normal state*/
      butCol = GetColor(widgSTPtr->butColSI);
      borderCol=GetColor(widgSTPtr->butBorderColSI);
      textCol = GetColor(widgSTPtr->textColSI);
   } /*Else If: button is in a normal state*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun57 Sec03:
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
        widgSTPtr->widthArySI[idSI] > def_maxButText_fun57
      )widgSTPtr->widthArySI[idSI] = def_maxButText_fun57;

      cpLen_ulCp(
         writeStr,
         textStr,
         widgSTPtr->widthArySI[idSI]
      );

      histIndexSI = widgSTPtr->widthArySI[idSI];
      widgSTPtr->widthArySI[idSI] =
         MeasureText(
            (char *) writeStr,
            widgSTPtr->fontSizeSI
         );
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

         widgSTPtr->widthArySI[idSI] =
            MeasureText(
               (char *) writeStr,
               widgSTPtr->fontSizeSI
            );
      } /*Loop: find number of bytes*/

      if(histIndexSI >= 3)
      { /*If: can copy in ...*/
         writeStr[histIndexSI - 1] = '.';
         writeStr[histIndexSI - 2] = '.';
         writeStr[histIndexSI - 3] = '.';
      } /*If: can copy in ...*/
   } /*Else If: I can not draw the full button*/

   if(! widgSTPtr->widthArySI[idSI])
      widgSTPtr->widthArySI[idSI] =
         MeasureText("a", widgSTPtr->fontSizeSI);
         /*button needs to be at least 1 char*/

   widgSTPtr->heightArySI[idSI] = oneCharSI << 1;
   widgSTPtr->heightArySI[idSI] += oneCharSI;

   widgSTPtr->widthArySI[idSI] += (oneCharSI << 1);
      /*want a one character pad around the button*/

   if(noDrawBl)
      goto done_fun57_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun57 Sec04:
   ^   - draw button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef MAC
      DrawRectangleRounded(
         (Rectangle){
            widgSTPtr->xArySI[idSI],
            widgSTPtr->yArySI[idSI],
            widgSTPtr->widthArySI[idSI],
            widgSTPtr->heightArySI[idSI]
         }, /*coordinates for rectangle*/
         def_macRoundness_rayWidg,
            /*roundness of rectangle*/
         def_macSegments_rayWidg,
             /*no idea what segments are for*/
         butCol
      );
      DrawRectangleRoundedLines(
         (Rectangle){
            widgSTPtr->xArySI[idSI],
            widgSTPtr->yArySI[idSI],
            widgSTPtr->widthArySI[idSI],
            widgSTPtr->heightArySI[idSI]
         }, /*coordinates for rectangle*/
         def_macRoundness_rayWidg,
            /*roundness of rectangle*/
         def_macSegments_rayWidg,
             /*no idea what segments are for*/
         borderCol
      );
   #else
      DrawRectangle(
         widgSTPtr->xArySI[idSI],
         widgSTPtr->yArySI[idSI],
         widgSTPtr->widthArySI[idSI],
         widgSTPtr->heightArySI[idSI],
         butCol
      );

      DrawRectangleLines(
         widgSTPtr->xArySI[idSI],
         widgSTPtr->yArySI[idSI],
         widgSTPtr->widthArySI[idSI],
         widgSTPtr->heightArySI[idSI],
         borderCol
      );
   #endif

   if(textStr)
      DrawText(
         (char *) writeStr,
         widgSTPtr->xArySI[idSI] + oneCharSI,
            /*center text in x*/
         widgSTPtr->yArySI[idSI] + (oneCharSI >> 1),
            /*center text on y*/
         widgSTPtr->fontSizeSI,
         textCol
      );

   if(widgSTPtr->stateAryUC[idSI] & def_focus_rayWidg)
   { /*If: button is in focus*/
      borderCol = GetColor(widgSTPtr->focusColSI);

      xSI = widgSTPtr->xArySI[idSI];
      xSI -= def_focusBorder_rayWidg;
      if(xSI < 0) xSI = 0;

      ySI = widgSTPtr->yArySI[idSI];
      ySI -= def_focusBorder_rayWidg;
      if(ySI < 0) ySI = 0;

      #ifdef MAC
         DrawRectangleRoundedLines(
            (Rectangle){
               xSI,
               ySI,
               widgSTPtr->widthArySI[idSI]
                  + def_focusBorder_rayWidg,
               widgSTPtr->heightArySI[idSI]
                  + def_focusBorder_rayWidg,
            }, /*coordinates for rectangle*/
            def_macRoundness_rayWidg,
               /*roundness of rectangle*/
            def_macSegments_rayWidg,
                /*no idea what segments are for*/
            butCol
         );
      #else
         DrawRectangleLines(
            xSI,
            ySI,
            widgSTPtr->widthArySI[idSI]
               + def_focusBorder_rayWidg,
            widgSTPtr->heightArySI[idSI]
               + def_focusBorder_rayWidg,
            borderCol
         );
      #endif
   } /*If: button is in focus*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun57 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun57_sec05:;
      return widgSTPtr->widthArySI[idSI];

   noWidget_fun57_sec05:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun57_sec05:;
      return 0;
} /*butDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun58: entryDraw_rayWidg
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
   ' Fun58 TOC:
   '   - draw an entry box, prevents typing if maxium is
   '     exceded or wrong entry type
   '   o fun58 sec01:
   '     - variable declarations
   '   o fun58 sec02:
   '     - check state of entry box
   '   o fun58 sec03:
   '     - copy string & check/set scroll & cursor values
   '   o fun58 sec04:
   '     - find text size and print cursor
   '   o fun58 sec05:
   '     - draw entry box
   '   o fun58 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color boxCol;
   struct Color borderCol;
   struct Color textCol;

   #define def_maxChar_fun58 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun58 + 8];
   signed char *cpStr = 0;
   signed char *dupStr = 0;
   signed char rmStr[2] = {0, 0}; /*for adjusting length*/

   signed int tmpSI = 0;
   signed int outLenSI = 0;
   signed int cursorPosSI = posArySI[1];

   signed int oneCharSI =
      MeasureText("a", widgSTPtr->fontSizeSI);
   signed int xSI = 0; /*for drawing focus border*/
   signed int ySI = 0; /*for drawing focus border*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec02:
   ^   - check state of entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun58_sec06;

   if(widgSTPtr->stateAryUC[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun58_sec06;
      /*entry box is hidden*/

   else if(
      widgSTPtr->stateAryUC[idSI] & def_inactive_rayWidg
   ){ /*Else If: entry box is being used*/
      boxCol = GetColor(widgSTPtr->guiColSI);
      borderCol = GetColor(widgSTPtr->entryBorderColSI);
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else If: entry box is being used*/

   /*will likely remove, or use lighter shade difference*/
   else if(
      widgSTPtr->stateAryUC[idSI] & def_active_rayWidg
   ){ /*Else If: entry box is being used*/
      boxCol = GetColor(widgSTPtr->entryActiveColSI);
      borderCol =
         GetColor(widgSTPtr->entryActiveBorderColSI);
      textCol = GetColor(widgSTPtr->textAltColSI);
   } /*Else If: entry box is being used*/

   else
   { /*Else: not being used*/
      boxCol = GetColor(widgSTPtr->entryColSI);
      borderCol = GetColor(widgSTPtr->entryBorderColSI);
      textCol = GetColor(widgSTPtr->textColSI);
      cursorPosSI = -1; /*not showing cursor*/
   } /*Else: not being used*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec03:
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


   if(outLenSI >= def_maxChar_fun58)
      outLenSI = def_maxChar_fun58 - 1;
         /*need one character for the cursor*/

   cpLen_ulCp(outStr, &textStr[posArySI[0]], outLenSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec04:
   ^   - find text size and print cursor
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*get cursor in measurment*/
   if(cursorPosSI >= 0)
   { /*If: adding in a cursor*/
      cursorPosSI = posArySI[1];
      outStr[outLenSI] = def_cursor_rayWidg;
      outStr[outLenSI + 1] = 0;
      tmpSI =
         MeasureText(
            (char *) outStr,
            widgSTPtr->fontSizeSI
         );
      outStr[outLenSI] = 0;
         /*will add cursor back in later*/
      --outLenSI;
      cpStr = outStr;
   } /*If: adding in a cursor*/

   else
   { /*Else: not adding a cursor*/
      tmpSI =
         MeasureText(
            (char *) outStr,
            widgSTPtr->fontSizeSI
         );
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

      tmpSI -=
        MeasureText((char *) rmStr,widgSTPtr->fontSizeSI);
   } /*Loop: shorten string down*/

   ++outLenSI;
   cpLen_ulCp(outStr, cpStr, outLenSI);

   if(cursorPosSI < 0)
      goto getDimensions_fun58_sec04;

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

   getDimensions_fun58_sec04:;

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] = oneCharSI << 1;
   widgSTPtr->heightArySI[idSI] += oneCharSI;

   widgSTPtr->widthArySI[idSI] = widthSI+(oneCharSI <<1);
      /*the oneCharSI << 1 is to give a 1 character
      `  padding around the edge
      */

   if(noDrawBl)
      goto done_fun58_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec05:
   ^   - draw entry box
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef MAC
      DrawRectangleRounded(
         (Rectangle){
            widgSTPtr->xArySI[idSI],
            widgSTPtr->yArySI[idSI],
            widgSTPtr->widthArySI[idSI],
            widgSTPtr->heightArySI[idSI]
         }, /*coordinates for rectangle*/
         def_macRoundness_rayWidg,
            /*roundness of rectangle*/
         def_macSegments_rayWidg,
             /*no idea what segments are for*/
         boxCol
      );
      DrawRectangleRoundedLines(
         (Rectangle){
            widgSTPtr->xArySI[idSI],
            widgSTPtr->yArySI[idSI],
            widgSTPtr->widthArySI[idSI],
            widgSTPtr->heightArySI[idSI]
         }, /*coordinates for rectangle*/
         def_macRoundness_rayWidg,
            /*roundness of rectangle*/
         def_macSegments_rayWidg,
             /*no idea what segments are for*/
         boxCol
      );
   #else
      DrawRectangle(
         widgSTPtr->xArySI[idSI],
         widgSTPtr->yArySI[idSI],
         widgSTPtr->widthArySI[idSI],
         widgSTPtr->heightArySI[idSI],
         boxCol
      );

      DrawRectangleLines(
         widgSTPtr->xArySI[idSI],
         widgSTPtr->yArySI[idSI],
         widgSTPtr->widthArySI[idSI],
         widgSTPtr->heightArySI[idSI],
         borderCol
      );
   #endif

   if(*outStr)
      DrawText(
         (char *) outStr,
         widgSTPtr->xArySI[idSI] + oneCharSI,
         widgSTPtr->yArySI[idSI] + (oneCharSI >> 1),
            /*keeping centered with box*/
         widgSTPtr->fontSizeSI,
         textCol
      );

   if(widgSTPtr->stateAryUC[idSI] & def_focus_rayWidg)
   { /*If: entry box is in focus*/
      borderCol = GetColor(widgSTPtr->focusColSI);

      xSI = widgSTPtr->xArySI[idSI];
      xSI -= def_focusBorder_rayWidg;
      if(xSI < 0) xSI = 0;

      ySI = widgSTPtr->yArySI[idSI];
      ySI -= def_focusBorder_rayWidg;
      if(ySI < 0) ySI = 0;

      #ifdef MAC
         DrawRectangleRoundedLines(
            (Rectangle){
               xSI,
               ySI,
               widgSTPtr->widthArySI[idSI]
                  + def_focusBorder_rayWidg,
               widgSTPtr->heightArySI[idSI]
                  + def_focusBorder_rayWidg,
            }, /*coordinates for rectangle*/
            def_macRoundness_rayWidg,
               /*roundness of rectangle*/
            def_macSegments_rayWidg,
                /*no idea what segments are for*/
            borderCol
         );
      #else
         DrawRectangleLines(
            xSI,
            ySI,
            widgSTPtr->widthArySI[idSI]
               + def_focusBorder_rayWidg,
            widgSTPtr->heightArySI[idSI]
               + def_focusBorder_rayWidg,
            borderCol
         );
      #endif
   } /*If: entry box is in focus*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun58 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun58_sec06:;
      return outLenSI;

   noWidget_fun58_sec06:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun58_sec06:;
      return 0;
} /*entryDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun59: labDraw_rayWidg
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
   ' Fun59 TOC:
   '   - draw a label
   '   o fun59 sec01:
   '     - variable declarations
   '   o fun59 sec02:
   '     - check state of label and copy string
   '   o fun59 sec03:
   '     - find text size and pad if needed
   '   o fun59 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color textCol;

   #define def_maxChar_fun59 256
      /*max characters in entry box*/
   signed char outStr[def_maxChar_fun59 + 8];
   signed int outLenSI = 0;
   signed int oldLenSI = 0;
   signed int tmpSI = 0;

   signed int oneCharSI =
      MeasureText("a", widgSTPtr->fontSizeSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec02:
   ^   - check state of label and copy string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(idSI >= widgSTPtr->lenSI)
      goto noWidget_fun59_sec04;

   if(widgSTPtr->stateAryUC[idSI] & def_hiden_rayWidg)
      goto hiddenWidget_fun59_sec04;
      /*entry box is hidden*/

   if(minWidthSI > def_maxChar_fun59)
      minWidthSI = def_maxChar_fun59;

   textCol = GetColor(widgSTPtr->textColSI);
   outLenSI = endStr_ulCp(textStr);

   if(outLenSI >= def_maxChar_fun59)
   { /*If: user input to long of string*/
      outLenSI = def_maxChar_fun59 - 4;
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
   ^ Fun59 Sec03:
   ^   - find text size and pad if needed
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpSI =
      MeasureText((char *) outStr, widgSTPtr->fontSizeSI);

   --outLenSI;

   while(tmpSI > maxWidthSI)
   { /*Loop: shorten string down*/
      outStr[outLenSI--] = 0;

      tmpSI =
        MeasureText((char *)outStr,widgSTPtr->fontSizeSI);
   } /*Loop: shorten string down*/

   ++outLenSI;

   if(padFlagSC)
   { /*If: padding*/
      oldLenSI = outLenSI;

      while(tmpSI < minWidthSI)
      { /*Loop: add initial padding to end*/
         outStr[outLenSI++] = padSC;
         outStr[outLenSI] = 0;
         tmpSI =
            MeasureText(
               (char *) outStr,
               widgSTPtr->fontSizeSI
            );
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

   outLenSI =
      MeasureText((char *) outStr, widgSTPtr->fontSizeSI);

   /*set width and height*/
   widgSTPtr->heightArySI[idSI] = oneCharSI << 1;
   widgSTPtr->heightArySI[idSI] += oneCharSI;

   widgSTPtr->widthArySI[idSI] = outLenSI + oneCharSI;
      /*the oneCharSI is to give a 1 character padding
      `  at the edge
      */

   if(noDrawBl)
      goto done_fun59_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun59 Sec04:
   ^   - draw label and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*outStr)
      DrawText(
          (char *) outStr,
          widgSTPtr->xArySI[idSI],
          widgSTPtr->yArySI[idSI] + (oneCharSI >> 1),
             /*keeping centered with box*/
          widgSTPtr->fontSizeSI,
          textCol
      );

   done_fun59_sec04:;
      return outLenSI;

   noWidget_fun59_sec04:;
      return def_noWidget_rayWidg;

   hiddenWidget_fun59_sec04:;
      return 0;
} /*labDraw_rayWidg*/

/*-------------------------------------------------------\
| Fun60: mkMesgBox_rayWidg
|   - makes an emtpy message box widget
| Input:
|   - widgSTPtr:
|     o widg_rayWidg struct pointer to add message box
|       and its children widgets to
| Output:
|   - Modifies:
|     o resizes arrays in widgSTptr if not enough memory
|       to hold the message box and its children widgets
|   - Returns:
|     o id (index) of the message box widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed int
mkMesgBox_rayWidg(
   struct widg_rayWidg *widgSTPtr
){
   signed int idSI = 0;
   idSI = widgSTPtr->lenSI + 4;

   if(idSI >= widgSTPtr->sizeSI)
   { /*If: need more memory*/
      idSI += (idSI >> 1);

      if( realloc_widg_rayWidg(idSI, widgSTPtr) )
         goto memErr_fun60;
   } /*If: need more memory*/

   idSI =
     addWidget_widg_rayWidg(0, 0, 0, 200, 100, widgSTPtr);

   addWidget_widg_rayWidg(
      0, /*colomn*/
      0, /*row*/
      1, /*use tile*/
      200, /*width*/
      widgSTPtr->charPadSI, /*height*/
      widgSTPtr
   ); /*add the top border*/
   childAdd_widg_rayWidg(idSI + 1, widgSTPtr);

   addWidget_widg_rayWidg(
      0, /*colomn*/
      1, /*row*/
      1, /*use tile*/
      200, /*width*/
      widgSTPtr->charPadSI, /*height*/
      widgSTPtr
   ); /*add the message*/
   childAdd_widg_rayWidg(idSI + 2, widgSTPtr);

   addWidget_widg_rayWidg(
      0,  /*colomn*/
      2,  /*row*/
      1,  /*use tile*/
      20, /*width*/
      widgSTPtr->charPadSI, /*height*/
      widgSTPtr
   ); /*add the ok button*/
   childAdd_widg_rayWidg(idSI + 3, widgSTPtr);

   hidenAdd_widg_rayWidg(idSI, widgSTPtr);
      /*hide the message box*/
   return idSI;

   memErr_fun60:;
      return -1;
} /*mkMesgBox_rayWidg*/

/*-------------------------------------------------------\
| Fun61: mesgBoxDraw_rayWidg
|   - draws a message box
| Input:
|   - idSI:
|     o id of message boxes parent widget
|       * a message box is a complex widget, with three
|         child widgets
|   - xSI:
|     o x coordinate of message box
|   - ySI:
|     o y coordinate of message box
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
|     o 0 for no errors
|     o 1 if message was to long
\-------------------------------------------------------*/
signed int
mesgBoxDraw_rayWidg(
   signed int idSI,      /*id of message box widget*/
   signed int xSI,       /*x coordinate to start draw at*/
   signed int ySI,       /*y coordinate to start draw at*/
   signed char *mesgStr, /*message to show*/
   signed char *butTextStr, /*text on button*/
   struct widg_rayWidg *widgSTPtr /*has widgets*/
){
   #define def_lenOut_fun61 128
   signed char outStr[def_lenOut_fun61 + 8];
   signed int textLenSI = endStr_ulCp(mesgStr);

   signed char *cpStr = 0;
   signed char *dupStr = 0;

   if(textLenSI > def_lenOut_fun61)
      goto mesgErr_fun61_sec0x;

   widgSTPtr->xArySI[idSI] = xSI;
   widgSTPtr->yArySI[idSI] = ySI;

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
   textLenSI =
      MeasureText((char *) outStr, widgSTPtr->fontSizeSI);

   /*TODO: VVV
   `  delete this and replace with draw button, here
   `  to appease compiler
   */
   textLenSI =
      MeasureText((char *) butTextStr, widgSTPtr->fontSizeSI);
   return 0;

   mesgErr_fun61_sec0x:;
      return 1;
} /*mesgBoxDraw_rayWidg*/
