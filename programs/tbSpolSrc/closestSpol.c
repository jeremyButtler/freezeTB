/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' closestSpol SOF: Start Of File
'   - finds the closest spoligotype for a barcode
'   o header:
'     - included libraries
'   o fun01: pversion_closestSpol
'     - prints version number for closestSpol
'   o fun02: phelp_closestSpol
'     - prints help message
'   o fun03: input_closestSpol
'     - checks for help and version number requests and
'       also some light checks
'   o fun04: barcodeToCode_closestSpol
'     - converts a spoligotype barcode to an unsigned long
'   o mac01: setDistIndex_closestSpol
'     - adds distance & database index into a signed long
'   o mac02: distGet_closestSpol
'     - gets the edit distance from a signed long set with
'       (setDistIndex_closestSpol)
'   o mac03: indexGet_closestSpol
'     - gets the index from a signed long set with
'       (setDistIndex_closestSpol)
'   o fun05: getDistances_closestSpol
'     - get every spoligotype lineage with x distance from
'       the input barcode
'   o fun06: codeToBarcode_closestSpol
'     - convert a code stored in an unsigned long
'       (returned from barcodeToCode_closestSpol) to a
'       c-string barcode
'   o fun07: pCloseLineages_closestSpol
'     - print the detected close lineages found by
'       getDistances_closestSpol
'   o main:
'     - driver function for closestSpol
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*Compiler command
  cc -o closestSpol \
     closestSpol.c \
     ../genLib/ulCp.c \
     ../genLib/numToStr.c \
     ../genLib/charCp.c \
     ../genLib/fileFun.c \
     ../genFreezeTB/spolST.c
for debbugging
  cc -o closestSpol \
     -ggdb -O0 -Wall -Wextra -Wpedantic -Wundef \
     -Wshadow -Wunreachable-code -Winit-self \
     -Wpointer-arith -Wstrict-prototypes \
     -Wwrite-strings -Waggregate-return -Wundef \
     closestSpol.c \
     ../genLib/ulCp.c \
     ../genLib/numToStr.c \
     ../genLib/charCp.c \
     ../genLib/fileFun.c \
     ../genFreezeTB/spolST.c
*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genFreezeTB/spolST.h"

