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
   tbSpoligoSource/kmerFind.c \
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

# Just in case the user trys this
mac:
	$(CC) $(CFLAGS) -o freezeTB $(SOURCE);
	make mac -C adjCoordsSrc;

static:
	$(CC) $(staticCFLAGS) -o freezeTB $(SOURCE);
	make -C adjCoordsSrc;

check:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);

debug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);
	gdb -x freezeTBSrc/bug-cmds-freezeTb.txt bugFreezeTb

macbug:
	$(CC) $(DEBUGCFLAGS) -o bugFreezeTb $(SOURCE);
	lldb --source freezeTBSrc/bug-cmds-freezeTb.txt bugFreezeTb

minimap: getminimap2 all
	make clean -C minimap2;
	make -C minimap2;

minimapneon: getminimap2 all
	make clean -C minimap2;
	ls minimap2 && make arm_neon=1 aarch64=1 -C minimap2 || printf "";

getminimap2:
	ls minimap2 || git clone https://github.com/lh3/minimap2;

install:
	mkdir $(PREFIX) || printf "";
	cp scripts/graphAmpDepth.r $(PREFIX);
	cp scripts/freezeTBGui.r $(PREFIX);
	cp freezeTB $(PREFIX);
	cp adjCoordsSrc/adjCoords "$(PREFIX)"
	mkdir $(dbPREFIX)/freezeTBFiles || printf "";
	cp freezeTBFiles/* $(dbPREFIX)/freezeTBFiles/;
	chmod -R a+x $(dbPREFIX)/freezeTBFiles;
	chmod a+rx $(dbPREFIX)/freezeTBFiles/*;
	chmod a+rx $(PREFIX)/freezeTB;
	chmod a+rx $(PREFIX)/adjCoords;
	chmod a+rx $(PREFIX)/freezeTBGui.r;
	chmod a+rx $(PREFIX)/graphAmpDepth.r;
	Rscript scripts/rDepends.r || printf "R is not on system\n";
	ls minimap2 1>/dev/null && sudo cp minimap2/minimap2 $(PREFIX)/minimap2 || printf "";
	ls minimap2 1>/dev/null && sudo chmod a+x $(PREFIX)/minimap2 || printf "";
	ls minimap2 1>/dev/null && make clean -C minimap2 || printf "";
	ls minimap2 1>/dev/null && sudo cp minimap2/minimap2.1 $(MANPREFIX) || printf "";

# commands for installing the modules

modules:
	make mac MACCFLAGS="$(CFLAGS)" -C adjCoordsSrc
	make mac MACCFLAGS="$(CFLAGS)" -C ampDepthSource
	make mac MACCFLAGS="$(CFLAGS)" -C cigToEqxCig
	make mac MACCFLAGS="$(CFLAGS)" -C filtSamSource
	make mac MACCFLAGS="$(CFLAGS)" -C primMaskSrc
	make mac MACCFLAGS="$(CFLAGS)" -C tbAmrSource
	make mac MACCFLAGS="$(CFLAGS)" -C tbAmrSource/swapDbRefSrc
	make mac MACCFLAGS="$(CFLAGS)" -C tbAmrSource/cnvt_whoToTbAmr_src
	make mac MACCFLAGS="$(CFLAGS)" -C tbConSource
	make mac MACCFLAGS="$(CFLAGS)" -C tbMiruSource
	make mac MACCFLAGS="$(CFLAGS)" -C tbMiruSource/mkMiruTbl
	make MACCFLAGS="$(CFLAGS)" -C tbSpoligoSource

staticmodules:
	make CFLAGS="$(staticCFLAGS)" -C adjCoordsSrc
	make CFLAGS="$(staticCFLAGS)" -C ampDepthSource
	make CFLAGS="$(staticCFLAGS)" -C cigToEqxCig
	make CFLAGS="$(staticCFLAGS)" -C filtSamSource
	make CFLAGS="$(staticCFLAGS)" -C primMaskSrc
	make CFLAGS="$(staticCFLAGS)" -C tbAmrSource
	make CFLAGS="$(staticCFLAGS)" -C tbAmrSource/swapDbRefSrc
	make CFLAGS="$(staticCFLAGS)" -C tbAmrSource/cnvt_whoToTbAmr_src
	make CFLAGS="$(staticCFLAGS)" -C tbConSource
	make CFLAGS="$(staticCFLAGS)" -C tbMiruSource
	make CFLAGS="$(staticCFLAGS)" -C tbMiruSource/mkMiruTbl
	make static CFLAGS="$(staticCFLAGS)" -C tbSpoligoSource


moduleinstall:
	cp scripts/catPrimers.sh "$(PREFIX)"
	cp tbMiruSource/getHitMiruTbl.awk "$(PREFIX)"
	cp tbMiruSource/getPercMiruTbl.awk "$(PREFIX)"
	cp adjCoordsSrc/adjCoords "$(PREFIX)"
	cp ampDepthSource/ampDepth "$(PREFIX)"
	cp cigToEqxCig/cigToEqxCig "$(PREFIX)"
	cp filtSamSource/filtsam "$(PREFIX)"
	cp primMaskSrc/primMask "$(PREFIX)"
	cp tbAmrSource/tbAmr "$(PREFIX)"
	cp tbAmrSource/swapDbRefSrc/swapDbRef "$(PREFIX)"
	cp tbAmrSource/cnvt_whoToTbAmr_src/whoToTbAmr "$(PREFIX)"
	cp tbConSource/tbCon "$(PREFIX)"
	cp tbMiruSource/tbMiru "$(PREFIX)"
	cp tbMiruSource/mkMiruTbl/mkMiruTbl "$(PREFIX)"
	cp tbSpoligoSource/tbSpoligo "$(PREFIX)"
	chmod a+x "$(PREFIX)/scripts/catPrimers.sh
	chmod a+x "$(PREFIX)/scripts/getMiruHitTbl.awk
	chmod a+x "$(PREFIX)/scripts/getPercMiruTbl.awk
	chmod a+x "$(PREFIX)/adjCoords"
	chmod a+x "$(PREFIX)/ampDepth"
	chmod a+x "$(PREFIX)/cigToEqxCig"
	chmod a+x "$(PREFIX)/filtsam"
	chmod a+x "$(PREFIX)/primMask"
	chmod a+x "$(PREFIX)/tbAmr"
	chmod a+x "$(PREFIX)/swapDbRef"
	chmod a+x "$(PREFIX)/whoToTbAmr"
	chmod a+x "$(PREFIX)/tbCon"
	chmod a+x "$(PREFIX)/tbMiru"
	chmod a+x "$(PREFIX)/mkMiruTbl"
	chmod a+x "$(PREFIX)/tbSpoligo"

# clean up all the binarries and minimap2s source files
clean:
	rm bugFreezeTb || printf "";
	rm -r -f minimap2 || printf "";
	rm freezeTB || printf "";
	rm adjCoords || printf "";
	rm adjCoordsSrc/adjCoords || printf "";
	rm ampDepthSource/ampDepth || printf "";
	rm cigToEqxCig/cigToEqxCig || printf "";
	rm filtSamSource/filtsam || printf "";
	rm primMaskSrc/primMask || printf "";
	rm tbAmrSource/tbAmr || printf "";
	rm tbAmrSource/swapDbRefSrc/swapDbRef || printf "";
	rm tbAmrSource/cnvt_whoToTbAmr_src/whoToTbAmr || printf "";
	rm tbConSource/tbCon || printf "";
	rm tbMiruSource/tbMiru || printf "";
	rm tbMiruSource/mkMiruTbl/mkMiruTbl || printf "";
	rm tbSpoligoSource/tbSpoligo || printf "";
