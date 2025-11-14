#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <set>
#include <string>

#ifdef _WIN32
#include <windows.h>
typedef DWORD e_key_t;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI KeyboardAsync(LPVOID lpParam);
std::string VkToString(DWORD vkCode);
#endif


#ifdef __linux__
#include <linux/input.h>
typedef __u16 e_key_t;
// #define e_key_t __u16
#define R_KEY KEY_ENTER
// typedef __u16 _e_key_t;
void LowLevelKeyboardProc(__u16 code, __s32 value);
void KeyboardAsync(void(*callback)(__u16, __s32));
#endif

std::string VkToString(e_key_t vkCode);
typedef void CallbackFunc(e_key_t);
size_t HashVK(std::set<e_key_t> &vks);
std::string VksToString(std::set<e_key_t> &vks);
void Create(CallbackFunc *cbDown, CallbackFunc *cbUp);
void Destroy();



#endif