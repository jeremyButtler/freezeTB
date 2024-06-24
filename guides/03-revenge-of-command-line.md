# Goal

Put freezeTB in an script

# Making an bash script

The first step in making an bash script is to put the
  commands you would normally run into a file.
After that we can add in user input, extra checks, or
  even processing of a programs output.
Lets start of by putting the commands from the last
  tutorial in an file named "runFreezeTB.sh".
I am using the ".sh" to mark that this is an bash script.

**Wait a few seconds for the user do this**

```
cat /path/to/fastq_pass/barcode01/*.fastq.gz > barcode01.fastq.gz;
minimap2 -a -x map-ont /usr/local/share/freezeTBFiles/NC000962.fa barcode01.fastq.gz | freezeTB -prefix barcode01;
```

# Polishing the script

At this point we can run this base script
  using `bash runFreezeTB.sh`.
However, there is no way to change the files I am
  inputting.
It also would be a bit annoying, though a good habit to
  write `bash runFreezeTB.sh` every time.
Finally, it would be nice to have the code look cleaner
  by breaking up long.

## shebang; removes the need to call bash

Lets start with removing the call to bash each time.
We can fix this by putting a shebang at the top of the
  file that tells the Unix system what program to do.
A shebang is just "#!" followed by the path to the program
  "/usr/bin/bash".
So, it might look like "#!/usr/bin/bash".
However, their is an problem, Macs do not install "bash"
  in "/usr/bin/bash".
This can be fixed by telling the system to find bash in
  the path (were to search for programs).
The path is part of an systems environment, which you
  can display with the `env` command.
We can do this with `#!/usr/bin/env bash`.
At this point you can use `./runFreezeTB` or even double
  click it to run freezeTB.
Or, well we could if we had permission to execute.
So, do `chmod a+x runFreezeTB.sh` to give this permission.

```
#!/usr/bin/env bash
cat /path/to/fastq_pass/barcode01/*.fastq.gz > barcode01.fastq.gz;
minimap2 -a -x map-ont /usr/local/share/freezeTBFiles/NC000962.fa barcode01.fastq.gz |
  freezeTB -prefix barcode01;
```

