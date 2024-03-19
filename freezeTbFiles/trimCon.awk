#!/usr/bin/awk

# This just extracts a target region of a reference
# sequence

BEGIN{
   split(mapCoordStr, coordAryI, /[ \t]/);
   getline;
   headStr = $0;
} # BEGIN

{seqStr = seqStr $0;} # MAIN

END{
   seqStr=substr(seqStr, coordAryI[1], coordAryI[2]);
   printf "%s start=%i", headStr, coordAryI[1];
   printf " end=%i\n%s\n", coordAryI[2], seqStr;
} # END
 
