CC=cc
PREFIX=/usr/local/bin

CFLAGS=-O3 -Wall -std=c89
STATICFLAGs=-O3 -static -Wall -std=c89
DEBUGFLAGS=-O0 -Wall -std=c89 -ggdb

varFlags=-DNOGAPEXTEND

source=\
   ../generalLib/samEntryStruct.c\
   ../memwater/alnSetStruct.c\
   ../memwater/seqStruct.c\
   ../memwater/memwater.c\
   tbSpoligo-memwater-fun.c\
   tbSpoligo-readDb.c\
   tbSpoligo-version.c\
   tbSpoligo-checkInput.c\
   tbSpoligo.c

all:
	$(CC) $(CFLAGS) $(varFlags) -o tbSpoligo $(source);

static:
	$(CC) $(STATICFLAGS) $(varFlags) -o tbSpoligo $(source);

debug:
	$(CC) $(DEBUGFLAGS) $(varFlags) -o bug-tbSpoligo $(source);
	gdb -x bug-cmds.txt bug-tbSpoligo;

check:
	$(CC) $(DEBUGFLAGS) $(varFlags) -o bug-tbSpoligo $(source);
