// automatically generated from replayExit.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void replayExitMenuOnInit();
static void exitOnSelect();
static void replayOnSelect();

#pragma pack(push, 1)
struct SwosMenu_replayExitMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, replayExitMenuOnInit, 0, 0, 0, 0, { true, false, false, false, }};

    Entry eb00{ 85, 66, 150, 20 };
         EntryTextNative et00{ 16, "PLAY AGAIN" };
         EntryColor ec00{ 14 };
         EntryNextPositions ep00{ 255, 255, 255, 1 };
         EntryOnSelectFunctionNative eosf00{ replayOnSelect };
    EntryEnd ee00{};

    Entry eb01{ 85, 116, 150, 20 };
         EntryText et01{ 16, SwosVM::Offsets::aExit };
         EntryColor ec01{ 4 };
         EntryNextPositions ep01{ 255, 255, 0, 255 };
         EntryOnSelectFunctionNative eosf01{ exitOnSelect };
    EntryEnd ee01{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const replayExitMenu
#endif
;
namespace ReplayExitMenu {
    enum Entries {
        replay = 0,
        exit = 1,
    };

    // entry references
    static auto& replayEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void replayExitMenuOnInit() {}
static void exitOnSelect() {}
static void replayOnSelect() {}
#endif
