// automatically generated from continue.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void continueOnInit();

#pragma pack(push, 1)
struct SwosMenu_continueMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, continueOnInit, 0, 0, 0, 1, { true, false, false, false, }};

    Entry eb00{ 2, 40, 300, 40 };
         EntryText et00{ 16, -1 };
         EntryColor ec00{ 12 };
    EntryEnd ee00{};

    Entry eb01{ 102, 185, 100, 15 };
         EntryTextNative et01{ 16, "CONTINUE" };
         EntryColor ec01{ 12 };
         EntryOnSelectFunctionNative eosf01{ SetExitMenuFlag };
    EntryEnd ee01{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const continueMenu
#endif
;
namespace ContinueMenu {
    enum Entries {
        errorText = 0,
        continueButton = 1,
    };

    // entry references
    static auto& errorTextEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& continueButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void continueOnInit() {}
#endif
