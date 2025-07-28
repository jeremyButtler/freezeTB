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
   
   if( setup_widg_rayWidg(&widgetsStackST) )
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
