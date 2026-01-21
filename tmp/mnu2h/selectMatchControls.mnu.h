// automatically generated from selectMatchControls.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void selectMatchControlsMenuOnInit();
static void configurePl1Controls();
static void selectControl();
static void scrollUpSelected();
static void cancelSelection();
static void selectMatchControlsMenuOnRestore();
static void confirmSelection();
static void configurePl2Controls();
static void selectMatchControlsMenuOnDraw();
static void player1ControlsSelected();
static void scrollDownSelected();
static void player2ControlsSelected();

#pragma pack(push, 1)
struct SwosMenu_selectMatchControlsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, selectMatchControlsMenuOnInit, selectMatchControlsMenuOnRestore, selectMatchControlsMenuOnDraw, 0, 19, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, (125) +6, 15 };
         EntryTextNative et00{ 0, "SELECT MATCH CONTROLS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ 5 -8, 20, 100, 12 };
         EntryText et01{ 13, -1 };
    EntryEnd ee01{};

    Entry eb02{ ((5 -8)), ((((20 + 12)))) +(4), 100, 12 };
         EntryText et02{ 0, -1 };
         EntryColor ec02{ 14 };
         EntryNextPositions ep02{ 255, 8, 255, 21 };
         EntryOnSelectFunctionWithMaskNative eosfm02{ player1ControlsSelected, 2 | 4 | 1 | (32) };
    EntryEnd ee02{};

    Entry eb03{ ((((5 -8)))), ((((((((20 + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryTextNative et03{ (8192) | (15), "INCOMPLETE CONFIG" };
    EntryEnd ee03{};

    Entry eb04{ (((5 -8) +5 +(100)) +5 +(100)), 20, 100, 12 };
         EntryText et04{ 13, -1 };
    EntryEnd ee04{};

    Entry eb05{ (((((5 -8) +5 +(100)) +5 +(100)))), ((((20 + 12)))) +(4), 100, 12 };
         EntryText et05{ 0, -1 };
         EntryColor ec05{ 14 };
         EntryNextPositions ep05{ 8, 255, 255, 22 };
         EntryOnSelectFunctionWithMaskNative eosfm05{ player2ControlsSelected, 2 | 4 | 1 | (32) };
    EntryEnd ee05{};

    Entry eb06{ (((((((5 -8) +5 +(100)) +5 +(100)))))), ((((((((20 + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryTextNative et06{ (8192) | (15), "INCOMPLETE CONFIG" };
    EntryEnd ee06{};

    Entry eb07{ ((5 -8) +5 +(100)), 20, 100, 12 };
         EntryTextNative et07{ 15, "CONTROLS" };
    EntryEnd ee07{};

    TemplateEntry te00{};

    Entry eb30008{ 0, 0, 1, 1 };
         EntryText et30008{ 0, -1 };
         EntryColor ec30008{ 13 };
         EntryOnSelectFunctionWithMaskNative eosfm30008{ selectControl, 2 | 4 | 1 };
    EntryEnd ee30008{};

    Entry eb08{ ((((5 -8) +5 +(100)))), ((((20 + 12)))) +(4), 100, 12 };
         EntryNextPositions ep08{ 2, 5, 255, 9 };
    EntryEnd ee08{};

    Entry eb09{ ((((((5 -8) +5 +(100)))))), ((((((((20 + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep09{ 2, 5, 8, 10 };
    EntryEnd ee09{};

    Entry eb10{ ((((((((5 -8) +5 +(100)))))))), ((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep10{ 2, 5, 9, 11 };
    EntryEnd ee10{};

    Entry eb11{ ((((((((((5 -8) +5 +(100)))))))))), ((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep11{ 2, 5, 10, 12 };
    EntryEnd ee11{};

    Entry eb12{ ((((((((((((5 -8) +5 +(100)))))))))))), ((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep12{ 2, 5, 11, 13 };
    EntryEnd ee12{};

    Entry eb13{ ((((((((((((((5 -8) +5 +(100)))))))))))))), ((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep13{ 2, 5, 12, 14 };
    EntryEnd ee13{};

    Entry eb14{ ((((((((((((((((5 -8) +5 +(100)))))))))))))))), ((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep14{ 2, 17, 13, 15 };
    EntryEnd ee14{};

    Entry eb15{ ((((((((((((((((((5 -8) +5 +(100)))))))))))))))))), ((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 100, 12 };
         EntryNextPositions ep15{ 2, 18, 14, 19 };
    EntryEnd ee15{};

    ResetTemplateEntry rte00{};

    Entry eb16{ -8, ((((((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4), 320, 12 };
         EntryText et16{ 0, -1 };
    EntryEnd ee16{};

    Entry eb17{ ((((((((((((((((((5 -8) +5 +(100)))))))))))))))) + 100)) +1, (((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4)), 8, 12 };
         EntryForegroundSprite efs17{ 183 };
         EntryNextPositions ep17{ 14, 255, 5, 18 };
         EntryOnSelectFunctionNative eosf17{ scrollUpSelected };
    EntryEnd ee17{};

    Entry eb18{ ((((((((((((((((((((5 -8) +5 +(100)))))))))))))))))) + 100)) +1, (((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4)), 8, 12 };
         EntryForegroundSprite efs18{ 184 };
         EntryNextPositions ep18{ 15, 255, 17, 20 };
         EntryOnSelectFunctionNative eosf18{ scrollDownSelected };
    EntryEnd ee18{};

    TemplateEntry te01{};

    Entry eb30021{ 0, 0, 1, 1 };
         EntryColor ec30021{ 4 };
    EntryEnd ee30021{};

    Entry eb19{ ((5 -8) +5 +(100)), ((((((((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)) +4, (((100) -2) / 2), 12 };
         EntryText et19{ 0, -1 };
         EntryColor ec19{ 10 };
         EntryNextPositions ep19{ 21, 20, 15, 255 };
         EntryDownSkip eds19{ 20, 3 };
         EntryOnSelectFunctionNative eosf19{ confirmSelection };
    EntryEnd ee19{};

    Entry eb20{ (((((5 -8) +5 +(100)) + (((100) -2) / 2)))) +2, ((((((((((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)) +4)), (((100) -2) / 2), 12 };
         EntryTextNative et20{ 0, "BACK" };
         EntryNextPositions ep20{ 19, 22, 15, 255 };
         EntryOnSelectFunctionNative eosf20{ cancelSelection };
    EntryEnd ee20{};

    Entry eb21{ 5 -8, ((((((((((((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)) +4)))), 100, 12 };
         EntryTextNative et21{ 0, "CONFIGURE" };
         EntryNextPositions ep21{ 255, 19, 2, 255 };
         EntryOnSelectFunctionNative eosf21{ configurePl1Controls };
    EntryEnd ee21{};

    Entry eb22{ (((5 -8) +5 +(100)) +5 +(100)), ((((((((((((((((((((((((((((((((((((((((((((20 + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)))) +(4) + 12)) +4)))))), 100, 12 };
         EntryTextNative et22{ 0, "CONFIGURE" };
         EntryNextPositions ep22{ 20, 255, 5, 255 };
         EntryDownSkip eds22{ 17, 2 };
         EntryOnSelectFunctionNative eosf22{ configurePl2Controls };
    EntryEnd ee22{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const selectMatchControlsMenu
#endif
;
namespace SelectMatchControlsMenu {
    enum Entries {
        header = 0,
        team1 = 1,
        player1Controls = 2,
        player1Warning = 3,
        team2 = 4,
        player2Controls = 5,
        player2Warning = 6,
        controlsLabel = 7,
        control00 = 8,
        control01 = 9,
        control02 = 10,
        control03 = 11,
        control04 = 12,
        control05 = 13,
        control06 = 14,
        control07 = 15,
        legend = 16,
        scrollUp = 17,
        scrollDown = 18,
        play = 19,
        back = 20,
        configure1 = 21,
        configure2 = 22,
        firstControl = 8,
        secondLastControl = 14,
        lastControl = 15,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& team1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& player1ControlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& player1WarningEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& team2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& player2ControlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& player2WarningEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& controlsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& control00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& control01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& control02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& control03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& control04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& control05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& control06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& control07Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& legendEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& scrollUpEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& scrollDownEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& playEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& backEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& configure1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& configure2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& firstControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& secondLastControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& lastControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));

    constexpr int kNumControlEntries = 8;
    constexpr int kDisabledColor = 7;
    constexpr int kEnabledColor = 4;
    constexpr int kGapWidth = 5;
    constexpr int kBottomButtonsColumnGap = 2;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void selectMatchControlsMenuOnInit() {}
static void configurePl1Controls() {}
static void selectControl() {}
static void scrollUpSelected() {}
static void cancelSelection() {}
static void selectMatchControlsMenuOnRestore() {}
static void confirmSelection() {}
static void configurePl2Controls() {}
static void selectMatchControlsMenuOnDraw() {}
static void player1ControlsSelected() {}
static void scrollDownSelected() {}
static void player2ControlsSelected() {}
#endif
