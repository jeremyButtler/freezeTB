# Goal:

Install freezeTB

# Guide:

This guide is here to help you install freezeTB on your
  computer. My audience is users that are not used to the
  command line. So, this guide is not for you if you are
  used installing programs from source or even conda
  (use the install instructions in the README.md file).

First open an terminal. Then if you are no Mac make sure
  R is installed by typing `Rscript --version` and hit
  enter to submit the command. If you get the version of
  Rscript, then you have R installed. In that case you
  can skip the installing R step.

# Installing R

If you are on Ubuntu or Debian Linux
  do `sudo apt-get install R`.

There are several ways for Mac, but I found homebrew to
  be an easy way. You can find the instructions toinstall
  homebrew at [https://brew.sh](https://brew.sh).

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
  `brew install minimap2`.

- sudo gives a command temporary super user access. It is
  needed for global installs.

Then do `make` to build freezeTB. After that install
  freezeTB with `sudo make install`.

You can check the help message for freezeTB
  with `freezeTB -h`.

# installing freezeTB locally

Local installs are a bit harder. First check if minimap2
  exists with `minimap2 --version`. This changes how we
  will build freezeTB. If minimap2 does not exist do
  `make minimap` for computers with Intel or AMD CPUs or
  `make minimapneon` for computers with ARM (Mac M chip)
  CPUs. Otherwise do `make` like in global install.

Make sure you have an documents folder by
  doing `ls ~/Documents`. If not make an Documents
  directory with `mkdir ~/Documents`.

First make an local install
  location `mkdir -p ~/local/bin`. Then install freezeTB
  in that location
  using `make install PREFIX="~/local/bin" -dbPEFIX="~/Documents"

At this point freezeTB is installed, but your system does
  not know were freezeTB is at. To tell it were freezeTB
  is at you need to add the install location to your path
  (see PATH comment). You can temporally add freezeTB to
  your path with `export PATH=$PATH:$HOME/local/bin'. For
  a more permanent solution
  do `printf "export PATH=\$PATH:\$HOME/local/bin\n" >> ~/.bashrc`.
  
- PATH holds the locations your computer searches for
  programs in (your path). Locations are separated by
  ":"'s.
- `.bashrc` the file that runs when you open an terminal
  or log in (not sure which).
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

https://www.bv-brc.org site with fully curated bacterial
  and viral genomes. Uses VIGOR4 for annotation. Also an
  weakly output reporter. Also includes an sequence
  submission tool. Also uploads virus sequences to
  Genbank.

read2tree: read paper. Using raw reads in an tree. From 
  what I heard they are building and consensus and then
  adding it to an tree.
