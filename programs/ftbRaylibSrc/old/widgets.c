/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' widgets SOF: Start Of File
'   - widgest using with raylib for freezeTB
'   o header:
'     - included libraries
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
'   o fun18: drawBut_widgets
'     - draw a button
'   o fun19: checkIfClick_widgets
'     - finds if a widget was clicked and returns the
'       widget id the mouse was on
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   /*never should use, but for good habit. raylib will
   `   not work on plan9
   */
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../raylib/src/raylib.h"
#include "../genLib/ulCp.h"


/*for dark mode detection and other OS specific stuff*/
#ifdef MAC
   #import <Foundation/Foundation.h>
#endif

#ifdef WINDOWS
   #include <windows.h>
#endif

/*-------------------------------------------------------\
| Fun01: blank_st_widgets
|   - blanks all non-tree pointer values in st_widgets
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
){
   if(! widgetsSTPtr)
      return;

   widgetsSTPtr->xSI = 0;
   widgetsSTPtr->ySI = 0;
   widgetsSTPtr->widthSI = 0;
   widgetsSTPtr->heightSI = 0;

   widgetsSTPtr->stateSC = 0;
   widgetsSTPtr->leftEventSC = 0;
   widgetsSTPtr->rightEventSC = 0;
   widgetsSTPtr->wheelBl = 0;

   widgetsSTPtr->colLenSI = 0;
   widgetsSTPtr->rowNumSI = 0;
   widgetsSTPtr->gridBl = 0

   blank_str_ptrAyr(widgetsSTPtr->strAryST);
   widgetsSTPtr->aryLenSI = 0;
   widgetsSTPtr->indexSI = 0;
} /*blank_st_widgets*/

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
){
   if(! widgetsSTPtr)
      return;

   widgetsSTPtr->idSI = -1;

   widgetsSTPtr->colWidthArySI = 0;
   widgetsSTPtr->colHeightArySI = 0;
   widgetsSTPtr->colSizeSI = 0;

   widgetsSTPtr->strAryST = 0;
   widgetsSTPtr->arySI = 0;
   widgetsSTPtr->arySizeSI = 0;

   widgetsSTPtr->par = 0;
   widgetsSTPtr->child = 0;
   widgetsSTPtr->last = 0;
   widgetsSTPtr->next = 0;

   blank_st_widgets(widgetsSTPtr);
} /*init_st_widgets*/

/*-------------------------------------------------------\
| Fun03: freeStack_st_widgets
|   - frees variables (except par) in a st_widgets struct
|     and returns the next struture in the list
| Input:
|   - widgetsSTPtr:
|     o st_widgets struct pointer with arrays to free
| Output:
|   - Modifies:
|     o frees all array pointers and sets to null (0)
|     o lenSI and size SI to be 0
|   - Returns:
|     o next widget in the list
| Warning:
|   - does not free par pointers
\-------------------------------------------------------*/
struct st_wigdets *
freeStack_st_widgets(
   struct st_widgets *widgetsSTPtr
){
   struct st_widgets *delSTPtr = 0;

   if(! widgetsSTPtr)
      return;

   if(widgetsSTPtr->colWidthArySI)
      free(widgetsSTPtr->colWidthArySI)
   if(widgetsSTPtr->colHeightArySI)
      free(widgetsSTPtr->colHeightArySI)
   if(widgetsSTPtr->strAryST)
      freeHeap_str_ptrAry(widgetsSTPtr->strAryST);

   *delSTPtr = widgetsSTPtr->child;

   while(delSTPtr)
      delSTPtr = freeStack_st_widgets(delSTPtr->child);
      /*recursion call, but makes sure all children are
      `   freeded
      */

   /*not freeing sub trees or children, because I am
   `  assuming the user already freeded them
   */
   delSTPtr = widgetsSTPtr->next;
   init_st_widgets(widgetsSTPtr);
   return delSTPtr;
} /*freeStack_st_widgets*/

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
){
   struct st_widgets *nextSTPtr = 0;
   if(! widgetsSTPtr)
      return;
   nextSTPtr = freeStack_st_widgets(widgetsSTPtr);
   free(widgetsSTPtr);
   return nextSTPtr;
} /*freeHeap_st_widgets*/

