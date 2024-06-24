# Goal:

In this episode I am going to build a do it yourself (diy)
  tuberculosis pipeline in bash.
I am limiting myself to only two programs of my own,
  tbSpoligo (spoligotyping) and tbAmr (AMR detection).

And no, this was not made just to get six episodes.
This was planned before I did the corny six episode joke.

Behold power of this barely operational command line.

# Introduction:

One decision I made when writing feezeTB was to make it
  modular.
This means that instead of being one giant program, it
  is composed of several smaller programs that can be
  used separately (my definition might be wrong).
Because of this, you can compile each component of
  freezeTB separately and run it on its own.
The one exception is the read filtering step, but I have
  include and extra program that does this as well.

In this demo, I am going to only use my spoloigytping
  module and my AMR detection module.
All the other programs I use will be from other people.
Though, before I start I will note, that I have not tested
  this script.
Also, I should note that I am setting this up assuming
  you sequence the full tuberculosis genome (not
  amplicons).

So, lets begin by adding our shebang and our basic
  variables (directory, prefix, reference, and help
  message [I often do this last]).
Basically copy the script from the end of episode 4 a new
  problem, but remove the feezeTB call.

```
#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declarations
#   o sec02:
#     - check user input
#   o sec03:
#     - prepare input for freezeTB and run freezeTB
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# input user modifies
fqPassDirStr=""; # fastq_pass folder basecaller made
prefixStr="out"; # what to call everything

# input user does not modify
minReadsUI="1000"; # minimum reads to run freezeTB

# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

numReadsUI="0"; # not needed, but I like pre-declaring

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     
while [[ "$#" -gt 0 ]]; do
   case $1 in
      -dir) fqPassDirStr="$2"; shift;;
      -d) fqPassDirStr="$2"; shift;;

      -prefix) prefixStr="$2"; shift;;
      -p) prefixStr="$2"; shift;;

      -h) printf "%s\n" "$helpStr"; exit;;

      *) printf "%s not recognized\n" "$1"; exit;;
   esac

   shift; # move to the next argument
done

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - prepare input for freezeTB and run freezeTB
#   o sec03 sub01:
#     - merge files and check number of reads
#   o sec03 sub02:
#     - run minimap2 and freezeTB
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files and check number of reads
#*********************************************************

# merge the fastq files in an barcode together
cat "$fqPassDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

numReadsUI="$( \
   wc -l "$prefixStr.fastq" |
     cut -f 1 -d " " \
);" # get the line counts

numReadsUI="$((numReadsUI / 4))"; # divide read count by 4

if [[ "$numReadsUI" -lt "$minReadsUI" ]]; then
   printf "ERROR to few reads %s\t%s\n" \
       "$prefixStr" \
       "$numReadsUI";
   rm "$prefixStr.fastq"; # remove the old file
   exit;
fi # if there were not enough reads to run freezeTB

# print out the prefix, tab "\t", and the number of lines
printf "%s\t%s\n" "$prefixStr" "$numReadsUI";

```

# Filtering

## Filtering fastq files with filtlong

The fist filtering step I am going to use is filtlong
  from Ryan Wick
  [https://github.com/rrwick/flitlong](
   https://github.com/rrwick/filtlong
  ).
We are going to filter out merged fastq file with filtlong
  filters fastq.
Sadly, filtlong does not take stdin input, so I can not
  use an pipe.

### Installing filtlong

The README.md file tells us what we need to do to run and
  install filtlong.
It also tells us that we have two dependences.

The first dependency is a c++ complier, which is present
  for Linux and is used go build filtlong.
If this ends up being an issue for Mac, it may not be,
  do `brew install gcc`, which should install `g++` with
  `gcc`.
The second dependency is zlib, which should be on Mac.
However, on Linux you may need the devel version, which
  includes files for integrating zlib into other
  programs.
For Ubuntu/Debian do `sudo apt-get install zlib-devel`.

You can clone flitlong from github with `git clone`.
If you can not remember how to use `git` see episode 1
  the installation menace (it is git clone web address).
You can then build filtlong doing `make`.
You will need to manually install flitlong, which means
  copying it to the install location.
If you get an error saying something like <stdint.h>
  missing from kmer.h, then add `#include <stdint.h>`
  after the `#define KMERS_H` (after line 17) in
  `src/kmer.h`.
