Likely out of date

This is just an quick guide on how you might use tbAmr in
  your own code. It is pretty much C or C++ only since it
  requires passing around structures.

Also, this is not very through.

# Reading in the AMR database

You will need the `amrST` (st01 amrST.c/h)

The first step is to read in the AMR database. This can
  be done with readTbl_amrST (fun11 in amrST.c/h).

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
  an amrST holding the database. This array of
  amrST's can be freeded with freeHeapAry_amrST
  (fun05 amrST.c/h).

The returned amrST array is sorted by the first
  mapped reference base. The index of the AMR closest to
  an input mapping coordinate can be found using 
  getAmr_amrST (fun07 amrST.c/h).

# Checking AMRs for all reads in an sam file

## Detecting AMRs:

The amrChecks can be run on an single sam file using
  samFindAmrs_checkAmr (fun15 checkAmr.c/h). This function
  takes the antibiotic array, AMR database, as well as
  the lengths that were output form readTbl_amrST. It also
  takes in several user settings.

However, for individual checks you will need to read in
  the sam file using readSamLine
  (fun07 ../genLib/samEntry.c/h) and an samEntry structure
  (st01 ../generalLib/samEntry.h). You will also
  need a buffer, which will need to be freeded when you
  are done (remember to set variable to null).

Once you have the sam file you can call checkAmr
  (fun07 checkAmr.c/h) to check for AMRs. This will
  return an amrHit_checkAmr structure (st01 checkAmr.h)
  that has a pointer to each detected AMR event (an amrST
  structure).

checkAmr will also incument counters in each amrST that
  recored the number of supported mapped reads and total
  mapped reads. There is no function to blank these
  counters. So, you will have to blank numSupReadsUI and
  numMapReadsUI in the amrST structure yourself.

## Printing detected AMRs:

You can print out the detected AMRs using the
  amrHit_checkAmr list returned from checAmr.

For consensuses, you can use pCon_checkAmr (fun10;
  checkAmr.c/h). To get the consensus header use
  pConHeap_checkAmr (fun09; checkAmr.c/h).
  
To print out read ids and their detected variants, use
  pIdVarTbl_checkArm (fun14; checkAmr.c/h). You can print
  out the header with pIDVarHead_checkArm (fun13;
  checkAmr.c/h).

To get the read AMR table, use pRead_checkAmr (fun12;
  checkAmr.c/h) after all reads have been scanned for
  AMRs (amrST structs have all needed infor). You can use
  pReadHead_checkAmr (fun11; checkAmr.c/h) to print out
  the header.

# Freeing amrHit_checkAmr lists

You can free the amrHit_checkAmr lists from checkAmr using
  freeHeapList_amrHit_checkAmr (fun03; checkAmr.c/h). Just
  remember to set the variable pointing to the
  amrHit_checkAmr list to 0 (null) after freeing.
