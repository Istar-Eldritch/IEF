call vars.bat

setlocal enabledelayedexpansion

FOR /D %%i IN (..\modules\*) DO (
    set filename=%%~ni

    rmdir "!DAYZ_PATH!\!filename!" /s /q
    rmdir "P:\!filename!" /s /q
)

endlocal

@REM Community Framework

rmdir "%DAYZ_PATH%\JM\COT" /s /q
rmdir "P:\JM\COT" /s /q

rmdir "%DAYZ_PATH%\JM" /s /q
rmdir "P:\JM" /s /q

rmdir "P:\Mods" /s /q

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Dismount P