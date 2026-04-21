/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' ampDepth SOF: Start Of File
'   - Had functions for updating and printing an read
'     depth histogram (array of integers)
'   o header:
'     - indcluded libraries
'   o fun01: addRead_ampDepth
'     - adds a read to an amp depth arrary (histogram)
'   o fun02: phead_ampDepth
'     - Prints the header for an ampDepth tsv file
'   o fun03: pDepthHead_ampDepth
'     - prints the header for the read depth output
'   o fun04: phist_ampDepth
'     - Prints an histogram of read depths as an tsv with
'       individual genes for ampDepth
'   o fun05: pDepthHead_ampDepth
'     - prints the header for the read depth output
'   o fun06: pdepth_ampDepth
'     - prints the read depth of each base
'   o fun07: pGeneDepth_ampDepth
'     - prints the read depth of each base in the target
'       genes
'   o fun08: pGeneCoverage_ampDepth
'     - prints percent gene coverage and start/mid/end
'   o fun09: getGeneCoverage_ampDepth
'     - puts the gene coverage and depth into an array
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - indcluded libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "ampDepth.h"

#include <stdio.h>

#include "geneCoord.h"
#include "samEntry.h"

/*No .c files*/
#include "../genLib/endLine.h"
#include "../genLib/genMath.h" /*using .h macros*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: addRead_ampDepth
|   - adds a read to an amp depth arrary (histogram)
| Input:
|   - samSTPtr:
|     o pointer to an samEntry structure with a read to
|       add to the histogram
|   - coordsSTPtr:
|     o geneCoord struct pointer with gene/target
|       coordinates want to extract
|     o input 0/null to not filter by genes
|   - depthArySI:
|     o histogram (int array) to add each base to
|   - numOffTargSI:
|     o number of reads not mapping to a gene coordiante,
|       but are mapped to the genome
| Output:
|   - Modifies:
|     o ampDepthIAry to be incurmented for each reference
|       base in the read
\-------------------------------------------------------*/
void
addRead_ampDepth(
   struct samEntry *samSTPtr,
   struct geneCoord *coordsSTPtr, /*list of genes*/
   signed int *depthArySI,  /*depth array to update*/
   signed int *numOffTargSI /*number reads not in list*/
){
   signed int siBase = 0;
   signed int siIndex = 0;
   signed int endSI = 0;

   if(coordsSTPtr)
   { /*If: doing coordinate filtering*/
      siIndex =
         findRange_geneCoord(
            coordsSTPtr,
            samSTPtr->refStartUI,
            samSTPtr->refEndUI,
            0 /*no reference sequence*/
         );

      if(siIndex < 0)
         ++*numOffTargSI;
      else
      { /*Else: gene has some on target coordiantes*/
         addBases_fun01:;
         siBase =
            max_genMath(
                (signed int) samSTPtr->refStartUI,
                (signed int)
                   coordsSTPtr->startAryUI[siIndex]
            );

         endSI =
            min_genMath(
               (signed int) samSTPtr->refEndUI,
               (signed int) coordsSTPtr->endAryUI[siIndex]
            );

         while(siBase <= endSI)
            ++depthArySI[siBase++];


         /*see if read has muttiple genes*/
         ++siIndex;

         if(siIndex >= coordsSTPtr->lenSI)
            ; /*end of genes list*/
         else if(
              samSTPtr->refEndUI
            > coordsSTPtr->startAryUI[siIndex]
         ) goto addBases_fun01; /*have another gene*/
      } /*Else: gene has some on target coordiantes*/
   } /*If: doing coordinate filtering*/

   else
   { /*Else: not doing coordinate filtering*/
      siIndex = samSTPtr->refStartUI;
      while(siIndex <= (signed int) samSTPtr->refEndUI)
        ++depthArySI[siIndex++];
   } /*Else: not doing coordinate filtering*/
} /*addRead_ampDepth*/

