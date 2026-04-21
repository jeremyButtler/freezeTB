/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' codonFun SOF: Start Of File
'   - has codon table and functions to support the
'     codon table
'   o header:
'     - guards
'   o fun01: codonToAA_codonFun
'     - converts codon into an amino acid
'   o fun02: revCodonToAA_codonFun
'     - reverses complements input codon & gets amino acid
'   o fun03: bacStartCode_codonFun
'     - checks to see if input bases is a bacterial start
'       codon (convert nt with ntTo2Bit.h)
'   o fun04: bacStart_codonFun
'     - checks if codon (no conversion) is a start codon
'   o fun05: bactRevStart_codonFun
'     - checks if codon (no conversion) is a start codon
'       for a reverse complement sequence
'   o fun06: aaTripToChar_codonFun
'     - converts a three letter amino acid idenity to its
'       single letter amino acid identity
'   o fun07: seqToAA_codonFun
'     - converts nucleotide sequence to amino acids
'   o fun08: revSeqToAA_codonFun
'     - converts a reverse complement nucleotide sequence
'       to amino acid sequence
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef CODON_FUNCTIONS_H
#define CODON_FUNCTIONS_H

/*keep these in sync with codonFun and as negative*/
#define def_unkownNt_codonFun -1
#define def_incomplete_codonFun -2

/*-------------------------------------------------------\
| Fun01: codonToAA_codonFun
|   - converts codon into an amino acid
| Input:
|   - firstNtSC:
|     o first base in codon
|   - secNtSC:
|     o second base in codon
|   - thirdNtSC:
|     o last base in codon
| Output:
|   - Returns:
|     o amino acid of the input codon
\-------------------------------------------------------*/
signed char
codonToAA_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
);

/*-------------------------------------------------------\
| Fun02: revCodonToAA_codonFun
|   - reverses complements input codon & finds amino acid
| Input:
|   - firstNtSC:
|     o first base in the foward codon, but third (last)
|       base in the reverse complement codon
|   - secNtSC:
|     o second base in forward codon, but second (middle)
|       base in the reverse complement codon
|   - thirdNtSC:
|     o last base in forward codon, but first base in the
|       reverse complement codon
| Output:
|   - Returns:
|     o reverse complement amino acid of the input
|       codon
\-------------------------------------------------------*/
signed char
revCodonToAA_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
);

/*-------------------------------------------------------\
| Fun03: bacStartCode_codonFun
|   - checks to see if input bases is a bacterial start
|     codon (convert nt with ntTo2Bit.h)
| Input:
|   - firstNtSC:
|     o first base to check; it needs to be the output of
|       an base to code look up table
|   - secBaseC:
|     o second base to check; it needs to be the output
|       of an base to code look up table
|   - thirdBaseC:
|     o third base to check; it needs to be the output
|       of an base to code look up table
| Output:
|   - Returns:
|     o 1 if this is an bacterial start codon
|     o 0 if this is not an bacterial start codon
\-------------------------------------------------------*/
signed char
bactStartCode_codonFun(
   unsigned char firstNtSC,
   unsigned char secNtSC,
   unsigned char thirdNtSC
);

/*-------------------------------------------------------\
| Fun04: bacStart_codonFun
|   - checks if codon (no conversion) is a start codon
| Input:
|   - firstNtSC:
|     o first base to check
|   - secBaseC:
|     o second base to check
|   - thirdBaseC:
|     o third base to check
| Output:
|   - Returns:
|     o 1 if this is a bacterial start codon
|     o 0 if this is not a bacterial start codon
\-------------------------------------------------------*/
signed char
bactStart_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
);

/*-------------------------------------------------------\
| Fun05: bactRevStart_codonFun
|   - checks if codon (no conversion) is a start codon
|     for a reverse complement sequence
| Input:
|   - firstNtSC:
|     o first base to check (last in codon)
|   - secBaseC:
|     o second base to check (middle in codon)
|   - thirdBaseC:
|     o third base to check (first in codon)
| Output:
|   - Returns:
|     o 1 if this is a reverse bacterial start codon
|     o 0 if this is not a reverse bacterial start codon
\-------------------------------------------------------*/
signed char
bactRevStart_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
);

/*-------------------------------------------------------\
| Fun06: aaTripToChar_codonFun
|   - converts a three letter amino acid idenity to its
|     single letter amino acid identity
| Input:
|   - codonStr:
|     o c-string with the three letter amino acid to
|       convert
| Output:
|   - Returns:
|     o one letter code for the amino acid
|     o 0 for an invalid code
\-------------------------------------------------------*/
signed char
aaTripToChar_codonFun(
   signed char *codonStr
);

/*--------------------------------------------------------\
| Fun07: seqToAA_codonFun
|  - converts a nucleotide sequence to amino acid sequence
| Input:
|  - seqStr: 
|    o c-string with the sequence to convert
|  - aaStr:
|    o c-string to hold the converted sequence, must be
|      at least sequence / 3 bases long
|  - startSL:
|    o position to start translation at
|    o use 0 for all sequence
|  - endSL:
|    o position to end translation at
|    o use 0 for all sequence
| Output:
|  - Modifies:
|    o aaStr to hold the amino acid sequence
|      * on nucleotide errors, a '\0' is added after last
|        correct call
|  - Returns:
|    o length (> 0) of returned ammino acid sequence
|    o def_unkownNt_codonFun (< 0) for sequence errors
|    o def_incomplete_codonFun (< 0) if had partial end
\--------------------------------------------------------*/
signed long
seqToAA_codonFun(
   signed char *seqStr,
   signed char *aaStr,
   signed long startSL,
   signed long endSL
);

/*-------------------------------------------------------\
| Fun08: revSeqToAA_codonFun
|  - converts a reverse complement nucleotide sequence to
|    amino acid sequence
| Input:
|  - seqStr: 
|    o c-string with the sequence to convert
|  - aaStr:
|    o c-string to hold the converted sequence, must be
|      at least sequence / 3 bases long
|  - startSL:
|    o position to start translation at
|    o use 0 for all sequence
|  - endSL:
|    o last base in sequence (first base to traslate)
|    o use 0 for all sequence
| Output:
|  - Modifies:
|    o aaStr to hold the amino acid sequence
|      * on nucleotide errors, a '\0' is added after last
|        correct call
|  - Returns:
|    o length (> 0) of returned ammino acid sequence
|    o def_unkownNt_codonFun (< 0) for sequence errors
|    o def_incomplete_codonFun (< 0) if had partial end
\-------------------------------------------------------*/
signed long
revSeqToAA_codonFun(
   signed char *seqStr,
   signed char *aaStr,
   signed long startSL,
   signed long endSL
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
