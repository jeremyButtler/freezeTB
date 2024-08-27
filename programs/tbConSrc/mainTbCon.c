/*#######################################################\
# Name: mainTbCon
#   - builds a consensus from a sam file aligned to a
#     singed reference
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - indcluded libraries
'   o fun01: pversion_mainTbCon
'     - prints out version number for tbCon
'   o fun02: phelp_mainTbCon
'     - prints out the tbCon help message
'   o fun03: input_mainTbCon
'     - gets user input
'   o main:
'     - driver function to build the consensus with
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - indcluded libraries
\-------------------------------------------------------*/

#ifdef PLAN9
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

#include "tbCon.h"

/*only using .h file (no .c or not using .c)*/
#include "../genLib/dataTypeShortHand.h"
#include "tbConDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainTbCon
|   - prints out version number for tbCon
| Input:
|   - outFILE:
|     - file to print versipo number to
| Output:
|   - Prints:
|     o version number of tbCon to outFILE
\-------------------------------------------------------*/
void
pversion_mainTbCon(
   void *outFILE
){
   fprintf(
       (FILE *) outFILE,
       "tbCon Version: %i-%02i-%02i\n",
       def_year_tbConDefs,
       def_month_tbConDefs,
       def_day_tbConDefs
   ); /*Version of tbCon*/
} /*pversion_mainTbCon*/

