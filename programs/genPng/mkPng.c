/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mkPng SOF:
'   - creates the structure to hold a png (st_mkPng) and
'     also prints the png to a file
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
'   o fun09: setup_st_mkPng
'     - allocates memory for a st_mkPng struct
'   o fun10: mk_st_mkPng
'     - makes a st_mkPng struct on heap
'   o .c fun11: addUint_mkPng
'     - adds a uint to a png buffer
'   o .c fun12: pIHDR_st_mkPng
'     - add the IHDR header to a st_mkPng struct
'   o .c fun13: pPLTE_st_mkPng
'     - add pallete (PLTE) header to a st_mkPng struct
'   o .c fun14: pIDAT_st_mkPng
'     - add image IDAT header
'     - copied from misc0110's libattpng repository
'   o .c fun15: pIEND_st_mkPng
'     - add end header (IEND) for png 
'   o fun16: print_st_mkPng
'     - prints a png to output file
'   o license:
'     - licensing for this code (CC0)
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

/*.h files only*/
#include "../genLib/64bit.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
   ul_64bit *pixAryUL = 0;

   if(! pngSTPtr)
      return;

   if(pngSTPtr->pixelAryUC)
   { /*If: need to blank pixel matrix*/

      /*not as fast as SIMD, but faster then byte by byte
      `  and is always present
      */
      pixAryUL = (ul_64bit *) pngSTPtr->pixelAryUC;

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

      /*bit8_fun02_DELETE:;*/ /*DELETE*/
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
|   - errSCPtr
|     o signed char pointer to get errors
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
      ++pngSTPtr->numColUC;

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

   pngSTPtr->numColUC = 0;
   pngSTPtr->pixDepthUC = 0;

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

   /*DELETE*/
   /*for(errSC = 4; errSC < 16; ++errSC)
   {
      pngSTPtr->redAryUC[errSC] = 0;
      pngSTPtr->bluAryUC[errSC] = 0;
      pngSTPtr->greAryUC[errSC] = 0;
      ++pngSTPtr->numColUC;
   }
   errSC = 0;*/ /*DELETE*/
     

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
|     o use 255 to force 8bit
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

   /*make sure every row ends in a byte*/
   /*pngSTPtr->widthUS +=
      pngSTPtr->widthUS % def_bytesInUL_64bit;
   */ /*I orginally wanted to use longs, but no longer*/

   pngSTPtr->numPixelSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;

   /*find number of pixels in png*/
   pngSTPtr->numPixelSL = pngSTPtr->widthUS;
   pngSTPtr->numPixelSL *= pngSTPtr->heightUS;


   /*find number of bytes needed for png size*/
   numBytesSL = pngSTPtr->numPixelSL;
   numBytesSL += def_idatEnd_mkPng;
     /*extra bytes at end of idat entry*/

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
|     o use 255 to force 8bit
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
| Fun11: addUint_mkPng
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
| Fun12: pIHDR_st_mkPng
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
   ' Fun12 TOC:
   '   - add the IHDR header to a st_mkPng struct
   '   o fun12 sec01:
   '    - variable declarations
   '   o fun12 sec02:
   '    - add length of IHDR chunk
   '   o fun12 sec03:
   '    - add name of IHDR chunk
   '   o fun12 sec04:
   '    - add image dimesions to IHDR header
   '   o fun12 sec05:
   '    - add color pallete and other values (leave 0)
   '   o fun12 sec06:
   '    - add crc32 to IHDR chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char *tmpAryUC = 0;
   unsigned int tmpUI = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - add length of IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   addUint_mkPng( 13, outFILE);
      /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
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
   ^ Fun12 Sec04:
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
   ^ Fun12 Sec05:
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
   ^ Fun12 Sec06:
   ^   - add crc32 to IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to IHDR header (end of header)*/
   crc32UI = crc32Finish_checkSum(crc32UI);
   addUint_mkPng(crc32UI, outFILE);
} /*pIHDR_mkPng*/

/*-------------------------------------------------------\
| Fun13: pPLTE_st_mkPng
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
   ' Fun13 TOC:
   '   - add the pallete (PLTE) header to st_mkPng struct
   '   o fun13 sec01:
   '     - variable declarations
   '   o fun13 sec02:
   '     - add length for PLTE chunk
   '   o fun13 sec03:
   '     - add PLTE chunk header
   '   o fun13 sec04:
   '     - add color pallete for PLTE chunk
   '   o fun13 sec05:
   '     - add crc32 to PLTE chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int uiCol = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - add length for PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add color pallete size to header*/
   uiCol = (pngSTPtr->numColUC * 3);

   addUint_mkPng(uiCol, outFILE);
      /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
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
   ^ Fun13 Sec04:
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
   ^ Fun13 Sec05:
   ^   - add crc32 to PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to PTLE header (end of header)*/
   crc32UI = crc32Finish_checkSum(crc32UI);

   addUint_mkPng(crc32UI, (FILE *) outFILE);
} /*pPLTE_st_mkPng*/