/*-------------------------------------------------------\
| Fun05: blank_root_widgets
|   - blanks a root_widgets structure
| Input:
|   - rootSTPtr:
|     o root_widgets struct pointer to blank
| Output:
|   - Modifies:
|     o xSI rootSTPtr to be half of
|       screen - def_windowWidth_widgets / 2
|     o ySI rootSTPtr to be half of
|       screen - def_windowHeight_widgets / 2
|     o widthSI in rootSTPtr to be def_windowWidth_widgets
|     o heightSI in rootSTPtr to def_windowHeight_widgets
\-------------------------------------------------------*/
void
blank_root_widgets(
   struct root_widgets *rootSTPtr
){
   if(! rootSTPtr)
      return;
   rootSTPtr->widthSI 
} /*blank_root_widgets*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun0x TOC:
   '   - adds a child widget to a st_widgets struct
   '   o fun0x sec01:
   '     - variable declarations 
   '   o fun0x sec02:
   '     - check if have id clash or free ids to assign
   '   o fun0x sec03:
   '     - if need to add id at end
   '   o fun0x sec04:
   '     - add child to root and parent
   '   o fun0x sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siId = 0;
   struct st_widgets *nextSTPtr = 0;
   struct st_widgets *parSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec02:
   ^   - check if have id clash or free ids to assign
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(childSTPtr->next)
      goto idErr_fun0x_sec05;/*child is in tree somewere*/

   if(childSTPtr->idSI < 0)
      ; /*no id assigned yet*/
   else if(childSTPtr->idSI > rootSTPtr->lenSI)
      ; /*id not in root tree*/
   else
      goto idErr_fun0x_sec05;

   if(parIdSI < 0)
      goto parErr_fun0x_sec05;
   else if(parIdSI >= rootSTPtr->idAryST)
      goto parErr_fun0x_sec05;
   else if(! rootSTPtr->idAryST[parIdSI])
      goto parErr_fun0x_sec05;

   if(! rootSTPtr->openIdsArySI)
      ;
   else if(rootSTPtr->openIdsLeftSI <= 0)
      ;
   else
   { /*Else: have an open id*/
      childSTPtr->idSI = rootSTPtr->openIdsArySI[0];
      --childStPtr->openIdsLeftSI;

      for(siId=0; siId < childStPtr->openIdsLeftSI;++siId)
         childSTPtr->openIdsArySI[siId] =
            childStPtr->openIdsArySI[siId + 1];
      goto addChild_fun0x_sec04;
   } /*Else: have an open id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec03:
   ^   - if need to add id at end
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(rootSTPtr->lenSI >= rootSTPtr->sizeSI)
   { /*If: need more id array memory*/
      siId = rootSTPtr->sizeSI + (rootSTPtr->sizeSI >> 1);

      if(rootSTPtr->openIdsArySI)
         free(rootSTPtr->openIdsArySI);
      rootSTPtr->openIdsArySI = 0;
      
      rootSTPtr->openIdsArySI =
         malloc(siId * sizeof(signed int));
      if(! rootSTPtr->openIdsArySI)
         goto memErr_fun0x_sec05

      if(! rootSTPtr->idAryST)
      { /*If: need to get initial memory*/
         rootSTPtr->idAryST = 
            malloc(siId * sizeof(struct st_widgets));
         if(! rootSTPtr->idAryST)
            goto memErr_fun0x_sec05
      } /*If: need to get initial memory*/

      else
      { /*Else: need to reallocate memory*/
         parSTPtr =
            realloc(
               rootSTPtr->idAryST,
               siId * sizeof(struct st_widgets)
            );
         if(! nextSTPtr)
            goto memErr_fun0x_sec05
         rootSTPtr->idAryST = parSTPtr;
         parSTPtr = 0;
      } /*Else: need to reallocate memory*/
   } /*If: need more id array memory*/

   /*assing length is new id*/
   childSTPtr->idSI = rootSTPtr->lenSI;
   ++rootSTPtr->lenSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec04:
   ^   - add child to root and parent
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   addChild_fun0x_sec04:;

   rootSTPtr->idAryST[childSTPtr->idSI] = childSTPtr;
   parSTPtr = rootSTPtr->idAryST[parIdSI];

   if(! parSTPtr->next)
     parSTPtr->next = childSTPtr;
   else
   { /*Else: need to insert the child in*/
      childSTPtr->next = parSTPtr->next;
      parSTPtr->next = childSTPtr;
   } /*Else: need to insert the child in*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return childStPtr->idSI;

   idErr_fun0x_sec05:;
      return -1;

   parErr_fun0x_sec05:;
      return -2;

   memErr_fun0x_sec05:;
      return -4;
} /*addChild_st_widgets*/

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
){
   #ifdef MAC
      @autoreleasepool {
         NSString *style =
            [
               [NSUserDefaults standardUserDefaults]
               stringForKey:@"AppleInterfaceStyle"
            ];
         if(! style)
            goto retUnkown_fun01;
         else if (
               [style caseInsensitiveCompare:@"Dark"]
            == NSOrderedSame
         ) goto retDark_fun01;
         else if (
         else
            goto retLight_fun01;
       } /*autoreleasepool block from objective C*/

   #endif

   #ifdef WINDOWS
      HKEY hKey;
      DWORD value = 1;
      DWORD valueSize = sizeof(value);

      /*Open the registry key*/
      if(
         RegOpenKeyExA(
            HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
            0,
            KEY_READ,
            &hKey
         ) == ERROR_SUCCESS
      ){ /*If: can open theme registory*/
          if(
             RegQueryValueExA(
                hKey,
                "AppsUseLightTheme",
                NULL,
                NULL,
                (LPBYTE)&value,
                &valueSize
              ) == ERROR_SUCCESS
          ){ /*If: could get dark mode status*/
              RegCloseKey(hKey);

              if (value == 0)
                 goto retDark_fun01;
              else
                 goto retLight_fun01;
          }  /*If: could get dark mode status*/

          RegCloseKey(hKey);
      } /*If: can open theme registory*/
   #endif

   goto retUnkown_fun01; /*could not get dark/light mode*/

   retUnkown_fun01:;
      return def_unkownMode_widgets;

   retLight_fun01:;
      return def_lightMode_widgets;

   retDark_fun01:;
      return def_darkMode_widgets;
} /*getDarkModeState_widgets*/

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
){
   /*TODO: need elevation change and border shadow*/
   if(getDarkModeState_widgets() == def_darkMode_widgets)
   { /*If: dark mode is set*/
      glob_textColSI_widgets = def_white_widgets;
      glob_textAltColSI_widgets = def_lightGrey_widgets;
      glob_guiColSI_widgets = def_black_widgets;

      glob_butColSI_widgets = def_darkGrey_widgets;
      glob_butBorderColSI_widgets = def_lightGrey_widgets;
      glob_butPressColSI_widgets = def_darkGrey_widgets;
      glob_butPressBorderColSI_widgets =
         def_lightGrey_widgets;

      glob_entryColSI_widgets = def_darkGrey_widgets;
      glob_entryBorderColSI_widgets=def_lightGrey_widgets;
   } /*If: dark mode is set*/

   else
   { /*Else: light mode is set*/
      glob_textColSI_widgets = def_black_widgets;
      glob_textAltColSI_widgets = def_darkGrey_widgets;
      glob_guiColSI_widgets = def_white_widgets;

      glob_butColSI_widgets=def_lightGrey_widgets;
      glob_butBorderColSI_widgets = def_darkGrey_widgets;
      glob_butPressColSI_widgets = def_darkGrey_widgets;
      glob_butPressBorderColSI_widgets =
         def_lightGrey_widgets;

      glob_entryColSI_widgets = def_lightGrey_widgets;
      glob_entryBorderColSI_widgets=def_darkGrey_widgets;
   } /*Else: light mode is set*/
} /*checkGuiColorMode_widgets*/


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
){
   if(! (stateSC & def_normal_widgets) )
       ;
   else(! (stateSC & def_press_widgets) )
       ;
   else(! (stateSC & def_inactive_widgets) )
       ;
   else(! (stateSC & def_hiden_widgets) )
       ;
   else
      goto badState_fun18;
      /*either invalid state or multiple states*/

   if(idSI < 0)
      goto badIndex_fun18;
   else if(idSI >= rootSTPtr->lenSI)
      goto badIndex_fun18;
   else if(! rootStPtr->idAryST[idSI])
      goto badIndex_fun18;

   rootSTPtr->idAryST[idSI]->stateArySC = stateSC;

   return 0;

   badState_fun18:;
      return -1;
   badIndex_fun18:;
      return -2;
} /*changeState_widgets*/


