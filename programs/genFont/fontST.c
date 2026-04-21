/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' fontST SOF: Start Of File
'   - handels loading and drawing png fonts
'   - I am avoidng using longs here in the drawing step to
'     avoid additional complexity
'   o header:
'     - libraries
'   o .h st01: font_fontST
'     - holds the font to draw into the png
'   o fun01: blank_font_fontST
'     - blanks a font_fontST struct
'   o fun02: init_font_fontST
'     - initializes a font_fontST struct
'   o fun03: freeStack_font_fontST
'     - frees all variables in a font_fontST struct
'   o fun04: freeHeap_font_fontST
'     - frees a font_fontST struct
'   o fun05: getFont_font_fontST
'     - get a font from my ascii font file
'   o fun06: measureText_font_fontST
'     - measure how many pixels long a cstring is
'   o fun07: fontToC_font_fontST
'     - convert a font_fontST to to .c and .h files
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "fontST.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"

/*only using the .h files*/
#include "../genLib/64bit.h"

/*-------------------------------------------------------\
| Fun01: blank_font_fontST
|   - blanks a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to blank
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthArySS to be
|       set to null/0
|     o frees all pionters in pixAryUC
\-------------------------------------------------------*/
void
blank_font_fontST(
   struct font_fontST *fontSTPtr
){
   signed int siPos = 0;

   if(! fontSTPtr)
      return;

   for(siPos=0; siPos < def_numAsciiChar_fontST; ++siPos)
   { /*Loop: free all characters*/
      if(fontSTPtr->pixAryUC[siPos])
         free(fontSTPtr->pixAryUC[siPos]);
   } /*Loop: free all characters*/

   init_font_fontST(fontSTPtr);
      /*calling init or freeing memory is unusal in a
      `  blank function for me. However, I do not want to
      `  deal with keeping track of memory size for each
      `  character. So, it is easier to free on blank and
      `  reallocate later
      */
} /*blank_font_fontST*/

/*-------------------------------------------------------\
| Fun02: init_font_fontST
|   - initializes a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to initialize
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthArySS to be
|       set to null/0
\-------------------------------------------------------*/
void
init_font_fontST(
   struct font_fontST *fontSTPtr
){
   signed int siPos = 0;

   if(! fontSTPtr)
      return;

   fontSTPtr->heightSS = 0;
   fontSTPtr->gapSS = 0;

   for(siPos=0; siPos < def_numAsciiChar_fontST; ++siPos)
   { /*Loop: set all values to 0*/
      fontSTPtr->pixAryUC[siPos] = 0;
      fontSTPtr->widthArySS[siPos] = 0;
      fontSTPtr->lenArySS[siPos] = 0;
   } /*Loop: set all values to 0*/

   /*I would often call a blank function, but in this case
   `  it is more efficent to initialize
   */
} /*init_font_fontST*/

/*-------------------------------------------------------\
| Fun03: freeStack_font_fontST
|   - frees all variables in a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all variables and initialize
\-------------------------------------------------------*/
void
freeStack_font_fontST(
   struct font_fontST *fontSTPtr
){
   if(! fontSTPtr)
      return;

   blank_font_fontST(fontSTPtr);
      /*the blank function frees and initializes*/
} /*freeStack_font_fontST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_font_fontST
|   - frees a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to free
| Output:
|   - Modifies:
|     o frees hte fontSTPtr struct, you must set the
|       pointer to 0/null
\-------------------------------------------------------*/
void
freeHeap_font_fontST(
   struct font_fontST *fontSTPtr
){
   if(! fontSTPtr)
      return;

   freeStack_font_fontST(fontSTPtr);
      /*the blank function frees and initializes*/
   free(fontSTPtr);
} /*freeHeap_font_fontST*/

