/*########################################################
# Name: ampDepth-fun
#    - Had functions for updating and printing an read
#      depth histogram (array of integers)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Indcluded libraries
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
|   - Indcluded libraries
\-------------------------------------------------------*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "ampDepth-fun.h"

#include <stdio.h>

#include "../generalLib/geneCoordStruct.h"

/*No .c files*/
#include "../generalLib/genMath.h"


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
){
   fprintf((FILE *) outFILE, "flag\tampNumber\trefStart");
   fprintf((FILE *) outFILE, "\trefEnd\tampStart");
   fprintf((FILE *) outFILE, "\tampEnd\tavgAmpDepth");
   fprintf((FILE *) outFILE, "\tminAmpDepth\tmaxAmpDepth");
   fprintf((FILE *) outFILE, "\tgeneId\trefGeneStart");
   fprintf((FILE *) outFILE, "\trefGeneEnd");
   fprintf((FILE *) outFILE, "\tfirstBaseDepth");
   fprintf((FILE *) outFILE, "\tlastBaseDepth");
   fprintf((FILE *) outFILE, "\tavgDepth\tminDepth");
   fprintf((FILE *) outFILE, "\tmaxDepth\n");
} /*pHeaderAmpDepth*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-03: pAmpDepthHistogram
   '    - Prints out an ampDepth histogram to file
   '    o fun-03 sec-01:
   '      - Variable declerations
   '    o fun-03 sec-02:
   '      - Find the mapping positions
   '    o fun-03 sec-03:
   '      - Print out the off targets an unampped reads
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    int mapStartI = 0;
    int tmpStartI = 0;

    int ampNumI = -1;
    int geneIndexI = 0;
    int tmpI = 0;

    int readsAtStartI = 0;
    int readsAtEndI = 0;

    /*Stats for individual genes*/
    int maxReadsI = 0;
    int minReadsI = 0;
    long avgDepthL = 0;

    /*Stats for the amplicon*/
    int ampStartI = 0;
    int ampEndI = 0;
    int ampGeneEndI = 0;
    int ampGeneStartI = 0;

    long ampAvgDepthL = 0;
    int ampMaxReadsI = 0;
    int ampMinReadsI = 0;

    struct geneCoords *genesST =
       (struct geneCoords *) geneCoordsSTPtr;

    FILE *outFILE = (FILE *) outVoidFILE;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-02:
   ^   - Find the mapping positions
   ^   o fun-03 sec-02 sub-01:
   ^     - Check if positon has enough depth to keep
   ^   o fun-03 sec-02 sub-02:
   ^     - Find the amplicon stats
   ^   o fun-03 sec-02 sub-03:
   ^     - Print the amplicon stats
   ^  o fun-03 sec-02 sub-04:
   ^    - Find the stats for each gene in the amplicon
   ^  o fun-03 sec-02 sub-05:
   ^    - Print the stats for each gene in the amplicon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-03 Sec-02 Sub-01:
   *   - Check if positon has enough depth to keep
   \*****************************************************/

   mapStartI = 0;

   while(mapStartI <= genesST->endAryUI[numGenesI] + 1)
   { /*Loop: Get the gene positions that mapped*/
      ++mapStartI;

      if(mapStartI < genesST->startAryUI[0]) continue;

      geneIndexI = 
         findStartCoordInGeneCoord(
            genesST,
            mapStartI,
            numGenesI
         ); /*Find the index of the gene at the position*/

      if(geneIndexI < 0) continue; /*Unmapped base*/
      if(readHistIAry[mapStartI] < minDepthI) continue;

      /**************************************************\
      * Fun-03 Sec-02 Sub-02:
      *   - Find amplicon stats
      \**************************************************/

      ++ampNumI;

      ampGeneEndI = geneIndexI;

      ampStartI = mapStartI;
      ampGeneStartI = genesST->startAryUI[geneIndexI];
      ampAvgDepthL = 0;
      ampMaxReadsI = readHistIAry[mapStartI];
      ampMinReadsI = ampMaxReadsI;

      for(
         ampEndI = mapStartI;
         readHistIAry[ampEndI] >= minDepthI;
         ++ampEndI
      ){ /*Loop: Find end of region*/
          ampGeneEndI +=
            ampEndI > genesST->endAryUI[ampGeneEndI];

          ampMaxReadsI = 
             noBranchMax(
                ampMaxReadsI,
                readHistIAry[ampEndI]
             ); /*Find the maximum*/

          ampMinReadsI = 
             noBranchMin(
                ampMinReadsI,
                readHistIAry[ampEndI]
             ); /*Find the maximum*/
          
          ampAvgDepthL += readHistIAry[ampEndI];
      } /*Loop: Find end of region*/

      --ampEndI; /*Account for overcounting*/
      ampGeneEndI = genesST->endAryUI[ampGeneEndI];

      /*Using interger mean, not decimal*/
      ampAvgDepthL /= (ampEndI - 1 - ampStartI);

      /**************************************************\
      * Fun-03 Sec-02 Sub-03:
      *   - Print the amplicon stats
      \**************************************************/


      /**************************************************\
      * Fun-03 Sec-02 Sub-04:
      *   - Find the stats for each gene in the amplicon
      \**************************************************/

      tmpI = 0;

      nextGene:

      ++tmpI;
      tmpStartI = mapStartI;
      avgDepthL = 0;
      readsAtStartI = readHistIAry[mapStartI];
      maxReadsI = readsAtStartI;
      minReadsI = maxReadsI;

      for(
         mapStartI = mapStartI;
         mapStartI <= genesST->endAryUI[geneIndexI];
         ++mapStartI
      ){ /*Loop: Check if gene is complete*/
          if(readHistIAry[mapStartI] < minDepthI) break;

          maxReadsI = 
             noBranchMax(
                maxReadsI,
                readHistIAry[mapStartI]
             ); /*Find the maximum*/

          minReadsI = 
             noBranchMin(
                minReadsI,
                readHistIAry[mapStartI]
             ); /*Find the maximum*/
          
          avgDepthL += readHistIAry[mapStartI];
      } /*Loop: Check if gene is complete*/

      readsAtEndI = readHistIAry[mapStartI - 1];

      /*Using interger mean, not decimal*/
      avgDepthL /= (mapStartI - 1 - tmpStartI);

      /**************************************************\
      * Fun-03 Sec-02 Sub-05:
      *   - Print the stats for each gene in the amplicon
      \**************************************************/


      fprintf(
         outFILE,
         "%s\t%i\t%i\t%i\t%i\t%i\t%li\t%i\t%i",
         extraColStr,
         ampNumI,
         ampGeneStartI,
         ampGeneEndI,
         ampStartI,
         ampEndI,
         ampAvgDepthL,
         ampMinReadsI,
         ampMaxReadsI
      ); /*Print out the starting and ending position*/

      fprintf(
         outFILE,
         "\t%s\t%i\t%i\t%i\t%i\t%li\t%i\t%i\n",
         genesST->idStrAry[geneIndexI],
         (int) genesST->startAryUI[geneIndexI],
         (int) genesST->endAryUI[geneIndexI],
         readsAtStartI,
         readsAtEndI,
         avgDepthL,
         minReadsI,
         maxReadsI
      );

      if(readHistIAry[mapStartI] > minDepthI)
      { /*If: I have another gene*/
         ++geneIndexI;
         if(geneIndexI > numGenesI) continue;;

         /*Make sure I am on the next gene*/
         for(
            mapStartI = mapStartI;
            mapStartI < genesST->startAryUI[geneIndexI];
            ++mapStartI
         ) if(readHistIAry[mapStartI] < minDepthI) break;

         if(readHistIAry[mapStartI] <minDepthI) continue;
 
         goto nextGene;
      } /*If: I have another gene*/
   } /*Loop: Get the gene positions that mapped*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-03 Sec-05:
   ^  - Print out the off targets an unampped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ++ampNumI;

   fprintf(
      outFILE,
      "%s\t%i\tNA\tNA\tNA\tNA\t%i\t%i\t%i",
      extraColStr,
      ampNumI,
      offTargReadsI,
      offTargReadsI,
      offTargReadsI
   ); /*Print out the off target reads*/

   fprintf(
      outFILE,
      "\tx-off-target\tNA\tNA\t%i\t%i\t%i\t%i\t%i\n",
      offTargReadsI,
      offTargReadsI,
      offTargReadsI,
      offTargReadsI,
      offTargReadsI
   );

   ++ampNumI;

   fprintf(
      outFILE,
      "%s\t%i\tNA\tNA\tNA\tNA\t%i\t%i\t%i",
      extraColStr,
      ampNumI,
      umappedI,
      umappedI,
      umappedI
   ); /*Print out the off target reads*/

   fprintf(
      outFILE,
      "\tz-unmapped\tNA\tNA\t%i\t%i\t%i\t%i\t%i\n",
      umappedI,
      umappedI,
      umappedI,
      umappedI,
      umappedI
   );
} /*pAmpDepthHistogram*/


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