- shebang: a line at the top of an file that tells a
  UNIX OS how to run the program (so you don't have to).
  You can use "/usr/bin/env" to search for an program.
  - "#!/usr/bin/env bash" to run an bash script
  - "#!/usr/bin/env awk -f file.awk" to run an awk script
  - "#!/usr/bin/env RScript file.r" to run an R script
- env: displays the all the places the system looks for
  files or other default settings.
- "/usr/bin" is were your system often installs programs
- "/usr/local/bin" is were your system often installs
  your programs or programs you installed
- chmod: change permissions of file and directories
  - a+r everyone can read the file
  - a+w everyone can write (includes delete) to the file
  - a+x everyone can execute
  - a+rwx everyone has all three permissions

## Lets break up some lines

We have some pretty long lines in our current file.
This is ok, but not ideal, so I would like to break them
  up into shorter lines.
As it turns out bash will always look for a command on
  the next line after an pipe "|".

```
#!/usr/bin/env bash
cat /path/to/fastq_pass/barcode01/*.fastq.gz > barcode01.fastq.gz;
minimap2 -a -x map-ont /usr/local/share/freezeTBFiles/NC000962.fa barcode01.fastq.gz |
  freezeTB -prefix barcode01;
```

When breaking an line it is always a good idea to indent
  the new line so that others and future you can easily
  see these lines belong together.
Some people prefer tabs and others spaces.
Tabs let others control the indenting, but also can result
  in spacing issues if a person uses an different tab
  size.

The next way we can break up an line is to use a "\" at
  the end of a line.
This tells bash that the two lines really do belong
  together.
Just make sure their is nothing else after the "\",
  otherwise bash will not think your breaking up an
  line.

```
#!/usr/bin/env bash

cat \
    /path/to/fastq_pass/barcode01/*.fastq.gz \
  > barcode01.fastq.gz;

minimap2 \
    -a \
    -x map-ont \
    /usr/local/share/freezeTBFiles/NC000962.fa \
    barcode01.fastq.gz |
  freezeTB \
    -prefix barcode01;
```

I am often a more aggressive then most people on my line
  breaking.
So, you code will look different than mine.

## Adding notes

This script is pretty simple, so maybe not the time to
  leave comments.
However, it is a good to leave your future self or others
  notes on what you program is doing or why you made an
  decision to do something.
In coding a comment is known as something that will not
  be run and is only there for humans.
We can make a comment in bash by using the "#" symbol.
Again here, it is a overkill.

```
#!/usr/bin/env bash

# merge the fastq files in an barcode together
cat \
    /path/to/fastq_pass/barcode01/*.fastq.gz \
  > barcode01.fastq.gz;

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    /usr/local/share/freezeTBFiles/NC000962.fa \
    barcode01.fastq.gz |
  freezeTB \
    -prefix barcode01;
```

## Getting user input

Right know the script is in a state were everything is
  hard coded in.
What I mean by that is that we have no way of changing
  what is running without changing the script.
My first step will be to make it so that we can change
  these values easily by storing our settings in
  an variable.

An variable stores an number or string to access latter.
An string can contain an character, sentence, paragraph,
  the entire decline and fall of the Roman empire books,
  or anything else.
In bash, an variable is not specific to an number or
  string.

We declare variables or change in bash by doing
  `name="value"`.
However, make sure their are no spaces between the `=`
  symbol, otherwise bash will think you are asking to
  call a command.

As a rule of thumb try to make your variable names
  descriptive of what they hold.
That way future you or other coders have an easier time
  to figure out what you are doing.

**Ask viewer to make variables and wait a few seconds**

```
#!/usr/bin/env bash

# path to the directory with fastq files to check
fqDirStr="/path/to/fastq_pass/barcode01/";

# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

prefixStr="barcode01"; # prefix to name everything

# merge the fastq files in an barcode together
cat \
    /path/to/fastq_pass/barcode01/*.fastq.gz \
  > barcode01.fastq.gz;

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    /usr/local/share/freezeTBFiles/NC000962.fa \
    barcode01.fastq.gz |
  freezeTB \
    -prefix barcode01;
```

At this point we have some variables, but they are useless
  so long as we do not access them.
We can access the value in the variable using an `$`
  (`"$name"`).
It is always a good idea to keep the variable in `""` to
  avoid bash interpreting the variable in the wrong way.
We can also combine variables with variables and other
  strings in bash by just placing them together.
So, we could do `"$var"*.fastq.gz` to glob all fastq.gz
  files.
Just be careful that the "\*" stays out of the `""`,
  otherwise it will be taken as an literal "\*".
Or we could do `"$prefix-$var.fastq.gz"` or
  `"$prefix""-$var".fastq.gz` to specify one fastq.gz
  file.
 
**Ask viewer to add variables to replace file names and
  outputs**

```
#!/usr/bin/env bash

# path to the directory with fastq files to check
fqDirStr="/path/to/fastq_pass/barcode01/";
prefixStr="barcode01"; # prefix to name everything

# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";


# merge the fastq files in an barcode together
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

## Check user input

### the prefix

At this point all we need to do to change an file is just
  change two variables.
However, it would be nice if the user could change these
  variables when they called the script.
To do this, bash includes an set of special variables that
  for each user input.
They are 0, 1, 2, 3, .... 
We can access them using an "$", like any other variable.
If we did "$0" we would get the path to the script.
For any number past zero, such as "$1", we would get the
  users input.
With "$1" giving the first item the user input.
Try setting the fastq directory variable you made to the
  users first input.
Then set the prefix variable you made to the users second
  input.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";


# merge the fastq files in an barcode together
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

At this point we have user input, but we also have a
  problem.
If the user does not input an file, the script still runs.
Also, if the user not input an prefix, we get files
  named ".sam" and ".fastq.gz" (you can remove these with
  `rm ./.sam`, `rm ./.fastq.gz` and `rm ./-*`).
So, we need an way to stop the script "if" the user inputs
  nothing.
We also need an way to set the prefix to some default
  value "if" no prefix was input.
Thankfully there is an coding value called if, which
  launches an series of command if an condition is true.

- rm: remove listed files
  - rm -r: remove directories

We can check if an variable has an empty string `""` by
  comparing to an empty string.
This would look something like
  `if [[ "$name" == "" ]]; then`.
We then add the commands, such as assigning an default
  value after the then.
After we have all the commands we want we then end the
  if with an `fi`.
Just remember to indent the commands in the if statement,
  so that other people can quickly see there part of
  the if.
Try to check and set the default prefix (output name).

**wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

### the input directory

At this point we need to do one more check and that is
  to see if the user input an directory.
We can check to see if an directory exits by using
  `-d "$var"` instead of `"var" == ""` in our if
  statement.
However, that would require us to put the cat, minimpa2,
  and freezeTB call in the if statement.
Another alternative is to check if an directory does not
  exist.
We can do this by adding an `!` before the `-d`.
The `!` is a not operator, which checks to see if an
  condition is not true.
So, `if [[ ! "$var" == "" ]]; then` would check to see
  if var did not equal the empty string.
We can then report the error to the user and then exit out
  with the `exit` command, which ends the script early.
For now, set up the if statement with only the exit
  command.
We will do reporting next.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

if [[ ! -d "$fqDirStr" ]]; then
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

### reporting errors

To report input we can use commands like `echo` or
  `printf`.
The more encourage command is `printf`, since `echo` may
  behave differently on different Unix's.
For printf we supply a string of what to print out and
  a newline `\n` character.
If we do not provide the newline character, printf will
  not output a new line.
An example would be `printf "hi\n"`.
We can also include variables in printf by adding a "%s"
  in the format string (`printf "%s hi\n" "$var"`).
Add an error message when the user does not provide an
  directory.

**Wait a few seconds**

```
#!/usr/bin/env bash

# input user modifies
fqDirStr="$1"; # fastq directory with fastq files to check
prefixStr="$2"; # prefix to name everything

# input user does not modify
# reference to use with freezeTB
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

if [[ ! -d "$fqDirStr" ]]; then
   printf "could not find %s\n" "$fqDirStr";
   exit; # the user did not input an fastq directory
fi

if [[ "$prefixStr" == "" ]]; then
   prefixStr="freeze"; # setting default name
fi # ending the if statement

# merge the fastq files in an barcode together
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

### Further if stuff

One thing I will leave you with is that you can combine
  checks into an single if statement by using `||` or
  `&&`.
Using `||` means that it if statement only requires one
  case to be true.
While, `&&` requires that both cases be true.
For example, if you wanted to accept an directory or an
  file for input you could
  do `if [[ ! -d "$fqDirStr" || ! -f "$fqDirStr" ]]; then`

### The help message

One thing we should include is an help message that the
  user can request.
This will tell the user how to run the program.
One nice thing about strings, is that we can store several
  lines in an string.

```
var="line 1
   line 2
";
```

So we can write out help message and assign it to an
  single variable.
We can then check to see if the user input nothing and
  then print the help message and exit.
Try adding in an help message with its if statement.

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
cat \
    "$fqDirStr/"*".fastq.gz" \
  > "$prefixStr.fastq.gz";
  # Notice the "*" is between double quotes

# make sam file for freezeTB and then run freezeTB
minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr".fastq.gz |
  freezeTB \
    -prefix "$prefixStr";
```

# Final words

At this point you have a basic script to run freezeTB.
The input options are rather basic, but they work (I will
  show you a different way later).
But, one nice thing is you should know enough or be able
  to find out how to change this.
So, you could tell freezeTB to require an minimum
  mapping quality of 20 (`-min-mapq 20`) and it would
  always be set.

In the next episode I will talk about dealing with an
  error I did not mention here. That is, the barcode had
  to few reads.

# Some extras:

- grep: grabs lines matching an supplied pattern
  - The pattern is an regular expression
  - `env | grep "PATH"`, grabs any line from the
    environment that has "PATH" in it
  - `env | grep "^PATH"`, grabs any line from the
    environment starting with "PATH"
  - `env | grep "^[PM]A[TN]"`, grabs any line from the
    environment starting with "MAN", "PAT", "MAT", or
    "PAN"
- sed: (streamline editor) is an Unix search and replace
  program and more. You can do an search and replace by
  doing `sed 's/regular expression/replacement/' file`
  - you can use "/" only if you escape them with "\/"
- regular expressions: are patterns to match to strings.
  They can include a set of special characters, which
  can allow more looser matches.
  - regular expressions are used in several programs, such
    as sed (search and replace) and grep (search)
  - regular expressions and are also included in text
    editors and coding languages (gsub or sub in R/awk).

- "\" escapes other characters to an alternate character
  or for special characters to their normal meaning 
    - get an literal "\" by using "\\"
    - "\n" is newline         (escape to special meaning)
    - "\t" is tab             (escape to special meaning)
    - "\r" is carriage return (escape to special meaning)
    - "\^" is "^"             (escape to normal meaning)
    - "\$" is "$"             (escape to normal meaning)
    - "\&" is "&"             (escape to normal meaning)
    - "\\\*" is "\*"          (escape to normal meaning)
    - "\\." is "."            (escape to normal meaning)
    - "\?" is "?"             (escape to normal meaning)
    - "\\+" is "+"            (escape to normal meaning)
    - "\\[" is "["            (escape to normal meaning)
    - "\\]" is "]"            (escape to normal meaning)
    - "\\|" is "|"            (escape to normal meaning)
    - "\\{" is "{"            (escape to normal meaning)
    - "\\}" is "}"            (escape to normal meaning)

- special symbols for regular expressions
  - "^" is start of line
  - "$" is end of line
  - "&" is input pattern
  - "\*" is repeat that last character 0 or more times
    - not quite the wild card used by bash
    - "a\*" for any number of a's (includes none)
  - "." is any character
    - ".\*" is any character any number of times, which is
       the same as using "*" in bash (ex cat "*".fastq.gz)
    - "." for any character
  - "?" character is seen 0 or 1 times
    - "a?" for zero or one a
  - "+" character is seen at least 1 time
    - unlike "\*", which allows no match, "+" says there
      must be at least one match
    - "a+" for at least one "a"
  - "[" and "]": character can be anything in "[]"
    - so "[atgcATGC]" would look for an upper or lower
      case nucleotide
    - "[atgcATGC]\*" would look for an nucleotide
      sequence
  - "{" and "}": has the number of matches that are
    expected for the previous character. Minimum and
    maximum values are denoted by a comma ","
    - "a{2}" two a's
    - "a{1,}" at least one a
    - "a{,2}" at most 2 a's
    - "a{1,5}" 1 to 5 a's
  - "|": have two variations in an pattern
    - "atgc|ATGC" to search for an lower or uppercase
      "atgc" pattern
    - these can be grouped by "(M|P)A(T|N)" is the same
      as "[MP]A[TN]" which looks for "MAN", "PAT", "MAT",
      an "PAN".
