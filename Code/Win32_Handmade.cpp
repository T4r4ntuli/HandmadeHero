/*  ===============================================
Project: HandmadeHero
File: Win32_Handmade.cpp
Creator: Tarantuli
Creation Date: 11/11/2017 4:14:20 PM

Description:
=============================================== */
#include <Windows.h>
#include "stdafx.h"

#define internal static
#define local_persist static 
#define global_variable static 

//typedef uint8_t uint8; == typedef unsigned char uint8;

namespace HandmadeHero
{
    //c type struct
    typedef struct
    {
        int x = 0;
    } TestStruct, *pTestStruct;

    //this is global for now
    global_variable bool running;

    global_variable BITMAPINFO bitmapInfo;
    global_variable void *bitmapMemory;
    global_variable int bitmapWidth;
    global_variable int bitmapHeight;

    internal void
    Win32ResizeDIPSection(int width, int height)
    {
        //todo bullethroof this.
        // maybe don't free first, free after, then free first if that fails.

        if(bitmapMemory)
        {
            VirtualFree(bitmapMemory, 0, MEM_RELEASE);
        }

        bitmapWidth = width;
        bitmapHeight = height;

        bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
        bitmapInfo.bmiHeader.biWidth = bitmapWidth;
        bitmapInfo.bmiHeader.biHeight = -bitmapHeight;
        bitmapInfo.bmiHeader.biPlanes = 1;
        bitmapInfo.bmiHeader.biBitCount = 32;
        bitmapInfo.bmiHeader.biCompression = BI_RGB;

        int bytesPerPixel = 4;
        int bitmapMemorySize = (bitmapWidth * bitmapHeight) * bytesPerPixel;
        bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

        int pitch = width*bytesPerPixel;
        uint8_t *row = (uint8_t *)bitmapMemory;
        for (int y = 0; y < bitmapHeight; ++y)
        {
            uint8_t *pixel = (uint8_t *)row;
            for (int x = 0; x < bitmapWidth; ++x)
            {
                /*                   
                    Pixel in memory: BB GG RR xx
                    LITTLE ENDIAN ARCHITECTURE!!
                    0X xxRRGGBB
                */
                *pixel = 0;
                ++pixel;

                *pixel = 0;
                ++pixel;

                *pixel = 255;
                ++pixel;

                *pixel = 0;
                ++pixel;
            }
            row += pitch;
        }
    }

    internal void
    Win32UpdateWindow(HDC deviceContext, RECT *windowRect)
    {
        int windowWidth = windowRect->right - windowRect->left;
        int windowHeight = windowRect->bottom - windowRect->top;
        StretchDIBits(deviceContext,
                      /*x, y, width, height,
                      x, y, width, height,*/
                      0, 0, bitmapWidth, bitmapHeight,
                      0, 0, windowWidth, windowHeight,
                      bitmapMemory,
                      &bitmapInfo,
                      DIB_RGB_COLORS,
                      SRCCOPY);
    }

    LRESULT CALLBACK
    Win32MainWindowCallBack(HWND windowHandle,
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
                RECT clientRect;
                GetClientRect(windowHandle, &clientRect);
                int width = clientRect.right - clientRect.left;
                int height = clientRect.bottom - clientRect.top;

                Win32ResizeDIPSection(width, height);
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

                RECT clientRect;
                GetClientRect(windowHandle, &clientRect);

                Win32UpdateWindow(deviceContext, &clientRect);
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
    windowClass.lpfnWndProc = HandmadeHero::Win32MainWindowCallBack;
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