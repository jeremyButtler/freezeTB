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
` Fun00: addAscii32_normalFont
`  - adds character ` ` (ascii 32) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii32_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[0])
      free(fontSTPtr->pixAryUC[0]);

   fontSTPtr->pixAryUC[0] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[0])
      return 1;

   fontSTPtr->widthArySS[0] = 14;
   fontSTPtr->lenArySS[0] = 25;

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
   fontSTPtr->pixAryUC[0][18] = 0;
   fontSTPtr->pixAryUC[0][19] = 0;
   fontSTPtr->pixAryUC[0][20] = 0;
   fontSTPtr->pixAryUC[0][21] = 0;
   fontSTPtr->pixAryUC[0][22] = 0;
   fontSTPtr->pixAryUC[0][23] = 0;
   fontSTPtr->pixAryUC[0][24] = 0;

   return 0;
} /*addAscii32_normalFont*/

/*___________________________________________
` Fun01: addAscii33_normalFont
`  - adds character `!` (ascii 33) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii33_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[1])
      free(fontSTPtr->pixAryUC[1]);

   fontSTPtr->pixAryUC[1] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[1])
      return 1;

   fontSTPtr->widthArySS[1] = 14;
   fontSTPtr->lenArySS[1] = 25;

   fontSTPtr->pixAryUC[1][0] = 0;
   fontSTPtr->pixAryUC[1][1] = 0;
   fontSTPtr->pixAryUC[1][2] = 56;
   fontSTPtr->pixAryUC[1][3] = 0;
   fontSTPtr->pixAryUC[1][4] = 31;
   fontSTPtr->pixAryUC[1][5] = 192;
   fontSTPtr->pixAryUC[1][6] = 7;
   fontSTPtr->pixAryUC[1][7] = 224;
   fontSTPtr->pixAryUC[1][8] = 0;
   fontSTPtr->pixAryUC[1][9] = 56;
   fontSTPtr->pixAryUC[1][10] = 0;
   fontSTPtr->pixAryUC[1][11] = 4;
   fontSTPtr->pixAryUC[1][12] = 0;
   fontSTPtr->pixAryUC[1][13] = 0;
   fontSTPtr->pixAryUC[1][14] = 224;
   fontSTPtr->pixAryUC[1][15] = 0;
   fontSTPtr->pixAryUC[1][16] = 124;
   fontSTPtr->pixAryUC[1][17] = 0;
   fontSTPtr->pixAryUC[1][18] = 14;
   fontSTPtr->pixAryUC[1][19] = 0;
   fontSTPtr->pixAryUC[1][20] = 0;
   fontSTPtr->pixAryUC[1][21] = 0;
   fontSTPtr->pixAryUC[1][22] = 0;
   fontSTPtr->pixAryUC[1][23] = 0;
   fontSTPtr->pixAryUC[1][24] = 0;

   return 0;
} /*addAscii33_normalFont*/

/*___________________________________________
` Fun02: addAscii34_normalFont
`  - adds character `"` (ascii 34) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii34_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[2])
      free(fontSTPtr->pixAryUC[2]);

   fontSTPtr->pixAryUC[2] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[2])
      return 1;

   fontSTPtr->widthArySS[2] = 14;
   fontSTPtr->lenArySS[2] = 25;

   fontSTPtr->pixAryUC[2][0] = 0;
   fontSTPtr->pixAryUC[2][1] = 0;
   fontSTPtr->pixAryUC[2][2] = 207;
   fontSTPtr->pixAryUC[2][3] = 195;
   fontSTPtr->pixAryUC[2][4] = 243;
   fontSTPtr->pixAryUC[2][5] = 192;
   fontSTPtr->pixAryUC[2][6] = 48;
   fontSTPtr->pixAryUC[2][7] = 48;
   fontSTPtr->pixAryUC[2][8] = 12;
   fontSTPtr->pixAryUC[2][9] = 199;
   fontSTPtr->pixAryUC[2][10] = 1;
   fontSTPtr->pixAryUC[2][11] = 0;
   fontSTPtr->pixAryUC[2][12] = 0;
   fontSTPtr->pixAryUC[2][13] = 0;
   fontSTPtr->pixAryUC[2][14] = 0;
   fontSTPtr->pixAryUC[2][15] = 0;
   fontSTPtr->pixAryUC[2][16] = 0;
   fontSTPtr->pixAryUC[2][17] = 0;
   fontSTPtr->pixAryUC[2][18] = 0;
   fontSTPtr->pixAryUC[2][19] = 0;
   fontSTPtr->pixAryUC[2][20] = 0;
   fontSTPtr->pixAryUC[2][21] = 0;
   fontSTPtr->pixAryUC[2][22] = 0;
   fontSTPtr->pixAryUC[2][23] = 0;
   fontSTPtr->pixAryUC[2][24] = 0;

   return 0;
} /*addAscii34_normalFont*/

/*___________________________________________
` Fun03: addAscii35_normalFont
`  - adds character `#` (ascii 35) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii35_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[3])
      free(fontSTPtr->pixAryUC[3]);

   fontSTPtr->pixAryUC[3] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[3])
      return 1;

   fontSTPtr->widthArySS[3] = 14;
   fontSTPtr->lenArySS[3] = 25;

   fontSTPtr->pixAryUC[3][0] = 0;
   fontSTPtr->pixAryUC[3][1] = 0;
   fontSTPtr->pixAryUC[3][2] = 0;
   fontSTPtr->pixAryUC[3][3] = 0;
   fontSTPtr->pixAryUC[3][4] = 51;
   fontSTPtr->pixAryUC[3][5] = 192;
   fontSTPtr->pixAryUC[3][6] = 12;
   fontSTPtr->pixAryUC[3][7] = 254;
   fontSTPtr->pixAryUC[3][8] = 159;
   fontSTPtr->pixAryUC[3][9] = 255;
   fontSTPtr->pixAryUC[3][10] = 7;
   fontSTPtr->pixAryUC[3][11] = 51;
   fontSTPtr->pixAryUC[3][12] = 248;
   fontSTPtr->pixAryUC[3][13] = 127;
   fontSTPtr->pixAryUC[3][14] = 254;
   fontSTPtr->pixAryUC[3][15] = 31;
   fontSTPtr->pixAryUC[3][16] = 204;
   fontSTPtr->pixAryUC[3][17] = 0;
   fontSTPtr->pixAryUC[3][18] = 51;
   fontSTPtr->pixAryUC[3][19] = 0;
   fontSTPtr->pixAryUC[3][20] = 0;
   fontSTPtr->pixAryUC[3][21] = 0;
   fontSTPtr->pixAryUC[3][22] = 0;
   fontSTPtr->pixAryUC[3][23] = 0;
   fontSTPtr->pixAryUC[3][24] = 0;

   return 0;
} /*addAscii35_normalFont*/

/*___________________________________________
` Fun04: addAscii36_normalFont
`  - adds character `$` (ascii 36) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii36_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[4])
      free(fontSTPtr->pixAryUC[4]);

   fontSTPtr->pixAryUC[4] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[4])
      return 1;

   fontSTPtr->widthArySS[4] = 14;
   fontSTPtr->lenArySS[4] = 25;

   fontSTPtr->pixAryUC[4][0] = 0;
   fontSTPtr->pixAryUC[4][1] = 0;
   fontSTPtr->pixAryUC[4][2] = 216;
   fontSTPtr->pixAryUC[4][3] = 0;
   fontSTPtr->pixAryUC[4][4] = 54;
   fontSTPtr->pixAryUC[4][5] = 224;
   fontSTPtr->pixAryUC[4][6] = 63;
   fontSTPtr->pixAryUC[4][7] = 252;
   fontSTPtr->pixAryUC[4][8] = 31;
   fontSTPtr->pixAryUC[4][9] = 219;
   fontSTPtr->pixAryUC[4][10] = 192;
   fontSTPtr->pixAryUC[4][11] = 127;
   fontSTPtr->pixAryUC[4][12] = 224;
   fontSTPtr->pixAryUC[4][13] = 63;
   fontSTPtr->pixAryUC[4][14] = 96;
   fontSTPtr->pixAryUC[4][15] = 141;
   fontSTPtr->pixAryUC[4][16] = 89;
   fontSTPtr->pixAryUC[4][17] = 193;
   fontSTPtr->pixAryUC[4][18] = 255;
   fontSTPtr->pixAryUC[4][19] = 240;
   fontSTPtr->pixAryUC[4][20] = 31;
   fontSTPtr->pixAryUC[4][21] = 96;
   fontSTPtr->pixAryUC[4][22] = 3;
   fontSTPtr->pixAryUC[4][23] = 0;
   fontSTPtr->pixAryUC[4][24] = 0;

   return 0;
} /*addAscii36_normalFont*/

/*___________________________________________
` Fun05: addAscii37_normalFont
`  - adds character `%` (ascii 37) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii37_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[5])
      free(fontSTPtr->pixAryUC[5]);

   fontSTPtr->pixAryUC[5] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[5])
      return 1;

   fontSTPtr->widthArySS[5] = 14;
   fontSTPtr->lenArySS[5] = 25;

   fontSTPtr->pixAryUC[5][0] = 0;
   fontSTPtr->pixAryUC[5][1] = 0;
   fontSTPtr->pixAryUC[5][2] = 7;
   fontSTPtr->pixAryUC[5][3] = 35;
   fontSTPtr->pixAryUC[5][4] = 98;
   fontSTPtr->pixAryUC[5][5] = 112;
   fontSTPtr->pixAryUC[5][6] = 12;
   fontSTPtr->pixAryUC[5][7] = 128;
   fontSTPtr->pixAryUC[5][8] = 1;
   fontSTPtr->pixAryUC[5][9] = 48;
   fontSTPtr->pixAryUC[5][10] = 0;
   fontSTPtr->pixAryUC[5][11] = 6;
   fontSTPtr->pixAryUC[5][12] = 192;
   fontSTPtr->pixAryUC[5][13] = 28;
   fontSTPtr->pixAryUC[5][14] = 152;
   fontSTPtr->pixAryUC[5][15] = 8;
   fontSTPtr->pixAryUC[5][16] = 195;
   fontSTPtr->pixAryUC[5][17] = 97;
   fontSTPtr->pixAryUC[5][18] = 0;
   fontSTPtr->pixAryUC[5][19] = 0;
   fontSTPtr->pixAryUC[5][20] = 0;
   fontSTPtr->pixAryUC[5][21] = 0;
   fontSTPtr->pixAryUC[5][22] = 0;
   fontSTPtr->pixAryUC[5][23] = 0;
   fontSTPtr->pixAryUC[5][24] = 0;

   return 0;
} /*addAscii37_normalFont*/

/*___________________________________________
` Fun06: addAscii38_normalFont
`  - adds character `&` (ascii 38) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii38_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[6])
      free(fontSTPtr->pixAryUC[6]);

   fontSTPtr->pixAryUC[6] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[6])
      return 1;

   fontSTPtr->widthArySS[6] = 14;
   fontSTPtr->lenArySS[6] = 25;

   fontSTPtr->pixAryUC[6][0] = 0;
   fontSTPtr->pixAryUC[6][1] = 0;
   fontSTPtr->pixAryUC[6][2] = 124;
   fontSTPtr->pixAryUC[6][3] = 128;
   fontSTPtr->pixAryUC[6][4] = 31;
   fontSTPtr->pixAryUC[6][5] = 32;
   fontSTPtr->pixAryUC[6][6] = 6;
   fontSTPtr->pixAryUC[6][7] = 248;
   fontSTPtr->pixAryUC[6][8] = 30;
   fontSTPtr->pixAryUC[6][9] = 159;
   fontSTPtr->pixAryUC[6][10] = 103;
   fontSTPtr->pixAryUC[6][11] = 204;
   fontSTPtr->pixAryUC[6][12] = 8;
   fontSTPtr->pixAryUC[6][13] = 30;
   fontSTPtr->pixAryUC[6][14] = 198;
   fontSTPtr->pixAryUC[6][15] = 151;
   fontSTPtr->pixAryUC[6][16] = 159;
   fontSTPtr->pixAryUC[6][17] = 199;
   fontSTPtr->pixAryUC[6][18] = 227;
   fontSTPtr->pixAryUC[6][19] = 0;
   fontSTPtr->pixAryUC[6][20] = 0;
   fontSTPtr->pixAryUC[6][21] = 0;
   fontSTPtr->pixAryUC[6][22] = 0;
   fontSTPtr->pixAryUC[6][23] = 0;
   fontSTPtr->pixAryUC[6][24] = 0;

   return 0;
} /*addAscii38_normalFont*/

/*___________________________________________
` Fun07: addAscii39_normalFont
`  - adds character `'` (ascii 39) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii39_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[7])
      free(fontSTPtr->pixAryUC[7]);

   fontSTPtr->pixAryUC[7] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[7])
      return 1;

   fontSTPtr->widthArySS[7] = 14;
   fontSTPtr->lenArySS[7] = 25;

   fontSTPtr->pixAryUC[7][0] = 0;
   fontSTPtr->pixAryUC[7][1] = 0;
   fontSTPtr->pixAryUC[7][2] = 60;
   fontSTPtr->pixAryUC[7][3] = 0;
   fontSTPtr->pixAryUC[7][4] = 15;
   fontSTPtr->pixAryUC[7][5] = 0;
   fontSTPtr->pixAryUC[7][6] = 3;
   fontSTPtr->pixAryUC[7][7] = 192;
   fontSTPtr->pixAryUC[7][8] = 0;
   fontSTPtr->pixAryUC[7][9] = 28;
   fontSTPtr->pixAryUC[7][10] = 0;
   fontSTPtr->pixAryUC[7][11] = 0;
   fontSTPtr->pixAryUC[7][12] = 0;
   fontSTPtr->pixAryUC[7][13] = 0;
   fontSTPtr->pixAryUC[7][14] = 0;
   fontSTPtr->pixAryUC[7][15] = 0;
   fontSTPtr->pixAryUC[7][16] = 0;
   fontSTPtr->pixAryUC[7][17] = 0;
   fontSTPtr->pixAryUC[7][18] = 0;
   fontSTPtr->pixAryUC[7][19] = 0;
   fontSTPtr->pixAryUC[7][20] = 0;
   fontSTPtr->pixAryUC[7][21] = 0;
   fontSTPtr->pixAryUC[7][22] = 0;
   fontSTPtr->pixAryUC[7][23] = 0;
   fontSTPtr->pixAryUC[7][24] = 0;

   return 0;
} /*addAscii39_normalFont*/

/*___________________________________________
` Fun08: addAscii40_normalFont
`  - adds character `(` (ascii 40) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii40_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[8])
      free(fontSTPtr->pixAryUC[8]);

   fontSTPtr->pixAryUC[8] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[8])
      return 1;

   fontSTPtr->widthArySS[8] = 14;
   fontSTPtr->lenArySS[8] = 25;

   fontSTPtr->pixAryUC[8][0] = 0;
   fontSTPtr->pixAryUC[8][1] = 0;
   fontSTPtr->pixAryUC[8][2] = 240;
   fontSTPtr->pixAryUC[8][3] = 1;
   fontSTPtr->pixAryUC[8][4] = 62;
   fontSTPtr->pixAryUC[8][5] = 192;
   fontSTPtr->pixAryUC[8][6] = 1;
   fontSTPtr->pixAryUC[8][7] = 48;
   fontSTPtr->pixAryUC[8][8] = 0;
   fontSTPtr->pixAryUC[8][9] = 6;
   fontSTPtr->pixAryUC[8][10] = 128;
   fontSTPtr->pixAryUC[8][11] = 1;
   fontSTPtr->pixAryUC[8][12] = 192;
   fontSTPtr->pixAryUC[8][13] = 0;
   fontSTPtr->pixAryUC[8][14] = 112;
   fontSTPtr->pixAryUC[8][15] = 0;
   fontSTPtr->pixAryUC[8][16] = 248;
   fontSTPtr->pixAryUC[8][17] = 0;
   fontSTPtr->pixAryUC[8][18] = 124;
   fontSTPtr->pixAryUC[8][19] = 0;
   fontSTPtr->pixAryUC[8][20] = 0;
   fontSTPtr->pixAryUC[8][21] = 0;
   fontSTPtr->pixAryUC[8][22] = 0;
   fontSTPtr->pixAryUC[8][23] = 0;
   fontSTPtr->pixAryUC[8][24] = 0;

   return 0;
} /*addAscii40_normalFont*/

/*___________________________________________
` Fun09: addAscii41_normalFont
`  - adds character `)` (ascii 41) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii41_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[9])
      free(fontSTPtr->pixAryUC[9]);

   fontSTPtr->pixAryUC[9] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[9])
      return 1;

   fontSTPtr->widthArySS[9] = 14;
   fontSTPtr->lenArySS[9] = 25;

   fontSTPtr->pixAryUC[9][0] = 0;
   fontSTPtr->pixAryUC[9][1] = 0;
   fontSTPtr->pixAryUC[9][2] = 31;
   fontSTPtr->pixAryUC[9][3] = 128;
   fontSTPtr->pixAryUC[9][4] = 15;
   fontSTPtr->pixAryUC[9][5] = 0;
   fontSTPtr->pixAryUC[9][6] = 7;
   fontSTPtr->pixAryUC[9][7] = 128;
   fontSTPtr->pixAryUC[9][8] = 1;
   fontSTPtr->pixAryUC[9][9] = 192;
   fontSTPtr->pixAryUC[9][10] = 0;
   fontSTPtr->pixAryUC[9][11] = 48;
   fontSTPtr->pixAryUC[9][12] = 0;
   fontSTPtr->pixAryUC[9][13] = 6;
   fontSTPtr->pixAryUC[9][14] = 192;
   fontSTPtr->pixAryUC[9][15] = 1;
   fontSTPtr->pixAryUC[9][16] = 62;
   fontSTPtr->pixAryUC[9][17] = 192;
   fontSTPtr->pixAryUC[9][18] = 7;
   fontSTPtr->pixAryUC[9][19] = 0;
   fontSTPtr->pixAryUC[9][20] = 0;
   fontSTPtr->pixAryUC[9][21] = 0;
   fontSTPtr->pixAryUC[9][22] = 0;
   fontSTPtr->pixAryUC[9][23] = 0;
   fontSTPtr->pixAryUC[9][24] = 0;

   return 0;
} /*addAscii41_normalFont*/

/*___________________________________________
` Fun10: addAscii42_normalFont
`  - adds character `*` (ascii 42) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii42_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[10])
      free(fontSTPtr->pixAryUC[10]);

   fontSTPtr->pixAryUC[10] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[10])
      return 1;

   fontSTPtr->widthArySS[10] = 14;
   fontSTPtr->lenArySS[10] = 25;

   fontSTPtr->pixAryUC[10][0] = 0;
   fontSTPtr->pixAryUC[10][1] = 0;
   fontSTPtr->pixAryUC[10][2] = 0;
   fontSTPtr->pixAryUC[10][3] = 0;
   fontSTPtr->pixAryUC[10][4] = 12;
   fontSTPtr->pixAryUC[10][5] = 48;
   fontSTPtr->pixAryUC[10][6] = 51;
   fontSTPtr->pixAryUC[10][7] = 240;
   fontSTPtr->pixAryUC[10][8] = 3;
   fontSTPtr->pixAryUC[10][9] = 252;
   fontSTPtr->pixAryUC[10][10] = 192;
   fontSTPtr->pixAryUC[10][11] = 204;
   fontSTPtr->pixAryUC[10][12] = 0;
   fontSTPtr->pixAryUC[10][13] = 3;
   fontSTPtr->pixAryUC[10][14] = 0;
   fontSTPtr->pixAryUC[10][15] = 0;
   fontSTPtr->pixAryUC[10][16] = 0;
   fontSTPtr->pixAryUC[10][17] = 0;
   fontSTPtr->pixAryUC[10][18] = 0;
   fontSTPtr->pixAryUC[10][19] = 0;
   fontSTPtr->pixAryUC[10][20] = 0;
   fontSTPtr->pixAryUC[10][21] = 0;
   fontSTPtr->pixAryUC[10][22] = 0;
   fontSTPtr->pixAryUC[10][23] = 0;
   fontSTPtr->pixAryUC[10][24] = 0;

   return 0;
} /*addAscii42_normalFont*/

/*___________________________________________
` Fun11: addAscii43_normalFont
`  - adds character `+` (ascii 43) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii43_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[11])
      free(fontSTPtr->pixAryUC[11]);

   fontSTPtr->pixAryUC[11] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[11])
      return 1;

   fontSTPtr->widthArySS[11] = 14;
   fontSTPtr->lenArySS[11] = 25;

   fontSTPtr->pixAryUC[11][0] = 0;
   fontSTPtr->pixAryUC[11][1] = 0;
   fontSTPtr->pixAryUC[11][2] = 0;
   fontSTPtr->pixAryUC[11][3] = 0;
   fontSTPtr->pixAryUC[11][4] = 12;
   fontSTPtr->pixAryUC[11][5] = 0;
   fontSTPtr->pixAryUC[11][6] = 3;
   fontSTPtr->pixAryUC[11][7] = 192;
   fontSTPtr->pixAryUC[11][8] = 128;
   fontSTPtr->pixAryUC[11][9] = 255;
   fontSTPtr->pixAryUC[11][10] = 231;
   fontSTPtr->pixAryUC[11][11] = 255;
   fontSTPtr->pixAryUC[11][12] = 1;
   fontSTPtr->pixAryUC[11][13] = 3;
   fontSTPtr->pixAryUC[11][14] = 192;
   fontSTPtr->pixAryUC[11][15] = 0;
   fontSTPtr->pixAryUC[11][16] = 48;
   fontSTPtr->pixAryUC[11][17] = 0;
   fontSTPtr->pixAryUC[11][18] = 12;
   fontSTPtr->pixAryUC[11][19] = 0;
   fontSTPtr->pixAryUC[11][20] = 0;
   fontSTPtr->pixAryUC[11][21] = 0;
   fontSTPtr->pixAryUC[11][22] = 0;
   fontSTPtr->pixAryUC[11][23] = 0;
   fontSTPtr->pixAryUC[11][24] = 0;

   return 0;
} /*addAscii43_normalFont*/

/*___________________________________________
` Fun12: addAscii44_normalFont
`  - adds character `,` (ascii 44) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii44_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[12])
      free(fontSTPtr->pixAryUC[12]);

   fontSTPtr->pixAryUC[12] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[12])
      return 1;

   fontSTPtr->widthArySS[12] = 14;
   fontSTPtr->lenArySS[12] = 25;

   fontSTPtr->pixAryUC[12][0] = 0;
   fontSTPtr->pixAryUC[12][1] = 0;
   fontSTPtr->pixAryUC[12][2] = 0;
   fontSTPtr->pixAryUC[12][3] = 0;
   fontSTPtr->pixAryUC[12][4] = 0;
   fontSTPtr->pixAryUC[12][5] = 0;
   fontSTPtr->pixAryUC[12][6] = 0;
   fontSTPtr->pixAryUC[12][7] = 0;
   fontSTPtr->pixAryUC[12][8] = 0;
   fontSTPtr->pixAryUC[12][9] = 0;
   fontSTPtr->pixAryUC[12][10] = 0;
   fontSTPtr->pixAryUC[12][11] = 0;
   fontSTPtr->pixAryUC[12][12] = 0;
   fontSTPtr->pixAryUC[12][13] = 0;
   fontSTPtr->pixAryUC[12][14] = 192;
   fontSTPtr->pixAryUC[12][15] = 3;
   fontSTPtr->pixAryUC[12][16] = 240;
   fontSTPtr->pixAryUC[12][17] = 0;
   fontSTPtr->pixAryUC[12][18] = 60;
   fontSTPtr->pixAryUC[12][19] = 0;
   fontSTPtr->pixAryUC[12][20] = 14;
   fontSTPtr->pixAryUC[12][21] = 176;
   fontSTPtr->pixAryUC[12][22] = 1;
   fontSTPtr->pixAryUC[12][23] = 56;
   fontSTPtr->pixAryUC[12][24] = 0;

   return 0;
} /*addAscii44_normalFont*/

