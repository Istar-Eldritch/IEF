call vars.bat

start /wait /D "%WORKDRIVE_PATH%" WorkDrive.exe /Mount P "%UserProfile%\Documents\DayZData"

mklink /J "P:\Mods" "%BUILT_MODS%"