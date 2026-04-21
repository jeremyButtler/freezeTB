/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' dirMatrix SOF: Start Of File
'   - holds functions for dealing with the dirMatrix
'     returned by water and needle
'   o header:
'     - forward declerations and guards
'   o .h st01: alnMatrixStruct
'     - Holds the direction matrix and best score(s) for a
'       single aligment
'   o fun01: blank_dirMatrix
'     - blanks all score, index, and length variables in a
'       dirMatrix structure
'   o fun02: init_dirMatrix
'     - initializes a dirMatrix structure
'   o fun03: freeStack_dirMatrix
'     - frees heap allocated variables in a dirMatrix
'       structure
'   o fun04: freeHeap_dirMatrix
'     - frees a dirMatrix structure
'   o fun05: getAln_dirMatrix
'     - gets a sam file entry (alignment) from a direction
'       matrix (inside the dirMatrix structure)
'   o fun06: getCig_dirMatrix
'     - gets a cigar for an alignment
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declerations and guards
\-------------------------------------------------------*/

#ifndef DIRECTION_MATRIX_H
#define DIRECTION_MATRIX_H

struct samEntry;
struct seqST;
struct alnSet;

/*-------------------------------------------------------\
| St01: dirMatrix
|  - Holds the direction matrix and best score(s) for a
|    single aligment
\-------------------------------------------------------*/
typedef struct dirMatrix
{ /*alnStruct*/
  signed char *dirMatrixSC; /*directional matrix*/
  signed long lenMatrixSL;  /*size of directional matrix*/

  signed long scoreSL;      /*best score in alignment*/
  signed long indexSL;      /*index for best score*/

  signed long *scoreArySL;  /*score array used to score*/
  signed long lenScoreSL;   /*length of score array*/

  signed long refLenSL;     /*reference length*/
  signed long refOffsetSL;  /*first ref base to align*/
  signed long refEndSL;     /*last ref base to align*/

  signed long qryLenSL;     /*length of query*/
  signed long qryOffsetSL;  /*first query base to align*/
  signed long qryEndSL;     /*last query base to align*/

  signed char errSC;        /*holds any errors*/
}dirMatrix;

/*-------------------------------------------------------\
| Fun01: blank_dirMatrix
|   - blanks all score, index, and length variables in a
|     dirMatrix structure
| Input:
|   - dirMatrixSTPtr:
|     o a pointer to a dirMatrix to blank
| Output:
|   - Sets:
|     o all score, length, and index variables are set to
|       0 (direction matrix is not touched)
\-------------------------------------------------------*/
void
blank_dirMatrix(
  struct dirMatrix *dirMatrixSCSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_dirMatrix
|   - initializes a dirMatrix structure
| Input:
|   - dirMatrixSTPtr:
|     o a pointer to a dirMatrix structure initialize
| Output:
|   - Sets:
|     o all variables in matrixSTPtr to 0
\-------------------------------------------------------*/
void
init_dirMatrix(
  struct dirMatrix *dirMatrixSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_dirMatrix
|   - frees heap allocated variables in a dirMatrix
|     structure
| Input:
|   - matrixSTPtr
|     o pointer to dirMatrix structure with variables to
|       free
| Output:
|   - Frees:
|     o dirMatrix->dirMatrixSC
|   - Sets:
|     o all non-freeded variables to 0
\-------------------------------------------------------*/
void
freeStack_dirMatrix(
   struct dirMatrix *matrixSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_dirMatrix
|   - Frees a dirMatrix structure
| Input:
|   - matrixSTPtr
|     o pointer to a dirMatrix structure to free
| Output:
|   - Frees:
|     o matrixSTPtr
\-------------------------------------------------------*/
void
freeHeap_dirMatrix(
   struct dirMatrix *matrixSTPtr
);

/*-------------------------------------------------------\
| Fun05: getAln_dirMatrix
|   - gets a sam file entry (alignment) from a direction
|     matrix (inside the dirMatrix structure)
| Input:
|   - matrixSTPtr
|     o pointer to a dirMatrix structure to get alignment
|       from
|   - indexSL:
|     o index of last base in the alignment
|     o 0 to use index from matirxSTPtr
|   - revBl:
|     o 1: reverse alignment (sam flag is 16)
|     o 0: foward alignment (sam flag is 0)
|   - qrySTPtr:
|     o pointer to a seqST with the query sequence
|   - refSTPtr:
|     o pointer to a seqST with the reference sequence
|   - samSTPtr:
|     o pointer to a samEntry struct to hold the alignment
|   - numAnonUI:
|     o pointer to unsigned in to hold the number of
|       anonymous bases (matches only)
|   - alnSetSTPtr:
|     o pointer to alnSet structure with the match matrix
| Output:
|   - Modifies:
|     o samSTPtr to have the aligned sequence
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error (only error possible)
\-------------------------------------------------------*/
signed char
getAln_dirMatrix(
   struct dirMatrix *matrixSTPtr,
   signed long indexSL,
   signed char revBl,
   struct seqST *qrySTPtr,
   struct seqST *refSTPtr,
   struct samEntry *samSTPtr,
   unsigned int *numAnonUI,
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun06: getCig_dirMatrix
|   - gets a cigar for an alignment
| Input:
|   - matrixSTPtr
|     o pointer to a dirMatrix structure to get alignment
|       from
|   - indexSL:
|     o index of last base in the alignment
|     o 0 to use index from matirxSTPtr
|   - revBl:
|     o 1: reverse alignment (sam flag is 16)
|       - this means I had to reverse complement the
|         reference sequence
|     o 0: foward alignment (sam flag is 0)
|   - qrySTPtr:
|     o pointer to a seqST with the query sequence
|   - refSTPtr:
|     o pointer to a seqST with the reference sequence
|   - cigTypeStr:
|     o pointer to c-string with cigar entry type array
|   - cigArySI:
|     o pointer to signed int array with the length of
|       each cibar entry
|   - cigPosUI:
|     o position at in cigar
|   - lenCigUI:
|     o pointer to unsigned int with length of the cigar
|       buffer
|   - refStartUI:
|     o unsigned int pointer to point to first reference
|       base in cigar
|   - numAnonUI:
|     o pointer to unsigned in to hold the number of
|       anonymous bases (matches only)
|   - numMaskUI:
|     o pointer to unsigned long to hold number of
|       maksed bases
|   - delAtEndBl:
|     o 1: add deltions if reference is short at end
|     o 0: ignore
|   - alnSetSTPtr:
|     o pointer to alnSet structure with the match matrix
| Output:
|   - Modifies:
|     o cigTypeStr to have the cigar entry types
|     o cigArySI to have the length of each cigar entry
|     o lenCigUI if cigTypeStr and cigArySI needed to be
|       resized
|     o refStartUI to have first reference base in cigar
|     o numAnonUI to have number of matching anonymous
|       bases
|     o numMaskUI to have number of masked bases
|   - Returns:
|     o new position in cigar
|     o -1 for memory error (only error possible)
\-------------------------------------------------------*/
signed long
getCig_dirMatrix(
   struct dirMatrix *matrixSTPtr,
   signed long indexSL,
   signed char revBl,
   struct seqST *qrySTPtr,
   struct seqST *refSTPtr,
   signed char **cigTypeStr,
   signed int **cigArySI,
   unsigned int cigPosUI,
   unsigned int *lenCigUI,
   unsigned int *refStartUI,
   unsigned int *numAnonUI,
   unsigned int *numMaskUI,
   signed char delAtEndBl,
   struct alnSet *alnSetSTPtr
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
