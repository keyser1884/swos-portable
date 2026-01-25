// automatically generated from about.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

void SetExitMenuFlag();

#pragma pack(push, 1)
struct SwosMenu_aboutMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, 0, 0, 0, 0, 8, { true, false, false, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 10, 125, 15 };
         EntryTextNative et00{ 0, "ABOUT" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ 30, 45, 260, 12 };
         EntryTextNative et01{ 0, "SWOS C++ PORT" };
         EntryColor ec01{ 14 };
    EntryEnd ee01{};

    Entry eb02{ 30, 65, 260, 10 };
         EntryTextNative et02{ 0, "BY RICH VICKERS" };
         EntryColor ec02{ 11 };
    EntryEnd ee02{};

    Entry eb03{ 30, 80, 260, 10 };
         EntryTextNative et03{ 0, "GITHUB.COM/KEYSER1884/SWOS-PORTABLE" };
         EntryColor ec03{ 7 };
    EntryEnd ee03{};

    Entry eb04{ 30, 105, 260, 10 };
         EntryTextNative et04{ 0, "BASED ON THE ORIGINAL PORT BY ZLATKOK" };
         EntryColor ec04{ 11 };
    EntryEnd ee04{};

    Entry eb05{ 30, 120, 260, 10 };
         EntryTextNative et05{ 0, "GITHUB.COM/ZLATKOK/SWOS-PORT" };
         EntryColor ec05{ 7 };
    EntryEnd ee05{};

    Entry eb06{ 30, 145, 260, 10 };
         EntryTextNative et06{ 0, "SENSIBLE WORLD OF SOCCER" };
         EntryColor ec06{ 11 };
    EntryEnd ee06{};

    Entry eb07{ 30, 160, 260, 10 };
         EntryTextNative et07{ 0, "ORIGINAL GAME BY SENSIBLE SOFTWARE" };
         EntryColor ec07{ 7 };
    EntryEnd ee07{};

    Entry eb08{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryTextNative et08{ 0, "EXIT" };
         EntryColor ec08{ 4 };
         EntryNextPositions ep08{ 255, 255, 255, 255 };
         EntryOnSelectFunctionNative eosf08{ SetExitMenuFlag };
    EntryEnd ee08{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const aboutMenu
#endif
;
namespace AboutMenu {
    enum Entries {
        header = 0,
        credit1 = 1,
        credit2 = 2,
        credit3 = 3,
        credit4 = 4,
        credit5 = 5,
        credit6 = 6,
        credit7 = 7,
        exit = 8,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& credit1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& credit2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& credit3Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& credit4Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& credit5Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& credit6Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& credit7Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
}
#pragma pack(pop)