/*-------------------------------------------------------\
| Fun18: drawBut_widgets
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
|      o number of widget
\-------------------------------------------------------*/
signed int
drawBut_widgets(
   signed int maxWidthSI, /*maximum width of button*/
   signed int xSI,        /*x coordinate of button*/
   signed int ySI,        /*y coordinate of button*/
   signed char stateSC,   /*state button is in*/
   signed char *textStr,  /*text to print on button*/
   struct st_widgets *widgetSTPtr /*store widget in*/
   if(widgetSTPtr->lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - draw a button
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec02:
   '     - get color of button (state in)
   '   o fun18 sec03:
   '     - get width of button
   '   o fun18 sec04:
   '     - draw button
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct Color butCol;
   struct Color butBorderCol;
   struct Color textCol;

   #define def_maxButText_fun18 32
   signed char writeStr[def_maxButText_fun18 + 8];
   signed int widthSI = 0;
   signed int oneCharSI=MeasureText("a", glob_fontSizeSI);

   /*these are for shorting strings*/
   signed char histUC = 0;
   signed int histIndexSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - get color of button (state in)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(stateSC == def_hiden_widgets)
       return 0; /*button is hidden*/

   else if(stateSC == def_inactive_widgets)
   { /*If: button is in a disabled state*/
      butCol = GetColor(glob_guiColSI_widgets);
      butBorderCol=GetColor(glob_butBorderColSI_widgets);
      textCol = GetColor(glob_textColSI_widgets);
   } /*If: button is in a disabled state*/

   else if(stateSC == def_press_widgets)
   { /*If: button is in a press state*/
      butCol = GetColor(glob_butPressColSI_widgets);
      butBorderCol =
         GetColor(glob_butPressBorderColSI_widgets);
      textCol = GetColor(glob_textAltColSI_widgets);
   } /*If: button is in a press state*/

   else
   { /*If: button is in a normal state*/
      butCol = GetColor(glob_butColSI_widgets);
      butBorderCol=GetColor(glob_butBorderColSI_widgets);
      textCol = GetColor(glob_textColSI_widgets);
   } /*If: button is in a normal state*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - get width of button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! textStr)
   { /*If: no text input*/
      widthSI = 0; /*no text input*/
      writeStr[0] = 0;
   } /*If: no text input*/

   else
   { /*Else: have text to use*/
      widthSI = endStr_ulCp(textStr);

      if(widthSI > def_maxButText_fun18)
         widthSI = def_maxButText_fun18;

      cpLen_ulCp(writeStr, textStr, widthSI);
      histIndexSI = widthSI;
      widthSI =
         MeasureText((char *) writeStr, glob_fontSizeSI);
   } /*Else: have text to use*/

   if(widthSI > maxWidthSI)
   { /*Else If: I can not draw the full button*/
      histUC = 0;

      while(widthSI > maxWidthSI)
      { /*Loop: find number of bytes*/
         writeStr[histIndexSI] = histUC;

         --histIndexSI;
         histUC = writeStr[histIndexSI];
         writeStr[histIndexSI] = 0;

         widthSI =
            MeasureText(
               (char *) writeStr,
               glob_fontSizeSI
            );
      } /*Loop: find number of bytes*/

      if(histIndexSI >= 3)
      { /*If: can copy in ...*/
         writeStr[histIndexSI - 1] = '.';
         writeStr[histIndexSI - 2] = '.';
         writeStr[histIndexSI - 3] = '.';
      } /*If: can copy in ...*/
   } /*Else If: I can not draw the full button*/

   if(! widthSI)
      widthSI = MeasureText("a", glob_fontSizeSI);
         /*button needs to be at least 1 char*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - draw button
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*TODO: add in functions for Mac rounded edges*/
   DrawRectangle(
      xSI,
      ySI,
      widthSI + (oneCharSI << 1), /*want 1 character pad*/
      def_height_widgets,
      butCol
   );

   if(textStr)
      DrawText(
         (char *) writeStr,
         xSI + oneCharSI,          /*center horizontally*/
         ySI + (def_height_widgets >> 1), /*y center*/
         glob_fontSizeSI_widgets,
         textCol
      );

   DrawRectangleLines(
      xSI,
      ySI,
      widthSI + (oneCharSI << 1), /*want 1 character pad*/
      def_height_widgets,
      butBorderCol
   );

   return widthSI + (oneCharSI << 1);
} /*drawBut_widgets*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - checks if the input widget was clicked
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - get mouse events
   '   o fun19 sec03:
   '     - detect if even was on a widget
   '   o fun19 sec04:
   '     - check if clicked on widget
   '   o fun19 sec05:
   '     - return the found event
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct st_widgets *widgetSTPtr = 0;
   signed int xSI = 0; /*x position of mouse click*/
   signed int ySI = 0; /*y position of mouse click*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - get mouse events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   xSI = GetMouseX();
   ySI = GetMousey();

   if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      *leftClickSCPtr = def_press_widgets;
   else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
      *leftClickSCPtr |= def_release_widgets;

   if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
      *rightClickSCPtr = def_press_widgets;
   else if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
      *rightClickSCPtr = def_release_widgets;

   *scrollFPtr = GetMouseWheelMove();

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - find widget mouse was on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   widgetSTPtr = widgetSTPtr->widgets;

   while(widgetSTPtr)
   { /*Loop: check all active widgets*/
      findWidget_fun19_sec03:;

      if(xSI < widgetSTPtr->xSI)
         ;
      else if(
         xSI > widgetSTPtr->xSI + widgetSTPtr->widthSI
      ) ;
      else if(ySI < widgetSTPtr->ySI)
         ;
      else if(
         ySI > widgetSTPtr->ySI + widgetSTPtr->heightSI
      ) ;

      else
         break; /*found the widget the mouse was on*/

      widgetSTPtr = widgetSTPtr->next;
   } /*Loop: check all active widgets*/

   if(! widgetSTPtr)
      goto noWidget_fun19_sec05; 

   if(widgetSTPtr->child)
   { /*If: widget has child widgets*/
      /*in this case the event happened in a container,
      `   so I need to find the actual widget that had
      `   the event
      */
      widgetSTPtr = widgetSTPtr->child;
      findWidget_fun19_sec03;
   } /*If: widget has child widgets*/


   /*set return for found events*/
   *leftClickSCPtr &= widgetSTPtr->leftEventSC;
   *rightClickSCPtr &= widgetSTPtr->rightEventSC;

   if(! widgetSTPtr->wheelBl)
      *scrollFPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec05:
   ^   - return the found event
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return widgetSTPtr->idSI;

   noWidget_fun19_sec05:;
      *leftClickSCPtr = 0;
      *rightClickSCPtr = 0;
      *scrollFPtr = 0;
      return -1;
} /*checkIfClick_widgets*/

