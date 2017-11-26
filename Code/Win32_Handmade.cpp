/*  ===============================================
Project: HandmadeHero
File: Win32_Handmade.cpp
Creator: Tarantuli
Creation Date: 11/11/2017 4:14:20 PM

Description:
=============================================== */
#include <Windows.h>

#define internal static
#define local_persist static 
#define global_variable static 

namespace HandmadeHero
{
    global_variable bool running;
    //c type struct
    typedef struct
    {
        int x = 0;
    } TestStruct, *pTestStruct;

    LRESULT CALLBACK
        MainWindowCallBack(HWND windowHandle,
                           UINT message,
                           WPARAM wParam,
                           LPARAM lParam)
    {
        LRESULT Result = 0;

        switch (message)
        {
            case WM_CREATE:
            {
                OutputDebugStringA("WM_CREATE\n");
            } break;

            case WM_SIZE:
            {
                OutputDebugStringA("WM_SIZE\n");
            } break;

            case WM_DESTROY:
            {
                running = false;
                //Handle this as an error
            } break;

            case WM_CLOSE:
            {
                //Handle this with a message to the user
                running = false;
            } break;

            case WM_ACTIVATEAPP:
            {
                OutputDebugStringA("WM_ACTIVATEAPP\n");
            } break;

            case WM_PAINT:
            {
                PAINTSTRUCT paint;

                HDC deviceContext = BeginPaint(windowHandle, &paint);
                int x = paint.rcPaint.left;
                int y = paint.rcPaint.top;
                int height = paint.rcPaint.bottom - x;
                int width = paint.rcPaint.right - y;
                DWORD operation = WHITENESS;
                PatBlt(deviceContext, x, y, width, height, operation);

                EndPaint(windowHandle, &paint);
            } break;

            default:
            {
                //OutputDebugStringA("default\n");
                Result = DefWindowProc(windowHandle, message, wParam, lParam);
            } break;
        }
        return(Result);
    }
}
int CALLBACK
WinMain(HINSTANCE instance,
        HINSTANCE prevInstance,
        LPSTR commandLine,
        int showCode)
{
    HandmadeHero::TestStruct test;
    test.x = 3;

    WNDCLASS windowClass = {};
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = HandmadeHero::MainWindowCallBack;
    windowClass.hInstance = instance;
    //windowClass.hIcon = ;
    windowClass.lpszClassName = "HandmadeHeroWindowClass";

    if (RegisterClass(&windowClass))
    {
        HWND windowHandle = CreateWindowEx(
            0,
            windowClass.lpszClassName,
            "Handmade Hero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            instance,
            0);
        if (windowHandle)
        {
            MSG message;
            HandmadeHero::running = true;
            while (HandmadeHero::running)
            {
                BOOL messageResult = GetMessage(&message, 0, 0, 0);
                if (messageResult > 0)
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } else { break; }
            }
        } else
        {

        }
    } else
    {

    }
    return (0);
}