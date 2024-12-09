#!/usr/bin/env sh

tclLib="$(\
   ls /opt/homebrew/lib/lib*tcl*.dylib 2>/dev/null |
     grep -v "stub" \
)";

tclLib="$tclLib $( \
   ls \
       /opt/homebrew/Cellar/tcl-tcl@8/8.6.15/lib/lib*tcl*.dylib\
       2>/dev/null |
     grep -v "stub" \
 )";

tclLib="$tclLib $( \
    ls /usr/lib/lib*tcl*.dylib 2>/dev/null |
      grep -v "stub" \
)";

# linux paths

tclLib="$tclLib $( \
   ls \
       /usr/lib/libtcl*.so \
       2>/dev/null |
     grep -v "stub" \
 )";

tclLib="$tclLib $( \
   ls \
       /usr/lib/x86_64-linux-gnu/libtcl*.so \
       2>/dev/null |
     grep -v "stub" \
 )"; # Ubuntu

tkLib="$(tr '\n' ' ' <<< "$tkLib")";

# filter out tk libraries + find most updated lib

tcLibId="$(
   awk '
   {
      spaceSC = "";
   
      for(siCol = 1; siCol <= NF; ++siCol){
         sub(/.*\//, "", $siCol);
         sub(/lib/, "", $siCol);
         sub(/\.dylib/, "", $siCol);
   
         if($siCol ~ /tcl.*tk/)
           continue;
         else if($siCol ~ /[1-9][0-9]/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
         else if($siCol ~ /9/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
         else if($siCol ~ /8\.[3-9]/)
           {printf "%s%s", spaceSC, $siCol; spaceSC = " ";}
      };
   }
   ' <<< "$tclLib" |
   sort -V -r |
   sed 's/ .*//;' \
)"; # get library names


tclPath="$( \
   awk \
      -v libStr="$tclLibId" \
      '{
         for(siCol = 1; siCol <= NF; ++siCol)
         { # Loop: find path to library
            if($siCol ~ /tcl[0-9.]*tk[0-9.]*.dylib/)
               continue;

            if( $siCol ~ libStr )
            { # If: found library path
               #sub(/\/libtcl.*/, "", $siCol);
               #sub(/^/, "-L", $siCol);
               print $siCol;
               break;
            } # If: found library path
         }; # Loop: find path to library
      }' <<< "$tclLib" \
)";

#printf "%s\t%s\n" "$tcLibId" "$tclPath";
printf "%s\n" "$tclPath";
