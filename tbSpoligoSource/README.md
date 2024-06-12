# Use:

Detects tuberculosis spoligotypes in an sequence or set of
  reads.

# License:

This is under an dual licenes. The first license is public
  domain, however, not all compinies/countries like public
  domain. In those cases or any other case were public
  domain is inconveinent this is under the MIT license.

# Install:

## Linux (static):

```
make
sudo make install
```

## Mac (or non-static):

```
make mac
sudo make install
```

# Usage:

## descrption:

tbSpoligo is designed to work on fastq (do not use
  fastq.gz), fasta (do not use fasta.gz), and sam files.
  It can also be combined with the lineage database from
  TBProfiler to call lineages when not checking fragments.

tbSpoligo has several modes, which can change the results.

1. fragment modes (-read-frag and -con-frag)
   assume that you are working with fragments of the
   direct repeat region instead of an full direct repeat
   region.
   - -read-frag will print out the barcode with all the
      number of reads that mapped to each spacer. It will
      not print out any lineage information.
   - -con-frag will print out the lineage information like
     normal, but does not require any sequence to have the
     full direct repeat reagion. This will not print out
     counts since it is assuming an consensus is input.
2. speed (-slow and -fast) determines the alogorithim used
   to find if an spacer was detected
   - -slow uses an waterman smith to detect each spacer.
     It should be more sensative, but is also much slower
   - -fast compares the number of kmers in an window of
     50 bases to the spacer sequences. If there are at
     least 10 kmers, then an waterman smith alignment is
     done on the spacer and the 50 base pair window. After
     checking for all spacers the window shifts by 25
     bases.
3. The file type (fastx or sam)
   - for -fa (fasta) and -fq (fastq) files the entire
     sequence is searched for spacers. This is slower, but
     less likely to miss something
   - for -sam only the region mapping to the direct repeat
     (coordinates from -dr-start and -dr-end) are searched
     for the direct repeat region. This allows for faster
     checking.

## quick guide:

### sam file

```
minimap -a ref.fa reads.fastq > reads.sam;
tbSpolgio -spoligo ../freezeTBFiles/spoligotype-seq.fa -sam reads.sam > spoligotypes.tsv;
```

### fasta file

```
tbSpolgio -spoligo ../freezeTBFiles/spoligotype-seq.fa -fa consensus.fasta > spoligotypes.tsv;
```

### fastq file

```
tbSpolgio -spoligo ../freezeTBFiles/spoligotype-seq.fa -fq reads.fastq > spoligotypes.tsv;
```

### with an lineage database (sam file method)

```
minimap -a ref.fa reads.fastq > reads.sam;
tbSpolgio -db ../freezeTBFiles/spoligo-lineages.csv -spoligo ../freezeTBFiles/spoligotype-seq.fa -sam reads.sam > spoligotypes.tsv;
```

### fragment mode (sam file method)

```
minimap -a ref.fa reads.fastq > reads.sam;
tbSpolgio -read-frag -db ../freezeTBFiles/spoligo-lineages.csv -spoligo ../freezeTBFiles/spoligotype-seq.fa -sam reads.sam > spoligotypes.tsv;
```

### consensus fragment mode (sam file method)

```
minimap -a ref.fa reads.fastq > reads.sam;
tbSpolgio -read-frag -db ../freezeTBFiles/spoligo-lineages.csv -spoligo ../freezeTBFiles/spoligotype-seq.fa -sam reads.sam > spoligotypes.tsv;
```
