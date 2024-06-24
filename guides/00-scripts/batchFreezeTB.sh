#!/usr/bin/env bash

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declerations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fqPassDirStr=""; # path to fastq_pass
prefixStr="Skywalker";    # provided prefix

outStr="";
pathStr="$(dirname "$0")";

helpStr="$(basename "$0") -dir /path/to/fastq_pass [...]
   - Runs freezeTB on all barcodes from a single
     sequencing run
Input:
   -dir /path/to/fastq_pass: [Required]
     o path to the fastq_pass directory with barcodes
   -prefix \"name\": [$prefixStr]
     o what to name the files and directories
   -h: print this help message and exit
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [[ "$#" -gt "0" ]]; do
   case "$1" in
      -dir) fqPassDirStr="$2"; shift;;
      -fastq_pass) fqPassDirStr="$2"; shift;;
      -pass) fqPassDirStr="$2"; shift;;

      -prefix) prefixStr="$2"; shift;;
      -p) prefixStr="$2"; shift;;

      -h) printf "%s\n" "$helpStr"; exit;;
      --h) printf "%s\n" "$helpStr"; exit;;
      --help) printf "%s\n" "$helpStr"; exit;;
      *) printf "%s not recongnized\n" "$1"; exit;;
   esac

   shift; # moves $2 to $1
done # Loop: get user input

if [[ ! -d "$fqPassDirStr" ]]; then
# if no directory provided
   if [[ "$fqPassDirStr" == "" ]]; then
      printf "No fastq_pass directory provided\n"
   else       
      printf "%s does not exits\n" "$fqPassDirStr";
   fi # if it was not the null string

   exit;
fi # if no directory provided

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run freezeTB
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

for strBarDir in ./"$fqPassDirStr/barcode"*; do
   if [[ ! -d "$strBarDir" ]]; then
      break;
   fi # if: no direcotories detected (null case)

   outStr="$(basename "$prefixStr")";
   outStr="$outStr-$(basename "$strBarDir")";

   if [[ ! -d "$outStr" ]]; then
      mkdir "$(dirname "$prefixStr")/$outStr";
   fi # if I do not have a directory

   outStr="$(dirname "$prefixStr")/$outStr/$outStr";

   "$pathStr/runFreezeTB.sh" \
        "$strBarDir/" \
        "$outStr" \
      >> "$prefixStr-log.txt";
done
