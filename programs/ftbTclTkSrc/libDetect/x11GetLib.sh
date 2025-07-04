#!/usr/bin/env sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# x11GetLib.sh SOF: Start Of File
#   - finds the path to a X11.so library
#   o sec01:
#     - find path to X11.so libraries
#   o sec02:
#     - pick one library in path search
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - find path to X11 libraries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# trying as many file paths as possible. Some likely have
#   never existed, but just in case.

for strLib in \
   /opt/X11/lib/libX11.dylib \
   /usr/lib/libX11.so \
   /usr/lib64/libX11.so \
   /usr/local/lib/libX11.so \
   /usr/local/lib64/libX11.so \
   /usr/lib/x86_64-linux-gnu/libX11.so \
   /usr/X11/lib/libX11.so \
   /usr/X11R6/lib/libX11.so \
;
do # Loop: find X11 files
   if [ ! -f "$strLib" ];
   then # If: null case
      continue;
   fi # If: null case

   x11Lib="$x11Lib $strLib";
done # Loop: find X11 files

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - pick one library in path search
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

x11Lib="$(\
   printf "%s" "$x11Lib" |
      sed '
         s/^[ \t]*//;
         s/[ \t]*$//;
         s/[ \t][ \t]/ /;
      ' \
)";

x11Lib="$(printf "%s" "$x11Lib" | tr ' ' '\n')";
x11Lib="$(printf "%s" "$x11Lib" | grep -v "stub")";

x11Lib="$(\
   printf "%s" "$x11Lib" | awk '{print $1; exit;};' \
)";

printf "%s" "$x11Lib";
