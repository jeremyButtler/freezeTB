CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 -std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall

tbAmrSource=\
   amrStruct.c\
   drug_str_ary.c\
   ../generalLib/samEntryStruct.c\
   checkAmr.c\
   tbAmr.c

all:
	$(CC) $(CFLAGS) -o tbAmr $(tbAmrSource)

mac:
	$(CC) $(MACCFLAGS) -o tbAmr $(tbAmrSource)

debug:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr $(tbAmrSource)
	gdb -x bug-cmds.txt debugTbAmr;

check:
	$(CC) $(DEBUGFLAGS) -o debugTbAmr tbAmr.c;

buildWhoCnvt:
	make -C cnvt_whoToTbAmr_src CC=$(CC) CFLAGS=$(CFLAGS)

install: 
	cp tbAmr $(PREFIX);
	chmod a+x $(PREFIX)/tbAmr;
