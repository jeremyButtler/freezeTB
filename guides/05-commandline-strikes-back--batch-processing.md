# Goal

Process an entire sequencing run with your freezeTB script

# Introduction

It is finally time to move from processing a single
  directory with fastq files to processing a directory of
  directories with fastq files.
In real world this means processing the output of an
  single sequencing run.

# Looping

One problem about processing a directory of directories is
  that we have no idea how many we might be running.
So, we need a way for the system to get an list of
  directories and then run our freezeTB script on.
This is were for loops come in.
A for loop  is an loop that is designed to repeat an
  section of our script we meet an certain condition, such
  as going through all directories in an list of
  directories.

We can make a list of what we want to repeat.
First we want each run to have an separate name, otherwise
  we would just overwrite the previous run.
Ideally the name would be a combination of user input
  (prefix) and the directories name (barcodeXX).

Next, our scripts creates a merged fastq file and
  freezeTB creates a lot of small files.
So, we probably want to create an directory to store
  everything in.
Of course, bash does not like to create an directory
  twice, so we should have an if statement to check if
  the directory already exists before creating it.

We will want to run our freezeTB script on each directory.
Also, we will want to store the number of reads in
  each fastq that our script reports to a separate file.
That way the user can know why some directories are
  empty.

This might seem like a lot at first, but the trick with
  this is to split it into smaller steps. Lets start with
  just trying to get a loop up an running.

But before that, take some time and pause this video to
  add in the shebang (episode 3; revenage of the command
  line) and make the variables you expect to fill
  (episode 3) at the top of the script.
Also add in the user input checks (episode 3).

**Wait a few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
```

# Making a for loop

In bash a for loop has six parts.
It starts with `for`, is followed by an variable to store
  the value in `var`, this is followed by `in`, which is
  followed by the value to iterate though, which in our
  case is an list of directories.
The list of directories, which we can create by globing
  (using "\*"),  ends in an ';', which is followed by a
  `do`.
After the `do` we add our commands and end the loop with
  a `done`.
It looks something like this.

```
for holds_directory in ./"/path/to/fastq_pass/"*; do
   commands
done
```

- "./": means current directory
- holds\_directory: is an variable (you pick the name) that
  will hold the directory the for loop is currently on.

In this case this for loop would look at everything in
  the fastq\_pass directory.
This includes the unclassified reads, which could not be
  assigned to an barcode.
So, we may want to add something before the glob ("\*") to
  make it more specific to just barcodes.
Add the for loop to your code and have it print out the
  directory held in the for loops variable (I named it
  "holds\_directory" to make it more clear).

**Wait an few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   printf "%s\n" "$strBarDir";
done
```

We have now made an for loop that prints out the name of
  all the barcode directories in our fastq_pass
  directory.
This name is ok, because this is what the basecaller
  outputs.

# Getting an name

At this point we still need to get the name from the
  directory.
Thankfully, this is made easy by a program named basename
  (basename --help for help message).
Try using basename on the loop variable to get the barcode
  directories name.
Then store it in an variable, like we did in episode 4 a
  new problem.

You will want to save your new prefix name to an new
  variable.
That way you still keep the users input prefix.
 
**Wait an few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# not modified by user
outStr=""; # holds new prefix name

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   # grab the prefix to use for this barcode
   outStr="$prefixStr-$(basename "$strBarDir")";

   printf "%s\n" "$outStr";
done
```

# Making a new directory

At this point we need to make a new directory to store
  the results from our freezeTB script in.
However, we should only make it if it does not already
  exist.
We can do that using mkdir (see episode 1 the
  installation menace) and an if statement to check if
  an directory is not present (episode 3 reveange of
  the command line).
Name this directory with the users input prefix and
  the name of the input directory (the new variable).

I would also update your prefix/input (the new variable)
  name to have its name as both the directory and the
 path.
Just remember, the user could have input a file path for
  the prefix, so make sure to use basename (this episode;
  episode 5 command line strikes back).
You can also set a variable equal to its self, so
  `var1="$var-test-$var1" is an valid command.
Also directories in a path and the file name at the end
  are separated by a "/" in Unix (sadly this is not true
  for Windows).

Before you finish, you will also want to check for
  something called the null case.
The null case fires when the loop has nothing to do.
In this case we can check for the null case by checking to
  see if the loops variable does not hold a path to a
  directory (the same if check as above).
If it does not, then we want to stop (`break`) the loop.
Also add an if check to check for the null case and stop
  the loop.
It might also be nice, but not needed to let the user know
  what happened.

**Wait an few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# not modified by user
outStr=""; # holds new prefix name

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   if [[ ! -d "$strBarDir" ]]; then
      fprintf "No barcode directories in %s\n" \
         "$strBarDir"; # let user know the problem

      break; # stop loop
   fi # check if on null case

   # grab the prefix to use for this barcode
   outStr="$prefixStr-$(basename "$strBarDir")";

   if [[ ! -d "$outStr" ]]; then
      mkdir "$outStr";
   fi # make an new directory if it does not exist

   outStr="$outStr/$outStr"; # finish the prefix name
done
```

# The null case
**Wait an few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# not modified by user
outStr=""; # holds new prefix name

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   if [[ ! -d "$strBarDir" ]]; then
      fprintf "No barcode directories in %s\n" \
         "$strBarDir"; # let user know the problem

      break; # stop loop
   fi # check if on null case

   # grab the prefix to use for this barcode
   outStr="$prefixStr-$(basename "$strBarDir")";

   if [[ ! -d "$outStr" ]]; then
      mkdir "$outStr";
   fi # make an new directory if it does not exist

   # finish the prefix name
   outStr="$outStr/$(basename "$outStr")";
     # I am using basename here in case the user input
     # an file path, such as ~/Downloads/test for their
     # prefix. Basename will strip the path (~/Downloads)
     # and leave the output name (test)
done
```

