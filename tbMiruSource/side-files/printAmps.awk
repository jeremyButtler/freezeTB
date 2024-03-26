BEGIN{
   OFS=FS="\t";
   stderrStr = "/dev/stderr";
   iPrim = 0;
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
      next;
   } # Else If: I have a header to print

   else seqStr = seqStr $0;
} # MAIN

END{
   for(iAmp = 1; iAmp <= iPrim; ++iAmp)
   { # Loop: Get the amplicons
      lenAmpI = endAryI[iAmp] - startAryI[iAmp];
      ampStr = substr(seqStr, startAryI[iAmp], lenAmpI);
      printf ">%s\n%s\n", nameAryStr[iAmp], ampStr;
   } # Loop: Get the amplicons
} # END
