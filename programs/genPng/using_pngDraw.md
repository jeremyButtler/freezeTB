# pngDraw

Files: pngDraw.h and pngDraw.c

- Dependencies:
  - ../genLib/endin.h
  - ../genFont/fontST.h
  - pngDraw.h
  - mkPng.h
  - ../genLib/64bit.h (.h file only)
- Hidden Dependencies:
  - ../genLib/base10str.h
  - ../genLib/ulCp.h
  - ../genLib/checkSum.h

## Drawing text

You can draw vertical or horizontal text. Newlines,
  carriage returns, and tabs are not allowed.

To draw text horizontally use the
  `drawHorzText_pngDraw()` function.

To draw text vertically use the
  `drawVertText_pngDraw()` function.

Both the horizontal and vertical text draw functions take
  the same input and return the same errors.

- Input:
  1. c-string with the text to draw
  2. x coordinate to start drawing the text at
  3. y coordinate to start drawing the text at
  4. forground color (as a png pallete index) to use
     - use -1 to not print the forground
  5. background color (as a png pallete index) to use
     - use -1 to not print the background
  6. pointer to a font\_pngFont structure with the font to
     use
  7. pointer to a st\_mkPng structure with the png
- Returns:
  - 0 for no errors
  - 1 if the text when outside the png bounds (overflow)
  - 2 if the text had an invalid character
    - not ascii 32 to 126

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "pngFont.h"
#include "pngDefaultFont.h"
#include "mkPng.h"

int
main(
   void
){
   signed char errSC = 0;
   struct st_mkPng *pngHeapST = mkPng(700, 700, 4);
   struct font_pngFont fontStackST;
   
   init_font_pngFont(&fontStackST);
   
   if(! pngSTPtr)
      goto err_main;
   if( loadFont_pngDefaultFont(&fontStackST) )
      goto err_main;
   
   drawHorzText_font_pngFont(
      (signed char *) "this is some vertical text",
      100, /*100 pixels on the x-axis*/
      20,  /*20 pixels up*/
      3,   /*set the forground color to the third color*/
      -1,  /*do not print the background*/
      &fontStackST,
      pngHeapST
   ); /*draw the text horizontally*/
   
   drawVertText_font_pngFont(
      (signed char *) "this is some text",
      0,  /*start at x-colomn 0*/
      20, /*20 pixels up*/
      -1, /*do not print the forground*/
      3,  /*set the background to the thrid color*/
      &fontStackST,
      pngHeapST
   ); /*draw the text vertically*/
   
   /*print the png out (not sure why you would do this)*/
   print_st_mkPng(pngHeapST, stdout);
   
   errSC = 0;
   goto ret_main;
   
   err_main;;
      errSC = 1;   
      goto ret_main;
   
   ret_main:;
      freeStack_font_pngFont(&fontStackST);
   
      if(pntHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;
   
      return errSC;
} /*main*/
```

