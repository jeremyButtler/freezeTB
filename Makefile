CC = cc
PREFIX = /usr/local/bin
MANPREFIX = /usr/local/share/man/man1

# Location to install the database files
dbPREFIX = /usr/local/share

CFLAGS += -O3
staticCFLAGS += -std=c89 -O3 -static
DEBUGCFLAGS += -O0 -ggdb -std=c89 -Wall

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
   tbMiruSource/miruTblStruct.c \
   tbSpoligoSource/tbSpoligo-memwater-fun.c \
   tbSpoligoSource/tbSpoligo-readDb.c \
   primMaskSrc/primMask-fun.c \
   memwater/seqStruct.c \
   memwater/alnSetStruct.c \
   memwater/memwater.c \
   freezeTBSrc/freezeTb.c

# The sed step is to make sure the default file locations
# are set up. No other way to do this on C

all:
	$(CC) $(CFLAGS) -o freezeTB $(SOURCE);
	make mac -C adjCoordsSrc;
	mv adjCoordsSrc/adjCoords ./;

# Just in case the user trys this
mac:
	$(CC) $(CFLAGS) -o freezeTB $(SOURCE);
	make mac -C adjCoordsSrc;
	mv adjCoordsSrc/adjCoords ./;

static:
	$(CC) $(staticCFLAGS) -o freezeTB $(SOURCE);
	make -C adjCoordsSrc;
	mv adjCoordsSrc/adjCoords ./;

check:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);

debug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);
	gdb -x bug-cmds-freezeTb.txt bugFreezeTb

macbug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);
	lldb --source freezeTBSrc/bug-cmds-freezeTb.txt bugFreezeTb

mk: getminimap2 all
	make clean -C minimap2;
	make -C minimap2;

mkneon: getminimap2 all
	make clean -C minimap2;
	ls minimap2 && make arm_neon=1 aarch64=1 -C minimap2 || printf "";

getminimap2:
	ls minimap2 || git clone https://github.com/lh3/minimap2;

install:
	mkdir $(PREFIX) || printf "";
	cp scripts/graphAmpDepth.r $(PREFIX);
	cp scripts/freezeTBGui.r $(PREFIX);
	cp freezeTB $(PREFIX);
	cp adjCoords $(PREFIX);
	mkdir $(dbPREFIX)/freezeTBFiles || printf "";
	cp freezeTBFiles/* $(dbPREFIX)/freezeTBFiles/;
	chmod -R a+x $(dbPREFIX)/freezeTBFiles;
	chmod a+rx $(dbPREFIX)/freezeTBFiles/*;
	chmod a+rx $(PREFIX)/freezeTB;
	chmod a+rx $(PREFIX)/adjCoords;
	chmod a+rx $(PREFIX)/freezeTBGui.r;
	chmod a+rx $(PREFIX)/graphAmpDepth.r;
	Rscript scripts/rDepends.r || printf "R is not on system\n";
	ls minimap2 1>/dev/null && sudo cp minimap2/minimap2.1 $(MANPREFIX) || printf "";
	ls minimap2 1>/dev/null && sudo cp minimap2/minimap2 $(PREFIX)/minimap2 || printf "";
	ls minimap2 1>/dev/null && sudo chmod a+x $(PREFIX)/minimap2 || printf "";
	ls minimap2 1>/dev/null && make clean -C minimap2 || printf "";

# Currently nothing to clean up
clean:
	rm bugFreezeTb || printf "";
	rm -r -f minimap2 || printf "";
