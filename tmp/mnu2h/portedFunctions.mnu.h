// automatically generated from portedFunctions.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void doSaveAndExit();
static void toggleCompareMode();
static void updateScrollInfo();
static void doScrollDown();
static void drawToggleValue();
static void drawCompareStatus();
static void doEnableAllAsm();
static void doEnableAllCpp();
static void drawToggleLabel();
static void selectToggle();
static void doScrollUp();
static void portedFunctionsMenuOnInit();
static void drawCompareToggle();
static void portedFunctionsMenuOnReturn();

#pragma pack(push, 1)
struct SwosMenu_portedFunctionsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, portedFunctionsMenuOnInit, portedFunctionsMenuOnReturn, 0, 0, 14, { true, true, false, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8) -(10), 0, 145, 15 };
         EntryTextNative et00{ 0, "PORTED FUNCTIONS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ 240, 4, 70, 10 };
         EntryText et01{ 0, -1 };
         EntryColor ec01{ 7 };
         EntryBeforeDrawFunctionNative ebdf01{ updateScrollInfo };
    EntryEnd ee01{};

    TemplateEntry te00{};

    Entry eb30002{ 0, 0, 1, 1 };
         EntryText et30002{ 0, -1 };
         EntryColor ec30002{ 7 };
         EntryBeforeDrawFunctionNative ebdf30002{ drawToggleLabel };
    EntryEnd ee30002{};

    Entry eb02{ 10, 28, 135, 11 };
    EntryEnd ee02{};

    Entry eb03{ 10, 40, 135, 11 };
    EntryEnd ee03{};

    Entry eb04{ 10, 52, 135, 11 };
    EntryEnd ee04{};

    Entry eb05{ 10, 64, 135, 11 };
    EntryEnd ee05{};

    Entry eb06{ 10, 76, 135, 11 };
    EntryEnd ee06{};

    Entry eb07{ 10, 88, 135, 11 };
    EntryEnd ee07{};

    Entry eb08{ 10, 100, 135, 11 };
    EntryEnd ee08{};

    Entry eb09{ 10, 112, 135, 11 };
    EntryEnd ee09{};

    Entry eb10{ 10, 124, 135, 11 };
    EntryEnd ee10{};

    Entry eb11{ 10, 136, 135, 11 };
    EntryEnd ee11{};

    Entry eb12{ 10, 148, 135, 11 };
    EntryEnd ee12{};

    Entry eb13{ 10, 160, 135, 11 };
    EntryEnd ee13{};

    ResetTemplateEntry rte00{};

    TemplateEntry te01{};

    Entry eb30016{ 0, 0, 1, 1 };
         EntryText et30016{ 0, -1 };
         EntryColor ec30016{ 13 };
         EntryOnSelectFunctionNative eosf30016{ selectToggle };
         EntryBeforeDrawFunctionNative ebdf30016{ drawToggleValue };
    EntryEnd ee30016{};

    Entry eb14{ 150, 28, 30, 11 };
         EntryNextPositions ep14{ 2, 255, 26, 15 };
    EntryEnd ee14{};

    Entry eb15{ 150, 40, 30, 11 };
         EntryNextPositions ep15{ 3, 255, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ 150, 52, 30, 11 };
         EntryNextPositions ep16{ 4, 255, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ 150, 64, 30, 11 };
         EntryNextPositions ep17{ 5, 255, 16, 18 };
    EntryEnd ee17{};

    Entry eb18{ 150, 76, 30, 11 };
         EntryNextPositions ep18{ 6, 255, 17, 19 };
    EntryEnd ee18{};

    Entry eb19{ 150, 88, 30, 11 };
         EntryNextPositions ep19{ 7, 255, 18, 20 };
    EntryEnd ee19{};

    Entry eb20{ 150, 100, 30, 11 };
         EntryNextPositions ep20{ 8, 255, 19, 21 };
    EntryEnd ee20{};

    Entry eb21{ 150, 112, 30, 11 };
         EntryNextPositions ep21{ 9, 255, 20, 22 };
    EntryEnd ee21{};

    Entry eb22{ 150, 124, 30, 11 };
         EntryNextPositions ep22{ 10, 255, 21, 23 };
    EntryEnd ee22{};

    Entry eb23{ 150, 136, 30, 11 };
         EntryNextPositions ep23{ 11, 255, 22, 24 };
    EntryEnd ee23{};

    Entry eb24{ 150, 148, 30, 11 };
         EntryNextPositions ep24{ 12, 255, 23, 25 };
    EntryEnd ee24{};

    Entry eb25{ 150, 160, 30, 11 };
         EntryNextPositions ep25{ 13, 255, 24, 28 };
    EntryEnd ee25{};

    ResetTemplateEntry rte01{};

    Entry eb26{ 200, (28) +(30), 8, 14 };
         EntryForegroundSprite efs26{ 183 };
         EntryNextPositions ep26{ 16, 255, 255, 27 };
         EntryOnSelectFunctionNative eosf26{ doScrollUp };
    EntryEnd ee26{};

    Entry eb27{ 200, (28) +(80), 8, 14 };
         EntryForegroundSprite efs27{ 184 };
         EntryNextPositions ep27{ 22, 255, 26, 28 };
         EntryOnSelectFunctionNative eosf27{ doScrollDown };
    EntryEnd ee27{};

    Entry eb28{ 10, 175, 85, 12 };
         EntryTextNative et28{ 0, "ALL C++" };
         EntryColor ec28{ 14 };
         EntryNextPositions ep28{ 255, 29, 25, 31 };
         EntryOnSelectFunctionNative eosf28{ doEnableAllCpp };
    EntryEnd ee28{};

    Entry eb29{ 100, 175, 85, 12 };
         EntryTextNative et29{ 0, "ALL ASM" };
         EntryColor ec29{ 4 };
         EntryNextPositions ep29{ 28, 30, 27, 31 };
         EntryOnSelectFunctionNative eosf29{ doEnableAllAsm };
    EntryEnd ee29{};

    Entry eb30{ 200, 175, 90, 12 };
         EntryText et30{ 0, SwosVM::Offsets::aExit };
         EntryColor ec30{ 4 };
         EntryNextPositions ep30{ 29, 255, 27, 31 };
         EntryOnSelectFunctionNative eosf30{ doSaveAndExit };
    EntryEnd ee30{};

    Entry eb31{ 10, 189, 100, 12 };
         EntryText et31{ 0, -1 };
         EntryColor ec31{ 12 };
         EntryNextPositions ep31{ 255, 32, 28, 255 };
         EntryOnSelectFunctionNative eosf31{ toggleCompareMode };
         EntryBeforeDrawFunctionNative ebdf31{ drawCompareToggle };
    EntryEnd ee31{};

    Entry eb32{ 115, 189, 175, 12 };
         EntryText et32{ 0, -1 };
         EntryColor ec32{ 7 };
         EntryNextPositions ep32{ 31, 255, 30, 255 };
         EntryBeforeDrawFunctionNative ebdf32{ drawCompareStatus };
    EntryEnd ee32{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const portedFunctionsMenu
#endif
;
namespace PortedFunctionsMenu {
    enum Entries {
        header = 0,
        scrollInfo = 1,
        label_00 = 2,
        label_01 = 3,
        label_02 = 4,
        label_03 = 5,
        label_04 = 6,
        label_05 = 7,
        label_06 = 8,
        label_07 = 9,
        label_08 = 10,
        label_09 = 11,
        label_10 = 12,
        label_11 = 13,
        toggle_00 = 14,
        toggle_01 = 15,
        toggle_02 = 16,
        toggle_03 = 17,
        toggle_04 = 18,
        toggle_05 = 19,
        toggle_06 = 20,
        toggle_07 = 21,
        toggle_08 = 22,
        toggle_09 = 23,
        toggle_10 = 24,
        toggle_11 = 25,
        scrollUp = 26,
        scrollDown = 27,
        enableAllCpp = 28,
        enableAllAsm = 29,
        exit = 30,
        compareToggle = 31,
        compareStatus = 32,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& scrollInfoEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& label_00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& label_01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& label_02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& label_03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& label_04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& label_05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& label_06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& label_07Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& label_08Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& label_09Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& label_10Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& label_11Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& toggle_00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& toggle_01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& toggle_02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& toggle_03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& toggle_04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& toggle_05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& toggle_06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& toggle_07Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& toggle_08Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& toggle_09Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& toggle_10Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& toggle_11Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& scrollUpEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& scrollDownEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 27 * sizeof(MenuEntry));
    static auto& enableAllCppEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 28 * sizeof(MenuEntry));
    static auto& enableAllAsmEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 29 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 30 * sizeof(MenuEntry));
    static auto& compareToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 31 * sizeof(MenuEntry));
    static auto& compareStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 32 * sizeof(MenuEntry));

    constexpr int kFirstLabelEntry = 1 +1;
    constexpr int kFirstToggleEntry = (13) +1;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void doSaveAndExit() {}
static void toggleCompareMode() {}
static void updateScrollInfo() {}
static void doScrollDown() {}
static void drawToggleValue() {}
static void drawCompareStatus() {}
static void doEnableAllAsm() {}
static void doEnableAllCpp() {}
static void drawToggleLabel() {}
static void selectToggle() {}
static void doScrollUp() {}
static void portedFunctionsMenuOnInit() {}
static void drawCompareToggle() {}
static void portedFunctionsMenuOnReturn() {}
#endif
