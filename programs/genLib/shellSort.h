/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' shellSort SOF: Start Of File
'   - holds functions for doing shell sorts on arrays
'   o note:
'     - most of this code is from
'        o Adam Drozdek. 2013. Data Structures and
'          Algorithims in c++. Cengage Leraning. fourth
'          edition. pages 505-508
'   o note: this is long, but is really a set of three
'     functions per data type
'     o xxSearch_shellSort
'       - search for query in a sorted array
'     o xxRange_shellSort
'       - search for start of range in a sorted array
'     o xx_shellSort
'       - sort an array by lowest value first
'   o header:
'     - guards
'   o fun01: ulSearch_shellSort
'     - search for query in unsigned long array
'   o fun02: ulRange_shellSort
'     - searches for range in unsinged long array
'   o fun03: ul_shellSort
'     - sorts a unsigned long array from least to greatest
'   o fun04: uiSearch_shellSort
'     - search for query in unsigned int array
'   o fun05: uiRange_shellSort
'     - searches for range in unsinged int array
'   o fun06: ui_shellSort
'     - sorts a unsigned int array from least to greatest
'   o fun07: us_Search_shellSort
'     - search for query in unsigned short array
'   o fun08: us_Range_shellSort
'     - searches for range in unsinged short array
'   o fun09: us_shellSort
'     - sorts unsigned short array from least to greatest
'   o fun10: uc_Search_shellSort
'     - search for query in unsigned char array
'   o fun11: uc_Range_shellSort
'     - searches for range in unsinged char array
'   o fun12: sl_shellSort
'     - sorts signed long array from least to greatest
'   o fun13: sl_Search_shellSort
'     - search for query in signed long array
'   o fun14: sl_Range_shellSort
'     - searches for range in singed long array
'   o fun15: sl_shellSort
'     - sorts signed long array from least to greatest
'   o fun16: si_Search_shellSort
'     - search for query in signed int array
'   o fun17: si_Range_shellSort
'     - searches for range in singed int array
'   o fun18: si_shellSort
'     - sorts signed int array from least to greatest
'   o fun19: ss_Search_shellSort
'     - search for query in signed short array
'   o fun20: ss_Range_shellSort
'     - searches for range in singed short array
'   o fun21: ss_shellSort
'     - sorts signed short array from least to greatest
'   o fun22: sc_Search_shellSort
'     - search for query in signed char array
'   o fun23: sc_Range_shellSort
'     - searches for range in singed char array
'   o fun24: sc_shellSort
'     - sorts signed char array from least to greatest
'   o fun25: dbl_shellSort
'     - sorts a double array from least to greatest
'   o fun26: uiTwinShort_kmerFind
'     - sorts a unsigned int array from least to greatest
'       and keeps it linked to a second unsigned int array
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef GENERAL_SHELL_SHORT_H
#define GENERAL_SHELL_SHORT_H

