#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declarations + help message
#   o sec02:
#     - check if printing help message
#   o sec03:
#     - get primer coordinates and masking coordinates
#   o sec04:
#     - extract amplicon sequences
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations + help message
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

primStr="$1";
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

helpStr="$(basename "$0") primers.tsv
     - extracts primer coordinates on reference and
       amplicon sequences for primers
     - uses /usr/local/share/freezeTBFiles/NC000962.fa for
       reference
     - does not do a good job for direct repeat region
       due to more than one primer binding site
   Input:
     - primers.tsv:
       o tsv file with primers to get ids for
       o \"id\tT_or_F\tforward_sequene\treverse_sequence\"
       o frist row is header, other rows are primers
   Output:
     - amp_coords.tsv:
       o has start and end coordinates for amplicons
     - amp_mask.tsv:
       o has masking coordinates for primers
     - amp_seq.fa
       o has amplicon sequences
"; # help message

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - check if printing help message
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

if [[ "$primStr" == "" || "$primStr" == "-h" ]]; then
   printf "%s\n", "$helpStr";
   exit;
fi # if user wanted help message

if [[ "$primStr" == "--h" || "$primStr" == "help" ]]; then
   printf "%s\n", "$helpStr";
   exit;
fi # if user wanted help message

if [[ "$primStr" == "-help" ]]; then
   printf "%s\n", "$helpStr";
   exit;
fi # if user wanted help message

if [[ "$primStr" == "--help" ]]; then
   printf "%s\n", "$helpStr";
   exit;
fi # if user wanted help message

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - get primer coordinates and masking coordinates
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

primFind \
    -fa "$refStr" \
    -prim-tsv "$primStr" |
  awk \
    '
       BEGIN{
          OFS="\t"; # tab formate
          maskStr = "amp_mask.tsv"; # mask file name

          # move past primer file hader and print headers
          getline;
          print "qry_id", "ref_id", "dir", "start", "end";

          printf "ref_id\tqry_id\tpaired" > maskStr;
          printf "\tfor_start\tfor_end" >> maskStr;
          printf"\trev_start\trevEnd\n" >> maskStr;
       } # BEGIN
  
       { # MAIN
          if($5 <= $8)
          { # If: forward starting coordinates
             forEndUI = $6;
             startUI = $5;
             endUI = $9;
             revStartUI = $8;
             dirStr = "+";
          } # If: forward starting coordinates

          else
          { # If: reverse starting coordinates
             forEndUI = $8;
             startUI = $9;
             endUI = $5;
             revStartUI = $6;
             dirStr = "+";
             # when extract from ref becomes forward
          } # If: reverse starting coordinates

          # print cooridiantes
          print $2, $1, dirStr, startUI, endUI;

          printf "%s\t%s\tFalse\t%i\t%i\t%i\t%i\n",
             $1,
             $2,
             startUI,
             forEndUI,
             revStartUI,
             endUI >> "amp_mask.tsv";
       }; # MAIN
    ' \
  > amp_coords.tsv;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - extract amplicon sequences
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

# extract amplicons using primer coordinates
awk '
       BEGIN{
          getline;
          coordsStr = FILENAME;
       } # BEGIN

       { # MAIN
          # get all coordinates from first file
          if(coordsStr == FILENAME)
          { # If: on prime coordiantes file still
             ++numPrimUI;
             idAryStr[numPrimUI] = $1;

             if($4 <= $5)
             { # If: have forward direction
                startAryUI[numPrimUI] = $4;
                endAryUI[numPrimUI] = $5;
             } # If: have forward direction

             else
             { # If: have reverse direction
                endAryUI[numPrimUI] = $4;
                startAryUI[numPrimUI] = $5;
             } # If: have reverse direction
          } # If: on prime coordiantes file still

          # on second file, get sequence
          else if($1 ~ /^>/)
             next;

          else
             seqStr = seqStr $0;
       }; # MAIN

       END{
          # extract and print amplicon seqeuences
          for(uiPrim = 1; uiPrim <= numPrimUI; ++uiPrim)
          { # Loop: get amplicon sequences
             endUI = endAryUI[uiPrim];
             endUI -= startAryUI[uiPrim];

             printf ">%s %i %i\n%s\n",
                idAryStr[uiPrim],
                startAryUI[uiPrim],
                endAryUI[uiPrim],
                substr(seqStr, startAryUI[uiPrim], endUI)
          }; # Loop: get amplicon sequences
       }; # END
   ' \
    amp_coords.tsv \
    "$refStr" \
  > amp_seq.fa;
