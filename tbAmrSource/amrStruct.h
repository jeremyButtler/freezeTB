/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and definitions
'   o st-01: amrStruct
'     - Holds the information for a single amr mutation
'       that was extracted from the WHO catalog
'   o fun-01: amrIsRes
'     - Dectects if an amr was classified as resistant or
'       is unknow/not resitance
'   o fun-02: blankAmrStruct
'     - Sets all non-pointer values in amrStructPtr to 0
'   o fun-03: initAmrStruct
'     - Sets all values, including pointers in the
'       amrStruct structure to 0
'   o fun-04: freeAmrStructStack
'     - Frees the geneIdStr, refSeqStr, and amrSeqStr
'       arrays and sets all values to 0 in the input
'       amrStruct 
'   o fun-05: freeAmrStruct
'     - Frees an heap allocated amrStruct structure
'   o fun-06: freeAmrStructArray
'     - Frees an heap allocated array of amrStruct
'       structures
'   o fun-07: swapAmrStructs
'     - Swaps the values in two amrStruct structures
'   o fun-08: sortAmrStructArray
'     - Sort on an amrStruct array structures by reference
'       coordiante (uses shell sort)
'   o fun-09: findNearestAmr
'      - Finds the nearest amr at or after the input query
'   o fun-10: cpDrugToDrugAry
'      - Copies an antibiotic to a drug array
'   o fun-11: getDrugFromDrugAry
'     - Gets the pointer to a drug c-string in a drugAry
'   o fun-12: mallocDrugAryStr
'     - Returns a pointer to allocated memory for
'       drugAryStr
'   o fun-13: reallocDrugAryStr
'     - Returns a pointer to rellocated memory for
'       drugAryStr
'   o fun-14: read_2021_WhoAmrCsv (TODO: UPDATE)
'     - Gets the amr data from the Who 2021 TB antibiotice
'       resistance catalog (genome indicie tab saved as
'       a csv).
'   o fun-15: checkCrossRes
'     - Check if there is cross resitance (2023 catalog)
'   o fun-16: pCrossRes
'     - Print out cross resitance (report not database)
'    o fun-17: read_2023_WhoAmrTsv
'      - Reads in the two tabs (as separate tsv's) and
'        converts them to an amrStructs array
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and definitions
\-------------------------------------------------------*/

#ifndef AMR_STRUCT_H
#define AMR_STRUCT_H

#include <stdio.h>
#include "../generalLib/genMath.h"
#include "../generalLib/codonTbl.h"
#include "genIndiceStruct.h"

#define def_amrST_forwardDir 0
#define def_amrST_revCompDir 1
#define def_amrST_unkownDir 2
#define def_amrST_memError 64
#define def_amrST_missingIndice 2

#define def_FQ_CrossResFlag 1
#define def_BDQ_CrossResFlag 2
#define def_CFZ_CrossResFlag 4
#define def_INH_CrossResFlag 8
#define def_ETO_CrossResFlag 16

/*-------------------------------------------------------\
| ST-01: amrStruct
|   - Holds the information for a single amr mutation
|     that was extracted from the WHO catalog
\-------------------------------------------------------*/
typedef struct amrStruct{
   uint refPosUI;   /*Starting position on reference*/
   uint codonPosUI; /*Codon starting position on ref*/
   uint codonNumUI; /*Codon number*/
   uint endCodonNumUI; /*end of an indel*/

   char refAaC;     /*What the reference amino acid is*/
   char amrAaC;     /*What the mutant amino acid is*/
   char frameshiftBl; /*1: is a frame shift*/

   char *geneIdStr; /*Gene name*/
   uint lenGeneIdUI;/*Length of gene id*/

   char *refSeqStr; /*Reference sequence at refPosUI*/
   uint lenRefSeqUI;/*Length of reference sequence*/

   char *amrSeqStr; /*AMR sequence at refPosUI*/
   uint lenAmrSeqUI;/*Length of amr sequence*/

   char *varIdStr;  /*Variant nomincalture/id*/
   uint lenVarIdUI;

   char *effectStr; /*WHO comment on what mutation did*/
   uint lenEffectUI;/*Length of effect*/

   char *commentStr; /*WHO comment (has epistatic)*/
   uint lenCommentUI; /*Length of comment*/

   char gradeC;    /*Grade of resitance*/
   char mutTypeStr[4]; /*Mutation type*/
   char dirFlag;   /*-1: gene reverse complement, else 0*/
      /*This is marked in the gene_locus column as a c
      ' at the end
      */
   ulong amrFlagsUL;   /*flags for antibiotic classes*/
   ulong crossResFlagsUL;/*flags for cross resistance*/
   uint numSupReadsUI; /*Number of reads supporting*/
   uint numMapReadsUI; /*Number of reads mapping*/
}amrStruct;

/*-------------------------------------------------------\
| Fun-01: amrIsRes
|   - Dectects if an amr was classified as resistant or
|     is unknow/not resitance
| Input:
|   - amrC:
|     - Character with grade assigned by the WHO
| Output:
|   - Returns:
|     o 1 if was a grade 1/2 (resistance)
|     o 0 if was not a grade 1/2
\-------------------------------------------------------*/
#define amrIsRes(amrC)(\
   ((lineStr[uiIter] > 48) & (lineStr[uiIter] < 51))\
)
  /*Logic:
  `  - lineStr[uiIter] < 51
  `    o 1 if is a number < 3 or is a special symbol, or
  `      an invisible character
  '    o limits to special symbols and 0, 1, 2
  '      This should remove NA's, and grade 3, 4, and 5
  '  - lineStr[uiIter] > 48
  `    o 1 if is a number > 0 or a character + extra
  `    o 0 if not 1
  `  - (lineStr[uiIter] >) 48 & (lineStr[uiIter] < 51)
  `    o 1 if number is 1 or 2
  `    o 0 if character is not 1 or 2
  */     


/*-------------------------------------------------------\
| Fun-02: blankAmrStruct
|   - Sets all non-pointer values in amrStructPtr to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to blank
| Output:
|   - Modifies
|     o All non-pointer values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define blankAmrStruct(amrStructPtr){\
   (amrStructPtr)->refPosUI = 0;\
   (amrStructPtr)->codonPosUI = 0;\
   (amrStructPtr)->codonNumUI = 0;\
   (amrStructPtr)->endCodonNumUI = 0;\
   \
   (amrStructPtr)->refAaC = 0;\
   (amrStructPtr)->amrAaC = 0;\
   (amrStructPtr)->frameshiftBl = 0;\
   \
   (amrStructPtr)->dirFlag = def_amrST_unkownDir;\
   (amrStructPtr)->mutTypeStr[0] = '\0';\
   \
   (amrStructPtr)->gradeC = 0;\
   (amrStructPtr)->amrFlagsUL = 0;\
   (amrStructPtr)->numSupReadsUI = 0;\
   (amrStructPtr)->numMapReadsUI = 0;\
   \
   (amrStructPtr)->crossResFlagsUL = 0;\
} /*blankAmrStruct*/

/*-------------------------------------------------------\
| Fun-03: initAmrStruct
|   - Sets all values, including pointers in amrStructPtr
|     to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to initialize
| Output:
|   - Modifies
|     o All values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define initAmrStruct(amrStructPtr){\
   blankAmrStruct((amrStructPtr));\
   (amrStructPtr)->geneIdStr = 0;\
   (amrStructPtr)->lenGeneIdUI = 0;\
   \
   (amrStructPtr)->refSeqStr = 0;\
   (amrStructPtr)->lenRefSeqUI = 0;\
    \
   (amrStructPtr)->amrSeqStr = 0;\
   (amrStructPtr)->lenAmrSeqUI = 0;\
   \
   (amrStructPtr)->varIdStr = 0;\
   (amrStructPtr)->lenVarIdUI = 0;\
   \
   (amrStructPtr)->effectStr = 0;\
   (amrStructPtr)->lenEffectUI = 0;\
   \
   (amrStructPtr)->commentStr = 0;\
   (amrStructPtr)->lenCommentUI = 0;\
} /*initAmrStruct*/

/*-------------------------------------------------------\
| Fun-04: freeAmrStructStack
|   - Frees the geneIdStr, refSeqStr, and amrSeqStr arrays
|     in amrStructPtr and sets all values to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrStructPtr to be 0
\-------------------------------------------------------*/
#define freeAmrStructStack(amrStructPtr){\
   if((amrStructPtr)->geneIdStr != 0)\
     free((amrStructPtr)->geneIdStr);\
   \
   if((amrStructPtr)->refSeqStr != 0)\
     free((amrStructPtr)->refSeqStr);\
   \
   if((amrStructPtr)->amrSeqStr != 0)\
     free((amrStructPtr)->amrSeqStr);\
   \
   if((amrStructPtr)->varIdStr != 0)\
     free((amrStructPtr)->varIdStr);\
   \
   if((amrStructPtr)->effectStr != 0)\
     free((amrStructPtr)->effectStr);\
   \
   if((amrStructPtr)->commentStr != 0)\
     free((amrStructPtr)->commentStr);\
   \
   initAmrStruct((amrStructPtr));\
} /*freeAmrStructStack*/

