This is just an quick guide on how you might use tbAmr in
  your own code. It is pretty much C or C++ only since it
  requires passing around structures.

Also, this is not very through.

# Reading in the AMR database

You will need the `amrStruct` (st-01 amrStruct.c/h)

The first step is to read in the AMR database. This can
  be done with readTbAmrTbl (fun-11 in amrStruct.c/h).

- This takes in:
  - The file name of the database (c-string) (char *)
  - an integer pointer (int *) to hold the number of
    extracted AMRs
  - an pointer to an c-string (char **) to hold the
    antibiotic names
  - an pointer to an integer (int *) to hold the number of
    anbitiotics
  - An integer pointer (int *) that will hold the max
    number of c-strings that can be in antibiotic array
  - An pointer to an character (char *) to report errors
    - 0 for no errors
    - def_amrST_invalidFILE for could not open the
      file
    - def_amrST_memError for memory errors

The output modifies most of the input values and returns
  an amrStruct holding the database. This array of
  amrStruct's can be freeded with freeAmrStructArray
  (fun-05 amrStruct.c/h).

The returned amrStruct array is sorted by the first
  mapped reference base. The index of the AMR closest to
  an input mapping coordinate can be found using 
  findNearestAmr (fun-09 amrStruct.c/h).

# Checking AMRs for all reads in an sam file

The amrChecks can be run on an single sam file using
  lookForAmrsSam (fun-17 checkAmr.c/h). This function
  takes the antibiotic array, AMR database, as well as
  the lengths that were output form readTbAmrTbl. It also
  takes in several user settings.

However, for individual checks you will need to read in
  the sam file using readSamLine
  (fun-09 ../generalLib/samEntryStruct.c/h) and an
  samEntry structure
  (st-01 ../generalLib/samEntryStruct.h). You will also
  need a buffer, which will need to be freeded when you
  are done.

Once you have the sam file you can call checkAmrSam
  (fun-09 checkAmr.c/h) to check for AMRs. This will
  return an amrHit structure (st-01 checkAmr.h) that
  has the AMRs hit. It will also incument counters in
  each amrStruct that recored the number of supported
  mapped reads and total mapped reads. Their is not
  function to blank these counters. So, you will have to
  blank numSupReadsUI and numMapReadsUI yourself.

Once you get an amrHit structure you can print it using
  pHeadAmrReadIds (header; fun-15 checkAmr.c/h) and
  pAmrReadIds (fun-16 checkAmr.c/h) (read id and variant
  id) for reads.

For consensuses or full entries use pHeadAmrHitList
  (fun-11 checkAmr.c/h) and pAmrHitList
  (fun-12 checkAmr.c/h).

You can print out the read AMR report using pHeadAmrs
  (fun-13 checkAmr.c/h) to print out the header and pAmrs
  (fun-14 checkAmr.c/h) to print out the amrStructs that
  had enough mapped reads.
