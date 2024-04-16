/*#######################################################\
# Name: checkAmr
#   - Checks if sequences in a sam file map to any amr
#     genes
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Definitions/defaults and header guards
'   o .h st-01: amrHit
'     - Holds a linked list of postive amrs for each amr
'       check
'   o .h fun-01: initAmrHit
'     - Initializes an amrHit structuer to all zeros
'   o fun-02: freeAmrHit
'     - Frees a single amrHit structure. This does not
'       free a list. For lists, use freeAmrHitList; fun-03
'   o fun-03: freeAmrHitList
'     - Frees a list of amrHit structures
'   o fun-04: makeAmr
'     - Makes a new, initialized amrHit structer on heap
'   o .c fun-05: getCigMutCount
'     - Updates the snp (or match)/ins/del counts for a
'   o .c fun-06: incCigCnt
'     - Incurments the cigar counter when all values for
'   o .c fun-07: LoFForwardCheck
'     - Gets the number of indels in an gene and checks
'       for early stops or missing stops and starts.
'   o .c fun-08: LoFReverseCheck
'     - Gets the number of indels in an gene and checks
'       for early stops or missing stops and starts. For
'       an reverse complement gene
'   o fun-09: checkAmrSam
'     - Checks if a sequence in a sam file entry has
'       amr's (antibiotic resitance)
'   o .c fun-10: pCrossRes
'     - Print out cross resitance
'   o fun-11: pHeadAmrHitList
'     - Prints the header for an amrHitList table
'   o fun-12: pAmrHitList
'     - Prints out all amr's that were in a sequence
'   o fun-13: pHeadAmrs
'     - Prints the header for an amr table (reads instead
'       of consensuses)
'   o fun-14: pAmrs
'     - Prints out all amr's that meant the min depth
'   o fun-15: pHeadAmrReadIds
'     - Prints out the header for the read id mapped
'       variant file
'   o fun-16: pAmrReadIds
'     - Prints out the read id and the ARMs variant ids
'       that it supported
'   o fun-17: lookForAmrsSam
'     - Look for anti-microbial (antibiotic) genes in the
'       reads in a sam file
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Definitions/defaults and header guards
\-------------------------------------------------------*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

#define def_checkFrameshift_checkAmr 0
  /*1: check frameshifts*/

#define defMinDepth 20

#define defMinPerReadsMap 0.05f
    /*At least 5% of reads with region support the amr*/
#define defMinPerReadsTotal 0.00f
    /*0% (turn off) of all kept reads mapped to this amr
    `   region
    */
/*-------------------------------------------------------\
| ST-01: amrHit
|   - Holds a linked list of postive amrs for each amr
|     check
\-------------------------------------------------------*/
typedef struct amrHit{
   struct amrStruct *amrST;
   unsigned seqPosUI;
   struct amrHit *nextAmr;
}amrHit;

/*-------------------------------------------------------\
| Fun-01: initAmrHit
|   - Initializes an amrHit structuer to all zeros
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
#define \
initAmrHit(amrHitSTPtr){\
   (amrHitSTPtr)->amrST = 0;\
   (amrHitSTPtr)->seqPosUI = 0;\
   (amrHitSTPtr)->nextAmr = 0;\
} /*initAmrHit*/

/*-------------------------------------------------------\
| Fun-02: freeAmrHit
|   - Frees a single amrHit structure. This does not free
|     a list. For lists, use freeAmrHitList (fun-03)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
\-------------------------------------------------------*/
void
freeAmrHit(
   struct amrHit *amrHitSTPtr
);

/*-------------------------------------------------------\
| Fun-03: freeAmrHitList
|   - Frees a list of amrHit structures
| Input:
|   - amrHitSTListPtr:
|     o pointer to start of an amrHist structure list to
|       free
| Output:
|   - Frees:
|     o All amrHits structures in amrHitSTPtr
\-------------------------------------------------------*/
void
freeAmrHitList(
   struct amrHit *amrHitSTListPtr
);

/*-------------------------------------------------------\
| Fun-04: makeAmr
|   - Makes a new, initialized amrHit structer on heap
| Input:
| Output:
|   - Returns:
|     o A pointer to the new amrHit structure
|     o 0 for memory error
\-------------------------------------------------------*/
struct amrHit *
makeAmrHit();

/*-------------------------------------------------------\
| Fun-09: checkAmrSam
|   - Checks if a sequence in a sam file entry has
|     amr's (antibiotic resitance)
| Input:
|   - samSTPtr:
|     o Pointer to samEntry struct with sequence to check
|       for amr's
|   - amrSTAryPtr:
|     o Pointer to an array of amrStruct structures with
|       amr's to check for.
|   - numArmI:
|     o Number of amrStruct structures in amrAryST
|   - numHitsI:
|     o Updated to hold the number of amr's found in
|       the sequence stored in samST
|   - frameshiftBl:
|     o 1: Check for LoFs in frameshift
|     o 0: Treat frameshifts as exact matches
|   - errC:
|     o Pointer to character to hold the error output
| Output:
|   - Modifies:
|     o numHitsI to hold the number of amr's found in the
|       sequence in samST
|     o errC:
|       - 0 for No errors
|       - 64 for memory errors
|   - Returns:
|     o List of amrHit structures with the detected amr's
|     o 0 for memory errors or when no amr's were found
\-------------------------------------------------------*/
struct amrHit *
checkAmrSam(
   void *samSTPtr,     /*Sequence to check*/
   void *amrSTAryPtr,  /*Has amr's to check*/
   int numAmrI,        /*Length of amrAryST*/
   int *numHitsI,      /*Number amr hits for seq*/
   char frameshiftBl,  /*1: Handle frameshifts*/
   char *errC          /*For error reporting*/
);