/*-------------------------------------------------------\
| Fun-05: freeAmrStruct
|   - Frees an heap allocated amrStruct structure
| Input:
|   - ampStructPtr:
|     - Pointer to amrStruct to free
| Output:
|   - Frees:
|     o amrStructPtr and the variables inside amrStructPtr
\-------------------------------------------------------*/
#define freeAmrStruct(amrStructPtr){\
   freeAmrStructStack((amrStructPtr));\
   free(amrStructPtr);\
   amrStructPtr = 0;\
} /*freeAmrStruct*/

/*-------------------------------------------------------\
| Fun-06: freeAmrStructArray
|   - Frees an heap allocated array of amrStruct
|     structures
| Input:
|   - ampStructPtr:
|     - Pointer to start of amrStruct array to free
| Output:
|   - Frees:
|     o The amrStructPtr array and the variables inside
|       each amrStructPtr
\-------------------------------------------------------*/
#define freeAmrStructArray(amrStructAryPtr, numElm){\
   ulong ulElmMac = 0;\
   for(ulElmMac = 0; ulElmMac < numElm; ++ulElmMac)\
      freeAmrStructStack(&(amrStructAryPtr)[ulElmMac]);\
   \
   free(amrStructAryPtr);\
   amrStructAryPtr = 0;\
} /*freeAmrStructArray*/

/*-------------------------------------------------------\
| Fun-07: swapAmrStructs
|   - Swaps the values in two amrStruct structures
| Input:
|   - firstAmrSTPtr:
|     - Pointer to first amrStruct to swap values in
|   - secAmrSTPtr:
|     - Pointer to second amrStruct to swap values in
| Output:
|   - Modifies:
|     o fristAmrSTPtr to have the values of secAmrSTPtr
|     o secAmrSTPtr to have the values of frstAmrSTPtr
\-------------------------------------------------------*/
#define swapAmrStructs(firstAmrSTPtr, secAmrSTPtr){\
   char tmpMacC = 0;\
   char *tmpMacStr = 0;\
   uint tmpMacUI = 0;\
   ulong tmpMacUL = 0;\
   \
   \
   tmpMacUI = (firstAmrSTPtr).refPosUI;\
   (firstAmrSTPtr).refPosUI = (secAmrSTPtr).refPosUI;\
   (secAmrSTPtr).refPosUI = tmpMacUI;\
   \
   \
   tmpMacUI = (firstAmrSTPtr).codonPosUI;\
   (firstAmrSTPtr).codonPosUI = (secAmrSTPtr).codonPosUI;\
   (secAmrSTPtr).codonPosUI = tmpMacUI;\
   \
   \
   tmpMacUI = (firstAmrSTPtr).codonNumUI;\
   (firstAmrSTPtr).codonNumUI = (secAmrSTPtr).codonNumUI;\
   (secAmrSTPtr).codonNumUI = tmpMacUI;\
   \
   \
   tmpMacC = (firstAmrSTPtr).refAaC;\
   (firstAmrSTPtr).refAaC = (secAmrSTPtr).refAaC;\
   (secAmrSTPtr).refAaC = tmpMacC;\
   \
   \
   tmpMacC = (firstAmrSTPtr).amrAaC;\
   (firstAmrSTPtr).amrAaC = (secAmrSTPtr).amrAaC;\
   (secAmrSTPtr).amrAaC = tmpMacC;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).geneIdStr;\
   (firstAmrSTPtr).geneIdStr = (secAmrSTPtr).geneIdStr;\
   (secAmrSTPtr).geneIdStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenGeneIdUI;\
   (firstAmrSTPtr).lenGeneIdUI=(secAmrSTPtr).lenGeneIdUI;\
   (secAmrSTPtr).lenGeneIdUI = tmpMacUI;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).refSeqStr;\
   (firstAmrSTPtr).refSeqStr = (secAmrSTPtr).refSeqStr;\
   (secAmrSTPtr).refSeqStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenRefSeqUI;\
   (firstAmrSTPtr).lenRefSeqUI = (secAmrSTPtr).lenRefSeqUI;\
   (secAmrSTPtr).lenRefSeqUI = tmpMacUI;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).amrSeqStr;\
   (firstAmrSTPtr).amrSeqStr = (secAmrSTPtr).amrSeqStr;\
   (secAmrSTPtr).amrSeqStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenAmrSeqUI;\
   (firstAmrSTPtr).lenAmrSeqUI=(secAmrSTPtr).lenAmrSeqUI;\
   (secAmrSTPtr).lenAmrSeqUI = tmpMacUI;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).varIdStr;\
   (firstAmrSTPtr).varIdStr = (secAmrSTPtr).varIdStr;\
   (secAmrSTPtr).varIdStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenVarIdUI;\
   (firstAmrSTPtr).lenVarIdUI = (secAmrSTPtr).lenVarIdUI;\
   (secAmrSTPtr).lenVarIdUI = tmpMacUI;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).effectStr;\
   (firstAmrSTPtr).effectStr = (secAmrSTPtr).effectStr;\
   (secAmrSTPtr).effectStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenEffectUI;\
   (firstAmrSTPtr).lenEffectUI=(secAmrSTPtr).lenEffectUI;\
   (secAmrSTPtr).lenEffectUI = tmpMacUI;\
   \
   \
   tmpMacStr = (firstAmrSTPtr).commentStr;\
   (firstAmrSTPtr).commentStr = (secAmrSTPtr).commentStr;\
   (secAmrSTPtr).commentStr = tmpMacStr;\
   \
   tmpMacUI = (firstAmrSTPtr).lenCommentUI;\
   \
   (firstAmrSTPtr).lenCommentUI =\
      (secAmrSTPtr).lenCommentUI;\
   \
   (secAmrSTPtr).lenCommentUI = tmpMacUI;\
   \
   \
   tmpMacC = (firstAmrSTPtr).dirFlag;\
   (firstAmrSTPtr).dirFlag = (secAmrSTPtr).dirFlag;\
   (secAmrSTPtr).dirFlag = tmpMacC;\
   \
   \
   tmpMacUL = (firstAmrSTPtr).amrFlagsUL;\
   (firstAmrSTPtr).amrFlagsUL = (secAmrSTPtr).amrFlagsUL;\
   (secAmrSTPtr).amrFlagsUL = tmpMacUL;\
   \
   \
   tmpMacUI = (firstAmrSTPtr).numSupReadsUI;\
   \
   (firstAmrSTPtr).numSupReadsUI =\
      (secAmrSTPtr).numSupReadsUI;\
   \
   (secAmrSTPtr).numSupReadsUI = tmpMacUI;\
   \
   \
   tmpMacUI = (firstAmrSTPtr).numMapReadsUI;\
   \
   (firstAmrSTPtr).numMapReadsUI =\
      (secAmrSTPtr).numMapReadsUI;\
   \
   (secAmrSTPtr).numMapReadsUI = tmpMacUI;\
   \
   \
   tmpMacC = (firstAmrSTPtr).mutTypeStr[0];\
   \
   (firstAmrSTPtr).mutTypeStr[0]\
      = (secAmrSTPtr).mutTypeStr[0];\
   \
   (secAmrSTPtr).mutTypeStr[0] = tmpMacC;\
   \
   tmpMacC = (firstAmrSTPtr).mutTypeStr[1];\
   \
   (firstAmrSTPtr).mutTypeStr[1]\
      = (secAmrSTPtr).mutTypeStr[1];\
   \
   (secAmrSTPtr).mutTypeStr[1] = tmpMacC;\
   \
   tmpMacC = (firstAmrSTPtr).mutTypeStr[2];\
   \
   (firstAmrSTPtr).mutTypeStr[2]\
      = (secAmrSTPtr).mutTypeStr[2];\
   \
   (secAmrSTPtr).mutTypeStr[2] = tmpMacC;\
}

