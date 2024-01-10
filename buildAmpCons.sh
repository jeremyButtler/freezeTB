#!/usr/bin/bash

##########################################################
# Name: getAmpCons
#   - Get the consensus for each set of gene mappings in
#     the fastq file
# Input:
#   -ref: [Required]
#     o Reference genome (.fasta) to map against
#   -genes: [Required]
#     o Genes from reference to use in getting stats and
#       extracting reads from the fastq file
#   -fastq: [Required]
#     o Fastq file to extract reads from
#   -prefix: [out]
#     o prefix to name the output consensus
#   -min-reads: [20]
#     o Min number of reads to bulid a consensus for
#   -threads/-t: [3]
#     o Number of threads to use
# Output:
#   - consensus to prefix-con.fasta
#   - prefix-tblStr.tsv with mapping counts
#   - prefix-supStr.tsv with suplemental alignments
##########################################################

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TOC: Table Of contents
#   o sec-01:
#     - Variable declerations
#   o sec-02:
#     - Get user input
#   o sec-03:
#     - Get stats and extract reads
#   o sec-04:
#     - Build the consensus for each set of reads
#   o sec-04:
#     - Put consnesuses into single file and exit
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-01:
#   - Variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

refStr="";
genesStr="";
fastqStr="";
prefixStr="out";
minReadsI=20;
threadsI=3;

scriptDirStr="$(dirname "$0")/ampMapScripts";

helpStr="
   $(basename "$0") -ref ref.fa -genes ref-genes.fa -fastq file.fq
   Use:
     - Get the consensus for each set of gene mappings in
       the fastq file
   Input:
     -ref: [Required]
       o Reference genome (.fasta) to map against
     -genes: [Required]
       o Genes from reference to use in getting stats and
         extracting reads from the fastq file
     -fastq: [Required]
       o Fastq file to extract reads from
     -prefix: [out]
       o prefix to name the output consensus
     -min-reads: [20]
       o Min number of reads to bulid a consensus for
     -threads/-t: [3]
       o Number of threads to use
   Output:
     - consensus to prefix-con.fasta
     - prefix-tblStr.tsv with mapping counts
     - prefix-supStr.tsv with suplemental alignments
"

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-02:
#   - Get user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Setting up some conveint defaults for the reference.
# This script was build for a Tubercilousis project
refStr="$scriptDirStr/TB-NC000962.fa";
genesStr="$scriptDirStr/TB-NC000962-genes.fa";

while [[ $# -gt 0 ]]; do
# Loop: get all user input
   case $1 in
      -ref) refStr="$2"; shift;;
      -genes) genesStr="$2"; shift;;
      -fastq) fastqStr="$2"; shift;;
      -prefix) prefixStr="$2"; shift;;
      -min-reads) minReadsI="$2"; shift;;
      -threads) threadsI="$2"; shift;;
      -t) threadsI="$2"; shift;;
      -h) printf "%s\n" "$helpStr"; exit;;
      --h) printf "%s\n" "$helpStr"; exit;;
      -help) printf "%s\n" "$helpStr"; exit;;
      --help) printf "%s\n" "$helpStr"; exit;;
      help) printf "%s\n" "$helpStr"; exit;;
      *) printf "%s\n%s is not valid\n" "$helpStr" "$1";
         exit;;
   esac

   shift;
done # Loop: get all user input

if [[ ! -f "$genesStr" ]]; then
# If: the genes reference file is not valid
   printf "could not open -genes %s\n" "$genesStr";
   exit;
fi # If: the genes reference file is not valid

if [[ ! -f "$refStr" ]]; then
# If: the reference file is not valid
   printf "could not open -ref %s\n" "$refStr";
   exit;
fi # If: the reference file is not valid

if [[ ! -f "$fastqStr" ]]; then
# If: the fastq file is not valid
   printf "could not open -fastq %s\n" "$fastqStr";
   exit;
fi # If: the fastq file is not valid

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-03:
#   - Get stats and extract reads
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# Get mappings
printf "Mapping reads to genes\n";
printf "Map reads to genes\n" >>"$prefixStr-log-ivar.txt";

minimap2\
   --eqx\
   -a\
   -x map-ont\
   -t "$threadsI"\
   "$genesStr"\
   "$fastqStr"\
 > tmp.sam\
 2>>"$prefixStr-log-ivar.txt" || exit;

# Find the counts in the file
"$scriptDirStr/ampMap"\
   -sam "tmp.sam"\
   -map-out "$prefixStr-mapTbl.tsv"\
   -sup-out "$prefixStr-supTbl.tsv"\
   -tsv || exit;

Rscript "$scriptDirStr/ampFig.r"\
  "$prefixStr-mapTbl.tsv"\
  "$prefixStr-mapGraph"\
  2> /dev/null\
  1> /dev/null;

# Extract the reads to build consensus for
printf "Extracting reads that mapped to genes\n";
awk\
   -f "$scriptDirStr/extractPrimRead.awk"\
   -v tblStr="$prefixStr-mapTbl.tsv"\
   -v minReadsI="$minReadsI"\
   -v prefStr="$prefixStr"\
   < tmp.sam || exit;

