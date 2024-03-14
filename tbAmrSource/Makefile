CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 -std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall

SOURCE=\
   amrStruct.c\
   drug_str_ary.c\
   ../generalLib/samEntryStruct.c\
   checkAmr.c\
   tbAmr.c

all:
	$(CC) $(CFLAGS) -o tbAmr $(SOURCE)

mac:
	$(CC) $(MACCFLAGS) -o tbAmr $(SOURCE)

debug:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr $(SOURCE)
	gdb -x bug-cmds.txt debugTbAmr;

check:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr $(SOURCE)

buildWhoCnvt:
	make -C cnvt_whoToTbAmr_src CC=$(CC) CFLAGS=$(CFLAGS)

install: 
	cp tbAmr $(PREFIX);
	chmod a+x $(PREFIX)/tbAmr;
