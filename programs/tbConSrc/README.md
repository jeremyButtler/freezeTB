# Use

tbCon builds an reference based majority consensus for the
  input mapped reads. It should produce similar results to
  ivar, but does not need the additional mpileup steps.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install

To build tbCon, you will use `make -f` with the make
  file targeting your OS. The OS the make file was built
  for is the end of the name.

Make file names:
  - mkfile.unix: any unix (Mac/Linux/BSD) OS
  - mkfile.static: static build (Linux/BSD)
  - mkfile.win: windows (does not install)
    - use `nmake /F` instead of `make -f`

```
make -f mkfile.unix
sudo make install;
```

# Input:

- sam: sam file to build an consensus for. It should be
  mapped to only one reference (headers are checked for
  multiple references).
- out: file to save the consensus to (as sam file)
- out-tsv: file to save the variants tsv report to
  - You need to provide this to get an variants report
- h-all: print all settings

# Run:

```
tbcon -sam reads.sam -out consensuses.sam;

# or

minimap2 -a reference.fasta reads.fastq | tbcon -sam reads.sam -out consensuses.sam;

# or for variants

tbcon -sam reads.sam -out consensuses.sam -out-tsv variants.tsv;
```

# Understanding the output

The consensus is output as an sam file without an eqx
  entry. So, it will not have the number of SNPs, but
  will have deletions and insertions. You can convert the
  output to a sam file with eqx entry using cigToEqx
  (../cigToEqxSrc).

If you want a fasta file in an fasta file (will have an
  entry for each fragment) you can use samtools
  or `filtsam -out-fasta -sam consensus.sam -out cons.fa`
  (../filtSamSrc).

The variants tsv file (not an vcf) has a line for each
  possible variant. This means that one reference position
  can have multiple lines. Only variants passing the
  minimum thresholds (`-p-min-setting`) are printed. The
  tsv file works a bit differently then the consensus in
  that it also requires each variant to meet the minimum
  read depth.

- First column: reference id
- second column: reference position of variant
- third column: variant type (base/del/ins)
- fourth column: Variant
  - Base: nucleotide
  - Deletion: an `-`
  - Insertion: the insertion sequence
- fifth column: How many reads supported the variant
- sixth column: Percentage of reads supported the variant
  - bases: support / (del + A's + T's + C's + G's)
  - deletions: support / (kept reads)
    - Includes masked bases
  - insertions: support / (most supported neighbor)
- seventh column: Number of kept reads with masked bases
  - bases/deletions: total - (del + A's + T's + C's + G's)
  - insertions: an `NA` is printed
- eighth column: number of bases kept
  - (bases with Q >= 7) + deletions
  - For insertions this is the position next to the
    insertion with the most kept reads
- ninth column: total reads supporting the position
  - (bases with Q >= 7) + (bases with Q < 7) + deletions
  - For insertions this is the position next to the
    insertion with the most reads

# How it works

tbCon first adds each read to the uncollapsed consensus.
  For A's, T's, G's, C's, and deletions an counter is
  incremented for each new base. However, for insertions
  an list of all previously detected insertions is kept.
  If the new insertion matches an old insertion then an
  counter is incremented. If the insertion is new then
  it is added to the list with an count of one.

To reduce noise, bases (snps/matches/single insertions)
  with Q-scores under seven are discarded before adding
  the read to the uncollapsed consensus.

The consensus is then fragmented into fragments that have
  an read depth at or over 20x for all positions. tbCon
  then discards all fragments that are shorter then 200
  bases (insertions are not counted).

The consensus is then collapsed by keeping the most
  supported insertion and variant (deletion, A, T, G, or
  C). Insertions with less the 70% of mapped reads are
  removed. Deletions with less then 70% of mapped reads
  are masked. Bases with less then 50% of reads without
  anonymous bases (deletions + A's + T's + G's + C's)
  are masked.

For finding the percentage of support for SNPs/matches I
  am excluding the number of masked and anonymous bases
  from the total. I do this because in tilling primer
  approaches you would want to mask or trim the primer
  sites. If you masked, this would result in an mix of
  50% N's and actual bases at an position. By excluding
  anonymous bases from the total used to find the
  percentage I can get around this issue.
  
One difference between the variant tsv and the consensus
  is that the variant vcf will also check to make sure the
  base has at least 20x read depth. Were as the consensus
  will not. For deep read depths this will have little
  effect, but for shallow read depths it will. This is why
  the minimum print read depth is 10x instead of 20x.
