# Use:

Here to give you an orientation of the C code files and
  functions.

# Libraries:

- Table Of Contents (TOC)
  - [base10str](#base10str)
    - c-string to base10 number conversoin
  - [endin](#endin)
    - converts datatypes to bigendin format
  - [charCp](#charCp)
    - byte copy functions
  - [checkSum](#checkSum)
    - does checks sums for inflate; currently alder32 only
  - [endLine](#endLine)
    - line ending variable for OS's
  - [genMath](#genMath)
    - math (branchless) functions
  - [inflate](#inflate)
    - inflates deflated data (for .gz files)
  - [numToStr](#numToStr)
    - number to c-string
  - [ptrAry](#ptrAry)
    - c-string array (using pointers)
  - [shellSort](#shellSort)
    - shell short and their functions
  - [strAry](#strAry)
    - c-string array (no pointers, fixed, 60 byte width)
  - [ulCp](#ulCp)
    - long byte copy and string manipulation functions

I guess for me libraries include .c and .h files and are
  not limited to .so/.dylib files. These are sometimes
  redunent, but often are more nuianced then the base
  C code.

To find a list of functions see the SOF (Start Of File)
  block (really a table of contens, but already use TOC)
  at the start of each file. They should be mostly up to
  date.

# base10str

base10str is here to convert c-strings into nubmers. Has
  both a .c and .h file.

Functions are in form of strTo<DataType>_base10str. With
  <DataType> being the data type to convert to. Input is
  a c-string and a pointer to the data type to convert.

| short hand | data Type      | function          |
|:----------:|:---------------|:------------------|
|     UL     | unsigned long  | strToUL_base10str |
|     UI     | unsigned int   | strToUI_base10str |
|     US     | unsigned short | strToUS_base10str |
|     UC     | unsigned char  | strToUC_base10str |
|     SL     | signed long    | strToSL_base10str |
|     SI     | signed int     | strToSI_base10str |
|     SS     | signed short   | strToSS_base10str |
|     SC     | signed char    | strToSC_base10str |

Table: of base10str functions.

# charCp

Has functions for manipulating c-strings. Not very
  usefull, since they work on one byte at a time.

Has both .c and .h files.

# checkSum

Has functions to checksum checks. Currently I have the
  adler32 and crc32 checksum.

The usAdler32_checksum (fun01 checksum.c/h) is designed to
  do the adler32 checksum on a 16 bit machine. On -O0 it
  is slower than adler32_checksum (fun02 checksum.c/h),
  but on -O3 is close. See function header for details.

For all checksums, you can add bytes to a checksum using
  the `byte` functions. However, at the end you must call
  the equivulent `finish` function to complete the
  checksum.

The main checkSum functions take an array of bytes
  (unsigned chars) and convert them to their checksum. For
  the non `us` functions the input is an array of bytes
  and the number of bytes to convert and the return value
  is a signed int
  (`checkSum = check_checkSum(bytesAry, length)`).
  However, for the us functions the return is void, but it
  instead modifies a input four byte character array
  (`usCheck_checkSum(bytesAry, length, checkAryUC)`). The
  returned checksum will be in little endin format.

| Function           | Use                              |
|:-------------------|:---------------------------------|
| crc32_checkSum     | find crc32 for a input string    |
| usAlder32_checkSum | find adler32 for an input string |
| alder32_checkSum   | find adler32 for string          |

Table: list of functions to get checksums from arrays of
  data. functions starting with `us` (adler32 only) should
  work on 16bit machines **untested**.

The byte functions take in a byte and a checksum to add
  the byte to. For the non `us` functions this will be
  a signed integer (`checkByte_checkSum(byte, checkSum)`).
  For the `us` fuctions this will be a unsigned char
  array of four
  elements `(checkByte_checkSum(byte, ucharAry)`).

| Function               | Use                         |
|:-----------------------|:----------------------------|
| crc32Byte_checkSum     | add a byte to a crc32 sum   |
| usAdler32Byte_checkSum | add a byte to a adler32 sum |
| adler32Byte_checkSum   | add a byte to a adler32 sum |

Table: list of functions to add bytes to a current
  checkus (really on compiler to compile in fuction).
  functions starting with `us` (adler32 only) should work
  on 16bit machines **untested**. When done adding bytes,
  you must call a finish function to finialize the
  checksum.

The Finish functions (call after added all bytes) take
  in the computed checksum (`idFinish_checkSum(checkSum)`
  and return the finialized checksum.

| Function                 | Use                       |
|:-------------------------|:--------------------------|
| crc32Finish_checkSum     | do final step in a crc32  |
| usAdler32Finish_checkSum | do final step in adler32  |
| adler32Finish_checkSum   | do final step in adler32  |

Table: list of functions to finish off the checksum. These
  functions do not add bytes, but instead perform the
  final operation in computing the checksum. Functions
  starting with `us` (adler32 only) should work on 16bit
  machines **untested**.

Other functions include mkCrc32Tbl_checkSum, which is not
  used, but can be used to auto-generate a crc32 table.
  Also, there is the crc32Mac_checkSum macro in
  checkSum.c.  The crc32Byte_checkSum function is really a
  wrapper for crc32Mac_checkSum.

# endLine

Has the line endings for each OS. Only has one variable
  named `str_endLine` (c-string). For printf, do
  `printf(outFILE, "text%s", str_endLine);`. To add
  line ending to variable do.

```
buffStr[indexSL++] = str_endLine[0];`
if(str_endLine[1] != '\0')
   buffStr[indexSL++] = str_endLine[1];`
```

You can set the line ending with flags

| Flag              | OS         | ending |
|:------------------|------------|--------|
| `-DWINDOWS`       | windows    | `\r\n` |
| `-DLINUX`         | linux      | `\n`   |
| `-DOLD_MAC`       | 90s mac    | `\r`   |
| `-DWIN_BREAK`     | any        | `\r\n` |
| `-DLINUX_BREAK`   | any        | `\n`   |
| `-DOLD_MAC_BREAK` | do not use | `\r`   |

Table: table of unix compiler flags to set the line break
  style. For windows replace `-D` with `/D`. Also never
  use `-DOLD_MAC` or `-DOLD_MAC_BREAK`. These are ancient
  (90s) systems that are no longer used.

# endin

I use this to convert integers (or longs/shorts) to big
  endin or little endin format. Not very great, but should
  allow variations in datatype size.

For big endin OS's use `-DBIG_ENDIN` flag.

- big endin Conversion functions
  - these are only enabled if you do not use `-DBIG_ENDIN`
    otherwise the input is returned
  - long: bigEndUL = ulToBig_endin(input)
  - int: bigEndUI = uiToBig_endin(input)
  - short: bigEndUS = usToBig_endin(input)
- little endin Conversion functions
  - these are only enabled if `-DBIG_ENDIN` is used,
    otherwise the input is returned
  - long: littleEndUL = ulToLittle_endin(input)
  - int: littleEndUI = uiToLittle_endin(input)
  - short: littleEndUS = usToLittle_endin(input)
- swap endin
  - these always convert to the oppisiste endin
  - long: swapEndUL = ulSwap_endin(input)
  - int: swapEndUI = uiSwap_endin(input)
  - short: swapEndUS = usSwap_endin(input)

# genMath

These are my general math functions. Most are branchless
  functions from the stanfford bithacking guide. Has
  min, max, abloxlute values (SL for long, regular int).
  Also includes and ifMax/ifMin function to find the
  maximum/minimum and copy its paired input
  (is branchless).

The min/max ifMin/ifMax exist are .h file macros. The
  other functions are in the .c file.

# numToStr:

Converts unsigned numbers (numToStr) or doubles
  (double_numToStr) to c-strings. For doubles, negatives
  values are allowed.

Has both a .c and a .h file.

# ptrAry

Depends on ulCp.h/ulCp.c

Has my functions to manage a string pointer array.
  Includes setup (Fun05 mk_str_ptrAry), adding strings
  (Fun07 add_str_ptrAry), sorting strings
  (Fun10 sort_str_ptrAry), finding strings from a
  sorted array (Fun12 find_str_ptrAry), and comparing
  strings in the array (Fun08 cmp_str_ptrAry).

Inorder to use this you first must create a str_ptrAry
  structure with mk_str_ptrAry (provide starting array
  size). Then you add strings with add_str_ptrAry (memory
  is reallocated as needed). Finally, when finished, you
  can free the array with freeHeap_str_ptrAry (fun04).

Another way to add strings is to use addSort_str_ptrAry.
  This is inefficent, but inserts string into a sorted
  position. This does not auto realloc memory, so make
  sure you have enough memory to add one more string. If
  you need more memory call resize_str_ptrAry (fun10). The
  returned value is the index of the inserted string or -1
  for memory errors.

# inflate

I would recomend using GNU's .gz program or zlib instead.
  Only use this if they do not provide what you need (
  very unlikely).

Maybe not best setup, but should work on most OS's. For
  16 bit (not recomemded and untested) OS's you will need
  at least 40kb to 50kb of memmory.

For inflate you will need to work with the file_inflate
  (st02 in inflate.h) structure. This structure manages
  the buffers needed to read in the zip file and also
  store and uncompress the data.

Most of the variables should not be interacted with,
  however, there are a few that are safe.

- timeAryUC is a four byte unsinged char array with the 
  date stamp from a gz header (currently one). You can
  cast this to an int
- nameStr is a c-string with the file name
  - nameLenUC has the name length
- buffStr is the uncompressed buffer. Do not fiddle with
  this, unless you know what you are doing
  - buffPosSL is the current position in the uncompressed
    buffer

## file_inflate overview

To use a file_inflate struct, you must first initialize
  it, then add your file to it, then set the file you
  added to null (inflate will call fclose), then read
  bytes from the buffer. When finished you should free
  your structure.

## file_inflate initialize

To initialize use init_file_inflate (fun08 inflate.c/h).
  This takes in a pointer to an inflate structure to
  initialize.

## file_inflate add file

To add a file call add_file_inflate (fun25 inflate.c/h).
  This takes in a FILE pionter (file to add), a
  pointer to a file_inflate structure to add the file to,
  and a null (or two byte char array with the first two
  bytes of file). It returns 0 for success and a wide
  variarity of errors (see function header for a list)
  for problems. After calling, you should set the FILE
  pointer you used to 0 (inflate will close the file with
  fclose).

## file_inflate get data

You can use get_inflate (fun23 inflate.c/h) to read in
  uncompressed bytes by count (like fread), by line
  (like fget), or by block with get_inflate. The input
  is:

1. a pointer to a file_inflate structure to inflate
2. the maximum number of bytes to read in
  - depnding on choosen method it could be less
3. a flag (endBl) marking if you want to behave like
  fread (0) (maximum bytes or end of deflate), fget
  (1) (line by line), or read block by block (2)
  - this is limited to the maximum number of bytes to
    grab, so it may not always get the entire line or
    block
  - use `4` to tell inflate there are always two extra
    btyes in the buffer. This sets inflate to grab line
    breaks when buffer seems full.
  - use `1 | 4` to apply `4` when just grabbing one line
  - use `2 | 4` to apply `4` when grabbing a block
4. the c-string buffer to save the uncompressed data to
  - on text based compression, line endings are changed to
    OS line endings
5. a pointer to a signed char to hold any errors
  - set to def_eof_inflate if finished the final block
  - see the codumentation

The output is the number bytes read in or a negative
  number for errors. Also the input c-string buffer (4)
  will be filled with the uncompressed data.

## file_inflate checksum and move on

After finishing an deflate entry, you can move to the next
  deflate entry (and do checksum) with next_file_inflate
  (fun24 inflate.c/h). The input is:

- a ponter to a file_inflate structure to move to the next
  entry on
- 0; here for add_file_inflate
  - would be next to bytes (if already read in)
- 0; here for add_file_inflate
  - 1: marks the first time reading this file

The return value is 0 for no errors, def_eof_inflate if
  at the end of the file, and several other return values
  for errors. Check the function header for a list.

## file_inflate next file or re-read

You can reset the inflate structure by using
  blank_file_inflate. This will either close the file or
  seek the start of the file. It will also set all
  variables to 0.

- Input:
  - pointer to file_inflate structure to blank
  - 1 to close (fclose) the file and set to 0
    - 0 to go to the start of the file

This funcition has no return value.

## file_inflate clean up

You can clean up your file_inflate structure using
  freeStack_file_inflate to free the variables in a
  file_inflate (fun09 inflate.c/h) structure or
  freeHeap_file_inflate (fun10 inflate.c/h) to free
  a file_inflate structure (you must set pointer to
  0/null).

## Inflate example

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../genLib/inflate.h"

/*inflate also uses ulCp.h endin.h, checksum.h, genMath.h,
`   and endline.h from genLib
*/

int main(){
   signed char errSC = 0;
   signed char outStr[1024 + 1];
   signed long bytesSL = 0;

   struct file_inflate zipST;
   FILE *zipFILE = stdin;

   init_file_inflate(&zipST);

   if(add_file_inflate(zipFILE, &zipST, 0))
      /*HANDEL ERRORS*/
   zipFILE = 0; /*so I do not use fclose at end*/
   
   while(! errSC)
   { /*Loop: ucompress and print file*/
      uncompress:;

      bytesSL =
         get_inflate(
            &zipST,
            1024 - 2,/*-2 to add 2 extra bytes at end*/
            4,      /*grab line break if have 1022 chars*/
            outStr,  /*buffer to hold new bytes*/
            &errSC   /*error messages*/
         );
         /*change 4 to `1 | 4` = 5 to read line by line*/

      if(bytesSL < 0)
         /*handel errors*/

      printf("%s", outStr);
   } /*Loop: ucompress and print file*/

   errSC = next_file_inflate(&zipST, 0, 0);

   if(! errSC)
      goto uncompress; /*restart uncompress loop*/
   else if(errSC != def_eof_inflate)
      /*HANDEL ERRORS*/

   freeStack_file_inflate(&zipST);
      /*will use fclose on the FILE pointer in zipST*/
   return 1;
}
```

# shellSort

Has shell sorting functions. There are a lot (one for
  each data type). The pattern is <dataType>_shellSort to
  sort, <dataType>Search_shellSort to search for an item
  in a sorted array, and <dataType>Range_shellSort to
  find the first value in a range in a sorted array. Each
  type of function takes the same input, but is designed
  for a paticular datatype.

For example, the unsigned long sort is ul_shellSort(),
  while the signed long sort is sl_shellSort(). To find
  and item in a unsigned long array I would do
  ulSearch_shellSort();.

Shell sort has both a .c file and .h file. I had an older
  verison wich was just macros, but it turns out those
  relied on GNU extensions, and so were not c89
  compatable. That was a reall pain and I miss the
  convience of macros.

# strAry

Depends on ulCp.h/ulCp.c

This is a set of functions to manage a string array in
  a single buffer (no pointers) [think matrix]. Each
  string is 64 bytes longs.

You can make a c-string buffer with mk_strAry(size)
  (Fun01). You can then add strings with add_strAry
  (Fun04) (WARNING DOES NOT RELLOC FOR YOU). To add more
  memory use realloc_strAry(). You can get pointers to
  strings with get_strAry (Fun03; .h macro). You can sort
  strings with sort_strAry (Fun07) and find strings in the
  sorted array with find_strAry (Fun09). To compare
  strings use cmp_strAry (Fun06).

The string is justs a c-string, so use free() to free you
  strAry.

# ulCp

## ulCp: overview

ulCp is short for unsigned long copy. It is like charCp,
  except that it uses unsigned long. As a general rule it
  is slower thang strcpy, which uses SIMD. However, it
  is often faster than charCp or other byte by byte
  methods (at least on 64 bit) when strings are longer
  then 8 or 16 bytes. It also allows for you to use your
  own deliminators for some functions. Use mkDelim_ulCp
  (fun02) to make a deliminator (store it in an ulong_ulCp
  dataType).

## ulCp: warnings

I have no idea if this will work on big endin systems. Not
  a big deal since most user OSs are little endin. I have
  tested one or two functions using qemu to emulate a
  mk68040 processor on a debian image I found online
  ([https://people.debian.org/~gio/dqib/](
   https://people.debian.org/~gio/dqib/)
  ). It seems to work, but then I also had mixed results
  with a browser emulator.

Some CPUs or OS's (not sure which) will require integers
  to be 32 bit aligned, which means casting characters
  to longs will not work. If you are on an OS were
  compatibility is an issue, then use `-DNOUL` to do
  byte by byte copying. You will get unused variable
  warnings for functions that use a long deliminator and
  a character deliminator.

## ulCp: some details

I had to define my own data type for ulCp because plan9
  treats longs as 32 bits. So, their is a special flag
  (`-DPLAN9_64`) which will use `long long`
  as `ulong_ulCp` to get 64 bit dataTypes (only use on
  64 bit systems).

I have defined several deliminators at the top of the
  ulCp.h file. To see a list of functions vew the table
  of contents (Start Of File/SOF) at the start of ulCp.c
  or ulCp.h. I define 64 bit values and then use casting
  to resize it to 32 bit or whatever size is needed.

| deliminator     | defined macro        |
|:----------------|:---------------------|
| null            | def_null_ulCp (or 0) |
| new line        | def_newline_ulCp     |
| carriage return | def_carriage_ulCp    |
| tab             | def_tab_ulCp         |
| space           | def_space_ulCp       |
| comma           | def_comma_ulCp       |

Table: of deliminators in ulCp.h

There are also several long check macros to see if a value
  is in a long (fun03 to fun08 in ulCp.h). Using these
  with `-DNOUL` will be very inefficent. They really on
  a four to eight bytes being stored in longs.

| deliminator     | function          | function id |
|:----------------|:-----------------:|:------------|
| user defined    | ifDelim_ulCp      | Fun03       |
| null + user     | ifDelimNull_ulCp  | Fun04       |
| null ('\\0')    | ifNull_ulCp       | Fun05       |
| newline + null  | ifLineUnix_ulCp   | Fun06       |
| \\n + \\r + \\0 | ifEndLine_ulCp    | Fun07       |
| white space     | ifWhite_ulCp      | Fun08       |

Table: of macros that check a long for certain charaters.
  for user or user defined deliminators, use a pre-defined
  deliminator or `ulong_ulCp delim = mkDelim_ulCp(value)`
  to make a deliminator. By white space I mean any
  asccii value less then 33 (this includes space, tab,
  carriage return, newline, and null).

Finally, this has a slew of functions, check the header
  of the .c or .h file for a list.
