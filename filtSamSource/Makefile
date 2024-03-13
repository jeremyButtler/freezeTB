CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 --std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall

source=\
   ../generalLib/samEntryStruct.c\
   filtSam.c

all:
	$(CC) $(CFLAGS) -o filtSam $(source);

mac:
	$(CC) $(MACCFLAGS) -o filtSam $(source);

debug:
	$(CC) $(DEBUGFLAGS) -mavx512bw -DAVX512 -o debugFiltSam $(source);
	gdb -x bug-cmds.txt debugFiltSam;

check:
	$(CC) $(DEBUGFLAGS) -o debugFiltSam $(source);