You will also get a bunch more errors with it, these are
  all coming from the missing library.
To make life easier I included the commands to fix kmer.h.

```
# I am using sed to append the needed line after line 20
sed '20a#include <stdint.h>' src/kmer.h > tmp.h
   # in 20a
   #   - 20 is the line number
   #   - "a" tells sed to append the line
   #     - we could use i to insert the line before 20
   #   - the line(s) to add comes after the "a"
   #   - you can use \ to split lines (like in bash)
   #     - each \ marks a new line to insert

# move new file to src/kmer.h
cp src/kmer.h src/oldKmer.h; # back up kmer.h
mv tmp.h src/kmer.h
  # moves an file. In this case we are overwriting kmer.h
  # with tmp.h

# sed can modify the input file with `-i`, however, this
#  is not good practice. It is to easy to mess things up
#  with sed.
```

To install flitlong we will do the same steps we did in
  episode 3 revenge of the command line to install our
  bash script.
Use `cp` to copy filtlong to its install location and
  then `chmod` to set its permissions to execute (run).

**Wait a few seconds then
  do `sudo cp filtlong /usr/local/bin`
  and `chmod a+x /usr/local/bin/filtlong`**

### Adding flitlong in

From the README.md file for filtlong I find out I can
  filter by mean Q-score using the `--mean_mean_q` flag.
I can also filter by read minimum length using
  `--min_length` flag.
I am going to add these values to our scripts, with some
  good default values.
Also, I am going to remove the gunzip step, because
  filtlong will unzip the fastq file for me.
Finally, I also am going to add an "filtlong" to the end
  of the name so I keep track of the program I used for
  this step.

In the new edition of episode 6, I have not only added in
  more content, but also have decided to only show what
  I am changing.

```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# REMOVED TO KEEP CODE SHORT

# for filtlong
minMeanQUI="7";
minLenUI="200";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - prepare input for freezeTB and run freezeTB
#   o sec03 sub01:
#     - merge files, call filtlong, and check read count
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files, call filtong and check number of reads
#*********************************************************

# merge the fastq files in an barcode together
cat "$fqPassDirStr/"*".fastq.gz" > "$prefixStr.fastq.gz";

filtlong \
    --min_mean_q "$minMeanQUI" \
    --min_length "$minLenUI" \
    "$prefixStr.fastq.gz" \
  > "$prefixStr-filtlong.fastq";

numReadsUI="$( \
   wc -l "$prefixStr.fastq" |
     cut -f 1 -d " " \
);" # get the line counts

numReadsUI="$((numReadsUI / 4))"; # divide read count by 4

if [[ "$numReadsUI" -lt "$minReadsUI" ]]; then
   printf "ERROR to few reads %s\t%s\n" \
       "$prefixStr" \
       "$numReadsUI";
   rm "$prefixStr.fastq"; # remove the old file
   exit;
fi # if there were not enough reads to run freezeTB

# print out the prefix, tab "\t", and the number of lines
printf "%s\t%s\n" "$prefixStr" "$numReadsUI";
```

## Filtering sam files with samtools

After our fitlong step, we have removed the low quality
  reads.
However, we could still have reads that are from other
  bacteria or even humans.
Of course, if you had an pure culture, all of this is of
  very little concern.

We can removing non-tuberculosis bacterial reads and human
  reads using samtools, which is designed to work on sam
  files.
To do this, we first find what maps to to the tuberculosis
  reference using minimap2 or another read mapper.
We can then use samtools to remove any read that did not
  map.
Also, we can remove reads that mapped poorly (low mapping
  qualities) to the tuberculosis genome by removing reads
  with low mapping qualities.
However, this will not remove all off target 16sRNA
  amplicons from non-tuberculosis reads.

There also is another cost to doing this.
By removing reads that did not map or that mapped poorly
  to the reference we could be removing inversions or
  large insertions.
So, we are adding in an reference basis to our data.

One trick to reduce reference bias would be to use
  multiple references.
We would have to make sure that the references represent
  the full, known diversity.
However, at the same time every extra reference means more
  time and more memory, so we can not use every reference.
So, we need to do some downsampling.
This is were an program like cd-hit, which clusters
  sequences might come in handy
  [https://github.com/weizhongli/cdhit](
   https://github.com/weizhongli/cdhit)
  ).

