/*########################################################
# Name: subsampleIds
# Use:
#   - Subsamples read ids by score from a sam file
# Libraries:
#   - "readIdSubsample.h"
#   o "minAlnStatsStruct.h"
#   o "../generalLib/trimSam.h"
#   o "../generalLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   o "../generalLib/ulCpStr"
#   o "../generalLib/numToStr.h"
# C Standard Libraries:
#   - <string.h>
#   o <stdlib.h>
#   o <stdio.h>
#   o <limits.h>
########################################################*/

#include "readIdSubsample.h"
#include <string.h>
#define defVersion 20240108

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOP: Start Of Program
'  o Main:
'    - function that runs everything
'  o fun-1: checkInput 
'   - check and process the user input
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Output:
|    Returns: 0 if no errors, pointer to argumet errored on for errors
|    Modifies: Every input varible to hold user input
\-------------------------------------------------------*/
char * checkInput(
    int *lenArgsInt,             /*Number of arugments user input*/
    char *argsStr[],            /*Argumenas & parameters input*/
    char **samFileStr,           /*Will hold path to reads fastq file*/
    char **outFileStr,          /*Will hold path of output file*/
    ulong *numReadsToExtUL, /*Number of reads to extract*/
    struct minAlnStats *minStats /*Holds mininum stats to keep a read*/
); /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   | Fun-1 TOC: Sec-1 Sub-1: checkInput
   |  - Checks user input & puts input into variables for later use
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main(int lenArgsInt, char *argsCStr[])
{ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
  ' Main TOC: main function
  '  o main sec-1: variable declerations
  '  o main sec-2: Check user input
  '  o main sec-3: Check if can open the primer fasta file
  '  o main sec-4: Check if can open fastq file
  '  o main sec-5: Check if can open output file
  '  o main sec-6: Run function to trim reads
  \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-1: variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *samFileStr = 0;     /*file to open*/
    char *outFileCStr = 0;    /*file to write to*/
    char *inputChar = 0;      /*Holds arguemnt that had input error*/

    ulong numReadsToExtractUL = 300;
    ulong numReadsExtractedUL = 0;
    ulong ulId = 0; /*For loop printing out ids*/

    FILE *outFILE = 0;
    FILE *samFILE = 0;

    struct minAlnStats minStats;
    struct samEntry samST;       /*For file reading*/
    struct idStack *idsST;

    char *helpMesgCStr = "\
        \n filterReads -fastq rads.fastq [options ...]\
        \n Use: Uses primer mappings to trim reads in a fastq file.\
        \n Input:\
        \n   -sam file.sam: [Required]\
        \n     o Sam file to extract reads from.\
        \n     o Use an eqx (minimap2 --eqx) cigar entry\
        \n     o \"-sam -\" means stdin input\
        \n   -out: [stdout]\
        \n     o output file to print read ids to\
        \n   -num-reads: [300]\
        \n     o Maximum number of reads to extract\
        \n   -min-mapq: [20]\
        \n     o min mapping quality to keep a read.\
        \n   -min-median-q: [10]\
        \n     o Remove reads with median Q-scores under\
        \n       this Q-score\
        \n   -min-mean-q: [10]\
        \n     o Remove reads with mean Q-scores under\
        \n   -min-length: [200]\
        \n     o Remove reads under this aligned length.\
        \n   -max-length: [3000]\
        \n     o Remove reads over this length\
        \n     o 0 means keep all reads\
        \n   -min-snps: [0.07=7%]\
        \n     o Maximum percent difference in SNPs\
        \n       between read and reference to keep read.\
        \n   -min-diff: [1=100%]\
        \n     o Maximum percent difference between read\
        \n       reference to keep read.\
        \n   -min-inss: [1=100%]\
        \n     o Maximum percent difference in insertions\
        \n       between read and reference to keep read.\
        \n   -min-dels: [1=100%]\
        \n     o Maximum percent difference in deletions\
        \n       between read and reference to keep read.\
        \n   -min-indels: [1=100%]\
        \n     o Maximum percent difference in indels\
        \n       between read and reference to keep read.\
        \n   -v:\
        \n     o Print version & exit\
        \n Output:\
        \n   stdout: prints out the kept reads to stdout\
         "; /*Help message*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-2: Check user input
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    blankMinStats(&minStats); /*Set up default values (read/ref map)*/
    initSamEntry(&samST); /*Blank the sam entry struct (avoids errors)*/

    inputChar =
        checkInput(
            &lenArgsInt,
            argsCStr,
            &samFileStr,
            &outFileCStr,
            &numReadsToExtractUL,
            &minStats
    ); /*Get the user input*/

    if(inputChar != 0)
    { /*If I have an error or a non-run request*/
        if(strcmp(inputChar, "-h") == 0 ||
           strcmp(inputChar, "--h") == 0 ||
           strcmp(inputChar, "-help") == 0 ||
           strcmp(inputChar, "--help") == 0 ||
           strcmp(inputChar, "help") == 0
        ) { /*If user wanted the help message*/
            fprintf(stdout, "%s\n", helpMesgCStr);
            exit(0);
        } /*If user wanted the help message*/

        if(strcmp(inputChar, "-V") == 0 ||
           strcmp(inputChar, "-v") == 0 ||
           strcmp(inputChar, "--V") == 0 ||
           strcmp(inputChar, "--v") == 0 ||
           strcmp(inputChar, "--version") == 0 ||
           strcmp(inputChar, "--Version") == 0 ||
           strcmp(inputChar, "-version") == 0 ||
           strcmp(inputChar, "-Version") == 0
        ) { /*if the user wanted the version number*/
            fprintf(
                stdout,
                "subsampleIds version %i\n",
                defVersion
            ); /*Print out the closest thing to a version*/
            exit(0);
        } /*Else if the user wanted the version number*/

        else if(inputChar != 0)
        { /*If user had invalid input*/
            fprintf(
                stderr,
                "%s\n%s is invalid\n",
                helpMesgCStr,
                inputChar
            ); /*Print out the problem*/

            exit(1); /*Let user know their was an error*/
        } /*If user had invalid input*/
     } /*If I have an error or a non-run request*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-3: Check if can open fastq file
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(samFileStr == 0)
    { /*If no file input*/
       fprintf(stderr, "No sam file provied with -sam\n");
       exit(-1);
    } /*If no file input*/

    if(samFileStr[0] == '-') samFILE = stdin;
 
    else
    { /*Else: the input is from a file*/
       samFILE = fopen(samFileStr, "r");

       if(samFILE == 0)
       { /*If no file was oppened*/
          fprintf(
             stderr,
             "-sam %s could not be opened\n",
             samFileStr
          );

          exit(-1);
       } /*If no file was oppened*/
    } /*Else: the input is from a file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-5: Check if can open output file
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(outFileCStr != 0)
    { /*If taking input from file*/
        outFILE = fopen(outFileCStr, "w");    /*Re-using the out file*/
    
        if(outFILE == 0)
        { /*If no file was oppened*/
            fprintf(
                stderr,
                "Could not open output file (-out %s)\n",
                outFileCStr
            ); /*If the filter file was invalid, let the user know*/

            fclose(samFILE);
            exit(-1);
        } /*If no file was oppened*/
    } /*If taking input from file*/

    else outFILE = stdout;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-6: Run function to trim reads
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    idsST = 
       getXSamIds(
          numReadsToExtractUL,
          &numReadsExtractedUL,
          1, /*Use the mapping quality*/
          &minStats,
          samFILE
       ); /*Subsample reads*/

    freeSamEntryStack(&samST);
    fclose(samFILE);
    samFILE = 0;

    if(!idsST)
    { /*If had a memory allocation error*/
        fprintf(stderr, "Memory or file error\n");
        fclose(outFILE);
        exit(-1);
    } /*If had a memory allocation error*/

    for(ulId = 0; ulId < numReadsExtractedUL; ++ulId)
       fprintf(outFILE, "%s\n", idsST[ulId].idStr);

    fclose(outFILE);
    freeIdStackAry(idsST);
    exit(0);
} /*main function*/

