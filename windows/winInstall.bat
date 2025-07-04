:: turn of printing each command
@echo off

:: pushes directory move to stack so can undo later
pushd %~dp0

set guiDir="..\programs\ftbTclTkSrc"
set foundBl=0
::set tclPath="blank"
::set tkPath="blank"
::set tcltkInclude="blank"

:: check if in a development shell (have cl.exe) or
:: try to actiavate the development shell settings

:: from stack overflow; find exectuable in path
::   if cl.exe is in path, then in a developer enverioment

for %%X in (cl.exe) do (set FOUND=%%~$PATH:X)

if defined FOUND (
   echo "in developer envrioment"
) else (
   if exist "%programFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
      CALL "%programFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
   ) else (
      echo could not find cl.exe
      goto :error
   )
)

:: the CALL command has called bat script operate in the
:: current bat scripts terminal, otherwise the other
:: script fires a separate terminal

:: Find tcltk path

::call set tcltkPath="%programFiles%\Tcl90\tk90t.lib"
::if exist "!tcltkPath!" (
::   goto :tk90_program
::)
::call set tcltkPath="%programFiles%\Tcl90\tcl9tk90t.lib" (
::if exist "!tcltkPath!" (
::   goto :tcl9tk90_program
::)
::call set tcltkPath="%programFiles%\Tcl86\tk86t.lib"
::if exist "!tcltkPath!" (
::   echo "found"
::   goto :tk86_program
::)
::call set tcltkPath="%programFiles%\Tcl86\tcl8tk86t.lib"
::if exist "!tcltkPath!" (
::   goto :tcl8tk86_program
::)
::call set tcltkPath="%localAppData%\Apps\Tcl90\tk90t.lib" 
::if exist "!tcltkPath!" (
::   goto :tk90_apps
::)
::call set tcltkPath="%localAppData%\Apps\Tcl90\tcl9tk90t.lib"
::if exist "!tcltkPath!" (
::   goto :tcl9tk90_apps
::)
::call set tcltkPath="%localAppData%\Apps\Tcl86\tk86t.lib"
::if exist "!tcltkPath!" (
::   goto :tk86_apps
::)
::call set tcltkPath="%localAppData%\Apps\Tcl86\tcl8tk86t.lib"
::if exist "!tcltkPath!" (
::   goto :tcl8tk86_apps
::)
::
:::: could not find tcltk case
::if %foundBl%==0 (
::  echo "could not find tcltk"
::  goto :error
::)
::
:::tk90_program
::   set foundBl=1
::   set tclPath="%programFiles%\Tcl90\lib\tcl90t.lib"
::   set tkPath="%programFiles%\Tcl90\lib\tk90t.lib"
::   set tcltkInclude="/I %programFiles%\Tcl90\include"
::   echo "found tcltk at %programFiles%\Tcl90"
::   goto :build
:::tcl9tk90_program
::   set foundBl=1
::   set tclPath="%programFiles%\Tcl90\lib\tcl90t.lib"
::   set tkPath="%programFiles%\Tcl90\lib\tcl9tk90t.lib"
::   set tcltkInclude="/I %programFiles%\Tcl90\include"
::   echo "found tcltk at %programFiles%\Tcl90"
::   goto :build
:::tk86_program
::   set foundBl=1
::   set tclPath="%programFiles%\Tcl86\lib\tcl86t.lib"
::   set tkPath="%programFiles%\Tcl86\lib\tk86t.lib"
::   set tcltkInclude="/I %programFiles%\Tcl86\include"
::   echo "found tcltk at %programFiles%\Tcl86"
::   goto :build
:::tcl8tk86_program
::   set foundBl=1
::   set tclPath="%programFiles%\Tcl86\lib\tcl86t.lib"
::   set tkPath="%programFiles%\Tcl86\lib\tcl8tk86t.lib"
::   set tcltkInclude="/I %programFiles%\Tcl86\include"
::   echo "found tcltk at %programFiles%\Tcl86"
::   goto :build
:::tk90_apps
::   set foundBl=1
::   set tclPath="%localAppData%\Apps\Tcl90\lib\tcl90t.lib"
::   set tkPath="%localAppData%\Apps\Tcl90\lib\tk90t.lib"
::   set tcltkInclude="/I %localAppData%\Apps\Tcl90\include"
::   echo "found tcltk at %localAppData%\Apps\Tcl90"
::   goto :build
:::tcl9tk90_apps
::   set foundBl=1
::   set tclPath="%localAppData%\Apps\Tcl90\lib\tcl90t.lib"
::   set tkPath="%localAppData%\Apps\Tcl90\lib\tcl9tk90t.lib"
::   set tcltkInclude="/I %localAppData%\Apps\Tcl90\include"
::   echo "found tcltk at %localAppData%\Apps\Tcl90"
::   goto :build
:::tk86_apps
::   set foundBl=1
::   set tclPath="%localAppData%\Apps\Tcl86\lib\tcl86t.lib"
::   set tkPath="%localAppData%\Apps\Tcl86\lib\tk86t.lib"
::   set tcltkInclude="/I %localAppData%\Apps\Tcl86\include"
::   echo "found tcltk at %localAppData%\Apps\Tcl86"
::   goto :build
:::tcl8tk86_apps
::   set foundBl=1
::   set tclPath="%localAppData%\Apps\Tcl86\lib\tcl86t.lib"
::   set tkPath="%localAppData%\Apps\Tcl86\lib\tcl8tk86t.lib"
::   set tcltkInclude="/I %localAppData%\Apps\Tcl86\include"
::   echo "found tcltk at %localAppData%\Apps\Tcl86"
::   goto :build
::
::goto :error
:::: compile freezeTB
:::build
::
:::: DELETE THIS PAUSE
::echo "tcltk is at %tcltkInclude%"
::echo "tcltk is at %tclPath%"
::echo "tcltk is at %tkPath%"
::pause

cd %guiDir%

if exist ..\..\windows\freezeTB\ftbTclTk.exe (
   del ..\..\windows\freezeTB\ftbTclTk.exe
) 
if exist ..\..\windows\freezeTB\graphAmpDepth.r (
   del ..\..\windows\freezeTB\graphAmpDepth.r
) 
if exist ..\..\windows\freezeTB\FTB-icon.ico (
   del ..\..\windows\freezeTB\FTB-icon.ico
) 

nmake /F mkfile.win
nmake /F mkfile.win clean

move ftbTclTk.exe ..\..\windows\freezeTB\
cd ..\..\windows
copy ..\scripts\graphAmpDepth.r freezeTB
copy ..\FTB-icon.ico freezeTB

:: The if statment is to detect if I can to install
::   globally or if I need to install localy

copy /y nul "%programFiles%\2025-02-11--test-file.txt"
if exist "%programFiles%\2025-02-11--test-file.txt" (
   del "%programFiles%\2025-02-11--test-file.txt"

   xcopy freezeTB "%programFiles%\freezeTB" /E /I
   xcopy ..\freezeTBFiles "%programFiles%\freezeTB\freezeTBFiles" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%programFiles%\freezeTB\ftbTclTk.exe" >> CreateShortcut.vbs
   echo oLink.Description = "TB AMR gene mutation detection" >> CreateShortcut.vbs
   echo oLink.IconLocation = "%programFiles%\freezeTB\FTB-icon.ico" >> CreateShortcut.vbs
   echo oLink.Save >> CreateShortcut.vbs
   cscript CreateShortcut.vbs
   del CreateShortcut.vbs
) else (
   ::: del "%localAppData%\2025-02-11--test-file.txt" ::: did not make it

   if exist "%localAppData%\freezeTB" (
      echo "found freezeTB directory"
   ) else ( mkdir "%localAppData%\freezeTB")

   xcopy freezeTB "%localAppData%\freezeTB" /E /I
   xcopy ..\freezeTBFiles "%localAppData%\freezeTB\freezeTBFiles" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%localAppData%\freezeTB\ftbTclTk.exe" >> CreateShortcut.vbs
   echo oLink.Description = "TB AMR gene mutation detection" >> CreateShortcut.vbs
   echo oLink.IconLocation = "%localAppData%\freezeTB\FTB-icon.ico" >> CreateShortcut.vbs
   echo oLink.Save >> CreateShortcut.vbs
   cscript CreateShortcut.vbs
   del CreateShortcut.vbs
)

:: the icon/shortcut trick was from
::   https://stackoverflow.com/questions/30028709/how-do-i-create-a-shortcut-via-command-line-in-windows

:: for xcopy /E for directory copy
:: for xcopy /I forces directory copy (skip asking user)


pause
goto :exit

:: return values

:error
pause

:exit

:: undo the initial directory move
popd
