#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "fontST.h"

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Hidden libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checksum.h"
!   - .c  #include "mkPng.h"
!   - .h  #include "../genLib/64bit.h"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*___________________________________________
` Fun00: addAscii32_smallFont
`  - adds character ` ` (ascii 32) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii32_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[0])
      free(fontSTPtr->pixAryUC[0]);

   fontSTPtr->pixAryUC[0] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[0])
      return 1;

   fontSTPtr->widthArySS[0] = 12;
   fontSTPtr->lenArySS[0] = 18;

   fontSTPtr->pixAryUC[0][0] = 0;
   fontSTPtr->pixAryUC[0][1] = 0;
   fontSTPtr->pixAryUC[0][2] = 0;
   fontSTPtr->pixAryUC[0][3] = 0;
   fontSTPtr->pixAryUC[0][4] = 0;
   fontSTPtr->pixAryUC[0][5] = 0;
   fontSTPtr->pixAryUC[0][6] = 0;
   fontSTPtr->pixAryUC[0][7] = 0;
   fontSTPtr->pixAryUC[0][8] = 0;
   fontSTPtr->pixAryUC[0][9] = 0;
   fontSTPtr->pixAryUC[0][10] = 0;
   fontSTPtr->pixAryUC[0][11] = 0;
   fontSTPtr->pixAryUC[0][12] = 0;
   fontSTPtr->pixAryUC[0][13] = 0;
   fontSTPtr->pixAryUC[0][14] = 0;
   fontSTPtr->pixAryUC[0][15] = 0;
   fontSTPtr->pixAryUC[0][16] = 0;
   fontSTPtr->pixAryUC[0][17] = 0;

   return 0;
} /*addAscii32_smallFont*/

/*___________________________________________
` Fun01: addAscii33_smallFont
`  - adds character `!` (ascii 33) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii33_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[1])
      free(fontSTPtr->pixAryUC[1]);

   fontSTPtr->pixAryUC[1] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[1])
      return 1;

   fontSTPtr->widthArySS[1] = 12;
   fontSTPtr->lenArySS[1] = 18;

   fontSTPtr->pixAryUC[1][0] = 0;
   fontSTPtr->pixAryUC[1][1] = 0;
   fontSTPtr->pixAryUC[1][2] = 7;
   fontSTPtr->pixAryUC[1][3] = 248;
   fontSTPtr->pixAryUC[1][4] = 128;
   fontSTPtr->pixAryUC[1][5] = 15;
   fontSTPtr->pixAryUC[1][6] = 112;
   fontSTPtr->pixAryUC[1][7] = 0;
   fontSTPtr->pixAryUC[1][8] = 2;
   fontSTPtr->pixAryUC[1][9] = 0;
   fontSTPtr->pixAryUC[1][10] = 0;
   fontSTPtr->pixAryUC[1][11] = 7;
   fontSTPtr->pixAryUC[1][12] = 112;
   fontSTPtr->pixAryUC[1][13] = 0;
   fontSTPtr->pixAryUC[1][14] = 0;
   fontSTPtr->pixAryUC[1][15] = 0;
   fontSTPtr->pixAryUC[1][16] = 0;
   fontSTPtr->pixAryUC[1][17] = 0;

   return 0;
} /*addAscii33_smallFont*/

/*___________________________________________
` Fun02: addAscii34_smallFont
`  - adds character `"` (ascii 34) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii34_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[2])
      free(fontSTPtr->pixAryUC[2]);

   fontSTPtr->pixAryUC[2] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[2])
      return 1;

   fontSTPtr->widthArySS[2] = 12;
   fontSTPtr->lenArySS[2] = 18;

   fontSTPtr->pixAryUC[2][0] = 0;
   fontSTPtr->pixAryUC[2][1] = 224;
   fontSTPtr->pixAryUC[2][2] = 121;
   fontSTPtr->pixAryUC[2][3] = 158;
   fontSTPtr->pixAryUC[2][4] = 135;
   fontSTPtr->pixAryUC[2][5] = 97;
   fontSTPtr->pixAryUC[2][6] = 24;
   fontSTPtr->pixAryUC[2][7] = 230;
   fontSTPtr->pixAryUC[2][8] = 56;
   fontSTPtr->pixAryUC[2][9] = 0;
   fontSTPtr->pixAryUC[2][10] = 0;
   fontSTPtr->pixAryUC[2][11] = 0;
   fontSTPtr->pixAryUC[2][12] = 0;
   fontSTPtr->pixAryUC[2][13] = 0;
   fontSTPtr->pixAryUC[2][14] = 0;
   fontSTPtr->pixAryUC[2][15] = 0;
   fontSTPtr->pixAryUC[2][16] = 0;
   fontSTPtr->pixAryUC[2][17] = 0;

   return 0;
} /*addAscii34_smallFont*/

/*___________________________________________
` Fun03: addAscii35_smallFont
`  - adds character `#` (ascii 35) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii35_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[3])
      free(fontSTPtr->pixAryUC[3]);

   fontSTPtr->pixAryUC[3] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[3])
      return 1;

   fontSTPtr->widthArySS[3] = 12;
   fontSTPtr->lenArySS[3] = 18;

   fontSTPtr->pixAryUC[3][0] = 0;
   fontSTPtr->pixAryUC[3][1] = 0;
   fontSTPtr->pixAryUC[3][2] = 0;
   fontSTPtr->pixAryUC[3][3] = 152;
   fontSTPtr->pixAryUC[3][4] = 129;
   fontSTPtr->pixAryUC[3][5] = 25;
   fontSTPtr->pixAryUC[3][6] = 254;
   fontSTPtr->pixAryUC[3][7] = 135;
   fontSTPtr->pixAryUC[3][8] = 25;
   fontSTPtr->pixAryUC[3][9] = 254;
   fontSTPtr->pixAryUC[3][10] = 135;
   fontSTPtr->pixAryUC[3][11] = 25;
   fontSTPtr->pixAryUC[3][12] = 152;
   fontSTPtr->pixAryUC[3][13] = 1;
   fontSTPtr->pixAryUC[3][14] = 0;
   fontSTPtr->pixAryUC[3][15] = 0;
   fontSTPtr->pixAryUC[3][16] = 0;
   fontSTPtr->pixAryUC[3][17] = 0;

   return 0;
} /*addAscii35_smallFont*/

/*___________________________________________
` Fun04: addAscii36_smallFont
`  - adds character `$` (ascii 36) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii36_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[4])
      free(fontSTPtr->pixAryUC[4]);

   fontSTPtr->pixAryUC[4] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[4])
      return 1;

   fontSTPtr->widthArySS[4] = 12;
   fontSTPtr->lenArySS[4] = 18;

   fontSTPtr->pixAryUC[4][0] = 0;
   fontSTPtr->pixAryUC[4][1] = 0;
   fontSTPtr->pixAryUC[4][2] = 18;
   fontSTPtr->pixAryUC[4][3] = 248;
   fontSTPtr->pixAryUC[4][4] = 195;
   fontSTPtr->pixAryUC[4][5] = 127;
   fontSTPtr->pixAryUC[4][6] = 44;
   fontSTPtr->pixAryUC[4][7] = 193;
   fontSTPtr->pixAryUC[4][8] = 31;
   fontSTPtr->pixAryUC[4][9] = 248;
   fontSTPtr->pixAryUC[4][10] = 3;
   fontSTPtr->pixAryUC[4][11] = 50;
   fontSTPtr->pixAryUC[4][12] = 252;
   fontSTPtr->pixAryUC[4][13] = 195;
   fontSTPtr->pixAryUC[4][14] = 31;
   fontSTPtr->pixAryUC[4][15] = 32;
   fontSTPtr->pixAryUC[4][16] = 1;
   fontSTPtr->pixAryUC[4][17] = 0;

   return 0;
} /*addAscii36_smallFont*/

/*___________________________________________
` Fun05: addAscii37_smallFont
`  - adds character `%` (ascii 37) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii37_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[5])
      free(fontSTPtr->pixAryUC[5]);

   fontSTPtr->pixAryUC[5] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[5])
      return 1;

   fontSTPtr->widthArySS[5] = 12;
   fontSTPtr->lenArySS[5] = 18;

   fontSTPtr->pixAryUC[5][0] = 0;
   fontSTPtr->pixAryUC[5][1] = 192;
   fontSTPtr->pixAryUC[5][2] = 49;
   fontSTPtr->pixAryUC[5][3] = 162;
   fontSTPtr->pixAryUC[5][4] = 193;
   fontSTPtr->pixAryUC[5][5] = 13;
   fontSTPtr->pixAryUC[5][6] = 96;
   fontSTPtr->pixAryUC[5][7] = 0;
   fontSTPtr->pixAryUC[5][8] = 3;
   fontSTPtr->pixAryUC[5][9] = 216;
   fontSTPtr->pixAryUC[5][10] = 193;
   fontSTPtr->pixAryUC[5][11] = 34;
   fontSTPtr->pixAryUC[5][12] = 198;
   fontSTPtr->pixAryUC[5][13] = 1;
   fontSTPtr->pixAryUC[5][14] = 0;
   fontSTPtr->pixAryUC[5][15] = 0;
   fontSTPtr->pixAryUC[5][16] = 0;
   fontSTPtr->pixAryUC[5][17] = 0;

   return 0;
} /*addAscii37_smallFont*/

/*___________________________________________
` Fun06: addAscii38_smallFont
`  - adds character `&` (ascii 38) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii38_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[6])
      free(fontSTPtr->pixAryUC[6]);

   fontSTPtr->pixAryUC[6] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[6])
      return 1;

   fontSTPtr->widthArySS[6] = 12;
   fontSTPtr->lenArySS[6] = 18;

   fontSTPtr->pixAryUC[6][0] = 0;
   fontSTPtr->pixAryUC[6][1] = 0;
   fontSTPtr->pixAryUC[6][2] = 15;
   fontSTPtr->pixAryUC[6][3] = 136;
   fontSTPtr->pixAryUC[6][4] = 129;
   fontSTPtr->pixAryUC[6][5] = 12;
   fontSTPtr->pixAryUC[6][6] = 124;
   fontSTPtr->pixAryUC[6][7] = 108;
   fontSTPtr->pixAryUC[6][8] = 108;
   fontSTPtr->pixAryUC[6][9] = 198;
   fontSTPtr->pixAryUC[6][10] = 225;
   fontSTPtr->pixAryUC[6][11] = 119;
   fontSTPtr->pixAryUC[6][12] = 60;
   fontSTPtr->pixAryUC[6][13] = 3;
   fontSTPtr->pixAryUC[6][14] = 0;
   fontSTPtr->pixAryUC[6][15] = 0;
   fontSTPtr->pixAryUC[6][16] = 0;
   fontSTPtr->pixAryUC[6][17] = 0;

   return 0;
} /*addAscii38_smallFont*/

/*___________________________________________
` Fun07: addAscii39_smallFont
`  - adds character `'` (ascii 39) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii39_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[7])
      free(fontSTPtr->pixAryUC[7]);

   fontSTPtr->pixAryUC[7] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[7])
      return 1;

   fontSTPtr->widthArySS[7] = 12;
   fontSTPtr->lenArySS[7] = 18;

   fontSTPtr->pixAryUC[7][0] = 0;
   fontSTPtr->pixAryUC[7][1] = 128;
   fontSTPtr->pixAryUC[7][2] = 7;
   fontSTPtr->pixAryUC[7][3] = 120;
   fontSTPtr->pixAryUC[7][4] = 0;
   fontSTPtr->pixAryUC[7][5] = 6;
   fontSTPtr->pixAryUC[7][6] = 96;
   fontSTPtr->pixAryUC[7][7] = 128;
   fontSTPtr->pixAryUC[7][8] = 3;
   fontSTPtr->pixAryUC[7][9] = 0;
   fontSTPtr->pixAryUC[7][10] = 0;
   fontSTPtr->pixAryUC[7][11] = 0;
   fontSTPtr->pixAryUC[7][12] = 0;
   fontSTPtr->pixAryUC[7][13] = 0;
   fontSTPtr->pixAryUC[7][14] = 0;
   fontSTPtr->pixAryUC[7][15] = 0;
   fontSTPtr->pixAryUC[7][16] = 0;
   fontSTPtr->pixAryUC[7][17] = 0;

   return 0;
} /*addAscii39_smallFont*/

/*___________________________________________
` Fun08: addAscii40_smallFont
`  - adds character `(` (ascii 40) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii40_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[8])
      free(fontSTPtr->pixAryUC[8]);

   fontSTPtr->pixAryUC[8] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[8])
      return 1;

   fontSTPtr->widthArySS[8] = 12;
   fontSTPtr->lenArySS[8] = 18;

   fontSTPtr->pixAryUC[8][0] = 0;
   fontSTPtr->pixAryUC[8][1] = 0;
   fontSTPtr->pixAryUC[8][2] = 30;
   fontSTPtr->pixAryUC[8][3] = 240;
   fontSTPtr->pixAryUC[8][4] = 128;
   fontSTPtr->pixAryUC[8][5] = 3;
   fontSTPtr->pixAryUC[8][6] = 24;
   fontSTPtr->pixAryUC[8][7] = 128;
   fontSTPtr->pixAryUC[8][8] = 1;
   fontSTPtr->pixAryUC[8][9] = 56;
   fontSTPtr->pixAryUC[8][10] = 0;
   fontSTPtr->pixAryUC[8][11] = 15;
   fontSTPtr->pixAryUC[8][12] = 224;
   fontSTPtr->pixAryUC[8][13] = 1;
   fontSTPtr->pixAryUC[8][14] = 0;
   fontSTPtr->pixAryUC[8][15] = 0;
   fontSTPtr->pixAryUC[8][16] = 0;
   fontSTPtr->pixAryUC[8][17] = 0;

   return 0;
} /*addAscii40_smallFont*/

/*___________________________________________
` Fun09: addAscii41_smallFont
`  - adds character `)` (ascii 41) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii41_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[9])
      free(fontSTPtr->pixAryUC[9]);

   fontSTPtr->pixAryUC[9] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[9])
      return 1;

   fontSTPtr->widthArySS[9] = 12;
   fontSTPtr->lenArySS[9] = 18;

   fontSTPtr->pixAryUC[9][0] = 0;
   fontSTPtr->pixAryUC[9][1] = 192;
   fontSTPtr->pixAryUC[9][2] = 3;
   fontSTPtr->pixAryUC[9][3] = 120;
   fontSTPtr->pixAryUC[9][4] = 0;
   fontSTPtr->pixAryUC[9][5] = 14;
   fontSTPtr->pixAryUC[9][6] = 192;
   fontSTPtr->pixAryUC[9][7] = 0;
   fontSTPtr->pixAryUC[9][8] = 12;
   fontSTPtr->pixAryUC[9][9] = 224;
   fontSTPtr->pixAryUC[9][10] = 128;
   fontSTPtr->pixAryUC[9][11] = 7;
   fontSTPtr->pixAryUC[9][12] = 60;
   fontSTPtr->pixAryUC[9][13] = 0;
   fontSTPtr->pixAryUC[9][14] = 0;
   fontSTPtr->pixAryUC[9][15] = 0;
   fontSTPtr->pixAryUC[9][16] = 0;
   fontSTPtr->pixAryUC[9][17] = 0;

   return 0;
} /*addAscii41_smallFont*/

/*___________________________________________
` Fun10: addAscii42_smallFont
`  - adds character `*` (ascii 42) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii42_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[10])
      free(fontSTPtr->pixAryUC[10]);

   fontSTPtr->pixAryUC[10] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[10])
      return 1;

   fontSTPtr->widthArySS[10] = 12;
   fontSTPtr->lenArySS[10] = 18;

   fontSTPtr->pixAryUC[10][0] = 0;
   fontSTPtr->pixAryUC[10][1] = 0;
   fontSTPtr->pixAryUC[10][2] = 0;
   fontSTPtr->pixAryUC[10][3] = 96;
   fontSTPtr->pixAryUC[10][4] = 192;
   fontSTPtr->pixAryUC[10][5] = 54;
   fontSTPtr->pixAryUC[10][6] = 240;
   fontSTPtr->pixAryUC[10][7] = 192;
   fontSTPtr->pixAryUC[10][8] = 54;
   fontSTPtr->pixAryUC[10][9] = 96;
   fontSTPtr->pixAryUC[10][10] = 0;
   fontSTPtr->pixAryUC[10][11] = 0;
   fontSTPtr->pixAryUC[10][12] = 0;
   fontSTPtr->pixAryUC[10][13] = 0;
   fontSTPtr->pixAryUC[10][14] = 0;
   fontSTPtr->pixAryUC[10][15] = 0;
   fontSTPtr->pixAryUC[10][16] = 0;
   fontSTPtr->pixAryUC[10][17] = 0;

   return 0;
} /*addAscii42_smallFont*/

/*___________________________________________
` Fun11: addAscii43_smallFont
`  - adds character `+` (ascii 43) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii43_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[11])
      free(fontSTPtr->pixAryUC[11]);

   fontSTPtr->pixAryUC[11] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[11])
      return 1;

   fontSTPtr->widthArySS[11] = 12;
   fontSTPtr->lenArySS[11] = 18;

   fontSTPtr->pixAryUC[11][0] = 0;
   fontSTPtr->pixAryUC[11][1] = 0;
   fontSTPtr->pixAryUC[11][2] = 0;
   fontSTPtr->pixAryUC[11][3] = 96;
   fontSTPtr->pixAryUC[11][4] = 0;
   fontSTPtr->pixAryUC[11][5] = 6;
   fontSTPtr->pixAryUC[11][6] = 254;
   fontSTPtr->pixAryUC[11][7] = 231;
   fontSTPtr->pixAryUC[11][8] = 127;
   fontSTPtr->pixAryUC[11][9] = 96;
   fontSTPtr->pixAryUC[11][10] = 0;
   fontSTPtr->pixAryUC[11][11] = 6;
   fontSTPtr->pixAryUC[11][12] = 96;
   fontSTPtr->pixAryUC[11][13] = 0;
   fontSTPtr->pixAryUC[11][14] = 0;
   fontSTPtr->pixAryUC[11][15] = 0;
   fontSTPtr->pixAryUC[11][16] = 0;
   fontSTPtr->pixAryUC[11][17] = 0;

   return 0;
} /*addAscii43_smallFont*/

