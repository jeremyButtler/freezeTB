# Goal:

Install freezeTB

# Guide:

# Windows

Make user R is installed with "ggplot2" and "viridisLite".
  I would recommend also installing "tcltk2" and
  "svgLite".

In R/Rstudio do:

```
install.packages("ggplot2")
install.packages("viridisLite")
install.packages("tcltk2")
install.packages("svgLite")
```

Then make a "freezeTB" directory (folder) in
  "C:\Program Files" (my computer->C->Program Files). Copy
  all files binary/binWin/ to "C:\Program Files\freezeTB".

Then copy freezeTBFiles to your "Documents" directory
  (folder) or to "Public Documents"
  (my compter->C->Users->Public->Documents).

See the README for instructions on compiling from source
  on Windows.

Copy the "freezeTBGui.r" (in scripts) to any location
  you want. Use R or Rscript to launch the GUI

How to tell Windows to use Rscript for R scripts:

Right click freezeTBGui.r, then select "open with".
   Select "more amps", and scroll down to the "+"
   (browse for more apps). R->R-[version]->bin->Rscript.

For source compiling see the README file (markdown file).

# Unix

This guide is here to help you install freezeTB on your
  computer. My audience is users that are not used to the
  command line. So, this guide is not for you if you are
  used installing programs from source or even conda
  (use the install instructions in the README.md file).

First open an terminal. Then if you make sure R is
  installed by typing `Rscript --version` and hit
  enter to submit the command. If you get the version of
  Rscript, then you have R installed. In that case you
  can skip the installing R step.

# Installing R

If you are on Ubuntu or Debian Linux
  do `sudo apt-get install R`.

There are several ways for Mac, but I found homebrew to
  be an easy way. You can find the instructions toinstall
  homebrew at [https://brew.sh](https://brew.sh). It also
  makes sure tcltk is compiled in.

After installing homebrew do `brew install R` to install
  R.

# clone/Download freezeTB

I want to clone (download) freezeTB into an directory that
  is not may home directory. First make sure you have a
  Downloads directory with `ls ~/Downloads` to make sure
  the Downloads directory exits.  If Downloads does not
  exist (you get "ls cannot access 'Downloads': No such
  file or directory) do `mkdir ~/Downloads`
  to make the downloads directory.

- `ls` lists the files and directories in an location.
- `~/` is an shortcut for your home directory.
- mkdir means make directory (mk = make, dir = directory).
  - an directory is an folder.

After checking/making and Downloads directory change your
  working directory to Downloads with `cd ~/Downloads`
  (I think of cd as "change directory").

- `cd path` changes the current working directory to an
   path.

Make sure you have git on you computer by doing 
  `git --version`. If you do not have git, you can install
  it on Ubuntu/Debian using `sudo apt-get install git`.
  Git should be installed on Mac by default. If not,
  install with homebrew `brew install git`.

Clone feezeTB from github
  with `git clone https://github.com/jeremybuttler/freezeTB`.
  Then change you directory to the cloned repository
  with `cd freezeTB`.

# Installing freezeTB (globally)

Before installing freezeTB, check if you have minimap2
  installed by doing `minimap2 --version`. If you do not
  have minimap2 you can install it on Ubuntu/Debian using
  `sudo apt-get install minimap2` or on Mac with
  `brew install minimap2`. I have also included a compiled
  binary for minimap2 in binaries/OS. You can install it
  with `sudo cp binaries/bin<OS>/minimap2 /usr/local/bin`
  and `sudo chmod a+x /usr/local/bin/minimap2`.

- sudo gives a command temporary super user access. It is
  needed for global installs.

Then do `make -f mkfile.unix` to build freezeTB. After
  that install freezeTB
  with `sudo make -f mkfile.unix install-all`. You can
  clean up temporary files
  with `make -f mkfile.unix clean`.

You can check the help message for freezeTB
  with `freezeTB -h`.

# installing freezeTB locally

We will install all files in a `~/local/bin` directory.
  Since freezeTBGui.r knows to look there.

First make sure you have a "local" folder in your home
  directory with `ls ~/local`. If not, then make a local
  directory with `mkdir -p ~/local/bin`.

We are using -p (parent) with mkdir because we are
  creating a new directory in a directory we are making.

If you have `~/local`, then make sure you
  have `~/local/bin`. If not create `~/local/bin`
  with `mkdir ~/local/bin`.

## Minimap2 from source

If you are installing minimap2 from source then clone
  minimap2
  with `git clone https://github.com/lh3/minimap2`. Then
  change your directory to the minimap2 repository
  (`cd minimap2`).

For non-ARM (Mac M chips) systems build minimap2 with
  `make`.

For ARM 64 bit systems (Mac M chips) build minimap2 with
  `make arm_neon=1 aarch64=1`.

If you are on a 32 bit ARM system (not likely), then build
  minimap2 with `make arm_neon=1`.

Copy minimap2 to your local directory
  with `cp minimap2 ~/local`. Then set execute permision
  with `chmod a+x ~/local/minimap2`.

## freezeTB

Make sure you have an documents folder by
  doing `ls ~/Documents`. If not make an Documents
  directory with `mkdir ~/Documents`.

Install freezeTB in your local install
  with `make -f mkfile.unix install-all PREFIX="~/local/bin" DB_PEFIX="~/Documents".

You can clean up temporary files
  with `make -f mkfile.unix clean`.

Copy the freezeTBGui.r script to a convient location.

## Optional; updating path

At this point freezeTB is installed and the GUI will know
  were to search (it will run). However, your system may
  not be able to find freezeTB. So, you will not be able
  to type `freezeTB` on the command line to run freezeTB.

Your path is were you system searches for programs. To add
  `~/local/bin` to your path you will need to use
  `export PATH=$PATH:$HOME/local/bin'. However, this
  change will be wiped after you reboot (or exit your
  terminal).

For  a more permanent solution add the path update to
  your .bashrc file, which fires when you start a
  new temrinal. You can do this in a text editor or you
  can use `printf "export PATH=\$PATH:\$HOME/local/bin\n" >> ~/.bashrc`.
  Make sure you typed everything correctly, otherwise
  you could loose your path.
  
- PATH holds the locations your computer searches for
  programs in (your path). Locations are separated by
  a ":" (Windows is ";").
- `.bashrc` the file that runs when you open an terminal
  (unix only).
- printf prints out an string
  - in printf an "\n" means newline
- `>>` append (add) output to and file
  - warning `>` wipes the file and then writes the output
    to the file.
 
# Ending comments

You have freezeTB now installed on your system. In the
  next module I will quickly cover how to run freezeTB by
  command line. In the module after that I will cover how
  to set up an bash script to do batch processing for
  freezeTB.

# Random item, ignore

https://www.bv-brc.org site with fully curated bacterial
  and viral genomes. Uses VIGOR4 for annotation. Also an
  weakly output reporter. Also includes an sequence
  submission tool. Also uploads virus sequences to
  Genbank.

read2tree: read paper. Using raw reads in an tree. From 
  what I heard they are building and consensus and then
  adding it to an tree.
