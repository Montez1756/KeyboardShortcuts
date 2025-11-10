#ifndef SHORTCUTS_H
#define SHORTCUTS_H
#include <set>
#include "keyboard.h"
#include <string>
#include <vector>

#define S_FILE "keys.bin"

typedef struct Shortcut{
    std::string name;
    std::string cmd;
    size_t hash;
    std::string keys;
    Shortcut *next;
}Shortcut;

// Save Shortcut to keys.txt shortcut file
bool SaveShortcut(std::vector<Shortcut *> &shortcuts, const std::string &name, size_t hash, const std::string &cmd, const std::string &keys);
// Load shortcuts from keys.txt shortcut file
bool LoadShortcuts(std::vector<Shortcut *> &shortcuts);
// Add shortcut to shortcuts vector hash table
void AddShortcut(std::vector<Shortcut *> &shortcuts, Shortcut *s, size_t hash);
Shortcut *CreateShortcut(const std::string  name, const size_t hash, const std::string cmd, const std::string keys);
bool DeleteShortcut(std::vector<Shortcut *> &shortcuts, const std::string name);
#endif