/*-------------------------------------------------------\
| Fun02: phead_ampDepth
|   - prints the header for an ampDepth tsv file
| Input:
|   - outFILE:
|     o file to print the header to
| Output:
|   - Prints:
|     o ampDepth header to outFILE
\-------------------------------------------------------*/
void
phead_ampDepth(
   void *outFILE
){
   fprintf((FILE *) outFILE, "flag\tampNumber\trefStart");
   fprintf((FILE *) outFILE, "\trefEnd\tampStart");
   fprintf((FILE *) outFILE, "\tampEnd\tavgAmpDepth");
   fprintf((FILE *) outFILE, "\tminAmpDepth\tmaxAmpDepth");
   fprintf((FILE *) outFILE, "\tgeneId\trefGeneStart");
   fprintf((FILE *) outFILE, "\trefGeneEnd");
   fprintf((FILE *) outFILE, "\tampGeneStart");
   fprintf((FILE *) outFILE, "\tampGeneEnd");
   fprintf((FILE *) outFILE, "\tfirstBaseDepth");
   fprintf((FILE *) outFILE, "\tlastBaseDepth");
   fprintf((FILE *) outFILE, "\tavgDepth\tminDepth");
   fprintf(
      (FILE *) outFILE,
      "\tmaxDepth%s",
      str_endLine
   );
} /*phead_ampDepth*/

