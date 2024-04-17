CC = cc
PREFIX = /usr/local/bin
MANPREFIX = /usr/local/share/man/man1

# Location to install the database files
dbPREFIX = /usr/local/share

CFLAGS += -O3
staticCFLAGS += -std=c89 -O3 -static
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
   tbMiruSource/miruTblStruct.c \
   freezeTb.c

# The sed step is to make sure the default file locations
# are set up. No other way to do this on C

all:
	$(CC) $(CFLAGS) -o freezeTb $(SOURCE);

# Just in case the user trys this
mac:
	$(CC) $(CFLAGS) -o freezeTb $(SOURCE);

static:
	$(CC) $(staticCFLAGS) -o freezeTb $(SOURCE);

check:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);

debug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);
	gdb -x bug-cmds-freezeTb.txt bugFreezeTb

mkminimap2: getminimap2
	make clean -C minimap2;
	make -C minimap2;

mkNeonMinimap2: getminimap2
	make clean -C minimap2;
	ls minimap2 && make arm_neon=1 -C minimap2 || printf "";

getminimap2:
	ls minimap2 || git clone https://github.com/lh3/minimap2;

install:
	cp graphAmpDepth.r $(PREFIX);
	cp freezeTb $(PREFIX);
	cp freezeTbGui.r $(PREFIX);
	cp -r freezeTbFiles $(dbPREFIX);
	chmod -R a+x $(dbPREFIX)/freezeTbFiles;
	chmod a+x $(dbPREFIX)/freezeTbFiles/*;
	chmod a+x $(PREFIX)/freezeTb;
	chmod a+x freezeTbGui.r $(PREFIX);
	chmod a+x $(PREFIX)/graphAmpDepth.r;
	Rscript rDepends.r;
	minimap2 --version || sudo cp $(PREIFX);
	minimap2 --version || sudo chmod a+x $(PREFIX)/minimap2;
	minimap2 --version || sudo cp minimap2/minimap2.1 $(MANPREFIX);
	make clean -C minimap2

# Currently nothing to clean up
clean:
	rm bugFreezeTb || printf "";
