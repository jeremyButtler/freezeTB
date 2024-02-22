CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall -Wno-unused-function

all:
	$(CC) $(CFLAGS) -o filtSam filtSam.c;

mac:
	$(CC) $(MACCFLAGS) -o filtSam filtSam.c;

sse:
	$(CC) $(CFLAGS) -msse4 -DSSE4 -o filtSam filtSam.c;

avx2:
	$(CC) $(CFLAGS) -mavx2 -DAVX2 -o filtSam filtSam.c;

avx512:
	$(CC) $(CFLAGS) -mavx512bw -DAVX512 -o filtSam filtSam.c;

noulcp:
	$(CC) $(CFLAGS) -DNO_UL_CP -o filtSam filtSam.c;

debug:
	$(CC) $(DEBUGFLAGS) -mavx512bw -DAVX512 -o debugFiltSam filtSam.c;
	gdb -x bug-cmds.txt debugFiltSam;

check:
	$(CC) $(DEBUGFLAGS) -o debugFiltSam filtSam.c;
