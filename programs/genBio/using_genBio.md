# Use:

Here to give an idea of how to use the libraries in
  genBio. This file is/will be very long, so leverage the
  table of contents to find what you want.

# TOC: Table Of Contents

- genBio TOC: Table Of Contents
  - [samEntry](#samEntry)
    - has structure to hold sam file entries
  - [seqST](#seqST)
    - has structure to read in fastq or fasta files

# samEntry

Has the samEntry structure, which is for reading in
  read mappings from sam files. This structure can be
  used to print sam file entries as stats (filtsam
  -out-stats), as fastq's, as sam files, or as fasta
  files.

Also has refs_samEntry, which is used to hold reference
  sequences length and name. This is stored in the header.
  This is mainly here for tbCon.

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
     - lenQryIdUC holds the length
   - refIdStr: is a c-string with the read id for the
     reference the read/sequence mapped to
     - lenRefIdUC holds the length
- variables with the cigar entry
  - cigTypeStr: is the symbols (=, X, M, I, D, S, H) in
    the cigar entry
  - cigArySI: has the number of times each symbol in the
    cigar entry is repeated
  - lenCigUI is the length of cigTypeStr and cigArySI
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
  - numMatchUI has the number of matches
  - numSnpUI has the number of SNPs
  - numInsUI has the number of insertions
  - numDelUI has the number of deletions
  - numMaskUI has the number of soft masked bases
- extra entries:
  - extraStr is a c-strig the extra entries (beyond the
    first 11) in the sam file
    - it also gets the header or comment when an `@line`
      is read
   - lenExtraUI is the length (number of bytes) in
     extraStr
- other entries:
  - rNextStr has the RNEXT entry
    - length is in lenRNextUC
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
  def_EOF_samEntry for end of file, and
  def_memErr_samEntry for memory errors.

- Input:
  - samEntry structure pionter to get new entry
    - this function calls blank_samEntry, so do not worry
      about this
  - c-string pointer to buffer to refill
    - this is resized as needed, until it can accomindate
      the largest entry in the sam file
    - it is ok if this points to null
  - unsigned long pointer to get the size of the input
    c-string
    - this is changed as needed
  - FILE pointer with sam file to get entry from
    - advanced to next line in sam file

Be aware, comments go in extra entry.

```
if(samEntryStruct->extraStr[0] == '@')
   /*handel comment (header line)*/;
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
{ /*Else: do somenthing with position*/
} /*Else: do somenthing with position*/
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

get_samEntry is a wrapper around two functions, which can
  be called separately to skip. The first is
  getLine_samEntry (fun10 samEntry.c/h), which reads in a
  single line from a sam file. The next is lineTo_samEntry
  (fun11 samEntry.c/h), which adds a line to a samEntry
  struct. See the function headers for more information if
  you are intrested in using one of these functions.

If you modify the q-score entry in a samEntry struct, then
  you can update thm with findQScores_samEntry (fun08
  samEntry.c/h). However, before calling, make sure you
  blank your q-score histogram (qHistUI) in your samEntry
  struct. This takes in a pointer to a samEntry struct to
  update and returns nothing.

Here is how you would blankl the q-score entry:

```
for(qScore = 0; qScore < def_maxQ_samEntry; ++qScore)
   samEntry_struct->qHistUI[qScore] = 0;
```

### samEntry printing:

#### samEntry printing; sam files

You can print a samEntry struct to a sam file using
  p_samEntry (fun15 samEntry.c/h). The return value is
  0 for no errors and def_memErr_samEntry for memory
  errors.

- Input:
  - samEntry structure pointer to print to sam file
  - pointer to c-string buffer to hold printed entry
    - this is resized as needed (use same buffer as
      get_samEntry)
  - unsigned long pointer with buffer size
    - updated when needed
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
   signed char *buffStr = 0;
   unsigned long sizeUL = 0;

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

   errorSC =
     get_samEntry(&samStruct, &buffStr, &sizeUL, samFILE);

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;
   else if(errorSC)
      goto empytFile_main;

   while(! errorSC)
   { /*Loop: get headers*/
      if(samStruct.extraStr[0] != '@')
         break; /*end of header*/

      errorSC =
         get_samEntry(
            &samStruct,
            &buffStr,
            &sizeUL,
            samFILE
         );
   } /*Loop: get headers*/

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;
   else if(errorSC)
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

      errorSC =
         get_samEntry(
            &samStruct,
            &buffStr,
            &sizeUL,
            samFILE
         );
   } /*Loop: get reads in sam file*/

   if(errorSC == def_memErr_samEntry)
      goto memErr_main;

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

      free(buffStr);
      buffStr = 0;

      return errorSC;
}
```

## refs_samEntry:

Not likely used often, but holds the list of references
  and their lengths found in the sam file header.

- lenAryUI array that has all the reference lengths
- idAryStr has the reference names
  - this is a c-string array from ../genLib/strAry.c, so
    you will need to use get_strAry() (fun03
    ../genLib/strAry.c) to get ids out of it.
- numRefUI has the number of references in lenAryUI and
  idAryStr
  
### refs_samEntry workflow

The workflow for a refs_samEntry struct is similar to a
  samEntry struct. You first initialize the struct, then
  setup initial memory. After that you can read in a
  samEntry header or add references one by one. You can
  then search the structure for references. Finally you
  will free the struct.

#### ref_SamEntry  initialize
