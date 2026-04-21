# mkPng:

Goal is a basic program to create pngs with. This does not
  use compression. Also, it is only designed to output
  pallet pngs.

Files: mkPng.h and mkPng.c

- Dependencies:
  - ../genLib/checkSum.h
  - ../genLib/endin.h
  - ../genLib/64bit.h (.h file only)

The flow is to make a png structure (st\_mkPng), then draw
  you image in the structure (see drawPng or make your
  own functions). Then print out the png with the print
  function.

The one thing to be aware of is the final png width is
  always a multiple of 8.

# st\_mkPng structure

The st\_mkPng structure is what stores the png. You should
  leave most of the variables alone. Here are a few
  variables that you might look at or use.

- Variables worth knowing:
  - pixelAryUC: array of pixels in the png
    - each byte is one pixel in the png
    - this uses the pallet system, so set each byte to
      the index of the color you want to use
  - widthUS: width of the png (do not change)
    - this is set with the `setup_st_mkPng()` and
      `mk_st_mkPng()` function
  - heightUS: height of the png (do not change)
    - this is set with the `setup_st_mkPng()` and
      `mk_st_mkPng()` function
  - numPixelSL: number of total pixels in the png
    - Is really `widthUS * heightUS`

You can get the index of a single pixel in the png with
  `index = (widthUS * yCoordinate) +xCoordinate;`, were
  widthUS is in the st\_mkPng structure.

The other variables are set by functions and should not
  be messed with.

# Functions:

## General overview:

The flow of mkPng is to first create your png with the
  `mk_st_mkPng()` function. Then add your colors to the
  color pallet with the `addCol_st_mkPng()` functions.
  You can then build your image with your own functions or
  by using the functions from drawPng.c. When finished you
  can print your png to a file using the
  `print_st_mkPng()` function. Finally, you can free your
  png structure using the `freeHeap_st_mkPng()` function.

If you want to reuse your png structure you can call
  `blank_st_mkPng()` to clear the png. Then use
  `setup_st_mkPng()` if you need to change the size of
  the png or remove your previously defined colors.

You can also use the `rmCol_st_mkPng()` function to remove
  any colors (by index) from your color pallet.

## making a st\_mkPng struct and adding colors

### making and reusing a st\_mkPng structure

You can make a st\_mkPng structure using the
  `mk_st_mkPng()` function.

- Input:
  1. Width in pixels of the png
     - if not divisible by eight, is set to the nearest
       number divisible by eight (ex: 20 goes to 24)
     - you can use 0 for the default value
  2. Height of the png in pixels
     - you can use 0 for the default value
  3. Number of colors you expect to use in the png
     - highest number is 255
- Output:
  - Returns:
    - pointer to a st\_mkPng structure (is setup for use)
    - 0 if had a memory error

```
struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);
   /*png that is 960 pixels wide, 720 pixels high and has
   `  four colors in the pallete
   */
```

### Reusing a st\_mkPng structure

To reuse a st\_mkPng structure you will first have to
  blank it and then set it up.

You can use the `blank_st_mkPng()` function to set all
  pixels to the 0th color in the color pallet. The input
  is a pointer to a st\_mkPng structure.

You can then use the `setup_st_mkPng()` to change the png
  size or color pallet size.

- Input:
  1. Pointer to a st\_mkPng structure
  2. Width of the new png
  3. Height of the new png
  4. Maximum number of colors to use
- Output:
  - Modifies:
    - the st\_mkPng (input 1) structure to have the new
      png size
    - the colors in the st\_mkPng (input 1) struct are
      set to greyscale
  - Returns:
    - 0 for no errors
    - def_memErr_mkPng for memory errors

```
struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);
blank_st_mkPng(pngHeapST);
setup_st_mkPng(pngHeapST, 420, 480, 4);
   /*resize the png to be 420 pixels wide and 480 pixels
   `  high
   */
