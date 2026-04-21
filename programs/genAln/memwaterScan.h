/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' memwaterScan SOF: Start Of Functions
'   - has memory efficent Waterman Smith alignment scan
'     (best alignment for each reference/query base)
'   o header:
'     - included libraries
'   o .h st01: aln_memwaterScan
'     - stores the alignment results from a waterman scan
'   o fun01: blank_aln_memwaterScan
'     - blanks (sets non-array values to 0) an
'   o fun02: init_aln_memwaterScan
'     - initializes (sets arrays to 0 + blanks) an
'       aln_memwaterScan structure
'   o fun03: freeStack_aln_memwaterScan
'     - frees the variables in an aln_memwaterScan struct
'   o fun04: freeHeap_aln_memwaterScan
'     - frees an aln_memwaterScan struct
'   o fun05: getCoord_aln_memwaterScan
'     - gets the coordinates for input base index for an
'       aln_memwaterScan structure
'   o fun07: refCoordSort_aln_memwaterScan
'     - sorts best bases alignments by reference start,
'       query start, reference end (closest first), then
'       and query end (closest first)
'     - all negatives (non-alignment found) are pushed to
'       the end
'   o fun08: filter_memwaterScan
'     - removes low scoring alignments and alignments that
'       are nested alignments
'   o fun09: merge_aln_memwaterScan
'     - merges one aln_memwaterScan struct into another
'       aln_memwaterScan struct
'   o fun10 memwaterScan:
'     - performs a memory efficent Smith Waterman scan
'       (keep best alignment for each query/reference base)
'       alignment on a pair of sequences
'   o fun11: simple_memwaterScan
'     - performs a memory efficent Smith Waterman scan
'       (keep best alignment for each query/reference
'       base) alignment on a pair of sequences
'     - simple means no settings or seqST structure used
'       and no match matrix
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef MEMORY_EFFICENT_WATERMAN_MATRIX_SCAN_H
#define MEMORY_EFFICENT_WATERMAN_MATRIX_SCAN_H

struct seqST;
struct alnSet;

/*-------------------------------------------------------\
| ST01: aln_memwaterScan
|   - stores the alignment results from a waterman scan
\-------------------------------------------------------*/
typedef struct aln_memwaterScan
{
   /*length of aligned sections of the sequences;
   `  ref=reference, qyr=query
   */
   signed int refLenSI;
   signed int qryLenSI;

   /*offset (starting alignment position) of sequences*/
   signed int refOffsetSI;
   signed int qryOffsetSI;

   /*alignment coordiantes (has both reference and query
   `   coordiantes, convert to individual coordinates with
   `   seqToIndex) of the best alignment for each
   `   reference (0 to refLenSI - 1) and query
   `   (refLenSI to refLenSI + qryLen - 1)
   `   base
   ` Also the best scores for each bases alignment
   */
   signed long *startArySL;
   signed long *endArySL;
   signed long *scoreArySL;
   signed long outSizeSL;
   signed long outLenSL;

   /*general variables used by memwaterScan and its
   `  supporting functions
   */
   signed long *scoreRowSL; /*scoring row*/
   signed long *indexRowSL; /*start index of each row*/
   signed char *dirRowSC;   /*direction row*/
   signed int rowSizeSI;    /*maximum bytes before need to
                            `  resize scoreRowSL,
                            `  indexRowSL, dirRowSC,
                            `  refStartArySL, refEndArySL,
                            `  and refScoreArySL
                            */
}aln_memwaterScan;

/*-------------------------------------------------------\
| Fun01: blank_aln_memwaterScan
|   - blanks (sets non-array values to 0) an
|     aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to blank
| Output:
|   - Modifies:
|     o sets refLenSI, qryLenSI, refOffsetSI, qryOffsetSI
|       in alnsTPtr to 0
\-------------------------------------------------------*/
void
blank_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_aln_memwaterScan
|   - initializes (sets arrays to 0 + blanks) an
|     aln_memwaterScan structure
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to initialize
| Output:
|   - Modifies:
|     o sets all values to 0
\-------------------------------------------------------*/
void
init_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_aln_memwaterScan
|   - frees the variables in an aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer with variable to
|       free
| Output:
|   - Modifies:
|     o frees all arrays and initializes
\-------------------------------------------------------*/
void
freeStack_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_aln_memwaterScan
|   - frees an aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to free
| Output:
|   - Modifies:
|     o frees alnSTPtr; you must set the pointer to 0/null
\-------------------------------------------------------*/
void
freeHeap_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun05: getCoord_aln_memwaterScan
|   - gets the coordinates for input base index for an
|     aln_memwaterScan structure
| Input:
|   - baseSI:
|     o base to get coordinates for
|   - refStartSIPtr:
|     o signed int pointer that gets the reference start
|   - refEndSIPtr:
|     o signed int pointer that gets the reference end
|   - qryStartSIPtr:
|     o signed int pointer that gets the query start
|   - qryEndSIPtr:
|     o signed int pointer that gets the query end
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer with coordinates
| Output:
|   - Modifies:
|     o refStartSIPtr to have reference starting position
|     o refEndSIPtr to have reference ending position
|     o qryStartSIPtr to have query starting position
|     o qryEndSIPtr to have query ending position
|   - Returns:
|     o score for success
|     o 0 if no position at index
|     o -1 for out of bounds error
\-------------------------------------------------------*/
signed char
getCoord_aln_memwaterScan(
   signed long baseSL,        /*index to grab*/
   signed int *refStartSIPtr, /*gets reference start*/
   signed int *refEndSIPtr,   /*gets reference end*/
   signed int *qryStartSIPtr, /*gets query start*/
   signed int *qryEndSIPtr,   /*gets query end*/
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun06: swap_memwaterScan
|   - swaps position in reference array
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to swap positions
|   - firstSI:
|     o first index to swap
|   - secSI:
|     o second index to swap
| Output:
|   - Modifies:
|     o scoreArySL, startArySL, endArySL in alnSTPtr to
|       have firstSI and secSI index's swapped
\-------------------------------------------------------*/
void
swap_memwaterScan(
   struct aln_memwaterScan *alnSTPtr,
   signed int firstSI,
   signed int secSI
);

/*-------------------------------------------------------\
| Fun07: refCoordSort_aln_memwaterScan
|   - sorts best bases alignments by reference start,
|     query start, reference end (closest first), then
|     and query end (closest first)
|   - all negatives (non-alignment found) are pushed to
|     the end
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pionter to sort
| Output:
|   - Modifies:
|     o qryStartArySL in alnSTPtr to be sorted by
|       reference coodrinate
|     o qryEndArySL and qryScoreArySL in alnSTPtr to be
|       kept in sync with qryStartArySL
\-------------------------------------------------------*/
void
refCoordSort_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
);

