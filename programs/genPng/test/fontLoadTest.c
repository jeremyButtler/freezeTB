/*The main goal of this function is to make sure that
`   that there are not errors on font loading. It is not
`   here to check the accuracy
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
   FILE *fontFILE =
      fopen("../../genFont/asciiNormalFont.txt", "r");

   init_font_fontST(&fontStackST);
   if(! fontFILE)
      goto err_main;


   errSL = getFont_font_fontST(&fontStackST, fontFILE);

   if(! errSL)
      ;
   else if(errSL == -1)
   { /*Else If: height problem*/
      fprintf(stderr, "could not get font height\n");
      goto err_main;
   } /*Else If: height problem*/

   else if(errSL == -2)
   { /*Else If: memory error*/
      fprintf(stderr, "memory error getting font\n");
      goto err_main;
   } /*Else If: memory error*/

   else if(errSL < 0)
   { /*Else If: duplicate character*/
      errSL *= -1;
      errSL -= 4;
      fprintf(stderr,"duplicate char; line %li\n",errSL);
      goto err_main;
   } /*Else If: duplicate character*/

   else
   { /*Else: bad character on a line*/
      fprintf(stderr, "bad char on line %li\n", errSL);
      goto err_main;
   } /*Else: bad character on a line*/

   /*make sure my test does not get optimized out*/
   printf("font height: %i\n", fontStackST.heightSS);

   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;

   ret_main:;
      freeStack_font_fontST(&fontStackST);

      if(fontFILE)
         fclose(fontFILE);
      fontFILE = 0;

      return errSL;
} /*main*/
