PREFIX=/usr/local/bin
CC=cc
CFLAGS += -O3 -static -std=c89
MACCFLAGS += -O3 -static -std=c89
DEBUGFLAGS += -O0 -ggdb -Wall -static -std=c89

all:
	$(CC) $(CFLAGS) -o fastTrimSam oldTrimSamFile.c

install:
	mv fastTrimSam $(PREFIX);
	chmod a+x $(PREFIX)/fastTrimSam;
