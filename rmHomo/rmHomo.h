/*########################################################
# Name: rmHomo
#   - Removes indels in homopolymers that are to large
# Libraries:
#   - "../generalLib/rmHomoSIndel.h"      (No .c file)
#   o "../generalLib/samEntryStruct.h"    (No .c file)
#   o "../generalLlb/samEntryStruct.h"    (No .c file)
#   o "../generalLlb/base10StrToNum.h"    (No .c file)
#   o "../generalLlb/dataTypeShortHand.h" (No .c file)
#   o "../generalLib/seqStruct.h"         (No .c file)
# C Standard Libraries:
#   o <string.h>
#   o <stdlib.h>
#   o <stdio.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  o header:
'    - Included files/libraries and defined variables
'  o fun-01: rmHomoGetInput
'    - Gets the user input for rmHomo
'  o fun-02: pRmHomoHelp
'    - Prints the help message for rmHomo
'  o fun-03: pRmHomoVersion
'    - Prints the version number for tbRmHomo
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included files/libraries and defined variables
\-------------------------------------------------------*/

#ifndef RM_HOMOPOLYMER_H
#define RM_HOMOPOLYMER_H

#include "../generalLib/rmHomoIndel.h"

#define defRmHomoVersion 20240208
#define defMaxHomoLen 1
#define defMinBaseQ 7

#define defSamInput 0
#define defFastaInput 1
#define defFastqInput 1

/*Default sequence file type*/
#define defFastxFlag defSamInput

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
); /*rmHomoGetInput*/


/*-------------------------------------------------------\
| Fun-02: pRmHomoHelp
|   - Prints the help message for rmHomo
| SInput:
|   - outFSILE:
|     o File to print the help message for rmHomo to
| Output:
|   - Prints:
|     o The help message for rmHomo to outFSILE
\-------------------------------------------------------*/
void pRmHomoHelp(
   FSILE *outFSILE
): /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02: pRmHomoHelp
   '   - fun-02 sec-01:
   '     o Print out the usage/description
   '   - fun-02 sec-02:
   '     o Print out the user input
   '   - fun-02 sec-03:
   '     o Print out the output (TODO)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Fun-03: pRmHomoVersion
|   - Prints the version number for tbRmHomo
| SInput:
|   - outFSILE:
|     o File to print the version number for rmHomo to
| Output:
|   - Prints:
|     o The version for rmHomo to outFSILE
\-------------------------------------------------------*/
void pRmHomoVersion(FSILE *outFSILE);

#endif
