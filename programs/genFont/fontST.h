/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' fontST SOF: Start Of File
'   - handels loading and drawing png fonts
'   o header:
'     - forward declarations and guards
'   o .h st01: font_fontST
'     - holds the font to draw into the png
'   o fun01: blank_font_fontST
'     - blanks a font_fontST struct
'   o fun02: init_font_fontST
'     - initializes a font_fontST struct
'   o fun03: freeStack_font_fontST
'     - frees all variables in a font_fontST struct
'   o fun04: freeHeap_font_fontST
'     - frees a font_fontST struct
'   o fun05: getFont_font_fontST
'     - get a font from my ascii font file
'   o fun06: measureText_font_fontST
'     - measure how many pixels long a cstring is
'   o fun07: fontToC_font_fontST
'     - convert a font_fontST to to .c and .h files
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef FONT_STRUCTURE_LOADING_H
#define FONT_STRUCTURE_LOADING_H

#define def_asciiOffset_fontST 32
   /*converts visible + space range from 32 to 126 to
   `   0 to 93
   */
#define def_numAsciiChar_fontST 95
   /*number of ascii characters*/

/*-------------------------------------------------------\
| ST01: font_fontST
|   - holds the font to draw into the png
\-------------------------------------------------------*/
typedef struct
font_fontST{
   unsigned char *pixAryUC[def_numAsciiChar_fontST];
      /*has all visible ascii characters in a bitmap
      `  uses 1 for forground and 0 for background
      `Each bit is one pixel
      */
   signed short widthArySS[def_numAsciiChar_fontST];
      /*width of each character*/
   signed short lenArySS[def_numAsciiChar_fontST];
      /*number of bytes in pixAryUC*/

   signed short heightSS;
      /*height of all characters*/
   signed short gapSS; /*gap between characters*/
}font_fontST;

/*-------------------------------------------------------\
| Fun01: blank_font_fontST
|   - blanks a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to blank
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthAryUC to be
|       set to null/0
|     o frees all pionters in pixAryUC
\-------------------------------------------------------*/
void
blank_font_fontST(
   struct font_fontST *fontSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_font_fontST
|   - initializes a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to initialize
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthAryUC to be
|       set to null/0
\-------------------------------------------------------*/
void
init_font_fontST(
   struct font_fontST *fontSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_font_fontST
|   - frees all variables in a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all variables and initialize
\-------------------------------------------------------*/
void
freeStack_font_fontST(
   struct font_fontST *fontSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_font_fontST
|   - frees a font_fontST struct
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to free
| Output:
|   - Modifies:
|     o frees hte fontSTPtr struct, you must set the
|       pointer to 0/null
\-------------------------------------------------------*/
void
freeHeap_font_fontST(
   struct font_fontST *fontSTPtr
);

/*-------------------------------------------------------\
| Fun05: getFont_font_fontST
|   - get a font from my ascii font file
| Input:
|   - fontSTPtr:
|     o font_fontST struct pionter to get the new font
|   - fontFILE:
|     o FILE pointer to file to get the font from
| Output:
|   - Modifies:
|     o fontSTPtr to have the new font
|     o fontFILE to be at the end of the file
|   - Returns:
|     o 0 for no errors
|     o -1 for no height error
|     o -2 for memory errors
|     o -1 * (lineSL + 4) for duplicate character
|     o > 0 for line the problem is on
| File format:
|   - First line has `height:<size_in_pixels>` header
|   - Remaning lines in the file are blank or have a
|     character
|     o header: `:<character>:<width>:` (ex: `:A:16:`)
|     o character:
|       * a `#` is a filled pixel
|       * a ` ` is an empty pixel
|       * if  you have less then less then height lines
|         then they will be filled in at the bottom 
|       * max width is 504 pixels
\-------------------------------------------------------*/
signed long
getFont_font_fontST(
   struct font_fontST *fontSTPtr,
   void *fontFILE
);

/*-------------------------------------------------------\
| Fun06: measureText_font_fontST
|   - measure how many pixels long a cstring is
| Input:
|   - textStr:
|     o cstring with text to get the length of
|   - fontSTPtr:
|     o font_fontST struct pointer with the font to use
| Output:
|   - Returns:
|     o length in pixels of the text
\-------------------------------------------------------*/
signed short
measureText_font_fontST(
   signed char *textStr,
   struct font_fontST *fontSTPtr
);

/*-------------------------------------------------------\
| Fun07: fontToC_font_fontST
|   - convert a font_fontST to to .c and .h files
| Input:
|   - fontSTPtr:
|     o font_fontST struct pointer to convert to a .c and
|       .h file
|   - prefixStr:
|     o cstring with name of the font file
|     o must be a valid function name for c
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
| Output:
|   - Prints:
|     o code to auto load the font to a .c and .h file
|   - Returns:
|     o 0 for no errors
|     o 1 if prefixStr is not a valid function name
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
|     o 2 if could not open the .c file
|     o 3 if could not open the .h file
\-------------------------------------------------------*/
signed char
fontToC_font_fontST(
   struct font_fontST *fontSTPtr,
   signed char *prefixStr
);

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
