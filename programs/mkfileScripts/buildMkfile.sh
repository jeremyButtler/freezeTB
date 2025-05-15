#!/usr/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# buildMkfile SOF:
#   - builds a mkfile for a specific OS
#   o sec01:
#     - variable declarations
#   o sec02:
#     - set up headers and variables for specified mkfile
#   o sec03:
#     - set up mkfile build commands
#   o sec04:
#     - build make file command
#   o sec05:
#     - print mkfile (makefile)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

slashSC=""; # OS file separator

# potential .h/.c file locations (libraries)
genLibStr="";
genBioStr="";
genAlnStr="";
genClustStr="";
#genGenoTypeStr="";
#seqByIdStr=""; add in for k2 taxa

# marks if library location was used (bioTools)
genLibBl=0;
genBioBl=0;
genAlnBl=0;
genClustBl=0;
#genGenoTypeBl=0;

# unique libraryes for freezeTB
genFreezeTBStr="";
genFreezeTBBl=0;

# for building compile calls
coreFlagsStr=""; # CFLAGS
ccStr="";     # CC
dashOStr="";  # -o
spaceStr="   ";  # spacing for objFiles entry
newLineStr="";# holds new line to add between commands
warnStr="-Wall -Wextra -Wpedantic"; # warnings

# for printing out make file
headStr="";     # first part of file
includeStr="";  # for windows tcltk
libPathStr="";  # library paths
linkStr="";     # link (all:) command
ldStr="";       # LD command in link
ldFlagsStr="";  # ld flag command
objFilesStr=""; # object files for copiling (objFiles)
mainCmdStr="";  # commat to build main .c file
cleanStr="";    # command to clean up object files
installStr="";  # install command
tclTkBl=0;

osStr="$1";       # OS/mkfile type
nameStr="$2";       # program name
mainFileStr="$3"; # main file input by user


helpStr="$(basename "$0") OS program_name main_file libraries ...
   - OS: OS t build mkfile for
     o bug: debug
     o unix: general unix (Linux/BSD/MAC)
     o static: unix supporting static (Linux/BSD)
     o win: windows
     o plan9: plan9
   - program_name:
     o what to name the output program
   - main_file:
     o main file for program
     o do not use .c exentsion
   - libraries or .h files:
     o libraries to include
     o do not use .c extensions
     o use .h extension to specify .h file
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
 

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - set up headers and variables for specified mkfile
#   o sec02 sub01:
#     - debuging mkfiles
#   o sec02 sub02:
#     - unix mkfiles
#   o sec02 sub03:
#     - static (unix) mkfiles
#   o sec02 sub04:
#     - windows mkfiles
#   o sec02 sub05:
#     - plan9 mkfiles
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [ "$1" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$1" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$1" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
fi

# add in some warnings I have seen others use. One or two
#   are left out because they often send warnings about
#   things I do (such as pointer casting used in ulCp)
warnStr="$warnStr -Wundef";
warnStr="$warnStr -Waggregate-return -Wwrite-strings";
warnStr="$warnStr -Wstrict-prototypes";
warnStr="$warnStr -Wpointer-arith -Wshadow"
warnStr="$warnStr -Wunreachable-code -Winit-self"
   # -Wfloat-eqaul messes up numToStr. I am comparing
   #   floats to see if there is a decimal, so float == 0
   #   is safe
   # -Wcast-align warns about ulCp on Mac's clang. I know
   #   it works on Mac so removing

#*********************************************************
# Sec02 Sub01:
#   - debuging mkfiles
#*********************************************************

if [ "$osStr" = "bug" ];
then # If: debugging make file
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nLD=cc";
   headStr="$headStr\ncoreCFLAGS= -c -O0 -std=c89 -g";
   headStr="$headStr -Werror $warnStr";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.bug";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   #genGenoTypeStr="\$(genGenoType)/";
   genFreezeTBStr="\$(genFreezeTB)/";

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="";

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n";
   ldStr="\t\$(LD)";
   ldFlagsStr="${dashOStr}\$(NAME) \$(objFiles)";

   slashSC="/";
# If: debugging make file

#*********************************************************
# Sec02 Sub02:
#   - unix mkfiles
#*********************************************************

elif [ "$osStr" = "unix" ];
then # Else If: general unix make file
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nLD=cc";
   headStr="$headStr\ncoreCFLAGS= -c -O3 -std=c89 $warnStr";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.unix";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   #genGenoTypeStr="\$(genGenoType)/";
   genFreezeTBStr="\$(genFreezeTB)/";

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)"
   installStr="$installStr\/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n";
   ldStr="\t\$(LD)";
   ldFlagsStr="${dashOStr}\$(NAME) \$(objFiles)";

   slashSC="/";
# Else If: general unix make file

#*********************************************************
# Sec02 Sub03:
#   - static (unix) mkfiles
#*********************************************************

elif [ "$osStr" = "static" ];
then # Else If: static unix make file
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nLD=cc";
   headStr="$headStr\ncoreCFLAGS= -c -O3 -std=c89 -static";
   headStr="$headStr $warnStr";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.static";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   #genGenoTypeStr="\$(genGenoType)/";
   genFreezeTBStr="\$(genFreezeTB)/";

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n";
   ldStr="\t\$(LD)";
   ldFlagsStr="${dashOStr}\$(NAME) \$(objFiles)";

   slashSC="/";
# Else If: static unix make file

#*********************************************************
# Sec02 Sub04:
#   - windows mkfiles
#*********************************************************

elif [ "$osStr" = "win" ];
then # Else If: windows make file
   headStr="$headStr\nCC=cl.exe";
   headStr="$headStr\nLD=link.exe";
   headStr="$headStr\ncoreCFLAGS= /c /O2 /Ot /Za /Tc";
   headStr="$headStr\nCFLAGS=/DNONE";
   headStr="$headStr\nNAME=$nameStr.exe";
   headStr="$headStr\nPREFIX=\"%localAppData%\"";
   headStr="$headStr\nO=o.win";

   genLibStr="\$(genLib)\\\\";
   genBioStr="\$(genBio)\\\\";
   genAlnStr="\$(genAln)\\\\";
   genClustStr="\$(genClust)\\\\";
   #genGenoTypeStr="\$(genGenoType)\\\\";
   genFreezeTBStr="\$(genFreezeTB)\\\\";
   #seqByIdStr="\$(seqByIdSrc)\\\\"; # add in for k2Taxa

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="/Fo:";

   installStr="install:"
   installStr="$installStr\n\tmove \$(NAME) \$(PREFIX)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\tdel \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n";
   ldStr="\t\$(LD)";
   ldFlagsStr="/out:\$(NAME) \$(objFiles)";

   slashSC="\\";
# Else If: windows make file

#*********************************************************
# Sec02 Sub05:
#   - plan9 mkfiles
#*********************************************************

elif [ "$osStr" = "plan9" ];
then # Else If: plan9 make file
   headStr="</\$objtype/mkfile";
   headStr="$headStr\ncoreCFLAGS=-DPLAN9";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=\$home/bin";

   genLibStr="\$genLib/";
   genBioStr="\$genBio/";
   genAlnStr="\$genAln/";
   genClustStr="\$genClust/";
   #genGenoTypeStr="\$genGenoType/";
   genFreezeTBStr="\$genFreezeTB/";
   #seqByIdStr="\$seqByIdSrc/"; # add in when in k2Taxa

   coreFlagsStr="\$coreCFLAGS";
   cFlagsStr="\$CFLAGS";
   ccStr="\$CC";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n";
   ldStr="\t\$(LD)";
   ldFlagsStr="${dashOStr}\$NAME \$objFiles";

   slashSC="/";
# Else If: plan9 make file

else
   exit; # not make file
fi # check makefile type

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - set up mkfile build commands
#   o sec03 sub01:
#     - setup start of main file build command
#   o sec03 sub02:
#     - genLib libraries
#   o sec03 sub03:
#     - genBio libraries
#   o sec03 sub04:
#     - genAln libraries
#   o sec03 sub05:
#     - genClust libraries
#   o sec03 sub06:
#     - genGenoType libraries
#   o sec03 sub07:
#     - code for freezeTB (genFreezeTB)
#   o sec03 sub08:
#     - tcltk library linking
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - setup start of main file build command
#*********************************************************

mainCmdStr="$mainFileStr.\$O: \\
	$mainFileStr.c";

#*********************************************************
# Sec03 Sub02:
#   - genLib libraries
#   o sec03 sub02 cat01:
#     - numToStr
#   o sec03 sub02 cat02:
#     - base10str
#   o sec03 sub02 cat03:
#     - ulCp
#   o sec03 sub02 cat04:
#     - charCp
#   o sec03 sub02 cat05:
#     - genMath
#   o sec03 sub02 cat06:
#     - shellSort
#   o sec03 sub02 cat07:
#     - endin
#   o sec03 sub02 cat08:
#     - checkSum
#   o sec03 sub02 cat09:
#     - strAry
#   o sec03 sub02 cat10:
#     - ptrAry
#   o sec03 sub02 cat11:
#     - fileFun
#   o sec03 sub02 cat12:
#     - inflate
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat01:
#   - numToStr
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

numToStr="${genLibStr}numToStr.\$O: \\
	${genLibStr}numToStr.c \\
	${genLibStr}numToStr.h
		$ccStr ${dashOStr}${genLibStr}numToStr.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}numToStr.c"

numToStrObj="${genLibStr}numToStr.\$O";
numToStrDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat02:
#   - base10str
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

base10Str="${genLibStr}base10str.\$O: \\
	${genLibStr}base10str.c \\
	${genLibStr}base10str.h
		$ccStr ${dashOStr}${genLibStr}base10str.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}base10str.c"

base10strObj="${genLibStr}base10str.\$O";
base10strDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat03:
#   - ulCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ulCpStr="${genLibStr}ulCp.\$O: \\
	${genLibStr}ulCp.c \\
	${genLibStr}ulCp.h
		$ccStr ${dashOStr}${genLibStr}ulCp.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}ulCp.c"

ulCpObj="${genLibStr}ulCp.\$O";
ulCpDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat04:
#   - charCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

