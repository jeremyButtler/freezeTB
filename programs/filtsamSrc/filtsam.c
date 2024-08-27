/*########################################################
# Name: filtsam
#  - Filters sam files
#  - This is a little slower than samtools.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and defaults/defined variables
'   o fun01: pversion_filtsam
'     - prints out the version number for filtsam
'   o fun02: phelp_filtsam
'     - prints the help message for filtsam
'   o fun04: input_filtsam
'     - gets user input from argAryStr
'   o fun03: readCoordsFile
'     - gets filtering coordinates from an input file
'   o main:
'     - filters sam files and converts sam output to the
'       desiered format
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and defaults / defined variables
|   o header sec01:
|     - Included libraries
|   o header sec02:
|     - Default variables or defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
#include "../genLib/trimSam.h"

/*These do not have .c files*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependencies
!   o .c  #include "../genLib/numToStr.h"
!   o .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - Default variables or defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_minLen_filtsam 0
#define def_maxLen_filtsam 0
#define def_minAlnLen_filtsam 0

#define def_minMapq_filtsam 0
#define def_minMedQ_filtsam 0.0f
#define def_minMeanQ_filtsam 0.0f

#define def_trimReadsBl_filtsam 0
#define def_coordsTrimBl_filtsam 0

/*Flags for the printout type*/
#define def_psam_filtsam 0
#define def_pfq_filtsam 1
#define def_pfa_filtsam 2
#define def_pstats_filtsam 3

/*Default output type*/
#define def_pdefault_filtsam def_psam_filtsam

/*print out anonymous counts when doing an stats print*/
#define def_nPrint_filtsam 1 /*1: yes, 0 is no*/

/*Version number*/
#define def_year_filtsam 2024
#define def_month_filtsam 5
#define def_day_filtsam 14

/*-------------------------------------------------------\
| Fun01: pversion_filtsam
|   - prints out the version number for filtsam
| Input:
|   - outFILE:
|     o file to print the version numbe to
| Output:
|   - Prints;
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_filtsam(
   void *outFILE
){

   fprintf(
      (FILE *) outFILE,
      "filtsam version: %i-%02i-%02i\n",
      def_year_filtsam,
      def_month_filtsam,
      def_day_filtsam
   );
} /*pversion_filtsam*/

