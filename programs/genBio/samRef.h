/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' samRef SOF: Start Of File
'   - structure functions to get references lengths from
'     a sam file
'   - keeping this separate from samEntry avoids usage
'     of strAry in other programs. This can save 4kb in
'     compile size
'   o header:
'     - forward declarations and guards
'   o .h st01: refs_samRef
'     - holds reference length & name from sam file header
'   o fun01: blank_refs_samRef
'     - blanks a refs_samRef struct
'   o fun02: init_refs_samRef
'     - initializes a refs_samRef struct
'   o fun03: freeStack_refs_samRef
'     - frees variables in a refs_samRef struct
'   o fun04: freeHeap_refs_samRef
'     - frees a refs_samRef struct
'   o fun05: setup_refs_samRef
'     - allocates memory for a refs_samRef struct
'   o fun06: realloc_refs_samRef
'     - reallocates memory for a refs_samRef struct
'   o fun07: getRefLen_samRef
'     - gets reference ids & length from a sam file header
'   o fun08: findRef_refs_samRef
'     - finds a reference id in a refs_samRef struct
'   o fun09: addRef_samRef
'     - adds reference information to array in refStack
'   o fun10: buildRefMergeIndex_samRef
'     - looks for refseq ids that might be contigs from
'       the sam assembly and builds an index for each
'       unique id
'   o fun11: pSamHeader_samRef
'     - prints samEntry header for a reference or set of
'       references
'   o fun12: pSamToRef_samRef
'     - print samEntry to its reference bin
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef SAM_FILE_GET_REFERENCES_H
#define SAM_FILE_GET_REFERENCES_H

struct samEntry;

/*-------------------------------------------------------\
| ST01: refs_samRef
|   - holds reference length and name from sam file header
\-------------------------------------------------------*/
typedef struct refs_samRef
{
   unsigned int *lenAryUI; /*reference lengths*/
   signed char *idAryStr;  /*reference names (ids)*/
   unsigned int numRefUI;  /*number of references*/
   unsigned int arySizeUI; /*length of length/id arrays*/
}refs_samRef;


/*-------------------------------------------------------\
| Fun01: blank_refs_samRef
|   - blanks a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to blank
| Output:
|   - Modifies:
|     o refsSTPtr to be blanked
\-------------------------------------------------------*/
void
blank_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_refs_samRef
|   - initializes a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to initialize
| Output:
|   - Modifies:
|     o refsSTPtr to be initialize (everything 0)
\-------------------------------------------------------*/
void
init_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_refs_samRef
|   - frees variables in a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef with variables to free
| Output:
|   - Frees:
|     o lenAryUI and refIdStr arrays in refsSTPtr
\-------------------------------------------------------*/
void
freeStack_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_refs_samRef
|   - frees a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to free
| Output:
|   - Frees:
|     o refSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_refs_samRef
|   - allocates memory for a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to get memory for
|   - numRefsUI:
|     o number of refs to start out with (0 goes to 16)
| Output:
|   - Mofidifies:
|     o lenAryUI and idAryUI in refSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
\-------------------------------------------------------*/
signed char
setup_refs_samRef(
   struct refs_samRef *refSTPtr,
   unsigned int numRefsUI
);

/*-------------------------------------------------------\
| Fun06: realloc_refs_samRef
|   - reallocates memory for a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to reallocate
|   - numRefsUI:
|     o new number of refs
| Output:
|   - Mofidifies:
|     o lenAryUI and idAryUI in refSTPtr to be resized
|     o sizeAryUI to be numRefsUI
|     o calls blank_refs_samRef
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
\-------------------------------------------------------*/
signed char
realloc_refs_samRef(
   struct refs_samRef *refSTPtr,
   unsigned int numRefsUI
);

/*-------------------------------------------------------\
| Fun07: getRefLen_samRef
|   - gets reference ids and length from a sam file header
| Input:
|   - refSTPtr:
|     o pointer to refs_samRef struct to hold id/length
|     o is sorted by reference id (use functions in
|       ../genLib/strAry to access ids)
|   - samSTPtr:
|     o pointer to samEntry struct to hold line after
|       last reference (used for reading sam file)
|   - samFILE:
|     o FILE pointer to sam file header
|   - outFILE:
|     o FILE pointer to print all headers to (0 no print)
|   - headStrPtr:
|     o pointer to c-string to hold non-reference headers
|   - lenHeadULPtr:
|     o unsigned long with headStrPtr length
| Output:
|   - Modifies:
|     o refSTPtr to have reference length and ids
|     o samSTPtr to have first read
|     o samFILE to point to first read after header
|     o outFILE to have header (or not use if 0)
|     o headStrPtr to have non-reference headers
|     o lenHeadULPtr have new headStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
|     o def_fileErr_samEntry for file errors
\-------------------------------------------------------*/
signed char
getRefLen_samRef(
   struct refs_samRef *refSTPtr,/*holds ref lengths*/
   struct samEntry *samSTPtr,    /*for reading sam*/
   void *samFILE,                /*sam file with lengths*/
   void *outFILE,                /*print headers to*/
   signed char **headStrPtr,     /*holds non-ref header*/
   unsigned long *lenHeadULPtr   /*length of headStrPtr*/
);

