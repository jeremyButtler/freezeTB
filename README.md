# Use

freezeTb is a program that detects AMR resistance in
  Nanopore sequence tuberculosis samples. It also outputs
  an set of consensus fragments (as an sam file) and
  checks for spoligotypes and MIRU-VNTR lineages. However,
  unlike TBProfiler, freezeTB does not check for
  genes that need to be functional for resistance, nor
  does it output a nice report.

Currently freezeTb uses an WHO 2023 catalog that has had
  all grade 3, 4, and 5 variants removed.

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it separately. I have set the default build
  methods to be static for the modules, so you will need
  to do `make mac` on anything that is not freezeTB. All
  of these programs take sam files as inputs.

List of programs:

- ampDepth: Makes the read depth tsv's that can be
  converted to graphs of mean read depth and coverage
  using graphAmpDepth.r
- trimSam: trims off soft masked bases in an sam file
- tbCon: Builds an consensus (output is sam file)
- tbAmr: AMR detection
- tbMiru: detects MIRU-VNTR lineages in reads
- tbSpoligo: does spoligotyping
  - fasta/fastq is slow, sam is somewhat fast.
- primMask: Masks primers in sequences (sam file) using
  coordinates. This can also do some filtering.
  - You can make an basic mask file file using your
    primers and catPrimers.sh

Other programs:

- filtSam: For filtering and converting sam files. You
  would be better off using samtools. I have used it for
  finding the number of SNPs (includes anonymous bases),
  deletions, and, insertions for each read in an sam file.
  - `minimap2 -a --eqx ref.fa reads.fq | filtSam -sam - -out-stats > read-stats.tsv`
- catPrimers.sh: bash script that uses minimap2 and awk
  to merge amplicons. It also prints out the amplicon and
  primer coordaintes.
- fqGetIds: was here for the original script and has its
  own separate repository. It is designed to extract read
  ids from an fastq file. I want to come back to this and
  see if I can improve it at some point.
- memwater:
  - This is more of an library. I took it from my alnSeq
    side project and fixed a few bugs. It is an waterman
    that operates in linear memory and takes close to as
    much time as an traditional waterman (slow, at least 
    10x slower then the complete striped smith waterman
    library).
  - It is designed for CPUs with branch prediction and so,
    often favors branch less operations.
  - It only returns the scores and the start and end
    coordinates of the alignment.
- oldCode: This has old code I am not quite ready to throw
  away yet. I may use it later or in another project.
  Some of it was never finished, other parts of it no
  longer work do to an change in my coding style.

# Requirements 

1. A read mapper that outputs a sam file.
   - Your choice for the command line version 
   - For the GUI you will need minimap2, which can be
     often installed by your package manager
     - apt for Ubuntu/Debian
     - brew (you have to install this) for Mac
     - source [https://github.com/lh3/minimap2](
               https://github.com/lh3/minimap2)
2. R and several R libraries. If you have R, then the
   make file will detect and then try to install missing R
   libraries when you install freezeTB.
   - For command line version this is only needed with
     the `-graph` options.
     - ggplot2
     - svgLite (if you request svgs)
   - For the GUI version you will need
     - ggplot2
     - tcltk (often is in base R) or tcltk2 (has tooltips)
     - svgLite (Only if you want svgs)
     - fs (Optional)

# Install

The freezeTB install will install freezeTB,
  graphAmpDepth.r, and everything in freezeTBFiles to the
  install location. freezeTb is the program you can run,
  graphAmpDepth.r is the R script to make graphs with, and
  freezeTBFiles are the files, such as the WHO catalog,
  that freezeTb uses for the default files.

## Linux or Mac

Non arm processors (Not an M chip mac). You can skip
  the mkminimap2 step if you already have minimap2.

```
git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make
sudo make install
make clean

# or for an static build (Non-Mac)

# Install minimap2

git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make static
sudo make install
make clean
```

You can try to have make install minimap2 for you by
  replacing `make` with `make mk`.
For ARM processors, such as Mac's M1/M2/M3 chips use
  `make mkneon`.

## More advanced

This installs freezeTb, freezeTbGui.r, and graphAmpDepth.r
  into `/usr/local/bin/`. It also installs the
  freezeTBFiles folder to `/usr/local/share`.

If you are having problems installing try doing a local
  install.

```
make
# or make mk or make mkneon
make PREFIX=~/local/path/ dbPREFIX=~/Documents/ install
```

- PREFIX changes the were freezeTB, freezeTBGui.r, and
  graphAmpDepth.r are installed.
- dbPREFIX changes were the database is installed.
  - The only supported locations are `/usr/local/share/`
    and `~/Documents/`.
  - You can change these by changing defPathStr (global)
     and (local path) in freezeTb.c
     - Header Sec-02: lines 112 to 116. The lines beneath
       it 119 to 121 have the expected file names.
     - For the gui, change the values in Header
       Sec-02 Sub-01 and Header Sec-02 Sub-02 in
       freezeTbGui.r.

## Windows

I have not tried it. I should, but probably never will.

# Run

## CLI

You can print the help message with `freezeTB -h`.

```
minimap -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq > reads.sam;
freezeTB -sam reads.sam -prefix good-name;

# or

minimap -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq | freezeTB.sh -sam - -prefix good-name
```

A more complex way to run freezeTB is to provide the paths
  to all the databases (AMR, MIRU, spoligotyping, and
  gene-table).

```
freezeTB -sam reads.sam -prefix good-name -amr-tbl freezeTBFiles/who-2023.tsv -miru-tbl freezeTBFiles/miruTbl.tsv -gene-coords freezeTBFiles/gene-tbl.tsv -spoligo freezeTBFiles/spoligotype-seq.fa -db-spoligo freezeTBFiles/spoligo-lineages.csv
```

## GUI

For the gui, just run `freezeTBGui.r`. The just fill in
  the values needed. This should be only fastq file if the
  databases were detected.

# How it works

![
  Clip are flow digram showing how freezeTB works. Tempted
  to make an ascii digram with dita.
](diagrams/freezeTb-diagram.svg)

1. freezeTB removes:
   - unmapped reads
   - secondary alignments
   - supplemental alignments
2. Soft masking is trimmed of the ends of each read
3. freezeTB adds each read to a histogram of read depths.
   This is the unfiltered histogram.
4. freezeTB then filters the reads, removing any read that
   has:
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality
   - has a low mean Q-score
   - has a low median Q-score
5. freezeTB then adds the kept reads to a histogram of
   filtered reads
6. freezeTB then checks the read for AMRs
7. freezeTB then checks for any MIRU-VNTR lineages in an
   read (Probably nothing for rapdid kits)
8. freezeTB then adds the read to the reference
   - Bases with Q-scores under 7 are removed
9. After going though all reads; freezeTB then prints
   out both histogram (unfiltered and filtered)
10. freezeTB prints out the AMRs that at least 5% of
   mapped reads supported
11. freezeTB Then prints out the MIRU-VNTR lineage table
    of read counts for the reads
12. freezeTB then collapses the consensus
    - Split into fragments with 20x or greater read depth
    - Fragments with under 200x read depth are removed
    - Most supported snp/match/deletion selected
    - Snps/matches with less than 50% support are masked
    - Most supported insertion selected
    - Indels wit less than 70% support are removed
13. freezeTB finds the AMRs for the consensus
14. freezeTB finds the MIRU-VNTR lineages for consensus
15. freezeTB finds spoligotype for consensus
16. freezeTB then makes the read depth and coverage graphs
    with R (only if you used -graph or -graph-ext "ext")
