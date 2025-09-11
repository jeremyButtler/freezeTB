![freezeTB icon](FTB-icon.svg)

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
  code from other repositories. The code in in the
  programs directory that is not from freezeTB are.

- raylib from raysan5's raylib
  [https://github.com/raysan5/raylib](
   https://github.com/raysan5/raylib)
  - zlib license
- IBM Plex Mono font from fontshare.org
  - Under SIL Open Font License Version 1.1
  - [https://www.fontspace.com/search?q=IBM%20Plex%20Mono](
     https://www.fontspace.com/search?q=IBM%20Plex%20Mono)

All code that is unique to freezeTB is under a dual
  license. The primary license is Unlicense. However, not
  all companies/countries like or will even respect the
  Unlicense. So, in those cases or cases were the
  Unlicense is not wanted/allowed, the freezeTB unique
  code is under the MIT license.

# Thanks

Here are the order of Merlin winners:

- People:
  - Tara Ness: helping in GUI disign, feedback, and
    co-PI on project
  - Eric Bortz: project PI, boucing ids, and general
    encouragment
  - Bryce Inman: GUI testing, AMR testing, and designed
    wet lab side
  - Anna Kovalenko for advice during the building process
  - Abigail Seeger: GUI testing on windows
  - My dad for listening and giving feedback
- Taken from other sources:
  - Files in freezeTBFiles
    - The amr database (amrDb.tsv) is from the WHO 2023
      catalog
    - The spoligotype lineages csv (spoligo-lineages.csv)
      is from TBProfiler
      - Phelan JE, O'Sullivan DM, Machado D, Ramos J,
        Oppong YEA, Campino S, O'Grady J, McNerney R,
        Hibberd ML, Viveiros M, Huggett JF, Clark TG.
        Integrating informatics tools and portable
        sequencing technology for rapid detection of
        resistance to anti-tuberculous drugs. Genome Med.
        2019 Jun 24;11(1):41.
        doi: 10.1186/s13073-019-0650-x.
        PMID: 31234910; PMCID: PMC6591855
    - NC000962.fa and genes-NC000962.fa were from Genbank
      - Also in benchmark as tbdb.fasta
    - My MIRU-VNTR database (miruTbl.tsv) came from
      MIRUReader
      [https://github.com/phglab/MIRUReader](
       https://github.com/phglab/MIRUReader)
    - spoligo-spacers.fa were from
      - Kamerbeek J, Schouls L, Kolk A, van Agterveld M,
        van Soolingen D, Kuijper S, Bunschoten A,
        Molhuizen H, Shaw R, Goyal M, van Embden J.
        Simultaneous detection and strain differentiation
        of Mycobacterium tuberculosis for diagnosis and
        epidemiology. J Clin Microbiol. 1997
        Apr;35(4):907-14.
        doi: 10.1128/jcm.35.4.907-914.1997.
        PMID: 9157152; PMCID: PMC229700.
  - minimap2: recommend; auto installed for Mac scripts
    - Under the MIT license
    - [https://github.com/lh3/minimap2](
       https://github.com/lh3/minimap2)
  - raylib: is the base for the ftbRay GUI (graphical user
    interface [the window])
    - Raylib is under the zlib licenense
    - [https://github.com/raysan5/raylib](
       https://github.com/raysan5/raylib)
  - IBM Plex Mono font from fontshare.org
    - Used in the raylib GUI (ftbRay)
    - Under SIL Open Font License Version 1.1
    - [https://www.fontspace.com/search?q=IBM%20Plex%20Mono](
       https://www.fontspace.com/search?q=IBM%20Plex%20Mono)
  - tcltk was used to make the ftbTcltk GUI (graphical
    user interface [the window])
    - Tcltk is under the MIT license
    - [https://github.com/tcltk/tk](
       https://github.com/tcltk/tk)
    - [https://github.com/tcltk/tcl](
       https://github.com/tcltk/tcl)

- Unique to freezeTB:
  - Most of the stuff in programs
  - The icon
  - Everything in scripts

# Install

For the graphical user interface (GUI), you have two
  choices. One GUI which uses TclTk or another GUI that
  uses raylib.

For Windows and Mac you also have binaries (in bin) you 
  can install. I also included the Linux binary, but I
  am not sure it will work.

For the command line programs, installation is not
  covered here. You can do `cd programs/<program>Src` and
  then build the program using the make file.
  Use `mkfile.unix` for Linux/BSD/Mac. Use `mkfile.win`
  with `nmake` for windows.

## Linux:

### raylib Linux

For the acutual installtion steps, I have two options. The
  global install and local installs. Pick the one that
  works best for you.

Install the dependiences listed in
  [https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux](
   https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

Once you have installed the dependencies:

```
git clone https://github.com/jeremybuttler/freezeTB ~/Downloads/freezeTB;
cd ~/Downloads/freezeTB/programs/ftbRaylibSrc;
make -f mkfile.linux

# this does a global install
sudo make -f mkfile.linux install
sudo cp -r ../../freezeTBFiles /usr/local/share;
sudo chmod -R a+r /usr/local/share/freezeTBFiles;

# for a local install do
if [ ! -d ~/local/bin ];
then
   mkdir -p ~/local/bin;
   export PATH="$PATH:~/local/bin";
   printf 'PATH="$PATH:~/local/bin"\n' >> ~/.bashrc;
fi; # setup your local install location

make -f mkfile.linux PREFIX=~/local/bin install;
cp -r ../../freezeTBFiles ~/Documents;
sudo chmod -R a+r ~/Documents/freezeTBFiles;
```

I recommend installing minimap2, it is more sensitive and
  faster then freezeTB's internal read mapper. For a
  Debain based system (ex Ubuntu)
  do `sudo apt-get install minimap2;`.

Otherwise:

```
cd ~/Downloads;
git clone https://github.com/lh3/mininmap2 ~/Downloads/minimap2;
cd minimap2;
make;

# this does a global install
sudo cp minimap2 /usr/local/bin;
sudo chmod a+x /usr/local/bin/minimap2;

# for a local install
if [ ! -d ~/local/bin ];
then
   mkdir -p ~/local/bin;
   export PATH="$PATH:~/local/bin";
   printf 'PATH="$PATH:~/local/bin"\n' >> ~/.bashrc;
fi; # setup your local install location

# this installs the manpage for minimap2, optional
sudo cp minimap2.1 /usr/share/man/man1;
```

### TclTk Linux

For Linux I am assuming you are using a Debian based
  distribution, such as Ubuntu. However, I have not been
  able to get the GUI (graphical user interface) compiled
  on Ubuntu (some linking issue). For other distros or
  Unix OS's, make sure you have the packages.

1. Install R: `sudo apt-get install r-base;`
2. Install: tcltk:`sudo apt-get install tcl-dev tk-dev`
3. You may need to install `gcc` and `make`
4. I recommend minimap2: `sudo apt-get install minimap2;`
   - Otherwise freezeTB is slow

```
git clone https://github.com/jeremybuttler/freezeTB ~/Downloads/freezeTB;
cd ~/Downloads/freezeTB/programs/ftbTclTkSrc;
make -f mkfile.unix
sudo make -f mkfile.unix install
```

## Mac

There are binararies in the `bin` folder. However, Mac has
  requires apps/programs to be signed by a person with a
  developer account or the user to manually flag it as ok.

You can install freezTB in multiple ways. The easiest way
  is using one of the Mac installer scripts. Both the
  raylib and TclTk install scripts install freezeTB and
  minimap2 locally.

The harder way is from source (not covered here).

### Mac; easy way pre-steps

You may get a request to install xcode. This is Mac
  developer software and is needed to compile the code.

Xcode may take a bit to install and you may have to
  retry a few times. After xcode is installed then
  run `cd ~/Downloads/freezeTB; sh macInstall;` again.

You can check/install xcode by running this command here.

```
export MACOSX_DEPLOYMENT_TARGET=10.9 && \
xcodeHere="$(xcode-select -p 1/dev/null;echo $?)"; \
if [ "$xcodeHere" -ne 0 ]; then\
   xcode-select --install; \
fi; \
```

### Mac raylib Easy way:

Open a terminal (apps->terminal). Then copy the code
  beneath into the terminal and hit enter/return.

```
git clone https://github.com/jeremybuttler/freezeTB ~/Downloads/freezeTB;
cd ~/Downloads/freezeTB;
sh macRayInstall;
```

Do not worry about the Rec errors at the end. This is
  a failed attempt to setup the icon.

### Mac TclTk Easy way:

Open a terminal (apps->terminal). Then copy the code
  beneath into the terminal and hit enter/return.

```
git clone https://github.com/jeremybuttler/freezeTB ~/Downloads/freezeTB;
cd ~/Downloads/freezeTB;
sh macInstall
```

Do not worry about the Rec errors at the end. This is
  a failed attempt to setup the icon.

If you want graphs, install R with PNG support.

## Windows

I would reccomend using the windows raylib exe (binary) in
  the `bin` folder. Copy `ftbRay.exe` (may be `ftbRay`) to
  your desktop. Then copy the `freezeTBFiles` to your
  desktop.

There is a TclTk version, but it requires installing Magic
  splat and visual studio build tools. The raylib version
  is the binary.

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
  in `%app%\Documents` (user Documents) first and
  then in `%PUBLIC%\Documents` (Public Documents).

### Mac/Linux:

Get the help message with `freezeTB -h`. It is long so
  I would recomend doing `freezeTB -h | less` on Linux.

Map reads to the reference using
  minimap2: `minimap2 -a /usr/local/share/freezeTBFiles/NC000962.fa reads.fastq > reads.sam`.

Then run
  freezeTB: `freezeTB -sam reads.sam -prefix goodName`.

You can also have freezeTB do the read mapping with
  `freezeTB -prefix goodName /path/to/fastq_pass/*.fastq*`.

To get graphs
  do: `graphAmpDepth.r -stats goodName-depths.tsv -amrs goodName-read-amrs.tsv -who /usr/local/share/freezeTBFiles/amrDb.tsv -prefix goodName`

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

### R9.4 flow cells

The settings for freezeTB are setup for R10.4 flow cells.
  If you plan on using R9.4 flow cells, then at least
  disable frame shift checking and increase the required
  percentage of indels needed to keep an AMR to 70%.

# Extra files:

The extra files are saved as `.md` files. You can view
  these at `https://github.com/jeremybuttler/freezeTB` or
  you can right click and select open with. Choose your
  regular text editor to view the files.

- The howWorks.md file has a description of the steps used
  in freezeTB
- Included-programs.md includes a list of programs that
  come with freezeTB. Some are installed by default,
  others are side programs. The side programs often do
  a single step (ex AMR analysis) in the freezeTB program.
