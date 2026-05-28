# How freezeTB works

![Flow diagram showing freezeTB work flow](FTB-diagram.svg)

Note the images in the figure above are safe, public
  domain images from openclipart.org (manigifying glass
  and trash can) or were made by the freezeTB crew and
  are under freezeTB's license (public domain or MIT; your
  choice).

# Read filtering

1. Read mapping to H37Rv (NC_000962) reference:
   a. for a sam file with `-sam` (command line only) no
      mapping is done (you have already mapped)
   b. GUI: if possible uses minimap2
   c. otherwise uses the internal read mapper
      (`mapRead` from bioTools) to map reads
2. freezeTB removes (filtsam from bioTools):
   - unmapped reads
   - secondary alignments
   - supplemental alignments
3. freezeTB then filters the reads, removing any read that
   has (filtsam from bioTools):
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality (unless mapRead is used)
   - has a low mean Q-score (not ONT)
   - has a low median Q-score

Most methods of finding mean q-scores take the mean of all
  the base q-scores. ONT adds a few extra steps, which
  makes the mean q-score lower. Plan on increasing your
  minimum q-score by five to ten to account for this.

- ONTs method:
  1. convert the base q-scores to the error rate
  2. finds the mean error rate
  3. converts the mean error rate into a quality score

# Read currating:

1. Soft masked bases at the ends of the reads are removed
   (filtsam or trimSam from bioTools)
2. Optional (not recomeneded): small indels in
   homopolymers can be removed (rmHomo from bioTools)
3. Optional: freezeTB can masks primer locations in reads
   by coordinates (if requested) (maskPrim from bioTools)
   - default file with primer coordinates is coords.tsv
     in the freezeTBFiles (typically `/usr/local/share/`
     or `${HOME}/Documents`) or ftbResources directory.

# Read analysis:

- First, freezeTB adds each base in the read to the read
  depth histogram.

- FreezeTB checks the read for AMRs (getAmr from bioTools)
   - SNPs
   - for amino acids converts the targeted codons to amino
     acids and then compares
   - for deletions, looks for neighboring deletions
   - for frameshifts, looks for a missing start codon,
     scans for early stop codons and indels.
     - if mod(`total_deletions - total_insertions`, 3) is
       not 0, then flags as a frame shift

- FreezeTB checks for any MIRU-VNTR lineages in an
   read (tbMiru)
   - fails for rapdid kits and the consensus method is bad
     with insertions, so do not trust to far
   - measures length from primer start to primer end and
     compares to the table of expected lengths
   - The expected length can be 15bases off and still be
     flagged as a MIRU lineage

- FreezeTB checks for spoligotype spacers present in reads
   (tbSpol)
   a. checks coordinates for the direct repeat region
     - yes, move to step b
     - no, move the next read step
   b. move the the direct repeat region in the read
   c. convert the next 50 bases to 5mers
   d. check if the window has 50% of the spacers kmers
      - yes, move to step e
      - no, move window by 25 bases and repeat step c
   e. do a waterman smith aligment on the spacer and the
      window
      - 90% or greater similarity, flag as spoligotype
      - < 90% similarity, not a spacer
   f. repeat step c until at end of the direct repeat
      region in the read

- freezeTB detectes the different *Mycobaterium* species
  and other lineages in each read using getLin
    1. scans for mutations in the hsp65-db-simple.tsv file
    2. if hsp65 lineage (in hsp65-db-complex.tsv)
       a. counts the number of mutations to each hsp65
          species
          - if: ten or fewer mutations, keep the species
          - else: discared the species
       b. If more then one species, find the closest
          species
          - or the first hit in the database
    3. if other lineage from hsp65-db-complex.tsv
       a. check if found mutations matches the target
          lineage
          - yes: print supported
          - no: ignore

- FreezeTB then adds the read to the consensus (tbCon from
   bioTools)
   a. For: matches, SNPs or insertions, remove base if the
      Q-score is under 3 are removed
   b. If: match, SNP, or deletion, add the base to its
      positions counter
   c. Else: insertions, search the bases insertion linked
      list for exact matches
      a. Exact match, incurment that insertions counter
         - this is why the consensus method does not
           handel insertions well
      b. No, add the insertion to the insertion linked
         list
- During the consensus collapes step:
  1. Split up mappings into fragments
     a. must have at least 20x read depth
     b. must be at least 50 bases long
  2. check if have a match or SNPs at a position with at
     least 50% support
     a. yes: keep the match/SNP
     b. no: mask with an N
  3. check if have a deletion with at least 70% support
     a. yes: keep the deletion
     b. no: mask with an N
  4. compare the insertions depth to its negheibors
     a. If have at least 70% support, keep the insertion
     b. else delete the insertion

- After going though all reads; freezeTB prints out
    the read stats

# Consensus:

- FreezeTB makes read depth and coverage from the read
  depth histogram
  - only targets in coords.tsv are kept
  - max supported targets is 32

- If not doing mixed infectio detection freezeTB collapses
  the consensus. See adding read to consensus for
  more detials.

- FreezeTB finds the AMRs for the consensus (same as
  reads).

- FreezeTB finds the MIRU-VNTR lineages for consensus.
  Same methods as reads. Be warned the consensus does not
  handel insertions very well, so do not expect this to
  be accurate.

- FreezeTB finds spoligotype for consensus. Same method as
  reads.

- For Mixed infection detection: (edClust from bioTools);
  optional
  1. score reads and set up a list of reads not clusted
     (all reads)
  2. picks highest scoring read from the read list
  3. compares picked read to all other reads in the list
     - this step compares mapped reads. The comparision is
       not very accurate and often makes the reads look
       more distant. It would be better to re-align the
       reads.
     a. must have at lest 75% overlap
     b. SNPs in both reads to the reference are checked
        for similarity
     c. deletions are checked for in both reads
     d. insertions under 5 bases are ignored (I think)
   4. if the score for both reads passes the min score,
      then the reads are grouped together
      - a the top x (300) scoring reads are tracked
   5. check if have enough reads to build a consensus (20)
      a. not enough reads; remove the picked read from the
         read list and restart at step 2
      b. if enough reads, then build the consnesus with
         the top scoring reads
   6. repeat steps 3 to 5 x (2) times, but replace the
      highest scoring read with the consensus
      - minimum threshold scores are increased
      - if fail, remove the highest scoring read from the
        list and repeat step 2
   7. assign all reads that mapped to the consensus to the
      consensuses cluster
      a. remove these reads from the read list
   8. repeat steps 2 to 7 with the remaining reads
   9. compare all consensuses
      a. consensuses are very similar (around 99%); merge
         clusters
      b. consensuses are different; keep as a unique
         cluster
