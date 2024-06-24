/*########################################################
# Name: cigToEqxCig
#   - converts an single reference sam file with regular
#     cigars into an sam file with eqx cigars
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: phel_cigToEqxCig
'     - prints the help message
'   o fun02: pversion_cigToEqxCig
'     - prints the version
'   o fun-03: getInput_cigToEqx
'     - convert an sam entry cigar to an eqx cigar
'   o main:
'     - driver function to convert cigar to eqx cigar
'   o license:
'     - licensing for this code (public domain / mit)
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

#include "cigToEqxCig-fun.h"
#include "../memwater/seqStruct.h"
#include "../generalLib/samEntryStruct.h"

/*.h files only*/
#include "../generalLib/dataTypeShortHand.h"
#include "../generalLib/ulCpStr.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .h #include "../generalLib/base10StrToNum.h"
!   o .h #include "../generalLib/numToStr.h"
!   o .h #include "../generalLib/ntToFiveBit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_nAsSnp 0 /*1: treat anoymous bases as snps*/

#define def_year_cigToEqxCig 2024
#define def_month_cigToEqxCig 5
#define def_day_cigToEqxCig 14

/*-------------------------------------------------------\
| Fun01: phelp_cigToEqxCig
|   - prints the help message
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_cigToEqxCig(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - print the help message
   '   o fun01 sec01:
   '     - print out usage
   '   o fun01 sec02:
   '     - print out input block
   '   o fun01 sec03:
   '     - print out output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - print out usage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "cigToEqxCig -sam reads.sam -ref reference.fasta\n"
   );

   fprintf(
      outFILE,
      "  - Converst cigars in sam files to eqx cigars\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - print out input block
   ^   o fun01 sec02 sub01:
   ^     - input block
   ^   o fun01 sec02 sub02:
   ^     - sam file entry
   ^   o fun01 sec02 sub03:
   ^     - reference file entry
   ^   o fun01 sec02 sub04:
   ^     - anonymous base entry
   ^   o fun01 sec02 sub05:
   ^     - help and version number entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *   - input block
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun01 Sec02 Sub02:
   *   - sam file entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -sam reads.sam: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Sam file to convert to cigar eqx entries\n"
   );

   fprintf(
      outFILE,
      "    o Use `sam -` for stdin input\n"
   );

   /*****************************************************\
   * Fun01 Sec02 Sub03:
   *   - reference file entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -ref reference.fastaq: [Required]\n"
   );

   fprintf(
      outFILE,
      "    o Reference file used to map reads to\n"
   );

   /*****************************************************\
   * Fun01 Sec02 Sub04:
   *   - anonymous base entry
   \*****************************************************/

   if(def_nAsSnp)
      fprintf(
         outFILE,
         "  -n-as-snp: [Yes]\n"
      );
   else
      fprintf(
         outFILE,
         "  -n-as-snp: [No]\n"
      );

   fprintf(
      outFILE,
      "    o Treat anonymous bases as SNPs\n"
   );

   fprintf(
      outFILE,
      "    o Disable with `-n-as-match`\n"
   );

   /*****************************************************\
   * Fun01 Sec02 Sub05:
   *   - help and version number entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: print out help message\n"
   );

   fprintf(
      outFILE,
      "  -v: print out version number\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - print out output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:\n"
   );

   fprintf(
      outFILE,
      "  - Prints converted sam file to stdout\n"
   );
} /*phelp_cigToEqxCig*/


/*-------------------------------------------------------\
| Fun02: pversion_cigToEqxCig
|   - prints the version
| Input:
|   - outFILE:
|     o FILE pointer to print version to
| Output:
|   - Prints:
|     o verision to outFILE
\-------------------------------------------------------*/
void
pversion_cigToEqxCig(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "%i-%02i-%02i\n",
      def_year_cigToEqxCig,
      def_month_cigToEqxCig,
      def_day_cigToEqxCig
   );
} /*pversion_cigToEqxCig*/

