#include "shortcuts.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

Shortcut *CreateShortcut(const std::string  name, const std::string cmd, const std::string keys){
    Shortcut *s = new Shortcut;
    s->name = name;
    s->cmd = cmd;
    s->keys = keys;
    s->next = nullptr;
    return s;
}

bool SaveShortcut(std::vector<Shortcut *> &shortcuts, const std::string name, const size_t hash, const std::string cmd, const std::string keys)
{
    for(const auto &s: shortcuts){
        if(!s)
            continue;
        if(s->name == name || s->keys == keys)
            return false;
    }
    std::ofstream out("keys.txt", std::ios::app);

    if (out.is_open())
    {
        out << name << "|" << hash << "|" << cmd << "|" << keys << std::endl;
        Shortcut *s = CreateShortcut(name, keys, cmd);
    
        AddShortcut(shortcuts, s, hash);
        out.close();
        return true;
    }

    return false;
}

std::vector<std::string> split(std::string str, char delim)
{
    std::stringstream s(str);
    std::vector<std::string> temp;
    std::string part;
    while (std::getline(s, part, delim))
    {
        temp.push_back(part);
    }
    return temp;
}

bool LoadShortcuts(std::vector<Shortcut *> &shortcuts)
{
    std::ifstream in("keys.txt");

    if (in.is_open())
    {
        std::string line;

        while (std::getline(in, line))
        {
            std::vector<std::string> splits = split(line, '|');
            Shortcut *s = CreateShortcut(splits[0], splits[2], splits[3]);
            size_t hash = std::stoull(splits[1]);
            AddShortcut(shortcuts, s, hash);
        }
        in.close();
        return true;
    }
    return false;
}

void AddShortcut(std::vector<Shortcut *> &shortcuts, Shortcut *s, size_t hash)
{
    size_t hash_index = hash % shortcuts.size();
    Shortcut *current = shortcuts[hash_index];
    if (current){
        while(current->next)
            current = current->next;
        current->next = s;
    }
    else
        shortcuts[hash_index] = s;
}