#include <windows.h>
#include <iostream>
#include "keyboard.h"
#include <string>
#include <set>

static std::set<DWORD> pressed_keys;
static bool flag = false;


std::string VkToString(DWORD vkCode)
{
    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    char name[128] = {0};
    if (GetKeyNameTextA(scanCode << 16, name, sizeof(name)) > 0)
        return std::string(name);
    return "";
}



void CALLBACK HandleDwn(DWORD vkCode)
{
    if (flag)
    {
        pressed_keys.insert(vkCode);
        bool first = true;
        for (const auto c : pressed_keys)
        {
            std::string vkStr = VkToString(c);
            if (!first) std::cout << " + ";
            std::cout << vkStr;
            first = false;
        }
        std::cout << std::endl;
    }
}

void CALLBACK HandleUp(DWORD vkCode)
{
    //Do Nothing
}

int main(){
    bool hooked = Create(HandleDwn, HandleUp);
    if(!hooked)
    {
        return -1;
    }
    char c;
    do{
        std::cout << "Shortcuts:" << std::endl;
        //Print shortcuts here
        std::cout << "a. " << "Create Shortcut" << std::endl;
        std::cout << "c. " << "Change Shortcut" << std::endl;
        std::cout << "d. " << "Delete Shortcut" << std::endl;
        std::cout << "q. " << "Quit" << std::endl;

        std::cin >> c;
        switch (c)
        {
        case 'a':
            flag = true;
            while(flag);
            break;
        case 'q':
            break;
        default:
            break;
        }
    }
    while(c != 'q');
    Destroy();

    return 0;
}