/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hardFtbWdig SOF: Start Of File
'   - has the hardcoded parts of the rayWidg that is
'     needed for ftbRaylib
'     * drawing function
'     * check if click event function
'     * rayWidg gui structure for drawing function
'   o header:
'     - guards, declare widg_rayWidg, max entry box width
'   o .h st01: gui_ftbRayST
'     - has widgets used in the ftbRaylib GUI
'     - TODO: add output gui
'   o fun01: blank_gui_ftbRayST
'     - blanks a gui_ftbRayST structure to default values
'   o fun02: init_gui_ftbRayST
'     - initialize a gui_ftbrayst structure to default
'       values (really just blank)
'   o fun03: freeStack_gui_ftbRayST
'     - free variables in a gui_ftbRayST
'   o fun04: freeHeap_gui_ftbRayST
'     - frees a gui_ftbRayST structure
'   o .c fun05: drawGUI_ftbRayST
'     - draws the gui for a gui_ftbRayST structure
'   o fun06: initRayGui_ftbRayST
'     - initialize raylibs GUI
'   o .c fun07: hideInput_ftbRayST
'     - hides the input menu
'   o .c fun08: hideOutput_ftbRayST
'     - hides the ouput menu
'   o .c fun09: hideReport_ftbRayST
'     - hides the report menu
'   o .c fun10: hideTable_ftbRayST
'     - hides the amr table menu
'   o .c fun11: hideHsp65_ftbRayST
'     - hides the hsp65 and user lineage table menu
'   o .c fun12: hideGeneCover_ftbRayST
'     - hides the gene coverage table
'   o .c fun13: spoligoLinGet_ftbRayST
'     - gets the spoligotype lineage and sets the spoligo
'   o .c fun14: miruLinGet_ftbRayST
'     - gets the MIRU-VNTR lineage & sets miru text output
'   o .c fun15: checkDrugs_ftbRayST
'     - builds the drug resistance part of the ftb report
'   o .c fun16: getDatabases_ftbRayST
'     - get database files for freezeTB (currently Mac)
'   o .c fun17: mkCoverageTbl_ftbRayST
'     - makes the gene percent coverage table
'   o .c fun18: getHsp65Lin_ftbRayST
'     - get the getLin hsp65 lineages (an others)
'   o fun19: checkRunEvent_ftbRayST
'     - checks for an event, and if can runs found event
'     - also redraws the GUI
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards, declare widg_rayWidg, max entry box width
\-------------------------------------------------------*/

#ifndef FREEZE_TB_RAYLIB_STRUCT_H
#define FREEZE_TB_RAYLIB_STRUCT_H

struct widg_rayWidg;
struct str_ptrAry;

#define def_numDrugs_ftbRayST 17

