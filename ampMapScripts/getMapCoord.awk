BEGIN{
   startI = 1000000000; # So gets overwitten early
} # BEGIN

{ # MAIN
   if($0 ~ /^@/) next;     # Is a header
   if(and($2, 4)) next;    # unmapped read
   if(and($2, 256)) next;  # secondary alignment
   if(and($2, 2048)) next; # supplemental alignment

   if($4 < startI) startI = $4;

   # I need to remove softmasking (S), insertions (I), and
   # any hardmasking (H) minimap2 applied from the cigar
   gsub(/[0-9]*[SIH]]/, "", $6);

   lenCigI = split($6, cigAryI, /[A-Z=]/);
   readAlnLenI = 0;

   for(iCig = 0; iCig <= lenCigI; ++iCig)
      readAlnLenI += cigAryI[iCig];

   if(readAlnLenI > endI) endI = readAlnLenI;
} # MAIN

END{print startI, endI;}
