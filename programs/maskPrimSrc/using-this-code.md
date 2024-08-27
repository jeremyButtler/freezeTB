# Note:

out of date

# Using in code:

This is an simple and not very through guide on how to
  use functions form primMask in your program. I am not
  sure if you will undersand it, but it should point you
  to the corect functions to use.

When I say (fun-xy file.c/h) I am telling you to do an
  case senestive search for the function/macro using
  `Fun-xy` in file.c or file.h. For (fun-xy file.h) I am
  saying `Fun-xy` is only in file.h.

For (st-xy file.h) I am telling you to do an case
  senestive search for structure `ST-xy` in file.h.

At the start of each file you will see an name block that
  has the files name an purpose. After that you will see
  an SOF (Start Of Function) block that will have all
  functions and strucutres in the .c and .h files. For
  functions unique to an .h file you will see an `.h`
  before the function number. For functions unique to .c
  files you will see an `.c` before the number. The best
  way to get an feel for this is to open primMask-fun.c/h.

As an rule the included libraries are in the .c files
  instead of the .h files. I also often include an
  hidden dependency block which lists dependencies that
  are not called directly by this .c file, but are called
  by another .c file this code uses.

fqGetIds is one of the few exeptions to this for freezeTB,
  but that was due to it being an older program and I have
  not gone back and worked on it.

# Basic overview

The simple way is to read in the primer table using
  readPrimCoords (fun-11 primMask-fun.c/h). You will
  need an separate array for each input. The returned
  arrays will be sorted by starting and ending positions.
  This will return 0 if there was an error.

You then read in the sam file using an samEntry structure
  (st-01 ../generalLib/samEntryStruct.h) with readSamLine
  (fun-09 ../generalLib/samEntryStruct.c/h). Before
  calling readSamLine you will need to first initialize
  the samEntry structure with initSamEntry (fun-02
  ../generalLib/samEntryStruct.c/h) if you did not use
  makeSamEntry (fun-05 ../generalLib/samEntryStruct.c/h).

The readSamLine function takes in the initialized samEntry
  structer, an buffer (auto expanded), the length of the
  current buffer (updated), and the sam file (as FILE) as
  to get the entry form. This returns 0 for success, 1 for
  EOF, and other values for errors.

You will need to check the first character of the extraStr
  entry in the samEntry structure (*samST.extraStr == '@')
  to see if it is an comment entry.

After that you ca mask the primers using maskPrimers
  (fun-08 primMask-fun.c/h). This takes in the samEntry
  structure (as pointer), the modified arrays and return
  values from readPrimCoords (fun-11 primMask-fun.c/h),
  the number of bases fudge by and the character to mask
  with.

After masking you can print out the sequence as an sam
  entry using pSamEntry (fun-11
  ../generalLib/samEntryStruct.c/h), as an fastq file
  with pSamEntryAsFastq
  (fun-12 ../generalLib/samEntryStruct.c/h), as an fasta 
  with pSamEntryAsFasta
  (fun-13 ../generalLib/samEntryStruct.c/h). Only the sam
  output function takes an buffer to modify.

For clean up you will have to free the arrays returned
  by readPrimCoords, the samEntry structure you maded,
  and the buffer you used with readSamLine.

To free the samEntry structure use freeSamEntryStack 
  (fun-03 ../generalLib/samEntryStruct.c/h) for
  stack allocated structures (struct samEntry samST;) or
  freeSamEntry (fun-04 ../generalLib/samEntryStruct.c/h)
  for heap allocated structures (malloc, calloc, or the
  my makeSamEntry function
  (struct samEntry *samST = makeSamEntry();).

# Deeper functions

The three arrays modified by readPrimCoords hold the
  starting coordinates, the ending coordinates, and the
  flags/index's of primer pairs. The flags and index's are
  combined into an single unsigned integer array I call
  the flag array.

The starting coordaintes and ending coordinates have the
  mapping coordinates of each primer.

The Flag array has two flags and one index. The first
  flag/bit tells if the primer is an reverse complement
  primer (1) or an foward primer (0). This can be set by
  setDirPrimFlag (fun-02 primMask-fun.h) or gotten with
  getDirPrimFlag (fun-05 primMask-fun.h).

The second bit (second flag) in the flag array tells if
  the user wanted the primers to be paired (2) or unpaired
  (0). The paired flag can be set with setPairPrimFlag
  (fun-03 primMask-fun.h) or gotten with getPairPrimFlag
  (fun-06 primMask-fun.h).

The remaing part of the flag holds the index of the
  primers mate (foward/reverse primer) in the starting,
  ending, and flag arrays. The index can be set by
  setMateIndexPrimFlag (fun-04 primMask-fun.h) or gotten
  with getMateIndexPrimFlag (fun-07 primMask-fun.h).

The logic behind the flag array is that you will never
  exceed 1 billion primers (maximum index). So, I use the
  two extra bits to add extra information.
