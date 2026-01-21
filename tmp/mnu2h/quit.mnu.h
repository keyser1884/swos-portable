// automatically generated from quit.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void quitGame();
static void returnToGame();

#pragma pack(push, 1)
struct SwosMenu_quitMenu : public BaseMenu
{
    MenuHeader header{ 0, 0, 0, 1 };

    Entry eb00{ 85, 66, 150, 20 };
         EntryTextNative et00{ 16, kQuitToOS };
         EntryColor ec00{ 14 };
         EntryNextPositions ep00{ 255, 255, 255, 1 };
         EntryOnSelectFunctionNative eosf00{ quitGame };
    EntryEnd ee00{};

    Entry eb01{ 85, 116, 150, 20 };
         EntryText et01{ 16, SwosVM::Offsets::aReturnToGame };
         EntryColor ec01{ 4 };
         EntryNextPositions ep01{ 255, 255, 0, 255 };
         EntryOnSelectFunctionNative eosf01{ returnToGame };
    EntryEnd ee01{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const quitMenu
#endif
;
namespace QuitMenu {
    enum Entries {
        quit = 0,
        returnToGame = 1,
    };

    // entry references
    static auto& quitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& returnToGameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void quitGame() {}
static void returnToGame() {}
#endif