/*-------------------------------------------------------\
| Fun04: phist_ampDepth
|   - prints an histogram of read depths as an tsv with
|     individual genes for ampDepth
| Input:
|   - histArySI:
|     o integer array with the histogram to print out
|   - minDepthSI:
|     o integer with the min depth to keep an histogram
|       entry
|   - geneCoordSTPtr:
|     o pointer to an geneCoord structure array with the
|       locations of each gene
|   - offTargSI:
|     o number off of target reads to print out
|   - noMapSI:
|     o number off of unmapped reads to print out
|   - extraColStr:
|     o c-string with the first column to print out
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o stats for each gene to outFILE
\-------------------------------------------------------*/
void
phist_ampDepth(
   signed int *histArySI,
   signed int minDepthSI,
   struct geneCoord *geneCoordSTPtr,
   signed int offTargSI,
   signed int noMapSI,
   signed char *extraColStr,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04: phist_ampDepth
   '    - prints out an ampDepth histogram to file
   '    o fun04 sec01:
   '      - variable declerations
   '    o fun04 sec02:
   '      - find the mapping positions
   '    o fun04 sec03:
   '      - print out the off targets an unampped reads
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^  - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed int mapStartSI = 0;
    signed int tmpStartSI = 0;

    signed int ampNumSI = -1;
    signed int geneIndexSI = 0;
    signed int tmpSI = 0;

    signed int readsAtStartSI = 0;
    signed int readsAtEndSI = 0;

    /*Stats for individual genes*/
    signed int maxReadsSI = 0;
    signed int minReadsSI = 0;
    signed long avgDepthSL = 0;

    /*Stats for the amplicon*/
    signed int ampStartSI = 0;
    signed int ampEndSI = 0;
    signed int ampGeneEndSI = 0;
    signed int ampGeneStartSI = 0;

    signed long ampAvgDepthSL = 0;
    signed int ampMaxReadsSI = 0;
    signed int ampMinReadSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - find the mapping positions
   ^   o fun04 sec02 sub01:
   ^     - check if positon has enough depth to keep
   ^   o fun04 sec02 sub02:
   ^     - find the amplicon stats
   ^  o fun04 sec02 sub03:
   ^    - find the stats for each gene in the amplicon
   ^  o fun04 sec02 sub04:
   ^    - print the stats for each gene in the amplicon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01:
   *   - check if positon has enough depth to keep
   \*****************************************************/

   mapStartSI = 0;

   while(
        mapStartSI
     <= (signed int)
        geneCoordSTPtr->endAryUI[geneCoordSTPtr->lenSI -1]
   ){ /*Loop: Get the gene positions that mapped*/
      ++mapStartSI;

      if(
           mapStartSI
         < (signed int) geneCoordSTPtr->startAryUI[0]
      ) continue;

      geneIndexSI = 
         findStart_geneCoord(
            geneCoordSTPtr,
            mapStartSI,
            0
         ); /*Find the index of the gene at the position*/

      if(geneIndexSI < 0)
         continue; /*Unmapped base*/
      if(histArySI[mapStartSI] < minDepthSI)
         continue;

      /**************************************************\
      * Fun04 Sec02 Sub02:
      *   - find amplicon stats
      \**************************************************/

      ++ampNumSI;
      ampGeneEndSI = geneIndexSI;
      ampStartSI = mapStartSI;
      ampGeneStartSI =
         geneCoordSTPtr->startAryUI[geneIndexSI];

      ampAvgDepthSL = 0;
      ampMaxReadsSI = histArySI[mapStartSI];
      ampMinReadSI = ampMaxReadsSI;

      for(
         ampEndSI = mapStartSI;
         histArySI[ampEndSI] >= minDepthSI;
         ++ampEndSI
      ){ /*Loop: Find end of region*/
          ampGeneEndSI +=
            (    ampEndSI
               > (signed int)
                 geneCoordSTPtr->endAryUI[ampGeneEndSI]
            );

          ampMaxReadsSI = 
             max_genMath(
                ampMaxReadsSI,
                histArySI[ampEndSI]
             ); /*Find the maximum*/

          ampMinReadSI = 
             min_genMath(
                ampMinReadSI,
                histArySI[ampEndSI]
             ); /*Find the maximum*/
          
          ampAvgDepthSL += histArySI[ampEndSI];
      } /*Loop: Find end of region*/

      --ampEndSI; /*Account for overcounting*/
      ampGeneEndSI =
         geneCoordSTPtr->endAryUI[ampGeneEndSI];

      /*Using interger mean, not decimal*/
      tmpSI = (ampEndSI - ampStartSI);

      if(tmpSI)
         ampAvgDepthSL /= tmpSI;

      /**************************************************\
      * Fun04 Sec02 Sub03:
      *   - find the stats for each gene in the amplicon
      \**************************************************/

      tmpSI = 0;

      nextGene_fun04_sec02_sub03:

      ++tmpSI;
      tmpStartSI = mapStartSI;
      avgDepthSL = 0;
      readsAtStartSI = histArySI[mapStartSI];
      maxReadsSI = readsAtStartSI;
      minReadsSI = maxReadsSI;

      while(
            mapStartSI
         <= (signed int)
            geneCoordSTPtr->endAryUI[geneIndexSI]
      ){ /*Loop: Check if gene is complete*/
          if(histArySI[mapStartSI] < minDepthSI) break;

          maxReadsSI = 
             max_genMath(
                maxReadsSI,
                histArySI[mapStartSI]
             ); /*Find the maximum*/

          minReadsSI = 
             min_genMath(
                minReadsSI,
                histArySI[mapStartSI]
             ); /*Find the maximum*/
          
          avgDepthSL += histArySI[mapStartSI];

         ++mapStartSI;
      } /*Loop: Check if gene is complete*/

      readsAtEndSI = histArySI[mapStartSI - 1];

      /*Using interger mean, not decimal*/
      tmpSI = (mapStartSI - 1 - tmpStartSI);

      if(tmpSI)
         avgDepthSL /= tmpSI;

      /**************************************************\
      * Fun04 Sec02 Sub04:
      *   - print the stats for each gene in the amplicon
      \**************************************************/


      fprintf(
         (FILE *) outFILE,
         "%s\t%i\t%i\t%i\t%i\t%i\t%li\t%i\t%i",
         extraColStr,
         ampNumSI,
         ampGeneStartSI + 1,
         ampGeneEndSI + 1,
         ampStartSI + 1,
         ampEndSI + 1,
         ampAvgDepthSL,
         ampMinReadSI,
         ampMaxReadsSI
      ); /*Print out the starting and ending position*/

      fprintf(
         (FILE *) outFILE,
         "\t%s\t%u\t%u\t%i\t%i\t%i\t%i\t%li\t%i\t%i%s",
         geneCoordSTPtr->idStrAry[geneIndexSI],
         geneCoordSTPtr->startAryUI[geneIndexSI] + 1,
         geneCoordSTPtr->endAryUI[geneIndexSI] + 1,
         (signed int)
             max_genMath(
                (signed int)
                  geneCoordSTPtr->startAryUI[geneIndexSI],
                ampStartSI
             ) + 1, /*first gene base in amplicon*/
         (signed int)
             min_genMath(
                (signed int)
                   geneCoordSTPtr->endAryUI[geneIndexSI],
                ampEndSI
             ) + 1, /*last gene base in amplicon*/
         readsAtStartSI,
         readsAtEndSI,
         avgDepthSL,
         minReadsSI,
         maxReadsSI,
         str_endLine
      );

      if(histArySI[mapStartSI] > minDepthSI)
      { /*If: I have another gene*/
         ++geneIndexSI;
         if(geneIndexSI > geneCoordSTPtr->lenSI)
            continue;

         /*Make sure I am on the next gene*/
         while(
              mapStartSI
            < (signed int)
              geneCoordSTPtr->startAryUI[geneIndexSI]
         ){ /*Loop: Move to next gene*/
            if(histArySI[mapStartSI] < minDepthSI)
               break;

            ++mapStartSI;
         } /*Loop: Move to next gene*/

         if(histArySI[mapStartSI] < minDepthSI)
            continue;
 
         goto nextGene_fun04_sec02_sub03;
      } /*If: I have another gene*/
   } /*Loop: Get the gene positions that mapped*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^  - print off targets and unampped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ++ampNumSI;

   fprintf(
      (FILE *) outFILE,
      "%s\t%i\tNA\tNA\tNA\tNA\t%i\t%i\t%i",
      extraColStr,
      ampNumSI,
      offTargSI,
      offTargSI,
      offTargSI
   ); /*Print out the off target reads*/

   fprintf(
      (FILE *) outFILE,
      "\tx-off-target\tNA\tNA\tNA\tNA\t%i\t%i",
      offTargSI,
      offTargSI
   );

   fprintf(
      (FILE *) outFILE,
      "\t%i\t%i\t%i%s",
      offTargSI,
      offTargSI,
      offTargSI,
      str_endLine
   );

   ++ampNumSI;

   fprintf(
      (FILE *) outFILE,
      "%s\t%i\tNA\tNA\tNA\tNA\t%i\t%i\t%i",
      extraColStr,
      ampNumSI,
      noMapSI,
      noMapSI,
      noMapSI
   ); /*Print out the off target reads*/

   fprintf(
      (FILE *) outFILE,
      "\tz-unmapped\tNA\tNA\tNA\tNA\t%i\t%i",
      noMapSI,
      noMapSI
   );

   fprintf(
      (FILE *) outFILE,
      "\t%i\t%i\t%i%s",
      noMapSI,
      noMapSI,
      noMapSI,
      str_endLine
   );
} /*phist_ampDepth*/

