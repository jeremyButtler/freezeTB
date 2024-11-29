head -n 11 gui.h > tmp.h;
scriptToCStr <gui.tcl >> tmp.h;
printf "\"; /*glob_guiScript_gui*/\n" >> tmp.h;
mv tmp.h gui.h
