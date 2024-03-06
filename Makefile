CC = cc
PREFIX = /usr/local/bin

CFLAGS += -std=c89 -O3 -static
MACCFLAGS += -O3

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
	sed '1s/#!usr\/bin\/Rscript/#!usr\/local\/bin\/Rscript/;' < graphAmpDepth.r > tmp.r;
	mv graphAmpDepth.r linuxGraphAmpDepth.r;
	mv tmp.r graphAmpDepth.r;
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
       freezeTb.sh\
     > tmp.sh;
	mv freezeTb.sh buildAmpConsLinux.sh;
	mv tmp.sh freezeTb.sh;


mac: macAmpDepth macTrimSam macSubsample macFqGetIds macTbAmr
	sed '1s/#!usr\/bin\/Rscript/#!usr\/local\/bin\/Rscript/;' < graphAmpDepth.r > tmp.r;
	mv graphAmpDepth.r linuxGraphAmpDepth.r;
	mv tmp.r graphAmpDepth.r;
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
       freezeTb.sh\
     > tmp.sh;
	mv freezeTb.sh buildAmpConsLinux.sh;
	mv tmp.sh freezeTb.sh;

macAmpDepth:
	make mac -C ./ampDepthSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macTrimSam:
	make mac -C ./trimSamSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macSubsample:
	make mac -C ./subsampleSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macFqGetIds:
	make mac -C ./fqGetIdsSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macTbAmr:
	make -C ./tbAmrSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

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
	cp graphAmpDepth.r $(PREFIX);
	chmod -R a+x $(PREFIX)/freezeTBScripts;
	chmod a+x $(PREFIX)/freezeTBScripts/*;
	chmod a+x $(PREFIX)/freezeTb.sh;
	chmod a+x $(PREFIX)/ivarConScript.sh;
	chmod a+x $(PREFIX)/ampDepth;
	chmod a+x $(PREFIX)/graphAmpDepth.r;
	mv linuxGraphAmpDepth.r graphAmpDepth.r || printf "";
	mv ivarConScriptLinux.sh ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh freezeTb.sh;
	rm freezeTBScripts/*.gawk;

# Currently nothing to clean up
clean:
	mv ivarConScriptLinux.sh ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh freezeTb.sh;
	rm freezeTBScripts/*.gawk;
