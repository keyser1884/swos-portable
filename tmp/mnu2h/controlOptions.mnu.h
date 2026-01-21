// automatically generated from controlOptions.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void controlOptionsMenuOnDraw();
static void pl2ScrollUpSelected();
static void redefinePlayer2Controls();
static void pl2SelectControls();
static void pl1ScrollDownSelected();
static void exitControlsMenu();
static void redefinePlayer1Controls();
static void pl1ScrollUpSelected();
static void pl2ScrollDownSelected();
static void controlOptionsMenuInit();
static void pl1SelectControls();

#pragma pack(push, 1)
struct SwosMenu_controlOptionsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, controlOptionsMenuInit, controlOptionsMenuOnDraw, controlOptionsMenuOnDraw, 0, 2, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "SELECT CONTROLS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ (((320) -2 * (144) -6) / 2 -8), 30, 144, (11) +1 };
         EntryTextNative et01{ 0, "PLAYER 1" };
    EntryEnd ee01{};

    Entry eb02{ (((320) -2 * (144) -6) / 2 -8), ((((30 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et02{ 0, -1 };
         EntryColor ec02{ 13 };
         EntryNextPositions ep02{ 6, 9, 255, 3 };
         EntryLeftSkip els02{ 1, 2 };
         EntryDownSkip eds02{ 3, 3 };
         EntryOnSelectFunctionNative eosf02{ pl1SelectControls };
    EntryEnd ee02{};

    Entry eb03{ (((320) -2 * (144) -6) / 2 -8), ((((((((30 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et03{ 0, -1 };
         EntryColor ec03{ 13 };
         EntryNextPositions ep03{ 6, 10, 2, 4 };
         EntryLeftSkip els03{ 2, 2 };
         EntryUpSkip eus03{ 2, 2 };
         EntryDownSkip eds03{ 4, 3 };
         EntryOnSelectFunctionNative eosf03{ pl1SelectControls };
    EntryEnd ee03{};

    Entry eb04{ (((320) -2 * (144) -6) / 2 -8), ((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et04{ 0, -1 };
         EntryColor ec04{ 13 };
         EntryNextPositions ep04{ 7, 11, 3, 5 };
         EntryLeftSkip els04{ 3, 2 };
         EntryUpSkip eus04{ 3, 2 };
         EntryDownSkip eds04{ 5, 3 };
         EntryOnSelectFunctionNative eosf04{ pl1SelectControls };
    EntryEnd ee04{};

    Entry eb05{ (((320) -2 * (144) -6) / 2 -8), ((((((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et05{ 0, -1 };
         EntryColor ec05{ 13 };
         EntryNextPositions ep05{ 7, 12, 4, 16 };
         EntryLeftSkip els05{ 4, 2 };
         EntryUpSkip eus05{ 4, 2 };
         EntryDownSkip eds05{ 16, 3 };
         EntryOnSelectFunctionNative eosf05{ pl1SelectControls };
    EntryEnd ee05{};

    Entry eb06{ ((((320) -2 * (144) -6) / 2 -8) -8 -2), (((((30 + (11) +1)))) +2), 8, 14 };
         EntryForegroundSprite efs06{ 183 };
         EntryNextPositions ep06{ 255, 2, 255, 7 };
         EntryOnSelectFunctionNative eosf06{ pl1ScrollUpSelected };
    EntryEnd ee06{};

    Entry eb07{ ((((320) -2 * (144) -6) / 2 -8) -8 -2), (((((((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2), 8, 14 };
         EntryForegroundSprite efs07{ 184 };
         EntryNextPositions ep07{ 255, 5, 6, 16 };
         EntryOnSelectFunctionNative eosf07{ pl1ScrollDownSelected };
    EntryEnd ee07{};

    Entry eb08{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), 30, 144, (11) +1 };
         EntryTextNative et08{ 0, "PLAYER 2" };
    EntryEnd ee08{};

    Entry eb09{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((30 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et09{ 0, -1 };
         EntryColor ec09{ 13 };
         EntryNextPositions ep09{ 2, 13, 255, 10 };
         EntryRightSkip ers09{ 8, 2 };
         EntryDownSkip eds09{ 10, 3 };
         EntryOnSelectFunctionNative eosf09{ pl2SelectControls };
    EntryEnd ee09{};

    Entry eb10{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((30 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et10{ 0, -1 };
         EntryColor ec10{ 13 };
         EntryNextPositions ep10{ 3, 13, 9, 11 };
         EntryRightSkip ers10{ 9, 2 };
         EntryUpSkip eus10{ 9, 2 };
         EntryDownSkip eds10{ 11, 3 };
         EntryOnSelectFunctionNative eosf10{ pl2SelectControls };
    EntryEnd ee10{};

    Entry eb11{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et11{ 0, -1 };
         EntryColor ec11{ 13 };
         EntryNextPositions ep11{ 4, 14, 10, 12 };
         EntryRightSkip ers11{ 10, 2 };
         EntryUpSkip eus11{ 10, 2 };
         EntryDownSkip eds11{ 12, 3 };
         EntryOnSelectFunctionNative eosf11{ pl2SelectControls };
    EntryEnd ee11{};

    Entry eb12{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2, 144, (11) +1 };
         EntryText et12{ 0, -1 };
         EntryColor ec12{ 13 };
         EntryNextPositions ep12{ 5, 14, 11, 15 };
         EntryRightSkip ers12{ 11, 2 };
         EntryUpSkip eus12{ 11, 2 };
         EntryDownSkip eds12{ 15, 3 };
         EntryOnSelectFunctionNative eosf12{ pl2SelectControls };
    EntryEnd ee12{};

    Entry eb13{ (((((320) -2 * (144) -6) / 2 -8) +(144) +6) +(144) +2), (((((30 + (11) +1)))) +2), 8, 14 };
         EntryForegroundSprite efs13{ 183 };
         EntryNextPositions ep13{ 9, 255, 255, 14 };
         EntryOnSelectFunctionNative eosf13{ pl2ScrollUpSelected };
    EntryEnd ee13{};

    Entry eb14{ (((((320) -2 * (144) -6) / 2 -8) +(144) +6) +(144) +2), (((((((((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2 + (11) +1)))) +2), 8, 14 };
         EntryForegroundSprite efs14{ 184 };
         EntryNextPositions ep14{ 12, 255, 13, 15 };
         EntryOnSelectFunctionNative eosf14{ pl2ScrollDownSelected };
    EntryEnd ee14{};

    Entry eb15{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), (((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10), 144, (11) +1 };
         EntryTextNative et15{ 0, "SETUP CONTROLS" };
         EntryColor ec15{ 14 };
         EntryNextPositions ep15{ 16, 255, 12, 20 };
         EntryRightSkip ers15{ 255, 3 };
         EntryUpSkip eus15{ 12, 2 };
         EntryOnSelectFunctionNative eosf15{ redefinePlayer2Controls };
    EntryEnd ee15{};

    Entry eb16{ (((320) -2 * (144) -6) / 2 -8), ((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)), 144, (11) +1 };
         EntryTextNative et16{ 0, "SETUP CONTROLS" };
         EntryColor ec16{ 14 };
         EntryNextPositions ep16{ 255, 15, 5, 23 };
         EntryLeftSkip els16{ 255, 3 };
         EntryUpSkip eus16{ 5, 2 };
         EntryOnSelectFunctionNative eosf16{ redefinePlayer1Controls };
    EntryEnd ee16{};

    Entry eb17{ (((320) -2 * (144) -6) / 2 -8), (((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10), 144, 11 };
         EntryTextNative et17{ 0, "AUTO-SELECT CONTROLLERS" };
         EntryColor ec17{ 7 };
    EntryEnd ee17{};

    Entry eb18{ (((320) -2 * (144) -6) / 2 -8), (((((((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10) + 11)))) +2, 144, 11 };
         EntryTextNative et18{ 0, "ENABLE MENU CONTROLLERS" };
         EntryColor ec18{ 7 };
    EntryEnd ee18{};

    Entry eb19{ (((320) -2 * (144) -6) / 2 -8), (((((((((((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10) + 11)))) +2 + 11)))) +2, 144, 11 };
         EntryTextNative et19{ 0, "SELECT MATCH CONTROLS" };
         EntryColor ec19{ 7 };
    EntryEnd ee19{};

    Entry eb20{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10)), 144, 11 };
         EntryBoolOption ebo20{ getAutoConnectJoypads, setAutoConnectJoypads, "auto connect joypads" };
         EntryColor ec20{ 13 };
         EntryNextPositions ep20{ 255, 255, 15, 21 };
    EntryEnd ee20{};

    Entry eb21{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10) + 11)))) +2), 144, 11 };
         EntryBoolOption ebo21{ getEnableMenuControllers, setEnableMenuControllers, "menu controllers" };
         EntryColor ec21{ 13 };
         EntryNextPositions ep21{ 255, 255, 20, 22 };
    EntryEnd ee21{};

    Entry eb22{ ((((320) -2 * (144) -6) / 2 -8) +(144) +6), ((((((((((((((((((30 + (11) +1)))) +2) +(4 * ((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -(((((((((((30 + (11) +1)))) +2 + (11) +1)))) +2) -(((((30 + (11) +1)))) +2)) -((11) +1))) +(10)) + (11) +1))) +(10) + 11)))) +2 + 11)))) +2), 144, 11 };
         EntryBoolOption ebo22{ getShowSelectMatchControlsMenu, setShowSelectMatchControlsMenu, "select match controls menu" };
         EntryColor ec22{ 13 };
         EntryNextPositions ep22{ 255, 255, 21, 23 };
    EntryEnd ee22{};

    Entry eb23{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et23{ 0, SwosVM::Offsets::aExit };
         EntryColor ec23{ 4 };
         EntryNextPositions ep23{ 255, 255, 22, 255 };
         EntryOnSelectFunctionNative eosf23{ exitControlsMenu };
    EntryEnd ee23{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const controlOptionsMenu
#endif
;
namespace ControlOptionsMenu {
    enum Entries {
        header = 0,
        pl1Control0 = 2,
        pl1Control1 = 3,
        pl1Control2 = 4,
        pl1Control3 = 5,
        pl1ScrollUp = 6,
        pl1ScrollDown = 7,
        pl2Control0 = 9,
        pl2Control1 = 10,
        pl2Control2 = 11,
        pl2Control3 = 12,
        pl2ScrollUp = 13,
        pl2ScrollDown = 14,
        pl2RedefineControls = 15,
        pl1RedefineControls = 16,
        autoConnectJoypadsLabel = 17,
        enableMenuControllersLabel = 18,
        selectMatchControlsLabel = 19,
        autoConnectJoypads = 20,
        enableMenuControllers = 21,
        selectMatchControls = 22,
        exit = 23,
        pl1FirstControl = 2,
        pl1LastControl = 5,
        pl2FirstControl = 9,
        pl2LastControl = 12,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& pl1Control0Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& pl1Control1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& pl1Control2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& pl1Control3Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& pl1ScrollUpEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& pl1ScrollDownEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& pl2Control0Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& pl2Control1Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& pl2Control2Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& pl2Control3Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& pl2ScrollUpEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& pl2ScrollDownEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& pl2RedefineControlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& pl1RedefineControlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& autoConnectJoypadsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& enableMenuControllersLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& selectMatchControlsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& autoConnectJoypadsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& enableMenuControllersEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& selectMatchControlsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& pl1FirstControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& pl1LastControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& pl2FirstControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& pl2LastControlEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));

    constexpr int kControlButtonWidth = 144;
    constexpr int kNumControlEntries = 4;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void controlOptionsMenuOnDraw() {}
static void pl2ScrollUpSelected() {}
static void redefinePlayer2Controls() {}
static void pl2SelectControls() {}
static void pl1ScrollDownSelected() {}
static void exitControlsMenu() {}
static void redefinePlayer1Controls() {}
static void pl1ScrollUpSelected() {}
static void pl2ScrollDownSelected() {}
static void controlOptionsMenuInit() {}
static void pl1SelectControls() {}
#endif