/*___________________________________________
` Fun12: addAscii44_smallFont
`  - adds character `,` (ascii 44) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii44_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[12])
      free(fontSTPtr->pixAryUC[12]);

   fontSTPtr->pixAryUC[12] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[12])
      return 1;

   fontSTPtr->widthArySS[12] = 12;
   fontSTPtr->lenArySS[12] = 18;

   fontSTPtr->pixAryUC[12][0] = 0;
   fontSTPtr->pixAryUC[12][1] = 0;
   fontSTPtr->pixAryUC[12][2] = 0;
   fontSTPtr->pixAryUC[12][3] = 0;
   fontSTPtr->pixAryUC[12][4] = 0;
   fontSTPtr->pixAryUC[12][5] = 0;
   fontSTPtr->pixAryUC[12][6] = 0;
   fontSTPtr->pixAryUC[12][7] = 0;
   fontSTPtr->pixAryUC[12][8] = 0;
   fontSTPtr->pixAryUC[12][9] = 240;
   fontSTPtr->pixAryUC[12][10] = 0;
   fontSTPtr->pixAryUC[12][11] = 15;
   fontSTPtr->pixAryUC[12][12] = 240;
   fontSTPtr->pixAryUC[12][13] = 0;
   fontSTPtr->pixAryUC[12][14] = 14;
   fontSTPtr->pixAryUC[12][15] = 108;
   fontSTPtr->pixAryUC[12][16] = 128;
   fontSTPtr->pixAryUC[12][17] = 3;

   return 0;
} /*addAscii44_smallFont*/

/*___________________________________________
` Fun13: addAscii45_smallFont
`  - adds character `-` (ascii 45) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii45_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[13])
      free(fontSTPtr->pixAryUC[13]);

   fontSTPtr->pixAryUC[13] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[13])
      return 1;

   fontSTPtr->widthArySS[13] = 12;
   fontSTPtr->lenArySS[13] = 18;

   fontSTPtr->pixAryUC[13][0] = 0;
   fontSTPtr->pixAryUC[13][1] = 0;
   fontSTPtr->pixAryUC[13][2] = 0;
   fontSTPtr->pixAryUC[13][3] = 0;
   fontSTPtr->pixAryUC[13][4] = 0;
   fontSTPtr->pixAryUC[13][5] = 0;
   fontSTPtr->pixAryUC[13][6] = 252;
   fontSTPtr->pixAryUC[13][7] = 193;
   fontSTPtr->pixAryUC[13][8] = 31;
   fontSTPtr->pixAryUC[13][9] = 0;
   fontSTPtr->pixAryUC[13][10] = 0;
   fontSTPtr->pixAryUC[13][11] = 0;
   fontSTPtr->pixAryUC[13][12] = 0;
   fontSTPtr->pixAryUC[13][13] = 0;
   fontSTPtr->pixAryUC[13][14] = 0;
   fontSTPtr->pixAryUC[13][15] = 0;
   fontSTPtr->pixAryUC[13][16] = 0;
   fontSTPtr->pixAryUC[13][17] = 0;

   return 0;
} /*addAscii45_smallFont*/

/*___________________________________________
` Fun14: addAscii46_smallFont
`  - adds character `.` (ascii 46) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii46_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[14])
      free(fontSTPtr->pixAryUC[14]);

   fontSTPtr->pixAryUC[14] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[14])
      return 1;

   fontSTPtr->widthArySS[14] = 12;
   fontSTPtr->lenArySS[14] = 18;

   fontSTPtr->pixAryUC[14][0] = 0;
   fontSTPtr->pixAryUC[14][1] = 0;
   fontSTPtr->pixAryUC[14][2] = 0;
   fontSTPtr->pixAryUC[14][3] = 0;
   fontSTPtr->pixAryUC[14][4] = 0;
   fontSTPtr->pixAryUC[14][5] = 0;
   fontSTPtr->pixAryUC[14][6] = 0;
   fontSTPtr->pixAryUC[14][7] = 0;
   fontSTPtr->pixAryUC[14][8] = 0;
   fontSTPtr->pixAryUC[14][9] = 224;
   fontSTPtr->pixAryUC[14][10] = 1;
   fontSTPtr->pixAryUC[14][11] = 30;
   fontSTPtr->pixAryUC[14][12] = 224;
   fontSTPtr->pixAryUC[14][13] = 1;
   fontSTPtr->pixAryUC[14][14] = 0;
   fontSTPtr->pixAryUC[14][15] = 0;
   fontSTPtr->pixAryUC[14][16] = 0;
   fontSTPtr->pixAryUC[14][17] = 0;

   return 0;
} /*addAscii46_smallFont*/

/*___________________________________________
` Fun15: addAscii47_smallFont
`  - adds character `/` (ascii 47) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii47_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[15])
      free(fontSTPtr->pixAryUC[15]);

   fontSTPtr->pixAryUC[15] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[15])
      return 1;

   fontSTPtr->widthArySS[15] = 12;
   fontSTPtr->lenArySS[15] = 18;

   fontSTPtr->pixAryUC[15][0] = 0;
   fontSTPtr->pixAryUC[15][1] = 0;
   fontSTPtr->pixAryUC[15][2] = 48;
   fontSTPtr->pixAryUC[15][3] = 128;
   fontSTPtr->pixAryUC[15][4] = 1;
   fontSTPtr->pixAryUC[15][5] = 12;
   fontSTPtr->pixAryUC[15][6] = 96;
   fontSTPtr->pixAryUC[15][7] = 0;
   fontSTPtr->pixAryUC[15][8] = 3;
   fontSTPtr->pixAryUC[15][9] = 24;
   fontSTPtr->pixAryUC[15][10] = 192;
   fontSTPtr->pixAryUC[15][11] = 0;
   fontSTPtr->pixAryUC[15][12] = 6;
   fontSTPtr->pixAryUC[15][13] = 0;
   fontSTPtr->pixAryUC[15][14] = 0;
   fontSTPtr->pixAryUC[15][15] = 0;
   fontSTPtr->pixAryUC[15][16] = 0;
   fontSTPtr->pixAryUC[15][17] = 0;

   return 0;
} /*addAscii47_smallFont*/

/*___________________________________________
` Fun16: addAscii48_smallFont
`  - adds character `0` (ascii 48) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii48_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[16])
      free(fontSTPtr->pixAryUC[16]);

   fontSTPtr->pixAryUC[16] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[16])
      return 1;

   fontSTPtr->widthArySS[16] = 12;
   fontSTPtr->lenArySS[16] = 18;

   fontSTPtr->pixAryUC[16][0] = 0;
   fontSTPtr->pixAryUC[16][1] = 0;
   fontSTPtr->pixAryUC[16][2] = 15;
   fontSTPtr->pixAryUC[16][3] = 248;
   fontSTPtr->pixAryUC[16][4] = 193;
   fontSTPtr->pixAryUC[16][5] = 49;
   fontSTPtr->pixAryUC[16][6] = 54;
   fontSTPtr->pixAryUC[16][7] = 102;
   fontSTPtr->pixAryUC[16][8] = 102;
   fontSTPtr->pixAryUC[16][9] = 204;
   fontSTPtr->pixAryUC[16][10] = 130;
   fontSTPtr->pixAryUC[16][11] = 31;
   fontSTPtr->pixAryUC[16][12] = 240;
   fontSTPtr->pixAryUC[16][13] = 3;
   fontSTPtr->pixAryUC[16][14] = 0;
   fontSTPtr->pixAryUC[16][15] = 0;
   fontSTPtr->pixAryUC[16][16] = 0;
   fontSTPtr->pixAryUC[16][17] = 0;

   return 0;
} /*addAscii48_smallFont*/

/*___________________________________________
` Fun17: addAscii49_smallFont
`  - adds character `1` (ascii 49) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii49_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[17])
      free(fontSTPtr->pixAryUC[17]);

   fontSTPtr->pixAryUC[17] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[17])
      return 1;

   fontSTPtr->widthArySS[17] = 12;
   fontSTPtr->lenArySS[17] = 18;

   fontSTPtr->pixAryUC[17][0] = 0;
   fontSTPtr->pixAryUC[17][1] = 0;
   fontSTPtr->pixAryUC[17][2] = 7;
   fontSTPtr->pixAryUC[17][3] = 120;
   fontSTPtr->pixAryUC[17][4] = 192;
   fontSTPtr->pixAryUC[17][5] = 6;
   fontSTPtr->pixAryUC[17][6] = 102;
   fontSTPtr->pixAryUC[17][7] = 0;
   fontSTPtr->pixAryUC[17][8] = 6;
   fontSTPtr->pixAryUC[17][9] = 96;
   fontSTPtr->pixAryUC[17][10] = 192;
   fontSTPtr->pixAryUC[17][11] = 63;
   fontSTPtr->pixAryUC[17][12] = 254;
   fontSTPtr->pixAryUC[17][13] = 7;
   fontSTPtr->pixAryUC[17][14] = 0;
   fontSTPtr->pixAryUC[17][15] = 0;
   fontSTPtr->pixAryUC[17][16] = 0;
   fontSTPtr->pixAryUC[17][17] = 0;

   return 0;
} /*addAscii49_smallFont*/

/*___________________________________________
` Fun18: addAscii50_smallFont
`  - adds character `2` (ascii 50) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii50_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[18])
      free(fontSTPtr->pixAryUC[18]);

   fontSTPtr->pixAryUC[18] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[18])
      return 1;

   fontSTPtr->widthArySS[18] = 12;
   fontSTPtr->lenArySS[18] = 18;

   fontSTPtr->pixAryUC[18][0] = 0;
   fontSTPtr->pixAryUC[18][1] = 192;
   fontSTPtr->pixAryUC[18][2] = 7;
   fontSTPtr->pixAryUC[18][3] = 254;
   fontSTPtr->pixAryUC[18][4] = 96;
   fontSTPtr->pixAryUC[18][5] = 12;
   fontSTPtr->pixAryUC[18][6] = 192;
   fontSTPtr->pixAryUC[18][7] = 0;
   fontSTPtr->pixAryUC[18][8] = 3;
   fontSTPtr->pixAryUC[18][9] = 24;
   fontSTPtr->pixAryUC[18][10] = 192;
   fontSTPtr->pixAryUC[18][11] = 31;
   fontSTPtr->pixAryUC[18][12] = 254;
   fontSTPtr->pixAryUC[18][13] = 3;
   fontSTPtr->pixAryUC[18][14] = 0;
   fontSTPtr->pixAryUC[18][15] = 0;
   fontSTPtr->pixAryUC[18][16] = 0;
   fontSTPtr->pixAryUC[18][17] = 0;

   return 0;
} /*addAscii50_smallFont*/

/*___________________________________________
` Fun19: addAscii51_smallFont
`  - adds character `3` (ascii 51) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii51_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[19])
      free(fontSTPtr->pixAryUC[19]);

   fontSTPtr->pixAryUC[19] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[19])
      return 1;

   fontSTPtr->widthArySS[19] = 12;
   fontSTPtr->lenArySS[19] = 18;

   fontSTPtr->pixAryUC[19][0] = 0;
   fontSTPtr->pixAryUC[19][1] = 192;
   fontSTPtr->pixAryUC[19][2] = 15;
   fontSTPtr->pixAryUC[19][3] = 254;
   fontSTPtr->pixAryUC[19][4] = 97;
   fontSTPtr->pixAryUC[19][5] = 48;
   fontSTPtr->pixAryUC[19][6] = 224;
   fontSTPtr->pixAryUC[19][7] = 1;
   fontSTPtr->pixAryUC[19][8] = 30;
   fontSTPtr->pixAryUC[19][9] = 6;
   fontSTPtr->pixAryUC[19][10] = 227;
   fontSTPtr->pixAryUC[19][11] = 31;
   fontSTPtr->pixAryUC[19][12] = 252;
   fontSTPtr->pixAryUC[19][13] = 0;
   fontSTPtr->pixAryUC[19][14] = 0;
   fontSTPtr->pixAryUC[19][15] = 0;
   fontSTPtr->pixAryUC[19][16] = 0;
   fontSTPtr->pixAryUC[19][17] = 0;

   return 0;
} /*addAscii51_smallFont*/

/*___________________________________________
` Fun20: addAscii52_smallFont
`  - adds character `4` (ascii 52) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii52_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[20])
      free(fontSTPtr->pixAryUC[20]);

   fontSTPtr->pixAryUC[20] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[20])
      return 1;

   fontSTPtr->widthArySS[20] = 12;
   fontSTPtr->lenArySS[20] = 18;

   fontSTPtr->pixAryUC[20][0] = 0;
   fontSTPtr->pixAryUC[20][1] = 0;
   fontSTPtr->pixAryUC[20][2] = 30;
   fontSTPtr->pixAryUC[20][3] = 240;
   fontSTPtr->pixAryUC[20][4] = 131;
   fontSTPtr->pixAryUC[20][5] = 49;
   fontSTPtr->pixAryUC[20][6] = 12;
   fontSTPtr->pixAryUC[20][7] = 227;
   fontSTPtr->pixAryUC[20][8] = 127;
   fontSTPtr->pixAryUC[20][9] = 0;
   fontSTPtr->pixAryUC[20][10] = 3;
   fontSTPtr->pixAryUC[20][11] = 48;
   fontSTPtr->pixAryUC[20][12] = 0;
   fontSTPtr->pixAryUC[20][13] = 3;
   fontSTPtr->pixAryUC[20][14] = 0;
   fontSTPtr->pixAryUC[20][15] = 0;
   fontSTPtr->pixAryUC[20][16] = 0;
   fontSTPtr->pixAryUC[20][17] = 0;

   return 0;
} /*addAscii52_smallFont*/

/*___________________________________________
` Fun21: addAscii53_smallFont
`  - adds character `5` (ascii 53) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii53_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[21])
      free(fontSTPtr->pixAryUC[21]);

   fontSTPtr->pixAryUC[21] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[21])
      return 1;

   fontSTPtr->widthArySS[21] = 12;
   fontSTPtr->lenArySS[21] = 18;

   fontSTPtr->pixAryUC[21][0] = 0;
   fontSTPtr->pixAryUC[21][1] = 192;
   fontSTPtr->pixAryUC[21][2] = 63;
   fontSTPtr->pixAryUC[21][3] = 252;
   fontSTPtr->pixAryUC[21][4] = 195;
   fontSTPtr->pixAryUC[21][5] = 0;
   fontSTPtr->pixAryUC[21][6] = 252;
   fontSTPtr->pixAryUC[21][7] = 193;
   fontSTPtr->pixAryUC[21][8] = 63;
   fontSTPtr->pixAryUC[21][9] = 0;
   fontSTPtr->pixAryUC[21][10] = 198;
   fontSTPtr->pixAryUC[21][11] = 63;
   fontSTPtr->pixAryUC[21][12] = 252;
   fontSTPtr->pixAryUC[21][13] = 1;
   fontSTPtr->pixAryUC[21][14] = 0;
   fontSTPtr->pixAryUC[21][15] = 0;
   fontSTPtr->pixAryUC[21][16] = 0;
   fontSTPtr->pixAryUC[21][17] = 0;

   return 0;
} /*addAscii53_smallFont*/

/*___________________________________________
` Fun22: addAscii54_smallFont
`  - adds character `6` (ascii 54) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii54_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[22])
      free(fontSTPtr->pixAryUC[22]);

   fontSTPtr->pixAryUC[22] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[22])
      return 1;

   fontSTPtr->widthArySS[22] = 12;
   fontSTPtr->lenArySS[22] = 18;

   fontSTPtr->pixAryUC[22][0] = 0;
   fontSTPtr->pixAryUC[22][1] = 0;
   fontSTPtr->pixAryUC[22][2] = 31;
   fontSTPtr->pixAryUC[22][3] = 248;
   fontSTPtr->pixAryUC[22][4] = 195;
   fontSTPtr->pixAryUC[22][5] = 0;
   fontSTPtr->pixAryUC[22][6] = 252;
   fontSTPtr->pixAryUC[22][7] = 193;
   fontSTPtr->pixAryUC[22][8] = 48;
   fontSTPtr->pixAryUC[22][9] = 12;
   fontSTPtr->pixAryUC[22][10] = 131;
   fontSTPtr->pixAryUC[22][11] = 31;
   fontSTPtr->pixAryUC[22][12] = 240;
   fontSTPtr->pixAryUC[22][13] = 0;
   fontSTPtr->pixAryUC[22][14] = 0;
   fontSTPtr->pixAryUC[22][15] = 0;
   fontSTPtr->pixAryUC[22][16] = 0;
   fontSTPtr->pixAryUC[22][17] = 0;

   return 0;
} /*addAscii54_smallFont*/

/*___________________________________________
` Fun23: addAscii55_smallFont
`  - adds character `7` (ascii 55) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii55_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[23])
      free(fontSTPtr->pixAryUC[23]);

   fontSTPtr->pixAryUC[23] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[23])
      return 1;

   fontSTPtr->widthArySS[23] = 12;
   fontSTPtr->lenArySS[23] = 18;

   fontSTPtr->pixAryUC[23][0] = 0;
   fontSTPtr->pixAryUC[23][1] = 128;
   fontSTPtr->pixAryUC[23][2] = 127;
   fontSTPtr->pixAryUC[23][3] = 248;
   fontSTPtr->pixAryUC[23][4] = 3;
   fontSTPtr->pixAryUC[23][5] = 24;
   fontSTPtr->pixAryUC[23][6] = 220;
   fontSTPtr->pixAryUC[23][7] = 198;
   fontSTPtr->pixAryUC[23][8] = 118;
   fontSTPtr->pixAryUC[23][9] = 96;
   fontSTPtr->pixAryUC[23][10] = 0;
   fontSTPtr->pixAryUC[23][11] = 6;
   fontSTPtr->pixAryUC[23][12] = 96;
   fontSTPtr->pixAryUC[23][13] = 0;
   fontSTPtr->pixAryUC[23][14] = 0;
   fontSTPtr->pixAryUC[23][15] = 0;
   fontSTPtr->pixAryUC[23][16] = 0;
   fontSTPtr->pixAryUC[23][17] = 0;

   return 0;
} /*addAscii55_smallFont*/

/*___________________________________________
` Fun24: addAscii56_smallFont
`  - adds character `8` (ascii 56) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii56_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[24])
      free(fontSTPtr->pixAryUC[24]);

   fontSTPtr->pixAryUC[24] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[24])
      return 1;

   fontSTPtr->widthArySS[24] = 12;
   fontSTPtr->lenArySS[24] = 18;

   fontSTPtr->pixAryUC[24][0] = 0;
   fontSTPtr->pixAryUC[24][1] = 128;
   fontSTPtr->pixAryUC[24][2] = 31;
   fontSTPtr->pixAryUC[24][3] = 12;
   fontSTPtr->pixAryUC[24][4] = 195;
   fontSTPtr->pixAryUC[24][5] = 48;
   fontSTPtr->pixAryUC[24][6] = 248;
   fontSTPtr->pixAryUC[24][7] = 129;
   fontSTPtr->pixAryUC[24][8] = 31;
   fontSTPtr->pixAryUC[24][9] = 12;
   fontSTPtr->pixAryUC[24][10] = 195;
   fontSTPtr->pixAryUC[24][11] = 48;
   fontSTPtr->pixAryUC[24][12] = 248;
   fontSTPtr->pixAryUC[24][13] = 1;
   fontSTPtr->pixAryUC[24][14] = 0;
   fontSTPtr->pixAryUC[24][15] = 0;
   fontSTPtr->pixAryUC[24][16] = 0;
   fontSTPtr->pixAryUC[24][17] = 0;

   return 0;
} /*addAscii56_smallFont*/