/*-------------------------------------------------------\
| Fun-08: sortAmrStructArray
|   - Sort on an amrStruct array structures by reference
|     coordiante (uses shell sort)
| Input:
|   - amrAryST:
|     - Pointer to start of amrStruct array to sort
| Output:
|   - Modifies:
|     o The amrAryST to be sorted by starting
|       reference coordiante
\-------------------------------------------------------*/
static void sortAmrStructArray(
   struct amrStruct *amrAryST,
   uint startUI,
   uint endUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-08 TOC: sortGenIndice
   '  - Sorts the arrays in a genIndice struct by variant
   '    id with shell short.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun-08 sec-01:
   '    - Variable declerations
   '  o fun-08 sec-02:
   '    - Find the number of rounds to sort for
   '  o fun-08 sec-03:
   '    - Sort the arrays in genIndiceST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun-08 Sec-01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  ulong numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  ulong subUL = 0;
  ulong nextElmUL = 0;
  ulong lastElmUL = 0;
  ulong elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  ulong ulIndex = 0;
  ulong ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun-08 Sec-02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun-08 Sec-03:
  ^  - Sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(ulIndex = 0; ulIndex <= subUL; ++ulIndex)
    { /*For each element in the subarray*/
      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmUL = ulElm + subUL;
        
        if(  amrAryST[ulElm].refPosUI
           > amrAryST[nextElmUL].refPosUI
        ){ /*If I need to swap an element*/
          swapAmrStructs(
             amrAryST[ulElm],
             amrAryST[nextElmUL]
          ); /*Swap the values in the two structuers*/
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*loop; move swapped element back*/
            lastElmUL -= subUL;
            
            if(  amrAryST[elmOnUL].refPosUI
               > amrAryST[lastElmUL].refPosUI
            ) break; /*Positioned the element*/
            
             swapAmrStructs(
                amrAryST[elmOnUL],
                amrAryST[lastElmUL]
             ); /*Swap the values in the two structuers*/
            
            elmOnUL = lastElmUL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sortGenIndice*/

/*-------------------------------------------------------\
| Fun-09: findNearestAmr
|  - Does a binary search for the nearest amr at or after
|    to the input query coordiante
| Input:
|  - amrAryST:
|    o Pointer to an array of amrStruct structures to
|      search
|  - qryUI:
|    o Starting coordinate (query) to search for in
|      amrAryST
|  - numGenesUI:
|    o Number of amrStruct's in amrAryST (index 0)
| Output:
|  - Returns:
|    o The nearest index at or after qryUI
|    o -1 for no amr genes after qryUI
\-------------------------------------------------------*/
static int findNearestAmr(
   struct amrStruct *amrAryST,
   uint qryUI,
   int numAmrI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun-09 TOC: findNearestAmr
   '   - Finds the nearest amr at or after the input query
   '     coordiante
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   int midI = 0; /*divide by 2*/
   int rightHalfI = numAmrI - 1;
   int leftHalfI = 0;

   while(leftHalfI <= rightHalfI)
   { /*Loop: Search for the starting coordinate*/
      midI = (leftHalfI + rightHalfI) >> 1;

     if(qryUI > amrAryST[midI].refPosUI)
         leftHalfI = midI + 1;

      else if(qryUI < amrAryST[midI].refPosUI)
         rightHalfI = midI - 1;

     else
     { /*Else: I found the query*/
        if(midI == 0) return 0;

        while(midI > 0)
        { /*Loop: Make sure I am on the first amr*/
           if(amrAryST[midI - 1].refPosUI != qryUI) break;
           --midI;
        } /*Loop: Make sure I am on the first amr*/

        return midI;
     } /*Else: I found the query*/

     /*branchless whith an if return was slower here*/
   } /*Loop: Search for the starting coordinate*/

   /*Make sure it is alwasy greater than*/
   midI += (amrAryST[midI].refPosUI < qryUI);

   /*I may have the same coordinates repeated*/
   while(midI > 0)
   { /*Loop: Make sure I am on the first amr*/
      if(amrAryST[midI - 1].refPosUI != qryUI) break;
      --midI;
   } /*Loop: Make sure I am on the first amr*/

   /*Make sure not at end of vector*/
   if(midI == numAmrI) return -1;

   return midI;
} /*findNearestAmr*/
 
/*-------------------------------------------------------\
| Fun-10: cpDrugToDrugAry
|   - Copies an antibiotic to a drug array
| Input:
|   - drugAryStr:
|     o Drug array to copy the antibiotic name to
|   - drugStr:
|     o Drug to copy
|   - indexUI:
|     o Index to copy drug to
|   - delimC:
|     o Deliminator to stop copy at
| Output:
|   - Modifies:
|     o dubAryStr to hold the new drug name
|   - Returns:
|     o The length of the copied string
\-------------------------------------------------------*/
#define cpDrugToDrugAry(\
   drugAryStr,\
   drugStr,\
   indexUI,\
   delimC\
)({\
   char *dupMacStr = (drugAryStr) + 32 * (indexUI);\
   char *cpMacStr = (drugStr);\
   char *endStr = (drugStr) + 32;\
   \
   while(cpMacStr != endStr && *cpMacStr != (delimC))\
      {*dupMacStr++ = *cpMacStr++;}\
   \
   *dupMacStr = '\0';\
   cpMacStr - (drugStr);\
}) /*cpDrugToDrugAry*/

/*-------------------------------------------------------\
| Fun-11: getDrugFromDrugAry
|   - Gets the pointer to a drug c-string in a drugAry
| Input:
|   - drugAryStr:
|     o Drug array to get the antibiotic name pointer
|   - indexUI:
|     o Index to grab
| Output:
|   - Returns:
|     o An pointer to the correct position
|       resitant (grade 1 and 2) mutations
\-------------------------------------------------------*/
#define getDrugFromDrugAry(drugAryStr, indexUI)(\
   (char *) (drugAryStr) + ((indexUI) * 32)\
) /*getDrugFromDrugAry*/

/*-------------------------------------------------------\
| Fun-12: mallocDrugAryStr
|   - Returns a pointer to allocated memory for drugAryStr
| Input:
|   - numStringsUI:
|     o Number of strings to make
| Output:
|   - Returns:
|     o An pointer to the new arrray
|     o 0 for memory error
\-------------------------------------------------------*/
#define mallocDrugAryStr(numStringsUI)(\
   malloc((numStringsUI) * 32 * sizeof(char))\
) /*mallocDrugAryStr*/

/*-------------------------------------------------------\
| Fun-13: reallocDrugAryStr
|   - Returns a pointer to rellocated memory for
|     drugAryStr
| Input:
|   - drugAryStr:
|     o Pionter to drugAryStr to reallocate memory for
|   - numStringsUI:
|     o Number of strings to make
| Output:
|   - Modifies:
|     o drugAryStr to be resized
|     o frees and sets drugAryStr to 0 if memory error
\-------------------------------------------------------*/
#define reallocDrugAryStr(drugAryStr, numStringsUI){\
   char * tmpMacStr = 0;\
   tmpMacStr =\
      realloc(\
         (drugAryStr),\
         (numStringsUI) * 32 * sizeof(char)\
      );\
   \
   if(!tmpMacStr)\
   { /*If: I had a memory error*/\
      free((drugAryStr));\
      (drugAryStr) = 0;\
   } /*If: I had a memory error*/\
   \
   else (drugAryStr) = tmpMacStr;\
} /*reallocDrugAryStr*/

