---
bibliography: refs.bib

geometry: margin=25mm

figPrefix: "Figure"
tblPrefix: "Table"

link-citations: true
link-bibliograpy: true

### for suplicmental figure reset in docx builds
chapters: True
chaptersDepth: 1
chapDelim: ""
---

**Notes**

get EPI2ME wf-tb-amr add to benchmarking scripts

**Paper**

# Methods

We compared detected antibiotics, memory usage, and time
  usage for freezeTB version 2024-06-11 
  ([https://github.com/jeremybuttler/freezeTB](
   https://github.com/jeremybuttler/freezeTB)), wf-tb-amr
  ([https://github.com/epi2me-labs/wf-tb-amr](
    https://github.com/epi2me-labs/wf-tb-amr), and
  TBProfiler version 6.2.0 and database version 82777ea
  [@phelan-2019-pmid-31234910].

For wf-tb-amr, we
  used `nextflow run epi2me-labs/wf-tb-amr` with
  `--fastq barcode0x-directory/` to run wf-tb-amr on one
  barcode and `-profile standard`.
We automated the steps to run wf-tb-amr using a bash
  script
  ([https://github.com/jeremybutter/freezeTB/benchmark/run-wfTbAmr.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/run-wfTbAmr.sh)).

For TBProfiler, we used Nanopore settings
  (`--platform nanopore`) with bcftools version 1.20
  [@danecek-2021-pmid-33590861] for variant calling
   (`--caller bcftools`), to output and csv file (`--csv`)
  with the detected AMRs and spoligotype (`--spoligotype`)
  of the input reads (`--read1 reads.fastq`).
We automated the steps to run TBProfiler using a bash
  script
  ([https://github.com/jeremybutter/freezeTB/benchmark/run-tbprofiler.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/run-tbprofiler.sh)).

To detect AMRs with freezeTB
  ([https://github.com/jeremybuttler/freezeTB](
   https://github.com/jeremybuttler/freezeTB)),
  we used a sam file (`-sam reads.sam`) with reads
  mapped to the NC000962.3 (H32Rv) reference genome.
We mapped the reads to H37Rv using minimap2 version
  2.28-r1209
  [@li-2018-pmid-29750242; @li-2021-pmid-34623391]
  with Nanopore settings (`-x map-ont`) to make a sam
  file (`-a`).
We automated these steps with a bash script
  ([https://github.com/jeremybutter/freezeTB/benchmark/run-freezeTB.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/run-freezeTB.sh)).

**add empi2e to this when I get a better idea**

Next, we compared the consensuses, the detected
  antibiotics, and the detected spoligotype for both
  freezeTB and TBProfiler.
For TBProfiler, we extracted the spoligotype from the
  output csv file using an awk script
  ([https://github.com/jeremybutter/freezeTB/benchmark/TBProfiler-process.awk](
   https://github.com/jeremybutter/freezeTB/benchmark/TBProfiler-process.awk)).
We also extracted the antibiotics that did not have
  "Uncertain significance", "Not assoc w R", or
  "Mutation from literature" using the same awk script.
Next, we built a consensus from the vcf file output by
  TBProfiler using bcftools consensus with the H32Rv
  reference genome.
We then found and masked low read depth positions using
  samtools depth version XXX [@danecek-2021-pmid-33590861]
  and primMask version 2024-05-14 from freezeTB.

For freezeTB, we extracted the detected antibiotics and
  detected spoligotype in the consensuses using awk.
We then converted the consensus to a fasta file
  (`-out-fasta`) with filtsam version 2024-05-14 from
  freezeTB.
Next, we mapped the consensus from freezeTB to the
  consensus from TBProfiler using minimap2 to output a
  sam file (`-a`) with an eqx cigar entry (`--eqx`).
We then found the number of matches, snps, indels, and
  anonymous bases between the mapped consensuses using
  filtsam to output stats (`-out-stats`).
We automated these steps for to run and comparing freezeTB
  and TBProfiler with a bash script
  ([https://github.com/jeremybutter/freezeTB/benchmark/TBOneBench.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/TBOneBench.sh)).

In our automated bash scripts, we also compared the time
  and memory usage of wf-tb-amr, TBProfiler, and freezeTB.
For time and memory comparisons, we used a desktop
  running Ubuntu, with an AMD Ryzen 9 5950X 16 core CPU,
  and DDR4 ram.
We measured the time using gnutime (`/usr/bin/time`) to
  find the elapsed time (`%e`), the resident memory usage
  (actual memory used) (`%M`), and the processor usage
  (`%P`) (`/usr/bin/time -f "%e\t%M\t%P"`).
We made graphs comparing time, memory usage, AMRs
  detected, and consensus similarity for wf-tb-amr,
  TBProfiler, and freezeTB with R version 3.6.3 [@R] using
  the ggplot2 version 3.2.1 [@ggplot2], data.table
  version 1.12.8 [@dataTbl], and viridisLite 0.3.0
  [@viridisLite] packages.

**May need to update versions. Things are out of date**

<!--This is to have separate numbering for suplemental
    figures
    Pandoc removes/ignores latex commands for docx builds
-->
\renewcommand{\thefigure}{Sup. \arabic{figure}}
\renewcommand{\thetable}{Sup. \arabic{table}}
\setcounter{figure}{0}
\setcounter{table}{0}

# Supplemental {label="Sup. "}

## Supplemental methods

### How freezeTB works

![
   An flow digram showing a overview of freezeTB.
](
 ../diagrams/freezeTb-diagram.png){ #fig:freezeTBDiagram }

FreezeTb is a program detects AMRs, MIRU-VNTR lineages,
  spoligotypes, and builds a consensus using a sam file
  of Nanopore sequence reads mapped to the NC000962.3
  (H32Rv) reference genome.
FreezeTB also reports the detected AMRs, the MIRU-VNTR
  lineages, the detected spoligotype, and the mean read
  depth per gene.
Also, output from freezeTb is a sam file with the
  consensus fragments.

#### Read depth and coverage reports

We find the mean read depth and coverage for each gene
  using pre-filtered and filtered read depth histograms
  (@fig:freezeTBDiagram).
For filtering, we remove any reads with a mapping quality
  under 15, a median Q-score under 7, a mean Q-score
  under 7, and that mapped to at least 200 bases in the
  reference genome.
We then use the histograms to find the mean read depths
  using the genes reference coordinates to find the genes
  location in the histogram.
Next, we make the mean read depth and coverage graphs
  using an R [@R] script that uses ggplot2 [@ggplot2],
  virdisLite [@viridisLite], data.table [@dataTbl], and
  the WHO's 2023 tuberculosis catalog [@WHO-TB-2023].

#### Primer masking

Before building the consensus, we mask any primers sites
  in the reads.
We find and mask primer sites using the primers mapping
  coordinates on the H37Rv genome.

#### Consensus building

For consensus building [@fig:freezeTBDiagram], we use a
  majority consensus step that is somewhat similar to Ivar
  [@Grubaugh-2019-pmid-30621750].
We first remove any SNPs, matches, or insertions from the
  masked reads that have a quality score under seven.
Next, we add in any surviving SNPs, matches, or indels
  to the tally of the matching base, deletion.
For new insertions, we add the sequence to the insertion
  list.
For old insertions, we increment the tally to the matching
  sequence.
We then collapse the consensus by keeping the most
  supported SNPs, matches, and deletions for each
  position.
We also keep the most supported insertion for a position.

Next, we mask SNPs (50%), matches (50%), and deletions
  (70%) in the consensus that have a low percentage of
  mapped reads.
We also remove any insertions that have a low percentage
  of mapped reads (70%).
For SNPs and matches, we do not count anonymous bases in
  the total used to find the percentage of support for an
  SNP or match.
However, for deletions and insertions, our total includes
  the anonymous bases.
Also, for insertions, our total comes from the neighbor
  with the highest number of mapped reads.

The last step in consensus building is to convert the
  single consensus into fragments with good read depth.
We fragment the consensus by removing any position that
  has less than 20x read depth.
Next, we remove any fragment that is under 50 bases long.
Finally, we save the consensus fragments in sam file.

#### AMR detection

For AMRs detection ([@fig:freezeTBDiagram]) we use an
  tsv version of the WHOs 2023 tuberculosis catalog that
  has had all grade thee, four, and five AMRs removed.
We find potential AMR variants for each sequence using
  the sequences mapping coordinates on the H37Rv
  reference.
For AMR variants that have an amino acid sequence, we
  translate the sequences AMR region.
We then check if the AMR region in the sequence matches
  the AMR variant's sequence.
Finally, we detect and remove false positive AMRs by
  detecting differences in length between the sequences
  AMR region and the AMR variant.

By default, freezeTB does not check for frame
  shifts, gene deletions, and loss of function AMRs.
However, though not recommended, freezeTB includes an
  option to allow a simple check for frame shifts and
  loss of function AMRs.
For loss of function AMRs, we translate sequence regions
  that cover the target gene.
We then look for early stop codons, lost stop codons
  (when possible), and lost start codons (when possible).
Also, we check for frame shifts by subtracting the number
  of deletions from number of insertions and checking for
  a remainder after division by
  three (`(deletions in gene - insertions in gene) mod 3`).

For local frame shifts, we look to see if their are any
  indels in the AMR region.
If there is an indel in the expected location we classify
  it as a frame shift.
Otherwise, we use the neighboring indels to detected
 frame shifts using `(deletions - insertions) mod 3`.

#### MIRU-VNTR lineages

To detect MIRU-VNTR lineages ([@fig:freezeTBDiagram]) we
  identify primers sites in the sequence using the primer
  mappings coordinates on the H37Rv genome.
We first locate any MIRU-VNTR primers in the sequence
  using the sequences mapping coordinates.
We then find the lineage for each MIRU-VNTR site by
  comparing the repeat lengths to the MIRU-VNTR
  table from MIRUReader
  [@Tang-2020-pmid-31603462].
To account for differences in length from indel errors, we
  allow a 15 base pair difference between the repeat
  length and the actual lineages length.

#### Spoligotyping

For spoligotyping, we use the direct repeat region
  coordinates from the H37Rv reference genome to identify
  reads or consensuses that could have a spacer.
We then count the number of matching 5mers present in a
  spacer and in a 50 nucleotide window in the sequences
  direct repeat region.
We ignore any window with less than ten 5mers.
While we align windows with ten or more 5mers to the
  spacer with a Waterman Smith alignment, using the
  EDANFULL matrix and a gap penalty of -10 and a
  gap extension penalty of -0.1.
We count a spacer as mapped (1) if the alignment score is
  at least 90% of the spacer's maximum score.
After mapping, we move the window to the next 25
  nucleotides the direct repeat region.
We then find the lineage using the barcode made by mapping
  spacers and the spoligotyping database from TBProfiler.

# References

::: (#refs)
:::
