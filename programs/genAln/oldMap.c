
/*-------------------------------------------------------\
| ST02: set_mapRead
|   - settings for a aln_mapRead
\-------------------------------------------------------*/
typedef struct set_mapRead
{
   /*for kmer scaning (sort by lenArySI)*/
   signed int lenArySI[16];  /*length to swith kmer size*/
   unsigned char kmerAryUC[16]; /*kmer size per length*/
   float minKmersAryF[16];   /*% kmers needed per length*/
   signed int chainLenArySI[16]; /*minimum chain length
                                 `  to keep a chain
                                 */

   unsigned char lenKmersUC;    /*number kmer lengths*/

   float percExtraNtF;      /*% extra nt in window*/
   float percShiftF;        /*% to shift window by*/

   signed char numWinSC;    /*number windows to scan
                             `  before doing alignment
                             */
   /*for gap filling*/
   struct alnSet *alnSetST;

   /*for scoreSub_mapRead*/
   signed char subBl;     /*1: do sub-aligment score*/
   float minScoreF;       /*min % score to keep*/
   float minPercLenF;     /*min % score to keep*/

   /*this part is here to reduce memory allocation steps*/
   struct tblST_kmerFind *tblSTPtr; /*for scan/chaining*/
   unsigned char maxKmerLenUC;      /*max tbl kmer len*/
      /*keeps track of previous alignments*/
   struct refST_kmerFind *qrySTPtr; /*for scan/chaining*/
   struct chains_mapRead *chainsSTPtr; /*for chaining*/

   /*holds kmer lists needed for chaining*/
   signed int *forQryKmersArySI;/*holds query kmer list*/
   signed int *revQryKmersArySI;/*holds query kmer list*/
   signed int lenQrySI;        /*length of qryKmersArySI*/
   signed int sizeQrySI;       /*size of qryKmersArySI*/

   signed int *refKmersArySI;/*has reference kmer list*/
   signed int *refSortArySI; /*sorted ref kmer array*/
                            /*for resorting refKmerArySI*/
   signed int lenRefSI;       /*length of refKmersArySI*/
   signed int sizeRefSI;      /*size of refKmersArySI*/
}set_mapRead;

/*-------------------------------------------------------\
| ST02: ref_mapRead
|   - holds reference variables used in read mapping
\-------------------------------------------------------*/
typedef struct ref_mapRead
{
   struct seqST *seqSTPtr;      /*has reference sequence*/
   unsigned char lenKmerUC;     /*length of one kmer*/

   /*reference array cost: bytes = 12x reference length*/
   signed int *refKmerArySI;     /*reference as kmer seq*/
   signed int *refSortKmerArySI; /*kmerAry, but sorted*/
   signed int *refIndexArySI;
      /*convert refSortKmerArySI index to refKmerArySI
      `  index
      */
}ref_mapRead;

/*-------------------------------------------------------\
| Fun16: blank_set_mapRead
|   - blank a set_mapRead struct
| Input:
|   - setSTPtr:
|     o pointer to set_mapRead struct to blank
|   - quickBl:
|     o tells if doing quick blank (used kmers only)
|       * 1: quick blank; only blank kmers in tblSTPtr
|       * 0: slow blank; blank all of tblSTPtr
|   - tblSeqBlankBl:
|     o tells if blanking sequence in table
|       * 1: blank sequence in tblSTPtr
|       * 0: do not blank sequece in tblSTPtr
| Output:
|   - Modifies:
|     o blanks structs in setSTPtr
\-------------------------------------------------------*/
void
blank_set_mapRead(
   struct set_mapRead *setSTPtr,
   signed char quickBl,/*1: do quick blank of tblSTPtr*/
   signed char tblSeqBlankBl /*blank sequences*/
){
   
   if(! setSTPtr)
      return;

   if(setSTPtr->tblSTPtr)
   { /*If: have table to blank*/
      if(quickBl)
         qckBlank_tblST_kmerFind(
            setSTPtr->tblSTPtr,
            tblSeqBlankBl
         );
      else
         blank_tblST_kmerFind(
            setSTPtr->tblSTPtr,
            tblSeqBlankBl
         );
   } /*If: have table to blank*/

   if(setSTPtr->qrySTPtr)
      blank_refST_kmerFind(setSTPtr->qrySTPtr);

   if(setSTPtr->chainsSTPtr)
      blank_chains_mapRead(setSTPtr->chainsSTPtr);

} /*blank_set_mapRead*/

