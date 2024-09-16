/*########################################################
# Name: rmHomo
#   - Removes indels in homopolymers that are to large
# Libraries:
#   - "../generalLib/rmHomoSIndel.h"      (No .c file)
#   o "../generalLib/samEntryStruct.h"    (No .c file)
#   o "../generalLlb/samEntryStruct.h"    (No .c file)
#   o "../generalLlb/base10StrToNum.h"    (No .c file)
#   o "../generalLlb/dataTypeShortHand.h" (No .c file)
# C Standard Libraries:
#   o <string.h>
#   o <stdlib.h>
#   o <stdio.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - Included header for this file
'  o main: main function for rmHomo
'    - Driver function for rmHomo
'  o fun-01: rmHomoGetInput
'    - Gets the user input for rmHomo
'  o fun-02: pRmHomoHelp
'    - Prints the help message for rmHomo
'  o fun-03: pRmHomoVersion
'    - Prints the version number for tbRmHomo
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included header for this file
\-------------------------------------------------------*/

#include "rmHomo.h" /*Header file for this program*/

/*-------------------------------------------------------\
| Main: main function for rmHomo
|   - Driver function for rmHomo
| SInput:
|   - numArgsSI:
|     o Number of arguments the user input
|   - argsAryStr:
|     o Arguments the user input
| Output:
|   - Prints:
|     o The cleaned up sequences to stdout or the input
|       output file (-sam as samfile, -fastx as fastx
|       file)
\-------------------------------------------------------*/
int main(
   int numArgsSI,
   char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: rmHomo
   '   o main sec-01:
   '     - Variable declerations
   '   o main sec-02:
   '     - Initialize struct5uers, get input and check
   '       input
   '   o main sec-03:
   '     - Check Input files
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*User input variables*/

   char *seqStr = 0;   /*Sequences to clean up*/
   char *refStr = 0;   /*reference sequence*/
   char *outStr = 0;   /*File to output to*/

   int maxHomoSI = defMaxHomoLen;/*Max homopolymer size*/
   int minQSI = defMinBaseQ; /*min q-score to keep ins*/

   /*Detect the type of file the input sequence is*/
   char fastxFlag = defFastxFlag;

   FILE *seqFILE = 0;
   FILE *outFILE = 0;
   FILE *refFILE = 0;

   struct refST;
   struct seqST; /*For fastx input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^  - Initialize struct5uers, get input and check input
   ^  o main sec-02 sub-01:
   ^    - Initialize structures and get user input
   ^  o main sec-02 sub-02:
   ^    - Check if had an error when getting input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /****************************************************\
   * Main Sec-02 Sub-01:
   *  - Initialize structures and get user input
   \****************************************************/

   initSeqST(&seqST);
   initSeqST(&refST);

   errStr =
     tbAMRGetSInput(
        numArgsSI,
        argsAryStr,
        &seqStr,      /*Sequence(s) to check*/
        &refStr,      /*Refernce sequence used/to use*/
        &maxHomoSI,   /*Maximum homopolymer size*/
        &minQSI,      /*Min q-score to keep insertion*/
        &fastxFlag,   /*File type input for seqStr*/
        &outStr
     ); /*Get the user input*/

   /****************************************************\
   * Main Sec-02 Sub-02:
   *  - Check if had an error when getting input
   \****************************************************/

   if(errStr)
   { /*If: SI had an error*/

      if(   strcmp(errStr, "-h") == 0
         || strcmp(errStr, "--h") == 0
         || strcmp(errStr, "-help") == 0
         || strcmp(errStr, "--help") == 0
         || strcmp(errStr, "help") == 0
      ){ /*If: the user requested the help message*/
         pRmHomoHelp(stdout);
         exit(0);
      } /*If: the user requested the help message*/


      if(   strcmp(errStr, "-v") == 0
         || strcmp(errStr, "--v") == 0
         || strcmp(errStr, "-version") == 0
         || strcmp(errStr, "--version") == 0
         || strcmp(errStr, "version") == 0
      ){ /*If: the user requested the version number*/
         pRmHomoVersion(stdout);
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

      if(strcmp(errStr, "-min-q") == 0)
      { /*If: the user input non-numeric argument*/
         fprintf(
            stderr,
            "Argument from -min-q is not a number"
         );
         fprintf(stderr, "\n");
         exit(-1);
      } /*If: the user input non-numeric argument*/

      pRmHomoHelp(stderr);

      fprintf(
         stderr,
         "%s is not an valid argument\n",
         errStr
      );

      exit(-1);
   } /*If: SI had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^  - Check Input files
   ^  o main sec-03 sub-01:
   ^    - Check if the sequence file was valid
   ^  o main sec-03 sub-02:
   ^    - Check if the output file was valid
   ^  o main sec-03 sub-03:
   ^    - Check and read in reference file (if input)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec-03 Sub-01:
   *   - Check if the sequence file was valid
   \*****************************************************/

   if(seqStr && seqStr[0] != '-')
   { /*If: The I am not using stdin*/
      seqFILE = fopen(seqStr, "r");

      if(seqFILE == 0)
      { /*If: I could not open the sequence file*/
         fprintf(stderr, "Unable to open");

         if(fastxFlag == defSamInput)
            fprintf(stderr, "-sam %s\n", seqStr);

         else if(fastxFlag == defFastaInput)
            fprintf(stderr, "-fasta %s\n", seqStr);

         else if(fastxFlag == defFastqInput)
            fprintf(stderr, "-fastq %s\n", seqStr);

         exit(-1);
      } /*If: I could not open the sequence file*/
   } /*If: The I am not using stdin*/

   else seqFILE = stdin;

   /*****************************************************\
   * Main Sec-03 Sub-02
   *   - Check if the output file was valid
   \*****************************************************/

   if(outStr && outStr[0] != '-')
   { /*If: The I am not using stdin*/
      outFILE = fopen(outStr, "r");

      if(outFILE == 0)
      { /*If: I could not open the output file*/
        fprintf(stderr,"Could not open -out %s\n" outStr);
        exit(-1);
      } /*If: I could not open the output file*/
   } /*If: The I am not using stdin*/

   else outFILE = stdout;

   /*****************************************************\
   * Main Sec-03 Sub-03:
   *   - Check if the reference file was valid
   \*****************************************************/

   if(refStr == 0 && fastxFlag != defSamInpput)
   { /*If: no refence file was input for -fast[aq]*/

      if(fastxFlag == defFastaInput && seqStr)
         fprintf(stderr, "-fasta %s", seqStr);

      else if(fastxFlag == defFastaInput)
         fprintf(stderr, "-fasta -");

      else if(fastxFlag == defFastqInput && seqStr)
         fprintf(stderr, "-fastq %s", seqStr);

      else if(fastxFlag == defFastaInput)
         fprintf(stderr, "-fastq -");

      fprintf(stderr, " needs -ref reference.fa\n");

      fclose(seqFILE);
      fclose(outFILE);

      exit(-1);
   } /*If: no refence file was input for -fast[aq]*/

   if(refStr)
   { /*If: the user input a reference sequence*/
      refFILE = fopen(refStr, "r");

      if(refFILE == 0)
      { /*If: I could not open the WHO catalog*/
        fprintf(stderr,"Could not open -ref %s\n",refStr);

        fclose(seqFILE);
        fclose(outFILE);

        exit(-1);
      } /*If: I could not open the WHO catalog*/

      errUC = readFaSeq(refFILE, &refST);

      if(errUC > 1)
      { /*If: I had a memoery error*/
         if(errUC & 64)
         { /*If: This was a memory error*/
            fprintf(
              stderr,
              "Ran out of memory while getting the"
            );

            fprintf(stderr, " reference sequence\n");
         } /*If: This was a memory error*/

         /*Else: invalid fasta file*/
         else
           fprintf(
             stderr,
             "-ref %s is not a fasta file/has an error\n",
              refStr
           );

         fclose(seqFILE);
         fclose(refFILE);
         fclose(outFILE);
         freeSeqSTStack(&refST);

         exit(-1);
      } /*If: I had a error*/

      fclose(refFILE);
   } /*If: the user input a reference sequence*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Clean up indels
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fastxFlag == defSamInput)
      samRmHomoIndels(
         seqFILE,
         maxHomoSI,
         minQSI,
         refST.seqCStr,
         outFILE
      );

   else if(fastxFlag == defFastaInput)
   else if(fastxFlag == defFastqInput)

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-05:
   ^  - Check for resistance mutations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   freeAmrStructArray(amrST, numAmrUL);
   exit(0);
} /*main*/

/*-------------------------------------------------------\
| Fun-01: rmHomoGetInput
|   - Gets the user input for rmHomo
| SInput:
|   - numArgsSI:
|     o Number of arguments the user input
|   - argsAryStr:
|     o Arguments the user input
|   - seqStr:
|     o Pointer to c-string to hold the path to the
|       sequence
|   - refStr:
|     o C-string to hold the path to the file with the
|       reference sequence to use
|   - maxHomlSI:
|     o Will hold the maximum homopolymer length to keep
|       an indel
|   - minQSI:
|     o Will hold the mininum q-score to keep an insertion
|   - fastxFlag:
|     o Set to 0 if the user input a sam file (-sam)
|     o Set to 1 if the user input a fasta file
|     o Set to 2 if the user input a fastq file
|   - refStr:
|     o C-string to hold the path to the file to output
|       cleaned sequences to
| Output:
|   - Modifies:
|     o seqStr, refStr, maxHomoSI, and fastxFlag to hold
|       the user input
\-------------------------------------------------------*/
char * rmHomoGetInput(
   int numArgsSI,
   char *argsAryStr[],
   char **seqStr,     /*Sequences to remove indels from*/
   char **refStr,     /*Reference sequence to use*/
   int *maxHomoSI,    /*Maximum homopolymer size*/
   int *minQSI,       /*Min q-score to keep an insertion*/
   char *fastxFlag,   /*0: Input a sam file; else fastx*/
   char *outStr       /*File to output to*/
){ /*rmHomoGetInput*/

   char *parmStr = 0;
   char *errStr = 0;
   int isArg = 0;

   for(siArg = 1; siArg < numArgsSI; ++siArg)
   { /*Loop: Get user input*/
      parmStr = argsAryStr[siArg];

      if(strcmp(parmStr, "-sam") == 0)
      { /*If: a sam file was input*/
         *seqStr = argsAryStr[siArg + 1];
         *fastxFlag = defSamInput;
         ++siArg;
      } /*If: a sam file was input*/

      else if(strcmp(parmStr, "-fasta") == 0)
      { /*Else If: a fast[aq] file was input*/
         *seqStr = argsAryStr[siArg + 1];
         *fastxFlag = defFastaInput;
         ++siArg;
      } /*Else If: a fast[aq] file was input*/

      else if(strcmp(parmStr, "-fastq") == 0)
      { /*Else If: a fast[aq] file was input*/
         *seqStr = argsAryStr[siArg + 1];
         *fastxFlag = defFastqInput;
         ++siArg;
      } /*Else If: a fast[aq] file was input*/

      else if(strcmp(parmStr, "-ref") == 0)
      { /*Else If: a reference file was input*/
         *refStr = argsAryStr[siArg + 1];
         ++siArg;
      } /*Else If: a reference file was input*/

      else if(strcmp(parmStr, "-max-homo-len") == 0)
      { /*Else if: Maximum homopolyer length for indel*/
         errStr =
            base10StrToSSI(
               argsAryStr[siArg + 1],
               *maxHomoSI
            ); /*Convert user input string to number*/

         if(*errStr > 32) return parmStr;
         ++siArg;
      } /*Else if: Maximum homopolyer length for indel*/

      else if(strcmp(parmStr, "-min-q") == 0)
      { /*Else if: min q-score for insertion*/
         errStr =
            base10StrToSSI(
               argsAryStr[siArg + 1],
               *minQSI
            ); /*Convert user input string to number*/

         if(*errStr > 32) return parmStr;
         ++siArg;
      } /*Else if: min q-score for insertion*/

      else if(strcmp(parmStr, "-out") == 0)
      { /*Else If: an output file was input*/
         *outStr = argsAryStr[siArg + 1];
         ++siArg;
      } /*Else If: an output file was input*/

      else return parmStr;
   } /*Loop: Get user input*/

   return 0;
} /*rmHomoGetInput*/

/*-------------------------------------------------------\
| Fun-02: pRmHomoHelp
|   - Prints the help message for rmHomo
| SInput:
|   - outFILE:
|     o File to print the help message for rmHomo to
| Output:
|   - Prints:
|     o The help message for rmHomo to outFILE
\-------------------------------------------------------*/
void pRmHomoHelp(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02: pRmHomoHelp
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
      "rmHomo -sam sequence.sam"
   );

   fprintf(
      outFILE,
      "  - Removes indels in homopolymer regions\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun-02 Sec-02:
   ^  - Print out the user input
   ^   o fun-02 sec-02 sub-01:
   ^     - Print out input line
   ^   o fun-02 sec-02 sub-02:
   ^     - Print out the -sam entry
   ^   o fun-02 sec-02 sub-03:
   ^     - Print out the fastx option
   ^   o fun-02 sec-02 sub-04:
   ^     - Print out the -ref option
   ^   o fun-02 sec-02 sub-05:
   ^     - Print out maximum homopolymer length
   ^   o fun-02 sec-02 sub-06:
   ^     - Print out minimum Q-score entry (for ins)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun-02 Sec-02 Sub-01:
   *   - Print out input line
   \*****************************************************/

    fprintf(outFILE, "Input:\n");

   /*****************************************************\
   * Fun-02 Sec-02 Sub-02:
   *   - Print out the -sam entry
   \*****************************************************/

    fprintf(outFILE,"  -sam: [REQUIRED]\n");
    fprintf(
       outFILE,
       "    o Sam file with mapped sequences to remove"
    );

    fprintf(
       outFILE,
       "      homopolymers from\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-03:
   *   - Print out the fastx option
   \*****************************************************/

    fprintf(outFILE, "  -fastx: [REPLACES -sam]\n");

    fprintf(
       outFILE,
       "    o Fasta or fastq (fastx) file with sequences/"
    );

    fprintf(outFILE, "\n");

    fprintf(
       outFILE,
       "      reads to align to a reference\n"
    );

    fprintf(
       outFILE,
       "    o A reference genome (-ref ref.fasta) is"
    );

    fprintf(outFILE, "required\n");

    fprintf(
       outFILE,
       "      when using the -fastx file.fastx option\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-04:
   *   - Print out the -ref optipon
   \*****************************************************/

    fprintf(outFILE, "  -ref: [None]\n");
    fprintf(
       outFILE,
       "    o Reference sequence to use to correct"
    );

    fprintf(outFILE, " deletions\n");

    fprintf(
       outFILE,
       "      in sam files or map -fastx entries to\n"
    );

   /*****************************************************\
   * Fun-02 Sec-02 Sub-05:
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

   /*****************************************************\
   * Fun-02 Sec-02 Sub-06:
   *   - Print out minimum Q-score entry (for ins)
   \*****************************************************/

    fprintf(outFILE, "  -min-a: [%i]\n", defMinBaseQ);

    fprintf(
       outFILE,
       "    o Minimum Q-score to keep an insertion\n"
    );
} /*pRmHomoHelp*/

/*-------------------------------------------------------\
| Fun-03: pRmHomoVersion
|   - Prints the version number for tbRmHomo
| SInput:
|   - outFILE:
|     o File to print the version number for rmHomo to
| Output:
|   - Prints:
|     o The version for rmHomo to outFILE
\-------------------------------------------------------*/
void pRmHomoVersion(FILE *outFILE){
   fprintf(
      outFILE,
      "rmHomo version: %i\n",
      defRmHomoVersion
   );
} /*pRmHomoVersion*/
