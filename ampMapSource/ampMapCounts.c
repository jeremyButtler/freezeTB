/*#######################################################\
# Name: ampMapCounts
#   - Prints out the number of reads that mapped to each
#     gene in the sam file
# Libraries:
#  - "refAvlTree.h"
#  o "refStruct.h"
#  o "../generalLib/dataTypeShortHand.h"
#  - "../generalLib/samEntryStruct.h"
#  o "../generalLib/base10StrToNum.h"
# C Standard Libraries:
#  o "<string.h>"
#  o "<stdlib.h>"
#  o "<stdio.h>"
#  o "<stdint.h>"
\#######################################################*/

#include "refAvlTree.h"
#include "../generalLib/samEntryStruct.h"

/*-------------------------------------------------------\
| Fun-01: getInput
|   - Get the user input from argsAryStr
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argsAryStr:
|     o Array of arguments the  user input
|   - samStr:
|     o Pointer to c-string to hold the path to the sam
|       file
|   - formatBl:
|     o Pointer to a char to hold if printing out the
|       mapping table as a tsv (1) or markdown (0)
|   - pLinksBl:
|     o Pointer to character to hold if printing out a
|       supplemental alignment table (1) or skip (0)
|   - pMapTblBl:
|     o Pointer to a character to hold if printing out a
|       mapping table (1) or skip (0)
|   - mapOutStr:
|     o Pointer to c-string to hold the mapping table
|   - supTblOutStr:
|     o Pointer to c-string to hold the supplemental table
| Output:
|   - Modifies:
|     o All input parameters to hold the selected values
|   - Returns:
|     o 0 if no problems
|     o pointer to argument errored out on
\-------------------------------------------------------*/
char * getInput(
   int numArgsI,       /*Number of arguments input*/
   char *argsAryStr[], /*User input arguments*/
   char **samStr,   /*Sam file to extract from*/
   char *formatBl,  /*1: tsv output; 0: markdown*/
   char *pLinksBl,  /*Print multiple mappings for reads*/
   char *pMapTblBl, /*Print table of mapped genes*/
   char **mapOutStr, /*Mapping file*/
   char **supTblOutStr /*Print the supplemental table*/
); /*getInput*/

