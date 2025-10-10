## widg\_rayWidg

The primary structure is the widg\_rayWidg struct. It
  keeps track of the widget coordinates and states each
  widget is in. You can get or set these variables by
  querying it with funcitions or by direct access.

Variables:

1. xArySI: array wigh each widgets x coordiantes
  - in tileing mode these will be updated by function
    calls
2. yArySI: array with each wdigets y coordiantes
  - in tileing mode these will be updated by function
    calls
3. widthArySI: array of the width of each widget
  - some functions (such as button and entry box) will
    auto set these values
4. heightArySI: array of the height of each widget
  - some functions (such as button and entry box) will
    auto set these values
5. rowArySI: the row number if using tileing format
  - this is < 0 for non-tile widgets
  - you need to call tile\_widg\_rayWidg to get tile
    widgets x and y coordaintes
6. colArySI: the column number if using tileing format
  - this is < 0 for non-tile widgets
  - you need to call tile\_widg\_rayWidg to get tile
    widgets x and y coordaintes
7. stateAryUS: state(s) the widget is in, this can affect
   if a widget is drawn and its coloring
   - pressed state means it is being pressed by a key or
     by the mosue
     - defition is: def\_press\_rayWidg
   - checked state means it is a widget that can be
     checked and that it has been checked
     - defintion is: def\_checked\_rayWidg
   - active state means the widget is being activly used
     - defintion is: def\_active\_rayWidg
   - focus state means the widget is currently in focus
     - defintion is: def\_focus\_rayWidg
   - inactive state means the widget is seen, but can not
     be interacted with
     - defintion is: def\_inactive\_rayWidg
   - hiden state means the widget can not be seen, nor
     interacted with
     - defintion is: def\_hiden\_rayWidg
   - child state means the widget is part of a more
     complex widget
     - defintion is: def\_child\_rayWidg
   - hog state means the widget will not let other widgets
     respond to user events or be in focus
     - defintion is: def\_hog\_rayWidg
8. lenSI is the number of widgets that have been added
9. sizeSI is the maximum number of widgets you can add
    before reszing (calling realloc\_widg\_rayWidg)
10. xScaleF is the current scaling of the GUI
    - there are several other variables that relate to
      window scale and size
11. fontSizeSI is the current size of the font being
    used
    - if you change this, then call
      `measureFont_widg_rayWidg()` to set the correct
      values for the new font size
    - `measureFont_widg_rayWidg(&widg_rayWidg_struct);`
12. spacingF is the current font spacing (default is 1)
    being used
    - if you change this, then call
      `measureFont_widg_rayWidg()` to set the correct
      values for the new font size
    - `measureFont_widg_rayWidg(&widg_rayWidg_struct);`
13. guiColSI: background color of the GUI
14. textColSI: color of text
15. textAltColSI: color of text for the alternate color
    scheme
15. widgColSI: color of a widget (normal)
16. borderColSI: color of the widget border
17. activeColSI: color of the widget when it is in focus
18. activeColSI: color of the widget when it is in focus

## event\_rayWidg

The event\_rayWidg structure holds the data needed to
  manage a single user event. This includes key presses,
  left mouse clicks, and scroll wheel movements.

The event\_rayWidg struct is designed to get input using
  the get\_event\_rayWidg function.

- Variables:
  1. keySI: is the key pressed by the user
     - from GetKeyPressed() function from rayWidg
  2. shiftBl: is set to 1 if a shift key was pressed
     - set to 0 if no shift or caps lock and shift
     - set to 1 if left shift pressed
     - set to 2 if right shift pressed
     - caps lock is not checked because raylib never could
       detect when it was turned off
  3. altBl: is set if alt key is pressed
     - set to 0 if alt key not pressed
     - set to 1 if left alt pressed
     - set to 2 if right alt pressed
  4. ctrlBl: is set if ctrl key is pressed
     - set to 0 if ctrl key not pressed
     - set to 1 if left ctrl pressed
     - set to 2 if right ctrl pressed
  5. superBl: is set if ctrl key is pressed
     - set to 0 if super key not pressed
     - set to 1 if left super pressed
     - set to 2 if right super pressed
  6. menuBl: is set if ctrl key is pressed
     - set to 0 if menu key not pressed
     - set to 1 if left menu pressed
     - set to 2 if right menu pressed
  7. xSI: has the mouses x-coordinate or -1 if none
  8. ySI: has the mouses y-coordinate or -1 if none
  9. parIdSI: has the id of the parent of selected widget
     - widget id if there is no parent; widget not child
  10. childIdSI: has id of the widget or -1 if widget is
      is not a child widget
  11. leftPressBl: is 1 if left moust button was pressed
  12. leftReleaseBl: is 1 if left moust button was
       released
  13. scrollF: has how much scroll wheel moved by or is
      0 for no movement

## listBox\_rayWidg

The listBox\_rayWidg structure holds the variables used
  to run a list box widget.

- Variables:
  1. maxWidthSI: maximum width of list box
  2. minWidthSI: minimum width of list box
  3. lineWidthSI: length in pixels of the longest line
     - this is auto updated with the add function
  4. maxHeightSI: maximum height of list box
  5. minHeightSI: minimum height of list box
  6. textAryStr: c-string array with contents of list box
     - there may be more items then shown to user
  7. stateArySC: what state each item is in
     - hidden: not shown to user
     - can select: user can select this item
     - special: tells the list box event function to
                notify you when this item is clicked or
                enter hit
       - can still apply even when it does not have the
          can select state
     - select: list box item has been selected
  8. lenSI: number of c-strings in the array
  9. onSI: item in list box the user is currently on
  10. lastSelectSI: the last item that the user selected
  11. scrollSI: first item shown in list box
  12. sideScrollSI: horizontal scroll position in list box
  13. maxSelectSI: maximum number of items the user can
                   select
  14. numSelectSI: number of items the user has selected

## files\_rayWidg

This structure is used with the file browser structure. It
  is used to hold the data needed to mange the file
  browser widget.

- Variables:
  1. pwdStr: c-string with the present working directory
     - maximum limit is 504 characters
  2. showHidenSC:
     - if set to 1; shows hidden files on Unix
     - if set to 0; does not show hidden files on Unix
     - Windows, will not work, since I think Windows hides
       hidden in the file attributes, Unix uses a `.` at
       the start
  3. fileListST: listBox\_rayWidg structure with the files
     in the present working directory (pwdStr / varible 1)
  4. extListST: listBox\_rayWidg structure with the file
     extensions
  5. lastWidgSI: widget was on before the file browser
     was opened (set by `fileBrowserDraw_rayWidg()`)

# Orginization

## Coordiantes

rayWidg uses two coordinate systems. The first is the x,y
  coordiante system used by raylib. Were x = 0 means the
  left edge of the GUI and y = 0 means the top of the GUI.
  In all cases these are global coordinates. Both x and y
  refer to the left top coner of the widget you wish to
  draw.

You also have width and height, which control how big the
  widget will be. In some cases these can be auto set for
  you, while in others you will have to provide the width
  and height.

```
x,y
 +----------------+~i
 :                : | height of box
 :                : |
 +----------------+_]
 |________________|
    Width of box
```

The tileing sytem uses the width and height from the x,y
  system, but instead of you providing the x,y coordiante,
  your provide the column number and row number. The
  tile\_widg\_rayWidg function will then auto find the x,y
  coordiantes using the width, height, row number, and
  column number.

In tileing, the distance betweens rows is found from the
  maximum height in one row. While the width of one column
  is found by the max width in the column.

```
      column_1   column_2 column_3
      +------+   +------+ +--------+
row_1 |      |   |      | |        |
      +------+   +------+ |        |
                          +--------+
      +--------+ +----+   +--------+
row_2 |        | |    |   |        |
      +--------+ +----+   +--------+
      +--------+ +------+ +--------+
row_3 |        | |      | |        |
      +--------+ +------+ +--------+
```

Tile coordiantes are considered local and so, in widgets
  with children (see states (next) section), the child
  tile coordiantes are always based on the x,y coordinate
  of the parent widget.

The x,y coordinate system is global and children x,y
  coordiantes are only shifted by the amount the parent
  widet was moved.

One reasond to prefere tiling over x,y coordinates is that
  tiling positions will scale with changes in screen size.
  You can get this same effect with x,y coordinates using
  the width and height values in the `widg_rayWidg`
  structure, but you need to put in the work.

## States

### States overview

The rayWidg library uses four states to organize widgets.
  These are the inactive state, hiden state, the, child
  state, and the hog state.

### hidden state

The hiden state means that the widget is not present or
  part of the current GUI. All drawing functions and event
  functions will ignore the widget. This can be used for
  menu bars or alternative GUI windows.

### inactive state

The inactive state means that the widget is present, but
  that it should be ignored. All drawing functions will
  draw the widget in the inactive color scheme and event
  functions will ignore the widget.

### child state

The child state means that the widget is part of a widget
  that is made up of multiple widgets (a complex widget),
  such as a list box or file browser. The rayWidg system
  only allows one level of nesting. This forces the
  complex widget to use functions to do its task. In some
  cases, such as the file browser, those functions are
  pre-made. For your own custom widgets you will have to
  make them.

You can think of a complex widget as a mini GUI.

A complex widget consists of two parts. A parent widget,
  which as the x,y coordiante and the final width and
  size of the widget. The other part is the child widgets,
  which do tasks. These child widgets can have x,y
  coordiantes (global) that are adjusted as the parent
  widget is updated, or use tileing coordiantes that are
  auto adjusted with tile_widg_rayWidg.

When the parent widget has a state change for the
  inactive and hiden states, then all the child widgets
  are also changed to the same state. However, you can
  alter the state of individual child widgets, so long
  as the parent state is not changed.

Each child is assigned to its parent by its id (index in
  the arrays). Child widgets next to each other all share
  the same parent, which is the first parent that the
  child with the lowest id is next to.

In the example beneath, the complex widget's parent is
  the first parent.

```
parent_1 child_1 child_2 parent_2
|______________________| 
   complex widget
```

In the second example we have two complex widgets.

```
parent_1 child_1 child_2 parent_2 child_3
|______________________| |______________|
   complex widget 1      complex widget 2
```

### hog state

The hog state means that a widget has priority over other
  widgets when it is not in a hidden state. When a hog
  state widget is visable the tab focus and click events
  will only fire for that widget. If the widget is a
  child of a parent (complex) widget, then the tab focus
  and click events work on other hog state child widgets
  in the same parent.

This state is used for message box's and file browsers.

# Defined variables

These are variables that can influence the default
  settings of a raywidg GUI.

## Default variables

