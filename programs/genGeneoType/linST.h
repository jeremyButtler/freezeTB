/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' linST SOF:
'   - has structure and functions to read a lineage
'     database
'   o header:
'     - guards, defined variables, & forward declarations
'   * structures
'     o .h st01: one_linST
'       - has data for one lineage
'     o .h st02: simple_linST:
'       - holds a array of one_linST structures, which are
'         simple lineages that are made from single
'         variants and the supporting array for default
'         lineage detection
'     o .h st03: multi_linST
'       - has data for a complex (multi-variant) lineage
'     o .h st04: complex_linST
'       - has a mulit_linST (complex (multi-variant))
'         lineage array
'   * one_linST structures
'     + one_linST general functions
'       o fun01: blank_one_linST
'         - blanks (sets to defaults) a one_linST
'       o fun02: init_one_linST
'         - initializes (all values to 0) a one_linST
'       o fun03: freeStack_one_linST
'         - frees arrays & blanks variables in one_linST
'       o fun04: freeHeap_one_linST
'         - frees a one_linST strucutre
'       o fun05: freeHeapAry_one_linST
'         - frees a one_linST struct array
'     + one_linST sorting and swap functions
'       ! these functions are hidden to avoid users
'         messing things up. Just using them will cause
'         errors for the multi_linST structures.
'       o .c fun06: swap_one_linST
'         - swap values between one_linST structures
'       o .c fun07: sortPosAry_one_linST
'         - sorts one_linST struct array by start position
'       o .c fun08: sortIdAry_one_linST
'         - sorts a one_linST struct array by variant id
'       o .c fun09: sortGroupAry_one_linST
'         - sorts one_linST struct array by variant group
'           and variants in a group by variant id
'       o .c fun10: unsort_one_linST
'         - unsorts a one_linST array using an array of
'           original index's
'     + one_linST find functions (all need a sorted array)
'       ! fun12 and fun 12 are hidden becuase fun39
'         retuns a position sorted array
'       o fun11: posFindAry_one_linST
'         - find the first variant that is between the
'           start and end position
'       o .c fun12: idFindAry_one_linST
'         - find the variant with the input id
'       o .c fun13: groupFindAry_one_linST
'         - find the first variant that is in the group
'   * simple_linST structure general functions
'     o fun14: blank_simple_linST
'       - does a lazy blank and sets all lengths in a
'         simple_linST struct to 0
'     o fun15: init_simple_linST
'       - sets array addresses in simple_linST to 0/null
'     o fun16: freeStack_simple_linST
'       - frees all arrays in a simple_linST struct
'     o fun17: freeHeap_simple_linST
'       - frees a simple_linST struct
'     o fun18: addMem_simple_linST
'       - adds memory to a simple_linST struct
'     o fun19: clearGroup_simple_linST
'       - clears all detected groups in a simple_linST
'     o fun20: unsort_simple_linST
'       - unsorts a simple_linST struct using an array
'         of original index's
'   * multi_linST structures
'     o fun21: blank_multi_linST
'       - blanks (sets to defaults) a multi_linST struct
'     o fun22: init_multi_linST
'       - initializes a multi_linST structure
'     o fun23: freeStack_multi_linST
'       - frees variables in a multi_linST structure
'     o fun24: freeHeap_multi_linST
'       - frees a multi_linST structure
'     o fun25: freeHeapAry_multi_linST
'       - frees a multi_linST struct array
'   * complex_linST structure general functions
'      o fun26: blank_complex_linST
'        - sets length in a complex_linST to 0;(lazy blank
'      o fun27: init_complex_linST
'        - sets all values complex_linST to 0/null
'      o fun28: freeStack_complex_linST
'        - frees all variabls in a complex_linST struct
'      o fun29: freeHeap_complex_linST
'        - frees a complex_linST struct
'      o fun30: addMem_complex_linST
'        - adds memory to a complex_linST struct
'      ! fun31 to fun36 are only used in getting complex
'        lineages to decrease search time in reading in
'        the database
'      o .c fun31: swap_complex_linST
'        - swap two index in a complex lineage array
'      o .c fun32: addIdSort_complex_linST
'        - add complex id to an lineage by sorting
'      o .c fun33: idFind_complex_linST
'        - finds an id in an id sorted complex_linST
'        - this is only used in the reading in function
'      o .c fun34: groupSort_complex_linST
'        - sorts a mult_linST array in a complex_linST
'          struct by the group id
'      o .c fun35: positionSort_complex_linST
'        - sorts a mult_linST array in a complex_linST
'          struct by the position (end coordinate)
'        - this sort also makes sure that lineages that
'          have more variants are at the end
'        - between end and total at ends, this ensures
'          complex lineages that are depenent on other
'          complex lineages end up at the end
'      o .c fun36: unsort_complex_linST
'        - unsort a mult_linST array in a complex_linST
'          by the original index's
'      o fun37: posFind_complex_linST
'        - find the first complex variant that is between
'          the start and end position in a complex_linST
'   * general functions
'      o .c fun38: flipIndexAry_complex_linST
'        - changes the index array so that the stored
'          index is the position and the old position is
'          the new stored index
'     o fun39: getSimpleLineages_linST
'       - gets the lineages from the variants lineage file
'     o .c fun40: intInsert_linST
'       - inserts an integer into an array at its sorted
'         position (this does not insert duplicates)
'     o fun41: getComplexLineages_linST
'       - gets the lineages from the complex lineage file
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards, defined variables, and forward declarations
\-------------------------------------------------------*/

