/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' osST SOF: Start Of File
'   - detects OS and other system information
'   o header:
'     - included libraries and OS types
'   o .h st01: specs_osST
'     - holds the system specs
'   o .c fun01: cpStr_osST:
'     - copy a string to a pointer
'   o fun02: getCpuBit_specs_osST
'     - finds if 64/32/16/8 bit OS
'   o fun03: getOS_specs_osST
'     - checks for compiled OS using OS macros (non-user
'       supplied)
'   o fun04: getCpu_specs_osST
'     - gets system cpu information and for ARM adds in
'       simd (based on cpu version)
'   o fun05: getSIMD_specs_osST
'     - gets intel simd type using compiler macros and
'       sets default SMID to scalar
'   o fun06: specs_osST
'     - gets if cpu is big or little endin
'   o fun07: blank_specs_osST
'     - blanks all values in an specs_osST struct
'   o fun08: init_specs_osST
'     - initializes all values in a specs_osST struct to 0
'   o fun09: freeStack_specs_osST
'     - frees variables in a specs_osST struct
'   o fun10: freeHeap_specs_osST
'     - frees a specs_osST struct
'   o fun11: setup_specs_osST
'     - setups a specs_osST struct; memory allocate +specs
'   o fun12: mk_specs_osST
'     - makes and returns heap allocated specs_osST struct
'   o fun13: addOS_specs_osST
'     - adds an os to the os list in a specs_osST struct
'   o fun14: addCpu_specs_osST
'     - adds a cpu to the cpu list in a specs_osST struct
'       and for ARM cpus adds if has simd support (by 
'       cpu version)
'   o fun15: addSIMD_specs_osST
'     - adds a SIMD to simd list in a specs_osST struct
'   o fun16: addBit_specs_osST
'     - add cpu bit size to osBitUS in specs_osST struct
'   o fun17: addEndin_specs_osST
'     - add if big end in or small end in
'   o fun18: addFlag_specs_osST
'     - uses flags to add a value into a specs_osST struct
'   o fun19: phelp_specs_osST
'     - prints input part of help message for osST
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and OS types
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - list of known OSs
|   o header sec03:
|     - cpu list
|   o header sec04:
|     - simd list
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "osST.h"

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - list of known OSs
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

signed char
   *glob_osAryStr[] =
   {
      (signed char *) "all",
      (signed char *) "unix",

      (signed char *) "linux",
      (signed char *) "andriod",

      (signed char *) "mac",
      (signed char *) "bsd",

      (signed char *) "openBSD",
      (signed char *) "freeBSD",
      (signed char *) "netBSD",
      (signed char *) "dragonflyeBSD",

      (signed char *) "amiga",
      (signed char *) "beOS",  /*should include haiku*/
      (signed char *) "sun",   /*illuminos/oracle*/
      (signed char *) "morph",
      (signed char *) "syllable",
      (signed char *) "hurd",  /*gnu hurd*/
      (signed char *) "xinuos",

      (signed char *) "plan9",

      (signed char *) "win",   /*windows*/
      (signed char *) "cygwin" /*windows cygwin*/
   }; /*os name list*/

#define def_allIndex_osST 0
#define def_unixIndex_osST 1

#define def_linuxIndex_osST 2
#define def_androidIndex_osST 4

#define def_macIndex_osST 5

#define def_bsdIndex_osST 5
#define def_openBsdIndex_osST 6
#define def_freeBsdIndex_osST 7
#define def_netBsdIndex_osST 8
#define def_dragonflyeBsdIndex_osST 9

#define def_amigaIndex_osST 10
#define def_beOSIndex_osST 11
#define def_sunIndex_osST 12
#define def_morphIndex_osST 13
#define def_syllableIndex_osST 14
#define def_hurdIndex_osST 15
#define def_xinuosIndex_osST 16

#define def_plan9Index_osST 17

#define def_winIndex_osST 18
#define def_cygwinIndex_osST 19

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec03:
^   - cpu list
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

signed char
   *glob_cpuAryStr[] =
   {
      (signed char *) "all",
      (signed char *) "x86",

      /*ARM and its sub variants*/
      (signed char *) "ARM",
      (signed char *) "ARM2",
      (signed char *) "ARM3",
      (signed char *) "ARM4",
      (signed char *) "ARM5",
      (signed char *) "ARM6",
      (signed char *) "ARM7",
      (signed char *) "ARM8",

      (signed char *) "powerpc",
      (signed char *) "superh",
      (signed char *) "sparc",
      (signed char *) "mips",
      (signed char *) "spim", /*simulated mips*/
      (signed char *) "riscv",
      (signed char *) "m68k",
      (signed char *) "68000",
      (signed char *) "68020"
   }; /*glob_cpuAryStr (cpu names)*/

#define def_allCPU_osST 0
#define def_x86CPU_osST 1

#define def_armCPU_osST 2
#define def_arm2CPU_osST 3
#define def_arm3CPU_osST 4
#define def_arm4CPU_osST 5
#define def_arm5CPU_osST 6
#define def_arm6CPU_osST 7
#define def_arm7CPU_osST 8
#define def_arm8CPU_osST 9

#define def_powerpcCPU_osST 10
#define def_superhCPU_osST 11
#define def_sparcCPU_osST 12
#define def_mipsCPU_osST 13
#define def_spimCPU_osST 14
#define def_riscvCPU_osST 15
#define def_m68kCPU_osST 16
#define def_68000CPU_osST 17
#define def_68020CPU_osST 18

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec04:
^   - simd list
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

signed char
   *glob_simdAryStr[] =
   {
      (signed char *) "scalar",
      (signed char *) "sse",
      (signed char *) "sse2",
      (signed char *) "sse3",
      (signed char *) "sse4",
      (signed char *) "avx2",
      (signed char *) "avx512",

      /*ARM variants*/
      (signed char *) "ARM6",   /*pre-neon arm6 cpu simd*/
      (signed char *) "neon",
      (signed char *) "neon32", /*aarch32*/
      (signed char *) "neon64", /*aarch64*/
   }; /*glob_simdAryStr (cpu names)*/

#define def_scalarSIMD_osST 0
#define def_sseSIMD_osST 1
#define def_sse2SIMD_osST 2
#define def_sse3SIMD_osST 3
#define def_sse4SIMD_osST 4
#define def_avx2SIMD_osST 5
#define def_avx512SIMD_osST 6
#define def_arm6SIMD_osST 7
#define def_neonSIMD_osST 8
#define def_neon32SIMD_osST 9
#define def_neon64SIMD_osST 10

/*-------------------------------------------------------\
| Fun01: cpStr_osST:
|   - copy a string to a pointer
| Input:
|   - dupStrPtr:
|     o pointer to c-string to have new string (addStr)
|   - sizeUCPtr:
|     o pointer to unsiged char with size of dupStrPtr
|   - addStr:
|     o c-string to add to dupStrPtr
|   - lenAddUC:
|     o length of addStr
| Output:
|   - Modifies:
|     o dupStrPtr to have addStr and be resized if needed
|     o sizeUCPtr (if dupStrPtr is resized) to have new
|       size of dupStrPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_osST for memory errors
\-------------------------------------------------------*/
signed char
cpStr_osST(
   signed char **dupStrPtr,
   unsigned char *sizeUCPtr,
   signed char *addStr
){
   unsigned char posUC = 0;
   unsigned char lenUC = 0;

   while(addStr[lenUC] != '\0')
      ++lenUC;

   if(*sizeUCPtr < lenUC)
   { /*If: need to free memory*/
      if(*dupStrPtr)
         free(*dupStrPtr);
      *dupStrPtr = 0;
   } /*If: need to free memory*/

   if(! *dupStrPtr)
   { /*If: need to allocate*/
      *dupStrPtr =
          malloc((lenUC + 1) * sizeof(signed char));

      if(! *dupStrPtr)
         return def_memErr_osST;
 
      *sizeUCPtr = lenUC;
   } /*If: need to allocate*/

   for(
      posUC = 0;
      posUC < lenUC;
      ++posUC
   ) (*dupStrPtr)[posUC] = addStr[posUC];

   (*dupStrPtr)[posUC] = '\0';
   return 0;
} /*addStr_osST*/

/*-------------------------------------------------------\
| Fun02: getCpuBit_specs_osST
|   - finds if 64/32/16/8 bit OS
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to cpu bit size
| Output:
\-------------------------------------------------------*/
void
getCpuBit_specs_osST(
   struct specs_osST *osSTPtr
){

   #ifdef PLAN9
      osSTPtr->osBitUS = sizeof(unsigned long long) << 3;
      /*plan9 defines unsigned long as 32 bits and
      `   long long as 64 bits
      */
   #else
      osSTPtr->osBitUS = sizeof(unsigned long) << 3;
   #endif
   /*unsigned longs should most of the time be limited to
   `   the maximum value in an OS
   */
} /*getCpuBit_specs_osST*/

