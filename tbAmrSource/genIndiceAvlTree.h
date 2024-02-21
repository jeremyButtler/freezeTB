/*#######################################################\
# Name: refAvlTree
#   - Makes a AVL tree with refStructs and refLinkStructs
# Libraries:
#   - "refStruct.h"          (No .c file)
#   o "../generalLib/dataTypeShortHand.h"  (No .c file)
# C Standard Libraries:
#   - <string.h>
#   - <stdio.h>
#   o <stdlib.h>
# Note:
#   - With a little effort this can be used for a
#     different project. The rebalancing functions are all
#     macros, which can be used so long as your structure
#     has
#      - a leftST pointer for left child nodes
#      - a rightST pointer for child nodes
#      - balC (as char) for mainting balance.
#   - You will also need to modify the datatype for nodeST
#     in indiceStack (st-01) to make a stack for your
#     structure.
#   - After that all functions after fun-14 need to be
#     modified (these are structure specific)
#   - You will also have to make your own structers
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Defines and includes
'   o st-01: indiceStack
'     - Holds a stack of refStruct nodes
'   o fun-01: blankRefSTStack
'     - Blanks a indiceStack structure
'   o fun-02: pushNodeOntoStack
'     - Pushes a new node (nodeST) onto a stack of nodes
'   o fun-03: popNodeOntoStack
'     - Pops a node off a stack of nodes
'   o fun-04: getLastNodeInStack
'     - Gets the last (top) node in a stack
'   o fun-05: leftLeftRebal
'     - Does a left left rebal in an AVL tree
'   o fun-06: rightRightRebal
'     - Does a right right rebal in an AVL tree
'   o fun-07: leftRightRebal
'     - Does a left right rebal in an AVL tree
'   o fun-08: rightLeftRebal
'     - Does a right left rebal in an AVL tree
'   o fun-09: updateDepth
'     - Updates the depth of a node in an AVL tree
'   o fun-10: getBalance
'     - Gets the balance of a node in an AVL tree
'   o fun-11: adjustPtrAfterSwap
'     - Adjusts the parent pointer after doing a swap in a
'       tree rebalance
'   o fun-12: rebalRightNode
'     - Rebalances a right child node on an AVL tree
'   o fun-13: rebalLeftNode
'     - Rebalances a left child node on an AVL tree
'   o fun-14: rebalTree
'     - Rebalances an AVL tree after a node has been added
'   o fun-15: freeTree
'     - Frees a tree, but not a forest
'   o fun-16: pRefMapTbl
'     - Prints out a table of the number of reads that
'       mapped to each gene
'   o fun-17: pAmpMapTsv
'     - Prints out a table of the number of reads that
'       mapped to each combination of genes
'   o fun-18: copyStr
'     - Copys a c-string until an non-space invisible
'       character.
'   o fun-19: 
'     - Compares two strings till the first tab, newline
'       or null. This assumes the query string ends first
'   o fun-20: findGeneInTree
'     - Finds a node in a tree or if no node is present
'       makes a new node.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*------------------------------------------------------\
| Header: 
|   - Has included libraries in defined variables
\------------------------------------------------------*/

#include <stdio.h>
#include "genIndiceStruct.h"

/*-------------------------------------------------------\
| ST-01: indiceStack
|   - Holds a stack of refStruct nodes
\-------------------------------------------------------*/
typedef struct indiceStack
{ /*indiceStack*/
   struct genIndice *nodeST[1024];
      /*For an well balanced AVL tree this would be over
      ` 10^18 nodes, which is well beyond modern
      ` computing. However, I am working with forests,
      ` so I have 10x the needed space
      */

   ushort stackIndexUS;             /*Index on in stack*/
}indiceStack;

