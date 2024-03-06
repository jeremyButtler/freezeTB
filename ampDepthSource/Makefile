CC = cc
PREFIX=/usr/local/bin

CFLAGS += -static -O3 -Wno-unused-function

# For MACS (do not lie -Wno-unused-function
MACCFLAGS += -O3

# Debuggin
BUGFLAGS += -ggdb -Wall -O0 -static -o debugAmpDepth

# Path to Rscript

all:
	$(CC) $(CFLAGS) -o ampDepth ampDepth.c

mac:
	$(CC) $(MACCFLAGS) -o ampDepth ampDepth.c

debug:
	$(CC) $(BUGFLAGS) -o debugAmpDepth ampDepth.c
	gdb -x bug-cmds.txt debugAmpDepth

install:
	mv ampDepth $(PREFIX);
	chmod a+x $(PREFIX)/ampDepth;
