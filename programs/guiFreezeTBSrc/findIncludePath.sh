if [[ -d "/opt/X11/include" ]];
then
   hPath="$hPath -I/opt/X11/include";
fi

if [[ -d "/opt/homebrew/include" ]];
then
   hPath="$hPath -I/opt/homebrew/include";
fi

if [[ -d "/usr/include/tcl-tk" ]] ;
then
   hPath="$hPath -I/usr/include/tcl-tk";
fi

if [[ -d "/usr/include" ]];
then
   hPath="$hPath -I/usr/include";
fi

if [[ -d "/usr/X11/include" ]];
then
   hPath="$hPath -I/usr/include";
fi

if [[ -d "/usr/local/include/tcl-tk" ]];
then
   hPath="$hPath -I/usr/local/include/tcl-tk";
fi

if [[ -d "/usr/local/include" ]];
then
   hPath="$hPath -I/usr/local/include";
fi

hPath="$(find /usr -name tcl.h 2>/dev/null)";
hPath="$hPath $(find /opt -name tcl.h 2>/dev/null)";

hPath="$hPath $(find /usr -name X11 2>/dev/null | sed 's/\/X11//;')";
hPath="$hPath $(find /opt -name X11 2>/dev/null | sed 's/\/X11//;')";
hPath="$( \
   sed \
     's/[a-zA-Z0-9.]*X11[a-zA-Z0-9.]*.h//g; s/\/X11//g;'\
     <<< "$hPath"
)"; # remove X11 ending (most places now to add on

hPath="$( \
   sed 's/[A-Za-z0-9.]*tcl.h//;' <<< "$hPath" \
)";

hPath="$(sort <<< "$hPath" | uniq | tr '\n' '&')";
hPath="$(sed 's/\&/ /g; s/^/-I/; s/ \// -I\//g;' <<< "$hPath")";

printf "%s\n" "$hPath";
