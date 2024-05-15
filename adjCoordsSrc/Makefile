CC=cc
PREFIX=/usr/local/bin

CFLAGS=-O3 -std=c89 -Wall -static
MACCFLAGS=-O3
BUGCFLAGS=-O0 -std=c89 -Wall -static -ggdb
BUGMACCFLAGS=-O0 -std=c89 -Wall -ggdb

SOURCE=\
   ../generalLib/geneCoordStruct.c \
   ../generalLib/samEntryStruct.c \
   adjCoordsFun.c \
   adjCoords.c

all:
	$(CC) $(CFLAGS) -o adjCoords $(SOURCE);

mac:
	$(CC) $(MACCFLAGS) -o adjCoords $(SOURCE);

check:
	$(CC) $(BUGCFLAGS) -o bugadjCoords $(SOURCE);

bug:
	$(CC) $(BUGCFLAGS) -o bugAdjCoords $(SOURCE);
	gdb -x bugcmds-adjCoords.txt bugAdjCoords;

macbug:
	$(CC) $(BUGMACCFLAGS) -o bugAdjCoords $(SOURCE);
	lldb -source bugcmds-adjCoords.txt bugAdjCoords;

install:
	cp adjCoords $(PREFIX);
	chmod a+xr $(PREFIX)/adjCoords;