rm tmp.sam;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-04:
#   - Bulid the consensus for each set of reads
#   o sec-04 sub-01:
#     - subsample reads
#   o sec-04 sub-02:
#     - Check if have enough reads for a consensus
#   o sec-04 sub-03:
#     - Get and report coordiantes reads mapped to on
#       the reference
#   o sec-04 sub-04:
#     - Extract mapped region from reference and 
#       build the consensus with ivar
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec-04 Sub-01:
#   - subsample reads
#*********************************************************

printf "\n\n";

mkdir "$prefixStr-fastq";
mkdir "$prefixStr-subsample";

for strFq in ./"$prefixStr-"*.fastq; do
# Loop: Build a consensus for each fastq file
   printf "%s\n" "$strFq" >> "$prefixStr-log.txt";
   printf "%s\n" "$strFq";

   subFqStr="$(\
      printf "%s" "$strFq" | sed 's/\.fa*s*t*q/-sub.fq/;'\
   )"; # Get the subsampe file name

   minimap2\
       --eqx\
       -a\
       -x map-ont\
       -t "$threadsI"\
       "$refStr"\
       "$strFq"\
       2>/dev/null\
     > tmp.sam;

   "$scriptDirStr/subsampleIds"\
        -num-reads 300\
        -sam tmp.sam |
     "$scriptDirStr/fqGetIds"\
         -stdin-filt\
         -fastq "$strFq"\
         -out "$subFqStr";

   mv "$strFq" "$prefixStr-fastq";

   #******************************************************
   # Sec-04 Sub-02:
   #   - Check if have enough reads for a consensus
   #******************************************************

   numReadsI="$(\
      awk\
        '{if($0 !~ /^[ \t\r]*$/){print $0;};}'\
        "$subFqStr" |
      wc -l |
      awk '{printf $1 / 4}'\
   )";

   if [[ "$numReadsI" -lt "$minReadsI" ]]; then
   # If: I could not extract enough reads
      { # Let the user know the problem
         printf "%s did not have enough reads" "$strFq";
         printf " to build a consnesus with\n\n"
      } >&2; # Let the user know the problem

      { # Add error to log
         printf "%s did not have enough reads" "$strFq";
         printf " to build a consnesus with\n\n"
      } >> "$prefixStr-log.txt"; # Add error to log

      rm tmp.sam;
      mv "$subFqStr" "$prefixStr-subsample";k
      continue;
   fi # If: I could not extract enough reads

   #******************************************************
   # Sec-04 Sub-03:
   #   - Get and report coordiantes reads mapped to on
   #     the reference
   #******************************************************

   startI="$(\
      printf "%s" "$mapCoordStr" | awk '{print $1;}'\
   )";

   lenI="$(\
      printf "%s" "$mapCoordStr" | awk '{print $2;}'\
   )";

   { # Report number of reads extracted
      printf "Extracted %s reads from" "$numReadsI";
      printf " %s" "$strFq";

      printf " (Start=%s" "$startI";
      printf " end=%s)\n\n" "$((startI + lenI))";
   } # Report number of reads extracted

   { # Report number of reads extracted
      printf "Extracted %s reads from" "$numReadsI";
      printf " %s" "$strFq";

      printf " (Start=%s" "$startI";
      printf " ( end=%s)\n\n" "$((startI + lenI))";
   } >> "$prefixStr-log.txt"

   mapCoordStr="$(\
      awk -f "$scriptDirStr/getMapCoord.awk" tmp.sam\
   )"; # get the mapping coordinates

   rm tmp.sam;

   #******************************************************
   # Sec-04 Sub-04:
   #   - Extract mapped region from reference and 
   #     build the consensus with ivar
   #******************************************************

   awk\
      -f "$scriptDirStr/trimCon.awk"\
      -v mapCoordStr="$mapCoordStr"\
      "$refStr"\
     > tmp-ref.fa;

   printf\
        "\n\n%s\n\n"\
       "$subFqStr"\
     >> "$prefixStr-log-ivar.txt";

   bash "$scriptDirStr/ivarConScript.sh"\
       -ref "tmp-ref.fa"\
       -fastq "$subFqStr"\
       -prefix "$(\
          printf "%s" "$subFqStr" | sed 's/.fq//;'\
       )"\
       -threads "$threadsI"\
       2>> "$prefixStr-log-ivar.txt"\
       1>> "$prefixStr-log-ivar.txt";

   mv "$subFqStr" "$prefixStr-subsample";
   rm tmp-ref.fa;
done # Loop: Build a consensus for each fastq file

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec-05:
#   - Put consensuses into single file and exit
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

mkdir "$prefixStr-consensuses";
mv "$prefixStr-"*con.fa "$prefixStr-consensuses";

cat "$prefixStr-consensuses/"* > "$prefixStr-cons.fa";

exit;