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
|   - included libraries and OS types
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - list of known OSs
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

#include "../genLib/ulCp.h"


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - list of known OSs
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/********************************************************\
* Header Sec02 Sub01:
*   - OS list
\********************************************************/

signed char
   *osAryStr[] =
   {
      (schar *) "all",
      (schar *) "unix",

      (schar *) "linux",
      (schar *) "andriod",

      (schar *) "mac",
      (schar *) "bsd",

      (schar *) "openBSD",
      (schar *) "freeBSD",
      (schar *) "netBSD",
      (schar *) "dragonflyeBSD",

      (schar *) "amiga",
      (schar *) "beOS",  /*should include haiku*/
      (schar *) "sun",   /*includes illuminos/oracle*/
      (schar *) "morph",
      (schar *) "syllable",
      (schar *) "hurd",  /*gnu hurd*/
      (schar *) "xinuos",

      (schar *) "plan9",

      (schar *) "win"      /*windows*/
      (schar *) "cygwin"   /*windows cygwin enviorment*/
   }; /*os name list*/

#define def_numOS_osDetect 20

#define def_allIndex_osDetect 0
#define def_unixIndex_osDetect 1

#define def_linuxIndex_osDetect 2
#define def_androidIndex_osDetect 4

#define def_macIndex_osDetect 5

#define def_bsdIndex_osDetect 5
#define def_openBsdIndex_osDetect 6
#define def_freeBsdIndex_osDetect 7
#define def_netBsdIndex_osDetect 8
#define def_dragonflyeBsdIndex_osDetect 9

#define def_amigaIndex_osDetect 10
#define def_beOsIndex_osDetect 11
#define def_sunIndex_osDetect 12
#define def_morphIndex_osDetect 13
#define def_syllableIndex_osDetect 14
#define def_hurdIndex_osDetect 15
#define def_xinuosIndex_osDetect 16

#define def_plan9Index_osDetect 17

