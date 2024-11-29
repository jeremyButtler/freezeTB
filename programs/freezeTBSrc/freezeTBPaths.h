/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTBPaths SOF: Start Of File
'   - holds functions for ting default file paths for
'     freezeTB
'   o header:
'     - guards
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
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef FREEZETB_DEFAULT_FILE_PATHS_H
#define FREEZETB_DEFAULT_FILE_PATHS_H

/*-------------------------------------------------------\
| Fun03: amrPath_freezeTBPaths
|   - finds default AMR path for freezeTB
| Input:
|   - amrPathStr:
|     o c-string to copy default amr path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not detected amr database
\-------------------------------------------------------*/
void
amrPath_freezeTBPaths(
   signed char *amrPathStr
);

/*-------------------------------------------------------\
| Fun04: miruPath_freezeTBPaths
|   - finds default MIRU table path for freezeTB
| Input:
|   - amrPathStr:
|     o c-string to copy default path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not detected amr database
\-------------------------------------------------------*/
void
miruPath_freezeTBPaths(
   signed char *miruPathStr
);

/*-------------------------------------------------------\
| Fun05: coordPath_freezeTBPaths
|   - finds default gene coordinates table path (freezeTB)
| Input:
|   - amrPathStr:
|     o c-string to copy default path to
| Output:
|   - Modifies:
|     o amrPathStr to have the default path or '\0' if
|       could not detected amr database
\-------------------------------------------------------*/
void
coordPath_freezeTBPaths(
   signed char *coordPathStr
);

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
);

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
);

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
);

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
);

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
);

#endif
