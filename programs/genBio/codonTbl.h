/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' codonTbl SOF: Start Of File
'   - has codon table and functions to support the
'     codon table
'   o header:
'     - guards
'   o tbl01 codonLkTbl:
'     - Table to convert three bases to codons
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef CODON_TABLE_H
#define CODON_TABLE_H

/*keep these in sync with codonFun and as negative*/
#define def_unkownNt_codonTbl -1
#define def_incomplete_codonTbl -2

/*--------------------------------------------------------\
| Tbl01 codonTbl:
|  - table to convert three bases to codons
\--------------------------------------------------------*/
static signed char codonTbl[5][5][5] =
   {  /*codonTbl*/
      { /*First element is an T*/
         {'f', 'f', 'l', 'l', 'x'}, /*2nd element is T*/
         {'s', 's', 's', 's', 's'}, /*2nd element is C*/
         {'y', 'y', '*', '*', 'x'}, /*2nd element is A*/
         {'c', 'c', '*', 'w', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an T*/

      { /*First element is an C*/
         {'l', 'l', 'l', 'l', 'l'}, /*2nd element is T*/
         {'p', 'p', 'p', 'p', 'p'}, /*2nd element is C*/
         {'h', 'h', 'q', 'q', 'x'}, /*2nd element is A*/
         {'r', 'r', 'r', 'r', 'r'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an C*/

      { /*First element is an A*/
         {'i', 'i', 'i', 'm', 'x'}, /*2nd element is T*/
         {'t', 't', 't', 't', 't'}, /*2nd element is C*/
         {'n', 'n', 'k', 'k', 'x'}, /*2nd element is A*/
         {'s', 's', 'r', 'r', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an A*/

      { /*First element is an G*/
         {'v', 'v', 'v', 'v', 'v'}, /*2nd element is T*/
         {'a', 'a', 'a', 'a', 'a'}, /*2nd element is C*/
         {'d', 'd', 'e', 'e', 'x'}, /*2nd element is A*/
         {'g', 'g', 'g', 'g', 'g'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an G*/

      { /*First element is an N*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is T*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is C*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is A*/
         {'x', 'x', 'x', 'x', 'x'}, /*2nd element is G*/
         {'x', 'x', 'x', 'x', 'x'}  /*2nd element is N*/
      }, /*First element is an N*/
   }; /*codonTbl*/

#endif

/*
   Ala A Alanine
   Arg R Arginine
   Asn N Asparagine
   Asp D Aspartic acid
   Cys C Cysteine
   Gln Q Glutamine
   Glu E Glutamic acid
   Gly G Glycine
   His H Histidine
   Ile I Isoleucine
   Lys K Lysine
   Leu L Luecine
   Met M Methionine
   Phe F Phenylalanine
   Pro P Proline
   Ser S Serine
   Thr T Threonine
   Trp W Tryptophan
   Try Y Tyrosine
   Val V Valine
   X   X anything
*/

/*Table
     T         C        A        G
  +----------+----------+----------+----------+
  | TTT  F . | TCT  S . | TAT  Y . | TGT  C . | T
T | TTC  F . | TCC  S . | TAC  Y . | TGC  C . | C
  | TTA  L . | TCA  S . | TAA  * . | TGA  * . | A
  | TTG  L 2 | TCG  S . | TAG  * . | TGG  W . | G
  +----------+----------+----------+----------+
  | CTT  L . | CCT  P . | CAT  H . | CGT  R . | T
C | CTC  L . | CCC  P . | CAC  H . | CGC  R . | C
  | CTA  L . | CCA  P . | CAA  Q . | CGA  R . | A
  | CTG  L . | CCG  P . | CAG  Q . | CGG  R . | G
  +----------+----------+----------+----------+
  | ATT  I . | ACT  T . | AAT  N . | AGT  S . | T
A | ATC  I . | ACC  T . | AAC  N . | AGC  S . | C
  | ATA  I . | ACA  T . | AAA  K . | AGA  R . | A
  | ATG  M 1 | ACG  T . | AAG  K . | AGG  R . | G
  +----------+----------+----------+----------+
  | GTT  V . | GCT  A . | GAT  D . | GGT  G . | T
G | GTC  V . | GCC  A . | GAC  D . | GGC  G . | C
  | GTA  V . | GCA  A . | GAA  E . | GGA  G . | A
  | GTG  V 2 | GCG  A . | GAG  E . | GGG  G . | G
  +----------+----------+----------+----------+

  1 = Canonical start codon
  2 = Bacterial non-canconical start codon
  . = Nothing
*/

/*ASCII table
   Dec Hex  Binary   Value  Description
   0    00  00000000 NUL    Null character
   1    01  00000001 SOH    Start of Heading
   2    02  00000010 STX    Start of Text
   3    03  00000011 ETX    End of Text
   4    04  00000100 EOT    End of Transmission
   5    05  00000101 ENQ    Enquiry
   6    06  00000110 ACK    Acknowledge
   7    07  00000111 BEL    Bell, Alert
   8    08  00001000 BS     Backspace
   9    09  00001001 HT     Horizontal Tab
   10   0A  00001010 LF     Line Feed
   11   0B  00001011 VT     Vertical Tabulation
   12   0C  00001100 FF     Form Feed
   13   0D  00001101 CR     Carriage Return
   14   0E  00001110 SO     Shift Out
   15   0F  00001111 SI     Shift In
   16   10  00010000 DLE    Data Link Escape
   17   11  00010001 DC1    Device Control One (XON)
   18   12  00010010 DC2    Device Control Two
   19   13  00010011 DC3    Device Control Three (XOFF)
   20   14  00010100 DC4    Device Control Four
   21   15  00010101 NAK    Negative Acknowledge
   22   16  00010110 SYN    Synchronous Idle
   23   17  00010111 ETB    End of Transmission Block
   24   18  00011000 CAN    Cancel
   25   19  00011001 EM     End of medium
   26   1A  00011010 SUB    Substitute
   27   1B  00011011 ESC    Escape
   28   1C  00011100 FS     File Separator
   29   1D  00011101 GS     Group Separator
   30   1E  00011110 RS     Record Separator
   31   1F  00011111 US     Unit Separator

   32   20  00100000 SP     Space
   33   21  00100001 !      Exclamation mark
   34   22  00100010 "      Double quotes
   35   23  00100011 #      Number sign
   36   24  00100100 $      Dollar
   37   25  00100101 %      Per cent sign
   38   26  00100110 &      Ampersand
   39   27  00100111 '      Single quote
   40   28  00101000 (      Open parenthesis
   41   29  00101001 )      Close parenthesis
   42   2A  00101010 *      Asterisk
   43   2B  00101011 +      Plus
   44   2C  00101100 ,      Comma
   45   2D  00101101 -      Hyphen-minus
   46   2E  00101110 .      Period, dot or full stop
   47   2F  00101111 /      Slash or divide
   48   30  00110000 0      Zero
   49   31  00110001 1      One
   50   32  00110010 2      Two
   51   33  00110011 3      Three
   52   34  00110100 4      Four
   53   35  00110101 5      Five
   54   36  00110110 6      Six
   55   37  00110111 7      Seven
   56   38  00111000 8      Eight
   57   39  00111001 9      Nine
   58   3A  00111010 :      Colon
   59   3B  00111011 ;      Semicolon
   60   3C  00111100 <      Less than
   61   3D  00111101 =      Equals
   62   3E  00111110 >      Greater than
   63   3F  00111111 ?      Question mark
   64   40  01000000 @      At sign
   65   41  01000001 A      Uppercase A
   66   42  01000010 B      Uppercase B
   67   43  01000011 C      Uppercase C
   68   44  01000100 D      Uppercase D
   69   45  01000101 E      Uppercase E
   70   46  01000110 F      Uppercase F
   71   47  01000111 G      Uppercase G
   72   48  01001000 H      Uppercase H
   73   49  01001001 I      Uppercase I
   74   4A  01001010 J      Uppercase J
   75   4B  01001011 K      Uppercase K
   76   4C  01001100 L      Uppercase L
   77   4D  01001101 M      Uppercase M
   78   4E  01001110 N      Uppercase N
   79   4F  01001111 O      Uppercase O
   80   50  01010000 P      Uppercase P
   81   51  01010001 Q      Uppercase Q
   82   52  01010010 R      Uppercase R
   83   53  01010011 S      Uppercase S
   84   54  01010100 T      Uppercase T
   85   55  01010101 U      Uppercase U
   86   56  01010110 V      Uppercase V
   87   57  01010111 W      Uppercase W
   88   58  01011000 X      Uppercase X
   89   59  01011001 Y      Uppercase Y
   90   5A  01011010 Z      Uppercase Z
   91   5B  01011011 [      Opening bracket
   92   5C  01011100 \      Backslash
   93   5D  01011101 ]      Closing bracket
   94   5E  01011110 ^      Caret - circumflex
   95   5F  01011111 _      Underscore
   96   60  01100000 `      Grave accent
   97   61  01100001 a      Lowercase a
   98   62  01100010 b      Lowercase b
   99   63  01100011 c      Lowercase c
   100  64  01100100 d      Lowercase d
   101  65  01100101 e      Lowercase e
   102  66  01100110 f      Lowercase f
   103  67  01100111 g      Lowercase g
   104  68  01101000 h      Lowercase h
   105  69  01101001 i      Lowercase i
   106  6A  01101010 j      Lowercase j
   107  6B  01101011 k      Lowercase k
   108  6C  01101100 l      Lowercase l
   109  6D  01101101 m      Lowercase m
   110  6E  01101110 n      Lowercase n
   111  6F  01101111 o      Lowercase o
   112  70  01110000 p      Lowercase p
   113  71  01110001 q      Lowercase q
   114  72  01110010 r      Lowercase r
   115  73  01110011 s      Lowercase s
   116  74  01110100 t      Lowercase t
   117  75  01110101 u      Lowercase u
   118  76  01110110 v      Lowercase v
   119  77  01110111 w      Lowercase w
   120  78  01111000 x      Lowercase x
   121  79  01111001 y      Lowercase y
   122  7A  01111010 z      Lowercase z
   123  7B  01111011 {      Opening brace
   124  7C  01111100 |      Vertical bar
   125  7D  01111101 }      Closing brace
   126  7E  01111110 ~      Equivalency sign - tilde
   127  7F  01111111 DEL    Delete

There is an extended ascii table, but I do not use it
  here since fasta/fastq use regular ascii
*/

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