/*-------------------------------------------------------\
| Fun08: filter_memwaterScan
|   - removes low scoring alignments and alignments that
|     are nested alignments
| Input:
|   - alnSTPtr:
|     o pointer to aln_memwaterScan struct with alignments
|       to filter
|   - minScoreSL:
|     o minimum score for an alignment
|   - minScoreTwoSL:
|     o second minimum score for an alignment
|       * use when you are putting a percent minimum score
|         for minScoreSL
|       * allows for a hard cutoff and a percent cuttoff
| Output:
|   - Modifies:
|     o startArySL, endArySL, and scoreArySL in alnSTPtr
|       to only have high scoring, non-nested alignments
|     o outLenSL to have the new number of alignments
|   - Returns:
|     o number of kept alignments
\-------------------------------------------------------*/
signed long
filter_memwaterScan(
   struct aln_memwaterScan *alnSTPtr,
   signed long minScoreSL,
   signed long minScoreTwoSL
);

/*-------------------------------------------------------\
| Fun09: merge_aln_memwaterScan
|   - merges one aln_memwaterScan struct into another
|     aln_memwaterScan struct
| Input:
|   - mergeSTPtr:
|     o aln_memwaterScan struct to merge into the final
|       structure
|   - outSTPtr:
|     o aln_memwaterScan struct to merge mergeSTPtr into
|   - minScoreSL:
|     o minumum score to keep score in mergeSTPtr
| Output:
|   - Modifies:
|     o variables in outSTPtr to include variables in
|       mergeSTPtr and outSTPtr
|   - Returns:
|     o 0 for success
|     o -1 if reference lengths differ (no way to merge)
|     o -2 for memory errors
\-------------------------------------------------------*/
signed char
merge_aln_memwaterScan(
   struct aln_memwaterScan *mergeSTPtr,
   struct aln_memwaterScan *outSTPtr,
   signed long minScoreSL
);

/*-------------------------------------------------------\
| Fun10: memwaterScan
|   - performs a memory efficent Smith Waterman scan
|     (keep best alignment for each query/reference base)
|     alignment on a pair of sequences
| Input;
|   - qrySeqSTPtr:
|     o pointer to an seqST with the query sequence and
|       index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - refSeqSTPtr:
|     o pointer to an seqST with the reference sequence
|       and index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - alnSTPtr:
|     o pointer to aln_memwaterScan structure to hold the
|       results of the alignment
|   - settings:
|     o pointer to an alnSet structure with the gap open,
|       gap extend, and scoring matrix for the alingment
| Output:
|  - Modifies:
|    o variables in alnSTPtr to have the new alignment
|  - Returns:
|    o score for aligment
|    o negative number for memory errors
\-------------------------------------------------------*/
signed long
memwaterScan(
   struct seqST *qrySTPtr, /*query sequence and data*/
   struct seqST *refSTPtr, /*ref sequence and data*/
   struct aln_memwaterScan *alnSTPtr,/*gets alignment*/
   struct alnSet *settings
);

/*-------------------------------------------------------\
| Fun11: simple_memwaterScan
|   - performs a memory efficent Smith Waterman scan
|     (keep best alignment for each query/reference base)
|     alignment on a pair of sequences
|   - simple means no settings or seqST structure used and
|     no match matrix
| Input;
|   - qrySeqStr:
|     o c-string with query sequence
|   - qryLenSI:
|     o length of qrySeqStr (index 1)
|   - refSeqStr:
|     o c-string with reference sequence
|   - refLenSI:
|     o length of refSeqStr (index 1)
|   - alnSTPtr:
|     o pointer to aln_memwaterScan structure to hold the
|       results of the alignment
| Output:
|  - Modifies:
|    o variables in alnSTPtr to have the new alignment
|  - Returns:
|    o score for aligment
|    o negative number for memory errors
\-------------------------------------------------------*/
signed long
simple_memwaterScan(
   signed char *qrySeqStr,   /*query sequence*/
   signed int lenQrySI,      /*length of query sequence*/
   signed char *refSeqStr,   /*reference sequence*/
   signed int lenRefSI,      /*reference sequence length*/
   struct aln_memwaterScan *alnSTPtr /*gets alignment*/
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
