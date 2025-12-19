#!/usr/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# addAmrsToCoords.sh SOF: Start Of File
#   - adds the AMRs to the amplicon coordinates files
#   o sec01:
#     - variable declarations
#   o sec02:
#     - get and check user input
#   o sec03:
#     - add AMRs to the coordinates
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fileStr="";
amrDbStr="";

helpStr="$(basename "$0") -coords coordinates.tsv -amrs amrs.tsv
   - gets the AMRs for each amplicons coordinates in the
     coordinates file and puts it in a database that
     ampDepth can use for gene coverage
Input:
   -coords: coordinates to get AMRs for.
     o format: row one is header, other rows are amplicons
       * column one: amplicon name
       * column two: should be reference name (ignored)
       * column three: should be direction (ignored)
       * column four: amplicon start
         - for this column the header should be 'start'
       * column five: amplicon end
   -amrs: database with AMRs in freezeTB format
Output:
   - -coords input with the AMRs in full names. You will
     have to shorthand these later.
     o the last column for AMRs is marked with a '*'
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get and check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [ $# -gt 0 ];
do   # Loop: get use input
   if [ "$1" = "-coords" ]; then
      shift;
      fileStr="$1";
   elif [ "$1" = "-amrs" ]; then
      shift;
      amrDbStr="$1";
   elif [ "$1" = "-h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "--h" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "-help" ]; then
      printf "%s\n" "$helpStr"; exit;
   elif [ "$1" = "--help" ]; then
      printf "%s\n" "$helpStr"; exit;
   else
      printf "%s is not recognized\n" "$1"; exit;
   fi;

   shift;
done # Loop: get use input

if [ ! -f "$fileStr" ]; then
   printf "could not open -coords %s\n" "$fileStr";
   exit;
fi;

if [ ! -f "$amrDbStr" ]; then
   printf "could not open -amrs %s\n" "$amrDbStr";
   exit;
fi;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - add AMRs to the coordinates
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while read -r lineStr;
do   # Loop: read in each amplicon coordinate
   startSI="$( \
      printf "%s" "$lineStr" | awk '{print $4;};' \
   )";
   endSI="$(printf "%s" "$lineStr" | awk '{print $5;};')";

   if [ "$startSI" = "start" ]; then
      printf "%s\tdrugs\n" "$lineStr";
      continue;
   fi;

   drugsStr="$(
      awk \
          -v startSI="$startSI" \
          -v endSI="$endSI" \
          '
             BEGIN{
                OFS="\t";
                getline;
                print $20, $21, $22, $23, $24, $25, $26,
                      $27, $28, $29, $30, $31, $32, $33,
                      $34, $35, $36, $1;
             }; # BEGIN
      
             { # MAIN
                # check if have gene, if so print out the
                #   drugs that are resistant
                if($3 >= startSI && $3 <= endSI)
                  print $20, $21, $22, $23, $24, $25, $26,
                        $27, $28, $29, $30, $31, $32, $33,
                        $34, $35, $36, $1;
             }; # MAIN
          ' "$amrDbStr" |
        awk '
               BEGIN{
                  getline;
                  for(siDrug = 1; siDrug < NF; ++siDrug)
                  { # Loop: get drug names + initialize
                     drugAryStr[siDrug] = $siDrug;
                     cntArySI[siDrug] = 0;
                  }; # Loop: get drug names + initialize
                  endSI = NF - 1;
               }; # BEGIN
      
               { # MAIN
                  for(siDrug = 1; siDrug < NF; ++siDrug)
                  { # Loop: find drug resistance
                     if($siDrug > 0)
                        ++cntArySI[siDrug];
                  }; # Loop: find drug resistance
                  geneStr = $NF;
                }; # MAIN
      
                END{
                  for(siDrug=1; siDrug <= endSI; ++siDrug)
                  { # Loop: print the resitance drugs
                     if(cntArySI[siDrug] > 0)
                        printf "\t%s", drugAryStr[siDrug];
                  }; # Loop: print the resitance drugs
                }; # END
            '; \
   )"; # get resistant drugs for each gene

   printf "%s%s\t*\n" "$lineStr" "$drugsStr";
done < "$fileStr" # Loop: read in each amplicon coordinate