/*-------------------------------------------------------\
| Fun03: getOS_specs_osST
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
getOS_specs_osST(
   struct specs_osST *osSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - checks for compiled OS using OS macros (non-user
   '     supplied)
   '   o fun03 sec02:
   '     - check for linux OS
   '   o fun03 sec03:
   '     - check for Mac OS
   '   o fun03 sec04:
   '     - check for BSDs
   '   o fun03 sec05:
   '     - check for off wall unix or unix like or odd os
   '   o fun03 sec06:
   '     - check for UNIX (non Mac/Linux/BSD)
   '       - does not always work (not posix)
   '   o fun03 sec07:
   '     - check for plan9
   '   o fun03 sec08:
   '     - check for windows
   '   o fun03 sec09:
   '     - else unkown OS
   '   o fun03 sec10:
   '     - add all os label in and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check for linux OS
   ^   o fun03 sec02 sub01:
   ^     - android linux check
   ^   o fun03 sec02 sub02:
   ^     - linux check (multiple macros)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - android linux check
   \*****************************************************/

   #ifdef __ANDROID__
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_androidIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec02 Sub02:
   *   - linux check (multiple macros)
   *   o fun03 sec02 sub02 cat01:
   *     - linux macro 1
   *   o fun03 sec02 sub02 cat02:
   *     - linux macro 2
   *   o fun03 sec02 sub02 cat03:
   *     - linux macro 3
   *   o fun03 sec02 sub02 cat04:
   *     - gnu linux macro
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec02 Sub02 Cat01:
   +   - linux macro 1
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __linux
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec02 Sub02 Cat02:
   +   - linux macro 2
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef linux__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec02 Sub02 Cat03:
   +   - linux macro 3
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef linux
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec02 Sub02 Cat04:
   +   - gnu linux macro
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   
   #ifdef __gnu_linux__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - check for Mac OS
   ^   o fun03 sec03 sub01:
   ^     - macintosh macro
   ^   o fun03 sec03 sub02:
   ^     - apple and mach macros
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - macintosh macro
   \*****************************************************/

   #ifdef __Macintosh__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_macIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __macintosh__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_macIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec03 Sub02:
   *   - apple and mach macros
   \*****************************************************/

   #ifdef __APPLE__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_macIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __MACH__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_macIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - check for BSDs
   ^   o fun03 sec04 sub01:
   ^     - open bsd
   ^   o fun03 sec04 sub02:
   ^     - freeBsd
   ^   o fun03 sec04 sub03:
   ^     - netBsd
   ^   o fun03 sec04 sub04:
   ^     - dragonfly bsd
   ^   o fun03 sec04 sub05:
   ^     - general bsd
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - open bsd
   \*****************************************************/

   #ifdef __OpenBSD__
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_openBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec04 Sub02:
   *   - freeBsd
   \*****************************************************/
   
   #ifdef __FreeBSD__
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_freeBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec04 Sub03:
   *   - netBsd
   \*****************************************************/
   
   #ifdef __NetBSD__
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_netBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec04 Sub04:
   *   - dragonfly bsd
   \*****************************************************/

   #ifdef __DragonFlye__
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_dragonflyeBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec04 Sub05:
   *   - bsd
   \*****************************************************/

   #ifdef __bsdi__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - check for off wall unix or unix like or odd os
   ^   o fun03 sec05 sub01:
   ^     - amiga os
   ^   o fun03 sec05 sub02:
   ^     - BeOS (haiku)
   ^   o fun03 sec05 sub03:
   ^     - gnu hurd (maybe one of these days)
   ^   o fun03 sec05 sub04:
   ^     - morph os
   ^   o fun03 sec05 sub05:
   ^     - sun os (illuminos)
   ^   o fun03 sec05 sub06:
   ^     - syllable (author of syllable plans to return)
   ^   o fun03 sec05 sub07:
   ^     - xinuos (sever OS)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I have no plan to code specific stuff for these OS's
   `  this is mainly here for future users
   */

   /*****************************************************\
   * Fun03 Sec05 Sub01:
   *   - amiga os
   \*****************************************************/

   #ifdef AMIGA
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_amigaIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __amigaos__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_amigaIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub02:
   *   - BeOS (haiku)
   \*****************************************************/

   #ifdef __BEOS__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_beOSIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub03:
   *   - gnu hurd (maybe one of these days)
   \*****************************************************/

   #ifdef __GNU__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_hurdIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __gnu_hurd__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_hurdIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub04:
   *   - morph os
   \*****************************************************/

   #ifdef __MORPHOS__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_morphIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub05:
   *   - sun os (illuminos)
   \*****************************************************/

   #ifdef sun
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_sunIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __sun
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_sunIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub06:
   *   - syllable (author of syllable plans to return)
   \*****************************************************/

   #ifdef __SYLLABLE__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_syllableIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec05 Sub07:
   *   - xinuos (sever OS)
   \*****************************************************/

   #ifdef sco
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_xinuosIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef _UNIXWARE7
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_xinuosIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec06:
   ^   - check for UNIX (non Mac/Linux/BSD)
   ^   - does not always work (not posix)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef __unix__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __unix
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec07:
   ^   - check for plan9
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef EPLAN9
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_plan9Index_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef PLAN9 /*my personal flag for plan9*/
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';
 
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_plan9Index_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif



   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec08:
   ^   - check for windows
   ^   o fun03 sec08 sub01:
   ^     - check for cygwin shell in windows
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec08 Sub01:
   *   - check for cygwin shell in windows
   \*****************************************************/

   #ifdef __CYGWIN__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      /*not putting windows flag in because cygwin is
      `  a linux enviorment, so windows flags and
      `  commands will likley not work
      */

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_cygwinIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*****************************************************\
   * Fun03 Sec08 Sub02:
   *   - check for windows
   *   o fun03 sec08 sub02 cat01:
   *     - 16 bit windows
   *   o fun03 sec08 sub02 cat02:
   *     - 32 bit windows
   *   o fun03 sec08 sub02 cat03:
   *     - 64 bit windows
   *   o fun03 sec08 sub02 cat04:
   *     - windows, no idea what flag means
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec08 Sub02 Cat01:
   +   - 16 bit windows
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef _WIN16
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec08 Sub02 Cat02:
   +   - 32 bit windows
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef _WIN32
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __WIN32__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec08 Sub02 Cat03:
   +   - 64 bit windows
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef _WIN64
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec08 Sub02 Cat04:
   +   - windows, no idea what flag means
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __TOS_WIN__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   #ifdef __WINDOWS__
      if(osSTPtr->osAryStr[def_varOS_osST])
         osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_winIndex_osST]
         )
      ) return def_memErr_osST;

      goto allOS_fun03_sec10;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec09:
   ^   - else unkown OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(osSTPtr->osAryStr[def_varOS_osST])
      osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

   if(osSTPtr->osAryStr[def_subOS_osST])
      osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

   if(osSTPtr->osAryStr[def_groupOS_osST])
      osSTPtr->osAryStr[def_groupOS_osST][0] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec10:
   ^   - add all os label in and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   allOS_fun03_sec10:;
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_allOS_osST],
            &osSTPtr->sizeOSAryUC[def_allOS_osST],
            glob_osAryStr[def_allIndex_osST]
         )
      ) return def_memErr_osST;

      return 0;
} /*getOS_specs_osST*/

/*-------------------------------------------------------\
| Fun04: getCpu_specs_osST
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
getCpu_specs_osST(
   struct specs_osST *osSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   o fun04 sec01:
   '     - plan9 cpu detectoin
   '   o fun04 sec02:
   '     - x86 cpus
   '   o fun04 sec03:
   '     - ARM cpus (neon detection by cpu version)
   '   o fun04 sec04:
   '     - powerpc
   '   o fun04 sec05:
   '     - superh cpu
   '   o fun04 sec06:
   '     - sparc cpu
   '   o fun04 sec07:
   '     - m68k cpu
   '   o fun04 sec08:
   '     - mips cpu
   '   o fun04 sec09:
   '     - any cpu (always assigned)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - plan9 cpu detection
   ^   o fun04 sec01 sub01:
   ^     - get cpu by enviromental variable
   ^   o fun04 sec01 sub02:
   ^     - 32 bit x86 cpu detection
   ^   o fun04 sec01 sub03:
   ^     - 68000 and 68020 cpu detection (moterola)
   ^   o fun04 sec01 sub04:
   ^     - mips cpu detection
   ^   o fun04 sec01 sub05:
   ^     - power cpu detection
   ^   o fun04 sec01 sub06:
   ^     - sparc and spim cpu detection
   ^   o fun04 sec01 sub07:
   ^     - x86 (64 bit [amd64]) and arm cpu detection
   ^   o fun04 sec01 sub08:
   ^     - goto all cpu flag
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec01 Sub01:
   *   - get cpu by enviromental variable
   \*****************************************************/

   #ifdef PLAN9
      signed char *tmpStr = 0;

      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      tmpStr = (signed char *) getenv("objtype");

      /*cpus: 386,68000,68020,amd64,arm,arm64,mips,power
      `       power64,sparc,sparc64,spim (mips simulator)
      */

      /**************************************************\
      * Fun04 Sec01 Sub02:
      *   - 32 bit x86 cpu detection
      \**************************************************/

      switch(tmpStr[0])
      { /*Switch: find cpu type*/
         case '3':
         /*386 (x86 cpu, 32 bit)*/
            if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_x86CPU_osST]
               )
            ) return def_memErr_osST;

            osSTPtr->osBitUS = 32;
            break;
         /*386 (x86 cpu, 32 bit)*/

         /***********************************************\
         * Fun04 Sec01 Sub03:
         *   - 68000 and 68020 cpu detection (moterola)
         \***********************************************/

         case '6':
         /*68000 or 68020*/

            osSTPtr->osBitUS = 32; /*both are 32 bit*/

            if(
              cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_m68kCPU_osST]
              )
            ) return def_memErr_osST;

            if(tmpStr[3] == '0')
            { /*If: 68000 cpu*/
              if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuVer_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
                 glob_cpuAryStr[def_68000CPU_osST]
               )
              ) return def_memErr_osST;
            } /*If: 68000 moterola cpu*/

            else
            { /*Else: 68020 cpu*/
              if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_68020CPU_osST]
               )
              ) return def_memErr_osST;
            } /*Else: 68020 moterola cpu*/

            break;
         /*68000 or 60820 (32/16bit and 32 bit)*/

         /***********************************************\
         * Fun04 Sec01 Sub04:
         *   - mips cpu detection
         \***********************************************/

         case 'm':
         /*Case: mips*/
            osSTPtr->osBitUS = 32;

            if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuVer_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
                 glob_cpuAryStr[def_mipsCPU_osST]
               )
            ) return def_memErr_osST;

            break;
         /*Case: mips cpu*/

         /***********************************************\
         * Fun04 Sec01 Sub05:
         *   - power cpu detection
         \***********************************************/

         case 'p':
         /*Case: power cpu*/
            if(tmpStr[5] == '6')
               osSTPtr->osBitUS = 64; /*power64*/
            else
               osSTPtr->osBitUS = 32; /*power\0*/

            if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_powerpcCPU_osST]
               )
            ) return def_memErr_osST;

            break;
         /*Case: power cpu*/

         /***********************************************\
         * Fun04 Sec01 Sub06:
         *   - sparc and spim cpu detection
         \***********************************************/

         case 's':
         /*Case: sparc and spim cpus*/
            if(tmpStr[6] == '6')
               osSTPtr->osBitUS = 64; /*sparc64*/
            else
               osSTPtr->osBitUS = 32; /*sparc\0*/

            if(tmpStr[2] == 'a')
            { /*If: sparc CPU*/
               if(
                cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_sparcCPU_osST]
                )
               ) return def_memErr_osST;
            } /*If: sparc CPU*/

            else
            { /*Else: spim CPU*/
               if(
                cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_spimCPU_osST]
                )
               ) return def_memErr_osST;
            } /*Else: spim CPU*/

            break;
         /*Case:sparc and spim cpus*/

         /***********************************************\
         * Fun04 Sec01 Sub07:
         *   - x86 (64 bit [amd64]) and arm cpu detection
         *   o fun04 sec01 sub06 cat01:
         *     - x86 (64 bit [amd64])
         *   o fun04 sec01 sub06 cat02:
         *     - arm (32/64 bit) detection
         *   o fun04 sec01 sub06 cat03:
         *     - arm 64 bit (arm8) detection + neon simd
         *   o fun04 sec01 sub06 cat04:
         *     - blank non-neon simd arrays elements
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun04 Sec01 Sub06 Cat01:
         +   - x86 (64 bit [amd64])
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'a':
         /*Case: amd64/arm/arm64*/
            if(tmpStr[1] == 'm')
            { /*If: amd64 cpu*/
              osSTPtr->osBitUS = 64;

              if(
               cpStr_osST(
                  &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                  glob_cpuAryStr[def_x86CPU_osST]
               )
              ) return def_memErr_osST;

              break;
            } /*If: amd64 (x86 64 bit cpu)*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun04 Sec01 Sub06 Cat02:
            +   - arm (32/64 bit) detection
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(
              cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuType_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
                 glob_cpuAryStr[def_armCPU_osST]
               )
            ) return def_memErr_osST;

            if(tmpStr[3] != '6')
            { /*If: 32 bit arm cpu (not 64 bit)*/
               osSTPtr->osBitUS = 32;
               break;
            } /*If: 32 bit arm cpu (not 64 bit)*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun04 Sec01 Sub06 Cat03:
            +   - arm 64 bit (arm8) detection + neon simd
            \+++++++++++++++++++++++++++++++++++++++++++*/

            osSTPtr->osBitUS = 64;

            if(
               cpStr_osST(
                 &osSTPtr->cpuAryStr[def_cpuVer_osST],
                 &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
                 glob_cpuAryStr[def_arm8CPU_osST]
               )
            ) return def_memErr_osST;

            if(
               cpStr_osST(
                  &osSTPtr->simdAryStr[def_1stSIMD_osST],
                &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
                  glob_simdAryStr[def_neonSIMD_osST]
               )
            ) return def_memErr_osST;

            if(
               cpStr_osST(
                  &osSTPtr->simdAryStr[def_0thSIMD_osST],
                &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
                  glob_simdAryStr[def_neon64SIMD_osST]
               )
            ) return def_memErr_osST;

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun04 Sec01 Sub06 Cat04:
            +   - blank non-neon simd arrays elements
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
               osSTPtr->simdAryStr[def_2ndSIMD_osST][0] =
                  '\0';

            if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
               osSTPtr->simdAryStr[def_3rdSIMD_osST][0] =
                  '\0';

            if(osSTPtr->simdAryStr[def_4thSIMD_osST])
               osSTPtr->simdAryStr[def_4thSIMD_osST][0] =
                  '\0';

            if(osSTPtr->simdAryStr[def_5thSIMD_osST])
               osSTPtr->simdAryStr[def_5thSIMD_osST][0] =
                 '\0';

            break;
         /*Case: amd64/arm/arm64*/
      } /*Switch: find cpu type*/

      /**************************************************\
      * Fun04 Sec01 Sub08:
      *   - goto all cpu flag
      \**************************************************/

      goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - x86 cpus
   ^   o fun04 sec02:
   ^     - 64 bit x86 cpus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02:
   *   - 64 bit x86 cpus
   \*****************************************************/

   #ifdef __x86_64__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef _M_X64  /*x86 64 bit*/
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   /*****************************************************\
   * Fun04 Sec02:
   *   - 32 bit x86 cpus
   \*****************************************************/

   #ifdef i386
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __i386__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __i386 
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef _M_IX86
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_x86CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - ARM cpus (neon detection by cpu version)
   ^   o fun04 sec03 sub01:
   ^     - non SIMD AMR CPUs
   ^   o fun04 sec03 sub02:
   ^     - SIMD AMR CPUs
   ^   o fun04 sec03 sub03:
   ^     - general ARM capture (in case miss with other)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec03 Sub01:
   *   - non SIMD AMR CPUs
   *   o fun04 sec03 sub01 cat01:
   *     - ARM2
   *   o fun04 sec03 sub01 cat02:
   *     - ARM3
   *   o fun04 sec03 sub01 cat03:
   *     - ARM4
   *   o fun04 sec03 sub01 cat04:
   *     - ARM5
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat01:
   +   - ARM2
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __ARM_ARCH_2__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm2CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat02:
   +   - ARM3
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __ARM_ARCH_3__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm3CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_3M__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm3CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif
 
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat03:
   +   - ARM4
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __ARM_ARCH_4T__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm4CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef  __TARGET_ARM_4T
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm4CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat04:
   +   - ARM5
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __ARM_ARCH_5_
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm5CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_5__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm5CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_5E_
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm5CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_5E__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm5CPU_osST]
         )
      ) return def_memErr_osST;

      goto allCPUfun04_sec09;
   #endif

   /*****************************************************\
   * Fun04 Sec03 Sub02:
   *   - SIMD AMR CPUs
   *   o fun04 sec03 sub02 cat01:
   *     - ARM6 (as non-neon simd)
   *   o fun04 sec03 sub02 cat02:
   *     - ARM7 (start of neon for aarch 32)
   *   o fun04 sec03 sub02 cat03:
   *     - ARM8 (neon for aarch 64)
   \*****************************************************/
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub02 Cat01:
   +   - ARM6 (as non-neon simd)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*start simd (non-neon)*/
   #ifdef __ARM_ARCH_6T2_
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_6__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_6J__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_6K__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_6Z__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_6ZK__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm6CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_arm6SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat02:
   +   - ARM7 (start of neon for aarch 32)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*some differences in co-processors, AMR7 is missing
   `  the floating point co-processor of ARM7A. So,
   `  treating as same since all should suppor neon
   `  Order of additional features goes 7->A->R->M->S
   */

   #ifdef __ARM_ARCH_7__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm7CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_7A__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm7CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_me mErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_7R__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm7CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_7M__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm7CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_7S__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm7CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';


      goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub01 Cat03:
   +   - ARM8 (neon for aarch 64)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __aarch64__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm8CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon64SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif

   #ifdef _M_ARM64
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm8CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon64SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';


      goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_ISA_A64
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_arm8CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_neon64SIMD_osST]
         )
      ) return def_memErr_osST;

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      goto allCPUfun04_sec09;
   #endif


   /*****************************************************\
   * Fun04 Sec03 Sub03:
   *   - flags to catch most AMR processors
   *   o fun04 sec03 sub02 cat01:
   *     - microsoft flags (i think)
   *   o fun04 sec03 sub02 cat02:
   *     - general 32 bit flag; added; not all are real
   *   o fun04 sec03 sub02 cat03:
   *     - all ARMS flag (added some; not all may be real)
   *   o fun04 sec03 sub02 cat04:
   *     - all ARMS flag (by risc flag)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub03 Cat01:
   +   - microsoft flags (i think)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef _M_ARM
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

     /*microssofts 32bit arm detection*/

     goto allCPUfun04_sec09;
   #endif

   #ifdef _M_ARM_FP
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

     /*microssofts 32bit floating point arm detection*/

     goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub03 Cat02:
   +   - general 32 bit flag; added some; not all are real
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __arm
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __arm_
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __arm__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub03 Cat03:
   +   - all ARMS flag (I added some; not all may be real)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   #ifdef __ARM_ARCH
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH_
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __ARM_ARCH__
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_armCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun04 Sec03 Sub03 Cat04:
   +   - all ARMS flag (by risc flag)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - powerpc
   ^   o fun04 sec04 sub01:
   ^     - powerpc 32 bit flags (or general)
   ^   o fun04 sec04 sub02:
   ^     - powerpc 64 bit flags
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun04 Sec04 Sub01:
   *   - powerpc 32 bit flags (or general)
   \*****************************************************/

   #ifdef __powerpc
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __powerpc__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __POWERPC__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __ppc__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __PPC__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef _ARCH_PPC
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*****************************************************\
   * Fun04 Sec04 Sub02:
   *   - powerpc 64 bit flags
   \*****************************************************/

   #ifdef __powerpc64__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __PPC64__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __ppc64__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef _ARCH_PPC64
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_powerpcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - superh cpu
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   #ifdef __sh__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_superhCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec06:
   ^   - sparc cpu
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   #ifdef __sparc__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_sparcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __sparc
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_sparcCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec07:
   ^   - m68k cpu
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   #ifdef __m68k__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_m68kCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec08:
   ^   - MIPS cpu
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   #ifdef mips
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_mipsCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __mips__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_mipsCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   #ifdef __mips
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_mipsCPU_osST]
         )
      ) return def_memErr_osST;

       goto allCPUfun04_sec09;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec10:
   ^   - riscv CPUs
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


    #ifdef __riscv
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

       if(
          cpStr_osST(
             &osSTPtr->cpuAryStr[def_cpuType_osST],
             &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
             glob_cpuAryStr[def_riscvCPU_osST]
          )
       ) return def_memErr_osST;

       goto allCPUfun04_sec09;
    #endif

    #ifdef __riscv
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

       if(
          cpStr_osST(
             &osSTPtr->cpuAryStr[def_cpuType_osST],
             &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
             glob_cpuAryStr[def_riscvCPU_osST]
          )
       ) return def_memErr_osST;

       goto allCPUfun04_sec09;
    #endif

    #ifdef __riscv32
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

       if(
          cpStr_osST(
             &osSTPtr->cpuAryStr[def_cpuType_osST],
             &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
             glob_cpuAryStr[def_riscvCPU_osST]
          )
       ) return def_memErr_osST;

       goto allCPUfun04_sec09;
    #endif

    #ifdef __riscv__
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

       if(
          cpStr_osST(
             &osSTPtr->cpuAryStr[def_cpuType_osST],
             &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
             glob_cpuAryStr[def_riscvCPU_osST]
          )
       ) return def_memErr_osST;

       goto allCPUfun04_sec09;
    #endif

    #ifdef _riscv
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

       if(
          cpStr_osST(
             &osSTPtr->cpuAryStr[def_cpuType_osST],
             &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
             glob_cpuAryStr[def_riscvCPU_osST]
          )
       ) return def_memErr_osST;

       goto allCPUfun04_sec09;
    #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec09:
   ^   - any cpu (always assigned)
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   if(osSTPtr->cpuAryStr[def_cpuVer_osST])
      osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

   if(osSTPtr->cpuAryStr[def_cpuType_osST])
      osSTPtr->cpuAryStr[def_cpuType_osST][0] = '\0';


   allCPUfun04_sec09:;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_lastCPU_osST],
            &osSTPtr->sizeCPUAryUC[def_lastCPU_osST],
            glob_cpuAryStr[def_allCPU_osST]
         )
      ) return def_memErr_osST;

      return 0;
} /*getCpu_specs_osST*/

/*-------------------------------------------------------\
| Fun05: getSIMD_specs_osST
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
getSIMD_specs_osST(
   struct specs_osST *osSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - gets intel simd type using compiler macros and
   '     sets default SMID to scalar
   '   o fun05 sec01:
   '     - AVX512 simd macros
   '   o fun05 sec02:
   '     - AVX2 simd macro
   '   o fun05 sec03:
   '     - AVX simd macro
   '   o fun05 sec04:
   '     - SSE4.1 macro
   '   o fun05 sec05:
   '     - SSE3 macros
   '   o fun05 sec06:
   '     - SSE macros
   '   o fun05 sec07:
   '     - define scalar fallback (no SIMD)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*do not want to deal with cpuid since not all
     compilers support inline (microsoft in some cases)
     assembly or may may not define (no idea, but assume)
   */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - AVX512 simd macros
   ^   o fun05 sec01 sub01:
   ^     - avx512 popcntdq
   ^   o fun05 sec01 sub02:
   ^     - avx512 VBMI
   ^   o fun05 sec01 sub03:
   ^     - avx512 IFMA
   ^   o fun05 sec01 sub04:
   ^     - avx512 VNNIW
   ^   o fun05 sec01 sub05:
   ^     - avx512 FMAPS
   ^   o fun05 sec01 sub06:
   ^     - avx512 BW (my minimum)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /* not checking these because they were before BW
     __AVX512F__
     __AVX512CD__
     __AVX512ER__
     __AVX512PF__
     __AVX512VL__
     __AVX512DQ__
   */
   
   /*****************************************************\
   * Fun05 Sec01 Sub01:
   *   - avx512 popcntdq
   \*****************************************************/

   #ifdef __AVX512VPOPCNTDQ__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec01 Sub02:
   *   - avx512 VBMI
   \*****************************************************/

   #ifdef __AVX512VBMI__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec01 Sub03:
   *   - avx512 IFMA
   \*****************************************************/

   #ifdef __AVX512IFMA__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec01 Sub04:
   *   - avx512 VNNIW
   \*****************************************************/

   #ifdef __AVX5124VNNIW__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec01 Sub05:
   *   - avx512 FMAPS
   \*****************************************************/

   #ifdef __AVX5124FMAPS__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec01 Sub06:
   *   - avx512 BW (my minimum)
   \*****************************************************/

   #ifdef __AVX512BW__
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - AVX2 simd macro
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*is both GCC and windows*/
   #ifdef __AVX2__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - AVX simd macro
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*is both GCC and windows*/
   #ifdef __AVX__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - SSE4 macros
   ^   o fun05 sec04 sub01:
   ^     - SSE4.2 macro
   ^   o fun05 sec04 sub02:
   ^     - SSE4.1 macro
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec04 Sub01:
   *   - SSE4.2 macro
   \*****************************************************/

   #ifdef __SSE4_2__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec04 Sub02:
   *   - SSE4.1 macro
   \*****************************************************/

   #ifdef __SSE4_1__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec05:
   ^   - SSE3 macros
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef __SSSE3__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   #ifdef __SSE3__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec06:
   ^   - SSE2 macros
   ^   o fun05 sec06 sub01:
   ^     - non-windows sse2 macros
   ^   o fun05 sec06 sub02:
   ^     - windows sse2 macros (64 bit)
   ^   o fun05 sec06 sub03:
   ^     - windows sse2 macros (32 bit)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec06 Sub01:
   *   - non-windows sse2 macros
   \*****************************************************/

   #ifdef __SSE2_MATH__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   #ifdef __SSE2__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec06 Sub02:
   *   - 64 bit windows sse2 macros
   \*****************************************************/

   #ifdef _M_AMD64
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   #ifdef _M_X64
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*****************************************************\
   * Fun05 Sec06 Sub03:
   *   - 32 bit windows sse2 macros
   \*****************************************************/

   #ifdef _M_IX86_FP
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec06:
   ^   - SSE macros
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #ifdef __SSE__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   #ifdef __SSE_MATH__
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

      goto lastSIMD_fun05_sec07;
   #endif

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec07:
   ^   - define scalar fallback (no SIMD)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(osSTPtr->simdAryStr[def_0thSIMD_osST])
      osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_1stSIMD_osST])
      osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
      osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
      osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_4thSIMD_osST])
      osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_5thSIMD_osST])
      osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';
   
   goto lastSIMD_fun05_sec07;

   lastSIMD_fun05_sec07:;

   if(
      cpStr_osST(
         &osSTPtr->simdAryStr[def_lastSIMD_osST],
         &osSTPtr->sizeSIMDAryUC[def_lastSIMD_osST],
         glob_simdAryStr[def_scalarSIMD_osST]
      )
   ) return def_memErr_osST;

   return 0;
} /*getSIMD_specs_osST*/

/*-------------------------------------------------------\
| Fun06: getEndin_specs_osST
|   - gets if cpu is big or little endin
| Input:
|   - osSTPtr:
|     o pointer to specs_osST struct to add endin to
| Output:
|   - Modifies:
|     o endinUC in osSTPtr to be def_bigEndin_osST if
|       cpu is big endin
|     o endinUC in osSTPtr to be def_littleEndin_osST if
|       cpu is little endin
| Note:
|   - trick is from stack overflow
|     https://stackoverflow.com/questions/12791864/c-program-to-check-little-vs-big-endin
\-------------------------------------------------------*/
void
getEndin_specs_osST(
   struct specs_osST *osSTPtr
){
   int testSI = 1;
   char *endSCPtr = (char *) &testSI;

   if( *endSCPtr )
      osSTPtr->endinUC = def_littleEndIn_osST;
   else
      osSTPtr->endinUC = def_bigEndIn_osST;

   /*In the stack overflow it was mentioned pointers were
   `  used to avoid compiler fixing assignment
   */
} /*specs_osST*/

