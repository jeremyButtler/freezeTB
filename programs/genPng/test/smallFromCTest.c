/*goal is to see if I can load a font from a .c and .h
`  file made using the fontToC_font_fontST() function
`To use this you need the delete.c and delete.h files
`  made using fontCFileTest.c
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../../genFont/fontST.h"
#include "../../genFont/smallFont.h"
#include "../mkPng.h"
#include "../pngDraw.h"

int
main(
   void
){
   signed long errSL = 0;
   signed char forSC = 3;

   signed short offsetSS = 10;
   signed short tabSS = offsetSS + 20;
   signed char backSC = -1;

   struct st_mkPng *pngHeapST = 0;
   struct font_fontST fontStackST;

   FILE *outFILE = 0;

   init_font_fontST(&fontStackST);
   loadFont_smallFont(&fontStackST);

   pngHeapST = mk_st_mkPng(0, 0, 0);
   if(! pngHeapST)
      goto err_main;

   /*draw some text*/
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "A B C D E F G H I J K L M N O",
         offsetSS, /*x-axis (column)*/
         20, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "P Q R S T U V W X Y Z",
         tabSS, /*x-axis (column)*/
         40, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "a b c d e f g h i j k l m n o",
         offsetSS, /*x-axis (column)*/
         60, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "p q r s t u v w x y z",
         tabSS, /*x-axis (column)*/
         80, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "0 1 2 3 4 5 6 7 8 9",
         offsetSS, /*x-axis (column)*/
         100, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) ") ! @ # $ % ^ & * (",
         offsetSS, /*x-axis (column)*/
         120, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "< > ? : \" { } | _ + ~",
         offsetSS, /*x-axis (column)*/
         140, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   errSL =
      drawHorizText_pngDraw(
         (signed char *) ", . / ; ' [ ] \\ - = `",
         offsetSS, /*x-axis (column)*/
         160, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );

   outFILE = fopen("delete-fromC-smallFont.png", "w");
   if(! outFILE)
      goto err_main;
   print_st_mkPng(pngHeapST, outFILE);
   fclose(outFILE);
   outFILE = 0;

   if(errSL < 0)
      goto err_main;
   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;

   ret_main:;
      freeStack_font_fontST(&fontStackST);

      if(pngHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return errSL;
} /*main*/
