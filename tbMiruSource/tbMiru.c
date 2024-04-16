/*#######################################################\
# Name: tbMiru
#   - Determines the MIRU lineages for TB amplicons
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Included libraries and function headers
'   o main:
'     - run tbMiru (driver function)
'   o fun-01: getInput_tbMiru
'     - Get the users input
'   o fun-02: pHelp_tbMiru
'     - Prints the help message for tbMiru
'   o fun-03: pVersion_tbMiru
'     - Prints out the verision number for tbMiru
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and function headers
|   o header sec-01:
|     - Included libraries
|   o header sec-02:
|     - function headers
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^   - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef plan9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "miruTblStruct.h"
#include "../generalLib/samEntryStruct.h"

/*.h file only*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"
#include "../generalLib/base10StrToNum.h"
#include "tbMiru-version.h"

/*Hidden dependencies:
  - have .c files
  - no .c files
    o #include "../generalLib/numToStr.h"
    o #include "../generalLib/genMath.h"
*/

#define def_pHelp_tbMiru 1
#define def_pVersion_tbMiru 2
#define nonNum_tbMiru 4

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^   - function headers
^   o fun-01: getInput_tbMiru
^   o fun-02: pHelp_tbMiru
^   o fun-03: pVersion_tbMiru
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*-------------------------------------------------------\
| Fun-01: getInput_tbMiru
|   - Get the users input
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argStrAry:
|     o Array of c-strings with user input
|   - samFileStr:
|     o Pointer to c-string to hold the sam file
|   - miruFileStr:
|     o Pointer to c-string with the miru table to use
|       in assinging lineages
|   - outFileStr:
|     o Pointer to c-string to hold the output flie
|   - tblOutFileStr:
|     o Pointer to c-string to hold the miru table output
|   - fudgeLenI:
|     o Pointer to integer with difference in allowed
|       amplicon lengths
| Output:
|   - Modifies:
|     o All input variables, except numArgsI and
|       argStrAry to hold user input
|   - Returns:
|     o 0 for success
|     o def_pHelp_tbMiru if the user requested help message
|     o def_pVersion_tbMiru if user requested version number
|     o argument index << 8 if a non-vallid parmeter
|     o ((argument index) << 8) | nonNum_tbMiru if I had
|       an number conversion error
\-------------------------------------------------------*/
long
getInput_tbMiru(
   int numArgsI,
   char *argStrAry[],
   char **samFileStr,
   char **miruFileStr,
   char **outFileStr,
   char **tblOutFileStr,
   int *fudgeLenI
);

/*-------------------------------------------------------\
| Fun-02: pHelp_tbMiru
|   - Prints the help message for tbMiru
| Input:
|   - outFILE:
|     o File to print the help message
| Output:
|   - Prints:
|     o The help message to outFILE
\-------------------------------------------------------*/
void
pHelp_tbMiru(
   FILE *outFILE
);

/*-------------------------------------------------------\
| Fun-03: pVersion_tbMiru
|   - Prints out the verision number for tbMiru
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o The versoin number message to outFILE
\-------------------------------------------------------*/
void
pVersion_tbMiru(
   FILE *outFILE
);

/*-------------------------------------------------------\
| Main:
|   - Driver function for tbMiru
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argStrAry:
|     o C-string array with user input arguments
| Output:
|   - Prints:
|     o Lineages or table to output file
\-------------------------------------------------------*/
#ifdef plan9
void
#else
int
#endif

