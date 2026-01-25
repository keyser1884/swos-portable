// automatically generated from options.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void initGamePlayOptions();
static void showGameplayOptions();
static void doShowControlOptionsMenu();
static void doShowPortedFunctionsMenu();
static void doShowVideoOptionsMenu();
static void doShowAudioOptionsMenu();
static void toggleAutoSaveReplays();
static void exitOptions();
static void toggleGameStyle();

#pragma pack(push, 1)
struct StringTableNative9 : public StringTableNative {
    const char *strings[9];
    const bool nativeFlags[10];
    constexpr StringTableNative9(int16_t *index, int16_t initialValue,
        const char *str0, const char *str1, const char *str2, const char *str3, const char *str4, const char *str5, const char *str6, const char *str7, const char *str8,
        bool flag0, bool flag1, bool flag2, bool flag3, bool flag4, bool flag5, bool flag6, bool flag7, bool flag8, bool flag9)
    :
        StringTableNative(index, initialValue, 9),
        strings{str0, str1, str2, str3, str4, str5, str6, str7, str8},
        nativeFlags{flag0, flag1, flag2, flag3, flag4, flag5, flag6, flag7, flag8, flag9}
    {}
};
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
struct StringTableNative4 : public StringTableNative {
    const char *strings[4];
    const bool nativeFlags[5];
    constexpr StringTableNative4(int16_t *index, int16_t initialValue,
        const char *str0, const char *str1, const char *str2, const char *str3,
        bool flag0, bool flag1, bool flag2, bool flag3, bool flag4)
    :
        StringTableNative(index, initialValue, 4),
        strings{str0, str1, str2, str3},
        nativeFlags{flag0, flag1, flag2, flag3, flag4}
    {}
};

const StringTableNative4 gameplayOptionsMenu_gameLength_stringTable {
    reinterpret_cast<int16_t *>(SwosVM::Offsets::g_gameLength), 0, "3 MINS", "5 MINS", "7 MINS", "10 MINS",
    false, true, true, true, true
};

const StringTableNative9 gameplayOptionsMenu_10_stringTable {
    reinterpret_cast<int16_t *>(SwosVM::Offsets::g_pitchType), -2, "SEASONAL", "RANDOM", "FROZEN", "MUDDY", "WET", "SOFT", "NORMAL", "DRY", "HARD",
    false, true, true, true, true, true, true, true, true, true
};

const StringTableNative2 gameplayOptionsMenu_11_stringTable {
    reinterpret_cast<int16_t *>(SwosVM::Offsets::g_autoReplays), 0, "OFF", "ON",
    false, true, true
};

const StringTableNative2 gameplayOptionsMenu_12_stringTable {
    reinterpret_cast<int16_t *>(SwosVM::Offsets::g_allPlayerTeamsEqual), 0, "OFF", "ON",
    false, true, true
};

const StringTableNative2 gameplayOptionsMenu_13_stringTable {
    reinterpret_cast<int16_t *>(SwosVM::Offsets::g_autoSaveHighlights), 0, "OFF", "ON",
    false, true, true
};

const StringTableNative2 gameplayOptionsMenu_gameStyle_stringTable {
    &m_gameStyle, 0, "PC", "AMIGA",
    true, true, true
};

