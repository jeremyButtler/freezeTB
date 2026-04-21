/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' illNano SOF: Start Of File
'   - extracs ONT reads with Illumina variants
'   o header:
'     - forward declarations, defined variabals, guards
'   o .h st01: prof_illNano
'     - holds a unique profile from illNano
'   o .h st02: profList_illNano
'     - head of a prof_illNano struct list
'   o fun01: blank_prof_illNano
'     - blanks a prof_illNano struct
'   o fun02: init_prof_illNano
'     - initialize a prof_illNano struct
'   o fun03: freeStack_prof_illNano
'     - frees arrays in prof_illNano (heap allocated)
'   o fun04: freeHeap_prof_illNano
'     - a heap allocated prof_illNano struct
'   o fun05: freeHeapList_prof_illNano
'     - a list of heap allocated prof_illNano structs
'   o fun06: mk_prof_illNano
'     - makes a prof_illNano struct
'   o fun06: mk_prof_illNano
'     - makes a prof_illNano struct
'   o fun07: cmp_prof_illNano
'     - compares two prof_illNano structs
'   o fun08: swap_prof_illNano
'     - swaps variables in two prof_illNano structs
'   o fun09: merge_prof_illNano
'     - merges two prof_illNano structs into one profile
'   o fun14: getIllSnp_illNano
'     - gets snps from tbCon tsv file for Illumina data
'   o fun15: getVarNano_illNano
'     - identifies variants in a nanopore read using an
'       Illumina profile
'   o fun16: getNanoReads_illNano
'     - gets nanopore reads using Illumina profile
'   o fun17: phead_profList_illNano
'     - prints header for unique profile tsv
'   o fun18: p_profList_illNano
'     - prints unique profiles in a profList struct
'   o fun19: run_illNano
'     - runs the illNano algorithim
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations, defined variabals, guards
\-------------------------------------------------------*/

#ifndef ILLUMINA_NANOPORE_READ_EXTRACT_H
#define ILLUMINA_NANOPORE_READ_EXTRACT_H

struct samEntry;

#define def_memErr_illNano 1
#define def_fileErr_illNano 2
#define def_noVar_illNano 4   /*no variants in file*/

#define def_A_illNano 1
#define def_C_illNano 2
#define def_G_illNano 4
#define def_T_illNano 8
#define def_del_illNano 16
#define def_noBase_illNano 64

#define def_delChar_illNano 'D'
#define def_unkown_illNano 'X'

/*-------------------------------------------------------\
| ST01: prof_illNano
|   - holds a unique profile from illNano
\-------------------------------------------------------*/
typedef struct prof_illNano
{
   unsigned int *posAryUI;      /*reference position*/
   signed char *ntArySC;        /*variant base*/

   unsigned int *depthAryUI; /*depth of known bases*/
   unsigned int *xDepthAryUI;/*depth of unknown bases*/
   unsigned int depthUI;     /*number reads with profile*/
   unsigned int totalDepthUI; /*number reads at position*/

   unsigned int sizeProfUI;     /*size of profile*/
   unsigned int varInProfUI;   /*# variants in profile*/
   unsigned int minDiffUI; /*min dif from other profiles*/
   unsigned int maxDiffUI; /*max dif from other profiles*/
   float avgDiffF;        /*mean dif from other profiles*/
   unsigned long sumDiffUL; /*sum of all differences*/

   unsigned int overlapUI; /*number overlapping profiles*/

   struct prof_illNano *nextST;
   struct prof_illNano *lastST;
}prof_illNano;

/*-------------------------------------------------------\
| ST02: profList_illNano
|   - head of a prof_illNano struct list
\-------------------------------------------------------*/
typedef struct profList_illNano
{
   unsigned long numProfUL;    /*number of profiles*/
   struct prof_illNano *listST;/*list of unique profiles*/
}profList_illNano;

