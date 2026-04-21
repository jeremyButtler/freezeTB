/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' geneCoord SOF: Start Of File
'   - holds the geneCoord struct and its supporting
'     functions I use to read in gene positions form a paf
'     file
'   o header:
'     - Includes and definitions
'   o .h st: geneCoord
'     - Holds the arrays for the gene coordinates and ids 
'       in gene table
'   o fun02: freeStack_geneCoord
'     - Frees the arrays in a geneCoord structure
'   o fun03: freeHeap_geneCoord
'     - Frees the a heap alloacted geneCoord structure
'   o fun01: init_geneCoord
'     - Initializes a geneCoord structure
'   o fun04: mk_geneCoord
'     - Makes a heap allocated geneCoord structure
'   o .c fun06: swap_geneCoord
'     - Swaps two array items in a geneCoord structure
'       around
'   o fun07: sort_geneCoord
'     - sorts the arrays in a genesCoord structure by
'       gene name and then reference
'   o fun08: findStart_geneCoord
'     - Does a binary search by starting coordinate for a
'       potentail gene in a geneCoord structure
'   o fun10: sortName_geneCoord
'     - Sorts the arrays in a genesCoord structure by
'       gene name
'   o fun11: nameSortFloat3IndexSync_geneCoord
'    - sorts the arrays in a genesCoord structure by
'      gene name and keep an array of floats (index 3) in
'      sync
'   o fun12: findName_geneCoord
'     - Does a binary search to find an gene name in an
'       gene geneCoord structer (must be sorted by name)
'   o fun13: getCoords_geneCoord
'     - Gets the gene coordinates from an gene coordinates
'       table
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "geneCoord.h"

#include <stdio.h>

#include "../genLib/fileFun.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"

/*.h files only*/
#include "../genLib/genMath.h" /*only max macro (in .h)*/

/*-------------------------------------------------------\
| Fun01: init_geneCoord
|    - Initializes a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to initialize
| Output:
|   - Sets:
|     o idStrAry, startAryUI, and endAryUI to 0
\-------------------------------------------------------*/
void
init_geneCoord(
   struct geneCoord *geneCoordST
){
   geneCoordST->idStrAry = 0;
   geneCoordST->refAryStr = 0;

   geneCoordST->startAryUI = 0;
   geneCoordST->endAryUI = 0;

   geneCoordST->dirAryUC = 0;
   geneCoordST->lenSI = 0;
} /*init_geneCoord*/

/*-------------------------------------------------------\
| Fun02: freeStack_geneCoord
|    - Frees the arrays in a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure with arrays to
|       free
| Output:
|   - Frees:
|     o idStrAry, startAryUI, and endAryUI (and sets to 0)
\-------------------------------------------------------*/
void
freeStack_geneCoord(
   struct geneCoord *geneCoordST
){
   if(! geneCoordST)
      return;

   if(geneCoordST->idStrAry)
      free(geneCoordST->idStrAry);

   if(geneCoordST->refAryStr)
      free(geneCoordST->refAryStr);
   
   if(geneCoordST->startAryUI)
      free(geneCoordST->startAryUI);

   if(geneCoordST->endAryUI)
      free(geneCoordST->endAryUI);

   if(geneCoordST->dirAryUC)
      free(geneCoordST->dirAryUC);

   init_geneCoord(geneCoordST);
} /*freeStack_geneCoord*/

/*-------------------------------------------------------\
| Fun03: freeHeap_geneCoord
|    - Frees the a heap alloacted geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to free
| Output:
|   - Frees:
|     o geneCoordST
\-------------------------------------------------------*/
void
freeHeap_geneCoord(
   struct geneCoord *geneCoordST
){
   if(! geneCoordST)
      return;

   freeStack_geneCoord(geneCoordST);
   free(geneCoordST);
} /*freeHeap_geneCoord*/

