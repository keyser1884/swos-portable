// automatically generated from setupKeyboard.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void onScrollUp();
static void onScrollDown();
static void deleteKey();
static void setupKeyboardMenuOnRestore();
static void updateAction();
static void redefineKeys();
static void addKeyMapping();
static void setupKeyboardMenuOnInit();
static void testKeyboard();
static void setupKeyboardMenuOnDraw();

#pragma pack(push, 1)
struct SwosMenu_setupKeyboardMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, setupKeyboardMenuOnInit, setupKeyboardMenuOnRestore, setupKeyboardMenuOnDraw, 0, 26, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryText et00{ 0, -1 };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ (((320) -((150) +8 +(110))) / 2 -8), 18, 150, 12 };
         EntryTextNative et01{ 0, "KEYS:" };
    EntryEnd ee01{};

    TemplateEntry te00{};

    Entry eb30002{ 0, 0, 1, 1 };
         EntryText et30002{ 0, -1 };
         EntryOnSelectFunctionNative eosf30002{ deleteKey };
    EntryEnd ee30002{};

    Entry eb02{ (((320) -((150) +8 +(110))) / 2 -8), ((((18 + 12)))) +(2), 150, 12 };
         EntryColor ec02{ 13 };
         EntryNextPositions ep02{ 19, 11, 19, 3 };
    EntryEnd ee02{};

    Entry eb03{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((18 + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec03{ 13 };
         EntryNextPositions ep03{ 19, 12, 2, 4 };
    EntryEnd ee03{};

    Entry eb04{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec04{ 13 };
         EntryNextPositions ep04{ 19, 13, 3, 5 };
    EntryEnd ee04{};

    Entry eb05{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec05{ 13 };
         EntryNextPositions ep05{ 20, 14, 4, 6 };
    EntryEnd ee05{};

    Entry eb06{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec06{ 13 };
         EntryNextPositions ep06{ 20, 15, 5, 7 };
    EntryEnd ee06{};

    Entry eb07{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec07{ 13 };
         EntryNextPositions ep07{ 20, 16, 6, 8 };
    EntryEnd ee07{};

    Entry eb08{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryColor ec08{ 13 };
         EntryNextPositions ep08{ 20, 17, 7, 23 };
    EntryEnd ee08{};

    ResetTemplateEntry rte00{};

    Entry eb09{ (((320) -((150) +8 +(110))) / 2 -8), ((((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 150, 12 };
         EntryTextNative et09{ 0, "(SELECT TO DELETE)" };
    EntryEnd ee09{};

    Entry eb10{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), 18, 110, 12 };
         EntryTextNative et10{ 0, "ACTIONS:" };
    EntryEnd ee10{};

    TemplateEntry te01{};

    Entry eb30013{ 0, 0, 1, 1 };
         EntryText et30013{ 0, -1 };
         EntryOnSelectFunctionNative eosf30013{ updateAction };
    EntryEnd ee30013{};

    Entry eb11{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((18 + 12)))) +(2), 110, 12 };
         EntryColor ec11{ 14 };
         EntryNextPositions ep11{ 2, 21, 21, 12 };
    EntryEnd ee11{};

    Entry eb12{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((18 + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec12{ 14 };
         EntryNextPositions ep12{ 3, 21, 11, 13 };
    EntryEnd ee12{};

    Entry eb13{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec13{ 14 };
         EntryNextPositions ep13{ 4, 21, 12, 14 };
    EntryEnd ee13{};

    Entry eb14{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec14{ 14 };
         EntryNextPositions ep14{ 5, 22, 13, 15 };
    EntryEnd ee14{};

    Entry eb15{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec15{ 14 };
         EntryNextPositions ep15{ 6, 22, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec16{ 14 };
         EntryNextPositions ep16{ 7, 22, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryColor ec17{ 14 };
         EntryNextPositions ep17{ 8, 22, 16, 23 };
    EntryEnd ee17{};

    ResetTemplateEntry rte01{};

    Entry eb18{ ((((320) -((150) +8 +(110))) / 2 -8) +(150) +8), ((((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2), 110, 12 };
         EntryTextNative et18{ 0, "(SELECT TO CHANGE)" };
    EntryEnd ee18{};

    Entry eb19{ ((((320) -((150) +8 +(110))) / 2 -8)) -8 -2, (((((18 + 12)))) +(2)), 8, 14 };
         EntryForegroundSprite efs19{ 183 };
         EntryNextPositions ep19{ 255, 2, 255, 20 };
         EntryOnSelectFunctionNative eosf19{ onScrollUp };
    EntryEnd ee19{};

    Entry eb20{ ((((((320) -((150) +8 +(110))) / 2 -8)) -8 -2)), ((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)) -(14), 8, 14 };
         EntryForegroundSprite efs20{ 184 };
         EntryNextPositions ep20{ 255, 8, 19, 255 };
         EntryOnSelectFunctionNative eosf20{ onScrollDown };
    EntryEnd ee20{};

    Entry eb21{ ((((((320) -((150) +8 +(110))) / 2 -8) +(150) +8) + 110)) +2, (((((18 + 12)))) +(2)), 8, 14 };
         EntryForegroundSprite efs21{ 183 };
         EntryNextPositions ep21{ 11, 255, 255, 22 };
         EntryOnSelectFunctionNative eosf21{ onScrollUp };
    EntryEnd ee21{};

    Entry eb22{ ((((((((320) -((150) +8 +(110))) / 2 -8) +(150) +8) + 110)) +2)), ((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)) -(14), 8, 14 };
         EntryForegroundSprite efs22{ 184 };
         EntryNextPositions ep22{ 17, 255, 21, 255 };
         EntryOnSelectFunctionNative eosf22{ onScrollDown };
    EntryEnd ee22{};

    Entry eb23{ (((320) -(125)) / 2 -8), ((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)) +(12) +2 +4, 125, (15) -3 };
         EntryTextNative et23{ 0, "ADD MAPPING" };
         EntryColor ec23{ 3 };
         EntryNextPositions ep23{ 255, 255, 8, 24 };
         EntryOnSelectFunctionNative eosf23{ addKeyMapping };
    EntryEnd ee23{};

    Entry eb24{ (((320) -(125)) / 2 -8), ((((((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)) +(12) +2 +4 + (15) -3)))) +(2), 125, (15) -3 };
         EntryTextNative et24{ 0, "QUICK CONFIG" };
         EntryColor ec24{ 3 };
         EntryNextPositions ep24{ 255, 255, 23, 25 };
         EntryOnSelectFunctionNative eosf24{ redefineKeys };
    EntryEnd ee24{};

    Entry eb25{ (((320) -(125)) / 2 -8), ((((((((((((((((((((((((((((((((((((((18 + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)))) +(2) + 12)) +(12) +2 +4 + (15) -3)))) +(2) + (15) -3)))) +(2), 125, (15) -3 };
         EntryTextNative et25{ 0, "TEST" };
         EntryColor ec25{ 3 };
         EntryNextPositions ep25{ 255, 255, 24, 26 };
         EntryOnSelectFunctionNative eosf25{ testKeyboard };
    EntryEnd ee25{};

    Entry eb26{ (((320) -(125)) / 2 -8), (200) -(((15) -3)), 125, (15) -3 };
         EntryText et26{ 0, SwosVM::Offsets::aExit };
         EntryColor ec26{ 4 };
         EntryNextPositions ep26{ 255, 255, 25, 255 };
         EntryOnSelectFunctionNative eosf26{ SetExitMenuFlag };
    EntryEnd ee26{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const setupKeyboardMenu
#endif
;
namespace SetupKeyboardMenu {
    enum Entries {
        header = 0,
        key00 = 2,
        key01 = 3,
        key02 = 4,
        key03 = 5,
        key04 = 6,
        key05 = 7,
        key06 = 8,
        keyLegend = 9,
        action00 = 11,
        action01 = 12,
        action02 = 13,
        action03 = 14,
        action04 = 15,
        action05 = 16,
        action06 = 17,
        actionLegend = 18,
        leftUpArrow = 19,
        leftDownArrow = 20,
        rightUpArrow = 21,
        rightDownArrow = 22,
        addMapping = 23,
        quickConfig = 24,
        test = 25,
        exit = 26,
        firstKey = 2,
        lastKey = 8,
        firstAction = 11,
        lastAction = 17,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& key00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& key01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& key02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& key03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& key04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& key05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& key06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& keyLegendEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& action00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& action01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& action02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& action03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& action04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& action05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& action06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& actionLegendEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& leftUpArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& leftDownArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& rightUpArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& rightDownArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& addMappingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& quickConfigEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& testEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& firstKeyEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& lastKeyEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& firstActionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& lastActionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));

    constexpr int kMaxKeyEntriesShown = 7;
    constexpr int kVerticalGap = 2;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void onScrollUp() {}
static void onScrollDown() {}
static void deleteKey() {}
static void setupKeyboardMenuOnRestore() {}
static void updateAction() {}
static void redefineKeys() {}
static void addKeyMapping() {}
static void setupKeyboardMenuOnInit() {}
static void testKeyboard() {}
static void setupKeyboardMenuOnDraw() {}
#endif
