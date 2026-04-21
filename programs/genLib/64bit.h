/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' 64bit SOF:
'   - holds data type and macro to get the maximum long
'     on a system
'   - this will not always be a 64 bit, but the values
'     will still behave the same so long as bytes are 8
'     bits
'   o .h var01:
'     - define ul_64bit and sl_64bit variable
'   o .h def01:
'     - definitions dealing with size of the ulong type
'   o .h def02:
'     - values to change byte lengths to get ulong lengths
'   o .h fun01: emptyBytes_64bit
'     - find number of empty bytes in a ulong
'   o .h fun02: bytesInUL_64bit
'     - find number of bytes in a single ulong
'   o .h fun03: bytesToNumUL_64bit
'     - converts length in bytes to number of ulongs
'       needed to hold the bytes
'     - this is sizeof(unsigned long) bytes alignment and
'       should allow you to avoid read errors
'   o .h fun04: byteLenToFullULLen_64bit
'      - converts length in bytes to number of fully
'        filled ulongs (a paritially filled ulong is lost)
'   o .h fun05: rshiftByte_64bit
'     - shift a byte right by x bytes; over shifts go to 0
'   o .h fun06: ulAlign_64bit
'      - gives the extra number of bytes to get a ul_64bit
'        array ending at the last long (alignment)
'      - this allows you to avoid read errors
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Var01:
|   - define ul_64bit and sl_64bit variable
\-------------------------------------------------------*/
#ifdef NOUL
   typedef unsigned char ul_64bit;
   typedef signed char sl_64bit;
#else
   #ifdef PLAN9_64
      /*plan9 defines long as 32 bit, so need long long*/
      typedef unsigned long long ul_64bit;
      typedef signed long long sl_64bit;
   #else
      #ifdef BIT_32
         typedef unsigned int ul_64bit;
         typedef signed int sl_64bit;
      #else
         typedef unsigned long ul_64bit;
         typedef signed long sl_64bit;
      #endif
   #endif
#endif

/*-------------------------------------------------------\
| Def01:
|   - definitions dealing with size of the ulong type
\-------------------------------------------------------*/
#define def_bitsPerChar_64bit 8 /*number bits / byte*/
#define def_shiftBitsToBytes_64bit 3
   /*do `variant >> def_shiftBitsToBytes_64bit` to get
   `  the number of bytes from bits
   `or do `variant << def_shiftBitsToBytes_64bit` to get
   `  the number of bits from bytes
   */

#define def_bytesInUL_64bit (sizeof(ul_64bit))
   /*number of bytes in the long*/
#define def_bitsInUL_64bit (def_bytesInUL_64bit << def_shiftBitsToBytes_64bit)
   /*number of bits in a long*/

#define def_ulNegBit_64bit ( ((ul_64bit) -1) & (((ul_64bit) -1) >> 1) )
   /*negative bit in a long*/
#define def_maxUL_64bit ((ul_64bit) -1)
   /*maximum value in ulong*/
#define def_maxSL_64bit ( (sl_64bit) ((ul_64bit) ~def_ulNegBit_64bit) )
   /*maximum value in ulong*/
#define def_minSL_64bit ((sl_64bit) def_ulNegBit_64bit)
   /*maximum value in ulong*/

/*-------------------------------------------------------\
| Def02:
|   - values to change byte lengths to get ulong lengths
\-------------------------------------------------------*/

/*do `byte_length & def_modUL_64bit` to find the number of
`   trailing bytes in the last ulong
` do `byte_length - (byte_length & def_modUL_64bit)` to
`   find the number of empty bytes in a ulong
*/
#define def_modUL_64bit (sizeof(ul_64bit ) - 1)
   /*The mod values are 7 (longs), 3 (ints), and 1
   `  (shorts), so, 1 - number of bytes will give me the
   `  mod value
   */

/*Used to convert a length in bytes to length in ulongs
`This should be compliled in.
`  This will only work for up to 128 bit numbers, after
`  that it will fail. However, it is very unlikely a
`  long will be even 128 bits.
*/
#define def_byteLenToULLen_64bit ( ((sizeof(ul_64bit) >> 1) - (sizeof(ul_64bit) >> 3)) - ((sizeof(ul_64bit) >> 3) * (sizeof(ul_64bit) >> 4)) )

/*Logic:
`   - 32bitSize: sizeof(ul_64bit) >> 1:
`     o This gets the number of bytes to shift to get
`       an 32 bit or 16 bit number to one bytes. Basically
`       it is divide by 2
`   - (sizeof(ul_64bit) >> 3)
`     o This adds in 1 if I have eight bytes. It is needed
`       as the 64 bit correction step
`   - 64bitSize: 32bitSize - (sizeof(ul_64bit) >> 3
`     - (sizeof(ul_64bit) >> 1) - (sizeof(ul_64bit) >> 3)
`     o Gives me the number of bits I need to shift to
`       get the number of characters in an long
`   - 128bitSize: 64bitSize - (sizeof(ul_64bit) >> 3) * (sizeof(ul_64bit) >> 4))
`     - (sizeof(ul_64bit >> 1) - sizeof(ul_64bit) >> 3) - ((sizeof(ul_64bit) >> 3) * (sizeof(ul_64bit) >> 4))
`     o used to correct for a 128 bit number.
`   - for values > 128 bit you need to provided a new
`     correction step
*/

