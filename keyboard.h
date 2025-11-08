#ifndef KEYBOARD_H
#define KEYBOARD_H


#ifdef _WIN32
#include <windows.h>
typedef DWORD __key_t;

typedef void CallbackFunc(DWORD);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI KeyboardAsync(LPVOID lpParam);
#endif


#ifdef __linux__
#include <linux/input.h>
#define __key_t __u16
// typedef __u16 ___key_t;
typedef void CallbackFunc(__u16);
void LowLevelKeyboardProc(__u16 code, __s32 value);
void KeyboardAsync(void(*callback)(__u16, __s32));
#endif

void Create(CallbackFunc *cbDown, CallbackFunc *cbUp);
void Destroy();



#endif