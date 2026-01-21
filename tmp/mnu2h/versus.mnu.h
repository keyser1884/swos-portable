// automatically generated from versus.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void versusMenuOnInit();

#pragma pack(push, 1)
struct SwosMenu_versusMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, versusMenuOnInit, 0, 0, 0, 5, { true, false, false, false, }};

    MenuXY menuXY00{0, -28};

    Entry eb00{ 152, 100, 0, 10 };
         EntryText et00{ (16) | 0, -1 };
    EntryEnd ee00{};

    Entry eb01{ 152, 112, 0, 10 };
         EntryText et01{ (16) | 0, -1 };
    EntryEnd ee01{};

    Entry eb02{ 0, 128, 137, 12 };
         EntryText et02{ (16) | (16384), -1 };
         EntryColor ec02{ 5 };
    EntryEnd ee02{};

    Entry eb03{ 142, 128, 20, 12 };
         EntryTextNative et03{ (16) | 0, "V" };
    EntryEnd ee03{};

    Entry eb04{ 167, 128, 137, 12 };
         EntryText et04{ (16) | (32768), -1 };
         EntryColor ec04{ 5 };
    EntryEnd ee04{};

    Entry eb05{ 0, 0, 0, 0 };
    EntryEnd ee05{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const versusMenu
#endif
;
namespace VersusMenu {
    enum Entries {
        gameName = 0,
        gameRound = 1,
        leftTeamName = 2,
        letterV = 3,
        rightTeamName = 4,
        nullEntry = 5,
    };

    // entry references
    static auto& gameNameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& gameRoundEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& leftTeamNameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& letterVEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& rightTeamNameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& nullEntryEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void versusMenuOnInit() {}
#endif