/*-------------------------------------------------------\
| Fun04: mk_geneCoord
|    - Makes a heap allocated geneCoord structure
| Input:
|   - numGenesUI:
|     o Length of the arrays in the new geneCoord
|       structure
| Output:
|   - Returns:
|     o geneCoord structure with allocated arrays
| Note:
|   - Do not use init_geneCoord on the returned structure
\-------------------------------------------------------*/
struct geneCoord *
mk_geneCoord(
   unsigned int numGenesUI
){
   struct geneCoord *retST = 0;

   retST = malloc(sizeof(struct geneCoord));
   if(! retST)
      goto memErr_fun04;
   init_geneCoord(retST);
   
   retST->idStrAry =
      calloc((numGenesUI), sizeof(*retST->idStrAry));
   if(! retST->idStrAry)
      goto memErr_fun04;

   retST->refAryStr =
      calloc((numGenesUI), sizeof(*retST->refAryStr));
   if(! retST->refAryStr)
      goto memErr_fun04;

   retST->startAryUI=
       malloc((numGenesUI) * sizeof(unsigned int));
   if(! retST->startAryUI)
      goto memErr_fun04;
   
   retST->endAryUI =
       malloc((numGenesUI) * sizeof(unsigned int));
   if(! retST->endAryUI)
      goto memErr_fun04;

   retST->dirAryUC =
       malloc((numGenesUI) * sizeof(unsigned char));
   if(! retST->dirAryUC)
      goto memErr_fun04;

   return retST;

   memErr_fun04:;
      freeHeap_geneCoord(retST);
      retST = 0;
      return 0;
} /*mk_geneCoord*/

/*-------------------------------------------------------\
| Fun06: swap_geneCoord
|  - Swaps two array items in a geneCoord structure
|    around
| Input:
|  - geneCoordST:
|    o Pointer to the geneCoord structure to swap elements
|      in
|  - posOne:
|    o The position (index) of the first gene to swap
|  - posTwo:
|    o The position (index) of the second gene to swap
| Output:
|  - Modifies:
|    o Swaps the start, end, and gene id around
\-------------------------------------------------------*/
void
swap_geneCoord(
   struct geneCoord *coordST,
   unsigned long oneUL,
   unsigned long secUL
){
  unsigned char ucSwap = 0;

  coordST->startAryUI[oneUL]^= coordST->startAryUI[secUL];
  coordST->startAryUI[secUL]^= coordST->startAryUI[oneUL];
  coordST->startAryUI[oneUL]^= coordST->startAryUI[secUL];

  coordST->endAryUI[oneUL] ^= coordST->endAryUI[secUL];
  coordST->endAryUI[secUL] ^= coordST->endAryUI[oneUL];
  coordST->endAryUI[oneUL] ^= coordST->endAryUI[secUL];

  coordST->dirAryUC[oneUL] ^= coordST->dirAryUC[secUL];
  coordST->dirAryUC[secUL] ^= coordST->dirAryUC[oneUL];
  coordST->dirAryUC[oneUL] ^= coordST->dirAryUC[secUL];

  while(
        coordST->idStrAry[oneUL][ucSwap] != '\0'
     || coordST->idStrAry[secUL][ucSwap] != '\0'
  ){ /*Loop: copy the gene name*/
     coordST->idStrAry[oneUL][ucSwap] ^=
        coordST->idStrAry[secUL][ucSwap];

     coordST->idStrAry[secUL][ucSwap] ^=
        coordST->idStrAry[oneUL][ucSwap];

     coordST->idStrAry[oneUL][ucSwap] ^=
        coordST->idStrAry[secUL][ucSwap];

     ++ucSwap;
  } /*Loop: copy the gene name*/

  coordST->idStrAry[oneUL][ucSwap] = '\0';
  coordST->idStrAry[secUL][ucSwap] = '\0';

  while(
        coordST->refAryStr[oneUL][ucSwap] != '\0'
     || coordST->refAryStr[secUL][ucSwap] != '\0'
  ){ /*Loop: copy the gene name*/
     coordST->refAryStr[oneUL][ucSwap] ^=
        coordST->refAryStr[secUL][ucSwap];

     coordST->refAryStr[secUL][ucSwap] ^=
        coordST->refAryStr[oneUL][ucSwap];

     coordST->refAryStr[oneUL][ucSwap] ^=
        coordST->refAryStr[secUL][ucSwap];

     ++ucSwap;
  } /*Loop: copy the gene name*/

  coordST->refAryStr[oneUL][ucSwap] = '\0';
  coordST->refAryStr[secUL][ucSwap] = '\0';
} /*swapScoreSTs*/