# call the freezeTB script

Were finally read to call the freezeTB script.
Just add the call to your bash script into the loop.
Before doing this though, I would recommend installing
  your freezeTB script.
You can do this by

```
sudo cp freezeTB-script.sh /usr/local/bin;
sudo chmod a+x /usr/local/bin/freezeTB-script.sh;
```

Otherwise you will have to tell the script were the other
  script is.
You could also insert your other scripts code here, but
  then you would always need this diretory set up to use
  the first script.

You can save the number of reads to an file by doing an
  file redirect (`>>`; episode 1 the installatil menance)
  from your freezeTB script to an log file.
If you are really determined, you can capture this in an
  variable and then print it to the user with printf
  and to an file with printf.
Or you could use tee, which prints to files and the
  command line (`command | tee -a file.txt`).
For tee you need to pipe in the input.

**Wait an few seconds**

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr="$1"; # fastq_pass folder basecaller made
prefixStr="$2"; # what to call everything

# not modified by user
outStr=""; # holds new prefix name

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$fqPassDirStr" == ""]]; then
   printf "%s\n" "$helpStr"; # print the help message
   exit;
fi # user input nothing

if [[ ! -d "$fqPassDirStr" ]]; then
   exit; # the user did not input an fastq_pass directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   if [[ ! -d "$strBarDir" ]]; then
      fprintf "No barcode directories in %s\n" \
         "$strBarDir"; # let user know the problem

      break; # stop loop
   fi # check if on null case

   # grab the prefix to use for this barcode
   outStr="$prefixStr-$(basename "$strBarDir")";

   if [[ ! -d "$outStr" ]]; then
      mkdir "$outStr";
   fi # make an new directory if it does not exist

   outStr="$outStr/$outStr"; # finish the prefix name

   runFreezeTB.sh \
       "$strBarDir" \
       "$outStr" |
     tee "$prefixStr-log.tsv";
     # tee saves the read counts in "$prefixStr-log.tsv" and
     # prints them to terminal
done
```

# Final thoughts

At this point we can now process and entire directory of
  barcode directories that have fastq files.
You can even take this script further if you wanted to do
  even more additional analysis.
In the next episode, episode 6 return not, we will quickly
  build a custom TB pipeline.
 
We have come a long ways since episode 2 attack of
  freezeTB, but there is so much more to learn.
Lame, I know, but it allowed me reference all six episodes
  in one episode.

# Extra; a better method of getting input

An better way of getting input is to employ a loop called
  an while loop.
This loop will loop until a if statment is false.
In this case we can get the number of arguments the user
  input with `"$#"`.
We can then shift up one argument or parameter using
  `shift`, which also decrease the number in `$#` by one.

```
while [[ "$#" -gt 0 ]]; do
   # check input
   shift; # move to the next argument
done
```

You will notice the while loop is similar to the for loop.
It starts with `while`, has an if
  statment `[[ "$#" -gt 0 ]]` to check, and then has
  `; do`.
It then has the commands to run ane ends in `done`.

We can add in if statements to check if the first input
  is a flag, such as "-ref" or "-h" we expect to see.
If so, we can set the proper variable (refStr=$2) to
  the second value or do the users request, such as
  printing an help message and then exiting.
However, an easier way would be to use an case statement.

The case statement starts of with `case variable in`
  (kinda like an for loop).
It then has a list of cases that have a pattern that must
  be matched for an case to be true, a ")", and one or
  more commands separated by a ";".
Each case is then ended with ";;"
  (`-h) printf "%s\n" "$helpStr"; exit;;`)
The case statement ends with the `esac` command.

There is one special case, which is "\*)".
This case stands for anything that was not in another
  case.
So, it is the default case and it means we have not idea
  what the user input.

This is an example of what a basic user input loop might
  look like for our script.

```
while [[ "$#" -gt 0 ]]; do
   case $1 in
      -dir) fqPassDirStr="$2"; shift;;
      -prefix) prefixStr="$2"; shift;;
      -p) prefixStr="$2"; shift;;
      -h) printf "%s\n" "$helpStr"; exit;;
      *) printf "%s not recognized\n" "$1"; exit;;
   esac

   shift; # move to the next argument
done
```

The nice thing about this trick, is we can now have
  default values without having to do if checks.
Also, the arguments do not have to be in a certain order,
  instead the user just needs to provide a flag, such
  as `-dir` for the fastq_pass directory.
However, we should still check required input to see if
  it is valid.

There is also another method that uses something called
  getopt and an case statement. 
However, I found that often limited me to single letter
  flags and did not allow me to input single value caes.

Here is what this might look like in an full script.

```
#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr=""; # fastq_pass folder basecaller made
prefixStr="out"; # what to call everything

# not modified by user
outStr=""; # holds new prefix name

# really basic help message
helpStr="fqPassFreezeTB.sh /path/to/fastq_pass prefix";
     
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
#   - run freezeTB on an sequencing run
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"fqPassDirStr/barcode"*; do
   if [[ ! -d "$strBarDir" ]]; then
      fprintf "No barcode directories in %s\n" \
         "$strBarDir"; # let user know the problem

      break; # stop loop
   fi # check if on null case

   # grab the prefix to use for this barcode
   outStr="$prefixStr-$(basename "$strBarDir")";

   if [[ ! -d "$outStr" ]]; then
      mkdir "$outStr";
   fi # make an new directory if it does not exist

   outStr="$outStr/$outStr"; # finish the prefix name

   runFreezeTB.sh \
       "$strBarDir" \
       "$outStr" |
     tee "$prefixStr-log.tsv";
     # tee saves the read counts in "$prefixStr-log.tsv"
     # and prints them to terminal
done
```