#define def_winIndex_osDetect 18
#define def_cygwinIndex_osDetect 19

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
){
   switch(sizeof(unsigned long) >> 1)
   { /*Switch: find OS bit by unsigned long size*/
         return def_8bit_osDetect;
      case 1:
         return def_16bit_osDetect;
      case 2:
         return def_32bit_osDetect;
      case 3:
         return 0; /*no id on OS bit size*/
      case 4:
         return def_64bit_osDetect;
   } /*Switch: find OS bit by unsigned long size*/

   return 0;
} /*getOS_osDetect*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - checks for compiled OS using OS macros (non-user
   '     supplied)
   '   o fun02 sec02:
   '     - check for linux OS
   '   o fun02 sec03:
   '     - check for Mac OS
   '   o fun02 sec04:
   '     - check for BSDs
   '   o fun02 sec05:
   '     - check for off wall unix or unix like or odd os
   '   o fun02 sec06:
   '     - check for UNIX (non Mac/Linux/BSD)
   '       - does not always work (not posix)
   '   o fun02 sec07:
   '     - check for plan9
   '   o fun02 sec08:
   '     - check for windows
   '   o fun02 sec09:
   '     - else unkown OS
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - check for linux OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(__ANDROID__)
   { /*If: android linux*/
      cpLine_ulCp(
         osAryStr->osAryStr[1],
         osAryStr[def_androidIndex_osDetect]
      ); /*add linux OS to list*/

      goto buildLinux_fun02_sec02:
   } /*If: android linux*/

   else if(__linux)
      goto buildLinux_fun02_sec02:
   
   else if(linux__)
      goto buildLinux_fun02_sec02:
   
   else if(linux)
      goto buildLinux_fun02_sec02:
   
   else if(__gnu_linux__)
   { /*Else If: final linux check*/
      buildLinux_fun02_sec02:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_linuxIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_unixIndex_osDetect]
      ); /*add unix OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: final linux check*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - check for Mac OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(__Macintosh__)
      goto buildMac_fun02_sec03;

   else if(__macintosh__)
      goto buildMac_fun02_sec03;

   else if(__APPLE__)
      goto buildMac_fun02_sec03;

   else if(__MACH__)
   { /*Else If: final mac check*/
      buildMac_fun02_sec03:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_macIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_unixIndex_osDetect]
      ); /*add unix OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: final mac check*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - check for BSDs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(__OpenBSD__)
   { /*Else If: openBSD*/
      cpLine_ulCp(
         osAryStr->osAryStr[1],
         osAryStr[def_openBsdIndex_osDetect]
      ); /*add linux OS to list*/

      goto buildBsd_fun02_sec04;
   } /*Else If: openBSD*/
   
   else if(__FreeBSD__)
   { /*Else If: freeBSD*/
      cpLine_ulCp(
         osAryStr->osAryStr[1],
         osAryStr[def_freeBsdIndex_osDetect]
      ); /*add linux OS to list*/

      goto buildBsd_fun02_sec04;
   } /*Else If: freeBSD*/
   
   else if(__NetBSD__)
   { /*Else If: netBSD*/
      cpLine_ulCp(
         osAryStr->osAryStr[1],
         osAryStr[def_netBsdIndex_osDetect]
      ); /*add linux OS to list*/

      goto buildBsd_fun02_sec04;
   } /*Else If: netBSD*/
   
   else if(__DragonFlye__)
   { /*Else If: dragonflyeBSD*/
      cpLine_ulCp(
         osAryStr->osAryStr[1],
         osAryStr[def_dragonflyeBsdIndex_osDetect]
      ); /*add linux OS to list*/

      goto buildBsd_fun02_sec04;
   } /*Else If: dragonflyeBSD*/

   else if(__bsdi__)
   { /*Else If: just bsd*/
      buildBsd_fun02_sec04:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_bsdIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_unixIndex_osDetect]
      ); /*add unix OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: just bsd*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - check for off wall unix or unix like or odd os
   ^   o fun02 sec05 sub01:
   ^     - amiga os
   ^   o fun02 sec05 sub02:
   ^     - BeOS (haiku)
   ^   o fun02 sec05 sub03:
   ^     - gnu hurd (maybe one of these days)
   ^   o fun02 sec05 sub04:
   ^     - morph os
   ^   o fun02 sec05 sub05:
   ^     - sun os (illuminos)
   ^   o fun02 sec05 sub06:
   ^     - syllable (author of syllable plans to return)
   ^   o fun02 sec05 sub07:
   ^     - xinous (sever OS)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I have no plan to code specific stuff for these OS's
   `  this is mainly here for future users
   */

   /*****************************************************\
   * Fun02 Sec05 Sub01:
   *   - amiga os
   \*****************************************************/

   else if(AMIGA)
      goto buildAmiga_fun02_sec05_sub01;

   else if(__amigaos__)
   { /*Else If: amiga OS*/
      buildAmiga_fun02_sec05_sub01:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_amigaIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: amiga OS*/

   /*****************************************************\
   * Fun02 Sec05 Sub02:
   *   - BeOS (haiku)
   \*****************************************************/

   else if(__BEOS__)
   { /*Else If: BeOS OS*/
      buildAmiga_fun02_sec05_sub01:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_beOsIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: BeOS OS*/

   /*****************************************************\
   * Fun02 Sec05 Sub03:
   *   - gnu hurd (maybe one of these days)
   \*****************************************************/

   else if(__GNU__)
      goto buildHurd_fun02_sec05_sub03;

   else if(__gnu_hurd__)
   { /*Else If: gnu hurd*/
      buildHurd_fun02_sec05_sub03:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_hurdIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: gnu hurd*/

   /*****************************************************\
   * Fun02 Sec05 Sub04:
   *   - morph os
   \*****************************************************/

   else if(__MORPHOS__)
   { /*Else If: morph OS*/
      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_morphIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: morph OS*/

   /*****************************************************\
   * Fun02 Sec05 Sub05:
   *   - sun os (illuminos)
   \*****************************************************/

   else if(sun)
      goto buildSun_fun02_sec05_sub05;

   else if(__sun)
   { /*Else If: sun (illuminos) OS*/
      buildSun_fun02_sec05_sub05:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_sunIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: sun (illuminos) OS*/

   /*****************************************************\
   * Fun02 Sec05 Sub06:
   *   - syllable (author of syllable plans to return)
   \*****************************************************/

   else if(__SYLLABLE__)
   { /*Else If: syllable os*/
      buildSun_fun02_sec05_sub05:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_syllableIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: syllable os*/

   /*****************************************************\
   * Fun02 Sec05 Sub07:
   *   - xinous (sever OS)
   \*****************************************************/

   else if(sco)
      goto buildXinuous_fun02_sec05_sub05;

   else if(_UNIXWARE7)
   { /*Else If: syllable os*/
      buildXinuous_fun02_sec05_sub05:;

      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_xinuosIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: syllable os*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec06:
   ^   - check for UNIX (non Mac/Linux/BSD)
   ^   - does not always work (not posix)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(__unix__)
      goto buildUnix_fun02_sec06;

   else if(__unix)
   { /*Else If: unix only*/
      buildUnix_fun02_sec06:;

      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_unixIndex_osDetect]
      ); /*add unix OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: unix only*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec07:
   ^   - check for plan9
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(EPLAN9)
   { /*Else If: plan9*/
      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_plan9Index_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: plan9*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec08:
   ^   - check for windows
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else if(__CYGWIN__)
   { /*Else If: cygwin enviorment in windows*/
      cpLine_ulCp(
         osAryStr->osAryStr[2],
         osAryStr[def_cygwinIndex_osDetect]
      ); /*add linux OS to list*/
      
      goto buildWin_fun02_sec08;
   } /*Else If: cygwin enviorment in windows*/

   else if(_WIN16)
      goto buildWin_fun02_sec08;

   else if(_WIN32)
      goto buildWin_fun02_sec08;

   else if(__WIN32__)
      goto buildWin_fun02_sec08;

   else if(_WIN64)
      goto buildWin_fun02_sec08;

   else if(__TOS_WIN__)
      goto buildWin_fun02_sec08;

   else if(__WINDOWS__)
   { /*Else If: windows*/
      buildWin_fun02_sec08:;

      cpLine_ulCp(
         osAryStr->osAryStr[3],
         osAryStr[def_winIndex_osDetect]
      ); /*add linux OS to list*/

      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else If: windows*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec09:
   ^   - else unkown OS
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
   { /*Else: no idea; default case only*/
      cpLine_ulCp(
         osAryStr->osAryStr[4],
         osAryStr[def_allIndex_osDetect]
      ); /*add default OS to list*/
   } /*Else: no idea; default case only*/
} /*getOS_osDetect*/
