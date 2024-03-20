/*########################################################
# Name: tbCon-inputFus.h
#  - Holds functions for getting input, printing version
#    numbers, and printing help messages
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - Definitions and header guard
'   o fun-01: checkTbConArg
'     - Checks user input to see if was a valid argument,
'       if so, this then sets the argument
'   o fun-02: pTbConHelp
'     - Prints out the tbCon help message
'   o fun-03: pTbConVersion
'     - Prints out the version number for tbCon
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Defintions and headeer guards
\-------------------------------------------------------*/

#ifndef TB_CONSENSUS_INPUT_H
#define TB_CONSENSUS_INPUT_H

/*Error message flags for user input*/
#define def_tbCon_input_nonNum_Err 1
#define def_tbCon_input_unkown_Err 2
#define def_tbCon_input_pHelp 4
#define def_tbCon_input_pFullHelp 8 
#define def_tbCon_input_pVersion 16

/*-------------------------------------------------------\
| Fun-01: checkTbConArg
|   - Checks user input to see if was a valid argument,
|     if so, this then sets the argument
| Input:
|   - parmStr:
|     o c-string with the paramter user input (flag)
|   - argStr
|     o c-string with the argument user input
|   - samFileStr:
|     o Pointer to c-string to hold the path to the sam
|       file
|   - outFileStr:
|     o Pointer to the file to output the sam entries for
|       the consensus to
|   - tsvFileStr:
|     o Pointer to the tsv file to output the variants
|       to
|   - refLenUI:
|     o Pointer to unsigned int to hold the reference
|       length
|   - tbConhSetSTPtr:
|     o Pointer to tbConSet structure with other settings
|   - shiftC:
|     o Pointer to character to hold 1, argument read,
|       or 0; parmater was a boolean
| Output:
|    - Modifies:
|      o All variables input to their specified values
|    - Returns:
|      o 0 for no errors
|      o def_tbCon_input_pHelp for the help message
|      o def_tbCon_pFullHelp | def_tbCon_pHelp for the
|        full help message
|      o def_tbCon_input_pVersion for the version number
|      o def_tbCon_input_nonNum_Err for non-numeric number
|      o def_tbCon_input_unkown_Err for an unkown entry
\-------------------------------------------------------*/
short
checkTbConArg(
   char *parmStr,/*Parameter for argument (flag)*/
   char *argStr, /*Argument (user input)*/
   char **samFileStr, /*Input sam file to use*/
   char **outFileStr, /*File to output to*/
   char **tsvFileStr, /*Path to tsv file to pring*/
   unsigned int *refLenUI,    /*Length of reference*/
   void *tbConSetSTPtr,
   char *shiftC        /*How much to shift argument by*/
);

/*-------------------------------------------------------\
| Fun-03: pTbConVersion
|   - Prints out the tbCon help message
| Input:
|   - pFullC:
|     o 1: Print out the full help message (TODO: add in)
|     o 0: Print out the short help message
|   - outVoidFILE:
|     o FILE pointer (FILE *outFILE) to file to print to
| Output:
|   - Prints:
|     o tbCon help message to outFILE
\-------------------------------------------------------*/
void
pTbConHelp(
   char pFullC,
   void *outVoidFILE
);

/*-------------------------------------------------------\
| Fun-03: pTbConVersion
|   - Prints out the version number for tbCon
| Input:
|   - outFILE:
|     - FILE pointer (FILE *outFILE) to file to print to
| Output:
|   - Prints:
|     o version number of tbCon to outFILE
\-------------------------------------------------------*/
void
pTbConVersion(
   void *outFILE
); /*pTbConVersion*/

#endif
