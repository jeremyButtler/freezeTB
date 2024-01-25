CC = cc
PREFIX=/usr/local/bin

CFLAGS += -static -O3 -Wno-unused-function

# For MACS (do not lie -Wno-unused-function
MACFLAGS += -static -O3

# Debuggin
BUGFLAGS += -ggdb -Wall -O0 -static -o debugAmpDepth

# Path to Rscript

all:
	$(CC) $(CFLAGS) -o ampDepth ampDepth.c

mac:
	$(CC) $(MACFLAGS) -o ampDepth ampDepth.c

debug:
	$(CC) $(BUGFLAGS) -o debugAmpDepth ampDepth.c
	#gdb -ex "b ampDepth.c:68" -ex "r -paf ../old.paf -sam ../test.sam" debugAmpDepth

install:
	cp ampDepth $(PREFIX);
	cp graphAmpDepth $(PREFIX);
	chmod a+x $(PREFIX)/ampDepth;
	chmod a+x $(PREFIX)/graphAmpDepth;
