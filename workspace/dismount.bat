
set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"

rmdir "%DAYZ_PATH%\IE" /s /q
rmdir "P:\IE" /s /q

@REM Community Framework

rmdir "P:\JM\COT" /s /q
rmdir "%DAYZ_PATH%\JM" /s /q
rmdir "P:\JM" /s /q

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Dismount P
