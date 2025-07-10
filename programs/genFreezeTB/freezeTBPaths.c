/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTBPaths SOF: Start Of File
'   - holds functions for ting default file paths for
'     freezeTB
'   o header:
'     - included libraries and defined variables
'   o .c fun01: checkPaths_freezeTBPaths
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
'   o license:
'     - licensing for this code (public dofun03 / mit)
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

   cpStr_ulCp(
      tmpStr,
      fileStr
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = pathStr;

      tmpStr +=
         cpStr_ulCp(
           tmpStr,
           homePathStr
         );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            docStr
         );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            dbDirStr
         );

      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            fileStr
         );

      testFILE =
         fopen(
            (char *) pathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun01 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = pathStr;

         tmpStr +=
            cpStr_ulCp(
              tmpStr,
              sharePathStr
            );

         tmpStr +=
            cpStr_ulCp(
               tmpStr,
               altPathStr
            );

         tmpStr +=
            cpStr_ulCp(
               tmpStr,
               dbDirStr
            );

         tmpStr +=
            cpStr_ulCp(
               tmpStr,
               fileStr
            );

         testFILE =
            fopen(
               (char *) pathStr,
               "r"
            );

         #ifndef WINDOWS
            if(! testFILE)
            { /*If: not in global path*/
               /*the local install hidden path*/
               tmpStr = pathStr;

               tmpStr +=
                  cpStr_ulCp(
                    tmpStr,
                    homePathStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     localInstallStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     dbDirStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     fileStr
                  );

               testFILE =
                  fopen(
                     (char *) pathStr,
                     "r"
                  );

                if(! testFILE)
                  pathStr[0] = '\0'; /*no idea were at*/ 
            } /*If: not in global path*/

            /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
            ^ Fun01 Sec05:
            ^   - windows check local install (appData)
            \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
         #else
            if(! testFILE)
            { /*If: database not in global path*/
               tmpStr = pathStr;

               tmpStr +=
                  cpStr_ulCp(
                    tmpStr,
                    appDataStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                    tmpStr,
                    installDirStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     dbDirStr
                  );

               tmpStr +=
                  cpStr_ulCp(
                     tmpStr,
                     fileStr
                  );

               testFILE =
                  fopen(
                     (char *) pathStr,
                     "r"
                  );

               /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
               ^ Fun01 Sec06:
               ^   - windows check global install; program
               \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

               if(! testFILE)
               { /*If: database not in local install*/
                  tmpStr = pathStr;

                  tmpStr +=
                     cpStr_ulCp(
                       tmpStr,
                       programStr
                     );

                  tmpStr +=
                     cpStr_ulCp(
                       tmpStr,
                       installDirStr
                     );

                  tmpStr +=
                     cpStr_ulCp(
                        tmpStr,
                        dbDirStr
                     );

                  tmpStr +=
                     cpStr_ulCp(
                        tmpStr,
                        fileStr
                     );

                  testFILE =
                     fopen(
                        (char *) pathStr,
                        "r"
                     );

                  if(! testFILE)
                     pathStr[0] = '\0';/*no idea were at*/ 
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

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         prefixStr,
         0,
         '\0'
      );

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         nameStr,
         0,
         '\0'
      );

   testFILE =
      fopen(
         (char *) outStr,
         "w"
      );

   if(! testFILE)
      return 1;

   fclose(testFILE);
   return 0;
} /*outputPath_freezeTBPaths*/

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
