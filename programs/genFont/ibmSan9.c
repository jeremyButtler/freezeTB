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
` Fun00: addAscii32_ibmSan9
`  - adds character ` ` (ascii 32) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii32_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[0])
      free(fontSTPtr->pixAryUC[0]);

   fontSTPtr->pixAryUC[0] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[0])
      return 1;

   fontSTPtr->widthArySS[0] = 3;
   fontSTPtr->lenArySS[0] = 4;

   fontSTPtr->pixAryUC[0][0] = 0;
   fontSTPtr->pixAryUC[0][1] = 0;
   fontSTPtr->pixAryUC[0][2] = 0;
   fontSTPtr->pixAryUC[0][3] = 0;

   return 0;
} /*addAscii32_ibmSan9*/

/*___________________________________________
` Fun01: addAscii33_ibmSan9
`  - adds character `!` (ascii 33) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii33_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[1])
      free(fontSTPtr->pixAryUC[1]);

   fontSTPtr->pixAryUC[1] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[1])
      return 1;

   fontSTPtr->widthArySS[1] = 3;
   fontSTPtr->lenArySS[1] = 4;

   fontSTPtr->pixAryUC[1][0] = 146;
   fontSTPtr->pixAryUC[1][1] = 36;
   fontSTPtr->pixAryUC[1][2] = 65;
   fontSTPtr->pixAryUC[1][3] = 2;

   return 0;
} /*addAscii33_ibmSan9*/

/*___________________________________________
` Fun02: addAscii34_ibmSan9
`  - adds character `"` (ascii 34) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii34_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[2])
      free(fontSTPtr->pixAryUC[2]);

   fontSTPtr->pixAryUC[2] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[2])
      return 1;

   fontSTPtr->widthArySS[2] = 5;
   fontSTPtr->lenArySS[2] = 6;

   fontSTPtr->pixAryUC[2][0] = 165;
   fontSTPtr->pixAryUC[2][1] = 20;
   fontSTPtr->pixAryUC[2][2] = 0;
   fontSTPtr->pixAryUC[2][3] = 0;
   fontSTPtr->pixAryUC[2][4] = 0;
   fontSTPtr->pixAryUC[2][5] = 0;

   return 0;
} /*addAscii34_ibmSan9*/

/*___________________________________________
` Fun03: addAscii35_ibmSan9
`  - adds character `#` (ascii 35) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii35_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[3])
      free(fontSTPtr->pixAryUC[3]);

   fontSTPtr->pixAryUC[3] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[3])
      return 1;

   fontSTPtr->widthArySS[3] = 7;
   fontSTPtr->lenArySS[3] = 8;

   fontSTPtr->pixAryUC[3][0] = 20;
   fontSTPtr->pixAryUC[3][1] = 202;
   fontSTPtr->pixAryUC[3][2] = 159;
   fontSTPtr->pixAryUC[3][3] = 66;
   fontSTPtr->pixAryUC[3][4] = 249;
   fontSTPtr->pixAryUC[3][5] = 83;
   fontSTPtr->pixAryUC[3][6] = 40;
   fontSTPtr->pixAryUC[3][7] = 0;

   return 0;
} /*addAscii35_ibmSan9*/

/*___________________________________________
` Fun04: addAscii36_ibmSan9
`  - adds character `$` (ascii 36) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii36_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[4])
      free(fontSTPtr->pixAryUC[4]);

   fontSTPtr->pixAryUC[4] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[4])
      return 1;

   fontSTPtr->widthArySS[4] = 7;
   fontSTPtr->lenArySS[4] = 8;

   fontSTPtr->pixAryUC[4][0] = 4;
   fontSTPtr->pixAryUC[4][1] = 95;
   fontSTPtr->pixAryUC[4][2] = 161;
   fontSTPtr->pixAryUC[4][3] = 224;
   fontSTPtr->pixAryUC[4][4] = 163;
   fontSTPtr->pixAryUC[4][5] = 80;
   fontSTPtr->pixAryUC[4][6] = 124;
   fontSTPtr->pixAryUC[4][7] = 4;

   return 0;
} /*addAscii36_ibmSan9*/

/*___________________________________________
` Fun05: addAscii37_ibmSan9
`  - adds character `%` (ascii 37) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii37_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[5])
      free(fontSTPtr->pixAryUC[5]);

   fontSTPtr->pixAryUC[5] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[5])
      return 1;

   fontSTPtr->widthArySS[5] = 9;
   fontSTPtr->lenArySS[5] = 11;

   fontSTPtr->pixAryUC[5][0] = 131;
   fontSTPtr->pixAryUC[5][1] = 134;
   fontSTPtr->pixAryUC[5][2] = 128;
   fontSTPtr->pixAryUC[5][3] = 128;
   fontSTPtr->pixAryUC[5][4] = 128;
   fontSTPtr->pixAryUC[5][5] = 128;
   fontSTPtr->pixAryUC[5][6] = 128;
   fontSTPtr->pixAryUC[5][7] = 128;
   fontSTPtr->pixAryUC[5][8] = 96;
   fontSTPtr->pixAryUC[5][9] = 193;
   fontSTPtr->pixAryUC[5][10] = 0;

   return 0;
} /*addAscii37_ibmSan9*/

/*___________________________________________
` Fun06: addAscii38_ibmSan9
`  - adds character `&` (ascii 38) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii38_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[6])
      free(fontSTPtr->pixAryUC[6]);

   fontSTPtr->pixAryUC[6] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[6])
      return 1;

   fontSTPtr->widthArySS[6] = 8;
   fontSTPtr->lenArySS[6] = 9;

   fontSTPtr->pixAryUC[6][0] = 28;
   fontSTPtr->pixAryUC[6][1] = 34;
   fontSTPtr->pixAryUC[6][2] = 34;
   fontSTPtr->pixAryUC[6][3] = 28;
   fontSTPtr->pixAryUC[6][4] = 18;
   fontSTPtr->pixAryUC[6][5] = 17;
   fontSTPtr->pixAryUC[6][6] = 17;
   fontSTPtr->pixAryUC[6][7] = 50;
   fontSTPtr->pixAryUC[6][8] = 44;

   return 0;
} /*addAscii38_ibmSan9*/

/*___________________________________________
` Fun07: addAscii39_ibmSan9
`  - adds character `'` (ascii 39) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii39_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[7])
      free(fontSTPtr->pixAryUC[7]);

   fontSTPtr->pixAryUC[7] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[7])
      return 1;

   fontSTPtr->widthArySS[7] = 3;
   fontSTPtr->lenArySS[7] = 4;

   fontSTPtr->pixAryUC[7][0] = 146;
   fontSTPtr->pixAryUC[7][1] = 0;
   fontSTPtr->pixAryUC[7][2] = 0;
   fontSTPtr->pixAryUC[7][3] = 0;

   return 0;
} /*addAscii39_ibmSan9*/

/*___________________________________________
` Fun08: addAscii40_ibmSan9
`  - adds character `(` (ascii 40) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii40_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[8])
      free(fontSTPtr->pixAryUC[8]);

   fontSTPtr->pixAryUC[8] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[8])
      return 1;

   fontSTPtr->widthArySS[8] = 4;
   fontSTPtr->lenArySS[8] = 5;

   fontSTPtr->pixAryUC[8][0] = 36;
   fontSTPtr->pixAryUC[8][1] = 34;
   fontSTPtr->pixAryUC[8][2] = 34;
   fontSTPtr->pixAryUC[8][3] = 34;
   fontSTPtr->pixAryUC[8][4] = 4;

   return 0;
} /*addAscii40_ibmSan9*/

/*___________________________________________
` Fun09: addAscii41_ibmSan9
`  - adds character `)` (ascii 41) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii41_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[9])
      free(fontSTPtr->pixAryUC[9]);

   fontSTPtr->pixAryUC[9] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[9])
      return 1;

   fontSTPtr->widthArySS[9] = 4;
   fontSTPtr->lenArySS[9] = 5;

   fontSTPtr->pixAryUC[9][0] = 33;
   fontSTPtr->pixAryUC[9][1] = 34;
   fontSTPtr->pixAryUC[9][2] = 34;
   fontSTPtr->pixAryUC[9][3] = 34;
   fontSTPtr->pixAryUC[9][4] = 1;

   return 0;
} /*addAscii41_ibmSan9*/

/*___________________________________________
` Fun10: addAscii42_ibmSan9
`  - adds character `*` (ascii 42) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii42_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[10])
      free(fontSTPtr->pixAryUC[10]);

   fontSTPtr->pixAryUC[10] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[10])
      return 1;

   fontSTPtr->widthArySS[10] = 7;
   fontSTPtr->lenArySS[10] = 8;

   fontSTPtr->pixAryUC[10][0] = 8;
   fontSTPtr->pixAryUC[10][1] = 21;
   fontSTPtr->pixAryUC[10][2] = 231;
   fontSTPtr->pixAryUC[10][3] = 207;
   fontSTPtr->pixAryUC[10][4] = 81;
   fontSTPtr->pixAryUC[10][5] = 33;
   fontSTPtr->pixAryUC[10][6] = 0;
   fontSTPtr->pixAryUC[10][7] = 0;

   return 0;
} /*addAscii42_ibmSan9*/

/*___________________________________________
` Fun11: addAscii43_ibmSan9
`  - adds character `+` (ascii 43) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii43_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[11])
      free(fontSTPtr->pixAryUC[11]);

   fontSTPtr->pixAryUC[11] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[11])
      return 1;

   fontSTPtr->widthArySS[11] = 7;
   fontSTPtr->lenArySS[11] = 8;

   fontSTPtr->pixAryUC[11][0] = 0;
   fontSTPtr->pixAryUC[11][1] = 4;
   fontSTPtr->pixAryUC[11][2] = 2;
   fontSTPtr->pixAryUC[11][3] = 241;
   fontSTPtr->pixAryUC[11][4] = 71;
   fontSTPtr->pixAryUC[11][5] = 32;
   fontSTPtr->pixAryUC[11][6] = 16;
   fontSTPtr->pixAryUC[11][7] = 0;

   return 0;
} /*addAscii43_ibmSan9*/

/*___________________________________________
` Fun12: addAscii44_ibmSan9
`  - adds character `,` (ascii 44) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii44_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[12])
      free(fontSTPtr->pixAryUC[12]);

   fontSTPtr->pixAryUC[12] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[12])
      return 1;

   fontSTPtr->widthArySS[12] = 3;
   fontSTPtr->lenArySS[12] = 4;

   fontSTPtr->pixAryUC[12][0] = 0;
   fontSTPtr->pixAryUC[12][1] = 0;
   fontSTPtr->pixAryUC[12][2] = 72;
   fontSTPtr->pixAryUC[12][3] = 1;

   return 0;
} /*addAscii44_ibmSan9*/