/*-------------------------------------------------------\
| Fun07: blank_specs_osST
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
){
   unsigned char ucCnt = 0;

   osSTPtr->osBitUS = 0;
   osSTPtr->endinUC = 0;

   for(
      ucCnt = 0;
      ucCnt < def_osDepth_osST;
      ++ucCnt
   ){ /*Loop: blank OS varaibles*/

      osSTPtr->osIndexAryUC[ucCnt] = 0;

      if(osSTPtr->osAryStr)
      { /*If: have os array*/
         if(osSTPtr->osAryStr[ucCnt])
            osSTPtr->osAryStr[ucCnt][0] = '\0';
      } /*If: have os array*/
   } /*Loop: blank OS varaibles*/

   for(
      ucCnt = 0;
      ucCnt < def_maxSIMD_osST;
      ++ucCnt
   ){ /*Loop: blank simd varaibles*/
      osSTPtr->simdIndexAryUC[ucCnt] = 0;

      if(osSTPtr->simdAryStr)
      { /*If: have simd array*/
         if(osSTPtr->simdAryStr[ucCnt])
            osSTPtr->simdAryStr[ucCnt][0] = '\0';
      } /*If: have simd array*/
   } /*Loop: blank simd varaibles*/

   for(
      ucCnt = 0;
      ucCnt < def_maxCPU_osST;
      ++ucCnt
   ){ /*Loop: blank cpu varaibles*/
      osSTPtr->cpuIndexAryUC[ucCnt] = 0;

      if(osSTPtr->cpuAryStr)
      { /*If: have cpu array*/
         if(osSTPtr->cpuAryStr[ucCnt])
            osSTPtr->cpuAryStr[ucCnt][0] = '\0';
      } /*If: have cpu array*/

   } /*Loop: blank cpu varaibles*/
} /*blank_specs_osST*/

