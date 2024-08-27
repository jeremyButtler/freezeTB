/*########################################################
# Name: maskPrim
#   - mask primers in an sequence using input coordaintes
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries and defined variables
'   o fun01: checkHelp_maskPrimMain
'     - checks if input paramter is help message request
'   o fun02: checkVersion_maskPrimMain
'     - checks if input paramter is an version request
'   o fun03: pversion_maskPrimMain
'     - print version number for maskPrim
'   o fun04: phelp_maskPrimMain
'     - print the help message for maskPrim
'   o fun05: input_maskPrimMain
'     - processes user command line supplied input
'   o main:
'     - driver function to mask primers
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/charCp.h"
#include "../genLib/samEntry.h"
#include "maskPrim.h"

/*No .c files (only .h)*/
#include "../genLib/dataTypeShortHand.h"
#include "maskPrimVersion.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c  #include "../genLIb/numToStr.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*Some default values*/
#define def_mask_maskPrimMain 'N'  /*Char to mask with*/
#define def_fudge_maskPrimMain -1  /*Fudge length*/
#define def_filter_maskPrimMain 0  /*0 = do not filter*/
   /*-1 is skip/anyware*/

/*-------------------------------------------------------\
| Fun01: checkHelp_maskPrimMain
|   - checks if input paramter is an help message request
| Input:
|   - parmStr:
|     o parameter to see if is an help message
| Output:
|   - Returns:
|     o 0 if not an help message request
|     o 1 if is an help message request
\-------------------------------------------------------*/
schar
checkHelp_maskPrimMain(
   signed char *parmStr
){
   schar retSC = 0;

   if(
      ! eql_charCp(
         (schar *) "-h",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "--h",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "help",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "-help",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "--help",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   return retSC;
} /*checkHelp_maskPrimMain*/

/*-------------------------------------------------------\
| Fun02: checkVersion_maskPrimMain
|   - checks if input paramter is an version request
| Input:
|   - parmStr:
|     o parameter to see if is an version requiest
| Output:
|   - Returns:
|     o 0 if not an verson request
|     o 1 if is an version request
\-------------------------------------------------------*/
schar
checkVersion_maskPrimMain(
   schar *parmStr
){
   schar retSC = 0;

   if(
      ! eql_charCp(
         (schar *) "-v",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "--v",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   if(
      ! eql_charCp(
         (schar *) "-V",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "--V",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "version",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "-version",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (schar *) "--version",
         (parmStr),
         (schar) '\0'
   )) retSC = 1;

   return retSC; 
} /*checkVersion_maskPrimMain*/

/*-------------------------------------------------------\
| Fun03: pversion_maskPrimMain
|   - print version number for maskPrim
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_maskPrimMain(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "maskPrim version: %i-%02i-%02i\n",
      def_year_maskPrimVersion,
      def_month_maskPrimVersion,
      def_day_maskPrimVersion
   );
} /*pversion_maskPrimMain*/

/*-------------------------------------------------------\
| Fun04: phelp_maskPrimMain
|   - print the help message for maskPrim
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints:
|     o the help messge to outFILE
\-------------------------------------------------------*/
void
phelp_maskPrimMain(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - print the help message for maskPrim
   '   o fun04 sec01:
   '     - usage block
   '   o fun04 sec02:
   '     - input block
   '   o fun04 sec03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "maskPrim -sam reads.sam -prim primer-coords.tsv\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Masks primer positions in reads\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - input block
   ^   o fun04 sec02 sub01:
   ^     - input block header
   ^   o fun04 sec02 sub02:
   ^     - sam file entry
   ^   o fun04 sec02 sub03:
   ^     - output entry
   ^   o fun04 sec02 sub04:
   ^     - primer coordinates entry
   ^   o fun04 sec02 sub05:
   ^     - mask character entry
   ^   o fun04 sec02 sub06:
   ^     - fudge length entry
   ^   o fun04 sec02 sub07:
   ^     - filtering
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub02:
   *   - sam file entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam: [stdin]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Sam file with reads to mask primers in\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdin input\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub03:
   *   - output entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out: [stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o File to output masked sam file to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdout output\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub04:
   *   - primer coordinates entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prim: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Tsv file with the primer coordinages\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - First column is ingored (refernce id?)\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Second column is ingored (primer name?)\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Third column is 1 primers are paired or 0\n"
   );

   fprintf(
     (FILE *) outFILE,
     "        o Paired = both primers must be present to"
   );

   fprintf((FILE *) outFILE,  " mask\n");

   fprintf(
     (FILE *) outFILE,
     "      - Fourth column is forward start coordinate\n"
   );

   fprintf(
     (FILE *) outFILE,
     "        - use \"NA\" for no forward primer\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Fifth column is forward end coordinate\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Six column is reverse start coordinate\n"
   );
   fprintf(
     (FILE *) outFILE,
     "        - use \"NA\" for no reverse primer\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      - Seventh column is reverse end coordinate\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub05:
   *   - mask character entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -mask: [%c]\n",
      def_mask_maskPrimMain
   );

   fprintf(
      (FILE *) outFILE,
      "    o Chacter to mask with\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub06:
   *   - fudge length entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -fudge: [%i]\n",
      def_fudge_maskPrimMain
   );

   fprintf(
      (FILE *) outFILE,
      "    o How much seqence can come before an primer\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o -1 = mask regardless of position\n"
   );

   /*****************************************************\
   * Fun04 Sec02 Sub07:
   *   - filtering
   \*****************************************************/

   if(def_filter_maskPrimMain)
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [Yes]\n"
      );
   } /*If: default is to filter*/

   else
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [No]\n"
      );
   } /*If: default is to filter*/

   fprintf(
      (FILE *) outFILE,
      "    o Remove reads with no detected primers\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o Disable with -no-filter\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints sam file with masked reads to -out\n"
   );
} /*phelp_maskPrimMain*/

