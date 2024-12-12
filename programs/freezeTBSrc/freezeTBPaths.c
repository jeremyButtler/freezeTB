/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTBPaths SOF: Start Of File
'   - holds functions for ting default file paths for
'     freezeTB
'   o header:
'     - included libraries and defined variables
'   o .c fun01: getSharePath_freezeTBPaths
'     - returns shared path for OS
'   o .c fun02: getHomePath_freezeTBPaths
'     - returns home path for OS
'   o fun03: amrPath_freezeTBPaths
'     - finds default AMR path for freezeTB
'   o fun04: miruPath_freezeTBPaths
'     - finds default MIRU table path for freezeTB
'   o fun05: coordPath_freezeTBPaths
'     - finds default gene coordinate table path; freezeTB
'   o fun06: spolSpacerPath_freezeTBPaths
'     - finds default fasta with spoligotype spacers path
'   o fun07: spolLineagePath_freezeTBPaths
'     - finds default spoligotype lineage path (freezeTB)
'   o fun08: maskPath_freezeTBPaths
'     - finds primer masking path
'   o fun09: refPath_freezeTBPaths
'     - finds default reference fasta path (guifreezeTB)
'   o fun10: outputPath_freezeTBPaths
'     - sets up an ouput file name & opens "w", the closes
'   o fun11: guiTclPath_freezeTBPaths
'     - path to tcl script for GUI
'   o license:
'     - licensing for this code (public dofun04 / mit)
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

#include "../genLib/ulCp.h"

#include "freezeTBPaths.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"


#ifdef WINDOWS
   signed char
      *def_path_freezeTBPaths =
         (schar *) "\\Documents\\freezeTBFiles\\";

   signed char
      *def_altPath_freezeTBPaths =
         (schar *) "\\Documents\\freezeTBFiles\\";
#else
   signed char
      *def_path_freezeTBPaths =
         (schar *) "/Documents/freezeTBFiles/";

   signed char
      *def_altPath_freezeTBPaths =
         (schar *) "/share/freezeTBFiles/";
#endif

signed char
   *def_coordTbl_freezeTBPaths =
      (schar *) "coords.tsv";

signed char
   *def_amrDb_freezeTBPaths =
      (schar *) "amrDb.tsv";

signed char
   *def_miruTbl_freezeTBPaths =
      (schar *) "miruTbl.tsv";

signed char
   *def_spolSpacer_freezeTBPaths =
      (schar *) "spoligo-spacers.fa";

signed char
   *def_spolLineage_freezeTBPaths =
      (schar *) "spoligo-lineages.csv";

signed char
   *def_mask_freezeTBPaths =
      (schar *) "mask.tsv";

signed char
   *def_ref_freezeTBPaths =
      (schar *) "NC000962.fa";

signed char
   *def_tclGui_freezeTBPaths =
      (schar *) "gui-FTB.tcl";

