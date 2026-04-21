/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' seqST SOF: Start Of File
'   - holds functions for reading in or manipulating
'     sequences
'   o header:
'     - guards and defined variables
'   o .h st01: seqST
'     - holds an single sequence (fasta/fastq)
'   o fun02 getFq_seqST:
'     - reads a fastq sequence from a fastq file
'   o fun03 getFa_seqST:
'     - grabs the next read in the fasta file
'   o fun05 revComp_seqST:
'     - reverse complement a sequence
'   o fun06 blank_seqST:
'     - sets values in seqST to zero
'   o fun07 init_seqST:
'     - sets values in seqST to blank values
'   o fun08 freeStack_seqST:
'     - frees variables in an seqST (calls blank_seqST)
'   o fun09 freeHeap_seqST:
'     - frees an seqST strucuter (calls fredSeqSTStack)
'   o fun10: freeHeapAry_seqST
'     - frees an array of seqST's
'   o fun11 cpIdEndPad_seqST:
'      - copies read id to a buffer and adds in endIdC to
'        the end. If needed, this function will add right
'        padding of spaces to the end.
'   o fun12: cp_seqST
'     - copies an seqST structure
'   o fun13: swap_seqST
'     - swaps values in two seqST structs
'   o fun14: sort_seqST
'     - sorts an array of seqST structs
'   o fun15: search_seqST
'     - searchs for sequence in seqST struct array
'   o fun16: mkAry_seqST
'     - makes a seqST struct array
'   o fun17: realloc_seqST
'     - add more memory to a seqST struct array
'   o fun18: readFaFile_seqST
'     - get all sequences from a fasta file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - guards and defined variables
\-------------------------------------------------------*/

#ifndef SEQUENCE_STRUCTURE_H
#define SEQUENCE_STRUCTURE_H

#define def_EOF_seqST 1
#define def_fileErr_seqST 2
#define def_badLine_seqST 16 /*invald fastx entry*/
#define def_memErr_seqST 4

/*-------------------------------------------------------\
| ST01: seqST
|  - holds an single sequence (fasta/fastq)
\-------------------------------------------------------*/
typedef struct seqST
{
  signed char *idStr;         /*sequence id*/
  signed long  idLenSL;       /*length of sequence id*/
  signed long  idSizeSL;      /*length of Id buffer*/

  signed char *seqStr;        /*sequence*/
  signed long  seqLenSL;      /*length of the sequence*/
  signed long  seqSizeSL;     /*length sequence buffer*/

  signed char *qStr;          /*q-score entry*/
  signed long  qLenSL;        /*length of the q-score*/
  signed long  qSizeSL;       /*length of q-score buffer*/

  signed long  offsetSL;      /*offset for an alignment*/
  signed long  endAlnSL;      /*marks end of alignment*/
}seqST;

