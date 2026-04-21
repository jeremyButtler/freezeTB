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
'     - licensing for this code (CC0)
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
#include "../genAmr/amrST.h"
#include "../genAmr/addAmr.h"

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
      "addAmr from ftbVersion version: %i-%02i-%02i%s",
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
   ^   o fun02 sec02 sub02:
   ^     - gene coordinates file
   ^   o fun02 sec02 sub03:
   ^     - reference
   ^   o fun02 sec02 sub04:
   ^     - database to add to
   ^   o fun02 sec02 sub05:
   ^     - output file
   ^   o fun02 sec02 sub06:
   ^     - file variant id input
   ^   o fun02 sec02 sub07:
   ^     - file variant id for the amr
   ^   o fun02 sec02 sub08:
   ^     - file reference id input
   ^   o fun02 sec02 sub09:
   ^     - gene the variant belongs to
   ^   o fun02 sec02 sub10:
   ^     - drug resistances
   ^   o fun02 sec02 sub11:
   ^     - comment entries
   ^   o fun02 sec02 sub12:
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
   *   - file variant id for the amr
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Variant file options:%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "  -var <var_id>: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o variant id for the AMR%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - file reference id input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref <ref_id>: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o reference id of the AMR is from%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub09:
   *   - gene the variant belongs to
   \*****************************************************/

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
   * Fun02 Sec02 Sub10:
   *   - drug resistances
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
   * Fun02 Sec02 Sub11:
   *   - comment entries
   *   o fun02 sec02 sub11 cat01:
   *     - grade
   *   o fun02 sec02 sub11 cat02:
   *     - high resistance
   *   o fun02 sec02 sub11 cat03:
   *     - low resistance
   *   o fun02 sec02 sub11 cat04:
   *     - additive resistance
   *   o fun02 sec02 sub11 cat05:
   *     - gene needed
   *   o fun02 sec02 sub11 cat06:
   *     - comment
   *   o fun02 sec02 sub11 cat07:
   *     - effect
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub11 Cat01:
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
   + Fun02 Sec02 Sub11 Cat02:
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
   + Fun02 Sec02 Sub11 Cat03:
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
   + Fun02 Sec02 Sub11 Cat04:
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
   + Fun02 Sec02 Sub11 Cat05:
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
   + Fun02 Sec02 Sub11 Cat06:
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
   + Fun02 Sec02 Sub11 Cat07:
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
   * Fun02 Sec02 Sub12:
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

   struct seqST *refSeqHeapAryST = 0;
   signed long refSeqLenSL = 0;
   signed long refSeqSizeSL = 0;
   signed int refIndexSI = 0;

   struct geneCoord *coordHeapST = 0;
   struct amrST *varAmrHeapST = 0;

   signed long errPosSL = 0; /*position of error in line*/
   unsigned long lineUL = 0; /*line on*/
  
   struct refList_amrST *refAmrHeapSTPtr = 0;

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

   refSeqHeapAryST =
      readFaFile_seqST(
         inFILE,
         &refSeqLenSL,
         &refSeqSizeSL,
         &errSC
      ); /*get the reference sequences*/

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

   for(
      refIndexSI = 0;
      refIndexSI < refSeqLenSL;
      ++refIndexSI
   ){ /*Loop: trim at frist white space in ref id*/
      lineUL =
         endWhite_ulCp(
            refSeqHeapAryST[refIndexSI].idStr
         );
     refSeqHeapAryST[refIndexSI].idStr[lineUL] = 0;
   }  /*Loop: trim at frist white space in ref id*/

   lineUL = 0;
   refIndexSI = 0;
   sort_seqST(refSeqHeapAryST, refSeqLenSL);

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - get gene coordinates
   \*****************************************************/

   coordHeapST =
      getCoords_geneCoord(coordsFileStr, &lineUL);

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

   sortName_geneCoord(coordHeapST);
      /*need to sort by name because var_addAmr uses
      `   a binary search by name
      */

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - get dabase file
   \*****************************************************/

   if(dbFileStr)
   { /*If: adding to a database*/
      refAmrHeapSTPtr = readTbl_amrST(dbFileStr, &errSC);

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
   } /*If: adding to a database*/

   else
   { /*Else: get some inital memory*/
      refAmrHeapSTPtr =
         malloc(sizeof(struct refList_amrST));

      if(! refAmrHeapSTPtr)
      { /*If: had memory error*/
         fprintf(
           stderr,
           "memory error setting up blank database%s",
           str_endLine
         );
         goto err_main_sec05;
      } /*If: had memory error*/

      init_refList_amrST(refAmrHeapSTPtr);
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
            &refIndexSI, /*gets index of input reference*/
            coordHeapST,
            refSeqHeapAryST,
            refSeqLenSL,
            &refAmrHeapSTPtr->drugAryStr,
            &refAmrHeapSTPtr->drugCntSI,
            &refAmrHeapSTPtr->drugSizeSI,
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

      else if(errSC == def_noRef_addAmr)
      { /*Else If: no reference was input*/
         fprintf(
            stderr,
            "no ref or missing reference for line %lu%s",
            lineUL,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: no reference was input*/

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

      if(
         amrAdd_refList_amrST(
            varAmrHeapST,
            refSeqHeapAryST[refIndexSI].idStr,
            refAmrHeapSTPtr
         ) > 1
      ) goto err_main_sec05;

      freeHeap_amrST(varAmrHeapST);
      varAmrHeapST = 0;
   } /*Loop: convert variants to AMRs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print new/modified database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortPos_refList_amrST(refAmrHeapSTPtr);
   p_amrST(refAmrHeapSTPtr, outFileStr);
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
      if(refSeqHeapAryST)
         freeHeapAry_seqST(refSeqHeapAryST, refSeqLenSL);
      refSeqHeapAryST = 0;

      if(varAmrHeapST)
         freeHeap_amrST(varAmrHeapST);
      varAmrHeapST = 0;

      if(refAmrHeapSTPtr)
         freeHeap_refList_amrST(refAmrHeapSTPtr);
      refAmrHeapSTPtr = 0;

      if(coordHeapST)
         freeHeap_geneCoord(coordHeapST);
      coordHeapST = 0;

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else fclose(inFILE);
      inFILE = 0;

      return errSC;
} /*main*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
