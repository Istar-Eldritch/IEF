
set "SCRIPT_DIR=%cd%"
set "DAYZ_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ\"
set "ADDON_BUILDER_PATH=%ProgramFiles(x86)%\Steam\steamapps\common\DayZ Tools\Bin\AddonBuilder\AddonBuilder.exe"

set "MOD_PATH=P:\Mods\@IEF"
rmdir /s /q "%MOD_PATH%"
md "%MOD_PATH%/addons"
xcopy /s /i "%SCRIPT_DIR%/../keys" "%MOD_PATH%/keys"
copy "%SCRIPT_DIR%/../LICENSE" "%MOD_PATH%/LICENSE"
copy "%SCRIPT_DIR%/../README.md" "%MOD_PATH%/README.md"
copy "%SCRIPT_DIR%/../mod.cpp" "%MOD_PATH%/mod.cpp"

set "includes=%SCRIPT_DIR%\buildincludes"

"%ADDON_BUILDER_PATH%" "P:\IE\Framework" "%MOD_PATH%\addons" -project="P:" -prefix="IE/Framework" -include=%includes% -clear -binarizeFullLogs -sign="%UserProfile%\Documents\Keys\IstarEldritch.biprivatekey"
