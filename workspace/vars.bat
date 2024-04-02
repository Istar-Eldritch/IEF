set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "DAYZ_SERVER_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZServer\"
set "ADDON_BUILDER_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"
set "WORKDRIVE_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkDrive"
set "WORKBENCH_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\WorkBench"

set "BUILT_MODS=%UserProfile%\Documents\Mods"

@REM This is where the code is kept, dependencies will be searched for here
set "CODE_DEPENDENCIES=%UserProfile%\Documents\DZ"

@REM Where the dayz files were extracted
set "DAYZ_DATA=%UserProfile%\Documents\DayZData"

@REM Where the mod will be placed
set "BUILD_PATH=%BUILT_MODS%\@IEF"

@REM The key that will be used to sign the pbos
set "SIGN_KEY=%UserProfile%\Documents\Keys\IstarEldritch.biprivatekey"
