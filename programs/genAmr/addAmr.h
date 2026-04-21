/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' addAmr SOF: Start Of File
'   - has functions to get tbAmr database information from
'     variant ids
'   o header;
'     - type defs, defined variables, and guards
'   o .c fun01: aaToSeq_addAmr
'     - change amino acids to a valid nucleotide sequence
'   o .c fun02: revCmp_addAmr
'     - reverse complement a sequence
'   o .c fun03: addOneAa_addAmr
'     - add single amino acid variant to an amrST struct
'   o .c fun04: parseVarId_addAmr
'     - parse variant id to its correct format
'   o .c fun05: addNtSeq_addAmr
'     - adds nucleotide sequence to AMR
'   o .c fun06: addCodonPos_addAmr
'     - adds amino acid sequences for deletions and
'       large duplications, reading frame orientation
'       (forward/reverse), and the first reference
'       base in the codon to an amrST that has been
'       processed with addNtSeq_addAmr
'   o fun07: var_addAmr
'     - adds a variant to an AMR struct, but does not
'       complete (call addCodonPos_addAmr)
'   o fun08: addDrugs_addAmr:
'     - add anti-microbials (antibiotics) to an AMR
'   o fun09: lineToVar_addAmr
'     - convert line from variant file to tbAmr entry
'   o fun10: getVar_addAmr
'     - get a variant id from a file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header;
|   - type defs, defined variables, and guards
\-------------------------------------------------------*/

#ifndef ADD_ANTI_MICROBIAL_RESISTANT_VARIANT_TO_DATABASE_H
#define ADD_ANTI_MICROBIAL_RESISTANT_VARIANT_TO_DATABASE_H

/*some quick default settings*/
#define def_grade_addAmr 2
#define def_highRes_addAmr 0
#define def_lowRes_addAmr 0
#define def_addRes_addAmr 0

#define def_noVar_addAmr 1
#define def_lof_addAmr 2
#define def_noGene_addAmr 3
#define def_del_addAmr 4
#define def_memErr_addAmr 5
#define def_fileErr_addAmr 6
#define def_varErr_addAmr 7
#define def_overflow_addAmr 8
#define def_eof_addAmr 8
#define def_noRef_addAmr 9

struct amrST;
struct seqST;
struct geneCoord;

