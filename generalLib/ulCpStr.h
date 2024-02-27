/*########################################################
# Name: ulCpStr
#   - Copies contents of string 1 to string two using
#     unsigned longs (poor mans vectors)
#   - These functions are slower than strcpy and strlen,
#     but they allow deliminators to be used.
# Libraries:
#   - "dataTypeShortHand.h"
#   - "genMath.h"
# C Standard Libraries:
#   - <limits.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
|  o header:
|    - Included libraries, definitions and preprocessor
|      checks
|  o fun-01: ulCpStr
|    - Copies cpStr into dupStr using unsigned longs
|  o fun-02: ulCpMakeDelim
|    - Makes an unsigned long delimintor from a character
|      deliminator for use in cpStrDelim
|  o fun-03: ulIfDelim
|    - Detect if the input deliminator is in the input
|      unsigned long
|    - This is a bit slow (9 op), 8 if delimUL can be
|      deterimined at compile time. This is less efficent
|      on 32 and 16 bit cpus (no longer borderline).
|  o fun-06: ulVectCpStrDelim TODO:
|    - ulCpStrDelim, but can be compiled with vectors
|  o fun-06: cCpStr
|   - Copies cpStr into dupStr using characters
|  o fun-07: cCpStrDelim
|    - Copies cpStr into dupStr until delimC is found
|  o fun-08: cLenStr
|    - Finds the length of a string using characters
|  o fun-0?: cStrEql
|    - Checks to see if two strings are equal
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, definitions and preprocessor
|     checks
\-------------------------------------------------------*/

#ifndef UNSINGED_LONG_COPY_STRING_H
#define UNSINGED_LONG_COPY_STRING_H

#include "dataTypeShortHand.h"
#include "genMath.h"
#include <limits.h> /*for ULONG_MAX*/

#define defBitsPerChar 8
#define defNullDelim 0

#if ULONG_MAX >= 0xffffffffffffffff
   #define DEFINED_OS_BIT 64
   #define defCharPerUL 8
   #define defShiftByUL 3
   #define defModByUL 0x7 /*Mod 8 is the last three bits*/
   #define defTabDelim 0x0909090909090909   /*9 or 0x9*/
   #define defCommaDelim 0x2c2c2c2c2c2c2c2c /*44 or 0x2c*/
   #define defNewLineDelim 0x0a0a0a0a0a0a0a0a /*0a 0x0a*/
   #define defHiBit 0x8080808080808080 /*For deliminator*/
   #define defOne 0x0101010101010101 /*For if delim*/

#elif ULONG_MAX >= 0xffffffff
   #define DEFINED_OS_BIT 32
   #define defCharPerUL 4
   #define defShiftByUL 2
   #define defModByUL 0x3 /*Mod 4 is the last two bits*/
   #define defTabDelim 0x09090909      /*9 or 0x9*/
   #define defCommaDelim 0x2c2c2c2c    /*44 or 0x2c*/
   #define defNewLineDelim 0x0a0a0a0a  /*0a or 0x0a*/
   #define defHiBit 0x80808080 /*For deliminator*/
   #define defOne 0x01010101 /*For if delim*/

#elif ULONG_MAX >= 0xffff
   #define DEFINED_OS_BIT 16
   #define defCharPerUL 2
   #define defShiftByUL 1 /*Finds number of elements*/
   #define defModByUL 0x1 /*Mod 2 is the last bit*/
   #define defTabDelim 0x0909    /*9 or 0x9*/
   #define defCommaDelim 0x2c2c  /*44 or 0x2c*/
   #define defNewLineDelim 0x0a0a  /*0a or 0x0a*/
   #define defOne 0x01010101 /*For if delim*/

#else
   #error System only supports 8 bit integers

#endif

/*-------------------------------------------------------\
| Fun-01: ulCpStr
|   - Copies cpStr into dupStr using unsigned longs
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - lenUI:
|     o length of cpStr
| Output:
|   - Modifies:
|     o  dupStr to hold lenUI characters from cpStr
\-------------------------------------------------------*/
#define ulCpStr(dupStr, cpStr, lenUI){\
   ulong *macCpUL = (ulong *) (cpStr);\
   ulong *macDupUL = (ulong *) (dupStr);\
   uint uiCharMac = 0;\
   \
   for(\
      uiCharMac= 0;\
      uiCharMac < (lenUI) >> defShiftByUL;\
      ++uiCharMac\
   ) macDupUL[uiCharMac] = macCpUL[uiCharMac];\
   \
   for(\
      uiCharMac = (lenUI) - ((lenUI) & defModByUL);\
      uiCharMac < (lenUI);\
     ++uiCharMac\
   ) (dupStr)[uiCharMac] = (cpStr)[uiCharMac];\
   \
   (dupStr)[lenUI] = '\0';\
   /*Faster than doing *val1++ = *val2++*/\
} /*ulCpStr*/

