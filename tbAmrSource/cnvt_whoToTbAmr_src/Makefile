CC=cc
PREFIX=/usr/local/bin
CFLAGS= -static -O3 -std=c89
MACCFLAGS= -O3
DEBUGFLAGS= -static -O0 -ggdb -Wall

cFiles=\
   whoToTbAmr.c\
   cnvt_whoToTbAmr.c\
   ../../generalLib/samEntryStruct.c\
   genIndiceStruct.c\
   ../drug_str_ary.c\
   ../amrStruct.c


all:
	$(CC) $(CFLAGS) -o whoToTbAmr $(cFiles)

debug:
	$(CC) $(DEBUGFLAGS) -o debugWhoToTbAmr $(cFiles)
	gdb -x bug-cmds.txt debugWhoToTbAmr

mac:
	$(CC) $(MACCFLAGS) -o whoToTbAmr $(cFiles)

# Not really worth installing, but just in case
install: 
	cp whoToTbAmr $(PREFIX);
	chmod a+x $(PREFIX)/whoToTbAmr;