#ifndef LINEAGE_STRUCUTER_H
#define LINEAGE_STRUCUTER_H

#define def_unknown_linST 0

#define def_ntSeq_linST 1
#define def_aaSeq_linST 2

/*these are the lineage type checks, for each type getLin
`  has a function
*/
#define def_noType_linST 0
#define def_snpType_linST 1
#define def_delType_linST 2
#define def_lenType_linST 3
#define def_insType_linST 4
#define def_insFastType_linST 5
#define def_trsType_linST 6
#define def_trsFastType_linST 7
#define def_countType_linST 8
#define def_countFastType_linST 9

#define def_orLineage_linST '|'
   /*altnernative pattern for a lineage*/
#define def_defLineage_linST '='
   /*default lineage for group*/
#define def_trsLinMark_linST ':'
#define def_needLin_linST '&'
#define def_notLin_linST '!'

struct refST_kmerFind;
struct tblST_kmerFind;

/*-------------------------------------------------------\
| ST01: one_linST
|   - has data for one lineage
\-------------------------------------------------------*/
typedef struct one_linST
{
   /*__________________identification___________________*/
   signed char *geneStr;  /*gene name*/
   signed char *groupStr; /*group the lineage belongs to*/
   signed char *idStr;    /*name of the lineage*/

   /*_____________________coordiantes___________________*/
   signed int startSI;    /*first base in the lineage*/
   signed int endSI;      /*last base in the lineage*/

   /*_________________lineage_system_type_______________*/
   signed char moleculeTypeSC;
      /*uses:
      `   def_unknown_linST: for blank or no input
      `   def_ntSeq_linST: for nucleotide sequence
      `   def_aaSeq_linST: for amino acid sequence
      */

   signed char checkTypeSC;
      /*uses:
      `  def_unknown_linST: for blank or no input
      `  def_snpType_linST: for snp checks
      `  def_delType_linST: for deletion checks
      `  def_insType_linST: for insertion checks
      `  def_insFastType_linST: for fast insertion checs
      `  def_lenType_linST: length lineage check
      `  def_trsType_linST: for tandum repeat checks
      `  def_trsFastType_linST: for fast tandum repeat
      `    checks
      */

   signed char revDirBl; /*1: for reverse; 0 for foward*/

   /*_________________pattern_or_sequence_______________*/
   signed char **seqAryStr; /*has sequence patterns*/
   signed int *seqLenArySI; /*sequence lengths*/
   signed int seqSizeSI;   /*max patterns*/
   signed int seqLenSI;    /*number of patterns*/
   signed char refMatchBl;  /*1: is the lineage of the
                            `    reference (no sequence)
                            */
   signed char defGroupBl;  /*1: this lineages has the
                            `  the default lineage for its
                            `  group
                            ` 0: not a default lineage
                            */
   struct refST_kmerFind *fastAryST;
                            /*structures needed for the
                            `  fast search methods
                            */
   signed int minScoreSI;   /*minimum score to keep aln*/
   signed int fudgeSI;      /*max number of differences
                            `  between lineage and read,
                            `  also applies to score
                            */
   signed int trsGapSI;     /*max gap bettween two TRS
                            `  repeats
                            */

   signed char printLinBl;   /*1: this linage can be
                             `  printed as a result
                             ` 0: lineage can not be
                             `    printed by its self, ex
                             `    if is part of a complex
                             `    lineage
                             */
   signed char *lineageStr; /*has lineage id (non-TRS)*/
   signed int trsLineageSI; /*has number of repeat copies
                            `  in the first TRS lineage
                            */
}one_linST;

