/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' tbCon SOF: Start Of File
'  - holds functions to make a quick consensus, more like
'    ivar. this is a bit memory intensive,
'    (40 + # insertions * (17) + bases in insertions)
'    bytes 
'   o header:
'     - Included libraries
'   o .h st01: insBase
'     - structure to hold an list of insetions for a
'       single position
'   o .h st02: conNt_tbCon
'     - structure to hold an single base in a sequence
'   o .h st03: set_tbCon
'     - holds the settings for tbCon
'   o fun01: initInsBase
'     - initializes an initBase structure
'   o fun02: mkIns_tbCon
'     - makes and initializes an ins_tbCon structure
'   o fun03: freeHeap_ins_tbCon
'     - Frees an heap allocated insBase structure
'   o fun04: blank_conNt_tbCon
'     - sets all variables in conNt_tbCon structure to 0
'   o fun05: init_conNt_tbCon
'     - initializes a conNt_tbCon structure
'   o fun06: mk_conNt_tbCon
'     - Makes an initialized, heap allocated conNt_tbCon
'       structure
'   o fun07: freeStack_conNt_tbCon
'     - frees heap allocated variables in a conNt_tbCon
'       structure (also initializes structure)
'   o fun08: freeHeap_conNt_tbCon
'     - frees a conNt_tbCon structure
'   o fun09: freeHeapAry_conNt_tbCon
'     - frees array of heap allocated conNt_tbCon structs
'   o fun10: init_set_tbCon
'     - initialize a set_tbCon struct to default settings
'   o fun11: freeStack_set_tbCon
'     - frees variables inside a set_tbCon struct
'   o fun12: addRead_tbCon
'     - adds read to a conNt_tbCon array
'   o fun13: collapse_tbCon
'     - Collapses a conNt_tbCon array into an array of
'       samEntry structures
'   o fun14: noFragCollapse_tbCon
'     - collapses a conNt_tbCon struct array into a single
'       samEntry struct (low read depth is masked)
'   o fun15: pvar_tbCon
'     - print entries in a conNt_tbCon structure array
'       that are above the minimum read depth
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Defined variables and header guards
\-------------------------------------------------------*/

#ifndef TB_CONSENSUS_H
#define TB_CONSENSUS_H

struct samEntry;

/*Settings for consensus buildiing*/

/*-------------------------------------------------------\
| ST01: ins_tbCon
|   - Structure to hold an list of insetions for a single
|     position
\-------------------------------------------------------*/
typedef struct ins_tbCon{
   signed char *insStr;  /*c-string with insertion*/
   signed int lenInsSI;  /*length of insertion*/
   signed int numHitsSI; /*number reads supporting a ins*/

   struct ins_tbCon *nextIns; /*Next insertion*/
}ins_tbCon;

/*-------------------------------------------------------\
| ST02: conNt_tbCon
|   - Structure to hold an single base in a sequence
\-------------------------------------------------------*/
typedef struct conNt_tbCon{
   signed int numASI;     /*number reads supporting a*/
   signed int numTSI;     /*number reads supporting t*/
   signed int numCSI;     /*number reads supporting c*/
   signed int numGSI;     /*number reads supporting g*/
   signed int numDelSI;   /*number reads supporting del*/

   signed int numInsSI;   /*number reads supporting ins*/
   struct ins_tbCon *insList; /*List of insertions*/

   signed int totalNtSI;  /*reads supporting position*/
   signed int ntKeptSI;   /*reads above min q-score*/
}conNt_tbCon;

/*-------------------------------------------------------\
| ST03: set_tbCon
|   - Structure to hold settings for tbCon
\-------------------------------------------------------*/
typedef struct set_tbCon{
   signed int minQSI;       /*min q-score to keep a base*/
   signed int minInsQSI;    /*min q-score to keep ins*/
   unsigned char minMapqUC; /*min mapq to keep a read*/

   /*min percent/depth/length to keep an match/mutation*/
   signed int minLenSI;     /*min fragment length*/
   signed int minDepthSI;   /*min depth to keep base*/
   float minPercSnpF;  /*min % support to keep snp/match*/
   float minPercDelF;  /*min % support to keep insertion*/
   float minPercInsF;  /*min % support to keep deletion*/

   /*to mask low quality/support cosnesus bases with*/
   signed char maskSC;      /*character to mask with*/

   /*min percent/depth to print an snp/match,ins,or del*/
   signed int minPrintDepthSI;
   float printMinSupSnpF;
   float printMinSupInsF;
   float printMinSupDelF;
}set_tbCon;

