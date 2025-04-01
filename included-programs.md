# Programs included with freezeTB

Introduction to the monster book of monsters:

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it out separately.

For unix several of the programs are installed in the
  install step, however for windows you will have to copy
  each one.

## freezeTB core:

Chapter one: monsters that consume all hope

- freezeTB: core program for freezeTB
- guiFreezeTB: primrary gui that wraps freezeTB
- graphAmpDepth.r: Rscript that makes graphs for freezeTB

## Modules:

Chapter two: here be dragons

- tbAmr: AMR detection
- tbMiru: detects MIRU-VNTR lineages in reads
  - can run on fastx files (slower)
  - there are some side awk scripts (not installed) for
    helping with the read count tables
    - awk -f `getPercMiruTbl.awk miru-tbl.tsv` prints
      out only percentage of mapped  reads for each primer
    - awk -f `getHitMiruTbl.awk miru-tbl.tsv` prints
      out the number of reads supporting each primer
- tbSpol: does spoligotyping
  - can run on fastx files (slower)

## Other, less useful programs:

Chapter three: do not underestimate the pixies

- whoToAmr: converts the WHO 2023 catalog to tbAmr format
- swapDbRef: is a program to switch out references in the
  reference database.
  - only adjusts coordinates by reference genome, does not
    check for frame shifts or other problems
- mkMiurTbl: takes in an miru table and adds the reference
  coordinates for tbMiru into the table header
- scripts/catPrimers.sh: bash script that uses minimap2
  and awk to merge amplicons (not installed)
  - also prints out the amplicon and primer coordinates
    needed for maskPrim
  - `bash scripts/catPrimers.sh -ref ref.fasta -tsv primers.tsv -prefix prefix`
- scripts/primToSeq.sh:
  - needs primFind from my bioTools repository
  - extracts amplicons using primer sequences
    - creates: amp_coords.tsv
      - coordiates adjustent step (adjCoords)
      - amplicon read depth/coverage tsv (ampDepth)
    - creates: amp_mask.tsv
      - for masking primer sitesj (maskPrim)
    - creates: amp_seq.fa
      - ampicon sequencees (map reads to with miniamp2)
- scripts/getSeqByCoord.sh:
  - extracts amplicon sequences by coordinate
  - creates: amp_seq.fa
    - ampicon sequencees (map reads to with minimap2)

## Older progams (some functional):

Chapter four: the undead awaken

- freezeTBGui.Rscript: gui built in R using tcltk, was
  replaced with guiFreezeTB, but might work (no idea)
- outputGui.Rscript: runs only the output menu displayed
  by freezeTBGui.Rscript (should work)
- oldGraphAmpDepth.r is older version of graphAmpDepth
  that used ggplot2

## Really random stuff:

Chapter five: the GUI fiend

In programs/guiFreezeTB there is a script called gui.tcl.
  This is the GUI script used in guiFreezeTB. However,
  you need to use rmBlocks from my `build` repository to
  get it to run with wish
  (`rmBlocks -file gui.tcl | wish`). You will also need
  the command line freezeTB (installed by default if
  did default install). However, it does run on its own
  with wish.

The main reason for gui.tcl is that it allows you to edit
  the gui for guiFreezeTB and view it in reall time. You
  can then convert it to the freezeTB gui script using
  `rmBlocks -no-script -file gui.tcl > gui-FTB.tcl`. You
  can then copy `gui-FTB.tcl` to your freezeTBFiles
  location to get the new gui.

On the surface it sounds good, until you dig in. Then
  comes the nightmare of my documentation (adds a lot of
  lines) and just how many lines this GUI takes (1k with
  everything merged and comments removed). You will also
  need to understand my syntax for rmBlocks.

I also need to release rmBlocks. It is currently part of a
  larger, side project I am working on. So, I do not plan
  on releasing it any time soon, but if you need it let me
  know and I will make the project public.

The rmBlocks rules:

Items in strings `""` or `''` are considered literal.

Blocks are distinguished by `---` and are only 1 level
  deep.

```
--- start of a block
   stuff to be merged goes here
     all lines in blocks are merged into a single line
     in rmBlocks

   # also comments in blocks are removed

   Each block start/end is percisly three dashes, so four
     dashes (----) is not a block start or end.
--- ; # end of a block
```

Code unique to the tcltk script is between `#>>>script`
  and `#<<<script` blocks. This is code such as default
  settings variables and the run freezeTB command.  These
  are defined in the actual C code.

```
#>>>script
only seen in the script, removed by scriptToCStr
#<<<script
```

I also have one point were I need to remove code unique to
  a C program from the script. To do this I use the block
  comment from rmBlock `#--- ---#` and put them in script
  only blocks.

```
#>>>script<<<#
script freezeTB command goes here. It is a call to the
  freezeTB program.

#--- comment block to remove C call
#>>>script<<<#

C freezeTB call (internal function) goes here and is
  removed with rmBlocks. However, scriptToCStr will remove
  the comment blocks, enabling it for the C code.

#>>>script
End of comment block ---#
#<<<script
```

# freezeTB as an bash script

Chapter six: make your own monster

A better picture of how freezeTB works might be gotten by
  showing how it would look if I used an bash script. This
  is for default settings only, since it is an example and
  I want to keep the script more simple. This has not been
  tested.

You will need some programs from my bioTools repo(
  [https://github.com/jeremybuttler/bioTools](
   https://github.com/jeremybuttler/bioTools)). These
  include ampDepth, filtsam, maskPrim, edClust, and tbCon.

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
   maskPrim \
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
if [[ "$mixedInfectBl" -lt 1 ]]; then
   tbCon \
       -sam "$samStr" \
       -out-tsv "$prefixStr-consnesuses.tsv" \
       -out "$prefixStr-cons.sam";
else
   edClust \
       -sam "$samStr" \
       -prefix "$prefixStr" \
fi

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
tbSpol \
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
    -sam "$prefixStr-cons.sam" \
    -out "$prefixStr-con-amrs.tsv";

# find the MIRU-VNTR lineage
tbMiru \
    -miru-tbl "$dbDirStr/miruTbl.tsv" \
    -sam "$prefixStr-cons.sam" \
    out-tbl "$prefixStr-con-miru.tsv";

# find the spoligotype
tbSpol \
    -con-frag \
    -spoligo "$dbDirStr/spoliogtype-seq.fa" \
    -db "$dbDirStr/spoligo-lineages.csv" \
    -sam "$prefixStr-cons.sam" \
    -out "$prefixStr-con-spoligo.tsv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec06:
#   - build the read depth and coverage graphs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$graphBl" == "TRUE" ]]; then
# If: graphs were wanted
   # For graphing (wait till ampDepth finishes)
   Rscript graphAmpDepth.r \
      -stats $prefixStr-stats.tsv" \
      -who "$dbDirStr/who-2023.tsv" \
      -ext "$graphExtStr" \
      -prefix "$prefixStr";

   # -min-len not used, but controls min amplicon length;
   # default is 50 (think no longer used)
fi # If: graphs were wanted
```
