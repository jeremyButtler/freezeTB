/*SPDX-License-Identifier: CC0-1.0*/

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
'   o main:
'     - driver function for closestSpol
' This file is released into the Public Domain under
'   CC0-1.0.
' See https://creativecommons.org/publicdomain/zero/1.0/
'   for details (or ../../license-CC-1.0.txt).
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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
#include "../genLib/base10str.h"
#include "../genFreezeTB/spolST.h"
#include "../genFreezeTB/findCloseBarcodes.h"

/*these files are only .h files or only .h file used*/
#include "../genLib/genMath.h"
#include "../genLib/endLine.h"
#include "../ftbVersion.h"
#include "../genFreezeTB/tbSpolDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/64bit.h"
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - inputs
   '   o fun02 sec03:
   '     - outputs
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "closestSpol -bar <barcode> -db <database.csv>%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  - prints lineages within 5 spacers of barcode%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - inputs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  -bar <barcode>: [Required]%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o barcode to search for (0101.../oIoI...):%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -db <database>:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o tbProfiler spoligotype lineage database%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -dist %i:%s",
      def_maxDist_tbSpolDefs,
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o maximum edit distance between barcodes%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - outputs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
|   - barStrPtr:
|     o c-string pointer to get set to input barcode
|   - dbStrPtr:
|     o c-string pointer to get set to input database path
|   - distSCPtr:
|     o signed char pointer to get maximum edit distance
| Output:
|   - Prints:
|     o help message or version number to stdout if
|       requested
|     o also prints errors to stdout
|   - Modifies:
|     o barStrPtr to piont to the barcode (do not free)
|     o dbStrPtr to piont to database file (do not free)
|     o distSCPtr to be set to the maximum edit distance
|   - Returns:
|     o 0 if can run closestSpol
|     o 1 if requested help or version number
|     o 2 if can not run closestSpol (error)
\-------------------------------------------------------*/
signed char
input_closestSpol(
   signed int argLenSI,
   char *argAryStr[],
   signed char **barStrPtr,
   signed char **dbStrPtr,
   signed char *distSCPtr
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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check for help and version number requests
   ^   o fun03 sec02 sub01:
   ^     - check if barcode + start loop
   ^   o fun03 sec02 sub02:
   ^     - check if database input
   ^   o fun03 sec02 sub03:
   ^     - check if distance input
   ^   o fun03 sec02 sub04:
   ^     - help message and version checks
   ^   o fun03 sec02 sub05:
   ^     - unkown input or move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - check if barcode + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: check for version/help*/
      errStr = (signed char *) argAryStr[siArg];

      if( ! eqlNull_ulCp((signed char *) "-bar", errStr) )
      { /*If: barcode*/
         ++siArg;

         if(siArg >= argLenSI)
         { /*If: no second argument*/
            errStr =
               (signed char *) "-bar needs an argument";
            goto err_fun03_sec05;
         } /*If: no second argument*/

         *barStrPtr = (signed char *) argAryStr[siArg];
      } /*If: barcode*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - check if database input
      \**************************************************/

      else if(
         ! eqlNull_ulCp((signed char *) "-db", errStr)
      ){ /*Else If: database*/
         ++siArg;

         if(siArg >= argLenSI)
         { /*If: no second argument*/
            errStr =
               (signed char *) "-db needs an argument";
            goto err_fun03_sec05;
         } /*If: no second argument*/

         *dbStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: database*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - check if distance input
      \**************************************************/

      else if(
         ! eqlNull_ulCp((signed char *) "-dist", errStr)
      ){ /*Else If: edit distance*/
         ++siArg;

         if(siArg >= argLenSI)
         { /*If: no second argument*/
            errStr =
               (signed char *) "-dist needs an argument";
            goto err_fun03_sec05;
         } /*If: no second argument*/

         errStr = (signed char *) argAryStr[siArg];
         errStr += strToSC_base10str(errStr, distSCPtr);

         if(*errStr)
         { /*If: could not get edit distance*/
            errStr =
               (signed char *)
               "-dist input is non-numeric or to large";
            goto err_fun03_sec05;
         } /*If: could not get edit distance*/

         else if(*distSCPtr < 0)
         { /*Else If: invalid edit distance*/
            errStr =
               (signed char *)
               "-dist input must be 0 or greater";
            goto err_fun03_sec05;
         } /*Else If: invalid edit distance*/
 
      }  /*Else If: edit distance*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - help message and version checks
      \**************************************************/

      else if(! eqlNull_ulCp((signed char *) "-h",errStr))
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

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - unkown input or move to next argument
      \**************************************************/

      else
      { /*Else: unkown input*/
         errStr = (signed char *) "unkown input";
         goto err_fun03_sec05;
      } /*Else: unkown input*/

      ++siArg;
   } /*Loop: check for version/help*/

   if(argLenSI < 4)
      goto phelp_fun03_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - check if barcode is valid
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *barStrPtr)
   { /*If: no barcode input*/
      errStr =
         (signed char *)
         "no bar (-bar <barcode>) input";
      goto err_fun03_sec05;
   } /*If: no barcode input*/

   errStr = *barStrPtr;
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

   if(! *dbStrPtr)
   { /*If: no barcode input*/
      errStr =
         (signed char *)
         "no spoligotype database (-db <database>) input";
      goto err_fun03_sec05;
   } /*If: no barcode input*/

   dbFILE = fopen((char *) *dbStrPtr, "r");

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
      fprintf(stderr, "%s%s", errStr, str_endLine);
      siArg = 2;
      goto ret_fun03_sec05;

   ret_fun03_sec05:;
      if(dbFILE)
         fclose(dbFILE);
      dbFILE = 0;

      return siArg;
} /*input_closestSpol*/

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
   signed char *dbStr = 0;
   signed char maxDistSC = def_maxDist_tbSpolDefs;

   struct spolST *spolHeapAryST = 0;
   signed int numSpolSI = 0;

   signed long *distHeapArySL = 0;
   signed int distLenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - check input and get database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   errSC =
      input_closestSpol(
         argLenSI,
         argAryStr,
         &barStr,
         &dbStr,
         &maxDistSC
      );
   if(errSC)
   { /*If: can not run closestSpol*/
      --errSC;
      goto ret_main_sec0x; /*errors and messages printed*/
   } /*If: can not run closestSpol*/

   spolHeapAryST = readDb_spolST(dbStr,&numSpolSI,&errSC);
   if(! spolHeapAryST)
      goto err_main_sec0x;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - find close lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   distHeapArySL =
      getDistances_findCloseBarcodes(
         barStr,
         maxDistSC,
         &distLenSI,
         spolHeapAryST,
         numSpolSI
      );

   if(distLenSI < 0)
      fprintf(stderr, "memory error%s", str_endLine);
   else if(! distLenSI)
      fprintf(
         stderr,
         "no barcode within a distance of %i or less%s",
         maxDistSC,
         str_endLine
      );
   else
   { /*Else: print all close matches*/
      pCloseLineages_findCloseBarcodes(
         barStr,
         distHeapArySL,
         distLenSI,
         spolHeapAryST,
         stdout
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
