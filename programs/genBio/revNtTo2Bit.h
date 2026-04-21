/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' revNtTo2Bit SOF: Start Of File
'   - had tables to convert nucleodtides to two bits
'   - the third bit holds a error bit that marks anonymous
'     base or non-nucleotide characters. it can be ingored
'     after checks
'   o header:
'     - guards and defined variables
'   o tbl01: revNtTo2Bit
'     - table to convert bases to four bit (a/t/g/c only)
'       values to two bits with the 3rd bit saved for an
'       error/anonymous base
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and defined variables
\-------------------------------------------------------*/

#ifndef REVERSE_NUCLEOTIDE_TO_TWO_BIT_H
#define REVERSE_NUCLEOTIDE_TO_TWO_BIT_H

#define def_t_revNtTo2Bit 0
#define def_c_revNtTo2Bit 1
#define def_a_revNtTo2Bit 2
#define def_g_revNtTo2Bit 3
#define def_err3rdBit_revNtTo2Bit 4

/*-------------------------------------------------------\
| Tbl01: revNtTo2Bit
|  - table to convert bases to four bit (a/t/g/c only)
|    values to two bits with the 3rd bit saved for an
|    error/anonymous base
|    o 1st bit is A (a_fourBit)
|    o 2nd bit is C (c_fourBit)
|    o 3rd bit is G (g_fourBit)
|    o 4th bit is T/U (t_fourBit)
\-------------------------------------------------------*/
static
unsigned char revNtTo2Bit[] =
{  /*revNtTo2Bit*/
   /*White space/invisible charactes block*/
   def_err3rdBit_revNtTo2Bit, /*0   = Null character*/

   def_err3rdBit_revNtTo2Bit, /*1   = Start of Heading*/
   def_err3rdBit_revNtTo2Bit, /*2   = Start of Text*/
   def_err3rdBit_revNtTo2Bit, /*3   = End of Text*/
   def_err3rdBit_revNtTo2Bit, /*4   = End Transmission*/
   def_err3rdBit_revNtTo2Bit, /*5   = Enquiry*/
   def_err3rdBit_revNtTo2Bit, /*6   = Acknowledge*/
   def_err3rdBit_revNtTo2Bit, /*7   = Bell*/
   def_err3rdBit_revNtTo2Bit, /*8   = Backspace*/

   def_err3rdBit_revNtTo2Bit, /*9   =  tab (horizontal)*/
   def_err3rdBit_revNtTo2Bit, /*10  = New line*/

   def_err3rdBit_revNtTo2Bit, /*11  = Vertical Tab*/
   def_err3rdBit_revNtTo2Bit, /*12  = Form Feed*/

   def_err3rdBit_revNtTo2Bit, /*13  = Carriage Return*/

   def_err3rdBit_revNtTo2Bit, /*14  = Shift Out*/
   def_err3rdBit_revNtTo2Bit, /*15  = Shift In*/
   def_err3rdBit_revNtTo2Bit, /*16  = Data Link Escape*/
   def_err3rdBit_revNtTo2Bit, /*17  = Device Control One*/
   def_err3rdBit_revNtTo2Bit, /*18  = Device Control Two*/
   def_err3rdBit_revNtTo2Bit, /*19  = Device Contol 3*/
   def_err3rdBit_revNtTo2Bit, /*20  = Device Control 4*/
   def_err3rdBit_revNtTo2Bit, /*21  = Neg Acknowledge*/
   def_err3rdBit_revNtTo2Bit, /*22  = Synchronous Idle*/
   def_err3rdBit_revNtTo2Bit, /*23  = End Transmission*/
   def_err3rdBit_revNtTo2Bit, /*24  = Cancle*/
   def_err3rdBit_revNtTo2Bit, /*25  = End of medium*/
   def_err3rdBit_revNtTo2Bit, /*26  = Substitute*/
   def_err3rdBit_revNtTo2Bit, /*27  = escape*/
   def_err3rdBit_revNtTo2Bit, /*28  = File Separator*/
   def_err3rdBit_revNtTo2Bit, /*29  = Group Separator*/
   def_err3rdBit_revNtTo2Bit, /*30  = Record Separator*/
   def_err3rdBit_revNtTo2Bit, /*31  = Unit Separator*/

   /*symbol/number block*/
   def_err3rdBit_revNtTo2Bit, /*32  = space*/
   def_err3rdBit_revNtTo2Bit, /*33  = !*/
   def_err3rdBit_revNtTo2Bit, /*34  = "*/
   def_err3rdBit_revNtTo2Bit, /*35  = #*/
   def_err3rdBit_revNtTo2Bit, /*36  = $*/
   def_err3rdBit_revNtTo2Bit, /*37  = %*/
   def_err3rdBit_revNtTo2Bit, /*38  = &*/
   def_err3rdBit_revNtTo2Bit, /*39  = '*/
   def_err3rdBit_revNtTo2Bit, /*40  = (*/
   def_err3rdBit_revNtTo2Bit, /*41  = )*/
   def_err3rdBit_revNtTo2Bit, /*42  = **/
   def_err3rdBit_revNtTo2Bit, /*43  = +*/
   def_err3rdBit_revNtTo2Bit, /*44  = ,*/
   def_err3rdBit_revNtTo2Bit, /*45  = -*/
   def_err3rdBit_revNtTo2Bit, /*46  = .*/
   def_err3rdBit_revNtTo2Bit, /*47  = /*/
   def_err3rdBit_revNtTo2Bit, /*48  = 0*/
   def_err3rdBit_revNtTo2Bit, /*49  = 1*/
   def_err3rdBit_revNtTo2Bit, /*50  = 2*/
   def_err3rdBit_revNtTo2Bit, /*51  = 3*/
   def_err3rdBit_revNtTo2Bit, /*52  = 4*/
   def_err3rdBit_revNtTo2Bit, /*53  = 5*/
   def_err3rdBit_revNtTo2Bit, /*54  = 6*/
   def_err3rdBit_revNtTo2Bit, /*55  = 7*/
   def_err3rdBit_revNtTo2Bit, /*56  = 8*/
   def_err3rdBit_revNtTo2Bit, /*57  = 9*/
   def_err3rdBit_revNtTo2Bit, /*58  = :*/
   def_err3rdBit_revNtTo2Bit, /*59  = ;*/
   def_err3rdBit_revNtTo2Bit, /*60  = <*/
   def_err3rdBit_revNtTo2Bit, /*61  = =*/
   def_err3rdBit_revNtTo2Bit, /*62  = >*/
   def_err3rdBit_revNtTo2Bit, /*63  = ?*/
   def_err3rdBit_revNtTo2Bit, /*64  = @*/

   /*Uppercase letters*/
   def_t_revNtTo2Bit,   /*65  = A*/

   def_err3rdBit_revNtTo2Bit, /*66=B; C/G/T*/

   def_g_revNtTo2Bit,   /*67  = C*/


   def_err3rdBit_revNtTo2Bit, /*68=D; A/G/T*/
   def_err3rdBit_revNtTo2Bit, /*69  = E not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*70  = F not nucleotide*/

   def_c_revNtTo2Bit,     /*71  = G*/

   def_err3rdBit_revNtTo2Bit, /*72=H; A/G/T*/
   def_err3rdBit_revNtTo2Bit, /*73  = I not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*74  = J not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*75=K; G/T*/
   def_err3rdBit_revNtTo2Bit, /*76  = L not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*77=M; A/C*/

   def_err3rdBit_revNtTo2Bit, /*78=N; any base*/
   def_err3rdBit_revNtTo2Bit, /*79  = O not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*80  = P not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*81  = Q not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*82=R A/G*/
   def_err3rdBit_revNtTo2Bit, /*83=S C/G*/

   def_a_revNtTo2Bit,   /*84  = T*/
   def_a_revNtTo2Bit,   /*85  = U*/

   def_err3rdBit_revNtTo2Bit, /*86  = V (ACG),treat as N*/
   def_err3rdBit_revNtTo2Bit, /*87=W A\T*/
   def_err3rdBit_revNtTo2Bit, /*88=X; any aa*/
   def_err3rdBit_revNtTo2Bit,   /*89=Y C/T*/
   def_err3rdBit_revNtTo2Bit, /*90  = Z not nucleotide*/

   /*Special characters after upercase letters*/
   def_err3rdBit_revNtTo2Bit, /*91  = [*/
   def_err3rdBit_revNtTo2Bit, /*92  = \*/
   def_err3rdBit_revNtTo2Bit, /*93  = ]*/
   def_err3rdBit_revNtTo2Bit, /*94  = ^*/
   def_err3rdBit_revNtTo2Bit, /*95  = _*/
   def_err3rdBit_revNtTo2Bit, /*96  = `*/

   /*lower case letters*/
   def_t_revNtTo2Bit,   /*97=a*/

   def_err3rdBit_revNtTo2Bit, /*98=b; C/G/T*/

   def_g_revNtTo2Bit,   /*99  = c*/

   def_err3rdBit_revNtTo2Bit, /*100=d; A/G/T*/
   def_err3rdBit_revNtTo2Bit, /*101 = e not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*102 = f not nucleotide*/
   def_c_revNtTo2Bit,   /*103 = g*/
   def_err3rdBit_revNtTo2Bit, /*104=h; A/G/T*/
   def_err3rdBit_revNtTo2Bit, /*105 = i not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*106 = j not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*107=k; G/T*/
   def_err3rdBit_revNtTo2Bit, /*108  = l not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*109=m; A/C*/

   def_err3rdBit_revNtTo2Bit, /*110=n; any base*/

   def_err3rdBit_revNtTo2Bit, /*111 = o not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*112 = p not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*113 = q not nucleotide*/
   def_err3rdBit_revNtTo2Bit, /*114=r A/G*/
   def_err3rdBit_revNtTo2Bit, /*115=s C/G*/
   def_a_revNtTo2Bit,     /*116 = t*/
   def_a_revNtTo2Bit,     /*117 = u*/

   
   def_err3rdBit_revNtTo2Bit, /*118 = v (ACG),treat as N*/
   def_err3rdBit_revNtTo2Bit, /*119=w A\T*/
   def_err3rdBit_revNtTo2Bit, /*120 = x*/
   def_err3rdBit_revNtTo2Bit,   /*121=y C/T*/
   def_err3rdBit_revNtTo2Bit, /*122 = z not nucleotide*/

   /*Special characters after lowercase letters*/
   def_err3rdBit_revNtTo2Bit, /*123 = {*/
   def_err3rdBit_revNtTo2Bit, /*124 = |*/
   def_err3rdBit_revNtTo2Bit, /*125 = }*/
   def_err3rdBit_revNtTo2Bit, /*126 = ~*/
   def_err3rdBit_revNtTo2Bit, /*127 = Del*/
}; /*revNtTo2Bit*/

#endif

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
