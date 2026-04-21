/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' edDist SOF: Start Of File
'   - has functions to find edit distances (does some
'     filtering)
'   o header:
'     - included libraries
'   o .h st01: res_edDist
'     - holds results and windows for edDist struct 
'   o fun01: blank_res_edDist
'     - blanks (sets 0) values in a res_edDist struct
'   o fun02: init_res_edDist
'     - initializes and res_edDist struct
'   o fun03: freeStack_res_edDist
'     - frees heap varaibles in a res_edDist struct
'   o fun04: freeHeap_res_edDist
'     - frees heap allocated res_edDist struct
'   o .c fun05: isTransNt_edDist
'     - checks to see if query base is a transversion
'     - not used but want to keep code
'   o fun06: readCmpDist_edDist
'     - gets edit distances between the query & reference
'   o fun07: dist_edDist
'     - gets edit distances for reference
'   o fun08: percDist_edDist
'     - finds probablity of reads matching by chance
'   o fun09: addReadToDepth_edDist
'     - adds a read to a depth profile
'   o fun10: mkDepthProfile_edDist
'     - finds depth of every base in samEntry reference
'   o fun11: phead_edDist
'     - prints header for edDist output tsv
'   o fun12: pdist_edDist
'     - prints edit distance tsv line for edDist
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and foward declerations
\-------------------------------------------------------*/

#ifndef SAM_FILE_CLUSTERING_H
#define SAM_FILE_CLUSTERING_H

struct samEntry;
struct seqST;

#define def_memErr_edDist -16
#define def_fileErr_edDist -32

#define def_noOverlap_edDist -1
#define def_diffRef_edDist -2
#define def_noSeq_edDist -4
#define def_noMap_edDist -8

/*-------------------------------------------------------\
| ST01: res_edDist
|   - holds results and windows for edDist struct 
\-------------------------------------------------------*/
typedef struct res_edDist
{
   unsigned int *depthAryUI;   /*depth array for profile*/
   unsigned long sizeDepthUL;  /*size of depthAryUI*/

   unsigned int probUI;    /*percent dist/error*/
   unsigned int overlapUI;     /*overlap between reads*/
   unsigned int startUI;       /*first shared base*/
   unsigned int numIndelUI;    /*number indels kept*/
   unsigned int indelEventsUI; /*number indel events*/

   unsigned int maxWinDistUI;  /*maximum dist in window*/
   unsigned int probMaxWinUI;  /*max window % dist/error*/
   unsigned int minWinDistUI;  /*minimum dist in window*/
   float avgWinDistF;          /*average dist in window*/

   unsigned int numFullWinUI;  /*number of full windows*/
   unsigned int extraWinNtUI; 
      /*number bases that did not fit in a window*/
   unsigned int lastWinDiffUI;
      /*number differences in last window*/

   signed long edDistSL;       /*edit distance*/
}res_edDist;

