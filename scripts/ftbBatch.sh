#!/usr/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ftbBatch: Start Of File
#   - run freezeTB with default settings in batch mode
# Input:
#   - takes in a multipe of two items. The frist item is
#     the prefix to call the output, the second is the
#     dictory with barcode directories (fastq_pass)
# Output:
#   - folder for each prefix, with the minimap2 and
#     freezeTB output (in directories named prefix-barxx)
#   - amr-resultsl.tsv: has all AMRs detected for each
#     barcode
#   - spol-resultsl.tsv: has spoloigotypes detected for
#     each barcode
# SOF:
#   o sec01:
#     - variable declarations
#   o sec02:
#     - print headers for results files
#   o sec03:
#     - run minimap2/freezeTB on directories
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

prefixStr="";
dirStr="";
outDirStr="";
outPrefixStr="";
barStr="";
rBl=1;
mapReadBl=0;

helpStr="$(basename "$0") prefix directory...
   - runs freezeTB on all barcodes in a directory
Input:
   -mapRead to use mapRead (FTB internal read mapper)
     instead of minimap2
     o this must be the first item input
   - order is always prefix, followed by the directory
     running, you can input multiple directories and
     prefix's
     o ex: prefix1 directory1 prefix2 directory2...
   - prefix: what to call output
   - directory: directory with barcode folders to run
     freezeTB on
     o ex fastq_pass directory from guppy
Output:
   - makes a folder with freezeTB's output for each
     barcode
   - extracts AMRs and lineages for each barcode
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - print headers for results files
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if ! command -v Rscript ;
then # If: no Rscript
   rBl=0;
   printf "no Rscript not found\n";
else
   rBl=1;
   printf "Rscript found\n";
fi # check if have Rscript

if [ $# -le 1 ]
then
   printf "%s\n" "$helpStr";
   exit;
fi # If: nothing input

if [ $1 = "-mapRead" ]; then mapReadBl=1; shift; fi;

printf \
    "prefix\tbarcode\tdrug...\n" \
  > "amr-results.tsv";

printf \
    "prefix\tbarcode\tstrain\tlineage\toctal\tcountry\n" \
   > "spol-results.tsv";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - run minimap2/freezeTB on directories
#   o sec03 sub01:
#     - print help message (if requested) + start loop
#   o sec03 sub02:
#     - create barcode set output directory + start loop
#   o sec03 sub03:
#     - make barcode directory and run programs
#   o sec03 sub04:
#     - get and print detected AMRs
#   o sec03 sub05:
#     - get and print detected spoligotypes
#   o sec03 sub06:
#     - build graphs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - print help messages (if requested) + start loop
#*********************************************************

while [ $# -gt 1 ] ;
do # Loop: get directories to process

   if [ "$1" = "-h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "--h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "-help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "--help" ]; then
      printf "%s\n" "$helpStr"; exit;
   fi # If: help message wanted

   if [ "$2" = "-h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$2" = "--h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$2" = "help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$2" = "-help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$2" = "--help" ]; then
      printf "%s\n" "$helpStr"; exit;
   fi # If: help message wanted

   #******************************************************
   # Sec03 Sub02:
   #   - create barcode set output directory + start loop
   #******************************************************

   prefixStr="$1";
   dirStr="$2";
   shift;  # move past prefix
   shift;  # move past directory

   if [ ! -d "$prefixStr" ];
   then
      mkdir "$prefixStr";
   fi # If need to make directory for barcodes

   for strBar in "$dirStr/barcode"* ;
   do # Loop: run each

      #***************************************************
      # Sec03 Sub03:
      #   - make barcode directory and run programs
      #***************************************************

      if [ ! -d "$strBar" ] ;
      then
         continue;
      fi # null case (nothing input)

      barStr="$( \
         basename "$strBar" | sed 's/barcode/bar/;' \
      )";

      outDirStr="$prefixStr/$prefixStr-$barStr";
      outPrefixStr="$outDirStr/$prefixStr-$barStr";
 
      if [ ! -d "$outDirStr" ];
      then
         mkdir "$outDirStr";
      fi # If need to make directory for this barcode

      if [ $mapReadBl -eq 0 ];
      then # if: using minimap2
         if [ ! -f "$outPrefixStr-map.sam" ];
         then # If: need to remap reads
            minimap2 \
               -a \
               --eqx \
               /usr/local/share/freezeTBFiles/NC000962.fa\
               "$strBar/"*.fastq* \
             > "$outPrefixStr-map.sam";
         fi # If: need to remap reads

         freezeTB \
            -sam "$outPrefixStr-map.sam" \
            -prefix "$outPrefixStr-map-FTB";
      else
         freezeTB \
            -prefix "$outPrefixStr-map-FTB" \
            "$strBar/"*.fastq*;
      fi; # check if using mapRead or minimap2

      drugStr="$(
         awk \
            '
              BEGIN{getline; OFS="\t";};
              { # MAIN
                 amrStr = amrStr "\t" $2;
                 if($3 != "NA")
                    amrStr = amrStr "\t" $3;
              }; # MAIN
              END{if(amrStr != "") print amrStr;};
            ' < "$outPrefixStr-map-FTB-read-amrs.tsv" \
      )"; # get detected drugs

      #***************************************************
      # Sec03 Sub04:
      #   - get and print detected AMRs
      #***************************************************

      if [ "$drugStr" != "" ];
      then # If: have AMRs
         printf \
             "%s\t%s\t%s\n" \
             "$prefixStr" \
             "$barStr" \
             "$drugStr" \
           >> "amr-results.tsv";
      else
         printf \
             "%s\t%s\tNA\n" \
             "$prefixStr" \
             "$barStr" \
           >> "amr-results.tsv";
      fi # print AMR results

      #***************************************************
      # Sec03 Sub05:
      #   - get and print detected spoligotypes
      #***************************************************

      spolStr="$(
         awk \
            '
              BEGIN{getline; OFS="\t";};
              { # MAIN
                 print $2, $5, $4, $7;
              }; # MAIN
            ' < "$outPrefixStr-map-FTB-read-spoligo.tsv" \
      )"; # get detected spoligotype
              
      printf \
          "%s\t%s\t%s\n" \
          "$prefixStr" \
          "$barStr" \
          "$spolStr" \
        >> "spol-results.tsv";

      #***************************************************
      # Sec03 Sub06:
      #   - build graphs
      #***************************************************

      if [ "$rBl" -ne 0 ] ;
      then # If: can make graphs
         graphAmpDepth.r \
           -who /usr/local/share/freezeTBFiles/amrDb.tsv \
           -ext "png" \
           -prefix "$outPrefixStr-map-FTB" \
           -stats "$outPrefixStr-map-FTB-depths.tsv";
      fi # If: can make graphs
   done # Loop: run each
done # Loop: get directories to process
