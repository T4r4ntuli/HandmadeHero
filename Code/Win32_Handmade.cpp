/*  ===============================================
	Project: $projectname$
	File: Win32_Handmade.cpp
	Creator: Tarantuli
	Creation Date: 11/11/2017 4:14:20 PM

	Description:
    =============================================== */

#include <Windows.h>

int CALLBACK 
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	MessageBox(0, "This is Handmade Hero!!", "Handmade Hero", MB_OK | MB_ICONINFORMATION);

	return 0;
}