/*-------------------------------------------------------\
| Fun05: pDepthHead_ampDepth
|   - prints the header for the read depth output
| Input:
|   - outFILE:
|     o file to print the header to
| Output:
|   - Prints:
|     o read depth header to outFILE
\-------------------------------------------------------*/
void
pDepthHead_ampDepth(
   void *outFILE
){
   fprintf((FILE *) outFILE, "reference\tflag\tbase");
   fprintf((FILE *) outFILE, "\tdepth%s", str_endLine);
} /*pDepthHead_ampDepth*/

/*-------------------------------------------------------\
| Fun06: pdepth_ampDepth
|   - prints the read depth of each base
| Input:
|   - depthArySI:
|     o integer array with the depthogram to print out
|   - depthLenSI:
|     o number of bases (length) of depthArySI (index 1)
|   - minDepthSI:
|     o integer with the min depth to keep an depthogram
|       entry
|   - refStr:
|     o c-string with name of reference sequence
|     o if 0/null then "reference"
|   - flagStr:
|     o c-string with flag to go in left column
|     o if 0/null then "out"
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o depth for each base to outFILE
\-------------------------------------------------------*/
void
pdepth_ampDepth(
   signed int *depthArySI,  /*has read depths*/
   signed int depthLenSI,   /*number bases in depthArySI*/
   signed int minDepthSI,   /*minimum read depth*/
   signed char *refStr,     /*name of reference*/
   signed char *flagStr,    /*flag to go in left column*/
   void *outFILE            /*output file*/
){

   signed int indexSI = 0;

   if(! flagStr)
      flagStr = (signed char *) "out";
   if(! refStr)
      refStr = (signed char *) "reference";

   while(indexSI < depthLenSI)
   { /*Loop: print bases*/
      if(depthArySI[indexSI] < minDepthSI)
      { /*Else If: position is filtered by read depth*/
         ++indexSI;
         continue;
      } /*Else If: position is filtered by read depth*/

      fprintf(
         (FILE *) outFILE,
         "%s\t%s\t%i\t%i%s",
         refStr,
         flagStr,
         indexSI + 1,
         depthArySI[indexSI],
         str_endLine
      );
      ++indexSI;
   } /*Loop: print bases*/
} /*pdepth_ampDepth*/

