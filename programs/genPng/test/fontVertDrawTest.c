/*goal is to find errors in my font drawing function*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../../genFont/fontST.h"
#include "../mkPng.h"
#include "../pngDraw.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_fontST fontStackST;
   struct st_mkPng *pngHeapST = 0;
   FILE *outFILE =
      fopen("../../genFont/asciiNormalFont.txt", "r");

   init_font_fontST(&fontStackST);
   if(! outFILE)
      goto err_main;

   errSL = getFont_font_fontST(&fontStackST, outFILE);
   if(errSL)
      goto err_main;
   fclose(outFILE);
   outFILE = 0;

   pngHeapST = mk_st_mkPng(0, 0, 0);
   if(! pngHeapST)
      goto err_main;

   /*two color draw*/
   errSL =
      drawVertText_pngDraw(
         (signed char *) "this is a test ~123",
         10, /*x-axis (column)*/
         20, /*y-axis (row)*/
         1,  /*color of 1*/
         3,  /*background color of three*/
         &fontStackST,
         pngHeapST
      );
   if(errSL < 0)
      goto err_main;

   /*transparent background draw*/
   errSL =
      drawVertText_pngDraw(
         (signed char *) "Transparent BackGround Test 1~",
         44, /*x-axis (column)*/
         20, /*y-axis (row)*/
         3,  /*color of 1*/
         -1, /*transparent background color*/
         &fontStackST,
         pngHeapST
      );
   if(errSL < 0)
      goto err_main;

   /*transparent forground draw*/
   errSL =
      drawVertText_pngDraw(
         (signed char *) "transparent forground test z~",
         68, /*x-axis (column)*/
         20, /*y-axis (row)*/
         -1, /*color of 1*/
         3,  /*darker background color*/
         &fontStackST,
         pngHeapST
      );
   if(errSL < 0)
      goto err_main;

   outFILE = fopen("delete-vert-font.png", "w");
   if(! outFILE)
      goto err_main;
   print_st_mkPng(pngHeapST, outFILE);

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

      return (int) errSL;
} /*main*/