/*___________________________________________
` Fun25: addAscii57_smallFont
`  - adds character `9` (ascii 57) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii57_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[25])
      free(fontSTPtr->pixAryUC[25]);

   fontSTPtr->pixAryUC[25] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[25])
      return 1;

   fontSTPtr->widthArySS[25] = 12;
   fontSTPtr->lenArySS[25] = 18;

   fontSTPtr->pixAryUC[25][0] = 0;
   fontSTPtr->pixAryUC[25][1] = 128;
   fontSTPtr->pixAryUC[25][2] = 63;
   fontSTPtr->pixAryUC[25][3] = 12;
   fontSTPtr->pixAryUC[25][4] = 198;
   fontSTPtr->pixAryUC[25][5] = 96;
   fontSTPtr->pixAryUC[25][6] = 248;
   fontSTPtr->pixAryUC[25][7] = 3;
   fontSTPtr->pixAryUC[25][8] = 24;
   fontSTPtr->pixAryUC[25][9] = 192;
   fontSTPtr->pixAryUC[25][10] = 0;
   fontSTPtr->pixAryUC[25][11] = 6;
   fontSTPtr->pixAryUC[25][12] = 48;
   fontSTPtr->pixAryUC[25][13] = 0;
   fontSTPtr->pixAryUC[25][14] = 0;
   fontSTPtr->pixAryUC[25][15] = 0;
   fontSTPtr->pixAryUC[25][16] = 0;
   fontSTPtr->pixAryUC[25][17] = 0;

   return 0;
} /*addAscii57_smallFont*/

/*___________________________________________
` Fun26: addAscii58_smallFont
`  - adds character `:` (ascii 58) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii58_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[26])
      free(fontSTPtr->pixAryUC[26]);

   fontSTPtr->pixAryUC[26] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[26])
      return 1;

   fontSTPtr->widthArySS[26] = 12;
   fontSTPtr->lenArySS[26] = 18;

   fontSTPtr->pixAryUC[26][0] = 0;
   fontSTPtr->pixAryUC[26][1] = 0;
   fontSTPtr->pixAryUC[26][2] = 0;
   fontSTPtr->pixAryUC[26][3] = 112;
   fontSTPtr->pixAryUC[26][4] = 0;
   fontSTPtr->pixAryUC[26][5] = 7;
   fontSTPtr->pixAryUC[26][6] = 112;
   fontSTPtr->pixAryUC[26][7] = 0;
   fontSTPtr->pixAryUC[26][8] = 0;
   fontSTPtr->pixAryUC[26][9] = 112;
   fontSTPtr->pixAryUC[26][10] = 0;
   fontSTPtr->pixAryUC[26][11] = 7;
   fontSTPtr->pixAryUC[26][12] = 112;
   fontSTPtr->pixAryUC[26][13] = 0;
   fontSTPtr->pixAryUC[26][14] = 0;
   fontSTPtr->pixAryUC[26][15] = 0;
   fontSTPtr->pixAryUC[26][16] = 0;
   fontSTPtr->pixAryUC[26][17] = 0;

   return 0;
} /*addAscii58_smallFont*/

/*___________________________________________
` Fun27: addAscii59_smallFont
`  - adds character `;` (ascii 59) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii59_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[27])
      free(fontSTPtr->pixAryUC[27]);

   fontSTPtr->pixAryUC[27] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[27])
      return 1;

   fontSTPtr->widthArySS[27] = 12;
   fontSTPtr->lenArySS[27] = 18;

   fontSTPtr->pixAryUC[27][0] = 0;
   fontSTPtr->pixAryUC[27][1] = 0;
   fontSTPtr->pixAryUC[27][2] = 0;
   fontSTPtr->pixAryUC[27][3] = 112;
   fontSTPtr->pixAryUC[27][4] = 0;
   fontSTPtr->pixAryUC[27][5] = 7;
   fontSTPtr->pixAryUC[27][6] = 0;
   fontSTPtr->pixAryUC[27][7] = 0;
   fontSTPtr->pixAryUC[27][8] = 0;
   fontSTPtr->pixAryUC[27][9] = 112;
   fontSTPtr->pixAryUC[27][10] = 0;
   fontSTPtr->pixAryUC[27][11] = 7;
   fontSTPtr->pixAryUC[27][12] = 96;
   fontSTPtr->pixAryUC[27][13] = 0;
   fontSTPtr->pixAryUC[27][14] = 3;
   fontSTPtr->pixAryUC[27][15] = 24;
   fontSTPtr->pixAryUC[27][16] = 0;
   fontSTPtr->pixAryUC[27][17] = 0;

   return 0;
} /*addAscii59_smallFont*/

/*___________________________________________
` Fun28: addAscii60_smallFont
`  - adds character `<` (ascii 60) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii60_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[28])
      free(fontSTPtr->pixAryUC[28]);

   fontSTPtr->pixAryUC[28] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[28])
      return 1;

   fontSTPtr->widthArySS[28] = 12;
   fontSTPtr->lenArySS[28] = 18;

   fontSTPtr->pixAryUC[28][0] = 0;
   fontSTPtr->pixAryUC[28][1] = 0;
   fontSTPtr->pixAryUC[28][2] = 24;
   fontSTPtr->pixAryUC[28][3] = 96;
   fontSTPtr->pixAryUC[28][4] = 128;
   fontSTPtr->pixAryUC[28][5] = 1;
   fontSTPtr->pixAryUC[28][6] = 6;
   fontSTPtr->pixAryUC[28][7] = 96;
   fontSTPtr->pixAryUC[28][8] = 0;
   fontSTPtr->pixAryUC[28][9] = 24;
   fontSTPtr->pixAryUC[28][10] = 0;
   fontSTPtr->pixAryUC[28][11] = 6;
   fontSTPtr->pixAryUC[28][12] = 128;
   fontSTPtr->pixAryUC[28][13] = 1;
   fontSTPtr->pixAryUC[28][14] = 0;
   fontSTPtr->pixAryUC[28][15] = 0;
   fontSTPtr->pixAryUC[28][16] = 0;
   fontSTPtr->pixAryUC[28][17] = 0;

   return 0;
} /*addAscii60_smallFont*/

/*___________________________________________
` Fun29: addAscii61_smallFont
`  - adds character `=` (ascii 61) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii61_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[29])
      free(fontSTPtr->pixAryUC[29]);

   fontSTPtr->pixAryUC[29] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[29])
      return 1;

   fontSTPtr->widthArySS[29] = 12;
   fontSTPtr->lenArySS[29] = 18;

   fontSTPtr->pixAryUC[29][0] = 0;
   fontSTPtr->pixAryUC[29][1] = 0;
   fontSTPtr->pixAryUC[29][2] = 0;
   fontSTPtr->pixAryUC[29][3] = 252;
   fontSTPtr->pixAryUC[29][4] = 195;
   fontSTPtr->pixAryUC[29][5] = 63;
   fontSTPtr->pixAryUC[29][6] = 0;
   fontSTPtr->pixAryUC[29][7] = 192;
   fontSTPtr->pixAryUC[29][8] = 63;
   fontSTPtr->pixAryUC[29][9] = 252;
   fontSTPtr->pixAryUC[29][10] = 3;
   fontSTPtr->pixAryUC[29][11] = 0;
   fontSTPtr->pixAryUC[29][12] = 0;
   fontSTPtr->pixAryUC[29][13] = 0;
   fontSTPtr->pixAryUC[29][14] = 0;
   fontSTPtr->pixAryUC[29][15] = 0;
   fontSTPtr->pixAryUC[29][16] = 0;
   fontSTPtr->pixAryUC[29][17] = 0;

   return 0;
} /*addAscii61_smallFont*/

/*___________________________________________
` Fun30: addAscii62_smallFont
`  - adds character `>` (ascii 62) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii62_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[30])
      free(fontSTPtr->pixAryUC[30]);

   fontSTPtr->pixAryUC[30] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[30])
      return 1;

   fontSTPtr->widthArySS[30] = 12;
   fontSTPtr->lenArySS[30] = 18;

   fontSTPtr->pixAryUC[30][0] = 0;
   fontSTPtr->pixAryUC[30][1] = 192;
   fontSTPtr->pixAryUC[30][2] = 0;
   fontSTPtr->pixAryUC[30][3] = 48;
   fontSTPtr->pixAryUC[30][4] = 0;
   fontSTPtr->pixAryUC[30][5] = 12;
   fontSTPtr->pixAryUC[30][6] = 0;
   fontSTPtr->pixAryUC[30][7] = 3;
   fontSTPtr->pixAryUC[30][8] = 48;
   fontSTPtr->pixAryUC[30][9] = 192;
   fontSTPtr->pixAryUC[30][10] = 0;
   fontSTPtr->pixAryUC[30][11] = 3;
   fontSTPtr->pixAryUC[30][12] = 12;
   fontSTPtr->pixAryUC[30][13] = 0;
   fontSTPtr->pixAryUC[30][14] = 0;
   fontSTPtr->pixAryUC[30][15] = 0;
   fontSTPtr->pixAryUC[30][16] = 0;
   fontSTPtr->pixAryUC[30][17] = 0;

   return 0;
} /*addAscii62_smallFont*/

/*___________________________________________
` Fun31: addAscii63_smallFont
`  - adds character `?` (ascii 63) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii63_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[31])
      free(fontSTPtr->pixAryUC[31]);

   fontSTPtr->pixAryUC[31] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[31])
      return 1;

   fontSTPtr->widthArySS[31] = 12;
   fontSTPtr->lenArySS[31] = 18;

   fontSTPtr->pixAryUC[31][0] = 0;
   fontSTPtr->pixAryUC[31][1] = 0;
   fontSTPtr->pixAryUC[31][2] = 62;
   fontSTPtr->pixAryUC[31][3] = 240;
   fontSTPtr->pixAryUC[31][4] = 135;
   fontSTPtr->pixAryUC[31][5] = 97;
   fontSTPtr->pixAryUC[31][6] = 16;
   fontSTPtr->pixAryUC[31][7] = 3;
   fontSTPtr->pixAryUC[31][8] = 24;
   fontSTPtr->pixAryUC[31][9] = 128;
   fontSTPtr->pixAryUC[31][10] = 1;
   fontSTPtr->pixAryUC[31][11] = 0;
   fontSTPtr->pixAryUC[31][12] = 128;
   fontSTPtr->pixAryUC[31][13] = 1;
   fontSTPtr->pixAryUC[31][14] = 0;
   fontSTPtr->pixAryUC[31][15] = 0;
   fontSTPtr->pixAryUC[31][16] = 0;
   fontSTPtr->pixAryUC[31][17] = 0;

   return 0;
} /*addAscii63_smallFont*/

/*___________________________________________
` Fun32: addAscii64_smallFont
`  - adds character `@` (ascii 64) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii64_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[32])
      free(fontSTPtr->pixAryUC[32]);

   fontSTPtr->pixAryUC[32] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[32])
      return 1;

   fontSTPtr->widthArySS[32] = 12;
   fontSTPtr->lenArySS[32] = 18;

   fontSTPtr->pixAryUC[32][0] = 0;
   fontSTPtr->pixAryUC[32][1] = 0;
   fontSTPtr->pixAryUC[32][2] = 0;
   fontSTPtr->pixAryUC[32][3] = 248;
   fontSTPtr->pixAryUC[32][4] = 193;
   fontSTPtr->pixAryUC[32][5] = 48;
   fontSTPtr->pixAryUC[32][6] = 230;
   fontSTPtr->pixAryUC[32][7] = 102;
   fontSTPtr->pixAryUC[32][8] = 105;
   fontSTPtr->pixAryUC[32][9] = 150;
   fontSTPtr->pixAryUC[32][10] = 100;
   fontSTPtr->pixAryUC[32][11] = 91;
   fontSTPtr->pixAryUC[32][12] = 124;
   fontSTPtr->pixAryUC[32][13] = 199;
   fontSTPtr->pixAryUC[32][14] = 7;
   fontSTPtr->pixAryUC[32][15] = 248;
   fontSTPtr->pixAryUC[32][16] = 0;
   fontSTPtr->pixAryUC[32][17] = 0;

   return 0;
} /*addAscii64_smallFont*/

/*___________________________________________
` Fun33: addAscii65_smallFont
`  - adds character `A` (ascii 65) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii65_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[33])
      free(fontSTPtr->pixAryUC[33]);

   fontSTPtr->pixAryUC[33] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[33])
      return 1;

   fontSTPtr->widthArySS[33] = 12;
   fontSTPtr->lenArySS[33] = 18;

   fontSTPtr->pixAryUC[33][0] = 0;
   fontSTPtr->pixAryUC[33][1] = 0;
   fontSTPtr->pixAryUC[33][2] = 6;
   fontSTPtr->pixAryUC[33][3] = 240;
   fontSTPtr->pixAryUC[33][4] = 128;
   fontSTPtr->pixAryUC[33][5] = 25;
   fontSTPtr->pixAryUC[33][6] = 252;
   fontSTPtr->pixAryUC[33][7] = 195;
   fontSTPtr->pixAryUC[33][8] = 63;
   fontSTPtr->pixAryUC[33][9] = 12;
   fontSTPtr->pixAryUC[33][10] = 195;
   fontSTPtr->pixAryUC[33][11] = 48;
   fontSTPtr->pixAryUC[33][12] = 158;
   fontSTPtr->pixAryUC[33][13] = 7;
   fontSTPtr->pixAryUC[33][14] = 0;
   fontSTPtr->pixAryUC[33][15] = 0;
   fontSTPtr->pixAryUC[33][16] = 0;
   fontSTPtr->pixAryUC[33][17] = 0;

   return 0;
} /*addAscii65_smallFont*/

/*___________________________________________
` Fun34: addAscii66_smallFont
`  - adds character `B` (ascii 66) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii66_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[34])
      free(fontSTPtr->pixAryUC[34]);

   fontSTPtr->pixAryUC[34] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[34])
      return 1;

   fontSTPtr->widthArySS[34] = 12;
   fontSTPtr->lenArySS[34] = 18;

   fontSTPtr->pixAryUC[34][0] = 0;
   fontSTPtr->pixAryUC[34][1] = 224;
   fontSTPtr->pixAryUC[34][2] = 31;
   fontSTPtr->pixAryUC[34][3] = 254;
   fontSTPtr->pixAryUC[34][4] = 99;
   fontSTPtr->pixAryUC[34][5] = 96;
   fontSTPtr->pixAryUC[34][6] = 254;
   fontSTPtr->pixAryUC[34][7] = 227;
   fontSTPtr->pixAryUC[34][8] = 63;
   fontSTPtr->pixAryUC[34][9] = 6;
   fontSTPtr->pixAryUC[34][10] = 230;
   fontSTPtr->pixAryUC[34][11] = 63;
   fontSTPtr->pixAryUC[34][12] = 254;
   fontSTPtr->pixAryUC[34][13] = 1;
   fontSTPtr->pixAryUC[34][14] = 0;
   fontSTPtr->pixAryUC[34][15] = 0;
   fontSTPtr->pixAryUC[34][16] = 0;
   fontSTPtr->pixAryUC[34][17] = 0;

   return 0;
} /*addAscii66_smallFont*/

/*___________________________________________
` Fun35: addAscii67_smallFont
`  - adds character `C` (ascii 67) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii67_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[35])
      free(fontSTPtr->pixAryUC[35]);

   fontSTPtr->pixAryUC[35] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[35])
      return 1;

   fontSTPtr->widthArySS[35] = 12;
   fontSTPtr->lenArySS[35] = 18;

   fontSTPtr->pixAryUC[35][0] = 0;
   fontSTPtr->pixAryUC[35][1] = 0;
   fontSTPtr->pixAryUC[35][2] = 31;
   fontSTPtr->pixAryUC[35][3] = 248;
   fontSTPtr->pixAryUC[35][4] = 195;
   fontSTPtr->pixAryUC[35][5] = 97;
   fontSTPtr->pixAryUC[35][6] = 6;
   fontSTPtr->pixAryUC[35][7] = 96;
   fontSTPtr->pixAryUC[35][8] = 0;
   fontSTPtr->pixAryUC[35][9] = 12;
   fontSTPtr->pixAryUC[35][10] = 134;
   fontSTPtr->pixAryUC[35][11] = 63;
   fontSTPtr->pixAryUC[35][12] = 240;
   fontSTPtr->pixAryUC[35][13] = 1;
   fontSTPtr->pixAryUC[35][14] = 0;
   fontSTPtr->pixAryUC[35][15] = 0;
   fontSTPtr->pixAryUC[35][16] = 0;
   fontSTPtr->pixAryUC[35][17] = 0;

   return 0;
} /*addAscii67_smallFont*/

/*___________________________________________
` Fun36: addAscii68_smallFont
`  - adds character `D` (ascii 68) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii68_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[36])
      free(fontSTPtr->pixAryUC[36]);

   fontSTPtr->pixAryUC[36] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[36])
      return 1;

   fontSTPtr->widthArySS[36] = 12;
   fontSTPtr->lenArySS[36] = 18;

   fontSTPtr->pixAryUC[36][0] = 0;
   fontSTPtr->pixAryUC[36][1] = 224;
   fontSTPtr->pixAryUC[36][2] = 15;
   fontSTPtr->pixAryUC[36][3] = 254;
   fontSTPtr->pixAryUC[36][4] = 97;
   fontSTPtr->pixAryUC[36][5] = 48;
   fontSTPtr->pixAryUC[36][6] = 6;
   fontSTPtr->pixAryUC[36][7] = 102;
   fontSTPtr->pixAryUC[36][8] = 96;
   fontSTPtr->pixAryUC[36][9] = 6;
   fontSTPtr->pixAryUC[36][10] = 227;
   fontSTPtr->pixAryUC[36][11] = 31;
   fontSTPtr->pixAryUC[36][12] = 254;
   fontSTPtr->pixAryUC[36][13] = 0;
   fontSTPtr->pixAryUC[36][14] = 0;
   fontSTPtr->pixAryUC[36][15] = 0;
   fontSTPtr->pixAryUC[36][16] = 0;
   fontSTPtr->pixAryUC[36][17] = 0;

   return 0;
} /*addAscii68_smallFont*/

