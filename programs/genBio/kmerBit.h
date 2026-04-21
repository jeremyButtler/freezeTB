/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' kmerBit SOF: Start Of File
'   - has table and macros to converts a set of three
'     nucleotide index's (seqToIndex_alnSet) to a kmer
'   o .h tbl01: alnNtTo_kmerBit
'     - converts an nucleotide alignment code form
'       alnSetStruct.h to an two bit value, with an 3rd
'       bit being used for anonymous bases and the 4th bit
'       for errors
'   o .h fun01: mkMask_kmerBit
'     - makes a mask for clearing extra bits from a kmer
'   o .h fun02: ntIndexToKmer_kmerBit
'     - adds a nucleotide (in index format
'      [seqToIndex_alnSet]) to a kmer
'   o .h fun03: ntBitToKmer_kmerBit
'     - adds a nucleotide bit (by alNtTo_kmerBit) to kmer
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef KMER_BIT_H
#define KMER_BIT_H

/*if you change these, then also change these in
`  kmerFun.h
*/
#define def_bitsPerKmer_kmerBit 2 /*do not change*/
#define def_endKmers_kmerBit -2
#define def_noKmer_kmerBit -1

#define def_a_kmerBit 0
#define def_c_kmerBit 1
#define def_g_kmerBit 2
#define def_t_kmerBit 3
#define def_anonNt_kmerBit 4 /*anonmyous nucleotide*/
#define def_noNt_kmerBit 8

/*-------------------------------------------------------\
| Tbl01: alnNtTo_kmerBit
|   - converts an nucleotide alignment code form
|     alnSetStruct.h to an two bit value, with an 3rd
|     bit being used for anonymous bases and the 4th bit
|     for errors
|   - this will generate a warning if not used. Bit of
|     a pain, but need the table to be public
\-------------------------------------------------------*/
static unsigned char alnNtTo_kmerBit[] =
{
   def_noNt_kmerBit,    /*00 = 64 = ,*/  
   def_a_kmerBit,       /*01 = 65 = A/a*/  
   def_anonNt_kmerBit,  /*02 = 66 = B/b*/  
   def_c_kmerBit,       /*03 = 67 = C/c*/  
   def_anonNt_kmerBit,  /*04 = 68 = D/d*/  
   def_noNt_kmerBit,    /*05 = 69 = E/e*/  
   def_noNt_kmerBit,    /*06 = 70 = F/f*/  
   def_g_kmerBit,       /*07 = 71 = G/g*/  
   def_anonNt_kmerBit,  /*08 = 72 = H/h*/  
   def_noNt_kmerBit,    /*09 = 73 = I/i*/  
   def_noNt_kmerBit,    /*10 = 74 = J/j*/  
   def_anonNt_kmerBit,  /*11 = 75 = K/k*/  
   def_noNt_kmerBit,    /*12 = 76 = L/l*/  
   def_anonNt_kmerBit,  /*13 = 77 = M/m*/  
   def_anonNt_kmerBit,  /*14 = 78 = N/n*/  
   def_noNt_kmerBit,    /*15 = 79 = O/o*/  
   def_noNt_kmerBit,    /*16 = 80 = P/p*/  
   def_noNt_kmerBit,    /*17 = 81 = Q/q*/  
   def_anonNt_kmerBit,  /*18 = 82 = R/r*/  
   def_anonNt_kmerBit,  /*19 = 83 = S/s*/  
   def_t_kmerBit,       /*20 = 84 = T/t*/  
   def_t_kmerBit,       /*21 = 85 = U/u*/  
   def_anonNt_kmerBit,  /*22 = 86 = V/v*/  
   def_anonNt_kmerBit,  /*23 = 87 = W/w*/  
   def_anonNt_kmerBit,  /*24 = 88 = X/x (amino acids)*/  
   def_anonNt_kmerBit,  /*25 = 89 = Y/y*/  
   def_noNt_kmerBit     /*26 = 90 = Z/z*/  
}; /*alnNtTo_kmerBit*/

/*-------------------------------------------------------\
| Fun01: mkMask_kmerBit
|   - makes a mask for clearing extra bits from a kmer
| Input:
|   - lenKmerMac:
|     o number nucleotides in one kmer
| Output:
|   - Returns:
|     o mask to clear kmer (0's for unused bits and 1's
|       for used bits)
\-------------------------------------------------------*/
#define mkMask_kmerBit(lenKmerMac) ( ((unsigned long) -1) >> (( sizeof(unsigned long) << 3 ) - ( (lenKmerMac) * def_bitsPerKmer_kmerBit )) )
/*Logic:
`  - kmerBits: lenKmerMac * def_bitsPerKmer_kmerBit:
`    o gets number of bits needed to fill one kmer
`  - sizeLong: sizeof(unsigned long) << 3
`    o number of bits in unsigned long
`  - emptyBits: sizeLong - kmerBits:
`    o number of extra bits in unsigned long (not used)
`  - ( (unsinged long) -1 ) >> emptyBits
`    o shifts -1 till all unsed bits are set to 0, and all
`      used bits are set to 1
`     
*/

/*-------------------------------------------------------\
| Fun02: ntIndexToKmer_kmerBit
|   - adds a nucleotide (in index format
|    [seqToIndex_alnSet]) to a kmer
| Input:
|   - ntMac:
|     o nucleotide to add
|   - kmerMac:
|     o kmer with previous nucleotides
|   - maskMac:
|     o mask to clear extr bits with (use mkMask_kmerBit)
| Output:
|   - Returns:
|     o kmer with added bits (does not modify kmerMac)
\-------------------------------------------------------*/
#define ntIndexToKmer_kmerBit(ntMac, kmerMac, maskMac) ( ( ((kmerMac) << def_bitsPerKmer_kmerBit) | (alnNtTo_kmerBit[ (unsigned char) (ntMac) ]) ) & (maskMac) )
/*Logic:
`   - mkRoom: kmerMac << def_bitsPerKmer_kmerBit:
`     o add room for new bit
`   - getBit: alnNtTo_kmerBit[(unsigned char) ntMac]:
`     o convert nucleotide index to bits for kmer
`   - kmer: mkRoom | getBit:
`     o add converted nucleotide index to kmer
`   - kmer & maskMac:
`     o clear any extra bits (not in kmer)
*/

/*-------------------------------------------------------\
| Fun03: ntBitToKmer_kmerBit
|   - adds a nucleotide bit (by alNtTo_kmerBit) to a kmer
| Input:
|   - ntBitMac:
|     o nucleotide bit to add; covert by alnNtTo_kmerBit
|   - kmerMac:
|     o kmer with previous nucleotides
|   - maskMac:
|     o mask to clear extr bits with (use mkMask_kmerBit)
| Output:
|   - Returns:
|     o kmer with added bits (does not modify kmerMac)
\-------------------------------------------------------*/
#define ntBitToKmer_kmerBit(ntBitMac, kmerMac, maskMac) ( ( ((kmerMac) << def_bitsPerKmer_kmerBit) | ((unsigned char) (ntBitMac)) ) & (maskMac) )
/*Logic:
`   - mkRoom: kmerMac << def_bitsPerKmer_kmerBit:
`     o add room for new bit
`   - kmer: mkRoom | ntBitMac:
`     o add converted nucleotide index to kmer
`   - kmer & maskMac:
`     o clear any extra bits (not in kmer)
*/

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