/*-------------------------------------------------------\
| Fun02: phelp_filtsam
|   - prints the help message for filtsam
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_filtsam(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - usage block of help message
   '   o fun02 sec02:
   '     - input block of help message
   '   o fun02 sec03:
   '     - print the output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block of help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "filtsam -sam reads.sam [options]\n");
   fprintf(outFILE, "  - Filters sam file from user\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block of help message
   ^   o fun02 Sec02 sub01:
   ^     - print the input header
   ^   o fun02 Sec02 sub02:
   ^     - print sam file input
   ^   o fun02 Sec02 sub03:
   ^     - Print flag filtering options
   ^   o fun02 Sec02 sub04:
   ^     - print min, min aligned, and max length options
   ^   o fun02 Sec02 sub05:
   ^     - print min median Q, mean Q, and mapq options
   ^   o fun02 Sec02 sub06:
   ^     - print coodinate options
   ^   o fun02 Sec02 sub07:
   ^     - print output options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 sec02 Sub01:
   *   - print the input header
   \*****************************************************/

   fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun02 sec02 Sub02:
   *   - print sam file input
   \*****************************************************/

   fprintf(outFILE,"  -sam: [Required; default stdin]\n");
   fprintf(outFILE, "    o Sam file to filter\n");
   fprintf(outFILE, "    o Use `-sam -` for stdin\n");

   /*****************************************************\
   * Fun02 sec02 Sub03:
   *   - print flag filtering options
   \*****************************************************/

   fprintf(outFILE, "  Flag filtering:\n");
   fprintf(outFILE, "    -F flag: [Nothing]\n");
   fprintf(
      outFILE,
      "      o Filter out reads with flag\n");

   fprintf(
      outFILE,
      "      o -F can be input multiple times\n"
   );

   fprintf(outFILE, "    -f flag: [Nothing]\n");
   fprintf(
      outFILE,
      "      o Keep only reads with input flags\n"
   );
   fprintf(
      outFILE,
      "      o -f can be input multiple times\n"
   );

   fprintf(outFILE,"    - Some usefull flags\n");
   fprintf(outFILE,"       o 0: forward (not as flag)\n");
   fprintf(outFILE,"       o 4: unmapped read\n");
   fprintf(outFILE,"       o 16: reverse complement\n");
   fprintf(outFILE,"       o 256: secondary alignment\n");
   fprintf(outFILE,"       o 2048: supplemental align\n");

   /*****************************************************\
   * Fun02 sec02 Sub04:
   *   - print min, min aligned, and max length options
   \*****************************************************/

   fprintf(outFILE, "  Length filtering:\n");

   fprintf(
      outFILE,
      "    -min-aln-len length: [%i]\n",
     def_minAlnLen_filtsam
   );

   fprintf(
      outFILE,
      "      o Minimum aligned read length to keep read\n"
   );
   fprintf(
      outFILE,
      "    -min-len length: [%i]\n",
     def_minLen_filtsam
   );

   fprintf(
      outFILE,
      "      o Minimum read length to keep an read\n"
   );

   fprintf(
      outFILE,
      "    -max-len length: [%i]\n",
     def_maxLen_filtsam
   );

   fprintf(
      outFILE,
      "      o Maximum read length to keep an read\n"
   );

   /*****************************************************\
   * Fun02 sec02 Sub05:
   *   - print min median Q, mean Q, and mapq options
   \*****************************************************/

   fprintf(outFILE, "  Q-score filtering:\n");

   fprintf(
      outFILE,
      "    -min-mapq mapq: [%i]\n",
     def_minMapq_filtsam
   );

   fprintf(
      outFILE,
      "      o Minimum mapping quality to keep an read\n"
   );

   fprintf(
      outFILE,
      "    -min-median-q medianQ: [%f]\n",
     def_minMedQ_filtsam
   );
   fprintf(
      outFILE,
      "      o Minimum median Q-score to keep an read\n"
   );

   fprintf(
      outFILE,
      "    -min-mean-q meanQ: [%f]\n",
     def_minMeanQ_filtsam
   );
   fprintf(
      outFILE,
      "      o Minimum mean Q-score to keep an read\n"
   );

   /*****************************************************\
   * Fun02 sec02 Sub07:
   *   - print anonymous base options
   \*****************************************************/

   fprintf(outFILE, "  Stats printing option:\n");

   if(def_nPrint_filtsam)
      fprintf(outFILE, "    -p-n: [Yes]\n");
   else
      fprintf(outFILE, "    -p-n: [No]\n");

   
   fprintf(
      outFILE,
      "      o Print anonymous base counts with"
   );

   fprintf(outFILE, " -out-stats\n");

   fprintf(
      outFILE,
      "      o Disable with `-no-p-n`\n"
   );

   /*****************************************************\
   * Fun02 sec02 Sub06:
   *   - print coodinate options
   \*****************************************************/

   fprintf(outFILE, "  Coordinate filtering:\n");

   if(def_coordsTrimBl_filtsam)
      fprintf(outFILE, "    -coord-trim: [Yes]\n");
   else
      fprintf(outFILE, "    -coord-trim: [No]\n");

   fprintf(
      outFILE,
      "      o Trim read to first -coords x,y matching"
   );

   fprintf(outFILE, " coordinate\n");

   fprintf(
      outFILE,
      "      o Disable with `-no-coord-trim`\n"
   );


   fprintf(
      outFILE,
      "    -coords x,y: [None]\n"
   );

   fprintf(
     outFILE,
     "      o Removes reads not having input coordinate\n"
   );

   fprintf(
      outFILE,
      "      o This can be used up to 4000 times\n"
   );

   fprintf(
      outFILE,
      "    - coords-file coords.tsv: [None]\n"
   );

   fprintf(
      outFILE,
      "      o Read coordaintes (-coords) from an file\n"
   );

   fprintf(
      outFILE,
      "        - First line is an header (ignored)\n"
   );

   fprintf(
      outFILE,
      "        - Remaing lines `ignored\tstart\tend`\n"
   );


   /*****************************************************\
   * Fun02 sec02 Sub07:
   *   - print output options
   \*****************************************************/

   fprintf(outFILE, "  Output options:\n");

   if(def_trimReadsBl_filtsam)
      fprintf(outFILE, "    -trim: [Yes]\n");
   else
      fprintf(outFILE, "    -trim: [No]\n");

   fprintf(
      outFILE,
      "      o Remove (trim) soft masked parts of reads\n"
   );

   fprintf(outFILE, "      o disble with -no-trim\n");

   fprintf(outFILE, "    -out file.ext: [stdout]\n");
   fprintf(outFILE, "      o File to print output to\n");
   fprintf(outFILE, "      o Use `-out -` for stdout\n");

   if(def_pdefault_filtsam == def_psam_filtsam)
      fprintf(outFILE, "    -out-sam: [Default]\n");
   else
      fprintf(outFILE, "    -out-sam: [No]\n");

   fprintf(outFILE, "      o Print -out as sam file\n");

   if(def_pdefault_filtsam == def_pfq_filtsam)
      fprintf(outFILE, "    -out-fastq: [Default]\n");
   else
      fprintf(outFILE, "    -out-fastq: [No]\n");

   fprintf(outFILE, "      o Print -out as fastq file\n");

   if(def_pdefault_filtsam == def_pfa_filtsam)
      fprintf(outFILE, "    -out-fasta: [Default]\n");
   else
      fprintf(outFILE, "    -out-fasta: [No]\n");

   fprintf(outFILE, "      o Print -out as fasta file\n");

   if(def_pdefault_filtsam == def_pstats_filtsam)
      fprintf(outFILE, "    -out-stats: [Default]\n");
   else
      fprintf(outFILE, "    -out-stats: [No]\n");

   fprintf(outFILE, "      o Print -out as stats file\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print the output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");
   fprintf(
      outFILE,
      "  - Prints filtered reads to selected file type\n"
   );
} /*phelp_filtsam*/