/*these files are only .h files or only .h file used*/
#include "../genLib/genMath.h"
#include "../genLib/endLine.h"
#include "../ftbVersion.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/64bit.h"
!   - .h  #include "tbSpolDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_closestSpol
|   - prints version number for closestSpol
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_closestSpol(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "closestSpol from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_closestSpol*/

/*-------------------------------------------------------\
| Fun02: phelp_closestSpol
|   - prints help message
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_closestSpol(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "closestSpol <barcode> <database.csv>%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  - prints lineages within 5 spacers of barcode%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - <barcode>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o barcode to search for (0101.../oIoI...):%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - <database>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o tbProfiler spoligotype lineage database%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints closet lineages to stdout%s",
      str_endLine
   );
} /*phelp_closestSpol*/

/*-------------------------------------------------------\
| Fun03: input_closestSpol
|   - checks for help and version number requests and also
|     some light checks
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o help message or version number to stdout if
|       requested
|     o also prints errors to stdout
|   - Returns:
|     o 0 if can run closestSpol
|     o 1 if requested help or version number
|     o 2 if can not run closestSpol (help/version/error)
\-------------------------------------------------------*/
signed char
input_closestSpol(
   signed int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - checks for help and version number requests
   '   o fun03 sec01:
   '     - variable declarations and argment length checks
   '   o fun03 sec02:
   '     - check for help and version number requests
   '   o fun03 sec03:
   '     - check if barcode is valid
   '   o fun03 sec04:
   '     - check if database is a file
   '   o fun03 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and argment length checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *errStr = 0;
   FILE *dbFILE = 0;

   if(argLenSI < 1)
      goto phelp_fun03_sec05;
   else if(argLenSI > 3)
   { /*Else If: to many arguments*/
      errStr = (signed char *) "to many values input";
      goto phelp_fun03_sec05;
   } /*Else If: to many arguments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check for help and version number requests
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(siArg < argLenSI)
   { /*Loop: check for version/help*/
      errStr = (signed char *) argAryStr[siArg];

      if( ! eqlNull_ulCp((signed char *) "-h", errStr) )
         goto phelp_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "--h", errStr)
      ) goto phelp_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "help", errStr)
      ) goto phelp_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "-help", errStr)
      ) goto phelp_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "--help", errStr)
      ) goto phelp_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "-v", errStr)
      ) goto pversion_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "--v", errStr)
      ) goto pversion_fun03_sec05;

      else if(
         ! eqlNull_ulCp((signed char *) "version", errStr)
      ) goto pversion_fun03_sec05;

      else if(
         !eqlNull_ulCp((signed char *) "-version",errStr)
      ) goto pversion_fun03_sec05;

      else if(
         !eqlNull_ulCp((signed char *) "--version",errStr)
      ) goto pversion_fun03_sec05;

      ++siArg;
   } /*Loop: check for version/help*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - check if barcode is valid
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errStr = (signed char *) argAryStr[1];
   siArg = 0;

   while(errStr[siArg])
   { /*Loop: check the barcode*/
      if( (errStr[siArg] | 32) == 'o' )
         ;
      else if( (errStr[siArg] | 32) == 'i' )
         ;
      else if( (errStr[siArg] | 32) == 'n' )
         ;
      else if( (errStr[siArg] | 32) == '0' )
         ;
      else if( (errStr[siArg] | 32) == '1' )
         ;
      else if( (errStr[siArg] | 32) == 'x' )
         ;
      else
      { /*Else: not a barcode*/
         errStr =
            (signed char *)
            "a barcode can only have: o, i, n, 0, 1, x";
         goto err_fun03_sec05;
      } /*Else: not a barcode*/

      ++siArg;
   } /*Loop: check the barcode*/

   if(siArg < 43)
   { /*If: barcode is to short*/
      errStr =
         (signed char *)
         "barcode is less then 43 spacers long";
      goto err_fun03_sec05;
   } /*If: barcode is to short*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - check if database is a file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   dbFILE = fopen(argAryStr[2], "r");

   if(! dbFILE)
   { /*If: could not open the lineage database*/
       errStr =
          (signed char *) "could not open input database";
       goto err_fun03_sec05;
   } /*If: could not open the lineage database*/

   else
      fclose(dbFILE);
   dbFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec05;

   phelp_fun03_sec05:;
      phelp_closestSpol(stdout);
      siArg = 1;
      goto ret_fun03_sec05;

   pversion_fun03_sec05:;
      pversion_closestSpol(stdout);
      siArg = 1;
      goto ret_fun03_sec05;

   err_fun03_sec05:;
      phelp_closestSpol(stdout);
      printf("%s%s", errStr, str_endLine);
      siArg = 2;
      goto ret_fun03_sec05;

   ret_fun03_sec05:;
      if(dbFILE)
         fclose(dbFILE);
      dbFILE = 0;

      return siArg;
} /*input_closestSpol*/

/*-------------------------------------------------------\
| Fun04: barcodeToCode_closestSpol
|   - converts a spoligotype barcode to an unsigned long
| Input:
|   - barStr:
|     o c-string with barcode to convert
| Output:
|   - Returns:
|     o unsigned long with barcoded
\-------------------------------------------------------*/
unsigned long
barcodeToCode_closestSpol(
   signed char *barStr
){
   unsigned char ucSpace = 0;
   unsigned long barcodeUL = 0;

   for(ucSpace = 0; ucSpace < 43; ++ucSpace)
   { /*Loop: convert barcode to code*/
      barcodeUL <<= 1;
      if( (barStr[ucSpace] | 32) == 'i')
         barcodeUL |= 1;
      else if( (barStr[ucSpace] | 32) == 'n')
         barcodeUL |= 1;
      else if(barStr[ucSpace] == '1')
         barcodeUL |= 1;
   } /*Loop: convert barcode to code*/

   return barcodeUL;
} /*barcodeToCode_closestSpol*/

