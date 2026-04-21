# fontST:

Has functions to print my font to a st\_mkPng structure
  (png). Also, has the function to reads in a font from an
  ascii font file.

Files: fontST.h and fontST.c

- Dependencies:
  - ../genLib/ulCp.h
  - ../genLib/base10str.h
  - ../genLib/64bit.h (.h file only)
- Hidden dependencies:
  - ../genLib/checkSum.h
  - ../genLib/endin.h

This library stores the uploaded font in a font\_fontST
  structure. Valid characters for printing are ascii
  32 (space) to ascii 128 (~). New lines, carriage
  returns, and tabs are not valid characters.

# font\_fontST:

The font\_fontST structure stores the font in a bitmap.

- Variables:
  - pixAryUC: is the unsigned char pointer array that has
    the bitmap for each each character in the font
    - length is 94 characters (all visible ascii + space)
    - each unsigned char array represents one character
    - each bit in the character array is one pixel
      - 1 is forground
      - 0 is bacground
  - widthArySS: array with the widith in pixels of each
    character in the font
  - lenArySS: number of bytes each character in the font
    has
  - hieghtSS: height in pixels of the font
  - gapSS: gap in pixels between each character in the
    font

# The font file

The font file for fontST is an ascii file. The first line
  should start with `height:<font_height_in_pixels`. If
  you want to set the gap between characters you can do
  `height:<font_height>:gap:<font_gap_in_pixels>`.

The remaning lines have the font.

Each character starts with a header that has the character
  to use and the width of the character in pixels. This
  header is `:<character>:<width_in_pixels>:`.

Each line after the header is a row of pixels in the
  character. The character stops when the height is
  reached or the next characters header is found.

If you have more rows then the fonts height, then the
  extra rows at the end are ignored. If you have less rows
  then the fonts height, then the blank rows are added at
  the bottom of the character.

For each row of pixels, only the width\_in\_pixels number
  of characters are read in. Any characters past the width
  are ignored. The maximum number of characters allowed
  per row is 504. Otherwise fgets not get the full line
  and this will mess up the function.

If you have less columns then your characters width, then
  blank columns are added in.

In a row each pixels is represented by a space (` `) or a
  pound symbol (`#`). The space is for a blank pixel
  (background) and is used to positionthe pounds. The
  poud is the character (forground).

Any character that is not a space, pound, or the next
  header is invalid. This is done intentionally because it
  gives an idea of how the font will really look.

Here is the header and tfirst two characters from the
  default font (it is a 12pt font).

```
height:16:gap:1
:A:16:                               
       ##
      ####
     ######
    ###  ###
   ###    ###
  ###      ###
  ############
  ############
  ###      ###
  ###      ###
  ###      ###
  ###      ###
 #####    #####
#######  #######


:B:16:
#############
##############
###         ###
###          ###
###          ###
###         ###
##############
##############
###         ###
###          ###
###          ###
###         ###
##############
#############
```

## Creating a font with large language models

You can create a font file using a large language model.
  I did this for the ibm plex san font that is 9 rows.

Here is the prompt that was refined by co-pilot. You
  should only need to change the font name.

```
I want you to generate a bitmap version of the following
  font:

Font family: <IBM Plex Serif / Sans / Mono / etc.>
Style: <Regular / Bold / Italic>
Pixel height: <number of rows>
Width: Variable-width per character
Character set: ASCII 32–126 only

Output format:
For each character, print a header in the form:
:<character>:<width_in_pixels>:

At the top of the file print height:<Pixel height>:<Horizontal pixel gap between characters>
Width is the number of pixels to pad after the character

Then print exactly <pixel height> rows using only '#' for
  filled pixels and ' ' (space) for empty pixels.

Design goals:
• Keep glyphs visually consistent with the chosen font family.
• Maintain a shared baseline across all characters.
• Use proportions that remain readable at the given pixel height.
• Avoid adding extra blank lines between characters.

Please generate the characters in ascending ASCII order.

For the header for ' print :':<Width>:
For the header for " print :":<Width>:
For the header for : print :::<Width>:
```

# Using fontST:

You first step is to create a font\_fontST structure.
  After creating the structure you will initialize it.
  Then load your font. When finished with your font you
  need to free it.

## Creating a font\_fontST structure

Creation of a font\_fontST structure is done by you.

After creation you need to initialize the structure with
  the `init_font_fontST()` function. The input is a
  pointer to your font\_fontST structure.

```
#include "fontST.h"

struct font_fontST fontStackST;
init_font_fontST(&fontStackST);
```

## Loading a font into a font\_fontST structure

### Loading the font from a file

You can load a font into your font\_fontST using a file
  with an ascii font or by using an auto generated font
  stored in a .c and .h file.

The `getFont_font_fontST()` function will allow you to
  load a font from a file.

- Input
  1. is a pointer to your font\_fontST structure
  2. the file with the font (a FILE pionter)
- Returns:
  - 0 for no errors
  - -1 if file did not list the height of the font
  - -2 for a memroy error
  - -1 x (line + 4) if had a duplicate character in the
    file (input 2)
    - get the line number with (line x -1) - 4
  - > 0 if had a file error (is the line with the problem)

