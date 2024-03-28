
set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "ADDON_BUILDER_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder"

set "MOD_PATH=P:\Mods\@IEF"
rmdir /s "%MOD_PATH%"
md "%MOD_PATH%/addons"
xcopy /s "%SCRIPT_DIR%/../keys" "%MOD_PATH%/keys"
copy "%SCRIPT_DIR%/../LICENSE" "%MOD_PATH%/LICENSE"
copy "%SCRIPT_DIR%/../README.md" "%MOD_PATH%/README.md"
copy "%SCRIPT_DIR%/../mod.cpp" "%MOD_PATH%/mod.cpp"

start /wait /D "%ADDON_BUILDER_PATH%" AddonBuilder.exe "P:\IE\Framework" "%MOD_PATH%\addons" -project="P:" -prefix="IE/Framework" -clear -binarizeFullLogs -sign="%UserProfile%\Documents\Keys\IstarEldritch.biprivatekey"
