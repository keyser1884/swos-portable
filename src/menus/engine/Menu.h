#pragma once

#include "MenuEntry.h"

#pragma pack(push, 1)
struct Menu
{
    SwosProcPointer onInit;
    SwosProcPointer onReturn;
    SwosProcPointer onDraw;
    SwosDataPointer<MenuEntry> selectedEntry;
    word numEntries;
    SwosDataPointer<char> endOfMenuPtr;

    MenuEntry *entries() const {
        return (MenuEntry *)(this + 1);
    }
    MenuEntry *sentinelEntry() const {
        return entries() + numEntries;
    }
};
#pragma pack(pop)
