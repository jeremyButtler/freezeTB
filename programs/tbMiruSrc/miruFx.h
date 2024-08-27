/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' miruFx SOF: Start Of File
'   - has functios to use kmerFind to identify MIRU
'     lineages in reads
'   o header:
'     - default settings, forward declartions and guards
'   o .c fun01: swap_refST_kmerFind_miruFx
'     - swaps two refST_kmerFind structures around
'   o fun02: readTbl_miruFx
'     - reads in a miru table for fastq processing
'   o fun03: kmerFind_miruFx
'     - scans an seqST structure for MIRU-VNTR lineages
'       using kmerFind
'   o license:
'     - licensing for this code (public domain / mit)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - default settings, forward declartions and guards
\-------------------------------------------------------*/

#ifndef MIRU_VNTR_FASTA_FASTQ_SCAN_H
#define MIRU_VNTR_FASTA_FASTQ_SCAN_H

#define def_minPercScore_miruFx 0.9f
#define def_maxAmpLen_miruFx 5000

typedef struct seqST seqST;
typedef struct miruTbl miruTbl;
typedef struct refST_kmerFind refST_kmerFind;
typedef struct tblST_kmerFind tblST_kmerFind;

/*-------------------------------------------------------\
| Fun02: readTbl_miruFx
|   - reads in a miru table for fastq processing
| Input:
|   - miruTblStr:
|     o c-string with path to miru table to use
|   - primStr:
|     o path to primers to align
|   - lenKmerUC:
|     o length of one kmer
|   - refAryST:
|     o pointer to pointer to refST_kmerFind structure
|       to hold reference sequences (should be null)
|   - lenRefUI:
|     o pinter to signed int to hold number primers in
|       refAryST
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to link
|       with this reference set
|   - minPercKmersF:
|     o float with minimum percentage of kmers to start
|       considering an window supports an spacer
|   - percExtraNtInWinF:
|     o float with percentage of extra nucleotides to
|       store in one window (beyond reference length)
|   - percWinShiftF:
|     o float with percentage of bases to shift for each
|       new window in tblSTPtr
|   - alnSetPtr:
|     o pointer to an alnSet structure with the alignment
|       settings (for finding max possible score)
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o refAryST: to be an array of primer sequences
|     o lenRefSI: to have number of primers read in
|     o tblSTPtr: to be setup for refAryST
|     o errSCPtr:
|       - 0 for no errors
|       - def_fileErr_tbMiruDefs for file error
|       - def_memErr_tbMiruDefs for memory error
|   - Returns:
|     o pointer to a set up miruTbl structure
\-------------------------------------------------------*/
struct miruTbl *
kmerFindSetup_miruTbl(
   signed char *miruTblStr,  /*tsv with path to table*/
   signed char *primTsvStr,  /*tsv with primers*/
   unsigned int lenKmerUC,   /*length of one kmer*/
   struct refST_kmerFind **refAryST,
   signed int *lenRefSI,     /*number of primers*/
   struct tblST_kmerFind *tblSTPtr,
   float minPercKmersF,
   float percExtraNtInWinF,
   float percWinShiftF,
   struct alnSet *alnSetPtr,
   signed char *errSCPtr     /*holds error message*/
);

/*-------------------------------------------------------\
| Fun03: kmerFind_miruFx
|   - scans an seqST structure for MIRU-VNTR lineages
|     using kmerFind
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings
|   - refAryST
|     o array of refST_kmerFind structures with reference
|       (primer) sequences to search for
|     o needs to be same length as
|       miruTblSTPtr->numPrimSI * 2
|   - fudgeSI:
|     o offset length by when comparing to lingeage length
|   - tblSTPtr:
|     o pointer to tblST_kmerFind structure with MIRU-VNTR
|       primers to scan for and settings
|   - seqSTPtr:
|     o pointer to seqST struct with sequence to scan
|   - minPercScoreF:
|     o minimum percent score to keep an alignment
|   - maxAmpLenSI:
|     o maximum amplicon length before discarding
|       a match
|   - alnSetPtr:
|     o pointer to alnSet sturct with alignment settings
| Output:
|   - Modifies:
|     o miruTblST to have detected lineages incurmented
|   - Returns:
|     o 0 if found lineage
|     o 1 if could not find lineage
|     o 2 for an error
\-------------------------------------------------------*/
signed char
kmerFind_miruFx(
   struct miruTbl *miruTblSTPtr, /*MIRU lineage table*/
   struct tblST_kmerFind *tblSTPtr,/*settings*/
   struct refST_kmerFind *refAryST,/*primer sequences*/
   signed int fudgeSI,           /*lineage match range*/
   struct seqST *seqSTPtr,       /*query sequence*/
   float minPercScoreF,           /*min score to keep map*/
   signed int maxAmpLenSI,       /*max amplicon length*/
   struct alnSet *alnSetPtr      /*alignment settings*/
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
