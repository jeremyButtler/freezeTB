/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' checkAmr SOF: Start Of File
'   - has functions to detect AMRs in sam file entrie
'   o header:
'     - definitions, forward declerations, and guards
'   o .h st01: amrHit_checkAmr
'     - linked list of postive amrs for each amr check
'   o fun01: init_amrHit_checkAmr
'     - initializes amrHit_checkAmr struct (all zeros)
'   o fun02: freeHeap_amrHit_checkAmr
'     - frees a amrHit_checkAmr structure
'   o fun03: freeHeapList_amrHit_checkAmr
'     - frees list of amrHit_checkAmr structures
'   o fun04: mk_marHit_checkAmr
'     - makes heap initialized amrHit_checkAmr structer
'   o fun05: LoFFor_checkAmr
'     - gets number of indels in gene and checks for early
'       stops or missing stops and starts.
'   o fun06: LofRev_checkAmr
'     - gets number of indels and checks for early stops
'       or missing stops/starts in reverse complement gene
'   o fun07: checkAmr
'     - checks if sam entry has amr's
'   o .c fun08: pCrossRes_checkAmr
'     - print out cross resitance (report, not database)
'   o fun09: cmpAmrs_checkAmr
'     - checks if two AMR structs are the same variant
'   o fun10: flagAmrHit_checkAmr
'     - flag all duplicates in a AMR hit list
'   o fun11: pConHead_checkAmr
'     - prints header for a consensus amr check
'   o fun12: pCon_checkAmr
'     - prints out all amr's that were in a consensus
'   o fun13: readGetVarHits_checkAmr
'     - finds non-duplicate AMRs for the target variant ID
'   o fun14: pReadHead_checkAmr
'     - prints header for reads AMR table
'   o fun15: pRead_checkAmr
'     - prints AMRs detected in reads that have min depth
'   o fun16: pIdVarHead_checkAmr
'     - prints header for the read id mapped variant table
'   o fun17: pIdVarTbl_checkAmr
'     - prints table of read ids and detected AMRs
'   o fun18: samo findAmrs_checkAmr
'     - look for AMRs in sam file entries
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - definitions, forward declerations, and guards
\-------------------------------------------------------*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

struct samEntry;
struct refList_amrST;
struct amrST;

/*-------------------------------------------------------\
| ST01: amrHit_checkAmr
|   - linked list of postive amrs for each amr check
\-------------------------------------------------------*/
typedef struct amrHit_checkAmr{
   struct amrST *amrSTPtr;
   unsigned int seqPosUI;
   struct amrHit_checkAmr *nextAmr;
   signed char dupBl; /*marks if duplicate variant*/
}amrHit_checkAmr;

/*-------------------------------------------------------\
| Fun01: init_amrHit_checkAmr
|   - initializes amrHit_checkAmr struct (all zeros)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
void
init_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
);

/*-------------------------------------------------------\
| Fun02: freeHeap_amrHit_checkAmr
|   - frees a amrHit_checkAmr structure
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeap_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeHeapList_amrHit_checkAmr
|   - frees list of amrHit_checkAmr structures
| Input:
|   - amrHitListST:
|     o pointer to start of an amrHist struct list to free
| Output:
|   - Frees:
|     o all amrHit_checkAmrs structures in amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeapList_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
);

/*-------------------------------------------------------\
| Fun04: mk_amrHit_checkAmr
|   - makes heap initialized amrHit_checkAmr structer
| Input:
| Output:
|   - Returns:
|     o pointer to the new amrHit_checkAmr structure
|     o 0 for memory error
\-------------------------------------------------------*/
struct amrHit_checkAmr *
mk_amrHit_checkAmr(
   void
);

