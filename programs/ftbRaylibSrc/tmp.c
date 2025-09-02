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
