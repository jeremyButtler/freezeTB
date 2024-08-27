{ # Main
   # First column is lineages
   printf "%s", $1;

   # The hit columns are every second column, with
   # there being 3 columns per primer
   for(iCol=2; iCol <= NF; iCol +=3) printf "\t%s", $iCol;

   printf "\n"; # end of row
}; #Main
