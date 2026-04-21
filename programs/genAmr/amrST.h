/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' amrST SOF: Start Of File
'   - holds functions to support the amrST struct used in
'     tbAmr.
'   o header:
'     - defined variables and guards
'   o .h st01: amrST
'     - holds a single amr mutation + supporting info
'   o .h st02: refList_amrST
'     - has amrST arrays organized by reference
'   * amrST structure general functions
'      o fun01: blank_amrST
'        - sets all non-pointer values in amrSTPtr to 0
'      o fun02: init_AmrST
'        - sets all values/pointers in amrSTPtr to 0
'      o fun03: freeStack_amrST
'        - frees internal variables in an amrST structure
'      o fun04: freeHeap_amrST
'        - frees heap allocated amrST structure
'      o fun05: freeHeapAry_amrST
'        - frees an heap allocated array of amrST structs
'      o .c fun06: swap_amrST
'        - swaps values between two amrST structures
'      o .c fun07: cp_amrST
'        - copies (deep copy) values from one amrST struct
'          to another
'      o fun08: sortPos_amrST
'        - Sort on an amrST array structures by reference
'          coordiante (uses shell sort)
'      o fun09: sortGeneId_amrST
'        - sort amrST struct array by gene names (ids)
'      o fun10: sortVarId_amrST
'        - sort amrST struct array by variant id
'      o fun11: getAmr_amrST
'        - finds nearest amr to input query coordiante
'   * refList_amrST structure general functions
'      o fun12: blank_refList_amrST
'        - blanks a refList_amrST structure
'      o fun13: init_refList_amrST
'        - initializes a refList_amrST structure
'      o fun14: freeStack_refList_amrST
'        - frees variables in a refList_amrST structure
'      o fun15: freeHeap_refList_amrST
'        - frees refList_amrST struct, you must set to 0
'      o .c fun16: swap_refList_amrST
'        - swap to index's ina refList_amrST structure
'      o fun17: refFind_refList_amrST
'        - finds the amrAry assigned to a reference
'      o fun18: getAmrList_refList_amrST
'        - get the amr list for a single reference
'      o fun19: refAdd_refList_amrST
'        - adds a reference to its correct possition in a
'          refList_amrST structure
'      o fun20: amrAdd_refList_amrST
'        - adds a amrST struct to the end of its amr list
'        - this is an unsorted list
'      o fun21: sortPos_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by position
'      o fun22: sortGeneId_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by gene id
'      o fun23: sortVarId_refList_amrST
'        - sort the amrST arrays in a refList_amrST struct
'          by variant id
'   * input/output functions
'      o fun24: p_amrST
'        - Print out the amr database used
'      o fun25: readTbl_amrST
'        - gets amr data from a getAmr database
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef AMR_STRUCT_H
#define AMR_STRUCT_H

/*This is the number of longs to hold the resistance
`   flags. Each long holds 64 drugs on a 64 bit system
`   or 32 drugs on an 32 bit system
*/
#define def_maxDrugs_amrST 5
#define def_firstDrugCol_amrST 19
   /*first antibiotic column is 19 columns in*/

/*Reading frame direction*/
#define def_forwardDir_amrST 0
#define def_revCompDir_amrST 1
#define def_unkownDir_amrST 2

/*Whole gene event flags*/
#define def_geneFrameshift_amrST 1
#define def_geneDel_amrST 2

/*This is the flags for each resistance type when printed
`   out
*/
#define def_resFlag_amrST 1
#define def_crossResFlag_amrST 2
#define def_crossToResShift_amrST 1
   /*for converting cross restence to one bit*/
#define def_maxFlags_amrST (sizeof(unsigned long) << 3)

/*Error values*/
#define def_memErr_amrST 64
#define def_fileErr_amrST 2
#define def_missingIndice_amrST 4

