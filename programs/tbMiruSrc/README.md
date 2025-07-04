# Use:

tbMIRU is used to detect MIRU lineages in TB samples.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install

To build you will use `make -f` with the make
  file targeting your OS. The OS the make file was built
  for is the end of the name.

Make file names:
  - mkfile.unix: any unix (Mac/Linux/BSD) OS
  - mkfile.static: static build (Linux/BSD)
  - mkfile.win: windows (does not install)
    - use `nmake /F` instead of `make -f`

```
make -f mkfile.unix
sudo make install;
```

# Run

You can get a help message with `tbMiru -h`.

You can find lineages for mapped reference (as sam file)
   and a table of primer coordinates and lineages
   using `tbMiru -sam file.sam -miru-tbl miru-Table.tsv`.

You can also scan fasta and fastq files using the kmer
  find settings and the primers in tsv format (see
  pimers.tsv file). I am not sure how well it works,
  other than on NC000962. No idea on reads.

Also note, if you are using `-fa` (fasta file) or `-fq`
  (fastq) you need to provide the MIRU-VNTR primers. The
  names on the primers must exactly match the names in the
  table. So, for example, primer `0154` is named `0154`
  in the primer file and is named `0154` in the table.
  The table will included some extra stuff after the name,
  but that is after the first `.`.

Also, do not use `.` in your names. It will mess up the
  table system.


Here is the full primer entry for MIRU-VNTR primer 1054:

```
0154  T  TGGACTTGCAGCAATGGACCAAC TACTCGGACGCCGGCTCAAAAT
```

However, the full table header entry for primer 0154
  is `0154.153949.23.154457.2`. Were the `.` seprate out
  first the id, then coordinate and length data.

# How to build a database:

## The format

The first column has the lineage names (can by any
  non-space or tab). The remaing columns are the primer
  names.

Each primer name has 5 items. The primer name, the forward
  primers starting position on the reference, the length
  of the foward primer, the reverse primer ending position
  (first reference base), and the reverse primers length.
  Each of these entrys are separated with an `.`. To be
  honest only the starting and ending positions are used.
  However, I do not want to change the read in function,
  so that is why the lengths are here.

Example of an primer header entry:

- test.300.20.500.18
  - Name: "test"
  - Forward primer starts at base 300
  - Forward primer ends at base 320
    - Forward primer length is 20
  - Reverse primer starts at base 500
  - Reverse primer ends at base 482
    - Reverse primer length is 18

## Add in postion data

You can add in the primer coordinates automatically with
  mkMiruTbl (see its README.md). It is slow, but it works.

## TODO:

Fix valgrind read warnings that happen when NC_015758,
  NZ_LR993210, NZ_KK339080, and NZ_LR993208 are used
  together. Likely unique to NZ_KK339080.
