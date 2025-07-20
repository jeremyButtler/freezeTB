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
- minimap2 from lh3 [https://github.com/lh3/minimap2](
  https://github.com/lh3/minimap2)
  - MIT license
- TclTk from [https://github.com/tcltk/tcl](
  https://github.com/tcltk/tcl) and
  [https://github.com/tcltk/tk](https://github.com/tcltk/tk)
  - MIT license

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
  - minimap2: included for unix, recommend for windows
    - [https://github.com/lh3/minimap2](
       https://github.com/lh3/minimap2)
  - raylib: not used yet, but hoping to replace tcltk with
    raylib for the GUI (under zlib licenense)
    - [https://github.com/raysan5/raylib](
       https://github.com/raysan5/raylib)
  - tcltk to make the GUI (graphical user interface [the
    window])
    - [https://github.com/tcltk/tk](
       https://github.com/tcltk/tk)
    - [https://github.com/tcltk/tcl](
       https://github.com/tcltk/tcl)

- Unique to freezeTB:
  - Most of the stuff in programs
    - raylib is the only thing not unique to freezeTB
  - The icon
  - Everything in scripts

# Install

## Linux:

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

### To install freezeTB

Download freezeTB from github and then use the
  `macInstall` script to install dependencies and
  freezeTB. This will auto install minimap2 for you.

Open a terminal (apps->terminal). Then copy the code
  beneath into the terminal and hit enter/return.

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
sh macInstall
```

Do not worry about the Rec errors at the end. This is
  a failed attempt to setup the icon.

Enter your password when prompted.

## Harder way:

1. Install homebrew [https://brew.sh/](https://brew.sh/)
2. Install X11 (xquartz) `brew install xquartz`
3. Install tcltk `brew install tcl-tk`
4. Install R `brew install R`
5. I recommend minimap2 `brew install minimap2`
   - Otherwise freezeTB is slow

Then you can install freezeTB:

```
cd ~/Downloads
git clone https://github.com/jeremybuttler/freezeTB
cd freezeTB
make -f mkfile.unix
sudo make -f mkfile.unix install
```

This is really painfull.

If you are having issues linking to tcltk, then edit the
  `mkfile.unix` in `programs/ftbTclTkSrc`. You will see
  three gaint if statments that are looking for X11, Tcl,
  and Tk. Copy an `elif` line and add your paths
  (locations) to X11, Tcl, and Tk to these if statements.

You will also have another if statement looking for the
  include directory. Make sure you path is in there.

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

I have setup my install script to find the tcltk libraries
  installed by magic splat. Sadly, there is no support
  for ARM (snapdragon CPUs). So, you can only use the
  command line version on ARM Windows.

### current method

Best way I found. It relies on visual 2022 build tools
  being installed.

- install tctlk 8.6 from magic splat
  - [https://sourceforge.net/projects/magicsplat/files/magicsplat-tcl/](
     https://sourceforge.net/projects/magicsplat/files/magicsplat-tcl/)
  - download the tcl-8.616-installer-1.16.0-x64.msi file
- Install visual studio or at least the developer console
   from visual studio
   - [https://visualstudio.microsoft.com/downloads/](
      https://visualstudio.microsoft.com/downloads/)
   - You can either install the full visual studio
     program or if you just want the tools go
     to `tools for visual studio`
     then select `build tools for visual studio`
   - While installing it will prompt you for what options
     you want to install, select the c++ development, it
     should be the computer monitor picture in the upper
     left corner
- Double click `winInstall.bat` in the `windows` folder
  - to install globaly; run as administratory
   (right click->run as administrator)

If you are on an ARM CPU, open a developer console (see if
  failed). Then `cd "%homePath%\Downloads\freezeTB\programs\freezeTBSrc\`.
  After that do `nmake /F mkfile.win`. Copy
  the `freezeTB.exe` to a convient location. This will
  give you the command line freezeTB program.

### if failed

Likley because the `winInstall.bat` script could not
  activate the developer enviroment. You can activate
  this by opening a developer console.

If it did not install, then open a developer console (run
  as administrator for a global install)
  (start menu->visual studio->x64 native tools command prompt).
  Then type `"cd %homePath%\freezeTB\windows"` and hit
  enter.  Then type `winInstall.bat` and hit enter. At this
  point freezeTB should be installed. Type `exit` and hit
  enter.

Currently the graphing step does not work in windows due
  to a windows error that causes Rscript to error out
  when called by tcl. However, you can build the graphs
  manually by opening the windows terminal.

```"%PROGRAMFILES%\R\R-<R version>\bin\Rscript.exe" "%PROGRAMFILES%\freezeTB\graphAmpDepth.r" -who "%HOMEPATH%\Documents\freezeTBFiles\amrDb.tsv" -stats "%HOMEPATH%/path/to/FTB_output/<prefix>-depths.tsv -prefix "%HOMEPATH%/path/to/FTB_output/<prefix>```

The <R version> needs to be filled with the version
  number of R. You can find this by going
  to `C:\Program Files\R`.

The <prefix> is the prefix for the output files from
  freezTB.

![
  Picture showing how to open developer console
](windows10devConsole.png)

### Windows Minimap2

I do not include a minimap2.exe compiled binary in
  the `windows` folder to avoid any weird issues. So, if
  you want minimap2, you will have to build it your self.

If your computer's CPU is an ARM CPU, such as a snapdragon
  CPU, you will need to compile minimap2 from source using
  the `-neon=1` and `-aarch64=1` flags (assuming minigw
  has an ARM release).

First install Cygwin using the setup-x86_64.exe (is a
  link):

[https://cygwin.com/install.html](
  https://cygwin.com/install.html)

When at "Select Packages" menu for Cygwin install:

- Search for "make" then select the package named make.
  - Ensure you are looking at the "full" and not the
    "pending list"
  - Change "Skip" to make version number with dropdown
    menu (choose most recent non-testing version)
  - Do not install gcc (it cannot statically link)
- Select the mingw64-x86_64-gcc-core entry
- Select the mingw64-x86_64-zlib entry

Download minimap2 to your downloads folder

[https://github.com/lh3/minimap2](
 https://github.com/lh3/minimap2)

Unzip the minimap2-master folder (right click->extract)

Open a Cygwin terminal (run Cygwin). Then move into
  minimap2's directory 
  (`cd c:/Users/<user name>/Downloads/minimap2-master/minimap2-master`).
  - For me `cd c:/Users/Jeremy/Downloads/minimap2-master/minimap2-master`

Build minimap2 (x86 [intel/amd])
  with `make CC=x86_64-w64-mingw32-gcc CFLAGS=--static`.
  You will need --static here to avoid needing .dll files.

Copy minimap2 to your freezeTB install location or into
  your PATH.

`copy minimap2.exe "C:\Users\<user_name>\Downloads\freezeTB\winBin"`

Or, if you already installed freezeTB, copy minimap2.exe
  to your install location. It is
  either `"C:\Users\<user_name>\appData\local\freezeTB"`
  or if you ran the winInstall script as an administrator
  it is in `"C:\Program Files\freezeTB"`.

For appData do
   do `copy minimap2.exe "C:\Users\<user_name>\appData\local\freezeTB"`

For program files (global) do
   do `copy minimap2.exe "C:\Program Files\freezeTB"`

# Uninstalling freezeTB

The uninstall scripts will uninstall the freezeTB
  programs, but will not uninstall the depenencies/auto
  installed programs. For Mac this is homebrew, minimap2, 
  R, xQuartz, and TclTk. For Linux and windows you will
  not remove TclTk, visual studio build tools (windows),
  R, and minimap2 (windows will remove).

## Mac

```
cd ~/Downloads/freezeTB;
sh macUninstall;
```

## Unix (Linux)

```
cd ~/Downloads/freezeTB
sudo make -f mkfile.unixUninstall
```

To uninstall, but not remove the freezeTB databases in you
  Documents directory, do.

```
cd ~/Downloads/freezeTB
sudo make -f mkfile.unixUninstall rmInstall
```

If you installed freezeTB to a different location using
  `make PREFIX=/path/to/new/location -f mkfile.unix install`,
  then do

```
cd ~/Downloads/freezeTB
sudo make PREFIX=/path/to/install/location -f mkfile.unixUninstall
```

## Windows

To uninstall; double click `winUninstall.bat` in `windows`
  folder.
  - to remove globaly; run as administratory
   (right click->run as administrator)

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
