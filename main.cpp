#include <iostream>
#include "keyboard.h"
#include <string>
#include <set>
#include "shortcuts.h"
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>

static std::vector<Shortcut *> shortcuts(1024, nullptr);
static std::set<e_key_t> pressed_keys;
static std::set<e_key_t> shortcut_keys;
static bool flag = false;


void HandleDwn(e_key_t vkCode)
{
    if (flag)
    {
        if (vkCode == R_KEY)
        {
            flag = false;
            return;
        }
        pressed_keys.insert(vkCode);
        std::cout << VksToString(pressed_keys) << std::endl;
    }
    shortcut_keys.insert(vkCode);
    size_t hash_index = HashVK(shortcut_keys) % shortcuts.size();

    Shortcut *current = shortcuts[hash_index];
    if (!current)
        return;
    if (current && current->next)
    {
        std::string keys_str = VksToString(shortcut_keys);
        while (current && current->keys != keys_str)
        {
            current = current->next;
        }
    }
    if (current)
    {
        // std::cout << "Hanging? " << hash_index <<  std::endl;
        std::cout << "Running Command: " << current->name << std::endl;
        std::cout << "\t" << current->keys << std::endl;
        std::cout << "\t" << current->cmd << std::endl;
        system(current->cmd.c_str());
    }
}

void HandleUp(e_key_t vkCode)
{
    shortcut_keys.erase(vkCode);
}

int main()
{
    LoadShortcuts(shortcuts);
    Create(HandleDwn, HandleUp);
    char c;
    do
    {
        std::cout << "Shortcuts:" << std::endl;

        for (const auto &s : shortcuts)
        {
            if (s)
            {
                std::cout << "\t" << s->name << ":" << std::endl;
                std::cout << "\t\t" << "Keys: " << s->keys << std::endl;
                std::cout << "\t\t" << "Command: " << s->cmd << std::endl;
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, name);

            char cont = '0';

            while (cont != 'y')
            {
                pressed_keys.clear();
                std::cout << "Press keys for shortcut: " << std::endl;
                flag = true;
                while (flag)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));

                if (pressed_keys.empty())
                {
                    std::cout << "EMPTY" << std::endl;
                    cont = 'n';
                    continue;
                }

                std::cout << "Are these the keys you want for your shortcut(y/n)?: ";

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> cont;

                cont = std::tolower(cont);
            }

            std::string keys_str = VksToString(pressed_keys);
            hash = HashVK(pressed_keys);
            pressed_keys.clear();

            std::cout << "Enter Shortcut Command: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, cmd);

            if (!SaveShortcut(shortcuts, name, hash, cmd, keys_str))
            {
                std::cerr << "[-]Failed to save shortcut[-]" << std::endl;
            }
            break;
        }
        case 'd':
        {
            std::string name;
            std::cout << "Enter shortcut name to be deleted: ";
            std::cin >> name;
            if(DeleteShortcut(shortcuts, name))
                std::cout << "Delete: " << name;
            else
                std::cerr <<  "Failed to Delete: " << name;
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