/*---------------------------------------------------------------------\
| Output:
|    Returns: 0 if no errors, pointer to argumet errored on for errors
|    Modifies: Every input varible to hold user input
\---------------------------------------------------------------------*/
char * checkInput(
    int *lenArgsInt,             /*Number of arugments user input*/
    char *argsStr[],            /*Argumenas & parameters input*/
    char **samFileStr,           /*Will hold path to reads fastq file*/
    char **outFileStr,          /*Will hold path of output file*/
    ulong *numReadsToExtUL, /*Number of reads to extract*/
    struct minAlnStats *minStats /*Holds mininum stats to keep a read*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   | Fun-1 TOC: Sec-1 Sub-1: checkInput
   |  - Checks user input & puts input into variables for later use
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    char *parmStr = 0;
    char *singleArgStr = 0;
    int iArg = 0;

    if(*lenArgsInt < 2)
        return parmStr; /*no arguments input*/

    for(iArg = 1; iArg < *lenArgsInt; iArg++)
    { /*loop through all user input arguments*/  /*0 is program name*/
        singleArgStr = *(argsStr +iArg + 1); /*supplied argument*/
        parmStr = *(argsStr + iArg);          /*Paramter*/

        if(strcmp(parmStr, "-sam") == 0)
            *samFileStr = singleArgStr;

        else if(strcmp(parmStr, "-out") == 0)
            *outFileStr = singleArgStr;

        else if(strcmp(parmStr, "-num-reads") == 0)
            *numReadsToExtUL = strtoul(singleArgStr, &parmStr, 10);

        /*General filtering settings*/
        else if(strcmp(parmStr, "-min-median-q") == 0)
            sscanf(singleArgStr, "%f", &minStats->minMedianQFlt);

        else if(strcmp(parmStr, "-min-mean-q") == 0)
            sscanf(singleArgStr, "%f", &minStats->minMeanQFlt);

       else if(strcmp(parmStr, "-min-length") == 0)
           minStats->minReadLenULng=strtoul(singleArgStr,&parmStr,10);

       else if(strcmp(parmStr, "-max-length") == 0)
           minStats->maxReadLenULng=strtoul(singleArgStr,&parmStr,10);

       else if(strcmp(parmStr, "-min-mapq") == 0)
           base10StrToUI(singleArgStr, minStats->minMapqUInt);

       /*Percent difference settings*/
       else if(strcmp(parmStr, "-min-snps") == 0)
           sscanf(singleArgStr, "%f", &minStats->minSNPsFlt);

       else if(strcmp(parmStr, "-min-diff") == 0)
           sscanf(singleArgStr, "%f", &minStats->minDiffFlt);

       else if(strcmp(parmStr, "-min-inss") == 0)
           sscanf(singleArgStr, "%f", &minStats->minInssFlt);

       else if(strcmp(parmStr, "-min-dels") == 0)
           sscanf(singleArgStr, "%f", &minStats->minDelsFlt);

       else if(strcmp(parmStr, "-min-indels") == 0)
           sscanf(singleArgStr, "%f", &minStats->minIndelsFlt);

       else
           return parmStr;

        iArg++; /*Move to the parameter, so next input is a flag*/
    } /*loop through all user input arguments*/

    return 0; /*input is valid*/
} /*checkInput*/
