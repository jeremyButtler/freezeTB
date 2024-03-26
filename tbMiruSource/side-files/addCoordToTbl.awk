BEGIN{
   OFS=FS="\t";
   stderrStr = "/dev/stderr";
   iPrim = 1;
}

{ # MAIN
   if(fileStr == "") fileStr = FILENAME;

   if(fileStr == FILENAME)
   { # If: I have primer coordinates to extract
      ++iPrim;
      nameAryStr[iPrim] = $1;
      startAryI[iPrim] = $2;
      endAryI[iPrim] = $3;
   } # If: I have primer coordinates to extract

   else if(headBl == "")
   { # Else If: I have a header to print
      headBl = 1;
      iName = 0;

      for(iCol = 2; iCol <= NF; ++iCol)
      { # Loop: Copy the primer ids
         primIdAryStr[iCol] = $iCol;
         sub(/\..*/, "", primIdAryStr[iCol]);
             # Remove the coordinates
      } # Loop: Copy the primer ids

      for(iCol = 2; iCol <= NF; ++iCol)
      { # Loop: though all primer entries

         for(iName = 2; iName <= NF; ++iName)
         { # Find the matching primer position
            if(nameAryStr[iCol] == primIdAryStr[iName])
               break; # Found the matching primer
         } # Find the matching primer position

         if(nameAryStr[iCol] != primIdAryStr[iName])
         { # If: I could not find the primer id
            printf "%s not found\n",
               primIdAryStr[iName] > stderrStr;

            continue;
         } # If: I could not find the primer id

         sub("NA", startAryI[iCol], $iName);
         sub("NA", endAryI[iCol], $iName);
      } # Loop: though all primer entries

      print $0;
   } # Else If: I have a header to print

   else print $0;
} # MAIN