/*-------------------------------------------------------\
| Fun02: mkIns_tbCon
|   - makes and initializes an ins_tbCon structure
| Input:
| Output:
|   - Returns:
|     o pointer to initialized ins_tbCon structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct ins_tbCon *
mkIns_tbCon(
   void
);

/*-------------------------------------------------------\
| Fun03: freeHeap_ins_tbCon
|   - frees heap allocated insBase structure
| Input:
|   - insSTPtr:
|     o pointer to insBase structure to free
| Output:
|   - Frees:
|     o insSTPtr
|   - Returns:
|     o next insBase structure (nextIns) in insBase list
\-------------------------------------------------------*/
struct ins_tbCon *
freeHeap_ins_tbCon(
   struct ins_tbCon *insSTPtr
);

/*-------------------------------------------------------\
| Fun04: blank_conNt_tbCon
|   - sets all variables in conNt_tbCon structure to 0
| Input:
|   - conNtSTPtr:
|     o pointer to conNt_tbCon structure to blank
| Output:
|   - Modifies:
|     o all non-pointer values in conNtSTPtr to be 0
\-------------------------------------------------------*/
void
blank_conNt_tbCon(
   struct conNt_tbCon *conNtSTPtr
);

/*-------------------------------------------------------\
| Fun05: init_conNt_tbCon
|   - initializes a conNt_tbCon structure
| Input:
|   - conNtSTPtr:
|     o pointer to an conNt_tbCon structure to initialize
| Output:
|   - Modifies:
|     o all values (including pointers) in conNtSTPtr to
|       be 0
\-------------------------------------------------------*/
void
init_conNt_tbCon(
   struct conNt_tbCon *conNtSTPtr
);

/*-------------------------------------------------------\
| Fun06: mk_conNt_tbCon
|   - makes initialized, heap allocated conNt_tbCon struct
| Input:
| Output:
|   - Returns:
|     o pointer to an initialized conNt_tbCon structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct conNt_tbCon *
mk_conNt_tbCon(
   void
);

/*-------------------------------------------------------\
| Fun07: freeStack_conNt_tbCon
|   - frees heap allocated variables in a conNt_tbCon
|     structure (also initializes structure)
| Input:
|   - conNtSTPtr:
|     o pointer to conNt_tbCon struct to free variables
| Output:
|   - Frees:
|     o frees the conNt_tbConST->insList 
|   - Sets:
|     o all values in conNt_tbConSTPTr to 0
\-------------------------------------------------------*/
void
freeStack_conNt_tbCon(
   struct conNt_tbCon *conNtSTPtr
);

/*-------------------------------------------------------\
| Fun08: freeHeap_conNt_tbCon
|   - Frees a conNt_tbCon structure
| Input:
|   - conNtSTPtr:
|     o pointer toa conNt_tbCon structure to free
| Output:
|   - Frees:
|     o frees conNt_tbConST
\-------------------------------------------------------*/
void
freeHeap_conNt_tbCon(
   struct conNt_tbCon *conNtSTPtr
);

/*-------------------------------------------------------\
| Fun09: freeHeapAry_conNt_tbCon
|   - frees array of heap allocated conNt_tbCon structs
| Input:
|   - conNtAryST:
|     o pointer conNt_tbCon struct array to free
|   - lenArySI:
|     o number of conNt_tbCon structurs in array
| Output:
|   - Frees:
|     o array in conNtSTPtr
\-------------------------------------------------------*/
void
freeHeapAry_conNt_tbCon(
   struct conNt_tbCon *conNtAryST,
   signed int lenArySI
);

/*-------------------------------------------------------\
| Fun10: init_set_tbCon
|   - initialize a set_tbCon struct to default settings
| Input:
|   - setSTPtr:
|     o Pointer to the set_tbCon structure with settings
|       for tbCon to set to defaults
| Output:
|   - Modifies:
|     o set_tbConSTPtr to default settings
\-------------------------------------------------------*/
void
init_set_tbCon(
   struct set_tbCon *setSTPtr
);

/*-------------------------------------------------------\
| Fun11: freeStack_set_tbCon
|   - frees variables inside a set_tbCon struct
| Input:
|   - setSTPtr:
|     o pointer to the set_tbCon struct
| Output:
|   - Nothing:
|     o here for future use
\-------------------------------------------------------*/
void
freeStack_set_tbCon(
   struct set_tbCon *setSTPtr
);

/*-------------------------------------------------------\
| Fun12: addRead_tbCon
|   - adds read to a conNt_tbCon array
| Input:
|   - sameEntrySTPtr:
|     o pointer to a samEntry struct with read to add
|   - conNtAryST:
|     o pointer to conNt_tbCon struct array to add read to
|     o the array is resize if to small
|   - lenRefUI:
|     o length of the reference sequence; use 0 for unkown
|     o is updated as conNt_tbConSTAry expands
|   - settings:
|     o pointer to set_tbCon struct with settings
| Output:
|   - Modifies:
|     o conNtAryST to have new read
|     o lenRefUI to have new conNtAryST size if conNtAryST
|       is resized
|   - Returns:
|     o for no errors
|     o def_header_tbConDefs for sam file header entry
|     o def_noMap_tbConDefs for an unmapped read
|     o def_noSeq_tbConDefs for a read missing a sequence
|     o def_memErr_tbConDefs for memory errors
\-------------------------------------------------------*/
signed char
addRead_tbCon(
   struct samEntry *samSTPtr, /*read to add to consensus*/
   struct conNt_tbCon *conNtAryST[], /*Consensus array*/
   unsigned int *lenRefUI,       /*Length of reference*/
   struct set_tbCon *settings
);

