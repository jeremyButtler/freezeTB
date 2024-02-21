/*########################################################
# Name: ulCpStr
#   - Copies contents of string 1 to string two using
#     unsigned longs (poor mans vectors)
# Libraries:
#   - "dataTypeShortHand.h"
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
|  o fun-04: ulCpStrDelim
|    - Copies cpStr into dupStr until delimC is found
|  o fun-05: ulVectCpStr TODO:
|    - ulCpStr, but can be compiled with vectors
|  o fun-06: ulVectCpStrDelim TODO:
|    - ulCpStrDelim, but can be compiled with vectors
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries, definitions and preprocessor
|     checks
\-------------------------------------------------------*/

#ifndef UL_CP_STR_H
#define UL_CP_STR_H

#include "dataTypeShortHand.h"
#include "genMath.h"
#include <limits.h> /*for ULONG_MAX*/

/*Using two lines to avoid line overflows*/
#if defined SSE2 || SSE4 || defined AVX2 || defined AVX512
   #define defUseVects
   #include "../../alnSeq/vectWrap/vectWrap.h"

#elif defined NEON || defined NEON64
   #define defUseVects
   #include "../../alnSeq/vectWrap/vectWrap.h"

#else
   typedef unsigned long VI8; /*when not using vectors*/
#endif

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

#elif ULONG_MAX >= 0xffffffff
   #define DEFINED_OS_BIT 32
   #define defCharPerUL 4
   #define defShiftByUL 2
   #define defModByUL 0x3 /*Mod 4 is the last two bits*/
   #define defTabDelim 0x09090909      /*9 or 0x9*/
   #define defCommaDelim 0x2c2c2c2c    /*44 or 0x2c*/
   #define defNewLineDelim 0x0a0a0a0a  /*0a or 0x0a*/

#elif ULONG_MAX >= 0xffff
   #define DEFINED_OS_BIT 16
   #define defCharPerUL 2
   #define defShiftByUL 1 /*Finds number of elements*/
   #define defModByUL 0x1 /*Mod 2 is the last bit*/
   #define defTabDelim 0x0909    /*9 or 0x9*/
   #define defCommaDelim 0x2c2c  /*44 or 0x2c*/
   #define defNewLineDelim 0x0a0a  /*0a or 0x0a*/

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
|   - This is a bit slow (9 op), 8 if delimUL is hardcoded
|     This goes to worse ratios at 32 and 16 bit cpus
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
#if DEFINED_OS_BIT == 64
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
|     strings. I also pay a steep cost (11 op) for the
|     if check. And I still have to pay a bit extra cost
|     for the end of the buffer, which might not be a
|     multiple of 8. However, I can copy 8 characters at
|     once for the main part. I am hoping that pays off.
\-------------------------------------------------------*/
#define ulCpStrDelim(dupStr, cpStr, delimUL)({\
   ulong *macCpUL = (ulong *) (cpStr);\
   ulong *macDupUL = (ulong *) (dupStr);\
   ulong macAtDelimUL = 0;\
   uint uiCharMac = 0;\
   \
   macAtDelimUL =ulIfDelim(macCpUL[uiCharMac],(delimUL));\
   \
   for(\
     macAtDelimUL = macAtDelimUL;\
     !macAtDelimUL;\
     macAtDelimUL =\
        ulIfDelim(macCpUL[uiCharMac], (delimUL))\
   ){ /*Loop: Copy cpStr to dupStr*/\
      macDupUL[uiCharMac] = macCpUL[uiCharMac];\
      ++uiCharMac;\
   } /*Loop: Copy cpStr to dupStr*/\
   \
   if(!((char) macAtDelimUL))\
   { /*If: cpStr's length is not a multipe of 8*/\
      uiCharMac <<= defShiftByUL; /*for long index*/\
      \
      /*Loop: Till macAtDelimUL says there is no longer
      ` a deliminator. The last character in the string
      ' is at the end
      */\
      for(\
         macAtDelimUL = macAtDelimUL;\
         !((char) macAtDelimUL);\
         macAtDelimUL = macAtDelimUL >> defBitsPerChar\
      ){ /*Loop: Finish copying cpStr*/\
         (dupStr)[uiCharMac] = (cpStr)[uiCharMac];\
         ++uiCharMac;\
      } /*Loop: Finish copying cpStr*/\
   } /*If: cpStr's length is not a multipe of 8*/\
   \
   else \
   { /*Else: length is multipe of 8*/\
      macDupUL[uiCharMac] = macCpUL[uiCharMac];\
      uiCharMac <<= defShiftByUL;\
   } /*Else: length is multipe of 8*/\
   \
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
#define ulLenStr(inStr, delimUL)({\
   ulong *macPtrUL = (ulong *) (inStr);\
   ulong macAtDelimUL = 0;\
   uint uiLenStrMac = 0;\
   \
   macAtDelimUL =\
         ulIfDelim(macPtrUL[uiLenStrMac], (delimUL));\
   \
   for(\
      macAtDelimUL = macAtDelimUL;\
      !macAtDelimUL;\
      macAtDelimUL = \
         ulIfDelim(macPtrUL[uiLenStrMac], (delimUL))\
   ) ++uiLenStrMac;\
   \
   uiLenStrMac <<= defShiftByUL;\
   \
   /*Find out how much I overshot by. The last character
   ' in the string is closest to the end.
   */\
   for(\
      macAtDelimUL = macAtDelimUL;\
      !((char) macAtDelimUL);\
      macAtDelimUL = macAtDelimUL >> defBitsPerChar\
   ) ++uiLenStrMac;\
   \
   uiLenStrMac; /*the number of characters copied*/\
}) /*ulCpStrDelim*/