/*-------------------------------------------------------\
| ST01: amrST
|   - holds a single amr mutation + supporting info
\-------------------------------------------------------*/
typedef struct amrST{
   unsigned int refPosUI;  /*start position on reference*/
   unsigned int codonPosUI;/*codon start position on ref*/
   unsigned int codonNumUI;/*codon number*/
   unsigned int endCodonNumUI; /*end of an indel*/

   /*this might allow my custom database to be a bit more
   ` usefull to others. It will at least help with mapping
   ` AMR's in my graphAmpDepth.r script
   */
   unsigned int geneFirstRefUI; /*1st ref base in gene*/
   unsigned int geneLastRefUI;  /*last ref base in gene*/

   /*Amino acid sequence stuff*/
   signed char *refAaStr;   /*reference amino acid seq*/
   unsigned int lenRefAaUI; /*length of reference aa seq*/

   signed char *amrAaStr;   /*mutant amino acid seq*/
   unsigned int lenAmrAaUI; /*length of mutant aa seq*/

   signed char frameshiftBl; /*1: is a frame shift*/
   signed char wholeGeneFlagSC;/*1: whole gene event*/
   signed char unknownBl;    /*1: have no idea what is*/
   signed char aaMultiDupBl; /*1: range duplicated*/

   /*general info/ mutation*/
   signed char *geneIdStr;          /*gene name*/
   unsigned int lenGeneIdUI;/*length of gene id*/

   signed char *refSeqStr; /*reference sequence for AMR*/
   unsigned int lenRefSeqUI;/*length of reference seq*/

   signed char *amrSeqStr; /*AMR sequence*/
   unsigned int lenAmrSeqUI;/*length of amr sequence*/

   signed char *varIdStr;  /*variant nomincalture/id*/
   unsigned int lenVarIdUI;

   signed char *effectStr;/*WHO comment; mutation effect*/
   unsigned int lenEffectUI;/*length of effect*/

   signed char *commentStr;/*WHO comment (has epistatic)*/
   unsigned int lenCommentUI; /*length of comment*/

   signed char gradeSC;    /*grade of resitance*/
   signed char mutTypeStr[4]; /*mutation type*/
   signed char dirFlagSC;   /*1: gene reverse, else 0*/
      /*this is marked in the gene_locus column as a c
      ' at the end
      */
   
   /*flags for antibiotic resistance*/
   unsigned long amrFlagsUL[def_maxDrugs_amrST];
   unsigned long crossResFlagsUL[def_maxDrugs_amrST];
      /*5 items should allow for ~ 150 antibiotics on a
      `  32 bit system. ~ 300 on a 64 bit
      */

   unsigned int numSupReadsUI; /*number of read support*/
   unsigned int numMapReadsUI; /*number of reads mapping*/

   /*flags/variables for special who comments*/
   signed char highResBl; /*1: high resitance primary*/
   signed char lowResBl;  /*1: low resitance primary*/
   signed char additiveResBl;/*1:low resitance addivitve*/
   signed char *needsGeneStr; /*gene needed for resitance*/
   unsigned int lenNeedsGeneUI; /*length of needsGeneStr*/
}amrST;

/*-------------------------------------------------------\
| ST02: refList_amrST
|   - has amrST arrays organized by reference
\-------------------------------------------------------*/
typedef struct refList_amrST{
   struct amrST **amrsAryST;/*arrays of AMRs by refrence*/
   signed int *amrLenArySI; /*amr array lengths*/
   signed int *amrSizeArySI; /*amr array sizes*/
   signed char **refAryStr; /*references found*/
   signed int refLenSI;     /*number of references*/
   signed int refSizeSI;    /*number of references*/

   signed char noRefBl;     /*1: means no reference in
                            `    the database
                            */
   signed char *drugAryStr; /*has drugs in database*/
   signed int drugCntSI;    /*number drugs in drugAryStr*/
   signed int drugSizeSI;   /*max drugs array can hold*/
}refList_amrST;

/*-------------------------------------------------------\
| Fun01: blank_amrST
|   - sets all non-pointer values in amrSTPtr to 0
| Input:
|   - ampStructPtr:
|     - pointer to amrST to blank
| Output:
|   - Modifies
|     o all non-pointer values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
blank_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_amrST
|   - sets all values/pointers in amrSTPtr to 0
| Input:
|   - ampSTPtr:
|     - pointer to amrST to initialize
| Output:
|   - Modifies
|     o all values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
init_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_amrST
|   - Frees the geneIdStr, refSeqStr, and amrSeqStr arrays
|     in amrSTPtr and sets all values to 0
| Input:
|   - ampStructPtr:
|     - Pointer to amrST to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrSTPtr to be 0
\-------------------------------------------------------*/
void freeStack_amrST(struct amrST *amrSTPtr);