/*-------------------------------------------------------\
| Fun-14: read_2021_WhoAmrCsv
|   - Gets the amr data from the Who TB antibiotice
|     resistance 2021 catalog (genome indicie tab saved as
|     a csv).
| Input:
|   - whoFILE:
|     o Csv file for the WHO's genome indience tab of
|       the TB antibiotic catalog spread sheet
|   - numArmUL:
|     o Modified to hold the number of catalog entriex
| Output:
|   - Modifies:
|     o numArmUL to hold the number of extracted amr
|       mutations.
|   - Returns:
|     o An array of amrStruct's (free with
|       freeAmrStructArray)
\-------------------------------------------------------*/
static struct amrStruct * read_2021_WhoAmrCsv(
   FILE *whoFILE,
   ulong *numAmrUL,
   char **drugAryStr
){
   ushort lenBuffUS = 2048;
   char buffStr[lenBuffUS]; /*buffer*/
   char *lineStr = buffStr; /*Holds a single line*/
   char *tmpStr = 0;

   uint  maxLineLenUI = 0;     /*max line length*/
   uint  lenLineUI = 0;        /*Length of one line*/
   ulong numLinesUL = 0;
   ulong charReadInUL = 0;

   uint uiIter = 0;        /*Iterator for loops*/
   uint uiColIter = 0;     /*For looping through columns*/
   uint uiAmr = 0;

   uint lenSeqUI = 0;
   uint lenIdUI = 0;

   struct amrStruct *amrST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-02:
   ^  - Find the number of lines in the who file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Read in the first section of the file*/
   charReadInUL =
      fread(lineStr, sizeof(char), lenBuffUS, whoFILE);

   while(charReadInUL)
   { /*Loop: Get the number of lines in the file*/
      lenLineUI = 0;

      for(uiIter = 0; uiIter < charReadInUL; ++uiIter)
      { /*Loop: Count number of new lines & characters*/
         ++lenLineUI;         
         numLinesUL += lineStr[uiIter] == '\n';
      } /*Loop: Count number of new lines & characters*/

      maxLineLenUI = noBranchMax(maxLineLenUI, lenLineUI);

      charReadInUL =
         fread(lineStr,sizeof(char),lenBuffUS,whoFILE);
   } /*Loop: Get the number of lines in the file*/

   if(numLinesUL < 2) return 0;

   --numLinesUL; /*Account for the header*/
   fseek(whoFILE, 0, SEEK_SET);
   maxLineLenUI += 2;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-03:
   ^  - Set up the antibiotic table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *drugAryStr = mallocDrugAryStr(14);

   if(*drugAryStr == 0) return 0;

   /*The who has changed there format, so this is safe
   ` with hardcoding
   */
   cpDrugToDrugAry(*drugAryStr, "RIF", 0, '\t');
   cpDrugToDrugAry(*drugAryStr, "INH", 1, '\t');
   cpDrugToDrugAry(*drugAryStr, "EMB", 2, '\t');
   cpDrugToDrugAry(*drugAryStr, "PZA", 3, '\t');
   cpDrugToDrugAry(*drugAryStr, "LEV", 4, '\t');
   cpDrugToDrugAry(*drugAryStr, "MXF", 5, '\t');
   cpDrugToDrugAry(*drugAryStr, "BDQ", 6, '\t');
   cpDrugToDrugAry(*drugAryStr, "LZD", 7, '\t');
   cpDrugToDrugAry(*drugAryStr, "CFZ", 8, '\t');
   cpDrugToDrugAry(*drugAryStr, "AMI", 9, '\t');
   cpDrugToDrugAry(*drugAryStr, "STM", 10, '\t');
   cpDrugToDrugAry(*drugAryStr, "ETH", 11, '\t');
   cpDrugToDrugAry(*drugAryStr, "KAN", 12, '\t');
   cpDrugToDrugAry(*drugAryStr, "CAP", 13, '\t');

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-14 Sec-04:
   ^  - Extract the AMR mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineStr = malloc(maxLineLenUI * sizeof(char));

   if(lineStr == 0)
   { /*If I failed to make a buffer*/
      free(*drugAryStr);
      *drugAryStr = 0;
      return 0;
   } /*If I failed to make a buffer*/

   amrST = calloc(numLinesUL, sizeof(struct amrStruct));
     /*There is some waste in memory usage, but this
     ` allows me to work with an array
     */

   if(amrST == 0)
   { /*If: I had a memroy error*/
      free(*drugAryStr);
      *drugAryStr = 0;

      free(lineStr);
      lineStr = 0;

      return 0;
   } /*If: I had a memroy error*/

   *numAmrUL = 0;

   /*tmpStr is to avoid complents from ubuntus compiler*/
   tmpStr = fgets(lineStr, maxLineLenUI, whoFILE);

   while(fgets(lineStr, maxLineLenUI, whoFILE))
   { /*Loop: Read in all lines in the WHO's amr file*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-02:
     *   - Find the length of the gene name
     \***************************************************/

     for(uiIter = 0; lineStr[uiIter] != ','; ++uiIter){}
     
     lenIdUI = uiIter; /*+1 for '\0'*/

     amrST[*numAmrUL].geneIdStr =
        malloc((lenIdUI + 1) * sizeof(char));

     if(!amrST[*numAmrUL].geneIdStr) goto variantMemErr;
     
     for(uiIter = 0; lineStr[uiIter] != ','; ++uiIter)
       amrST[*numAmrUL].geneIdStr[uiIter]=lineStr[uiIter];

     amrST[*numAmrUL].geneIdStr[uiIter] = '\0';
     amrST[*numAmrUL].lenGeneIdUI = uiIter;

     ++uiIter;

     /***************************************************\
     * Fun-14 Sec-04 Sub-03:
     *   - Check if it is reverse complement
     \***************************************************/

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
    
     amrST[*numAmrUL].dirFlag =
       -(lineStr[uiIter -1] =='c') & def_amrST_revCompDir;

     ++uiIter;
     
     /***************************************************\
     * Fun-14 Sec-04 Sub-04:
     *   - Get the codon number
     \***************************************************/
     
     /*Get off variant column*/
     while(lineStr[uiIter] != ',') ++uiIter;
     ++uiIter;

     if(lineStr[uiIter] != ',')
     { /*If: I have a codon number*/
        while(lineStr[uiIter] <48 && lineStr[uiIter] >57)
           ++uiIter;

        base10StrToUI(
           &lineStr[uiIter],
           amrST[*numAmrUL].codonNumUI
        ); /*Copy the codon number*/

     } /*If: I have a codon number*/

     /*They have some odd stuff in the codon number, such
     ' as blanks, ('s, and )'s; best to be safe
     */
     while(lineStr[uiIter] != ',') ++uiIter;
     ++uiIter;

     /***************************************************\
     * Fun-14 Sec-04 Sub-05:
     *   - Get the reference coordinate of the mutation
     \***************************************************/

     tmpStr = &lineStr[uiIter];

     /*Get around multi entries; they used commas*/
     for(tmpStr = tmpStr; *tmpStr < 48; ++tmpStr){}

     tmpStr =
        base10StrToUI(tmpStr, amrST[*numAmrUL].refPosUI);

     /*The next entry is the sequence, so no numbers.
     ` Numbers denote a multi entry
     */
     if(lineStr[uiIter] == '"')
     { /*If: this had multiple positions*/
        for(
           uiIter = 1 + tmpStr - lineStr;
           lineStr[uiIter] != '"';
           ++uiIter
        ){}

        uiIter += 2;
     } /*If: this had multiple positions*/

     else
     { /*Else: This entry only had a single position*/
        for(tmpStr = tmpStr; *tmpStr != ','; ++tmpStr){};
        uiIter = 1 + tmpStr - lineStr;
     } /*Else: This entry only had a single position*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-06:
     *   - Copy the reference sequence
     \***************************************************/

     /*Get the length of the reference and mutant
     ` sequence; My goal is to store both sequences in the
     ` same buffer
     */
     for(
        lenSeqUI = uiIter;
        lineStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){} /*Loop: get length of reference sequence*/

     amrST[*numAmrUL].refSeqStr =
        malloc((lenSeqUI + 1) * sizeof(char));

     if(!amrST[*numAmrUL].refSeqStr) goto variantMemErr;


     while(uiIter < lenSeqUI)
     { /*Loop: Copy the reference sequence*/
       amrST[*numAmrUL].refSeqStr[uiIter]=lineStr[uiIter];
       ++uiIter;
     } /*Loop: Copy the reference sequence*/

     amrST[*numAmrUL].refSeqStr[uiIter] = '\0';
     amrST[*numAmrUL].lenRefSeqUI = uiIter;

     ++uiIter; /*Get off ','*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-07:
     *   - Copy the mutant sequence
     \***************************************************/

     for(
        lenSeqUI = uiIter;
        lineStr[lenSeqUI] != ',';
        ++lenSeqUI
     ){} /*Loop: get length of the mutant sequence*/

     amrST[*numAmrUL].amrSeqStr =
        malloc((lenSeqUI + 1) * sizeof(char));

     if(!amrST[*numAmrUL].amrSeqStr) goto variantMemErr;

     while(uiIter < lenSeqUI)
     { /*Loop: Copy the amrerence sequence*/
       amrST[*numAmrUL].amrSeqStr[uiIter]=lineStr[uiIter];
       ++uiIter;
     } /*Loop: Copy the amrerence sequence*/

     amrST[*numAmrUL].amrSeqStr[uiIter] = '\0';
     amrST[*numAmrUL].lenAmrSeqUI = uiIter;

     ++uiIter; /*Get off ','*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-08:
     *   - See if the mutation caused an aa change
     \***************************************************/

     /*Move to the mutant amino acid*/

     /*I need to find the first base in the codon*/
     if(   lineStr[uiIter] != 'N'
        && lineStr[uiIter + 1] != 'A'
     ){ /*If: I have an amino acid change*/

        /*Get the reference aa*/
        amrST[*numAmrUL].refAaC =
          aaThreeLetterToChar(&lineStr[uiIter]);

        while(lineStr[uiIter] !=',') ++uiIter;
        ++uiIter; /*Get off the ','*/

        /*Get the mutant aa*/
        amrST[*numAmrUL].amrAaC =
          aaThreeLetterToChar(&lineStr[uiIter]);

        /*I am assuming nothing is reverse complement*/
        amrST[*numAmrUL].codonPosUI =
           amrST[*numAmrUL].refPosUI;

        if(amrST[*numAmrUL].refSeqStr[0]
           != amrST[*numAmrUL].amrSeqStr[0]
        ) ; /*Avoids label found at end of empty statement
            ` error on unbuntu
            */

        else if(
                 amrST[*numAmrUL].refSeqStr[1]
              != amrST[*numAmrUL].amrSeqStr[1]
           &&    amrST[*numAmrUL].dirFlag
              != def_amrST_revCompDir
        ) --(amrST[*numAmrUL].codonPosUI);

        else if(
                 amrST[*numAmrUL].refSeqStr[1]
              != amrST[*numAmrUL].amrSeqStr[1]
        ) ++(amrST[*numAmrUL].codonPosUI);

        else if(
             amrST[*numAmrUL].dirFlag
          != def_amrST_revCompDir
        ) amrST[*numAmrUL].codonPosUI -= 2;

        else amrST[*numAmrUL].codonPosUI += 2;
     } /*If: I have an amino acid change*/

     else
     { /*Else: No aa, move past reference aa*/
        while(lineStr[uiIter] !=',') ++uiIter;
        ++uiIter; /*Get off the ','*/
     } /*Else: No aa, move past reference aa*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-09:
     *   - Get to the first antibiotic column
     \***************************************************/

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter){}
     ++uiIter;

     /***************************************************\
     * Fun-14 Sec-04 Sub-10:
     *   - Get the remaining antibiotics
     \***************************************************/

     for(uiAmr = 0; uiAmr < 15; ++uiAmr)
     { /*Loop: Get the rating of all AMRs*/
        /*Move to the next AMR confidence column*/
        for(uiColIter = 0; uiColIter < 3; ++uiColIter)
        { /*Loop: Move to the genome_index column*/
           for(
              uiIter = uiIter;
              lineStr[uiIter] != ',';
              ++uiIter
            ){}

           ++uiIter;
        } /*Loop: Move to the genome_index column*/

        amrST[*numAmrUL].amrFlagsUL |=
           (amrIsRes(lineStr[uiIter]) << uiAmr);
     } /*Loop: Get the rating of all AMRs*/

     if(amrST[*numAmrUL].amrFlagsUL == 0)
     { /*If: this mutant has no antibiotic resitance*/
        freeAmrStructStack(&amrST[*numAmrUL]);
        continue;
     } /*If: this mutant has no antibiotic resitance*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-11:
     *   - Get the final decided position
     \***************************************************/

     /*Get of the last AMR entry*/
     for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
     ++uiIter; /*Get of the final coma*/

     /*Get off the reference entry*/
     for(uiIter=uiIter;lineStr[uiIter]!=',';++uiIter){}
     ++uiIter;

     tmpStr =
        base10StrToUI(
           &lineStr[uiIter],
           amrST[*numAmrUL].refPosUI
        );

     for(tmpStr = tmpStr; *tmpStr != ','; ++tmpStr){};
     uiIter = 1 + tmpStr - lineStr;

     /***************************************************\
     * Fun-14 Sec-04 Sub-12:
     *   - Get the final decided reference sequence
     \***************************************************/

     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final reference decision*/
        amrST[*numAmrUL].refSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final reference decision*/
      
     amrST[*numAmrUL].refSeqStr[lenSeqUI] = '\0';
     amrST[*numAmrUL].lenRefSeqUI = lenSeqUI;
     ++uiIter; /*Get off the ','*/

     /***************************************************\
     * Fun-14 Sec-04 Sub-13:
     *   - Get the final decided mutant sequence
     \***************************************************/

     lenSeqUI = 0;

     for(uiIter=uiIter; lineStr[uiIter] !=','; ++uiIter)
     { /*Loop: Copy the final mutant decision*/
        amrST[*numAmrUL].amrSeqStr[lenSeqUI] =
            lineStr[uiIter];

        ++lenSeqUI;
     } /*Loop: Copy the final mutant decision*/

     amrST[*numAmrUL].amrSeqStr[lenSeqUI] = '\0';
     amrST[*numAmrUL].lenAmrSeqUI = lenSeqUI;

     /***************************************************\
     * Fun-14 Sec-04 Sub-14:
     *   - Get the variant id
     \***************************************************/

     for(uiColIter = 0; uiColIter < 6; ++uiColIter)
     { /*Loop: To the variant column*/
        while(lineStr[uiIter] != ',') ++uiIter;
        ++uiIter;
     } /*Loop: To the variant column*/

     if(lineStr[uiIter] == ',' || lineStr[uiIter] < 33)
     { /*If: this is a blank cell*/
       amrST[*numAmrUL].lenVarIdUI = 2;
       amrST[*numAmrUL].varIdStr=malloc(3 * sizeof(char));

       if(!amrST[*numAmrUL].varIdStr){goto variantMemErr;}

       amrST[*numAmrUL].varIdStr[0] = 'N';
       amrST[*numAmrUL].varIdStr[1] = 'A';
       amrST[*numAmrUL].varIdStr[2] = '\0';
     } /*If: this is a blank cell*/

     for(
        uiColIter = uiIter;
        lineStr[uiColIter] != ',';
        ++uiColIter
     ) if(lineStr[uiColIter] == '\n') break;

     uiColIter -= uiIter;
     amrST[*numAmrUL].lenVarIdUI = uiColIter;
     ++uiColIter;

     amrST[*numAmrUL].varIdStr =
        malloc(uiColIter * sizeof(char));

     if(!amrST[*numAmrUL].varIdStr)
     { /*If: I had a memroy error*/
        variantMemErr:;

        free(*drugAryStr);
        *drugAryStr = 0;

        freeAmrStructArray(amrST, *numAmrUL);
        free(lineStr);

        return 0;
     } /*If: I had a memroy error*/

     for(
        uiColIter = 0;
        uiColIter < amrST[*numAmrUL].lenVarIdUI;
        ++uiColIter
     ) { /*Loop: Copy the variant id*/
        if(lineStr[uiIter] == '\n') break;

        amrST[*numAmrUL].varIdStr[uiColIter] =
           lineStr[uiIter];

        ++uiIter;
     } /*Loop: Copy the variant id*/

     amrST[*numAmrUL].varIdStr[uiColIter] = '\0';

     /***************************************************\
     * Fun-14 Sec-04 Sub-15:
     *   - Set the variant type
     \***************************************************/

     if(  amrST[*numAmrUL].lenAmrSeqUI
        < amrST[*numAmrUL].lenRefSeqUI
     ){ /*If: this was a deletion*/
        amrST[*numAmrUL].mutTypeStr[0] = 'd';
        amrST[*numAmrUL].mutTypeStr[1] = 'e';
        amrST[*numAmrUL].mutTypeStr[2] = 'l';
     } /*If: this was a deletion*/

     else if(
          amrST[*numAmrUL].lenAmrSeqUI
        > amrST[*numAmrUL].lenRefSeqUI
     ){ /*Else If: this was an insertion*/
        amrST[*numAmrUL].mutTypeStr[0] = 'i';
        amrST[*numAmrUL].mutTypeStr[1] = 'n';
        amrST[*numAmrUL].mutTypeStr[2] = 's';
     } /*Else If: this was an insertion*/

     else
     { /*Else: Assuming snp*/
        amrST[*numAmrUL].mutTypeStr[0] = 's';
        amrST[*numAmrUL].mutTypeStr[1] = 'n';
        amrST[*numAmrUL].mutTypeStr[2] = 'p';
     } /*Else: Assuming snp*/

     amrST[*numAmrUL].mutTypeStr[3] = '\0';

     /***************************************************\
     * Fun-14 Sec-04 Sub-16:
     *   - Move to next amr
     \***************************************************/

     ++(*numAmrUL);
     initAmrStruct(&(amrST[*numAmrUL]));
   } /*Loop: Read in all lines in the WHO's amr file*/
  
   sortAmrStructArray(amrST, 0, *numAmrUL - 1);
   free(lineStr);
   return amrST;
} /*read_2021_WhoAmrCsv*/

