#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <set>
#include <windows.h>
#include "keyboard.h"
#include <string>
#include <vector>

typedef struct Shortcut{
    std::string name;
    std::string cmd;
    std::string keys;
}Shortcut;

bool SaveShortcut(const std::string name, const size_t hash, const std::string cmd, const std::string keys);
bool LoadShortcuts(std::vector<Shortcut *> &shortcuts);
#endif