# Use

freezeTB is designed to detect amr resistant tb strains
   using Nanopore reads.

Consensuses are built for genes that have enough mapped
  reads (at least 20) using ivar. Subsampling by mapping
  quality and median q-score is done when a gene has more
  than 300 mapped reads.

Currently this uses the WHO 2021 catalog.

# Requirements

For mac, you can install some of the dependencies by
  homebrew. Instructions for installing homebrew can be
  found at [https://brew.sh/](https://brew.sh/).

1. minimap2 [https://github.com/lh3/minimap2](
   https://github.com/lh3/minimap2)
2. samtools [https://github.com/samtools/samtools](
   https://github.com/samtools/samtools)
   - For Debian/Unbuntu "sudo apt-get install samtools"
   - For mac you can install samtools with homebrew
     `brew install samtools`
3. ivar [https://github.com/andersen-lab/ivar](
   https://github.com/andersen-lab/ivar)
   - For mac, you can install ivar with homebrew. I can
     not remmeber the full name. Do `brew search ivar` and
     then `brew install` on the found package.
4. R with ggplot2, viridisLite, svgLite, and data.table
   - use "sudo make R" to run an install script for the
     libraries
5. Basic unix tools, like gawk (awk on linux)
   - For MAC this will require you to install gawk through
     homebrew `brew install gawk`

# Install

Install ivar, minimap2, samtools, and the R packages you
  need. Then do `sudo make install` for linux or 
  `make mac` and the `sudo make install` for mac.

## Linux

```
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
sudo make install
```

## Mac

This  should work, but I am not sure. You will need to
  install the dependencies by homebrew.

```
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make mac
sudo make install
```

# Run

Simplest way (needed file are at install locations):

```
freezeTB.sh -fastq reads.fastq
```

# How it works

1. Map reads to a set of genes (TB by default)
2. Get number of primary/secondary mappings
   - This also outputs a prefix-supTbl.tsv which as each
     row start with a primary alignment. The alignments
     after are additional genes that the same reads mapped
     to. These could be duplicate genes or reads that
     spanned multiple genes.
3. Build graph of number of read mappings per gene in R.
4. Split reads up into mapped genes
5. Map each set of reads to the TB (default) reference
6. Filter and if needed subsample reads
   - I have set this up for primers, were we often have
     many high quality reads of the desired length. So, I
     only use read length to check if reads should be
     kept. Read length is not used for subsampling.
   - Subsampling using mapq and median q-score.
     The scoring is "mapq * 4 + median Q-score".
   - This was just a quick way to give some priority
     to the mapping quality.
7. Find the largest region the set of reads could cover
   in the TB (default) genome. Then extract the largest
   region.
8. Build a consensus for the subsampled reads using the
   extracted largest region from the TB referenced.
9. Use the consensuses to find the amr resistant genes
   using tbAmr