/*-------------------------------------------------------\
| Fun-11: pHeadAmrHitList
|   - Prints the header for an amrHitList table
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrHitList table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrHitList(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-12: pAmrHitList
|   - Prints out all amr's that were in a sequence
| Input:
|   - seqIdStr:
|     o C-string with name of sequence that had the amr
|   - amrHitSTListPtr:
|     o Pointer to a List of amrHit structures with amr's
|       to print out
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's in amrHitSTListPtr to outFILE
|     o The header if pHeadBl is 1
\-------------------------------------------------------*/
void
pAmrHitList(
   char *seqIdStr,
   struct amrHit *amrHitSTListPtr,
   char *drugAryStr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-13: pHeadAmrs
|   - Prints the header for an amr table (reads instead
|     of consensuses)
| Input:
|   - outFILE:
|     o FILE pointer to file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pHeadAmrs(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-14: pAmrs
|   - Prints out all amr's that meant the min depth
| Input:
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally)
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|   - totalReadsUI:
|     o total number of reads input
|   - amrSTAry:
|     o Pointer to an array of amrStruct structures to
|       check and print
|   - drugAryStr:
|     o List of antibiotic drugs. It should follow the
|       same order as the flags
|   - outFILE:
|     o File to print the amr's to
| Output: 
|   - Prints:
|     o The amr's that meet the min stats and have at
|       least one mapped read
|     o The header if pHeadBl is 1
\-------------------------------------------------------*/
void
pAmrs(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   unsigned int totalReadsUI,
   struct amrStruct *amrSTAry,
   unsigned int numAmrsUI,
   char *drugAryStr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-15: pHeadAmrReadIds
|   - Prints out the header for the read id mapped variant
|     file
| Input:
|   - outFILE:
|     o Pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o The header for the read id mapped variant table
\-------------------------------------------------------*/
void
pHeadAmrReadIds(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-16: pAmrReadIds
|   - Prints out the read id and the ARMs variant ids that
|     it supported
| Input:
|   - idStr:
|     o C-string with read id to print out
|   - amrHitSTListPtr:
|     o Pointer to an list of amrHit structures having the
|       AMRs that the read supported
|   - outFILE:
|     o Pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o The read id and any AMRs that it supported (tsv)
\-------------------------------------------------------*/
void
pAmrReadIds(
   char *idStr,
   struct amrHit *amrHitSTListPtr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-17: lookForAmrsSam
|   - Look for anti-microbial (antibiotic) genes in the
|     reads in a sam file
| Input:
|   - amrSTAryPtr
|     o Pointer to an array of amrStructs with AMRs to
|       check
|   - numAmrI:
|     o Number of AMRs in amrSTAryPtr
|   - drugAryStr:
|     o C-string array (see drug_str_ary.c/h) with
|       antibiotic names
|   - readsBl:
|     o 1: Printing out read stats (use pAmr)
|     o 0: Printing out consensus stats (use pAmrHitList)
|   - framshiftBl:
|     o 1: Check for framshifts if LoF and frameshift AMRs
|     o 0: Treat all frameshifts and LoFs as exact matches
|   - minDepthUI:
|     o uinsigned in with  he minumum depth to keep an amr
|     o This is applied to read checks only
|   - minPercMapF:
|     o Float with the min percent of supporting reads
|       to keep an amr (only compared locally) 
|     o This is applied to read checks only
|   - minPercTotalF:
|     o Float with the min percent of mapped reads needed
|       to keep an amr (all reads kept)
|     o This is applied to read checks only
|   - samFileStr:
|     o C-string with sam file to check for AMRs
|   - outFileStr:
|     o C-string with name to print AMR hits to
|   - idFileStr:
|     o C-sring with name of file to print read ids to
| Output:
|   - Prints:
|     o Stats about AMRs to outFILE
|     o Read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o 2 for file open errors
|     o 64 for memory errors
\-------------------------------------------------------*/
char
lookForAmrsSam(
   void *amrSTAryPtr,     /*Has amr's to check*/
   int numAmrI,           /*Length of amrAryST*/
   char *drugAryStr,      /*Has antibiotic names*/
   char readsBl,          /*1: processing reads not cons*/
   char frameshiftBl,     /*1: check frameshitfs AMRs*/
   unsigned int minDepthUI,/*Min depth to keep amr*/
   float minPercMapF,     /*% support to keep amr; read*/
   float minPercTotalF,   /*% mapped reads to keep read*/
   char *samFileStr,      /*Sam file with reads to check*/
   char *outFileStr,      /*File to output results to*/
   char *idFileStr        /*File to print read ids to*/
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
