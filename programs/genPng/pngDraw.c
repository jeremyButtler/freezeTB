/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pngDraw SOF: Start Of File
'   - has functions to draw lines, pixels, or object on
'     a png
'   o header:
'     - included libraries
'   o fun01: addPixel_pngDraw
'     - adds a single pixel to a png (in st_mkPng struct)
'   o fun02: addBar_pngDraw
'     - adds a bar (vertical or hoziontal line) to a png
'       (in st_mkPng struct)
'   o fun03: addDiamond_pngDraw
'     - adds a diamond to a st_mkPng image
'   o fun04: drawHorizText_pngDraw
'     - draw horizontal text to a png
'   o fun05: drawVertText_pngDraw
'     - draw text vertically to a png
'   o fun06: drawVertNoAdjText_pngDraw
'     - draw text vertically to a png, but does not adjust
'       characters to be in a vertical form
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "pngDraw.h"

#include "../genLib/endin.h"
#include "../genLib/ulCp.h"

#include "../genFont/fontST.h"
#include "mkPng.h"

/*.h files only*/
#include "../genLib/64bit.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdio.h.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/checkSum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: addPixel_pngDraw
|   - adds a single pixel to a png (in st_mkPng struct)
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of pixel (index 0)
|   - ySL:
|     o y coordiante of pixel (index 0)
|   - colUC:
|     o color (0-4) to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have pixel
|   - Returns:
|     o 0 if no errors
|     o def_overflow_pngDraw if coordinates are outside of
|       grpah range
\-------------------------------------------------------*/
signed char
addPixel_pngDraw(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed char colUC          /*color of bar*/
){
   if(xSL > (signed long) pngSTPtr->widthUS)
      goto overflow_fun01;
   if(ySL > (signed long) pngSTPtr->heightUS)
      goto overflow_fun01;

   ySL *= pngSTPtr->widthUS;
   ySL += xSL;
   pngSTPtr->pixelAryUC[ySL] = colUC;

   return 0;

   overflow_fun01:;
      return def_overflow_pngDraw;
}  /*addPixel_pngDraw*/

/*-------------------------------------------------------\
| Fun02: addBar_pngDraw
|   - adds a bar to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of left conner; start of bar; index 0
|   - ySL:
|     o y coordiante of bottom of bar (index 0)
|   - widthSL:
|     o width in pixels of bar
|   - heightSL:
|     o heigth in pixels of bar
|   - colUC:
|     o index of color in pallete to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have bar
|   - Returns:
|     o 0 for no errors
|     o def_overflow_pngDraw if went out of bounds
| Note:
|   o the minimum width is at least one byte worth,
|     otherwise, do single pixel modifications
|     with addPixel_pngDraw
\-------------------------------------------------------*/
signed char
addBar_pngDraw(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed long widthSL,       /*pixels wide of bar*/
   signed long heightSL,      /*pixels high of bar*/
   signed char colUC          /*color of bar*/
){
   signed long yPosSL = 0;
   signed long byteSL = 0;
   signed long xPosSL = 0;

   if(widthSL + xSL >= (signed int) pngSTPtr->widthUS)
      goto overflow_fun02;
   if(heightSL + ySL >= (signed int) pngSTPtr->heightUS)
      goto overflow_fun02;

   byteSL = (pngSTPtr->widthUS * ySL) + xSL;

   for(yPosSL = ySL; yPosSL < ySL + heightSL; ++yPosSL)
   { /*Loop: draw a line for each row (the bar)*/
      for(xPosSL = 0; xPosSL < widthSL; ++xPosSL)
         pngSTPtr->pixelAryUC[byteSL++] = colUC;
      byteSL += pngSTPtr->widthUS - xPosSL;
   } /*Loop: draw a line for each row (the bar)*/

   return 0;

   overflow_fun02:;
      return def_overflow_pngDraw;
} /*addBar_pngDraw*/