/*___________________________________________
` Fun13: addAscii45_ibmSan9
`  - adds character `-` (ascii 45) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii45_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[13])
      free(fontSTPtr->pixAryUC[13]);

   fontSTPtr->pixAryUC[13] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[13])
      return 1;

   fontSTPtr->widthArySS[13] = 5;
   fontSTPtr->lenArySS[13] = 6;

   fontSTPtr->pixAryUC[13][0] = 0;
   fontSTPtr->pixAryUC[13][1] = 128;
   fontSTPtr->pixAryUC[13][2] = 15;
   fontSTPtr->pixAryUC[13][3] = 0;
   fontSTPtr->pixAryUC[13][4] = 0;
   fontSTPtr->pixAryUC[13][5] = 0;

   return 0;
} /*addAscii45_ibmSan9*/

/*___________________________________________
` Fun14: addAscii46_ibmSan9
`  - adds character `.` (ascii 46) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii46_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[14])
      free(fontSTPtr->pixAryUC[14]);

   fontSTPtr->pixAryUC[14] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[14])
      return 1;

   fontSTPtr->widthArySS[14] = 3;
   fontSTPtr->lenArySS[14] = 4;

   fontSTPtr->pixAryUC[14][0] = 0;
   fontSTPtr->pixAryUC[14][1] = 0;
   fontSTPtr->pixAryUC[14][2] = 64;
   fontSTPtr->pixAryUC[14][3] = 2;

   return 0;
} /*addAscii46_ibmSan9*/

/*___________________________________________
` Fun15: addAscii47_ibmSan9
`  - adds character `/` (ascii 47) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii47_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[15])
      free(fontSTPtr->pixAryUC[15]);

   fontSTPtr->pixAryUC[15] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[15])
      return 1;

   fontSTPtr->widthArySS[15] = 6;
   fontSTPtr->lenArySS[15] = 7;

   fontSTPtr->pixAryUC[15][0] = 32;
   fontSTPtr->pixAryUC[15][1] = 4;
   fontSTPtr->pixAryUC[15][2] = 33;
   fontSTPtr->pixAryUC[15][3] = 8;
   fontSTPtr->pixAryUC[15][4] = 65;
   fontSTPtr->pixAryUC[15][5] = 8;
   fontSTPtr->pixAryUC[15][6] = 2;

   return 0;
} /*addAscii47_ibmSan9*/

/*___________________________________________
` Fun16: addAscii48_ibmSan9
`  - adds character `0` (ascii 48) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii48_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[16])
      free(fontSTPtr->pixAryUC[16]);

   fontSTPtr->pixAryUC[16] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[16])
      return 1;

   fontSTPtr->widthArySS[16] = 7;
   fontSTPtr->lenArySS[16] = 8;

   fontSTPtr->pixAryUC[16][0] = 190;
   fontSTPtr->pixAryUC[16][1] = 96;
   fontSTPtr->pixAryUC[16][2] = 48;
   fontSTPtr->pixAryUC[16][3] = 24;
   fontSTPtr->pixAryUC[16][4] = 12;
   fontSTPtr->pixAryUC[16][5] = 6;
   fontSTPtr->pixAryUC[16][6] = 131;
   fontSTPtr->pixAryUC[16][7] = 62;

   return 0;
} /*addAscii48_ibmSan9*/

/*___________________________________________
` Fun17: addAscii49_ibmSan9
`  - adds character `1` (ascii 49) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii49_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[17])
      free(fontSTPtr->pixAryUC[17]);

   fontSTPtr->pixAryUC[17] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[17])
      return 1;

   fontSTPtr->widthArySS[17] = 6;
   fontSTPtr->lenArySS[17] = 7;

   fontSTPtr->pixAryUC[17][0] = 140;
   fontSTPtr->pixAryUC[17][1] = 146;
   fontSTPtr->pixAryUC[17][2] = 32;
   fontSTPtr->pixAryUC[17][3] = 8;
   fontSTPtr->pixAryUC[17][4] = 130;
   fontSTPtr->pixAryUC[17][5] = 32;
   fontSTPtr->pixAryUC[17][6] = 62;

   return 0;
} /*addAscii49_ibmSan9*/

/*___________________________________________
` Fun18: addAscii50_ibmSan9
`  - adds character `2` (ascii 50) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii50_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[18])
      free(fontSTPtr->pixAryUC[18]);

   fontSTPtr->pixAryUC[18] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[18])
      return 1;

   fontSTPtr->widthArySS[18] = 7;
   fontSTPtr->lenArySS[18] = 8;

   fontSTPtr->pixAryUC[18][0] = 190;
   fontSTPtr->pixAryUC[18][1] = 32;
   fontSTPtr->pixAryUC[18][2] = 16;
   fontSTPtr->pixAryUC[18][3] = 196;
   fontSTPtr->pixAryUC[18][4] = 17;
   fontSTPtr->pixAryUC[18][5] = 4;
   fontSTPtr->pixAryUC[18][6] = 2;
   fontSTPtr->pixAryUC[18][7] = 127;

   return 0;
} /*addAscii50_ibmSan9*/

/*___________________________________________
` Fun19: addAscii51_ibmSan9
`  - adds character `3` (ascii 51) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii51_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[19])
      free(fontSTPtr->pixAryUC[19]);

   fontSTPtr->pixAryUC[19] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[19])
      return 1;

   fontSTPtr->widthArySS[19] = 7;
   fontSTPtr->lenArySS[19] = 8;

   fontSTPtr->pixAryUC[19][0] = 190;
   fontSTPtr->pixAryUC[19][1] = 32;
   fontSTPtr->pixAryUC[19][2] = 16;
   fontSTPtr->pixAryUC[19][3] = 196;
   fontSTPtr->pixAryUC[19][4] = 1;
   fontSTPtr->pixAryUC[19][5] = 1;
   fontSTPtr->pixAryUC[19][6] = 131;
   fontSTPtr->pixAryUC[19][7] = 62;

   return 0;
} /*addAscii51_ibmSan9*/

/*___________________________________________
` Fun20: addAscii52_ibmSan9
`  - adds character `4` (ascii 52) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii52_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[20])
      free(fontSTPtr->pixAryUC[20]);

   fontSTPtr->pixAryUC[20] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[20])
      return 1;

   fontSTPtr->widthArySS[20] = 7;
   fontSTPtr->lenArySS[20] = 8;

   fontSTPtr->pixAryUC[20][0] = 161;
   fontSTPtr->pixAryUC[20][1] = 80;
   fontSTPtr->pixAryUC[20][2] = 40;
   fontSTPtr->pixAryUC[20][3] = 244;
   fontSTPtr->pixAryUC[20][4] = 7;
   fontSTPtr->pixAryUC[20][5] = 129;
   fontSTPtr->pixAryUC[20][6] = 64;
   fontSTPtr->pixAryUC[20][7] = 32;

   return 0;
} /*addAscii52_ibmSan9*/

/*___________________________________________
` Fun21: addAscii53_ibmSan9
`  - adds character `5` (ascii 53) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii53_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[21])
      free(fontSTPtr->pixAryUC[21]);

   fontSTPtr->pixAryUC[21] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[21])
      return 1;

   fontSTPtr->widthArySS[21] = 7;
   fontSTPtr->lenArySS[21] = 8;

   fontSTPtr->pixAryUC[21][0] = 255;
   fontSTPtr->pixAryUC[21][1] = 64;
   fontSTPtr->pixAryUC[21][2] = 224;
   fontSTPtr->pixAryUC[21][3] = 3;
   fontSTPtr->pixAryUC[21][4] = 4;
   fontSTPtr->pixAryUC[21][5] = 2;
   fontSTPtr->pixAryUC[21][6] = 131;
   fontSTPtr->pixAryUC[21][7] = 62;

   return 0;
} /*addAscii53_ibmSan9*/

/*___________________________________________
` Fun22: addAscii54_ibmSan9
`  - adds character `6` (ascii 54) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii54_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[22])
      free(fontSTPtr->pixAryUC[22]);

   fontSTPtr->pixAryUC[22] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[22])
      return 1;

   fontSTPtr->widthArySS[22] = 7;
   fontSTPtr->lenArySS[22] = 8;

   fontSTPtr->pixAryUC[22][0] = 190;
   fontSTPtr->pixAryUC[22][1] = 96;
   fontSTPtr->pixAryUC[22][2] = 32;
   fontSTPtr->pixAryUC[22][3] = 240;
   fontSTPtr->pixAryUC[22][4] = 11;
   fontSTPtr->pixAryUC[22][5] = 6;
   fontSTPtr->pixAryUC[22][6] = 131;
   fontSTPtr->pixAryUC[22][7] = 62;

   return 0;
} /*addAscii54_ibmSan9*/

/*___________________________________________
` Fun23: addAscii55_ibmSan9
`  - adds character `7` (ascii 55) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii55_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[23])
      free(fontSTPtr->pixAryUC[23]);

   fontSTPtr->pixAryUC[23] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[23])
      return 1;

   fontSTPtr->widthArySS[23] = 8;
   fontSTPtr->lenArySS[23] = 9;

   fontSTPtr->pixAryUC[23][0] = 127;
   fontSTPtr->pixAryUC[23][1] = 32;
   fontSTPtr->pixAryUC[23][2] = 16;
   fontSTPtr->pixAryUC[23][3] = 255;
   fontSTPtr->pixAryUC[23][4] = 8;
   fontSTPtr->pixAryUC[23][5] = 8;
   fontSTPtr->pixAryUC[23][6] = 4;
   fontSTPtr->pixAryUC[23][7] = 4;
   fontSTPtr->pixAryUC[23][8] = 4;

   return 0;
} /*addAscii55_ibmSan9*/

/*___________________________________________
` Fun24: addAscii56_ibmSan9
`  - adds character `8` (ascii 56) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii56_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[24])
      free(fontSTPtr->pixAryUC[24]);

   fontSTPtr->pixAryUC[24] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[24])
      return 1;

   fontSTPtr->widthArySS[24] = 7;
   fontSTPtr->lenArySS[24] = 8;

   fontSTPtr->pixAryUC[24][0] = 190;
   fontSTPtr->pixAryUC[24][1] = 96;
   fontSTPtr->pixAryUC[24][2] = 48;
   fontSTPtr->pixAryUC[24][3] = 232;
   fontSTPtr->pixAryUC[24][4] = 11;
   fontSTPtr->pixAryUC[24][5] = 6;
   fontSTPtr->pixAryUC[24][6] = 131;
   fontSTPtr->pixAryUC[24][7] = 62;

   return 0;
} /*addAscii56_ibmSan9*/

