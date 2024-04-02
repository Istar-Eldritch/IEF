call vars.bat


setlocal enabledelayedexpansion

set "mod=P:\JM\CF\Scripts;P:\JM\COT\Scripts;P:\IE\Framework"

FOR /D %%i IN (..\modules\*) DO (
    set filename=%%~ni
    set "mod=!mod!;P:\!filename!"
)

start /D "%WORKBENCH_PATH%" workbenchApp.exe "-profiles=%SCRIPT_DIR%\Workbench" "-mod=%mod%"
endlocal