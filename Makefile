CC = cc
CFLAGS += -std=c99 -O3 -static -Wall -Wno-unused-function

all: ampCountsCode trimSamCode subsampleCode fqGetIdsCode

ampCountsCode:
	make -C ./ampMapSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./ampMapSource/ampMapCounts ./ampMapScripts/ampMap;

trimSamCode:
	make -C ./trimSamSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./trimSamSource/trimSam ./ampMapScripts/;

subsampleCode:
	make -C ./subsampleSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./subsampleSource/subsampleIds ./ampMapScripts/;

fqGetIdsCode:
	make -C ./fqGetIdsSource CC="$(CC)" CFLAGS="$(CFLAGS)";
	mv ./fqGetIdsSource/fqGetIds ./ampMapScripts/;

R:
	Rscript rDepends.r

install:
	cp -r ampMapScripts /usr/local/bin;
	cp buildAmpCons.sh /usr/local/bin;
	chmod -R a+x /usr/local/bin/ampMapScripts;
	chmod a+x /usr/local/bin/ampMapScripts/*;
	chmod a+x /usr/local/bin/buildAmpCons.sh;

# Currently nothing to clean up
clean: