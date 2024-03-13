CC = cc
PREFIX=/usr/local/bin

CFLAGS += -static -O3 --std=c89

MACCFLAGS += -O3

# Debuggin
BUGFLAGS += -ggdb -Wall -O0 -static -o debugAmpDepth

source=\
   ../generalLib/samEntryStruct.c\
   ../generalLib/trimSam.c\
   ../generalLib/geneCoordStruct.c\
   ampDepth.c

all:
	$(CC) $(CFLAGS) -o ampDepth $(source);

mac:
	$(CC) $(MACCFLAGS) -o ampDepth $(source);

debug:
	$(CC) $(BUGFLAGS) -o debugAmpDepth $(source);
	gdb -x bug-cmds.txt debugAmpDepth

install:
	mv ampDepth $(PREFIX);
	chmod a+x $(PREFIX)/ampDepth;