/*-------------------------------------------------------\
| Fun08: init_specs_osST
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
){
   unsigned char ucCnt = 0;

   /*set alloc arrays to null*/
   osSTPtr->osAryStr = 0;
   osSTPtr->simdAryStr = 0;
   osSTPtr->cpuAryStr = 0;
   osSTPtr->osBitUS = 0;

   /*need to blank the size arrays*/
   for(
      ucCnt = 0;
      ucCnt < def_osDepth_osST;
      ++ucCnt
   ) osSTPtr->sizeOSAryUC[ucCnt] = 0;

   for(
      ucCnt = 0;
      ucCnt < def_maxSIMD_osST;
      ++ucCnt
   ) osSTPtr->sizeSIMDAryUC[ucCnt] = 0;

   for(
      ucCnt = 0;
      ucCnt < def_maxCPU_osST;
      ++ucCnt
   ) osSTPtr->sizeCPUAryUC[ucCnt] = 0;

   blank_specs_osST(osSTPtr);
} /*init_specs_osST*/

/*-------------------------------------------------------\
| Fun09: freeStack_specs_osST
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
){
   unsigned char ucCnt = 0;

   if(! osSTPtr)
      return;

   if(osSTPtr->osAryStr)
   { /*If: need to free the os array*/
      for(
         ucCnt = 0;
         ucCnt < def_osDepth_osST;
         ++ucCnt
      ){ /*Loop: free individal arrays*/
         if(osSTPtr->osAryStr[ucCnt])
            free(osSTPtr->osAryStr[ucCnt]);
         osSTPtr->osAryStr[ucCnt] = 0;
      } /*Loop: free individal arrays*/

      free(osSTPtr->osAryStr);
   } /*If: need to free the os array*/

   if(osSTPtr->simdAryStr)
   { /*If: need to free the os array*/
      for(
         ucCnt = 0;
         ucCnt < def_maxSIMD_osST;
         ++ucCnt
      ){ /*Loop: free individal arrays*/
         if(osSTPtr->simdAryStr[ucCnt])
            free(osSTPtr->simdAryStr[ucCnt]);
         osSTPtr->simdAryStr[ucCnt] = 0;
      } /*Loop: free individal arrays*/

      free(osSTPtr->simdAryStr);
   } /*If: need to free the os array*/

   if(osSTPtr->cpuAryStr)
   { /*If: need to free the os array*/
      for(
         ucCnt = 0;
         ucCnt < def_maxCPU_osST;
         ++ucCnt
      ){ /*Loop: free individal arrays*/
         if(osSTPtr->cpuAryStr[ucCnt])
            free(osSTPtr->cpuAryStr[ucCnt]);
         osSTPtr->cpuAryStr[ucCnt] = 0;
      } /*Loop: free individal arrays*/

      free(osSTPtr->cpuAryStr);
   } /*If: need to free the os array*/


   init_specs_osST(osSTPtr); /*initialize structure*/
} /*freeStack_sepcs_osST*/

/*-------------------------------------------------------\
| Fun10: freeHeap_specs_osST
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
){
   if(! osSTPtr)
      return;

   freeStack_specs_osST(osSTPtr);
   free(osSTPtr);
} /*freeHeap_specs_osST*/

/*-------------------------------------------------------\
| Fun11: setup_specs_osST
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - setups a specs_osST struct
   '   o fun11 sec01:
   '     - variable declaration + clearing old data
   '   o fun11 sec02:
   '     - allocate memory
   '   o fun11 sec03:
   '     - get system specs (from macros) + return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declaration + clearing old data
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char ucCnt = 0;
   freeStack_specs_osST(osSTPtr);/*calls init_specs_osST*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   osSTPtr->osAryStr =
      malloc(def_osDepth_osST * sizeof(signed char *));

   if(! osSTPtr->osAryStr)
      goto memErr_fun11_sec0x;

   for(
      ucCnt = 0;
      ucCnt < def_osDepth_osST;
      ++ucCnt
   ) osSTPtr->osAryStr[ucCnt] = 0;
      

   osSTPtr->simdAryStr =
      malloc(def_maxSIMD_osST * sizeof(signed char *));

   if(! osSTPtr->simdAryStr)
      goto memErr_fun11_sec0x;

   for(
      ucCnt = 0;
      ucCnt < def_maxSIMD_osST;
      ++ucCnt
   ) osSTPtr->simdAryStr[ucCnt] = 0;
      

   osSTPtr->cpuAryStr =
      malloc(def_maxCPU_osST * sizeof(signed char *));

   if(! osSTPtr->cpuAryStr)
      goto memErr_fun11_sec0x;

   for(
      ucCnt = 0;
      ucCnt < def_maxCPU_osST;
      ++ucCnt
   ) osSTPtr->cpuAryStr[ucCnt] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - get system specs (from macros) + return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*get system specs*/
   getEndin_specs_osST(osSTPtr);
   getCpuBit_specs_osST(osSTPtr);

   if(getSIMD_specs_osST(osSTPtr))
      goto memErr_fun11_sec0x;

   /*cpu is after simd because ARM simd (neon/arm6)
   `  support is detected by OS not compiler flags
   */
   if(getCpu_specs_osST(osSTPtr))
      goto memErr_fun11_sec0x;

   if(getOS_specs_osST(osSTPtr))
      goto memErr_fun11_sec0x;

   return 0;

   memErr_fun11_sec0x:;
      return def_memErr_osST;
} /*setup_specs_osST*/

/*-------------------------------------------------------\
| Fun12: mk_specs_osST
|   - makes and returns heap allocated specs_osST struct
| Input:
| Output:
|   - Returns:
|     o pointer to heap allocated, setup specs_osST struct
|     o 0 for memory errorsj
\-------------------------------------------------------*/
struct specs_osST *
mk_specs_osST(
){
   struct specs_osST *retHeapST = 0;

   retHeapST = malloc(sizeof(specs_osST));

   if(! retHeapST)
      goto memErr_fun12;

   init_specs_osST(retHeapST);

   if(setup_specs_osST(retHeapST))
      goto memErr_fun12;

   goto ret_fun12;

   memErr_fun12:;
      if(retHeapST)
         freeHeap_specs_osST(retHeapST);
      retHeapST = 0;
      goto ret_fun12;

   ret_fun12:;
      return retHeapST;
} /*mk_specs_osST*/

