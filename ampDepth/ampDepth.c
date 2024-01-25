
/*########################################################
# Name: ampDepth
# Use:
#  - Makes a tsv file with the read depths for detected
#    amplicons in a sam file. It includes the genes names
#    in each amplicon and the read depths for each gene in
#    an amplicon
# Libraries:
#   - "../generalLib/trimSam.h"
#   o "../generalLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   - "../generalLib/geneCoordStruct.h"
#   o "../generalLib/genMath.h"
# C Sta../generalLib/ndard Libraries:
#   o <stdlib.h>
#   o <stdint.h>
#   o <stdio.h>
#   o <string.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'   - Included libraries, defintions, and global variables
'  o main:
'    - Prints out a tsv with the mean, minimum, and
'  o fun-01: ampDepthGetCmdInput
'    - Gets the command line input supplied by the user
'  o fun-02: pAmpDepthHelp
'    - Prints the help message for ampDepth
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, defintions, and global variables
\-------------------------------------------------------*/

#include "../generalLib/geneCoordStruct.h"
#include "../generalLib/trimSam.h"

#define defVersion 20240125
#define defMinDepth 20

char *globalExtraCol = "own";

/*-------------------------------------------------------\
| Fun-01: ampDepthGetCmdInput
|   - Gets the command line input supplied by the user
| Input:
|   - argsStrAry:
|     o c-string array with user input
|   - numArgsI:
|     o Number of strings in argsStrAry
|   - pafStr:
|     o Pointer to hold the path of the input paf file
|   - samStr:
|     o Pionter to hold the path of the input sam file
|   - outStr:
|     o Pionter to hold the file to output the tsv to
|   - minDepthI:
|     o Minimum read depth to not split up an amplicon
|   - extraColStr:
|     o Will point to the flag (c-string) to add to the
|       first column of the output tsv
| Output:
|  - Modifies:
|    o pafStr, samStr, outStr, minDepthI, and extraColStr
|      to hold the user supplied values
|  - Returns:
|    o 0 for no problems
|    o Pointer to invalid arguement (error)
\-------------------------------------------------------*/
char * ampDepthGetCmdInput(
   char *argsStrAry[], /*User input arguments*/
   int numArgsI,       /*Number of arguments input*/
   char **pafStr,      /*Name/path to paf file*/
   char **samStr,      /*Name/path to sam file*/
   char **outStr,      /*Name/path to output file*/
   int *minDepthI,     /*Min depth to use*/
   char **extraColStr  /*Flag to add for data*/
);

