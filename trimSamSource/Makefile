PREFIX=/usr/local/bin
CC=cc
CFLAGS += -O3 -static -std=c89
MAC_CFLAGS += -O3 
DEBUGFLAGS += -O0 -ggdb -Wall -static -std=c89

all:
	$(CC) $(CFLAGS) -o trimSam trimSamFile.c
mac:
	$(CC) $(MAC_CFLAGS) -o trimSam trimSamFile.c
check:
	$(CC) $(CFLAGS) -Wall -o trimSam trimSamFile.c
debug:
	$(CC) $(DEBUGFLAGS) -o bug-trimSam trimSamFile.c
	gdb -x bug-cmds.txt bug-trimSam

install:
	mv trimSam $(PREFIX);
	chmod a+x $(PREFIX)/trimSam;
