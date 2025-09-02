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

struct Font;
typedef struct Font (*fontFunction)(void);
   /*for reading embeded fonts*/

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
#define def_macRoundness_rayWidg 0.43f
#define def_macSegments_rayWidg 100

#define def_maxStrLen_rayWidg ( (signed int) (((unsigned int) -1) - 1) )
#define def_fontSize_rayWidg 20

#define def_maxFileLen_rayWidg 256
   /*maximum length for a file path*/
#ifdef WINDOWS
   #define def_pathSep_rayWidg '\\'
#else
   #define def_pathSep_rayWidg '/'
#endif

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
#define def_widgHeightGap_rayWidg 4
   /*divide height by to get y-axia gaps between widgets
   `   and padding
   */
#define def_cursor_rayWidg '|'
#define def_blinkCursor_rayWidg ' '

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
#define def_hog_rayWidg 256 /*widget is prioritiy*/
   /*means widget is a hog and trys to prevent you
   `  from interacting with other widgets in the GUI.
   `  The only exception are other hog widgets that are
   `  in a child state will allow interaction with other
   `  children with the hog state from the same parent
   */

/*these are for the get_event_rayWidg function return
`  values
*/
#define def_keyEvent_rayWidg 4
#define def_clickEvent_rayWidg 2
#define def_releaseEvent_rayWidg 16
#define def_scrollEvent_rayWidg 32

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - color scheme
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*OS states for light/dark mode*/
#define def_unkownMode_rayWidg 0
#define def_darkMode_rayWidg 1
#define def_lightMode_rayWidg 2

#define def_border_rayWidg 1
   /*how many pixels to offset a border by*/
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
typedef struct
widg_rayWidg{
   /*for widget drawing*/
   signed int *xArySI;       /*x coordiantes;all widgets*/
   signed int *yArySI;       /*y coordiantes;all widgets*/
   signed int *widthArySI;   /*width of all widgets*/
   signed int *heightArySI;  /*height of all widgets*/

   /*for tiling auto-calculations of widgets*/
   signed int *rowArySI;     /*row for each widget*/
   signed int *colArySI;     /*column for each widget*/
   /*if row or col are set to -1, then tile not used*/


   unsigned short *stateAryUS;/*state each widget is in*/

   signed int lenSI;  /*number widgets stored*/
   signed int sizeSI; /*maximum widgets before realloc*/
   signed int focusSI;/*id (index) focus is set to*/


   /*color scheme for the GUI*/

   signed int guiColSI;    /*background color of GUI*/
   signed int focusColSI;  /*color of focused border*/

   signed int textColSI;    /*primary text color*/
   signed int textAltColSI;
      /*alternate state (press) text color*/

   signed int widgColSI;  /*color of a widget*/
   signed int borderColSI;/*color of the border*/
   signed int pressColSI; /*color of widget when pressed*/
   signed int pressBorderColSI;
      /*color of wiget border when pressed*/

   signed int activeColSI; /*color of widget when active*/
   signed int activeTextColSI;
      /*color of text when active*/

   /*variables for GUI font type*/
   struct Font *fontSTPtr;    /*font for GUI*/
   signed char freeFontBl;
       /*1: safe to free font pointer*/
   signed char unloadFontBl;
       /*1: safe to unload the font*/
   float fontWidthF;          /*width of "D" character*/
   float fontHeightF;         /*height of "D" character*/
   signed int fontSizeSI;     /*size of font*/
   float spacingF;            /*spacing for font*/
}widg_rayWidg;

/*-------------------------------------------------------\
| ST02: event_rayWidg
|   o stores the mouse, keyboard, and scroll wheel events
\-------------------------------------------------------*/
typedef struct
event_rayWidg{
   signed int keySI;      /*keyboard event*/
   signed char shiftBl;   /*1: if shift key is pressed*/
   signed char altBl;     /*1: if alt key is pressed*/
   signed char ctrlBl;    /*1: if control key is pressed*/
   signed char superBl;   /*1: if super key is pressed*/
   signed char menuBl;    /*1: if menu key is pressed*/

   /*mouse events*/
   signed int xSI;        /*mouse x-coordinate*/
   signed int ySI;        /*mouse y-coordinate*/

   signed int parIdSI;    /*parent widget id mouse is on*/
   signed int childIdSI;  /*child widget id mouse is on*/

   signed char leftPressBl;   /*1: mouse was pressed*/
   signed char leftReleaseBl; /*1: mouse was released*/

   float scrollF;         /*how much scroll whell moved*/
}event_rayWidg;

