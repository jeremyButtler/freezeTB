/*########################################################
# Name: adjCoords
#   - Adjust coordinates in an sam file
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and version number
'   o fun01: adjCoords_version
'     - prints the version number for adjCoords
'   o fun02: adjCoords_help
'     - prints the help message for adjCoords
'   o main:
'     - driver function to adjust coordinates with
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and version number
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - defined variables (defaults)
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../generalLib/geneCoordStruct.h"
#include "../generalLib/samEntryStruct.h"
#include "adjCoordsFun.h"

/*These have no .c files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden includes:
!   - .h #include "numToStr.h"
!   - .h #include "base10StrToNum.h"
!   - .h #include "genMath.h"
!   - .h #include "ntToBit.h"
!     o this is only used in printing sam entry stats,
!       so it is not used
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - defined variables (defaults)
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_year_adjCoords 2024
#define def_month_adjCoords 5
#define def_day_adjCoords 14

schar *def_refStr = (schar *) "NC000962.3";

/*-------------------------------------------------------\
| Fun01: adjCoords_version
|   - prints the version number for adjCoords
| Input:
|   - outFILE:
|     o file to print the version number to
| Output:
|   - prints the version number to outFILE
\-------------------------------------------------------*/
void
adjCoords_version(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "adjCoords version: %i-%02i-%02i\n",
      def_year_adjCoords,
      def_month_adjCoords,
      def_day_adjCoords
    );
} /*adjCoords_version*/

/*-------------------------------------------------------\
| Fun02: adjCoords_help
|   - prints the help message for adjCoords
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - prints the help message to outFILE
\-------------------------------------------------------*/
void
adjCoords_help(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for adjCoords
   '   o fun02 sec01:
   '     - print the usage entry
   '   o fun02 sec02:
   '     - print the input entry
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print the usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "adjCoords -coords gene-coords.tsv -sam file.sam\n"
    );

   fprintf(
      outFILE,
      "  - adjust coordinates in sam file to reference\n"
    );

   fprintf(
      outFILE,
      "  - entries with genes not in gene-coords.tsv are"
    );

   fprintf(
      outFILE,
      " removed\n"
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print the input entry
   ^   o fun02 sec02 sub01:
   ^      - print input block header
   ^   o fun02 sec02 sub02:
   ^      - genome coordinates block
   ^   o fun02 sec02 sub03:
   ^      - sam file block
   ^   o fun02 sec02 sub04:
   ^      - reference block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - print input block header
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - genome coordinates block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -coords gene-coordinates.tsv: [Required]\n"
    );

   fprintf(
      outFILE,
      "   o tsv file with coordiantes of each gene\n"
    );

    fprintf(
      outFILE,
      "      - gene_name\\tref_name_ignored\\tdirection"
    );

    fprintf(
      outFILE,
      "\\tref_start\\tref_end\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - sam file block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -sam file.sam: [stdin]\n"
    );

   fprintf(
      outFILE,
      "   o sam file to adjust coordinates on\n"
    );

   fprintf(
      outFILE,
      "   o use `-sam -` for stdin input \n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - reference block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -ref name: [%s]\n",
      def_refStr
    );

   fprintf(
      outFILE,
      "   o name of new mapped reference for each gene\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help and version block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: print this help message\n"
    );

   fprintf(
      outFILE,
      "  -v: print the versio number\n"
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:\n"
    );

   fprintf(
      outFILE,
      "  - prints adjusted sam file to stdout\n"
    );
} /*adjCoords_help*/