/*-------------------------------------------------------\
| Fun01: getSharePath_freezeTBPaths
|   - returns shared path for OS
| Input:
| Output:
|   - Returns:
|     o pointer to shared path (do not free)
\-------------------------------------------------------*/
signed char *
getSharePath_freezeTBPaths(
){
   #ifdef WINDOWS
      return (schar *) getenv("PUBLIC");
   #else
      return (schar *) "/usr/local";
   #endif
} /*getSharePath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun02: getHomePath_freezeTBPaths
|   - returns home path for OS
| Input:
| Output:
|   - Returns:
|     o pointer to home path (do not free)
\-------------------------------------------------------*/
signed char *
getHomePath_freezeTBPaths(
){
   #ifdef WINDOWS
      return (schar *) getenv("HOMEPATH");
   #else
      return (schar *) getenv("HOME");
   #endif
} /*getHomePath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun03: amrPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - see if database in working directory
   '   o fun03 sec03:
   '     - if not, check local documents folder
   '   o fun03 sec04:
   '     - if not, check global share/documents
   '   o fun03 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = amrPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_amrDb_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = amrPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_amrDb_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) amrPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun03 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = amrPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_amrDb_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) amrPathStr,
               "r"
            );

         if(! testFILE)
            amrPathStr[0] = '\0'; /*no idea were at*/ 
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*amrPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun04: miruPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - see if database in working directory
   '   o fun04 sec03:
   '     - if not, check local documents folder
   '   o fun04 sec04:
   '     - if not, check global share/documents
   '   o fun04 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = miruPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_miruTbl_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = miruPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_miruTbl_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) miruPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun04 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = miruPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_miruTbl_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) miruPathStr,
               "r"
            );

         if(! testFILE)
            miruPathStr[0] = '\0'; /*no idea were at*/ 
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*miruPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun05: coordPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - see if database in working directory
   '   o fun05 sec03:
   '     - if not, check local documents folder
   '   o fun05 sec04:
   '     - if not, check global share/documents
   '   o fun05 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = coordPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_coordTbl_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = coordPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_coordTbl_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) coordPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun05 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = coordPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_coordTbl_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) coordPathStr,
               "r"
            );

         if(! testFILE)
            coordPathStr[0] = '\0'; /*no idea were at*/ 
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*coordPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun06: spolSpacerPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun06 sec01:
   '     - variable declarations
   '   o fun06 sec02:
   '     - see if database in working directory
   '   o fun06 sec03:
   '     - if not, check local documents folder
   '   o fun06 sec04:
   '     - if not, check global share/documents
   '   o fun06 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = spolSpacerPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_spolSpacer_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = spolSpacerPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_spolSpacer_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) spolSpacerPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun06 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = spolSpacerPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_spolSpacer_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) spolSpacerPathStr,
               "r"
            );

         if(! testFILE)
            spolSpacerPathStr[0] = '\0'; /*no idea were at*/ 
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*spolSpacerPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun07: spolLineagePath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - see if database in working directory
   '   o fun07 sec03:
   '     - if not, check local documents folder
   '   o fun07 sec04:
   '     - if not, check global share/documents
   '   o fun07 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = spolLineagePathStr;

   cpDelim_ulCp(
      tmpStr,
      def_spolLineage_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = spolLineagePathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_spolLineage_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) spolLineagePathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun07 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = spolLineagePathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_spolLineage_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) spolLineagePathStr,
               "r"
            );

         if(! testFILE)
            spolLineagePathStr[0] = '\0'; /*no idea*/
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*spolLineagePath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun08: maskPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - see if database in working directory
   '   o fun08 sec03:
   '     - if not, check local documents folder
   '   o fun08 sec04:
   '     - if not, check global share/documents
   '   o fun08 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = maskPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_mask_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = maskPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_mask_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) maskPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun08 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = maskPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_mask_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) maskPathStr,
               "r"
            );

         if(! testFILE)
            maskPathStr[0] = '\0'; /*no idea*/
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*maskPath_freezeTBPaths*/

/*-------------------------------------------------------\
| Fun09: refPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - see if database in working directory
   '   o fun09 sec03:
   '     - if not, check local documents folder
   '   o fun09 sec04:
   '     - if not, check global share/documents
   '   o fun09 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = refPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_ref_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = refPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_ref_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) refPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun09 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = refPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_ref_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) refPathStr,
               "r"
            );

         if(! testFILE)
            refPathStr[0] = '\0'; /*no idea*/
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;
} /*refPath_freezeTBPaths*/

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
   schar *tmpStr = outStr;
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

/*-------------------------------------------------------\
| Fun11: guiTclPath_freezeTBPaths
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - path to tcl script for GUI
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - see if database in working directory
   '   o fun09 sec03:
   '     - if not, check local documents folder
   '   o fun09 sec04:
   '     - if not, check global share/documents
   '   o fun09 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   schar *sharePathStr = getSharePath_freezeTBPaths();
   schar *homePathStr = getHomePath_freezeTBPaths();

   FILE *testFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - see if database in working directory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = guiPathStr;

   cpDelim_ulCp(
      tmpStr,
      def_tclGui_freezeTBPaths,
      0,
      '\0'
   );

   testFILE =
      fopen(
         (char *) tmpStr,
         "r"
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - if not, check local documents folder
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! testFILE)
   { /*If: database not in current directory*/
      tmpStr = guiPathStr;

      tmpStr +=
         cpDelim_ulCp(
           tmpStr,
           homePathStr,
           0,
           '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_path_freezeTBPaths,
            0,
            '\0'
         );

      tmpStr +=
         cpDelim_ulCp(
            tmpStr,
            def_tclGui_freezeTBPaths,
            0,
            '\0'
         );

      testFILE =
         fopen(
            (char *) guiPathStr,
            "r"
         );

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun09 Sec04:
      ^   - if not, check global share/documents
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(! testFILE)
      { /*If: database not in $HOME/documents*/
         tmpStr = guiPathStr;

         tmpStr +=
            cpDelim_ulCp(
              tmpStr,
              sharePathStr,
              0,
              '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_altPath_freezeTBPaths,
               0,
               '\0'
            );

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               def_tclGui_freezeTBPaths,
               0,
               '\0'
            );

         testFILE =
            fopen(
               (char *) guiPathStr,
               "r"
            );

         if(! testFILE)
            goto err_fun09_sec05;
      } /*If: database not in $HOME/documents*/
   } /*If: database not in current directory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(testFILE)
      fclose(testFILE);

   testFILE = 0;

   return 0;

   err_fun09_sec05:;
      guiPathStr[0] = '\0'; /*no idea*/
      return 1;
} /*tclGuiPath_freezeTBPaths*/

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