/*-------------------------------------------------------\
| Fun03: addDiamond_pngDraw
|   - adds a diamond to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of mid point to draw the diamond
|   - ySL:
|     o y coordinate of mid point to draw the diamond
|   - widthSL:
|     o width in pixels of diamond (min 3; should be odd)
|   - heightSL:
|     o heigth in pixels of diamond (min 3; should be odd)
|   - colUC:
|     o index of color in pallete to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have a diamond
|   - Returns:
|     o 0 for no errors
|     o def_overflow_pngDraw if went out of bounds
\-------------------------------------------------------*/
signed char
addDiamond_pngDraw(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed long widthSL,       /*pixels wide of diamond*/
   signed long heightSL,      /*pixels high of diamond*/
   signed char colUC          /*color of bar*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - adds a diamond to a st_mkPng image
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - setup to draw the diamond
   '   o fun03 sec03:
   '     - draw the diamond
   '   o fun03 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long yPosSL = 0;
   signed long byteSL = 0;
   signed long xPosSL = 0;

   signed long startSL = 0;
   signed long endSL = 0;
   signed long rowStartSL = 0;

   float xDriftF = 0;
   float xF = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - setup to draw the diamond
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(widthSL + xSL >= (signed int) pngSTPtr->widthUS)
      goto overflow_fun03_sec04;
   if(heightSL + ySL >= (signed int) pngSTPtr->heightUS)
      goto overflow_fun03_sec04;

   if(widthSL >=  3)
      ;
   else if(heightSL < 3)
      addBar_pngDraw(
         pngSTPtr,
         xSL,
         ySL,
         widthSL,
         heightSL,
         colUC
      );

   /*make sure I have an odd width and height*/
   widthSL |= 1;
   heightSL |= 1;

   xDriftF = (float) widthSL / (float) heightSL;

   startSL = widthSL >> 1;
   ++startSL;
   endSL = startSL;

   rowStartSL =
      pngSTPtr->widthUS * (ySL - (heightSL >> 1));
      /*- heightSL / 2 is to account for ySL being a 
      `  midpoint
      */
   byteSL = rowStartSL + xSL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - draw the diamond
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      yPosSL = ySL - (heightSL >> 1);
      yPosSL <= ySL + (heightSL >> 1);
      ++yPosSL
   ){ /*Loop: draw a line for each row (the bar)*/
      for(xPosSL = startSL; xPosSL <= endSL; ++xPosSL)
         pngSTPtr->pixelAryUC[byteSL++] = colUC;

      if(yPosSL == ySL)
        xF = xDriftF;
      else
        xF += xDriftF;

      if(xF >= 1)
      { /*If: have enough drift to apply a change*/
         if(yPosSL >= ySL)
         { /*If: past the mid point*/
            startSL += xF;
            endSL -= xF;
         } /*If: past the mid point*/

         else
         { /*Else: moving to the midpoint*/
            startSL -= xF;
            endSL += xF;
         } /*Else: moving to the midpoint*/

         xF -= (signed int) xF;
      } /*If: have enough drift to apply a change*/

      rowStartSL += pngSTPtr->widthUS;
      byteSL =
         rowStartSL + xSL - (1 + widthSL / 2 - startSL);
   } /*Loop: draw a line for each row (the bar)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflow_fun03_sec04:;
      return def_overflow_pngDraw;
} /*addDiamond_pngDraw*/

