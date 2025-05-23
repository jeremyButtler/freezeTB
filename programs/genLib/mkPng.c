/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mkPng SOF:
'   - make graphs for ampDepth
'   - a good amount of this code came from misc0110's
'     libattopng (https://github.com/misc0110/libattpng),
'     which is under the MIT license
'   - the parts not from libattopng were inspired by it
'   o .h st01: st_mkPng
'     - has png image for mkPng
'   o fun01: blank_st_mkPng
'     - blanks the pixel matrix in a st_mkPng struct
'   o .c fun02 calcDepth_st_mkPng
'     - finds minimum bit depth for a st_mkPng struct
'   o fun03: addCol_st_mkPng
'     - add a color to a st_mkPng struct
'   o fun04: rmCol_st_mkPng
'     - remove a color from a st_mkPng struct
'   o fun05: initCol_st_mkPng
'     - initialize color array with x greyscale colors
'   o fun06: init_st_mkPng
'     - initialize a st_mkPng struct
'   o fun07: freeStack_st_mkPng
'     - frees variables in a st_mkPng struct
'   o fun08: freeHeap_st_mkPng
'     - frees a st_mkPng struct
'   o fun10: mk_st_mkPng
'     - makes a st_mkPng struct on heap
'   o fun11: addPixel_st_mkPng
'     - adds a single pixel to a st_mkPng image
'   o fun12: addBar_st_mkPng
'     - adds a bar to a st_mkPng image
'   o .c fun16: addUint_mkPng
'     - adds a uint to a png buffer
'   o .c fun17: pIHDR_st_mkPng
'     - add the IHDR header to a st_mkPng struct
'   o .c fun18: pPLTE_st_mkPng
'     - add pallete (PLTE) header to a st_mkPng struct
'   o .c fun19: pIDAT_st_mkPng
'     - add image IDAT header
'     - copied from misc0110's libattpng repository
'   o .c fun20: pIEND_st_mkPng
'     - add end header (IEND) for png 
'   o fun21: print_st_mkPng
'     - prints a png to output file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #inlcude <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "mkPng.h"

#include <stdio.h>

#include "../genLib/endin.h"
#include "../genLib/checkSum.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*default color scheme*/
   /*white*/
   #define def_1stColRed_mkPng 0xFF
   #define def_1stColBlu_mkPng 0xFF
   #define def_1stColGre_mkPng 0xFF
   
   /*magma yellow*/
   #define def_2ndColRed_mkPng 0xFD
   #define def_2ndColBlu_mkPng 0xE7
   #define def_2ndColGre_mkPng 0x25
   
   /*magma pink*/
   #define def_3rdColRed_mkPng 0xF1
   #define def_3rdColBlu_mkPng 0x60
   #define def_3rdColGre_mkPng 0x5D
   
   /*magma dark purple*/
   #define def_4thColRed_mkPng 0x00
   #define def_4thColBlu_mkPng 0x00
   #define def_4thColGre_mkPng 0x04

/*-------------------------------------------------------\
| Fun01: blank_st_mkPng
|   - blanks the pixel matrix in a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to blank
| Output:
|   - Modifies:
|     o pixelAryUC to be full of zeros
\-------------------------------------------------------*/
void
blank_st_mkPng(
   struct st_mkPng *pngSTPtr
){
   signed long slPix = 0;
   ul_mkPng *pixAryUL = 0;

   if(! pngSTPtr)
      return;

   if(pngSTPtr->pixelAryUC)
   { /*If: need to blank pixel matrix*/

      /*not as fast as SIMD, but faster then byte by byte
      `  and is always present
      */
      pixAryUL = (ul_mkPng *) pngSTPtr->pixelAryUC;

      for(
         slPix = 0;
         slPix < pngSTPtr->sizePixelSL;
         ++slPix
      ) pixAryUL[slPix] = 0;

   } /*If: need to blank pixel matrix*/
} /*blank_st_mkPng*/

