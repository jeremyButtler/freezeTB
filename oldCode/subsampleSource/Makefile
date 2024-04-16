PREFIX=/usr/local/bin
CC=cc
CFLAGS += -O3 -static -std=c89
MACCFLAGS += -O3

all:
	$(CC) $(CFLAGS) -o subsampleIds subsampleIds.c

mac:
	$(CC) $(MACCFLAGS) -o subsampleIds subsampleIds.c

debug:
	$(CC) -O0 -static -std=c99 -ggdb -Wall -o test.o subsampleIds.c
	gdb -x bug-cmds.txt test.o

install:
	mv subsampleIds $(PREFIX);
	chmod a+x $(PREFIX)/subsampleIds;
