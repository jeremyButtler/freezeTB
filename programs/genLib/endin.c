/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' endin SOF: Start Of File
'   - converts 
'   o header:
'     - defined variables
'   o fun01: ulToBig_endin
'     - converts unsigned long to big endin format
'   o fun02: uiToBig_endin
'     - converts unsigned int to big endin format
'   o fun03: usToBig_endin
'     - converts unsigned short to big endin format
'   o fun04: ulToLittle_endin
'     - converts unsigned long to little endin format
'   o fun05: uiToLittle_endin
'     - converts unsigned int to little endin format
'   o fun06: usToLittle_endin
'     - converts unsigned short to little endin format
'   o fun07: ulSwap_endin
'     - converts unsigned long to different endin format
'   o fun08: uiSwap_endin
'     - converts unsigned int to oppisite endin format
'   o fun09: usSwap_endin
'     - converts unsigned short to oppisite endin format
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "endin.h"

/*these are assumptions that are not rigid, but if false
`  would be devisitating to the coding community. So,
`  though the number of bits in a byte is not formal it
`  is still very unlikely to be different on any modern
`  machine. Basically, it might as well be in the
`  standard.
*/
#define def_bitInByte_endin 8
#define def_ulHalf_endin (sizeof(unsigned long) << 2)
#define def_uiHalf_endin (sizeof(unsigned int) << 2)
#define def_usHalf_endin (sizeof(unsigned short) << 2)
   /*shifting by 2 instead of 3 to get half of the size
   `   of the datatype
   */

/*-------------------------------------------------------\
| Fun01: ulToBig_endin
|   - converts unsigned long to big endin format
| Input:
|   - valUL:
|     o unsigned long to convert
| Output:
|   - Returns:
|     o bigendin number as unsigned long
\-------------------------------------------------------*/
unsigned long
ulToBig_endin(
   unsigned long valUL
){
   #ifdef BIG_ENDIN
      return valUL; /*already in bigendin format*/
   #else
      return ulSwap_endin(valUL);
   #endif
} /*ulToBig_endin*/

/*-------------------------------------------------------\
| Fun02: uiToBig_endin
|   - converts unsigned int to big endin format
| Input:
|   - valUI:
|     o unsigned int to convert
| Output:
|   - Returns:
|     o bigendin number as unsigned int
\-------------------------------------------------------*/
unsigned int
uiToBig_endin(
   unsigned int valUI
){
   #ifdef BIG_ENDIN
      return valUI; /*already in bigendin format*/
   #else
      return uiSwap_endin(valUI);
   #endif
} /*uiToBig_endin*/

/*-------------------------------------------------------\
| Fun03: usToBig_endin
|   - converts unsigned short to big endin format
| Input:
|   - valUI:
|     o unsigned short to convert
| Output:
|   - Returns:
|     o bigendin number as unsigned short
\-------------------------------------------------------*/
unsigned short
usToBig_endin(
   unsigned short valUS
){
   #ifdef BIG_ENDIN
      return valUS; /*already in bigendin format*/
   #else
      return usSwap_endin(valUS);
   #endif
} /*usToBig_endin*/

/*-------------------------------------------------------\
| Fun04: ulToLittle_endin
|   - converts unsigned long to little endin format
| Input:
|   - valUL:
|     o unsigned long to convert
| Output:
|   - Returns:
|     o little endin number as unsigned long
\-------------------------------------------------------*/
unsigned long
ulToLittle_endin(
   unsigned long valUL
){
   #ifdef BIG_ENDIN
      return ulSwap_endin(valUL);
   #else
      return valUL; /*already in little endin format*/
   #endif
} /*ulToLittle_endin*/

/*-------------------------------------------------------\
| Fun05: uiToLittle_endin
|   - converts unsigned int to little endin format
| Input:
|   - valUI:
|     o unsigned int to convert
| Output:
|   - Returns:
|     o little endin number as unsigned int
\-------------------------------------------------------*/
unsigned int
uiToLittle_endin(
   unsigned int valUI
){
   #ifdef BIG_ENDIN
      return uiSwap_endin(valUI);
   #else
      return valUI; /*already in little endin format*/
   #endif
} /*uiToLittle_endin*/

/*-------------------------------------------------------\
| Fun06: usToLittle_endin
|   - converts unsigned short to little endin format
| Input:
|   - valUI:
|     o unsigned short to convert
| Output:
|   - Returns:
|     o little endin number as unsigned short
\-------------------------------------------------------*/
unsigned short
usToLittle_endin(
   unsigned short valUS
){
   #ifdef BIG_ENDIN
      return usSwap_endin(valUS);
   #else
      return valUS; /*already in little endin format*/
   #endif
} /*usToLittle_endin*/