/*-------------------------------------------------------\
| Fun01: ulSearch_shellSort
|  - search for query in unsigned long array
| Input:
|  - ulAry:
|    o to unsigned int array
|  - qryUL:
|    o number to find
|  - lenArySL:
|    o length of ulAry (index 1)
| Output:
|  - Returns:
|    o index of qryUL in ulAry
|    o -1 if qryUL is not in ulAry
\-------------------------------------------------------*/
signed long
ulSearch_shellSort(
   unsigned long *ulAry,
   unsigned long qryUL,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun02: ulRange_shellSort
|   - searches for range in unsinged long array
| Input:
|   - ulAry:
|     o unsigned long array
|   - startUL:
|     o start of the range to look for
|   - endUL:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ulRange_shellSort(
   unsigned long *ulAry,
   unsigned long startUL,
   unsigned long endUL,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun03: ul_shellSort
|   - sorts a unsigned long array from least to greatest
| Input:
|   - ulAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ulAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ul_shellSort(
   unsigned long *ulAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun04: uiSearch_shellSort
|  - search for query in unsigned int array
| Input:
|  - uiAry:
|    o unsigned int array
|  - qryUI:
|    o number to find
|  - lenArySL:
|    o length of uiAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in uiAry
|    o -1 if qryUI is not in uiAry
\-------------------------------------------------------*/
signed long
uiSearch_shellSort(
   unsigned int *uiAry,
   unsigned int qryUI,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun05: uiRange_shellSort
|   - searches for range in unsinged int array
| Input:
|   - uiAry:
|     o unsigned int array
|   - startUI:
|     o start of the range to look for
|   - endUI:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
uiRange_shellSort(
   unsigned int *uiAry,
   unsigned int startUI,
   unsigned int endUI,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun06: ui_shellSort
|   - sorts a unsigned int array from least to greatest
| Input:
|   - uiAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o uiAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ui_shellSort(
   unsigned int *uiAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun07: usSearch_shellSort
|  - search for query in unsigned short array
| Input:
|  - usAry:
|    o unsigned short array
|  - qryUS:
|    o number to find
|  - lenArySL:
|    o length of usAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in usAry
|    o -1 if qryUI is not in usAry
\-------------------------------------------------------*/
signed long
usSearch_shellSort(
   unsigned short *usAry,
   unsigned short qryUS,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun08: usRange_shellSort
|   - searches for range in unsinged short array
| Input:
|   - usAry:
|     o unsigned short array
|   - startUS:
|     o start of the range to look for
|   - endUS:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
usRange_shellSort(
   unsigned short *usAry,
   unsigned short startUS,
   unsigned short endUS,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun09: us_shellSort
|   - sorts a unsigned short array from least to greatest
| Input:
|   - usAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o usAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
us_shellSort(
   unsigned short *usAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun10: ucSearch_shellSort
|  - search for query in unsigned char array
| Input:
|  - ucAry:
|    o unsigned char array
|  - qryUC:
|    o number to find
|  - lenArySL:
|    o length of ucAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in ucAry
|    o -1 if qryUI is not in ucAry
\-------------------------------------------------------*/
signed long
ucSearch_shellSort(
   unsigned char *ucAry,
   unsigned char qryUC,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun11: ucRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - ucAry:
|     o unsigned char array
|   - startUC:
|     o start of the range to look for
|   - endUC:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ucRange_shellSort(
   unsigned char *ucAry,
   unsigned char startUC,
   unsigned char endUC,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun12: uc_shellSort
|   - sorts a unsigned char array from least to greatest
| Input:
|   - ucAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ucAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
uc_shellSort(
   unsigned char *ucAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun13: slSearch_shellSort
|  - search for query in signed long array
| Input:
|  - slAry:
|    o signed long array
|  - qrySL:
|    o number to find
|  - lenArySL:
|    o length of slAry (index 1)
| Output:
|  - Returns:
|    o index of qryUI in slAry
|    o -1 if qryUI is not in slAry
\-------------------------------------------------------*/
signed long
slSearch_shellSort(
   signed long *slAry,
   signed long qrySL,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun14: slRange_shellSort
|   - searches for range in singed long array
| Input:
|   - slAry:
|     o signed long array
|   - startSL:
|     o start of the range to look for
|   - endSL:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
slRange_shellSort(
   signed long *slAry,
   signed long startSL,
   signed long endSL,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun15: sl_shellSort
|   - sorts a signed long array from least to greatest
| Input:
|   - slAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o slAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
sl_shellSort(
   signed long *slAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun16: siSearch_shellSort
|  - search for query in signed int array
| Input:
|  - siAry:
|    o signed int array
|  - qrySI:
|    o number to find
|  - lenArySL:
|    o length of siAry (index 1)
| Output:
|  - Returns:
|    o index of qrySI in siAry
|    o -1 if qrySI is not in siAry
\-------------------------------------------------------*/
signed long
siSearch_shellSort(
   signed int *siAry,
   signed int qrySI,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun17: siRange_shellSort
|   - searches for range in singed int array
| Input:
|   - siAry:
|     o signed int array
|   - startSI:
|     o start of the range to look for
|   - endSI:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
siRange_shellSort(
   signed int *siAry,
   signed int startSI,
   signed int endSI,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun18: si_shellSort
|   - sorts a signed int array from least to greatest
| Input:
|   - siAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o siAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
si_shellSort(
   signed int *siAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun19: ssSearch_shellSort
|  - search for query in signed short array
| Input:
|  - ssAry:
|    o signed short array
|  - qrySS:
|    o number to find
|  - lenArySL:
|    o length of ssAry (index 1)
| Output:
|  - Returns:
|    o index of qrySS in ssAry
|    o -1 if qrySS is not in ssAry
\-------------------------------------------------------*/
signed long
ssSearch_shellSort(
   signed short *ssAry,
   signed short qrySS,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun20: ssRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - ssAry:
|     o signed short array
|   - startSS:
|     o start of the range to look for
|   - endSS:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
ssRange_shellSort(
   signed short *ssAry,
   signed short startSS,
   signed short endSS,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun21: ss_shellSort
|   - sorts a signed short array from least to greatest
| Input:
|   - ssAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o ssAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
ss_shellSort(
   signed short *ssAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun22: scSearch_shellSort
|  - search for query in signed char array
| Input:
|  - scAry:
|    o signed char array
|  - qrySC:
|    o number to find
|  - lenArySL:
|    o length of scAry (index 1)
| Output:
|  - Returns:
|    o index of qrySC in scAry
|    o -1 if qrySC is not in scAry
\-------------------------------------------------------*/
signed long
scSearch_shellSort(
   signed char *scAry,
   signed char qrySC,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun23: scRange_shellSort
|   - searches for range in unsinged char array
| Input:
|   - scAry:
|     o signed char array
|   - startSC:
|     o start of the range to look for
|   - endSC:
|     o end of the range to look for
|   - lenArySL:
|     o length of qrySIPtr (index 1)
| Output:
|   - Returns:
|     o first index in the range
|     o -1 if no value is in the range
\-------------------------------------------------------*/
signed long
scRange_shellSort(
   signed char *scAry,
   signed char startSC,
   signed char endSC,
   signed long lenArySL
);

/*-------------------------------------------------------\
| Fun24: sc_shellSort
|   - sorts a signed char array from least to greatest
| Input:
|   - scAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o scAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
sc_shellSort(
   signed char *scAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun25: dbl_shellSort
|   - sorts a double array from least to greatest
| Input:
|   - dblAry:
|     o array to sort
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o scAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
dbl_shellSort(
   double *dblAry,
   unsigned long startUL,
   unsigned long endUL
);

/*-------------------------------------------------------\
| Fun26: uiTwinSort_shellSort
|   - sorts a unsigned int array from least to greatest
|     and keeps it linked to a second unsigned int array
| Input:
|   - uiAry:
|     o array to sort
|   - uiSecAry:
|     o second array to keep in order with uiAry
|   - startUL:
|     o first element to start sorting at
|   - endUL:
|     o last element to sort (index 0)
| Output:
|   - Modifies:
|     o uiAry to be sorted form least to greatest
|     o uiSecAry to be sorted by uiAry
\-------------------------------------------------------*/
void
uiTwinSort_shellSort(
   unsigned int *uiAry,
   unsigned int *uiSecAry,
   unsigned long startUL,
   unsigned long endUL
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