int main(
   int numArgsI,
   char *argsAryStr[]
){
   /*char *samStr = "tmp.sam";*/
   char *samStr = "full.sam";
   char formatBl = 0;
   char pLinksBl = 1;/*Print multiple mappings for reads*/
   char pMapTblBl = 1;     /*Print table of mapped genes*/
   char *mapOutStr = 0;
   char *supTblOutStr = 0;

   char *inputErrStr = 0;
   uchar errUC = 0;        /*Holds errors from functions*/
   ulong samFileLineUL = 0; /*Line at in sam file*/
   ulong numUnmappedUL = 0; /*Number of unmapped reads*/
   ulong minStartUL = 0; /*First base it can be at*/

   struct refStruct *treeST = 0;
   struct refStruct *supAlnST = 0;
   struct refStruct *tmpRef = 0;

   struct refSTStack stackST; /*For printing*/

   struct samEntry readST;

   FILE *samFILE = 0;
   FILE *mapOutFILE = 0;
   FILE *supOutFILE = 0;

   char *helpStr =
      "ampMapCounts -sam samStr\
       \n  -sam: [Required]\
       \n    o Sam file to get stats for (\"-\" = stdin)\
       \n  -map-out: [-map-out stdout]\
       \n    o File to output mapping table to\
       \n      (\"-\" = stdout)\
       \n  -sup-out: [-sup-out stdout]\
       \n    o File to output supplemental table to\
       \n      (\"-\" = stdout)\
       \n  -map-tbl: [-map-tbl]\
       \n    o Print out the mapping table\
       \n    o Disable with -no-map-tbl\
       \n  -sup-tbl: [-sup-tbl]\
       \n    o Print out the supplemental alignment table\
       \n    o Disable with -no-sup-tbl\
       \n  -tsv: [-markdown]\
       \n    o Print out the mapping table in tsv format\
       \n    o Use -markdown for markdown format\
       ";

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Get and check user input
   ^   o main sec-02 sub-01:
   ^     - Get user input
   ^   o main sec-02 sub-02:
   ^     - Check if the output files are valid
   ^   o main sec-02 sub-03:
   ^     - Open the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-02 Sub-01:
   *   - Get user input
   \*****************************************************/

   inputErrStr =
      getInput(
         numArgsI,      /*Number of arguments input*/
         argsAryStr,    /*User input arguments*/
         &samStr,       /*Sam file to extract from*/
         &formatBl,     /*1: tsv output; 0: markdown*/
         &pLinksBl,     /*Print sup mappings for reads*/
         &pMapTblBl,    /*Print table of mapped genes*/
         &mapOutStr,    /*Mapping file*/
         &supTblOutStr  /*Print the supplemental table*/
      ); /*Get user input*/

   if(inputErrStr != 0)
   { /*If: the user input an invalid argument*/
      if(   !strEqual("-h", inputErrStr)
         || !strEqual("--h", inputErrStr)
         || !strEqual("-help", inputErrStr)
         || !strEqual("--help", inputErrStr)
         || !strEqual("help", inputErrStr)
      ){ /*If: Printing the help message*/
         printf("%s\n", helpStr);
         exit(0);
      } /*If: Printing the help message*/

      else
      { /*Else: print out the problem input*/
         fprintf(stderr,"%s is not valid\n",inputErrStr);
         exit(-1);
      } /*Else: print out the problem input*/
   } /*If: the user input an invalid argument*/

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Check if the output files are valid
   \*****************************************************/

   if(mapOutStr == 0 || *mapOutStr == '-')
      mapOutFILE = stdout;

   else
   { /*Else: the user provided a file*/
      mapOutFILE = fopen(mapOutStr, "w");

      if(mapOutFILE == 0)
      { /*If: the input file could not be opened*/
         fprintf(
            stderr,
            "-map-out %s could not be opened\n",
            mapOutStr
         );

         exit(-1);
      } /*If: the input file could not be opened*/
   } /*Else: the user provided a file*/

   if(supTblOutStr == 0 || *supTblOutStr == '-')
      supOutFILE = stdout;

   else
   { /*Else: the user provided a file*/
      supOutFILE = fopen(supTblOutStr, "w");

      if(supOutFILE == 0)
      { /*If: the input file could not be opened*/
         fprintf(
            stderr,
            "-sup-out %s could not be opened\n",
            supTblOutStr
         );

         fclose(mapOutFILE);
         exit(-1);
      } /*If: the input file could not be opened*/
   } /*Else: the user provided a file*/
   
   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Open the sam file
   \*****************************************************/

   blankRefSTStack(&stackST);
   initSamEntry(&readST);

   if(samStr[0] == '-') samFILE = stdin;
   else samFILE = fopen(samStr, "r");

   if(samFILE == 0)
   { /*If: I could not open the sam file*/
      fprintf(stderr, "Could not open -sam %s\n", samStr);
      fclose(mapOutFILE);
      fclose(supOutFILE);
      exit(-1);
   } /*If: I could not open the sam file*/

   /*Read each line in the sam file*/
   errUC = readSamLine(&readST, samFILE);
   ++samFileLineUL;

   if(errUC & 2)
   { /*If: the file had nothing in it*/
      fprintf(stderr,"-sam %s is a blank file\n",samStr);

      fclose(samFILE);
      samFILE = 0;
      freeStackSamEntry(&readST);
      fclose(mapOutFILE);
      fclose(supOutFILE);

      exit(-1);
   } /*If: the file had nothing in it*/

   if(errUC & 64)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "The first line of -sam %s is exceeds ram\n",
         samStr
      );

      fclose(samFILE);
      samFILE = 0;
      freeStackSamEntry(&readST);
      fclose(mapOutFILE);
      fclose(supOutFILE);

      exit(-1);
   } /*If: I had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Build tree with mapping counts from the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(errUC == 1)
   { /*Loop: Until an error in the sam file is found*/
      /*Check if the line is a header entry*/
      if(*readST.samEntryCStr == '@') goto nextLine;

      if(readST.flagUSht & 4) 
      { /*If: the read was unmapped*/
         ++numUnmappedUL;
         supAlnST = 0;  /*Next alignment is primary*/
         goto nextLine; /*Get next entry & restart loop*/
      } /*If: the read was unmapped*/

      tmpRef = findGeneInTree(readST.refCStr, &treeST);

      /*I think supplementary alignments refere to
      ` different references, while secondary alignments
      ` referes to the same reference. I need to look
      ` this up
      */
      if(readST.flagUSht & 2048)
      { /*If: was a supplementary alignment*/
         /*Check if this mapping is a continuation of an
         ` alignment or is a random mapping
         */
         if(   readST.posOnRefUInt > minStartUL
            && minStartUL > 0
         ) goto nextLine;

         ++tmpRef->totalAlnUL;

         supAlnST =
            findGeneInTree(
               readST.refCStr,
               &(supAlnST->link)
         );
         
         ++supAlnST->totalAlnUL;
      } /*If: was a supplementary alignment*/

      else if(readST.flagUSht & 256) goto nextLine;

      else
      { /*Else: is a primary alignment*/
         ++tmpRef->totalAlnUL;
         supAlnST = tmpRef;
         ++supAlnST->numPrimaryAlnUL;
      } /*Else: is a primary alignment*/

      nextLine:

      blankSamEntry(&readST);
      errUC = readSamLine(&readST, samFILE);
      ++samFileLineUL;
   } /*Loop: Until an error in the sam file is found*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Print out the number of mappings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fclose(samFILE);
   samFILE = 0;
   freeStackSamEntry(&readST);
   supAlnST = 0;

   if(errUC & 64)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "Ran out of memory at line %lu in -sam %s\n",
         samFileLineUL,
         samStr
      );

      freeRefForest(&treeST);
      fclose(mapOutFILE);
      fclose(supOutFILE);

      exit(-1);      
   } /*If: I had a memory error*/

   /*Add in unmapped reads*/
   tmpRef = findGeneInTree("*", &treeST);
   tmpRef->totalAlnUL = numUnmappedUL;
   tmpRef->numPrimaryAlnUL = numUnmappedUL;
   tmpRef = 0;

   if(pMapTblBl) pRefMapTbl(treeST,formatBl,1,mapOutFILE);

   if(pMapTblBl && pLinksBl && mapOutFILE == supOutFILE)
      fprintf(supOutFILE, "\n");

   if(pLinksBl) pAmpMapTsv(treeST, &stackST, supOutFILE);

   freeRefForest(&treeST);
   fclose(mapOutFILE);
   fclose(supOutFILE);
   exit(0);
} /*main*/