charCpStr="${genLibStr}charCp.\$O: \\
	${genLibStr}charCp.c \\
	${genLibStr}charCp.h
		$ccStr ${dashOStr}${genLibStr}charCp.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}charCp.c"

charCpObj="${genLibStr}charCp.\$O";
charCpDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat05:
#   - genMath
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

genMathStr="${genLibStr}genMath.\$O: \\
	${genLibStr}genMath.c \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genLibStr}genMath.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}genMath.c"

genMathObj="${genLibStr}genMath.\$O";
genMathDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat06:
#   - shellSort
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

shellSortStr="${genLibStr}shellSort.\$O: \\
	${genLibStr}shellSort.c \\
	${genLibStr}shellSort.h
		$ccStr ${dashOStr}${genLibStr}shellSort.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}shellSort.c"

shellSortObj="${genLibStr}shellSort.\$O";
shellSortDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat07:
#   - endin
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

endinStr="${genLibStr}endin.\$O: \\
	${genLibStr}endin.c \\
	${genLibStr}endin.h
		$ccStr ${dashOStr}${genLibStr}endin.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}endin.c"

endinObj="${genLibStr}endin.\$O";
endinDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat08:
#   - checkSum
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

checkSumStr="${genLibStr}checkSum.\$O: \\
	${genLibStr}checkSum.c \\
	${genLibStr}checkSum.h \\
	${genLibStr}endin.\$O
	$ccStr ${dashOStr}${genLibStr}checkSum.\$O \\
		$cFlagsStr $coreFlagsStr ${genLibStr}checkSum.c";


checkSumObj="${genLibStr}checkSum.\$O";
checkSumDep="endin $endinDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat09:
#   - strAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

strAryStr="${genLibStr}strAry.\$O: \\
	${genLibStr}strAry.c \\
	${genLibStr}strAry.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}strAry.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}strAry.c";


strAryObj="${genLibStr}strAry.\$O";
strAryDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat10:
#   - ptrAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ptrAryStr="${genLibStr}ptrAry.\$O: \\
	${genLibStr}ptrAry.c \\
	${genLibStr}ptrAry.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}ptrAry.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}ptrAry.c";


ptrAryObj="${genLibStr}ptrAry.\$O";
ptrAryDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat11:
#   - fileFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

fileFunStr="${genLibStr}fileFun.\$O: \\
	${genLibStr}fileFun.c \\
	${genLibStr}fileFun.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}fileFun.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}fileFun.c";


fileFunObj="${genLibStr}fileFun.\$O";
fileFunDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat12:
#   - inflate
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inflateStr="${genLibStr}inflate.\$O: \\
	${genLibStr}inflate.c \\
	${genLibStr}inflate.h \\
	${genLibStr}checkSum.\$O \\
	${genLibStr}ulCp.\$O \\
	${genLibStr}genMath.\$O \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genLibStr}inflate.\$O \\
			$cFlagsStr $coreFlagsStr ${genLibStr}inflate.c";


inflateObj="${genLibStr}inflate.\$O";
inflateDep="checkSum $checkSumDep genMath $genMathDep";
inflateDep="$inflateDep ulCp $ulCpDep";


#*********************************************************
# Sec03 Sub03:
#   - genBio libraries
#   o sec03 sub03 cat01:
#     - codonFun
#   o sec03 sub03 cat02:
#     - kmerFun
#   o sec03 sub03 cat03:
#     - seqST
#   o sec03 sub03 cat04:
#     - gzSeqST
#   o sec03 sub03 cat05:
#     - kmerCnt
#   o sec03 sub03 cat06:
#     - geneCoord
#   o sec03 sub03 cat07:
#     - samEntry
#   o sec03 sub03 cat08:
#     - samRef
#   o sec03 sub03 cat09:
#     - adjCoords
#   o sec03 sub03 cat10:
#     - cigToEqx
#   o sec03 sub03 cat11:
#     - maskPrim
#   o sec03 sub03 cat12:
#     - trimSam
#   o sec03 sub03 cat13:
#     - ampDepth
#   o sec03 sub03 cat14:
#     - tbCon
#   o sec03 sub03 cat15:
#     - edDist
#   o sec03 sub03 cat16:
#     - rmHomo
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat01:
#   - codonFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

codonFunStr="${genBioStr}codonFun.\$O: \\
	${genBioStr}codonFun.c \\
	${genBioStr}codonFun.h \\
	${genBioStr}codonTbl.h \\
	${genBioStr}ntTo2Bit.h \\
	${genBioStr}revNtTo2Bit.h
		$ccStr ${dashOStr}${genBioStr}codonFun.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}codonFun.c"

codonFunObj="${genBioStr}codonFun.\$O";
codonFunDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat02:
#   - kmerFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerFunStr="${genBioStr}kmerFun.\$O: \\
	${genBioStr}kmerFun.c \\
	${genBioStr}kmerFun.h \\
	${genBioStr}kmerBit.h
		$ccStr ${dashOStr}${genBioStr}kmerFun.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}kmerFun.c"

kmerFunObj="${genBioStr}kmerFun.\$O";
kmerFunDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat03:
#   - seqST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

seqSTStr="${genBioStr}seqST.\$O: \\
	${genBioStr}seqST.c \\
	${genBioStr}seqST.h \\
	${genLibStr}fileFun.\$O
		$ccStr ${dashOStr}${genBioStr}seqST.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}seqST.c";


seqSTObj="${genBioStr}seqST.\$O";
seqSTDep="fileFun $fileFunDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat04:
#   - gzSeqST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

gzSeqSTStr="${genBioStr}gzSeqST.\$O: \\
	${genBioStr}gzSeqST.c \\
	${genBioStr}gzSeqST.h \\
	${genBioStr}seqST.\$O \\
 	${genLibStr}inflate.\$O
		$ccStr ${dashOStr}${genBioStr}gzSeqST.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}gzSeqST.c";


gzSeqSTObj="${genBioStr}gzSeqST.\$O";
gzSeqSTDep="inflate $inflateDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat05:
#   - kmerCnt
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerCntStr="${genBioStr}kmerCnt.\$O: \\
	${genBioStr}kmerCnt.c \\
	${genBioStr}kmerCnt.h \\
	${genBioStr}seqST.\$O \\
	${genBioStr}ntTo2Bit.h \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genBioStr}kmerCnt.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}kmerCnt.c";


kmerCntObj="${genBioStr}kmerCnt.\$O";
kmerCntDep="seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat06:
#   - geneCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

geneCoordStr="${genBioStr}geneCoord.\$O: \\
	${genBioStr}geneCoord.c \\
	${genBioStr}geneCoord.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}charCp.\$O \\
	${genLibStr}genMath.h
	$ccStr ${dashOStr}${genBioStr}geneCoord.\$O \\
		$cFlagsStr $coreFlagsStr ${genBioStr}geneCoord.c";

geneCoordObj="${genBioStr}geneCoord.\$O";
geneCoordDep="fileFun $fileFunDep base10str $base10strDep";
geneCoordDep="$geneCoordDep $ulCpDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat07:
#   - samEntry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samEntryStr="${genBioStr}samEntry.\$O: \\
	${genBioStr}samEntry.c \
	${genBioStr}samEntry.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}strAry.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}numToStr.\$O \\
	${genBioStr}ntTo5Bit.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}samEntry.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}samEntry.c";

samEntryObj="${genBioStr}samEntry.\$O";
samEntryDep="fileFun $fileFunDep strAry $strAryDep";
samEntryDep="$samEntryDep numToStr $numToStrDep";
samEntryDep="$samEntryDep base10str $base10strDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat08:
#   - samRef
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samRefStr="${genBioStr}samRef.\$O: \\
	${genBioStr}samRef.c \
	${genBioStr}samRef.h \\
	${genBioStr}samEntry.\$O \\
	${genLibStr}strAry.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}samRef.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}samRef.c";

samRefObj="${genBioStr}samRef.\$O";
samRefDep="samEntry $samEntryDep strAry $strAryDep";
samRefDep="$samRefDep base10str $base10strDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat09:
#   - adjCoords
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

adjCoordsStr="${genBioStr}adjCoords.\$O: \\
	${genBioStr}adjCoords.c \\
	${genBioStr}adjCoords.h \\
	${genBioStr}geneCoord.\$O \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}adjCoords.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}adjCoords.c";

adjCoordsObj="${genBioStr}adjCoords.\$O";
adjCoordsDep="geneCoord $geneCoordDep";
adjCoordsDep="$adjCoordsDep samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat10:
#   - cigToEqx
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

cigToEqxStr="${genBioStr}cigToEqx.\$O: \\
	${genBioStr}cigToEqx.c \\
	${genBioStr}cigToEqx.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}cigToEqx.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}cigToEqx.c";

cigToEqxObj="${genBioStr}cigToEqx.\$O";
cigToEqxDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat11:
#   - maskPrim
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

maskPrimStr="${genBioStr}maskPrim.\$O: \\
	${genBioStr}maskPrim.c \\
	${genBioStr}maskPrim.h \\
	${genBioStr}samEntry.\$O \\
	${genLibStr}shellSort.\$O
		$ccStr ${dashOStr}${genBioStr}maskPrim.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}maskPrim.c";

maskPrimObj="${genBioStr}maskPrim.\$O";
maskPrimDep="samEntry $samEntryDep";
maskPrimDep="$maskPrimDep shellSort shellSortDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat12:
#   - trimSam
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

trimSamStr="${genBioStr}trimSam.\$O: \\
	${genBioStr}trimSam.c \\
	${genBioStr}trimSam.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}trimSam.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}trimSam.c";

trimSamObj="${genBioStr}trimSam.\$O";
trimSamDep="samEntry samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat13:
#   - ampDepth
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ampDepthStr="${genBioStr}ampDepth.\$O: \\
	${genBioStr}ampDepth.c \\
	${genBioStr}ampDepth.h \\
	${genBioStr}geneCoord.\$O \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}ampDepth.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}ampDepth.c";