/*-------------------------------------------------------\
| Fun20: fileBrowser_widgets
|   - opens file browser (takes up entire main GUI)
| Input:
|   - xSI:
|     o x coordinate
|   - ySI:
|     o y coordinate
|   - widthSI:
|     o width of window
|   - heightSI:
|     o width of window
|   - filterAryStr:
|     o c-string array with allowd file extensions (file
|       types) for user to choose
|     o use 0 for no filters
|   - filterLenSI:
|     o number of filters in filterAryStr
|     o use 0 for no filters
|   - maxFilesSI:
|     o maximum files for user to choose
|     o use <= 0 for any number
|   - titleStr:
|     o c-string with browser tile (title) to print
|     o limited to 64 chars before "..." is used
|   - fileLenSIPtr:
|     o signed int pointer to get number of files choosen
| Output:
|   - Returns:
|     o list of files selected
\-------------------------------------------------------*/
signed char *char[]
fileBrowser_widgets(
   signed int xSI,          /*x coordinate of window*/
   signed int ySI,          /*y coordinate of window*/
   signed int widthSI,      /*width of window*/
   signed int heightSI,     /*height of window*/
   signed char *filterAryStr[], /*extensions to use*/
   signed int filterLenSI,  /*number of filters to use*/
   signed int maxFilesSI,   /*max files user can choose*/
   signed char *titleStr,   /*title of window*/
   signed int *fileLenSIPtr /*gets number files picked*/
){
} /*fil[eBrowser_widgets*/