/*-------------------------------------------------------\
| Fun05: getFont_font_fontST
|   - get a font from my ascii font file
| Input:
|   - fontSTPtr:
|     o font_fontST struct pionter to get the new font
|   - fontFILE:
|     o FILE pointer to file to get the font from
| Output:
|   - Modifies:
|     o fontSTPtr to have the new font
|     o fontFILE to be at the end of the file
|   - Returns:
|     o 0 for no errors
|     o -1 for no height error
|     o -2 for memory errors
|     o -1 * (lineSL + 4) for duplicate character
|     o > 0 for line the problem is on
| File format:
|   - First line has `height:<size_in_pixels>` header
|     o or `height:<size>:gap:<size_in_pixels>` header
|   - Remaning lines in the file are blank or have a
|     character
|     o header: `:<character>:<width>:` (ex: `:A:16:`)
|     o character:
|       * a `#` is a filled pixel
|       * a ` ` is an empty pixel
|       * if  you have less then less then height lines
|         then they will be filled in at the bottom 
|       * max width is 504 pixels
\-------------------------------------------------------*/
signed long
getFont_font_fontST(
   struct font_fontST *fontSTPtr,
   void *fontFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - get a font from my ascii font file
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - get the height for the font
   '   o fun05 sec03:
   '     - get the font
   '   o fun05 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define buffSize_fun05 512
   signed char buffStr[buffSize_fun05];

   signed int posSI = 0;
   signed long lineSL = 0;

   unsigned char charUC = 0;
   signed short bytesSS = 0;
   signed short charByteSS = 0;
   signed short heightSS = 0;

   signed short pixSS = 0; /*pixel in byte I am on*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - get the height for the font
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   blank_font_fontST(fontSTPtr);

   while(
      ! fontSTPtr->heightSS
      &&
        fgets(
           (char *) buffStr,
           buffSize_fun05,
           (FILE *) fontFILE
        )
   ){ /*Loop: get the height*/
      ++lineSL;
      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI; /*get off white space*/

      checkIfGap_fun05_sec02:;

      if( (buffStr[posSI] | 32) != 'g')
         ;
      else if( (buffStr[posSI + 1] | 32) != 'a')
         ;
      else if( (buffStr[posSI + 2] | 32) != 'p')
         ;
      else if( (buffStr[posSI + 3] | 32) != ':')
         ;
      else
      { /*Else: got width spacing*/
         charUC = 1;
         posSI += 4;
         posSI +=
            strToSS_base10str(
               &buffStr[posSI],
               &fontSTPtr->gapSS
            );

         if(buffStr[posSI] > 32 && buffStr[posSI] != ':')
            goto noHeight_fun05_sec04;
            /*invalid height*/
      } /*Else: got width spacing*/


      if( (buffStr[posSI] | 32) != 'h' )
         continue;
      else if( (buffStr[posSI + 1] | 32) != 'e' )
         continue;
      else if( (buffStr[posSI + 2] | 32) != 'i' )
         continue;
      else if( (buffStr[posSI + 3] | 32) != 'g' )
         continue;
      else if( (buffStr[posSI + 4] | 32) != 'h' )
         continue;
      else if( (buffStr[posSI + 5] | 32) != 't' )
         continue;
      else if( (buffStr[posSI + 6] | 32) != ':' )
         continue;
      else
      { /*Else: got height*/
         posSI += 7;
         posSI +=
            strToSS_base10str(
               &buffStr[posSI],
               &fontSTPtr->heightSS
            );

         if(buffStr[posSI] > 32 && buffStr[posSI] != ':')
            goto noHeight_fun05_sec04;

         else if(buffStr[posSI] == ':')
         { /*Else If: might have a gap size*/
            ++posSI;
            goto checkIfGap_fun05_sec02;
         } /*Else If: might have a gap size*/
      } /*Else: got height*/
   } /*Loop: get the height*/

   if(! fontSTPtr->heightSS)
      goto noHeight_fun05_sec04;
   if(! charUC)
      fontSTPtr->gapSS = fontSTPtr->heightSS / 4;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - get the font
   ^   o fun05 sec03 sub01:
   ^     - find the height of all characters
   ^   o fun05 sec03 sub02:
   ^     - get the memory for the character
   ^   o fun05 sec03 sub03:
   ^     - get the bitmap for the character
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - find the height of all characters
   \*****************************************************/

   while(
      fgets(
         (char *) buffStr,
         buffSize_fun05,
         (FILE *) fontFILE
      )
   ){ /*Loop: get the font*/
      ++lineSL;

      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI; /*get off white space*/

      getChar_fun05_sec03_sub01:;
        if(! buffStr[posSI])
           continue; /*blank line*/
        else if(buffStr[posSI] != ':')
           continue;
        else if( buffStr[posSI + 1] > 126)
           continue; /*not visible or extened ascii*/
        else if( buffStr[posSI + 1] < 32)
           continue; /*invisible or not a space*/
        else
        { /*Else: have a character*/
           ++posSI;
           charUC =
              (unsigned char)
              buffStr[posSI] - def_asciiOffset_fontST;
              /*- 32 to convert space to 0*/
           ++posSI;
        } /*Else: have a character*/


        if(buffStr[posSI] != ':')
           goto fileErr_fun05_sec04;
           /*no width given*/

        ++posSI;
        posSI +=
           strToSS_base10str(
              &buffStr[posSI],
              &fontSTPtr->widthArySS[charUC]
           );
        if(buffStr[posSI] != ':')
           goto fileErr_fun05_sec04;

        if(fontSTPtr->pixAryUC[charUC])
           goto duplicateChar_fun05_sec04;
           /*I have already read in this character*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - get the memory for the character
      \**************************************************/

      bytesSS = fontSTPtr->widthArySS[charUC];
      bytesSS *= fontSTPtr->heightSS;

      if(bytesSS % def_bitsPerChar_64bit)
      { /*If: will loose pixels with a floor*/
         bytesSS /= def_bitsPerChar_64bit;
         ++bytesSS; /*account for flooring*/
      } /*If: will loose pixels with a floor*/

      else
         bytesSS /= def_bitsPerChar_64bit;

      fontSTPtr->pixAryUC[charUC] =
         calloc(bytesSS, sizeof(unsigned char));
      if(! fontSTPtr->pixAryUC[charUC])
         goto memErr_fun05_sec04;
      fontSTPtr->lenArySS[charUC] = bytesSS;

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - get the bitmap for the character
      *   o fun05 sec03 sub03 cat01:
      *     - get line and make sure not a new character
      *   o fun05 sec03 sub03 Cat02:
      *     - add the line to the character bitmap
      *   o fun05 sec03 sub03 cat03:
      *     - line may not end at the width of the
      *       character; move over the missing pixels
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub03 Cat01:
      +   - get line and make sure not a new character
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      charByteSS = 0;
      pixSS = 0;
      heightSS = 0;

      while(
         fgets(
            (char *) buffStr,
            buffSize_fun05,
            (FILE *) fontFILE
         )
      ){ /*Loop: get the bitmap*/
         ++heightSS;
         ++lineSL;
         posSI = 0;

         while(buffStr[posSI] && buffStr[posSI] < 33)
            ++posSI;
            /*get past white space to check if header*/

         if(buffStr[posSI] == ':' )
             goto getChar_fun05_sec03_sub01;
         else
            posSI = 0;

         if(pixSS >= def_bitsPerChar_64bit)
         { /*If: need to move to the next byte*/
            pixSS = 0;
            ++charByteSS;
         } /*If: need to move to the next byte*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun05 Sec03 Sub03 Cat02:
         +   - add the line to the character bitmap
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(buffStr[posSI] > 31)
         { /*Loop: copy the bitmap*/
           for(; pixSS < def_bitsPerChar_64bit; ++pixSS)
           { /*Loop: add pixels*/
              if(posSI >= fontSTPtr->widthArySS[charUC])
                 break; /*the width of the character*/
              else if(buffStr[posSI] < 32)
                 break;
              else if(buffStr[posSI] == ' ')
                 ; /*blank cell*/
              else if(buffStr[posSI] == '#')
                fontSTPtr->pixAryUC[charUC][charByteSS] |=
                   (unsigned char) (1 << pixSS);
              else
                 goto fileErr_fun05_sec04;

              ++posSI;
           }  /*Loop: add pixels*/

           if(pixSS >= def_bitsPerChar_64bit)
           { /*If: need to move to the next byte*/
              pixSS = 0;
              ++charByteSS;
           } /*If: need to move to the next byte*/

           if(posSI >= fontSTPtr->widthArySS[charUC])
              break; /*the width of the character*/
           else if(! buffStr[posSI])
              break; /*end of line*/
         }  /*Loop: copy the bitmap*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun05 Sec03 Sub03 Cat03:
         +   - line may not end at the width of the
         +     character; move over the missing pixels
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(posSI < fontSTPtr->widthArySS[charUC])
         { /*Loop: account for jagged ends*/
            for( ; pixSS < def_bitsPerChar_64bit; ++pixSS)
            { /*Loop: move past pixel*/
               if(posSI >= fontSTPtr->widthArySS[charUC])
                  break; /*the width of the character*/
               ++posSI;
            } /*Loop: move past pixel*/

            if(pixSS >= def_bitsPerChar_64bit)
            { /*If: filled the last byte*/
               pixSS = 0;
               ++charByteSS;
            } /*If: filled the last byte*/

            if(posSI >= fontSTPtr->widthArySS[charUC])
               break; /*the width of the character*/
         } /*Loop: account for jagged ends*/

         if(heightSS >= fontSTPtr->heightSS)
            break; /*finished getting the character*/
      }  /*Loop: get the bitmap*/
   } /*Loop: get the font*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun05_sec04:;
      return -2;
   noHeight_fun05_sec04:;
      return -1;
   fileErr_fun05_sec04:;
      return lineSL;
   duplicateChar_fun05_sec04:;
      return -1 * (lineSL + 4);
} /*getFont_font_fontST*/

