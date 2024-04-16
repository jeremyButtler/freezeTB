CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 --std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall

source=\
   ../generalLib/samEntryStruct.c\
   ../generalLib/trimSam.c\
   filtsam.c

all:
	$(CC) $(CFLAGS) -o filtsam $(source);

mac:
	$(CC) $(MACCFLAGS) -o filtsam $(source);

debug:
	$(CC) $(DEBUGFLAGS) -o debugFiltsam $(source);
	gdb -x bug-cmds.txt debugFiltsam;

check:
	$(CC) $(DEBUGFLAGS) -o debugFiltsam $(source);