/*___________________________________________
` Fun13: addAscii45_normalFont
`  - adds character `-` (ascii 45) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii45_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[13])
      free(fontSTPtr->pixAryUC[13]);

   fontSTPtr->pixAryUC[13] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[13])
      return 1;

   fontSTPtr->widthArySS[13] = 14;
   fontSTPtr->lenArySS[13] = 25;

   fontSTPtr->pixAryUC[13][0] = 0;
   fontSTPtr->pixAryUC[13][1] = 0;
   fontSTPtr->pixAryUC[13][2] = 0;
   fontSTPtr->pixAryUC[13][3] = 0;
   fontSTPtr->pixAryUC[13][4] = 0;
   fontSTPtr->pixAryUC[13][5] = 0;
   fontSTPtr->pixAryUC[13][6] = 0;
   fontSTPtr->pixAryUC[13][7] = 0;
   fontSTPtr->pixAryUC[13][8] = 0;
   fontSTPtr->pixAryUC[13][9] = 254;
   fontSTPtr->pixAryUC[13][10] = 129;
   fontSTPtr->pixAryUC[13][11] = 127;
   fontSTPtr->pixAryUC[13][12] = 0;
   fontSTPtr->pixAryUC[13][13] = 0;
   fontSTPtr->pixAryUC[13][14] = 0;
   fontSTPtr->pixAryUC[13][15] = 0;
   fontSTPtr->pixAryUC[13][16] = 0;
   fontSTPtr->pixAryUC[13][17] = 0;
   fontSTPtr->pixAryUC[13][18] = 0;
   fontSTPtr->pixAryUC[13][19] = 0;
   fontSTPtr->pixAryUC[13][20] = 0;
   fontSTPtr->pixAryUC[13][21] = 0;
   fontSTPtr->pixAryUC[13][22] = 0;
   fontSTPtr->pixAryUC[13][23] = 0;
   fontSTPtr->pixAryUC[13][24] = 0;

   return 0;
} /*addAscii45_normalFont*/

/*___________________________________________
` Fun14: addAscii46_normalFont
`  - adds character `.` (ascii 46) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii46_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[14])
      free(fontSTPtr->pixAryUC[14]);

   fontSTPtr->pixAryUC[14] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[14])
      return 1;

   fontSTPtr->widthArySS[14] = 14;
   fontSTPtr->lenArySS[14] = 25;

   fontSTPtr->pixAryUC[14][0] = 0;
   fontSTPtr->pixAryUC[14][1] = 0;
   fontSTPtr->pixAryUC[14][2] = 0;
   fontSTPtr->pixAryUC[14][3] = 0;
   fontSTPtr->pixAryUC[14][4] = 0;
   fontSTPtr->pixAryUC[14][5] = 0;
   fontSTPtr->pixAryUC[14][6] = 0;
   fontSTPtr->pixAryUC[14][7] = 0;
   fontSTPtr->pixAryUC[14][8] = 0;
   fontSTPtr->pixAryUC[14][9] = 0;
   fontSTPtr->pixAryUC[14][10] = 0;
   fontSTPtr->pixAryUC[14][11] = 0;
   fontSTPtr->pixAryUC[14][12] = 0;
   fontSTPtr->pixAryUC[14][13] = 0;
   fontSTPtr->pixAryUC[14][14] = 224;
   fontSTPtr->pixAryUC[14][15] = 1;
   fontSTPtr->pixAryUC[14][16] = 120;
   fontSTPtr->pixAryUC[14][17] = 0;
   fontSTPtr->pixAryUC[14][18] = 30;
   fontSTPtr->pixAryUC[14][19] = 0;
   fontSTPtr->pixAryUC[14][20] = 0;
   fontSTPtr->pixAryUC[14][21] = 0;
   fontSTPtr->pixAryUC[14][22] = 0;
   fontSTPtr->pixAryUC[14][23] = 0;
   fontSTPtr->pixAryUC[14][24] = 0;

   return 0;
} /*addAscii46_normalFont*/

/*___________________________________________
` Fun15: addAscii47_normalFont
`  - adds character `/` (ascii 47) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii47_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[15])
      free(fontSTPtr->pixAryUC[15]);

   fontSTPtr->pixAryUC[15] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[15])
      return 1;

   fontSTPtr->widthArySS[15] = 14;
   fontSTPtr->lenArySS[15] = 25;

   fontSTPtr->pixAryUC[15][0] = 0;
   fontSTPtr->pixAryUC[15][1] = 0;
   fontSTPtr->pixAryUC[15][2] = 0;
   fontSTPtr->pixAryUC[15][3] = 3;
   fontSTPtr->pixAryUC[15][4] = 96;
   fontSTPtr->pixAryUC[15][5] = 0;
   fontSTPtr->pixAryUC[15][6] = 12;
   fontSTPtr->pixAryUC[15][7] = 128;
   fontSTPtr->pixAryUC[15][8] = 1;
   fontSTPtr->pixAryUC[15][9] = 48;
   fontSTPtr->pixAryUC[15][10] = 0;
   fontSTPtr->pixAryUC[15][11] = 6;
   fontSTPtr->pixAryUC[15][12] = 192;
   fontSTPtr->pixAryUC[15][13] = 0;
   fontSTPtr->pixAryUC[15][14] = 24;
   fontSTPtr->pixAryUC[15][15] = 0;
   fontSTPtr->pixAryUC[15][16] = 3;
   fontSTPtr->pixAryUC[15][17] = 96;
   fontSTPtr->pixAryUC[15][18] = 0;
   fontSTPtr->pixAryUC[15][19] = 0;
   fontSTPtr->pixAryUC[15][20] = 0;
   fontSTPtr->pixAryUC[15][21] = 0;
   fontSTPtr->pixAryUC[15][22] = 0;
   fontSTPtr->pixAryUC[15][23] = 0;
   fontSTPtr->pixAryUC[15][24] = 0;

   return 0;
} /*addAscii47_normalFont*/

/*___________________________________________
` Fun16: addAscii48_normalFont
`  - adds character `0` (ascii 48) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii48_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[16])
      free(fontSTPtr->pixAryUC[16]);

   fontSTPtr->pixAryUC[16] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[16])
      return 1;

   fontSTPtr->widthArySS[16] = 14;
   fontSTPtr->lenArySS[16] = 25;

   fontSTPtr->pixAryUC[16][0] = 0;
   fontSTPtr->pixAryUC[16][1] = 0;
   fontSTPtr->pixAryUC[16][2] = 252;
   fontSTPtr->pixAryUC[16][3] = 128;
   fontSTPtr->pixAryUC[16][4] = 127;
   fontSTPtr->pixAryUC[16][5] = 240;
   fontSTPtr->pixAryUC[16][6] = 48;
   fontSTPtr->pixAryUC[16][7] = 62;
   fontSTPtr->pixAryUC[16][8] = 152;
   fontSTPtr->pixAryUC[16][9] = 25;
   fontSTPtr->pixAryUC[16][10] = 102;
   fontSTPtr->pixAryUC[16][11] = 140;
   fontSTPtr->pixAryUC[16][12] = 25;
   fontSTPtr->pixAryUC[16][13] = 102;
   fontSTPtr->pixAryUC[16][14] = 12;
   fontSTPtr->pixAryUC[16][15] = 15;
   fontSTPtr->pixAryUC[16][16] = 254;
   fontSTPtr->pixAryUC[16][17] = 1;
   fontSTPtr->pixAryUC[16][18] = 63;
   fontSTPtr->pixAryUC[16][19] = 0;
   fontSTPtr->pixAryUC[16][20] = 0;
   fontSTPtr->pixAryUC[16][21] = 0;
   fontSTPtr->pixAryUC[16][22] = 0;
   fontSTPtr->pixAryUC[16][23] = 0;
   fontSTPtr->pixAryUC[16][24] = 0;

   return 0;
} /*addAscii48_normalFont*/

/*___________________________________________
` Fun17: addAscii49_normalFont
`  - adds character `1` (ascii 49) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii49_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[17])
      free(fontSTPtr->pixAryUC[17]);

   fontSTPtr->pixAryUC[17] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[17])
      return 1;

   fontSTPtr->widthArySS[17] = 14;
   fontSTPtr->lenArySS[17] = 25;

   fontSTPtr->pixAryUC[17][0] = 0;
   fontSTPtr->pixAryUC[17][1] = 0;
   fontSTPtr->pixAryUC[17][2] = 56;
   fontSTPtr->pixAryUC[17][3] = 0;
   fontSTPtr->pixAryUC[17][4] = 15;
   fontSTPtr->pixAryUC[17][5] = 96;
   fontSTPtr->pixAryUC[17][6] = 3;
   fontSTPtr->pixAryUC[17][7] = 204;
   fontSTPtr->pixAryUC[17][8] = 0;
   fontSTPtr->pixAryUC[17][9] = 48;
   fontSTPtr->pixAryUC[17][10] = 0;
   fontSTPtr->pixAryUC[17][11] = 12;
   fontSTPtr->pixAryUC[17][12] = 0;
   fontSTPtr->pixAryUC[17][13] = 3;
   fontSTPtr->pixAryUC[17][14] = 192;
   fontSTPtr->pixAryUC[17][15] = 0;
   fontSTPtr->pixAryUC[17][16] = 255;
   fontSTPtr->pixAryUC[17][17] = 227;
   fontSTPtr->pixAryUC[17][18] = 255;
   fontSTPtr->pixAryUC[17][19] = 1;
   fontSTPtr->pixAryUC[17][20] = 0;
   fontSTPtr->pixAryUC[17][21] = 0;
   fontSTPtr->pixAryUC[17][22] = 0;
   fontSTPtr->pixAryUC[17][23] = 0;
   fontSTPtr->pixAryUC[17][24] = 0;

   return 0;
} /*addAscii49_normalFont*/

/*___________________________________________
` Fun18: addAscii50_normalFont
`  - adds character `2` (ascii 50) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii50_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[18])
      free(fontSTPtr->pixAryUC[18]);

   fontSTPtr->pixAryUC[18] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[18])
      return 1;

   fontSTPtr->widthArySS[18] = 14;
   fontSTPtr->lenArySS[18] = 25;

   fontSTPtr->pixAryUC[18][0] = 0;
   fontSTPtr->pixAryUC[18][1] = 0;
   fontSTPtr->pixAryUC[18][2] = 63;
   fontSTPtr->pixAryUC[18][3] = 224;
   fontSTPtr->pixAryUC[18][4] = 31;
   fontSTPtr->pixAryUC[18][5] = 24;
   fontSTPtr->pixAryUC[18][6] = 6;
   fontSTPtr->pixAryUC[18][7] = 128;
   fontSTPtr->pixAryUC[18][8] = 1;
   fontSTPtr->pixAryUC[18][9] = 48;
   fontSTPtr->pixAryUC[18][10] = 0;
   fontSTPtr->pixAryUC[18][11] = 6;
   fontSTPtr->pixAryUC[18][12] = 192;
   fontSTPtr->pixAryUC[18][13] = 0;
   fontSTPtr->pixAryUC[18][14] = 24;
   fontSTPtr->pixAryUC[18][15] = 0;
   fontSTPtr->pixAryUC[18][16] = 255;
   fontSTPtr->pixAryUC[18][17] = 225;
   fontSTPtr->pixAryUC[18][18] = 255;
   fontSTPtr->pixAryUC[18][19] = 0;
   fontSTPtr->pixAryUC[18][20] = 0;
   fontSTPtr->pixAryUC[18][21] = 0;
   fontSTPtr->pixAryUC[18][22] = 0;
   fontSTPtr->pixAryUC[18][23] = 0;
   fontSTPtr->pixAryUC[18][24] = 0;

   return 0;
} /*addAscii50_normalFont*/

/*___________________________________________
` Fun19: addAscii51_normalFont
`  - adds character `3` (ascii 51) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii51_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[19])
      free(fontSTPtr->pixAryUC[19]);

   fontSTPtr->pixAryUC[19] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[19])
      return 1;

   fontSTPtr->widthArySS[19] = 14;
   fontSTPtr->lenArySS[19] = 25;

   fontSTPtr->pixAryUC[19][0] = 0;
   fontSTPtr->pixAryUC[19][1] = 0;
   fontSTPtr->pixAryUC[19][2] = 126;
   fontSTPtr->pixAryUC[19][3] = 192;
   fontSTPtr->pixAryUC[19][4] = 63;
   fontSTPtr->pixAryUC[19][5] = 48;
   fontSTPtr->pixAryUC[19][6] = 24;
   fontSTPtr->pixAryUC[19][7] = 0;
   fontSTPtr->pixAryUC[19][8] = 6;
   fontSTPtr->pixAryUC[19][9] = 240;
   fontSTPtr->pixAryUC[19][10] = 0;
   fontSTPtr->pixAryUC[19][11] = 60;
   fontSTPtr->pixAryUC[19][12] = 0;
   fontSTPtr->pixAryUC[19][13] = 24;
   fontSTPtr->pixAryUC[19][14] = 12;
   fontSTPtr->pixAryUC[19][15] = 6;
   fontSTPtr->pixAryUC[19][16] = 255;
   fontSTPtr->pixAryUC[19][17] = 128;
   fontSTPtr->pixAryUC[19][18] = 31;
   fontSTPtr->pixAryUC[19][19] = 0;
   fontSTPtr->pixAryUC[19][20] = 0;
   fontSTPtr->pixAryUC[19][21] = 0;
   fontSTPtr->pixAryUC[19][22] = 0;
   fontSTPtr->pixAryUC[19][23] = 0;
   fontSTPtr->pixAryUC[19][24] = 0;

   return 0;
} /*addAscii51_normalFont*/

/*___________________________________________
` Fun20: addAscii52_normalFont
`  - adds character `4` (ascii 52) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii52_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[20])
      free(fontSTPtr->pixAryUC[20]);

   fontSTPtr->pixAryUC[20] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[20])
      return 1;

   fontSTPtr->widthArySS[20] = 14;
   fontSTPtr->lenArySS[20] = 25;

   fontSTPtr->pixAryUC[20][0] = 0;
   fontSTPtr->pixAryUC[20][1] = 0;
   fontSTPtr->pixAryUC[20][2] = 120;
   fontSTPtr->pixAryUC[20][3] = 0;
   fontSTPtr->pixAryUC[20][4] = 63;
   fontSTPtr->pixAryUC[20][5] = 96;
   fontSTPtr->pixAryUC[20][6] = 12;
   fontSTPtr->pixAryUC[20][7] = 12;
   fontSTPtr->pixAryUC[20][8] = 131;
   fontSTPtr->pixAryUC[20][9] = 255;
   fontSTPtr->pixAryUC[20][10] = 231;
   fontSTPtr->pixAryUC[20][11] = 255;
   fontSTPtr->pixAryUC[20][12] = 1;
   fontSTPtr->pixAryUC[20][13] = 12;
   fontSTPtr->pixAryUC[20][14] = 0;
   fontSTPtr->pixAryUC[20][15] = 3;
   fontSTPtr->pixAryUC[20][16] = 192;
   fontSTPtr->pixAryUC[20][17] = 0;
   fontSTPtr->pixAryUC[20][18] = 48;
   fontSTPtr->pixAryUC[20][19] = 0;
   fontSTPtr->pixAryUC[20][20] = 0;
   fontSTPtr->pixAryUC[20][21] = 0;
   fontSTPtr->pixAryUC[20][22] = 0;
   fontSTPtr->pixAryUC[20][23] = 0;
   fontSTPtr->pixAryUC[20][24] = 0;

   return 0;
} /*addAscii52_normalFont*/

/*___________________________________________
` Fun21: addAscii53_normalFont
`  - adds character `5` (ascii 53) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii53_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[21])
      free(fontSTPtr->pixAryUC[21]);

   fontSTPtr->pixAryUC[21] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[21])
      return 1;

   fontSTPtr->widthArySS[21] = 14;
   fontSTPtr->lenArySS[21] = 25;

   fontSTPtr->pixAryUC[21][0] = 0;
   fontSTPtr->pixAryUC[21][1] = 0;
   fontSTPtr->pixAryUC[21][2] = 255;
   fontSTPtr->pixAryUC[21][3] = 195;
   fontSTPtr->pixAryUC[21][4] = 255;
   fontSTPtr->pixAryUC[21][5] = 48;
   fontSTPtr->pixAryUC[21][6] = 0;
   fontSTPtr->pixAryUC[21][7] = 12;
   fontSTPtr->pixAryUC[21][8] = 0;
   fontSTPtr->pixAryUC[21][9] = 255;
   fontSTPtr->pixAryUC[21][10] = 193;
   fontSTPtr->pixAryUC[21][11] = 255;
   fontSTPtr->pixAryUC[21][12] = 0;
   fontSTPtr->pixAryUC[21][13] = 48;
   fontSTPtr->pixAryUC[21][14] = 0;
   fontSTPtr->pixAryUC[21][15] = 12;
   fontSTPtr->pixAryUC[21][16] = 255;
   fontSTPtr->pixAryUC[21][17] = 195;
   fontSTPtr->pixAryUC[21][18] = 127;
   fontSTPtr->pixAryUC[21][19] = 0;
   fontSTPtr->pixAryUC[21][20] = 0;
   fontSTPtr->pixAryUC[21][21] = 0;
   fontSTPtr->pixAryUC[21][22] = 0;
   fontSTPtr->pixAryUC[21][23] = 0;
   fontSTPtr->pixAryUC[21][24] = 0;

   return 0;
} /*addAscii53_normalFont*/

/*___________________________________________
` Fun22: addAscii54_normalFont
`  - adds character `6` (ascii 54) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii54_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[22])
      free(fontSTPtr->pixAryUC[22]);

   fontSTPtr->pixAryUC[22] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[22])
      return 1;

   fontSTPtr->widthArySS[22] = 14;
   fontSTPtr->lenArySS[22] = 25;

   fontSTPtr->pixAryUC[22][0] = 0;
   fontSTPtr->pixAryUC[22][1] = 0;
   fontSTPtr->pixAryUC[22][2] = 252;
   fontSTPtr->pixAryUC[22][3] = 129;
   fontSTPtr->pixAryUC[22][4] = 255;
   fontSTPtr->pixAryUC[22][5] = 48;
   fontSTPtr->pixAryUC[22][6] = 96;
   fontSTPtr->pixAryUC[22][7] = 6;
   fontSTPtr->pixAryUC[22][8] = 128;
   fontSTPtr->pixAryUC[22][9] = 255;
   fontSTPtr->pixAryUC[22][10] = 224;
   fontSTPtr->pixAryUC[22][11] = 127;
   fontSTPtr->pixAryUC[22][12] = 24;
   fontSTPtr->pixAryUC[22][13] = 48;
   fontSTPtr->pixAryUC[22][14] = 6;
   fontSTPtr->pixAryUC[22][15] = 12;
   fontSTPtr->pixAryUC[22][16] = 255;
   fontSTPtr->pixAryUC[22][17] = 129;
   fontSTPtr->pixAryUC[22][18] = 63;
   fontSTPtr->pixAryUC[22][19] = 0;
   fontSTPtr->pixAryUC[22][20] = 0;
   fontSTPtr->pixAryUC[22][21] = 0;
   fontSTPtr->pixAryUC[22][22] = 0;
   fontSTPtr->pixAryUC[22][23] = 0;
   fontSTPtr->pixAryUC[22][24] = 0;

   return 0;
} /*addAscii54_normalFont*/

/*___________________________________________
` Fun23: addAscii55_normalFont
`  - adds character `7` (ascii 55) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii55_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[23])
      free(fontSTPtr->pixAryUC[23]);

   fontSTPtr->pixAryUC[23] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[23])
      return 1;

   fontSTPtr->widthArySS[23] = 14;
   fontSTPtr->lenArySS[23] = 25;

   fontSTPtr->pixAryUC[23][0] = 0;
   fontSTPtr->pixAryUC[23][1] = 0;
   fontSTPtr->pixAryUC[23][2] = 254;
   fontSTPtr->pixAryUC[23][3] = 131;
   fontSTPtr->pixAryUC[23][4] = 127;
   fontSTPtr->pixAryUC[23][5] = 0;
   fontSTPtr->pixAryUC[23][6] = 12;
   fontSTPtr->pixAryUC[23][7] = 128;
   fontSTPtr->pixAryUC[23][8] = 1;
   fontSTPtr->pixAryUC[23][9] = 183;
   fontSTPtr->pixAryUC[23][10] = 195;
   fontSTPtr->pixAryUC[23][11] = 246;
   fontSTPtr->pixAryUC[23][12] = 128;
   fontSTPtr->pixAryUC[23][13] = 1;
   fontSTPtr->pixAryUC[23][14] = 96;
   fontSTPtr->pixAryUC[23][15] = 0;
   fontSTPtr->pixAryUC[23][16] = 24;
   fontSTPtr->pixAryUC[23][17] = 0;
   fontSTPtr->pixAryUC[23][18] = 6;
   fontSTPtr->pixAryUC[23][19] = 0;
   fontSTPtr->pixAryUC[23][20] = 0;
   fontSTPtr->pixAryUC[23][21] = 0;
   fontSTPtr->pixAryUC[23][22] = 0;
   fontSTPtr->pixAryUC[23][23] = 0;
   fontSTPtr->pixAryUC[23][24] = 0;

   return 0;
} /*addAscii55_normalFont*/

/*___________________________________________
` Fun24: addAscii56_normalFont
`  - adds character `8` (ascii 56) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii56_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[24])
      free(fontSTPtr->pixAryUC[24]);

   fontSTPtr->pixAryUC[24] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[24])
      return 1;

   fontSTPtr->widthArySS[24] = 14;
   fontSTPtr->lenArySS[24] = 25;

   fontSTPtr->pixAryUC[24][0] = 0;
   fontSTPtr->pixAryUC[24][1] = 0;
   fontSTPtr->pixAryUC[24][2] = 252;
   fontSTPtr->pixAryUC[24][3] = 128;
   fontSTPtr->pixAryUC[24][4] = 127;
   fontSTPtr->pixAryUC[24][5] = 48;
   fontSTPtr->pixAryUC[24][6] = 48;
   fontSTPtr->pixAryUC[24][7] = 12;
   fontSTPtr->pixAryUC[24][8] = 12;
   fontSTPtr->pixAryUC[24][9] = 254;
   fontSTPtr->pixAryUC[24][10] = 129;
   fontSTPtr->pixAryUC[24][11] = 127;
   fontSTPtr->pixAryUC[24][12] = 48;
   fontSTPtr->pixAryUC[24][13] = 48;
   fontSTPtr->pixAryUC[24][14] = 12;
   fontSTPtr->pixAryUC[24][15] = 12;
   fontSTPtr->pixAryUC[24][16] = 254;
   fontSTPtr->pixAryUC[24][17] = 1;
   fontSTPtr->pixAryUC[24][18] = 63;
   fontSTPtr->pixAryUC[24][19] = 0;
   fontSTPtr->pixAryUC[24][20] = 0;
   fontSTPtr->pixAryUC[24][21] = 0;
   fontSTPtr->pixAryUC[24][22] = 0;
   fontSTPtr->pixAryUC[24][23] = 0;
   fontSTPtr->pixAryUC[24][24] = 0;

   return 0;
} /*addAscii56_normalFont*/

