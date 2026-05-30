/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' convertFontToFontSTC SOF : Start Of File
'   - converts my font file format to a fontST font in a
'     c file format
'   o header:
'     - included files
'   o fun01: pversion_convertFontToFontSTC
'     - prints the version number
'   o fun02: phelp_convertFontToFontSTC
'     - prints the input message for convertFontToFontSTC
'   o fun03: input_convertFontToFontSTC
'     - gets user input for convertFontToFontSTC
'   o main:
'     - driver function to convert a font text file for
'       fontST to a .c and .h file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included files
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "fontST.h"
#include "../genLib/ulCp.h"

/*only using the .h file*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .h  #include "../genLib/64bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_convertFontToFontSTC
|   - prints the version number
| Input:
|   - inFILE:
|     o FILE pointer to file to print version number to
| Output:
|   - Prints:
|     o prints the version number to inFILE
\-------------------------------------------------------*/
void
pversion_convertFontToFontSTC(
   void *inFILE
){
   fprintf(
     (FILE *) inFILE,
     "convertFontToFontSTC from bioTools: %i-%02i-%02i%s",
     def_year_bioTools,
     def_month_bioTools,
     def_day_bioTools,
     str_endLine
   );
} /*pversion_convertFontToFontSTC*/

/*-------------------------------------------------------\
| Fun02: phelp_convertFontToFontSTC
|   - prints the input message for convertFontToFontSTC
| Input:
|   - inFILE:
|     o FILE pointer to file to print help message to
| Output:
|   - Prints:
|     o prints the help messate to inFILE
\-------------------------------------------------------*/
void
phelp_convertFontToFontSTC(
   void *inFILE
){
   fprintf(
      (FILE *) inFILE,
      "convertFontToFontSTC -font font.txt%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "  - Converts the ascii file for fontST into a%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "    .c and .h file for using in C programs%s",
      str_endLine
   );

   fprintf((FILE *) inFILE, "Input:%s", str_endLine);
   fprintf(
      (FILE *) inFILE,
      "  -prefix out: [Optional; out]%s",
      str_endLine
   );
   fprintf(
     (FILE *) inFILE,
     "    o prefix to name .c and .h files (no spaces)%s",
     str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "  -font <file>.txt: [Required]%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "    o font file to convert%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o format:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "      * First line:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "        height:<height_in_pixels>:gap:<gap_in_pixels>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "      * Header for each character:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "        :<character>:<width_in_pixels>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "      * Rest of file is `#` for a filled pixels%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "        and a space or blank line for unfilled%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "        pixels%s",
      str_endLine
   );

   fprintf((FILE *) inFILE, "Output:%s", str_endLine);
   fprintf(
     (FILE *) inFILE,
     "  - Prints the font to <prefix>.c and <prefix>.h%s",
     str_endLine
   );
   fprintf(
     (FILE *) inFILE,
     "    o The header includes #include \"fontST.h\"%s",
     str_endLine
   );
   fprintf(
     (FILE *) inFILE,
     "      * you may need to change this to the%s",
     str_endLine
   );
   fprintf(
     (FILE *) inFILE,
     "        location of the fontST library%s",
     str_endLine
   );

   fprintf((FILE *) inFILE, "%s", str_endLine);
   fprintf(
      (FILE *) inFILE,
      "AI prompt to make a font text file%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  I want you to generate a bitmap version of the%s",
      str_endLine
   );
       
   fprintf(
      (FILE *) inFILE,
      "    following font:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Font family: <IBM Plex Serif / Sans / Mono / etc.>%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Style: <Regular / Bold / Italic>%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Pixel height: <number of rows>%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Width: Variable-width per character%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Character set: ASCII 32–126 only%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  Output format:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  For each character, print a header in the form:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "    :<character>:<width_in_pixels>:%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  At the top of the file print height:<Pixel height>:<Horizontal pixel gap between characters>%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  Width is the number of pixels to pad after the character%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  Then print exactly <pixel height> rows using only '#' for%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "    filled pixels and ' ' (space) for empty pixels.%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  Design goals:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  * Keep glyphs visually consistent with the chosen font family.%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  * Maintain a shared baseline across all characters.%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  * Use proportions that remain readable at the given pixel height.%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  * Avoid adding extra blank lines between characters.%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  Please generate the characters in ascending ASCII order.%s",
      str_endLine
   );
   fprintf((FILE *) inFILE, "%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  For the header for ' print :':<Width>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  For the header for \" print :\":<Width>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "  For the header for : print :::<Width>:%s",
      str_endLine
   );
} /*phelp_convertFontToFontSTC*/