/*-------------------------------------------------------\
| ST01: gui_ftbRayST
|   - has widgets used in the ftbRaylib GUI
|   - TODO: add output gui
\-------------------------------------------------------*/
typedef struct gui_ftbRayST
{
   /*_________________general GUI_______________________*/

   struct widg_rayWidg *widgSTPtr; /*has widgets*/

   signed char mesgStr[128];
      /*message for the message box*/

   signed char screenIndexSC;
      /*0: on input screen
      ` 1: on output file load screen
      ` 2: on output report screen
      ` 3: on output amr table screen
      ` 4: on output graph screen (does not exist)
      */
   signed char appDirStr[1024]; /*directory of ftbRay*/
   signed char curDirStr[4096]; /*current location*/

   signed char blinkSC;       /*status of cursor blink*/
   signed int numFramesSI;    /*number of frames*/

   /*_________________menu buttons______________________*/

   signed int inputGuiIdSI; /*input GUI button*/
   signed int outGuiIdSI;   /*output GUI button*/
   signed int reportGuiIdSI;/*report GUI button*/
   signed int amrsGuiIdSI;  /*amr list GUI button*/
   signed int hsp65GuiIdSI; /*hsp65 species GUI button*/
   signed int coverGuiIdSI; /*gene % coverage GUI button*/

   /*_________________input GUI_________________________*/

   /*for fastq input button*/
   struct str_ptrAry *fqStrSTPtr;
      /*c-string array with fastq files*/

   /*prefix entry box*/
   signed char inPrefixStr[128]; /*input prefix name*/
   signed int prefixLenSI;       /*length of text*/
   signed int prefixPosArySI[2]; /*scroll and cursor pos*/
      /*0 is scroll position, 1 is cursor position*/

   /*output button*/
   signed char outDirStr[1024];

   /*for configuration file button*/
   signed char configFileStr[1024];

   /*input widget ids*/
   signed int mesgBoxIdSI;
   signed int fileBrowserIdSI;
   signed int fqButIdSI;
   signed int fqLabIdSI;
   signed int prefixLabIdSI;
   signed int prefixEntryIdSI;
   signed int outDirIdSI;
   signed int outDirLabIdSI;
   signed int configIdSI;
   signed int configLabIdSI;
   signed int runIdSI;

   signed char browserSC;
      /*0 for fq, 1 for output directory, 3 for config*/
   struct files_rayWidg *fqFileSTPtr;
      /*for fastq file browser*/
   struct files_rayWidg *outDirSTPtr;
      /*for getting output directory*/
   struct files_rayWidg *configFileSTPtr;
      /*for getting configuration directory*/

   signed char fileMesgStr[128];

   /*_________________output GUI________________________*/

   /*output widget ids*/
   signed int getPrefixButIdSI;
   signed int getPrefixLabIdSI;
      /*gets file to get prefix from*/
   signed int minAmrPercEntryIdSI;
   signed int minAmrPercLabIdSI;
      /*minimum AMR % support*/
   signed int minAmrIndelPercEntryIdSI;
   signed int minAmrIndelPercLabIdSI;
      /*minimum AMR indel % support*/
   signed int getOutButSI;
      /*get output and build report*/

   struct files_rayWidg *oldFtbFileSTPtr;
      /*for FTB output file browser*/
   signed char filePrefixStr[128]; /*prefix for files*/

   /*minimum AMR percent text*/
   signed char amrSupStr[128]; /*minimum amr % support*/
   signed int amrSupLenSI;       /*length of text*/
   signed int amrSupPosArySI[2]; /*scroll and cursor pos*/
      /*0 is scroll position, 1 is cursor position*/

   /*minimum AMR indel percent text*/
   signed char indelSupStr[128]; /*minimum amr % support*/
   signed int indelSupLenSI;       /*length of text*/
   signed int indelSupPosArySI[2]; /*scroll & cursor pos*/
      /*0 is scroll position, 1 is cursor position*/
   
   /*_________________report_GUI________________________*/
   signed int reportPrefixLabIdSI;
   signed int spoligoLabIdSI;  /*spoligotype reporting*/
   signed int spoligoStrainLabIdSI;
   signed int spoligoLineageLabIdSI;
   signed int miruLabIdSI;     /*miru lineage reporting*/
   signed int drugResRectIdSI; /*first item in list*/

   unsigned int drugBackColUI[def_numDrugs_ftbRayST];
   unsigned int drugTextColUI[def_numDrugs_ftbRayST];

   signed char spoligoStr[256]; /*gets spoligotype*/
   signed char spoligoStrainStr[256];
   signed char spoligoLineageStr[256];

   signed char miruStr[256];    /*gets MIRU-VNTR lineage*/

   /*__________________table_GUI________________________*/
   signed int amrTblIdSI;
   signed int amrLabIdSI;
   struct listBox_rayWidg *amrListSTPtr;

   /*_______________hsp65_species_GUI___________________*/
   signed int hsp65TblIdSI;
   signed int hsp65LabIdSI;
   struct listBox_rayWidg *hsp65ListSTPtr;

   /*_______________gene_coverage_GUI___________________*/
   signed int geneCoverTblIdSI;
   signed int geneCoverLabIdSI;
   struct listBox_rayWidg *geneCoverSTPtr;
}gui_ftbRayST;

/*-------------------------------------------------------\
| Fun01: blank_gui_ftbRayST
|   - blanks a gui_ftbRayST structure to default values
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to blank
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
blank_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_gui_ftbRayST
|   - initialize a gui_ftbRayST structure to default
|     values (really just blank)
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to initialize
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
init_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_gui_ftbRayST
|   - free variables in a gui_ftbRayST
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with variables to free
| Output:
|   - Modifies:
|     o all values in gui_ftbRayST to default values
\-------------------------------------------------------*/
void
freeStack_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_gui_ftbRayST
|   - frees a gui_ftbRayST structure
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer to free
| Output:
|   - Modifies:
|     o frees guiSTPtr, you must set to 0/null
\-------------------------------------------------------*/
void
freeHeap_gui_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
);

/*-------------------------------------------------------\
| Fun05: drawGUI_ftbRayST
|   - draws the gui for a gui_ftbRayST structure
| Input:
|   - voidGuiSTPtr:
|     o gui_ftbRayST struct pointer with gui to draw
|       sent in as void
| Output:
|   - Draws:
|     o current GUI state to screen
\-------------------------------------------------------*/
void
draw_gui_ftbRayST(
   void *voidGuiSTPtr
);

/*-------------------------------------------------------\
| Fun06: initRayGui_ftbRayST
|   - makes a heap allocated gui_ftbRayST struct and draws
|     the initial GUI
| Input:
| Output:
|   - Draws:
|     o initial gui to screen
|   - Returns:
|     o heap allocated gui_ftbRayST struct with widgets
|     o 0 for memory errors
\-------------------------------------------------------*/
struct gui_ftbRayST *
mk_gui_ftbRayST(
   void
);

/*-------------------------------------------------------\
| Fun19: checkRunEvent_ftbRayST
|   - checks for an event, and if can runs the found event
|   - also redraws the GUI
| Input:
|   - guiSTPtr:
|     o gui_ftbRayST struct pointer with gui
| Output:
|   - Draws:
|     o current GUI state during and after event to screen
|   - Modifies:
|     o state and other widget related variables in
|       guiSTPtr
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
checkRunEvent_ftbRayST(
   struct gui_ftbRayST *guiSTPtr
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
