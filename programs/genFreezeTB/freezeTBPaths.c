/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTBPaths SOF: Start Of File
'   - holds functions for ting default file paths for
'     freezeTB
'   o header:
'     - included libraries and defined variables
'   o fun01: checkPaths_freezeTBPaths
'     - checks/finds if input file exits in freezeTB paths
'   o fun02: amrPath_freezeTBPaths
'     - finds default AMR path for freezeTB
'   o fun03: miruPath_freezeTBPaths
'     - finds default MIRU table path for freezeTB
'   o fun04: coordPath_freezeTBPaths
'     - finds default gene coordinate table path; freezeTB
'   o fun05: spolSpacerPath_freezeTBPaths
'     - finds default fasta with spoligotype spacers path
'   o fun06: spolLineagePath_freezeTBPaths
'     - finds default spoligotype lineage path (freezeTB)
'   o fun07: maskPath_freezeTBPaths
'     - finds primer masking path
'   o fun08: refPath_freezeTBPaths
'     - finds default reference fasta path (guifreezeTB)
'   o fun09: outputPath_freezeTBPaths
'     - sets up an ouput file name & opens "w", the closes
'   o fun10: guiTclPath_freezeTBPaths
'     - path to tcl script for GUI
'   o fun11: hsp65SimpleDbPath_freezeTBPaths
'     - finds default hsp65 simple database file path
'   o fun12: hsp65ComplexDbPath_freezeTBPaths
'     - finds default hsp65 complex database file path
'   o license:
'     - licensing for this code (CC0)
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

#include "freezeTBPaths.h"
#include <stdio.h>

#include "../genLib/ulCp.h"


signed char
   *def_coordTbl_freezeTBPaths =
      (signed char *) "coords.tsv";

signed char
   *def_amrDb_freezeTBPaths =
      (signed char *) "amrDb.tsv";

signed char
   *def_miruTbl_freezeTBPaths =
      (signed char *) "miruTbl.tsv";

signed char
   *def_spolSpacer_freezeTBPaths =
      (signed char *) "spoligo-spacers.fa";

signed char
   *def_spolLineage_freezeTBPaths =
      (signed char *) "spoligo-lineages.csv";

signed char
   *def_mask_freezeTBPaths =
      (signed char *) "mask.tsv";

signed char
   *def_ref_freezeTBPaths =
      (signed char *) "NC000962.fa";

signed char
   *def_hsp65SimpleDb_freezeTBPaths =
      (signed char *) "hsp65-db-simple.tsv";

signed char
   *def_hsp65ComplexDb_freezeTBPaths =
      (signed char *) "hsp65-db-complex.tsv";

signed char
   *def_tclGui_freezeTBPaths =
      (signed char *) "gui-FTB.tcl";