/*-------------------------------------------------------\
| Fun-06: VI8CpStr
|   - Copies an input number of elements from one c-string
|     to another c-string using vectors if complied with
|     vectors, else ulCpStr()
| Input:
|   - cpStr:
|     o C-string to copy
|   - dupStr:
|     o c-string to copy to
|   - lenUI:
|     o Number of characters to copy
| Output:
|   - Modifies:
|     o dupStr to hold lenUI characters from cpStr
\-------------------------------------------------------*/
#ifdef defUseVects
   /*#define VI8CpStr(dupStr, cpStr, lenUI){\*/
   static void VI8CpStr(char *dupStr, char *cpStr, uint lenUI){
      uint macCpOffUL=
            noBranchAB(
              (char *) int8AlnPointer((cpStr)) - (cpStr)
            );
      uint macDupOffUL =
            noBranchAB(
              (char *) int8AlnPointer((dupStr)) - (dupStr)
            );
      
      VI8 macVI8;
      uint iIterMac = 0;
      
      if(macCpOffUL != macDupOffUL)
      { /*If: I have to do unaligned vectors*/
         for(
          iIterMac = 0; 
          iIterMac + defNum8BitElms < (lenUI);
          iIterMac += defNum8BitElms
         ){ /*Loop: copy with vectors*/
            macVI8 =
               loadu_I8Ary_retVI8(&(cpStr)[iIterMac]);
            
            storeu_VI8_retAryI8(
               &(dupStr)[iIterMac],
               macVI8
            );
         } /*Loop: copy with vectors*/
      } /*If: I have to do unaligned vectors*/
      
      else
      { /*Else: the strings have the same alignment*/
         for(
            iIterMac = 0;
            iIterMac < (lenUI);
            ++iIterMac
         ) (dupStr)[iIterMac] = (cpStr)[iIterMac];
         
         for(
          iIterMac = iIterMac;
          iIterMac + defNum8BitElms < (lenUI);
          iIterMac += defNum8BitElms
         ){ /*Loop: copy with vectors*/
            macVI8 =
               load_I8Ary_retVI8(&(cpStr)[iIterMac]);
            
            store_VI8_retAryI8(
               &(dupStr)[iIterMac],
               macVI8
             );
         } /*Loop: copy with vectors*/
      } /*Else: the strings have the same alignment*/
      
      iIterMac -= defNum8BitElms;
      
      for(iIterMac=iIterMac; iIterMac < lenUI;++iIterMac)
         (dupStr)[iIterMac] = (cpStr)[iIterMac];
   } /*VI8CpStr*/

#elif defined NO_UL_CP
   #define VI8CpStr(cpStr, dupStr, lenUI){\
      uint uiIterMac = 0;\
      for(uiIterMac = 0; uiIterMac < lenUI; ++uiIterMac)\
         (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
   }

#else
   #define VI8CpStr(cpStr, dupStr, lenUI)\
      {ulCpStr((cpStr), (dupStr), (lenUI))}
