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
  refere to the left top coner of the widget you wish to
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

The rayWidg library uses three states to organize widgets.
  These are the hiden state and the child states.

The hiden state means that the widget is not present or
  part of the current GUI. All drawing functions and event
  functions will ignore the widget. This can be used for
  menu bars or alternative GUI windows.

The inactive state means that the widget is present, but
  that it should be ingored. All drawing functions will
  draw the widget in the inactive color scheme and event
  functions will ignore the widget.

The child state means that the widget is part of a widget
  that is made up of multiple widgets (a complex widget),
  such as a list box or file browser. The rayWidg system
  only allows one level of nesting. This forces the
  complex wigdg to use functions to allow it to do its
  task. In some cases, such as the file browser, those
  functions are pre-made. In other cases you will have to
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
- def_macRoundness_rayWidg: controlls how round the
  rectangles are on a Mac or when `-DMAC` is used during
  compile time
- def_maSegments_rayWidg: segments is used in drawing
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
  flags are possible. Currenly only 8 flags are used.

- def_press_rayWidg: is the pressed state
- def_checked_rayWidg: is the checked state
- def_active_rayWidg: is the active state
- def_focus_rayWidg: is the focus state
- def_inactive_rayWidg: is the inactive state
- def_hiden_rayWidg: is the hiden state
- def_child_rayWidg: is the child state

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

   UnloadFont(newFontST);
      /*I think this is needed, but have no idea*/
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
  font to be a bit ugley and not very legibal.

Again, rayWidg is not supper usefull. It was designed to
  be something I could peices together in a shorter period
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

1. create, initialize, and setup a widg_rayWidg struture
2. add widgets to widg_rayWidg strcuter
3. draw gui, use BeginDrawing, ClearBackground, and
   EndDrawing from raylib and the widget functions from
   rayWidg
   - for tileing you will also have to call widget
     functions
4. get user events and udate and draw GUI
5. free memory and structuers

## 1. create, initialize, and setup

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
  structure to setup and the font size to use. The
  return value is 0 for success and 1 for memory errors.

Currently this only affects the font.

```
struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);
if( setup_widg_rayWidg(&widgStackST, 20) );
   /*deal with memory errors*/
```


## 2. add widgets

You can add widgets with the addWidget_widg_rayWidt
  function.

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

if( setup_widg_rayWidg(&widgStackST, 20) );
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


## 3. draw gui

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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

if( setup_widg_rayWidg(&widgStackST, 20) );
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

## 4. get user events and udate and draw GUI

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