/*-------------------------------------------------------\
| Fun13: collapse_tbCon
|   - collapses a conNt_tbCon strrucvt array into an array
|     of samEntry structs
| Input:
|   - conNtAryST:
|     o pointer to a conNt_tbCon struct array to collapse
|   - lenConAryUI:
|     o length of the conNt_tbCon array
|   - lenSamArySI:
|     o pointer to integer to have number of samEntry
|       structures made
|   - refIdStr:
|     o c-string with referernce sequence name
|   - settings:
|     o pointer to set_tbCon struct with consensus
|       building settings
|   - errSC:
|     o pointer to character to hold error type
| Output:
|   - Modifies:
|     o lenSamST to hold the returned samEntry arrray
|       length
|     o errSC:
|       - 0 for no errors
|       - def_noSeq_tbConDefs if could not build consensus
|       - def_memErr_tbConDefs if had memory error
|   - Returns:
|     o array of sam entry structures with consensus 
|       fragments.
|       - idealy this would be one structure, but for
|         amplicons it will be an array
|     o 0 for memory erors
\-------------------------------------------------------*/
struct samEntry *
collapse_tbCon(
   struct conNt_tbCon conNtAryST[], /*to collapse*/
   unsigned int lenConAryUI,   /*length of consensus*/
   signed int *lenSamArySI,    /*set to out array length*/
   signed char *refIdStr,      /*name of reference seq*/
   struct set_tbCon *settings, /*settings for collapsing*/
   signed char *errSC          /*error reports*/
);

/*-------------------------------------------------------\
| Fun14: noFragCollapse_tbCon
|   - collapses a conNt_tbCon struct array into a single
|     samEntry struct (low read depth is masked)
| Input:
|   - conNtAryST:
|     o pointer to a conNt_tbCon struct array to collapse
|   - lenConAryUI:
|     o length of the conNt_tbCon array
|   - refIdStr:
|     o c-string with referernce sequence name
|   - numMaskUIPtr:
|     o pointer to unsigned int to have number of bases
|       masked in consensus
|   - settings:
|     o pointer to set_tbCon struct with consensus
|       building settings
|   - errSC:
|     o pointer to character to hold error type
| Output:
|   - Modifies:
|     o lenSamST to hold the returned samEntry arrray
|       length
|     o numMaskUIPtr to have number of masked bases
|     o errSC:
|       - 0 for no errors
|       - def_noSeq_tbConDefs if could not build consensus
|       - def_memErr_tbConDefs if had memory error
|   - Returns:
|     o array of sam entry structures with consensus 
|       fragments.
|       - idealy this would be one structure, but for
|         amplicons it will be an array
|     o 0 for memory erors
\-------------------------------------------------------*/
struct samEntry *
noFragCollapse_tbCon(
   struct conNt_tbCon conNtAryST[], /*to collapse*/
   unsigned int lenConAryUI,   /*length of consensus*/
   signed char *refIdStr,      /*name of reference seq*/
   unsigned int *numMaskUIPtr, /*# bases masked*/
   struct set_tbCon *settings, /*settings for collapsing*/
   signed char *errSC          /*error reports*/
);

/*-------------------------------------------------------\
| Fun15: pvar_tbCon
|   - Print entries in a conNt_tbCon structure array that
|     are above the minimum read depth
| Input:
|   - conNtAryST:
|     o pointer to a conNt_tbCon structure array to print
|   - lenConAryUI:
|     o length of the conNt_tbCon array
|   - minDepthSI:
|     o minimum read depth to print out a position
|   - refIdStr:
|     o c-string with referernce sequence name
|   - outFILE:
|     o c-string with name of file to print everything to
| Output:
|   - Prints:
|     o entries in conNtAryST to outFILE
|   - Returns:
|     o 0 for success
|     o def_fileErr_tbConDefs for file errors
\-------------------------------------------------------*/
char
pvar_tbCon(
   struct conNt_tbCon conNtAryST[], /*consensus array*/
   unsigned int lenConAryUI,     /*length of conNtAryST*/
   signed char *refIdStr,        /*name of reference seq*/
   struct set_tbCon *settings,   /*settings for printing*/
   signed char *outStr           /*file to print to*/
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