- window size variables:
  - def\_scaleWidth\_rayWidg: is the default screen type
    expected. By default, raywidg will scale up but not
    down
  - def\_winWidth\_rayWidg: default width of the window
    - this is set for the smallest smartphone screen
      possible
  - def\_winHeight\_rayWidg: default height of the window
    - this is set for the smallest smartphone screen
      possible
  - def\_FPS_rayWidg: frames per second (60)
- tiling variables
  - def\_widgHeightGap\_rayWidg: controlls the y-axis
    (vertical) gap between widgts and the padding around
    text in widgets
    - for gap: font\_height / def\_widgHeightGap\_rayWidg
    - for padding:
      font\_height / def\_widgHeightGap\_rayWidg
   - def_getPad_rayWidg: gets height padding for to top
     or bottom of a widget (uses widgHeightGap)
     - you input the font height
   - def_getTotalPad_rayWidg: gets padding for both the
     top and bottom of a widget (uses getPad)
     - you input the font height
- font and entry box variables
  - def\_fontSize\_rayWidg: default font size to use (20)
  - def\_fontSpacing\_rayWidg: default font spacing to use
  - def\_cursor\_rayWidg: ascii character printed for
    cursor in entry boxes
  - def\_blinkCursor\_rayWidg: ascii chacter printed for
    when the cursor is blinked
  - def\_cursorBlinkInterval\_rayWidg: number of frames
    needed to do a complete blink of the cursor
  - def\_maxStrLen\_rayWidg: maximum length of string
    allowed
- file variables
  - def\_pathSep\_rayWidg: symbol used to separate file
    paths (chagnes for windows)
  - def\_maxFileLen\_rayWidg: maximum length of string
    allowed for a file name
- widet variables
  - def\_border\_rayWidg: number of pixels to offset the
    border around a shape by
  - def\_focusBorder\_rayWidg: number of pixels to offset
    the focus border by
- color variables
  - raywidg is designed around a two color theme, whith
    a slight change for when the GUI is in focus
  - darkmode variables:
    - def\_backFocusDarkCol\_rayWidg: is the background
      color when the GUI is in dark mode and in focus
    - def\_forFocusDarkCol\_rayWidg: is the forground
      color when the GUI is in dark mode and in focus
    - def\_backDarkCol\_rayWidg: is the background color
      when the GUI is in dark mode and not in focus
    - def\_forDarkCol\_rayWidg: is the forground color
      when the GUI is in dark mode and not in focus
  - lightmode variables:
    - def\_backFocusLightCol\_rayWidg: is the background
      color when the GUI is in light mode and in focus
    - def\_forFocusLightCol\_rayWidg: is the forground
      color when the GUI is in light mode and in focus
    - def\_backLightCol\_rayWidg: is the background color
      when the GUI is in light mode and not in focus
    - def\_forLightCol\_rayWidg: is the forground color
      when the GUI is in light mode and not in focus
- variables unique to Mac
  - def\_macRoundness\_rayWidg: controlls how round the
    rectangles are on a Mac or when `-DMAC` is used during
    compile time (not very good, but kinda works)
  - def\_macSegments\_rayWidg: segments is used in drawing
    rouned rectangles in raylib, no idea what does
    - only appies to a Mac or when `-DMAC` is used during
      compile time

## State variables

As a rule you should use the functions for changing or
  accessing these. However, you can also check or modify
  states with their pre-defined variables. Each variable
  targets a separate bit, so one widget could in theory
  have all states set at once.

The state array is an unsigned short, so a total of 16
  flags are possible. Currenly only 9 flags are used.

- def\_press\_rayWidg: is the pressed state
  - button is pressed
- def\_checked\_rayWidg: is the checked state
  - check box or radio button (not used) is checked
- def\_active\_rayWidg: is the active state
  - entery box or similar active widget is being used
- def\_focus\_rayWidg: is the focus state
  - current widget in focus/working with
- def\_inactive\_rayWidg: is the inactive state
  - widget is does not take user input, ingore in focus
    states
- def\_hiden\_rayWidg: is the hiden state
  - widget is hidden and should be ignored
- def\_child\_rayWidg: is the child state
  - is a child widget in a complex widget
- def\_hog\_rayWidg: is the hog state
  - widget is a high priority widget, nothing else can
    be interacted with

# initial setups in a GUI

## get a gui initialized

Inorder to setup a GUI you first need to make a rayWidg
  structure, initialize it, then do the set it up step
  were memory is allocated and the GUI is started. You
  can then run the raylib event loop to check when the
  window should close.

At the end you should always call `CloseWindow();` from
  raylib.

```
#include <raylib.h>
#include "raywidg.h"

int
main(
){
   signed int errorSI = 0;
   struct rayWidg widgStackST;
   
   init_widg_rayWidg(&widgStackST);
   if(
      setup_widg_rayWidg(
         &widgStackST,
         (signed char *) "title",
         1
      )
   ) goto memoryErr_main_sec0x;
   
   while( ! WindowShouldClose() )
      /*do something*/

   CloseWindow();
   
   memoryErr_main_sec0x:;
      errorSI = 1;
      fprintf(stderr, "memory error\n");
      goto ret_main_sec0x;
   
   ret_main_sec0x:;
      freeStack_widg_rayWidg(&widgStackST);
      return errorSI;
} /*main*/
```

The input for the `setup_widg_rayWidg()` function is the
  `widg_rayWidg` structure to intialize, the tite of the
  GUI and a number (0 to 3).

- Number values:
  - 0 means do not scale the GUI
  - 1 means adjust the GUI font and tileing system when
    the user is using a higher resolution screen then
    expected
  - 2 means scale down font and tiling for smaller screens
    - as a rule avoid this
  - 3 does both 1 (scale up for HDPI) and 2 (scale down
    for smaller screens)
    - as a rule, plan on the smallest screen possible

## check events and build draw function

# change fonts and raylib emmbeding fonts

This is a piont were raywidg is a bit weak. It is desinged
  to work with a single font and font size, which makes it
  simpiler to handel. This section covers how to make
  an emmbeded font in raylib and then change the default
  font to the emmbeded font or a font from a file.

I also mention how to use the emmbeded font in raylib.

Also, warning. Not all the font functions have been tesed.
  They are simple enough were should work though.

## get embeded font (raylib)

I found little documentation on how to emmbed fonts in
  raylib, but plently on how to load them. It turned out
  to be an easy task, but took some work to figure out.
  There is a redit page that does mention the needed
  function, but there is no example given.

You can emmbed fonts in raylib or load them from files.
  For emmbeding, you first load the font with
  `Font fontST = LoadFont("/path/to/font.ext")`. Then you
  output the emmbed file
  with `ExportFontAsCode(fontST, "<fontFileName>.h")`.

For loading an emmbeded font, use the load function at the
  bottom of the emmbed fonts file (`<fontFileName.h>`),
  then use the font structure in the DrawTextEx function.

Here is an example of emmbeding and then loading an
  emmbed font in raylib (not raywidg).

```
#include "/path/to/raylib.h"

int
main(
){
   Font fontST = LoadFont("/path/to/font.ext");
   ExportFontAsCode(fontST, "<fontFileName>.h");
   UnloadFont(fontST);
   return 0;
} /*main*/
```

At the bottom of `"<fontFileName>.h"` you will find font
  loading function. It will be named
  `static Font LoadFont_<fontFileName>(void)`. This
  function is used to load the font into raylib.

```
#include "/path/to/raylib.h";
#include "<fontFileName>.h";

int
main(
){
   Font newFontST;

   InitWindow(<width>, <height>, "<title>");
      /*always initialize the window before loading fonts
      `  (from file or emmbeded) first, otherwise raylibs
      `  default font will overwrite the loaded font
      */
   SetTargetFPS(60);
   newFontST = LoadFont_<fontFileName>();

   while(! WindowShouldClose() )
   { /*Loop: draw GUI*/
      BeginDrawing();
        DrawTextEx(
           newFontST,     /*font to use*/
           "hello world", /*message to print*/
           (Vector2) {10, 100},/*message x, y coordinate*/
           float 1,       /*spacing (no idea what does)
           BLACK          /*use the black font color*/
        );
      EndDrawing();
   } /*Loop: draw GUI*/

   /*do not use UnloadFont(newFontST);, raylib points
   `  Font arrays to the static structures in the Font
   `  file
   */
   CloseWindow();
} /*main*/
```

## raywidg font system

### overview raywidg fonts

For raywidg, you can only have one font and one font size
  in the GUI. This is do to me not wanting to keep track
  of different fonts and sizes in the widgets. Also, I
  find that my GUI's often need only one font size.

You can cheat the system if needed by changing the font
  size or font type with functions (not suggested
  directly) when drawing a widget, however, be aware that
  this will mess up the tiling systems calculations. If
  you do this, plan on either having these change between
  windows (so previous window is hidden) or using X,Y
  coordinates.

The default font is IBM Plex Mono. I find raylibs default
  font to be a bit ugly and not very legibal.

Again, rayWidg is not supper usefull. It was designed to
  be something I could peice together in a shorter period
  of time.

### using raylib embeded fonts

For rayWidg, you can change the default font to an embeded
  font using the loading an emmbeded font
  (`useEmbedFont_widg_rayWidg`) function. The embed font
  must have been made by raylib's `ExportFontAsCode()`
  function.

To change the default font to an embed font:

```
#include "rayWidg.h"
#include "embededFont.h"

int
main(
){
   struct widg_rayWidg widgStackStruct;
   init_widg_rayWidg(&widgStackStruct);

   InitWindow(400, 200, "newGui");
   SetTargetFPS(60);

   if( setup_widg_rayWidg(&widgStackStruct) )
      /*deal with memory error*/
   
   if(
      useEmbedFont_widg_rayWidg(
         &widgStackStruct,
         LoadFont_embededFont(), /*function to load font*/
         20 /*font size*/
      )
   ) /*deal with memory error*/

   /*code goes here*/

   freeStack_widg_rayWidg(&widgStackStruct);
   CloseWindow();
} /*main*/
```

### loading fonts from file

You can set the raywidg font by load a font from a file.
  The function to do this is `changeFont_widg_rayWidg()`.

```
#include "rayWidg.h"
#include "embededFont.h"

int
main(
){
   struct widg_rayWidg widgStackStruct;
   init_widg_rayWidg(&widgStackStruct);

   InitWindow(400, 200, "newGui");
   SetTargetFPS(60);

   if( setup_widg_rayWidg(&widgStackStruct) )
      /*deal with memory error*/
   
   if(
      changeFont_widg_rayWidg(
         widgStruct,
         (signed char *) "<fontFileName>.<ext>",
         20 /*font size*/
      )
   ) /*deal with memory error*/

   /*code goes here*/

   freeStack_widg_rayWidg(&widgStackStruct);
   CloseWindow();
} /*main*/
```