/*-------------------------------------------------------\
| Fun-01: blankRefSTStack
|   - Blanks a indiceStack structure
| Input:
|   - indiceStackST:
|     o Pionter to indiceStack structure to blank
| Output:
|   - Sets:
|     o stackIndexUS to 0
|      o nodeST[0] to 0
\-------------------------------------------------------*/
#define blankRefSTStack(indiceStackST){\
   (indiceStackST)->stackIndexUS = 0;\
   (indiceStackST)->nodeST[0] = 0;\
} /*blankRefSTStack*/

/*-------------------------------------------------------\
| Fun-02: pushNodeOntoStack
|   - Pushes a new node (nodeST) onto a stack of nodes
| Input:
|   - nodeST:
|     o Pointer to structure to put on stackAry
|   - stackST:
|     o Pointer to structure with an array of pointers
|       named nodeST and an index named stackIndexUS.
| Output:
|   - Modifies:
|     o Pushes nodeST onto stackAry
|     o Incurments stackIndex to the last item in the
|       stack (this is nodeST)
\-------------------------------------------------------*/
#define pushNodeOntoStack(newNodeST, stackST){\
   (stackST)->stackIndexUS +=\
      ((stackST)->nodeST[(stackST)->stackIndexUS] != 0);\
   (stackST)->nodeST[(stackST)->stackIndexUS] =\
      (newNodeST);\
} /*pushNodeOntoStack*/

/*-------------------------------------------------------\
| Fun-03: popNodeOntoStack
|   - Pops a node off a stack of nodes
| Input:
|   - stackST:
|     o Pointer to structure with an array of pointers
|       named nodeST and an index named stackIndexUS.
| Output:
|   - Modifies:
|     o Decurments (pops) stackIndex to the 2nd to last
|       structer
|     o Pops the last structer off of stackAry
\-------------------------------------------------------*/
#define popNodeOffStack(stackST){\
   (stackST)->nodeST[(stackST)->stackIndexUS] = 0;\
   \
   (stackST)->stackIndexUS -=\
      ((stackST)->stackIndexUS > 0);\
} /*popNodeOffStack*/

/*-------------------------------------------------------\
| Fun-04: getLastNodeInStack
|   - Gets the last (top) node in a stack
| Input:
|   - stackST:
|     o Pointer to structure with an array of pointers
|       named nodeST and an index named stackIndexUS.
| Output:
|   - Returns
|     o Pointer to last node in stack
\-------------------------------------------------------*/
#define getLastNodeInStack(stackST)(\
   (stackST)->nodeST[(stackST)->stackIndexUS]\
) /*popNodeOffStack*/

/*-------------------------------------------------------\
| Fun-05: leftLeftRebal
|   - Does a left left rebal in an AVL tree
| Input:
|   - nodeST:
|     o Pointer to nodes structure to rebal
|   - tmpST:
|      o Blank pointer to use for holding temporary
|        variables in rebalacing
| Output:
|   - Modifies:
|     o Tree at nodeST to be rebald
|   - Returns:
|     o The new top nodeST
\-------------------------------------------------------*/
#define leftLeftRebal(nodeST, tmpST)({\
     (tmpST) = (nodeST)->leftST;\
     (nodeST)->leftST = (tmpST)->rightST;\
     (tmpST)->rightST = (nodeST);\
     \
     if((nodeST)->rightST != 0)\
     { /*if have a chlld, child sets bal*/\
         (nodeST)->balC =\
            (nodeST)->rightST->balC + 1;\
         \
         (tmpST)->balC = (nodeST)->balC +1;\
     } /*if have a chlld, child sets bal*/\
     \
     else (nodeST)->balC = (tmpST)->leftST->balC + 1;\
         /*Childs bal is unchanged*/\
     \
     (tmpST);\
}) /*leftLeftRebal*/

