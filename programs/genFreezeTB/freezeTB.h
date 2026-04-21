/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' freezeTB SOF: Start Of File
'   - Process input reads for AMR(s) and MIRU lineages.
'     This also builds a cosenssu and prints out depths
'   o header:
'     o Included libraries
'   o .c st01: set_freezeTB
'     - has loose (not in structure) settings for freezeTB
'   o fun01: blank_set_freezeTB
'     - blanks a set_freezeTB stucture
'   o fun02: init_set_freezeTB
'     - initializes a set_freezeTB stucture
'   o fun04: freeStack_set_freezeTB
'     - frees variables in a set_freezeTB stack struct
'   o fun05: freeHeap_set_freezeTB
'     - frees a set_freezeTB stack struct
'   o fun06: pversion_freezeTB
'     - prints version number for freezeTB and |submodules
'   o fun07: pcitation_freezeTB
'     - prints citation for freezeTB
'   o fun08: pminimap2Citation_freezeTB
'     - prints the citation for minimap2
'   o .c fun09: phelp_freezeTB
'     - prints help message for freezeTB
'   o .c fun10: input_freezeTB
'     - gets user input
'   o .c fun11: mkAmrCoverageTbl_freezeTB
'     - makes the gene percent coverage table with drug
'       resisitance
'   o fun12: run_freezeTB:
'     - drives everything, but not fun12 (for tcltk)
'   o .h note01:
'     - windows enviromental variables
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   o guards
\-------------------------------------------------------*/

#ifndef FREEZE_TB_H
#define FREEZE_TB_H

/*-------------------------------------------------------\
| Fun07: pversion_freezeTB
|   - prints version number for freezeTB and |submodules
| Input:
|   - outFILE:
|     o file to print version numbers to
| Ouput:
|   - Prints:
|     o version numbers to outFILE
\-------------------------------------------------------*/
void
pversion_freezeTB(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun07: pcitation_freezeTB
|   - prints citation for freezeTB
| Input:
|   - outFILE:
|     o file to print citation to
| Ouput:
|   - Prints:
|     o the citation to outFILE
\-------------------------------------------------------*/
void
pcitation_freezeTB(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun08: pminimap2Citation_freezeTB
|   - prints the citation for minimap2
| Input:
|   - outFILE:
|     o file to print minimap2 citation to
| Ouput:
|   - Prints:
|     o minimap2 citation to outFILE
\-------------------------------------------------------*/
void
pMinimap2Citation_freezeTB(
   void *outFILE
);

/*-------------------------------------------------------\
| Fun12: run_freezeTB
|    - Analyze ONT sequenced TB reads
| Input:
|    - numArgsSI:
|      o Integer with number of argument the user input
|    - argAryStr:
|      o C-string array with the input arguments
| Output:
|    - Prints:
|      o all files for freezeTB command line program
|      o errors to stderr
|      o clustering progress to stderr
|    - Returns:
|      o 0 for no errors
|      o c-string with error message
\-------------------------------------------------------*/
signed char *
run_freezeTB(
   int numArgsSI,
   char *argAryStr[]
);

#endif

/* Note01:
     - Windows enviromental variables
` From: https://pureinfotech.com/list-environment-variables-windows-10/
`  %ALLUSERSPROFILE%           C:\ProgramData
`  %APPDATA%                   C:\Users\{username}\AppData\Roaming
`  %COMMONPROGRAMFILES%        C:\Program Files\Common Files
`  %COMMONPROGRAMFILES(x86)%   C:\Program Files (x86)\Common Files
`  %CommonProgramW6432%        C:\Program Files\Common Files
`  %COMSPEC%                   C:\Windows\System32\cmd.exe
`  %HOMEDRIVE%                 C:\
`  %HOMEPATH%                  C:\Users\{username}
`  %LOCALAPPDATA%              C:\Users\{username}\AppData\Local
`  %LOGONSERVER%               \\{domain_logon_server}
`  %PATH%                      C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem
`  %PathExt%                   .com;.exe;.bat;.cmd;.vbs;.vbe;.js;.jse;.wsf;.wsh;.msc
`  %PROGRAMDATA%               C:\ProgramData
`  %PROGRAMFILES%              C:\Program Files
`  %ProgramW6432%              C:\Program Files
`  %PROGRAMFILES(X86)%         C:\Program Files (x86)
`  %PROMPT%                    $P$G
`  %SystemDrive%               C:
`  %SystemRoot%                C:\Windows
`  %TEMP%                      C:\Users\{username}\AppData\Local\Temp
`  %TMP%                       C:\Users\{username}\AppData\Local\Temp
`  %USERDOMAIN%                Userdomain associated with current user.
`  %USERDOMAIN_ROAMINGPROFILE% Userdomain associated with roaming profile.
`  %USERNAME%                  {username}
`  %USERPROFILE%               C:\Users\{username}
`  %WINDIR%                    C:\Windows
`  %PUBLIC%                    C:\Users\Public
`  %PSModulePath%              %SystemRoot%\system32\WindowsPowerShell\v1.0\Modules\
`  %OneDrive%                  C:\Users\{username}\OneDrive
`  %DriverData%                C:\Windows\System32\Drivers\DriverData
`  %CD%                        Outputs current directory path. (Command Prompt.)
`  %CMDCMDLINE%                Outputs command line used to launch current Command Prompt session. (Command Prompt.)
`  %CMDEXTVERSION%             Outputs the number of current command processor extensions. (Command Prompt.)
`  %COMPUTERNAME%              Outputs the system name.
`  %DATE%                      Outputs current date. (Command Prompt.)
`  %TIME%                      Outputs time. (Command Prompt.)
`  %ERRORLEVEL%                Outputs the number of defining exit status of previous command. (Command Prompt.)
`  %PROCESSOR_IDENTIFIER%      Outputs processor identifier.
`  %PROCESSOR_LEVEL%           Outputs processor level.
`  %PROCESSOR_REVISION%        Outputs processor revision.
`  %NUMBER_OF_PROCESSORS%      Outputs the number of physical and virtual cores.
`  %RANDOM%                    Outputs random number from 0 through 32767.
`  %OS%                        Windows_NT
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
