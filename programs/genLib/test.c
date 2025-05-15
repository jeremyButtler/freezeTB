#include <stdio.h>
#include "mkPng.h"
#include "endLine.h"

int
main(
){
   signed char errSC = 0;
   struct st_mkPng *pngSTPtr = mk_st_mkPng(0, 0, 0);
   FILE *outFILE = fopen("del.png", "w");

   if(! pngSTPtr)
      goto memErr_main;
   if(! outFILE)
      goto fileErr_main;

   addBar_st_mkPng(
      pngSTPtr,
      0,
      0,
      100,
      100,
      3
   );

   print_st_mkPng(pngSTPtr, outFILE);

   memErr_main:;
      errSC = 1;
      fprintf(stderr, "memory error%s", str_endLine);
      goto cleanUp_main;

   fileErr_main:;
      errSC = 2;
      fprintf(stderr, "file error%s", str_endLine);
      goto cleanUp_main;

   cleanUp_main:;
      if(pngSTPtr)
         freeHeap_st_mkPng(pngSTPtr);
      pngSTPtr = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*main*/
