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
'   o .c fun07: phelp_freezeTB
'     - prints help message for freezeTB
'   o .c fun08: input_freezeTB
'     - gets user input
'   o fun09: run_freezeTB:
'     - drives everything, but not fun09 (for tcltk)
'   o .h note01:
'     - windows enviromental variables
'   o license:
'     - licensing for this code (public dofun09 / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   o guards
\-------------------------------------------------------*/

#ifndef FREEZE_TB_H
#define FREEZE_TB_H

/*-------------------------------------------------------\
| Fun06: pversion_freezeTB
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
| Fun04: run_freezeTB
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