/*___________________________________________
` Fun25: addAscii57_normalFont
`  - adds character `9` (ascii 57) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii57_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[25])
      free(fontSTPtr->pixAryUC[25]);

   fontSTPtr->pixAryUC[25] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[25])
      return 1;

   fontSTPtr->widthArySS[25] = 14;
   fontSTPtr->lenArySS[25] = 25;

   fontSTPtr->pixAryUC[25][0] = 0;
   fontSTPtr->pixAryUC[25][1] = 0;
   fontSTPtr->pixAryUC[25][2] = 254;
   fontSTPtr->pixAryUC[25][3] = 192;
   fontSTPtr->pixAryUC[25][4] = 127;
   fontSTPtr->pixAryUC[25][5] = 24;
   fontSTPtr->pixAryUC[25][6] = 48;
   fontSTPtr->pixAryUC[25][7] = 6;
   fontSTPtr->pixAryUC[25][8] = 12;
   fontSTPtr->pixAryUC[25][9] = 255;
   fontSTPtr->pixAryUC[25][10] = 129;
   fontSTPtr->pixAryUC[25][11] = 63;
   fontSTPtr->pixAryUC[25][12] = 0;
   fontSTPtr->pixAryUC[25][13] = 6;
   fontSTPtr->pixAryUC[25][14] = 192;
   fontSTPtr->pixAryUC[25][15] = 0;
   fontSTPtr->pixAryUC[25][16] = 24;
   fontSTPtr->pixAryUC[25][17] = 0;
   fontSTPtr->pixAryUC[25][18] = 3;
   fontSTPtr->pixAryUC[25][19] = 0;
   fontSTPtr->pixAryUC[25][20] = 0;
   fontSTPtr->pixAryUC[25][21] = 0;
   fontSTPtr->pixAryUC[25][22] = 0;
   fontSTPtr->pixAryUC[25][23] = 0;
   fontSTPtr->pixAryUC[25][24] = 0;

   return 0;
} /*addAscii57_normalFont*/

/*___________________________________________
` Fun26: addAscii58_normalFont
`  - adds character `:` (ascii 58) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii58_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[26])
      free(fontSTPtr->pixAryUC[26]);

   fontSTPtr->pixAryUC[26] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[26])
      return 1;

   fontSTPtr->widthArySS[26] = 14;
   fontSTPtr->lenArySS[26] = 25;

   fontSTPtr->pixAryUC[26][0] = 0;
   fontSTPtr->pixAryUC[26][1] = 0;
   fontSTPtr->pixAryUC[26][2] = 0;
   fontSTPtr->pixAryUC[26][3] = 0;
   fontSTPtr->pixAryUC[26][4] = 28;
   fontSTPtr->pixAryUC[26][5] = 0;
   fontSTPtr->pixAryUC[26][6] = 7;
   fontSTPtr->pixAryUC[26][7] = 192;
   fontSTPtr->pixAryUC[26][8] = 1;
   fontSTPtr->pixAryUC[26][9] = 0;
   fontSTPtr->pixAryUC[26][10] = 0;
   fontSTPtr->pixAryUC[26][11] = 0;
   fontSTPtr->pixAryUC[26][12] = 0;
   fontSTPtr->pixAryUC[26][13] = 0;
   fontSTPtr->pixAryUC[26][14] = 192;
   fontSTPtr->pixAryUC[26][15] = 1;
   fontSTPtr->pixAryUC[26][16] = 112;
   fontSTPtr->pixAryUC[26][17] = 0;
   fontSTPtr->pixAryUC[26][18] = 28;
   fontSTPtr->pixAryUC[26][19] = 0;
   fontSTPtr->pixAryUC[26][20] = 0;
   fontSTPtr->pixAryUC[26][21] = 0;
   fontSTPtr->pixAryUC[26][22] = 0;
   fontSTPtr->pixAryUC[26][23] = 0;
   fontSTPtr->pixAryUC[26][24] = 0;

   return 0;
} /*addAscii58_normalFont*/

/*___________________________________________
` Fun27: addAscii59_normalFont
`  - adds character `;` (ascii 59) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii59_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[27])
      free(fontSTPtr->pixAryUC[27]);

   fontSTPtr->pixAryUC[27] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[27])
      return 1;

   fontSTPtr->widthArySS[27] = 14;
   fontSTPtr->lenArySS[27] = 25;

   fontSTPtr->pixAryUC[27][0] = 0;
   fontSTPtr->pixAryUC[27][1] = 0;
   fontSTPtr->pixAryUC[27][2] = 0;
   fontSTPtr->pixAryUC[27][3] = 0;
   fontSTPtr->pixAryUC[27][4] = 28;
   fontSTPtr->pixAryUC[27][5] = 0;
   fontSTPtr->pixAryUC[27][6] = 7;
   fontSTPtr->pixAryUC[27][7] = 192;
   fontSTPtr->pixAryUC[27][8] = 1;
   fontSTPtr->pixAryUC[27][9] = 0;
   fontSTPtr->pixAryUC[27][10] = 0;
   fontSTPtr->pixAryUC[27][11] = 0;
   fontSTPtr->pixAryUC[27][12] = 0;
   fontSTPtr->pixAryUC[27][13] = 0;
   fontSTPtr->pixAryUC[27][14] = 192;
   fontSTPtr->pixAryUC[27][15] = 1;
   fontSTPtr->pixAryUC[27][16] = 112;
   fontSTPtr->pixAryUC[27][17] = 0;
   fontSTPtr->pixAryUC[27][18] = 24;
   fontSTPtr->pixAryUC[27][19] = 0;
   fontSTPtr->pixAryUC[27][20] = 3;
   fontSTPtr->pixAryUC[27][21] = 96;
   fontSTPtr->pixAryUC[27][22] = 0;
   fontSTPtr->pixAryUC[27][23] = 0;
   fontSTPtr->pixAryUC[27][24] = 0;

   return 0;
} /*addAscii59_normalFont*/

/*___________________________________________
` Fun28: addAscii60_normalFont
`  - adds character `<` (ascii 60) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii60_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[28])
      free(fontSTPtr->pixAryUC[28]);

   fontSTPtr->pixAryUC[28] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[28])
      return 1;

   fontSTPtr->widthArySS[28] = 14;
   fontSTPtr->lenArySS[28] = 25;

   fontSTPtr->pixAryUC[28][0] = 0;
   fontSTPtr->pixAryUC[28][1] = 0;
   fontSTPtr->pixAryUC[28][2] = 0;
   fontSTPtr->pixAryUC[28][3] = 3;
   fontSTPtr->pixAryUC[28][4] = 48;
   fontSTPtr->pixAryUC[28][5] = 0;
   fontSTPtr->pixAryUC[28][6] = 3;
   fontSTPtr->pixAryUC[28][7] = 48;
   fontSTPtr->pixAryUC[28][8] = 0;
   fontSTPtr->pixAryUC[28][9] = 3;
   fontSTPtr->pixAryUC[28][10] = 192;
   fontSTPtr->pixAryUC[28][11] = 0;
   fontSTPtr->pixAryUC[28][12] = 192;
   fontSTPtr->pixAryUC[28][13] = 0;
   fontSTPtr->pixAryUC[28][14] = 192;
   fontSTPtr->pixAryUC[28][15] = 0;
   fontSTPtr->pixAryUC[28][16] = 192;
   fontSTPtr->pixAryUC[28][17] = 0;
   fontSTPtr->pixAryUC[28][18] = 192;
   fontSTPtr->pixAryUC[28][19] = 0;
   fontSTPtr->pixAryUC[28][20] = 0;
   fontSTPtr->pixAryUC[28][21] = 0;
   fontSTPtr->pixAryUC[28][22] = 0;
   fontSTPtr->pixAryUC[28][23] = 0;
   fontSTPtr->pixAryUC[28][24] = 0;

   return 0;
} /*addAscii60_normalFont*/

/*___________________________________________
` Fun29: addAscii61_normalFont
`  - adds character `=` (ascii 61) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii61_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[29])
      free(fontSTPtr->pixAryUC[29]);

   fontSTPtr->pixAryUC[29] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[29])
      return 1;

   fontSTPtr->widthArySS[29] = 14;
   fontSTPtr->lenArySS[29] = 25;

   fontSTPtr->pixAryUC[29][0] = 0;
   fontSTPtr->pixAryUC[29][1] = 0;
   fontSTPtr->pixAryUC[29][2] = 0;
   fontSTPtr->pixAryUC[29][3] = 128;
   fontSTPtr->pixAryUC[29][4] = 255;
   fontSTPtr->pixAryUC[29][5] = 225;
   fontSTPtr->pixAryUC[29][6] = 127;
   fontSTPtr->pixAryUC[29][7] = 0;
   fontSTPtr->pixAryUC[29][8] = 0;
   fontSTPtr->pixAryUC[29][9] = 0;
   fontSTPtr->pixAryUC[29][10] = 128;
   fontSTPtr->pixAryUC[29][11] = 255;
   fontSTPtr->pixAryUC[29][12] = 225;
   fontSTPtr->pixAryUC[29][13] = 127;
   fontSTPtr->pixAryUC[29][14] = 0;
   fontSTPtr->pixAryUC[29][15] = 0;
   fontSTPtr->pixAryUC[29][16] = 0;
   fontSTPtr->pixAryUC[29][17] = 0;
   fontSTPtr->pixAryUC[29][18] = 0;
   fontSTPtr->pixAryUC[29][19] = 0;
   fontSTPtr->pixAryUC[29][20] = 0;
   fontSTPtr->pixAryUC[29][21] = 0;
   fontSTPtr->pixAryUC[29][22] = 0;
   fontSTPtr->pixAryUC[29][23] = 0;
   fontSTPtr->pixAryUC[29][24] = 0;

   return 0;
} /*addAscii61_normalFont*/

/*___________________________________________
` Fun30: addAscii62_normalFont
`  - adds character `>` (ascii 62) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii62_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[30])
      free(fontSTPtr->pixAryUC[30]);

   fontSTPtr->pixAryUC[30] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[30])
      return 1;

   fontSTPtr->widthArySS[30] = 14;
   fontSTPtr->lenArySS[30] = 25;

   fontSTPtr->pixAryUC[30][0] = 0;
   fontSTPtr->pixAryUC[30][1] = 0;
   fontSTPtr->pixAryUC[30][2] = 3;
   fontSTPtr->pixAryUC[30][3] = 0;
   fontSTPtr->pixAryUC[30][4] = 3;
   fontSTPtr->pixAryUC[30][5] = 0;
   fontSTPtr->pixAryUC[30][6] = 3;
   fontSTPtr->pixAryUC[30][7] = 0;
   fontSTPtr->pixAryUC[30][8] = 3;
   fontSTPtr->pixAryUC[30][9] = 0;
   fontSTPtr->pixAryUC[30][10] = 3;
   fontSTPtr->pixAryUC[30][11] = 192;
   fontSTPtr->pixAryUC[30][12] = 0;
   fontSTPtr->pixAryUC[30][13] = 12;
   fontSTPtr->pixAryUC[30][14] = 192;
   fontSTPtr->pixAryUC[30][15] = 0;
   fontSTPtr->pixAryUC[30][16] = 12;
   fontSTPtr->pixAryUC[30][17] = 192;
   fontSTPtr->pixAryUC[30][18] = 0;
   fontSTPtr->pixAryUC[30][19] = 0;
   fontSTPtr->pixAryUC[30][20] = 0;
   fontSTPtr->pixAryUC[30][21] = 0;
   fontSTPtr->pixAryUC[30][22] = 0;
   fontSTPtr->pixAryUC[30][23] = 0;
   fontSTPtr->pixAryUC[30][24] = 0;

   return 0;
} /*addAscii62_normalFont*/

/*___________________________________________
` Fun31: addAscii63_normalFont
`  - adds character `?` (ascii 63) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii63_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[31])
      free(fontSTPtr->pixAryUC[31]);

   fontSTPtr->pixAryUC[31] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[31])
      return 1;

   fontSTPtr->widthArySS[31] = 14;
   fontSTPtr->lenArySS[31] = 25;

   fontSTPtr->pixAryUC[31][0] = 0;
   fontSTPtr->pixAryUC[31][1] = 0;
   fontSTPtr->pixAryUC[31][2] = 252;
   fontSTPtr->pixAryUC[31][3] = 128;
   fontSTPtr->pixAryUC[31][4] = 127;
   fontSTPtr->pixAryUC[31][5] = 96;
   fontSTPtr->pixAryUC[31][6] = 24;
   fontSTPtr->pixAryUC[31][7] = 16;
   fontSTPtr->pixAryUC[31][8] = 3;
   fontSTPtr->pixAryUC[31][9] = 96;
   fontSTPtr->pixAryUC[31][10] = 0;
   fontSTPtr->pixAryUC[31][11] = 24;
   fontSTPtr->pixAryUC[31][12] = 0;
   fontSTPtr->pixAryUC[31][13] = 6;
   fontSTPtr->pixAryUC[31][14] = 0;
   fontSTPtr->pixAryUC[31][15] = 0;
   fontSTPtr->pixAryUC[31][16] = 96;
   fontSTPtr->pixAryUC[31][17] = 0;
   fontSTPtr->pixAryUC[31][18] = 24;
   fontSTPtr->pixAryUC[31][19] = 0;
   fontSTPtr->pixAryUC[31][20] = 0;
   fontSTPtr->pixAryUC[31][21] = 0;
   fontSTPtr->pixAryUC[31][22] = 0;
   fontSTPtr->pixAryUC[31][23] = 0;
   fontSTPtr->pixAryUC[31][24] = 0;

   return 0;
} /*addAscii63_normalFont*/

/*___________________________________________
` Fun32: addAscii64_normalFont
`  - adds character `@` (ascii 64) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii64_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[32])
      free(fontSTPtr->pixAryUC[32]);

   fontSTPtr->pixAryUC[32] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[32])
      return 1;

   fontSTPtr->widthArySS[32] = 14;
   fontSTPtr->lenArySS[32] = 25;

   fontSTPtr->pixAryUC[32][0] = 0;
   fontSTPtr->pixAryUC[32][1] = 0;
   fontSTPtr->pixAryUC[32][2] = 0;
   fontSTPtr->pixAryUC[32][3] = 128;
   fontSTPtr->pixAryUC[32][4] = 63;
   fontSTPtr->pixAryUC[32][5] = 240;
   fontSTPtr->pixAryUC[32][6] = 31;
   fontSTPtr->pixAryUC[32][7] = 198;
   fontSTPtr->pixAryUC[32][8] = 141;
   fontSTPtr->pixAryUC[32][9] = 121;
   fontSTPtr->pixAryUC[32][10] = 99;
   fontSTPtr->pixAryUC[32][11] = 155;
   fontSTPtr->pixAryUC[32][12] = 89;
   fontSTPtr->pixAryUC[32][13] = 102;
   fontSTPtr->pixAryUC[32][14] = 182;
   fontSTPtr->pixAryUC[32][15] = 153;
   fontSTPtr->pixAryUC[32][16] = 217;
   fontSTPtr->pixAryUC[32][17] = 198;
   fontSTPtr->pixAryUC[32][18] = 230;
   fontSTPtr->pixAryUC[32][19] = 241;
   fontSTPtr->pixAryUC[32][20] = 3;
   fontSTPtr->pixAryUC[32][21] = 248;
   fontSTPtr->pixAryUC[32][22] = 1;
   fontSTPtr->pixAryUC[32][23] = 0;
   fontSTPtr->pixAryUC[32][24] = 0;

   return 0;
} /*addAscii64_normalFont*/

/*___________________________________________
` Fun33: addAscii65_normalFont
`  - adds character `A` (ascii 65) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii65_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[33])
      free(fontSTPtr->pixAryUC[33]);

   fontSTPtr->pixAryUC[33] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[33])
      return 1;

   fontSTPtr->widthArySS[33] = 14;
   fontSTPtr->lenArySS[33] = 25;

   fontSTPtr->pixAryUC[33][0] = 0;
   fontSTPtr->pixAryUC[33][1] = 0;
   fontSTPtr->pixAryUC[33][2] = 48;
   fontSTPtr->pixAryUC[33][3] = 0;
   fontSTPtr->pixAryUC[33][4] = 30;
   fontSTPtr->pixAryUC[33][5] = 192;
   fontSTPtr->pixAryUC[33][6] = 12;
   fontSTPtr->pixAryUC[33][7] = 24;
   fontSTPtr->pixAryUC[33][8] = 6;
   fontSTPtr->pixAryUC[33][9] = 255;
   fontSTPtr->pixAryUC[33][10] = 195;
   fontSTPtr->pixAryUC[33][11] = 255;
   fontSTPtr->pixAryUC[33][12] = 48;
   fontSTPtr->pixAryUC[33][13] = 48;
   fontSTPtr->pixAryUC[33][14] = 12;
   fontSTPtr->pixAryUC[33][15] = 140;
   fontSTPtr->pixAryUC[33][16] = 135;
   fontSTPtr->pixAryUC[33][17] = 231;
   fontSTPtr->pixAryUC[33][18] = 225;
   fontSTPtr->pixAryUC[33][19] = 1;
   fontSTPtr->pixAryUC[33][20] = 0;
   fontSTPtr->pixAryUC[33][21] = 0;
   fontSTPtr->pixAryUC[33][22] = 0;
   fontSTPtr->pixAryUC[33][23] = 0;
   fontSTPtr->pixAryUC[33][24] = 0;

   return 0;
} /*addAscii65_normalFont*/

/*___________________________________________
` Fun34: addAscii66_normalFont
`  - adds character `B` (ascii 66) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii66_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[34])
      free(fontSTPtr->pixAryUC[34]);

   fontSTPtr->pixAryUC[34] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[34])
      return 1;

   fontSTPtr->widthArySS[34] = 14;
   fontSTPtr->lenArySS[34] = 25;

   fontSTPtr->pixAryUC[34][0] = 0;
   fontSTPtr->pixAryUC[34][1] = 128;
   fontSTPtr->pixAryUC[34][2] = 255;
   fontSTPtr->pixAryUC[34][3] = 225;
   fontSTPtr->pixAryUC[34][4] = 255;
   fontSTPtr->pixAryUC[34][5] = 24;
   fontSTPtr->pixAryUC[34][6] = 96;
   fontSTPtr->pixAryUC[34][7] = 6;
   fontSTPtr->pixAryUC[34][8] = 152;
   fontSTPtr->pixAryUC[34][9] = 255;
   fontSTPtr->pixAryUC[34][10] = 227;
   fontSTPtr->pixAryUC[34][11] = 255;
   fontSTPtr->pixAryUC[34][12] = 24;
   fontSTPtr->pixAryUC[34][13] = 96;
   fontSTPtr->pixAryUC[34][14] = 6;
   fontSTPtr->pixAryUC[34][15] = 152;
   fontSTPtr->pixAryUC[34][16] = 255;
   fontSTPtr->pixAryUC[34][17] = 227;
   fontSTPtr->pixAryUC[34][18] = 127;
   fontSTPtr->pixAryUC[34][19] = 0;
   fontSTPtr->pixAryUC[34][20] = 0;
   fontSTPtr->pixAryUC[34][21] = 0;
   fontSTPtr->pixAryUC[34][22] = 0;
   fontSTPtr->pixAryUC[34][23] = 0;
   fontSTPtr->pixAryUC[34][24] = 0;

   return 0;
} /*addAscii66_normalFont*/

/*___________________________________________
` Fun35: addAscii67_normalFont
`  - adds character `C` (ascii 67) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii67_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[35])
      free(fontSTPtr->pixAryUC[35]);

   fontSTPtr->pixAryUC[35] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[35])
      return 1;

   fontSTPtr->widthArySS[35] = 14;
   fontSTPtr->lenArySS[35] = 25;

   fontSTPtr->pixAryUC[35][0] = 0;
   fontSTPtr->pixAryUC[35][1] = 0;
   fontSTPtr->pixAryUC[35][2] = 252;
   fontSTPtr->pixAryUC[35][3] = 129;
   fontSTPtr->pixAryUC[35][4] = 255;
   fontSTPtr->pixAryUC[35][5] = 112;
   fontSTPtr->pixAryUC[35][6] = 96;
   fontSTPtr->pixAryUC[35][7] = 6;
   fontSTPtr->pixAryUC[35][8] = 128;
   fontSTPtr->pixAryUC[35][9] = 0;
   fontSTPtr->pixAryUC[35][10] = 32;
   fontSTPtr->pixAryUC[35][11] = 0;
   fontSTPtr->pixAryUC[35][12] = 24;
   fontSTPtr->pixAryUC[35][13] = 0;
   fontSTPtr->pixAryUC[35][14] = 12;
   fontSTPtr->pixAryUC[35][15] = 24;
   fontSTPtr->pixAryUC[35][16] = 254;
   fontSTPtr->pixAryUC[35][17] = 3;
   fontSTPtr->pixAryUC[35][18] = 127;
   fontSTPtr->pixAryUC[35][19] = 0;
   fontSTPtr->pixAryUC[35][20] = 0;
   fontSTPtr->pixAryUC[35][21] = 0;
   fontSTPtr->pixAryUC[35][22] = 0;
   fontSTPtr->pixAryUC[35][23] = 0;
   fontSTPtr->pixAryUC[35][24] = 0;

   return 0;
} /*addAscii67_normalFont*/

/*___________________________________________
` Fun36: addAscii68_normalFont
`  - adds character `D` (ascii 68) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii68_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[36])
      free(fontSTPtr->pixAryUC[36]);

   fontSTPtr->pixAryUC[36] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[36])
      return 1;

   fontSTPtr->widthArySS[36] = 14;
   fontSTPtr->lenArySS[36] = 25;

   fontSTPtr->pixAryUC[36][0] = 0;
   fontSTPtr->pixAryUC[36][1] = 128;
   fontSTPtr->pixAryUC[36][2] = 127;
   fontSTPtr->pixAryUC[36][3] = 224;
   fontSTPtr->pixAryUC[36][4] = 63;
   fontSTPtr->pixAryUC[36][5] = 24;
   fontSTPtr->pixAryUC[36][6] = 24;
   fontSTPtr->pixAryUC[36][7] = 6;
   fontSTPtr->pixAryUC[36][8] = 140;
   fontSTPtr->pixAryUC[36][9] = 1;
   fontSTPtr->pixAryUC[36][10] = 102;
   fontSTPtr->pixAryUC[36][11] = 128;
   fontSTPtr->pixAryUC[36][12] = 25;
   fontSTPtr->pixAryUC[36][13] = 48;
   fontSTPtr->pixAryUC[36][14] = 6;
   fontSTPtr->pixAryUC[36][15] = 134;
   fontSTPtr->pixAryUC[36][16] = 255;
   fontSTPtr->pixAryUC[36][17] = 224;
   fontSTPtr->pixAryUC[36][18] = 31;
   fontSTPtr->pixAryUC[36][19] = 0;
   fontSTPtr->pixAryUC[36][20] = 0;
   fontSTPtr->pixAryUC[36][21] = 0;
   fontSTPtr->pixAryUC[36][22] = 0;
   fontSTPtr->pixAryUC[36][23] = 0;
   fontSTPtr->pixAryUC[36][24] = 0;

   return 0;
} /*addAscii68_normalFont*/

