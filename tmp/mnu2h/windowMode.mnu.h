// automatically generated from windowMode.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void scrollResolutionsUpSelected();
static void scrollResolutionsDownSelected();
static void windowModeMenuOnDraw();
static void inputWindowHeight();
static void windowModeMenuOnInit();
static void changeResolutionSelected();
static void inputWindowWidth();

#pragma pack(push, 1)
struct StringTableNative2 : public StringTableNative {
    const char *strings[2];
    const bool nativeFlags[3];
    constexpr StringTableNative2(int16_t *index, int16_t initialValue,
        const char *str0, const char *str1,
        bool flag0, bool flag1, bool flag2)
    :
        StringTableNative(index, initialValue, 2),
        strings{str0, str1},
        nativeFlags{flag0, flag1, flag2}
    {}
};

const StringTableNative2 windowModeMenu_resizableOnOff_stringTable {
    &m_windowResizable, 0, "OFF", "ON",
    true, true, true
};

const StringTableNative2 windowModeMenu_maximizedOnOff_stringTable {
    &m_windowMaximized, 0, "OFF", "ON",
    true, true, true
};

#pragma pack(pop)
#pragma pack(push, 1)
struct SwosMenu_windowModeMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, windowModeMenuOnInit, windowModeMenuOnInit, windowModeMenuOnDraw, 0, 2, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "WINDOW MODE" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ 74, 20, 10, 12 };
         EntryForegroundSprite efs01{ 175 };
    EntryEnd ee01{};

    Entry eb02{ (((74 + 10))) +3, 20, ((44)) +4, 12 };
         EntryTextNative et02{ 32768, "WINDOWED" };
         EntryNextPositions ep02{ 255, 3, 255, 6 };
         EntryOnSelectFunctionNative eosf02{ switchToWindow };
    EntryEnd ee02{};

    Entry eb03{ (((((((74 + 10))) +3 + ((44)) +4)))) +1, ((20)) +1, ((36) +4), 10 };
         EntryNumber en03{ 0, 9999 };
         EntryColor ec03{ 13 };
         EntryNextPositions ep03{ 2, 5, 255, 7 };
         EntryOnSelectFunctionNative eosf03{ inputWindowWidth };
    EntryEnd ee03{};

    Entry eb04{ ((((((((((74 + 10))) +3 + ((44)) +4)))) +1 + ((36) +4)))) +2, 20, 10, 12 };
         EntryTextNative et04{ 32768, "X" };
    EntryEnd ee04{};

    Entry eb05{ (((((((((((((74 + 10))) +3 + ((44)) +4)))) +1 + ((36) +4)))) +2 + 10))) -3, ((20)) +1, ((36) +4), 10 };
         EntryNumber en05{ 0, 9999 };
         EntryColor ec05{ 13 };
         EntryNextPositions ep05{ 3, 255, 255, 7 };
         EntryOnSelectFunctionNative eosf05{ inputWindowHeight };
    EntryEnd ee05{};

    Entry eb06{ (96) +(10), (20) +(12), ((50)) +4, (12) -2 };
         EntryTextNative et06{ 0, "RESIZABLE" };
         EntryNextPositions ep06{ 255, 7, 2, 8 };
         EntryOnSelectFunctionNative eosf06{ toggleWindowResizable };
    EntryEnd ee06{};

    Entry eb07{ (((((96) +(10) + ((50)) +4)))) +1, (((20) +(12))), ((18)) +3 +4, (12) -2 };
         EntryStringTableNative est07{ 0, &windowModeMenu_resizableOnOff_stringTable };
         EntryColor ec07{ 13 };
         EntryNextPositions ep07{ 6, 255, 3, 8 };
         EntryOnSelectFunctionNative eosf07{ toggleWindowResizable };
    EntryEnd ee07{};

    Entry eb08{ ((96) +(10)), (((20) +(12) + (12) -2)) +1, ((46)) +4, ((12) -2) };
         EntryTextNative et08{ 0, "MAXIMIZED" };
         EntryNextPositions ep08{ 255, 9, 6, 11 };
         EntryOnSelectFunctionNative eosf08{ toggleWindowMaximized };
    EntryEnd ee08{};

    Entry eb09{ ((((((96) +(10) + ((50)) +4)))) +1), (((((20) +(12) + (12) -2)) +1)), ((18)) +3 +4, (12) -2 };
         EntryStringTableNative est09{ 0, &windowModeMenu_maximizedOnOff_stringTable };
         EntryColor ec09{ 13 };
         EntryNextPositions ep09{ 8, 255, 7, 11 };
         EntryOnSelectFunctionNative eosf09{ toggleWindowMaximized };
    EntryEnd ee09{};

    Entry eb10{ 82, ((20) +3 * (12) -2), 10, 12 };
         EntryForegroundSprite efs10{ 175 };
    EntryEnd ee10{};

    Entry eb11{ 96, ((20) +3 * (12) -2), ((109)) +5, ((12) -2) };
         EntryTextNative et11{ 0, "BORDERLESS MAXIMIZED" };
         EntryNextPositions ep11{ 255, 255, 6, 14 };
         EntryOnSelectFunctionNative eosf11{ switchToBorderlessMaximized };
    EntryEnd ee11{};

    Entry eb12{ 82, ((80) -(12) +1), 10, 12 };
         EntryForegroundSprite efs12{ 175 };
    EntryEnd ee12{};

    Entry eb13{ 96, ((80) -(12) +1), ((125) -(10)) +(14), 12 };
         EntryText et13{ 7, -1 };
         EntryNextPositions ep13{ 255, 255, 11, 14 };
    EntryEnd ee13{};

    TemplateEntry te00{};

    Entry eb30014{ 0, 0, 1, 1 };
         EntryText et30014{ 0, -1 };
         EntryColor ec30014{ 13 };
         EntryOnSelectFunctionNative eosf30014{ changeResolutionSelected };
    EntryEnd ee30014{};

    Entry eb14{ ((((320) -(125)) / 2 -8) +4), (80) +0 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep14{ 255, 26, 11, 15 };
    EntryEnd ee14{};

    Entry eb15{ ((((320) -(125)) / 2 -8) +4), (80) +1 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep15{ 255, 26, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ ((((320) -(125)) / 2 -8) +4), (80) +2 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep16{ 255, 26, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ ((((320) -(125)) / 2 -8) +4), (80) +3 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep17{ 255, 26, 16, 18 };
    EntryEnd ee17{};

    Entry eb18{ ((((320) -(125)) / 2 -8) +4), (80) +4 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep18{ 255, 27, 17, 19 };
    EntryEnd ee18{};

    Entry eb19{ ((((320) -(125)) / 2 -8) +4), (80) +5 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep19{ 255, 27, 18, 20 };
    EntryEnd ee19{};

    Entry eb20{ ((((320) -(125)) / 2 -8) +4), (80) +6 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep20{ 255, 27, 19, 21 };
    EntryEnd ee20{};

    Entry eb21{ ((((320) -(125)) / 2 -8) +4), (80) +7 * (12), ((125) -(10)), 12 };
         EntryNextPositions ep21{ 255, 27, 20, 28 };
    EntryEnd ee21{};

    Entry eb22{ ((((320) -(125)) / 2 -8) +4), (80) +8 * (12), ((125) -(10)), 12 };
         EntryInvisible ei22{};
         EntryNextPositions ep22{ 255, 27, 21, 23 };
    EntryEnd ee22{};

    Entry eb23{ ((((320) -(125)) / 2 -8) +4), (80) +9 * (12), ((125) -(10)), 12 };
         EntryInvisible ei23{};
         EntryNextPositions ep23{ 255, 27, 22, 24 };
    EntryEnd ee23{};

    Entry eb24{ ((((320) -(125)) / 2 -8) +4), (80) +(10) * (12), ((125) -(10)), 12 };
         EntryInvisible ei24{};
         EntryNextPositions ep24{ 255, 27, 23, 25 };
    EntryEnd ee24{};

    Entry eb25{ ((((320) -(125)) / 2 -8) +4), (80) +(11) * (12), ((125) -(10)), 12 };
         EntryInvisible ei25{};
         EntryNextPositions ep25{ 255, 27, 24, 26 };
    EntryEnd ee25{};

    ResetTemplateEntry rte00{};

    Entry eb26{ (((((320) -(125)) / 2 -8) +4) +((125) -(10)) +2), ((80) +0 * (12)), 8, 14 };
         EntryForegroundSprite efs26{ 183 };
         EntryNextPositions ep26{ 14, 255, 11, 27 };
         EntryOnSelectFunctionNative eosf26{ scrollResolutionsUpSelected };
    EntryEnd ee26{};

    Entry eb27{ (((((320) -(125)) / 2 -8) +4) +((125) -(10)) +2), ((80) +8 * (12) -(14)), 8, 14 };
         EntryForegroundSprite efs27{ 184 };
         EntryNextPositions ep27{ 19, 255, 26, 28 };
         EntryOnSelectFunctionNative eosf27{ scrollResolutionsDownSelected };
    EntryEnd ee27{};

    Entry eb28{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et28{ 0, SwosVM::Offsets::aExit };
         EntryColor ec28{ 4 };
         EntryNextPositions ep28{ 255, 255, 19, 255 };
         EntryOnSelectFunctionNative eosf28{ SetExitMenuFlag };
    EntryEnd ee28{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const windowModeMenu
#endif
;
namespace WindowModeMenu {
    enum Entries {
        header = 0,
        customSizeArrow = 1,
        windowed = 2,
        customWidth = 3,
        customHeight = 5,
        resizable = 6,
        resizableOnOff = 7,
        maximized = 8,
        maximizedOnOff = 9,
        borderlessMaximizedArrow = 10,
        borderlessMaximized = 11,
        fullScreenArrow = 12,
        fullScreen = 13,
        resolutionField0 = 14,
        resolutionField1 = 15,
        resolutionField2 = 16,
        resolutionField3 = 17,
        resolutionField4 = 18,
        resolutionField5 = 19,
        resolutionField6 = 20,
        resolutionField7 = 21,
        resolutionField8 = 22,
        resolutionField9 = 23,
        resolutionField10 = 24,
        resolutionField11 = 25,
        scrollUpArrow = 26,
        scrollDownArrow = 27,
        exit = 28,
        firstResolutionField = 14,
        lastResolutionField = 25,
        lastResolutionFieldShort = 21,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& customSizeArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& windowedEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& customWidthEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& customHeightEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& resizableEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& resizableOnOffEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& maximizedEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& maximizedOnOffEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& borderlessMaximizedArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& borderlessMaximizedEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& fullScreenArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& fullScreenEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& resolutionField0Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& resolutionField1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& resolutionField2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& resolutionField3Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& resolutionField4Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& resolutionField5Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& resolutionField6Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& resolutionField7Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& resolutionField8Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& resolutionField9Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& resolutionField10Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& resolutionField11Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& scrollUpArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& scrollDownArrowEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 27 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 28 * sizeof(MenuEntry));
    static auto& firstResolutionFieldEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& lastResolutionFieldEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& lastResolutionFieldShortEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));

    constexpr int kNumResolutionFieldsShort = 8;
    constexpr int kNumResolutionFieldsFull = 12;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void scrollResolutionsUpSelected() {}
static void scrollResolutionsDownSelected() {}
static void windowModeMenuOnDraw() {}
static void inputWindowHeight() {}
static void windowModeMenuOnInit() {}
static void changeResolutionSelected() {}
static void inputWindowWidth() {}
#endif