/*-------------------------------------------------------\
| Fun13: addOS_specs_osST
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - adds OS to os list in a specs_osST struct
   '   o fun13 sec01:
   '     - add os to user specified level (not -1)
   '   o fun13 sec02:
   '     - level not specified, find os level and add
   '   o fun13 sec03:
   '     - add all os and return (level = -1 only)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - add os to user specified level (not -1)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(levSC > def_allOS_osST)
      return def_levelErr_osST;
      /*unkown simd level*/

   if(levSC == def_allOS_osST)
      levSC = def_groupOS_osST;

   if(levSC >= 0)
   { /*If: user wanted a specific level*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[(unsigned char) levSC],
            &osSTPtr->sizeOSAryUC[(unsigned char) levSC],
            osStr
         )
      ) return def_memErr_osST;

      return 0; /*added os*/
   } /*If: user wanted a specific level*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - level not specified find OS level and add
   ^   o fun13 sec02 sub01:
   ^     - blank all os levels (except all level)
   ^   o fun13 sec02 sub02:
   ^     - android OS
   ^   o fun13 sec02 sub03:
   ^     - linux OS
   ^   o fun13 sec02 sub04:
   ^     - mac OS
   ^   o fun13 sec02 sub05:
   ^     - bsd OS
   ^   o fun13 sec02 sub06:
   ^     - bsd OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun13 Sec02 Sub01:
   *   - blank os levels
   \*****************************************************/

   if(osSTPtr->osAryStr[def_varOS_osST])
      osSTPtr->osAryStr[def_varOS_osST][0] = '\0';

   if(osSTPtr->osAryStr[def_subOS_osST])
      osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

   if(osSTPtr->osAryStr[def_groupOS_osST])
      osSTPtr->osAryStr[def_groupOS_osST][0] = '\0';

   /*****************************************************\
   * Fun13 Sec02 Sub02:
   *   - android OS
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_androidIndex_osST]
      )
   ){ /*If: android OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_androidIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*If: android OS*/

   /*****************************************************\
   * Fun13 Sec02 Sub03:
   *   - linux OS
   \*****************************************************/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_linuxIndex_osST]
      )
   ){ /*Else If: linux OS*/

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_linuxIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: linux OS*/

   /*****************************************************\
   * Fun13 Sec02 Sub04:
   *   - mac OS
   \*****************************************************/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_linuxIndex_osST]
      )
   ){ /*Else If: mac OS*/

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_macIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: mac OS*/

   /*****************************************************\
   * Fun13 Sec02 Sub05:
   *   - bsd OS
   *   o fun13 sec02 sub05 cat01:
   *     - openbsd OS
   *   o fun13 sec02 sub05 cat02:
   *     - freebsd OS
   *   o fun13 sec02 sub05 cat03:
   *     - netbsd OS
   *   o fun13 sec02 sub05 cat04:
   *     - drgonflyebsd OS
   *   o fun13 sec02 sub05 cat05:
   *     - bsd OS
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun13 Sec02 Sub05 Cat01:
   +   - openbsd OS
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_openBsdIndex_osST]
      )
   ){ /*Else If: openbsd OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_openBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: openbsd OS*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun13 Sec02 Sub05 Cat02:
   +   - freebsd OS
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_freeBsdIndex_osST]
      )
   ){ /*Else If: freebsd OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_freeBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: freebsd OS*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun13 Sec02 Sub05 Cat03:
   +   - netbsd OS
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_netBsdIndex_osST]
      )
   ){ /*Else If: netbsd OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_netBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: netbsd OS*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun13 Sec02 Sub05 Cat04:
   +   - dragonflyebsd OS
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_dragonflyeBsdIndex_osST]
      )
   ){ /*Else If: freebsd OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_varOS_osST],
            &osSTPtr->sizeOSAryUC[def_varOS_osST],
            glob_osAryStr[def_dragonflyeBsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: dragonflybsd OS*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun13 Sec02 Sub05 Cat05:
   +   - bsd (general) OS
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_bsdIndex_osST]
      )
   ){ /*Else If: bsd (general) OS*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_subOS_osST],
            &osSTPtr->sizeOSAryUC[def_subOS_osST],
            glob_osAryStr[def_bsdIndex_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            glob_osAryStr[def_unixIndex_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: bsd (general) OS*/

   /*****************************************************\
   * Fun13 Sec02 Sub06:
   *   - os with only group level
   \*****************************************************/

   else if(
      eqlNull_ulCp(
         osStr,
         glob_osAryStr[def_allIndex_osST]
      ) /*make sure not all os flag*/
   ){ /*Else: OS does not have subgroup or variant*/
      if(
         cpStr_osST(
            &osSTPtr->osAryStr[def_groupOS_osST],
            &osSTPtr->sizeOSAryUC[def_groupOS_osST],
            osStr
         )
      ) return def_memErr_osST;
   } /*Else: OS does not have subgroup or variant*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - add all os group label in and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
      cpStr_osST(
         &osSTPtr->osAryStr[def_allOS_osST],
         &osSTPtr->sizeOSAryUC[def_allOS_osST],
         glob_osAryStr[def_allIndex_osST]
      )
   ) return def_memErr_osST;

   return 0;
} /*addOS_specs_osST*/

/*-------------------------------------------------------\
| Fun14: addCpu_specs_osST
|   - adds a cpu to the cpu list in a specs_osST struct
|     and for ARM cpus adds if has simd support (by cpu 
|     version)
| Input:
|   - osSTPtr:
|     o  pointer to a specs_osST struct to add cpu to
|   - cpuStr:
|     o c-string with cpu to add
|   - levSC:
|     o level t- add OS to (-1 for lowest possible)
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - adds cpu to os list in a specs_osST struct
   '   o fun14 sec01:
   '     - add cpu to user specified level (not -1)
   '   o fun14 sec02:
   '     - find cpu level and check if arm (levSC < 0)
   '   o fun14 sec03:
   '     - if ont arm cpu, add as type cpu (levSC < 0)
   '   o fun14 sec04:
   '     - add all cpu flag in (levSC < 0) and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - add cpu to user specified level (not -1)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(levSC > def_lastCPU_osST)
      return def_levelErr_osST;
      /*unkown simd level*/

   if(levSC == def_lastCPU_osST)
      levSC = def_cpuType_osST;

   if(levSC >= 0)
   { /*If: user wanted a specific level*/
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[(unsigned char) levSC],
            &osSTPtr->sizeCPUAryUC[(unsigned char) levSC],
            cpuStr
         )
      ) return def_memErr_osST;

      return 0; /*added cpu*/
   } /*If: user wanted a specific level*/
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - find cpu level and add (check if ARM)
   ^   o fun14 sec02 sub01:
   ^     - detect if is AMR cpu (has version) add ARM type
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec02 Sub01:
   *   - detect if is AMR cpu (has version) add ARM type
   \*****************************************************/

   if(
         (cpuStr[0] & ~32) != 'A'
      || (cpuStr[1] & ~32) != 'R'
      || (cpuStr[2] & ~32) != 'M'
   ) goto addCpuType_fun14_sec03;
     /*not an ARM cpu*/

   if(
      cpStr_osST(
         &osSTPtr->cpuAryStr[def_cpuType_osST],
         &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
         glob_cpuAryStr[def_armCPU_osST]
      )
   ) return def_memErr_osST;

   /*****************************************************\
   * Fun14 Sec02 Sub02:
   *   - detect if is AMR cpu (has version) add ARM type
   *   o fun14 sec02 sub02 cat01:
   *     - arm version 2 + start switch
   *   o fun14 sec02 sub02 cat02:
   *     - arm version 3
   *   o fun14 sec02 sub02 cat03:
   *     - arm version4
   *   o fun14 sec02 sub02 cat04:
   *     - arm version 5
   *   o fun14 sec02 sub02 cat05:
   *     - arm version 6 and arm6 simd
   *   o fun14 sec02 sub02 cat06:
   *     - arm version 7 and neon + aarch32 (neon32)
   *   o fun14 sec02 sub02 cat07:
   *     - arm version 8 and neon + aarch64 (neon64)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun14 Sec02 Sub02 Cat01:
   +   - arm version 2 + start switch
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   switch(cpuStr[3])
   { /*Switch: check ARM cpu version*/
      case '2':
      /*Case: ARM version 2*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm2CPU_osST]
            )
         ) return def_memErr_osST; /*version 2*/

         break;
      /*Case: ARM version 2*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat02:
      +   - arm version 3
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '3':
      /*Case: ARM version 3*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm3CPU_osST]
            )
         ) return def_memErr_osST; /*version 3*/

         break;
      /*Case: ARM version 3*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat03:
      +   - arm version 4
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '4':
      /*Case: ARM version 4*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm4CPU_osST]
            )
         ) return def_memErr_osST; /*version 4*/

         break;
      /*Case: ARM version 4*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat04:
      +   - arm version 5
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '5':
      /*Case: ARM version 5*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm5CPU_osST]
            )
         ) return def_memErr_osST; /*version 5*/

         break;
      /*Case: ARM version 5*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat05:
      +   - arm version 6 + arm6 simd
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '6':
      /*Case: ARM version 6*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm6CPU_osST]
            )
         ) return def_memErr_osST; /*version 6*/

         if(
            cpStr_osST(
               &osSTPtr->simdAryStr[def_0thSIMD_osST],
               &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
               glob_simdAryStr[def_arm6SIMD_osST]
            )
         ) return def_memErr_osST;
   
         if(osSTPtr->simdAryStr[def_1stSIMD_osST])
            osSTPtr->simdAryStr[def_1stSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
            osSTPtr->simdAryStr[def_2ndSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
            osSTPtr->simdAryStr[def_3rdSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_4thSIMD_osST])
            osSTPtr->simdAryStr[def_4thSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_5thSIMD_osST])
            osSTPtr->simdAryStr[def_5thSIMD_osST][0]='\0';

         break;
      /*Case: ARM version 6*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat06:
      +   - arm version 7 + neon aarch 32
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '7':
      /*Case: ARM version 7*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm7CPU_osST]
            )
         ) return def_memErr_osST; /*version 7*/

         if(
            cpStr_osST(
               &osSTPtr->simdAryStr[def_0thSIMD_osST],
               &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
               glob_simdAryStr[def_neon32SIMD_osST]
            )
         ) return def_memErr_osST;

         if(
            cpStr_osST(
               &osSTPtr->simdAryStr[def_1stSIMD_osST],
               &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
               glob_simdAryStr[def_neonSIMD_osST]
            )
         ) return def_memErr_osST;
   
         if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
            osSTPtr->simdAryStr[def_2ndSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
            osSTPtr->simdAryStr[def_3rdSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_4thSIMD_osST])
            osSTPtr->simdAryStr[def_4thSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_5thSIMD_osST])
            osSTPtr->simdAryStr[def_5thSIMD_osST][0]='\0';

         break;
      /*Case: ARM version 7*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun14 Sec02 Sub02 Cat07:
      +   - arm version 8 + neon aarch 64
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      case '8':
      /*Case: ARM version 8*/
         if(
            cpStr_osST(
               &osSTPtr->cpuAryStr[def_cpuVer_osST],
               &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
               glob_cpuAryStr[def_arm8CPU_osST]
            )
         ) return def_memErr_osST; /*version 8*/

         if(
            cpStr_osST(
               &osSTPtr->simdAryStr[def_0thSIMD_osST],
               &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
               glob_simdAryStr[def_neon64SIMD_osST]
            )
         ) return def_memErr_osST;

         if(
            cpStr_osST(
               &osSTPtr->simdAryStr[def_1stSIMD_osST],
               &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
               glob_simdAryStr[def_neonSIMD_osST]
            )
         ) return def_memErr_osST;

         if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
            osSTPtr->simdAryStr[def_2ndSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
            osSTPtr->simdAryStr[def_3rdSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_4thSIMD_osST])
            osSTPtr->simdAryStr[def_4thSIMD_osST][0]='\0';
   
         if(osSTPtr->simdAryStr[def_5thSIMD_osST])
            osSTPtr->simdAryStr[def_5thSIMD_osST][0]='\0';

         break;
      /*Case: ARM version 8*/
   } /*Switch: check ARM cpu version*/

   goto allCpu_fun14_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - non-ARM cpu add as type (no version)
   ^   o fun14 sec03 sub01:
   ^     - check if moterola 68000 cpu
   ^   o fun14 sec03 sub02:
   ^     - check if moterola 68020 cpu
   ^   o fun14 sec03 sub03:
   ^     - else cpu without version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   addCpuType_fun14_sec03:;

   /*****************************************************\
   * Fun14 Sec03 Sub01:
   *   - check if moterola 68000 cpu
   \*****************************************************/

   if(
      eqlNull_ulCp(
         cpuStr,
         glob_cpuAryStr[def_68000CPU_osST]
      )
   ){ /*If: moterola 68000 cpu*/
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_68000CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_m68kCPU_osST]
         )
      ) return def_memErr_osST;

      goto allCpu_fun14_sec04;
   } /*If: moterola 68000 cpu*/

   /*****************************************************\
   * Fun14 Sec03 Sub02:
   *   - check if moterola 68020 cpu
   \*****************************************************/

   else if(
      eqlNull_ulCp(
         cpuStr,
         glob_cpuAryStr[def_68020CPU_osST]
      )
   ){ /*If: moterola 68020 cpu*/
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuVer_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuVer_osST],
            glob_cpuAryStr[def_68020CPU_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            glob_cpuAryStr[def_m68kCPU_osST]
         )
      ) return def_memErr_osST;

      goto allCpu_fun14_sec04;
   } /*If: moterola 68020 cpu*/

   /*****************************************************\
   * Fun14 Sec03 Sub03:
   *   - else cpu without version
   \*****************************************************/

   else if(
      eqlNull_ulCp(
         cpuStr,
         glob_cpuAryStr[def_allCPU_osST]
      )
   ){ /*Else If: not all cpu flags*/
      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_cpuType_osST],
            &osSTPtr->sizeCPUAryUC[def_cpuType_osST],
            cpuStr
         )
      ) return def_memErr_osST;
   } /*Else If: not all cpu flags*/

   goto allCpu_fun14_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec04:
   ^   - add all cpu type flag
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   allCpu_fun14_sec04:;

      if(
         cpStr_osST(
            &osSTPtr->cpuAryStr[def_lastCPU_osST],
            &osSTPtr->sizeCPUAryUC[def_lastCPU_osST],
            glob_cpuAryStr[def_allCPU_osST]
         )
      ) return def_memErr_osST;

      return 0;
} /*addCpu_specs_osST*/

