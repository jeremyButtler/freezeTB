{ # Main
   # First column is lineages
   printf "%s", $1;

   # The percentage columns are every third column, with
   # there being 3 columns per primer
   for(iCol=3; iCol <= NF; iCol +=3) printf "\t%s", $iCol;

   printf "\n"; # end of row
}; #Main