### loading font from raylib Font struct

You can also load a font from a raylib Font structure.
  Though, you should be warned that this is a shallow
  copy and only the pointer address is copied. So, do
  not free the input Font structure untill you are done
  with the font. Also, do not edit the values in
  fontSTPtr, as they will change the values in the
  input Font struct.

To do this use the `swapFont_widg_rayWidg()` function. The
  input is the `widg_rayWidg` structure pointer to do
  the swap on, the Font structure pointer to use, and the
  new font size.

```
#include <raylib.h>
#include "rayWidg.h"

int
main(
){
   Font newFont;
   struct widg_rayWidg widgStackStruct;
   init_widg_rayWidg(&widgStackStruct);

   InitWindow(400, 200, "newGui");
   SetTargetFPS(60);

   if( setup_widg_rayWidg(&widgStackStruct) )
      /*deal with memory error*/
   newFont = LoadFont("<fontFileName>.otg");
   
   swapFont_widg_rayWidg(widgStruct, &newFont, 20);

   /*code goes here*/

   UnloadFont(newFont);
   freeStack_widg_rayWidg(&widgStackStruct);
   CloseWindow();
} /*main*/
```

### changing the font size

You can change the font size with
  the `setFontSize_widg_rayWidg()` function. It is better
  to use the function since the font height and width
  are also changed (to size of letter `D`).

```
#include <raylib.h>
#include "rayWidg.h"

int
main(
){
   Font newFont;
   struct widg_rayWidg widgStackStruct;
   init_widg_rayWidg(&widgStackStruct);

   InitWindow(400, 200, "newGui");
   SetTargetFPS(60);

   if( setup_widg_rayWidg(&widgStackStruct) )
      /*deal with memory error*/
   
   /*change the font size to 32 (default is 20)*/
   setFontSize_widg_rayWidg(widgStruct, 32);

   /*code goes here*/

   freeStack_widg_rayWidg(&widgStackStruct);
   CloseWindow();
} /*main*/
```

### changing the font spacing

You can change the font spacing (default is 1) with
  the `setFontSpacing_widg_rayWidg()` function. It is
  better to use the function since the font height and
  width are also changed (to size of letter `D`).

```
#include <raylib.h>
#include "rayWidg.h"

int
main(
){
   Font newFont;
   struct widg_rayWidg widgStackStruct;
   init_widg_rayWidg(&widgStackStruct);

   InitWindow(400, 200, "newGui");
   SetTargetFPS(60);

   if( setup_widg_rayWidg(&widgStackStruct) )
      /*deal with memory error*/
   
   /*change the font size to 32 (default is 1)*/
   setFontSpacing_widg_rayWidg(widgStruct, 2.0f);

   /*code goes here*/

   freeStack_widg_rayWidg(&widgStackStruct);
   CloseWindow();
} /*main*/
```

# Flow

The steps in using rayWidg are:

1. initialize the raylib GUI
2. create, initialize, and setup a widg_rayWidg struture
3. add simple widgets to widg_rayWidg strcuter
4. draw gui, use BeginDrawing, ClearBackground, and
   EndDrawing from raylib and the widget functions from
   rayWidg
   - for tileing you will also have to call widget
     functions
5. the complex widgets
6. get user events and udate and draw GUI
7. free memory and structuers

## 1. initialize the raylib GUI

Before using rayWidg you should initialize the raylib
  GUI first. This prevents raylib from overriding rayWidgs
  default font. All of these functions are from raylib.

```
InitWindow(<width>, <height>, <title>);
SetTargetFPS(60); /*60 frames per second*/
```

```
InitWindow(400, 200, "a GUI");
SetTargetFPS(60); /*60 frames per second*/
```

## 2. create, initialize, and setup

You can initialize (set everything to 0/null) a
  widg\_rayWidg structer with init\_widg\_rayWidg(). The
  input is a pionter to a widg\_rayWidg structer to
  initialize.

```
struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);
```

After initialization you needt to setup a widg\_rayWidg
  structure after initialization with
  `setup_widg_rayWidg()`. The input is the `widg_rayWidg`
  structure to setup. The return value is 0 for success
  and 1 for memory errors.

Currently this only affects the font.

```
struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);
if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/
```

## 3. add simple widgets

You can add simple widgets with the
  addWidget\_widg\_rayWidg function. The only exceptions
  are for complex widgets, which as a rule require
  dedicated functions to set them up.

Input:

- x coordinate or column to assign of widget to
- 7 coordinate or row to assign of widget to
- 1: if using tile coordinates (column/row) for the widget
  - else use 0
- width of the widget or -1 to auto assign
  - you must provided a non-negative width for the entry
    box
- hieght of the widget or -1 to auto assign
- widg\_rayWidg structure pointer to add widget to

The output is the id (index) assigned to the widget or
  -1 for mememory errors.

```
signed int idOneSI = 0;
signed int idTwoSI = 0;
struct widg_rayWidg widgetsStackST;

init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

idTwoSI =
   addWidget_widg_rayWidg(
      0,   /*first row*/
      1,   /*second column*/
      1,   /*using tile coordainates*/
      100, /*assign width manually*/
      50,  /*assign height manually*/
      &widgetStackST
   );

if(idTwoSI < 0)
   /*memory error*/
```


## 4. draw gui

You will have to start your GUI drawing with the
  BeginDrawing, ClearBackground, and EndDrawing functions
  from raylib.h. You can then draw your widgets with the
  few widget functions in from rayWidg.

For tileing, you will use the widget functions to return
  the dimensions of the widget. Then call the tile
  function to get the x,y coordinates for each widget,
  then call the widget drawing function to draw your
  widget.

For non-tileling or tileing you will also call these
  functions to draw your widgets. In this case you should
  always start with `BeginDrawing()`
  and `ClearBackgound()`. You should end
  with `EndDrawing()`.

- Functions:
  - non-widget (by coorinate) drawing functions: these
    are not used in any tiling steps
    - textDrawByCoord\_rayWidg: draws some text to the
      screen based on input x,y coordinates
      - this is not a widget, but a drawing function
        (really a wrapper)
    - drawRectByCoord\_rayWidg: draws a rectangle by x,y
      coordinates
      - this is not a widget, but a drawing function
        (really a wrapper)
  - widget functions: these can be tiled or the x,y
    coordinates are used in the tileing step
    - drawRect\_rayWidg: draws a widget as a rectangle
    - butDraw\_rayWidg: draws a button and/or updates
      height and width of a button widget
    - entryDraw\_rayWidg: draws an entry box and/or
      updates height of entry box
    - labDraw\_rayWidg: draws a lable and/or updates
      height and width of label

### non-widgets; drawing text

The `textDrawByCoord_rayWidg()` will draw a string of text
  at the input coordinates. It will also shorten the
  output text if needed.

- Input:
  1. c-string with text to draw
  2. x coordinate
  3. y coordinate
  4. maximum width in pixels for the text before
     shortening
  5. color (as unsigned int in hex format [0xFFFFFFFF]) to
     draw
  6. value of 0 to 1 to tell if padding the text
     - 0 no padding
     - 1 pad the width (x, start and end)
     - 2 pad the height (y, top and bottom)
     - pad width and height
  7. widg\_rayWidg structure pointer with text variables
- Output:
  - Returns: width of drawn text

### non-widgets; drawing rectangle

The `rectDrawByCoord_rayWidg()` will draw a rectangle
  at the input coordinates.

- Input:
  1. x coordinate
  2. y coordinate
  3. width of rectangle to draw
  4. height of rectangle to draw
  5. number telling if rectangle has a border
     - 0 no border
     - 1 regular border
     - 2 focus border
     - 3 both regular and focus border
     - 4 do not draw the rectangle
     - 5 only draw the regular border (no rectangle)
     - 6 only draw the focus border (no rectangle)
     - 7 draw focuse and regular border, but no rectangle
  6. color of rectangle (as hex code)
  7. color of regular border (as hex code)
  8. color of focus border (as hex code)
  5. color (as unsigned int in hex format [0xFFFFFFFF]) to
     draw

### tile widgets

The tileing system uses tile\_widg\_rayWidg to find the
  x,y coordinates for the tiled widgets. You will need to
  call the drawing functions first or set the width and
  height manually.

Input:

- widg\_rayWidg structure with tiled widgets to update
- the number of pixels to offset each row by
  - spacing between rows, use -1 to do the height of one
    character (def\_height\_rayWidg)
- the number of pixels to offset each column by
  - spacing between columns, use -1 to do number of pixels
    in one letter ('a')

The output is 0 for no errors and 1 for memeory errors.
  Look at the buttons, entry, and other widgets tile
  examples for tile\_widg\_rayWidg examples.

### rectangles

The most basic widget, that makes up most other widgets is
  the rectangle. You can draw a rectangle
  with `drawRect_rayWidg()`.

For Macs or with `-DMAC` it will draw a rectangle with
  rounded corners.

- Input:
  - `widg_rayWidg` structure pointer with x, y, width,
    and height of the widget
  - id (index) of the widget in the `widg_rayWidg`
    structure
  - number to tell if drawing a rectangle or a rectanle
    with borders
    - 0: draw rectangle only
    - 1: draw rectangle with a border (offset is
         def\_border\_rayWidg)
    - 2: draw rectangle with a focus (2nd) border (offset
         is def\_focusBorder\_rayWidg)
    - 3: draw rectangle with border and focus border
  - rectangles color (as hex code)
  - borders color (as hex code)
  - focus borders color (as hex code)
- Returns:
  - 0 if the widget is hidden
  - def\_noWidget\_rayWidg if the widget id is out of
    bounds (no widget)
  - width of widget (`widgSTPtr->widthArySI[id]`)
    - this is not changed

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      100,/*width is 100 pixels*/
      25, /*heigth is 100 pixels*/
      &widgetStackST
   );

/*tile_widg_rayWidg(&widgetStackST, -1, -1);*/

if(idOneSI < 0)
   /*memory error*/

BeginDrawing();
   ClearBackground();
   recDraw_rayWidg(
      widgSTPtr,
      idOneSI,
      3, /*rectangle with both borders*/
      0x858585FF, /*make the rectangle grey*/
      0xFFFFFFFF, /*white for border (inner border)*/
      0x000000FF  /*black for focus (outer) border*/
   );
   /*ignoring the return value, because I already know
   `   the rectangle exists
   */
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

