struct conBase *
readStats(
   struct miruTbl *miruST,
   struct amrStruct *amrSTAry,
   int numAmrI,
   char *drugStrAry,
   int numDrugsI,
   char refIdStr[],
   char **samHeadStr,
   int *buffHeadLenSI,
   int *strHeadLenSI,
   int fudgeLenI,
   float minMedianQF,
   float minMeanQF,
   struct tbConSet *tbConSettings
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   short errS = 0;

   /*Histograms*/
   int *readMap_HeapArySI = 0;
   int *filt_readMap_HeapArySI = 0;
   int offTargReadsSI = 0;
   int oldOfTargSI = 0;
   int umapReadCntSI = 0;

   /*For sam file processing*/
   struct samEntry samST;
   char *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   char refIdStr[256];
   FILE *samFILE = 0;

   ulong tabUL = ulCpMakeDelim('\t');
   char multiRefBl = 0; /*1: means multiple refs/exit*/
   int lenRefI = 0;

   struct amrHit *amrHitSTList = 0;
   int numHitsI = 0;

   struct conBase *conBaseSTAry;

   if(! *samHeadStr)
   { /*If: I need to make an sam file header*/
      *samHeadStr = malloc(sizeof(char) * 2048);

      if(! *samHeadStr) goto memErr_fun01_sec_0x_sub02;
   } /*If: I need to make an sam file header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-06:
   ^   - Get reference name and length + get past header
   ^   o main sec-06 sub-01:
   ^     - Get the reference name/length from header
   ^   o main sec-06 sub-02:
   ^     - print tbCon cosensus (same file) settings
   ^   o main sec-06 sub-03:
   ^     - print tbCon tsv file settings
   ^   o main sec-06 sub-04:
   ^     - Check if I could extract reference name/length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-06 Sub-01:
   *   - Get the reference length from the header
   \*****************************************************/

   initSamEntry(&samST);

   errS =
      readSamLine(&samST, &buffStr, &lenBuffUL, samFILE);

   while(! errS)
   { /*Loop: read in the header*/

      /*Check if I have a comment*/
      if(*samST.extraStr != '@') break;

      if(
      fprintf(samConFILE, "%s\n", samST.extraStr);

      /*This is not perfect because it assumes the
      `  sam file @SQ entry is in the order of
      `  @SQ\tSN:reference-name\tLN:reference-length
      */

      if(! cStrEql("@SQ\t", samST.extraStr, '\t'))
      { /*If: this has length information*/
         if(multiRefBl)
         { /*If: sam file has multiple references*/
            if(samFILE != stdin) fclose(samFILE);

            fclose(samConFILE);

            freeSamEntryStack(&samST);
            freeGeneCoords(&coordsST);
            freeAmrStructArray(&amrSTAry, numAmrI);
            freeMiruTbl(&miruST);
            free(buffStr);
            
            fprintf(
               stderr,
               "-sam %s has multiple references\n",
               samFileStr
            ); /*Let user know the problem*/

            if(samFILE != stdin) fclose(samFILE);

            exit(-1);
         } /*If: sam file has multiple references*/

         multiRefBl = 1;

         /*Get past "@SQ\t"*/
         tmpStr = samST.extraStr + 4; 

         while(*tmpStr++ !=':') if(*tmpStr <31) break;

         /*Check if I have a reference id*/
         if(*(tmpStr - 1) != ':') goto nextHeader_feezeTb;

         tmpStr +=
            ulCpStrDelim(refIdStr,tmpStr,tabUL,'\t');

         ++tmpStr;
        
         if(*tmpStr < 31 ) goto nextHeader_feezeTb;

         /*Move past the LN: flag*/
         while(*tmpStr++ !=':') if(*tmpStr <31) break;

         /*Check if I have a read length*/
         if(*(tmpStr - 1) != ':') goto nextHeader_feezeTb;

         /*Get the reference length*/
         tmpStr = base10StrToUI(tmpStr, lenRefI);

         /*Check if I had a conversion error*/
         if(*tmpStr > 31) lenRefI = defRefLen;
      } /*If: this has length information*/

      nextHeader_feezeTb:;

      errS =
         readSamLine(
            &samST,
            &buffStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next line*/
   } /*Loop: read in the header*/

   /*****************************************************\
   * Main Sec-06 Sub-04:
   *   - Check if I could extract reference name/length
   \*****************************************************/

   /*This is from tbConSource/tbCon-fun.h*/
   if(! lenRefI) lenRefI = defRefLen;

   if(refIdStr[0] == '\0')
   { /*If: I could not get the reference name*/
      refIdStr[0] = 'u';
      refIdStr[1] = 'n';
      refIdStr[2] = 'k';
      refIdStr[3] = 'o';
      refIdStr[4] = 'w';
      refIdStr[5] = 'n';
      refIdStr[6] = '\0';
   } /*If: I could not get the reference name*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-07:
   ^   - Do the reade analysis
   ^   o main sec-06 sub-01:
   ^     - Allocate memory for the read stats arrays
   ^   o main sec-06 sub-03:
   ^     - Filter sam entries
   ^   o main sec-06 sub-04:
   ^     - Build the filtered histogram
   ^   o main sec-06 sub-05:
   ^     - Build the consensus
   ^   o main sec-06 sub-06:
   ^     - Check for AMRs
   ^   o main sec-06 sub-07:
   ^     - Check for MIRU lineages
   ^   o main sec-06 sub-08:
   ^     - Move to the next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-07 Sub-01:
   *   - Allocate memory for the read stats arrays
   \*****************************************************/

   readMapIAry = calloc(lenRefI, sizeof(int));

   if(! readMapIAry)
   { /*If: I had an memory error*/
      freeSamEntryStack(&samST);
      freeGeneCoords(&coordsST);
      freeAmrStructArray(&amrSTAry, numAmrI);
      freeMiruTbl(&miruST);
      free(buffStr);
            
      fclose(samConFILE);

      if(samFILE != stdin) fclose(samFILE);

      fprintf(stderr, "Ran out of memory\n");

      exit(-1);
   } /*If: I had an memory error*/

   filt_readMapIAry = calloc(lenRefI, sizeof(int));

   if(! filt_readMapIAry)
   { /*If: I had an memory error*/
      freeSamEntryStack(&samST);
      freeGeneCoords(&coordsST);
      freeAmrStructArray(&amrSTAry, numAmrI);
      freeMiruTbl(&miruST);

      free(buffStr);
      free(readMapIAry);

      fclose(samConFILE);
            
      if(samFILE != stdin) fclose(samFILE);

      fprintf(stderr, "Ran out of memory\n");

      exit(-1);
   } /*If: I had an memory error*/

   /*****************************************************\
   * Main Sec-07 Sub-02:
   *   - filter the reads
   \*****************************************************/

   while(! errS)
   { /*Loop: Process each read in the sam file*/
      if(samST.flagUS & (4 | 256 | 2048))
      { /*If:umapped 4, secondary 256, or suplemtal 2048*/
         /*If this was an unmapped read*/
         if(samST.flagUS & 4) ++umapReadCntI;

         goto nextSamRead_freezeTb;
      } /*If:umapped 4, secondary 256, or suplemtal 2048*/

      /*Remove soft masked bases*/
      trimSamEntry(&samST);

      oldOfTargI = offTargReadsI;

      /*Build up the histogram of stats*/
      addBaseToAmpDepth(
         &samST,
         coordsST,
         numCoordsI,
         readMapIAry,
         offTargReadsI
      );

      /*If this read was off target*/
      if(oldOfTargI < offTargReadsI)
         goto nextSamRead_freezeTb;

      /*Check the Q-scores*/
      if(samST.medianQF < minMedianQF)
         goto nextSamRead_freezeTb;

      if(samST.medianQF < minMedianQF)
         goto nextSamRead_freezeTb;

      /*Check the mapping quality*/
      if(samST.mapqUC < tbConSettings.minMapqUC)
         goto nextSamRead_freezeTb;

      /*Check if the read is to short*/
      if(samST.alnReadLenUI < tbConSettings.minLenI)
         goto nextSamRead_freezeTb;

      /**************************************************\
      * Main Sec-07 Sub-03:
      *   - Build the filtered histogram
      \**************************************************/

      ++totalReadsUI;

      addBaseToAmpDepth(
         &samST,
         coordsST,
         numCoordsI,
         filt_readMapIAry,
         oldOfTargI         /*Never used*/
      );

      /**************************************************\
      * Main Sec-07 Sub-04:
      *   - Build the consensus
      \**************************************************/

      /*Add the read to the consensus*/
      errS =
         addReadToConBaseArray(
            &samST,
            &conBaseSTAry,
            (uint *) &lenRefI,
            &tbConSettings
         ); /*Add the read to the consnesus*/
            
      if(errS & def_tbCon_Memory_Err)
      { /*If: memory error for consensus*/
         freeSamEntryStack(&samST);
         freeGeneCoords(&coordsST);
         freeAmrStructArray(&amrSTAry, numAmrI);
         freeConBaseAry(&conBaseSTAry, lenRefI);
         freeMiruTbl(&miruST);

         free(buffStr);
         free(readMapIAry);
         free(filt_readMapIAry);
            
         if(samFILE != stdin) fclose(samFILE);
      } /*If: memory error for consensus*/

      /**************************************************\
      * Main Sec-07 Sub-05:
      *   - Check for AMRs
      \**************************************************/

      amrHitSTList =
         checkAmrSam(
            &samST,
            amrSTAry,
            numAmrI,
            &numHitsI,
            &errC
         ); /*Check if read has antibiotic resistance*/

      if(errC)
      { /*If: memory error for consensus*/
         freeSamEntryStack(&samST);
         freeGeneCoords(&coordsST);
         freeAmrStructArray(&amrSTAry, numAmrI);
         freeConBaseAry(&conBaseSTAry, lenRefI);
         freeMiruTbl(&miruST);

         free(buffStr);
         free(readMapIAry);
         free(filt_readMapIAry);
            
         fclose(samConFILE);

         if(samFILE != stdin) fclose(samFILE);
      } /*If: memory error for consensus*/

      /*I only care about this for consensuses*/
      if(amrHitSTList) freeAmrHitList(amrHitSTList);

      /**************************************************\
      * Main Sec-07 Sub-06:
      *   - Check for MIRU lineages 
      \**************************************************/

      inc_matching_len_lineages(&samST,fudgeLenI,miruST);

      /**************************************************\
      * Main Sec-07 Sub-07:
      *   - Move to the next read
      \**************************************************/

      nextSamRead_freezeTb:;

      errS =
         readSamLine(
            &samST,
            &buffStr,
            &lenBuffUL,
            samFILE
         ); /*Get the next line*/
   } /*Loop: Process each read in the sam file*/

   freeSamEntryStack(&samST);

   if(samFILE != stdin) fclose(samFILE);

   samFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-08:
   ^   - Print out the data collected for the reads
   ^   o main sec-08 sub-01:
   ^     - Print out the unfiltered read stats
   ^   o main sec-08 sub-02:
   ^     - Print out the filtered read stats
   ^   o main sec-08 sub-03:
   ^     - Print out the AMR hits for the reads
   ^   o main sec-08 sub-04:
   ^     - Print out read MIRU table
   ^   o main sec-08 sub-05:
   ^     - Print out the tsv file of variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-08 Sub-01:
   *   - Print out the unfiltered read stats
   \*****************************************************/

   /*I already checked if I could open this file*/
   outFILE = fopen(readStatsStr, "w");

   pHeaderAmpDepth(outFILE);

   pAmpDepthHistogram(
      readMapIAry,
      tbConSettings.printMinDepthI,      
      coordsST,
      numCoordsI,
      offTargReadsI,
      umapReadCntI,
      graphFlagStr,
      outFILE
    ); /*Print out the unfilterd read stats*/

   free(readMapIAry);

   /*****************************************************\
   * Main Sec-08 Sub-02:
   *   - Print out the filtered read stats
   \*****************************************************/

   /*Adding a unique column name so I can compare
   `  filtered and unfiltered reads
   */
   tmpStr = spareStr;
   tmpStr += ulCpStrDelim(tmpStr, graphFlagStr, 0, '\0');
   ulCpStrDelim(tmpStr, "-filt", 0, '\0');

   pAmpDepthHistogram(
      filt_readMapIAry,
      tbConSettings.printMinDepthI,      
      coordsST,
      numCoordsI,
      0,
      0,
      spareStr,
      outFILE
    ); /*Print out the filterd read stats*/

   fclose(outFILE);
   outFILE = 0;

   free(filt_readMapIAry);
   freeGeneCoords(&coordsST);

   /*****************************************************\
   * Main Sec-08 Sub-03:
   *   - Print out the AMR hits for the reads
   \*****************************************************/

   /*I already checked if I could open this file*/
   outFILE = fopen(readAmrStr, "w");

   pHeadAmrs(outFILE);

   pAmrs(
      (uint) tbConSettings.minDepthI,
      minPercMapF,
      (float) 0, /*Not really usefull, so leaving out*/
      totalReadsUI,
      amrSTAry,
      (uint) numAmrI,
      drugStrAry,
      outFILE
   ); /*Print out the AMRs for the reads*/
      
   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec-08 Sub-04:
   *   - Print out read MIRU table
   \*****************************************************/

   errC = pMiruTbl(miruST, readMiruStr);

   /*this should never happen*/
   if(errC)
   { /*If: I could not open the output file*/
      fprintf(
         stderr,
         "Could not write read MIRU table to %s\n",
         readMiruStr
      ); /*Let the user know the file problem*/
   } /*If: I could not open the output file*/

   /*****************************************************\
   * Main Sec-08 Sub-05:
   *   - Print out the tsv file of variants
   \*****************************************************/

   /*Build the tsv of variants table*/
   errC =
      pConBaseArray(
         conBaseSTAry,
         lenRefI,
         refIdStr,
         &tbConSettings,
         conTsvStr
      ); /*Print out the variants (not a vcf)*/

   memErr_fun01_sec_0x_sub02:;

   free(readMap_HeapArySI);
   readMap_HeapArySI = 0;

   free(filt_readMap_HeapArySI);
   filt_readMap_HeapArySI = 0;

   free(buffHeapStr);
   buffHeapStr = 0;

   fclose(samFILE);

   if(conBaseSTAry != 0) freeConBaseAry(&conBaseSTAry);
   conBaseSTAry = 0;

   if(amrHitSTList != 0) freeAmrHistList(&amrHitSTList)
   amrHitSTList = 0;
}
