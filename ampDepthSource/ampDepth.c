/*########################################################
# Name: ampDepth
#  - Makes a tsv file with the read depths for detected
#    amplicons in a sam file. It includes the genes names
#    in each amplicon and the read depths for each gene in
#    an amplicon
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
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, defintions, and global variables
\-------------------------------------------------------*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../generalLib/samEntryStruct.h"
#include "../generalLib/trimSam.h"
#include "../generalLib/geneCoordStruct.h"

#include "ampDepth-fun.h"

/*No .c files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"
#include "../generalLib/genMath.h"
#include "ampDepth-version.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependencies:
!   o .h #include "../generalLib/numToStr.h"
!   o .h #include "../generalLib/ntToFiveBit.h"
!   o .h #include "../generalLib/numToStr.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
   '    - Print out the histogram
   '  o main sec-10:
   '    - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input*/
   char *geneTblFileStr = 0;
   char *samStr = 0;
   char *outStr = 0;
   int minDepthI = defMinDepth;
   char *extraColStr = globalExtraCol;

   char *errStr = 0; /*User inpupt invalid paramater*/
   uchar errUC = 0;  /*Error report*/
   ulong errUL = 0;  /*For gene table reading errors*/
   
   int numGenesI = 0;

   uint *readMapHeapAryUI = 0; /*Mapped region of reads*/
   uint offTargUI = 0;
   uint umappedUI = 0;

   struct geneCoords *genesHeapST = 0; 

   struct samEntry readST;
   char *samBuffHeapStr = 0;
   ulong lenSamBuffUL = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^  - Get and check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errStr = 
      ampDepthGetCmdInput(
         argsStrAry,  /*User input arguments*/
         numArgsI,    /*Number of arguments input*/
         &geneTblFileStr, /*Has gene coordinates*/
         &samStr,     /*Name/path to sam file*/
         &outStr,     /*Name/path to output file*/
         &minDepthI,  /*Min depth to use*/
         &extraColStr /*Flag to add for data*/
     ); /*Get the user input*/

   if(errStr != 0)
   { /*If: I had an error*/
     if(   ! cStrEql("-h", errStr, '\0')
        || ! cStrEql("--h", errStr, '\0')
        || ! cStrEql("-help", errStr, '\0')
        || ! cStrEql("--help", errStr, '\0')
        || ! cStrEql("help", errStr, '\0')
     ){ /*If: the user requested the help message*/
        pAmpDepthHelp(stdout);
        exit(0);
     } /*If: the user requested the help message*/


     if(   ! cStrEql("-v", errStr, '\0')
        || ! cStrEql("--v", errStr, '\0')
        || ! cStrEql("-version", errStr, '\0')
        || ! cStrEql("--version", errStr, '\0')
        || ! cStrEql("version", errStr, '\0')
     ){ /*If: the user requested the version number*/
        fprintf(
           stdout,
           "ampDepth version: %i-%02i-%02i\n",
           def_year_ampDepth,
           def_month_ampDepth,
           def_day_ampDepth
        );

        exit(0);
     } /*If: the user requested the version number*/
 

     if(! cStrEql("-min-depth", errStr, '\0'))
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
   ^    - Check the sam file (has reads)
   ^  o main sec-03 sub-02:
   ^   - Check the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Check the input sam file with reads
   \*****************************************************/

   if(samStr == 0 || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: the user supplied a sam file*/
      samFILE = fopen(samStr, "r");

      if(samFILE == 0)
      { /*If: the file could not be opened*/
         fprintf(
            stderr,
            "-sam %s could not be opened\n",
            samStr
          );

          goto errCleanUp_main;
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check the output file
   \*****************************************************/

   if(outStr == 0 || outStr[0] == '-') outFILE = stdout;
 
   else
   { /*Else: the user supplied a sam file*/
      outFILE = fopen(outStr, "w");

      if(outFILE == 0)
      { /*If: the file could not be opened*/
         fprintf(
            stderr,
            "-out %s could not be opened\n",
            outStr
          );

          goto errCleanUp_main;
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Extract the genes from the paf file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   genesHeapST =
      getGeneCoords(
         geneTblFileStr,
         &numGenesI,
         &errUL
      ); /*Get the gene coordinates from the file*/

   if(! genesHeapST)
   { /*If: I had an error*/
      if(errUL & def_fileErr_geneCoord)
      { /*If: This was an file error*/;
         fprintf(
            stderr,
            "Could not open %s\n",
            geneTblFileStr
         );
      } /*If: This was an file error*/

      else if(errUL & def_memErr_geneCoord)
      { /*Else If: This was an memory error*/;
         fprintf(
            stderr,
            "Memory error when reading in %s\n",
            geneTblFileStr
         );
      } /*Else If: This was an memory error*/

      else if(errUL & def_invalidEntry_geneCoord)
      { /*Else If: This was an memory error*/;
         fprintf(
            stderr,
            "Line number %lu in %s is not valid\n",
            (errUL >> 8),
            geneTblFileStr
         );
      } /*Else If: This was an memory error*/

      goto errCleanUp_main;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^  - Get the read mapping tables set up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   offTargUI = genesHeapST->endAryUI[numGenesI] + 2;
     /*These are genes that did not map to the end of the
     ` gene range, but still mapped to the reference
     ` The + 2 is to account for index 1 and to ensure
     `   thre array ends in 0
     */
   readMapHeapAryUI = calloc(offTargUI, sizeof(uint));

   if(readMapHeapAryUI == 0)
   { /*If: I had a memory error*/
      fprintf(stderr, "Memory error (main sec-05)\n");
      goto errCleanUp_main;
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

   errUC =
      readSamLine(
          &readST,
          &samBuffHeapStr,
          &lenSamBuffUL,
          samFILE
      );

   offTargUI = 0;

   while(!errUC)
   { /*Loop: read in the samfile*/
      /*check if this is a header line*/
      if(readST.extraStr[0] == '@') goto nextLine;

      if(readST.flagUS & 4)
      { /*If: the read was unmapped*/
         ++umappedUI;
         goto nextLine;
      } /*If: the read was unmapped*/

      trimSamEntry(&readST); /*Trim off softmasked bases*/

      /**************************************************\
      * Main Sec-06 Sub-02:
      *   - Handle secondary and supplemental mapped reads
      \**************************************************/

      if(readST.flagUS & (256 | 2048)) goto nextLine;

      /**************************************************\
      * Main Sec-06 Sub-03:
      *   - Handle primary mapped reads
      \**************************************************/

      addBaseToAmpDepth(
         &readST,
         genesHeapST,
         numGenesI,
         readMapHeapAryUI,
         offTargUI
      ); /*Add in the coverd bases to the histogram*/

      nextLine:;

      errUC =
         readSamLine(
             &readST,
             &samBuffHeapStr,
             &lenSamBuffUL,
             samFILE
         );
   } /*Loop: read in the samfile*/

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   free(samBuffHeapStr);
   samBuffHeapStr = 0;
   lenSamBuffUL = 0;

   if(errUC & 64)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "Memory error while reading sam file\n"
      );

      goto errCleanUp_main;
   } /*If: I had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-08:
   ^  - Print out the header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   pHeaderAmpDepth(outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-09:
   ^   - Print out the histogram
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   pAmpDepthHistogram(
      (int *) readMapHeapAryUI,
      minDepthI,
      genesHeapST,
      numGenesI,
      (int) offTargUI,
      (int) umappedUI,
      extraColStr,
      outFILE
   ); /*Print out the histogram*/

   if(outFILE != stdout) fclose(outFILE);
   outFILE = 0;

   freeGeneCoords(genesHeapST); /*Finished with this*/
   genesHeapST = 0;

   free(readMapHeapAryUI);      /*Finished with this*/
   readMapHeapAryUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-10:
   ^   - Clean up
   ^   o main sec-10 sub-01:
   ^     - Clean up after success (nothing)
   ^   o main sec-10 sub-02:
   ^     - Clean up after an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-10 Sub-01:
   *   - Clean up after success (nothing)
   \*****************************************************/

   exit(0);

   /*****************************************************\
   * Main Sec-10 Sub-02:
   *   - Clean up after an error
   \*****************************************************/

   errCleanUp_main:;

   free(samBuffHeapStr);
   samBuffHeapStr = 0;
   lenSamBuffUL = 0;

   freeGeneCoords(genesHeapST);
   genesHeapST = 0;

   free(readMapHeapAryUI);
   readMapHeapAryUI = 0;

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   exit(-1);
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

      if(! cStrEql("-gene-tbl", parmStr, '\0'))
         *pafStr = argStr;

      else if(! cStrEql("-sam", parmStr, '\0'))
         *samStr=argStr;

      else if(! cStrEql("-out", parmStr, '\0'))
         *outStr=argStr;

      else if(! cStrEql("-min-depth", parmStr, '\0'))
      { /*Else if: the user input a min depth*/
         errStr = base10StrToSI(argStr, *minDepthI);
         if(errStr[0] > 32) return parmStr;
      } /*Else if: the user input a min depth*/

      else if(! cStrEql("-flag", parmStr, '\0'))
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
   ' TOC:paMpDepthHelp
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
     "ampDepth -gene-tbl gene-tbl.tsv -sam reads.sam\n"
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
   ^     - Print out -gene-tbl entry (gene coordinates)
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
   *   - Print out the -gene-tbl entry (gene coordinates)
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:\n"
   );

   fprintf(outFILE, "  -gene-tbl: [Required]\n");

   fprintf(
     outFILE,
     "    o Tsv file with the coordiantes of each gene in"
   );

   fprintf(outFILE, "\n");

   fprintf(
      outFILE,
      "      the reference genome\n"
   );


   fprintf(
     outFILE,
     "    o Format: \n"
   );

   fprintf(
     outFILE,
     "      - There is no header (ref = reference)\n"
   );

   fprintf(
     outFILE,
     "      - gene_name\\tref_name\\tdirection"
   );
   fprintf(
     outFILE,
     "\\tref_start\\tref_end\n"
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
     "    o File to output the tsv file to\n"
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
     "    o Minimum read depth to count base as part of"
   );

   fprintf(outFILE, "\n      an amplicon\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Print out the -flag entry (string for column 1)
   \*****************************************************/

   fprintf(outFILE, "  -flag: [%s]\n", globalExtraCol);

   fprintf(
     outFILE,
     "    o Flag to add to the first column\n"
   );


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print out the program output lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:\n"
   );

   fprintf(
      outFILE,
      "  - Prints the tsv to the file provided by -out\n"
   );

   fprintf(
      outFILE,
      "    o 01: Value input for -flag\n"
   );

   fprintf(
      outFILE,
      "    o 02: Number marking wich amplicon on\n"
   );

   fprintf(
      outFILE,
      "    o 03: First base of first mapped gene\n"
   );

   fprintf(
      outFILE,
      "    o 04: Last base of last mapped gene\n"
   );

   fprintf(
      outFILE,
      "    o 05: First reference base in amplicon\n"
   );

   fprintf(
      outFILE,
      "    o 06: Last reference base in amplicon\n"
   );

   fprintf(
      outFILE,
      "    o 07: Mean read depth for amplicon\n"
   );

   fprintf(
      outFILE,
      "    o 08: Minimum read depth for amplicon\n"
   );


   fprintf(
      outFILE,
      "    o 09: Maximum read depth for amplicon\n"
   );

   fprintf(
      outFILE,
      "    o 10: Gene name\n" 
   );

   fprintf(
      outFILE,
      "    o 11: Start of gene on reference\n" 
   );

   fprintf(
      outFILE,
      "    o 12: End of gene on reference\n" 
   );

   fprintf(
      outFILE,
      "    o 13: Read depth of first base in gene\n" 
   );

   fprintf(
      outFILE,
      "    o 14: Read depth of last base in gene\n" 
   );

   fprintf(
      outFILE,
      "    o 15: Mean read depth for gene\n" 
   );

   fprintf(
      outFILE,
      "    o 16: Minimum read depth for gene\n" 
   );

   fprintf(
      outFILE,
      "    o 17: Maximum read depth for gene\n" 
   );
} /*pAmpDepthHelp*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
