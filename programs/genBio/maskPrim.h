/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' maskPrim SOF: Start Of File
'   - holds functions for primer masking or trimming with
'     sam files
'   o header:
'     - defined variables and guards
'   o .h fun01: blankFlag_maskPrim
'     - blanks all flags in an primFlag uint
'   o .h fun02: setDir_maskPrim
'     - sets the direction flag for an primer
'   o .h fun03: setPair_maskPrim
'     - sets the paired flag for an primer
'   o .h fun04: setMateIndexPrimIndex
'     - sets the index of the other primer
'   o .h fun05: getDir_maskPrim
'     - gets the primer direction from an primer flag uint
'   o .h fun06: getPair_maskPrim
'     - gets if the primer is to be treated as an pair
'   o .h fun07: getMateIndex_maskPrim
'     - gets the other primers index in the pair
'   o fun08: maskPrim
'     - masks the primers in the sam sequence
'   o .c fun09: swapPrimCoords
'     - swaps two primer coordinates values around and
'       updates the index's
'   o fun09: sortCoords_maskPrim
'     - Sorts the primer coordiantes of a primer flag
'       array setup. Order is least to greates by first
'       starting positon, then ending position
'   o fun11: getCoords_maskPrim
'     - gets primer coordinates and flags from an file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined varaibles and guards
\-------------------------------------------------------*/

#ifndef SAM_PRIMER_MASK_H
#define SAM_PRIMER_MASK_H

struct samEntry;

/*Do not change these*/
#define def_reverse_maskPrim 1
#define def_pair_maskPrim 2
#define def_clearIndex_maskPrim 0x00000003
#define def_shiftIndex_maskPrim 2

#define def_fileErr_maskPrim 2
#define def_emptyFileErr_maskPrim 4
#define def_memErr_maskPrim 64

/*-------------------------------------------------------\
| Fun01: blankFlag_maskPrim
|   - blanks all flags in an primFlag uint
| Input:
|   - flagUI:
|     o unsigned int to blank
| Output:
|   - Modifies:
|     o flagUI to be 0
\-------------------------------------------------------*/
#define blankFlag_maskPrim(flagUI) ((flagUI) = 0)

/*-------------------------------------------------------\
| Fun02: setDir_maskPrim
|   - sets the direction flag for an primer
| Input:
|   - flagUI:
|     o unsigned int to hold the flags and indexes
|   - revBl:
|     o 1 if this is an reverse complement primer
|     o 0 if this is an foward primer
| Output:
|   - Modifies:
|     o flagUI to have the input direction
|       def_reverse_maskPrim
\-------------------------------------------------------*/
#define setDir_maskPrim(flagUI, revBl) ( (flagUI) = ( (flagUI) & ~def_reverse_maskPrim ) | ( def_reverse_maskPrim & -(revBl) ) )
   /*Logic
   `  - clear: flagUI & ~def_reverse_maskPrim:
   `    o clears the reverse primer flag
   `  - clear | (def_reverse_maskPrim & -(revBl))
   `    o adds reverse flag in or 0 if not reverse
   */


/*-------------------------------------------------------\
| Fun03: setPair_maskPrim
|   - sets the paired flag for an primer
| Input:
|   - flagUI:
|     o unsigned int to hold the flags and indexes
|   - pairBl:
|     o 0 if this primer should not be treated as an pair
|     o 1 if this primer is treated as an pair
| Output:
|   - Modifies:
|     o flagUI to have the def_pair_maskPrim flag set
|       to 1 if paired or 0 if false
\-------------------------------------------------------*/
#define setPair_maskPrim(flagUI, pairBl) ( (flagUI) = ( (flagUI) & ~def_pair_maskPrim ) | ( def_pair_maskPrim & -(pairBl) ) )
   /*Logic
   `  - clear: flagUI & ~def_pair_maskPrim:
   `    o clears the pair flag
   `  - clear | (def_pair_maskPrim & -(pairBl))
   `    o adds new flag in
   */

/*-------------------------------------------------------\
| Fun04: setMateIndexPrimIndex
|   - sets the index of the other primer
| Input:
|   - flagUI:
|     o unsigned int to hold the forward index
|   - indexUI:
|     o index of the foward primer in the primer array
| Output:
|   - Modifies:
|     o flagUI to have the foward index (after the flags)
\-------------------------------------------------------*/
#define setMateIndex_maskPrim(flagUI, indexUI) ( (flagUI) = (flagUI & def_clearIndex_maskPrim) | ( (indexUI) << def_shiftIndex_maskPrim ) )
   /*Logic
   `  - clear: flagUI & def_clearIndex_maskPrim
   `    o clears old mate index from the flag
   `  - clear | (indexUI << def_shiftIndex_maskPrim)
   `    o adds new primer mates index into flag
   */

/*-------------------------------------------------------\
| Fun05: getDir_maskPrim
|   - gets the primer direction from an primer flag (uint)
| Input:
|   - flagUI:
|     o unsigned int with primer direction to get
|       index from
| Output:
|   - Returns:
|     o 0 for an foward primer
|     o 1 for an reverse primer
\-------------------------------------------------------*/
#define getDir_maskPrim(flagUI) ((flagUI) & def_reverse_maskPrim)

