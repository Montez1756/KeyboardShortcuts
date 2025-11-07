#include <windows.h>
#include <iostream>
#include "keyboard.h"
#include <string>
#include <set>
#include "shortcuts.h"
#include <unordered_map>
#include <vector>
#include <cstdint>
static std::vector<Shortcut *> shortcuts(50, NULL);

static std::set<DWORD> pressed_keys;
static std::set<DWORD> shortcut_keys;
static bool flag = false;

size_t HashVK(std::set<DWORD>* vks)
{
    size_t result = 0;
    for (DWORD vk : *vks)
    {
        result += vk;
    }
    return result;

}


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
        if (vkCode == 0x0D)
        {
            flag = false;
            return;
        }
        pressed_keys.insert(vkCode);
        bool first = true;
        for (const auto c : pressed_keys)
        {
            std::string vkStr = VkToString(c);
            if (!first)
                std::cout << " + ";
            std::cout << vkStr;
            first = false;
        }
        std::cout << std::endl;
    }
    shortcut_keys.insert(vkCode);
    size_t hash_index = HashVK(&shortcut_keys) % 50;

    Shortcut *s = shortcuts[hash_index];
    // std::cout << "Hanging? " << hash_index <<  std::endl;
    if(s){
        std::cout << "Running Command: " << s->name << std::endl;
        system(s->cmd.c_str());
    }

}

void CALLBACK HandleUp(DWORD vkCode)
{
    shortcut_keys.erase(vkCode);
}


int main()
{
    LoadShortcuts(shortcuts);
    bool hooked = Create(HandleDwn, HandleUp);
    if (!hooked)
    {
        return -1;
    }
    char c;
    do
    {
        std::cout << "Shortcuts:" << std::endl;

        for(const auto &s: shortcuts)
        {
            if(s){
                std::cout << "\t" << s->name << ":" << std::endl;
                std::cout << "\t\t" << "Keys: " << s->keys << std::endl;
                std::cout << "\t\t" << "Command: " <<s->cmd << std::endl;
            }
        }
        // Print shortcuts here
        std::cout << "a. " << "Create Shortcut" << std::endl;
        std::cout << "c. " << "Change Shortcut" << std::endl;
        std::cout << "d. " << "Delete Shortcut" << std::endl;
        std::cout << "q. " << "Quit" << std::endl;

        std::cin >> c;
        switch (c)
        {
        case 'a':
        {
            std::string name;
            std::string cmd;
            size_t hash;

            std::cout << "Enter Shortcut Name: ";
            std::cin.ignore();
            std::getline(std::cin, name);

            std::cout << "Press keys for shortcut: " << std::endl;
            flag = true;
            while (flag);
            
            char cont;
            std::cout << "Are these the keys you want for your shortcut(y/n)?: ";
            std::cin.ignore();
            std::cin >> cont;
            
            while (pressed_keys.empty() || cont != 'y')
            {
                pressed_keys.clear();
                std::cout << "Enter Keys again..." << std::endl;
                flag = true;
                while (flag);
                std::cout << "Are these the keys you want for your shortcut(y/n)?: ";
                std::cin.ignore();
                std::cin >> cont;
            }
            std::string keys_str;
            for(const auto vk: pressed_keys)
            {
                keys_str += VkToString(vk) + " ";
            }

            hash = HashVK(&pressed_keys);
            std::cout << hash << std::endl;
            pressed_keys.clear();

            std::cout << "Enter Shortcut Command: ";
            std::cin.ignore();
            std::getline(std::cin, cmd);

            Shortcut *s = new Shortcut;
            s->name = name;
            s->keys = keys_str;
            s->cmd = cmd;
            shortcuts[hash % 50] = s;
            SaveShortcut(name, hash, cmd, keys_str);
            break;
        }
        case 'q':
            break;
        default:
            break;
        }
    } while (c != 'q');
    Destroy();

    return 0;
}