/*-------------------------------------------------------\
| Fun07: pGeneDepth_ampDepth
|   - prints the read depth of each base in the target
|     genes
| Input:
|   - depthArySI:
|     o integer array with the depthogram to print out
|     o the length needs to be the same as the last
|       gene coordinate in geneCoordSTPtr
|   - geneCoordSTPtr:
|     o geneCoord struct pointer with the coordinates
|       to print
|   - minDepthSI:
|     o integer with the min depth to keep an depthogram
|       entry
|   - refStr:
|     o c-string with name of reference sequence
|     o if 0/null then "reference"
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o depth for each base to outFILE
\-------------------------------------------------------*/
void
pGeneDepth_ampDepth(
   signed int *depthArySI,  /*has read depths*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   signed int minDepthSI,   /*minimum read depth*/
   signed char *refStr,     /*name of reference*/
   void *outFILE            /*output file*/
){
   unsigned int indexUI = 0;
   signed int geneSI;

   if(! refStr)
      refStr = (signed char *) "reference";

   for(geneSI=0; geneSI < geneCoordSTPtr->lenSI; ++geneSI)
   { /*Loop: print out the depth for each gene*/
      indexUI = geneCoordSTPtr->startAryUI[geneSI];

      while(indexUI < geneCoordSTPtr->endAryUI[geneSI])
      { /*Loop: print bases*/
         if(depthArySI[indexUI] < minDepthSI)
         { /*Else If: position is filtered by read depth*/
            ++indexUI;
            continue;
         } /*Else If: position is filtered by read depth*/

         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%u\t%i%s",
            refStr,
            geneCoordSTPtr->idStrAry[geneSI],
            indexUI + 1,
            depthArySI[indexUI],
            str_endLine
         );

         ++indexUI;
      } /*Loop: print bases*/
   } /*Loop: print out the depth for each gene*/
} /*pGeneDepth_ampDepth*/

