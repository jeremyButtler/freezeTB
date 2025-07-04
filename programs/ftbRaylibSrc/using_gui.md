# Use

Describe how to use the code used in generating the
  freezeTB GUI.

This is not really that useful, but is here in case you
  need it.

# Structure:

There are three files you can work with; ftbWidg,
  ftbRayST, and ftbRaylib. ftbWidg is the most usefull
  and has the functions to setup the individual widgets
  to be drawn, but does not glue anything together.
  ftbRayST is the hardcoded parts the glue the widgets
  together. Then ftbRayLib is the main wrapper/function.

# ftbWidg:

In ftbWidg you have four functions and several defined
  variables. The functions are getDarkModeState_ftbWidg,
  checkGuiColorMode_ftbWidg, butDraw_ftbWidg, and
  entryDraw_ftbWidg.

You will need raylib.h to be in the include path. You
  will also need ulCp.h in `../genLib`.

## global and defined variables

There are several pre-defiend variables in ftbWidg that
  are used across the functions.

### size and text variables

- def_windowWidth_ftbWidg
  - default width of the initialized window
  - is a defined (`#defined var <number>`) variable
- def_windowHeight_ftbWidg
  - default width of the initialized window
  - is a defined (`#defined var <number>`) variable
- def_cursor_ftbWidg
  - asci character to display for the cursor
  - is a defined (`#defined var '|'`) variable
- def_blinkCursor_ftbWidt
  - asci character to display in entry box's when the
    type is 8
  - is a defined (`#defined var ' '`) variable
- glob_fontSizeSI_ftbWidg
  - font size to use with raylibs font
  - default is 18
- glob_heightSI_ftbWidg
  - height of a single character;
  - for raylibs default font it is the font size. So, if
    you are using raylibs default font, you need to keep
    this in sync with glob_fontSizeSI_ftbWidg

## global color variables

There are several global variables for colors, which
  can change when dark or light mode is dected with
  checkGuiColorMode_ftbWidg. The are also used in widgets
  to set widget colors.

To use these, you will need to call raylib's GetColor()
  to use these on your own. However, many of my functions
  call these colors and the dark/light mode check will
  modify these colors.

Ex: `Color newColor = GetColor(glob_<colorSI>_ftbWidg)`

### Global color variables

- glob_textColSI_ftbWidg
  - hex code for the default text color
- glob_textAltColSI_ftbWidg
  - hex code for text color when a state is changed
  - this is for the button pressed event
- glob_guiColSI_ftbWidg
  - hex code for background color of GUI
- glob_butColSI_ftbWidg
  - hex code for the color of a button
- glob_butBorderColSI_ftbWidg
  - hex code for a buttons border color
- glob_butPressColSI_ftbWidg
  - hex code for the color of a button when it is pressed
- glob_butPressBorderColSI_ftbWidg
  - hex code for a buttons border when the button is
   pressed
- glob_entryColSI_ftbWidg
  - hex code code color for an entry box color
- glob_entryBorderColSI_ftbWidg
  -  hex code code color for an entry box border
- glob_entryActiveColSI_ftbWidg
  - hex code code color for an entry box color when the
    user is typing in it
- glob_entryActiveBorderColSI_ftbWidg
  - hex code code color for an entry box border when the
     user is typing in it

### Global defined colors

In addtions to the colors for widgets, there are also
  the defiend colors for each variable. These colors do
  not change and are the hex codes for raylib. So, you
  can use these for stable colors or to define the dark
  and light pallets.

You need to call raylib's GetColor(defined_color) to
  convert this to a color for raylib. I will likey add
  more colors in when I get the GUI to a more polished
  state.

Ex: `Color newColor = GetColor(def_<colorSI>_ftbWidg)`

- def_lightGrey_ftbWidg
  - light grey color
  - light mode is the normal button
  - dark mode is the text alternative color
    - this is shown during a button press
- def_darkGrey_ftbWidg
  - a dark grey color
  - light mode is a button press color
  - dark mode is the button color
- def_white_ftbWidg
  - white
  - light mode is background color
  - dark mode is the text color
- def_black_ftbWidg
  - a very dark grey or black
  - light mode is the text color
  - dark mode is the backgroud color

### state defined variables

State defined variables are variables that report on the
  widgets state.

- def_press_ftbWidg: widget is being pressed on by the
  mouse (or something else)
- def_checked_ftbWidg: checkbox widget is in the checked
  state (use selected it)
- def_inactive_ftbWidg: widget is still seen, but the user
  can not interact with it
  - this state is enoforced by you, the widget is
    drawn differently
- def_hiden_ftbWidg: widget is hiden from the user and is
  not drawn