#endif

/*-------------------------------------------------------\
| Fun-07: VI8CpMakeDelim
|   - Makes an vector of deliminators for the input
|     deliminator or if not using vectors calls
|     ulCpMakeDelim
| Input:
|   - delimC:
|     o A Deliminator (char) to use to end a string copy
| Output:
|   - Returns:
|     o  A VI8 type with the deliminators
\-------------------------------------------------------*/
#ifdef defUseVects
  #define VI8CpMakeDelim(delimC)(set1_I8_retVI8((delimC)))
#else
  #define VI8CpMakeDelim(delimC)\
    ((VI8) ulCpMakeDelim((delimC)))
#endif

/*-------------------------------------------------------\
| Fun-08: VI8IfDelim
|   - Detect if the input deliminator is in the input
|     unsigned long
| Input:
|   - strVI8:
|     o A VI8 type to do an if equal check on
|   - delimVI8:
|     o An VI8 type set to the correct deliminator. Use
|       VI8CpMakeDelim to get this.
| Output:
|   - Returns:
|     o  unsigned long > 0; strUL has no deliminator.
|     o  unsigned long == 0; strUL has deliminator
\-------------------------------------------------------*/
#ifdef defUseVects
   #define VI8IfDelim(strVI8, delimVI8)(\
      store_mask8_retUL(\
         cmpeq_VI8_retMask8((strVI8), (delimVI8))\
      )\
   ) /*VI8IfDelim*/

#else
   #define VI8IfDelim(strVI8, delimVI8)\
      (ulIfDelim((strVI8), (delimVI8)))

#endif

/*-------------------------------------------------------\
| Fun-09: VI8CpStrDelim
|   - Copy from on string to another until a deliminator
|     is found. This can use vectors if specified during
|     compile time or ulCpStrDelim if vectors were not
|     specified
| Input:
|   - cpStr:
|     o C-string to copy
|   - dupStr:
|     o C-string to copy to
|   - delimVI8:
|     o A VI8 type holding a deliminator. You can make
|       this with VI8CpMakeDelim.
|   - delimC:
|     o The delimintor ued to make delimVI8
| Output:
|   - Modifies:
|     o dupStr to hold lenUI characters from cpStr
|   - Returns:
|     o Number of characters copied
\-------------------------------------------------------*/
#ifdef defUseVects
   #define VI8CpStrDelim(cpStr,dupStr,delimVI8,delimC)({\
      \
      uint macCpOffUL =\
         (char *) int8AlnPointer((cpStr)) - (cpStr);\
      uint macDupOffUL =\
         (char *_ int8AlnPointer((dupStr)) - (dupStr);\
      \
      VI8 macVI8;\
      uint uiLenStrMac = 0;\
      uint uiIterMac = 0;\
      \
      if(macCpOffUL != macDupOffUL)\
      { /*If: I have to do unaligned vectors*/\
            macVI8 = loadu_I8Ary_retVI8((cpStr));\
            \
            for(\
               macVI8 =\
                  load_I8Ary_retVI8(&(inStr)[uiLenMac]);\
               !VI8IfDelim(macVI8, (delimVI8));\
               macVI8=\
                  load_I8Ary_retVI8(&(inStr)[uiLenMac])\
            ){ /*Loop: copy with vectors*/\
               uiLenMac += defNum8BitElms;\
               \
               storeu_VI8_retAryI8(\
                  &(dupStr)[uiIterMac],\
                  macVI8\
               );\
            } /*Loop: copy with vectors*/\
      } /*If: I have to do unaligned vectors*/\
      \
      else\
      { /*Else: the strings have the same alignment*/\
         for(\
            uiIterMac = 0;\
            uiIterMac < macCpOffUL;\
            ++uiIterMac\
         ){ /*Loop: Copy unaligned parts*/\
            if((inStr)[uiIterMac] == (delimC)) break;\
            (dupStr)[uiIterMac] = (inStr)[uiIterMac]\
         } /*Loop: Copy unaligned parts*/\
         \
         if((inStr)[uiIterMac] != (delimC))\
         { /*If: I have more string to find*/\
            for(\
               macVI8 =\
                  load_I8Ary_retVI8(&(inStr)[uiLenMac]);\
               !VI8IfDelim(macVI8, (delimVI8));\
               macVI8=\
                  load_I8Ary_retVI8(&(inStr)[uiLenMac])\
            ){ /*Loop: copy with vectors*/\
               uiLenMac += defNum8BitElms\
               \
               store_VI8_retAryI8(\
                  &(dupStr)[uiIterMac],\
                  macVI8\
               );\
            } /*Loop: copy with vectors*/\
         } /*If: I have more string to find*/\
      } /*Else: the strings have the same alignment*/\
      \
      for(\
         uiIterMac = uiIterMac;\
         (cpStr)[uiIterMac] != (delimC);\
         ++uiIterMac\
      ) (dupStr)[uiIterMac]=(cpStr)[uiIterMac];\
      \
      uiIterMac;\
   }) /*VI8CpStrDelim*/

