CC = cc
PREFIX = /usr/local/bin

CFLAGS += -std=c89 -O3 -static
MAC_CFLAGS += -O3

all: buildAmpDepth buildTrimSam buildSubsample buildFqGetIds buildTbAmr

buildAmpDepth:
	make -C ./ampDepthSource CC="$(CC)" CFLAGS="$(CFLAGS)";

buildTrimSam:
	make -C ./trimSamSource CC="$(CC)" CFLAGS="$(CFLAGS)";


buildSubsample:
	make -C ./subsampleSource CC="$(CC)" CFLAGS="$(CFLAGS)";

buildFqGetIds:
	make -C ./fqGetIdsSource CC="$(CC)" CFLAGS="$(CFLAGS)";

buildTbAmr:
	make -C ./tbAmrSource CC="$(CC)" CFLAGS="$(CFLAGS)";

openbsd: buildAmpDepth buildTrimSam buildSubsample buildFqGetIds buildTbAmr
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/extractPrimRead.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/extractPrimRead.gawk;
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/getMapCoord.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/getMapCoord.gawk;
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/trimCon.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/trimCon.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/local\/bin\/bash/'\
      ivarConScript.sh\
     > tmp.sh;
	mv ivarConScript.sh ivarConScriptLinux.sh;
	mv tmp.sh ivarConScript.sh;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/local\/bin\/bash/; s/awk/gawk/g;'\
       buildAmpCons.sh\
     > tmp.sh;
	mv buildAmpCons.sh buildAmpConsLinux.sh;
	mv tmp.sh buildAmpCons.sh;


mac: macAmpDepth macTrimSam macSubsample macFqGetIds macTbAmr
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/extractPrimRead.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/extractPrimRead.gawk;
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/getMapCoord.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/getMapCoord.gawk;
	sed\
      '1s/#!\/usr\/bin\/awk/#!\/usr\/bin\/gawk/'\
       freezeTBScripts/trimCon.awk\
     > tmp.gawk;
	mv tmp.gawk freezeTBScripts/trimCon.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/bin\/bash/'\
      ivarConScript.sh\
     > tmp.sh;
	mv ivarConScript.sh ivarConScriptLinux.sh;
	mv tmp.sh ivarConScript.sh;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/bin\/bash/; s/awk/gawk/g;'\
       buildAmpCons.sh\
     > tmp.sh;
	mv buildAmpCons.sh buildAmpConsLinux.sh;
	mv tmp.sh buildAmpCons.sh;

macAmpDepth:
	make mac -C ./ampDepthSource CC="$(CC)" MAC_CFLAGS="$(MAC_CFLAGS)";

macTrimSam:
	make mac -C ./trimSamSource CC="$(CC)" MAC_CFLAGS="$(MAC_CFLAGS)";

macSubsample:
	make mac -C ./subsampleSource CC="$(CC)" MAC_CFLAGS="$(MAC_CFLAGS)";

macFqGetIds:
	make mac -C ./fqGetIdsSource CC="$(CC)" MAC_CFLAGS="$(MAC_CFLAGS)";

macTbAmr:
	make -C ./tbAmrSource CC="$(CC)" MAC_CFLAGS="$(MAC_CFLAGS)";

R:
	Rscript rDepends.r

install:
	make -C ampDepthSource PREFIX=$(PREFIX) install;
	make -C trimSamSource PREFIX=$(PREFIX) install;
	make -C fqGetIdsSource PREFIX=$(PREFIX) install;
	make -C subsampleSource PREFIX=$(PREFIX) install;
	make -C tbAmrSource PREFIX=$(PREFIX) install;
	cp -r freezeTBScripts $(PREFIX);
	cp -r tbAmrSource/WHO-TB-catalog-genomeIndicies.csv $(PREFIX)/freezeTBScripts;
	cp freezeTB.sh $(PREFIX);
	cp ivarConScript.sh $(PREFIX);
	chmod -R a+x $(PREFIX)/freezeTBScripts;
	chmod a+x $(PREFIX)/freezeTBScripts/*;
	chmod a+x $(PREFIX)/buildAmpCons.sh;
	chmod a+x $(PREFIX)/ivarConScript.sh;
	mv ampDepthSource/linuxGraphAmpDepth.r ampDepthSource/graphAmpDepth.r || exit;
	mv ivarConScriptLinux.sh ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh buildAmpCons.sh;
	rm freezeTBScripts/*.gawk;

# Currently nothing to clean up
clean:
	mv ivarConScriptLinux.sh ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh buildAmpCons.sh;
	rm freezeTBScripts/*.gawk;