/*-------------------------------------------------------\
| Fun14: pIDAT_st_mkPng
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
   ' Fun14 TOC:
   '   - add image IDAT header
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - write IDAT header
   '   o fun14 sec03:
   '     - add image to image header
   '   o fun14 sec04:
   '     - add adler and crc32 to image header
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun14 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char *tmpAryUC = 0;
   unsigned short tmpUS = 0;
   unsigned int indexUI = 0;
   unsigned short rowUS = 0;

   unsigned char bitUC = 0;
   unsigned char outUC = 0;
   unsigned char tmpUC = 0;

   signed int sumOneSI = 1;
   signed int sumTwoSI = 0;

   signed long pixelSL = 0;
   signed long totalPixelSL = 0;
   unsigned short endRowUS =
      pngSTPtr->widthUS / pngSTPtr->pixPerByteUC;

   unsigned int crc32UI = 0xffffffff;

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun14 Sec02:
   ^   - write IDAT header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


   /*find the total pixels + extra bytes in the image*/
   totalPixelSL =
        (pngSTPtr->heightUS * endRowUS)
      + (pngSTPtr->heightUS * def_lenZlibHeader_mkPng)
      + def_lenZlibHeader_mkPng;

   addUint_mkPng(totalPixelSL, outFILE);
      /*size of png, before idat header (not in crc)
      `   This has the number of bytes in the idat
      `   entry. This includes the 6 bytes at the end
      `   of each row
      */

   crc32UI = crc32Byte_checkSum('I', crc32UI);
   crc32UI = crc32Byte_checkSum('D', crc32UI);
   crc32UI = crc32Byte_checkSum('A', crc32UI);
   crc32UI = crc32Byte_checkSum('T', crc32UI);
   crc32UI = crc32Byte_checkSum(0x78, crc32UI);
   crc32UI = crc32Byte_checkSum(0xda, crc32UI);
   
   fputc('I', (FILE *) outFILE);
   fputc('D', (FILE *) outFILE);
   fputc('A', (FILE *) outFILE);
   fputc('T', (FILE *) outFILE);
   fputc(0x78, (FILE *) outFILE);
   fputc(0xda, (FILE *) outFILE);

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun14 Sec03:
   ^   - add image to image header
   ^   o fun14 sec03 sub01:
   ^     - start loop & deal with end of rows
   ^   o fun14 sec03 sub02:
   ^     - add pixe to row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun14 Sec03 Sub01:
   *   - start loop & deal with end of rows
   *   o fun14 sec03 sub01 cat01:
   *     - start loop & deal with non-last row end of row
   *   o fun14 sec03 sub01 cat02:
   *     - deal with last row (end of file row)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun14 Sec03 Sub01 Cat01:
   +   - start loop & deal with non-last row end of row
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   indexUI = 0;
   pixelSL = 0;
   goto addZlibHeader_fun14_sec03_sub01_cat01;

   /*height is index 1*/
   while(rowUS < pngSTPtr->heightUS)
   { /*Loop: add pixels to crc32*/

      if(! (pixelSL % pngSTPtr->widthUS) )
      { /*If: end of the row*/
         ++rowUS;

         addZlibHeader_fun14_sec03_sub01_cat01:;
         if(rowUS == pngSTPtr->heightUS - 1)
         { /*If: on the last row*/
            fputc('\1', (FILE *) outFILE);
            crc32UI = crc32Byte_checkSum('\1', crc32UI);
         } /*If: on the last row*/

         else if(rowUS < pngSTPtr->heightUS)
         { /*Else: not last row*/
            fputc('\0', (FILE *) outFILE);
            crc32UI = crc32Byte_checkSum('\0', crc32UI);
         } /*Else: not last row*/

         else
            break;

         /*bits in one line*/
         tmpUS = usToLittle_endin(endRowUS + 1);
         tmpAryUC = (unsigned char *) &tmpUS;
         crc32UI =
            crc32Byte_checkSum(tmpAryUC[0], crc32UI);
         crc32UI =
            crc32Byte_checkSum(tmpAryUC[1], crc32UI);
         fputc(tmpAryUC[0], (FILE *) outFILE);
         fputc(tmpAryUC[1], (FILE *) outFILE);


         tmpUS = ~tmpUS;
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
             /*row header for uncompressed*/

         /*add no-fitler to adler*/
         adler32Byte_checkSum(0, &sumOneSI, &sumTwoSI);
      } /*If: end of the row*/

      /**************************************************\
      * Fun14 Sec03 Sub02:
      *   - add pixel to row
      \**************************************************/

      outUC = 0;
      tmpUC = 0;

      for(
         bitUC = 0;
         bitUC < def_bitsPerChar_64bit;
         bitUC += pngSTPtr->pixDepthUC
      ){ /*Loop: print the pixels to the idat entry*/
         outUC |=
            (
               pngSTPtr->pixelAryUC[
                  pixelSL +
                   pngSTPtr->pixPerByteUC - tmpUC - 1
               ] << bitUC
            );
         ++tmpUC;
      }  /*Loop: print the pixels to the idat entry*/
      pixelSL += tmpUC;

      fputc(outUC, (FILE *) outFILE);
      crc32UI = crc32Byte_checkSum(outUC, crc32UI);
      adler32Byte_checkSum(outUC, &sumOneSI, &sumTwoSI);
   }  /*Loop: add pixels to crc32*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun14 Sec04:
   ^   - add adler and crc32 to image header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add adler sum to crc32*/
      indexUI = adler32Finish_checkSum(sumOneSI,sumTwoSI);
      indexUI = uiToBig_endin(indexUI);
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
| Fun15: pIEND_st_mkPng
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
| Fun16: print_st_mkPng
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
   ' Fun16 TOC:
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