```

## adding and removing colors

### adding colors

You can changes colors in your color pallet a st\_mkPng
  structure using the `addCol_st_mkPng()` function.

- Input:
  1. Pointer to a st\_mkPng structure to add the color to
  2. Value for the red channel (0 to 255)
  3. Value for the blue channel (0 to 255)
  4. Value for the green channel (0 to 255)
  5. Color pallet index (index 0) to put the new color at
  6. Signed char pointer to get errors or changes
     - You should ignore this
- Output:
  - Modifies:
    - The st\_mkPng (input 1) structure to have the new
      color
    - error value (input 6) to be (you should ignore this)
      - 0 for no errors
      - def_overflow_mkPng if went of 255 colors
      - def_bitChange_mkPng if the bit depth (numbers of
        pixels per byte) changed (decreased)
  - Returns (you can ignore this):
    - index of the added color
    - -1 if your index color was over 255
      - This is the maximum number of colors in a png
        pallet. You should already have checks for this.

```
signed char errSC = 0;
struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);

addCol_st_mkPng(
   pngHeapST,
   0xFD,   /*red channel*/
   0xE7   /*blue channel*/
   0x25,  /*green channel*/
   3,     /*make this color the 4th color*/
   &errSC /*ignore this*/
); /*add the yellow color (from viridis magma pallet)*/
```

### removing colors

You can remove colors with the `rmCol_st_mkPng()`
  function. This function will also shift all forward
  colors one index back. So, be warned, this will mess up
  you indexing.

- Input:
  1. Pointer to a st\_mkPng structure to add the color to
  2. Index of the color to remove
- Output:
  - Modifies:
    - The st\_mkPng (input 1) structure to have the color
      at the input index (input 2) removed
  - You should ignore all return values from this
    function.
    - If the return is not 0 it means the bit depth
      (pixels per byte) has changed (increased)

```
signed char errSC = 0;
struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);
rmCol_st_mkPng(pngHeapST, 3); /*remove the 4th color*/
```

If you need to remove all colors call the
  `initCol_st_mkPng()` function. This will overwrite all
  existing colors with greyscale. It will also reset the
  current color limit.

- Input:
  1. Pointer to a st\_mkPng structure to add the color to
  2. New maximum number of colors
- Output:
  - Modifies:
    - The st\_mkPng structure to have the new maximum
      color count (input 2) number of greyscale colors
  - Returns (you can ignore this):
    - 0 for no change in bit depth
    - not 0 if had change in bit depth (pixels per byte)

```
struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);
initCol_st_mkPng(pngHeapST, 16);
   /*clear old/default colors and use 16 colors instead of
   ` 4
   */
```

## printing and freeing

You can print your png to a file using the
  `print_st_mkPng()` function.

- Input:
  1. Pointer to a st\_mkPng structure to add the color to
  2. FILE pointer to print the png to
- Output:
  - Prints: the png to the FILE (input 2)

When you are finished with your st\_mkPng structure you
  can free it using the `freeHeap_st_mkPng()` function.
  The input is a pointer to the st\_mkPng structure to
  free. You must set the pointer to null.

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "mkPng.h"

int
main(
   void
){
   signed int ySI = 0;
   signed int xSI = 0;
   signed int positionSI = 0;
   struct st_mkPng pngHeapST = mk_st_mkPng(960, 720, 4);
   
   FILE *outFILE = stdout;
      /*stdout is easy to use for an example*/
   
   if(! pngHeapST)
     return 1;
   
   /*this loop builds an image were each pixel is set to
   `  a different color. It does nothing useful
   */
   for(ySI = 200; ySI < pngHeapST->heightUS; ++ySI)
   { /*Loop: fill in some rows*/
      for(xSI = 10; xSI < 100; ++xSI)
      { /*Loop: fill in some columns*/
         positionSI = (ySI * pngHeapST->widthUS) + xSI;
         pngHeapST->pixelAryUC[positionSI] = positionSI % 4;
            /*fill in some random colors*/
      } /*Loop: fill in some columns*/
   } /*Loop: fill in some rows*/
   
   print_st_mkPng(pngHeapST, outFILE);
   freeHeap_st_mkPng(pngHeapST);
   pngHeapST = 0;
   outFILE = 0;
   return 0;
} /*main*/
```