/*-------------------------------------------------------\
| Fun-06: rightRightRebal
|   - Does a right right rebal on an AVL tree
| Input:
|   - nodeST:
|     o Pointer to nodes structure to rebal
|   - tmpST:
|      o Blank pointer to use for holding temporary
|        variables in rebalacing
| Output:
|   - Modifies:
|     o Tree at nodeST to be rebald
|   - Returns:
|     o The new top nodeST
\-------------------------------------------------------*/
#define rightRightRebal(nodeST, tmpST)({\
     (tmpST) = (nodeST)->rightST;\
     (nodeST)->rightST = (tmpST)->leftST;\
     (tmpST)->leftST = (nodeST);\
     \
     if((nodeST)->rightST != 0)\
     { /*if the paranet had a chlld, child sets bal*/\
         (nodeST)->balC =\
            (nodeST)->rightST->balC + 1;\
         (tmpST)->balC = (nodeST)->balC +1;\
     } /*if the paranet had a chlld, child sets bal*/\
     \
     else\
         (nodeST)->balC =\
            (tmpST)->leftST->balC + 1;\
         /*Childs bal is unchanged*/\
     \
     (tmpST);\
}) /*rightRightRebal*/

/*-------------------------------------------------------\
| Fun-07: leftRightRebal
|   - Does a left right rebal on an AVL tree
| Input:
|   - nodeST:
|     o Pointer to nodes structure to rebal
|   - tmpST:
|      o Blank pointer to use for holding temporary
|        variables in rebalacing
|   - swapST:
|      o Another temporary structure for swaping branches
|        around (same as (tmpST))
| Output:
|   - Modifies:
|     o Tree at nodeST to be rebald
|   - Returns:
|     o The new top nodeST
\-------------------------------------------------------*/
#define leftRightRebal(nodeST, tmpST, swapST)({\
     (tmpST) = (nodeST)->leftST;\
     (swapST) = (tmpST)->rightST;\
     \
     (tmpST)->rightST = (swapST)->leftST;\
     (nodeST)->leftST = (swapST)->rightST;\
     (swapST)->rightST = (nodeST);\
     (swapST)->leftST = (tmpST);\
     \
     /*Update depths*/\
     if((nodeST)->rightST != 0)\
        (nodeST)->balC =\
           (nodeST)->rightST->balC + 1;\
     else\
        (nodeST)->balC = (swapST)->balC;\
     \
     (tmpST)->balC = (swapST)->balC;\
     ++((swapST)->balC);\
     \
     (swapST);\
}) /*leftRightRebal*/

/*-------------------------------------------------------\
| Fun-08: rightLeftRebal
|   - Does a right left rebal on an AVL tree
| Input:
|   - nodeST:
|     o Pointer to nodes structure to rebal
|   - tmpST:
|      o Blank pointer to use for holding temporary
|        variables in rebalacing
|   - swapST:
|      o Another temporary structure for swaping branches
|        around (same as (tmpST))
| Output:
|   - Modifies:
|     o Tree at nodeST to be rebald
|   - Returns:
|     o The new top nodeST
\-------------------------------------------------------*/
#define rightLeftRebal(nodeST, tmpST, swapST)({\
     (tmpST) = (nodeST)->rightST;\
     (swapST) = (tmpST)->leftST;\
     \
     (tmpST)->leftST = (swapST)->rightST;\
     (nodeST)->rightST = (swapST)->leftST;\
     (swapST)->leftST = (nodeST);\
     (swapST)->rightST = (tmpST);\
     \
     /*Update depths*/\
     if((nodeST)->leftST != 0)\
        (nodeST)->balC =\
           (nodeST)->leftST->balC + 1;\
     else\
        (nodeST)->balC = (swapST)->balC;\
     \
     (tmpST)->balC = (swapST)->balC;\
     ++((swapST)->balC);\
     \
     (swapST);\
}) /*rightLeftRebal*/

