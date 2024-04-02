call vars.bat

set "mod=P:\Mods\@CF;P:\Mods\@DayZ-Community-Online-Tools"

FOR /D %%i IN (..\modules\*) DO (
    set filename=%%~ni
    set "mod=!mod!;P:\Mods\@!filename!"
)

start /D "%DAYZ_PATH%" DayZDiag_x64.exe "-mod=%mod%"^
 "-name=Istar Eldritch" -filePatching -connect=127.0.0.1 -port=2302 -window