/*___________________________________________
` Fun37: addAscii69_smallFont
`  - adds character `E` (ascii 69) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii69_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[37])
      free(fontSTPtr->pixAryUC[37]);

   fontSTPtr->pixAryUC[37] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[37])
      return 1;

   fontSTPtr->widthArySS[37] = 12;
   fontSTPtr->lenArySS[37] = 18;

   fontSTPtr->pixAryUC[37][0] = 0;
   fontSTPtr->pixAryUC[37][1] = 224;
   fontSTPtr->pixAryUC[37][2] = 127;
   fontSTPtr->pixAryUC[37][3] = 254;
   fontSTPtr->pixAryUC[37][4] = 103;
   fontSTPtr->pixAryUC[37][5] = 0;
   fontSTPtr->pixAryUC[37][6] = 254;
   fontSTPtr->pixAryUC[37][7] = 224;
   fontSTPtr->pixAryUC[37][8] = 15;
   fontSTPtr->pixAryUC[37][9] = 6;
   fontSTPtr->pixAryUC[37][10] = 224;
   fontSTPtr->pixAryUC[37][11] = 127;
   fontSTPtr->pixAryUC[37][12] = 254;
   fontSTPtr->pixAryUC[37][13] = 7;
   fontSTPtr->pixAryUC[37][14] = 0;
   fontSTPtr->pixAryUC[37][15] = 0;
   fontSTPtr->pixAryUC[37][16] = 0;
   fontSTPtr->pixAryUC[37][17] = 0;

   return 0;
} /*addAscii69_smallFont*/

/*___________________________________________
` Fun38: addAscii70_smallFont
`  - adds character `F` (ascii 70) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii70_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[38])
      free(fontSTPtr->pixAryUC[38]);

   fontSTPtr->pixAryUC[38] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[38])
      return 1;

   fontSTPtr->widthArySS[38] = 12;
   fontSTPtr->lenArySS[38] = 18;

   fontSTPtr->pixAryUC[38][0] = 0;
   fontSTPtr->pixAryUC[38][1] = 128;
   fontSTPtr->pixAryUC[38][2] = 255;
   fontSTPtr->pixAryUC[38][3] = 248;
   fontSTPtr->pixAryUC[38][4] = 143;
   fontSTPtr->pixAryUC[38][5] = 129;
   fontSTPtr->pixAryUC[38][6] = 248;
   fontSTPtr->pixAryUC[38][7] = 131;
   fontSTPtr->pixAryUC[38][8] = 63;
   fontSTPtr->pixAryUC[38][9] = 24;
   fontSTPtr->pixAryUC[38][10] = 128;
   fontSTPtr->pixAryUC[38][11] = 1;
   fontSTPtr->pixAryUC[38][12] = 126;
   fontSTPtr->pixAryUC[38][13] = 0;
   fontSTPtr->pixAryUC[38][14] = 0;
   fontSTPtr->pixAryUC[38][15] = 0;
   fontSTPtr->pixAryUC[38][16] = 0;
   fontSTPtr->pixAryUC[38][17] = 0;

   return 0;
} /*addAscii70_smallFont*/

/*___________________________________________
` Fun39: addAscii71_smallFont
`  - adds character `G` (ascii 71) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii71_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[39])
      free(fontSTPtr->pixAryUC[39]);

   fontSTPtr->pixAryUC[39] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[39])
      return 1;

   fontSTPtr->widthArySS[39] = 12;
   fontSTPtr->lenArySS[39] = 18;

   fontSTPtr->pixAryUC[39][0] = 0;
   fontSTPtr->pixAryUC[39][1] = 0;
   fontSTPtr->pixAryUC[39][2] = 63;
   fontSTPtr->pixAryUC[39][3] = 248;
   fontSTPtr->pixAryUC[39][4] = 199;
   fontSTPtr->pixAryUC[39][5] = 0;
   fontSTPtr->pixAryUC[39][6] = 198;
   fontSTPtr->pixAryUC[39][7] = 103;
   fontSTPtr->pixAryUC[39][8] = 124;
   fontSTPtr->pixAryUC[39][9] = 12;
   fontSTPtr->pixAryUC[39][10] = 134;
   fontSTPtr->pixAryUC[39][11] = 63;
   fontSTPtr->pixAryUC[39][12] = 240;
   fontSTPtr->pixAryUC[39][13] = 1;
   fontSTPtr->pixAryUC[39][14] = 0;
   fontSTPtr->pixAryUC[39][15] = 0;
   fontSTPtr->pixAryUC[39][16] = 0;
   fontSTPtr->pixAryUC[39][17] = 0;

   return 0;
} /*addAscii71_smallFont*/

/*___________________________________________
` Fun40: addAscii72_smallFont
`  - adds character `H` (ascii 72) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii72_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[40])
      free(fontSTPtr->pixAryUC[40]);

   fontSTPtr->pixAryUC[40] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[40])
      return 1;

   fontSTPtr->widthArySS[40] = 12;
   fontSTPtr->lenArySS[40] = 18;

   fontSTPtr->pixAryUC[40][0] = 0;
   fontSTPtr->pixAryUC[40][1] = 224;
   fontSTPtr->pixAryUC[40][2] = 121;
   fontSTPtr->pixAryUC[40][3] = 12;
   fontSTPtr->pixAryUC[40][4] = 195;
   fontSTPtr->pixAryUC[40][5] = 48;
   fontSTPtr->pixAryUC[40][6] = 252;
   fontSTPtr->pixAryUC[40][7] = 195;
   fontSTPtr->pixAryUC[40][8] = 63;
   fontSTPtr->pixAryUC[40][9] = 12;
   fontSTPtr->pixAryUC[40][10] = 195;
   fontSTPtr->pixAryUC[40][11] = 48;
   fontSTPtr->pixAryUC[40][12] = 158;
   fontSTPtr->pixAryUC[40][13] = 7;
   fontSTPtr->pixAryUC[40][14] = 0;
   fontSTPtr->pixAryUC[40][15] = 0;
   fontSTPtr->pixAryUC[40][16] = 0;
   fontSTPtr->pixAryUC[40][17] = 0;

   return 0;
} /*addAscii72_smallFont*/

/*___________________________________________
` Fun41: addAscii73_smallFont
`  - adds character `I` (ascii 73) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii73_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[41])
      free(fontSTPtr->pixAryUC[41]);

   fontSTPtr->pixAryUC[41] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[41])
      return 1;

   fontSTPtr->widthArySS[41] = 12;
   fontSTPtr->lenArySS[41] = 18;

   fontSTPtr->pixAryUC[41][0] = 0;
   fontSTPtr->pixAryUC[41][1] = 224;
   fontSTPtr->pixAryUC[41][2] = 127;
   fontSTPtr->pixAryUC[41][3] = 252;
   fontSTPtr->pixAryUC[41][4] = 3;
   fontSTPtr->pixAryUC[41][5] = 6;
   fontSTPtr->pixAryUC[41][6] = 96;
   fontSTPtr->pixAryUC[41][7] = 0;
   fontSTPtr->pixAryUC[41][8] = 6;
   fontSTPtr->pixAryUC[41][9] = 96;
   fontSTPtr->pixAryUC[41][10] = 192;
   fontSTPtr->pixAryUC[41][11] = 63;
   fontSTPtr->pixAryUC[41][12] = 254;
   fontSTPtr->pixAryUC[41][13] = 7;
   fontSTPtr->pixAryUC[41][14] = 0;
   fontSTPtr->pixAryUC[41][15] = 0;
   fontSTPtr->pixAryUC[41][16] = 0;
   fontSTPtr->pixAryUC[41][17] = 0;

   return 0;
} /*addAscii73_smallFont*/

/*___________________________________________
` Fun42: addAscii74_smallFont
`  - adds character `J` (ascii 74) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii74_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[42])
      free(fontSTPtr->pixAryUC[42]);

   fontSTPtr->pixAryUC[42] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[42])
      return 1;

   fontSTPtr->widthArySS[42] = 12;
   fontSTPtr->lenArySS[42] = 18;

   fontSTPtr->pixAryUC[42][0] = 0;
   fontSTPtr->pixAryUC[42][1] = 0;
   fontSTPtr->pixAryUC[42][2] = 127;
   fontSTPtr->pixAryUC[42][3] = 240;
   fontSTPtr->pixAryUC[42][4] = 7;
   fontSTPtr->pixAryUC[42][5] = 24;
   fontSTPtr->pixAryUC[42][6] = 128;
   fontSTPtr->pixAryUC[42][7] = 1;
   fontSTPtr->pixAryUC[42][8] = 24;
   fontSTPtr->pixAryUC[42][9] = 198;
   fontSTPtr->pixAryUC[42][10] = 225;
   fontSTPtr->pixAryUC[42][11] = 31;
   fontSTPtr->pixAryUC[42][12] = 252;
   fontSTPtr->pixAryUC[42][13] = 0;
   fontSTPtr->pixAryUC[42][14] = 0;
   fontSTPtr->pixAryUC[42][15] = 0;
   fontSTPtr->pixAryUC[42][16] = 0;
   fontSTPtr->pixAryUC[42][17] = 0;

   return 0;
} /*addAscii74_smallFont*/

/*___________________________________________
` Fun43: addAscii75_smallFont
`  - adds character `K` (ascii 75) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii75_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[43])
      free(fontSTPtr->pixAryUC[43]);

   fontSTPtr->pixAryUC[43] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[43])
      return 1;

   fontSTPtr->widthArySS[43] = 12;
   fontSTPtr->lenArySS[43] = 18;

   fontSTPtr->pixAryUC[43][0] = 0;
   fontSTPtr->pixAryUC[43][1] = 224;
   fontSTPtr->pixAryUC[43][2] = 103;
   fontSTPtr->pixAryUC[43][3] = 60;
   fontSTPtr->pixAryUC[43][4] = 131;
   fontSTPtr->pixAryUC[43][5] = 25;
   fontSTPtr->pixAryUC[43][6] = 248;
   fontSTPtr->pixAryUC[43][7] = 128;
   fontSTPtr->pixAryUC[43][8] = 15;
   fontSTPtr->pixAryUC[43][9] = 152;
   fontSTPtr->pixAryUC[43][10] = 193;
   fontSTPtr->pixAryUC[43][11] = 51;
   fontSTPtr->pixAryUC[43][12] = 126;
   fontSTPtr->pixAryUC[43][13] = 6;
   fontSTPtr->pixAryUC[43][14] = 0;
   fontSTPtr->pixAryUC[43][15] = 0;
   fontSTPtr->pixAryUC[43][16] = 0;
   fontSTPtr->pixAryUC[43][17] = 0;

   return 0;
} /*addAscii75_smallFont*/

/*___________________________________________
` Fun44: addAscii76_smallFont
`  - adds character `L` (ascii 76) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii76_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[44])
      free(fontSTPtr->pixAryUC[44]);

   fontSTPtr->pixAryUC[44] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[44])
      return 1;

   fontSTPtr->widthArySS[44] = 12;
   fontSTPtr->lenArySS[44] = 18;

   fontSTPtr->pixAryUC[44][0] = 0;
   fontSTPtr->pixAryUC[44][1] = 224;
   fontSTPtr->pixAryUC[44][2] = 15;
   fontSTPtr->pixAryUC[44][3] = 124;
   fontSTPtr->pixAryUC[44][4] = 128;
   fontSTPtr->pixAryUC[44][5] = 1;
   fontSTPtr->pixAryUC[44][6] = 24;
   fontSTPtr->pixAryUC[44][7] = 128;
   fontSTPtr->pixAryUC[44][8] = 97;
   fontSTPtr->pixAryUC[44][9] = 24;
   fontSTPtr->pixAryUC[44][10] = 199;
   fontSTPtr->pixAryUC[44][11] = 127;
   fontSTPtr->pixAryUC[44][12] = 254;
   fontSTPtr->pixAryUC[44][13] = 7;
   fontSTPtr->pixAryUC[44][14] = 0;
   fontSTPtr->pixAryUC[44][15] = 0;
   fontSTPtr->pixAryUC[44][16] = 0;
   fontSTPtr->pixAryUC[44][17] = 0;

   return 0;
} /*addAscii76_smallFont*/

/*___________________________________________
` Fun45: addAscii77_smallFont
`  - adds character `M` (ascii 77) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii77_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[45])
      free(fontSTPtr->pixAryUC[45]);

   fontSTPtr->pixAryUC[45] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[45])
      return 1;

   fontSTPtr->widthArySS[45] = 12;
   fontSTPtr->lenArySS[45] = 18;

   fontSTPtr->pixAryUC[45][0] = 0;
   fontSTPtr->pixAryUC[45][1] = 192;
   fontSTPtr->pixAryUC[45][2] = 48;
   fontSTPtr->pixAryUC[45][3] = 156;
   fontSTPtr->pixAryUC[45][4] = 195;
   fontSTPtr->pixAryUC[45][5] = 63;
   fontSTPtr->pixAryUC[45][6] = 108;
   fontSTPtr->pixAryUC[45][7] = 195;
   fontSTPtr->pixAryUC[45][8] = 48;
   fontSTPtr->pixAryUC[45][9] = 12;
   fontSTPtr->pixAryUC[45][10] = 195;
   fontSTPtr->pixAryUC[45][11] = 48;
   fontSTPtr->pixAryUC[45][12] = 158;
   fontSTPtr->pixAryUC[45][13] = 7;
   fontSTPtr->pixAryUC[45][14] = 0;
   fontSTPtr->pixAryUC[45][15] = 0;
   fontSTPtr->pixAryUC[45][16] = 0;
   fontSTPtr->pixAryUC[45][17] = 0;

   return 0;
} /*addAscii77_smallFont*/

/*___________________________________________
` Fun46: addAscii78_smallFont
`  - adds character `N` (ascii 78) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii78_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[46])
      free(fontSTPtr->pixAryUC[46]);

   fontSTPtr->pixAryUC[46] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[46])
      return 1;

   fontSTPtr->widthArySS[46] = 12;
   fontSTPtr->lenArySS[46] = 18;

   fontSTPtr->pixAryUC[46][0] = 0;
   fontSTPtr->pixAryUC[46][1] = 96;
   fontSTPtr->pixAryUC[46][2] = 96;
   fontSTPtr->pixAryUC[46][3] = 14;
   fontSTPtr->pixAryUC[46][4] = 230;
   fontSTPtr->pixAryUC[46][5] = 97;
   fontSTPtr->pixAryUC[46][6] = 54;
   fontSTPtr->pixAryUC[46][7] = 102;
   fontSTPtr->pixAryUC[46][8] = 102;
   fontSTPtr->pixAryUC[46][9] = 198;
   fontSTPtr->pixAryUC[46][10] = 102;
   fontSTPtr->pixAryUC[46][11] = 120;
   fontSTPtr->pixAryUC[46][12] = 6;
   fontSTPtr->pixAryUC[46][13] = 7;
   fontSTPtr->pixAryUC[46][14] = 0;
   fontSTPtr->pixAryUC[46][15] = 0;
   fontSTPtr->pixAryUC[46][16] = 0;
   fontSTPtr->pixAryUC[46][17] = 0;

   return 0;
} /*addAscii78_smallFont*/

/*___________________________________________
` Fun47: addAscii79_smallFont
`  - adds character `O` (ascii 79) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii79_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[47])
      free(fontSTPtr->pixAryUC[47]);

   fontSTPtr->pixAryUC[47] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[47])
      return 1;

   fontSTPtr->widthArySS[47] = 12;
   fontSTPtr->lenArySS[47] = 18;

   fontSTPtr->pixAryUC[47][0] = 0;
   fontSTPtr->pixAryUC[47][1] = 0;
   fontSTPtr->pixAryUC[47][2] = 15;
   fontSTPtr->pixAryUC[47][3] = 248;
   fontSTPtr->pixAryUC[47][4] = 193;
   fontSTPtr->pixAryUC[47][5] = 48;
   fontSTPtr->pixAryUC[47][6] = 6;
   fontSTPtr->pixAryUC[47][7] = 102;
   fontSTPtr->pixAryUC[47][8] = 96;
   fontSTPtr->pixAryUC[47][9] = 12;
   fontSTPtr->pixAryUC[47][10] = 131;
   fontSTPtr->pixAryUC[47][11] = 31;
   fontSTPtr->pixAryUC[47][12] = 240;
   fontSTPtr->pixAryUC[47][13] = 0;
   fontSTPtr->pixAryUC[47][14] = 0;
   fontSTPtr->pixAryUC[47][15] = 0;
   fontSTPtr->pixAryUC[47][16] = 0;
   fontSTPtr->pixAryUC[47][17] = 0;

   return 0;
} /*addAscii79_smallFont*/

/*___________________________________________
` Fun48: addAscii80_smallFont
`  - adds character `P` (ascii 80) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii80_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[48])
      free(fontSTPtr->pixAryUC[48]);

   fontSTPtr->pixAryUC[48] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[48])
      return 1;

   fontSTPtr->widthArySS[48] = 12;
   fontSTPtr->lenArySS[48] = 18;

   fontSTPtr->pixAryUC[48][0] = 0;
   fontSTPtr->pixAryUC[48][1] = 128;
   fontSTPtr->pixAryUC[48][2] = 31;
   fontSTPtr->pixAryUC[48][3] = 248;
   fontSTPtr->pixAryUC[48][4] = 131;
   fontSTPtr->pixAryUC[48][5] = 97;
   fontSTPtr->pixAryUC[48][6] = 248;
   fontSTPtr->pixAryUC[48][7] = 131;
   fontSTPtr->pixAryUC[48][8] = 31;
   fontSTPtr->pixAryUC[48][9] = 24;
   fontSTPtr->pixAryUC[48][10] = 128;
   fontSTPtr->pixAryUC[48][11] = 1;
   fontSTPtr->pixAryUC[48][12] = 254;
   fontSTPtr->pixAryUC[48][13] = 0;
   fontSTPtr->pixAryUC[48][14] = 0;
   fontSTPtr->pixAryUC[48][15] = 0;
   fontSTPtr->pixAryUC[48][16] = 0;
   fontSTPtr->pixAryUC[48][17] = 0;

   return 0;
} /*addAscii80_smallFont*/

/*___________________________________________
` Fun49: addAscii81_smallFont
`  - adds character `Q` (ascii 81) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii81_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[49])
      free(fontSTPtr->pixAryUC[49]);

   fontSTPtr->pixAryUC[49] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[49])
      return 1;

   fontSTPtr->widthArySS[49] = 12;
   fontSTPtr->lenArySS[49] = 18;

   fontSTPtr->pixAryUC[49][0] = 0;
   fontSTPtr->pixAryUC[49][1] = 0;
   fontSTPtr->pixAryUC[49][2] = 7;
   fontSTPtr->pixAryUC[49][3] = 248;
   fontSTPtr->pixAryUC[49][4] = 192;
   fontSTPtr->pixAryUC[49][5] = 24;
   fontSTPtr->pixAryUC[49][6] = 6;
   fontSTPtr->pixAryUC[49][7] = 35;
   fontSTPtr->pixAryUC[49][8] = 32;
   fontSTPtr->pixAryUC[49][9] = 118;
   fontSTPtr->pixAryUC[49][10] = 195;
   fontSTPtr->pixAryUC[49][11] = 28;
   fontSTPtr->pixAryUC[49][12] = 248;
   fontSTPtr->pixAryUC[49][13] = 1;
   fontSTPtr->pixAryUC[49][14] = 119;
   fontSTPtr->pixAryUC[49][15] = 0;
   fontSTPtr->pixAryUC[49][16] = 6;
   fontSTPtr->pixAryUC[49][17] = 0;

   return 0;
} /*addAscii81_smallFont*/