/*-------------------------------------------------------\
| Fun-02: ulCpMakeDelim
|   - Makes an unsigned long delimintor from a character
|     deliminator for use in cpStrDelim
| Input:
|   - delimC:
|     o Deliminator to use to end a string copy
| Output:
|   - Modifies:
|     o  dupStr to hold the characters from cpStr
\-------------------------------------------------------*/

#if DEFINED_OS_BIT == 64
   #define ulCpMakeDelim(delimC)({\
      ulong macDelimUL = (ulong) (delimC);\
      macDelimUL |= (macDelimUL << 32);\
      macDelimUL |= (macDelimUL << 16);\
      macDelimUL |= (macDelimUL << 8);\
      macDelimUL;\
   }) /*ulMakeDelim*/

#elif DEFINED_OS_BIT == 32
   #define ulCpMakeDelim(delimC)({\
      ulong macDelimUL = (ulong) (delimC);\
      macDelimUL |= (macDelimUL << 16);\
      macDelimUL |= (macDelimUL << 8);\
      macDelimUL;\
   }) /*ulMakeDelim*/

#elif DEFINED_OS_BIT == 16
   #define ulCpMakeDelim(delimC)({\
      ulong macDelimUL = (ulong) (delimC);\
      macDelimUL |= (macDelimUL << 8);\
      macDelimUL;\
   }) /*ulMakeDelim*/

#endif

/*-------------------------------------------------------\
| Fun-03: ulIfDelim
|   - Detect if the input deliminator is in the input
|     unsigned long
| Variations:
|   - SAFE_DELIM uses 9 to 8 OP for 64 bit, 7 to 6 OP for
|     for 32 bit, and 5 to 4 OP for 16 bit. The amount
|     depends on if the deliminator is hardcoded or not.
|   - ifndef SAFE_DELIM takes 5 op no matter what your
|     OS type is. You could reduce it down by removing the
|     (addUL & 1) + addUL step, but this does cause some
|     issues with values that are one off the actual. Tab
|     is safe, since it is odd, but not comma or space.
| Input:
|   - strUL:
|     o Unsigned long to check for the delimintor in
|   - delimUL:
|     o Unsinged long with delimintor (use ulMakeDelim
|       to get this value)
| Output:
|   - Returns:
|     o  unsigned long > 0; strUL has no deliminator. The
|        position of the delimintor can be found with
|        log2(returned ulong) >> 8. The 1 is shifted to
|        the left of the character.
|     o  unsigned long == 0; strUL has deliminator
\-------------------------------------------------------*/
#ifndef SAFE_DELIM
   #define ulIfDelim(strUL, delimUL)({\
      ulong macAddUL = ( (ulong) (strUL) ) ^ (delimUL);\
      ( ((macAddUL & 1) + macAddUL) -defOne ) & defHiBit;\
   })
   /*Logic:
   ` - addUL = strUL ^ delimUL:
   `   o Converts all values matching delimUL to 0, and
   `     everything else to > 0. Saves result to addUL
   ` - (addUL & 1) + addUL
   `   o Convets odd numbers to even numbers. This gets
   `     means there is no 1;
   `   o This is needed to ensure -1 will not cause an
   `     overflow when 1 is next to an deliminator. The
   `     & 1 ensures that -1 will alwas remove the sign
   `     bit, if one character was one off.
   ` - ((addUL & 1) + addUL) - defOne
   `   o subtracts one from every character. This results
   `     in an overflow for only 0, while every other
   `     value is >= 1, but can go to 0 if next number
   `     overflowed. This sets the value 0 numbers to -1
   ` - (( (addUL & 1) + addUL) - defOne) & defHiBit
   `   o This keeps only the overflow bit (sign bit). This
   `     means that every non-deliminator value becomes
   `     0.
   */

#elif DEFINED_OS_BIT == 64
   #define ulIfDelim(strUL, delimUL)({\
      ulong macRetUL = (strUL) ^ ~(delimUL);\
      \
      macRetUL &= ((macRetUL & 0xf0f0f0f0f0f0f0f0) >> 4);\
      macRetUL &= (macRetUL>> 2);\
      macRetUL & (macRetUL >> 1);\
      \
   }) /*ulIfDelim*/