/*-------------------------------------------------------\
| ST03: listBox_rayWidg
|   o structure to store values in a list box
|   o also grouped macros here
\-------------------------------------------------------*/

#define def_listHidden_rayWidg 1
   /*list box item is not shown*/
#define def_listCanSelect_rayWidg 2
   /*user can select this list box item; otherwise user
   `   can only move on this item
   */
#define def_listSpecial_rayWidg 4
   /*let user handel result*/
#define def_listSelect_rayWidg 8
   /*user selected this item*/

typedef struct
listBox_rayWidg{
   signed int maxWidthSI;    /*maximum width of box*/
   signed int minWidthSI;    /*miniumum width of box*/
   signed int lineWidthSI;   /*width of longest line*/

   signed int maxHeightSI;    /*maximum height of box*/
   signed int minHeightSI;    /*miniumum height of box*/

   signed char **textAryStr; /*text to print*/
   signed int *sizeArySI;  /*max bytes for each string*/
   signed char *stateArySC;/*index of each selected item*/
      /*state the item is in. States can be combined into
      `   multiple values
      `     0: nothing special and visiable
      `     def_listHidden_rayWidg: not shown
      `     def_listSpecial_rayWidg: not a normal entry,
      `       so if selected the user has to do something
      `         - ex in file browser is directory
      `     def_listSelect_rayWidg: user selected this
      `       item
      */
   signed int sizeSI;      /*max text pointers*/
   signed int lenSI;       /*number of strings*/
   signed int onSI;        /*entry on in list box*/
   signed int lastSelectSI;/*last item on*/
   signed int scrollSI;    /*position at in scroll*/

   signed int clickSI;     /*item clicked on; -1 if none*/

   signed int maxSelectSI; /*max items to select*/
   signed int numSelectSI; /*number items selected*/
}listBox_rayWidg;

/*-------------------------------------------------------\
| ST04: files_rayWidg
|   o structure with files listed in a file browser
|   o TODO: add support functions
\-------------------------------------------------------*/
typedef struct
files_rayWidg{
   signed char pwdStr[512]; /*present working directory*/
   struct listBox_rayWidg fileListST;/*has files*/
   struct listBox_rayWidg extListST; /*file extensions*/
}files_rayWidg;

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
);

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
);

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
|     o fontSizeF in widgSTPtr to be def_fontSize_rayWidg
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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
\-------------------------------------------------------*/
void
setFontSize_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   signed int fontSizeSI
);

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
\-------------------------------------------------------*/
void
setFontSpacing_widg_rayWidg(
   struct widg_rayWidg *widgSTPtr,
   float spacingF
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*-------------------------------------------------------\
| Fun066: focusCheck_widg_rayWidg
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
| Fun067: enterCheck_widg_rayWidg
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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*-------------------------------------------------------\
| Fun078: drawRec_rayWidg
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
drawRec_rayWidg(
   struct widg_rayWidg *widgSTPtr, /*has coordinates*/
   signed int idSI,          /*id of widget to draw*/
   signed char typeSC,       /*type of rectangle*/
   unsigned int colorHexUI,  /*hex color of rectangle*/
   unsigned int borderHexUI, /*hex color of border*/
   unsigned int focusHexUI   /*hex color of focus border*/
);

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
);

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
);

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
|     o -2 if idSI did not match the event id
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
);

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
);

/*-------------------------------------------------------\
| Fun084: mkMesgBox_rayWidg
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
|     o id (index) of the message box widget
|     o -1 for memory errors
\-------------------------------------------------------*/
signed int
mkMesgBox_rayWidg(
   struct widg_rayWidg *widgSTPtr
);

/*-------------------------------------------------------\
| Fun085: mesgBoxDraw_rayWidg
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
|     o def_noWidg_rayWidg if widget does not exit
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
);

/*-------------------------------------------------------\
| Fun086: mesgBoxEvent_rayWidg
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
);

/*-------------------------------------------------------\
| Fun086: checkExt_rayWidg
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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

/*-------------------------------------------------------\
| Fun103: selectGet_listBox_rayWidg
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
);

/*-------------------------------------------------------\
| Fun104: specialGet_listBox_rayWidg
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
);

/*-------------------------------------------------------\
| Fun106: canSelectGet_listBox_rayWidg
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
);

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
);

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
);

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
);

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
);

#endif