/*___________________________________________
` Fun50: addAscii82_smallFont
`  - adds character `R` (ascii 82) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii82_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[50])
      free(fontSTPtr->pixAryUC[50]);

   fontSTPtr->pixAryUC[50] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[50])
      return 1;

   fontSTPtr->widthArySS[50] = 12;
   fontSTPtr->lenArySS[50] = 18;

   fontSTPtr->pixAryUC[50][0] = 0;
   fontSTPtr->pixAryUC[50][1] = 224;
   fontSTPtr->pixAryUC[50][2] = 15;
   fontSTPtr->pixAryUC[50][3] = 254;
   fontSTPtr->pixAryUC[50][4] = 97;
   fontSTPtr->pixAryUC[50][5] = 48;
   fontSTPtr->pixAryUC[50][6] = 254;
   fontSTPtr->pixAryUC[50][7] = 225;
   fontSTPtr->pixAryUC[50][8] = 15;
   fontSTPtr->pixAryUC[50][9] = 198;
   fontSTPtr->pixAryUC[50][10] = 96;
   fontSTPtr->pixAryUC[50][11] = 24;
   fontSTPtr->pixAryUC[50][12] = 198;
   fontSTPtr->pixAryUC[50][13] = 3;
   fontSTPtr->pixAryUC[50][14] = 0;
   fontSTPtr->pixAryUC[50][15] = 0;
   fontSTPtr->pixAryUC[50][16] = 0;
   fontSTPtr->pixAryUC[50][17] = 0;

   return 0;
} /*addAscii82_smallFont*/

/*___________________________________________
` Fun51: addAscii83_smallFont
`  - adds character `S` (ascii 83) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii83_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[51])
      free(fontSTPtr->pixAryUC[51]);

   fontSTPtr->pixAryUC[51] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[51])
      return 1;

   fontSTPtr->widthArySS[51] = 12;
   fontSTPtr->lenArySS[51] = 18;

   fontSTPtr->pixAryUC[51][0] = 0;
   fontSTPtr->pixAryUC[51][1] = 128;
   fontSTPtr->pixAryUC[51][2] = 63;
   fontSTPtr->pixAryUC[51][3] = 252;
   fontSTPtr->pixAryUC[51][4] = 199;
   fontSTPtr->pixAryUC[51][5] = 0;
   fontSTPtr->pixAryUC[51][6] = 252;
   fontSTPtr->pixAryUC[51][7] = 131;
   fontSTPtr->pixAryUC[51][8] = 127;
   fontSTPtr->pixAryUC[51][9] = 0;
   fontSTPtr->pixAryUC[51][10] = 198;
   fontSTPtr->pixAryUC[51][11] = 127;
   fontSTPtr->pixAryUC[51][12] = 248;
   fontSTPtr->pixAryUC[51][13] = 3;
   fontSTPtr->pixAryUC[51][14] = 0;
   fontSTPtr->pixAryUC[51][15] = 0;
   fontSTPtr->pixAryUC[51][16] = 0;
   fontSTPtr->pixAryUC[51][17] = 0;

   return 0;
} /*addAscii83_smallFont*/

/*___________________________________________
` Fun52: addAscii84_smallFont
`  - adds character `T` (ascii 84) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii84_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[52])
      free(fontSTPtr->pixAryUC[52]);

   fontSTPtr->pixAryUC[52] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[52])
      return 1;

   fontSTPtr->widthArySS[52] = 12;
   fontSTPtr->lenArySS[52] = 18;

   fontSTPtr->pixAryUC[52][0] = 0;
   fontSTPtr->pixAryUC[52][1] = 224;
   fontSTPtr->pixAryUC[52][2] = 127;
   fontSTPtr->pixAryUC[52][3] = 254;
   fontSTPtr->pixAryUC[52][4] = 7;
   fontSTPtr->pixAryUC[52][5] = 6;
   fontSTPtr->pixAryUC[52][6] = 96;
   fontSTPtr->pixAryUC[52][7] = 0;
   fontSTPtr->pixAryUC[52][8] = 6;
   fontSTPtr->pixAryUC[52][9] = 96;
   fontSTPtr->pixAryUC[52][10] = 0;
   fontSTPtr->pixAryUC[52][11] = 6;
   fontSTPtr->pixAryUC[52][12] = 96;
   fontSTPtr->pixAryUC[52][13] = 0;
   fontSTPtr->pixAryUC[52][14] = 0;
   fontSTPtr->pixAryUC[52][15] = 0;
   fontSTPtr->pixAryUC[52][16] = 0;
   fontSTPtr->pixAryUC[52][17] = 0;

   return 0;
} /*addAscii84_smallFont*/

/*___________________________________________
` Fun53: addAscii85_smallFont
`  - adds character `U` (ascii 85) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii85_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[53])
      free(fontSTPtr->pixAryUC[53]);

   fontSTPtr->pixAryUC[53] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[53])
      return 1;

   fontSTPtr->widthArySS[53] = 12;
   fontSTPtr->lenArySS[53] = 18;

   fontSTPtr->pixAryUC[53][0] = 0;
   fontSTPtr->pixAryUC[53][1] = 96;
   fontSTPtr->pixAryUC[53][2] = 96;
   fontSTPtr->pixAryUC[53][3] = 6;
   fontSTPtr->pixAryUC[53][4] = 102;
   fontSTPtr->pixAryUC[53][5] = 96;
   fontSTPtr->pixAryUC[53][6] = 6;
   fontSTPtr->pixAryUC[53][7] = 102;
   fontSTPtr->pixAryUC[53][8] = 96;
   fontSTPtr->pixAryUC[53][9] = 14;
   fontSTPtr->pixAryUC[53][10] = 199;
   fontSTPtr->pixAryUC[53][11] = 63;
   fontSTPtr->pixAryUC[53][12] = 248;
   fontSTPtr->pixAryUC[53][13] = 1;
   fontSTPtr->pixAryUC[53][14] = 0;
   fontSTPtr->pixAryUC[53][15] = 0;
   fontSTPtr->pixAryUC[53][16] = 0;
   fontSTPtr->pixAryUC[53][17] = 0;

   return 0;
} /*addAscii85_smallFont*/

/*___________________________________________
` Fun54: addAscii86_smallFont
`  - adds character `V` (ascii 86) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii86_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[54])
      free(fontSTPtr->pixAryUC[54]);

   fontSTPtr->pixAryUC[54] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[54])
      return 1;

   fontSTPtr->widthArySS[54] = 12;
   fontSTPtr->lenArySS[54] = 18;

   fontSTPtr->pixAryUC[54][0] = 0;
   fontSTPtr->pixAryUC[54][1] = 96;
   fontSTPtr->pixAryUC[54][2] = 48;
   fontSTPtr->pixAryUC[54][3] = 6;
   fontSTPtr->pixAryUC[54][4] = 99;
   fontSTPtr->pixAryUC[54][5] = 48;
   fontSTPtr->pixAryUC[54][6] = 142;
   fontSTPtr->pixAryUC[54][7] = 195;
   fontSTPtr->pixAryUC[54][8] = 24;
   fontSTPtr->pixAryUC[54][9] = 216;
   fontSTPtr->pixAryUC[54][10] = 0;
   fontSTPtr->pixAryUC[54][11] = 7;
   fontSTPtr->pixAryUC[54][12] = 32;
   fontSTPtr->pixAryUC[54][13] = 0;
   fontSTPtr->pixAryUC[54][14] = 0;
   fontSTPtr->pixAryUC[54][15] = 0;
   fontSTPtr->pixAryUC[54][16] = 0;
   fontSTPtr->pixAryUC[54][17] = 0;

   return 0;
} /*addAscii86_smallFont*/

/*___________________________________________
` Fun55: addAscii87_smallFont
`  - adds character `W` (ascii 87) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii87_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[55])
      free(fontSTPtr->pixAryUC[55]);

   fontSTPtr->pixAryUC[55] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[55])
      return 1;

   fontSTPtr->widthArySS[55] = 12;
   fontSTPtr->lenArySS[55] = 18;

   fontSTPtr->pixAryUC[55][0] = 0;
   fontSTPtr->pixAryUC[55][1] = 192;
   fontSTPtr->pixAryUC[55][2] = 96;
   fontSTPtr->pixAryUC[55][3] = 12;
   fontSTPtr->pixAryUC[55][4] = 198;
   fontSTPtr->pixAryUC[55][5] = 100;
   fontSTPtr->pixAryUC[55][6] = 236;
   fontSTPtr->pixAryUC[55][7] = 198;
   fontSTPtr->pixAryUC[55][8] = 127;
   fontSTPtr->pixAryUC[55][9] = 188;
   fontSTPtr->pixAryUC[55][10] = 199;
   fontSTPtr->pixAryUC[55][11] = 113;
   fontSTPtr->pixAryUC[55][12] = 12;
   fontSTPtr->pixAryUC[55][13] = 6;
   fontSTPtr->pixAryUC[55][14] = 0;
   fontSTPtr->pixAryUC[55][15] = 0;
   fontSTPtr->pixAryUC[55][16] = 0;
   fontSTPtr->pixAryUC[55][17] = 0;

   return 0;
} /*addAscii87_smallFont*/

/*___________________________________________
` Fun56: addAscii88_smallFont
`  - adds character `X` (ascii 88) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii88_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[56])
      free(fontSTPtr->pixAryUC[56]);

   fontSTPtr->pixAryUC[56] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[56])
      return 1;

   fontSTPtr->widthArySS[56] = 12;
   fontSTPtr->lenArySS[56] = 18;

   fontSTPtr->pixAryUC[56][0] = 0;
   fontSTPtr->pixAryUC[56][1] = 224;
   fontSTPtr->pixAryUC[56][2] = 121;
   fontSTPtr->pixAryUC[56][3] = 24;
   fontSTPtr->pixAryUC[56][4] = 3;
   fontSTPtr->pixAryUC[56][5] = 27;
   fontSTPtr->pixAryUC[56][6] = 224;
   fontSTPtr->pixAryUC[56][7] = 0;
   fontSTPtr->pixAryUC[56][8] = 14;
   fontSTPtr->pixAryUC[56][9] = 176;
   fontSTPtr->pixAryUC[56][10] = 129;
   fontSTPtr->pixAryUC[56][11] = 49;
   fontSTPtr->pixAryUC[56][12] = 158;
   fontSTPtr->pixAryUC[56][13] = 7;
   fontSTPtr->pixAryUC[56][14] = 0;
   fontSTPtr->pixAryUC[56][15] = 0;
   fontSTPtr->pixAryUC[56][16] = 0;
   fontSTPtr->pixAryUC[56][17] = 0;

   return 0;
} /*addAscii88_smallFont*/

/*___________________________________________
` Fun57: addAscii89_smallFont
`  - adds character `Y` (ascii 89) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii89_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[57])
      free(fontSTPtr->pixAryUC[57]);

   fontSTPtr->pixAryUC[57] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[57])
      return 1;

   fontSTPtr->widthArySS[57] = 12;
   fontSTPtr->lenArySS[57] = 18;

   fontSTPtr->pixAryUC[57][0] = 0;
   fontSTPtr->pixAryUC[57][1] = 224;
   fontSTPtr->pixAryUC[57][2] = 121;
   fontSTPtr->pixAryUC[57][3] = 12;
   fontSTPtr->pixAryUC[57][4] = 131;
   fontSTPtr->pixAryUC[57][5] = 25;
   fontSTPtr->pixAryUC[57][6] = 240;
   fontSTPtr->pixAryUC[57][7] = 0;
   fontSTPtr->pixAryUC[57][8] = 6;
   fontSTPtr->pixAryUC[57][9] = 96;
   fontSTPtr->pixAryUC[57][10] = 0;
   fontSTPtr->pixAryUC[57][11] = 6;
   fontSTPtr->pixAryUC[57][12] = 240;
   fontSTPtr->pixAryUC[57][13] = 0;
   fontSTPtr->pixAryUC[57][14] = 0;
   fontSTPtr->pixAryUC[57][15] = 0;
   fontSTPtr->pixAryUC[57][16] = 0;
   fontSTPtr->pixAryUC[57][17] = 0;

   return 0;
} /*addAscii89_smallFont*/

/*___________________________________________
` Fun58: addAscii90_smallFont
`  - adds character `Z` (ascii 90) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii90_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[58])
      free(fontSTPtr->pixAryUC[58]);

   fontSTPtr->pixAryUC[58] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[58])
      return 1;

   fontSTPtr->widthArySS[58] = 12;
   fontSTPtr->lenArySS[58] = 18;

   fontSTPtr->pixAryUC[58][0] = 0;
   fontSTPtr->pixAryUC[58][1] = 224;
   fontSTPtr->pixAryUC[58][2] = 127;
   fontSTPtr->pixAryUC[58][3] = 254;
   fontSTPtr->pixAryUC[58][4] = 3;
   fontSTPtr->pixAryUC[58][5] = 24;
   fontSTPtr->pixAryUC[58][6] = 222;
   fontSTPtr->pixAryUC[58][7] = 230;
   fontSTPtr->pixAryUC[58][8] = 118;
   fontSTPtr->pixAryUC[58][9] = 48;
   fontSTPtr->pixAryUC[58][10] = 192;
   fontSTPtr->pixAryUC[58][11] = 127;
   fontSTPtr->pixAryUC[58][12] = 254;
   fontSTPtr->pixAryUC[58][13] = 7;
   fontSTPtr->pixAryUC[58][14] = 0;
   fontSTPtr->pixAryUC[58][15] = 0;
   fontSTPtr->pixAryUC[58][16] = 0;
   fontSTPtr->pixAryUC[58][17] = 0;

   return 0;
} /*addAscii90_smallFont*/

/*___________________________________________
` Fun59: addAscii91_smallFont
`  - adds character `[` (ascii 91) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii91_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[59])
      free(fontSTPtr->pixAryUC[59]);

   fontSTPtr->pixAryUC[59] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[59])
      return 1;

   fontSTPtr->widthArySS[59] = 12;
   fontSTPtr->lenArySS[59] = 18;

   fontSTPtr->pixAryUC[59][0] = 0;
   fontSTPtr->pixAryUC[59][1] = 192;
   fontSTPtr->pixAryUC[59][2] = 31;
   fontSTPtr->pixAryUC[59][3] = 252;
   fontSTPtr->pixAryUC[59][4] = 193;
   fontSTPtr->pixAryUC[59][5] = 0;
   fontSTPtr->pixAryUC[59][6] = 12;
   fontSTPtr->pixAryUC[59][7] = 192;
   fontSTPtr->pixAryUC[59][8] = 0;
   fontSTPtr->pixAryUC[59][9] = 12;
   fontSTPtr->pixAryUC[59][10] = 192;
   fontSTPtr->pixAryUC[59][11] = 31;
   fontSTPtr->pixAryUC[59][12] = 252;
   fontSTPtr->pixAryUC[59][13] = 1;
   fontSTPtr->pixAryUC[59][14] = 0;
   fontSTPtr->pixAryUC[59][15] = 0;
   fontSTPtr->pixAryUC[59][16] = 0;
   fontSTPtr->pixAryUC[59][17] = 0;

   return 0;
} /*addAscii91_smallFont*/

/*___________________________________________
` Fun60: addAscii92_smallFont
`  - adds character `\` (ascii 92) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii92_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[60])
      free(fontSTPtr->pixAryUC[60]);

   fontSTPtr->pixAryUC[60] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[60])
      return 1;

   fontSTPtr->widthArySS[60] = 12;
   fontSTPtr->lenArySS[60] = 18;

   fontSTPtr->pixAryUC[60][0] = 0;
   fontSTPtr->pixAryUC[60][1] = 96;
   fontSTPtr->pixAryUC[60][2] = 0;
   fontSTPtr->pixAryUC[60][3] = 12;
   fontSTPtr->pixAryUC[60][4] = 128;
   fontSTPtr->pixAryUC[60][5] = 1;
   fontSTPtr->pixAryUC[60][6] = 48;
   fontSTPtr->pixAryUC[60][7] = 0;
   fontSTPtr->pixAryUC[60][8] = 6;
   fontSTPtr->pixAryUC[60][9] = 192;
   fontSTPtr->pixAryUC[60][10] = 0;
   fontSTPtr->pixAryUC[60][11] = 24;
   fontSTPtr->pixAryUC[60][12] = 0;
   fontSTPtr->pixAryUC[60][13] = 3;
   fontSTPtr->pixAryUC[60][14] = 0;
   fontSTPtr->pixAryUC[60][15] = 0;
   fontSTPtr->pixAryUC[60][16] = 0;
   fontSTPtr->pixAryUC[60][17] = 0;

   return 0;
} /*addAscii92_smallFont*/

/*___________________________________________
` Fun61: addAscii93_smallFont
`  - adds character `]` (ascii 93) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii93_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[61])
      free(fontSTPtr->pixAryUC[61]);

   fontSTPtr->pixAryUC[61] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[61])
      return 1;

   fontSTPtr->widthArySS[61] = 12;
   fontSTPtr->lenArySS[61] = 18;

   fontSTPtr->pixAryUC[61][0] = 0;
   fontSTPtr->pixAryUC[61][1] = 128;
   fontSTPtr->pixAryUC[61][2] = 63;
   fontSTPtr->pixAryUC[61][3] = 248;
   fontSTPtr->pixAryUC[61][4] = 3;
   fontSTPtr->pixAryUC[61][5] = 48;
   fontSTPtr->pixAryUC[61][6] = 0;
   fontSTPtr->pixAryUC[61][7] = 3;
   fontSTPtr->pixAryUC[61][8] = 48;
   fontSTPtr->pixAryUC[61][9] = 0;
   fontSTPtr->pixAryUC[61][10] = 131;
   fontSTPtr->pixAryUC[61][11] = 63;
   fontSTPtr->pixAryUC[61][12] = 248;
   fontSTPtr->pixAryUC[61][13] = 3;
   fontSTPtr->pixAryUC[61][14] = 0;
   fontSTPtr->pixAryUC[61][15] = 0;
   fontSTPtr->pixAryUC[61][16] = 0;
   fontSTPtr->pixAryUC[61][17] = 0;

   return 0;
} /*addAscii93_smallFont*/

