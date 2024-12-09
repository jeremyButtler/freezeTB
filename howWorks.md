# How freezeTB works

![
  Clip are flow digram showing how freezeTB works. Tempted
  to make an ascii digram with dita.
](freezeTb-diagram.svg)

1. GUI only: uses minimap2 to map fastq file to reference
2. freezeTB removes:
   - unmapped reads
   - secondary alignments
   - supplemental alignments
3. Soft masking is trimmed of the ends of each read
4. freezeTB adds each read to a histogram of read depths.
   This is the unfiltered histogram.
5. freezeTB then filters the reads, removing any read that
   has:
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality
   - has a low mean Q-score
   - has a low median Q-score
6. freezeTB then adds the kept reads to a histogram of
   filtered reads
7. freezeTB then masks primer locations in reads by
   coordinates (if requested)
8. freezeTB then checks the read for AMRs
9. freezeTB then checks for any MIRU-VNTR lineages in an
   read (Probably nothing for rapdid kits)
10. freezeTB then checks for spoligotype spacers present
   in reads
11. freezeTB then adds the read to the reference
   - Bases with Q-scores under 7 are removed
12. After going though all reads; freezeTB then prints
    out both histogram (unfiltered and filtered)
13. mixed infection with edClust if requested, replaces
    tbCon consensus building
14. freezeTB prints out the AMRs that at least 5% of
   mapped reads supported
15. freezeTB prints out the MIRU-VNTR lineage table
    of read counts for the reads
16. freezeTB prints out the spacers detected with the
    the number of reads that supported each spacer
17. freezeTB then collapses the consensus (if not doing
    mixed infection detection)
    - Split into fragments with 20x or greater read depth
    - Fragments less than 50x bases long are removed
    - Most supported snp/match/deletion selected
    - Snps/matches with less than 50% support are masked
    - Most supported insertion selected
    - Indels wit less than 70% support are removed
18. freezeTB finds the AMRs for the consensus
19. freezeTB finds the MIRU-VNTR lineages for consensus
20. freezeTB finds spoligotype for consensus
21. GUI only: freezeTB makes read depth and coverage
    graphs with R and calls the output GUI
    - you can do this separately with `outputGui.Rscript`
      or for just graphs (larger) `graphAmpDepth.r`

