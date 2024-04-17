# Use

freezeTb is a program that detects AMR resistance in
  Nanopore sequence tuberculosis samples. It does not have
  all the features that TBProfiler has.

Currently freezeTb uses the WHO 2023 catalog.

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it separatley. I have set the default build
  methods to be static for the modules, so you will need
  to do `make mac` on anything that is not freezeTb. All
  of these programs take sam files as inputs.

List of programs:

- ampDepth: Makes the read depth tsv's used by
  graphAmpDepth.r
- trimSam: trims off soft masked bases
- tbCon: Builds a consensus
- tbAmr: is the AMR detection program
- tbMiru: detects MIRU lineages in reads
  - See tbMiru's read me for building your own MIRU table

Other programs:

- filtSam: For filtering and converting sam files. You
  would be better off using samtools. I have used it for
  finding the number of SNPs, deletions, and, insertions
  for each read in an sam file with
  - `minimap2 -a --eqx ref.fa reads.fq | filtSam -sam - -out-stats > read-stats.tsv`
- fqGetIds: was here for the original script. It has its
  own separate repository. It is designed to extact read
  ids from an fastq file. I need to come back to this and
  improve it.
- oldCode: This has old code I am not quite ready to throw
  away yet. I may use it later or in another project.
  Some of it was never finished, other parts of it no
  longer work do to an change in my coding style.

# Requirements 

1. A read mapper that outputs a sam file.
   - Your choice for the command line version 
   - For the GUI you will need minimap2
     [https://github.com/lh3/minimap2](
      https://github.com/lh3/minimap2)
   - Installed with make file (git is needed)
2. R and several R libraries. The R libraries will be
   installed when you install freezeTb.
   - For command line version this is only needed with
     the `-graph` options.
     - ggplot2
     - svgLite (if you request svgs)
   - For the GUI version you will need
     - ggplot2
     - fs
     - tcltk (often is in base R) or tcltk2
     - svgLite (if you request svgs)

# Install

The freezeTb install will install freezeTb,
  graphAmpDepth.r, and everything in freezeTbFiles to the
  install location. freezeTb is the program you can run,
  graphAmpDepth.r is the R script to make graphs with, and
  freezeTbFiles are the files, such as the WHO catalog,
  that freezeTb uses for the default files.

## Linux or Mac

Non arm processors (Not an M chip mac). You can skip
  the mkminimap2 step if you already have minimap2.

```
git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make
make mkminimap2
sudo make install
make clean

# or for an static build (Non-Mac)

git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make static
make mkminimap2
sudo make install
make clean
```

Mchip macs

```
git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make mchipmac
make mkNeonMinimap2
sudo make install
make clean
```

This installs freezeTb, freezeTbGui.r, and graphAmpDepth.r
  into `/usr/local/bin/`. It also installs the
  freezeTbFiles folder to `/usr/local/share`.


If you are having problems installing try doing a local
  install.

```
make
make PREFIX=~/local/path/ dbPREFIX=~/Documents/ install
```

The PREFIX changes the were freezeTb, freezeTbGui.r, and
  graphAmpDepth.r are installed. While dbPREFIX changes
  were the database is installed. The only supported
  locations are `/usr/local/share/` and `~/Documents/`.
  You can change these by changing defPathStr
  (`/usr/local/share/`; global path) and defAltPathStr
  (`~/Documents/`; local path) in freezeTb.c
  (Header Sec-02: lines 112 to 116). The lines beneath it
  (119 to 121) have the expected file names. For the 
  gui, change the values in Header Sec-02 Sub-01 and
  Header Sec-02 Sub-02 in freezeTbGui.r.

## Windows

I have not tried it.

# Run

## CLI

You can print the help message with `freezeTb -h`.

To make running this program simple, I have put the needed
  databases in `/usr/local/share`.

```
minimap -a /usr/local/share/freezeTbFiles/NC000962.fa reads.fastq > reads.sam;
freezeTb -sam reads.sam -prefix good-name;

# or

minimap -a /usr/local/share/freezeTbFiles/NC000962.fa reads.fastq | freezeTb.sh -sam - -prefix good-name
```

A more complex way to run freezeTb is to provide the paths
  to all the databases (AMR, MIRU, and gene-table). These
  are just tsvs, so they are easy to edit.

```
freezeTb -sam reads.sam -prefix good-name -amr-tbl freezeTbFiles/who-2023.tsv -miru-tbl freezeTbFiles/miruTbl.tsv -gene-coords freezeTbFiles/TB-gene-coordinates.paf
```

## GUI

For the gui, just run `freezeTbGui.r`. The just fill in
  the values needed. This should be only fastq file if the
  databases were detected.

# How it works

![
  Clip are flow digram showing how freezeTb works. Tempted
  to make an ascii digram with dita.
](diagrams/freezeTb-diagram.svg)

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
6. freezeTb then checks for any MIRU lineages in an read
7. freezeTb then adds the read to the reference
8. After going though all reads; freezeTb then prints
   out both histogram (unfiltered and filtered)
9. freezeTb prints out the AMRs that at least 45% of
   mapped reads supported
10. freezeTb THen prints out the MIRU lineage table of
    read counts for the reads
11. freezeTb then finishes building the consensus
12. freezeTb finds the AMRs for the consensus
13. freezeTb finds the MIRU lineages for the consensus
14. freezeTb then makes the read depth and coverage graphs
    with R (only if you used -graph or -graph-ext "ext")