/*___________________________________________
` Fun62: addAscii94_smallFont
`  - adds character `^` (ascii 94) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii94_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[62])
      free(fontSTPtr->pixAryUC[62]);

   fontSTPtr->pixAryUC[62] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[62])
      return 1;

   fontSTPtr->widthArySS[62] = 12;
   fontSTPtr->lenArySS[62] = 18;

   fontSTPtr->pixAryUC[62][0] = 0;
   fontSTPtr->pixAryUC[62][1] = 0;
   fontSTPtr->pixAryUC[62][2] = 2;
   fontSTPtr->pixAryUC[62][3] = 112;
   fontSTPtr->pixAryUC[62][4] = 128;
   fontSTPtr->pixAryUC[62][5] = 13;
   fontSTPtr->pixAryUC[62][6] = 140;
   fontSTPtr->pixAryUC[62][7] = 1;
   fontSTPtr->pixAryUC[62][8] = 0;
   fontSTPtr->pixAryUC[62][9] = 0;
   fontSTPtr->pixAryUC[62][10] = 0;
   fontSTPtr->pixAryUC[62][11] = 0;
   fontSTPtr->pixAryUC[62][12] = 0;
   fontSTPtr->pixAryUC[62][13] = 0;
   fontSTPtr->pixAryUC[62][14] = 0;
   fontSTPtr->pixAryUC[62][15] = 0;
   fontSTPtr->pixAryUC[62][16] = 0;
   fontSTPtr->pixAryUC[62][17] = 0;

   return 0;
} /*addAscii94_smallFont*/

/*___________________________________________
` Fun63: addAscii95_smallFont
`  - adds character `_` (ascii 95) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii95_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[63])
      free(fontSTPtr->pixAryUC[63]);

   fontSTPtr->pixAryUC[63] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[63])
      return 1;

   fontSTPtr->widthArySS[63] = 12;
   fontSTPtr->lenArySS[63] = 18;

   fontSTPtr->pixAryUC[63][0] = 0;
   fontSTPtr->pixAryUC[63][1] = 0;
   fontSTPtr->pixAryUC[63][2] = 0;
   fontSTPtr->pixAryUC[63][3] = 0;
   fontSTPtr->pixAryUC[63][4] = 0;
   fontSTPtr->pixAryUC[63][5] = 0;
   fontSTPtr->pixAryUC[63][6] = 0;
   fontSTPtr->pixAryUC[63][7] = 0;
   fontSTPtr->pixAryUC[63][8] = 0;
   fontSTPtr->pixAryUC[63][9] = 0;
   fontSTPtr->pixAryUC[63][10] = 0;
   fontSTPtr->pixAryUC[63][11] = 0;
   fontSTPtr->pixAryUC[63][12] = 0;
   fontSTPtr->pixAryUC[63][13] = 0;
   fontSTPtr->pixAryUC[63][14] = 0;
   fontSTPtr->pixAryUC[63][15] = 255;
   fontSTPtr->pixAryUC[63][16] = 243;
   fontSTPtr->pixAryUC[63][17] = 63;

   return 0;
} /*addAscii95_smallFont*/

/*___________________________________________
` Fun64: addAscii96_smallFont
`  - adds character ``` (ascii 96) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii96_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[64])
      free(fontSTPtr->pixAryUC[64]);

   fontSTPtr->pixAryUC[64] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[64])
      return 1;

   fontSTPtr->widthArySS[64] = 12;
   fontSTPtr->lenArySS[64] = 18;

   fontSTPtr->pixAryUC[64][0] = 0;
   fontSTPtr->pixAryUC[64][1] = 0;
   fontSTPtr->pixAryUC[64][2] = 6;
   fontSTPtr->pixAryUC[64][3] = 192;
   fontSTPtr->pixAryUC[64][4] = 0;
   fontSTPtr->pixAryUC[64][5] = 24;
   fontSTPtr->pixAryUC[64][6] = 128;
   fontSTPtr->pixAryUC[64][7] = 1;
   fontSTPtr->pixAryUC[64][8] = 0;
   fontSTPtr->pixAryUC[64][9] = 0;
   fontSTPtr->pixAryUC[64][10] = 0;
   fontSTPtr->pixAryUC[64][11] = 0;
   fontSTPtr->pixAryUC[64][12] = 0;
   fontSTPtr->pixAryUC[64][13] = 0;
   fontSTPtr->pixAryUC[64][14] = 0;
   fontSTPtr->pixAryUC[64][15] = 0;
   fontSTPtr->pixAryUC[64][16] = 0;
   fontSTPtr->pixAryUC[64][17] = 0;

   return 0;
} /*addAscii96_smallFont*/

/*___________________________________________
` Fun65: addAscii97_smallFont
`  - adds character `a` (ascii 97) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii97_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[65])
      free(fontSTPtr->pixAryUC[65]);

   fontSTPtr->pixAryUC[65] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[65])
      return 1;

   fontSTPtr->widthArySS[65] = 12;
   fontSTPtr->lenArySS[65] = 18;

   fontSTPtr->pixAryUC[65][0] = 0;
   fontSTPtr->pixAryUC[65][1] = 0;
   fontSTPtr->pixAryUC[65][2] = 0;
   fontSTPtr->pixAryUC[65][3] = 56;
   fontSTPtr->pixAryUC[65][4] = 192;
   fontSTPtr->pixAryUC[65][5] = 7;
   fontSTPtr->pixAryUC[65][6] = 194;
   fontSTPtr->pixAryUC[65][7] = 128;
   fontSTPtr->pixAryUC[65][8] = 15;
   fontSTPtr->pixAryUC[65][9] = 196;
   fontSTPtr->pixAryUC[65][10] = 198;
   fontSTPtr->pixAryUC[65][11] = 103;
   fontSTPtr->pixAryUC[65][12] = 216;
   fontSTPtr->pixAryUC[65][13] = 3;
   fontSTPtr->pixAryUC[65][14] = 0;
   fontSTPtr->pixAryUC[65][15] = 0;
   fontSTPtr->pixAryUC[65][16] = 0;
   fontSTPtr->pixAryUC[65][17] = 0;

   return 0;
} /*addAscii97_smallFont*/

/*___________________________________________
` Fun66: addAscii98_smallFont
`  - adds character `b` (ascii 98) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii98_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[66])
      free(fontSTPtr->pixAryUC[66]);

   fontSTPtr->pixAryUC[66] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[66])
      return 1;

   fontSTPtr->widthArySS[66] = 12;
   fontSTPtr->lenArySS[66] = 18;

   fontSTPtr->pixAryUC[66][0] = 0;
   fontSTPtr->pixAryUC[66][1] = 192;
   fontSTPtr->pixAryUC[66][2] = 1;
   fontSTPtr->pixAryUC[66][3] = 30;
   fontSTPtr->pixAryUC[66][4] = 128;
   fontSTPtr->pixAryUC[66][5] = 1;
   fontSTPtr->pixAryUC[66][6] = 216;
   fontSTPtr->pixAryUC[66][7] = 129;
   fontSTPtr->pixAryUC[66][8] = 51;
   fontSTPtr->pixAryUC[66][9] = 24;
   fontSTPtr->pixAryUC[66][10] = 134;
   fontSTPtr->pixAryUC[66][11] = 49;
   fontSTPtr->pixAryUC[66][12] = 240;
   fontSTPtr->pixAryUC[66][13] = 1;
   fontSTPtr->pixAryUC[66][14] = 0;
   fontSTPtr->pixAryUC[66][15] = 0;
   fontSTPtr->pixAryUC[66][16] = 0;
   fontSTPtr->pixAryUC[66][17] = 0;

   return 0;
} /*addAscii98_smallFont*/

/*___________________________________________
` Fun67: addAscii99_smallFont
`  - adds character `c` (ascii 99) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii99_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[67])
      free(fontSTPtr->pixAryUC[67]);

   fontSTPtr->pixAryUC[67] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[67])
      return 1;

   fontSTPtr->widthArySS[67] = 12;
   fontSTPtr->lenArySS[67] = 18;

   fontSTPtr->pixAryUC[67][0] = 0;
   fontSTPtr->pixAryUC[67][1] = 0;
   fontSTPtr->pixAryUC[67][2] = 0;
   fontSTPtr->pixAryUC[67][3] = 224;
   fontSTPtr->pixAryUC[67][4] = 1;
   fontSTPtr->pixAryUC[67][5] = 63;
   fontSTPtr->pixAryUC[67][6] = 24;
   fontSTPtr->pixAryUC[67][7] = 198;
   fontSTPtr->pixAryUC[67][8] = 0;
   fontSTPtr->pixAryUC[67][9] = 24;
   fontSTPtr->pixAryUC[67][10] = 6;
   fontSTPtr->pixAryUC[67][11] = 63;
   fontSTPtr->pixAryUC[67][12] = 224;
   fontSTPtr->pixAryUC[67][13] = 1;
   fontSTPtr->pixAryUC[67][14] = 0;
   fontSTPtr->pixAryUC[67][15] = 0;
   fontSTPtr->pixAryUC[67][16] = 0;
   fontSTPtr->pixAryUC[67][17] = 0;

   return 0;
} /*addAscii99_smallFont*/

/*___________________________________________
` Fun68: addAscii100_smallFont
`  - adds character `d` (ascii 100) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii100_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[68])
      free(fontSTPtr->pixAryUC[68]);

   fontSTPtr->pixAryUC[68] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[68])
      return 1;

   fontSTPtr->widthArySS[68] = 12;
   fontSTPtr->lenArySS[68] = 18;

   fontSTPtr->pixAryUC[68][0] = 0;
   fontSTPtr->pixAryUC[68][1] = 0;
   fontSTPtr->pixAryUC[68][2] = 12;
   fontSTPtr->pixAryUC[68][3] = 192;
   fontSTPtr->pixAryUC[68][4] = 0;
   fontSTPtr->pixAryUC[68][5] = 12;
   fontSTPtr->pixAryUC[68][6] = 252;
   fontSTPtr->pixAryUC[68][7] = 96;
   fontSTPtr->pixAryUC[68][8] = 12;
   fontSTPtr->pixAryUC[68][9] = 198;
   fontSTPtr->pixAryUC[68][10] = 192;
   fontSTPtr->pixAryUC[68][11] = 111;
   fontSTPtr->pixAryUC[68][12] = 248;
   fontSTPtr->pixAryUC[68][13] = 3;
   fontSTPtr->pixAryUC[68][14] = 0;
   fontSTPtr->pixAryUC[68][15] = 0;
   fontSTPtr->pixAryUC[68][16] = 0;
   fontSTPtr->pixAryUC[68][17] = 0;

   return 0;
} /*addAscii100_smallFont*/

/*___________________________________________
` Fun69: addAscii101_smallFont
`  - adds character `e` (ascii 101) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii101_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[69])
      free(fontSTPtr->pixAryUC[69]);

   fontSTPtr->pixAryUC[69] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[69])
      return 1;

   fontSTPtr->widthArySS[69] = 12;
   fontSTPtr->lenArySS[69] = 18;

   fontSTPtr->pixAryUC[69][0] = 0;
   fontSTPtr->pixAryUC[69][1] = 0;
   fontSTPtr->pixAryUC[69][2] = 0;
   fontSTPtr->pixAryUC[69][3] = 248;
   fontSTPtr->pixAryUC[69][4] = 193;
   fontSTPtr->pixAryUC[69][5] = 63;
   fontSTPtr->pixAryUC[69][6] = 6;
   fontSTPtr->pixAryUC[69][7] = 230;
   fontSTPtr->pixAryUC[69][8] = 127;
   fontSTPtr->pixAryUC[69][9] = 6;
   fontSTPtr->pixAryUC[69][10] = 192;
   fontSTPtr->pixAryUC[69][11] = 127;
   fontSTPtr->pixAryUC[69][12] = 248;
   fontSTPtr->pixAryUC[69][13] = 3;
   fontSTPtr->pixAryUC[69][14] = 0;
   fontSTPtr->pixAryUC[69][15] = 0;
   fontSTPtr->pixAryUC[69][16] = 0;
   fontSTPtr->pixAryUC[69][17] = 0;

   return 0;
} /*addAscii101_smallFont*/

/*___________________________________________
` Fun70: addAscii102_smallFont
`  - adds character `f` (ascii 102) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii102_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[70])
      free(fontSTPtr->pixAryUC[70]);

   fontSTPtr->pixAryUC[70] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[70])
      return 1;

   fontSTPtr->widthArySS[70] = 12;
   fontSTPtr->lenArySS[70] = 18;

   fontSTPtr->pixAryUC[70][0] = 0;
   fontSTPtr->pixAryUC[70][1] = 0;
   fontSTPtr->pixAryUC[70][2] = 28;
   fontSTPtr->pixAryUC[70][3] = 240;
   fontSTPtr->pixAryUC[70][4] = 3;
   fontSTPtr->pixAryUC[70][5] = 3;
   fontSTPtr->pixAryUC[70][6] = 254;
   fontSTPtr->pixAryUC[70][7] = 225;
   fontSTPtr->pixAryUC[70][8] = 31;
   fontSTPtr->pixAryUC[70][9] = 48;
   fontSTPtr->pixAryUC[70][10] = 0;
   fontSTPtr->pixAryUC[70][11] = 3;
   fontSTPtr->pixAryUC[70][12] = 120;
   fontSTPtr->pixAryUC[70][13] = 0;
   fontSTPtr->pixAryUC[70][14] = 0;
   fontSTPtr->pixAryUC[70][15] = 0;
   fontSTPtr->pixAryUC[70][16] = 0;
   fontSTPtr->pixAryUC[70][17] = 0;

   return 0;
} /*addAscii102_smallFont*/

/*___________________________________________
` Fun71: addAscii103_smallFont
`  - adds character `g` (ascii 103) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii103_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[71])
      free(fontSTPtr->pixAryUC[71]);

   fontSTPtr->pixAryUC[71] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[71])
      return 1;

   fontSTPtr->widthArySS[71] = 12;
   fontSTPtr->lenArySS[71] = 18;

   fontSTPtr->pixAryUC[71][0] = 0;
   fontSTPtr->pixAryUC[71][1] = 0;
   fontSTPtr->pixAryUC[71][2] = 0;
   fontSTPtr->pixAryUC[71][3] = 240;
   fontSTPtr->pixAryUC[71][4] = 131;
   fontSTPtr->pixAryUC[71][5] = 127;
   fontSTPtr->pixAryUC[71][6] = 24;
   fontSTPtr->pixAryUC[71][7] = 134;
   fontSTPtr->pixAryUC[71][8] = 97;
   fontSTPtr->pixAryUC[71][9] = 240;
   fontSTPtr->pixAryUC[71][10] = 3;
   fontSTPtr->pixAryUC[71][11] = 126;
   fontSTPtr->pixAryUC[71][12] = 0;
   fontSTPtr->pixAryUC[71][13] = 134;
   fontSTPtr->pixAryUC[71][14] = 63;
   fontSTPtr->pixAryUC[71][15] = 240;
   fontSTPtr->pixAryUC[71][16] = 1;
   fontSTPtr->pixAryUC[71][17] = 0;

   return 0;
} /*addAscii103_smallFont*/

/*___________________________________________
` Fun72: addAscii104_smallFont
`  - adds character `h` (ascii 104) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii104_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[72])
      free(fontSTPtr->pixAryUC[72]);

   fontSTPtr->pixAryUC[72] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[72])
      return 1;

   fontSTPtr->widthArySS[72] = 12;
   fontSTPtr->lenArySS[72] = 18;

   fontSTPtr->pixAryUC[72][0] = 0;
   fontSTPtr->pixAryUC[72][1] = 96;
   fontSTPtr->pixAryUC[72][2] = 0;
   fontSTPtr->pixAryUC[72][3] = 6;
   fontSTPtr->pixAryUC[72][4] = 96;
   fontSTPtr->pixAryUC[72][5] = 0;
   fontSTPtr->pixAryUC[72][6] = 118;
   fontSTPtr->pixAryUC[72][7] = 224;
   fontSTPtr->pixAryUC[72][8] = 15;
   fontSTPtr->pixAryUC[72][9] = 206;
   fontSTPtr->pixAryUC[72][10] = 96;
   fontSTPtr->pixAryUC[72][11] = 108;
   fontSTPtr->pixAryUC[72][12] = 198;
   fontSTPtr->pixAryUC[72][13] = 3;
   fontSTPtr->pixAryUC[72][14] = 0;
   fontSTPtr->pixAryUC[72][15] = 0;
   fontSTPtr->pixAryUC[72][16] = 0;
   fontSTPtr->pixAryUC[72][17] = 0;

   return 0;
} /*addAscii104_smallFont*/

/*___________________________________________
` Fun73: addAscii105_smallFont
`  - adds character `i` (ascii 105) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii105_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[73])
      free(fontSTPtr->pixAryUC[73]);

   fontSTPtr->pixAryUC[73] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[73])
      return 1;

   fontSTPtr->widthArySS[73] = 12;
   fontSTPtr->lenArySS[73] = 18;

   fontSTPtr->pixAryUC[73][0] = 0;
   fontSTPtr->pixAryUC[73][1] = 128;
   fontSTPtr->pixAryUC[73][2] = 1;
   fontSTPtr->pixAryUC[73][3] = 24;
   fontSTPtr->pixAryUC[73][4] = 0;
   fontSTPtr->pixAryUC[73][5] = 0;
   fontSTPtr->pixAryUC[73][6] = 24;
   fontSTPtr->pixAryUC[73][7] = 128;
   fontSTPtr->pixAryUC[73][8] = 1;
   fontSTPtr->pixAryUC[73][9] = 152;
   fontSTPtr->pixAryUC[73][10] = 129;
   fontSTPtr->pixAryUC[73][11] = 31;
   fontSTPtr->pixAryUC[73][12] = 240;
   fontSTPtr->pixAryUC[73][13] = 0;
   fontSTPtr->pixAryUC[73][14] = 0;
   fontSTPtr->pixAryUC[73][15] = 0;
   fontSTPtr->pixAryUC[73][16] = 0;
   fontSTPtr->pixAryUC[73][17] = 0;

   return 0;
} /*addAscii105_smallFont*/

/*___________________________________________
` Fun74: addAscii106_smallFont
`  - adds character `j` (ascii 106) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii106_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[74])
      free(fontSTPtr->pixAryUC[74]);

   fontSTPtr->pixAryUC[74] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[74])
      return 1;

   fontSTPtr->widthArySS[74] = 12;
   fontSTPtr->lenArySS[74] = 18;

   fontSTPtr->pixAryUC[74][0] = 0;
   fontSTPtr->pixAryUC[74][1] = 0;
   fontSTPtr->pixAryUC[74][2] = 24;
   fontSTPtr->pixAryUC[74][3] = 128;
   fontSTPtr->pixAryUC[74][4] = 1;
   fontSTPtr->pixAryUC[74][5] = 0;
   fontSTPtr->pixAryUC[74][6] = 128;
   fontSTPtr->pixAryUC[74][7] = 1;
   fontSTPtr->pixAryUC[74][8] = 24;
   fontSTPtr->pixAryUC[74][9] = 140;
   fontSTPtr->pixAryUC[74][10] = 129;
   fontSTPtr->pixAryUC[74][11] = 31;
   fontSTPtr->pixAryUC[74][12] = 240;
   fontSTPtr->pixAryUC[74][13] = 0;
   fontSTPtr->pixAryUC[74][14] = 0;
   fontSTPtr->pixAryUC[74][15] = 0;
   fontSTPtr->pixAryUC[74][16] = 0;
   fontSTPtr->pixAryUC[74][17] = 0;

   return 0;
} /*addAscii106_smallFont*/

