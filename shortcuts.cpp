#include "shortcuts.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
bool SaveShortcut(const std::string name, const size_t hash, const std::string cmd, const std::string keys)
{
    std::ofstream out("keys.txt", std::ios::app);

    if(out.is_open())
    {
        out << name << "|" << hash << "|" << cmd << "|" << keys << std::endl;
    }
    return false;
}

std::vector<std::string> split(std::string str, char delim)
{
    std::stringstream s(str);
    std::vector<std::string> temp;
    std::string part;
    while(std::getline(s, part, delim))
    {
        temp.push_back(part);
    }
    return temp;
}

bool LoadShortcuts(std::vector<Shortcut *> &shortcuts)
{
    std::ifstream in("keys.txt");

    if(in.is_open())
    {
        std::string line;
    
        while(std::getline(in, line))
        {
            std::vector<std::string> splits = split(line, '|');
            Shortcut *s = new Shortcut;
            s->name = splits[0];
            size_t hash = std::stoull(splits[1]);
            s->cmd = splits[2];
            s->keys = splits[3];
            if(shortcuts.size() == 50){
                std::cout << hash << std::endl;
                std::cout << hash % 50 << std::endl;
                shortcuts[hash % 50] = s;
            }
        }
        return true; 
    }
    return false;

}