/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' kmerCnt SOF: Start Of File
'   - holds functions to count number of matching kmers in
'     sequences
'   o header:
'     - forward declerations, defined variables, guards
'   o fun01: blank_kmerCnt
'     - blanks all variables in a kmerCnt structure
'   o fun02: init_kmerCnt
'     - initialize a kmerCnt structure
'   o fun04: freeStack_kmerCnt
'     - frees variables inside a kmerCnt structure
'   o fun05: freeHeap_kmerCnt
'     - frees a kmerCnt structure (you set to 0)
'   o fun06: freeStackAry_kmerCnt
'     - frees internal memory form all kmerCnt structure
'       in an array
'   o fun07: freeHeapAry_kmerCnt
'     - frees an array of kmerCnt structure (you set to 0)
'   o .c fun08: mkKmerMask_kmerCnt
'     - makes a kmer mask for removing extra bases from
'       kmer
'   o fun09: addSeq_kmerCnt
'     - adds a sequence to a kmerCnt structure
'   o .c fun10: dualSort_kmerCnt
'     - sorts first unsigned int array and keeps second
'       signed int array in order with first
'   o fun11: ntToKmerAry_kmerCnt
'     - converts a nucleotide sequence to a array of kmer
'       counts
'   o fun12: get_kmerCnt
'     - gets number of matching kmers between a kmerCnt
'       structure and the kmer arrays
'   o fun13: faToKmerCnt_kmerCnt
'     - converts the sequences in a fasta file to a
'       kmerCnt array
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declerations, defined variables, guards
\-------------------------------------------------------*/

#ifndef KMER_COUNT_H
#define KMER_COUNT_H

struct seqST;

#define def_noKmer_kmerCnt -1
#define def_endKmers_kmerCnt -2
#define def_bitsPerKmer_kmerCnt 2 /*do not change*/

#define def_noMatch_kmerCnt 1
#define def_fileErr_kmerCnt 2
#define def_memErr_kmerCnt 4

/*-------------------------------------------------------\
| ST01: kmerCnt
|   - holds the number of kmers in each sequence
\-------------------------------------------------------*/
typedef struct kmerCnt
{
   unsigned char lenKmerUC;  /*bases in one kmer*/
   unsigned int maxKmersUI;  /*maximum number of kmers*/

   signed int *forKmerArySI; /*array of forward kmers*/
   unsigned int forKmersUI;  /*kmers in sequence*/

   signed int *revKmerArySI; /*array of reverse kmers*/
   unsigned int revKmersUI;  /*kmers in sequence*/

   /*costs a bit more to have both, but saves a little
   `  time later
   */
   struct seqST *forSeqST; /*forward sequence*/
   struct seqST *revSeqST; /*reverse seqeunce*/
}kmerCnt;

