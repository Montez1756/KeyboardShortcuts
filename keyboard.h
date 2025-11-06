#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>

typedef void CALLBACK CallbackFunc(DWORD);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI KeyboardAsync(LPVOID lpParam);
bool Create(CallbackFunc *cbDown, CallbackFunc *cbUp);
bool Destroy();


#endif