/*-------------------------------------------------------\
| Fun02: getFq_seqsT
|  - grabs the next read in the fastq file
| Input:
|  - fqFILE:
|    o pointer to FILE handle to a fastq file to get the
|      next read from
|  - seqST:
|    o pointer to seqST structure to hold next read
| Output:
|  - Modifies:
|    o refStruct to hold the read in fastq entry & sets
|      its pointers
|  - Returns:
|     o 0: for no errors
|     o def_EOF_seqST: if EOF
|     o def_fileErr_seqST If file was not a fastq file
|     o def_badLine_seqST | def_fileErr_seqST for  a
|       invalid fastq entry
|     o def_memErr_seqST If malloc failed to find memory
\-------------------------------------------------------*/
signed char
getFq_seqST(
  void *fqFILE,           /*fastq file with sequence*/
  struct seqST *seqSTPtr /*will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun03 TOC: getFa_seqST
|  -  grabs the next read in the fasta file
| Input:
|  - faFILE:
|    o Pointer to FILE handle to a fasta file to get the
|      next read from
|  - seqST:
|    o pointer to seqST structure to hold next read
| Output:
|  - Modifies:
|    o seqST to hold one fasta entry
|  - Returns:
|     o 0: for no errors
|     o def_EOF_seqST: if EOF
|     o def_fileErr_seqST If file was not a fastq file
|     o def_badLine_seqST | def_fileErr_seqST for  a
|       invalid fasta entry
|     o def_memErr_seqST If malloc failed to find memory
| Note:
|   - this will remove new lines from the sequence.
|   - this will only remove spaces or tabs at the end of
|     each sequence entry, so "atg atc \n" will got to
|     "atc atc".
\-------------------------------------------------------*/
signed char
getFa_seqST(
  void *faFILE,           /*fasta file with sequence*/
  struct seqST *seqSTPtr  /*will hold one fastq entry*/
);

/*-------------------------------------------------------\
| Fun05: revComp_seqST
|  - reverse complement a sequence
| Input:
|  - seqST:
|    o pointer to seqST structure with a sequence to
|      reverse complement
| Output:
|  - Modfies
|    o reverse complements the sequence entry and if
|      reverses the Q-score entry
\-------------------------------------------------------*/
void
revComp_seqST(
  struct seqST *seqSTPtr  /*sequence to reverse comp*/
);


/*-------------------------------------------------------\
| Fun06: blank_seqST
|  - sets values in seqST to blank values
| Input:
|  - seqSTPtr:
|    o pointer to an seqST structuer to blank
| Output:
|  - Modifies
|    o sets id, sequence, and Q-score entreis to start
|      with '\0' and the id, sequence, and Q-score lengths
|      to 0. This does not change the buffer lengths.
\-------------------------------------------------------*/
void
blank_seqST(
   struct seqST *seqSTPtr
);

/*-------------------------------------------------------\
| Fun07: init_seqST
|  - sets vlues in seqST to zero
| Input:
|  - seqSTPtr:
|    o Pointer to an seqST structuer to initialize
| Output:
|  - Modifies
|    o all values in seqST to be 0
\-------------------------------------------------------*/
void
init_seqST(
   struct seqST *seqSTPtr
);

/*-------------------------------------------------------\
| Fun08: freeStack_seqST
|  - frees the variables in an seqST strucuter
| Input:
|  - seqSTPtr:
|    o pointer to an seqST structuer that has variables
|      to free
| Output:
|  - Frees
|    o seqSTPtr-idStr and sets to 0
|    o seqSTPtr-seqStr and sets to 0
|    o seqSTPtr-qStr and sets to 0
\-------------------------------------------------------*/
void
freeStack_seqST(
   struct seqST *seqSTPtr
);

/*-------------------------------------------------------\
| Fun09: freeHeap_seqST
|  - frees the seqST strucuter
| Input:
|  - seqSTPtr:
|    o pointer to an seqST structuer to free
| Output:
|  - Frees
|    o seqST from memory and sets to 0
\-------------------------------------------------------*/
void
freeHeap_seqST(
   struct seqST *seqSTPtr
);

/*-------------------------------------------------------\
| Fun10: freeHeapAry_seqST
|  - frees an array of seqST's
| Input:
|  - seqAryST:
|    o pointer to an arrya of seqST structuer to free
|  - numSeqSI:
|    o number of seqSTs in the seqST array
|     (seqSTAryPtr)
| Output:
|  - Frees
|    o seqST from memory
\-------------------------------------------------------*/
void
freeHeapAry_seqST(
   struct seqST *seqAryST,
   signed long  numSeqSL
);

/*-------------------------------------------------------\
| Fun11: cpIdEndPad_seqST
|  - copies read id to a buffer and adds in endIdC to
|    the end. If needed, this function will add right
|    padding of spaces to the end.
| Input:
|  - seqST:
|    o pointer to seqST with read id to copy
|  - buffStr:
|    o c-string to copy read id to
|  - endIdC:
|    o character to add in to mark the end of the read id
|  - padRI:
|    o amount of padding to add to the right of the read
|      id
| Output:
|  - Modifies:
|    o buffStr to hold the sequence id + endIdC. If the
|      id + endIdC is shorter than padRI, the copied id is
|      padded with spaces on the right till it is the same
|      size as padRI.
|  - Returns:
|    o pointer to end of copied id or padding if padding
|      was applied.
\-------------------------------------------------------*/
signed char *
cpIdEndPad_seqST(
   struct seqST *seqSTPtr,  /*has read id to copy*/
   signed char *buffStr, /*buffer to add read id to*/
   signed char endIdC,   /*padding; id end (0 to skip)*/
   signed int padRI      /*padding; id start*/
);

/*-------------------------------------------------------\
| Fun12: cp_seqST
|   - Copies an seqST structure
| Input:
|   - dupSeqST:
|     o Pointer to an seqST to copy to (duplicate)
|   - cp_seqST:
|     o Pointer to an seqST to copy
| Output:
|   - Modifies:
|     o dupSeqST to be an copy of cp_seqST
|   - returns:
|     o 0 for no errors
|     o def_memErr_seqST for an memory error
\-------------------------------------------------------*/
signed char
cp_seqST(
   struct seqST *dupSeqST, /*copy to (duplicate)*/
   struct seqST *cpSeqST   /*seqSTPtr to copy*/
);

/*-------------------------------------------------------\
| Fun13: swap_seqST
|   - swaps values in two seqST structs
| Input:
|   - firstSTPtr:
|     o pointer first seqST struct to swap
|   - secSTPtr:
|     o pointer second seqST struct to swap
| Output:
|   - Modifies:
|     o firstSTPtr to have values from secSTPtr
|     o secSTPtr to have values from firstSTPtr
\-------------------------------------------------------*/
void
swap_seqST(
   struct seqST *firstSTPtr,
   struct seqST *secSTPtr
);

/*-------------------------------------------------------\
| Fun14: sort_seqST
|   - sorts an array of seqST structs
| Input:
|   - arySTPtr:
|     o pointer to array of seqST struct to sort
|   - lenSL:
|     o length to sort in array
| Output:
|   - Modifies:
|     o arySTPtr to be sorted by read/ref id
\-------------------------------------------------------*/
void
sort_seqST(
   struct seqST *arySTPtr, /*array to sort*/
   signed long lenSL       /*length of array*/
);

/*-------------------------------------------------------\
| Fun15: search_seqST
|  - searchs for sequence in seqST struct array
| Input:
|  - seqArySTPtr:
|    o pionter to seqST stuct array to search
|  - qryStr:
|    o query to find
|  - lenSL:
|    o length of seqArySTPtr (index 1)
| Output:
|  - Returns:
|    o index of qryStr in seqArySTPtr
|    o -1 if qryUL is not in ulAry
\-------------------------------------------------------*/
signed long
search_seqST(
   struct seqST *seqArySTPtr, /*sequence array to search*/
   signed char *qryStr,       /*query to hunt for*/
   signed long lenSL          /*length to search in*/
);

/*-------------------------------------------------------\
| Fun16: mkAry_seqST
|  - makes a seqST struct array
| Input:
|  - sizeSL:
|    o size of seqST struct array
| Output:
|  - Returns:
|    o pointer to seqST struct array
|    o 0 for memory errors
\-------------------------------------------------------*/
struct seqST *
mkAry_seqST(
   signed long sizeSL         /*new length (size)*/
);

/*-------------------------------------------------------\
| Fun17: realloc_seqST
|  - add more memory to a seqST struct array
| Input:
|  - seqArySTPtr:
|    o pionter to seqST stuct array to expand
|  - lenSL:
|    o index of last item
|    o this marks the start of initialization
|  - sizeSL:
|    o new size of seqST struct array
| Output:
|  - Returns:
|    o 0 for no errors
|    o def_memErr_seqST for memory errors
\-------------------------------------------------------*/
signed char
realloc_seqST(
   struct seqST **seqArySTPtr,/*sequence array to expand*/
   signed long lenSL,         /*index of last item*/
   signed long sizeSL         /*new length (size)*/
);

/*-------------------------------------------------------\
| Fun18: readFaFile_seqST
|   - get all sequences from a fasta file
| Input:
|   - faFILE:
|     o FILE pointer to file to get sequences from
|   - lenSLPtr:
|     o pointer to signed long to hold number sequences
|       in returned array (also in faFILE)
|   - sizeSLPtr:
|     o pointer to signed long to hold returned array size
|   - errSCPtr:
|     o pointer to signed char to hold errors
| Output:
|   - Modifies:
|     o faFILE to be at end of file
|     o lenSLPtr to have number sequences in faFILE/array
|     o sizeSLPtr to have size of returned array
|     o errSCPtr to have errors
|       * 0 for no errors
|       * def_fileErr_seqST if had file error
|       * def_badLine_seqST | def_fileErr_seqST for
|         invalid fasta entry
|       * def_memErr_seqST for memory errors
|   - Returns:
|     o seqST struct array with sequences
|     o 0 for errors
\-------------------------------------------------------*/
struct seqST *
readFaFile_seqST(
   void *faFILE,           /*file to get reads from*/
   signed long *lenSLPtr,  /*number items in array*/
   signed long *sizeSLPtr, /*length of array*/
   signed char *errSCPtr   /*holds error values*/
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
