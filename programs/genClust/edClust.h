/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' edClust SOF: Start Of File
'   - clusters reads by similarity using edit distance
'   o header:
'     - included libraries
'   o fun01: getBestRead_edClust
'     - finds read with best score from a score array
'   o fun02: depthProf_edClust
'     - find the depth profile for a read
'   o fun03: findNumMap_edClust
'     - finds number of reads mapping to best read
'   o fun04: getDepth_edClust
'     - gets number of reads that overlap a cluster
'   o fun05: cluster_edClust
'     - clusters reads
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations, defined variables, and guards
\-------------------------------------------------------*/

#ifndef EDIT_DISTANCE_CLUSTER_H
#define EDIT_DISTANCE_CLUSTER_H

struct samEntry;
struct set_tbCon;
struct res_edDist;

struct set_clustST ;
struct index_clustST;
struct con_clustST;

#define def_memErr_edClust -1
#define def_fileErr_edClust -2
#define def_noReads_edClust -4

/*-------------------------------------------------------\
| Fun01: getBestRead_edClust
|   - finds read with best score from a score array; fun01
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct to have best read
|   - indexSTPtr:
|     o pointer to index_clustST with score array to
|       search
|   - buffStrPtr:
|     o c-string to use reading sam file
|     o this must be big enough to get the entire line
|       use buffer used with mk_index_clustST to get this
|   - samFILE:
|     o sam file to find best read in
| Output:
|   - Modifies:
|     o samFILE to be at start
|     o samSTPtr to have selected read
|   - Returns:
|     o index of selected read in scoreArySC
|     o def_memErr_edClust for memory errors
|     o def_fileErr_edClust for file errors
|     o def_noReads_edClust if no reads in file
\-------------------------------------------------------*/
signed long
getBestRead_edClust(
   struct samEntry *samSTPtr,
   struct index_clustST *indexSTPtr,
   signed char *buffStr,
   void *samFILE
);

/*-------------------------------------------------------\
| Fun02: depthProf_edClust
|   - find the depth profile for a read
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference (read)
|       to get profile for
|   - refNumUI:
|     o reference the reference read was mapped to
|   - clustSI:
|     o cluster working on
|   - indexSTPtr:
|     o pointer to index_clustST struct with file index's
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with settings
|   - resEdDistSTPtr:
|     o pointer to res_edDist struct for holding edit
|       distance results
|   - samSTPtr:
|     o pointer to samEntry struct to use in file reading
|   - buffStr:
|     o pointer to c-string use as buffer in file reading
|     o needs to be able to hold largest line in file
|   - samFILE:
|     o FILE pointer to sam file with reads
| Output:
|   - Modifies:
|     o samSTPtr to have last read in file
|     o buffStr to have last line in file
|     o samFILE to point to the start
|     o errSCPtr to hold error messages:
|  - Returns:
|    o 0 for no errors
|    o def_memErr_edClust for memory error
|    o def_fileErr_edClust for file error
\-------------------------------------------------------*/
signed char
depthProf_edClust(
   struct samEntry *refSTPtr, /*read to get profile for*/
   unsigned int refNumUI,   /*reference read mapped to*/
   signed int clustSI,               /*cluster on*/
   struct index_clustST *indexSTPtr, /*has file index's*/
   struct set_clustST *clustSetSTPtr,/*settings*/
   struct res_edDist *resEdDistSTPtr,/*for edDist result*/
   struct samEntry *samSTPtr,        /*for reading file*/
   signed char *buffStr,             /*for reading file*/
   void *samFILE                     /*sam file*/
);