/*-------------------------------------------------------\
| Fun-09: updateDepth
|   - Updates the depth of a node in an AVL tree
| Input:
|   - nodeST:
|     o Pointer to node structure to update depth on
| Output:
|   - Modifies:
|     o balC to reflect the depth
| Note:
|   - I found that using short cuts, like balencing by
|     how the shift should change a node just resulted in
|     deeper trees. Taking time to do a proper balance is
|     important.
\-------------------------------------------------------*/
#define updateDepth(nodeST){\
    if((nodeST)->rightST != 0)\
    { /*if have right child, check if have a left child*/\
        if(nodeST)->leftST == 0)\
           (nodeST)->balC =(nodeST)->rightST->balC +1;\
        \
        else if(\
             (nodeST)->rightST->balC\
           > (nodeST)->leftST->balC\
        ) (nodeST)->balC =(nodeST)->rightST->balC +1;\
        \
        else\
           (nodeST)->balC = (nodeST)->leftST->balC + 1;\
    } /*if have right child, check if have a left child*/\
    \
    else if((nodeST)->leftST != 0) /*Left child only*/\
        (nodeST)->balC = (nodeST)->leftST->balC + 1;\
    \
    else (nodeST)->balC = 0; /*No child nodes*/\
} /*updateDepth*/

/*-------------------------------------------------------\
| Fun-10: getBalance
|   - Gets the balance of a node in an AVL tree
| Input:
|   - nodeST:
|     o Pointer to node structure to get the balance from
| Output:
|   - Returns:
|     o The balance of the node (as char)
| Note:
|   - I found that using short cuts, like balencing by
|     how the shift should change a node just resulted in
|     deeper trees. Taking time to do a proper balance is
|     important.
\-------------------------------------------------------*/
#define getBalance(nodeST)({\
    char retC = 0;\
    \
    if((nodeST)->rightST != 0)\
    { /*if have right child, check if have a left child*/\
        if((nodeST)->leftST == 0)\
           retC = (nodeST)->rightST->balC + 1;\
        \
        else\
           retC =\
                (nodeST)->rightST->balC\
              - (nodeST)->leftST->balC\
              + 1;\
    } /*if have right child, check if have a left child*/\
    \
    else if((nodeST)->leftST != 0)\
       retC = -1 * ((nodeST)->leftST->balC + 1);\
    \
    retC;\
}) /*getBalance*/

/*-------------------------------------------------------\
| Fun-11: adjustPtrAfterSwap
|   - Adjusts the parent pointer after doing a swap in a
|     tree rebalance
| Input:
|   - nodeST:
|     o Pionter to original parent node before the swap
|   - parST:
|     o Pointer to the new parent node from the swap
|   - swapST:
|     o Temporary pionter to use in swaping parST and
|       nodeST (if needed).
|   - rootST:
|     o Pionter to root of tree
| Output:
|   - Modifies:
|     o Tree to have the parent node in the correct
|       position
\-------------------------------------------------------*/
#define adjustPtrAfterSwap(\
    nodeST,  /*node before a rebalance*/\
    parST,   /*New parent node after a rebalance*/\
    swapST,  /*For making pointer swaps*/\
    stackST, /*Stack to get the parent node from*/\
    rootST   /*Root of the tree*/\
){ /*adjustPtrAfterSwap*/\
   if((nodeST) == (rootST)) /*Is a new root node*/\
      (rootST) = (parST);\
   \
   else\
   { /*Else: need to adjust parent nodes pointer*/\
      popNodeOffStack((stackST));\
      (swapST) = getLastNodeInStack(stackST);\
      \
      if((swapST)->rightST == (nodeST))\
      { /*If was a right shift or right left shift*/\
         (swapST)->rightST = (parST);\
         \
         if((swapST)->leftST == 0)\
            swapST->balC = parST->balC + 1;\
         \
         else if((parST)->balC > (swapST)->leftST->balC)\
            (swapST)->balC = (parST)->balC + 1;\
         \
         else\
            (swapST)->balC = (swapST)->leftST->balC + 1;\
      } /*If was a right shift or right left shift*/\
      \
      else\
      { /*Else was a left of left right swap*/\
         (swapST)->leftST = (parST);\
         \
         if((swapST)->rightST == 0)\
           (swapST)->balC = (parST)->balC + 1;\
         \
         else if((parST)->balC > (swapST)->rightST->balC)\
            (swapST)->balC = (parST)->balC + 1;\
         \
         else\
            (swapST)->balC = (swapST)->rightST->balC + 1;\
      } /*Else was a left of left right swap*/\
   } /*Else need to adjust the parent nodes pointer*/\
} /*adjustPtrAfterSwap*/

