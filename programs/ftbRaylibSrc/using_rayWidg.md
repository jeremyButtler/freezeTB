# Goal

Give an idea of how to use the rayWidg system to construct
  and build a gui.

# Reason

The rayWidg system is not a very good system. It was made
  because I needed a sysmtem to build a non-game based
  GUI using raylib. I had the option of building a GUI
  using base raylib, which would have inovled a lot of
  hard coding or I could have used guiraylib, which had
  the widgets, but no documentation.

In the end I building a system. The design decisions
  reflect my goal of getting a GUI up somewhat quickly,
  but also have some organization on logic.

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
7. stateAryUC: state(s) the widget is in, this can affect
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

# Flow

The steps in using rayWidg are:

1. create and initialize widg_rayWidg struture
2. add widgets to widg_rayWidg strcuter
3. draw gui, use BeginDrawing, ClearBackground, and
   EndDrawing from raylib and the widget functions from
   rayWidg
   - for tileing you will also have to call widget
     functions
4. get user events and udate and draw GUI
5. free memory and structuers

## 1. create and initialize

You can initialize (set everything to 0/null) a
  widg_rayWidg structer with init_rayWidg(). The input is
  a pionter to a widg_rayWidg structer to initialize.

```
struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);
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
```

**tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

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
```

**tileling example**

```
signed int idOneSI = 0;

struct widg_rayWidg widgetsStackST;
init_widg_rayWidg(&widgetsStackST);

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
```

## 4. get user events and udate and draw GUI

You can get users events using the user event functions.

Note: when using enter to press buttons, you can hold down
  the press by detecting if the enter key `KEY_ENTER` is
  held down. However, you need to redraw the GUI to update
  if the enter key has been released. Otherwise, you get
  an infinite loop.

```
indexSI =
   enterCheck_widg_rayWidg(keySI, 0, guiSTPtr->widgSTPtr);
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
