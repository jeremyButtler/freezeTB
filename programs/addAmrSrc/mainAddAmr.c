/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainAddAmr SOF: Start Of File
'   - has functions to add AMRs to a tbAmr database
'   o fun01: pversion_mainAddAmr
'     - prints version number for mainAddAmr
'   o fun02: phelp_mainAddAmr
'     - prints help message for addAmr
'   o fun03: input_mainAddAmr
'     - gets input from argument array provided to main
'   o license:
'     - licensing for this code (public dofun09 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genBio/geneCoord.h"
#include "../genBio/seqST.h"
#include "../genFreezeTB/amrST.h"
#include "../genFreezeTB/addAmr.h"

/*.h files only*/
#include "../ftbVersion.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "../genBio/codonFun.h"
!   - .c  #include "../genFreezeTB/drugAry.h"
!   - .h  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainAddAmr
|   - prints version number for addAmr
| Input:
|   - outFILE:
|     o FILE pointer to file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainAddAmr(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "addAmr from freezeTB version: %i-%02i-%02i%s",
      def_year_ftbVersion,
      def_month_ftbVersion,
      def_day_ftbVersion,
      str_endLine
   );
} /*pversion_mainAddAmr*/

/*-------------------------------------------------------\
| Fun02: phelp_mainAddAmr
|   - prints help message for addAmr
| Input:
|   - outFILE:
|     o FILE pointer to file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainAddAmr(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for addAmr
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - input entry
   '   o fun02 sec03:
   '     - output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "addAmr -ref reference.fasta"
   );

   fprintf(
      (FILE *) outFILE,
      " -coords gene-coordinates.tsv -var variants.tsv%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  - adds variant ids in variants.tsv to a tbAmr%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    (freezeTB) database%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input entry
   ^   o fun02 sec02 sub01:
   ^     - input header
   *   o fun02 sec02 sub02:
   *     - gene coordinates file
   *   o fun02 sec02 sub03:
   *     - reference
   ^   o fun02 sec02 sub04:
   ^     - database to add to
   ^   o fun02 sec02 sub05:
   ^     - output file
   ^   o fun02 sec02 sub06:
   ^     - file variant id input
   ^   o fun02 sec02 sub07:
   ^     - variant gene
   ^   o fun02 sec02 sub08:
   ^     - AMRs
   ^   o fun02 sec02 sub09:
   ^     - comment entries
   ^   o fun02 sec02 sub10:
   ^     - help and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - reference
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref reference.fasta: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta file with reference sequence%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - gene coordinates file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -coords gene-coords.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o tsv file with coordinates for each gene%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o format:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * 1st column is the gene name%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * 2nd column is ignored (reference id)%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * 3rd is `-` or `R` for reverse, anything%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        else for forward gene%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * 4th is 1st gene position on reference%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * 5th is last gene position on reference%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o 1st line is always a header (skipped)%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `gene_name ignore gene_direction"
   );

   fprintf(
      (FILE *) outFILE,
      " gene_start gene_end`%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o remaining lines (like example beneath)%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `gryA H37Rv F 7302 9818`%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `katG H37Rv R 2153889 2156111`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - database to add to
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -db database.tsv: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o database to print out with new variants%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - output file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out file.tsv: [Option; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to save new tbAmr database to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use `-out -` for stdout output%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - file variant id input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -var file.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o file with variant ID's%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o each line starts with the variant id and%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      entries are separated by spaces%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o ex: `katG_p.Lys100Gln -INH -note \"why\"`%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use `-var-file -` for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - variant gene
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Variant file options:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -gene <gene>: [Only if no gene in variant id]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o gene with the AMR%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o only required if the variant id does not%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      include the gene%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - AMRs
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -<drug>: [at least one]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o drugs this variant is resistant to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * `-amk` for amikacin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-bdq` for bedaquiline%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-cap` for capreomycin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-cfz` for clorfazimine%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-dlm` for delamanid%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-emb` for ethambutol%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-eto` for ethionamide%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-flq` for fluoroquine%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-inh` for isoniazid%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-kan` for kanamycin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-lfx` for levofloxacin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-lzd` for linezolid%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-mfx` for moxifloxacin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-pmd` for protonimid%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-pza` for pyrazinamide%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-rif` for rifampicin%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * `-stm` for streptomycin%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * `-other <drug>` for an unlisted drug%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub09:
   *   - comment entries
   *   o fun02 sec02 sub09 cat01:
   *     - grade
   *   o fun02 sec02 sub09 cat02:
   *     - high resistance
   *   o fun02 sec02 sub09 cat03:
   *     - low resistance
   *   o fun02 sec02 sub09 cat04:
   *     - additive resistance
   *   o fun02 sec02 sub09 cat05:
   *     - gene needed
   *   o fun02 sec02 sub09 cat06:
   *     - comment
   *   o fun02 sec02 sub09 cat07:
   *     - effect
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat01:
   +   - grade
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -grade-<1-5>: [Default `-grade-%i`]%s",
      def_grade_addAmr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o WHO grade (1 to 5) of new AMR%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat02:
   +   - high resistance
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_highRes_addAmr)
      fprintf(
         (FILE *) outFILE,
         "  -high-res: [Default; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -high-res: [Default; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o flag variant as high resistance%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-high-res` or `-low-res`%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat03:
   +   - low resistance
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_lowRes_addAmr)
      fprintf(
         (FILE *) outFILE,
         "  -low-res: [Default; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -low-res: [Default; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o flag variant as low resistance%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-low-res` or `-high-res`%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat04:
   +   - additive resistance
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_addRes_addAmr)
      fprintf(
         (FILE *) outFILE,
         "  -add-res: [Default; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -add-res: [Default; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o resistance increases with number of AMRs%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-add-res`%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat05:
   +   - gene needed
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -needs <gene>: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o AMR needs a <gene> to confir resistance%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat06:
   +   - comment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -note \"<note>\": [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o comment about AMR%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub09 Cat07:
   +   - effect
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -effect \"<effect of mutation>\": [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o effect of the AMR%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub10:
   *   - help and version number
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Help and version:%s",
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
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);
   fprintf(
      (FILE *) outFILE,
      "  - prints new database to stdout%s",
      str_endLine
   );
} /*phelp_mainAddAmr*/