/*___________________________________________
` Fun37: addAscii69_normalFont
`  - adds character `E` (ascii 69) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii69_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[37])
      free(fontSTPtr->pixAryUC[37]);

   fontSTPtr->pixAryUC[37] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[37])
      return 1;

   fontSTPtr->widthArySS[37] = 14;
   fontSTPtr->lenArySS[37] = 25;

   fontSTPtr->pixAryUC[37][0] = 0;
   fontSTPtr->pixAryUC[37][1] = 128;
   fontSTPtr->pixAryUC[37][2] = 255;
   fontSTPtr->pixAryUC[37][3] = 231;
   fontSTPtr->pixAryUC[37][4] = 255;
   fontSTPtr->pixAryUC[37][5] = 25;
   fontSTPtr->pixAryUC[37][6] = 96;
   fontSTPtr->pixAryUC[37][7] = 134;
   fontSTPtr->pixAryUC[37][8] = 129;
   fontSTPtr->pixAryUC[37][9] = 127;
   fontSTPtr->pixAryUC[37][10] = 224;
   fontSTPtr->pixAryUC[37][11] = 31;
   fontSTPtr->pixAryUC[37][12] = 24;
   fontSTPtr->pixAryUC[37][13] = 6;
   fontSTPtr->pixAryUC[37][14] = 6;
   fontSTPtr->pixAryUC[37][15] = 152;
   fontSTPtr->pixAryUC[37][16] = 255;
   fontSTPtr->pixAryUC[37][17] = 231;
   fontSTPtr->pixAryUC[37][18] = 255;
   fontSTPtr->pixAryUC[37][19] = 1;
   fontSTPtr->pixAryUC[37][20] = 0;
   fontSTPtr->pixAryUC[37][21] = 0;
   fontSTPtr->pixAryUC[37][22] = 0;
   fontSTPtr->pixAryUC[37][23] = 0;
   fontSTPtr->pixAryUC[37][24] = 0;

   return 0;
} /*addAscii69_normalFont*/

/*___________________________________________
` Fun38: addAscii70_normalFont
`  - adds character `F` (ascii 70) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii70_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[38])
      free(fontSTPtr->pixAryUC[38]);

   fontSTPtr->pixAryUC[38] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[38])
      return 1;

   fontSTPtr->widthArySS[38] = 14;
   fontSTPtr->lenArySS[38] = 25;

   fontSTPtr->pixAryUC[38][0] = 0;
   fontSTPtr->pixAryUC[38][1] = 0;
   fontSTPtr->pixAryUC[38][2] = 254;
   fontSTPtr->pixAryUC[38][3] = 135;
   fontSTPtr->pixAryUC[38][4] = 255;
   fontSTPtr->pixAryUC[38][5] = 97;
   fontSTPtr->pixAryUC[38][6] = 96;
   fontSTPtr->pixAryUC[38][7] = 24;
   fontSTPtr->pixAryUC[38][8] = 3;
   fontSTPtr->pixAryUC[38][9] = 254;
   fontSTPtr->pixAryUC[38][10] = 128;
   fontSTPtr->pixAryUC[38][11] = 63;
   fontSTPtr->pixAryUC[38][12] = 96;
   fontSTPtr->pixAryUC[38][13] = 12;
   fontSTPtr->pixAryUC[38][14] = 24;
   fontSTPtr->pixAryUC[38][15] = 0;
   fontSTPtr->pixAryUC[38][16] = 15;
   fontSTPtr->pixAryUC[38][17] = 224;
   fontSTPtr->pixAryUC[38][18] = 7;
   fontSTPtr->pixAryUC[38][19] = 0;
   fontSTPtr->pixAryUC[38][20] = 0;
   fontSTPtr->pixAryUC[38][21] = 0;
   fontSTPtr->pixAryUC[38][22] = 0;
   fontSTPtr->pixAryUC[38][23] = 0;
   fontSTPtr->pixAryUC[38][24] = 0;

   return 0;
} /*addAscii70_normalFont*/

/*___________________________________________
` Fun39: addAscii71_normalFont
`  - adds character `G` (ascii 71) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii71_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[39])
      free(fontSTPtr->pixAryUC[39]);

   fontSTPtr->pixAryUC[39] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[39])
      return 1;

   fontSTPtr->widthArySS[39] = 14;
   fontSTPtr->lenArySS[39] = 25;

   fontSTPtr->pixAryUC[39][0] = 0;
   fontSTPtr->pixAryUC[39][1] = 0;
   fontSTPtr->pixAryUC[39][2] = 252;
   fontSTPtr->pixAryUC[39][3] = 129;
   fontSTPtr->pixAryUC[39][4] = 255;
   fontSTPtr->pixAryUC[39][5] = 48;
   fontSTPtr->pixAryUC[39][6] = 48;
   fontSTPtr->pixAryUC[39][7] = 6;
   fontSTPtr->pixAryUC[39][8] = 128;
   fontSTPtr->pixAryUC[39][9] = 224;
   fontSTPtr->pixAryUC[39][10] = 35;
   fontSTPtr->pixAryUC[39][11] = 252;
   fontSTPtr->pixAryUC[39][12] = 25;
   fontSTPtr->pixAryUC[39][13] = 48;
   fontSTPtr->pixAryUC[39][14] = 12;
   fontSTPtr->pixAryUC[39][15] = 12;
   fontSTPtr->pixAryUC[39][16] = 254;
   fontSTPtr->pixAryUC[39][17] = 1;
   fontSTPtr->pixAryUC[39][18] = 63;
   fontSTPtr->pixAryUC[39][19] = 0;
   fontSTPtr->pixAryUC[39][20] = 0;
   fontSTPtr->pixAryUC[39][21] = 0;
   fontSTPtr->pixAryUC[39][22] = 0;
   fontSTPtr->pixAryUC[39][23] = 0;
   fontSTPtr->pixAryUC[39][24] = 0;

   return 0;
} /*addAscii71_normalFont*/

/*___________________________________________
` Fun40: addAscii72_normalFont
`  - adds character `H` (ascii 72) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii72_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[40])
      free(fontSTPtr->pixAryUC[40]);

   fontSTPtr->pixAryUC[40] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[40])
      return 1;

   fontSTPtr->widthArySS[40] = 14;
   fontSTPtr->lenArySS[40] = 25;

   fontSTPtr->pixAryUC[40][0] = 0;
   fontSTPtr->pixAryUC[40][1] = 128;
   fontSTPtr->pixAryUC[40][2] = 207;
   fontSTPtr->pixAryUC[40][3] = 199;
   fontSTPtr->pixAryUC[40][4] = 225;
   fontSTPtr->pixAryUC[40][5] = 96;
   fontSTPtr->pixAryUC[40][6] = 24;
   fontSTPtr->pixAryUC[40][7] = 24;
   fontSTPtr->pixAryUC[40][8] = 6;
   fontSTPtr->pixAryUC[40][9] = 254;
   fontSTPtr->pixAryUC[40][10] = 129;
   fontSTPtr->pixAryUC[40][11] = 127;
   fontSTPtr->pixAryUC[40][12] = 96;
   fontSTPtr->pixAryUC[40][13] = 24;
   fontSTPtr->pixAryUC[40][14] = 24;
   fontSTPtr->pixAryUC[40][15] = 6;
   fontSTPtr->pixAryUC[40][16] = 135;
   fontSTPtr->pixAryUC[40][17] = 227;
   fontSTPtr->pixAryUC[40][18] = 243;
   fontSTPtr->pixAryUC[40][19] = 1;
   fontSTPtr->pixAryUC[40][20] = 0;
   fontSTPtr->pixAryUC[40][21] = 0;
   fontSTPtr->pixAryUC[40][22] = 0;
   fontSTPtr->pixAryUC[40][23] = 0;
   fontSTPtr->pixAryUC[40][24] = 0;

   return 0;
} /*addAscii72_normalFont*/

/*___________________________________________
` Fun41: addAscii73_normalFont
`  - adds character `I` (ascii 73) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii73_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[41])
      free(fontSTPtr->pixAryUC[41]);

   fontSTPtr->pixAryUC[41] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[41])
      return 1;

   fontSTPtr->widthArySS[41] = 14;
   fontSTPtr->lenArySS[41] = 25;

   fontSTPtr->pixAryUC[41][0] = 0;
   fontSTPtr->pixAryUC[41][1] = 128;
   fontSTPtr->pixAryUC[41][2] = 255;
   fontSTPtr->pixAryUC[41][3] = 199;
   fontSTPtr->pixAryUC[41][4] = 255;
   fontSTPtr->pixAryUC[41][5] = 0;
   fontSTPtr->pixAryUC[41][6] = 3;
   fontSTPtr->pixAryUC[41][7] = 192;
   fontSTPtr->pixAryUC[41][8] = 0;
   fontSTPtr->pixAryUC[41][9] = 48;
   fontSTPtr->pixAryUC[41][10] = 0;
   fontSTPtr->pixAryUC[41][11] = 12;
   fontSTPtr->pixAryUC[41][12] = 0;
   fontSTPtr->pixAryUC[41][13] = 3;
   fontSTPtr->pixAryUC[41][14] = 192;
   fontSTPtr->pixAryUC[41][15] = 0;
   fontSTPtr->pixAryUC[41][16] = 255;
   fontSTPtr->pixAryUC[41][17] = 227;
   fontSTPtr->pixAryUC[41][18] = 255;
   fontSTPtr->pixAryUC[41][19] = 1;
   fontSTPtr->pixAryUC[41][20] = 0;
   fontSTPtr->pixAryUC[41][21] = 0;
   fontSTPtr->pixAryUC[41][22] = 0;
   fontSTPtr->pixAryUC[41][23] = 0;
   fontSTPtr->pixAryUC[41][24] = 0;

   return 0;
} /*addAscii73_normalFont*/

/*___________________________________________
` Fun42: addAscii74_normalFont
`  - adds character `J` (ascii 74) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii74_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[42])
      free(fontSTPtr->pixAryUC[42]);

   fontSTPtr->pixAryUC[42] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[42])
      return 1;

   fontSTPtr->widthArySS[42] = 14;
   fontSTPtr->lenArySS[42] = 25;

   fontSTPtr->pixAryUC[42][0] = 0;
   fontSTPtr->pixAryUC[42][1] = 0;
   fontSTPtr->pixAryUC[42][2] = 255;
   fontSTPtr->pixAryUC[42][3] = 199;
   fontSTPtr->pixAryUC[42][4] = 255;
   fontSTPtr->pixAryUC[42][5] = 1;
   fontSTPtr->pixAryUC[42][6] = 6;
   fontSTPtr->pixAryUC[42][7] = 128;
   fontSTPtr->pixAryUC[42][8] = 1;
   fontSTPtr->pixAryUC[42][9] = 96;
   fontSTPtr->pixAryUC[42][10] = 0;
   fontSTPtr->pixAryUC[42][11] = 24;
   fontSTPtr->pixAryUC[42][12] = 0;
   fontSTPtr->pixAryUC[42][13] = 6;
   fontSTPtr->pixAryUC[42][14] = 198;
   fontSTPtr->pixAryUC[42][15] = 129;
   fontSTPtr->pixAryUC[42][16] = 127;
   fontSTPtr->pixAryUC[42][17] = 192;
   fontSTPtr->pixAryUC[42][18] = 15;
   fontSTPtr->pixAryUC[42][19] = 0;
   fontSTPtr->pixAryUC[42][20] = 0;
   fontSTPtr->pixAryUC[42][21] = 0;
   fontSTPtr->pixAryUC[42][22] = 0;
   fontSTPtr->pixAryUC[42][23] = 0;
   fontSTPtr->pixAryUC[42][24] = 0;

   return 0;
} /*addAscii74_normalFont*/

/*___________________________________________
` Fun43: addAscii75_normalFont
`  - adds character `K` (ascii 75) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii75_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[43])
      free(fontSTPtr->pixAryUC[43]);

   fontSTPtr->pixAryUC[43] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[43])
      return 1;

   fontSTPtr->widthArySS[43] = 14;
   fontSTPtr->lenArySS[43] = 25;

   fontSTPtr->pixAryUC[43][0] = 0;
   fontSTPtr->pixAryUC[43][1] = 128;
   fontSTPtr->pixAryUC[43][2] = 207;
   fontSTPtr->pixAryUC[43][3] = 199;
   fontSTPtr->pixAryUC[43][4] = 243;
   fontSTPtr->pixAryUC[43][5] = 96;
   fontSTPtr->pixAryUC[43][6] = 12;
   fontSTPtr->pixAryUC[43][7] = 152;
   fontSTPtr->pixAryUC[43][8] = 1;
   fontSTPtr->pixAryUC[43][9] = 62;
   fontSTPtr->pixAryUC[43][10] = 128;
   fontSTPtr->pixAryUC[43][11] = 15;
   fontSTPtr->pixAryUC[43][12] = 96;
   fontSTPtr->pixAryUC[43][13] = 6;
   fontSTPtr->pixAryUC[43][14] = 24;
   fontSTPtr->pixAryUC[43][15] = 3;
   fontSTPtr->pixAryUC[43][16] = 207;
   fontSTPtr->pixAryUC[43][17] = 227;
   fontSTPtr->pixAryUC[43][18] = 243;
   fontSTPtr->pixAryUC[43][19] = 1;
   fontSTPtr->pixAryUC[43][20] = 0;
   fontSTPtr->pixAryUC[43][21] = 0;
   fontSTPtr->pixAryUC[43][22] = 0;
   fontSTPtr->pixAryUC[43][23] = 0;
   fontSTPtr->pixAryUC[43][24] = 0;

   return 0;
} /*addAscii75_normalFont*/

/*___________________________________________
` Fun44: addAscii76_normalFont
`  - adds character `L` (ascii 76) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii76_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[44])
      free(fontSTPtr->pixAryUC[44]);

   fontSTPtr->pixAryUC[44] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[44])
      return 1;

   fontSTPtr->widthArySS[44] = 14;
   fontSTPtr->lenArySS[44] = 25;

   fontSTPtr->pixAryUC[44][0] = 0;
   fontSTPtr->pixAryUC[44][1] = 0;
   fontSTPtr->pixAryUC[44][2] = 127;
   fontSTPtr->pixAryUC[44][3] = 128;
   fontSTPtr->pixAryUC[44][4] = 15;
   fontSTPtr->pixAryUC[44][5] = 192;
   fontSTPtr->pixAryUC[44][6] = 0;
   fontSTPtr->pixAryUC[44][7] = 48;
   fontSTPtr->pixAryUC[44][8] = 0;
   fontSTPtr->pixAryUC[44][9] = 12;
   fontSTPtr->pixAryUC[44][10] = 0;
   fontSTPtr->pixAryUC[44][11] = 3;
   fontSTPtr->pixAryUC[44][12] = 192;
   fontSTPtr->pixAryUC[44][13] = 96;
   fontSTPtr->pixAryUC[44][14] = 56;
   fontSTPtr->pixAryUC[44][15] = 28;
   fontSTPtr->pixAryUC[44][16] = 255;
   fontSTPtr->pixAryUC[44][17] = 231;
   fontSTPtr->pixAryUC[44][18] = 255;
   fontSTPtr->pixAryUC[44][19] = 1;
   fontSTPtr->pixAryUC[44][20] = 0;
   fontSTPtr->pixAryUC[44][21] = 0;
   fontSTPtr->pixAryUC[44][22] = 0;
   fontSTPtr->pixAryUC[44][23] = 0;
   fontSTPtr->pixAryUC[44][24] = 0;

   return 0;
} /*addAscii76_normalFont*/

/*___________________________________________
` Fun45: addAscii77_normalFont
`  - adds character `M` (ascii 77) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii77_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[45])
      free(fontSTPtr->pixAryUC[45]);

   fontSTPtr->pixAryUC[45] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[45])
      return 1;

   fontSTPtr->widthArySS[45] = 14;
   fontSTPtr->lenArySS[45] = 25;

   fontSTPtr->pixAryUC[45][0] = 0;
   fontSTPtr->pixAryUC[45][1] = 0;
   fontSTPtr->pixAryUC[45][2] = 3;
   fontSTPtr->pixAryUC[45][3] = 195;
   fontSTPtr->pixAryUC[45][4] = 225;
   fontSTPtr->pixAryUC[45][5] = 240;
   fontSTPtr->pixAryUC[45][6] = 60;
   fontSTPtr->pixAryUC[45][7] = 252;
   fontSTPtr->pixAryUC[45][8] = 15;
   fontSTPtr->pixAryUC[45][9] = 123;
   fontSTPtr->pixAryUC[45][10] = 195;
   fontSTPtr->pixAryUC[45][11] = 204;
   fontSTPtr->pixAryUC[45][12] = 48;
   fontSTPtr->pixAryUC[45][13] = 48;
   fontSTPtr->pixAryUC[45][14] = 12;
   fontSTPtr->pixAryUC[45][15] = 12;
   fontSTPtr->pixAryUC[45][16] = 135;
   fontSTPtr->pixAryUC[45][17] = 227;
   fontSTPtr->pixAryUC[45][18] = 243;
   fontSTPtr->pixAryUC[45][19] = 1;
   fontSTPtr->pixAryUC[45][20] = 0;
   fontSTPtr->pixAryUC[45][21] = 0;
   fontSTPtr->pixAryUC[45][22] = 0;
   fontSTPtr->pixAryUC[45][23] = 0;
   fontSTPtr->pixAryUC[45][24] = 0;

   return 0;
} /*addAscii77_normalFont*/

/*___________________________________________
` Fun46: addAscii78_normalFont
`  - adds character `N` (ascii 78) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii78_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[46])
      free(fontSTPtr->pixAryUC[46]);

   fontSTPtr->pixAryUC[46] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[46])
      return 1;

   fontSTPtr->widthArySS[46] = 14;
   fontSTPtr->lenArySS[46] = 25;

   fontSTPtr->pixAryUC[46][0] = 0;
   fontSTPtr->pixAryUC[46][1] = 128;
   fontSTPtr->pixAryUC[46][2] = 1;
   fontSTPtr->pixAryUC[46][3] = 230;
   fontSTPtr->pixAryUC[46][4] = 128;
   fontSTPtr->pixAryUC[46][5] = 121;
   fontSTPtr->pixAryUC[46][6] = 96;
   fontSTPtr->pixAryUC[46][7] = 54;
   fontSTPtr->pixAryUC[46][8] = 152;
   fontSTPtr->pixAryUC[46][9] = 25;
   fontSTPtr->pixAryUC[46][10] = 102;
   fontSTPtr->pixAryUC[46][11] = 140;
   fontSTPtr->pixAryUC[46][12] = 25;
   fontSTPtr->pixAryUC[46][13] = 102;
   fontSTPtr->pixAryUC[46][14] = 6;
   fontSTPtr->pixAryUC[46][15] = 155;
   fontSTPtr->pixAryUC[46][16] = 129;
   fontSTPtr->pixAryUC[46][17] = 103;
   fontSTPtr->pixAryUC[46][18] = 192;
   fontSTPtr->pixAryUC[46][19] = 1;
   fontSTPtr->pixAryUC[46][20] = 0;
   fontSTPtr->pixAryUC[46][21] = 0;
   fontSTPtr->pixAryUC[46][22] = 0;
   fontSTPtr->pixAryUC[46][23] = 0;
   fontSTPtr->pixAryUC[46][24] = 0;

   return 0;
} /*addAscii78_normalFont*/

/*___________________________________________
` Fun47: addAscii79_normalFont
`  - adds character `O` (ascii 79) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii79_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[47])
      free(fontSTPtr->pixAryUC[47]);

   fontSTPtr->pixAryUC[47] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[47])
      return 1;

   fontSTPtr->widthArySS[47] = 14;
   fontSTPtr->lenArySS[47] = 25;

   fontSTPtr->pixAryUC[47][0] = 0;
   fontSTPtr->pixAryUC[47][1] = 0;
   fontSTPtr->pixAryUC[47][2] = 252;
   fontSTPtr->pixAryUC[47][3] = 128;
   fontSTPtr->pixAryUC[47][4] = 127;
   fontSTPtr->pixAryUC[47][5] = 48;
   fontSTPtr->pixAryUC[47][6] = 48;
   fontSTPtr->pixAryUC[47][7] = 6;
   fontSTPtr->pixAryUC[47][8] = 152;
   fontSTPtr->pixAryUC[47][9] = 0;
   fontSTPtr->pixAryUC[47][10] = 36;
   fontSTPtr->pixAryUC[47][11] = 0;
   fontSTPtr->pixAryUC[47][12] = 25;
   fontSTPtr->pixAryUC[47][13] = 96;
   fontSTPtr->pixAryUC[47][14] = 12;
   fontSTPtr->pixAryUC[47][15] = 12;
   fontSTPtr->pixAryUC[47][16] = 254;
   fontSTPtr->pixAryUC[47][17] = 1;
   fontSTPtr->pixAryUC[47][18] = 63;
   fontSTPtr->pixAryUC[47][19] = 0;
   fontSTPtr->pixAryUC[47][20] = 0;
   fontSTPtr->pixAryUC[47][21] = 0;
   fontSTPtr->pixAryUC[47][22] = 0;
   fontSTPtr->pixAryUC[47][23] = 0;
   fontSTPtr->pixAryUC[47][24] = 0;

   return 0;
} /*addAscii79_normalFont*/

/*___________________________________________
` Fun48: addAscii80_normalFont
`  - adds character `P` (ascii 80) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii80_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[48])
      free(fontSTPtr->pixAryUC[48]);

   fontSTPtr->pixAryUC[48] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[48])
      return 1;

   fontSTPtr->widthArySS[48] = 14;
   fontSTPtr->lenArySS[48] = 25;

   fontSTPtr->pixAryUC[48][0] = 0;
   fontSTPtr->pixAryUC[48][1] = 0;
   fontSTPtr->pixAryUC[48][2] = 254;
   fontSTPtr->pixAryUC[48][3] = 129;
   fontSTPtr->pixAryUC[48][4] = 255;
   fontSTPtr->pixAryUC[48][5] = 96;
   fontSTPtr->pixAryUC[48][6] = 96;
   fontSTPtr->pixAryUC[48][7] = 24;
   fontSTPtr->pixAryUC[48][8] = 24;
   fontSTPtr->pixAryUC[48][9] = 254;
   fontSTPtr->pixAryUC[48][10] = 131;
   fontSTPtr->pixAryUC[48][11] = 127;
   fontSTPtr->pixAryUC[48][12] = 96;
   fontSTPtr->pixAryUC[48][13] = 0;
   fontSTPtr->pixAryUC[48][14] = 24;
   fontSTPtr->pixAryUC[48][15] = 0;
   fontSTPtr->pixAryUC[48][16] = 31;
   fontSTPtr->pixAryUC[48][17] = 224;
   fontSTPtr->pixAryUC[48][18] = 15;
   fontSTPtr->pixAryUC[48][19] = 0;
   fontSTPtr->pixAryUC[48][20] = 0;
   fontSTPtr->pixAryUC[48][21] = 0;
   fontSTPtr->pixAryUC[48][22] = 0;
   fontSTPtr->pixAryUC[48][23] = 0;
   fontSTPtr->pixAryUC[48][24] = 0;

   return 0;
} /*addAscii80_normalFont*/

