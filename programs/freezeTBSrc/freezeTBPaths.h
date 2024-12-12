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
'   o fun11: guiTclPath_freezeTBPaths
'     - path to tcl script for GUI
'   o license:
'     - licensing for this code (public dofun04 / mit)
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
);

#endif

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