/*-------------------------------------------------------\
| Fun03: freeStack_amrST
|   - frees internal variables in an amrST structure
| Input:
|   - amrSTPtr:
|     - Pointer to amrST to free memory from
| Output:
|   - Frees:
|     o refSeqStr, amrSeqStr, and geneIdStr (one buffer)
|   - Modifies:
|     o All values in amrSTPtr to be 0
\-------------------------------------------------------*/
void
freeStack_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_amrST
|   - frees heap allocated amrST structure
| Input:
|   - amrSTPtr:
|     - pointer to amrST to free
| Output:
|   - Frees:
|     o amrSTPtr and the variables inside amrSTPtr
\-------------------------------------------------------*/
void
freeHeap_amrST(
   struct amrST *amrSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeapAry_amrST
|   - frees an heap allocated array of amrST structs
| Input:
|   - amrSTAry:
|     - pointer to start of amrST array to free
| Output:
|   - Frees:
|     o amrSTPtr array (includes internal variables)
\-------------------------------------------------------*/
void
freeHeapAry_amrST(
   struct amrST *amrSTAry, /*array to free*/
   unsigned long numElmUL  /*number of elements*/
);

/*-------------------------------------------------------\
| Fun08: sortPos_amrST
|   - sort an array of amrST structs by reference position
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by start reference position
\-------------------------------------------------------*/
void
sortPos_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI      /*last element (index 0)*/
);

/*-------------------------------------------------------\
| Fun09: sortGeneId_amrST
|   - sort amrST struct array by gene names (ids)
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by starting gene names
\-------------------------------------------------------*/
void
sortGeneId_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI
);

/*-------------------------------------------------------\
| Fun10: sortVarId_amrST
|   - sort amrST struct array by variant id
| Input:
|   - amrAryST:
|     o pointer to start of amrST array to sort
|   - startUI:
|     o index of first elment to sort
|   - endUI:
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o amrAryST to be sorted by variant ids
\-------------------------------------------------------*/
void
sortVarId_amrST(
   struct amrST *amrAryST,
   unsigned int startUI,
   unsigned int endUI
);

/*-------------------------------------------------------\
| Fun11: getAmr_amrST
|  - finds nearest amr to input query coordiante
| Input:
|  - amrAryST:
|    o pointer to an amrST array to search
|  - qryUI:
|    o coordinate (starting) to search for
|  - numGenesUI:
|    o number of amrST's in amrAryST (index 0)
| Output:
|  - Returns:
|    o nearest index at or after qryUI
|    o -1 for no amr genes after qryUI
\-------------------------------------------------------*/
signed int
getAmr_amrST(
   struct amrST *amrAryST,
   unsigned int qryUI,
   signed int numAmrSI
);

