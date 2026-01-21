// automatically generated from configureAxis.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void deleteCurrentInterval();
static void createInterval();
static void inputCurrentInterval();
static void pickEvents();
static void goToPreviousInterval();
static void inputIntervalEnd();
static void inputIntervalStart();
static void configureAxisMenuOnDraw();
static void configureAxisMenuOnInit();
static void inputCurrentAxis();
static void goToNextInterval();
static void configureAxisMenuOnRestore();
static void goToPreviousAxis();
static void goToNextAxis();

#pragma pack(push, 1)
struct SwosMenu_configureAxisMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, configureAxisMenuOnInit, configureAxisMenuOnRestore, configureAxisMenuOnDraw, 0, 7, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryText et00{ 0, -1 };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ -8, 20, 320, 10 };
         EntryText et01{ 0, -1 };
    EntryEnd ee01{};

    Entry eb02{ ((320) -2 * (20) -(20) -2 * 2) / 2 -8, (((20 + 10))) +2, 20, 10 };
         EntryForegroundSprite efs02{ 179 };
         EntryColor ec02{ 3 };
         EntryNextPositions ep02{ 255, 3, 255, 7 };
         EntryOnSelectFunctionNative eosf02{ goToPreviousAxis };
    EntryEnd ee02{};

    Entry eb03{ ((((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20)))) +(2), (((((20 + 10))) +2)), 20, 10 };
         EntryNumber en03{ 0, 0 };
         EntryColor ec03{ 13 };
         EntryNextPositions ep03{ 2, 4, 255, 7 };
         EntryOnSelectFunctionNative eosf03{ inputCurrentAxis };
    EntryEnd ee03{};

    Entry eb04{ ((((((((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20)))) +(2) + 20)))) +(2), (((((((20 + 10))) +2)))), 20, 10 };
         EntryForegroundSprite efs04{ 175 };
         EntryColor ec04{ 3 };
         EntryNextPositions ep04{ 3, 255, 255, 7 };
         EntryOnSelectFunctionNative eosf04{ goToNextAxis };
    EntryEnd ee04{};

    Entry eb05{ -8, 46, 320, 10 };
         EntryText et05{ 0, -1 };
    EntryEnd ee05{};

    Entry eb06{ ((320) -2 * (20) -(20) -2 * 2) / 2 -8, (((46 + 10))) +2, 20, 10 };
         EntryForegroundSprite efs06{ 179 };
         EntryColor ec06{ 3 };
         EntryNextPositions ep06{ 255, 7, 2, 10 };
         EntryOnSelectFunctionNative eosf06{ goToPreviousInterval };
    EntryEnd ee06{};

    Entry eb07{ ((((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20)))) +(2), (((((46 + 10))) +2)), 20, 10 };
         EntryNumber en07{ 0, 0 };
         EntryColor ec07{ 13 };
         EntryNextPositions ep07{ 6, 8, 3, 10 };
         EntryOnSelectFunctionNative eosf07{ inputCurrentInterval };
    EntryEnd ee07{};

    Entry eb08{ ((((((((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20)))) +(2) + 20)))) +(2), (((((((46 + 10))) +2)))), 20, 10 };
         EntryForegroundSprite efs08{ 175 };
         EntryColor ec08{ 3 };
         EntryNextPositions ep08{ 7, 255, 4, 10 };
         EntryOnSelectFunctionNative eosf08{ goToNextInterval };
    EntryEnd ee08{};

    Entry eb09{ (((320) -(28) -(48) -2 * 2) / 2 -8), 78, 28, 10 };
         EntryTextNative et09{ 16384, "FROM:" };
    EntryEnd ee09{};

    Entry eb10{ (((((((320) -(28) -(48) -2 * 2) / 2 -8) + 28)))) +2 * (2), ((78)), 48, 10 };
         EntryText et10{ 0, -1 };
         EntryColor ec10{ 13 };
         EntryNextPositions ep10{ 255, 255, 7, 12 };
         EntryOnSelectFunctionNative eosf10{ inputIntervalStart };
    EntryEnd ee10{};

    Entry eb11{ (((320) -(28) -(48) -2 * 2) / 2 -8), (78) +(10) +1, 28, 10 };
         EntryTextNative et11{ 16384, "TO:" };
    EntryEnd ee11{};

    Entry eb12{ (((((((320) -(28) -(48) -2 * 2) / 2 -8) + 28)))) +2 * (2), (((78) +(10) +1)), 48, 10 };
         EntryText et12{ 0, -1 };
         EntryColor ec12{ 13 };
         EntryNextPositions ep12{ 255, 255, 10, 15 };
         EntryOnSelectFunctionNative eosf12{ inputIntervalEnd };
    EntryEnd ee12{};

    Entry eb13{ -8, (((((78) +(10) +1)))) +(10) +2 * 2, 320, 10 };
         EntryTextNative et13{ 0, "(MIN: -32768  MAX: 32767)" };
    EntryEnd ee13{};

    Entry eb14{ -8, (((((((78) +(10) +1)))) +(10) +2 * 2)) +(10) +2 * 2, 320, 10 };
         EntryTextNative et14{ 0, "EVENTS:" };
    EntryEnd ee14{};

    Entry eb15{ (((320) -(125)) / 2 -8), (((((((((78) +(10) +1)))) +(10) +2 * 2)) +(10) +2 * 2)) +(10) +2 -1, 125, 10 };
         EntryText et15{ 0, -1 };
         EntryColor ec15{ 13 };
         EntryNextPositions ep15{ 255, 255, 12, 16 };
         EntryOnSelectFunctionNative eosf15{ pickEvents };
    EntryEnd ee15{};

    Entry eb16{ (((320) -(125)) / 2 -8), ((200) -(15)) -2 * ((15) +2), 125, (15) -2 };
         EntryTextNative et16{ 0, "ADD NEW INTERVAL" };
         EntryColor ec16{ 14 };
         EntryNextPositions ep16{ 255, 255, 15, 17 };
         EntryOnSelectFunctionNative eosf16{ createInterval };
    EntryEnd ee16{};

    Entry eb17{ (((320) -(125)) / 2 -8), ((200) -(15)) -(15) -2, 125, (15) -2 };
         EntryTextNative et17{ 0, "DELETE INTERVAL" };
         EntryColor ec17{ 14 };
         EntryNextPositions ep17{ 255, 255, 16, 18 };
         EntryOnSelectFunctionNative eosf17{ deleteCurrentInterval };
    EntryEnd ee17{};

    Entry eb18{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, (15) -2 };
         EntryText et18{ 0, SwosVM::Offsets::aExit };
         EntryColor ec18{ 4 };
         EntryNextPositions ep18{ 255, 255, 17, 255 };
         EntryOnSelectFunctionNative eosf18{ SetExitMenuFlag };
    EntryEnd ee18{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const configureAxisMenu
#endif
;
namespace ConfigureAxisMenu {
    enum Entries {
        header = 0,
        axisLabel = 1,
        prevAxis = 2,
        currentAxis = 3,
        nextAxis = 4,
        intervalLabel = 5,
        prevInterval = 6,
        currentInterval = 7,
        nextInterval = 8,
        fromInput = 10,
        toInput = 12,
        events = 15,
        addNewInterval = 16,
        deleteInterval = 17,
        exit = 18,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& axisLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& prevAxisEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& currentAxisEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& nextAxisEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& intervalLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& prevIntervalEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& currentIntervalEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& nextIntervalEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& fromInputEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& toInputEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& eventsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& addNewIntervalEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& deleteIntervalEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));

    constexpr int kRowGap = 2;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void deleteCurrentInterval() {}
static void createInterval() {}
static void inputCurrentInterval() {}
static void pickEvents() {}
static void goToPreviousInterval() {}
static void inputIntervalEnd() {}
static void inputIntervalStart() {}
static void configureAxisMenuOnDraw() {}
static void configureAxisMenuOnInit() {}
static void inputCurrentAxis() {}
static void goToNextInterval() {}
static void configureAxisMenuOnRestore() {}
static void goToPreviousAxis() {}
static void goToNextAxis() {}
#endif
