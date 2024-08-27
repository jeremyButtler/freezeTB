/*#######################################################\
# Name: samToAlnMain
#   - driver function to convert sam file to aln
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and defined variables
'   o fun01: pversion_samToAlnMain
'     - prints versoin number for samToAln
'   o fun02: phelp_samToAlnMain
'     - prints help message for samToAln
'   o fun03: checkIfHelp_samToAlnMain
'     - checks if input was the help message
'   o fun04: checkIfVersion_samToAlnMain
'     - checks if input was the version number request
'   o fun05: input_samToAlnMain
'     - gets user input
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/seqST.h"
#include "../genLib/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/samToAln.h"

/*no .c files*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependices
!   - .c  #include "../genLib/numToStr.h"
!   - .h  #include "../genAln/ntTo5Big.h"
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_year_samToAlnMain 2024
#define def_month_samToAlnMain 7
#define def_day_samToAlnMain 31

#define def_pmask_samToAlnMain 0    /*print masked bases*/
#define def_ppos_samToAlnMain 1     /*print positions*/
#define def_lineWrap_samToAlnMain 58 /*line wrap*/

/*-------------------------------------------------------\
| Fun01: pversion_samToAlnMain
|   - prints versoin number for samToAln
| Input:
|   - outFILE:
|     o file to print version  number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_samToAlnMain(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "samToAln version: %i-%02i-%02i\n",
      def_year_samToAlnMain,
      def_month_samToAlnMain,
      def_day_samToAlnMain
   );
} /*pversion_samToAlnMain*/

/*-------------------------------------------------------\
| Fun02: phelp_samToAlnMain
|   - prints help message for samToAln
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_samToAlnMain(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for samToAln
   '   o fun02 sec01:
   '     - print usage block
   '   o fun02 sec02:
   '     - print input block
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "samToAln -ref ref.fa -sam reads.sam [options...]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - converts sam file to expand cigar aligment\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input block
   ^   o fun02 sec02 sub01:
   ^     - input block
   ^   o fun02 sec02 sub02:
   ^     - file input arguments
   ^   o fun02 sec02 sub03:
   ^     - output arguments
   ^   o fun02 sec02 sub04:
   ^     - print help/version number commands
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file input arguments
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref ref.fa: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta with reference sequence used for\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      mapping\n"
   );


   /*sam file*/
   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required; stdin]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with mapped reads\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-sam -\" for stdin input\n"
   );


   /*match matrix*/
   fprintf(
      (FILE *) outFILE,
      "  -match matrix.txt: [Optional]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o match matrix for alignment\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - output arguments
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -wrap %i: [Optional]\n",
      def_lineWrap_samToAlnMain
   );

   fprintf(
      (FILE *) outFILE,
      "    o maximum line length (0 is no limit)\n"
   );


   /*keep/remove masking*/
   if(def_pmask_samToAlnMain)
      fprintf(
         (FILE *) outFILE,
         "  -mask: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -mask: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "    o print unaligned reference and query\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-mask\"\n"
   );

   /*print sequence positions*/
   if(def_ppos_samToAlnMain)
      fprintf(
         (FILE *) outFILE,
         "  -pos: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -pos: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "    o print out line start and end positions\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-pos\"\n"
   );


   /*output file*/
   fprintf(
      (FILE *) outFILE,
      "  -out out.file: [Optional; stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to save alignmet to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout\n"
   );


   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - print help/version number commands
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message to stdout and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number to stdout and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -p-match: print default match matrix to stdout\n"
   );
   fprintf(
      (FILE *) outFILE,
      "    o only used when have \'M\' in cigar\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints alignment to stdout or -out file.aln\n"
   );
} /*phelp_samToAlnMain*/

/*-------------------------------------------------------\
| Fun03: checkIfHelp_samToAlnMain
|   - checks if input was the help message
| Input:
|   - argStr:
|     o c-string with argument to check for help message
| Output:
|   - Returns:
|     o 1 if is the help message
|     o 0 if not the help message
\-------------------------------------------------------*/
signed char
checkIfHelp_samToAlnMain(
   schar *argStr
){
   if(! eql_charCp((schar *) "-h", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "--h", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "help", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "-help", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "--help", argStr, '\0'))
      return 1;

   return 0;
} /*checkIfHelp_samToAlnMain*/

