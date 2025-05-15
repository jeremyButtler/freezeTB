/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and defined variables
'   o fun01: pversion_mainWhoToAmr
'     - prints version number for whoToAmr
'   o fun02: phelp_mainWhoToAmr
'     - prints help message for whoToAmr
'   o fun03: input_mainWhoToAmr
'     - gets user input for mainWhoToAmr
'   o main:
'     - main function
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
   #include <string.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/charCp.h"
#include "../genFreezeTB/amrST.h"
#include "../genFreezeTB/genIndice.h"
#include "../genFreezeTB/whoToAmr.h"

/*no .c files*/
#include "../ftbVersion.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden includes
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .c  #include "../genBio/codonFun.h"
!   o .c  #include "../genFreezeTB/drugAry.h"
!   o .h  #include "../genLib/ntTo2Bit.h"
!   o .h  #include "../genLib/revNtTo2Bit.h"
!   o .h  #include "../genLib/codonTbl.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainWhoToAmr
|   - prints version number for whoToAmr
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainWhoToAmr(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "whoToAmr from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_mainWhoToAmr*/

/*-------------------------------------------------------\
| Fun02: phelp_mainWhoToAmr
|   - prints help message for whoToAmr
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help messsage to outFILE
\-------------------------------------------------------*/
void
phelp_mainWhoToAmr(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - print usage entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "whoToAmr -coords gene_coords.tsv"
   );

   fprintf(
      (FILE *) outFILE,
      "  -master tab_one.tsv -indice tab_two.tsv"
   );

   fprintf(
      (FILE *) outFILE,
      "  -ref rerence.fa > out.tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - converts 2023 WHO tuberculosis AMR catalog%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    to tbAmr format (is a tsv file)%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input entry
   ^   o fun02 sec02 sub01:
   ^     - input block
   ^   o fun02 sec02 sub02:
   ^     - master tab input
   ^   o fun02 sec02 sub03:
   ^     - indice tab input
   ^   o fun02 sec02 sub04:
   ^     - 2021 mode
   ^   o fun02 sec02 sub05:
   ^     - output file
   ^   o fun02 sec02 sub06:
   ^     - all  AMRs entry
   ^   o fun02 sec02 sub07:
   ^     - whole gene entries
   ^   o fun02 sec02 sub08:
   ^     - coordinates
   ^   o fun02 sec02 sub09:
   ^     - help message/version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *    - input block
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *    - master tab input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -master tab_one.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o master tab (1st) of WHO 2023 tuberculosis%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      catalog saved as a tsv file%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o if converting with excel, delete first%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      two headers (keep thrid header)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *    - indice tab input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -indice tab_two.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o genome indice tab (2nd) of WHO 2023%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      tuberculosis catalog (as tsv)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *    - 2021 mode
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -2021 indice.csv: [Replaces; -master & -indice]"
   );

   fprintf(
      (FILE *) outFILE,
      "%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o using 2021 WHO tubucerlosis catalog indice%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      tab (needs to be csv)%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o sets program to 2021 catalog mode%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *    - output file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out out.tsv: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o file to output converted database to%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *    - all  AMRs entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -all-amrs: [Optional; No]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o print grade 1, 2 3, 4, and 5 (all) AMRs%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "  -res-amrs: [Optional; Yes]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o print grade 1 and 2 AMRs (resistant)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *    - whole gene entries
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -gene-del: [Optional; Yes]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o print gene deletion AMRs%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -no-lof: [Optional; No]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o do not print gene deletion AMRs%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *    - coordinates
   *   o fun02 sec02 sub08 cat01:
   *     - coordinates description
   *   o fun02 sec02 sub08 cat02:
   *     - auto build coordinates file command
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub08 Cat01:
   +   - coordinates description
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -coords gene_coords.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o mapping coordinates of genes in reference%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o format:%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - column 1: gene name (ignored)%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - column 2: reference name (ignored)%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - column 3: genes reading frame direction%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        * \"+\" for foward reading frame%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        * \"-\" for reverse reading frame%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - column 4: starting coordiante of gene%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - column 5: ending coordiante of gene%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o auto build gene_coords.tsv:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub08 Cat02:
   +   - auto build coordinates file command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "      minimap2 -a --secondary=no ref.fa genes.fa |"
   );

   fprintf(
     (FILE *) outFILE,
     "%s        awk \'%s",
      str_endLine,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "           BEGIN{FS=OFS=\"\\t\"};%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "           {%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              if($6 !~ /^[0-9]*M$/) next;%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              if($2 != 16 && $2 != 0) next;%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              sub(/M/, \"\", $6);%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              --$6;%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              if($2 == 16) dirSC = \"-\";%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              else dirSC = \"+\";%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "              print $1, $3, dirSC, $4, $4 + $6;%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "           }' > gene_coords.tsv%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub09:
   *    - help message/version number
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints converted database to -out or stdout%s",
      str_endLine
   );
} /*phelp_mainWhoToAmr*/

/*-------------------------------------------------------\
| Fun03: input_mainWhoToAmr
|   - gets user input for mainWhoToAmr
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o c-string array with input arguments
|   - whoFistTabStr:
|     o pointer to point to who master tab (fist) tsv file
|       path
|   - whoSecTabStr:
|     o pointer to point to who indice tab (2nd) tsv file
|       path
|   - refStr:
|     o pointer to point to reference fasta file path
|   - outStr:
|     o pointer to point to output file path
|   - allAmrsBl:
|     o pointer to signed char to be set to 1 or 0
|       - 1: if user wanted all AMR grades
|       - 0: if user wanted just grade 1 and 2 AMRs
|   - rmGeneDelBl:
|     o pointer to signed char to be set to 1 or 0
|       - 1: if user wanted to remove gene deletion AMRs
|       - 0: if user wanted gene deletion AMRs
|   - who2023Bl:
|     o pointer to signed char to be set to 1 or 0
|       - 1: if user provided 2023 catalog files
|       - 0: if user provided 2021 catalog files
|     o the 2021 file pointer whoFirstTabStr
| Output:
|   - Modifies:
|     o all input (except numArgsSI and argAryStr) to
|       user provided input
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message/version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainWhoToAmr(
   int numArgsSI,
   char *argAryStr[],
   signed char **whoFirstTabStr,
   signed char **whoSecTabStr,
   signed char **refStr,
   signed char **coordsStr,
   signed char **outStr,
   signed char *allAmrsBl,
   signed char *rmGeneDelBl,
   signed char *who2023Bl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - get error and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no user input*/
      phelp_mainWhoToAmr(stdout);
      goto phelp_fun03_sec04;
   } /*If: no user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - get user input plust start loop
   ^   o fun03 sec03 sub02:
   ^     - help message requests
   ^   o fun03 sec03 sub03:
   ^     - version number requests
   ^   o fun03 sec03 sub04:
   ^     - invalid input
   ^   o fun03 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - get user input plust start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: Get user input*/
      if(! strcmp("-ref", argAryStr[siArg]))
      { /*If: reference sequence*/
         ++siArg;
         *refStr = (signed char *) argAryStr[siArg];
      } /*If: reference sequence*/

      else if(! strcmp("-coords", argAryStr[siArg]))
      { /*Else If: coordinates file*/
         ++siArg;
         *coordsStr = (signed char *) argAryStr[siArg];
      } /*Else If: coordinates file*/

      else if(! strcmp("-all-amrs", argAryStr[siArg]))
         *allAmrsBl = 1;

      else if(! strcmp("-res-amrs", argAryStr[siArg]))
         *allAmrsBl = 0;

      else if(! strcmp("-no-lof", argAryStr[siArg]))
         *rmGeneDelBl = 1;

      else if(! strcmp("-gene-del", argAryStr[siArg]))
         *rmGeneDelBl = 0;
      
      else if(! strcmp("-2021", argAryStr[siArg]))
      { /*Else If: 2021 who genome indice csv file*/
         ++siArg;
         *whoFirstTabStr =
             (signed char *) argAryStr[siArg];
         *who2023Bl = 0;
      } /*Else If: 2021 who genome indice csv file*/

      else if(! strcmp("-master", argAryStr[siArg]))
      { /*Else If: tab1 (master) of who 2023*/
         ++siArg;
         *whoFirstTabStr =
            (signed char *) argAryStr[siArg];
         *who2023Bl = 1;
      } /*Else If: tab1 (master) of who 2023*/

      else if(! strcmp("-indice", argAryStr[siArg]))
      { /*Else If: tab2 (genome indice) of who 2023*/
         ++siArg;
         *whoSecTabStr = (signed char *) argAryStr[siArg];
         *who2023Bl = 1;
      } /*Else If: tab2 (genome indice) of who 2023*/

      else if(! strcmp("-out", argAryStr[siArg]))
      { /*Else If: output file*/
         ++siArg;
         *outStr = (signed char *) argAryStr[siArg];
      } /*Else If: output file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - help message requests
      \**************************************************/

      else if(! strcmp("-h", argAryStr[siArg]))
      { /*Else If: help mesage*/
         phelp_mainWhoToAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help mesage*/

      else if(! strcmp("--h", argAryStr[siArg]))
      { /*Else If: help mesage*/
         phelp_mainWhoToAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help mesage*/

      else if(! strcmp("help", argAryStr[siArg]))
      { /*Else If: help mesage*/
         phelp_mainWhoToAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help mesage*/

      else if(! strcmp("-help", argAryStr[siArg]))
      { /*Else If: help mesage*/
         phelp_mainWhoToAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help mesage*/

      else if(! strcmp("--help", argAryStr[siArg]))
      { /*Else If: help mesage*/
         phelp_mainWhoToAmr(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help mesage*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - version number requests
      \**************************************************/

      else if(! strcmp("-v", argAryStr[siArg]))
      { /*Else If: version number*/
         pversion_mainWhoToAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(! strcmp("--v", argAryStr[siArg]))
      { /*Else If: version number*/
         pversion_mainWhoToAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(! strcmp("version", argAryStr[siArg]))
      { /*Else If: version number*/
         pversion_mainWhoToAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(! strcmp("-version", argAryStr[siArg]))
      { /*Else If: version number*/
         pversion_mainWhoToAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(! strcmp("--version", argAryStr[siArg]))
      { /*Else If: version number*/
         pversion_mainWhoToAmr(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is no recognized%s",
            argAryStr[siArg],
            str_endLine
         );
 
         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - get error and return
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
} /*input_mainWhoToAmr*/

/*-------------------------------------------------------\
| Main:
|   - main fuction to convert the WHO catalog to tbAmr
|     format
\-------------------------------------------------------*/
int
main(
   int numArgsSI,       /*number arguments from user*/
   char *argAryStr[]    /*arguments from user*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get and check user input
   '   o main sec03:
   '     - read in AMRs from the catalog
   '   o main sec04:
   '     - parse each AMRs variant id
   '   o main sec05:
   '     - add in the aa data
   '   o main sec06:
   '     - print AMRs
   '   o main sec07:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *whoFirstTabStr = 0;
   signed char *whoSecTabStr = 0;
   signed char *refStr = 0;
   signed char *coordsStr = 0;
   signed char *outStr = (signed char *) "-";

   signed char allAmrsBl = 0;
   signed char rmGeneDelBl = 0;
   signed char who2023Bl = 1; /*2023 catalog entries*/

   unsigned long numAmrUL = 0;
   signed char *drugHeapAryStr = 0;
   signed int numDrugsSI = 0;

   signed char errSC = 0;

   FILE *checkFILE = 0;

   struct amrST *amrHeapAryST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check user input
   ^   o main sec02 sub01:
   ^     - get user input
   ^   o main sec02 sub02:
   ^     - check if can open first tab
   ^   o main sec02 sub03:
   ^     - check if can open second tab of 2023 catalog
   ^   o main sec02 sub04:
   ^     - check if the user provided the reference file
   ^   o main sec02 sub05:
   ^     - check if user provided gene coordinates file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainWhoToAmr(
         numArgsSI,
         argAryStr,
         &whoFirstTabStr,
         &whoSecTabStr,
         &refStr,
         &coordsStr,
         &outStr,
         &allAmrsBl,
         &rmGeneDelBl,
         &who2023Bl
      ); /*get user input*/

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version to no error*/
      goto cleanUp_main_sec07_sub04;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if can open first tab
   \*****************************************************/

   if(! whoFirstTabStr)
   { /*If: no first file was provied*/

      fprintf(
         stderr,
         "No file for -2021 or -master%s",
         str_endLine
      );

      goto fileErr_main_sec07_sub03;
   } /*If: No first file was provied*/

   checkFILE =
      fopen(
         (char *) whoFirstTabStr,
         "r"
      );

   if(! checkFILE)
   { /*If: no first file provied*/
      if(who2023Bl)
         fprintf(
            stderr,
            "could not open -master %s%s",
             whoFirstTabStr,
             str_endLine
         );

      else
         fprintf(
            stderr,
            "could not open -2021 %s%s",
             whoFirstTabStr,
             str_endLine
         );

      goto fileErr_main_sec07_sub03;
   } /*If: no first file provied*/

   fclose(checkFILE);
   checkFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - check if can open second tab of 2023 catalog
   \*****************************************************/

   if(who2023Bl)
   { /*If: have 2023 catalog entry*/
      if(! whoSecTabStr)
      { /*If: file not input*/
         fprintf(
            stderr,
            "no file for -indice%s",
            str_endLine
         );

         goto fileErr_main_sec07_sub03;
      } /*If: file not input*/

      checkFILE =
         fopen(
            (char *) whoSecTabStr,
            "r"
         );

      if(! checkFILE)
      { /*If: could not open second tab file*/
         fprintf(
            stderr,
            "Could not open -indice %s%s",
             whoSecTabStr,
             str_endLine
         );
   
         goto fileErr_main_sec07_sub03;
      } /*If: could not open second tab file*/

      fclose(checkFILE);
      checkFILE = 0;
   } /*If: have 2023 catalog entry*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if user provided reference file
   \*****************************************************/

   if(! refStr)
   { /*If: no reference sequence file was input*/
      fprintf(
         stderr,
         "a reference sequence (-ref ref.fa) is needed%s",
         str_endLine
      );

      goto fileErr_main_sec07_sub03;
   } /*If: no reference sequence file was input*/

   checkFILE =
      fopen(
         (char *) refStr,
         "r"
      );

   if(! checkFILE)
   { /*If: could not open reference sequence file*/
         fprintf(
            stderr,
            "Could not open -ref %s%s",
             refStr,
             str_endLine
         );
   
         goto fileErr_main_sec07_sub03;
   } /*If: could not open reference sequence file*/

   fclose(checkFILE);
   checkFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - check if user provided gene coordinates file
   \*****************************************************/

   if(! coordsStr)
   { /*If: no gene coordiantes file was input*/
      fprintf(
         stderr,
         "gene coordiantes (-coords genes.tsv) are needed"
      );

      fprintf(stderr, "%s", str_endLine);
      goto fileErr_main_sec07_sub03;
   } /*If: no gene coordinates file was input*/

   checkFILE =
      fopen(
         (char *) coordsStr,
         "r"
      );

   if(! checkFILE)
   { /*If: could not open gene coordinates file*/
       fprintf(
          stderr,
          "Could not open -coords %s%s",
           refStr,
           str_endLine
       );
   
      goto fileErr_main_sec07_sub03;
   } /*If: could not open gene coordinates file*/

   fclose(checkFILE);
   checkFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read AMRs from the catalog
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   switch(who2023Bl)
   { /*Switch: check amr input method*/
      case 0:
         amrHeapAryST =
            WHO2021Catalog_whoToAmr(
               whoFirstTabStr,
               &numAmrUL,
               &drugHeapAryStr,
               &numDrugsSI,
               allAmrsBl,
               &errSC
         ); /*get amrs from 2021 catalog*/

         break;

      case 1:
         amrHeapAryST =
             WHO2023Catalog_whoToAmr(
                whoFirstTabStr,
                whoSecTabStr,
                &numAmrUL,
                &drugHeapAryStr,
                &numDrugsSI,
                allAmrsBl,
                rmGeneDelBl,
                &errSC
         ); /*get amrs from 2023 catalog*/

         break;
   } /*Switch: check amr input method*/

   if(amrHeapAryST == 0)
   { /*If: could not extract AMRs*/
      if(errSC == def_memErr_amrST)
      { /*If: memory error*/
         fprintf(
            stderr,
            "mememory error: making AMR array%s",
            str_endLine
         );

         goto memErr_main_sec07_sub02;
      } /*If: memory error*/

      else
      { /*If: file error*/
         fprintf(
            stderr,
            "catalog file not in correct format%s",
            str_endLine
         );

         goto fileErr_main_sec07_sub03;
      } /*If: file error*/
   } /*If: could not extract AMRs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - parse each AMRs variant id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      parseVarId_whoToAmr(
         amrHeapAryST,
         (signed int) numAmrUL
      );

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error: processing variant ids%s",
         str_endLine
      );

      goto memErr_main_sec07_sub02;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - Add in the aa data
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      addCodonPos_whoToAmr(
         amrHeapAryST,
         (int) numAmrUL,
         coordsStr,
         refStr
      );

   if(errSC)
   { /*If: memory error*/
      /*already checked the files, likely memory error*/

      fprintf(
         stderr,
         "memory error: finding AMR codon positions%s",
         str_endLine
      );

      goto memErr_main_sec07_sub02;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - print AMRs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      p_amrST(
         amrHeapAryST,
         numAmrUL,
         drugHeapAryStr,
         numDrugsSI,
         outStr
      );

   if(errSC)
   { /*If: file error*/
      fprintf(
         stderr,
         "could not print to -out %s%s",
         outStr,
         str_endLine
      );

      goto fileErr_main_sec07_sub03;
   } /*If: file error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec07:
   ^   - clean up and return
   ^   o main sec07 sub01:
   ^     - no error clean up
   ^   o main sec07 sub02:
   ^     - memory error clean up
   ^   o main sec07 sub03:
   ^     - file error clean up
   ^   o main sec07 sub04:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec07 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec07_sub04;

   /*****************************************************\
   * Main Sec07 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec07_sub02:;
   errSC = 2;
   goto cleanUp_main_sec07_sub04;

   /*****************************************************\
   * Main Sec07 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_main_sec07_sub03:;
   errSC = 3;
   goto cleanUp_main_sec07_sub04;

   /*****************************************************\
   * Main Sec07 Sub04:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_main_sec07_sub04:;

   if(amrHeapAryST)
      freeHeapAry_amrST(
         amrHeapAryST,
         numAmrUL
      );

   amrHeapAryST = 0;

   if(drugHeapAryStr)
      free(drugHeapAryStr);

   drugHeapAryStr = 0;

   if(
         checkFILE
      && checkFILE != stdin
      && checkFILE != stdout
   ) fclose(checkFILE);

   checkFILE = 0;

   return errSC;
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
