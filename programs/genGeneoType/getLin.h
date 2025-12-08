/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getLin SOF:
'   - get lineages for a genome or group of reads mapped
'     to a reference genome
'   ! fun01 to fun11 do not apply default lineages, but
'     instead return if the lineage was a default lineage
'     function 16 uses this return do assign default
'     lineages when the search is finished
'   o header:
'     - guards and forward declarations
'   o .h st01: cnt_getLin:
'     - keeps the number of reads each lineage had
'   * lineage detection functions
'     o fun01: snpNtLineage_getLin
'       - checks if a variant supports a snp lineage for a
'         nucleotide sequence
'     o fun02: snpAALineage_getLin
'       - checks if a variant supports a snp lineage for
'         an amino acid sequence
'     o fun03: delLineage_getLin
'       - checks if variant supports a deletion lineage
'     O fun04: lengthLineage_getLin
'       - scans for a lineage that is set by length
'     o .c fun05: fastFindPatterns_kmerFind
'       - finds all patterns using kmerFind primer search
'     o .c fun06: startCoordSort_getLin
'       - sorts coordinates from fastFindPatterns by start
'     o .c fun07: filterCoords_getLin
'       - filters overlapping coordinates from
'         fastFindPatterns_getLin by
'     o fun09: insNtLineage_getLin
'       - checks if have a nucleotide insertion lineage
'     o fun10: insNtFastLineage_getLin
'       - checks if have a nucleotide insertion lineage by
'         using a faster kmer search
'     o fun11: insAALineage_getLin
'       - checks if have an amino acid insertion lineage
'     o .c fun12: coordsToTrsLin_getLin
'       - converts alignments coordinates to a trs lineage
'     o fun13: trsNtLineage_getLin
'       - tandam repeat lineage in nucleotide sequences
'     o fun14: trsNtFastLineage_getLin
'       - detect tandam repeat lineage in nucleotide
'         sequences using the faster kmer scan
'     o fun15: trsAALineage_getLin
'       - tandam repeat lineage in amino acid sequences
'     o fun16: countNtLineage_getLin
'       - find lineages by counting number of times a
'         pattern is duplicated
'     o fun17: countNtFastLineage_getLin
'       - detect count lineage in nucleotide sequences
'         using the faster kmer scan
'     o fun18: countAALineage_getLin
'       - detects lineages based on number of times a
'         pattern is duplicated for an amino acid sequence
'     o fun19: simpleLineage_getLin
'       - finds all one variant lineages in a sequence
'     o fun20: complexLineage_getLin
'       - detect simple lineages and returns the index of
'         the detected complex lineages
'       - mLinAryST (multi_linST array) must not be sorted
'         or changed from getComplexLineages_multi_linST
'   * cnt_getLin structu functions
'     o fun21: blank_cnt_getLin
'       - blanks values in a cnt_getLin struct
'     o fun22: init_cnt_getLin
'       - initializes all values in a cnt_getLin struct
'     o fun23: freeStack_cnt_getLin
'       - frees arrays and initialzes a cnt_getLine struct
'     o fun24: freeHeap_cnt_getLin
'       - frees a cnt_getLine struct
'     o fun25: addMem_cnt_getLin
'       - add memory (if memory rellocate) to a cnt_getLin
'     o .c fun26: addLineage_cnt_getLin
'       - adds a lineage to a cnt_getLin structure
'   * lineage print functions
'     o fun27: addReadLineages_cnt_getLin
'       - adds lineages for a read to a cnt_getLin struct
'     o fun28: pReadLineages_getLin
'       - prints the lineage counts for a set of reads and
'         then prints the consensus lineage for the reads
'       !!! warning, this does not have a conistent header
'           system, because the TRS lineages are variable.
'           expect different headers for different input
'     o fun29: pGenomeLineage_getLin
'       - prints lineages found by simpleLineage_getLin &
'         complexLineage_getLin for a single sequence
'       !!! warning, this does not have a conistent header
'           system, because the TRS lineages are variable.
'           expect different headers for different input
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and forward declarations
\-------------------------------------------------------*/

