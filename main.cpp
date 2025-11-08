#include <windows.h>
#include <iostream>
#include "keyboard.h"
#include <string>
#include <set>
#include "shortcuts.h"
#include <unordered_map>
#include <vector>
#include <cstdint>
static std::vector<Shortcut *> shortcuts(1024, nullptr);

static std::set<DWORD> pressed_keys;
static std::set<DWORD> shortcut_keys;
static bool flag = false;

size_t HashVK(std::set<DWORD>* vks)
{
    size_t seed = 0;
    std::hash<DWORD> hasher;

    for (DWORD vk : *vks)
    {
        seed ^= hasher(vk) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}


std::string VkToString(DWORD vkCode)
{
    UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    char name[128] = {0};   
    if (GetKeyNameTextA(scanCode << 16, name, sizeof(name)) > 0)
        return std::string(name);
    return "";
}

std::string VksToString(std::set<DWORD> *vks)
{
    std::string keys_str = "";
    bool first = true;
    for(const auto vk: *vks)
    {
        std::string vkStr = VkToString(vk);
        if(!first)
        {
            keys_str += " + ";
        }
        keys_str += vkStr;
        first = false;
    }
    return keys_str;
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
        std::string keys_str = VksToString(&pressed_keys);
        std::cout << keys_str << std::endl;
    }
    shortcut_keys.insert(vkCode);
    size_t hash_index = HashVK(&shortcut_keys) % shortcuts.size();

    Shortcut *current = shortcuts[hash_index];
    if(!current) return;
    if(current && current->next)
    {
        std::string keys_str = VksToString(&shortcut_keys);
        while(current->keys != keys_str)
        {
            current = current->next;
        }
    }
    if(current){
    // std::cout << "Hanging? " << hash_index <<  std::endl;
        std::cout << "Running Command: " << current->name << std::endl;
        system(current->cmd.c_str());
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

            char cont = '0';
            
            while (cont != 'y')
            {
                pressed_keys.clear();
                std::cout << "Press keys for shortcut: " << std::endl;
                flag = true;
                while (flag);
                    Sleep(10);
                if(pressed_keys.empty()){
                    std::cout << "EMPTY" << std::endl;
                    cont = 'n';
                    continue;
                }
                std::cout.flush();
                std::cout << "Are these the keys you want for your shortcut(y/n)?: ";
                std::cin.ignore();
                std::cin >> cont;
                std::cout << cont << std::endl;
            }

            std::string keys_str = VksToString(&pressed_keys);

            hash = HashVK(&pressed_keys);
            std::cout << hash << std::endl;
            pressed_keys.clear();

            std::cout << "Enter Shortcut Command: ";
            std::cin.ignore();
            std::getline(std::cin, cmd);

            SaveShortcut(shortcuts, name, hash, cmd, keys_str);

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