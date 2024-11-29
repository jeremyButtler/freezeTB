/*-------------------------------------------------------\
' stBuild SOF: Start Of File
'   - has build structures and their supporting functions
'   o header:
'     - defined variables and guards
'   o tof01:
'     - var_stBuild structure and dedicated functions
'   o tof02:
'     - input_stBuild structure and dedicated functions
'   o tof02:
'     - input_stBuild structure and dedicated functions
'   o tof03:
'     - cmd_stBuild structure and dedicated functions
'   o tof04:
'     - config_stBuild structure and dedicated functions
'   o tof05:
'     - file_stBuild structure and dedicated functions
'   o fun01: blank_var_stBuild
'     - blanks (sets strings to to null) a var_stBuild
\-------------------------------------------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof01:
'   - var_stBuild structure and dedicated functions
'   o .h st01: var_stBuild
'     - holds the variables used in build
'   o fun01: blank_var_stBuild
'     - blanks (sets strings to to null) a var_stBuild
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| ST01: var_stBuild
|   - holds a single build variable
\-------------------------------------------------------*/
typedef struct var_stBuild
{
   signed char *idStr;       /*variable id*/
   unsigned short lenIdUS;   /*length of id*/
   unsigned short sizeIdUS;  /*max bytes in idStr*/

   signed char *valStr;      /*variable value*/
   unsigned int lenValUI;    /*length of value*/
   unsigned int sizeValUI;   /*max bytes in valStr*/

   signed char *osStr;       /*os variable targets*/
   unsigned short lenOSUS;   /*length of OS*/
   unsigned short sizeOSUS;  /*max bytes in osStr*/

   signed char simdSC:       /*flag with simd type*/
   signed char bitSC:        /*flag with cpu bit type*/
}oneVar_stBuild;

/*-------------------------------------------------------\
| ST01: var_stBuild
|   - holds the variables used in build
\-------------------------------------------------------*/
typedef struct varAry_stBuild
{
   /*variable arrays*/
   signed char **valAryStr;  /*variable values*/
   unsigned int *sizeValUI;  /*size of variable values*/
   unsigned int *lenValUI;   /*length of contents*/

   signed char **idAryStr;   /*variable ids (names)*/
   unsigned short *sizeIdUS; /*size of id arrays*/

   signed char **osAryStr;   /*OS targeted by variable*/
   unsigned short *sizeOsUS; /*size of each OS id*/

   signed char *simdArySC;  /*vector type of variable*/
   signed char *bitArySC;   /*OS bit target of varialbe*/

   unsigned long lenVarUL;   /*number of variables*/
   unsigned long sizeVarUL;  /*maximum variables*/
}var_stBuild;

/*-------------------------------------------------------\
| Fun01: blank_var_stBuild
|   - blanks (sets strings to to null) a var_stBuild
| Input:
|   - varSTPtr:
|     o pointer to var_stBuild struct to blank
| Output:
|    - Modifies:
|      o all lengths to be 0
|      o all arrays to bet to 0/null/default values
\-------------------------------------------------------*/
void
blank_var_stBuild(
   struct var_stBuild *varSTPtr
);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof02:
'   - input_stBuild structure and dedicated functions
'   o .h st02: input_stBuild
'     - holds user input variables (over rides all else)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| ST02: input_stBuild
|   - holds user input variables (over rides all else)
\-------------------------------------------------------*/
varInput_stBuild
{
   signed char *osStr;      /*usr input OS*/
   unsigned short sizeOSUS; /*size of OS array*/

   signed char simdAryStr;  /*vector type of variable*/
   signed char bitAryStr;   /*OS bit target of varialbe*/

   signed char **valAryStr;  /*variable values*/
   unsigned int *sizeValUI;  /*size of variable values*/
   unsigned int *lenValUI;   /*length of contents*/

   signed char **idAryStr;   /*variable ids (names)*/
   unsigned short *sizeIdUS; /*size of id arrays*/

   unsigned long lenVarUL;   /*number of variables*/
   unsigned long sizeVarUL;  /*maximum variables*/

   signed char **includeLibAryStr; /*included libraries*/
   unsigned int lenIncludeUI;/*number included librareis*/
   unsigned int sizeIncludeUI;    /*size of str array*/
   unsigned int *sizeIncludeAryUI; /*variable sizes*/

   signed char **excludedLibAryStr; /*excludedd libraries*/
   unsigned int lenExcludeUI;/*number excludedd librareis*/
   unsigned int sizeExcludeUI;    /*size of str array*/
   unsigned int *sizeExcludeAryUI; /*variable sizes*/

   signed char **excludeLibAryStr; /*excluded libraries*/
}varInput_stBuild;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof03:
'   - cmd_stBuild structure and dedicated functions
'   o st03: cmd_stBuild
'     - holds a user defined build command
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| ST03: cmd_stBuild
|   - holds a user defined build command
\-------------------------------------------------------*/
typedef struct cmd_stBuild
{
   signed char *idStr;       /*command id*/
   unsigned short sizeIdUS;  /*size of id array*/

   signed char *fileIdStr;   /*id of file command is in*/
   unsigned short sizeFileUS;/*size of file array*/

   /*for checking date stamps to see if to date*/
   signed char **checkAryStr;/*files to check*/
   unsigned int lenCheckUI;  /*number of files to check*/
   unsigned int sizeCheckUI; /*maximum files in array*/
   
   struct var_stBuild varsST; /*variables for command*/
}cmd_stBuild;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' Tof04:
'   - config_stBuild structure and dedicated functions
'   o .h st04: config_stBuild
'     - holds user defined configure commands
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| ST04: config_stBuild
|   - holds user defined configure commands
\-------------------------------------------------------*/
typedef struct config_stBuild
{
   signed char *idStr;       /*command id*/
   unsigned short sizeIdUS;  /*size of id array*/

   /*had files to check for*/
   struct var_stBuild libsST; /*variables for command*/
      /*bitAry holds if required or not*/
      /*valAry holds compiler flags for library/include*/
      /*idAry holds file name and paths*/
}config_stBuild;

/*-------------------------------------------------------\
| ST05: file_stBuild
|   - holds a files variables
\-------------------------------------------------------*/
typedef struct file_stBuild
{
   signed char *fileStr;      /*file name building for*/
   unsigned short sizeFileUS; /*size of file buffer*/

   signed char *idStr;        /*id of file*/
   unsigned short sizeIdUS;   /*size of id buffer*/

   /*variable arrays*/
   struct var_stBuild varST;    /*variables for command*/
   struct config_ST configST; /*configure checks*/
   struct cmd_stBuild cmdST;    /*commands to build*/

   struct file_stBuild *fileAryST;
      /*unmerged file commands*/
   ulong numFilesUL;     /*number files with input*/
   ulong sizeFilesUL;    /*maximum files in array*/
}file_stBuild;