/*-------------------------------------------------------\
| Fun02: phelp_mainTbCon
|   - prints out the tbCon help message
| Input:
|   - pFullC:
|     o 1: print out the full help message (TODO: add in)
|     o 0: print out the short help message
|   - outVoidFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o tbCon help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainTbCon(
   char pFullC,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - print the help message for tbCon
   '   o fun02 sec01:
   '     - print out the usage and description entry
   '   o fun02 sec02:
   '     - print out the input paramaters
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print out the usage and description entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Usage: tbCon -sam file.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Makes an consensus genome from input sam file"
    );

    fprintf((FILE *) outFILE, "\n");

    fprintf((FILE *) outFILE, "Input:\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print out the input paramaters
   ^   o fun02 sec02 sub02:
   ^     - print out user input/output options
   ^   o fun02 sec02 sub03:
   ^     - print cosnesus settings
   ^   o fun02 sec02 sub04:
   ^     - print variant file settings
   ^   o fun02 sec02 sub05:
   ^     - help message and version numbers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun02 Sec02 Sub02:
    *   - print out user input/output options
    *   o fun02 sec02 sub02 cat01:
    *     - file IO header
    *   o fun02 sec02 sub02 cat02:
    *     - sam file input parameter
    *   o fun02 sec02 sub02 cat03:
    *     - output file parameter
    *   o fun02 sec02 sub02 cat04:
    *     - output variant tsv parameter
    \****************************************************/

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub02 Cat01:
    +   - file IO header
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf((FILE *) outFILE, "  Input and Output:\n");

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub02 Cat02:
    +   - sam file input parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -sam: [Required; stdin]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Sam file with mapped reads\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Use '-' for stdin input\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub02 Cat03:
    +   - output file parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *) outFILE,
       "    -out: [stdout]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Sam file to save output cosnesuses in\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Use '-' for stdout output\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun02 Sec02 Sub02 Cat04:
    +   - output variant tsv parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       (FILE *)outFILE,
       "    -out-tsv: [Not used]\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Name of tsv to save filtered variants to"
    );

    fprintf((FILE *) outFILE, "\n");

   if(! pFullC)
      goto skipExtaSettings_fun02_sec02_sub05;

   /****************************************************\
   * Fun02 Sec02 Sub03:
   *   - print cosnesus settings
   *   o fun02 sec02 sub03 cat01:
   *     - Consensus settings header
   *   o fun02 sec02 sub03 cat02:
   *     - minimum mapping quality parameter
   *   o fun02 sec02 sub03 cat03:
   *     - minimum Base q-score value parameter
   *   o fun02 sec02 sub03 cat04:
   *     - minimum insertion q-score value parameter
   *   o fun02 sec02 sub03 cat05:
   *     - depht minimum setting
   *   o fun02 sec02 sub03 cat06:
   *     - minimum length setting
   *   o fun02 sec02 sub03 cat07:
   *     - mask parameter
   *   o fun02 sec02 sub03 cat08:
   *     - snp minimum percent support
   *   o fun02 sec02 sub03 cat09:
   *     - insertion minimum percent setting
   *   o fun02 sec02 sub03 cat10:
   *     - deletion minimum percent setting
   \****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - consensus settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf((FILE *) outFILE, "  Cosensus settings:\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat02:
   +   - minimum mapping quality parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-mapq: [%i]\n",
      def_minMapq_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum mapping quailty to keep an read\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat03:
   +   - minimum Base q-score value parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   fprintf(
      (FILE *) outFILE,
      "    -min-q: [%i]\n",
      def_minNtQ_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum Q-score to keep an base\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat04:
   +   - minimum insertion q-score value parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   fprintf(
      (FILE *) outFILE,
      "    -min-q-ins: [%i]\n",
      def_minInsQ_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum Q-score to keep an insertion\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat05:
   +   - depth minimum setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-depth: [%i]\n",
      def_minDepth_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum read depth to not break consensus"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        into fragments\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat06:
   +   - minimum length setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len: [%i]\n",
      def_minLen_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum fragment length (-min-depth)"
    );

    fprintf(
       (FILE *) outFILE,
       " needed\n        to keep an fragment\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat06:
   +   - mask parameter
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -mask: [%c]\n",
      def_mask_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Base to mask unsupported positions with\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat07:
   +   - snp minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-snp-sup: [%.2f]\n",
      def_minSnpPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to not"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        mask an SNP/match (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat08:
   +   - insertion minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-ins-sup: [%.2f]\n",
      def_minInsPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to keep an"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        insertion (range: 0 to 1)\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Unsupported positions are removed\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat09:
   +   - deletion minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -perc-del-sup: [%.2f]\n",
      def_minDelPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to keep an"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        deletion (range: 0 to 1)\n"
    );

    fprintf(
       (FILE *) outFILE,
       "      o Unsupported positions are replaced with"
    );

   fprintf((FILE *) outFILE, " -mask\n");

   /****************************************************\
   * Fun02 Sec02 Sub04:
   *   - print variant file settings
   *   o fun02 sec02 sub04 cat01:
   *     - variant file header
   *   o fun02 sec02 sub04 cat02:
   *     - print depth minimum setting
   *   o fun02 sec02 sub04 cat03:
   *     - print snp minimum percent setting
   *   o fun02 sec02 sub04 cat04:
   *     - print insertion minimum percent setting
   *   o fun02 sec02 sub04 cat05:
   *     - print deletion minimum percent setting
   \****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat01:
   +   - consensus settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf((FILE *) outFILE, "  Print variants:\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat02:
   +   - print depth minimum setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-min-depth: [%i]\n",
      def_minPrintDepth_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum read depth print out an variant\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat03:
   +   - print snp minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-snp-sup: [%.2f]\n",
      def_minSnpPrintPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       "\n        an SNP/match variant (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat04:
   +   - print insertion minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-ins-sup: [%.2f]\n",
      def_minInsPrintPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       " an\n        insertion variant (range: 0 to 1)\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat05:
   +   - deletion minimum percent setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-perc-del-sup: [%.2f]\n",
      def_minDelPrintPerc_tbConDefs
   );

    fprintf(
       (FILE *) outFILE,
       "      o Minimum percent support needed to print"
    );

    fprintf(
       (FILE *) outFILE,
       " an\n        deletion variant (range: 0 to 1)\n"
    );

   /****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help message and version numbers
   \****************************************************/

   skipExtaSettings_fun02_sec02_sub05:;

   fprintf(
      (FILE *) outFILE,
      "  -h: Print the shortened help message\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -h-all: Print the entire help message\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: Print the version number\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");

   fprintf(
      outFILE,
      "  - Prints the consensus to -out as a sam file\n"
    );

    fprintf(
       outFILE,
      "  - Prints variants to -out-tsv (if provided)\n"
    );
} /*phelp_mainTbCon*/

