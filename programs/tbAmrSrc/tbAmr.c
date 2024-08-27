/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o fun01: pversion_tbAmr
'     - prints version number for tbAMR
'   o fun02: phelp_tbAmr
'     - prints help message for tbAMR
'   o fun03: input_tbAmr
'     - gets user input
'   o main:
'     - Driver function for tbAmr
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/charCp.h"
#include "../genLib/base10str.h"

#include "checkAmr.h"
#include "amrST.h"

/*Only .h file*/
#include "../genLib/dataTypeShortHand.h"
#include "tbAmrDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/samEntry.h"
!   o .c  #include "../genLib/codonTbl.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "drugAry.h"
!   o .h  #include "../genLib/ntTo2Bit.h"
!   o .h  #include "../genLib/revNtTo2Bit.h"
!   o .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_tbAmr
|   - prints version number for tbAMR
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o version for tbAMR to outFILE
\-------------------------------------------------------*/
void pversion_tbAmr(
   void *outFILE
){
   fprintf(
       (FILE *) outFILE,
       "tbAmr Version: %i.%02i.%02i\n",
       def_year_tbAmrDefs,
       def_month_tbAmrDefs,
       def_day_tbAmrDefs
   ); /*Version of tbCon*/
} /*pversion_tbAmr*/

/*-------------------------------------------------------\
| Fun02: phelp_tbAmr
|   - prints help message for tbAMR
| Input:
|   - outFILE:
|     o File to print the help message to
| Output:
|   - Prints:
|     o help message for tbAMR to outFILE
\-------------------------------------------------------*/
void phelp_tbAmr(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for tbAMR
   '   o fun02 sec01:
   '     - print usage/description
   '   o fun02 sec02:
   '     - print user input
   '   o fun02 sec03:
   '     - print output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^  - print usage/description
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
     (FILE *) outFILE,
     "tbAMR -sam-con consenus.sam -amr-tbl database.tsv"
   );

   fprintf(
      (FILE *)
      outFILE, "\nor\n"
   );

   fprintf(
      (FILE *) outFILE,
      "tbAMR -sam reads.sam -amr-tbl database.tsv\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Detects Anti-microbal (antibiotic) resistant"
   );
        
   fprintf(
      (FILE *) outFILE,
      " genes\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    (AMRs) in input consensuses or reads\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^  - print user input
   ^   o fun02 sec02 sub01:
   ^     - print input line
   ^   o fun02 sec02 sub02:
   ^     - database entry
   ^   o fun02 sec02 sub03:
   ^     - sequence input
   ^   o fun02 sec02 sub04:
   ^     - output options
   ^   o fun02 sec02 sub05:
   ^     - help message and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - print input line
   \*****************************************************/

    fprintf(
       (FILE *) outFILE,
       "Input:\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - database entry
   \*****************************************************/

    fprintf(
       (FILE *) outFILE,
       "  -amr-tbl database.tsv: [Requried]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o AMR database in tbAmr format\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Use whoToTbAmr to build this\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - sequence input
   *   o fun02 sec02 sub03 cat01:
   *     - read input
   *   o fun02 sec02 sub03 cat02:
   *     - consnesus input
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - read input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -sam reads.sam: [Required; stdin]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Sam file with reads mapped to the same\n"
    );

    fprintf(
      (FILE *) outFILE,
      "       reference (NC000962.3) as AMR database\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Use \"-sam -\" for stdin\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - consnesus input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -sam-con consensus.sam: [Replaces -sam]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Sam file with consensuses mapped to same\n"
    );


    fprintf(
      (FILE *) outFILE,
      "      reference (NC000962.3) as AMR database\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Use \"-sam-con -\" for stdin\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - output options
   *   o fun02 sec02 sub04 cat01:
   *     - general output
   *   o fun02 sec02 sub04 cat02:
   *     - read id output
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat01:
   +   - General output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -out output.tsv: [Optional; stdout]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o File to output results to\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o Use \"-out -\" for stdout\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat02:
   +   - Read id output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -id-file ids.tsv: [Optional; Not used]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    o File to print the read ids that supported"
    );
    fprintf(
       (FILE *) outFILE,
       "\n      a variant to (read_id\\tvariant_id)\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - settings
   *   o fun02 sec02 sub04 cat01:
   *     - checking frameshift paramater
   *   o fun02 sec02 sub04 cat02:
   *     - min depth paramater
   *   o fun02 sec02 sub04 cat03:
   *     - min % reads mapping parameter
   *   o fun02 sec02 sub04 cat04:
   *     - min % of total reads parameter
   \*****************************************************/

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub04 Cat01:
    +   - checking frameshift paramater
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    if(def_checkFramshift_tbAmrDefs)
       fprintf(
          (FILE *) outFILE,
          "  -frameshift: [Optional; Yes]\n"
       );

   else
       fprintf(
          (FILE *) outFILE,
          "  -frameshift: [Optional; No]\n"
       );

   fprintf(
      (FILE *) outFILE,
      "    o Check frames shifts\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Without this tbAmr treats frames shifts\n"
   );

   fprintf(
      outFILE,
      "      an extact match\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Turn off with: -no-frameshift\n"
   );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub04 Cat02:
    +   - min depth paramater
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -min-depth %i: [Optional; %i]\n",
       def_minDepth_tbAmrDefs,
       def_minDepth_tbAmrDefs
    );

    fprintf(
       (FILE *) outFILE,
       "    o mininum read depth to keep AMR (-sam only)"
    );

    fprintf(
       (FILE *) outFILE,
       "\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub04 Cat03:
    +   - Min % reads mapping parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -min-amr-map-perc %.2f: [Optional; %.2f]\n",
       def_minPercMapped_tbAmrDefs,
       def_minPercMapped_tbAmrDefs
   );

    fprintf(
       (FILE *) outFILE,
       "    o Reads only; mininmum percent of mapped\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      reads that supported the AMR needed to\n"
    );

    fprintf(
       (FILE *) outFILE,
       "        keep an AMR\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub04 Cat04:
    +   - Min % of total reads parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "  -min-total-map-perc %.2f: [Optional; %.2f]\n",
       def_minPercTotalReads_tbAmrDefs,
       def_minPercTotalReads_tbAmrDefs
   );

    fprintf(
       (FILE *) outFILE,
       "    o Minimum percent of total reads\n"
    );
    fprintf(
       (FILE *) outFILE,
       "      (mapped/unmapped) needed to keep an AMR\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help message and version number
   \*****************************************************/

    fprintf(
       (FILE *) outFILE,
       "  -h: print this help message and exit\n"
   );

    fprintf(
       (FILE *) outFILE,
       "  -v: print version number and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^  - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
       "  - Prints detected AMRs to -out\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints read ids with AMRs to -id-file\n"
   );
} /*phelp_tbAmr*/


/*-------------------------------------------------------\
| Fun03: input_tbAmr
|   - gets user input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with arguments user input
|   - amrTblStr:
|     o will hold the path to the AMR table to check for
|       antibiotic resitance with
|   - samFileStr:
|     o will hold the path to the gehoeme to check for
|       antibiotic resitiance
|   - outFileStr:
|     o will hold the path to the file to output to
|   - idFileStr:
|     o will point to file to print read ids to
|   - readsBl:
|     o set to 1 if user input reads (as sam file)
|   - framshiftBl:
|     o set to 1 (check frameshits) or 0 (do not check)
|   - minDepthUI:
|     o min read depth the user input
|   - minPerMapF:
|     o set to the min percent of mapped reads to keep an
|       AMR the user wants
|   - minPercTotalF:
|     o set to the min percent of reads total reads to
|       keep an AMR the user wants
| Output:
|   - Modifies:
|     o all input variables
|   - Returns:
|     o 0 for success
|     o 1 if printed help message or version number
|     o 2 for errors
\-------------------------------------------------------*/
signed char
input_tbAmr(
   int numArgsSI,
   char *argAryStr[],
   signed char **amrTblStr,  /*table with AMRs*/
   signed char **samFileStr,  /*sam file to check*/
   signed char **outFileStr,  /*file to output to*/
   signed char **idFileStr,   /*prefix for read id files*/
   signed char *readsBl,      /*1: user input reads*/
   signed char *frameshiftBl, /*1: checking frameshifts*/
   uint *minDepthUI,   /*min read depth to keep an amr*/
   float *minPercMapF, /*min % support to keep amr; read*/
   float *minPercTotalF/*min % mapped reads to keep*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - set error type and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar errSC = 0;
   sint siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_tbAmr(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - file io plus start loop
   ^   o fun03 sec03 sub02:
   ^     - output file io
   ^   o fun03 sec03 sub03:
   ^     - read filter settings
   ^   o fun03 sec03 sub04:
   ^     - frameshift settings
   ^   o fun03 sec03 sub05:
   ^     - help message checks
   ^   o fun03 sec03 sub06:
   ^     - version number checks
   ^   o fun03 sec03 sub07:
   ^     - invalid input
   ^   o fun03 sec03 sub08:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - input file io plust start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get input*/
      if(
         ! eql_charCp(
            (schar *) "-amr-tbl",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: amr table*/
         ++siArg;
         *amrTblStr = (schar *) argAryStr[siArg];
      } /*If: amr table*/

      else if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: sam file with reads*/
         ++siArg;
         *samFileStr = (schar *) argAryStr[siArg];
         *readsBl = 1;
      } /*Else if: sam file with reads*/

      else if(
         ! eql_charCp(
            (schar *) "-sam-con",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: sam file with consensuses*/
         ++siArg;
         *samFileStr = (schar *) argAryStr[siArg];
         *readsBl = 0;
      } /*Else if: sam file with consensuses*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - output file io
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: output file*/
         ++siArg;
         *outFileStr = (schar *) argAryStr[siArg];
      } /*Else if: output file*/

      else if(
         ! eql_charCp(
            (schar *) "-id-file",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      )
      { /*Else if: output id file*/
         ++siArg;
         *idFileStr = (schar *) argAryStr[siArg];
      } /*Else if: output id file*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - read filter settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: minimum read depth for AMR*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
           strToUI_base10str(
              (schar *) argAryStr[siArg],
              minDepthUI
           );

         if(*tmpStr != '\0')
         { /*If: non-numeric or to large*/
             fprintf(
                stderr,
                "-min-depth %s; non-numeric/to large\n",
                argAryStr[siArg]
             );

            goto err_fun03_sec04;
         } /*If: non-numeric or to large*/
      } /*Else if: minimum read depth for AMR*/

      else if(
         ! eql_charCp(
            (schar *) "-min-amr-map-perc",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: minimum map percent input*/
         ++siArg;
         *minPercMapF= atof(argAryStr[siArg]);
      } /*Else if: minimum map percent input*/

      else if(
         ! eql_charCp(
            (schar *) "-min-total-map-perc",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: total map percent input*/
         ++siArg;
         *minPercTotalF= atof(argAryStr[siArg]);
      } /*Else if: total map percent input*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - frameshift settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-frameshift",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *frameshiftBl = 1;

      else if( 
         ! eql_charCp(
            (schar *) "-no-frameshift",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *frameshiftBl = 0;

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - help message checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: help message*/
         phelp_tbAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else if: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: help message*/
         phelp_tbAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else if: help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: help message*/
         phelp_tbAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else if: help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: help message*/
         phelp_tbAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else if: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: help message*/
         phelp_tbAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else if: help message*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - version number checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: version number*/
         pversion_tbAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else if: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: version number*/
         pversion_tbAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else if: version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: version number*/
         pversion_tbAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else if: version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: version number*/
         pversion_tbAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else if: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else if: version number*/
         pversion_tbAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else if: version number*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
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
      * Fun03 Sec03 Sub08:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - set error type and return
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
} /*input_tbAmr*/

/*-------------------------------------------------------\
| Main:
|   - detectes AMRs in reads/consensuses (driver fuction)
| Input:
| Output:
|   - Prints:
|     o detected AMRs in user requested output
|     o read ids for reads with AMRs (user requested)
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main Sec02:
   '     - get and check user input (open files)
   '   o main Sec03:
   '     - open amr table file and get AMRs
   '   o main Sec04:
   '     - check for resistance mutations
   '   o main Sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^  - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*user input variables*/
   schar *amrTblStr = 0;  /*table with AMRs*/
   schar *samFileStr = 0; /*sequences to check for AMRs*/
   schar *outFileStr = 0; /*holds path to output file*/
   schar *idFileStr = 0;  /*name of file to print ids to*/

   schar readsBl;     /*tells if read/consensus sam file*/

   /*read filtering settings*/
   uint minDepthUI = def_minDepth_tbAmrDefs;
   float minPercMapF = def_minPercMapped_tbAmrDefs;
   float minPercTotalF = def_minPercTotalReads_tbAmrDefs;
   schar frameshiftBl = def_checkFramshift_tbAmrDefs;

   schar *drugHeapAryStr = 0;/*holds antibiotic names*/
   sint numDrugsSI = 0;  /*holds number drugs in drugAry*/
   sint maxDrugsSI = 0;  /*max drugs drugAry can have*/
   
   schar errSC = 0;

   uint numAmrUI = 0;

   FILE *checkFILE = 0;

   struct amrST *amrHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^  - get and check user input (open files)
   ^  o main sec02 sub01:
   ^    - get user input
   ^  o main sec02 sub02:
   ^    - sam file; check if can open
   ^  o main sec02 sub03:
   ^    - output file; check if can open
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /****************************************************\
   * Main Sec02 Sub01:
   *  - Get user input
   \****************************************************/

   errSC =
     input_tbAmr(
        numArgsSI,
        argAryStr,
        &amrTblStr,    /*table with AMR mutations*/
        &samFileStr,   /*sequence to check for resitance*/
        &outFileStr,   /*file to outuput to*/
        &idFileStr,    /*prefix for read id files*/
        &readsBl,      /*1: user input reads*/
        &frameshiftBl, /*1: user wants frameshift checks*/
        &minDepthUI,   /*min read depth to keep an amr*/
        &minPercMapF,  /*min % support to keep amr; read*/
        &minPercTotalF /*min % mapped reads to keep*/
     );

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version to 0 (no error)*/
      goto cleanUp_main_sec05_sub0x;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - sam file; check if can open
   \*****************************************************/

   if(
         samFileStr
      && samFileStr[0] != '-'
   ){ /*If: sam file was input*/
      checkFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! checkFILE)
      { /*If: could not open file*/
        fprintf(
           stderr,
           "Could not open -sam %s\n",
           samFileStr
        );

        goto fileErr_main_sec05_sub03;
      } /*If: could not open file*/

      fclose(checkFILE); 
      checkFILE = 0;
   } /*If: sam file was input*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - output file; check if can open
   \*****************************************************/

   if(
          outFileStr
       && outFileStr[0] != '-'
   ){ /*If: output file was input*/
      checkFILE =
         fopen(
            (char *) outFileStr,
            "r"
         );

      if(! checkFILE)
      { /*If: could not open the file*/
        fprintf(
           stderr,
           "Could not open -out %s\n",
           outFileStr
        );

        goto fileErr_main_sec05_sub03;
      } /*If: could not open the file*/

      fclose(checkFILE); 
      checkFILE = 0;
   } /*If: output file was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^  - open amr table file and get AMRs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   checkFILE =
      fopen(
         (char *) amrTblStr,
         "r"
      );

   if(checkFILE == 0)
   { /*If: could not open AMR database*/
      fprintf(
         stderr,
         "Could not open -amr-tbl %s\n",
         amrTblStr
      );

      goto fileErr_main_sec05_sub03;
   } /*If: could not open AMR database*/

   fclose(checkFILE);
   checkFILE = 0;

   amrHeapST =
      readTbl_amrST(
         amrTblStr, 
         &numAmrUI,
         &drugHeapAryStr,
         &numDrugsSI,
         &maxDrugsSI,
         &errSC
      ); /*get amr entries*/

   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_amrST)
      { /*If: had memory error*/
         fprintf(
           stderr,
           "memory error; reading -amr-tbl %s\n",
           amrTblStr
         );

         goto memErr_main_sec05_sub02;
      } /*If: had memory error*/

      fprintf(
        stderr,
        "-amr-tbl %s; not tbAmr format or no data\n",
        amrTblStr
      );

      goto fileErr_main_sec05_sub03;
   } /*If: error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^  - check for resistance mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      samFindAmrs_checkAmr(
         amrHeapST,
         (sint) numAmrUI,
         drugHeapAryStr,
         readsBl,      /*reads or consesnsuses*/
         frameshiftBl, /*process frameshifts*/
         minDepthUI,   /*minimum read depth*/
         minPercMapF,  /*min % support to keep amr*/
         minPercTotalF,/*min % mapped reads to keep*/
         samFileStr,
         outFileStr,
         idFileStr /*prefix for variant read id files*/
      ); /*check for amrs*/

   if(errSC)
   { /*If: had error*/
     if(errSC == def_memErr_amrST)
     { /*If: had file error*/
        fprintf(
           stdout,
           "memory error when checking AMRs\n"
        );

        goto memErr_main_sec05_sub02;
     } /*If: had file error*/

     fprintf(
        stdout,
        "file error -out, -id-file, -sam, or -sam-con\n"
     );


     goto fileErr_main_sec05_sub03;
   } /*If: had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   ^   o main sec05 sub01:
   ^     - no error clean up
   ^   o main sec05 sub02:
   ^     - memory error clean up
   ^   o main sec05 sub03:
   ^     - file error clean up
   ^   o main sec05 sub04:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec05_sub0x;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec05_sub02:;
   errSC = 1;
   goto cleanUp_main_sec05_sub0x;

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - file error clean up
   \*****************************************************/
      
   fileErr_main_sec05_sub03:;
   errSC = 2;
   goto cleanUp_main_sec05_sub0x;

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_main_sec05_sub0x:;

   if(amrHeapST)
      freeHeapAry_amrST(
         amrHeapST,
         numAmrUI
      );

   amrHeapST = 0;
   numAmrUI = 0;

   if(drugHeapAryStr)
      free(drugHeapAryStr);

   drugHeapAryStr = 0;

   if(
         checkFILE
      && checkFILE != stdin
      && checkFILE != stdout
   ) fclose(checkFILE);

   checkFILE = 0;

   return (sint) errSC;
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