/*-------------------------------------------------------\
| ST02: simple_linST:
|   - holds a array of one_linST structures, which are
|     simple lineages that are made from single variants
|     and the supporting array for default lineage
|     detection
\-------------------------------------------------------*/
typedef struct simple_linST
{
   struct one_linST *linAryST; /*one_linST struct array*/
   signed int lenSI;  /*number of lineages*/
   signed int sizeSI; /*maximum lineages before resize*/

   signed int *groupArySI;
      /*index group each lineage is assigned to, this will
      `  be the same size as linAryST
      */
   signed char *foundGroupAryBl;
      /*tells if found a non-default group lineage, this
      `  will be shorter then groupArySI and linAryST
      `  do -1 if the default group was not found
      */
   signed int *defGroupArySI;
      /*has the index's for the default lineage for each
      `  group or -1 if there is no default
      */
   signed int groupLenSI;
      /*number of unique groups in the linAryST struct
      `  (length of foundGroupAryBl)
      `  o flags:
      `    * -1: this group is not in the read
      `    * 0: this groups default was in the read
      `    * 1: non-default group assigned
      */
}simple_linST;

/*-------------------------------------------------------\
| ST03: multi_linST
|   - has data for a complex (multi-variant) lineage
\-------------------------------------------------------*/
#define def_matchComplexType_linST 1
   /*find lineages that match the variant patterns*/
#define def_closestComplexType_linST 2
   /*find lineage by finding closet complex lineage type*/
#define def_startVariantsComplex_linST '*'
#define def_startIgnoreComplex_linST '#'
   

typedef struct multi_linST
{
   signed char *idStr;  /*id of complex lineage*/
   signed char *groupIdStr;/*group complex lineage is in*/
   signed char *geneStr;/*gene assigned to multi lineage*/
   signed char *lineageStr; /*lineage to assign*/
   signed char typeSC;  /*type of lineage looking at*/
   signed int fudgeSI; /*how many lineages can be off*/
   signed char overwriteBl;
      /* Values:
      `    - 1: overwrite simple lineages in the complex
      `         lineage
      `    - 2: overriwte default lineage
      `    - 3: overriwte simple lilneages and the default
      `         lineage
      */
   signed char printLinBl;   /*1: this linage can be
                             `  printed as a result
                             ` 0: lineage can not be
                             `    printed by its self, ex
                             `    if is part of a complex
                             `    lineage
                             */

   /*these are for figuring out what lineages are needed
   `  to get a complex lineage
   */
   signed int *linIndexArySI;
      /*index of variants in the one_linST array*/
   signed int *linTrsArySI;
      /*trs lineages for each variant that makes up this
      `  complex lineages
      */
   signed char *linNeedAryBl;
      /*tells if lineage is needed to call (1)*/
   signed char *linNotAryBl;
      /*tells if lineage needs to be missing (not
      `  present)
      */
   signed int linLenSI;
      /*number of variants to get lineage*/
   signed int *defGroupArySI;
      /*default groups to remove when I am removing
      `   default lineages
      */
   signed int defGroupLenSI; /*number default lineages*/
   
   signed int *mLinIndexArySI;
      /*array of each multi_linST index assigned to this
      `  complex lineage
      */
   signed char *mLinNeedAryBl;
      /*tells if lineage is needed to call (1)*/
   signed char *mLinNotAryBl;
      /*tells if lineage needs to be missing (not
      `  present)
      */
   signed int mLinLenSI;
      /*number of multi-variants in lineage assignment*/

   signed int totalOneLinSI; /*total one_linST structures
                             `  that are in this lineage,
                             `  this includes ones hidden
                             `  in complex lineages.
                             `  It is basic and assumes
                             `  no complex lineage shares
                             `  a one_linST (var) lineage
                             */
   signed int totalLinSI;   /*total lineages that are in
                            `  in this lineage (includes
                            `  complex lineages)
                            */
   /*these variables are used to figure out coordinates
   `  for positions
   */
   signed int startSI; /*first base possible in lineage*/
   signed int endSI;   /*last base possible in lineage*/
}multi_linST;

