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
	sed '1s/#!usr\/bin\/Rscript/#!usr\/local\/bin\/Rscript/;' < graphAmpDepth.r > tmp.r;
	mv graphAmpDepth.r linuxGraphAmpDepth.r;
	mv tmp.r graphAmpDepth.r;
	$(CC) $(MACCFLAGS) -o ampDepth ampDepth.c

debug:
	$(CC) $(BUGFLAGS) -o debugAmpDepth ampDepth.c
	gdb -x bug-cmds.txt debugAmpDepth

install:
	cp ampDepth $(PREFIX);
	cp graphAmpDepth.r $(PREFIX);
	chmod a+x $(PREFIX)/ampDepth;
	chmod a+x $(PREFIX)/graphAmpDepth.r;
	mv linuxGraphAmpDepth.r graphAmpDepth.r || printf "";
