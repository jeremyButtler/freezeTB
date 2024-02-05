#include "tbAMR.h" /*Header for this file*/

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

   char *pafStr = 0;   /*File to hold gene positions*/
   char *amrTblStr = 0;/*Table with AMR mutations*/
   char *seqStr = 0;    /*Sequence to check for resitance*/
   int maxHomoI = defMaxHomoLen;/*Max homopolymer size*/
   char whoTblBl = 0;  /*amrTblStr is from the who (csv)*/

   char *errStr = 0;

   ulong numAmrUL = 0;
   FILE *amrFILE = 0;

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
        argsAryStr[],
        &pafStr,     /*File to hold gene positions*/
        &amrTblStr,  /*Table with AMR mutations*/
        &seqStr,     /*Sequence to check for resitance*/
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
   ^  - Check if the amr table file extis/read in
   ^  - For who data print out this programs format
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   amrST = readWhoAmrCsv(amrFILE, &numAmrUL);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Check if the sequence (genome) file extis/read in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^  - Check for resistance mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
|   - pafStr:
|     o Pointer to c-string to hold the path to the paf
|       file
|   - amrTblStr:
|     o Will hold the path to the AMR table to check for
|       antibiotic resitance with
|   - seqStr:
|     o Will hold the path to the gehoeme to check for
|       antibiotic resitiance
|   - maxHomlI:
|     o Will hold the maximum homopolymer length to keep
|       an indel
|   - whoTlbBl:
|     o Holds if amrTblStr pionts to a csv from the
|       WHO or my own tsv file format
|   - fileDelC:
|     0 Holds the deliminator for the AMR table
| Output:
|   - Prints:
|     o The help message for tbAMR to outFILE
\-------------------------------------------------------*/
char * tbAMRGetInput(
   int numArgsI,
   char *argsAryStr[],
   char **pafStr,     /*File to hold gene positions*/
   char **amrTblStr,  /*Table with AMR mutations*/
   char **seqStr,     /*Sequence to check for resitance*/
   int *maxHomoI,     /*Maximum homopolymer size*/
   char *whoTblBl     /*amrTblStr is from the who (csv)*/
){ /*tbAMRGetInput*/

   char *parmStr = 0;
   char *errStr = 0;
   int iArg = 0;

   for(iArg = 1; iArg < numArgsI; ++iArg)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[iArg];

      if(strcmp(parmStr, "-paf") == 0)
      { /*If: this is the gene coordinates paf file*/
         *pafStr = argsAryStr[iArg + 1];
         ++iArg;
      } /*If: this is the gene coordinates paf file*/

      else if(strcmp(parmStr, "-who-amr-tbl") == 0)
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

      else if(strcmp(parmStr, "-seq") == 0)
      { /*Else if: tb sequence to check*/
         *seqStr = argsAryStr[iArg + 1];
         ++iArg;
      } /*Else if: tb sequence to check*/

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
      "tbAMR -seq genome.fasta -who-amr-tbl tbl.csv"
   );

   fprintf(outFILE, " -paf pos.paf\n  or");

   fprintf(
      outFILE,
      " tbAMR -seq genome.fasta -amr-tbl amr-table.tsv\n"
   );

   fprintf(
      outFILE,
      "  - Detects Anti-microbal (antibiotic) resistant"
   );
        
   fprintf(outFILE, " genes\n");
   fprintf(outFILE, "    (AMRs) in an input genome\n");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^  - Print out the user input
   ^   o fun-02 sec-02 sub-01:
   ^     - Print out input line
   ^   o fun-02 sec-02 sub-02:
   ^     - Print out the -paf entry
   ^   o fun-02 sec-02 sub-03:
   ^     - Print out the who table entry
   ^   o fun-02 sec-02 sub-04:
   ^     - Print out the amr table entry
   ^   o fun-02 sec-02 sub-05:
   ^     - Print out the sequence entru
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
   *   - Print out the -paf entry
   \*****************************************************/

    fprintf(outFILE,"  -paf: [REQUIRED FOR WHO TABLE]\n");
    fprintf(
       outFILE,
       "    o Paf file with gene coordiantes of all genes"
    );

    fprintf(outFILE, " in the\n");

    fprintf(
       outFILE,
       "      Tuberculoisis genome (same ref as table)\n"
    );


    fprintf(
       outFILE,
       "    o Make with: minimap2 ref.fasta genes.fasta >"
    );
    fprintf(outFILE, " file.paf\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
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

    fprintf(
       outFILE,
       "    o You also need to provide a paf file (-paf)"
    );

    fprintf(outFILE, "\n");

    fprintf(
       outFILE,
       "      with the genomes cooridantes\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Print out the amr table entry
   \*****************************************************/

    fprintf(outFILE, "  -amr-tbl: [REQUIRED]\n");
    fprintf(
       outFILE,
       "    o Tsv with AMR anotations (replaces -who-amr"
    );

    fprintf(outFILE, "-tbl)\n");

    fprintf(
       outFILE,
       "    o Format: gene-id\tfirst-base\tcodon-start\t"
    );

    fprintf(
       outFILE,
       "      mutation-pattern\namino-mutation\t"
    );

    fprintf(outFILE, "1st-antibiotic\n");

    fprintf(
        outFILE,
        "2nd-antibiotic\t3rd-antibiotic\t...\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
   *   - Print out the sequence entru
   \*****************************************************/

    fprintf(outFILE, "  -seq: [REQUIRED]\n");

    fprintf(
       outFILE,
       "    o Genome to detect antibiotic resistance in\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-06:
   *   - Print out maximum homopolymer length
   \*****************************************************/

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
