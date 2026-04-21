/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' checkSum TOC:
'   - library for checksum computing
'   o header:
'     - defiend variables and guards
'   o .c tbl01: crc32Tbl_checkSum
'     - crc32 lookup table
'     - copied from misc0110's libattpng repository
'   o fun01: usAdler32_checkSum
'     - find adler32 checksum with unsigned shorts; 16 bit
'   o fun02: usAdler32Byte_checkSum
'     - does adler32 (signed short) checksum on one byte
'   o fun03: usAdler32Finish_checkSum
'     - does little endin swap for final value of adler32
'   o fun04: adler32_checkSum
'     - finds adler32 checksum with ints; 32 bit
'   o fun05: adler32Byte_checkSum
'     - adds a byte to an adler32 checksum
'   o .h fun06: adler32Finish_checkSum
'     - finishes the addler32 checksum
'   o .c fun07: crc32Mac_checkSum
'     - find new crc value after adding next byte
'     - copied from misc0110's libattpng repository, I
'       really do not know whats going on here
'   o fun08: crc32_checkSum
'     - does a crc32 checksum on input data
'   o fun09: crc32Byte_checkSum
'     - adds byte to a crc32 checksum
'   o .h fun10: crc32Finish_checkSum
'     - does final step in crc32 checkusm
'   o fun11: mkCrc32Tbl_checkSum
'     - makes a crc32 table (used to generate this table)
'     - this was adapted from python code from github
'       co-pilot, which it in turn took from others
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defiend variables and guards
\-------------------------------------------------------*/

#ifndef CHECKSUM_H
#define CHECKSUM_H

#define def_adler32_checkSum 65521
#define def_adler32ToUSMax_checkSum 14
   /*max unsigned short is 65535, so + 14 would get
   `  the maximum for an unsigned short
   */

/*-------------------------------------------------------\
| Fun01: usAdler32_checkSum
|   - finds adler32 checksum with unsigned shorts; 16 bit
| Input:
|   - dataAryUC:
|     o unsigned char array with data to get adler32 with
|   - lenUL:
|     o length to find adler32 for in dataAryUC
|   - adlerAryUC:
|     o 4 byte array to get adler32 value
| Output:
|   - Modifies:
|     o adlerAryUC to have adler32 sum (little endin)
\-------------------------------------------------------*/
void
usAdler32_checkSum(
   unsigned char *dataAryUC,  /*data to sum*/
   unsigned long lenUL,       /*amount of data*/
   unsigned char *adlerAryUC  /*4 bytes, gets adler32*/
);

/*-------------------------------------------------------\
| Fun02: usAdler32Byte_checkSum
|   - does adler32 (signed short) checksum on one byte
| Input:
|   - byteUC:
|     o unsigned char to add to adler32 checksum
|   - adlerAryUC:
|     o 4 byte array to update adler32 checksum
| Output:
|   - Modifies:
|     o adlerAryUC to have updated adler32 sum
| Note:
|  - you will need to convert to little endin at end
|    unsigned short *oneUS = (unsigned short *) &aryUC[2];
|    unsigned short *twoUS = (unsigned short *) &aryUC[0];
|    *oneUS = usToLittle_endin(*oneUS);
|    *twoUS = usToLittle_endin(*twoUS);
\-------------------------------------------------------*/
void
usAdler32Byte_checkSum(
   unsigned char byteUC,      /*byte to add*/
   unsigned char *adlerAryUC  /*4 bytes, gets adler32*/
);

/*-------------------------------------------------------\
| Fun03: usAdler32Finish_checkSum
|   - does little endin swap for final value of adler32
| Input:
|   - adlerAryUC:
|     o 4 byte array with adler32 checksum to finish
| Output:
|   - Modifies:
|     o adlerAryUC to have final adler32 format
\-------------------------------------------------------*/
void
usAdler32Finish_checkSum(
   unsigned char *aryUC    /*4 bytes, value to finish*/
);

/*-------------------------------------------------------\
| Fun04: adler32_checkSum
|   - finds adler32 checksum with ints; 32 bit
| Input:
|   - dataAryUC:
|     o unsigned char array with data to get adler32 with
|   - lenUL:
|     o length to find adler32 for in dataAryUC
| Output:
|   - Returns:
|     o adler32 calculated checksum
\-------------------------------------------------------*/
signed int
adler32_checkSum(
   unsigned char *dataAryUC,  /*data to sum*/
   unsigned long lenUL        /*amount of data*/
);

/*-------------------------------------------------------\
| Fun05: adler32Byte_checkSum
|   - adds a byte to an adler32 checksum
| Input:
|   - byteUC:
|     o byte to add to adler32 checksum
|   - oneSIPtr:
|     o signed int pointer with first checksum value
|   - twoSIPtr:
|     o signed int pointer with second checksum value
| Output:
|   - Modifies:
|     o oneSIPtr to have updated checksum for first value
|     o twoSIPtr to have updated checksum for 2nd value
\-------------------------------------------------------*/
void
adler32Byte_checkSum(
   unsigned char byteUC,  /*byte to add to checksum*/
   signed int *oneSIPtr,  /*first value in adler32*/
   signed int *twoSIPtr   /*second value in adler32*/
);

/*-------------------------------------------------------\
| Fun06: adler32Finish_checkSum
|   - finishes the addler32 checksum
| Input:
|   - oneMac:
|     o first value in adler32 checksum (signed int)
|   - twoSIPtr:
|     o second value in adler32 checksum (signed int)
| Output:
|   - Returns:
|     o adler32 checksum
\-------------------------------------------------------*/
#define adler32Finish_checkSum(oneMac, twoMac) ( ((twoMac) << 16) | (oneMac))

/*-------------------------------------------------------\
| Fun08: crc32_checkSum
|   - does a crc32 checksum on input data
| Input:
|   - aryUC:
|     o unsigned char array with data to get crc32 with
|   - lenUL:
|     o length of dataAryUC
| Output:
|   - Returns:
|     o crc32 checksum
\-------------------------------------------------------*/
unsigned int
crc32_checkSum(
   unsigned char *aryUC, /*data to get crc32 checksum on*/
   unsigned long lenUL   /*bytes in aryUC*/
);

/*-------------------------------------------------------\
| Fun09: crc32Byte_checkSum
|   - adds byte to a crc32 checksum
| Input:
|   - byteUC:
|     o byte to add to crc32 checksum
|   - crc32UI:
|     o currnet crc32 checksum
| Output:
|   - Returns:
|     o updated crc32 checksum
\-------------------------------------------------------*/
unsigned int
crc32Byte_checkSum(
   unsigned char byteUC, /*data to add to crc32 checksum*/
   unsigned int crc32UI  /*current crc32 checksum*/
);

/*-------------------------------------------------------\
| Fun10: crc32Finish_checkSum
|   - does final step in crc32 checkusm
| Input:
|   - crc32Mac:
|     o crc32 checksum to finalize
| Output:
|   - Returns:
|     o finalized crc32 checksum
\-------------------------------------------------------*/
#define crc32Finish_checkSum(crc32Mac) ((unsigned int) ~(crc32Mac))

/*-------------------------------------------------------\
| Fun11: mkCrc32Tbl_checkSum
|   - makes a crc32 table (used to generate this table)
|   - this was adapted from python code from github
|     co-pilot, which it in turn took from others
| Input:
|   - crc32AryUI:
|     o unsigned int array to hold crc32 values; needs to
|       have at least 256 elements
| Output:
|   - Modifies:
|     o crc32AryUI to have the crc32 table
\-------------------------------------------------------*/
void
mkCrc32Tbl_checkSum(
   unsigned int *crc32AryUI
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
