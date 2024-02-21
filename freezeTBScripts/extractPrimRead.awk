#!/usr/bin/awk

BEGIN{
   if(minReadsI == "") minReadsI = 20;
   if(prefStr == "") prefStr = "map";

   if(tblStr == "")
   { # If: the mapping table was not input
      printf "Input a map table with -v tblStr=tbl.tsv\n"
      exit;
   } # If: the mapping table was not input

   ampOnI = -1;

   if((getline < tblStr) < 0) 
   { # If: The table has no data
      printf "Input map table"
      printf " (-v tblStr=%s) has no data\n", tblStr;
      exit;
   } # If: The table has no data

   while((getline < tblStr) != 0)
   { # Loop: Read in the genes to extract
      if($3 == "NA") continue; # Read is ignored

      if(ampOnI == $2)
      { # If: this amplicon has multiple genes
         geneIdAryStr[ampOnI + 1] = geneIdAryStr[ampOnI + 1] "-" $10;
         continue;
      } # If: this amplicon has multiple genes

      ++ampOnI;
      geneIdAryStr[ampOnI + 1] = $10;
      startAryI[ampOnI + 1] = $5;
      endAryI[ampOnI + 1] = $6;
   } # Loop: Read in the genes to extract

   ++ampOnI;
   getline;
} # BEGIN

{ # MAIN
   if($3 == "*" || and($2, 256) || and($2, 2048)) next;

   for(iAmp = 1; iAmp <= ampOnI; ++iAmp)
   { # Loop: Check if keeping a read
      if(startAryI[iAmp] <= $4 && endAryI[iAmp] >= $4)
      { # If: I am keeping the read
         outStr = prefStr "-" geneIdAryStr[iAmp] ".fastq";
         printf "@%s\n%s\n+\n%s\n", $1, $10, $11 >>outStr;
      } # If: I am keeping the read
   } # Loop: Check if keeping a read
} # MAIN
