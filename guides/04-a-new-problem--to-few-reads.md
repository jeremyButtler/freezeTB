# Goal

Use Unix tools to detect low read counts for an barcode

# Introduction

One problem we did not deal with in the last episode was
  what happens when I have to few reads?
Of course this means that freezeTB errors out, which is
  something we would like to avoid.
So, in this episode we will talk about how to detect
  the number of reads in an fastq file.

# Unzipping the fastq.gz file

Our first problem is that fastq.gz files are compressed.
This means that we either need an program to uncompress
  the fastq file and then a program to count the number
  of lines.
We can use `gunzip -c file.fastq.gz` (get help message
  with `gunzip --help`) to uncompress the fasta.gz file.
The `-c` is to force the file to be written to the
  terminal (stdout).
It also prevents gunzip does not delete the original
  file after it has converted it.
Another solution would be to redirect (pipe) the merged
  fastq.gz file to gunzip an save it as a fastq file.
Try adding in an gunzip command to your script.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat "$fqDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

# Getting the number of lines

The next step is to count the number of lines.
We can do this with `wc` (get help message
  with `wc --help`), which can count the number of lines,
  characters, and words in an file.
To count the number of lines use the `-l` parameter
  (`wc -l reads.fastq`).
Add in an step to get the number of lines from our merged
  fastq file into the script.
  
**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat "$fqDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

wc -l "$prefixStr.fastq";

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

Notice how we get both the line counts and the file name.
We will be storing the line counts in a variable, so
  we will need to remove the file name.
There are plenty of ways to do this.
We could use awk, which is a scripting language designed
  to process deliminated files such as the tsv (tabs
  separate columns) and csv (commas separate columns).
Or we could use cut, which also works with deliminated
  files.
To keep things simple I will redirect the output from
  `wc -l` to cut (check help message with `cut --help`).
However, `awk` is a tool that I often use on the command
  line, so it can be worth it to learn if you stick with
  bash.

The wc program separates the count and file name by an
  space, so our deliminator is an space.
However, the default deliminator for cut is a tab.
So, we need specify space as an deliminator with `-d " "`.

We also will need to specify the column (field) or range
  of columns (fields) we want to keep with `-f`.
In this case we want the first column `-f 1`.

Try adding the cut command to your script to just get the
  line count.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat "$fqDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

wc -l "$prefixStr.fastq" |
  cut -f 1 -d " ";

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

# Saving the number of lines

At this point we have the number of lines, but it is not
  in an variable.
We can store the results of bash commands in an variable
  by using `var="$(command)"`.
The dollar sign tells bash to evaluate the parentheses.
While the parentheses open a sub shell that run and then
  returns the result.
Try capturing the line count to an variable and then
  printing it to stdout (terminal) with printf or echo.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

numReadsUI="0"; # not needed, but I like pre-declaring

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat "$fqDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

numReadsUI="$( \
   wc -l "$prefixStr.fastq" |
     cut -f 1 -d " " \
);" # get the line counts

# print out the prefix, tab "\t", and the number of lines
printf "%s\t%s\n" "$prefixStr" "$numReadsUI";


# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

# Bash division

We now have the number of lines, but also have an problem.
The fastq file from Nanopore has four lines per read.
We can solve this by dividing the read count by 4.
To do math operations in bash we do `var="$((5 + 10))"` or
  for variables `var="$((var1 / var2))"`.
In this case we do not need to use "$" to access the
  variables, because the math operation will access them
  for us.
The only problem is that bash only works in whole numbers.
So, do something else if you want an decimal.
Add an adjustment for the four line counts to your script.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

numReadsUI="0"; # not needed, but I like pre-declaring

# really basic help message
helpStr="runFreezeTB.sh /path/to/fastq.gz/dir prefix";
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat "$fqDirStr/"*".fastq.gz" |
  gunzip > "$prefixStr.fastq";

numReadsUI="$( \
   wc -l "$prefixStr.fastq" |
     cut -f 1 -d " " \
);" # get the line counts

numReadsUI="$((numReadsUI / 4))"; # divide read count by 4

# print out the prefix, tab "\t", and the number of lines
printf "%s\t%s\n" "$prefixStr" "$numReadsUI";


# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

# Compare numbers

At this point we can finally see if we have enough reads
  to run freezeTB.
In an if statement, such
  as `if [[ "$var1" == "$var2" ]]; then` we are comparing
  two strings.
To compare numbers we have to replace the `==` with an
  number comparison operator.
These include `-eq` for equal, `-gt` for greater then,
  `-ge` for greater then or equal to, `-lt` for less then,
  and `-le` for less then or equal to.
Try adding an comparison step to make sure there are 1000
  reads.
If there are less, print out a warning and exit.
Keep the old print statement, so the user knows how many
  reads were present if there were enough reads.

Make sure the minimum value is stored in a variable at the
  top, just in case it needs to be modified later.

**Wait a few seconds**

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
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

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
     
if [[ "$fqDirStr" == "" ]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

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
cat "$fqDirStr/"*".fastq.gz" |
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

#*********************************************************
# Sec03 Sub02:
#   - run minimap2 and freezeTB
#*********************************************************

minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq |
  freezeTB -prefix "$prefixStr";
```

This code is small, so the extra documentation I added
  is probably to much.
Still, I like the visual breaks between major ideas.
However, it comes at the cost of adding more clutter, so
  I would not recommend this system to anyone else.

# Ending comments

We now have an script the checks to make sure there are
  enough reads before calling freezeTB.
In the next script I will set it up so we are running this
  script on an directory of barcodes. 