/*___________________________________________
` Fun49: addAscii81_normalFont
`  - adds character `Q` (ascii 81) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii81_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[49])
      free(fontSTPtr->pixAryUC[49]);

   fontSTPtr->pixAryUC[49] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[49])
      return 1;

   fontSTPtr->widthArySS[49] = 14;
   fontSTPtr->lenArySS[49] = 25;

   fontSTPtr->pixAryUC[49][0] = 0;
   fontSTPtr->pixAryUC[49][1] = 0;
   fontSTPtr->pixAryUC[49][2] = 124;
   fontSTPtr->pixAryUC[49][3] = 128;
   fontSTPtr->pixAryUC[49][4] = 63;
   fontSTPtr->pixAryUC[49][5] = 48;
   fontSTPtr->pixAryUC[49][6] = 24;
   fontSTPtr->pixAryUC[49][7] = 6;
   fontSTPtr->pixAryUC[49][8] = 140;
   fontSTPtr->pixAryUC[49][9] = 0;
   fontSTPtr->pixAryUC[49][10] = 34;
   fontSTPtr->pixAryUC[49][11] = 128;
   fontSTPtr->pixAryUC[49][12] = 152;
   fontSTPtr->pixAryUC[49][13] = 51;
   fontSTPtr->pixAryUC[49][14] = 188;
   fontSTPtr->pixAryUC[49][15] = 7;
   fontSTPtr->pixAryUC[49][16] = 254;
   fontSTPtr->pixAryUC[49][17] = 1;
   fontSTPtr->pixAryUC[49][18] = 223;
   fontSTPtr->pixAryUC[49][19] = 1;
   fontSTPtr->pixAryUC[49][20] = 96;
   fontSTPtr->pixAryUC[49][21] = 0;
   fontSTPtr->pixAryUC[49][22] = 0;
   fontSTPtr->pixAryUC[49][23] = 0;
   fontSTPtr->pixAryUC[49][24] = 0;

   return 0;
} /*addAscii81_normalFont*/

/*___________________________________________
` Fun50: addAscii82_normalFont
`  - adds character `R` (ascii 82) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii82_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[50])
      free(fontSTPtr->pixAryUC[50]);

   fontSTPtr->pixAryUC[50] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[50])
      return 1;

   fontSTPtr->widthArySS[50] = 14;
   fontSTPtr->lenArySS[50] = 25;

   fontSTPtr->pixAryUC[50][0] = 0;
   fontSTPtr->pixAryUC[50][1] = 128;
   fontSTPtr->pixAryUC[50][2] = 255;
   fontSTPtr->pixAryUC[50][3] = 224;
   fontSTPtr->pixAryUC[50][4] = 127;
   fontSTPtr->pixAryUC[50][5] = 24;
   fontSTPtr->pixAryUC[50][6] = 48;
   fontSTPtr->pixAryUC[50][7] = 6;
   fontSTPtr->pixAryUC[50][8] = 140;
   fontSTPtr->pixAryUC[50][9] = 255;
   fontSTPtr->pixAryUC[50][10] = 225;
   fontSTPtr->pixAryUC[50][11] = 63;
   fontSTPtr->pixAryUC[50][12] = 24;
   fontSTPtr->pixAryUC[50][13] = 12;
   fontSTPtr->pixAryUC[50][14] = 6;
   fontSTPtr->pixAryUC[50][15] = 134;
   fontSTPtr->pixAryUC[50][16] = 1;
   fontSTPtr->pixAryUC[50][17] = 99;
   fontSTPtr->pixAryUC[50][18] = 224;
   fontSTPtr->pixAryUC[50][19] = 1;
   fontSTPtr->pixAryUC[50][20] = 0;
   fontSTPtr->pixAryUC[50][21] = 0;
   fontSTPtr->pixAryUC[50][22] = 0;
   fontSTPtr->pixAryUC[50][23] = 0;
   fontSTPtr->pixAryUC[50][24] = 0;

   return 0;
} /*addAscii82_normalFont*/

/*___________________________________________
` Fun51: addAscii83_normalFont
`  - adds character `S` (ascii 83) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii83_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[51])
      free(fontSTPtr->pixAryUC[51]);

   fontSTPtr->pixAryUC[51] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[51])
      return 1;

   fontSTPtr->widthArySS[51] = 14;
   fontSTPtr->lenArySS[51] = 25;

   fontSTPtr->pixAryUC[51][0] = 0;
   fontSTPtr->pixAryUC[51][1] = 0;
   fontSTPtr->pixAryUC[51][2] = 254;
   fontSTPtr->pixAryUC[51][3] = 193;
   fontSTPtr->pixAryUC[51][4] = 255;
   fontSTPtr->pixAryUC[51][5] = 48;
   fontSTPtr->pixAryUC[51][6] = 96;
   fontSTPtr->pixAryUC[51][7] = 4;
   fontSTPtr->pixAryUC[51][8] = 0;
   fontSTPtr->pixAryUC[51][9] = 255;
   fontSTPtr->pixAryUC[51][10] = 129;
   fontSTPtr->pixAryUC[51][11] = 255;
   fontSTPtr->pixAryUC[51][12] = 0;
   fontSTPtr->pixAryUC[51][13] = 48;
   fontSTPtr->pixAryUC[51][14] = 6;
   fontSTPtr->pixAryUC[51][15] = 8;
   fontSTPtr->pixAryUC[51][16] = 255;
   fontSTPtr->pixAryUC[51][17] = 131;
   fontSTPtr->pixAryUC[51][18] = 127;
   fontSTPtr->pixAryUC[51][19] = 0;
   fontSTPtr->pixAryUC[51][20] = 0;
   fontSTPtr->pixAryUC[51][21] = 0;
   fontSTPtr->pixAryUC[51][22] = 0;
   fontSTPtr->pixAryUC[51][23] = 0;
   fontSTPtr->pixAryUC[51][24] = 0;

   return 0;
} /*addAscii83_normalFont*/

/*___________________________________________
` Fun52: addAscii84_normalFont
`  - adds character `T` (ascii 84) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii84_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[52])
      free(fontSTPtr->pixAryUC[52]);

   fontSTPtr->pixAryUC[52] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[52])
      return 1;

   fontSTPtr->widthArySS[52] = 14;
   fontSTPtr->lenArySS[52] = 25;

   fontSTPtr->pixAryUC[52][0] = 0;
   fontSTPtr->pixAryUC[52][1] = 128;
   fontSTPtr->pixAryUC[52][2] = 255;
   fontSTPtr->pixAryUC[52][3] = 231;
   fontSTPtr->pixAryUC[52][4] = 255;
   fontSTPtr->pixAryUC[52][5] = 1;
   fontSTPtr->pixAryUC[52][6] = 3;
   fontSTPtr->pixAryUC[52][7] = 192;
   fontSTPtr->pixAryUC[52][8] = 0;
   fontSTPtr->pixAryUC[52][9] = 48;
   fontSTPtr->pixAryUC[52][10] = 0;
   fontSTPtr->pixAryUC[52][11] = 12;
   fontSTPtr->pixAryUC[52][12] = 0;
   fontSTPtr->pixAryUC[52][13] = 3;
   fontSTPtr->pixAryUC[52][14] = 192;
   fontSTPtr->pixAryUC[52][15] = 0;
   fontSTPtr->pixAryUC[52][16] = 48;
   fontSTPtr->pixAryUC[52][17] = 0;
   fontSTPtr->pixAryUC[52][18] = 12;
   fontSTPtr->pixAryUC[52][19] = 0;
   fontSTPtr->pixAryUC[52][20] = 0;
   fontSTPtr->pixAryUC[52][21] = 0;
   fontSTPtr->pixAryUC[52][22] = 0;
   fontSTPtr->pixAryUC[52][23] = 0;
   fontSTPtr->pixAryUC[52][24] = 0;

   return 0;
} /*addAscii84_normalFont*/

/*___________________________________________
` Fun53: addAscii85_normalFont
`  - adds character `U` (ascii 85) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii85_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[53])
      free(fontSTPtr->pixAryUC[53]);

   fontSTPtr->pixAryUC[53] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[53])
      return 1;

   fontSTPtr->widthArySS[53] = 14;
   fontSTPtr->lenArySS[53] = 25;

   fontSTPtr->pixAryUC[53][0] = 0;
   fontSTPtr->pixAryUC[53][1] = 128;
   fontSTPtr->pixAryUC[53][2] = 1;
   fontSTPtr->pixAryUC[53][3] = 99;
   fontSTPtr->pixAryUC[53][4] = 192;
   fontSTPtr->pixAryUC[53][5] = 24;
   fontSTPtr->pixAryUC[53][6] = 48;
   fontSTPtr->pixAryUC[53][7] = 6;
   fontSTPtr->pixAryUC[53][8] = 140;
   fontSTPtr->pixAryUC[53][9] = 1;
   fontSTPtr->pixAryUC[53][10] = 99;
   fontSTPtr->pixAryUC[53][11] = 192;
   fontSTPtr->pixAryUC[53][12] = 24;
   fontSTPtr->pixAryUC[53][13] = 48;
   fontSTPtr->pixAryUC[53][14] = 14;
   fontSTPtr->pixAryUC[53][15] = 14;
   fontSTPtr->pixAryUC[53][16] = 255;
   fontSTPtr->pixAryUC[53][17] = 129;
   fontSTPtr->pixAryUC[53][18] = 63;
   fontSTPtr->pixAryUC[53][19] = 0;
   fontSTPtr->pixAryUC[53][20] = 0;
   fontSTPtr->pixAryUC[53][21] = 0;
   fontSTPtr->pixAryUC[53][22] = 0;
   fontSTPtr->pixAryUC[53][23] = 0;
   fontSTPtr->pixAryUC[53][24] = 0;

   return 0;
} /*addAscii85_normalFont*/

/*___________________________________________
` Fun54: addAscii86_normalFont
`  - adds character `V` (ascii 86) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii86_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[54])
      free(fontSTPtr->pixAryUC[54]);

   fontSTPtr->pixAryUC[54] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[54])
      return 1;

   fontSTPtr->widthArySS[54] = 14;
   fontSTPtr->lenArySS[54] = 25;

   fontSTPtr->pixAryUC[54][0] = 0;
   fontSTPtr->pixAryUC[54][1] = 0;
   fontSTPtr->pixAryUC[54][2] = 6;
   fontSTPtr->pixAryUC[54][3] = 131;
   fontSTPtr->pixAryUC[54][4] = 193;
   fontSTPtr->pixAryUC[54][5] = 96;
   fontSTPtr->pixAryUC[54][6] = 48;
   fontSTPtr->pixAryUC[54][7] = 24;
   fontSTPtr->pixAryUC[54][8] = 12;
   fontSTPtr->pixAryUC[54][9] = 6;
   fontSTPtr->pixAryUC[54][10] = 131;
   fontSTPtr->pixAryUC[54][11] = 227;
   fontSTPtr->pixAryUC[54][12] = 192;
   fontSTPtr->pixAryUC[54][13] = 29;
   fontSTPtr->pixAryUC[54][14] = 224;
   fontSTPtr->pixAryUC[54][15] = 3;
   fontSTPtr->pixAryUC[54][16] = 112;
   fontSTPtr->pixAryUC[54][17] = 0;
   fontSTPtr->pixAryUC[54][18] = 8;
   fontSTPtr->pixAryUC[54][19] = 0;
   fontSTPtr->pixAryUC[54][20] = 0;
   fontSTPtr->pixAryUC[54][21] = 0;
   fontSTPtr->pixAryUC[54][22] = 0;
   fontSTPtr->pixAryUC[54][23] = 0;
   fontSTPtr->pixAryUC[54][24] = 0;

   return 0;
} /*addAscii86_normalFont*/

/*___________________________________________
` Fun55: addAscii87_normalFont
`  - adds character `W` (ascii 87) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii87_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[55])
      free(fontSTPtr->pixAryUC[55]);

   fontSTPtr->pixAryUC[55] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[55])
      return 1;

   fontSTPtr->widthArySS[55] = 14;
   fontSTPtr->lenArySS[55] = 25;

   fontSTPtr->pixAryUC[55][0] = 0;
   fontSTPtr->pixAryUC[55][1] = 0;
   fontSTPtr->pixAryUC[55][2] = 3;
   fontSTPtr->pixAryUC[55][3] = 195;
   fontSTPtr->pixAryUC[55][4] = 192;
   fontSTPtr->pixAryUC[55][5] = 48;
   fontSTPtr->pixAryUC[55][6] = 48;
   fontSTPtr->pixAryUC[55][7] = 12;
   fontSTPtr->pixAryUC[55][8] = 12;
   fontSTPtr->pixAryUC[55][9] = 51;
   fontSTPtr->pixAryUC[55][10] = 195;
   fontSTPtr->pixAryUC[55][11] = 222;
   fontSTPtr->pixAryUC[55][12] = 240;
   fontSTPtr->pixAryUC[55][13] = 63;
   fontSTPtr->pixAryUC[55][14] = 60;
   fontSTPtr->pixAryUC[55][15] = 15;
   fontSTPtr->pixAryUC[55][16] = 135;
   fontSTPtr->pixAryUC[55][17] = 195;
   fontSTPtr->pixAryUC[55][18] = 192;
   fontSTPtr->pixAryUC[55][19] = 0;
   fontSTPtr->pixAryUC[55][20] = 0;
   fontSTPtr->pixAryUC[55][21] = 0;
   fontSTPtr->pixAryUC[55][22] = 0;
   fontSTPtr->pixAryUC[55][23] = 0;
   fontSTPtr->pixAryUC[55][24] = 0;

   return 0;
} /*addAscii87_normalFont*/

/*___________________________________________
` Fun56: addAscii88_normalFont
`  - adds character `X` (ascii 88) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii88_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[56])
      free(fontSTPtr->pixAryUC[56]);

   fontSTPtr->pixAryUC[56] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[56])
      return 1;

   fontSTPtr->widthArySS[56] = 14;
   fontSTPtr->lenArySS[56] = 25;

   fontSTPtr->pixAryUC[56][0] = 0;
   fontSTPtr->pixAryUC[56][1] = 128;
   fontSTPtr->pixAryUC[56][2] = 239;
   fontSTPtr->pixAryUC[56][3] = 131;
   fontSTPtr->pixAryUC[56][4] = 49;
   fontSTPtr->pixAryUC[56][5] = 192;
   fontSTPtr->pixAryUC[56][6] = 6;
   fontSTPtr->pixAryUC[56][7] = 224;
   fontSTPtr->pixAryUC[56][8] = 0;
   fontSTPtr->pixAryUC[56][9] = 16;
   fontSTPtr->pixAryUC[56][10] = 0;
   fontSTPtr->pixAryUC[56][11] = 14;
   fontSTPtr->pixAryUC[56][12] = 192;
   fontSTPtr->pixAryUC[56][13] = 6;
   fontSTPtr->pixAryUC[56][14] = 24;
   fontSTPtr->pixAryUC[56][15] = 3;
   fontSTPtr->pixAryUC[56][16] = 131;
   fontSTPtr->pixAryUC[56][17] = 225;
   fontSTPtr->pixAryUC[56][18] = 241;
   fontSTPtr->pixAryUC[56][19] = 0;
   fontSTPtr->pixAryUC[56][20] = 0;
   fontSTPtr->pixAryUC[56][21] = 0;
   fontSTPtr->pixAryUC[56][22] = 0;
   fontSTPtr->pixAryUC[56][23] = 0;
   fontSTPtr->pixAryUC[56][24] = 0;

   return 0;
} /*addAscii88_normalFont*/

/*___________________________________________
` Fun57: addAscii89_normalFont
`  - adds character `Y` (ascii 89) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii89_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[57])
      free(fontSTPtr->pixAryUC[57]);

   fontSTPtr->pixAryUC[57] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[57])
      return 1;

   fontSTPtr->widthArySS[57] = 14;
   fontSTPtr->lenArySS[57] = 25;

   fontSTPtr->pixAryUC[57][0] = 0;
   fontSTPtr->pixAryUC[57][1] = 128;
   fontSTPtr->pixAryUC[57][2] = 135;
   fontSTPtr->pixAryUC[57][3] = 231;
   fontSTPtr->pixAryUC[57][4] = 225;
   fontSTPtr->pixAryUC[57][5] = 49;
   fontSTPtr->pixAryUC[57][6] = 48;
   fontSTPtr->pixAryUC[57][7] = 24;
   fontSTPtr->pixAryUC[57][8] = 6;
   fontSTPtr->pixAryUC[57][9] = 252;
   fontSTPtr->pixAryUC[57][10] = 0;
   fontSTPtr->pixAryUC[57][11] = 30;
   fontSTPtr->pixAryUC[57][12] = 0;
   fontSTPtr->pixAryUC[57][13] = 3;
   fontSTPtr->pixAryUC[57][14] = 192;
   fontSTPtr->pixAryUC[57][15] = 0;
   fontSTPtr->pixAryUC[57][16] = 120;
   fontSTPtr->pixAryUC[57][17] = 0;
   fontSTPtr->pixAryUC[57][18] = 63;
   fontSTPtr->pixAryUC[57][19] = 0;
   fontSTPtr->pixAryUC[57][20] = 0;
   fontSTPtr->pixAryUC[57][21] = 0;
   fontSTPtr->pixAryUC[57][22] = 0;
   fontSTPtr->pixAryUC[57][23] = 0;
   fontSTPtr->pixAryUC[57][24] = 0;

   return 0;
} /*addAscii89_normalFont*/

/*___________________________________________
` Fun58: addAscii90_normalFont
`  - adds character `Z` (ascii 90) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii90_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[58])
      free(fontSTPtr->pixAryUC[58]);

   fontSTPtr->pixAryUC[58] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[58])
      return 1;

   fontSTPtr->widthArySS[58] = 14;
   fontSTPtr->lenArySS[58] = 25;

   fontSTPtr->pixAryUC[58][0] = 0;
   fontSTPtr->pixAryUC[58][1] = 128;
   fontSTPtr->pixAryUC[58][2] = 255;
   fontSTPtr->pixAryUC[58][3] = 231;
   fontSTPtr->pixAryUC[58][4] = 255;
   fontSTPtr->pixAryUC[58][5] = 0;
   fontSTPtr->pixAryUC[58][6] = 24;
   fontSTPtr->pixAryUC[58][7] = 0;
   fontSTPtr->pixAryUC[58][8] = 131;
   fontSTPtr->pixAryUC[58][9] = 111;
   fontSTPtr->pixAryUC[58][10] = 231;
   fontSTPtr->pixAryUC[58][11] = 237;
   fontSTPtr->pixAryUC[58][12] = 129;
   fontSTPtr->pixAryUC[58][13] = 1;
   fontSTPtr->pixAryUC[58][14] = 48;
   fontSTPtr->pixAryUC[58][15] = 0;
   fontSTPtr->pixAryUC[58][16] = 255;
   fontSTPtr->pixAryUC[58][17] = 231;
   fontSTPtr->pixAryUC[58][18] = 255;
   fontSTPtr->pixAryUC[58][19] = 1;
   fontSTPtr->pixAryUC[58][20] = 0;
   fontSTPtr->pixAryUC[58][21] = 0;
   fontSTPtr->pixAryUC[58][22] = 0;
   fontSTPtr->pixAryUC[58][23] = 0;
   fontSTPtr->pixAryUC[58][24] = 0;

   return 0;
} /*addAscii90_normalFont*/

/*___________________________________________
` Fun59: addAscii91_normalFont
`  - adds character `[` (ascii 91) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii91_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[59])
      free(fontSTPtr->pixAryUC[59]);

   fontSTPtr->pixAryUC[59] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[59])
      return 1;

   fontSTPtr->widthArySS[59] = 14;
   fontSTPtr->lenArySS[59] = 25;

   fontSTPtr->pixAryUC[59][0] = 0;
   fontSTPtr->pixAryUC[59][1] = 0;
   fontSTPtr->pixAryUC[59][2] = 254;
   fontSTPtr->pixAryUC[59][3] = 129;
   fontSTPtr->pixAryUC[59][4] = 127;
   fontSTPtr->pixAryUC[59][5] = 96;
   fontSTPtr->pixAryUC[59][6] = 0;
   fontSTPtr->pixAryUC[59][7] = 24;
   fontSTPtr->pixAryUC[59][8] = 0;
   fontSTPtr->pixAryUC[59][9] = 6;
   fontSTPtr->pixAryUC[59][10] = 128;
   fontSTPtr->pixAryUC[59][11] = 1;
   fontSTPtr->pixAryUC[59][12] = 96;
   fontSTPtr->pixAryUC[59][13] = 0;
   fontSTPtr->pixAryUC[59][14] = 24;
   fontSTPtr->pixAryUC[59][15] = 0;
   fontSTPtr->pixAryUC[59][16] = 254;
   fontSTPtr->pixAryUC[59][17] = 129;
   fontSTPtr->pixAryUC[59][18] = 127;
   fontSTPtr->pixAryUC[59][19] = 0;
   fontSTPtr->pixAryUC[59][20] = 0;
   fontSTPtr->pixAryUC[59][21] = 0;
   fontSTPtr->pixAryUC[59][22] = 0;
   fontSTPtr->pixAryUC[59][23] = 0;
   fontSTPtr->pixAryUC[59][24] = 0;

   return 0;
} /*addAscii91_normalFont*/

/*___________________________________________
` Fun60: addAscii92_normalFont
`  - adds character `\` (ascii 92) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii92_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[60])
      free(fontSTPtr->pixAryUC[60]);

   fontSTPtr->pixAryUC[60] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[60])
      return 1;

   fontSTPtr->widthArySS[60] = 14;
   fontSTPtr->lenArySS[60] = 25;

   fontSTPtr->pixAryUC[60][0] = 0;
   fontSTPtr->pixAryUC[60][1] = 128;
   fontSTPtr->pixAryUC[60][2] = 1;
   fontSTPtr->pixAryUC[60][3] = 192;
   fontSTPtr->pixAryUC[60][4] = 0;
   fontSTPtr->pixAryUC[60][5] = 96;
   fontSTPtr->pixAryUC[60][6] = 0;
   fontSTPtr->pixAryUC[60][7] = 48;
   fontSTPtr->pixAryUC[60][8] = 0;
   fontSTPtr->pixAryUC[60][9] = 24;
   fontSTPtr->pixAryUC[60][10] = 0;
   fontSTPtr->pixAryUC[60][11] = 12;
   fontSTPtr->pixAryUC[60][12] = 0;
   fontSTPtr->pixAryUC[60][13] = 6;
   fontSTPtr->pixAryUC[60][14] = 0;
   fontSTPtr->pixAryUC[60][15] = 3;
   fontSTPtr->pixAryUC[60][16] = 128;
   fontSTPtr->pixAryUC[60][17] = 1;
   fontSTPtr->pixAryUC[60][18] = 192;
   fontSTPtr->pixAryUC[60][19] = 0;
   fontSTPtr->pixAryUC[60][20] = 0;
   fontSTPtr->pixAryUC[60][21] = 0;
   fontSTPtr->pixAryUC[60][22] = 0;
   fontSTPtr->pixAryUC[60][23] = 0;
   fontSTPtr->pixAryUC[60][24] = 0;

   return 0;
} /*addAscii92_normalFont*/

/*___________________________________________
` Fun61: addAscii93_normalFont
`  - adds character `]` (ascii 93) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii93_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[61])
      free(fontSTPtr->pixAryUC[61]);

   fontSTPtr->pixAryUC[61] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[61])
      return 1;

   fontSTPtr->widthArySS[61] = 14;
   fontSTPtr->lenArySS[61] = 25;

   fontSTPtr->pixAryUC[61][0] = 0;
   fontSTPtr->pixAryUC[61][1] = 0;
   fontSTPtr->pixAryUC[61][2] = 248;
   fontSTPtr->pixAryUC[61][3] = 7;
   fontSTPtr->pixAryUC[61][4] = 254;
   fontSTPtr->pixAryUC[61][5] = 1;
   fontSTPtr->pixAryUC[61][6] = 96;
   fontSTPtr->pixAryUC[61][7] = 0;
   fontSTPtr->pixAryUC[61][8] = 24;
   fontSTPtr->pixAryUC[61][9] = 0;
   fontSTPtr->pixAryUC[61][10] = 6;
   fontSTPtr->pixAryUC[61][11] = 128;
   fontSTPtr->pixAryUC[61][12] = 1;
   fontSTPtr->pixAryUC[61][13] = 96;
   fontSTPtr->pixAryUC[61][14] = 0;
   fontSTPtr->pixAryUC[61][15] = 24;
   fontSTPtr->pixAryUC[61][16] = 248;
   fontSTPtr->pixAryUC[61][17] = 7;
   fontSTPtr->pixAryUC[61][18] = 254;
   fontSTPtr->pixAryUC[61][19] = 1;
   fontSTPtr->pixAryUC[61][20] = 0;
   fontSTPtr->pixAryUC[61][21] = 0;
   fontSTPtr->pixAryUC[61][22] = 0;
   fontSTPtr->pixAryUC[61][23] = 0;
   fontSTPtr->pixAryUC[61][24] = 0;

   return 0;
} /*addAscii93_normalFont*/

