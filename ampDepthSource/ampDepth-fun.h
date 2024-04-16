/*########################################################
# Name: ampDepth-fun
#    - Had functions for updating and printing an read
#      depth histogram (array of integers)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - header gaurds
'   o fun-01: addBaseToAMpDepth (.h only)
'     - Adds an base to an amp depth arrary (histogram)
'   o fun-02: pHeaderAmpDepth
'     - Prints the header for an ampDepth tsv file
'   o fun-03: pAmpDepthHistogram
'     - Prints an histogram of read depths as an tsv with
'       individual genes for ampDepth
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Header garuds
\-------------------------------------------------------*/

#ifndef AMPLICON_DEPTH_FUNCTIONS_H
#define AMPLICON_DEPTH_FUNCTIONS_H

/*-------------------------------------------------------\
| Fun-01: addBaseToAMpDepth
|   - Adds an base to an amp depth arrary (histogram)
| Input:
|   - samSTPtr:
|     o Pointer to an samEntry structure with a read to
|       add to the histogram
|   - geneCoordSTPtr:
|     o Pointer to an array of geneCoord structures with
|       gene coordinates for each gene of interest
|   - numCoordsI:
|     o number of geneCoord structures in geneCoordSTPtr
|   - ampDepthIAry:
|     o Histogram (int array) to add each base to
|   - offTargReadsI:
|     o Number of reads not mapping to a gene coordiante,
|       but are mapped to the genome
| Output:
|   - Modifies:
|     o ampDepthIAry to be incurmented for each reference
|       base in the read
\-------------------------------------------------------*/
#define \
addBaseToAmpDepth(\
   samSTPtr,\
   geneCoordSTPtr,\
   numCoordsI,\
   ampDepthIAry,\
   offTargReadsI\
){\
   int iBaseMac = 0;\
   \
   for(\
      iBaseMac = (samSTPtr)->refStartUI;\
      iBaseMac < (samSTPtr)->refEndUI;\
      ++iBaseMac\
   ){ /*Loop: Fill in bases*/\
      if(  iBaseMac\
         > (geneCoordSTPtr)->endAryUI[(numCoordsI)]\
      ) { /*If: the read has an offtarget section*/\
        ++(offTargReadsI);\
        break;\
      } /*If: the read has an offtarget section*/\
      \
      ++(ampDepthIAry)[iBaseMac];\
   } /*Loop: Fill in bases*/\
} /*addBaseToAmDepth*/

/*-------------------------------------------------------\
| Fun-02: pHeaderAmpDepth
|   - Prints the header for an ampDepth tsv file
| Input:
|   - outFILE:
|     o File to print the header to
| Output:
|   - Prints:
|     o The ampDepth header to outFILE
\-------------------------------------------------------*/
void
pHeaderAmpDepth(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun-03: pAmpDepthHistogram
|   - Prints an histogram of read depths as an tsv with
|     individual genes for ampDepth
| Input:
|   - readHistIAry:
|     o Integer array with the histogram to print out
|   - minDepthI:
|     o integer with the min depth to keep an histogram
|       entry
|   - geneCoordsSTPtr:
|     o Pointer to an geneCoord structure array with the
|       locations of each gene
|   - numGenesI:
|     o Integer with number of genes in genesST
|   - offTargReadsI:
|     o Number off of target reads to print out
|   - umappedI:
|     o Number off of unmapped reads to print out
|   - extraColStr:
|     o C-string with the first column to print out
|   - outVoidFILE:
|     o FILE pointer with output file to print to
| Output:
|   - Prints:
|     o The stats for each gene to outVoidFILE
\-------------------------------------------------------*/
void
pAmpDepthHistogram(
   int *readHistIAry,
   int minDepthI,
   void *geneCoordsSTPtr,
   int numGenesI,
   int offTargReadsI,
   int umappedI,
   char *extraColStr,
   void *outVoidFILE
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