/*-------------------------------------------------------\
| Fun12: blank_refList_amrST
|   - blanks a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to blank
| Output:
|   - Modifies:
|     o all values in refList_amrST to be defaults (blank)
|       - this does a lazy blank (length to 0)
\-------------------------------------------------------*/
void
blank_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun13: init_refList_amrST
|   - initializes a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to initialize
| Output:
|   - Modifies:
|     o all values to be 0 (includes pointers)
\-------------------------------------------------------*/
void
init_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun14: freeStack_refList_amrST
|   - frees variables in a refList_amrST structure
| Input:
|   - refListSTPtr:
|     o refList_amrST structure with variables to free
| Output:
|   - Modifies:
|     o frees amrsAryST and refAryStr and initializes
\-------------------------------------------------------*/
void
freeStack_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun15: freeHeap_refList_amrST
|   - frees a refList_amrST structure, you must set to 0
| Input:
|   - refListSTPtr:
|     o refList_amrST structure to free
| Output:
|   - Modifies:
|     o frees refListSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_refList_amrST(
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun17: refFind_refList_amrST
|   - finds the index of amrAry assigned to a reference
| Input:
|   - refStr:
|     o c-string with reference to find
|   - refListSTPtr:
|     o refList_amrST structure to with refernces
| Output:
|   - Returns:
|     o index of the amrST struct with the matching
|       reference
|     o < 0 if no reference matched
|       * returned value is index to insert at times -1
|         + either elements after or greater or at end
\-------------------------------------------------------*/
signed int
refFind_refList_amrST(
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun18: getAmrList_refList_amrST
|   - get the amr list for a single reference
| Input:
|   - refStr:
|     o c-string with reference the amr list is assigned
|       to
|   - amrLenSIPtr:
|     o signed int pointer with the AMRs length
|   - refListSTPtr:
|     o refList_amrST structure with the amr lists
| Output:
|   - Modifies:
|     o amrLenSIPtr to be
|       * < 0 if reference was not found
|       * number of AMRs (>= 0) in the amr array
|   - Returns:
|     o amrST struct array pointer with references AMRs
|     o 0 if reference could not be found or if no AMRs
|       in the list
\-------------------------------------------------------*/
struct amrST *
getAmrList_refList_amrST(
   signed char *refStr,
   signed int *amrLenSIPtr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun19: refAdd_refList_amrST
|   - adds a reference to its correct possition in a
|     refList_amrST structure
| Input:
|   - refStr:
|     o c-string with the reference the amr belongs to
|     o use 0/null for if not using a reference
|   - refListSTPtr:
|     o refList_amrST structure to sort
| Output:
|   - Modifies:
|     o if refStr is not in refAryStr; then inserts
|       refStr into its sorted position in refAryStr
|   - Returns:
|     o index (>=0) of reference
|     o -1 if reference is blank
|     o -2 for memory errors
\-------------------------------------------------------*/
signed int
refAdd_refList_amrST(
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun20: amrAdd_refList_amrST
|   - adds a amrST struct to the end of its amr list
|   - this is an unsorted list
| Input:
|   - amrSTPtr:
|     o amrST struct pointer with settings to transfer
|     o this is a deep copy, so you can resuse the struct
|   - refStr:
|     o c-string with the reference the amr belongs to
|     o use 0/null for if not using a reference
|   - refListSTPtr:
|     o refList_amrST structure to sort
| Output:
|   - Modifies:
|     o amrsAryST in refListSTPtr to have the new AMR
|       * if needed will resize and set amrSizeArySI to
|         the new length
|     o amrLenArySI in refListSTPtr for the AMR list to
|       have one more AMR
|     o refAryStr to have reference (if it is a new ref)
|       * refLenSI and refSizeSI may also be resized
|   - Returns:
|     o 0 for no errors
|     o 1 if amr or reference is blank
|     o 2 for memory errors
\-------------------------------------------------------*/
signed char
amrAdd_refList_amrST(
   struct amrST *amrSTPtr,
   signed char *refStr,
   struct refList_amrST *refListSTPtr
);

/*-------------------------------------------------------\
| Fun21: sortPos_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortPos_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun22: sortGeneId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortGeneId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun23: sortVarId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortVarId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun21: sortPos_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     position
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortPos_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun22: sortGeneId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     gene id
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortGeneId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun23: sortVarId_refList_amrST
|   - sort the amrST arrays in a refList_amrST struct by
|     variant id
| Input:
|   - refSTPtr:
|     o refList_amrST struct pointer to sort
|     o index (0) of last elment to sort
| Output:
|   - Modifies:
|     o each amrST struct array in refSTPtr to be sorted
|       by position
\-------------------------------------------------------*/
void
sortVarId_refList_amrST(
   refList_amrST *refSTPtr
);

/*-------------------------------------------------------\
| Fun24: p_amrST
|  - print out amr database used
| Input:
|  - refSTPtr:
|    o pointer to refList_amrST struct with a AMR database
|      to print out
|  - outStr:
|    o c-string with path to output to.
|    o a "-" or null (0) is stdout
| Output:
|  - Prints:
|    o amrSTAry array to outStr as a tsv file
|  - Returns:
|    o 0 for success
|    o def_fileErr_amrST if outStr could not be opend
\-------------------------------------------------------*/
signed char
p_amrST(
   struct refList_amrST *refSTPtr,
   signed char *outStr
);

/*-------------------------------------------------------\
| Fun25: readTbl_amrST
|   - gets amr data from tbAmr tsv file (p_amrST; fun24)
| Input:
|   - tbAmrTblStr:
|     o c-string with path to AMR database/table
|   - errSC:
|     o signed char ponter to hold error message
| Output:
|   - Modifies:
|     o errSC to hold the error message
|       - 0 for no errors
|       - def_fileErr_amrST if could not open tbAmrTblStr
|       - def_memErr_amrST for memory errors
|   - Returns:
|     o refList_amrST struct pointer with the amrs and
|       drugs in the database
|     o 0 for errors; see errSC for specific error
\-------------------------------------------------------*/
struct refList_amrST *
readTbl_amrST(
   signed char *tbAmrTblStr, /*path to tsv with AMR(s)*/
   signed char *errSC        /*holds errors*/
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