/*-------------------------------------------------------\
| Fun01: blank_prof_illNano
|   - blanks a prof_illNano struct
| Input:
|   - profSTPtr:
|     o pointer to prof_illNano struct to blank
| Output:
|   - Modifies:
|     o varInProfUI, maxDiffUI, minDiffUI, and avgDiffF
|       to be 0
|     o all arrays to be filled with 0 (if have)
\-------------------------------------------------------*/
void
blank_prof_illNano(
   struct prof_illNano *profSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_prof_illNano
|   - initialize a prof_illNano struct
| Input:
|   - profSTPtr:
|     o pointer to prof_illNano struct to initialize
| Output:
|   - Modifies:
|     o all variants in profSTPtr to be 0
\-------------------------------------------------------*/
void
init_prof_illNano(
   struct prof_illNano *profSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_prof_illNano
|   - frees arrays in prof_illNano (assumes heap allocate)
| Input:
|   - profSTPtr:
|     o pointer to prof_illNano struct with arrays to free
| Output:
|   - Frees:
|     o posAryUI, ntArySC, depthAryUI, and xDepthAryUI in
|       profSTPtr
\-------------------------------------------------------*/
void
freeStack_prof_illNano(
   struct prof_illNano *profSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_prof_illNano
|   - a heap allocated prof_illNano struct
| Input:
|   - profSTPtr:
|     o pointer to prof_illNano struct to free
| Output:
|   - Frees:
|     o profSTPtr (you must set to 0)
|   - Returns:
|     o next prof_illNano struct in list
\-------------------------------------------------------*/
struct prof_illNano *
freeHeap_prof_illNano(
   struct prof_illNano *profSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeapList_prof_illNano
|   - a list of heap allocated prof_illNano structs
| Input:
|   - profSTPtr:
|     o pointer to list of prof_illNano struct to free
| Output:
|   - Frees:
|     o every prof_illNano struct in profSTPtr
|       (you must set to profSTPtr to 0)
\-------------------------------------------------------*/
void
freeHeapList_prof_illNano(
   struct prof_illNano *profSTPtr
);

/*-------------------------------------------------------\
| Fun06: mk_prof_illNano
|   - makes a prof_illNano struct
| Input:
|   - posAryUI:
|     o unsigned int array with positions to copy
|     o use 0 (null) to not use
|   - ntArySC:
|     o signed char array with nucleotides to copy
|     o use 0 (null) to not use
|   - numVarUI:
|     o number of variants in ntArySC and posAryUI
| Output:
|   - Returns:
|     o heap allocated prof_illNano struct with copied
|       arrays
|     o 0 for errors
\-------------------------------------------------------*/
struct prof_illNano *
mk_prof_illNano(
   unsigned int *posAryUI, /*has positions to copy*/
   signed char *ntArySC,   /*has nucleotides to copy*/
   unsigned int numVarUI   /*number variants in arrays*/
);

/*-------------------------------------------------------\
| Fun07: cmp_prof_illNano
|   - compares two prof_illNano structs
| Input:
|   - firstProfSTPtr;
|     o pointer to first prof_illNano struct to compare
|   - secProfSTPtr;
|     o pointer to second prof_illNano struct to compare
|   - cmpSmallLapBl:
|     o 1: compare event when only a few variants overlap
|     o 0: require at least one profile to have at least
|          the variants present in the other profile
|          (can have more)
| Output:
|   - Returns:
|     o > -1, number of differences between profiles
|       - def_unkown_illNano bases are treated as wild
|         cards
|     o 0 no differences
|     o < 0 no overlap
\-------------------------------------------------------*/
signed int
cmp_prof_illNano(
   struct prof_illNano *firstProfSTPtr, /*first struct*/
   struct prof_illNano *secProfSTPtr,   /*second struct*/
   signed char cmpSmallLapBl/*1 do not worry about exact*/
);

/*-------------------------------------------------------\
| Fun08: swap_prof_illNano
|   - swaps variables in two prof_illNano structs
| Input:
|   - firstProfSTPtr;
|     o pointer to first prof_illNano struct to swap
|   - secProfSTPtr;
|     o pointer to second prof_illNano struct to swap
| Output:
|   - Modifies:
|     o firstProfSTPtr to have values of secProfSTPtr
|     o secProfSTPtr to have values of firstProfSTPtr
| WARNING:
|   - does not swap nextST
\-------------------------------------------------------*/
void
swap_prof_illNano(
   struct prof_illNano *firstProfSTPtr, /*first struct*/
   struct prof_illNano *secProfSTPtr    /*second struct*/
);

/*-------------------------------------------------------\
| Fun09: merge_prof_illNano
|   - merges two prof_illNano structs into one profile
| Input:
|   - firstProfSTPtr;
|     o pointer to first prof_illNano struct to merge
|     o should have link to list
|   - secProfSTPtr;
|     o pointer to second prof_illNano struct to merge
| Output:
|   - Modifies:
|     o firstProfSTPtr to have merged profile
|     o secProfSTPtr to have umerged values to free
| WARNING:
|   - does not free the second structure
|   - assumes one struct has all varaints in both structs
|   - will swap first and second struct if needed, but
|     will not swap nextST (list order)
\-------------------------------------------------------*/
void
merge_prof_illNano(
   struct prof_illNano *firstProfSTPtr, /*first struct*/
   struct prof_illNano *secProfSTPtr    /*second struct*/
);

/*-------------------------------------------------------\
| Fun10: blank_profList_illNano
|   - blanks a profList_illNano struct (does nothing)
| Input:
|   - profListSTPtr;
|     o pointer to profList_illNano struct to blank
| Output:
\-------------------------------------------------------*/
void
blank_profList_illNano(
   struct profList_illNano *profListSTPtr
);

/*-------------------------------------------------------\
| Fun11: init_profList_illNano
|   - initializes a profList_illNano struct
| Input:
|   - profListSTPtr;
|     o pointer to profList_illNano struct to initialize
| Output:
|   - Modifies:
|     o all values in profListSTPtr to be 0 (null)
\-------------------------------------------------------*/
void
init_profList_illNano(
   struct profList_illNano *profListSTPtr
);

/*-------------------------------------------------------\
| Fun12: freeStack_profList_illNano
|   - frees variables (list) inside a profList_illNano
| Input:
|   - profListSTPtr;
|     o pointer to profList_illNano with variables to free
| Output:
|   - Frees:j
|     o heap allocated (list) variables in profListSTPtr
\-------------------------------------------------------*/
void
freeStack_profList_illNano(
   struct profList_illNano *profListSTPtr
);

/*-------------------------------------------------------\
| Fun13: freeHeap_profList_illNano
|   - frees a profList_illNano struct
| Input:
|   - profListSTPtr;
|     o pointer to profList_illNano struct to free
| Output:
|   - Frees:
|     o profListSTPtr (you must set to 0 [null])
\-------------------------------------------------------*/
void
freeHeap_profList_illNano(
   struct profList_illNano *profListSTPtr
);

/*-------------------------------------------------------\
| Fun14: getIllSnp_illNano
|   - gets snps from tbCon tsv file for Illumina data
| Input:
|   - minDepthUI:
|     o min read depth to keep varaint
|   - minPercDepthF:
|     o min percent read depth to keep varaint
|   - minPercDelF:
|     o min percent read depth to keep deletion; you
|       should use at least 50%, so deletion needs to be
|       major choice
|   - numVarUIPtr:
|     o pointer to unsigned int to hold the number of
|       variant positions found
|   - lenProfUI:
|     o pointer to unsigned int to hold length (final
|       reference position) of profile
|   - tsvFILE:
|     o FILE pointer to tsv file from tbCon
|   - errSCPtr:
|     o pointer to signed char to hold returned error
| Output:
|   - Modifies:
|     o numVarUIPtr to have number of variant positions
|     o lenProfUI to have reference position of last
|       variant
|     o tsvFILE to point to end of file (EOF)
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_illNano for memory errors
|       - def_fileErr_illNano for file errors
|   - Returns:
|     o signed char array with variants
|       o def_A_illNano if A supported
|       o def_C_illNano if C supported
|       o def_G_illNano if G supported
|       o def_T_illNano if T supported
|       o def_del_illNano if del supported
|       o def_noBase_illNano if no variant for position
|       o '\0' for end of variants (no more variants)
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
getIllSnp_illNano(
   unsigned int minDepthUI, /*min depth to keep snp*/
   float minPercDepthF,     /*min % depth to keep snp*/
   float minPercDelF,       /*min % depth to keep del*/
   unsigned int *numVarUIPtr,/*number variants found*/
   unsigned int *lenProfUI, /*length of final profile*/
   void *tsvFILE,           /*tsv with snps*/
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun15: getVarNano_illNano
|   - identifies variants in a nanopore read using an
|     Illumina profile
| Input:
|   - samSTPtr:
|     o pointer to samEntry stuct with read to scan
|   - profileStr:
|     o c-strint with variants (profile) to extract
|   - mapPosAryUI:
|     o unsigned int array to store variant positions in
|       (size needs to be number variants in profileStr)
|   - mapNtArySC:
|     o signed char array to store variant nucleotides in
|       (size needs to be number variants in profileStr)
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
| Output:
|   - Modifies:
|     o mapPosAryUI to have variant positions
|     o mapNtArySC to have variant nucleotdies
|   - Returns:
|     o number of detected variant positions
\-------------------------------------------------------*/
unsigned int
getVarNano_illNano(  
   struct samEntry *samSTPtr,/*read to scan*/
   signed char *profileStr,  /*illumina profile*/
   signed char tranBl,       /*1: is uknown transition*/
   unsigned int *mapPosAryUI,/*will have variant pos*/
   signed char *mapNtAryUI  /*will have variant bases*/
);

/*-------------------------------------------------------\
| Fun16: getNanoReads_illNano
|   - gets nanopore reads using Illumina profile
| Input:
|   - profileStr:
|     o c-strint with variants (profile) to extract
|   - lenProfileUI:
|     o position of last reference base in profile
|   - uniqProfSTPtr:
|     o pointer to profList_illNano struct to store unqiue
|       profile list (nothting done if 0)
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
|   - mergeOverBl:
|     o 1: merge overlaps were a read does not completly
|          cover, but their is no difference
|     o 0: only merge when at least on read completly
|          overlaps other
|   - samFILE:
|     o FILE pointer to sam file with reads to get
|   - outFILE:
|     o FILE pointer to file to print reads to
| Output:
|   - Modifies:
|     o samSTPtr to have last sam file entry
|     o buffStrPtr to have last line in sam flie
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|     o samFILE to point to end of file (EOF)
|   - Prints:
|     o read ids + profile to outFILE as tsv
|   - Returns:
|     o 0 for no errors
|     o def_memErr_illNano for memory errors
|     o def_fileErr_illNano for file errors
\-------------------------------------------------------*/
signed char
getNanoReads_illNano(  
   signed char *profileStr,  /*illumina profile*/
   unsigned int lenProfileUI,/*last ref position in prof*/
   struct profList_illNano *profListSTPtr,
   signed char tranBl,       /*1: is uknown transition*/
   signed char mergeOverBl,/*1 merge incompelte overlaps*/
   struct samEntry *samSTPtr,/*sam file reading*/
   void *samFILE,            /*sam file with ONT reads*/
   void *outFILE             /*file to save reads to*/
);

/*-------------------------------------------------------\
| Fun17: phead_profList_illNano
|   - prints header for unique profile tsv
| Input
|   - outFILE:
|     o FILE pointer to print unique profiler header to
| Output:
|   - Prints:
|     o header (unique profile) to outFILE
\-------------------------------------------------------*/
void
phead_profList_illNano(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun18: p_profList_illNano
|   - prints unique profiles in a profList struct
| Input
|   - profListSTPtr:
|     o pointer to profListSTPtr with profiles to print
|   - outFILE:
|     o FILE pointer to print unique profiler header to
| Output:
|   - Prints:
|     o unique profiles to outFILE
\-------------------------------------------------------*/
void
p_profList_illNano(
   struct profList_illNano *profListSTPtr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun19: run_illNano
|   - runs the illNano algorithim
| Input:
|   - minDepthUI:
|     o minimum depth to keep illumina variant
|   - minPercDepthF:
|     o minimum percent read depth to keep Illumina SNP
|   - minPercDelF:
|     o min percent read depth to keep Illumina deletion
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
|   - mergeOverBl:
|     o 1: merge overlaps were a read does not completly
|          cover, but their is no difference
|     o 0: only merge when at least on read completly
|          overlaps other
|   - illTsvFILE:
|     o FILE pointer to tbCon tsv file with Illumina vars
|   - samFILE:
|     o FILE pointer to sam file with reads to get
|   - outFILE:
|     o FILE pointer to file to print reads to
|   - uniqFILE:
|     o FILE pointer to file to print uniqe profiles to
| Output:
|   - Modifies:
|     o illTsvFILE to point to end of file (EOF)
|     o samFILE to point to end of file (EOF)
|   - Prints:
|     o read ids + profile to outFILE as tsv
|     o unique profiels to uniqFILE (if provided)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_illNano for memory errors
|     o def_fileErr_illNano for file errors
\-------------------------------------------------------*/
signed char
run_illNano(  
   unsigned int minDepthUI,  /*min depth for illumina*/
   float minPercDepthF,      /*min % depth for ill snp*/
   float minPercDelF,        /*min % depth for ill del*/
   signed char tranBl,       /*1: is uknown transition*/
   signed char mergeOverBl,/*1 merge incompelte overlaps*/
   void *illTsvFILE,         /*tbCon tsv file for ill*/
   void *samFILE,            /*sam file with ONT reads*/
   void *outFILE,            /*file to save reads to*/
   void *uniqFILE            /*file for unique profiles*/
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
