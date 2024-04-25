CC=cc
PREFIX=/usr/local/bin

CFLAGS=-O3 -static -std=c89
MACCFLAGS=-O3
DEBUGFLAGS=-Wall -ggdb -std=c89 -O0 -static

variantFlags=-DNOGAPEXTEND

SOURCE=\
   ../../memwater/alnSetStruct.c\
   ../../memwater/seqStruct.c\
   ../../memwater/memwater.c\
   mkMiruTbl.c

all:
	$(CC) $(CFLAGS) $(variantFlags) -o mkMiruTbl $(SOURCE);

mac:
	$(CC) $(MACCFLAGS) $(variantFlags) -o mkMiruTbl $(SOURCE);

debug:
	$(CC) $(DEBUGFLAGS) $(variantFlags) -o debug_mkMiruTbl $(SOURCE);
	gdb -x bug-cmds-mkMiruTbl.txt debug_mkMiruTbl;

check:
	$(CC) $(DEBUGFLAGS) $(variantFLags) -o debug_mkMiruTbl $(SOURCE);

install:
	sudo cp mkMiruTbl $(PREFIX);
	sudo chmod a+x $(PREFIX)/mkMiruTbl;