Note: `drawRec_rayWidg()` does nothing for tiling, because
  you need to know the width and height before hand. To
  tile, set the height, width, row, and column. Then,
  call the tile function to get coordiantes.

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      0, /*row on*/
      0, /*column on*/
      1,  /*using tile coordainates*/
      100,/*width is 100 pixels*/
      25, /*heigth is 100 pixels*/
      &widgetStackST
   );

tile_widg_rayWidg(&widgetStackST, -1, -1);

if(idOneSI < 0)
   /*memory error*/

BeginDrawing();
   ClearBackground();
   recDraw_rayWidg(
      widgSTPtr,
      idOneSI,
      3, /*rectangle with both borders*/
      0x858585FF, /*make the rectangle grey*/
      0xFFFFFFFF, /*white for border (inner border)*/
      0x000000FF  /*black for focus (outer) border*/
   );
   /*ignoring the return value, because I already know
   `   the rectangle exists
   */
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

### buttons

You can draw or get dimensions for a button widget by
  using butDraw\_rayWidg().

Input:

- maximum width of the button
  - if drawn text is to long, then shorten drawn text
- minimum width of the button
  - does not do padding (TODO add padding)
  - 0 is changed to one character
- id of widget that represents the button
- c-string with text to draw on the button
- 1: if you only want to update width and height of the
  widget
  - 0: if you also want to draw the button
- widg\_rayWidg structure pointer with widget to draw as
  a button

Return value:

- width of the button for succes,
- 0 if the widget is hidden (not drawable)
- def\_noWidget\_rayWidg if the widget does not exist

This will also modify the height and width values.

**non-tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

BeginDrawing();
   ClearBackground();
   butDraw_rayWidg(
      120,      /*maximum width*/
      30,       /*minium width*/
      idOneSI, /*id of widget to make into a button*/
      (signed char *) "this is a button",
      0,       /*want to draw the button*/
      &widgetStackST
   );
   /*ignoring the return value, because I already know
   `   the button exists
   */
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

**tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      0, /*row on*/
      0, /*column on*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

butDraw_rayWidg(
   120,      /*maximum width*/
   30,       /*minium width*/
   idOneSI, /*id of widget to make into a button*/
   (signed char *) "this is a button",
   1,       /*set dimensions of button, but do not draw*/
   &widgetStackST
);

/*tileing using default offsets (-1)*/
tile_widg_rayWidg(&widgetStackST, -1, -1);

BeginDrawing();
   ClearBackground();
   butDraw_rayWidg(
      120,      /*maximum width*/
      30,       /*minium width*/
      idOneSI, /*id of widget to make into a button*/
      (signed char *) "this is a button",
      0,       /*want to draw the button*/
      &widgetStackST
   );
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

### Entry box's

You can draw or get dimensions for an entry box widget by
  using entryDraw\_rayWidg(). An entry box displays a line
  of text.

Input:

- width of the entry box
  - if drawn text is to long, then scrolling is applied
  - the function sets a 256 character limit
- id of widget that represents the entry box
- two element signed int array
  - element one (index 0) has the first character in
    entry box
  - element two (index 1) has character the cursor is
    after
- 1: print cursor as a space (blink the cursor)
  0: print the cursor (`|`)
- c-string with text to draw print
- 1: if you only want to update width and height of the
  widget
  - 0: if you also want to draw the entry box
- widg\_rayWidg structure pointer with widget to draw as
  an entry box

Return value:

- width of the entry box for success
- 0 if the widget is hidden (not drawable)
- def\_noWidget\_rayWidg if the widget does not exist

This will also modify the height and width values.

**text manipulation functions for entry box:**

There are also support functions for processing the text
  that goes into an entry box.

- intStrCheck\_rayWidg will modifiy the input text to be
  an integer.
  - it will also remove characters until it is less then
    the maximum value
  - first character is the cursor, next are the characters
    at the end
- floatStrCheck\_rayWidg intStrCheck\_rayWidg, but for
  floats instead of integers
- fileStrCheck\_rayWidg check if file/path has valid
  characters, if not modifiy.
  - spaces and tabs are converted to underscores `_`
  - the allowed characters are `A-Z`, `a-z`, `0-9`, `-`,
    and `_`
  - this should only be used for saving files

All three functions take in three values, the text to
  modify (as c-string), the cursor position (or -1 for
  none), and the maximum value. For ints and floats the
  maximum is the largest number possible, for files it
  is the longest path allowed.

The return value for all three functions is the length
  of the c-string, even if the c-string was not modified.


**non-tileling example**

```
signed int idOneSI = 0;
signed int entryPosition[2];

signed char *buffStr[128];
signed int buffLenSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

/*start draw at 1st character & cursor at 1st character*/
entryPosition[0] = 0;
entryPosition[1] = 0;
buffStr[0] = '\0';

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

BeginDrawing();
   ClearBackground();
   entryDraw_rayWidg(
      60,        /*width*/
      idOneSI,   /*id of widget to make into entry box*/
      enteryPosition,
      0,         /*do not blink*/
      buffStr, /*user input text*/
      0,         /*want to draw the entry box*/
      &widgetStackST
   );
EndDrawing();

/*check if user added a new character*/
buffStr[buffLenSI] = GetCharPressed();

if(buffStr[buffLenSI] > 31 || buffStr[buffLenSI] < 127)
{ /*If: got a character*/
   ++bufLenSI;
   buffStr[bufLenSI] = '\0';
   ++entryPosition[1]; /*move cursor one character*/

   buffLenSI =
      fileStrCheck_rayWidg(
          textStr,            /*user input*/
          entryPositionSI[1], /*cusor position*/
          64               /*maximum file name length*/
      ); /*making sure is valid file name*/
} /*If: got a character*/

freeStack_widg_rayWidg(&widgStackST);
```

**tileling example**

```
signed char blinkBl = 0;
signed int idOneSI = 0;
signed int entryPosition[2];

signed char *buffStr[128];
signed int buffLenSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

/*start draw at 1st character & cursor at 1st character*/
entryPosition[0] = 0;
entryPosition[1] = 0;
buffStr[0] = '\0';

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );
if(idOneSI < 0)
   /*memory error*/


entryDraw_rayWidg(
   60,        /*width*/
   idOneSI,   /*id of widget to make into entry box*/
   enteryPosition,
   0,         /*do not blink*/
   buffStr, /*user input text*/
   1,         /*want to only update entry box dimensions*/
   &widgetStackST
); /*get initial dimesions for tileling*/

/*tileing using y (row) of 20 and x (column) of 15*/
tile_widg_rayWidg(&widgetStackST, 20, 15);


BeginDrawing();
   ClearBackground();
   entryDraw_rayWidg(
      60,        /*width*/
      idOneSI,   /*id of widget to make into entry box*/
      enteryPosition,
      blinkBl,   /*tells if blinking cursor*/
      buffStr, /*user input text*/
      0,         /*want to draw the entry box*/
      &widgetStackST
   );
EndDrawing();


/*check if user added a new character*/
blinkBl != blinkBl; /*so can blink the cursor*/
   /*this would only be used in a loop*/

buffStr[buffLenSI] = GetCharPressed();

if(buffStr[buffLenSI] > 31 || buffStr[buffLenSI] < 127)
{ /*If: got a character*/
   ++bufLenSI;
   buffStr[bufLenSI] = '\0';
   ++entryPosition[1]; /*move cursor one character*/

   buffLenSI =
      fileStrCheck_rayWidg(
          textStr,            /*user input*/
          entryPositionSI[1], /*cusor position*/
          64               /*maximum file name length*/
      ); /*making sure is valid file name*/
} /*If: got a character*/

freeStack_widg_rayWidg(&widgStackST);
```

### Entry box add character

The last example with an entry box added the characters
  in manually. However, if you prefer a more automated
  approach, you can used the `entryEvent_rayWidg()`
  function to check if the event was your entry box widget
  and handel the event.

- Input:
  1. id of the entry box you want to check
  2. two element signed int array
     - element one (index 0) has the first character in
       entry box
     - element two (index 1) has character the cursor is
       after
  3. c-string with the current text in the entry box
  4. current c-string length
  5. maximum allowed length for the c-string (if running
     a check function)
  6. a function to check if the c-string is valid
     - returns a signed int
     - input is:
       1. c-string (signed char \*)
       2. signed int
       3. signed int
  7. event\_rayWidg structure with the event to run
  8. widg\_rayWidg structure with the entry box widget
- Output:
  1. length of c-string
  2. -1 if no event
  3. -2 if the id did not match the entry box or the entry
     box was not active

**Example**

```
#include <stdio.h>
#include <raylib.h>
#include "rayWidg.h"

/*this is used to pass paramaters to the draw function*/
typedef struct
GUI{
   signed int entryWidthSI;      /*width of entry box*/
   signed int entryIdSI;         /*id of entry box*/
   signed int positionArySI[2];
      /*scroll and cursor position in entry box*/

   signed char buffStr[135];     /*text in entry box*/
   signed int buffLenSI;       /*number characters input*/
   struct widg_rayWidg *widgets; /*has widgets*/
}

/*this function is here to draw the GUI*/
void
drawFun(
   void *inVoid
){
   struct GUI in = (struct GUI) *inVoid;

   BeginDrawing();
      ClearBackground();

      entryDraw_rayWidg(
         in->entryWidthSI,  /*width*/
         in->entryIdSI,     /*id of entry box*/
         in->positionArySI, /*cursor/scroll position*/
         0,                 /*do not blink*/
         in->buffStr,       /*user input text*/
         0,                 /*want to draw the entry box*/
         in->widgets        /*has widgets*/
      );
   EndDrawing();
}

int
main(
){
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01: variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct GUI inGUI;
   struct widg_rayWidg widgetsStackST;
   struct event_rayWidg eventStackST;

   signed int errorSI = 0;
   signed char ranGuiBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02: initialize and setup widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*initialize the GUI*/
   InitWindow(200 /*height*/, 400 /*width*/, "a_gui");
   SetTargetFPS(60);

   /*initialize  my structures*/
   init_widg_rayWidg(&widgetsStackST);
   init_event_rayWidg(&eventStackST);

   /*setup the input GUI structure*/
   inGUI.widgets = &widgetsStackST;
   inGUI.positionArySI[0] = 0;
   inGUI.positionArySI[1] = 0;
   inGUI.buffStr[0] = '\0';
   inGUI.buffLenSI = 0;

   if( setup_widg_rayWidg(&widgStackST) );
      goto memoryErr_main;

   /*add my entry box to the GUI*/
   inGUI.entryIdSI =
      addWidget_widg_rayWidg(
         20, /*x coordinate is 20*/
         20, /*y coordinate is 20*/
         0,  /*using x,y not tile coordainates*/
         -1, /*auto assign width*/
         -1, /*auto assing height*/
         inGUI.widgets
      );

   if(inGUI.entryIdSI < 0)
      goto memoryErr_main;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03: run GUI
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(! WindowShouldClose() )
   { /*Loop: run the GUI*/

      errorSI =
         get_event_rayWidg(
            1,       /*have tab switching widgets*/
            drawFun, /*drawing function to call*/
            &inGUI,  /*passed to drawing function*/
            &eventStackST,
            inGUI->widgets
         );
         /*errorSI has the event type. In this case there
         `   is no need to check it
         */

      inGUI.buffLenSI =
         entryEvent_rayWidg(
            inGUI.entryIdSI, /*id of entry box*/
            inGUI.positionArySI, /*position in entry box*/
            inGUI.buffStr,  /*user input text*/
            inGUI.buffLenSI,/*current input length*/
            127,            /*limit to 127 characters*/
            fileStrCheck_rayWidg, /*check function*/      
               /*this is the function to check if the
               `   input is a valid file name character
               */
            &eventStackST,
            inGUI->widgets
         ); /*if entry box not used, this is ignored*/
   } /*Loop: run the GUI*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04: clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   printf("you input: %s\n", inGUI.buffStr);
   errorSI = 0;
   goto return_main;

   memoryErr_main:;
      fprintf(stderr, "memory error\n");
      errorSI = 1;
      goto return_main;

   return_main:;
      CloseWindow(); /*opened a window*/
      freeStack_widg_rayWidg(&widgStackST);
      freeStack_event_rayWidg(&eventStackST);

      return errorSI;