```
#include "fontST.h"

signed long errSL = 0;
struct font_fontST fontStackST;
FILE *fontFILE = fopen("<your_font_file>", "r");

init_font_fontST(&fontStackST);
if(! fontFILE)
   goto err_main:;


errSL = getFont_fong_fontST(&fontStackST, fontFILE);
if(! errSL)
   ; /*no error*/
else
   goto err_main; /*some kind of error*/


errSL = 0;
goto ret_main;

err_main:;
   errSL = 1;

ret_main:;
   freeStack_font_fontST(&fontStackST);

   if(fontFILE)
      fclose(fontFILE);
   fontFILE = 0;

   return errSL;
```

### Loading the font from an auto-generated function

Another way to load a font is from an auto-generated
  function made by the `fontToC_font_fontST()` function.
  The function name will be `loadFont_<your_font_name>()`.

- Input
  1. pointer to your font\_fontPng structure.
- Returns:
  - 0 for no errors
  - 1 for a memory error

```
#include "fontST.h"
#include "<your_font_name>.h"

struct font_fontST fontStackST;

init_font_fontST(&fontStackST);

if( loadFont_<your_font_name>(&fontStackST) )
{ /*If: memory error*/
   freeStack_font_fontST(&fontStackST);
   return 1;
} /*If: memory error*/

return 0;
```

## Freeing your structure

You can free your font\_fontST structure using the
  `freeStack_font_fontST()` function or the
  `freeHeap_font_fontST()` function. The freeStack
  function frees the variables in the font\_fontST
  structure, while the freeHeap function frees the
  structure (you must set to null or 0).

The input for both functions is a pointer to the
  font\_fontST structure to free.

### An example using freeStack:

```
#include "fontST.h"

struct font_fontST fontStackST;
init_font_fontST(&fontStackST);

/*your code goes here*/
freeStack_font_fontST(&fontStackST);
```

### An example using freeHeap:

```
#include "fontST.h"

struct font_fontST *fontHeapST =
   malloc(sizeof(struct font_fontST));

if(! fontHeapST)
   return 1;
init_font_fontST(fontHeapST);

/*your code goes here*/
freeHeap_font_fontST(&fontHeapST);
```

## Misc functions

These are functions that I can not put into a bin.

### Measure text

If you need to find the length of your printed text you
  can use the `measureText_font_fontST()` function.

- Input:
   1. c-string with text to measure
   2. pointer to a font\_fontST structure with the font
- Returns:
  - length of the text (input 1) when printed with the
    font (input 2)

```
#include <stdio.h>
#include "fontST.h"

signed long textLengthSL = 0;
struct font_fontST fontStackST;
   
init_font_fontST(&fontStackST);

if( loadFont_pngDefaultFont(&fontStackST) )
{ /*If; failed to load my font*/
   freeStack_font_fontST(&fontStackST);
   return 1;
} /*If; failed to load my font*/

testLengthSL =
   measureText_font_fontST(
      (signed char *) "some text to measure",
      &fontStackST
   );
printf("My text is %li characters long\n", textLengthSL);

freeStack_font_fontST(&fontStackST);
return 0;
```

### Convert a font to .c and .h file

Sometimes you want your font to be embed in your code.
  In this case we can make a .c and .h file to load the
  font for you using the `fontToC_font_fontST()`
  function. The output creates a function that takes a
  font\_fontST structure to load the font into and
  returns 0 for success and 1 for a memory error.

After making your font file you will need to open the
  `<font_name>.c` file and change the `#include fontST.h`
  line to have the path to the fontST.h file. The
  default output assumes this file is in genPng.

In the output font file change:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "fontST.h"
```

To:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "path/to/fontST.h"
```

- Input:
  1. pointer to a font\_fontST structure with the font to
     embed
  2. c-string with the name of the font
     - must be a valid c function name
       - no numbers at the start
       - allowed characters a-z, A-Z, 0-9, and `_`
     - this is the name for your output .c and .h files
- Returns:
  - 0 for success
  - 1 if the c-string (input 2) was not valid
  - 2 if could not open the .c file
  - 3 if could not open the .h file

Example: saving a font to a .c and .h file

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>
#include "fontST.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_fontST fontStackST;
   FILE *fontFILE = fopen("<your_font_file>", "r");
   
   init_font_fontST(&fontStackST);
   if(! fontFILE)
      goto err_main:;
   
   
   errSL = getFont_fong_fontST(&fontStackST, fontFILE);
   if(! errSL)
      ; /*no error*/
   else
      goto err_main; /*some kind of error*/
   fclose(fontFILE);
   fontFILE = 0;

   
   errSL =
      fontToC_font_fontST(
         &fontStackST,
         (signed char *) "myCoolFont"
      ); /*saves font to myCoolFont.c and myCoolFont.h*/
         /*the function name is loadFont_myCoolFont()*/
   if(errSL)
      goto err_main;
   

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
```

Loading the saved front (from .c and .h file):

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>
#include "fontST.h"
#include "myCoolFont.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_fontST fontStackST;
   
   init_font_fontST(&fontStackST);
   if(loadFont_myCoolFont(&fontStackST))
      goto err_main;
   
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
```
