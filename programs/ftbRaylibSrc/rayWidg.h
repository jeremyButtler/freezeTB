/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' rayWidg SOF: Start Of File
'   - widgets or other functions to manipulate raylib and
'     make the GUI building step eaiser
'   o header:
'     - guards and defiend variables
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
|   - defined variables and guards
|   o header sec01:
|     - forward declarations and guards
|   o header sec02:
|     - font size and widgets states
|   o header sec02:
|     - color scheme
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - forward declarations and guards
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifndef RAY_LIB_WIDGETS_H
#define RAY_LIB_WIDGETS_H

#define def_noValueGet_rayWidg -1
   /*attribute (x/y coordinate, row, and column) has no
   `  assigned value yet. For <attribute>Get_rayWidg and
   `  <x,y>GetCoord_rayWidg functions
   */
#define def_noWidgetGet_rayWidg -2
   /*no widget return for <attribute>Get_rayWidg
   `  and <x,y>GetCoord_rayWidg functions
   */

/*TODO: work on Mac parameters (need to test)*/
#define def_macRoundness_rayWidg 0.9f
#define def_macSegments_rayWidg 1000

#define def_maxStrLen_rayWidg ( (signed int) (((unsigned int) -1) - 1) )

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - font size and widgets states
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*errors*/
#define def_noValue_rayWidg -2
   /*could not apply operation to widget*/
#define def_noWidget_rayWidg -2
   /*could not find the widget*/


/*defaults*/
#define def_windowWidth_rayWidg 400
#define def_windowHeight_rayWidg 200
#define def_cursor_rayWidg '|'
#define def_blinkCursor_rayWidg ' '

#define def_fontSizeSI_rayWidg 18
#define def_charHeightSI_rayWidg 18
   /*18 is recomended size for large text in disablity
   `  accesibility guidelines
   `  Resizable is also suggested, but this might be more
   `    difficult for my GUI
   */
#define def_height_rayWidg ( def_charHeightSI_rayWidg + (def_charHeightSI_rayWidg >> 1) )
   /*vertical padding for text in widgets*/


/*states widgets can be in*/
#define def_press_rayWidg 1   /*mouse press event*/
#define def_checked_rayWidg 2 /*check box checked state*/
#define def_active_rayWidg 4  /*widget is being used*/
#define def_focus_rayWidg 8   /*widget is in focus*/
#define def_inactive_rayWidg 16
   /*wigdet is seen, but can not be interacted with*/
#define def_hiden_rayWidg 32 /*widget is not drawn*/
#define def_child_rayWidg 128 /*widget has a parent*/
   /*rayWidg only supports one level of child widgets
   `   the idea is that comples widgets that need children
   `   are hidden in functions, which can resolve them
   */

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - color scheme
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*OS states for light/dark mode*/
#define def_unkownMode_rayWidg 0
#define def_darkMode_rayWidg 1
#define def_lightMode_rayWidg 2

#define def_focusBorder_rayWidg 1
   /*how many pixels to offset focus border by*/

#ifdef MAC
   /*TODO: get offical Mac colors in here*/
   #define def_lightGrey_rayWidg 0xE1dFE1FF /*platinum*/
   #define def_darkGrey_rayWidg 0xC0BFC0FF  /*argent*/
   #define def_white_rayWidg 0xF5F5F5FF     /*cultured*/
   #define def_black_rayWidg 0x333436FF
#else
   /*TODO: find windows offical colors*/
   #define def_lightGrey_rayWidg 0xBEBEBEFF
   #define def_darkGrey_rayWidg 0x7E7E7EFF
   #define def_white_rayWidg 0xFFFFFFFF
   #define def_black_rayWidg 0x000000FF
#endif

/*-------------------------------------------------------\
| ST01: widg_rayWidg
|   - has widget ids, coordiantes, and dimensions
\-------------------------------------------------------*/
typedef struct widg_rayWidg
{
   /*for widget drawing*/
   signed int *xArySI;       /*x coordiantes;all widgets*/
   signed int *yArySI;       /*y coordiantes;all widgets*/
   signed int *widthArySI;   /*width of all widgets*/
   signed int *heightArySI;  /*height of all widgets*/

   /*for tiling auto-calculations of widgets*/
   signed int *rowArySI;     /*row for each widget*/
   signed int *colArySI;     /*column for each widget*/
   /*if row or col are set to -1, then tile not used*/


   unsigned char *stateAryUC;/*state each widget is in*/

   signed int lenSI;  /*number widgets stored*/
   signed int sizeSI; /*maximum widgets before realloc*/
   signed int focusSI;/*id (index) focus is set to*/


   /*font variables for GUI*/
   signed int fontSizeSI;   /*font size of text*/
   signed int charHeightSI; /*hieght of one character*/
   signed int charPadSI;    /*padding around character*/


   /*color scheme for the GUI*/

   signed int guiColSI;    /*background color of GUI*/
   signed int focusColSI;  /*color of focused border*/

   signed int textColSI;    /*primary text color*/
   signed int textAltColSI;
      /*alternate state (press) text color*/

   signed int butColSI;    /*color of a button*/
   signed int butBorderColSI; /*button border color*/
   signed int butPressColSI;
      /*color of button when pressed*/
   signed int butPressBorderColSI;
      /*color of button border when pressed*/

   signed int entryColSI; /*entry box color*/
   signed int entryBorderColSI; /*entry box border color*/
   signed int entryActiveColSI; /*active entry box color*/
   signed int entryActiveBorderColSI;
      /*active entry box border color*/
}widg_rayWidg;

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
|     o def_noValGet_rayWidg if widget does not have width
|       assigned
|       * in this case 0 is always the reall value
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
widthGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
);

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
|     o def_noValGet_rayWidg if widget has no height
|       * in this case 0 is always the reall value
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
heightGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
);

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
|     o def_noValGet_rayWidg if widget is not using the
|       tile system
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
rowGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
);

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
|     o def_noValGet_rayWidg if widget is not using the
|       tile system
|     o def_noWidget_rayWidg if widget does not exist
\-------------------------------------------------------*/
signed int
colGet_widg_rayWidg(
   signed int idSI,               /*id of widget*/
   struct widg_rayWidg *widgSTPtr /*has wiget*/
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
   struct widg_rayWidg *widgSTptr /*widgets to search*/
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif
