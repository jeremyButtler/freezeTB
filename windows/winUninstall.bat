:: remove installed programs/databases

if exist "%programFiles%\freezeTB" (
   rmdir /Q /S "%programFiles%\freezeTB"
)

if exist "%localAppData%\freezeTB" (
   rmdir /Q /S "%localAppData%\freezeTB"
)

:: rmdir removes directory
::   /S allows removal if directory is non-empty
::   /Q forces removal (do not ask user)

:: remove shortcut

if exit "%userprofile%\Desktop\freezeTB.lnk" (
 del "%userprofile%\Desktop\freezeTB.lnk"
)
