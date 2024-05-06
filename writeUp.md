# Methods

## NON-BENCHMARKING VERSION

We mapped our reads to the NC000962.3 reference genome
  using minimap2 version XXX [@REF] with -a for sam file
  output and -x map-ont for ONT settings.
To detect AMRs, detect spoligotypes, and to build
  consensuses we used the mapped reads sam file and
  freezeTB version 2024-04-29
  [https://github.com/jeremybuttler/freezeTB](
   https://github.com/jeremybuttler/freezeTB
  ) with the -graph setting to output read depth and
  coverage graphs. For more details see the freezeTB
  supplemental methods.

## Benchmarking VERSION

NOTE Not all of this has to happen at once. We can do the
  accuracy tests afterwards if the study needs to stay
  blinded.

We compared the results, memory usage, and time usage for
  freezeTB version XXX and tbProfiler version XXX.
For tbProfiler we used Nanopore settings (`FLAG`), with
  AMRs limited to the strict WHO 2023 tuberculosis catalog
  (`FLAG`), with variant calling done by Pilon version XXX
  [@REF], and output the results as an csv file (`--csv`).
We extracted the antibiotics detected and genes with AMRs
  using an awk script
  [https://github.com/NAME/BENCHMARK_REPO_NAME](
   https://github.com/NAME/BENCHMARK_REPO_NAME)
Next we built the consensus genome using bcftools XXXX
  version [@REF] with ..., and the NC000962.3 reference.
We found the accuracy of the consensus using using quast
   version XXX [@REF] with ..., and the reference genome.
Spoligotyping for TBProfiler was ignored since TBProfiler
  does not due spoligotyping on long reads (THINK IS STILL
  THE CASE).
We automated these steps using an bash script
  [https://github.com/NAME/BENCHMARK_REPO_NAME](
   https://github.com/NAME/BENCHMARK_REPO_NAME)

For benchmarking freezeTB we first mapped the reads to
  the NC000962.3 reference genome using minimap2 version
  XXX with Nanopore settings (`-x map-ont`) and `-a` to
  output an sam file.
We extracted the detected antibiotics for the consensus
  using `awk 'BEGIN{getline;}; {print $2, $3;}' prefix-con-amrs.tsv`.
Next, we converted the consensus to an fasta file with
  filtsam from freezeTB.
We then found the consensus accuracy using quast with
  the previously mentioned settings for TBProfiler.
These steps were automated with an bash script
  [https://github.com/NAME/BENCHMARK_REPO_NAME](
   https://github.com/NAME/BENCHMARK_REPO_NAME)

We also compared the time and memory usage of both
  TBProfiler and freezeTB.
For time and memory comparisons we used an COMPUTER
  running Ubuntu Linux, with with an XXX cpu, XXXX
  gigabytes of TYPE ram, and an XXX m2 hardrive.
We measured the time using gnutime (`/usr/bin/time) to
  find the elapsed time (`%e`), the resident memory usage
  (actual memory used) (`%M`), and the processor usage
  (`%P`) `/usr/bin/time -f "%e\t%M\t%P"`.
Graphs comparing both freezeTB and TBProfile were made
  in R using the ggplot2 [@REF], data.table [@REF], and
  viridisLite [@REF] packages.

# Supplemental

## Supplemental methods

### How freezeTB works

![
   An flow digram showing an overview of freezeTB.
](diagrams/freezeTB-diagram.png){ #fig:freezeTBDiagram }

FreezeTb is an program that can detect AMRs, MIRU-VNTR
  lineages, spoligotypes, and build an consensus from an
  sam file of Nanopore sequence reads mapped to the
  NC000962.3 reference genomes.
The output of freezeTB is an tsv file with the detected
  AMRs (for reads and consensus), an tsv file with the
  consensus MIRU-VNTR lineages, and an tsv file with the
  consensus spoligotype.
FreezeTb also outputs an tsv with the mean read depth per
  gene and if requested an graph of read depth and 
  coverage that compares the filtered reads to the
  unfiltered reads.

To find the mean read depth and coverage for each gene we
  build an histogram of trimmed (soft masked bases
  removed) unfiltered and filtered read depths of each
  position supported [@fig:freezeTBDiagram].
To make the filtered histogram we removed any reads with
  an mapping quality under 15, an median Q-score under 7,
  an mean Q-score under 7, and that were shorter than 200
  bases.
We then find the mean read depth for all mapped genes or
  gene fragments that have at least 20x read depth.
Next we make can mean read depth and coverage graphs using
  an R script that uses ggplot2 [@REF], virdisLite [@REF],
  data.table [@REF], and the WHO's 2023 tuberculosis
  catalog [@REF].

FreezeTb builds consensuses with the filtered reads using
  an majority consensus step that is somewhat similar to
  Ivar [@REF] [@fig:freezeTBDiagram].
We first remove any SNPs, matches, or insertions from the
  reads that have an quality score under 7 from the read.
After all the reads are added, we collapse the consensus
  by keeping the most supported SNP, match, deletion or
  insertion.
We then masking positions without 50% support for SNPs or
  matches and remove indels with less than 70% support.
Finally, we split the consensus into regions with over 20x
  read depth and then discard regions that are under 200
  bases long and output the consensus fragments as an sam
  file.

FreezeTb uses the same steps to detect AMRs both in the
  filtered reads and the consensus [@fig:freezeTBDiagram].
For our database we removed all variants that were not
  an grade 1 or 2 from the 2023 WHO tuberculosis catalog.
We then searched the AMR database for variants matching
  the mapping coordinates of the sequence.
For cases were an catalog ARM variant had an amino acid
  sequences, we convert the AMR region in the sequence to
  amino acids.
We then compare the nucleotide or amino acid sequences
  to see if the sequence had catalog AMR variant.
Next, we detected and then removed false positives AMRs by
  comparing the length of the AMR region in the sequence 
  to length of the catalogs AMR variant.

On default settings freezeTb does not do additional checks
  on frame shifts and loss of function mutations.
However, though not recommended, freezeTB does include an
  option for some additional rudimentary checks for
  frame shifts and loss of function AMRs.
For loss of function AMRs, we look for early stop codons,
  lost stop codons (when possible), and lost start codons
  in the gene (when possible).
While for frame shifts we look to see if their are any
  indels in the AMR region.
For both frame shifts and loss of function mutations, we
  also check to if see if the number of indels are not
  divisible by three.

For MIRU-VNTR lineage detection we use the sequence
  coordinates from the sam file to find the primer
  starting and ending coordinates (from NC000962.3)
  [@fig:freezeTBDiagram].
Next we find the lineage by finding the length of the
  repeat region between the coordinates.
To account for for indel errors we classify the sequence
  to any lineage within 15 base pairs of its actual
  length.

For spoligotyping we use the sequence coordinates from the
  sam file to find the direct repeat region.
We then build our barcode by mapping the internal spacer
 sequences to extracted direct repeat region.
Next, we search the lineage database (csv) from TBProfiler
  for an matching lineage.
