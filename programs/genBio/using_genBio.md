# Use:

Here to give an idea of how to use the libraries in
  genBio. This file is/will be very long, so leverage the
  table of contents to find what you want.

# TOC: Table Of Contents

- genBio TOC: Table Of Contents
  - [samRef](#samEntry)
    - get reference ids and lengths from samfile header
  - [samEntry](#samEntry)
    - has structure to hold sam file entries
  - [seqST](#seqST)
    - has structure to read in fastq or fasta files

# samRef

Has structure and function to scan sam file header and
  find/extract reference ids and lengths.

# refs_samRef:

Has the list of references and their lengths found in the
  sam file header.

- lenAryUI array that has all the reference lengths
- idAryStr has the reference names
  - this is a c-string array from ../genLib/strAry.c, so
    you will need to use get_strAry() (fun03
    ../genLib/strAry.c) to get ids out of it.
- numRefUI has the number of references in lenAryUI and
  idAryStr
  
### refs_samRef workflow

The workflow for a refs_samRef struct is similar to a
  samEntry struct. You first initialize the struct, then
  setup initial memory. After that you can read in a
  samEntry header or add references one by one. You can
  then search the structure for references. Finally you
  will free the struct.

You can initialize a refs_samRef struct with
  `init_refs_samRef(refs_samRefPointer);`.

You can then setup memory using 

```
init_refs_samRef(refs_samRefPointer);
if(setup_refs_samRef(refs_samRefPointer);
   /*deal with memory errors*/
```

You can then read the headers from a sam file using
  getRefLen_samEntry. This will read the headers until
  it hits the frist sequence.

- Input:
  - pointer to a `refs_samRef` structure to store
    lengths and reference ids in
  - pointer to a samEntry to get lines
    - will be set to first sequence line
  - FILE pointer to sam file to get references from
  - FILE pointer to file to print headers to
    - use 0/null to not print headers
  - c-string pointer to get non-reference header entries
    - resized as needed (ok if pointer is set to 0)
  - unsinged long pointer to get/has header c-string size

- Output:
  - Returns 0 for no errors
  - Returns def_memErr_samEntry for memory errors
  - Returns def_fileErr_samEntry for file errors
  - stores header in input c-string
  - updates unsigned long pointer to header c-string is
    resized

You can then find references in the refs_samRef struct
  using findRef_refs_samRef. The input is the id to
  search for and teh refs_samRef struct to search. The
  output is the index of the reference or a negative
  number if no referene was found.

Finally you can free the refs_samRef structure using
  freeStack_refs_samRef (for variables in struct) or
  with freeHeap_refs_samRef (for entire struct). If
  you use freeHeap_refs_samRef, remember to set you
  structure pointer to null.

```
freeStack_refs_samRef(refs_samRefPointer);
```

```
freeHeap_refs_samRef(refs_samRefPointer);
refs_samRefPointer = 0;
```

### refs_samRef example

```
#ifdef PLAN9
  #include <u.h>
  #include <libc.h>
#else
  #include <stdlib.h>
#endif

#include <stdio.h>
#include "samEntry.h"


int
main(
){

   signed char errorSC = 0;
   signed char *headerStr = 0;
   unsigned long headerSizeUL = 0;
   signed long indexSL = 0;
   
   struct refs_samRef refsStruct;
   struct samEntry samStruct;
   
   FILE *samFILE = fopen("file.sam", "r");
   
   init_refs_samRef(&refsStruct);
   init_samEntry(&samStruct);
   
   if(! samFILE)
      /*deal with file errors*/
   if(setup_samEntry(&samStruct)
      /*deal with memory errors*/
   if(setup_refs_samRef(&refsStruct))
      /*deal with memory errors*/
   
   errorSC =
      getRefLen_samRef(
         &refStruct,
         &samStruct, /*set to first read in sam file*/
         samFILE,
         0,      /*not printing headers to output file*/
         &headerStr,
         &headerSizeUL
      );
   
   indexSL =
     findRef_refs_samRef(&refStruct,samStruct.refIdStr);
   
   if(indexSL < 0)
      /*likely umapped read (reference not in header)*/
   else
   { /*Else: found reference, print reference length*/
      printf(
         "%s\t%li\n",
         refStruct.idAryStr[indexSL],
         refStruct.lenAryUI[indexSL]
      );
   } /*Else: found reference, print reference length*/
   
   freeStack_samEntry(&samStruct);
   freeStack_refs_samRef(&refStruct);
   fclose(samFILE);
   samFILE = 0;
   
   if(headerStr)
      free(headerStr);
   headerStr = 0;
   
   return 0;
}
```


# samEntry

Has the samEntry structure, which is for reading in
  read mappings from sam files. This structure can be
  used to print sam file entries as stats (filtsam
  -out-stats), as fastq's, as sam files, or as fasta
  files.

## Error types:

| error                | meaning                     |
|:---------------------|:----------------------------|
| 0                    | no error or memory error \* |
| def_EOF_samEntry     | at end of file              |
| def_memErr_samEntry  | memory error                |
| def_fileErr_samEntry | error reading input file    |
| def_expand_samEntry  | resized input array         |

Table: error types returned by samEntry functions. \*
  means return message depends on fuction.

Not all functions return all error types. Also, 0 can
  mean no errors (if error type returned) or an error if
  a value, such as a pointer is returned.

## samEntry struct:

The samEntry struct is large, but should have enough
  in it to work with sam file entries.

- read and reference ID/name variables
   - qryIdStr: is a c-string with the read id for the
     mapped read/sequence
     - qryIdLenUC holds the length
   - refIdStr: is a c-string with the read id for the
     reference the read/sequence mapped to
     - refIdLenUC holds the length
- variables with the cigar entry
  - cigTypeStr: is the symbols (=, X, M, I, D, S, H) in
    the cigar entry
  - cigArySI: has the number of times each symbol in the
    cigar entry is repeated
  - cigLenUI is the length of cigTypeStr and cigArySI
- sequence variables
  - seqStr is a c-string with the sequence
  - qStr is a c-string with the q-score entry
    - for no q-score it can be null, `\0`, or `*\0`
  - readLenUI is the length of the sequence
- reference coordinates
  - refStartUI is the first reference base in alingment
  - refEndUI is the last reference base in alingment
  - alnReadLenUI is the number of reference bases that
    were aligned (matches + snps + deletions)
- stats:
  - mapqUC has the mapping quality
  - flagUS has the flags (2nd sam file entry)
  - medianQF has the median q-score
  - meanQF has the mean q-score
  - matchCntUI has the number of matches
  - snpCntUI has the number of SNPs
  - insCntUI has the number of insertions
  - delCntUI has the number of deletions
  - maskCntUI has the number of soft masked bases
- extra entries:
  - extraStr is a c-strig the extra entries (beyond the
    first 11) in the sam file
    - it also gets the header or comment when an `@line`
      is read
   - extraLenUI is the length (number of bytes) in
     extraStr
- other entries:
  - rNextStr has the RNEXT entry
    - length is in rnextLenUC
  - pNextSI has the PNEXT entry
  - tLenSI has the TLEN entry

### samEntry struct workflow:

To use a samEntry struct you must first initalize it,
  then setup it up (allocate initial memory). After this
  it is ready to use. Finally, you will need to free your
  samEntry struct.

### samEntry initalize and setup:

You can initializing a samEntry struct with init_samEntry
  (fun02 samEntry.c/h). This takes in a point to a
  samEntry struct to initialize. There is no return.

To setup a samEntry structure use setup_samEntry (fun03
  samEntry.c/h). This takes in a samEntry pionter and
  returns 0 for no errors and memErr_samEntry for memory
  errors.

You can skip these two steps by using makeSamEntry (fun06
  samEntry.c/h). It returns 0 for memory error or a
  pointer to a initialized and setup heap allocated
  samEntry struct on success.

At times you may need to blank all values in a samEntry
  struct. To do this use blank_samEntry (fun01
  samEntry.c/h). This will lazly blank all c-strings and
  the cigar values (first values set to 0), but will fully
  clear the quality score histogram (not mentioned in
  core section because not imporant).

### samEntry freeing:

you can free samEntry structs using freeStack_samEntry
  (fun04 samEntry.c/h) to free all internal variables or
  samEntry structs on the stack (call setup_samEntry if
  you plan to reuse).

For heap allocated samEntry structs use freeHeap_samEntry
  (fun05 samEntry.c/h). You must set the samEntry
  structure pointer to null/0.

### samEntry manipulation:

#### samEntry: get sam file entry

You can use get_samEntry (fun12 samEntry.c/h) to get a
  sam file entry from a file. This returns 0 for success,
  def_EOF_samEntry for end of file, def_fileErr_samEntry
  for an invalid line, and def_memErr_samEntry for memory
  errors.

- Input:
  - samEntry structure pionter to get new entry
    - this function calls blank_samEntry, so do not worry
      about this
  - FILE pointer with sam file to get entry from
    - advanced to next line in sam file

Be aware, comments go in extra entry.

```
errorSC = get_samEntry(samEntryStructPointer, samFILE);
if(errorSC == def_memErr_samEntry)
   /*deal with memory error*/
else if(errorSC == def_fileErr_samEntry)
   /*deal with file error*/
else if(errorSC == def_EOF_samEntry)
{ /*Else If: at end of file*/
   freeHeap_samEntry(samEntryStructPointer);
   return 1;
} /*Else If: at end of file*/

if(samEntryStructPointer->extraStr[0] == '@')
   /*if is comment (header line)*/;
else
   /*else is read*/
```

You can also read in a line and convert it to a sam file
  entry using lineTo_samEntry (fun11 samEntry.c/h). The
  input is a pointer to a samEntry struct and the c-string
  to convert. The return values are def_memErr_samEntry
  for memory errors or 0 for success.

```
lengthSL =
   getFullLine_fileFun(
      samFILE,
      &bufferStr,
      &bufferSizeSL,
      &bytesSL,  /*real bytes read from file*/
      0          /*copy at index 0 in buffStr*/
   );

if(lineTo_samEntry(samStructPionter, bufferStr))
   /*deal with memory error*/
```

#### samEntry: move to target position

You can find the sequence position of a reference position
  in a samEntry using findRefPos_samEntry (fun13
  samEntry.c/h). This returns the index of the last
  cigar entry before the current one. It also modifies
  the input to be on the correct base.

- Input:
  - samEntry structure pointer with sequence to find
    refernce position
  - int pointer to cigar entry currently on
  - int pointer to number of bases left in the cigar entry
    (what have I not moved past)
  - the reference position to find (target)
  - int poiter to the current reference position
    - this is updated to the target position or the last
      reference base in the sequence
  - int poiter to the current sequence position
    - this is updated to the target position or the last
      base

Here is an example to get base 100 in the query sequence.
  Just remember to check for deletions (missing in query).

```
signed int sequencePosition = 0;
signed int referencePosition = samEntryStruct->refStartUI;
signed int cigarPosition = 0;
signed int basesInCigar = samEntryStruct->cigArySI[0];
signed int target = 100;
signed int lastCigar = 0;

lastCigar =
   findRefPos_samEntry(
     samEntryStruct,
     &cigarPosition,
     &basesInCigar,
     target,
     &referencePosition,
     &sequencePosition,
);

if(samEntryStruct->cigTypeStr[cigarPosition] == 'D')
   /*reference base at target is a deltion in the query*/;
else
  /*do somenthing with position*/

/*now find position 1000 from current position, this can
`   only move forward
*/
lastCigar =
   findRefPos_samEntry(
     samEntryStruct,
     &cigarPosition,
     &basesInCigar,
     1000,
     &referencePosition,
     &sequencePosition,
);
```

#### samEntry: swap entries

You can swap the entries in two samEntry structures using
  swap_samEntry (fun14 samEntry.c/h). This takes pointers
  to two samEntry structures to sawp and returns nothing.

Swapping is done lazely, so pointers are swapped instead
  of copying the sequence, q-score entry, cigar arrays,
  and the extra entry, pointers are swapped. The query id,
  reference id, RNEXT, and qHistUI are not pointers, and
  so are copied.

#### samEntry: less usefull functions

**q-score update**

If you modify the q-score entry in a samEntry struct, then
  you can update thm with findQScores_samEntry (fun08
  samEntry.c/h). However, before calling, make sure you
  blank your q-score histogram (qHistUI) in your samEntry
  struct. This takes in a pointer to a samEntry struct to
  update and returns nothing. It will blank and then
  update all q-score values in the structure.

**q-score copy**

If you need to copy a q-score entry into you samEntry
  structure, use cpQEntry_samEntry (fun09 samEntry.c/h).
  This function will both copy the q-score entry and
  also update all q-score stats. You must make sure there
  is enough memory before updating the q-scores.

- Input:
  - samEntry structure pointer to copy q-scores to
  - c-string with q-scores to copy
  - 1 (always provide this)

The return value is the length of the copied c-string.

**get sam file header**

You can read the entire header for a samEntry struct
  into a c-string using getHead_samEntry
  (fun20 samEntry.c/h).

- Input:
  - samEntry structure pointer to read sam file with
    - will be set to frist non-header line
  - FILE poitner to sam file to get header from
- Returns:
  - pointer to c-string with header
  - 0 for memory errors

### samEntry printing:

#### samEntry printing; sam files

You can print a samEntry struct to a sam file using
  p_samEntry (fun15 samEntry.c/h).

- Input:
  - samEntry structure pointer to print to sam file
  - 1 or 0 marking if you want a line break at the end
    - 0: put line break at end
    - 1: do not add a line break
    - Note: line breaks will vary by OS (`\n`, `\r`,
      `\n\r`, or `\r\n`).
  - file to write the samEntry to

#### samEntry printing; fastx files

You can also print to a fastq file using pfq_samEntry (
  fun16 samEntry.c/h) or to a fasta file with
  pfa_samEntry (fun17 samEntry.c/h). Both fuctions take
  a samEntry structure to print and a file to save the
  fastx entry to. There is no return value.

#### samEntry printing; stats

Finally, you can print out the stats in a spread sheet
  format (filtsam -out-stats) with pstats_samEntry. There
  is not return value.

- Input:
  - samEntry structure pointer with entry to print
  - signed char pointer telling if to print the header
    - 1 print the header (set to 0 afterwards)
    - 0 do not print the header
  - 0 or 1:
    - 1: print counts for anonymous bases
    - 0: do not print counts for anonymous bases
  - FILE pointer with file to print to

#### samEntry example

Here is a quick example of using a samEntry struct. In
  this example I am reading each entry and printing out
  the stats for it (like filtsam does).

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #inlcude <stdlib.h>
#endif

#include <stdio.h>
#include "../genLib/endLine.h" /*gives me str_endLine*/
#include "../genBio/samEntry.h"

int
main(
){
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char headBl = 1; /*print header for stats file*/
   signed char errorSC = 0;
   struct samEntry samStruct;

   singed char *fileStr = (signed char *) "file.sam";
   FILE *samFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ setup and open file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStruct);

   if(setup_samEntry(&samStruct))
      goto memErr_main;

   samFILE = fopen((char *) fileStr, "r");
   if(! samFILE)
      goto invalidFile_main;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ get past headers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errorSC = get_samEntry(&samStruct, samFILE);

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;
   else if(errorSC == def_fileErr_samEntry)
      goto fileErr_main;
   else if(errorSC)
      goto empytFile_main;

   while(! errorSC)
   { /*Loop: get headers*/
      if(samStruct.extraStr[0] != '@')
         break; /*end of header*/

      errorSC = get_samEntry(&samStruct, samFILE);
   } /*Loop: get headers*/

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;
   else if(errorSC == def_fileErr_samEntry)
      goto fileErr_main;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ print stats for each read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(! errorSC)
   { /*Loop: get reads in sam file*/
      if(samStruct.flagUS & 4)
         continue; /*umapped read*/
      else if(samStruct.seqStr[0] == '\0')
         continue; /*no sequence*/
      else if(samStruct.seqStr[0] == '*')
         continue; /*no sequence (version 2)*/

      pstats_samEntry(&samStruct, &headBl, 1, stdout);
           /*1 is to get anonymous base counts*/
           /*headBl is set to 0 after frist call*/

      errorSC = get_samEntry(&samStruct, samFILE);
   } /*Loop: get reads in sam file*/

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;
   if(errorSC == def_fileErr_samEntry)
      goto fileErr_main;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ clean up, deal with errors, and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errorSC = 0;
   goto cleanUp_main;

   memErr_main:;
      errorSC = 1;
      fprintf(stderr, "memory error%s", str_endLine);
      goto cleanUp_main;

   invalidFile_main:;
      errorSC = 2;
      fprintf(
         stderr,
         "could not open %s%s",
         fileStr,
         str_endLine
      );
      goto cleanUp_main;

   fileErr_main:;
      errorSC = 3;
      fprintf(
         stderr,
         "%s is not a sam file%s",
         fileStr,
         str_endLine
      );
      goto cleanUp_main;

   empytFile_main:;
      errorSC = 4;
      fprintf(stderr, "empty file%s", str_endLine);
      goto cleanUp_main;

   cleanUp_main:;
      if(samFILE)
         fclose(samFILE);
      samFILE = 0;

      freeStack_samEntry(&samStruct);

      return errorSC;
}
```

# seqST

seqST is a library designed to read in sequences from
  fasta and fastq files. It also includes a reverse
  complement function.

For reading `.gz` compressed files, use the seqST
  structure in seqST with `gzSeqST`. The gz size of
  sequence reading is separate because adding gz support
  adds roughly 18kb to the musl static binary size.

## seqST structure

The seqST structure is designed to hold sequence data
  from fasta or fastq files. The variables you migth work
  with are:

- idStr holds the read id
  - the read id length is stored in idLenSL
- seqStr holds the sequence
  - the sequence length is stored in seqLenSL
- qStr (fastq only) holds the quality score entry
  - the q-score length is stored in qLenSL
  - the first character set to 0/null if no q-score entry
- offsetSL is the index of first base to align
  - for pairwise aligners (needle, water, memwater)
  - index 0
- endAlnSL is the index of last base to align
  - for pairwise aligners (needle, water, memwater)
  - index 0

## seqST functions

The workflow for a seqST structure is to initialize, then
  read sequences. You can then manipulate the seqST
  structer how you want.

### seqST setup

To initialize a seqST structure use init_seqST (fun07).
  The input is a pointer to a seqST structure to
  initialize.

### seqST get sequences

You can read a fasta sequence using getFa_seqST (fun03).
  The input is a pointer to a seqST structure to hold the
  sequence and a FILE poitner to the file to read from.
  The output is 0 for no errors, def_EOF_seqST if at end
  of file (no sequence read in), def_fileErr_seqST for
  file errors, and `def_fileErr_seqST | def_badLine_seqST`
  for non-fasta entries.

You can read a fastq sequence using getFq_seqST (fun03).
  The input is a pointer to a seqST structure to hold the
  sequence and a FILE poitner to the file to read from.
  The output is 0 for no errors, def_EOF_seqST if at end
  of file (no sequence read in), def_fileErr_seqST for
  file errors, and `def_fileErr_seqST | def_badLine_seqST`
  for non-fasta entries.

### seqST reverse complement

You can reverse complement a sequence using revComp_seqST.
  The input is a pionter to a seqST structure to reverse
 complement.

### seqST read fasta file

You can get every sequence from a fasta file using
  readFaFile_seqST. The output is a pointer to a seqST
  structure array (sorted by id) or 0 for any kind of
  error.

- Input
  - FILE pointer to fasta file to get sequences from
  - signed long pointer get number of sequences read
  - signed long pointer get number of seqST structs in
    the returned array
  - signed char pointer to get any errors
    - set to def_memErr_seqST for memory errors
    - set to def_fileErr_seqST for file errors
    - set to `def_fileErr_seqST | def_badLine_seqST`
      a invalid entry

You can then find sequences by name/id using search_seqST.
  The input is the sorted seqST structure array to search,
  the name/id to search for (c-string), and the number
  of sequences in the array.

When finished with the seqST structure array, you can
  free it using freeHeapAry_seqST. You will need to
  provide the array and the number of seqST structers in
  the array.

