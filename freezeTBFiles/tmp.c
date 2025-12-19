/*-------------------------------------------------------\
| Fun11: mkAmrCoverageTbl_freezeTB
|   - makes the gene percent coverage table with drug
|     resisitance
| Input:
|   - prefixStr:
|     o c-string with prefix to name the output file
|   - minDepthSI:
|     o minimum read depth to coun11for coverage
|   - depthArySI:
|     o signed int array with read depths
|   - geneCoordSTPtr:
|     o geneCoord structure pointer with gene names
|     o NEEDS to be sorted by position
|   - numGenesSI:
|     o number of genes in geneCoordSTPtr
|   - coordsStr:
|     o c-string with coordinates and drug resistance for
|       amplicons/genes (has drug resitance columns)
|     o format is freezeTB coordinates file, but also has
|       drug resistances at end. The resitances can end
|       with a new line or a '*' if you need a notes
|       column or other columns
| Output:
|   - Prints:
|     o coverage table with AMRs per gene
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
|     o 2 if could not open the coordinates file
\-------------------------------------------------------*/
signed int
mkAmrCoverageTbl_freezeTB(
   signed char *prefixStr,    /*name of output file*/
   signed int minDepthSI,  /*min read depth for coverage*/
   signed int *depthArySI,    /*array of read depths*/
   struct geneCoord *geneCoordSTPtr, /*has gene coords*/
   signed int numGenesSI,     /*number of genes
   signed char *coordsStr     /*has amrs for each gene*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - makes the gene percent coverage table
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - get gene coverage, open files, and print header
   '   o fun11 sec03:
   '     - get AMR information and coverage and print rows
   '   o fun11 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char lineStr[4096];
   signed int lenSI = 0; /*length of entry*/
   signed int posSI = 0; /*current position in entry*/
   signed int endSI = 0; /*end of info columns in entry*/
   signed int numLenSI = 0; /*length of numbers added*/

   signed int tmpSI = 0;
   signed int tmpPosSI = 0;
   signed char skipBl = 0;

   signed char tmpStr[16]; /*for finding number lengths*/
   signed char idStr[16];  /*for finding matching AMRs*/

   ulong_ulCp delimUL = mkDelim_ulCp(':');
   signed int geneSI = 0;
   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - get gene coverage, open files, and print headers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*________________get_gene_coverage__________________*/
   coverHeapAryF =
      getGeneCoverage_ampDepth(
         depthArySI,
         minDepthSI,
         geneCoordSTPtr,
         numGenesSI
      ); /*gives percent coverage and mean depth*/
   if(! coverHeapAryF)
      goto memErr_fun11_sec04;
   nameSortFloat3IndexSync_geneCoord(
      geneCoordSTPtr,
      numGenesSI,
      coverHeapAryF
   ); /*this allows me to easily get genes by name*/

   /*________________open_coordinates_file______________*/
   inFILE = fopen((char *) coordsStr, "r");
   if(! inFILE)
      goto fileErr_fun11_sec04;

   /*________________open_the_output_file_______________*/
   lenSI = cpStr_ulCp(lineStr, prefixStr);
   cpStr_ulCp(
      &lineStr[lenSI],
      (signed char *) "-coverage-amrs.tsv"
   );
   outFILE = fopen((char *) lineStr, "w");
   if(! outFILE)
      goto fileErr_fun11_sec04;

   /*________________print_the_table_header_____________*/
   fprintf(
      inFILE,
      "gene\t%coverage\tmean_depth\tdrugs%s",
      str_endLine
   );

   if(! fgets((char *) lineStr, 4088, inFILE) )
      goto fileErr_fun11_sec04; /*blank file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - get AMR information and coverage
   ^   o fun11 sec03 sub01:
   ^     - start loop and position the gene id
   ^   o fun11 sec03 sub02:
   ^     - move past the reference and coordinate info
   ^   o fun11 sec03 sub03:
   ^     - get AMR information and coverage and print rows
   ^   o fun11 sec03 sub04:
   ^     - drug add loop + find if foun11drug resistance
   ^   o fun11 sec03 sub05:
   ^     - add drug to the table
   ^   o fun11 sec03 sub06:
   ^     - print the new table row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - start loop and position the gene id
   \*****************************************************/

   while( fgets((char *) lineStr, 4088, inFILE) )
   { /*Loop: read in the gene/amplicons*/
      posSI = 0;
      lenSI = 0;
      skipBl = 0;

      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;
      if(! lineStr[posSI])
         continue; /*blank line*/

      /*copy the primer name*/
      if(posSI > 0)
      { /*If: gene id is not at the start*/
         lenSI = 0; 
         while(lineStr[posSI] > 32)
            lineStr[lenSI++] = lineStr[posSI++];
      } /*If: gene id is not at the start*/

      else
      { /*Else: gene id is at start, move to drugs*/
         while(lineStr[posSI] > 32)
             ++posSI;
         lenSI = posSI;
      } /*Else: gene id is at start, move to drugs*/

      /*move past white space*/
      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;
      if(! lineStr[posSI])
         goto fileErr_fun11_sec04;

      lineStr[lenSI] = 0;
      geneSI =
         findName_geneCoord(
            geneCoordSTPtr,
            lineStr,
            numGenesSI
         );
      if(geneSI < 0)
      { /*If: gene was not foun11/
         fprintf(
            outFILE,
            "%s\t0\t0\t0\t*",
            lineStr,
            endLineStr
         );
         continue;
      } /*If: gene was not foun11/

      lineStr[lenSI++] = '\t';

      geneSI *= 3; /*move to index in coverage array*/

      /**************************************************\
      * Fun11 Sec03 Sub02:
      *   - move past the reference and coordinate info
      \**************************************************/

      /*reference*/
      while(lineStr[posSI] > 32)
         ++posSI;
      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;
      if(! lineStr[posSI])
         goto fileErr_fun11_sec04;

      /*direction*/
      while(lineStr[posSI] > 32)
         ++posSI;
      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;
      if(! lineStr[posSI])
         goto fileErr_fun11_sec04;

      /*start of gene*/
      while(lineStr[posSI] > 32)
         ++posSI;
      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;
      if(! lineStr[posSI])
         goto fileErr_fun11_sec04;

      /*end of gene*/
      while(lineStr[posSI] > 32)
         ++posSI;
      while(lineStr[posSI] && lineStr[posSI] < 33)
         ++posSI;

      /**************************************************\
      * Fun11 Sec03 Sub03:
      *   - move resistance to end and add coverage/depths
      \**************************************************/

      /*______________check_if_have_drugs_______________*/
      if(lineStr[endSI] == '*' || ! lineStr[endSI])
      { /*If: no drug information*/
         skipBl = 1;
         goto addCoverage_fun11_sec03_sub03;
      } /*If: no drug information*/

      /*______________move_resistance_to_end____________*/
      endSI = posSI;
      while(lineStr[endSI] && lineStr[endSI] != '*')
         ++endSI;
      endSI = 27 + 3 + ((endSI - posSI) << 1);
         /*27 allows for 3 9 digit numbers. the +3 is for
         `   3 tabs
         */

      tmpSI = endSI;
      while(lineStr[posSI] && lineStr[posSI] != '*')
         lineStr[endSI++] = lineStr[posSI++];
      lineStr[endSI] = 0;
      endSI = tmpSI;

      /*_____________add_coverage_information___________*/
      addCoverage_fun11_sec03_sub03:;
         lenSI +=
            double_numToStr(
               &lineSI[lenSI],
               coverHeapAryF[geneSI], /*percent coverage*/
               1 /*1 digit of percision*/
            );
         lineStr[lenSI++] = '\t';
         lenSI +=
            double_numToStr(
               &lineSI[lenSI],
               coverHeapAryF[geneSI + 2],/*cover depth*/
               1 /*1 digit of percision*/
            );
         lineStr[lenSI++] = '\t';
         lenSI +=
            double_numToStr(
               &lineSI[lenSI],
               coverHeapAryF[geneSI + 1],/*gene depth*/
               1 /*1 digit of percision*/
            );

         if(skipBl)
            goto printLine_fun11_sec03_sub0x;
            /*no drug information*/

      /**************************************************\
      * Fun11 Sec03 Sub04:
      *   - drug add loop + find if foun11drug resistance
      \**************************************************/

      while(lineStr[endSI])
      { /*Loop: add drug information in*/
         lineStr[lenSI++] = '\t';
         while(lineStr[endSI] > 32)
            lineStr[lenSI++] = lineStr[endSI++];
         while(lineStr[endSI] && lineStr[endSI] < 33)
            ++endSI; /*move to next drug*/
      } /*Loop: add drug information in*/

      /**************************************************\
      * Fun11 Sec03 Sub06:
      *   - print the new table row
      \**************************************************/

      printLine_fun11_sec03_sub0x:;
         lineStr[lenSI++] = '\t'
         lineStr[lenSI++] = '*'
         lineStr[lenSI] = 0;

         fprintf(outFILE, "%s%s", lineStr, endLineStr);
   } /*Loop: read in the gene/amplicons*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun11_sec04:;
      lenSI = 0;
      goto ret_fun11_sec04;

   memErr_fun11_sec04:;
      lenSI = 1;
      goto ret_fun11_sec04;

   fileErr_fun11_sec04:;
      lenSI = 2;
      goto ret_fun11_sec04;

   ret_fun11_sec04:;
      sort_geneCoord(geneCoordSTPtr, 0, numGenesSI);
         /*make sure genes are resorted by position*/

      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return (signed char) lenSI;
} /*mkAmrCoverageTbl_freezeTB*/