/*___________________________________________
` Fun25: addAscii57_ibmSan9
`  - adds character `9` (ascii 57) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii57_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[25])
      free(fontSTPtr->pixAryUC[25]);

   fontSTPtr->pixAryUC[25] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[25])
      return 1;

   fontSTPtr->widthArySS[25] = 7;
   fontSTPtr->lenArySS[25] = 8;

   fontSTPtr->pixAryUC[25][0] = 190;
   fontSTPtr->pixAryUC[25][1] = 96;
   fontSTPtr->pixAryUC[25][2] = 48;
   fontSTPtr->pixAryUC[25][3] = 232;
   fontSTPtr->pixAryUC[25][4] = 7;
   fontSTPtr->pixAryUC[25][5] = 2;
   fontSTPtr->pixAryUC[25][6] = 131;
   fontSTPtr->pixAryUC[25][7] = 62;

   return 0;
} /*addAscii57_ibmSan9*/

/*___________________________________________
` Fun26: addAscii58_ibmSan9
`  - adds character `:` (ascii 58) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii58_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[26])
      free(fontSTPtr->pixAryUC[26]);

   fontSTPtr->pixAryUC[26] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[26])
      return 1;

   fontSTPtr->widthArySS[26] = 3;
   fontSTPtr->lenArySS[26] = 4;

   fontSTPtr->pixAryUC[26][0] = 144;
   fontSTPtr->pixAryUC[26][1] = 0;
   fontSTPtr->pixAryUC[26][2] = 72;
   fontSTPtr->pixAryUC[26][3] = 0;

   return 0;
} /*addAscii58_ibmSan9*/

/*___________________________________________
` Fun27: addAscii59_ibmSan9
`  - adds character `;` (ascii 59) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii59_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[27])
      free(fontSTPtr->pixAryUC[27]);

   fontSTPtr->pixAryUC[27] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[27])
      return 1;

   fontSTPtr->widthArySS[27] = 3;
   fontSTPtr->lenArySS[27] = 4;

   fontSTPtr->pixAryUC[27][0] = 144;
   fontSTPtr->pixAryUC[27][1] = 0;
   fontSTPtr->pixAryUC[27][2] = 72;
   fontSTPtr->pixAryUC[27][3] = 1;

   return 0;
} /*addAscii59_ibmSan9*/

/*___________________________________________
` Fun28: addAscii60_ibmSan9
`  - adds character `<` (ascii 60) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii60_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[28])
      free(fontSTPtr->pixAryUC[28]);

   fontSTPtr->pixAryUC[28] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[28])
      return 1;

   fontSTPtr->widthArySS[28] = 6;
   fontSTPtr->lenArySS[28] = 7;

   fontSTPtr->pixAryUC[28][0] = 16;
   fontSTPtr->pixAryUC[28][1] = 66;
   fontSTPtr->pixAryUC[28][2] = 8;
   fontSTPtr->pixAryUC[28][3] = 2;
   fontSTPtr->pixAryUC[28][4] = 129;
   fontSTPtr->pixAryUC[28][5] = 64;
   fontSTPtr->pixAryUC[28][6] = 0;

   return 0;
} /*addAscii60_ibmSan9*/

/*___________________________________________
` Fun29: addAscii61_ibmSan9
`  - adds character `=` (ascii 61) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii61_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[29])
      free(fontSTPtr->pixAryUC[29]);

   fontSTPtr->pixAryUC[29] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[29])
      return 1;

   fontSTPtr->widthArySS[29] = 7;
   fontSTPtr->lenArySS[29] = 8;

   fontSTPtr->pixAryUC[29][0] = 128;
   fontSTPtr->pixAryUC[29][1] = 63;
   fontSTPtr->pixAryUC[29][2] = 0;
   fontSTPtr->pixAryUC[29][3] = 240;
   fontSTPtr->pixAryUC[29][4] = 7;
   fontSTPtr->pixAryUC[29][5] = 0;
   fontSTPtr->pixAryUC[29][6] = 0;
   fontSTPtr->pixAryUC[29][7] = 0;

   return 0;
} /*addAscii61_ibmSan9*/

/*___________________________________________
` Fun30: addAscii62_ibmSan9
`  - adds character `>` (ascii 62) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii62_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[30])
      free(fontSTPtr->pixAryUC[30]);

   fontSTPtr->pixAryUC[30] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[30])
      return 1;

   fontSTPtr->widthArySS[30] = 6;
   fontSTPtr->lenArySS[30] = 7;

   fontSTPtr->pixAryUC[30][0] = 129;
   fontSTPtr->pixAryUC[30][1] = 64;
   fontSTPtr->pixAryUC[30][2] = 32;
   fontSTPtr->pixAryUC[30][3] = 8;
   fontSTPtr->pixAryUC[30][4] = 33;
   fontSTPtr->pixAryUC[30][5] = 4;
   fontSTPtr->pixAryUC[30][6] = 0;

   return 0;
} /*addAscii62_ibmSan9*/

/*___________________________________________
` Fun31: addAscii63_ibmSan9
`  - adds character `?` (ascii 63) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii63_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[31])
      free(fontSTPtr->pixAryUC[31]);

   fontSTPtr->pixAryUC[31] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[31])
      return 1;

   fontSTPtr->widthArySS[31] = 7;
   fontSTPtr->lenArySS[31] = 8;

   fontSTPtr->pixAryUC[31][0] = 190;
   fontSTPtr->pixAryUC[31][1] = 32;
   fontSTPtr->pixAryUC[31][2] = 16;
   fontSTPtr->pixAryUC[31][3] = 132;
   fontSTPtr->pixAryUC[31][4] = 65;
   fontSTPtr->pixAryUC[31][5] = 0;
   fontSTPtr->pixAryUC[31][6] = 16;
   fontSTPtr->pixAryUC[31][7] = 8;

   return 0;
} /*addAscii63_ibmSan9*/

/*___________________________________________
` Fun32: addAscii64_ibmSan9
`  - adds character `@` (ascii 64) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii64_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[32])
      free(fontSTPtr->pixAryUC[32]);

   fontSTPtr->pixAryUC[32] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[32])
      return 1;

   fontSTPtr->widthArySS[32] = 9;
   fontSTPtr->lenArySS[32] = 11;

   fontSTPtr->pixAryUC[32][0] = 124;
   fontSTPtr->pixAryUC[32][1] = 4;
   fontSTPtr->pixAryUC[32][2] = 229;
   fontSTPtr->pixAryUC[32][3] = 44;
   fontSTPtr->pixAryUC[32][4] = 90;
   fontSTPtr->pixAryUC[32][5] = 180;
   fontSTPtr->pixAryUC[32][6] = 104;
   fontSTPtr->pixAryUC[32][7] = 110;
   fontSTPtr->pixAryUC[32][8] = 1;
   fontSTPtr->pixAryUC[32][9] = 124;
   fontSTPtr->pixAryUC[32][10] = 0;

   return 0;
} /*addAscii64_ibmSan9*/

/*___________________________________________
` Fun33: addAscii65_ibmSan9
`  - adds character `A` (ascii 65) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii65_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[33])
      free(fontSTPtr->pixAryUC[33]);

   fontSTPtr->pixAryUC[33] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[33])
      return 1;

   fontSTPtr->widthArySS[33] = 7;
   fontSTPtr->lenArySS[33] = 8;

   fontSTPtr->pixAryUC[33][0] = 8;
   fontSTPtr->pixAryUC[33][1] = 138;
   fontSTPtr->pixAryUC[33][2] = 72;
   fontSTPtr->pixAryUC[33][3] = 228;
   fontSTPtr->pixAryUC[33][4] = 19;
   fontSTPtr->pixAryUC[33][5] = 137;
   fontSTPtr->pixAryUC[33][6] = 68;
   fontSTPtr->pixAryUC[33][7] = 34;

   return 0;
} /*addAscii65_ibmSan9*/

/*___________________________________________
` Fun34: addAscii66_ibmSan9
`  - adds character `B` (ascii 66) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii66_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[34])
      free(fontSTPtr->pixAryUC[34]);

   fontSTPtr->pixAryUC[34] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[34])
      return 1;

   fontSTPtr->widthArySS[34] = 7;
   fontSTPtr->lenArySS[34] = 8;

   fontSTPtr->pixAryUC[34][0] = 191;
   fontSTPtr->pixAryUC[34][1] = 96;
   fontSTPtr->pixAryUC[34][2] = 48;
   fontSTPtr->pixAryUC[34][3] = 248;
   fontSTPtr->pixAryUC[34][4] = 11;
   fontSTPtr->pixAryUC[34][5] = 6;
   fontSTPtr->pixAryUC[34][6] = 131;
   fontSTPtr->pixAryUC[34][7] = 63;

   return 0;
} /*addAscii66_ibmSan9*/

/*___________________________________________
` Fun35: addAscii67_ibmSan9
`  - adds character `C` (ascii 67) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii67_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[35])
      free(fontSTPtr->pixAryUC[35]);

   fontSTPtr->pixAryUC[35] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[35])
      return 1;

   fontSTPtr->widthArySS[35] = 7;
   fontSTPtr->lenArySS[35] = 8;

   fontSTPtr->pixAryUC[35][0] = 190;
   fontSTPtr->pixAryUC[35][1] = 96;
   fontSTPtr->pixAryUC[35][2] = 32;
   fontSTPtr->pixAryUC[35][3] = 16;
   fontSTPtr->pixAryUC[35][4] = 8;
   fontSTPtr->pixAryUC[35][5] = 4;
   fontSTPtr->pixAryUC[35][6] = 130;
   fontSTPtr->pixAryUC[35][7] = 62;

   return 0;
} /*addAscii67_ibmSan9*/

/*___________________________________________
` Fun36: addAscii68_ibmSan9
`  - adds character `D` (ascii 68) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii68_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[36])
      free(fontSTPtr->pixAryUC[36]);

   fontSTPtr->pixAryUC[36] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[36])
      return 1;

   fontSTPtr->widthArySS[36] = 7;
   fontSTPtr->lenArySS[36] = 8;

   fontSTPtr->pixAryUC[36][0] = 159;
   fontSTPtr->pixAryUC[36][1] = 80;
   fontSTPtr->pixAryUC[36][2] = 48;
   fontSTPtr->pixAryUC[36][3] = 24;
   fontSTPtr->pixAryUC[36][4] = 12;
   fontSTPtr->pixAryUC[36][5] = 6;
   fontSTPtr->pixAryUC[36][6] = 67;
   fontSTPtr->pixAryUC[36][7] = 31;

   return 0;
} /*addAscii68_ibmSan9*/

