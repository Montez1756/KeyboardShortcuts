#include "keyboard.h"
#include <iostream>


static CallbackFunc *callbackdown;
static CallbackFunc *callbackup;

#ifdef _WIN32
#include <windows.h>
static HANDLE KeyboardThread;

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

void Create(CallbackFunc *cbDown, CallbackFunc *cbUp)
{
    callbackdown = cbDown;
    callbackup = cbUp;
    
    KeyboardThread = CreateThread(NULL, 0, KeyboardAsync, NULL, 0, NULL);
}

void Destroy()
{
    TerminateThread(KeyboardThread, 0);
    UnhookWindowsHookEx(hHook);
}
#endif
#ifdef __linux__
#include <thread>
#include <chrono>
#include <linux/input.h>
#include <fstream>
#include <atomic>

static std::ifstream in;
static std::thread t;
static std::atomic<bool> running{true};


void LowLevelKeyboardProc(__u16 vkCode, __s32 value)
{
    if(value == 1)
    {
        callbackdown(vkCode);
    }
    else if(value == 0)
    {
        callbackup(vkCode);
    }
}

void KeyboardAsync(void(*callback)(__u16, __s32))
{
    const char *device =  "/dev/input/event2";
    in = std::ifstream(device, std::ios::binary);

    if(!in.is_open())
    {
        std::cerr << "Failed to open " << device << " (try running as root using sudo)" << std::endl;
        exit(1);
    }
    input_event ie;
    while(running.load())
    {
        in.read(reinterpret_cast<char*>(&ie), sizeof(input_event));
        if(ie.type == EV_KEY)
        {
            callback(ie.code, ie.value);
            // std::cout << "time=" << ie.time.tv_sec << " type=" << ie.type << " code=" << ie.code << " value=" << ie.value << std::endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void Create(CallbackFunc *cbDown, CallbackFunc *cbUp){
    callbackdown = cbDown;
    callbackup = cbUp;

    t = std::thread(KeyboardAsync, LowLevelKeyboardProc);
}

void Destroy()
{
    running.store(false);
    in.close();
    t.join();
}
#endif