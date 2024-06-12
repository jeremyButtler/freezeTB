# Use

freezeTb is a program that detects AMR resistance in
  Nanopore sequence tuberculosis samples. It also outputs
  an set of consensus fragments (as an sam file) and
  checks for spoligotypes and MIRU-VNTR lineages. However,
  unlike TBProfiler, freezeTB does not check for
  genes that need to be functional for resistance, nor
  does it output a nice report.

Currently freezeTb uses an WHO 2023 catalog that has had
  all grade 3, 4, and 5 variants removed.

# License:

This is under an dual license. The first license is public
  domain, however, not all companies/countries like public
  domain. In those cases or any other case were public
  domain is inconvenient this is under the MIT license.

# Requirements 

1. A read mapper that outputs a sam file.
   - Your choice for the command line version 
   - For the GUI you will need minimap2, which can be
     often installed by your package manager
     - `sudo apt-get install minimap2` for Ubuntu/Debian
       - or use `aptitude` to find it
     - brew (you have to install this) for Mac
     - source [https://github.com/lh3/minimap2](
               https://github.com/lh3/minimap2)
2. R and several R libraries. If you have R, then the
   make file will detect and then try to install missing R
   libraries when you install freezeTB.
   - For command line version this is only needed with
     the `-graph` option.
     - ggplot2
     - svgLite (if you request svgs)
   - For the GUI version you will need
     - ggplot2
     - tcltk (often is in base R) or tcltk2 (has tooltips)
     - svgLite (Only if you want svgs)
     - fs (Optional)

# Install

The freezeTB install will install freezeTB,
  graphAmpDepth.r, and everything in freezeTBFiles to the
  install location. freezeTB is the program you can run,
  graphAmpDepth.r is the R script to make graphs with, and
  freezeTBFiles are the databases, such as the WHO
  catalog, that freezeTb uses for the default files.

Do not worry if you see errors in the `make install`
  command about minimap2. These are not errors, but extra
  commands in case you did the minimap2 install command
  (`make mk` or for Mac/ARM cpus `mkneon`) instead of
  just `make`.

## Linux or Mac

```
git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make
sudo make install
make clean
```

or for an static build (Non-Mac)

```

git clone https://github.com/jeremybuttler/freezeTb
cd freezeTb
make static
sudo make install
make clean
```

## More advanced

This installs freezeTb, freezeTbGui.r, and graphAmpDepth.r
  into `/usr/local/bin/`. It also installs the
  freezeTBFiles folder to `/usr/local/share`.

If you are having problems installing try doing a local
  install.

```
make # or make mk or make mkneon
make PREFIX=~/local/path/ dbPREFIX=~/Documents/ install
```

- PREFIX changes the were freezeTB, freezeTBGui.r, and
  graphAmpDepth.r are installed.
- dbPREFIX changes were the database is installed.
  - The only supported locations are `/usr/local/share/`
    and `~/Documents/`.
  - You can change these by changing defPathStr (global)
     and (local path) in freezeTb.c
     - Header Sec-02: lines 112 to 116. The lines beneath
       it 119 to 121 have the expected file names.
     - For the gui, change the values in Header
       Sec-02 Sub-01 and Header Sec-02 Sub-02 in
       freezeTbGui.r.

## Windows

I have not tried it. I should, but probably never will.

# Run

## CLI

You can print the help message with `freezeTB -h`.

```
minimap -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq > reads.sam;
freezeTB -sam reads.sam -prefix good-name;
```

or

```
minimap -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq | freezeTB.sh -sam - -prefix good-name
```

A more complex way to run freezeTB is to provide the paths
  to all the databases (AMR, MIRU, spoligotyping, and
  gene-table). You will have to do this if you the files
  are not in the default location or if you decide to
  use an different reference for the AMR database.

If these cases are normal, then I would recommend putting
  the commands in an bash script.

```
freezeTB \
    -sam reads.sam \
    -prefix good-name \
    -amr-tbl freezeTBFiles/who-2023.tsv \
    -miru-tbl freezeTBFiles/miruTbl.tsv \
    -gene-coords freezeTBFiles/gene-tbl.tsv \
    -spoligo freezeTBFiles/spoligotype-seq.fa \
    -db-spoligo freezeTBFiles/spoligo-lineages.csv \
    -dr-start 3119037 \
    -dr-end 3123624;
```

Were `-dr-start` and `-dr-end` are the coordinates for the
  direct repeat region in the reference genome.

You can leave out `-spoligo` if you are changing an
  reference, since it has sequences not coordinates.

## GUI

For the gui, just run `freezeTBGui.r`. Then fill in the
  values needed. This should be only fastq file if the
  databases were detected.

# How it works

![
  Clip are flow digram showing how freezeTB works. Tempted
  to make an ascii digram with dita.
](diagrams/freezeTb-diagram.svg)

1. freezeTB removes:
   - unmapped reads
   - secondary alignments
   - supplemental alignments
2. Soft masking is trimmed of the ends of each read
3. freezeTB adds each read to a histogram of read depths.
   This is the unfiltered histogram.
4. freezeTB then filters the reads, removing any read that
   has:
   - to short by aligned length (number of reference bases
     covered)
   - has a low mapping quality
   - has a low mean Q-score
   - has a low median Q-score
5. freezeTB then adds the kept reads to a histogram of
   filtered reads
6. freezeTB then masks primer locations in reads by
   coordinates (if requested)
7. freezeTB then checks the read for AMRs
8. freezeTB then checks for any MIRU-VNTR lineages in an
   read (Probably nothing for rapdid kits)
9. freezeTB then checks for spoligotype spacers present
   in reads
10. freezeTB then adds the read to the reference
   - Bases with Q-scores under 7 are removed
11. After going though all reads; freezeTB then prints
   out both histogram (unfiltered and filtered)
12. freezeTB prints out the AMRs that at least 5% of
   mapped reads supported
13. freezeTB prints out the MIRU-VNTR lineage table
    of read counts for the reads
14. freezeTB prints out the spacers detected with the
    the number of reads that supported each spacer
15. freezeTB then collapses the consensus
    - Split into fragments with 20x or greater read depth
    - Fragments with under 200x read depth are removed
    - Most supported snp/match/deletion selected
    - Snps/matches with less than 50% support are masked
    - Most supported insertion selected
    - Indels wit less than 70% support are removed
16. freezeTB finds the AMRs for the consensus
17. freezeTB finds the MIRU-VNTR lineages for consensus
18. freezeTB finds spoligotype for consensus
19. freezeTB then makes the read depth and coverage graphs
    with R (only if you used -graph or -graph-ext "ext")

# Programs included with freezeTB

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it separately. I have set the default build
  methods to be static for the modules, so you will need
  to do `make mac` on anything that is not freezeTB. All
  of these programs take sam files as inputs.

List of modules:

- ampDepth: Makes the read depth tsv's that can be
  converted to graphs of mean read depth and coverage
  using scripts/graphAmpDepth.r
- trimSam: trims off soft masked bases in an sam file
- primMask: Masks primers in sequences (sam file) using
  coordinates. This can also do some filtering.
  - You can make an basic mask file file using your
    primers and catPrimers.sh
- tbCon: Builds an consensus (output is sam file)
- tbAmr: AMR detection
- tbMiru: detects MIRU-VNTR lineages in reads
  - This also includes a couple of awk scripts for
    processing the table made with `-out-tbl miru-tlb.tsv`
    - awk -f `getPercMiruTbl.awk miru-tbl.tsv` is here to
      print out the percentage of mapped  reads for each
      primer
    - awk -f `getHitMiruTbl.awk miru-tbl.tsv` is here to
      print out the number of reads supporting each primer
- tbSpoligo: does spoligotyping
  - fasta/fastq is slow, sam is somewhat fast.

Other, less useful programs:

If there is no README.md file, then do `make mac` or
  `make` to compile the program and then `program -h` to
  get the help message. Most programs should install with
  `sudo make install`. If not
  do `sudo mv program /usr/local/bin`
  and `sudo chmod a+x /usr/local/bin/program` to install
  the program.

- filtSam: For filtering trimming softmasked regions, and
  converting sam files. You would be better off using
  samtools.
  - I have used it for finding the number of SNPs,
    deletions, and, insertions for each read in an sam
    file.
    - `filtSam -p-n -out-stats -sam file.sam > stats.tsv`
  - I have also used it to filter reads by coordinates
    on an reference `filtsam -sam reads.sam -coords x,y`
    or `fitsam -sam reads.sam -coords-file filters.tsv`
  - If you only want trimming, then trimSam might be an
    better option. Go with the faster version.
- adjCoords: Adjust coordinates in sam file using an
  list of genes.
  - The idea is that you map your amplicons to an list of
    target genes and then correct the coordinates to the
    reference with adjCoords.
  - You can find the commands to make an gene table for
    adjCoords in `slidesAndWriting/making-gene-table.md`
  - adjCoords removes any entry it can not adjust (not in
    the gene table)
    - This removes unmapped reads or reads mapping to
      references not found in the gene table
      - So, you can include the 16sRNA *E. coli* gene
        in the references, but not include it in the gene
        table to remove any *E. coli* 16sRNA reads
- cnvt_woToTbAmr (tbAmrSource/cnvt_whoToTbAmr_src)
  converts the WHO 2023 catalog to tbAmr format.
  - This has a separate public repository
  - Only needed if you want grade 3 variants or the WHO
    updates the 2023 database
- swapDbRef (tbArmSource/swapDbRefSrc) is a program to
  switch out references in the reference database.
  - The convserion does not check for frameshifts or other
    problems. It just adjusts the coordinates by the
    positions on the reference genome.
- mkMiurTbl (tbMiruSource/mkMiruTbl) is an program that
  takes in an miru table and adds the reference
  coordinates for tbMiru into the table header
  - The pre-miruTbl.txt in tbMiruSource/mkMiruTbl is an
    blank table ready to have coordinates added in
  - The MIRU primers are in
    tbMiruSource/mkMiruTbl/primers.fa
  - Currently does not use the faster kmerFind used in
    spoligotyping. I figure you only need to do it once
- cigToEqxCig: Converts an sam file with an normal cigar
  to an eqx cigar
  - Mainly here to avoid running minimap2 again on
    consensuses from tbCon
  - `cigToEqxCig -sam file.sam -ref ref.fasta > out.sam`
- scrpits/catPrimers.sh: bash script that uses minimap2
  and awk to merge amplicons.
  - It also prints out the amplicon and primer coordaintes
    needed for primMask.
  - `bash scripts/catPrimers.sh -ref ref.fasta -tsv primers.tsv -prefix prefix`
- memwater:
  - This is more of an library. I took it from my alnSeq
    side project and fixed a few bugs. It is an waterman
    that operates in linear memory and takes close to as
    much time as an traditional waterman (slow, at least 
    10x slower then the complete striped smith waterman
    library).
  - It is designed for CPUs with branch prediction and so,
    uses branchless operations instead of branched.
  - It only returns the scores and the start and end
    coordinates of the alignment.
- oldCode: This has old code I am not quite ready to throw
  away yet. I may use it later or in another project.
  Some of it was never finished, other parts of it no
  longer work do to an change in my coding style.


# freezeTB as an bash script

A better picture of how freezeTB works might be gotten by
  showing how it would look if I used an bash script. This
  is for default settings only, since it is an example and
  I want to keep the script more simple. This has not been
  tested.

```
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - declare variables
#   o sec02:
#     - get user input
#   o sec03:
#     - map and fiter reads
#   o sec04:
#     - get read stats, find AMRs in reads, find lineages
#       in reads, and build the consensuses
#   o sec05:
#     - for consensuses find AMRs and find lineages,
#   o sec06:
#     - build the read depth and coverage graphs
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - declare variables
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# these variables can be changed by the user

readsStr="";
prefixStr="out";

graphExtStr="tiff";

graphBl="FALSE";
adjBl="FALSE";
maskBl="FALSE";

# these variables are not changed by the user

samStr="$prefixStr-map";
unfiltSamStr="";
dbDirStr="/usr/local/share/freezeTBFiles";

helpStr="freezeTB.sh -fq reads.fastq
  - Checks for AMRs, spoligotypes, and MIUR-VNTR lineages,
    in ONT sequenced tuberculosis samples. Also builds an
    set of consensus genomes.
Input:
  -fq reads.fastq: [Required]
    o fastq file with tuberculosis reads to check
  -prefix good-name: [$prefixStr]
    o what to name output files
  -coord-adj: [$adjBl]
    o makes freezeTB.sh adjust mapping coordinates. This
      means you mapped to an set of genes instead of an
      reference.
  -prim-mask: [$maskBl]
    o mask any primer coordinates in an gene
  -graph: [$graphBl]
    o makes freezeTB.sh output an set of read depth and
      read coverage graphs
  -ext: [$extStr]
    o file extension to save graphs as
Output:
  - a lot of files named after -prefix.
" 

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [[ #$ -gt 0 ]]; do
# Loop: get user input
   case $1 in
      -fq) readsStr="$2"; shift;;
      -prefix) prefixStr="$2"; shift;;
      -graph) graphBl="TRUE";;
      -coord-adj) adjBl="TRUE";;
      -prim-mask) maskBl="TRUE";;
      -ext) graphExtStr="$2"; graphBl="TRUE";;
      -h) printf "%s\n" "$helpStr"; exit;;
      *) printf "%s not recognized\n" "$1"; exit;;
   esac

   shift;
done # Loop: get user input

# I would be doing file checks here if this were an real
#  script. I also would likely allow more input options

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - map and fiter reads
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

minimap2 \
    -a \
    -x map-ont \
     "$dbStr/NC000962.fa" \
     "$readsStr"\
  > "$samStr.sam";

# This program is an optional program in the gui only.
#   The logic is that for amplicons it is faster to map
#   to an small set of genes then it is to map to an
#   reference. This uses the genes coordinates to adjust
#   to the reference coordinates. Any reference gene not
#   in the coordinates file is removed. This includes
#   mapped genes, like those 16 sRNA genes that mapped to
#   and *E. coli* gene or did not map
if [[ "$adjBl" == "TRUE" ]]; then
# If: If I need to adjust coordinates
   adjCoords \
      -coords "$dbDirStr/genes-NC000962.fa" \
      -ref "$dbDirStr/NC000962.3" \
      -sam "$samStr.sam" \
      > "$prefixStr-map-adj.sam";

   samStr="$prefixStr-map-adj";
fi # If: If I need to adjust coordinates

# filter out low quality reads
filtsam \
    -F 4 \
    -F 256 \
    -F 2048 \
    -trim \
    -min-aln-len 200 \
    -min-mapq 15 \
    -min-mean-q 7 \
    -min-median-q 7 \
    -sam "$samStr.sam" \
    -out "$samStr-filt.sam";

samStr="$samStr-filt";

if [[ "$maskPrimBl" == "TRUE" ]]; then
# If: primer masking was requested
   primMask \
      -sam "$samStr" \
      -prim "$dbDirStr/mask.tsv" \
      -out "$samStr-mask.sam";

   unfiltSamStr="$samStr";
   samStr="$samStr-mask.sam";
else
   unfiltSamStr="$samStr";
   samStr="$samStr.sam";
fi # If: primer masking was requested

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - get read stats, find AMRs in reads, find lineages
#     in reads, and build the consensuses
#   - these steps could be run in parallel
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# unfiltered read depth
ampDepth \
    -gene-tbl "$dbDirStr/gene-tbl.tsv" \
    -flag "unfiltered" \
    -sam "$unfiltSamStr" \
    -out "$prefixStr-stats.tsv";

# filtered read depth (tail is to remove the header)
ampDepth \
    -gene-tbl "$dbDirStr/gene-tbl.tsv" \
    -flag "filtered" \
    -sam "$samStr" |
  tail -n+1 \
  >> "$prefixStr-stats.tsv";

# build the consensuses
tbCon \
    -sam "$samStr" \
    -out-tsv "$prefixStr-consnesuses.tsv" \
    -out "$prefixStr-consensuses.sam";

# find AMRs
tbAmr \
   -amr-tbl "$dbDirStr/who-2023.tsv" \
   -sam "$samStr" \
   -id-file "$prefixStr-read-amrsIds.tsv" \
   -out "$prefixStr-amrs.tsv";

# find the MIRU-VNTR lineages
tbMiru \
    -miru-tbl "$dbDirStr/miruTbl.tsv" \
    -sam "$samStr" \
    out-tbl "$prefixStr-read-miru.tsv";

# find the spoligotype
tbSpoligo \
    -read-frag \
    -spoligo "$dbDirStr/spoliogtype-seq.fa" \
    -db "$dbDirStr/spoligo-lineages.csv" \
    -sam "$samStr" \
    -out "$prefixStr-read-spoligo.tsv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec05:
#   - for consensuses find AMRs and find lineages,
#   - these steps could be run in parallel
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# find the AMRs
tbAmr \
   -amr-tbl freezeTBFiles/who-2023.tsv \
    -sam "$prefixStr-consensuses.sam" \
    -out "$prefixStr-con-amrs.tsv";

# find the MIRU-VNTR lineage
tbMiru \
    -miru-tbl "$dbDirStr/miruTbl.tsv" \
    -sam "$prefixStr-consensuses.sam" \
    out-tbl "$prefixStr-con-miru.tsv";

# find the spoligotype
tbSpoligo \
    -con-frag \
    -spoligo "$dbDirStr/spoliogtype-seq.fa" \
    -db "$dbDirStr/spoligo-lineages.csv" \
    -sam "$prefixStr-consensuses.sam" \
    -out "$prefixStr-con-spoligo.tsv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec06:
#   - build the read depth and coverage graphs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# This is done automatically in the gui version, but not
#   the command line version
if [[ "$graphBl" == "TRUE" ]]; then
# If: graphs were wanted
   # For graphing (wait till ampDepth finishes)
   Rscript graphAmpDepth.r \
      -stats $prefixStr-stats.tsv" \
      -who "$dbDirStr/who-2023.tsv" \
      -ext "$graphExtStr" \
      -prefix "$prefixStr";
fi # If: graphs were wanted
```