/*-------------------------------------------------------\
| Fun03: adjCoords_getInput
|   - gets user input in argAryStr
| Input:
|   - argAryStr:
|     o array of c-strings with user input
|   - numArgsSI:
|     o number of c-strings in argAryStr
|   - tblFileStr:
|     o pointer to c-string to point to the gene
|       coordinates file
|   - samFileStr:
|     o pointer to c-string to point to the sam file
|   - refStr:
|     o pointer to c-string to point to the reference name
| Output:
|   - Modifies:
|     o tblFileStr to point to the coordinates file in
|       argAryStr
|     o samFileStr to point to the sam file in argAryStr
|     o refStr to point to the reference name in argAryStr
|   - Prints;
|     o help message or version to stdou if requested
|     o error messages to stderr
|   - Returns;
|     o 0 for no errors
|     o 1 for help message or version request
|     o 2 for errors
\-------------------------------------------------------*/
schar
adjCoords_getInput(
   char *argAryStr[],
   int numArgsSI,
   signed char **tblFileStr,
   signed char **samFileStr,
   signed char **refStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if I have input and start loop
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - return errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if I have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(siArg >= numArgsSI)
   { /*If: no arguments were input*/
      adjCoords_help(stderr);

      fprintf(
         stderr,
         "No arguments input\n"
      );

      errSC = 1;
   } /*If: no arguments were input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - start loop & check if input is coordinate file
   ^   o fun03 sec03 sub02:
   ^     - is sam file input
   ^   o fun03 sec03 sub03:
   ^     - is reference input
   ^   o fun03 sec03 sub04:
   ^     - is an help messge request
   ^   o fun03 sec03 sub05:
   ^     - is an version number request
   ^   o fun03 sec03 sub06:
   ^     - unkown input
   ^   o fun03 sec03 sub07:
   ^     - move to next input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - start loop and check if input is coordinate file
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: process the user input*/
      if(!
          cStrEql(
             "-coords",
             argAryStr[siArg],
             '\0'
          )
      ){ /*If: the user provided coordinates*/
         ++siArg;
         *tblFileStr = (schar *) argAryStr[siArg];
      } /*If: the user provided coordinates*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - is sam file input
      \**************************************************/

      else if(!
          cStrEql(
             "-sam",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: the user provided an sam file*/
         ++siArg;
         *samFileStr = (schar *) argAryStr[siArg];
      } /*Else If: the user provided an sam file*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - is reference input
      \**************************************************/

      else if(!
          cStrEql(
             "-ref",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: the user provided an reference name*/
         ++siArg;
         *refStr = (schar *) argAryStr[siArg];
      } /*Else If: the user provided an reference name*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - is an help messge request
      \**************************************************/
      
      else if(!
          cStrEql(
             "-h",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested help message*/
         adjCoords_help(stdout);
         return 1;
      } /*Else If: requested help message*/

      else if(!
          cStrEql(
             "--h",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested help message*/
         adjCoords_help(stdout);
         return 1;
      } /*Else If: requested help message*/

      else if(!
          cStrEql(
             "help",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested help message*/
         adjCoords_help(stdout);
         return 1;
      } /*Else If: requested help message*/

      else if(!
          cStrEql(
             "-help",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested help message*/
         adjCoords_help(stdout);
         return 1;
      } /*Else If: requested help message*/

      else if(!
          cStrEql(
             "--help",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested help message*/
         adjCoords_help(stdout);
         return 1;
      } /*Else If: requested help message*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - is an version number request
      \**************************************************/

      else if(!
          cStrEql(
             "-v",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested version number*/
         adjCoords_version(stdout);
         return 1;
      } /*Else If: requested version number*/

      else if(!
          cStrEql(
             "--v",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested version number*/
         adjCoords_version(stdout);
         return 1;
      } /*Else If: requested version number*/

      else if(!
          cStrEql(
             "version",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested version number*/
         adjCoords_version(stdout);
         return 1;
      } /*Else If: requested version number*/

      else if(!
          cStrEql(
             "-version",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested version number*/
         adjCoords_version(stdout);
         return 1;
      } /*Else If: requested version number*/

      else if(!
          cStrEql(
             "--version",
             argAryStr[siArg],
             '\0'
          )
      ){ /*Else If: requested version number*/
         adjCoords_version(stdout);
         return 1;
      } /*Else If: requested version number*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - unkown input
      \**************************************************/

      else
      { /*Else: no idea what is*/
         if(! (errSC & 2) )
            fprintf(
               stderr,
               "%s is not recongnized\n",
               argAryStr[siArg]
            ); /*No errors reported yet*/

         errSC |= 2;
      } /*Else: no idea what is*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - move to next input
      \**************************************************/

      ++siArg;
   } /*Loop: process the user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return errSC;
} /*adjCoords_getInput*/

/*-------------------------------------------------------\
| Main:
|   - driver function to adjust coordinates with
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o pointer to array of c-strings with user input
| Output:
|   - Prints:
|     o ajdust sam file to stdout
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize variables and get input
   '   o main sec03:
   '     - open sam file and read in genome coodinates
   '   o main sec04:
   '     - adjust genome coordinates
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tblFileStr = 0;
   schar *samFileStr = 0;
   schar *refStr = def_refStr;

   ulong errUL = 0;

   struct geneCoords *coordsHeapST = 0;
   sint numGenesSI = 0;

   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   schar onReadsBl = 0; /*For printing the header*/

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize variables and get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   initSamEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/


   errUL =
      (ulong)
      adjCoords_getInput(
         argAryStr,
         numArgsSI,
         &tblFileStr,
         &samFileStr,
         &refStr
      );

   if(errUL)
   { /*If: I had an error*/
      --errUL; /*remove help message and vesion requests*/
      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - open sam file and read in genome coodinates
   ^   o main sec03 sub01:
   ^     - open the output file
   ^   o main sec03 sub02:
   ^     - open the sam file
   ^   o main sec03 sub03:
   ^     - read in the gene coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - open the output file
   \*****************************************************/

   outFILE = stdout;

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - open the sam file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
      samFILE = stdin;
   else
   { /*Else: I need to open the sam file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
            stderr,
            "Could not open -sam %s\n",
            samFileStr
         );

         goto cleanUp_main_sec05;
      } /*If: I could not open the sam file*/
   } /*Else: I need to open the sam file*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - read in the gene coordinates
   \*****************************************************/

   coordsHeapST =
      getGeneCoords(
         (char *) tblFileStr,
         &numGenesSI,
         &errUL
      );

   if(errUL)
   { /*If: I had an error*/
      if(errUL & def_fileErr_geneCoord)
         fprintf(
            stderr,
            "could not open -coords %s\n",
            tblFileStr
         );

      if(errUL & def_memErr_geneCoord)
         fprintf(
            stderr,
            "mem error reading -coords %s\n",
            tblFileStr
         );

      else
         fprintf(
            stderr,
            "line %lu in -coords %s is not valid\n",
            errUL >> 8,
            tblFileStr

          );

      errUL = (ulong) -1;
      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   fprintf(
      outFILE,
      "@SQ\tSN:%s\tLN:%u\n",
      refStr,
      coordsHeapST->endAryUI[numGenesSI - 1]
    ); /*print out the sequence and length header*/

   geneCoords_sortNames(
      coordsHeapST,
      0,
      numGenesSI /*Is index 0*/
   ); /*sort genes by name instead of start*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - adjust genome coordinates
   ^   o main sec04 sub01:
   ^     - read first sam file line and start loop
   ^   o main sec04 sub02:
   ^     - handle comment entries
   ^   o main sec04 sub03:
   ^     - print out the program header
   ^   o main sec04 sub04:
   ^     - adjust coordinates
   ^   o main sec04 sub05:
   ^     - print out the entry
   ^   o main sec04 sub06:
   ^     - move to next entry
   ^   o main sec04 sub07:
   ^     - check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - read first sam file line and start loop
   \*****************************************************/

   errUL =
      readSamLine(
         &samStackST,
         (char **) &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errUL)
   { /*Loop: convert coordinates in the sam file*/

     /***************************************************\
     * Main Sec04 Sub02:
     *   - handle comment entries
     \***************************************************/

     if(
           samStackST.lenExtraUI > 0
        && samStackST.extraStr[0] == '@'
     ){ /*If: have an comment*/
        if(! cStrEql("@SQ\t", samStackST.extraStr, '\t'))
           goto nextEntry_main_sec04_sub06;
           /*this is an sequence header that I need to
           `  discard
           */

        goto printEntry_main_sec04_sub05;
     } /*If: have an comment*/

     /***************************************************\
     * Main Sec04 Sub03:
     *   - print out the program header
     \***************************************************/

     if(! onReadsBl)
     { /*If: I need to print the programs header*/
        fprintf(
           outFILE,
           "@PG\tID:adjCoords\tPN:adjCoords\tVN:%i-%02i",
           def_year_adjCoords,
           def_month_adjCoords
        );

        fprintf(
           outFILE,
           "-%02i\tCL:adjCoords -ref %s -coords %s -sam",
           def_day_adjCoords,
           refStr,
           tblFileStr
        );

        if(samFileStr)
        { /*If: An sam file was input*/
           fprintf(
              outFILE,
              " %s\n",
              samFileStr
           );
        } /*If: An sam file was input*/

        else
        { /*Else: sam file read in by stdin*/
           fprintf(
              outFILE,
              " -\n"
           );
        } /*Else: sam file read in by stdin*/

        onReadsBl = 1;
     } /*If: I need to print the programs header*/

     /***************************************************\
     * Main Sec04 Sub04:
     *   - adjust coordinates
     \***************************************************/

     errUL =
        (ulong)
        adjCoordsFun_adjCoords(
           &samStackST,  /*sam entry to adjust*/
           coordsHeapST, /*has genes names & coordinates*/
           numGenesSI,   /*number genes in coordsHeapST*/
           refStr        /*name of new reference*/
        ); /*ajust the coordinates to the new reference*/

     /*check if I had coordinates for the gene*/
     if(errUL)
        goto nextEntry_main_sec04_sub06;

     /***************************************************\
     * Main Sec04 Sub05:
     *   - print the entry
     \***************************************************/

     printEntry_main_sec04_sub05:;

     pSamEntry(
        &samStackST,
        (char **) &buffHeapStr,
        &lenBuffUL,
        outFILE
     );
           
     /***************************************************\
     * Main Sec04 Sub06:
     *   - move to next entry
     \***************************************************/

     nextEntry_main_sec04_sub06:;

     errUL =
        readSamLine(
           &samStackST,
           (char **) &buffHeapStr,
           &lenBuffUL,
           samFILE
        );
   } /*Loop: convert coordinates in the sam file*/

   /*****************************************************\
   * Main Sec04 Sub07:
   *   - check for errors
   \*****************************************************/

   --errUL; /*convert from EOF to 0*/

   if(errUL)
   { /*If: I had an error*/
      if(samFileStr)
         fprintf(
            stderr,
            "memory error reading -sam %s\n",
            samFileStr
          );
      else
         fprintf(
            stderr,
            "memory error reading -sam -\n"
          );

      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp_main_sec05:;

   free(buffHeapStr);
   buffHeapStr = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   freeGeneCoords(coordsHeapST);
   coordsHeapST = 0;

   freeSamEntryStack(&samStackST);

   exit((int) errUL);
} /*main*/
