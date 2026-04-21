/*#######################################################\
# Name: geneIndiceStruct
#   - structer and functions for reading the genome
#     indice (tab 2) of the WHO's 2023 TB catalog
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header: 
'     - guards
'   o .h st01: genIndice
'     - Structure to hold a single genome indice from
'       the who 2023 catalog
'   o fun01: blankGeneIndice
'     - blanks a genIndice struct
'   o fun02: init_genIndice
'     - initialize a genIndice
'   o fun03: mk_genIndice
'     - makes a new genIndice struct (on heap)
'   o fun04: freestack_genindice
'     - frees the variables in an geneIndice structure
'   o fun05: freeHeap_geneIndice
'     - frees a single geneIndiceST structure.
'   o fun06: freeHeapAry_genIndice
'     - frees an array of geneIndiceST structures
'   o fun07: swap_genIndice
'     - swaps two genIndice structures in an array
'   o fun08: sort_genIndice
'     - sorts array of genIndice structures by variant ids
'   o fun09: findVariant_genIndice
'     - finds nearest amr to input query coordiante
'   o fun10: read2023Db_genIndice
'     - reads in the genome coordinates and variants from
'       the 2023 WHO TB catalog genome indice tab (saved
'       as a tsv)
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header: 
|   - guards
\-------------------------------------------------------*/

#ifndef GENOME_INDICIE_STRUCT_H
#define GENOME_INDICIE_STRUCT_H

#define def_memErr_genIndice 1
#define def_fileErr_genIndice 2

/*-------------------------------------------------------\
| ST01: genIndice
|   - Structure to hold a single genome indice from
|     the who 2023 catalog
\-------------------------------------------------------*/
typedef struct genIndice
{ /*genIndice*/
   unsigned int posUI;

   signed char *varIdStr;
   unsigned int lenVarIdUI;

   signed char *refSeqStr;
   unsigned int lenRefSeqUI;

   signed char *amrSeqStr;
   unsigned int lenAmrSeqUI;
}genIndice;

/*-------------------------------------------------------\
| Fun01: blankGeneIndice
|   - blanks a genIndice struct
| Input:
|   - geneIndiceSTPtr:
|     o pointer to genIndice to blank
| Output:
|   - Modifies:
|     o posUI in geneIndiceSTPtr to be 0
\-------------------------------------------------------*/
void
blank_genIndice(
   struct genIndice *genIndiceSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_genIndice
|   - initialize a genIndice
| Input:
|   - geneIndiceStPtr:
|     o pointer to genIndice to initialize
| Output:
|   - Modifies:
|     o geneIndice to have all values set to 0
\-------------------------------------------------------*/
void
init_genIndice(
   struct genIndice *genIndiceSTPtr
);

/*-------------------------------------------------------\
| Fun03: mk_genIndice
|   - makes a new genIndice struct (on heap)
| Input:
| Output:
|   - Returns:
|     o genIndice on heap
\-------------------------------------------------------*/
struct genIndice *
mk_genIndice(
   void
);

/*-------------------------------------------------------\
| fun04: freestack_genindice
|   - frees the variables in an geneIndice structure
| Input:
|   - geneIndiceST:
|     o pointer to genIndice sturct with varialbes to free
| Output:
|   - Frees:
|     o strings in the geneIndiceST structure
|   - Sets:
|     o everything to 0
\-------------------------------------------------------*/
void
freeStack_genIndice(
   struct genIndice *geneIndiceST
);

/*-------------------------------------------------------\
| Fun05: freeHeap_geneIndice
|   - frees a single geneIndiceST structure.
| Input:
|   - geneIndiceST:
|     o pointer to geneIndiceST structure to free
| Output:
|   - Frees:
|     o geneIndiceST
\-------------------------------------------------------*/
void
freeHeap_geneIndice(
   struct genIndice *geneIndiceST
);

/*-------------------------------------------------------\
| Fun06: freeHeapAry_genIndice
|   - frees an array of geneIndiceST structures
| Input:
|   - genIndiceST:
|     o pointer to genIndiceST structure arrray to free
|  - numElmUI:
|     o number of elements in genIndiceST
| Output:
|   - Frees:
|     o genIndiceST
|   - Sets:
|     o genIndiceST to 0
\-------------------------------------------------------*/
void
freeHeapAry_genIndice(
   struct genIndice *genIndiceST,
   unsigned int numElmUI
);

/*-------------------------------------------------------\
| Fun07: swap_genIndice
|  - swaps two genIndice structures in an array
| Input:
|  - genIndiceAry:
|    o pointer to geneIndice struct array
|  - posOne:
|    o first position (index) to swap
|  - posTwo:
|    o second position (index) to swap
| Output:
|  - Modifies:
|    o swaps first and second positions in genIndicAry
\-------------------------------------------------------*/
void
swap_genIndice(
   struct genIndice *genAry,
   unsigned int posOne,
   unsigned int posTwo
);

/*-------------------------------------------------------\
| Fun08: sort_genIndice
|  - sorts an array of genIndice structures by variant ids
| Input:
|  - genIndiceST:
|    o pointer to genIndice structure array to sort
|  - startUI:
|    o first element sort
|  - endUI:
|    o last element to sort (index 0)
| Output:
|  - Modifies:
|    o arrays in genIndiceST to be sorted by varint id
\-------------------------------------------------------*/
void sort_genIndice(
   struct genIndice *genIndiceST,
   unsigned int startUI,
   unsigned int endUI
);


/*-------------------------------------------------------\
| Fun09: findVariant_genIndice
|  - finds nearest amr to input query coordiante
| Input:
|  - genIndiceAry:
|    o genIndice struct array to find variant in
|  - qryStr:
|    o c-string with variant to search for
|  - lenArySI:
|    o lenth of the genIndice structure array (index 1)
| Output:
|  - Returns:
|    o index of the variant in the array
|    o -1 if the variant is not in the array
\-------------------------------------------------------*/
int findVariant_genIndice(
   struct genIndice *genIndiceAry,
   signed char *qryStr,
   signed int lenArySI
);

/*-------------------------------------------------------\
| Fun10: read2023Db_genIndice
|   - reads in the genome coordinates and variants from
|     the 2023 WHO TB catalog genome indice tab (saved
|     as a tsv)
| Input:
|   - indiceStr:
|     o c-string with the path to the tsv with the
|       genome indice tab (tab 2) of the who 2023 catalog
|   - numIndicesUI:
|     o pointer to unsigned int to hold number of AMRs
|   - errSC:
|     o pointer to signed char to hold error
| Output:
|   - Modifies:
|     o numIndicesUI to have number of extracted genome
|       indices
|     o errSC to have error:
|       - 0 for no errors
|       - def_memErr_genIndice for memory errors
|       - def_fileErr_genIndice for file errors
|   - Returns:
|     o 0 for memory error or file errors
|     o pointer to an array of genIndice structures
\-------------------------------------------------------*/
struct genIndice *
read2023Db_genIndice(
   signed char *indiceStr,
   unsigned int *numIndicesUI,
   signed char *errSC
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