/*___________________________________________
` Fun37: addAscii69_ibmSan9
`  - adds character `E` (ascii 69) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii69_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[37])
      free(fontSTPtr->pixAryUC[37]);

   fontSTPtr->pixAryUC[37] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[37])
      return 1;

   fontSTPtr->widthArySS[37] = 7;
   fontSTPtr->lenArySS[37] = 8;

   fontSTPtr->pixAryUC[37][0] = 255;
   fontSTPtr->pixAryUC[37][1] = 64;
   fontSTPtr->pixAryUC[37][2] = 32;
   fontSTPtr->pixAryUC[37][3] = 240;
   fontSTPtr->pixAryUC[37][4] = 9;
   fontSTPtr->pixAryUC[37][5] = 4;
   fontSTPtr->pixAryUC[37][6] = 2;
   fontSTPtr->pixAryUC[37][7] = 127;

   return 0;
} /*addAscii69_ibmSan9*/

/*___________________________________________
` Fun38: addAscii70_ibmSan9
`  - adds character `F` (ascii 70) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii70_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[38])
      free(fontSTPtr->pixAryUC[38]);

   fontSTPtr->pixAryUC[38] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[38])
      return 1;

   fontSTPtr->widthArySS[38] = 7;
   fontSTPtr->lenArySS[38] = 8;

   fontSTPtr->pixAryUC[38][0] = 255;
   fontSTPtr->pixAryUC[38][1] = 64;
   fontSTPtr->pixAryUC[38][2] = 32;
   fontSTPtr->pixAryUC[38][3] = 240;
   fontSTPtr->pixAryUC[38][4] = 9;
   fontSTPtr->pixAryUC[38][5] = 4;
   fontSTPtr->pixAryUC[38][6] = 2;
   fontSTPtr->pixAryUC[38][7] = 1;

   return 0;
} /*addAscii70_ibmSan9*/

/*___________________________________________
` Fun39: addAscii71_ibmSan9
`  - adds character `G` (ascii 71) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii71_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[39])
      free(fontSTPtr->pixAryUC[39]);

   fontSTPtr->pixAryUC[39] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[39])
      return 1;

   fontSTPtr->widthArySS[39] = 7;
   fontSTPtr->lenArySS[39] = 8;

   fontSTPtr->pixAryUC[39][0] = 190;
   fontSTPtr->pixAryUC[39][1] = 96;
   fontSTPtr->pixAryUC[39][2] = 32;
   fontSTPtr->pixAryUC[39][3] = 16;
   fontSTPtr->pixAryUC[39][4] = 15;
   fontSTPtr->pixAryUC[39][5] = 6;
   fontSTPtr->pixAryUC[39][6] = 131;
   fontSTPtr->pixAryUC[39][7] = 62;

   return 0;
} /*addAscii71_ibmSan9*/

/*___________________________________________
` Fun40: addAscii72_ibmSan9
`  - adds character `H` (ascii 72) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii72_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[40])
      free(fontSTPtr->pixAryUC[40]);

   fontSTPtr->pixAryUC[40] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[40])
      return 1;

   fontSTPtr->widthArySS[40] = 7;
   fontSTPtr->lenArySS[40] = 8;

   fontSTPtr->pixAryUC[40][0] = 193;
   fontSTPtr->pixAryUC[40][1] = 96;
   fontSTPtr->pixAryUC[40][2] = 48;
   fontSTPtr->pixAryUC[40][3] = 248;
   fontSTPtr->pixAryUC[40][4] = 15;
   fontSTPtr->pixAryUC[40][5] = 6;
   fontSTPtr->pixAryUC[40][6] = 131;
   fontSTPtr->pixAryUC[40][7] = 65;

   return 0;
} /*addAscii72_ibmSan9*/

/*___________________________________________
` Fun41: addAscii73_ibmSan9
`  - adds character `I` (ascii 73) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii73_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[41])
      free(fontSTPtr->pixAryUC[41]);

   fontSTPtr->pixAryUC[41] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[41])
      return 1;

   fontSTPtr->widthArySS[41] = 5;
   fontSTPtr->lenArySS[41] = 6;

   fontSTPtr->pixAryUC[41][0] = 159;
   fontSTPtr->pixAryUC[41][1] = 16;
   fontSTPtr->pixAryUC[41][2] = 66;
   fontSTPtr->pixAryUC[41][3] = 8;
   fontSTPtr->pixAryUC[41][4] = 33;
   fontSTPtr->pixAryUC[41][5] = 31;

   return 0;
} /*addAscii73_ibmSan9*/

/*___________________________________________
` Fun42: addAscii74_ibmSan9
`  - adds character `J` (ascii 74) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii74_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[42])
      free(fontSTPtr->pixAryUC[42]);

   fontSTPtr->pixAryUC[42] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[42])
      return 1;

   fontSTPtr->widthArySS[42] = 6;
   fontSTPtr->lenArySS[42] = 7;

   fontSTPtr->pixAryUC[42][0] = 62;
   fontSTPtr->pixAryUC[42][1] = 4;
   fontSTPtr->pixAryUC[42][2] = 65;
   fontSTPtr->pixAryUC[42][3] = 16;
   fontSTPtr->pixAryUC[42][4] = 4;
   fontSTPtr->pixAryUC[42][5] = 69;
   fontSTPtr->pixAryUC[42][6] = 14;

   return 0;
} /*addAscii74_ibmSan9*/

/*___________________________________________
` Fun43: addAscii75_ibmSan9
`  - adds character `K` (ascii 75) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii75_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[43])
      free(fontSTPtr->pixAryUC[43]);

   fontSTPtr->pixAryUC[43] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[43])
      return 1;

   fontSTPtr->widthArySS[43] = 7;
   fontSTPtr->lenArySS[43] = 8;

   fontSTPtr->pixAryUC[43][0] = 161;
   fontSTPtr->pixAryUC[43][1] = 72;
   fontSTPtr->pixAryUC[43][2] = 162;
   fontSTPtr->pixAryUC[43][3] = 48;
   fontSTPtr->pixAryUC[43][4] = 40;
   fontSTPtr->pixAryUC[43][5] = 36;
   fontSTPtr->pixAryUC[43][6] = 34;
   fontSTPtr->pixAryUC[43][7] = 33;

   return 0;
} /*addAscii75_ibmSan9*/

/*___________________________________________
` Fun44: addAscii76_ibmSan9
`  - adds character `L` (ascii 76) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii76_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[44])
      free(fontSTPtr->pixAryUC[44]);

   fontSTPtr->pixAryUC[44] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[44])
      return 1;

   fontSTPtr->widthArySS[44] = 7;
   fontSTPtr->lenArySS[44] = 8;

   fontSTPtr->pixAryUC[44][0] = 129;
   fontSTPtr->pixAryUC[44][1] = 64;
   fontSTPtr->pixAryUC[44][2] = 32;
   fontSTPtr->pixAryUC[44][3] = 16;
   fontSTPtr->pixAryUC[44][4] = 8;
   fontSTPtr->pixAryUC[44][5] = 4;
   fontSTPtr->pixAryUC[44][6] = 2;
   fontSTPtr->pixAryUC[44][7] = 127;

   return 0;
} /*addAscii76_ibmSan9*/

/*___________________________________________
` Fun45: addAscii77_ibmSan9
`  - adds character `M` (ascii 77) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii77_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[45])
      free(fontSTPtr->pixAryUC[45]);

   fontSTPtr->pixAryUC[45] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[45])
      return 1;

   fontSTPtr->widthArySS[45] = 9;
   fontSTPtr->lenArySS[45] = 11;

   fontSTPtr->pixAryUC[45][0] = 1;
   fontSTPtr->pixAryUC[45][1] = 7;
   fontSTPtr->pixAryUC[45][2] = 23;
   fontSTPtr->pixAryUC[45][3] = 77;
   fontSTPtr->pixAryUC[45][4] = 25;
   fontSTPtr->pixAryUC[45][5] = 49;
   fontSTPtr->pixAryUC[45][6] = 96;
   fontSTPtr->pixAryUC[45][7] = 192;
   fontSTPtr->pixAryUC[45][8] = 128;
   fontSTPtr->pixAryUC[45][9] = 1;
   fontSTPtr->pixAryUC[45][10] = 1;

   return 0;
} /*addAscii77_ibmSan9*/

/*___________________________________________
` Fun46: addAscii78_ibmSan9
`  - adds character `N` (ascii 78) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii78_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[46])
      free(fontSTPtr->pixAryUC[46]);

   fontSTPtr->pixAryUC[46] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[46])
      return 1;

   fontSTPtr->widthArySS[46] = 8;
   fontSTPtr->lenArySS[46] = 9;

   fontSTPtr->pixAryUC[46][0] = 129;
   fontSTPtr->pixAryUC[46][1] = 131;
   fontSTPtr->pixAryUC[46][2] = 133;
   fontSTPtr->pixAryUC[46][3] = 137;
   fontSTPtr->pixAryUC[46][4] = 145;
   fontSTPtr->pixAryUC[46][5] = 161;
   fontSTPtr->pixAryUC[46][6] = 193;
   fontSTPtr->pixAryUC[46][7] = 129;
   fontSTPtr->pixAryUC[46][8] = 129;

   return 0;
} /*addAscii78_ibmSan9*/

/*___________________________________________
` Fun47: addAscii79_ibmSan9
`  - adds character `O` (ascii 79) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii79_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[47])
      free(fontSTPtr->pixAryUC[47]);

   fontSTPtr->pixAryUC[47] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[47])
      return 1;

   fontSTPtr->widthArySS[47] = 7;
   fontSTPtr->lenArySS[47] = 8;

   fontSTPtr->pixAryUC[47][0] = 190;
   fontSTPtr->pixAryUC[47][1] = 96;
   fontSTPtr->pixAryUC[47][2] = 48;
   fontSTPtr->pixAryUC[47][3] = 24;
   fontSTPtr->pixAryUC[47][4] = 12;
   fontSTPtr->pixAryUC[47][5] = 6;
   fontSTPtr->pixAryUC[47][6] = 131;
   fontSTPtr->pixAryUC[47][7] = 62;

   return 0;
} /*addAscii79_ibmSan9*/

/*___________________________________________
` Fun48: addAscii80_ibmSan9
`  - adds character `P` (ascii 80) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii80_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[48])
      free(fontSTPtr->pixAryUC[48]);

   fontSTPtr->pixAryUC[48] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[48])
      return 1;

   fontSTPtr->widthArySS[48] = 7;
   fontSTPtr->lenArySS[48] = 8;

   fontSTPtr->pixAryUC[48][0] = 191;
   fontSTPtr->pixAryUC[48][1] = 96;
   fontSTPtr->pixAryUC[48][2] = 48;
   fontSTPtr->pixAryUC[48][3] = 248;
   fontSTPtr->pixAryUC[48][4] = 11;
   fontSTPtr->pixAryUC[48][5] = 4;
   fontSTPtr->pixAryUC[48][6] = 2;
   fontSTPtr->pixAryUC[48][7] = 1;

   return 0;
} /*addAscii80_ibmSan9*/

