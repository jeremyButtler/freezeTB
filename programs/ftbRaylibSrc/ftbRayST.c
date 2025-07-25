/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hardFtbWdig SOF: Start Of File
'   - has the hardcoded parts of the rayWidg that is
'     needed for ftbRaylib
'     * initization for raylib
'     * check and run events (click/keyboard)
'     * rayWidg gui structure for drawing function
'   o header:
'     - included libraries and global variables
'   o .h st01: gui_ftbRayST
'     - has widgets used in the ftbRaylib GUI
'     - TODO: add output gui
'   o fun01: blank_gui_ftbRayST
'     - blanks a gui_ftbRayST structure to default values
'   o fun02: init_gui_ftBrayST
'     - initialize a gui_ftbrayst structure to default
'       values (really just blank)
'   o fun03: freeStack_gui_ftbRayST
'     - free variables in a gui_ftbRayST
'   o fun04: freeHeap_gui_ftbRayST
'     - frees a gui_ftbRayST structure
'   o .c fun05: drawGUI_ftbRayST
'     - draws the gui for a gui_ftbRayST structure
'   o fun06: initRayGui_ftbRayST
'     - initialize raylibs GUI
'   o fun07: checkRunEvent_ftbRayST
'    - checks for an event, and if can runs found event
'    - also redraws the GUI
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and global variables
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - global and defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../genLib/ulCp.h"

#include "../raylib/src/raylib.h"
#include "ftbRayST.h"
#include "rayWidg.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .h  #include "ibmPlexMono.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - global and defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*main window settings*/
#define def_winWidth_ftbRayST 400
#define def_winHeight_ftbRayST 200

#define def_maxWidgetWidth_ftbRayST 240

/*cursor blink settings*/
#define def_blinkInterval_ftbRayST 10
   /*how many frames for blink*/
#define def_blinkTime_ftbRayST (def_blinkInterval_ftbRayST>>1)
   /*how long a blink lasts; using
   `  (counter % def_interval) > def_blinkTime to find how
   `  long a blink lasts
   */

/*widget rows*/
#define def_xFqBut_ftbRayST 0
#define def_xPrefixLab_ftbRayST 1
#define def_xPrefixEntry_ftbRayST 1
#define def_xOutBut_ftbRayST 2
#define def_xConfigBut_ftbRayST 3
#define def_xIllumina_ftbRayST 4
#define def_xRunBut_ftbRayST 5

/*widget columns*/
#define def_yFqBut_ftbRayST 0
#define def_yPrefixLab_ftbRayST 0
#define def_yPrefixEntry_ftbRayST 1
#define def_yOutBut_ftbRayST 0
#define def_yConfigBut_ftbRayST 0
#define def_yIllumina_ftbRayST 0
#define def_yRunBut_ftbRayST 0

/*labels for each widget*/
static signed char *textFqButStr =
   (signed char *) "fastq files:";
static signed char *inputStr =
   (signed char *) "FTB Input:";
static signed char *outputStr =
   (signed char *) "FTB Output:";
static signed char *textConfigButStr =
   (signed char *) "Config File:";
/*static signed char *textIlluminaStr =
   (signed char *) "Short reads:"; TODO: add in later*/
static signed char *textPrefixLabStr =
   (signed char *) "Output Prefix:";
static signed char *textOutButStr =
   (signed char *) "Output Folder:";
static signed char *textRunButStr =
   (signed char *) "Run FTB:";

