call vars.bat

rmdir /s /q "%BUILD_PATH%"
md "%BUILD_PATH%/addons"
xcopy /s /i "%SCRIPT_DIR%/../keys" "%BUILD_PATH%/keys"
copy "%SCRIPT_DIR%/../LICENSE" "%BUILD_PATH%/LICENSE"
copy "%SCRIPT_DIR%/../README.md" "%BUILD_PATH%/README.md"
copy "%SCRIPT_DIR%/../mod.cpp" "%BUILD_PATH%/mod.cpp"

setlocal enabledelayedexpansion

FOR /D %%i IN (..\modules\*) DO (
    pboProject.exe +M=%BUILD_PATH% +P  +H +E=dayz +K=%SIGN_KEY% +C P:\%%~ni
)

endlocal