/*-------------------------------------------------------\
| Fun17: init_set_mapRead
|   - initializes a set_mapRead struct
| Input:
|   - setSTPtr:
|     o pointer to set_mapRead struct to initializes
| Output:
|   - Modifies:
|     o sets variables to defaults and pointers to null
\-------------------------------------------------------*/
void
init_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   setSTPtr->percExtraNtF = def_percExtraNt_defsMapRead;
   setSTPtr->percShiftF = def_percShift_defsMapRead;
   setSTPtr->numWinSC = def_winNumScan_defsMapRead;

   setSTPtr->lenArySI[0] = def_minLen_defsMapRead;
   setSTPtr->lenArySI[1] = def_secLen_defsMapRead;
   setSTPtr->lenArySI[2] = def_thirdLen_defsMapRead;
   setSTPtr->lenArySI[3] = def_fourthLen_defsMapRead;
   setSTPtr->lenArySI[4] = def_fifthLen_defsMapRead;
   setSTPtr->lenArySI[5] = def_sixthLen_defsMapRead;
   setSTPtr->lenArySI[6] = def_finalLen_defsMapRead;

   setSTPtr->kmerAryUC[0] = def_minKmer_defsMapRead;
   setSTPtr->kmerAryUC[1] = def_secKmer_defsMapRead;
   setSTPtr->kmerAryUC[2] = def_thirdKmer_defsMapRead;
   setSTPtr->kmerAryUC[3] = def_fourthKmer_defsMapRead;
   setSTPtr->kmerAryUC[4] = def_fifthKmer_defsMapRead;
   setSTPtr->kmerAryUC[5] = def_sixthKmer_defsMapRead;
   setSTPtr->kmerAryUC[6] = def_finalKmer_defsMapRead;

   setSTPtr->minKmersAryF[0] = def_minPerc_defsMapRead;
   setSTPtr->minKmersAryF[1] = def_secPerc_defsMapRead;
   setSTPtr->minKmersAryF[2] = def_thirdPerc_defsMapRead;
   setSTPtr->minKmersAryF[3] = def_fourthPerc_defsMapRead;
   setSTPtr->minKmersAryF[4] = def_fifthPerc_defsMapRead;
   setSTPtr->minKmersAryF[5] = def_sixthPerc_defsMapRead;
   setSTPtr->minKmersAryF[6] = def_finalPerc_defsMapRead;

   setSTPtr->chainLenArySI[0] = def_minChain_defsMapRead;
   setSTPtr->chainLenArySI[1] = def_secChain_defsMapRead;
   setSTPtr->chainLenArySI[2] =
        def_thirdChain_defsMapRead;
   setSTPtr->chainLenArySI[3] =
       def_fourthChain_defsMapRead;
   setSTPtr->chainLenArySI[4] =
      def_fifthChain_defsMapRead;
   setSTPtr->chainLenArySI[5] =
      def_sixthChain_defsMapRead;
   setSTPtr->chainLenArySI[6] =
      def_finalChain_defsMapRead;

   setSTPtr->lenKmersUC = 7;


   setSTPtr->subBl = def_subAln_defsMapRead;
   setSTPtr->minScoreF = def_minScore_defsMapRead;
   setSTPtr->minPercLenF = def_minPercLen_defsMapRead;

   setSTPtr->alnSetST = 0;
   setSTPtr->tblSTPtr = 0;
   setSTPtr->qrySTPtr = 0;
   setSTPtr->chainsSTPtr = 0;

   setSTPtr->forQryKmersArySI = 0;
   setSTPtr->revQryKmersArySI = 0;
   setSTPtr->lenQrySI = 0;
   setSTPtr->sizeQrySI = 0;

   setSTPtr->refKmersArySI = 0;
   setSTPtr->refSortArySI = 0;
   setSTPtr->lenRefSI = 0;
   setSTPtr->sizeRefSI = 0;

   /*currently adds nothing, but for future*/
   blank_set_mapRead(
      setSTPtr,
      0,        /*do slower blank (nothing to blank)*/
      1         /*blank sequences (none to blank)*/
   );
} /*init_set_mapRead*/

/*-------------------------------------------------------\
| Fun18: freeStack_set_mapRead
|   - frees variables in a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all pointers and intitializes; defaults/null
\-------------------------------------------------------*/
void
freeStack_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   if(setSTPtr->alnSetST)
      freeHeap_alnSet(setSTPtr->alnSetST);

   if(setSTPtr->tblSTPtr)
      freeHeap_tblST_kmerFind(setSTPtr->tblSTPtr);

   if(setSTPtr->qrySTPtr)
      freeHeap_refST_kmerFind(setSTPtr->qrySTPtr);

   if(setSTPtr->chainsSTPtr)
      freeHeap_chains_mapRead(setSTPtr->chainsSTPtr);


   if(setSTPtr->forQryKmersArySI)
      free(setSTPtr->forQryKmersArySI);

   if(setSTPtr->revQryKmersArySI)
      free(setSTPtr->revQryKmersArySI);

   if(setSTPtr->refKmersArySI)
      free(setSTPtr->refKmersArySI);

   if(setSTPtr->refSortArySI)
      free(setSTPtr->refSortArySI);

   init_set_mapRead(setSTPtr);
} /*freeStack_set_mapRead*/

/*-------------------------------------------------------\
| Fun19: freeHeap_set_mapRead
|   - frees a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer to free
| Output:
|   - Modifies:
|     o frees setSTPtr (you must set to null/0)
\-------------------------------------------------------*/
void
freeHeap_set_mapRead(
   struct set_mapRead *setSTPtr
){
   if(! setSTPtr)
      return;

   freeStack_set_mapRead(setSTPtr);
   free(setSTPtr);
} /*freeHeap_set_mapRead*/