/*-------------------------------------------------------\
| Fun03: input_convertFontToFontSTC
|   - gets user input for convertFontToFontSTC
| Input:
|   - argLenSI:
|     o number arugments input
|   - argAryStr:
|     o c-string array with user arguments
|   - inFileStrPtr:
|     o c-string pionter to piont to the input file
|   - prefixStrPtr:
|     o c-string pionter to pointer to the prefix name
| Output:
|   - Modifies:
|     o inFileStrPtr to pointer to the input font file to
|       convert
|     o prefixStrPtr to pointer to the input prefix
|   - Prints:
|     o help message or version number to stdout if
|       requested
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had an error
\-------------------------------------------------------*/
signed char
input_convertFontToFontSTC(
   signed int argLenSI,        /*number of arguments*/
   char *argAryStr[],          /*user arguments*/
   signed char **inFileStrPtr, /*gets input font file*/
   signed char **prefixStrPtr  /*gets user prefix*/
){
   signed int siArg = 1;

   if(! argLenSI)
      goto phelp_fun03;

   while(siArg < argLenSI)
   { /*Loop: get all user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-font", 
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: on the font file input*/
         ++siArg;
         *inFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*If: on the font file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-prefix", 
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: getting the prefix*/
         ++siArg;
         *prefixStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: getting the prefix*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h", 
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h", 
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "help", 
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help", 
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help", 
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v", 
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v", 
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "version", 
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version", 
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03;
      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version", 
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03;

      else
      { /*Else: could not get the input*/
         fprintf(
            stderr,
            "%s is not recognized%s",
            argAryStr[siArg],
            str_endLine
         );
         goto err_fun03;
      } /*Else: could not get the input*/

      ++siArg;
   } /*Loop: get all user input*/

   siArg = 0;
   goto ret_fun03;

   phelp_fun03:;
      phelp_convertFontToFontSTC(stdout);
      siArg = 1;
      goto ret_fun03;
   pversion_fun03:;
      pversion_convertFontToFontSTC(stdout);
      siArg = 1;
      goto ret_fun03;
   err_fun03:;
      siArg = 2;
      goto ret_fun03;

   ret_fun03:;
      return siArg;
} /*input_convertFontToFontSTC*/

/*-------------------------------------------------------\
| Main:
|   - driver function to convert a font text file for
|     fontST to a .c and .h file
| Input:
|   - argLenSI:
|     o number of user input arguments
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o font as text file to a .c and .h file for c
|       programs
|     o error messages to stderr
|   - Returns;
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
int
main(
   int argLenSI,
   char *argAryStr[]
){
   signed char *inFileStr = 0;
   signed char *prefixStr = 0;
   signed long errSL = 0;
   struct font_fontST fontStackST;
   FILE *inFILE = 0;

   init_font_fontST(&fontStackST);

   errSL =
      input_convertFontToFontSTC(
         argLenSI,
         argAryStr,
         &inFileStr,
         &prefixStr
      );

   if(errSL)
   { /*If: printed help, version or had error*/
      --errSL;       /*convert help/version to no error*/
      goto ret_main; /*error message already printed*/
   } /*If: printed help, version or had error*/

   inFILE = fopen((char *) inFileStr, "r");
   if(! inFILE)
   { /*If: no file*/
      fprintf(
         stderr,
         "could not open -font %s%s",
         inFileStr,
         str_endLine
      );
      goto err_main;
   } /*If: no file*/

   errSL = getFont_font_fontST(&fontStackST, inFILE);
   fclose(inFILE);
   inFILE = 0;

   if(! errSL)
      ;
   else if(errSL > 0)
   { /*Else If: bad line*/
      fprintf(
         stderr,
         "line %li in -font %s is invalid%s",
         errSL,
         inFileStr,
         str_endLine
      );
      goto err_main;
   } /*Else If: bad line*/

   else if(errSL < -2)
   { /*Else If: duplicate character*/
      fprintf(
        stderr,
        "line %li in -font %s is a duplicate character%s",
        (errSL * -1) - 1,
        inFileStr,
        str_endLine
      );
      goto err_main;
   } /*Else If: duplicate character*/

   else if(errSL == -1)
   { /*Else If: file had no height*/
      fprintf(
        stderr,
        "-font %s has no font height (height:<height>)%s",
        inFileStr,
        str_endLine
      );
      fprintf(
        stderr,
        "  on the first line%s",
        str_endLine
      );
      goto err_main;
   } /*Else If: file had no height*/

   else
   { /*Else: memory error*/
      fprintf(
        stderr,
        "memory error reading -font %s%s",
        inFileStr,
        str_endLine
      );
      goto err_main;
   } /*Else: memory error*/

   errSL = fontToC_font_fontST(&fontStackST, prefixStr);
   if(! errSL)
      ;
   else if(errSL == 1)
   { /*Else If: bad prefix*/
      fprintf(
         stderr,
         "-prefix %s has a non a-z, A-Z, _, 0-9, or /%s",
         prefixStr,
         str_endLine
      );
      goto err_main;
   } /*Else If: bad prefix*/

   else
   { /*Else: could not open an output file*/
      fprintf(
         stderr,
         "could not open .c or .h file for -prefix %s%s",
         prefixStr,
         str_endLine
      );
      goto err_main;
   } /*Else: could not open an output file*/

   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;
   ret_main:;
      freeStack_font_fontST(&fontStackST);
      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      return errSL;
} /*main*/