/*-------------------------------------------------------\
| Fun-15: checkCrossRes
|   - Check if there is cross resitance (2023 catalog)
| Input:
|   - crossStr:
|     o C-string with cross resitance to check
|   - amrSTPtr:
|     o Pionter to amrStruct structure to add corss
|       resitance to
| Output:
|   - Modifies:
|     o crosRefFlagsUL in amrSTPtr to hold the cross
|       resitance results
\-------------------------------------------------------*/
#define checkCrossRes(crossStr, amrSTPtr){\
   if(*(crossStr) < 32) ;\
   \
   else if(!cStrEql("FQ X-R\t", (crossStr), '\t'))\
      (amrSTPtr)->crossResFlagsUL |= def_FQ_CrossResFlag;\
   \
   else if(!cStrEql("BDQ-CFZ X-R\t", (crossStr), '\t'))\
   { /*Else If: there was bedaquiline/clorfazimine*/\
      (amrSTPtr)->crossResFlagsUL |=def_BDQ_CrossResFlag;\
      (amrSTPtr)->crossResFlagsUL |=def_CFZ_CrossResFlag;\
   } /*Else If: there was bedaquiline/clorfazimine*/\
   \
   else if(!cStrEql("INH-ETO X-R\t", (crossStr), '\t'))\
   { /*Else If: there was isoniazid-ethionamide*/\
      (amrSTPtr)->crossResFlagsUL |=def_INH_CrossResFlag;\
      (amrSTPtr)->crossResFlagsUL |=def_ETO_CrossResFlag;\
   } /*Else If: there was isoniazid-ethionamide*/\
} /*checkCrossRes*/

