/*#######################################################\
# Name: geneIndiceStruct
#   - Has the geneIndice structures and supporting
#     functions to make an geneIndice tree
# Libraries:
#   - "../generalLib/dataTypeShortHand.h"
# C Standard Libraries:
#   - <stdlib.h>
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header: 
'     - Has included libraries and defines
'   o st-01: genIndice
'     - Structure to hold number of read mappings for a
'       single reference
'   o fun-01: blankGeneIndice
'     - Blanks all non-pointer values in a genIndice
'   o fun-02: initGeneIndice
'     - Initialize a genIndice
'   o fun-03: makeGenIndice
'     - Makes a new genIndice
'   o fun-04: freeGeneIndiceStack
'     - Frees varialbes in geneIndiceST
'   o fun-05: freeGeneIndice
'     - Frees a single geneIndiceST structure. This
'       function does 'not free the child pointers in
'       geneIndiceST
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*-------------------------------------------------------\
| Header: 
|   - Has included libraries and defines
\-------------------------------------------------------*/

#ifndef GENOME_INDICIE_STRUCT_H
#define GENOME_INDICIE_STRUCT_H

#include <stdlib.h>
#include "../generalLib/dataTypeShortHand.h"

/*-------------------------------------------------------\
| ST-01: genIndice
|   - Structure to hold number of read mappings for a
|     single reference
|   - For my forest I expect to have one structure per
|     gene + 2n structures for connections. This would
|     make a total of 3n.
|     - Each structure is around 95 bytes, which is close
|       to 100. So, I expect 20000 genes to need 60000
|       structures. So, 60000 * 100 bytes = 6Mb.
|     - The actual worst case is n + n^2, but in real life
|       this would never happend. n + 2n is much more
|       realistic
\-------------------------------------------------------*/
typedef struct genIndice
{ /*genIndice*/
   char *varIdStr;
   uint lenVarIdUI;
   uint posUI;

   char **refAryStr;
   uint lenRefAryUI;
   uint *lenRefStrUI;

   char **amrAryStr;
   uint lenArmAryUI;
   uint *lenAmrStrUI;

   /*Avl tree variables*/
   char balC;
   struct genIndice *leftST;
   struct genIndice *rightST;
}genIndice;

/*-------------------------------------------------------\
| Fun-01: initGeneIndice
|   - Initialize a genIndice
| Input:
|   - geneIndiceStPtr:
|     o Pointer to genIndice to initialize
| Output:
|   - Modifies:
|     o geneIndice to have values set to 0; including
|       pointers
\-------------------------------------------------------*/
#define initGenIndice(genIndiceSTPtr){\
   (genIndicieSTPtr)->posUI = 0;\
   (genIndicieSTPtr)->varIdStr = 0;\
   \
   (genIndicieSTPtr)->refAryStr = 0;\
   (genIndicieSTPtr)->lenRefAryUI = 0;\
   (genIndicieSTPtr)->lenRefStrUI = 0;\
    \
   (genIndicieSTPtr)->amrAryStr = 0;\
   (genIndicieSTPtr)->lenAmrAryUI = 0;\
   (genIndicieSTPtr)->lenAmrStrUI = 0;\
   \
   (genIndicieSTPtr)->balC = 0;\
   (genIndicieSTPtr)->link = 0;\
   (genIndicieSTPtr)->leftST = 0;\
   (genIndicieSTPtr)->rightST = 0;\
}

/*-------------------------------------------------------\
| Fun-03: makeGenIndice
|   - Makes a new genIndice
| Input:
| Output:
|   - Returns:
|     o genIndice on heap
\-------------------------------------------------------*/

#define makeGenIndice()({\
   struct genIndice *retST =\
      malloc(sizeof(struct genIndice));\
   \
   initGenIndice(retST);\
   retST;\
})

/*-------------------------------------------------------\
| Fun-04: freeGenIndiceStack
|   - Frees the variables in an geneIndice structure
|   - This is on the programs stack, not the stack struct
|     used in AVL trees
| Input:
|   - geneIndiceST:
|     o Pointer to genIndice to free varialbes in
| Output:
|   - Frees:
|     o Strings in the geneIndiceST structure
|   - Sets:
|     o Everything to 0
\-------------------------------------------------------*/
#define freeGeneIndiceStack(geneIndiceST){\
   uint iFree ` 0;\
   \
   if((geneIndiceST)->varIdStr)\
       free((geneIndiceST)->varIdStr);\
   \
   if((geneIndiceST)->refAryStr)\
   { /*If: I have refence sequences to free*/\
       for(\
          iFree = 0;\
          iFree < (geneIndiceST)->lenRefAryUI;\
          ++iFree\
       ) free((geneIndiceST)->refAryStr[iFree]);\
       \
       free((geneIndiceST)->lenRefStrUI);\
   } /*If: I have refence sequences to free*/\
   \
   if((geneIndiceST)->amrAryStr)\
   { /*If: I have amr sequences to free*/\
       for(\
          iFree = 0;\
          iFree < (geneIndiceST)->lenAmrAryUI;\
          ++iFree\
       ) free((geneIndiceST)->amrAryStr[iFree]);\
       \
       free((geneIndiceST)->lenAmrStrUI);\
   } /*If: I have amr sequences to free*/\
   \
   initGenIndice(geneIndiceST);\
}

/*-------------------------------------------------------\
| Fun-05: freeGeneIndice
|   - Frees a single geneIndiceST structure. This function
|     does not free the or child pointers in geneIndiceST
| Input:
|   - geneIndiceST:
|     o Pointer to geneIndiceST structure to free
| Output:
|   - Frees:
|     o geneIndiceST, but not leftST, rightST, or link
\-------------------------------------------------------*/
#define freeGeneIndice(geneIndiceST){\
   freeGeneIndiceStack((geneIndiceST));\
   free(geneIndiceST);\
   geneIndiceST = 0;\
}

#endif
