@echo off

mkdir ..\Build
pushd ..\Build
cl -Zi ..\Code\Win32_Handmade.cpp user32.lib gdi32.lib
popd