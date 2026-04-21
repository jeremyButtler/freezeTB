# Goal:

Describe how to use the fileFun library to read line
  by line across different OS's.

Files: fileFun.h and fileFun.c

- Dependencies:
  - ulCp
  - endline

# fileFun

## fileFun overview

Non-gz functions for file manipulation.

- [lineCnt\_fileFun](#lineCnt_fileFun)
  - count number of lines in a file
- [getLine\_fileFun](#getLine_fileFun)
  - get a line or maximum number of bytes from a file
- [getFullLine\_fileFun](#getFullLine_fileFun)
  - get a full length line from a file

| Flag           | Use                                   |
|:---------------|:--------------------------------------|
| `-DSLOW_BREAK` | slower, reliable line break detection |

Table: configuartion options. For windows replace `-D`
  with `/D`.

## lineCnt\_fileFun

lineCnt\_fileFun will seek the start of the file after it
  is finished. It will also modify the signed long to hold
  the length of the longest line in the file. This length
  does not include line breaks.

| Input            | Use                                 |
|:-----------------|:------------------------------------|
| `FILE *`         | file to find the number of lines in |
| `signed long *`  | max line length; minus line breaks  |

Table: Input for lineCnt\_fileFun

| Output               |
|:---------------------|
| number lines in file |

Table: output for lineCnt\_fileFun

**Example**

```
FILE *inFILE = fopen("file.txt", "r");
signed long *maxWidthSL = 0;
signed long fileLinesSL =
   lineCnt_fileFun(inFILE, &maxWidthSL);

/*maxWidthSL gets the width of the longest line,
`  fileLinesSL gets the number of lines in the file
`  sets inFILE to be at start
*/
```

## getLine\_fileFun

getLine\_fileFun will return the number of bytes in the
  read in buffer, but it will also modify a signed long
  to have the number of bytes read from the file. These
  values can differ, since getLine\_fileFun will replace
  line breaks (`\n`, `\r\n`, `\r`, and `\n\r`) with the
  OS line breaks it was compiled for. So, the return
  value includes the modified line break ending, while
  the modified signed long has the actual bytes read from
  the file.

| Input            | Use                             |
|:-----------------|:--------------------------------|
| `FILE *`         | file to get next line in        |
| `signed char *`  | buffer to hold output           |
| `signed long`    | maximum number of bytes to grab |
| `signed long *`  | number bytes read from file     |

Table: Input for getLine\_fileFun

| Output | Use                                           |
|:-------|:----------------------------------------------|
| > 0    | number bytes in buffer (can differ from file) |
| 0      | End Of File (EOF)                             |

Table: Output for getLine\_fileFun

**Example**

```
signed char bufferStr[512];
signed long realLengthSL = 0;
signed long numBytesSL = 0;
FILE *inFILE = fopen("file.txt", "r");

numBytesSL =
   getLine_fileFun(inFILE, bufferStr, 521, &realLengthSL);
   /*real length gets the number of bytes fgets read in
   `   numBytesSL gets the number of bytes in bufferStr
   `    - this count excludes the `\r` in `\r\n`
```


## getFullLine\_fileFun

getFullLine\_fileFun will use expand a buffer until a full
  line has been read in. It uses getLine\_fileFun to read
  in line by line.

| Input            | Use                                |
|:-----------------|:-----------------------------------|
| `FILE *`         | file to get next line in           |
| `signed char **` | buffer to hold output (is resized) |
| `signed long *`  | current size of buffer             |
| `signed long *`  | number bytes read from file        |

Table: Input for getFullLine\_fileFun

| Output | Use                                           |
|:-------|:----------------------------------------------|
| > 0    | number bytes in buffer (can differ from file) |
| 0      | End Of File (EOF)                             |
| < 0    | memory error when resizing the buffer         |

Table: Output for getFullLine\_fileFun

**Example**

```
signed char *bufferStr = 0;
signed long bufferSizeSL = 0;

signed long realLengthSL = 0;
signed long numBytesSL = 0;

FILE *inFILE = fopen("file.txt", "r");

numBytesSL =
   getLine_fileFun(
      inFILE,
      &bufferStr,    /*auto allocated*/
      &bufferSizeSL, /*auto filled*/
      &realLengthSL, /*bytes read in; includes '\r'*/
      0              /*start at character 0*/
   );
   /*numBytesSL gets number bytes read, but excludes the
   `  '\r' in "\r\n"
   */

if(numBytesSL)
   /*deal with memory errors*/
```

## file indexing

You can index each line in the file using the
  `lineIndex_fileFun`. After the index step you can
  extract a line at a index using the
  `getLineByIndex_fileFun` function.

To index each line in the file use `lineIndex_fileFun`.
  This function will return a signed long array with
  each index. The array will have the number of lines + 1
  elements, with the last element being one byte after
  the last character in the file (the null).

- Input:
  1. FILE pionter with the file to index
  2. signed long pointer to get the number of lines
  3. signed long pointer to get the number of bytes
     (including `\n` and `\r`) in the longest line
- Output:
  - Modifies:
    - input 2 to have the number of lines in the file
      - for memory errors; input 2 will be set to -1
    - input 3 to get teh maximum line length
  - Returns:
    - signed long array with input 2 + 1 items
      - each item is the index of a line, with the very
        last item has the null byte

After indexing the file you can get a line by index by
  using the `getLineByIndex_fileFun` function. The input
  buffer must be the size of the longest line found by
  the `lineIndex_fileFun` function.

- Input:
  1. c-string buffer to store the line in
     - must be the same size as the modified input 3 from
       `lineIndex_fileFun`
  2. index of the line to extract
  3. the index array (signed long array) returned from
     `lineIndex_fileFun` (has index's for each lines)
  4. number of lines in the file (input 2 from the
     `lineIndex_fileFun` function)
  5. FILE pointer to get lines from
- Output:
  - Modifies:
    - adds the target line to input buffer (input 1)
    - FILE pointer (intput 5) to be at end of the target
      line
  - Returns:
    - length of line (>= 0) for no errors
    - -1 if input 2 (index to extract) is out of bounds

**a non-tested example**

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "fileFun.h"

int
main(
){
   signed int retSI = 0;
   signed char *buffHeapStr = 0;

   signed long numLinesSL = 0;
   signed long maxLineLenSL = 0;
   signed long *indexHeapArySL = 0;

   FILE *inFILE = fopen("test.txt", r);

   if(! inFILE)
      goto err_main;

   indexHeapArySL =
      lineIndex_fileFun(inFILE,&numLinesSL,&maxLineLenSL);
   if(! indexHeapArySL)
      goto err_main;

   buffHeapStr =
      malloc(maxLineLenSL * sizeof(signed char));
   if(! buffHeapStr)
      goto err_main;

   if(
      getLineByIndex_fileFun(
         buffHeapStr,  /*gets the line*/
         3,            /*get 3rd line; (header +2 lines)*/
         indexHeapArySL, /*index for each line in file*/
         numLinesSL,
         inFILE
      )
   ) goto err_main; /*file does not have this many lines*/

   printf(
      "line 4 (3rd line after header): %s\n",
      buffHeapStr
   );

   retSI = 0;
   goto ret_main;

   err_main:;
      retSI = 1;
      goto ret_main;

   ret_main:;
      if(indexHeapArySL)
         free(indexHeapArySL);
      indexHeapArySL = 0;

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      return retSI;
} /*main*/
```
