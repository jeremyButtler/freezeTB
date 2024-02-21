/*######################################################################
# Name: trimSamFile.c
# Use:
#    - Trims & prints alignments with sequences in a sam file.
#      Alginments without sequences are ignored.
# Input:
#    -file:
#        - Samfile to trim                [Required if -stdin not used]
#    -stdin:
#        - Pipe sam file in with command line [Required if -f not used]
#    -out:
#        - File to save trimmed file to       [Default stdout]
#    -keep-unmapped-reads [No[
#        - Keep reads that do not map to a reference (can not trim)
# Output:
#    stdout: sam file
# Includes:
#    - "trimSam.h"
#    o "samEntryStruct.h"
#    o "cStrToNumberFun.h"
#    o "printError.h"
# C standard libraries:
#   o <stdlib.h>
#   o <string.h>
#   o <stdint.h>
#   k <stdio.h>
######################################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOP:
'   main Main function to glue everything together
'   fun-1 checkInput: Checks the user input [returns: 0 if invalid]
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <string.h> /*strcmp function*/
#include "oldTrimSam.h"
#define defTrimSamVersion 20240215

/*---------------------------------------------------------------------\
| Output: Modifies: Each input variable to hold user input
|    Sets: samPathCStr to piont to sam file name if file name provided
|    Sets: outPathCStr to point to out file name if file name provided
|    Sets: stdinChar to 1 if user is providing stdin input
|    Returns:
|             1: if valid input
|             2: if an invalid parameter was input
|             4: if not a same file
|    Sets: samPathCStr to point to the invalid input
\---------------------------------------------------------------------*/
char checkInput(
    int *lenArgsInt,               /*Number arguments user input*/
    char *argsCStr[],              /*Array with user arguments*/
    char **samPathCStr,            /*file name of input file*/
    char **outPathCStr,            /*File name of output file*/
    char *stdinChar,               /*Tells if taking input from stdin*/
    char *keepUnmapBl              /*Keeping unmapped reads?*/
); /*Checks & extracts user input*/

int main(int lenArgsInt, char *argsPtrCStr[])
{ /*main function*/

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
    ' Main TOC: main
    '    main sec-1: Variable declarations
    '    main sec-2: Read in and check user input
    '    main sec-3: Call the read scoring functions
    '    main Sec-4: Open output file
    \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-1: Variable declarations
    ^    main sec-1 sub-1: normal variable declerations
    ^    main sec-1 sub-2: help message
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /******************************************************************\
    * Main Sec-1 Sub-1: normal variable declerations
    \******************************************************************/

    char *samPathCStr = 0;     /*Path to sam file to work on*/
    char *outPathCStr = 0;     /*sam file to output to*/
    char stdinChar = 0;        /*Char marking if input is from stdin*/
    char keepUnmapBl = 0;      /*Do not keep umapped reads*/
    char errChar = 0;          /*Stores error from checkInput*/

    FILE *samFILE = 0; /*Points to file to get data from*/
    FILE *outFILE;

    /******************************************************************\
    * Main Sec-1 Sub-2: help message
    \******************************************************************/

    char
        *helpMesgCStr = "\
            \n Command: trimSamFile -file reads.sam [options ...]\
            \n Use:\
            \n    - Trims & prints alignments with sequences in a sam\
            \n       file. Alginments without sequences are ignored.\
            \n Output:\
            \n    - stdout: Prints trimmed alignments & headers\
            \n Input:\
            \n    -stdin: [No]\
            \n        - Take input from command line\
            \n    -file or -sam: [Required]\
            \n        - Take input from file\
            \n        - Can be replaced with -stdin\
            \n    -out: [stdout]\
            \n        - File to print trimmed aligments to\
            \n    -keep-unmapped-reads: [No]\
            \n        - Keep unmapped reads\
            \n        - These reads will/can not be trimmed\
            \n";

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-2: Read in and check user input
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errChar =
        checkInput(
            &lenArgsInt,     /*Number of items user input*/
            argsPtrCStr,     /*Arguments the user input*/
            &samPathCStr,    /*Path to the sam file*/
            &outPathCStr,    /*Path to the output file*/
            &stdinChar,      /*Take input from stdin instead of a file*/
            &keepUnmapBl     /*Keep unammped reads?*/
    ); /*Get user input*/

    if(errChar == 0)
    { /*If user requested the help message*/
            fprintf(stderr, "%s\nNo input arguments", helpMesgCStr);
            exit(-1);
    } /*If user requested the help message*/

    if(errChar & 2)
    { /*if the user input an invalid input*/
        if(
            strcmp(samPathCStr, "-h") == 0 ||
            strcmp(samPathCStr, "--h") == 0 ||
            strcmp(samPathCStr, "-help") == 0 ||
            strcmp(samPathCStr, "--help") == 0
        ) { /*If user requested the help message*/
            fprintf(stdout, "%s\n", helpMesgCStr);
            exit(0);
        } /*If user requested the help message*/

        if(
            strcmp(samPathCStr, "-v") == 0 ||
            strcmp(samPathCStr, "-V") == 0 ||
            strcmp(samPathCStr, "-Version") == 0 ||
            strcmp(samPathCStr, "-version") == 0
        ) { /*If the user is requesting the version number*/
            fprintf(
                stdout,
                "trimSamFile version: %i\n",
                defTrimSamVersion
            );
            exit(0);
        } /*If the user is requesting the version number*/


        fprintf(
            stderr,
            "%s\n%s is invalid\n",
            helpMesgCStr,
            samPathCStr
        );
        exit(-1);
    } /*if the user input an invalid input*/

    if(errChar & 4)
    { /*if the user input an invalid file*/
        fprintf(
            stderr,
            "Input file (%s) does not end in .sam\n",
            samPathCStr
        ); /*Let user know the error*/
        exit(-1);
    } /*if the user input an invalid file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-3: Open sam file for reading
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(!samPathCStr || samPathCStr[0] == '-')
       samFILE = stdin;

    else if(stdinChar != 1)
    { /*If using a file for input*/
        samFILE = fopen(samPathCStr, "r");
    
        if(samFILE == 0)
        { /*If was unable to open the sam file*/
            fprintf(
                stderr,
                "Unable to open sam file (%s)\n",
                samPathCStr
            ); /*If file was not valid*/

            exit(-1);
        } /*If was unable to open the sam file*/
    } /*If using a file for input*/

    else samFILE = stdin;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-4: Open output file
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(outPathCStr != 0)
    { /*If user provded the reference the reads mapped to*/
        outFILE = fopen(outPathCStr, "w");

        if(outFILE == 0)
        { /*If malloc failed to create memory*/
            fprintf(
                stderr,
                "Unable to open the output file for writing (%s)\n",
                outPathCStr
            ); /*Let user know that we could not open the file*/
                
            fclose(samFILE);   /*Need to release the file*/
            exit(-1);
        } /*If malloc failed to create memory*/
    } /*If user provded the reference the reads mapped to*/

    else
        outFILE = stdout;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec-5: Call read trimming function
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    trimSamReads(samFILE, outFILE, keepUnmapBl);
        /*handles printing and trimming*/

    exit(0);
} /*main function*/