ampDepthObj="${genBioStr}ampDepth.\$O";
ampDepthDep="geneCoord $geneCoordDep";
ampDepthDep="$ampDepthDep samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat14:
#   - tbCon
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tbConStr="${genBioStr}tbCon.\$O: \\
	${genBioStr}tbCon.c \\
	${genBioStr}tbCon.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}tbConDefs.h \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}tbCon.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}tbCon.c";

tbConObj="${genBioStr}tbCon.\$O";
tbConDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat15:
#   - edDist
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edDistStr="${genBioStr}edDist.\$O: \\
	${genBioStr}edDist.c \\
	${genBioStr}edDist.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}edDist.\$O \\
		$cFlagsStr $coreFlagsStr ${genBioStr}edDist.c";


edDistObj="${genBioStr}edDist.\$O";
edDistDep="samEntry $samEntryDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat16:
#   - rmHomo
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

rmHomoStr="${genBioStr}rmHomo.\$O: \\
	${genBioStr}rmHomo.c \\
	${genBioStr}rmHomo.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O
		$ccStr ${dashOStr}${genBioStr}rmHomo.\$O \\
			$cFlagsStr $coreFlagsStr ${genBioStr}rmHomo.c";


rmHomoObj="${genBioStr}rmHomo.\$O";
rmHomoDep="samEntry $samEntryDep seqST $seqSTDep";

#*********************************************************
# Sec03 Sub04:
#   - genAln libraries
#   o sec03 sub04 cat01:
#     - indexToCoord
#   o sec03 sub04 cat02:
#     - alnSet
#   o sec03 sub04 cat03:
#     - dirMatrix
#   o sec03 sub04 cat04:
#     - water
#   o sec03 sub04 cat05:
#     - needle
#   o sec03 sub04 cat06:
#     - memwater
#   o sec03 sub04 cat07:
#     - kmerFind
#   o sec03 sub04 cat08:
#     - samToAln
#   o sec03 sub04 cat09:
#     - mapRead
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat01:
#   - indexToCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

indexToCoordStr="${genAlnStr}indexToCoord.\$O: \\
	${genAlnStr}indexToCoord.c \\
	${genAlnStr}indexToCoord.h
		$ccStr ${dashOStr}${genAlnStr}indexToCoord.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}indexToCoord.c";


indexToCoordObj="${genAlnStr}indexToCoord.\$O";
indexToCoordDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat02:
#   - alnSet
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

alnSetStr="${genAlnStr}alnSet.\$O: \\
	${genAlnStr}alnSet.c \\
	${genAlnStr}alnDefs.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}ulCp.\$O \\
	${genLibStr}base10str.\$O \\
	${genAlnStr}alnDefs.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genAlnStr}alnSet.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}alnSet.c";


alnSetObj="${genAlnStr}alnSet.\$O";
alnSetDep="base10str $base10strDep fileFun $fileFunDep"

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat03:
#   - dirMatrix
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

dirMatrixStr="${genAlnStr}dirMatrix.\$O: \\
	${genAlnStr}dirMatrix.c \\
	${genAlnStr}alnSet.\$O \\
	${genAlnStr}indexToCoord.\$O \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}charCp.\$O
		$ccStr ${dashOStr}${genAlnStr}dirMatrix.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}dirMatrix.c";


dirMatrixObj="${genAlnStr}dirMatrix.\$O";
dirMatrixDep="alnSet $alnSetDep";
dirMatrixDep="$dirMatrixDep indexToCoord";
dirMatrixDep="$dirMatrixDep $indexToCoordDep";
dirMatrixDep="$dirMatrixDep samEntry $samEntryDep";
dirMatrixDep="$dirMatrixDep seqST $seqSTDep";
dirMatrixDep="$dirMatrixDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat04:
#   - water
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

waterStr="${genAlnStr}water.\$O: ${genAlnStr}water.c \\
	${genAlnStr}dirMatrix.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}water.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}water.c";


waterObj="${genAlnStr}water.\$O";
waterDep="dirMatrix $dirMatrixDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat05:
#   - needle
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

needleStr="${genAlnStr}needle.\$O: \\
	${genAlnStr}needle.c \\
	${genAlnStr}dirMatrix.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}needle.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}needle.c";


needleObj="${genAlnStr}needle.\$O";
needleDep="dirMatrix $dirMatrixDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat06:
#   - memwater
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

memwaterStr="${genAlnStr}memwater.\$O: \\
	${genAlnStr}memwater.c \\
	${genAlnStr}alnSet.\$O \\
	${genAlnStr}indexToCoord.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}memwater.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}memwater.c";


memwaterObj="${genAlnStr}memwater.\$O";
memwaterDep="alnSet $alnSetDep indexToCoord";
memwaterDep="$memwaterDep $indexToCoordDep";
memwaterDep="$memwaterDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat07:
#   - kmerFind
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerFindStr="${genAlnStr}kmerFind.\$O: \\
	${genAlnStr}kmerFind.c \\
	${genAlnStr}memwater.\$O \\
	${genLibStr}shellSort.\$O \\
	${genLibStr}genMath.h \\
	${genBioStr}kmerBit.h
		$ccStr ${dashOStr}${genAlnStr}kmerFind.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}kmerFind.c";


kmerFindObj="${genAlnStr}kmerFind.\$O";
kmerFindDep="memwater $memwaterDep";
kmerFindDep="$kmerFindDep shellSort $shellSortDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat08:
#   - samToAln
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samToAlnStr="${genAlnStr}samToAln.\$O: \\
	${genAlnStr}samToAln.c \\
	${genAlnStr}samToAln.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genAlnStr}alnSet.\$O
		$ccStr ${dashOStr}${genAlnStr}samToAln.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}samToAln.c";


samToAlnObj="${genAlnStr}samToAln.\$O";
samToAlnDep="alnSet $alnSetDep samEntry $samEntryDep";
samToAlnDep="$samToAlnDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat09:
#   - mapRead
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

mapReadStr="${genAlnStr}mapRead.\$O: \\
	${genAlnStr}mapRead.c \\
	${genAlnStr}mapRead.h \\
	${genAlnStr}needle.\$O \\
	${genAlnStr}water.\$O \\
	${genAlnStr}dirMatrix.\$O \\
	${genBioStr}kmerFun.\$O \\
	${genLibStr}shellSort.\$O \\
	${genAlnStr}defsMapRead.h  \\
	${genLibStr}genMath.h
	 	$ccStr ${dashOStr}${genAlnStr}mapRead.\$O \\
			$cFlagsStr $coreFlagsStr ${genAlnStr}mapRead.c";


mapReadObj="${genAlnStr}mapRead.\$O";
mapReadDep="needle $needleDep dirMatrix $dirMatrixDep";
mapReadDep="$mapReadDep water $waterDep";
mapReadDep="$mapReadDep dirMatrix $dirMatrixDep";
mapReadDep="$mapReadDep samEntry $samEntryDep";
mapReadDep="$mapReadDep seqST $seqSTDep";
mapReadDep="$mapReadDep kmerFun $kmerFunDep";
mapReadDep="$mapReadDep shellSort $shellSortDep";

#*********************************************************
# Sec03 Sub05:
#   - genClust libraries
#   o sec03 sub04 cat01:
#     - illNano
#   o sec03 sub04 cat02:
#     - clustST
#   o sec03 sub04 cat03:
#     - edClust
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat01:
#   - illNano
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

illNanoStr="${genClustStr}illNano.\$O: \\
	${genClustStr}illNano.c \\
	${genClustStr}illNano.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genClustStr}illNano.\$O \\
			$cFlagsStr $coreFlagsStr ${genClustStr}illNano.c;
";


illNanoObj="${genClustStr}illNano.\$O";
illNanoDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat02:
#   - clustST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

clustSTStr="${genClustStr}clustST.\$O: \\
	${genClustStr}clustST.c \\
	${genClustStr}clustST.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}tbCon.\$O \\
	${genBioStr}edDist.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genClustStr}clustST.\$O \\
			$cFlagsStr $coreFlagsStr ${genClustStr}clustST.c
";

clustSTObj="${genClustStr}clustST.\$O";
clustSTDep="samEntry $samEntryDep tbCon $tbConDep";
clustSTDep="$clustSTDep edDist $edDistDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat03:
#   - edClust
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edClustStr="${genClustStr}edClust.\$O: \\
	${genClustStr}edClust.c \\
	${genClustStr}edClust.h \\
	${genClustStr}clustST.\$O
		$ccStr ${dashOStr}${genClustStr}edClust.\$O \\
			$cFlagsStr $coreFlagsStr ${genClustStr}edClust.c
";

edClustObj="${genClustStr}edClust.\$O";
edClustDep="clustST $clustSTDep";

#*********************************************************
# Sec03 Sub07:
#   - code for freezeTB (genFreezeTB)
#   o sec03 sub07 cat01:
#     - drugAry (antibiotic array for tbAmr/amrST)
#   o sec03 sub07 cat02:
#     - amrST (for tbAmr)
#   o sec03 sub07 cat03:
#     - checkAmr (for tbAmr)
#   o sec03 sub07 cat04:
#     - miruTbl (for tbMiru)
#   o sec03 sub07 cat05:
#     - miruFx (for tbMiru)
#   o sec03 sub07 cat06:
#     - spolST (for tbSpol)
#   o sec03 sub07 cat07:
#     - spolFind (for tbSpol)
#   o sec03 sub07 cat08:
#     - refSwap (for swapRef)
#   o sec03 sub07 cat09:
#     - genIndice (for whoToAmr)
#   o sec03 sub07 cat10:
#     - whoToAmr (for whoToAmr)
#   o sec03 sub07 cat11:
#     - freezeTB (for mainFreezeTB)
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat01:
#   - drugAry (antibiotic array for tbAmr/amrST)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

drugAryStr="${genFreezeTBStr}drugAry.\$O: \\
	${genFreezeTBStr}drugAry.c ${genFreezeTBStr}drugAry.h
		$ccStr $dashOStr${genFreezeTBStr}drugAry.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}drugAry.c";

