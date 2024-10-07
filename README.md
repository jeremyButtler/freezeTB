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

Alternatives to freezeTB are TBProfiler
  [https://github.com/jodyphelan/TBProfiler](
   https://github.com/jodyphelan/TBProfiler)
  and ONTs wf-tb-amr (can be installed through epi2me)
  [https://github.com/epi2me-labs/wf-tb-amr](
   https://github.com/epi2me-labs/wf-tb-amr). Both are
  good alternatives.

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
    - spoligo-lineages from TBProfiler
    - NC000962.fa and genes-NC000962.fa were from Genbank
      - also in benchmark as tbdb.fasta
    - miruTbl.tsv is the modified MIRU-VNTRU table from
      MIRUReader (github)
    - spoligo-spacers.fa were from a spoligotyping program
      on github (can not remember which one)
    - mask.tsv and coords.tsv is unique to freezeTB
  - minimap2 (MIT) binaries from
    [https://github.com/lh3/minimap2](
     https://github.com/lh3/minimap2)
  - luaFilters in slidesAndWriting
    - [https://github.com/pandoc-ext](
       https://github.com/pandoc-ext)
       - pretty-urls, pagebreak, and abstract-section
    - [https://github.com/pandoc/lua-filters](
       https://github.com/pandoc/lua-filters)

The stuff unique to freezeTB is under an dual license. The
  primary license is public domain. However, we know that
  not all companies/countries like or will even respect
  the public domain. So, in cases were public domain gets
  in the way, such as being inconvenient, undesired, or
  not allowed, freezeTB is under the MIT license.

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

# Requirements 

1. A read mapper that outputs a sam file
   - Your choice for the command line version 
   - For the GUI you will need minimap2, which can be
     often installed by your package manager (for Mac
     you can use homebrew)
2. R + several R libraries; GUI only
   - ggplot2
   - viridisLite (often installed with ggplot2)
   - data.table (often installed with ggplot2)
   - tcltk (base R) or tcltk2 (has tooltips)
     - note I have had some issues with tcltk not being
       compiled on some Macs. It seems the homebrew
       install of R works better.
   - svgLite (Optional; svgs output)
   - fs (Optional; Unix only)

# Install

## Easy way for unix

The easy way is to do a local install, which will only
  install freezeTB for you (no other user gets). The
  advanatage about a local install is that you do not need
  special permissions to install. The downside is that
  each user on your system needs a copy.

### Local (only you):

#### Setting up local envrioment

```
# make sure have local install and Downloads directory

if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

if [[ ! -d "$HOME/local/bin" ]]; then
   mkdir -p "$HOME/local/bin";
fi

# check if have path setup to local (add to path if not)

localBl="$(grep "$HOME/local/bin" <<< "$PATH")"

if [[ "$localBl" == "" ]]; then
   printf "export \"PATH=$HOME/local/bin:$PATH\"\n" >> $HOME/.bashrc;
fi

export "PATH=$HOME/local/bin:$PATH"

# you can remove $HOME/local/bin from your path by
# deleting the "export PATH=/home/<user name>/local/bin:"
# from your .bashrc file
```

#### Installing (locally):

```
cd $HOME/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
make -f mkfile.unix install-all PREFIX=$HOME/local/bin DB_PREFIX=$HOME/Documents
R
source("rDepends.r")
q("no")
```

### Global (everyone)

```
cd $HOME/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
if [[ ! -d "/usr/local/bin" ]]; then mkdir "/usr/local/bin"; fi
if [[ ! -d "/usr/local/share" ]]; then mkdir "/usr/local/share"; fi
make -f mkfile.unix
sudo make -f mkfile.unix install-all
sudo Rscript rDepnds.r
```

## Minimap2 the next step 

After this you will need minimap2 (I am assuming you do
  not have minimap2). I do have some pre-compiled
  binaries (programs), but a source install is easy on
  unix.

## Mac minimap2 local install

This is for the M chip series. If you have an old Mac with
  an intel/AMD CPU; then do the Linux install.

### Local:

```
cd $HOME/Downloads
git clone https://github.com/lh3/minimap2
cd minimap2
make arm_neon=1 aarch64=1
cp minimap2 $HOME/local/bin
chmod a+x $HOME/local/bin/minimap2
```

### Global:

```
cd $HOME/Downloads
git clone https://github.com/lh3/minimap2
cd minimap2
make arm_neon=1 aarch64=1
sudo cp minimap2 /usr/local/bin
sudo chmod a+x /usr/local/bin/minimap2
```

## Linux minimap2 local install

### Local:

```
cd $HOME/Downloads
git clone https://github.com/lh3/minimap2
cd minimap2
make
cp minimap2 $HOME/local/bin
chmod a+x $HOME/local/bin/minimap2
```

### Global:

```
cd $HOME/Downloads
git clone https://github.com/lh3/minimap2
cd minimap2
make
sudo cp minimap2 /usr/local/bin
sudo chmod a+x /usr/local/bin/minimap2
```

## Windows

### Easy method

There is no hope.

Use my pre-complied binaries. Likely out of date, but do
  work.

Make a freezeTB directory in "C:\Program Files". Then
  copy all the binaries and scripts into
  "C:\Program Files\freezeTB". After that copy the
  freezeTBFiles to your Documents folder (or public
  Documents ["C:\Public\Documents"]).

Make sure R is installed that the
  ggplot2 (`install.packages("ggplot2")`),
  viridisLite (`install.packates("viridisLite")`),
  svtLite (`install.packages("svtLite")`),
  data.tables (`install.packates("data.table")`),
  and tcltk2 (`install.packages("tcltk2")`) packages
  are installed. You can install by installing R studio
  or from Cran (see Windows from source).
  
At this copy the "freezeTBGui.r" script from scripts to
  were you want run freezeTB. Then open the file in R
  or Rstudio to run freezeTB. You can also set it is
  that the `.r` extension is always run by Rscript
  ("C:\Program Files\R\R-version\bin\Rscript"). That way
  double clicking freezeTBGui.r launches freezeTB.

### Windows from source

This is not the method your are looking for

#### Install R (GUI dependencies):

1. install R
   - [https://cran.r-project.org/bin/windows/base/](
      https://cran.r-project.org/bin/windows/base/)
   - follow instructions on exe.
2. Download freezeTB from github into Downloads then
   extract (right click->extract).
3. Move into freezeTB->scripts and double click
   rDepends.r
   - You may have to install these manually
     - big is ggplot2 (otherwise no graphs)
     - also install tcltk2 (nicer gui with tooltips)
   - select more apps->look for another app on this pc->R->R-version->bin->Rscript
   - at this point Rscript will run and install the GUI dependencies
       
#### freezeTB:

Get freezeTB from github and extract it. I saved it to
  Downloads.

you will need to use the mkfile.win to compile the source
  code.

1. Open a developer terminal with administrator
   privlages (for install step)
   - you can do this by installing visual studio
   - start menu go to visual studio (folder)->developer
2. `cd C:\Users\<user name>\Downloads\freezeTB`
   - or to were you put freezeTB
   - There my be two freezeTB directories
3. `nmake /F mkfile.win`
4. `nmake /F mkfile.win install-all`
5. `nmake /F mkfile.win clean`

![
  picture showing how to open developer console
](windows10devConsole.png)

## Minimap2 (from source)

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

Open a terminal and type `$HOME/local/bin/freezeTBGui.r`
 for a local install or `freezeTBGui.r` for a global
 install (or if it is in your path). Then fill in the
 values needed. This should be only fastq file if the
 databases were detected. You can select multiple fastq
 files at once.

## CLI (command line)

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
13. freezeTB prints out the AMRs that at least 5% of
   mapped reads supported
14. freezeTB prints out the MIRU-VNTR lineage table
    of read counts for the reads
15. freezeTB prints out the spacers detected with the
    the number of reads that supported each spacer
16. freezeTB then collapses the consensus
    - Split into fragments with 20x or greater read depth
    - Fragments less than 50x bases long are removed
    - Most supported snp/match/deletion selected
    - Snps/matches with less than 50% support are masked
    - Most supported insertion selected
    - Indels wit less than 70% support are removed
17. freezeTB finds the AMRs for the consensus
18. freezeTB finds the MIRU-VNTR lineages for consensus
19. freezeTB finds spoligotype for consensus
20. GUI only: freezeTB makes read depth and coverage
    graphs with R

# Programs included with freezeTB

Introduction to the monster book of monsters:

I have made the programs in freezeTb to be modular. This
  means that if you like a particular program, then you
  can compile it separately.

You can install freezeTB and the side modules
  with `make -f mkfile.OS install-modules` or
  all programs with `make -f mkfile.OS install-all`.

## freezeTB core:

Chapter one: monsters that consume all hope

- freezeTBGui.r: gui built in R using tcltk for freezeTB
- freezeTB: core program for freezeTB
- adjCoords: adjust coordinates in sam file using an
  list of genes (instead of mapping to full TB genome)
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
- oldCode: old code I am not quite ready to throw
  away yet

# freezeTB as an bash script

Chapter four: make your own monster

A better picture of how freezeTB works might be gotten by
  showing how it would look if I used an bash script. This
  is for default settings only, since it is an example and
  I want to keep the script more simple. This has not been
  tested.

You will need some programs from my bioTools repo(
  [https://github.com/jeremybuttler/bioTools](
   https://github.com/jeremybuttler/bioTools)). These
  include ampDepth, filtsam, maskPrim, and tbCon.

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
    -sam "$prefixStr-consensuses.sam" \
    -out "$prefixStr-con-amrs.tsv";

# find the MIRU-VNTR lineage
tbMiru \
    -miru-tbl "$dbDirStr/miruTbl.tsv" \
    -sam "$prefixStr-consensuses.sam" \
    out-tbl "$prefixStr-con-miru.tsv";

# find the spoligotype
tbSpol \
    -con-frag \
    -spoligo "$dbDirStr/spoliogtype-seq.fa" \
    -db "$dbDirStr/spoligo-lineages.csv" \
    -sam "$prefixStr-consensuses.sam" \
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
   # default is 50
fi # If: graphs were wanted
```
