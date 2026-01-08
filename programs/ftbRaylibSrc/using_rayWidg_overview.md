# Goal

Give an idea of how to use the rayWidg system to construct
  and build a gui.

The examples in here (other then the final) have not
  been tested. There are here to give an idea of how
  to use rayWidg.

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
  figuring what to do when an event happened. Querying
  functions are used to find the id of the clicked/enter
  widget. You then have to set up the functions to handle
  what the event does.

Also, you have to set up what to draw, however, there
  are widget functions to help you here. They handle
  drawing the widgts, for more complicated widgets, they
  help out with event handeling. They can also auto scale
  text and GUI size.

The default settings are set to assume the smallest
  realisitic smart phone screen size (420x320) in pixels.
  The font size is set to 20 (larger for HDPI). The color
  theme is limited to two colors to acheive a contrast
  ratio of 1:7.

# Quick guide

For a quick (for me) guide on how to get started see
  the `using_rayWidg_quick.md` file. This should walk you
  through a minimal example with only a label.

# Structures

There are several structures used to manage the rayWidg
  system.

- widg\_rayWidg is the widget orginazation structure, it
  holds all the details (coordiantes, width, height, and
  state) needed for a general widget managment
- event\_rayWidg holds the data for a single event and
  is generally used with the more complex widgets
- listBox\_rayWidg is a structure that is used for listbox
  widgets and widgets that use list boxes, such as the
  file browser
- files\_rayWidg is a structure that is used with the file
  browser functions (to much to pass around)

You will find details for the widg\_rayWidg structure in
  `using_rayWidg_widg.md`. For the event\_rayWidg, see
  `using_rayWidg_events.md`. For the list box structure
  see `using_rayWidg_listBox.md`. Finally, for the
  files\_rayWidg see `using_rayWidg_fileBrowser.md`.

# Widgets:

Currently rayWidg supports the button, label, entry box,
  message box, list box, and file browser widgets. It also
  supports drawing a rectangle (as a widget or not) and
  drawing text (as non-widget).

- Each widget has its own readme file.
  - the button is in the `using_rayWidg_button.md` file
  - the label is in the `using_rayWidg_label.md` file
  - the entry box is in the `using_rayWidg_entryBox.md`
    file
  - the message box is in
    the `using_rayWidg_messageBox.md` file
  - the message box is in
    the `using_rayWidg_messageBox.md` file
  - the list box is in the `using_rayWidg_listBox.md` file
  - the file browser is in
    the `using_rayWidg_fileBrowser.md` file
  - text and rectangles are in
    the `using_raywidg_miscWidgets.md` file
