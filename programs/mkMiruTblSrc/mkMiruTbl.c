/*########################################################
# Name: mkMiruTbl
#   - adds primer coordinates to an MIRU table
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and definitions
'   o fun01: pversion_mkMiruTbl
'     - prints verision number
'   o fun02: phelp_mkMiruTbl
'     - prints help message
'   o fun03: input_mkMiruTbl
'     - gets user input
'   o main:
'     - Adds the primer coordinates to the MIRU table
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and definitions
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
#include "../genLib/fileFun.h"
#include "../genBio/seqST.h"
#include "../genAln/memwater.h"
#include "../genAln/alnSet.h"

/*.h files only*/
#include "../ftbVersion.h"
#include "../genLib/endLine.h"
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genAln/indexToCoord.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*maximum score for an base*/
#define def_minPercScore_mkMiruTbl 0.82f
#define def_divLen_mkMiruTbl 15
   /*this is to handle some primers having bases that get
   `   get trimmed off
   */

/*-------------------------------------------------------\
| Fun01: pversion_mkMiruTbl
|   - prints verision number
| Input:
|   - outFILE:
|     o file to print the version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mkMiruTbl(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "mkMiruTbl version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_mkMiruTbl*/

/*-------------------------------------------------------\
| Fun02: phelp_mkMiruTbl
|   - prints help message
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mkMiruTbl(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: phelp_mkMiruTbl
   '   - prints out the help message
   '   o fun02 sec01:
   '     - usage line and program description
   '   o fun02 sec02:
   '     - user input
   '   o fun02 sec03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage line and program description
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "mkMiruTbl -ref ref.fa -prim primers.fa -miru-tbl"
   );

   fprintf(
      outFILE,
      "table.tsv%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Adds in the primers coordinates to an table of"
   );

   fprintf(
      outFILE,
      "%s    MIRU lineages%s",
      str_endLine,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - user input
   ^   o fun02 sec02 sub01:
   ^     - input header
   ^   o fun02 sec02 sub02:
   ^     - reference paramater
   ^   o fun02 sec02 sub03:
   ^     - primer parameter
   ^   o fun02 sec02 sub04:
   ^     - MIRU table parameter
   ^   o fun02 sec02 sub05:
   ^     - min pecent of max score paramater
   ^   o fun02 sec02 sub06:
   ^     - help message opiton and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - reference paramater
   \*****************************************************/

   fprintf(
      outFILE,
      "  -ref: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Fasta with reference to map primers to%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - primer parameter
   \*****************************************************/

   fprintf(
      outFILE,
      "  -prim: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Fasta with primers to get coordinates of%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o The reverse primer must always come right%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      after the forward primer%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - MIRU table parameter
   \*****************************************************/

   fprintf(
      outFILE,
      "  -miru-tbl: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Tsv file with the MIRU table to add"
   );

   fprintf(outFILE, " coordinates to%s", str_endLine);

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - min pecent of max score paramater
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-perc-score: [%.2f]%s",
      def_minPercScore_mkMiruTbl,
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Minimum percent of max score needed to keep"
   );

   fprintf(outFILE, " primer%s", str_endLine);

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help message opiton and version number
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: Print this help message%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  -v: Print the version number%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Prints the MIRU table with the added primer%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "    coordinates (in header) to stdout%s",
      str_endLine
   );
} /*Print out the version number*/