/*-------------------------------------------------------\
| Fun04: checkIfVersion_samToAlnMain
|   - checks if input was the version number request
| Input:
|   - argStr:
|     o c-string with argument to check for version number
| Output:
|   - Returns:
|     o 1 if is version number
|     o 0 if not version number
\-------------------------------------------------------*/
signed char
checkIfVersion_samToAlnMain(
   schar *argStr
){
   if(! eql_charCp((schar *) "-v", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "--v", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "version", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "-version", argStr, '\0'))
      return 1;

   if(! eql_charCp((schar *) "--version", argStr, '\0'))
      return 1;

   return 0;
} /*checkIfVersion_samToAlnMain*/

/*-------------------------------------------------------\
| Fun05: input_samToAlnMain
|   - gets user input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStrPtr:
|     o pointer to c-string to point to the sam file
|   - refFileStrPtr:
|     o pointer to c-string to point to refernece file
|   - outFileStrPtr:
|     o pointer to c-string to point to output file
|   - alnSetSTPtr:
|     o pointer to alnSet structure to hold input
| Output:
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help, version number, or match matrix
|     o 2 for errors
\-------------------------------------------------------*/
signed char
input_samToAlnMain(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStrPtr,
   signed char **refFileStrPtr,
   signed char **outFileStrPtr,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - gets user input
   '   o fun05 sec02:
   '     - variable declerations
   '   o fun05 sec02:
   '     - check if user input anything
   '   o fun05 sec03:
   '     - get use input
   '   o fun05 sec03:
   '     - return any errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   sint siArg = 1;
   schar *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - check if user input anything
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no arguments input*/
      phelp_samToAlnMain(stdout);
      goto phelp_fun05_sec04;
   } /*If: no arguments input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - get use input
   ^   o fun05 sec03 sub01:
   ^     - get file io and start get input loop
   ^   o fun05 sec03 sub02:
   ^     - get output formating
   ^   o fun05 sec03 sub03:
   ^     - see if help/version/match settings print
   ^   o fun05 sec03 sub04:
   ^     - else; unkown input
   ^   o fun05 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - get file io and start get input loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (schar *) "-ref",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*If: reference seqeunce*/
          ++siArg;
          *refFileStrPtr = (schar *) argAryStr[siArg];
       } /*If: reference seqeunce*/

      else if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*Else If: sam file with mapped reads*/
          ++siArg;
          *samFileStrPtr = (schar *) argAryStr[siArg];
       } /*Else If: sam file with mapped reads*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*Else If: output file*/
          ++siArg;
          *outFileStrPtr = (schar *) argAryStr[siArg];
       } /*Else If: output file*/

      else if(
         ! eql_charCp(
            (schar *) "-match",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*Else If: match matrix file*/
          ++siArg;
          errSC =
              readMatchFile_alnSet(
                 alnSetSTPtr,
                 (schar *) argAryStr[siArg]
             );

          if(errSC)
          { /*If: had an error*/
             fprintf(
                stderr,
                "error reading -match %s\n",
                argAryStr[siArg]
             );

             goto err_fun05_sec04;
          } /*If: had an error*/
       } /*Else If: match matrix file*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - get output formating
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-mask",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ) alnSetSTPtr->pFullAlnBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-mask",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ) alnSetSTPtr->pFullAlnBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-pos",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ) alnSetSTPtr->pBasePosBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-pos",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ) alnSetSTPtr->pBasePosBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-wrap",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*Else If: line wrap*/
          ++siArg;
          
          tmpStr = (schar *) argAryStr[siArg];

          tmpStr +=
             strToUI_base10str(
                (schar *) argAryStr[siArg],
                &alnSetSTPtr->lineWrapUI
             );

          if(*tmpStr != '\0')
          { /*If: non-numeric entry*/
             fprintf(
                stderr,
                "non-numeric or to large -wrap %s\n",
                argAryStr[siArg]
             );

             goto err_fun05_sec04;
          } /*If: non-numeric entry*/
       } /*Else If: line wrap*/

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - see if help/version/match settings print
      \**************************************************/

      else if(
         checkIfHelp_samToAlnMain(
           (schar *) argAryStr[siArg]
        )
      ){ /*Else If: wanted help message*/
         phelp_samToAlnMain(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      else if(
         checkIfVersion_samToAlnMain(
           (schar *) argAryStr[siArg]
        )
      ){ /*Else If: wanted version number*/
         pversion_samToAlnMain(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version numbe*/

      else if(
         ! eql_charCp(
            (schar *) "-p-match",
            (schar *) argAryStr[siArg],
            '\0'
           )
       ){ /*Else If: printing default match matrix*/
          pDefMatchMatrix_alnSet(stdout);
          goto pmatchMatrix_fun05_sec04;
       } /*Else If: printing default match matrix*/
      
      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - else; unkown input
      \**************************************************/

       else
       { /*Else: unkown input*/
          fprintf(
             stderr,
             "%s is not reconginized\n",
             argAryStr[siArg]
           );

          goto err_fun05_sec04;
       } /*Else: unkown input*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return any errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_fun05_sec04;

   phelp_fun05_sec04:;
   pmatchMatrix_fun05_sec04:;
   pversion_fun05_sec04:;
   errSC = 1;
   goto cleanUp_fun05_sec04;

   err_fun05_sec04:;
   errSC = 2;
   goto cleanUp_fun05_sec04;
  
   cleanUp_fun05_sec04:;
   return errSC;
} /*input_samToAlnMain*/

/*-------------------------------------------------------\
| Main:
|   - driver function for samToAln
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array of user input arguments
| Output:
|   - Prints:
|     o aligned sequences to stdout or "-out file.aln"
\-------------------------------------------------------*/
#ifdef PLAN9
signed char
#else
int
#endif
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get user input, open files
   '   o main sec03:
   '     - print out alignments
   '   o main sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^    - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   ulong seqUL = 0; /*sequence on*/

   schar *samFileStr = 0;
   schar *refFileStr = 0;
   schar *outFileStr = 0;

   struct alnSet alnSetStackST;
   struct seqST refStackST;

   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^    - initialize, get user input, open files
   ^    o main sec02 sub01:
   ^      - initialize structures
   ^    o main sec02 sub02:
   ^      - get user input
   ^    o main sec02 sub03:
   ^      - set up structures
   ^    o main sec02 sub04:
   ^      - open sam file
   ^    o main sec02 sub05:
   ^      - open reference file and get reference
   ^    o main sec02 sub06:
   ^      - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_alnSet(&alnSetStackST);
   init_seqST(&refStackST);
   init_samEntry(&samStackST);

   alnSetStackST.lineWrapUI = def_lineWrap_samToAlnMain;
   alnSetStackST.pBasePosBl = def_ppos_samToAlnMain;
   alnSetStackST.pFullAlnBl = def_pmask_samToAlnMain;

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input
   \*****************************************************/

   errSC =
      input_samToAlnMain(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &refFileStr,
         &outFileStr,
         &alnSetStackST
      );

   if(errSC)
   { /*If: had an error*/
      --errSC;     /*convert help/version to 0*/
      errSC >>= 2; /*convert invalid input to 4*/
      goto cleanUp_main_sec04_sub04;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - set up structures
   \*****************************************************/

   setup_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open sam file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
      samFILE = stdin;

   else
   { /*Else: user provided a file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open the sam file*/
         fprintf(
            stderr,
            "could not open -sam %s\n",
            samFileStr
         );

         goto fileErr_main_sec04_sub03;
      } /*If: could not open the sam file*/
   } /*Else: user provided a file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open reference file and get reference
   \*****************************************************/

   outFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   if(! outFILE)
   { /*If: could not open reference file*/
      fprintf(
         stderr,
         "could not open -ref %s\n",
         refFileStr
      );

      goto fileErr_main_sec04_sub03;
   } /*If: could not open reference file*/


   errSC =
      getFaSeq_seqST(
         outFILE,
         &refStackST
      );

   if(errSC)
   { /*If: had error*/
      if(errSC & def_fileErr_seqST)
      { /*If: not a fasta file*/
         fprintf(
            stderr,
            "-ref %s is not a fasta file\n",
            refFileStr
         );

         goto fileErr_main_sec04_sub03;
      } /*If: not a fasta file*/

      if(errSC & def_memErr_seqST)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error reading -ref %s\n",
            refFileStr
         );

         goto memErr_main_sec04_sub02;
      } /*If: memory error*/

      /*else is EOF; only one seqeunce*/
   } /*If: had error*/

   fclose(outFILE);
   outFILE = 0;

   refStackST.lenIdUL =
      (ulong)
      cpWhite_ulCp(
         refStackST.idStr,
         &refStackST.idStr[refStackST.idStr[0] == '>']
      ); /*remove white space from reference id*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open output file
   \*****************************************************/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: user provided an output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "could not open -out %s\n",
            outFileStr
         );

         goto fileErr_main_sec04_sub03;
      } /*If: could not open output file*/
   } /*Else: user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^    - print out alignments
   ^    o main sec03 sub01:
   ^      - get first sam file line
   ^    o main sec03 sub02:
   ^      - print global header (program history)
   ^    o main sec03 sub03:
   ^     - add samToAlnMain to program header
   ^    o main sec03 sub04:
   ^     - print alingments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first sam file line
   \*****************************************************/

   errSC =
     get_samEntry(
        &samStackST,
        &buffHeapStr,
        &lenBuffUL,
        samFILE
     );

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_EOF_samEntry)
      { /*If: had empty file*/
          fprintf(
             stderr,
             "-sam %s is empty\n",
             samFileStr
           );

         goto fileErr_main_sec04_sub03;
      } /*If: had empty file*/

      fprintf(
         stderr,
         "memory error reading first line of -sam %s\n",
         samFileStr
       );

      goto memErr_main_sec04_sub02;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print global header (program history)
   \*****************************************************/

   errSC =
      psamPg_samToAln(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE,
         outFILE
      );

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_EOF_samEntry)
      { /*If: had empty file*/
          fprintf(
             stderr,
             "-sam %s had only header entries\n",
             samFileStr
           );

         goto fileErr_main_sec04_sub03;
      } /*If: had empty file*/

      fprintf(
         stderr,
         "memory error header/first read of -sam %s\n",
         samFileStr
       );

      goto memErr_main_sec04_sub02;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - add samToAlnMain to program header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "#PG\tID:%s\tPN:%s\tVN:%i-%02i-%02i\tCL:%s",
      "samToAln",
      "samToAln",
      def_year_samToAlnMain,
      def_month_samToAlnMain,
      def_day_samToAlnMain,
      "samToAln"
   ); /*print command without user arguments*/

   for(
      seqUL = 1;
      seqUL < (ulong) numArgsSI;
      ++seqUL
   ){ /*Loop: print out command arguments*/
      fprintf(
         (FILE *) outFILE,
         " %s",
         argAryStr[seqUL]
      );
   } /*Loop: print out command arguments*/

   fprintf(
      (FILE*) outFILE,
      "\n\n"
    );

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - print alingments
   \*****************************************************/

   seqUL = 0;

   while(! errSC)
   { /*Loop: print alignments in sam file*/
      ++seqUL;

      if(*samStackST.seqStr == '*')
         goto nextRead_main_sec03_sub04;

      if(samStackST.mapqUC & 4)
         goto nextRead_main_sec03_sub04;

      if(
          eql_charCp(
             refStackST.idStr,
             samStackST.refIdStr,
             '\0'
          ) /*see if same reference*/
      ) goto nextRead_main_sec03_sub04;

      errSC =
         paln_samToAln(
            &samStackST,
            &refStackST,
            &alnSetStackST,
            outFILE
         ); /*print alignment*/
            
      nextRead_main_sec03_sub04:;

      errSC =
        get_samEntry(
           &samStackST,
           &buffHeapStr,
           &lenBuffUL,
           samFILE
        );
   } /*Loop: print alignments in sam file*/

   if(errSC & def_memErr_samEntry)
   { /*If: had an error*/
      fprintf(
         stderr,
         "memory error for read %lu of -sam %s\n",
         seqUL,
         samFileStr
       );

      goto memErr_main_sec04_sub02;
   } /*If: had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   ^   o main sec04 sub01:
   ^     - no error clean up
   ^   o main sec04 sub02:
   ^     - memory error clean up
   ^   o main sec04 sub03:
   ^     - file error clean up
   ^   o main sec04 sub04:
   ^     - general clean up (error and no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec04_sub02:;
   errSC = 1;
   goto cleanUp_main_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_main_sec04_sub03:;
   errSC = 2;
   goto cleanUp_main_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub04:
   *   - general clean up (error and no error)
   \*****************************************************/

   cleanUp_main_sec04_sub04:;

   freeStack_alnSet(&alnSetStackST);
   freeStack_seqST(&refStackST);
   freeStack_samEntry(&samStackST);

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

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
:   However,for cases were the public domain is not
:   suitable,such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible,this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy,modify,publish,use,compile,
:   sell,or distribute this software,either in source
:   code form or as a compiled binary,for any purpose,
:   commercial or non-commercial,and by any means.
: 
: In jurisdictions that recognize copyright laws,the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS\n",WITHOUT WARRANTY OF
:   ANY KIND,EXPRESS OR IMPLIED,INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF
:   CONTRACT,TORT OR OTHERWISE,ARISING FROM,OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information,please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted,free of charge,to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"),to
:   deal in the Software without restriction,including
:   without limitation the rights to use,copy,modify,
:   merge,publish,distribute,sublicense,and/or sell
:   copies of the Software,and to permit persons to whom
:   the Software is furnished to do so,subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS\n",WITHOUT WARRANTY OF
:   ANY KIND,EXPRESS OR IMPLIED,INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM,DAMAGES OR OTHER LIABILITY,WHETHER IN
:   AN ACTION OF CONTRACT,TORT OR OTHERWISE,ARISING
:   FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
