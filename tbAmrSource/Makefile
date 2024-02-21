CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 -Wno-unused-function -std=c89
MAC_CFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall -Wno-unused-function

all:
	$(CC) $(CFLAGS) -o tbAmr tbAMR.c;

mac:
	$(CC) $(MAC_CFLAGS) -o tbAmr tbAMR.c;

debug:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr tbAMR.c;
	gdb -x bug-cmds.txt debugTbAmr;

check:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr tbAMR.c;

install: 
	cp tbAmr $(PREFIX);
	chmod a+x $(PREFIX)/tbAmr;