/*-------------------------------------------------------\
| Fun01: blank_kmerCnt
|   - blanks all variables in a kmerCnt structure
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCnt structure to blank
| Output:
|   - Modifies:
|     o all variables in kmerCntSTPtr to be blank settings
\-------------------------------------------------------*/
void
blank_kmerCnt(
   struct kmerCnt *kmerCntSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_kmerCnt
|   - initialize a kmerCnt structure
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCnt structure to initialize
| Output:
|   - Modifies:
|     o all values in kmerCntSTPtr to be 0 or null
\-------------------------------------------------------*/
void
init_kmerCnt(
   struct kmerCnt *kmerCntSTPtr
);

/*-------------------------------------------------------\
| Fun03: setup_kmerCnt
|   - sets up kmerCnt structure for use
|     (memory allocations)
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCnt structure to initialize
|   - lenKmerUC:
|     o length of one kmer
| Output:
|   - Modifies:
|     o lenKmerUC to be one kmer length
|     o maxKmersUI to be the maximum number of kmers
|     o totalKmersUI to be 0 (by blank_kmerCnt)
|   - Allocates:
|     o maxKmersUI uints for forKmerArySI and revKMerArySI
|     o seqST for forSeqST and revSeqST
|   - Returns:
|     o 0 for no problems
|     o def_memErr_kmerCnt for memory errors
\-------------------------------------------------------*/
signed char
setup_kmerCnt(
   struct kmerCnt *kmerCntSTPtr,
   unsigned char lenKmerUC
);

/*-------------------------------------------------------\
| Fun04: freeStack_kmerCnt
|   - frees variables inside a kmerCnt structure
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCnt structure with variables to
|       free
| Output:
|   - Frees:
|     o forKmerArySI, revKmerArySI, forSeqST, and revSeqST
|   - Sets:
|     o blanks structure and sets pointers to 0
|       - you will needed to call init_kmerCnt again
|       - only lenKmerUC and maxKMerUC are not set to 0
\-------------------------------------------------------*/
void
freeStack_kmerCnt(
   struct kmerCnt *kmerCntSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeap_kmerCnt
|   - frees a kmerCnt structure (you set to 0)
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCnt structure to free
| Output:
|   - Frees:
|     o kmerCntSTPtr
\-------------------------------------------------------*/
void
freeHeap_kmerCnt(
   struct kmerCnt *kmerCntSTPtr
);

/*-------------------------------------------------------\
| Fun06: freeStackAry_kmerCnt
|   - frees internal memory form all kmerCnt structure
|     in an array
| Input:
|   - kmerCntAryST:
|     o pointer to an array of kmerCnt structures to
|       all variables for (goes to uninitialzed state)
|   - numSTUI:
|     o number of initialized kmerCnt structures in
|       kmerCntAryST (actually have internal memory)
| Output:
|   - Frees:
|     o forKmerArySI, revKmerArySI, forSeqST, and revSeqST
|       for numSTUI kmerCnt structures in the array
|   - Sets:
|     o blanks structure and sets pointers to 0
|       - you will needed to call init_kmerCnt again
\-------------------------------------------------------*/
void
freeStackAry_kmerCnt(
   struct kmerCnt *kmerCntAryST,
   unsigned int numSTUI
);

/*-------------------------------------------------------\
| Fun07: freeHeapAry_kmerCnt
|   - frees an array of kmerCnt structure (you set to 0)
| Input:
|   - kmerCntAryST:
|     o pointer to an array of kmerCnt structures to free
|   - numSTUI:
|     o number of initialized kmerCnt structures in
|       kmerCntAryST (actually have internal memory)
| Output:
|   - Frees:
|     o kmerCntAryST
\-------------------------------------------------------*/
void
freeHeapAry_kmerCnt(
   struct kmerCnt *kmerCntAryST,
   unsigned int numSTUI
);

/*-------------------------------------------------------\
| Fun09: addSeq_kmerCnt
|   - adds a sequence to a kmerCnt structure
| Input:
|   - kmerCntSTPtr:
|     o pointer to kmerCnt structure to add sequence to
|   - seqSTPtr:
|     o pointer to a seqST with sequence to add
| Output:
|   - Modifies:
|     o kmerCntSTPtr to have the new sequence
|   - Returns:
|     o 0 for no errors
|     o def_memErr_kmerCnt for memory errors
\-------------------------------------------------------*/
signed char
addSeq_kmerCnt(
   struct kmerCnt *kmerCntSTPtr,/*will hold new sequence*/
   struct seqST *seqSTPtr   /*sequence to copy*/
);

/*-------------------------------------------------------\
| Fun11: ntToKmerAry_kmerCnt
|   - converts a nucleotide sequence to a array of kmer
|     counts
| Input:
|   - seqSTPtr:
|     o pointer to a seqST with sequence to get kmer
|       counts for
|   - lenKmerUC:
|     o length of one kmer
|   - kmerArySI:
|     o pointer to a signed int array to add kmer so
|     o needs to be size of ((lenKmerUC ^ 4) + 1)
|   - cntArySI:
|     o pointer to a signed int array to add kmer counts
|       to
|     o needs to be size of ((lenKmerUC ^ 4) + 1)
| Output:
|   - Modifies:
|     o kmerArySI to hold kmers in seqSTPtr
|     o cntArySI to hold number times each kmer happened
|     o sorts kmerArySI and cntArySI by kmer
|       - this converts the hash table to a list of kmers
|       - end will be marked with a -2
|   - Returns:
|     o number of kmers in sequence
\-------------------------------------------------------*/
signed int
ntToKmerAry_kmerCnt(
   struct seqST *seqSTPtr, /*sequence to convert*/
   unsigned char lenKmerUC,    /*length of one kmer*/
   signed int *kmerArySI,      /*will hold uniqe kmers*/
   signed int *cntArySI        /*gets # kmer duplicates*/
);

/*-------------------------------------------------------\
| Fun12: get_kmerCnt
|   - gets number of matching kmers between a kmerCnt
|     structure and the kmer arrays
| Input:
|   - kmerCntSTPtr:
|     o pointer to a kmerCntSTPtr structure with the kmer
|       table to compare to
|   - kmerArySI:
|     o pointer to a singed in array with the unique kmers
|       in the sequence (use fun11; ntToKmerAry_kmerCnt)
|   - cntArySI:
|     o pointer to a singed in array with the number of
|       duplicates per unique kmers in the sequence (use
|       fun11; ntToKmerAry_kmerCnt)
| Output:
|   - Returns:
|     o highes number of kmers found
|       - + signed long if forward sequence more kmers
|       - - signed long if reverse sequence more kmers
\-------------------------------------------------------*/
signed int
get_kmerCnt(
   struct kmerCnt *kmerCntSTPtr, /*table to get counts*/
   signed int *kmerArySI,        /*sequence uniqe kmers*/
   signed int *cntArySI          /*sequence kmer counts*/
);

/*-------------------------------------------------------\
| Fun13: faToKmerCnt_kmerCnt
|   - converts a the sequences in a fasta file to a
|     kmerCnt array
| Input:
|   - faFileStr:
|     o path to and name of fasta file with sequnces
|   - lenKmerUC:
|     o length of one kmer
|   - numSeqUI:
|     o pointer to unsigned int to hold the number of
|       sequences in the kmerCnt array
|   - errSCPtr:
|     o pointer to signed char to hold the errors
| Output:
|   - Modfies:
|     o numSeqUI to have the number of sequences in the
|       returned kmerCnt array
|     o errSCPtr to hold errors
|       - 0 for no erors
|       - def_fileErr_kmerCnt if could not open faFileStr
|       - def_memErr_kmerCnt for memory errors
\-------------------------------------------------------*/
struct kmerCnt *
faToKmerCnt_kmerCnt(
   signed char *faFileStr,/*path to fasta file*/
   unsigned char lenKmerUC, /*length of one kmer*/
   unsigned int *numSeqUI,/*will hold number of sequnces*/
   signed char *errSCPtr  /*holds error message*/
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