#ifndef GET_LINEAGES_H
#define GET_LINEAGES_H

struct samEntry;
struct tblST_kmerFind;
struct one_linST;
struct simple_linST;
struct multi_linST;
struct complex_linST;

/*-------------------------------------------------------\
| ST01: cnt_getLin:
|   - keeps the number of reads each lineage had
\-------------------------------------------------------*/
typedef struct cnt_getLin
{
   signed char **linStrAry; /*has lineage names*/
   signed int *idSizeArySI; /*max bytes for each name*/
   signed int *linCntArySI; /*number of reads supporting*/
   signed int lenSI;        /*number of lineages*/
   signed int sizeSI;       /*max lineages before resize*/

   signed long numReadsSL;  /*number of reads found*/ 
}cnt_getLin;

/*-------------------------------------------------------\
| Fun01: snpNtLineage_getLin
|   - checks if a variant supports a snp lineage for a
|     nucleotide sequence
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type for function to check
|     o 0 if had memory error
\-------------------------------------------------------*/
signed int
snpNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun02: snpAALineage_getLin
|   - checks if a variant supports a snp lineage for an
|     amino acid sequence
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type input
|     o 0 if had memory error
\-------------------------------------------------------*/
signed int
snpAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun03: delLineage_getLin
|   - checks if the variant supports a deletion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if wrong lineage type input
\-------------------------------------------------------*/
signed int
delLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun04: lengthLineage_getLin
|   - scans for a lineage that is set by length
| Input:
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
\-------------------------------------------------------*/
signed char
lengthLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun09: insNtLineage_getLin
|   - checks if have a nucleotide insertion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
\-------------------------------------------------------*/
signed int
insNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun10: insNtFastLineage_getLin
|   - checks if have a nucleotide insertion lineage by
|     using a faster kmer search
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
|     o 1 if match found
\-------------------------------------------------------*/
signed int
insNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun11: insAALineage_getLin
|   - checks if have an amino acid insertion lineage
| Input
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|   - Modifies:
|     - posArySI to be at 1st base to compare in linSTPtr
|       * for foward sequence, this is startSI
|       * for reverse sequence, this is endSI
|   - Returns:
|     o 1 if match found
|     o 0 for memory errors
|     o -1 if no match found, but the lineage supports a
|       reference match (no other lineage matches) lineage
|     o -2 if no match found
|     o -3 if did not cover the lineage
|     o -4 if wrong lineage type input
\-------------------------------------------------------*/
signed char
insAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun13: trsNtLineage_getLin
|   - detect tandam repeat lineage in nucleotide sequences
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun14: trsNtFastLineage_getLin
|   - detect tandam repeat lineage in nucleotide sequences
|     using the faster kmer scan
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage type was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun15: trsAALineage_getLin
|   - detect tandam repeat lineage in amino acid sequences
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|     o AA conversion is done on the flye
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage number of TRS
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
trsAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun16: countNtLineage_getLin
|   - find lineages by counting number of times a pattern 
|     is duplicated
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countNtLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun17: countNtFastLineage_getLin
|   - detect count lineage in nucleotide sequences using
|     the faster kmer scan
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|   - tblSTPtr:
|     o tblST_kmerFind struct pionter to use in search
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -1024 if read does not have a complete TRS section
|    o -2048 if the wrong lineage type was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countNtFastLineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   struct tblST_kmerFind *tblSTPtr, /*for search*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun18: countAALineage_getLin
|   - detects lineages based on number of times a pattern
|     is duplicated for an amino acid sequence
| Input;
|   - linSTPtr:
|     o one_linST struct pointer with lineage to check
|   - samSTPtr:
|     o samEntry struct pointer with read to check
|     o AA conversion is done on the flye
|   - posArySI:
|     o signed int array with position at in samEntry
|       * index 0 is reference position (base on)
|       * index 1 is sequence position  (base on)
|       * index 2 is the cigar position (entry on)
|       * index 3 is the number of bases left at the cigar
|         position (in entry)
| Output:
|  - Modifies:
|    o posArySI to be at 1st base to compare in linSTPtr
|      * for foward sequence, this is startSI
|      * for reverse sequence, this is endSI
|  - Returns:
|    o lineage of the read (counts - 1st_lineage_number)
|    o -1024 if read does not fully cover the range
|    o -2048 if the wrong lineage was input
|    o -4096 if had memory error
\-------------------------------------------------------*/
signed long
countAALineage_getLin(
   struct one_linST *linSTPtr, /*lineage to check*/
   struct samEntry *samSTPtr,  /*read/sequence to check*/
   signed int posArySI[]       /*coordinates*/
);

