#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declerations
#   o sec02:
#     - get user input
#   o sec03:
#     - run freezeTB
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqDirStr=""; # directory of fastq/fastq.gz files
prefixStr="Skywalker"; # what to name output
minReadsUI=1000; # minimum reads to run freezeTB

# reference genome
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

numReadsUI="0";

helpStr="$(basename "$0") -dir /path/to/fastq_dir
   - Runs freezeTB on a directory of fastq files
Input:
   -dir /path/to/fastq_dir: [Required]
     o path to the directory of fastq files or fastq file
       to run
   -min-reads $minReadsUI: [Optional]
     o minimum number of reads to run freezeTB
   -prefix $prefixStr: [Optional]
     o prefix to name output files
   -h: show this help message
Output:
   - runs freezeTB (so all of freezeTBs output)
   - number of reads and an message
"

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get and check user input
#   o sec02 sub01:
#     - get user input
#   o sec02 sub02:
#     - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec02 Sub01:
#   - get user input
#*********************************************************

while [[ "$#" -gt "0" ]]; do
# Loop: get user input
   case "$1" in
      -dir)
         fqDirStr="$2";
         shift;;

      -min-reads)
         minReadsUI="$2";
         shift;;

      -prefix)
         prefixStr="$2";
         shift;;
      -p)
         prefixStr="$2";
         shift;;

      -h)
         printf "%s\n" "$helpStr";
         exit;;
      --h)
         printf "%s\n" "$helpStr";
         exit;;
      help)
         printf "%s\n" "$helpStr";
         exit;;
      -help)
         printf "%s\n" "$helpStr";
         exit;;
      --help)
         printf "%s\n" "$helpStr";
         exit;;

      *)
         printf "%s is not recognized\n" "$1";
         exit;;
   esac

   shift; # move to next input ($2 becomes $1)
done # Loop: get user input

#*********************************************************
# Sec02 Sub02:
#   - check user input
#*********************************************************

if [[ ! -f "$fqDirStr" && ! -d "$fqDirStr" ]]; then
# If: user input fastq directory that does not exists

   if [[ "$fqDirStr" == "" ]]; then
      printf "No fastq directory input with -dir\n"
   else
      printf "%s is not a fastq file or a diretory\n" \
          "$fqDirStr";
   fi # if no fastq file or directory input

   exit;
fi # If: user input fastq directory that does not exists

if [[ ! -f "$refStr" ]]; then
# If: The reference might be in ~/Documents
   refStr="$HOME/Documents/freezeTBFiles/NC000962.fa";

   if [[ ! -f "$refStr" ]]; then
   # If: The reference might be in this directory
      refStr="NC000962.fa";

      if [[ ! -f "$refStr" ]]; then
      # If: I could not find the reference
         printf "Reference \"NC000962.fa\" not found\n";
         exit;
      fi # If: I could not find the reference
   fi # If: The reference might be in this directory
fi # If: The reference might be in ~/Documents

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ -d "$fqDirStr" ]]; then
   minimap2 \
       -a \
       -x map-ont \
       "$refStr" \
       "$fqDirStr/"*.fastq* \
     > "$prefixStr-map.sam"; # fqDirStr is a directory
else
   minimap2 \
       -a \
       -x map-ont \
       "$refStr" \
       "$fqDirStr" \
     > "$prefixStr-map.sam"; # fqDirStr is a file
fi # check if is a directory or file

numReadsUI="$( \
   awk \
      '
         { # MAIN block (runs on file contents)
            if($2 == 0 || $2 == 16)
               ++numReadsUI; # add up primary mapped
	                     # foward and reverse reads
         } # MAIN block (runs on file contents)

         # END block (runs after file(s) are finished
	 END{print numReadsUI;};  # print the count
      ' \
      "$prefixStr-map.sam"; \
)"; # get the number of mapped reads

if [[ "$numReadsUI" -lt "$minReadsUI" ]]; then
   printf "ERROR for %s: %s has %s mapped reads\n" \
      "$prefixStr" \
      "$fqDirStr" \
      "$numReadsUI";
   exit;
fi # If: I do not have enough reads to run freezeTB

freezeTB -sam "$prefixStr-map.sam" -prefix "$prefixStr";

# print out number of reads
printf "%s\t%i\n" "$prefixStr" "$numReadsUI";