/*-------------------------------------------------------\
| Fun04: drawHorizText_pngDraw
|   - draw horizontal text to a png
| Input:
|   - textStr:
|     o cstring with a single line of text to draw
|   - xUS:
|     o x coordinate in the png (index 0)
|   - yUS:
|     o y coordinate in the png (index 0)
|   - fgColSC:
|     o index of the forground color to use with the font
|     o use -1 for no color
|   - bgColSC:
|     o index of the background color to use with the font
|     o use -1 for no color
|   - fontSTPtr:
|     o font_fontST struct pionter to get the new font
|   - pngSTPtr:
|     o st_mkPng struct pointer to draw the text to
| Output:
|   - Modifies:
|     o pngSTPtr to have the drawn text
|   - Returns:
|     o column (x-axis) ended on (>= 0) for no errors
|     o -1 if the coordinates were out of bounds
|     o -2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed int
drawHorizText_pngDraw(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_fontST *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - draw text horizontally to a png
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - setup the color long and get initial position
   '   o fun04 Sec03:
   '     - draw the text
   '   o fun04 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char charUC = 0;   /*character to add*/
   unsigned char pixelsUC = 0; /*has pixels for font*/
   signed char colSC = 0;

   signed long byteSL = 0;     /*byte adding to*/
   signed long endRowSL = 0;   /*byte adding to*/
   signed long nextRowSL = 0;  /*start of the next row*/
   signed short rowPixSS = 0;

   signed short charByteSS = 0;/*has one byte of pixel*/
   signed short heightSS = 0;  /*number of rows printed*/
   signed short pixSS = 0;     /*the pix on in my byte*/
   signed short widthSS = 0;   /*width of the gap*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - setup the color long and get initial position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(xUS >= pngSTPtr->widthUS)
      goto overflowErr_fun04_sec04;
   if(yUS >= pngSTPtr->heightUS)
      goto overflowErr_fun04_sec04;

   byteSL = (yUS * pngSTPtr->widthUS) + xUS;
   nextRowSL = byteSL + pngSTPtr->widthUS;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - draw the text
   ^   o fun04 sec03 sub01:
   ^     - draw each character
   ^   o fun04 sec03 sub02:
   ^     - draw the gap between characters
   ^   o fun04 sec03 sub03:
   ^     - move to the next character
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec03 Sub01:
   *   - draw each character
   \*****************************************************/

   while(*textStr)
   { /*Loop: add the text to the png*/
      if(byteSL >= pngSTPtr->numPixelSL)
         goto overflowErr_fun04_sec04; /*out of bounds*/
      else if(*textStr < 32 || *textStr > 126)
         goto nonAsciiChar_fun04_sec04;

      rowPixSS = 0;

      charUC =
         (unsigned char)
         *textStr - def_asciiOffset_fontST;

      endRowSL = byteSL % pngSTPtr->widthUS;/*x position*/
      endRowSL += fontSTPtr->widthArySS[charUC];
      if(endRowSL > pngSTPtr->widthUS)
         goto overflowErr_fun04_sec04; /*out of bounds*/

      ++textStr;
      endRowSL = 0;
      pixSS = 0;
      rowPixSS = 0; /*keep track of pixel on in the row*/

      for(
         charByteSS = 0;
         charByteSS < fontSTPtr->lenArySS[charUC];
         ++charByteSS
      ){ /*Loop: fill in pixels for the character*/
         pixelsUC =
            fontSTPtr->pixAryUC[charUC][charByteSS];

         for(
            pixSS = 0;
            pixSS < def_bitsPerChar_64bit;
            ++pixSS
         ){ /*Loop: fill each row of the character*/
            if(rowPixSS >= fontSTPtr->widthArySS[charUC])
               break;

            addPixels_fun04_sec03_sub01:;
              if(pixelsUC & 1)
                 colSC = fgColSC;
              else
                 colSC = bgColSC;

              if(colSC >= 0)
                 pngSTPtr->pixelAryUC[byteSL] = colSC;

              pixelsUC >>= 1;
              ++byteSL;
              ++rowPixSS;
         }  /*Loop: fill each row of the character*/

         /***********************************************\
         * Fun04 Sec03 Sub02:
         *   - draw the gap between characters
         \***********************************************/

         if(! *textStr)
            ;
         else if(rowPixSS >=fontSTPtr->widthArySS[charUC])
         { /*If: need to print the gap*/
            for(
               widthSS = 0;
               widthSS < fontSTPtr->gapSS;
               ++widthSS
            ){ /*Loop: add the gap between characters*/
               if(bgColSC >= 0)
                  pngSTPtr->pixelAryUC[byteSL] = bgColSC;
               ++byteSL;
            }  /*Loop: add the gap between characters*/
         } /*If: need to print the gap*/

         if(rowPixSS >=fontSTPtr->widthArySS[charUC])
         { /*If: need to move to the next row*/
            if(! endRowSL)
               endRowSL = byteSL;
            rowPixSS = 0;

            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;
            ++heightSS;
         } /*If: need to move to the next row*/

         if(heightSS >= fontSTPtr->heightSS)
            break; /*done with the character*/
         else if(nextRowSL > pngSTPtr->numPixelSL)
            goto overflowErr_fun04_sec04;
         else if(pixSS < def_bitsPerChar_64bit)
            goto addPixels_fun04_sec03_sub01;
            /*the byte still has pixels left*/
      }  /*Loop: fill in pixels for the character*/

      /**************************************************\
      * Fun04 Sec03 Sub03:
      *   - move to the next character
      \**************************************************/

      if(! *textStr)
         break; /*done*/

      byteSL = endRowSL;
      nextRowSL = endRowSL + pngSTPtr->widthUS;
      heightSS = 0;
      pixSS = 0;

      if(! *textStr)
         ;
      else if(rowPixSS >=fontSTPtr->widthArySS[charUC])
         goto overflowErr_fun04_sec04;
   } /*Loop: add the text to the png*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   yUS = byteSL / pngSTPtr->widthUS;
      /*get the row on; I am reallying on truncation to
      `  floor the number
      */
   byteSL -= (yUS * pngSTPtr->widthUS);
      /*get the start of the row on and subtract from the
      `  position
      */
   return byteSL; /*column on (x-axis)*/

   overflowErr_fun04_sec04:;
      return -1;
   nonAsciiChar_fun04_sec04:;
      return -2;
} /*drawHorizText_pngDraw*/