/*-------------------------------------------------------\
| Fun03: findNumMap_edClust
|   - finds number of reads mapping to best read
| Input:
|   - numReadsSL:
|     o pointer to unsigned long to have number of mapped
|       reads
|   - lenConUI:
|     o pointer to unsigned int to have consensus length
|   - bestSTPtr:
|     o pointer to samEntry struct with best read
|   - bestIndexSL:
|     o index of best read (return from fun01)
|   - clustSetSTPtr:
|     o has settings for filtering and stores the first
|       and last possible base of the consensesus
|   - indexSTPtr:
|     o pointer to index_clustST struct with reads index's
|       and open reads
|   - resEdDistSTPtr:
|     o pointer to res_edDist struct to hold results from
|       edDist comparision
|   - clustSI:
|     o cluster working on (what to assign to targArySI)
|   - conBl:
|     o 1: use consensus to read error rate
|     o 0: use read to read error rate
|   - buffStrPtr:
|     o c-string to use reading sam file
|     o should be able to hold longest line in file
|       (fun19 sets this up)
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - samFILE:
|     o sam file with reads to compare to best read
| Output:
|   - Modifies:
|     o numReadsUL to have number of reads that mapped or
|       the error
|       - def_memErr_edClust for memory errors
|       - def_fileErr_edClust for file errors
|     o lenConUI to have length of returend consensus
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o startUI and endUI in clustSetSTPtr to have start
|       and end of the consensus
|     o clustArySI in indexSTPtr to have hits assigned to
|       clustSI
|   - Returns:
|     o conNt_tbCon array with uncollapsed consensus
\-------------------------------------------------------*/
struct conNt_tbCon *
findNumMap_edClust(
   signed long *numReadsSL,         /*num mapped reads*/
   unsigned int *lenConUI,          /*consensus length*/
   struct samEntry *bestSTPtr,      /*best read for map*/
   signed long bestIndexSL,         /*best read index*/
   struct set_clustST *clustSetSTPtr,/*cluster settings*/
   struct set_tbCon *conSetSTPtr,   /*consensus settings*/
   struct index_clustST *indexSTPtr,/*has read index's*/
   struct res_edDist *resEdDistSTPtr, /*for edDist*/
   signed int clustSI,              /*cluster working on*/
   signed char conBl,             /*1: best is consensus*/
   struct samEntry *samSTPtr,     /*for reading sam file*/
   signed char *buffStr,          /*for reading sam file*/
   void *samFILE                  /*has reads*/
);

/*-------------------------------------------------------\
| Fun04: getDepth_edClust
|   - gets number of reads that overlap a cluster
| Input:
|   - conListSTPtr:
|     o pointer to head of con_clustST struct list with
|       cluster consensus to check depths for
|   - indexSTPtr:
|     o pointer to index_clustST struct with each reads
|       assigned cluster, start position, and end position
|   - minOverlapF:
|     o mininum overlap to count read and cluster as
|       overlapping (for total count)
| Output:
|   - Modifies:
|     o maxReadsUL in each con_clustST struct in
|       conListSTPtr to have number of possible overlaping
|       reads
\-------------------------------------------------------*/
void
getMaxDepth_edClust(
   struct con_clustST *conListSTPtr,
   struct index_clustST *indexSTPtr,
   float minOverlapF
);

/*-------------------------------------------------------\
| Fun05: cluster_edClust
|   - clusters reads
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST pointer to set to
|       clusters (should be null if not; is freeded)
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with cluster setting
|   - conSetSTPtr:
|     o pointer to set_tbCon struct with consensus setting
|   - samSTPtr:
|     o pointer to samEntry struct to use in reading file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam files
|   - lenBuffULPtr:
|     o pointer to unsigned long with buffStrPtr length
|   - samFILE:
|     o sam file with reads to cluster
|   - logFILE:
|     o file to print log to (use 0 for no logging)
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o all arries clustSTPtr to have clusters and index's
|     o buffStrPtr to be resized to longest line in file
|     o lenBuffULPtr to hav buffStrPtr's length
|     o samFILE to point to start
|     o clustSI in clustSetSTPtr to be on last cluster
|     o errSC:
|       o 0 for no errors
|       o def_memErr_edClust for memory errors
|       o def_fileErr_edClust for file errors
|       o def_noReads_edClust if to few reads
|   - Prints:
|     o log (stats; results, and errors) to logFILE
|   - Returns:
|     o list of con_clustST structs with consensus
|       clusters
\-------------------------------------------------------*/
struct con_clustST *
cluster_edClust(
   struct index_clustST **indexSTPtr,
   struct set_clustST *clustSetSTPtr,
   struct set_tbCon *conSetSTPtr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE,
   void *logFILE,
   signed char *errSCPtr
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