/*-------------------------------------------------------\
| Fun03: input_mkMiruTbl
|   - gets user input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o arguments the user input
|   - refFileStr:
|     o will hold name/path of fasta file with reference
|   - primFileStr:
|     o will hold name/path of fasta file with primers
|   - tblFileStr:
|     o will hold name/path of tsv with MIRU table
|   - minPercScoreF:
|     o minimum percent score to keep an primer mapping
| Output:
|   - Modifies:
|     o all input variables to hold user input
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or versio number
|     o 2 if had an error
\-------------------------------------------------------*/
signed char
input_mkMiruTbl(
   int numArgsSI,
   char *argAryStr[],
   signed char **refFileStr,
   signed char **primFileStr,
   signed char **tblFileStr,
   float *minPercScoreF
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get user input
   '   o fun04 sec04:
   '     - return success or errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mkMiruTbl(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - input for running mkMiruTbl
   ^   o fun03 sec03 sub02:
   ^     - help message checks
   ^   o fun03 sec03 sub03:
   ^     - version number checks
   ^   o fun03 sec03 sub04:
   ^     - invalid input
   ^   o fun03 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - input for running mkMiruTbl
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: Get the user input*/
      if(
         ! eql_charCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*If: reference file*/
         ++siArg;
         *refFileStr = (signed char *) argAryStr[siArg]; 
      } /*If: reference file*/

      else if(
         ! eql_charCp(
            (signed char *) "-prim",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: is primer file*/
         ++siArg;
         *primFileStr = (signed char *) argAryStr[siArg]; 
      } /*Else If: is primer file*/

      else if(
         ! eql_charCp(
            (signed char *) "-miru-tbl",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: is miru table to add coordinates to*/
         ++siArg;
         *tblFileStr = (signed char *) argAryStr[siArg]; 
      } /*Else If: is miru table to add coordinates to*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-perc-score",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: is minimum score to keep alignment*/
         ++siArg;
         *minPercScoreF = atof(argAryStr[siArg]);
      } /*Else If: is minimum score to keep alignment*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - help message checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - version number checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
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
            "%s is not recognized%s",
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
   } /*Loop: Get the user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return success or errors
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
} /*input_mkMiruTbl*/

/*-------------------------------------------------------\
| Main:
|   - adds the primer coordinates to input MIRU table
| Input:
| Output:
|   - Prints the MIRU table with the primer coordiantes
|     added in
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - adds the primer coordinates to input MIRU table
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, and check input
   '   o main sec03:
   '     - read in refernece and primer sequences
   '   o main sec04:
   '     - find primer coordinates
   '   o main sec05:
   '     - read in table and add primer coordinates
   '   o main sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *refFileStr = 0; /*path to reference file*/
   signed char *primFileStr = 0;/*path to primers*/
   signed char *tblFileStr = 0; /*path to miru table*/
   float minPercScoreF = def_minPercScore_mkMiruTbl;

   struct seqST refSTStack;
   signed char errSC = 0;

   struct seqST *seqSTHeapAry = 0;
   signed int numPrimSI = 0;
   signed int siPrim = 0;

   signed long scoreSL = 0;
   float percScoreF = 0;
   signed long startAlnSL = 0;
   signed long endAlnSL = 0;
   signed long discardSL = 0;
   signed long ignoreSL = 0;

   struct alnSet alnSetSTStack;
   signed char *buffHeapStr = 0;
      /*buffer for reading files*/
   signed char *posBuffStr = 0;
      /*position in buffHeapStr*/
   signed char *pBuffStr = 0;
      /*position to print in buffHeap*/
   signed long lenBuffSL = 0;
   signed long bytesSL = 0;
   signed long numBytesSL = 0;
   signed char revCmpBl = 0;
      /*try reverse complement primer*/

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialize and get input
   ^   o main sec02 sub02:
   ^     - check reference file exits
   ^   o main sec02 sub03:
   ^     - check primer file exits
   ^   o main sec02 sub04:
   ^     - check MIRU table file exits
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize and get input
   \*****************************************************/

   init_alnSet(&alnSetSTStack);
   init_seqST(&refSTStack);

   errSC =
      input_mkMiruTbl(
         numArgsSI,
         argAryStr,
         &refFileStr,
         &primFileStr,
         &tblFileStr,
         &minPercScoreF
      );

   if(errSC)
   { /*If: error*/
      --errSC; /*convert help/version print to no error*/
      goto cleanUp_main_sec06_sub03;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if reference file exits
   \*****************************************************/

   inFILE = fopen((char *) refFileStr, "r");
   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -ref %s%s",
         refFileStr,
         str_endLine
      );

      goto err_main_sec06_sub02;
   } /*If: could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - check if primer file exits
   \*****************************************************/

   inFILE = fopen((char *) primFileStr, "r");
   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -prim %s%s",
         primFileStr,
         str_endLine
      );

      goto err_main_sec06_sub02;
   } /*If: could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if MIRU table file exits
   \*****************************************************/

   inFILE = fopen((char *) tblFileStr, "r");

   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -miru-tbl %s%s",
         tblFileStr,
         str_endLine
      );

      goto err_main_sec06_sub02;
   } /*If: could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in refernece and primer sequences
   ^   o main sec03 sub01:
   ^     - read in reference sequence
   ^   o main sec03 sub02:
   ^     - read primer sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - read in reference sequence
   \*****************************************************/

   inFILE = fopen( (char *) refFileStr, "r");
   errSC = getFa_seqST(inFILE, &refSTStack);

   if(errSC == def_EOF_seqST) ;
   else if(errSC)
   { /*Else If: error*/
      if(errSC & def_fileErr_seqST)
      { /*If: invalid fasta file*/
         fprintf(
            stderr,
            "-ref %s is not a fasta file%s",
            refFileStr,
            str_endLine
         );
      } /*If: invalid fasta file*/

      else
      { /*Else: memory error*/
         fprintf(
            stderr,
            "memory error while reading -ref %s%s",
            refFileStr,
            str_endLine
         );
      } /*Else: memory error*/

      goto err_main_sec06_sub02;
   } /*Else If: error*/

   seqToIndex_alnSet(refSTStack.seqStr); /*for alingment*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - read in primer sequences
   *   o main sec03 sub02 cat01:
   *     - find number of primers in primer fasta
   *   o main sec03 sub02 cat02:
   *     - read in primers
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub02 Cat01:
   +   - find number of primers in primer fasta
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   inFILE = fopen((char *) primFileStr, "r");
   seqSTHeapAry = malloc(sizeof(seqST));

   if(! seqSTHeapAry)
      fprintf(stderr, "Memory error%s", str_endLine);
   init_seqST(seqSTHeapAry);
   errSC = getFa_seqST(inFILE, seqSTHeapAry);

   while(! errSC)
   { /*Loop: find number of primers in*/
      ++numPrimSI;
      errSC = getFa_seqST(inFILE, seqSTHeapAry);
   } /*Loop: find number of primers in*/

   if(errSC != def_EOF_seqST)
   { /*If: error*/
      if(errSC & def_fileErr_seqST)
      { /*If: invalid fasta file*/
         fprintf(
            stderr,
            "entry %i in -prim %s is not a fasta entry%s",
            numPrimSI,
            primFileStr,
            str_endLine
         );
      } /*If: invalid fasta file*/

      else
      { /*Else: memory error*/
         fprintf(
            stderr,
            "memory error on entry %i in -prim %s%s",
            numPrimSI,
            primFileStr,
            str_endLine
         );
      } /*Else: memory error*/

      goto err_main_sec06_sub02;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub02 Cat02:
   +   - read in primers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fseek(inFILE, 0, SEEK_SET);
   freeHeap_seqST(seqSTHeapAry);
   seqSTHeapAry = 0;

   seqSTHeapAry =
      malloc(numPrimSI * sizeof(struct seqST));

   if(! seqSTHeapAry)
      fprintf(stderr, "Memory error%s", str_endLine);

   for(siPrim = 0; siPrim < numPrimSI; ++siPrim)
   { /*Loop: get primer sequences*/
      init_seqST(&seqSTHeapAry[siPrim]);

      errSC = 
        getFa_seqST(inFILE, &seqSTHeapAry[siPrim]);

      if(errSC > 1)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "memory error on entry %i in -prim %s%s",
            siPrim,
            primFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: I had an memory error*/
   } /*Loop: get primer sequences*/

   fclose(inFILE);
   inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - find primer coordinates
   ^   o main sec04 sub01:
   ^     - start loop and align the foward primer
   ^   o main sec04 sub02:
   ^     - align reverse primer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - start loop and align the foward primer
   \*****************************************************/

   for(siPrim = 0; siPrim + 1 < numPrimSI; siPrim += 2)
   { /*Loop: align primer sequences*/
      alnFor_main_sec04_sub01:;

      seqToIndex_alnSet(seqSTHeapAry[siPrim].seqStr);

      scoreSL =
         memwater(
            &refSTStack,          /*reference sequence*/
            &seqSTHeapAry[siPrim],/*primer sequence*/
            &discardSL,      /*position of ref on primer*/
            &ignoreSL,       /*position of ref on primer*/
            &startAlnSL,
            &endAlnSL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreSL == 0)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "memory error aligning primer %i: -prim %s%s",
            siPrim,
            primFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: I had an memory error*/

      percScoreF =
         (float)
         maxScore_alnDefs(seqSTHeapAry[siPrim].seqLenSL);

      percScoreF = ((float) scoreSL) / percScoreF;

      fprintf(
        stderr,
        "primer number %i; forward\t%s\t%% match: %.2f%s",
        siPrim >> 1,
        seqSTHeapAry[siPrim].idStr,
        percScoreF * 100,
        str_endLine
      );

      if(percScoreF < minPercScoreF)
      { /*If: could not map this primer*/
         if(! revCmpBl)
         { /*If: have not tried reverse complementing*/
            indexToSeq_alnSet(
               seqSTHeapAry[siPrim].seqStr
            );

            revComp_seqST(&seqSTHeapAry[siPrim]); 
            revCmpBl = 1;
            goto alnFor_main_sec04_sub01;
         } /*If: have not tried reverse complementing*/
             
         fprintf(
            stderr,
            "Unable to align primer %i, foward: %s%s",
            siPrim >> 1,
            seqSTHeapAry[siPrim].idStr,
            str_endLine
         );

         fprintf(
            stderr,
            " in -prim %s%s",
            primFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: could not map this primer*/

      /*add in the primer coordinates*/
      seqSTHeapAry[siPrim].offsetSL = startAlnSL;
      seqSTHeapAry[siPrim].endAlnSL = endAlnSL;
      revCmpBl = 0;

      /**************************************************\
      * Main Sec04 Sub02:
      *   - align reverse primer
      \**************************************************/

      alnRev_main_sec04_sub02:;

      revComp_seqST(&seqSTHeapAry[siPrim + 1]); 
      seqToIndex_alnSet(seqSTHeapAry[siPrim + 1].seqStr);

      scoreSL =
         memwater(
            &refSTStack,            /*reference sequence*/
            &seqSTHeapAry[siPrim+1],/*primer sequence*/
            &discardSL,      /*position of ref on primer*/
            &ignoreSL,       /*position of ref on primer*/
            &startAlnSL,
            &endAlnSL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreSL == 0)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error aligning primer %i: -prim %s%s",
            siPrim + 1,
            primFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: memory error*/

      percScoreF=
         (float)
          maxScore_alnDefs(
             seqSTHeapAry[siPrim + 1].seqLenSL
         );

      percScoreF = ((float) scoreSL) / percScoreF;

      fprintf(
        stderr,
        "primer number %i; reverse\t%s\t%% match: %.2f%s",
        siPrim >> 1,
        seqSTHeapAry[siPrim + 1].idStr + 1,
        percScoreF * 100,
        str_endLine
      );

      if(percScoreF < minPercScoreF)
      { /*If: could not map this primer*/

         if(! revCmpBl)
         { /*If: have not tried reverse complementing*/
            indexToSeq_alnSet(
               seqSTHeapAry[siPrim + 1].seqStr
            ); /*reverse complement after goto*/

            revCmpBl = 1;
            goto alnRev_main_sec04_sub02;
         } /*If: have not tried reverse complementing*/

         fprintf(
            stderr,
            "unable to align primer %i, reverse: %s%s",
            siPrim + 1,
            seqSTHeapAry[siPrim + 1].idStr,
            str_endLine
         );

         fprintf(
            stderr,
            " in -prim %s%s",
            primFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: could not map this primer*/

      /*primer coordinates*/
      seqSTHeapAry[siPrim + 1].offsetSL = startAlnSL;
      seqSTHeapAry[siPrim + 1].endAlnSL = endAlnSL;
      revCmpBl = 0;
   } /*Loop: align primer sequences*/

   freeStack_seqST(&refSTStack);
   freeStack_alnSet(&alnSetSTStack);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - read in table and add primer coordinates
   ^   o main sec05 sub01:
   ^     - set up buffer memory & open miru table
   ^   o main sec05 sub02:
   ^     - see if can get first line in one read
   ^   o main sec05 sub03:
   ^     - make sure have entire frist line
   ^   o main sec05 sub04:
   ^     - add primer coordinates to primer names
   ^   o main sec05 sub05:
   ^     - print rest of the file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - set up buffer memory and open miru table
   \*****************************************************/

   inFILE = fopen( (char *) tblFileStr, "r");

   numBytesSL =
      getFullLine_fileFun(
         inFILE,
         &buffHeapStr,
         &lenBuffSL,
         &bytesSL,
         0
      );

   if(numBytesSL < 0)
      goto memErr_main_sec06;
   else if(! numBytesSL)
      goto emptyTbl_main_sec06;
   
   /*****************************************************\
   * Main Sec05 Sub04:
   *   - add primer coordinates to primer names
   *   o main sec05 sub02 Cat01:
   *     - get past and print out the first column
   *   o main sec05 sub02 Cat02:
   *     - print out the header for each primer
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat01:
   +   - Get past and print out the first column
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   pBuffStr = buffHeapStr;
   posBuffStr = buffHeapStr;

   while(*posBuffStr++ > 32) ;

   if(*(posBuffStr - 1) == '\t') ;
   else if(*(posBuffStr - 1) == ' ') ;
   else
   { /*Else: file has only 1 column*/
      fprintf(
         stderr,
         "-miru-tbl %s\n has only one column\n",
         tblFileStr
      );

      goto err_main_sec06_sub02;
   } /*Else: file does not have an new line*/

   *(posBuffStr - 1) = '\0';
   fprintf(stdout, "%s\t", pBuffStr);
      /*print frist columns header*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat02:
   +   - print out the header for each primer
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*I am going to assume the user input the primers in
   `   order. It is a bit dumb, but it works
   */
   siPrim = 0;

   while(*posBuffStr != '\n')
   { /*Loop: print header*/
      pBuffStr = posBuffStr;

      while(*posBuffStr++ > 32) ;

      if(*(posBuffStr - 1) == '\n')
         break;

      *(posBuffStr - 1) = '\0';

      if(
           seqSTHeapAry[siPrim].offsetSL
         < seqSTHeapAry[siPrim + 1].endAlnSL
      ){/*If: primers order is forward and reverse*/
         fprintf(      
            stdout,
            "%s.%lu.%lu.%lu.%lu\t",
            pBuffStr,
            seqSTHeapAry[siPrim].offsetSL,
            seqSTHeapAry[siPrim].seqLenSL,
            seqSTHeapAry[siPrim + 1].endAlnSL,
            seqSTHeapAry[siPrim + 1].seqLenSL
         );
      }/*If: primers order is forward and reverse*/

      else
      {/*Else: primers are backwards*/
         fprintf(      
            stdout,
            "%s.%lu.%lu.%lu.%lu\t",
            pBuffStr,
            seqSTHeapAry[siPrim + 1].offsetSL,
            seqSTHeapAry[siPrim + 1].seqLenSL,
            seqSTHeapAry[siPrim].endAlnSL,
            seqSTHeapAry[siPrim].seqLenSL
         );
      }/*Else: primers are backwards*/

      siPrim += 2;
   } /*Loop: print header*/

   *(posBuffStr - 1) = '\0';

   if(
        seqSTHeapAry[siPrim].offsetSL
      < seqSTHeapAry[siPrim + 1].endAlnSL
   ){/*If: primers orderr is forward and reverse*/
      fprintf(      
         stdout,
         "%s.%lu.%lu.%lu.%lu\n%s",
         pBuffStr,
         seqSTHeapAry[siPrim].offsetSL,
         seqSTHeapAry[siPrim].seqLenSL,
         seqSTHeapAry[siPrim + 1].endAlnSL,
         seqSTHeapAry[siPrim + 1].seqLenSL,
         posBuffStr
      );
   }/*If: primers orderr is forward and reverse*/

   else
   {/*Else: primers are backwards*/
      fprintf(      
         stdout,
         "%s.%lu.%lu.%lu.%lu\n%s",
         pBuffStr,
         seqSTHeapAry[siPrim + 1].offsetSL,
         seqSTHeapAry[siPrim + 1].seqLenSL,
         seqSTHeapAry[siPrim].endAlnSL,
         seqSTHeapAry[siPrim].seqLenSL,
         posBuffStr
      );
   }/*Else: primers are backwards*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - print out rest of file
   \*****************************************************/

   numBytesSL = 
     fread(buffHeapStr, sizeof(char), lenBuffSL, inFILE);

   while(numBytesSL)
   { /*Loop: read in file*/
      buffHeapStr[numBytesSL] = '\0';
      fprintf(stdout, "%s", buffHeapStr);
      numBytesSL = 
        fread(buffHeapStr,sizeof(char),lenBuffSL,inFILE);
   } /*Loop: read in file*/

   free(buffHeapStr);
   buffHeapStr = 0;

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   freeHeapAry_seqST(seqSTHeapAry, numPrimSI);
   seqSTHeapAry = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - clean up
   ^   o main sec06 sub01:
   ^     - no error clean up
   ^   o main sec06 sub02:
   ^     - error clean up
   ^   o main sec06 sub03:
   ^     - general clean up (error or no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec06_sub03;

   err_main_sec06_sub02:;
      errSC = 1;
      goto cleanUp_main_sec06_sub03;

   memErr_main_sec06:;
      errSC = 2;
      fprintf(stderr, "memory error%s", str_endLine);
      goto cleanUp_main_sec06_sub03;

   emptyTbl_main_sec06:;
      errSC = 3;
      fprintf(
         stderr,
         "nothing in -miru-tbl%s",
         str_endLine
      );
      goto cleanUp_main_sec06_sub03;

   cleanUp_main_sec06_sub03:;
      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(seqSTHeapAry)
         freeHeapAry_seqST(seqSTHeapAry, numPrimSI);
      seqSTHeapAry = 0;

      freeStack_seqST(&refSTStack);
      freeStack_alnSet(&alnSetSTStack);

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else
         fclose(inFILE);
      inFILE = 0;

      return errSC;
} /*main*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