/*-------------------------------------------------------\
| Fun-16: pCrossRes
|   - Print out cross resitance (for report, not database)
| Input:
|   - amrSTPtr:
|     o Pionter to amrStruct structure to print out cross
|       resistance for
|   - oufFILE:
|     o FIle to print the cross restance to
| Output:
|   - Prints:
|     o NA if no cross resitance
|     o drug1-drug2-...-drugn if there is any cross
|       resistance
\-------------------------------------------------------*/
#define pCrossRes(amrSTPtr, outFILE){\
   char firstPrintBl = 1;\
   \
   if((amrSTPtr)->crossResFlagsUL & def_FQ_CrossResFlag)\
   { /*If: there was cross resistance*/\
      fprintf((outFILE), "fluoroquine");\
      firstPrintBl = 0;\
   } /*If: there was cross resistance*/\
   \
   if((amrSTPtr)->crossResFlagsUL & def_BDQ_CrossResFlag)\
   { /*If: there was cross resistance*/\
      if(firstPrintBl)\
         fprintf((outFILE), "bedaquiline");\
      else\
         fprintf((outFILE), "-bedaquiline");\
      firstPrintBl = 0;\
   } /*If: there was cross resistance*/\
   \
   if((amrSTPtr)->crossResFlagsUL & def_CFZ_CrossResFlag)\
   { /*If: there was cross resistance*/\
      if(firstPrintBl)\
         fprintf((outFILE), "clofazimine");\
      else\
         fprintf((outFILE), "-clofazimine");\
      firstPrintBl = 0;\
   } /*If: there was cross resistance*/\
   \
   if((amrSTPtr)->crossResFlagsUL & def_INH_CrossResFlag)\
   { /*If: there was cross resistance*/\
      if(firstPrintBl)\
         fprintf((outFILE), "isoniazid");\
      else\
         fprintf((outFILE), "-isoniazid");\
      firstPrintBl = 0;\
   } /*If: there was cross resistance*/\
   \
   if((amrSTPtr)->crossResFlagsUL & def_ETO_CrossResFlag)\
   { /*If: there was cross resistance*/\
      if(firstPrintBl)\
         fprintf((outFILE), "ethionamide");\
      else\
         fprintf((outFILE), "-ethionmaide");\
      firstPrintBl = 0;\
   } /*If: there was cross resistance*/\
   \
   /*If there was no cross resistance*/\
   if(firstPrintBl) fprintf((outFILE), "NA");\
} /*pCrossRes*/