/*-------------------------------------------------------\
| Fun05: input_maskPrimMain
|   - processes user command line supplied input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with the users input
|   - samFileStr:
|     o pointer to c-string to point to the input sam file
|       name
|   - primFileStr:
|     o pointer to c-string to point to the input primer
|       coorindates file name
|   - outFileStr:
|     o pointer to c-string to point to ouput file name
|   - maskSC:
|     o Pointer to char to hold charcter to mask with
|   - fudgeSI:
|     o int Pointer to hold the amount to fudge primer
|       start/end by
|   - filterBl:
|     o character set to 1 or 0 if doing filtering.
|       - 1 removing reads without primers
|       - 0 not removing reads without primers
| Output:
|   - Modifies:
|     o all input values except numArgsSI and argAryStr
|       to the user input.
|   - Returns:
|     o 0 for no errors
|     o def_help_maskPrim if help message requested
|     o def_version_maskPrim if version number requested
|     o def_nonNumeric | (arg_index << 8) for numeric
|       errors
|     o def_unkownInput | (arg_index << 8) if an argument
|       could not be recongnized
\-------------------------------------------------------*/
signed long
input_maskPrimMain(
   signed int numArgsSI,
   char *argAryStr[],
   signed char **samFileStr,
   signed char **primFileStr,
   signed char **outFileStr,
   signed char *maskSC,
   signed int *fudgeSI,
   signed char *filterBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - process user command line input
   '   o fun05 sec01:
   '     - variable declerations
   '   o fun05 sec02:
   '     - check if have input
   '   o fun05 sec03:
   '     - process user input
   '   o fun05 sec04:
   '     - return success or error
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiArg = 1;
   schar *tmpStr = 0;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_maskPrimMain(stdout);
      goto phelp_fun05_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - process user input
   ^   o fun05 sec03 sub01:
   ^     - file io (input/output)
   ^   o fun05 sec03 sub02:
   ^     - masking and filter settings
   ^   o fun05 sec03 sub03:
   ^     - help message and version number
   ^   o fun05 sec03 sub04:
   ^     - invalid input
   ^   o fun05 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - file io (input/output)
   \*****************************************************/

   while(uiArg < (uint) numArgsSI)
   { /*Loop: Process each peice of user input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
         )
      ){ /*If: sam file*/
         ++uiArg;
         *samFileStr = (schar *) argAryStr[uiArg];
      } /*If: sam file*/

      else if(
         ! eql_charCp(
            (schar *) "-prim",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
         )
      ){ /*Else If: primer coordiantes file*/
         ++uiArg;
         *primFileStr = (schar *) argAryStr[uiArg];
      } /*Else If: primer coordinates file*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
         )
      ){ /*Else If: output file*/
         ++uiArg;
         *outFileStr = (schar *) argAryStr[uiArg];
      } /*Else If: output file*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - masking and filter settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-mask",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
         )
      ){ /*Else If: mask primers*/
         ++uiArg;
         *maskSC = (schar) *argAryStr[uiArg];
      } /*Else If: mask primers*/

      else if(
         ! eql_charCp(
            (schar *) "-fudge",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
         )
      ){ /*Else If: fudge length*/
         ++uiArg;

         tmpStr = (schar *) argAryStr[uiArg];

         tmpStr +=
            strToSI_base10str(
               (schar *) argAryStr[uiArg],
               fudgeSI
            );

         if(*tmpStr)
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-fudge %s is non-numeric or to large\n",
               argAryStr[uiArg]
            );
 
            goto err_fun05_sec04;
         } /*If: non-numeric input*/
      } /*Else If: fudge length*/

      else if(
         ! eql_charCp(
            (schar *) "-filter",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
      )) *filterBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-filter",
            (schar *) argAryStr[uiArg],
            (schar) '\0'
       )) *filterBl = 0;

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - help message and version number
      \**************************************************/

      else if(
         checkHelp_maskPrimMain(
            (schar *) argAryStr[uiArg]
         )
      ){ /*Else If: help message requested*/
         phelp_maskPrimMain(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: help message requested*/

      else if(
         checkVersion_maskPrimMain(
            (schar *) argAryStr[uiArg])
         )
      { /*Else If: version number requested*/
         pversion_maskPrimMain(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: version number requested*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s no recognized\n",
            argAryStr[uiArg]
         );

         goto err_fun05_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - move to next argument
      \**************************************************/

      ++uiArg;
   } /*Loop: Process each peice of user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return success or error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun05_sec04;

   phelp_fun05_sec04:;
   pversion_fun05_sec04:;
   errSC = 1;
   goto ret_fun05_sec04;

   err_fun05_sec04:;
   errSC = 2;
   goto ret_fun05_sec04;

   ret_fun05_sec04:;
   return errSC;
} /*input_maskPrimMain*/

/*-------------------------------------------------------\
| Main:
|   - driver function to mask primers
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o pointer to an array of c-strings with user
|       arguments
| Output:
|   - Prints:
|     o sam file with masked sequenes to output file
\-------------------------------------------------------*/
#ifdef PLAN9
schar
#else
int
#endif
main(
   signed int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to mask primers
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize, get input, and check input
   '   o main sec03:
   '     - read in the primer coordinates
   '   o main sec04:
   '     - print header
   '   o main sec05:
   '     - mask primers
   '   o main sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *samFileStr = 0;
   schar *primFileStr = 0;
   schar *outFileStr = 0;
   schar maskSC = def_mask_maskPrimMain;
   sint fudgeSI = def_fudge_maskPrimMain;

   schar filterBl = def_filter_maskPrimMain;
     /*Remove reads with no primers*/

   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   uint *startAryHeapUI = 0;
   uint *endAryHeapUI = 0;
   uint *flagAryHeapUI = 0;
   sint numPrimSI = 0;

   struct samEntry samStackST;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*For error reporting*/
   slong errSL = 0;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialzei and get (process) the user input
   ^   o main sec02 sub02:
   ^     - check if can open the primer file
   ^   o main sec02 sub03:
   ^     - check if can open output file
   ^   o main sec02 sub04:
   ^     - check if can open sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize and get (process) user input
   \*****************************************************/

   init_samEntry(&samStackST);

   errSC =
      input_maskPrimMain(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &primFileStr,
         &outFileStr,
         &maskSC,
         &fudgeSI,
         &filterBl
      );

   if(errSC)
   { /*If: error in the user input*/
      --errSC;
          /*convert help/version request to no error (0)*/

      goto cleanUp_main_sec05_sub03;
   } /*If: error in the user input*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if can open primer file
   \*****************************************************/

   outFILE =
      fopen(
         (char *) primFileStr,
         "r"
      );
   
   if(! outFILE)
   { /*If: could not open primer coordinates file*/
      fprintf(
         stderr,
         "Unable to open -prim %s\n",
         primFileStr
      );

      goto err_main_sec05_sub02;
   } /*If: coudl not open primer coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr 
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: output file provided*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );
      
      if(! outFILE)
      { /*If: I could not open primer coordinates file*/
         fprintf(
            stderr,
            "Unable to open -out %s\n",
            outFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: I could not open primer coordinates file*/
   } /*Else: output file provided*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if can open sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: sam file was provided*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open sam file*/
         fprintf(
            stderr,
            "Unable to open -sam %s\n",
            samFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: could not open sam file*/
   } /*Else: sam file was provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numPrimSI =
      getCoords_maskPrim(
         primFileStr,
         &startAryHeapUI,
         &endAryHeapUI,
         &flagAryHeapUI,
         &errSL
      ); /*Get the primer list*/

   if(! numPrimSI)
   { /*If: I had an error*/
      if(errSL & def_emptyFileErr_maskPrim)
      { /*If: I had an empty file*/
         fprintf(
            stderr,
            "-prim %s is emtpy\n",
            primFileStr
         );
      } /*If: I had an empty file*/

      else if(errSL & def_emptyFileErr_maskPrim)
      { /*Else If: I had an invalid line*/
         fprintf(
            stderr,
            "line number %lu in -prim %s is invalid\n",
            (errSL >> 8),
            primFileStr
         );
      } /*Else If: I had an invalid line*/
      
      else
      { /*Else: I had an memory error*/
         fprintf(
            stderr,
            "Memory error reading %s\n",
            primFileStr
         );

      } /*Else: I had an memory error*/

      goto err_main_sec05_sub02;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print header
   ^   o main sec04 sub01:
   ^     - get first sam file entry
   ^   o main sec04 sub02:
   ^     - print sam file header
   ^   o main sec04 sub03:
   ^     - make and print the maskPrim header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - get first sam file entry
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );
       
      goto err_main_sec05_sub02;
   } /*If: memory error*/

   errSC =
      (schar)
      get_samEntry(
        &samStackST,
        &buffHeapStr,
        &lenBuffUL,
        samFILE
      ); /*Get the first line in the sam file*/

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - print sam file header
   \*****************************************************/

   while(! errSC)
   { /*Loop: print header*/
      if(*samStackST.extraStr != '@')
         break;

      p_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         0,          /*0 = print newline at end*/
         outFILE
      );

      errSC =
         get_samEntry(
           &samStackST,
           &buffHeapStr,
           &lenBuffUL,
           samFILE
         ); /*Get the first line in the sam file*/
   } /*Loop: print header*/

   /**************************************************\
   * Main Sec04 Sub03:
   *   - make and print the maskPrim header
   \**************************************************/

   fprintf(
      outFILE,
      "@PG\tID:maskPrim\tPN:maskPrim"
   );

   fprintf(
      outFILE,
      "\tVN:%i-%02i-%02i\tCL:maskPrim -mask %c",
      def_year_maskPrimVersion,
      def_month_maskPrimVersion,
      def_day_maskPrimVersion,
      maskSC
   );

   if(filterBl)
      fprintf(
         outFILE,
         "\t-filter"
      );
   else
      fprintf(
         outFILE,
         "\t-no-filter"
      );

   fprintf(
      outFILE,
      "\t-fudge %i -prim %s",
      fudgeSI,
      primFileStr
   );

   if(samFileStr)
      fprintf(
         outFILE,
         "\t-sam %s",
         samFileStr
      );
   else
      fprintf(
         outFILE,
         "\t-sam -"
      );

   if(outFileStr)
      fprintf(
         outFILE,
         "\t-out %s\n",
         outFileStr
      );
   else
      fprintf(
         outFILE,
         "\t-out -\n"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - mask primers
   ^   o main sec04 sub01:
   ^     - mask primers and print entry + start loop
   ^   o main sec04 sub02:
   ^     - get the next entry
   ^   o main sec04 sub03:
   ^     - check if had an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *  - mask primers and print entry + start loop
   \*****************************************************/

   while(! errSC)
   { /*Loop: Read in all lines and mask primers*/
      if(*samStackST.seqStr == '*')
         goto nextLine_main_sec05; /*no sequence*/
      
      errSL =
         maskPrim(
            &samStackST,
            startAryHeapUI,
            endAryHeapUI,
            flagAryHeapUI,
            numPrimSI,
            fudgeSI,
            maskSC
         ); /*Mask primers in the sequence*/

      if(
           filterBl
         && errSL
      ) goto nextLine_main_sec05; /*rm non-primer seq*/

      p_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         0,          /*0 = print newline at end*/
         outFILE
      );

      /**************************************************\
      * Main Sec05 Sub02:
      *  - get the next entry
      \**************************************************/

      nextLine_main_sec05:;

      errSC =
         get_samEntry(
           &samStackST,
           &buffHeapStr,
           &lenBuffUL,
           samFILE
         ); /*Get the first line in the sam file*/
   } /*Loop: Read in all lines and mask primers*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *  - check if had an error
   \*****************************************************/

   if(errSC != 1)
   { /*If: I had an memroy error*/
      fprintf(
         stderr,
         "Memory error reading -sam %s\n",
         samFileStr
      );

      goto err_main_sec05_sub02;
   } /*If: I had an memroy error*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - Success clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec05_sub03;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - Error clean up
   \*****************************************************/

   err_main_sec05_sub02:;
   errSC = -1;
   goto cleanUp_main_sec05_sub03;

   cleanUp_main_sec05_sub03:;

   if(startAryHeapUI)
      free(startAryHeapUI);

   startAryHeapUI = 0;

   if(endAryHeapUI)
      free(endAryHeapUI);

   endAryHeapUI = 0;

   if(flagAryHeapUI)
      free(flagAryHeapUI);

   flagAryHeapUI = 0;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

   freeStack_samEntry(&samStackST);

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   outFILE = 0;

   return errSC;
} /*main*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
