@echo off
call shell
call build
:try_again
set /P "choice=Enter 1 to debug, 2 to run program, 3 to exit: "
echo %choice%|findstr /r "[^1-3]" && (
	echo enter a number
	goto :try_again
)
cd W:\HandmadeHero

if %choice% equ 1 (
    setlocal enableextensions enabledelayedexpansion
    set EXE=devenv.exe

    tasklist /FI "IMAGENAME eq !EXE!" /FO CSV > search.log

    FOR /F %%A IN (search.log) DO (
        SET /a VAR+=1
    )
    SET /a VAR-=1

    if !VAR! leq 1 (		
        devenv Build\Win32_Handmade.exe
    )
    if !VAR! gtr 1 (
        Rem activate window
        echo set WshShell = CreateObject("WScript.Shell"^) > Activate.vbs
        Rem build name is the app name 
        echo WshShell.AppActivate "Win32_Handmade - Microsoft Visual Studio" >> Activate.vbs
        echo WshShell.SendKeys "{F5}" >> Activate.vbs
        Rem echo WshShell.SendKeys "^{Esc} + 1" >> Activate.vbs
        Rem echo WshShell.SendKeys "^{Esc} + 1" >> Activate.vbs
        wscript Activate.vbs
        del Activate.vbs 
    )
    endlocal
)
if %choice% equ 2 (
    Build\Win32_Handmade.exe
)
exit