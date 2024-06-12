/*########################################################
# Name: filtsam
#  - Filters sam files
#  - This is a little slower than samtools.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and defaults/defined variables
'   o fun01: readCoordsFile
'     - Gets filtering coordinates from an input file
'   o fun02: getFiltSamInput
'     - Gets user input from argsAryStr
'   o fun03: pHelp_filtsam
'     - Prints the help message for filtsam
'   o fun04: pversion_filtsam
'     - Prints out the version number for filtsam
'   o main:
'     - Filters sam files and converts sam output to the
'       desiered format
'   o license:
'     - Licensing for this code (public domain / mit)
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
#include "../generalLib/samEntryStruct.h"
#include "../generalLib/trimSam.h"

/*These do not have .c files*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependencies
!   o .h #include "../generalLib/numToStr.h"
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
#define defSamPSam 0
#define defSamPFastq 1
#define defSamPFasta 2
#define defSamPStats 3

/*Default output type*/
#define defDefaultOut defSamPSam

/*print out anonymous counts when doing an stats print*/
#define def_nPrint_filtsam 1 /*1: yes, 0 is no*/

/*Version number*/
#define def_year_filtsam 2024
#define def_month_filtsam 5
#define def_day_filtsam 14

/*-------------------------------------------------------\
| Fun01: readCoordsFile
|   - Gets filtering coordinates from an input file
| Input:
|   - fileStr:
|     o Path to file to get coordinates from
|   - startPosArySI:
|     o integer array to add starting filter coordinates 
|       to
|   - endPosArySI:
|     o integer array to add ending filter coordinates 
|       to
|   - numCoordsSI:
|     o pointer to integer with number of coordinates in
|       startPosArySI and endPosArySI
| Output:
|   - Modifies:
|     o startPosArySI to have starting coordinates
|     o endPosArySI to have ending coordinates
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
   signed int *startPosArySI,
   signed int *endPosArySI,
   signed int *numCoordsSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC
   '   - Gets filtering coordinates from an input file
   '   o fun01 sec01:
   '     - Variable declerations
   '   o fun01 sec02:
   '     - Open input file
   '   o fun01 sec03:
   '     - Read in the coordaintes
   '   o fun01 sec04:
   '     - Clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint lenBuffSI = 1024;
   sint lineSI = 0;
   sint errSI = 0;

   schar buffStr[lenBuffSI];
   schar *tmpStr = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - Open input file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inFILE = fopen((char *) fileStr, "r");

   if(! inFILE)
   { /*If: I could not open the file*/
      errSI = 1;
      goto cleanUp;
   } /*If: I could not open the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - Read in the coordaintes
   ^   o fun01 sec03 sub01:
   ^     - Get past header and start loop
   ^   o fun01 sec03 sub02:
   ^     - Get past the ignored entry
   ^   o fun01 sec03 sub03:
   ^     - Read in the starting coordiante
   ^   o fun01 sec03 sub04:
   ^     - Read in the ending coordiante
   ^   o fun01 sec03 sub05:
   ^     - Convert to index 0 and move to next coordinate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - Get past header and start loop
   \*****************************************************/

   /*Skip the header*/
   tmpStr =
      (schar*)
      fgets((char *) buffStr, lenBuffSI, inFILE);

   ++lineSI;

   while(fgets((char *) buffStr, lenBuffSI, inFILE))
   { /*Loop: Read in the file*/
      tmpStr = buffStr;

      /**************************************************\
      * Fun01 Sec03 Sub02:
      *   - Get past the ignored entry
      \**************************************************/

      while(*tmpStr++ > 32) ;

      if(*tmpStr == '\0' || *tmpStr == '\n')
      { /*If: The user input no coordiantes*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp;
      } /*If: The user input no coordiantes*/
      
      while(*tmpStr++ < 33)
      { /*Loop: Until I am off the ignored entry*/
         if(*tmpStr == '\0')
            break;
      } /*Loop: Until I am off the ignored entry*/

      if(*tmpStr == '\0' || *tmpStr == '\n')
      { /*If: The user input no coordiantes*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp;
      } /*If: The user input no coordiantes*/

      --tmpStr;

      /**************************************************\
      * Fun01 Sec03 Sub03:
      *   - Read in the starting coordiante
      \**************************************************/

      tmpStr = (schar *)
         base10StrToSI(
            (char *) tmpStr,
            startPosArySI[*numCoordsSI]
         );

      if(*tmpStr == '\0' || *tmpStr == '\n')
      { /*If: The user only input one coordinate*/
         errSI = (lineSI << 8) | 4;
         goto cleanUp;
      } /*If: The user only input one coordinate*/

      if(*tmpStr > 33)
      { /*If: is an non-numeric entry*/
         errSI = (lineSI << 8) | 2;
         goto cleanUp;
      } /*If: is an non-numeric entry*/

      ++tmpStr;

      /**************************************************\
      * Fun01 Sec03 Sub04:
      *   - Read in the ending coordiante
      \**************************************************/

      tmpStr = (schar *)
         base10StrToSI(
            (char *) tmpStr,
            endPosArySI[*numCoordsSI]
         );
      
      if(*tmpStr != '\0' && *tmpStr != '\n')
      { /*If: This was non-numeric format*/
         errSI = (lineSI << 8) | 2;
         goto cleanUp;
      } /*If: This was non-numeric format*/

      /**************************************************\
      * Fun01 Sec03 Sub05:
      *   - Convert to index 0 and move to next coordinate
      \**************************************************/

      /*Convert to index 0*/
      --(startPosArySI[*numCoordsSI]);
      --(endPosArySI[*numCoordsSI]);

      ++(*numCoordsSI);
   } /*Loop: Read in the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp:;

   if(inFILE)
      fclose(inFILE);

   inFILE = 0;
   return errSI;
} /*readCoordsFile*/

/*-------------------------------------------------------\
| Fun02: getFiltSamInput
|   - Gets user input from argsAryStr
| Input:
|   - lenArgsI:
|     o Numbe of arguments and parameters the user input
|   - argsAryStr:
|     o Pointer to c-string array with user input
|   - samStr:
|     o Will point to path to the sam file to process
|   - outStr:
|     o Will point to path to the output file
|   - outFlagC:
|     o Will hold the output type (sam/fastq/fasta/stats)
|   - rmAryS:
|     o Will hold flags to remove
|   - numRmFlagsS:
|     o Will hold numbe of flags in rmAryS
|   - keepAryS:
|     o Will hold flags to keep
|   - numKeepFlagsS:
|     o Will hold number of flags in keepAryS
|   - minLenUI:
|     o Will hold the minimum read length
|   - maxLenUI:
|     o Will hold the maximum read length
|   - minmedainQF:
|     o Will hold the minimum median Q-score
|   - minMeanQF:
|     o Will hold the minimum mean Q-score
|   - minMapqUC:
|     o Will hold the minimum mapping quality
|   - minAlnLenUI:
|     o Will hold the minimum aligned length
|   - pNBl:
|     o pointer to char to be set to 1 or 0
|     o 1: user asked to print out anonymous base counts
|     o 0: no anonymous base printing
|   - trimReadsBl:
|     o 1: Trim soft masked regions off of reads
|     o 0: Do not trim reads
|   - startPosArySI:
|     o integer array to add starting filter coordinates 
|       to
|   - endPosArySI:
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
|     o all input variables except lenArgsI and argsAryStr
|       to hold user input.
\-------------------------------------------------------*/
char * getFiltSamInput(
   int lenArgsI,
   char *argsAryStr[],
   schar cmdInStr[],
   char **samStr,
   char **outStr,
   char *outFlagC,
   ushort rmAryS[],
   uchar *numRmFlagsS,
   ushort keepAryS[],
   uchar *numKeepFlagsS,
   uint *minLenUI,
   uint *maxLenUI,
   float *minMedianQF,
   float *minMeanQF,
   uchar *minMapqUC,
   uint *minAlnLenUI,
   schar *pNBl,
   char *trimReadsBl,
   sint *startPosArySI,
   sint *endPosArySI,
   sint *numCoordsSI,
   schar *coordTrimBl
){
   char *parmStr = 0;
   char *cmdStr = (char *) cmdInStr;
   char *tmpStr = 0;
   int iArg = 1;
   sint errSI = 0;

   while(iArg < lenArgsI)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      *cmdStr++ = ' ';
      cmdStr += cCpStrDelim(cmdStr, argsAryStr[iArg], 0);
      
      if(! cStrEql("-sam", parmStr, '\0'))
      { /*If: The user input a sam file*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         *samStr = argsAryStr[iArg];
      } /*If: The user input a sam file*/

      else if(! cStrEql("-out", parmStr, '\0'))
      { /*Else If: The user provided an output file*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         *outStr = argsAryStr[iArg];
      } /*Else If: The user provided an output file*/

      else if(! cStrEql("-out-sam", parmStr, '\0'))
         outFlagC[0] = defSamPSam;

      else if(! cStrEql("-out-fastq", parmStr, '\0'))
         outFlagC[0] = defSamPFastq;

      else if(! cStrEql("-out-fasta", parmStr, '\0'))
         outFlagC[0] = defSamPFasta;

      else if(! cStrEql("-out-stats", parmStr, '\0'))
         outFlagC[0] = defSamPStats;

      else if(parmStr[0] == '-' && parmStr[1] == 'F')
      { /*Else if: this is a removal entry*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUS(
            argsAryStr[iArg],
            rmAryS[*numRmFlagsS]
         );

         ++(*numRmFlagsS);
      } /*Else if: this is a removal entry*/

      else if(parmStr[0] == '-' && parmStr[1] == 'f')
      { /*Else if: this is a removal entry*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUS(
            argsAryStr[iArg],
            keepAryS[*numKeepFlagsS]
         );

         ++(*numKeepFlagsS);
      } /*Else if: this is a removal entry*/

      else if(! cStrEql("-min-len", parmStr, '\0'))
      { /*Else If: the min length was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUI(argsAryStr[iArg], *minLenUI);
      } /*Else If: the min length was input*/

      else if(! cStrEql("-max-len", parmStr, '\0'))
      { /*Else If: the max length was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUI(argsAryStr[iArg], *maxLenUI);
      } /*Else If: the max length was input*/

      else if(! cStrEql("-min-aln-len", parmStr, '\0'))
      { /*Else If: the min aligned length was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUI(argsAryStr[iArg], *minAlnLenUI);
      } /*Else If: the min aligned length was input*/

      else if(! cStrEql("-min-median-q", parmStr, '\0'))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         *minMedianQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(! cStrEql("-min-mean-q", parmStr, '\0'))
      { /*Else If: the min medain Q score was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         *minMeanQF = atof(argsAryStr[iArg]);
      } /*Else If: the min medain Q score was input*/

      else if(! cStrEql("-min-mapq", parmStr, '\0'))
      { /*Else If: the min mapq was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         base10StrToUC(argsAryStr[iArg], *minMapqUC);
      } /*Else If: the min mapq was input*/

      else if(! cStrEql("-p-n", parmStr, 0))
         *pNBl = 1;

      else if(! cStrEql("-no-p-n", parmStr, 0))
         *pNBl = 0;

      else if(! cStrEql("-trim", parmStr, '\0'))
         *trimReadsBl = 1;

      else if(! cStrEql("-no-trim", parmStr, '\0'))
         *trimReadsBl = 0;

      else if(! cStrEql("-coord-trim", parmStr, '\0'))
         *coordTrimBl = 1;

      else if(! cStrEql("-no-coord-trim", parmStr, '\0'))
         *coordTrimBl = 0;

      else if(! cStrEql("-coords", parmStr, '\0'))
      { /*Else If: an target coordinate was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         tmpStr =
            base10StrToSI(
               argsAryStr[iArg],
               startPosArySI[*numCoordsSI]
            );

         if(*tmpStr == '\0')
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "-coords %s is not in start,end format\n",
               argsAryStr[iArg]
            );

            return parmStr;
         } /*If: The user only input one coordinate*/

         if(*tmpStr != ',')
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "-coords %s is not numeric\n",
               argsAryStr[iArg]
            );

            return parmStr;
         } /*If: The user only input one coordinate*/

         ++tmpStr;

         tmpStr =
            base10StrToSI(
               tmpStr,
               endPosArySI[*numCoordsSI]
            );
         
         if(*tmpStr != '\0')
         { /*If: This was non-numeric format*/
            fprintf(
              stderr,
              "-coords %s has non-numeric end coordinate",
              argsAryStr[iArg]
            );

            fprintf(
               stderr,
               "\n"
            );

            return parmStr;
         } /*If: This was non-numeric format*/

         /*Convert to index 0*/
         --(startPosArySI[*numCoordsSI]);
         --(endPosArySI[*numCoordsSI]);

         ++(*numCoordsSI);
      } /*Else If: an target coordinate was input*/

      else if(! cStrEql("-coords-file", parmStr, '\0'))
      { /*Else If: An coordinates file was input*/
         ++iArg;
         *cmdStr++ = ' ';

         cmdStr +=
            cCpStrDelim(cmdStr, argsAryStr[iArg], 0);

         errSI =
            readCoordsFile(
               (schar *) argsAryStr[iArg],
               startPosArySI,
               endPosArySI,
               numCoordsSI
            );

         if(errSI)
         { /*If: I had an error*/
            if(errSI & 1)
            { /*If: I could not open the file*/
               fprintf(
                  stderr, 
                  "could not open -coords-file %s\n",
                   argsAryStr[iArg]
               );

               return parmStr;
            } /*If: I could not open the file*/

            if(errSI & 2)
            { /*If: I could not open the file*/
               fprintf(
                stderr, 
                "non-numeric line (%i) -coords-file %s\n",
                (errSI >> 8),
                argsAryStr[iArg]
               );

               return parmStr;
            } /*If: I could not open the file*/

            fprintf(
               stderr, 
               "One or no coordinates at line %i in",
               (errSI >> 8)
            );

            fprintf(
               stderr,
               " -coords-file %s\n",
               argsAryStr[iArg]
            );

            return parmStr;
         } /*If: I had an error*/
      } /*Else If: An coordinates file was input*/

      else return parmStr;

      ++iArg;
   } /*Loop: Get user input*/

   return 0;
} /*getFiltSamInput*/


/*-------------------------------------------------------\
| Fun03: pHelp_filtsam
|   - Prints the help message for filtsam
| Input:
|   - outFILE:
|     o File to print help message to
| Output:
|   - Prints:
|     o Help message to outFILE
\-------------------------------------------------------*/
void
pHelp_filtsam(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - Usage block of help message
   '   o fun03 sec02:
   '     - Input block of help message
   '   o fun03 sec03:
   '     - Print the output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - Usage block of help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "filtsam -sam reads.sam [options]\n");
   fprintf(outFILE, "  - Filters sam file from user\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - Input block of help message
   ^   o fun03 Sec02 sub01:
   ^     - Print the input header
   ^   o fun03 Sec02 sub02:
   ^     - Print sam file input
   ^   o fun03 Sec02 sub03:
   ^     - Print flag filtering options
   ^   o fun03 Sec02 sub04:
   ^     - Print min, min aligned, and max length options
   ^   o fun03 Sec02 sub05:
   ^     - Print min median Q, mean Q, and mapq options
   ^   o fun03 Sec02 sub06:
   ^     - Print coodinate options
   ^   o fun03 Sec02 sub07:
   ^     - Print output options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 sec02 Sub01:
   *   - Print the input header
   \*****************************************************/

   fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun03 sec02 Sub02:
   *   - Print sam file input
   \*****************************************************/

   fprintf(outFILE,"  -sam: [Required; default stdin]\n");
   fprintf(outFILE, "    o Sam file to filter\n");
   fprintf(outFILE, "    o Use `-sam -` for stdin\n");

   /*****************************************************\
   * Fun03 sec02 Sub03:
   *   - Print flag filtering options
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
   * Fun03 sec02 Sub04:
   *   - Print min, min aligned, and max length options
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
   * Fun03 sec02 Sub05:
   *   - Print min median Q, mean Q, and mapq options
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
   * Fun03 sec02 Sub07:
   *   - Print anonymous base options
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
   * Fun03 sec02 Sub06:
   *   - Print coodinate options
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
   * Fun03 sec02 Sub07:
   *   - Print output options
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

   if(defDefaultOut == defSamPSam)
      fprintf(outFILE, "    -out-sam: [Default]\n");
   else
      fprintf(outFILE, "    -out-sam: [No]\n");

   fprintf(outFILE, "      o Print -out as sam file\n");

   if(defDefaultOut == defSamPFastq)
      fprintf(outFILE, "    -out-fastq: [Default]\n");
   else
      fprintf(outFILE, "    -out-fastq: [No]\n");

   fprintf(outFILE, "      o Print -out as fastq file\n");

   if(defDefaultOut == defSamPFasta)
      fprintf(outFILE, "    -out-fasta: [Default]\n");
   else
      fprintf(outFILE, "    -out-fasta: [No]\n");

   fprintf(outFILE, "      o Print -out as fasta file\n");

   if(defDefaultOut == defSamPStats)
      fprintf(outFILE, "    -out-stats: [Default]\n");
   else
      fprintf(outFILE, "    -out-stats: [No]\n");

   fprintf(outFILE, "      o Print -out as stats file\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - Print the output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");
   fprintf(
      outFILE,
      "  - Prints filtered reads to selected file type\n"
   );
} /*pHelp_filtsam*/

/*-------------------------------------------------------\
| Fun04: pversion_filtsam
|   - Prints out the version number for filtsam
| Input:
|   - outFILE:
|     o File to print the version numbe to
| Output:
|   - Prints;
|     o the version number to outFILE
\-------------------------------------------------------*/
void
pversion_filtsam(
   FILE *outFILE
){

   fprintf(
      outFILE,
      "filtsam version: %i-%02i-%02i\n",
      def_year_filtsam,
      def_month_filtsam,
      def_day_filtsam
   );
} /*pversion_filtsam*/

/*-------------------------------------------------------\
| Main:
|   - Filters sam files and converts sam output to the
|     desiered format
| Input:
| Output:
|   - Prints:
|     o Filtered sam filt to format speficified by
|       "-out-format"
\-------------------------------------------------------*/
int main(
   int lenArgsI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - Filters sam files and converts sam output to the
   '     desiered format
   '   o main sec01:
   '     - Variable declerations
   '   o main sec02:
   '     - Check user input
   '   o main sec03:
   '     - Check user supplied files
   '   o main sec04:
   '     - Filter the user input
   '   o main sec05:
   '     - Clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar cmdInStr[4096];
   schar firstReadBl = 0; /*Marks end of header*/

   char *samStr = 0;
   char *outStr = 0;
   char pFlagC = defDefaultOut;
   schar pNsBl = def_nPrint_filtsam;

   uchar checkFlagUC = 0;
   ushort rmAryS[256];
   uchar numRmFlagsS = 0;
   ushort keepAryS[256];
   uchar numKeepFlagsS = 0;

   char trimReadsBl = def_trimReadsBl_filtsam;

   uint minLenUI = def_minLen_filtsam;
   uint maxLenUI = def_maxLen_filtsam;
   uint minAlnLenUI = def_minAlnLen_filtsam;
   float minMedianQF = def_minMedQ_filtsam;
   float minMeanQF = def_minMeanQ_filtsam;
   uchar minMapqUC = def_minMapq_filtsam;

   /*For coordinate filtering*/
   sint startPosArySI[4096];
   sint endPosArySI[4096];
   sint numCoordsSI = 0;
   sint siPosFilt = 0;
   schar coordTrimBl = def_coordsTrimBl_filtsam;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry samST;

   char errC = 0;    /*Error reporting*/
   char *inStr = 0;  /*Error reporting; getFiltSamInput*/
   char *buffStr = 0;
   ulong lenBuffUL = 0;
   char pHeadBl = 1;  /*For printing a stats tsv file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - Check user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   inStr =
      getFiltSamInput(
         lenArgsI,
         argsAryStr,
         cmdInStr,
         &samStr,
         &outStr,
         &pFlagC,
         rmAryS,
         &numRmFlagsS,
         keepAryS,
         &numKeepFlagsS,
         &minLenUI,
         &maxLenUI,
         &minMedianQF,
         &minMeanQF,
         &minMapqUC,
         &minAlnLenUI,
         &pNsBl,
         &trimReadsBl,
         startPosArySI,
         endPosArySI,
         &numCoordsSI,
         &coordTrimBl
      ); /*Get the user input*/

   if(inStr)
   { /*If: I had an error*/
      if(
            ! cStrEql(inStr, "-h", '\0')
         || ! cStrEql(inStr, "--h", '\0')
         || ! cStrEql(inStr, "help", '\0')
         || ! cStrEql(inStr, "-help", '\0')
         || ! cStrEql(inStr, "--help", '\0')
      ){ /*If: user wanted help message*/
         pHelp_filtsam(stdout);
         exit(0);
      } /*If: user wanted help message*/

      if(
            ! cStrEql(inStr, "-v", '\0')
         || ! cStrEql(inStr, "--v", '\0')
         || ! cStrEql(inStr, "-V", '\0')
         || ! cStrEql(inStr, "--V", '\0')
         || ! cStrEql(inStr, "version", '\0')
         || ! cStrEql(inStr, "-version", '\0')
         || ! cStrEql(inStr, "--version", '\0')
      ){ /*If: user wanted help message*/
         pversion_filtsam(stdout);
         exit(0);
      } /*If: user wanted help message*/

      if(! cStrEql("-coords", inStr, '\0'))
         exit(-1); /*User has error*/

      if(! cStrEql("-coords-file", inStr, '\0'))
         exit(-1); /*User has error*/

      pHelp_filtsam(stderr);
      fprintf(stderr, "%s is not valid\n", inStr);
      exit(-1);
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - Check user supplied files
   ^   o main sec03 sub01:
   ^     - Check the sam file (input)
   ^   o main sec03 sub02:
   ^     - Check the output file (output)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - Check the sam file (input)
   \*****************************************************/

   if(!samStr || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: The user supplied a sam file*/
      samFILE = fopen(samStr, "r");

      if(!samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
             stderr,
             "-sam %s could not be opened\n",
             samStr
         );

         exit(-1);
      } /*If: I could not open the sam file*/
   } /*Else: The user supplied a sam file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - Check the out file (output)
   \*****************************************************/

   if(!outStr || outStr[0] == '-') outFILE = stdout;

   else
   { /*Else: The user supplied a out file*/
      outFILE = fopen(outStr, "w");

      if(!outFILE)
      { /*If: I could not open the out file*/
         fprintf(
             stderr,
             "-out %s could not be opened\n",
             outStr
         );

         fclose(samFILE);
         exit(-1);
      } /*If: I could not open the out file*/
   } /*Else: The user supplied a out file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - Filter the user input
   ^   o main sec04 sub01:
   ^     - Set up samEntry struct and read first line
   ^   o main sec04 sub02:
   ^     - Start loop and check if need to  print header
   ^   o main sec04 sub03:
   ^     - Filter reads by length, Q-scores, and mapq
   ^   o main sec04 sub04:
   ^     - Trim reads (if requested)
   ^   o main sec04 sub05:
   ^     - Check if read has flag user wanted to keep
   ^   o main sec04 sub06:
   ^     - Check if read has flag user wanted to discard
   ^   o main sec04 sub07:
   ^     - Print out kept reads
   ^   o main sec04 sub08:
   ^     - Get the next entry in the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - Set up samEntry struct and read first line
   \*****************************************************/

   if(initSamEntry(&samST))
   { /*If: I had a memory error*/
      freeSamEntryStack(&samST);
      fprintf(stderr, "Memory error\n");

      fclose(samFILE);
      fclose(outFILE);

      exit(-1);
   } /*If: I had a memory error*/

   errC = readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - Start loop and check if need to  print header
   \*****************************************************/

   while(!errC)
   { /*Loop: Read in the sam file*/
      if(samST.extraStr[0] == '@')
      { /*If: I am on an header; check if need to print*/
         if(pFlagC == defSamPSam)
            pSamEntry(&samST,&buffStr,&lenBuffUL,outFILE);

          goto nextLine_main_sec04_sub_08;
      } /*If: I am on an header; check if need to print*/

      if(! firstReadBl && pFlagC == defSamPSam)
      { /*If: I need to print the program header*/
         fprintf(
           outFILE,
           "@PG\tID:filtsam\tPN:filtsam"
         );

         fprintf(
           outFILE,
           "\tVN:%i-%02i-%02i\tCL:filtsam%s\n",
           def_year_filtsam,
           def_month_filtsam,
           def_day_filtsam,
           cmdInStr /*starts with an space*/
         );

         firstReadBl = 1;
      } /*If: I need to print the program header*/

      /**************************************************\
      * Main Sec04 Sub03:
      *   - Filter reads by length, Q-scores, and mapq
      \**************************************************/
         
      if(samST.readLenUI < minLenUI)
         goto nextLine_main_sec04_sub_08;

      if(samST.readLenUI > maxLenUI && maxLenUI > 0)
          goto nextLine_main_sec04_sub_08;
      
      if(samST.medianQF < minMedianQF)
         goto nextLine_main_sec04_sub_08;

      if(samST.meanQF < minMeanQF)
         goto nextLine_main_sec04_sub_08;

      if(samST.mapqUC < minMapqUC)
         goto nextLine_main_sec04_sub_08;

      if(samST.alnReadLenUI < minAlnLenUI)
         goto nextLine_main_sec04_sub_08;

      /**************************************************\
      * Main Sec04 Sub04:
      *   - Trim reads (if requested)
      \**************************************************/

      if(trimReadsBl)
         trimSamEntry(&samST);

      /**************************************************\
      * Main Sec04 Sub05:
      *   - Check if read has flag user wanted to keep
      \**************************************************/

      /*Ideally I would sort by flag and then reduce to
      ` the minimum flag
      */
      for(
         checkFlagUC = 0;
         checkFlagUC < numKeepFlagsS;
         ++checkFlagUC
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(   samST.flagUS & keepAryS[checkFlagUC]
            || samST.flagUS == keepAryS[checkFlagUC]
          ) goto printSam_main_sec04_sub07;
      } /*Loop: Check all keep read flags*/

      if(numKeepFlagsS > 0)
         goto nextLine_main_sec04_sub_08;

      /**************************************************\
      * Main Sec04 Sub06:
      *   - Check if read has flag user wanted to discard
      \**************************************************/

      for(
         checkFlagUC = 0;
         checkFlagUC < numRmFlagsS;
         ++checkFlagUC
      ){ /*Loop: Check all keep read flags*/
         /*The second check is for the 0 flag*/
         if(   samST.flagUS & rmAryS[checkFlagUC]
            || samST.flagUS == rmAryS[checkFlagUC]
          ) goto nextLine_main_sec04_sub_08;
      } /*Loop: Check all keep read flags*/

      /**************************************************\
      * Main Sec04 Sub07:
      *   - Check if read is in user coordinates range
      \**************************************************/

      for(
         siPosFilt = 0;
         siPosFilt < numCoordsSI;
         ++siPosFilt
      ){ /*Loop: See if have requested coordinates*/
         if(
               samST.refStartUI <=startPosArySI[siPosFilt] 
            && samST.refEndUI >= endPosArySI[siPosFilt]
         ){ /*If: I found the coordinates*/
            if(coordTrimBl)
            { /*If: I am trimming to coordiantes*/
               trimByCoords(
                  &samST,
                  startPosArySI[siPosFilt],
                  endPosArySI[siPosFilt]
               );
            } /*If: I am trimming to coordiantes*/

            break;
         } /*If: I found the coordinates*/
      } /*Loop: See if have requested coordinates*/

      if(numCoordsSI > 0 && siPosFilt >= numCoordsSI)
         goto nextLine_main_sec04_sub_08;

      /**************************************************\
      * Main Sec04 Sub07:
      *   - Print out kept reads
      \**************************************************/

      printSam_main_sec04_sub07:;

      switch(pFlagC)
      { /*Switch; decide printing method*/
         case defSamPSam:
            pSamEntry(&samST,&buffStr,&lenBuffUL,outFILE);
            break;

         case defSamPFastq:
            pSamEntryAsFastq(&samST, outFILE);
            break;

         case defSamPFasta:
            pSamEntryAsFasta(&samST, outFILE);
            break;

         case defSamPStats:
            pSamEntryStats(
               &samST,
               &pHeadBl,
               pNsBl,
               outFILE
            );

            break;
      } /*Switch; decide printing method*/

      /**************************************************\
      * Main Sec04 Sub08:
      *   - Get the next entry in the sam file
      \**************************************************/

      nextLine_main_sec04_sub_08:

      errC =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   freeSamEntryStack(&samST);
   free(buffStr);
   fclose(samFILE);
   fclose(outFILE);

   exit(0);
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