```

### label

You can draw a label (text) or get dimensions for a label
  using labDraw\_rayWidg().

Input:

- maximum width of the label
  - the function sets a 256 character limit
- minimum width of the label
- id of widget that represents the label
- c-string with text to draw print
- the character to pad strings that are under the minium
  length with
- a flag telling how to pad the string if it is beneath
  the minimum width
  - 0: doe not pad
  - 1: add padding to left of string
  - 2: add padding to right of string
  - 3: add padding to both sides
- 1: if you only want to update width and height of the
  widget
  - 0: if you also want to draw the label
- widg\_rayWidg structure pointer with widget to draw as
  a label

Return value:

- width of the label for success
- 0 if the widget is hidden (not drawable)
- def\_noWidget\_rayWidg if the widget does not exist


**non-tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

BeginDrawing();
   ClearBackground();
   labDraw_rayWidg(
      120,      /*maximum width*/
      30,       /*minium width*/
      idOneSI,  /*id of widget to make into a label*/
      (signed char *) "this is a label",
      ' ',      /*pad with spaces*/
      0,        /*do not pad*/
      0,        /*want to draw the label*/
      &widgetStackST
   );
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

**tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

idOneSI =
   addWidget_widg_rayWidg(
      20, /*x coordinate is 20*/
      20, /*y coordinate is 20*/
      0,  /*using x,y not tile coordainates*/
      -1, /*auto assign width*/
      -1, /*auto assing height*/
      &widgetStackST
   );

if(idOneSI < 0)
   /*memory error*/

labDraw_rayWidg(
   120,      /*maximum width*/
   30,       /*minium width*/
   idOneSI,  /*id of widget to make into a label*/
   (signed char *) "this is a label",
   ' ',      /*pad with spaces*/
   2,        /*right pad*/
   1,        /*do not draw the label*/
   &widgetStackST
);

/*tileing using default offsets (-1)*/
tile_widg_rayWidg(&widgetStackST, -1, -1);

BeginDrawing();
   ClearBackground();
   labDraw_rayWidg(
      120,      /*maximum width*/
      30,       /*minium width*/
      idOneSI,  /*id of widget to make into a label*/
      (signed char *) "this is a label",
      ' ',      /*pad with spaces*/
      2,        /*right pad*/
      0,        /*want to draw the label*/
      &widgetStackST
   );
EndDrawing();

freeStack_widg_rayWidg(&widgStackST);
```

## 5. complex widgets

Complex widgets are widgets that can not be drawn using
  a single widget entry. The consist of a parent widget,
  which has the widget dimensions and child widgets, which
  are the buttons, lables, and other parts of the widget.

### message boxes (complex widgets)

A mess box consists of four widgets; the parent rectangle,
  the top border, the label, and a button. The use is
  to print a single, short, one line message to the user
  about an error or some other message. For rayWidg you
  only need to keep track of the parent's id.

```
     parent (widget 0)
         VVVVVV
+--------------------------+
| child 1 (to border)      |
|--------------------------|
|                          |
| child 2 message for user |
|                          |
|    +----------------+    |
|    | child 3 button |    |
|    +----------------+    |
+--------------------------+
```

The message box button has the hog priority, so when
  visible, the user is unable to interact with the rest
  of the GUI.

When created the message box is set to the hidden state.
  You then add the message in the draw command. Any events
  that map to the message box are then processed by the
  message box event function. This will set the message
  box to the inivisible state if it is no longer needed.

Because the message box is a hog and takes over the
  GUI it is not designed to be a tiling widget. It is
  drawn over the other tiling widgets close to the
  center of the GUI.

The `mkMesgBox_rayWidg()` function is used to make the
  message box widget. You only need to call this once if
  you plan to show one message box at a time. The input
  is the `widg_rayWidg` structure to hold the message
  box widget. The output is the id assigned to the parent
  widget of the message box. A -1 is returned for memory
  errors.

When the message box is needed, you can then reveal the
  message box by removeing the hidden
  state (`hidenClear_widg_rayWidg()`) and then draw the
  message box with the `mesgBoxDraw_rayWidg()` function.
  This draws the message box with your message to the
  screen.

- Input:
  - parent widget ID of the message box
    - return from `mkMesgBox_rayWidg`
  - width in pixels of the GUI
  - height in pixels of the GUI
  - c-string with the message to print out
    - can not be longer then 128 characters, and it is
      reccommend to have it be a short sentence
    - must be a single line
  - c-string with text to print on the button
  - `widg_rayWidg` structure with message box
- Output:
  - width of message box if a message box was drawn
  - 0 if message box was hidden (not drawn)
  - def\_noWidg\_rayWidg if no widget was in the message
    box
  - -1 if the message is to long

If youget the message box parent id 
  from `getMouseWidg_widg_rayWidg()`
  or `enterCheck_widg_rayWidg` you can then check if the
  message box button was the target widget with
  the `mesgBoxEvent_rayWidg()` function. This function
  will set the press state of the button (if pressed) and
  will also set the message box to the hidden state if
  the button was released.

- Input:
  - the event you passing in
    - 0 if you are clearing press events
    - 1 if there was a press (mouse click or enter) event
    - 1 if there was a release (mouse click or enter)
      event
  - the id of message box parent widget
    - the return value from `mkMesgBox_rayWidg()`, or
      the signed int pionter (parSIPtr) (3rd input) set
      by `getMouseWidg_widg_rayWidg()`
      and `enterCheck_widg_rayWidg()`
  - the id of the child widget clicked/pressed
    - return of the `getMouseWidg_widg_rayWidg()`
      and `enterCheck_widg_rayWidg()` functions
  - `widg_rayWidg` structure pointer with the message
    box widget
      
This is a poor example, but shows how a message box
  might be used.

```
signed int mesgBoxIdSI = 0;
signed int keySI = 0;
signed int parIdSI = 0;
signed int idSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

/*initialize GUI for raylib*/

if( setup_widg_rayWidg(&widgStackST) );
   /*deal with memory errors*/

/*make other widgets*/

mesgBoxIdSI = mkMesgBox_rayWidg(&widgStackST);

if(mesgBoxIdSi < 0)
   /*memory error*/

/*set the message box to a non-hiden state*/
hidenClear_widg_rayWidg(mesgBoxIdSI, &widgStackST);

/*this is in your event loop*/
BeginDrawing();
   ClearBackground();

   /*draw your other widgets here*/

   mesgBoxEvent_rayWidg(
      0,           /*0 here is the clear command*/
      mesgBoxIdSI,
      mesgBoxIdSI, /*ok since not targeting a child*/
      &widgStackST
   ); /*clear the message box of press events*/

   if(
      mesgBoxDraw_rayWidg(
         mesgBoxIdSI,
         400,  /*GUI was created to be 400 pixels wide*/
         200,  /*GUI was made 200 pixels high*/
         (signed char *) "hello world!",
         (signed char *) "Press this button",
         &widgStackST
      ) < 0
   ) /*message was to long*/
EndDrawing();

keySI = GetKeyPressed();

idSI =
   enterCheck_widg_rayWidg(
      keySI,
      0, /*key wad from GetKeyPressed*/
      &parIdSI, /*gets parent id or -1 if not child*/
      &widgStackST
   );

if(parIdSI >= 0 && parIdSI == mesgBoxIdSI)
{ /*If: user selected a message box*/
   mesgBoxEvent_rayWidg(
      2,    /*treaing as release*/
      parIdSI,
      idSI,  /*child widget clicked*/
      &widgStackST
   ); /*returns 1 if message box was closed, not worring
      `  about that here since I would do nothing
      */
} /*If: user selected a message box*/

else
{ /*Else: need to check if click event*/
   idSI =
      getMouseWidg_widg_rayWidg(
         GetMouseX(),
         GetMouseY(),
         &parIdSI,
         &widgStackST
      );
   
   if(parIdSI >= 0 && parIdSI == mesgBoxIdSI)
   { /*If: user selected a message box*/
      if( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) )
         keySI = 2;
      else if( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) )
         keySI = 1;
   
      mesgBoxEvent_rayWidg(
         keySI, /*has if press or release event*/
         parIdSI,
         idSI,  /*child widget clicked*/
         &widgStackST
      ); /*returns 1 if message box was closed, not worring
         `  about that here since I would do nothing
         */
   } /*If: user selected a message box*/
} /*Else: need to check if click event*/

freeStack_widg_rayWidg(&widgStackST);
```

## 6. get user events and udate and draw GUI

You can get users events using the user event functions.
  There are two kinds, the lower level functions cover a
  specific event, such as the enter key, tab, or a mouse
  event. The higher level function uses
  the `event_rayWidg` to store the event and handles the
  lower level events. It will even redraw the GUI for you
  one key pressed if you input your drawing function.

Note: when using enter to press buttons, you can hold down
  the press by detecting if the enter key `KEY_ENTER` is
  held down. However, you need to redraw the GUI to update
  if the enter key has been released. Otherwise, you get
  an infinite loop.

```
signed int parentWidgetSI = 0;

