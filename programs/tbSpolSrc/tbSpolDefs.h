/*########################################################
# Name: tbSpolDefs
#   - holds defualts, version numbers, and pre-defined
#     variables for tbSpol
########################################################*/

#ifndef TB_SPOLIGOTYPE_DEFAULTS_H
#define TB_SPOLIGOTYPE_DEFAULTS_H

#define def_year_tbSpolDefs 2024
#define def_month_tbSpolDefs 8
#define def_day_tbSpolDefs 19

#define def_numSpol_tbSpolDefs 64/*max # spacers allowed*/

#define def_noSpol_tbSpolDefs 1
#define def_fileErr_tbSpolDefs 2
#define def_memErr_tbSpolDefs 64

#define def_fastSearch_tbSpolDefs 1
#define def_frag_tbSpolDefs 0
#define def_conFrag_tbSpolDefs 0

#define def_fqFile_tbSpolDefs 1
#define def_faFile_tbSpolDefs 2
#define def_samFile_tbSpolDefs 4
#define def_samConFile_tbSpolDefs 8
#define def_seqIn_tbSpolDefs def_samFile_tbSpolDefs

/*waterman variables*/
#define def_minPercScore_tbSpolDefs 0.9f

/*kmer fin variables*/
#define def_minKmerPerc_tbSpolDefs 0.5f
#define def_percShift_tbSpolDefs 0.5f
#define def_extraNtInWin_tbSpolDefs 1.0f
#define def_lenKmer_tbSpolDefs 5

/*These are a bit wide, but they should work*/
#define def_DRStart_tbSpolDefs 3119037
#define def_DREnd_tbSpolDefs 3123624

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
