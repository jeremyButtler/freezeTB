/*########################################################
# Name: kmerFind
#   - holds functions for spoligotype detection
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - forward declerations and guards
'   o fun01: fx_spolFind
'     - finds spoligotype spacers in an sequence (from
'       fastx file) using an faster kmer search followed
'       by an slower waterman to finalize alignments
'   o fun02: sam_spolFind
'     - finds spoligotype spacers in an sequence using
'       an faster kmer search followed by an slower
'       waterman to finalize alignments
'   o fun03: getRefs_spolFind
'     - gets the sequence for each spoligotypes (for
'       waterman find only (fun04/fun05)
'   o fun04: fxwater_spolFind
'     - searches for spoligotypes in fastx sequences using
'       a waterman
'   o fun05: samWater_spolFind
'     - searches for spoligotypes in sam files using a
'       waterman alignment (slow)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declerations and guards
\-------------------------------------------------------*/

#ifndef SPOLIGOTYPE_FIND_H
#define SPOLIGOTYPE_FIND_H

typedef struct samEntry samEntry;
typedef struct seqST seqST;
typedef struct alnSet alnSet;

typedef struct tblST_kmerFind tblST_kmerFind;
typedef struct refST_kmerFind refST_kmerFind;

/*-------------------------------------------------------\
| Fun01: fx_spolFind
|   - finds spoligotype spacers in an sequence (from
|     fastx file) using an faster kmer search followed by
|     an slower waterman to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       alnSetSTPtr and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - seqSTPtr:
|     o pointer to an seqST structure with the
|       sequence to check for spoligotypes in
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - def_noSpol_tbSpolDefs if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o def_noSpol_tbSpolDefs if no spoligotypes were found
\-------------------------------------------------------*/
signed char
fx_spolFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct seqST *seqSTPtr,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun02: sam_spolFind
|   - finds spoligotype spacers in an sequence using
|     an faster kmer search followed by an slower waterman
|     to finalize alignments
| Input:
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       alnSetSTPtr and to hold the query sequence to check
|   - lenRefAryUI:
|     o number of refST_kmerFind structures in refSTAry
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       to check for spoligotypes in
|   - dirStartSI:
|     o position of first base in the direct repeat region
|   - dirEndSI:
|     o position of last base in the direct repeat region
|   - minPerScoreF:
|     o float with minimum percent score to keep an
|       alingment
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - fragCheckBl:
|     o 1: check direct repeat fragments and do not blank
|          codeAryUI
|     o 0: only check full direct repeat regions and blank
|          codeAryUI
|   - alnSetSTPtr:
|     o pointer to an alnSet structure with the alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o each position codeStr
|       - 1 if an spoligotype
|       - def_noSpol_tbSpolDefs if no spoligotype was found
|   - Returns:
|     o 0 if the reference sequence was found in the kmer
|       table (query) sequence
|     o def_noSpol_tbSpolDefs if no spoligotypes were found
\-------------------------------------------------------*/
signed char
sam_spolFind(
   struct tblST_kmerFind *tblSTPtr,
   struct refST_kmerFind *refSTAry,
   unsigned int lenRefAryUI,
   struct samEntry *samSTPtr,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun03: getRefs_spolFind
|   - gets the sequence for each spoligotypes; for
|     waterman find only (fun04/fun05)
| Input:
|   - spoligoFileStr:
|     o c-string with spoligotype sequences
|   - numSpoligosSIPtr:
|     o pointer to an int with the number of spoligotype
|       sequences in the returned spoligotype array
|   - errSCPtr:
|     o pointer to character to hold the error type
| Output:
|   - Modifies:
|     o numSpoligosSI to hold the number of spoligotypes
|       in the returned array
|     o errSCPtr to hold the error type
|       - 0 for no errors
|       - def_fileErr_tbSpolDefs for file errors
|       - def_memErr_tbSpolDefs for memory errors
|   - Returns:
|     o array of seqStructs for success
|     o 0 for an error
\-------------------------------------------------------*/
struct seqST *
getRefs_spolFind(
   signed char *spoligoFileStr,
   signed int *numSpoligosSIPtr,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun04: fxwater_spolFind
|   - searches for spoligotypes in fastx sequences using
|     a waterman
| Input:
|   - fx_seqSTPtr:
|     o pointer to an seqStruct with the sequence to
|       search (sequence as characters. This function
|       converts)
|   - spoliogAry_SeqSTPtr:
|     o pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun03: getRefs_spolFind
|       return)
|   - minPercScoreF:
|     o minimum percentage to count a spoligotype
|   - codeAryUI:
|     o unsigned integer array to hold the number of times
|       each spacer was detected
|   - pProgressBl:
|     o 1: print progress reports to the screen (stderr)
|     o 0: do not print progress reports
|   - fragCheckBl:
|     o 1: do not blank codeAryUI (checking fragments)
|     o 0: blank codeAryUI (expect full length reads)
|   - alnSetSTPtr:
|     o pointer to an alnSet structer with alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Prints:
|     o Status to stderr if user requested
|   - Returns:
|     o 0 for success
|     o def_noSpol_tbSpolDefs if no spacers found
|     o def_memErr_tbSpolDefs for memory errors
\-------------------------------------------------------*/
signed char
fxwater_spolFind(
   struct seqST *refAryST,
   struct seqST *spoligoAryST,
   signed int numSpoligosSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char pProgressBl,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun05: samWater_spolFind
|   - searches for spoligotypes in sam files using a
|     waterman alignment (slow)
| Input:
|   - samSTPtr:
|     o pointer to an samEntry structure with sequence to
|       search for spoligotypes
|   - spoliogAry_SeqSTPtr:
|     o pointer to an seqStruct array with the converted
|       spoloigotype sequences (fun03: getRefs_spolFind
|       return)
|   - drStartSI:
|     o starting position of the direct repeat region on
|       the reference
|   - drEndSI:
|     o ending position on the direct repeat region on the
|       reference
|   - minPercScoreF:
|     o float with minimum percentage to count an
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
|     o pointer to an alnSet structer with alignment
|       alnSetSTPtr
| Output:
|   - Modifies:
|     o codeStr to hold the spoligotype barcode
|   - Returns:
|     o 0 for success
|     o def_noSpol_tbSpolDefs
|     o def_memErr_tbSpolDefs for memory errors
\-------------------------------------------------------*/
signed char
samWater_spolFind(
   struct samEntry *samSTPtr,
   struct seqST *spoligoAryST,
   signed int numSpoligosSI,
   signed int dirStartSI,
   signed int dirEndSI,
   float minPercScoreF,
   unsigned int *codeAryUI,
   signed char fragCheckBl,
   struct alnSet *alnSetSTPtr
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
