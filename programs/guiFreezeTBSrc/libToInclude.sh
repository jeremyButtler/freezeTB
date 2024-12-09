pathStr="$1";

pathStr="$( \
   sed '
       s/lib\( *\)/include\1/g;
       s/-L/-I/g;
     ' <<< "$pathStr" \
   )";

pathStr="$(\
   awk '{
           for(siCol = 1; siCol <= NF; ++siCol)
              printf "%s %s/tcl-tk ", $siCol, $siCol;
       }' <<< "$pathStr" \
)";
   #sed 'p; s/include/&\/tcl-tk/g;' <<< "$pathStr" \

spaceStr="";

while read strPath;
do # Loop: check if valid directories
   if [[ "$strPath" == "" ]]; then
      continue;
   fi

   tmpPathStr="$(sed 's/-I//' <<< "$strPath")";

   if [[ -d "$tmpPathStr" ]] ;
   then
      printf "%s%s" "$spaceStr" "$strPath";
      spaceStr=" ";
   fi # If: is a directory
done <<< "$(tr ' ' '\n' <<< "$pathStr")"
   # Loop: check if valid directories
