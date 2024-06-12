/*########################################################
# Name: tbSpoligo-memwater-fun.c
#   - Holds function for detecting spoligotypes using
#     an memory efficent waterman alignment. Slow, but
#     works.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Guards and definitions
'   o fun01: getSpoligoRefs
'     - Gets the sequence for each spoligotypes
'   o fun02: fxSpoligoSearch
'     - Searches for spoligotypes in fastx sequences using
'       the memory efficent waterman
'   o fun03: samSpoligoSearch
'     - Searchs for spoligotypes in sam files
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Guards and definitions
\-------------------------------------------------------*/

#ifndef TB_SPOLIGOTYPE_MEMWATERMAN_FUN_H
#define TB_SPOLIGOTYPE_MEMWATERMAN_FUN_H

#define def_minPercScore_tbSpoligoWater 0.9f
#define def_matchScore_tbSpoligoWater 5

/*These are a bit wide, but they should work*/
#define def_DRStart_tbSpoligo 3119037
#define def_DREnd_tbSpoligo 3123624

/*-------------------------------------------------------\
| Fun01: getSpoligoRefs
|   - gets the sequence for each spoligotypes
| Input:
|   - spoligoFileStr:
|     o C-string with spoligotype sequences
|   - numSpoligosSIPtr:
|     o Pointer to an int with the number of spoligotype
|       sequences in the returned spoligotype array
|   - errSCPtr:
|     o Pointer to character to hold the error type
| Output:
|   - Modifies:
|     o numSpoligosSI to hold the number of spoligotypes
|       in the returned array
|     o errSCPtr to hold the error type
|       - 0 for no errors
|       - fileErr_tbSpoligo for file errors
|       - memErr_tbSpoligo for memory errors
|   - Returns:
|     o array of seqStructs for success
|     o 0 for an error
\-------------------------------------------------------*/
void *
getSpoligoRefs(
   signed char *spoligoFileStr,
   signed int *numSpoligosSIPtr,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun02: fxSpoligoSearch
|   - Searches for spoligotypes in fastx sequences using
|      the memory efficent waterman
| Input:
|   - fx_seqSTPtr:
|     o Pointer to an seqStruct with the sequence to
|       search (sequence as characters. This function
|       converts)
|   - spoliogAry_SeqSTPtr:
|     o Pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun01: getSpoligoRefs
|       return)
|   - minPercScoreF:
|     o Float with minimum percentage to count an
|       spoligotype
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - pProgressBl:
|     o 1: Print progress reports to the screen (stderr)
|     o 0: Do not print progress reports
|   - fragCheckBl:
|     o 1: do not blank codeAryUI (checking fragments)
|     o 0: blank codeAryUI (expect full length reads)
|   - alnSetSTPtr:
|     o Pointer to an alnSet structer with alignment
|       settings
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Prints:
|     o Status to stderr if user requested
|   - Returns:
|     o 0 for success
|     o noSpoligo_tbSpoligo if no spacers found
|     o memErr_tbSpoligo for memory errors
\-------------------------------------------------------*/
signed char
fxSpoligoSearch(
   void *fx_SeqSTPtr,
   void *spoligoAry_SeqSTPtr,
   signed int numSpoligosSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char pProgressBl,
   signed char fragCheckBl,
   void *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun03: samSpoligoSearch
|   - Searchs for spoligotypes in sam files
| Input:
|   - samSTPtr:
|     o Pointer to an samEntry structure with sequence to
|       search for spoligotypes
|   - spoliogAry_SeqSTPtr:
|     o Pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun01: getSpoligoRefs
|       return)
|   - drStartSI:
|     o Starting position of the direct repeat region on
|       the reference
|   - drEndSI:
|     o Ending position on the direct repeat region on the
|       reference
|   - minPercScoreF:
|     o Float with minimum percentage to count an
|       spoligotype
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o Pointer to an alnSet structer with alignment
|       settings
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Returns:
|     o 0 for success
|     o noSpoligo_tbSpoligo
|     o memErr_tbSpoligo for memory errors
\-------------------------------------------------------*/
signed char
samSpoligoSearch(
   void *samSTPtr,
   void *spoligoAry_SeqSTPtr,
   signed int numSpoligosSI,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   void *alnSetSTPtr
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
