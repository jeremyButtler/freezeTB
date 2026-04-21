/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' checkAmr SOF: Start Of File
'   - has functions to detect AMRs in sam file entrie
'   o header:
'     - definitions, forward declerations, and guards
'   o .h st01: amrHit_checkAmr
'     - linked list of postive amrs for each amr check
'   o fun01: init_amrHit_checkAmr
'     - initializes amrHit_checkAmr struct (all zeros)
'   o fun02: freeHeap_amrHit_checkAmr
'     - frees a amrHit_checkAmr structure
'   o fun03: freeHeapList_amrHit_checkAmr
'     - frees list of amrHit_checkAmr structures
'   o fun04: mk_marHit_checkAmr
'     - makes heap initialized amrHit_checkAmr structer
'   o fun05: LoFFor_checkAmr
'     - gets number of indels in gene and checks for early
'       stops or missing stops and starts.
'   o fun06: LofRev_checkAmr
'     - gets number of indels and checks for early stops
'       or missing stops/starts in reverse complement gene
'   o fun07: checkAmr
'     - checks if sam entry has amr's
'   o .c fun08: pCrossRes_checkAmr
'     - print out cross resitance (report, not database)
'   o fun09: cmpAmrs_checkAmr
'     - checks if two AMR structs are the same variant
'   o fun10: flagAmrHit_checkAmr
'     - flag all duplicates in a AMR hit list
'   o fun11: pConHead_checkAmr
'     - prints header for a consensus amr check
'   o fun12: pCon_checkAmr
'     - prints out all amr's that were in a consensus
'   o fun13: readGetVarHits_checkAmr
'     - finds non-duplicate AMRs for the target variant ID
'   o fun14: pReadHead_checkAmr
'     - prints header for reads AMR table
'   o fun15: pRead_checkAmr
'     - prints AMRs detected in reads that have min depth
'   o fun16: pIdVarHead_checkAmr
'     - prints header for the read id mapped variant table
'   o fun17: pIdVarTbl_checkAmr
'     - prints table of read ids and detected AMRs
'   o fun18: samo findAmrs_checkAmr
'     - look for AMRs in sam file entries
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - definitions, forward declerations, and guards
\-------------------------------------------------------*/

#ifndef CHECK_AMR_H
#define CHECK_AMR_H

struct samEntry;
struct refList_amrST;
struct amrST;

/*-------------------------------------------------------\
| ST01: amrHit_checkAmr
|   - linked list of postive amrs for each amr check
\-------------------------------------------------------*/
typedef struct amrHit_checkAmr{
   struct amrST *amrSTPtr;
   unsigned int seqPosUI;
   struct amrHit_checkAmr *nextAmr;
   signed char dupBl; /*marks if duplicate variant*/
}amrHit_checkAmr;

/*-------------------------------------------------------\
| Fun01: init_amrHit_checkAmr
|   - initializes amrHit_checkAmr struct (all zeros)
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to initialize
| Output:
|   - Modifies:
|     o amrST and nextAmr to point to 0 (NULL)
|     o seqPosUI to be 0
\-------------------------------------------------------*/
void
init_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
);

/*-------------------------------------------------------\
| Fun02: freeHeap_amrHit_checkAmr
|   - frees a amrHit_checkAmr structure
| Input:
|   - amrHitSTPtr:
|     o pointer to amrHist structure to free
| Output:
|   - Frees:
|     o amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeap_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeHeapList_amrHit_checkAmr
|   - frees list of amrHit_checkAmr structures
| Input:
|   - amrHitListST:
|     o pointer to start of an amrHist struct list to free
| Output:
|   - Frees:
|     o all amrHit_checkAmrs structures in amrHitSTPtr
\-------------------------------------------------------*/
void
freeHeapList_amrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
);

/*-------------------------------------------------------\
| Fun04: mk_amrHit_checkAmr
|   - makes heap initialized amrHit_checkAmr structer
| Input:
| Output:
|   - Returns:
|     o pointer to the new amrHit_checkAmr structure
|     o 0 for memory error
\-------------------------------------------------------*/
struct amrHit_checkAmr *
mk_amrHit_checkAmr(
   void
);

/*-------------------------------------------------------\
| Fun05: LoFFor_checkAmr
|   - gets number of indels in gene and checks for early
|     stops or missing stops and starts.
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with the sequence
|       having the gene to check
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o delCntSI to hold the number of deletions
|     o insCntSI to hold the number of insertions
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LoFFor_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry *samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
);

/*-------------------------------------------------------\
| Fun06: LofRev_checkAmr
|   - gets number of indels and checks for early stops or
|     missing stops and starts in reverse complement gene
| Input:
|   - startGeneSI:
|     o index (0) of the first reference base in the gene
|   - endGeneSI:
|     o index (0) of the last reference base in the gene
|   - refPosSI:
|     o position at in the reference genome
|   - readPosSI:
|     o position at in the reads sequence
|   - samSTPtr:
|     o pointer to an samEntry structure with sequence
|   - cigPosSI:
|     o position at in the cigar
|   - cigBaseSI:
|     o base on in the current cigar entry
|   - delCntSI:
|     o pointer to signed integer to hold number of dels
|   - insCntSI:
|     o pointer to signed integer to hold number of ins
| Output:
|   - Modifies:
|     o indelCntSI to hold the number of indels
|   - Returns:
|     o 1 if there was an missing start or early stop
|     o 0 if there was no missing start or early stop
\-------------------------------------------------------*/
signed char
LofRev_checkAmr(
   signed int startGeneSI,
   signed int endGeneSI,
   signed int refPosSI,
   signed int readPosSI,
   struct samEntry *samSTPtr,
   signed int cigPosSI,
   signed int cigBaseSI,
   signed int *delCntSI,
   signed int *insCntSI
);

