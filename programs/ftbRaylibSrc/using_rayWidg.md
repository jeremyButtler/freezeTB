# Goal

Give an idea of how to use the rayWidg system to construct
  and build a gui.

# Reason

The rayWidg system is not a very good system. It was made
  because I needed a system to build a non-game based
  GUI using raylib. I had the option of building a GUI
  using base raylib, which would have required a lot of
  hard coding or I could have used guiraylib, which had
  the widgets, but no documentation.

In the end I buit a system. The design decisions reflect
  my goal of getting a GUI up somewhat quickly, but also
  having some organization on logic.

# Use

Organizes widgets, so you mostly have to focus on the
  figuring to do when an event happened. Querying
  functions are used to find the id of the clicked/enter
  widget. You then have to set up the functions to handle
  what the event does.

Also, you have to set up what to draw, however, there
  are widget functions to help you here.

# Structures

Only one structure is used and it is the widg_rayWidg
  struct. It keeps track of the widget coordinates and
  states each widget is in. You can get or set these
  variables by querying it with funcitions or by direct
  access.

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
  - you need to call tile_widg_rayWidg to get tile widgets
    x and y coordaintes
6. colArySI: the column number if using tileing format
  - this is < 0 for non-tile widgets
  - you need to call tile_widg_rayWidg to get tile widgets
    x and y coordaintes
7. stateAryUS: state(s) the widget is in, this can affect
   if a widget is drawn and its coloring
   - pressed state means it is being pressed by a key or
     by the mosue
     - defition is: def_press_rayWidg
   - checked state means it is a widget that can be
     checked and that it has been checked
     - defintion is: def_checked_rayWidg
   - active state means the widget is being activly used
     - defintion is: def_active_rayWidg
   - focus state means the widget is currently in focus
     - defintion is: def_focus_rayWidg
   - inactive state means the widget is seen, but can not
     be interacted with
     - defintion is: def_inactive_rayWidg
   - hiden state means the widget can not be seen, nor
     interacted with
     - defintion is: def_hiden_rayWidg
   - child state means the widget is part of a more
     complex widget
     - defintion is: def_child_rayWidg
   - hog state means the widget will not let other widgets
     respond to user events or be in focus
     - defintion is: def_hog_rayWidg
8. lenSI is the number of widgets that have been added
9. sizeSI is the maximum number of widgets you can add
    before reszing (calling realloc_widg_rayWidg)
10. fontSizeSI is the current size of the font being
    used
    - if you change this, then call
      `measureFont_widg_rayWidg()` to set the correct
      values for the new font size
    - `measureFont_widg_rayWidg(&widg_rayWidg_struct);`
11. spacingF is the current font spacing (default is 1)
    being used
    - if you change this, then call
      `measureFont_widg_rayWidg()` to set the correct
      values for the new font size
    - `measureFont_widg_rayWidg(&widg_rayWidg_struct);`

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
  tile_widg_rayWidg function will then auto find the x,y
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

- def_widgHeightGap_rayWidg: controlls the y-axis
  (vertical) gap between widgts and the padding around
  text in widgets
  - for gap: font_height / def_widgHeightGap_rayWidg
  - for padding: font_height / def_widgHeightGap_rayWidg
- def_maxStrLen_rayWidg: maximum length of string allowed
- def_cursor_rayWidg: ascii character printed for cursor
  in entry boxes or other widgets that use the cursor
  (no idea which)
- def_blinkCursor_rayWidg: ascii chacter printed for when
  the cursor is blinked
- def_border_rayWidg: number of pixels to offset the
  border around a shape by
- def_focusBorder_rayWidg: number of pixels to offset
  the focus border by
- def_<colorName>_rayWidg: predefined hex code for a color
  in rayWidg (colors can vary between OSs)
  - use `Color colST GetColor(def_<colorName>_rayWidg)` to
    get the color for raylib or for rayWidg use hex values
  - lightGrey is a light grey color
  - darkGrey is a dark grey color
  - white is white
  - black is black
- def_fontSize_rayWidg: default font size to use (20)
- def_macRoundness_rayWidg: controlls how round the
  rectangles are on a Mac or when `-DMAC` is used during
  compile time
- def_macSegments_rayWidg: segments is used in drawing
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

- def_press_rayWidg: is the pressed state
  - button is pressed
- def_checked_rayWidg: is the checked state
  - check box or radio button (not used) is checked
- def_active_rayWidg: is the active state
  - entery box or similar active widget is being used
- def_focus_rayWidg: is the focus state
  - current widget in focus/working with
- def_inactive_rayWidg: is the inactive state
  - widget is does not take user input, ingore in focus
    states
- def_hiden_rayWidg: is the hiden state
  - widget is hidden and should be ignored
- def_child_rayWidg: is the child state
  - is a child widget in a complex widget