/*-------------------------------------------------------\
| Fun08: pGeneCoverage_ampDepth
|   - prints percent gene coverage and start/mid/end
| Input
|   - depthArySI:
|     o signed int array with read depths
|   - minDepthSI:
|     o minimum read depth to count as covered
|   - geneCoordSTPtr:
|     o geneCoord struct with gene coordinates to print
|   - outFILE:
|     o FILE * pointer to print to
| Output:
|   - Prints:
|     o percent coverage and starts/ends to outFILE
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
pGeneCoverage_ampDepth(
   signed int *depthArySI, /*histogram of read depths*/
   signed int minDepthSI,  /*min depth to print*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   void *outFILE           /*file to print to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - prints percent gene coverage and start/mid/end
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - memory allocation and print header
   '   o fun08 sec03:
   '     - find gene coverage
   '   o fun08 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int *startHeapArySI = 0;
   signed int *endHeapArySI = 0;
   signed int *swapSIPtr = 0;
   signed int aryLenSI = 0;
   signed int arySizeSI = 0;
   signed char lowDepthBl = 0;

   signed int ntSI = 0;
   signed int geneSI = 0;
   signed int posSI = 0;
   signed int endSI = 0;
   signed int lenSI = 0;
   signed long depthSL = 0;

   signed int ampMaxSI = 0;
   signed char getFragBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - memory allocation and print header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   startHeapArySI = malloc(16 * sizeof(signed int));
   if(! startHeapArySI)
      goto memErr_fun08_sec04;

   endHeapArySI = malloc(16 * sizeof(signed int));
   if(! endHeapArySI)
      goto memErr_fun08_sec04;

   arySizeSI = 16;


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - find gene coverage
   ^   o fun08 sec03 sub01:
   ^     - start loop for each gene & get gene coordinates
   ^   o fun08 sec03 sub02:
   ^     - find the coverage for each gene
   ^   o fun08 sec03 sub03:
   ^     - print out the stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec03 Sub01:
   *   - start loop for each gene and get gene coordinates
   \*****************************************************/

   printTable_fun08_sec04_sub01:;
      /*avoids writing almost the same loop twice*/

   if(! getFragBl)
   { /*If: found the maximum number of fragments*/
      ++ampMaxSI; /*convert to index 1*/

      fprintf(
         (FILE *) outFILE,
         "gene\tperc_coverage\tnumber_bases\tmean_depth"
      );
      fprintf((FILE *) outFILE, "\tgene_length");

   
      for(ntSI = 1; ntSI <= ampMaxSI; ++ntSI)
      { /*Loop: print out the fragment header*/
         fprintf(
            (FILE *) outFILE,
            "\tstart_%i\tend_%i",
            ntSI,
            ntSI
         );
      } /*Loop: print out the fragment header*/

      fprintf((FILE *) outFILE, "%s", str_endLine);
   } /*If: found the maximum number of fragments*/


   for(geneSI=0; geneSI < geneCoordSTPtr->lenSI; ++geneSI)
   { /*Loop: go though all genes to print out*/
      posSI = geneCoordSTPtr->startAryUI[geneSI];
      endSI = geneCoordSTPtr->endAryUI[geneSI];
      lenSI = 1 + endSI - posSI;
         /*+ 1 to convert to index 0*/
      aryLenSI = 0;
      lowDepthBl = -1;
      startHeapArySI[0] = 0;
      endHeapArySI[0] = 0;
      ntSI = 0;
      depthSL = 0;

      /**************************************************\
      * Fun08 Sec03 Sub02:
      *   - find the coverage for each gene
      \**************************************************/

      while(posSI <= endSI)
      { /*Loop: get read depth*/
         depthSL += depthArySI[posSI];

         if(depthArySI[posSI] < minDepthSI)
         { /*If: have low read depth*/
            if(lowDepthBl < 0)
               ; /*first position, no start found yet*/
            else if(! lowDepthBl)
            { /*Else If: have a true split*/
               endHeapArySI[aryLenSI] = posSI - 1;
               ++aryLenSI;

               if(aryLenSI >= arySizeSI)
               { /*If: need more memory*/
                  arySizeSI += (arySizeSI >> 1);

                  swapSIPtr =
                     realloc(
                        startHeapArySI,
                        arySizeSI * sizeof(signed int)
                     );
                  if(! swapSIPtr)
                     goto memErr_fun08_sec04;
                  startHeapArySI = swapSIPtr;

                  swapSIPtr =
                     realloc(
                        endHeapArySI,
                        arySizeSI * sizeof(signed int)
                     );
                  if(! swapSIPtr)
                     goto memErr_fun08_sec04;
                  endHeapArySI = swapSIPtr;
               } /*If: need more memory*/

               lowDepthBl = 1;
            } /*Else If: have a true split*/

            /*Else: already moved to next item*/
         } /*If: have low read depth*/

         else
         { /*Else: have enough read depth*/
            if(lowDepthBl)
               startHeapArySI[aryLenSI] = posSI;
            ++ntSI;
            lowDepthBl = 0;
         } /*Else: have enough read depth*/

         ++posSI;
      } /*Loop: get read depth*/

      /**************************************************\
      * Fun08 Sec03 Sub03:
      *   - print out the stats
      \**************************************************/

      if(getFragBl)
      { /*If: finding the maximum number of fragments*/
         if(aryLenSI > ampMaxSI)
            ampMaxSI = aryLenSI;
         continue;
      } /*If: finding the maximum number of fragments*/

      if(! lowDepthBl)
      { /*If: had last base in gene*/
         endHeapArySI[aryLenSI] = endSI;
         ++aryLenSI;
      } /*If: had last base in gene*/

      if(! ntSI)
         fprintf(
            (FILE *) outFILE,
            "%s\t0\t0\t0\t%i",
            geneCoordSTPtr->idStrAry[geneSI],
            lenSI
         );
      else
         fprintf(
            (FILE *) outFILE,
            "%s\t%0.2f\t%i\t%0.2f\t%i",
            geneCoordSTPtr->idStrAry[geneSI],
            (float) ntSI / (float) lenSI,
            ntSI,
            (float) ((double) depthSL / (float) lenSI),
            lenSI
         );

      for(ntSI = 0; ntSI < aryLenSI; ++ntSI)
      { /*Loop: print out start and ends of gaps*/
         if(! endHeapArySI[ntSI])
            fprintf((FILE *) outFILE, "\tNA\tNA");
         else 
            fprintf(
               (FILE *) outFILE,
               "\t%i\t%i",
               startHeapArySI[ntSI] + 1,
               endHeapArySI[ntSI] + 1
            );
      } /*Loop: print out start and ends of gaps*/

      for(; ntSI < ampMaxSI; ++ntSI)
         fprintf((FILE *) outFILE, "\tNA\tNA");
         /*mark blank cells with NA*/

      fprintf((FILE *) outFILE, "%s", str_endLine);

      aryLenSI = 0;
   } /*Loop: go though all genes to print out*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(getFragBl)
   { /*If: need to print out the table*/
      getFragBl = 0;
      goto printTable_fun08_sec04_sub01;
   } /*If: need to print out the table*/

   ntSI = 0;
   goto ret_fun08_sec04;

   memErr_fun08_sec04:;
      ntSI = 1;
      goto ret_fun08_sec04;

   ret_fun08_sec04:;
      if(startHeapArySI)
         free(startHeapArySI);
      startHeapArySI = 0;

      if(endHeapArySI)
         free(endHeapArySI);
      endHeapArySI = 0;

      return ntSI;
} /*pGeneCoverage_ampDepth*/

