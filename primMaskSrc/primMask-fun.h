/*########################################################
# Name: primMask-fun
#   - holds functions for primer masking or trimming with
#     sam files
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o .h fun-01: blankPrimFlag
'     - blanks all flags in an primFlag uint
'   o .h fun-02: setDirPrimFlag
'     - sets the direction flag for an primer
'   o .h fun-03: setPairPrimFlag
'     - sets the paired flag for an primer
'   o .h fun-04: setMateIndexPrimIndex
'     - sets the index of the other primer
'   o .h fun-05: getDirPrimFlag
'     - gets the primer direction from an primer flag uint
'   o .h fun-06: getPairPrimFlag
'     - gets if the primer is to be treated as an pair
'   o .h fun-07: getMateIndexPrimFlag
'     - gets the other primers index in the pair
'   o fun-08: maskPrimers
'     - masks the primers in the sam sequence
'   o .c fun-09: swapPrimCoords
'     - swaps two primer coordinates values around and
'       updates the index's
'   o fun-10: sortPrimCoords
'     - Sorts the primer coordiantes of a primer flag
'       array setup. Order is least to greates by first
'       starting positon, then ending position
'   o fun-11: readPrimCoords
'     - gets primer coordinates and flags from an file
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined varaibles and guards
\-------------------------------------------------------*/

#ifndef SAM_PRIMER_MASK_FUN_H
#define SAM_PRIMER_MASK_FUN_H

/*Some default values*/
#define def_mask_primMask 'N'  /*Char to mask with*/
#define def_fudge_primMask -1  /*Fudge length*/
#define def_filter_primMask 0  /*0 = do not filter*/
   /*-1 is skip/anyware*/

/*Do not change these*/
#define def_reverse_primMask 1
#define def_pair_primMask 2

#define def_fileErr_primMask 2
#define def_emptyFileErr_primMask 4
#define def_memErr_primMask 64

/*-------------------------------------------------------\
| Fun-01: blankPrimFlag
|   - blanks all flags in an primFlag uint
| Input:
|   - flagUI:
|     o unsigned int to blank
| Output:
|   - Modifies:
|     o flagUI to be 0
\-------------------------------------------------------*/
#define \
blankPrimFlag( \
   flagUI /*Has flags*/ \
){ \
   (flagUI) = 0;\
} /blankPrimFlag*/

/*-------------------------------------------------------\
| Fun-02: setDirPrimFlag
|   - sets the direction flag for an primer
| Input:
|   - flagUI:
|     o unsigned int to hold the flags and indexes
|   - revBl:
|     o 1 if this is an reverse complement primer
|     o 0 if this is an foward primer
| Output:
|   - Modifies:
|     o flagUI to have the input direction
|       def_reverse_primMask
\-------------------------------------------------------*/
#define \
setDirPrimFlag( \
   flagUI, /*Has flags*/ \
   revBl   /*1 if reverse complement, 0 if forward*/ \
){ \
   (flagUI) &= (~def_reverse_primMask);/*Clear flags*/\
   (flagUI) |= (def_reverse_primMask & ( -(revBl) )); \
} /*setPrimFlag*/

/*-------------------------------------------------------\
| Fun-03: setPairPrimFlag
|   - sets the paired flag for an primer
| Input:
|   - flagUI:
|     o unsigned int to hold the flags and indexes
|   - pairBl:
|     o 0 if this primer should not be treated as an pair
|     o 1 if this primer is treated as an pair
| Output:
|   - Modifies:
|     o flagUI to have the def_pair_primMask flag set
|       to 1 if paired or 0 if false
\-------------------------------------------------------*/
#define \
setPairPrimFlag( \
   flagUI, /*Has flags*/ \
   pairBl   /*1 if reverse complement, 0 if forward*/ \
){ \
   (flagUI) &= (~def_pair_primMask);/*Clear flags*/\
   (flagUI) |= (def_pair_primMask & ( -(pairBl) )); \
} /*setPrimFlag*/

/*-------------------------------------------------------\
| Fun-04: setMateIndexPrimIndex
|   - sets the index of the other primer
| Input:
|   - flagUI:
|     o unsigned int to hold the forward index
|   - indexUI:
|     o index of the foward primer in the primer array
| Output:
|   - Modifies:
|     o flagUI to have the foward index (after the flags)
\-------------------------------------------------------*/
#define \
setMateIndexPrimFlag( \
   flagUI, /*Has flags*/ \
   indexUI  /*Index of foward primer*/ \
){ \
   (flagUI) &= 0x00000003; /*Clear the old index*/ \
   (flagUI) |= ( (indexUI) << 2); \
} /*setMatePrimIndex*/

/*-------------------------------------------------------\
| Fun-05: getDirPrimFlag
|   - gets the primer direction from an primer flag (uint)
| Input:
|   - flagUI:
|     o unsigned int with primer direction to get
|       index from
| Output:
|   - Returns:
|     o 0 for an foward primer
|     o 1 for an reverse primer
\-------------------------------------------------------*/
#define \
getDirPrimFlag( \
   flagUI /*Has flags*/ \
)( \
   (flagUI) & def_reverse_primMask \
) /*getDirPrimFlag*/

