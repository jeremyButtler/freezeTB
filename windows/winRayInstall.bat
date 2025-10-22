:: turn of printing each command
@echo off

:: pushes directory move to stack so can undo later
pushd %~dp0

set guiDir="..\programs\ftbRaylibSrc"
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

cd %guiDir%

if exist ..\..\windows\freezeTB\ftbRay.exe (
   del ..\..\windows\freezeTB\ftbRay.exe
) 
if exist ..\..\windows\freezeTB\FTB-icon.ico (
   del ..\..\windows\freezeTB\FTB-icon.ico
) 

nmake /F mkfile.win
nmake /F mkfile.win clean
cd ..\..\windows

move %guiDir%\ftbRay.exe .
copy ..\FTB-icon.ico FTB-icon.ico

:: The if statment is to detect if I can to install
::   globally or if I need to install localy

copy /y nul "%programFiles%\2025-02-11--test-file.txt"
if exist "%programFiles%\2025-02-11--test-file.txt" (
   del "%programFiles%\2025-02-11--test-file.txt"

   if not exist "%programFiles%\freezeTB" (
      mkdir "%programFiles%\freezeTB"
   )

   move ftbRay.exe "%programFiles%\freezeTB"
   xcopy ..\freezeTBFiles "%programFiles%\freezeTB\freezeTBFiles" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%programFiles%\freezeTB\ftbRay.exe" >> CreateShortcut.vbs
   echo oLink.Description = "TB AMR gene mutation detection" >> CreateShortcut.vbs
   echo oLink.IconLocation = "%programFiles%\freezeTB\FTB-icon.ico" >> CreateShortcut.vbs
   echo oLink.Save >> CreateShortcut.vbs
   cscript CreateShortcut.vbs
   del CreateShortcut.vbs
) else (
   ::: del "%localAppData%\2025-02-11--test-file.txt" ::: did not make it

   if not exist "%localAppData%\freezeTB" (
      mkdir "%localAppData%\freezeTB"
   )

   move ftbRay.exe "%localAppData%\freezeTB"
   xcopy ..\freezeTBFiles "%localAppData%\freezeTB\freezeTBFiles" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%localAppData%\freezeTB\ftbRay.exe" >> CreateShortcut.vbs
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