/*-------------------------------------------------------\
| Mac01: setDistIndex_closestSpol
|   - adds distance and database index into a signed long
| Input:
|   - indexMacSI:
|     o index to add to the signed long
|   - distanceMacSC:
|     o distance (signed char) to add to the signed long
| Output:
|   - Returns:
|     o signed long with index and distance
\-------------------------------------------------------*/
#define setDistIndex_closestSpol(indexMacSI, distanceMacSC) ( (signed long) ((((signed long) (indexMacSI)) << 8) | ((distanceMacSC) & 0xff)) )
   /*Logic:
   `   - movedIndex: indexSIMac << 8:
   `     o moves index up 8 bits, adds room for distance
   `   - distance: distanceMacSC & 0xff
   `     o converts distance to a signed char
   `   - movedIndex | distance:
   `     o sets up signed long to have distance and index
   */

/*-------------------------------------------------------\
| Mac02: distGet_closestSpol
|   - gets the edit distance from a signed long set with
|     (setDistIndex_closestSpol)
| Input:
|   - distanceMacOneSL:
|     o signed long with edit distance to get
| Output:
|   - Returns:
|     o signed char with distance
\-------------------------------------------------------*/
#define distGet_closestSpol(distanceMacOneSL) ( (distanceMacOneSL) & 0xff)

/*-------------------------------------------------------\
| Mac03: indexGet_closestSpol
|   - gets the index from a signed long set with
|     (setDistIndex_closestSpol)
| Input:
|   - distanceMacTwoSL:
|     o signed long with index to get
| Output:
|   - Returns:
|     o signed int with index
\-------------------------------------------------------*/
#define indexGet_closestSpol(distanceMacTwoSL) ( (distanceMacTwoSL) >> 8 )

/*-------------------------------------------------------\
| Fun05: getDistances_closestSpol
|   - get every spoligotype lineage with x distance from
|     the input barcode
| Input:
|   - barStr:
|     o c-string with barcode to convert
|     o valid characters:
|       * spacer persent: I, i, N, n, or 1
|       * spacer missing: any chacter that is not present
|   - maxDistSC:
|     o maximum edit distance to count lineage as close
|   - retLenSIPtr:
|     o signed int pointer to be set to the number of
|       lineages that were close to barStr
|   - spolAryST:
|     o spolST struct array with lineages to compare to
|   - numSpolSI:
|     o number lineages in spolAryST
| Output:
|   - Modifies:
|     o retLenSIPtr to have the number of close lineages
|       * set to -1 for a memory error
|   - Returns:
|     o signed long array with lineage index (in
|       spolAryST) and edit distance
|       * use indexGet_closestSpol(array[index]) to get
|         the lineage index
|       * use distGet_closestSpol(array[index]) to get
|         the edit distance from a lineage
|       * length of returned array is set in retLenSIPtr
|     o 0 for no lineages or a memory error
\-------------------------------------------------------*/
signed long *
getDistances_closestSpol(
   signed char *barStr,      /*barcode to search for*/
   signed char maxDistSC,    /*max distance to keep*/
   signed int *retLenSIPtr,  /*number of hits found*/
   struct spolST *spolAryST, /*spoligotype database*/
   signed int numSpolSI      /*number of spoligotypes*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - get every spoligotype lineage with x distance
   '     from the input barcode
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - find the distances
   '   o fun05 sec03:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned long codeUL = 0;
   unsigned long cmpUL = 0;
   unsigned long barcodeUL = 0;

   signed int siSpol = 0;
   unsigned char ucSpace = 0;
   signed char distSC = 0;

   signed long *tmpArySL = 0;
   signed long *distHeapArySL = 0;
   signed int distSizeSI = 16;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - find the distances
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *retLenSIPtr = 0;

   distHeapArySL =
      malloc(distSizeSI * sizeof(signed long));
   if(! distHeapArySL)
      goto memErr_fun02_sec0x;

   barcodeUL = barcodeToCode_closestSpol(barStr);
   for(siSpol = 0; siSpol < numSpolSI; ++siSpol)
   { /*Loop: find edit distance*/
      distSC = 0;

      codeUL = spolAryST[siSpol].codeUL;
      cmpUL = barcodeUL;

      for(ucSpace = 0; ucSpace < 43; ++ucSpace)
      { /*Loop: compare to spoligotype*/
         distSC += ((codeUL & 1) ^ (cmpUL & 1));
         codeUL >>= 1;
         cmpUL >>= 1;
      } /*Loop: compare to spoligotype*/

      if(distSC <= maxDistSC)
      { /*If: distance within limits*/
         if(*retLenSIPtr >= distSizeSI)
         { /*If: need more memory*/
            distSizeSI += (distSizeSI >> 2);
            tmpArySL =
               realloc(
                  distHeapArySL,
                  distSizeSI * sizeof(signed long)
               );
            if(! tmpArySL)
               goto memErr_fun02_sec0x;
            distHeapArySL = tmpArySL;
            tmpArySL = 0;
         } /*If: need more memory*/

         distHeapArySL[*retLenSIPtr] =
            setDistIndex_closestSpol(siSpol, distSC);
         ++(*retLenSIPtr);
      } /*If: distance within limits*/
   } /*Loop: find edit distance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return distHeapArySL;

   memErr_fun02_sec0x:;
      if(distHeapArySL)
         free(distHeapArySL);
      distHeapArySL = 0;
      *retLenSIPtr = -1;
      return 0;
} /*getDistances_closestSpol*/

