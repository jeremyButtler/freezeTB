BEGIN{
   if(minReadsI == "") minReadsI = 20;
   if(prefStr == "") prefStr = "map";

   if(tblStr == "")
   { # If: the mapping table was not input
      printf "Input a map table with -v tblStr=tbl.tsv\n"
      exit;
   } # If: the mapping table was not input

   while((getline < tblStr) != 0)
   { # Loop: Read in the genes to extract
      if($3 >= minReadsI) genesAryStr[$1] = 1;
      else genesAryStr[$1] = 0;
   } # Loop: Read in the genes to extract

   getline;
} # BEGIN

{ # MAIN
   if($3 == "*" || and($2, 256) || and($2, 2048)) next;

   if(genesAryStr[$3] == 1)
   { # If: I am keeping the read
      outStr = prefStr "-" $3 ".fastq";
      printf "@%s\n%s\n+\n%s\n", $1, $10, $11 >> outStr;
   } # If: I am keeping the read
} # MAIN