/*___________________________________________
` Fun49: addAscii81_ibmSan9
`  - adds character `Q` (ascii 81) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii81_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[49])
      free(fontSTPtr->pixAryUC[49]);

   fontSTPtr->pixAryUC[49] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[49])
      return 1;

   fontSTPtr->widthArySS[49] = 8;
   fontSTPtr->lenArySS[49] = 9;

   fontSTPtr->pixAryUC[49][0] = 62;
   fontSTPtr->pixAryUC[49][1] = 65;
   fontSTPtr->pixAryUC[49][2] = 65;
   fontSTPtr->pixAryUC[49][3] = 65;
   fontSTPtr->pixAryUC[49][4] = 65;
   fontSTPtr->pixAryUC[49][5] = 89;
   fontSTPtr->pixAryUC[49][6] = 97;
   fontSTPtr->pixAryUC[49][7] = 65;
   fontSTPtr->pixAryUC[49][8] = 222;

   return 0;
} /*addAscii81_ibmSan9*/

/*___________________________________________
` Fun50: addAscii82_ibmSan9
`  - adds character `R` (ascii 82) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii82_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[50])
      free(fontSTPtr->pixAryUC[50]);

   fontSTPtr->pixAryUC[50] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[50])
      return 1;

   fontSTPtr->widthArySS[50] = 7;
   fontSTPtr->lenArySS[50] = 8;

   fontSTPtr->pixAryUC[50][0] = 191;
   fontSTPtr->pixAryUC[50][1] = 96;
   fontSTPtr->pixAryUC[50][2] = 48;
   fontSTPtr->pixAryUC[50][3] = 248;
   fontSTPtr->pixAryUC[50][4] = 139;
   fontSTPtr->pixAryUC[50][5] = 132;
   fontSTPtr->pixAryUC[50][6] = 130;
   fontSTPtr->pixAryUC[50][7] = 65;

   return 0;
} /*addAscii82_ibmSan9*/

/*___________________________________________
` Fun51: addAscii83_ibmSan9
`  - adds character `S` (ascii 83) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii83_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[51])
      free(fontSTPtr->pixAryUC[51]);

   fontSTPtr->pixAryUC[51] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[51])
      return 1;

   fontSTPtr->widthArySS[51] = 7;
   fontSTPtr->lenArySS[51] = 8;

   fontSTPtr->pixAryUC[51][0] = 190;
   fontSTPtr->pixAryUC[51][1] = 96;
   fontSTPtr->pixAryUC[51][2] = 32;
   fontSTPtr->pixAryUC[51][3] = 224;
   fontSTPtr->pixAryUC[51][4] = 3;
   fontSTPtr->pixAryUC[51][5] = 2;
   fontSTPtr->pixAryUC[51][6] = 131;
   fontSTPtr->pixAryUC[51][7] = 62;

   return 0;
} /*addAscii83_ibmSan9*/

/*___________________________________________
` Fun52: addAscii84_ibmSan9
`  - adds character `T` (ascii 84) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii84_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[52])
      free(fontSTPtr->pixAryUC[52]);

   fontSTPtr->pixAryUC[52] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[52])
      return 1;

   fontSTPtr->widthArySS[52] = 7;
   fontSTPtr->lenArySS[52] = 8;

   fontSTPtr->pixAryUC[52][0] = 127;
   fontSTPtr->pixAryUC[52][1] = 4;
   fontSTPtr->pixAryUC[52][2] = 2;
   fontSTPtr->pixAryUC[52][3] = 129;
   fontSTPtr->pixAryUC[52][4] = 64;
   fontSTPtr->pixAryUC[52][5] = 32;
   fontSTPtr->pixAryUC[52][6] = 16;
   fontSTPtr->pixAryUC[52][7] = 8;

   return 0;
} /*addAscii84_ibmSan9*/

/*___________________________________________
` Fun53: addAscii85_ibmSan9
`  - adds character `U` (ascii 85) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii85_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[53])
      free(fontSTPtr->pixAryUC[53]);

   fontSTPtr->pixAryUC[53] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[53])
      return 1;

   fontSTPtr->widthArySS[53] = 7;
   fontSTPtr->lenArySS[53] = 8;

   fontSTPtr->pixAryUC[53][0] = 193;
   fontSTPtr->pixAryUC[53][1] = 96;
   fontSTPtr->pixAryUC[53][2] = 48;
   fontSTPtr->pixAryUC[53][3] = 24;
   fontSTPtr->pixAryUC[53][4] = 12;
   fontSTPtr->pixAryUC[53][5] = 6;
   fontSTPtr->pixAryUC[53][6] = 131;
   fontSTPtr->pixAryUC[53][7] = 62;

   return 0;
} /*addAscii85_ibmSan9*/

/*___________________________________________
` Fun54: addAscii86_ibmSan9
`  - adds character `V` (ascii 86) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii86_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[54])
      free(fontSTPtr->pixAryUC[54]);

   fontSTPtr->pixAryUC[54] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[54])
      return 1;

   fontSTPtr->widthArySS[54] = 7;
   fontSTPtr->lenArySS[54] = 8;

   fontSTPtr->pixAryUC[54][0] = 193;
   fontSTPtr->pixAryUC[54][1] = 96;
   fontSTPtr->pixAryUC[54][2] = 80;
   fontSTPtr->pixAryUC[54][3] = 36;
   fontSTPtr->pixAryUC[54][4] = 162;
   fontSTPtr->pixAryUC[54][5] = 80;
   fontSTPtr->pixAryUC[54][6] = 16;
   fontSTPtr->pixAryUC[54][7] = 8;

   return 0;
} /*addAscii86_ibmSan9*/

/*___________________________________________
` Fun55: addAscii87_ibmSan9
`  - adds character `W` (ascii 87) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii87_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[55])
      free(fontSTPtr->pixAryUC[55]);

   fontSTPtr->pixAryUC[55] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[55])
      return 1;

   fontSTPtr->widthArySS[55] = 9;
   fontSTPtr->lenArySS[55] = 11;

   fontSTPtr->pixAryUC[55][0] = 1;
   fontSTPtr->pixAryUC[55][1] = 3;
   fontSTPtr->pixAryUC[55][2] = 6;
   fontSTPtr->pixAryUC[55][3] = 140;
   fontSTPtr->pixAryUC[55][4] = 24;
   fontSTPtr->pixAryUC[55][5] = 49;
   fontSTPtr->pixAryUC[55][6] = 101;
   fontSTPtr->pixAryUC[55][7] = 209;
   fontSTPtr->pixAryUC[55][8] = 193;
   fontSTPtr->pixAryUC[55][9] = 1;
   fontSTPtr->pixAryUC[55][10] = 1;

   return 0;
} /*addAscii87_ibmSan9*/

/*___________________________________________
` Fun56: addAscii88_ibmSan9
`  - adds character `X` (ascii 88) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii88_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[56])
      free(fontSTPtr->pixAryUC[56]);

   fontSTPtr->pixAryUC[56] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[56])
      return 1;

   fontSTPtr->widthArySS[56] = 7;
   fontSTPtr->lenArySS[56] = 8;

   fontSTPtr->pixAryUC[56][0] = 65;
   fontSTPtr->pixAryUC[56][1] = 17;
   fontSTPtr->pixAryUC[56][2] = 5;
   fontSTPtr->pixAryUC[56][3] = 129;
   fontSTPtr->pixAryUC[56][4] = 160;
   fontSTPtr->pixAryUC[56][5] = 136;
   fontSTPtr->pixAryUC[56][6] = 130;
   fontSTPtr->pixAryUC[56][7] = 65;

   return 0;
} /*addAscii88_ibmSan9*/

/*___________________________________________
` Fun57: addAscii89_ibmSan9
`  - adds character `Y` (ascii 89) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii89_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[57])
      free(fontSTPtr->pixAryUC[57]);

   fontSTPtr->pixAryUC[57] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[57])
      return 1;

   fontSTPtr->widthArySS[57] = 7;
   fontSTPtr->lenArySS[57] = 8;

   fontSTPtr->pixAryUC[57][0] = 65;
   fontSTPtr->pixAryUC[57][1] = 17;
   fontSTPtr->pixAryUC[57][2] = 5;
   fontSTPtr->pixAryUC[57][3] = 129;
   fontSTPtr->pixAryUC[57][4] = 64;
   fontSTPtr->pixAryUC[57][5] = 32;
   fontSTPtr->pixAryUC[57][6] = 16;
   fontSTPtr->pixAryUC[57][7] = 8;

   return 0;
} /*addAscii89_ibmSan9*/

/*___________________________________________
` Fun58: addAscii90_ibmSan9
`  - adds character `Z` (ascii 90) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii90_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[58])
      free(fontSTPtr->pixAryUC[58]);

   fontSTPtr->pixAryUC[58] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[58])
      return 1;

   fontSTPtr->widthArySS[58] = 7;
   fontSTPtr->lenArySS[58] = 8;

   fontSTPtr->pixAryUC[58][0] = 127;
   fontSTPtr->pixAryUC[58][1] = 16;
   fontSTPtr->pixAryUC[58][2] = 228;
   fontSTPtr->pixAryUC[58][3] = 79;
   fontSTPtr->pixAryUC[58][4] = 16;
   fontSTPtr->pixAryUC[58][5] = 4;
   fontSTPtr->pixAryUC[58][6] = 2;
   fontSTPtr->pixAryUC[58][7] = 127;

   return 0;
} /*addAscii90_ibmSan9*/

/*___________________________________________
` Fun59: addAscii91_ibmSan9
`  - adds character `[` (ascii 91) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii91_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[59])
      free(fontSTPtr->pixAryUC[59]);

   fontSTPtr->pixAryUC[59] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[59])
      return 1;

   fontSTPtr->widthArySS[59] = 4;
   fontSTPtr->lenArySS[59] = 5;

   fontSTPtr->pixAryUC[59][0] = 31;
   fontSTPtr->pixAryUC[59][1] = 17;
   fontSTPtr->pixAryUC[59][2] = 17;
   fontSTPtr->pixAryUC[59][3] = 17;
   fontSTPtr->pixAryUC[59][4] = 15;

   return 0;
} /*addAscii91_ibmSan9*/

/*___________________________________________
` Fun60: addAscii92_ibmSan9
`  - adds character `\` (ascii 92) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii92_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[60])
      free(fontSTPtr->pixAryUC[60]);

   fontSTPtr->pixAryUC[60] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[60])
      return 1;

   fontSTPtr->widthArySS[60] = 6;
   fontSTPtr->lenArySS[60] = 7;

   fontSTPtr->pixAryUC[60][0] = 129;
   fontSTPtr->pixAryUC[60][1] = 32;
   fontSTPtr->pixAryUC[60][2] = 16;
   fontSTPtr->pixAryUC[60][3] = 4;
   fontSTPtr->pixAryUC[60][4] = 130;
   fontSTPtr->pixAryUC[60][5] = 64;
   fontSTPtr->pixAryUC[60][6] = 16;

   return 0;
} /*addAscii92_ibmSan9*/