/*-------------------------------------------------------\
| Fun07: var_addAmr
|   - adds a variant to an AMR struct, but does not
|     complete (call addCodonPos_addAmr)
| Input:
|   - varStr:
|     o variant id to process, should be in form of
|       * "gene_p.refAAnumberAMRAA" (ex gyrB_p.Ser447Phe)
|       * "gene_LoF"
|       * "gene_deletion"
|   - geneStr:
|     o c-string with name of gene to add
|   - coordsSTPtr:
|     o geneCoord struct pointer with gene coordinates
|     o needs to be sorted by gene name
|       * use sortName_geneCoord()
|   - refIdStr:
|     o c-string with reference id
|   - refSeqStr:
|     o c-string with the reference sequence
|   - errSCPtr:
|     o pointer to signed char to get returned error type
| Output:
|   - Modifies:
|     o errSCPtr to have error types:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_noVar_addAmr if variant id is empty
|   - Returns:
|     o pointer to amrST struct with added variant
|     o 0 for memory errors
\-------------------------------------------------------*/
struct amrST *
var_addAmr(
   signed char *varStr,      /*variant to add*/
   signed char *geneStr,     /*name of gene to add*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   signed char *refIdStr,      /*reference id*/
   signed char *refSeqStr,     /*reference sequence*/
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun08: addDrugs_addAmr:
|   - add anti-microbials (antibiotics) to an AMR
| Input:
|   - amrSTPtr:
|     o amrST struct pointer to add drugs to
|   - newDrugStr:
|     o c-string with new drug to add to armSTPtr
|   - crossResBl:
|     o 1: set drug as cross resistant in amrSTPtr
|     o 0: set drug as primrary resistant in amrSTPtr
|   - drugAryStr:
|     o c-string pointer with antiobitics names
|     o this is a an array, so only update with functions
|       in drugAry.c/h
|     o you can set drugAryStr to 0 to have this function
|       allocate memory for you
|       * signed char *drugAryStr = 0;
|       * signed int drugLenSI = 0;
|       * signed int drugSizeSI = 0;
|       * sigend char errorSC =
|            addDrugs_addAmr(
|              amrSTPtr,
|              (signed char *) "PMD",
|              0,
|              &drugAryStr,
|              &drugLenSI,
|              &drugSizeSI
|            );
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
| Output:
|   - Modifies:
|     o amrSTPtr to have the new drug
|     o drugAryStr to have new drug if not already in it
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|   - Returns:
|     o 0 for no errors
|     o def_memErr_addAmr for memory errors
|     o def_overflow_addAmr if went past the maximum drug
|       limit
\-------------------------------------------------------*/
signed char
addDrugs_addAmr(
   struct amrST *amrSTPtr,  /*add drug to*/
   signed char *newDrugStr, /*drug to add*/
   signed char crossResBl,  /*1: is cross resistance*/
   signed char **drugAryStr,/*has drugs already added*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr/*max drugs in drugAryStr*/
);

/*-------------------------------------------------------\
| Fun09: lineToVar_addAmr
|   - convert line from variant file to tbAmr entry
| Input:
|   - lineStr:
|     o c-string with line to convert
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if variant id is empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|       * def_noRef_addAmr if no reference found
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
lineToVar_addAmr(
   signed char *lineStr,         /*has variant id*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr,/*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
);

/*-------------------------------------------------------\
| Fun10: getVar_addAmr
|   - get a variant id from a file
| Input:
|   - varFILE:
|     o FILE pointer with variant line to get
|   - refIndexSIPtr:
|     o signed int pointer to get the index of the
|       reference used
|   - coordsSTPtr:
|     o geneCoord struct pointer with genes to map to
|   - refSeqArySTPtr:
|     o seqST struct array with the reference sequences
|       sorted by reference name (use `sort_seqST()`)
|   - refSeqLenSL:
|     o number of reference sequences
|   - drugAryStr:
|     o pointer to c-string (techincally array) with drugs
|       to add to
|   - drugLenSIPtr:
|     o signed int pointer with number drugs in drugAryStr
|   - drugSizeSIPtr:
|     o signed int pointer with maximum number of drugs
|       that can be in drugAryStr before resizing
|   - errSCPtr:
|     o signed char to get errors
|   - errPosSLPtr:
|     o signed long pointer to get positon of error in
|       line
| Output:
|   - Modifies:
|     o varFILE to be on next line after variant
|     o refIndexSIPtr to have -1 or the index of the
|       reference used
|     o drugAryStr to have new drugs
|       * realloced or malloced if need to get more memory
|     o drugLenSIPtr to have new drugAryStr length if a
|       new drug was added
|     o drugSizeSIPtr to have new drugAryStr size if
|       drugAryStr is resized
|     o errSCPtr:
|       * 0 for no errors
|       * def_memErr_addAmr for memory errors
|       * def_fileErr_addAmr for file errors
|       * def_noVar_addAmr if blank line or variant id is
|         empty
|       * def_varErr_addAmr if something is wrong with the
|         varaint entry
|       * def_overflow_addAmr if went past the maximum
|         drug limit (for an amrST struct)
|       * def_eof_addAmr if end of file (no variant)
|     o errPosSLPtr to get position of error in line
|   - Returns:
|     o 0 for errors
|     o amrST struct with new AMR
\-------------------------------------------------------*/
struct amrST *
getVar_addAmr(
   void *varFILE,            /*file with variant ids*/
   signed int *refIndexSIPtr,    /*gets reference used*/
   struct geneCoord *coordsSTPtr,/*has gene coordaintes*/
   struct seqST *refSeqArySTPtr, /*reference sequences*/
   signed long refSeqLenSL,  /*number of references*/
   signed char **drugAryStr, /*has drugs in database*/
   signed int *drugLenSIPtr,/*number drugs in drugAryStr*/
   signed int *drugSizeSIPtr, /*max drugs in drugAryStr*/
   signed char *errSCPtr,    /*gets errors*/
   signed long *errPosSLPtr  /*positon in line error at*/
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
