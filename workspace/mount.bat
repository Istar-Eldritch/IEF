call vars.bat

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Mount P "%DAYZ_DATA%"

setlocal enabledelayedexpansion

FOR /D %%i IN (..\modules\*) DO (
    set filename=%%~ni

    mklink /J "!DAYZ_PATH!\!filename!" "%%i"
    mklink /J "P:\!filename!" "%%i"
)

endlocal

mklink /J "P:\Mods" "%BUILT_MODS%"

@REM Community Framework
if not exist "%CODE_DEPENDENCIES%\DayZ-CommunityFramework" (
    git clone git@github.com:Arkensor/DayZ-CommunityFramework.git "%CODE_DEPENDENCIES%\DayZ-CommunityFramework"
)
mklink /J "%DAYZ_PATH%JM" "%CODE_DEPENDENCIES%\DayZ-CommunityFramework\JM"
mklink /J "P:\JM" "%CODE_DEPENDENCIES%\DayZ-CommunityFramework\JM"

@REM COT
if not exist "%CODE_DEPENDENCIES%\DayZ-CommunityOnlineTools" (
    git clone git@github.com:Jacob-Mango/DayZ-CommunityOnlineTools.git "%CODE_DEPENDENCIES%\DayZ-CommunityOnlineTools"
)
mklink /J "%DAYZ_PATH%JM\COT" "%CODE_DEPENDENCIES%\DayZ-CommunityOnlineTools\JM\COT"
mklink /J "P:\JM\COT" "%CODE_DEPENDENCIES%\DayZ-CommunityOnlineTools\JM\COT"