#pragma pack(pop)
#pragma pack(push, 1)
struct SwosMenu_optionsMenu : public BaseMenu
{
    MenuHeader header{ 0, 0, 0, 1 };

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryText et00{ 0, SwosVM::Offsets::aOptions };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ (((320) -(125)) / 2 -8), 48, 125, 15 };
         EntryTextNative et01{ 0, "CONTROLS" };
         EntryColor ec01{ 14 };
         EntryNextPositions ep01{ 255, 255, 255, 2 };
         EntryOnSelectFunctionNative eosf01{ doShowControlOptionsMenu };
    EntryEnd ee01{};

    Entry eb02{ (((320) -(125)) / 2 -8), ((((48 + 15)))) +(12), 125, 15 };
         EntryTextNative et02{ 0, "VIDEO" };
         EntryColor ec02{ 14 };
         EntryNextPositions ep02{ 255, 255, 1, 3 };
         EntryOnSelectFunctionNative eosf02{ doShowVideoOptionsMenu };
    EntryEnd ee02{};

    Entry eb03{ (((320) -(125)) / 2 -8), ((((((((48 + 15)))) +(12) + 15)))) +(12), 125, 15 };
         EntryTextNative et03{ 0, "AUDIO" };
         EntryColor ec03{ 14 };
         EntryNextPositions ep03{ 255, 255, 2, 4 };
         EntryOnSelectFunctionNative eosf03{ doShowAudioOptionsMenu };
    EntryEnd ee03{};

    Entry eb04{ (((320) -(125)) / 2 -8), ((((((((((((48 + 15)))) +(12) + 15)))) +(12) + 15)))) +(12), 125, 15 };
         EntryTextNative et04{ 0, "GAMEPLAY" };
         EntryColor ec04{ 14 };
         EntryNextPositions ep04{ 255, 255, 3, 5 };
         EntryOnSelectFunctionNative eosf04{ showGameplayOptions };
    EntryEnd ee04{};

    Entry eb05{ (((320) -(125)) / 2 -8), ((((((((((((((((48 + 15)))) +(12) + 15)))) +(12) + 15)))) +(12) + 15)))) +(12), 125, 15 };
         EntryTextNative et05{ 0, "PORT MENU" };
         EntryColor ec05{ 14 };
         EntryNextPositions ep05{ 255, 255, 4, 6 };
         EntryOnSelectFunctionNative eosf05{ doShowPortedFunctionsMenu };
    EntryEnd ee05{};

    Entry eb06{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et06{ 0, SwosVM::Offsets::aExit };
         EntryColor ec06{ 4 };
         EntryNextPositions ep06{ 255, 255, 5, 255 };
         EntryOnSelectFunctionWithMaskNative eosfm06{ exitOptions, (32) | (16) };
    EntryEnd ee06{};

    Entry eb07{ -8, 0, 320, 15 };
         EntryTextNative et07{ 15, "WHO'S JOHNNY? SHE SAID AND SMILED IN HER SPECIAL WAY" };
         EntryColor ec07{ (10) | (240) };
         EntryInvisible ei07{};
    EntryEnd ee07{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const optionsMenu
#endif
;
namespace OptionsMenu {
    enum Entries {
        header = 0,
        controls = 1,
        video = 2,
        audio = 3,
        gameplay = 4,
        portedFunctions = 5,
        exit = 6,
        secret = 7,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& controlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& videoEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& audioEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& gameplayEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& portedFunctionsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& secretEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
}
struct SwosMenu_gameplayOptionsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, initGamePlayOptions, initGamePlayOptions, 0, 0, 9, { true, true, false, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "GAMEPLAY OPTIONS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    TemplateEntry te00{};

    Entry eb30001{ 0, 0, 1, 1 };
         EntryColor ec30001{ 7 };
    EntryEnd ee30001{};

    Entry eb01{ 10, ((60) -(11)), 140, 11 };
         EntryText et01{ 0, SwosVM::Offsets::aFriendlyDiyGam };
    EntryEnd ee01{};

    Entry eb02{ 10, ((((((60) -(11)) + 11)))) +(1), 140, 11 };
         EntryTextNative et02{ 0, "FIX PITCH TYPE" };
    EntryEnd ee02{};

    Entry eb03{ 10, ((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et03{ 0, "AUTO REPLAYS" };
    EntryEnd ee03{};

    Entry eb04{ 10, ((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et04{ 0, "ALL PLAYER TEAMS EQUAL" };
    EntryEnd ee04{};

    Entry eb05{ 10, ((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et05{ 0, "AUTO SAVE HIGHLIGHTS" };
    EntryEnd ee05{};

    Entry eb06{ 10, ((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et06{ 0, "AUTO SAVE REPLAYS" };
    EntryEnd ee06{};

    Entry eb07{ 10, ((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et07{ 0, "SHOW PRE-MATCH MENUS" };
    EntryEnd ee07{};

    Entry eb08{ 10, ((((((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et08{ 0, "GAME STYLE" };
    EntryEnd ee08{};

    TemplateEntry te01{};

    Entry eb30010{ 0, 0, 1, 1 };
         EntryColor ec30010{ 13 };
    EntryEnd ee30010{};

    Entry eb09{ 154, ((60) -(11)), 140, 11 };
         EntryStringTableNative est09{ 0, &gameplayOptionsMenu_gameLength_stringTable };
         EntryNextPositions ep09{ 255, 255, 255, 10 };
         EntryOnSelectFunction eosf09{ SwosVM::Procs::ChangeGameLength };
    EntryEnd ee09{};

    Entry eb10{ 154, ((((((60) -(11)) + 11)))) +(1), 140, 11 };
         EntryStringTableNative est10{ 0, &gameplayOptionsMenu_10_stringTable };
         EntryNextPositions ep10{ 255, 255, 9, 11 };
         EntryOnSelectFunction eosf10{ SwosVM::Procs::ChangePitchType };
    EntryEnd ee10{};

    Entry eb11{ 154, ((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryStringTableNative est11{ 0, &gameplayOptionsMenu_11_stringTable };
         EntryNextPositions ep11{ 255, 255, 10, 12 };
         EntryOnSelectFunction eosf11{ SwosVM::Procs::ChangeAutoReplays };
    EntryEnd ee11{};

    Entry eb12{ 154, ((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryStringTableNative est12{ 0, &gameplayOptionsMenu_12_stringTable };
         EntryNextPositions ep12{ 255, 255, 11, 13 };
         EntryOnSelectFunction eosf12{ SwosVM::Procs::ChangeAllPlTeamsEqual };
    EntryEnd ee12{};

    Entry eb13{ 154, ((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryStringTableNative est13{ 0, &gameplayOptionsMenu_13_stringTable };
         EntryNextPositions ep13{ 255, 255, 12, 14 };
         EntryOnSelectFunction eosf13{ SwosVM::Procs::ChangeAutoSaveHighlights };
    EntryEnd ee13{};

    Entry eb14{ 154, ((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryText et14{ 0, -1 };
         EntryNextPositions ep14{ 255, 255, 13, 15 };
         EntryOnSelectFunctionNative eosf14{ toggleAutoSaveReplays };
    EntryEnd ee14{};

    Entry eb15{ 154, ((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo15{ showPreMatchMenus, setPreMatchMenus, "pre-match menus" };
         EntryNextPositions ep15{ 255, 255, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ 154, ((((((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryStringTableNative est16{ 0, &gameplayOptionsMenu_gameStyle_stringTable };
         EntryNextPositions ep16{ 255, 255, 15, 17 };
         EntryOnSelectFunctionNative eosf16{ toggleGameStyle };
    EntryEnd ee16{};

    Entry eb17{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et17{ 0, SwosVM::Offsets::aExit };
         EntryColor ec17{ 4 };
         EntryNextPositions ep17{ 255, 255, 16, 255 };
         EntryOnSelectFunction eosf17{ SwosVM::Procs::SetExitMenuFlag };
    EntryEnd ee17{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const gameplayOptionsMenu
#endif
;
namespace GameplayOptionsMenu {
    enum Entries {
        header = 0,
        gameLength = 9,
        autoSaveReplays = 14,
        gameStyle = 16,
        exit = 17,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& gameLengthEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& autoSaveReplaysEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& gameStyleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void initGamePlayOptions() {}
static void showGameplayOptions() {}
static void doShowControlOptionsMenu() {}
static void doShowPortedFunctionsMenu() {}
static void doShowVideoOptionsMenu() {}
static void doShowAudioOptionsMenu() {}
static void toggleAutoSaveReplays() {}
static void exitOptions() {}
static void toggleGameStyle() {}
#endif
