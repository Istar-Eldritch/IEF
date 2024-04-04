call vars.bat

if not exist "%SCRIPT_DIR%\mission" (
    mkdir "%SCRIPT_DIR%\mission"
    xcopy /s "%DAYZ_SERVER_PATH%\mpmissions\dayzOffline.chernarusplus" "%SCRIPT_DIR%\mission\dayzOffline.chernarusplus"
)

setlocal enabledelayedexpansion
set "mod=P:\Mods\@CF;P:\Mods\@DayZ-Community-Online-Tools"

FOR /D %%i IN (..\modules\*) DO (
    set filename=%%~ni
    set "mod=!mod!;P:\Mods\@!filename!"
)

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mission=%SCRIPT_DIR%\mission\dayzOffline.chernarusplus" "-mod=%mod%"^
 -server -filePatching "-profiles=%SCRIPT_DIR%\profiles" "-config=%SCRIPT_DIR%\serverDZ.cfg"

endlocal