/*-------------------------------------------------------\
| Fun05: drawVertText_pngDraw
|   - draw text vertically to a png
| Input:
|   - textStr:
|     o cstring with a single line of text to draw
|   - xUS:
|     o x coordinate in the png (index 0)
|   - yUS:
|     o y coordinate in the png (index 0)
|   - fgColSC:
|     o index of the forground color to use with the font
|     o use -1 for no color
|   - bgColSC:
|     o index of the background color to use with the font
|     o use -1 for no color
|   - fontSTPtr:
|     o font_fontST struct pionter to get the new font
|   - pngSTPtr:
|     o st_mkPng struct pointer to draw the text to
| Output:
|   - Modifies:
|     o pngSTPtr to have the drawn text
|   - Returns:
|     o 0 for no errors
|     o 1 if the coordinates were out of bounds
|     o 2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed int
drawVertText_pngDraw(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_fontST *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - draw text vertitically to a png
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - setup the color long and get initial position
   '   o fun05 sec03:
   '     - draw the text
   '   o fun05 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char charUC = 0;  /*character to add*/
   unsigned char pixelsUC = 0;/*has pixels to add*/
   signed char colSC = 0;     /*color to use for a pixel*/

   signed long byteSL = 0;    /*byte adding to*/
   signed long nextRowSL = 0; /*start of the next row*/
   signed short rowPixSS = 0;
      /*last pixel for char in a row*/

   signed short charByteSS = 0;/*has one byte of pixel*/
   signed short heightSS = 0; /*number of rows printed*/
   signed short pixSS = 0;    /*the pix on in my byte*/
   signed short pixPosSS = 0;

   signed long tmpSL = 0;
   signed int lenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - setup the color long and get initial position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(xUS >= pngSTPtr->widthUS)
      goto overflowErr_fun05_sec04;
   if(yUS >= pngSTPtr->heightUS)
      goto overflowErr_fun05_sec04;

   byteSL = (yUS * pngSTPtr->widthUS) + xUS;
   nextRowSL = byteSL + pngSTPtr->widthUS;
   lenSI = endStr_ulCp(textStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - draw the text
   ^   o fun05 sec03 sub01:
   ^     - print the character
   ^   o fun05 sec03 sub02:
   ^     - print the gap between characters
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - print the character
   \*****************************************************/

   textStr = textStr + lenSI - 1;
   --lenSI;

   while(lenSI >= 0)
   { /*Loop: add the text to the png*/
      if(*textStr < 32 || *textStr > 126)
         goto nonAsciiChar_fun05_sec04;
      heightSS = 0;

      charUC =
         (unsigned char)
         *textStr - def_asciiOffset_fontST;
      --textStr;
      if(nextRowSL >= pngSTPtr->numPixelSL)
         goto overflowErr_fun05_sec04;

      tmpSL = byteSL % pngSTPtr->widthUS;/*x position*/
      tmpSL += fontSTPtr->heightSS;
      if(tmpSL > pngSTPtr->widthUS)
         goto overflowErr_fun05_sec04;

      for(
         charByteSS = fontSTPtr->widthArySS[charUC] - 1;
         charByteSS >= 0;
         --charByteSS
      ){ /*Loop: fill in pixels for the character*/
         for(
            rowPixSS = 0;
            rowPixSS < fontSTPtr->heightSS;
            ++rowPixSS
         ){ /*Loop: draw one column of pixels*/
            pixSS =
               fontSTPtr->widthArySS[charUC] * rowPixSS;
               /*get the pixel I need to get*/
            pixSS += charByteSS;
               /*add in the column I am on*/
            pixPosSS = pixSS % def_bitsPerChar_64bit;
               /*pixel I am on in the byte*/
            pixSS /= def_bitsPerChar_64bit;
               /*the byte I am on*/
            
            pixelsUC = fontSTPtr->pixAryUC[charUC][pixSS];
            pixelsUC &= (1 << pixPosSS);

            if(pixelsUC)
               colSC = fgColSC;
            else
               colSC = bgColSC;

            if(colSC >= 0)
               pngSTPtr->pixelAryUC[byteSL] = colSC;

            ++byteSL;
         }  /*Loop: fill each row of the character*/


         for(
           pixSS = 0;
           pixSS < fontSTPtr->gapSS;
           ++pixSS
         ){ /*Loop: add the gap between characters*/
             if(bgColSC >= 0)
                pngSTPtr->pixelAryUC[byteSL] = bgColSC;
             ++byteSL;
         }  /*Loop: add the gap between characters*/

         /*I need to keep track of the next row*/
         byteSL = nextRowSL;
         nextRowSL += pngSTPtr->widthUS;

         if(nextRowSL >= pngSTPtr->numPixelSL)
            goto overflowErr_fun05_sec04;
         else if(heightSS >= fontSTPtr->heightSS)
            break; /*done with the character*/
      }  /*Loop: fill in pixels for the character*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - print the gap between characters
      \**************************************************/

      if(lenSI)
      { /*If: need to print the gap*/
         for(
            heightSS = 0;
            heightSS < fontSTPtr->gapSS;
            ++heightSS
         ){ /*Loop: add in the gap for vertical text*/
            for(
              pixSS = 0;
              pixSS < fontSTPtr->widthArySS[charUC];
              ++pixSS
            ){ /*Loop: add the gap between characters*/
                if(bgColSC >= 0)
                   pngSTPtr->pixelAryUC[byteSL] = bgColSC;
                ++byteSL;
            }  /*Loop: add the gap between characters*/

            /*I need to keep track of the next row*/
            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;

            if(! *textStr)
               ;
            else if(nextRowSL >= pngSTPtr->numPixelSL)
               goto overflowErr_fun05_sec04;
         }  /*Loop: add in the gap for vertical text*/
      } /*If: need to print the gap*/

      --lenSI;
   } /*Loop: add the text to the png*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return byteSL / pngSTPtr->widthUS;
      /*get the row on; I am reallying on truncation to
      `  floor the number
      */

   overflowErr_fun05_sec04:;
      return -1;

   nonAsciiChar_fun05_sec04:;
      return -2;
} /*drawVertText_pngDraw*/

