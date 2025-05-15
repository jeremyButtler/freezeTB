/*#######################################################\
# Name: geneIndiceStruct
#   - structer and functions for reading the genome
#     indice (tab 2) of the WHO's 2023 TB catalog
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header: 
'     - included libraries
'   o .h st01: genIndice
'     - Structure to hold a single genome indice from
'       the who 2023 catalog
'   o fun01: blankGeneIndice
'     - blanks a genIndice struct
'   o fun02: init_genIndice
'     - initialize a genIndice
'   o fun03: mk_genIndice
'     - makes a new genIndice struct (on heap)
'   o fun04: freestack_genindice
'     - frees the variables in an geneIndice structure
'   o fun05: freeHeap_geneIndice
'     - frees a single geneIndiceST structure.
'   o fun06: freeHeapAry_genIndice
'     - frees an array of geneIndiceST structures
'   o fun07: swap_genIndice
'     - swaps two genIndice structures in an array
'   o fun08: sort_genIndice
'     - sorts array of genIndice structures by variant ids
'   o fun09: findVariant_genIndice
'     - finds nearest amr to input query coordiante
'   o fun10: read2023Db_genIndice
'     - reads in the genome coordinates and variants from
'       the 2023 WHO TB catalog genome indice tab (saved
'       as a tsv)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#include "genIndice.h"

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/charCp.h"

/*-------------------------------------------------------\
| Fun01: blankGeneIndice
|   - blanks a genIndice struct
| Input:
|   - geneIndiceSTPtr:
|     o pointer to genIndice to blank
| Output:
|   - Modifies:
|     o posUI in geneIndiceSTPtr to be 0
\-------------------------------------------------------*/
void
blank_genIndice(
   struct genIndice *genIndiceSTPtr
){
   genIndiceSTPtr->posUI = 0;
} /*init_genIndice*/

/*-------------------------------------------------------\
| Fun02: init_genIndice
|   - initialize a genIndice
| Input:
|   - geneIndiceStPtr:
|     o pointer to genIndice to initialize
| Output:
|   - Modifies:
|     o geneIndice to have all values set to 0
\-------------------------------------------------------*/
void
init_genIndice(
   struct genIndice *genIndiceSTPtr
){
   genIndiceSTPtr->varIdStr = 0;
   genIndiceSTPtr->lenVarIdUI = 0;

   genIndiceSTPtr->refSeqStr = 0;
   genIndiceSTPtr->lenRefSeqUI = 0;

   genIndiceSTPtr->amrSeqStr = 0;
   genIndiceSTPtr->lenAmrSeqUI = 0;

   blank_genIndice(genIndiceSTPtr);
} /*init_genIndice*/


/*-------------------------------------------------------\
| Fun03: mk_genIndice
|   - makes a new genIndice struct (on heap)
| Input:
| Output:
|   - Returns:
|     o genIndice on heap
\-------------------------------------------------------*/
struct genIndice *
mk_genIndice(
   void
){
   struct genIndice *retST = 0;
   retST = malloc(sizeof(struct genIndice));
   
   if(retST)
      init_genIndice(retST);

   return retST;
} /*mk_genIndice*/

/*-------------------------------------------------------\
| fun04: freestack_genindice
|   - frees the variables in an geneIndice structure
| Input:
|   - geneIndiceST:
|     o pointer to genIndice sturct with varialbes to free
| Output:
|   - Frees:
|     o strings in the geneIndiceST structure
|   - Sets:
|     o everything to 0
\-------------------------------------------------------*/
void
freeStack_genIndice(
   struct genIndice *geneIndiceST
){
   if(geneIndiceST)
   { /*If: have structure to free*/
      if(geneIndiceST->varIdStr)
         free(geneIndiceST->varIdStr);
      
      if(geneIndiceST->refSeqStr)
         free(geneIndiceST->refSeqStr);
      
      if(geneIndiceST->amrSeqStr)
         free(geneIndiceST->amrSeqStr);
      
      init_genIndice(geneIndiceST);
   } /*If: have structure to free*/
} /*freeStack_genIndice*/

/*-------------------------------------------------------\
| Fun05: freeHeap_geneIndice
|   - frees a single geneIndiceST structure.
| Input:
|   - geneIndiceST:
|     o pointer to geneIndiceST structure to free
| Output:
|   - Frees:
|     o geneIndiceST
\-------------------------------------------------------*/
void
freeHeap_geneIndice(
   struct genIndice *geneIndiceST
){
   if(geneIndiceST)
   { /*If: have a strucure to free*/
      freeStack_genIndice(geneIndiceST);
      free(geneIndiceST);
   } /*If: have a strucure to free*/
} /*freeHeap_geneIndice*/

