/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' base10str SOF: Start Of Header
'   - functions to converts c-strings to numbers
'   o header:
'     - has includes and the defined size of each number
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
|  - Has includes and the defined size of each number
|  o header sec-01:
|    - guards
|  o header sec-02:
|    - maximum digits and values for unsigned data types
|  o header sec-03:
|    - maximum digits and values for signed data types
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-01:
^  - guards
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifndef BASE_10_STRING_TO_NUMBER_H
#define BASE_10_STRING_TO_NUMBER_H

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-02:
^  - maximum digits and values for unsigned data types
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_floatPercision_base10str 6
   /*think 6 decimal digits of percision*/
#define def_maxF_base10str (24 / 3.3333)
   /*floats are always 24 bit for integer, but 8 bit
   `   for mentesa
   */

/*Some tricks I use that will be optimized out at
` compiler time:
*/
#define def_maxUL_base10str (unsigned long) -1
#define def_ulBits_base10str (sizeof(unsigned long) << 3)
#define def_ulDig_base10str (def_ulBits_base10str /3.3333)

#define def_maxUI_base10str (unsigned int) -1
#define def_uiBits_base10str (sizeof(unsigned int) << 3)
#define def_uiDig_base10str (def_uiBits_base10str /3.3333)

#define def_maxUS_base10str (unsigned short) -1
#define def_usBits_base10str (sizeof(unsigned short) << 3)
#define def_usDig_base10str (def_usBits_base10str /3.3333)

#define def_maxUC_base10str (unsigned char) -1
#define def_ucBits_base10str (sizeof(unsigned char) << 3)
#define def_ucDig_base10str (def_ucBits_base10str /3.3333)

/* Logic:
`  - def_maxType_base10str:
`    o (unsigned type) -1:
`      - gives largest number possible for the datatype
`  - def_typeBits_base10str:
`    o sizeof(type) << 3:
`      - gives number of bits (bytes * 8) in data type
`  - def_ulDig_base10str:
`    o def_typeBits_base10str / 3.3333
`      - gives a rough idea of the maximum number of base
`        10 digits for a datatype (ie: 10^x)
*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec-03:
^  - Maximum digits and values for signed data types
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_maxSL_base10str (signed long) (def_maxUL_base10str >> 1)
#define def_slBits_base10str ((sizeof(signed long) <<3)-1)
#define def_slDig_base10str (def_slBits_base10str /3.3333)


#define def_maxSI_base10str (signed int) (def_maxUI_base10str >> 1)
#define def_siBits_base10str ((sizeof(signed int) <<3) -1)
#define def_siDig_base10str (def_siBits_base10str /3.3333)


#define def_maxSS_base10str (signed short) (def_maxUS_base10str >> 1)
#define def_ssBits_base10str ((sizeof(signed short)<<3)-1)
#define def_ssDig_base10str (def_ssBits_base10str /3.3333)


#define def_maxSC_base10str (signed char) (def_maxUC_base10str >> 1)
#define def_scBits_base10str ((sizeof(signed char) <<3)-1)
#define def_scDig_base10str (def_scBits_base10str /3.3333)

/* Logic:
`  - def_maxType_base10str:
`    o (signed type) ((unsigned type) -1 >> 1):
`      - sets all bits for type to 1 and then removes the
`        negative bit, giving the largest number possible
`  - def_typeBits_base10str:
`    o (sizeof(type) << 3) - 1:
`      - gives number of bits (bytes * 8) in data type and
`        removes the negative bit (-1)
`  - def_ulDig_base10str:
`    o def_typeBits_base10str / 3.3333
`      - gives a rough idea of the maximum number of base
`        10 digits for a datatype (ie: 10^x)
*/

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif

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
