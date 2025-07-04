/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' widgets SOF: Start Of File
'   - widgest using with raylib for freezeTB
'   o header:
'     - defined variables and guards
'   o fun01: blank_st_widgets
'     - blanks a st_widgets struct, but does not blank
'       the tree pointers
'   o fun02: init_st_widgets
'     - initializes a st_widgets struct
'   o fun03: freeStack_st_widgets
'     - frees variables (except par) in a st_widgets
'       struct and returns the next struture in the list
'   o fun04: freeHeap_st_widgets
'     - frees a single st_widgets struct and returns the
'       next st_widgets structure in the list
'   o fun0x: addChild_st_widgets
'     - adds a child widget to a st_widgets struct
'   o fun14: getDarkModeState_widgets
'     - detects if Mac or Windows computer is in dark mode
'   o fun15: checkGuiColorMode_widgets
'     - checks is user is using dark or light mode and
'       sets color scheme to the correct mode
'   o fun16: addWidget_st_widgets
'     - add a new widget to an st_widgets struct
'   o fun17: changeState_widgets
'     - change the state of a widget in root_widgets
'   o fun10: drawBut_widgets
'     - draw a button
'   o fun19: checkIfClick_widgets
'     - finds if a widget was clicked and returns the
'       widget id the mouse was on
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

#ifndef WIDGETS_RAYLIB_H
#define WIDGETS_RAYLIB_H

struct str_ptrAry;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - font size and widgets states
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

static signed int glob_fontSizeSI_widgets = 15;
static signed int glob_charHeightSI_widgets = 15;

/*defaults*/

#define def_windowWidth_widgets 400
#define def_windowHeight_widgets 200

#define def_maxWidgetWidth_widgets 240
#define def_height_widgets ( glob_charHeightSI_widgets + (glob_charHeightSI_widgets >> 1) )

/*states widgets can be in*/
#define def_press_widgets 1
#define def_release_widgets 2
#define def_inactive_widgets 4
#define def_hiden_widgets 8
#define def_normal_widgets 16

/*mouse buttons*/
#define def_left_widgets 128
#define def_right_widgets 256
#define def_wheel_widgets 512


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - color scheme
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_unkownMode_widgets 0
#define def_darkMode_widgets 1
#define def_lightMode_widgets 2

#ifdeF MAC
   /*TODO: get offical Mac colors in here*/
   #define def_lightGrey_widgets 0xE1dFE1FF /*platinum*/
   #define def_darkGrey_widgets 0xC0BFC0FF  /*argent*/
   #define def_white_widgets 0xF5F5F5FF     /*cultured*/
   #define def_black_widgets 0x333436FF
#else
   /*TODO: find windows offical colors*/
   #define def_lightGrey_widgets 0xBEBEBEFF
   #define def_darkGrey_widgets 0x7E7E7EFF
   #define def_white_widgets 0xFFFFFFFF
   #define def_black_widgets 0x000000FF
#endif

/*these are the default colors, I am using a variable
`   because I will change them if user is using dark or
`   light mode
*/
static
   signed int glob_textColSI_widgets = def_black_widgets;
static
   signed int glob_textAltColSI_widgets =
      def_lightGrey_widgets;
      /*test color for states like a button press*/
static
   signed int glob_guiColSI_widgets = def_white_widgets;
static
   signed int glob_butColSI_widgets=def_lightGrey_widgets;
static
   signed int glob_butBorderColSI_widgets =
      def_darkGrey_widgets;
static
   signed int glob_butPressColSI_widgets =
      def_darkGrey_widgets;
static
   signed int glob_butPressBorderColSI_widgets =
      def_lightGrey_widgets;
static
   signed int glob_entryColSI_widgets =
      def_lightGrey_widgets;
static
   signed int glob_entryBorderColSI_widgets =
      def_darkGrey_widgets;

