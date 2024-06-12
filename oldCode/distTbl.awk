BEGIN{
   OFS="\t";
   siSeq = 1;
};

{ # MAIN
   sub(">Spacer", "", $0);
   numArySI[siSeq] = $0;
   getline;
   seqAryStr[siSeq] = $0;
   ++siSeq
}; # MAIN

END{
   --siSeq;

   for(siEntry = 1; siEntry <= siSeq; ++siEntry)
   { # Loop: find edit distance table
      lenQrySI = split(seqAryStr[siSeq], qrySeqStr, "");

       distTblSI[siEntry][siEntry] = 0;

      for(siRef = siEntry + 1; siRef <= siSeq; ++siRef)
      { # Loop: split up the reference sequence
         lenRefSI = split(seqAryStr[siRef],refSeqStr,"");
         editDistSI = 0;

         for(siNt = 1; siNt < lenRefSI; ++siNt)
         { # Loop: find the edit distance
            if(qrySeqStr[siNt] != refSeqStr[siNt])
               ++editDistSI;
         } # Loop: find the edit distance

         distTblSI[siEntry][siRef] = editDistSI;
         distTblSI[siRef][siEntry] = editDistSI;
      } # Loop: split up the reference sequence
   } # Loop: find edit distance table

   printf "query";

   for(siEntry = 1; siEntry <= siSeq; ++siEntry)
      printf "\t%i", siEntry;

   printf "\n";

   for(siEntry = 1; siEntry <= siSeq; ++siEntry)
   { # Loop: print the edit distance table
      printf "%i", siEntry;

      for(siRef = 1; siRef <= siSeq; ++siRef)
         printf "\t%i", distTblSI[siEntry][siRef];

      printf "\n";
   } # Loop: print the edit distance table
} # END
