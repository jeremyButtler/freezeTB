/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' alnSet SOF: Start Of File
'  - holds settings structures and supporting functions
'    for my pairwise aligners
'  o header:
'    - header guards and defined variables
'  o .h st01 alnSet:
'     o Holds settings for my alignment program
'  o .h fun01 setScore_alnSet:
'    - Sets the score for a base pair (reference/query)
'  o .h fun02 setMatch_alnSet:
'    - Sets if two bases are a match or mismtach
'  o fun03 freeStack_alnSet:
'    o Frees variables inside alnSet
'  o fun04 freeHeap_alnSet:
'    o Frees an alnSet structure (and sets to 0)
'  o .h fun05 getScore_alnSet:
'    - Get the score for a pair of bases from an alignment
'  o .h fun06 getMatch_alnSet:
'    - Check if two bases were a match or mismatch
'  o fun07 readScoreFile_alnSet
'     - Reads in a file of scores for a scoring matrix
'  o fun08 readMatchFile_alnSet:
'    - Reads in a file of matches
'  o fun09 seqToIndex_alnSet:
'    - Converts a sequence to a look up table index
'  o fun10 indexToSeq_alnSet:
'    - Converts a sequence of lookup indexs back into
'      uppercase characters (a-z)
'  o fun11: revCmpIndex_alnSet
'    - reverse complement sequence in lookup index format
'      (from seqToIndex_alnSet)
'  o fun12: changeGap_alnSet
'    - changes the gap penalties in an alnSet structure
'  o fun14 init_alnSet:
'    - Set all values in altSet (alingment settings)
'      structure to defaults
'  o fun13: maxScore_alnSet
'    - finds maximum score possible for a sequence
'  o fun15: pDefMatchMatrix_alnSet
'    - print out the default match matrix
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - header guards and defined variables
\-------------------------------------------------------*/

#ifndef ALIGNMENT_SETTINGS_STRUCT_H
#define ALIGNMENT_SETTINGS_STRUCT_H

#define def_lenCol_alnSet 27 /*one column/row of matrix*/

/*For seq conversion to index functions*/
#define def_ntToCode_alnSet (1 | 2 | 4 | 8 | 16)
  /*clear 64 bit and case*/

/*-------------------------------------------------------\
| ST01: alnSet
| Use: Holds settings for my alignment program
\-------------------------------------------------------*/
typedef struct alnSet
{ /*alnSet*/
   signed long minScoreSL; /*Min score to keep alignment*/

   /*for printing alignments*/
   unsigned int lineWrapUI;/*length of line before break*/
   signed char pFullAlnBl;/*print masked bases*/
   signed char pBasePosBl;/*print start & end coordinate*/


   /*scoring variables*/
   signed short gapSS;    /*Penalty for starting indel*/
   signed short extendSS; /* Penalty for extending indel*/

   signed short delArySS[4]; /*gap penalties*/
   signed short insArySS[4]; /*gap penalties*/

   signed short scoreMatrixSS[
         def_lenCol_alnSet
      ][
         def_lenCol_alnSet
      ]; /*scoring matrix*/

   signed char matchMatrixSC[
         def_lenCol_alnSet
      ][
         def_lenCol_alnSet
      ]; /*match matrix*/

     /* Size of both non-WORDS matrixies (27 or 26) is due
     ` to wanting a look up table that can handle
     ` anonymous bases.  Most cells will be set to 0. 
     */
}alnSet;

/*-------------------------------------------------------\
| Fun01: setScore_alnSet
|  - Sets the score for a base pair (reference/query)
| Input:
|  - qryBase:
|    o query base to set score fore
|  - refBase:
|    o reference base to set score fore
|  - score:
|    o Score to have for this query/reference combination
|      (max is a short)
|  - alnSetSTPtr:
|    o pointer to alnSet (settings) structer with matrix
|      to add the new query reference comparison score to.
| Output:
|  o Modifies:
|    - one score in an snp/match scoring matrix
\-------------------------------------------------------*/
#define setScore_alnSet(qryBase, refBase, score, alnSetSTPtr) ((alnSetSTPtr)->scoreMatrixSS[(unsigned char) ((qryBase) & def_ntToCode_alnSet)][(unsigned char) ((refBase) & def_ntToCode_alnSet)] = (score))
/*setScore_alnSet, sequences will be lookup indexes*/