/*-------------------------------------------------------\
| Fun06: drawVertNoAdjText_pngDraw
|   - draw text vertically to a png, but does not adjust
|     characters to be in a vertical form
| Input:
|   - textStr:
|     o cstring with a single line of text to draw
|   - xUS:
|     o x coordinate in the png (index 0)
|   - yUS:
|     o y coordinate in the png (index 0)
|   - fgColSC:
|     o index of the forground color to use with the font
|     o use -1 for no color
|   - bgColSC:
|     o index of the background color to use with the font
|     o use -1 for no color
|   - fontSTPtr:
|     o font_fontST struct pionter to get the new font
|   - pngSTPtr:
|     o st_mkPng struct pointer to draw the text to
| Output:
|   - Modifies:
|     o pngSTPtr to have the drawn text
|   - Returns:
|     o 0 for no errors
|     o 1 if the coordinates were out of bounds
|     o 2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed int
drawVertNoAdjText_pngDraw(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_fontST *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - draw text vertically to a png, but does not
   '     adjust characters to be in a vertical form
   '   o fun06 sec01:
   '     - variable declarations
   '   o fun06 sec02:
   '     - setup the color long and get initial position
   '   o fun06 sec03:
   '     - draw the text
   '   o fun06 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char charUC = 0;  /*character to add*/
   unsigned char pixelsUC = 0;/*has pixels to add*/
   signed char colSC = 0;     /*color to use for a pixel*/

   signed long byteSL = 0;    /*byte adding to*/
   signed long nextRowSL = 0; /*start of the next row*/
   signed short rowPixSS = 0;
      /*last pixel for char in a row*/

   signed short charByteSS = 0;/*has one byte of pixel*/
   signed short heightSS = 0; /*number of rows printed*/
   signed short pixSS = 0;    /*the pix on in my byte*/

   signed long tmpSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - setup the color long and get initial position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(xUS >= pngSTPtr->widthUS)
      goto overflowErr_fun06_sec04;
   if(yUS >= pngSTPtr->heightUS)
      goto overflowErr_fun06_sec04;

   byteSL = (yUS * pngSTPtr->widthUS) + xUS;
   nextRowSL = byteSL + pngSTPtr->widthUS;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - draw the text
   ^   o fun06 sec03 sub01:
   ^     - print the character
   ^   o fun06 sec03 sub02:
   ^     - print the gap between characters
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec03 Sub01:
   *   - print the character
   \*****************************************************/

   while(*textStr)
   { /*Loop: add the text to the png*/
      if(*textStr < 32 || *textStr > 126)
         goto nonAsciiChar_fun06_sec04;
      heightSS = 0;

      charUC =
         (unsigned char)
         *textStr - def_asciiOffset_fontST;
      ++textStr;
      rowPixSS = 0; /*keep track of pixel on in the row*/
      if(nextRowSL >= pngSTPtr->numPixelSL)
         goto overflowErr_fun06_sec04;

      tmpSL = byteSL % pngSTPtr->widthUS;/*x position*/
      tmpSL += fontSTPtr->widthArySS[charUC];
      if(tmpSL > pngSTPtr->widthUS)
         goto overflowErr_fun06_sec04;

      for(
         charByteSS = 0;
         charByteSS < fontSTPtr->lenArySS[charUC];
         ++charByteSS
      ){ /*Loop: fill in pixels for the character*/
         pixelsUC =
            fontSTPtr->pixAryUC[charUC][charByteSS];

         for(
            pixSS = 0;
            pixSS < def_bitsPerChar_64bit;
            ++pixSS
         ){ /*Loop: fill each row of the character*/
            if(rowPixSS >= fontSTPtr->widthArySS[charUC])
               break;

            addPixels_fun06_sec03_sub01:;
               if(pixelsUC & 1)
                  colSC = fgColSC;
               else
                  colSC = bgColSC;

               if(colSC >= 0)
                  pngSTPtr->pixelAryUC[byteSL] = colSC;

               pixelsUC >>= 1;
               ++byteSL;
               ++rowPixSS;
         }  /*Loop: fill each row of the character*/


         if(rowPixSS >= fontSTPtr->widthArySS[charUC])
         { /*If: I need to move to the next line*/
            rowPixSS = 0;
            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;
            ++heightSS;

            if(nextRowSL >= pngSTPtr->numPixelSL)
               goto overflowErr_fun06_sec04;
         } /*If: I need to move to the next line*/

         if(heightSS >= fontSTPtr->heightSS)
            break; /*done with the character*/
         else if(pixSS < def_bitsPerChar_64bit)
            goto addPixels_fun06_sec03_sub01;
            /*this reduces the amount of duplicate code*/
      }  /*Loop: fill in pixels for the character*/

      /**************************************************\
      * Fun06 Sec03 Sub02:
      *   - print the gap between characters
      \**************************************************/

      if(*textStr)
      { /*If: need to print the gap*/
         for(
            heightSS = 0;
            heightSS < fontSTPtr->gapSS;
            ++heightSS
         ){ /*Loop: add in the gap for vertical text*/
            for(
              pixSS = 0;
              pixSS < fontSTPtr->widthArySS[charUC];
              ++pixSS
            ){ /*Loop: add the gap between characters*/
                if(bgColSC >= 0)
                   pngSTPtr->pixelAryUC[byteSL] = bgColSC;
                ++byteSL;
            }  /*Loop: add the gap between characters*/

            /*I need to keep track of the next row*/
            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;

            if(! *textStr)
               ;
            else if(nextRowSL >= pngSTPtr->numPixelSL)
               goto overflowErr_fun06_sec04;
         }  /*Loop: add in the gap for vertical text*/
      } /*If: need to print the gap*/
   } /*Loop: add the text to the png*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return byteSL / pngSTPtr->widthUS;
      /*get the row on; I am reallying on truncation to
      `  floor the number
      */

   overflowErr_fun06_sec04:;
      return -1;

   nonAsciiChar_fun06_sec04:;
      return -2;
} /*drawVertNoAdjText_pngDraw*/


/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