/*-------------------------------------------------------\
| Fun03: readCoordsFile
|   - gets filtering coordinates from an input file
| Input:
|   - fileStr:
|     o path to file to get coordinates from
|   - startAryUI:
|     o integer array to add starting filter coordinates 
|       to
|   - endAryUI:
|     o integer array to add ending filter coordinates 
|       to
|   - numCoordsSI:
|     o pointer to integer with number of coordinates in
|       startAryUI and endAryUI
| Output:
|   - Modifies:
|     o startAryUI to have starting coordinates
|     o endAryUI to have ending coordinates
|     o numCoordsSI to include prevois value + number of
|       new coordianates
|   - Returns:
|     o 0 for errors
|     o 1 for invalid file
|     o (line << 8) | 2 for non-numeric
|     o (line << 8) | 4 for incomplete entry
\-------------------------------------------------------*/
signed int
readCoordsFile(
   signed char *fileStr,
   unsigned int *startAryUI,
   unsigned int *endAryUI,
   signed int *numCoordsSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC
   '   - gets filtering coordinates from an input file
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - open input file
   '   o fun03 sec03:
   '     - read in the coordaintes
   '   o fun03 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint lineSI = 0;
   sint errSI = 0;

   #define lenBuffSI 1024
   schar buffStr[lenBuffSI];
   schar *tmpStr = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - open input file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inFILE =
      fopen(
         (char *) fileStr,
         "r"
      );

   if(! inFILE)
   { /*If: I could not open the file*/
      errSI = 1;
      goto cleanUp_fun03_sec04;
   } /*If: I could not open the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - read in the coordaintes
   ^   o fun03 sec03 sub01:
   ^     - get past header and start loop
   ^   o fun03 sec03 sub02:
   ^     - get past the ignored entry
   ^   o fun03 sec03 sub03:
   ^     - read in the starting coordiante
   ^   o fun03 sec03 sub04:
   ^     - read in the ending coordiante
   ^   o fun03 sec03 sub05:
   ^     - convert to index 0 and move to next coordinate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - get past header and start loop
   \*****************************************************/

   /*Skip the header*/
   tmpStr =
      (schar*)
      fgets(
         (char *) buffStr,
         lenBuffSI,
         inFILE
      );

   ++lineSI;

   while(fgets((char *) buffStr, lenBuffSI, inFILE))
   { /*Loop: Read in the file*/
      tmpStr = buffStr;

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - get past the ignored entry
      \**************************************************/

      while(*tmpStr++ > 32) ;

      if(*tmpStr == '\0' || *tmpStr == '\n')
      { /*If: The user input no coordiantes*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp_fun03_sec04;
      } /*If: The user input no coordiantes*/
      
      while(*tmpStr++ < 33)
      { /*Loop: Until I am off the ignored entry*/
         if(*tmpStr == '\0')
            break;
      } /*Loop: Until I am off the ignored entry*/

      if(*tmpStr == '\0' || *tmpStr == '\n')
      { /*If: The user input no coordiantes*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp_fun03_sec04;
      } /*If: The user input no coordiantes*/

      --tmpStr;

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - read in the starting coordiante
      \**************************************************/

      tmpStr +=
         strToUI_base10str(
            tmpStr,
            &startAryUI[*numCoordsSI]
         );

      if(
            *tmpStr == '\0'
         || *tmpStr == '\n'
      ){ /*If: The user only input one coordinate*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp_fun03_sec04;
      } /*If: The user only input one coordinate*/

      if(*tmpStr > 33)
      { /*If: is an non-numeric entry*/
         errSI = (lineSI << 8) | 2;
         goto cleanUp_fun03_sec04;
      } /*If: is an non-numeric entry*/

      ++tmpStr;

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - read in the ending coordiante
      \**************************************************/

      tmpStr +=
         strToUI_base10str(
            tmpStr,
            &endAryUI[*numCoordsSI]
         );
      
      if(
            *tmpStr != '\0'
         && *tmpStr != '\n'
      ){ /*If: This was non-numeric format*/
         errSI = (lineSI << 8) | 2;
         goto cleanUp_fun03_sec04;
      } /*If: This was non-numeric format*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - convert to index 0 and move to next coordinate
      \**************************************************/

      /*Convert to index 0*/
      --startAryUI[*numCoordsSI];
      --endAryUI[*numCoordsSI];

      ++(*numCoordsSI);
   } /*Loop: Read in the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp_fun03_sec04:;

   if(inFILE)
      fclose(inFILE);

   inFILE = 0;
   return errSI;
} /*readCoordsFile*/

/*-------------------------------------------------------\
| Fun04: input_filtsam
|   - gets user input from argAryStr
| Input:
|   - numArgsSI:
|     o number of arguments and parameters the user input
|   - argAryStr:
|     o pointer to c-string array with user input
|   - samStr:
|     o will point to path to the sam file to process
|   - outStr:
|     o will point to path to the output file
|   - outFlagSC:
|     o will hold the output type (sam/fastq/fasta/stats)
|   - rmAryUS:
|     o will hold flags to remove
|   - numRmFlagsUS:
|     o will hold numbe of flags in rmAryUS
|   - keepAryUS:
|     o will hold flags to keep
|   - numKeepFlagsUS:
|     o will hold number of flags in keepAryUS
|   - minLenUI:
|     o will hold the minimum read length
|   - maxLenUI:
|     o will hold the maximum read length
|   - minmedainQF:
|     o will hold the minimum median Q-score
|   - minMeanQF:
|     o will hold the minimum mean Q-score
|   - minMapqUC:
|     o will hold the minimum mapping quality
|   - minAlnLenUI:
|     o will hold the minimum aligned length
|   - pNBl:
|     o pointer to char to be set to 1 or 0
|     o 1: user asked to print out anonymous base counts
|     o 0: no anonymous base printing
|   - trimReadsBl:
|     o 1: trim soft masked regions off of reads
|     o 0: do not trim reads
|   - startAryUI:
|     o integer array to add starting filter coordinates 
|       to
|   - endAryUI:
|     o integer array to add ending filter coordinates 
|       to
|   - numCoordsSI:
|     o pointer to an integer to hold the number of
|       input coordinates
|   - coordTrimBl:
|     o set to 1 if triming reads by filter coordinates
|     o set to 0 if not trimming
| Output:
|   - Modifies:
|     o all input variables except numArgsSI and argAryStr
|       to hold user input.
\-------------------------------------------------------*/
schar
input_filtsam(
   int numArgsSI,
   char *argAryStr[],
   signed char **samStr,
   signed char **outStr,
   signed char *outFlagSC,
   unsigned short rmAryUS[],
   unsigned short *numRmFlagsUS,
   unsigned short keepAryUS[],
   unsigned short *numKeepFlagsUS,
   unsigned int *minLenUI,
   unsigned int *maxLenUI,
   float *minMedianQF,
   float *minMeanQF,
   unsigned char *minMapqUC,
   unsigned int *minAlnLenUI,
   signed char *pNBl,
   signed char *trimReadsBl,
   unsigned int *startAryUI,
   unsigned int *endAryUI,
   signed int *numCoordsSI,
   signed char *coordTrimBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - get user input
   '   o fun04 sec01:
   '     - variable declerations
   '   o fun04 sec02:
   '     - check if anything was input
   '   o fun04 sec03:
   '     - get user input
   '   o fun04 sec04:
   '     - return errors (or no error)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   sint errSI = 0;
   schar errSC = 0;
   schar *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - check if anything was input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no user input*/
      phelp_filtsam(stdout);
      goto phelp_fun04_sec04;
   } /*If: no user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - get user input
   ^   o fun04 sec03 sub01:
   ^     - file io + start loop
   ^   o fun04 sec03 sub02:
   ^     - file output type
   ^   o fun04 sec03 sub03:
   ^     - flag (keep/remove)
   ^   o fun04 sec03 sub04:
   ^     - length filtering
   ^   o fun04 sec03 sub05:
   ^     - quality score filtering
   ^   o fun04 sec03 sub06:
   ^     - booleans (trimming/how to treat masked bases)
   ^   o fun04 sec03 sub07:
   ^     - getting coordiantes (from input)
   ^   o fun04 sec03 sub08:
   ^     - file, get coordiantes
   ^   o fun04 sec03 sub09:
   ^     - help message
   ^   o fun04 sec03 sub10:
   ^     - version number
   ^   o fun04 sec03 sub11:
   ^     - invalid input
   ^   o fun04 sec03 sub12:
   ^     - get next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec03 Sub01:
   *   - file io + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: Get user input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: The user input a sam file*/
         ++siArg;
         *samStr = (schar *) argAryStr[siArg];
      } /*If: The user input a sam file*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: The user provided an output file*/
         ++siArg;
         *outStr = (schar *) argAryStr[siArg];
      } /*Else If: The user provided an output file*/

      /**************************************************\
      * Fun04 Sec03 Sub02:
      *   - file output type
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-out-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *outFlagSC = def_psam_filtsam;

      else if(
         ! eql_charCp(
            (schar *) "-out-fastq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *outFlagSC = def_pfq_filtsam;

      else if(
         ! eql_charCp(
            (schar *) "-out-fasta",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *outFlagSC = def_pfa_filtsam;

      else if(
         ! eql_charCp(
            (schar *) "-out-stats",
            (schar *) argAryStr[siArg],
             (schar) '\0'
         )
      ) *outFlagSC = def_pstats_filtsam;

      /**************************************************\
      * Fun04 Sec03 Sub03:
      *   - flag (keep/remove)
      \**************************************************/

      else if(
            argAryStr[siArg][0] == '-'
         && argAryStr[siArg][1] == 'F'
      ){ /*Else if: this is a removal entry*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUS_base10str(
               (schar *) argAryStr[siArg],
               &rmAryUS[*numRmFlagsUS]
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-F %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/

         ++(*numRmFlagsUS);
      } /*Else if: this is a removal entry*/

      else if(
            argAryStr[siArg][0] == '-'
         && argAryStr[siArg][1] == 'f'
      ){ /*Else if: this is a removal entry*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUS_base10str(
               (schar *) argAryStr[siArg],
               &keepAryUS[*numKeepFlagsUS]
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-f %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/

         ++(*numKeepFlagsUS);
      } /*Else if: this is a removal entry*/

      /**************************************************\
      * Fun04 Sec03 Sub04:
      *   - length filtering
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the min length was input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (schar *) argAryStr[siArg],
               minLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-len %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/
      } /*Else If: the min length was input*/

      else if(
         ! eql_charCp(
            (schar *) "-max-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the max length was input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (schar *) argAryStr[siArg],
               maxLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-max-len %s is non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/
      } /*Else If: the max length was input*/

      else if(
         ! eql_charCp(
            (schar *) "-min-aln-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min aligned length was input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (schar *) argAryStr[siArg],
               minAlnLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-aln-len %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/
      } /*Else If: min aligned length was input*/

      /**************************************************\
      * Fun04 Sec03 Sub05:
      *   - quality score filtering
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-median-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the min medain Q score was input*/
         ++siArg;
         *minMedianQF = atof(argAryStr[siArg]);
      } /*Else If: the min medain Q score was input*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mean-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the min medain Q score was input*/
         ++siArg;
         *minMeanQF = atof(argAryStr[siArg]);
      } /*Else If: the min medain Q score was input*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mapq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the min mapq was input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];   

         tmpStr +=
            strToUC_base10str(
               (schar *) argAryStr[siArg],
               minMapqUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: invalid number*/
      } /*Else If: the min mapq was input*/

      /**************************************************\
      * Fun04 Sec03 Sub06:
      *   - booleans (trimming/how to treat masked bases)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-p-n",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *pNBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-p-n",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *pNBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-trim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *trimReadsBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-trim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *trimReadsBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-coord-trim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *coordTrimBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-coord-trim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *coordTrimBl = 0;

      /**************************************************\
      * Fun04 Sec03 Sub07:
      *   - getting coordiantes (from input)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-coords",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: an target coordinate was input*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               (schar *) argAryStr[siArg],
               &startAryUI[*numCoordsSI]
            );

         if(*tmpStr == '\0')
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "-coords %s is not in start,end format\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: The user only input one coordinate*/

         if(*tmpStr != ',')
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "-coords %s is not numeric\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: The user only input one coordinate*/

         ++tmpStr;

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               &endAryUI[*numCoordsSI]
            );
         
         if(*tmpStr != '\0')
         { /*If: This was non-numeric format*/
            fprintf(
              stderr,
              "-coords %s; end coordiante non-numeric/",
              argAryStr[siArg]
            );

            fprintf(
               stderr,
               "to large\n"
            );

            goto err_fun04_sec04;
         } /*If: This was non-numeric format*/

         /*Convert to index 0*/
         --startAryUI[*numCoordsSI];
         --endAryUI[*numCoordsSI];

         ++(*numCoordsSI);
      } /*Else If: an target coordinate was input*/

      /**************************************************\
      * Fun04 Sec03 Sub08:
      *   - file, get coordiantes
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-coords-file",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: An coordinates file was input*/
         ++siArg;

         errSI =
            readCoordsFile(
               (schar *) argAryStr[siArg],
               startAryUI,
               endAryUI,
               numCoordsSI
            );

         if(errSI)
         { /*If: I had an error*/
            if(errSI & 1)
            { /*If: I could not open the file*/
               fprintf(
                  stderr, 
                  "could not open -coords-file %s\n",
                   argAryStr[siArg]
               );

               goto err_fun04_sec04;
            } /*If: I could not open the file*/

            if(errSI & 2)
            { /*If: I could not open the file*/
               fprintf(
                stderr, 
                "non-numeric line (%i) -coords-file %s\n",
                (errSI >> 8),
                argAryStr[siArg]
               );

               goto err_fun04_sec04;
            } /*If: I could not open the file*/

            fprintf(
               stderr, 
               "One or no coordinates at line %i in",
               (errSI >> 8)
            );

            fprintf(
               stderr,
               " -coords-file %s\n",
               argAryStr[siArg]
            );

            goto err_fun04_sec04;
         } /*If: I had an error*/
      } /*Else If: An coordinates file was input*/

      /**************************************************\
      * Fun04 Sec03 Sub09:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_filtsam(stdout);
         goto phelp_fun04_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_filtsam(stdout);
         goto phelp_fun04_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_filtsam(stdout);
         goto phelp_fun04_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_filtsam(stdout);
         goto phelp_fun04_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_filtsam(stdout);
         goto phelp_fun04_sec04;
      } /*Else If: wanted help message*/

      /**************************************************\
      * Fun04 Sec03 Sub10:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_filtsam(stdout);
         goto pversion_fun04_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_filtsam(stdout);
         goto pversion_fun04_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_filtsam(stdout);
         goto pversion_fun04_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_filtsam(stdout);
         goto pversion_fun04_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_filtsam(stdout);
         goto pversion_fun04_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun04 Sec03 Sub11:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun04_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun04 Sec03 Sub12:
      *   - get next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - return errors (or no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun04_sec04;

   phelp_fun04_sec04:;
   pversion_fun04_sec04:;
   errSC = 1;
   goto ret_fun04_sec04;

   err_fun04_sec04:;
   errSC = 2;
   goto ret_fun04_sec04;

   ret_fun04_sec04:;
   return errSC;
} /*input_filtsam*/

