CC=cc
CFLAGS += -O3 -static -std=c99 -Wno-unused-function
MACCFLAGS += -O3 -static -std=c99

all:
	$(CC) $(CFLAGS) -o subsampleIds subsampleIds.c

mac:
	$(CC) $(MACCFLAGS) -o subsampleIds subsampleIds.c

debug:
	$(CC) -O0 -static -std=c99 -ggdb -Wall -o test.o subsampleIds.c
