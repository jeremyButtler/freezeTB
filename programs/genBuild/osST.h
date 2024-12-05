/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' osST SOF: Start Of File
'   - detects OS and other system information
'   o header:
'     - defined variables and guards
'   o .h st01: specs_osST
'     - holds the system specs
'   o .c fun01: cpStr_osST:
'     - copy a string to a pointer
'   o fun02: getCpuBit_osST
'     - finds if 64/32/16/8 bit OS
'   o fun03: getOS_osST
'     - checks for compiled OS using OS macros (non-user
'       supplied)
'   o fun04: getCpu_osST
'     - gets system cpu information and for ARM adds in
'       simd (based on cpu version)
'   o fun05: getSIMD_osST
'     - gets intel simd type using compiler macros and
'       sets default SMID to scalar
'   o fun06: blank_specs_osST
'     - blanks all values in an specs_osST struct
'   o fun07: init_specs_osST
'     - initializes all values in a specs_osST struct to 0
'   o fun08: freeStack_specs_osST
'     - frees variables in a specs_osST struct
'   o fun09: freeHeap_specs_osST
'     - frees a specs_osST struct
'   o fun10: setup_specs_osST
'     - setups a specs_osST struct; memory allocate +specs
'   o fun11: mk_specs_osST
'     - makes and returns heap allocated specs_osST struct
'   o fun12: addOS_specs_osST
'     - adds an os to the os list in a specs_osST struct
'   o fun13: addCpu_specs_osST
'     - adds a cpu to the cpu list in a specs_osST struct
'       and for ARM cpus adds if has simd support (by 
'       cpu version)
'   o fun14: addSIMD_specs_osST
'     - adds a SIMD to simd list in a specs_osST struct
'   o fun15: addBit_specs_osST
'     - add cpu bit size to osBitUS in specs_osST struct
'   o fun16: addFlag_specs_osST
'     - uses flags to add a value into a specs_osST struct
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef OS_INORMATION_DETECTION_H
#define OS_INORMATION_DETECTION_H

#define def_memErr_osST 1
#define def_levelErr_osST 2
#define def_badFlag_osST 4
#define def_badBit_osST 8

#define def_osDepth_osST 4     /*max OSs per variable*/
   /* Levels
   `   0: for most specific OS (variant/flavor)
   `   1: for OS sub group (ex: linux/bsd/mac/cygwin)
   `   2: for OS group (ex: unix)
   `   3: for default/no idea setting (all OS's get)
   */

#define def_varOS_osST 0   /*most specific os level*/
#define def_subOS_osST 1   /*os subgroup (ex: linux/bsd)*/
#define def_groupOS_osST 2 /*os group (ex: unix/windows)*/
#define def_allOS_osST 3   /*any OS*/

/*SIMD flags*/
#define def_maxSIMD_osST 7
   /*0 for scalar; 1 for type; 2 for ARM architecture*/

/*SIMD types
`  Do not depend on level being in a particular order,
`  except that scalar is always last
`     scalar (last, no simd)
`     sse    (5th)
`     sse2   (4th)
`     sse3   (3rd)
`     sse4   (2nd)
`     avx2   (1st)
`     avx512 (0th)
`     ARM6   (0th)
`     neon (1st):
`       aarch32 (0th)
`       aarch64 (0th)
*/

#define def_0thSIMD_osST 0  /*0th SIMD choice*/
#define def_1stSIMD_osST 1  /*1st SIMD choice*/
#define def_2ndSIMD_osST 2  /*2nd SIMD choice*/
#define def_3rdSIMD_osST 3  /*3rd SIMD choice*/
#define def_4thSIMD_osST 4  /*4th SIMD choice*/
#define def_5thSIMD_osST 5  /*5th SIMD choice*/
#define def_lastSIMD_osST 6 /*scalar (no SIMD)*/

/*CPU flags*/
#define def_maxCPU_osST 3
   /*0 for all; 1 system cpu; 2 for ARM verison*/

#define def_cpuVer_osST 0 /*cpu version*/
#define def_cpuType_osST 1 /*cpu type*/
#define def_lastCPU_osST 2 /*any cpu*/

/*-------------------------------------------------------\
| ST01: specs_osST
|   - holds the system specs
\-------------------------------------------------------*/
typedef struct specs_osST
{
   signed char **osAryStr;
   unsigned char osIndexAryUC[def_osDepth_osST];
   unsigned char sizeOSAryUC[def_osDepth_osST];

   /*at most I expect two simd types*/
   signed char **simdAryStr;
   unsigned char simdIndexAryUC[def_maxSIMD_osST];
   unsigned char sizeSIMDAryUC[def_maxSIMD_osST];

   signed char **cpuAryStr;      /*cpu type*/
   unsigned char cpuIndexAryUC[def_maxCPU_osST];
   unsigned char sizeCPUAryUC[def_maxCPU_osST];

   unsigned short osBitUS;  /*only number allowed here*/
}specs_osST;