/*-------------------------------------------------------\
| Fun-12: rebalRightNode
|   - Rebalances a right child node on an AVL tree
| Input:
|   - rootST:
|     o Root node of the tree
|   - balC:
|     o Balance of the node doing a swap for
|   - nodeST:
|     o Pointer to refStruct node in tree to rebalance
|   - tmpST:
|      o refStruct pointer to use for holding temporary
|        variables in rebalacing
|   - swapST:
|      o Another temporary structure for swaping branches
|        around (same as (tmpST))
|   - refStackST:
|     o Pointer to refStack structure with the search
|       history (nodes to rebalance)
| Output:
|   - Modifies:
|     o AVL tree to be rebalanced
\-------------------------------------------------------*/
#define rebalRightNode(\
   rootST,\
   balC,\
   nodeST,\
   tmpST,\
   swapST,\
   stackST\
){\
    if(getBalance((nodeST)->rightST) < 0)\
      (swapST)=rightLeftRebal((nodeST),(tmpST),(swapST));\
    \
    else (swapST) = rightRightRebal((nodeST), (tmpST));\
    \
    adjustPtrAfterSwap(\
       (nodeST),  /*Old parent node*/\
       (swapST),  /*New parent node*/\
       (tmpST),   /*For swaping nodes*/\
       (stackST),\
       (rootST)\
    ); /*Adjust pointers after swap*/\
    \
    popNodeOffStack((stackST)); /*Move to next node*/\
    (swapST) = getLastNodeInStack((stackST));\
    \
    while((swapST))\
    { /*Loop: Update all node depths*/\
       if((swapST)->leftST == 0)\
          (swapST)->balC = (swapST)->rightST->balC + 1;\
       \
       else if((swapST)->rightST == 0)\
           (swapST)->balC = (swapST)->leftST->balC + 1;\
       \
       else if(\
         (swapST)->leftST->balC < (swapST)->rightST->balC\
       ) (swapST)->balC = (swapST)->rightST->balC + 1;\
       \
       else (swapST)->balC = (swapST)->leftST->balC + 1;\
       \
       popNodeOffStack((stackST)); /*Move to next node*/\
       (swapST) = getLastNodeInStack((stackST));\
    } /*Loop: Update all node depths*/\
} /*rebalRightNode*/

/*-------------------------------------------------------\
| Fun-13: rebalLeftNode
|   - Rebalances a left child node on an AVL tree
| Input:
|   - rootST:
|     o Root node of the tree
|   - balC:
|     o Balance of the node doing a swap for
|   - nodeST:
|     o Pointer to refStruct node in tree to rebalance
|   - tmpST:
|      o refStruct pointer to use for holding temporary
|        variables in rebalacing
|   - swapST:
|      o Another temporary structure for swaping branches
|        around (same as (tmpST))
|   - refStackST:
|     o Pointer to refStack structure with the search
|       history (nodes to rebalance)
| Output:
|   - Modifies:
|     o AVL tree to be rebalanced
\-------------------------------------------------------*/
#define rebalLeftNode(\
   rootST,\
   balC,\
   nodeST,\
   tmpST,\
   swapST,\
   stackST\
){\
   if(getBalance((nodeST)->leftST) > 0)\
      (swapST)=leftRightRebal((nodeST),(tmpST),(swapST));\
   else\
      (swapST) = leftLeftRebal((nodeST), (tmpST));\
    \
    adjustPtrAfterSwap(\
       (nodeST),  /*Old parent node*/\
       (swapST),  /*New parent node*/\
       (tmpST),   /*For swaping nodes*/\
       (stackST),\
       (rootST)\
    ); /*Adjust pointers after swap*/\
   \
   popNodeOffStack((stackST)); /*Move to next node*/\
   (swapST) = getLastNodeInStack((stackST));\
   \
   /*I found that I get better balances if I keep all
   ` nodes in the tree updated. Using tricks to avoid this
   ` just resulted in poor trees
   */\
   while((swapST))\
   { /*Loop: Update all node depths*/\
       if((swapST)->rightST == 0)\
          (swapST)->balC = (swapST)->leftST->balC + 1;\
       \
       else if((swapST)->leftST == 0)\
          (swapST)->balC = (swapST)->rightST->balC + 1;\
       \
       else if(\
         (swapST)->leftST->balC > (swapST)->rightST->balC\
       ) (swapST)->balC = (swapST)->leftST->balC + 1;\
       \
       else\
          (swapST)->balC = (swapST)->rightST->balC + 1;\
       \
       popNodeOffStack((stackST)); /*Move to next node*/\
       (swapST) = getLastNodeInStack((stackST));\
   } /*Loop: Update all node depths*/\
} /*rebalLeftNode*/

