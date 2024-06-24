/*########################################################
# Name: ntToFiveBit
#   - has look up table to convert nucleodtides to a five
#     bit value
#   - the six bit is an error bit for non-nucleotide
#     characters. it can be ignored after checking.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards and defined variables
'   o tbl01: ntToFiveBit
'     - table to convert bases to five bit values, with an
'       extra sixth bit acting as an error
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and defined variables
\-------------------------------------------------------*/

#ifndef NUCLEOTIDE_TO_SIX_BIT_H
#define NUCLEOTIDE_TO_SIX_BIT_H

#define a_fourBit 1
#define c_fourBit 1 << 1
#define g_fourBit 1 << 2
#define t_fourBit 1 << 3
#define n_fiveBit 1 << 4  /*Marks an anonymous base*/
#define err_sixBit 1 << 5

/*-------------------------------------------------------\
| Tbl-01: ntToFiveBit
|  - table to convert bases to five bit values (a/c/g/t/
|    is anonymous), with an extra sixth bit acting as an
|    error flag
|    o 1st bit is A (a_fourBit)
|    o 2nd bit is C (c_fourBit)
|    o 3rd bit is G (g_fourBit)
|    o 4th bit is T/U (t_fourBit)
|    o 5th bit marks anonymous base (n_fiveBit)
|    o 6th bit is error (err_sixBit)
\-------------------------------------------------------*/
static
unsigned char ntToFiveBit[] =
{ /*ntToFiveBit*/
   /*White space/invisible charactes block*/
   err_sixBit, /*0   = Null character*/

   err_sixBit, /*1   = Start of Heading*/
   err_sixBit, /*2   = Start of Text*/
   err_sixBit, /*3   = End of Text*/
   err_sixBit, /*4   = End of Transmission*/
   err_sixBit, /*5   = Enquiry*/
   err_sixBit, /*6   = Acknowledge*/
   err_sixBit, /*7   = Bell*/
   err_sixBit, /*8   = Backspace*/

   err_sixBit, /*9   =  tab (horizontal)*/
   err_sixBit, /*10  = New line*/

   err_sixBit, /*11  = Vertical Tab (not key)*/
   err_sixBit, /*12  = Form Feed*/

   err_sixBit, /*13  = Carriage Return*/

   err_sixBit, /*14  = Shift Out*/
   err_sixBit, /*15  = Shift In*/
   err_sixBit, /*16  = Data Link Escape*/
   err_sixBit, /*17  = Device Control One*/
   err_sixBit, /*18  = Device Control Two*/
   err_sixBit, /*19  = Device Contol Three*/
   err_sixBit, /*20  = Device Control Four*/
   err_sixBit, /*21  = Negative Acknowledge*/
   err_sixBit, /*22  = Synchronous Idle*/
   err_sixBit, /*23  = End Transmission Block*/
   err_sixBit, /*24  = Cancle*/
   err_sixBit, /*25  = End of medium*/
   err_sixBit, /*26  = Substitute*/
   err_sixBit, /*27  = escape*/
   err_sixBit, /*28  = File Separator*/
   err_sixBit, /*29  = Group Separator*/
   err_sixBit, /*30  = Record Separator*/
   err_sixBit, /*31  = Unit Separator*/

   /*symbol/number block*/
   err_sixBit, /*32  = space*/
   err_sixBit, /*33  = !*/
   err_sixBit, /*34  = "*/
   err_sixBit, /*35  = #*/
   err_sixBit, /*36  = $*/
   err_sixBit, /*37  = %*/
   err_sixBit, /*38  = &*/
   err_sixBit, /*39  = '*/
   err_sixBit, /*40  = (*/
   err_sixBit, /*41  = )*/
   err_sixBit, /*42  = **/
   err_sixBit, /*43  = +*/
   err_sixBit, /*44  = ,*/
   err_sixBit, /*45  = -*/
   err_sixBit, /*46  = .*/
   err_sixBit, /*47  = /*/
   err_sixBit, /*48  = 0*/
   err_sixBit, /*49  = 1*/
   err_sixBit, /*50  = 2*/
   err_sixBit, /*51  = 3*/
   err_sixBit, /*52  = 4*/
   err_sixBit, /*53  = 5*/
   err_sixBit, /*54  = 6*/
   err_sixBit, /*55  = 7*/
   err_sixBit, /*56  = 8*/
   err_sixBit, /*57  = 9*/
   err_sixBit, /*58  = :*/
   err_sixBit, /*59  = ;*/
   err_sixBit, /*60  = <*/
   err_sixBit, /*61  = =*/
   err_sixBit, /*62  = >*/
   err_sixBit, /*63  = ?*/
   err_sixBit, /*64  = @*/

   /*Uppercase letters*/
   a_fourBit,   /*65  = A*/

   /*66=B; C/G/T*/
   (n_fiveBit | c_fourBit | g_fourBit | t_fourBit),

   c_fourBit,   /*67  = C*/

   /*68=D; A/G/T*/
   (n_fiveBit | a_fourBit | g_fourBit | t_fourBit),

   err_sixBit, /*69  = E not nucleotide*/
   err_sixBit, /*70  = F not nucleotide*/
   g_fourBit,   /*71  = G*/

   /*72=H; A/G/T*/
   (n_fiveBit | a_fourBit | c_fourBit | t_fourBit),

   err_sixBit, /*73  = I not nucleotide*/
   err_sixBit, /*74  = J not nucleotide*/
   (n_fiveBit | g_fourBit | t_fourBit), /*75=K; G/T*/
   err_sixBit, /*76  = L not nucleotide*/
   (n_fiveBit | a_fourBit | c_fourBit), /*77=M; A/C*/

   /*78=N; any base*/
   (   n_fiveBit
     | a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*Any base (N)*/

   err_sixBit, /*79  = O not nucleotide*/
   err_sixBit, /*80  = P not nucleotide*/
   err_sixBit, /*81  = Q not nucleotide*/
   (n_fiveBit | a_fourBit | g_fourBit),   /*82=R A/G*/
   (n_fiveBit | c_fourBit | g_fourBit),   /*83=S C/G*/
   t_fourBit,   /*84  = T*/
   t_fourBit,   /*85  = U*/

   /*86  = V (ACG), treat as N*/
   (n_fiveBit | a_fourBit | c_fourBit | g_fourBit),

   (n_fiveBit | a_fourBit | t_fourBit), /*87=W A\T*/

   /*88=X; any aa (I will hold same for nucelotides)*/
   (   n_fiveBit
     | a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*X*/

   (n_fiveBit | c_fourBit | t_fourBit),   /*89=Y C/T*/
   err_sixBit, /*90  = Z not nucleotide*/

   /*Special characters after upercase letters*/
   err_sixBit, /*91  = [*/
   err_sixBit, /*92  = \*/
   err_sixBit, /*93  = ]*/
   err_sixBit, /*94  = ^*/
   err_sixBit, /*95  = _*/
   err_sixBit, /*96  = `*/

   /*lower case letters*/
   a_fourBit,   /*97=a*/

   /*98=b; C/G/T*/
   (n_fiveBit | c_fourBit | g_fourBit | t_fourBit),

   c_fourBit,   /*99  = c*/

   /*100=d; A/G/T*/
   (n_fiveBit | a_fourBit | g_fourBit | t_fourBit),

   err_sixBit, /*101 = e not nucleotide*/
   err_sixBit, /*102 = f not nucleotide*/
   g_fourBit,   /*103 = g*/

   /*104=h; A/G/T*/
   (n_fiveBit | a_fourBit | c_fourBit | t_fourBit),

   err_sixBit, /*105 = i not nucleotide*/
   err_sixBit, /*106 = j not nucleotide*/
   (n_fiveBit | g_fourBit | t_fourBit), /*107=k; G/T*/
   err_sixBit, /*108  = l not nucleotide*/
   (n_fiveBit | a_fourBit | c_fourBit), /*109=m; A/C*/

   /*110=n; any base*/
   (   n_fiveBit
     | a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*Any base (n)*/

   err_sixBit, /*111 = o not nucleotide*/
   err_sixBit, /*112 = p not nucleotide*/
   err_sixBit, /*113 = q not nucleotide*/
   (n_fiveBit | a_fourBit | g_fourBit),   /*114=r A/G*/
   (n_fiveBit | c_fourBit | g_fourBit),   /*115=s C/G*/
   t_fourBit,   /*116 = t*/
   t_fourBit,   /*117 = u*/

   /*118 = v (ACG), treat as N*/
   (n_fiveBit | a_fourBit | c_fourBit | g_fourBit),

   (n_fiveBit | a_fourBit | t_fourBit), /*119=w A\T*/

   /*120=x; any aa (I will hold same for nucelotides)*/
   (   n_fiveBit
     | a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*x*/

   (n_fiveBit | c_fourBit | t_fourBit),   /*121=y C/T*/
   err_sixBit, /*122 = z not nucleotide*/

   /*Special characters after lowercase letters*/
   err_sixBit, /*123 = {*/
   err_sixBit, /*124 = |*/
   err_sixBit, /*125 = }*/
   err_sixBit, /*126 = ~*/
   err_sixBit, /*127 = Del*/
}; /*ntToFiveBit*/

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the publikc domain
:   is inconvient / not possible, this code is under the
:   MIT license.
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
