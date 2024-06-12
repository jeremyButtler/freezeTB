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

# Methods

We compared detected antibiotics, memory usage, and time
  usage for freezeTB version 2024-06-11 
  [https://github.com/jeremybuttler/freezeTB](
   https://github.com/jeremybuttler/freezeTB) and
  TBProfiler version 6.2.0 and database version 82777ea
  [@phelan-2019-pmid-31234910].
For TBProfiler, we used Nanopore settings
  (`--platform nanopore`) with bcftools version 1.20
  [@danecek-2021-pmid-33590861] for variant calling
   (`--caller bcftools`), to output and csv file (`--csv`)
  with the detected AMRs and spoligotype (`--spoligotype`)
  of the input reads (`--read1 reads.fastq`).
We automated the steps to run TBProfiler using an bash
  script
  [https://github.com/jeremybutter/freezeTB/benchmark/run-tbprofiler.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/run-tbprofiler.sh).

To detect AMRs with freezeTB
  [https://github.com/jeremybuttler/freezeTB](
   https://github.com/jeremybuttler/freezeTB
  ), we used an sam file (`-sam reads.sam`) with reads
  mapped to the NC000962.3 (H32Rv( reference genome.
We mapped the reads to H37Rv using minimap2 version
  2.28-r1209 [
      @li-2018-pmid-29750242; @li-2021-pmid-34623391
  ] with Nanopore settings (`-x map-ont`) to make an sam
  file (`-a`).
We automated these steps with an bash script
  [https://github.com/jeremybutter/freezeTB/benchmark/run-freezeTB.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/run-freezeTB.sh).

Next we compared the consensuses, the detected
  antibiotics, and the detected spoligotype for both
  freezeTB and TBProfiler.
For TBProfiler, we extracted the spoligotype from the
  output csv file using an awk script
  [https://github.com/jeremybutter/freezeTB/benchmark/TBProfiler-process.awk](
   https://github.com/jeremybutter/freezeTB/benchmark/TBProfiler-process.awk).
We also, extracted the antibiotics that did not have
  "Uncertain significance", "Not assoc w R", or
  "Mutation from literature" using the same awk script.
Next, we built an consensus from the vcf file output by
  TBProfiler using bcftools consensus with the H32Rv
  reference genome.
We then found and masked low read depth positions using
  samtools depth version XXX [@danecek-2021-pmid-33590861]
  and primMask version 2024-05-14 from freezeTB.

For freezeTB, we extracted the detected antibiotics and
  detected spoligotype in the consensuses using awk.
We then converted the consensus to an fasta file
  (`-out-fasta`) with filtsam version 2024-05-14 from
  freezeTB.
Next we mapped the consensus from freezeTB to the
  consensus from TBProfiler using minimap2 to output an
  sam file (`-a`) with an eqx cigar entry (`--eqx`).
We then found the number of matches, snps, indels, and
  anonymous bases between the mapped consensuses using
  filtsam to output stats (`-out-stats`).
We automated these steps for to run and comparing freezeTB
  and TBProfiler with an bash script
  [https://github.com/jeremybutter/freezeTB/benchmark/TBOneBench.sh](
   https://github.com/jeremybutter/freezeTB/benchmark/TBOneBench.sh).

In our automated bash scripts we also compared the time
  and memory usage of both TBProfiler and freezeTB.
For time and memory comparisons we used an desktop
  running Ubuntu, with an AMD Ryzen 9 5950X 16 core cpu,
  and DDR4 ram.
We measured the time using gnutime (`/usr/bin/time`) to
  find the elapsed time (`%e`), the resident memory usage
  (actual memory used) (`%M`), and the processor usage
  (`%P`) (`/usr/bin/time -f "%e\t%M\t%P"`).
Graphs comparing time, memory usage, and output for
  freezeTB and TBProfile were made in R version 3.6.3
  [@R] using the ggplot2 version 3.2.1 [@ggplot2],
  data.table version 1.12.8 [@dataTbl], and viridisLite
  0.3.0 [@viridisLite] packages.

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
   An flow digram showing an overview of freezeTB.
](../diagrams/freezeTb-diagram.png){ #fig:freezeTBDiagram }

FreezeTb is an program detects AMRs, MIRU-VNTR lineages,
  spoligotypes, and builds an consensus using an sam file
  of Nanopore sequence reads mapped to the NC000962.3
  (H32Rv) reference genomes.
The output of freezeTB are a series tsv files with the
  detected AMRs, with the MIRU-VNTR lineages, with the
  detected spoligotype, and the mean read depth per gene.
FreezeTb also outputs an sam file with an set of
  consensuses and if requested, graphs of mean read depth 
  and coverage.

#### Read depth and coverage reports

Before finding the mean read depth we trim of soft masked
  regions of the read.
We then find the mean read depth and coverage for each
  gene using histograms for the pre-filtered reads
  and reads kept after filtering
  (@fig:freezeTBDiagram).
For filtering, we remove any reads with an mapping quality
  under 15, an median Q-score under 7, an mean Q-score
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

For consensus building [@fig:freezeTBDiagram], we use an
  majority consensus step that is somewhat similar to Ivar
  [@Grubaugh-2019-pmid-30621750].
We first remove any SNPs, matches, or insertions from the
  masked reads that have an quality score under seven.
Next, we add in any surviving SNPs, matches, or indels
  to the tally of the matching base, deletion, or for
  insertions matching sequence, at the bases position in
  the H37Rv.
After all reads are added to the consensus, we then
  collapse the consensus by keeping the most supported
  SNPs, matches, and deletions for each position.
We also, keep the most supported insertion for an
  position.

Next, we mask SNPs (50%), matches (50%), and deletions
  (70%)in the consensus that have low percentage of mapped
  reads.
We also, remove any insertions that have an low percentage
  of mapped reads (70%).
For SNPs and matches, we do not count anonymous bases in
  the total used to find the percentage of support for an
  SNP or match.
However for deletions and insertions, our total includes
  the anonymous bases.
Also, for insertions, our total comes from the neighbor
  with with the highest number of mapped reads.

The final step in consensus building is to convert the
  single consensus into fragments with good read depth.
We fragment the consensus by removing any position that
  has less than 20x read depth.
Next, we remove any fragment that is under 200 bases long.
Finally, we save the consensus fragments in sam file.

#### AMR detection

For AMRs detection ([@fig:freezeTBDiagram]) we use an
  tsv version of the WHOs 2023 tuberculosis catalog that
  has had all grade thee, four, and five AMRs removed.
We find potential AMR variants for each sequence using
  the sequences mapping coordinates on the H37Rv
  reference.
For AMR variants that have an amino acid sequence, we
  translate the AMR region in the sequence.
We then check if the AMR region in the sequence matches
  the AMR variant's sequence.
Finally, we detect and remove false positive AMRs by
  detecting differences in length between the sequences
  AMR region and the AMR variant.

On default settings freezeTB does not check for frame
  shifts, gene deletions, and loss of function AMRs.
However, though nadditional ot recommended, freezeTB does include an
  option to allow an simple check for frame shifts and
  loss of function AMRs.
For loss of function AMRs, we translate the parts of the
  sequence that cover the target gene.
We then look for early stop codons, lost stop codons
  (when possible), and lost start codons (when possible).
Also, we count the number of indels and check if they
  are divisible by three (not an LoF).

For frame shifts we look to see if their are any indels in
  the AMR region.
We also count the number of neighboring indels and check
  if they are divisible by three.

#### MIRU-VNTR lineages

To detect MIRU-VNTR lineages ([@fig:freezeTBDiagram]) we
  identify primers sites in the sequence using the primer
  mappings coordinates on the H37Rv genome.
We find the lineage for the site by finding the sequences
  repeat region length and comparing it to an modified
  version of the MIRU-VNTR table from MIRUReader
  [@Tang-2020-pmid-31603462].
To account for for indel errors we allow an 15 base pair
  difference between the repeat length and the actual
  lineages length.

#### Spoligotyping

For spoligotyping we use the direct repeat region
  coordinates from the H37Rv reference genome to identify
  reads that could have have an spacer.
We then count the number of matching 5mers present in an
  spacer and in an 50 nucleotide window in the sequences
  direct repeat region.
If their are ten or more matching 5mers, we do an
  Waterman Smith alignment, using an EDANFULL matrix, and
  gap penalty of -10.
We count an spacer as detected if the alignment has an
  score that is 90% of the maximum possible score for the
  spacer.
For detected spacers, we increment the mapped spacers
  counter by one.
We then move the window to the next 25 nucleotides in
  the direct repeat region and check for spacers until the
  entire direct repeat region has been checked.
Next, we convert the spacer mapped counts into an barcode
  by reducing all counts to one.
For reads, we then print out the barcode, number of reads
  having at least on spacer, and the number of reads that
  supported each position in the barcode.
For consensuses, we then search the lineage database (csv)
  from TBProfiler for an lineage that matches the
  consensuses barcode.

# References

::: (#refs)
:::