For this tutorial I am going to keep things simple and
  stick with only one reference.

### installing samtools

There are several ways to install samtools.
The first and easiest way is to install it through your
  repositories.
For Ubuntu/Debian this is (sudo apt-get install samtools`.
For Mac this is `brew install samtools`.

You can also install samtools by source, but then you also
  have to keep it up to date.
It is a bit harder then the previous installs, but not
  bad.
First you will need to install `htslib` (follow
  instructions in the README.md file).
Then follow the instructions in samtools README.md file.
You may have to install some dependencies, but after that
  you should be ok.

### samtools

To remove unmapped reads, I am going to
  use `samtools view -F 4`
  (4 is for unmapped reads, see
   [https://broadinstitute.github.io/picard/explain-flags.html](
    https://broadinstitute.github.io/picard/explain-flags.html)
   for an full list of flags).
I am also going to remove reads with low mapping qualities
  (how well aligned) using `-min-MQ 15`.
Samtools requires a sam file, so I need to using a read
  mapper like minimap2.

Our the consensus builder I am going to use, flye takes
  an fasta file or fastq file, so we need to convert the
  sam file to an fastq file.
So, I will use `sam fastq` to get an fastq file.
However, `sam fastq` needs an bam (binary sam) file, so I
  will have `samtools view` output an bam file with `-b`.

# Building and consensus/variant calling

I am going to use flye 
  ([https://github.com/fenderglass/flye](
    https://github.com/fenderglass/flye
  ) to build my initial (draft) consensus.
I am then going to polish my draft consensus with medaka
  ([https://github.com/nanoporetech/medaka](
    https://github.com/nanoporetech/medaka)
  ).
In particular I am going to use medaka_consensus, since it
   will output an consensus genome.

Flye is a denovo assembler, which means is does not use an
  reference genome.
So, flye will try to assemble a consensus genome for
  every organism in the sample.
In this case I do not want to build genomes for
  non-tuberculosis reads.
So, I am going to filter out any reads that did not map
  to the tuberculosis reference genome.
However, one downside of this is that I may remove reads
  with real, large insertions that did map well.
I could overcome this somewhat by include a reference
  database with multiple tuberculosis references and then
  using the main reference for AMR detection.
These references would have to be different enough that
  I capture a wide range of the tuberculosis diversity
  (try something like cd-hits
  [https://github.com/weizhongli/cdhit](
   https://github.com/weizhongli/cdhit)
  ).
However, I am a bit lazy here, so I will leave that for
  you to implement.

### Filtering reads with samtools

For filtering, I am going to
  use `samtools view -F 4` to remove unmapped reads
  (4 is for unmapped reads, see
   [https://broadinstitute.github.io/picard/explain-flags.html](
    https://broadinstitute.github.io/picard/explain-flags.html)
   for an full list of flags).
I am also going to add `-min-MQ 15` to keep reads with an
  mapping quality (how well the read mapped to the
  reference) above 15.
Samtools requires a sam file, so I need to using a read
  mapper like minimap2.
The one problem with this though is that flye requires an
  fasta or fastq file.

Ideally, I would convert the sam file to a fastq file
  with `samtools fastq`.
However, the sam file is missing the meta data, which
  includes to model for basecalling.
This model will make life easier in the polishing step.
So, I am going to use one of my own programs, fqGetIds,
  which can take a sam file and use the read ids in it
  to extract reads from an fastq file.

I am also going to do my read filtering before I check
  the read counts.

### Installing fqGetIds

First clone the fqGetIds repository
  [https://github.com/jeremybuttler/fqGetIds](
   https://github.com/jeremybuttler/fqGetIds
  ).
Then for non-Mac users build fqGetIds with do
  `make sse`.
For Mac users (Mac only; not Linux users with ARM
  architectures), remove the `-static \` (line 5) from
  Makefile.
Then do `make neon` (ARM architectures only).
Finally install fqGetIds with `sudo make install`.

NOTE: do not use `make avx512` (I was never able to test
  it)

### Running fqGetIds

Technically fqGetIds was not designed to get read ids from
  sam files.
However, the format of input fqGetIds was designed for
  is close enough so that it can use an sam file.
This allows me to skip the step of extracting read ids.
You can specify read id input with `-f file.sam`, but if
  you want to use `stdin` (terminal input), then you can
  use `-stdin-filt`.
The only other input is an fastq file, which you can
  provide with `-fastq`.

An alternative would be seqkit, which is a tool kit for
  manipulating fastq files.
However, I am not sure if it can get read ids from sam
  files.
Also, for ONT data, non-scaler fqGetIds (sse, avx2, or
  neon) is faster than seqkit.
However, the default is a little slower and takes a bit
  more memory.
I am hoping to fix this in the future.
 
```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - prepare input for freezeTB and run freezeTB
#   o sec03 sub01:
#     - merge files and filter out low quality reads
#   o sec03 sub02:
#     - check if have enough reads
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files and filter out low quality reads
#*********************************************************

# merge the fastq files in an barcode together
cat "$fqPassDirStr/"*".fastq.gz" > "$prefixStr.fastq.gz";

filtlong \
    --min_mean_q "$minMeanQUI" \
    --min_length "$minLenUI" \
    "$prefixStr.fastq.gz" \
  > "$prefixStr-filtlong.fastq";

prefixStr="$prefixStr-filtlong"; # build up my name

# filter out reads that did not map or had mapping
# qualities under minMapqUI

minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  samtools view \
    -F 4 \
    -min-MQ "$minMapqUI" \
    - |
  fqGetIds \
    -stdin-filt -
    -fastq "$prefixStr.fastq" \
  > "$prefixStr-sam.fastq";

prefixStr="$prefixStr-sam";

```

# Building the consensus

## Flye, get the draft consensus

Flye is a denovo (does not use an reference) assembler we
  can use to build a consensus genome.
It is also a metagenomic assembler (samples with more than
  one organism).
Some alternatives are `raven`, `canu`, and `redbean`.

Readbean is a fast denove assembler that  has low memory
  requirements, but does not produce very good results.
Overall, I would not recommend readbean.

Raven is another fast denovo assembler with low memory
  requirements.
For metagenomics, raven is not as good as flye, but is ok.
I am not sure how well raven does on non-metagnomic
  assemblies.

Canu includes an read correction step an used to be the
  preferred assembler.
However, though canu produces good results, it also takes
  much more time and memory than flye.
Now days flye produces good results and is preferred over
  canu.

One downside of denovo assemblers, is that they do not
  work as well with sort reads, such as amplicons.
This is due to needing longer overlap lengths then are
  in the amplicons.
So, if we were doing amplicons we would have to use a
  different method to build our consensuses.

### Installing flye

Flye's github page (the web address to use with git) is
  [https://github.com/fenderglass/flye](
   https://github.com/fenderglass/flye).
The README.md file mentions that the install instructions
  for flye are in docs/INSTALL.md.
You should have most of the dependencies, if not try to
  install the build essentials tools (I can not remember
  the name).
I think flye is often in repositories, so you might be
  able use `apt` for Ubuntu/Debian or `brew` to install
  flye.
It looks like you can do an global
  install `python setup.py install`.
However, if it errors out for Mac, we could try an local
  install.

You can then build an local install of flye using `make`
  for Linux.
However, for Mac or other systems with ARM architectures,
  you will need to use `make arm_neon=1 aarch64=1` flye.
This is the same command you need to use to build minimap2
  from source on 64 bit ARM architectures.

You should not get this for Ubuntu/Debian or Mac, but if
  you get "missing #include <execinfo.h>" then try
  installing `glib` with brew.
The error is because <execinfo.h> is a library that is
  unique to to Linux, but I think it is in the glib
  system.
So, on Mac, you might be able to get around this by
  installing `gcc` with `brew`, which will should include
  glib.
Then do `make CC=gcc CXX=g++ arm_neon=1 aarch64=1`.
Were CC is your c compiler you want to use and CXX is the
  c++ compiler you want to use.

I could not get past this error, because I am using an
  system with musl instead of glib (I know this is over
  your head).
But, you should be able to install flye by doing
  doing `sudo cp bin /usr/local/bin`
  and `sudo cp lib /usr/local/lib`.
After that, set the file permissions to execute with
  `sudo chmod a+rx` for files and `chmod -R a+rx` for
  directories.

- chmod -R: sets permissions for the directory and every
  file/directory in the directory.

### Using flye

Reading though their github page it looks like I need to
  specify `--nano-hq reads.fastq`, since we will likely be
  using supper accuracy based reads or Q20 reads.
Also, because we will likely be using Q20 reads, we need
  to add `--read-error 0.03`.
That being said if you expect to use lower accuracy reads
  then use `--nano-raw reads.fastq`.
We will also need to give an output directory with
  `--out-dir name`.
We can put this all together to call flye in our script.

Ideally we would allow the user to select the accuracy
  mode by using an if statement or by having an variable
  hold the assembled name.
Just make sure that the variable is never blank, otherwise
  you can get some odd errors when you do line breaks.

```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

flyeFlagsStr="--nano-hq";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [[ "$#" -gt 0 ]]; do
   case $1 in
      -q20) flyFlagsStr="--read-error 0.03 --nano-hq";;
      -high-accuracy) flyFlagsStr="--nano-hq";;
      -low-accuracy) flyFlagsStr="--nano-raw";;
      # I am putting the --nano-hq option at the end so
      #   that I can add in the fastq file later
      # OTHER INPUT OPTIONS REMOVED TO KEEP CODE SHORT
   esac 