/*-------------------------------------------------------\
| Fun01: blank_res_edDist
|   - blanks (sets 0) values in a res_edDist struct
| Input:
|   - resSTPtr
|     o pointer to res_edDist struct to blank
| Output:
|   - Modifies:
|     o all values in resSTPtr, except depthAryUI and
|       sizeDepthUI to be 0
\-------------------------------------------------------*/
void
blank_res_edDist(
   struct res_edDist *resSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_res_edDist
|   - initializes and res_edDist struct
| Input:
|   - resSTPtr
|     o pointer to res_edDist struct to initialize
| Output:
|   - Modifies:
|     o all values in resSTPtr to be 0
\-------------------------------------------------------*/
void
init_res_edDist(
   struct res_edDist *resSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_res_edDist
|   - frees heap varaibles in a res_edDist struct
| Input:
|   - resSTPtr
|     o pointer to res_edDist struct with vars to free
| Output:
|   - Frees:
|     o depthAryUI; the calls init_res_edDist (fun02)
\-------------------------------------------------------*/
void
freeStack_res_edDist(
   struct res_edDist *resSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_res_edDist
|   - frees heap allocated res_edDist struct
| Input:
|   - resSTPtr
|     o pointer to res_edDist struct to free
| Output:
|   - Frees:
|     o resSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_res_edDist(
   struct res_edDist *resSTPtr
);

/*-------------------------------------------------------\
| Fun06: readCmpDist_edDist
|   - gets edit distances between two reads mapped to the
|     same reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o pointer to samEntry structure with reference to
|       compare query (qrySTPtr) to
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - minOverlapF:
|     o minimum percent overlap to score
|     o use 0 or negative value to not check overlap
|       - still checks to see reads share bases
|   - minDepthUI:
|     o minimum depth for reference base to keep
|       difference if using a depth profile (depthAryUI)
|   - winSizeUI:
|     o size of window for window error rates
|     o windows shifts by size every time
|   - resSTPtr:
|     o pointer to res_edDist struct to have results
|     o uses depthAryUI in resSTPtr to get depths
| Output:
|   - Modifies:
|     o numIndelUI in resSTPtr to have number kept indels
|     o indelEventsUI in resSTPtr to have number of indel
|       events
|     o overlapUI in resSTPtr to hold aligned length
|     o maxWinDistUI in resSTPtr to hold maximum number
|       of errors in all windows
|     o minWinDistUI in resSTPtr to hold minimum number
|       of errors in all windows
|     o avgWinDistF in resSTPtr to hold mean number of
|       errors in all widows
|     o edDistSL in resSTPtr to have edit distance
|   - Returns:
|     o edit distance between query and ref
|     o negative value if could not find distance
|       - def_noOveralp_edDist if reads to not overlap
|       - def_diffRef_edDist if reads have different ref
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
readCmpDist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct samEntry *refSTPtr, /*ref to compare*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   unsigned int minDepthUI,   /*min depth if profiling*/
   unsigned int winSizeUI,    /*size of one window*/
   struct res_edDist *resSTPtr/*results/depth profile*/
);

/*-------------------------------------------------------\
| Fun07: dist_edDist
|   - gets edit distances for mapped reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o reference sequence, if provided checks 'M' cases
|     o use 0 to not check
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - winSizeUI:
|     o size of window for window error rates
|     o windows shifts by size every time
|   - resSTPtr:
|     o pointer to res_edDist struct to have results
| Output:
|   - Modifies:
|     o numIndelUI in resSTPtr to have number kept indels
|     o indelEventsUI in resSTPtr to have number of indel
|       events
|     o overlapUI in resSTPtr to hold aligned length
|     o maxWinDistUI in resSTPtr to hold maximum number
|       of errors in all windows
|     o minWinDistUI in resSTPtr to hold minimum number
|       of errors in all windows
|     o avgWinDistF in resSTPtr to hold mean number of
|       errors in all widows
|     o edDistSL in resSTPtr to have edit distance
|   - Returns:
|     o edit distance for query and mapped reference
|     o negative value if could not find distance
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
dist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct seqST *refSTPtr,    /*has reference sequence*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   unsigned int winSizeUI,    /*size of one window*/
   struct res_edDist *resSTPtr/*holds results*/
);

/*-------------------------------------------------------\
| Fun08: percDist_edDist
|   - gives a rough idea on precentage of difference from
|     error
|   - not great, but allows lumping reads together
| Input:
|   - resSTPtr:
|     o pointer to res_edDist struct with edit distance
|       and overlap size
|   - winSizeUI:
|     o size of one window
|   - percErrF:
|     o expected percent of errors (0 to 1) in reads
|     o if doing read to read comparsions; double this
| Output:
|   - Returns:
|     o probablity of edit distance due to chance
\-------------------------------------------------------*/
signed int
percDist_edDist(
   struct res_edDist *resSTPtr,
   unsigned int winSizeUI,
   float percErrF
);

/*-------------------------------------------------------\
| Fun09: addReadToDepth_edDist
|   - adds a read to a depth profile
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - qrySTPtr:
|     o query to add to profile
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|     o use 0 or negative value to not check overlap
|       - still checks to see reads share bases
|   - resSTPtr:
|     o pointer to res_edDist struct with depth array to
|       store read depths in (must be length of reference)
| Output:
|   - Modifies:
|     o depthAryUI in resSTPtr to have query added
|   - Returns:
|     o 0 for no errors
|     o 1 if read was not added (failed filters)
\-------------------------------------------------------*/
signed char
addReadToDepth_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   struct samEntry *qrySTPtr, /*query samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   struct res_edDist *resSTPtr/*has depth profile array*/
);

/*-------------------------------------------------------\
| Fun10: mkDepthProfile_edDist
|   - finds the depth of every base in samEntry reference
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - resSTPtr:
|     o pointer to res_edDist struct to have depth profile
|   - samSTPtr:
|     o for reading each line in the sam file
|   - samFILE:
|     o sam file to scan
| Output:
|   - Modifies:
|     o samSTPtr to have last entry in file
|     o samFILE to point to start
|     o buffStrPtr to have last line in sam file and is
|       resized if needed
|     o lenBuffULPtr to new buffStrPtr size (if resized)
|     o depthAryUI in resSTPtr to have depth profile
|   - Returns:
|     o 0 for no errors
|     o def_memErr_edDist for memory errors
|     o def_fileErr_edDist for memory errors
\-------------------------------------------------------*/
signed char
mkDepthProfile_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   struct res_edDist *resSTPtr,/*has depth array*/
   struct samEntry *samSTPtr, /*for reading sam file*/
   void *samFILE
);

/*-------------------------------------------------------\
| Fun11: phead_edDist
|   - prints header for edDist output tsv
| Input:
|   - outFILE:
|     o FILE pointer to print header to
| Output:
|   - Prints:
|     o edDist header to outFILE
\-------------------------------------------------------*/
void
phead_edDist(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: pdist_edDist
|   - prints edit distance tsv line for edDist
| Input:
|   - resSTPtr:
|     o pointer to res_edDist struct with distance to
|       print
|   - qryIdStr:
|     o c-string with query id (name) to print
|   - refIdStr:
|     o c-string with reference id (name) to print
|   - outFILE:
|     o FILE pointer to print header to
| Output:
|   - Prints:
|     o stats in resSTPtr to outFILE
\-------------------------------------------------------*/
void
pdist_edDist(
   struct res_edDist *resSTPtr,
   signed char *qryIdStr,
   signed char *refIdStr,
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
