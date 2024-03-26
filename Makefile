CC = cc
PREFIX = /usr/local/bin

CFLAGS += -std=c89 -O3 -static
MACCFLAGS += -O3
DEBUGCFLAGS += -O0 -ggdb -static -std=c89 -Wall

# There are hidden .h only files. So, this is not
# the actual full list of source files
SOURCE=\
   tbConSource/tbCon-fun.c \
   tbAmrSource/checkAmr.c \
   ampDepthSource/ampDepth-fun.c \
   generalLib/trimSam.c \
   generalLib/samEntryStruct.c \
   generalLib/geneCoordStruct.c \
   tbAmrSource/drug_str_ary.c \
   tbAmrSource/amrStruct.c \
   tbMiruSource/miruTblStruct.c

# The sed step is to make sure the default file locations
# are set up. No other way to do this on C

all: setUpPath
	$(CC) $(CFLAGS) -o freezeTb $(SOURCE) buildFreezeTb.c;

mac: altRScriptPath setUpPath
	$(CC) $(MACCFLAGS) -o freezeTb $(SOURCE) buildFreezeTb.c;

openbsd: altRScriptPath setUpPath
	$(CC) $(CFLAGS) -o freezeTb $(SOURCE) buildFreezeTb.c;

check:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE) freezeTb.c;

debug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE) freezeTb.c;
	gdb -x bug-cmds-freezeTb.txt bugFreezeTb

# This is to set up the path to freezeTBFiles
setUpPath:
	awk -v pathStr=$(PREFIX) 'BEGIN{pathStr = "char *defPathStr = \"" pathStr "\";"}; {if($$0 ~ /char \*defPathStr/) print pathStr; else print $$0;}' < freezeTb.c > buildFreezeTb.c;

altRScriptPath:
	sed '1s/#!usr\/bin\/Rscript/#!usr\/local\/bin\/Rscript/;' < graphAmpDepth.r > tmp.r;
	mv graphAmpDepth.r linuxGraphAmpDepth.r;
	mv tmp.r graphAmpDepth.r;

R:
	Rscript rDepends.r

install:
	cp graphAmpDepth.r $(PREFIX);
	cp -r freezeTBFiles $(PREFIX);
	mv freezeTb $(PREFIX);
	chmod -R a+x $(PREFIX)/freezeTBScripts;
	chmod a+x $(PREFIX)/freezeTBScripts/*;
	chmod a+x $(PREFIX)/freezeTb;
	chmod a+x $(PREFIX)/graphAmpDepth.r;
	mv linuxGraphAmpDepth.r graphAmpDepth.r || printf "";

# Currently nothing to clean up
clean:
	rm bugFreezeTb || printf "";
	rm buildFreezeTb.c || printf "":
	mv linuxGraphAmpDepth.r graphAmpDepth.r || printf "";