/*-------------------------------------------------------\
| Fun-14: rebalTree
|   - Rebalances an AVL tree after a node has been added
| Input:
|   - rootST:
|     o Root node of tree
|   - nodeST:
|     o refStruct pointer to use in rebalancing
|   - tmpST:
|     o refStruct pointer to use for holding temporary
|       variables in rebalacing
|   - swapST:
|     o Another temporary structure for swaping branches
|       around (same as (tmpST))
|   - refStackST:
|     o Pointer to refStack structure with the search
|       history (nodes to rebalance)
| Output:
|   - Modifies:
|     o AVL tree to be rebalanced
\-------------------------------------------------------*/
#define rebalTree(rootST, nodeST, tmpST, swapST,stackST){\
   char balC = 0;  /*Balance of tree*/\
   (nodeST) = getLastNodeInStack((stackST));\
   \
   while((nodeST))\
   { /*Loop: Till have balanced every node in the tree*/\
      balC = getBalance(nodeST); /*Get bal of the node*/\
      \
      if(balC > 1)\
      { /*If: more nodes on the right side*/\
         rebalRightNode(\
            (rootST),\
            balC,\
            (nodeST),\
            (tmpST),\
            (swapST),\
            (stackST)\
         );\
      } /*If: more nodes on the right side*/\
      \
      else if(balC < -1)\
      { /*Else if: have more nodes on the left side*/\
         rebalLeftNode(\
            (rootST),\
            balC,\
            (nodeST),\
            (tmpST),\
            (swapST),\
            (stackST)\
         );\
      } /*Else if: have more nodes on the left side*/\
      \
      else if(balC < 0) /*Left child is deeper*/\
        (nodeST)->balC = (nodeST)->leftST->balC + 1;\
      \
      else if(balC > 0) /*right child is deeper*/\
        (nodeST)->balC = (nodeST)->rightST->balC + 1;\
      \
      else if((nodeST)->leftST == 0)\
        (nodeST)->balC = 0; /*Is a leaf node*/\
      \
      else /*Both childern are equally balanced*/\
        (nodeST)->balC = (nodeST)->rightST->balC + 1;\
      \
      popNodeOffStack((stackST)); /*Move to next node*/\
      (nodeST) = getLastNodeInStack((stackST));\
   } /*Loop: Till have balanced every node in the tree*/\
} /*rebalTree*/

/*-------------------------------------------------------\
| Fun-15: freeTree
|   - Frees a tree, but not a forest
| Input:
|   - rootST:
|     o Pointer to pointer to root of tree to free
| Output:
|   - Frees:
|     o All nodes in the tree
|   - Sets:
|     o rootNode to 0
\-------------------------------------------------------*/
static void freeRefForest(struct refStruct **rootST){
   if((*rootST)->leftST)
      freeRefForest(&(*rootST)->leftST);

   if((*rootST)->link)
      freeRefForest(&(*rootST)->link);

   if((*rootST)->rightST)
      freeRefForest(&(*rootST)->rightST);

   freeRefST(*rootST);
} /*freeRefForest*/

