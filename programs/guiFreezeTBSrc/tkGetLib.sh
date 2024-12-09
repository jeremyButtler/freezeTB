#!/usr/bin/env sh

# mac paths

tkLib="$(\
   ls /opt/homebrew/lib/lib*tk*.dylib 2>/dev/null |
     grep -v "stub" \
)";

tkLib="$tkLib $( \
   ls \
       /opt/homebrew/Cellar/tcl-tk@8/8.6.15/lib/lib*tk*.dylib\
       2>/dev/null |
     grep -v "stub" \
 )";

tkLib="$tkLib $( \
    ls /usr/lib/lib*tk*.dylib 2>/dev/null |
      grep -v "stub" \
)";

# linux paths

tkLib="$tkLib $( \
   ls \
       /usr/lib/libtk*.so \
       2>/dev/null |
     grep -v "stub" \
 )";

tkLib="$tkLib $( \
   ls \
       /usr/lib/x86_64-linux-gnu/libtk*.so \
       2>/dev/null |
     grep -v "stub" \
 )"; # Ubuntu

tkLib="$(tr '\n' ' ' <<< "$tkLib")";

# find most updated libtk

tkLibId="$(
   awk '
   {
      spaceSC = "";
   
      for(siCol = 1; siCol <= NF; ++siCol){
         sub(/.*\//, "", $siCol);
         sub(/lib/, "", $siCol);
         sub(/\.dylib/, "", $siCol);
   
         if($siCol ~ /[1-9][0-9]/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
         else if($siCol ~ /9/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
         else if($siCol ~ /8\.[3-9]/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
      };
   }
   ' <<< "$tkLib" |
   sort -V -r |
   sed 's/ .*//;' \
)"; # get library names

tkPath="$( \
   awk \
      -v libStr="$tkLibId" \
      '{
         for(siCol = 1; siCol <= NF; ++siCol)
         { # Loop: find path to library
            if( $siCol ~ libStr )
            { # If: found library path
               sub(/\/libtcl.*/, "", $siCol);
               #sub(/^/, "-L", $siCol);
               print $siCol;
               break;
            } # If: found library path
         }; # Loop: find path to library
      }' <<< "$tkLib" \
)";

#printf "%s\t%s\n" "$tkLibId" "$tkPath";
printf "%s" "$tkPath";