/*-------------------------------------------------------\
| Fun07: checkAmr
|   - checks if sam entry has amr's
| Input:
|   - samSTPTr:
|     o pointer to samEntry struct with sequence to check
|   - amrSTAryPtr:
|     o pointer to an amrST struct array with amr's
|   - numAmrSI:
|     o number of amrST structures in amrAryST
|   - numHitsSI:
|     o pointer to signed int to hold number detected amrs
|   - frameshiftBl:
|     o 1: check for LoFs in frameshift
|     o 0: treat frameshifts as exact matches
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - errSC:
|     o pointer to signed char to hold the error output
| Output:
|   - Modifies:
|     o numHitsSI to have number of amrs found in samSTPTr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o list of amrHit_checkAmr structs with detected amrs
|     o 0 for no amrs or memory errors
\-------------------------------------------------------*/
struct amrHit_checkAmr *
checkAmr(
   struct samEntry *samSTPTr, /*sequence to check*/
   struct amrST *amrAryST,    /*AMRs to search*/
   signed int numAmrSI,       /*length of amrAryST*/
   signed int *numHitsSI,     /*holds number amr hits*/
   signed char frameshiftBl,  /*1: frameshift/indel mode*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   signed char *errSC         /*for error reporting*/
);

/*-------------------------------------------------------\
| Fun09: cmpAmrs_checkAmr
|   - checks if two AMR structs are the same variant
| Input:
|   - oneST:
|     o pointer to first amrST structures to compare
|   - twoST:
|     o pointer to second amrST structures to compare
| Output:
|   - Returns:
|     o -1 if are different by variant ID
|     o -2 if same variant ID, but different attribute
|     o 0 if are the same to checkAmr
|     o 1 if first AMR has more support
|     o 2 if second AMR has more support
\-------------------------------------------------------*/
signed char
cmpAmrs_checkAmr(
   struct amrST *oneST,
   struct amrST *twoST
);

/*-------------------------------------------------------\
| Fun10: flagAmrHit_checkAmr
|   - flag all duplicates in a AMR hit list
| Input:
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs to
|       flag duplicate AMRs
| Output:
|    - Modifies:
|      o dupBl in each amrHit_checkAmr struct in
|        amrHitListST to be 1 (if duplicates) or 0 if
|        unique/best AMR
\-------------------------------------------------------*/
void
flagAmrHit_checkAmr(
   struct amrHit_checkAmr *amrHitListST
);

