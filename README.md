# Use

freezeTb is designed to detect AMR resistance in Nanopore
  sequence TB samples. Its goal is to be easy to install,
  and easy to use. However, it may not give the same
  quality of results as TBProfile.

One reason why not is that freezeTb assumes that Nanopore
  reads and consensuses are to noisy and likely have
  errors that cause frameshifts. So, freezeTb will only
  treat frame shits as regular nucleotide sequences and
  not compare their amino acid sequences.

Currently freezeTb uses the WHO 2023 catalog.

I have made the programs in freezeTb to be modular. So,
  they can be built on there own.

List of programs:

- ampDepth: Makes the read depth tsv's used by
  graphAmpDepth.r
- trimSam: trims off soft masked bases
- tbCon: Builds a consensus
- tbAmr: is the AMR detection program

Other programs:

- rmHomo: never was completed.
- filtSam: not useful, but I wanted to see how close I
  could get to samtools view. It is slower and has no
  help message.
- fqGetIds: was here for the original script. It has its
  own separate repository.

# Requirements 

1. A read mapper that outputs a sam file, minimap2 is what
   I use [https://github.com/lh3/minimap2](
          https://github.com/lh3/minimap2)
2. For graphs. This only applies if you use the `-graph`
   or `-graph-ext` options.
   - Rscript (this is R). You will also need some R
     packages
     - ggplot2
     - viridisLite
     - svgLite
     - data.table

# Install

The freezeTb install will install freezeTb,
  graphAmpDepth.r, and everything in freezeTbFiles to the
  install location. freezeTb is the program you can run,
  graphAmpDepth.r is the R script to make graphs with, and
  freezeTbFiles are the files, such as the WHO catalog,
  that freezeTb uses for the default files.

## Linux

```
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make
sudo make install
make clean
```

## Mac

This  should work, but I am not sure. You will need to
  install the dependencies by homebrew.

```
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make mac
sudo make install
make clean
```

y

I am going to say no. I have not tried it.

However, in theory this should be possible, since I have
  only use functions from the C standard libraries, so
  there should be nothing unique to Linux. However, Widows
  does not come with a C compiler (you have to install
  it). Windows also does not support awk, which I use to
  set my default file paths in my Makefile. To set the
  default file path change
  line 108 `char *defPathStr = "";` to
  `char *defPathStr = "C:\\default\file\path";`. You will
  also likely have a lot of compiler warnings about unsafe
  functions, which are considered standard C functions.

# Run

You can print the help message with `freezeTb -h`.

To make running this program simple, I have put the needed
  databases in the same location as freezeTb. However,
  this location is hardcoded in at compile time and so,
  you need to run `sudo make install` to get the simple
  method.

```
minimap -a /usr/local/bin/freezeTbFiles/TB-NC000962.fa reads.fastq > reads.sam;
freezeTb -sam reads.sam -prefix good-name;

# or

minimap -a /usr/local/bin/freezeTbFiles/TB-NC000962.fa reads.fastq | freezeTB.sh -sam - -prefix good-name
```

Do not use the `--eqx` flag for minimap2. For some odd
  reason tbCon has an issue with the --eqx flag.
  **I will need to come back and figure out why.**

Also tbAmr does not work very well with stdin input, but
  freezeTb does. **I will need to come back and debug this
  problem.**


A more complex way to run freezeTb is to provide the paths
  to the AMR database and the gene coordinates.

```
freezeTb -sam reads.sam -prefix good-name -amr-tbl freezeTbFILES/who-2023.tsv -gene-coords freezeTbFiles/TB-gene-coordinates.paf
```

# How it works

1. freezeTb removes:
   - unmapped reads
   - secondary alignments
   - supplemental alignments
2. freezeTb adds each read to a histogram of read depths.
   This is the unfiltered histogram.
3. freezeTb then filters the reads, removing any read that
   has:
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality
   - has a low mean Q-score
   - has a low median Q-score
4. freezeTb then adds the kept reads to a histogram of
   filtered reads
5. freezeTb then checks the read for AMRs
6. freezeTb then adds the read to the reference
7. After going though all reads; freezeTb then prints
   out both histogram (unfiltered and filtered)
8. freezeTb prints out the AMRs that at least 45% of
   mapped reads supported
9. freezeTb then finishes building the consensus and
   then finds the AMRs for the consensus.
10. freezeTb then makes the read depth and coverage graphs
    with R (only if you used -graph or -graph-ext "ext")
