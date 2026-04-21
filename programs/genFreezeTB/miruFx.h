/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' miruFx SOF: Start Of File
'   - has functios to use kmerFind to identify MIRU
'     lineages in reads
'   o header:
'     - default settings, forward declartions and guards
'   o .c fun01: swap_refST_kmerFind_miruFx
'     - swaps two refST_kmerFind structures around
'   o fun02: readTbl_miruFx
'     - reads in a miru table for fastq processing
'   o fun03: kmerFind_miruFx
'     - scans an seqST structure for MIRU-VNTR lineages
'       using kmerFind
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - default settings, forward declartions and guards
\-------------------------------------------------------*/

#ifndef MIRU_VNTR_FASTA_FASTQ_SCAN_H
#define MIRU_VNTR_FASTA_FASTQ_SCAN_H

#define def_minPercScore_miruFx 0.9f
#define def_maxAmpLen_miruFx 5000

struct seqST;
struct miruTbl;
struct refST_kmerFind;
struct tblST_kmerFind;

/*-------------------------------------------------------\
| Fun02: readTbl_miruFx
|   - reads in a miru table for fastq processing
| Input:
|   - miruTblStr:
|     o c-string with path to miru table to use
|   - primStr:
|     o path to primers to align
|   - lenKmerUC:
|     o length of one kmer
|   - refAryST:
|     o pointer to pointer to refST_kmerFind structure
|       to hold reference sequences (should be null)
|   - lenRefUI:
|     o pinter to signed int to hold number primers in
|       refAryST
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure to link
|       with this reference set
|   - minPercKmersF:
|     o float with minimum percentage of kmers to start
|       considering an window supports an spacer
|   - percExtraNtInWinF:
|     o float with percentage of extra nucleotides to
|       store in one window (beyond reference length)
|   - percWinShiftF:
|     o float with percentage of bases to shift for each
|       new window in tblSTPtr
|   - alnSetPtr:
|     o pointer to an alnSet structure with the alignment
|       settings (for finding max possible score)
|   - errSCPtr:
|     o signed char pointer to hold any errors
| Output:
|   - Modifies:
|     o refAryST: to be an array of primer sequences
|     o lenRefSI: to have number of primers read in
|     o tblSTPtr: to be setup for refAryST
|     o errSCPtr:
|       - 0 for no errors
|       - def_fileErr_tbMiruDefs for file error
|       - def_memErr_tbMiruDefs for memory error
|   - Returns:
|     o pointer to a set up miruTbl structure
\-------------------------------------------------------*/
struct miruTbl *
kmerFindSetup_miruTbl(
   signed char *miruTblStr,  /*tsv with path to table*/
   signed char *primTsvStr,  /*tsv with primers*/
   unsigned int lenKmerUC,   /*length of one kmer*/
   struct refST_kmerFind **refAryST,
   signed int *lenRefSI,     /*number of primers*/
   struct tblST_kmerFind *tblSTPtr,
   float minPercKmersF,
   float percExtraNtInWinF,
   float percWinShiftF,
   struct alnSet *alnSetPtr,
   signed char *errSCPtr     /*holds error message*/
);

/*-------------------------------------------------------\
| Fun03: kmerFind_miruFx
|   - scans an seqST structure for MIRU-VNTR lineages
|     using kmerFind
| Input:
|   - miruTblSTPtr:
|     o pointer to an miruTbl structure with a column in
|       an MIRU table to search (lengths for this amplicon
|       for each lineage in the table)
|   - tblSTPtr:
|     o pointer to an tblST_kmerFind structure with
|       settings
|   - refAryST
|     o array of refST_kmerFind structures with reference
|       (primer) sequences to search for
|     o needs to be same length as
|       miruTblSTPtr->numPrimSI * 2
|   - fudgeSI:
|     o offset length by when comparing to lingeage length
|   - tblSTPtr:
|     o pointer to tblST_kmerFind structure with MIRU-VNTR
|       primers to scan for and settings
|   - seqSTPtr:
|     o pointer to seqST struct with sequence to scan
|   - minPercScoreF:
|     o minimum percent score to keep an alignment
|   - maxAmpLenSI:
|     o maximum amplicon length before discarding
|       a match
|   - alnSetPtr:
|     o pointer to alnSet sturct with alignment settings
| Output:
|   - Modifies:
|     o miruTblST to have detected lineages incurmented
|   - Returns:
|     o 0 if found lineage
|     o 1 if could not find lineage
|     o 2 for an error
\-------------------------------------------------------*/
signed char
kmerFind_miruFx(
   struct miruTbl *miruTblSTPtr, /*MIRU lineage table*/
   struct tblST_kmerFind *tblSTPtr,/*settings*/
   struct refST_kmerFind *refAryST,/*primer sequences*/
   signed int fudgeSI,           /*lineage match range*/
   struct seqST *seqSTPtr,       /*query sequence*/
   float minPercScoreF,           /*min score to keep map*/
   signed int maxAmpLenSI,       /*max amplicon length*/
   struct alnSet *alnSetPtr      /*alignment settings*/
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