/*-------------------------------------------------------\
| ST01: st_widgets
|   - holds coordinates and states of all widgets
|   - total; 101 bytes per widget; so very inefficent
\-------------------------------------------------------*/
typedef struct st_widgets
{
   /*general input: 21 bytes*/
   signed int idSI;     /*number assigned to widget*/
   signed int xSI;      /*all wigets x coordiantes*/
   signed int ySI;      /*all widgets y coordiantes*/
   signed int widthSI;  /*widths of all widgets*/
   signed int heightSI; /*heights of all widgets*/
   signed char stateSC; /*state fo all widgets*/

   /*click events to respond to: 3 bytes
   `  these are a list of possible events, for right
   `  and left there are two possible events, a press
   `  or a release. For wheel, it is scrolling only
   */
   signed char leftEventSC;
   signed char rightEventSC;
   signed char wheelBl;

   /*grid settings (do not mess with): 25 bytes*/
   signed int *colWidthArySI; /*width of columns in grid*/
   signed int *colHeightArySI; /*height of grid columns*/
   signed int colLenSI;        /*number columns in grid*/
   signed int colSizeSI;       /*size of arrays*/
   signed int rowNumSI;        /*number of rows in grid*/
   signed char gridBl;         /*1: is in grid mode*/

   /*hold values of widgets: 28 bytes*/
   struct str_ptrAry *strAryST; /*strings in this widget*/
   signed int *arySI;           /*array of signed ints*/
      /*this allows list boxes to know what was selected*/
   signed int aryLenSI;         /*bytes in array*/
   signed int arySizeSI;         /*bytes in array*/
   signed int indexSI;          /*string/int index at*/

   /*tree pointers: 24 bytes*/

   /*these pointers deal with widgets that are in this
   `   widget (children) or the widget this widget is
   `   assigned to (parent)
   */
   struct st_widgets *par;  /*parent node, if in widgets*/
   struct st_widgets *child;/*children in widgets*/

   /*these pointers deal with moving into widgets sharing
   `   the same parent, but not part of this widget
   */
   struct st_widgets *next;  /*move to next widget*/
}st_widgets;

/*-------------------------------------------------------\
| ST02: root_widgets
|   - root of the widgets tree
\-------------------------------------------------------*/
typedef struct root_widgets
{
   signed int xSI; /*x coordinate*/
   signed int ySI; /*y coordinate*/
   signed int widthSI; /*width of window*/
   signed int heightSI; /*height of window*/

   struct st_widgets *widgets;/*list of seen widgets*/
   struct st_widgets *hidden; /*all hiden all widgets*/
   struct st_widgets *idAryST;/*array ordered by widget
                              `  id of all widgets
                              */
   signed int *openIdsArySI;/*array with free ids that
                            `  are under lenSI
                            */
   signed int openIdsLeftSI; /*number of open ids left*/
   signed int lenSI;       /*number ids in idAryST*/
   signed int sizeSI;      /*size of openIdsSI and array*/
} /*root_widgets*/

