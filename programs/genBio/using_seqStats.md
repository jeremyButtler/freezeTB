# Use;

Seq stats is used to the mean and or medain q-scores from
  fastq files.

Files: seqStats.c and seqStats.h

- Dependencies:
  - endLine.h from genLib
  - math.h from the standard library (never managed go get
    a good log function replacement)
    - You can disable this with `-DNO_MATH_H`. The only
      problem is my log function does not work right, so
      the output is not correct.

For seqStats there are three main functions. You have a
  function to get the regular mean and median q-scores
  (`meanMedQ_seqStats`). You have a function to get the
  q-scores usings ONT's method (`oneMeanMedQ_seqStats`).
  then you have a function to print the q-scores for a
  single read (`pReadStats_seqStats`).

# Functions:

## meanMedQ\_seqStats

You can get the regular mean and medain q-scores using the
  `meanMedQ_seqStats` function. This will not use ONT's
  method so you q-scores will look higher.

- Input:
  1. c-string with the quality score entry
  2. length of the sequence
  3. float pointer to get the median q-score
  4. number of bases to skip at the start
     - ONT often skips the first 60 bases because they are
       low quality, so use 60. For Illumina use 0.
- Output:
  - Modifies:
    - input 3 to have the median q-score
  - Returns:
    - the mean q-score

**non-tested example**

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "seqStats.h"
#include "seqST.h" /*easily read in fastq file*/

int
main(
){
   signed char errSC = 0;
   float meanQF = 0;
   float medianQF = 0;
   struct seqST seqStackST;
   FILE *fqFILE = stdin;

   init_seqST(&seqStackST);

   errSC = getFq_seqST(fqFILE, &seqStackST);

   while(! errSC)
   { /*Loop: get reads and their stats*/
      meanQF =
         meanMedQ_seqStats(
            seqStackST.qStr,
            seqStackST.seqLenSL,
            &medianQF,
            60 /*ignore the first 60 bases*/
         );
      printf(
         "id=%s\tmean=%.1f\tmedian=%.1f\n", 
         seqStackST.idStr,
         meanQF,
         medianQF
      );
      errSC = getFq_seqST(fqFILE, &seqStackST);
   } /*Loop: get reads and their stats*/

   if(errSC == def_EOF_seqST)
      ; /*no errors*/
   else
      goto err_main; /*can check for the error type*/

   errSC = 0;
   goto ret_main;

   err_main:;
      errSC = 1;
      goto ret_main;

   ret_main:;
      freeStack_seqST(&seqStackST);
      fqFILE = 0; /*is only pointed to stdin*/

      return errSC;
} /*main*/
```

## meanMedQ\_seqStats

You can get the ONT mean and medain (sames as regular)
  q-scores using the `ontMeanMedQ_seqStats` function. This
  will use ONT's method so you q-scores will reflect the
  ONT output.

- Input:
  1. c-string with the quality score entry
  2. lenght of the sequence
  3. float pointer to get the median q-score
  4. number of bases to skip at the start
     - ONT often skips the first 60 bases because they are
       low quality, so use 60. For Illumina use 0.
- Output:
  - Modifies:
    - input 3 to have the median q-score
  - Returns:
    - the mean q-score

**non-tested example**

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "seqStats.h"
#include "seqST.h" /*easily read in fastq file*/

int
main(
){
   signed char errSC = 0;
   float meanQF = 0;
   float medianQF = 0;
   struct seqST seqStackST;
   FILE *fqFILE = stdin;

   init_seqST(&seqStackST);

   errSC = getFq_seqST(fqFILE, &seqStackST);

   while(! errSC)
   { /*Loop: get reads and their stats*/
      meanQF =
         ontMeanMedQ_seqStats(
            seqStackST.qStr,
            seqStackST.seqLenSL,
            &medianQF,
            60 /*ingore the frist 60 bases*/
         );
      printf(
         "id=%s\tmean=%.1f\tmedian=%.1f\n", 
         seqStackST.idStr,
         meanQF,
         medianQF
      );
      errSC = getFq_seqST(fqFILE, &seqStackST);
   } /*Loop: get reads and their stats*/

   if(errSC == def_EOF_seqST)
      ; /*no errors*/
   else
      goto err_main; /*can check for the error type*/

   errSC = 0;
   goto ret_main;

   err_main:;
      errSC = 1;
      goto ret_main;

   ret_main:;
      freeStack_seqST(&seqStackST);
      fqFILE = 0; /*is only pointed to stdin*/

      return errSC;
} /*main*/
```

## pReadStats\_seqStats

You can print the mean and median q-scores and some other
  states (likley length) using the `pReadStats_seqStats`
  function.

- Input:
  1. c-string with the quality score entry
  2. lenght of the sequence
  3. c-string with the id to print out for the read
  4. signed char telling if to print the header or not
     - 1: print header
     - 0: do not print the header
  5. number of bases to skip at the start for the ONT
     q-scores mentod
     - ONT often skips the first 60 bases because they are
       low quality, so use 60. For Illumina use 0.
  6. number of bases to skip at the start for the ONT
     q-scores mentod
     - ONT often skips the first 60 bases because they are
       low quality, so use 60. For Illumina use 0.
  7. method to print out (1 ONT; 2 non-ONT; 3 both)
  8. File to print to
- Output:
  - Prints: the stats to the output file

**non-tested example**

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "seqStats.h"
#include "seqST.h" /*easily read in fastq file*/

int
main(
){
   signed char errSC = 0;
   signed char printHeaderBl = 1;

   float meanQF = 0;
   float medianQF = 0;
   struct seqST seqStackST;
   FILE *fqFILE = stdin;

   init_seqST(&seqStackST);

   errSC = getFq_seqST(fqFILE, &seqStackST);

   while(! errSC)
   { /*Loop: get reads and their stats*/
      pReadStats(
         seqStackST.qStr,
         seqStackST.seqLenSL,
         &medianQF,
         &printHeaderBl,
         60, /*ingore the frist 60 bases*/
         60, /*ingore the frist 60 bases*/
         3, /*do ONT and non-ONT q-scores*/
         stdout /*print to stdout*/
      );

      errSC = getFq_seqST(fqFILE, &seqStackST);
   } /*Loop: get reads and their stats*/

   if(errSC == def_EOF_seqST)
      ; /*no errors*/
   else
      goto err_main; /*can check for the error type*/

   errSC = 0;
   goto ret_main;

   err_main:;
      errSC = 1;
      goto ret_main;

   ret_main:;
      freeStack_seqST(&seqStackST);
      fqFILE = 0; /*is only pointed to stdin*/

      return errSC;
} /*main*/
```
