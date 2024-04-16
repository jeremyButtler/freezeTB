# Use:

tbAmr is designed to detect AMR mutations in Nanopore
  sequence reads or consensuses.

# Install;

## Linux:

```
make;
sudo make install;
```

## Mac:

```
make mac;
sudo make install;
```

# Run:

To run tbAmr you will need to have an database of AMRs
  in the tbAmr format. A database for the 2023 WHO TB
  AMR catalog with only grade 1/2 AMRs is include with
  freezeTb (../freezeTbFiles/who-2023.tsv). However, if
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
  least 0.45 (45%).

```
minimap2 -a ../freezeTbFiles/TB-NC000962.fa reads.fastq > tmp.sam;
tbAmr -amr-tbl ../freezeTbFiles/who-2023.tsv -sam tmp.sam;
```

or

```
minimap2 -a ../freezeTbFiles/TB-NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTbFiles/who-2023.tsv -sam -;
```

## Running consensuses

Make sure you use the -sam-con entry for consensus.
  Otherwise tbAmr will apply read depth filters.

```
minimap2 -a ../freezeTbFiles/TB-NC000962.fa consensus.fasta > tmp.sam;
tbAmr -amr-tbl ../freezeTbFiles/who-2023.tsv -sam-con tmp.sam;
```

or

```
minimap2 -a ../freezeTbFiles/TB-NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTbFiles/who-2023.tsv -sam-con -;
```

# Some extra commands:

## Printing read ids for AMR sequences

You can print out the read ids that mapped to each unique
  variant by providing a file name with `-id-file`. You
  can use seqkit or my fqGetIds to extract the sequences
  from an fastq file using the read ids.

## Frameshift checking (not recommended)

You can apply frameshift checking using `-frameshift`.
  This checks to see if there was a non-multiple of three
  number of deletions in an gene. For complete genes it
  will also check for missing starts, missing stops, or
  early stops in an genes. If these are found it will
  flag resistance if the database has an entire gene
  LoF entry for the gene.

For regular frameshifts (not entire gene) it will also
  check to see if there is an indel. If so, it will report
  it. This means that there will be a lot of false
  positives for reads.

Given that Nanopore is less likely, but still has errors
  as indel's in homopolymers, I would suggest not using
  `-frameshift`.