/*-------------------------------------------------------\
| Fun-01: getInput
|   - Get the user input from argsAryStr
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argsAryStr:
|     o Array of arguments the  user input
|   - samStr:
|     o Pointer to c-string to hold the path to the sam
|       file
|   - formatBl:
|     o Pointer to a char to hold if printing out the
|       mapping table as a tsv (1) or markdown (0)
|   - pLinksBl:
|     o Pointer to character to hold if printing out a
|       supplemental alignment table (1) or skip (0)
|   - pMapTblBl:
|     o Pointer to a character to hold if printing out a
|       mapping table (1) or skip (0)
|   - mapOutStr:
|     o Pointer to c-string to hold the mapping table
|   - supTblOutStr:
|     o Pointer to c-string to hold the supplemental table
| Output:
|   - Modifies:
|     o All input parameters to hold the selected values
|   - Returns:
|     o 0 if no problems
|     o pointer to argument errored out on
\-------------------------------------------------------*/
char * getInput(
   int numArgsI,       /*Number of arguments input*/
   char *argsAryStr[], /*User input arguments*/
   char **samStr,   /*Sam file to extract from*/
   char *formatBl,  /*1: tsv output; 0: markdown*/
   char *pLinksBl,  /*Print multiple mappings for reads*/
   char *pMapTblBl, /*Print table of mapped genes*/
   char **mapOutStr, /*Mapping file*/
   char **supTblOutStr /*Print the supplemental table*/
){ /*getInput*/
   char *parmStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < numArgsI; ++iArg)
   { /*Loop: Get the user input*/
      parmStr = argsAryStr[iArg];

      if(!strEqual("-sam", parmStr))
      { /*If: the user provided a sam file*/
         ++iArg;
         *samStr = argsAryStr[iArg];
      } /*If: the user provided a sam file*/

      else if(!strEqual("-tsv", parmStr))
         *formatBl = 1;

      else if(!strEqual("-markdown", parmStr))
         *formatBl = 0;

      else if(!strEqual("-sup-tbl", parmStr))
         *pLinksBl = 1;

      else if(!strEqual("-no-sup-tbl", parmStr))
         *pLinksBl = 0;

      else if(!strEqual("-map-tbl", parmStr))
         *pMapTblBl = 1;

      else if(!strEqual("-no-map-tbl", parmStr))
         *pMapTblBl = 0;

      else if(!strEqual("-map-out", parmStr))
      { /*Else If: the user provided an output file*/
          ++iArg;
          *mapOutStr = argsAryStr[iArg];
      } /*Else If: the user provided an output file*/

      else if(!strEqual("-sup-out", parmStr))
      { /*Else If: the user provided an output file*/
          ++iArg;
          *supTblOutStr = argsAryStr[iArg];
      } /*Else If: the user provided an output file*/

      else return parmStr;
   } /*Loop: Get the user input*/

   return 0;
} /*getInput*/