/*-------------------------------------------------------\
| Fun01: emptyBytes_64bit
|   - find number of empty bytes in a ulong
| Input:
|   - lenMacroVar1:
|     o length of string in bytes
| Output:
|   - Returns:
|     o number of extra bytes at the end
\-------------------------------------------------------*/
#define emptyBytes_64bit(lenMacroVar1) (sizeof(ul_64bit) - (def_modUL_64bit & (lenMacroVar1)) )

/*-------------------------------------------------------\
| Fun02: bytesInUL_64bit
|   - find number of bytes in a single ulong
| Input:
|   - lenMacroVar2:
|     o length of string in bytes
| Output:
|   - Returns:
|     o number of extra bytes at the end
\-------------------------------------------------------*/
#define bytesInUL_64bit(lenMacroVar2) (def_bytesInUL_64bit - emptyBytes_64bit((lenMacroVar2)))

/*-------------------------------------------------------\
| Fun03: bytesToNumUL_64bit
|   - converts length in bytes to number of ulongs needed
|     to hold the bytes
| Input:
|   - lenMacroVar3:
|     o number of bytes (byte length)
| Output:
|   - Returns:
|     o number of ulongs needed to hold lenSI bytes
\-------------------------------------------------------*/
#define bytesToNumUL_64bit(lenMacroVar3) ( ((lenMacroVar3) >> def_byteLenToULLen_64bit) + (emptyBytes_64bit((lenMacroVar3)) > 0) )

/*-------------------------------------------------------\
| Fun04: byteLenToFullULLen_64bit
|   - converts length in bytes to number of fully filled
|     ulongs (a paritially filled ulong is lost)
| Input:
|   - lenMacroVar4:
|     o number of bytes (byte length)
| Output:
|   - Returns:
|     o number of ulongs needed to hold lenSI bytes
\-------------------------------------------------------*/
#define byteLenToFullULLen_64bit(lenMacroVar4) ((lenMacroVar4) >> def_byteLenToULLen_64bit)

/*-------------------------------------------------------\
| Fun05: rshiftByte_64bit
|   - shift a byte right by x bytes (over shifts go to 0)
| Input:
|   - val:
|     o value to shift right
|   - byte:
|     o number of bytes to shift by
| Output:
|   - Returns:
|     o shifted value
|     o 0 if over shifted
\-------------------------------------------------------*/
#define rshiftByte_64bit(val, byte) ( ( ((ul_64bit) (val)) << ( ( ((ul_64bit) (byte)) & (-(( ((signed int) sizeof(ul_64bit)) - ((signed int) (byte)) ) > 0 ))) << 3 ) ) & (-(( ((signed int) sizeof(ul_64bit)) - ((signed int) (byte)) ) > 0 )))
/* Logic:
`    - right shift by bytes
`    - boundsCheck: (sizeof(ul_64bit)) - byte) ) > 0:
`      o check if shift is out of bounds
`      o 0 if out of bounds (ul_64bit - byte) <= 0
`      o 1 if in bounds of ul_64bit
`    - boundsMask: - boundsCheck: 
`      o 0 if out of bounds
`      o -1 if in of bounds
`    - getBytes: byte & boundsMask:
`      o check if keep bytes
`      o 0 if out of bounds
`      o bytes if in of bounds
`    - shiftVal: getBytes << 3:
`      o get the number of bits to shift
`      o 0 if out of bounds
`      0 number bytes * 8
`    - shift: val << shiftVal:
`      o shift value to correct position
`      0 value if out of bounds
`      0 shifted value
`    - finalVal: shift & boundsMask:
`      o sets out of bounds values to 0
`      o 0 if out of bounds
`      o shifted value if in bounds
*/

/*-------------------------------------------------------\
| Fun06: ulAlign_64bit
|   - gives the extra number of bytes to get a ul_64bit
|     array ending at the last long (alignment)
|   - this allows you to avoid read errors
| Input:
|   - val6Mac:
|     o number of data types
|   - size6Mac:
|     o size of data type
| Output:
|   - Returns:
|     o number of bytes needed to allocate to keep
|       alignment
\-------------------------------------------------------*/
#define ulAlign_64bit(val6Mac, size6Mac) ( bytesToNumUL_64bit((val6Mac) * (size6Mac)) * (def_bytesInUL_64bit / (size6Mac)) )
/*Logic:
` - find alignment to have an array be all ulongs
`   - numBytes: val6Mac * size6Mac
`     o number of bytes the user wants to hold in the
`       array
`   - numberLongs: byteLenToULLen_64(numBytes)
`     o gets the number of longs needed to hold the
`       number of input bytes
`   - sizePerUlong: def_bytesInUL_64bit / size6Mac
`     o get number of objects putting into the ulong array
`   - chunkNumber: numberLongs / sizePerUlong
`     o get the number of chunks to allocate and keep the
`       array having a ulong number of bytes
*/

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