/*-------------------------------------------------------\
| Fun-17: read_2023_WhoAmrTsv
|   - Reads in the two tabs (as separate tsv's) and
|     converts them to an amrStructs array
| Input:
|   - whoMasterFILE:
|     o Tab one (master) of the 2023 who TB catalog (as
|       a tsv)
|   - whoIndiceFILE:
|     o Tab two (genome indices) of the 2023 who catalog
|       (as a tsv)
|   - numAmrUL:
|     o Modifed to hold the number of kept amr genes
|   - drugStr:
|     o Modified to hold the antibiotic names. Each name
|       is at most 31 characters long. To get to the next
|       name add 32.
| Output:
|   - Modifies:
|     o numAmrUL to hold the number of used amrStructs in
|       the returned amrStruct structure array
|   - Returns:
|     o An array of amrStruct structures that have the
|       resitant (grade 1 and 2) mutations
\-------------------------------------------------------*/
static amrStruct * read_2023_WhoAmrTsv(
   FILE *whoMasterFILE,  /*Master tab as tsv*/
   FILE *whoIndiceFILE,  /*genome indicie tab as tsv*/
   ulong *numAmrUL,      /*Number of amrs kept*/
   char **drugAryStr,    /*Holds antibiotics*/
   uchar *errUC         /*Reports errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-17 TOC: read_2023_WhoAmrTsv
    '   o fun-17 sec-01:
    '     - Variable declerations
    '   o fun-17 sec-02:
    '     - Get the genome coordinates
    '   o fun-17 sec-03:
    '     - Get the master file length
    '   o fun-17 sec-04:
    '     - Set up the buffers
    '   o fun-17 sec-05:
    '     - Read in the file
    '   o fun-17 sec-06:
    '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-01:
    ^   - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*The master file is sorted by the drug name, there
    ' are less than 20 drugs, so I should be ok
    */
    ushort lenBuffUS = 4096;
    char buffStr[lenBuffUS];
    int iTab = 0;
    uint uiPos = 0;

    uint lenFileUI = 0;
    uint lenPosAryUI = 0;
    char *tmpStr = 0;

    uint uiDrug = 0;         /*Antbiotic on*/
    uint drugAryLimitUI = 0; /*Number strings in drugAry*/
    char *drugOnStr = 0;     /*Index of drug working on*/

    char cpAmrBl = 0;   /*1: more than 1 indice for amr*/
    int indexI = 0;
    struct genIndice *indiceAryST = 0;

    struct amrStruct *amrST = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-02:
    ^   - Get the genome coordinates
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    indiceAryST =
       read_who2023_indiceTabTsv(
          whoIndiceFILE,
          &lenPosAryUI
       ); /*Get the genome coordiantes*/

    if(indiceAryST == 0)
    { /*If: I had a memory error*/
       *errUC = def_amrST_memError;
       return 0; /*Memory error*/
    } /*If: I had a memory error*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-03:
    ^   - Get the master file length
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    while(fgets(buffStr, lenBuffUS, whoMasterFILE))
       ++lenFileUI;

    if(lenFileUI < 3)
    { /*If: I had a memory error*/
       freeGeneIndiceAry(indiceAryST, uiElm);
       *errUC = def_amrST_memError;
       return 0; /*Memory error*/
    } /*If: I had a memory error*/

    fseek(whoMasterFILE, 0, SEEK_SET); /*Start of file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-04:
    ^   - Set up the buffers
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *drugAryStr = mallocDrugAryStr(16);
    if(*drugAryStr == 0) return 0;
    drugAryLimitUI = 16;

    /*This will be massively oversized, but it works*/
    amrST = calloc(lenPosAryUI, sizeof(struct amrStruct));

    if(amrST == 0)
    { /*If: I had a memory error*/
       free(*drugAryStr);
       *drugAryStr = 0;

       freeGeneIndiceAry(indiceAryST, uiElm);
       *errUC = def_amrST_memError;
       return 0; /*Memory error*/
    } /*If: I had a memory error*/

    *numAmrUL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-05:
    ^   - Read in the file
    ^   o fun-17 sec-05 sub-01:
    ^     - get past header, start loop, get antibiotic
    ^   o fun-17 sec-05 sub-02:
    ^     - get the gene name
    ^   o fun-17 sec-05 sub-03:
    ^     - Move past the mutation column
    ^   o fun-17 sec-05 sub-04:
    ^     - Move past the mutation column
    ^   o fun-17 sec-05 sub-05:
    ^     - Move past the mutation column
    ^   o fun-17 sec-05 sub-06:
    ^     - Get the effect column
    ^   o fun-17 sec-05 sub-07:
    ^     - Move past the genome coordinate. I will get
    ^       this form the genIndice array I made at the
    ^       end
    ^   o fun-17 sec-05 sub-08:
    ^     - Move to the next set of targets to extract
    ^   o fun-17 sec-05 sub-09:
    ^     - Check if this provides resitance
    ^   o fun-17 sec-05 sub-10:
    ^     - Get the comment entry
    ^   o fun-17 sec-05 sub-11:
    ^     - Check for cross resistance (additional grade)
    ^   o fun-17 sec-05 sub-12:
    ^     - Deal with genomic coordiantes
    ^   o fun-17 sec-05 sub-13:
    ^     - Move to the next amr entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun-17 Sec-05 Sub-01:
    *   - get past header, start loop, get antibiotic
    \****************************************************/

    /*Move past the headers*/
    drugOnStr = fgets(buffStr, lenBuffUS, whoMasterFILE);
    drugOnStr = 0;

    while(fgets(buffStr, lenBuffUS, whoMasterFILE))
    { /*Loop: Read in the file*/
       
       uiPos = 0;

       if(!drugOnStr) goto firstDrug;

       while(drugOnStr[uiPos] == buffStr[uiPos]) ++uiPos;

       if(drugOnStr[uiPos]!='\0' || buffStr[uiPos]!='\t')
       { /*If: I am on a new antibiotic*/ 
         ++uiDrug;

         if(uiDrug >= drugAryLimitUI)
         { /*If: I need to resize the drug list*/
            reallocDrugAryStr(*drugAryStr, uiDrug + 10);

            if(!(*drugAryStr)) goto memoryError;
            drugAryLimitUI += 10;
         } /*If: I need to resize the drug list*/

         firstDrug:;

         cpDrugToDrugAry(*drugAryStr,buffStr,uiDrug,'\t');

         drugOnStr =
            getDrugFromDrugAry(*drugAryStr, uiDrug);
       } /*If: I am on a new antibiotic*/ 

       amrST[*numAmrUL].amrFlagsUL =
          ((ulong) 1 << (uiDrug));

       while(buffStr[uiPos] > 31) ++uiPos;

       ++uiPos; /*Get off the tab*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-02:
       *   - get the gene name
       \*************************************************/

       amrST[*numAmrUL].lenGeneIdUI = uiPos;

       while(buffStr[amrST[*numAmrUL].lenGeneIdUI] !='\t')
          ++(amrST[*numAmrUL].lenGeneIdUI);
       
       amrST[*numAmrUL].lenGeneIdUI -= uiPos;

       amrST[*numAmrUL].geneIdStr =
          malloc(
                (amrST[*numAmrUL].lenGeneIdUI + 1)
              * sizeof(char)
          ); /*Make memory for the gene id*/

       if(!amrST[*numAmrUL].geneIdStr) goto memoryError;
       
       cCpStr(
          amrST[*numAmrUL].geneIdStr,
          &buffStr[uiPos],
          amrST[*numAmrUL].lenGeneIdUI
       );

       uiPos += amrST[*numAmrUL].lenGeneIdUI + 1;

       /*************************************************\
       * Fun-17 Sec-05 Sub-03:
       *   - Move past the mutation column
       \*************************************************/

       while(buffStr[uiPos] != '\t') ++uiPos;
       ++uiPos; /*get off the tab*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-04:
       *   - Read in the variant column
       \*************************************************/

       amrST[*numAmrUL].lenVarIdUI = uiPos;

       while(buffStr[amrST[*numAmrUL].lenVarIdUI] != '\t')
          ++(amrST[*numAmrUL].lenVarIdUI);
       
       amrST[*numAmrUL].lenVarIdUI -= uiPos;

       amrST[*numAmrUL].varIdStr =
          malloc(
                (amrST[*numAmrUL].lenVarIdUI + 1)
              * sizeof(char)
          ); /*Make memory for the var id*/

       if(!amrST[*numAmrUL].varIdStr) goto memoryError;

       cCpStr(
          amrST[*numAmrUL].varIdStr,
          &buffStr[uiPos],
          amrST[*numAmrUL].lenVarIdUI
       );

       uiPos += amrST[*numAmrUL].lenVarIdUI + 1;

       /*************************************************\
       * Fun-17 Sec-05 Sub-05:
       *   - Read past the teir column
       \*************************************************/

       while(buffStr[uiPos] != '\t') ++uiPos;
       ++uiPos; /*get off the tab*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-06:
       *   - Get the effect column
       \*************************************************/

       amrST[*numAmrUL].lenEffectUI = uiPos;

       while(buffStr[amrST[*numAmrUL].lenEffectUI] !='\t')
          ++(amrST[*numAmrUL].lenEffectUI);
       
       amrST[*numAmrUL].lenEffectUI -= uiPos;

       if(amrST[*numAmrUL].lenEffectUI == 0)
          goto skipEffect;

       amrST[*numAmrUL].effectStr =
          malloc(
                (amrST[*numAmrUL].lenEffectUI + 1)
              * sizeof(char)
          ); /*Make memory for the var id*/

       if(!amrST[*numAmrUL].effectStr) goto memoryError;
       
       cCpStr(
          amrST[*numAmrUL].effectStr,
          &buffStr[uiPos],
          amrST[*numAmrUL].lenEffectUI
       );

       /*Convert spaces to dashes*/
       tmpStr = amrST[*numAmrUL].effectStr;

       while(buffStr[uiPos] > 31)
       { /*Loop: Convert spaces to dashes*/
          *tmpStr += (-(*tmpStr == ' ') & 13);
          ++tmpStr;
          ++uiPos;
       } /*Loop: Convert spaces to dashes*/

       ++uiPos; /*Get off the tab*/

       skipEffect:;

       /*************************************************\
       * Fun-17 Sec-05 Sub-07:
       *   - Move past the genome coordinate. I will get
       *     this form the genIndice array I made at the
       *     end
       \*************************************************/

       while(buffStr[uiPos] != '\t') ++uiPos;
       ++uiPos; /*get off the tab*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-08:
       *   - Move to the next set of targets to extract
       \*************************************************/

       for(iTab = 7; iTab < 105; ++iTab)
       { /*Loop: Get past columns I am ignoring*/
          while(buffStr[uiPos] != '\t') ++uiPos;
          ++uiPos; /*Get off the tab*/
       } /*Loop: Get past columns I am ignoring*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-09:
       *   - Check if this provides resitance
       \*************************************************/

       if(buffStr[uiPos] !='1' && buffStr[uiPos] !='2')
       { /*If: their is no antibiotic resitance*/
          freeAmrStructStack(&amrST[*numAmrUL]);
          continue;
       } /*If: their is no antibiotic resitance*/

       /*Store the grade*/
       amrST[*numAmrUL].gradeC = buffStr[uiPos] - 48;

       /*Move to the next entry*/
       while(buffStr[uiPos] != '\t') ++uiPos;
       ++uiPos; /*get off the tab*/

       /*************************************************\
       * Fun-17 Sec-05 Sub-10:
       *   - Get the comment entry
       \*************************************************/

       amrST[*numAmrUL].lenCommentUI = uiPos;

       while(buffStr[amrST[*numAmrUL].lenCommentUI]!='\t')
          ++(amrST[*numAmrUL].lenCommentUI);

       amrST[*numAmrUL].lenCommentUI -= uiPos;
       
       if(amrST[*numAmrUL].lenCommentUI < 1)
          goto skipComent;

       amrST[*numAmrUL].commentStr =
          malloc(
                (amrST[*numAmrUL].lenCommentUI + 3)
              * sizeof(char)
          ); /*Make memory for the var id*/

       if(!amrST[*numAmrUL].commentStr) goto memoryError;

       ulCpStr(
          amrST[*numAmrUL].commentStr,
          &buffStr[uiPos],
          amrST[*numAmrUL].lenCommentUI
       );

       /*Convert spaces to dashes*/
       tmpStr = amrST[*numAmrUL].commentStr;

       while(buffStr[uiPos] > 31)
       { /*Loop: Convert spaces (32) to dashes*/
          *tmpStr += (-(*tmpStr == 32) & 13);
          ++tmpStr;
          ++uiPos;
       } /*Loop: Convert spaces (32) to dashes*/

       ++uiPos; /*Get off the tab*/

       skipComent:;

       /*************************************************\
       * Fun-17 Sec-05 Sun-11:
       *   - Check for cross resistance (additional grade)
       \*************************************************/

       for(iTab = 0; iTab < 4; ++iTab)
       { /*Loop: Get past columns I am ignoring*/
          while(buffStr[uiPos] != '\t') ++uiPos;
          ++uiPos; /*Get off the tab*/
       } /*Loop: Get past columns I am ignoring*/

       checkCrossRes(&buffStr[uiPos], &amrST[*numAmrUL]);

       /*************************************************\
       * Fun-17 Sec-05 Sun-12:
       *   - Deal with genomic coordiantes
       *   o fun-17 sec-05 sun-12 cat-01:
       *     - Find the matching gene indice(s)
       *   o fun-17 sec-05 sun-12 cat-02:
       *     - Check if I have another amr/start loop
       *   o fun-17 sec-05 sun-12 cat-03:
       *     - In next amr, copy the gene id
       *   o fun-17 sec-05 sun-12 cat-04:
       *     - In next amr, copy variant id
       *   o fun-17 sec-05 sun-12 cat-05:
       *     - In next amr, copy effect entry
       *   o fun-17 sec-05 sun-12 cat-06:
       *     - In next amr, copy the comment entry
       *   o fun-17 sec-05 sun-12 cat-07:
       *     - Get reference postion
       *   o fun-17 sec-05 sun-12 cat-08:
       *     - Copy the reference sequence
       *   o fun-17 sec-05 sun-12 cat-09:
       *     - Copy the amr sequence
       *   o fun-17 sec-05 sun-12 cat-10:
       *     - Check the amr mutation type
       *   o fun-17 sec-05 sun-12 cat-11:
       *     - Move to the next gene indice/dup amr
       \*************************************************/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun-17 Sec-05 Sun-12 Cat-01:
       +   - Find the matching gene indice(s)
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

       indexI =
          findGenIndiceVariant(
             indiceAryST,
             amrST[*numAmrUL].varIdStr,
             (int) lenPosAryUI
          ); /*Find the variant's gene indice*/

       if(indexI < 0)
       { /*If: I could not find the indice*/
          fprintf(
              stderr,
              "%s not found in indice sheet\n",
              amrST[*numAmrUL].varIdStr
          ); /*Let the user know it is missing*/

          freeAmrStructStack(&amrST[*numAmrUL]);
          continue;
       } /*If: I could not find the indice*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun-17 Sec-05 Sun-12 Cat-02:
       +   - Check if I have another amr/start loop
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

      cpAmrBl = 1;

       while(cpAmrBl)
       { /*Loop: Copy needed coordiantes*/
          cpAmrBl = !
             cStrEql(
                indiceAryST[indexI + 1].varIdStr,
                amrST[*numAmrUL].varIdStr,
                '\0'
             ); /*Compare strings*/

          if(cpAmrBl)
          { /*If: I have multiple amrs for this entry*/
             initAmrStruct(&amrST[*numAmrUL + 1]);

             amrST[*numAmrUL + 1].crossResFlagsUL = 
                amrST[*numAmrUL].crossResFlagsUL;

             amrST[*numAmrUL + 1].amrFlagsUL = 
                amrST[*numAmrUL].amrFlagsUL;

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun-17 Sec-05 Sun-12 Cat-03:
             +   - In next amr, copy the gene id
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrST[*numAmrUL + 1].geneIdStr = 
               malloc(
                   (amrST[*numAmrUL].lenGeneIdUI + 1)
                 * sizeof(char)
               ); /*allocate memory for the sequence*/

             if(!amrST[*numAmrUL + 1].geneIdStr)
                goto memoryError;

             amrST[*numAmrUL + 1].lenGeneIdUI = 
                amrST[*numAmrUL].lenGeneIdUI;

             cCpStr(
                amrST[*numAmrUL + 1].geneIdStr,
                amrST[*numAmrUL].geneIdStr,
                amrST[*numAmrUL].lenGeneIdUI
             ); /*Copy the reference sequence*/

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun-17 Sec-05 Sun-12 Cat-04:
             +   - In next amr, copy variant id
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrST[*numAmrUL + 1].varIdStr = 
               malloc(
                   (amrST[*numAmrUL].lenVarIdUI + 1)
                 * sizeof(char)
               ); /*allocate memory for the sequence*/

             if(!amrST[*numAmrUL + 1].varIdStr)
                goto memoryError;

             amrST[*numAmrUL + 1].lenVarIdUI = 
                amrST[*numAmrUL].lenVarIdUI;

             cCpStr(
                amrST[*numAmrUL + 1].varIdStr,
                amrST[*numAmrUL].varIdStr,
                amrST[*numAmrUL].lenVarIdUI
             ); /*Copy the reference sequence*/

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun-17 Sec-05 Sun-12 Cat-05:
             +   - In next amr, copy effect entry
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrST[*numAmrUL + 1].effectStr = 
               malloc(
                   (amrST[*numAmrUL].lenEffectUI + 1)
                 * sizeof(char)
               ); /*allocate memory for the sequence*/

             if(!amrST[*numAmrUL + 1].effectStr)
                goto memoryError;

             amrST[*numAmrUL + 1].lenEffectUI = 
                amrST[*numAmrUL].lenEffectUI;

             cCpStr(
                amrST[*numAmrUL + 1].effectStr,
                amrST[*numAmrUL].effectStr,
                amrST[*numAmrUL].lenEffectUI
             ); /*Copy the reference sequence*/

             /*++++++++++++++++++++++++++++++++++++++++++\
             + Fun-17 Sec-05 Sun-12 Cat-06:
             +   - In next amr, copy the comment entry
             \++++++++++++++++++++++++++++++++++++++++++*/

             amrST[*numAmrUL + 1].lenCommentUI = 
                amrST[*numAmrUL].lenCommentUI;

             amrST[*numAmrUL + 1].commentStr = 
               malloc(
                   (amrST[*numAmrUL].lenCommentUI + 1)
                 * sizeof(char)
               ); /*allocate memory for the sequence*/

             if(!amrST[*numAmrUL + 1].commentStr)
                goto memoryError;

             cCpStr(
                amrST[*numAmrUL + 1].commentStr,
                amrST[*numAmrUL].commentStr,
                amrST[*numAmrUL].lenCommentUI
             ); /*Copy the reference sequence*/
          } /*If: I have multiple amrs for this entry*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun-17 Sec-05 Sun-12 Cat-07:
          +   - Copy the reference position
          \+++++++++++++++++++++++++++++++++++++++++++++*/

           amrST[*numAmrUL].refPosUI =
             indiceAryST[indexI].posUI;

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun-17 Sec-05 Sun-12 Cat-08:
          +   - Copy the reference sequence
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          amrST[*numAmrUL].refSeqStr = 
            malloc(
                (indiceAryST[indexI].lenRefSeqUI + 1)
              * sizeof(char)
            ); /*allocate memory for the sequence*/

          if(!amrST[*numAmrUL].refSeqStr)
             goto memoryError;

          amrST[*numAmrUL].lenRefSeqUI = 
             indiceAryST[indexI].lenRefSeqUI;

          cCpStr(
             amrST[*numAmrUL].refSeqStr,
             indiceAryST[indexI].refSeqStr,
             amrST[*numAmrUL].lenRefSeqUI
          ); /*Copy the reference sequence*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun-17 Sec-05 Sun-12 Cat-09:
          +   - Copy the amr sequence
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          amrST[*numAmrUL].amrSeqStr = 
            malloc(
                (indiceAryST[indexI].lenAmrSeqUI + 1)
              * sizeof(char)
            ); /*allocate memory for the sequence*/

          if(!amrST[*numAmrUL].amrSeqStr)
             goto memoryError;

          amrST[*numAmrUL].lenAmrSeqUI = 
             indiceAryST[indexI].lenAmrSeqUI;

          cCpStr(
             amrST[*numAmrUL].amrSeqStr,
             indiceAryST[indexI].amrSeqStr,
             amrST[*numAmrUL].lenAmrSeqUI
          ); /*Copy the amrerence sequence*/

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun-17 Sec-05 Sun-12 Cat-10:
          +   - Check the amr mutation type
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          uiPos = amrST[*numAmrUL].lenVarIdUI;

          if(     (amrST[*numAmrUL].varIdStr[uiPos-1]&~32)
                == 'L'
             &&
                  (amrST[*numAmrUL].varIdStr[uiPos-2]&~32)
                == 'O'
             &&
                  (amrST[*numAmrUL].varIdStr[uiPos-3]&~32)
                == 'F'
          ){ /*If: this was a loss of function mutation*/
             amrST[*numAmrUL].mutTypeStr[0] = 'l';
             amrST[*numAmrUL].mutTypeStr[1] = 'o';
             amrST[*numAmrUL].mutTypeStr[2] = 'f';
          } /*If: this was a loss of function mutation*/

          else if(
               amrST[*numAmrUL].lenRefSeqUI
             > amrST[*numAmrUL].lenAmrSeqUI
          ){ /*Else If: I have a deletion*/
             amrST[*numAmrUL].mutTypeStr[0] = 'd';
             amrST[*numAmrUL].mutTypeStr[1] = 'e';
             amrST[*numAmrUL].mutTypeStr[2] = 'l';
          } /*Else If: I have a deletion*/

          else if(
               amrST[*numAmrUL].lenRefSeqUI
             < amrST[*numAmrUL].lenAmrSeqUI
          ){ /*Else If: I have an insertion*/
             amrST[*numAmrUL].mutTypeStr[0] = 'i';
             amrST[*numAmrUL].mutTypeStr[1] = 'n';
             amrST[*numAmrUL].mutTypeStr[2] = 's';
          } /*Else If: I have a deletion*/

          else
          { /*Else: I have an snp*/
             amrST[*numAmrUL].mutTypeStr[0] = 's';
             amrST[*numAmrUL].mutTypeStr[1] = 'n';
             amrST[*numAmrUL].mutTypeStr[2] = 'p';
          } /*Else: I have an snp*/

          amrST[*numAmrUL].mutTypeStr[3] = '\0';

          /*+++++++++++++++++++++++++++++++++++++++++++++\
          + Fun-17 Sec-05 Sun-12 Cat-11:
          +   - Move to the next gene indice/dup amr
          \+++++++++++++++++++++++++++++++++++++++++++++*/

          *numAmrUL += cpAmrBl;
          ++indexI;
       } /*Loop: Copy needed coordiantes*/

       /*************************************************\
       * Fun-17 Sec-05 Sun-13:
       *   - Move to the next amr entry
       \*************************************************/

       ++(*numAmrUL);
    } /*Loop: Read in the file*/
    
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-17 Sec-06:
    ^   - Clean up
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    goto noError;

    memoryError:;

    *errUC = def_amrST_memError;
    freeGeneIndiceAry(indiceAryST, uiElm);
    freeAmrStructArray(amrST, *numAmrUL);

    if(*drugAryStr) free(*drugAryStr);
    *drugAryStr = 0;

    return 0;

    noError:;

    sortAmrStructArray(amrST, 0, *numAmrUL - 1);
    freeGeneIndiceAry(indiceAryST, uiElm);

    return amrST;
} /*read_2023_WhoAmrTsv*/
#endif
