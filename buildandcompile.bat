@echo off
call Misc\shell
call build
:try_again
set /P "choice=Enter 1 to debug, 2 to run program, 3 to exit: "
echo %choice%|findstr /r "[^1-3]" && (
	echo enter a number
	goto :try_again
)
cd W:\HandmadeHero

if %choice% equ 1 (
	devenv Build\Win32_Handmade.exe
)
if %choice% equ 2 (
	Build\Win32_Handmade.exe
)
cmd /k