/*-------------------------------------------------------\
| Fun-18: copyStr
|   - Copys a c-string until an non-space invisible
|     character.
| Input:
|   - buffStr:
|     o Buffer to copy the string to (as c-string)
|   - cpStr:
|     o c-string with string to copy
| Output:
|   - Modifies:
|     o buffStr to hold the string in cpStr (until first
|       non-space invisible character)
\-------------------------------------------------------*/
#define copyStr(buffStr, cpStr){\
   int iChar = 0;\
   \
   for(iChar = 0; cpStr[iChar] > 31; ++iChar)\
      buffStr[iChar] = cpStr[iChar];\
   \
   buffStr[iChar] = '\0';\
} /*copyStr*/

/*-------------------------------------------------------\
| Fun-19: 
|   - Compares two strings till the first tab, newline
|     or null. This assumes the query string ends first
| Input:
|   - qryStr
|     o Query (first) string to compare
|   - refStr
|     o Reference (second) string to compare
| Output:
|   - Returns:
|     o negative number if query is less then reference
|     o 0 if the query is in the reference
|     o positive  number if the query is greater than the
|       reference
\-------------------------------------------------------*/
#define strEqual(qryStr, refStr)({\
   int iChar = 0;\
   \
   for(iChar=0; qryStr[iChar] == refStr[iChar]; ++iChar)\
      if(qryStr[iChar] < 32) {--iChar; break;}\
   \
   iChar -= (qryStr[iChar] < 32);\
   (ulong) (qryStr[iChar] - refStr[iChar]);\
}) /*strEqual*/

/*-------------------------------------------------------\
| Fun-20: findGeneInTree
|   - Finds a node in a tree or if no node is present
|     makes a new node.
| Input:
|   - refIdStr:
|     o Name of node to find (using refStruct->idStr)
|   - refTreeST:
|     o Pointer to pointer to the root of tree to search
|       for id in
| Output:
|   - Modifies:
|     o refTreeST to hold the new node if it is not in
|       the tree
|   - Returns:
|     o The node that was created/found in the tree
\-------------------------------------------------------*/
static struct refStruct * findGeneInTree(
    char * refIdStr,            /*gene id to find*/\
    struct refStruct **refTreeST /*Tree to search*/\
){ /*findGeneInTree*/
    long matchL = 0;

    struct refStruct *refST = *refTreeST;
    struct refStruct *newST = 0;
    struct indiceStack stackST;

    /*These variables are used for tree rebalancing.
    ` doing this allows me to make my macros apply
    ` to an structure with balC, rightST, and leftST
    */
    struct refStruct *tmpST = 0;
    struct refStruct *swapST = 0;

    blankRefSTStack(&stackST);

    if(*refTreeST == 0)
    { /*If there is no tree*/
        *refTreeST = makeRefST();
        copyStr((*refTreeST)->idStr, refIdStr);
        return *refTreeST;
    } /*If there is no tree*/

    while(refST != 0)
    { /*Loop: till at a leaf node or found node*/
        matchL = strEqual(refIdStr, refST->idStr);
        if(matchL == 0) return refST;

        pushNodeOntoStack(refST, &stackST);

        if(matchL < 0) refST = refST->leftST;
        else refST = refST->rightST;
    } /*Loop: till at a leaf node or found node*/

    newST = makeRefST();
    copyStr(newST->idStr, refIdStr);
    refST = getLastNodeInStack(&stackST);

    if(matchL < 0) refST->leftST = newST;
    else refST->rightST = newST;

    rebalTree(*refTreeST, refST, tmpST, swapST, &stackST);
    return newST;
} /*findGeneInTree*/

