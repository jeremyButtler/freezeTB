/*########################################################
# Name: tbAMR
#  -
# Libraries:
#   - "../generalLib/geneCoordStruct.h"
#   o "../generalLib/genMath.h"
#   - "checkAmr.h"
#   o "../generalLib/trimSam.h"
#   o "../generalLib/samEntryStruct.h"
#   o "../generalLib/base10StrToNum.h"
#   o "../generalLib/dataTypeShortHand.h"
#   o "ulCpStr.h"
#   o "amrStruct.h"
#   o "../generalLib/codonTbl.h"
# C Standard Libraries:
#   o <stdlib.h>
#   o <stdio.h>
#   o <string.h>
#   o <limits.h>
########################################################*/

#ifndef TB_AMR_H
#define TB_AMR_H

#include "../generalLib/geneCoordStruct.h"
#include "checkAmr.h"

#define defTbAmrVersion 20240201
#define defWhoTblSep ','  /*Deleminator for who table*/
#define defAmrTblSep '\t' /*Deliminator for my amr table*/
#define defMaxHomoLen 1   /*Maximum homopolymer length*/


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
|   - seqStr:
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
   char **seqStr,     /*Sequence to check for resitance*/
   char **outStr,     /*File to output to*/
   int *maxHomoI,     /*Maximum homopolymer size*/
   char *whoTblBl     /*amrTblStr is from the who (csv)*/
); /*tbAMRGetInput*/

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
); /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02: pHelp
   '   - fun-02 sec-01:
   '     o Print out the usage/description
   '   - fun-02 sec-02:
   '     o Print out the user input
   '   - fun-02 sec-03:
   '     o Print out the output (TODO)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


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
void pTbAmrVersion(FILE *outFILE);

#endif

