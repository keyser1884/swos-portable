// automatically generated from replays.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void replaysMenuOnInit();
static void updateMenuButtonsState();
static void selectHighlightToSave();
static void selectReplayToLoad();
static void selectReplayToSave();
static void selectHighlightToLoad();
static void playHighlights();
static void playReplay();

#pragma pack(push, 1)
struct SwosMenu_replaysMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, replaysMenuOnInit, updateMenuButtonsState, 0, 0, 1, { true, true, false, false, }};

    MenuXY menuXY00{-8, 0};

    Entry eb00{ 92, 0, 120, 15 };
         EntryTextNative et00{ 0, "REPLAYS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ (((320) -2 * (140)) / 3), 60, 140, 15 };
         EntryText et01{ 0, SwosVM::Offsets::aViewHighlights };
         EntryColor ec01{ 14 };
         EntryNextPositions ep01{ 255, 4, 255, 2 };
         EntryLeftSkip els01{ 2, 3 };
         EntryOnSelectFunctionNative eosf01{ playHighlights };
    EntryEnd ee01{};

    Entry eb02{ (((320) -2 * (140)) / 3), (((60))) +(30), 140, 15 };
         EntryText et02{ 0, SwosVM::Offsets::aLoadHighlights };
         EntryColor ec02{ 14 };
         EntryNextPositions ep02{ 255, 5, 1, 3 };
         EntryOnSelectFunctionNative eosf02{ selectHighlightToLoad };
    EntryEnd ee02{};

    Entry eb03{ (((320) -2 * (140)) / 3), ((((((60))) +(30)))) +(30), 140, 15 };
         EntryText et03{ 0, SwosVM::Offsets::aSaveHighlights };
         EntryColor ec03{ 14 };
         EntryNextPositions ep03{ 255, 6, 2, 7 };
         EntryLeftSkip els03{ 2, 2 };
         EntryOnSelectFunctionNative eosf03{ selectHighlightToSave };
    EntryEnd ee03{};

    Entry eb04{ ((((320) -2 * (140)) / 3) +(140) +(((320) -2 * (140)) / 3)), 60, 140, 15 };
         EntryTextNative et04{ 0, "VIEW REPLAY" };
         EntryColor ec04{ 14 };
         EntryNextPositions ep04{ 1, 255, 255, 5 };
         EntryRightSkip ers04{ 5, 3 };
         EntryOnSelectFunctionNative eosf04{ playReplay };
    EntryEnd ee04{};

    Entry eb05{ ((((320) -2 * (140)) / 3) +(140) +(((320) -2 * (140)) / 3)), (((60))) +(30), 140, 15 };
         EntryTextNative et05{ 0, "LOAD REPLAY" };
         EntryColor ec05{ 14 };
         EntryNextPositions ep05{ 2, 255, 4, 6 };
         EntryOnSelectFunctionNative eosf05{ selectReplayToLoad };
    EntryEnd ee05{};

    Entry eb06{ ((((320) -2 * (140)) / 3) +(140) +(((320) -2 * (140)) / 3)), ((((((60))) +(30)))) +(30), 140, 15 };
         EntryTextNative et06{ 0, "SAVE REPLAY" };
         EntryColor ec06{ 14 };
         EntryNextPositions ep06{ 3, 255, 5, 7 };
         EntryRightSkip ers06{ 5, 2 };
         EntryOnSelectFunctionNative eosf06{ selectReplayToSave };
    EntryEnd ee06{};

    Entry eb07{ ((320) -(140)) / 2, ((200) -(15)), 140, 15 };
         EntryText et07{ 0, SwosVM::Offsets::aExit };
         EntryColor ec07{ 4 };
         EntryNextPositions ep07{ 255, 255, 3, 255 };
         EntryOnSelectFunctionNative eosf07{ SetExitMenuFlag };
    EntryEnd ee07{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const replaysMenu
#endif
;
namespace ReplaysMenu {
    enum Entries {
        title = 0,
        viewHighlights = 1,
        loadHighlights = 2,
        saveHighlights = 3,
        viewReplays = 4,
        loadReplays = 5,
        saveReplays = 6,
        exit = 7,
    };

    // entry references
    static auto& titleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& viewHighlightsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& loadHighlightsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& saveHighlightsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& viewReplaysEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& loadReplaysEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& saveReplaysEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void replaysMenuOnInit() {}
static void updateMenuButtonsState() {}
static void selectHighlightToSave() {}
static void selectReplayToLoad() {}
static void selectReplayToSave() {}
static void selectHighlightToLoad() {}
static void playHighlights() {}
static void playReplay() {}
#endif
