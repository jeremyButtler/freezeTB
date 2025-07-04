/*#######################################################\
# Name: tbMiru
#   - Determines the MIRU lineages for TB amplicons
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and function headers
'   o fun01: pversion_tbMiru
'     - prints verision number for tbMiru
'   o fun02: phelp_tbMiru
'     - prints help message for tbMiru
'   o fun03: input_tbMiru
'     - get user input
'   o main:
'     - run tbMiru (driver function)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef plan9
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

#include "../genFreezeTB/miruTbl.h"
#include "../genFreezeTB/miruFx.h"

/*.h file only*/
#include "../ftbVersion.h"
#include "../genLib/endLine.h"
#include "../genFreezeTB/tbMiruDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/endin.h"
!   o .c  #include "../genLib/checkSum.h"
!   o .c  #include "../genLib/inflate.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/memWater.h"
!   o .h  #include "../genLib/strAry.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
!   o .h  #include "../genBio/kmerBit.h"
!   o .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_phelp_tbMiru 1
#define def_pversion_tbMiru 2
#define nonNum_tbMiru 4
#define def_genome_tbMiru 0

#define def_samIn_tbMiru 0
#define def_fqIn_tbMiru 1
#define def_faIn_tbMiru 2

/*-------------------------------------------------------\
| Fun01: pversion_tbMiru
|   - prints verision number for tbMiru
| Input:
|   - outFILE:
|     o file to print the version number to
| Output:
|   - Prints:
|     o version number message to outFILE
\-------------------------------------------------------*/
void
pversion_tbMiru(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "tbMiru from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_tbMiru*/

/*-------------------------------------------------------\
| Fun02: phelp_tbMiru
|   - prints help message for tbMiru
| Input:
|   - outFILE:
|     o file to print the help message
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_tbMiru(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: phelp_tbMiru
   '   - prints help message for tbMiru
   '   o fun02 sec01;
   '     - print usage entry
   '   o fun02 sec02;
   '     - print input options
   '   o fun02 sec03;
   '     - print output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "tbMiru -sam reads.sam -miru-tbl MIRU-table.tsv%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "tbMiru -sam consensuses.sam"
   );
   fprintf(
      outFILE,
     " -miru-tbl MIRU-table.tsv%s",
     str_endLine
   );

   fprintf(
      outFILE,
      "  - Finds MIRU lineage for input reads/consensuses"
   );

   fprintf(outFILE, "%s", str_endLine);


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input options
   ^   o fun02 sec02 sub01:
   ^     - print input header
   ^   o fun02 sec02 sub02:
   ^     - input reads/sequences
   ^   o fun02 sec02 sub03:
   ^     - input primer sequences
   ^   o fun02 sec02 sub04:
   ^     - fudge (length) entry
   ^   o fun02 sec02 sub05:
   ^     - miru table entry
   ^   o fun02 sec02 sub07:
   ^     - each line is a genome flag
   ^   o fun02 sec02 sub07:
   ^     - out file entry
   ^   o fun02 sec02 sub08:
   ^     - out table file entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /**************************************************\
   * Fun02 Sec02 Sub01:
   *   - print input header
   \**************************************************/

   fprintf(outFILE, "Input:%s", str_endLine);

   /**************************************************\
   * Fun02 Sec02 Sub02:
   *   - input reads/sequences
   \**************************************************/

   fprintf(
      outFILE,
      "  -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o Sam file with mapped reads to find lineages"
   );
   fprintf(outFILE, " for%s", str_endLine);

   fprintf(
     outFILE,
     "    o minimap2 -a ref.fasta reads.fastq > reads.sam"
   );
   fprintf(outFILE, "%s", str_endLine);

   fprintf(
     outFILE,
     "    o minimap2 -a ref.fa consensuses.fa >"
   );
   fprintf(outFILE, " conesnuses.sam%s", str_endLine);


   fprintf(
      outFILE,
      "  -fq reads.fastq: [Optional; Replaces -sam]%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o fastq file with reads (replaces -sam)%s",
      str_endLine
   );


   fprintf(
       outFILE,
       "  -fa sequence.fasta: [Optional; Replaces -sam]%s",
      str_endLine
    );

   fprintf(
     outFILE,
     "    o fasta file with sequences (replaces -sam)%s",
      str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub03:
   *   - input primer sequences
   \**************************************************/

   fprintf(
       outFILE,
       "  -prim primers.tsv: [Required for -fa or -fq]%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "    o tsv file with primers (1st row is header)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o columns in tsv file:%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      - column 1 is primer id%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "        * names must be names used in -miru-tbl%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      - column 2 paired (Yes) or unpaired (No)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        primers (column 2 is not used in tbMiru)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "     - column 3 forward primer sequence%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "     - column 4 reverse primer sequence%s",
      str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub04:
   *   - fudge (length) entry
   \**************************************************/

   fprintf(
      outFILE,
      "  -fudge: [%i]%s",
      def_fudgeLen_tbMiruDefs,
      str_endLine
   );

   fprintf(
     outFILE,
     "    o Range to fudge lengths by to call lineage%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "    o Lineage length range is + or - fudge%s",
     str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub05:
   *   - miru table entry
   \**************************************************/

   fprintf(
      outFILE,
      "  -miru-tbl: [Required]%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o tsv with the MIRU table to find lineages"
   );
   fprintf(outFILE, " with%s ", str_endLine);

   fprintf(
     outFILE,
     "   - Format: %s",
     str_endLine
   );

   fprintf(
     outFILE,
     "      o The first column is the lineage name%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "      o Remaining columns are for each primer%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "      o Header has primer name and coordinates%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "        - Primer: name.forward.len.reverse.len%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "          i. name: Name of the primer%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "          ii. forward: Forward primer start%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "          iii. len: Length of forward primer%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "          iv. reverse: Reverse primer position%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "          v. len: Length of reverse primer%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "      o Each row (non-header) has the length %s",
      str_endLine
   );

   fprintf(
     outFILE,
     "        needed for a read to match an lineage;%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "        this includes the foward primer, but%s",
     str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub06:
   *   - each line is a genome flag
   \**************************************************/

   fprintf(
     outFILE,
     "        not the reverse primer%s",
     str_endLine
   );

   if(def_genome_tbMiru)
      fprintf(
         outFILE,
        " -genome: [Optional; Yes]%s",
        str_endLine
      );
   else
      fprintf(
         outFILE,
        " -genome: [Optional; No]%s",
        str_endLine
      );

   fprintf(
      outFILE,
      "  - each sequence in file is an entire genome%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "  - -out is always set to stdout%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "  - disable with `-no-genome`%s",
      str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub07:
   *   - out file entry
   \**************************************************/

   fprintf(outFILE, "  -out: [stdout]%s", str_endLine);

   fprintf(
     outFILE,
     "    o File to output each primers lineages to%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "    o Use `-` for stdout%s",
     str_endLine
   );

   /**************************************************\
   * Fun02 Sec02 Sub08:
   *   - out table file entry
   \**************************************************/

   fprintf(
      outFILE,
      "  -out-tbl: [do not output]%s",
      str_endLine
   );

   fprintf(
     outFILE,
     "    o File to print filled in MIRU tsv table to%s",
     str_endLine
   );

   fprintf(
     outFILE,
     "    o Use `-` for stdout%s",
     str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:%s", str_endLine);

   fprintf(
      outFILE,
      "  - Miru table with the number of reads that%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    mapped to each lineage and primer (-out-tbl)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - The most supported MIRU lineage for each"
   );

   fprintf(
      outFILE,
      " primer%s",
      str_endLine
   );
} /*phelp_tbMiru*/

/*-------------------------------------------------------\
| Fun03: input_tbMiru
|   - get user input
| Input:
|   - numArgsSI:
|     o Number of arguments the user input
|   - argAryStr:
|     o array of c-strings with user input
|   - samFileStr:
|     o pointer to c-string to hold the sam file
|   - fxFlagSC:
|     o pointer to signed char to be set to input file
|       type
|   - miruFileStr:
|     o pointer to c-string with the miru table to use
|       in assinging lineages
|   - outFileStr:
|     o pointer to c-string to hold the output flie
|   - tblOutFileStr:
|     o pointer to c-string to hold the miru table output
|   - fudgeLenSI:
|     o pointer to integer with difference in allowed
|       amplicon lengths
|   - genomeBlPtr:
|     o pointer to signed char, set to 1 if user wanted
|       multi-fasta genome mode
| Output:
|   - Modifies:
|     o all input variables, except numArgsSI and
|       argAryStr to hold user input
|   - Returns:
|     o 0 for success
|     o 1 if printed help messge or version number
|     o 2 for errors
\-------------------------------------------------------*/
signed char
input_tbMiru(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStr,
   signed char *fxFlagSC,
   signed char **miruFileStr,
   signed char **primFileStr,
   signed char **outFileStr,
   signed char **tblOutFileStr,
   signed int *fudgeLenSI,
   signed char *genomeBlPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec04:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if user provided input
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return if had error or not
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if user provided input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no input provided*/
      phelp_tbMiru(stdout);
      goto phelp_fun03_sec04_sub02;
   } /*If: no input provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - input files + start loop
   ^   o fun03 sec03 sub02:
   ^     - output files
   ^   o fun03 sec03 sub03:
   ^     - misc settings (fudge length only)
   ^   o fun03 sec03 sub04:
   ^     - help message requested?
   ^   o fun03 sec03 sub05:
   ^     - version number requested?
   ^   o fun03 sec03 sub06:
   ^     - invalid input
   ^   o fun03 sec03 sub07:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - input files + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: sam file input*/
         ++siArg;
         *fxFlagSC = def_samIn_tbMiru;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-fq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: fastq file input*/
         ++siArg;
         *fxFlagSC = def_fqIn_tbMiru;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: fastq file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-fa",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: fasta file input*/
         ++siArg;
         *fxFlagSC = def_faIn_tbMiru;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: fasta file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-miru-tbl",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: miru table file input*/
         ++siArg;
         *miruFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: miru table file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-prim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: miru table file input*/
         ++siArg;
         *primFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: miru table file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-genome",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) *genomeBlPtr = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-genome",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) *genomeBlPtr = 0;

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - output files
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: output file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-out-tbl",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: output table file input*/
         ++siArg;
          *tblOutFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: output table file input*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - misc settings (fudge length only)
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-fudge",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: fudge length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[siArg],
               fudgeLenSI
            );

         if(*tmpStr)
         { /*If: non-numeric/to large*/
             fprintf(
                stderr,
                "-fudge %s is non-numeric or to large%s",
                argAryStr[siArg],
                str_endLine
             );

             goto err_fun03_sec04_sub02;
         } /*If: non-numeric/to large*/
      } /*Else If: fudge length*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - help message requested?
      \**************************************************/

      else if(
          ! eql_charCp(
             (signed char *) "-h",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: help message requested*/
          phelp_tbMiru(stdout);
          goto phelp_fun03_sec04_sub02;
       } /*Else If: help message requested*/

      else if(
          ! eql_charCp(
             (signed char *) "--h",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: help message requested*/
          phelp_tbMiru(stdout);
          goto phelp_fun03_sec04_sub02;
       } /*Else If: help message requested*/

      else if(
          ! eql_charCp(
             (signed char *) "help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: help message requested*/
          phelp_tbMiru(stdout);
          goto phelp_fun03_sec04_sub02;
       } /*Else If: help message requested*/

      else if(
          ! eql_charCp(
             (signed char *) "-help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: help message requested*/
          phelp_tbMiru(stdout);
          goto phelp_fun03_sec04_sub02;
       } /*Else If: help message requested*/

      else if(
          ! eql_charCp(
             (signed char *) "--help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: help message requested*/
          phelp_tbMiru(stdout);
          goto phelp_fun03_sec04_sub02;
       } /*Else If: help message requested*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - version number requested?
      \**************************************************/

      else if(
          ! eql_charCp(
             (signed char *) "-v",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: version number requested*/
          pversion_tbMiru(stdout);
          goto pversion_fun03_sec04_sub02;
       } /*Else If: version number requested*/

      else if(
          ! eql_charCp(
             (signed char *) "--v",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: version number requested*/
          pversion_tbMiru(stdout);
          goto pversion_fun03_sec04_sub02;
       } /*Else If: version number requested*/

      else if(
          ! eql_charCp(
             (signed char *) "version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: version number requested*/
          pversion_tbMiru(stdout);
          goto pversion_fun03_sec04_sub02;
       } /*Else If: version number requested*/

      else if(
          ! eql_charCp(
             (signed char *) "-version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: version number requested*/
          pversion_tbMiru(stdout);
          goto pversion_fun03_sec04_sub02;
       } /*Else If: version number requested*/

      else if(
          ! eql_charCp(
             (signed char *) "--version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
       ){ /*Else If: version number requested*/
          pversion_tbMiru(stdout);
          goto pversion_fun03_sec04_sub02;
       } /*Else If: version number requested*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
          fprintf(
             stderr,
             "%s not recognized%s",
             argAryStr[siArg],
             str_endLine
          );

         goto err_fun03_sec04_sub02;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return if had error or not
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04_sub02;

   pversion_fun03_sec04_sub02:;
   phelp_fun03_sec04_sub02:;
   errSC = 1;
   goto ret_fun03_sec04_sub02;

   err_fun03_sec04_sub02:;
   errSC = 2;
   goto ret_fun03_sec04_sub02;

   ret_fun03_sec04_sub02:;
   return errSC;
} /*input_tbMiru*/

/*-------------------------------------------------------\
| Main:
|   - driver function for tbMiru
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input arguments
| Output:
|   - Prints:
|     o lineages or table to output file
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for tbMiru
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get and check user input
   '   o main sec03:
   '     - read in miru table
   '   o main sec04:
   '     - fill in miru table
   '   o main sec06:
   '     - print MIRU lineage/table
   '   o main sec07:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *samFileStr = 0;
   signed char *miruFileStr = 0;
   signed char *outFileStr = 0;
   signed char *tblOutFileStr = 0;

   signed int fudgeLenSI = def_fudgeLen_tbMiruDefs;
   signed char genomeBl = def_genome_tbMiru;

   signed char errSC = 0;

   struct miruTbl *miruHeapST = 0;

   FILE *samFILE = 0;
   struct samEntry samStackST;
   unsigned long ulEntry = 0;

   /*unique to kmer find*/
   signed char *primFileStr = 0;
   signed char fxFlagSC = def_samIn_tbMiru;
   signed char *fxTypeStr = 0;

   signed int maxAmpLenSI = def_maxAmpLen_miruFx;
   float minScorePercF = def_minPercScore_miruFx;
   float minKmerPercF = def_minKmerPerc_kmerFind;
   float percShiftF = def_percShift_kmerFind;
   float extraNtPercF = def_extraNtInWin_kmerFind;
   unsigned char lenKmerUC = def_lenKmer_kmerFind;

   struct seqST seqStackST;

   struct refST_kmerFind *refHeapAryST = 0;
   signed int lenRefSI = 0;

   struct tblST_kmerFind tblStackST;
   struct alnSet alnSetStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize variables and get user input
   ^   o main sec02 sub01:
   ^     - initialzie variables
   ^   o main sec02 sub02:
   ^     - get user input
   ^   o main sec02 sub03:
   ^     - open sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialzie variables
   \*****************************************************/

   init_samEntry(&samStackST);
   init_seqST(&seqStackST);
   init_alnSet(&alnSetStackST);
   init_tblST_kmerFind(&tblStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - Get the user input
   \*****************************************************/

   errSC =
      input_tbMiru(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &fxFlagSC,
         &miruFileStr,
         &primFileStr,
         &outFileStr,
         &tblOutFileStr,
         &fudgeLenSI,
         &genomeBl
      ); /*Get the user input*/

   if(errSC)
   { /*If: had input error*/
      --errSC; /*convert help/version prints to no error*/
      goto cleanUp_main_sec07_sub03;
   } /*If: had input error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: user provided an file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open the sam file*/
         fprintf(
            stderr,
            "could not open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto err_main_sec07_sub02;
      } /*If: could not open the sam file*/
   } /*Else: user provided an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in miru table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fxFlagSC != def_samIn_tbMiru)
      goto fxLineages_main_sec05_sub01;
      /*not a sam file, need differnt functions*/

   miruHeapST =
      get_miruTbl(
         miruFileStr,
         &errSC
      );

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_fileErr_tbMiruDefs)
         fprintf(
            stderr,
            "Could not open -miru -tbl %s%s",
            miruFileStr,
            str_endLine
          );

      else if(errSC == def_memErr_tbMiruDefs)
         fprintf(
            stderr,
            "Ran out of memory%s",
            str_endLine
         );

      else
         fprintf(
            stderr,
            "Error while reading -miru-tbl %s%s",
            miruFileStr,
            str_endLine
         );

      goto err_main_sec07_sub02;
   } /*If: had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - fill in miru table
   ^   o main sec04 sub01:
   ^     - read in first entry (line) of same file
   ^   o main sec04 sub02:
   ^     - find number reads mapped to each lineage
   ^   o main sec04 sub03:
   ^     - check for errors and minor clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - read in first entry (line) of same file
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct%s",
         str_endLine
      );

      goto err_main_sec07_sub02;
   } /*If: memory error*/

   errSC = get_samEntry(&samStackST, samFILE);

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - find number reads mapped to each lineage
   \*****************************************************/

   while(! errSC)
   { /*Loop: find lineages*/
      ++ulEntry;

      if(! samStackST.seqStr)
         goto nextEntry_main_sec04_sub02;

      if(*samStackST.seqStr == '*')
         goto nextEntry_main_sec04_sub02;

      if(*samStackST.seqStr == '\0')
         goto nextEntry_main_sec04_sub02;

      /*check unammped reads (4), secondary alingments
      `  (256), and supplemental alignments (2048)
      */
      if(samStackST.flagUS & (4 | 256 | 2048))
         goto nextEntry_main_sec04_sub02;

      incLineage_miruTbl(
         &samStackST,
         fudgeLenSI,
         miruHeapST
      ); /*find lineages*/

      nextEntry_main_sec04_sub02:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: find lineages*/

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - check for errors and minor clean up
   \*****************************************************/

   if(errSC != def_EOF_samEntry)
   { /*If: had an error*/
      fprintf(
         stderr,
         "memory error read line %lu in -sam %s%s",
         ulEntry,
         samFileStr,
         str_endLine
      );

      goto err_main_sec07_sub02;
   } /*If: had an error*/

   freeStack_samEntry(&samStackST);

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   goto pLineages_main_sec06_sub01;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - find lineages in fastx files
   ^   o main sec05 sub01:
   ^     - read in tables
   ^   o main sec05 sub02:
   ^     - get first fx entry
   ^   o main sec05 sub03:
   ^     - find lineages + start loop
   ^   o main sec05 sub04:
   ^     - check for file errors
   ^   o main sec05 sub05:
   ^     - minor clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - read in tables
   \*****************************************************/

   fxLineages_main_sec05_sub01:;

   setup_tblST_kmerFind(
      &tblStackST,
      lenKmerUC
   );

   miruHeapST =
      kmerFindSetup_miruTbl(
         miruFileStr,
         primFileStr,
         lenKmerUC,
         &refHeapAryST,
         &lenRefSI,
         &tblStackST,
         minKmerPercF,
         extraNtPercF,
         percShiftF,
         &alnSetStackST,
         &errSC
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_fileErr_tbMiruDefs)
         fprintf(
            stderr,
            "file error reading -prim %s%s",
            primFileStr,
            str_endLine
         );

      else
         fprintf(
            stderr,
            "memory error reading -prim %s%s",
            primFileStr,
            str_endLine
         );

      goto err_main_sec07_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - get first fx entry
   \*****************************************************/

   if(fxFlagSC == def_fqIn_tbMiru)
   { /*If: fastq file input*/
      errSC = getFq_seqST(samFILE, &seqStackST);
      fxTypeStr = (signed char *) "-fq";
   } /*If: fastq file input*/

   else
   { /*Else: fasta file input*/
      errSC = getFa_seqST(samFILE, &seqStackST);
      fxTypeStr = (signed char *) "-fa";
   } /*Else: fasta file input*/

   if(errSC)
   { /*If: error*/
      if(errSC == def_EOF_seqST)
         fprintf(
            stderr,
            "nothing in %s %s%s",
            fxTypeStr,
            samFileStr,
            str_endLine
         );

      else if(errSC == def_memErr_seqST)
          fprintf(
            stderr,
            "memory error for first entry in %s %s%s",
            fxTypeStr,
            samFileStr,
            str_endLine
         );

      else
          fprintf(
            stderr,
            "bad first entry in %s %s%s",
            fxTypeStr,
            samFileStr,
            str_endLine
         );

      goto err_main_sec07_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - find lineages + start loop
   \*****************************************************/

   while(! errSC)
   { /*Loop: find lineages*/
      ++ulEntry;

      errSC =
         kmerFind_miruFx(
            miruHeapST,
            &tblStackST,
            refHeapAryST,
            fudgeLenSI,
            &seqStackST,
            minScorePercF,
            maxAmpLenSI,
            &alnSetStackST
         );

      if(errSC == 2)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error on read %lu in %s %s%s",
            ulEntry,
            fxTypeStr,
            samFileStr,
            str_endLine
         );

         goto err_main_sec07_sub02;
      } /*If: memory error*/

      /*get next sequence*/

      if(fxFlagSC == def_fqIn_tbMiru)
         errSC = getFq_seqST(samFILE, &seqStackST);

      else
         errSC = getFa_seqST(samFILE, &seqStackST);

      if(genomeBl)
      { /*If: each sequence is an entire genome*/
         plineages_miruTbl(miruHeapST, 0);
           /*not using stdout (0) will overwrite*/
         resetCnt_miruTbl(miruHeapST);
           /*clear this genomes results*/
      } /*If: each sequence is an entire genome*/
   } /*Loop: find lineages*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - check for file errors
   \*****************************************************/

   if(errSC != def_EOF_seqST)
   { /*If: error*/
      if(errSC == def_memErr_seqST)
          fprintf(
            stderr,
            "memory error for entry %lu in %s %s%s",
            ulEntry,
            fxTypeStr,
            samFileStr,
            str_endLine
         );

      else
          fprintf(
            stderr,
            "bad entry (number %lu) in %s %s%s",
            ulEntry,
            fxTypeStr,
            samFileStr,
            str_endLine
         );

      goto err_main_sec07_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec05 Sub05:
   *   - minor clean up
   \*****************************************************/

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   freeHeapAry_refST_kmerFind(
      refHeapAryST,
      lenRefSI
   );

   refHeapAryST = 0;
   lenRefSI = 0;

   freeStack_tblST_kmerFind(&tblStackST);
   freeStack_seqST(&seqStackST);

   if(genomeBl)
      goto done_main_sec07;
      /*already printed results*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - print in MIRU lineage/table
   ^   o main sec06 sub01:
   ^     - print MIRU table for reads
   ^   o main sec06 sub02:
   ^     - print MIRU lineage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - print MIRU table for reads
   \*****************************************************/

   pLineages_main_sec06_sub01:;

   if(tblOutFileStr)
   { /*If: am  printing out the read table*/
      errSC =
         p_miruTbl(
            miruHeapST,
            tblOutFileStr
         );

      if(errSC)
      { /*If: could not open output file*/
         if(outFileStr)
            fprintf(
               stderr,
               "Could not write to -out-tbl %s%s",
               outFileStr,
               str_endLine
            );

         else
            fprintf(
               stderr,
               "Could not write to stdout (-out-tbl -)%s",
               str_endLine
            );

         goto err_main_sec07_sub02;
      } /*If: could not open output file*/
   } /*If: am  printing out the read table*/

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - print MIRU lineage
   \*****************************************************/

   errSC =
      plineages_miruTbl(
         miruHeapST,
         outFileStr
      );

   if(errSC)
   { /*If: could not open the output file*/
      if(outFileStr)
         fprintf(
            stderr,
            "Could not write to -out %s%s",
            outFileStr,
            str_endLine
         );

      else
         fprintf(
            stderr,
            "Could not write to stdout (-out -)%s",
            str_endLine
         );

      goto err_main_sec07_sub02;
   } /*If: could not open the output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec07:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_main_sec07:;
      errSC = 0;
      goto cleanUp_main_sec07_sub03;

   err_main_sec07_sub02:;
      errSC = 1;
      goto cleanUp_main_sec07_sub03;

   cleanUp_main_sec07_sub03:;
      freeStack_samEntry(&samStackST);

      if(miruHeapST)
         freeHeap_miruTbl(miruHeapST);
      miruHeapST = 0;

      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else
         fclose(samFILE);
      samFILE = 0;

      if(refHeapAryST)
         freeHeapAry_refST_kmerFind(
            refHeapAryST,
            lenRefSI
         );
      refHeapAryST = 0;
      lenRefSI = 0;

      freeStack_tblST_kmerFind(&tblStackST);
      freeStack_alnSet(&alnSetStackST);

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