/*-------------------------------------------------------\
| ST04: complex_linST
|   - has a mulit_linST (complex (multi-variant)) lineage
|     array
\-------------------------------------------------------*/
typedef struct complex_linST
{
   struct multi_linST *linAryST;/*complex lineage array*/
   signed int lenSI;  /*number of complex lineages*/
   signed int sizeSI; /*maximum lineages before resize*/
   signed int *groupArySI; /*has index of the next lineage
                           `  that is in same group
                           ` Each group ends with the
                           `   first lineage index. So,
                           `   each group has a circular
                           `   list
                           */
   signed int *cntArySI; /*here to recored number hits
                         `   I had per complex lineage
                         */
}complex_linST;

/*-------------------------------------------------------\
| Fun01: blank_one_linST
|   - blanks (sets to defaults) a one_linST structure
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to blank
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o all arrays to be set to 0/null
\-------------------------------------------------------*/
void
blank_one_linST(
   struct one_linST *linSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_one_linST
|   - initializes (all values to 0) a one_linST structure
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to initialize
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o all arrays to be 0
\-------------------------------------------------------*/
void
init_one_linST(
   struct one_linST *linSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_one_linST
|   - frees all arrays and blanks variables in a one_linST
| Input:
|   - linSTPtr:
|     o one_linST structure pointer with variables to free
|       and blank
| Output:
|   - Modifies:
|     o all non-array variables in linSTPtr to be 0/null
|     o frees all arrays (not keeping track)
\-------------------------------------------------------*/
void
freeStack_one_linST(
   struct one_linST *linSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_one_linST
|   - frees a one_linST strucutre
| Input:
|   - linSTPtr:
|     o one_linST structure pointer to free
| Output:
|   - Modifies:
|     o frees linSTPtr, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_one_linST(
   struct one_linST *linSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeapAry_one_linST
|   - frees a one_linST struct array
| Input:
|   - linAryST:
|     o one_linST structure array to free
|   - lenSI:
|     o number of one_linST structures in the array
| Output:
|   - Modifies:
|     o frees linAryST, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeapAry_one_linST(
   struct one_linST *linAryST,
   signed int lenSI
);

/*-------------------------------------------------------\
| Fun11: posFindAry_one_linST
|   - find the first variant that is between the start and
|     end position
| Input:
|   - startSI:
|     o start of range to find
|   - linAryST:
|     o one_linST struct array to find variant
|   - lenSI:
|     o number of variants in linAryST
| Output:
|   - Returns:
|     o index of first possible variant
|     o -1 if no variant is after the start position
\-------------------------------------------------------*/
signed int
posFindAry_one_linST(
   signed int startSI, /*first base variant can have*/
   struct one_linST *linAryST, /*array of lineages*/
   signed int lenSI    /*number of variants in array*/
);

/*-------------------------------------------------------\
| Fun14: blank_simple_linST
|   - does a lazy blank and sets all lengths in a
|     simple_linST struct to 0
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to blank
| Output:
|   - Modifies:
|     o lenSI in simpleSTPtr to be 0
|     o groupLenSI in simpleSTPtr to be 0
\-------------------------------------------------------*/
void
blank_simple_linST(
   struct simple_linST *simpleSTPtr
);

/*-------------------------------------------------------\
| Fun15: init_simple_linST
|   - sets all array addresses in simple_linST to 0/null
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to initialize
| Output:
|   - Modifies:
|     o all variables to be 0/null
\-------------------------------------------------------*/
void
init_simple_linST(
   struct simple_linST *simpleSTPtr
);

/*-------------------------------------------------------\
| Fun16: freeStack_simple_linST
|   - frees all arrays in a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees all variables and initializes
\-------------------------------------------------------*/
void
freeStack_simple_linST(
   struct simple_linST *simpleSTPtr
);

/*-------------------------------------------------------\
| Fun17: freeHeap_simple_linST
|   - frees a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to free
| Output:
|   - Modifies:
|     o frees simpleSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_simple_linST(
   struct simple_linST *simpleSTPtr
);

/*-------------------------------------------------------\
| Fun18: addMem_simple_linST
|   - adds memory to a simple_linST struct
| Input:
|   - simpleSTPtr:
|     o simple_linST struct pointer to add memory to
|   - sizeSI:
|     o new size of arrays in simpleSTPtr
| Output:
|   - Modifies:
|     o linAryST and groupArySI arrays in simpleSTPtr to
|       be the size of sizeSI if sizeSI is larger then
|       sizeSI in simpleSTPtr
|       * uses malloc if no arrays
|       * uses reallic if already  have arrays
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addMem_simple_linST(
   struct simple_linST *simpleSTPtr,
   signed int sizeSI
);

/*-------------------------------------------------------\
| Fun19: clearGroup_simple_linST
|   - clears all detected groups in a simple_linST struct
| Input
|   - simpleSTPtr:
|     o simple_linST struct array to clear found groups
| Output:
|   - Modifies:
|     o foundGroupAryBl in simpleSTPtr to be filled with
|       0's
\-------------------------------------------------------*/
void
clearGroup_simple_linST(
   struct simple_linST *simpleSTPtr
);

/*-------------------------------------------------------\
| Fun20: unsort_simple_linST
|   - unsorts a simple_linST struct using an array of
|     original index's
| Input:
|   - simpleSTPtr:
|     o simple_linST struct array to unsort
|   - indexArySI:
|     o singed int array with original index's to unsort
|       to
| Output:
|   - Modifies:
|     o linSTPtr to be in unsorted order
|     o indexArySI to be 0 to lenSI - 1
\-------------------------------------------------------*/
void
unsort_simple_linST(
   struct simple_linST *simpleSTPtr,
   signed int *indexArySI
);

/*-------------------------------------------------------\
| Fun21: blank_multi_linST
|   - blanks (sets to defaults) a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to blank
| Output:
|   - Modifies:
|     o all non-array variables in mLinSTPtr to be 0/null
|     o all arrays to be set to 0/null
\-------------------------------------------------------*/
void
blank_multi_linST(
   struct multi_linST *mLinSTPtr
);

/*-------------------------------------------------------\
| Fun22: init_multi_linST
|   - initializes a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to initialize
| Output:
|   - Modifies:
|     o all arrays and variables to be set to 0/null
\-------------------------------------------------------*/
void
init_multi_linST(
   struct multi_linST *mLinSTPtr
);

/*-------------------------------------------------------\
| Fun23: freeStack_multi_linST
|   - frees variables in a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees the arrays and initializes
| Note:
|   - does not free individual pointers in the varArySTPtr
|     or multiArySTPtr arrays
\-------------------------------------------------------*/
void
freeStack_multi_linST(
   struct multi_linST *mLinSTPtr
);

/*-------------------------------------------------------\
| Fun24: freeHeap_multi_linST
|   - frees a multi_linST structure
| Input:
|   - mLinSTPtr:
|     o multi_linST structure pointer to free
| Output:
|   - Modifies:
|     o frees the mLinSTPtr structure and its variables
| Note:
|   - does not free the varArySTPtr or multiArySTPtr
|     arrays
\-------------------------------------------------------*/
void
freeHeap_multi_linST(
   struct multi_linST *mLinSTPtr
);

/*-------------------------------------------------------\
| Fun25: freeHeapAry_multi_linST
|   - frees a multi_linST struct array
| Input:
|   - mLinAryST:
|     o multi_linST structure array to free
|   - lenSI:
|     o number of multi_linST structures in the array
| Output:
|   - Modifies:
|     o frees mLinAryST, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeapAry_multi_linST(
   struct multi_linST *mLinAryST,
   signed int lenSI
);

/*-------------------------------------------------------\
| Fun26: blank_complex_linST
|   - sets length in a complex_linST to 0 (lazy blank)
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to blank
| Output:
|   - Modifies:
|     o lenSI in complexSTPtr to be 0 (lazy blank)
\-------------------------------------------------------*/
void
blank_complex_linST(
   struct complex_linST *complexSTPtr
);

/*-------------------------------------------------------\
| Fun27: init_complex_linST
|   - sets all values complex_linST to 0/null
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to initialize
| Output:
|   - Modifies:
|     o all values/arrays in complexSTPtr to be 0/null
\-------------------------------------------------------*/
void
init_complex_linST(
   struct complex_linST *complexSTPtr
);

/*-------------------------------------------------------\
| Fun28: freeStack_complex_linST
|   - frees all variabls in a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer with variables to
|       free
| Output:
|   - Modifies:
|     o frees all values/arrays in complexSTPtr and then
|       initializes
\-------------------------------------------------------*/
void
freeStack_complex_linST(
   struct complex_linST *complexSTPtr
);

/*-------------------------------------------------------\
| Fun29: freeHeap_complex_linST
|   - frees a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to free
| Output:
|   - Modifies:
|     o frees complexSTPtr, but you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_complex_linST(
   struct complex_linST *complexSTPtr
);

/*-------------------------------------------------------\
| Fun30: addMem_complex_linST
|   - adds memory to a complex_linST struct
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer to expand lineage
|       array
|   - sizeSI:
|     o size to resize to
| Output:
|   - Modifies:
|     o if linAryST in complexSTPtr is 0/null uses malloc
|     o if sizeSI is greater then sizeSI in complexSTPtr
|       used realloc to resize
|     o initializes all new array elements
|   - Returns:
|     o 0 for no errors
|     o 1 for memory erros
\-------------------------------------------------------*/
signed char
addMem_complex_linST(
   struct complex_linST *complexSTPtr,
   signed int sizeSI
);

/*-------------------------------------------------------\
| Fun37: posFind_complex_linST
|   - find the first complex variant that is between the
|     start and end position in a complex_linST struct
| Input:
|   - endSI:
|     o last base in range to find
|     o I only look for the end because the complex
|       lineages are sorted by end position
|   - complexSTPtr:
|     o complex_linST struct array to find variant index
| Output:
|   - Returns:
|     o index of first variant in between the start and
|       end
|     o -1 if no variant is in the input range
\-------------------------------------------------------*/
signed int
posFind_complex_linST(
   signed int endSI,   /*last base variant can have*/
   struct complex_linST *complexSTPtr /*complex lineages*/
);

/*-------------------------------------------------------\
| Fun39: getSimpleLineages_linST
|   - gets the lineages from the variants lineage file
| Input:
|   - noFastBl
|     o 1: ignore fast alignment types (ftrs and fins)
|     o 0: allow fast alignment types
|   - tblSTPtr:
|     o tblST_kmerFind struct pointer with settings for
|       fast alignment (only used if noFastBl == 0)
|   - errSL:
|     o signed long pointer to get the line the error was
|       on or number lines in file
|   - linFILE:
|     o FILE pionter to variant lineage file
| Output:
|   - Modifies:
|     o errSL to have line the error was on
|       * is negative if error was no variant error
|       * is positive if error was file or memory error
|       * if no error, number of lines in file
|   - Returns:
|     o a simple_linST struct with the lineages
|     o 0 for memory or file errors
\-------------------------------------------------------*/
struct simple_linST *
getSimpleLineages_linST(
   signed char noFastBl,   /*1: ignore fast types*/
   struct tblST_kmerFind *tblSTPtr,
   signed long *errSL,     /*gets line error was on*/
   void *linFILE
);

/*-------------------------------------------------------\
| Fun41: getComplexLineages_linST
|   - gets the lineages from the complex lineage file
| Input:
|   - simpleSTPtr:
|     o simple_linST struct array with variant lineages
|   - errSL:
|     o signed long pointer to get the line the error was
|       on or number lines in file
|   - linFILE:
|     o FILE pionter to variant lineage file
| Output:
|   - Modifies:
|     o errSL to have line the error was on
|       * is negative if error was no variant error
|       * is positive if error was file or memory error
|       * if no error, number of lines in file
|   - Returns:
|     o multi_linST struct array with the complex lineages
|     o 0 for memory or file errors
\-------------------------------------------------------*/
struct complex_linST *
getComplexLineages_linST(
   struct simple_linST *simpleSTPtr,
   signed long *errSL,
   void *linFILE
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2025 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
