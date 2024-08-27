/*########################################################
# Name: ampDepth
#  - Makes a tsv file with the read depths for detected
#    amplicons in a sam file. It includes the genes names
#    in each amplicon and the read depths for each gene in
#    an amplicon
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'    - included libraries and defined/global variables
'   o fun02: phelp_ampDepthMain
'     - prints the help message for ampDepth
'   o fun03: input_ampDepth
'     - gets the command line input supplied by the user
'   o main:
'     - prints out a tsv with the mean, minimum, and
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined/global variables
\-------------------------------------------------------*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/base10str.h"
#include "../genLib/samEntry.h"
#include "../genLib/trimSam.h"
#include "../genLib/geneCoord.h"

#include "ampDepth.h"

/*No .c files*/
#include "../genLib/dataTypeShortHand.h"
#include "ampDepthDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/numToStr.h"
!   - .h  #include "../genLib/genMath.h" using macros
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

signed char *globalExtraCol = (signed char *) "out";

/*-------------------------------------------------------\
| Fun01: pversoin_ampDepthMain
|   - prints vesion number
| Input:
|   - outFILE:
|     o file to print the version number to
| Output:
|  - Prints:
|    o prints versionnumbe to outFILE
\-------------------------------------------------------*/
void pversion_ampDepthMain(
   void *outFILE
){

   fprintf(
      (FILE *) outFILE,
      "ampDepth version: %i-%02i-%02i\n",
      def_year_ampDepthDefs,
      def_month_ampDepthDefs,
      def_day_ampDepthDefs
   );
} /*pversion_ampDepthMain*/

/*-------------------------------------------------------\
| Fun02: phelp_ampDepthMain
|   - prints the help message for ampDepth
| Input:
|   - (FILE *) outFILE:
|     o file to print the help message to
| Output:
|  - Prints:
|    o prints the help message to (FILE *) outFILE
\-------------------------------------------------------*/
void phelp_ampDepthMain(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - print the help message for ampDepth
   '   o fun02 sec01:
   '      - print out the usage and description lines
   '   o fun02 sec02:
   '      - print out the user input ilneS
   '   o fun02 sec03:
   '      - print out the program output lines
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - Print out the usage and description lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*General*/
   fprintf(
     (FILE *) outFILE,
     "ampDepth -gene-tbl gene-tbl.tsv -sam reads.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints out a tsv file with the mean, min, and"
   );

   fprintf((FILE *) outFILE, "\n");

   fprintf(
      (FILE *) outFILE,
      "    and max read depths for each amplicon. It also"
   );

   fprintf((FILE *) outFILE, "\n");

   fprintf(
      (FILE *) outFILE,
      "    incudes a list of genes in each amplicon"
   );

   fprintf((FILE *) outFILE, "\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - Print out the user input ilneS
   ^   o fun02 sec02 sub01:
   ^     - Print out -gene-tbl entry (gene coordinates)
   ^   o fun02 sec02 sub02:
   ^     - Print out the -sam entry (mapped reads)
   ^   o fun02 sec02 sub03:
   ^     - Print out the -out entry (output file)
   ^   o fun02 sec02 sub04:
   ^     - Print out the -min-depth entry (min read depth)
   ^   o fun02 sec02 sub05:
   ^     - Print out the -flag entry (string for column 1)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - Print out the -gene-tbl entry (gene coordinates)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   fprintf((FILE *) outFILE, "  -gene-tbl: [Required]\n");

   fprintf(
     (FILE *) outFILE,
     "    o Tsv file with the coordiantes of each gene in"
   );

   fprintf((FILE *) outFILE, "\n");

   fprintf(
      (FILE *) outFILE,
      "      the reference genome\n"
   );


   fprintf(
     (FILE *) outFILE,
     "    o Format: \n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - There is no header (ref = reference)\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - gene_name\\tref_name\\tdirection"
   );
   fprintf(
     (FILE *) outFILE,
     "\\tref_start\\tref_end\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - Print out the -sam entry (mapped reads)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam: [Required (stdin)]\n"
   );

   fprintf(
     (FILE *) outFILE,
     "    o Sam file with reads mapped to the same"
   );

   fprintf((FILE *) outFILE, "\n");

   fprintf(
      (FILE *) outFILE,
      "      reference genome as file from -paf\n"
   );


   fprintf(
     (FILE *) outFILE,
     "    o You can use \"-\" to specify stdin input\n"
   );


   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - Print out the -out entry (output file)
   \*****************************************************/

   fprintf((FILE *) outFILE, "  -out: [stdout]\n");

   fprintf(
     (FILE *) outFILE,
     "    o File to output the tsv file to\n"
   );

   fprintf(
     (FILE *) outFILE,
     "    o You can use \"-\" to specify stdout output\n"
   );


   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - Print out the -min-depth entry (min read depth)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -min-depth: [%u]\n", def_minDepth_ampDepthDefs
   );

   fprintf(
     (FILE *) outFILE,
     "    o Minimum read depth to count base as part of"
   );

   fprintf((FILE *) outFILE, "\n      an amplicon\n");

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - Print out the -flag entry (string for column 1)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -flag: [%s]\n",
      globalExtraCol
   );

   fprintf(
     (FILE *) outFILE,
     "    o Flag to add to the first column\n"
   );


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - Print out the program output lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints the tsv to the file provided by -out\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 01: Value input for -flag\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 02: Number marking wich amplicon on\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 03: First base of first mapped gene\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 04: Last base of last mapped gene\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 05: First reference base in amplicon\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 06: Last reference base in amplicon\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 07: Mean read depth for amplicon\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 08: Minimum read depth for amplicon\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    o 09: Maximum read depth for amplicon\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o 10: Gene name\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 11: Start of gene on reference\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 12: End of gene on reference\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 13: Read depth of first base in gene\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 14: Read depth of last base in gene\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 15: Mean read depth for gene\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 16: Minimum read depth for gene\n" 
   );

   fprintf(
      (FILE *) outFILE,
      "    o 17: Maximum read depth for gene\n" 
   );
} /*phelp_ampDepthMain*/