/*-------------------------------------------------------\
| Fun08: findRef_refs_samRef
|   - finds a reference id in a refs_samRef struct
| Input:
|   - idStr:
|     o c-string with reference id to find
|   - refSTPtr:
|     o pointer to refs_samRef struct with references
| Output:
|   - Returns:
|     o index of reference id if found
|     o < 0 if reference id not in list
\-------------------------------------------------------*/
signed long
findRef_refs_samRef(
   signed char *idStr,            /*id to find*/
   struct refs_samRef *refSTPtr /*holds ref lengths*/
);

/*-------------------------------------------------------\
| Fun09: addRef_samRef
|   - adds reference information to array in refStack
| Input:
|   - idStr:
|     o c-string with id to add
|   - lenUI:
|     o length of reference sequence
|   - refsPtr:
|     o pointer to refs_samRef struct to add ref to
|   - errSCPtr:
|     o pointer to signed char to hold errors
| Output:
|   - Modifies:
|     o idAryStr in refsPtr to have idStr
|     o lenAryUI in refsPtr to have lenUI
|     o numRefUI in refsPtr to be resized if realloc used
|     o arrySizeUI in refsPtr to be incurmented by 1
|     o errSCPtr to be
|       * 0 for no error
|       * def_expand_samEntry if needed to realloc
|       * def_memErr_samEntry for memory error
|   - Returns
|     o index of reference
|     o -1 for errors
\-------------------------------------------------------*/
signed long
addRef_samRef(
   signed char *idStr,
   unsigned int lenUI,
   struct refs_samRef *refsPtr,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun10: buildRefMergeIndex_samRef
|   - looks for refseq ids that might be contigs from the
|     sam assembly and builds an index for each unique id
| Input:
|   - refsSTPtr:
|     o refs_samRef struct with reference ids to scan for
|       refseq ids from the same assembly
|     o needs to be sorted by name (default state)
| Output:
|   - Returns:
|     o signed int array to with the index assigned to
|       each reference or -1 if no other references group
|       with this reference
|     o 0 for memory errors
\-------------------------------------------------------*/
signed int *
buildRefMergeIndex_samRef(
   struct refs_samRef *refsSTPtr
);

/*-------------------------------------------------------\
| Fun11: pSamHeader_samRef
|   - prints samEntry header for a reference or set of
|     references
| Input:
|   - refIndexSI:
|     o reference printing header to
|   - headStr:
|     o c-string with general header to print out
|     o these are headers that do not relate to the
|       reference
|     o this is made by getRefLen_samRef
|   - unmapBl:
|     o 1: print the unmapped reads header (no sequence
|          (reference) entry)
|     o 0: print header for a mapped read
|   - refSTPtr:
|     o samRef struct pointer with reference ids
|     o 0/null, use refIdStr in samSTPtr
|   - refIndexArySI:
|     o index array with reference index to print to
|     o no input (0/null), use refIdStr in samSTPtr
|     o index values:
|       * -1: no merging
|       * >=0: print reference to reference id at this
|         index
|     o must be in same order as index's in refSTPtr and
|       mereged references must be next to each other
|   - outFILE:
|     o FILE pionter to file to print to
| Output:
|   - Prints:
|     o header and reference lengths to outFILE
\-------------------------------------------------------*/
void
pSamHeader_samRef(
   signed int refIndexSI,     /*check if file was opened*/
   signed char *headStr,      /*header to print out*/
   signed char unmapBl,       /*1: unmapped header*/
   struct refs_samRef *refSTPtr, /*references used*/
   signed int *refIndexArySI, /*index to print read to*/
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: pSamToRef_samRef
|   - print samEntry to its reference bin
| Input:
|   - samSTPtr:
|     o samEntry struct pointer with read to print
|   - prefixStr:
|     o c-string with prefix to print to
|   - headStr:
|     o c-string with header to print to the new file
|   - fileSC:
|     o 0 printing to a sam file
|     o 1 printing to a fastq file (header is not used)
|     o 2 printing to a fasta file (header is not used)
|   - pUnmapBl:
|     o 1: print unmapped reads
|     o 0: do not print unmapped reads
|   - refSTPtr:
|     o samRef struct pointer with reference ids
|     o 0/null, use refIdStr in samSTPtr
|   - refIndexArySI:
|     o index array with reference index to print to
|     o no input (0/null), use refIdStr in samSTPtr
|     o index values:
|       * -1: no merging
|       * >=0: print reference to reference id at this
|         index
| Output:
|   - Prints:
|     o read in samSTPtr to file named
|       "prefix-referenceId.sam"
|   - Returns:
|     o 0 for success
|     o 1 if no reference id
|     o -1 for file errors
\-------------------------------------------------------*/
signed char
pSamToRef_samRef(
   struct samEntry *samSTPtr,  /*read to print*/
   signed char *prefixStr,     /*file prefix to print to*/
   signed char *headStr,       /*header for new file*/
   signed char fileSC,         /*output file format*/
   signed char pUnmapBl,       /*1: print unmapped reads*/
   struct refs_samRef *refSTPtr,/*has reference ids*/
   signed int *refIndexArySI   /*index to print read to*/
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
