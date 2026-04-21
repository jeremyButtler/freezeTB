/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' base10str SOF: Start Of Header
'   - functions to converts c-strings to numbers
'   o header:
'     - included libraries
'   o fun01: strToUL_base10str
'     - converts a base 10 c-string to an unsigned long
'   o fun02: strToUI_base10str
'     - converts a base 10 c-string to an unsigned int
'   o fun03: strToUS_base10str
'     - converts a base 10 c-string to an unsigned short
'   o fun04: strToUC_base10str
'     - converts a base 10 c-string to an unsigned char
'   o fun05: strToSL_base10str
'     - converts base 10 c-string to a signed long
'   o fun06: strToSI_base10str
'     - converts base 10 c-string to a signed int
'   o fun07: strToSS_base10str
'     - converts base 10 c-string to a signed short
'   o fun08: strToSC_base10str
'     - converts base 10 c-string to a signed char
'   o fun09: strToF_base10str
'     - converts base 10 c-string to float
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "base10str.h"

/*-------------------------------------------------------\
| Fun01: strToUL_base10str
|   - converts a base 10 c-string to an unsigned long
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retUL:
|     o pointer to unisnged long to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retUL to hold the unsigned long
| Note:
|   - only converts till retUL goes out off bounds
\-------------------------------------------------------*/
unsigned int
strToUL_base10str(
   signed char *inStr,
   unsigned long *retUL
){
   unsigned int uiDig = 0;
   unsigned long tmpUL = 0;

   *retUL = 0;

   for(
      uiDig = 0;
      uiDig < def_ulDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retUL = *retUL * 10 + inStr[uiDig] - 48;

      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/
      if(*retUL <= def_maxUL_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpUL = 10 * *retUL + inStr[uiDig] - 48;

         if(tmpUL > 9)
         { /*If: I have room for 10th digit*/
            *retUL = tmpUL;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (could overflow)*/

   return uiDig;
} /*strToUL_base10str*/

/*-------------------------------------------------------\
| Fun02: strToUI_base10str
|   - converts a base 10 c-string to an unsigned int
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retUI:
|     o pointer to unisnged int to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retUI to hold the unsigned int
| Note:
|   - only converts till retUI goes out off bounds
\-------------------------------------------------------*/
unsigned int
strToUI_base10str(
   signed char *inStr,
   unsigned int *retUI
){
   unsigned int uiDig = 0;
   unsigned int tmpUI = 0;

   *retUI = 0;

   for(
      uiDig = 0;
      uiDig < def_uiDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retUI = *retUI * 10 + inStr[uiDig] - 48;

      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (couid overflow)*/
      if(*retUI <= def_maxUI_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpUI = 10 * *retUI + inStr[uiDig] - 48;

         if(tmpUI > 9)
         { /*If: I have room for 10th digit*/
            *retUI = tmpUI;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (couid overflow)*/

   return uiDig;
} /*strToUI_base10str*/

/*-------------------------------------------------------\
| Fun03: strToUS_base10str
|   - converts a base 10 c-string to an unsigned short
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retUS:
|     o pointer to unisnged short to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retUS to hold the unsigned short
| Note:
|   - only converts till retUS goes out off bounds
\-------------------------------------------------------*/
unsigned int
strToUS_base10str(
   signed char *inStr,
   unsigned short *retUS
){
   unsigned int uiDig = 0;
   unsigned short tmpUS = 0;

   *retUS = 0;

   for(
      uiDig = 0;
      uiDig < def_usDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retUS = *retUS * 10 + inStr[uiDig] - 48;

      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (cousd overflow)*/
      if(*retUS <= def_maxUS_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpUS = 10 * *retUS + inStr[uiDig] - 48;

         if(tmpUS > 9)
         { /*If: I have room for 10th digit*/
            *retUS = tmpUS;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (cousd overflow)*/

   return uiDig;
} /*strToUS_base10str*/

/*-------------------------------------------------------\
| Fun04: strToUC_base10str
|   - converts a base 10 c-string to an unsigned char
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retUC:
|     o pointer to unisnged char to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retUC to hold the unsigned char
| Note:
|   - only converts till retUC goes out off bounds
\-------------------------------------------------------*/
unsigned int
strToUC_base10str(
   signed char *inStr,
   unsigned char *retUC
){
   unsigned int uiDig = 0;
   unsigned char tmpUC = 0;

   *retUC = 0;

   for(
      uiDig = 0;
      uiDig < def_ucDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retUC = *retUC * 10 + inStr[uiDig] - 48;

      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (coucd overflow)*/
      if(*retUC <= def_maxUC_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpUC = 10 * *retUC + inStr[uiDig] - 48;

         if(tmpUC > 9)
         { /*If: I have room for 10th digit*/
            *retUC = tmpUC;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (coucd overflow)*/

   return uiDig;
} /*strToUC_base10str*/

/*-------------------------------------------------------\
| Fun05: strToSL_base10str
|   - converts base 10 c-string to a signed long
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retSL:
|     o pointer to signed long to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retSL to hold the signed long
| Note:
|   - only converts until retSL overflows
\-------------------------------------------------------*/
unsigned int
strToSL_base10str(
   signed char *inStr,
   signed long *retSL
){
   unsigned int uiDig = 0;
   signed long tmpSL = 0;

   *retSL = 0;

   for(
      uiDig = (inStr[0] == '-');
      uiDig < def_slDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retSL = *retSL * 10 + inStr[uiDig] - 48;
      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/
      if(*retSL <= def_maxSL_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpSL = 10 * *retSL + inStr[uiDig] - 48;

         if(tmpSL > 9)
         { /*If: I have room for 10th digit*/
            *retSL = tmpSL;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (could overflow)*/

   if(inStr[0] == '-')
      *retSL = -(*retSL);

   return uiDig;
} /*strToSL_base10str*/

/*-------------------------------------------------------\
| Fun06: strToSI_base10str
|   - converts base 10 c-string to a signed int
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retSI:
|     o pointer to signed int to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retSI to hold the signed int
| Note:
|   - only converts until retSI overflows
\-------------------------------------------------------*/
unsigned int
strToSI_base10str(
   signed char *inStr,
   signed int *retSI
){
   unsigned int uiDig = 0;
   signed int tmpSI = 0;

   *retSI = 0;

   for(
      uiDig = (inStr[0] == '-');
      uiDig < def_siDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retSI = *retSI * 10 + inStr[uiDig] - 48;
      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/
      if(*retSI <= def_maxSI_base10str / 10)
      { /*If: number is < the max unsigned int size/10*/
         tmpSI = 10 * *retSI + inStr[uiDig] - 48;

         if(tmpSI > 9)
         { /*If: I have room for 10th digit*/
            *retSI = tmpSI;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned int size/10*/
   } /*If have a last digit (could overflow)*/

   if(inStr[0] == '-')
      *retSI = -(*retSI);

   return uiDig;
} /*strToSI_base10str*/

/*-------------------------------------------------------\
| Fun07: strToSS_base10str
|   - converts base 10 c-string to a signed short
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retSS:
|     o pointer to signed short to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retSS to hold the signed short
| Note:
|   - only converts until retSS overflows
\-------------------------------------------------------*/
unsigned short
strToSS_base10str(
   signed char *inStr,
   signed short *retSS
){
   unsigned short uiDig = 0;
   signed short tmpSS = 0;

   *retSS = 0;

   for(
      uiDig = (inStr[0] == '-');
      uiDig < def_ssDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retSS = *retSS * 10 + inStr[uiDig] - 48;
      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/
      if(*retSS <= def_maxSS_base10str / 10)
      { /*If: number is < the max unsigned short ssze/10*/
         tmpSS = 10 * *retSS + inStr[uiDig] - 48;

         if(tmpSS > 9)
         { /*If: I have room for 10th digit*/
            *retSS = tmpSS;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned short ssze/10*/
   } /*If have a last digit (could overflow)*/

   if(inStr[0] == '-')
      *retSS = -(*retSS);

   return uiDig;
} /*strToSS_base10str*/

/*-------------------------------------------------------\
| Fun08: strToSC_base10str
|   - converts base 10 c-string to a signed char
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retSC:
|     o pointer to signed char to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retSC to hold the signed char
| Note:
|   - only converts until retSC overflows
\-------------------------------------------------------*/
unsigned char
strToSC_base10str(
   signed char *inStr,
   signed char *retSC
){
   unsigned char uiDig = 0;
   signed char tmpSC = 0;

   *retSC = 0;

   for(
      uiDig = (inStr[0] == '-');
      uiDig < def_scDig_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retSC = *retSC * 10 + inStr[uiDig] - 48;
      else
         break;
   } /*Loop: Convert digits with no overflow concern*/

   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/
      if(*retSC <= def_maxSC_base10str / 10)
      { /*If: number is < the max unsigned char scze/10*/
         tmpSC = 10 * *retSC + inStr[uiDig] - 48;

         if(tmpSC > 9)
         { /*If: I have room for 10th digit*/
            *retSC = tmpSC;
            ++uiDig;
         } /*If: I have room for 10th digit*/
      } /*If: number is < the max unsigned char scze/10*/
   } /*If have a last digit (could overflow)*/

   if(inStr[0] == '-')
      *retSC = -(*retSC);

   return uiDig;
} /*strToSC_base10str*/

/*-------------------------------------------------------\
| Fun09: strToF_base10str
|   - converts base 10 c-string to float
| Input:
|   - inStr:
|     o c-string with base 10 number to convert
|   - retF:
|     o pointer to float to hold converted number
| Output:
|   - Returns:
|     o number of characters converted
|   - Modifies:
|     o retF to hold the float
| Note:
|   - only converts until retF overflows
\-------------------------------------------------------*/
unsigned char
strToF_base10str(
   signed char *inStr,
   float *retF
){
   unsigned char uiDig = 0;
   float tmpF = 0;
   signed int powerTenSI;
   unsigned char decUC = 0;

   *retF = 0;


   for(
      uiDig = (inStr[0] == '-');
      uiDig < def_maxF_base10str;
      ++uiDig
   ){ /*Loop: Convert digits with no overflow concern*/
      if(
            inStr[uiDig] > 47
         && inStr[uiDig] < 58
      ) *retF = *retF * 10 + inStr[uiDig] - 48;
      else
         break;
   } /*Loop: Convert digits with no overflow concern*/


   if(
         inStr[uiDig] > 47
      && inStr[uiDig] < 58
   ){ /*If have a last digit (could overflow)*/

      if(*retF <= def_maxF_base10str / 10)
      { /*If: number is < the max float size/10*/
         tmpF = 10 * *retF + inStr[uiDig] - 48;

         if(tmpF > 9)
         { /*If: I have room for 10th digit*/
            *retF = tmpF;
            ++uiDig;
         } /*If: I have room for 10th digit*/

      } /*If: number is < the max float size/10*/
   } /*If have a last digit (could overflow)*/


   if(inStr[uiDig] == '.')
   { /*If: have decimal*/

      powerTenSI = 10;
      ++uiDig;

      while(decUC < def_floatPercision_base10str)
      { /*Loop: add decimial to float*/
         if(
               inStr[uiDig] > 47
            && inStr[uiDig] < 58
         ){ /*If: have digit*/
            *retF +=
               (float)
               ( inStr[uiDig] - 48 ) / powerTenSI;
            powerTenSI *= 10;
         }  /*If: have digit*/

         else
            break;

         ++decUC;
         ++uiDig;
      } /*Loop: add decimial to float*/

   } /*If: have decimal*/


   if(inStr[0] == '-')
      *retF = -(*retF);

   return uiDig;
} /*strToF_base10str*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
