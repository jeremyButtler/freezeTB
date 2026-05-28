# Programs included with freezeTB

Introduction to the monster book of monsters:

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it out separately.

Some of the programs apply to more then just TB. In these
  cases the program can be found in my bioTools
  repository.

For unix several of the programs are installed in the
  install step, however for windows you will have to copy
  each one.

## freezeTB core:

Chapter one: monsters that consume all hope

- freezeTB: core program for freezeTB
- ftbTclTk: TclTK gui that wraps freezeTB
- ftbRaylib: Raylib gui that wraps freezeTB

## Modules:

Chapter two: here be dragons

- tbAmr: AMR detection
  - move to bioTools repository and renamed to getAmr
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
  - poor insertion handeling (often deletes)
- getLin: generalized lineage system (not spoligotyping
  or MIRU) used in freezeTB (in bioTools repository)

## Other, less useful programs:

Chapter three: do not underestimate the pixies

- addAmr: adds an drug resistance mutation to a tbAmr
  (freezeTB) AMR databse (also in bioTools repository)
- whoToAmr: converts the WHO 2023 catalog to tbAmr format
- swapDbRef: is a program to switch out references in the
  reference database.
  - only adjusts coordinates by reference genome, does not
    check for frame shifts or other problems
- mkMiurTbl: takes in an miru table and adds the reference
  coordinates for tbMiru into the table header
- rayWidg (in ftbRaylibSrc) is the widget organization
  system for the raylib GUI
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
- scripts/readLenGraph.sh:
  - makes a read length graph out of reads that aligned
    to the reference (includess N50, mean length, and
    median length).
  - you will need filtsam and minimap2 to use this script
- scripts/gui-run.sh:
  - this is a tcltk script that runs freezeTB without
    integration (calls freezeTB separatly). It has an
    issue with using mapRead, but otherwise works.
- scripts/ftbBatch:
  - runs freezeTB in batch mode on `fastq_pass` directory
    output by ONT basecallers
  - input `ftbBatch.sh output-name /path/to/fastq_pass/`
    - or for multiple runs:
      `ftbBatch.sh 1st 1st/fastq_pass/ 2nd 2nd/fastq_pass/ ...`
  - output:
    - amr-results.tsv file with all detected AMRs in all
      barcodes
    - spol-results.tsv file with all detected spoligotypes
      in all barcodes
    - freezeTB results saved to each fastq_pass's input
      prefix

## Future programs:

Chapter four: monsters from beyond

Currently none

## Older progams (some functional):

Chapter five: the undead awaken

These are so out of date it is not worth using. Still,
  they are examples of tcltk in R.

- freezeTBGui.Rscript: gui built in R using tcltk, was
  replaced with guiFreezeTB, but might work (no idea)
- outputGui.Rscript: runs only the output menu displayed
  by freezeTBGui.Rscript (should work)
- minGui.r: minimal R tcltk example (just runs minimap2)
  - this may not work anymore. I added some stuff for
    file detection and it may have messed it up

## Really random stuff:

Chapter six: the GUI fiend

There are two GUI version of freezeTB. For ftbRay, you
  can not edit the GUI (everything is in C). For ftbTclTk
  you can, because it uses a Tcl script for the GUI. This
  script is `gui.tcl` and is located
  in `programs/ftbTclTkSrc`. In order to use `gui.tcl` 
  you will have to convert it from its current form to a
  true Tcl script with rmBlocks from my `build`. After
  the conversion you can run it
  with (`rmBlocks -file gui.tcl | wish`). You will also
  need the command line freezeTB (installed by default if
  did default install).

The main reason for gui.tcl is that it it is needed for
  TclTk. You can then convert it to the primary freezeTB
  gui script
  using `rmBlocks -no-script -file gui.tcl > gui-FTB.tcl`.
  You can then copy `gui-FTB.tcl` to your freezeTBFiles
  location to get the new gui.

On the surface it sounds good, until you dig in. Then
  comes the nightmare of my documentation (adds a lot of
  lines) and just how many lines this GUI takes. You will
  also need to understand my syntax for rmBlocks.

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

Chapter seven: make your own monster

To give a a better picture of how freezeTB works I made
  a shell script that should do the same task. This
  is for default settings only, since it is an example and
  I want to keep the script more simple. Also, this has
  not been tested.

The only thing you can not make is the same graphs. These
  have been built into freezeTB.

Finally, the shell script will be slower, since these
  programs are built into freezeTB.