/*-------------------------------------------------------\
| Fun02: getCpuBit_osST
|   - finds if 64/32/16/8 bit OS
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to cpu bit size
| Output:
\-------------------------------------------------------*/
void
getCpuBit_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun03: getOS_osST
|   - checks for compiled OS using compiler macros
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to add OS info
| Output:
|   - Modifes:
|     o each index in glob_osAryStr to have the os
|       - 0 not modified (user input only)
|       - 1 set to most specific OS (ex openbsd) or '\0'
|       - 2 set to os sub group (ex bsd) OS or '\0'
|       - 3 set to OS group (ex unix) or '\0'
|       - 4 is always set to all (all OSs)
|   - Returns:
|     o 0 if no errors
|     o def_memErr_osST for memory errors
\-------------------------------------------------------*/
signed char
getOS_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun04: getCpu_osST
|   - gets system cpu information and for ARM adds in
|     simd (based on cpu version)
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to add cpu
|       information to and if ARM is using SIMD
| Output:
|   - Modifies:
|     o cpuAryStr in osSTPtr to have the all CPU's flag
|       as first index, then the CPU type, and for ARM
|       the CPU version as (AMR#)
|     o sizeCPUAryUC to have resized values (if done)
|     o simdAryStr to have ARM SIMD types (index 1 and 2)
|     o sizeSIMDAryUC to have resized values (if done)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_osST for memory errors
| Source:
|    - modified version of Kotori0 answer to
|      https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
\-------------------------------------------------------*/
signed char
getCpu_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun05: getSIMD_osST
|   - gets intel simd type using compiler macros and sets
|     default SMID to scalar
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to add SIMD info
| Output:
|   - Modifies:
|     o simdAryStr to have intel SIMD (0-4) and scalar (5)
|     o sizeSIMDAryUC to have resized values (if done)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_osST for memory errors
| Source:
|   - gcc SIMD macros from
|     https://stackoverflow.com/questions/28939652/how-to-detect-sse-sse2-avx-avx2-avx-512-avx-128-fma-kcvi-availability-at-compile
|   - microsoft SIMD macros from
|     https://stackoverflow.com/questions/18563978/detect-the-availability-of-sse-sse2-instruction-set-in-visual-studio
| Note:
|   - did not want to deal with cpuid since not all
|      compilers support inline (microsoft in some cases)
|      assembly or may may not define (no idea, but
|      assumed)
\-------------------------------------------------------*/
signed char
getSIMD_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun06: blank_specs_osST
|   - blanks all values in an specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to blank
| Output:
|   - Modifies:
|     o all variables in osSTPtr to be blank
\-------------------------------------------------------*/
void
blank_specs_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun07: init_specs_osST
|   - initializes all values in a specs_osST struct to 0
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to initialize
| Output:
|   - Modifies:
|     o all variables and pointers in osSTPtr to 0
\-------------------------------------------------------*/
void
init_specs_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun08: freeStack_specs_osST
|   - frees variables in a specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to free varialbes in
| Output:
|   - Modifies:
|     o frees all variables in osSTPtr and sets to 0
\-------------------------------------------------------*/
void
freeStack_specs_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun09: freeHeap_specs_osST
|   - frees a specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to free
| Output:
|   - Modifies:
|     o frees a osSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_specs_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun10: setup_specs_osST
|   - setups a specs_osST struct (memory allocate + specs)
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to setup memory for
|       and get system specs from compiler macros
| Output:
|   - Modifies:
|     o allocates memory for all pointers in osSTPtr
|     o adds system default values to variables in osSTPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_osST for memory error
\-------------------------------------------------------*/
signed char
setup_specs_osST(
   struct specs_osST *osSTPtr
);

/*-------------------------------------------------------\
| Fun11: mk_specs_osST
|   - makes and returns heap allocated specs_osST struct
| Input:
| Output:
|   - Returns:
|     o pointer to heap allocated, setup specs_osST struct
|     o 0 for memory errorsj
\-------------------------------------------------------*/
struct specs_osST *
mk_specs_osST(
);
/*-------------------------------------------------------\
| Fun12: addOS_specs_osST
|   - adds an os to the os list in a specs_osST struct
| Input:
|   - osSTPtr:
|     o  pointer to a specs_osST struct to add os to
|   - osStr:
|     o c-string with os to add
|   - levSC:
|     o level to add OS to (-1 for lowest possible)
|       - def_varOS_osST for variant level
|       - def_subOS_osST for subgroup os level
|       - def_groupOS_osST for os group
|       - def_allOS_osSt for def_groupOS_osST
|       - -1 for system places (def_groupOS_osST if
|         not recoginzed)
| Output:
|   - Returns:
|     o 0 for  no errors
|     o def_memErr_osST for memory errors
|     o def_levelErr_osST for unkown os level
\-------------------------------------------------------*/
signed char
addOS_specs_osST(
   struct specs_osST *osSTPtr,
   signed char *osStr,
   signed char levSC
);

/*-------------------------------------------------------\
| Fun13: addCpu_specs_osST
|   - adds a cpu to the cpu list in a specs_osST struct
|     and for ARM cpus adds if has simd support (by cpu 
|     version)
| Input:
|   - osSTPtr:
|     o  pointer to a specs_osST struct to add cpu to
|   - cpuStr:
|     o c-string with cpu to add
|   - levSC:
|     o level to add OS to (-1 for lowest possible)
|       - def_cpuVer_osST for cpu verions (ex arm8)
|       - def_cpuType_osST for cpu type
|       - def_lastCPU_osSt for def_cpuType_osST
|       - -1 for system places
| Output:
|   - Returns:
|     o 0 for  no errors
|     o def_memErr_osST for memory errors
|     o def_levelErr_osST for unkown os level
\-------------------------------------------------------*/
signed char
addCpu_specs_osST(
   struct specs_osST *osSTPtr,
   signed char *cpuStr,
   signed char levSC
);

/*-------------------------------------------------------\
| Fun14: addSIMD_specs_osST
|   - adds a SIMD to the simd list in a specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to a specs_osST struct to add simd to
|   - simdStr:
|     o c-string with simd to add
|   - levSC:
|     o level t- add OS to (-1 for lowest possible)
|       - def_0thSIMD_osST for first choice simd
|       - def_1stSIMD_osST for second choice simd
|       - def_2ndSIMD_osST for third choice simd
|       - def_3rdSIMD_osST for fourth choice simd
|       - def_4thSIMD_osST for 5th choice simd
|       - def_5thSIMD_osST for 6th choice simd
|       - def_lastSIMD_osST for def_5thSIMD_osST
|       - -1 for system places
| Output:
|   - Returns:
|     o 0 for  no errors
|     o def_memErr_osST for memory errors
|     o def_levelErr_osST for unkown os level
\-------------------------------------------------------*/
signed char
addSIMD_specs_osST(
   struct specs_osST *osSTPtr,
   signed char *simdStr,
   signed char levSC
);

/*-------------------------------------------------------\
| Fun15: addBit_specs_osST
|   - add cpu bit size to osBitUS in specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to a specs_osST struct to have bitUS
|   - bitUS:
|     o bit size of cpu to add
| Output:
\-------------------------------------------------------*/
void
addBit_specs_osST(
   struct specs_osST *osSTPtr,
   unsigned short bitUS
);

/*-------------------------------------------------------\
| Fun16: addFlag_specs_osST
|   - uses flags to add a value into a specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to a specs_osST struct to add value to
|   - flagStr:
|     o flag to add in
|   - valStr:
|     o c-string with value (can be 0 if in flag)
|   - argSIPtr:
|     o pointer to signed int to be incurmented for number
|       of arguments used (1 or 2)
| Output:
|   - Modifies:
|     o arrays in osSTPtr to have coorect values
|   - Returns:
|     o 0 for no errors
|     o def_memErr_osST for memory error
|     o def_badFlag_osST if invalid flag input
|     o def_badbit_osST if value for bit flag is
|       non-numierc or to large
\-------------------------------------------------------*/
signed char
addFlag_specs_osST(
   struct specs_osST *osSTPtr,
   signed char *flagStr,
   signed char *valStr,
   signed int *argSIPtr
);

/*-------------------------------------------------------\
| Fun17: phelp_specs_osST
|   - prints input part of help message for osST
| Input:
|   - outFILE:
|     o FILE to pointer to print input help message to
|   - indentStr:
|     o indent to add to input block
| Output:
|   - Prints:
|     o prints input part of help message to stdout
\-------------------------------------------------------*/
signed char
phelp_specs_osST(
   void *outFILE,
   signed char *indentStr
);

#endif