/*___________________________________________
` Fun75: addAscii107_smallFont
`  - adds character `k` (ascii 107) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii107_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[75])
      free(fontSTPtr->pixAryUC[75]);

   fontSTPtr->pixAryUC[75] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[75])
      return 1;

   fontSTPtr->widthArySS[75] = 12;
   fontSTPtr->lenArySS[75] = 18;

   fontSTPtr->pixAryUC[75][0] = 0;
   fontSTPtr->pixAryUC[75][1] = 192;
   fontSTPtr->pixAryUC[75][2] = 48;
   fontSTPtr->pixAryUC[75][3] = 140;
   fontSTPtr->pixAryUC[75][4] = 193;
   fontSTPtr->pixAryUC[75][5] = 12;
   fontSTPtr->pixAryUC[75][6] = 124;
   fontSTPtr->pixAryUC[75][7] = 192;
   fontSTPtr->pixAryUC[75][8] = 7;
   fontSTPtr->pixAryUC[75][9] = 204;
   fontSTPtr->pixAryUC[75][10] = 192;
   fontSTPtr->pixAryUC[75][11] = 24;
   fontSTPtr->pixAryUC[75][12] = 12;
   fontSTPtr->pixAryUC[75][13] = 3;
   fontSTPtr->pixAryUC[75][14] = 0;
   fontSTPtr->pixAryUC[75][15] = 0;
   fontSTPtr->pixAryUC[75][16] = 0;
   fontSTPtr->pixAryUC[75][17] = 0;

   return 0;
} /*addAscii107_smallFont*/

/*___________________________________________
` Fun76: addAscii108_smallFont
`  - adds character `l` (ascii 108) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii108_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[76])
      free(fontSTPtr->pixAryUC[76]);

   fontSTPtr->pixAryUC[76] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[76])
      return 1;

   fontSTPtr->widthArySS[76] = 12;
   fontSTPtr->lenArySS[76] = 18;

   fontSTPtr->pixAryUC[76][0] = 0;
   fontSTPtr->pixAryUC[76][1] = 128;
   fontSTPtr->pixAryUC[76][2] = 1;
   fontSTPtr->pixAryUC[76][3] = 24;
   fontSTPtr->pixAryUC[76][4] = 128;
   fontSTPtr->pixAryUC[76][5] = 1;
   fontSTPtr->pixAryUC[76][6] = 24;
   fontSTPtr->pixAryUC[76][7] = 128;
   fontSTPtr->pixAryUC[76][8] = 49;
   fontSTPtr->pixAryUC[76][9] = 152;
   fontSTPtr->pixAryUC[76][10] = 131;
   fontSTPtr->pixAryUC[76][11] = 31;
   fontSTPtr->pixAryUC[76][12] = 240;
   fontSTPtr->pixAryUC[76][13] = 0;
   fontSTPtr->pixAryUC[76][14] = 0;
   fontSTPtr->pixAryUC[76][15] = 0;
   fontSTPtr->pixAryUC[76][16] = 0;
   fontSTPtr->pixAryUC[76][17] = 0;

   return 0;
} /*addAscii108_smallFont*/

/*___________________________________________
` Fun77: addAscii109_smallFont
`  - adds character `m` (ascii 109) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii109_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[77])
      free(fontSTPtr->pixAryUC[77]);

   fontSTPtr->pixAryUC[77] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[77])
      return 1;

   fontSTPtr->widthArySS[77] = 12;
   fontSTPtr->lenArySS[77] = 18;

   fontSTPtr->pixAryUC[77][0] = 0;
   fontSTPtr->pixAryUC[77][1] = 0;
   fontSTPtr->pixAryUC[77][2] = 0;
   fontSTPtr->pixAryUC[77][3] = 10;
   fontSTPtr->pixAryUC[77][4] = 227;
   fontSTPtr->pixAryUC[77][5] = 121;
   fontSTPtr->pixAryUC[77][6] = 246;
   fontSTPtr->pixAryUC[77][7] = 102;
   fontSTPtr->pixAryUC[77][8] = 102;
   fontSTPtr->pixAryUC[77][9] = 102;
   fontSTPtr->pixAryUC[77][10] = 102;
   fontSTPtr->pixAryUC[77][11] = 102;
   fontSTPtr->pixAryUC[77][12] = 102;
   fontSTPtr->pixAryUC[77][13] = 6;
   fontSTPtr->pixAryUC[77][14] = 0;
   fontSTPtr->pixAryUC[77][15] = 0;
   fontSTPtr->pixAryUC[77][16] = 0;
   fontSTPtr->pixAryUC[77][17] = 0;

   return 0;
} /*addAscii109_smallFont*/

/*___________________________________________
` Fun78: addAscii110_smallFont
`  - adds character `n` (ascii 110) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii110_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[78])
      free(fontSTPtr->pixAryUC[78]);

   fontSTPtr->pixAryUC[78] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[78])
      return 1;

   fontSTPtr->widthArySS[78] = 12;
   fontSTPtr->lenArySS[78] = 18;

   fontSTPtr->pixAryUC[78][0] = 0;
   fontSTPtr->pixAryUC[78][1] = 0;
   fontSTPtr->pixAryUC[78][2] = 0;
   fontSTPtr->pixAryUC[78][3] = 56;
   fontSTPtr->pixAryUC[78][4] = 192;
   fontSTPtr->pixAryUC[78][5] = 7;
   fontSTPtr->pixAryUC[78][6] = 238;
   fontSTPtr->pixAryUC[78][7] = 96;
   fontSTPtr->pixAryUC[78][8] = 12;
   fontSTPtr->pixAryUC[78][9] = 198;
   fontSTPtr->pixAryUC[78][10] = 96;
   fontSTPtr->pixAryUC[78][11] = 108;
   fontSTPtr->pixAryUC[78][12] = 198;
   fontSTPtr->pixAryUC[78][13] = 3;
   fontSTPtr->pixAryUC[78][14] = 0;
   fontSTPtr->pixAryUC[78][15] = 0;
   fontSTPtr->pixAryUC[78][16] = 0;
   fontSTPtr->pixAryUC[78][17] = 0;

   return 0;
} /*addAscii110_smallFont*/

/*___________________________________________
` Fun79: addAscii111_smallFont
`  - adds character `o` (ascii 111) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii111_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[79])
      free(fontSTPtr->pixAryUC[79]);

   fontSTPtr->pixAryUC[79] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[79])
      return 1;

   fontSTPtr->widthArySS[79] = 12;
   fontSTPtr->lenArySS[79] = 18;

   fontSTPtr->pixAryUC[79][0] = 0;
   fontSTPtr->pixAryUC[79][1] = 0;
   fontSTPtr->pixAryUC[79][2] = 0;
   fontSTPtr->pixAryUC[79][3] = 240;
   fontSTPtr->pixAryUC[79][4] = 128;
   fontSTPtr->pixAryUC[79][5] = 31;
   fontSTPtr->pixAryUC[79][6] = 12;
   fontSTPtr->pixAryUC[79][7] = 99;
   fontSTPtr->pixAryUC[79][8] = 96;
   fontSTPtr->pixAryUC[79][9] = 12;
   fontSTPtr->pixAryUC[79][10] = 131;
   fontSTPtr->pixAryUC[79][11] = 31;
   fontSTPtr->pixAryUC[79][12] = 240;
   fontSTPtr->pixAryUC[79][13] = 0;
   fontSTPtr->pixAryUC[79][14] = 0;
   fontSTPtr->pixAryUC[79][15] = 0;
   fontSTPtr->pixAryUC[79][16] = 0;
   fontSTPtr->pixAryUC[79][17] = 0;

   return 0;
} /*addAscii111_smallFont*/

/*___________________________________________
` Fun80: addAscii112_smallFont
`  - adds character `p` (ascii 112) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii112_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[80])
      free(fontSTPtr->pixAryUC[80]);

   fontSTPtr->pixAryUC[80] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[80])
      return 1;

   fontSTPtr->widthArySS[80] = 12;
   fontSTPtr->lenArySS[80] = 18;

   fontSTPtr->pixAryUC[80][0] = 0;
   fontSTPtr->pixAryUC[80][1] = 0;
   fontSTPtr->pixAryUC[80][2] = 0;
   fontSTPtr->pixAryUC[80][3] = 252;
   fontSTPtr->pixAryUC[80][4] = 192;
   fontSTPtr->pixAryUC[80][5] = 24;
   fontSTPtr->pixAryUC[80][6] = 140;
   fontSTPtr->pixAryUC[80][7] = 193;
   fontSTPtr->pixAryUC[80][8] = 31;
   fontSTPtr->pixAryUC[80][9] = 12;
   fontSTPtr->pixAryUC[80][10] = 192;
   fontSTPtr->pixAryUC[80][11] = 0;
   fontSTPtr->pixAryUC[80][12] = 12;
   fontSTPtr->pixAryUC[80][13] = 0;
   fontSTPtr->pixAryUC[80][14] = 0;
   fontSTPtr->pixAryUC[80][15] = 0;
   fontSTPtr->pixAryUC[80][16] = 0;
   fontSTPtr->pixAryUC[80][17] = 0;

   return 0;
} /*addAscii112_smallFont*/

/*___________________________________________
` Fun81: addAscii113_smallFont
`  - adds character `q` (ascii 113) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii113_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[81])
      free(fontSTPtr->pixAryUC[81]);

   fontSTPtr->pixAryUC[81] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[81])
      return 1;

   fontSTPtr->widthArySS[81] = 12;
   fontSTPtr->lenArySS[81] = 18;

   fontSTPtr->pixAryUC[81][0] = 0;
   fontSTPtr->pixAryUC[81][1] = 0;
   fontSTPtr->pixAryUC[81][2] = 0;
   fontSTPtr->pixAryUC[81][3] = 248;
   fontSTPtr->pixAryUC[81][4] = 193;
   fontSTPtr->pixAryUC[81][5] = 24;
   fontSTPtr->pixAryUC[81][6] = 140;
   fontSTPtr->pixAryUC[81][7] = 129;
   fontSTPtr->pixAryUC[81][8] = 31;
   fontSTPtr->pixAryUC[81][9] = 128;
   fontSTPtr->pixAryUC[81][10] = 1;
   fontSTPtr->pixAryUC[81][11] = 24;
   fontSTPtr->pixAryUC[81][12] = 128;
   fontSTPtr->pixAryUC[81][13] = 1;
   fontSTPtr->pixAryUC[81][14] = 0;
   fontSTPtr->pixAryUC[81][15] = 0;
   fontSTPtr->pixAryUC[81][16] = 0;
   fontSTPtr->pixAryUC[81][17] = 0;

   return 0;
} /*addAscii113_smallFont*/

/*___________________________________________
` Fun82: addAscii114_smallFont
`  - adds character `r` (ascii 114) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii114_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[82])
      free(fontSTPtr->pixAryUC[82]);

   fontSTPtr->pixAryUC[82] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[82])
      return 1;

   fontSTPtr->widthArySS[82] = 12;
   fontSTPtr->lenArySS[82] = 18;

   fontSTPtr->pixAryUC[82][0] = 0;
   fontSTPtr->pixAryUC[82][1] = 0;
   fontSTPtr->pixAryUC[82][2] = 0;
   fontSTPtr->pixAryUC[82][3] = 188;
   fontSTPtr->pixAryUC[82][4] = 99;
   fontSTPtr->pixAryUC[82][5] = 103;
   fontSTPtr->pixAryUC[82][6] = 50;
   fontSTPtr->pixAryUC[82][7] = 6;
   fontSTPtr->pixAryUC[82][8] = 3;
   fontSTPtr->pixAryUC[82][9] = 48;
   fontSTPtr->pixAryUC[82][10] = 0;
   fontSTPtr->pixAryUC[82][11] = 3;
   fontSTPtr->pixAryUC[82][12] = 48;
   fontSTPtr->pixAryUC[82][13] = 0;
   fontSTPtr->pixAryUC[82][14] = 0;
   fontSTPtr->pixAryUC[82][15] = 0;
   fontSTPtr->pixAryUC[82][16] = 0;
   fontSTPtr->pixAryUC[82][17] = 0;

   return 0;
} /*addAscii114_smallFont*/

/*___________________________________________
` Fun83: addAscii115_smallFont
`  - adds character `s` (ascii 115) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii115_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[83])
      free(fontSTPtr->pixAryUC[83]);

   fontSTPtr->pixAryUC[83] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[83])
      return 1;

   fontSTPtr->widthArySS[83] = 12;
   fontSTPtr->lenArySS[83] = 18;

   fontSTPtr->pixAryUC[83][0] = 0;
   fontSTPtr->pixAryUC[83][1] = 0;
   fontSTPtr->pixAryUC[83][2] = 0;
   fontSTPtr->pixAryUC[83][3] = 248;
   fontSTPtr->pixAryUC[83][4] = 231;
   fontSTPtr->pixAryUC[83][5] = 255;
   fontSTPtr->pixAryUC[83][6] = 6;
   fontSTPtr->pixAryUC[83][7] = 192;
   fontSTPtr->pixAryUC[83][8] = 63;
   fontSTPtr->pixAryUC[83][9] = 0;
   fontSTPtr->pixAryUC[83][10] = 230;
   fontSTPtr->pixAryUC[83][11] = 63;
   fontSTPtr->pixAryUC[83][12] = 252;
   fontSTPtr->pixAryUC[83][13] = 1;
   fontSTPtr->pixAryUC[83][14] = 0;
   fontSTPtr->pixAryUC[83][15] = 0;
   fontSTPtr->pixAryUC[83][16] = 0;
   fontSTPtr->pixAryUC[83][17] = 0;

   return 0;
} /*addAscii115_smallFont*/

/*___________________________________________
` Fun84: addAscii116_smallFont
`  - adds character `t` (ascii 116) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii116_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[84])
      free(fontSTPtr->pixAryUC[84]);

   fontSTPtr->pixAryUC[84] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[84])
      return 1;

   fontSTPtr->widthArySS[84] = 12;
   fontSTPtr->lenArySS[84] = 18;

   fontSTPtr->pixAryUC[84][0] = 0;
   fontSTPtr->pixAryUC[84][1] = 0;
   fontSTPtr->pixAryUC[84][2] = 6;
   fontSTPtr->pixAryUC[84][3] = 96;
   fontSTPtr->pixAryUC[84][4] = 224;
   fontSTPtr->pixAryUC[84][5] = 127;
   fontSTPtr->pixAryUC[84][6] = 254;
   fontSTPtr->pixAryUC[84][7] = 7;
   fontSTPtr->pixAryUC[84][8] = 6;
   fontSTPtr->pixAryUC[84][9] = 96;
   fontSTPtr->pixAryUC[84][10] = 6;
   fontSTPtr->pixAryUC[84][11] = 126;
   fontSTPtr->pixAryUC[84][12] = 192;
   fontSTPtr->pixAryUC[84][13] = 3;
   fontSTPtr->pixAryUC[84][14] = 0;
   fontSTPtr->pixAryUC[84][15] = 0;
   fontSTPtr->pixAryUC[84][16] = 0;
   fontSTPtr->pixAryUC[84][17] = 0;

   return 0;
} /*addAscii116_smallFont*/

/*___________________________________________
` Fun85: addAscii117_smallFont
`  - adds character `u` (ascii 117) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii117_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[85])
      free(fontSTPtr->pixAryUC[85]);

   fontSTPtr->pixAryUC[85] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[85])
      return 1;

   fontSTPtr->widthArySS[85] = 12;
   fontSTPtr->lenArySS[85] = 18;

   fontSTPtr->pixAryUC[85][0] = 0;
   fontSTPtr->pixAryUC[85][1] = 0;
   fontSTPtr->pixAryUC[85][2] = 0;
   fontSTPtr->pixAryUC[85][3] = 204;
   fontSTPtr->pixAryUC[85][4] = 192;
   fontSTPtr->pixAryUC[85][5] = 12;
   fontSTPtr->pixAryUC[85][6] = 204;
   fontSTPtr->pixAryUC[85][7] = 192;
   fontSTPtr->pixAryUC[85][8] = 12;
   fontSTPtr->pixAryUC[85][9] = 204;
   fontSTPtr->pixAryUC[85][10] = 134;
   fontSTPtr->pixAryUC[85][11] = 107;
   fontSTPtr->pixAryUC[85][12] = 144;
   fontSTPtr->pixAryUC[85][13] = 3;
   fontSTPtr->pixAryUC[85][14] = 0;
   fontSTPtr->pixAryUC[85][15] = 0;
   fontSTPtr->pixAryUC[85][16] = 0;
   fontSTPtr->pixAryUC[85][17] = 0;

   return 0;
} /*addAscii117_smallFont*/

/*___________________________________________
` Fun86: addAscii118_smallFont
`  - adds character `v` (ascii 118) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii118_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[86])
      free(fontSTPtr->pixAryUC[86]);

   fontSTPtr->pixAryUC[86] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[86])
      return 1;

   fontSTPtr->widthArySS[86] = 12;
   fontSTPtr->lenArySS[86] = 18;

   fontSTPtr->pixAryUC[86][0] = 0;
   fontSTPtr->pixAryUC[86][1] = 0;
   fontSTPtr->pixAryUC[86][2] = 0;
   fontSTPtr->pixAryUC[86][3] = 12;
   fontSTPtr->pixAryUC[86][4] = 195;
   fontSTPtr->pixAryUC[86][5] = 48;
   fontSTPtr->pixAryUC[86][6] = 12;
   fontSTPtr->pixAryUC[86][7] = 195;
   fontSTPtr->pixAryUC[86][8] = 48;
   fontSTPtr->pixAryUC[86][9] = 152;
   fontSTPtr->pixAryUC[86][10] = 1;
   fontSTPtr->pixAryUC[86][11] = 15;
   fontSTPtr->pixAryUC[86][12] = 96;
   fontSTPtr->pixAryUC[86][13] = 0;
   fontSTPtr->pixAryUC[86][14] = 0;
   fontSTPtr->pixAryUC[86][15] = 0;
   fontSTPtr->pixAryUC[86][16] = 0;
   fontSTPtr->pixAryUC[86][17] = 0;

   return 0;
} /*addAscii118_smallFont*/