/*-------------------------------------------------------\
| Fun07: sort_geneCoord
|  - Sorts the arrays in a genesCoord structure by
|    starting positiion with shell short.
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sort_geneCoord(
   struct geneCoord *geneCoordST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun07 TOC: sort_geneCoord
   '  - Sorts the arrays in a geneCoord struct by
   '    starting position with shell short.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun07 sec01:
   '    - Variable declerations
   '  o fun07 sec02:
   '    - Find the number of rounds to sort for
   '  o fun07 sec03:
   '    - Sort the arrays in geneCoordST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  signed long numElmSL = geneCoordST->lenSI - 1;
  
  /*Number of sorting rounds*/
  signed long subSL = 0;
  signed long nextElmSL = 0;
  signed long lastElmSL = 0;
  signed long elmOnSL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  signed long slIndex = 0;
  signed long slElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subSL = 1; /*Initialzie first array*/
  while(subSL < numElmSL - 1)
     subSL = (3 * subSL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun07 Sec03:
  ^  - Sort the arrays in geneCoordST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subSL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(slIndex = 0; slIndex <= subSL; ++slIndex)
    { /*For each element in the subarray*/
      for(slElm = slIndex;
          slElm + subSL <= numElmSL;
          slElm += subSL
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmSL = slElm + subSL;
        
        if(
             geneCoordST->startAryUI[slElm]
           > geneCoordST->startAryUI[nextElmSL]
        ) goto swapNext_fun07_sec03;

        else if(
             geneCoordST->startAryUI[slElm]
           < geneCoordST->startAryUI[nextElmSL]
        ) ;

        else if(
            eqlNull_ulCp(
               geneCoordST->refAryStr[slElm],
               geneCoordST->refAryStr[nextElmSL]
            ) > 0
        ){ /*If I need to swap an element*/
          swapNext_fun07_sec03:;
          swap_geneCoord((geneCoordST),slElm,nextElmSL);
          
          lastElmSL = slElm;
          elmOnSL = slElm;
          
          while(lastElmSL >= subSL)
          { /*loop; move swapped element back*/
            lastElmSL -= subSL;
            
            if(
                  geneCoordST->startAryUI[elmOnSL]
                > geneCoordST->startAryUI[lastElmSL]
            ) break; /*positioned the element*/

            else if(
                  geneCoordST->startAryUI[elmOnSL]
                < geneCoordST->startAryUI[lastElmSL]
            ) ; /*need to move elmOnSL one item back*/

            else if(
               eqlNull_ulCp(
                  geneCoordST->refAryStr[elmOnSL],
                  geneCoordST->refAryStr[lastElmSL]
               ) > 0
            ) break;
            
            swap_geneCoord(geneCoordST,elmOnSL,lastElmSL);
            
            elmOnSL = lastElmSL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subSL = (subSL - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sort_geneCoord*/

/*-------------------------------------------------------\
| Fun08: findStart_geneCoord
|  - Does a binary search by starting coordinate for a
|    potentail gene in a geneCoord structure
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - qryUI:
|    o Starting coordinate (query) to search for in
|      geneCoordST
|  - refStr:
|    o c-string with reference to find the gene for
|    o 0/null for ignore reference
| Output:
|  - Returns:
|    o The index of the starting position
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findStart_geneCoord(
   struct geneCoord *geneST,
   unsigned int qryUI,
   signed char *refStr
){
   signed int midSI = 0;
   signed int rightSI = geneST->lenSI - 1;
   signed int leftSI = 0;

   signed long matchSL = 0;

   while(leftSI <= rightSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftSI + rightSI) >> 1;

      if(qryUI < geneST->startAryUI[midSI])
         rightSI = midSI - 1;

      else if(qryUI > geneST->endAryUI[midSI])
         leftSI = midSI + 1;

     else
     { /*Else: if may have a match*/
        if(! refStr || ! refStr[0])
           return midSI; /*no reference sequence*/
        
         matchSL =
            eqlNull_ulCp(refStr,geneST->refAryStr[midSI]);
         if(! matchSL)
            return midSI; /*have matching reference*/
         else if(matchSL < 0)
            rightSI = midSI - 1;
         else
            leftSI = midSI - 1;
     } /*Else: if may have a match*/
   } /*Loop: Search for the starting coordinate*/

   if(qryUI < geneST->startAryUI[midSI])
      return -1;
   if(qryUI > geneST->endAryUI[midSI])
      return -1;

   return midSI;
} /*findStart_geneCoord*/

/*-------------------------------------------------------\
| Fun09: findRange_geneCoord
|  - finds start of range read coordinate may be in
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - startUI:
|    o starting coordinate (query) to search for
|  - endUI:
|    o ending coordinate (query) to search for
|  - refStr:
|    o c-string with reference to find the gene for
|    o 0/null for ignore reference
| Output:
|  - Returns:
|    o index of gene that overlaps with startUI and endUI
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findRange_geneCoord(
   struct geneCoord *geneST,
   unsigned int startUI,
   unsigned int endUI,
   signed char *refStr
){
   signed int midSI = 0;
   signed int rightSI = geneST->lenSI - 1;
   signed int leftSI = 0;

   signed long matchSL = 0;

   while(leftSI <= rightSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftSI + rightSI) >> 1;

      if(endUI < geneST->startAryUI[midSI])
         rightSI = midSI - 1;

      else if(startUI > geneST->endAryUI[midSI])
         leftSI = midSI + 1;

     else if(! midSI)
        goto possibleMatch_fun10; /*at start of array*/

     else if(startUI > geneST->endAryUI[midSI - 1])
     { /*Else: may have foud a match*/
        possibleMatch_fun10:;

        if(! refStr || ! refStr[0])
           return midSI; /*no reference sequence*/
        
         matchSL =
            eqlNull_ulCp(refStr,geneST->refAryStr[midSI]);
         if(! matchSL)
            return midSI; /*have matching reference*/
         else if(matchSL < 0)
            rightSI = midSI - 1;
         else
            leftSI = midSI - 1;
     } /*Else: may have foud a match*/

     else
         rightSI = midSI - 1;
         /*at least one more item in range*/
   } /*Loop: Search for the starting coordinate*/

   return -1; /*nothing in range*/
} /*findRange_geneCoord*/

/*-------------------------------------------------------\
| Fun10: sortName_geneCoord
|  - sorts the arrays in a genesCoord structure by
|    gene name and then reference
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - startUI:
|    o First element to start sorting at
|  - endUI:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sortName_geneCoord(
   struct geneCoord *geneCoordST
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun10 TOC:
   '  - Sorts the arrays in a genesCoord structure by
   '    gene name
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun10 sec01:
   '    - Variable declerations
   '  o fun10 sec02:
   '    - Find the number of rounds to sort for
   '  o fun10 sec03:
   '    - Sort the arrays in geneCoordST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  signed long numElmSL = geneCoordST->lenSI - 1;
  
  /*Number of sorting rounds*/
  signed long subSL = 0;
  signed long nextElmSL = 0;
  signed long lastElmSL = 0;
  signed long elmOnSL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  signed long slIndex = 0;
  signed long slElm = 0;
  signed long compareSL = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subSL = 1; /*Initialzie first array*/
  while(subSL < numElmSL - 1)
     subSL = (3 * subSL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun10 Sec03:
  ^  - Sort the arrays in geneCoordST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subSL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(slIndex = 0; slIndex <= subSL; ++slIndex)
    { /*For each element in the subarray*/
      for(slElm = slIndex;
          slElm + subSL <= numElmSL;
          slElm += subSL
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmSL = slElm + subSL;
        
        compareSL =
           eqlNull_ulCp(
              geneCoordST->idStrAry[slElm],
              geneCoordST->idStrAry[nextElmSL]
           );

        if(compareSL > 0)
           goto swapNext_fun10_sec03;
        else if(compareSL < 0)
           ;
        else if(
            eqlNull_ulCp(
               geneCoordST->refAryStr[slElm],
               geneCoordST->refAryStr[nextElmSL]
            ) > 0
        ){ /*If I need to swap an element*/
          swapNext_fun10_sec03:;
          swap_geneCoord((geneCoordST),slElm,nextElmSL);
          
          lastElmSL = slElm;
          elmOnSL = slElm;
          
          while(lastElmSL >= subSL)
          { /*loop; move swapped element back*/
            lastElmSL -= subSL;
            
            compareSL =
               eqlNull_ulCp(
                  geneCoordST->idStrAry[elmOnSL],
                  geneCoordST->idStrAry[lastElmSL]
               );
            if(compareSL > 0)
               break; /*positioned the gene*/
            else if(compareSL < 0)
               ; /*need to position the element*/
            else if(
                eqlNull_ulCp(
                   geneCoordST->refAryStr[elmOnSL],
                   geneCoordST->refAryStr[lastElmSL]
                ) > 0
            ) break; /*Positioned the element*/
            
            swap_geneCoord(geneCoordST,elmOnSL,lastElmSL);
            elmOnSL = lastElmSL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subSL = (subSL - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sortName_geneCoord*/

/*-------------------------------------------------------\
| Fun11: nameSortFloat3IndexSync_geneCoord
|  - sorts the arrays in a genesCoord structure by
|    gene name and keep an array of floats (index 3) in
|    sync
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - numGenesSI:
|    o number of genes
|  - floatAry:
|    o float array to keep in sync
|    o float array has 3 entries per gene, so is moved in
|      groups of threee
| Output:
|  - Modifies:
|    o arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
|    o floatAry to be in sync with geneCoordST
\-------------------------------------------------------*/
void
nameSortFloat3IndexSync_geneCoord(
   struct geneCoord *geneCoordST,
   float *floatAryF
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun11 TOC:
   '  - Sorts the arrays in a genesCoord structure by
   '    gene name and keep an array of floats (index 3) in
   '    sync
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun11 sec01:
   '    - Variable declerations
   '  o fun11 sec02:
   '    - Find the number of rounds to sort for
   '  o fun11 sec03:
   '    - Sort the arrays in geneCoordST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun11 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  signed long endSI = geneCoordST->lenSI - 1;
  
  /*Number of sorting rounds*/
  signed int subSI = 0;
  signed int nextSI = 0;
  signed int lastSI = 0;
  signed int onSI = 0;

  /*these variables are for keeping the float array in
  `  sync
  */
  float swapF = 0;
  signed int firstSwapUL = 0;
  signed int secSwapUL = 0;

  /*Variables to incurment loops*/
  signed int siIndex = 0;
  signed int curSI = 0;

  signed long compareSL = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun11 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  if(endSI <= 0)
     return;

  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subSI = 1; /*Initialzie first array*/
  while(subSI < endSI)
     subSI = (3 * subSI) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun11 Sec03:
  ^  - Sort the arrays in geneCoordST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subSI > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(siIndex = 0; siIndex <= subSI; ++siIndex)
    { /*For each element in the subarray*/
      for(curSI = siIndex;
          curSI + subSI <= endSI;
          curSI += subSI
      ){ /*Loop; swap each nth element of the subarray*/
        nextSI = curSI + subSI;
        
        compareSL =
           eqlNull_ulCp(
              geneCoordST->idStrAry[curSI],
              geneCoordST->idStrAry[nextSI]
           );

        if(compareSL > 0)
           goto swapNext_fun11_sec03;
        else if(compareSL < 0)
           ;
        else if(
            eqlNull_ulCp(
               geneCoordST->refAryStr[curSI],
               geneCoordST->refAryStr[nextSI]
            ) > 0
        ){ /*If I need to swap an element*/
          swapNext_fun11_sec03:;

          swap_geneCoord((geneCoordST),curSI,nextSI);
          firstSwapUL = curSI * 3;
          secSwapUL = nextSI * 3;
          
          swapF = floatAryF[firstSwapUL];
          floatAryF[firstSwapUL++] = floatAryF[secSwapUL];
          floatAryF[secSwapUL++] = swapF;

          swapF = floatAryF[firstSwapUL];
          floatAryF[firstSwapUL++] = floatAryF[secSwapUL];
          floatAryF[secSwapUL++] = swapF;

          swapF = floatAryF[firstSwapUL];
          floatAryF[firstSwapUL++] = floatAryF[secSwapUL];
          floatAryF[secSwapUL++] = swapF;

          lastSI = curSI;
          onSI = curSI;
          
          while(lastSI >= subSI)
          { /*loop; move swapped element back*/
            lastSI -= subSI;
            
            compareSL =
               eqlNull_ulCp(
                  geneCoordST->idStrAry[onSI],
                  geneCoordST->idStrAry[lastSI]
               );
            if(compareSL > 0)
               break; /*positioned the gene*/
            else if(compareSL < 0)
               ; /*need to position the element*/
            else if(
                eqlNull_ulCp(
                   geneCoordST->refAryStr[onSI],
                   geneCoordST->refAryStr[lastSI]
                ) > 0
            ) break; /*Positioned the element*/
            
            swap_geneCoord(geneCoordST, onSI, lastSI);

            firstSwapUL = onSI * 3;
            secSwapUL = lastSI * 3;
             
            swapF = floatAryF[firstSwapUL];
            floatAryF[firstSwapUL++]=floatAryF[secSwapUL];
            floatAryF[secSwapUL++] = swapF;

             swapF = floatAryF[firstSwapUL];
             floatAryF[firstSwapUL++] =
                floatAryF[secSwapUL];
             floatAryF[secSwapUL++] = swapF;

             swapF = floatAryF[firstSwapUL];
             floatAryF[firstSwapUL++] =
                floatAryF[secSwapUL];
             floatAryF[secSwapUL++] = swapF;
            
            onSI = lastSI;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subSI = (subSI - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*nameSortFloat3IndexSync_geneCoord*/

/*-------------------------------------------------------\
| Fun12: findName_geneCoord
|  - Does a binary search to find an gene name in an gene
|    geneCoord structer (must be sorted by name)
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure to sort
|      coordinates to search
|  - nameStr:
|    o c-string with name to search for
|  - refStr:
|    o c-string with reference sequence
|    o use 0 for search for gene only
| Output:
|  - Returns:
|    o The index of gene with the same name
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findName_geneCoord(
   struct geneCoord *geneST,
   signed char *nameStr,
   signed char *refStr
){ 
   signed int midSI = 0;
   signed int rightHalfSI = geneST->lenSI - 1;
   signed int leftHalfSI = 0;

   signed long matchSL = 0;

   while(leftHalfSI <= rightHalfSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftHalfSI + rightHalfSI) >> 1;

      matchSL =
         eqlNull_ulCp(nameStr, geneST->idStrAry[midSI]);
            /*see if I have an match*/

      if(matchSL < 0)
         rightHalfSI = midSI - 1;

      else if(matchSL > 0)
         leftHalfSI = midSI + 1;

     else
     { /*Else: may have foud a match*/
        if(! refStr || ! refStr[0])
           return midSI; /*no reference sequence*/
        
         matchSL =
            eqlNull_ulCp(refStr,geneST->refAryStr[midSI]);
         if(! matchSL)
            return midSI; /*have matching reference*/
         else if(matchSL < 0)
            rightHalfSI = midSI - 1;
         else
            leftHalfSI = midSI - 1;
     } /*Else: may have foud a match*/
   } /*Loop: Search for the starting coordinate*/

   return -1; /*No match found*/
} /*findName_geneCoord*/

/*-------------------------------------------------------\
| Fun13: getCoords_geneCoord
|  - Gets the gene coordinates from a gene table (tsv)
| Input:
|  - geneTblFileStr:
|    o C-string with name of the gene table file to
|      extract the gene coordinates and names from
|  - errULPtr:
|    o Will hold the error return value
| Output:
|  - Returns:
|    o Pointer to an sorted geneCoord structure with the 
|      gene coordinates
|    o 0 for errors
|  - Modifies:
|    o errULPtr to hold the error
|      - 0 for no errors
|      - def_fileErr_geneCoord for an file opening error
|      - def_memErr_geneCoord for an memor error
|      - (line_number << 8) | def_invalidEntry_geneCoord
|        for an invalid line in the file
|        o Get the line number with (*errULPtr >> 8)
\-------------------------------------------------------*/
struct geneCoord *
getCoords_geneCoord(
   signed char *geneTblFileStr,
   unsigned long *errULPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC: getCoords_geneCoord
   '   - Gets the gene coordinates from a gene table (tsv)
   '   o fun13 Sec01:
   '     - Variable declerations
   '   o fun13 Sec02:
   '     - Check input and allocate memory for buffer
   '   o fun13 Sec03:
   '     - Find number lines/max line length in table file
   '   o fun13 Sec04:
   '     - Allocate memory and go back to start of file
   '   o fun13 Sec05:
   '     - Read in the gene coordinates from the file
   '   o fun13 Sec06:
   '     - Clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buffHeapStr = 0;
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   signed long maxLineSL = 0;
   signed long numLinesSL = 0;
   
   struct geneCoord *genesHeapST = 0;
   FILE *tblFILE  = 0;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - Check input and allocate memory for buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - Find number lines/max line length in table file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errULPtr = 0;

   tblFILE  = fopen( (char *) geneTblFileStr, "r");
   if(! tblFILE)
      goto fileErr_fun13_sec06_sub03;
   numLinesSL = lineCnt_fileFun(tblFILE, &maxLineSL);
   maxLineSL += 3; /*account for line endings*/

   buffHeapStr =
      calloc((maxLineSL + 8), sizeof(signed char));
   if(! buffHeapStr)
       goto memErr_fun13_sec06_sub02;

   genesHeapST = mk_geneCoord(numLinesSL);
   if(! genesHeapST)
      goto memErr_fun13_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec05:
   ^   - Read in the gene coordinates from the file
   ^   o fun13 sec05 sub01:
   ^     - Start loop and copy gene name
   ^   o fun13 sec05 sub02:
   ^     - Move past the refernce id
   ^   o fun13 sec05 sub03:
   ^     - Move past the gene direction
   ^   o fun13 sec05 sub04:
   ^     - Get coordiante of frist reference base in gene
   ^   o fun13 sec05 sub05:
   ^     - Get coordiante of last reference base in gene
   ^   o fun13 sec05 sub06:
   ^     - Move to the next gene
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun13 Sec05 Sub01:
   *   - Start loop and copy gene name
   \*****************************************************/

   
   numLinesSL =
      getLine_fileFun(
         tblFILE,
         buffHeapStr,
         maxLineSL,
         &numLinesSL /*will ingore*/
      ); /*get past header (already know has)*/

   while(
      getLine_fileFun(
         tblFILE,
         buffHeapStr,
         maxLineSL,
         &numLinesSL /*will ingore*/
      )
   ){ /*Loop: Get entries from coordinates file*/

      cpStr = buffHeapStr;
      dupStr = genesHeapST->idStrAry[genesHeapST->lenSI];

      /*Copy the gene name*/
      while(*cpStr > 32)
         *dupStr++ = *cpStr++;

      *dupStr = '\0';

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                      /*new line or null*/
         goto invalidEntry_fun13_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun13 Sec05 Sub02:
      *   - get refernce id
      \**************************************************/

      dupStr = genesHeapST->refAryStr[genesHeapST->lenSI];

      /*Copy the gene name*/
      while(*cpStr > 32)
         *dupStr++ = *cpStr++;

      *dupStr = '\0';

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                            /*new line or null*/
         goto invalidEntry_fun13_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun13 Sec05 Sub03:
      *   - get the gene direction
      \**************************************************/

      genesHeapST->dirAryUC[genesHeapST->lenSI] =
         (*cpStr =='-');
      genesHeapST->dirAryUC[genesHeapST->lenSI] |=
         (*cpStr=='R');

      while(*cpStr++ > 32) ;

      if(*(cpStr - 1) == '\t') ;      /*Expected*/
      else if(*(cpStr - 1) == ' ') ;  /*Odd, but works*/
      else                            /*new line or null*/
         goto invalidEntry_fun13_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun13 Sec05 Sub04:
      *   - Get coordiante of frist reference base in gene
      \**************************************************/

      cpStr +=
         strToUI_base10str(
            cpStr,
            &genesHeapST->startAryUI[genesHeapST->lenSI]
         ); /*Get the genes frist reference base*/

      --genesHeapST->startAryUI[genesHeapST->lenSI];

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                      /*new line or null*/
         goto invalidEntry_fun13_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun13 Sec05 Sub05:
      *   - Get coordiante of last reference base in gene
      \**************************************************/

      cpStr +=
         strToUI_base10str(
            cpStr,
            &genesHeapST->endAryUI[genesHeapST->lenSI]
         ); /*Get the genes last reference base*/

      --genesHeapST->endAryUI[genesHeapST->lenSI];

      if(*cpStr > 32)
         goto invalidEntry_fun13_sec06_sub04; /*not tsv*/

      if(
           genesHeapST->endAryUI[genesHeapST->lenSI]
         < genesHeapST->startAryUI[genesHeapST->lenSI]
      ){ /*If: I need to coordinates*/
         genesHeapST->endAryUI[genesHeapST->lenSI] ^=
            genesHeapST->startAryUI[genesHeapST->lenSI];

         genesHeapST->startAryUI[genesHeapST->lenSI] ^=
            genesHeapST->endAryUI[genesHeapST->lenSI];

         genesHeapST->endAryUI[genesHeapST->lenSI] ^=
            genesHeapST->startAryUI[genesHeapST->lenSI];
      } /*If: I need to swap coordinates*/

      /**************************************************\
      * Fun13 Sec05 Sub06:
      *   - Move to the next gene
      \**************************************************/

      ++genesHeapST->lenSI;
   } /*Loop: Get entries from coordinates file*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec06:
   ^   - Clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sort_geneCoord(genesHeapST);

   goto cleanUp_fun13_sec06_sub05;

   memErr_fun13_sec06_sub02:;
      *errULPtr = def_memErr_geneCoord;
      goto errCleanUp_fun13_sec06_sub05;

   fileErr_fun13_sec06_sub03:;
      *errULPtr = def_fileErr_geneCoord;
      goto errCleanUp_fun13_sec06_sub05;

   invalidEntry_fun13_sec06_sub04:;
      *errULPtr = def_invalidEntry_geneCoord;
      *errULPtr |= (genesHeapST->lenSI << 8);
      goto errCleanUp_fun13_sec06_sub05;

   errCleanUp_fun13_sec06_sub05:;
      freeHeap_geneCoord(genesHeapST);
      genesHeapST = 0;
      goto cleanUp_fun13_sec06_sub05;

   cleanUp_fun13_sec06_sub05:;
      free(buffHeapStr);
      buffHeapStr = 0;

      if(tblFILE)
         fclose(tblFILE);

      tblFILE = 0;

      return genesHeapST;
} /*getCoords_geneCoord*/

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
