CC = cc
PREFIX = /usr/local/bin

CFLAGS += -std=c99 -O3 -static -Wall -Wno-unused-function
MACFLAGS += -std=c99 -O3 -static -Wall

all: buildAmpDepth buildTrimSam buildSubsample buildFqGetIds

buildAmpDepth:
	make -C ./ampDepthSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./ampDepthSource/ampDepth ./;

buildTrimSam:
	make -C ./trimSamSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./trimSamSource/trimSam ./;


buildSubsample:
	make -C ./subsampleSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./subsampleSource/subsampleIds ./;

buildFqGetIds:
	make -C ./fqGetIdsSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./fqGetIdsSource/fqGetIds ./;


mac: macAmpDepth macTrimSam macSubsample macFqGetIds
	sed\
      '1s/awk/gawk/'\
       ampMapScripts/extractPrimRead.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/extractPrimRead.gawk;
	sed\
      '1s/awk/gawk/'\
       ampMapScripts/getMapCoord.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/getMapCoord.gawk;
	sed\
      '1s/awk/gawk/'\
       ampMapScripts/trimCon.awk\
     > tmp.gawk;
	mv tmp.awk ampMapScripts/trimCon.gawk;
	sed\
      '1s/\/usr//'\
      ivarConScript.sh\
     > tmp.sh;
	mv ivarConScript.sh ivarConScriptLinux.sh;
	mv tmp.sh ivarConScript.sh;
	sed\
      '1s/\/usr//; s/awk/gawk/g;'\
       buildAmpCons.sh\
     > tmp.sh;
	mv buildAmpCons.sh buildAmpConsLinux.sh;
	mv tmp.sh buildAmpCons.sh;

macAmpDepth:
	make mac -C ./ampDepthSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";
	mv ./ampDepthSource/ampDepth ./;

macTrimSam:
	make mac -C ./trimSamSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";
	mv ./trimSamSource/trimSam ./;

macSubsample:
	make mac -C ./subsampleSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";
	mv ./subsampleSource/subsampleIds ./;

macFqGetIds:
	make mac -C ./fqGetIdsSource CC="$(CC)" MACCFLAGS="$(MACCFLAGS)";
	mv ./fqGetIdsSource/fqGetIds ./;

R:
	Rscript rDepends.r

install:
	cp -r ampMapScripts $(PREFIX);
	cp buildAmpCons.sh $(PREFIX);
	cp trimSam subsampleIds fqGetIds ampDepth $(PREFIX);
	cp ivarConScript.sh $(PREFIX);
	chmod -R a+x $(PREFIX)/ampMapScripts;
	chmod a+x $(PREFIX)/ampMapScripts/*;
	chmod a+x $(PREFIX)/buildAmpCons.sh;
	chmod a+x $(PREFIX)/trimSam;
	chmod a+x $(PREFIX)/subsampleIds;
	chmod a+x $(PREFIX)/fqGetIds;
	chmod a+x $(PREFIX)/ampDepth;
	chmod a+x $(PREFIX)/ivarConScript.sh;

# Currently nothing to clean up
clean:
	# Handle mac install case
	mv\

# Currently nothing to clean up
clean:
	# Handle mac install case
	mv\
       ampMapScripts/ivarConScriptLinux.sh\
       ampMapScripts/ivarConScript.sh || exit;
	mv buildAmpConsLinux.sh buildAmpCons.sh;
	rm ampMApScripts/*.gawk;