indexSI =
   enterCheck_widg_rayWidg(
      keySI,
      0,
      &parentWidgetSI,
      guiSTPtr->widgSTPtr
   );
while( IsKeyDown(KEY_ENTER) )
   draw_function(guiSTPtr);
   /*redraw GUI to detect when enter is released*/
pressClear_widg_rayWidg(indeSI, guiSTPtr->widgSTPtr);

switch(indexSI)
{ /*Switch: detect which button was pressed*/
   case button1:
      /*code goes here*/
      break;
   case button2:
      /*code goes here*/
      break;
} /*Switch: detect which button was pressed*/
```

### higher; get events

The `get_event_rayWidg()` function will get one user
  input event and store it in an event\_rayWidg structure.

- Input:
  1. 1 to have this function handle tabs (change focus
     on tab input)
  2. function to draw your GUI with
     - is `void (*<name>(void *))`
     - use 0/null to not use
  3. void pointer to pass to the draw function
  4. event\_rayWidg structure pointer to store events in
  5. widg\_rayWidg structure pointer with widgets
- Output:
  1. 0 if nothing happened
  2. def\_keyEvent\_rayWidg if a key was input
  3. def\_keyEvent\_rayWidg | def\_releaseEvent\_rayWidg
     if enter was released (only if draw function input)
  4. def\_keyEvent\_rayWidg | def\_focus\_rayWidg tab key
     was pressed (only if draw input 1 is set to 1)
  5. def\_clickEvent\_rayWidg | def\_press\_rayWidg if
     left mouse button was pressed down
  6. def\_clickEvent\_rayWidg | def\_rleaseEvent\_rayWidg
     if left mouse button was released

For an example of a simple use case, please see the
  entry box function. Also for a list of output not listed
  here, please see the `event_rayWidg` structure section.

You can get check if was a child widget using
   `event_rayWidg->childIdSI`. If it is 0 or greater the
   widget was a child widget.

You can get the parent widget
  using `event_rayWidg->parIdSI`. If it is 0 or greater
  the widget was a child widget.

You can check for release events using
  using `event_rayWidg->leftReleaseBl`. If it is 1, then
  the left mouse button was released.

You can get the input key using `event_rayWidg->keySI`.
  If it is 0, then no key was input. The key was taken
  with `GetKeyPressed()` from raylib, so you will need to
  convert it before use.

### list boxes

The list box occupies the space of one widget. The event
  handeling for the list box is managed by a function,
  which needs a listBox\_rayWidg structure.

The list box events has several hotkeys that it uses.

- Mouse events:
  - left release: select one item in the list and remove
    all other selected items
  - left release + control: add one item in the list to
    your selection
    - if the item is already selected it is removed
  - left release + shift: add a range of items to your
    list, up to the maximum number of files
    - only applies to last selected item, so if the last
      item was removed from the selection, this will not
      work
  - move scroll wheel forward: move up one item
  - move scroll wheel down: move down one item
- Keyboard events
  - up or k: move up one item in list
  - shift+up, shift+K, or page up: move up one page
  - down or j: move down one item in list
  - shift+down, shift+J, or page down: move down one page
  - home or g: move to top of list box
  - end or shift+G: move to end of list box
  - enter: select item and clear rest or if is a special
    item (ex directory), return negative index (special
    item)
  - space: select one item and clear reast
  - space+control: add one item to selection or if already
    selected, remove it
    - limited to max number of items
  - space+shift: select all items between last selected
    item and current item
    - only applies to last selected item, so if the last
      item was removed from the selection, this will not
      work
  - control+a: select all items (if possible, otherwise
    first items until hit maximum limit)
  - control+r: deselect all items

#### make a list box

You can make a list box widget using
  the `mk_listBox_rayWidg()` function.

- Input:
  1. x coordinate (or column) of list box
  2. y coordinate (or row) of list box
  3. if doing tiling or strict x,y
     - 1: treat input 1 and 2 as tile coordinates
     - 0: treat input 1 and 2 as x,y coordinates
  4. widg\_rayWidg struct pointer to hold the list box
- Output:
  - Returns: id (index) of list box
  - Returns: -1 for memory errors

After that you will need a listBox\_rayWidg structure to
  interact with the list box. It will have to be
  initialized and have items added to it.

You can initialize the listbox using
  the `init_listBox_rayWidg()` function. The input is a
  pointer to your listBox\_rayWidg structure.

After initialization you can add items to the list box
  with the `addItem_listBox_rayWidg()` function.

- Input:
  1. c-string to add to list box
  2. state (hidden, can select, special, selected) to add
     to the item
     - 0: goes to the can select state
     - you can change this later
  3. listBox\_rayWidg structure pointer to add input 1 to
  4. widg\_rayWidg structure pointer
     - has font, so used to measure size of added string
- Output:
  - Returns: 0 for no errors
  - Returns: 1 for memory errors

**Example**

```
struct widg_rayWidg widgetsStackST;
struct listBox_rayWidg listBoxStackST;

init_widg_rayWidg(&widgetsStackST);
init_listBox_rayWidg(&listBoxStackST);

if( setup_widg_rayWidg(&widgetsStackST) )
   /*deal with  memory errors*/

mk_listbox_rayWidg(20, 100, 0, &widgetsStackST);
  /*20 is x, 100 is y, 0 for no tile*/

/*add item to the list box, repeat for more items*/
if(
   addItem_listBox_rayWidg(
      (signed char *) "some text to add",
      0, /*set to can select state*/
      &listBoxStackST,
      &widgetsStackST
   )
) /*deal with memory error*/

freeStack_widg_rayWidg(&widgetsStackST);
freeStack_listBox_rayWidg(&listBoxStackST);
```

#### list box drawing and events

You can draw the list box in your GUI using
  the `draw_listBox_rayWidg()` function.

- Input:
  1. id (index) of list box in widgets
  2. decide if drawing or just getting dimensions
     - 1: only get dimensions of the list box
       - for tileing
     - 0: draw the list box
  3. listBox\_rayWidg structure pointer with list box
     items to draw
  4. widg\_rayWidg structure pointer with list box
     dimensions
- Output:
  - Returns: width of list box for succes
  - Returns: 0 if list box is hidden
  - Returns: def\_noWidg\_rayWidg if widget does not exist

You can check/run events for the list box using
  the `listBoxEvent_rayWidg()` function. This is needed
  because of all the complexity with the list box.

- Input:
  1. id (index) of list box in widgets
     - this is used to verify that the list box is input,
     - it allows you to call the event function without
       checking in an if statement
  2. listBox\_rayWidg structure pointer with list box
     items and other related stats
  3. event\_rayWidg structure with the event to check/run
  4. widg\_rayWidg structure with the widgets
- Output:
  - Returns: -2 if input widget is not in widgets list
  - Returns: -1 if the id in the event\_rayWidg struct
             (input 3) does not match input it (input 1)
  - Returns: 0 for no event of note
  - Returns: 1 for movement event
  - Returns: 2 when the user selected something
  - Returns: 4 when the user pressed enter or clicked
             twice (no time limit) on a special state item

Finally, when finished with your list box use
  `freeStack_listBox_rayWidg()` to free variables insied
  your list box or `freeHeap_listBox_rayWidg()` to free
  your list box. For freeHeap, remember to set the
  listBox\_rayWidg pointer to null.

- Input (freeStack and freeHeap):
  - listBox\_rayWidg structure pionter to free

**Example**

```
#include <stdio.h>
#include <raylib.h>
#include "rayWidg.h"

typedef struct guiST
{
   struct widg_rayWidg wigetsST;

   signed int listBoxIdSI;
   struct listBox_rayWidg listBoxST;
} /*guiST*/

void
drawGUI_main(
   void *guiStruct
){
   struct guiST guiInST = (guiST *) guiStruct;

   BeginDrawing();
      ClearBackground( GetColor(def_white_rayWidg) );

      draw_listBox_rayWidg(
         0, /*only widget in GUI, so index 0*/
         0, /*draw the list box*/
         &guiInST->listBoxST,
         &guiInST->widgetsST
      );
   EndDrawing();
} /*drawGUI_main*/

