/*########################################################
# Name: whoToTbAmr
#   - holds functions to convert 2021/2023 WHO database to
#     tbAmr format.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o .c fun01: amrIsRes_whoToAmr
'     - dectects if an amr is classified as resistant
'   o .c fun02: addOneAa_whoToAmr
'     - add single amino acid variant to an amrST struct
'   o fun03: WHO2021Catalog_whoToAmr
'     - gets amrs from the genome indice tab (as csv) of
'       the WHO TB antibiotic resistance 2021 catalog
'   o c. fun04: crossRes_whoToAmr
'     - check if have cross resitance (2023 catalog)
'   o fun05: WHO2023Catalog_whoToAmr
'     - reads in both tabs of the 2023 WHO TB catalog (as
'       separate tsv's) and extracts AMRs
'   o fun06: parseVarId_whoToAmr
'     - parse variant idea from the WHO 2023 TB catalog
'   o .c fun07: sortCoords_whoToAmr
'     - sorts the gene start, end, and direction arrays by
'       the starting coordiantes
'   o fun08: addCodonPos_whoToAmr
'       - Adds the amino acid sequences for deletions and
'         large duplications, reading frame orientation
'         (forward/reverse) to the, and the first
'         reference base in the codon to an amrST that
'         has been processed with parseVarId_whoToAmr.
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef CONVERT_WHO_TO_TBAMR_DATABASE_H
#define CONVERT_WHO_TO_TBAMR_DATABASE_H

struct amrST;

#define def_noRes_whoToAmr 1

/*-------------------------------------------------------\
| Fun03: WHO2021Catalog_whoToAmr
|   - gets amrs from the genome indice tab (as csv) of the
|     WHO TB antibiotic resistance 2021 catalog
| Input:
|   - whoCsvStr:
|     o c-string with the path to the csv file for the
|       WHO's genome indience tab of the TB antibiotic
|       catalog spread sheet
|   - numArmUL:
|     o modified to hold the number of catalog entriex
|   - drugAryStr:
|     o c-string that will hold the name of each drug
|   - numDrugsSI:
|     o will have the number of druges in drugAryStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - errSC:
|     o pointer to signed char to hold error message
| Output:
|   - Modifies:
|     o numArmUL to hold the number of extracted amr
|       mutations.
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC to hold error message
|       - 0 if no error
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST's (free with freeHeapAry_amrST)
\-------------------------------------------------------*/
struct amrST *
WHO2021Catalog_whoToAmr(
   signed char *whoCsvStr,
   unsigned long *numAmrUL,
   signed char **drugAryStr,
   signed int *numDrugsSI,
   signed char keepNonResBl,   /*1: to keep everything*/
   signed char *errSC
);

/*-------------------------------------------------------\
| Fun05: WHO2023Catalog_whoToAmr
|   - reads in both tabs of the 2023 WHO TB catalog (as
|     separate tsv's) and extracts AMRs
| Input:
|   - whoMasterStr:
|     o c-string with the path to the tsv file with 
|       tab one (master) of the 2023 who TB catalog
|   - whoIndiceFILE:
|     o c-string with the path to the tsv with tab two
|       (genome indices) of the 2023 who catalog
|   - numAmrUL:
|     o modifed to hold the number of kept amr genes
|   - drugStr:
|     o modified to hold the antibiotic names. Each name
|       is at most 31 characters long. To get to the next
|       name add 32.
|   - numDrugsSI:
|     o modfied to hold the number of drugs in drugStr
|   - keepNonResBl:
|     o 1: keep grade 3, 4, and 5 entries (not an AMR)
|     o 0: only keep mutations that cause an AMR
|   - rmFullGeneVarBl, 
|     o 1: ignore all variants that apply to the full gene
|     o 0: keep all variants
|   - errSC:
|     o holds error type
| Output:
|   - Modifies:
|     o numAmrUL to hold the number of used amrSTs in
|       the returned amrST structure array
|     o drugAryStr to hold the name of each antibiotic
|     o numDrugsSI to hold the number of drugs in
|       drugAryStr
|     o errSC:
|       - 0 for no errors
|       - def_memErr_amrST for memory errors
|       - def_fileErr_amrST for file errors
|   - Returns:
|     o array of amrST structures with catalog AMRs
\-------------------------------------------------------*/
struct amrST *
WHO2023Catalog_whoToAmr(
   signed char *whoMasterStr,  /*master tab tsv path*/
   signed char *whoIndiceStr,  /*genome indice tab path*/
   unsigned long *numAmrUL,    /*number of amrs kept*/
   signed char **drugAryStr,   /*holds antibiotic names*/
   signed int *numDrugsSI,     /*number drugs in drugAry*/
   signed char keepNonResBl,   /*1: keep all AMR grades*/
   signed char rmFullGeneVarBl,/*1:no entire gene events*/
   signed char *errSC          /*reports errors*/
);

/*-------------------------------------------------------\
| Fun06: parseVarId_whoToAmr
|   - parse variant idea from the WHO 2023 TB catalog
| Input:
|   - amrST:
|     - pointer to array of amrST structures to update and
|       process variants for
|   - numAmrSI:
|     - number of amrSTs in amrST
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence whe possible. for
|       large deletions/duplications you will need to call
|       addCodonPos_whoToAmr
|   - Returns
|     o 0 for success
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
parseVarId_whoToAmr(
   struct amrST *amrSTPtr, /*has amr variants to update*/
   signed int numAmrSI     /*number of amrs*/
);

/*-------------------------------------------------------\
| Fun08: addCodonPos_whoToAmr
|   - adds amino acid sequences for deletions and
|     large duplications, reading frame orientation
|     (forward/reverse), and the first reference
|     base in the codon to an amrST that has been
|     processed with parseVarId_whoToAmr.
| Input:
|   - amrST:
|     o pointer to an array of amrST structs to update
|   - numAmrSI:
|     o number of amrSTs in amrST
|   - geneTblFileStr:
|     o c-string with the path to the gene coordinates
|       file with gene mappings 
|       - column 3: + for foward gene; - for reverse gene
|       - column 4: frist mapped base in reference
|       - column 5: last mapped base in reference
|   - refFileStr:
|     o c-string with path to fasta file with reference
|       sequence (should only have one sequence)
| Output:
|   - Modifies:
|     o each structure in amrST with amino acid mutations
|       to have the amino acid sequence and starting
|       position of codon on the reference
|     o buffStr to be larger if needed
|     o lenBuffUL if buffStr is resized
|     o samFILE to point to the end of the file
|   - Returns
|     o 0 for success
|     o def_fileErr_amrST for an file error
|     o def_memErr_amrST for a memory error
\-------------------------------------------------------*/
signed char
addCodonPos_whoToAmr(
   struct amrST *amrSTPtr,     /*amr variants to update*/
   signed int numAmrSI,        /*number of amrs*/
   signed char *geneTblFileStr,/*gene coordiantes path*/
   signed char *refFileStr     /*reference file path*/
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