/*___________________________________________
` Fun62: addAscii94_normalFont
`  - adds character `^` (ascii 94) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii94_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[62])
      free(fontSTPtr->pixAryUC[62]);

   fontSTPtr->pixAryUC[62] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[62])
      return 1;

   fontSTPtr->widthArySS[62] = 14;
   fontSTPtr->lenArySS[62] = 25;

   fontSTPtr->pixAryUC[62][0] = 0;
   fontSTPtr->pixAryUC[62][1] = 0;
   fontSTPtr->pixAryUC[62][2] = 16;
   fontSTPtr->pixAryUC[62][3] = 0;
   fontSTPtr->pixAryUC[62][4] = 14;
   fontSTPtr->pixAryUC[62][5] = 192;
   fontSTPtr->pixAryUC[62][6] = 6;
   fontSTPtr->pixAryUC[62][7] = 24;
   fontSTPtr->pixAryUC[62][8] = 3;
   fontSTPtr->pixAryUC[62][9] = 131;
   fontSTPtr->pixAryUC[62][10] = 1;
   fontSTPtr->pixAryUC[62][11] = 0;
   fontSTPtr->pixAryUC[62][12] = 0;
   fontSTPtr->pixAryUC[62][13] = 0;
   fontSTPtr->pixAryUC[62][14] = 0;
   fontSTPtr->pixAryUC[62][15] = 0;
   fontSTPtr->pixAryUC[62][16] = 0;
   fontSTPtr->pixAryUC[62][17] = 0;
   fontSTPtr->pixAryUC[62][18] = 0;
   fontSTPtr->pixAryUC[62][19] = 0;
   fontSTPtr->pixAryUC[62][20] = 0;
   fontSTPtr->pixAryUC[62][21] = 0;
   fontSTPtr->pixAryUC[62][22] = 0;
   fontSTPtr->pixAryUC[62][23] = 0;
   fontSTPtr->pixAryUC[62][24] = 0;

   return 0;
} /*addAscii94_normalFont*/

/*___________________________________________
` Fun63: addAscii95_normalFont
`  - adds character `_` (ascii 95) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii95_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[63])
      free(fontSTPtr->pixAryUC[63]);

   fontSTPtr->pixAryUC[63] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[63])
      return 1;

   fontSTPtr->widthArySS[63] = 14;
   fontSTPtr->lenArySS[63] = 25;

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
   fontSTPtr->pixAryUC[63][15] = 0;
   fontSTPtr->pixAryUC[63][16] = 0;
   fontSTPtr->pixAryUC[63][17] = 0;
   fontSTPtr->pixAryUC[63][18] = 0;
   fontSTPtr->pixAryUC[63][19] = 0;
   fontSTPtr->pixAryUC[63][20] = 0;
   fontSTPtr->pixAryUC[63][21] = 255;
   fontSTPtr->pixAryUC[63][22] = 207;
   fontSTPtr->pixAryUC[63][23] = 255;
   fontSTPtr->pixAryUC[63][24] = 3;

   return 0;
} /*addAscii95_normalFont*/

/*___________________________________________
` Fun64: addAscii96_normalFont
`  - adds character ``` (ascii 96) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii96_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[64])
      free(fontSTPtr->pixAryUC[64]);

   fontSTPtr->pixAryUC[64] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[64])
      return 1;

   fontSTPtr->widthArySS[64] = 14;
   fontSTPtr->lenArySS[64] = 25;

   fontSTPtr->pixAryUC[64][0] = 0;
   fontSTPtr->pixAryUC[64][1] = 0;
   fontSTPtr->pixAryUC[64][2] = 48;
   fontSTPtr->pixAryUC[64][3] = 0;
   fontSTPtr->pixAryUC[64][4] = 24;
   fontSTPtr->pixAryUC[64][5] = 0;
   fontSTPtr->pixAryUC[64][6] = 12;
   fontSTPtr->pixAryUC[64][7] = 0;
   fontSTPtr->pixAryUC[64][8] = 3;
   fontSTPtr->pixAryUC[64][9] = 0;
   fontSTPtr->pixAryUC[64][10] = 0;
   fontSTPtr->pixAryUC[64][11] = 0;
   fontSTPtr->pixAryUC[64][12] = 0;
   fontSTPtr->pixAryUC[64][13] = 0;
   fontSTPtr->pixAryUC[64][14] = 0;
   fontSTPtr->pixAryUC[64][15] = 0;
   fontSTPtr->pixAryUC[64][16] = 0;
   fontSTPtr->pixAryUC[64][17] = 0;
   fontSTPtr->pixAryUC[64][18] = 0;
   fontSTPtr->pixAryUC[64][19] = 0;
   fontSTPtr->pixAryUC[64][20] = 0;
   fontSTPtr->pixAryUC[64][21] = 0;
   fontSTPtr->pixAryUC[64][22] = 0;
   fontSTPtr->pixAryUC[64][23] = 0;
   fontSTPtr->pixAryUC[64][24] = 0;

   return 0;
} /*addAscii96_normalFont*/

/*___________________________________________
` Fun65: addAscii97_normalFont
`  - adds character `a` (ascii 97) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii97_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[65])
      free(fontSTPtr->pixAryUC[65]);

   fontSTPtr->pixAryUC[65] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[65])
      return 1;

   fontSTPtr->widthArySS[65] = 14;
   fontSTPtr->lenArySS[65] = 25;

   fontSTPtr->pixAryUC[65][0] = 0;
   fontSTPtr->pixAryUC[65][1] = 0;
   fontSTPtr->pixAryUC[65][2] = 0;
   fontSTPtr->pixAryUC[65][3] = 128;
   fontSTPtr->pixAryUC[65][4] = 15;
   fontSTPtr->pixAryUC[65][5] = 240;
   fontSTPtr->pixAryUC[65][6] = 7;
   fontSTPtr->pixAryUC[65][7] = 6;
   fontSTPtr->pixAryUC[65][8] = 3;
   fontSTPtr->pixAryUC[65][9] = 254;
   fontSTPtr->pixAryUC[65][10] = 192;
   fontSTPtr->pixAryUC[65][11] = 48;
   fontSTPtr->pixAryUC[65][12] = 24;
   fontSTPtr->pixAryUC[65][13] = 12;
   fontSTPtr->pixAryUC[65][14] = 12;
   fontSTPtr->pixAryUC[65][15] = 27;
   fontSTPtr->pixAryUC[65][16] = 127;
   fontSTPtr->pixAryUC[65][17] = 134;
   fontSTPtr->pixAryUC[65][18] = 243;
   fontSTPtr->pixAryUC[65][19] = 0;
   fontSTPtr->pixAryUC[65][20] = 0;
   fontSTPtr->pixAryUC[65][21] = 0;
   fontSTPtr->pixAryUC[65][22] = 0;
   fontSTPtr->pixAryUC[65][23] = 0;
   fontSTPtr->pixAryUC[65][24] = 0;

   return 0;
} /*addAscii97_normalFont*/

/*___________________________________________
` Fun66: addAscii98_normalFont
`  - adds character `b` (ascii 98) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii98_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[66])
      free(fontSTPtr->pixAryUC[66]);

   fontSTPtr->pixAryUC[66] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[66])
      return 1;

   fontSTPtr->widthArySS[66] = 14;
   fontSTPtr->lenArySS[66] = 25;

   fontSTPtr->pixAryUC[66][0] = 0;
   fontSTPtr->pixAryUC[66][1] = 0;
   fontSTPtr->pixAryUC[66][2] = 7;
   fontSTPtr->pixAryUC[66][3] = 224;
   fontSTPtr->pixAryUC[66][4] = 1;
   fontSTPtr->pixAryUC[66][5] = 96;
   fontSTPtr->pixAryUC[66][6] = 0;
   fontSTPtr->pixAryUC[66][7] = 216;
   fontSTPtr->pixAryUC[66][8] = 3;
   fontSTPtr->pixAryUC[66][9] = 254;
   fontSTPtr->pixAryUC[66][10] = 129;
   fontSTPtr->pixAryUC[66][11] = 193;
   fontSTPtr->pixAryUC[66][12] = 32;
   fontSTPtr->pixAryUC[66][13] = 96;
   fontSTPtr->pixAryUC[66][14] = 24;
   fontSTPtr->pixAryUC[66][15] = 12;
   fontSTPtr->pixAryUC[66][16] = 254;
   fontSTPtr->pixAryUC[66][17] = 1;
   fontSTPtr->pixAryUC[66][18] = 63;
   fontSTPtr->pixAryUC[66][19] = 0;
   fontSTPtr->pixAryUC[66][20] = 0;
   fontSTPtr->pixAryUC[66][21] = 0;
   fontSTPtr->pixAryUC[66][22] = 0;
   fontSTPtr->pixAryUC[66][23] = 0;
   fontSTPtr->pixAryUC[66][24] = 0;

   return 0;
} /*addAscii98_normalFont*/

/*___________________________________________
` Fun67: addAscii99_normalFont
`  - adds character `c` (ascii 99) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii99_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[67])
      free(fontSTPtr->pixAryUC[67]);

   fontSTPtr->pixAryUC[67] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[67])
      return 1;

   fontSTPtr->widthArySS[67] = 14;
   fontSTPtr->lenArySS[67] = 25;

   fontSTPtr->pixAryUC[67][0] = 0;
   fontSTPtr->pixAryUC[67][1] = 0;
   fontSTPtr->pixAryUC[67][2] = 0;
   fontSTPtr->pixAryUC[67][3] = 0;
   fontSTPtr->pixAryUC[67][4] = 62;
   fontSTPtr->pixAryUC[67][5] = 192;
   fontSTPtr->pixAryUC[67][6] = 31;
   fontSTPtr->pixAryUC[67][7] = 24;
   fontSTPtr->pixAryUC[67][8] = 12;
   fontSTPtr->pixAryUC[67][9] = 3;
   fontSTPtr->pixAryUC[67][10] = 64;
   fontSTPtr->pixAryUC[67][11] = 0;
   fontSTPtr->pixAryUC[67][12] = 48;
   fontSTPtr->pixAryUC[67][13] = 0;
   fontSTPtr->pixAryUC[67][14] = 24;
   fontSTPtr->pixAryUC[67][15] = 12;
   fontSTPtr->pixAryUC[67][16] = 252;
   fontSTPtr->pixAryUC[67][17] = 1;
   fontSTPtr->pixAryUC[67][18] = 62;
   fontSTPtr->pixAryUC[67][19] = 0;
   fontSTPtr->pixAryUC[67][20] = 0;
   fontSTPtr->pixAryUC[67][21] = 0;
   fontSTPtr->pixAryUC[67][22] = 0;
   fontSTPtr->pixAryUC[67][23] = 0;
   fontSTPtr->pixAryUC[67][24] = 0;

   return 0;
} /*addAscii99_normalFont*/

/*___________________________________________
` Fun68: addAscii100_normalFont
`  - adds character `d` (ascii 100) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii100_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[68])
      free(fontSTPtr->pixAryUC[68]);

   fontSTPtr->pixAryUC[68] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[68])
      return 1;

   fontSTPtr->widthArySS[68] = 14;
   fontSTPtr->lenArySS[68] = 25;

   fontSTPtr->pixAryUC[68][0] = 0;
   fontSTPtr->pixAryUC[68][1] = 0;
   fontSTPtr->pixAryUC[68][2] = 192;
   fontSTPtr->pixAryUC[68][3] = 0;
   fontSTPtr->pixAryUC[68][4] = 48;
   fontSTPtr->pixAryUC[68][5] = 0;
   fontSTPtr->pixAryUC[68][6] = 12;
   fontSTPtr->pixAryUC[68][7] = 0;
   fontSTPtr->pixAryUC[68][8] = 3;
   fontSTPtr->pixAryUC[68][9] = 255;
   fontSTPtr->pixAryUC[68][10] = 224;
   fontSTPtr->pixAryUC[68][11] = 63;
   fontSTPtr->pixAryUC[68][12] = 24;
   fontSTPtr->pixAryUC[68][13] = 12;
   fontSTPtr->pixAryUC[68][14] = 6;
   fontSTPtr->pixAryUC[68][15] = 3;
   fontSTPtr->pixAryUC[68][16] = 255;
   fontSTPtr->pixAryUC[68][17] = 134;
   fontSTPtr->pixAryUC[68][18] = 255;
   fontSTPtr->pixAryUC[68][19] = 0;
   fontSTPtr->pixAryUC[68][20] = 0;
   fontSTPtr->pixAryUC[68][21] = 0;
   fontSTPtr->pixAryUC[68][22] = 0;
   fontSTPtr->pixAryUC[68][23] = 0;
   fontSTPtr->pixAryUC[68][24] = 0;

   return 0;
} /*addAscii100_normalFont*/

/*___________________________________________
` Fun69: addAscii101_normalFont
`  - adds character `e` (ascii 101) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii101_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[69])
      free(fontSTPtr->pixAryUC[69]);

   fontSTPtr->pixAryUC[69] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[69])
      return 1;

   fontSTPtr->widthArySS[69] = 14;
   fontSTPtr->lenArySS[69] = 25;

   fontSTPtr->pixAryUC[69][0] = 0;
   fontSTPtr->pixAryUC[69][1] = 0;
   fontSTPtr->pixAryUC[69][2] = 0;
   fontSTPtr->pixAryUC[69][3] = 128;
   fontSTPtr->pixAryUC[69][4] = 127;
   fontSTPtr->pixAryUC[69][5] = 240;
   fontSTPtr->pixAryUC[69][6] = 63;
   fontSTPtr->pixAryUC[69][7] = 6;
   fontSTPtr->pixAryUC[69][8] = 152;
   fontSTPtr->pixAryUC[69][9] = 255;
   fontSTPtr->pixAryUC[69][10] = 231;
   fontSTPtr->pixAryUC[69][11] = 255;
   fontSTPtr->pixAryUC[69][12] = 8;
   fontSTPtr->pixAryUC[69][13] = 0;
   fontSTPtr->pixAryUC[69][14] = 6;
   fontSTPtr->pixAryUC[69][15] = 24;
   fontSTPtr->pixAryUC[69][16] = 255;
   fontSTPtr->pixAryUC[69][17] = 135;
   fontSTPtr->pixAryUC[69][18] = 255;
   fontSTPtr->pixAryUC[69][19] = 0;
   fontSTPtr->pixAryUC[69][20] = 0;
   fontSTPtr->pixAryUC[69][21] = 0;
   fontSTPtr->pixAryUC[69][22] = 0;
   fontSTPtr->pixAryUC[69][23] = 0;
   fontSTPtr->pixAryUC[69][24] = 0;

   return 0;
} /*addAscii101_normalFont*/

/*___________________________________________
` Fun70: addAscii102_normalFont
`  - adds character `f` (ascii 102) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii102_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[70])
      free(fontSTPtr->pixAryUC[70]);

   fontSTPtr->pixAryUC[70] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[70])
      return 1;

   fontSTPtr->widthArySS[70] = 14;
   fontSTPtr->lenArySS[70] = 25;

   fontSTPtr->pixAryUC[70][0] = 0;
   fontSTPtr->pixAryUC[70][1] = 0;
   fontSTPtr->pixAryUC[70][2] = 224;
   fontSTPtr->pixAryUC[70][3] = 0;
   fontSTPtr->pixAryUC[70][4] = 126;
   fontSTPtr->pixAryUC[70][5] = 128;
   fontSTPtr->pixAryUC[70][6] = 25;
   fontSTPtr->pixAryUC[70][7] = 96;
   fontSTPtr->pixAryUC[70][8] = 0;
   fontSTPtr->pixAryUC[70][9] = 255;
   fontSTPtr->pixAryUC[70][10] = 193;
   fontSTPtr->pixAryUC[70][11] = 127;
   fontSTPtr->pixAryUC[70][12] = 128;
   fontSTPtr->pixAryUC[70][13] = 1;
   fontSTPtr->pixAryUC[70][14] = 96;
   fontSTPtr->pixAryUC[70][15] = 0;
   fontSTPtr->pixAryUC[70][16] = 60;
   fontSTPtr->pixAryUC[70][17] = 128;
   fontSTPtr->pixAryUC[70][18] = 31;
   fontSTPtr->pixAryUC[70][19] = 0;
   fontSTPtr->pixAryUC[70][20] = 0;
   fontSTPtr->pixAryUC[70][21] = 0;
   fontSTPtr->pixAryUC[70][22] = 0;
   fontSTPtr->pixAryUC[70][23] = 0;
   fontSTPtr->pixAryUC[70][24] = 0;

   return 0;
} /*addAscii102_normalFont*/

/*___________________________________________
` Fun71: addAscii103_normalFont
`  - adds character `g` (ascii 103) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii103_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[71])
      free(fontSTPtr->pixAryUC[71]);

   fontSTPtr->pixAryUC[71] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[71])
      return 1;

   fontSTPtr->widthArySS[71] = 14;
   fontSTPtr->lenArySS[71] = 25;

   fontSTPtr->pixAryUC[71][0] = 0;
   fontSTPtr->pixAryUC[71][1] = 0;
   fontSTPtr->pixAryUC[71][2] = 0;
   fontSTPtr->pixAryUC[71][3] = 0;
   fontSTPtr->pixAryUC[71][4] = 127;
   fontSTPtr->pixAryUC[71][5] = 224;
   fontSTPtr->pixAryUC[71][6] = 63;
   fontSTPtr->pixAryUC[71][7] = 12;
   fontSTPtr->pixAryUC[71][8] = 24;
   fontSTPtr->pixAryUC[71][9] = 3;
   fontSTPtr->pixAryUC[71][10] = 134;
   fontSTPtr->pixAryUC[71][11] = 193;
   fontSTPtr->pixAryUC[71][12] = 192;
   fontSTPtr->pixAryUC[71][13] = 31;
   fontSTPtr->pixAryUC[71][14] = 224;
   fontSTPtr->pixAryUC[71][15] = 15;
   fontSTPtr->pixAryUC[71][16] = 0;
   fontSTPtr->pixAryUC[71][17] = 198;
   fontSTPtr->pixAryUC[71][18] = 192;
   fontSTPtr->pixAryUC[71][19] = 224;
   fontSTPtr->pixAryUC[71][20] = 31;
   fontSTPtr->pixAryUC[71][21] = 240;
   fontSTPtr->pixAryUC[71][22] = 3;
   fontSTPtr->pixAryUC[71][23] = 0;
   fontSTPtr->pixAryUC[71][24] = 0;

   return 0;
} /*addAscii103_normalFont*/

/*___________________________________________
` Fun72: addAscii104_normalFont
`  - adds character `h` (ascii 104) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii104_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[72])
      free(fontSTPtr->pixAryUC[72]);

   fontSTPtr->pixAryUC[72] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[72])
      return 1;

   fontSTPtr->widthArySS[72] = 14;
   fontSTPtr->lenArySS[72] = 25;

   fontSTPtr->pixAryUC[72][0] = 0;
   fontSTPtr->pixAryUC[72][1] = 128;
   fontSTPtr->pixAryUC[72][2] = 1;
   fontSTPtr->pixAryUC[72][3] = 96;
   fontSTPtr->pixAryUC[72][4] = 0;
   fontSTPtr->pixAryUC[72][5] = 24;
   fontSTPtr->pixAryUC[72][6] = 0;
   fontSTPtr->pixAryUC[72][7] = 246;
   fontSTPtr->pixAryUC[72][8] = 128;
   fontSTPtr->pixAryUC[72][9] = 127;
   fontSTPtr->pixAryUC[72][10] = 224;
   fontSTPtr->pixAryUC[72][11] = 24;
   fontSTPtr->pixAryUC[72][12] = 24;
   fontSTPtr->pixAryUC[72][13] = 6;
   fontSTPtr->pixAryUC[72][14] = 134;
   fontSTPtr->pixAryUC[72][15] = 137;
   fontSTPtr->pixAryUC[72][16] = 97;
   fontSTPtr->pixAryUC[72][17] = 99;
   fontSTPtr->pixAryUC[72][18] = 120;
   fontSTPtr->pixAryUC[72][19] = 0;
   fontSTPtr->pixAryUC[72][20] = 0;
   fontSTPtr->pixAryUC[72][21] = 0;
   fontSTPtr->pixAryUC[72][22] = 0;
   fontSTPtr->pixAryUC[72][23] = 0;
   fontSTPtr->pixAryUC[72][24] = 0;

   return 0;
} /*addAscii104_normalFont*/

/*___________________________________________
` Fun73: addAscii105_normalFont
`  - adds character `i` (ascii 105) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii105_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[73])
      free(fontSTPtr->pixAryUC[73]);

   fontSTPtr->pixAryUC[73] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[73])
      return 1;

   fontSTPtr->widthArySS[73] = 14;
   fontSTPtr->lenArySS[73] = 25;

   fontSTPtr->pixAryUC[73][0] = 0;
   fontSTPtr->pixAryUC[73][1] = 0;
   fontSTPtr->pixAryUC[73][2] = 12;
   fontSTPtr->pixAryUC[73][3] = 0;
   fontSTPtr->pixAryUC[73][4] = 3;
   fontSTPtr->pixAryUC[73][5] = 0;
   fontSTPtr->pixAryUC[73][6] = 0;
   fontSTPtr->pixAryUC[73][7] = 0;
   fontSTPtr->pixAryUC[73][8] = 0;
   fontSTPtr->pixAryUC[73][9] = 12;
   fontSTPtr->pixAryUC[73][10] = 0;
   fontSTPtr->pixAryUC[73][11] = 3;
   fontSTPtr->pixAryUC[73][12] = 192;
   fontSTPtr->pixAryUC[73][13] = 0;
   fontSTPtr->pixAryUC[73][14] = 48;
   fontSTPtr->pixAryUC[73][15] = 6;
   fontSTPtr->pixAryUC[73][16] = 252;
   fontSTPtr->pixAryUC[73][17] = 0;
   fontSTPtr->pixAryUC[73][18] = 30;
   fontSTPtr->pixAryUC[73][19] = 0;
   fontSTPtr->pixAryUC[73][20] = 0;
   fontSTPtr->pixAryUC[73][21] = 0;
   fontSTPtr->pixAryUC[73][22] = 0;
   fontSTPtr->pixAryUC[73][23] = 0;
   fontSTPtr->pixAryUC[73][24] = 0;

   return 0;
} /*addAscii105_normalFont*/

