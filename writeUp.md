# Methods

## NON-BENCHMARKING VERSION

We mapped our reads to the NC000962.3 reference genome
  using minimap2 version XXX [@REF] with -a for sam file
  output and -x map-ont for ONT settings.
To detect AMRs, detect spoligotypes, and to build
  consensuses we used the mapped reads sam file and
  freezeTb version XXX
   [https://github.com/jeremybuttler/freezeTb](
    https://github.com/jeremybuttler/freezeTb
   ) with the -graph setting to output read depth and
   coverage graphs (for more details on freezeTb see the
   freezeTb supplemental methods).

## Benchmarking VERSION

NOTE Not all of this has to happen at once. We can do the
  accuracy tests afterwards if the study needs to stay
  blinded.

We compared the results, memory usage, and time usage for
  freezeTb and tbProfiler.
For tbProfiler version XXX, we used Nanopore settings
  (`FLAG`), with AMRs limited to the strict WHO 2023
  tuberculosis catalog (`FLAG`), with variant calling done
  by Pilon version XXX [@REF], and output an csv file
  ('--csv').
We extracted the antibiotics detected and there gene names
  using an awk script
  [https://github.com/NAME/BENCHMARK_REPO_NAME](
   https://github.com/NAME/BENCHMARK_REPO_NAME)
Next we built the consensus genome using bcftools XXXX
  version [@REF] with ... and the NC000962.3 reference.
We found the accuracy of the consensus using using quast
   version XXX [@REF] with ... and the reference genome.
IF WE HAVE LINEAGE INFORMATION We did not compare lineages
  (MIRU VNTR or spoligotyping), because TBProfiler does
  not support lineage detection for long reads.

For freezeTb version XXX used the default settings (no
  graphs were made) and an sam file made using minimap2
  with sam file output (`-a`), ONT settings
  (`-x map-ont`), and the NC000962.3 reference genome.
We found the detected antibiotics
  using `awk 'BEGIN{getline;}; {print $2, $3;}'` on the
  consensus AMRs (prefix-con-amrs.tsv) file.
Next, we found the consensus accuracy using quast with
  the previously mentioned settings.
IF WE HAVE LINAGE INFORMATION We also compared the
  MIRU VNTR and spoligotype lineages to the known answer
  to.
To avoid issues with minimap2 time not being measured we
  made an base script to run freezeTb
  [https://github.com/NAME/BENCHMARK_REPO_NAME](
   https://github.com/NAME/BENCHMARK_REPO_NAME)

We also compared the time and memory usage of both
  TBProfiler and freezeTb.
For time and memory comparisons we used an COMPUTER
  running Ubuntu Linux, with with an XXX cpu, XXXX
  gigabytes of TYPE ram, and an XXX m2 hardrive.
We measured the time us (`/usr/bin/time) to find the
  elapsed time (`%e`), the resident memory (actual memory
  used) usage (`%M`), and the processor usage (`%P`)
  `/usr/bin/time -f "%e\t%M\t%P"`.
Graphs comparing both freezeTb and TBProfile were made
  in R using the ggplot2 [@REF], data.table [@REF], and
  viridisLite [@REF] packages.

# Supplemental

## Supplemental methods

### How freezeTb works

![
   An flow digram showing an overview of freezeTb.
](diagrams/freezeTb-diagram.pdf){ #fig:freezeTbDiagram }

FreezeTb is an program that can detect AMRs, MIRU-VNTR
  lineages, spoligotypes, and build an consensus from an
  sam file of Nanopore sequence reads mapped the
  NC000962.3 reference genomes.
For each of these steps (except consensus building),
  freezeTb outputs both an reference report (as tsv) and
  an read report (as tsv).
FreezeTb also outputs an tsv with the mean read depth per
  gene and if requested an graph of read depth and 
  coverage that compares the filtered reads to the
  unfiltered reads.

To built the mean read depth graph and coverage graph we
  build an histogram of unfiltered and filtered read
  depths [@fig:freezeTbDiagram].
We used the mapping coordinates [@tbl:coordinates] from
  the sam file to find the read depth for both histograms.
However, for the filtered histogram we removed any reads
  with an mapping quality under 15, an median Q-score
  under 7, an mean Q-score under 7, and that were shorter
  than 200 bases.
We then find the mean read depth for each section/gene
  that had an mean read depth at or greater than 20x.
To make the mean read depth and coverage graphs we use R
  (graphAmpDepth.r) with ggplot2 [@REF],
  virdisLite [@REF], data.table [@REF], the mean read
  depths, and the WHO's 2023 tuberculosis catalog [@REF].

| Coordinate | Sam entry        | Formula                       |
|:-----------|:-----------------|:------------------------------|
| Start      | Position         | NA                            |
| End        | Position + cigar | start + matches + SNPs + dels |

Table:
  Shows the method used for finding the start and end of
    the aligned read.
  Position is column 4, cigar is column 6.
  Dels refers to deletions.
  NA means the value is present.
  {#tbl:coordinates}

FreezeTb uses the same steps to detect AMRs both in the
  filtered reads and the consensus [@fig:freezeTbDiagram].
We restrict AMR detection to grade one and two AMR
  variants from the 2023 WHO tuberculosis catalog.
For AMRs with only nucleotide sequences or frame shift
  AMRs, we check to see if the read is an match to the AMR
  sequence.
However, for AMRs with amino acid sequences, we translate
  the AMR region in the read and then compare the two
  amino acid sequences.
For start codon losses we use all three bacterial start
  codons (atg, gtg, ttg).
Finally, we remove false positive matches caused by
  deletions by comparing the length of the reads AMR
  region to the length of the catalogs AMR variant.

FreezeTb also includes some support (off by default) for
  detecting frame shifts AMRs in consensus sequences.
For loss of function AMRs that affect the whole gene, we
  look for early stop codons, lost stop codons, lost start
  codons, and if the number of indels are not divisible by
  3 (indels % 3 != 0).
However, the missing start and stop codon checks are
  skipped for consensus missing the start and stop regions
  of an gene.
For frame shifts that are limited to an single position in
  an gene we only check to see if the indels are not an
  multiple of 3 (not best method, but is quick).
We have set shift detection to off by default, because
  Nanopore sequenced reads and to an lesser degree
  consensuses, are known to have indel errors, which look
  like frame shifts, in homopolymer regions [@REF].

FreezeTb does MIRU-VNTR lineage detection on both the
  filtered reads and the consensus genome
  [@fig:freezeTbDiagram].
We identify the primer binding sites using the primer
  mapping coordinates on the NC000962.3 reference genome
  (Part of the input table).
Next we find the length of the amplifed region and see if
  any lineages have an amplicon length within within 15
  bases of the reads or consensuses length.
Finally, we print out the lineage for the consensus or an
  MIRU-VNTR lineage table with read counts.

**TODO: Add in spoligotyping when I have program made.**

FreezeTb builds consensuses with the filtered reads using
  an majority consensus step that is somewhat similar to
  Ivar [@REF] [@fig:freezeTbDiagram].
We first remove any SNPs, matches, or insertions from the
  reads that have an quality score under 7 from the read.
After all the reads are added, we collapse the consensus
  by keeping the most supported SNP, match, deletion or
  insertion.
We then masking positions without 50% support for SNPs or
  matches and remove indels with less than 70% support.
Finally, we split the consensus into regions with over 20x
  read depth and then discard regions that are under 200
  bases long.