#elif defined NO_UL_CP
   #define VI8CpStrDelim(dupStr,cpStr,delimVI8,delimC)({\
      uint uiIterMac = 0;\
      for(\
         uiIterMac = 0;\
         (cpStr)[uiIterMac] != (delimC);\
         ++uiIterMac\
      )\
         (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
      \
      uiIterMac;\
   })

#else
   #define VI8CpStrDelim(dupStr,cpStr,delimVI8,delimC)({\
      (ulCpStrDelim((cpStr), (dupStr), (delimVI8)))
#endif


/*-------------------------------------------------------\
| Fun-08: VI8LenStr
|   - Finds the length of a string using vectors, if
|     specified or ulLenStr if vectors were not specified
| Input:
|   - inStr:
|     o C-string or string with deliminator to find length
|       of
|   - delimVI8:
|     o A VI8 type holding a deliminator. You can make
|       this with VI8CpMakeDelim.
|   - delimC:
|     o The delimintor ued to make delimVI8
| Output:
|   - Returns:
|     o Number of characters in the string
\-------------------------------------------------------*/
#ifdef defUseVects
   #define VI8LenStr(inStr, delimVI8, delimC)({\
      uint macCpOffUL=\
        (char *) int8AlnPointer((inStr)) - (inStr);\
      VI8 macVI8;\
      uint uiLenMac = 0;\
      \
      for(\
         uiLenMac = 0;\
         uiLenMac < macCpOffUL;\
         ++uiLenMac\
      ) {if((inStr)[uiLenMac] == (delimC)) break;};\
      \
      if((inStr)[uiLenMac] != (delimC))\
      { /*If: I have more string to find*/\
         for(\
            macVI8=load_I8Ary_retVI8(&(inStr)[uiLenMac]);\
            !VI8IfDelim(macVI8, (delimVI8));\
            macVI8=load_I8Ary_retVI8(&(inStr)[uiLenMac])\
         ) uiLenMac += defNum8BitElms;\
         \
         for(\
            uiLenMac = uiLenMac;\
            (inStr)[uiLenMac] != (delimC);\
            ++uiLenMac\
         ) {};\
      } /*If: I have more string to find*/\
      \
      uiLenMac;\
   }) /*ulCpStrDelim*/

#elif defined NO_UL_CP
   #define VI8LenStr(inStr, delimVI8, delimC)({\
      uint uiIterMac = 0;\
      for(\
         uiIterMac = 0;\
         (inStr)[uiIterMac] != (delimC);\
         ++uiIterMac\
      ) {}\
      \
      uiIterMac;\
   })
#else
   #define VI8LenStr(inStr, delimVI8, delimC)\
      (ulLenStr((inStr), (delimVI8)))
#endif

/*These are the single byte copy functions*/
#define cCpStr(dupStr, cpStr, lenUI){\
   uint uiIterMac = 0;\
   for(uiIterMac = 0; uiIterMac < lenUI; ++uiIterMac)\
      (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
}


#define cCpStrDelim(dupStr, cpStr, delimC)({\
   uint uiIterMac = 0;\
   for(\
      uiIterMac = 0;\
      (cpStr)[uiIterMac] != (delimC);\
      ++uiIterMac\
   )\
      (dupStr)[uiIterMac] = (cpStr)[uiIterMac];\
   \
   uiIterMac;\
})

#define cLenStr(inStr, delimC)({\
   uint uiIterMac = 0;\
   for(\
      uiIterMac = 0;\
      (inStr)[uiIterMac] != (delimC);\
      ++uiIterMac\
   ) {}\
   \
   uiIterMac;\
})
#endif