/*---------------------------------------------------------------------\
| Output: Modifies: Each input variable to hold user input
|    Sets: samPathCStr to piont to sam file name if file name provided
|    Sets: outPathCStr to point to out file name if file name provided
|    Sets: stdinChar to 1 if user is providing stdin input
|    Returns:
|             0: No input
|             1: if valid input
|             2: if an invalid parameter was input
|             4: if not a same file
|    Sets: samPathCStr to point to the invalid input
\---------------------------------------------------------------------*/
char checkInput(
    int *lenArgsInt,               /*Number arguments user input*/
    char *argsCStr[],              /*Array with user arguments*/
    char **samPathCStr,            /*file name of input file*/
    char **outPathCStr,            /*File name of output file*/
    char *stdinChar,               /*Tells if taking input from stdin*/
    char *keepUnmapBl              /*Keeping unmapped reads?*/
) /*Checks & extracts user input*/
{ /*checkInput*/

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
    ' Fun-1 TOC: checkInput
    '  o fun-1 sec-1:
    '    - Variable declerations
    '  o fun-1 sec-2:
    '    - Look through user input
    '  o fun-1 sec-3:
    '    - Check if user provided sam file if -file used
    \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-1 Sec-1: Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *tmpCStr = 0;
    char *singleArgCStr = 0;
    int intArg = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-1 Sec-2: Look through user input
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*0 is program name; so I need to start at 1*/
    for(intArg = 1; intArg < *lenArgsInt; intArg++)
    { /*loop through all user input arguments*/
        singleArgCStr = *(argsCStr +intArg + 1);
        tmpCStr = *(argsCStr + intArg);

        if(!strcmp(tmpCStr, "-file"))
        { /*If: This is the input file*/
            *samPathCStr = singleArgCStr;
            intArg++;
        } /*If: This is the input file*/

        else if(!strcmp(tmpCStr, "-sam"))
        { /*If: This is the input file*/
            *samPathCStr = singleArgCStr;
            intArg++;
        } /*If: This is the input file*/

        else if(!strcmp(tmpCStr, "-stdin"))
            *stdinChar = 1;

        else if(!strcmp(tmpCStr, "-out"))
        { /*Else if: This is the output file*/
            *outPathCStr = singleArgCStr;
            intArg++;
        } /*Else if: This is the output file*/

        else if(!strcmp(tmpCStr, "-keep-unmapped-reads"))
            *keepUnmapBl = 1;

        else
        { /*Else is invalid input*/
            *samPathCStr = tmpCStr;
            return 2;
        } /*Else is invalid input*/

    } /*loop through all user input arguments*/

    return 1; /*input is valid*/
} /*checkInput*/