main(
   int numArgsI,
   char *argStrAry[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - Driver function for tbMiru
   '   o main sec-01:
   '     - Variable declerations
   '   o main sec-02:
   '     - Get and check user input
   '   o main sec-03:
   '     - Read in the miru table
   '   o main sec-04:
   '     - Fill in the miru table
   '   o main sec-05:
   '     - Print in the MIRU lineage/table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *samFileStr = "side-files/tbRef.sam";
   char *miruFileStr = "miruTbl.tsv";
   char *outFileStr = 0;
   char *tblOutFileStr = 0;

   int fudgeLenI = def_fudgeLen_miruTblST;

   long inputErrL = 0;
   char errC = 0;
   char errTblC = 0;

   struct miruTbl *miruST = 0;

   FILE *samFILE = 0;
   struct samEntry samST;
   char *buffStr = 0;
   ulong lenBuffUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^   - Initialize variables and get user input
   ^   o main sec-02 sub-01:
   ^     - Check if the user input anything
   ^   o main sec-02 sub-02:
   ^     - Get the user input
   ^   o main sec-02 sub-03:
   ^     - Check if had error when getting user input
   ^   o main sec-02 sub-04:
   ^     - Open/check the input sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-02 Sub-01:
   *   - Check if the user input anything
   \*****************************************************/

   if(numArgsI < 1)
   { /*If: Nothing was input*/
      pHelp_tbMiru(stderr);
      fprintf(stderr, "Nothing was input\n");
      exit(-1);
   } /*If: Nothing was input*/

   /*****************************************************\
   * Main Sec-02 Sub-02:
   *   - Get the user input
   \*****************************************************/

   inputErrL =
      getInput_tbMiru(
         numArgsI,
         argStrAry,
         &samFileStr,
         &miruFileStr,
         &outFileStr,
         &tblOutFileStr,
         &fudgeLenI
      ); /*Get the user input*/

   /*****************************************************\
   * Main Sec-02 Sub-03:
   *   - Check if had error when getting user input
   \*****************************************************/

   if(inputErrL)
   { /*If: I had an error*/
      if(inputErrL & def_pHelp_tbMiru)
      { /*If: The user requested the help message*/
         pHelp_tbMiru(stdout);
         exit(0);
      } /*If: The user requested the help message*/

      if(inputErrL & def_pVersion_tbMiru)
      { /*If: The user requested the help message*/
         pVersion_tbMiru(stdout);
         exit(0);
      } /*If: The user requested the help message*/

      if(inputErrL & nonNum_tbMiru)
      { /*If: This entry had non-numeric input error*/
         inputErrL >>= 8; /*Get argument*/

         fprintf(
            stderr,
            "Input for %s (%s) is not a number\n",
            argStrAry[inputErrL],
            argStrAry[inputErrL - 1]
         );
      } /*If: This entry had non-numeric input error*/

      else
      { /*Else: unkown input*/
         pHelp_tbMiru(stderr);

         fprintf(
            stderr,
            "%s is not recognized\n",
            (argStrAry[inputErrL >> 8])
        );
      } /*Else: unkown input*/

      exit(-1);
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec-02 Sub-04:
   *   - Open/check the input sam file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-') samFILE = stdin;

   else
   { /*Else: The user provided an file*/
      samFILE = fopen(samFileStr, "r");

      if(! samFILE)
      { /*If: I could not open the sam file*/
         fprintf(stderr, "Could not open ");
         fprintf(stderr, "-sam %s\n", samFileStr);

         exit(-1);
      } /*If: I could not open the sam file*/
   } /*Else: The user provided an file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^   - Read in the miru table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   miruST = readMiruTbl(miruFileStr, &errC);

   if(errC)
   { /*If: I had an error with the miru table file*/
      if(samFILE != stdin) fclose(samFILE);

      if(errC == def_fileErr_miruTblST)
         fprintf(
            stderr,
            "Could not open -miru -tbl %s\n",
            miruFileStr
          ); /*Let the user know the error type*/

      else if(errC == def_memErr_miruTblST)
         fprintf(stderr, "Ran out of memory\n");

      else
         fprintf(
            stderr,
            "Error while reading -miru-tbl %s\n",
               miruFileStr
         ); /*Let the user know the error*/

      exit(-1);
   } /*If: I had an error with the miru table file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^   - Fill in the miru table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSamEntry(&samST);

   errC = readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);

   while(! errC)
   { /*Loop: Read in the lines from the sam file*/
      /*Check if there is an sequence entry*/
      if(! samST.seqStr) goto samNextLine_tbMIRU;
      if(*samST.seqStr == '*') goto samNextLine_tbMIRU;
      if(*samST.seqStr == '\0') goto samNextLine_tbMIRU;

      /*Check for unammped reads (4), secondary alingments
      `  (256), and supplemental alignments (2048)
      */
      if(samST.flagUS & (4 | 256 | 2048))
         goto samNextLine_tbMIRU;

      inc_matching_len_lineages(&samST,fudgeLenI,miruST);

      samNextLine_tbMIRU:;

      errC =
         readSamLine(&samST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: Read in the lines from the sam file*/

   freeSamEntryStack(&samST);
   fclose(samFILE);
   samFILE = 0;
   free(buffStr);
   buffStr = 0;
   lenBuffUL = 0;

   if(errC & 64)
   { /*If: I had an memory error*/
      fprintf(
         stderr,
         "Memory error when reading sam file\n"
      );

      exit(-1);
   } /*If: I had an memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^   - Print in the MIRU lineage/table
   ^   o main sec-05 sub-01:
   ^     - Print out the MIRU table for reads
   ^   o main sec-05 sub-02:
   ^     - Print out the MIRU lineage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-05 Sub-01:
   *   - Print out the MIRU table for reads
   \*****************************************************/

   if(tblOutFileStr)
   { /*If: I am  printing out the read table*/
      errTblC = pMiruTbl(miruST, tblOutFileStr);

      if(errTblC)
      { /*If: I could not open the output file*/

         if(outFileStr)
            fprintf(
               stderr,
               "Could not write to -out-tbl %s\n",
               outFileStr
            ); /*Let the user know the file problem*/

         else
            fprintf(
               stderr,
               "Could not write to stdout (-out-tbl -)\n"
            ); /*Let the user know the file problem*/
      } /*If: I could not open the output file*/
   } /*If: I am  printing out the read table*/

   /*****************************************************\
   * Main Sec-05 Sub-02:
   *   - Print out the MIRU lineage
   \*****************************************************/

   errC = pLineages(miruST, outFileStr);

   freeMiruTbl(&miruST);

   if(errC)
   { /*If: I could not open the output file*/
      if(outFileStr)
         fprintf(
            stderr,
            "Could not write to -out %s\n",
            outFileStr
         ); /*Let the user know the file problem*/

      else
         fprintf(
            stderr,
            "Could not write to stdout (-out -)\n"
         ); /*Let the user know the file problem*/

      exit(-1);
   } /*If: I could not open the output file*/

   if(errTblC) exit(-1);

   exit(0);
} /*Main*/

/*-------------------------------------------------------\
| Fun-01: getInput_tbMiru
|   - Get the users input
| Input:
|   - numArgsI:
|     o Number of arguments the user input
|   - argStrAry:
|     o Array of c-strings with user input
|   - samFileStr:
|     o Pointer to c-string to hold the sam file
|   - miruFileStr:
|     o Pointer to c-string with the miru table to use
|       in assinging lineages
|   - outFileStr:
|     o Pointer to c-string to hold the output flie
|   - tblOutFileStr:
|     o Pointer to c-string to hold the miru table output
|   - fudgeLenI:
|     o Pointer to integer with difference in allowed
|       amplicon lengths
| Output:
|   - Modifies:
|     o All input variables, except numArgsI and
|       argStrAry to hold user input
|   - Returns:
|     o 0 for success
|     o def_pHelp_tbMiru if the user requested help message
|     o def_pVersion_tbMiru if user requested version number
|     o argument index << 8 if a non-vallid parmeter
|     o ((argument index) << 8) | nonNum_tbMiru if I had
|       an number conversion error
\-------------------------------------------------------*/
long
getInput_tbMiru(
   int numArgsI,
   char *argStrAry[],
   char **samFileStr,
   char **miruFileStr,
   char **outFileStr,
   char **tblOutFileStr,
   int *fudgeLenI
){
   int iArg = 0;
   char *tmpStr = 0;

   for(iArg = 1; iArg < numArgsI; iArg += 2)
   { /*Loop: Get the user input*/
      if(! cStrEql("-sam", argStrAry[iArg], '\0'))
         *samFileStr = argStrAry[iArg + 1];


      else if(!cStrEql("-miru-tbl", argStrAry[iArg],'\0'))
         *miruFileStr = argStrAry[iArg + 1];

      else if(! cStrEql("-out", argStrAry[iArg], '\0'))
         *outFileStr = argStrAry[iArg + 1];

      else if(! cStrEql("-out-tbl",argStrAry[iArg],'\0'))
         *tblOutFileStr = argStrAry[iArg + 1];

      else if(! cStrEql("-fudge",argStrAry[iArg],'\0'))
      { /*Else If: This was the fudge length*/
         tmpStr =
            base10StrToUI(argStrAry[iArg], *fudgeLenI);

         if(*tmpStr) return (iArg << 8) | nonNum_tbMiru;
         
      } /*Else If: This was the fudge length*/

     /*Check for help message requests*/
     else if(! cStrEql("-h", argStrAry[iArg], '\0'))
        return def_pHelp_tbMiru;

     else if(! cStrEql("--h", argStrAry[iArg], '\0'))
        return def_pHelp_tbMiru;

     else if(! cStrEql("help", argStrAry[iArg], '\0'))
        return def_pHelp_tbMiru;
 
     else if(! cStrEql("-help", argStrAry[iArg], '\0'))
        return def_pHelp_tbMiru;

     else if(! cStrEql("--help", argStrAry[iArg], '\0'))
        return def_pHelp_tbMiru;

     /*Check for version number requests*/
     else if(! cStrEql("-v", argStrAry[iArg], '\0'))
        return def_pVersion_tbMiru;

     else if(! cStrEql("--v", argStrAry[iArg], '\0'))
        return def_pVersion_tbMiru;

     else if(! cStrEql("version", argStrAry[iArg], '\0'))
        return def_pVersion_tbMiru;

     else if(! cStrEql("-version", argStrAry[iArg], '\0'))
        return def_pVersion_tbMiru;

     else if(! cStrEql("--version",argStrAry[iArg],'\0'))
        return def_pVersion_tbMiru;

     /*Else the input was not recongnized*/
     else return (iArg << 8);
   } /*Loop: Get the user input*/

   return 0;
} /*getInput_tbMiru*/

/*-------------------------------------------------------\
| Fun-02: pHelp_tbMiru
|   - Prints the help message for tbMiru
| Input:
|   - outFILE:
|     o File to print the help message
| Output:
|   - Prints:
|     o The help message to outFILE
\-------------------------------------------------------*/
void
pHelp_tbMiru(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC: pHelp_tbMiru
   '   - Prints the help message for tbMiru
   '   o fun-02 sec-01;
   '     - Print out usage entry
   '   o fun-02 sec-02;
   '     - Print out the input options
   '   o fun-02 sec-03;
   '     - Print out the output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^   - Print out usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "tbMiru -sam reads.sam -miru-tbl MIRU-table.tsv\n"
   );

   fprintf(
      outFILE,
      "tbMiru -sam consensuses.sam"
   );
   fprintf(outFILE, " -miru-tbl MIRU-table.tsv\n");

   fprintf(
      outFILE,
      "  - Finds MIRU lineage for input reads/consensuses"
   );

   fprintf(outFILE, "\n");
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^   - Print out the input options
   ^   o fun-02 sec-02 sub-01:
   ^     - Print the input header
   ^   o fun-02 sec-02 sub-02:
   ^     - The sam file entry
   ^   o fun-02 sec-02 sub-03:
   ^     - The fudge (length) entry
   ^   o fun-02 sec-02 sub-04:
   ^     - The miru table entry
   ^   o fun-02 sec-02 sub-05:
   ^     - The out file entry
   ^   o fun-02 sec-02 sub-06:
   ^     - The out table file entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /**************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Print the input header
   \**************************************************/

   fprintf(outFILE, "Input:\n");

   /**************************************************\
   * Fun-02 Sec-02 Sub-02:
   *   - The sam file entry
   \**************************************************/

   fprintf(outFILE, "  -sam: [Required]\n");

   fprintf(
     outFILE,
     "   o Sam file with mapped reads to find lineages"
   );
   fprintf(outFILE, " for\n");

   fprintf(
     outFILE,
     "   o minimap2 -a ref.fasta reads.fastq > reads.sam"
   );
   fprintf(outFILE, "\n");

   fprintf(
     outFILE,
     "   o minimap2 -a ref.fa consensuses.fa >"
   );
   fprintf(outFILE, " conesnuses.sam\n");

   /**************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - The fudge (length) entry
   \**************************************************/

   fprintf(
      outFILE,
      "  -fudge: [%i]\n",
      def_fudgeLen_miruTblST
   );

   fprintf(
     outFILE,
     "   o Range to fudge lengths by to call lineage"
   );

   fprintf(
     outFILE,
     "\n   o Lineage length range is + or - fudge\n"
   );

   /**************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - The miru table entry
   \**************************************************/

   fprintf(outFILE, "  -miru-tbl: [Required]\n");

   fprintf(
     outFILE,
     "   o tsv with the MIRU table to find lineages"
   );
   fprintf(outFILE, " with\n ");

   fprintf(
     outFILE,
     "   - Format: \n"
   );

   fprintf(
     outFILE,
     "     o The first column is the lineage name\n"
   );

   fprintf(
     outFILE,
     "     o Remaining columns are for each primer\n"
   );

   fprintf(
     outFILE,
     "     o Header has primer name and coordinates\n"
   );

   fprintf(
     outFILE,
     "       - Primer: name.forward.len.reverse.len\n"
   );

   fprintf(
     outFILE,
     "         i. name: Name of the primer\n"
   );

   fprintf(
     outFILE,
     "         ii. forward: Forward primer start\n"
   );

   fprintf(
     outFILE,
     "         iii. len: Length of forward primer\n"
   );

   fprintf(
     outFILE,
     "         iv. reverse: Reverse primer position\n"
   );

   fprintf(
     outFILE,
     "         v. len: Length of reverse primer\n"
   );

   fprintf(
     outFILE,
     "     o Each row (non-header) has the length \n"
   );

   fprintf(
     outFILE,
     "       needed for a read to match an lineage;\n"
   );

   fprintf(
     outFILE,
     "       this includes the foward primer, but\n"
   );

   fprintf(
     outFILE,
     "       not the reverse primer\n"
   );

   /**************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - The out file entry
   \**************************************************/

   fprintf(outFILE, "  -out: [stdout]\n");

   fprintf(
     outFILE,
     "   o File to output each primers lineages to\n"
   );

   fprintf(
     outFILE,
     "   o Use `-` for stdout\n"
   );

   /**************************************************\
   * Fun-02 Sec-02 Sub-06:
   *   - The out table file entry
   \**************************************************/

   fprintf(outFILE, "  -out-tbl: [do not output]\n");

   fprintf(
     outFILE,
     "   o File to print filled in MIRU tsv table to\n"
   );

   fprintf(
     outFILE,
     "   o Use `-` for stdout\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-03:
   ^   - Print out the output entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:\n");

   fprintf(
      outFILE,
      "  - Miru table with the number of reads that\n"
   );

   fprintf(
      outFILE,
      "    mapped to each lineage and primer (-out-tbl)\n"
   );

   fprintf(
      outFILE,
      "  - The most supported MIRU lineage for each"
   );

   fprintf(
      outFILE,
      " primer\n"
   );
} /*pHelp_tbMiru*/

/*-------------------------------------------------------\
| Fun-03: pVersion_tbMiru
|   - Prints out the verision number for tbMiru
| Input:
|   - outFILE:
|     o File to print the version number to
| Output:
|   - Prints:
|     o The version number message to outFILE
\-------------------------------------------------------*/
void
pVersion_tbMiru(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "tbMiru version: %i-%02i-%02i\n",
      def_year_tbMiru,
      def_month_tbMiru,
      def_day_tbMiru
   );
} /*pVersion_tbMiru*/

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
