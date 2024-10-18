#!/usr/bin/env bash

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOF: Start Of File
#   o sec01:
#     - variable declarations + help message
#   o sec02:
#     - check if printing help message
#   o sec03:
#     - extract amplicon sequences
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations + help message
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

coordTsvStr="$1";
refStr="/usr/local/share/freezeTBFiles/NC000962.fa";

helpStr="$(basename "$0") amplicon_coordinates.tsv
     - extracts amplicon sequences from
       /usr/local/share/freezeTBFiles/NC000962.fa
   Input:
     - amplicon_coordinates.tsv:
       o tsv file with coordinates of amplicons to extract
       o \"quey_id\treference_id\tT_or_F\t+\tforward_sequene\treverse_sequence\"
       o frist row is header, other rows are primers
   Output:
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
#   - extract amplicon sequences
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

awk '
       BEGIN{
          getline;
          coordsStr = FILENAME;
       } # BEGIN

       { # MAIN
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

          else if($1 ~ /^>/)
             next;

          else
             seqStr = seqStr $0;
       }; # MAIN

       END{
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
    "$1" \
    "$refStr" \
  > amp_seq.fa;
