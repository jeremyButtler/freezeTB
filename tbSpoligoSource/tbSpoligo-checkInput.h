/*########################################################
# Name: tbSpoligo-checkInput
#   - checks if the users input is valid and updates the
#     correct values
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards and defined varibles
'   o fun-01: checkInput_tbSpoligo
'     - checks the users input & sets to correct variable
'   o fun-02: phelp_tbSpoligo
'     - prints the help messge for tbSpoligo
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables
\-------------------------------------------------------*/

#ifndef TB_SPOLIGOTYPING_CHECK_INPUT_H
#define TB_SPOLIGOTYPING_CHECK_INPUT_H

#define def_phelp_tbSpoligo 2
#define def_pversion_tbSpoligo 4
#define def_invalidErr_tbSpoligo 8
#define def_numericErr_tbSpoligo 16

#define def_fqFile_tbSpoligo 1
#define def_faFile_tbSpoligo 2
#define def_samFile_tbSpoligo 4
#define def_samConFile_tbSpoligo 8
#define def_seqStdin_tbSpoligo def_samFile_tbSpoligo

/*-------------------------------------------------------\
| Fun-01: checkInput_tbSpoligo
|   - checks the users input and sets to correct variable
| Input:
|   o parmStr:
|     - C-string with the paramter to check
|   o argStr:
|     - C-string with the argument to check
|   o seqFileStr:
|     - Pointer to c-string to hold the sequence file name
|   o seqTypeFlag:
|     - Holds the type of file input for seqFileStr
|   o refFileStr:
|     - Pointer to c-string to hold the spoligo sequences
|       file name
|   o spoliogDbStr:
|     - Pointer to c-string to hold the path to the
|       database with spoligo lineages
|   o outFileStr:
|     - Pointer to c-string to hold the output file name
|   o minPercScoreF:
|     - Pointer to float to hold the minimum percent score
|       to detect an spoligotype hit
|   o dirStartSI:
|     - Will hold the start of the DR region
|   o dirEndSI:
|     - Will hold the end of the DR region
|   o minKmersSI:
|     - int to hold the minimum number of kmer matches to
|       have to do an waterman alignment (kmer method)
|   o fastTypingBl:
|     - Set to 1 if user wants faster kmer method
|     - Set to 0 if user wants slower method
| Output:
|   - Modifies:
|     o All input variables except parmStr and argStr to
|       hold the users input
|       - Sets seqTypeFlag to:
|         o def_fqFile_tbSpoligo if fastq file input
|         o def_faFile_tbSpoligo if fasta file input
|         o def_samFile_tbSpoligo if an sam file input
|         o def_samConFile_tbSpoligo if an sam file with
|           mapped consensuses was input
|   - Returns:
|     o 0: Parameter is a boolean (no argument)
|     o 1: Parameter has an agrument
|     o 1 | def_numericErr_tbSpoligo for numeric errors
|     o def_phelp_tbSpoligo if help message requested
|     o def_pversoin_tbSpoligo if version number requested
|     o def_invalidErr_tbSpoligo for unrecongnized entries
\-------------------------------------------------------*/
signed char
checkInput_tbSpoligo(
   signed char *parmStr,     /*Paramter to check*/
   signed char *argStr,      /*Argument to check*/
   signed char **seqFileStr, /*sequence file name*/
   signed char *seqTypeFlag,   /*Type of file input*/
   signed char **refFileStr, /*Spoligotype seq file name*/
   signed char **spoliogDbStr,/*database of lineages*/
   signed char **outFileStr, /*File to save to*/
   float *minPercScoreF,     /*min % score to count hit*/
   signed int *dirStartSI,   /*start of DR region*/
   signed int *dirEndSI,     /*start of DR region*/
   signed int *minKmersSI,   /*Min number kmers for fast*/
   signed char *fastTypingBl /*1: Do fast spoligo typing*/
);

/*-------------------------------------------------------\
| Fun-02: phelp_tbSpoligo
|   - prints the help messge for tbSpoligo
| Input:
|   o outFileStr:
|     - c-string with name of file to print help message
|       to
|     - Use "-" or "1" for stdout and "2" for stderr
| Output:
|   o Prints:
|     - The help message to outFileStr
|   o Returns:
|     - 0 for no errors
|     - 1 if I could not open the file
\-------------------------------------------------------*/
signed char
phelp_tbSpoligo(
   signed char *outFileStr
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