/*-------------------------------------------------------\
| Fun15: addSIMD_specs_osST
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - adds simd to simd list in a specs_osST struct
   '   o fun15 sec01:
   '     - add simd to user specified level (not -1)
   '   o fun15 sec02:
   '     - level un15specified, find simd level and add
   '   o fun15 sec03:
   '     - if un15kown simd (levSC < 0)
   '   o fun15 sec04:
   '     - add simd cpu flag in (levSC < 0) and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - add simd to user specified level (not -1)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(levSC > def_lastSIMD_osST)
      return def_levelErr_osST;
      /*un15kown simd level*/

   if(levSC == def_lastSIMD_osST)
      levSC = def_5thSIMD_osST;

   if(levSC >= 0)
   { /*If: user wanted a specific level*/
      if(
         cpStr_osST(
           &osSTPtr->simdAryStr[(unsigned char) levSC],
           &osSTPtr->sizeSIMDAryUC[(unsigned char) levSC],
           simdStr
         )
      ) return def_memErr_osST;

      return 0; /*added simd*/
   } /*If: user wanted a specific level*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - level un15specified, find simd level and add
   ^   o fun15 sec02 sub01:
   ^     - blank simd values
   ^   o fun15 sec02 sub02:
   ^     - x86/amd64 simd
   ^   o fun15 sec02 sub03:
   ^     - neon simd
   ^   o fun15 sec02 sub04:
   ^     - simds with only one level (exluding last (all))
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec02 Sub01:
   *   - blank simd values
   \*****************************************************/

   if(osSTPtr->simdAryStr[def_0thSIMD_osST])
      osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_1stSIMD_osST])
      osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
      osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
      osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_4thSIMD_osST])
      osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

   if(osSTPtr->simdAryStr[def_5thSIMD_osST])
      osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

   /*****************************************************\
   * Fun15 Sec02 Sub02:
   *   - x86/amd64 simd
   *   o fun15 sec02 sub02 cat01:
   *     - avx512
   *   o fun15 sec02 sub02 cat02:
   *     - avx2
   *   o fun15 sec02 sub02 cat03:
   *     - sse4
   *   o fun15 sec02 sub02 cat04:
   *     - ss3
   *   o fun15 sec02 sub02 cat05:
   *     - sse2
   *   o note: sse is only a single level (sub04 handles)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub02 Cat01:
   +   - avx512
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_avx512SIMD_osST]
      )
   ){ /*If: avx512*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_0thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_0thSIMD_osST],
            glob_simdAryStr[def_avx512SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*If: avx512*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub02 Cat02:
   +   - avx2
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_avx2SIMD_osST]
      )
   ){ /*Else If: avx2*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_1stSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_1stSIMD_osST],
            glob_simdAryStr[def_avx2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*Else If: avx2*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub02 Cat03:
   +   - sse4
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_sse4SIMD_osST]
      )
   ){ /*Else If: sse4*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_2ndSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_2ndSIMD_osST],
            glob_simdAryStr[def_sse4SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*Else If: sse4*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub02 Cat04:
   +   - sse3
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_sse3SIMD_osST]
      )
   ){ /*Else If: sse3*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_3rdSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_3rdSIMD_osST],
            glob_simdAryStr[def_sse3SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*Else If: sse3*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub02 Cat05:
   +   - sse2
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_sse2SIMD_osST]
      )
   ){ /*Else If: sse2*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_sse2SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_sseSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*Else If: sse2*/

   /*****************************************************\
   * Fun15 Sec02 Sub03:
   *   - ARM neon simd
   *   o fun15 sec02 sub03 cat01:
   *     - neon32 (neon for aarch32)
   *   o fun15 sec02 sub03 cat02:
   *     - neon64 (neon for aarch64)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub03 Cat01:
   +   - neon32 (neon for aarch32)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_neon32SIMD_osST]
      )
   ){ /*Else If: neon32*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_neon32SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;

   } /*Else If: neon32*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec02 Sub03 Cat02:
   +   - neon64 (neon for aarch64)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(
      ! eqlNull_ulCp(
         simdStr,
         glob_simdAryStr[def_neon64SIMD_osST]
      )
   ){ /*Else If: neon64*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_4thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_4thSIMD_osST],
            glob_simdAryStr[def_neon64SIMD_osST]
         )
      ) return def_memErr_osST;

      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            glob_simdAryStr[def_neonSIMD_osST]
         )
      ) return def_memErr_osST;
   } /*Else If: neon64*/

   /*****************************************************\
   * Fun15 Sec02 Sub03:
   *   - simds with only one level (exluding last (all))
   \*****************************************************/

   else if(
      eqlNull_ulCp(
         glob_simdAryStr[def_scalarSIMD_osST],
         simdStr
      )
   ){ /*Else: simd only has one level (not scalar)*/
      if(
         cpStr_osST(
            &osSTPtr->simdAryStr[def_5thSIMD_osST],
            &osSTPtr->sizeSIMDAryUC[def_5thSIMD_osST],
            simdStr
         )
      ) return def_memErr_osST;
   } /*Else: simd only has one level (not scalar)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - add default scalar simd in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
      cpStr_osST(
         &osSTPtr->simdAryStr[def_lastSIMD_osST],
         &osSTPtr->sizeSIMDAryUC[def_lastSIMD_osST],
         glob_simdAryStr[def_scalarSIMD_osST]
      )
   ) return def_memErr_osST;

   return 0;
} /*addSIMD_specs_osST*/

/*-------------------------------------------------------\
| Fun16: addBit_specs_osST
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
){
   osSTPtr->osBitUS = bitUS;
} /*addBig_specs_osST*/

/*-------------------------------------------------------\
| Fun17: addEndin_specs_osST
|   - add if big end in or small end in
| Input:
|   - osSTPtr:
|     o pointer to a specs_osST struct add endin to
|   - endinBl:
|     o 1: for big endin
|     o 0: for small endin
| Output:
\-------------------------------------------------------*/
void
addEndin_specs_osST(
   struct specs_osST *osSTPtr,
   unsigned char endinBl
){
   osSTPtr->endinUC = endinBl;
} /*addBig_specs_osST*/