/*-------------------------------------------------------\
| Fun06: measureText_font_fontST
|   - measure how many pixels long a cstring is
| Input:
|   - textStr:
|     o cstring with text to get the length of
|   - fontSTPtr:
|     o font_fontST struct pointer with the font to use
| Output:
|   - Returns:
|     o length in pixels of the text
\-------------------------------------------------------*/
signed short
measureText_font_fontST(
   signed char *textStr,
   struct font_fontST *fontSTPtr
){
   signed long lenSL = 0;

   while(*textStr)
   { /*Loop: get the length*/
      lenSL +=
        fontSTPtr->widthArySS[
             ((unsigned char) *textStr)
           - def_asciiOffset_fontST
        ];
      lenSL += fontSTPtr->gapSS;
      ++textStr;
   } /*Loop: get the length*/

   return lenSL;
} /*measureText_font_fontST*/

/*-------------------------------------------------------\
| Fun07: fontToC_font_fontST
|   - convert a font_fontST to to .c and .h files
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to convert to a .c and
|       .h file
|   - prefixStr:
|     o cstring with name of the font file
|     o must be a valid function name for c
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
|       * can include a file path `.` or `/`
| Output:
|   - Prints:
|     o code to auto load the font to a .c and .h file
|   - Returns:
|     o 0 for no errors
|     o 1 if prefixStr is not a valid function name
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
|     o 2 if could not open the .c file
|     o 3 if could not open the .h file
\-------------------------------------------------------*/
signed char
fontToC_font_fontST(
   struct font_fontST *fontSTPtr,
   signed char *prefixStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - convert fontST ascii font file to .c & .h files
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - print to the .c file
   '   o fun07 sec03:
   '     - print the main function in the .c file
   '   o fun07 sec04:
   '     - print the .h file
   '   o fun07 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char outStr[1024];
   signed short lenSS = 0;   /*end of output file name*/

   signed char funPrefixStr[128];
   signed short funPrefixLenSS = 0;

   unsigned char charUC = 0; /*character I am on*/
   signed short pixSS = 0;   /*pixel I am on*/

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - print to the .c file
   ^   o fun07 sec02 sub01:
   ^     - open the .c file
   ^   o fun07 sec02 sub02:
   ^     - print the libraries used
   ^   o fun07 sec02 sub03:
   ^     - print the header for each character
   ^   o fun07 sec02 sub04:
   ^     - print function name and input
   ^   o fun07 sec02 sub05:
   ^     - add memory allocation
   ^   o fun07 sec02 sub06:
   ^     - add each pixel and end the function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec02 Sub01:
   *   - open the .c file
   \*****************************************************/

   lenSS = 0;
   if(prefixStr[0] >= 'A' && prefixStr[0] <= 'Z')
      ;
   else if(prefixStr[0] >='a' && prefixStr[0] <='z')
      ;
   else if(prefixStr[0] =='_')
      ;
   else if(prefixStr[0] =='.')
      ;
   else
      goto badPrefix_fun08_sec05;

   while(prefixStr[lenSS])
   { /*Loop: check for invalid characters*/
      if(
        prefixStr[lenSS] >= 'A' && prefixStr[lenSS] <= 'Z'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(
         prefixStr[lenSS] >='a' && prefixStr[lenSS] <='z'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(
         prefixStr[lenSS] >='0' && prefixStr[lenSS] <='9'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(prefixStr[lenSS] =='_')
         outStr[lenSS] = prefixStr[lenSS];

      else if(prefixStr[lenSS] =='.')
         outStr[lenSS] = prefixStr[lenSS];

      else if(prefixStr[lenSS] =='/')
         outStr[lenSS] = prefixStr[lenSS];
         /*user is making a file path*/

      else
         goto badPrefix_fun08_sec05;

      if(outStr[lenSS] == '.')
         funPrefixLenSS = 0;
      else if(outStr[lenSS] == '/')
         funPrefixLenSS = 0;
      else
         funPrefixStr[funPrefixLenSS++] = outStr[lenSS];

      ++lenSS;
   } /*Loop: check for invalid characters*/

   funPrefixStr[funPrefixLenSS] = 0;

   outStr[lenSS] = '.';
   outStr[lenSS + 1] = 'c';
   outStr[lenSS + 2] = 0;

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto noCFile_fun07_sec05;
 
   /*****************************************************\
   * Fun07 Sec02 Sub02:
   *   - print the libraries used
   \*****************************************************/

   fprintf(outFILE, "#ifdef PLAN9\n");
   fprintf(outFILE, "   #include <u.h>\n");
   fprintf(outFILE, "   #include <libc.h>\n");
   fprintf(outFILE, "#else\n");
   fprintf(outFILE, "   #include <stdlib.h>\n");
   fprintf(outFILE, "#endif\n\n");

   fprintf(outFILE, "#include \"fontST.h\"\n\n");

   /*these are libraries that are included in the
   `  fontST library
   */
   fprintf(
     outFILE,
     "/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
   );
   fprintf(outFILE, "! Hidden libraries:\n");
   fprintf(outFILE, "!   - std #include <stdio.h>\n");
   fprintf(
      outFILE,
      "!   - .c  #include \"../genLib/ulCp.h\"\n"
   );
   fprintf(
      outFILE,
      "!   - .c  #include \"../genLib/base10str.h\"\n"
   );
   fprintf(
      outFILE,
      "!   - .c  #include \"../genLib/endin.h\"\n"
   );
   fprintf(
      outFILE,
      "!   - .c  #include \"../genLib/checksum.h\"\n"
   );
   fprintf( outFILE, "!   - .c  #include \"mkPng.h\"\n");
   fprintf(
      outFILE,
      "!   - .h  #include \"../genLib/64bit.h\"\n"
   );
   fprintf(
     outFILE,
     "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/\n"
   );
   fprintf(outFILE, "\n");

   /*****************************************************\
   * Fun07 Sec02 Sub03:
   *   - print the header for each character
   \*****************************************************/

   for(
      charUC = 0;
      charUC < def_numAsciiChar_fontST;
      ++charUC
   ){ /*Loop: print functions to print each character*/
      fprintf(
         outFILE,
         "/*___________________________________________\n"
      );
      fprintf(
         outFILE,
         "` Fun%02i: addAscii%i_%s\n",
         charUC,
         charUC + 32,
         funPrefixStr
      );
      fprintf(
         outFILE,
         "`  - adds character `%c` (ascii %i) to font\n",
         charUC + 32,
         charUC + 32
      );
      fprintf(
         outFILE,
         "`  - returns 1 for success or 0 for failure\n"
      );
      fprintf(
         outFILE,
         "`__________________________________________*/\n"
      );

      /**************************************************\
      * Fun07 Sec02 Sub04:
      *   - print function name and input
      \**************************************************/

      fprintf(outFILE, "signed char\n");

      fprintf(
         outFILE,
         "addAscii%i_%s(\n",
         charUC + 32,
         funPrefixStr
      );

      fprintf(
         outFILE,
         "   struct font_fontST *fontSTPtr\n"
      );
      fprintf(outFILE, "){\n");

      /**************************************************\
      * Fun07 Sec02 Sub05:
      *   - add memory allocation
      \**************************************************/

      fprintf(
         outFILE,
         "   if(fontSTPtr->pixAryUC[%u])\n",
         charUC
      ); /*check if need to free memory*/

      fprintf(
         outFILE,
         "      free(fontSTPtr->pixAryUC[%u]);\n\n",
         charUC
      ); /*free memory*/

      fprintf(
         outFILE,
         "   fontSTPtr->pixAryUC[%u] =\n",
         charUC
      ); /*add memroy*/

      fprintf(
         outFILE,
         "     calloc(%i, sizeof(unsigned char));\n",
         fontSTPtr->lenArySS[charUC]
      ); /*add memory calloc call*/

      fprintf(
         outFILE,
         "   if(! fontSTPtr->pixAryUC[%u])\n",
         charUC
      ); /*check if failed to add memroy*/

      fprintf(outFILE, "      return 1;\n\n");
         /*check if failed to add memroy*/

      fprintf(
         outFILE,
         "   fontSTPtr->widthArySS[%i] = %i;\n",
         charUC,
         fontSTPtr->widthArySS[charUC]
      ); /*add the width of the character*/

      fprintf(
         outFILE,
         "   fontSTPtr->lenArySS[%i] = %i;\n\n",
         charUC,
         fontSTPtr->lenArySS[charUC]
      ); /*add thh number bytes used to store the char*/

      /**************************************************\
      * Fun07 Sec02 Sub06:
      *   - add each pixel and end the function
      \**************************************************/

      for(
         pixSS = 0;
         pixSS < fontSTPtr->lenArySS[charUC];
         ++pixSS
      ){ /*Loop: add each pixel indivdually*/
         fprintf(
            outFILE,
            "   fontSTPtr->pixAryUC[%u][%i] = %i;\n",
            charUC,
            pixSS,
            fontSTPtr->pixAryUC[charUC][pixSS]
         );
      }  /*Loop: add each pixel indivdually*/

      fprintf(outFILE, "\n   return 0;\n");
      fprintf(
         outFILE,
         "} /*addAscii%i_%s*/\n\n",
         charUC + 32,
         funPrefixStr
      );
   }  /*Loop: print functions to print each character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - print the main function in the .c file
   ^   o fun07 sec03 sub01:
   ^     - print the header for the function
   ^   o fun07 sec03 sub02:
   ^     - print the function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec03 Sub01:
   *   - print the header for the function
   \*****************************************************/

   fprintf(
      outFILE,
      "/*_____________________________________________\n"
   );
   fprintf(
      outFILE,
      "` Fun%02i: loadFont_%s\n",
      charUC,
      funPrefixStr
   );
   fprintf(
      outFILE,
      "`  - loads the font in %s.c/h to the input\n",
      funPrefixStr
   );
   fprintf(
      outFILE,
      "`    font_fontST struct (as pionter)\n"
   );
   fprintf(
      outFILE,
      "`  - returns 1 for success or 0 for failure\n"
   );
   fprintf(
      outFILE,
      "`_____________________________________________*/\n"
   );

   /*****************************************************\
   * Fun07 Sec03 Sub02:
   *   - print the function
   \*****************************************************/

   fprintf(outFILE, "signed char\n");
   fprintf(outFILE, "loadFont_%s(\n", funPrefixStr);
   fprintf(
      outFILE,
      "   struct font_fontST *fontSTPtr\n"
   );
   fprintf(outFILE, "){\n");


   fprintf(
      outFILE,
      "   fontSTPtr->heightSS = %i;\n",
      fontSTPtr->heightSS
   );
   fprintf(
      outFILE,
      "   fontSTPtr->gapSS = %i;\n\n",
      fontSTPtr->gapSS
   );

   for(
      charUC = 0;
      charUC < def_numAsciiChar_fontST;
      ++charUC
   ){ /*Loop: print the user function*/
      fprintf(
         outFILE,
         "   if( addAscii%i_%s(fontSTPtr) )\n",
         charUC + 32,
         funPrefixStr
      ); /*add each character to the function*/

      fprintf(
         outFILE,
         "      return 1;\n"
      ); /*if had a memory error*/
   }  /*Loop: print the user function*/

   fprintf(outFILE, "\n   return 0;\n");
   fprintf(outFILE, "} /*loadFont_%s*/\n", funPrefixStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - print the .h file
   ^   o fun07 sec04 sub01:
   ^     - print the header for the .h file
   ^   o fun07 sec04 sub02:
   ^     - print the font load function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *   - print the header for the .h file
   \*****************************************************/

   fclose(outFILE);
   outFILE = 0;

   outStr[lenSS] = '.';
   outStr[lenSS + 1] = 'h';
   outStr[lenSS + 2] = 0;

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto noHFile_fun07_sec05;

   outStr[lenSS] = 0;
   lenSS = 0;
   pixSS = 0;
   while(funPrefixStr[pixSS])
   { /*Loop: convert the prefix to uppercase*/
      if(funPrefixStr[pixSS] >= 'a')
         outStr[lenSS++] = funPrefixStr[pixSS] & ~32;

      else if(funPrefixStr[pixSS] < 'A')
         outStr[lenSS++] = funPrefixStr[pixSS];

      else if(funPrefixStr[pixSS] > 'Z')
         outStr[lenSS++] = funPrefixStr[pixSS];

      else
      { /*Else: is an uppercase letter*/
         outStr[lenSS++] = '_';
         outStr[lenSS++] = funPrefixStr[pixSS];
      } /*Else: is an uppercase letter*/

      ++pixSS;
   } /*Loop: convert the prefix to uppercase*/


   outStr[lenSS] = 0;

   fprintf(
      outFILE,
      "#ifndef LOAD_THE_%s_FONT_H\n",
      outStr
   );
   fprintf(
      outFILE,
      "#define LOAD_THE_%s_FONT_H\n\n",
      outStr
   );
   fprintf(outFILE, "struct font_fontST;\n\n");

   /*****************************************************\
   * Fun07 Sec04 Sub02:
   *   - print the font load function
   \*****************************************************/

   fprintf(
      outFILE,
      "/*______________________________________________\n"
   );
   fprintf(
      outFILE,
      "` Fun%02i: loadFont_%s\n",
      charUC,
      funPrefixStr
   );
   fprintf(
      outFILE,
      "`  - loads the font in %s.c/h to the input\n",
      funPrefixStr
   );
   fprintf(
      outFILE,
      "`    font_fontST struct (as pionter)\n"
   );
   fprintf(
      outFILE,
      "`  - returns 1 for success or 0 for failure\n"
   );
   fprintf(
      outFILE,
      "`_____________________________________________*/\n"
   );

   fprintf(outFILE, "signed char\n");
   fprintf(outFILE, "loadFont_%s(\n", funPrefixStr);
   fprintf(
      outFILE,
      "   struct font_fontST *fontSTPtr\n"
   );
   fprintf(outFILE, ");\n\n");
   fprintf(outFILE, "#endif\n");


   fclose(outFILE);
   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lenSS = 0;
   goto ret_fun07_sec05;

   badPrefix_fun08_sec05:;
      lenSS = 1;
      goto ret_fun07_sec05;

   noCFile_fun07_sec05:;
      lenSS = 2;
      goto ret_fun07_sec05;

   noHFile_fun07_sec05:;
      lenSS = 3;
      goto ret_fun07_sec05;

   ret_fun07_sec05:;
      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return (signed char) lenSS;
} /*fontToC_font_fontST*/

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
