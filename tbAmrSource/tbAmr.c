/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries
'   o fun-01: tbAmryGetInput
'     - Gets the user input
'   o fun-02: pTbAmrHelp
'     - Prints the help message for tbAMR
'   o fun-03: pTbAmrVersion
'     - Prints the version number for tbAMR
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

#include "checkAmr.h"
#include "amrStruct.h"

/*Only .h file*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"
#include "tbAmr-version.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden
!   o .c #include "drug_str_ary.h"
!   o .c #include "../generalLib/samEntryStruct.h"
!   o .h #include "../generalLib/codonTbl.h"
!   o .h #include "../generalLib/numToStr.h"
!   o .h #include "../generalLib/base10StrToNum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun-01: tbAmryGetInput
|   - Gets the user input
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argsAryStr:
|     o Arguments the user input
|   - amrTblStr:
|     o Will hold the path to the AMR table to check for
|       antibiotic resitance with
|   - samFileStr:
|     o Will hold the path to the gehoeme to check for
|       antibiotic resitiance
|   - outFileStr:
|     o Will hold the path to the file to output to
|   - idFileStr:
|     o Will point to file to print read ids to
|   - readsBl:
|     o Set to 1 if user input reads (as sam file)
|   - framshiftBl:
|     o Set to 1 (check frameshits) or 0 (do not check)
|   - minDepthUI:
|     o Min read depth the user input
|   - minPerMapF:
|     o Set to the min percent of mapped reads to keep an
|       AMR the user wants
|   - minPercTotalF:
|     o Set to the min percent of reads total reads to
|       keep an AMR the user wants
| Output:
|   - Modifies:
|     o All input variables
|   - Returns:
|     o 0 for success
|     o Pionter to c-string with problematic argument
\-------------------------------------------------------*/
char *
tbAMRGetInput(
   int numArgsI,
   char *argsAryStr[],
   char **amrTblStr,  /*Table with AMR mutations*/
   char **samFileStr,  /*Sequence to check for resitance*/
   char **outFileStr,  /*File to output to*/
   char **idFileStr,   /*Prefix for read id files*/
   char *readsBl,      /*1: user input reads*/
   char *frameshiftBl, /*Tells if checking frameshifts*/
   uint *minDepthUI,   /*Min read depth to keep an amr*/
   float *minPercMapF, /*Min % support to keep amr; read*/
   float *minPercTotalF/*Min % mapped reads to keep*/
){ /*tbAMRGetInput*/

   char *parmStr = 0;
   char *errStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < numArgsI; ++iArg)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      if(cStrEql(parmStr, "-amr-tbl", '\0') == 0)
      { /*If: this is the amr table*/
         ++iArg;
         *amrTblStr = argsAryStr[iArg];
      } /*If: this is the amr table*/

      else if(cStrEql(parmStr, "-sam", '\0') == 0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *samFileStr = argsAryStr[iArg];
         *readsBl = 1;
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-sam-con", '\0') == 0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *samFileStr = argsAryStr[iArg];
         *readsBl = 0;
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-out", '\0') == 0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *outFileStr = argsAryStr[iArg];
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-id-file", '\0') ==0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *idFileStr = argsAryStr[iArg];
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-min-depth", '\0') == 0)
      { /*Else if: tb sequence to check*/
         ++iArg;

         errStr =
           base10StrToSI(argsAryStr[iArg], *minDepthUI);

         if(*errStr > 32) return parmStr;
      } /*Else if: tb sequence to check*/

      else if(!cStrEql(parmStr,"-min-amr-map-perc",'\0'))
      { /*Else if: Minimum map percent input*/
         ++iArg;
         *minPercMapF= atof(argsAryStr[iArg]);
      } /*Else if: Minimum map percent input*/

      else if(
         !cStrEql(parmStr,"-min-total-map-perc",'\0')
      ){ /*Else if: Total map percent input*/
         ++iArg;
         *minPercTotalF= atof(argsAryStr[iArg]);
      } /*Else if: Total map percent input*/

      else if(! cStrEql(parmStr,"-frameshift",'\0'))
         *frameshiftBl = 1;

      else if(! cStrEql(parmStr,"-no-frameshift",'\0'))
         *frameshiftBl = 0;

      else return parmStr;
   } /*Loop: Get user input*/

   return 0;
} /*tbAMRGetInput*/