done

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - prepare input for freezeTB and run freezeTB
#   o sec03 sub01:
#     - merge files and filter out low quality reads
#   o sec03 sub02:
#     - check if have enough reads
#   o sec03 sub03:
#     - build the consensus
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files and filter out low quality reads
#*********************************************************

# FILTERING CODE WOULD BE HERE
# REMOVED TO KEEP CODE SHORT

prefixStr="$prefixStr-sam";

# set up the flags for flye
flyeFlagsStr="$flyeFlagsStr $prefixStr.fastq";

#*********************************************************
# Sec03 Sub02:
#   - check if have enough reads
#*********************************************************

# REMOVED TO KEEP CODE SHORT

#*********************************************************
# Sec03 Sub03:
#   - build the consensus
#*********************************************************

flye --out-dir "$prefixStr-flye" "$flyeFlagsStr";
```

## Polishing; improving the draf consensus accuracy

The assembly made by flye is pretty good.
However, it can be often improved further by a polishing
  step.
We can do this using medaka, an program made by ONT.

### Installing medaka

It has been awhile since I last installed medaka, however,
  I do not have fond memories of installing it.
The only thing I can say is read the README.md and try
  the pip install (this looks nicer then it used to be).
Just do not do the conda install, because in an script or
  program we have to add `eval "$(conda shell.bash hook)"`
  to be able to do `conda activate medaka`.
The url to clone medaka with git is
  [https://github.com/nanoporetech/medaka](
   https://github.com/nanoporetech/medaka).

### Using medaka

The assembly output from flye that will will polish up
  with medaka is assembly.fasta.
It is saved in in the output directory made  by flye
  ("$prefixSTr-flye/assembly.fasta").

We will use the medaka\_consensus command for polishing,
  since it will output a consensus.
medaka\_consensus takes in the reads as `-i reads.fastq`,
  the draft assembly from
  flye `-d $prefixStr-flye/assembly.fasta`, the model we
  used in basecalling `-m modelname`, and the output
  directory to make `-o output`.
We can also modify the number of threads used with `-t`.

The consensus built by medaka will by saved in the output
  directory as "consensus.fasta".

Thankfully, from medaka's github it looks like the more
  recent basecallers will output the model in the fastq
  file.
And when we check our fastq files we can see that it is
  true, even after filtlong has run.
Even better, it looks like medaka will be able to figure
  this out, so we do not need to use the `-m` option
  to supply the model.

Of course I am assuming recent data is being used.
If you know it will not be, then you best provide a
  model option to let the user pick the model they
  can choose.
Picking the wrong model can be bad.

```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - prepare input for freezeTB and run freezeTB
#   o sec03 sub01:
#     - merge files and filter out low quality reads
#   o sec03 sub02:
#     - check if have enough reads
#   o sec03 sub03:
#     - build the consensus
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files and filter out low quality reads
#*********************************************************

