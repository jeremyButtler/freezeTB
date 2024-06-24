/*########################################################
# Name: ntToFourBit
#   - has look up table to convert nucleodtides to a three
#     bit value
#   - there is an extra (fourth) bit for errors. this
#     reports characters that are not nucleotides.
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards and defined variables
'   o tbl01: ntToFourBit
'     - table to convert bases to four bit (a/t/g/c only)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and defined variables
\-------------------------------------------------------*/

#ifndef NUCLEOTIDE_TO_FOUR_BIT_H
#define NUCLEOTIDE_TO_FOUR_BIT_H

#define a_fourBit 1
#define c_fourBit 1 << 1
#define g_fourBit 1 << 2
#define t_fourBit 1 << 3
#define n_fiveBit 1 << 4  /*Marks an anonymous base*/

#define err_fiveBit 1 << 4 /*For four bit only*/

/*-------------------------------------------------------\
| Tbl-01: ntToFourBit
|  - table to convert bases to four bit (a/t/g/c only)
|    values, with the extra fifth bit acting as an error
|    o 1st bit is A (a_fourBit)
|    o 2nd bit is C (c_fourBit)
|    o 3rd bit is G (g_fourBit)
|    o 4th bit is T/U (t_fourBit)
|    o 5th bit is set if chacter is not an nucleotide
\-------------------------------------------------------*/
static
unsigned char ntToFourBit[] =
{ /*ntToFourBit*/
   /*White space/invisible charactes block*/
   err_fiveBit, /*0   = Null character*/

   err_fiveBit, /*1   = Start of Heading*/
   err_fiveBit, /*2   = Start of Text*/
   err_fiveBit, /*3   = End of Text*/
   err_fiveBit, /*4   = End of Transmission*/
   err_fiveBit, /*5   = Enquiry*/
   err_fiveBit, /*6   = Acknowledge*/
   err_fiveBit, /*7   = Bell*/
   err_fiveBit, /*8   = Backspace*/

   err_fiveBit, /*9   =  tab (horizontal)*/
   err_fiveBit, /*10  = New line*/

   err_fiveBit, /*11  = Vertical Tab (not key)*/
   err_fiveBit, /*12  = Form Feed*/

   err_fiveBit, /*13  = Carriage Return*/

   err_fiveBit, /*14  = Shift Out*/
   err_fiveBit, /*15  = Shift In*/
   err_fiveBit, /*16  = Data Link Escape*/
   err_fiveBit, /*17  = Device Control One*/
   err_fiveBit, /*18  = Device Control Two*/
   err_fiveBit, /*19  = Device Contol Three*/
   err_fiveBit, /*20  = Device Control Four*/
   err_fiveBit, /*21  = Negative Acknowledge*/
   err_fiveBit, /*22  = Synchronous Idle*/
   err_fiveBit, /*23  = End Transmission Block*/
   err_fiveBit, /*24  = Cancle*/
   err_fiveBit, /*25  = End of medium*/
   err_fiveBit, /*26  = Substitute*/
   err_fiveBit, /*27  = escape*/
   err_fiveBit, /*28  = File Separator*/
   err_fiveBit, /*29  = Group Separator*/
   err_fiveBit, /*30  = Record Separator*/
   err_fiveBit, /*31  = Unit Separator*/

   /*symbol/number block*/
   err_fiveBit, /*32  = space*/
   err_fiveBit, /*33  = !*/
   err_fiveBit, /*34  = "*/
   err_fiveBit, /*35  = #*/
   err_fiveBit, /*36  = $*/
   err_fiveBit, /*37  = %*/
   err_fiveBit, /*38  = &*/
   err_fiveBit, /*39  = '*/
   err_fiveBit, /*40  = (*/
   err_fiveBit, /*41  = )*/
   err_fiveBit, /*42  = **/
   err_fiveBit, /*43  = +*/
   err_fiveBit, /*44  = ,*/
   err_fiveBit, /*45  = -*/
   err_fiveBit, /*46  = .*/
   err_fiveBit, /*47  = /*/
   err_fiveBit, /*48  = 0*/
   err_fiveBit, /*49  = 1*/
   err_fiveBit, /*50  = 2*/
   err_fiveBit, /*51  = 3*/
   err_fiveBit, /*52  = 4*/
   err_fiveBit, /*53  = 5*/
   err_fiveBit, /*54  = 6*/
   err_fiveBit, /*55  = 7*/
   err_fiveBit, /*56  = 8*/
   err_fiveBit, /*57  = 9*/
   err_fiveBit, /*58  = :*/
   err_fiveBit, /*59  = ;*/
   err_fiveBit, /*60  = <*/
   err_fiveBit, /*61  = =*/
   err_fiveBit, /*62  = >*/
   err_fiveBit, /*63  = ?*/
   err_fiveBit, /*64  = @*/

   /*Uppercase letters*/
   a_fourBit,   /*65  = A*/

   /*66=B; C/G/T*/
   (c_fourBit | g_fourBit | t_fourBit),

   c_fourBit,   /*67  = C*/

   /*68=D; A/G/T*/
   (a_fourBit | g_fourBit | t_fourBit),

   err_fiveBit, /*69  = E not nucleotide*/
   err_fiveBit, /*70  = F not nucleotide*/
   g_fourBit,   /*71  = G*/

   /*72=H; A/G/T*/
   (a_fourBit | c_fourBit | t_fourBit),

   err_fiveBit, /*73  = I not nucleotide*/
   err_fiveBit, /*74  = J not nucleotide*/
   (g_fourBit | t_fourBit), /*75=K; G/T*/
   err_fiveBit, /*76  = L not nucleotide*/
   (a_fourBit | c_fourBit), /*77=M; A/C*/

   /*78=N; any base*/
   (
       a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*Any base (N)*/

   err_fiveBit, /*79  = O not nucleotide*/
   err_fiveBit, /*80  = P not nucleotide*/
   err_fiveBit, /*81  = Q not nucleotide*/
   (a_fourBit | g_fourBit),   /*82=R A/G*/
   (c_fourBit | g_fourBit),   /*83=S C/G*/
   t_fourBit,   /*84  = T*/
   t_fourBit,   /*85  = U*/

   /*86  = V (ACG), treat as N*/
   (a_fourBit | c_fourBit | g_fourBit),

   (a_fourBit | t_fourBit), /*87=W A\T*/

   /*88=X; any aa (I will hold same for nucelotides)*/
   (
       a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*X*/

   (c_fourBit | t_fourBit),   /*89=Y C/T*/
   err_fiveBit, /*90  = Z not nucleotide*/

   /*Special characters after upercase letters*/
   err_fiveBit, /*91  = [*/
   err_fiveBit, /*92  = \*/
   err_fiveBit, /*93  = ]*/
   err_fiveBit, /*94  = ^*/
   err_fiveBit, /*95  = _*/
   err_fiveBit, /*96  = `*/

   /*lower case letters*/
   a_fourBit,   /*97=a*/

   /*98=b; C/G/T*/
   (c_fourBit | g_fourBit | t_fourBit),

   c_fourBit,   /*99  = c*/

   /*100=d; A/G/T*/
   (a_fourBit | g_fourBit | t_fourBit),

   err_fiveBit, /*101 = e not nucleotide*/
   err_fiveBit, /*102 = f not nucleotide*/
   g_fourBit,   /*103 = g*/

   /*104=h; A/G/T*/
   (a_fourBit | c_fourBit | t_fourBit),

   err_fiveBit, /*105 = i not nucleotide*/
   err_fiveBit, /*106 = j not nucleotide*/
   (g_fourBit | t_fourBit), /*107=k; G/T*/
   err_fiveBit, /*108  = l not nucleotide*/
   (a_fourBit | c_fourBit), /*109=m; A/C*/

   /*110=n; any base*/
   ( 
       a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*Any base (n)*/

   err_fiveBit, /*111 = o not nucleotide*/
   err_fiveBit, /*112 = p not nucleotide*/
   err_fiveBit, /*113 = q not nucleotide*/
   (a_fourBit | g_fourBit),   /*114=r A/G*/
   (c_fourBit | g_fourBit),   /*115=s C/G*/
   t_fourBit,   /*116 = t*/
   t_fourBit,   /*117 = u*/

   /*118 = v (ACG), treat as N*/
   (a_fourBit | c_fourBit | g_fourBit),

   (a_fourBit | t_fourBit), /*119=w A\T*/

   /*120=x; any aa (I will hold same for nucelotides)*/
   ( 
       a_fourBit
     | c_fourBit
     | g_fourBit
     | t_fourBit
   ), /*x*/

   (c_fourBit | t_fourBit),   /*121=y C/T*/
   err_fiveBit, /*122 = z not nucleotide*/

   /*Special characters after lowercase letters*/
   err_fiveBit, /*123 = {*/
   err_fiveBit, /*124 = |*/
   err_fiveBit, /*125 = }*/
   err_fiveBit, /*126 = ~*/
   err_fiveBit, /*127 = Del*/
}; /*ntToFourBit*/

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