/*___________________________________________
` Fun61: addAscii93_ibmSan9
`  - adds character `]` (ascii 93) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii93_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[61])
      free(fontSTPtr->pixAryUC[61]);

   fontSTPtr->pixAryUC[61] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[61])
      return 1;

   fontSTPtr->widthArySS[61] = 4;
   fontSTPtr->lenArySS[61] = 5;

   fontSTPtr->pixAryUC[61][0] = 143;
   fontSTPtr->pixAryUC[61][1] = 136;
   fontSTPtr->pixAryUC[61][2] = 136;
   fontSTPtr->pixAryUC[61][3] = 136;
   fontSTPtr->pixAryUC[61][4] = 15;

   return 0;
} /*addAscii93_ibmSan9*/

/*___________________________________________
` Fun62: addAscii94_ibmSan9
`  - adds character `^` (ascii 94) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii94_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[62])
      free(fontSTPtr->pixAryUC[62]);

   fontSTPtr->pixAryUC[62] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[62])
      return 1;

   fontSTPtr->widthArySS[62] = 7;
   fontSTPtr->lenArySS[62] = 8;

   fontSTPtr->pixAryUC[62][0] = 8;
   fontSTPtr->pixAryUC[62][1] = 138;
   fontSTPtr->pixAryUC[62][2] = 8;
   fontSTPtr->pixAryUC[62][3] = 0;
   fontSTPtr->pixAryUC[62][4] = 0;
   fontSTPtr->pixAryUC[62][5] = 0;
   fontSTPtr->pixAryUC[62][6] = 0;
   fontSTPtr->pixAryUC[62][7] = 0;

   return 0;
} /*addAscii94_ibmSan9*/

/*___________________________________________
` Fun63: addAscii95_ibmSan9
`  - adds character `_` (ascii 95) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii95_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[63])
      free(fontSTPtr->pixAryUC[63]);

   fontSTPtr->pixAryUC[63] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[63])
      return 1;

   fontSTPtr->widthArySS[63] = 7;
   fontSTPtr->lenArySS[63] = 8;

   fontSTPtr->pixAryUC[63][0] = 0;
   fontSTPtr->pixAryUC[63][1] = 0;
   fontSTPtr->pixAryUC[63][2] = 0;
   fontSTPtr->pixAryUC[63][3] = 0;
   fontSTPtr->pixAryUC[63][4] = 0;
   fontSTPtr->pixAryUC[63][5] = 0;
   fontSTPtr->pixAryUC[63][6] = 0;
   fontSTPtr->pixAryUC[63][7] = 127;

   return 0;
} /*addAscii95_ibmSan9*/

/*___________________________________________
` Fun64: addAscii96_ibmSan9
`  - adds character ``` (ascii 96) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii96_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[64])
      free(fontSTPtr->pixAryUC[64]);

   fontSTPtr->pixAryUC[64] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[64])
      return 1;

   fontSTPtr->widthArySS[64] = 4;
   fontSTPtr->lenArySS[64] = 5;

   fontSTPtr->pixAryUC[64][0] = 33;
   fontSTPtr->pixAryUC[64][1] = 4;
   fontSTPtr->pixAryUC[64][2] = 0;
   fontSTPtr->pixAryUC[64][3] = 0;
   fontSTPtr->pixAryUC[64][4] = 0;

   return 0;
} /*addAscii96_ibmSan9*/

/*___________________________________________
` Fun65: addAscii97_ibmSan9
`  - adds character `a` (ascii 97) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii97_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[65])
      free(fontSTPtr->pixAryUC[65]);

   fontSTPtr->pixAryUC[65] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[65])
      return 1;

   fontSTPtr->widthArySS[65] = 8;
   fontSTPtr->lenArySS[65] = 9;

   fontSTPtr->pixAryUC[65][0] = 0;
   fontSTPtr->pixAryUC[65][1] = 0;
   fontSTPtr->pixAryUC[65][2] = 28;
   fontSTPtr->pixAryUC[65][3] = 32;
   fontSTPtr->pixAryUC[65][4] = 32;
   fontSTPtr->pixAryUC[65][5] = 62;
   fontSTPtr->pixAryUC[65][6] = 33;
   fontSTPtr->pixAryUC[65][7] = 161;
   fontSTPtr->pixAryUC[65][8] = 126;

   return 0;
} /*addAscii97_ibmSan9*/

/*___________________________________________
` Fun66: addAscii98_ibmSan9
`  - adds character `b` (ascii 98) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii98_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[66])
      free(fontSTPtr->pixAryUC[66]);

   fontSTPtr->pixAryUC[66] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[66])
      return 1;

   fontSTPtr->widthArySS[66] = 7;
   fontSTPtr->lenArySS[66] = 8;

   fontSTPtr->pixAryUC[66][0] = 129;
   fontSTPtr->pixAryUC[66][1] = 64;
   fontSTPtr->pixAryUC[66][2] = 32;
   fontSTPtr->pixAryUC[66][3] = 208;
   fontSTPtr->pixAryUC[66][4] = 27;
   fontSTPtr->pixAryUC[66][5] = 6;
   fontSTPtr->pixAryUC[66][6] = 135;
   fontSTPtr->pixAryUC[66][7] = 61;

   return 0;
} /*addAscii98_ibmSan9*/

/*___________________________________________
` Fun67: addAscii99_ibmSan9
`  - adds character `c` (ascii 99) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii99_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[67])
      free(fontSTPtr->pixAryUC[67]);

   fontSTPtr->pixAryUC[67] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[67])
      return 1;

   fontSTPtr->widthArySS[67] = 6;
   fontSTPtr->lenArySS[67] = 7;

   fontSTPtr->pixAryUC[67][0] = 0;
   fontSTPtr->pixAryUC[67][1] = 224;
   fontSTPtr->pixAryUC[67][2] = 133;
   fontSTPtr->pixAryUC[67][3] = 65;
   fontSTPtr->pixAryUC[67][4] = 16;
   fontSTPtr->pixAryUC[67][5] = 132;
   fontSTPtr->pixAryUC[67][6] = 30;

   return 0;
} /*addAscii99_ibmSan9*/

/*___________________________________________
` Fun68: addAscii100_ibmSan9
`  - adds character `d` (ascii 100) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii100_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[68])
      free(fontSTPtr->pixAryUC[68]);

   fontSTPtr->pixAryUC[68] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[68])
      return 1;

   fontSTPtr->widthArySS[68] = 8;
   fontSTPtr->lenArySS[68] = 9;

   fontSTPtr->pixAryUC[68][0] = 64;
   fontSTPtr->pixAryUC[68][1] = 64;
   fontSTPtr->pixAryUC[68][2] = 64;
   fontSTPtr->pixAryUC[68][3] = 64;
   fontSTPtr->pixAryUC[68][4] = 94;
   fontSTPtr->pixAryUC[68][5] = 97;
   fontSTPtr->pixAryUC[68][6] = 65;
   fontSTPtr->pixAryUC[68][7] = 97;
   fontSTPtr->pixAryUC[68][8] = 222;

   return 0;
} /*addAscii100_ibmSan9*/

/*___________________________________________
` Fun69: addAscii101_ibmSan9
`  - adds character `e` (ascii 101) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii101_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[69])
      free(fontSTPtr->pixAryUC[69]);

   fontSTPtr->pixAryUC[69] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[69])
      return 1;

   fontSTPtr->widthArySS[69] = 7;
   fontSTPtr->lenArySS[69] = 8;

   fontSTPtr->pixAryUC[69][0] = 0;
   fontSTPtr->pixAryUC[69][1] = 0;
   fontSTPtr->pixAryUC[69][2] = 71;
   fontSTPtr->pixAryUC[69][3] = 20;
   fontSTPtr->pixAryUC[69][4] = 252;
   fontSTPtr->pixAryUC[69][5] = 7;
   fontSTPtr->pixAryUC[69][6] = 132;
   fontSTPtr->pixAryUC[69][7] = 60;

   return 0;
} /*addAscii101_ibmSan9*/

/*___________________________________________
` Fun70: addAscii102_ibmSan9
`  - adds character `f` (ascii 102) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii102_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[70])
      free(fontSTPtr->pixAryUC[70]);

   fontSTPtr->pixAryUC[70] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[70])
      return 1;

   fontSTPtr->widthArySS[70] = 7;
   fontSTPtr->lenArySS[70] = 8;

   fontSTPtr->pixAryUC[70][0] = 56;
   fontSTPtr->pixAryUC[70][1] = 34;
   fontSTPtr->pixAryUC[70][2] = 225;
   fontSTPtr->pixAryUC[70][3] = 67;
   fontSTPtr->pixAryUC[70][4] = 32;
   fontSTPtr->pixAryUC[70][5] = 16;
   fontSTPtr->pixAryUC[70][6] = 8;
   fontSTPtr->pixAryUC[70][7] = 4;

   return 0;
} /*addAscii102_ibmSan9*/

/*___________________________________________
` Fun71: addAscii103_ibmSan9
`  - adds character `g` (ascii 103) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii103_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[71])
      free(fontSTPtr->pixAryUC[71]);

   fontSTPtr->pixAryUC[71] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[71])
      return 1;

   fontSTPtr->widthArySS[71] = 7;
   fontSTPtr->lenArySS[71] = 8;

   fontSTPtr->pixAryUC[71][0] = 0;
   fontSTPtr->pixAryUC[71][1] = 128;
   fontSTPtr->pixAryUC[71][2] = 63;
   fontSTPtr->pixAryUC[71][3] = 24;
   fontSTPtr->pixAryUC[71][4] = 244;
   fontSTPtr->pixAryUC[71][5] = 3;
   fontSTPtr->pixAryUC[71][6] = 131;
   fontSTPtr->pixAryUC[71][7] = 62;

   return 0;
} /*addAscii103_ibmSan9*/

/*___________________________________________
` Fun72: addAscii104_ibmSan9
`  - adds character `h` (ascii 104) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii104_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[72])
      free(fontSTPtr->pixAryUC[72]);

   fontSTPtr->pixAryUC[72] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[72])
      return 1;

   fontSTPtr->widthArySS[72] = 8;
   fontSTPtr->lenArySS[72] = 9;

   fontSTPtr->pixAryUC[72][0] = 1;
   fontSTPtr->pixAryUC[72][1] = 1;
   fontSTPtr->pixAryUC[72][2] = 1;
   fontSTPtr->pixAryUC[72][3] = 1;
   fontSTPtr->pixAryUC[72][4] = 61;
   fontSTPtr->pixAryUC[72][5] = 35;
   fontSTPtr->pixAryUC[72][6] = 33;
   fontSTPtr->pixAryUC[72][7] = 161;
   fontSTPtr->pixAryUC[72][8] = 97;

   return 0;
} /*addAscii104_ibmSan9*/