/*-------------------------------------------------------\
| Fun-02: pAmpDepthHelp
|   - Prints the help message for ampDepth
| Input:
|   - outFILE:
|     o File to print the help message to
| Output:
|  - Prints:
|    o Prints the help message to outFILE
\-------------------------------------------------------*/
void pAmpDepthHelp(
   FILE *outFILE
); /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' TOC:pAMpDepthHelp
   '   - Print the help message for ampDepth
   '   o fun-02 sec-01:
   '      - Print out the usage and description lines
   '   o fun-02 sec-02:
   '      - Print out the user input ilneS
   '   o fun-02 sec-03:
   '      - Print out the program output lines
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Main:
|   - Prints out a tsv with the mean, minimum, and
|     maximum read depths for each amplicon.
| Input:
|   - numArgsI:
|     o Number of arguments/parameters the user input
|   - argsStrAry:
|     o User input
| Output:
|  - Prints:
|    o The tsv file mentioned in pAmpDepthHelp
\-------------------------------------------------------*/
int main(
   int numArgsI,       /*Number of arguments input*/
   char *argsStrAry[]  /*User input arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' TOC: Main
   '
   '  o main sec-01:
   '    - Variable declerations
   '  o main sec-02:
   '    - Get and check user input
   '  o main sec-03:
   '    - Check the input files
   '  o main sec-04:
   '    - Extract the genes from the paf file
   '  o main sec-05:
   '    - Get the read mapping tables set up
   '  o main sec-06:
   '    - Read in the mapping file
   '  o main sec-07:
   '    - Find the max number of columns (max genes)
   '  o main sec-08:
   '    - Print out the header
   '  o main sec-09:
   '    - Find the mapping positions
   '  o main sec-10:
   '    - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input*/
   char *pafStr = 0;
   char *samStr = 0;
   char *outStr = 0;
   int minDepthI = defMinDepth;
   char *extraColStr = globalExtraCol;

   char *errStr = 0;

   ushort lenBuffUS = 2048;
   char buffStr[lenBuffUS];
   char alnTypeC;
   uchar errUC = 0; /*Error report*/
   
   uint numLinesUI = 0;
   int numGenesI = 0;
   int geneIndexI = 0;
   uint idIndexUI = 0;

   uint *readMapAryUI = 0; /*Mapped region of reads*/
   uint offTargUI = 0;
   uint umappedUI = 0;

   uint mapStartUI = 0;
   uint mapEndUI = 0;
   uint tmpStartUI = 0;

   /*For printing out read counts*/
   uint readsAtStartUI = 0;
   uint readsAtEndUI = 0;
   uint maxReadsUI = 0;
   uint minReadsUI = 0;
   ulong avgDepthUL = 0; /*average number of reads*/
   uint uiBase = 0;
   uint maxColUI = 0; /*How many columns I will print*/
   int tmpI = 0;

   struct geneCoords *genesST = 0; 
   struct samEntry readST;

   FILE *pafFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = stdout;


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^  - Get and check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errStr = 
      ampDepthGetCmdInput(
         argsStrAry,  /*User input arguments*/
         numArgsI,    /*Number of arguments input*/
         &pafStr,     /*Name/path to paf file*/
         &samStr,     /*Name/path to sam file*/
         &outStr,     /*Name/path to output file*/
         &minDepthI,  /*Min depth to use*/
         &extraColStr /*Flag to add for data*/
     ); /*Get the user input*/

   if(errStr != 0)
   { /*If: I had an error*/
     if(   strcmp(errStr, "-h") == 0
        || strcmp(errStr, "--h") == 0
        || strcmp(errStr, "-help") == 0
        || strcmp(errStr, "--help") == 0
        || strcmp(errStr, "help") == 0
     ){ /*If: the user requested the help message*/
        pAmpDepthHelp(stdout);
        exit(0);
     } /*If: the user requested the help message*/


     if(   strcmp(errStr, "-v") == 0
        || strcmp(errStr, "--v") == 0
        || strcmp(errStr, "-version") == 0
        || strcmp(errStr, "--version") == 0
        || strcmp(errStr, "version") == 0
     ){ /*If: the user requested the version number*/
        fprintf(
           stdout,
           "ampDepth version: %i\n",
           defVersion
        );
        exit(0);
     } /*If: the user requested the version number*/
 

     if(strcmp(errStr, "-min-depth") == 0)
     { /*If: the -min-depth value was not numeric*/
        fprintf(
           stderr,
           "value from -min-depth is non-numeric\n"
        );
        exit(-1);
     } /*If: the -min-depth value was not numeric*/

     pAmpDepthHelp(stderr);

     fprintf(
           stderr,
           "%s is not valid input\n",
           errStr
        );

     exit(-1);
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^  - Check the input files
   ^  o main sec-03 sub-01:
   ^    - Check the paf file with gene coordinates
   ^  o main sec-03 sub-02:
   ^    - Check the sam file (has reads)
   ^  o main sec-03 sub-03:
   ^   - Check the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Check the paf file with gene coordinates
   \*****************************************************/

   if(pafStr == 0)
   { /*If: No paf file was input*/
      fprintf(
         stderr,
        "No paf file with gene positions input (-paf)\n"
      );

      exit(-1);
   } /*If: No paf file was input*/

   else if(pafStr[0] == '-')
   { /*If: the paf file is comming from stdin*/
      if(samStr == 0 || samStr[0] == '-')
      { /*If: I have two stdin file*/
         fprintf(
            stderr,
           "ERROR both -paf and -sam are set to stdin\n"
         );

         exit(-1);
      } /*If: I have two stdin file*/

      pafFILE = stdin;
   } /*If: the paf file is comming from stdin*/

   else
   { /*Else: the user supplied a paf file*/
      pafFILE = fopen(pafStr, "r");

      if(pafFILE == 0)
      { /*If: the file could not be opened*/
         fprintf(
            stderr,
            "-paf %s could not be opened\n",
            pafStr
          );

          exit(-1);
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check the input sam file with reads
   \*****************************************************/

   if(samStr == 0 || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: the user supplied a sam file*/
      samFILE = fopen(samStr, "r");

      if(samFILE == 0)
      { /*If: the file could not be opened*/
         fclose(pafFILE);

         fprintf(
            stderr,
            "-sam %s could not be opened\n",
            samStr
          );

          exit(-1);
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*****************************************************\
   * Main Sec-03 Sub-03:
   *   - Check the output file
   \*****************************************************/

   if(outStr == 0 || outStr[0] == '-') outFILE = stdout;
 
   else
   { /*Else: the user supplied a sam file*/
      outFILE = fopen(outStr, "w");

      if(outFILE == 0)
      { /*If: the file could not be opened*/
         fclose(pafFILE);
         fclose(outFILE);

         fprintf(
            stderr,
            "-out %s could not be opened\n",
            outStr
          );

          exit(-1);
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Extract the genes from the paf file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Find the number of entries in the paf file*/
   while(fgets(buffStr, lenBuffUS, pafFILE)) ++numLinesUI; 

   /*Extract each entry*/
   fseek(pafFILE, 0, SEEK_SET);

   genesST = makeGeneCoords(numLinesUI);

   if(genesST == 0)
   { /*If: I had a memory error*/
      fclose(pafFILE);
      fclose(samFILE);

      fprintf(stderr, "Memory error\n");
      exit(-1);
   } /*If: I had a memory error*/

   while(fgets(buffStr, lenBuffUS, pafFILE))
   { /*Loop: Get entries from the paf file*/
      /*Get the gene locations from the paf line*/
      getPafGene(genesST, numGenesI, alnTypeC, buffStr);

      if(alnTypeC == 'P')
      { /*If: This was a primary alignment*/
         idIndexUI += 64;
         ++numGenesI;
      } /*If: This was a primary alignment*/
   } /*Loop: Get entries from the paf file*/

   --numGenesI; /*Convert to index 0*/
   sortGeneCoords(genesST, 0, numGenesI);

   fclose(pafFILE);
   pafFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^  - Get the read mapping tables set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   offTargUI = genesST->endAryUI[numGenesI] + 2;
     /*These are genes that did not map to the end of the
     ` gene range, but still mapped to the reference
     ` The + 2 is to account for index 1 and to ensure
     `   thre array ends in 0
     */
   readMapAryUI = calloc(offTargUI, sizeof(uint));

   if(readMapAryUI == 0)
   { /*If: I had a memory error*/
      fclose(samFILE);
      freeGeneCoords(genesST);
      
      fprintf(stderr, "Memory error (main sec-05)\n");
      exit(-1);
   } /*If: I had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-06:
   ^  - Read in the mapping file
   ^  o main sec-06 sub-01:
   ^    - Set up, detect headers, unmapped reads, and trim
   ^      reads
   ^  o main sec-06 sub-02:
   ^    - Handle secondary and supplemental mapped reads
   ^  o main sec-06 sub-03:
   ^    - Handle primary mapped reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-06 Sub-01:
   *   - Set up, detect headers, unmapped reads, and trim
   *     reads
   \*****************************************************/

   initSamEntry(&readST);
   errUC = readSamLine(&readST, samFILE);
   offTargUI = 0;

   while(errUC == 1)
   { /*Loop: read in the samfile*/
      /*check if this is a header line*/
      if(readST.samEntryCStr[0] == '@') goto nextLine;

      if(readST.flagUSht & 4)
      { /*If: the read was unmapped*/
         ++umappedUI;
         goto nextLine;
      } /*If: the read was unmapped*/

      trimSamEntry(&readST); /*Trim off softmasked bases*/
      mapEndUI = readST.readLenUInt + readST.posOnRefUInt;

      /**************************************************\
      * Main Sec-06 Sub-02:
      *   - Handle secondary and supplemental mapped reads
      \**************************************************/

      if(readST.flagUSht & (4 | 2048)) goto nextLine;

      /**************************************************\
      * Main Sec-06 Sub-03:
      *   - Handle primary mapped reads
      \**************************************************/

      for(
         mapStartUI = readST.posOnRefUInt;
         mapStartUI < mapEndUI;
         ++mapStartUI
      ){ /*Loop: Fill in bases*/
         if(mapStartUI > genesST->endAryUI[numGenesI])
         { /*If: the read has an offtarget section*/
           ++offTargUI;
           goto nextLine;
         } /*If: the read has an offtarget section*/

         ++readMapAryUI[mapStartUI];
      } /*Loop: Fill in bases*/

      nextLine:

      blankSamEntry(&readST);
      errUC = readSamLine(&readST, samFILE);
   } /*Loop: read in the samfile*/


   fclose(samFILE);
   samFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-07:
   ^  - Find the max number of columns
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   mapStartUI = 0;
   maxColUI = 0;

   while(mapStartUI <= genesST->endAryUI[numGenesI] + 1)
   { /*Loop: Get the gene positions that mapped*/
      ++mapStartUI;

      if(mapStartUI < genesST->startAryUI[0]) continue;

      geneIndexI = 
         findStartCoordInGeneCoord(
            genesST,
            mapStartUI,
            numGenesI
         ); /*Find the index of the gene at the position*/

      if(geneIndexI < 0) continue; /*Unmapped base*/
      if(readMapAryUI[mapStartUI] < minDepthI) continue;

      tmpI = 0;

      nextCol:

      ++tmpI;

      for(
         mapStartUI = mapStartUI;
         mapStartUI <= genesST->endAryUI[geneIndexI];
         ++mapStartUI
      ) if(readMapAryUI[mapStartUI] < minDepthI) break;

      if(readMapAryUI[mapStartUI] > minDepthI)
      { /*If: I have another gene*/
         ++geneIndexI;
         if(geneIndexI <= numGenesI) goto nextCol;
      } /*If: I have another gene*/

      maxColUI = noBranchMax(maxColUI, tmpI);
   } /*Loop: Get the gene positions that mapped*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-08:
   ^  - Print out the header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "flag\tstart\tend\tavgAmpDepth");
   fprintf(outFILE, "\tminAmpDepth\tmaxAmpDepth");
   fprintf(outFILE, "\tentireFirstGene\tentireLastGene");

   for(tmpI = 0; tmpI < maxColUI; ++tmpI)
   { /*Loop: Print out the header for each gene*/
      fprintf(
         outFILE,
         "\tfirstBaseDepth%i\tlastBaseDepth%i",
         tmpI,
         tmpI
      );

      fprintf(
         outFILE,
         "\tavgDepth%i\tminDepth%i\tmaxDepth%i\tgeneId%i",
         tmpI,
         tmpI,
         tmpI,
         tmpI
      );
   } /*Loop: Print out the header for each gene*/

   fprintf(outFILE, "\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-09:
   ^   - Find the mapping positions
   ^   o main sec-09 sub-01:
   ^     - Check if positon has enough depth to keep
   ^   o main sec-09 sub-02:
   ^     - Find the amplicon stats
   ^   o main sec-09 sub-03:
   ^     - Print the amplicon stats
   ^  o main sec-09 sub-04:
   ^    - Find the stats for each gene in the amplicon
   ^  o main sec-09 sub-05:
   ^    - Print the stats for each gene in the amplicon
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-09 Sub-01:
   *   - Check if positon has enough depth to keep
   \*****************************************************/

   mapStartUI = 0;

   while(mapStartUI <= genesST->endAryUI[numGenesI] + 1)
   { /*Loop: Get the gene positions that mapped*/
      ++mapStartUI;

      if(mapStartUI < genesST->startAryUI[0]) continue;

      geneIndexI = 
         findStartCoordInGeneCoord(
            genesST,
            mapStartUI,
            numGenesI
         ); /*Find the index of the gene at the position*/

      if(geneIndexI < 0) continue; /*Unmapped base*/
      if(readMapAryUI[mapStartUI] < minDepthI) continue;


      /**************************************************\
      * Main Sec-09 Sub-02:
      *   - Find amplicon stats
      \**************************************************/

      tmpI = geneIndexI;
      avgDepthUL = 0;
      readsAtStartUI = readMapAryUI[mapStartUI];
      maxReadsUI = readsAtStartUI;
      minReadsUI = maxReadsUI;

      for(
         uiBase = mapStartUI;
         readMapAryUI[uiBase] >= minDepthI;
         ++uiBase
      ){ /*Loop: Find end of region*/
          tmpI += uiBase > genesST->endAryUI[tmpI];

          maxReadsUI = 
             noBranchMax(
                maxReadsUI,
                readMapAryUI[uiBase]
             ); /*Find the maximum*/

          minReadsUI = 
             noBranchMin(
                minReadsUI,
                readMapAryUI[uiBase]
             ); /*Find the maximum*/
          
          avgDepthUL += readMapAryUI[uiBase];
      } /*Loop: Find end of region*/


      /*Using interger mean, not decimal*/
      avgDepthUL /= (uiBase - 1 - mapStartUI);


      /**************************************************\
      * Main Sec-09 Sub-03:
      *   - Print the amplicon stats
      \**************************************************/

      fprintf(
         outFILE,
         "%s\t%i\t%u\t%lu\t%u\t%u",
         extraColStr,
         mapStartUI,
         uiBase - 1,
         avgDepthUL,
         minReadsUI,
         maxReadsUI
      ); /*Print out the starting and ending position*/

      if(tmpI == geneIndexI)
      { /*If: I only had one gene*/
         if(  mapStartUI > genesST->startAryUI[geneIndexI]
           || mapStartUI < genesST->endAryUI[geneIndexI]
         ) fprintf(outFILE, "\tPart\tNA");

         else fprintf(outFILE, "\tFull\tNA");
      }/*If: I only had one gene*/

      else
      { /*Else: I have multiple genes in one primer*/
         /*Print out if the first gene was complete*/
         if(mapStartUI > genesST->startAryUI[geneIndexI])
            fprintf(outFILE, "\tPart");

         else fprintf(outFILE, "\tFull");

         /*Print out if the last gene was complete*/
         if(uiBase < genesST->endAryUI[tmpI])
            fprintf(outFILE, "\tPart");

         else fprintf(outFILE, "\tFull");
      } /*Else: I have multiple genes in one primer*/

      /**************************************************\
      * Main Sec-09 Sub-04:
      *   - Find the stats for each gene in the amplicon
      \**************************************************/

      tmpI = 0;

      nextGene:

      ++tmpI;
      tmpStartUI = mapStartUI;
      avgDepthUL = 0;
      readsAtStartUI = readMapAryUI[mapStartUI];
      maxReadsUI = readsAtStartUI;
      minReadsUI = maxReadsUI;

      for(
         mapStartUI = mapStartUI;
         mapStartUI <= genesST->endAryUI[geneIndexI];
         ++mapStartUI
      ){ /*Loop: Check if gene is complete*/
          if(readMapAryUI[mapStartUI] < minDepthI) break;

          maxReadsUI = 
             noBranchMax(
                maxReadsUI,
                readMapAryUI[mapStartUI]
             ); /*Find the maximum*/

          minReadsUI = 
             noBranchMin(
                minReadsUI,
                readMapAryUI[mapStartUI]
             ); /*Find the maximum*/
          
          avgDepthUL += readMapAryUI[mapStartUI];
      } /*Loop: Check if gene is complete*/

      readsAtEndUI = readMapAryUI[mapStartUI - 1];

      /*Using interger mean, not decimal*/
      avgDepthUL /= (mapStartUI - 1 - tmpStartUI);

      /**************************************************\
      * Main Sec-09 Sub-05:
      *   - Print the stats for each gene in the amplicon
      \**************************************************/

      fprintf(
         outFILE,
         "\t%u\t%u\t%lu\t%u\t%u\t%s",
         readsAtStartUI,
         readsAtEndUI,
         avgDepthUL,
         minReadsUI,
         maxReadsUI,
         genesST->idStrAry[geneIndexI]
      );

      if(readMapAryUI[mapStartUI] > minDepthI)
      { /*If: I have another gene*/
         ++geneIndexI;
         if(geneIndexI <= numGenesI) goto nextGene;
      } /*If: I have another gene*/

      for(tmpI = tmpI; tmpI < maxColUI; ++tmpI)
         fprintf(outFILE, "\tNA\tNA\tNA\tNA\tNA\tNA");

      fprintf(outFILE, "\n");
   } /*Loop: Get the gene positions that mapped*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-10:
   ^  - clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   freeGeneCoords(genesST);
   free(readMapAryUI);
   readMapAryUI = 0;

   exit(0);
} /*main*/


/*-------------------------------------------------------\
| Fun-01: ampDepthGetCmdInput
|   - Gets the command line input supplied by the user
| Input:
|   - argsStrAry:
|     o c-string array with user input
|   - numArgsI:
|     o Number of strings in argsStrAry
|   - pafStr:
|     o Pointer to hold the path of the input paf file
|   - samStr:
|     o Pionter to hold the path of the input sam file
|   - outStr:
|     o Pionter to hold the file to output the tsv to
|   - minDepthI:
|     o Minimum read depth to not split up an amplicon
|   - extraColStr:
|     o Will point to the flag (c-string) to add to the
|       first column of the output tsv
| Output:
|  - Modifies:
|    o pafStr, samStr, outStr, minDepthI, and extraColStr
|      to hold the user supplied values
|  - Returns:
|    o 0 for no problems
|    o Pointer to invalid arguement (error)
\-------------------------------------------------------*/
char * ampDepthGetCmdInput(
   char *argsStrAry[], /*User input arguments*/
   int numArgsI,       /*Number of arguments input*/
   char **pafStr,      /*Name/path to paf file*/
   char **samStr,      /*Name/path to sam file*/
   char **outStr,      /*Name/path to output file*/
   int *minDepthI,     /*Min depth to use*/
   char **extraColStr  /*Flag to add for data*/
){ /*ampDepthGetCmdInput*/
   char *argStr = 0;
   char *parmStr = 0;
   char *errStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < numArgsI; iArg += 2)
   { /*Loop: Process user input*/
      parmStr = argsStrAry[iArg];
      argStr = argsStrAry[iArg + 1];

      if(strcmp("-paf", parmStr) == 0) *pafStr = argStr;
      else if(strcmp("-sam",parmStr) == 0) *samStr=argStr;
      else if(strcmp("-out",parmStr) == 0) *outStr=argStr;

      else if(strcmp("-min-depth", parmStr) == 0)
      { /*Else if: the user input a min depth*/
         errStr = base10StrToSI(argStr, *minDepthI);
         if(errStr[0] > 32) return parmStr;
      } /*Else if: the user input a min depth*/

      else if(strcmp("-flag", parmStr) == 0)
         *extraColStr = argStr;

      else return parmStr;
   } /*Loop: Process user input*/

   return 0;
} /*ampDepthGetCmdInput*/


/*-------------------------------------------------------\
| Fun-02: pAmpDepthHelp
|   - Prints the help message for ampDepth
| Input:
|   - outFILE:
|     o File to print the help message to
| Output:
|  - Prints:
|    o Prints the help message to outFILE
\-------------------------------------------------------*/
void pAmpDepthHelp(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' TOC:pAMpDepthHelp
   '   - Print the help message for ampDepth
   '   o fun-02 sec-01:
   '      - Print out the usage and description lines
   '   o fun-02 sec-02:
   '      - Print out the user input ilneS
   '   o fun-02 sec-03:
   '      - Print out the program output lines
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Print out the usage and description lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*General*/
   fprintf(
     outFILE,
     "ampDepth -paf gene-coordinates.paf -sam reads.sam\n"
   );

   fprintf(
      outFILE,
      "  - Prints out a tsv file with the mean, min, and"
   );

   fprintf(outFILE, "\n");

   fprintf(
      outFILE,
      "    and max read depths for each amplicon. It also"
   );

   fprintf(outFILE, "\n");

   fprintf(
      outFILE,
      "    incudes a list of genes in each amplicon"
   );

   fprintf(outFILE, "\n");



   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Print out the user input ilneS
   ^   o fun-02 sec-02 sub-01:
   ^     - Print out the -paf entry (gene coordinates)
   ^   o fun-02 sec-02 sub-02:
   ^     - Print out the -sam entry (mapped reads)
   ^   o fun-02 sec-02 sub-03:
   ^     - Print out the -out entry (output file)
   ^   o fun-02 sec-02 sub-04:
   ^     - Print out the -min-depth entry (min read depth)
   ^   o fun-02 sec-02 sub-05:
   ^     - Print out the -flag entry (string for column 1)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Print out the -paf entry (gene coordinates)
   \*****************************************************/

   fprintf(
      outFILE,
      "  Input:"
   );

   /*-paf*/
   fprintf(outFILE, "  -paf: [Required]\n");

   fprintf(
     outFILE,
     "    o Paf file with the coordiantes of each gene in"
   );

   fprintf(outFILE, "\n");

   fprintf(
      outFILE,
      "      the reference genome\n"
   );


   fprintf(
     outFILE,
     "    o Paf file: minimap2 reference.fa reads.fq\n"
   );


   fprintf(
     outFILE,
     "    o You can use \"-\" to specify stdin input\n"
   );


   /*****************************************************\
   * Fun-02 Sec-02 Sub-02:
   *   - Print out the -sam entry (mapped reads)
   \*****************************************************/

   fprintf(outFILE, "  -sam: [Required (stdin)]\n");

   fprintf(
     outFILE,
     "    o Sam file with reads mapped to the same"
   );

   fprintf(outFILE, "\n");

   fprintf(
      outFILE,
      "      reference genome as file from -paf\n"
   );


   fprintf(
     outFILE,
     "    o You can use \"-\" to specify stdin input\n"
   );


   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Print out the -out entry (output file)
   \*****************************************************/

   fprintf(outFILE, "  -out: [stdout]\n");

   fprintf(
     outFILE,
     "    o  File to output the tsv file to\n"
   );

   fprintf(
     outFILE,
     "    o You can use \"-\" to specify stdout output\n"
   );


   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Print out the -min-depth entry (min read depth)
   \*****************************************************/

   fprintf(outFILE, "  -min-depth: [%u]\n", defMinDepth);

   fprintf(
     outFILE,
     "    o  Minimum read depth to count base as part of"
   );

   fprintf(outFILE, "\n       an amplicon\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Print out the -flag entry (string for column 1)
   \*****************************************************/

   fprintf(outFILE, "  -flag: [%s]\n", globalExtraCol);

   fprintf(
     outFILE,
     "    o  Flag to add to the first column\n"
   );


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print out the program output lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "  Output:"
   );

   fprintf(
      outFILE,
      "  - Prints the tsv to the file provided by -out\n"
   );

   fprintf(
      outFILE,
      "    o Column 1 is the string from -flag\n"
   );

   fprintf(
      outFILE,
      "    o Column 2 is the ampicon starting base\n"
   );

   fprintf(
      outFILE,
      "    o Column 3 is the ampicon ending base\n"
   );

   fprintf(
      outFILE,
      "    o Column 4 is the mean amplicon read depth\n"
   );

   fprintf(
     outFILE,
     "    o Column 5 is the minimum amplicon read depth\n"
   );


   fprintf(
     outFILE,
     "    o Column 6 is the maximum amplicon read depth\n"
   );


   fprintf(
    outFILE,
    "    o  Remaining columns are specific to each gene\n"
   );


   fprintf(
      outFILE,
      "      and are read depth of first base, read depth" 
   );

   fprintf(outFILE, "\n");

   fprintf(
       outFILE,
       "of last base, mean read depth, minimum read depth"
   );

   fprintf(outFILE, "\n");


   fprintf(
       outFILE,
       "maximum read depth, and the gene name\n"
   );
} /*pAmpDepthHelp*/
