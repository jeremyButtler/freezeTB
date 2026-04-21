/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' geneCoord SOF: Start Of File
'   - holds the geneCoord struct and its supporting
'     functions I use to read in gene positions form a paf
'     file
'   o header:
'     - Header guards
'   o .h st: geneCoord
'     - Holds the arrays for the gene coordinates and ids 
'       in an gene coordiantes table
'   o fun02: freeStack_geneCoord
'     - Frees the arrays in a geneCoord structure
'   o fun03: freeHeap_geneCoord
'     - Frees the a heap alloacted geneCoord structure
'   o fun01: init_geneCoord
'     - Initializes a geneCoord structure
'   o fun04: mk_geneCoord
'     - Makes a heap allocated geneCoord structure
'   o .c fun06: swap_geneCoord
'     - Swaps two array items in a geneCoord structure
'       around
'   o fun07: sort_geneCoord
'     - Sorts the arrays in a genesCoord structure by
'       starting positiion with shell short.
'   o fun08: findStart_geneCoord
'     - Does a binary search by starting coordinate for a
'       potentail gene in a geneCoord structure
'   o fun09: findRange_geneCoord
'     - finds start of range read coordinate may be in
'   o fun10: sortName_geneCoord
'     - Sorts the arrays in a genesCoord structure by
'       gene name
'   o fun11: nameSortFloat3IndexSync_geneCoord
'    - sorts the arrays in a genesCoord structure by
'      gene name and keep an array of floats (index 3) in
'      sync
'   o fun12: findName_geneCoord
'     - Does a binary search to find an gene name in an
'       gene geneCoord structer (must be sorted by name)
'   o fun13: getCoords_geneCoord
'     - Gets the gene coordinates from an gene coordinates
'       table
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Header guards
\-------------------------------------------------------*/

#ifndef GENE_COORDINATE_STRUCTER_H
#define GENE_COORDINATE_STRUCTER_H

#define def_fileErr_geneCoord 2
#define def_invalidEntry_geneCoord 4
#define def_memErr_geneCoord 64

#define def_lenId_geneCoord 64

/*-------------------------------------------------------\
| ST01: geneCoord
|   - Holds the arrays for the gene coordinates and ids 
|     in an gene coordinates table
\-------------------------------------------------------*/
typedef struct geneCoord{
   signed char (*idStrAry)[def_lenId_geneCoord];
      /*id of gene/section to change*/

   signed char (*refAryStr)[def_lenId_geneCoord];
      /*id of reference to convert coordinates to*/

   unsigned int *startAryUI; /*new starting position*/
   unsigned int *endAryUI;   /*new ending position*/
   unsigned char *dirAryUC;  /*direction*/

   signed int lenSI; /*number of genes in gene coord*/
}geneCoord;

/*#define getIdIndex_geneCoord(indexUI) ((indexUI) * def_lenId_geneCoord)*/

/*-------------------------------------------------------\
| Fun01: init_geneCoord
|    - Initializes a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to initialize
| Output:
|   - Sets:
|     o idStrAry, startAryUI, and endAryUI to 0
\-------------------------------------------------------*/
void
init_geneCoord(
   struct geneCoord *geneCoordST
);

/*-------------------------------------------------------\
| Fun02: freeStack_geneCoord
|    - Frees the arrays in a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure with arrays to
|       free
| Output:
|   - Frees:
|     o idStrAry, startAryUI, and endAryUI (and sets to 0)
\-------------------------------------------------------*/
void
freeStack_geneCoord(
   struct geneCoord *geneCoordST
);

/*-------------------------------------------------------\
| Fun03: freeHeap_geneCoord
|    - Frees the a heap alloacted geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to free
| Output:
|   - Frees:
|     o geneCoordST
\-------------------------------------------------------*/
void
freeHeap_geneCoord(
   struct geneCoord *geneCoordST
);

