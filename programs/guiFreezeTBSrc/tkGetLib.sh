#!/usr/bin/env sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# tkGetLib.sh SOF: Start Of File
#   - finds the path to a tk library
#   o sec01:
#     - find path to tk libraries
#   o sec02:
#     - pick one library in path search
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - find path to tk libraries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# trying as many file paths as possible. Some likely have
#   never existed, but just in case.
# /opt/homebrew is from mac
# /usr/lib is from unix
# /usr/lib64 is from unix (never seen tcltk here)
# /usr/libexec is from unix (never seen tcltk here)
# /usr/local/lib is from openbsd
# the x68_64-linux-gnu is from ubuntu (mabye debain??)

for strLib in \
   /opt/homebrew/lib/lib*tk*.dylib \
   /opt/homebrew/Cellar/tcl-tk@8/8.6.15/lib/lib*tk*.dylib\
   /usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9tk9.0.dylib \
   /usr/lib/libtk*.so \
   /usr/lib/x86_64-linux-gnu/libtk*.so \
   /usr/lib64/libtk*.so \
   /usr/lib64/x86_64-linux-gnu/libtk*.so \
   /usr/libexec/libtk*.so \
   /usr/libexec/x86_64-linux-gnu/libtk*.so \
   /usr/local/lib/libtk*.so \
   /usr/local/lib/x86_64-linux-gnu/libtk*.so \
   /usr/local/lib64/libtk*.so \
   /usr/local/lib64/x86_64-linux-gnu/libtk*.so \
   /usr/local/libexec/libtk*.so \
   /usr/local/libexec/x86_64-linux-gnu/libtk*.so \
;
do # Loop: find tk files
   if [ ! -f "$strLib" ];
   then # If: null case
      continue;
   fi # If: null case

   tkLib="$tkLib $strLib";
done # Loop: find tk files

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - pick one library in path search
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

tkLib="$(\
   printf "%s" "$tkLib" |
      sed '
         s/^[ \t]*//;
         s/[ \t]*$//;
         s/[ \t][ \t]/ /;
      ' \
)";

tkLib="$(printf "%s" "$tkLib" | tr ' ' '\n')";
tkLib="$(printf "%s" "$tkLib" | grep -v "stub")";
tkLib="$(printf "%s" "$tkLib" | awk '{print $1; exit;};')";

printf "%s" "$tkLib";