/*-------------------------------------------------------\
| Fun03: input_mainTbCon
|   - gets user input
| Input:
|   - numArgsSI:
|     o number arguments user input
|   - argStr
|     o c-string array with user input
|   - samFileStr:
|     o pointer to c-string to hold the path to the sam
|       file
|   - outFileStr:
|     o pointer to the file to output the sam entries for
|       the consensus to
|   - tsvFileStr:
|     o pointer to the tsv file to output the variants
|       to
|   - refLenUI:
|     o pointer to unsigned int to hold the reference
|       length
|   - tbConhSetSTPtr:
|     o pointer to tbConSet structure with other settings
| Output:
|    - Modifies:
|      o all variables input to their specified values
|    - Prints:
|      o any errors to stderr
|      o help message (if requested) to stdout
|      o version number (if requested) to stdout
|    - Returns:
|      o 0 for no errors
|      o 1 if printed help message or version number
|      o 2 for iput error
\-------------------------------------------------------*/
short
input_mainTbCon(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStr, /*Input sam file to use*/
   signed char **outFileStr, /*File to output to*/
   signed char **tsvFileStr, /*Path to tsv file to pring*/
   unsigned int *refLenUI,   /*Length of reference*/
   struct set_tbCon *settings
){ /*input_mainTbCon*/
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: input_mainTbCon
   '   - Check a single user input argument
   '   o fun03 sec01:
   '      - variable declerations
   '   o fun03 sec02:
   '     - check if user input anything
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   schar *tmpStr = 0;
   sint siArg = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if user input anything
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainTbCon(0, stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - file io + start loop
   ^   o fun03 sec03 sub02:
   ^     - reference data
   ^   o fun03 sec03 sub03:
   ^     - quality scores, length, & depth (non-print)
   ^   o fun03 sec03 sub04:
   ^     - length & depth (non-print)
   ^   o fun03 sec03 sub05:
   ^     - min percent support for snp/match, ins, & del
   ^   o fun03 sec03 sub06:
   ^     - masking
   ^   o fun03 sec03 sub07:
   ^     - variant printing settings
   ^   o fun03 sec03 sub08:
   ^     - help message
   ^   o fun03 sec03 sub09:
   ^     - full help message
   ^   o fun03 sec03 sub10:
   ^     - version number
   ^   o fun03 sec03 sub11:
   ^     - invalid input
   ^   o fun03 sec03 sub12:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file io + start loop
   \*****************************************************/

   siArg = 1;

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: The user input a sam file*/
         ++siArg;
         *samFileStr = (schar *) argAryStr[siArg];
      } /*If: The user input a sam file*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: an output file*/
         ++siArg;
         *outFileStr = (schar *) argAryStr[siArg];
      } /*Else If: an output file*/

      else if(
         ! eql_charCp(
            (schar *) "-out-tsv",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: variant output file*/
         ++siArg;
         *tsvFileStr = (schar *) argAryStr[siArg];
      } /*Else If: variant output file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - reference data
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-ref-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: reference length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToUI_base10str(
              (schar *) argAryStr[siArg],
              refLenUI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-ref-len %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: reference length*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - quality scores variables
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              (schar *) argAryStr[siArg],
              &settings->minQSI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-min-q %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: minimum q-score*/

      else if(
         ! eql_charCp(
            (schar *) "-min-q-ins",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: insertion q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              (schar *) argAryStr[siArg],
              &settings->minInsQSI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-min-q-ins %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: insertion q-score*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mapq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: mapping quality*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToUC_base10str(
              (schar *) argAryStr[siArg],
              &settings->minMapqUC
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: mapping quality*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - length & depth (non-print)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum fragment length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              (schar *) argAryStr[siArg],
              &settings->minLenSI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-min-len %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: minimum fragment length*/

      else if(
         ! eql_charCp(
            (schar *) "-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              (schar *) argAryStr[siArg],
              &settings->minDepthSI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: minimum depth*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - min percent support for snp/match, ins, & del
      \**************************************************/

      /*atof reurns 0 for failure*/
      else if(
         ! eql_charCp(
            (schar *) "-perc-snp-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum % snp support*/
         ++siArg;
         settings->minPercSnpF = atof(argAryStr[siArg]);
      } /*Else If: minimum % snp support*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-ins-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum % ins support*/
         ++siArg;
         settings->minPercInsF = atof(argAryStr[siArg]);
      } /*Else If: minimum % ins support*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-del-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum % del support*/
         ++siArg;
         settings->minPercDelF = atof(argAryStr[siArg]);
      } /*Else If: minimum % del support*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - masking
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-mask",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: base to mask with*/
         ++siArg;
         settings->maskSC = *argAryStr[siArg];
      } /*Else If: base to mask with*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - variant printing settings
      \**************************************************/

      /*Settings for printing out variations*/
      else if(
         ! eql_charCp(
            (schar *) "-p-min-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum print depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              (schar *) argAryStr[siArg],
              &settings->minPrintDepthSI
            );

         /*Check for errors*/
         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-p-min-depth %s; non-numeric/ to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: minimum print depth*/

      /*The print percentage depths*/
      else if(
         ! eql_charCp(
            (schar *) "-p-perc-snp-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum print % snp support*/
         ++siArg;

         settings->printMinSupSnpF =
            atof(argAryStr[siArg]);
      } /*Else If: minimum print % snp support*/

      else if(
         ! eql_charCp(
            (schar *) "-p-perc-ins-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: minimum print % ins support*/
         ++siArg;

         settings->printMinSupInsF =
            atof(argAryStr[siArg]);
      } /*If: minimum print % ins support*/

      else if(
         ! eql_charCp(
            (schar *) "-p-perc-del-sup",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum print % snp support*/
         ++siArg;

         settings->printMinSupDelF =
            atof(argAryStr[siArg]);
      } /*Else If: minimum print % snp support*/

      /**************************************************\
      * Fun03 Sec03 Sub08:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing help message*/
         phelp_mainTbCon(0, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing help message*/
         phelp_mainTbCon(0, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing help message*/
         phelp_mainTbCon(0, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing help message*/
         phelp_mainTbCon(0, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing help message*/
         phelp_mainTbCon(0, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing help message*/

      /**************************************************\
      * Fun03 Sec03 Sub09:
      *   - full help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h-all",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing full help message*/
         phelp_mainTbCon(1, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing full help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h-all",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing full help message*/
         phelp_mainTbCon(1, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing full help message*/

      else if(
         ! eql_charCp(
            (schar *) "help-all",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing full help message*/
         phelp_mainTbCon(1, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing full help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help-all",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing full help message*/
         phelp_mainTbCon(1, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing full help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help-all",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing full help message*/
         phelp_mainTbCon(1, stdout);
         goto phelp_fun03_sec04;
      } /*Else If: printing full help message*/

      /**************************************************\
      * Fun03 Sec03 Sub10:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing version number*/
         pversion_mainTbCon(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: printing version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing version number*/
         pversion_mainTbCon(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: printing version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing version number*/
         pversion_mainTbCon(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: printing version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing version number*/
         pversion_mainTbCon(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: printing version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: printing version number*/
         pversion_mainTbCon(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: printing version number*/

      /**************************************************\
      * Fun03 Sec03 Sub11:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: not recognized*/
         fprintf(
            stderr,
            "%s not recognized\n",
            argAryStr[siArg]
         );
 
         goto err_fun03_sec04;
      } /*Else: not recognized*/

      /**************************************************\
      * Fun03 Sec03 Sub12:
      *   - move to next argument
      \**************************************************/
 
      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Se04:
   ^   - return result
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
} /*input_mainTbCon*/


/*-------------------------------------------------------\
| Main:
|   - driver function to build the consensus with
| Input:
|   - the system provides these
| Output:
|   - Prints:
|     o consensus as an sam file
|     o variants to tsv file (if requested)
\-------------------------------------------------------*/
#ifdef PLAN9
char
#else
int
#endif
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - builds a consensus from the input sam file
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize, get input, and check input
   '   o main sec03:
   '     - print sam file header
   '   o main sec04:
   '     - build consensus
   '   o main sec05:
   '     - print and collapse the consensus
   '   o main sec06:
   '     - clean up and return errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*files to output to*/
   schar *samFileStr = 0;
   schar *outFileStr = 0;
   schar *tsvFileStr = 0; /*file to print results to*/

   uint lenRefUI = def_refLen_tbConDefs;
      /*expected length of reference*/

   schar refIdStr[256]; /*name of reference*/
      /*i would be supprised if a reference id was longer
      `   then 64 characters, so 256 should be safe
      */

   struct set_tbCon setStackST;

   /*Iterators for loops*/
   sint siIter = 0;

   /*Files for input*/
   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*Non-user input*/
   /*For the sam file*/
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   ulong ulRead = 0;
   struct samEntry samStackST;
 
   /*Grabbing reference id from sam file*/
   schar *tmpStr = 0;
   ulong tabUL = mkDelim_ulCp('\t');
   schar multiRefBl = 0; /*1: means multiple refs/exit*/
   
   /*For building the consensus*/
   struct conNt_tbCon *conNtHeapAryST = 0;

   /*For collapsing the consensus*/
   schar errSC = 0;
   sint numFragSI = 0;
   struct samEntry *samHeapAryST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - open sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   /*Default reference id*/
   refIdStr[0] = 'N';
   refIdStr[2] = 'A';
   refIdStr[3] = '\0';

   init_set_tbCon(&setStackST);
   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainTbCon(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &outFileStr,
         &tsvFileStr,
         &lenRefUI,
         &setStackST
      );

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version to no error (0)*/
      goto cleanUp_main_sec06_sub04;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open sam file
   \*****************************************************/

   if(
         samFileStr == 0
      || samFileStr[0] == '-'
   ) samFILE = stdin;

   else
   { /*Else: sam file input*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(!samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
             stderr,
             "Could not open -sam %s\n",
             samFileStr
         );

         goto fileErr_main_sec06_sub03;
      } /*If: I could not open the sam file*/
   } /*Else: sam file input*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open output file
   \*****************************************************/

   if(
         outFileStr == 0
      || outFileStr[0] == '-'
   ) outFILE = stdout;

   else
   { /*Else: out file input*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(!outFILE)
      { /*If: I could not open the out file*/
         fprintf(
             stderr,
             "Could not open -out %s\n",
             outFileStr
         );

         goto fileErr_main_sec06_sub03;
      } /*If: I could not open the out file*/
   } /*Else: out file input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print sam file header
   ^   o main sec03 sub01:
   ^     - get first sam file entry (likely header)
   ^   o main sec03 sub02:
   ^     - get refernce lenths fom SQ entries
   ^   o main sec03 sub03:
   ^     - print heaader and get next entry
   ^   o main sec03 sub04:
   ^     - check for errors
   ^   o main sec03 sub05:
   ^     - print tbCon cosensus (same file) program header
   ^   o main sec03 sub06:
   ^     - print tbCon tsv settings; finish program header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first sam file entry (likely header)
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );

      goto memErr_main_sec06_sub02;
   } /*If: memory error*/

   errSC =
      get_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get refernce lenths fom SQ entries
   \*****************************************************/

   while(! errSC)
   { /*Loop: print the header*/
       if(samStackST.extraStr[0] != '@') 
          break; /*off of header*/

       if(
          ! eql_charCp(
             (schar *) "@SQ\t",
             samStackST.extraStr,
             (schar) '\t'
           )
       ){ /*If: has length information*/
          if(multiRefBl)
          { /*If: multiple references*/
             fprintf(
                stderr,
                "-sam %s has multiple references\n",
                samFileStr
             ); /*Let user know the problem*/

             goto fileErr_main_sec06_sub03;
          } /*If: multiple references*/

          multiRefBl = 1;

          /*Get past "@SQ\t"*/
          tmpStr = samStackST.extraStr + 4; 

          while(*tmpStr++ != ':')
          { /*Loop: past next part of SQ line*/
             if(*tmpStr < 31)
                goto pheader_main_sec03_sub03;
          } /*Loop: past next part of SQ line*/

          tmpStr +=
             cpDelim_ulCp(
                refIdStr,
                tmpStr,
                tabUL,
                '\t'
             );

          ++tmpStr;
         
          if(*tmpStr < 31 )
             goto pheader_main_sec03_sub03;

          /*Move past the LN: flag*/
          while(*tmpStr++ !=':')
          { /*Loop: get past LN: flag*/
             if(*tmpStr < 31)
                goto pheader_main_sec03_sub03;
          } /*Loop: get past LN: flag*/

          /*Get the reference length*/
          tmpStr +=
             strToUI_base10str(
                tmpStr,
                &lenRefUI
             );

          /*check if I had a conversion error*/
          if(*tmpStr > 31)
             lenRefUI = def_refLen_tbConDefs;
       } /*If: has length information*/

       /*************************************************\
       * Main Sec03 Sub03:
       *   - print heaader and get next entry
       \*************************************************/

       pheader_main_sec03_sub03:;

       p_samEntry(
          &samStackST,
          &buffHeapStr,
          &lenBuffUL,
          0,          /*0 = print newline*/
          outFILE
       );

       errSC =
          get_samEntry(
             &samStackST,
             &buffHeapStr,
             &lenBuffUL,
             samFILE
          );
   } /*Loop: print the header*/

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - check for errors
   \*****************************************************/

   if(errSC)
   { /*If: had error*/
      if(errSC == def_EOF_samEntry)
      { /*If: had no reads*/
         fprintf(
            stderr,
            "-sam %s has no reads\n",
            samFileStr
         );

         goto fileErr_main_sec06_sub03;
      } /*If: had no reads*/

      fprintf(
         stderr,
         "memory error reading header of -sam %s\n",
         samFileStr
      );

      goto memErr_main_sec06_sub02;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec03 Sub05:
   *   - print tbCon cosensus (same file) program header
   \*****************************************************/

   fprintf(
      outFILE,
      "@PG\tID:tbCon\tPN:tbCon"
   );

   fprintf(
      outFILE,
      "\tVN:%i-%02i-%02i\tCL:tbCon",
      def_year_tbConDefs,
      def_month_tbConDefs,
      def_day_tbConDefs
   );

   /*Check if getting sam file from stdin*/
   if(! samFileStr || *samFileStr == '-')
     fprintf(outFILE, " -sam -");
   else
     fprintf(outFILE, " -sam %s", samFileStr);

   fprintf(
      outFILE,
      " -min-mapq %i -min-q %i -min-q-ins %i",
      setStackST.minMapqUC,
      setStackST.minQSI,
      setStackST.minInsQSI
   );

   fprintf(
      outFILE,
      " -min-len %i -min-depth %i -perc-snp-sup %f",
      setStackST.minLenSI,
      setStackST.minDepthSI,
      setStackST.minPercSnpF
   );

   fprintf(
      outFILE,
      " -perc-ins-sup %f -min-del-sup %f",
      setStackST.minPercInsF,
      setStackST.minPercDelF
   );

   /*****************************************************\
   * Main Sec03 Sub06:
   *   - print tbCon tsv settings (part of program header)
   \*****************************************************/

   if(tsvFileStr)
   { /*If: the user is printing the tsv file*/
      fprintf(
         outFILE,
         " -p-min-depth %i -p-perc-snp-sup %f",
         setStackST.minPrintDepthSI,
         setStackST.printMinSupSnpF
      );

      fprintf(
         outFILE,
         " -p-perc-ins-sup %f -p-min-del-sup %f",
         setStackST.printMinSupInsF,
         setStackST.printMinSupDelF
      );

      fprintf(
         outFILE,
          " -out-tsv %s\n",
          tsvFileStr
      );
   } /*If: the user is printing the tsv file*/

   if(! outFileStr || *outFileStr == '-')
     fprintf(outFILE, " -out -\n");
   else
     fprintf(outFILE, " -out %s\n", outFileStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - build the consensus
   ^   o main sec04 sub01:
   ^     - add reads to the consensus
   ^   o main sec04 sub02:
   ^     - check for errors and minor clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - add reads to the consensus
   \*****************************************************/

   while(! errSC)
   { /*Loop: add reads to consensus*/
       ++ulRead;

       /*Check if the read is an umapped read (4), an
       `   secondary alignemnt (256), or an supplementary
       `   alignment (2048)
       */
       if(samStackST.flagUS & (4 | 256 | 2048))
           goto nextEntry_main_sec04_sub01;

       if(samStackST.mapqUC < setStackST.minMapqUC)
          goto nextEntry_main_sec04_sub01;

       errSC =
          addRead_tbCon(
             &samStackST,
             &conNtHeapAryST,
             &lenRefUI, 
             &setStackST
          ); /*Build the consensus; read by read*/

       if(errSC & def_memErr_tbConDefs)
       { /*If: I had a memory error*/
          fprintf(
             stderr,
             "memory error while building consensus\n"
          );

          goto memErr_main_sec06_sub02;
       } /*If: I had a memory error*/

       nextEntry_main_sec04_sub01:;

       errSC =
          get_samEntry(
             &samStackST,
             &buffHeapStr,
             &lenBuffUL,
             samFILE
          );
   } /*Loop: add reads to consensus*/

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - check for errors and minor clean up
   \*****************************************************/

   if(errSC == def_memErr_samEntry)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error for read %lu in -sam %s\n",
         ulRead,
         samFileStr
      );

      goto memErr_main_sec06_sub02;
   } /*If: memory error*/

   freeStack_samEntry(&samStackST);

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - print and collapse the consensus
   ^   o main sec05 sub01:
   ^     - print tsv variant file
   ^   o main sec05 sub02:
   ^     - check if variant and consensus to same file
   ^   o main sec05 sub03:
   ^     - collapse consensuses and print (minor freeing)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - print tsv variant file
   \*****************************************************/

   if(tsvFileStr)
   { /*If: the user wanted the variants*/
      errSC =
         pvar_tbCon(
            conNtHeapAryST,
            lenRefUI,
            refIdStr,
            &setStackST,
            tsvFileStr
         ); /*Print out the variants (not a vcf)*/

      if(errSC)
      { /*If: I had a file error*/
         fprintf(
            stderr,
            "could not open -out-tsv %s; skipping tsv\n",
            tsvFileStr
         );
      } /*If: I had a file error*/
   } /*If: the user wanted the variants*/

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - check if variant and consensus to same file
   \*****************************************************/

   if(outFILE == stdout)
   { /*If: the ouput file was stdout*/
     if((tsvFileStr && *tsvFileStr == '-'))
        fprintf(outFILE, "\n");
   } /*If: the ouput file was stdout*/

   else if(
         tsvFileStr
      && ! eql_charCp(outFileStr, tsvFileStr, '\0')
   ) fprintf(outFILE, "\n"); /*out and tsv file are same*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - collapse consensuses and print (minor freeing)
   \*****************************************************/

   /*Print the consensuses*/
   samHeapAryST =
      collapse_tbCon(
         conNtHeapAryST,
         lenRefUI,
         &numFragSI,
         refIdStr,
         &setStackST,
         &errSC
      ); /*Collapse the consensus*/

   freeHeapAry_conNt_tbCon(
      conNtHeapAryST,
      lenRefUI
   );

   conNtHeapAryST = 0;
   lenRefUI = 0;

   /*Print the consensuses*/
   for(
      siIter = 0;
      siIter < numFragSI;
      ++siIter
   ){ /*Loop: print and free each sam entry*/
      /*check if false entry (numFragSI is off)*/
      if(samHeapAryST[siIter].qryIdStr[0] != '\0')
          p_samEntry(
             &samHeapAryST[siIter],
             &buffHeapStr,
             &lenBuffUL,
             0,
             outFILE
          ); /*Print consensuses as sam file*/

      freeStack_samEntry(&samHeapAryST[siIter]);
   } /*Loop: print and free each sam entry*/

   free(samHeapAryST);
   numFragSI = 0;
   samHeapAryST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - clean up and return errors
   ^   o main sec06 sub01:
   ^     - no error clean up
   ^   o main sec06 sub02:
   ^     - memory error clean up
   ^   o main sec06 sub03:
   ^     - file error clean up
   ^   o main sec06 sub04:
   ^     - general clean up (both no error and error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec06_sub04;

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec06_sub02:;
   errSC = 1;
   goto cleanUp_main_sec06_sub04;

   /*****************************************************\
   * Main Sec06 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_main_sec06_sub03:;
   errSC = 2;
   goto cleanUp_main_sec06_sub04;

   /*****************************************************\
   * Main Sec06 Sub04:
   *   - general clean up (both no error and error)
   \*****************************************************/

   cleanUp_main_sec06_sub04:;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;
   lenBuffUL = 0;

   /*free consensuses*/
   for(
      siIter = 0;
      siIter < numFragSI;
      ++siIter
   ) freeStack_samEntry(&samHeapAryST[siIter]);

   numFragSI = 0;

   if(samHeapAryST)
      free(samHeapAryST);
   samHeapAryST = 0;

   freeStack_samEntry(&samStackST);
   freeStack_set_tbCon(&setStackST);

   freeHeapAry_conNt_tbCon(
      conNtHeapAryST,
      lenRefUI
   );

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

   return errSC;
} /*main*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
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