/*-------------------------------------------------------\
| Fun01: checkPaths_freezeTBPaths
|   - checks/finds if input file exits in freezeTB paths
| Input:
|   - pathStr:
|     o c-string to hold final path
|   - fileStr:
|     o c-string with file name
| Output:
|   - Modifies:
|     o pathStr to have detected path/file name
|     o pathStr to be '\0' if no path detected
\-------------------------------------------------------*/
void
checkPaths_freezeTBPaths(
   signed char *pathStr, /*holds final path*/
   signed char *fileStr  /*file to search for*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - checks/finds if input file exits in freezeTB path
   '   o fun01 sec02:
   '     - see if database in working directory
   '   o fun01 sec03:
   '     - if not, check local documents folder
   '   o fun01 sec04:
   '     - if not, check global share/documents
   '   o fun01 sec05:
   '     - windows check local install (appData)
   '   o fun01 sec06:
   '     - windows check global install; program
   '   o fun01 sec07:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   signed char *tmpStr = 0;

   #ifdef WINDOWS
      signed char *dbDirStr =
         (signed char *) "\\freezeTBFiles\\";
      signed char *altDbDirStr =
         (signed char *) "\\ftbResources\\";
      signed char *installDirStr =
         (signed char *) "\\freezeTB\\";

      signed char *sharePathStr =
         (signed char *) getenv("PUBLIC");
      signed char *homePathStr =
         (signed char *) getenv("HOMEPATH");
      signed char *appDataStr =
         (signed char *) getenv("LOCALAPPDATA");
      signed char *programStr =
         (signed char *) getenv("PROGRAMFILES");

      signed char *docStr =
         (signed char *) "\\Documents\\";
      signed char *altPathStr =
         (signed char *) "\\Documents\\";
   #else
      signed char *dbDirStr =
         (signed char *) "/freezeTBFiles/";
      signed char *altDbDirStr =
         (signed char *) "/ftbResources/";

      signed char *homePathStr =
         (signed char *) getenv("HOME");
      signed char *sharePathStr = 
         (signed char *) "/usr/local/";

      signed char *docStr =
         (signed char *) "/Documents/";
      signed char *altPathStr =
         (signed char *) "/share/";

      signed char *localInstallStr =
         (signed char *) "/.ftb/";
   #endif

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = pathStr;
   cpStr_ulCp(tmpStr, fileStr);
   testFILE = fopen((char *) tmpStr, "r");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   checkDatabaseDir_fun01_sec03:;

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = pathStr;

      tmpStr += cpStr_ulCp(tmpStr, homePathStr);
      tmpStr += cpStr_ulCp(tmpStr, docStr);
      tmpStr += cpStr_ulCp(tmpStr, dbDirStr);
      tmpStr += cpStr_ulCp(tmpStr, fileStr);

      testFILE = fopen((char *) pathStr, "r");

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun01 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = pathStr;

         tmpStr += cpStr_ulCp(tmpStr, sharePathStr);
         tmpStr += cpStr_ulCp(tmpStr, altPathStr);
         tmpStr += cpStr_ulCp(tmpStr, dbDirStr);
         tmpStr += cpStr_ulCp(tmpStr, fileStr);

         testFILE = fopen((char *) pathStr, "r");

         #ifndef WINDOWS
            if(! testFILE)
            { /*If: not in global path*/
               /*the local install hidden path*/
               tmpStr = pathStr;

               tmpStr += cpStr_ulCp(tmpStr, homePathStr);
               tmpStr +=
                  cpStr_ulCp(tmpStr, localInstallStr);
               tmpStr += cpStr_ulCp(tmpStr, dbDirStr);
               tmpStr += cpStr_ulCp(tmpStr, fileStr);

               testFILE = fopen((char *) pathStr, "r");

                if(! testFILE)
                { /*If: could not find the file*/
                   if(dbDirStr == altDbDirStr)
                      pathStr[0] = '\0';
                      /*no idea were at*/ 
                   else
                   { /*Else: can check alternate paths*/
                      dbDirStr = altDbDirStr;
                      goto checkDatabaseDir_fun01_sec03;
                   } /*Else: can check alternate paths*/
                } /*If: could not find the file*/
            } /*If: not in global path*/

            /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
            ^ Fun01 Sec05:
            ^   - windows check local install (appData)
            \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
         #else
            if(! testFILE)
            { /*If: database not in global path*/
               tmpStr = pathStr;
               tmpStr += cpStr_ulCp(tmpStr, appDataStr);
               tmpStr += cpStr_ulCp(tmpStr,installDirStr);
               tmpStr += cpStr_ulCp(tmpStr, dbDirStr);
               tmpStr += cpStr_ulCp(tmpStr, fileStr);

               testFILE = fopen((char *) pathStr, "r");

               /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
               ^ Fun01 Sec06:
               ^   - windows check global install; program
               \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

               if(! testFILE)
               { /*If: database not in local install*/
                  tmpStr = pathStr;

                  tmpStr += cpStr_ulCp(tmpStr,programStr);
                  tmpStr +=
                     cpStr_ulCp(tmpStr, installDirStr);
                  tmpStr += cpStr_ulCp(tmpStr, dbDirStr);
                  tmpStr += cpStr_ulCp(tmpStr, fileStr);

                  testFILE = fopen((char *) pathStr, "r");

                  if(! testFILE)
                  { /*If: could not find the databse*/
                     if(dbDirStr == altDbDirStr)
                        pathStr[0] = '\0';
                        /*no idea were at*/ 
                     else
                     { /*Else: can check alternate paths*/
                        dbDirStr = altDbDirStr;
                        goto checkDatabaseDir_fun01_sec03;
                     } /*Else: can check alternate paths*/
                  } /*If: could not find the databse*/
               } /*If: database not in local install*/
            } /*If: database not in global path*/
         #endif
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec07:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*checkPaths_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun02: amrPath_freezeTBPaths
|   - finds default AMR path for freezeTB
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
amrPath_freezeTBPaths(
   signed char *amrPathStr
){
   checkPaths_freezeTBPaths(
      amrPathStr,
      def_amrDb_freezeTBPaths
   );
} /*amrPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun03: miruPath_freezeTBPaths
|   - finds default MIRU table path for freezeTB
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
miruPath_freezeTBPaths(
   signed char *miruPathStr
){
   checkPaths_freezeTBPaths(
      miruPathStr,
      def_miruTbl_freezeTBPaths
   );
} /*miruPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun04: coordPath_freezeTBPaths
|   - finds default gene coordinates table path (freezeTB)
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
coordPath_freezeTBPaths(
   signed char *coordPathStr
){
   checkPaths_freezeTBPaths(
      coordPathStr,
      def_coordTbl_freezeTBPaths
   );
} /*coordPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun05: spolSpacerPath_freezeTBPaths
|   - finds default fasta with spoligotype spacers path
|     (freezeTB)
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
spolSpacerPath_freezeTBPaths(
   signed char *spolSpacerPathStr
){
   checkPaths_freezeTBPaths(
      spolSpacerPathStr,
      def_spolSpacer_freezeTBPaths
   );
} /*spolSpacerPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun06: spolLineagePath_freezeTBPaths
|   - finds default spoligotype lineage path (freezeTB)
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
spolLineagePath_freezeTBPaths(
   signed char *spolLineagePathStr
){
   checkPaths_freezeTBPaths(
      spolLineagePathStr,
      def_spolLineage_freezeTBPaths
   );
} /*spolLineagePath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun07: maskPath_freezeTBPaths
|   - finds primer masking path
| Input:
|   - amrPathStr:
|     o c-string to copy default path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
maskPath_freezeTBPaths(
   signed char *maskPathStr
){
   checkPaths_freezeTBPaths(
      maskPathStr,
      def_mask_freezeTBPaths
   );
} /*maskPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun08: refPath_freezeTBPaths
|   - finds default reference fasta path (guifreezeTB)
| Input:
|   - refPathStr:
|     o c-string to copy default reference path to
| Output:
|   - Modifies:
|     o reference to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
refPath_freezeTBPaths(
   signed char *refPathStr
){
   checkPaths_freezeTBPaths(
      refPathStr,
      def_ref_freezeTBPaths
   );
} /*refPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun09: guiTclPath_freezeTBPaths
|   - path to tcl script for GUI
| Input:
|   - guiPathStr:
|     o c-string to add tcl script path to
| Output:
|   - Modifies:
|     o guiPathStr to have the default path or '\0' if
|       could not find file
|   - Returns:
|     o 0 for no errors
|     o 1 if could not open file
\-------------------------------------------------------*/
signed char
tclGuiPath_freezeTBPaths(
   signed char *guiPathStr
){
   checkPaths_freezeTBPaths(
      guiPathStr,
      def_tclGui_freezeTBPaths
   );

   if(guiPathStr[0] == '\0')
      return 1;
   else
      return 0;
} /*tclGuiPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun10: outputPath_freezeTBPaths
|   - sets up an ouput file name and opens "w", the closes
| Input:
|   - prefixStr:
|     o c-string with prefix to add to output file name
|   - nameStr:
|     o c-string with name of file to output
|   - outStr:
|     o c-string to hold output file name
| Output:
|   - Modifies:
|     o outStr to have new file name
|   - Returns:
|     o 0 for no errors
|     o 1 if could not open output file
\-------------------------------------------------------*/
signed char
outputPath_freezeTBPaths(
   signed char *prefixStr,
   signed char *nameStr,
   signed char *outStr
){
   signed char *tmpStr = outStr;
   FILE *testFILE = 0;

   tmpStr += cpStr_ulCp(tmpStr, prefixStr);
   tmpStr += cpStr_ulCp(tmpStr, nameStr);
   testFILE = fopen((char *) outStr, "w");

   if(! testFILE)
      return 1;
   fclose(testFILE);
   return 0;
} /*outputPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun11: hsp65SimpleDbPath_freezeTBPaths
|   - finds default hsp65 simple database file path
| Input:
|   - pathStr:
|     o c-string to copy default hsp65 simple database
|       path to
| Output:
|   - Modifies:
|     o pathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
hsp65SimpleDbPath_freezeTBPaths(
   signed char *pathStr
){
   checkPaths_freezeTBPaths(
      pathStr,
      def_hsp65SimpleDb_freezeTBPaths
   );
} /*hsp65SimpleDbPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun12: hsp65ComplexDbPath_freezeTBPaths
|   - finds default hsp65 complex database file path
| Input:
|   - pathStr:
|     o c-string to copy default hsp65 complex database
|       path to
| Output:
|   - Modifies:
|     o pathStr to have the default path or '\0' if
|       could not find file
\-------------------------------------------------------*/
void
hsp65ComplexDbPath_freezeTBPaths(
   signed char *pathStr
){
   checkPaths_freezeTBPaths(
      pathStr,
      def_hsp65ComplexDb_freezeTBPaths
   );
} /*hsp65ComplexDbPath_freezeTBPaths*/

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