/*-------------------------------------------------------\
| Fun05: LoFFor_checkAmr
|   - gets number of indels in gene and checks for early
|     stops or missing stops and starts.
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       having the gene to check
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o delCntSI to hold the number of deletions
|     o insCntSI to hold the number of insertions
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LoFFor_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry *samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
);

/*-------------------------------------------------------\
| Fun06: LofRev_checkAmr
|   - gets number of indels and checks for early stops or
|     missing stops and starts in reverse complement gene
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with sequence
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o indelCntSI to hold the number of indels
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LofRev_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry *samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
);

/*-------------------------------------------------------\
| Fun07: checkAmr
|   - checks if sam entry has amr's
| Input:
|   - samSTPTr:
|     o pointer to samEntry struct with sequence to check
|   - amrSTAryPtr:
|     o pointer to an amrST struct array with amr's
|   - numAmrSI:
|     o number of amrST structures in amrAryST
|   - numHitsSI:
|     o pointer to signed int to hold number detected amrs
|   - frameshiftBl:
|     o 1: check for LoFs in frameshift
|     o 0: treat frameshifts as exact matches
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - errSC:
|     o pointer to signed char to hold the error output
| Output:
|   - Modifies:
|     o numHitsSI to have number of amrs found in samSTPTr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o list of amrHit_checkAmr structs with detected amrs
|     o 0 for no amrs or memory errors
\-------------------------------------------------------*/
struct amrHit_checkAmr *
checkAmr(
   struct samEntry *samSTPTr, /*sequence to check*/
   struct amrST *amrAryST,    /*AMRs to search*/
   signed int numAmrSI,       /*length of amrAryST*/
   signed int *numHitsSI,     /*holds number amr hits*/
   signed char frameshiftBl,  /*1: frameshift/indel mode*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   signed char *errSC         /*for error reporting*/
);

/*-------------------------------------------------------\
| Fun09: cmpAmrs_checkAmr
|   - checks if two AMR structs are the same variant
| Input:
|   - oneST:
|     o pointer to first amrST structures to compare
|   - twoST:
|     o pointer to second amrST structures to compare
| Output:
|   - Returns:
|     o -1 if are different by variant ID
|     o -2 if same variant ID, but different attribute
|     o 0 if are the same to checkAmr
|     o 1 if first AMR has more support
|     o 2 if second AMR has more support
\-------------------------------------------------------*/
signed char
cmpAmrs_checkAmr(
   struct amrST *oneST,
   struct amrST *twoST
);

/*-------------------------------------------------------\
| Fun10: flagAmrHit_checkAmr
|   - flag all duplicates in a AMR hit list
| Input:
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs to
|       flag duplicate AMRs
| Output:
|    - Modifies:
|      o dupBl in each amrHit_checkAmr struct in
|        amrHitListST to be 1 (if duplicates) or 0 if
|        unique/best AMR
\-------------------------------------------------------*/
void
flagAmrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
);

