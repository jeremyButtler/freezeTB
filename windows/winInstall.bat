:: turn of printing each command
@echo off

:: pushes directory move to stack so can undo later
pushd %~dp0

set guiDir="..\programs\guiFreezeTBSrc"

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
) else (goto :error))

:: the CALL command has called bat script operate in the
:: current bat scripts terminal, otherwise the other
:: script fires a separate terminal

:: compile freezeTB
cd %guiDir%
nmake /F mkfile.win
nmake /F mkfile.win clean
move guiFreezeTB.exe ..\..\windows\freezeTB\
cd ..\..\windows

:: install locally

copy /y nul "%programFiles%\2025-02-11--test-file.txt"
if exist "%programFiles%\2025-02-11--test-file.txt" (
   del "%programFiles%\2025-02-11--test-file.txt"
   xcopy freezeTB "%programFiles%\freezeTB" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%programFiles%\freezeTB\guiFreezeTB.exe" >> CreateShortcut.vbs
   echo oLink.Description = "TB AMR gene mutation detection" >> CreateShortcut.vbs
   echo oLink.IconLocation = "%programFiles%\freezeTB\FTB-icon.ico" >> CreateShortcut.vbs
   echo oLink.Save >> CreateShortcut.vbs
   cscript CreateShortcut.vbs
   del CreateShortcut.vbs
) else (
   xcopy freezeTB "%localAppData%\freezeTB" /E /I

   echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
   echo sLinkFile = "%userprofile%\Desktop\freezeTB.lnk" >> CreateShortcut.vbs
   echo Set oLink = oWS.CreateShortcut(sLinkFile) >> CreateShortcut.vbs
   echo oLink.TargetPath = "%localAppData%\freezeTB\guiFreezeTB.exe" >> CreateShortcut.vbs
   echo oLink.Description = "TB AMR gene mutation detection" >> CreateShortcut.vbs
   echo oLink.IconLocation = "%localAppData%\freezeTB\FTB-icon.ico" >> CreateShortcut.vbs
   echo oLink.Save >> CreateShortcut.vbs
   cscript CreateShortcut.vbs
   del CreateShortcut.vbs
)

:: the icon/shortcut trick was from
::   https://stackoverflow.com/questions/30028709/how-do-i-create-a-shortcut-via-command-line-in-windows

:: for xcopoy /E for directory copy
:: for xcopoy /I forces directory copy (skip asking user)


goto :exit

:: return values

:error
echo could not find cl.exe
pause

:exit

:: undo the initial directory move
popd