/*-------------------------------------------------------\
| Fun01: blank_gui_ftbRayST
|   - blanks a gui_ftbRayST structure to default values
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to blank
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
blank_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   guiSTPtr->mesgStr[0] = 0;

   guiSTPtr->inputBl = 1;
   guiSTPtr->inWinWidthSI = def_winWidth_ftbRayST;
   guiSTPtr->inWinHeightSI= def_winHeight_ftbRayST;
   guiSTPtr->focusSI = 5;
   guiSTPtr->blinkSC = 0;

   /*fastq button defaults*/
   guiSTPtr->fileCntSI = 0;

   /*prefix button defaults*/
   guiSTPtr->inPrefixStr[0] = 0;
   guiSTPtr->prefixPosArySI[0] = 0; /*scroll*/
   guiSTPtr->prefixPosArySI[1] = 0; /*cusor position*/
   guiSTPtr->prefixLenSI = 0;

   /*output button*/
   guiSTPtr->outDirStr[0] = 0;

   /*configuration file button*/
   guiSTPtr->configFileStr[0] = 0;
} /*blank_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun02: init_gui_ftbRayST
|   - initialize a gui_ftbRaySt structure to default
|     values (really just blank)
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to initialize
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
init_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   guiSTPtr->fileAryStr = 0;
   guiSTPtr->fileIndexArySI = 0;
   guiSTPtr->widgSTPtr = 0;

   guiSTPtr->mesgBoxIdSI = 0;
   guiSTPtr->fqButIdSI = 0;
   guiSTPtr->prefixLabIdSI = 0;
   guiSTPtr->prefixEntryIdSI = 0;
   guiSTPtr->outDirIdSI = 0;
   guiSTPtr->configIdSI = 0;
   guiSTPtr->runIdSI = 0;

   blank_gui_ftbRayST(guiSTPtr);
} /*init_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun03: freeStack_gui_ftbRayST
|   - free variables in a gui_ftbRayST
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with variables to free
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
freeStack_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   if(guiSTPtr->fileAryStr)
      free(guiSTPtr->fileAryStr);
   if(guiSTPtr->fileIndexArySI)
      free(guiSTPtr->fileIndexArySI);

   init_gui_ftbRayST(guiSTPtr);
} /*freeStack_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_gui_ftbRayST
|   - frees a gui_ftbRayST structure
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to free
| Output:
|   - Modifies:
|     o frees guiSTPtr, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   if(! guiSTPtr)
      return;

   freeStack_gui_ftbRayST(guiSTPtr);
   free(guiSTPtr);
} /*freeStack_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun05: drawGUI_ftbRayST
|   - draws the gui for a gui_ftbRayST structure
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui to draw
| Output:
|   - Draws:
|     o current GUI state to screen
\-------------------------------------------------------*/
void
draw_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){
   signed char blinkBl = 0; /*be in blink state*/
   struct Color guiCol =
      GetColor(guiSTPtr->widgSTPtr->guiColSI);
   signed int heightSI = 0;
   signed int widthSI = 0;

   if(guiSTPtr->inputBl)
   { /*If: displaying the input GUI*/
      heightSI = guiSTPtr->inWinHeightSI;
      widthSI = guiSTPtr->inWinWidthSI;
      SetWindowTitle((char *) inputStr);
   } /*If: displaying the input GUI*/

   else
   { /*Else: displaying the output GUI*/
      heightSI = guiSTPtr->outWinHeightSI;
      widthSI = guiSTPtr->outWinWidthSI;
      SetWindowTitle((char *) outputStr);
   } /*Else: displaying the output GUI*/

   SetWindowSize(widthSI, heightSI);

   /*check if blinking the cursor*/
   guiSTPtr->blinkSC =
      blinkGet_rayWidg(
         guiSTPtr->blinkSC,
         def_blinkInterval_ftbRayST,
         def_blinkTime_ftbRayST
      );
   ++guiSTPtr->blinkSC;


   BeginDrawing();
      ClearBackground(guiCol);

      /*check if light/dark*/
		checkGuiColorMode_rayWidg(guiSTPtr->widgSTPtr);

      /*for most of the draw functions, the return value
      `  is either, width, 0 for hidden buton, or
      `  def_noWidget_rayWidg if no widget is present.
      `  I already know all widgets exist and I do not
      `  care about  hidden widgets, so I am ignoring the
      `  return values
      */

      /*fastq file input*/
      butDraw_rayWidg(
         def_maxWidgetWidth_ftbRayST,
         0,                /*minumum width = any size*/
         guiSTPtr->fqButIdSI,  /*button id in array*/
         textFqButStr,
         0,                     /*drawing button*/
         guiSTPtr->widgSTPtr    /*has widgets to draw*/
      );

      /*prefix entry box*/
      labDraw_rayWidg(
         def_maxWidgetWidth_ftbRayST,
         0,                        /*no min width*/
         guiSTPtr->prefixLabIdSI,
         textPrefixLabStr,
         ' ',                      /*padding with spaces*/
         2,                        /*right pad if needed*/
         0,                        /*draw label*/
         guiSTPtr->widgSTPtr
      ); /*label for entry box*/

      entryDraw_rayWidg(
         def_widthPrefixEntry_ftbRayST,
         guiSTPtr->prefixEntryIdSI,
         guiSTPtr->prefixPosArySI, /*cursor + scroll pos*/
         blinkBl,                  /*blink cursor*/
         guiSTPtr->inPrefixStr,    /*current user input*/
         0,                        /*drawing coordinates*/
         guiSTPtr->widgSTPtr
      );

      /*output directory button*/
      butDraw_rayWidg(
         def_maxWidgetWidth_ftbRayST,
         0,                /*minumum width = any size*/
         guiSTPtr->outDirIdSI,  /*button id in array*/
         textOutButStr,
         0,                     /*drawing button*/
         guiSTPtr->widgSTPtr    /*has widgets to draw*/
      );

      /*TODO: add illumina check box*/

      /*confituration button*/
      butDraw_rayWidg(
         def_maxWidgetWidth_ftbRayST,
         0,                /*minumum width = any size*/
         guiSTPtr->configIdSI,  /*button id in array*/
         textConfigButStr,
         0,                     /*drawing button*/
         guiSTPtr->widgSTPtr    /*has widgets to draw*/
      );

      /*run button*/
      butDraw_rayWidg(
         def_maxWidgetWidth_ftbRayST,
         0,                /*minumum width = any size*/
         guiSTPtr->runIdSI,  /*button id in array*/
         textRunButStr,
         0,                     /*drawing button*/
         guiSTPtr->widgSTPtr    /*has widgets to draw*/
      );

      mesgBoxDraw_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         widthSI,
         heightSI,
         guiSTPtr->mesgStr,
         (signed char *) "Ok",
         guiSTPtr->widgSTPtr
      );
         
   EndDrawing();
} /*draw_gui_ftbRayST*/

