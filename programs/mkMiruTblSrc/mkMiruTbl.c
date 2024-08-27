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
'     - licensing for this code (public domain / mit)
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
#include "../genLib/seqST.h"
#include "../genAln/memwater.h"
#include "../genAln/alnSet.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .c  #include "../genLib/base10str.h"
!   o .h  #include "../genLib/genMath.h"
!   o .c  #include "../genAln/indexToCoord.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*version numbers*/
#define def_year_mkMiruTbl 2024
#define def_month_mkMiruTbl 8
#define def_day_mkMiruTbl 8

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
      "mkMiruTbl version: %i-%02i-%02i\n",
      def_year_mkMiruTbl,
      def_month_mkMiruTbl,
      def_day_mkMiruTbl
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
      "table.tsv\n"
   );

   fprintf(
      outFILE,
      "  - Adds in the primers coordinates to an table of"
   );

   fprintf(
      outFILE,
      "\n    MIRU lineages\n"
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
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - reference paramater
   \*****************************************************/

   fprintf(
      outFILE,
      "  -ref: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Fasta with reference to map primers to\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - primer parameter
   \*****************************************************/

   fprintf(
      outFILE,
      "  -prim: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Fasta with primers to get coordinates of\n"
   );

   fprintf(
      outFILE,
      "    o The reverse primer must always come right\n"
   );

   fprintf(
      outFILE,
      "      after the forward primer\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - MIRU table parameter
   \*****************************************************/

   fprintf(
      outFILE,
      "  -miru-tbl: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Tsv file with the MIRU table to add"
   );

   fprintf(outFILE, " coordinates to\n");

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - min pecent of max score paramater
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-perc-score: [%.2f]\n",
      def_minPercScore_mkMiruTbl
   );

   fprintf(
      outFILE,
      "    o Minimum percent of max score needed to keep"
   );

   fprintf(outFILE, " primer\n");

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help message opiton and version number
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: Print this help message\n"
   );

   fprintf(
      outFILE,
      "  -v: Print the version number\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:\n"
   );

   fprintf(
      outFILE,
      "  - Prints the MIRU table with the added primer\n"
   );
   fprintf(
      outFILE,
      "    coordinates (in header) to stdout\n"
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

   sint siArg = 1;
   schar errSC = 0;

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
            (schar *) "-ref",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: reference file*/
         ++siArg;
         *refFileStr = (schar *) argAryStr[siArg]; 
      } /*If: reference file*/

      else if(
         ! eql_charCp(
            (schar *) "-prim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is primer file*/
         ++siArg;
         *primFileStr = (schar *) argAryStr[siArg]; 
      } /*Else If: is primer file*/

      else if(
         ! eql_charCp(
            (schar *) "-miru-tbl",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is miru table to add coordinates to*/
         ++siArg;
         *tblFileStr = (schar *) argAryStr[siArg]; 
      } /*Else If: is miru table to add coordinates to*/

      else if(
         ! eql_charCp(
            (schar *) "-min-perc-score",
            (schar *) argAryStr[siArg],
            (schar) '\0'
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
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mkMiruTbl(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
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
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mkMiruTbl(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
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
            "%s is not recognized\n",
            argAryStr[siArg]
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

   schar *refFileStr = 0;  /*path to reference file*/
   schar *primFileStr = 0; /*path to primers*/
   schar *tblFileStr = 0;  /*path to miru table*/
   float minPercScoreF = def_minPercScore_mkMiruTbl;

   struct seqST refSTStack;
   schar errSC = 0;

   struct seqST *seqSTHeapAry = 0;
   sint numPrimSI = 0;
   sint siPrim = 0;

   slong scoreSL = 0;
   float percScoreF = 0;
   ulong startAlnUL = 0;
   ulong endAlnUL = 0;
   ulong discardUL = 0;
   ulong ignoreUL = 0;

   struct alnSet alnSetSTStack;
   schar *buffHeapStr = 0;/*buffer for reading files*/
   schar *posBuffStr = 0; /*position in buffHeapStr*/
   schar *pBuffStr = 0;  /*position to print in buffHeap*/
   sint lenBuffSI = 0;
   ulong oldByteCountUL = 0;
   ulong numBytesUL = 0;
   schar revCmpBl = 0; /*try reverse complement primer*/

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

   inFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -ref %s\n",
         refFileStr
      );

      goto err_main_sec06_sub02;
   } /*If: could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - check if primer file exits
   \*****************************************************/

   inFILE =
      fopen(
         (char *) primFileStr,
         "r"
      );

   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -prim %s\n",
         primFileStr
      );

      goto err_main_sec06_sub02;
   } /*If: could not open the primer file*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if MIRU table file exits
   \*****************************************************/

   inFILE =
       fopen(
          (char *) tblFileStr,
          "r"
       );

   if(! inFILE)
   { /*If: could not open the primer file*/
      fprintf(
         stderr,
         "could not open -miru-tbl %s\n",
         tblFileStr
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

   inFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   errSC = 
     getFaSeq_seqST(
        inFILE,
        &refSTStack
     );

   if(
          errSC
       && errSC != def_EOF_seqST
   ){ /*If: error*/
      if(errSC & def_fileErr_seqST)
      { /*If: invalid fasta file*/
         fprintf(
            stderr,
            "-ref %s is not a fasta file\n",
            refFileStr
         );
      } /*If: invalid fasta file*/

      else
      { /*Else: memory error*/
         fprintf(
            stderr,
            "memory error while reading -ref %s\n",
            refFileStr
         );
      } /*Else: memory error*/

      goto err_main_sec06_sub02;
   } /*If: error*/

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

   inFILE =
      fopen(
         (char *) primFileStr,
         "r"
      );

   seqSTHeapAry = malloc(sizeof(seqST));

   if(! seqSTHeapAry)
   { /*If: I had memory error*/
      fprintf(
         stderr,
         "Memory error\n"
      );
   } /*If: I had memory error*/
   
   init_seqST(seqSTHeapAry);

   errSC = 
     getFaSeq_seqST(
        inFILE,
        seqSTHeapAry
     );

   while(! errSC)
   { /*Loop: find number of primers in*/
      ++numPrimSI;

      errSC = 
        getFaSeq_seqST(
           inFILE,
           seqSTHeapAry
        );
   } /*Loop: find number of primers in*/

   if(errSC != def_EOF_seqST)
   { /*If: error*/
      if(errSC & def_fileErr_seqST)
      { /*If: invalid fasta file*/
         fprintf(
            stderr,
            "entry %i in -prim %s is not a fasta entry\n",
            numPrimSI,
            primFileStr
         );
      } /*If: invalid fasta file*/

      else
      { /*Else: memory error*/
         fprintf(
            stderr,
            "memory error on entry %i in -prim %s\n",
            numPrimSI,
            primFileStr
         );
      } /*Else: memory error*/

      goto err_main_sec06_sub02;
   } /*If: error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub02 Cat02:
   +   - read in primers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fseek(
      inFILE,
      0,
      SEEK_SET
   );

   freeHeap_seqST(seqSTHeapAry);
   seqSTHeapAry = 0;

   seqSTHeapAry =
      malloc(numPrimSI * sizeof(struct seqST));

   if(! seqSTHeapAry)
   { /*If: I had memory error*/
      fprintf(
         stderr,
         "Memory error\n"
      );
   } /*If: I had memory error*/

   for(
      siPrim = 0;
      siPrim < numPrimSI;
      ++siPrim
   ){ /*Loop: get primer sequences*/
      init_seqST(&seqSTHeapAry[siPrim]);

      errSC = 
        getFaSeq_seqST(
           inFILE,
           &seqSTHeapAry[siPrim]
        );

      if(errSC > 1)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "memory error on entry %i in -prim %s\n",
            siPrim,
            primFileStr
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

   for(
      siPrim = 0;
      siPrim + 1 < numPrimSI;
      siPrim += 2
   ){ /*Loop: align primer sequences*/
      alnFor_main_sec04_sub01:;

      seqToIndex_alnSet(seqSTHeapAry[siPrim].seqStr);

      scoreSL =
         memwater(
            &refSTStack,          /*reference sequence*/
            &seqSTHeapAry[siPrim],/*primer sequence*/
            &discardUL,      /*position of ref on primer*/
            &ignoreUL,       /*position of ref on primer*/
            &startAlnUL,
            &endAlnUL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreSL == 0)
      { /*If: I had an memory error*/
         fprintf(
            stderr,
            "memory error aligning primer %i: -prim %s\n",
            siPrim,
            primFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: I had an memory error*/

      percScoreF =
         (float)
         maxScore_alnDefs(seqSTHeapAry[siPrim].lenSeqUL);

      percScoreF = ((float) scoreSL) / percScoreF;

      fprintf(
        stderr,
        "primer number %i; forward\t%s\t%% match: %.2f\n",
        siPrim >> 1,
        seqSTHeapAry[siPrim].idStr + 1,
        percScoreF * 100
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
            "Unable to align primer %i, foward: %s\n",
            siPrim >> 1,
            seqSTHeapAry[siPrim].idStr
         );

         fprintf(
            stderr,
            " in -prim %s\n",
            primFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: could not map this primer*/

      /*add in the primer coordinates*/
      seqSTHeapAry[siPrim].offsetUL = startAlnUL;
      seqSTHeapAry[siPrim].endAlnUL = endAlnUL;
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
            &discardUL,      /*position of ref on primer*/
            &ignoreUL,       /*position of ref on primer*/
            &startAlnUL,
            &endAlnUL,
            &alnSetSTStack
         ); /*Aling the forward primer*/

      if(scoreSL == 0)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error aligning primer %i: -prim %s\n",
            siPrim + 1,
            primFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: memory error*/

      percScoreF=
         (float)
          maxScore_alnDefs(
             seqSTHeapAry[siPrim + 1].lenSeqUL
         );

      percScoreF = ((float) scoreSL) / percScoreF;

      fprintf(
        stderr,
        "primer number %i; reverse\t%s\t%% match: %.2f\n",
        siPrim >> 1,
        seqSTHeapAry[siPrim + 1].idStr + 1,
        percScoreF * 100
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
            "unable to align primer %i, reverse: %s\n",
            siPrim + 1,
            seqSTHeapAry[siPrim + 1].idStr
         );

         fprintf(
            stderr,
            " in -prim %s\n",
            primFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: could not map this primer*/

      /*primer coordinates*/
      seqSTHeapAry[siPrim + 1].offsetUL = startAlnUL;
      seqSTHeapAry[siPrim + 1].endAlnUL = endAlnUL;
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

   lenBuffSI = 1 << 10; /*~ 2000 characters*/

   buffHeapStr = malloc((lenBuffSI + 9) * sizeof(char));

   if(! buffHeapStr)
   { /*If: I had an memory error*/
      fprintf(
         stderr,
         "Memory error after primer step\n"
       );

      goto err_main_sec06_sub02;
   } /*If: I had an memory error*/

   inFILE =
      fopen(
         (char *) tblFileStr,
         "r"
       );

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - see if can get first line in one read
   \*****************************************************/

   numBytesUL =
      fread(
         (char *) buffHeapStr,
         sizeof(char),
         lenBuffSI,
         inFILE
      ); /*read in frist line*/

   if(! numBytesUL)
   { /*If: file does not have an new line*/
      fprintf(
         stderr,
         "-miru-tbl %s\n has no lines\n",
         tblFileStr
      );

      goto err_main_sec06_sub02;
   } /*If: file does not have an new line*/

   buffHeapStr[numBytesUL] = '\0';
   posBuffStr = buffHeapStr;
   posBuffStr += endLine_ulCp(buffHeapStr);

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - make sure have entire frist line
   \*****************************************************/

   while(*posBuffStr != '\n')
   { /*Loop: read in first line*/
      lenBuffSI <<= 1;

      posBuffStr =
         realloc(
            buffHeapStr,
            (lenBuffSI + 1) * sizeof(schar)
         ); /*resize buffer*/

      if(! posBuffStr)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error reading -miru-tbl %s\n",
            tblFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: memory error*/

      buffHeapStr = posBuffStr;
      posBuffStr += numBytesUL;

      oldByteCountUL = numBytesUL;

      numBytesUL +=
         fread(
            (char *) posBuffStr,
            sizeof(char),
            (lenBuffSI >> 1),
            inFILE
         );

      buffHeapStr[numBytesUL] = '\0';

      if(oldByteCountUL == numBytesUL)
      { /*If: file does not have an new line*/
         fprintf(
            stderr,
            "-miru-tbl %s\n has no newline\n",
            tblFileStr
         );

         goto err_main_sec06_sub02;
      } /*If: file does not have an new line*/

      buffHeapStr[numBytesUL] = '\0';
      posBuffStr = buffHeapStr;
      posBuffStr += endLine_ulCp(buffHeapStr);
   } /*Loop: Read in the first line*/

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

   fprintf(
      stdout,
      "%s\t",
      pBuffStr
   ); /*print frist columns header*/

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
           seqSTHeapAry[siPrim].offsetUL
         < seqSTHeapAry[siPrim + 1].endAlnUL
      ){/*If: primers order is forward and reverse*/
         fprintf(      
            stdout,
            "%s.%lu.%lu.%lu.%lu\t",
            pBuffStr,
            seqSTHeapAry[siPrim].offsetUL,
            seqSTHeapAry[siPrim].lenSeqUL,
            seqSTHeapAry[siPrim + 1].endAlnUL,
            seqSTHeapAry[siPrim + 1].lenSeqUL
         );
      }/*If: primers order is forward and reverse*/

      else
      {/*Else: primers are backwards*/
         fprintf(      
            stdout,
            "%s.%lu.%lu.%lu.%lu\t",
            pBuffStr,
            seqSTHeapAry[siPrim + 1].offsetUL,
            seqSTHeapAry[siPrim + 1].lenSeqUL,
            seqSTHeapAry[siPrim].endAlnUL,
            seqSTHeapAry[siPrim].lenSeqUL
         );
      }/*Else: primers are backwards*/

      siPrim += 2;
   } /*Loop: print header*/

   *(posBuffStr - 1) = '\0';

   if(
        seqSTHeapAry[siPrim].offsetUL
      < seqSTHeapAry[siPrim + 1].endAlnUL
   ){/*If: primers orderr is forward and reverse*/
      fprintf(      
         stdout,
         "%s.%lu.%lu.%lu.%lu\n%s",
         pBuffStr,
         seqSTHeapAry[siPrim].offsetUL,
         seqSTHeapAry[siPrim].lenSeqUL,
         seqSTHeapAry[siPrim + 1].endAlnUL,
         seqSTHeapAry[siPrim + 1].lenSeqUL,
         posBuffStr
      );
   }/*If: primers orderr is forward and reverse*/

   else
   {/*Else: primers are backwards*/
      fprintf(      
         stdout,
         "%s.%lu.%lu.%lu.%lu\n%s",
         pBuffStr,
         seqSTHeapAry[siPrim + 1].offsetUL,
         seqSTHeapAry[siPrim + 1].lenSeqUL,
         seqSTHeapAry[siPrim].endAlnUL,
         seqSTHeapAry[siPrim].lenSeqUL,
         posBuffStr
      );
   }/*Else: primers are backwards*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - print out rest of file
   \*****************************************************/

   numBytesUL = 
     fread(
        buffHeapStr,
        sizeof(char),
        lenBuffSI,
        inFILE
     );

   while(numBytesUL)
   { /*Loop: read in file*/
      buffHeapStr[numBytesUL] = '\0';

      fprintf(
         stdout,
         "%s",
         buffHeapStr
      );

      numBytesUL = 
        fread(
           buffHeapStr,
           sizeof(char),
           lenBuffSI,
           inFILE
        );
   } /*Loop: read in file*/

   free(buffHeapStr);
   buffHeapStr = 0;

   if(inFILE != stdin)
      fclose(inFILE);

   inFILE = 0;

   freeHeapAry_seqST(
      seqSTHeapAry,
      numPrimSI
   );

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

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - error clean up
   \*****************************************************/

   err_main_sec06_sub02:;
   errSC = 1;
   goto cleanUp_main_sec06_sub03;

   /*****************************************************\
   * Main Sec06 Sub03:
   *   - general clean up (error or no error)
   \*****************************************************/

   cleanUp_main_sec06_sub03:;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

   if(seqSTHeapAry)
      freeHeapAry_seqST(
         seqSTHeapAry,
         numPrimSI
      );

   seqSTHeapAry = 0;

   freeStack_seqST(&refSTStack);
   freeStack_alnSet(&alnSetSTStack);


   if(
         inFILE
      && inFILE != stdin
      && inFILE != stdout
   ) fclose(inFILE);

   inFILE = 0;

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
