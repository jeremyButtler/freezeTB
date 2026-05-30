/*goal is to find errors in my font output function for
`  .c and .h files
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../../genFont/fontST.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_fontST fontStackST;
   FILE *outFILE =
      fopen("../../genFont/asciiSmallFont.txt", "r");

   init_font_fontST(&fontStackST);
   if(! outFILE)
      goto err_main;

   errSL = getFont_font_fontST(&fontStackST, outFILE);
   if(errSL)
      goto err_main;
   fclose(outFILE);
   outFILE = 0;

   /*save the font to delete.c and delete.h*/
   if(
      fontToC_font_fontST(
         &fontStackST,
         (signed char *) "../../genFont/smallFont"
      )
   ) goto err_main;

   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;

   ret_main:;
      freeStack_font_fontST(&fontStackST);

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return (int) errSL;
} /*main*/