/*-------------------------------------------------------\
| Fun06: initRayGui_ftbRayST
|   - makes a heap allocated gui_ftbRayST struct and draws
|     the initial GUI
| Input:
| Output:
|   - Draws:
|     o initial gui to screen
|   - Returns:
|     o heap allocated gui_ftbRayST struct with widgets
|     o 0 for memory errors
\-------------------------------------------------------*/
struct gui_ftbRayST *
mk_gui_ftbRayST(
   void
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '  - makes a heap allocated gui_ftbRayST struct and
   '    draws the initial GUI
   '  o fun06 sec01:
   '    - variable declarations
   '  o fun06 sec02:
   '    - add widgets
   '  o fun06 sec03:
   '    - draw initial GUI
   '  o fun06 sec04:
   '    - convert tile coordinates to x,y coordinates
   '  o fun06 sec05:
   '    - draw the GUI and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   struct gui_ftbRayST *retHeapGUI = 0;
   /*Image ftbIconImg = 0;*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - add widgets
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retHeapGUI = malloc(sizeof(struct gui_ftbRayST));
   if(! retHeapGUI)
      goto memErr_fun06;
   init_gui_ftbRayST(retHeapGUI);

   retHeapGUI->widgSTPtr =
      malloc(sizeof(struct widg_rayWidg));
   if(! retHeapGUI->widgSTPtr)
      goto memErr_fun06;
   init_widg_rayWidg(retHeapGUI->widgSTPtr);

   retHeapGUI->mesgBoxIdSI =
      mkMesgBox_rayWidg(retHeapGUI->widgSTPtr);
   if(retHeapGUI->mesgBoxIdSI < 0)
      goto memErr_fun06;

   retHeapGUI->fqButIdSI =
      addWidget_widg_rayWidg(
         def_xFqBut_ftbRayST,
         def_yFqBut_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->fqButIdSI < 0)
      goto memErr_fun06;

   retHeapGUI->prefixLabIdSI =
      addWidget_widg_rayWidg(
         def_xPrefixLab_ftbRayST,
         def_yPrefixLab_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->prefixLabIdSI < 0)
      goto memErr_fun06;

   retHeapGUI->prefixEntryIdSI =
      addWidget_widg_rayWidg(
         def_xPrefixEntry_ftbRayST,
         def_yPrefixEntry_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->prefixEntryIdSI < 0)
      goto memErr_fun06;

   retHeapGUI->outDirIdSI =
      addWidget_widg_rayWidg(
         def_xOutBut_ftbRayST,
         def_yOutBut_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->outDirIdSI < 0)
      goto memErr_fun06;

   retHeapGUI->configIdSI =
      addWidget_widg_rayWidg(
         def_xConfigBut_ftbRayST,
         def_yConfigBut_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->configIdSI < 0)
      goto memErr_fun06;

   /*TODO: add illumin check box*/

   retHeapGUI->runIdSI =
      addWidget_widg_rayWidg(
         def_xRunBut_ftbRayST,
         def_yRunBut_ftbRayST,
         1,  /*tileing*/
         -1, /*auto set width of widget*/
         -1, /*auto set height of widget*/
         retHeapGUI->widgSTPtr
      );
   if(retHeapGUI->runIdSI < 0)
      goto memErr_fun06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - draw initial GUI
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inactiveAdd_widg_rayWidg(
      retHeapGUI->prefixLabIdSI,
      retHeapGUI->widgSTPtr
   ); /*make sure label is in inactive state (no focus)*/

   if(retHeapGUI->inputBl)
   { /*If: drawing the input GUI*/
      InitWindow(
         retHeapGUI->inWinWidthSI,
         retHeapGUI->inWinHeightSI,
         (char *) inputStr
      );
   } /*If: drawing the input GUI*/

   else
   { /*Else: drawing the output GUI*/
      InitWindow(
         retHeapGUI->outWinWidthSI,
         retHeapGUI->outWinHeightSI,
         (char *) outputStr
      );
   } /*Else: drawing the output GUI*/

   /*SetWindowIcon(ftbIconImg)*/

   SetTargetFPS(60);

   if(
      setup_widg_rayWidg(
          retHeapGUI->widgSTPtr,
          def_fontSize_ftbRayST
      )
   ) goto memErr_fun06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - convert tile coordinates to x,y coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*this needs to come after window initialization step,
   `  otherwise I can not measure size of text
   */

   /*fastq file input*/
   butDraw_rayWidg(
      def_maxWidgetWidth_ftbRayST,
      0,                /*minumum width = any size*/
      retHeapGUI->fqButIdSI,  /*button id in array*/
      textFqButStr,
      1,                     /*finding coordinates*/
      retHeapGUI->widgSTPtr    /*has widgets to draw*/
   );

   /*prefix entry box*/
   labDraw_rayWidg(
      def_maxWidgetWidth_ftbRayST,
      0,                        /*no min width*/
      retHeapGUI->prefixLabIdSI,
      textPrefixLabStr,
      ' ',                      /*padding with spaces*/
      2,                        /*right pad if needed*/
      1,                     /*finding coordinates*/
      retHeapGUI->widgSTPtr
   ); /*label for entry box*/

   entryDraw_rayWidg(
      def_widthPrefixEntry_ftbRayST,
      retHeapGUI->prefixEntryIdSI,
      retHeapGUI->prefixPosArySI, /*cursor + scroll pos*/
      1,                          /*draw cursor*/
      retHeapGUI->inPrefixStr,    /*current user input*/
      1,                        /*finding coordinates*/
      retHeapGUI->widgSTPtr
   );

   /*output directory button*/
   butDraw_rayWidg(
      def_maxWidgetWidth_ftbRayST,
      0,                /*minumum width = any size*/
      retHeapGUI->outDirIdSI,  /*button id in array*/
      textOutButStr,
      1,                        /*finding coordinates*/
      retHeapGUI->widgSTPtr    /*has widgets to draw*/
   );

   /*TODO: add illumina check box*/

   /*confituration button*/
   butDraw_rayWidg(
      def_maxWidgetWidth_ftbRayST,
      0,                /*minumum width = any size*/
      retHeapGUI->configIdSI,  /*button id in array*/
      textConfigButStr,
      1,                        /*finding coordinates*/
      retHeapGUI->widgSTPtr    /*has widgets to draw*/
   );

   /*run button*/
   butDraw_rayWidg(
      def_maxWidgetWidth_ftbRayST,
      0,                /*minumum width = any size*/
      retHeapGUI->runIdSI,  /*button id in array*/
      textRunButStr,
      1,                        /*finding coordinates*/
      retHeapGUI->widgSTPtr    /*has widgets to draw*/
   );

   /*only need to tile once, since I do not dyamically
   `  change any widget sizes
   */
   tile_widg_rayWidg(retHeapGUI->widgSTPtr, -1, -1);
      /*using -1 because I want default spacing*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec05:
   ^   - draw the GUI and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   draw_gui_ftbRayST(retHeapGUI);

   return retHeapGUI;

   memErr_fun06:;
      if(retHeapGUI)
         freeHeap_gui_ftbRayST(retHeapGUI);
      retHeapGUI = 0;
      return 0;
} /*initRayGui_ftbRayST*/

/*-------------------------------------------------------\
| Fun07: checkRunEvent_ftbRayST
|   - checks for an event, and if can runs the found event
|   - also redraws the GUI
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Draws:
|     o current GUI state during and after event to screen
|   - Modifies:
|     o state and other widget related variables in
|       guiSTPtr
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
checkRunEvent_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - checks for an event, and if can runs found event
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - handle switching focus (tab) events
   '   o fun07 sec03:
   '     - detect enter key hits
   '   o fun07 sec04:
   '     - detect click events
   '   o fun07 sec05:
   '     - detect scroll events and add text events
   '   o fun07 sec06:
   '     - handle running button events
   '   o fun07 sec07:
   '     - return results and redraw gui
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int keySI = GetKeyPressed();
   signed int xSI = GetMouseX();
   signed int ySI = GetMouseY();

   signed char releaseBl =
      IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
   signed char pressBl =
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

   int scrollSI = (signed int) GetMouseWheelMove();
      /*returns integers, not percentage*/

   signed int indexSI = 0;
   signed int parSI = -1;    /*updated for parent events*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - handle switching focus (tab) events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexSI =
     focusCheck_widg_rayWidg(keySI,0,guiSTPtr->widgSTPtr);

   if(indexSI != -2)
   { /*If: user switched focus*/
      if(indexSI == guiSTPtr->prefixEntryIdSI)
         activeAdd_widg_rayWidg(
            guiSTPtr->prefixEntryIdSI,
            guiSTPtr->widgSTPtr
         );
      else
         activeClear_widg_rayWidg(
            guiSTPtr->prefixEntryIdSI,
            guiSTPtr->widgSTPtr
         );

      goto done_fun07_sec07;
      /*event was a change in focus*/
   } /*If: user switched focus*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - detect enter key hits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexSI =
     enterCheck_widg_rayWidg(
        keySI,
        0, /*key is from GetKey()*/
        &parSI, /*gets parent id if widget is child*/
        guiSTPtr->widgSTPtr
     );

   if(indexSI != -2)
   { /*If: enter key was hit*/
      /*I can ignore other errors here because I made sure
      `  they would never happen
      */
      draw_gui_ftbRayST(guiSTPtr);

      while( IsKeyDown(KEY_ENTER) )
         draw_gui_ftbRayST(guiSTPtr);

      pressClear_widg_rayWidg(
         indexSI,
         guiSTPtr->widgSTPtr
      );

      if(indexSI == guiSTPtr->fqButIdSI)
         goto getFqFiles_fun07_sec06_sub0x;

      else if(indexSI == guiSTPtr->outDirIdSI)
         goto getOutDir_fun07_sec06_sub0x;

      else if(indexSI == guiSTPtr->configIdSI)
         goto getConfigFile_fun07_sec06_sub0x;

      else if(indexSI == guiSTPtr->runIdSI)
         goto runFtb_fun07_sec06_sub0x;

      else if(parSI == guiSTPtr->mesgBoxIdSI)
         goto mesgBox_fun07_sec06_sub0x;

      goto done_fun07_sec07;
   } /*If: enter key*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - detect click events
   ^   o fun07 sec04 sub01:
   ^     - check if had event + clear old events and focus
   ^   o fun07 sec04 sub02:
   ^     - find click event widget
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *   - check if had event + clear old events and focus
   \*****************************************************/

   if(pressBl)
      ;
   else if(releaseBl)
      ;
   else
      goto detectScroll_fun07_sec06;

   indexSI =
      getMouseWidg_widg_rayWidg(
         xSI,
         ySI,
         &parSI,
         guiSTPtr->widgSTPtr
      ); /*get id of widget mouse is on*/

    pressClear_widg_rayWidg(
       guiSTPtr->fqButIdSI,
       guiSTPtr->widgSTPtr
   );

   pressClear_widg_rayWidg(
      guiSTPtr->outDirIdSI,
      guiSTPtr->widgSTPtr
   );

   pressClear_widg_rayWidg(
      guiSTPtr->configIdSI,
      guiSTPtr->widgSTPtr
   );

   pressClear_widg_rayWidg(
      guiSTPtr->runIdSI,
      guiSTPtr->widgSTPtr
   );

   mesgBoxEvent_rayWidg(
      0, /*clear button press*/
      guiSTPtr->mesgBoxIdSI,
      0, /*no need to list child*/
      guiSTPtr->widgSTPtr
   );

   /*****************************************************\
   * Fun07 Sec04 Sub02:
   *   - find click event widget
   \*****************************************************/

   if(indexSI == guiSTPtr->fqButIdSI)
   { /*If: hit fastq button*/
      if(pressBl)
         pressAdd_widg_rayWidg(
            guiSTPtr->fqButIdSI,
            guiSTPtr->widgSTPtr
         );
      else
         goto getFqFiles_fun07_sec06_sub0x;
   } /*If: hit fastq button*/

   else if(indexSI == guiSTPtr->outDirIdSI)
   { /*Else If: output directory button*/
      if(pressBl)
         pressAdd_widg_rayWidg(
            guiSTPtr->outDirIdSI,
            guiSTPtr->widgSTPtr
         );
      else
         goto getOutDir_fun07_sec06_sub0x;
   } /*Else If: output directory button*/

   else if(indexSI == guiSTPtr->configIdSI)
   { /*Else If: configuration file button*/
      if(pressBl)
         pressAdd_widg_rayWidg(
            guiSTPtr->configIdSI,
            guiSTPtr->widgSTPtr
         );
      else
         goto getConfigFile_fun07_sec06_sub0x;
   } /*Else If: configuration file button*/

   else if(indexSI == guiSTPtr->runIdSI)
   { /*Else If: run freezeTB button*/
      if(pressBl)
         pressAdd_widg_rayWidg(
            guiSTPtr->runIdSI,
            guiSTPtr->widgSTPtr
         );
      else
         goto runFtb_fun07_sec06_sub0x;
   } /*Else If: run freezeTB button*/

   else if(indexSI == guiSTPtr->prefixEntryIdSI)
   { /*Else If: prefix entry box*/
      activeAdd_widg_rayWidg(
         guiSTPtr->prefixEntryIdSI,
         guiSTPtr->widgSTPtr
      );
   } /*Else If: prefix entry box*/

   else if(parSI == guiSTPtr->mesgBoxIdSI)
   { /*Else If: dealing with a message box*/
      if(pressBl)
         mesgBoxEvent_rayWidg(
            1, /*add button press*/
            guiSTPtr->mesgBoxIdSI,
            indexSI,
            guiSTPtr->widgSTPtr
         );
      else
         goto mesgBox_fun07_sec06_sub0x;
   } /*Else If: dealing with a message box*/

   goto done_fun07_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - detect scroll events and add text events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   detectScroll_fun07_sec06:;

   if(
      activeStateGet_widg_rayWidg(
         guiSTPtr->prefixEntryIdSI,
         guiSTPtr->widgSTPtr
      )
   ){ /*If: scrolling in prefix entry box*/
      /*keeping scroll and cursor locked for sanity*/

      if(scrollSI)
      { /*If: user wanted to change text position*/
         guiSTPtr->prefixPosArySI[0] += scrollSI;
            /*scroll position*/
         goto done_fun07_sec07;
      } /*If: user wanted to change text position*/

      else if(! keySI)
         goto done_fun07_sec07;
         /*nothing input*/

      else
      { /*Else If: user input text*/
         addCharToEntry_rayWidg(
            keySI,
            guiSTPtr->inPrefixStr,
            &guiSTPtr->prefixLenSI,
            def_widthPrefixEntry_ftbRayST,
            guiSTPtr->prefixPosArySI,
            guiSTPtr->widgSTPtr
         ); /*do not care about output*/

         guiSTPtr->prefixLenSI =
            fileStrCheck_rayWidg(
               guiSTPtr->inPrefixStr,       /*input text*/
               guiSTPtr->prefixPosArySI[1], /*cusor pos*/
               127
            ); /*make sure valid output name*/

         goto done_fun07_sec07;
      } /*Else If: user input text*/
   } /*If: scrolling in prefix entry box*/

   goto done_fun07_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec06:
   ^   - handle running button events
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*TODO: add function calls*/
   mesgBox_fun07_sec06_sub0x:;
      mesgBoxEvent_rayWidg(
         2, /*rease key event*/
         parSI,
         indexSI,
         guiSTPtr->widgSTPtr
      );
      goto done_fun07_sec07;

   getFqFiles_fun07_sec06_sub0x:;
      hidenClear_widg_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         guiSTPtr->widgSTPtr
      );
      cpStr_ulCp(
         guiSTPtr->mesgStr,
         (signed char *) "open file browser to get fastq files"
      ); /*TODO add real function*/
      goto done_fun07_sec07;

   getOutDir_fun07_sec06_sub0x:;
      hidenClear_widg_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         guiSTPtr->widgSTPtr
      );
      cpStr_ulCp(
         guiSTPtr->mesgStr,
         (signed char *) "open file browser to get output"
      ); /*TODO add real function*/
      goto done_fun07_sec07;

   getConfigFile_fun07_sec06_sub0x:;
      hidenClear_widg_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         guiSTPtr->widgSTPtr
      );
      cpStr_ulCp(
         guiSTPtr->mesgStr,
         (signed char *) "open file browser to get FTB config file"
      ); /*TODO add real function*/
      goto done_fun07_sec07;

   runFtb_fun07_sec06_sub0x:;
      hidenClear_widg_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         guiSTPtr->widgSTPtr
      );
      cpStr_ulCp(
         guiSTPtr->mesgStr,
         (signed char *) "runs freezeTB on input"
      ); /*TODO add real function*/
      goto done_fun07_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec07:
   ^   - return results and redraw gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun07_sec07:;
      draw_gui_ftbRayST(guiSTPtr);
      return 0;
} /*checkRunEvent_ftbRayST*/