- def_active_ftbWidg: entry box like widget is accpeting
  text input
  - this is enforced by you, widget is drawn differently
- def_focus_ftbWidg: the focus border is drawn around the
  widget
  - this is enforced by you, widget is drawn differently

## light versus dark mode

On Mac and Windows you can query if light or dark mode
  is present. For Linux the return value (if is one) is
  always unkown, which goes to light mode.

- getDarkModeState_ftbWidg: checks to see if the Mac or
  Windows computer is in light or dark mode.
  - No input
  - Return values:
    - def_darkMode_ftbWidg for dark mode
    - def_lightMode_ftbWidg for light mode
    - def_unkownMode_ftbWidg unable to check for light
      or dark mode (ex Linux)

- checkGuiColorMode_ftbWidg: runs getDarkModeState_ftbWidg
  and then sets the color pallate variables to the correct
  mode
  - for def_unownMode_ftbWidg, this defaults to light
    mode
  - No Input
  - No Return
  - Modifies:
    - glob_focusColSI_ftbWidg
    - glob_textColSI_ftbWidg
    - glob_textAltColSI_ftbWidg
    - glob_guiColSI_ftbWidg
    - glob_butColSI_ftbWidg
    - glob_butBorderCol_ftbWidg
    - glob_butPressColSI_ftbWidg
    - glob_butPressBorderCol_ftbWidg
    - glob_entryColSI_ftbWidg
    - glob_entryBorderColSI_ftbWidg
    - glob_entryActiveColSI_ftbWidg
    - glob_entryActiveBorderColSI_ftbWidg

## widgets

The widgets are designed for you to handle the event
  processing, but they handle the drawing. The are not
  very pretty, but work.

### Buttons

