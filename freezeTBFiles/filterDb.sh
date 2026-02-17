#!/usr/bin/env sh

# Filters out duplicate AMRs out of the freezeTB AMR
#  database. May have problems with large deletions
#  - use: sh filterDb.sh amr_database.tsv

awk ' # set database for sorting
      BEGIN{OFS="\t";};
      { # MAIN
         if(NR == 1)
            $3 = "0" $3; # make header stay at top in sort
         printf "%s\t%s\t%s\t%s", $3, $6, $7, $17;
         printf "\t%s\t%s\t%s", $18, $19, $20;

         # shift drug resistances to the start
         if(NR != 1)
         { # If: not the first row
            for(siDrug = 21; $siDrug != "*"; ++siDrug)
               printf "\t%s", $siDrug;
         } # If: not the first row

         else
         { # Else: on the first row
            for(siDrug = 21; $siDrug != "endAntibiotics"; ++siDrug)
               printf "\t%s", $siDrug;
         } # Else: on the first row

         printf "\t%s", $siDrug; # print end of drug res

         siPrint = siDrug + 3;
         printf "\t%s", $siPrint; # grade
         ++siPrint;
         printf "\t%s", $siPrint; # whole gene
         ++siPrint;
         printf "\t%s", $siPrint; # unkown

         printf "\t%s\t%s\t%s\t%s", $1, $2, $4, $5;
         printf "\t%s\t%s\t%s\t%s", $8, $9, $10, $11;
         printf "\t%s\t%s\t%s\t%s", $12, $13, $14, $15;
         printf "\t%s", $16;
         ++siDrug;
         printf "\t%s", $siDrug; # siDrug + 1
         ++siDrug;
         printf "\t%s\n", $siDrug; # siDrug + 2
      } # MAIN
  ' "$1" |
  sort -V |
  awk ' # set the database back to its original format
       BEGIN{OFS="\t"};
       { # MAIN
         # shift drug resistances to the start
         if(NR != 1)
         { # If: not the first row
            for(siDrug = 8; $siDrug != "*"; ++siDrug)
               ;
         } # If: not the first row

         else
         { # Else: on the first row
            sub(/^0/, "", $1);
            for(siDrug = 8; $siDrug != "endAntibiotics"; ++siDrug)
               ;
         } # Else: on the first row

          gradeSI = siDrug + 1;
          geneSI = siDrug + 2;
          lastColSI = siDrug + 3;

          colSI = siDrug + 4;
          printf "%s", $colSI; # column 1
          ++colSI;
          printf "\t%s", $colSI; # column 2
          printf "\t%s", $1;     # orginally column 3
          ++colSI;
          printf "\t%s", $colSI; # column 4
          ++colSI;
          printf "\t%s", $colSI; # column 5
          printf "\t%s", $2;     # orginally column 6
          printf "\t%s", $3;     # orginally column 7
          ++colSI;
          printf "\t%s", $colSI; # column 8
          ++colSI;
          printf "\t%s", $colSI; # column 9
          ++colSI;
          printf "\t%s", $colSI; # column 10
          ++colSI;
          printf "\t%s", $colSI; # column 11
          ++colSI;
          printf "\t%s", $colSI; # column 12
          ++colSI;
          printf "\t%s", $colSI; # column 13
          ++colSI;
          printf "\t%s", $colSI; # column 14
          ++colSI;
          printf "\t%s", $colSI; # column 15
          ++colSI;
          printf "\t%s", $colSI; # column 16
          printf "\t%s", $4;     # orginally column 17
          printf "\t%s", $5;     # orginally column 18
          printf "\t%s", $6;     # orginally column 19
          printf "\t%s", $7;     # orginally column 20

          for(pDrugSI = 8; pDrugSI <= siDrug; ++pDrugSI)
             printf "\t%s", $pDrugSI; # print drugs
          colSI = NF - 1; 
          printf "\t%s\t%s", $colSI, $NF;
          printf "\t%s\t%s", $gradeSI, $geneSI;
          printf "\t%s\n", $lastColSI;
       } # MAIN
    ' |
  awk '
    BEGIN{
       OFS="\t";
       getline;
       oldLineStr = $0;
    }; # BEGIN
    
    { # MAIN
       if(varIdStr != $3)
          print oldLineStr;
       else if(mutStr != $6)
          print oldLineStr;
       else if(shiftBl != $7)
          print oldLineStr;
       else if(highResBl != $17)
          print oldLineStr;
       else if(lowResBl != $18)
          print oldLineStr;
       else if(addResBl != $19)
          print oldLineStr;
       else if(needGeneStr != $20)
          print oldLineStr;
       else
       { # Else: need to check drug resistances
          breakBl = 0;

          #if($3 !~ /_p\./ || $3 ~ /fs$/ || $3 ~ /LoF$/)
          if($3 !~ /_p\./)
          { # If: this is not an amino acid change
             if(refSeqStr != $8 || amrSeqStr != $9)
             { # If: different sequences, non-protein
                print oldLineStr;
                breakBl = 1;
             } # If: different sequences, non-protein
          } # If: this is not an amino acid change

          for(siDrug = 21; $siDrug != "*"; ++siDrug)
          { # Loop: check if have the same drugs
             if(drugArySI[siDrug - 20] != $siDrug)
             { # If: have different drug resistances
                print oldLineStr;
                breakBl = 1;
                break;
             } # If: have different drug resistances
          } # Loop: check if have the same drugs
    
          if(! breakBl)
          { # If: same drug resistances
             siGrade = siDrug + 3;
             siGene = siDrug + 4;
             siUnkown = siDrug + 5;

             if(gradeSI != $siGrade)
                print oldLineStr;
             else if(entireGeneBl != $siGene)
                print oldLineStr;
             else if(unkownBl != $siUnkown)
                print oldLineStr;
             else
                next; # same target mutation
          } # If: same drug resistances
       } # Else: need to check drug resistances
    
       # at this point the new drug is differnt, so I need
       #   to reget the values
       oldLineStr = $0;
       varIdStr = $3;
       mutStr = $6;
       shiftBl = $7;
       refSeqStr = $8;
       amrSeqStr = $9;
       highResBl = $17;
       lowResBl = $18;
       addResBl = $19;
       needGeneStr = $20;
    
       # get drug resistances (ends in *)
       for(siDrug = 21; $siDrug != "*"; ++siDrug)
          drugArySI[siDrug - 20] = $siDrug;

       siDrug += 3;
       gradeSI = $siDrug;
       ++siDrug;
       entireGeneBl = $siDrug;
       ++siDrug;
       unkownBl = $siDrug;
    } # MAIN
  ';