/*-------------------------------------------------------\
| Fun03: input_mainAddAmr
|   - gets input from argument array provided to main
| Input:
|   - argLenSI:
|     o number of arguments in argAryStr
|   - argAryStr:
|     o c-string array with user arguments
|   - refStrPtr:
|     o c-string pointer to be set to reference fasta file
|   - coordsStrPtr:
|     o c-string pointer; set to gene coordinates file
|   - dbStrPtr:
|     o c-string pointer to be set to amr database file
|   - outStrPtr:
|     o c-string pointer to be set to output file
|   - varStrPtr:
|     o c-string pointer to be set to variant files
| Output:
|   - Modifies:
|     o everything but aryAryStr and argLenSI to have
|       user input
|       * c-strings are not copied, but instead point
|         to input, so DO NO FREE THEM
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 for an error
\-------------------------------------------------------*/
signed char
input_mainAddAmr(
   int argLenSI,               /*number arguments input*/
   char *argAryStr[],          /*has input arguments*/
   signed char **refStrPtr,    /*gets reference file*/
   signed char **coordsStrPtr, /*gene coordinates file*/
   signed char **dbStrPtr,     /*tbAmr database file*/
   signed char **outStrPtr,    /*output file*/
   signed char **varStrPtr     /*file with variants*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC
   '   - gets input from argument array provided to main
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec02:
   '     - get input
   '   o fun03 sec03:
   '     - return and print help/version (if requested)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;

   if(argLenSI <= 1)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - check file input + start loop
   ^   o fun03 sec02 sub02:
   ^     - check help message
   ^   o fun03 sec02 sub03:
   ^     - check version number
   ^   o fun03 sec02 sub04:
   ^     - handel invalid input
   ^   o fun03 sec02 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - check file input + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: reference file input*/
         ++siArg;
         *refStrPtr = (signed char *) argAryStr[siArg];
      }  /*If: reference file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-coords",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gene coordinate file input*/
         ++siArg;
         *coordsStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: gene coordiante file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-db",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: tbAmr database file input*/
         ++siArg;
         *dbStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: tbAmr database file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-var",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *varStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - check help message
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;
      
      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;
      
      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - check version number
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;
      
      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - handel invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recongnized%s",
            argAryStr[siArg],
            str_endLine
         );
         goto err_fun03_sec03;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return and print help/version (if requested)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainAddAmr(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainAddAmr(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_mainAddAmr*/

/*-------------------------------------------------------\
| Main:
|   - driver function to convert variants to database
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user arguments
| Output:
|   - Prints:
|     o datbase to stdout or `-out <database>.tsv`
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 for some kind of error
\-------------------------------------------------------*/
int
main(
   int argLenSI,     /*number of user arguments*/
   char *argAryStr[] /*users arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to convert variants to database
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, and process/check input
   '   o main sec04:
   '     - print new/modified database
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *refFileStr = 0;
   signed char *coordsFileStr = 0;
   signed char *dbFileStr = 0;
   signed char *outFileStr = 0;
   signed char *varFileStr = 0;

   signed char errSC = 0;

   struct seqST refStackST;

   struct geneCoord *coordHeapST = 0;
   signed int coordLenSI = 0;

   struct amrST *varAmrHeapST = 0;
   signed long errPosSL = 0; /*position of error in line*/
   unsigned long lineUL = 0; /*line on*/
  

   struct amrST *tmpAmrAryST = 0;
   struct amrST *dbAmrHeapAryST = 0;
   unsigned int dbAmrLenUI = 0;
   unsigned int dbAmrSizeUI = 0;
   unsigned int tmpUI = 0;

   /*store drugs in database*/
   signed char *drugHeapAryStr = 0;
   signed int drugLenSI = 0;
   signed int drugSizeSI = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and process/check input
   ^   o main sec02 sub01:
   ^     - initialize and get input
   ^   o main sec02 sub02:
   ^     - get reference sequence
   ^   o main sec02 sub03:
   ^     - get gene coordinates
   ^   o main sec02 sub04:
   ^     - get dabase file
   ^   o main sec02 sub05:
   ^     - open output file
   ^   o main sec02 sub06:
   ^     - open variant file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize and get input
   \*****************************************************/

   init_seqST(&refStackST);

   errSC =
      input_mainAddAmr(
         argLenSI,       /*number input arguments*/
         argAryStr,      /*input arguments*/
         &refFileStr,    /*gets reference file*/
         &coordsFileStr, /*gets gene coordinates file*/
         &dbFileStr,     /*gets old database to add to*/
         &outFileStr,    /*gets output file name*/
         &varFileStr     /*gets files with variants*/
      );

   if(errSC)
   { /*If: had error or exit condition*/
      --errSC;/*change help/version print to 0; no error*/
      goto ret_main_sec05;
   } /*If: had error or exit condition*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get reference sequence
   *   o main sec02 sub02 cat01:
   *     - open refernece file
   *   o main sec02 sub02 cat02:
   *     - get reference from file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub02 Cat01:
   +   - open refernece file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! refFileStr || *refFileStr == '-')
   { /*If: stdin input*/
      if(! dbFileStr)
         ; /*user not using database file*/
      else if(*dbFileStr == '-')
      { /*Else If: database and reference from stdin*/
         fprintf(
            stderr,
            "-ref and -db can not both be from stdin%s",
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: database and reference from stdin*/

      if(! varFileStr || *varFileStr == '-')
      { /*If: variants and reference from stdin*/
         fprintf(
            stderr,
            "-ref and -var can not both be from stdin%s",
            str_endLine
         );
         goto err_main_sec05;
      } /*If: variants and reference from stdin*/

      refFileStr = (signed char *) "-";
      inFILE = stdin;
   } /*If: stdin input*/

   else
   { /*Else: reference from file*/
      inFILE = fopen((char *) refFileStr, "r");
      if(! inFILE)
      { /*If: could not open reference file*/
         fprintf(
            stderr,
            "could not open -ref %s%s",
            refFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open reference file*/
   } /*Else: reference from file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub02 Cat02:
   +   - get reference from file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   errSC = getFa_seqST(inFILE, &refStackST);

   if(! errSC)
      ;
   else
   { /*Else: could not get reference sequence*/
      fprintf(
         stderr,
         "file error reading reference from -ref %s%s",
         refFileStr,
         str_endLine
      );
      goto err_main_sec05;
   } /*Else: could not get reference sequence*/

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - get gene coordinates
   \*****************************************************/

   coordHeapST =
      getCoords_geneCoord(
         coordsFileStr, /*file with coordinates*/
         &coordLenSI,   /*number of genes (coordianates)*/
         &lineUL        /*for errors, re-using variable*/
      );

   if(! lineUL)
      ;
   else if(lineUL & def_fileErr_geneCoord)
   { /*Else If: could not open the file*/
      fprintf(
         stderr,
         "could not open -coords %s%s",
         coordsFileStr,
         str_endLine
      );
      goto err_main_sec05;
   } /*Else If: could not open the file*/

   else if(lineUL & def_memErr_geneCoord)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "ran out of memory reaidng -coords %s%s",
         coordsFileStr,
         str_endLine
      );
      goto err_main_sec05;
   } /*Else If: memory error*/

   else
   { /*Else: invalid line*/
      fprintf(
         stderr,
         "problem reading line %lu in -coords %s%s",
         lineUL >> 8,
         coordsFileStr,
         str_endLine
      );
      goto err_main_sec05;
   } /*Else: invalid line*/

   sortName_geneCoord(coordHeapST, 0, coordLenSI - 1);
      /*need to sort by name because var_addAmr uses
      `   a binary search by name
      */

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - get dabase file
   \*****************************************************/

   if(dbFileStr)
   { /*If: adding to a database*/
      dbAmrHeapAryST =
         readTbl_amrST(
            dbFileStr,       /*database to read in*/
            &dbAmrLenUI,     /*AMRs in array/array size*/
            &drugHeapAryStr, /*gets drugs in database*/
            &drugLenSI,      /*number drugs read in*/
            &drugSizeSI,     /*size of drugHeapAryStr*/
            &errSC           /*gets errors*/
         );

      if(! errSC)
         ;
      else if(errSC == def_memErr_amrST)
      { /*Else If: memory error*/
         fprintf(
            stderr,
            "ran out of memor reading -db %s%s",
            dbFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: memory error*/

      else
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error reading -db %s%s",
            dbFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else: file error*/

      dbAmrSizeUI = dbAmrLenUI;
   } /*If: adding to a database*/

   else
   { /*Else: get some inital memory*/
      dbAmrHeapAryST = malloc(16 * sizeof(struct amrST));

      if(! dbAmrHeapAryST)
      { /*If: had memory error*/
         fprintf(
           stderr,
           "memory error setting up blank database%s",
           str_endLine
         );
         goto err_main_sec05;
      } /*If: had memory error*/

      for(
         dbAmrSizeUI = 0;
         dbAmrSizeUI < 16;
         ++dbAmrSizeUI
      ) init_amrST(&dbAmrHeapAryST[dbAmrSizeUI]);
   } /*Else: get some inital memory*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open output file
   \*****************************************************/

   if(! outFileStr)
      ; /*stdout*/
   else if(*outFileStr == '-')
      ; /*stdout*/
   else
   { /*Else: outputing to a file*/
      inFILE = fopen((char *) outFileStr, "a");

      if(! inFILE)
      { /*If: failed to open the file*/
         fprintf(
            stderr,
            "could not open -out %s%s",
            outFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: failed to open the file*/

      fclose(inFILE);
      inFILE = 0;
   } /*Else: outputing to a file*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open variant file
   \*****************************************************/

   if(! varFileStr || *varFileStr == '-')
   { /*If: getting variants from stdin*/
      inFILE = stdin;
      varFileStr = (signed char *) "-";
   } /*If: getting variants from stdin*/

   else
   { /*Else: getting variants from a file*/
      inFILE = fopen((char *) varFileStr, "r");

      if(! inFILE)
      { /*If: failed to open the file*/
         fprintf(
            stderr,
            "could not open -var %s%s",
            varFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: failed to open the file*/
   } /*Else: getting variants from a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - process variants
   ^   o main sec03 sub01:
   ^     - convert variant to database entry + start loop
   ^   o main sec03 sub02:
   ^     - check for errors in variant conversion
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - convert variant to database entry + start loop
   \*****************************************************/
   errSC = 0;

   while(! errSC)
   { /*Loop: convert variants to AMRs*/
      ++lineUL;

      varAmrHeapST =
         getVar_addAmr(
            inFILE,
            coordHeapST,
            coordLenSI,
            refStackST.seqStr,
            &drugHeapAryStr,
            &drugLenSI,
            &drugSizeSI,
            &errSC,
            &errPosSL
         ); /*convert one variant to an AMR*/

      /**************************************************\
      * Main Sec03 Sub02:
      *   - check for errors in variant conversion
      \**************************************************/

      if(! errSC)
         ;

      else if(errSC == def_eof_addAmr)
         continue; /*done*/

      else if(errSC == def_memErr_addAmr)
      { /*Else If: memory error*/
         fprintf(
            stderr,
            "ran out of memory on line %lu of -var %s%s",
            lineUL,
            varFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: memory error*/

      else if(errSC == def_overflow_addAmr)
      { /*Else If: exceded drug limit*/
         fprintf(
            stderr,
            "to many drugs in to database, max is %lu%s",
            (sizeof(unsigned long) << 3)
               * def_maxDrugs_amrST,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: exceded drug limit*/

      else if(errSC == def_noVar_addAmr)
      { /*Else If: blank line or empty id*/
         if(! errPosSL)
         { /*If: blank line*/
            errSC = 0;
            continue;
         } /*If: blank line*/

         else
         { /*Else: blank variant id*/
            fprintf(
               stderr,
               "could not process variant id on%s",
               str_endLine
            );

            fprintf(
               stderr,
               "  line %lu in -var %s%s",
               lineUL,
               varFileStr,
               str_endLine
            );
            goto err_main_sec05;
         } /*Else: blank variant id*/
      } /*Else If: blank line or empty id*/

      else
      { /*Else If: file error or variant error*/
            fprintf(
               stderr,
               "file error on line %lu, position %li%s",
               lineUL,
               errPosSL,
               str_endLine
            );

            fprintf(
               stderr,
               "  of -var %s%s",
               varFileStr,
               str_endLine
            );
            goto err_main_sec05;
      } /*Else If: file error or variant error*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - add variant to database array
      \**************************************************/

      if(dbAmrLenUI >= dbAmrSizeUI)
      { /*If: need a larger database*/
         dbAmrSizeUI += (dbAmrSizeUI >> 1);
         tmpAmrAryST =
            realloc(
               dbAmrHeapAryST,
               dbAmrSizeUI * sizeof(struct amrST)
            );
         if(! tmpAmrAryST)
         { /*If: memory error*/
            fprintf(
               stderr,
               "ran out of memory processing variants%s",
               str_endLine
            );
            goto err_main_sec05;
         } /*If: memory error*/

         dbAmrHeapAryST = tmpAmrAryST;
         tmpAmrAryST = 0;

         for(
            tmpUI = dbAmrLenUI;
            tmpUI < dbAmrSizeUI;
            ++tmpUI
         ) init_amrST(&dbAmrHeapAryST[tmpUI]);
      } /*If: need a larger database*/

      swap_amrST(
         &dbAmrHeapAryST[dbAmrLenUI++],
         varAmrHeapST
      ); /*add the new variant to the database*/

      freeHeap_amrST(varAmrHeapST);
      varAmrHeapST = 0;
   } /*Loop: convert variants to AMRs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print new/modified database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortPos_amrST(dbAmrHeapAryST, 0, dbAmrLenUI - 1);

   p_amrST(
      dbAmrHeapAryST,
      dbAmrLenUI,
      drugHeapAryStr,
      drugLenSI,
      outFileStr
   );
      /*already checked if could open the output file*/
           
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec05;

   err_main_sec05:;
      errSC = 1;
      goto ret_main_sec05;

   ret_main_sec05:;
      if(varAmrHeapST)
         freeHeap_amrST(varAmrHeapST);
      varAmrHeapST = 0;

      if(dbAmrHeapAryST)
         freeHeapAry_amrST(dbAmrHeapAryST, dbAmrLenUI);
      dbAmrHeapAryST = 0;
      dbAmrLenUI = 0;

      if(coordHeapST)
         freeHeap_geneCoord(coordHeapST);
      coordHeapST = 0;
      coordLenSI = 0;

      if(drugHeapAryStr)
         free(drugHeapAryStr);
      drugHeapAryStr = 0;
      drugLenSI = 0;
      drugSizeSI = 0;

      freeStack_seqST(&refStackST);

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else
         fclose(inFILE);
      inFILE = 0;

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
: Copyright (c) 2025 jeremyButtler
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