/*___________________________________________
` Fun87: addAscii119_smallFont
`  - adds character `w` (ascii 119) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii119_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[87])
      free(fontSTPtr->pixAryUC[87]);

   fontSTPtr->pixAryUC[87] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[87])
      return 1;

   fontSTPtr->widthArySS[87] = 12;
   fontSTPtr->lenArySS[87] = 18;

   fontSTPtr->pixAryUC[87][0] = 0;
   fontSTPtr->pixAryUC[87][1] = 0;
   fontSTPtr->pixAryUC[87][2] = 0;
   fontSTPtr->pixAryUC[87][3] = 6;
   fontSTPtr->pixAryUC[87][4] = 102;
   fontSTPtr->pixAryUC[87][5] = 102;
   fontSTPtr->pixAryUC[87][6] = 246;
   fontSTPtr->pixAryUC[87][7] = 230;
   fontSTPtr->pixAryUC[87][8] = 127;
   fontSTPtr->pixAryUC[87][9] = 158;
   fontSTPtr->pixAryUC[87][10] = 231;
   fontSTPtr->pixAryUC[87][11] = 112;
   fontSTPtr->pixAryUC[87][12] = 6;
   fontSTPtr->pixAryUC[87][13] = 6;
   fontSTPtr->pixAryUC[87][14] = 0;
   fontSTPtr->pixAryUC[87][15] = 0;
   fontSTPtr->pixAryUC[87][16] = 0;
   fontSTPtr->pixAryUC[87][17] = 0;

   return 0;
} /*addAscii119_smallFont*/

/*___________________________________________
` Fun88: addAscii120_smallFont
`  - adds character `x` (ascii 120) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii120_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[88])
      free(fontSTPtr->pixAryUC[88]);

   fontSTPtr->pixAryUC[88] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[88])
      return 1;

   fontSTPtr->widthArySS[88] = 12;
   fontSTPtr->lenArySS[88] = 18;

   fontSTPtr->pixAryUC[88][0] = 0;
   fontSTPtr->pixAryUC[88][1] = 0;
   fontSTPtr->pixAryUC[88][2] = 0;
   fontSTPtr->pixAryUC[88][3] = 140;
   fontSTPtr->pixAryUC[88][4] = 129;
   fontSTPtr->pixAryUC[88][5] = 13;
   fontSTPtr->pixAryUC[88][6] = 112;
   fontSTPtr->pixAryUC[88][7] = 0;
   fontSTPtr->pixAryUC[88][8] = 2;
   fontSTPtr->pixAryUC[88][9] = 112;
   fontSTPtr->pixAryUC[88][10] = 128;
   fontSTPtr->pixAryUC[88][11] = 13;
   fontSTPtr->pixAryUC[88][12] = 140;
   fontSTPtr->pixAryUC[88][13] = 1;
   fontSTPtr->pixAryUC[88][14] = 0;
   fontSTPtr->pixAryUC[88][15] = 0;
   fontSTPtr->pixAryUC[88][16] = 0;
   fontSTPtr->pixAryUC[88][17] = 0;

   return 0;
} /*addAscii120_smallFont*/

/*___________________________________________
` Fun89: addAscii121_smallFont
`  - adds character `y` (ascii 121) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii121_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[89])
      free(fontSTPtr->pixAryUC[89]);

   fontSTPtr->pixAryUC[89] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[89])
      return 1;

   fontSTPtr->widthArySS[89] = 12;
   fontSTPtr->lenArySS[89] = 18;

   fontSTPtr->pixAryUC[89][0] = 0;
   fontSTPtr->pixAryUC[89][1] = 0;
   fontSTPtr->pixAryUC[89][2] = 0;
   fontSTPtr->pixAryUC[89][3] = 6;
   fontSTPtr->pixAryUC[89][4] = 198;
   fontSTPtr->pixAryUC[89][5] = 48;
   fontSTPtr->pixAryUC[89][6] = 152;
   fontSTPtr->pixAryUC[89][7] = 1;
   fontSTPtr->pixAryUC[89][8] = 15;
   fontSTPtr->pixAryUC[89][9] = 112;
   fontSTPtr->pixAryUC[89][10] = 0;
   fontSTPtr->pixAryUC[89][11] = 3;
   fontSTPtr->pixAryUC[89][12] = 24;
   fontSTPtr->pixAryUC[89][13] = 192;
   fontSTPtr->pixAryUC[89][14] = 0;
   fontSTPtr->pixAryUC[89][15] = 6;
   fontSTPtr->pixAryUC[89][16] = 0;
   fontSTPtr->pixAryUC[89][17] = 0;

   return 0;
} /*addAscii121_smallFont*/

/*___________________________________________
` Fun90: addAscii122_smallFont
`  - adds character `z` (ascii 122) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii122_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[90])
      free(fontSTPtr->pixAryUC[90]);

   fontSTPtr->pixAryUC[90] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[90])
      return 1;

   fontSTPtr->widthArySS[90] = 12;
   fontSTPtr->lenArySS[90] = 18;

   fontSTPtr->pixAryUC[90][0] = 0;
   fontSTPtr->pixAryUC[90][1] = 0;
   fontSTPtr->pixAryUC[90][2] = 0;
   fontSTPtr->pixAryUC[90][3] = 254;
   fontSTPtr->pixAryUC[90][4] = 231;
   fontSTPtr->pixAryUC[90][5] = 63;
   fontSTPtr->pixAryUC[90][6] = 128;
   fontSTPtr->pixAryUC[90][7] = 225;
   fontSTPtr->pixAryUC[90][8] = 118;
   fontSTPtr->pixAryUC[90][9] = 24;
   fontSTPtr->pixAryUC[90][10] = 192;
   fontSTPtr->pixAryUC[90][11] = 127;
   fontSTPtr->pixAryUC[90][12] = 254;
   fontSTPtr->pixAryUC[90][13] = 7;
   fontSTPtr->pixAryUC[90][14] = 0;
   fontSTPtr->pixAryUC[90][15] = 0;
   fontSTPtr->pixAryUC[90][16] = 0;
   fontSTPtr->pixAryUC[90][17] = 0;

   return 0;
} /*addAscii122_smallFont*/

/*___________________________________________
` Fun91: addAscii123_smallFont
`  - adds character `{` (ascii 123) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii123_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[91])
      free(fontSTPtr->pixAryUC[91]);

   fontSTPtr->pixAryUC[91] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[91])
      return 1;

   fontSTPtr->widthArySS[91] = 12;
   fontSTPtr->lenArySS[91] = 18;

   fontSTPtr->pixAryUC[91][0] = 0;
   fontSTPtr->pixAryUC[91][1] = 0;
   fontSTPtr->pixAryUC[91][2] = 62;
   fontSTPtr->pixAryUC[91][3] = 240;
   fontSTPtr->pixAryUC[91][4] = 129;
   fontSTPtr->pixAryUC[91][5] = 1;
   fontSTPtr->pixAryUC[91][6] = 12;
   fontSTPtr->pixAryUC[91][7] = 192;
   fontSTPtr->pixAryUC[91][8] = 0;
   fontSTPtr->pixAryUC[91][9] = 24;
   fontSTPtr->pixAryUC[91][10] = 0;
   fontSTPtr->pixAryUC[91][11] = 31;
   fontSTPtr->pixAryUC[91][12] = 224;
   fontSTPtr->pixAryUC[91][13] = 3;
   fontSTPtr->pixAryUC[91][14] = 0;
   fontSTPtr->pixAryUC[91][15] = 0;
   fontSTPtr->pixAryUC[91][16] = 0;
   fontSTPtr->pixAryUC[91][17] = 0;

   return 0;
} /*addAscii123_smallFont*/

/*___________________________________________
` Fun92: addAscii124_smallFont
`  - adds character `|` (ascii 124) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii124_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[92])
      free(fontSTPtr->pixAryUC[92]);

   fontSTPtr->pixAryUC[92] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[92])
      return 1;

   fontSTPtr->widthArySS[92] = 12;
   fontSTPtr->lenArySS[92] = 18;

   fontSTPtr->pixAryUC[92][0] = 0;
   fontSTPtr->pixAryUC[92][1] = 0;
   fontSTPtr->pixAryUC[92][2] = 3;
   fontSTPtr->pixAryUC[92][3] = 48;
   fontSTPtr->pixAryUC[92][4] = 0;
   fontSTPtr->pixAryUC[92][5] = 3;
   fontSTPtr->pixAryUC[92][6] = 48;
   fontSTPtr->pixAryUC[92][7] = 0;
   fontSTPtr->pixAryUC[92][8] = 3;
   fontSTPtr->pixAryUC[92][9] = 48;
   fontSTPtr->pixAryUC[92][10] = 0;
   fontSTPtr->pixAryUC[92][11] = 3;
   fontSTPtr->pixAryUC[92][12] = 48;
   fontSTPtr->pixAryUC[92][13] = 0;
   fontSTPtr->pixAryUC[92][14] = 0;
   fontSTPtr->pixAryUC[92][15] = 0;
   fontSTPtr->pixAryUC[92][16] = 0;
   fontSTPtr->pixAryUC[92][17] = 0;

   return 0;
} /*addAscii124_smallFont*/

/*___________________________________________
` Fun93: addAscii125_smallFont
`  - adds character `}` (ascii 125) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii125_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[93])
      free(fontSTPtr->pixAryUC[93]);

   fontSTPtr->pixAryUC[93] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[93])
      return 1;

   fontSTPtr->widthArySS[93] = 12;
   fontSTPtr->lenArySS[93] = 18;

   fontSTPtr->pixAryUC[93][0] = 0;
   fontSTPtr->pixAryUC[93][1] = 192;
   fontSTPtr->pixAryUC[93][2] = 7;
   fontSTPtr->pixAryUC[93][3] = 248;
   fontSTPtr->pixAryUC[93][4] = 0;
   fontSTPtr->pixAryUC[93][5] = 24;
   fontSTPtr->pixAryUC[93][6] = 0;
   fontSTPtr->pixAryUC[93][7] = 3;
   fontSTPtr->pixAryUC[93][8] = 48;
   fontSTPtr->pixAryUC[93][9] = 128;
   fontSTPtr->pixAryUC[93][10] = 129;
   fontSTPtr->pixAryUC[93][11] = 15;
   fontSTPtr->pixAryUC[93][12] = 124;
   fontSTPtr->pixAryUC[93][13] = 0;
   fontSTPtr->pixAryUC[93][14] = 0;
   fontSTPtr->pixAryUC[93][15] = 0;
   fontSTPtr->pixAryUC[93][16] = 0;
   fontSTPtr->pixAryUC[93][17] = 0;

   return 0;
} /*addAscii125_smallFont*/

/*___________________________________________
` Fun94: addAscii126_smallFont
`  - adds character `~` (ascii 126) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii126_smallFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[94])
      free(fontSTPtr->pixAryUC[94]);

   fontSTPtr->pixAryUC[94] =
     calloc(18, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[94])
      return 1;

   fontSTPtr->widthArySS[94] = 12;
   fontSTPtr->lenArySS[94] = 18;

   fontSTPtr->pixAryUC[94][0] = 0;
   fontSTPtr->pixAryUC[94][1] = 0;
   fontSTPtr->pixAryUC[94][2] = 0;
   fontSTPtr->pixAryUC[94][3] = 0;
   fontSTPtr->pixAryUC[94][4] = 0;
   fontSTPtr->pixAryUC[94][5] = 0;
   fontSTPtr->pixAryUC[94][6] = 112;
   fontSTPtr->pixAryUC[94][7] = 196;
   fontSTPtr->pixAryUC[94][8] = 125;
   fontSTPtr->pixAryUC[94][9] = 6;
   fontSTPtr->pixAryUC[94][10] = 3;
   fontSTPtr->pixAryUC[94][11] = 0;
   fontSTPtr->pixAryUC[94][12] = 0;
   fontSTPtr->pixAryUC[94][13] = 0;
   fontSTPtr->pixAryUC[94][14] = 0;
   fontSTPtr->pixAryUC[94][15] = 0;
   fontSTPtr->pixAryUC[94][16] = 0;
   fontSTPtr->pixAryUC[94][17] = 0;

   return 0;
} /*addAscii126_smallFont*/

/*_____________________________________________
` Fun95: loadFont_smallFont
`  - loads the font in smallFont.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_smallFont(
   struct font_fontST *fontSTPtr
){
   fontSTPtr->heightSS = 12;
   fontSTPtr->gapSS = 0;

   if( addAscii32_smallFont(fontSTPtr) )
      return 1;
   if( addAscii33_smallFont(fontSTPtr) )
      return 1;
   if( addAscii34_smallFont(fontSTPtr) )
      return 1;
   if( addAscii35_smallFont(fontSTPtr) )
      return 1;
   if( addAscii36_smallFont(fontSTPtr) )
      return 1;
   if( addAscii37_smallFont(fontSTPtr) )
      return 1;
   if( addAscii38_smallFont(fontSTPtr) )
      return 1;
   if( addAscii39_smallFont(fontSTPtr) )
      return 1;
   if( addAscii40_smallFont(fontSTPtr) )
      return 1;
   if( addAscii41_smallFont(fontSTPtr) )
      return 1;
   if( addAscii42_smallFont(fontSTPtr) )
      return 1;
   if( addAscii43_smallFont(fontSTPtr) )
      return 1;
   if( addAscii44_smallFont(fontSTPtr) )
      return 1;
   if( addAscii45_smallFont(fontSTPtr) )
      return 1;
   if( addAscii46_smallFont(fontSTPtr) )
      return 1;
   if( addAscii47_smallFont(fontSTPtr) )
      return 1;
   if( addAscii48_smallFont(fontSTPtr) )
      return 1;
   if( addAscii49_smallFont(fontSTPtr) )
      return 1;
   if( addAscii50_smallFont(fontSTPtr) )
      return 1;
   if( addAscii51_smallFont(fontSTPtr) )
      return 1;
   if( addAscii52_smallFont(fontSTPtr) )
      return 1;
   if( addAscii53_smallFont(fontSTPtr) )
      return 1;
   if( addAscii54_smallFont(fontSTPtr) )
      return 1;
   if( addAscii55_smallFont(fontSTPtr) )
      return 1;
   if( addAscii56_smallFont(fontSTPtr) )
      return 1;
   if( addAscii57_smallFont(fontSTPtr) )
      return 1;
   if( addAscii58_smallFont(fontSTPtr) )
      return 1;
   if( addAscii59_smallFont(fontSTPtr) )
      return 1;
   if( addAscii60_smallFont(fontSTPtr) )
      return 1;
   if( addAscii61_smallFont(fontSTPtr) )
      return 1;
   if( addAscii62_smallFont(fontSTPtr) )
      return 1;
   if( addAscii63_smallFont(fontSTPtr) )
      return 1;
   if( addAscii64_smallFont(fontSTPtr) )
      return 1;
   if( addAscii65_smallFont(fontSTPtr) )
      return 1;
   if( addAscii66_smallFont(fontSTPtr) )
      return 1;
   if( addAscii67_smallFont(fontSTPtr) )
      return 1;
   if( addAscii68_smallFont(fontSTPtr) )
      return 1;
   if( addAscii69_smallFont(fontSTPtr) )
      return 1;
   if( addAscii70_smallFont(fontSTPtr) )
      return 1;
   if( addAscii71_smallFont(fontSTPtr) )
      return 1;
   if( addAscii72_smallFont(fontSTPtr) )
      return 1;
   if( addAscii73_smallFont(fontSTPtr) )
      return 1;
   if( addAscii74_smallFont(fontSTPtr) )
      return 1;
   if( addAscii75_smallFont(fontSTPtr) )
      return 1;
   if( addAscii76_smallFont(fontSTPtr) )
      return 1;
   if( addAscii77_smallFont(fontSTPtr) )
      return 1;
   if( addAscii78_smallFont(fontSTPtr) )
      return 1;
   if( addAscii79_smallFont(fontSTPtr) )
      return 1;
   if( addAscii80_smallFont(fontSTPtr) )
      return 1;
   if( addAscii81_smallFont(fontSTPtr) )
      return 1;
   if( addAscii82_smallFont(fontSTPtr) )
      return 1;
   if( addAscii83_smallFont(fontSTPtr) )
      return 1;
   if( addAscii84_smallFont(fontSTPtr) )
      return 1;
   if( addAscii85_smallFont(fontSTPtr) )
      return 1;
   if( addAscii86_smallFont(fontSTPtr) )
      return 1;
   if( addAscii87_smallFont(fontSTPtr) )
      return 1;
   if( addAscii88_smallFont(fontSTPtr) )
      return 1;
   if( addAscii89_smallFont(fontSTPtr) )
      return 1;
   if( addAscii90_smallFont(fontSTPtr) )
      return 1;
   if( addAscii91_smallFont(fontSTPtr) )
      return 1;
   if( addAscii92_smallFont(fontSTPtr) )
      return 1;
   if( addAscii93_smallFont(fontSTPtr) )
      return 1;
   if( addAscii94_smallFont(fontSTPtr) )
      return 1;
   if( addAscii95_smallFont(fontSTPtr) )
      return 1;
   if( addAscii96_smallFont(fontSTPtr) )
      return 1;
   if( addAscii97_smallFont(fontSTPtr) )
      return 1;
   if( addAscii98_smallFont(fontSTPtr) )
      return 1;
   if( addAscii99_smallFont(fontSTPtr) )
      return 1;
   if( addAscii100_smallFont(fontSTPtr) )
      return 1;
   if( addAscii101_smallFont(fontSTPtr) )
      return 1;
   if( addAscii102_smallFont(fontSTPtr) )
      return 1;
   if( addAscii103_smallFont(fontSTPtr) )
      return 1;
   if( addAscii104_smallFont(fontSTPtr) )
      return 1;
   if( addAscii105_smallFont(fontSTPtr) )
      return 1;
   if( addAscii106_smallFont(fontSTPtr) )
      return 1;
   if( addAscii107_smallFont(fontSTPtr) )
      return 1;
   if( addAscii108_smallFont(fontSTPtr) )
      return 1;
   if( addAscii109_smallFont(fontSTPtr) )
      return 1;
   if( addAscii110_smallFont(fontSTPtr) )
      return 1;
   if( addAscii111_smallFont(fontSTPtr) )
      return 1;
   if( addAscii112_smallFont(fontSTPtr) )
      return 1;
   if( addAscii113_smallFont(fontSTPtr) )
      return 1;
   if( addAscii114_smallFont(fontSTPtr) )
      return 1;
   if( addAscii115_smallFont(fontSTPtr) )
      return 1;
   if( addAscii116_smallFont(fontSTPtr) )
      return 1;
   if( addAscii117_smallFont(fontSTPtr) )
      return 1;
   if( addAscii118_smallFont(fontSTPtr) )
      return 1;
   if( addAscii119_smallFont(fontSTPtr) )
      return 1;
   if( addAscii120_smallFont(fontSTPtr) )
      return 1;
   if( addAscii121_smallFont(fontSTPtr) )
      return 1;
   if( addAscii122_smallFont(fontSTPtr) )
      return 1;
   if( addAscii123_smallFont(fontSTPtr) )
      return 1;
   if( addAscii124_smallFont(fontSTPtr) )
      return 1;
   if( addAscii125_smallFont(fontSTPtr) )
      return 1;
   if( addAscii126_smallFont(fontSTPtr) )
      return 1;

   return 0;
} /*loadFont_smallFont*/
