#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declerations
#   o sec02:
#     - get user input
#   o sec03:
#     - run tuberculosis analysis
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqDirStr=""; # directory of fastq/fastq.gz files
prefixStr="Skywalker"; # what to name output
minReadsUI=1000; # minimum reads to run freezeTB

# user should be able to modify, but I am to lazy
minMeanQUI="7";
minLenUI="200";
minMapqUI="15"; # min mapping quality for filtering

# reference genome
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";
amrStr="/usr/local/share/freezeTBFiles/who-2023.tsv";
dbStr="/usr/local/share/freezeTBFiles/spoligo-linages.csv"
spacersFaStr="/usr/local/share/freezeTBFiles/spoligotype-seq.fa"

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
#   - run tuberculosis analysis
#   o sec03 sub01:
#     - filter reads
#   o sec03 sub02:
#     - bulid consensus
#   o sec03 sub03:
#     - consensus anylsis
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - filter reads
#*********************************************************

if [[ -d "$fqDirStr" ]]; then
# If: fastq files in a directory
   cat "$fqDirStr/"*.fastq* > "$prefixStr-tmp.fastq";

   filtlong \
       --min_length "$minLenUI" \
       --min_mean_q "$minMeanQUI" \
       "$prefixStr-tmp.fastq" \
     > "$prefixStr-filt.fastq";

   rm "$prefixStr-tmp.fastq";
# If: fastq files in a directory

else
# Else: user input a fastq file
   filtlong \
       --min_length "$minLenUI" \
       --min_mean_q "$minMeanQUI" \
       "$fqDirStr" \
     > "$prefixStr-filt.fastq";
# Else: user input a fastq file
fi # check fastq input type

prefixStr="$prefixStr-filt";

minimap2 \
    -a \
    -x map-ont \
    "$refStr" \
    "$prefixStr.fastq" |
  samtools view -F 4 -q "$minMapqUI" |
  awk '{if($0 !~ /^@/) print $1;}' \
  > "$prefixStr-sam.ids";

numReadsUI="$( \
  wc -l "$prefixStr-sam.ids" |
  cut -d " " -f 1 \
)";


if [[ "$numReadsUI" -lt "$minReadsUI" ]]; then
   printf "ERROR for %s: %s has %s mapped reads\n" \
      "$prefixStr" \
      "$fqDirStr" \
      "$numReadsUI";
   exit;
fi # If: I do not have enough reads to run freezeTB

seqkit grep \
    -f "$prefixStr-sam.ids" \
    "$prefixStr.fastq" \
  > "$prefixStr-sam-seqkit.fastq";

prefixStr="$prefixStr-sam-seqkit";

#*********************************************************
# Sec03 Sub02:
#   - bulid consensus
#*********************************************************

flye --nano-raw "$prefixStr.fastq" "$prefixStr-flye";
 
#source "$HOME/medaka/venv/bin/activate";
medaka_consensus \
   -i "$prefixStr.fastq" \
   -d "$prefixStr-flye/assembly.fasta" \
   -o "$prefixStr-flye-medaka";

#deactivate;

cp \
    "$prefixStr-flye-medaka/consensus.fasta" \
    "$prefixStr-flye-medaka.fasta";

#*********************************************************
# Sec03 Sub03:
#   - consensus anylsis
#*********************************************************

tbSpoligo \
    -fast \
    -spoligo "$spacersFaStr" \
    -db "$dbStr" \
    -fa "$prefixStr-flye-medaka.fasta" \
  > "$prefixStr-flye-medaka-spoligo.tsv";

minimap2 \
    -a \
    "$refStr" \
    "$prefixStr-flye-medaka.fasta" |
  tbAmr \
    -amr-tbl "$amrStr" \
    -sam-con - \
  > "$prefixStr-flye-medaka-AMRs.tsv";