/*-------------------------------------------------------\
| Fun06: codeToBarcode_closestSpol
|   - convert a code stored in an unsigned long (returned
|     from barcodeToCode_closestSpol) to a c-string
|     barcode
| Input:
|   - codeUL:
|     o returned value from barcodeToCode_closestSpol
|       * unsigned long with bits set to 1 or 0 for
|         each spacer
|   - barStr:
|     o c-string with 44 elements to store the barcode in
| Output:
|   - Modifies:
|     o barStr to have 'o' for missing spacers and 'I' for
|       present spacers in codeUL
\-------------------------------------------------------*/
void
codeToBarcode_closetSpol(
   unsigned long codeUL,
   signed char *barStr   /*44 element c-string*/
){
   signed char scSpacer = 0;

   barStr[43] = 0;
   for(scSpacer = 42; scSpacer >= 0; --scSpacer)
   { /*Loop: build the barcode*/
      if(codeUL & 1)
         barStr[scSpacer] = 'I';
      else
         barStr[scSpacer] = 'o';
      codeUL >>= 1;
   } /*Loop: build the barcode*/
} /*codeToBarcode_cloestSpol*/

/*-------------------------------------------------------\
| Fun07: pCloseLineages_closestSpol
|   - print the detected close lineages found by
|     getDistances_closestSpol
| Input:
|   - barStr:
|     o c-stirng with barcode to print results for
|   - distArySL:
|     o signed long array wit edit distance and lineage
|       distances from getDistances_closestSpol
|   - distLenSI:
|     o number of distances found
|   - spolAryST:
|     o spolST struct array with lineages that were
|       searched
| Output:
|   - Prints:
|     o lineages ind distArySL to stdout
|       * lineage_strain\tlineage\tdistance\tbarcode
|       * first line is a header
|       * for barcode, uppercase characters are matches,
|         lower case characters are missmatches
\-------------------------------------------------------*/
void
pCloseLineages_closestSpol(
   signed char *barStr,      /*barcode looked for*/
   signed long *distArySL,   /*has distance and index*/
   signed int distLenSI,     /*number of matches*/
   struct spolST *spolAryST  /*spoligotype database*/
){
   signed int indexSI = 0;
   signed int siDist = 0;
   signed char distSC = 0;
   signed char outBarStr[44];
   unsigned long codeUL = 0;
   unsigned long barCodeUL = 0;
   unsigned long cmpCodeUL = 0;

   signed int setMissSI = 0;
   signed int delMissSI = 0;

   codeUL = barcodeToCode_closestSpol(barStr);

   printf("id\tlineage\tdistance\tdelMiss\tsetMiss");
   printf("\tbarcode%s", str_endLine);

   if(! distLenSI)
      return ;
   else
   { /*Else: print all close matches*/
      for(siDist = 0; siDist < distLenSI; ++siDist)
      { /*Loop: print close hits*/
         indexSI=indexGet_closestSpol(distArySL[siDist]);
         cmpCodeUL = spolAryST[indexSI].codeUL;
         barCodeUL = codeUL;

         setMissSI = 0;
         delMissSI = 0;
 
         for(distSC = 42; distSC >= 0; --distSC)
         { /*Loop: convert output barcode to show diffs*/
            switch((barCodeUL &1) | ((cmpCodeUL &1) <<1))
            { /*Switch: check if match*/
               case 0:
                  outBarStr[distSC] = 'O';
                  break;
               case 1:
                   outBarStr[distSC] = 'i';
                   ++delMissSI;
                   break;
               case 2:
                   outBarStr[distSC] = 'o';
                   ++setMissSI;
                   break;
               case 3:
                   outBarStr[distSC] = 'I';
                   break;
            } /*Switch: check if match*/

            barCodeUL >>= 1;
            cmpCodeUL >>= 1;
         } /*Loop: convert output barcode to show diffs*/

         outBarStr[43] = 0;
         distSC = distGet_closestSpol(distArySL[siDist]);

         printf(
            "%s\t%s\t%i\t%i\t%i\t%s%s",
            spolAryST[indexSI].idStr,
            spolAryST[indexSI].lineageStr,
            distSC,
            setMissSI,
            delMissSI,
            outBarStr,
            str_endLine
         );
      } /*Loop: print close hits*/
   } /*Else: print all close matches*/
} /*pCloseLineages_closestSpol*/