drugAryObj="${genFreezeTBStr}drugAry.\$O";
drugAryDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat02:
#   - amrST (for tbAmr)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

amrSTStr="${genFreezeTBStr}amrST.\$O: \\
	${genFreezeTBStr}amrST.c \\
	${genFreezeTBStr}amrST.h \\
	${genFreezeTBStr}drugAry.\$O \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}charCp.\$O \\
	${genLibStr}endLine.h
		$ccStr $dashOStr${genFreezeTBStr}amrST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}amrST.c";

amrSTObj="${genFreezeTBStr}amrST.\$O";
amrSTDep="drugAry $drugAryDep fileFun $fileFunDep";
amrSTDep="$amrSTDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat03:
#   - checkAmr (for tbAmr)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

checkAmrStr="${genFreezeTBStr}checkAmr.\$O: \\
	${genFreezeTBStr}checkAmr.c \\
	${genFreezeTBStr}checkAmr.h \\
	${genFreezeTBStr}amrST.\$O \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}codonFun.\$O \\
	${genLibStr}genMath.\$O
		$ccStr $dashOStr${genFreezeTBStr}checkAmr.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}checkAmr.c";

checkAmrObj="${genFreezeTBStr}checkAmr.\$O";
checkAmrDep="amrST $amrSTDep samEntry $samEntryDep";
checkAmrDep="$checkAmrDep codonFun $codonFunDep";
checkAmrDep="$checkAmrDep genMath $genMathDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat04:
#   - miruTbl (for tbMiru)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

miruTblStr="${genFreezeTBStr}miruTbl.\$O: \\
	${genFreezeTBStr}miruTbl.c \\
	${genFreezeTBStr}miruTbl.h \\
	${genBioStr}samEntry.\$O \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}charCp.\$O \\
	${genLibStr}genMath.h \\
	${genFreezeTBStr}tbMiruDefs.h
		$ccStr $dashOStr${genFreezeTBStr}miruTbl.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}miruTbl.c";

miruTblObj="${genFreezeTBStr}miruTbl.\$O";
miruTblDep="samEntry $samEntryDep fileFun $fileFunDep";
miruTblDep="$miruTblDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat05:
#   - miruFx (for tbMiru)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

miruFxStr="${genFreezeTBStr}miruFx.\$O: \\
	${genFreezeTBStr}miruFx.c \\
	${genFreezeTBStr}miruFx.h \\
	${genFreezeTBStr}miruTbl.\$O \\
	${genBioStr}seqST.\$O \\
	${genAlnStr}kmerFind.\$O \\
	${genLibStr}charCp.\$O \\
	${genFreezeTBStr}tbMiruDefs.h
		$ccStr $dashOStr${genFreezeTBStr}miruFx.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}miruFx.c";

miruFxObj="${genFreezeTBStr}miruFx.\$O";
miruFxDep="miruTbl $miruTblDep kmerFind $kmerFindDep";
miruFxDep="$miruFxDep seqST $seqSTDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat06:
#   - spolST (for tbSpol)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

spolSTStr="${genFreezeTBStr}spolST.\$O: \\
	${genFreezeTBStr}spolST.c \\
	${genFreezeTBStr}spolST.h \\
	${genLibStr}numToStr.\$O \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}charCp.\$O \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h \\
	${genFreezeTBStr}tbSpolDefs.h
		$ccStr $dashOStr${genFreezeTBStr}spolST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}spolST.c";

spolSTObj="${genFreezeTBStr}spolST.\$O";
spolSTDep="numToStr $numToStrDep fileFun $fileFunDep";
spolSTDep="$spolSTDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat07:
#   - spolFind (for tbSpol)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

spolFindStr="${genFreezeTBStr}spolFind.\$O: \\
	${genFreezeTBStr}spolFind.c \\
	${genFreezeTBStr}spolFind.h \\
	${genFreezeTBStr}spolST.\$O \\
	${genBioStr}seqST.\$O \\
	${genBioStr}samEntry.\$O \\
	${genAlnStr}kmerFind.\$O
		$ccStr $dashOStr${genFreezeTBStr}spolFind.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}spolFind.c";

spolFindObj="${genFreezeTBStr}spolFind.\$O";
spolFindDep="spolST $spolSTDep seqST $seqSTDep";
spolFindDep="$spolFindDep samEntry $samEntryDep";
spolFindDep="$spolFindDep kmerFind $kmerFindDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat08:
#   - refSwap (for swapDbRef)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

refSwapStr="${genFreezeTBStr}refSwap.\$O: \\
	${genFreezeTBStr}refSwap.c \\
	${genFreezeTBStr}refSwap.h \\
	${genFreezeTBStr}amrST.\$O \\
	${genBioStr}samEntry.\$O
		$ccStr $dashOStr${genFreezeTBStr}refSwap.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}refSwap.c";

refSwapObj="${genFreezeTBStr}refSwap.\$O";
refSwapDep="amrST $amrSTDep samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat09:
#   - genIndice (for whoToAmr)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

genIndiceStr="${genFreezeTBStr}genIndice.\$O: \\
	${genFreezeTBStr}genIndice.c \\
	${genFreezeTBStr}genIndice.h \\
	${genLibStr}base10str.\$O \\
	${genLibStr}charCp.\$O
		$ccStr $dashOStr${genFreezeTBStr}genIndice.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}genIndice.c";

genIndiceObj="${genFreezeTBStr}genIndice.\$O";
genIndiceDep="base10str $base10strDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat10:
#   - whoToAmr (for whoToAmr)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

whoToAmrStr="${genFreezeTBStr}whoToAmr.\$O: \\
	${genFreezeTBStr}whoToAmr.c \\
	${genFreezeTBStr}whoToAmr.h \\
	${genFreezeTBStr}genIndice.\$O \\
	${genFreezeTBStr}amrST.\$O \\
	${genBioStr}codonFun.\$O \\
	${genLibStr}genMath.\$O \\
	${genLibStr}endLine.h
		$ccStr $dashOStr${genFreezeTBStr}whoToAmr.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}whoToAmr.c";

whoToAmrObj="${genFreezeTBStr}whoToAmr.\$O";
whoToAmrDep="genIndice $genIndiceDep amrST $amrSTDep";
whoToAmrDep="$whoToAmrDep genMath $genMathDep";
whoToAmrDep="$whoToAmrDep codonFun $codonFunDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat11:
#   - freezeTBPaths (for freezeTB)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

freezeTBPathsStr="${genFreezeTBStr}freezeTBPaths.\$O: \\
	${genFreezeTBStr}freezeTBPaths.c \\
	${genFreezeTBStr}freezeTBPaths.h \\
	${genLibStr}ulCp.\$O
		$ccStr \\
			$dashOStr${genFreezeTBStr}freezeTBPaths.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}freezeTBPaths.c";

freezeTBPathsObj="${genFreezeTBStr}freezeTBPaths.\$O";
freezeTBPathsDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat12:
#   - freezeTB (for mainFreezeTB)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

freezeTBStr="${genFreezeTBStr}freezeTB.\$O: \\
	${genFreezeTBStr}freezeTB.c \\
	${genFreezeTBStr}freezeTB.h \\
	${genFreezeTBStr}checkAmr.\$O \\
	${genFreezeTBStr}miruTbl.\$O \\
	${genFreezeTBStr}spolFind.\$O \\
	${genFreezeTBStr}freezeTBPaths.\$O \\
	${genAlnStr}mapRead.\$O \\
	${genBioStr}gzSeqST.\$O \\
	${genBioStr}trimSam.\$O \\
	${genBioStr}maskPrim.\$O \\
	${genBioStr}ampDepth.\$O \\
	${genBioStr}adjCoords.\$O \\
	${genBioStr}rmHomo.\$O \\
	${genBioStr}tbCon.\$O \\
	${genClustStr}edClust.\$O \\
	${genFreezeTBStr}freezeTBDefs.h \\
	${genFreezeTBStr}tbAmrDefs.h \\
	${genFreezeTBStr}tbMiruDefs.h \\
	${genFreezeTBStr}tbSpolDefs.h \\
	${genBioStr}tbConDefs.h \\
	${genLibStr}charCp.h \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h \\
	..${slashSC}ftbVersion.h
		$ccStr $dashOStr${genFreezeTBStr}freezeTB.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genFreezeTBStr}freezeTB.c";

freezeTBObj="${genFreezeTBStr}freezeTB.\$O";
freezeTBDep="freezeTBPaths $freezeTBPathsDep";
freezeTBDep="$freezeTBDep checkAmr $checkAmrDep";
freezeTBDep="$freezeTBDep miruTbl $miruTblDep";
freezeTBDep="$freezeTBDep spolFind $spolFindDep";
freezeTBDep="$freezeTBDep trimSam $trimSamDep";
freezeTBDep="$freezeTBDep maskPrim $maskPrimDep";
freezeTBDep="$freezeTBDep ampDepth $ampDepthDep";
freezeTBDep="$freezeTBDep adjCoords $adjCoordsDep";
freezeTBDep="$freezeTBDep rmHomo $rmHomoDep";
freezeTBDep="$freezeTBDep edClust $edClustDep";
freezeTBDep="$freezeTBDep tbCon $tbConDep";
freezeTBDep="$freezeTBDep mapRead $mapReadDep";
freezeTBDep="$freezeTBDep gzSeqST $gzSeqSTDep";

#*********************************************************
# Sec03 Sub08:
#   - tcltk library linking
#   o sec03 sub08 cat01:
#     - windows
#   o sec03 sub07 cat02:
#     - unix; tcltk object files
#   o sec03 sub07 cat03:
#     - unix tcltk include files
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat01:
#   - windows
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if [ "$osStr" = "win" ];
then
   tclTkStr="$headStr\n\n