/*-------------------------------------------------------\
| Fun19: simpleLineage_getLin
|   - finds all one variant lineages in a sequence
| Input:
|   - samSTPtr:
|     o samEntry struct pointer with read to get lineages
|       for
|   - simpleSTPtr:
|     o simple_linST struct with lineages to search
|   - tblSTPtr:
|     o tblST_kmerFind struct to search with, it should
|       be setup with the `def_lenKmer_kmerFind` variable
|   - trsLinArySIPtr:
|     o signed int pionter to array to get the trs
|       lineages detected (otherwise, value set to -1024)
|   - outLenSIPtr:
|     o signed int pointer to get returned list length
| Output:
|   - Modifies:
|     o outLenSIPtr:
|       * have returned list length
|       * 0 if no errors
|       * -1 if memory error
|     o trsLinArySIPtr to have returned trs lineages
|       * freeded and then resized as needed
|       * freeded and set to 0 on memory error
|   - Returns:
|     o signed int array with the list of index's of the
|       detected lineages
|     o 0 for memory errors or no lineages
\-------------------------------------------------------*/
signed int *
simpleLineage_getLin(
   struct samEntry *samSTPtr,    /*sequence to search*/
   struct simple_linST *simpleSTPtr,/*lineages to search*/
   struct tblST_kmerFind *tblSTPtr, /*for fast searches*/
   signed int **trsLinArySIPtr,  /*gets trs lineages*/
   signed int *outLenSIPtr       /*number lineages found*/
);

/*-------------------------------------------------------\
| Fun20: complexLineage_getLin
|   - detect simple lineages and returns the index of the
|     detected complex lineages
|   - mLinAryST (multi_linST array) must not be sorted
|     or changed from getComplexLineages_multi_linST
| Input:
|   - complexSTPtr:
|     o complex_linST struct pointer with the complex
|       lineages to check
|     o should be in format of user input (do not sort)
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       lineages (allows narrowing down of complex
|       lineages needed to be searched)
|   - simpleLinArySI:
|     o signed int array that has the index of the
|       lineages from simpleLineage_getLin; returned array
|     o this is assumed to be sorted by index (value),
|       which is the default from simpleLineage_getLin
|   - trsLinArySI:
|     o signed int array that has the detected trs lineage
|     o this is input five of simpleLineage_getLin
|   - simpleLenSIPtr:
|     o signed int pointer with the number of lineages in
|       simpLinArySI and trsLinArySI
|     o this is intput six of simpleLineage_getLin
|   - outLenSIPtr:
|     o signed int pionter to get the length of the
|       returned signed int array
| Output:
|   - Modifies:
|     o simpLinArySI and trsLinArySI to have the index of
|       the simple lineages that the complex lineages did
|       not replace (so, removes lineages as needed)
|     o simpleLenSIPtr to have the new length of
|       simpLineArySI and trsLinArySI (same lengths)
|     o outLenSIPtr
|       * have the returned arrays length
|       * 0 if no complex lineages were found
|       * -1 if no simple lineages were found
|       * -2 for memory errors
|   - Returns
|     o signed int array with the index's of the detected
|       complex lineages
|     o 0 for no lineages or errrors
\-------------------------------------------------------*/
signed int *
complexLineage_getLin(
   struct complex_linST *complexSTPtr,/*complex lineages*/
   struct simple_linST *simpleSTPtr,  /*simple lineages*/
   signed int *simpleLinArySI, /*found simple lineages*/
   signed int *trsLinArySI,    /*found TRS lineages*/
   signed int *simpleLenSIPtr, /*# found simple lineages*/
   signed int *outLenSIPtr     /*returned array length*/
);