- butDraw_ftbWidg: draws a button on your GUI
  - this only draws the button, you must monitor it and
    check if it was clicked on.
  - Input:
    - maximum width of button
      - this is not the actual width, but is the maximum
        allowed width before the text is shortend
    - x coordinate (0 is left)
    - y coordinate (0 is top)
    - state the button is in (you can include mutiple
      states with (state1 | state2)
      - def_hiden_ftbWidg for a hidden button (not drawn)
        - this state is checked first and all other states
          are ignored
      - def_inactive_ftbWidg for a visiable, but
        non-usable button
        - this state is checked second and all other
          states are ignored
      - def_press_ftbWidg if the button is being pressed
      - def_focus_ftbWidg draw the focuse border around
        the button
        - do `def_press_ftbWidg | def_focus_ftbWidg` to
          enter both states
    - c-string with text to draw on the button
      - is reduced if needed
  - Returns the width of the drawn button
  - All buttons y-axis width is in def_height_ftbWidg

### entry box's

- entryDraw_ftbWidg: draws an entry box on your GUI
  - this only draws the entry box and does some basic
    checks for floats, integers, and string lengths, you
    must detect key strokes and set the input accordinly
  - Input:
    - width of entry box
    - x coordiante of entry box (0 is left)
    - y coordinate of entry box (0 is top)
    - offset between the entry box label and the entry
      box
      - use -1 for auto (def_height_ftbWidg / 2)
      - any other value for custom value
    - the scroll positon (start of text to display)
    - the position of the cursor
    - the type of entry box, this determines what can
      be input
      - 0 (not 1 or 2) is string entry box
      - 16, for string entry mode forces a strict file
        name only mode 
        - only accepts: `A-Z`, `a-z`, `0-9`, `-`, `_`
      - 1 is a integer entry box
      - 2 is a float entry box
      - 4 draws entry box beneath the label
      - 8 replaces cursor with a space
      - you can merge the 4 (box beneath), and 8 (cursor
        to space) types with an integer or float
    - the state the entry box is in
      - def_hiden_ftbWidg for hidden entry box (not drawn)
        - this state is checked first and all other states
          are ignored
      - def_inactive_ftbWidg for a visiable, but
        non-usable entry box
        - this state is checked second and all other
          states are ignored
      - def_active_ftbWidg if the entry box is active
        and being typed in
      - def_focus_ftbWidg draw the focuse border around
        the entry box
        - do `def_active_ftbWidg | def_focus_ftbWidg` to
          enter both states
    - the c-string to display in the entry box
      - this is modified if bad input
    - the c-string with the lable to print before or above
      the entry box
    - the maximum value or string length allowed in the
      entry box

# ftbRayST:

## overview 

This file is not very usefull, but shows how to set up
  a GUI, or for you to hack up a GUI. You must either
  tweak this functions for your own use (nightmere one) or
  make up your own version (nightmere two).

## structure

The gui_ftbRayST structure has all the needed input for
  each widgets function. I also keep track of the focused
  widget by each widgets id. This id is defined in
  the `#define def_id<widget>_ftbRayST` variables.

Each widgets x and y coordinates and the entry box's
  sizes are also stored in predefined variables and also
  in the structure.

The state, width of each widget, text to print for each
  widget or key stored values are aslo stored in the
  gui_ftbRayST structure.

## functions

The blank_gui_ftbRayST funtion is used to set the default
  values and is called by init_gui_ftbRayST. The structure
  can be freeded with the freeStack_gui_ftbRayST and
  freeHeap_gui_ftbRayST functions.

The gui is drawn with drawGui_ftbRayST. This function
  includes the ray functions needed to draw the gui.
  However, it is hidden because you should call the
  other functions. The widgets need to go between
  a `BeginDrawing()` and an `EndDrawing()` block.

```
void
draw_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   BeginDrawing();
      ClearBackgrond(GetColor(glob_guiColSI_ftbWidg));

      /*make sure in correct light/dark mode*/
      checkGuiColorMode_ftbWidg();

      /*Your widget functions and drawing commands go
      `  here, my exmaples beneath do not use the 
      `  guiSTPtr structgure much, however, the real code
      `  does for most of the input
      */
      
      /*draw a button*/
      guiSTPtr->widthFqButSI =
         butDraw_ftbWidg(
            def_maxWidgetWidth_ftbWidg,
            20, /*x coordiante*/
            50, /*y coordinate*/
            0,  /*has no current state*/
            "this is a button" /*button label*/
         );

      /*drawing an entry box*/
      guiSTPtr->widthPrefixEntrySI =
         entryDraw_ftbWidg(
            guiSTPtr->entryWidthSI,
            20,  /*X coordiante*/
            100, /*Y coordiante*/
            -1,  /*use default spacing for label & entry*/
            0,   /*at character 0 in text box*/
            0,   /*cursor is at character 3*/
            16,  /*only allowing valid file names*/
            def_active_ftbWidg | def_focus_ftbWidg,
               /*is in active state and focus is on box*/
            guiSTPtr->userInputStr,
               /*c-string with user input*/
            "this is an entry box:", /*label for box*/
            32 /*limiting user input to 32 characters*/
         );
   EndDrawing();
}
```

The init_gui_ftbRayST initializes and sets a GUI up. It
  will set the default sizes, frames per second, and
  the icon.

```
void
init_gui_ftbRay(
   struct gui_ftbRayST *guiSTPtr
){
   /*set initial window size and title*/
   InitWindow(guiSTPtr->width, guiSTPtr->height, "title");

   SetTargetFPS(60); /*60 frames per second refresh rate*/

   /*draw the initial GUI*/
   draw_gui_ftbRayST(guiSTPtr);
}
```

The checkRunEvent_ftbRayST checks which widgets have been
  selected and then runs there events. It will also handel
  the key presses (tab, enter, entry box text), click
  events, and will redraw the GUI when finished. This is
  were all the glue is. Yes it is long.

The core of it is to get possible input, and then check
  that possible input

```
void
checkRunEvent_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   /*detect if user pressed a key*/
   signed char keySC = GetCharPressed();

   /*get mouse coordinates*/
   signed int xSI = GetMouseX();
   signed int ySI = GetMouseY();

   /*detect if user pressed the left mouse button*/
   signed char releaseBl = IsMouseButtonReleased(LEFT);
   signed char pressBl = IsMouseButtonPressed(LEFT);

   /*detect if user moved the scroll wheel*/
   int scrollSI = (signed int) GetMouseWheelMove();
      /*returns integers, not percentage*/

  /*now run checks on the user input to check what input
  `   was input and if so, then what widget (if any)
  `   gets/had the input
  */

  /*when finished draw the new GUI*/
  done_fun07_sec0x:;
     draw_gui_ftbRayST(*guiSTPtr);
}
```

# ftbRayLib

The ftbRayLib file has just the main function for
  handeling my GUI. When setup properly all you need is
  the strucure, the initialization function, the event
  loop, the close window function, and the structure
  freeing function.

```
#include <raylib.h>
#include "ftbRayST.h" /*you set this up*/

/*hidden libraries that need to be included in the
` compiler commands
` - ../genLib/ulCp.h in
` - ftbWidg.h
*/

int
main(
){
   struct gui_ftbRayST guiStackST;
   init_gui_ftbRayST(&guiStackST); /*initialiaze gui*/
   
   while( ! WindowShouldClose() ) /*run till GUI closed*/
      checkRunEvent_ftbRayST(&guiStackST);
      /*handels all events and redraws everything*/
   CloseWindow(); /*raylibs finish GUI*/
   
   /*free memory in the GUI structure*/
   freeStack_gui_ftbRayST(&guiStackST);
   return 0;
}
```
