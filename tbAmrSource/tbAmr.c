#include <stdio.h>
#include <stdlib.h>

#include "tbAmr.h" /*Header for this file*/

#include "checkAmr.h"
#include "amrStruct.h"

/*Only .h file*/
#include "../generalLib/ulCpStr.h"
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/base10StrToNum.h"

int main(
   int numArgsI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: tbAMR
   '   o main sec-01:
   '     - Variable declerations
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input variables*/

   char *amrTblStr = 0;/*Table with AMR mutations*/

   char *samStr = 0;   /*Sequence to check for resitance*/
   char *outStr = 0;   /*Holds path to File to output to*/
   char *idPrefStr = 0; /*Prefix for read id files*/

   char readsBl;     /*1: user input reads*/

   /*Stats for read processing*/
   uint minDepthUI = defMinDepth;
   float minPercMapF = defMinPerReadsMap;
   float minPercTotalF = defMinPerReadsTotal;

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
        &amrTblStr,  /*Table with AMR mutations*/
        &samStr,     /*Sequence to check for resitance*/
        &outStr,     /*File to outuput to*/
        &idPrefStr,   /*Prefix for read id files*/
        &readsBl,     /*1: user input reads*/
        &minDepthUI,   /*Min read depth to keep an amr*/
        &minPercMapF,/*Min % support to keep amr (read)*/
        &minPercTotalF  /*Min % mapped reads to keep*/
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

   if(samStr && samStr[0] != '-')
   { /*If: I was given a sam file*/
      checkFILE = fopen(samStr, "r");

      if(! checkFILE)
      { /*If: I could not open the file*/
        fprintf(stderr,"Could not open -sam %s\n",samStr);
        exit(-1);
      } /*If: I could not open the file*/

      fclose(checkFILE); 
      checkFILE = 0;
   } /*If: I was given a sam file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check if I can open the output file
   \*****************************************************/

   if(outStr && outStr[0] != '-')
   { /*If: I was given a out file*/
      checkFILE = fopen(outStr, "r");

      if(! checkFILE)
      { /*If: I could not open the file*/
        fprintf(stderr,"Could not open -out %s\n",outStr);
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
      readsBl,    /*Working with reads or consesnsuses*/
      minDepthUI,  /*Minimum read depth*/
      minPercMapF,/*Min % support to keep amr (read)*/
      minPercTotalF, /*Min % mapped reads to keep*/
      samStr,
      outStr,
      idPrefStr /*Prefix for variant read id files*/
   ); /*check for amrs*/
      
   freeAmrStructArray(&amrST, numAmrUI);
   free(drugAryStr);

   exit(0);
} /*main*/

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
|   - samStr:
|     o Will hold the path to the gehoeme to check for
|       antibiotic resitiance
|   - outStr:
|     o Will hold the path to the file to output to
|   - maxHomlI:
|     o Will hold the maximum homopolymer length to keep
|       an indel
|   - whoTlbFlag:
|     o Tells if using the 2021 (1) or 2023 (2) catalog or
|       if using my own format once implements
| Output:
|   - Modifies:
|     o All input variables
\-------------------------------------------------------*/
char * tbAMRGetInput(
   int numArgsI,
   char *argsAryStr[],
   char **amrTblStr,  /*Table with AMR mutations*/
   char **samStr,     /*Sequence to check for resitance*/
   char **outStr,     /*File to output to*/
   char **idPrefStr,   /*Prefix for read id files*/
   char *readsBl,     /*1: user input reads*/
   uint *minDepthUI,   /*Min read depth to keep an amr*/
   float *minPercMapF,/*Min % support to keep amr (read)*/
   float *minPercTotalF  /*Min % mapped reads to keep*/
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
         *samStr = argsAryStr[iArg];
         *readsBl = 1;
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-sam-con", '\0') == 0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *samStr = argsAryStr[iArg];
         *readsBl = 0;
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-out", '\0') == 0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *outStr = argsAryStr[iArg];
      } /*Else if: User input consensus mappings*/

      else if(cStrEql(parmStr, "-id-file-pref", '\0') ==0)
      { /*Else if: User input consensus mappings*/
         ++iArg;
         *idPrefStr = argsAryStr[iArg];
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
       "    o AMR database in tbAmr format. Use"
    );

    fprintf(
       outFILE,
       "    o Use whoToTbAmr to build this from the WHO"
    );

    fprintf(outFILE,  " catalogs\n");

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
       "   o Sam file with reads mapped to the same\n"
    );

    fprintf(
      outFILE,
      "      reference that was used to build the"
    );

    fprintf(outFILE,  " database\n");
  
    fprintf(
       outFILE,
       "   o Use \"-sam -\" to specify stdin\n"
    );

    fprintf(
       outFILE,
       "   o The reference is likely Genbank acession"
    );

    fprintf(outFILE, " NC000962\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-03 Cat-01:
   +   - Consnesus input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "  -sam-con: [Replaces -sam]\n");

    fprintf(
       outFILE,
       "     o Sam file with the consensus(es) mapped"
    );

    fprintf(outFILE, "to the same\n");

    fprintf(
      outFILE,
      "      reference that was used to build the"
    );

    fprintf(outFILE,  " database\n");
  
    fprintf(
       outFILE,
       "      o Use \"-sam-con -\" to specify stdin\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Output options
   *   o fun-02 sec-02 sub-04 cat-01:
   *     - General output
   *   o fun-02 sec-02 sub-04 cat-02:
   *     - Read id output
   \*****************************************************/

   fprintf(outFILE,"  Output options:\n");

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-01:
   +   - General output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "    -out: [stdout]\n");

    fprintf(
       outFILE,
       "      o File to output results to\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun-02 Sec-02 Sub-04 Cat-02:
   +   - Read id output
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(outFILE, "    -id-file-pref: [Not used]\n");

    fprintf(
       outFILE,
       "      o Prints the read ids that mapped to each\n"
    );
    fprintf(
       outFILE,
       "      o variant in a file name prefix-variant.ids"
    );

    fprintf(outFILE, "\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Settings
   \*****************************************************/

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
      "tbAMR version: %i\n",
      defTbAmrVersion
   );
}
