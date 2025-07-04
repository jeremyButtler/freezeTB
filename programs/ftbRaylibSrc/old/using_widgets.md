# Goal:

Give an overview of using the widgets.c file.

# Use:

Widgets is my extra functions I used with raylib to make
  the freeze TB GUI. It is limited and is not very great.
  This guide is here incase you disagree with me.

Why does this exist. Some of it is my goal to try to
  document my code as I go. Another part of it is my
  experince trying to pick up raylib.

# Working wit raylib

Maybe I will expand this out later to functions, but
  probably not.

If you are new to raylib, then here is were you can start.
  It is were I started. The cheat sheet has a list of
  functions and one liner description of what they do
  [https://www.raylib.com/cheatsheet/cheatsheet.html](
   https://www.raylib.com/cheatsheet/cheatsheet.html).
  To get an idea of the work flow you need to check the
  examples they provide
  [https://github.com/raysan5/raylib/tree/master/examples](
   https://github.com/raysan5/raylib/tree/master/examplesL).
  These are often simple enough you can figure out.

# Compiling:

Most of this, if not all is straight from the raylib wiki.

## Linux:

You will have to install the Linux required dependencies
  for raylib. Check
  [https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux](
   https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)
  for how to install the dependencies. I will eventually
  copy them here, but for now I am to lazy.

You will need to compile and install `raylib`. Also, make
  sure `raylib.h` is in `../raylib/src`. The .h file
  assumes raylib is one directory up. Also this program
  will need at least my own ulCp library (not very great).

For Linux, you should complie raylib as a shared library.
  Otherwise, you will have issues with not having a static
  `GL` library on your system. If you can get that fixed,
  then go ahead with static.

```
cd ../raylib/src
make PLATFORM=PLATFORM_DESKTOP RAYLIB_TYPE=SHARED
cd ../../<your_program_directory>
```

Then compile your code
  with `gcc -o <out> <your_progam.c> ../genLib/ulCp.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11`

## Mac:

For Mac (I need to test) it should be.

```
cd ../raylib/src;
export MACOSX_DEPLOYMENT_TARGET=10.9;
xcode-select --install
make PLATFORM=PLATFOMR_DESKTOP
cd ../../<your_program_directory>
```

Then to complile your own program you will follow raylibs
  instructions, but also include `-framework Foundation`.
  This is needed to detect dark and light mode.

`clang -o <out> -framework Foundation -framework CoreVideo -framework OIKit -framework Cocoa -framework GLUT -framework OpenGL ../raylib/src/librarylib.a <your_program.c> ../genLib/ulCp.c`

The raylib wiki suggest you check with `otool -l <out>` to
  confirm the Mac version and `otool -L <out> to make sure
  all libraries are staticlly compliled in. The suggest
  `otool -L` should only point to `/usr/lib/`
  or `/System/Library`.

# Global variables:

There are several global variables for colors or for font
  sizes you can use. You need to call raylib's GetColor()
  to use these on your own. However, many of my functions
  call these colors and the dark/light mode check will
  modify these colors.

- glob_fontSizeSI_widgets
  - font size to use with raylibs font
  - default is 15
- glob_charHeightSI_widgets
  - height of a single character;
  - for raylibs default font it is the font size. So, if
    you are using raylibs default font, you need to keep
    this in sync with glob_fontSizeSI_widgets
- glob_textColSI_widgets
  - hex code for the default text color
- glob_textAltColSI_widgets
  - hex code for text color when a state is changed
  - this is for the button pressed event
- glob_guiColSI_widgets
  - hex code for background color of GUI
- glob_butColSI_widgets
  - hex code for the color of a button
- glob_butBorderColSI_widgets
  - hex code for a buttons border color
- glob_butPressColSI_widgets
  - hex code for the color of a button when it is pressed
- glob_butPressBorderColSI_widgets
  - hex code for a buttons border when the button is
   pressed
- glob_entryColSI_widgets
  - hex code code color for an entry box color
- glob_entryBorderColSI_widgets
  -  hex code code color for an entry box border

These colors are changed in Mac or Windows when you GUI
  detects dark mode or light mode being turned on or off.
  You detect light and dark mode and have the sytem
  change colors with the checkGuiColorMode_widgets
  function.

In addtions to the colors for widgets, there are also
  the defiend colors for each widget. These colors do
  not change and are the hex codes for raylib. You need
  to call raylib's GetColor(defined_color) to convert this
  to a color for raylib. I will likey add more colors in
  when I get the GUI to a more polished state.

- def_lightGrey_widgets
  - light grey color
  - light mode is the normal button
  - dark mode is the text alternative color
    - this is shown during a button press
- def_darkGrey_widgets
  - a dark grey color
  - light mode is a button press color
  - dark mode is the button color
- def_white_widgets
  - white
  - light mode is background color
  - dark mode is the text color
- def_black_widgets
  - a very dark grey or black
  - light mode is the text color
  - dark mode is the backgroud color

For buttons, you can have several states. For mouse events
  these are button is pressed (def_press_widgets) or
  button is released (def_release_widgets). You can also
  hide a widget using the def_hiden_widgets state or make
  a widget non-interactive (seen, but does nothing) with
  def_inactive_state. Finally, the normal (unused state)
  of any widget is def_normal_widgets.

The three mouse buttons are redifined to def_left_widgets
  (left mouse button), def_right_widgets (right mouse
  button), and def_wheel_widgets (middle mouse wheel).

# Structures:

The widgets.h file uses two structures, the root_widgets
  for the entire GUI tree and the st_widgets strucutre.
  This structure stores a widgets loation, and size. It
  also stores the state and what kind of events this
  widget responds to.

This tree is not used to run the functions, but instead
  is used to draw/refresh the GUI.

## root_widgets structrure

This structure represents the GUI window. It is always
  the root node of a widget tree. It also keeps an
  internal array of all the widgets sorted by id for
  quick look ups.

## st_widgets structure

This holds a single widget and all its child widgets. It
  is built into a tree for widget transversal.

- idSI:
  - number assigned to the widget
- xSI
  - has x coordinate of the widget
- ySI
  - has y coordinate of the widget
- widthSI
  - has the width of the widget
  - some functions, such as the button function, may
    change this value
- heightSI
  - has the height of the widget
  - some functions, such as the button function, may
    change this value
- stateSC
  - has the current states of the widget
  - `def_press_widgets`
  - `def_release_widgets`
  - `def_normal_widgets`
  - `def_inactive_widgets`
  - `def_hiden_widgets`
- leftEventSC 
  - has the left clicks events the widget responds to
  - 0 no events
  - `def_press_widgets`
  - `def_release_widgets`
- rightEventSC 
  - has the right clicks events the widget responds to
  - 0 no events
  - `def_press_widgets`
  - `def_release_widgets`
- wheelBl
  - if the widget responds to scroll wheel movementsj
    - 1 the widget responds to scroll wheel movement
    - 0 the widget does not

**widget contents**

- strAryST:
  - str_ptrAry from ptrAry with all strings the widget
    needs to function and be printed
- arySI:
  - array of ints. Can be used as index's for list boxes.
  - aryLenSI has number of elements in the array
  - arySizeSI has number of max elements in the array
    before resizing
- indexSI:
  - index at in int array or string array

**grid mode**

You should not mess with these values directly, as they
  will mess up all

- gridBl:
  - if this widget displays contents in grid mode
  - xSI and ySI becomes the column and rows in the grid
    and the true x and y coordinates are back calculated
    from the original parent
  - once this is set for the parent, then all child
    widgets need to be set to grid mode
- colWidthArySI:
  - width of off all the columns in a parent
- colHeightArySI:
  - height of all the columns in a parent
- colLenSI:
  - number of columns in the widget
  - colSizeSI is the size of the column arrays
- rowNumSI:
  - number of rows in the widget

**values you should not mess with directly**

I would not recommend messing with these values, since
  they often can mess up stuff.


- par:
  - pointer to closest parent widget that this widget
    belongs to
  - the root node is a different struct, so if par == 0,
    then you are at the top level of the GUI
- child:
  - pointer to the first widget that belongs to this
    widget
- next:
  - move to next widget in list

Other then the arrays, there are the sizeSI and lenSI
  variables. SizeSI is the the array size, which is the
  maximum widgets that can be stored before I need to
  resize the array. LenSI is the number of widgets
  currently in the array. As a rule, do not mess with
  these.

# Flow:

## random stuff

You can check if the user has switched between dark and
  light mode using getDarkModeState_widgets(). There is
  no input. The return is def_darkMode_widgets for dark
  mode, def_lightMode_widgets for light mode, or
  def_unkownMode_widgets if dark or light mode could
  not be checked.

You can have widgets auto set the dark or light pallete
  by using checkGuiColorMode_widgets. This assumes you
  are using the default color pallate.

## setup and clean up

### initialize (setup and clean up)

The first step is to initialize a st_widgets structure.
  You can do this with init_st_widgets(). The input is a
  st_widgets structure pointer. This will set all arrays
  to zero, so only call this once per structure.

```
struct st_wdigets widgetsStackST;
init_st_widgets(&widgetsStackST);
```

### freeing (setup and clean up)

You can free a stack allocated st_widgets structure using
  freeStack_st_widgets. This will free all arrays and
  re-initialize the structure for you.

```
struct st_wdigets widgetsStackST;
init_st_widgets(&widgetsStackST);

/*you code goes here*/
freeStack_st_widgets(widgetsStackST);
```

Another option is freeHeap_st_widgets if ou declared your
  structure on the heap. Just remember to set your
  structure to null when you are done.

```
struct st_wdigets *widgetsHeapST = 0;
widgetsHeapST = malloc(sizeof(struct st_widgets));

if(! widgetsHeapST)
   /*deal with memory error*/
init_st_widgets(&widgetsStackST);

/*you code goes here*/
freeHeap_st_widgets(widgetsHeapST);
widgetsHeapST = 0;
```

## adding widgets

The next step is to add a widget to the st_widgets
  structure. Do not worry about defining what it is yet,
  that is done later. You can do this with
  addWidget_st_widgets().

### input (adding widgets)

- X-coordiante
  -  integer greater then -1
- Y-coordinate
  - integer greater then -1
- does this respond to left press events
  - 0 for no event
  - `def_press_widgets` for left mouse button press events
     only
  - `def_release_widgets` for left mouse button release
     events only
  - `def_press_widgets | def_release_widgets` for both
     left mouse button press and release events
- does this respond to right press events
  - 0 for no event
  - `def_press_widgets` for right mouse button press events
     only
  - `def_release_widgets` for right mouse button release
     events only
  - `def_press_widgets | def_release_widgets` for both
     right mouse button press and release events
- if the widget should respond to the scroll wheel
  - 0 no do not respond  to scroll wheel movement
  - 1 respond to scroll wheel movement
  - anything else, assume 1, may change if needed later
- a st_widgets structure to add the new widget to
  - as a st_widgets pointer

### output (adding widgets)

- index of the added widget
  - this is your only handle on the widget, so do not
    loose it
- -1 if you had a memory error, or you messed up sizeSI
  or lenSI in the st_widgets struct
- -2 if input was invalid (bounds error or bad left clikc
  event or right click event input)

### example (adding wdigets)

```
signed int firstWidgetSI = 0;
struct st_wdigets widgetsStackST;
init_st_widgets(&widgetsStackST);

firstWidgetSI =
   addWidget_st_widgets(
      15,  /*X coordiante*/
      100, /*y coordiante*/
      def_release_widgets,/*report on left mouse release*/
      0,   /*ignore right clicks*/
      0,   /*ingore the scroll wheel*/
      &widgetsStackST
   );
```

## modifing a wigets

### changing state (modifing widgets)

You can modify the state of a widget using the
  changeState_st_widgets. You can not change the pressed
  or release states, but the other states you can.

#### Input:

- is the state to change the widget to
  - `def_normal_widgets` if widget should respond to user
     input
  - `def_inactive_widgets` if widget should do nothing
  - `def_hiden_widgets` if widget should not be shown
- index of widget to change state for
- st_widgets structure with widget to modify state of
  - as a pointer

## assign type
