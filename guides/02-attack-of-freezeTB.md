# Goal:

Run freezeTB on the commandline.

# Introduction:

The command line might look challenging at first, but it
  becomes more and more easy the more often you use it.
It also allows the ability to automate or create your
  own, shareable pipelines.
These pipelines can include standard Unix tools, such
  as awk, sed, grep and sort/uniq.
Or they can include other command line programs you have
  installed or made.
For Bioinformatics, were most programs are command line
  based, this means you can have a very large tool set
  to build pipelines with.

For right now we are going to focus on just running
  freezeTB on the command line.
But in the last part of this series, I will show you how
  to automate freezeTB to process the output from a single
  flow cell.

# Mapping reads

To run freezeTB on the command line you first need to
  build an sam file with a read mapper, such as minimap2.
If you have an single fastq file or fastq.gz file there
  is no problem.
We can
  do `minimap2 -a -x map-ont reference.fasta reads.fastq`.
However, most of the time the base caller gives us an
  group of fastq.gz files that we need to combine to make
  one fastq.gz file.
If you have base called data on hand you can do this by
  looking in the barcode directories in the `fastq_pass`
  folder.
You can use `ls` (`ls /path/to/fastq_pass/barcode01/`) to
  look at the contents of a directory
  (use `ls /path/to/fastq_pass` to get an list of
  barcodes).

- `ls`: list the files and directories in an directory
  - you can provide an path to an directory to look at
    with `ls /path/to/directory`
- tab completion: hitting tab will try to autocomplete
  your program name or file name.
  - Hitting tab twice will list all the possible programs
    or files/directories.
- minimap2: a read mapper (align reads to reference) often
  used for Nanopore sequenced reads. The two files at the
  end of an minimap2
  command (`minimap2 -a ref.fasta reads.fastq`) are the
  reference and the reads to map to the reference.
  - -a outputs a sam file (instead of an paf file)
  - -x changes the settings
    - map-ont is for mapping Nanopore sequenced reads
      to an reference (default)
    - ava-ont is for and all versus all alignment mapping
      Nanopore reads to other reads in the fastq file
      instead of an reference
  - --eqx makes in eqx cigar (see sam file note at bottom
    of page).
  - -h prints the help message
    - a more detailed version can be had with
      with `man minimap2`. However, this requires the
      minimap2 man page installed (often is).

To merge the fastq.gz files together we are going to use
  an program named cat (short for concatenate).
You can get a help message with `cat --help` or view the
  man page with `man cat`.
We can concatenate files by listing each one with cat,
  like `cat file1.fastq.gz file2.fastq.gz`, but this takes
  a lot of time.
Another option is to use an wild card, which globs the
  files together. 
In bash the wild card is `*`.
So, we can make an merged .fastq.gz file for barcode01
  using `cat /path/to/fastq_pass/barcode01/*.fastq.gz`.
However, you will notice that that printed everything out
  to screen.
So, like in the last episode we will have to redirect our
  output to an file with an `>` symbol (we would prefer
  older files get overwritten).
The final command might look like
  `cat /path/to/fastq_pass/barcode01/*.fastq.gz > barcode01.fastq.gz`.

- wild card/glob (`*`): fills in all possible names given
  what is around the `*`. Literally it stands for any
  character repeated any number of times.
  - `ls *` show all files in the current directory
  - `cat *.fastq.gz` concatenate all fastq.gz files in
    the present working directory (current location)
  - `ls barcode*` show all directories and files that
    start with barcode in the current location.
  - `ls barcode*9` show all barcodes that end in 9
- `>>` append (add) output to and file
  - warning `>` wipes the file and then writes the output
    to the file.

At this point we only need the reference sequence to build
  the sam file with minimap2 for freezeTB.
To make life easier and support the GUI wrapper, freezeTB
  installs the reference
  in `/usr/local/share/freezeTBFiles/NC000962.fa` for
  global installs
  or `~/Documents/freezeTBFiles/NC000962.fa` for local
  installs.
So, the final minimap2 command might look
  like `minimap2 -a -x map-ont /usr/local/share/freezeTBFiles/NC000962.fa barcode01.fastq.gz > barcode01.sam`.
You will notice I did a file redirect here to avoid
  minimap2 printing to the screen.
We can then run freezeTB on the sam file
  using `freezeTB -sam barcode01.sam -prefix barcode01-freezeTB`.
Were `-sam` tells the sam files name and `-prefix` is what
  we want to name the output.

# Running freezeTB

One thing you may have noticed is that minimap2 outputs
  to the terminal.
As it turns out that output can be redirected to freezeTB
  uses and symbol called a pipe `|`.
This would look something 
  like `minimap2 -a -x map-ont /usr/local/share/freezeTBFiles/NC000962.fa barcode01.fastq.gz | freezeTB -prefix barcode01-freezeTB`.

- pipe `|`: redirects terminal output from one program to
  be input of another program (if the other program
  supports it).
  - Think of it as I am uses pipes to connect programs
    into an pipeline
  - All programs run at the same time so it does speed
    things up, but also increase memory usage
  - `minimap2 -a ref.fa reads.fastq | freezeTB`
  - Most of the standard Unix tools can take input from
    an pipe.

# Wrap up

At this point you have run freezeTB.
Now you can take an look at the files.
The files that have "con" in the name are from
  consensuses, while the files with "read" are from the
  reads.

In the next and last part I will show you how to automate
  this so that you can process an entire runs barcodes
  with a single bash script.

# The sam file (extra)

- sam file: a tab deliminated file (tabs separate columns)
  that has a set of reads mapped to one or in some cases
  more references. Each line is either a comment
  (starts with an '@') or an mapped (aligned) or unmapped
  (not aligned) read.
  - column 1 is the read id
  - column 2 is the flag
    - 0 is an mapped forward read
    - 4 is an unmapped read
    - 16 is an mapped reverse complement read
      - minimap2 always converts reverse complement reads
        to the forward direction
  - column 3 is the reference id
  - column 4 is the starting position on the reference
  - column 5 is the mapping quality (how well read mapped
    to the reference)
  - column 6 is the cigar (condensed alignment) or is '\*'
    if the read is unmapped. It has a series of
    M, X, =, I, D, S, and H's that have a number before
    them.
    - the number is the number of times that the letter
      (M, I, D, X, =, S, H) is repeated
      (ex 13M2I3D is MMMMMMMMMMMMIIDDD)
    - M is a match or mismatch (13M = 13 matches or SNPs)
    - I is a insertion (2I = 2 insertions)
    - D is a deletion  (3D = 3 deletions)
    - S is a soft mask; not aligned, but still in sequence
        - always at ends of sequences
    - H is a hard mask; not aligned and removed from
        sequence
        - always at ends of sequences
    - = is a match; eqx cigar only
    - X is a mismatch; eqx cigar only
  - column 7 to 9 I have never really used. I think they
    are for chromosomes or paired reads.
  - column 10 is the sequence or is an "\*" if no sequence
  - column 11 is the Q-score line or is "\*" if missing
  - all other columns are extra entries that may be
    present, but not required