/*___________________________________________
` Fun74: addAscii106_normalFont
`  - adds character `j` (ascii 106) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii106_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[74])
      free(fontSTPtr->pixAryUC[74]);

   fontSTPtr->pixAryUC[74] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[74])
      return 1;

   fontSTPtr->widthArySS[74] = 14;
   fontSTPtr->lenArySS[74] = 25;

   fontSTPtr->pixAryUC[74][0] = 0;
   fontSTPtr->pixAryUC[74][1] = 0;
   fontSTPtr->pixAryUC[74][2] = 192;
   fontSTPtr->pixAryUC[74][3] = 0;
   fontSTPtr->pixAryUC[74][4] = 48;
   fontSTPtr->pixAryUC[74][5] = 0;
   fontSTPtr->pixAryUC[74][6] = 0;
   fontSTPtr->pixAryUC[74][7] = 0;
   fontSTPtr->pixAryUC[74][8] = 3;
   fontSTPtr->pixAryUC[74][9] = 192;
   fontSTPtr->pixAryUC[74][10] = 0;
   fontSTPtr->pixAryUC[74][11] = 48;
   fontSTPtr->pixAryUC[74][12] = 96;
   fontSTPtr->pixAryUC[74][13] = 12;
   fontSTPtr->pixAryUC[74][14] = 24;
   fontSTPtr->pixAryUC[74][15] = 3;
   fontSTPtr->pixAryUC[74][16] = 252;
   fontSTPtr->pixAryUC[74][17] = 0;
   fontSTPtr->pixAryUC[74][18] = 30;
   fontSTPtr->pixAryUC[74][19] = 0;
   fontSTPtr->pixAryUC[74][20] = 0;
   fontSTPtr->pixAryUC[74][21] = 0;
   fontSTPtr->pixAryUC[74][22] = 0;
   fontSTPtr->pixAryUC[74][23] = 0;
   fontSTPtr->pixAryUC[74][24] = 0;

   return 0;
} /*addAscii106_normalFont*/

/*___________________________________________
` Fun75: addAscii107_normalFont
`  - adds character `k` (ascii 107) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii107_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[75])
      free(fontSTPtr->pixAryUC[75]);

   fontSTPtr->pixAryUC[75] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[75])
      return 1;

   fontSTPtr->widthArySS[75] = 14;
   fontSTPtr->lenArySS[75] = 25;

   fontSTPtr->pixAryUC[75][0] = 0;
   fontSTPtr->pixAryUC[75][1] = 0;
   fontSTPtr->pixAryUC[75][2] = 6;
   fontSTPtr->pixAryUC[75][3] = 131;
   fontSTPtr->pixAryUC[75][4] = 97;
   fontSTPtr->pixAryUC[75][5] = 96;
   fontSTPtr->pixAryUC[75][6] = 12;
   fontSTPtr->pixAryUC[75][7] = 152;
   fontSTPtr->pixAryUC[75][8] = 1;
   fontSTPtr->pixAryUC[75][9] = 62;
   fontSTPtr->pixAryUC[75][10] = 128;
   fontSTPtr->pixAryUC[75][11] = 15;
   fontSTPtr->pixAryUC[75][12] = 96;
   fontSTPtr->pixAryUC[75][13] = 6;
   fontSTPtr->pixAryUC[75][14] = 24;
   fontSTPtr->pixAryUC[75][15] = 3;
   fontSTPtr->pixAryUC[75][16] = 134;
   fontSTPtr->pixAryUC[75][17] = 129;
   fontSTPtr->pixAryUC[75][18] = 193;
   fontSTPtr->pixAryUC[75][19] = 0;
   fontSTPtr->pixAryUC[75][20] = 0;
   fontSTPtr->pixAryUC[75][21] = 0;
   fontSTPtr->pixAryUC[75][22] = 0;
   fontSTPtr->pixAryUC[75][23] = 0;
   fontSTPtr->pixAryUC[75][24] = 0;

   return 0;
} /*addAscii107_normalFont*/

/*___________________________________________
` Fun76: addAscii108_normalFont
`  - adds character `l` (ascii 108) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii108_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[76])
      free(fontSTPtr->pixAryUC[76]);

   fontSTPtr->pixAryUC[76] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[76])
      return 1;

   fontSTPtr->widthArySS[76] = 14;
   fontSTPtr->lenArySS[76] = 25;

   fontSTPtr->pixAryUC[76][0] = 0;
   fontSTPtr->pixAryUC[76][1] = 0;
   fontSTPtr->pixAryUC[76][2] = 12;
   fontSTPtr->pixAryUC[76][3] = 0;
   fontSTPtr->pixAryUC[76][4] = 3;
   fontSTPtr->pixAryUC[76][5] = 192;
   fontSTPtr->pixAryUC[76][6] = 0;
   fontSTPtr->pixAryUC[76][7] = 48;
   fontSTPtr->pixAryUC[76][8] = 0;
   fontSTPtr->pixAryUC[76][9] = 12;
   fontSTPtr->pixAryUC[76][10] = 0;
   fontSTPtr->pixAryUC[76][11] = 3;
   fontSTPtr->pixAryUC[76][12] = 192;
   fontSTPtr->pixAryUC[76][13] = 24;
   fontSTPtr->pixAryUC[76][14] = 48;
   fontSTPtr->pixAryUC[76][15] = 7;
   fontSTPtr->pixAryUC[76][16] = 252;
   fontSTPtr->pixAryUC[76][17] = 0;
   fontSTPtr->pixAryUC[76][18] = 30;
   fontSTPtr->pixAryUC[76][19] = 0;
   fontSTPtr->pixAryUC[76][20] = 0;
   fontSTPtr->pixAryUC[76][21] = 0;
   fontSTPtr->pixAryUC[76][22] = 0;
   fontSTPtr->pixAryUC[76][23] = 0;
   fontSTPtr->pixAryUC[76][24] = 0;

   return 0;
} /*addAscii108_normalFont*/

/*___________________________________________
` Fun77: addAscii109_normalFont
`  - adds character `m` (ascii 109) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii109_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[77])
      free(fontSTPtr->pixAryUC[77]);

   fontSTPtr->pixAryUC[77] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[77])
      return 1;

   fontSTPtr->widthArySS[77] = 14;
   fontSTPtr->lenArySS[77] = 25;

   fontSTPtr->pixAryUC[77][0] = 0;
   fontSTPtr->pixAryUC[77][1] = 0;
   fontSTPtr->pixAryUC[77][2] = 0;
   fontSTPtr->pixAryUC[77][3] = 160;
   fontSTPtr->pixAryUC[77][4] = 48;
   fontSTPtr->pixAryUC[77][5] = 120;
   fontSTPtr->pixAryUC[77][6] = 30;
   fontSTPtr->pixAryUC[77][7] = 246;
   fontSTPtr->pixAryUC[77][8] = 134;
   fontSTPtr->pixAryUC[77][9] = 153;
   fontSTPtr->pixAryUC[77][10] = 97;
   fontSTPtr->pixAryUC[77][11] = 102;
   fontSTPtr->pixAryUC[77][12] = 152;
   fontSTPtr->pixAryUC[77][13] = 25;
   fontSTPtr->pixAryUC[77][14] = 102;
   fontSTPtr->pixAryUC[77][15] = 134;
   fontSTPtr->pixAryUC[77][16] = 153;
   fontSTPtr->pixAryUC[77][17] = 101;
   fontSTPtr->pixAryUC[77][18] = 230;
   fontSTPtr->pixAryUC[77][19] = 0;
   fontSTPtr->pixAryUC[77][20] = 0;
   fontSTPtr->pixAryUC[77][21] = 0;
   fontSTPtr->pixAryUC[77][22] = 0;
   fontSTPtr->pixAryUC[77][23] = 0;
   fontSTPtr->pixAryUC[77][24] = 0;

   return 0;
} /*addAscii109_normalFont*/

/*___________________________________________
` Fun78: addAscii110_normalFont
`  - adds character `n` (ascii 110) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii110_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[78])
      free(fontSTPtr->pixAryUC[78]);

   fontSTPtr->pixAryUC[78] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[78])
      return 1;

   fontSTPtr->widthArySS[78] = 14;
   fontSTPtr->lenArySS[78] = 25;

   fontSTPtr->pixAryUC[78][0] = 0;
   fontSTPtr->pixAryUC[78][1] = 0;
   fontSTPtr->pixAryUC[78][2] = 0;
   fontSTPtr->pixAryUC[78][3] = 128;
   fontSTPtr->pixAryUC[78][4] = 15;
   fontSTPtr->pixAryUC[78][5] = 240;
   fontSTPtr->pixAryUC[78][6] = 7;
   fontSTPtr->pixAryUC[78][7] = 142;
   fontSTPtr->pixAryUC[78][8] = 131;
   fontSTPtr->pixAryUC[78][9] = 193;
   fontSTPtr->pixAryUC[78][10] = 96;
   fontSTPtr->pixAryUC[78][11] = 48;
   fontSTPtr->pixAryUC[78][12] = 24;
   fontSTPtr->pixAryUC[78][13] = 12;
   fontSTPtr->pixAryUC[78][14] = 6;
   fontSTPtr->pixAryUC[78][15] = 131;
   fontSTPtr->pixAryUC[78][16] = 193;
   fontSTPtr->pixAryUC[78][17] = 102;
   fontSTPtr->pixAryUC[78][18] = 240;
   fontSTPtr->pixAryUC[78][19] = 0;
   fontSTPtr->pixAryUC[78][20] = 0;
   fontSTPtr->pixAryUC[78][21] = 0;
   fontSTPtr->pixAryUC[78][22] = 0;
   fontSTPtr->pixAryUC[78][23] = 0;
   fontSTPtr->pixAryUC[78][24] = 0;

   return 0;
} /*addAscii110_normalFont*/

/*___________________________________________
` Fun79: addAscii111_normalFont
`  - adds character `o` (ascii 111) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii111_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[79])
      free(fontSTPtr->pixAryUC[79]);

   fontSTPtr->pixAryUC[79] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[79])
      return 1;

   fontSTPtr->widthArySS[79] = 14;
   fontSTPtr->lenArySS[79] = 25;

   fontSTPtr->pixAryUC[79][0] = 0;
   fontSTPtr->pixAryUC[79][1] = 0;
   fontSTPtr->pixAryUC[79][2] = 0;
   fontSTPtr->pixAryUC[79][3] = 0;
   fontSTPtr->pixAryUC[79][4] = 63;
   fontSTPtr->pixAryUC[79][5] = 224;
   fontSTPtr->pixAryUC[79][6] = 31;
   fontSTPtr->pixAryUC[79][7] = 12;
   fontSTPtr->pixAryUC[79][8] = 140;
   fontSTPtr->pixAryUC[79][9] = 1;
   fontSTPtr->pixAryUC[79][10] = 38;
   fontSTPtr->pixAryUC[79][11] = 0;
   fontSTPtr->pixAryUC[79][12] = 25;
   fontSTPtr->pixAryUC[79][13] = 96;
   fontSTPtr->pixAryUC[79][14] = 12;
   fontSTPtr->pixAryUC[79][15] = 12;
   fontSTPtr->pixAryUC[79][16] = 254;
   fontSTPtr->pixAryUC[79][17] = 1;
   fontSTPtr->pixAryUC[79][18] = 63;
   fontSTPtr->pixAryUC[79][19] = 0;
   fontSTPtr->pixAryUC[79][20] = 0;
   fontSTPtr->pixAryUC[79][21] = 0;
   fontSTPtr->pixAryUC[79][22] = 0;
   fontSTPtr->pixAryUC[79][23] = 0;
   fontSTPtr->pixAryUC[79][24] = 0;

   return 0;
} /*addAscii111_normalFont*/

/*___________________________________________
` Fun80: addAscii112_normalFont
`  - adds character `p` (ascii 112) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii112_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[80])
      free(fontSTPtr->pixAryUC[80]);

   fontSTPtr->pixAryUC[80] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[80])
      return 1;

   fontSTPtr->widthArySS[80] = 14;
   fontSTPtr->lenArySS[80] = 25;

   fontSTPtr->pixAryUC[80][0] = 0;
   fontSTPtr->pixAryUC[80][1] = 0;
   fontSTPtr->pixAryUC[80][2] = 0;
   fontSTPtr->pixAryUC[80][3] = 192;
   fontSTPtr->pixAryUC[80][4] = 63;
   fontSTPtr->pixAryUC[80][5] = 240;
   fontSTPtr->pixAryUC[80][6] = 31;
   fontSTPtr->pixAryUC[80][7] = 12;
   fontSTPtr->pixAryUC[80][8] = 12;
   fontSTPtr->pixAryUC[80][9] = 3;
   fontSTPtr->pixAryUC[80][10] = 195;
   fontSTPtr->pixAryUC[80][11] = 127;
   fontSTPtr->pixAryUC[80][12] = 240;
   fontSTPtr->pixAryUC[80][13] = 15;
   fontSTPtr->pixAryUC[80][14] = 12;
   fontSTPtr->pixAryUC[80][15] = 0;
   fontSTPtr->pixAryUC[80][16] = 3;
   fontSTPtr->pixAryUC[80][17] = 192;
   fontSTPtr->pixAryUC[80][18] = 0;
   fontSTPtr->pixAryUC[80][19] = 0;
   fontSTPtr->pixAryUC[80][20] = 0;
   fontSTPtr->pixAryUC[80][21] = 0;
   fontSTPtr->pixAryUC[80][22] = 0;
   fontSTPtr->pixAryUC[80][23] = 0;
   fontSTPtr->pixAryUC[80][24] = 0;

   return 0;
} /*addAscii112_normalFont*/

/*___________________________________________
` Fun81: addAscii113_normalFont
`  - adds character `q` (ascii 113) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii113_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[81])
      free(fontSTPtr->pixAryUC[81]);

   fontSTPtr->pixAryUC[81] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[81])
      return 1;

   fontSTPtr->widthArySS[81] = 14;
   fontSTPtr->lenArySS[81] = 25;

   fontSTPtr->pixAryUC[81][0] = 0;
   fontSTPtr->pixAryUC[81][1] = 0;
   fontSTPtr->pixAryUC[81][2] = 0;
   fontSTPtr->pixAryUC[81][3] = 128;
   fontSTPtr->pixAryUC[81][4] = 31;
   fontSTPtr->pixAryUC[81][5] = 240;
   fontSTPtr->pixAryUC[81][6] = 15;
   fontSTPtr->pixAryUC[81][7] = 6;
   fontSTPtr->pixAryUC[81][8] = 134;
   fontSTPtr->pixAryUC[81][9] = 129;
   fontSTPtr->pixAryUC[81][10] = 193;
   fontSTPtr->pixAryUC[81][11] = 127;
   fontSTPtr->pixAryUC[81][12] = 224;
   fontSTPtr->pixAryUC[81][13] = 31;
   fontSTPtr->pixAryUC[81][14] = 0;
   fontSTPtr->pixAryUC[81][15] = 6;
   fontSTPtr->pixAryUC[81][16] = 128;
   fontSTPtr->pixAryUC[81][17] = 1;
   fontSTPtr->pixAryUC[81][18] = 240;
   fontSTPtr->pixAryUC[81][19] = 0;
   fontSTPtr->pixAryUC[81][20] = 0;
   fontSTPtr->pixAryUC[81][21] = 0;
   fontSTPtr->pixAryUC[81][22] = 0;
   fontSTPtr->pixAryUC[81][23] = 0;
   fontSTPtr->pixAryUC[81][24] = 0;

   return 0;
} /*addAscii113_normalFont*/

/*___________________________________________
` Fun82: addAscii114_normalFont
`  - adds character `r` (ascii 114) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii114_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[82])
      free(fontSTPtr->pixAryUC[82]);

   fontSTPtr->pixAryUC[82] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[82])
      return 1;

   fontSTPtr->widthArySS[82] = 14;
   fontSTPtr->lenArySS[82] = 25;

   fontSTPtr->pixAryUC[82][0] = 0;
   fontSTPtr->pixAryUC[82][1] = 0;
   fontSTPtr->pixAryUC[82][2] = 0;
   fontSTPtr->pixAryUC[82][3] = 192;
   fontSTPtr->pixAryUC[82][4] = 115;
   fontSTPtr->pixAryUC[82][5] = 216;
   fontSTPtr->pixAryUC[82][6] = 51;
   fontSTPtr->pixAryUC[82][7] = 114;
   fontSTPtr->pixAryUC[82][8] = 24;
   fontSTPtr->pixAryUC[82][9] = 12;
   fontSTPtr->pixAryUC[82][10] = 6;
   fontSTPtr->pixAryUC[82][11] = 3;
   fontSTPtr->pixAryUC[82][12] = 192;
   fontSTPtr->pixAryUC[82][13] = 0;
   fontSTPtr->pixAryUC[82][14] = 48;
   fontSTPtr->pixAryUC[82][15] = 0;
   fontSTPtr->pixAryUC[82][16] = 12;
   fontSTPtr->pixAryUC[82][17] = 0;
   fontSTPtr->pixAryUC[82][18] = 3;
   fontSTPtr->pixAryUC[82][19] = 0;
   fontSTPtr->pixAryUC[82][20] = 0;
   fontSTPtr->pixAryUC[82][21] = 0;
   fontSTPtr->pixAryUC[82][22] = 0;
   fontSTPtr->pixAryUC[82][23] = 0;
   fontSTPtr->pixAryUC[82][24] = 0;

   return 0;
} /*addAscii114_normalFont*/

/*___________________________________________
` Fun83: addAscii115_normalFont
`  - adds character `s` (ascii 115) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii115_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[83])
      free(fontSTPtr->pixAryUC[83]);

   fontSTPtr->pixAryUC[83] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[83])
      return 1;

   fontSTPtr->widthArySS[83] = 14;
   fontSTPtr->lenArySS[83] = 25;

   fontSTPtr->pixAryUC[83][0] = 0;
   fontSTPtr->pixAryUC[83][1] = 0;
   fontSTPtr->pixAryUC[83][2] = 0;
   fontSTPtr->pixAryUC[83][3] = 128;
   fontSTPtr->pixAryUC[83][4] = 127;
   fontSTPtr->pixAryUC[83][5] = 248;
   fontSTPtr->pixAryUC[83][6] = 63;
   fontSTPtr->pixAryUC[83][7] = 6;
   fontSTPtr->pixAryUC[83][8] = 128;
   fontSTPtr->pixAryUC[83][9] = 255;
   fontSTPtr->pixAryUC[83][10] = 192;
   fontSTPtr->pixAryUC[83][11] = 127;
   fontSTPtr->pixAryUC[83][12] = 0;
   fontSTPtr->pixAryUC[83][13] = 16;
   fontSTPtr->pixAryUC[83][14] = 6;
   fontSTPtr->pixAryUC[83][15] = 134;
   fontSTPtr->pixAryUC[83][16] = 255;
   fontSTPtr->pixAryUC[83][17] = 192;
   fontSTPtr->pixAryUC[83][18] = 31;
   fontSTPtr->pixAryUC[83][19] = 0;
   fontSTPtr->pixAryUC[83][20] = 0;
   fontSTPtr->pixAryUC[83][21] = 0;
   fontSTPtr->pixAryUC[83][22] = 0;
   fontSTPtr->pixAryUC[83][23] = 0;
   fontSTPtr->pixAryUC[83][24] = 0;

   return 0;
} /*addAscii115_normalFont*/

/*___________________________________________
` Fun84: addAscii116_normalFont
`  - adds character `t` (ascii 116) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii116_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[84])
      free(fontSTPtr->pixAryUC[84]);

   fontSTPtr->pixAryUC[84] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[84])
      return 1;

   fontSTPtr->widthArySS[84] = 14;
   fontSTPtr->lenArySS[84] = 25;

   fontSTPtr->pixAryUC[84][0] = 0;
   fontSTPtr->pixAryUC[84][1] = 0;
   fontSTPtr->pixAryUC[84][2] = 48;
   fontSTPtr->pixAryUC[84][3] = 0;
   fontSTPtr->pixAryUC[84][4] = 12;
   fontSTPtr->pixAryUC[84][5] = 0;
   fontSTPtr->pixAryUC[84][6] = 3;
   fontSTPtr->pixAryUC[84][7] = 252;
   fontSTPtr->pixAryUC[84][8] = 15;
   fontSTPtr->pixAryUC[84][9] = 255;
   fontSTPtr->pixAryUC[84][10] = 3;
   fontSTPtr->pixAryUC[84][11] = 12;
   fontSTPtr->pixAryUC[84][12] = 0;
   fontSTPtr->pixAryUC[84][13] = 3;
   fontSTPtr->pixAryUC[84][14] = 192;
   fontSTPtr->pixAryUC[84][15] = 12;
   fontSTPtr->pixAryUC[84][16] = 240;
   fontSTPtr->pixAryUC[84][17] = 3;
   fontSTPtr->pixAryUC[84][18] = 120;
   fontSTPtr->pixAryUC[84][19] = 0;
   fontSTPtr->pixAryUC[84][20] = 0;
   fontSTPtr->pixAryUC[84][21] = 0;
   fontSTPtr->pixAryUC[84][22] = 0;
   fontSTPtr->pixAryUC[84][23] = 0;
   fontSTPtr->pixAryUC[84][24] = 0;

   return 0;
} /*addAscii116_normalFont*/

/*___________________________________________
` Fun85: addAscii117_normalFont
`  - adds character `u` (ascii 117) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii117_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[85])
      free(fontSTPtr->pixAryUC[85]);

   fontSTPtr->pixAryUC[85] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[85])
      return 1;

   fontSTPtr->widthArySS[85] = 14;
   fontSTPtr->lenArySS[85] = 25;

   fontSTPtr->pixAryUC[85][0] = 0;
   fontSTPtr->pixAryUC[85][1] = 0;
   fontSTPtr->pixAryUC[85][2] = 0;
   fontSTPtr->pixAryUC[85][3] = 192;
   fontSTPtr->pixAryUC[85][4] = 24;
   fontSTPtr->pixAryUC[85][5] = 48;
   fontSTPtr->pixAryUC[85][6] = 6;
   fontSTPtr->pixAryUC[85][7] = 140;
   fontSTPtr->pixAryUC[85][8] = 1;
   fontSTPtr->pixAryUC[85][9] = 99;
   fontSTPtr->pixAryUC[85][10] = 192;
   fontSTPtr->pixAryUC[85][11] = 24;
   fontSTPtr->pixAryUC[85][12] = 48;
   fontSTPtr->pixAryUC[85][13] = 6;
   fontSTPtr->pixAryUC[85][14] = 140;
   fontSTPtr->pixAryUC[85][15] = 25;
   fontSTPtr->pixAryUC[85][16] = 94;
   fontSTPtr->pixAryUC[85][17] = 3;
   fontSTPtr->pixAryUC[85][18] = 115;
   fontSTPtr->pixAryUC[85][19] = 0;
   fontSTPtr->pixAryUC[85][20] = 0;
   fontSTPtr->pixAryUC[85][21] = 0;
   fontSTPtr->pixAryUC[85][22] = 0;
   fontSTPtr->pixAryUC[85][23] = 0;
   fontSTPtr->pixAryUC[85][24] = 0;

   return 0;
} /*addAscii117_normalFont*/

/*___________________________________________
` Fun86: addAscii118_normalFont
`  - adds character `v` (ascii 118) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii118_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[86])
      free(fontSTPtr->pixAryUC[86]);

   fontSTPtr->pixAryUC[86] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[86])
      return 1;

   fontSTPtr->widthArySS[86] = 14;
   fontSTPtr->lenArySS[86] = 25;

   fontSTPtr->pixAryUC[86][0] = 0;
   fontSTPtr->pixAryUC[86][1] = 0;
   fontSTPtr->pixAryUC[86][2] = 0;
   fontSTPtr->pixAryUC[86][3] = 128;
   fontSTPtr->pixAryUC[86][4] = 97;
   fontSTPtr->pixAryUC[86][5] = 96;
   fontSTPtr->pixAryUC[86][6] = 24;
   fontSTPtr->pixAryUC[86][7] = 24;
   fontSTPtr->pixAryUC[86][8] = 6;
   fontSTPtr->pixAryUC[86][9] = 134;
   fontSTPtr->pixAryUC[86][10] = 129;
   fontSTPtr->pixAryUC[86][11] = 97;
   fontSTPtr->pixAryUC[86][12] = 96;
   fontSTPtr->pixAryUC[86][13] = 24;
   fontSTPtr->pixAryUC[86][14] = 48;
   fontSTPtr->pixAryUC[86][15] = 3;
   fontSTPtr->pixAryUC[86][16] = 120;
   fontSTPtr->pixAryUC[86][17] = 0;
   fontSTPtr->pixAryUC[86][18] = 12;
   fontSTPtr->pixAryUC[86][19] = 0;
   fontSTPtr->pixAryUC[86][20] = 0;
   fontSTPtr->pixAryUC[86][21] = 0;
   fontSTPtr->pixAryUC[86][22] = 0;
   fontSTPtr->pixAryUC[86][23] = 0;
   fontSTPtr->pixAryUC[86][24] = 0;

   return 0;
} /*addAscii118_normalFont*/

