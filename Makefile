CC = cc
PREFIX = /usr/local/bin

CFLAGS += -std=c99 -O3 -static -Wall -Wno-unused-function
MACFLAGS += -std=c99 -O3 -static -Wall

all: buildAmpDepth buildTrimSam buildSubsample buildFqGetIds

buildAmpDepth:
	make -C ./ampDepthSource CC="$(CC)" CFLAGS="$(CFLAGS)";

buildTrimSam:
	make -C ./trimSamSource CC="$(CC)" CFLAGS="$(CFLAGS)";


buildSubsample:
	make -C ./subsampleSource CC="$(CC)" CFLAGS="$(CFLAGS)";

buildFqGetIds:
	make -C ./fqGetIdsSource CC="$(CC)" CFLAGS="$(CFLAGS)";


openbsd: buildAmpDepth buildTrimSam buildSubsample buildFqGetIds
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/local\/bin\/gawk/'\
       ampMapScripts/extractPrimRead.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/extractPrimRead.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/local\/bin\/gawk/'\
       ampMapScripts/getMapCoord.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/getMapCoord.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/local\/bin\/gawk/'\
       ampMapScripts/trimCon.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/trimCon.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/local\/bin\/bash/'\
      ivarConScript.sh\
     > tmp.sh;
	mv ivarConScript.sh ivarConScriptLinux.sh;
	mv tmp.sh ivarConScript.sh;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/local\/bin\/bash/;
       s/awk/gawk/g;
      '\
       buildAmpCons.sh\
     > tmp.sh;
	mv buildAmpCons.sh buildAmpConsLinux.sh;
	mv tmp.sh buildAmpCons.sh;


mac: macAmpDepth macTrimSam macSubsample macFqGetIds
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/bin\/gawk/'\
       ampMapScripts/extractPrimRead.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/extractPrimRead.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/bin\/gawk/'\
       ampMapScripts/getMapCoord.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/getMapCoord.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/bin\/gawk/'\
       ampMapScripts/trimCon.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/trimCon.gawk;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/bin\/bash/'\
      ivarConScript.sh\
     > tmp.sh;
	mv ivarConScript.sh ivarConScriptLinux.sh;
	mv tmp.sh ivarConScript.sh;
	sed\
      '1s/#!\/usr\/bin\/bash/#!\/usr\/bin\/bash/;
       s/awk/gawk/g;
      '\
       buildAmpCons.sh\
     > tmp.sh;
	mv buildAmpCons.sh buildAmpConsLinux.sh;
	mv tmp.sh buildAmpCons.sh;

macAmpDepth:
	make mac -C ./ampDepthSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macTrimSam:
	make mac -C ./trimSamSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macSubsample:
	make mac -C ./subsampleSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

macFqGetIds:
	make mac -C ./fqGetIdsSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";

R:
	Rscript rDepends.r

install:
	make -C ampDepthSource PREFIX=$(PREFIX) install;
	make -C trimSamSouce PREFIX=$(PREFIX) install;
	make -C fqGetIdsSouce PREFIX=$(PREFIX) install;
	make -C subsampleSource PREFIX=$(PREFIX) install;
	cp -r ampMapScripts $(PREFIX);
	mv buildAmpCons.sh $(PREFIX);
	mv ivarConScript.sh $(PREFIX);
	chmod -R a+x $(PREFIX)/ampMapScripts;
	chmod a+x $(PREFIX)/ampMapScripts/*;
	chmod a+x $(PREFIX)/buildAmpCons.sh;
	chmod a+x $(PREFIX)/ivarConScript.sh;
	mv ampDepthSource/linuxGraphAmpDepth.r ampDepthSource/graphAmpDepth.r || exit;
	mv\
       ampMapScripts/ivarConScriptLinux.sh\
       ampMapScripts/ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh buildAmpCons.sh;
	rm ampMApScripts/*.gawk;

# Currently nothing to clean up
clean:
