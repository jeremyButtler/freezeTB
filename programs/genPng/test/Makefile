LD=cc
CFLAGS= -O0 -std=c89 -g -Werror -Wall -Wextra -Wpedantic -Wundef -Waggregate-return -Wwrite-strings -Wstrict-prototypes -Wpointer-arith -Wshadow -Wunreachable-code -Winit-self
O=o.test

build: testMkPng fontLoadTest fontHorzDrawTest fontVertDrawTest normalFontToC alphbetTest smallFontToC normalFromCTest smallFromCTest

testMkPng: testMkPng.$O
	$(LD) -o testMkPng ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngDraw.$O ../../genFont/fontST.$O testMkPng.$O
testMkPng.$O:	testMkPng.c ../pngDraw.$O ../../genLib/endLine.h
	$(CC) -c -o testMkPng.$O $(CFLAGS) testMkPng.c

fontLoadTest: fontLoadTest.$O
	$(LD) -o fontLoadTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../../genFont/fontST.$O fontLoadTest.$O
fontLoadTest.$O:	fontLoadTest.c ../../genFont/fontST.$O
	$(CC) -c -o fontLoadTest.$O $(CFLAGS) fontLoadTest.c

fontHorzDrawTest: fontHorzDrawTest.$O
	$(LD) -o fontHorzDrawTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../pngDraw.$O ../mkPng.$O ../../genFont/fontST.$O fontHorzDrawTest.$O
fontHorzDrawTest.$O:	fontHorzDrawTest.c ../pngDraw.$O
	$(CC) -c -o fontHorzDrawTest.$O $(CFLAGS) fontHorzDrawTest.c

fontVertDrawTest: fontVertDrawTest.$O
	$(LD) -o fontVertDrawTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../pngDraw.$O ../mkPng.$O ../../genFont/fontST.$O fontVertDrawTest.$O
fontVertDrawTest.$O:	fontVertDrawTest.c ../pngDraw.$O
	$(CC) -c -o fontVertDrawTest.$O $(CFLAGS) fontVertDrawTest.c

normalFontToC: normalFontToC.$O
	$(LD) -o normalFontToC ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../../genFont/fontST.$O normalFontToC.$O
normalFontToC.$O:	normalFontToC.c ../../genFont/fontST.$O
	$(CC) -c -o normalFontToC.$O $(CFLAGS) normalFontToC.c

normalFromCTest: normalFromCTest.$O
	$(LD) -o normalFromCTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../pngDraw.$O ../mkPng.$O ../../genFont/fontST.$O normalFromCTest.$O ../../genFont/normalFont.$O
normalFromCTest.$O:	normalFromCTest.c ../../genFont/normalFont.$O ../pngDraw.$O
	$(CC) -c -o normalFromCTest.$O $(CFLAGS) normalFromCTest.c
../../genFont/normalFont.$O:	../../genFont/normalFont.c ../../genFont/normalFont.h
	$(CC) -c -o ../../genFont/normalFont.$O $(CFLAGS) ../../genFont/normalFont.c

smallFontToC: smallFontToC.$O
	$(LD) -o smallFontToC ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../../genFont/fontST.$O smallFontToC.$O
smallFontToC.$O:	smallFontToC.c ../../genFont/fontST.$O
	$(CC) -c -o smallFontToC.$O $(CFLAGS) smallFontToC.c

smallFromCTest: smallFromCTest.$O
	$(LD) -o smallFromCTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../pngDraw.$O ../mkPng.$O ../../genFont/fontST.$O smallFromCTest.$O ../../genFont/smallFont.$O
smallFromCTest.$O:	smallFromCTest.c ../../genFont/smallFont.$O ../pngDraw.$O
	$(CC) -c -o smallFromCTest.$O $(CFLAGS) smallFromCTest.c
../../genFont/smallFont.$O:	../../genFont/smallFont.c ../../genFont/smallFont.h
	$(CC) -c -o ../../genFont/smallFont.$O $(CFLAGS) ../../genFont/smallFont.c

alphbetTest: alphbetTest.$O
	$(LD) -o alphbetTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../pngDraw.$O ../mkPng.$O ../../genFont/fontST.$O alphbetTest.$O
