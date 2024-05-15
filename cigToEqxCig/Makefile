CC=cc
PREFIX=/usr/local/bin
CFLAGS=-O3 -Wall -std=c89 -static
MACCFLAGS=-O3 -Wall
BUGCFLAGS=-O0 -Wall -std=c89 -static -ggdb

SOURCE= \
   ../memwater/seqStruct.c \
   ../generalLib/samEntryStruct.c \
   cigToEqxCig-fun.c \
   cigToEqxCig.c

all:
	$(CC) $(CFLAGS) -o cigToEqxCig $(SOURCE);

mac:
	$(CC) $(MACCFLAGS) -o cigToEqxCig $(SOURCE);

check:
	$(CC) $(BUGCFLAGS) -o bugCigToEqxCig $(SOURCE);

bug:
	$(CC) $(BUGCFLAGS) -o bugCigToEqxCig $(SOURCE);
	gdb -x bugcmds-cigToEqxCig.txt bugCigToEqxCig;
	rm bugCigToEqxCig; 

install:
	cp cigToEqxCig $(PREFIX);
	chmod a+xr $(PREFIX)/cigToEqxCig;