/*-------------------------------------------------------\
| Fun07: ulSwap_endin
|   - converts unsigned long to different endin format
| Input:
|   - valUL:
|     o unsigned long to convert
| Output:
|   - Returns:
|     o oppisite endin format as valUL
\-------------------------------------------------------*/
unsigned long
ulSwap_endin(
   unsigned long valUL
){
   unsigned char ucByte = 0;
   unsigned long retUL = 0;

   for(
      ucByte = def_bitInByte_endin;
      ucByte < ( sizeof(unsigned long) << 3 );
      ucByte += (def_bitInByte_endin << 1)
   ){ /*Loop: change endin*/
      
      retUL |=
         (
             (valUL << ucByte)
           & (
                   0xff
                << (
                      def_ulHalf_endin
                    + (ucByte >> 1) - 4
                   )
              )
         );
         /*Logic:
         `   - numBits: 32 + (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 32 is half of a unsigned long
         `     o 32 + ( 8 >> 1) - 4 = 32 +  4 - 4 = 32
         `     o 32 + (24 >> 1) - 4 = 32 + 12 - 4 = 40
         `     o 32 + (40 >> 1) - 4 = 32 + 20 - 4 = 48
         `     o 32 + (56 >> 1) - 4 = 32 + 28 - 4 = 56
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff00000000
         `     o 24: 0xff0000000000
         `     o 40: 0xff000000000000
         `     o 56: 0xff00000000000000
         `   - shift: valUL >> ucByte
         `     - shifts value to swap position
         `   - retUL |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */

      retUL |=
         (
              (valUL >> ucByte)
            & (
                    0xff
                 << (
                        def_ulHalf_endin
                      - (ucByte >> 1) - 4
                    )
               )
         );
         /*Logic:
         `   - numBits: 32 - (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 32 is half of a unsigned long
         `     o 32 - ( 8 >> 1) - 4 = 32 -  4 - 4 = 24
         `     o 32 - (24 >> 1) - 4 = 32 - 12 - 4 = 16
         `     o 32 - (40 >> 1) - 4 = 32 - 20 - 4 = 8
         `     o 32 - (56 >> 1) - 4 = 32 - 28 - 4 = 0
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff000000
         `     o 24: 0xff0000
         `     o 40: 0xff00
         `     o 56: 0xff
         `   - shift: valUL >> ucByte
         `     - shifts value to swap position
         `   - retUL |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */
   }  /*Loop: change endin*/

   return retUL;
} /*ulSwap_endin*/

/*-------------------------------------------------------\
| Fun08: uiSwap_endin
|   - converts unsigned int to oppisite endin format
| Input:
|   - valUI:
|     o unsigned int to convert
| Output:
|   - Returns:
|     o oppisiste endin number as unsigned int
\-------------------------------------------------------*/
unsigned int
uiSwap_endin(
   unsigned int valUI
){
   unsigned char ucByte = 0;
   unsigned int retUI = 0;

   for(
      ucByte = def_bitInByte_endin;
      ucByte < ( sizeof(unsigned int) << 3 );
      ucByte += (def_bitInByte_endin << 1)
   ){ /*Loop: swap to oppisite endin*/
      
      retUI |=
         (
             (valUI << ucByte)
           & (
                   0xff
                << (
                      def_uiHalf_endin
                    + (ucByte >> 1) - 4
                   )
              )
         );
         /*Logic:
         `   - numBits: 16 + (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 16 is half of a unsigned int
         `     o 16 + ( 8 >> 1) - 4 = 16 +  4 - 4 = 16
         `     o 16 + (24 >> 1) - 4 = 16 + 12 - 4 = 24
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff0000
         `     o 24: 0xff000000
         `   - shift: valUI >> ucByte
         `     - shifts value to swap position
         `   - retUI |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */

      retUI |=
         (
              (valUI >> ucByte)
            & (
                    0xff
                 << (
                        def_uiHalf_endin
                      - (ucByte >> 1) - 4
                    )
               )
         );
         /*Logic:
         `   - numBits: 16 - (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 16 is half of a unsigned int
         `     o 16 - ( 8 >> 1) - 4 = 16 -  4 - 4 = 8
         `     o 16 - (24 >> 1) - 4 = 16 - 12 - 4 = 0
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff00
         `     o 24: 0xff
         `   - shift: valUI >> ucByte
         `     - shifts value to swap position
         `   - retUI |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */
   }  /*Loop: swap to oppisite endin*/

   return retUI;
} /*uiSwap_endin*/

/*-------------------------------------------------------\
| Fun09: usSwap_endin
|   - converts unsigned short to oppisite endin format
| Input:
|   - valUI:
|     o unsigned short to convert
| Output:
|   - Returns:
|     o oppisite endin number as unsigned short
\-------------------------------------------------------*/
unsigned short
usSwap_endin(
   unsigned short valUS
){
   unsigned char ucByte = 0;
   unsigned short retUS = 0;

   for(
      ucByte = def_bitInByte_endin;
      ucByte < ( sizeof(unsigned short) << 3 );
      ucByte += (def_bitInByte_endin << 1)
   ){ /*Loop: swap to oppisite endin*/
      
      retUS |=
         (
             (valUS << ucByte)
           & (
                   0xff
                << (
                      def_usHalf_endin
                    + (ucByte >> 1) - 4
                   )
              )
         );
         /*Logic:
         `   - numBits: 8 + (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 8 is half of a unsigned short
         `     o 8 + ( 8 >> 1) - 4 = 8 +  4 - 4 = 8
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff00
         `   - shift: valUS >> ucByte
         `     - shifts value to swap position
         `   - retUS |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */

      retUS |=
         (
              (valUS >> ucByte)
            & (
                    0xff
                 << (
                        def_usHalf_endin
                      - (ucByte >> 1) - 4
                    )
               )
         );
         /*Logic:
         `   - numBits: 8 - (ucByte >> 1) - 4
         `     - number of bits need to shift 0xff with
         `       to clear everything except the target
         `       - 8 is half of a unsigned short
         `     o 8 - ( 8 >> 1) - 4 = 8 -  4 - 4 = 0
         `   - mask: 0xff << numBits
         `     - mask to clear everything except swapped
         `       value
         `     o 8:  0xff
         `   - shift: valUS >> ucByte
         `     - shifts value to swap position
         `   - retUS |= (shift & mask)
         `     - clears bits not in swap & adds to
         `       return value
         */
   }  /*Loop: swap to oppisite endin*/

   return retUS;
} /*usSwap_endin*/

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