/*-------------------------------------------------------\
| Fun18: addFlag_specs_osST
|   - uses flags to add a value into a specs_osST struct
| Input:
|   - osSTPtr:
|     o pointer to a specs_osST struct to add value to
|   - flagStr:
|     o flag to add in
|   - valStr:
|     o c-string with value (can be 0 if value in flag)
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
|     o defbadBit_osST if value for bit flag is
|       non-numierc or to large
\-------------------------------------------------------*/
signed char
addFlag_specs_osST(
   struct specs_osST *osSTPtr,
   signed char *flagStr,
   signed char *valStr,
   signed int *argSIPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - uses flags to add value into specs_osST struct
   '   o fun18 sec01:
   '     - os flags
   '   o fun18 sec02:
   '     - cpu flags
   '   o fun18 sec03:
   '     - simd flags
   '   o fun18 sec04:
   '     - cpu bit type
   '   o fun18 sec05:
   '     - add endin in
   '   o fun18 sec06:
   '     - blank stat flag
   '   o fun18 sec07:
   '     - unkown flag
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - os flags
   ^   o fun18 sec01 sub01:
   ^     - os flags; any OS
   ^   o fun18 sec01 sub02:
   ^     - specific os input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec01 Sub01:
   *   - os flags; any OS
   *   o fun18 sec01 sub01 cat01:
   *     - no level any os
   *   o fun18 sec01 sub01 cat02:
   *     - variant level any os
   *   o fun18 sec01 sub01 cat03:
   *     - subgroup level any os
   *   o fun18 sec01 sub01 cat04:
   *     - group level any os
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub01 Cat01:
   +   - no level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-os"
      )
   ){ /*If: os flag*/
      if(! valStr)
         return def_badFlag_osST;
      if(
         addOS_specs_osST(
            osSTPtr,
            valStr,
            -1
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: os flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub01 Cat02:
   +   - variant level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-os-var"
      )
   ){ /*If: os variant flag*/
      if(! valStr)
         return def_badFlag_osST;
      if(
         addOS_specs_osST(
            osSTPtr,
            valStr,
            def_varOS_osST
         )
      ) return def_memErr_osST;

      (*argSIPtr) += 2;
      return 0;
   } /*If: os variant flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub01 Cat03:
   +   - subgroup level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-os-sub"
      )
   ){ /*If: os subgroup flag*/
      if(! valStr)
         return def_badFlag_osST;
      if(
         addOS_specs_osST(
            osSTPtr,
            valStr,
            def_subOS_osST
         )
      ) return def_memErr_osST;

      (*argSIPtr) += 2;
      return 0;
   } /*If: os subgroup flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub01 Cat04:
   +   - group level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-os-group"
      )
   ){ /*If: os group flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addOS_specs_osST(
            osSTPtr,
            valStr,
            def_groupOS_osST
         )
      ) return def_memErr_osST;

      (*argSIPtr) += 2;
      return 0;
   } /*If: os group flag*/

   /*****************************************************\
   * Fun18 Sec01 Sub02:
   *   - specific os input
   *   o fun18 sec01 sub02 cat01:
   *     - droid (android) os flag
   *   o fun18 sec01 sub02 cat02:
   *     - linux os flag
   *   o fun18 sec01 sub02 cat03:
   *     - mac os flag
   *   o fun18 sec01 sub02 cat04:
   *     - openbsd os flag
   *   o fun18 sec01 sub02 cat05:
   *     - freebsd os flag
   *   o fun18 sec01 sub02 cat06:
   *     - netbsd os flag
   *   o fun18 sec01 sub02 cat07:
   *     - dragonflye bsd os flag
   *   o fun18 sec01 sub02 cat08:
   *     - bsd os flag
   *   o fun18 sec01 sub02 cat09:
   *     - unix os flag
   *   o fun18 sec01 sub02 cat10:
   *     - amiga os flag
   *   o fun18 sec01 sub02 cat11:
   *     - beOS os
   *   o fun18 sec01 sub02 cat12:
   *     - sun os
   *   o fun18 sec01 sub02 cat13:
   *     - morph os
   *   o fun18 sec01 sub02 cat14:
   *     - syllable os
   *   o fun18 sec01 sub02 cat15:
   *     - hurd os
   *   o fun18 sec01 sub02 cat16:
   *     - xinuos os
   *   o fun18 sec01 sub02 cat17:
   *     - plan9 os
   *   o fun18 sec01 sub02 cat18:
   *     - win (windows) os
   *   o fun18 sec01 sub02 cat19:
   *     - cygwin (windows lynx enviroment)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat01:
   +   - droid (android) os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-droid"
      )
   ){ /*If: android os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_androidIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: android os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat02:
   +   - linux os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-linux"
      )
   ){ /*If: linux os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_linuxIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: linux os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat03:
   +   - mac os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-mac"
      )
   ){ /*If: mac os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_macIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: mac os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat04:
   +   - openbsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-openbsd"
      )
   ){ /*If: openbsd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_openBsdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: openbsd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat05:
   +   - freebsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-freebsd"
      )
   ){ /*If: freebsd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_freeBsdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: freebsd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat06:
   +   - netbsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-netbsd"
      )
   ){ /*If: netbsd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_netBsdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: netbsd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat07:
   +   - dragonflyebsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-dragonflyebsd"
      )
   ){ /*If: dragonflyebsd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_dragonflyeBsdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: dragonflyebsd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat08:
   +   - bsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-bsd"
      )
   ){ /*If: bsd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_bsdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: bsd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat09:
   +   - unix os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-unix"
      )
   ){ /*If: unix os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_unixIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: unix os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat10:
   +   - amiga os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-amiga"
      )
   ){ /*If: amiga os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_amigaIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: amiga os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat11:
   +   - beOS os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-beos"
      )
   ){ /*If: beos os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_beOSIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: beos os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat12:
   +   - sun os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sun"
      )
   ){ /*If: sun os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_sunIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sun os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat13:
   +   - morph os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-morph"
      )
   ){ /*If: morph os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_morphIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: morph os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat14:
   +   - syllable os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-syllable"
      )
   ){ /*If: syllable os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_syllableIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: syllable os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat15:
   +   - hurd os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-hurd"
      )
   ){ /*If: hurd os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_hurdIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: hurd os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat16:
   +   - xinuos os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-xinuos"
      )
   ){ /*If: xinuos os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_xinuosIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: xinuos os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat17:
   +   - plan9 os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-plan9"
      )
   ){ /*If: plan9 os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_plan9Index_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: plan9 os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat18:
   +   - win os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-win"
      )
   ){ /*If: win os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_winIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: win os*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec01 Sub02 Cat19:
   +   - cygwin (windows lynx enviroment)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-cygwin"
      )
   ){ /*If: cygwin os*/
      if(
         addOS_specs_osST(
            osSTPtr,
            glob_osAryStr[def_cygwinIndex_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: cygwin os*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - cpu flags
   ^   o fun18 sec01 sub01:
   ^     - cpu flags; any cpu
   ^   o fun18 sec01 sub02:
   ^     - arm cpu flags
   ^   o fun18 sec01 sub03:
   ^     - cpu flags; non-arm cpu flags
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec02 Sub01:
   *   - cpu flags; any cpu
   *   o fun18 sec02 sub01 cat01:
   *     - no level any cpu
   *   o fun18 sec02 sub01 cat02:
   *     - cpu version level; any cpu
   *   o fun18 sec02 sub01 cat03:
   *     - cpu type level; any cpu
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub01 Cat01:
   +   - no level any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-cpu"
      )
   ){ /*If: cpu flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addCpu_specs_osST(
            osSTPtr,
            valStr,
            -1
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: cpu flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub01 Cat02:
   +   - cpu version level; any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-cpu-ver"
      )
   ){ /*If: cpu flag; version level*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addCpu_specs_osST(
            osSTPtr,
            valStr,
            def_cpuVer_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: cpu flag; version level*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub01 Cat03:
   +   - cpu type level; any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-cpu-type"
      )
   ){ /*If: cpu flag; type level*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addCpu_specs_osST(
            osSTPtr,
            valStr,
            def_cpuType_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: cpu flag; type level*/

   /*****************************************************\
   * Fun18 Sec02 Sub02:
   *   - cpu flags; arm cpu flags
   *   o fun18 sec02 sub02 cat01:
   *     - arm 2 cpu input
   *   o fun18 sec02 sub02 cat02:
   *     - arm 3 cpu input
   *   o fun18 sec02 sub02 cat03:
   *     - arm 4 cpu input
   *   o fun18 sec02 sub02 cat04:
   *     - arm 5 cpu input
   *   o fun18 sec02 sub02 cat05:
   *     - arm 6 cpu input
   *   o fun18 sec02 sub02 cat06:
   *     - arm 7 cpu input
   *   o fun18 sec02 sub02 cat07:
   *     - arm 8 cpu input
   *   o fun18 sec02 sub02 cat08:
   *     - arm (no version) cpu input
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat01:
   +   - arm 2 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm2"
      )
   ){ /*If:arm2 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm2CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm2 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat02:
   +   - arm 3 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm3"
      )
   ){ /*If:arm3 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm3CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm3 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub04 Cat03:
   +   - arm 4 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm4"
      )
   ){ /*If:arm4 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm4CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm4 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub05 Cat04:
   +   - arm 5 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm5"
      )
   ){ /*If:arm5 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm5CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm5 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub06 Cat05:
   +   - arm 6 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm6"
      )
   ){ /*If:arm6 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm6CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm6 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub07 Cat06:
   +   - arm 7 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm7"
      )
   ){ /*If:arm7 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm7CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm7 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub08 Cat07:
   +   - arm 8 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm8"
      )
   ){ /*If:arm8 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_arm8CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm8 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat08:
   +   - arm (no version) cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm"
      )
   ){ /*If:arm cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_armCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm cpu input*/

   /*****************************************************\
   * Fun18 Sec02 Sub03:
   *   - cpu flags; non-arm cpu flags
   *   o fun18 sec02 sub01 cat01:
   *     - x86/amd64
   *   o fun18 sec02 sub01 cat02:
   *     - powerpc
   *   o fun18 sec02 sub01 cat03:
   *     - superh
   *   o fun18 sec02 sub01 cat04:
   *     - sparc
   *   o fun18 sec02 sub01 cat05:
   *     - mips
   *   o fun18 sec02 sub01 cat06:
   *     - spim (simulated mips)
   *   o fun18 sec02 sub01 cat07:
   *     - riscv
   *   o fun18 sec02 sub01 cat08:
   *     - m68k
   *   o fun18 sec02 sub01 cat09:
   *     - 68000 cpu (m68k)
   *   o fun18 sec02 sub01 cat10:
   *     - 68020 cpu (m68k)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat01:
   +   - x86/amd64 cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-x86"
      )
   ){ /*If:x86 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_x86CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: x86 cpu input*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-amd64"
      )
   ){ /*If:x86 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_x86CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: x86 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat02:
   +   - powerpc
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-powerpc"
      )
   ){ /*If:powerpc cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_powerpcCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: powerpc cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat03:
   +   - superh
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-superh"
      )
   ){ /*If:superh cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_superhCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: superh cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat04:
   +   - sparc
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sparc"
      )
   ){ /*If:sparc cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_sparcCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sparc cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat05:
   +   - mips
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-mips"
      )
   ){ /*If:mips cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_mipsCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: mips cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat06:
   +   - spim
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-spim"
      )
   ){ /*If:spim cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_spimCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: spim cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat07:
   +   - riscv
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-riscv"
      )
   ){ /*If:riscv cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_riscvCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: riscv cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat08:
   +   - m68k
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-m68k"
      )
   ){ /*If:m68k cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_m68kCPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: m68k cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat09:
   +   - 68000 (m68k)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-68000"
      )
   ){ /*If:68000 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_68000CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: 68000 cpu input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec02 Sub02 Cat10:
   +   - 68020 (m68k)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-68020"
      )
   ){ /*If:68020 cpu input*/
      if(
         addCpu_specs_osST(
            osSTPtr,
            glob_cpuAryStr[def_68020CPU_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: 68020 cpu input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - simd flags
   ^   o fun18 sec01 sub01:
   ^     - simd flags; any simd
   ^   o fun18 sec01 sub02:
   ^     - specific simd flags
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec03 Sub01:
   *   - simd flags; any simd
   *   o fun18 sec03 sub01 cat01:
   *     - no level any simd
   *   o fun18 sec03 sub01 cat02:
   *     - 0th level; any simd
   *   o fun18 sec03 sub01 cat03:
   *     - 1st level; any simd
   *   o fun18 sec03 sub01 cat04:
   *     - 2nd level; any simd
   *   o fun18 sec03 sub01 cat05:
   *     - 3rd level; any simd
   *   o fun18 sec03 sub01 cat06:
   *     - 4th level; any simd
   *   o fun18 sec03 sub01 cat07:
   *     - 5th level; any simd
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub01 Cat01:
   +   - no level any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd"
      )
   ){ /*If: simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            -1
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub01 Cat02:
   +   - 0th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-0"
      )
   ){ /*If: 0th level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_0thSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 0th level simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec13 Sub11 Cat03:
   +   - 1st level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-1"
      )
   ){ /*If: 1st level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_1stSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 1st level simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec13 Sub11 Cat04:
   +   - 2nd level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-2"
      )
   ){ /*If: 2nd level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_2ndSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 2nd level simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec13 Sub11 Cat05:
   +   - 3rd level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-3"
      )
   ){ /*If: 3rd level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_3rdSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 3rd level simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec13 Sub11 Cat06:
   +   - 4th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-4"
      )
   ){ /*If: 4th level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_4thSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 4th level simd flag*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec13 Sub11 Cat07:
   +   - 5th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-simd-5"
      )
   ){ /*If: 5th level simd flag*/
      if(! valStr)
         return def_badFlag_osST;

      if(
         addSIMD_specs_osST(
            osSTPtr,
            valStr,
            def_5thSIMD_osST
         )
      ) return def_memErr_osST;

      *argSIPtr += 2;
      return 0;
   } /*If: 5th level simd flag*/

   /*****************************************************\
   * Fun18 Sec03 Sub02:
   *   - specific simd flags
   *   o fun18 sec03 sub01 cat01:
   *     - avx512
   *   o fun18 sec03 sub01 cat02:
   *     - avx2
   *   o fun18 sec03 sub01 cat03:
   *     - sse4
   *   o fun18 sec03 sub01 cat04:
   *     - sse3
   *   o fun18 sec03 sub01 cat05:
   *     - sse2
   *   o fun18 sec03 sub01 cat06:
   *     - sse
   *   o fun18 sec03 sub01 cat07:
   *     - arm6
   *   o fun18 sec03 sub01 cat08:
   *     - neon32
   *   o fun18 sec03 sub01 cat09:
   *     - neon64
   *   o fun18 sec03 sub01 cat10:
   *     - neon
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat01:
   +   - avx512 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-avx512"
      )
   ){ /*If: avx512*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_avx512SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: avx512*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat02:
   +   - avx2 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-avx2"
      )
   ){ /*If: avx2*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_avx2SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: avx2*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat03:
   +   - sse4 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sse4"
      )
   ){ /*If: sse4*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_sse4SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sse4*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat04:
   +   - sse3 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sse3"
      )
   ){ /*If: sse3*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_sse3SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sse3*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat05:
   +   - sse2 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sse2"
      )
   ){ /*If: sse2*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_sse2SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sse2*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat06:
   +   - sse flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-sse"
      )
   ){ /*If: sse*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_sseSIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: sse*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat07:
   +   - arm6 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-arm6-simd"
      )
   ){ /*If: arm6*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_arm6SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: arm6*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat08:
   +   - neon32 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-neon32"
      )
   ){ /*If: neon32*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_neon32SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: neon32*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat09:
   +   - neon64 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-neon64"
      )
   ){ /*If: neon64*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_neon64SIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: neon64*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun18 Sec03 Sub02 Cat10:
   +   - neon flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-neon"
      )
   ){ /*If: neon*/
      if(
         addSIMD_specs_osST(
            osSTPtr,
            glob_simdAryStr[def_neonSIMD_osST],
            -1
         )
      ) return def_memErr_osST;

      ++(*argSIPtr);
      return 0;
   } /*If: neon*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - cpu bit type
   ^   o fun18 sec01 sub01:
   ^     - any bit
   ^   o fun18 sec01 sub02:
   ^     - 8 bit (bulid and everything else will fail here)
   ^   o fun18 sec01 sub03:
   ^     - 16 bit (bulid will probably fail here)
   ^   o fun18 sec01 sub04:
   ^     - 32 bit
   ^   o fun18 sec01 sub05:
   ^     - 64 bit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec04 Sub01:
   *   - any bit
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-bit"
      )
   ){ /*If:any cpu bit input*/
      if(! valStr)
         return def_badFlag_osST;

      tmpStr = valStr;

      tmpStr +=
         strToUS_base10str(
            valStr,
            &osSTPtr->osBitUS
         );

      if(*tmpStr != '\0')
        return def_badBit_osST;

      *argSIPtr += 2;
      return 0;
   } /*If:any cpu bit input*/

   /*****************************************************\
   * Fun18 Sec04 Sub02:
   *   - 8 bit (bulid and everything else will fail here)
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-8bit"
      )
   ){ /*If: 8bit*/
      osSTPtr->osBitUS = 8;
      ++(*argSIPtr);
      return 0;
   } /*If:  8bit*/

   /*****************************************************\
   * Fun18 Sec04 Sub03:
   *   - 16 bit (bulid will probably fail here)
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-16bit"
      )
   ){ /*If:16bit*/
      osSTPtr->osBitUS = 16;
      ++(*argSIPtr);
      return 0;
   } /*If: 16bit*/

   /*****************************************************\
   * Fun32 Sec04 Sub04:
   *   - 32 bit
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-32bit"
      )
   ){ /*If:32bit*/
      osSTPtr->osBitUS = 32;
      ++(*argSIPtr);
      return 0;
   } /*If: 32bit*/

   /*****************************************************\
   * Fun64 Sec04 Sub05:
   *   - 64 bit
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-64bit"
      )
   ){ /*If:64bit*/
      osSTPtr->osBitUS = 64;
      ++(*argSIPtr);
      return 0;
   } /*If: 64bit*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - add endin in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-big-endin"
      )
   ){ /*If: big endin cpu*/
      osSTPtr->endinUC = def_bigEndIn_osST;
      ++(*argSIPtr);
      return 0;
   } /*If: big endin cpu*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-little-endin"
      )
   ){ /*If: little endin cpu*/
      osSTPtr->endinUC = def_littleEndIn_osST;
      ++(*argSIPtr);
      return 0;
   } /*If: little endin cpu*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec06:
   ^   - blank stat flag
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-blank-os"
      )
   ){ /*If: blanking os flags*/
      if(osSTPtr->osAryStr[def_subOS_osST])
         osSTPtr->osAryStr[def_subOS_osST][0] = '\0';

      if(osSTPtr->osAryStr[def_groupOS_osST])
         osSTPtr->osAryStr[def_groupOS_osST][0] = '\0';

      ++(*argSIPtr);
      return 0;
   } /*If: blanking os flags*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-blank-cpu"
      )
   ){ /*If: blanking os flags*/
      if(osSTPtr->cpuAryStr[def_cpuVer_osST])
         osSTPtr->cpuAryStr[def_cpuVer_osST][0] = '\0';

      if(osSTPtr->cpuAryStr[def_cpuType_osST])
         osSTPtr->cpuAryStr[def_cpuType_osST][0] = '\0';

      ++(*argSIPtr);
      return 0;
   } /*If: blanking os flags*/

   if(
      ! eqlNull_ulCp(
         flagStr,
         (signed char *) "-blank-simd"
      )
   ){ /*If: blanking simd flags*/
      if(osSTPtr->simdAryStr[def_0thSIMD_osST])
         osSTPtr->simdAryStr[def_0thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_2ndSIMD_osST])
         osSTPtr->simdAryStr[def_2ndSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_3rdSIMD_osST])
         osSTPtr->simdAryStr[def_3rdSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_4thSIMD_osST])
         osSTPtr->simdAryStr[def_4thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_5thSIMD_osST])
         osSTPtr->simdAryStr[def_5thSIMD_osST][0] = '\0';

      if(osSTPtr->simdAryStr[def_1stSIMD_osST])
         osSTPtr->simdAryStr[def_1stSIMD_osST][0] = '\0';

      ++(*argSIPtr);
      return 0;
   } /*If: blanking simd flags*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec07:
   ^   - unkown flag
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*at this point no idea of flag value*/
   return def_badFlag_osST;
} /*addFlag_specs_osST*/