/*-------------------------------------------------------\
| Fun03: input_ampDepth
|   - Gets the command line input supplied by the user
| Input:
|   - argAryStr:
|     o c-string array with user input
|   - numArgsSI:
|     o number of strings in argAryStr
|   - tblStr:
|     o pointer to pointer to path to gene table file
|   - samStr:
|     o pointer to hold the path of the input sam file
|   - outStr:
|     o pointer to hold the file to output the tsv to
|   - minDepthSI:
|     o minimum read depth to not split up an amplicon
|   - extraColStr:
|     o will point to the flag (c-string) to add to the
|       first column of the output tsv
| Output:
|  - Modifies:
|    o tblStr, samStr, outStr, minDepthSI, and extraColStr
|      to hold the user supplied values
|  - Returns:
|    o 0 for no problems
|    o Pointer to invalid arguement (error)
\-------------------------------------------------------*/
signed char
input_ampDepth(
   char *argAryStr[],     /*user input arguments*/
   int numArgsSI,         /*number of arguments input*/
   signed char **tblStr,  /*name/path to gene table file*/
   signed char **samStr,  /*name/path to sam file*/
   signed char **outStr,  /*name/path to output file*/
   signed int *minDepthSI,/*min depth to use*/
   signed char **extraColStr /*flag to add for data*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - get errors and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   schar *errStr = 0;
   sint siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_ampDepthMain(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec01 sub01:
   ^     - get file io and start loo;
   ^   o fun03 sec01 sub02:
   ^     - get flag and min depth
   ^   o fun03 sec01 sub03:
   ^     - help message requests
   ^   o fun03 sec01 sub04:
   ^     - version number requests
   ^   o fun03 sec01 sub05:
   ^     - invalid input
   ^   o fun03 sec01 sub06:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec01 Sub01:
   *   - get file io and start loo;
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: Process user input*/
      if(
         ! eql_charCp(
            (schar *) "-gene-tbl",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) { /*If: is gene table input*/
         ++siArg;
         *tblStr = (schar *) argAryStr[siArg];
      } /*If: is gene table input*/

      else if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) { /*Else If: sam file input*/
         ++siArg;
         *samStr = (schar *) argAryStr[siArg];
      } /*Else If: sam file input*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) { /*Else If: output file input*/
         ++siArg;
         *outStr = (schar *) argAryStr[siArg];
      } /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec01 Sub02:
      *   - get flag and min depth
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-flag",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) { /*Else If: output file input*/
         ++siArg;
         *extraColStr = (schar *) argAryStr[siArg];
      } /*Else If: output file input*/

      else if(
         ! eql_charCp(
            (schar *) "-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) { /*Else If: output file input*/
         ++siArg;
         errStr = (schar *) argAryStr[siArg];

         errStr +=
            strToSI_base10str(
               errStr,
               minDepthSI
            );

         if(*errStr != '\0')
         { /*If: input to large*/
            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large\n",
               errStr
            );

            goto err_fun03_sec04;
         } /*If: input to large*/
      } /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec01 Sub03:
      *   - help message requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted help message*/
         phelp_ampDepthMain(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted help message*/
         phelp_ampDepthMain(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted help message*/
         phelp_ampDepthMain(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted help message*/
         phelp_ampDepthMain(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted help message*/
         phelp_ampDepthMain(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: user wanted help message*/

      /**************************************************\
      * Fun03 Sec01 Sub04:
      *   - version number requests
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted version number*/
         pversion_ampDepthMain(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: user wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted version number*/
         pversion_ampDepthMain(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: user wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted version number*/
         pversion_ampDepthMain(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: user wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted version number*/
         pversion_ampDepthMain(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: user wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            '\0'
         )
      ){ /*Else If: user wanted version number*/
         pversion_ampDepthMain(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: user wanted version number*/

      /**************************************************\
      * Fun03 Sec01 Sub05:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec01 Sub06:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Process user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - get errors and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;

   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
   errSC = 1;
   goto ret_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;

   return errSC;
} /*input_ampDepth*/



/*-------------------------------------------------------\
| Main:
|   - Prints out a tsv with the mean, minimum, and
|     maximum read depths for each amplicon.
| Input:
|   - numArgsSI:
|     o Number of arguments/parameters the user input
|   - argAryStr:
|     o User input
| Output:
|  - Prints:
|    o The tsv file mentioned in phelp_ampDepthMain
\-------------------------------------------------------*/
#ifdef PLAN9
char
#else
int
#endif
main(
   int numArgsSI,       /*Number of arguments input*/
   char *argAryStr[]  /*User input arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' TOC: Main
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get and check/open user input
   '   o main sec03:
   '     - set up (read in coordinates/memory allocation)
   '   o main sec04:
   '     - build histogram from reads
   '   o main sec05:
	'     - print histogram and finish cleaning up
   '   o main sec06:
   '     - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^  - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input*/
   schar *geneTblFileStr = 0;
   schar *samStr = 0;
   schar *outStr = 0;
   sint minDepthSI = def_minDepth_ampDepthDefs;
   schar *extraColStr = globalExtraCol;

   schar errSC = 0;  /*Error report*/
   ulong errUL = 0;  /*For gene table reading errors*/
   
   sint numGenesSI = 0;

   sint *readMapHeapArySI = 0; /*Mapped region of reads*/
   sint offTargSI = 0;
   sint noMapSI = 0;

   struct geneCoord *genesHeapST = 0; 

   struct samEntry samStackST;
   schar *samBuffHeapStr = 0;
   ulong lenSamBuffUL = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check/open user input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - open input sam file
   ^   o main sec02 sub04:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC = 
      input_ampDepth(
         argAryStr,  /*User input arguments*/
         numArgsSI,    /*Number of arguments input*/
         &geneTblFileStr, /*Has gene coordinates*/
         &samStr,     /*Name/path to sam file*/
         &outStr,     /*Name/path to output file*/
         &minDepthSI,  /*Min depth to use*/
         &extraColStr /*Flag to add for data*/
     ); /*Get the user input*/

   if(errSC)
   { /*If: I had an error*/
      --errSC; /*convert help/version to 0 (no erro)*/
      goto cleanUp_main_sec06_sub03;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open input sam file
   \*****************************************************/

   if(samStr == 0 || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: the user supplied a sam file*/
      samFILE =
         fopen(
            (char *) samStr,
            "r"
         );

      if(samFILE == 0)
      { /*If: the file could not be opened*/
         fprintf(
            stderr,
            "-sam %s could not be opened\n",
            samStr
          );

          goto err_main_sec06_sub02;
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open output file
   \*****************************************************/

   if(outStr == 0 || outStr[0] == '-') outFILE = stdout;
 
   else
   { /*Else: the user supplied a sam file*/
      outFILE =
         fopen(
            (char *) outStr,
            "w"
         );

      if(outFILE == 0)
      { /*If: the file could not be opened*/
         fprintf(
            stderr,
            "-out %s could not be opened\n",
            outStr
          );

          goto err_main_sec06_sub02;
      } /*If: the file could not be opened*/
   } /*Else: the user supplied a paf file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - set up (read in coordinates/memory allocation)
   ^   o main sec03 sub01:
   ^     - get gene coordinates
   ^   o main sec03 sub02:
   ^     - allocate memory/set up samEntry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get gene coordinates
   \*****************************************************/

   genesHeapST =
      getCoords_geneCoord(
         geneTblFileStr,
         &numGenesSI,
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

      goto err_main_sec06_sub02;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - allocate memory/set up samEntry
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: had memory error*/
      fprintf(
          stderr,
          "memory error\n"
      );
          
      goto err_main_sec06_sub02;
   } /*If: had memory error*/
 
   offTargSI = genesHeapST->endAryUI[numGenesSI] + 2;
     /*These are genes that did not map to the end of the
     ` gene range, but still mapped to the reference
     ` The + 2 is to account for index 1 and to ensure
     `   thre array ends in 0
     */

   readMapHeapArySI =
      calloc(
         offTargSI,
         sizeof(uint)
      );

   if(readMapHeapArySI == 0)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "Memory error (main sec05)\n"
      );

      goto err_main_sec06_sub02;
   } /*If: I had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^  - build histogram from reads
   ^  o main sec04 sub01:
   ^    - skip headers, alternative alignments, & unmapped
   ^  o main sec04 sub02:
   ^    - trim reads, add to histogram, & get next read
   ^  o main sec04 sub03:
   ^    - check for errors and clean up uneeded variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - skip headers, alternative alignments, & unmapped
   \*****************************************************/

   errSC =
      get_samEntry(
          &samStackST,
          &samBuffHeapStr,
          &lenSamBuffUL,
          samFILE
      ); /*get first line of sam file (likely header)*/

   offTargSI = 0;

   while(! errSC)
   { /*Loop: read in the samfile*/
      /*check if this is a header line*/
      if(samStackST.extraStr[0] == '@')
         goto nextEntry_main_sec06_sub03;

      if(samStackST.flagUS & 4)
      { /*If: the read was unmapped*/
         ++noMapSI;
         goto nextEntry_main_sec06_sub03;
      } /*If: the read was unmapped*/

      if(samStackST.flagUS & (256 | 2048))
          goto nextEntry_main_sec06_sub03;

      /**************************************************\
      * Main Sec04 Sub02:
      *   - trim reads, add to histogram, & get next read
      \**************************************************/

      trimSeq_trimSam(&samStackST);
         /*trim off masked bases*/

      addRead_ampDepth(
         &samStackST,
         genesHeapST,
         numGenesSI,
         readMapHeapArySI,
         &offTargSI
      ); /*Add in the coverd bases to the histogram*/

      nextEntry_main_sec06_sub03:;

      errSC =
         get_samEntry(
             &samStackST,
             &samBuffHeapStr,
             &lenSamBuffUL,
             samFILE
         );
   } /*Loop: read in the samfile*/

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - check for errors and clean up uneeded variables
   \*****************************************************/

   if(errSC & 64)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "Memory error while reading sam file\n"
      );

      goto err_main_sec06_sub02;
   } /*If: I had a memory error*/

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   free(samBuffHeapStr);
   samBuffHeapStr = 0;
   lenSamBuffUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
	^  - print histogram and finish cleaning up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   phead_ampDepth(outFILE);

   phist_ampDepth(
      (sint *) readMapHeapArySI,
      minDepthSI,
      genesHeapST,
      numGenesSI,
      offTargSI,
      noMapSI,
      extraColStr,
      outFILE
   ); /*Print out the histogram*/

   if(outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   freeHeap_geneCoord(genesHeapST); /*Finished with this*/
   genesHeapST = 0;

   if(readMapHeapArySI)
      free(readMapHeapArySI);      /*Finished with this*/

   readMapHeapArySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - clean up
   ^   o main sec06 sub01:
   ^     - no error clean up
   ^   o main sec06 sub02:
   ^     - error clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec06_sub03;

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - error clean up
   \*****************************************************/

   err_main_sec06_sub02:;
   errSC = -1;
   goto cleanUp_main_sec06_sub03;

   /*****************************************************\
   * Main Sec06 Sub03:
   *   - general clean up (no error/error)
   \*****************************************************/

   cleanUp_main_sec06_sub03:;

   if(samBuffHeapStr)
      free(samBuffHeapStr);

   samBuffHeapStr = 0;
   lenSamBuffUL = 0;

   freeStack_samEntry(&samStackST);

   freeHeap_geneCoord(genesHeapST);
   genesHeapST = 0;

   if(readMapHeapArySI)
      free(readMapHeapArySI);

   readMapHeapArySI = 0;

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   outFILE = 0;

   return(errSC);
} /*main*/


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
