/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' seqToAmr SOF: Start Of File
'   - has functions to add AMRs to a tbAmr database
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "../genLib/geneCoord.h"
#include "../genLib/seqST.h"
#include "../genLib/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/dirMatrix.h"
#include "../genAln/needle.h"

#include "../tbAmr/amrST.h"

/*-------------------------------------------------------\
| Fun01: getVar_seqToAmr
|   - get variant id from sequence
| Input:
|   - refSeqStr:
|     o pointer to seqST struct with reference sequence
|       for AMR region
|   - amrSeqStr:
|     o pointer to seqST struct with query sequence for
|       AMR region
|     o sequence of AMR region
|   - startUI:
|     o position of first base in refSeqStr in reference
|   - noAaBl:
|     o 0: want amino acid sequence if in gene
|       - does not translate if target outside of gene
|     o 1: do not translate
|   - coordAryST:
|     o geneCood struct array with coordiantes and
|       direction of each gene
|   - numGenesSI:
|     o number of genes in coordAryST
|   - matrixSTPtr:
|     o pointer to a direction matrix to hold alignment
|   - alnSetPtr:
|     o alnSet structure with settings    
| Output:
|   - Returns:
|     o amrST struct pointer with new AMR sequence and
|       variant id
\-------------------------------------------------------*/
struct amrST *
getVar_seqToAmr(
   struct seqST *refSeqStr,
   struct seqST *amrSeqStr,
   unsigned int startUI,
   signed char noAaBl,
   struct geneCoord *coordAryST,
   signed int numGenesSI
   struct alnSet *alnSetPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - get variant id from sequence
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - allocate memory
   '   o fun01 sec0x:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;

   slong scoreSL = 0;
   struct amrST *amrHeapST = 0;

   struct dirMatrix *matrixHeapST = 0;
   struct samEntry samStackST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrHeapST = malloc(sizeof(struct amrST));

   if(! amrHeapST)
      goto memErr_fun01_sec0x_sub02;

   init_amrST(amrHeapST);
   init_samEntry(&samStackST);
   init_dirMatrix(matrixHeapST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - align and translate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   scoreSL =
      needle(
         qrySTPtr,
         refSTPtr,
         matrixSTPtr,    
         alnSetPtr
      );

   if(! scoreSL)
      goto memErr_fun01_sec0x_sub02;

   errSC =
      getAln_dirMatrix(
         matrixSTPtr,
         0,            /*use index from matrix*/
         0,            /*forward alignment*/
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec0x:
   ^   - clean up
   ^   o fun01 sec0x sub01:
   ^     - no error clean up
   ^   o fun01 sec0x sub02:
   ^     - memory error clean up
   ^   o fun01 sec0x sub03:
   ^     - error clean up (general; all errors call)
   ^   o fun01 sec0x sub04:
   ^     - clean up (general; everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec0x Sub01:
   *   - no error clean up
   \*****************************************************/

   /*****************************************************\
   * Fun01 Sec0x Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun01_sec0x_sub02:;
   errSC = def_memErr_addAmrDefs;
   goto errCleanUp_fun01_sec0x_sub03;

   /*****************************************************\
   * Fun01 Sec0x Sub03:
   *   - error clean up (general; all errors call)
   \*****************************************************/

   errCleanUp_fun01_sec0x_sub03:;

   if(amrHeapST)
      freeHeap_amrST(amrHeapST);

   amrHeapST = 0;

   /*****************************************************\
   * Fun01 Sec0x Sub04:
   *   - clean up (general; everything calls)
   \*****************************************************/

   cleanUp_fun01_sec0x_sub04:;

   freeStack_samEntry(samStackST);

   if(matrixHeapST)
      freeHeap_dirMatrix(matrixHeapST);

   dirMatrix = 0;

   return amrHeapST;
} /*getVar_seqToAmr*/
