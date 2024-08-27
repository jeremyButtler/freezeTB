# Use:

tbAmr is designed to detect AMR mutations in Nanopore
  sequence reads or consensuses.

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

# Run:

To run tbAmr you will need to have an database of AMRs
  in the tbAmr format. A database for the 2023 WHO TB
  AMR catalog with only grade 1/2 AMRs is include with
  freezeTB (../freezeTBFiles/who-2023.tsv). However, if
  you wish to make your catalog based off the WHO 2023
  catalog or 2021 catalog you can install and use
  whoToTbAmr in cnvt_whoToTbAmr_src. The other option is
  to read the documentation for the tbAmr format (see
  cnvt_whoToTbAmr README.md). This would be painful, but
  could work.

The reads/consensuses should be in a sam file format. So,
  you will need an read mapper, such as minimap2 to
  prepare your fastq/fasta files for tbAmr.

The help message can be printed with `tbAmr -h`.

The version number (date) can be printed with `tbAmr -v`.

## Running reads

For raw reads my filtering settings are quite low and are
  set up to catch noise. My logic is that the user is
  more likely to use read checking as part of an pipeline
  to detect mixed infections If you plan to not check
  consensuses, then I would recommend setting
  the `-min-amr-map-perc` (percentage of reads covering
  the AMR variant that supported the AMR variant) to at
  least 0.05 (5%).

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq > tmp.sam;
tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam tmp.sam;
```

or

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam -;
```

## Running consensuses

Make sure you use the -sam-con entry for consensus.
  Otherwise tbAmr will apply read depth filters.

```
minimap2 -a ../freezeTBFiles/NC000962.fa consensus.fasta > tmp.sam;
tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam-con tmp.sam;
```

or

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam-con -;
```

# Some extra commands:

## Printing read ids for AMR sequences

You can print out the read ids and the variants they
  mapped to using `-id-file file.tsv`. Each line will have
  an read id and one variant that it mapped to. Multiple
  lines are used for read ids that mapped to multple
  variants. You can use fqGetIds or seqkit extract the
  fastq entries using the read ids.

## Frameshift checking (not recommended)

You can apply frameshift checking using `-frameshift`.
  This checks for early stop codons, missing stop codons
  (if have gene end), and missing start codons (if have
  start of gene). It also checks if the reading
  frame offset caused by indels (deletions - insertions)
  is not a multiple of three (frame shift) (crude, but
  works and is quick). The indel count is applied on the
  gene and the AMR region (part of gene causing an AMR).

One consenquence of the indel counting is that a single
  positive indel can result in an gene knock out. So,
  reads, which have a lot of false positive indes, are
  going to make a lot of false positives.
