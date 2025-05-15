#!/usr/bin/sh

scriptDirStr="$(printf "%s" "$(cd "$(dirname "$0")" && pwd)")"

helpStr="$(basename "$0") program_name main_file library_one library_two library_three ... library_n
   - builds make files for unix, windows, and plan9
   o program_name: name to call compiled program
   o main_file: main file of program (without .c extension)
   o library_number: library program uses (do not use
     .c extensions)
   o .h file: if a file ends in .h, then buildMkfiles.sh
     assumes that there is no .c or that the .c/.o file
     will not be used. The .h file is added to the list of
     files for make to check
     o the script knows the dependencys for each library,
       so you only need to put the top level dependency,
       however, it is ok if you wish to include all
       libraries
       - for example; samEntry would also print entries
         for strAry, ulCp, base10str, and numToStr
     o Options:
       - base10str (genLib)
       - charCp (genLib)
       - checkSum (genLib)
       - endin (genLib)
       - genMath (genLib)
       - inflate (genLib)
       - numToStr (genLib)
       - ptrAry (genLib)
       - shellSort (genLib)
       - strAry (genLib)
       - ulCp (genLib)

       - adjCoords (genBio)
       - ampDepth (genBio)
       - cigToEqx (genBio)
       - codoTble (genBio)
       - edDist (genBio)
       - geneCoord (genBio)
       - kmerCnt (genBio)
       - maskPrim (genBio)
       - rmHomo (genBio)
       - samEntry (genBio)
       - seqST (genBio)
       - tbCon (genBio)
       - trimSam (genBio)

       - alnSet (genAln)
       - dirMatrix (genAln)
       - indexToCoord (genAln)
       - kmerFind (genAln)
       - mapRead (genAln)
       - memwater (genAln)
       - needle (genAln)
       - samToAln (genAln)
       - water (genAln)

       - illNano (genClust)
       - clustST (genClust)
       - edClust (genClust)
"

if [ "$1" = "" ]; then
   printf "%s\n" "$helpStr";
   exit
elif [ "$1" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit
elif [ "$1" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit
fi
# $* is all input

bash "$scriptDirStr/buildMkfile.sh" bug "$@" > mkfile.bug
bash "$scriptDirStr/buildMkfile.sh" unix "$@" > mkfile.unix
bash "$scriptDirStr/buildMkfile.sh" static "$@" > mkfile.static
bash "$scriptDirStr/buildMkfile.sh" win "$@" > mkfile.win
bash "$scriptDirStr/buildMkfile.sh" plan9 "$@" > mkfile.plan9