/*-------------------------------------------------------\
| Fun02: setMatch_alnSet
|  - Sets if two bases are a match or mismtach
| Input:
|  - qryBase:
|    o query base to set score fore
|  - refBase:
|    o reference base to set score fore
|  - match:
|    o 1: the query and reference bases are matches
|    o 0: the query and reference bases are mismatches
|  - alnSetSTPtr:
|    o pointer to alnSet (settings) structer with matrix
|      to add the new query reference comparison score to.
| Output:
|  - Modifies:
|    o one match in the matchMatrix
\-------------------------------------------------------*/
/*The user will not be providing converted bases*/
#define setMatch_alnSet(qryBase, refBase, match, alnSetSTPtr) ((alnSetSTPtr)->matchMatrixSC[(unsigned char) ((qryBase) & def_ntToCode_alnSet)][(unsigned char) ((refBase) & def_ntToCode_alnSet)] = (match))
/*setBasePairMatch, sequences will be lookup indexes*/


/*-------------------------------------------------------\
| Fun03: freeStack_alnSet
|  - Does a stack free of an alnSet structer
| Input:
|  - alnSetSTPtr:
|    o alnSetSTPtr to free internal variables in
| Output:
|  - Free:
|    o Nothing, there are no heap allocated variables.
|      Is here in case I decide to have heap allocated
|      variables on day.
\-------------------------------------------------------*/
void
freeStack_alnSet(
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_alnSet
|  - Frees and alnSet (alignment settings) structure
| Input:
|  - alnSetSTPtr:
|    o Pionter to alnSetST to free
| Output:
|  - Free:
|    o alnSetSTPtr
|  - Set:
|    o alnSetSTPtr to 0 (NULL)
\-------------------------------------------------------*/
void
freeHeap_alnSet(
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun05: getScore_alnSet:
|  - Get the score for a pair of bases from an alignment
| Input:
|  - qryBase:
|    o character with query base to get score for
|  - refBase:
|    o character with reference base to get score for
|  - alnSetPtr:
|    o alnSet (settings) structer pionter with scoring
|      matrix
| Output:
|  - Returns:
|    o score of the input base pair
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
#define getScore_alnSet(qryBase, refBase, alnSetSTPtr) ((alnSetSTPtr)->scoreMatrixSS[(unsigned char) (qryBase)][(unsigned char) (refBase)])
/*getBasePairScore, with sequence converted to index*/

/*-------------------------------------------------------\
| Fun06: getMatch_alnSet
|  - Check if two bases were a match or mismatch
| Input:
|  - qryBase:
|    o character with query base to compare
|  - refBase:
|    o character with reference base to compare to
|  - alnSetPtr:
|    o alnSet (settings) structer pionter with scoring
|      matrix
| Output:
|  - Returns:
|    o def_ntEql_alnDefs if the bases matched (same)
|    o def_ntNotEql_alnDefs if bases were not a match
|    o def_anonymous_alnDefs for anonymous bases
|    o def_anonymous_alnDefs | def_ntEql_alnDefs for
|      anonymous matches (anoymous base could be same)
|  - default
|    o This assumes that the sequences are converted to
|      indexes
\-------------------------------------------------------*/
#define getMatch_alnSet(qryBase, refBase, alnSetSTPtr) ((alnSetSTPtr)->matchMatrixSC[(unsigned char) (qryBase)][(unsigned char) (refBase)])
/*getBasePairMatch, with sequence converted to index*/

/*-------------------------------------------------------\
| Fun07: readScoreFile_alnSet
|  - Reads in a file of scores for a scoring matrix
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the score matrix to modify
|  - scoreFILE:
|    o File to get scores from
| Output:
|  - Modifies:
|    o Score matrix in alngSetST to hold the scores from
|      the file (scoreFILE)
|    o scoreFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readScoreFile_alnSet(
    struct alnSet *alnSetSTPtr, /*score matrix to change*/
    void *scoreFILE  /*File scoring matrix scores*/
);

/*-------------------------------------------------------\
| Fun08: readMatchFile_alnSet
|  - Reads in a file of matches
| Input:
|  - alnSetSTPtr:
|    o pointer to an alnSetST (settings) structure with
|      the match matrix to modify
|  - matchFILE:
|    o File to get matchs from
| Output:
|  - Modifies:
|    o Match matrix in alngSetST to hold the matchs from
|      the file (matchFILE)
|    o matchFILE to point to the end of the file
|  - Returns:
|    o 0 for no errors
|    o position of error in file
\-------------------------------------------------------*/
unsigned long
readMatchFile_alnSet(
    struct alnSet *alnSetSTPtr,
    void *matchFILE  /*File of matchs for scoring matrix*/
);

/*-------------------------------------------------------\
| Fun09: seqToIndex_alnSet
|  - Converts a sequence to a look up table index
|    (table is in alnSetStruct.c/h)
| Input:
|  - seqStr:
|    o pointer c-string with the sequence to convert
| Output:
|  - Modifies:
|    o seqST->seqStr to have look up table indexs (1-27, 
|      with null as 0) instead of bases
\-------------------------------------------------------*/
void
seqToIndex_alnSet(
   signed char *seqStr
);

/*-------------------------------------------------------\
| Fun10: indexToSeq_alnSet
|  - Converts a sequence of lookup indexs back into
|    uppercase characters (a-z)
| Input:
|  - seqStructPtr:
|    o Pointer to sequence structer with converte sequence
|      to deconvert (lookup index to base)
| Output:
|  - Modifies:
|    o seqST->seqStr to have bases instead of look up
|      table indexs
\-------------------------------------------------------*/
void
indexToSeq_alnSet(
   signed char *seqStr
);

/*-------------------------------------------------------\
| Fun11: revCmpIndex_alnSet
|   - reverse complement sequence in lookup index format
|     (from seqToIndex_alnSet)
| Input:
|   - seqStr:
|     o c-string with look up index sequence to
|       reverse complement
|   - syncStr:
|     o c-string to keep in sync with seqStr
|     o use 0/null for no sequence
|   - lenSeqSL:
|     o length of sequence to convert (index 1)
| Output:
|   - Modifies:
|     o seqStr to be reverse complemented
\-------------------------------------------------------*/
void
revCmpIndex_alnSet(
   signed char *seqStr,
   signed char *syncStr,
   unsigned long lenSeqUL
);

/*-------------------------------------------------------\
| Fun12: changeGap_alnSet
|   - changes the gap penalties in an alnSet structure
| Input:
|   - alnSetSTPtr:
|     o pointer to alnSet structure to update gap scores
|   - gapSS:
|     o new gap opening penalty
|   - extendSS:
|     o new gap extension penalty
| Output:
|   - Modifies:
|     o delArySS to have gap extension/opening penalties
|     o insArySS to have gap extension/opening penalties
|     o gapSS in alnSetSTPtr to have gap opening penalty
|     o extendSS to have gap extension penalty
\-------------------------------------------------------*/
void
changeGap_alnSet(
   struct alnSet *alnSetSTPtr,
   signed short gapSS,
   signed short extendSS
);

/*-------------------------------------------------------\
| Fun13: maxScore_alnSet
|  - finds maximum score possible for a sequence
| Input:
|  - seqStr:
|    o c-string with sequence to find max score for
|    o needs to be converted to index with
|      indexToSeq_alnSet (prepared for alignment)
|  - startUL:
|    o index of first base to check (index 0)
|  - lenSeqUL:
|    o length of the sequence to check
|  - alnSetPtr:
|    o pointer to an alnSet (settings) structure to
|      initialize
| Output:
|  o Returns:
|    - maximum possible score for alignment
\-------------------------------------------------------*/
signed long
maxScore_alnSet(
   signed char *seqStr,
   unsigned long startUL,
   unsigned long lenSeqUL,
   struct alnSet *alnSetPtr
);

/*-------------------------------------------------------\
| Fun14: init_alnSet
|  - Set values in altSet (alingment settings) structure
|    to default values
| Input:
|  - alnSetSTPtr:
|    o poineter to an alnSet (settings) structure to
|      initialize
| Output:
|  o Modifies:
|    - alnSetST to have default alignment settings values
\-------------------------------------------------------*/
void
init_alnSet(
    struct alnSet *alnSetST /*Has settings to initialize*/
);

/*-------------------------------------------------------\
| Fun15: pDefMatchMatrix_alnSet
|   - print out the default match matrix
| Input:
|   - outFILE:
|     o file to print match matrix to
| Output:
|   - Prints:
|     o default match matrix in alnDefs.h to outFILE
\-------------------------------------------------------*/
void
pDefMatchMatrix_alnSet(
   void *outFILE
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