/*-------------------------------------------------------\
| Fun06: freeHeapAry_genIndice
|   - frees an array of geneIndiceST structures
| Input:
|   - genIndiceST:
|     o pointer to genIndiceST structure arrray to free
|  - numElmUI:
|     o number of elements in genIndiceST
| Output:
|   - Frees:
|     o genIndiceST
|   - Sets:
|     o genIndiceST to 0
\-------------------------------------------------------*/
void
freeHeapAry_genIndice(
   struct genIndice *genIndiceST,
   unsigned int numElmUI
){
   unsigned int uiElm = 0;

   if(genIndiceST)
   { /*If: have structure to free*/
      for(
         uiElm = 0;
         uiElm < numElmUI;
         ++uiElm
      ) freeStack_genIndice(&genIndiceST[uiElm]);
      
      free(genIndiceST);
   } /*If: have structure to free*/
} /*freeHeapAry_genIndice*/

/*-------------------------------------------------------\
| Fun07: swap_genIndice
|  - swaps two genIndice structures in an array
| Input:
|  - genIndiceAry:
|    o pointer to geneIndice struct array
|  - posOne:
|    o first position (index) to swap
|  - posTwo:
|    o second position (index) to swap
| Output:
|  - Modifies:
|    o swaps first and second positions in genIndicAry
\-------------------------------------------------------*/
void
swap_genIndice(
   struct genIndice *genAry,
   unsigned int posOne,
   unsigned int posTwo
){
  signed char *swapStr = 0;

  /*positon*/

  genAry[posOne].posUI ^= genAry[posTwo].posUI;
  genAry[posTwo].posUI ^= genAry[posOne].posUI;
  genAry[posOne].posUI ^= genAry[posTwo].posUI;

  /*variant id*/

  swapStr = genAry[posOne].varIdStr;
  genAry[posOne].varIdStr = genAry[posTwo].varIdStr;
  genAry[posTwo].varIdStr = swapStr;

  genAry[posOne].lenVarIdUI ^= genAry[posTwo].lenVarIdUI;
  genAry[posTwo].lenVarIdUI ^= genAry[posOne].lenVarIdUI;
  genAry[posOne].lenVarIdUI ^= genAry[posTwo].lenVarIdUI;

  /*swap reference*/

  swapStr = genAry[posOne].refSeqStr;
  genAry[posOne].refSeqStr = (genAry)[posTwo].refSeqStr;
  genAry[posTwo].refSeqStr = swapStr;

  genAry[posOne].lenRefSeqUI ^=genAry[posTwo].lenRefSeqUI;
  genAry[posTwo].lenRefSeqUI ^=genAry[posOne].lenRefSeqUI;
  genAry[posOne].lenRefSeqUI ^=genAry[posTwo].lenRefSeqUI;

  /*swap amr sequence*/

  swapStr = genAry[posOne].amrSeqStr;
  genAry[posOne].amrSeqStr = (genAry)[posTwo].amrSeqStr;
  genAry[posTwo].amrSeqStr = swapStr;

  genAry[posOne].lenAmrSeqUI ^=genAry[posTwo].lenAmrSeqUI;
  genAry[posTwo].lenAmrSeqUI ^=genAry[posOne].lenAmrSeqUI;
  genAry[posOne].lenAmrSeqUI ^=genAry[posTwo].lenAmrSeqUI;
} /*swap_genIndice*/