# REMOVED TO KEEP CODE SHORT

#*********************************************************
# Sec03 Sub02:
#   - check if have enough reads
#*********************************************************

# REMOVED TO KEEP CODE SHORT

#*********************************************************
# Sec03 Sub03:
#   - build the consensus
#*********************************************************

flye --out-dir "$prefixStr-flye" "$flyeFlagsStr";

# I am not updating my prefix here because I need the
# reads for medaka

# activate the medaka environment, at least this is how
# you used to do it. Not sure if is current

source "$HOME/medaka/venv/bin/activate";
   # $HOME holds the path to your home directory

medaka_consensus \
    -i "$prefixStr.fastq" \
    -d "$prefixStr-flye/assembly.fasta" \
    -out-dir "$prefixStr-flye-medaka";

deactivate; # should exit the medaka enviroment
   # for conda this would be `conda deactivate`

# copy the consensus out of medaka's output so I can work
# with it easier later
cp \
    "$prefixStr-flye-medaka/consesnsus.fasta" \
    "$prefixStr-flye-medaka.fasta";

prefixStr="$prefixStr-flye-medaka";
```

# freezeTB module time

Finally, it is time to use the modules from freezeTB.
Go into the freezeTB directory (I saved it in
  ~/Downloads).
Then install tbSpoligo, tbAmr, and all the other other
  freezeTB side programs and scripts (not used here) using
  `make modules`.
The install all the programs and scripts
  using `sudo make moduleinstall`.
For local installs,
  do `make moduleinstall PREFIX=/path/to/install`.

## tbSpoligo; spoligotping

tbSpoligo requires two items to be input, but I would
  recommend three.
It requires a fasta `-fa file.fasta`,
  a fastq `-fq file.fastq`, or a sam file `-sam file.sam`.
In this case we are just comparing an reference, so I am
  going to input an fasta file.

We also need to provide the spoligotyping spacer
  sequences (`-spoligo spacers.fasta`).
These are installed with freezeTB
  to `/usr/local/share/freezeTBFiles/spoligotype-seq.fa`.

Finally, the option item that I would recommend is an
  database of lineages to search for matches
  (`-db database.csv`).
This was also installed with freezeTB and can be found
  at `/usr/local/share/freezeTBFiles/spoligo-linages.csv`.
The database is taken from TBProfiler.

So, all I need to do is call tbSpoligo and then provide
  the needed files and then I should be good.

```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# REMOVED TO KEEP CODE SHORT

