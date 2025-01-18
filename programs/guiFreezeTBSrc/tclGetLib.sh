#!/usr/bin/env sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# tclGetLib.sh SOF: Start Of File
#   - finds the path to a tcl library
#   o sec01:
#     - find path to tcl libraries
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
  /opt/homebrew/lib/libtcl*.dylib \
  /opt/homebrew/Cellar/tcl-tk@8/8.6.15/lib/libtcl*.dylib\
  /usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9.0.dylib \
  /usr/lib/libtcl*.so \
  /usr/lib/x86_64-linux-gnu/libtcl*.so \
  /usr/lib64/libtcl*.so \
  /usr/lib64/x86_64-linux-gnu/libtcl*.so \
  /usr/libexec/libtcl*.so \
  /usr/libexec/x86_64-linux-gnu/libtcl*.so \
  /usr/local/lib/libtcl*.so \
  /usr/local/lib/x86_64-linux-gnu/libtcl*.so \
  /usr/local/lib64/libtcl*.so \
  /usr/local/lib64/x86_64-linux-gnu/libtcl*.so \
  /usr/local/libexec/libtcl*.so \
  /usr/local/libexec/x86_64-linux-gnu/libtcl*.so \
;
do # Loop: find tcl files
   if [ ! -f "$strLib" ];
   then # If: null case
      continue;
   fi # If: null case

   tclLib="$tclLib $strLib";
done # Loop: find tcl files

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - pick one library in path search
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

tclLib="$(\
   printf "%s" "$tclLib" |
      sed '
         s/^[ \t]*//;
         s/[ \t]*$//;
         s/[ \t][ \t]/ /;
      ' \
)";

tclLib="$(printf "%s" "$tclLib" | tr ' ' '\n')";
tclLib="$(printf "%s" "$tclLib" | grep -v "stub")";
tclLib="$(printf "%s" "$tclLib" | grep -v "tcl[0-9]*tk")";
tclLib="$(\
   printf "%s" "$tclLib" | awk '{print $1; exit;};' \
)";

printf "%s" "$tclLib";
