# How freezeTB works

![Flow diagram showing freezeTB work flow](FTB-diagram.svg)

Note the images in the figure above are safe, public
  domain images from openclipart.org (manigifying glass
  and trash can) or were made by the freezeTB crew and
  are under freezeTB's license (public domain or MIT; your
  choice).

1. Read mapping to H37Rv (NC_000962) reference:
   1. you input a sam file with `-sam` (command line only)
   2. GUI's default: use minimap2 to map reads
   3. use the internal read mapper (`mapRead` from
      bioTools) to map reads (GUI fallback/command line)
2. freezeTB removes:
   - unmapped reads
   - secondary alignments
   - supplemental alignments
3. Soft masking is trimmed of the ends of each read
4. Optional: rmHomo is used to remove small indels in
   homopolymers over three bases long
5. freezeTB then filters the reads, removing any read that
   has:
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality (unless mapRead is used)
   - has a low mean Q-score
   - has a low median Q-score
6. freezeTB then adds the read depth histogram of
7. Optional: freezeTB masks primer locations in reads by
   coordinates (if requested)
8. freezeTB checks the read for AMRs
9. freezeTB checks for any MIRU-VNTR lineages in an
   read (Probably nothing for rapdid kits)
10. freezeTB then checks for spoligotype spacers present
   in reads
11. freezeTB then adds the read to the reference
   - Bases with Q-scores under 3 are removed
12. After going though all reads; freezeTB prints out
    the read stats
13. Optinoal: mixed infection with edClust if requested,
    replaces/uses tbCon consensus building
14. freezeTB then collapses the consensus (if not doing
    mixed infection detection)
    - Split into fragments with 20x or greater read depth
    - Fragments less than 50x bases long are removed
    - Most supported snp/match/deletion selected
    - Snps/matches with less than 50% support are masked
    - Most supported insertion selected
    - Indels wit less than 70% support are removed
15. freezeTB finds the AMRs for the consensus
16. freezeTB finds the MIRU-VNTR lineages for consensus
17. freezeTB finds spoligotype for consensus
18. The consensus stats are then printed out
19. GUI only: freezeTB makes read depth and coverage
    graphs with R and calls the output GUI
    - you can do this separately with `outputGui.Rscript`
      or for just graphs (larger) `graphAmpDepth.r`