# spacers sequences for tbSpoligo
spacersFqStr="usr/local/share/freezeTBFiles/NC000962.fa";

# linage database for tbSpoligo
dbStr="usr/local/share/freezeTBFiles/spoligo-linages.csv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - gets spoligotypes and ARMs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# find the spoligotypes
tbSpoligo \
    -spoligo "$spacersFqStr" \
    -db "$dbStr" \
    -fa "$prefixStr.fasta" \
  > "$prefixStr-spoligotype.tsv";
```

## Adding the AMR checks

Finally, I will wrap this up by adding in the checks for
  AMRs.
To use tbAmr we need to provide an sam file.
The sam file can be from stdin, so we can redirect output
  from minimap2 to tbAmr.
We will want to specify that we are using an consensus
  file, so do `-sam-con -` (`-` at the end is for stdin).

We will also have to provide an table of AMR mutations to
  search through (`-amr-tbl`).
The 2023 tuberculosis catalog from the WHO can be found
  in `/usr/local/share/freezeTBFiles/who-2023.tsv`.

```
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# REMOVED TO KEEP CODE SHORT

# spacers sequences for tbSpoligo
spacersFqStr="usr/local/share/freezeTBFiles/NC000962.fa";

# linage database for tbSpoligo
dbStr="usr/local/share/freezeTBFiles/spoligo-linages.csv";

# AMR table for tbAmr
amrStr="usr/local/share/freezeTBFiles/who-2023.tsv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - gets spoligotypes and ARMs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# find the spoligotypes
tbSpoligo \
    -spoligo "$spacersFqStr" \
    -db "$dbStr" \
    -fa "$prefixStr.fasta" \
  > "$prefixStr-spoligotype.tsv";

minimap2 \
    -a \
    "$refStr" \
    "$prefixStr.fasta" |
  tbAmr \
    -amr-tbl "$amrStr" \
    -sam-con - \
  > "$prefixStr-AMRs.tsv";