#elif DEFINED_OS_BIT == 32
   #define ulIfDelim(strUL, delimUL)({\
      ulong macRetUL = (strUL) ^ ~(delimUL);\
      \
      macRetUL &= ((macRetUL & 0xf0f0f0f0) >> 2);\
      macRetUL & (macRetUL>> 1);\
      \
   }) /*ulIfDelim*/

#elif DEFINED_OS_BIT == 16
   #define ulIfDelim(strUL, delimUL)({\
      ulong macRetUL = (strUL) ^ ~(delimUL);\
      \
      macRetUL & ((macRetUL & 0xf0f0) >> 1);\
   }) /*ulIfDelim*/

#endif
   /*Logic: for 64 bit (32 and 16 bit are just shorter)
   `   o strUL ^ ~delimUL:
   `     - Converts extact matches to delimUL to -1, and
   `     - Converts non-extact mathces to delimUL to a
   `       mixture of 1's and 0's (at least one 0)
   `   o (retUL 0xf0f0...f0)
   `     - Clears (sets to 0) lower four bits in the
   `       temporary retUL. This prevents values from
   `       other characters merging when shifting.
   `     - 11111111 becomes 11110000
   `     - 10111110 00001011 becomes 10110000
   `   o ((retUL 0xf0f0...f0) >> 4)
   `     - Shifts the upper for bits in each charcter to
   `       to the lower four bits
   `     - 11110000 becomes 00001111
   `     - 10110000 becomes 10110000
   `   o retUL & ((retUL 0xf0f0...f0) >> 4)
   `     - Clears values were there were zeros. -1 is
   `       converted to 15, but everything else will have
   `       at least one extra bit cleared.
   `     - 11111111 & 00001111 becomes 00001111
   `     - 10111110 & 00001011 becomes 00001010
   `   o retUL & (retUL >> 2)
   `     - Clears values were there were zeros. 15 (-1) is
   `       convert to 3, but everything else will have at
   `       least one more bit cleared.
   `     - 00001111 & 00000011 becomes 00000011
   `     - 00001010 & 00000010 becomes 00000010
   `   o retUL & (retUL >> 1)
   `     - Clears values were there were zeros. 3 (-1) is
   `       converted to 1, but everything else will be 0
   `     - 00001111 & 00000001 becomes 00000001
   `     - 00000010 & 00000001 becomes 00000000
   */

/*-------------------------------------------------------\
| Fun-04: ulCpStrDelim
|   - Copies cpStr into dupStr until delimC is found
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - delimUL:
|     o Deliminatro to end at (as long). Use makeULDelim
|       to build this deliminator
| Output:
|   - Modifies:
|     o  dupStr to hold the characters from cpStr
| Note:
|   - This will likely not be very good at copying short
|     strings.
\-------------------------------------------------------*/
#define ulCpStrDelim(dupStr, cpStr, delimUL, delimC)({\
   ulong *macCpUL = (ulong *) (cpStr);\
   ulong *macDupUL = (ulong *) (dupStr);\
   char *dupMacStr = 0;\
   char *cpMacStr = 0;\
   ulong macAtDelimUL = 0;\
   uint uiCharMac = 0;\
   \
   macAtDelimUL =ulIfDelim(*macCpUL, (delimUL));\
   \
   while(!macAtDelimUL)\
   { /*Loop: Copy cpStr to dupStr*/\
      *macDupUL++ = *macCpUL++;\
      macAtDelimUL = ulIfDelim(*macCpUL, (delimUL));\
   } /*Loop: Copy cpStr to dupStr*/\
   \
   cpMacStr = (char *) macCpUL;\
   dupMacStr = (char *) macDupUL;\
   /*Loop: Till macAtDelimUL says there is no longer
   ` a deliminator. The last character in the string
   ' is at the end
   */\
   while(*cpMacStr != (delimC))\
      *dupMacStr++ = *cpMacStr++;\
   \
   uiCharMac = dupMacStr - (dupStr);\
   (dupStr)[uiCharMac] = '\0';\
   uiCharMac; /*the number of characters copied*/\
}) /*ulCpStrDelim*/

/*-------------------------------------------------------\
| Fun-05: ulLenStr
|   - Finds the length of a string using unsigned longs
| Input:
|   - inStr:
|     o C-string or string with deliminator to find length
|       of
|   - delimUL:
|     o Deliminator marking the end of the string
| Output:
|   - Returns:
|     o Number of characters in the string
\-------------------------------------------------------*/
#define ulLenStr(inStr, delimUL, delimC)({\
   ulong *macPtrUL = (ulong *) (inStr);\
   ulong macAtDelimUL = 0;\
   uint uiLenStrMac = 0;\
   \
   macAtDelimUL = ulIfDelim(*macPtrUL, (delimUL));\
   \
   while(!macAtDelimUL)\
   { /*Loop: Copy cpStr to dupStr*/\
      uiLenStrMac += 8;\
      ++macPtrUL;\
      macAtDelimUL = ulIfDelim(*macPtrUL, (delimUL));\
   } /*Loop: Copy cpStr to dupStr*/\
   \
   while((inStr)[uiLenStrMac] !=(delimC)) ++uiLenStrMac;\
   \
   uiLenStrMac;\
}) /*ulCpStrDelim*/