/*-------------------------------------------------------\
| Fun08: sort_genIndice
|  - sorts an array of genIndice structures by variant ids
| Input:
|  - genIndiceST:
|    o pointer to genIndice structure array to sort
|  - startUI:
|    o first element sort
|  - endUI:
|    o last element to sort (index 0)
| Output:
|  - Modifies:
|    o arrays in genIndiceST to be sorted by varint id
\-------------------------------------------------------*/
void sort_genIndice(
   struct genIndice *genIndiceST,
   unsigned int startUI,
   unsigned int endUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun08 TOC: sort_genIndice
   '  - sorts an array of genIndice structs by variant ids
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun08 sec01:
   '    - variable declerations
   '  o fun08 sec02:
   '    - find the number of rounds to sort for
   '  o fun08 sec03:
   '    - sort the arrays in genIndiceST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec01:
  ^  - variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*get arrays to sort from the matrix (for sanity)*/
  
  /*variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  signed long resSL = 0;

  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun08 Sec02:
  ^  - find max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun08 Sec03:
  ^  - sort the arrays in genIndiceST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*Loop: through sub arrays*/

    for(
       ulIndex = 0;
       ulIndex <= subUL;
       ++ulIndex
    ){ /*Loop: all elements in subarray*/

      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop: sort a subarray*/
        nextElmUL = ulElm + subUL;

        resSL =
            eql_charCp(
               genIndiceST[ulElm].varIdStr,
               genIndiceST[nextElmUL].varIdStr,
               (signed char) '\0'
            );

        if( resSL > 0)
           goto firstSwap_fun08_sec03;

        else if(resSL < 0) ; /*do nothing*/
      
        else if(
             genIndiceST[ulElm].posUI
           > genIndiceST[nextElmUL].posUI
        ){ /*Else If: I need to swap an element*/
          firstSwap_fun08_sec03:;

          swap_genIndice(
             genIndiceST,
             ulElm,
             nextElmUL
          );
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*loop; move swapped element back*/
            lastElmUL -= subUL;
            
             resSL =
                 eql_charCp(
                    genIndiceST[elmOnUL].varIdStr,
                    genIndiceST[lastElmUL].varIdStr,
                    (signed char) '\0'
                 );

             if( resSL > 0)
                break;

             else if(resSL < 0) ; /*do nothing*/
      
             else if(
                  genIndiceST[elmOnUL].posUI
                > genIndiceST[lastElmUL].posUI
            ) break;
            
            swap_genIndice(
               genIndiceST,
               elmOnUL,
               lastElmUL
            ); /*swap the two indices*/
            
            elmOnUL = lastElmUL;
          } /*loop; move swapped element back*/
        } /*Else If: I need to swap elements*/
      } /*Loop: sort a subarray*/
    } /*Loop: all elements in subarray*/
    
    subUL = (subUL - 1) / 3; /*Move to the next round*/
  } /*Loop: through sub arrays*/
} /*sort_genIndice*/

/*-------------------------------------------------------\
| Fun09: findVariant_genIndice
|  - finds nearest amr to input query coordiante
| Input:
|  - genIndiceAry:
|    o genIndice struct array to find variant in
|  - qryStr:
|    o c-string with variant to search for
|  - lenArySI:
|    o lenth of the genIndice structure array (index 1)
| Output:
|  - Returns:
|    o index of the variant in the array
|    o -1 if the variant is not in the array
\-------------------------------------------------------*/
int findVariant_genIndice(
   struct genIndice *genIndiceAry,
   signed char *qryStr,
   signed int lenArySI
){
   signed int diffSI = 0;
   signed int midSI = 0;
   signed int rightSI = lenArySI - 1;
   signed int leftSI = 0;

   while(leftSI <= rightSI)
   { /*Loop: search for starting coordinate*/
      midSI = (leftSI + rightSI) >> 1;

      diffSI =
         eql_charCp(
            qryStr,
            genIndiceAry[midSI].varIdStr,
            (signed char) '\0'
         );

      if(diffSI > 0)
         leftSI = midSI + 1;

      else if(diffSI < 0)
         rightSI = midSI - 1;

     else
     { /*Else: found query*/
        foundResult_fun09:;

        while(midSI > 0)
        { /*Loop: make sure on first amr*/
           diffSI =
              eql_charCp(
                 qryStr,
                 genIndiceAry[midSI - 1].varIdStr,
                 (signed char) '\0'
              );

           if(diffSI)
              break; /*At start of variant*/

           --midSI;
        } /*Loop: make sure on first amr*/

        return midSI;
     } /*Else: found query*/
   } /*Loop: search for starting coordinate*/

   if(
      ! eql_charCp(
         qryStr,
         genIndiceAry[midSI].varIdStr,
         (signed char) '\0'
      )
   ) goto foundResult_fun09;

   return -1; /*variant is not in array*/
} /*findVariant_genIndice*/

