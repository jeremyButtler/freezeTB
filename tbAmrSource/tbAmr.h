/*########################################################
# Name: tbAMR
#  - Determines in reads or input consensuses from
#    tuberculosis are antibiotic resitant or not
########################################################*/

#ifndef TB_AMR_H
#define TB_AMR_H

#define defTbAmrVersion 20240201

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
   char **samStr,     /*Sequence to check for resitance*/
   char **outStr,     /*File to output to*/
   char **idPrefStr,   /*Prefix for read id files*/
   char *readsBl,     /*1: user input reads*/
   unsigned int *minDepthI, /*Min read depth to keep amr*/
   float *minPercMapF,/*Min % support to keep amr (read)*/
   float *minPercTotalF  /*Min % mapped reads to keep*/
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
   void *outVoidFILE
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
void pTbAmrVersion(void *outFILE);

#endif

