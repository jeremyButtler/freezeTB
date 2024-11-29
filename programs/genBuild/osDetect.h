/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' osDetect SOF: Start Of File
'   - detects OS and other system information
'   o header:
'     - included libraries and OS types
'   o fun01: getSysBit_osDetect
'     - finds if 64/32/16/8 bit OS
'   o fun02: getOS_osDetect
'     - checks for compiled OS using OS macros (non-user
'       supplied)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef OS_INORMATION_DETECTION_H
#define OS_INORMATION_DETECTION_H

#define def_maxOSIdLen_osDetect 13 /*longest OS name*/
#define def_osDepth_osDetect 5     /*max OSs per variable*/
   /* Levels
   `   0: for user supplied OS
   `   1: for most specific OS (variant/flavor)
   `   2: for OS sub group (ex: linux/bsd/mac/cygwin)
   `   3: for OS group (ex: unix)
   `   4: for default/no idea setting (all OS's get)
   */


/*cpu bit size*/
/*0 is for blank*/
#define def_8bit_osDetect 1
#define def_16bit_osDetect 2
#define def_32bit_osDetect 3
#define def_64bit_osDetect 4

/*SIMD flags*/
/*0 is for blank*/
#define def_neon_osDetect 1
#define def_sse_osDetect 2
#define def_sse2_osDetect 4
#define def_sse4_osDetect 8
#define def_avx2_osDetect 16
#define def_avx512_osDetect 32

/*-------------------------------------------------------\
| Fun01: getSysBit_osDetect
|   - finds if 64/32/16/8 bit OS
| Input:
|   - osAryStr:
|     o pointer to globSet_osDetect struct to hold bit type
| Output:
|   - Returns:
|     o bit size of detected OS
|     o 0 if unkown bit size
\-------------------------------------------------------*/
void
getSysBit_osDetect(
);

/*-------------------------------------------------------\
| Fun02: getOS_osDetect
|   - checks for compiled OS using OS macros (non-user
|     supplied)
| Input:
|   - osAryStr:
|     o pointer to c-string arrary to hold OS types
|       - make sure c-string array has at least
|         def_osDepth_osDetect elements and that each
|         element is at least def_OSLen_osDetect + 1 bytes
|         long
| Output:
|    - Modifes:
|      o each index in osAryStr to have the os
|        - 0 not modified (user input only)
|        - 1 set to most specific OS (ex openbsd) or '\0'
|        - 2 set to os sub group (ex bsd) OS or '\0'
|        - 3 set to OS group (ex unix) or '\0'
|        - 4 is always set to all (all OSs)
\-------------------------------------------------------*/
void
getOS_osDetect(
   signed char *osAryStr[]
);

#endif
