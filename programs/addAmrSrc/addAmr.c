/*-------------------------------------------------------\
| Fun01: var_addAmr
|   - adds a variant to an AMR
| Input:
|   - varStr:
|     o variant id to process
|   - geneStr:
|     o gene variant mapped to (use 0 if in variant id)
|   - genesFileStr:
|     o c-string with path/name of file with gene
|       coordiantes
|   - refFileStr:
|     o c-string with reference to use in finishing
|       variant id
|   - errSCPtr:
|     o pointer to signed char to get returned error type
| Output:
|   - Modifies:
|     o errSCPtr to have error types:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_noVar_addAmr if variant id is empty
|   - Returns:
|     o pointer to amrST struct with added variant
|     o 0 for memory errors
\-------------------------------------------------------*/
amrST *
var_addAmr(
   signed char *varStr,       /*variant to add*/
   signed char *geneStr,      /*name of gene to add*/
   signed char *genesFileStr, /*gene coordiantes file*/
   signed char *refFileStr,   /*file with reference*/
   signed char *errSCPtr
){
   signed char errSC = 0;
   struct amrST *amrHeapST = 0;

   amrHeapST = malloc(sizeof(struct amrST));
   if(! amrHeapST)
      goto memErr_fun01_sec0x;

   amrSTPtr->lenVarIdUI =
      cpStr_ulCp(amrSTPtr->varIdStr, varStr);

   if(! amrSTPtr->lenVarIdUI)
      goto noVar_fun01_sec0x;

   goto ret_fun01_sec0x;
 
   memErr_fun01_sec0x:;
      *errSC = def_memErr_addAmr;
      goto errClean_fun01_sec0x;

   noVar_fun01_sec0x:;
      *errSC = def_memErr_addAmr;
      goto errClean_fun01_sec0x;

   errClean_fun01_sec0x:;
      if(amrHeapST)
         freeHeap_amrST(amrHeapST);
      amrHeapST = 0;
      goto ret_fun01_sec0x;

   ret_fun01_sec0x:;
      return amrHeapST;
} /*var_addAmr*/