You will need some programs from my bioTools repo(
  [https://github.com/jeremybuttler/bioTools](
   https://github.com/jeremybuttler/bioTools)). These
  include ampDepth, filtsam, getAmr, getLin, maskPrim,
  edClust, tbCon, rmHomo, and mapRead.

```
#!/usr/bin/sh
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
maskBl="FALSE";
rmHomoBl="FALSE";

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
  -prim-mask: [$maskBl]
    o mask any primer coordinates in an gene
  -graph: [$graphBl]
    o makes freezeTB.sh output an set of read depth and
      read coverage graphs
  -ext: [$extStr]
    o file extension to save graphs as
  -rmHomo: [$rmHomoBl]
    o remove homopolymers
Output:
  - a lot of files named after -prefix.
" 

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [ #$ -gt 0 ]; do
# Loop: get user input
   if [ "$1" = "-fq" ]; then
      shift;
      readsStr="$1";
   elif [ "$1" = "-prefix" ]; then
      shift;
      prefixStr="$1";
   elif [ "$1" = "-graph" ]; then
      graphBl="TRUE";
   elif [ "$1" = "-prim-mask" ]; then
      maskBl="TRUE";
   elif [ "$1" = "-ext" ]; then
      shift;
      graphExtStr="$1";
      graphBl="TRUE";
   elif [ "$1" = "-rmHomo" ]; then
      rmHomoeBl="TRUE";
   elif [ "$1" = "-h" ]; then
      printf "%s\n" "$helpStr";
      exit;
   else
      printf "%s not recognized\n" "$1";
      exit;
   fi;

   shift;
done # Loop: get user input

# I would be doing file checks here if this were an real
#  script. I also would likely allow more input options

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - map and fiter reads
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [ "$mapStr" = "minimap2" ];
then
   minimap2 \
       -a \
       -x map-ont \
        "$dbStr/NC000962.fa" \
        "$readsStr"\
     > "$samStr.sam";
   minMapqSI = 15;
else
   mapRead \
       -ref "$dbStr/NC000962.fa" \
       "$readsStr" \
     > "$samStr.sam";
     # fallback read mapper
   minMapqSI = 0;
fi;

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
    -sam "$samStr.sam";

samStr="$samStr-filt";

if [ "$rmHomoBl" = "TRUE" ];
then
  rmHomo \
     -ref "$dbStr/NC000962 \
     -sam "$samStr.sam" \
     -out "$samStr-filt-rm.sam"; # also clean up indels
  samStr="$samStr-filt-rm";
fi;


if [ "$maskPrimBl" = "TRUE" ]; then
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
fi; # If: primer masking was requested

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - get read stats, find AMRs in reads, find lineages
#     in reads, and build the consensuses
#   - these steps could be run in parallel
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# filtered read depth (tail is to remove the header)
ampDepth \
    -gene-tbl "$dbDirStr/gene-tbl.tsv" \
    -flag "filtered" \
    -sam "$samStr" \
  > "$prefixStr-stats.tsv";

# find AMRs
getAmr \
   -amr-tbl "$dbDirStr/amrDb.tsv" \
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

# find species using the hsp65 gene
getLin \
    -simple "$dbDirStr/hsp65-db-simple.tsv" \
    -complex "$dbDirStr/hsp65-db-complex.tsv" \
    -sam "$samStr" \
    -id "$prefixStr" \
    -pmode-read \
    -out "$prefixStr-read-hsp65.tsv";

# build the consensuses
if [ "$mixedInfectBl" -lt 1 ]; then
   tbCon \
       -sam "$samStr" \
       -out-tsv "$prefixStr-consnesuses.tsv" \
       -min-mapq "$minMapSI" \
       -out "$prefixStr-cons.sam";
else
   edClust \
       -sam "$samStr" \
       -min-mapq "$minMapSI" \
       -prefix "$prefixStr" \
fi;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec05:
#   - for consensuses find AMRs and find lineages,
#   - these steps could be run in parallel
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# find the AMRs
tbAmr \
   -amr-tbl freezeTBFiles/amrDb.tsv \
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

# find species using the hsp65 gene
getLin \
    -simple "$dbDirStr/hsp65-db-simple.tsv" \
    -complex "$dbDirStr/hsp65-db-complex.tsv" \
    -sam "$prefixStr-cons.sam" \
    -id "$prefixStr" \
    -pmode-genome \
    -out "$prefixStr-con-hsp65.tsv";

# output the fasta file
filtsam \
    -out-fasta \
    -sam "$prefixStr-cons.sam" \
    -out "$prefixStr-cons.fa";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec06:
#   - build the read depth and coverage graphs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [ "$graphBl" = "TRUE" ]; then
# If: graphs were wanted
   # For graphing (wait till ampDepth finishes)
   Rscript graphAmpDepth.r \
      -stats $prefixStr-stats.tsv" \
      -who "$dbDirStr/amrDb.tsv" \
      -ext "$graphExtStr" \
      -amrs ""$prefixStr-amrs.tsv" \
      -prefix "$prefixStr";

   # -min-len not used, but controls min amplicon length;
   # default is 50 (think no longer used)
fi; # If: graphs were wanted
```