/*-------------------------------------------------------\
| Main:
|   - filters sam files and converts sam output to the
|     desiered format
| Input:
| Output:
|   - Prints:
|     o filtered sam filt to format speficified by
|       "-out-format"
\-------------------------------------------------------*/
int main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - filters sam files and converts sam output to the
   '     desiered format
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - check user input
   '   o main sec03:
   '     - print/skip headers
   '   o main sec04:
   '     - filter sam file
   '   o main sec05:
   '     - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *samStr = 0;
   schar *outStr = 0;
   schar pFlagC = def_pdefault_filtsam;
   schar pNsBl = def_nPrint_filtsam;

   ushort checkFlagUS = 0;
   ushort rmAryUS[256];
   ushort numRmFlagsUS = 0;
   ushort keepAryUS[256];
   ushort numKeepFlagsUS = 0;

   schar trimReadsBl = def_trimReadsBl_filtsam;

   uint minLenUI = def_minLen_filtsam;
   uint maxLenUI = def_maxLen_filtsam;
   uint minAlnLenUI = def_minAlnLen_filtsam;
   float minMedianQF = def_minMedQ_filtsam;
   float minMeanQF = def_minMeanQ_filtsam;
   uchar minMapqUC = def_minMapq_filtsam;

   /*For coordinate filtering*/
   uint startAryUI[4096];
   uint endAryUI[4096];
   sint numCoordsSI = 0;
   sint siPos = 0;
   schar coordTrimBl = def_coordsTrimBl_filtsam;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry samStackST;

   schar errSC = 0;    /*error reporting*/
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   schar pHeadBl = 1;  /*for printing a stats tsv file*/
   ulong ulEntry = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - check user input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get user input
   ^   o main sec02 sub03:
   ^     - open input sam file
   ^   o main sec02 sub04:
   ^     - open provided output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input
   \*****************************************************/

   errSC =
      input_filtsam(
         numArgsSI,
         argAryStr,
         &samStr,
         &outStr,
         &pFlagC,
         rmAryUS,
         &numRmFlagsUS,
         keepAryUS,
         &numKeepFlagsUS,
         &minLenUI,
         &maxLenUI,
         &minMedianQF,
         &minMeanQF,
         &minMapqUC,
         &minAlnLenUI,
         &pNsBl,
         &trimReadsBl,
         startAryUI,
         endAryUI,
         &numCoordsSI,
         &coordTrimBl
      ); /*Get the user input*/

   if(errSC)
   { /*If: I had an error*/
      --errSC; /*reduce help/version print to no error*/
      goto cleanUp_main_sec05_sub04;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open input sam file
   \*****************************************************/

   if(
         ! samStr
      || *samStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: sam file input*/
      samFILE =
         fopen(
            (char *) samStr,
            "r"
         );

      if(!samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
             stderr,
             "-sam %s could not be opened\n",
             samStr
         );

         goto fileErr_main_sec05_sub03;
      } /*If: I could not open the sam file*/
   } /*Else: sam file input*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open provided output file
   \*****************************************************/

   if(
         ! outStr
      || outStr[0] == '-'
   ) outFILE = stdout;

   else
   { /*Else: output file input*/
      outFILE =
         fopen(
            (char *) outStr,
            "w"
         );

      if(!outFILE)
      { /*If: I could not open the out file*/
         fprintf(
             stderr,
             "-out %s could not be opened\n",
             outStr
         );

         goto fileErr_main_sec05_sub03;
      } /*If: I could not open the out file*/
   } /*Else: output file input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print/skip headers
   ^   o main sec04 sub01:
   ^     - set up samEntry struct and read first line
   ^   o main sec04 sub02:
   ^     - print headers in sam file (or get past)
   ^   o main sec04 sub03:
   ^     - print this programs header (sam output only)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - set up samEntry struct and read first line
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );

      goto memErr_main_sec05_sub02;
   } /*If: I had a memory error*/

   errSC =
      get_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print headers in sam file (or get past)
   \*****************************************************/

   while(! errSC)
   { /*Loop: print header*/
      if(samStackST.extraStr[0] != '@')
         break; /*finished headers*/

      if(pFlagC == def_psam_filtsam)
      { /*If: printing sam file out*/
         p_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            0,           /*want a new line*/
            outFILE
         );
      } /*If: printing sam file out*/

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: print header*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - print this programs header (sam output only)
   \*****************************************************/

   if(pFlagC == def_psam_filtsam)
   { /*If: printing program header*/
      fprintf(
        outFILE,
        "@PG\tID:filtsam\tPN:filtsam"
      );

      fprintf(
        outFILE,
        "\tVN:%i-%02i-%02i\tCL:filtsam",
        def_year_filtsam,
        def_month_filtsam,
        def_day_filtsam
      );

      for(
         ulEntry = 1;
         ulEntry < (uint) numArgsSI;
         ++ulEntry
      ){ /*Loop: print input commands*/
         fprintf(
            outFILE,
            " %s",
            argAryStr[minLenUI]
         );
      } /*Loop: print input commands*/

      ulEntry = 0;

      fprintf(
         outFILE,
         "\n"
      );
   } /*If: printing program header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - filter sam file
   ^   o main sec03 sub01:
   ^     - filter by length and q-scores + start loop
   ^   o main sec03 sub02:
   ^     - trim reads (if requested)
   ^   o main sec03 sub03:
   ^     - discard reads without wanted (-f) flags
   ^   o main sec03 sub04:
   ^     - discard reads with unwanted (-F) flags
   ^   o main sec03 sub05:
   ^     - check coorinates of read (against input range)
   ^   o main sec03 sub06:
   ^     - print kept reads
   ^   o main sec03 sub07:
   ^     - get next sam file entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - filter by length and q-scores + start loop
   \*****************************************************/

   while(! errSC)
   { /*Loop: filter reads in sam file*/

         
      if(samStackST.readLenUI < minLenUI)
         goto nextEntry_main_sec04_sub07;

      if(
            samStackST.readLenUI > maxLenUI
         && maxLenUI > 0
      ) goto nextEntry_main_sec04_sub07;
      
      if(samStackST.medianQF < minMedianQF)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.meanQF < minMeanQF)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.mapqUC < minMapqUC)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.alnReadLenUI < minAlnLenUI)
         goto nextEntry_main_sec04_sub07;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - trim reads (if requested)
      \**************************************************/

      if(trimReadsBl)
      { /*If: trimming reads*/
         errSC = trimSeq_trimSam(&samStackST);

         if(errSC)
            goto nextEntry_main_sec04_sub07;
            /*no sequehce to trim*/
      } /*If: trimming reads*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - discard reads without wanted (-f) flags
      \**************************************************/

      /*Ideally I would sort by flag and then reduce to
      ` the minimum flag
      */
      for(
         checkFlagUS = 0;
         checkFlagUS < numKeepFlagsUS;
         ++checkFlagUS
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(
               samStackST.flagUS & keepAryUS[checkFlagUS]
            || samStackST.flagUS == keepAryUS[checkFlagUS]
          ) goto printSam_main_sec04_sub06;
      } /*Loop: Check all keep read flags*/

      if(numKeepFlagsUS > 0)
         goto nextEntry_main_sec04_sub07;

      /**************************************************\
      * Main Sec03 Sub04:
      *   - discard reads with unwanted (-F) flags
      \**************************************************/

      for(
         checkFlagUS = 0;
         checkFlagUS < numRmFlagsUS;
         ++checkFlagUS
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(
               samStackST.flagUS & rmAryUS[checkFlagUS]
            || samStackST.flagUS == rmAryUS[checkFlagUS]
          ) goto nextEntry_main_sec04_sub07;
      } /*Loop: Check all keep read flags*/

      /**************************************************\
      * Main Sec03 Sub05:
      *   - check coorinates of read (against input range)
      \**************************************************/

      for(
         siPos = 0;
         siPos < numCoordsSI;
         ++siPos
      ){ /*Loop: See if have requested coordinates*/
         if(
               samStackST.refStartUI <= startAryUI[siPos] 
            && samStackST.refEndUI >= endAryUI[siPos]
         ){ /*If: I found the coordinates*/
            if(coordTrimBl)
            { /*If: I am trimming to coordiantes*/
               errSC =
                  trimCoords_trimSam(
                     &samStackST,
                     startAryUI[siPos],
                     endAryUI[siPos]
                  );

               if(errSC)
                  goto nextEntry_main_sec04_sub07;
                  /*no sequece to trim/out of range*/
            } /*If: I am trimming to coordiantes*/

            break;
         } /*If: I found the coordinates*/
      } /*Loop: See if have requested coordinates*/

      if(
         numCoordsSI > 0 &&
         siPos >= numCoordsSI
      ) goto nextEntry_main_sec04_sub07;

      /**************************************************\
      * Main Sec03 Sub06:
      *   - print kept reads
      \**************************************************/

      printSam_main_sec04_sub06:;

      switch(pFlagC)
      { /*Switch; decide printing method*/
         case def_psam_filtsam:
         /*Case: printing out a sam file*/
            errSC =
               p_samEntry(
                  &samStackST,
                  &buffHeapStr,
                  &lenBuffUL,
                  0, /*want newline at end*/
                  outFILE
               );

            if(errSC)
            { /*If: had memory error*/
               fprintf(
                  stderr,
                  "memory error on -sam %s read %lu\n",
                  samStr,
                  ulEntry
               );

               goto memErr_main_sec05_sub02;
            } /*If: had memory error*/

            break;

         case def_pfq_filtsam:
            pfq_samEntry(
               &samStackST,
               outFILE
            );

            break;
         /*Case: printing out a sam file*/

         case def_pfa_filtsam:
            pfa_samEntry(
               &samStackST,
               outFILE
            );

            break;

         case def_pstats_filtsam:
            pstats_samEntry(
               &samStackST,
               &pHeadBl,
               pNsBl,
               outFILE
            );

            break;
      } /*Switch; decide printing method*/

      /**************************************************\
      * Main Sec03 Sub07:
      *   - get next sam file entry
      \**************************************************/

      nextEntry_main_sec04_sub07:

      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: filter reads in sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and exit
   ^   o main sec05 sub01:
   ^     - no error clean up
   ^   o main sec05 sub02:
   ^     - memory error clean up
   ^   o main sec05 sub03:
   ^     - file error clean up
   ^   o main sec05 sub04:
   ^     - general clean up (no error/any error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec05_sub02:;
   errSC = 1;
   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_main_sec05_sub03:;
   errSC = 2;
   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - general clean up (no error/any error)
   \*****************************************************/

   cleanUp_main_sec05_sub04:;

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