/*-------------------------------------------------------\
| Fun-02: pTbAmrHelp
|   - Prints the help message for tbAMR
| Input:
|   - outFILE:
|     o File to print the help message to
| Output:
|   - Prints:
|     o The help message for tbAMR to outFILE
\-------------------------------------------------------*/
void pTbAmrHelp(
   void *outVoidFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02: pHelp
   '   - fun-02 sec-01:
   '     o Print out the usage/description
   '   - fun-02 sec-02:
   '     o Print out the user input
   '   - fun-02 sec-03:
   '     o Print out the output (TODO)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


   FILE *outFILE = (FILE *) outVoidFILE;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^  - Print out the usate/description
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
     outFILE,
     "tbAMR -sam-con map-consenuses.sam -amr-tbl file.tsv"
   );

   fprintf(outFILE, "\nor\n");
   fprintf(
      outFILE,
      "tbAMR -sam mapped-reads.sam -amr-tbl file.tsv\n"
   );

   fprintf(
      outFILE,
      "  - Detects Anti-microbal (antibiotic) resistant"
   );
        
   fprintf(outFILE, " genes\n");
   fprintf(outFILE, "    (AMRs) in input consensuses\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^  - Print out the user input
   ^   o fun-02 sec-02 sub-01:
   ^     - Print out input line
   ^   o fun-02 sec-02 sub-02:
   ^     - Database entries
   ^   o fun-02 sec-02 sub-03:
   ^     - Print out the -sam entry
   ^   o fun-02 sec-02 sub-04:
   ^     - Print out the amr table entry
   ^   o fun-02 sec-02 sub-05:
   ^     - Print out the -out entry
   ^   o fun-02 sec-02 sub-06:
   ^     - Print out maximum homopolymer length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Print out input line
   \*****************************************************/

    fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-02:
   *   - Database entry
   \*****************************************************/

    fprintf(outFILE, "  -amr-tbl:\n");

    fprintf(
       outFILE,
       "    o AMR database in tbAmr format. Use\n"
    );

    fprintf(
       outFILE,
       "    o Use whoToTbAmr to build this from the WHO\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Sequence input
   *   o fun-02 sec-02 sub-03 cat-01:
   *     - Read input
   *   o fun-02 sec-02 sub-03 cat-02:
   *     - Consnesus input
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-01:
   +   - Read input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "  -sam: [Default]\n");

    fprintf(
       outFILE,
       "    o Sam file with reads mapped to the same\n"
    );

    fprintf(
      outFILE,
      "       reference (NC000962.3) as AMR database\n"
    );

    fprintf(
       outFILE,
       "    o Use \"-sam -\" to specify stdin\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-01:
   +   - Consnesus input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "  -sam-con: [Replaces -sam]\n");

    fprintf(
       outFILE,
       "    o Sam file with consensus(es) mapped to the\n"
    );


    fprintf(
      outFILE,
      "      same reference (NC000962.c) in the AMR"
    );
    fprintf(outFILE, " databse\n");

    fprintf(
       outFILE,
       "    o Use \"-sam-con -\" to specify stdin\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Output options
   *   o fun-02 sec-02 sub-04 cat-01:
   *     - General output
   *   o fun-02 sec-02 sub-04 cat-02:
   *     - Read id output
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-01:
   +   - General output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "  -out: [stdout]\n");

    fprintf(
       outFILE,
       "    o File to output results to\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-02:
   +   - Read id output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "  -id-file: [Not used]\n");

    fprintf(
       outFILE,
       "    o File to print the read ids that supported"
    );
    fprintf(
       outFILE,
       "\n      a variant to (read_id\\tvariant_id)"
    );

    fprintf(outFILE, "\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Settings
   *   o fun-02 sec-02 sub-04 cat-01:
   *     - Checking frameshift paramater
   *   o fun-02 sec-02 sub-04 cat-02:
   *     - Min depth paramater
   *   o fun-02 sec-02 sub-04 cat-03:
   *     - Min % reads mapping parameter
   *   o fun-02 sec-02 sub-04 cat-04:
   *     - Min % of total reads parameter
   \*****************************************************/

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-04 Cat-01:
    +   - Checking frameshift paramater
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       outFILE,
       "  -frameshift: [%i]\n",
       def_checkFrameshift_checkAmr
   );

   fprintf(
      outFILE,
      "    o Check frames shifts\n"
   );

   fprintf(
      outFILE,
      "    o Without this tbAmr treats frames shifts\n"
   );

   fprintf(
      outFILE,
      "      an extact match\n"
   );

   fprintf(
      outFILE,
      "    o Turn off with: -no-frameshift\n"
   );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-04 Cat-02:
    +   - Min depth paramater
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE,"  -min-depth: [%i]\n",defMinDepth);

    fprintf(
       outFILE,
       "    o Reads only; min read depth to keep an AMR"
    );
    fprintf(outFILE, "\n");

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-04 Cat-03:
    +   - Min % reads mapping parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       outFILE,
       "  -min-amr-map-perc: [%f]\n",
       defMinPerReadsMap
   );

    fprintf(
       outFILE,
       "    o Reads only; mininmum percent of mapped\n"
    );

    fprintf(
       outFILE,
       "      reads that supported the AMR needed to\n"
    );

    fprintf(
       outFILE,
       "        keep an AMR\n"
    );

    /*+++++++++++++++++++++++++++++++++++++++++++++++++++\
    + Fun-02 Sec-02 Sub-04 Cat-04:
    +   - Min % of total reads parameter
    \+++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
       outFILE,
       "  -min-total-map-perc: [%f]\n",
       defMinPerReadsTotal
   );

    fprintf(
       outFILE,
       "    o Reads only; Minimum percent of total reads"
    );
    fprintf(
       outFILE,
       "\n      (mapped/un-mapped) needed to keep an AMR"
    );
    fprintf(outFILE, "\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^  - Print out the output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
   fprintf(outFILE, "Output:\n");
   fprintf(outFILE, "  - Prints detected AMRs to -out\n");

   fprintf(
      outFILE,
      "  - Prints read ids with AMRs to -id-file\n"
   );
} /*pHelp*/

