# Use:

ampDepth gets the rough location of an amplicon, the genes
  in an amplicon, and the mean, maximum, and minimum depth
  for amplicons. The graphAmpDepth.r script can be used to
  visualize the depths for each gene(not positions). It
  also puts out a graph showing the coverage for each
  gene.

# Install:

```
sudo make install; # Run make if you have issues

# or for mac (should work)
make mac;
sudo make install;
```

Put the graphAmpDepth.r script were you want to run it.

The binary for ampDepth is compliled on void linux with
   musl. So, it should be smaller in size then other linux
   distributions. It is compiled statically, so it should
   work on all linux distrubutions.

# Running:

You will need a tsv file with the reference gene
  coordinates for each gene to run this code. I have
  included the gene coordinates for NC000962.3.fa in
  "genes-tbl.tsv". If you want to use a different
  reference, then make a file in the same format as
  "genes-tbl.tsv" (see help message for format).

Print the help message for ampDepth with `ampDepth -h`.

Print the help message for graphAmpDepth with
  `graphAmpDepth -h`.

```
# Map the reads to the reference
minimap2 -a -x map-ont ref.fasta reads.fastq > out.sam;

## Minimum
ampDepth -gene-tbl gene-tbl.tsv -sam out.sam -out out.tsv;

## Change the label in column one to ONT
ampDepth -flag Filtered -gene-tbl gene-tbl.tsv -sam out.sam -out out.tsv;

# Build the graphs
graphAmpDepth.r -stats out.tsv -who ../freezeTbFiles/who-2023.tsv;
```

You can grab the row of a gene from the tsv made by
  ampDepth by using `grep "geneName" out.tsv`.

# Dummy figure

![Read depth figure made by graphAmpDepth.r](
  demo-readDepth.svg)

![Ampilcon gene coverage figure made by graphAmpDepth.r](
  demo-ampMap.svg)

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