```

# The final; complete script

This is what the final script looks like when we stop
  removing parts of it.

```
#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declarations
#   o sec02:
#     - check user input
#   o sec03:
#     - prepare input for freezeTB and run freezeTB
#   o sec04:
#     - gets spoligotypes and ARMs
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# input user modifies
fqPassDirStr=""; # fastq_pass folder basecaller made
prefixStr="out"; # what to call everything
flyeFlagsStr="--nano-hq";

# input user does not modify, but should

# for filtlong
minMeanQUI="7";
minLenUI="200";

minReadsUI="1000"; # minimum reads to run freezeTB
minMapqUI="15";    # minimum mapping quality

# spacers sequences for tbSpoligo
spacersFqStr="usr/local/share/freezeTBFiles/NC000962.fa";

# linage database for tbSpoligo
dbStr="usr/local/share/freezeTBFiles/spoligo-linages.csv";

# AMR table for tbAmr
amrStr="usr/local/share/freezeTBFiles/who-2023.tsv";

# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

# input the user does not care about
numReadsUI="0"; # not needed, but I like pre-declaring

# more better help message; I improved this at the end
helpStr="$(basename "$0") /path/to/fastq.gz/dir prefix
  - builds a consensus and then detects AMRs and the
    consensus and the consensus spoligotype.
Input:
  -dir /path/to/fastq.gz/dir: [Required]
    o path to the directory with the fastq.gz files to
      look for spoligotypes and AMRs in
  -q20: [No]
    o using Q20 (high accuracy) Nanopore reads
  -hq: [Yes]
    o using high quality Nanopore reads
  -raw: [No]
    o using older, no supper accuracy bases called
      Nanopore reads
  -prefix "name": [$prefixStr]
    o prefix to add to the outputs name
  -h:
    o print this help message
Output:
  - builds a consensus using flye and medaka 
  - reports detected AMRs and spoligotypes
";

# I am using "$(basename "$0")" to get the name of the
#   script.
# I could use "$(dirname "$0")" to get the location of
#   the script.

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     
while [[ "$#" -gt 0 ]]; do
   case $1 in
      -dir) fqPassDirStr="$2"; shift;;
      -d) fqPassDirStr="$2"; shift;;

      -q20) flyFlagsStr="--read-error 0.03 --nano-hq";;
      -high-accuracy) flyFlagsStr="--nano-hq";;
      -hq) flyFlagsStr="--nano-hq";;
      -low-accuracy) flyFlagsStr="--nano-raw";;
      -raw) flyFlagsStr="--nano-raw";;

      -prefix) prefixStr="$2"; shift;;
      -p) prefixStr="$2"; shift;;

      -h) printf "%s\n" "$helpStr"; exit;;

      *) printf "%s not recognized\n" "$1"; exit;;
   esac

   shift; # move to the next argument
done

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - build the consensus genome
#   o sec03 sub01:
#     - merge files and filter out low quality reads
#   o sec03 sub02:
#     - check if have enough reads
#   o sec03 sub03:
#     - build the consensus
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - merge files and filter out low quality reads
#*********************************************************

# merge the fastq files in an barcode together
cat "$fqPassDirStr/"*".fastq.gz" > "$prefixStr.fastq.gz";

filtlong \
    --min_mean_q "$minMeanQUI" \
    --min_length "$minLenUI" \
    "$prefixStr.fastq.gz" \
  > "$prefixStr-filtlong.fastq";

prefixStr="$prefixStr-filtlong"; # build up my name

# filter out reads that did not map or had mapping
# qualities under minMapqUI

minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  samtools view \
    -F 4 \
    -min-MQ "$minMapqUI" \
    - |
  fqGetIds \
    -stdin-filt -
    -fastq "$prefixStr.fastq" \
  > "$prefixStr-sam.fastq";

  # the - at the end of the samtools command is to
  # make sure it is taking stdin (other programs output)
  # input

prefixStr="$prefixStr-sam";

# set up the flags for flye
flyeFlagsStr="$flyeFlagsStr $prefixStr.fastq";

#*********************************************************
# Sec03 Sub02:
#   - check if have enough reads
#*********************************************************

numReadsUI="$( \
   wc -l "$prefixStr.fastq" |
     cut -f 1 -d " " \
);" # get the line counts

numReadsUI="$((numReadsUI / 4))"; # divide read count by 4