/*-------------------------------------------------------\
| Fun01: blank_st_widgets
|   - blanks array in a st_widgets struct
| Input:
|   - widgetsSTPtr:
|     o st_widgets struct pointer with arrays to blank
| Output:
|   - Modifies:
|     o arrays to be filled with 0's (if present)
|     o lenSI to be 0
\-------------------------------------------------------*/
void
blank_st_widgets(
   struct st_widgets *widgetsSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_st_widgets
|   - initializes a st_widgets struct
| Input:
|   - widgetsSTPtr:
|     o st_widgets struct pointer to initialize
| Output:
|   - Modifies:
|     o all array pointers to be null (0)
|     o lenSI and size SI to be 0
\-------------------------------------------------------*/
void
init_st_widgets(
   struct st_widgets *widgetsSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_st_widgets
|   - frees a single st_widgets struct and returns the
|     next st_widgets structure in the list
| Input:
|   - widgetsSTPtr:
|     o st_widgets struct pointer to free
| Output:
|   - Modifies:
|     o frees widgetSTPtr, but you must set to 0/null
|   - Returns:
|     o next widget in the list
| Warning:
|   - does not free par pointers
\-------------------------------------------------------*/
struct st_wigdets *
freeHeap_st_widgets(
   struct st_widgets *widgetsSTPtr
);

/*-------------------------------------------------------\
| Fun0x: addChild_st_widgets
|   - adds a child widget to a st_widgets struct
| Input:
|   - childSTPtr:
|     o st_widgets struct pointer to add
|   - parIdSI:
|     o id of parent to add child to
|   - rootSTPtr:
|     o root_widges struct pointer to add child to
| Output:
|   - Modifies:
|     o idSI in childSTPtr to have new id
|     o next in parent to point to the new child
|     o idAryST in rootSTPtr to include the new child
|   - Returns:
|     o id assigned to the new child struct
|     o -1 if child is already in rootSTPtr; idSI not -1
|       * also if childSTPtr has a next node (was in a
|         tree at some point)
|     o -2 if parent not in tree
|     o -4 for memory errors
| Warning:
|   - does not free par pointers
\-------------------------------------------------------*/
struct st_wigdets *
addChild_st_widgets(
   struct st_widgets *childSTPtr, /*child to add*/
   signed int parIdSI,            /*parent id*/
   struct root_widgets *rootSTPtr
);

/*-------------------------------------------------------\
| Fun14: getDarkModeState_widgets
|   - detects if Mac or Windows computer is in dark mode
| Input:
| Output:
|   - Returns:
|     o def_darkMode_widgets for dark mode
|     o def_lightMode_widgets for light mode
|     o def_unkownMode_widgets if could not decide
| Note:
|   - this is from github co-pilot (chat gpt)
\-------------------------------------------------------*/
signed char
getDarkModeState_widgets(
   void
);

/*-------------------------------------------------------\
| Fun15: checkGuiColorMode_widgets
|   - checks is user is using dark or light mode, and
|     sets the color scheme to the correct mode
| Input:
| Output:
|   - Modifies:
|     o if user has dark mode on, then
|       glob_textColSI_widgets, glob_guiColSI_widgets,
|       glob_butColSI_widgets, glob_entryColSI_widgets to
|       is set to the dark mode colors
|     o if user has light mode on, then
|       glob_textColSI_widgets, glob_guiColSI_widgets,
|       glob_butColSI_widgets, glob_entryColSI_widgets to
|       is set to the light mode colors
\-------------------------------------------------------*/
signed char
checkGuiColorMode_widgets(
   void
);

/*-------------------------------------------------------\
| Fun17: changeState_widgets
|   - change the state of a widget in root_widgets struct
| Input:
|   - stateSC:
|     o new state to change to
|       * def_normal_widgets for normal widget state
|       * def_press_widgets if mouse is clicking on
|       * def_inactive_widgets if widget is in inactive
|         (non-usable, but visiable) state
|       * def_hiden_widgets if wigdet is not shown and can
|         not be interacted with
|   - idSI:
|     o id (index) of widget to change state for
|   - rootSTPtr:
|     o root_widgets struct pointer to root of widget list
| Output:
|   - Returns:
|     o 0 for no errors
|     o -1 state was invalid
|     o -2 if id (index) was invalid
\-------------------------------------------------------*/
signed char
changeState_widgets(
   signed char stateSC, /*new state to assing*/
   signed int idSI,  /*index of widget*/
   struct root_widgets *rootSTPtr /*has wigdet to change*/
);

/*-------------------------------------------------------\
| Fun10: drawBut_widgets
|   - draw a button
| Input:
|   - maxWidthSI:
|     o maximum width of button
|   - xSI:
|     o x coordinate
|   - ySI:
|     o y coordinate
|   - stateSC:
|     o state button is in
|       * def_normal_widgets for normal
|       * def_press_widgets for press
|       * def_inactive_widgets for inactive
|   - textStr:
|     o c-string with text to print on button
|     o the maximum string length is 240 by ray lib
|       width, which is around 12 to 18 items at font
|       size 15
| Output:
|    - Returns:
|      o width of button
\-------------------------------------------------------*/
signed int
drawBut_widgets(
   signed int maxWidthSI, /*maximum width of button*/
   signed int xSI,        /*x coordinate of button*/
   signed int ySI,        /*y coordinate of button*/
   signed char stateSC,   /*state button is in*/
   signed char *textStr   /*text to print on button*/
);

/*-------------------------------------------------------\
| Fun19: checkIfClick_widgets
|   - finds if a widget was clicked and returns the widget
|     id the mouse was on
| Input:
|   - rootSTPtr:
|     o root_widgets struct with widgets to check
|   - scrollFPtr:
|     o float pointer to get scroll percent of mouse
|       wheel
|   - leftClickSCPtr:
|     o signed char pointer to get return state
| Output:
|   - Modifies:
|     o scrollFPtr to have percentage of scroll wheel
|       moved
|     o leftClickSCPtr to have any valid left click events
|       for the returned widget
|       * 0 if no click event
|       * def_press_widgets for left click event
|         widget supports left clicks
|       * def_release_widgets for left release event
|         widget supports left release events
|     o rightClickSCPtr to have any valid right click
|       events for the returned widget
|       * def_press_widgets for right click event and
|         widget supports right clicks
|       * def_release_widgets for right release event
|         widget supports right release events
|   - Returns:
|     o widget id mouse was on
|     o -1 if mouse was not on a
\-------------------------------------------------------*/
signed short
checkIfClick_widgets(
   struct root_widgets *rootSTPtr, /*widgets to check*/
   float *scrollFPtr,              /*% mouse wheel moved*/
   signed short *leftClickSCPtr,   /*gets left event*/
   signed short *rightClickSCPtr   /*gets right event*/
);

#endif