!IF [cmd /C IF EXIST \"%programFiles%\\Tcl90\\lib\\\\tk90t.lib\" exit 1]
tclTkInclude = /I \"%programFiles%\\Tcl90\\include\"
tclTkLib = \"%programFiles%\\Tcl90\\lib\\\\tk90t.lib\" \"%programFiles%\\Tcl90\\lib\\\\tcl90t.lib\"
!MESSAGE found tcltk 9.0.x in \"%programFiles%\"

!IF [cmd /C IF EXIST \"%programFiles%\\Tcl90\\lib\\\\tcl9tk90.lib\" exit 1]
tclTkInclude = /I \"%programFiles%\\Tcl90\\include\"
tclTkLib = \"%programFiles%\\Tcl90\\lib\\\\tcl9tk90.lib\" \"%programFiles%\\Tcl90\\lib\\\\tcl90.lib\"
!MESSAGE found tcltk 9.0.x in \"%programFiles%\"

!ELSEIF [cmd /C IF EXIST \"%programFiles%\\Tcl86\\lib\\\\tk86t.lib\" exit 1]
tclTkInclude = /I \"%programFiles%\\Tcl86\\include\"
tclTkLib = \"%programFiles%\\Tcl86\\lib\\\\tk86t.lib\" \"%programFiles%\\Tcl86\\lib\\\\tcl86t.lib\"
!MESSAGE found tcltk 8.6.x in \"%programFiles%\"

!ELSEIF [cmd /C IF EXIST \"%programFiles%\\Tcl86\\lib\\\\tcl8tk86.lib\" exit 1]
tclTkInclude = /I \"%programFiles%\\Tcl86\\include\"
tclTkLib = \"%programFiles%\\Tcl86\\lib\\\\tcl8tk86.lib\" \"%programFiles%\\Tcl86\\lib\\\\tcl86.lib\"
!MESSAGE found tcltk 8.6.x in \"%programFiles%\"

!ELSEIF [cmd /C IF EXIST \"%localAppData%\\Apps\\Tcl90\\lib\\\\tk90t.lib\" exit 1]
tclTkInclude = /I \"%localAppData%\\Apps\\Tcl90\\include\"
tclTkLib = \"%localAppData%\\Apps\\Tcl90\\lib\\\\tk90t.lib\" \"%localAppData%\\Apps\\Tcl90\\lib\\\\tcl90t.lib\"
!MESSAGE found tcltk 9.0.x in \"%localAppData%\\Apps\"

!ELSEIF [cmd /C IF EXIST \"%localAppData%\\Apps\\Tcl90\\lib\\\\tcl9tk90.lib\" exit 1]
tclTkInclude = /I \"%localAppData%\\Apps\\Tcl90\\include\"
tclTkLib = \"%localAppData%\\Apps\\Tcl90\\lib\\\\tcl9tk90.lib\" \"%localAppData%\\Apps\\Tcl90\\lib\\\\tcl90.lib\"
!MESSAGE found tcltk 9.0.x in \"%localAppData%\\Apps\"

!ELSEIF [cmd /C IF EXIST \"%localAppData%\\Apps\\Tcl86\\lib\\\\tk86t.lib\" exit 1]
tclTkInclude = /I \"%localAppData%\\Apps\\Tcl86\\include\"
tclTkLib = \"%localAppData%\\Apps\\Tcl86\\lib\\\\tk86t.lib\" \"%localAppData%\\Apps\\Tcl86\\lib\\\\tcl86t.lib\"
!MESSAGE found tcltk 8.6.x is in \"%localAppData%\\Apps\"

!ELSEIF [cmd /C IF EXIST \"%localAppData%\\Apps\\Tcl86\\lib\\\\tcl8tk86.lib\" exit 1]
tclTkInclude = /I \"%localAppData%\\Apps\\Tcl86\\include\"
tclTkLib = \"%localAppData%\\Apps\\Tcl86\\lib\\\\tcl8tk86.lib\" \"%localAppData%\\Apps\\Tcl86\\lib\\\\tcl86.lib\"
!MESSAGE found tcltk 8.6.x is in \"%localAppData%\\Apps\"

!ELSE
!ERROR could not find tcltk
!ENDIF
  ";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat02:
#   - unix; tcltk object files
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

else
# Else: building unix make file
   # THIS TRICK FAILED ON MAC; the program would complie,
   #   but then would crash on running
   # trying as many file paths as possible. Some likely
   #   have never existed, but just in case.
   # /opt/homebrew is from mac
   # /usr/lib is from unix
   # /usr/lib64 is from unix (never seen tcltk here)
   # /usr/libexec is from unix (never seen tcltk here)
   # /usr/local/lib is from openbsd
   # the x68_64-linux-gnu is from ubuntu (mabye debain??)

   tclTkStr="@x11Obj=\"\"; \\
		if [ -f /opt/X11/lib/libX11.dylib ]; then x11Obj=\"/opt/X11/lib/libX11.dylib\"; \\
		elif [ -f /usr/lib/libX11.so ]; then x11Obj=\"/usr/lib/libX11.so\"; \\
		elif [ -f /usr/lib64/libX11.so ]; then x11Obj=\"/usr/lib64/libX11.so\"; \\
		elif [ -f /usr/local/lib/libX11.so ]; then x11Obj=\"/usr/local/lib/libX11.so\"; \\
		elif [ -f /usr/local/lib64/libX11.so ]; then x11Obj=\"/usr/local/lib64/libX11.so\"; \\
		elif [ -f /usr/lib/x86_64-linux-gnu/libX11.so ]; then x11Obj=\"/usr/lib/x86_64-linux-gnu/libX11.so\"; \\
		elif [ -f /usr/X11/lib/libX11.so ]; then x11Obj=\"usr/X11/lib/libX11.so\"; \\
		elif [ -f /usr/X11R6/lib/libX11.so ]; then x11Obj=\"usr/X11R6/lib/libX11.so\"; \\
		else echo \"could not find libX11\"; exit; \\
		fi; \\
		\\
		tclObj=\"\"; \\
		if [ -f /opt/homebrew/lib/libtcl9.0.dylib ]; then tclObj=/opt/homebrew/lib/libtcl9.0.dylib; \\
		elif [ -f /opt/homebrew/lib/libtcl8.6.dylib ]; then tclObj=/opt/homebrew/lib/libtcl8.6.dylib; \\
		elif [ -f /opt/homebrew/lib/libtcl9.0.dylib ]; then tclObj=/opt/homebrew/lib/libtcl9.0.dylib; \\
		elif [ -f /opt/homebrew/Cellar/tcl-tk@8/8.6.15/lib/libtcl8.6.dylib ]; then tclObj=/opt/homebrew/Cellar/tcl-tk@8/8.6.15/lib/libtcl8.6.dylib; \\
		elif [ -f /usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9.0.dylib ]; then tclObj=/usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9.0.dylib; \\
		elif [ -f /usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9.0.dylib ]; then tclObj=/usr/local/Cellar/tcl-tk/9.0.1/lib/libtcl9.0.dylib; \\
		elif [ -f /usr/lib/libtcl9.0.so ]; then tclObj=/usr/lib/libtcl9.0.so; \\
		elif [ -f /usr/lib/libtcl8.6.so ]; then tclObj=/usr/lib/libtcl8.6.so; \\
		elif [ -f /usr/local/lib/libtcl9.0.so ]; then tclObj=/usr/local/lib/libtcl9.0.so; \\
		elif [ -f /usr/local/lib/libtcl8.6.so ]; then tclObj=/usr/local/lib/libtcl8.6.so; \\
		elif [ -f /usr/lib/x86_64-linux-gnu/libtcl9.0.so ]; then tclObj=/usr/lib/x86_64-linux-gnu//libtcl9.0.so; \\
		elif [ -f /usr/lib/x86_64-linux-gnu/libtcl8.6.so ]; then tclObj=/usr/lib/x86_64-linux-gnu/libtcl8.6.so; \\
		elif [ -f /usr/local/lib/x86_64-linux-gnu/libtcl9.0.so ]; then tclObj=/usr/local/lib/x86_64-linux-gnu/libtcl9.0.so; \\
		elif [ -f /usr/local/lib/x86_64-linux-gnu/libtcl8.6.so ]; then tclObj=/usr/local/lib/x86_64-linux-gnu/libtcl8.6.so; \\
		elif [ -f /usr/lib64/libtcl9.0.so ]; then tclObj=/usr/lib64/libtcl9.0.so; \\
		elif [ -f /usr/lib64/libtcl8.6.so ]; then tclObj=/usr/lib64/libtcl8.6.so; \\
		elif [ -f /usr/lib64/x86_64-linux-gnu/libtcl9.0.so ]; then tclObj=/usr/lib64/x86_64-linux-gnu/libtcl9.0.so; \\
		elif [ -f /usr/lib64/x86_64-linux-gnu/libtcl8.6.so ]; then tclObj=/usr/lib64/x86_64-linux-gnu/libtcl8.6.so; \\
		elif [ -f /usr/local/lib64/libtcl9.0.so ]; then tclObj=/usr/local/lib64/x86_64-linux-gnu/libtcl9.0.so; \\
		elif [ -f /usr/local/lib64/libtcl8.6.so ]; then tclObj=/usr/local/lib64/x86_64-linux-gnu/libtcl8.6.so; \\
		elif [ -f /usr/local/lib64/x86_64-linux-gnu/libtcl9.0.so ]; then tclObj=/usr/local/lib64/x86_64-linux-gnu/libtcl9.0.so; \\
		elif [ -f /usr/local/lib64/x86_64-linux-gnu/libtcl8.6.so ]; then tclObj=/usr/local/lib64/x86_64-linux-gnu/libtcl8.6.so; \\
		elif [ -f /usr/libexec/libexectcl9.0.so ]; then tclObj=/usr/libexec/libexectcl9.0.so; \\
		elif [ -f /usr/libexec/libexectcl8.6.so ]; then tclObj=/usr/libexec/libexectcl8.6.so; \\
		elif [ -f /usr/libexec/x86_64-linux-gnu/libexectcl9.0.so ]; then tclObj=/usr/libexec/x86_64-linux-gnu/libexectcl9.0.so; \\
		elif [ -f /usr/libexec/x86_64-linux-gnu/libexectcl8.6.so ]; then tclObj=/usr/libexec/x86_64-linux-gnu/libexectcl8.6.so; \\
		elif [ -f /usr/local/libexec/libexectcl9.0.so ]; then tclObj=/usr/local/libexec/libexectcl9.0.so; \\
		elif [ -f /usr/local/libexec/libexectcl8.6.so ]; then tclObj=/usr/local/libexec/libexectcl8.6.so; \\
		elif [ -f /usr/local/libexec/x86_64-linux-gnu/libexectcl9.0.so ]; then tclObj=/usr/local/libexec/x86_64-linux-gnu/libexectcl9.0.so; \\
		elif [ -f /usr/local/libexec/x86_64-linux-gnu/libexectcl8.6.so ]; then tclObj=/usr/local/libexec/x86_64-linux-gnu/libexectcl8.6.so; \\
		else echo \"could not find libtcl\"; exit; \\
		fi; \\
		\\
		tkObj=\"\"; \\
		if [ -f /opt/homebrew/lib/libtcl9tk9.0.dylib ]; then tkObj=/opt/homebrew/lib/libtcl9tk9.0.dylib; \\
		elif [ -f /opt/homebrew/lib/libtcl8tk8.6.dylib ]; then tkObj=/opt/homebrew/lib/libtcl8tk8.6.dylib; \\
		elif [ -f /opt/homebrew/Cellar/tk-tk@8/8.6.15/lib/libtcl8tk8.6.dylib ]; then tkObj=/opt/homebrew/Cellar/tk-tk@8/8.6.15/lib/libtcl8tk8.6.dylib; \\
		elif [ -f /usr/local/Cellar/tk-tk/9.0.1/lib/libtcl9tk9.0.dylib ]; then tkObj=/usr/local/Cellar/tk-tk/9.0.1/lib/libtcl9tk9.0.dylib; \\
		elif [ -f /usr/local/Cellar/tk-tk/9.0.1/lib/libtcl9tk9.0.dylib ]; then tkObj=/usr/local/Cellar/tk-tk/9.0.1/lib/libtcl9tk9.0.dylib; \\
		elif [ -f /usr/lib/libtk9.0.so ]; then tkObj=/usr/lib/libtk9.0.so; \\
		elif [ -f /usr/lib/libtk8.6.so ]; then tkObj=/usr/lib/libtk8.6.so; \\
		elif [ -f /usr/local/lib/libtk9.0.so ]; then tkObj=/usr/local/lib/libtk9.0.so; \\
		elif [ -f /usr/local/lib/libtk8.6.so ]; then tkObj=/usr/local/lib/libtk8.6.so; \\
		elif [ -f /usr/lib/x86_64-linux-gnu/libtk9.0.so ]; then tkObj=/usr/lib/x86_64-linux-gnu//libtk9.0.so; \\
		elif [ -f /usr/lib/x86_64-linux-gnu/libtk8.6.so ]; then tkObj=/usr/lib/x86_64-linux-gnu/libtk8.6.so; \\
		elif [ -f /usr/local/lib/x86_64-linux-gnu/libtk9.0.so ]; then tkObj=/usr/local/lib/x86_64-linux-gnu/libtk9.0.so; \\
		elif [ -f /usr/local/lib/x86_64-linux-gnu/libtk8.6.so ]; then tkObj=/usr/local/lib/x86_64-linux-gnu/libtk8.6.so; \\
		elif [ -f /usr/lib64/libtk9.0.so ]; then tkObj=/usr/lib64/libtk9.0.so; \\
		elif [ -f /usr/lib64/libtk8.6.so ]; then tkObj=/usr/lib64/libtk8.6.so; \\
		elif [ -f /usr/lib64/x86_64-linux-gnu/libtk9.0.so ]; then tkObj=/usr/lib64/x86_64-linux-gnu/libtk9.0.so; \\
		elif [ -f /usr/lib64/x86_64-linux-gnu/libtk8.6.so ]; then tkObj=/usr/lib64/x86_64-linux-gnu/libtk8.6.so; \\
		elif [ -f /usr/local/lib64/libtk9.0.so ]; then tkObj=/usr/local/lib64/x86_64-linux-gnu/libtk9.0.so; \\
		elif [ -f /usr/local/lib64/libtk8.6.so ]; then tkObj=/usr/local/lib64/x86_64-linux-gnu/libtk8.6.so; \\
		elif [ -f /usr/local/lib64/x86_64-linux-gnu/libtk9.0.so ]; then tkObj=/usr/local/lib64/x86_64-linux-gnu/libtk9.0.so; \\
		elif [ -f /usr/local/lib64/x86_64-linux-gnu/libtk8.6.so ]; then tkObj=/usr/local/lib64/x86_64-linux-gnu/libtk8.6.so; \\
		elif [ -f /usr/libexec/libexectk9.0.so ]; then tkObj=/usr/libexec/libexectk9.0.so; \\
		elif [ -f /usr/libexec/libexectk8.6.so ]; then tkObj=/usr/libexec/libexectk8.6.so; \\
		elif [ -f /usr/libexec/x86_64-linux-gnu/libexectk9.0.so ]; then tkObj=/usr/libexec/x86_64-linux-gnu/libexectk9.0.so; \\
		elif [ -f /usr/libexec/x86_64-linux-gnu/libexectk8.6.so ]; then tkObj=/usr/libexec/x86_64-linux-gnu/libexectk8.6.so; \\
		elif [ -f /usr/local/libexec/libexectk9.0.so ]; then tkObj=/usr/local/libexec/libexectk9.0.so; \\
		elif [ -f /usr/local/libexec/libexectk8.6.so ]; then tkObj=/usr/local/libexec/libexectk8.6.so; \\
		elif [ -f /usr/local/libexec/x86_64-linux-gnu/libexectk9.0.so ]; then tkObj=/usr/local/libexec/x86_64-linux-gnu/libexectk9.0.so; \\
		elif [ -f /usr/local/libexec/x86_64-linux-gnu/libexectk8.6.so ]; then tkObj=/usr/local/libexec/x86_64-linux-gnu/libexectk8.6.so; \\
		else echo \"could not find libtk\"; exit; \\
		fi; \\
		$ldStr \$\$x11Obj \$\$tkObj \$\$tclObj ";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat03:
#   - unix tcltk include files
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

includeStr="
		@hPath=""; \\
		if [ -d /opt/X11/include ]; then hPath=\"\$\$hPath -I/opt/X11/include\"; fi; \\
		if [ -d /opt/homebrew/include ]; then hPath=\"\$\$hPath -I/opt/homebrew/include\"; fi; \\
		if [ -d /usr/include/tcl-tk ]; then hPath=\"\$\$hPath -I/usr/include/tcl-tk\"; fi; \\
		if [ -d /usr/include ]; then hPath=\"\$\$hPath -I/usr/include\"; fi; \\
		if [ -d /usr/X11/include ]; then hPath=\"\$\$hPath -I/usr/X11/include\"; fi; \\
		if [ -d /usr/local/include/tcl-tk ]; then hPath=\"\$\$hPath -I/usr/local/include/tcl-tk\"; fi; \\
		if [ -d /usr/local/include ]; then hPath=\"\$\$hPath -I/usr/local/include\"; fi; \\
      \\"

fi # check if unix or windows make file

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - build make file command
#   o sec04 sub01:
#     o move past name and OS and get user arguments
#   o sec04 sub02:
#     - deal with .h files and spacing + start mkfile loop
#   o sec04 sub03:
#     - genLib libraries + start makefile loop
#   o sec04 sub04:
#     - genBio libraries
#   o sec04 sub05:
#     - genAln libraries
#   o sec04 sub07:
#     - genClust libraries
#   o sec04 sub07:
#     - genotypeing libraries (not used)
#   o sec04 sub08:
#     - freezeTB libraries
#   o sec04 sub09:
#     - move to next library or dependency
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec04 Sub01:
#   o move past name and OS and get user arguments
#*********************************************************

objFilesStr="objFiles=";
mainCntSI=0;
libCntSI=0;

# move past header
shift; # move past os type
shift; # move past program name
shift; # move past main file

while [ $# -gt 0 ];
do   # Loop: get dependencies
   depStr="$depStr $1";
   mainCntSI="$((mainCntSI + 1))";
   shift;
done # Loop: get dependencies


depStr="${depStr#"${depStr%%[![:space:]]*}"}";
   # clear leading spaces
libStr="${depStr%% *}";
   # get first argument
depStr="${depStr#"${depStr%%[[:space:]]*}"}";
   # pop off first arugment
depStr="${depStr#"${depStr%%[![:space:]]*}"}";
   # clear leading spaces

#*********************************************************
# Sec04 Sub02:
#   - deal with .h files and spacing + start mkfile loop
#*********************************************************

firstBl="";

while [ "$libStr" != "" ];
do # Loop: get dependencies

   if [ "$firstBl" = "" ]; then
      newLineStr="";
      firstBl=1;
   else
      newLineStr="\n\n";
   fi 

   # this step is to save any .h files the user provided
   #   that way they are included in the command
   hFileBl="$(\
      printf "%s" "$libStr" | sed 's/.*\.h$/\.h/' \
   )";

   if [ "$hFileBl" = ".h" ];
   then # If: is a .h file
      mainCmdStr="$mainCmdStr \\
	$libStr";
      libCntSI="$((libCntSI + 1))";

      libStr="${depStr%% *}";
         # get first argument
      depStr="${depStr#"${depStr%%[[:space:]]*}"}";
         # pop off first arugment
      depStr="${depStr#"${depStr%%[![:space:]]*}"}";
         # clear leading spaces

      continue;
   fi

   #******************************************************
   # Sec04 Sub03:
   #   - genLib libraries
   #   o sec04 sub03 cat01:
   #     - numToStr
   #   o sec04 sub03 cat02:
   #     - base10str
   #   o sec04 sub03 cat03:
   #     - ulCp
   #   o sec04 sub03 cat04:
   #     - charCp
   #   o sec04 sub03 cat05:
   #     - genMath
   #   o sec04 sub03 cat06:
   #     - shellSort
   #   o sec04 sub03 cat07:
   #     - endin
   #   o sec04 sub03 cat08:
   #     - checkSum
   #   o sec04 sub03 cat09:
   #     - strAry
   #   o sec04 sub03 cat10:
   #     - ptrAry
   #   o sec04 sub03 cat11:
   #     - fileFun
   #   o sec04 sub03 cat12:
   #     - inflate
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat01:
   #   - numToStr
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   if [ "$libStr" = "numToStr" ]; then
   # If: numToStr library
      if [ "$numToStrBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$numToStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$numToStrObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\\n	$numToStrObj";
         fi

         numToStrBl=1;
         depStr="$depStr $numToStrDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # If: numToStr library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat02:
   #   - base10str
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "base10str" ]; then
   # Else If: base10str library
      if [ "$base10strBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$base10Str";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$base10strObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$base10strObj";
         fi

         base10strBl=1;

         depStr="$depStr $base10strDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: base10str library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat03:
   #   - ulCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ulCp" ]; then
   # Else If: ulCp library
      if [ "$ulCpBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ulCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$ulCpObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ulCpObj";
         fi

         ulCpBl=1;
         depStr="$depStr $ulCpDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: ulCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat04:
   #   - charCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "charCp" ]; then
   # Else If: charCp library
      if [ "$charCpBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$charCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$charCpObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$charCpObj";
         fi

         charCpBl=1;
         depStr="$depStr $charCpDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: charCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat05:
   #   - genMath
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "genMath" ]; then
   # Else If: genMath library
      if [ "$genMathBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$genMathStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$genMathObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$genMathObj";
         fi

         genMathBl=1;
         depStr="$depStr $genMathDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: genMath library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat06:
   #   - shellSort
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "shellSort" ]; then
   # Else If: shellSort library
      if [ "$shellSortBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$shellSortStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$shellSortObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$shellSortObj";
         fi

         shellSortBl=1;
         depStr="$depStr $shellSortDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: shellSort library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat07:
   #   - endin
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "endin" ]; then
   # Else If: endin library
      if [ "$endinBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$endinStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${endinObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$endinObj";
         fi

         endinBl=1;
         depStr="$depStr $endinDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: endin library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat08:
   #   - checkSum
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "checkSum" ]; then
   # Else If: checkSum library
      if [ "$checkSumBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$checkSumStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${checkSumObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$checkSumObj";
         fi

         checkSumBl=1;
         depStr="$depStr $checkSumDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: checkSum library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat09:
   #   - strAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "strAry" ]; then
   # Else If: strAry library
      if [ "$strAryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$strAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${strAryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$strAryObj";
         fi

         strAryBl=1;
         depStr="$depStr $strAryDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: strAry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat10:
   #   - ptrAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ptrAry" ]; then
   # Else If: ptrAry library
      if [ "$ptrAryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ptrAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${ptrAryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ptrAryObj";
         fi

         ptrAryBl=1;
         depStr="$depStr $ptrAryDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: ptrAry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat11:
   #   - fileFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "fileFun" ]; then
   # Else If: fileFun library
      if [ "$fileFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$fileFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${fileFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$fileFunObj";
         fi

         fileFunBl=1;
         depStr="$depStr $fileFunDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: fileFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat12:
   #   - inflate
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "inflate" ]; then
   # Else If: inflate library
      if [ "$inflateBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$inflateStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${inflateObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$inflateObj";
         fi

         inflateBl=1;
         depStr="$depStr $inflateDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: inflate library

   #******************************************************
   # Sec04 Sub04:
   #   - genBio libraries
   #   o sec04 sub04 cat01:
   #     - codonFun
   #   o sec04 sub04 cat02:
   #     - kmerFun
   #   o sec04 sub04 cat03:
   #     - seqST
   #   o sec04 sub04 cat04:
   #     - gzSeqST
   #   o sec04 sub04 cat05:
   #     - kmerCnt
   #   o sec04 sub04 cat06:
   #     - geneCoord
   #   o sec04 sub04 cat07:
   #     - samEntry
   #   o sec04 sub04 cat08:
   #     - samRef
   #   o sec04 sub04 cat09:
   #     - adjCoords
   #   o sec04 sub04 cat10:
   #     - cigToEqx
   #   o sec04 sub04 cat11:
   #     - maskPrim
   #   o sec04 sub04 cat12:
   #     - trimSam
	#   o sec04 sub04 cat13:
   #     - ampDepth
	#   o sec04 sub04 cat14:
   #     - tbCon
	#   o sec04 sub04 cat15:
   #     - edDist
	#   o sec04 sub04 cat16:
   #     - rmHomo
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat01:
   #   - codonFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "codonFun" ]; then
   # Else If: codonFun library
      if [ "$codonFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$codonFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${codonFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$codonFunObj";
         fi

         codonFunBl=1;
         depStr="$depStr $codonFunDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: codonFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat02:
   #   - kmerFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerFun" ]; then
   # Else If: kmerFun library
      if [ "$kmerFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerFunObj";
         fi

         kmerFunBl=1;
         depStr="$depStr $kmerFunDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: kmerFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat03:
   #   - seqST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "seqST" ]; then
   # Else If: seqST library
      if [ "$seqSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$seqSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${seqSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$seqSTObj";
         fi

         seqSTBl=1;

         depStr="$depStr $seqSTDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: seqST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat04:
   #   - seqST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "gzSeqST" ]; then
   # Else If: gzSeqST library
      if [ "$gzSeqSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$gzSeqSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${gzSeqSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$gzSeqSTObj";
         fi

         gzSeqSTBl=1;

         depStr="$depStr $gzSeqSTDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: gzSeqST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat05:
   #   - kmerCnt
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerCnt" ]; then
   # Else If: kmerCnt library
      if [ "$kmerCntBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerCntStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerCntObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerCntObj";
         fi

         kmerCntBl=1;
         depStr="$depStr $kmerCntDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: kmerCnt library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat06:
   #   - geneCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "geneCoord" ]; then
   # Else If: geneCoord library
      if [ "$geneCoordBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$geneCoordStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${geneCoordObj}";

         geneCoordBl=1;
         depStr="$depStr $geneCoordDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: geneCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat07:
   #   - samEntry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samEntry" ]; then
   # Else If: samEntry library
      if [ "$samEntryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samEntryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samEntryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samEntryObj";
         fi

         samEntryBl=1;
         depStr="$depStr $samEntryDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: samEntry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat08:
   #   - samRef
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samRef" ]; then
   # Else If: samRef library
      if [ "$samRefBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samRefStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samRefObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samRefObj";
         fi

         samRefBl=1;
         depStr="$depStr $samRefDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: samRef library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat09:
   #   - adjCoords
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "adjCoords" ]; then
   # Else If: adjCoords library
      if [ "$adjCoordsBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$adjCoordsStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${adjCoordsObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$adjCoordsObj";
         fi

         adjCoordsBl=1;
         depStr="$depStr $adjCoordsDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: adjCoords library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat10:
   #   - cigToEqx
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "cigToEqx" ]; then
   # Else If: cigToEqx library
      if [ "$cigToEqxBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$cigToEqxStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${cigToEqxObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$cigToEqxObj";
         fi

         cigToEqxBl=1;

         depStr="$depStr $cigToEqxDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: cigToEqx library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat11:
   #   - maskPrim
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "maskPrim" ]; then
   # Else If: maskPrim library
      if [ "$maskPrimBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$maskPrimStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${maskPrimObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$maskPrimObj";
         fi

         maskPrimBl=1;
         depStr="$depStr $maskPrimDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: maskPrim library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat12:
   #   - trimSam
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "trimSam" ]; then
   # Else If: trimSam library
      if [ "$trimSamBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$trimSamStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${trimSamObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$trimSamObj";
         fi

         trimSamBl=1;

         depStr="$depStr $trimSamDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: trimSam library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat13:
   #   - ampDepth
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ampDepth" ]; then
   # If: ampDepth library
      if [ "$ampDepthBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ampDepthStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${ampDepthObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ampDepthObj";
         fi

         ampDepthBl=1;
         depStr="$depStr $ampDepthDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: ampDepth library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat14:
   #   - tbCon
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "tbCon" ]; then
   # If: tbCon library
      if [ "$tbConBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$tbConStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${tbConObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$tbConObj";
         fi

         tbConBl=1;

         depStr="$depStr $tbConDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: tbCon library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat15:
   #   - edDist
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "edDist" ]; then
   # If: edDist library
      if [ "$edDistBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$edDistStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${edDistObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$edDistObj";
         fi

         edDistBl=1;
         depStr="$depStr $edDistDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: edDist library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat16:
   #   - rmHomo
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "rmHomo" ]; then
   # If: rmHomo library
      if [ "$rmHomoBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$rmHomoStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${rmHomoObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$rmHomoObj";
         fi

         rmHomoBl=1;
         depStr="$depStr $rmHomoDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: rmHomo library

   #******************************************************
   # Sec04 Sub05:
   #   - genAln libraries
   #   o sec04 sub05 cat01:
   #     - indexToCoord
   #   o sec04 sub05 cat02:
   #     - alnSet
   #   o sec04 sub05 cat03:
   #     - dirMatrix
   #   o sec04 sub05 cat04:
   #     - water
   #   o sec04 sub05 cat05:
   #     - needle
   #   o sec04 sub05 cat06:
   #     - memwater
   #   o sec04 sub05 cat07:
   #     - kmerFind
   #   o sec04 sub05 cat08:
   #     - samToAln
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat01:
   #   - indexToCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "indexToCoord" ]; then
   # Else If: indexToCoord library
      if [ "$indexToCoordBl" = "" ]; then
        cmdStr="$cmdStr$newLineStr$indexToCoordStr";
        objFilesStr="$objFilesStr \\\\\n$spaceStr";
        objFilesStr="${objFilesStr}${indexToCoordObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
           mainCmdStr="$mainCmdStr \\
	$indexToCoordObj";
        fi

        indexToCoordBl=1;
        depStr="$depStr $indexToCoordDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: indexToCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat02:
   #   - alnSet
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "alnSet" ]; then
   # Else If: alnSet library
      if [ "$alnSetBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$alnSetStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${alnSetObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$alnSetObj";
         fi

         alnSetBl=1;

         depStr="$depStr $alnSetDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: alnSet library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat03:
   #   - dirMatrix
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "dirMatrix" ]; then
   # Else If: dirMatrix library
      if [ "$dirMatrixBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$dirMatrixStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${dirMatrixObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$dirMatrixObj";
         fi

         dirMatrixBl=1;
         depStr="$depStr $dirMatrixDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: dirMatrix library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat04:
   #   - water
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "water" ]; then
   # Else If: water library
      if [ "$waterBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$waterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${waterObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$waterObj";
         fi

         waterBl=1;

         depStr="$depStr $waterDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: water library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat05:
   #   - needle
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "needle" ]; then
   # Else If: needle library
      if [ "$needleBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$needleStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${needleObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$needleObj";
         fi

         needleBl=1;
         depStr="$depStr $needleDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: needle library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat06:
   #   - memwater
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "memwater" ]; then
   # Else If: memwater library
      if [ "$memwaterBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$memwaterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${memwaterObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$memwaterObj";
         fi

         memwaterBl=1;
         depStr="$depStr $memwaterDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: memwater library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat07:
   #   - kmerFind
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerFind" ]; then
   # Else If: kmerFind library
      if [ "$kmerFindBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerFindStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerFindObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerFindObj";
         fi

         kmerFindBl=1;

         depStr="$depStr $kmerFindDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: kmerFin library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat08:
   #   - samToAln
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samToAln" ]; then
   # Else If: samToAln library
      if [ "$samToAlnBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samToAlnStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samToAlnObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samToAlnObj";
         fi

         samToAlnBl=1;
         depStr="$depStr $samToAlnDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: samToAln library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat09:
   #   - mapRead
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "mapRead" ]; then
   # Else If: mapRead library
      if [ "$mapReadBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$mapReadStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${mapReadObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$mapReadObj";
         fi

         mapReadBl=1;

         depStr="$depStr $mapReadDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: mapRead library

   #******************************************************
   # Sec04 Sub06:
   #   - genAln libraries
   #   o sec04 sub05 cat01:
   #     - illNano
   #   o sec04 sub05 cat02:
   #     - clustST
   #   o sec04 sub05 cat03:
   #     - edClust
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat01:
   #   - illNano
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "illNano" ]; then
   # Else If: illNano library
      if [ "$illNanoBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$illNanoStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${illNanoObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$illNanoObj";
         fi

         illNanoBl=1;
         depStr="$depStr $illNanoDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: illNano library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat02:
   #   - clustST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "clustST" ]; then
   # Else If: clustST library
      if [ "$clustSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$clustSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${clustSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$clustSTObj";
         fi

         clustSTBl=1;
         depStr="$depStr $clustSTDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: clustST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat03:
   #   - edClust
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "edClust" ]; then
   # Else If: edClust library
      if [ "$edClustBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$edClustStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${edClustObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$edClustObj";
         fi

         edClustBl=1;
         depStr="$depStr $edClustDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: edClust library


   #******************************************************
   # Sec04 Sub08:
   #   - freezeTB libraries
   #   o sec04 sub08 cat01:
   #     - drugAry
   #   o sec04 sub08 cat02:
   #     - amrST
   #   o sec04 sub08 cat03:
   #     - checkAmr
   #   o sec04 sub08 cat04:
   #     - miruTbl
   #   o sec04 sub08 cat05:
   #     - miruFx
   #   o sec04 sub08 cat06:
   #     - spolST
   #   o sec04 sub08 cat07:
   #     - spolFind
   #   o sec04 sub08 cat08:
   #     - refSwap
   #   o sec04 sub08 cat09:
   #     - genIndice
   #   o sec04 sub08 cat10:
   #     - whoToAmr
   #   o sec04 sub08 cat11:
   #     - freezeTBPaths
   #   o sec04 sub08 cat12:
   #     - freezeTB
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat01:
   #   - drugAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "drugAry" ]; then
   # Else If: drugAry program
      if [ "$drugAryBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${drugAryStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$drugAryObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$drugAryObj";
         fi

         drugAryBl=1;
         depStr="$depStr $drugAryDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: drugAry program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat02:
   #   - amrST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "amrST" ]; then
   # Else If: amrST program
      if [ "$amrSTBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${amrSTStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$amrSTObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$amrSTObj";
         fi

         amrSTBl=1;
         depStr="$depStr $amrSTDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: amrST program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat03:
   #   - checkAmr
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "checkAmr" ]; then
   # Else If: checkAmr program
      if [ "$checkAmrBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${checkAmrStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$checkAmrObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$checkAmrObj";
         fi

         checkAmrBl=1;
         depStr="$depStr $checkAmrDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: checkAmr program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat04:
   #   - miruTbl
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "miruTbl" ]; then
   # Else If: miruTbl program
      if [ "$miruTblBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${miruTblStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$miruTblObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$miruTblObj";
         fi

         miruTblBl=1;
         depStr="$depStr $miruTblDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: miruTbl program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat05:
   #   - miruFx
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "miruFx" ]; then
   # Else If: miruFx program
      if [ "$miruFxBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${miruFxStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$miruFxObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$miruFxObj";
         fi

         miruFxBl=1;
         depStr="$depStr $miruFxDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: miruFx program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat06:
   #   - spolST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "spolST" ]; then
   # Else If: spolST program
      if [ "$spolSTBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${spolSTStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$spolSTObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$spolSTObj";
         fi

         spolSTBl=1;
         depStr="$depStr $spolSTDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: spolST program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat07:
   #   - spolFind
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "spolFind" ]; then
   # Else If: spolFind program
      if [ "$spolFindBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${spolFindStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$spolFindObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$spolFindObj";
         fi

         spolFindBl=1;
         depStr="$depStr $spolFindDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: spolFind program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat08:
   #   - refSwap
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "refSwap" ]; then
   # Else If: refSwap program
      if [ "$refSwapBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${refSwapStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$refSwapObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$refSwapObj";
         fi

         refSwapBl=1;
         depStr="$depStr $refSwapDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: refSwap program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat09:
   #   - genIndice
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "genIndice" ]; then
   # Else If: genIndice program
      if [ "$genIndiceBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${genIndiceStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$genIndiceObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$genIndiceObj";
         fi

         genIndiceBl=1;
         depStr="$depStr $genIndiceDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: genIndice program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat10:
   #   - whoToAmr
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "whoToAmr" ]; then
   # Else If: whoToAmr program
      if [ "$whoToAmrBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${whoToAmrStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$whoToAmrObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$whoToAmrObj";
         fi

         whoToAmrBl=1;
         depStr="$depStr $whoToAmrDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: whoToAmr program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat11:
   #   - freezeTBPaths
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "freezeTBPaths" ]; then
   # Else If: freezeTBPaths program
      if [ "$freezeTBPathsBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${freezeTBPathsStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$freezeTBPathsObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$freezeTBPathsObj";
         fi

         freezeTBPathsBl=1;
         depStr="$depStr $freezeTBPathsDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: freezeTBPaths program

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat11:
   #   - freezeTB
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "freezeTB" ]; then
   # Else If: freezeTB program
      if [ "$freezeTBBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${freezeTBStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$freezeTBObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$freezeTBObj";
         fi

         freezeTBBl=1;
         depStr="$depStr $freezeTBDep";

         if [ "$genFreezeTBBl" -lt 1 ]; then
            genFreezeTBBl=1;
            libPathStr="$libPathStr\ngenFreezeTB=..${slashSC}genFreezeTB";
         fi
      fi
   # Else If: freezeTB program

   #******************************************************
   # Sec04 Sub09:
   #   - move to next library or dependency
   #******************************************************

   elif [ "$libStr" = "tcltk" ]; then
      tclTkBl=1;
   fi # check librarys called


   libCntSI="$((libCntSI + 1))";
   libStr="${depStr%% *}";
      # get first argument
   depStr="${depStr#"${depStr%%[[:space:]]*}"}";
      # pop off first arugment
   depStr="${depStr#"${depStr%%[![:space:]]*}"}";
      # clear leading spaces

   if [ "$libStr" = "" ];
   then # If: no lib in path
      if [ "$depStr" = "" ];
      then # Check if have more dependencies
         break; # finished
      else
         libStr="${depStr%% *}"; # reget argument
      fi # Check if have more dependencies
       
   fi # If: no lib in path
done

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec05:
#   - print mkfile (makefile)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# need to add in main file to object files list
objFilesStr="$objFilesStr \\\\\n$spaceStr$mainFileStr.\$O";

# need to complete compiler part of main file command
if [ $tclTkBl -eq 0 ];
then
   mainCmdStr="$mainCmdStr
			$ccStr ${dashOStr}$mainFileStr.\$O \\
				$cFlagsStr $coreFlagsStr $mainFileStr.c
"
elif [ "$osStr" = "win" ]; then
   headStr="$headStr\n\n$tclTkStr";
   ldStr="$ldStr \$(tclTkLib) ";
   mainCmdStr="$mainCmdStr
   		$ccStr \$(tclTkInclude) ${dashOStr}$mainFileStr.\$O \\
   			$cFlagsStr $coreFlagsStr $mainFileStr.c"
else
   ldStr="$tclTkStr";
   mainCmdStr="$mainCmdStr $includeStr
		$ccStr \$\$hPath ${dashOStr}$mainFileStr.\$O \\
			$cFlagsStr $coreFlagsStr $mainFileStr.c"
fi

{
   printf "%b\n" "$headStr"
   printf "%b\n\n" "$libPathStr"
   printf "%b\n\n" "$objFilesStr"
   printf "%b" "$linkStr"
   printf "\t%b %b\n\n" "$ldStr" "$ldFlagsStr"
   printf "%b\n" "$mainCmdStr"
   printf "%b\n\n" "$cmdStr"
   printf "%b\n\n" "$installStr"
   printf "%b\n" "$cleanStr"
} # print out make file
