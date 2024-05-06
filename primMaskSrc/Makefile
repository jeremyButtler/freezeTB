CC=\
   cc

PREFIX=\
   /usr/local/bin

CFLAGS=\
   -O3\
   -std=c89\
   -Wall\
   -static

MACCFLAGS=\
   -O3

DEBUGFLAGS=\
   -O0\
   -std=c89\
   -Wall\
   -static\
   -ggdb

SOURCE=\
   ../generalLib/samEntryStruct.c\
   primMask-fun.c\
   primMask-input.c\
   primMask.c

all:
	$(CC) $(CFLAGS) -o primMask $(SOURCE);

mac:
	$(CC) $(MACCFLAGS) -o primMask $(SOURCE);

debug:
	$(CC) $(DEBUGFLAGS) -o bugPrimMask $(SOURCE);
	gdb -x bugcmds-primmask.txt bugPrimMask;

check:
	$(CC) $(DEBUGFLAGS) -o bugPrimMask $(SOURCE);
