/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' ampDepth SOF: Start Of File
'   - Had functions for updating and printing an read
'     depth histogram (array of integers)
'   o header:
'     - foward declarations and guards
'   o fun01: addRead_ampDepth
'     - adds a read to an amp depth arrary (histogram)
'   o fun02: phead_ampDepth
'     - prints the header for an ampDepth tsv file
'   o fun03: pDepthHead_ampDepth
'     - prints the header for the read depth output
'   o fun04: phist_ampDepth
'     - prints an histogram of read depths as an tsv with
'       individual genes for ampDepth
'   o fun05: pDepthHead_ampDepth
'     - prints the header for the read depth output
'   o fun06: pdepth_ampDepth
'     - prints the read depth of each base
'   o fun07: pGeneDepth_ampDepth
'     - prints the read depth of each base in the target
'       genes
'   o fun08: pGeneCoverage_ampDepth
'     - prints percent gene coverage and start/mid/end
'   o fun09: getGeneCoverage_ampDepth
'     - puts the gene coverage and depth into an array
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - foward declarations and guards
\-------------------------------------------------------*/

#ifndef AMPLICON_DEPTH_H
#define AMPLICON_DEPTH_H

struct samEntry;
struct geneCoord;

/*-------------------------------------------------------\
| Fun01: addRead_ampDepth
|   - adds a read to an amp depth arrary (histogram)
| Input:
|   - samSTPtr:
|     o pointer to an samEntry structure with a read to
|       add to the histogram
|   - coordsSTPtr:
|     o geneCoord struct pointer with gene/target
|       coordinates want to extract
|     o input 0/null to not filter by genes
|   - depthArySI:
|     o histogram (int array) to add each base to
|   - numOffTargSI:
|     o number of reads not mapping to a gene coordiante,
|       but are mapped to the genome
| Output:
|   - Modifies:
|     o ampDepthIAry to be incurmented for each reference
|       base in the read
\-------------------------------------------------------*/
void
addRead_ampDepth(
   struct samEntry *samSTPtr,
   struct geneCoord *coordsSTPtr, /*list of genes*/
   signed int *depthArySI,  /*depth array to update*/
   signed int *numOffTargSI /*number reads not in list*/
);

/*-------------------------------------------------------\
| Fun02: phead_ampDepth
|   - prints the header for an ampDepth tsv file
| Input:
|   - outFILE:
|     o file to print the header to
| Output:
|   - Prints:
|     o ampDepth header to outFILE
\-------------------------------------------------------*/
void
phead_ampDepth(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun04: phist_ampDepth
|   - prints an histogram of read depths as an tsv with
|     individual genes for ampDepth
| Input:
|   - histArySI:
|     o integer array with the histogram to print out
|   - minDepthSI:
|     o integer with the min depth to keep an histogram
|       entry
|   - geneCoordSTPtr:
|     o pointer to an geneCoord structure array with the
|       locations of each gene
|   - offTargSI:
|     o number off of target reads to print out
|   - noMapSI:
|     o number off of unmapped reads to print out
|   - extraColStr:
|     o c-string with the first column to print out
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o stats for each gene to outFILE
\-------------------------------------------------------*/
void
phist_ampDepth(
   signed int *histArySI,
   signed int minDepthSI,
   struct geneCoord *geneCoordSTPtr,
   signed int offTargSI,
   signed int noMapSI,
   signed char *extraColStr,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun05: pDepthHead_ampDepth
|   - prints the header for the read depth output
| Input:
|   - outFILE:
|     o file to print the header to
| Output:
|   - Prints:
|     o read depth header to outFILE
\-------------------------------------------------------*/
void
pDepthHead_ampDepth(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun06: pdepth_ampDepth
|   - prints the read depth of each base
| Input:
|   - depthArySI:
|     o integer array with the depthogram to print out
|   - depthLenSI:
|     o number of bases (length) of depthArySI (index 1)
|   - minDepthSI:
|     o integer with the min depth to keep an depthogram
|       entry
|   - refStr:
|     o c-string with name of reference sequence
|     o if 0/null then "reference"
|   - flagStr:
|     o c-string with flag to go in left column
|     o if 0/null then "out"
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o depth for each base to outFILE
\-------------------------------------------------------*/
void
pdepth_ampDepth(
   signed int *depthArySI,  /*has read depths*/
   signed int depthLenSI,   /*number bases in depthArySI*/
   signed int minDepthSI,   /*minimum read depth*/
   signed char *refStr,     /*name of reference*/
   signed char *flagStr,    /*flag to go in left column*/
   void *outFILE            /*output file*/
);

/*-------------------------------------------------------\
| Fun07: pGeneDepth_ampDepth
|   - prints the read depth of each base in the target
|     genes
| Input:
|   - depthArySI:
|     o integer array with the depthogram to print out
|     o the length needs to be the same as the last
|       gene coordinate in geneCoordSTPtr
|   - geneCoordSTPtr:
|     o geneCoord struct pointer with the coordinates
|       to print
|   - minDepthSI:
|     o integer with the min depth to keep an depthogram
|       entry
|   - refStr:
|     o c-string with name of reference sequence
|     o if 0/null then "reference"
|   - outFILE:
|     o file to print to
| Output:
|   - Prints:
|     o depth for each base to outFILE
\-------------------------------------------------------*/
void
pGeneDepth_ampDepth(
   signed int *depthArySI,  /*has read depths*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   signed int minDepthSI,   /*minimum read depth*/
   signed char *refStr,     /*name of reference*/
   void *outFILE            /*output file*/
);

/*-------------------------------------------------------\
| Fun08: pGeneCoverage_ampDepth
|   - prints percent gene coverage and start/mid/end
| Input
|   - depthArySI:
|     o signed int array with read depths
|   - minDepthSI:
|     o minimum read depth to count as covered
|   - geneCoordSTPtr:
|     o geneCoord struct with gene coordinates to print
|   - outFILE:
|     o FILE * pointer to print to
| Output:
|   - Prints:
|     o percent coverage and starts/ends to outFILE
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
pGeneCoverage_ampDepth(
   signed int *depthArySI, /*histogram of read depths*/
   signed int minDepthSI,  /*min depth to print*/
   struct geneCoord *geneCoordSTPtr, /*gene coordinates*/
   void *outFILE           /*file to print to*/
);

/*-------------------------------------------------------\
| Fun09: getGeneCoverage_ampDepth
|   - puts the gene coverage and depth into an array
| Input
|   - depthArySI:
|     o signed int array with read depths
|   - minDepthSI:
|     o minimum read depth to count as covered
|   - geneCoordSTPtr:
|     o geneCoord struct with gene coordinates to print
| Output:
|   - Returns:
|     o float array (size = geneCoordSTPtr->lenSI * 3)
|       with percent coverage, coverage mean read depth,
|       and gene mean read depth
|       * has two items per index
|       * index 0: gene percent coverage (as decimal)
|       * index 1: mean read depth in covered region
|       * index 2: gene mean read depth (as decimal)
|       * you can get the gene index with (index / 3)
|     o 0 if had a memory error
\-------------------------------------------------------*/
float *
getGeneCoverage_ampDepth(
   signed int *depthArySI, /*histogram of read depths*/
   signed int minDepthSI,  /*min depth to print*/
   struct geneCoord *geneCoordSTPtr /*gene coordinates*/
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
