# Use:

ampDepth gets the rough location of an amplicon, the genes
  in an amplicon, and the mean, maximum, and minimum depth
  for amplicons. The graphAmpDepth.r script can be used to
  visualize the depths for each gene(not positions). It
  also puts out a graph showing the coverage for each
  gene.

# Install:

If Rscript is not in `/usr/bin/`, then set the top line
  (`#!/usr/Rscript`) of graphAmpMap.r to the correct
  location. On a Unix OS you can find this by
  using `find / -name Rscript 2>/dev/null | grep -v "lib"`

```
sudo make install; # Run make if you have issues

# or for mac (should work)
make mac;
sudo make install;
```

The binary for ampDepth is compliled on void linux with
   musl. So, it should be smaller in size then other linux
   distributions. It is compiled statically, so linux
   distrubution should not be a concern.

# Running:

You will need a paf file with the reference gene
  coordinates to run this code. This can be gotten by
  mapping the genes (can download from genbank) to the
  reference genome
  with `minimap2 ref.fasta genes.fasta > out.paf`. You
  will then need to map your reads to the same reference
  and make a sam file
  `minimap2 -a -x map-ont ref.fasta reads.fastq > out.sam`

The first column in the output tsv file is a flag which
  separates out runs in graphAmpDepth.r. You can change
  this with the `-flag "newFlag"` argument.

Print the help message for ampDepth with `ampDepth -h`.

Print the help message for graphAmpDepth with
  `graphAmpDepth -h`.

```
# Make the paf file
minimap2 ref.fasta genes.fasta > out.paf;

# Map the reads to the reference
minimap2 -a -x map-ont ref.fasta reads.fastq > out.sam;

# Get the tsv file read read stats

## Minimum
ampDepth -paf out.paf -sam out.sam -out out.tsv;

## Change the label in column one to ONT
ampDepth -flag ONT -paf out.paf -sam out.sam -out out.tsv;

# Get the graphs

graphAmpDepth out.tsv;
```

You can grab the row of a gene from the tsv made by
  ampDepth by using `grep "geneName" out.tsv`.

# Dummy figure

![Read depth figure made by graphAmpDepth.r](
  test-readDepth.svg)

![Ampilcon gene coverage figure made by graphAmpDepth.r](
  test-ampMap.svg)

| flag | ampNumber | refStart | refEnd | ampStart | ampEnd | avgAmpDepth | minAmpDepth | maxAmpDepth | geneId       | refGeneStart | refGeneEnd | firstBaseDepth | lastBaseDepth | avgDepth | minDepth | maxDepth |
|:----:|:----------|:---------|:-------|:---------|:-------|:------------|:------------|:------------|:------------:|:-------------|:-----------|:---------------|:--------------|:---------|:---------|:---------|
| ONT  |    0      |   5241   |  9809  |  6503    | 7744   | 316         | 21          | 386         | gyrB         | 5241         | 7261       | 85             | 323           | 307      | 85       | 369      |
| ONT  |    0      |   5241   |  9809  |  6503    | 7744   | 316         | 21          | 386         | gyrA         | 7306         | 9809       | 359            | 21            | 331      | 21       | 386      |
| ONT  |    1      |   490782 |  493857|  490782  | 491864 | 164         | 26          | 212         | fgd1         | 490782       | 491791     | 70             | 156           | 166      | 70       | 212      |
| ONT  |    1      |   490782 |  493857|  490782  | 491864 | 164         | 26          | 212         | pta          | 491790       | 493857     | 156            | 26            | 125      | 26       | 156      |
| ONT  |    2      |   528609 |  530223|  528748  | 529674 | 241         | 25          | 288         | groEL2       | 528609       | 530223     | 78             | 25            | 241      | 25       | 288      |
| ONT  |    25     |   NA     |  NA    |  NA      | NA     | 0           | 0           | 0           | x-off-target | NA           | NA         | 0              | 0             | 0        | 0        | 0        |
| ONT  |    26     |   NA     |  NA    |  NA      | NA     | 6075        | 6075        | 6075        | z-unmapped   | NA           | NA         | 6075           | 6075          | 6075     | 6075     | 6075     |

Table: Example of three rows from the tsv output from
  ampDepth