/*-------------------------------------------------------\
| Fun-03: pTbAmrVersion
|   - Prints the version number for tbAMR
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o The version for tbAMR to outFILE
\-------------------------------------------------------*/
void pTbAmrVersion(void *outFILE){
   fprintf(
       (FILE *) outFILE,
       "tbAmr Version: %i.%02i.%02i\n",
       def_year_tbAmr,
       def_month_tbAmr,
       def_day_tbAmr
   ); /*Version of tbCon*/
} /*pTbAmrVersion*/

/*-------------------------------------------------------\
| Main:
|   - Detectes AMRs in reads/consensuses (driver fuction
|     for tbAmr).
| Input:
| Output:
|   - Prints:
|     o detected AMRs in user requested output
|     o Read ids for reads with AMRs (user requested)
\-------------------------------------------------------*/
int main(
   int numArgsI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: tbAMR
   '   o main sec-01:
   '     - Variable declerations
   '   o main Sec-02:
   '     - Get and check user input
   '   o main Sec-03:
   '     - Check if the sam file and output file exist
   '   o main Sec-04:
   '     - Open amr table file and get amr's
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input variables*/

   char *amrTblStr = 0;/*Table with AMR mutations*/

   char *samFileStr = 0; /*Sequences to check for AMRs*/
   char *outFileStr = 0; /*Holds path to output file*/
   char *idFileStr = 0;  /*Name of file to print ids to*/

   char readsBl;     /*1: user input reads*/

   /*Stats for read processing*/
   uint minDepthUI = defMinDepth;
   float minPercMapF = defMinPerReadsMap;
   float minPercTotalF = defMinPerReadsTotal;
   char frameshiftBl = def_checkFrameshift_checkAmr;

   char *drugAryStr = 0; /*Holds antibiotic names*/
   int numDrugsI = 0;    /*Holds number drugs in drugAry*/
   int maxDrugsI = 0;    /*max drugs drugAry can have*/
   
   char *errStr = 0;
   char errC = 0;

   uint numAmrUI = 0;

   FILE *checkFILE = 0;

   struct amrStruct *amrST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^  - Get and check user input
   ^  o main sec-02 sub-01:
   ^    - Get user input
   ^  o main sec-02 sub-02:
   ^    - Check if had an error when getting input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /****************************************************\
   * Main Sec-02 Sub-01:
   *  - Get user input
   \****************************************************/

   errStr =
     tbAMRGetInput(
        numArgsI,
        argsAryStr,
        &amrTblStr,    /*Table with AMR mutations*/
        &samFileStr,   /*Sequence to check for resitance*/
        &outFileStr,   /*File to outuput to*/
        &idFileStr,    /*Prefix for read id files*/
        &readsBl,      /*1: user input reads*/
        &frameshiftBl, /*1: User wants frameshift checks*/
        &minDepthUI,   /*Min read depth to keep an amr*/
        &minPercMapF,  /*Min % support to keep amr; read*/
        &minPercTotalF /*Min % mapped reads to keep*/
     ); /*Get the user input*/

   /****************************************************\
   * Main Sec-02 Sub-02:
   *  - Check if had an error when getting input
   \****************************************************/

   if(errStr)
   { /*If: I had an error*/

      if(   cStrEql(errStr, "-h", '\0') == 0
         || cStrEql(errStr, "--h", '\0') == 0
         || cStrEql(errStr, "-help", '\0') == 0
         || cStrEql(errStr, "--help", '\0') == 0
         || cStrEql(errStr, "help", '\0') == 0
      ){ /*If: the user requested the help message*/
         pTbAmrHelp(stdout);
         exit(0);
      } /*If: the user requested the help message*/


      if(   cStrEql(errStr, "-v", '\0') == 0
         || cStrEql(errStr, "--v", '\0') == 0
         || cStrEql(errStr, "-version", '\0') == 0
         || cStrEql(errStr, "--version", '\0') == 0
         || cStrEql(errStr, "version", '\0') == 0
      ){ /*If: the user requested the version number*/
         pTbAmrVersion(stdout);
         exit(0);
      } /*If: the user requested the version number*/

      pTbAmrHelp(stderr);

      fprintf(
         stderr,
         "%s is not an valid argument\n",
         errStr
      );

      exit(-1);
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Check if the sam file and output file exist
   ^   o main sec-03 sub-01:
   ^     - Check if I can open the sam file
   ^   o main sec-03 sub-02:
   ^     - Check if I can open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Check if I can open the sam file
   \*****************************************************/

   if(samFileStr && samFileStr[0] != '-')
   { /*If: I was given a sam file*/
      checkFILE = fopen(samFileStr, "r");

      if(! checkFILE)
      { /*If: I could not open the file*/
        fprintf(
           stderr,
           "Could not open -sam %s\n",
           samFileStr
        );
        exit(-1);
      } /*If: I could not open the file*/

      fclose(checkFILE); 
      checkFILE = 0;
   } /*If: I was given a sam file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check if I can open the output file
   \*****************************************************/

   if(outFileStr && outFileStr[0] != '-')
   { /*If: I was given a out file*/
      checkFILE = fopen(outFileStr, "r");

      if(! checkFILE)
      { /*If: I could not open the file*/
        fprintf(
           stderr,
           "Could not open -out %s\n",
           outFileStr
        );
        exit(-1);
      } /*If: I could not open the file*/

      fclose(checkFILE); 
      checkFILE = 0;
   } /*If: I was given a out file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Open amr table file and get amr's
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   checkFILE = fopen(amrTblStr, "r");

   if(checkFILE == 0)
   { /*If: I could not open the WHO catalog*/
      fprintf(
         stderr,
         "Could not open -amr-tbl %s\n",
         amrTblStr
      );

      exit(-1);
   } /*If: I could not open the WHO catalog*/

   fclose(checkFILE);
   checkFILE = 0;

   amrST =
      readTbAmrTbl(
         amrTblStr, 
         &numAmrUI,
         &drugAryStr,
         &numDrugsI,
         &maxDrugsI,
         &errC
      ); /*Get the amr entries*/

   if(errC == def_amrST_invalidFILE)
   { /*If: I could not read in the AMR table*/
      fprintf(
        stderr,
        "%s is no in tbAmr format or has no data\n",
        amrTblStr
      );

      exit(-1);
   } /*If: I could not read in the AMR table*/

   if(errC == def_amrST_memError)
   { /*If: I had a memory error*/
      fprintf(
        stderr,
        "(mem) Err; when processing variant id's\n"
      );
 
      exit(-1);
   } /*If: I had a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Check for resistance mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lookForAmrsSam(
      amrST,
      (int) numAmrUI,
      drugAryStr,
      readsBl,      /*Working with reads or consesnsuses*/
      frameshiftBl, /*Process frameshifts*/
      minDepthUI,   /*Minimum read depth*/
      minPercMapF,  /*Min % support to keep amr (read)*/
      minPercTotalF,/*Min % mapped reads to keep*/
      samFileStr,
      outFileStr,
      idFileStr /*Prefix for variant read id files*/
   ); /*check for amrs*/
      
   freeAmrStructArray(&amrST, numAmrUI);
   free(drugAryStr);

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
