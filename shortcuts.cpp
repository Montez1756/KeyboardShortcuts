#include "shortcuts.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

Shortcut *CreateShortcut(const std::string name, const size_t hash, const std::string cmd, const std::string keys)
{
    Shortcut *s = new Shortcut;
    s->name = name;
    s->cmd = cmd;
    s->hash = hash;
    s->keys = keys;
    s->next = nullptr;
    return s;
}

bool SaveShortcut(std::vector<Shortcut *> &shortcuts, const std::string &name, size_t hash, const std::string &cmd, const std::string &keys)
{
    for (const auto &s : shortcuts)
    {
        if (!s)
            continue;
        if (s->name == name || s->keys == keys)
            return false;
    }

    std::ofstream out(S_FILE, std::ios::app | std::ios::binary);
    if (!out)
        return false;

    Shortcut *s = CreateShortcut(name, hash, cmd, keys);
    AddShortcut(shortcuts, s, hash);

    // Manual serialization (safe for strings)
    size_t name_len = name.size();
    size_t cmd_len = cmd.size();
    size_t keys_len = keys.size();

    out.write(reinterpret_cast<const char *>(&hash), sizeof(hash));
    out.write(reinterpret_cast<const char *>(&name_len), sizeof(name_len));
    out.write(name.c_str(), name_len);
    out.write(reinterpret_cast<const char *>(&cmd_len), sizeof(cmd_len));
    out.write(cmd.c_str(), cmd_len);
    out.write(reinterpret_cast<const char *>(&keys_len), sizeof(keys_len));
    out.write(keys.c_str(), keys_len);

    out.close();
    return true;
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
    std::ifstream in(S_FILE, std::ios::binary);
    if (!in) return false;

    while (true)
    {
        size_t hash, name_len, cmd_len, keys_len;
        if (!in.read(reinterpret_cast<char*>(&hash), sizeof(hash))) break;

        in.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
        std::string name(name_len, '\0');
        in.read(&name[0], name_len);

        in.read(reinterpret_cast<char*>(&cmd_len), sizeof(cmd_len));
        std::string cmd(cmd_len, '\0');
        in.read(&cmd[0], cmd_len);

        in.read(reinterpret_cast<char*>(&keys_len), sizeof(keys_len));
        std::string keys(keys_len, '\0');
        in.read(&keys[0], keys_len);

        Shortcut *s = CreateShortcut(name, hash, cmd, keys);
        AddShortcut(shortcuts, s, hash);
    }

    in.close();
    return true;
}


void AddShortcut(std::vector<Shortcut *> &shortcuts, Shortcut *s, size_t hash)
{
    size_t hash_index = hash % shortcuts.size();
    Shortcut *current = shortcuts[hash_index];
    if (current)
    {
        while (current->next)
            current = current->next;
        current->next = s;
    }
    else
        shortcuts[hash_index] = s;
}

bool DeleteShortcut(std::vector<Shortcut *> &shortcuts, const std::string name)
{
    std::ifstream in(S_FILE);

    if (in.is_open())
    {
        Shortcut s_temp;
        while (in.read(reinterpret_cast<char *>(&s_temp), sizeof(Shortcut)))
        {
            if (name == s_temp.name)
            {
                return true;
            }
        }
    }
    return false;
}