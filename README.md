# Use

freezeTB is a program that detects gene mutations
  associated with drug resistance from nanopore sequenced
  tuberculosis samples. It also outputs a set of
  consensus fragments (as a sam file) and checks for
  lineages by spoligotyping and/or MIRU-VNTR.

Currently freezeTb uses the WHO 2023 mutation catalog from
  [https://github.com/GTB-tbsequencing/mutation-catalogue-2023](
   https://github.com/GTB-tbsequencing/mutation-catalogue-2023).

# License:

freezeTB includes databases from other repositories and
  complied code (minimap2) from other repositories. In
  those cases the licensing will be under the owner's
  repository.

Here are the order of Merlin winners:

- Taken from other sources:
  - files in freezeTBFiles
    - amrDb.tsv is the WHO 2023 catalog (converted)
    - spoligo-lineages.csv from TBProfiler
    - NC000962.fa and genes-NC000962.fa were from Genbank
      - also in benchmark as tbdb.fasta
    - miruTbl.tsv is the modified MIRU-VNTRU table from
      MIRUReader (github)
    - spoligo-spacers.fa were from a spoligotyping program
      - Kamerbeek J, Schouls L, Kolk A, van Agterveld M,
        van Soolingen D, Kuijper S, Bunschoten A,
        Molhuizen H, Shaw R, Goyal M, van Embden J.
        Simultaneous detection and strain differentiation
        of Mycobacterium tuberculosis for diagnosis and
        epidemiology. J Clin Microbiol. 1997
        Apr;35(4):907-14.
        doi: [10.1128/jcm.35.4.907-914.1997](
              10.1128/jcm.35.4.907-914.1997).
        PMID: 9157152; PMCID: PMC229700.
  - minimap2 (MIT) window binaries compiled from
    [https://github.com/lh3/minimap2](
     https://github.com/lh3/minimap2)
  - tcltk (the GUI (graphical user interface [the window])
    - [https://github.com/tcltk/tk](https://github.com/tcltk/tk)
    - [https://github.com/tcltk/tk](https://github.com/tcltk/tcl)

The stuff unique to freezeTB is under a dual license. The
  primary license is Unlicense. However, not all
  companies/countries like or will even respect the
  Unlicense. So, in those cases or cases were the
  Unlicense is not wanted/allowed, freezeTB is under the
  MIT license.

- Unique to freezeTB:
  - everything in programs
    - This is the reason Norbert, I mean freezeTB, can fly
      on Windows.
  - everything in scripts
  - the terrible guides (guides folder)
  - the scripts in benchmark

# Install

## Linux:

For Linux I am assuming you are using a Debian based
  distribution, such as Ubuntu. However, I have not been
  able to get the GUI (graphical user interface) compiled
  on Ubuntu (some linking issue). But, I have on void
  Linux.

1. minimap2: `sudo apt-get install minimap2;`
2. R: `sudo apt-get install r-base;`
3. tcltk:`sudo apt-get install tcl-dev tk-dev`

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
sudo make -f mkfile.unix install
```

## Mac

You can install freezTB in multiple ways. The easiest way
  is using the Mac installer script. The harder way is
  from source.

## Easy way:

download freezeTB from github, then use the
  `macInstall` script to install dependencies and
  freezeTB.

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
sh macInstall
```

Enter your password when prompted.


## Harder way:

1. Install homebrew [https://brew.sh/](https://brew.sh/)
2. Install X11 (xquartz) `brew install xquartz`
3. Install tcltk `brew install tcl-tk`
4. Install R `brew install R`
5. Install minimap2 `brew install minimap2`

Then you can install freezeTB:

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
sudo make -f mkfile.unix install
```

### Mac minimap2 from source

If you do not want to use the brew install for minimap2
  then you can install it from source. Linux is very
  similar, except for x86 CPUs (non-ARM) remove
  do `make` instead of `make arm_neon=1 aarch64=1`.

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

I need to build the easy way. Work in progress.

### Windows method (source)

This is not the method your are looking for

GUI has not been tested.

1. Install visual studio or at least the developer console
   from visual studio
2. Make a freezeTB directory in "C:\Program Files"
3. Download freezeTB from git hub and unzip in Downloads
4. Copy the freezeTBFiles directory to Documents
   (or public)
5. Open the developer console
   (visual studio->view->developer console) and
   do `cd $HOME$/Downloads/freezeTB`.
6. Compile freezeTB with `nmake /F mkfile.win`, then close
   the developer console
7. In the freezeTB folder copy the freezeTB.exe in
   "programs->freezeTBSrc" to the freezeTB folder in
    "C:\Program Files"
9. Copy the minimap2.exe (minimap2) file minimap2Windows
   to your freezeTB folder in "C:\Program fIles"
   - or compile minimap2 and copy the .exe file
10. Copy the freezeTBGui.Rscript (freezeTBGui) file in
    scripts to your desktop or where you want it.
11. Make sure R is installed or if not install R or
    Rstudio
    [https://cran.r-project.org/bin/windows/base/](
     https://cran.r-project.org/bin/windows/base/)
12. Right click freezeTBGui.Rscript and select
    "open with", then "more programs", then "R",
    "R <version number>", "bin", "Rscript"

![
  picture showing how to open developer console
](windows10devConsole.png)

### Windows Minimap2 (from source)

I include a minimap2.exe compiled binary in winBin.
  So, you do not need this. However, here are the
  steps if you want a more recent version of minimap2. If
  your computers CPU is an ARM (snap dragon) you will need
  to compile minimap2 from source.

First install Cygwin using the setup-x86_64.exe (is a
  link):

[https://cygwin.com/install.html](
  https://cygwin.com/install.html)

When at "Select Packages" menu for Cygwin install:

- Search for "make" then select the package named make.
  - Ensure you are looking at the "full" and not the
    "pending list"
  - change "Skip" to make version number with dropdown
    menu (choose most recent non-testing version)
  - do not install gcc (it cannot statically link)
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

The freezeTB icon should be on your desktop.

For Windows, the freezeTB GUI needs some work.

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
  in `%HOME%\Documents` (user Documents) first and
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

Windows command line is painful, but yes you can use it.
  This should work, so long as carriage returns do not
  become an issue. If have errors or infinite loops, let
  me know.

You can get the help message
  with: `%ProgramFiles%\freezeTB\freezeTB.exe -h`

Map reads to the reference using
  minimap2: `%ProgramFiles%/freezeTB/minimap2.exe -a %PUBLIC%\Documents\freezeTBFiles\NC000962.fa reads.fastq > reads.sam`.

Map reads to the reference using
  freezeTB: `%ProgramFiles%\freezeTB\freezeTB.exe -sam reads.sam -prefix goodName`

Build graphs (if wanted)
  do: `%ProgramFiles%\R\<R version>\bin\Rscript %ProgramFiles%\freezeTB\graphAmdDepth.r -stats goodName-depths.tsv -who %PUBLIC%\Documents\freezeTBFiles\amrDb.tsv -prefix goodName`

# Extra files:

- howWorks.md has a description of the steps used in
  freezeTB
- included-programs.md includes a list of programs that
  come with freezeTB. Some are installed by default,
  others are side programs. The side programs often do
  a single step (ex AMR analysis) in the freezeTB program.