/*-------------------------------------------------------\
| Fun21: blank_cnt_getLin
|   - blanks values in a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to blank
| Output:
|   - Modifies:
|     o lenSI and numReadsSL to be 0 (lazy blank)
\-------------------------------------------------------*/
void
blank_cnt_getLin(
   struct cnt_getLin *cntSTPtr
);

/*-------------------------------------------------------\
| Fun22: init_cnt_getLin
|   - initializes all values in a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to initialze
| Output:
|   - Modifies:
|     o all values (including pointers) to be 0
\-------------------------------------------------------*/
void
init_cnt_getLin(
   struct cnt_getLin *cntSTPtr
);

/*-------------------------------------------------------\
| Fun23: freeStack_cnt_getLin
|   - frees arrays and initialzes a cnt_getLine struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter with variables to free
| Output:
|   - Modifies:
|     o frees all arrays and initializes
\-------------------------------------------------------*/
void
freeStack_cnt_getLin(
   struct cnt_getLin *cntSTPtr
);

/*-------------------------------------------------------\
| Fun24: freeHeap_cnt_getLin
|   - frees a cnt_getLine struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to free
| Output:
|   - Modifies:
|     o frees cntSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_cnt_getLin(
   struct cnt_getLin *cntSTPtr
);

/*-------------------------------------------------------\
| Fun25: addMem_cnt_getLin
|   - add memory (or if memory rellocate) to a cnt_getLin
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pionter to add memory to
|   - lenSI:
|     o new size of arrays (must be >= sizeSI in cntSTPtr)
| Output:
|   - Modifies:
|     o uses malloc to allocate memory to arrays in
|       cntSTPtr if the arrays in cntSTPtr are set to
|       0/null
|     o uses realloc to add memory to arrays in cntSTPtr
|       if the arrays have something
|     o sizeSI in cntStPtr is set to lenSI; if an increase
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors (you must clean up)
\-------------------------------------------------------*/
signed char
addMem_cnt_getLin(
   struct cnt_getLin *cntSTPtr,
   signed int lenSI
);

/*-------------------------------------------------------\
| Fun27: addReadLineages_cnt_getLin
|   - adds the lineages for a read to a cnt_getLin struct
| Input:
|   - cntSTPtr:
|     o cnt_getLin struct pointer to hold the counts for
|       each lineage
|   - simpLinArySI:
|     o signed int array with the simple lineages found by
|       simpleLineage_getLin to print
|   - trsLinArySI:
|     o signed int array with the trs lineage assigned to
|       each simple lineage (from simpleLineage_getLin)
|   - simpLenSI:
|     o number of simple lineages in simpLinArySI and
|       trsLinArySI
|   - complexLinArySI:
|     o signed int array with the complex lineages found
|       by complexLineage_getLin
|   - complexLenSI:
|     o number of complese lineages found
|   - pAllVarsBl:
|     o 1: print variants that are set to no print
|     o 0: ingore variants set to no print
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       (one variant) lineage names
|   - complexSTPtr:
|     o complex_linST struct array with the complex
|       (multiple variant) lineages names
| Output:
|   - Modifies:
|     o linStrAry, idSizeArySI, and linCntAry in cntSTPtr
|       to have all lineages in the list
|     o numReadsSL in cntSTPtr to be incurmented by 1
|     o if needed lenSI and sizeSI in cntSTPtr to account
|       for the new lineage or resize
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
addReadLineages_cnt_getLin(
   struct cnt_getLin *cntSTPtr,/*holds lineage counts*/
   signed int *simpLinArySI,   /*simple lineages found*/
   signed int *trsLinArySI,    /*simple lineage TRS #s*/
   signed int simpLenSI,       /*# found simple lineages*/
   signed int *complexLinArySI,/*complex lineages found*/
   signed int complexLenSI,   /*# found complex lineages*/
   signed char pAllVarsBl,     /*print no-print lineages*/
   struct simple_linST *simpleSTPtr,/*1VariantLineages*/
   struct complex_linST *complexSTPtr /*complex lineages*/
);

