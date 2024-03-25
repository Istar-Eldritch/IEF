
set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Mount P "%UserProfile%\Documents\DayZData"

mkdir "%DAYZ_PATH%\IE"
mklink /J "%DAYZ_PATH%\IE\Framework" "%SCRIPT_DIR%\..\mod"
mkdir "P:\IE"
mklink /J "P:\IE\Framework" "%SCRIPT_DIR%\..\mod"

mklink /J "P:\Mods" "%UserProfile%\Documents\Mods"

@REM Community Framework

mklink /J "%DAYZ_PATH%JM" "%UserProfile%\Documents\DZ\CommunityFramework\JM"
mklink /J "P:\JM" "%UserProfile%\Documents\DZ\CommunityFramework\JM"

@REM COT

mklink /J "%DAYZ_PATH%JM\COT" "%UserProfile%\Documents\DZ\DayZ-CommunityOnlineTools\JM\COT"
mklink /J "P:\JM\COT" "%UserProfile%\Documents\DZ\DayZ-CommunityOnlineTools\JM\COT"