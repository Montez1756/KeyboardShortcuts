#ifndef SHORTCUTS_H
#define SHORTCUTS_H


#include <set>
#include "keyboard.h"
#include <string>
#include <vector>
typedef struct Shortcut{
    std::string name;
    std::string cmd;
    std::string keys;
    Shortcut *next;
}Shortcut;

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#endif
// Save Shortcut to keys.txt shortcut file
bool SaveShortcut(std::vector<Shortcut *> &shortcuts, const std::string name, const size_t hash, const std::string cmd, const std::string keys);
// Load shortcuts from keys.txt shortcut file
bool LoadShortcuts(std::vector<Shortcut *> &shortcuts);
// Add shortcut to shortcuts vector hash table
void AddShortcut(std::vector<Shortcut *> &shortcuts, Shortcut *s, size_t hash);
Shortcut *CreateShortcut(const std::string  name, const std::string cmd, const std::string keys);
#endif