/*-------------------------------------------------------\
| Fun19: phelp_specs_osST
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - prints input part of help message of osST
   '   o fun19 sec01:
   '     - os part of help message
   '   o fun19 sec02:
   '     - cpu part of help  message
   '   o fun19 sec03:
   '     - simd flags
   '   o fun19 sec04:
   '     - cpu bit type
   '   o fun19 sec05:
   '     - blanking specs flags
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   fprintf(
      (FILE *) outFILE,
      "%sOS Specs:\n",
      indentStr
   ); /*catagory for input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - os part of help message
   ^   o fun19 sec01 sub01:
   ^     - os specific flags
   ^   o fun19 sec01 sub02:
   ^     - specific os input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec01 Sub01:
   *   - os flags; any OS
   *   o fun19 sec01 sub01 cat01:
   *     - no level any os
   *   o fun19 sec01 sub01 cat02:
   *     - variant level any os
   *   o fun19 sec01 sub01 cat03:
   *     - subgroup level any os
   *   o fun19 sec01 sub01 cat04:
   *     - group level any os
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sGeneral OS flags:\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub01 Cat01:
   +   - no level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-os name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   ); /*catagory for input*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so os to add\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub01 Cat02:
   +   - variant level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-os-var name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so os variant (openbsd/ubuntu) to add\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub01 Cat03:
   +   - subgroup level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-os-sub name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so os subgroup (ex linux/mac/bsd) to add\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub01 Cat04:
   +   - group level any os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-os-group name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so os group (ex unix/windows) to add\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun19 Sec01 Sub02:
   *   - specific os input
   *   o fun19 sec01 sub02 cat01:
   *     - droid (android) os flag
   *   o fun19 sec01 sub02 cat02:
   *     - linux os flag
   *   o fun19 sec01 sub02 cat03:
   *     - mac os flag
   *   o fun19 sec01 sub02 cat04:
   *     - openbsd os flag
   *   o fun19 sec01 sub02 cat05:
   *     - freebsd os flag
   *   o fun19 sec01 sub02 cat06:
   *     - netbsd os flag
   *   o fun19 sec01 sub02 cat07:
   *     - dragonflye bsd os flag
   *   o fun19 sec01 sub02 cat08:
   *     - bsd os flag
   *   o fun19 sec01 sub02 cat09:
   *     - unix os flag
   *   o fun19 sec01 sub02 cat10:
   *     - amiga os flag
   *   o fun19 sec01 sub02 cat11:
   *     - beOS os
   *   o fun19 sec01 sub02 cat12:
   *     - sun os
   *   o fun19 sec01 sub02 cat13:
   *     - morph os
   *   o fun19 sec01 sub02 cat14:
   *     - syllable os
   *   o fun19 sec01 sub02 cat15:
   *     - hurd os
   *   o fun19 sec01 sub02 cat16:
   *     - xinuos os
   *   o fun19 sec01 sub02 cat17:
   *     - plan9 os
   *   o fun19 sec01 sub02 cat18:
   *     - win (windows) os
   *   o fun19 sec01 sub02 cat19:
   *     - cygwin (windows lynx enviroment)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sOS flags (auto detect):\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat01:
   +   - droid (android) os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-droid: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use anroid os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat02:
   +   - linux os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-linux: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use linux os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat03:
   +   - mac os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-mac: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use mac os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat04:
   +   - openbsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-openbsd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use openbsd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat05:
   +   - freebsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-freebsd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use freebsd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat06:
   +   - netbsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-netbsd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use netbsd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat07:
   +   - dragonflyebsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-dragonflybsd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use dragonflye bsd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat08:
   +   - bsd os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-bsd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use bsd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat09:
   +   - unix os flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-unix: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use unix os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat10:
   +   - amiga os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-amiga: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use amiga os for build\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat11:
   +   - beOS os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-beOS: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use beOS (ex Haiku) os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat12:
   +   - sun os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sun: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use sun (ex illuminos) os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat13:
   +   - morph os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-morph: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use morph os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat14:
   +   - syllable os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-syllable: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use syllable os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat15:
   +   - hurd os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-hurd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use hurd os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat16:
   +   - xinuos os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-xinuos: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use xinuos os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat17:
   +   - plan9 os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-plan9: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use plan9 os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat18:
   +   - win os
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-win: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use windows os for specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec01 Sub02 Cat19:
   +   - cygwin (windows lynx enviroment)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-cygwin: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use cygiwn (unix windows enviroment)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - cpu flags
   ^   o fun19 sec01 sub01:
   ^     - cpu flags; any cpu
   ^   o fun19 sec01 sub02:
   ^     - arm cpu flags
   ^   o fun19 sec01 sub03:
   ^     - cpu flags; non-arm cpu flags
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec02 Sub01:
   *   - cpu flags; any cpu
   *   o fun19 sec02 sub01 cat01:
   *     - no level any cpu
   *   o fun19 sec02 sub01 cat02:
   *     - cpu version level; any cpu
   *   o fun19 sec02 sub01 cat03:
   *     - cpu type level; any cpu
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sGeneral CPU input:\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub01 Cat01:
   +   - no level any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-cpu name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so system cpu to build for\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub01 Cat02:
   +   - cpu version level; any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-cpu-ver name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so system cpu version to build for\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub01 Cat03:
   +   - cpu type level; any cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-cpu-type name: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so type of cpu to build for\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun19 Sec02 Sub02:
   *   - cpu flags; arm cpu flags
   *   o fun19 sec02 sub02 cat01:
   *     - arm 2 cpu input
   *   o fun19 sec02 sub02 cat02:
   *     - arm 3 cpu input
   *   o fun19 sec02 sub02 cat03:
   *     - arm 4 cpu input
   *   o fun19 sec02 sub02 cat04:
   *     - arm 5 cpu input
   *   o fun19 sec02 sub02 cat05:
   *     - arm 6 cpu input
   *   o fun19 sec02 sub02 cat06:
   *     - arm 7 cpu input
   *   o fun19 sec02 sub02 cat07:
   *     - arm 8 cpu input
   *   o fun19 sec02 sub02 cat08:
   *     - arm (no version) cpu input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sCPU input (auto reconginzed):\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat01:
   +   - arm 2 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm2: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM2\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat02:
   +   - arm 3 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm3: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM3\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec04 Sub04 Cat03:
   +   - arm 4 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm4: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM4\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec05 Sub05 Cat04:
   +   - arm 5 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm5: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM5\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec06 Sub06 Cat05:
   +   - arm 6 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm6: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM6 + sets ARM6 SIMD\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec07 Sub07 Cat06:
   +   - arm 7 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm7: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM7 + set neon32 (aarch32)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec08 Sub08 Cat07:
   +   - arm 8 cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm8: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM8 + set neon64 (aarch64)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat08:
   +   - arm (no version) cpu input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for ARM (no cpu version)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun19 Sec02 Sub03:
   *   - cpu flags; non-arm cpu flags
   *   o fun19 sec02 sub01 cat01:
   *     - x86/amd64
   *   o fun19 sec02 sub01 cat02:
   *     - powerpc
   *   o fun19 sec02 sub01 cat03:
   *     - superh
   *   o fun19 sec02 sub01 cat04:
   *     - sparc
   *   o fun19 sec02 sub01 cat05:
   *     - mips
   *   o fun19 sec02 sub01 cat06:
   *     - spim (simulated mips)
   *   o fun19 sec02 sub01 cat07:
   *     - riscv
   *   o fun19 sec02 sub01 cat08:
   *     - m68k
   *   o fun19 sec02 sub01 cat09:
   *     - 68000 cpu (m68k)
   *   o fun19 sec02 sub01 cat10:
   *     - 68020 cpu (m68k)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat01:
   +   - x86/amd64 cpu
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-x86: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for x86 or x86_64/amd64 cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat02:
   +   - powerpc
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-powerpc: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for powerpc cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat03:
   +   - superh
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-superh: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for superh cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat04:
   +   - sparc
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sparc: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for sparc cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat05:
   +   - mips
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-mips: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for mips cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat06:
   +   - spim
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-spim: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for spim (likely not work) cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat07:
   +   - riscv
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-riscv: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for riscv cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat08:
   +   - m68k
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-m68k: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for m68k (moterola 680X0) cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat09:
   +   - 68000 (m68k)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-68000: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for moterola 68000 (m68k) cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec02 Sub02 Cat10:
   +   - 68020 (m68k)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-68020: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so build for moterola 68020 (m68k) cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - simd flags
   ^   o fun19 sec01 sub01:
   ^     - simd flags; any simd
   ^   o fun19 sec01 sub02:
   ^     - specific simd flags
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec03 Sub01:
   *   - simd flags; any simd
   *   o fun19 sec03 sub01 cat01:
   *     - no level any simd
   *   o fun19 sec03 sub01 cat02:
   *     - 0th level; any simd
   *   o fun19 sec03 sub01 cat03:
   *     - 1st level; any simd
   *   o fun19 sec03 sub01 cat04:
   *     - 2nd level; any simd
   *   o fun19 sec03 sub01 cat05:
   *     - 3rd level; any simd
   *   o fun19 sec03 sub01 cat06:
   *     - 4th level; any simd
   *   o fun19 sec03 sub01 cat07:
   *     - 5th level; any simd
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sGeneral SIMD input:\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub01 Cat01:
   +   - no level any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so provide SIMD type (auto finds level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub01 Cat02:
   +   - 0th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-0: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so primary SIMD to target (0th level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec13 Sub11 Cat03:
   +   - 1st level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-1: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so secondary SIMD to target (1st level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec13 Sub11 Cat04:
   +   - 2nd level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-2: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so tertiary SIMD to target (2nd level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec13 Sub11 Cat05:
   +   - 3rd level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-3: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so quatenary SIMD to target (3rd level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec13 Sub11 Cat06:
   +   - 4th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-5: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so 5th SIMD type to target (4th level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec13 Sub11 Cat07:
   +   - 5th level; any simd
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-simd-6: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so 6th SIMD type to target (5th level)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun19 Sec03 Sub02:
   *   - specific simd flags
   *   o fun19 sec03 sub01 cat01:
   *     - avx512
   *   o fun19 sec03 sub01 cat02:
   *     - avx2
   *   o fun19 sec03 sub01 cat03:
   *     - sse4
   *   o fun19 sec03 sub01 cat04:
   *     - sse3
   *   o fun19 sec03 sub01 cat05:
   *     - sse2
   *   o fun19 sec03 sub01 cat06:
   *     - sse
   *   o fun19 sec03 sub01 cat07:
   *     - arm6
   *   o fun19 sec03 sub01 cat08:
   *     - neon32
   *   o fun19 sec03 sub01 cat09:
   *     - neon64
   *   o fun19 sec03 sub01 cat10:
   *     - neon
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%sSIMD input (auto detect):\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat01:
   +   - avx512 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-avx512: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use avx512 if possible, else avx2-sse\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat02:
   +   - avx2 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-avx2: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use avx2 if possible, else sse4-sse\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat03:
   +   - sse4 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sse4: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use sse4 if possible, else sse3-sse\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat04:
   +   - sse3 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sse3: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use sse3 if possible, else sse2/sse\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat05:
   +   - sse2 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sse2: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use sse2 if possible, else sse\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat06:
   +   - sse flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-sse: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use sse if possible, else scalar\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat07:
   +   - arm6 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-arm6-simd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use arm6 simd support\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat08:
   +   - neon32 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-neon32: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use neon with aarch32 for simd\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat09:
   +   - neon64 flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-neon64: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use neon with aarch64 for simd\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub02 Cat10:
   +   - neon flag
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-neon: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so use neon simd (no aarch specified)\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - cpu bit type
   ^   o fun19 sec01 sub01:
   ^     - any bit
   ^   o fun19 sec01 sub02:
   ^     - 16 bit (bulid will probably fail here)
   ^   o fun19 sec01 sub03:
   ^     - 32 bit
   ^   o fun19 sec01 sub04:
   ^     - 64 bit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "%s%sCPU bit type:\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   /*****************************************************\
   * Fun19 Sec04 Sub01:
   *   - any bit
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-bit number: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so set cpu bit type to a number\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun19 Sec04 Sub02:
   *   - 16 bit (bulid will probably fail here)
   \*****************************************************/

   /*not offically regonizing, but behind scene*/

   /*****************************************************\
   * Fun32 Sec04 Sub03:
   *   - 32 bit
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-32bit: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so set build for 32 bit cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*****************************************************\
   * Fun64 Sec04 Sub04:
   *   - 64 bit
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-64bit: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so set build for 64 bit cpus\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec05:
   ^   - cpu endin
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-big-endin: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so set CPU type to big endin cpu\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-little-endin: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so set CPU type to little endin cpu\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec06:
   ^   - blanking specs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "%s%sBlank specs (does not remove all flags):\n",
      indentStr,
      indentStr
   ); /*catagory for input*/

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-blank-os: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so remove all detected os specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-blank-cpu: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so remove all detected cpu specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s-blank-simd: [Optional]\n",
      indentStr,
      indentStr,
      indentStr
   );

   fprintf(
      (FILE *) outFILE,
      "%s%s%s%so remove all detected simd specs\n",
      indentStr,
      indentStr,
      indentStr,
      indentStr
   );
   /*at this point no idea of flag value*/
   return def_badFlag_osST;
} /*phelp_specs_osST*/