- def_hog_rayWidg: is the hog state
  - widget is a high priority widget, nothing else can
    be interacted with

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
  widg_rayWidg structer with init_widg_rayWidg(). The
  input is a pionter to a widg_rayWidg structer to
  initialize.

```
struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);
```

After initialization you needt to setup a widg_rayWidg
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

You can add simple widgets with the addWidget_widg_rayWidg
  function. The only exceptions are for complex widgets,
  which as a rule require dedicated functions to set them
  up.

Input:

- x coordinate or column to assign of widget to
- 7 coordinate or row to assign of widget to
- 1: if using tile coordinates (column/row) for the widget
  - else use 0
- width of the widget or -1 to auto assign
  - you must provided a non-negative width for the entry
    box
- hieght of the widget or -1 to auto assign
- widg_rayWidg structure pointer to add widget to

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

Functions:

- butDraw_rayWidg: draws a button and/or updates height
  and width of a button widget
- entryDraw_rayWidg: draws an entry box and/or updates
  height of entry box
- labDraw_rayWidg: draws a lable and/or updates height and
  width of label

### tile widgets

The tilein system uses tile_widg_rayWidg to find the x,y
  coordinates for the tiled widgets. You will need to call
  the drawing functions first or set the width and height
  manually.

Input:

- widg_rayWidg structure with tiled widgets to update
- the number of pixels to offset each row by
  - spacing between rows, use -1 to do the height of one
    character (def_height_rayWidg)
- the number of pixels to offset each column by
  - spacing between columns, use -1 to do number of pixels
    in one letter ('a')

The output is 0 for no errors and 1 for memeory errors.
  Look at the buttons, entry, and other widgets tile
  examples for tile_widg_rayWidg examples.

### rectangles

The most basic widget, that makes up most other widgets is
  the rectangle. You can draw a rectangle
  with `drawRec_rayWidg()`.

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
         def_border_rayWidg)
    - 2: draw rectangle with a focus (2nd) border (offset
         is def_focusBorder_rayWidg)
    - 3: draw rectangle with border and focus border
  - rectangles color (as hex code)
  - borders color (as hex code)
  - focus borders color (as hex code)
- Returns:
  - 0 if the widget is hidden
  - def_noWidget_rayWidg if the widget id is out of bounds
    (no widget)
  - width of widget (widgSTPtr->widthArySI[id])
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
  using butDraw_rayWidg().

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
- widg_rayWidg structure pointer with widget to draw as
  a button

Return value:

- width of the button for succes,
- 0 if the widget is hidden (not drawable)
- def_noWidget_rayWidg if the widget does not exist

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
  using entryDraw_rayWidg(). An entry box displays a line
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
- widg_rayWidg structure pointer with widget to draw as
  an entry box

Return value:

- width of the entry box for success
- 0 if the widget is hidden (not drawable)
- def_noWidget_rayWidg if the widget does not exist

This will also modify the height and width values.

**text manipulation functions for entry box:**

There are also support functions for processing the text
  that goes into an entry box.

- intStrCheck_rayWidg will modifiy the input text to be
  an integer.
  - it will also remove characters until it is less then
    the maximum value
  - first character is the cursor, next are the characters
    at the end
- floatStrCheck_rayWidg intStrCheck_rayWidg, but for
  floats instead of integers
- fileStrCheck_rayWidg check if file/path has valid
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

### label

You can draw a label (text) or get dimensions for a label
  using labDraw_rayWidg().

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
- widg_rayWidg structure pointer with widget to draw as
  a label

Return value:

- width of the label for success
- 0 if the widget is hidden (not drawable)
- def_noWidget_rayWidg if the widget does not exist


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
  - def_noWidg_rayWidg if no widget was in the message
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

## 7. free memory and structuers

When finshed you need to free the `widg_rayWidg` structure
  with `freeStack_widg_rayWidg()` (for stack allocations)
  or `freeHeap_widg_rayWidg()` (for heap allocations).

Then you need to call `CloseWindow()` from  raylib to
  close the GUI.

```
#include <raylib.h>
#include "rayWidg.h"

int
main(
){
   struct widg_rayWidg &widgetsStackST;
   init_widg_rayWidg(&widgetsStackST);
   
   /*initialize*/
   InitWindow(400, 200, "this is a GUI");
   SetTargetFPS(60);
   
   if(setup_widg_rayWidg(&widgetsStackST)
       /*deal with  memory errors*/
   
   while( ! WindowShouldClose() )
   { /*Loop: run GUI*/
      /*GUI commands go here; idealy as a function*/
   } /*Loop: run GUI*/
   
   freeStack_widg_rayWidg(&widgetsStackST);
   CloseWindow();
   return 0;
} /*main*/
```

This is a very simple example hellow world and shows why
  rayWidg is not the best system. You can compile it on
  Linux with. Unlike the previous examples, this one has
  been tested.

```
cc -o example example.c rayWidg.c ../genLib/base10str.c ../genLib/ulCp.c -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
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
