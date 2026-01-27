/*########################################################
# Name: drugAry
#   - holds fuctions to maintain an array of 32 length
#     c-strings. This is in the from of a matrix so you
#     can free using free(drug_cstring_array);
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and header guard
'   o fun01: alloc_drugAry
'     - makes drug array (c-string) on heap
'   o fun02: realloc_drugAry
'     - returns pointer to rellocated memory for a drugAry
'   o .h fun03: get_drugAry
'     - gets pointer to a drug c-string in a drugAry
'   o fun04: cpDrug_drugAry
'     - copies an antibiotic (as lower case) to drug array
'   o fun05: find_drugAry()
'     - finds antibiotic in a drugAray c-string
'   o fun06: swap_drugAry
'     - swaps two drug names
'   o fun07: cpDrugCase_drugAry
'     - copies an antibiotic to a drug array and keeps the
'       input case (fun04 converts to lower case)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and header guard
\-------------------------------------------------------*/

#ifndef DRUG_ARRAY_H
#define DRUG_ARRAY_H

#define def_strLen_drugAry 32

/*-------------------------------------------------------\
| Fun01: alloc_drugAry
|   - makes drug array (c-string) on heap
| Input:
|   - numStringsUI:
|     o number of strings to make
| Output:
|   - Returns:
|     o an pointer to the new arrray
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
alloc_drugAry(
   unsigned int numStringsUI
);

/*-------------------------------------------------------\
| Fun02: realloc_drugAry
|   - returns pointer to rellocated memory for a drugAry
| Input:
|   - drugAryStr:
|     o pionter to drugAryStr to reallocate memory for
|   - numStringsUI:
|     o number of strings to make
| Output:
|   - Frees:
|     o drugArySTr if memory was succesfully allocated
|       (realloc does this for me)
|   - Returns:
|     o 0 for memory error (old memory freeded)
|     o pointer to new memory (old memory freeded)
\-------------------------------------------------------*/
signed char *
realloc_drugAry(
   signed char *drugAryStr,
   unsigned int numStrUI
);

/*-------------------------------------------------------\
| Fun03: get_drugAry
|   - gets pointer to a drug c-string in a drugAry
| Input:
|   - drugAryStr:
|     o drug array to get the antibiotic name pointer
|   - indexUI:
|     o index to grab
| Output:
|   - Returns:
|     o pointer to index of target drug
\-------------------------------------------------------*/
#define get_drugAry(drugAryStr, indexUI) ( (drugAryStr) + ((indexUI) * def_strLen_drugAry) )

/*-------------------------------------------------------\
| Fun04: cpDrug_drugAry
|   - copies an antibiotic (as lower case) to drug array
| Input:
|   - drugAryStr:
|     o drug array to copy the antibiotic name to
|   - drugStr:
|     o drug to copy
|   - indexUI:
|     o index to copy drug to
|   - delimC:
|     o deliminator to stop copy at
|     o will not copy past '\0'
|     o use -1 to stop at any white space (< 33)
| Output:
|   - Modifies:
|     o dubAryStr to hold the new drug name
|   - Returns:
|     o length of the copied string
\-------------------------------------------------------*/
signed int
cpDrug_drugAry(
   signed char *drugAryStr,
   signed char *drugStr,
   unsigned int indexUI,
   signed char delimSC
);

/*-------------------------------------------------------\
| Fun05: find_drugAry()
|   - finds antibiotic in a drugAray c-string
| Input:
|   - qryStr:
|     o drug to search for in drugAryStr
|   - drugAryStr:
|     o drug array (c-string) to search
|   - numDrugsSI:
|     o number of antibiotics in drugAryStr
| Output:
|   - Returns:
|     o index of the drug if qryStr is in drugAryStr
|     o -1 if the drug is not in drugAry
| Note:
|    - do to the way this is procesed, I can not use a
|      fast search method like a binary search, since
|      (there is no garuntee of order).
\-------------------------------------------------------*/
signed int
find_drugAry(
   signed char *qryStr,     /*drug to find*/
   signed char *drugAryStr, /*array to search*/
   signed int numDrugsSI,   /*number drugs in array*/
   signed char delimSC      /*deliminator to stop at*/
);

/*-------------------------------------------------------\
| Fun06: swap_drugAry
|   - swaps two drug names
| Input:
|   - drugAryStr:
|     o drug array to with drugs to swap
|   - firstUI:
|     o first drug to swap
|   - secUI:
|     o second drug to swap
| Output:
|   - Modifies:
|     o dubAryStr to have drugs swapped
\-------------------------------------------------------*/
void
swap_drugAry(
   signed char *drugAryStr,
   unsigned int firstUI,
   unsigned int secUI
);

/*-------------------------------------------------------\
| Fun07: cpDrugCase_drugAry
|   - copies an antibiotic to a drug array and keeps the
|     input case
| Input:
|   - drugAryStr:
|     o drug array to copy the antibiotic name to
|   - drugStr:
|     o drug to copy
|   - indexUI:
|     o index to copy drug to
|   - delimSC:
|     o deliminator to stop copy at
|     o will not copy past '\0'
| Output:
|   - Modifies:
|     o dubAryStr to hold the new drug name
|   - Returns:
|     o length of the copied string
\-------------------------------------------------------*/
signed int
cpDrugCase_drugAry(
   signed char *drugAryStr,
   signed char *drugStr,
   unsigned int indexUI,
   signed char delimSC
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
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