/*-------------------------------------------------------\
| Fun20: setup_set_mapRead
|   - allocates memory for structs in a set_mapRead struct
| Input:
|   - setSTPtr:
|     o set_mapRead struct pointer to free
|   - kmerUC:
|     o size of one kmer
|   - maxRefLenSL:
|     o size of expected longest reference
| Output:
|   - Modifies:
|     o all arrays in setSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\-------------------------------------------------------*/
signed char
setup_set_mapRead(
   struct set_mapRead *setSTPtr, /*to setup*/
   unsigned char kmerUC,         /*size of one kmer*/
   signed long maxRefLenSL       /*size of longest ref*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - allocates memory for structs in set_mapRead
   '   o fun20 sec01:
   '     - setup alnset (alnsetsTPtr) struct + input check
   '   o fun20 sec02:
   '     - setup tblsT_kmero fInd (tblsTPtr) struct
   '   o fun20 sec03:
   '     - setup refsT_kmero fInd (qrysTPtr) struct
   '   o fun20 sec04:
   '     - setup chains_mapRead (chainssTPtr) struct
   '   o fun20 sec05:
   '     - setup query kmer arrays
   '   o fun20 sec06:
   '     - setup reference kmer arrays
   '   o fun20 sec07:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec01:
   ^   - setup alnSet (alnSetSTPtr) struct + input check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! setSTPtr)
      return 0;

   if(! setSTPtr->alnSetST)
   { /*If: do not have alignment settings*/

      setSTPtr->alnSetST = malloc(sizeof(struct alnSet));
      if(! setSTPtr->alnSetST)
         goto memErr_fun20_sec07;
      init_alnSet(setSTPtr->alnSetST);

   } /*If: do not have alignment settings*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec02:
   ^   - setup tblST_kmerfInd (tblSTPtr) struct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(setSTPtr->tblSTPtr)
      freeHeap_tblST_kmerFind(setSTPtr->tblSTPtr);
   setSTPtr->tblSTPtr = 0;

   setSTPtr->tblSTPtr =
      malloc(sizeof(struct tblST_kmerFind));
   if(! setSTPtr->tblSTPtr)
       goto memErr_fun20_sec07;
   init_tblST_kmerFind(setSTPtr->tblSTPtr);

   if(
      setup_tblST_kmerFind(
         setSTPtr->tblSTPtr,
         kmerUC
      )
   ) goto memErr_fun20_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec03:
   ^   - setup refST_kmerFind (qrySTPtr) struct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(setSTPtr->qrySTPtr)
      freeHeap_refST_kmerFind(setSTPtr->qrySTPtr);
   setSTPtr->qrySTPtr = 0;

   setSTPtr->qrySTPtr =
      malloc(sizeof(struct refST_kmerFind));
   if(! setSTPtr->qrySTPtr)
       goto memErr_fun20_sec07;
   init_refST_kmerFind(setSTPtr->qrySTPtr);

   if(
      setup_refST_kmerFind(
         setSTPtr->qrySTPtr,
         kmerUC
      )
   ) goto memErr_fun20_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - setup chains_mapRead (chainsSTPtr) struct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(setSTPtr->chainsSTPtr)
      freeHeap_chains_mapRead(setSTPtr->chainsSTPtr);
   setSTPtr->chainsSTPtr = 0;

   setSTPtr->chainsSTPtr =
      malloc(sizeof(struct chains_mapRead));
   if(! setSTPtr->chainsSTPtr)
      goto memErr_fun20_sec07;
   init_chains_mapRead(setSTPtr->chainsSTPtr);

   if( 
      setup_chains_mapRead(
         setSTPtr->chainsSTPtr,
         32,
         maxRefLenSL
      )
   ) goto memErr_fun20_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec05:
   ^   - setup query kmer arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(setSTPtr->forQryKmersArySI)
       free(setSTPtr->forQryKmersArySI);
   setSTPtr->forQryKmersArySI = 0;

   setSTPtr->forQryKmersArySI =
      malloc((maxRefLenSL + 8) * sizeof(signed int));
   if(! setSTPtr->forQryKmersArySI)
      goto memErr_fun20_sec07;


   if(setSTPtr->revQryKmersArySI)
       free(setSTPtr->revQryKmersArySI);
   setSTPtr->revQryKmersArySI = 0;

   setSTPtr->revQryKmersArySI =
      malloc((maxRefLenSL + 8) * sizeof(signed int));
   if(! setSTPtr->revQryKmersArySI)
      goto memErr_fun20_sec07;


   setSTPtr->lenQrySI = 0;
   setSTPtr->sizeQrySI = (signed int) maxRefLenSL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec06:
   ^   - setup reference kmer arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(setSTPtr->refKmersArySI)
       free(setSTPtr->refKmersArySI);
   setSTPtr->refKmersArySI = 0;

   setSTPtr->refKmersArySI =
      malloc((maxRefLenSL + 8) * sizeof(signed int));
   if(! setSTPtr->refKmersArySI)
      goto memErr_fun20_sec07;


   if(setSTPtr->refSortArySI)
       free(setSTPtr->refSortArySI);
   setSTPtr->refSortArySI = 0;

   setSTPtr->refSortArySI =
      malloc((maxRefLenSL + 8) * sizeof(signed int));
   if(! setSTPtr->refSortArySI)
      goto memErr_fun20_sec07;


   setSTPtr->lenRefSI = 0;
   setSTPtr->sizeRefSI = (signed int) maxRefLenSL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec07:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun20_sec07:;
      return def_memErr_mapRead;
} /*setup_set_mapRead*/