/*___________________________________________
` Fun73: addAscii105_ibmSan9
`  - adds character `i` (ascii 105) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii105_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[73])
      free(fontSTPtr->pixAryUC[73]);

   fontSTPtr->pixAryUC[73] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[73])
      return 1;

   fontSTPtr->widthArySS[73] = 3;
   fontSTPtr->lenArySS[73] = 4;

   fontSTPtr->pixAryUC[73][0] = 130;
   fontSTPtr->pixAryUC[73][1] = 36;
   fontSTPtr->pixAryUC[73][2] = 73;
   fontSTPtr->pixAryUC[73][3] = 2;

   return 0;
} /*addAscii105_ibmSan9*/

/*___________________________________________
` Fun74: addAscii106_ibmSan9
`  - adds character `j` (ascii 106) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii106_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[74])
      free(fontSTPtr->pixAryUC[74]);

   fontSTPtr->pixAryUC[74] =
     calloc(5, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[74])
      return 1;

   fontSTPtr->widthArySS[74] = 4;
   fontSTPtr->lenArySS[74] = 5;

   fontSTPtr->pixAryUC[74][0] = 4;
   fontSTPtr->pixAryUC[74][1] = 68;
   fontSTPtr->pixAryUC[74][2] = 68;
   fontSTPtr->pixAryUC[74][3] = 84;
   fontSTPtr->pixAryUC[74][4] = 6;

   return 0;
} /*addAscii106_ibmSan9*/

/*___________________________________________
` Fun75: addAscii107_ibmSan9
`  - adds character `k` (ascii 107) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii107_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[75])
      free(fontSTPtr->pixAryUC[75]);

   fontSTPtr->pixAryUC[75] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[75])
      return 1;

   fontSTPtr->widthArySS[75] = 6;
   fontSTPtr->lenArySS[75] = 7;

   fontSTPtr->pixAryUC[75][0] = 65;
   fontSTPtr->pixAryUC[75][1] = 16;
   fontSTPtr->pixAryUC[75][2] = 37;
   fontSTPtr->pixAryUC[75][3] = 71;
   fontSTPtr->pixAryUC[75][4] = 145;
   fontSTPtr->pixAryUC[75][5] = 68;
   fontSTPtr->pixAryUC[75][6] = 33;

   return 0;
} /*addAscii107_ibmSan9*/

/*___________________________________________
` Fun76: addAscii108_ibmSan9
`  - adds character `l` (ascii 108) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii108_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[76])
      free(fontSTPtr->pixAryUC[76]);

   fontSTPtr->pixAryUC[76] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[76])
      return 1;

   fontSTPtr->widthArySS[76] = 5;
   fontSTPtr->lenArySS[76] = 6;

   fontSTPtr->pixAryUC[76][0] = 66;
   fontSTPtr->pixAryUC[76][1] = 8;
   fontSTPtr->pixAryUC[76][2] = 33;
   fontSTPtr->pixAryUC[76][3] = 132;
   fontSTPtr->pixAryUC[76][4] = 144;
   fontSTPtr->pixAryUC[76][5] = 12;

   return 0;
} /*addAscii108_ibmSan9*/

/*___________________________________________
` Fun77: addAscii109_ibmSan9
`  - adds character `m` (ascii 109) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii109_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[77])
      free(fontSTPtr->pixAryUC[77]);

   fontSTPtr->pixAryUC[77] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[77])
      return 1;

   fontSTPtr->widthArySS[77] = 9;
   fontSTPtr->lenArySS[77] = 11;

   fontSTPtr->pixAryUC[77][0] = 0;
   fontSTPtr->pixAryUC[77][1] = 0;
   fontSTPtr->pixAryUC[77][2] = 236;
   fontSTPtr->pixAryUC[77][3] = 78;
   fontSTPtr->pixAryUC[77][4] = 153;
   fontSTPtr->pixAryUC[77][5] = 50;
   fontSTPtr->pixAryUC[77][6] = 101;
   fontSTPtr->pixAryUC[77][7] = 202;
   fontSTPtr->pixAryUC[77][8] = 148;
   fontSTPtr->pixAryUC[77][9] = 41;
   fontSTPtr->pixAryUC[77][10] = 1;

   return 0;
} /*addAscii109_ibmSan9*/

/*___________________________________________
` Fun78: addAscii110_ibmSan9
`  - adds character `n` (ascii 110) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii110_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[78])
      free(fontSTPtr->pixAryUC[78]);

   fontSTPtr->pixAryUC[78] =
     calloc(9, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[78])
      return 1;

   fontSTPtr->widthArySS[78] = 8;
   fontSTPtr->lenArySS[78] = 9;

   fontSTPtr->pixAryUC[78][0] = 0;
   fontSTPtr->pixAryUC[78][1] = 0;
   fontSTPtr->pixAryUC[78][2] = 61;
   fontSTPtr->pixAryUC[78][3] = 35;
   fontSTPtr->pixAryUC[78][4] = 33;
   fontSTPtr->pixAryUC[78][5] = 33;
   fontSTPtr->pixAryUC[78][6] = 33;
   fontSTPtr->pixAryUC[78][7] = 161;
   fontSTPtr->pixAryUC[78][8] = 97;

   return 0;
} /*addAscii110_ibmSan9*/

/*___________________________________________
` Fun79: addAscii111_ibmSan9
`  - adds character `o` (ascii 111) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii111_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[79])
      free(fontSTPtr->pixAryUC[79]);

   fontSTPtr->pixAryUC[79] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[79])
      return 1;

   fontSTPtr->widthArySS[79] = 7;
   fontSTPtr->lenArySS[79] = 8;

   fontSTPtr->pixAryUC[79][0] = 0;
   fontSTPtr->pixAryUC[79][1] = 128;
   fontSTPtr->pixAryUC[79][2] = 47;
   fontSTPtr->pixAryUC[79][3] = 24;
   fontSTPtr->pixAryUC[79][4] = 12;
   fontSTPtr->pixAryUC[79][5] = 6;
   fontSTPtr->pixAryUC[79][6] = 131;
   fontSTPtr->pixAryUC[79][7] = 62;

   return 0;
} /*addAscii111_ibmSan9*/

/*___________________________________________
` Fun80: addAscii112_ibmSan9
`  - adds character `p` (ascii 112) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii112_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[80])
      free(fontSTPtr->pixAryUC[80]);

   fontSTPtr->pixAryUC[80] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[80])
      return 1;

   fontSTPtr->widthArySS[80] = 7;
   fontSTPtr->lenArySS[80] = 8;

   fontSTPtr->pixAryUC[80][0] = 0;
   fontSTPtr->pixAryUC[80][1] = 192;
   fontSTPtr->pixAryUC[80][2] = 47;
   fontSTPtr->pixAryUC[80][3] = 20;
   fontSTPtr->pixAryUC[80][4] = 250;
   fontSTPtr->pixAryUC[80][5] = 5;
   fontSTPtr->pixAryUC[80][6] = 2;
   fontSTPtr->pixAryUC[80][7] = 1;

   return 0;
} /*addAscii112_ibmSan9*/

/*___________________________________________
` Fun81: addAscii113_ibmSan9
`  - adds character `q` (ascii 113) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii113_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[81])
      free(fontSTPtr->pixAryUC[81]);

   fontSTPtr->pixAryUC[81] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[81])
      return 1;

   fontSTPtr->widthArySS[81] = 7;
   fontSTPtr->lenArySS[81] = 8;

   fontSTPtr->pixAryUC[81][0] = 0;
   fontSTPtr->pixAryUC[81][1] = 128;
   fontSTPtr->pixAryUC[81][2] = 63;
   fontSTPtr->pixAryUC[81][3] = 20;
   fontSTPtr->pixAryUC[81][4] = 242;
   fontSTPtr->pixAryUC[81][5] = 131;
   fontSTPtr->pixAryUC[81][6] = 64;
   fontSTPtr->pixAryUC[81][7] = 32;

   return 0;
} /*addAscii113_ibmSan9*/

/*___________________________________________
` Fun82: addAscii114_ibmSan9
`  - adds character `r` (ascii 114) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii114_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[82])
      free(fontSTPtr->pixAryUC[82]);

   fontSTPtr->pixAryUC[82] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[82])
      return 1;

   fontSTPtr->widthArySS[82] = 5;
   fontSTPtr->lenArySS[82] = 6;

   fontSTPtr->pixAryUC[82][0] = 0;
   fontSTPtr->pixAryUC[82][1] = 180;
   fontSTPtr->pixAryUC[82][2] = 25;
   fontSTPtr->pixAryUC[82][3] = 66;
   fontSTPtr->pixAryUC[82][4] = 8;
   fontSTPtr->pixAryUC[82][5] = 1;

   return 0;
} /*addAscii114_ibmSan9*/

/*___________________________________________
` Fun83: addAscii115_ibmSan9
`  - adds character `s` (ascii 115) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii115_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[83])
      free(fontSTPtr->pixAryUC[83]);

   fontSTPtr->pixAryUC[83] =
     calloc(7, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[83])
      return 1;

   fontSTPtr->widthArySS[83] = 6;
   fontSTPtr->lenArySS[83] = 7;

   fontSTPtr->pixAryUC[83][0] = 0;
   fontSTPtr->pixAryUC[83][1] = 224;
   fontSTPtr->pixAryUC[83][2] = 133;
   fontSTPtr->pixAryUC[83][3] = 129;
   fontSTPtr->pixAryUC[83][4] = 3;
   fontSTPtr->pixAryUC[83][5] = 134;
   fontSTPtr->pixAryUC[83][6] = 30;

   return 0;
} /*addAscii115_ibmSan9*/

/*___________________________________________
` Fun84: addAscii116_ibmSan9
`  - adds character `t` (ascii 116) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii116_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[84])
      free(fontSTPtr->pixAryUC[84]);

   fontSTPtr->pixAryUC[84] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[84])
      return 1;

   fontSTPtr->widthArySS[84] = 7;
   fontSTPtr->lenArySS[84] = 8;

   fontSTPtr->pixAryUC[84][0] = 8;
   fontSTPtr->pixAryUC[84][1] = 4;
   fontSTPtr->pixAryUC[84][2] = 226;
   fontSTPtr->pixAryUC[84][3] = 143;
   fontSTPtr->pixAryUC[84][4] = 64;
   fontSTPtr->pixAryUC[84][5] = 32;
   fontSTPtr->pixAryUC[84][6] = 144;
   fontSTPtr->pixAryUC[84][7] = 48;

   return 0;
} /*addAscii116_ibmSan9*/

