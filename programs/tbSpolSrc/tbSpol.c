/*########################################################
# Name: tbSpol
#   - Identifies spoligotypes in TB samples (driver)
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun-01: pVersion_tbSpol
'     - prints out the version number
'   o fun02: phelp_tbSpol
'     - prints the help messge for tbSpol
'   o fun03: input_tbSpol
'     - checks the users input and sets to input variable
'   o fun02: pHelp_tbSpol
'     - prints the help message to an file
'   o main:
'     - driver function (runs the spoligotyping)
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
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/charCp.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/kmerFind.h"

#include "../genFreezeTB/spolST.h"
#include "../genFreezeTB/spolFind.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../genFreezeTB/tbSpolDefs.h"
#include "../ftbVersion.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genlLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genLib/inflate.h"
!   - .c  #include "../genAln/memwater.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .h  #include "../genAln/alnDefs.h"
!   - .h  #include "../genLib/genMath.h" .h max macros
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pVersion_tbSpol
|   - prints out the version number
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to out FILE
\-------------------------------------------------------*/
void
pversion_tbSpol(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "tbSpol from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pVersion_tbSpol*/

/*-------------------------------------------------------\
| Fun02: phelp_tbSpol
|   - prints the help messge for tbSpol
| Input:
|   o outFileStr:
|     - c-string with name of file to print help message
|       to
|     - Use "-" or "1" for stdout and "2" for stderr
| Output:
|   o Prints:
|     - The help message to outFileStr
|   o Returns:
|     - 0 for no errors
|     - 1 if I could not open the file
\-------------------------------------------------------*/
signed char
phelp_tbSpol(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help messge for tbSpol
   '   o fun02 sec01:
   '     - Variable declerations
   '   o fun02 sec02:
   '     - Open the output file
   '   o fun02 sec03:
   '     - Print usage lines
   '   o fun02 sec04:
   '     - Print user input lines
   '   o fun02 sec05:
   '     - Print output options
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - Print usage lines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "tbSpol -spoligo spacers.fa -sam consensus.sam%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "or tbSpol -spoligo spacers.fa -fa consensus.fa%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Finds spoligotype of an sequence%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - Print user input line
   ^   o fun02 sec04 sub01:
   ^     - Print input header
   ^   o fun02 sec04 sub02:
   ^     - Print spoligotype sequence input
   ^   o fun02 sec04 sub03:
   ^     - Print sam/fasta/fastq input options
   ^   o fun02 sec04 sub04:
   ^     - Print database input
   ^   o fun02 sec04 sub05:
   ^     - Print mapping coordinates options
   ^   o fun02 sec04 sub06:
   ^     - Print the filtering paramerters
   ^   o fun02 sec04 sub07:
   ^     - Print alignment method
   ^   o fun02 sec04 sub08:
   ^     - print alignment method
   ^   o fun02 sec04 sub09:
   ^     - fragment mode options
   ^   o fun02 sec04 sub10:
   ^     - Print help/verson command
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec04 Sub01:
   *   - Print input header
   \*****************************************************/

   fprintf(outFILE, "Input:%s", str_endLine);

   /*****************************************************\
   * Fun02 Sec04 Sub02:
   *   - Print spoligotype sequence input
   \*****************************************************/

   fprintf(
      outFILE,
      "  -spoligo: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Fasta file with spoligotype references%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub03:
   *   - Print sam/fasta/fastq input options
   \*****************************************************/

   if(def_seqIn_tbSpolDefs == def_samFile_tbSpolDefs)
      fprintf(
         outFILE,
         "  -sam mappings.sam: [Requried; stdin]%s",
      str_endLine
      );
   else if(def_seqIn_tbSpolDefs == def_faFile_tbSpolDefs)
      fprintf(
         outFILE,
         "  -fa reads.fa: [Required; stdin]%s",
      str_endLine
      );
   else if(def_seqIn_tbSpolDefs == def_fqFile_tbSpolDefs)
      fprintf(
         outFILE,
         "  -fq reads.fq: [Required; stdin]%s",
      str_endLine
      );
   else
      fprintf(
         outFILE,
         "  -sam file.sam: [Required; stdin]%s",
      str_endLine
      );

   fprintf(
      outFILE,
      "    o File with sequences to spoligotype%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Make sure your sequence(s) cover the%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      entire direct repeat region%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Use \"-option -\" to use stdin input%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Options%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        -sam: Use an sam file%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        -fa: use an fasta file (slow)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        -fq: use an fastq file (slow)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub04:
   *   - Print database input
   \*****************************************************/

   fprintf(outFILE, "  -db: [None]%s", str_endLine);

   fprintf(
      outFILE,
      "    o Database (csv file) to look up lineages in%s",
      str_endLine
   );

   fprintf(
    outFILE,
    "    o Strain,barcode,ignored,lineage,sit,countries%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub05:
   *   - Print output file
   \*****************************************************/

   fprintf(outFILE, "  -out: [stdout]%s", str_endLine);

   fprintf(
     outFILE,
     "    o File to output results to (- is stdout)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub06:
   *   - Print mapping coordinates options
   \*****************************************************/

   fprintf(
      outFILE,
      "  -dr-start: [%i]%s",
      def_DRStart_tbSpolDefs,
      str_endLine
   );

   fprintf(
    outFILE,
    "    o First direct repeat region base in reference%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o -sam: to align spacers to DR region only%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  -dr-end: [%i]%s",
      def_DREnd_tbSpolDefs,
      str_endLine
   );

   fprintf(
    outFILE,
    "    o Last direct repeat region base in reference%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o -sam: to align spacers to DR region only%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub07:
   *   - Print the filtering paramerters
   \*****************************************************/

   fprintf(
      outFILE,
      "  -min-score: [%.2f]%s",
      def_minPercScore_tbSpolDefs,
      str_endLine
   );

   fprintf(
     outFILE,
     "    o Minimim percentage of score to count an hit%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o Scores: match = 5, snp = -4, indel = -10%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  -min-kmer-perc: [%f]%s",
      def_minKmerPerc_tbSpolDefs,
      str_endLine
   );

   fprintf(
     outFILE,
     "    o Minimum percent of kmers needed to do an%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "      waterman alignment on an window for -fast%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o 50%% is 10 or more kmers for spoligotypes%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub08:
   *   - Print alignment method
   \*****************************************************/

   if(def_fastSearch_tbSpolDefs)
   { /*If: I fast kmer is default method*/
      fprintf(
         outFILE,
         "  -fast: [Default]%s",
      str_endLine
      );
   } /*If: I fast kmer is default method*/

   else
   { /*Else: I slow is default method*/
      fprintf(
         outFILE,
         "  -fast: [No]%s",
      str_endLine
      );
   } /*Else: I slow is default method*/

   fprintf(
     outFILE,
     "    o Use kmers to narrow down possible%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "      spoligotype locations (faster)%s",
      str_endLine
   );

   if(! def_fastSearch_tbSpolDefs)
      fprintf(
         outFILE,
         "  -slow: [Default]%s",
      str_endLine
      );

   else
      fprintf(
         outFILE,
         "  -slow: [No]%s",
      str_endLine
      );

   fprintf(
     outFILE,
     "    o Uses an Waterman alignment to map spacers%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec04 Sub09:
   *   - fragment mode options
   \*****************************************************/

   if(   def_frag_tbSpolDefs
      || def_conFrag_tbSpolDefs
   ) fprintf(
         outFILE,
         "  -frag: [Yes]%s",
      str_endLine
      );

    else 
      fprintf(
         outFILE,
         "  -frag: [No]%s",
      str_endLine
      );

   fprintf(
     outFILE,
     "    o search for fragments (not full genomes)%s",
      str_endLine
   );

   if(def_conFrag_tbSpolDefs)
      fprintf(
         outFILE,
         "  -con-frag: [Yes]%s",
      str_endLine
      );

   else
      fprintf(
         outFILE,
         "  -con-frag: [No]%s",
      str_endLine
      );

   fprintf(
     outFILE,
     "    o fragments are in an conensus%s",
      str_endLine
   );
   
   if(! def_conFrag_tbSpolDefs
      && def_frag_tbSpolDefs
   )
      fprintf(
         outFILE,
         "  -read-frag: [Yes]%s",
      str_endLine
      );

   else
      fprintf(
         outFILE,
         "  -read-frag: [No]%s",
      str_endLine
      );

   fprintf(
     outFILE,
     "    o fragments are in reads%s",
      str_endLine
   );

   if(! def_frag_tbSpolDefs
      && ! def_conFrag_tbSpolDefs
   ) fprintf(
         outFILE,
         "  -no-frag: [Yes]%s",
      str_endLine
      );

   else
      fprintf(
         outFILE,
         "  -no-frag: [No]%s",
      str_endLine
      );

   fprintf(
     outFILE,
     "    o reads/consensus have full direct repeat%s",
      str_endLine
   );
   
   /*****************************************************\
   * Fun02 Sec04 Sub10:
   *   - Print help/verson command
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: print this help message%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  -v: print the version number%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - Print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:%s", str_endLine);

   fprintf(
      outFILE,
      "  - Prints sequence name and spoligotype barcode%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - name\\tlineageId\\tbarcode\\toctal\\tstrain"

   );

   fprintf(outFILE, "\\tSIT\\tcountries%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec06:
   ^   - Close the output file and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outFILE != stdout && outFILE != stderr)
      fclose(outFILE);

   outFILE = 0;

   return 0;
} /*phelp_tbSpol*/


/*-------------------------------------------------------\
| Fun03: input_tbSpol
|   - checks the users input and sets to correct variable
| Input:
|   o numArgsSI:
|     - number arguments user input
|   o argAryStr:
|     - c-string array with user input
|   o seqFileStr:
|     - pointer to c-string to hold the sequence file name
|   o seqTypeFlag:
|     - holds the type of file input for seqFileStr
|   o refFileStr:
|     - pointer to c-string to hold the spoligo sequences
|       file name
|   o spoliogDbStr:
|     - pointer to c-string to hold the path to the
|       database with spoligo lineages
|   o outFileStr:
|     - pointer to c-string to hold the output file name
|   o minPercScoreF:
|     - pointer to float to hold the minimum percent score
|       to detect an spoligotype hit
|   o dirStartSI:
|     - will hold the start of the DR region
|   o dirEndSI:
|     - Will hold the end of the DR region
|   o minKmersPercF:
|     - pointer to float to hold the minimum percent of
|       kmers needed to do an waterman alignment on an
|       window in the fast kmer method
|   o fastTypingBl:
|     - Set to 1 if user wants faster kmer method
|     - Set to 0 if user wants slower method
|   o fragBl:
|     - Set to 1 if user is inputing fragments
|     - Set to 0 if user is inputing direct repeat regions
|   o conFragBl:
|     - Set to 1 if user is inputing consensus fragments
|     - Set to 0 if user not consensus or not fragments
| Output:
|   - Modifies:
|     o All input variables except argAryStr[siArg and
|       argStr to hold the users input
|       - Sets seqTypeFlag to:
|         o def_fqFile_tbSpolDefs if fastq file input
|         o def_faFile_tbSpolDefs if fasta file input
|         o def_samFile_tbSpolDefs if an sam file input
|         o def_samConFile_tbSpolDefs if an sam file with
|           mapped consensuses was input
|   - Prints:
|     o error messages to stderr (for errors)
|     o help message and version number requests to stdout
|   - Returns:
|     o 0 if no errors
|     o 1 if printed help message or version number
|     o 2 if error
\-------------------------------------------------------*/
signed char
input_tbSpol(
   int numArgsSI,            /*number arguments input*/
   char *argAryStr[],        /*array of arguments*/
   signed char **seqFileStr, /*sequence file name*/
   signed char *seqTypeFlag, /*Type of file input*/
   signed char **refFileStr, /*spoligotype seq file name*/
   signed char **spoliogDbStr,/*database of lineages*/
   signed char **outFileStr, /*File to save to*/
   float *minPercScoreF,     /*min % score to count hit*/
   signed int *dirStartSI,   /*start of DR region*/
   signed int *dirEndSI,     /*start of DR region*/
   float *minKmersPercF,     /*Min number kmers for fast*/
   signed char *fastTypingBl,/*1: Do fast spoligo typing*/
   signed char *fragBl,      /*1: fragment checking*/
   signed char *conFragBl    /*1: non-con fragment mode*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - Check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - clean up and return errors/success
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no input*/
      phelp_tbSpol(stdout);
      goto phelp_fun03_sec04;
   } /*If: no input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - database and output file io (non-read/seq)
   ^   o fun03 sec03 sub02:
   ^     - read/sequence file io input
   ^   o fun03 sec03 sub03:
   ^     - filtering parameter input checks
   ^   o fun03 sec03 sub04:
   ^     - alignment method input checks
   ^   o fun03 sec03 sub05:
   ^     - help message request checks
   ^   o fun03 sec03 sub06:
   ^     - version number request check
   ^   o fun03 sec03 sub07:
   ^     - invalid argument
   ^   o fun03 sec03 sub08:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - database and output file io (non-read/sequence)
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (signed char *) "-spoligo",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: an fasta file with spoligotypes*/
         ++siArg;
         *refFileStr = (signed char *) argAryStr[siArg];
      } /*If: an fasta file with spoligotypes*/

      else if(
         ! eql_charCp(
            (signed char *) "-db",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the lineage database was input*/
         ++siArg;
         *spoliogDbStr = (signed char *) argAryStr[siArg];
      } /*Else If: the lineage database was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: an fastq file was input*/
         ++siArg;
         *outFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: an fastq file was input*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - read/sequence file io input
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-fq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: an fastq file was input*/
         *seqTypeFlag = def_fqFile_tbSpolDefs;

         ++siArg;
         *seqFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: an fastq file was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-fa",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: an fastq file was input*/
         *seqTypeFlag = def_faFile_tbSpolDefs;

         ++siArg;
         *seqFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: an fastq file was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: an sam file was input*/
         *seqTypeFlag = def_samFile_tbSpolDefs;

         ++siArg;
         *seqFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: an sam file was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-sam-con",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: sam file of an consensus was input*/
         *seqTypeFlag = def_samConFile_tbSpolDefs;

         ++siArg;
         *seqFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: sam file of an consensus was input*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - filtering parameter input checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-dr-start",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the direct repeat start is input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[siArg],
               dirStartSI
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-dr-start %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

             goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: the direct repeat start is input*/

      else if(
         ! eql_charCp(
            (signed char *) "-dr-end",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the direct repeat start is input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[siArg],
               dirEndSI
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-dr-end %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

             goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: the direct repeat start is input*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-score",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min score to keep an spoligotype*/
         ++siArg;
         *minPercScoreF = atof(argAryStr[siArg]);
      } /*Else If: min score to keep an spoligotype*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-perc-kmer",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: Min kmers for fast method selected*/
         ++siArg;
         *fastTypingBl = 1;
         *minKmersPercF = atof(argAryStr[siArg]);
      } /*Else If: Min kmers for fast method selected*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - alignment method input checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-fast",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
      )) *fastTypingBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-slow",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
      )) *fastTypingBl = 0;

      else if(
         ! eql_charCp(
            (signed char *) "-frag",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
      )) *fragBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-con-frag",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: consensus is in fragments*/
         *conFragBl = 1; /*using consensuses*/
         *fragBl = 1;
      } /*Else If: consensus is in fragments*/

      else if(
         ! eql_charCp(
            (signed char *) "-read-frag",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: sequence is in fragments*/
         *conFragBl = 0; /*using consensuses*/
         *fragBl = 1;
      } /*Else If: sequence is in fragments*/

      else if(
         ! eql_charCp(
            (signed char *) "-no-frag",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: sequence is in fragments*/
         *fragBl = 0;
         *conFragBl = 0; /*using consensuses*/
      } /*Else If: sequence is in fragments*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - help message request checks
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_tbSpol(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_tbSpol(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_tbSpol(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_tbSpol(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_tbSpol(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: wanted help message*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - version number request check
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_tbSpol(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_tbSpol(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_tbSpol(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_tbSpol(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_tbSpol(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - invalid argument
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recongnized%s",
            argAryStr[siArg],
            str_endLine
          );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub08:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - clean up and return errors/success
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
} /*input_tbSpol*/

/*-------------------------------------------------------\
| Main:
|   - driver function for tbSpol
| Input:
|   o argAryStr:
|     - Array of c-strings the user input
|     - see the help message for an list of options
|   o numArgsSI:
|     - Length of argAryStr
| Output:
|   - Prints:
|     o spoligotypes to specifed files
\-------------------------------------------------------*/
int
main(
   signed int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   -
   '   o main sec01:
   '     - Variable declerations
   '   o main sec02:
   '     - Process user input
   '   o main sec03:
   '     - Read in the spoligotype sequences
   '   o main sec04:
   '     - Read in the spoligotype database
   '   o main sec05:
   '     - Check reads for spoligotypes
   '   o main sec06:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 0;
   signed char errSC = 0;
   #define def_numSpol_tbSpolDefs 64
   unsigned int codeAryUI[def_numSpol_tbSpolDefs + 1];
      /*holds barcode for spoligo*/

   signed char *seqFileStr = 0;   /*sequences to type*/
   signed char *spoligoFileStr = 0;
      /*spoligotype Sequences*/
   signed char *spoligoDbStr = 0; /*Database of lineages*/
   signed char *outFileStr = 0;   /*file to output to*/

   signed int drStartSI = def_DRStart_tbSpolDefs;
   signed int drEndSI = def_DREnd_tbSpolDefs;

   /*Type of sequence file input*/
   signed char seqTypeFlagSC = def_seqIn_tbSpolDefs;

   signed char fragCheckBl = def_frag_tbSpolDefs;
     /*do fragmentation checks*/

   signed char conFragBl = def_conFrag_tbSpolDefs;
      /*consensus fragment mode*/

   unsigned int numSupReadsUI = 0;
     /*number reads with an spacer*/

   float minPercScoreF = def_minPercScore_tbSpolDefs;

   /*for kmer checks*/
   float minKmerPercF = def_minKmerPerc_tbSpolDefs;
   float percShiftF = def_percShift_tbSpolDefs;
   float percExtraNtInWinF = def_extraNtInWin_tbSpolDefs;
   unsigned char lenKmerUC = def_lenKmer_tbSpolDefs;
   signed char fastBl = def_fastSearch_tbSpolDefs;

   struct seqST *slowRefHeapAryST= 0;/*ref sequences*/
   signed int numSpacersSI = 0;

   struct seqST seqStackST;      /*Sequences to check*/
   struct alnSet alnSetStackST;  /*Settings*/

   struct samEntry samStackST;   /*Sequences to check*/

   /*Varaibles dealng with the spoligo lineage database*/
   struct spolST *spolDbHeapAryST = 0;
   signed int numLineagesSI = 0;
      /*Number lineages in database*/

   /*kmer finding of spoligotype variables*/
   struct tblST_kmerFind kmerTblStackST;
   struct refST_kmerFind *fastRefHeapAryST = 0;

   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - Process user input and intialize variables
   ^   o main sec02 sub01:
   ^     - Extract the user input
   ^   o main sec02 sub02:
   ^     - initialize variables
   ^   o main sec02 sub03:
   ^     - Check if I can open spoligotype file
   ^   o main sec02 sub04:
   ^     - Check if I can open the output file
   ^   o main sec02 sub05:
   ^     - Check if I can open the sequence file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_seqST(&seqStackST);
   init_alnSet(&alnSetStackST);
   init_samEntry(&samStackST);
   init_tblST_kmerFind(&kmerTblStackST);

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - Extract the user input
   \*****************************************************/

   siArg = 1;

   errSC =
      input_tbSpol(
         numArgsSI,
         argAryStr,
         &seqFileStr,
         &seqTypeFlagSC,
         &spoligoFileStr,
         &spoligoDbStr,
         &outFileStr,
         &minPercScoreF,
         &drStartSI,
         &drEndSI,
         &minKmerPercF,
         &fastBl,
         &fragCheckBl,
         &conFragBl
      );

   if(errSC)
   { /*If: had an error*/
      --errSC; /*reduce help/version to no error (0)*/
      goto err_main_sec06_sub02;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - initialize variables
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory%s", str_endLine);
      goto err_main_sec06_sub02;
   } /*If: I had an memory error*/

   errSC =
      setup_tblST_kmerFind(
         &kmerTblStackST,
         lenKmerUC
      );

   if(errSC)
   { /*If: I had an memory error*/
      fprintf(stderr, "Ran out of memory%s", str_endLine);
      goto err_main_sec06_sub02;
   } /*If: I had an memory error*/

   for(
      siArg = 0;
      siArg < def_numSpol_tbSpolDefs;
      ++siArg
   ) codeAryUI[siArg] = 0;

   codeAryUI[siArg] = -1;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - Check if I can open spoligotype file
   \*****************************************************/

   inFILE =
      fopen(
         (char *) spoligoFileStr,
         "r"
      );

   if(! inFILE)
   { /*If: The spoligotype file could not be opened*/
      fprintf(
         stderr,
         "Unable to open -spoligo %s%s",
         spoligoFileStr,
         str_endLine
      );

      goto err_main_sec06_sub02;
   } /*If: The spoligotype file could not be opened*/

   fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - Check if I can open the output file
   \*****************************************************/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: An output file was provided*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: The output file could not be opened*/
         fprintf(
            stderr,
            "Unable to open -out %s%s",
            outFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: The output file could not be opened*/
   } /*Else: An output file was provided*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - check if I can open the sequence file
   \*****************************************************/

   if(! seqFileStr || *seqFileStr == '-')
      inFILE = stdin;

   else
   { /*Else: An input file was provided*/
      inFILE =
         fopen(
            (char *) seqFileStr,
            "r"
         );

      if(! inFILE)
      { /*If: The input file could not be opened*/
         fprintf(
            stderr,
            "Unable to open - %s%s",
            seqFileStr,
            str_endLine
         );

         if(seqTypeFlagSC == def_fqFile_tbSpolDefs)
            fprintf(
               stderr,
               "-fq %s%s",
               seqFileStr,
               str_endLine
            );

         else if(seqTypeFlagSC == def_faFile_tbSpolDefs)
            fprintf(
               stderr,
               "-fa %s%s",
               seqFileStr,
               str_endLine
            );

         else if(seqTypeFlagSC == def_samFile_tbSpolDefs)
            fprintf(
               stderr,
               "-sam %s%s",
               seqFileStr,
               str_endLine
            );

         else if(
            seqTypeFlagSC == def_samConFile_tbSpolDefs
         ) fprintf(
              stderr,
              "-sam-con %s%s",
              seqFileStr,
              str_endLine
           );

         goto err_main_sec06_sub02;
      } /*If: The input file could not be opened*/
   } /*Else: An input file was provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - Read in the spoligotype sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fastBl)
   { /*If: I am doing the kmer method*/
      fastRefHeapAryST =
        faToAry_refST_kmerFind(
           spoligoFileStr,
           lenKmerUC,
           &numSpacersSI,
           minKmerPercF,
           &kmerTblStackST,
           percExtraNtInWinF,
           percShiftF,
           &alnSetStackST,
           &errSC
      );
   } /*If: I am doing the kmer method*/

   else
   { /*Else: I am doing the waterman method*/
      slowRefHeapAryST =
        getRefs_spolFind(
           spoligoFileStr,
           &numSpacersSI,
           &errSC
        );
   } /*Else: I am doing the waterman method*/

   if(errSC)
   { /*If: I had an error*/
      if(errSC == def_memErr_tbSpolDefs)
      { /*If: I had an file error*/
         fprintf(
            stderr,
            "-spoligo %s has a non-fasta entry%s",
            spoligoFileStr,
            str_endLine
        );
      } /*If: I had an file error*/

      else
      { /*Else: This is an memory error*/
         fprintf(
            stderr,
            "Ran out of memory getting spoligo seq%s",
            str_endLine
        );
      } /*Else: This is an memory error*/

      goto err_main_sec06_sub02;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - Read in the spoligotype database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(spoligoDbStr)
   { /*If: An database of lineages was input*/
      spolDbHeapAryST =
         readDb_spolST(
            spoligoDbStr,
            &numLineagesSI,
            &errSC
      );

      if(errSC)
      { /*If: I had an error*/
         if(errSC == def_memErr_tbSpolDefs)
         { /*If: I had an file error*/
            fprintf(
               stderr,
               "Could not open -db %s%s",
               spoligoDbStr,
               str_endLine
            );
         } /*If: I had an file error*/

         else
         { /*Else: I had an memory error*/
            fprintf(
               stderr,
               "Ran out of memory reading -db %s%s",
               spoligoDbStr,
               str_endLine
            );
         } /*Else: I had an memory error*/

         goto err_main_sec06_sub02;
      } /*If: I had an error*/
   } /*If: An database of lineages was input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - Check reads for spoligotypes
   ^   o main sec05 sub01:
   ^     - print header
   ^   o main sec05 sub02:
   ^     - fastx file spoligotype detection
   ^   o main sec05 sub03:
   ^     - sam file spoligotype detection
   ^   o main sec05 sub04:
   ^     - print out fragment check results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - print header
   \*****************************************************/

   phead_spolST(
      (!conFragBl) & fragCheckBl,
      outFILE
   );

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - fastx file spoligotype detection
   *   o main sec05 sub02 cat01:
   *     - Check if fastx file and read in first entry
   *   o main sec05 sub02 cat02:
   *     - Look for spoligotypes for each entry in fastx
   *   o main sec05 sub02 cat03:
   *     - get next sequence/read
   *   o main sec05 sub02 cat04:
   *     - Report fastx errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub02 Cat01:
   +   - Check if fastx file and read in first entry
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
        seqTypeFlagSC
      & (def_fqFile_tbSpolDefs | def_faFile_tbSpolDefs)
   ){ /*If: I am checking an fastq file*/

      if(seqTypeFlagSC == def_fqFile_tbSpolDefs)
         errSC = getFq_seqST(inFILE, &seqStackST);
      else
         errSC = getFa_seqST(inFILE, &seqStackST);

      siArg = 1;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec05 Sub02 Cat02:
      +   - Look for spoligotypes for each entry in fastx
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: Check read for spoligotypes*/

         if(fastBl)
         { /*If: I am doing the faster kmer method*/
            errSC =
               fx_spolFind(
                  &kmerTblStackST,
                  fastRefHeapAryST,
                  numSpacersSI,
                  &seqStackST,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*find spoligotype with kmer search*/
         } /*If: I am doing the faster kmer method*/

         else
         { /*Else: I am doing the slower waterman search*/
            errSC =
               fxwater_spolFind(
                  &seqStackST,
                  slowRefHeapAryST,
                  numSpacersSI,
                  minPercScoreF,
                  codeAryUI,
                  1, /*Print progress*/
                  fragCheckBl,
                  &alnSetStackST
               ); /*Detect spoligotypes*/
         } /*Else: I am doing the slower waterman search*/

         if(errSC & def_memErr_tbSpolDefs)
         { /*If: I had an memory error*/
            fprintf(
               stderr,
               "Ran out of memory%s",
               str_endLine
            );
            goto err_main_sec06_sub02;
         } /*If: I had an memory error*/

         if(! errSC)
         { /*If: I detected an spoligotype*/
            if(! fragCheckBl)
            { /*If: I am not doing fragment checks*/
               pspol_spolST(
                  seqStackST.idStr,
                  codeAryUI,
                  (!conFragBl) & fragCheckBl,
                  numSupReadsUI,
                  spolDbHeapAryST,
                  numLineagesSI,
                  outFILE
               );
            } /*If: I am not doing fragment checks*/

            /*add read to total sup read count*/
            ++numSupReadsUI;
         } /*If: I detected an spoligotype*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec05 Sub02 Cat03:
         +   - get next sequence/read
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         ++siArg;

         if(seqTypeFlagSC == def_fqFile_tbSpolDefs)
            errSC = getFq_seqST(inFILE, &seqStackST);

         else
            errSC = getFa_seqST(inFILE, &seqStackST);
      } /*Loop: Check read read for spoligotypes*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec05 Sub02 Cat04:
      +   - Report fastx errors
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errSC != def_EOF_seqST)
      { /*If: I had an error*/
         if(seqTypeFlagSC == def_fqFile_tbSpolDefs)
         { /*If: it was an fastq file error*/
            fprintf(
               stderr,
               "Invalid entry %i in -fq %s%s",
               siArg,
               seqFileStr,
               str_endLine
            );
         } /*If: it was an fastq file error*/

         else
         { /*Else: it was an fasta file error*/
            fprintf(
               stderr,
               "Invalid entry %i in -fa %s%s",
               siArg,
               seqFileStr,
               str_endLine
            );
         } /*Else: it was an fasta file error*/

         goto err_main_sec06_sub02;
      } /*If: I had an error*/
   } /*If: I am checking an fastq file*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - sam file spoligotype detection
   *   o main sec05 sub03 cat01:
   *     - Read in the first sam file entry
   *   o main sec05 sub03 cat02:
   *     - check if header or seqequence entry
   *   o main sec05 sub03 cat03:
   *     - Look for spoligotypes for each sam entry
   *   o main sec05 sub03 cat04:
   *     - move to next sam entry
   *   o main sec05 sub03 cat05:
   *     - Report sam file errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec05 Sub03 Cat01:
   +   - Read in the first sam file entry
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: I am checking an sam file for spoligotypes*/
      errSC = get_samEntry(&samStackST, inFILE);
      siArg = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec05 Sub03 Cat02:
      +   - check if header or seqequence entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: Check read read for spoligotypes*/
         if(*samStackST.seqStr == '\0')
            goto nextSamEntry_main_sec04_sub03_cat02;

         ++siArg;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec05 Sub03 Cat03:
         +   - Look for spoligotypes for each sam entry
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(fastBl)
         { /*If: faster kmer method*/
            errSC =
               sam_spolFind(
                  &kmerTblStackST,
                  fastRefHeapAryST,
                  numSpacersSI,
                  &samStackST,
                  drStartSI,
                  drEndSI,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*find spoligotype with kmer search*/
         } /*If: faster kmer method*/

         else
         { /*Else: slower waterman*/
            errSC =
               samWater_spolFind(
                  &samStackST,
                  slowRefHeapAryST,
                  numSpacersSI,
                  drStartSI,
                  drEndSI,
                  minPercScoreF,
                  codeAryUI,
                  fragCheckBl,
                  &alnSetStackST
               ); /*Detect spoligotypes*/
         } /*Else: slower waterman*/

         if(errSC == def_memErr_tbSpolDefs)
            goto err_main_sec06_sub02;

         if(! errSC)
         { /*If: I could detect spoligotypes*/
            if(! fragCheckBl)
            { /*If: I am not checking for fragments*/
               pspol_spolST(
                  samStackST.qryIdStr,
                  codeAryUI,
                  (!conFragBl) & fragCheckBl,
                  numSupReadsUI,
                  spolDbHeapAryST,
                  numLineagesSI,
                  outFILE
               );
            } /*If: I am not checking for fragments*/

            /*add read to total sup read count*/
            ++numSupReadsUI;
         } /*If: I could detect spoligotypes*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec05 Sub03 Cat04:
         +   - move to next sam entry
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         nextSamEntry_main_sec04_sub03_cat02:;
            errSC = get_samEntry(&samStackST, inFILE);
      } /*Loop: Check read read for spoligotypes*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec05 Sub03 Cat05:
      +   - Report sam file errors
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(errSC != 1)
      { /*If: I had an error*/
         fprintf(
            stderr,
            "Invalid entry %i in -sam %s%s",
            siArg,
            seqFileStr,
            str_endLine
         );

         goto err_main_sec06_sub02;
      } /*If: I had an error*/
   } /*Else: I am checking an sam file for spoligotypes*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - print out fragment check results
   \*****************************************************/

   if(fragCheckBl && ! conFragBl)
   { /*If: I am did fragment checks*/
      pspol_spolST(
         seqFileStr,
         codeAryUI,
         1,                 /*do not output lineage*/
         numSupReadsUI,
         spolDbHeapAryST,
         numLineagesSI,
         outFILE
      );
   } /*If: I am did fragment checks*/

   else if(conFragBl)
   { /*Else: I am working with consensus fragments*/
      pspol_spolST(
         samStackST.qryIdStr,
         codeAryUI,
         0,     /*I want to output the lineage*/
         numSupReadsUI,
         spolDbHeapAryST,
         numLineagesSI,
         outFILE
      );
   } /*Else: I am working with consensus fragments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec06_sub03;

   err_main_sec06_sub02:;
      errSC = 1;
      goto cleanUp_main_sec06_sub03;

   cleanUp_main_sec06_sub03:;

      /*check if did an slow waterman or fast kmer*/
      if(slowRefHeapAryST)
        freeHeapAry_seqST(slowRefHeapAryST, numSpacersSI);
      slowRefHeapAryST = 0;
   
      if(fastRefHeapAryST)
         freeHeapAry_refST_kmerFind(
            fastRefHeapAryST,
            numSpacersSI
         );
      fastRefHeapAryST = 0;
   
      if(spolDbHeapAryST)
        freeHeapAry_spolST(spolDbHeapAryST, numLineagesSI);
      spolDbHeapAryST = 0;
   
      freeStack_samEntry(&samStackST);
      freeStack_seqST(&seqStackST);
      freeStack_alnSet(&alnSetStackST);
      freeStack_tblST_kmerFind(&kmerTblStackST);
   
      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else
         fclose(inFILE);
      inFILE = 0;
   
      if(! outFILE) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stdout) ;
      else
         fclose(outFILE);
      outFILE = 0;
   
      return errSC;
} /*main*/