/*-------------------------------------------------------\
| Fun09: getGeneCoverage_ampDepth
|   - puts the gene coverage and depth into an array
| Input
|   - depthArySI:
|     o signed int array with read depths
|   - minDepthSI:
|     o minimum read depth to count as covered
|   - geneCoordSTPtr:
|     o geneCoord struct with gene coordinates to print
| Output:
|   - Returns:
|     o float array (size = geneCoordSTPtr->lenSI * 3)
|       with percent coverage, coverage mean read depth,
|       and gene mean read depth
|       * has two items per index
|       * index 0: gene percent coverage (as decimal)
|       * index 1: mean read depth in covered region
|       * index 2: gene mean read depth (as decimal)
|       * you can get the gene index with (index / 3)
|     o 0 if had a memory error
\-------------------------------------------------------*/
float *
getGeneCoverage_ampDepth(
   signed int *depthArySI, /*histogram of read depths*/
   signed int minDepthSI,  /*min depth to print*/
   struct geneCoord *geneCoordSTPtr /*gene coordinates*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - prints percent gene coverage and start/mid/end
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - memory allocation and print header
   '   o fun09 sec03:
   '     - find gene coverage
   '   o fun09 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int geneSI = 0;/*gene on*/
   signed int posSI = 0; /*current position at in genome*/
   signed int endSI = 0; /*end coordinate of gene*/
   signed int lenSI = 0;
      /*holds gene length for finding mean read depth*/

   signed int ntSI = 0;  /*number of bases reads covered*/
   signed long depthSL = 0; /*for gene mean read depth*/
   signed long coverDepthSL = 0;
      /*for coverge mean read depth*/

   float *retHeapAryF = 0;
   signed int retPosSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - memory allocation and print header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retHeapAryF =
      calloc((geneCoordSTPtr->lenSI * 3), sizeof(float));
   if(! retHeapAryF)
      goto memErr_fun09_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - find gene coverage
   ^   o fun09 sec03 sub01:
   ^     - start loop for each gene & get gene coordinates
   ^   o fun09 sec03 sub02:
   ^     - find the coverage for each gene
   ^   o fun09 sec03 sub03:
   ^     - print out the stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(geneSI=0; geneSI < geneCoordSTPtr->lenSI; ++geneSI)
   { /*Loop: go though all genes to print out*/
      posSI = geneCoordSTPtr->startAryUI[geneSI];
      endSI = geneCoordSTPtr->endAryUI[geneSI];
      lenSI = 1 + endSI - posSI;

      ntSI = 0;
      depthSL = 0;
      coverDepthSL = 0;


      while(posSI <= endSI)
      { /*Loop: get coverage and read depth*/
         if(depthArySI[posSI] >= minDepthSI)
         { /*If: enough read depth to mark as covered*/
            ++ntSI;
            coverDepthSL += depthArySI[posSI];
         } /*If: enough read depth to mark as covered*/

         depthSL += depthArySI[posSI];
         ++posSI;
      } /*Loop: get coverage and read depth*/


      if(! ntSI)
      { /*If: had no coverage*/
         retHeapAryF[retPosSI++] = 0;
         retHeapAryF[retPosSI++] = 0;
      } /*If: had no coverage*/

      else
      { /*Else: had coverage*/
         retHeapAryF[retPosSI++] =
            (float) ntSI / (float) lenSI;
         retHeapAryF[retPosSI++] =
            (float) coverDepthSL / (float) ntSI;
      } /*Else: had coverage*/

      retHeapAryF[retPosSI++] =
         (float) depthSL / (float) lenSI;
   } /*Loop: go though all genes to print out*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun09_sec04;

   memErr_fun09_sec04:;
      ntSI = 1;
      goto errClean_fun09_sec04;

   errClean_fun09_sec04:;
      if(retHeapAryF)
         free(retHeapAryF);
      retHeapAryF = 0;

      goto ret_fun09_sec04;

   ret_fun09_sec04:;
      return retHeapAryF;
} /*getGeneCoverage_ampDepth*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