alphbetTest.$O:	alphbetTest.c ../pngDraw.$O
	$(CC) -c -o alphbetTest.$O $(CFLAGS) alphbetTest.c


../pngDraw.$O: ../pngDraw.h ../pngDraw.c ../../genFont/fontST.$O ../mkPng.$O
	$(CC) -c -o ../pngDraw.$O $(CFLAGS) ../pngDraw.c
../../genFont/fontST.$O: ../../genFont/fontST.h ../../genFont/fontST.c ../../genLib/base10str.$O ../../genLib/ulCp.$O
	$(CC) -c -o ../../genFont/fontST.$O $(CFLAGS) ../../genFont/fontST.c



../mkPng.$O: ../mkPng.h ../mkPng.c ../../genLib/checkSum.$O ../../genLib/64bit.h
	$(CC) -c -o ../mkPng.$O $(CFLAGS) ../mkPng.c

../../genLib/checkSum.$O: ../../genLib/checkSum.c ../../genLib/checkSum.h ../../genLib/endin.$O
	$(CC) -c -o ../../genLib/checkSum.$O $(CFLAGS) ../../genLib/checkSum.c

../../genLib/endin.$O: ../../genLib/endin.c ../../genLib/endin.h
	$(CC) -c -o ../../genLib/endin.$O $(CFLAGS) ../../genLib/endin.c

../../genLib/ulCp.$O: ../../genLib/ulCp.c ../../genLib/ulCp.h
	$(CC) -c -o ../../genLib/ulCp.$O $(CFLAGS) ../../genLib/ulCp.c

../../genLib/base10str.$O: ../../genLib/base10str.c ../../genLib/base10str.h
	$(CC) -c -o ../../genLib/base10str.$O $(CFLAGS) ../../genLib/base10str.c



cleanAll: clean cleanBin

clean:
	if [ -f ../../genLib/ulCp.$O ]; then rm ../../genLib/ulCp.$O; fi;
	if [ -f ../../genLib/base10str.$O ]; then rm ../../genLib/base10str.$O; fi;
	if [ -f ../../genLib/endin.$O ]; then rm ../../genLib/endin.$O; fi;
	if [ -f ../../genLib/checkSum.$O ]; then rm ../../genLib/checkSum.$O; fi;
	if [ -f ../mkPng.$O ]; then rm ../mkPng.$O; fi;
	if [ -f ../pngDraw.$O ]; then rm ../pngDraw.$O; fi;
	if [ -f ../../genFont/fontST.$O ]; then rm ../../genFont/fontST.$O; fi;
	if [ -f ../../genFont/normalFont.$O ]; then rm ../../genFont/normalFont.$O; fi;
	if [ -f ../../genFont/smallFont.$O ]; then rm ../../genFont/smallFont.$O; fi;
	if [ -f testMkPng.$O ]; then rm testMkPng.$O; fi;
	if [ -f fontLoadTest.$O ]; then rm fontLoadTest.$O; fi;
	if [ -f fontHorzDrawTest.$O ]; then rm fontHorzDrawTest.$O; fi;
	if [ -f fontVertDrawTest.$O ]; then rm fontVertDrawTest.$O; fi;
	if [ -f normalFontToC.$O ]; then rm normalFontToC.$O; fi;
	if [ -f normalFromCTest.$O ]; then rm normalFromCTest.$O; fi;
	if [ -f smallFontToC.$O ]; then rm smallFontToC.$O; fi;
	if [ -f smallFromCTest.$O ]; then rm smallFromCTest.$O; fi;
	if [ -f alphbetTest.$O ]; then rm alphbetTest.$O; fi;

cleanBin:
	if [ -f testMkPng ]; then rm testMkPng; fi;
	if [ -f fontLoadTest ]; then rm fontLoadTest; fi;
	if [ -f fontHorzDrawTest ]; then rm fontHorzDrawTest; fi;
	if [ -f fontVertDrawTest ]; then rm fontVertDrawTest; fi;
	if [ -f normalFontToC ]; then rm normalFontToC; fi;
	if [ -f normalFromCTest ]; then rm normalFromCTest; fi;
	if [ -f smallFontToC ]; then rm smallFontToC; fi;
	if [ -f smallFromCTest ]; then rm smallFromCTest; fi;
	if [ -f alphbetTest ]; then rm alphbetTest; fi;
