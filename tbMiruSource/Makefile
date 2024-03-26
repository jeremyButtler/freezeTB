CC=cc

CFLAGS=-O3 -std=c89 -static
MACCFLAGS=-O3
DEBUGFLAGS=-Wall -ggdb -std=c89 -O0 -static

SOURCE=\
   ../generalLib/samEntryStruct.c\
   miruTblStruct.c\
   tbMiru.c

all:
	$(CC) $(CFLAGS) -o tbMiru $(SOURCE)

debug:
	$(CC) $(DEBUGFLAGS) -o bugTbMiru $(SOURCE)
	gdb -x bug-tbMiru-cmds.txt bugTbMiru

check:
	$(CC) $(DEBUGFLAGS) -o bugTbMiru $(SOURCE)

mac:
	$(CC) $(MACCFLAGS) -o tbMiru $(SOURCE)
