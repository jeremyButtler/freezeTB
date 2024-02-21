#include "tbAMR.h" /*Header for this file*/
#include "checkAmr.h"
#include <string.h>

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
   int maxHomoI = defMaxHomoLen;/*Max homopolymer size*/
   char whoTblBl = 0; /*amrTblStr is from the who (csv)*/


   char *drugAryStr[] =
      {"RIF", "INH", "EMB", "PZA", "LEV", "MXF", "BDQ",
       "LZD", "CFZ", "DLM", "AMI", "STM", "ETH", "KAN",
       "CAP"  
      }; /*This is hardcoded for know, but I need to set
         ` this up so it is grabbed ffrom the file
         */

   char *buffStr = 0;   /*For reading sam files*/
   ulong lenBuffUL = 0; /*Size of buffStr*/

   char *errStr = 0;

   ulong numAmrUL = 0;

   FILE *amrFILE = 0;
   FILE *samFILE = 0;
   FILE *outFILE = 0;

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
        &maxHomoI,   /*Maximum homopolymer size*/
        &whoTblBl    /*amrTblStr is from the who (csv)*/
     ); /*Get the user input*/

   /****************************************************\
   * Main Sec-02 Sub-02:
   *  - Check if had an error when getting input
   \****************************************************/

   if(errStr)
   { /*If: I had an error*/

      if(   strcmp(errStr, "-h") == 0
         || strcmp(errStr, "--h") == 0
         || strcmp(errStr, "-help") == 0
         || strcmp(errStr, "--help") == 0
         || strcmp(errStr, "help") == 0
      ){ /*If: the user requested the help message*/
         pTbAmrHelp(stdout);
         exit(0);
      } /*If: the user requested the help message*/


      if(   strcmp(errStr, "-v") == 0
         || strcmp(errStr, "--v") == 0
         || strcmp(errStr, "-version") == 0
         || strcmp(errStr, "--version") == 0
         || strcmp(errStr, "version") == 0
      ){ /*If: the user requested the version number*/
         pTbAmrVersion(stdout);
         exit(0);
      } /*If: the user requested the version number*/

      if(strcmp(errStr, "-max-homo-len") == 0)
      { /*If: the user input non-numeric argument*/
         fprintf(
            stderr,
            "Argument from -max-homo-len is not a number"
         );
         fprintf(stderr, "\n");
         exit(-1);
      } /*If: the user input non-numeric argument*/

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

   if(!samStr || samStr[0] == '-') samFILE = stdin;

   else
   { /*Else: I am getting the sam file from a file*/
      samFILE = fopen(samStr, "r");

      if(!samFILE)
      { /*If: I could not open the file*/
        fprintf(stderr,"Could not open -sam %s\n",samStr);
        exit(-1);
      } /*If: I could not open the file*/
   } /*Else: I am getting the sam file from a file*/

   /*****************************************************\
   * Main Sec-03 Sub-02:
   *   - Check if I can open the output file
   \*****************************************************/

   if(!outStr || outStr[0] == '-') outFILE = stdout;

   else
   { /*Else: I am sending the output to a file*/
      outFILE = fopen(outStr, "r");

      if(!outFILE)
      { /*If: I could not open the file*/
        fprintf(stderr,"Could not open -out %s\n",outStr);
        fclose(samFILE);
        exit(-1);
      } /*If: I could not open the file*/
   } /*Else: I am sending the output to a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Open amr table file and get amr's
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrFILE = fopen(amrTblStr, "r");

   if(amrFILE == 0)
   { /*If: I could not open the WHO catalog*/
      fprintf(stderr, "Unable to open %s\n", amrTblStr);

      fclose(samFILE);
      fclose(outFILE);

      exit(-1);
   } /*If: I could not open the WHO catalog*/

   amrST = read_2021_WhoAmrCsv(amrFILE, &numAmrUL);
   fclose(amrFILE);

   if(amrST == 0)
   { /*If: There was a memory error*/
      fprintf(
         stderr,
         "Memory errror while extracting resistance"
      );

      fclose(samFILE);
      fclose(outFILE);

      fprintf(stderr, " profiles\n from %s\n", amrTblStr);
      exit(-1);
   } /*If: There was a memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Check if the sequence (genome) file extis/read in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^  - Check for resistance mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lookForAmrsSam(
      amrST,
      (int) numAmrUL,
      drugAryStr,
      &buffStr,
      &lenBuffUL,
      samFILE,
      outFILE
   );
      
   fclose(samFILE);
   fclose(outFILE);
   free(buffStr);
   freeAmrStructArray(amrST, numAmrUL);

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
|   - whoTlbBl:
|     o Holds if amrTblStr pionts to a csv from the
|       WHO or my own tsv file format
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
   int *maxHomoI,     /*Maximum homopolymer size*/
   char *whoTblBl     /*amrTblStr is from the who (csv)*/
){ /*tbAMRGetInput*/

   char *parmStr = 0;
   char *errStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < numArgsI; ++iArg)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      if(strcmp(parmStr, "-who-amr-tbl") == 0)
      { /*Else if: this is the amr table (from who)*/
         *amrTblStr = argsAryStr[iArg + 1];
         *whoTblBl = 1;
         ++iArg;
      } /*Else if: this is the amr table (from who)*/

      else if(strcmp(parmStr, "-amr-tbl") == 0)
      { /*Else if: this is the amr table*/
         *amrTblStr = argsAryStr[iArg + 1];
         *whoTblBl = 0;
         ++iArg;
      } /*Else if: this is the amr table*/

      else if(strcmp(parmStr, "-sam") == 0)
      { /*Else if: User input consensus mappings*/
         *samStr = argsAryStr[iArg + 1];
         ++iArg;
      } /*Else if: User input consensus mappings*/

      else if(strcmp(parmStr, "-out") == 0)
      { /*Else if: User input consensus mappings*/
         *outStr = argsAryStr[iArg + 1];
         ++iArg;
      } /*Else if: User input consensus mappings*/

      else if(strcmp(parmStr, "-max-homo-len") == 0)
      { /*Else if: tb sequence to check*/
         errStr =
            base10StrToSI(argsAryStr[iArg + 1],*maxHomoI);

         if(*errStr > 32) return parmStr;
         ++iArg;
      } /*Else if: tb sequence to check*/

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
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02: pHelp
   '   - fun-02 sec-01:
   '     o Print out the usage/description
   '   - fun-02 sec-02:
   '     o Print out the user input
   '   - fun-02 sec-03:
   '     o Print out the output (TODO)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-01:
   ^  - Print out the usate/description
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "tbAMR -sam mapped-consenuses.sam -who-amr-tbl"
   );

   fprintf(outFILE, " tbl.csv\n");

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
   ^     - Print out the who table entry
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
   *   - Print out the who table entry
   \*****************************************************/

    fprintf(outFILE, "  -who-amr-tbl: [REQUIRED]\n");

    fprintf(
       outFILE,
       "    o Csv with AMR anotations. This is the genome"
    );

    fprintf(outFILE, "\n");

    fprintf(
       outFILE,
       "      indicies sheet from the who TB catalog\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Print out the sam entry
   \*****************************************************/

    fprintf(outFILE, "  -sam: [REQUIRED; stdin]\n");

    fprintf(
       outFILE,
       "    o Sam file with consensuses mapped to the"
    );

    fprintf(outFILE,  " same\n");

    fprintf(
       outFILE,
       "      reference that was used for -who-amr-tbl"
    );
  
    fprintf(outFILE, " file.csv\n");

    fprintf(
       outFILE,
       "    o The reference is likely Genbank acession"
    );

    fprintf(outFILE, " NC000962\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Print out the amr table entry
   \*****************************************************/

    /*TODO: Add this feature in
    fprintf(outFILE, "  -amr-tbl: [REQUIRED]\n");
    fprintf(
       outFILE,
       "    o Tsv with AMR anotations (replaces -who-amr"
    );

    fprintf(outFILE, "-tbl)\n");

    fprintf(
       outFILE,
       "    o Format: gene-id\tfirst-base\tcodon-start\n"
    );

    fprintf(
       outFILE,
       "      mutation-pattern\tamino-mutation\t"
    );

    fprintf(outFILE, "1st-antibiotic\n");

    fprintf(
        outFILE,
        "      2nd-antibiotic\t3rd-antibiotic\t...\n"
    );
   */

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Print out the -out entry
   \*****************************************************/

    fprintf(outFILE, "  -out: [stdout]\n");

    fprintf(
       outFILE,
       "    o File to output results to\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-06:
   *   - Print out maximum homopolymer length
   \*****************************************************/

    /*TODO: Add this feature in
    fprintf(
       outFILE,
       "  -max-homo-len: [%i]\n",
       defMaxHomoLen
    );

    fprintf(
       outFILE,
       "    o Maximum homopolymer size to not discard"
    );

    fprintf(outFILE, "indels in\n");
    */
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
void pTbAmrVersion(FILE *outFILE){
   fprintf(outFILE,"tbAMR version: %i\n",defTbAmrVersion);
}
