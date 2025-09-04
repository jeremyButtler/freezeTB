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

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/ptrAry.h"
#include "../genLib/fileFun.h"

#include "../raylib/src/raylib.h"
#include "ftbRayST.h"
#include "rayWidg.h"

#include "../genFreezeTB/freezeTB.h"
#include "../genFreezeTB/freezeTBPaths.h"

/*.h files only*/
#include "../genLib/endLine.h"

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
#define def_winWidth_ftbRayST 420
#define def_winHeight_ftbRayST 320

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

   /*prefix button defaults*/
   guiSTPtr->prefixLenSI =
      cpStr_ulCp(
         guiSTPtr->inPrefixStr,
         (signed char *) "FTB_OUT"
      );
   guiSTPtr->prefixPosArySI[0] = 0; /*scroll*/
   guiSTPtr->prefixPosArySI[1] = 0; /*cusor position*/

   /*output button*/
   guiSTPtr->outDirStr[0] = 0;

   /*file browser message*/
   guiSTPtr->fileMesgStr[0] = 0;

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

   guiSTPtr->fqStrSTPtr = 0;
   guiSTPtr->widgSTPtr = 0;

   guiSTPtr->mesgBoxIdSI = 0;
   guiSTPtr->fqButIdSI = 0;
   guiSTPtr->prefixLabIdSI = 0;
   guiSTPtr->prefixEntryIdSI = 0;
   guiSTPtr->outDirIdSI = 0;
   guiSTPtr->configIdSI = 0;
   guiSTPtr->runIdSI = 0;
   guiSTPtr->fileBrowserIdSI = 0;

   guiSTPtr->fqFileSTPtr = 0;
   guiSTPtr->outDirSTPtr = 0;
   guiSTPtr->configFileSTPtr = 0;
   guiSTPtr->browserSC = -1;

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

   if(guiSTPtr->fqStrSTPtr)
      freeHeap_str_ptrAry(guiSTPtr->fqStrSTPtr);
   if(guiSTPtr->widgSTPtr)
      freeHeap_widg_rayWidg(guiSTPtr->widgSTPtr);
   if(guiSTPtr->fqFileSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->fqFileSTPtr);
   if(guiSTPtr->outDirSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->outDirSTPtr);
   if(guiSTPtr->configFileSTPtr)
      freeHeap_files_rayWidg(guiSTPtr->configFileSTPtr);

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
|   - voidGuiSTPtr:
|     o gui_ftbRayST struct pointer with gui to draw
|       sent in as void
| Output:
|   - Draws:
|     o current GUI state to screen
\-------------------------------------------------------*/
void
draw_gui_ftbRayST(
   void *voidGuiSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - draws the gui for a gui_ftbRayST structure
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - get file browser and set width and height
   '   o fun05 sec03:
   '     - start drawing + draw buttons and entry boxes
   '   o fun05 sec04:
   '     - draw labels for file paths
   '   o fun05 sec05:
   '     - draw message box and file browser
   '   o fun05 sec06:
   '     - end drawing
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char tmpStr[1024];
   signed int lenSI = 0;

   signed char blinkBl = 0; /*be in blink state*/
   struct Color guiCol;
   signed int heightSI = 0;
   signed int widthSI = 0;

   struct gui_ftbRayST *guiSTPtr =
      (gui_ftbRayST *) voidGuiSTPtr;
   struct files_rayWidg *fileSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - get file browser and set width and height
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   switch(guiSTPtr->browserSC)
   { /*Switch: find which browser using*/
      case 0: fileSTPtr = guiSTPtr->fqFileSTPtr; break;
      case 1: fileSTPtr = guiSTPtr->outDirSTPtr; break;
      case 2: fileSTPtr =guiSTPtr->configFileSTPtr; break;

      default: fileSTPtr = guiSTPtr->fqFileSTPtr; break;
         /*file browser is hidden, just give something*/
   } /*Switch: find which browser using*/

   guiCol = GetColor(guiSTPtr->widgSTPtr->guiColSI);

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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - start drawing + draw buttons and entry boxes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec04:
      ^   - draw labels for file paths
      ^   o fun05 sec04 sub01:
      ^     - draw fastq file paths label
      ^   o fun05 sec04 sub02:
      ^     - draw output directory label
      ^   o fun05 sec04 sub03:
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      /**************************************************\
      * Fun05 Sec04 Sub01:
      *   - draw fastq file paths label
      \**************************************************/

      if(guiSTPtr->fqStrSTPtr->lenSL)
      { /*If: have fastq files to draw*/
         if(guiSTPtr->fqStrSTPtr->lenSL > 1)
         { /*If: have more than one file*/
            lenSI =
               cpStr_ulCp(
                  tmpStr,
                  guiSTPtr->fqFileSTPtr->pwdStr
               );
            tmpStr[lenSI++] = '*';
            tmpStr[lenSI] = 0;
         } /*If: have more than one file*/

         else
         { /*Else: only one file*/
            cpStr_ulCp(
               tmpStr,
               guiSTPtr->fqStrSTPtr->strAry[0]
            );
         } /*Else: only one file*/

         textDrawByCoord_rayWidg(
            tmpStr,
            guiSTPtr->widgSTPtr->xArySI[
               guiSTPtr->fqButIdSI
            ]
               + guiSTPtr->widgSTPtr->widthArySI[
                   guiSTPtr->fqButIdSI
                 ],
            guiSTPtr->widgSTPtr->yArySI[
               guiSTPtr->fqButIdSI
            ],
            guiSTPtr->inWinWidthSI -
               guiSTPtr->widgSTPtr->widthArySI[
                  guiSTPtr->fqButIdSI
               ],
            guiSTPtr->widgSTPtr->textColSI,
            3, /*pad for x and y*/
            guiSTPtr->widgSTPtr
         );
      } /*If: have fastq files to draw*/

      /**************************************************\
      * Fun05 Sec04 Sub02:
      *   - draw output directory label
      \**************************************************/

      if(guiSTPtr->outDirStr[0])
      { /*If: have a output directory*/
         textDrawByCoord_rayWidg(
            guiSTPtr->outDirStr,
            guiSTPtr->widgSTPtr->xArySI[
               guiSTPtr->outDirIdSI
            ]
               + guiSTPtr->widgSTPtr->widthArySI[
                   guiSTPtr->outDirIdSI
                 ],
            guiSTPtr->widgSTPtr->yArySI[
               guiSTPtr->outDirIdSI
            ],
            guiSTPtr->inWinWidthSI -
               guiSTPtr->widgSTPtr->widthArySI[
                  guiSTPtr->outDirIdSI
               ],
            guiSTPtr->widgSTPtr->textColSI,
            3, /*pad for x and y*/
            guiSTPtr->widgSTPtr
         );
      } /*If: have a output directory*/

      /**************************************************\
      * Fun05 Sec04 Sub03:
      *   - draw configuration file label
      \**************************************************/

      if(guiSTPtr->configFileStr[0])
      { /*If: have a configuration file*/
         textDrawByCoord_rayWidg(
            guiSTPtr->configFileStr,
            guiSTPtr->widgSTPtr->xArySI[
               guiSTPtr->configIdSI
            ]
               + guiSTPtr->widgSTPtr->widthArySI[
                   guiSTPtr->configIdSI
                 ],
            guiSTPtr->widgSTPtr->yArySI[
               guiSTPtr->configIdSI
            ],
            guiSTPtr->inWinWidthSI -
               guiSTPtr->widgSTPtr->widthArySI[
                  guiSTPtr->configIdSI
               ],
            guiSTPtr->widgSTPtr->textColSI,
            3, /*pad for x and y*/
            guiSTPtr->widgSTPtr
         );
      } /*If: have a configuration file*/

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec05:
      ^   - draw message box and file browser
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      mesgBoxDraw_rayWidg(
         guiSTPtr->mesgBoxIdSI,
         widthSI,
         heightSI,
         guiSTPtr->mesgStr,
         (signed char *) "Ok",
         guiSTPtr->widgSTPtr
      );

      fileBrowserDraw_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->fileMesgStr,
         widthSI,
         heightSI,
         fileSTPtr,
         guiSTPtr->widgSTPtr
      );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec06:
      ^   - end drawing
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
   ^   - initialize and add widgets
   ^   o fun06 sec02 sub01:
   ^     - initialize the gui
   ^   o fun06 sec02 sub02:
   ^     - add input buttons and entry boxes
   ^   o fun06 sec02 sub03:
   ^     - add input file browsers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec02 Sub02:
   *   - initialize the gui
   \*****************************************************/

   retHeapGUI = malloc(sizeof(struct gui_ftbRayST));
   if(! retHeapGUI)
      goto memErr_fun06;
   init_gui_ftbRayST(retHeapGUI);

   retHeapGUI->widgSTPtr =
      malloc(sizeof(struct widg_rayWidg));
   if(! retHeapGUI->widgSTPtr)
      goto memErr_fun06;
   init_widg_rayWidg(retHeapGUI->widgSTPtr);

   retHeapGUI->fqStrSTPtr = mk_str_ptrAry(32);
   if(! retHeapGUI->fqStrSTPtr)
      goto memErr_fun06;


   InitWindow(
      retHeapGUI->inWinWidthSI,
      retHeapGUI->inWinHeightSI,
      (char *) inputStr
   );

   /*SetWindowIcon(ftbIconImg)*/

   SetTargetFPS(60);

   /*****************************************************\
   * Fun06 Sec02 Sub02:
   *   - add input buttons and entry boxes
   \*****************************************************/

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

   /*****************************************************\
   * Fun06 Sec02 Sub02:
   *   - add input file browsers
   *   o fun06 sec02 sub02 cat01:
   *     - add file browswer widget
   *   o fun06 sec02 sub02 cat02:
   *     - add fastq file browser structure
   *   o fun06 sec02 sub02 cat03:
   *     - add output directory file browser structure
   *   o fun06 sec02 sub02 cat04:
   *     - add configuration file file browser structure
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec02 Sub02 Cat01:
   +   - add file browswer widget
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->fileBrowserIdSI =
      mkFileBrowser_rayWidg(retHeapGUI->widgSTPtr);

   if(retHeapGUI->fileBrowserIdSI < 0)
      goto memErr_fun06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - draw initial GUI
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inactiveAdd_widg_rayWidg(
      retHeapGUI->prefixLabIdSI,
      retHeapGUI->widgSTPtr
   ); /*make sure label is in inactive state (no focus)*/

   if( setup_widg_rayWidg(retHeapGUI->widgSTPtr) )
      goto memErr_fun06;

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

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec02 Sub02 Cat02:
   +   - add fastq file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->fqFileSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->fqFileSTPtr)
      goto memErr_fun06;
   init_files_rayWidg(retHeapGUI->fqFileSTPtr);

   if(
      addExt_files_rayWidg(
         (signed char *) ".fq",
         1, /*cloear old extensions*/
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fastq",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fastq.gz",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   if(
      addExt_files_rayWidg(
         (signed char *) ".fq.gz",
         0,
         1, /*select this extension*/
         retHeapGUI->fqFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec02 Sub02 Cat03:
   +   - add output directory file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->outDirSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->outDirSTPtr)
      goto memErr_fun06;
   init_files_rayWidg(retHeapGUI->outDirSTPtr);
   setFileLimit_files_rayWidg(1, retHeapGUI->outDirSTPtr);

   if(
      addExt_files_rayWidg(
         (signed char *) "dir",
         1, /*cloear old extensions*/
         1, /*select this extension*/
         retHeapGUI->outDirSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   cpStr_ulCp(
      retHeapGUI->outDirStr,
      retHeapGUI->outDirSTPtr->pwdStr
   );
      

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun06 Sec02 Sub02 Cat04:
   +   - add configuration file file browser structure
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   retHeapGUI->configFileSTPtr =
      malloc(sizeof(struct files_rayWidg));
   if(! retHeapGUI->configFileSTPtr)
      goto memErr_fun06;
   init_files_rayWidg(retHeapGUI->configFileSTPtr);

   setFileLimit_files_rayWidg(
      1,
      retHeapGUI->configFileSTPtr
   );

   if(
      addExt_files_rayWidg(
         (signed char *) "*",
         1, /*cloear old extensions*/
         1, /*select this extension*/
         retHeapGUI->configFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

   if(
      addExt_files_rayWidg(
         (signed char *) ".txt",
         0, /*do not clear old extensions*/
         0, /*do not select this extension*/
         retHeapGUI->configFileSTPtr,
         retHeapGUI->widgSTPtr
      )
   ) goto memErr_fun06;

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
   '     - get and check events
   '   o fun07 sec06:
   '     - handle running button events
   '   o fun07 sec07:
   '     - return results and redraw gui
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpHeapStr = 0;

   /*for reading the config file*/
   #define def_lineLen_fun07 1024
   signed char lineStr[def_lineLen_fun07 + 8];
   signed char logFileStr[def_lineLen_fun07 + 8];
   FILE *inFILE = 0;
   signed char refStr[def_lineLen_fun07 + 8];
   signed long discardSL = 0; /*for reading files*/

   /*for buiding freezeTB run command*/
   signed char *argAryStr[1024];
      /*just give more memory then I will ever need*/
   signed int argLenSI = 0;

   struct event_rayWidg eventStackST;
   signed int indexSI = 0;
   signed int tmpSI = 0;

   struct files_rayWidg *fileSTPtr = 0;

   for(tmpSI = 0; tmpSI < 1024; ++tmpSI)
      argAryStr[tmpSI] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - get and check events
   ^   o fun07 sec02 sub01:
   ^     - get event and check entery event
   ^   o fun07 sec02 sub02:
   ^     - check which event I am running
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec02 Sub01:
   *   - get event and check entery event
   \*****************************************************/

   init_event_rayWidg(&eventStackST);

   indexSI =
      get_event_rayWidg(
         1,                  /*handel focus changes*/
         draw_gui_ftbRayST,  /*redraw GUI for presses*/
         guiSTPtr,           /*has GUI to redraw*/
         &eventStackST,      /*has events*/
         guiSTPtr->widgSTPtr /*has widgets*/
      );

   /*not worried, bucause this will check if the entry
   `  box id matches the found id or is not active. So, it
   `  does nothing if not on an entry box
   */
   tmpSI =
      entryEvent_rayWidg(
         guiSTPtr->prefixEntryIdSI,/*id of entry box*/
         guiSTPtr->prefixPosArySI,/*cursor/scroll pos*/
         guiSTPtr->inPrefixStr,   /*text in entry box*/
         guiSTPtr->prefixLenSI,   /*string length*/
         127,                     /*max string length*/
         fileStrCheck_rayWidg,    /*check function*/
         &eventStackST,
         guiSTPtr->widgSTPtr
      ); /*add any keyboard inputs to entry box*/

   if(tmpSI >= 0)
   { /*If: was a entry box input event*/
      guiSTPtr->prefixLenSI = tmpSI;
      goto done_fun07_sec07;
   } /*If: was a entry box input event*/

   /*****************************************************\
   * Fun07 Sec02 Sub02:
   *   - check which event I am running
   \*****************************************************/

   if(eventStackST.idSI == guiSTPtr->fqButIdSI)
      goto getFqFiles_fun07_sec06_sub02;

   else if(eventStackST.idSI == guiSTPtr->outDirIdSI)
      goto getOutDir_fun07_sec06_sub03;

   else if(eventStackST.idSI == guiSTPtr->configIdSI)
      goto getConfigFile_fun07_sec06_sub04;

   else if(eventStackST.idSI == guiSTPtr->runIdSI)
      goto runFtb_fun07_sec06_sub06;

   else if(eventStackST.parIdSI == guiSTPtr->mesgBoxIdSI)
      goto mesgBox_fun07_sec06_sub01;

   else if(
      eventStackST.parIdSI == guiSTPtr->fileBrowserIdSI
   ) goto fileBrowser_fun07_sec06_sub05;

   goto done_fun07_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec06:
   ^   - handle running button events
   ^   o fun07 sec06 sub01:
   ^     - message box event
   ^   o fun07 sec06 sub02:
   ^     - get fastq files event
   ^   o fun07 sec06 sub03:
   ^     - get output directory event
   ^   o fun07 sec06 sub04:
   ^     - get configuration file event
   ^   o fun07 sec06 sub05:
   ^     - file browser event actions
   ^   o fun07 sec06 sub06:
   ^     - run event actions
   ^   o fun07 sec06 sub07:
   ^     - button pressed to build the output report
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec06 Sub01:
   *   - message box event
   \*****************************************************/

   mesgBox_fun07_sec06_sub01:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun07_sec07;

      mesgBoxEvent_rayWidg(
         2, /*rease key event*/
         eventStackST.parIdSI,
         eventStackST.idSI,
         guiSTPtr->widgSTPtr
      );
      goto done_fun07_sec07;

   /*****************************************************\
   * Fun07 Sec06 Sub02:
   *   - get fastq files event
   \*****************************************************/

   getFqFiles_fun07_sec06_sub02:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun07_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 0;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select fastq files to run"
      );
      goto done_fun07_sec07;

   /*****************************************************\
   * Fun07 Sec06 Sub03:
   *   - get output directory event
   \*****************************************************/

   getOutDir_fun07_sec06_sub03:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun07_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 1;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select output folder"
      );
      goto done_fun07_sec07;

   /*****************************************************\
   * Fun07 Sec06 Sub04:
   *   - get configuration file event
   \*****************************************************/

   getConfigFile_fun07_sec06_sub04:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun07_sec07;

      hidenClear_widg_rayWidg(
         guiSTPtr->fileBrowserIdSI,
         guiSTPtr->widgSTPtr
      );

      guiSTPtr->browserSC = 2;

      cpStr_ulCp(
         guiSTPtr->fileMesgStr,
         (signed char *) "select FTB configuration file"
      );
      goto done_fun07_sec07;

   /*****************************************************\
   * Fun07 Sec06 Sub05:
   *   - file browser event actions
   *   o fun07 sec06 sub05 cat01:
   *     - run file brower event
   *   o fun07 sec06 sub05 cat02:
   *     - cancel event
   *   o fun07 sec06 sub05 cat03:
   *     - selected output directory
   *   o fun07 sec06 sub05 cat04:
   *     - selected configuration file
   *   o fun07 sec06 sub05 cat05:
   *     - selected fastq files
   *   o fun07 sec06 sub05 cat05:
   *     - error or no event
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec06 Sub05 Cat01:
   +   - run file brower event
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fileBrowser_fun07_sec06_sub05:;
      switch(guiSTPtr->browserSC)
      { /*Switch: find which browser using*/
         case 0: fileSTPtr = guiSTPtr->fqFileSTPtr; break;
         case 1: fileSTPtr = guiSTPtr->outDirSTPtr; break;
         case 2: fileSTPtr = guiSTPtr->configFileSTPtr;
                 break;

         default: goto done_fun07_sec07;
           /*invalid option*/
      } /*Switch: find which browser using*/

      tmpSI =
         fileBrowserEvent_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            &eventStackST,
            fileSTPtr,
            guiSTPtr->widgSTPtr
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub05 Cat02:
      +   - cancel event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpSI == 2)
      { /*If: hit cancel*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
          ); /*use hit cancel*/

          goto done_fun07_sec07;
      } /*If: hit cancel*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub05 Cat03:
      +   - selected output directory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI == 1)
      { /*Else If: files were selected*/
         hidenAdd_widg_rayWidg(
            guiSTPtr->fileBrowserIdSI,
            guiSTPtr->widgSTPtr
         ); /*user hit select*/

         switch(guiSTPtr->browserSC)
         { /*Switch: find which browser using*/
            case 1:
            /*Case: output directory selected*/
               tmpSI = 0;
               tmpHeapStr =
                  getFile_files_rayWidg(
                     &tmpSI,
                     0, /*only selected items*/
                     fileSTPtr
                  );
               if(! tmpHeapStr)
                  goto err_fun07_sec07;

               cpStr_ulCp(guiSTPtr->outDirStr,tmpHeapStr);
               free(tmpHeapStr);
               tmpHeapStr = 0;
               goto done_fun07_sec07;
            /*Case: output directory selected*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun07 Sec06 Sub05 Cat04:
            +   - selected configuration file
            \+++++++++++++++++++++++++++++++++++++++++++*/

            case 2:
            /*Case: configuration file selected*/
               tmpSI = 0;
               tmpHeapStr =
                  getFile_files_rayWidg(
                     &tmpSI,
                     0, /*only selected items*/
                     fileSTPtr
                  );
               if(! tmpHeapStr)
                  goto err_fun07_sec07;

               cpStr_ulCp(
                  guiSTPtr->configFileStr,
                  tmpHeapStr
               );
               free(tmpHeapStr);
               tmpHeapStr = 0;
               goto done_fun07_sec07;
            /*Case: configuration file selected*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun07 Sec06 Sub05 Cat05:
            +   - selected fastq files
            \+++++++++++++++++++++++++++++++++++++++++++*/

            case 0:
            /*Case: fastq files selected*/
               tmpSI = 0;

               while(tmpSI >= 0)
               { /*Loop: get fastq files*/
                  tmpHeapStr =
                     getFile_files_rayWidg(
                        &tmpSI,
                        0, /*only selected items*/
                        fileSTPtr
                     );
                  if(tmpSI < 0)
                      break;
                  else if(! tmpHeapStr)
                     goto err_fun07_sec07;

                  if(
                     add_str_ptrAry(
                        tmpHeapStr,
                        guiSTPtr->fqStrSTPtr,
                        guiSTPtr->fqStrSTPtr->lenSL
                     )
                  ) goto err_fun07_sec07;

                  free(tmpHeapStr);
                  tmpHeapStr = 0;
               } /*Loop: get fastq files*/

               goto done_fun07_sec07;
            /*Case: fastq files selected*/
         } /*Switch: find which browser using*/
      } /*Else If: files were selected*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub05 Cat05:
      +   - error or no event
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(tmpSI < -2)
         goto err_fun07_sec07;

      goto done_fun07_sec07;

   /*****************************************************\
   * Fun07 Sec06 Sub06:
   *   - run event actions
   *   o fun07 sec06 sub06 cat01:
   *     - check if everything was input
   *   o fun07 sec06 sub06 cat02:
   *     - read in the config file
   *   o fun07 sec06 sub06 cat03:
   *     - build prefix & make output directory
   *   o fun07 sec06 sub06 cat04:
   *     - build the log file
   *   o fun07 sec06 sub06 cat05:
   *     - check if can run minimap2
   *   o fun07 sec06 sub06 cat06:
   *     - find length of minimap2 command and get memory
   *   o fun07 sec06 sub06 cat07:
   *     - build minimap2 command
   *   o fun07 sec06 sub06 cat08:
   *     - add the -sam <file>.sam entry to ftb
   *   o fun07 sec06 sub06 cat09:
   *     - run minimap2
   *   o fun07 sec06 sub06 cat10:
   *     - if cannot, copy fastq files to ftb command
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun07 Sec06 Sub06 Cat01:
   +   - check if everything was input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   runFtb_fun07_sec06_sub06:;
      if(! (indexSI & def_releaseEvent_rayWidg) )
         goto done_fun07_sec07;

      if(guiSTPtr->fqStrSTPtr->lenSL <= 0)
      { /*If: no fastq files input*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(
            guiSTPtr->mesgStr,
            (signed char *) "no fastq files input"
         );

         goto done_fun07_sec07;
      } /*If: no fastq files input*/

      else if(guiSTPtr->prefixLenSI <= 0)
      { /*Else If: no prefix input*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(
            guiSTPtr->mesgStr,
            (signed char *) "no prefix input"
         );

         guiSTPtr->prefixLenSI =
            cpStr_ulCp(
               guiSTPtr->inPrefixStr,
               (signed char *) "FTB_OUT"
            );
         goto done_fun07_sec07;
      } /*Else If: no prefix input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat02:
      +   - read in the config file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*get path to default reference sequence*/
      refPath_freezeTBPaths(refStr);

      argLenSI = 1; /*0 is always reserved*/

      if(guiSTPtr->configFileStr[0])
      { /*If: user provided a configuration file*/
         inFILE =
            fopen((char *) guiSTPtr->configFileStr, "r");

         while(
            getLine_fileFun(
               inFILE,
               lineStr,
               def_lineLen_fun07,
               &discardSL
            )
         ){ /*Loop: read in configuration file*/
            tmpHeapStr = lineStr;

            while(*tmpHeapStr && *tmpHeapStr < 33)
               ++tmpHeapStr;
            if(! *tmpHeapStr)
               continue;
            tmpSI = endWhite_ulCp(tmpHeapStr);

            argAryStr[argLenSI] =
               malloc((tmpSI + 8) * sizeof(signed char));
            if(! argAryStr[argLenSI])
               goto err_fun07_sec07;

            cpWhite_ulCp(argAryStr[argLenSI], tmpHeapStr);
            ++argLenSI;
            tmpHeapStr = 0;

            /*copy the second entry (if is one)*/
            while(*tmpHeapStr && *tmpHeapStr < 33)
               ++tmpHeapStr;
            if(! *tmpHeapStr)
               continue;
            tmpSI = endStr_ulCp(tmpHeapStr);

            argAryStr[argLenSI] =
               malloc((tmpSI + 8) * sizeof(signed char));
            if(! argAryStr[argLenSI])
               goto err_fun07_sec07;

            cpStr_ulCp(argAryStr[argLenSI], tmpHeapStr);

            /*make sure user is not changing reference*/
            if(argAryStr[argLenSI - 1][0] != '-') ;
            else if(argAryStr[argLenSI - 1][1] != 'r') ;
            else if(argAryStr[argLenSI - 1][2] != 'e') ;
            else if(argAryStr[argLenSI - 1][3] != 'f') ;
            else
               cpStr_ulCp(refStr, argAryStr[argLenSI]);
               /*user input reference sequence*/

            ++argLenSI;
            tmpHeapStr = 0;
         } /*Loop: read in configuration file*/

         tmpHeapStr = 0;

         fclose(inFILE);
         inFILE = 0;
      } /*If: user provided a configuration file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat03:
      +   - build prefix and make output directory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*prefix tag*/
      argAryStr[argLenSI] =
         malloc((7 + 8) * sizeof(signed char));

      if(! argAryStr[argLenSI])
         goto err_fun07_sec07;
      cpStr_ulCp(
         argAryStr[argLenSI],
         (signed char *) "-prefix"
      );
      ++argLenSI;

      /*prefix path*/
      tmpSI = endStr_ulCp(guiSTPtr->inPrefixStr) * 2;
      tmpSI += endStr_ulCp(guiSTPtr->outDirStr) + 1;
      tmpSI += 2;
      argAryStr[argLenSI] =
         malloc((tmpSI + 8) * sizeof(signed char));
      if(! argAryStr[argLenSI])
         goto err_fun07_sec07;
      tmpSI =
         cpStr_ulCp(
            argAryStr[argLenSI],
            guiSTPtr->outDirStr
         );
      argAryStr[argLenSI][tmpSI++] = def_pathSep_rayWidg;
      tmpSI +=
         cpStr_ulCp(
            &argAryStr[argLenSI][tmpSI],
            guiSTPtr->inPrefixStr
         );
      
      if( MakeDirectory((char *) argAryStr[argLenSI]) )
      { /*If: could not make the output directory*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         tmpSI =
            cpStr_ulCp(
               guiSTPtr->mesgStr,
               (signed char *) "could not make folder: "
            );
         cpStr_ulCp(
            &guiSTPtr->mesgStr[tmpSI],
            argAryStr[argLenSI]
         );
         goto done_fun07_sec07;
      } /*If: could not make the output directory*/

      argAryStr[argLenSI][tmpSI++] = def_pathSep_rayWidg;
      tmpSI +=
         cpStr_ulCp(
            &argAryStr[argLenSI][tmpSI],
            guiSTPtr->inPrefixStr
         );

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat04:
      +   - build the log file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSI = cpStr_ulCp(logFileStr, argAryStr[argLenSI]);
      logFileStr[tmpSI++] = '-';
      logFileStr[tmpSI++] = 'l';
      logFileStr[tmpSI++] = 'o';
      logFileStr[tmpSI++] = 'g';
      logFileStr[tmpSI++] = '.';
      logFileStr[tmpSI++] = 't';
      logFileStr[tmpSI++] = 'x';
      logFileStr[tmpSI++] = 't';
      logFileStr[tmpSI] = 0;

      ++argLenSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat05:
      +   - check if can run minimap2
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSI =
         cpStr_ulCp(
            lineStr,
            (signed char *) "minimap2 --version > "
         );
      tmpSI += cpStr_ulCp(&lineStr[tmpSI], logFileStr);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat06:
      +   - find length of minimap2 command and get memory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(! system((char *) lineStr) )
      { /*If: minimap2 exists*/
         tmpSI = 22;/*length of "minimap2 -a -x map-ont"*/
         tmpSI += endStr_ulCp(refStr) + 2; /*+2 for "'s*/
         tmpSI += endStr_ulCp(argAryStr[argLenSI - 1]);
         tmpSI += 5; /*" > \"<out_file>.sam\""*/

         for(
            indexSI = 0;
            indexSI < guiSTPtr->fqStrSTPtr->lenSL;
            ++indexSI
         ) tmpSI += 
             guiSTPtr->fqStrSTPtr->lenAryUI[indexSI] + 3;
             /*length for adding fastq files; + 1 for
             `  space between files; + 2 for "'s
             */
         tmpHeapStr =
            malloc((tmpSI + 8) * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun07_sec07;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun07 Sec06 Sub06 Cat07:
         +   - build minimap2 command
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpSI =
            cpStr_ulCp(
               tmpHeapStr,
               (signed char *) "minimap2 -a -x map-ont \""
            );
         tmpSI += cpStr_ulCp(&tmpHeapStr[tmpSI], refStr);
         tmpHeapStr[tmpSI++] = '"';
         tmpHeapStr[tmpSI++] = ' ';
         tmpHeapStr[tmpSI] = 0;

         for(
            indexSI = 0;
            indexSI < guiSTPtr->fqStrSTPtr->lenSL;
            ++indexSI
         ){ /*Loop: copy fastq sequences*/
            tmpHeapStr[tmpSI++] = '"';

            cpLen_ulCp(
               &tmpHeapStr[tmpSI],
               guiSTPtr->fqStrSTPtr->strAry[indexSI],
               guiSTPtr->fqStrSTPtr->lenAryUI[indexSI]
            ); /*copy fastq file name*/

            tmpSI +=
               guiSTPtr->fqStrSTPtr->lenAryUI[indexSI];
            tmpHeapStr[tmpSI++] = '"';
            tmpHeapStr[tmpSI++] = ' ';
         }  /*Loop: copy fastq sequences*/

         tmpHeapStr[tmpSI++] = '>';
         tmpHeapStr[tmpSI++] = ' ';
         tmpHeapStr[tmpSI++] = '"';
         tmpSI +=
            cpStr_ulCp(
               &tmpHeapStr[tmpSI],
               argAryStr[argLenSI - 1]
            ); /*copy output name*/

         tmpHeapStr[tmpSI++] = '.';
         tmpHeapStr[tmpSI++] = 's';
         tmpHeapStr[tmpSI++] = 'a';
         tmpHeapStr[tmpSI++] = 'm';
         tmpHeapStr[tmpSI++] = '"';
         tmpHeapStr[tmpSI] = 0;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun07 Sec06 Sub06 Cat08:
         +   - add the -sam <file>.sam entry to ftb
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         argAryStr[argLenSI] =
            malloc(11 * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun07_sec07;
         argAryStr[argLenSI][0] = '-';
         argAryStr[argLenSI][1] = 's';
         argAryStr[argLenSI][2] = 'a';
         argAryStr[argLenSI][3] = 'm';
         argAryStr[argLenSI][4] = 0;
         ++argLenSI;

         tmpSI = endStr_ulCp(argAryStr[argLenSI - 2]);
         argAryStr[argLenSI] =
            malloc((tmpSI + 13) * sizeof(signed char));
         if(! tmpHeapStr)
            goto err_fun07_sec07;
         cpLen_ulCp(
            argAryStr[argLenSI],
            argAryStr[argLenSI - 2],
            tmpSI
         );
         argAryStr[argLenSI][tmpSI++] = '.';
         argAryStr[argLenSI][tmpSI++] = 's';
         argAryStr[argLenSI][tmpSI++] = 'a';
         argAryStr[argLenSI][tmpSI++] = 'm';
         argAryStr[argLenSI][tmpSI] = 0;
         ++argLenSI;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun07 Sec06 Sub06 Cat09:
         +   - run minimap2
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         inFILE = fopen((char *) logFileStr, "a");
         fprintf(
            inFILE,
            "^^^^minimap2 version^^^^%s",
           str_endLine
         );
         fprintf(
            inFILE,
            "minimap2 cmd: %s%s",
            tmpHeapStr,
            str_endLine
         );
         fclose(inFILE);
         inFILE = 0;

         if(system((char *) tmpHeapStr))
         { /*If: minimap2 errored out*/
            hidenClear_widg_rayWidg(
               guiSTPtr->mesgBoxIdSI,
               guiSTPtr->widgSTPtr
            );
            tmpSI =
               cpStr_ulCp(
                  guiSTPtr->mesgStr,
                  (signed char *) "minimap2 errored out"
               );
            cpStr_ulCp(
               &guiSTPtr->mesgStr[tmpSI],
               argAryStr[argLenSI]
            );
            goto done_fun07_sec07;
         } /*If: minimap2 errored out*/

         free(tmpHeapStr);
         tmpHeapStr = 0;
      } /*If: minimap2 exists*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat10:
      +   - if cannot, copy fastq files to ftb command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: no minimap2*/
         for(
            indexSI = 0;
            indexSI < guiSTPtr->fqStrSTPtr->lenSL;
            ++indexSI
         ){ /*Loop: add fastq files to ftb command*/
            argAryStr[argLenSI] =
               malloc(
                 ( guiSTPtr->fqStrSTPtr->lenAryUI[indexSI]
                  + 8 
                 ) * sizeof(signed char)
               );

            if(! argAryStr[argLenSI])
               goto err_fun07_sec07;

            cpLen_ulCp(
               argAryStr[argLenSI],
               guiSTPtr->fqStrSTPtr->strAry[indexSI],
               guiSTPtr->fqStrSTPtr->lenAryUI[indexSI]
            ); /*copy fastq file name*/

            ++argLenSI;
         }  /*Loop: add fastq files to ftb command*/
      } /*Else: no minimap2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun07 Sec06 Sub06 Cat11:
      +   - run freezeTB
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(tmpHeapStr)
         free(tmpHeapStr);
      tmpHeapStr = 0;

      inFILE = fopen((char *) logFileStr, "a");

      pversion_freezeTB(inFILE);
      fprintf(inFILE, "FreezeTB cmd:%s", str_endLine);
      fprintf(inFILE, "  freezeTB \\%s", str_endLine);

      for(indexSI = 1; indexSI < argLenSI; indexSI += 2)
      { /*Loop: print freezeTB command*/
         if(indexSI + 2 < argLenSI)
            fprintf(inFILE,
               "    %s %s \\%s",
               argAryStr[indexSI],
               argAryStr[indexSI + 1],
               str_endLine
            );
         else if(indexSI + 1 < argLenSI)
            fprintf(inFILE,
               "    %s %s%s",
               argAryStr[indexSI],
               argAryStr[indexSI + 1],
               str_endLine
            );
         else
            fprintf(inFILE,
               "    %s%s",
               argAryStr[indexSI],
               str_endLine
            );
      } /*Loop: print freezeTB command*/

      fclose(inFILE);
      inFILE = 0;

      tmpHeapStr =
         run_freezeTB(argLenSI, (char **) argAryStr);

      if(tmpHeapStr)
      { /*If: had an error*/
         hidenClear_widg_rayWidg(
            guiSTPtr->mesgBoxIdSI,
            guiSTPtr->widgSTPtr
         );
         cpStr_ulCp(guiSTPtr->mesgStr, tmpHeapStr);
         goto done_fun07_sec07;
      } /*If: had an error*/

      /*remove run fastq files*/
      blank_str_ptrAry(guiSTPtr->fqStrSTPtr);
      goto buildOutReport_fun07_sec06_sub07;

   /*****************************************************\
   * Fun07 Sec06 Sub07:
   *   - button pressed to build the output report
   \*****************************************************/

   /*TODO: work on the output part of the GUI*/
   buildOutReport_fun07_sec06_sub07:;
      goto done_fun07_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec07:
   ^   - return results and redraw gui
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto done_fun07_sec07;

   err_fun07_sec07:;
      tmpSI = 1;
      goto ret_fun07_sec07;

   done_fun07_sec07:;
      draw_gui_ftbRayST(guiSTPtr);
      tmpSI = 0;
      goto ret_fun07_sec07;

   ret_fun07_sec07:;
      if(inFILE)
         fclose(inFILE); /*never will be stdout/in/err*/
      inFILE = 0;

      freeStack_event_rayWidg(&eventStackST);

      if(tmpHeapStr)
         free(tmpHeapStr);
      tmpHeapStr = 0;

      for(indexSI = 0; indexSI < argLenSI; ++indexSI)
      { /*Loop: free c-string array*/
         if(argAryStr[indexSI])
            free(argAryStr[indexSI]);
         argAryStr[indexSI] = 0;
      } /*Loop: free c-string array*/

      argLenSI = 0;

      return (signed char) tmpSI;
} /*checkRunEvent_ftbRayST*/