/*-------------------------------------------------------\
| Fun-03: getInput_cigToEqxCig
|   - extracts user input from an array for cigToEqxCig
| Input:
|   - argAryStr:
|     o array of c-stringsi with user input arguments
|   - numArgsSI:
|     o number of arguments the user input
|   - samFileStr:
|     o pointer to c-string to point to the samfiles name
|   - samFILE:
|     o FILE pointer to hold sam file to convert
|   - samFileStr:
|     o pointer to c-string to point to the reference
|      fasta file name
|   - refSeqSTPtr:
|     o pointer to seqStruct to hold the reference
|       sequence
|   - nAsSnpBl:
|     o point to char to hold if treating anonymous bases
|       as matches or mismathces
|       - 0: matches
|       - 0: mismatches (snps)
| Output:
|   - Modifies:
|     o input to hold user input values
|   - Prints:
|     o help message or version number to sdtout if asked
|       for
|     o For errors; prints the error
|   - Returns:
|     o 0 for no problems
|     o 1 for help message
|     o 2 for problem
\-------------------------------------------------------*/
signed char
getInput_cigToEqxCig(
   char *argAryStr[],
   signed int numArgsSI,
   schar **samFileStr,
   FILE **samFILE,
   schar **refFileStr,
   struct seqStruct *refSeqSTPtr,
   signed char *nAsSnpBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - check user input
   '   o fun03 sec02:
   '     - variable declerations
   '   o fun03 sec03:
   '     - return any errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   FILE *refFILE = 0;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check user input
   ^   o fun03 sec02 sub01:
   ^     - set the sam file pointer
   ^   o fun03 sec02 sub02:
   ^     - read in the reference sequence
   ^   o fun03 sec02 sub03:
   ^     - check how handleing anonymous bases
   ^   o fun03 sec02 sub04:
   ^     - Check for help messages
   ^   o fun03 sec02 sub05:
   ^     - Check for version number
   ^   o fun03 sec02 sub06:
   ^     - Check for invalid input
   ^   o fun03 sec02 sub07:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(siArg < numArgsSI)
   { /*Loop: Read in user input*/

      /**************************************************\
      * Fun03 Sec02 Sub01:
      *   - set the sam file pointer
      \**************************************************/

      if(! cStrEql("-sam", argAryStr[siArg], 0) )
      { /*If: An sam file was input*/
         if(*samFILE && *samFILE != stdin)
            fclose(*samFILE);

         *samFILE = 0;

         ++siArg;

         *samFileStr = (schar *) argAryStr[siArg];

         if(*argAryStr[siArg] == '-')
            *samFILE = stdin;
         else
         { /*Else: User supplied an sam file*/
            *samFILE =
               fopen(
                  (char *) argAryStr[siArg],
                  "r"
               );

            if(! *samFILE)
            { /*If: I could not open the sam file*/
               fprintf(
                  stderr,
                  "Could not open -sam %s\n",
                  argAryStr[siArg]
               );

               errSC |= 2;
            } /*If: I could not open the sam file*/
         } /*Else: User supplied an sam file*/
      } /*If: An sam file was input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - read in the reference sequence
      \**************************************************/

      else if(! cStrEql("-ref", argAryStr[siArg], 0) )
      { /*Else If: An reference file was input*/
         ++siArg;
         *refFileStr = (schar *) argAryStr[siArg];

         refFILE =
             fopen(
                (char *) argAryStr[siArg],
                "r"
             );

         if(! refFILE)
         { /*If: I could not open the sam file*/
            fprintf(
               stderr,
               "Could not open -ref %s\n",
               argAryStr[siArg]
            );

            errSC |= 2;
         } /*If: I could not open the sam file*/

         if(readFaSeq(refFILE, refSeqSTPtr) > 1)
         { /*If: I could not get reference sequence*/
            fprintf(
               stderr,
               "Error while reading -ref %s\n",
               argAryStr[siArg]
            );

            errSC |= 2;
         } /*If: I could not get reference sequence*/

         fclose(refFILE);
         refFILE = 0;
      } /*Else If: An reference file was input*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - check how handleing anonymous bases
      \**************************************************/

      else if(! cStrEql("-n-as-snp", argAryStr[siArg], 0))
         *nAsSnpBl = 1;

      else if(!cStrEql("-n-as-match", argAryStr[siArg],0))
         *nAsSnpBl = 0;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - Check for help messages
      \**************************************************/

      else if(! cStrEql("-h", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         phelp_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("--h", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         phelp_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("help", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         phelp_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("-help", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         phelp_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("--help", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         phelp_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - Check for version number
      \**************************************************/

      else if(! cStrEql("-v", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         pversion_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("--v", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         pversion_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("version", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         pversion_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("-version", argAryStr[siArg], 0) )
      { /*Else If: user wanted the help message*/
         pversion_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      else if(! cStrEql("--version", argAryStr[siArg], 0))
      { /*Else If: user wanted the help message*/
         pversion_cigToEqxCig(stdout);
         return 1;
      } /*Else If: user wanted the help message*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - Check for invalid input
      \**************************************************/

      else
      { /*Else: input not recongnized*/
         fprintf(
            stderr,
            "%s is unkown\n",
            argAryStr[siArg]
         );

         errSC |= 2;
      } /*Else: input not recongnized*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - move to the next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Read in user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return any errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return errSC;
} /*getInput_cigToEqxCig*/


/*-------------------------------------------------------\
| Main:
|    - driver function to convert cigar to eqx cigar
| Input:
|    - numArgsSI:
|      o integer with user input
|    - argsAryStr:
|      o pointer to c-string array with user input
| Output:
|   - prints:
|     o sam file with eqx cigar entries to stdout
\-------------------------------------------------------*/
#ifdef PLAN9
void
#else
int
#endif
main(
  signed int numArgsSI,
  char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to convert cigar to eqx cigar
   '   o main Sec01:
   '     - variable declerations
   '   o main Sec02:
   '     - initialize varialbes and get input
   '   o main sec03:
   '     - convert cigar entris to eqx cigars
   '   o main sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   sint *cigHeapArySI = 0;
   sint lenCigSI = 0;

   schar nAsSnpBl = def_nAsSnp;

   struct seqStruct refSeqStackST;
   struct samEntry samStackST;

   schar endOfHeadBl = 0;

   schar *samFileStr = 0; /*For input command reporting*/
   schar *refFileStr = 0; /*For input command reporting*/
   FILE *samFILE = 0;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize varialbes and get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   initSeqST(&refSeqStackST);
   initSamEntry(&samStackST);

   if(numArgsSI <= 1)
   { /*If: nothing was input*/
      phelp_cigToEqxCig(stderr);

      fprintf(
         stderr,
         "Error: nothing input\n"
      );

      goto cleanUp;
   } /*If: nothing was input*/
   
   errSC =
      getInput_cigToEqxCig(
         argsAryStr,
         numArgsSI,
         &samFileStr,
         &samFILE,
         &refFileStr,
         &refSeqStackST,
         &nAsSnpBl
      ); /*Get user input*/
      
   if(errSC)
   { /*If: I need to exit or had error*/
      --errSC;

      if(errSC)
      { /*If: This was an error*/
         fprintf(
            stderr,
            "Error reading input; use -h for message\n"
         );
      } /*If: This was an error*/

      goto cleanUp;
   } /*If: I need to exit or had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - convert cigar entris to eqx cigars
   ^   o main sec03 sub01:
   ^     - read in first sam file line and start loop
   ^   o main sec03 sub02:
   ^     - print headers
   ^   o main sec03 sub03:
   ^     - print this programs sam file header
   ^   o main sec03 sub04:
   ^     - check if can filter, then filter
   ^   o main sec03 sub05:
   ^     - print entry if keeping and get next entry
   ^   o main sec03 sub06:
   ^     - convert EOF error to no error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - read in first sam file line and start loop
   \*****************************************************/

   errSC =
      readSamLine(
         &samStackST,
         (char **) &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errSC)
   { /*Loop: convert cigar entries to eqx cigars*/

      /**************************************************\
      * Main Sec03 Sub02:
      *   - print headers
      \**************************************************/

      if(
            samStackST.lenExtraUI > 0
         && samStackST.extraStr[0] == '@'
      ){ /*If: This is an comment*/
         pSamEntry(
            &samStackST,
            (char **) &buffHeapStr,
            &lenBuffUL,
            outFILE
         ); /*Print out the comment*/

         goto nextEntry;
      } /*If: This is an comment*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - print this programs sam file header
      \**************************************************/

      if(! endOfHeadBl)
      { /*If: I need to print out the program comment*/
         
         fprintf(
            outFILE,
            "@PG\tID:cigToEqxCig\tPN:cigToEqxCig"
         );

         fprintf(
            outFILE,
            "\tVN:%i-%02i-%02i\tCL:cigToEqxCig -ref %s",
            def_year_cigToEqxCig,
            def_month_cigToEqxCig,
            def_day_cigToEqxCig,
            refFileStr
         );

         if(nAsSnpBl)
            fprintf(
               outFILE,
               " -n-as-snp"
            );

         else
            fprintf(
               outFILE,
               " -n-as-match"
            );

         if(samFileStr)
            fprintf(
               outFILE,
               " -sam %s\n",
               samFileStr
            );

         else
            fprintf(
               outFILE,
               " -sam -\n"
            );

         
         endOfHeadBl = 1;
      } /*If: I need to print out the program comment*/

      /**************************************************\
      * Main Sec03 Sub04:
      *   - check if can filter, then filter
      \**************************************************/

      if(samStackST.readLenUI < 1)
         goto nextEntry; /*No sequence*/

      if(lenBuffUL > lenCigSI)
      { /*If: I need to resize buffers*/
         lenCigSI = (sint) lenBuffUL;

         free(cigHeapArySI);
         cigHeapArySI = 0;

         cigHeapArySI =
            malloc((lenCigSI + 1) * sizeof(sint));

         if(! cigHeapArySI)
         { /*If: I had an memory error*/
            errSC = 1;
            goto cleanUp;
         } /*If: I had an memory error*/
      } /*If: I need to resize buffers*/

      errSC =
         cigToEqxCig(
            &samStackST,
            (schar *) refSeqStackST.seqStr,
            (uint) refSeqStackST.lenSeqUL,
            &buffHeapStr,
            &cigHeapArySI,
            &lenCigSI,
            nAsSnpBl
         );

      if(lenBuffUL < lenCigSI)
         lenBuffUL = (ulong) lenCigSI;

      /**************************************************\
      * Main Sec03 Sub05:
      *   - print entry if keeping and get next entry
      \**************************************************/

      pSamEntry(
         &samStackST,
         (char **) &buffHeapStr,
         &lenBuffUL,
         outFILE
      ); /*Print out the comment*/

      nextEntry:;

      errSC =
         readSamLine(
            &samStackST,
            (char **) &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: convert cigar entries to eqx cigars*/

   /*****************************************************\
   * Main Sec03 Sub06:
   *   - convert EOF error to no error
   \*****************************************************/

   --errSC; /*-1 converts EOF to 0*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp:;

   free(buffHeapStr);
   buffHeapStr = 0;

   free(cigHeapArySI);
   cigHeapArySI = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   if(samFILE && samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   freeSeqSTStack(&refSeqStackST);
   freeSamEntryStack(&samStackST);

   if(errSC)
      exit(-1);

   exit(0);
} /*main*/
