// automatically generated from configureTrackball.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void configureTrackballMenuOnRestore();
static void selectEvents();
static void goToPreviousBall();
static void configureTrackballMenuOnInit();
static void configureTrackballMenuOnDraw();
static void goToNextBall();

#pragma pack(push, 1)
struct SwosMenu_configureTrackballMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, configureTrackballMenuOnInit, configureTrackballMenuOnRestore, configureTrackballMenuOnDraw, 0, 6, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "CONFIGURE TRACKBALL" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ -8, ((((200) -4 * ((14) +4)) / 2 +(10)) -(32)), 320, 6 };
         EntryText et01{ 0, -1 };
    EntryEnd ee01{};

    Entry eb02{ (((((320) -3 * (40) -2 * 2) / 2 -8)) -((14) +2) -4), (((200) -4 * ((14) +4)) / 2 +(10)), ((14) +2), 14 };
         EntryTextNative et02{ 16384, "X+:" };
    EntryEnd ee02{};

    Entry eb03{ (((((((320) -3 * (40) -2 * 2) / 2 -8)) -((14) +2) -4))), (((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4), ((14) +2), 14 };
         EntryTextNative et03{ 16384, "X-:" };
    EntryEnd ee03{};

    Entry eb04{ (((((((((320) -3 * (40) -2 * 2) / 2 -8)) -((14) +2) -4))))), (((((((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4) + 14)))) +(4), ((14) +2), 14 };
         EntryTextNative et04{ 16384, "Y+:" };
    EntryEnd ee04{};

    Entry eb05{ (((((((((((320) -3 * (40) -2 * 2) / 2 -8)) -((14) +2) -4))))))), (((((((((((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4) + 14)))) +(4) + 14)))) +(4), ((14) +2), 14 };
         EntryTextNative et05{ 16384, "Y-:" };
    EntryEnd ee05{};

    TemplateEntry te00{};

    Entry eb30006{ 0, 0, 1, 1 };
         EntryText et30006{ 0, -1 };
         EntryColor ec30006{ 13 };
         EntryOnSelectFunctionNative eosf30006{ selectEvents };
    EntryEnd ee30006{};

    Entry eb06{ ((((320) -3 * (40) -2 * 2) / 2 -8)), (((200) -4 * ((14) +4)) / 2 +(10)), 125, 14 };
         EntryNextPositions ep06{ 10, 12, 255, 7 };
    EntryEnd ee06{};

    Entry eb07{ ((((((320) -3 * (40) -2 * 2) / 2 -8)))), (((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4), 125, 14 };
         EntryNextPositions ep07{ 10, 12, 6, 8 };
    EntryEnd ee07{};

    Entry eb08{ ((((((((320) -3 * (40) -2 * 2) / 2 -8)))))), (((((((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4) + 14)))) +(4), 125, 14 };
         EntryNextPositions ep08{ 10, 12, 7, 9 };
    EntryEnd ee08{};

    Entry eb09{ ((((((((((320) -3 * (40) -2 * 2) / 2 -8)))))))), (((((((((((((((200) -4 * ((14) +4)) / 2 +(10)) + 14)))) +(4) + 14)))) +(4) + 14)))) +(4), 125, 14 };
         EntryNextPositions ep09{ 10, 12, 8, 10 };
    EntryEnd ee09{};

    ResetTemplateEntry rte00{};

    Entry eb10{ (((320) -3 * (40) -2 * 2) / 2 -8), ((((200) -4 * ((14) +4)) / 2 +(10)) +4 * ((14) +4) +(16)), 40, 15 };
         EntryForegroundSprite efs10{ 179 };
         EntryColor ec10{ 4 };
         EntryNextPositions ep10{ 255, 11, 9, 255 };
         EntryOnSelectFunctionNative eosf10{ goToPreviousBall };
    EntryEnd ee10{};

    Entry eb11{ (((((((320) -3 * (40) -2 * 2) / 2 -8) + 40)))) +(2), ((((200) -4 * ((14) +4)) / 2 +(10)) +4 * ((14) +4) +(16)), 40, 15 };
         EntryText et11{ 0, SwosVM::Offsets::aExit };
         EntryColor ec11{ 4 };
         EntryNextPositions ep11{ 10, 12, 9, 255 };
         EntryOnSelectFunctionNative eosf11{ SetExitMenuFlag };
    EntryEnd ee11{};

    Entry eb12{ (((((((((((320) -3 * (40) -2 * 2) / 2 -8) + 40)))) +(2) + 40)))) +(2), ((((200) -4 * ((14) +4)) / 2 +(10)) +4 * ((14) +4) +(16)), 40, 15 };
         EntryForegroundSprite efs12{ 175 };
         EntryColor ec12{ 4 };
         EntryNextPositions ep12{ 11, 255, 9, 255 };
         EntryOnSelectFunctionNative eosf12{ goToNextBall };
    EntryEnd ee12{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const configureTrackballMenu
#endif
;
namespace ConfigureTrackballMenu {
    enum Entries {
        header = 0,
        trackballLabel = 1,
        xPosLabel = 2,
        xNegLabel = 3,
        yPosLabel = 4,
        yNegLabel = 5,
        xPosEvent = 6,
        xNegEvent = 7,
        yPosEvent = 8,
        yNegEvent = 9,
        prevBall = 10,
        exit = 11,
        nextBall = 12,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& trackballLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& xPosLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& xNegLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& yPosLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& yNegLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& xPosEventEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& xNegEventEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& yPosEventEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& yNegEventEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& prevBallEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& nextBallEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void configureTrackballMenuOnRestore() {}
static void selectEvents() {}
static void goToPreviousBall() {}
static void configureTrackballMenuOnInit() {}
static void configureTrackballMenuOnDraw() {}
static void goToNextBall() {}
#endif