/*-------------------------------------------------------\
| Main:
|   - driver function for closestSpol
| Input:
|   - argLenSI;
|     o number of input arguments
|   - argAryStr:
|     o c-string array with user input
|       * argAryStr[1] is the barcode (as c-string)
|       * argAryStr[2] is the lineage database
\-------------------------------------------------------*/
int
main(
   int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for closestSpol
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - check input and get database
   '   o main sec03:
   '     - find close lineages
   '   o main sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char *barStr = 0;
   struct spolST *spolHeapAryST = 0;
   signed int numSpolSI = 0;

   signed long *distHeapArySL = 0;
   signed int distLenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - check input and get database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   errSC = input_closestSpol(argLenSI, argAryStr);
   if(errSC)
   { /*If: can not run closestSpol*/
      --errSC;
      goto ret_main_sec0x; /*errors and messages printed*/
   } /*If: can not run closestSpol*/

   barStr = (signed char *) argAryStr[2];
   spolHeapAryST =
      readDb_spolST(barStr, &numSpolSI, &errSC);
   if(! spolHeapAryST)
      goto err_main_sec0x;

   barStr = (signed char *) argAryStr[1];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - find close lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   distHeapArySL =
      getDistances_closestSpol(
         barStr,
         5,
         &distLenSI,
         spolHeapAryST,
         numSpolSI
      );

   if(distLenSI < 0)
      fprintf(stderr, "memory error%s", str_endLine);
   else if(! distLenSI)
      fprintf(
         stderr,
         "no barcode within a distance of 5 or less%s",
         str_endLine
      );
   else
   { /*Else: print all close matches*/
      pCloseLineages_closestSpol(
         barStr,
         distHeapArySL,
         distLenSI,
         spolHeapAryST
      );
   } /*Else: print all close matches*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec0x;

   err_main_sec0x:;
      phelp_closestSpol(stdout);
      errSC = 1;
      goto ret_main_sec0x;

   ret_main_sec0x:;
      if(spolHeapAryST)
         freeHeapAry_spolST(spolHeapAryST, numSpolSI);
      spolHeapAryST = 0;

      if(distHeapArySL)
         free(distHeapArySL);
      distHeapArySL = 0;

      return errSC;
} /*main*/