/*___________________________________________
` Fun85: addAscii117_ibmSan9
`  - adds character `u` (ascii 117) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii117_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[85])
      free(fontSTPtr->pixAryUC[85]);

   fontSTPtr->pixAryUC[85] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[85])
      return 1;

   fontSTPtr->widthArySS[85] = 7;
   fontSTPtr->lenArySS[85] = 8;

   fontSTPtr->pixAryUC[85][0] = 0;
   fontSTPtr->pixAryUC[85][1] = 64;
   fontSTPtr->pixAryUC[85][2] = 40;
   fontSTPtr->pixAryUC[85][3] = 20;
   fontSTPtr->pixAryUC[85][4] = 10;
   fontSTPtr->pixAryUC[85][5] = 133;
   fontSTPtr->pixAryUC[85][6] = 98;
   fontSTPtr->pixAryUC[85][7] = 94;

   return 0;
} /*addAscii117_ibmSan9*/

/*___________________________________________
` Fun86: addAscii118_ibmSan9
`  - adds character `v` (ascii 118) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii118_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[86])
      free(fontSTPtr->pixAryUC[86]);

   fontSTPtr->pixAryUC[86] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[86])
      return 1;

   fontSTPtr->widthArySS[86] = 7;
   fontSTPtr->lenArySS[86] = 8;

   fontSTPtr->pixAryUC[86][0] = 0;
   fontSTPtr->pixAryUC[86][1] = 64;
   fontSTPtr->pixAryUC[86][2] = 48;
   fontSTPtr->pixAryUC[86][3] = 40;
   fontSTPtr->pixAryUC[86][4] = 18;
   fontSTPtr->pixAryUC[86][5] = 81;
   fontSTPtr->pixAryUC[86][6] = 40;
   fontSTPtr->pixAryUC[86][7] = 8;

   return 0;
} /*addAscii118_ibmSan9*/

/*___________________________________________
` Fun87: addAscii119_ibmSan9
`  - adds character `w` (ascii 119) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii119_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[87])
      free(fontSTPtr->pixAryUC[87]);

   fontSTPtr->pixAryUC[87] =
     calloc(11, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[87])
      return 1;

   fontSTPtr->widthArySS[87] = 9;
   fontSTPtr->lenArySS[87] = 11;

   fontSTPtr->pixAryUC[87][0] = 0;
   fontSTPtr->pixAryUC[87][1] = 0;
   fontSTPtr->pixAryUC[87][2] = 4;
   fontSTPtr->pixAryUC[87][3] = 12;
   fontSTPtr->pixAryUC[87][4] = 24;
   fontSTPtr->pixAryUC[87][5] = 49;
   fontSTPtr->pixAryUC[87][6] = 162;
   fontSTPtr->pixAryUC[87][7] = 42;
   fontSTPtr->pixAryUC[87][8] = 85;
   fontSTPtr->pixAryUC[87][9] = 68;
   fontSTPtr->pixAryUC[87][10] = 0;

   return 0;
} /*addAscii119_ibmSan9*/

/*___________________________________________
` Fun88: addAscii120_ibmSan9
`  - adds character `x` (ascii 120) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii120_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[88])
      free(fontSTPtr->pixAryUC[88]);

   fontSTPtr->pixAryUC[88] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[88])
      return 1;

   fontSTPtr->widthArySS[88] = 7;
   fontSTPtr->lenArySS[88] = 8;

   fontSTPtr->pixAryUC[88][0] = 0;
   fontSTPtr->pixAryUC[88][1] = 64;
   fontSTPtr->pixAryUC[88][2] = 80;
   fontSTPtr->pixAryUC[88][3] = 68;
   fontSTPtr->pixAryUC[88][4] = 65;
   fontSTPtr->pixAryUC[88][5] = 80;
   fontSTPtr->pixAryUC[88][6] = 68;
   fontSTPtr->pixAryUC[88][7] = 65;

   return 0;
} /*addAscii120_ibmSan9*/

/*___________________________________________
` Fun89: addAscii121_ibmSan9
`  - adds character `y` (ascii 121) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii121_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[89])
      free(fontSTPtr->pixAryUC[89]);

   fontSTPtr->pixAryUC[89] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[89])
      return 1;

   fontSTPtr->widthArySS[89] = 7;
   fontSTPtr->lenArySS[89] = 8;

   fontSTPtr->pixAryUC[89][0] = 0;
   fontSTPtr->pixAryUC[89][1] = 64;
   fontSTPtr->pixAryUC[89][2] = 80;
   fontSTPtr->pixAryUC[89][3] = 68;
   fontSTPtr->pixAryUC[89][4] = 65;
   fontSTPtr->pixAryUC[89][5] = 16;
   fontSTPtr->pixAryUC[89][6] = 4;
   fontSTPtr->pixAryUC[89][7] = 1;

   return 0;
} /*addAscii121_ibmSan9*/

/*___________________________________________
` Fun90: addAscii122_ibmSan9
`  - adds character `z` (ascii 122) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii122_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[90])
      free(fontSTPtr->pixAryUC[90]);

   fontSTPtr->pixAryUC[90] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[90])
      return 1;

   fontSTPtr->widthArySS[90] = 7;
   fontSTPtr->lenArySS[90] = 8;

   fontSTPtr->pixAryUC[90][0] = 0;
   fontSTPtr->pixAryUC[90][1] = 192;
   fontSTPtr->pixAryUC[90][2] = 31;
   fontSTPtr->pixAryUC[90][3] = 4;
   fontSTPtr->pixAryUC[90][4] = 249;
   fontSTPtr->pixAryUC[90][5] = 19;
   fontSTPtr->pixAryUC[90][6] = 4;
   fontSTPtr->pixAryUC[90][7] = 127;

   return 0;
} /*addAscii122_ibmSan9*/

/*___________________________________________
` Fun91: addAscii123_ibmSan9
`  - adds character `{` (ascii 123) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii123_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[91])
      free(fontSTPtr->pixAryUC[91]);

   fontSTPtr->pixAryUC[91] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[91])
      return 1;

   fontSTPtr->widthArySS[91] = 5;
   fontSTPtr->lenArySS[91] = 6;

   fontSTPtr->pixAryUC[91][0] = 76;
   fontSTPtr->pixAryUC[91][1] = 8;
   fontSTPtr->pixAryUC[91][2] = 49;
   fontSTPtr->pixAryUC[91][3] = 132;
   fontSTPtr->pixAryUC[91][4] = 16;
   fontSTPtr->pixAryUC[91][5] = 12;

   return 0;
} /*addAscii123_ibmSan9*/

/*___________________________________________
` Fun92: addAscii124_ibmSan9
`  - adds character `|` (ascii 124) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii124_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[92])
      free(fontSTPtr->pixAryUC[92]);

   fontSTPtr->pixAryUC[92] =
     calloc(4, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[92])
      return 1;

   fontSTPtr->widthArySS[92] = 3;
   fontSTPtr->lenArySS[92] = 4;

   fontSTPtr->pixAryUC[92][0] = 146;
   fontSTPtr->pixAryUC[92][1] = 36;
   fontSTPtr->pixAryUC[92][2] = 73;
   fontSTPtr->pixAryUC[92][3] = 2;

   return 0;
} /*addAscii124_ibmSan9*/

/*___________________________________________
` Fun93: addAscii125_ibmSan9
`  - adds character `}` (ascii 125) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii125_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[93])
      free(fontSTPtr->pixAryUC[93]);

   fontSTPtr->pixAryUC[93] =
     calloc(6, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[93])
      return 1;

   fontSTPtr->widthArySS[93] = 5;
   fontSTPtr->lenArySS[93] = 6;

   fontSTPtr->pixAryUC[93][0] = 131;
   fontSTPtr->pixAryUC[93][1] = 16;
   fontSTPtr->pixAryUC[93][2] = 194;
   fontSTPtr->pixAryUC[93][3] = 8;
   fontSTPtr->pixAryUC[93][4] = 33;
   fontSTPtr->pixAryUC[93][5] = 3;

   return 0;
} /*addAscii125_ibmSan9*/

/*___________________________________________
` Fun94: addAscii126_ibmSan9
`  - adds character `~` (ascii 126) to font
`  - returns 1 for success or 0 for failure
`__________________________________________*/
signed char
addAscii126_ibmSan9(
   struct font_fontST *fontSTPtr
){
   if(fontSTPtr->pixAryUC[94])
      free(fontSTPtr->pixAryUC[94]);

   fontSTPtr->pixAryUC[94] =
     calloc(8, sizeof(unsigned char));
   if(! fontSTPtr->pixAryUC[94])
      return 1;

   fontSTPtr->widthArySS[94] = 7;
   fontSTPtr->lenArySS[94] = 8;

   fontSTPtr->pixAryUC[94][0] = 0;
   fontSTPtr->pixAryUC[94][1] = 99;
   fontSTPtr->pixAryUC[94][2] = 42;
   fontSTPtr->pixAryUC[94][3] = 6;
   fontSTPtr->pixAryUC[94][4] = 0;
   fontSTPtr->pixAryUC[94][5] = 0;
   fontSTPtr->pixAryUC[94][6] = 0;
   fontSTPtr->pixAryUC[94][7] = 0;

   return 0;
} /*addAscii126_ibmSan9*/

/*_____________________________________________
` Fun95: loadFont_ibmSan9
`  - loads the font in ibmSan9.c/h to the input
`    font_fontST struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_ibmSan9(
   struct font_fontST *fontSTPtr
){
   fontSTPtr->heightSS = 9;
   fontSTPtr->gapSS = 1;

   if( addAscii32_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii33_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii34_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii35_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii36_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii37_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii38_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii39_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii40_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii41_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii42_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii43_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii44_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii45_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii46_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii47_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii48_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii49_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii50_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii51_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii52_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii53_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii54_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii55_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii56_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii57_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii58_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii59_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii60_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii61_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii62_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii63_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii64_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii65_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii66_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii67_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii68_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii69_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii70_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii71_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii72_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii73_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii74_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii75_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii76_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii77_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii78_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii79_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii80_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii81_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii82_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii83_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii84_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii85_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii86_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii87_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii88_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii89_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii90_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii91_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii92_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii93_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii94_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii95_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii96_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii97_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii98_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii99_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii100_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii101_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii102_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii103_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii104_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii105_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii106_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii107_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii108_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii109_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii110_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii111_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii112_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii113_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii114_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii115_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii116_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii117_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii118_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii119_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii120_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii121_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii122_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii123_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii124_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii125_ibmSan9(fontSTPtr) )
      return 1;
   if( addAscii126_ibmSan9(fontSTPtr) )
      return 1;

   return 0;
} /*loadFont_ibmSan9*/
