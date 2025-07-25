/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' ftbRayLib SOF: Start Of File
'   - uses raylib to run a gui for freezeTB
'     * https://github.com/raysan5/raylib
'   o header:
'     - included libraries
'   o st01: guiST_ftbRay
'     - structure with all GUI settings and options
'   o fun0z: drawBut_ftbRay
'     - draw a button
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
#endif

#include "../raylib/src/raylib.h"
#include "ftbRayST.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "rayWidg.h"
!   - .h  #include "ibmPlexMono.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Main:
|   - main function to run raylib
| Input:
| Output:
|   - Screen:
|     o GUI to run freezeTB
|   - Prints:
|     o files made by freezeTB
\-------------------------------------------------------*/
int
main(
){
   signed char errSC = 0;
   struct gui_ftbRayST *guiHeapST = 0;
   /*const char *pwdConstStr = GetWorkingDirectory();*/
      /*directory user launched freezeTB in*/

   guiHeapST = mk_gui_ftbRayST();
   if(! guiHeapST)
      goto memErr_main;

   while( ! WindowShouldClose() )
      checkRunEvent_ftbRayST(guiHeapST);

   CloseWindow();


   errSC = 0;
   goto cleanUp_main;

   memErr_main:;
      errSC = 1;
      goto cleanUp_main;

   cleanUp_main:;
      freeHeap_gui_ftbRayST(guiHeapST);
      return errSC;
} /*main*/
