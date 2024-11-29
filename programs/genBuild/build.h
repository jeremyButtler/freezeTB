/*-------------------------------------------------------\
' build SOF: Start Of File
'   - holds functions and variables to run build
'   o header:
'     - defined variables, forward declarations, & guards
'   o .h st01: var_build
'     - holds the variables used in build
'   o .h st02: input_build
'     - holds user input variables (over rides all else)
'   o .h st03: cmd_build
'     - holds a user defined build command
'   o .h st04: config_build
'     - holds user defined configure commands
'   o .h st05: file_build
'     - holds a files variables
'   o fun03: blank_var_build
'     - blanks (sets strings to to null) a var_build
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Header:
|   - defined variables, forward declarations, and guards
|   o header sec01:
|     - guards and forward declarations
|   o header sec02:
|     - defined variables, forward declarations, & gaurds
\-------------------------------------------------------*/

#ifndef BUILD_NOT_MAKE_H
#define BUILD_NOT_MAKE_H

typedef struct var_stBuild var_stBuild;
typedef struct varInput_stBuild varInput_stBuild;
typedef struct cmd_stBuild cmd_stBuild;
typedef struct config_stBuild config_stBuild;
typedef struct file_stBuild file_stBuild;

#define def_eof_build 1
#define def_memErr_build 2
#define def_fileErr_build 4
#define def_sysErr_build 8     /*system error*/
#define def_exit_build 16      /*exit command trigured*/

#define def_noVar_build 32     /*variable does not exist*/
#define def_cmdErr_build 64   /*could not file sys cmd*/

/*-------------------------------------------------------\
| ST01: oneVar_build
|   - holds values for a single variable (for parsing ids)
\-------------------------------------------------------*/
typedef struct oneVar_build
{
   signed char *idStr;       /*variable id*/
   unsigned short *lenIdUS;  /*length of id*/
   unsigned short *sizeIdUS; /*max bytes in idStr*/

   signed char *osStr;       /*os variable targets*/
   unsigned short lenOsUS;   /*length of OS*/
   unsigned short sizeOsUS;  /*max bytes in osStr*/

   signed char simdSC:       /*flag with simd type*/
   signed char bitSC:        /*flag with cpu bit type*/
}oneVar_build;

/*-------------------------------------------------------\
| Fun03: sysCmd_build
|   - runs a system command
| Input:
|   - varStr:
|     o c-string with variable id of system command to run
|   - osStr:
|     o list of OS's trying targeting
|   - simdSC:
|     o simd flag using; see osDetect.h for list
|   - bitSC
|     o CPU bit (ex 64bit is def_64bit_osDetect); see
|       osDetect.h for list
|   - varInputSTPtr:
|     o pionter to a varInput_stBuild struct with user
|       input (zero (top) priority)
|   - primVarsSTPtr:
|     o pointer to var_stBuild structure with variables
|       having first priority
   - secVarSTPtr:
|     o pointer to var_stBuild structure with variables
|       having second priority
| Output:
|   - Returns
|     o 0 if command succeded
|     o def_noVar_build if could not find variable with
|       system command
|     o def_cmdErr_build if system command failed to fire
\-------------------------------------------------------*/
signed char
sysCmd_build(
   signed char *varStr,   /*variable with command to run*/
   signed char *osStr[],  /*OS list to target*/
   signed char simdSC,    /*simd type using*/
   signed char bitSC,     /*cpu bit type*/
   struct varInput_stBuild *inputSTPtr, /*user variables*/
   struct var_stBuild *primVarsSTPtr,   /*primary vars*/
   struct var_stBuild *secVarsSTPtr     /*secondary vars*/
);

#endif