/*-------------------------------------------------------\
| Fun02 calcDepth_st_mkPng
|   - finds minimum bit depth for a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to get bitdept for
| Output:
|   - Modifies:
|     o pixDepthUC in pngSTPtr to have bit depth
|     o pixPerByteUC in pngSTPtr to have number pixels in
|       one byte
|     o shiftUC in pngSTPtr to have the correct shift
|       value in pngSTPtr to convert byte count to pixel
|       count
|     o modUC in pngSTPtr to have modvalue to convert
|       pixel coordinate into byte position; coord % modUC
|   - Returns:
|     o 0 if color depth is unchaged
|     o 1 if color depth is chaged
\-------------------------------------------------------*/
signed char
calcDepth_st_mkPng(
   struct st_mkPng *pngSTPtr /*has bit depth values*/
){
   signed char changeSC = 0;

   if(pngSTPtr->numColUC > 16)
   { /*If: 8 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 8)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 8;
      pngSTPtr->pixPerByteUC = 1;
      pngSTPtr->shiftUC = 0;
      pngSTPtr->modUC = 1;
   } /*If: 8 bit color scheme*/

   else if(pngSTPtr->numColUC > 4)
   { /*Else If: 4 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 4)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 4;
      pngSTPtr->pixPerByteUC = 2;
      pngSTPtr->shiftUC = 1;
      pngSTPtr->modUC = 2;
   } /*Else If: 4 bit color scheme*/

   else if(pngSTPtr->numColUC > 2)
   { /*Else If: 2 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 2)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 2;
      pngSTPtr->pixPerByteUC = 4;
      pngSTPtr->shiftUC = 2;
      pngSTPtr->modUC = 4;
   } /*Else If: 2 bit color scheme*/

   else
   { /*Else: 1 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 1)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 1;
      pngSTPtr->pixPerByteUC = 8;
      pngSTPtr->shiftUC = 3;
      pngSTPtr->modUC = 8;
   } /*Else: 1 bit color scheme*/

   return changeSC;
}  /*calcDepth_st_mkPng*/

/*-------------------------------------------------------\
| Fun03: addCol_st_mkPng
|   - add a color to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add color to
|   - redUC:
|     o 0-255 value for red channel
|   - bluUC:
|     o 0-255 value for blue channel
|   - greUC:
|     o 0-255 value for green channel
|   - indexSS:
|     o position to add index 0 (use -1 for end)
|       * -1 for append to end
|       * if > end, will append to end
|       * if > -1 and < end will overwrite color at index
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have last value set to input color
|     o numColUC in pngSTPtr to have input color
|     o sets errSCPtr:
|       * 0 for no errors
|       * def_overflow_mkPng for color array overflow
|       * def_bitChange_mkPng if color bit depth incresed
|         (ex; from 2bit to 4bit)
|     o calls calcDepth_st_mkPng to reset bit values if
|       color is appened to end
|   - Returns:
|     o index of added color
|       * if no errors
|       * also if had an increase in bit depth
|     o -1 if had an error
\-------------------------------------------------------*/
signed short
addCol_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add color to*/
   unsigned char redUC,       /*red color channel*/
   unsigned char bluUC,       /*blue color channel*/
   unsigned char greUC,       /*green color channel*/
   signed short indexSS,      /*index to add value to*/
   signed char *errSCPtr      /*holds errors*/
){
   *errSCPtr = 0;

   if(indexSS < 0)
      indexSS = pngSTPtr->numColUC;
   else if(indexSS > pngSTPtr->numColUC)
      indexSS = pngSTPtr->numColUC;

   if(indexSS > def_maxCol_mkPng)
      goto overflow_fun03;


   pngSTPtr->redAryUC[indexSS] = redUC;
   pngSTPtr->bluAryUC[indexSS] = bluUC;
   pngSTPtr->greAryUC[indexSS] = greUC;


   if( indexSS >= pngSTPtr->numColUC )
   { /*If: added a new color*/
      ++(pngSTPtr->numColUC);

      /*check if bit depth is same*/
      if( calcDepth_st_mkPng(pngSTPtr) )
         *errSCPtr = def_bitChange_mkPng;
   } /*If: added a new color*/


   goto ret_fun03;

   overflow_fun03:;
      indexSS = -1;
      *errSCPtr = def_overflow_mkPng;
      goto ret_fun03;
   
   ret_fun03:;
      return indexSS;
} /*addCol_st_mkPng*/
   
/*-------------------------------------------------------\
| Fun04: rmCol_st_mkPng
|   - remove a color from a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to remove color from
|   - indexUC:
|     o index of color to remove (the next color will
|       be the color at indexUC)
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have all values past color shifted to old position
|     o numColUC in pngSTPtr to be 1 less
|     o calls calcDepth_st_mkPng to reset bit values if
|       removed color reduces bit depth
|   - Returns:
|     o 0 if bit depth stayed same or index out of range
|     o def_bitChange_mkPng it bit depth decreased
\-------------------------------------------------------*/
signed char
rmCol_st_mkPng(
   struct st_mkPng *pngSTPtr,/*to remove color from*/
   unsigned char indexUC     /*index of color to remove*/
){
   unsigned char ucCol = 0;

   if(indexUC >= pngSTPtr->numColUC)
      goto ret_fun04; /*nothing to do*/

   else if(pngSTPtr->numColUC == 1)
   { /*Else If: only one color to delete*/  
      pngSTPtr->numColUC = 0;
      goto ret_fun04; /*nothing to do*/
   } /*Else If: only one color to delete*/  

   --pngSTPtr->numColUC;

   for( 
      ucCol = 0;
      ucCol < pngSTPtr->numColUC;
      ++ucCol
   ){ /*Loop: shift next colors back*/
      pngSTPtr->redAryUC[ucCol] ^=
         pngSTPtr->redAryUC[ucCol + 1];
      pngSTPtr->redAryUC[ucCol + 1] ^=
         pngSTPtr->redAryUC[ucCol];
      pngSTPtr->redAryUC[ucCol] ^=
         pngSTPtr->redAryUC[ucCol + 1];

      pngSTPtr->bluAryUC[ucCol] ^=
         pngSTPtr->bluAryUC[ucCol + 1];
      pngSTPtr->bluAryUC[ucCol + 1] ^=
         pngSTPtr->bluAryUC[ucCol];
      pngSTPtr->bluAryUC[ucCol] ^=
         pngSTPtr->bluAryUC[ucCol + 1];

      pngSTPtr->greAryUC[ucCol] ^=
         pngSTPtr->greAryUC[ucCol + 1];
      pngSTPtr->greAryUC[ucCol + 1] ^=
         pngSTPtr->greAryUC[ucCol];
      pngSTPtr->greAryUC[ucCol] ^=
         pngSTPtr->greAryUC[ucCol + 1];
   }  /*Loop: shift next colors back*/

   ucCol = 0;

   if( calcDepth_st_mkPng(pngSTPtr) )
      ucCol = def_bitChange_mkPng;

   ret_fun04:;
      return (signed char) ucCol;
} /*rmCol_st_mkPng*/

/*-------------------------------------------------------\
| Fun05: initCol_st_mkPng
|   - initialize color array with x greyscale colors
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to add colors to
|   - numColUC:
|     o number of colors to add
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have numColUC grey colors
|     o numColUC in pngSTPtr to be input numColUC
|     o calls calcDepth_st_mkPng to reset bit values if
|       removed color reduces bit depth
|   - Returns:
|     o 0 if bit depth stayed same or index out of range
|     o def_bitChange_mkPng it bit depth decreased
\-------------------------------------------------------*/
signed char
initCol_st_mkPng(
   struct st_mkPng *pngSTPtr, /*to add all colors to*/
   unsigned char numColUC     /*number colors to add*/
){
   signed short ssCol = 0;

   for(
      ssCol = numColUC;
      ssCol > -1;
      --ssCol
   ){ /*Loop: add colors*/
      pngSTPtr->redAryUC[ssCol] = (unsigned char) ssCol;
      pngSTPtr->bluAryUC[ssCol] = (unsigned char) ssCol;
      pngSTPtr->greAryUC[ssCol] = (unsigned char) ssCol;
   }  /*Loop: add colors*/

   pngSTPtr->numColUC = numColUC;
   return calcDepth_st_mkPng(pngSTPtr);
} /*initCol_st_mkPng*/

/*-------------------------------------------------------\
| Fun06: init_st_mkPng
|   - initialize a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to initialize
| Output:
|   - Modifies:
|     o sets pixelAryUC and sizePixeSL in pngSTPtr to 0
|     o all values in pngSTPtr to be defaults
\-------------------------------------------------------*/
void
init_st_mkPng(
   struct st_mkPng *pngSTPtr
){
   signed char errSC = 0;

   if(! pngSTPtr)
      return;

   pngSTPtr->pixelAryUC = 0;
   pngSTPtr->sizePixelSL = 0;


   pngSTPtr->widthUS = def_width_mkPng;
   pngSTPtr->heightUS = def_height_mkPng;

   pngSTPtr->numPixelSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;

   /*set the color scheme*/
   addCol_st_mkPng(
      pngSTPtr,
      def_1stColRed_mkPng,
      def_1stColBlu_mkPng,
      def_1stColGre_mkPng,
      0,                   /*first index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_2ndColRed_mkPng,
      def_2ndColBlu_mkPng,
      def_2ndColGre_mkPng,
      1,                   /*second index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_3rdColRed_mkPng,
      def_3rdColBlu_mkPng,
      def_3rdColGre_mkPng,
      2,                   /*third index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_4thColRed_mkPng,
      def_4thColBlu_mkPng,
      def_4thColGre_mkPng,
      3,                   /*fourth index*/
      &errSC               /*ignore*/
   );
     

   /*set number of used bytes*/
   pngSTPtr->usedBytesSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;
   pngSTPtr->usedBytesSL >>= pngSTPtr->shiftUC;
   pngSTPtr->usedBytesSL += def_idatEnd_mkPng;


   /*called in case of future additions, but not needed*/
   blank_st_mkPng(pngSTPtr);
} /*init_st_mkPng*/

/*-------------------------------------------------------\
| Fun07: freeStack_st_mkPng
|   - frees variables in a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng with variables to free
| Output:
|   - Frees:
|     o pixelAryUC in pngSTPtr
|   - Modifies:
|     o pixelAryUC in pngSTPtr to be 0
|     o sizePixelSL in pngSTPtr to be 0
|     o all other values to default wiht init_st_mkPng
\-------------------------------------------------------*/
void
freeStack_st_mkPng(
   struct st_mkPng *pngSTPtr
){
   if(! pngSTPtr)
      return;   /*nothing input*/

   if(pngSTPtr->pixelAryUC)
      free(pngSTPtr->pixelAryUC);

   init_st_mkPng(pngSTPtr);
} /*freeStack_st_mkPng*/

/*-------------------------------------------------------\
| Fun08: freeHeap_st_mkPng
|   - frees a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to free
| Output:
|   - Frees:
|     o pngSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_st_mkPng(
   struct st_mkPng *pngSTPtr
){
   if(! pngSTPtr)
      return;

   freeStack_st_mkPng(pngSTPtr);
   free(pngSTPtr);
} /*freeHeap_st_mkPng*/

/*-------------------------------------------------------\
| Fun09: setup_st_mkPng
|   - allocates memory for a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to allocate memory
|   - widthUS:
|     o width of png in pixels
|   - heightUS:
|     o height of png in pixels
|   - maxColUC:
|     o maximum colors plan to be used in color pallete
|     o use 256 to force 8bit
|     o 0 to not change
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have needed memory
|     o sizePixeSL to have number bytes in pixelAryUC
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mkPng for memory errors
\-------------------------------------------------------*/
signed char
setup_st_mkPng(
   struct st_mkPng *pngSTPtr,
   unsigned short widthUS,
   unsigned short heightUS,
   unsigned char maxColUC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - allocates memory for a st_mkPng struct
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - set up color scheme
   '   o fun09 sec03:
   '     - setup pixel matrix memory
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   signed long numBytesSL = 0;

   if(! pngSTPtr)
      return 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - set up color scheme
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   if(maxColUC > pngSTPtr->numColUC)
      initCol_st_mkPng( pngSTPtr, maxColUC);
      /*If: need more colors then already added*/

   else if(! maxColUC)
      ; /*no colors added*/

   else if(maxColUC < pngSTPtr->numColUC)
   { /*If: need to remove colors*/
      while(pngSTPtr->numColUC < maxColUC)
         rmCol_st_mkPng(pngSTPtr, pngSTPtr->numColUC - 1);
         /*remove extra colors at end*/
   } /*If: need to remove colors*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - setup pixel matrix
   ^   o fun09 sec03 sub01:
   ^     - find matrix dimensions
   ^   o fun09 sec03 sub02:
   ^     - allocate memory
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - find matrix dimensions
   \*****************************************************/

   if(widthUS)
      pngSTPtr->widthUS = widthUS;
   if(heightUS)
      pngSTPtr->heightUS = heightUS;

   pngSTPtr->numPixelSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;

   /*make sure every row ends in a byte*/
   pngSTPtr->widthUS +=
      pngSTPtr->widthUS % pngSTPtr->pixPerByteUC;


   /*find number of pixels in png*/
   pngSTPtr->numPixelSL = pngSTPtr->widthUS;
   pngSTPtr->numPixelSL *= pngSTPtr->heightUS;
   pngSTPtr->numPixelSL >>= pngSTPtr->shiftUC;


   /*find number of bytes needed for png size*/
   numBytesSL = pngSTPtr->numPixelSL;
   numBytesSL += def_idatEnd_mkPng;
     /*extra bytes at end of idat entry*/

   /*find number used (to print) bytes*/
   pngSTPtr->usedBytesSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;
   pngSTPtr->usedBytesSL >>= pngSTPtr->shiftUC;
   pngSTPtr->usedBytesSL += def_idatEnd_mkPng;

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - allocate memory
   \*****************************************************/

   if(pngSTPtr->pixelAryUC)
   { /*If: already have memory*/
      if(numBytesSL <= pngSTPtr->sizePixelSL)
      { /*If: already have needed memory*/
         blank_st_mkPng(pngSTPtr);
         return 0;
      }  /*If: already have needed memory*/

      else
      { /*Else: do not have enough space*/
         free(pngSTPtr->pixelAryUC);
         pngSTPtr->pixelAryUC = 0;
      } /*Else: do not have enough space*/
   } /*If: already have memory*/


   pngSTPtr->sizePixelSL = numBytesSL;

   pngSTPtr->pixelAryUC =
      calloc(
         (pngSTPtr->numPixelSL + 8), /*+8 for long copy*/
         sizeof(unsigned char)
      );


   if(! pngSTPtr->pixelAryUC)
      return def_memErr_mkPng;
   return 0;
} /*setup_st_mkPng*/

/*-------------------------------------------------------\
| Fun10: mk_st_mkPng
|   - makes a st_mkPng struct on heap
| Input:
|   - widthUS:
|     o width of graph (0 for default)
|   - heightUS:
|     o heigth of graph (0 for default)
|   - maxColUC:
|     o maximum colors plan to be used in color pallete
|     o use 256 to force 8bit
|     o 0 to not change
| Output:
|   - Returns:
|     o pointer to st_mkPng struct
|     o 0 for no errors
\-------------------------------------------------------*/
struct st_mkPng*
mk_st_mkPng(
   unsigned short widthUS,
   unsigned short heightUS,
   unsigned char maxColUC
){
   struct st_mkPng *retHeapST = 0;

   retHeapST = malloc(sizeof(struct st_mkPng));

   if(! retHeapST)
      goto memErr_fun10;

   init_st_mkPng(retHeapST);

   if(
      setup_st_mkPng(retHeapST,widthUS,heightUS,maxColUC)
   ) goto memErr_fun10;


   goto ret_fun10;

   memErr_fun10:;
      if(retHeapST)
         freeHeap_st_mkPng(retHeapST);
      retHeapST = 0;
      goto ret_fun10;

   ret_fun10:;
     return retHeapST;
} /*mk_st_mkPng*/

/*-------------------------------------------------------\
| Fun11: addPixel_st_mkPng
|   - adds a single pixel to a st_mkPng image
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
|     o def_overflow_mkPng if coordinates are outside of
|       grpah range
\-------------------------------------------------------*/
signed char
addPixel_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed char colUC          /*color of bar*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 addBar_st_mkPng
   '   - adds a single pixel to a st_mkPng image
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - set up mask
   '   o fun11 sec02:
   '     - create color stamp
   '   o fun11 sec03:
   '     - add color
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*doing byte copies to get my system down, latter I
   `   plan to add multi-byte steps
   */
   signed long startSL = xSL + (ySL * xSL);
   unsigned char bitUC = 0;  /*bit pixel is at*/
   unsigned char maskUC = 0;
   unsigned char *byteUCPtr = 0;
   

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - set up mask
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(startSL >= pngSTPtr->numPixelSL)
      return def_overflow_mkPng;

   /*find bit for pixel*/
   bitUC = (startSL & pngSTPtr->pixPerByteUC);
   bitUC <<= pngSTPtr->shiftUC;
   /*Logic:
   `   - targeting second pixel (xx) in 11xx 1111
   `   - or second pixel in 4 bit (xxxx) 1111 xxxx
   `   - pixPos: startSL & pixPerByteUC:
   `     o pixel position of byte, deals with larg index's
   `     o 1bit: 7 & 8 = 7
   `     o 1bit: 6 & 8 = 6
   `     o 1bit: 5 & 8 = 5
   `     o 1bit: 4 & 8 = 4
   `     o 1bit: 3 & 8 = 3
   `     o 1bit: 2 & 8 = 2
   `     o 1bit: 1 & 8 = 1
   `     o 1bit: 0 & 8 = 0

   `     o 2bit: 0 & 4 = 0
   `     o 2bit: 3 & 4 = 3
   `     o 2bit: 2 & 4 = 2
   `     o 2bit: 1 & 4 = 1
   `     o 2bit: 0 & 4 = 0
   `
   `     o 4bit: 1 & 2 = 1
   `     o 4bit: 0 & 2 = 0
   `
   `     o 8bit: 1 & 1 = 0
   `
   `     o large index example 102 & 0xff = 2
   `   - shift << shiftUC
   `     o this gives the position of the first bit of
   `       the target pixel
   `     o 1bit: 8 << 0 = 8
   `     o 1bit: 7 << 0 = 7
   `     o 1bit: 6 << 0 = 6
   `     o 1bit: 5 << 0 = 5
   `     o 1bit: 4 << 0 = 4
   `     o 1bit: 3 << 0 = 3
   `     o 1bit: 2 << 0 = 2
   `     o 1bit: 1 << 0 = 1
   `     o 1bit: 0 << 0 = 0

   `     o 2bit: 3 << 1 = 6
   `     o 2bit: 2 << 1 = 4
   `     o 2bit: 1 << 1 = 2
   `     o 2bit: 0 << 1 = 0

   `     o 4bit: 1 << 2 = 4
   `     o 4bit: 0 << 2 = 0
   `
   `     o 8bit: 0 << 2 = 0
   */

   maskUC =
          ( (unsigned char) -1 )
       >> ( (sizeof(unsigned char) << 3) - bitUC );
   /*Logic:
   `   - (sizeof(uchar) << 3) - bitUC
   `     o sizeof(uchar) << 3 goes to 8
   `     o 8 - keepBit is target pixel bit + bits after
   `     o 1bit, 7th bit: 8 - 7 = 1
   `     o 1bit, 6th bit: 8 - 6 = 2
   `     o 1bit, 5th bit: 8 - 5 = 3
   `     o 1bit, 4th bit: 8 - 4 = 4
   `     o 1bit, 3rd bit: 8 - 3 = 5
   `     o 1bit, 2nd bit: 8 - 2 = 6
   `     o 1bit, 1st bit: 8 - 1 = 7
   `     o 1bit, 0th bit: 8 - 0 = 8

   `     o 2bit, 3rd bit: 8 - 6 = 2
   `     o 2bit, 2nd bit: 8 - 4 = 4
   `     o 2bit, 1st bit: 8 - 2 = 6
   `     o 2bit, 0th bit: 8 - 0 = 8
   `
   `     o 4bit, 1st bit: 8 - 4 = 4
   `     o 4bit, 0th bit: 8 - 0 = 8

   `     o 8bit, 0th bit: 8 - 0 = 8
   `   - (uchar) -1 >> lastKeepBit:
   `     o adds 11's until startBit (first target pixel
   `       bit)
   `     o 1bit, 7th bit: -1 << 1 = 1111 1110
   `     o 1bit, 6th bit: -1 << 2 = 1111 1100
   `     o 1bit, 5th bit: -1 << 3 = 1111 1000
   `     o 1bit, 4th bit: -1 << 4 = 1111 0000
   `     o 1bit, 3rd bit: -1 << 5 = 1110 0000
   `     o 1bit, 2nd bit: -1 << 6 = 1100 0000
   `     o 1bit, 1st bit: -1 << 7 = 1000 0000
   `     o 1bit, 0th bit: -1 << 8 = 0000 0000

   `     o 2bit, 3rd bit: -1 << 2 = 1111 1100
   `     o 2bit, 2nd bit: -1 << 4 = 1111 0000
   `     o 2bit, 1st bit: -1 << 6 = 1100 0000
   `     o 2bit, 0th bit: -1 << 8 = 0000 0000

   `     o 4bit, 1st bit: -1 << 4 = 1111 0000
   `     o 4bit, 0th bit: -1 << 8 = 0000 0000

   `     o 8bit, 0th bit: -1 << 8 = 0000 0000
   */

   maskUC |= ( ((unsigned char) -1) << bitUC );
      /*Logic
      `   - endMask: (uchar) -1 << bitUC:
      `     o 2bit, 3rd bit: -1 << 2 = 0011 1111
      `     o 2bit, 2nd bit: -1 << 4  = 0000 1111
      `     o 2bit, 1st bit: -1 << 6  = 0000 0011
      `     o 2bit, 0th bit: -1 << 8  = 0000 0000
      `
      `     o bitUC is last bit in target pixel, by
      `       1 by this I get ones for every bit after
      `       bitUC, but zero for bitUC and before
      `   - maskUC | endMask:
      `     o sets the ending bits of maskUC to 1,
      `       this leaves only the target bits as 0
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - add color
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   byteUCPtr =
        pngSTPtr->pixelAryUC
      + (startSL >> pngSTPtr->shiftUC);
      /* startSL >> shiftUC
      `   - byte I need to shift two (if no extra bytes)
      ` (startSL >> shiftUC)
      `   - byte I need to change
      */


   *byteUCPtr &= maskUC;          /*clear target bits*/
   maskUC = (colUC << bitUC);     /*add color to mask*/
   *byteUCPtr |= maskUC;          /*add color*/

   return 0;
}  /*addBar_st_mkPng*/

/*-------------------------------------------------------\
| Fun12: addBar_st_mkPng
|   - adds a bar to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of left conner; start of bar; index 0
|   - ySL:
|     o y coordiante of bottom of bar (index 0)
|   - widthUS:
|     o width in pixels of bar
|   - heightUS:
|     o heigth in pixels of bar
|   - colUC:
|     o index of color in pallete to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have bar
| Note:
|   o the minimum width is at least one byte worth,
|     otherwise, do single pixel modifications
|     with addPixel_st_mkPng
\-------------------------------------------------------*/
signed char
addBar_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed long widthUS,       /*pixels wide of bar*/
   signed long heightUS,      /*pixels high of bar*/
   signed char colUC          /*color of bar*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 addBar_st_mkPng
   '   - adds a bar to a st_mkPng image
   '   o fun12 sec01:
   '     - variable declarations
   '   o fun12 sec02:
   '     - setup + create color stamp
   '   o fun12 sec03:
   '     - make bar
   '   o fun12 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ul_mkPng maskStartUL = 0;/*mask to clear start*/
   ul_mkPng colStartUL = 0; /*color stamp for mask start*/

   ul_mkPng maskEndUL = 0;  /*ending mask to clear bits*/
   ul_mkPng colEndUL = 0;   /*color stamp for mask end*/

   ul_mkPng colByteUL = 0;  /*byte full of color*/
   ul_mkPng *pixAryUL = (ul_mkPng *) pngSTPtr->pixelAryUC;
     /*pixel array as long*/

   signed long startSL = 0;    /*start of bar on row*/
   signed long endSL = 0;      /*start of bar on row*/
   signed long cpIndexSL = 0;  /*index copying at*/
   signed long lenRowSL = 0;   /*length of one row*/

   unsigned char bitUC = 0;    /*for building mask*/
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - setup + create color stamp
   ^   o fun12 sec02 sub01:
   ^     - check if have overflow and find row length
   ^   o fun12 sec02 sub02:
   ^     - find start, end, and setup masks
   ^   o fun12 sec02 sub03:
   ^     - setup color stamps
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec02 Sub01:
   *   - check if have overflow and find row length
   \*****************************************************/

   startSL = (signed long) widthUS;
   startSL *= ySL;       /*gives pixels till row*/
   startSL += xSL;       /*gives frist pixel*/
   startSL += widthUS;   /*gives last pixel*/

   if( startSL > pngSTPtr->numPixelSL)
      goto overflow_fun12;


   /*this is safe because setup enforces that width must
   `   be a multiple of bits per byte
   */
   lenRowSL = pngSTPtr->widthUS >> pngSTPtr->shiftUC;
   lenRowSL >>= sizeof(ul_mkPng); /*bytes to longs*/

   /*****************************************************\
   * Fun12 Sec02 Sub02:
   *   - find start, end, and setup masks
   \*****************************************************/

   startSL = xSL;
   endSL = startSL + widthUS;

   bitUC =
        startSL
      & (pngSTPtr->pixPerByteUC * sizeof(ul_mkPng));
   bitUC <<= (pngSTPtr->shiftUC * sizeof(ul_mkPng));

   /*build mask*/
   maskStartUL =
         ((ul_mkPng) -1)
      >> ((sizeof(ul_mkPng) << 3) - bitUC);

   maskEndUL = ((ul_mkPng) -1) << bitUC;


   /*convert start and end to bytes*/
   startSL >>= (pngSTPtr->shiftUC * sizeof(ul_mkPng));
   endSL >>= (pngSTPtr->shiftUC * sizeof(ul_mkPng));


   /*move to first pixel to change position*/
   pixAryUL +=
         (ySL * pngSTPtr->widthUS)
      >> (pngSTPtr->pixPerByteUC * sizeof(ul_mkPng));
      /*moves to y coordinate (ingores extra bytes)*/

   /*****************************************************\
   * Fun12 Sec02 Sub03:
   *   - setup color stamps
   \*****************************************************/

   for(
      bitUC = 0;
      bitUC < (pngSTPtr->pixDepthUC  * sizeof(ul_mkPng));
      bitUC += pngSTPtr->pixDepthUC
   ) colByteUL |= ( ((ul_mkPng) colUC) << bitUC );

   colStartUL = ~maskStartUL;
   colStartUL &= colByteUL;

   colEndUL = ~maskEndUL;
   colEndUL &= colByteUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - make bar
   ^   o fun12 sec03 sub01:
   ^     - one limb coloring
   ^   o fun12 sec03 sub02:
   ^     - two limb coloring
   ^   o fun12 sec03 sub03:
   ^     - three or more limb coloring
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
	* Fun12 Sec03 Sub01:
   *   - one limb coloring
   \*****************************************************/

   if(startSL == endSL)
   { /*If: only coloring one limb*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/

   } /*If: only coloring one limb*/

   /*****************************************************\
	* Fun12 Sec03 Sub02:
   *   - two limb coloring
   \*****************************************************/

   else if((startSL + 1) == endSL)
   { /*Else If: only coloring two limbs*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL[endSL] &= maskEndUL;
         pixAryUL[endSL] |= colEndUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/

   } /*Else If: only coloring two limbs*/

   /*****************************************************\
   * Fun12 Sec03 Sub03:
   *   - three or more limb coloring
   \*****************************************************/

   else
   { /*Else If: coloring three or more limbs*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         cpIndexSL = startSL;
         while(cpIndexSL < endSL)
            pixAryUL[cpIndexSL++] = colByteUL;

         pixAryUL[endSL] &= maskEndUL;
         pixAryUL[endSL] |= colEndUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/

   } /*Else If: coloring three or more  limbs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflow_fun12:;
      return def_overflow_mkPng;
}  /*addBar_st_mkPng*/

/*-------------------------------------------------------\
| Fun16: addUint_mkPng
|   - adds a uint to a png buffer
| Input:
|   - inUI:
|     o unsigned int to add
|   - outFILE:
|     o FILE pointer to print unsigned int to
| Output:
|   - Modifies:
|     o buffStr to have inUI (converted to bigendin)
|   - Returns:
|     o next open index in buffStr
\-------------------------------------------------------*/
void
addUint_mkPng(
   unsigned int inUI,
   void *outFILE
){
   unsigned char *tmpAryUC = 0;
   inUI = uiToBig_endin(inUI);
   tmpAryUC = (unsigned char *) &inUI;

   fputc(tmpAryUC[0], (FILE *) outFILE);
   fputc(tmpAryUC[1], (FILE *) outFILE);
   fputc(tmpAryUC[2], (FILE *) outFILE);
   fputc(tmpAryUC[3], (FILE *) outFILE);
} /*addUint_mkPng*/

/*-------------------------------------------------------\
| Fun17: pIHDR_st_mkPng
|   - add the IHDR header to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add IHDR header to
|   - outFILE:
|     o FILE pointer to print IHDR block to
| Output:
|   - Prints:
|     o IHDR to outFILE
\-------------------------------------------------------*/
void
pIHDR_st_mkPng(
   struct st_mkPng *pngSTPtr,  /*to add header to*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - add the IHDR header to a st_mkPng struct
   '   o fun17 sec01:
   '    - variable declarations
   '   o fun17 sec02:
   '    - add length of IHDR chunk
   '   o fun17 sec03:
   '    - add name of IHDR chunk
   '   o fun17 sec04:
   '    - add image dimesions to IHDR header
   '   o fun17 sec05:
   '    - add color pallete and other values (leave 0)
   '   o fun17 sec06:
   '    - add crc32 to IHDR chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char *tmpAryUC = 0;
   unsigned int tmpUI = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - add length of IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   addUint_mkPng( 13, outFILE);
      /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - add name of IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add IHDR header*/
   crc32UI = crc32Byte_checkSum('I', crc32UI);
   crc32UI = crc32Byte_checkSum('H', crc32UI);
   crc32UI = crc32Byte_checkSum('D', crc32UI);
   crc32UI = crc32Byte_checkSum('R', crc32UI);

   fputc('I', (FILE *) outFILE);
   fputc('H', (FILE *) outFILE);
   fputc('D', (FILE *) outFILE);
   fputc('R', (FILE *) outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - add image dimesions to IHDR header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add image width to header*/
      tmpUI = uiToBig_endin(pngSTPtr->widthUS);
      tmpAryUC = (unsigned char *) &tmpUI;

      crc32UI = crc32Byte_checkSum(tmpAryUC[0], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[1], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[2], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[3], crc32UI);

      fputc(tmpAryUC[0], (FILE *) outFILE);
      fputc(tmpAryUC[1], (FILE *) outFILE);
      fputc(tmpAryUC[2], (FILE *) outFILE);
      fputc(tmpAryUC[3], (FILE *) outFILE);


   /*add image height to header*/
      tmpUI = uiToBig_endin(pngSTPtr->heightUS);
      tmpAryUC = (unsigned char *) &tmpUI;

      crc32UI = crc32Byte_checkSum(tmpAryUC[0], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[1], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[2], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[3], crc32UI);

      fputc(tmpAryUC[0], (FILE *) outFILE);
      fputc(tmpAryUC[1], (FILE *) outFILE);
      fputc(tmpAryUC[2], (FILE *) outFILE);
      fputc(tmpAryUC[3], (FILE *) outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec05:
   ^   - add color pallete and other values (leave 0)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add bit depth to header*/
     crc32UI =
        crc32Byte_checkSum(pngSTPtr->pixDepthUC, crc32UI);
     fputc(pngSTPtr->pixDepthUC, (FILE *) outFILE);

   /*specifiy using index color pallete*/
     crc32UI = crc32Byte_checkSum(3, crc32UI);
     fputc(3, (FILE *) outFILE);

   /*compression type; 0 is only option*/
     crc32UI = crc32Byte_checkSum(0, crc32UI);
     fputc(0, (FILE *) outFILE);

   /*filter type; 0 is only option*/
     crc32UI = crc32Byte_checkSum(0, crc32UI);
     fputc(0, (FILE *) outFILE);

   /*Interlacing; 0 = no interlace; 1 is adam 7*/
     crc32UI = crc32Byte_checkSum(0, crc32UI);
     fputc(0, (FILE *) outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec06:
   ^   - add crc32 to IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to IHDR header (end of header)*/
   crc32UI = crc32Finish_checkSum(crc32UI);
   addUint_mkPng(crc32UI, outFILE);
} /*pIHDR_mkPng*/

/*-------------------------------------------------------\
| Fun18: pPLTE_st_mkPng
|   - add the pallete (PLTE) header to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add PLTE header to
|   - outFILE:
|     o FILE pointer to print pallete header/chunk to
| Output:
|   - Modifies:
|     o palleteStr in pngSTPtr to have pallete png header
\-------------------------------------------------------*/
void
pPLTE_st_mkPng(
   struct st_mkPng *pngSTPtr, /*to add header to*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - add the pallete (PLTE) header to st_mkPng struct
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec02:
   '     - add length for PLTE chunk
   '   o fun18 sec03:
   '     - add PLTE chunk header
   '   o fun18 sec04:
   '     - add color pallete for PLTE chunk
   '   o fun18 sec05:
   '     - add crc32 to PLTE chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int uiCol = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - add length for PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add color pallete size to header*/
   uiCol = 4 + (pngSTPtr->numColUC * 3);
      /*4 for header, * 3 for color pallete*/

   addUint_mkPng(uiCol, outFILE);
      /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - add PLTE chunk header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add PLTE header*/
      crc32UI = crc32Byte_checkSum('P', crc32UI);
      crc32UI = crc32Byte_checkSum('L', crc32UI);
      crc32UI = crc32Byte_checkSum('T', crc32UI);
      crc32UI = crc32Byte_checkSum('E', crc32UI);

      fputc('P', (FILE *) outFILE);
      fputc('L', (FILE *) outFILE);
      fputc('T', (FILE *) outFILE);
      fputc('E', (FILE *) outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - add color pallete for PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add color pallete*/
   uiCol = 0;
   while(uiCol < pngSTPtr->numColUC)
   { /*Loop: add color pallete to header*/
      crc32UI =
         crc32Byte_checkSum(
            pngSTPtr->redAryUC[uiCol],
            crc32UI
         );
      fputc(pngSTPtr->redAryUC[uiCol], (FILE *) outFILE);

      crc32UI =
         crc32Byte_checkSum(
            pngSTPtr->bluAryUC[uiCol],
            crc32UI
         );
      fputc(pngSTPtr->bluAryUC[uiCol], (FILE *) outFILE);

      crc32UI =
         crc32Byte_checkSum(
            pngSTPtr->greAryUC[uiCol],
            crc32UI
         );
      fputc(pngSTPtr->greAryUC[uiCol], (FILE *) outFILE);
      ++uiCol;
   } /*Loop: add color pallete to header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - add crc32 to PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to PTLE header (end of header)*/
   crc32UI = crc32Finish_checkSum(crc32UI);

   addUint_mkPng(crc32UI, (FILE *) outFILE);
} /*pPLTE_st_mkPng*/

/*-------------------------------------------------------\
| Fun19: pIDAT_st_mkPng
|   - add image IDAT header
|   - not fully sure on this one
|   - copied from misc0110's libattpng repository
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add IDAT header to
|     o also has image to add
| Output:
|   - Modifies:
|     o idatStr in pngSTPtr to have IDAT header
\-------------------------------------------------------*/
void
pIDAT_st_mkPng(
   struct st_mkPng *pngSTPtr,/*add header to + has image*/
   void *outFILE             /*file to print idat to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - add image IDAT header
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - write IDAT header
   '   o fun19 sec03:
   '     - add image to image header
   '   o fun19 sec04:
   '     - add adler and crc32 to image header
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char *tmpAryUC = 0;
   unsigned short tmpUS = 0;
   unsigned int indexUI = 0;

   signed int sumOneSI = 1;
   signed int sumTwoSI = 0;

   signed long pixelSL = 0;
   signed long endSL = 0;

   unsigned int crc32UI = 0xffffffff;
   unsigned short pixPerRowUS =
      1 + (unsigned short) pngSTPtr->widthUS;

   unsigned short sizeUS =
       pngSTPtr->heightUS / pngSTPtr->pixPerByteUC;
      /*think recording number of bytes per column*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec02:
   ^   - write IDAT header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*size of png to print out ot header*/
   tmpUS = 5 + pngSTPtr->pixPerByteUC; /*(5 + bpl)*/
   tmpUS *= pngSTPtr->heightUS; /*(5 + bpl) * height)*/
   tmpUS += 6;           /*2 + heigth * (5 + bpl) + 4)*/

   addUint_mkPng(tmpUS, outFILE);
      /*size of png, comes before idat header*/

   crc32Byte_checkSum('I', crc32UI);
   crc32Byte_checkSum('D', crc32UI);
   crc32Byte_checkSum('A', crc32UI);
   crc32Byte_checkSum('T', crc32UI);
   crc32Byte_checkSum(0x78, crc32UI);
   crc32Byte_checkSum(0x01, crc32UI);
   
   fputc('I', (FILE *) outFILE);
   fputc('D', (FILE *) outFILE);
   fputc('A', (FILE *) outFILE);
   fputc('T', (FILE *) outFILE);
   fputc(0x78, (FILE *) outFILE);
   fputc(0x01, (FILE *) outFILE);

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec03:
   ^   - add image to image header
   ^   o fun19 sec03 sub01:
   ^     - start loop & deal with end of rows
   ^   o fun19 sec03 sub02:
   ^     - add pixe to row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun19 Sec03 Sub01:
   *   - start loop & deal with end of rows
   *   o fun19 sec03 sub01 cat01:
   *     - start loop & deal with non-last row end of row
   *   o fun19 sec03 sub01 cat02:
   *     - deal with last row (end of file row)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub01 Cat01:
   +   - start loop & deal with non-last row end of row
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   endSL = pngSTPtr->numPixelSL >> pngSTPtr->shiftUC;
      /*number bytes in png image*/

   indexUI = 0;
   pixelSL = 0;

   while(pixelSL < endSL)
   { /*Loop: add pixels to crc32*/
      
      if(! indexUI)
      { /*If: line header*/
         if(pixelSL > (signed long) pixPerRowUS)
         { /*If: not last row*/
            fputc('\0', (FILE *) outFILE);
            crc32UI = crc32Byte_checkSum('\0', crc32UI);

            /*bytes per pixel (index so only 1) */
            tmpUS = 1; /*pixel is in entire byte*/
            tmpAryUC = (unsigned char *) &tmpUS;
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[0], crc32UI);
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[1], crc32UI);
            fputc(tmpAryUC[0], (FILE *) outFILE);
            fputc(tmpAryUC[1], (FILE *) outFILE);


            tmpUS = ~1; /*pixel is in entire byte*/
            tmpAryUC = (unsigned char *) &tmpUS;
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[0], crc32UI);
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[1], crc32UI);
            fputc(tmpAryUC[0], (FILE *) outFILE);
            fputc(tmpAryUC[1], (FILE *) outFILE);

            /*no filter marker*/
            crc32UI = crc32Byte_checkSum(0, crc32UI);
            fputc(0, (FILE *) outFILE);
                /*row header; no pixel*/

            /*add no-fitler to adler*/
            adler32Byte_checkSum(0, &sumOneSI, &sumTwoSI);

            --sizeUS;
         } /*If: not last row*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec03 Sub01 Cat02:
         +   - deal with last row (end of file row)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: is last row*/
            fputc('\1', (FILE *) outFILE);
            crc32UI = crc32Byte_checkSum('\1', crc32UI);

            /*write offset (number bytes in last row)*/
            tmpUS = sizeUS; /*pixel is in entire byte*/
            tmpAryUC = (unsigned char *) &tmpUS;
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[0], crc32UI);
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[1], crc32UI);
            fputc(tmpAryUC[0], (FILE *) outFILE);
            fputc(tmpAryUC[1], (FILE *) outFILE);


            tmpUS = ~sizeUS; /*pixel is in entire byte*/
            tmpAryUC = (unsigned char *) &tmpUS;
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[0], crc32UI);
            crc32UI =
               crc32Byte_checkSum(tmpAryUC[1], crc32UI);
            fputc(tmpAryUC[0], (FILE *) outFILE);
            fputc(tmpAryUC[1], (FILE *) outFILE);


            crc32UI = crc32Byte_checkSum(0, crc32UI);
            fputc(0, (FILE *) outFILE);
                /*row header; no pixel*/

            tmpUS = sizeUS; /*pixel is in entire byte*/
            tmpAryUC = (unsigned char *) &tmpUS;
            adler32Byte_checkSum(
               tmpAryUC[0],
               &sumOneSI,
               &sumTwoSI
            );
            adler32Byte_checkSum(
               tmpAryUC[0],
               &sumOneSI,
               &sumTwoSI
            );

            --sizeUS;
         } /*Else: is last row*/
      } /*If: line header*/

      /**************************************************\
      * Fun19 Sec03 Sub02:
      *   - add pixe to row
      \**************************************************/

      fputc(
         pngSTPtr->pixelAryUC[pixelSL],
         (FILE *) outFILE
      );

      crc32UI =
          crc32Byte_checkSum(
             pngSTPtr->pixelAryUC[pixelSL],
             crc32UI
          );
      adler32Byte_checkSum(
         pngSTPtr->pixelAryUC[pixelSL],
         &sumOneSI,
         &sumTwoSI
      );

      /*move to next pixel*/
      --sizeUS;
      indexUI = (indexUI + 1) % pngSTPtr->widthUS;

      ++pixelSL;
   }  /*Loop: add pixels to crc32*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec04:
   ^   - add adler and crc32 to image header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add adler sum to crc32*/
      indexUI = adler32Finish_checkSum(sumOneSI,sumTwoSI);
      tmpAryUC = (unsigned char *) &indexUI;

      crc32UI = crc32Byte_checkSum(tmpAryUC[0], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[1], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[2], crc32UI);
      crc32UI = crc32Byte_checkSum(tmpAryUC[3], crc32UI);
      
      fputc(tmpAryUC[0], (FILE *) outFILE);
      fputc(tmpAryUC[1], (FILE *) outFILE);
      fputc(tmpAryUC[2], (FILE *) outFILE);
      fputc(tmpAryUC[3], (FILE *) outFILE);

   /*add crc to IDAT header (end of header)*/
      crc32UI = crc32Finish_checkSum(crc32UI);
      crc32UI = uiToBig_endin(crc32UI);
      tmpAryUC = (unsigned char *) &crc32UI;

      fputc(tmpAryUC[0], (FILE *) outFILE);
      fputc(tmpAryUC[1], (FILE *) outFILE);
      fputc(tmpAryUC[2], (FILE *) outFILE);
      fputc(tmpAryUC[3], (FILE *) outFILE);
} /*pIDAT_st_mkPng*/

/*-------------------------------------------------------\
| Fun20: pIEND_st_mkPng
|   - add end header (IEND) for png 
| Input:
|   - outFILE:
|     o FILE pointer to print IEND header to
| Output:
|   - Prints:
|     o IEND header to outFILE
\-------------------------------------------------------*/
void
pIEND_st_mkPng(
   void *outFILE
){
   unsigned int crc32UI = 0xffffffff;

   addUint_mkPng(0, (FILE *) outFILE);
     /*IEND is always length 0*/

   crc32UI = crc32Byte_checkSum('I', crc32UI);
   crc32UI = crc32Byte_checkSum('E', crc32UI);
   crc32UI = crc32Byte_checkSum('N', crc32UI);
   crc32UI = crc32Byte_checkSum('D', crc32UI);

   fputc('I', (FILE *) outFILE);
   fputc('E', (FILE *) outFILE);
   fputc('N', (FILE *) outFILE);
   fputc('D', (FILE *) outFILE);

   /*IEND has no data*/

   /*add crc to PTLE header (end of header)*/
   crc32UI = crc32Finish_checkSum(crc32UI);
   addUint_mkPng(crc32UI, (FILE *) outFILE);
} /*pIEND_st_mkPng*/

/*-------------------------------------------------------\
| Fun21: print_st_mkPng
|   - prints a png to output file
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to write to outFILE
|   - outFILE:
|     o pionter to FILE struct to write png to (binary
|       file ("wb"))
| Output:
|   - Prints:
|     o png header and contents of pngSTPtr to outFILE
\-------------------------------------------------------*/
void
print_st_mkPng(
   struct st_mkPng *pngSTPtr,  /*png to print*/
   void *outFILE               /*file to print to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun21 TOC:
   '   - prints a png to output file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*header for png file*/
   fputc(137, (FILE *) outFILE);
   fputc('P', (FILE *) outFILE);
   fputc('N', (FILE *) outFILE);
   fputc('G', (FILE *) outFILE);
   fputc('\r', (FILE *) outFILE);
   fputc('\n', (FILE *) outFILE);
   fputc(26, (FILE *) outFILE);
   fputc('\n', (FILE *) outFILE);


   pIHDR_st_mkPng(pngSTPtr, outFILE);
   pPLTE_st_mkPng(pngSTPtr, outFILE);
   pIDAT_st_mkPng(pngSTPtr, outFILE);
   pIEND_st_mkPng(outFILE);

   /*not including transparency*/
} /*print_st_mkPng*/
