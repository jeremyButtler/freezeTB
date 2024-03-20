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
