/*########################################################
# Name: primMask-input
#   - get input the user provided by commandline and print
#     help message
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header
'     - defined variables and guards
'   o .c fun-01: checkIfHelp
'     - Checks if input paramter is help message request
'   o .c fun-02: checkIfVersion
'     - Checks if input paramter is an version request
'   o fun-03: getInputPrimMask
'     - processes user command line supplied input
'   o fun-04: pHelpPrimMask
'     - print the help message for primMask
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef PRIMER_MASK_INPUT_H
#define PRIMER_MASK_INPUT_H

#define def_help_primMask 1
#define def_version_primMask 2
#define def_nonNumeric_primMask 4
#define def_unkownInput_primMask 8

/*-------------------------------------------------------\
| Fun-03: getInputPrimMask
|   - processes user command line supplied input
| Input:
|   - numArgsUI:
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
|     o all input values except numArgsUI and argAryStr
|       to the user input.
|   - Returns:
|     o 0 for no errors
|     o def_help_primMask if help message requested
|     o def_version_primMask if version number requested
|     o def_nonNumeric | (arg_index << 8) for numeric
|       errors
|     o def_unkownInput | (arg_index << 8) if an argument
|       could not be recongnized
\-------------------------------------------------------*/
signed long
getInputPrimMask(
   unsigned int numArgsUI,
   char *argAryStr[],
   signed char **samFileStr,
   signed char **primFileStr,
   signed char **outFileStr,
   signed char *maskSC,
   signed int *fudgeSI,
   signed char *filterBl
);

/*-------------------------------------------------------\
| Fun-04: pHelpPrimMask
|   - print the help message for primMask
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints:
|     o the help messge to outFILE
\-------------------------------------------------------*/
void
pHelpPrimMask(
   void *outFILE
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
