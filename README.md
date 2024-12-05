# Use

freezeTb is a program that detects AMR resistance in
  Nanopore sequence tuberculosis samples. It also outputs
  an set of consensus fragments (as an sam file) and
  checks for spoligotypes and MIRU-VNTR lineages. However,
  unlike TBProfiler, freezeTB does not check for
  genes that need to be functional for resistance, nor
  does it output a nice report.

Currently freezeTb uses a older WHO 2023 catalog that has
  had all grade 3, 4, and 5 variants removed.

- Good Alternatives to freezeTB are
- TBProfiler [https://github.com/jodyphelan/TBProfiler](
   https://github.com/jodyphelan/TBProfiler)
- ONTs wf-tb-amr (can be installed through epi2me)
  [https://github.com/epi2me-labs/wf-tb-amr](
   https://github.com/epi2me-labs/wf-tb-amr)

- Some missing items (not all) from freezeTB
  - the sample
  - the sequencer
  - the lab tech
  - the bioinformatition
  - the basecaller
  - a read mapper
  - ability to cause mayhem and steal every ones left shoe

PS: thank or blame the CO-PI for the bad jokes. She asked
  for more Harry Potter jokes.

# License:

freezeTB does include databases from other repositories
  and complied code (minimap2) from other repositories. In
  those cases the licensing will be under the owners
  repository.

Here are the order of merlin winners:

- Taken from other sources:
  - files in freezeTBFiles
    - amrDb.tsv is the who 2023 catalog (converted)
    - spoligo-lineages.csv from TBProfiler
    - NC000962.fa and genes-NC000962.fa were from Genbank
      - also in benchmark as tbdb.fasta
    - miruTbl.tsv is the modified MIRU-VNTRU table from
      MIRUReader (github)
    - spoligo-spacers.fa were from a spoligotyping program
      on github (can not remember which one)
  - minimap2 (MIT) window binaries compiled from
    [https://github.com/lh3/minimap2](
     https://github.com/lh3/minimap2)
  - tcltk (might need compiled version for windows)
  - luaFilters in slidesAndWriting
    - [https://github.com/pandoc-ext](
       https://github.com/pandoc-ext)
       - pretty-urls, pagebreak, and abstract-section
    - [https://github.com/pandoc/lua-filters](
       https://github.com/pandoc/lua-filters)

The stuff unique to freezeTB is under an dual license. The
  primary license is unlicense. However, not all
  companies/countries like or will even respect the
  unlicense. So, in those cases or cases were the
  unlicense is not wanted/allowed, freezeTB is under the
  MIT license.

- Unique to freezeTB:
  - everything in programs
    - This is the reason Norbert, I mean freezeTB can fly
      on Windows.
  - everything in scripts
  - some of the clip art in diagarms
    - the rest is from openclipart.org (public domain)
  - the terrible guides (guides folder)
  - the scripts in benchmark
  - everything in oldCode
  - the slidshows and write up in slidesAndWriting
    - except luaFilters and nbib files

# Install

## My starwars joke

- The installation menence; your start to the dark side
  - step one: anger; why are there so many requirments
  - step two: hatered; why will it not install on OS X
  - step three: fear; must I use Ubuntu instead of X
  - step four: chaos; repeat steps one to four for the
    next program
  - step five: disaster; install Ubuntu

- The light side; a cycle of hardship
  - step one: peace; do I really need this program
  - step two: knowledge; maybe there is an alternative
  - step three: serenity; the alternative will install
  - step four: harmony; nope, but I really did not need
    that program
  - step five: disaster; I did need it, repeat steps
    one through four

## Requirements Installation (Linux/Mac)

For Linux I am assuming you are using a Debian based
  distrabution, such as Ubuntu.

1. For **Mac** homebrew [https://brew.sh/](https://brew.sh/)
   - website: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
2. minimap2
   - for command line version you only need a sam file,
     so minimap2 can be replaced.
   - For **Linux** `sudo apt-get install minimap2;`
   - For **Mac** `brew install minimap2;`
3. R (for graphs)
   - For **Linux** `sudo apt-get install r-base;`
   - For **Mac** `brew install r`
4. tcltk
   - **Linux** `sudo apt-get install tcl-dev tk-dev`
   - **Mac** `brew install xquartz tcl-tk`

## Linux:

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
sudo make -f mkfile.unix install
```

## Mac

```
if [[ ! -d "/usr/local/bin" ]]; then
   sudo mkdir -p "/usr/local/bin";
fi

if [[ ! -d "/usr/local/share" ]]; then
   sudo mkdir -p "/usr/local/share";
fi

cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
sudo make -f mkfile.unix install
```
The easy way is `brew install minimap2`.

### Mac minimap2 from source

If yo do not want to use the brew install for minimap2
  then you can install it from source. Linux is very
  similar, except remove the `arm_neon=1` and `aarch64=1`
  flags from the `make` command.

```
if [[ ! -d "/usr/local/bin" ]]; then
   sudo mkdir -p "/usr/local/bin";
fi

if [[ ! -d "/usr/local/share" ]]; then
   sudo mkdir -p "/usr/local/share";
fi

cd ~/Downloads
git clone https://github.com/lh3/minimap2
cd minimap2
make arm_neon=1 aarch64=1
sudo cp minimap2 /usr/local/bin
sudo chmod a+x /usr/local/bin/minimap2
```

## Windows

### Easy method

Use Unix, there is no hope. Well, there would be if I had
  precompiled binaries.

The Rscript gui works, but the primrary GUI needs the
  Rscript detection step (so non-working). Also, I need to
  take time to figure out how to get tcltk setup on
  windows. Then I can give instructions.

### Windoows Actuall method (source)

This is not the method your are looking for

1. Install visual studio or at least the developer console
   from visual studio
2. Make a freezeTB directory in "C:\Program Files"
3. Download freezeTB from git hub and unzip in Downloads
4. Copy the freezeTBFiles directory to Documents (or public
   Documents ["C:\Public\Documents"])
5. open the developer console
   (visual studio->view->developer console) and
   do `cd $HOME$/Downloads/freezeTB`.
6. compile freezeTB with `nmake /F mkfile.win`, then close
   the developer console
7. in the freezeTB folder copy the freezeTB.exe in
   "programs->freezeTBSrc" to the freezeTB folder in
    "C:\Program Files"
9. copy the minimap2.exe (minimap2) file minimap2Windows
   to your freezeTB folder in "C:\Program fIles"
   - or compile minimap2 and copy the .exe file
10. copy the freezeTBGui.Rscript (freezeTBGui) file in
   scripts to your desktop or were you want it.
11. Make sure R is installed or if not install R or
    Rstudio
   [https://cran.r-project.org/bin/windows/base/](
    https://cran.r-project.org/bin/windows/base/)
12. right click freezeTBGui.Rscript and select
    "open with", then "more programs", then "R",
    "R <version number>", "bin", "Rscript"

![
  picture showing how to open developer console
](windows10devConsole.png)

### Windows Minimap2 (from source)

I include a minimap2.exe compiled binary in winBin.
  So, you do not need this. However, here are the
  steps if you want a more recent version of minimap2
  or if you are on an ARM CPU.

First install Cygwin using the setup-x86_64.exe (is a
  link):

[https://cygwin.com/install.html](
  https://cygwin.com/install.html)

When at "Select Packages" menu for Cygwin install:

- Search for "make" then select the package named make.
  - make sure you are looking at the "full" and not the
    "pending list"
  - change "Skip" to make verison number with dropdown
    menu (choose most recent non-testing version)
  - do not install gcc (it can not statically link)
- mingw64-x86_64-gcc-core
- mingw64-x86_64-zlib

Download minimap2 to your downloads folder

[https://github.com/lh3/minimap2](
 https://github.com/lh3/minimap2)

Unzip the minimap2-master folder (right click->extract)

Open a Cygwin terminal (run Cygwin). Then move into
  minimap2's directory 
  (`cd c:/Users/<user name>/Downloads/minimap2-master/minimap2-master`).
  - for me `cd c:/Users/Jeremy/Downloads/minimap2-master/minimap2-master`

Build minimap2
  with `make CC=x86_64-w64-mingw32-gcc CFLAGS=--static`.
  You will need --static here to avoid needing .dll files.

Copy minimap2 to your freezeTB install location or into
  your PATH.

`cp minimap2.exe C:/Users/<user name>/Downloads/freezeTB/winBin`

# Run

## GUI

If it is guiFreezeTB then just double click, if
  freezeTBGui.Rscript then in a terminal
  type `freezeTBGui.Rscript`. On Linux (not Mac) you can
  likely double click the `freezeTBGui.Rscript` file
  after doing `chmod a+x freezeTBGui.Rscript` to run.

It is a bit odd that double clicking will work on Linux,
  but not Mac. From what I have seen, I think this is due
  to Mac trying to second guess the shebang line for my R
  scripts and assumes you really wanted a R session.

For windows, if you set up Windows to always open up
  a `.Rscript` file with `Rscript` then you just need to
  double click freezeTBGui.Rscript. If not, then get
  open "freezeTBGui" with "R studio" and hit run (the
  green arrow). Or if you do not want R studio you can
  open a terminal and cd to the freezeTBGui
  script `cd %ProgramFiles%\freezeTB` location and then
  call Rscript to run the
  gui `%ProgramFiles%\R\<Rversion>\bin\Rscript %ProgramFiles/freezeTB/freezeTBGui.Rscript`.

Ye Windows is a pain. So, set the default open with
  program for a `.Rscript` file to `Rscript`. Makes life
  so much easier.

## Command line:

I have setup freezeTB to auto detect database files. It
  looks for the freezeTBFiles databases in the working
  directory and in a few other locations. So, you should 
  need to provide any databases unless you want different
  settings.

For Linux freezeTB will also search for the freezeTBFiles
  directory (has databases) in `~/Documents` frist and
  then in `/usr/local/share`.

For Windows freezeTB will also search for the
  freezeTBFiles directory (has databases)
  in `%HOME%\Documents` (user Documents) frist and
  then in `%PUBLIC%\Documents` (Public Documents).

### Mac/Linux:

Get the help message with `freezeTB -h`. It is long so
  I would recomend doing `freezeTB -h | less` on Linux.

Map reads to the reference using
  minimap2: `minimap2 -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq > reads.sam`.

Then run
  freezeTB: `freezeTB -sam reads.sam -prefix goodName`.

To get graphs
  do: `graphAmpDepth.r -stats goodName-depths.tsv -who /usr/local/share/freezeTBFiles/amrDb.tsv -prefix goodName`

### Windows:

Windows command line is painfull, but yes you can use it.
  This should work, so long as carriage returns do not
  become an issue. If have errors or inifinite loops, let
  me know.

You can get the help message
  with: `%ProgramFiles%\freezeTB\freezeTB.exe -h`

Map reads to the reference using
  minimap2: `%ProgramFiles%/freezeTB/minimap2.exe -a %PUBLIC%\Documents\freezeTBFiles\NC000962.fa reads.fastq > reads.sam`.

Map reads to the reference using
  freezeTB: `%ProgramFiles%\freezeTB\freezeTB.exe -sam reads.sam -prefix goodName`

Buld graphs (if wanted)
  do: `%ProgramFiles%\R\<R version>\bin\Rscript %ProgramFiles%\freezeTB\graphAmdDepth.r -stats goodName-depths.tsv -who %PUBLIC%\Documents\freezeTBFiles\amrDb.tsv -prefix goodName`

# How it works

![
  Clip are flow digram showing how freezeTB works. Tempted
  to make an ascii digram with dita.
](diagrams/freezeTb-diagram.svg)

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
    - ampicon sequencees (map reads to with miniamp2)

## Older progams (some functional):

Chapter four: the undead awaken

- freezeTBGui.Rscript: gui built in R using tcltk, was
  replaced with guiFreezeTB, but still works
- outputGui.Rscript: runs only the output menu displayed
  by freezeTBGui.Rscript
- gui-run.tcl: script used to build guiFreezeTB gui
  - will run if use `wish` from tcltk
- oldGraphAmpDepth.r is older version of graphAmpDepth
  that used ggplot2
- oldCode: old code I am not quite ready to throw
  away yet (or am to lazy to clean)

## Really random stuff:

Chapter five: the GUI feind

In programs/guiFreezeTB there is a script called gui.tcl.
  This is the GUI script used in guiFreezeTB. However,
  you need to use rmBlocks from my `build` repository to
  get it to run with wish (`rmBlocks < gui.tcl | wish`).

The main reason for gui.tcl is that it allows you to edit
  the gui for guiFreezeTB and view it in reall time. You
  can the integrate it into the guiFreezeTB code with the
  addScript.sh script `bash addScript.sh`. You would then
  do `make -f mkfile.unix` or `make -f mkfile.mac` to
  rebuild guiFreezeTB with your own gui.

To run addScript.sh you will need `scriptToCStr` from
  my `build` repository.

On the surface it sounds good, until you dig in. Then
  comes the nightmere of my documentation (adds a lot of
  lines) and just how many lines this GUI takes (1k with
  everything merged and comments removed). You will also
  need to understand my styntax for rmBlocks.

The rmBlocks rules:

Items in strings `""` are considered literal.

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

Code unique to the tcltk script is between
  two `#>>>script<<<#` blocks. This is code such as
  default settings variables and the run freezeTB command.
  These are defined in the actual C code.

```
#>>>script<<<#
only seen in the script, removed by scriptToCStr
#>>>script<<<#
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

#>>>script<<<#
End of comment block ---#
#>>>script<<<#
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