/*-------------------------------------------------------\
| Fun10: read2023Db_genIndice
|   - reads in the genome coordinates and variants from
|     the 2023 WHO TB catalog genome indice tab (saved
|     as a tsv)
| Input:
|   - indiceStr:
|     o c-string with the path to the tsv with the
|       genome indice tab (tab 2) of the who 2023 catalog
|   - numIndicesUI:
|     o pointer to unsigned int to hold number of AMRs
|   - errSC:
|     o pointer to signed char to hold error
| Output:
|   - Modifies:
|     o numIndicesUI to have number of extracted genome
|       indices
|     o errSC to have error:
|       - 0 for no errors
|       - def_memErr_genIndice for memory errors
|       - def_fileErr_genIndice for file errors
|   - Returns:
|     o 0 for memory error or file errors
|     o pointer to an array of genIndice structures
\-------------------------------------------------------*/
struct genIndice *
read2023Db_genIndice(
   signed char *indiceStr,
   unsigned int *numIndicesUI,
   signed char *errSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC: read2023Db_genIndice
   '   - reads in the genome coordinates and variants from
   '     the 2023 WHO TB catalog genome indice tab (saved
   '     as a tsv)
   '   o fun10 Sec01:
   '     - variable declerations
   '   o fun10 Sec02:
   '     - find number of entries in file
   '   o fun10 Sec03:
   '     - read in each entry in file
   '   o fun10 Sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuffUS 4096
   signed char buffStr[lenBuffUS];

   unsigned int uiElm = 0;
      /*indice entry on (excluding header)*/
   unsigned int uiPos = 0;  /*positon at in buffStr*/

   struct genIndice *indiceHeapAryST =  0;

   FILE *indiceFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - find number of entries in file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indiceFILE =
      fopen(
         (char *) indiceStr,
         "r"
      );

   if(indiceFILE == 0)
      goto fileErr_fun10_sec04_sub03;

   *numIndicesUI = 0;

   while(
      fgets(
         (char *) buffStr,
         lenBuffUS,
         indiceFILE)
   ) ++(*numIndicesUI);

   fseek(
      indiceFILE,
      0,
      SEEK_SET
   ); /*go to start of file*/

   /*ignore header*/
   if(
      ! fgets(
         (char *) buffStr,
         lenBuffUS,
         indiceFILE
      )
   ) goto fileErr_fun10_sec04_sub03; /*nothing in file*/

   indiceHeapAryST =
      malloc(*numIndicesUI * sizeof(struct genIndice));

   if(! indiceHeapAryST)
      goto memErr_fun10_sec04_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - read in each entry in file
   ^   o fun10 sec03 sub01:
   ^     - read in the variant id (also start loop)
   ^   o fun10 sec03 sub02:
   ^     - move past reference/chromosome id
   ^   o fun10 sec03 sub03:
   ^     - get refernce position
   ^   o fun10 sec03 sub04:
   ^     - get refernce sequence
   ^   o fun10 sec03 sub05:
   ^     - get amr sequence
   ^   o fun10 sec03 sub06:
   ^     - sort indices by index
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec03 Sub01:
   *   - read in the variant id (also start loop)
   \*****************************************************/

   while(
      fgets(
         (char * ) buffStr,
         lenBuffUS,
         indiceFILE
      )
   ){ /*Loop: get each entry from the file*/
      init_genIndice(&indiceHeapAryST[uiElm]);
      uiPos = 0;

      indiceHeapAryST[uiElm].lenVarIdUI =
         lenStrNull_charCp(
            &buffStr[uiPos],
            '\t'
         );

      if(
         buffStr[uiPos+indiceHeapAryST[uiElm].lenVarIdUI]
         != '\t' 
      ) goto fileErr_fun10_sec04_sub03;

      indiceHeapAryST[uiElm].varIdStr =
         malloc(
             (indiceHeapAryST[uiElm].lenVarIdUI + 1)
           * sizeof(char)
         ); /*make variant id buffer*/

      /*check for memory errors*/
      if(! indiceHeapAryST[uiElm].varIdStr)
         goto memErr_fun10_sec04_sub02;

      cpLen_charCp(
         indiceHeapAryST[uiElm].varIdStr,
         &buffStr[uiPos],
         indiceHeapAryST[uiElm].lenVarIdUI
      ); /*copy variant id*/

      uiPos = indiceHeapAryST[uiElm].lenVarIdUI + 1;
        /*+1 to get of tab*/

      /**************************************************\
      * Fun10 Sec03 Sub02:
      *   - move past reference/chromosome id
      \**************************************************/

      /*move past the chromosome entry*/
      while(buffStr[uiPos] > 31)
         ++uiPos;

      if(buffStr[uiPos] != '\t')
         goto fileErr_fun10_sec04_sub03;

      ++uiPos; /*Get of tab*/

      /**************************************************\
      * Fun10 Sec03 Sub03:
      *   - get refernce position
      \**************************************************/

      uiPos +=
         strToUI_base10str(
           &buffStr[uiPos],
           &indiceHeapAryST[uiElm].posUI
         ); /*get genome coordiant*/

       /*convert index 1 to index 0*/
       --(indiceHeapAryST[uiElm].posUI);

      if(buffStr[uiPos] != '\t')
         goto fileErr_fun10_sec04_sub03;

      ++uiPos; /*Get of tab*/

      /**************************************************\
      * Fun10 Sec03 Sub04:
      *   - get refernce sequence
      \**************************************************/

      indiceHeapAryST[uiElm].lenRefSeqUI =
         lenStrNull_charCp(
            &buffStr[uiPos],
            '\t'
         ); /*most sequeuences are only one nucleotide*/

      if(
         buffStr[uiPos+indiceHeapAryST[uiElm].lenRefSeqUI]
         != '\t'
      ) goto fileErr_fun10_sec04_sub03;

      indiceHeapAryST[uiElm].refSeqStr =
         malloc(
             (indiceHeapAryST[uiElm].lenRefSeqUI + 1)
           * sizeof(char)
         ); /*make reference sequence buffer*/

      if(! indiceHeapAryST[uiElm].refSeqStr)
         goto memErr_fun10_sec04_sub02;

      cpLen_charCp(
         indiceHeapAryST[uiElm].refSeqStr,
         &buffStr[uiPos],
         indiceHeapAryST[uiElm].lenRefSeqUI
      ); /*copy reference sequence*/

      uiPos += indiceHeapAryST[uiElm].lenRefSeqUI + 1;
         /*+1 to get off the tab*/

      /**************************************************\
      * Fun10 Sec03 Sub05:
      *   - get amr sequence
      \**************************************************/

      indiceHeapAryST[uiElm].lenAmrSeqUI = uiPos;

      while(
         buffStr[indiceHeapAryST[uiElm].lenAmrSeqUI] > 31
      ) ++indiceHeapAryST[uiElm].lenAmrSeqUI;

      indiceHeapAryST[uiElm].lenAmrSeqUI -= uiPos;

      indiceHeapAryST[uiElm].amrSeqStr =
         malloc(
             (indiceHeapAryST[uiElm].lenAmrSeqUI + 1)
           * sizeof(char)
         ); /*make amr sequence buffer*/

      if(! indiceHeapAryST[uiElm].amrSeqStr)
         goto memErr_fun10_sec04_sub02;

      cpLen_charCp(
         indiceHeapAryST[uiElm].amrSeqStr,
         &buffStr[uiPos],
         indiceHeapAryST[uiElm].lenAmrSeqUI
      ); /*copy amr sequence*/

       ++uiElm;
   } /*Loop: get each entry from the file*/

   /*****************************************************\
   * Fun10 Sec03 Sub06:
   *   - sort indices by index
   \*****************************************************/

   /*sort the array by the variant; this is for easier
   ` lookups later
   */
   sort_genIndice(
       indiceHeapAryST,
       0,
       uiElm - 1
   );

   *numIndicesUI = uiElm;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - clean up and return
   ^   o fun10 sec04 sub01:
   ^     - no error clean up
   ^   o fun10 sec04 sub02:
   ^     - memory error clean up
   ^   o fun10 sec04 sub03:
   ^     - file error clean up
   ^   o fun10 sec04 sub04:
   ^     - error clean up (all errors call)
   ^   o fun10 sec04 sub05:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec04 Sub01:
   *   - no error clean up
   \*****************************************************/

   goto cleanUp_fun10_sec04_sub05;

   /*****************************************************\
   * Fun10 Sec04 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun10_sec04_sub02:;
   *errSC = def_memErr_genIndice;
   goto errCleanUp_fun10_sec04_sub04;

   /*****************************************************\
   * Fun10 Sec04 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_fun10_sec04_sub03:;
   *errSC = def_fileErr_genIndice;
   goto errCleanUp_fun10_sec04_sub04;

   /*****************************************************\
   * Fun10 Sec04 Sub04:
   *   - error clean up (all errors call)
   \*****************************************************/

   errCleanUp_fun10_sec04_sub04:;

   if(indiceHeapAryST)
      freeHeapAry_genIndice(
         indiceHeapAryST,
         uiElm
      );

   indiceHeapAryST = 0;


   goto cleanUp_fun10_sec04_sub05;

   /*****************************************************\
   * Fun10 Sec04 Sub05:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun10_sec04_sub05:;

   if(
         indiceFILE
      && indiceFILE != stdin
      && indiceFILE != stdout
   ) fclose(indiceFILE);

   indiceFILE = 0;

   return indiceHeapAryST;
} /*read2023Db_genIndice*/

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
