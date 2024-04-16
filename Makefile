CC = cc
PREFIX = /usr/local/bin

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

# Currently nothing to clean up
clean:
	rm bugFreezeTb || printf "";
