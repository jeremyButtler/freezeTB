# Use:

List the configuration file options for the raylib
  freezeTB GUI.

- [Databases](#databases)
- [AMR settings for freezeTB](#amr)
- [Read filtering](#filtering)
- [Lineage settings](#lineage)
- [Consensus settings](#consensus)
- [Variant file for consensus](#variants)
- [Clustering settings](#clustering)
- [Other settings that are not very useful](#other)

# Databases

## Database you can change

`-gene-coords coordinates.tsv`: changes the genes
  searching for. The first row is ignored, since it is
  assuemd to be a header. The second row and onwards has
  the genes. The format is a tsv file, were spaces or tabs
  separate columns.

| column | value                                         |
|:-------|:----------------------------------------------|
| one    | gene name                                     |
| two    | reference name                                |
| three  | gene direction; `+` is forward; `-` is reverse|
| four   | start of gene in the reference genome         |
| five   | end of gene in the reference genome           |

Table: table showing the values in each column of the gene
  coordinates file

You can have more then five columns, but only the first
  five columns are read in.

Example of a coordinates file:

```
gene  reference    dir  start    end
atpE  NC_000962.3  +    1461045  1461290
ddn   NC_000962.3  +    3986844  3987299
```

`-maks-prim primer-coordinates.tsv`: this file has the
  primer or other coordinates to mask. The format is the
  same as `-gene-coords`, except there are two extra
  columns at the end for gene direction. It is a
  tsv file, were spaces or tabs separate columns.

| column | value                                       |
|:-------|:--------------------------------------------|
| one    | gene name (not used)                        |
| two    | reference name (not used)                   |
| three  | direction (not used)                        |
| four   | start of forward primer in reference genome |
| five   | end of forward primer in reference genome   |
| six    | start of reverse primer in reference genome |
| seven  | end of reverse primer in reference genome   |

Example of a primer masking file:

```
gene         reference    dir  forward_start forward_end reverse_start reverse_end
atpE_primer  NC_000962.3  +    1461045       1461065     1461270       1461290
ddn_primer   NC_000962.3  +    3986844       3986864     3987279       3987299
```

`-hsp65-simple hsp65-simple-database.tsv`: this is the
  simple database used for getLin. For more details, see
  the instructions for building a getLin lineage database
  in the getLinSrc directory of my bioTools repository.

`-hsp65-complex hsp65-complex-database.tsv`: this is the
  complex database used for getLin. For more details, see
  the instructions for building a getLin lineage database
  in the getLinSrc directory of my bioTools repository.

`-db-spoligo spoligotype-database.csv`: this is the
  spoligotype lineage database from tbProfiler. It is a
  csv file.

# Database

`-amr-tbl database.tsv`: changes the AMR database used
  with freezeTB. This is a somewhat complex database, so
  I would not suggest messing with it unless you know what
  you are doing. The best way is to use addAmr from
  freezeTB (in programs/addAmrSrc).

## Database you should not change

`-spoligo spacers.fasta`: is a fasta file with the
  spoligotype spacer sequences.The default database has
  all spaces, so no need to change this.

`-miru-tbl table.tsv`: This has the coordinates and
  lineages for MIRU-VNTR lineages. This has all lineages
  an primers, so there is not reason to change this.

`-ref reference.fasta`: reference to use with freezeTB.
  Everything is set up for H37Rv (NC000962), so do not
  change this.

# AMR settings

These settings change how sensitive freezeTB is for
  calling AMRs in reads.

`-min-depth 10`: minimum depth needed to keep an AMR. This
  setting applies to the consensus and all other minimum
  depth settings as well.

`-min-amr-amp-perc 0.1`: minimum percentage of mapped
  reads to keep an AMR caused by an SNP . Valid input is
  0 to 1.

`-amr-indel-sup 0.7`: minimum percentage of mapped reads
  needed to keep an AMR cause by an indel (insertion or
  deletion). Valid values are 0 to 1.

`-frameshift`: turn on frame shift detection for the AMR
  detection step. This is on by default and is applied to
  both the consensus and reads.

`-no-frameshift`: turn off frame shift detection for the
  AMR detection step. This setting is applied to both the
  consensus and reads.

`-frameshift-sup 0.70`: minimum percent of reads needed to
  keep an AMR caused by a frame shift. Valid input is 0
  to 1.

# Read filtering

These settings change what reads to remove. FreezeTB will
  always remove all unmapped reads, secondary alignments,
  and supplemental alignments.

`-min-mapq 15`: minimum mapping quality needed to keep a
  read. This setting changes with different read mappers.
  The default (15) is setup for minimap2. This setting is
  ignored for the internal read mapper.

`-min-len 50`: minimum read length. This length does not
  included the softmasked bases.

`-min-mean-q 7`: minimum mean read quality score to keep a
  read. This mean does not agree with ONT's mean since it
  does not convert the score to the percent error and then
  back. It is often to optimistic.

`-min-meanian-q 7`: minimum median read quality score to
  keep a read. This median has the same issue as the
  `-min-mean-q`.

# Lineage

The lineage systems in freezeTB that you can change
  parameters for are the spoligotyping and the
  MIRU-VNTR lineages. The getLin system used for finding
  species with the hsp65 gene does not use flags. Instead
  the databases have all the settings.

## lineage variables you can change

`-fudge 15`: Amount to fudge by (length can be off by) in
  a MIRU-VNTR lineage. For tuberculosis, 15 allows some
  room, but also does not let any lineage to overlap.

`-spoligo-min-score 0.9`: minimum score needed to count
  a spoligotype spacer as mapped (for the waterman
  alignment step). Valid values are 0 to 1.

## lineage variables you should not change

`-dr-start 3119037`: first base of the direct repeat
  region in the reference genome. This is set for H37Rv
  (NC000962). Do not change this.

`-dr-end 3123624`: last base of the direct repeat
  region in the reference genome. This is set for H37Rv
  (NC000962). Do not change this.

# Consensus

These are the settings to change the consensus output. The
  consensus does not handle insertions well, so I suggest
  that you just ignore the consensus. Especially for
  MIRU-VNTR lineages, which are based on insertions.

`-min-depth 10`: minimum read depth to not mask a base.
  This setting also changes the minimum read depth for
  AMRs.

`-min-len 50`: minimum length to print out a consensus.
  This setting also changes the minimum length for read
  filtering.

`-min-q 3`: minimum quality score need for a base to keep
  a match or SNP.

`-min-q-ins 0`: minimum quality score need for a base to
  keep an insertion.

`-perc-snp-sup 0.5`: minimum percent support of bases
  needed to not mask a match or SNP.

`-perc-ins-sup 0.5`: minimum percent support of bases
  needed to not remove an insertion from the consensus.
  The insertion method requires exact matches, this means
  that large insertions will often never get enough
  support.
 
`-perc-del-sup 0.5`: minimum percent support of bases
  needed to not mask a deletion.

# Variants

The variant file shows the possible variants that were in
  the reads for each position. It is a tsv file with a
  single header, it is not a vcf file. These settings
  change the sensitivity for the variants that are.

`-p-min-depth 10`: minimum read depth need to print a
  variant.

`-p-perc-snp-sup 0.1`: minimum percent supported needed to
  print a SNP variant. Valid values are from 0 to 1.

`-p-perc-ins-sup 0.1`: minimum percent supported needed to
  print an insertion variant. Valid values are from 0 to 1.

`-p-perc-del-sup 0.1`: minimum percent supported needed to
  print an deletion variant. Valid values are from 0 to 1.

# Clustering

The clustering method in freezeTB is slow and is not very
  sensitive. So, I suggest you do not use this. It will
  also replace the consensus method for building a
  consensus. It uses the same consensus builder, but makes
  sure the reads are clustered together first. It can
  handle large insertions somewhat better, but only
  because it will group large insertions into separate
  clusters first and then build the consensus.

`-clust`: do clustering to build the consensus. This is
  off.

`-no-clust`: disable and use the default consensus method.

`-len-weight 2`: this is used in finding the score for
  each read being chosen first. It sets how much read
  length should be used. For example, 2 means the length
  score should be doubled. Decimal values are valid.

The read scoring formula is

```
floor[weight * floor[log10(length)] + floor[median quality score]
```

Were; weight is the value provied by `-len-weight`.

`-clust-depth 20`: this is the mean read depth needed to
  keep a single cluster.

`-clust-perc-depth 0.001`: minimum percent read depth
  needed to keep a cluster. This is lower, because with
  higher settings, it is rare for clusters to be kept.
  Valid values are from 0 to 1.

`-read-err 0.046`: this is the assumed error rate for
  reads. It is used to determine if a reads is the same
  as a consensus. Valid values are 0 to 1.

`-con-err 0.023`: this is the assumed error rate for the
  consensus. It is used to find if consensus are the same
  (just error) or not. Valid vales are 0 to 1.

`-err-to-var 100`: maximum error ration to consider a read
  or as part of a cluster or decide if two consensus are
  the same. The error rate in this comes from
  the `-read-err` and `-con-err` inputs.

Formula for `-err-to-var`:

```
100 * (number differences) / (error rate * length)
```

`-con-sim 0.99`: maximum percent similarity to consider to
  consensus difference. Otherwise, they are the same and
  both consensus clusters are merged. Valid values are 0
  to 1.

`-overlap 0.75`: minimum percent overlap needed between
  two clusters consensuses needed to consider merging two
  clusters. Valid values are 0 to 1.

`-perc-n 0.05`: maximum percent of masked bases allowed
  in a consensus. Valid values are 0 to 1.

`-con-iter 1`: number of times to re-find the cluster and
  then rebuild the consensus. For each value past 0, a
  consensus is built, then it is used to rebuild its
  cluster. From my experience with the consensus method
  used here, you will one get improvements for one
  re-build.

`-depth-prof`: this adds in a profile building step for
  reads that removes low depth variants. By default this
  is disabled, since it does not improve results.

`-no-depth-prof`: disable `-depth-prof`

`-win-len 500`: for larger reads, error rates are compared
  across the whole read and in windows. If either the
  whole read or window has an error rate hight then the
  same, then the read is discarded. The default size of
  one window is 500 bases.

`-win-err 200`: maximum error allowed in one window. This
  is the same as `-err-to-var`, except it is applied to
  a window.

`-indel-len 10`: minimum indel (insertion or deletion)
  length needed to count two reads, clusters, or a
  consensus and read as different. This disregards all
  other settings.

`-clust-q-snp 7`: minimum q-score needed to count a SNP
  as different.

# Other

Other settings that have little use in freezeTB.

`-depth-flag "tb"`: this flag sets the value used in
  column one in the `prefix-depths.tsv` file. Not very
  useful, unless you want to merge depth file results.