/*These are the single byte copy functions*/

/*-------------------------------------------------------\
| Fun-06: cCpStr
|   - Copies cpStr into dupStr using characters
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - lenUI:
|     o length of cpStr
| Output:
|   - Modifies:
|     o  dupStr to hold lenUI characters from cpStr
\-------------------------------------------------------*/
#define cCpStr(dupStr, cpStr, lenUI){\
   uint uiIterMac = 0;\
   for(uiIterMac = 0; uiIterMac < lenUI; ++uiIterMac)\
      (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
   (dupStr)[uiIterMac] = '\0';\
}

/*-------------------------------------------------------\
| Fun-07: cCpStrDelim
|   - Copies cpStr into dupStr until delimC is found
| Input:
|   - dupStr:
|     o Pointer to string to copy cpStr into
|   - cpStr:
|     o Pointer to string to copy
|   - delimUL:
|     o Deliminatro to end at (as long). Use makeULDelim
|       to build this deliminator
| Output:
|   - Modifies:
|     o  dupStr to hold the characters from cpStr
\-------------------------------------------------------*/
#define cCpStrDelim(dupStr, cpStr, delimC)({\
   char *dupMacStr = (dupStr);\
   char *cpMacStr = (cpStr);\
   \
   while(*(cpMacStr) != (delimC))\
      *dupMacStr++ = *cpMacStr++;\
   \
   *dupMacStr = '\0';\
   dupMacStr - (dupStr);\
})
/*The *dupStr++ = *cpStr++; is faster than incurmenting
` individually or by number
*/

/*-------------------------------------------------------\
| Fun-08: cLenStr
|   - Finds the length of a string using characters
| Input:
|   - inStr:
|     o C-string or string with deliminator to find length
|       of
|   - delimUL:
|     o Deliminator marking the end of the string
| Output:
|   - Returns:
|     o Number of characters in the string
\-------------------------------------------------------*/
#define cLenStr(inStr, delimC)({\
   char *inMacStr = (inStr);\
   while(*inMacStr != (delimC)) ++inMacStr;\
   inMacStr - (inStr);\
})

/*-------------------------------------------------------\
| Fun-0?: cStrEql
|   - Checks to see if two strings are equal
| Input:
|   - qryStr:
|     o The query c-string to compare against the
|       reference
|   - refStr:
|     o The reference c-string to compare against the
|       query
| Output:
|   - Returns:
|     o < 0 for qry less than reference
|     o 0 for qry is same as the reference
|     o >0 for qry is greater than the reference
\-------------------------------------------------------*/
#define cStrEql(qryStr, refStr, delimC)({\
   int iCntMac = 0;\
   \
   /*This is faster than incurmenting qryMacStr and
   ` refMacStr separately. Doing *val++ == *val2++ does
   ` not work here.
   */\
   while((qryStr)[iCntMac] == (refStr)[iCntMac])\
   { /*Looop*/\
      if((qryStr)[iCntMac] == (delimC)) break;\
      ++iCntMac;\
   } /*Looop*/\
   \
   (qryStr)[iCntMac] - (refStr)[iCntMac];\
   \
   /*this was removed because it caused different behavior
   ` them strcmp
   */\
   /*(-(*(qryStr) !=(delimC))) &(*(qryStr) -*(refStr));*/\
   /*Logic (for old code):
   `  - qryStr != delimC:
   `    o 0 if I have the delimintaor (match)
   `    o 1 if I do not have the delimintaor (no match)
   `  - -(qryStr != delimC):
   `    o Converts 1 to -1 (all bits set) and 0 to 0, no
   `      bits set
   `  - qryStr - refStr
   `    o 0 if the stings were equal
   `    o < 0 if qryStr is less than refStr
   `    o > 0 if qryStr is greater than refStr
   `  - (-(qryStr != delimC)) & (qryStr - refStr)
   `    o is (-1 & number) = number, if qryStr was not on
   `      the deliminator
   `    o is (0 & number) = 0, if qryStr was on the
   `      deliminator
   */\
})

#endif