/*-------------------------------------------------------\
| Fun04: mk_geneCoord
|    - Makes a heap allocated geneCoord structure
| Input:
|   - numGenesUI:
|     o Length of the arrays in the new geneCoord
|       structure
| Output:
|   - Returns:
|     o geneCoord structure with allocated arrays
| Note:
|   - Do not use init_geneCoord on the returned structure
\-------------------------------------------------------*/
struct geneCoord *
mk_geneCoord(
   unsigned int numGenesUI
);

/*-------------------------------------------------------\
| Fun07: sort_geneCoord
|  - Sorts the arrays in a genesCoord structure by
|    starting positiion with shell short.
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sort_geneCoord(
   struct geneCoord *geneCoordST
);

/*-------------------------------------------------------\
| Fun08: findStart_geneCoord
|  - Does a binary search by starting coordinate for a
|    potentail gene in a geneCoord structure
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - qryUI:
|    o Starting coordinate (query) to search for in
|      geneCoordST
|  - refStr:
|    o c-string with reference to find the gene for
|    o 0/null for ignore reference
| Output:
|  - Returns:
|    o The index of the starting position
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findStart_geneCoord(
   struct geneCoord *geneST,
   unsigned int qryUI,
   signed char *refStr
);

/*-------------------------------------------------------\
| Fun09: findRange_geneCoord
|  - finds start of range read coordinate may be in
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - startUI:
|    o starting coordinate (query) to search for
|  - endUI:
|    o ending coordinate (query) to search for
|  - refStr:
|    o c-string with reference to find the gene for
|    o 0/null for ignore reference
| Output:
|  - Returns:
|    o index of gene that overlaps with startUI and endUI
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findRange_geneCoord(
   struct geneCoord *geneST,
   unsigned int startUI,
   unsigned int endUI,
   signed char *refStr
);

/*-------------------------------------------------------\
| Fun10: sortName_geneCoord
|  - sorts the arrays in a genesCoord structure by
|    gene name and then reference
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - startUI:
|    o First element to start sorting at
|  - endUI:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sortName_geneCoord(
   struct geneCoord *geneCoordST
);

/*-------------------------------------------------------\
| Fun11: nameSortFloat3IndexSync_geneCoord
|  - sorts the arrays in a genesCoord structure by
|    gene name and keep an array of floats (index 3) in
|    sync
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - numGenesSI:
|    o number of genes
|  - floatAry:
|    o float array to keep in sync
|    o float array has 3 entries per gene, so is moved in
|      groups of threee
| Output:
|  - Modifies:
|    o arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
|    o floatAry to be in sync with geneCoordST
\-------------------------------------------------------*/
void
nameSortFloat3IndexSync_geneCoord(
   struct geneCoord *geneCoordST,
   float *floatAryF
);

/*-------------------------------------------------------\
| Fun12: findName_geneCoord
|  - Does a binary search to find an gene name in an gene
|    geneCoord structer (must be sorted by name)
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure to sort
|      coordinates to search
|  - nameStr:
|    o c-string with name to search for
|  - refStr:
|    o c-string with reference sequence
|    o use 0 for search for gene only
| Output:
|  - Returns:
|    o The index of gene with the same name
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findName_geneCoord(
   struct geneCoord *geneST,
   signed char *nameStr,
   signed char *refStr
);

/*-------------------------------------------------------\
| Fun13: getCoords_geneCoord
|  - Gets the gene coordinates from a gene table (tsv)
| Input:
|  - geneTblFileStr:
|    o C-string with name of the gene table file to
|      extract the gene coordinates and names from
|  - errULPtr:
|    o Will hold the error return value
| Output:
|  - Returns:
|    o Pointer to an sorted geneCoord structure with the 
|      gene coordinates
|    o 0 for errors
|  - Modifies:
|    o errULPtr to hold the error
|      - 0 for no errors
|      - def_fileErr_geneCoord for an file opening error
|      - def_memErr_geneCoord for an memor error
|      - (line_number << 8) | def_invalidEntry_geneCoord
|        for an invalid line in the file
|        o Get the line number with (*errULPtr >> 8)
\-------------------------------------------------------*/
struct geneCoord *
getCoords_geneCoord(
   signed char *geneTblFileStr,
   unsigned long *errULPtr
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
