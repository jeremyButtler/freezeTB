CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 -Wno-unused-function -std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall -Wno-unused-function

all:
	$(CC) $(CFLAGS) -o tbAmr tbAmr.c;

mac:
	$(CC) $(MACCFLAGS) -o tbAmr tbAmr.c;

debug:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr tbAmr.c;
	gdb -x bug-cmds.txt debugTbAmr;

check:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr tbAmr.c;

install: 
	cp tbAmr $(PREFIX);
	chmod a+x $(PREFIX)/tbAmr;
