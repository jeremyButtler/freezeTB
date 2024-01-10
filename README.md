# Use

ampMapCounts is designed to map Nanopore sequenced
   amplicons to a set of target genes. The number of
   mapped reads per gene is recored and the mapped reads
   are then split up into individual files by the gene
   they primarly mapped to. A graph (prefix-mapGraph.svg)
   is made to show how many reads mapped.

Consensuses are built for genes that have enough mapped
  reads (at least 20) using ivar. Subsampling by mapping
  quality and median q-score is done when a gene has more
  than 300 mapped reads.

# Requirements

1. minimap2 [https://github.com/lh3/minimap2](https://github.com/lh3/minimap2)
2. ivar [https://github.com/andersen-lab/ivar](https://github.com/andersen-lab/ivar)
3. samtools [https://github.com/samtools/samtools](https://github.com/samtools/samtools)
   - For Debian/Unbuntu "sudo apt-get install samtools"
4. R with ggplot2, viridisLite, and svgLite
   - use "sudo make R" to run an install script for the libraries
4. Basic unix tools, like awk

# Install

Install ivar, minimap2, samtools, and the R packages you
  need. Then do "make" and "sudo make install".

# Run

```
buildAmpCons.sh -ref ref.fasta -genes genes.fasta -fastq reads.fastq
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
     