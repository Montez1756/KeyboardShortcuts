#include "keyboard.h"
#include <windows.h>
#include <iostream>

static HANDLE KeyboardThread;

static CallbackFunc *callbackdown;
static CallbackFunc *callbackup;
static HHOOK hHook{NULL};

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    DWORD vkCode = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode;

    if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        callbackdown(vkCode);
    }
    else if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
    {
        callbackup(vkCode);
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardAsync(LPVOID lpParam)
{
    MSG msg;

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    while(GetMessage(&msg, NULL, 0, 0))
    {

    }
    return true;
}

bool Create(CallbackFunc *cbDown, CallbackFunc *cbUp)
{
    callbackdown = cbDown;
    callbackup = cbUp;

    KeyboardThread = CreateThread(NULL, 0, KeyboardAsync, NULL, 0, NULL);
    return true;

}

bool Destroy()
{
    TerminateThread(KeyboardThread, 0);
    UnhookWindowsHookEx(hHook);
    return true;
}