if [[ "$numReadsUI" -lt "$minReadsUI" ]]; then
   printf "ERROR to few reads %s\t%s\n" \
       "$prefixStr" \
       "$numReadsUI";
   rm "$prefixStr.fastq"; # remove the old file
   exit;
fi # if there were not enough reads to run freezeTB

# print out the prefix, tab "\t", and the number of lines
printf "%s\t%s\n" "$prefixStr" "$numReadsUI";

#*********************************************************
# Sec03 Sub03:
#   - build the consensus
#*********************************************************

flye --out-dir "$prefixStr-flye" "$flyeFlagsStr";

# I am not updating my prefix here because I need the
# reads for medaka

# activate the medaka environment, at least this is how
# you used to do it. Not sure if is current

source "$HOME/medaka/venv/bin/activate";
   # $HOME holds the path to your home directory

medaka_consensus \
    -i "$prefixStr.fastq" \
    -d "$prefixStr-flye/assembly.fasta" \
    -out-dir "$prefixStr-flye-medaka";

deactivate; # should exit the medaka enviroment
   # for conda this would be `conda deactivate`

# copy the consensus out of medaka's output so I can work
# with it easier later
cp \
    "$prefixStr-flye-medaka/consesnsus.fasta" \
    "$prefixStr-flye-medaka.fasta";

prefixStr="$prefixStr-flye-medaka";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - gets spoligotypes and ARMs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# find the spoligotypes
tbSpoligo \
    -spoligo "$spacersFqStr" \
    -db "$dbStr" \
    -fa "$prefixStr.fasta" \
  > "$prefixStr-spoligotype.tsv";

minimap2 \
    -a \
    "$refStr" \
    "$prefixStr.fasta" |
  tbAmr \
    -amr-tbl "$amrStr" \
    -sam-con - \
  > "$prefixStr-AMRs.tsv";
```

# Ending comments

At this point we have made an script to run an
  tuberculosis analysis pipeline on an whole genome
  sequence tuberculosis sample.
You could easily modify this to change the filtering
  methods to use other programs.
Or the consensus building method to be a different
  program.
You could even add in steps to do additional analyses on
  the output or extra analyses steps, like something to
  do with mixed infections, MIRU-VNTR lineages, or
  something else.
You could call this bash script in your script for an
  entire sequencing run and walk away.
Basically, your goals, computer resources, time, and your
  imagination are your limits.

However, there are some comments I should make.
When I went though this script with you I did it all in
  one go.
However, in real life I would have built it up and tested
  it step by step.
I would have first ran the filtlong commands and made sure
  they worked.
To be honest I did run filtlong just to make sure that
  it did not remove any meta data from the header of
  the fastq files.
Then I would have run the minimap2 and samtools filtering
  commands.

After that I would have run flye separately.
Once I made sure flye worked I would have integrated it
  in and still made sure it worked in the script.
Then I would have done the same with medaka, after that
  tbSpoligo, and after that tbAmr.

This slower approach would allow me to deal with bugs as
  they pop up.
Instead of trying to find them as I run a large script
  for the first time.

Which brings me to some advice about dealing with bugs.
One way is to add printf commands to find were the bug
  is happening at.
Then you can inspect the variables by printing them out
  to see what is happening.
After that you can run the lines in that part of your
  program separately to see if you can find it.
This can be painful at times and does not always help, but
  it is an start.
In the end debugging is all about banging your head
  against the wall till you find out what went wrong and
  how to fix it or find another solution.

The best way to avoid bugs, is to catch them before you
  run your code.
This sounds easier said then done, but there are tools out
  there to help you.
For bash, shellcheck is a goo resource.
Shellcheck checks your scripts for common errors can be a
  life saver here.
It will also make suggestions on how to improve your
  codding style.
You can find an online version of shell check at
  [https://www.shellcheck.net](
   https://www.shellcheck.net).
Or you can install it from a local repository
  repository `sudo apt-get install shellcheck`
  or for Mac `brew install shellcheck`.

I actually did run the scripts here though shellcheck and
  was able to catch a few errors.
Though the activating medaka line did raise an warning,
  but that is something that could not be helped.
To be honest that is the line I am not sure if will work.

Good luck

And remeber if you want to install this script
do `sudo cp script_name.sh /usr/local/bin`
and `sudo chmod a+x /usr/local/bin/script_name.sh`