/*___________________________________________
` Fun87: addAscii119_normalFont
`  - adds character `w` (ascii 119) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii119_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[87])
      free(fontSTPtr->pixAryUC[87]);

   fontSTPtr->pixAryUC[87] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[87])
      return 1;

   fontSTPtr->widthArySS[87] = 14;
   fontSTPtr->lenArySS[87] = 25;

   fontSTPtr->pixAryUC[87][0] = 0;
   fontSTPtr->pixAryUC[87][1] = 0;
   fontSTPtr->pixAryUC[87][2] = 0;
   fontSTPtr->pixAryUC[87][3] = 96;
   fontSTPtr->pixAryUC[87][4] = 128;
   fontSTPtr->pixAryUC[87][5] = 25;
   fontSTPtr->pixAryUC[87][6] = 96;
   fontSTPtr->pixAryUC[87][7] = 198;
   fontSTPtr->pixAryUC[87][8] = 152;
   fontSTPtr->pixAryUC[87][9] = 121;
   fontSTPtr->pixAryUC[87][10] = 102;
   fontSTPtr->pixAryUC[87][11] = 191;
   fontSTPtr->pixAryUC[87][12] = 249;
   fontSTPtr->pixAryUC[87][13] = 124;
   fontSTPtr->pixAryUC[87][14] = 30;
   fontSTPtr->pixAryUC[87][15] = 158;
   fontSTPtr->pixAryUC[87][16] = 3;
   fontSTPtr->pixAryUC[87][17] = 103;
   fontSTPtr->pixAryUC[87][18] = 128;
   fontSTPtr->pixAryUC[87][19] = 1;
   fontSTPtr->pixAryUC[87][20] = 0;
   fontSTPtr->pixAryUC[87][21] = 0;
   fontSTPtr->pixAryUC[87][22] = 0;
   fontSTPtr->pixAryUC[87][23] = 0;
   fontSTPtr->pixAryUC[87][24] = 0;

   return 0;
} /*addAscii119_normalFont*/

/*___________________________________________
` Fun88: addAscii120_normalFont
`  - adds character `x` (ascii 120) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii120_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[88])
      free(fontSTPtr->pixAryUC[88]);

   fontSTPtr->pixAryUC[88] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[88])
      return 1;

   fontSTPtr->widthArySS[88] = 14;
   fontSTPtr->lenArySS[88] = 25;

   fontSTPtr->pixAryUC[88][0] = 0;
   fontSTPtr->pixAryUC[88][1] = 0;
   fontSTPtr->pixAryUC[88][2] = 0;
   fontSTPtr->pixAryUC[88][3] = 192;
   fontSTPtr->pixAryUC[88][4] = 96;
   fontSTPtr->pixAryUC[88][5] = 96;
   fontSTPtr->pixAryUC[88][6] = 12;
   fontSTPtr->pixAryUC[88][7] = 176;
   fontSTPtr->pixAryUC[88][8] = 1;
   fontSTPtr->pixAryUC[88][9] = 56;
   fontSTPtr->pixAryUC[88][10] = 0;
   fontSTPtr->pixAryUC[88][11] = 4;
   fontSTPtr->pixAryUC[88][12] = 128;
   fontSTPtr->pixAryUC[88][13] = 3;
   fontSTPtr->pixAryUC[88][14] = 176;
   fontSTPtr->pixAryUC[88][15] = 1;
   fontSTPtr->pixAryUC[88][16] = 198;
   fontSTPtr->pixAryUC[88][17] = 192;
   fontSTPtr->pixAryUC[88][18] = 96;
   fontSTPtr->pixAryUC[88][19] = 0;
   fontSTPtr->pixAryUC[88][20] = 0;
   fontSTPtr->pixAryUC[88][21] = 0;
   fontSTPtr->pixAryUC[88][22] = 0;
   fontSTPtr->pixAryUC[88][23] = 0;
   fontSTPtr->pixAryUC[88][24] = 0;

   return 0;
} /*addAscii120_normalFont*/

/*___________________________________________
` Fun89: addAscii121_normalFont
`  - adds character `y` (ascii 121) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii121_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[89])
      free(fontSTPtr->pixAryUC[89]);

   fontSTPtr->pixAryUC[89] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[89])
      return 1;

   fontSTPtr->widthArySS[89] = 14;
   fontSTPtr->lenArySS[89] = 25;

   fontSTPtr->pixAryUC[89][0] = 0;
   fontSTPtr->pixAryUC[89][1] = 0;
   fontSTPtr->pixAryUC[89][2] = 0;
   fontSTPtr->pixAryUC[89][3] = 96;
   fontSTPtr->pixAryUC[89][4] = 128;
   fontSTPtr->pixAryUC[89][5] = 49;
   fontSTPtr->pixAryUC[89][6] = 48;
   fontSTPtr->pixAryUC[89][7] = 24;
   fontSTPtr->pixAryUC[89][8] = 6;
   fontSTPtr->pixAryUC[89][9] = 204;
   fontSTPtr->pixAryUC[89][10] = 0;
   fontSTPtr->pixAryUC[89][11] = 30;
   fontSTPtr->pixAryUC[89][12] = 128;
   fontSTPtr->pixAryUC[89][13] = 3;
   fontSTPtr->pixAryUC[89][14] = 96;
   fontSTPtr->pixAryUC[89][15] = 0;
   fontSTPtr->pixAryUC[89][16] = 12;
   fontSTPtr->pixAryUC[89][17] = 128;
   fontSTPtr->pixAryUC[89][18] = 1;
   fontSTPtr->pixAryUC[89][19] = 48;
   fontSTPtr->pixAryUC[89][20] = 0;
   fontSTPtr->pixAryUC[89][21] = 14;
   fontSTPtr->pixAryUC[89][22] = 0;
   fontSTPtr->pixAryUC[89][23] = 0;
   fontSTPtr->pixAryUC[89][24] = 0;

   return 0;
} /*addAscii121_normalFont*/

/*___________________________________________
` Fun90: addAscii122_normalFont
`  - adds character `z` (ascii 122) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii122_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[90])
      free(fontSTPtr->pixAryUC[90]);

   fontSTPtr->pixAryUC[90] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[90])
      return 1;

   fontSTPtr->widthArySS[90] = 14;
   fontSTPtr->lenArySS[90] = 25;

   fontSTPtr->pixAryUC[90][0] = 0;
   fontSTPtr->pixAryUC[90][1] = 0;
   fontSTPtr->pixAryUC[90][2] = 0;
   fontSTPtr->pixAryUC[90][3] = 224;
   fontSTPtr->pixAryUC[90][4] = 255;
   fontSTPtr->pixAryUC[90][5] = 249;
   fontSTPtr->pixAryUC[90][6] = 63;
   fontSTPtr->pixAryUC[90][7] = 128;
   fontSTPtr->pixAryUC[90][8] = 135;
   fontSTPtr->pixAryUC[90][9] = 183;
   fontSTPtr->pixAryUC[90][10] = 231;
   fontSTPtr->pixAryUC[90][11] = 246;
   fontSTPtr->pixAryUC[90][12] = 193;
   fontSTPtr->pixAryUC[90][13] = 0;
   fontSTPtr->pixAryUC[90][14] = 24;
   fontSTPtr->pixAryUC[90][15] = 0;
   fontSTPtr->pixAryUC[90][16] = 255;
   fontSTPtr->pixAryUC[90][17] = 231;
   fontSTPtr->pixAryUC[90][18] = 255;
   fontSTPtr->pixAryUC[90][19] = 1;
   fontSTPtr->pixAryUC[90][20] = 0;
   fontSTPtr->pixAryUC[90][21] = 0;
   fontSTPtr->pixAryUC[90][22] = 0;
   fontSTPtr->pixAryUC[90][23] = 0;
   fontSTPtr->pixAryUC[90][24] = 0;

   return 0;
} /*addAscii122_normalFont*/

/*___________________________________________
` Fun91: addAscii123_normalFont
`  - adds character `{` (ascii 123) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii123_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[91])
      free(fontSTPtr->pixAryUC[91]);

   fontSTPtr->pixAryUC[91] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[91])
      return 1;

   fontSTPtr->widthArySS[91] = 14;
   fontSTPtr->lenArySS[91] = 25;

   fontSTPtr->pixAryUC[91][0] = 0;
   fontSTPtr->pixAryUC[91][1] = 0;
   fontSTPtr->pixAryUC[91][2] = 248;
   fontSTPtr->pixAryUC[91][3] = 1;
   fontSTPtr->pixAryUC[91][4] = 63;
   fontSTPtr->pixAryUC[91][5] = 96;
   fontSTPtr->pixAryUC[91][6] = 0;
   fontSTPtr->pixAryUC[91][7] = 24;
   fontSTPtr->pixAryUC[91][8] = 0;
   fontSTPtr->pixAryUC[91][9] = 3;
   fontSTPtr->pixAryUC[91][10] = 192;
   fontSTPtr->pixAryUC[91][11] = 0;
   fontSTPtr->pixAryUC[91][12] = 96;
   fontSTPtr->pixAryUC[91][13] = 0;
   fontSTPtr->pixAryUC[91][14] = 24;
   fontSTPtr->pixAryUC[91][15] = 0;
   fontSTPtr->pixAryUC[91][16] = 252;
   fontSTPtr->pixAryUC[91][17] = 0;
   fontSTPtr->pixAryUC[91][18] = 126;
   fontSTPtr->pixAryUC[91][19] = 0;
   fontSTPtr->pixAryUC[91][20] = 0;
   fontSTPtr->pixAryUC[91][21] = 0;
   fontSTPtr->pixAryUC[91][22] = 0;
   fontSTPtr->pixAryUC[91][23] = 0;
   fontSTPtr->pixAryUC[91][24] = 0;

   return 0;
} /*addAscii123_normalFont*/

/*___________________________________________
` Fun92: addAscii124_normalFont
`  - adds character `|` (ascii 124) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii124_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[92])
      free(fontSTPtr->pixAryUC[92]);

   fontSTPtr->pixAryUC[92] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[92])
      return 1;

   fontSTPtr->widthArySS[92] = 14;
   fontSTPtr->lenArySS[92] = 25;

   fontSTPtr->pixAryUC[92][0] = 0;
   fontSTPtr->pixAryUC[92][1] = 0;
   fontSTPtr->pixAryUC[92][2] = 24;
   fontSTPtr->pixAryUC[92][3] = 0;
   fontSTPtr->pixAryUC[92][4] = 6;
   fontSTPtr->pixAryUC[92][5] = 128;
   fontSTPtr->pixAryUC[92][6] = 1;
   fontSTPtr->pixAryUC[92][7] = 96;
   fontSTPtr->pixAryUC[92][8] = 0;
   fontSTPtr->pixAryUC[92][9] = 24;
   fontSTPtr->pixAryUC[92][10] = 0;
   fontSTPtr->pixAryUC[92][11] = 6;
   fontSTPtr->pixAryUC[92][12] = 128;
   fontSTPtr->pixAryUC[92][13] = 1;
   fontSTPtr->pixAryUC[92][14] = 96;
   fontSTPtr->pixAryUC[92][15] = 0;
   fontSTPtr->pixAryUC[92][16] = 24;
   fontSTPtr->pixAryUC[92][17] = 0;
   fontSTPtr->pixAryUC[92][18] = 6;
   fontSTPtr->pixAryUC[92][19] = 0;
   fontSTPtr->pixAryUC[92][20] = 0;
   fontSTPtr->pixAryUC[92][21] = 0;
   fontSTPtr->pixAryUC[92][22] = 0;
   fontSTPtr->pixAryUC[92][23] = 0;
   fontSTPtr->pixAryUC[92][24] = 0;

   return 0;
} /*addAscii124_normalFont*/

/*___________________________________________
` Fun93: addAscii125_normalFont
`  - adds character `}` (ascii 125) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii125_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[93])
      free(fontSTPtr->pixAryUC[93]);

   fontSTPtr->pixAryUC[93] =
     calloc(28, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[93])
      return 1;

   fontSTPtr->widthArySS[93] = 16;
   fontSTPtr->lenArySS[93] = 28;

   fontSTPtr->pixAryUC[93][0] = 0;
   fontSTPtr->pixAryUC[93][1] = 0;
   fontSTPtr->pixAryUC[93][2] = 252;
   fontSTPtr->pixAryUC[93][3] = 0;
   fontSTPtr->pixAryUC[93][4] = 248;
   fontSTPtr->pixAryUC[93][5] = 1;
   fontSTPtr->pixAryUC[93][6] = 0;
   fontSTPtr->pixAryUC[93][7] = 3;
   fontSTPtr->pixAryUC[93][8] = 0;
   fontSTPtr->pixAryUC[93][9] = 3;
   fontSTPtr->pixAryUC[93][10] = 0;
   fontSTPtr->pixAryUC[93][11] = 6;
   fontSTPtr->pixAryUC[93][12] = 0;
   fontSTPtr->pixAryUC[93][13] = 6;
   fontSTPtr->pixAryUC[93][14] = 0;
   fontSTPtr->pixAryUC[93][15] = 3;
   fontSTPtr->pixAryUC[93][16] = 0;
   fontSTPtr->pixAryUC[93][17] = 3;
   fontSTPtr->pixAryUC[93][18] = 248;
   fontSTPtr->pixAryUC[93][19] = 1;
   fontSTPtr->pixAryUC[93][20] = 252;
   fontSTPtr->pixAryUC[93][21] = 0;
   fontSTPtr->pixAryUC[93][22] = 0;
   fontSTPtr->pixAryUC[93][23] = 0;
   fontSTPtr->pixAryUC[93][24] = 0;
   fontSTPtr->pixAryUC[93][25] = 0;
   fontSTPtr->pixAryUC[93][26] = 0;
   fontSTPtr->pixAryUC[93][27] = 0;

   return 0;
} /*addAscii125_normalFont*/

/*___________________________________________
` Fun94: addAscii126_normalFont
`  - adds character `~` (ascii 126) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii126_normalFont(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[94])
      free(fontSTPtr->pixAryUC[94]);

   fontSTPtr->pixAryUC[94] =
     calloc(25, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[94])
      return 1;

   fontSTPtr->widthArySS[94] = 14;
   fontSTPtr->lenArySS[94] = 25;

   fontSTPtr->pixAryUC[94][0] = 0;
   fontSTPtr->pixAryUC[94][1] = 0;
   fontSTPtr->pixAryUC[94][2] = 0;
   fontSTPtr->pixAryUC[94][3] = 0;
   fontSTPtr->pixAryUC[94][4] = 0;
   fontSTPtr->pixAryUC[94][5] = 0;
   fontSTPtr->pixAryUC[94][6] = 0;
   fontSTPtr->pixAryUC[94][7] = 0;
   fontSTPtr->pixAryUC[94][8] = 0;
   fontSTPtr->pixAryUC[94][9] = 60;
   fontSTPtr->pixAryUC[94][10] = 196;
   fontSTPtr->pixAryUC[94][11] = 185;
   fontSTPtr->pixAryUC[94][12] = 25;
   fontSTPtr->pixAryUC[94][13] = 56;
   fontSTPtr->pixAryUC[94][14] = 0;
   fontSTPtr->pixAryUC[94][15] = 0;
   fontSTPtr->pixAryUC[94][16] = 0;
   fontSTPtr->pixAryUC[94][17] = 0;
   fontSTPtr->pixAryUC[94][18] = 0;
   fontSTPtr->pixAryUC[94][19] = 0;
   fontSTPtr->pixAryUC[94][20] = 0;
   fontSTPtr->pixAryUC[94][21] = 0;
   fontSTPtr->pixAryUC[94][22] = 0;
   fontSTPtr->pixAryUC[94][23] = 0;
   fontSTPtr->pixAryUC[94][24] = 0;

   return 0;
} /*addAscii126_normalFont*/

/*_____________________________________________
` Fun95: loadFont_normalFont
`  - loads the font in normalFont.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_normalFont(
   struct font_fontST *fontSTPtr
){
   fontSTPtr->heightSS = 14;
   fontSTPtr->gapSS = 0;

   if( addAscii32_normalFont(fontSTPtr) )
      return 1;
   if( addAscii33_normalFont(fontSTPtr) )
      return 1;
   if( addAscii34_normalFont(fontSTPtr) )
      return 1;
   if( addAscii35_normalFont(fontSTPtr) )
      return 1;
   if( addAscii36_normalFont(fontSTPtr) )
      return 1;
   if( addAscii37_normalFont(fontSTPtr) )
      return 1;
   if( addAscii38_normalFont(fontSTPtr) )
      return 1;
   if( addAscii39_normalFont(fontSTPtr) )
      return 1;
   if( addAscii40_normalFont(fontSTPtr) )
      return 1;
   if( addAscii41_normalFont(fontSTPtr) )
      return 1;
   if( addAscii42_normalFont(fontSTPtr) )
      return 1;
   if( addAscii43_normalFont(fontSTPtr) )
      return 1;
   if( addAscii44_normalFont(fontSTPtr) )
      return 1;
   if( addAscii45_normalFont(fontSTPtr) )
      return 1;
   if( addAscii46_normalFont(fontSTPtr) )
      return 1;
   if( addAscii47_normalFont(fontSTPtr) )
      return 1;
   if( addAscii48_normalFont(fontSTPtr) )
      return 1;
   if( addAscii49_normalFont(fontSTPtr) )
      return 1;
   if( addAscii50_normalFont(fontSTPtr) )
      return 1;
   if( addAscii51_normalFont(fontSTPtr) )
      return 1;
   if( addAscii52_normalFont(fontSTPtr) )
      return 1;
   if( addAscii53_normalFont(fontSTPtr) )
      return 1;
   if( addAscii54_normalFont(fontSTPtr) )
      return 1;
   if( addAscii55_normalFont(fontSTPtr) )
      return 1;
   if( addAscii56_normalFont(fontSTPtr) )
      return 1;
   if( addAscii57_normalFont(fontSTPtr) )
      return 1;
   if( addAscii58_normalFont(fontSTPtr) )
      return 1;
   if( addAscii59_normalFont(fontSTPtr) )
      return 1;
   if( addAscii60_normalFont(fontSTPtr) )
      return 1;
   if( addAscii61_normalFont(fontSTPtr) )
      return 1;
   if( addAscii62_normalFont(fontSTPtr) )
      return 1;
   if( addAscii63_normalFont(fontSTPtr) )
      return 1;
   if( addAscii64_normalFont(fontSTPtr) )
      return 1;
   if( addAscii65_normalFont(fontSTPtr) )
      return 1;
   if( addAscii66_normalFont(fontSTPtr) )
      return 1;
   if( addAscii67_normalFont(fontSTPtr) )
      return 1;
   if( addAscii68_normalFont(fontSTPtr) )
      return 1;
   if( addAscii69_normalFont(fontSTPtr) )
      return 1;
   if( addAscii70_normalFont(fontSTPtr) )
      return 1;
   if( addAscii71_normalFont(fontSTPtr) )
      return 1;
   if( addAscii72_normalFont(fontSTPtr) )
      return 1;
   if( addAscii73_normalFont(fontSTPtr) )
      return 1;
   if( addAscii74_normalFont(fontSTPtr) )
      return 1;
   if( addAscii75_normalFont(fontSTPtr) )
      return 1;
   if( addAscii76_normalFont(fontSTPtr) )
      return 1;
   if( addAscii77_normalFont(fontSTPtr) )
      return 1;
   if( addAscii78_normalFont(fontSTPtr) )
      return 1;
   if( addAscii79_normalFont(fontSTPtr) )
      return 1;
   if( addAscii80_normalFont(fontSTPtr) )
      return 1;
   if( addAscii81_normalFont(fontSTPtr) )
      return 1;
   if( addAscii82_normalFont(fontSTPtr) )
      return 1;
   if( addAscii83_normalFont(fontSTPtr) )
      return 1;
   if( addAscii84_normalFont(fontSTPtr) )
      return 1;
   if( addAscii85_normalFont(fontSTPtr) )
      return 1;
   if( addAscii86_normalFont(fontSTPtr) )
      return 1;
   if( addAscii87_normalFont(fontSTPtr) )
      return 1;
   if( addAscii88_normalFont(fontSTPtr) )
      return 1;
   if( addAscii89_normalFont(fontSTPtr) )
      return 1;
   if( addAscii90_normalFont(fontSTPtr) )
      return 1;
   if( addAscii91_normalFont(fontSTPtr) )
      return 1;
   if( addAscii92_normalFont(fontSTPtr) )
      return 1;
   if( addAscii93_normalFont(fontSTPtr) )
      return 1;
   if( addAscii94_normalFont(fontSTPtr) )
      return 1;
   if( addAscii95_normalFont(fontSTPtr) )
      return 1;
   if( addAscii96_normalFont(fontSTPtr) )
      return 1;
   if( addAscii97_normalFont(fontSTPtr) )
      return 1;
   if( addAscii98_normalFont(fontSTPtr) )
      return 1;
   if( addAscii99_normalFont(fontSTPtr) )
      return 1;
   if( addAscii100_normalFont(fontSTPtr) )
      return 1;
   if( addAscii101_normalFont(fontSTPtr) )
      return 1;
   if( addAscii102_normalFont(fontSTPtr) )
      return 1;
   if( addAscii103_normalFont(fontSTPtr) )
      return 1;
   if( addAscii104_normalFont(fontSTPtr) )
      return 1;
   if( addAscii105_normalFont(fontSTPtr) )
      return 1;
   if( addAscii106_normalFont(fontSTPtr) )
      return 1;
   if( addAscii107_normalFont(fontSTPtr) )
      return 1;
   if( addAscii108_normalFont(fontSTPtr) )
      return 1;
   if( addAscii109_normalFont(fontSTPtr) )
      return 1;
   if( addAscii110_normalFont(fontSTPtr) )
      return 1;
   if( addAscii111_normalFont(fontSTPtr) )
      return 1;
   if( addAscii112_normalFont(fontSTPtr) )
      return 1;
   if( addAscii113_normalFont(fontSTPtr) )
      return 1;
   if( addAscii114_normalFont(fontSTPtr) )
      return 1;
   if( addAscii115_normalFont(fontSTPtr) )
      return 1;
   if( addAscii116_normalFont(fontSTPtr) )
      return 1;
   if( addAscii117_normalFont(fontSTPtr) )
      return 1;
   if( addAscii118_normalFont(fontSTPtr) )
      return 1;
   if( addAscii119_normalFont(fontSTPtr) )
      return 1;
   if( addAscii120_normalFont(fontSTPtr) )
      return 1;
   if( addAscii121_normalFont(fontSTPtr) )
      return 1;
   if( addAscii122_normalFont(fontSTPtr) )
      return 1;
   if( addAscii123_normalFont(fontSTPtr) )
      return 1;
   if( addAscii124_normalFont(fontSTPtr) )
      return 1;
   if( addAscii125_normalFont(fontSTPtr) )
      return 1;
   if( addAscii126_normalFont(fontSTPtr) )
      return 1;

   return 0;
} /*loadFont_normalFont*/