int
main(
){
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int errSI = 0;

   struct guiST guiStackST;
   struct event_rayWidg eventStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and setup structures
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   init_event_rayWidg(&eventStackST);
   init_widg_rayWidg(&guiStackST.widgetsST);
   init_listBox_rayWidg(&guiStackST.listBoxST);
   
   if( setup_widg_rayWidg(&guiStackST.widgetsST) )
      goto memErr_main_sec05;
   
   mk_listbox_rayWidg(20, 100, 0, &guiStackST.widgetsST);
     /*20 is x, 100 is y, 0 for no tile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - setup gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add item to the list box, repeat for more items*/
   if(
      addItem_listBox_rayWidg(
         (signed char *) "some text to add",
         0, /*set to can select state*/
         &guiStackST.listBoxST,
         &guiStackST.widgetsST
      )
   ) goto memErr_main_sec05;

   if(
      addItem_listBox_rayWidg(
         (signed char *) "some more text",
         0, /*set to can select state*/
         &guiStackST.listBoxST,
         &guiStackST.widgetsST
      )
   ) goto memErr_main_sec05;

   /*initialize*/
   InitWindow(400, 200, "this is a GUI");
   SetTargetFPS(60);
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - draw the GUI and interact with users
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while( ! WindowShouldClose() )
   { /*Loop: run GUI*/
      errSI =
         get_event_rayWidg(
            1, /*incurment tabs for me*/
            drawGUI_main,/*my function to redraw the GUI*/
            &guiStackST, /*fed to drawGUI_main_sec05*/
            &guiStackST.wigetsST
         );

       listBoxEvent_rayWidg(
          0, /*only widget in GUI*/
          &guiStackST.listBoxST,
          &eventStackST,
          &guiStackST.widgetsST
       ); /*do not care about user selection, so not
          `   checking return values
          */

      drawGUI_main(&guiStackST);
   } /*Loop: run GUI*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSI = 0;
   goto ret_main_sec05;

   memErr_main_sec05:;
      fprintf(stderr, "memory error\n");
      errSI = 1;
      goto ret_main_sec05;

   ret_main_sec05::
      CloseWindow();
      freeStack_widg_rayWidg(&guiStackST.widgetsST);
      freeStack_listBox_rayWidg(&guiStackST.listBoxST);
      freeStack_event_rayWidg(&eventStackST);
      return errSI;
} /*main*/
```

#### list box manipuation functions

There are functions to change or get the states of
  different items, width, height, or maximun number of
  selected items in the list box.

Here is a list of the additional supporting functions.

- Clear items and states
  - `clear_listBox_rayWidg()`: clears (removes) all items
    from the list box (lazy delete)
    - Input: listBox\_rayWidg structure pionter
  - `clearSelect_listBox_rayWidg()`: removes the select
    state from all items in the list box
    - Input: listBox\_rayWidg structure pionter
- Set limits
  - `selectMaxSet_listBox_rayWidg()`: set the maximum
     number of items the user can select
     - -1 for any number
     - 0 for no items
     - > 0 for a set number of items
    - input:
      1. number of items to set the maxium to
      2. listBox\_rayWidg structure pionter
  - `widthSet_listBox_rayWidg()`: set the maximum and
    minimum width for the list box
    - input:
      1. maximum width (in pixels) for list box
      2. minimum width (in pixels) for list box
      3. listBox\_rayWidg structure pionter
  - `heightSet_listBox_rayWidg()`: set the maximum and
    minimum height for the list box
    - input:
      1. maximum height (in pixels) for list box
      2. minimum height (in pixels) for list box
      3. listBox\_rayWidg structure pionter
- Set states for an item
  - `hiddenSet_listBox_rayWidg()`: set (add or remove) the
    hidden state form/to an item (not shown in list box)
    - input:
      1. item to add or remove the hidden state from
      2. 1 is add hidden state, 0 is remove hidden state
      3. listBox\_rayWidg structure pionter
  - `selectSet_listBox_rayWidg()`: set (add or remove) the
    select state form/to an item
    - input:
      1. item to add or remove the select state from
      2. 1 is add select state, 0 is remove select state
      3. listBox\_rayWidg structure pionter
  - `specialSet_listBox_rayWidg()`: set (add or remove) the
    special state form/to an item
    - input:
      1. item to add or remove the special state from
      2. 1 is add special state, 0 is remove special state
      3. listBox\_rayWidg structure pionter
  - `canSelectSet_listBox_rayWidg()`: set (add or remove)
    the "can select" state form/to an item
    - input:
      1. item to add or remove the "can select" state from
      2. 1 is add state, 0 is remove state
      3. listBox\_rayWidg structure pionter
- Get state of an item
  - `hiddenGet_listBox_rayWidg()`: get if item is hidden
    - Input:
      1. index of item to get state of
      2. listBox\_rayWidg stucture pointer
    - Output:
      - Returns: 1 if item has hidden state
      - Returns: 0 if item is not hidden
  - `selectGet_listBox_rayWidg()`: get if item is selected
    - Input:
      1. index of item to get state of
      2. listBox\_rayWidg stucture pointer
    - Output:
      - Returns: 1 if item is selected
      - Returns: 0 if item is not selected
  - `specialGet_listBox_rayWidg()`: get if item is special
    - Input:
      1. index of item to get state of
      2. listBox\_rayWidg stucture pointer
    - Output:
      - Returns: 1 if item is special
      - Returns: 0 if item is not special
  - `canSelectGet_listBox_rayWidg()`: get if item can ge
     selected by the user
    - Input:
      1. index of item to get state of
      2. listBox\_rayWidg stucture pointer
    - Output:
      - Returns: 1 if item can be selected
      - Returns: 0 if item can not be selected

### file browsers

#### file browswer overview

A file browswer allows the user to select files. The file
  browser in rayWidg has five widgets. The parent widget,
  a list box to display files, two buttons (select and
  cancel), and a list box to display file extensions.

- Widgets:
  1. parent widget (does nothing)
  2. file list box (shows files)
     - uses all the hotkeys from the list box
  3. select button
  4. cancel button
  5. extension list box (extenions to show)
     - uses all the hotkeys from the list box

The file browser is a hog widget, so once revealed it
  takes the priority from all other widgets. It also takes
  up the GUI, hidding all other widgets.

#### file browswer setup

You can make a file browser widgets with
  the `mkFileBrowser_rayWidg()` function.

- Input:
  1. widg\_rayWidg structure to add the file browswer to
- Output:
  - Returns: id of parent widget for file browser
  - Returns: -1 for memory errors

The file browser widget only stores the state, coordinates
  and size of the file browser and its children. The
  actual browsing (present working directory and files)
  are stored in a files\_rayWidg structure. This allows
  you to have a separate structure for each file browser,
  but use the same widget for all.

To initialize a files\_rayWidg structure you use the
  `init_files_rayWidg()` function. The initialization
  function will clear all values, but will also set the
  present working directory (pwdStr) to the users home
  directory. It should only ever by called once. If you
  need to blank (clear files and reset home directory)
  use the `blank_files_rayWidg()` function.

- Input (init and blank):
  1. pointer to files\_rayWidg structure to intialize
     or if blanking, blank

After initialzation, you can set the maximum number of
  files the user can select with
  the `setFileLimit_files_rayWidg()` function.

- Input:
  1. maximum number of files the user can select
     - -1 for all files
  2. files\_rayWidg structure pointer to set file limit

#### file browsers; adding file extensions

You can then add file extensions to for the file browser
  to look for using the `addExt_files_rayWidg()` function.

- Input:
  1. c-string with file extension to add
  2. 1 to remove all extensions and then add the new file
     extension (input 1)
     - 0 to append the new file extension
  3. 1 to set the new file extension (input 1) to the
     select state
     - 0 to leave unselected
  4. files\_rayWidg structuer pointer to add the file
     extension (input 1) to
  5. widg\_rayWidg structure with the font using
- Output:
  - Returns 0 for no errors
  - Returns 1 for memory errors

- List of file extensions:
  1. `*` any file
  2. `+` any file or directory
  3. `dir` one or more directories
  4. `.<extension>` your file extension

#### file browser drawing

You can draw the file browser by first removing the
  hidden state from the widget
  (`hiddenClear_widg_rayWidg()`), and then calling the
  draw function.

The `fileBrowserDraw_rayWidg()` function will draw the
  file browser. It will also get files for a directory if
  no files are present in the files list.

- Input:
  1. id/index of file browser
  2. message to print in the file browser
     - must be short (one line max)
  3. width of the GUI
     - used to figure out the file browsers width
  4. height of the GUI
     - used to figure out the file browsers height
  5. files\_rayWidg structure pointer with present working
     directory, files to print, and file extensions
  6. widg\_rayWidg structure pointer with file browser
     widget
- Output:
  - yes there is output, but not worth noting

#### file browser; handeling events

You can deal with user input in a file brower by using
  the `fileBrowserEvent_rayWidg()` function. This uses
  the event\_rayWidg structure to get the user event.

- Input:
  1. id/index of file browser (parent)
     - used to make sure input in event\_rayWidg was
       really the file browser
  2. event\_rayWidg structure pointer with the users input
     event
  3. files\_rayWidg structure pointer with the present
     working directory (pwd), files in the pwd, and the
     file extensions to filter by
  4. widg\_rayWidg structure pionter with the file browser
     widget
- Output:
  - Returns: 2 if the user hit cancel
    - will wipe file selection history
  - Returns: 1 if the user hit select
  - Returns: 0 if the user did something with the file
    browser, but not worth noting
  - Returns: -1 if the file browser was not selected
    - input 1 (id) does not equal parent id in input 2
      (event\_rayWidg strucutre)
    - it is a different widget
  - Returns: -2 if no widget was selected
  - Returns: -3 for errors

#### file browser; get files

After the user has hit select, you can then get the
  selected file using the `getFile_files_rayWidg()`
  function. This function will return a heap allocated
  c-string with the first selected file to the input
  index. When finished the returned c-string must be
  freeded with `free()`.

To get multiple files, you can keep
  calling `getFile_files_rayWidg()` until the index is
  set to `-1`.

- Input:
  1. signed int pionter with index of first possible file
     - use index 0
     - this is modified to have the first possible index
       of the next file
  2. 1: to also return non-selected special files
     (directories)
     - use 0 (disables this; so only selected returned)
  3. files\_rayWidg structure pointer with files
- Output:
  - Modifies: input 1 (signed int pionter) to be at the
    first index of the next possible file, -1 if no more
    files, or -2 for a memory error
  - Returns: 0 for no files or an error
  - Returns: c-string with the next file
    - you must free this whith `free()` when you are
      finished

#### file browser; free structure

Finally, when finished, you need to free the file browser
  function using `freeStack_files_fileBrowser()` or
  the `freeHeap_files_fileBrowser()` functins. The stack
  free only frees the variables in the files\_rayWidg
  structure, while heap frees the structure, but you
  must set the pointer to 0.

- Input (stack and heap):
  1. pointer to files\_rayWidg structure to free

#### file browser example

```
#include <stdio.h>
#include <raylib.h>
#include "rayWidg.h"
#include "../genLib/ulCp.h"

typedef struct guiST
{
   struct widg_rayWidg widgetsST;
   signed char *mesgStr;

   signed int buttonIdSI;
   signed int mesgBoxIdSI;
   signed int browserIdSI;

   struct files_rayWidg filesRayWidgST;
} /*guiST*/

void
drawGUI_main(
   void *guiStruct
){
   struct guiST guiInST = (guiST *) guiStruct;

   BeginDrawing();
      ClearBackground( GetColor(def_white_rayWidg) );

      butDraw_rayWidg(
        200, /*maximum width*/
        20,  /*minimum width*/
        guiInST->buttonIdSI, /*id (index) of button*/
        (signed char *) "press_me",
        0,   /*want to draw*/
        &guiInST->widgetsST
      );

      fileBrowserDraw_rayWidg(
         guiInST->browserIdSI,
         (signed char *) "select some files",
         400, /*GUI is 400 pixels wide*/
         200, /*GUI is 200 pixels high*/
         &guiInST->filesRayWidgST,
         &guiInST->widgetsST
      );

      mesgBoxDraw_rayWidg(
         guiInST->mesgBoxIdSI,
         400, /*GUI is 400 pixels wide*/
         200, /*GUI is 200 pixels high*/
         guiInST->mesgStr,
         (signed char *) "Ok", /*text on button*/
         &guiInST->widgestST
      );
   EndDrawing();
} /*drawGUI_main*/

int
main(
){
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int errSI = 0;
   signed int indexSI = 0; /*index of file on*/

   struct guiST guiStackST;
   struct event_rayWidg eventStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and setup structures
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   guiStackST.mesgStr = 0;

   init_event_rayWidg(&eventStackST);
   init_widg_rayWidg(&guiStackST.widgetsST);
   init_files_rayWidg(&guiStackST.filesRayWidgST);
   
   if( setup_widg_rayWidg(&guiStackST.widgetsST) )
      goto memErr_main_sec05;
   
   guiStackST.browserIdSI =
      mkFileBrowser_rayWidg(&guiStackST.widgetsST);
   if(guiStackST.browserIdSI < 0)
      goto memErr_main_sec05;

   guiStackST->mesgBoxIdSI =
      mkMesgBox_rayWidg(&guiStackST.widgetsST)
   if(guiStackST.mesgBoxIdSI < 0)
      goto memErr_main_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - setup gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*initialize*/
   InitWindow(400, 200, "this is a GUI");
   SetTargetFPS(60);
   
   setFileLimit_files_rayWidg(
      2,
      &guiStackST.fileRayWidgST
   ); /*only allowing user to select two files*/

   /*add some file extensions*/
   if(
      addExt_files_rayWidg(
         (signed char *) ".txt",
         1, /*make sure no older extensions*/
         1, /*select by default*/
         &guiStackST.fileRayWidgST
         &guiStackST.widgetsST
      )
   ) goto memErr_main_sec05;

   if(
      addExt_files_rayWidg(
         (signed char *) ".tsv",
         0, /*want older file extensions, so append*/
         1, /*select by default*/
         &guiStackST.fileRayWidgST
         &guiStackST.widgetsST
      )
   ) goto memErr_main_sec05;

   if(
      addExt_files_rayWidg(
         (signed char *) "*", /*any file*/
         0, /*want older file extensions, so append*/
         0, /*do not select by default (user must)*/
         &guiStackST.fileRayWidgST
         &guiStackST.widgetsST
      )
   ) goto memErr_main_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - draw the GUI and interact with users
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while( ! WindowShouldClose() )
   { /*Loop: run GUI*/
      errSI =
         get_event_rayWidg(
            1, /*incurment tabs for me*/
            drawGUI_main,/*my function to redraw the GUI*/
            &guiStackST, /*fed to drawGUI_main_sec05*/
            &eventStackST, /*gets the event*/
            &guiStackST.widgetsST
         );

       if(eventStackST.idSI == guiStackST.buttonIdSI)
          hidenClear(guiStackST.browserId, &guiStackST);
          /*need to reveal the file browser*/

       else if(eventStackST.idSI ==guiStackST.browserIdSI)
       { /*Else If: user interacted with file browser*/
          errSI =
             fileBrowserEvent_rayWidg(
                guiStackST.browserIdSI,
                &eventStackST,
                &guiStackST.fileRayWidgST,
                &guiStackST.widgetsST
             ); /*do not care about user selection, so not
             `   checking return values
             */

          if(errSI == 1)
          { /*If: user selected files*/
             hidenClear_widg_rayWidg(
                guiStackST.mesgBoxIdSI,
                &guiStackST.widgetsST
             );

             indexSI = 0;

             guiStackST.mesgStr =
                getFile_files_rayWidg(
                   &indexSI, /*moved to next file*/
                   0,
                   guiStackST.filesRayWidgST
                );

             if(indexSI < -1)
               goto memErr_main_sec05;
          } /*If: user selected files*/
       } /*Else If: user interacted with file browser*/

       else if(eventStackST.idSI ==guiStackST.mesgBoxIdSI)
       { /*Else If: showing message box*/
           free(guiStackST.mesgStr);

           guiStackST.mesgStr =
              getFile_files_rayWidg(
                 &indexSI, /*moved to next file*/
                 0,
                 guiStackST.filesRayWidgST
              );

           if(indexSI < -1)
              goto memErr_main_sec05;
           else if(indexSI == -1)
           { /*Else If: no more files to show*/
              mesgBoxEvent_rayWidg(
                 2, /*release message box*/
                eventStackST.parIdSI,
                eventStackST.idSI, 
                &guiStackST.widgetsST
              );
           } /*Else If: no more files to show*/
       } /*Else If: showing message box*/

       drawGUI_main(&guiStackST);
   } /*Loop: run GUI*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSI = 0;
   goto ret_main_sec05;

   memErr_main_sec05:;
      fprintf(stderr, "memory error\n");
      errSI = 1;
      goto ret_main_sec05;

   ret_main_sec05::
      CloseWindow();
      if(guiStackST.mesgStr)
         free(guiStackST.mesgStr);
      guiStackST.mesgStr = 0;

      freeStack_widg_rayWidg(&guiStackST.widgetsST);
      freeStack_files_rayWidg(&guiStackST.filesRayWidgST);
      freeStack_event_rayWidg(&eventStackST);
      return errSI;
} /*main*/
```

This is a very simple example hellow world and shows why
  rayWidg is not the best system. You can compile it on
  Linux with. Unlike the previous examples, this one has
  been tested.

```
cc -o example example.c rayWidg.c ../genLib/base10str.c ../genLib/ulCp.c ../genLib/ptrAry.c -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

Code in example.c

```
#include <raylib.h>
#include "rayWidg.h"

int
main(
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Table Of Contents:
   '   o sec01: variable declaratins
   '   o sec02: initialization
   '   o sec03: add widgets
   '   o sec04: draw the GUI
   '   o sec05: detect tab key preses (cycle focus)
   '   o sec06: detect enter key preses
   '   o sec07: detect mouse clicks
   '   o sec08: clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Sec01: variable declaratins
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buttonTextStr= (signed char *) "Press me";
   signed int guiWidthSI = 400;
   signed int guiHeightSI = 200;
   signed int buttonIdSI = 0;
   signed int messageBoxIdSI = 0;

   signed int halfWidthSI = 0;
   signed int halfHeightSI = 0;

   signed int keySI = 0;
   signed int indexSI = 0;
   signed int parentIdSI = 0;

   struct widg_rayWidg widgetsStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Sec02: initialization
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_widg_rayWidg(&widgetsStackST);
   
   /*initialize*/
   InitWindow(guiWidthSI, guiHeightSI, "this is a GUI");
   SetTargetFPS(60);
   
   if( setup_widg_rayWidg(&widgetsStackST, 20) )
       goto err_sec08;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Sec03: add widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*this part is not needed, but I wanted to place the
   `  button in the center
   */
   halfWidthSI =
      textMeasure_widg_rayWidg(
         buttonTextStr,
         &halfHeightSI,
         &widgetsStackST
      );

   halfWidthSI = guiWidthSI / 2 - halfWidthSI;
   halfHeightSI = guiHeightSI / 2 - halfHeightSI;

   /*this is the add button command*/
   buttonIdSI =
      addWidget_widg_rayWidg(
         halfWidthSI,  /*x-axis coordinate of button*/
         halfHeightSI, /*y-axis coordinate of button*/
          0, /*not using tiling system*/
         -1, /*set height automatically*/
         -1, /*set width automatically*/
         &widgetsStackST
      );
         
   messageBoxIdSI = mkMesgBox_rayWidg(&widgetsStackST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Sec04: draw the GUI
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while( ! WindowShouldClose() )
   { /*Loop: run GUI*/
      BeginDrawing();
         ClearBackground( GetColor(def_white_rayWidg) );

         butDraw_rayWidg(
            guiWidthSI, /*maximum width of button*/
            20,  /*minimum width of button*/
            buttonIdSI,
            "Press This",
            0,   /*draw this button*/
            &widgetsStackST
         );

         mesgBoxDraw_rayWidg(
            messageBoxIdSI,
            guiWidthSI,
            guiHeightSI,
            "hello world!",
            "hi",
            &widgetsStackST
         );
      EndDrawing();

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Sec05: detect tab key preses (cycle focus)
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      keySI = GetKeyPressed();
      indexSI =
        focusCheck_widg_rayWidg(keySI, 0, &widgetsStackST);
          
       if(indexSI > -2)
          continue;
          /*means user pressed tab; For entry boxes you
          `  will want to set/clear the activate state
          `  here
          */

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Sec06: detect enter key preses
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      indexSI =
         enterCheck_widg_rayWidg(
            keySI,
            0,
            &parentIdSI,
            &widgetsStackST
         );

      if(indexSI > -2)
      { /*If: user hit enter*/
         while( IsKeyDown(KEY_ENTER) )
         { /*Loop: update GUI until enter key released*/
            /*this section here should be put into a
            `  separate function
            */
            BeginDrawing();
               ClearBackground(
                  GetColor(def_white_rayWidg)
               );

               butDraw_rayWidg(
                  guiWidthSI, /*maximum width of button*/
                  20,  /*minimum width of button*/
                  buttonIdSI,
                  "Press This",
                  0,   /*draw this button*/
                  &widgetsStackST
               );

               mesgBoxDraw_rayWidg(
                  messageBoxIdSI,
                  guiWidthSI,
                  guiHeightSI,
                  "hello world!",
                  "hi",
                  &widgetsStackST
               );
            EndDrawing();
         } /*Loop: update GUI until enter key released*/

         /*remove the press event*/
         pressClear_widg_rayWidg(indexSI, &widgetsStackST);

         if(indexSI == buttonIdSI)
         { /*If: user selected the button*/
            hidenClear_widg_rayWidg(
               messageBoxIdSI,
               &widgetsStackST
            );
         } /*If: user selected the button*/

         else if(parentIdSI == messageBoxIdSI)
         { /*Else If: user selected something in message*/
            mesgBoxEvent_rayWidg(
               2,
               messageBoxIdSI, 
               indexSI,
               &widgetsStackST
            );
         } /*Else If: user selected something in message*/

         continue;
      } /*If: user hit enter*/

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Sec07: detect mouse clicks
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) )
         keySI = 2;
      else if( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) )
         keySI = 1;
      else
         continue; /*not left click*/

       /*clear any old press events*/
       pressClear_widg_rayWidg(buttonIdSI,&widgetsStackST);
       mesgBoxEvent_rayWidg(
          0,
          messageBoxIdSI,
          0, /*no need to list child for a clear*/
          &widgetsStackST
       );

       indexSI =
          getMouseWidg_widg_rayWidg(
             GetMouseX(),
             GetMouseY(),
             &parentIdSI,
             &widgetsStackST
          ); /*find which widget was clicked*/

       if(indexSI == buttonIdSI)
       { /*If: user selected the button*/
          hidenClear_widg_rayWidg(
             messageBoxIdSI,
             &widgetsStackST
          );
       } /*If: user selected the button*/

       else if(parentIdSI == messageBoxIdSI)
       { /*Else If: user selected something in message*/
          mesgBoxEvent_rayWidg(
             keySI,
             messageBoxIdSI, 
             indexSI,
             &widgetsStackST
          );
       } /*Else If: user selected something in message*/

       continue;
   } /*Loop: run GUI*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Sec08: clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexSI = 0;
   goto ret_sec08;

   err_sec08:;
      indexSI = 1;
      goto ret_sec08;

   ret_sec08:;
      freeStack_widg_rayWidg(&widgetsStackST);
      CloseWindow();
      return indexSI;
} /*main*/
```
