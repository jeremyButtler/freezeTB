PREFIX=/usr/local/bin
CC=cc
CFLAGS += -O3 -static -std=c99 -Wno-unused-fuction
MACCFLAGS += -O3 -static -std=c99

all:
	$(CC) $(CFLAGS) -o trimSam trimSamFile.c
mac:
	$(CC) $(MACCFLAGS) -o trimSam trimSamFile.c

install:
	mv trimSam subsampleIds fqGetIds ampDepth $(PREFIX);
	chmod a+x $(PREFIX)/trimSam;
