X11Lib="$(ls /opt/X11/lib/libX11.dylib 2>/dev/null)";

X11Lib="$X11Lib $(ls /usr/lib/libX11.so 2>/dev/null)";
X11Lib="$X11Lib $(ls /usr/lib64/libX11.so 2>/dev/null)";
X11Lib="$X11Lib $(ls /usr/local/lib/libX11.so 2>/dev/null)";
X11Lib="$X11Lib $(ls /usr/local/lib64/libX11.so 2>/dev/null)";
X11Lib="$X11Lib $(ls /usr/lib/x86_64-linux-gnu/libX11.so 2>/dev/null)";

X11Lib="$X11Lib $(ls /usr/X11/lib/libX11.so 2>/dev/null)";
X11Lib="$X11Lib $(ls /usr/X11R6/lib/libX11.so 2>/dev/null)";
X11Lib="$(tr '\n' ' ' <<< "$X11Lib")";

awk '{print $1;};' <<< "$X11Lib";