/*TODO:
`   not doing secondary prints, so mabye kmer scans to
`   find best positions, then alignment.
`   Need to think this through
*/
/*-------------------------------------------------------\
| Fun22: testAln_mapRead
|   - simpler debug case for mapping read to reference
| Input:
|   - samSTPtr:
|     o samEntry struct pointer to hold alignment
|   - qrySTPtr:
|     o seqST struct pointer with query sequence
|   - refSTPtr:
|     o seqST struct pointer with reference sequence
|   - setSTPtr:
|     o pointer to set_mapRead struct with settings
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o samSTPtr to have alignment (everything modified)
|     o errSCPtr to have error message
|       * 0 for no errors
|       * def_memErr_mapRead for no errors
|       * def_noChains_mapRead if chould not get chains
|       * def_noAln_mapRead if could not find alignment
|     o these are input to avoid making each time (reduce
|       memory allcation/deallocation), so no need to
|       check
|       * everything tblSTPtr
|       * everything in refKmerFindSTPtr
|       * everything in chainsSTPtr
|   - Returns:
|     o score for alignment
\-------------------------------------------------------*/
signed char
testAln_mapRead(
   struct samEntry *samSTPtr,     /*gets aligment*/
   struct seqST *qrySTPtr,        /*query sequence*/
   struct seqST *refSTPtr,        /*reference sequence*/
   struct set_mapRead *setSTPtr,  /*settings*/
   signed char *errSCPtr          /*gets errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun22 TOC:
   '   - maps a read to a reference
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun22 sec02:
   '     - setup table and query
   '   o fun22 sec03:
   '     - kmer scan
   '   o fun22 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char *swapSCPtr = 0;
   signed char flipBl =
      qrySTPtr->lenSeqUL > refSTPtr->lenSeqUL;
   unsigned char kmerUC = 0;
   float minKmerPercF = 0; /*min % kmers for scan*/

   signed long scoreSL = 0;
   signed long maxScoreSL = 0;
   signed long maxPossibleScoreSL = 0;

   /*holding best reference windows kmers*/
   unsigned int refStartUI = 0;

   signed long numSecSL = 0; /*number secondary alns*/

   signed long maxSL = 0; /*maxium kmers for window*/
   signed char revBl = 0; /*reverse direction best?*/
   signed char firstBl = 1; /*first kmer set*/
   signed char endBl = 0;  /*at end; terminate loop*/
   float percScoreF = 0;
   float percLenF = 0;

   signed long forKmersSL = 0;
   signed long bestForKmersSL = 0;

   signed long revKmersSL = 0;
   signed long bestRevKmersSL = 0;

   signed char winSC = 0;     /*window on*/
   signed int chainLenSI = 0; /*min chain length*/

   struct samEntry tmpSamStackST;
   struct seqST *tblSeqST = 0; /*temporary copy*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec02:
   ^   - setup table and query
   ^   o fun22 sec02 sub01:
   ^     - find best way to align sequence
   ^   o fun22 sec02 sub02:
   ^     - find kmer length for alignment
   ^   o fun22 sec02 sub03:
   ^     - table setup
   ^   o fun22 sec02 sub04:
   ^     - refST_kmerFind (query) setup
   ^   o fun22 sec02 sub05:
   ^     - reference target kmers memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun22 Sec02 Sub01:
   *   - find best way to align sequence
   \*****************************************************/

   init_samEntry(&tmpSamStackST);

   if( setup_samEntry(&tmpSamStackST) )
      goto memErr_fun22_sec04;

   flipBl = qrySTPtr->lenSeqUL > refSTPtr->lenSeqUL;

   if(flipBl)
   { /*If swaping query and reference sequence*/
      swapSCPtr = (signed char *) qrySTPtr;
      qrySTPtr = refSTPtr;
      refSTPtr = (struct seqST *) qrySTPtr;
   } /*If swaping query and reference sequence*/

   /*****************************************************\
   * Fun22 Sec02 Sub02:
   *   - find kmer length for alignment
   \*****************************************************/

   /*not effiicent, but should be nothing compared to
   `  kmer scan step
   */
   while(
         kmerUC < setSTPtr->lenKmersUC
      &&
           setSTPtr->lenArySI[kmerUC]
         < (signed int) qrySTPtr->lenSeqUL
   ) ++kmerUC;

   /*only off one index if all lengths were less than*/
   kmerUC -= (kmerUC > 0);
   minKmerPercF = setSTPtr->minKmersAryF[kmerUC];
   chainLenSI = setSTPtr->chainLenArySI[kmerUC];
   kmerUC = setSTPtr->kmerAryUC[kmerUC];
  
   setSTPtr->qrySTPtr->lenKmerUC = kmerUC;

   /*****************************************************\
   * Fun22 Sec02 Sub03:
   *   - table setup
   \*****************************************************/

   /*needed to avoid setup call, no desier to do a full
   `  blank for a 4mb array
   */

   qckBlank_tblST_kmerFind(
      setSTPtr->tblSTPtr,
      0 /*not blanking sequence*/
   ); /*blank kmertable*/

   setSTPtr->tblSTPtr->lenKmerUC = kmerUC;
   setSTPtr->tblSTPtr->kmerMaskUL= mkMask_kmerBit(kmerUC);


   errSC =
      prep_tblST_kmerFind(
         setSTPtr->tblSTPtr,     /*has table*/
         setSTPtr->percExtraNtF, /*% extra nt in window*/
         setSTPtr->percShiftF,   /*% to shift window by*/
         qrySTPtr->lenSeqUL
      ); /*prepare table for alignment*/
      /*this will remove all old sequene kmers, but not
      `  modify the kmer table. techincally blanking
      `  kmerArySI twice, but should be minimal compare
      `  to scan
      */


   tblSeqST = setSTPtr->tblSTPtr->seqSTPtr;
   setSTPtr->tblSTPtr->seqSTPtr = refSTPtr;

   /*****************************************************\
   * Fun22 Sec02 Sub04:
   *   - refST_kmerFind (query) setup
   \*****************************************************/

   /*this step also converts sequences to index's*/
   if(
     ! addSeqToRefST_kmerFind(
        setSTPtr->tblSTPtr, /*tblST_kmerFind struct*/
        setSTPtr->qrySTPtr, /*refST_kmerFind struct*/
        qrySTPtr,           /*query sequence*/
        minKmerPercF,       /*minim % kmers to align*/
        qrySTPtr->lenSeqUL, /*longest sequence*/
        setSTPtr->alnSetST  /*scoring matrix*/
     )
   ) goto memErr_fun22_sec04;

   /*****************************************************\
   * Fun22 Sec02 Sub05:
   *   - reference target kmers memory allocation
   \*****************************************************/

   /*this is to hold best window kmers, so do not loose
   `  them or have to back shift
   */

   setSTPtr->lenRefSI =
      (signed int) setSTPtr->tblSTPtr->numKmerUI;


   if(setSTPtr->sizeRefSI < setSTPtr->lenRefSI)
   { /*If: need to free arrays*/

      if(setSTPtr->refKmersArySI)
         free(setSTPtr->refKmersArySI); 
      setSTPtr->refKmersArySI = 0;


      if(setSTPtr->refSortArySI)
         free(setSTPtr->refSortArySI); 
      setSTPtr->refSortArySI = 0;


      setSTPtr->sizeRefSI = setSTPtr->lenRefSI;

   } /*If: need to free arrays*/
   

   if(! setSTPtr->refKmersArySI)
   { /*If: need to allocate memory*/
      setSTPtr->refKmersArySI =
         malloc(
             (setSTPtr->lenRefSI + 8)
           * sizeof(signed int)
         );

      if(! setSTPtr->refKmersArySI)
         goto memErr_fun22_sec04;
   } /*If: need to allocate memory*/


   if(! setSTPtr->refSortArySI)
   { /*If: need to allocate memory*/
      setSTPtr->refSortArySI =
         malloc(
             (setSTPtr->lenRefSI + 8)
           * sizeof(signed int)
         );

      if(! setSTPtr->refSortArySI)
         goto memErr_fun22_sec04;
   } /*If: need to allocate memory*/

   /*****************************************************\
   * Fun22 Sec02 Sub06:
   *   - get query kmer sequence
   \*****************************************************/

   setSTPtr->lenQrySI =
      (signed int) setSTPtr->qrySTPtr->forSeqST->lenSeqUL;

   errSC =
      addQryKmers_mapRead(
         setSTPtr->qrySTPtr->forSeqST->seqStr,
         setSTPtr->qrySTPtr->revSeqST->seqStr,
         setSTPtr->lenQrySI,
         &setSTPtr->forQryKmersArySI,
         &setSTPtr->revQryKmersArySI,
         &setSTPtr->sizeQrySI,
         kmerUC
      ); /*add query kmer sequence to arrays*/

   if(errSC)
      goto memErr_fun22_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec03:
   ^   - kmer scan
   ^   o fun22 sec03 sub01:
   ^     - get first window (chunk)
   ^   o fun22 sec03 sub02:
   ^     - get and check matchig kmer counts + start loop
   ^   o fun22 sec03 sub03:
   ^     - may have match, find best window to align
   ^   o fun22 sec03 sub04:
   ^     - align kmers
   ^   o fun22 sec03 sub05:
   ^     - move to next window
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun22 Sec03 Sub01:
   *   - get first window (chunk)
   \*****************************************************/

   endBl = 0;

   errSC =
      nextSeqChunk_tblST_kmerFind(
         setSTPtr->tblSTPtr,
         &firstBl
      );

   if(errSC)
   { /*If: have no more windows*/
      endBl = 1;
      goto kmerCount_fun22_sec03_sub0x;
   } /*If: have no more windows*/

   /*****************************************************\
   * Fun22 Sec03 Sub02:
   *   - get and check matchig kmer counts + start loop
   \*****************************************************/
   
   while(! errSC)
   { /*Loop: find alignments*/

      kmerCount_fun22_sec03_sub0x:;

      forKmersSL =
         forCntMatchs_kmerFind(
            setSTPtr->tblSTPtr,
            setSTPtr->qrySTPtr
         ); /*forward kmer count*/

      revKmersSL =
         revCntMatchs_kmerFind(
            setSTPtr->tblSTPtr,
            setSTPtr->qrySTPtr
         ); /*reverse kmer count*/


      /*realing on max here being unpredictiable, so
      `   that branch prediction is bad here
      */
      maxSL =
         max_genMath(      
            forKmersSL,
            revKmersSL
         ); /*direction with most kmers*/

      if(
           maxSL
         < (signed long) setSTPtr->qrySTPtr->minKmersUI
      ) goto nextWin_fun22_sec03_sub05;
         /*nothing here*/

      revBl = (forKmersSL < revKmersSL);

      /**************************************************\
      * Fun22 Sec03 Sub03:
      *   - may have match, find best window to align
      *   o fun22 sec03 sub03 cat01:
      *     - get first windows kmers + scores
      *   o fun22 sec03 sub03 cat02:
      *     - check if next windows are better
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub03 Cat01:
      +   - get first windows kmers + scores
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      bestForKmersSL = forKmersSL;
      bestRevKmersSL = revKmersSL;

      cpLen_ulCp(
         (signed char *) setSTPtr->refKmersArySI,
         (signed char *) setSTPtr->tblSTPtr->kmerArySI,
         (setSTPtr->tblSTPtr->numKmerUI + 1)
           * sizeof(signed int)
          /*+1 to get -2 kmer*/
      );

      /*get reference starting coordiante*/
      refStartUI = setSTPtr->tblSTPtr->seqPosUL;
      refStartUI -= setSTPtr->tblSTPtr->ntInWinUI;
         /*account for seqPosUL being end of window*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub03 Cat02:
      +   - check next windows are better
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(! errSC)
      { /*If: not at end*/

         for(
            winSC = 0;
            winSC < setSTPtr->numWinSC;
            ++winSC
         ){ /*Loop: find best overlapping window*/

            errSC =
               nextSeqChunk_tblST_kmerFind(
                  setSTPtr->tblSTPtr,
                  &firstBl
               ); /*move to next window*/


            forKmersSL =
               forCntMatchs_kmerFind(
                  setSTPtr->tblSTPtr,
                  setSTPtr->qrySTPtr
               ); /*forward kmer count*/

            revKmersSL =
               revCntMatchs_kmerFind(
                  setSTPtr->tblSTPtr,
                  setSTPtr->qrySTPtr
               ); /*reverse kmer count*/


            bestForKmersSL =
               max_genMath(      
                  forKmersSL,
                  bestForKmersSL
               );

            bestRevKmersSL =
               max_genMath(      
                  revKmersSL,
                  bestRevKmersSL
               );
  
            forKmersSL =
               max_genMath(
                  bestForKmersSL,
                  bestRevKmersSL
               );

            if(forKmersSL > maxSL)
            { /*If: new best*/
               maxSL = forKmersSL;
               revBl = (maxSL == bestRevKmersSL);

               cpLen_ulCp(
                  (signed char *) setSTPtr->refKmersArySI,
                  (signed char *)
                     setSTPtr->tblSTPtr->kmerArySI,
                  (setSTPtr->tblSTPtr->numKmerUI + 1)
                    * sizeof(signed int)
                   /*+1 to get -2 kmer*/
               );
                  
               /*get reference starting coordiante*/
               refStartUI = setSTPtr->tblSTPtr->seqPosUL;
               refStartUI-= setSTPtr->tblSTPtr->ntInWinUI;
            } /*If: new best*/

            if(errSC)
                break; /*no more windows to scan*/

         } /*Loop: find best overlapping window*/
      } /*If: not at end*/

      /**************************************************\
      * Fun22 Sec03 Sub04:
      *   - align kmers
      *   o fun22 sec03 sub04 cat01:
      *     - get chains and merge chains
      *   o fun22 sec03 sub04 cat02:
      *     - fill in merged chain gaps
      *   o fun22 sec03 sub04 cat03:
      *     - adjust coordinates, flip if needed, & score
      *   o fun22 sec03 sub04 cat04:
      *     - check if keep chain (min score + new best)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub04 Cat01:
      +   - get chains and merge chains
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(revBl)
      { /*If: reverse complement best alignment*/
         if(
            kmerChain_mapRead(
               setSTPtr->chainsSTPtr,
               chainLenSI,
               setSTPtr->revQryKmersArySI,
               setSTPtr->qrySTPtr->lenRevKmerSI,
               setSTPtr->refKmersArySI,
               setSTPtr->refSortArySI,
               setSTPtr->lenRefSI,
               (signed int) refStartUI
            )
         ) goto memErr_fun22_sec04;
      } /*If: reverse complement best alignment*/

      else
      { /*Else: forward best alignment*/
         if(
            kmerChain_mapRead(
               setSTPtr->chainsSTPtr,
               chainLenSI,
               setSTPtr->forQryKmersArySI,
               setSTPtr->qrySTPtr->lenForKmerSI,
               setSTPtr->refKmersArySI,
               setSTPtr->refSortArySI,
               setSTPtr->lenRefSI,
               (signed int) refStartUI
            )
         ) goto memErr_fun22_sec04;
      } /*Else: foward best alignment*/


      scoreSL= mergeChains_mapRead(setSTPtr->chainsSTPtr);

      if(scoreSL == def_noChains_mapRead)
         goto nextWin_fun22_sec03_sub05;
         /*no chains to merge*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub04 Cat02:
      +   - fill in merged chain gaps
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
      
      if(
         mergeToSam_mapRead(
            setSTPtr->chainsSTPtr,
            (signed int) scoreSL,
            setSTPtr->tblSTPtr,
            setSTPtr->qrySTPtr,
            qrySTPtr->qStr,
            revBl,
            chainLenSI,
            setSTPtr->alnSetST,
            &tmpSamStackST
         ) /*make merged chains into an alignment*/
      ) goto memErr_fun22_sec04;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub04 Cat03:
      +   - adjust coordinates, flip if needed, and score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/
      
      if(flipBl)
      { /*If: need to flip reference and query*/
         if(
            flipRef_mapRead(
               refSTPtr, /*is really query in this case*/
               &tmpSamStackST
            )
         ) goto memErr_fun22_sec04;

         swapSCPtr = (signed char *) refSTPtr;
         refSTPtr = qrySTPtr;
         qrySTPtr = (struct seqST *) swapSCPtr;

         if(samSTPtr->flagUS & 16)
         { /*If: is reverse complement alignment*/
            revCmpIndex_alnSet(
               qrySTPtr->seqStr,
               qrySTPtr->qStr,
               qrySTPtr->lenSeqUL
            );
         } /*If: is reverse complement alignment*/
      } /*If: need to flip reference and query*/


      scoreSL =
         scoreSub_mapRead(
            &tmpSamStackST,
            setSTPtr->subBl,  /*if doing sub-alignment*/
            refSTPtr,
            qrySTPtr,
            setSTPtr->alnSetST,
            &maxPossibleScoreSL,
            &errSC
         );

      percScoreF = (float) scoreSL;
      percScoreF /= (float) maxPossibleScoreSL;

      percLenF = (float) tmpSamStackST.numMatchUI;
      percLenF += (float) tmpSamStackST.numSnpUI;
      percLenF += (float) tmpSamStackST.numInsUI;
      percLenF /= (float) qrySTPtr->lenSeqUL;


      /*direction does not matter here*/
      if(percScoreF < setSTPtr->minScoreF)
      { /*If: read has to low of score*/
         blank_samEntry(&tmpSamStackST);
         goto nextWin_fun22_sec03_sub05;
      } /*If: read has to low of score*/

      else if(percLenF < setSTPtr->minPercLenF)
      { /*Else If: aligned to few read bases*/
         blank_samEntry(&tmpSamStackST);
         goto nextWin_fun22_sec03_sub05;
      } /*Else If: aligned to few read bases*/

    
      if(flipBl)
      { /*If: need to swap qeury and reference*/
         swapSCPtr = (signed char *) refSTPtr;
         refSTPtr = qrySTPtr;
         qrySTPtr = (struct seqST *) swapSCPtr;

         if(samSTPtr->flagUS & 16)
         { /*If: is reverse complement alignment*/
            revCmpIndex_alnSet(
               qrySTPtr->seqStr,
               qrySTPtr->qStr,
               qrySTPtr->lenSeqUL
            );
         } /*If: is reverse complement alignment*/
      } /*If: need to swap qeury and reference*/

      if(errSC)
      { /*If: had error*/
         if(errSC == def_noScore_mapRead)
            goto nextWin_fun22_sec03_sub05;
         else
            goto memErr_fun22_sec04;
      } /*If: had error*/

      if(! maxPossibleScoreSL)
         goto nextWin_fun22_sec03_sub05;
         /*should never happen, but in case*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec03 Sub04 Cat04:
      +   - check if keep chain (min score + new best)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(maxScoreSL < scoreSL)
      { /*Else If: better score*/
         ++numSecSL;

         swap_samEntry(
            &tmpSamStackST,
            samSTPtr
         );
      } /*Else If: better score*/

      else
         ++numSecSL; /*secondary alignment, not better*/

      blank_samEntry(&tmpSamStackST);

      /**************************************************\
      * Fun22 Sec03 Sub05:
      *   - move to next window
      \**************************************************/

      nextWin_fun22_sec03_sub05:;
         if(endBl)
            break;

         errSC =
            nextSeqChunk_tblST_kmerFind(
               setSTPtr->tblSTPtr,
               &firstBl
            ); /*move to next window*/
   }  /*Loop: find alignments*/

   if(! endBl)
   { /*If: on last window*/
      endBl = 1;
      goto kmerCount_fun22_sec03_sub0x;
   } /*If: on last window*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numSecSL)
      *errSCPtr = def_noAln_mapRead;
   else
      *errSCPtr = 0;

   goto ret_fun22_sec04;

   memErr_fun22_sec04:;
      *errSCPtr = def_memErr_mapRead;
      goto ret_fun22_sec04;

   ret_fun22_sec04:;
      freeStack_samEntry(&tmpSamStackST);
      setSTPtr->tblSTPtr->seqSTPtr = tblSeqST;
      return maxScoreSL;
} /*testAln_mapRead*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
| Fun14: flipRef_mapRead
|   - flips samEntry reference to query
| Input:
|   - qrySTPtr:
|     o seqST struct pointer with query sequence to add
|   - samSTPtr:
|     o samEntry struct pointer with sam entry to flip
|       * qrySTPtr must be reference mapped to in samSTPtr
| Output:
|   - Modifies:
|     o samSTPtr to have querySTPtr as sequence
|       * most variables are modified
|       * reverse complements if samSTPtr->flagUS & 16
|         - put in foward query sequence, this function
|           reverse complements the sequence copied to
|           samSTPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mapRead for memory errors
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
signed char
flipRef_mapRead(
   struct seqST *qrySTPtr,   /*query to add in*/
   struct samEntry *samSTPtr /*to flip*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - flips samEntry reference to query
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - make sure have enough memory for cigar
   '   o fun14 sec03:
   '     - find reference start from sam alignment
   '   o fun14 sec04:
   '     - add starting softmasking (if needed)
   '   o fun14 sec05:
   '     - swap cigar insertions and deletions (for query)
   '   o fun14 sec06:
   '     - add ending softmasking
   '   o fun14 sec07:
   '     - swap query and reference ids (names)
   '   o fun14 sec08:
   '     - add in query sequence (and length)
   '   o fun14 sec09:
   '     - add in query q-score entry
   '   o fun14 sec10:
   '     - reverse complement
   '   o fun14 sec11:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *swapSCPtr = 0;
   unsigned int qryStartUI = samSTPtr->refStartUI;
   signed int posSI = 0;
   signed int siCig = 0;
   unsigned long lenQryUL = 0;

   unsigned int lenRefUI = samSTPtr->readLenUI;
      /*for reverse complent step*/

   struct seqST swapSeqST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - make sure have enough memory for cigar
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    init_seqST(&swapSeqST);

    /*might have to add softmasking to ends (2 extra)*/

    if(samSTPtr->lenCigUI >= samSTPtr->lenCigBuffUI - 2)
    { /*If: need to get more memory*/
       swapSCPtr =
          realloc(
             samSTPtr->cigTypeStr,
               (samSTPtr->lenCigUI + 11)
             * sizeof(signed char)
          );
       if(! swapSCPtr)
          goto memErr_fun14_sec11;
       samSTPtr->cigTypeStr = swapSCPtr;

       swapSCPtr =
          (signed char *)
          realloc(
             samSTPtr->cigArySI,
               (samSTPtr->lenCigUI + 11)
             * sizeof(signed int)
          );
       if(! swapSCPtr)
          goto memErr_fun14_sec11;
       samSTPtr->cigArySI = (signed int *) swapSCPtr;

       samSTPtr->lenCigBuffUI += 2;
    } /*If: need to get more memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - find reference start from sam alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*softmasking marks end of alignment*/

   if(samSTPtr->cigTypeStr[0] == 'S')
      samSTPtr->refStartUI = samSTPtr->cigArySI[0];
   else
      samSTPtr->refStartUI = 0;

   samSTPtr->refEndUI = samSTPtr->refStartUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec04:
   ^   - add starting softmasking (if needed)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qryStartUI != 0)
   { /*If: have query soft masking*/

      if(samSTPtr->cigTypeStr[0] != 'S')
      { /*If: no soft mask entry at start*/
         siCig = samSTPtr->lenCigUI;

         while(siCig > 0)
         { /*Loop: shift cigar one up*/
            samSTPtr->cigTypeStr[siCig] =
               samSTPtr->cigTypeStr[siCig - 1];

            samSTPtr->cigArySI[siCig] =
               samSTPtr->cigArySI[siCig - 1];

            --siCig;
         } /*Loop: shift cigar one up*/

         samSTPtr->cigTypeStr[0] = 'S';
         ++samSTPtr->lenCigUI;
      } /*If: no soft mask entry at start*/

      samSTPtr->cigArySI[0] = qryStartUI - 1;
      lenQryUL += qryStartUI - 1;
   } /*If: have query soft masking*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec05:
   ^   - swap cigar insertions and deletions (for query)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siCig = 0;

   while(siCig < (signed int) samSTPtr->lenCigUI)
   { /*Loop: flip cigars*/

      if(samSTPtr->cigTypeStr[siCig] == 'I')
      { /*If: true reference is insertion*/
         samSTPtr->cigTypeStr[siCig] = 'D';
         samSTPtr->refEndUI +=
            samSTPtr->cigTypeStr[siCig];
      } /*If: true reference is insertion*/

      else if(samSTPtr->cigTypeStr[siCig] == 'D')
      { /*Else: reference had deletion*/
         samSTPtr->cigTypeStr[siCig] = 'I';
         lenQryUL += samSTPtr->cigTypeStr[siCig];
      } /*Else: reference had deletion*/

      else if(samSTPtr->cigTypeStr[siCig] != 'S')
         lenQryUL += samSTPtr->cigTypeStr[siCig];
         /*keep track of query length*/

      ++siCig;

   } /*Loop: flip cigars*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec06:
   ^   - add ending softmasking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   --siCig; /*get back on last entry*/

   if(lenQryUL < qrySTPtr->lenSeqUL)
   { /*If: ending soft masking*/

      if(samSTPtr->cigTypeStr[siCig] != 'S')
      { /*If: reference had no end softmasking*/
         ++siCig;
         samSTPtr->cigTypeStr[siCig] = 'S';
      } /*If: reference had no end softmasking*/

      samSTPtr->cigArySI[siCig] =
         (signed int)
         (qrySTPtr->lenSeqUL - lenQryUL);

      ++samSTPtr->lenCigUI;
   } /*If: ending soft masking*/

   else if(samSTPtr->cigTypeStr[siCig] == 'S')
      --samSTPtr->lenCigUI; /*remove entry at end*/
   
   samSTPtr->cigTypeStr[samSTPtr->lenCigUI] = '\0';
   samSTPtr->cigArySI[samSTPtr->lenCigUI] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec07:
   ^   - swap query and reference ids (names)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siCig = 0;

   /*swap read ids*/
   swapNull_ulCp(
      samSTPtr->qryIdStr, 
      samSTPtr->refIdStr
   );

   samSTPtr->lenQryIdUC ^= samSTPtr->lenRefIdUC;
   samSTPtr->lenRefIdUC ^= samSTPtr->lenQryIdUC;
   samSTPtr->lenQryIdUC ^= samSTPtr->lenRefIdUC;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec08:
   ^   - add in query sequence (and length)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qrySTPtr->lenSeqUL >= samSTPtr->lenSeqBuffUI)
   { /*If: need to get more sequence buffer memory*/
      free(samSTPtr->seqStr);
      samSTPtr->seqStr = 0;

      samSTPtr->seqStr =
         malloc(
           (qrySTPtr->lenSeqUL + 9) * sizeof(signed char)
         );
      if(! samSTPtr->seqStr)
         goto memErr_fun14_sec11;
   } /*If: need to get more sequence buffer memory*/

   cpLen_ulCp(
      samSTPtr->seqStr,
      qrySTPtr->seqStr,
      (unsigned int) qrySTPtr->lenSeqUL
   );

   samSTPtr->readLenUI= (unsigned int) qrySTPtr->lenSeqUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec09:
   ^   - add in query q-score entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qrySTPtr->lenQUL > 0)
   { /*If: have q-score entry*/

      if(samSTPtr->lenQBuffUI < qrySTPtr->lenQUL)
      { /*If: need more memory*/

         free(samSTPtr->qStr);
         samSTPtr->qStr = 0;

         samSTPtr->qStr =
            malloc(
                 (qrySTPtr->lenSeqUL + 9)
               * sizeof(unsigned char)
            );

         if(! samSTPtr)
            goto memErr_fun14_sec11;

         samSTPtr->lenSeqBuffUI =
           (unsigned int) qrySTPtr->lenSeqUL;

      } /*If: need more memory*/

      cpQEntry_samEntry(
         samSTPtr,
         qrySTPtr->qStr,
         1 /*need to blank histgram and q-score values*/
      ); /*copy q-score entries*/

   } /*If: have q-score entry*/

   else
   { /*Else: no q-socre; need to blank*/

      samSTPtr->medianQF = 0;
      samSTPtr->meanQF = 0;
      samSTPtr->sumQUL = 0;
      samSTPtr->qStr[0] = '*';
      samSTPtr->qStr[1] = '\0';

      siCig = 0;
      while(siCig < def_maxQ_samEntry)
         samSTPtr->qHistUI[siCig++] = 0;

   } /*Else: no q-score; need to blank*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec10:
   ^   - reverse complement
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! (samSTPtr->flagUS & 16) )
      goto ret_fun14_sec11;
      /*if was a foward alignment*/
  
   /*adjust reference start and end coordinates*/
   lenRefUI = samSTPtr->refEndUI - samSTPtr->refStartUI;
   samSTPtr->refStartUI = lenRefUI - samSTPtr->refEndUI;
   samSTPtr->refEndUI = samSTPtr->refStartUI + lenQryUL;

   siCig = (signed int) samSTPtr->lenCigUI - 1;
   posSI = 0;

   while(posSI < siCig)
   { /*Loop: invert cigar*/

      /*swap cigar entries*/
      samSTPtr->cigTypeStr[siCig] ^=
         samSTPtr->cigTypeStr[posSI];
      samSTPtr->cigTypeStr[posSI] ^=
         samSTPtr->cigTypeStr[siCig];
      samSTPtr->cigTypeStr[siCig] ^=
         samSTPtr->cigTypeStr[posSI];

      samSTPtr->cigArySI[siCig] ^=
         samSTPtr->cigArySI[posSI];
      samSTPtr->cigArySI[posSI] ^=
         samSTPtr->cigArySI[siCig];
      samSTPtr->cigArySI[siCig] ^=
         samSTPtr->cigArySI[posSI];

      ++siCig;
      ++posSI;

   } /*Loop: invert cigar*/

   /*this is a bit lazy, but it works*/
   swapSeqST.seqStr = samSTPtr->seqStr;
   swapSeqST.lenSeqUL = samSTPtr->readLenUI;
   swapSeqST.qStr = samSTPtr->qStr;

   revComp_seqST(&swapSeqST);

   swapSeqST.seqStr = 0;
   swapSeqST.lenSeqUL = 0;
   swapSeqST.qStr = 0;

   samSTPtr->refEndUI =
      samSTPtr->refEndUI - samSTPtr->refStartUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec11:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ret_fun14_sec11:;
      return 0;

   memErr_fun14_sec11:;
      return def_memErr_mapRead;
} /*flipRef_mapRead*/

