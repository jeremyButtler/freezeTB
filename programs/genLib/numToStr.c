/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' numToStr SOF: Start Of File
'   - function(s) to convert a number to a string
'   o header:
'     - included libraries
'   o fun01: numToStr
'     - converts a number to a c-string
'   o fun02: backNumToStr
'     - converts a number to a c-string backwards
'   o fun03: signed_numToStr
'     - converts a signed number to a c-string
'   o fun04: double_numToStr
'     - converts a double to a c-string
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "numToStr.h"

/*-------------------------------------------------------\
| Fun01: numToStr
|   - converts a number to a c-string
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numUL:
|     o number to convert
| Output:
|   - Modifies:
|     o cstr to have the number (adds a '\0' at the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
numToStr(
   signed char *cstr,
   unsigned long numUL
){
   unsigned int retUI = 0;
   signed char *startStr = 0;
   signed char *endStr = 0;

   do{
      cstr[retUI] = (numUL % 10) + 48;
      ++retUI;
      numUL /= 10;
   } while(numUL);

   cstr[retUI] = '\0';

   endStr = &cstr[retUI - 1];
   startStr = cstr;

   while(startStr < endStr)
   { /*Loop: Reverse array*/
      *endStr ^= *startStr;
      *startStr ^= *endStr;
      *endStr-- ^= *startStr++;
   } /*Loop: Reverse array*/

   return retUI;
} /*numToStr*/

/*-------------------------------------------------------\
| Fun02: backNumToStr
|   - converts a number to a c-string backwards
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numUL:
|     o number to convert
| Output:
|   - Modifies:
|     o cstr to have the backwards number (adds a '\0' at
|       the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
backwards_numToStr(
   signed char *cstr,
   unsigned long numUL
){
   unsigned int retUI = 0;

   do{
      cstr[retUI] = (numUL % 10) + 48;
      ++retUI;
      numUL /= 10;
   } while(numUL);

   cstr[retUI] = '\0';
   return retUI;
} /*backNumToStr*/

/*-------------------------------------------------------\
| Fun03: signed_numToStr
|   - converts a signed number to a c-string
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numSL:
|     o number to convert
| Output:
|   - Modifies:
|     o cstr to have the number (adds a '\0' at the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
signed int
signed_numToStr(
   signed char *cstr,
   signed long numSL
){
   signed int retSI = 0;
   signed char *startStr = 0;
   signed char *endStr = 0;

   if(numSL < 0)
   { /*If: negative number*/
      numSL *= -1;
      cstr[0] = '-';
      ++retSI;
      startStr = cstr + 1;
   } /*If: negative number*/

   else
      startStr = cstr;

   do{
      cstr[retSI] = (numSL % 10) + 48;
      ++retSI;
      numSL /= 10;
   } while(numSL);

   cstr[retSI] = '\0';
   endStr = &cstr[retSI - 1];

   while(startStr < endStr)
   { /*Loop: Reverse array*/
      *endStr ^= *startStr;
      *startStr ^= *endStr;
      *endStr-- ^= *startStr++;
   } /*Loop: Reverse array*/

   return retSI;
} /*signed_numToStr*/

/*-------------------------------------------------------\
| Fun04: double_numToStr
|   - converts a double to a c-string
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numDbl:
|     o number to convert
|   - decUC:
|     o number decimal digits to keep
|       (max_dblDecimal_numToStr)
| Output:
|   - Modifies:
|     o cstr to have the number (adds a '\0' at the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
double_numToStr(
   signed char *cstr,
   double numDbl,
   unsigned char decUC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - converts a double to a c-string
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - convert non-decimal part to c-string
   '   o fun04 sec03:
   '     - convert decimal part to number
   '   o fun04 sec04:
   '     - check if need to round
   '   o fun04 sec05:
   '     - add null and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int retUI = 0;
   signed char *startStr = 0;
   signed char *endStr = 0;
   signed long numSL = 0;   /*has non-decimal number*/
   double decDbl = 0;       /*has decimal fraction*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - convert non-decimal part to c-string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numDbl < 0)
   { /*If: negative number*/
      numDbl *= -1;
      cstr[retUI++] = '-'; /*negative number*/
   } /*If: negative number*/

   numSL = (signed long) numDbl;
   decDbl = numDbl - numSL;

   do{
      cstr[retUI++] = (numSL % 10) + 48;
      numSL /= 10;
   } while(numSL);

   endStr = &cstr[retUI - 1];
   startStr = cstr;
   startStr += (*cstr == '-');

   while(startStr < endStr)
   { /*Loop: Reverse array (is bacwards)*/
      *endStr ^= *startStr;
      *startStr ^= *endStr;
      *endStr-- ^= *startStr++;
   } /*Loop: Reverse array (is backwards)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - convert decimal part to number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(decUC > max_dblDecimal_numToStr)
      decUC = max_dblDecimal_numToStr;
      /*make sure not out of percision range*/

   if(decDbl != 0)
   { /*If: have decimal part*/
      cstr[retUI++] = '.';

      while(decDbl != 0)
      { /*Loop: add in decimals*/
         if(! decUC)
            break;

         decDbl *= 10; /*convert decimal to whole number*/
         cstr[retUI++] = ((signed char) decDbl) + 48;

         decDbl -= ((signed char) decDbl);
            /*remove decimal*/

         --decUC;
      } /*Loop: add in decimals*/
   } /*If: have decimal part*/

   cstr[retUI] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - check if need to round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(decDbl * 10 >= 5)
   { /*If: need to round up*/
      startStr = &cstr[retUI - 1];

      if(*startStr != '9')
         ++(*startStr);

      else
      { /*Else: need to round multiple values*/
         while(*startStr == '9')
         { /*Loop: round up*/
            if(
                  startStr == cstr
               || *startStr == '-'
            ){ /*If: need to round up on frist digit*/
               endBuff_fun04_sec04:;

               endStr = startStr + 1;

               while(*endStr != '\0')        
               { /*Loop: copy values*/
                  *endStr ^= *startStr;
                  *startStr ^= *endStr;
                  *endStr++ ^= *startStr;
               } /*Loop: copy values*/

               *endStr = *startStr; /*copy last value*/
               *startStr = '1';
               ++retUI; /*account for one more digit*/

               goto rounded_fun04_sec03;
            } /*If: need to round up on frist digit*/

            if(*startStr != '.')
               *startStr = '0';

            --startStr;

            if(*startStr == '.')
               --startStr; /*on the decimal*/
         } /*Loop: round up*/

         if(*startStr != '.')
            ++(*startStr); /*round up last digit*/
         else if(
              startStr != cstr
           && *startStr != '-'
         ) *(startStr - 1) += 1; /*round up last digit*/

         else
            goto endBuff_fun04_sec04;
         
         rounded_fun04_sec03:;
      } /*Else: need to round multiple values*/
   } /*If: need to round up*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - add null and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   return retUI;
} /*double_numToStr*/

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