/*-------------------------------------------------------\
| Fun06: getPair_maskPrim
|   - gets if the primer is to be treated as an pair
| Input:
|   - flagUI:
|     o unsigned int with primer direction to get
|       index from
| Output:
|   - Returns:
|     o 0 if this is not an pair
|     o 1 if this is an pair
\-------------------------------------------------------*/
#define getPair_maskPrim(flagUI) ((flagUI) & def_pair_maskPrim)

/*-------------------------------------------------------\
| Fun07: getMateIndex_maskPrim
|   - gets the other primers index in the pair
| Input:
|   - flagUI:
|     o unsigned int with pimer flags to get the primers
|       mate from
| Output:
|   - Returns:
|     o An short with the forward primers index
\-------------------------------------------------------*/
#define getMateIndex_maskPrim(flagUI) ((flagUI) >> 2)

/*-------------------------------------------------------\
| Fun08: maskPrim
|   - masks the primers in the sam sequence
| Input:
|   - samSTVoidPtr:
|     o pointer to an sam structure to mask primers in
|   - primStartAryUI:
|     o uint array of primer starting coordiantes
|     o starting position on the reference sequence
|   - primEndAryUI:
|     o uint array of primer ending coordiantes
|     o ending position on the reference sequence
|   - primFlagAryUI:
|     o int array with bits filled for the flags and
|       coordinates
|       - first two bits have the direction,
|         o def_reverse_maskPrim (1) if is an reverse
|           primer
|         o def_pair_maskPrim (2) if you want to only
|           mask primers that have pairs instead off any
|           primer
|       - next 15 bits (3 to 17) index of forward primer
|       - last 15 bits (18 to 32) index of reverse primer
|   - numPrimSI:
|     o number of primers in primStartAryUI and primEndUI
|   - fudgeSI:
|     o sint with the number of bases before (start) or
|       after (end) to 
|       - -1 mask any detected primer
|       - 0 is no bases before start or after end
|       - > 0 Allow 
|   - maskSC:
|     o character to mask with
| Output:
|   - Modifies:
|     o samSTVoidPtr to have any detected primers masked
|   - Returns:
|     o 0 if no problems
|     o -1 for no detected primers
|     o (primer index << 1) | 1 if primer found but not
|        masked. This can only happen for fudge lengths
|        out of bounds or incomplete primer pairs
\-------------------------------------------------------*/
signed int
maskPrim(
   struct samEntry *samSTPtr,
   unsigned int *primStartAryUI,
   unsigned int *primEndAryUI,
   unsigned int *primFlagAryUI,
   signed int numPrimSI,
   signed int fudgeSI, /*Max fudge size*/
   signed char maskSC
);

/*-------------------------------------------------------\
| Fun09: sortCoords_maskPrim
|  - sorts the primer coordiantes of a primer flag array
|    setup. Order is least to greates by first starting
|    positon, then ending position
| Input:
|  - startAryUI:
|    o starting coordinates for the primers
|  - endAryUI:
|    o ending coordinates for the primers
|  - flagAryUI:
|    o array full of primFlags with indexs to keep up to
|      date
|  - lenAryUI:
|    o Length of the array
| Output:
|  - Modifies:
|    o startAry array to be sorted form least to greatest
|    o endAry to be kept in order with firstAry
|    o flagAry to be kept in order with firstAry and have
|      any index's updated
\-------------------------------------------------------*/
void
sortCoords_maskPrim(
   unsigned int *startAryUI,
   unsigned int *endAryUI,
   unsigned int *flagAryUI,
   unsigned int lenAryUI 
);

/*-------------------------------------------------------\
| Fun11: getCoords_maskPrim
|   - gets primer coordinates and flags from an file
| Input:
|   o primFileStr:
|     - c-string with name of file to get primer
|       coordinates from
|       o ignored\tignored\tpair\tstart\tend\tstart\tend
|         - pair: 1, P, T, or + if primers paired
|         - start: start coordinate of foward/reverse
|           primer
|         - end: end coordinate of foward/reverse primer
|   o startAryUI:
|     - pointer to an unsigned int pointer to hold the
|       starting coordinates for each primer
|   o endAryUI:
|     - pointer to an unsigned int pointer to hold the
|       ending coordinates for each primer
|   o flagAryUI:
|     - pointer to an unsigned int pointer to hold the
|       flags and pair index for each primer
|   o errSL:
|     - pointer to unsigned long to hold the error
|       message
| Output:
|   - Modifies:
|     o startAryUI to have the starting coordiantes
|     o endAryUI to have the ending coordiantes
|     o flagAryUI to have the direction and pair flags
|       and the index of the other primer
|     o errSL to hold the error
|       - 0 for no errors
|       - def_memErr_maskPrim for an memory error
|       - (siLine << 8) | def_fileErr_maskPrim for file
|         errors
|       - def_emptyFileErr_maskPrim for empty file
|  - Returns:
|    o 0 for errors
|    o number of primers extracted
\-------------------------------------------------------*/
signed int
getCoords_maskPrim(
   signed char *primFileStr,
   unsigned int **startAryUI,
   unsigned int **endAryUI,
   unsigned int **flagAryUI,
   signed long *errSL
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