/*-------------------------------------------------------\
| Fun11: pConHead_checkAmr
|   - prints header for a consensus amr check
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrHit_checkAmrList table header to outFILE
\-------------------------------------------------------*/
void
pConHead_checkAmr(
   signed char refBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: pCon_checkAmr
|   - prints out all amr's that were in a consensus
| Input:
|   - refStr:
|     o c-string with the reference id
|     o or 0/null if no reference column
|   - seqIdStr:
|     o c-string with name of sequence
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs with
|       detected AMRs
|   - refSTPtr:
|     o refList_amrST struct pointer with drugs used in
|       the database
|   - outFILE:
|     o file to print amr's to
| Output: 
|   - Prints:
|     o amr's in amrHitListST to outFILE
|   - Modifies:
|     o dupBl in amrHitListST to be 1 for duplicate
|       variant ids
\-------------------------------------------------------*/
void
pCon_checkAmr(
   signed char *refStr, /*reference id to print*/
   signed char *seqIdStr,
   struct amrHit_checkAmr *amrHitListST,
   struct refList_amrST *refSTPtr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun13: readGetVarHits_checkAmr
|   - finds non-duplicate AMRs for the target variant ID
| Input:
|   - amrAryST:
|     o pointer to an array of amrST structures to print
|   - indexSI:
|     o index of AMR in amrAryST
|   - numAmrsUI:
|     o number AMRs in amrAryST
| Output:
|   - Returns:
|     o 0 for memory errors
|     o signed int array ending in -1 with the index of
|       each different AMR
|       * the index after -1 has the next variant ID to
|         scan
\-------------------------------------------------------*/
signed int *
readGetVarHits_checkAmr(
   struct amrST *amrAryST,
   signed int indexSI,      /*index at in AMR array*/
   unsigned int numAmrsUI
);

/*-------------------------------------------------------\
| Fun14: pReadHead_checkAmr
|   - prints header for reads AMR table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pReadHead_checkAmr(
   signed char refBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun15: pRead_checkAmr
|   - prints AMRs detected in reads that have min depth
| Input:
|   - minDepthUI:
|     o minumum depth to keep an amr
|   - minPercMapF:
|     o mininmum percent of supporting reads to keep an
|       amr (only compared locally)
|   - minPercTotalF:
|     o min percent of mapped reads needed to keep an amr
|       (compared to all reads [total depth])
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - framShiftBl:
|     o 1: looked for frameshifts in data
|   - totalReadsUI:
|     o total number of reads input
|   - refSTPtr:
|     o pointer to a refList_amrST struct with amrs to
|       check if detected
|   - outFILE:
|     o file to print the amr's to
| Output: 
|   - Modifies:
|     o amrAryST to be sorted by position
|   - Prints:
|     o AMRs with >= min stats to outFILE
\-------------------------------------------------------*/
void
pRead_checkAmr(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   float minIndelSupF,      /*% support to keep indel*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char frameShiftBl,/*looked for frameshifts*/
   unsigned int totalReadsUI,
   struct refList_amrST *refSTPtr, /*has amrs to check*/
   void *outFILE
);

/*-------------------------------------------------------\
| Fun16: pIdVarHead_checkAmr
|   - prints header for the read id mapped variant table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o header for read id mapped variant table to outFILE
\-------------------------------------------------------*/
void
pIdVarHead_checkAmr(
   signed char pRefBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun17: pIdVarTbl_checkAmr
|   - prints table of read ids and detected AMRs
| Input:
|   - refStr:
|     o c-string with reference used
|     o use 0/null to not print the reference column
|   - idStr:
|     o c-string with read id to print out
|   - amrHitListST:
|     o pointer to an list of amrHit_checkAmr structures
|       having AMRs detected in the read
|   - outFILE:
|     o pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o read id and detected AMRs to outFILE
\-------------------------------------------------------*/
void
pIdVarTbl_checkAmr(
   signed char *refStr,
   signed char *idStr,
   struct amrHit_checkAmr *amrHitListST,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun18: samFindAmrs_checkAmr
|   - look for AMRs in sam file entries
| Input:
|   - refSTPtr:
|     o pointer to refList_amrST struct with the amrs for
|       each reference
|   - readsBl:
|     o 1: print read AMRs (pRead_checkAmry [fun15])
|     o 0: print consensus AMRs (pCon_checkAmr [fun13])
|   - framshiftBl:
|     o 1: check for framshifts (LoF/frameshift AMRs)
|     o 0: ingore frameshifts (are exact matches)
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - minDepthUI:
|     o minumum depth to keep an amr (reads only)
|   - minPercMapF:
|     o mininimum percent of supporting reads to keep an
|       amr (reads only) 
|   - minPercTotalF:
|     o mininimum percent of mapped reads needed to keep
|       an amr (all possible mapped reads; reads only)
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - samFileStr:
|     o c-string with sam file to check for AMRs
|   - outFileStr:
|     o c-string with name to print AMR hits to
|   - idFileStr:
|     o c-sring with name of file to print read ids to
| Output:
|   - Prints:
|     o stats about AMRs to outFILE
|     o read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o def_fileErr_amrST for file open errors
|     o def_memErr_amrST for memory errors
\-------------------------------------------------------*/
signed char
samFindAmrs_checkAmr(
   struct refList_amrST *refSTPtr,/*has amrs and ref ids*/
   signed char readsBl,     /*1: checking reads not cons*/
   signed char frameshiftBl,/*1: check frameshifts*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   unsigned int minDepthUI, /*min depth to keep amr*/
   float minPercMapF,       /*% support to keep amr*/
   float minPercTotalF,     /*% mapped reads to keep amr*/
   float minIndelSupF,      /*%support to keep indel amr*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char *samFileStr, /*sam file to check*/
   signed char *outFileStr, /*output file (main)*/
   signed char *idFileStr   /*output file (ids)*/
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
