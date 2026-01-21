// automatically generated from continueAbort.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void abortSelected();
static void continueAbortMenuInit();
static void continueSelected();

#pragma pack(push, 1)
struct SwosMenu_continueAbortMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, continueAbortMenuInit, 0, 0, 0, 3, { true, false, false, false, }};

    Entry eb00{ 152, 0, 100, 15 };
         EntryText et00{ 0, nullptr };
         EntryColor ec00{ 7 | (16) };
    EntryEnd ee00{};

    Entry eb01{ 52, 50, 200, 40 };
         EntryMultilineText emt01{ 0, nullptr };
         EntryColor ec01{ 4 };
    EntryEnd ee01{};

    Entry eb02{ 102, 165, 100, 15 };
         EntryText et02{ 0, nullptr };
         EntryColor ec02{ 14 };
         EntryNextPositions ep02{ 255, 255, 255, 3 };
         EntryOnSelectFunctionNative eosf02{ continueSelected };
    EntryEnd ee02{};

    Entry eb03{ 102, 185, 100, 15 };
         EntryText et03{ 0, nullptr };
         EntryColor ec03{ 12 };
         EntryNextPositions ep03{ 255, 255, 2, 255 };
         EntryOnSelectFunctionNative eosf03{ abortSelected };
    EntryEnd ee03{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const continueAbortMenu
#endif
;
namespace ContinueAbortMenu {
    enum Entries {
        header = 0,
        warningText = 1,
        cont = 2,
        abort = 3,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& warningTextEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& contEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& abortEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void abortSelected() {}
static void continueAbortMenuInit() {}
static void continueSelected() {}
#endif
