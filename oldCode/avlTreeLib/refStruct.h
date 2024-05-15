/*#######################################################\
# Name: refAndPathSTs
#   - Has the reference and path structures which are
#     desigined to work together
# Libraries:
#   - "../generalLib/dataTypeShortHand.h"
# C Standard Libraries:
#   - <stdlib.h>
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header: 
'     - Has included libraries and defines
'   o st-01: refStruct
'     - Structure to hold number of read mappings for a
'       single reference
'   o fun-01: blankRefST
'     - Blanks all non-pointer values in a refStruct
'   o fun-02: initRefST
'     - Initialize a refStruct
'   o fun-03: makeRefST
'     - Makes a new refStruct
'   o fun-04: freeRefSTStack
'     - Frees non-linked list/tree varialbes in refST
'   o fun-05: freeRefST
'     - Frees a single refST structure. This function does
'       not free the refLinkStruct or child points in
'       refST, but does return the refLinkStruct in refST
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header: 
|   - Has included libraries and defines
\-------------------------------------------------------*/

#ifndef REF_AND_REFLINK_STS_H
#define REF_AND_REFLINK_STS_H

#include <stdlib.h>
#include "../generalLib/dataTypeShortHand.h"

/*So I can have circular structures*/
struct refStruct;
struct refLinkStruct;

/*-------------------------------------------------------\
| ST-01: refStruct
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
typedef struct refStruct
{ /*refStruct*/
   char idStr[64];
   ulong numPrimaryAlnUL;
   ulong totalAlnUL;

   struct refStruct *link;

   /*Avl tree variables*/
   char balC;
   struct refStruct *leftST;
   struct refStruct *rightST;
}refStruct;

/*-------------------------------------------------------\
| Fun-01: blankRefST
|   - Blanks all non-pointer values in a refStruct
| Input:
|   - refST:
|     o Pointer to refStruct to blank
| Output:
|   - Modifies:
|     o refST to have totalAlnUL set to 0, numPrimaryAryUL
|       set to 0, and the first character in idStr to be
|       '\0'
\-------------------------------------------------------*/
#define blankRefST(refST){\
   (refST)->numPrimaryAlnUL = 0;\
   (refST)->totalAlnUL = 0;\
   (refST)->idStr[0] = '\0';\
}

/*-------------------------------------------------------\
| Fun-02: initRefST
|   - Initialize a refStruct
| Input:
|   - refST:
|     o Pointer to refStruct to initialize
| Output:
|   - Modifies:
|     o refST to have values set to 0; including idStr and
|       all pointers
\-------------------------------------------------------*/
#define initRefST(refST){\
   (refST)->idStr[0] = '\0';\
   blankRefST((refST));\
   (refST)->balC = 0;\
   (refST)->link = 0;\
   (refST)->leftST = 0;\
   (refST)->rightST = 0;\
}

/*-------------------------------------------------------\
| Fun-03: makeRefST
|   - Makes a new refStruct
| Input:
| Output:
|   - Returns:
|     o refStruct on heap
\-------------------------------------------------------*/

#define makeRefST()({\
   struct refStruct *refST =\
      malloc(sizeof(struct refStruct));\
   \
   initRefST(refST);\
   refST;\
})

/*-------------------------------------------------------\
| Fun-04: freeRefSTStack
|   - Frees non-linked list/tree varialbes in refST
|   - This is on the programs stack, not the stack struct
|     used in AVL trees
| Input:
|   - refST:
|     o Pointer to refStruct to free varialbes in
| Output:
|   - Nothing:
|     o Currently does nothing. Is here for future use.
\-------------------------------------------------------*/
#define freeRefSTStack(refST) ({})

/*-------------------------------------------------------\
| Fun-05: freeRefST
|   - Frees a single refST structure. This function does
|     not free the refLinkStruct or child points in refST
| Input:
|   - refST:
|     o Pointer to refST structure to free
| Output:
|   - Frees:
|     o refST, but not leftST, rightST, or link
|   - Returns:
|     o Pointer to link (is a refLinkStruct)
\-------------------------------------------------------*/
#define freeRefST(refST)({\
   struct refStruct *linkST = (refST)->link;\
   freeRefSTStack((refST));\
   free(refST);\
   refST = 0;\
   linkST;\
})

#endif