/*-------------------------------------------------------\
| Fun28: pReadLineages_getLin
|   - prints the lineage counts for a set of reads and
|     then prints the consensus lineage for the reads
|   !!! warning, this does not have a conistent header
|       system, because the TRS lineages are variable.
|       expect different headers for different input
| Input:
|   - idStr:
|     o c-string with id to print for the read
|     o printed until the first white space
|   - cntSTPtr:
|     o cnt_getLin struct pointer with lineage counts to
|       print
|   - minDepthSI:
|     o minimum depth to assign a lineage to the consensus
|   - maxMixedPercF:
|     o maximum percent read support for a minor variant
|       in mixed infection
|       * minor_variant / (major_variant + minor_variant)
|       * 50% (max) means can have an equal ratio
|       * 40% (max) means the minor variant can only take
|         up 40% of the reads
|   - outFILE:
|     o FILE pointer to file to print lineage to
| Output:
|   - Prints:
|     o header, then lineage counts, then the consensus
|       lineage
|       * format: id\ttype\tlineage_1\tlineage_2\t...
|         + id: is the read name
|         + type: marks if the count row (`count`) or if
|                 the consensus row (`consensus`)
|         + lineage_*: name of lineage for this row
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
pReadLineages_getLin(
   signed char *idStr,         /*id for sample to  print*/
   struct cnt_getLin *cntSTPtr,/*counts for each lineage*/
   signed int minDepthSI,    /*min depth to keep lineage*/
   float maxMixedPercF,        /*max minor var in mixed*/
   void *outFILE               /*file to print to*/
);

/*-------------------------------------------------------\
| Fun29: pGenomeLineage_getLin
|   - prints lineages found by simpleLineage_getLin and
|     complexLineage_getLin for a single sequence
|   !!! warning, this does not have a conistent header
|       system, because the TRS lineages are variable.
|       expect different headers for different input
| Input:
|   - idStr:
|     o c-string with sequence id/name to print out
|   - simpLinArySI:
|     o signed int array with the simple lineages found by
|       simpleLineage_getLin to print
|   - trsLinArySI:
|     o signed int array with the trs lineage assigned to
|       each simple lineage (from simpleLineage_getLin)
|   - simpLenSI:
|     o number of simple lineages in simpLinArySI and
|       trsLinArySI
|   - complexLinArySI:
|     o signed int array with the complex lineages found
|       by complexLineage_getLin
|   - complexLenSI:
|     o number of complese lineages found
|   - simpleSTPtr:
|     o simple_linST struct pointer with the simple
|       lineage names
|   - complexSTPtr:
|     o complex_linST struct array with the complex
|       lineage names
|   - pAllVarsBl:
|     o 1: print variants that are set to no print
|     o 0: ingore variants set to no print
|   - pHeadBlPtr:
|     o signed char pointer telling if to print the header
|       * 1: print header and set bool to 0
|       * 0: do not print the header
|   - outFILE:
|     o FILE pointer to print lineages to
| Output:
|   - Modifies:
|     o pHeadBlPtr to be 0 if it is set to 1
|   - Prints:
|     o header to outFILE (output file) if pHeadBlPtr is 1
|       * format:  "id\ttype\tgenome\tlineage..."
|     o lineage row to outFILE
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
plineages_getLin(
   signed char *idStr,         /*id to print out*/
   signed int *simpleLinArySI, /*simple lineages*/
   signed int *trsLinArySI,    /*tsr simple lineages*/
   signed int simpleLenSI,     /*number simple lineages*/
   signed int *complexLinArySI,/*complex lineages*/
   signed int complexLenSI,    /*number complex lineages*/
   struct simple_linST *simpleSTPtr, /*simple lineages*/
   struct complex_linST *complexSTPtr, /*complexLineages*/
   signed char pAllVarsBl,     /*print no-print lineages*/
   signed char *pHeadBlPtr,    /*1: print header*/
   void *outFILE               /*file to print to*/
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