/*-------------------------------------------------------\
| Fun-06: getPairPrimFlag
|   - gets if the primer is to be treated as an pair
| Input:
|   - flagUI:
|     o unsigned int with primer direction to get
|       index from
| Output:
|   - Returns:
|     o 0 if this is not an pair
|     o 1 if this is an pair
\-------------------------------------------------------*/
#define \
getPairPrimFlag( \
   flagUI /*Has flags*/ \
)( \
   (flagUI) & def_pair_primMask \
) /*getPairPrimFlag*/

/*-------------------------------------------------------\
| Fun-07: getMateIndexPrimFlag
|   - gets the other primers index in the pair
| Input:
|   - flagUI:
|     o unsigned int with pimer flags to get the primers
|       mate from
| Output:
|   - Returns:
|     o An short with the forward primers index
\-------------------------------------------------------*/
#define \
getMateIndexPrimFlag( \
   flagUI /*Has flags*/ \
)( \
   (flagUI) >> 2 \
) /*getMatePrimIndex*/

/*-------------------------------------------------------\
| Fun-08: maskPrimers
|   - masks the primers in the sam sequence
| Input:
|   - samSTVoidPtr:
|     o pointer to an sam structure to mask primers in
|   - primStartAryUI:
|     o uint array of primer starting coordiantes
|     o This is the starting position on the reference
|       sequence
|   - primEndAryUI:
|     o uint array of primer ending coordiantes
|     o This is the starting position on the reference
|       sequence
|   - primFlagAryUI:
|     o Int array with bits filled for the flags and
|       coordinates
|       - the first two bits have the direction,
|         o def_reverse_primMask (1) if is an reverse
|           primer
|         o def_pair_primMask (2) if you want to only
|           mask primers that have pairs instead off any
|           primer
|       - the next 15 bits (2 bytes - 1) has the index of
|         the forward primer
|       - the last 15 bits has the index of the reverse
|         primer
|   - numPrimSI:
|     o number of primers in primStartAryUI and primEndUI
|   - fudgeSI:
|     o sint with the number of bases before (start) or
|       after (end) to 
|       - -1 mask any detected primer
|       - 0 is no bases before start or after end
|       - > 0 Allow 
|   - maskSC:
|     o character to mask with
| Output:
|   - Modifies:
|     o samSTVoidPtr to have any detected primers masked
|   - Returns:
|     o 0 if no problems
|     o -1 for no detected primers
|     o (primer index << 1) | 1 if primer found but not
|        masked. This can only happen for fudge lengths
|        out of bounds or incomplete primer pairs
\-------------------------------------------------------*/
signed int
maskPrimers(
   void *samSTVoidPtr, /*samEntry struct to mask primer*/
   unsigned int *primStartAryUI,
   unsigned int *primEndAryUI,
   unsigned int *primFlagAryUI,
   signed int numPrimSI,
   signed int fudgeSI, /*Max fudge size*/
   signed char maskSC
);

/*-------------------------------------------------------\
| Fun-11: sortPrimCoords
|  - Sorts the primer coordiantes of a primer flag array
|    setup. Order is least to greates by first starting
|    positon, then ending position
| Input:
|  - startAryUI:
|    o starting coordinates for the primers
|  - endAryUI:
|    o ending coordinates for the primers
|  - flagAryUI:
|    o array full of primFlags with indexs to keep up to
|      date
|  - lenAryUI:
|    o Length of the array
| Output:
|  - Modifies:
|    o startAry array to be sorted form least to greatest
|    o endAry to be kept in order with firstAry
|    o flagAry to be kept in order with firstAry and have
|      any index's updated
\-------------------------------------------------------*/
void
sortPrimCoords(
   unsigned int *startAryUI,
   unsigned int *endAryUI,
   unsigned int *flagAryUI,
   unsigned int lenAryUI 
);

/*-------------------------------------------------------\
| Fun-11: readPrimCoords
|   - gets primer coordinates and flags from an file
| Input:
|   o primFileStr:
|     - c-string with name of file to get primer
|       coordinates from
|       o ignored\tignored\tpair\tstart\tend\tstart\tend
|         - pair: 1, P, T, or + if primers paired
|         - start: start coordinate of foward/reverse
|           primer
|         - end: end coordinate of foward/reverse primer
|   o startAryUI:
|     - pointer to an unsigned int pointer to hold the
|       starting coordinates for each primer
|   o endAryUI:
|     - pointer to an unsigned int pointer to hold the
|       ending coordinates for each primer
|   o flagAryUI:
|     - pointer to an unsigned int pointer to hold the
|       flags and pair index for each primer
|   o errSL:
|     - pointer to unsigned long to hold the error
|       message
| Output:
|   - Modifies:
|     o startAryUI to have the starting coordiantes
|     o endAryUI to have the ending coordiantes
|     o flagAryUI to have the direction and pair flags
|       and the index of the other primer
|     o errSL to hold the error
|       - 0 for no errors
|       - def_memErr_primMask for an memory error
|       - (siLine << 8) | def_fileErr_primMask for file
|         errors
|       - def_emptyFileErr_primMask for empty file
|  - Returns:
|    o 0 for errors
|    o number of primers extracted
\-------------------------------------------------------*/
signed int
readPrimCoords(
   signed char *primFileStr,
   unsigned int **startAryUI,
   unsigned int **endAryUI,
   unsigned int **flagAryUI,
   signed long *errSL
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