/*-------------------------------------------------------\
| Fun11: pConHead_checkAmr
|   - prints header for a consensus amr check
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrHit_checkAmrList table header to outFILE
\-------------------------------------------------------*/
void
pConHead_checkAmr(
   signed char refBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: pCon_checkAmr
|   - prints out all amr's that were in a consensus
| Input:
|   - refStr:
|     o c-string with the reference id
|     o or 0/null if no reference column
|   - seqIdStr:
|     o c-string with name of sequence
|   - amrHitListST:
|     o pointer to list of amrHit_checkAmr structs with
|       detected AMRs
|   - refSTPtr:
|     o refList_amrST struct pointer with drugs used in
|       the database
|   - outFILE:
|     o file to print amr's to
| Output: 
|   - Prints:
|     o amr's in amrHitListST to outFILE
|   - Modifies:
|     o dupBl in amrHitListST to be 1 for duplicate
|       variant ids
\-------------------------------------------------------*/
void
pCon_checkAmr(
   signed char *refStr, /*reference id to print*/
   signed char *seqIdStr,
   struct amrHit_checkAmr *amrHitListST,
   struct refList_amrST *refSTPtr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun13: readGetVarHits_checkAmr
|   - finds non-duplicate AMRs for the target variant ID
| Input:
|   - amrAryST:
|     o pointer to an array of amrST structures to print
|   - indexSI:
|     o index of AMR in amrAryST
|   - numAmrsUI:
|     o number AMRs in amrAryST
| Output:
|   - Returns:
|     o 0 for memory errors
|     o signed int array ending in -1 with the index of
|       each different AMR
|       * the index after -1 has the next variant ID to
|         scan
\-------------------------------------------------------*/
signed int *
readGetVarHits_checkAmr(
   struct amrST *amrAryST,
   signed int indexSI,      /*index at in AMR array*/
   unsigned int numAmrsUI
);

/*-------------------------------------------------------\
| Fun14: pReadHead_checkAmr
|   - prints header for reads AMR table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o amrs table header to outFILE
\-------------------------------------------------------*/
void
pReadHead_checkAmr(
   signed char refBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun15: pRead_checkAmr
|   - prints AMRs detected in reads that have min depth
| Input:
|   - minDepthUI:
|     o minumum depth to keep an amr
|   - minPercMapF:
|     o mininmum percent of supporting reads to keep an
|       amr (only compared locally)
|   - minPercTotalF:
|     o min percent of mapped reads needed to keep an amr
|       (compared to all reads [total depth])
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - framShiftBl:
|     o 1: looked for frameshifts in data
|   - totalReadsUI:
|     o total number of reads input
|   - refSTPtr:
|     o pointer to a refList_amrST struct with amrs to
|       check if detected
|   - outFILE:
|     o file to print the amr's to
| Output: 
|   - Modifies:
|     o amrAryST to be sorted by position
|   - Prints:
|     o AMRs with >= min stats to outFILE
\-------------------------------------------------------*/
void
pRead_checkAmr(
   unsigned int minDepthUI,
   float minPercMapF,
   float minPercTotalF,
   float minIndelSupF,      /*% support to keep indel*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char frameShiftBl,/*looked for frameshifts*/
   unsigned int totalReadsUI,
   struct refList_amrST *refSTPtr, /*has amrs to check*/
   void *outFILE
);

/*-------------------------------------------------------\
| Fun16: pIdVarHead_checkAmr
|   - prints header for the read id mapped variant table
| Input:
|   - refBl:
|     o 0: do not print out reference column
|     o 1: print out reference column
|   - outFILE:
|     o file to print header to
| Output:
|   - Prints:
|     o header for read id mapped variant table to outFILE
\-------------------------------------------------------*/
void
pIdVarHead_checkAmr(
   signed char pRefBl,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun17: pIdVarTbl_checkAmr
|   - prints table of read ids and detected AMRs
| Input:
|   - refStr:
|     o c-string with reference used
|     o use 0/null to not print the reference column
|   - idStr:
|     o c-string with read id to print out
|   - amrHitListST:
|     o pointer to an list of amrHit_checkAmr structures
|       having AMRs detected in the read
|   - outFILE:
|     o pointer to FILE structure to print header to
| Output:
|   - Prints:
|     o read id and detected AMRs to outFILE
\-------------------------------------------------------*/
void
pIdVarTbl_checkAmr(
   signed char *refStr,
   signed char *idStr,
   struct amrHit_checkAmr *amrHitListST,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun18: samFindAmrs_checkAmr
|   - look for AMRs in sam file entries
| Input:
|   - refSTPtr:
|     o pointer to refList_amrST struct with the amrs for
|       each reference
|   - readsBl:
|     o 1: print read AMRs (pRead_checkAmry [fun15])
|     o 0: print consensus AMRs (pCon_checkAmr [fun13])
|   - framshiftBl:
|     o 1: check for framshifts (LoF/frameshift AMRs)
|     o 0: ingore frameshifts (are exact matches)
|   - aaIndelBl:
|     o 3: skip aa indel check and check if codon has
|          indel
|     o 2: skip aa indel check
|     o 1: amino acid changes, if codon has indel not
|          in target position, check amino acids
|     o 0: ignore all sequences with indel in codon
|   - minDepthUI:
|     o minumum depth to keep an amr (reads only)
|   - minPercMapF:
|     o mininimum percent of supporting reads to keep an
|       amr (reads only) 
|   - minPercTotalF:
|     o mininimum percent of mapped reads needed to keep
|       an amr (all possible mapped reads; reads only)
|   - minIndelSupF:
|     o minimum percent support to keep an indel AMR
|   - minFrameshiftF:
|     o minimum percent support to keep a frame shift
|   - samFileStr:
|     o c-string with sam file to check for AMRs
|   - outFileStr:
|     o c-string with name to print AMR hits to
|   - idFileStr:
|     o c-sring with name of file to print read ids to
| Output:
|   - Prints:
|     o stats about AMRs to outFILE
|     o read id's to the AMR files they matched, but only
|       if a idPrefStr was provided
|   - Returns:
|     o 0 for no problems
|     o def_fileErr_amrST for file open errors
|     o def_memErr_amrST for memory errors
\-------------------------------------------------------*/
signed char
samFindAmrs_checkAmr(
   struct refList_amrST *refSTPtr,/*has amrs and ref ids*/
   signed char readsBl,     /*1: checking reads not cons*/
   signed char frameshiftBl,/*1: check frameshifts*/
   signed char aaIndelBl,     /*1: check aa for indels*/
   unsigned int minDepthUI, /*min depth to keep amr*/
   float minPercMapF,       /*% support to keep amr*/
   float minPercTotalF,     /*% mapped reads to keep amr*/
   float minIndelSupF,      /*%support to keep indel amr*/
   float minFrameshiftF,    /*% support for frameshift*/
   signed char *samFileStr, /*sam file to check*/
   signed char *outFileStr, /*output file (main)*/
   signed char *idFileStr   /*output file (ids)*/
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
