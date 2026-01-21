// automatically generated from selectGameControlEvents.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void selectPreviousEvent();
static void selectGameControlEventsOnInit();
static void onOk();
static void updateWarning();
static void selectGameControlEventsOnDraw();
static void toggleInverted();
static void toggleEvent();
static void selectGameControlEventsOnRestore();
static void toggleEventFromStatus();
static void selectNextEvent();
static void onCancel();

#pragma pack(push, 1)
struct SwosMenu_selectGameControlEvents : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, selectGameControlEventsOnInit, selectGameControlEventsOnRestore, selectGameControlEventsOnDraw, 0, 2, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryText et00{ 0, -1 };
         EntryColor ec00{ (7 | (16)) };
         EntryNextPositions ep00{ 26, 27, 255, 255 };
    EntryEnd ee00{};

    Entry eb01{ -8, 21, 320, 10 };
         EntryText et01{ 0, -1 };
         EntryNextPositions ep01{ 26, 27, 255, 255 };
    EntryEnd ee01{};

    TemplateEntry te00{};

    Entry eb30002{ 0, 0, 1, 1 };
         EntryColor ec30002{ 7 };
         EntryOnSelectFunctionNative eosf30002{ toggleEvent };
    EntryEnd ee30002{};

    Entry eb02{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), 44, 120, 16 };
         EntryTextNative et02{ 0, "UP" };
         EntryNextPositions ep02{ 26, 14, 255, 3 };
    EntryEnd ee02{};

    Entry eb03{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), ((((44 + 16)))) +(2), 120, 16 };
         EntryTextNative et03{ 0, "DOWN" };
         EntryNextPositions ep03{ 26, 15, 2, 4 };
    EntryEnd ee03{};

    Entry eb04{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), ((((((((44 + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et04{ 0, "LEFT" };
         EntryNextPositions ep04{ 26, 16, 3, 5 };
    EntryEnd ee04{};

    Entry eb05{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), ((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et05{ 0, "RIGHT" };
         EntryNextPositions ep05{ 26, 17, 4, 6 };
    EntryEnd ee05{};

    Entry eb06{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), ((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et06{ 0, "KICK" };
         EntryNextPositions ep06{ 26, 18, 5, 7 };
    EntryEnd ee06{};

    Entry eb07{ (((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8), ((((((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et07{ 0, "BENCH" };
         EntryNextPositions ep07{ 26, 19, 6, 26 };
    EntryEnd ee07{};

    Entry eb08{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), 44, 120, 16 };
         EntryTextNative et08{ 0, "PAUSE" };
         EntryNextPositions ep08{ 14, 20, 255, 9 };
    EntryEnd ee08{};

    Entry eb09{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), ((((44 + 16)))) +(2), 120, 16 };
         EntryTextNative et09{ 0, "REPLAY" };
         EntryNextPositions ep09{ 15, 21, 8, 10 };
    EntryEnd ee09{};

    Entry eb10{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), ((((((((44 + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et10{ 0, "SAVE HIGHLIGHT" };
         EntryNextPositions ep10{ 16, 22, 9, 11 };
    EntryEnd ee10{};

    Entry eb11{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), ((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et11{ 0, "ZOOM IN" };
         EntryNextPositions ep11{ 17, 23, 10, 12 };
    EntryEnd ee11{};

    Entry eb12{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), ((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et12{ 0, "ZOOM OUT" };
         EntryNextPositions ep12{ 18, 24, 11, 27 };
    EntryEnd ee12{};

    Entry eb13{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6), ((((((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 120, 16 };
         EntryTextNative et13{ 0, "INVERTED" };
         EntryInvisible ei13{};
         EntryNextPositions ep13{ 19, 25, 12, 30 };
         EntryOnSelectFunctionNative eosf13{ toggleInverted };
    EntryEnd ee13{};

    ResetTemplateEntry rte00{};

    TemplateEntry te01{};

    Entry eb30016{ 0, 0, 1, 1 };
         EntryText et30016{ 0, -1 };
         EntryColor ec30016{ 7 };
         EntryOnSelectFunctionNative eosf30016{ toggleEventFromStatus };
    EntryEnd ee30016{};

    Entry eb14{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), 44, 26, 16 };
         EntryNextPositions ep14{ 2, 8, 255, 15 };
    EntryEnd ee14{};

    Entry eb15{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), ((((44 + 16)))) +(2), 26, 16 };
         EntryNextPositions ep15{ 3, 9, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), ((((((((44 + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep16{ 4, 10, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), ((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep17{ 5, 11, 16, 18 };
    EntryEnd ee17{};

    Entry eb18{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), ((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep18{ 6, 12, 17, 19 };
    EntryEnd ee18{};

    Entry eb19{ ((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +(120) +4), ((((((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep19{ 7, 12, 18, 26 };
    EntryEnd ee19{};

    Entry eb20{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), 44, 26, 16 };
         EntryNextPositions ep20{ 8, 27, 255, 21 };
    EntryEnd ee20{};

    Entry eb21{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), ((((44 + 16)))) +(2), 26, 16 };
         EntryNextPositions ep21{ 9, 27, 20, 22 };
    EntryEnd ee21{};

    Entry eb22{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), ((((((((44 + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep22{ 10, 27, 21, 23 };
    EntryEnd ee22{};

    Entry eb23{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), ((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep23{ 11, 27, 22, 24 };
    EntryEnd ee23{};

    Entry eb24{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), ((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryNextPositions ep24{ 12, 27, 23, 27 };
    EntryEnd ee24{};

    Entry eb25{ (((((320) -2 * ((120) +(26) +4) -4 * 3) / 2 -8) +((120) +(26) +4) +6) +(120) +4), ((((((((((((((((((((44 + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2) + 16)))) +(2), 26, 16 };
         EntryInvisible ei25{};
         EntryNextPositions ep25{ 13, 30, 24, 30 };
         EntryOnSelectFunctionNative eosf25{ toggleInverted };
    EntryEnd ee25{};

    ResetTemplateEntry rte01{};

    Entry eb26{ (((320) -2 * (((120) +(26)) / 2) -4) / 2 -8), (200) -(15) -4, (((120) +(26)) / 2), 15 };
         EntryText et26{ 0, SwosVM::Offsets::aOk };
         EntryColor ec26{ 4 };
         EntryNextPositions ep26{ 255, 27, 7, 255 };
         EntryDownSkip eds26{ 27, 3 };
         EntryOnSelectFunctionNative eosf26{ onOk };
    EntryEnd ee26{};

    Entry eb27{ ((((320) -2 * (((120) +(26)) / 2) -4) / 2 -8) +(((120) +(26)) / 2) +4), (((200) -(15) -4)), (((120) +(26)) / 2), 15 };
         EntryText et27{ 0, SwosVM::Offsets::aCancel };
         EntryColor ec27{ 4 };
         EntryNextPositions ep27{ 26, 255, 13, 255 };
         EntryOnSelectFunctionNative eosf27{ onCancel };
    EntryEnd ee27{};

    Entry eb28{ (((320) -2 * (((120) +(26)) / 2) -4) / 2 -8), (((((200) -(15) -4)))), (((120) +(26) -4) / 3), 15 };
         EntryForegroundSprite efs28{ 179 };
         EntryColor ec28{ 4 };
         EntryNextPositions ep28{ 26, 29, 7, 255 };
         EntryOnSelectFunctionNative eosf28{ selectPreviousEvent };
    EntryEnd ee28{};

    Entry eb29{ ((((((320) -2 * (((120) +(26)) / 2) -4) / 2 -8) + (((120) +(26) -4) / 3)))) +4, (((((((200) -(15) -4)))))), (((120) +(26) -4) / 3) +(((120) +(26) -4) % 3 > 0), 15 };
         EntryText et29{ 0, SwosVM::Offsets::aExit };
         EntryColor ec29{ 4 };
         EntryNextPositions ep29{ 28, 30, 19, 255 };
         EntryOnSelectFunctionNative eosf29{ SetExitMenuFlag };
    EntryEnd ee29{};

    Entry eb30{ (((((((((320) -2 * (((120) +(26)) / 2) -4) / 2 -8) + (((120) +(26) -4) / 3)))) +4 + (((120) +(26) -4) / 3) +(((120) +(26) -4) % 3 > 0)))) +4, (((((((((200) -(15) -4)))))))), (((120) +(26) -4) / 3) +(((120) +(26) -4) % 3 > 1), 15 };
         EntryForegroundSprite efs30{ 175 };
         EntryColor ec30{ 4 };
         EntryNextPositions ep30{ 29, 27, 12, 255 };
         EntryOnSelectFunctionNative eosf30{ selectNextEvent };
    EntryEnd ee30{};

    Entry eb31{ -8, 186, 320, 15 };
         EntryText et31{ 15, -1 };
         EntryInvisible ei31{};
         EntryNextPositions ep31{ 26, 27, 255, 255 };
         EntryBeforeDrawFunctionNative ebdf31{ updateWarning };
    EntryEnd ee31{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const selectGameControlEvents
#endif
;
namespace SelectGameControlEvents {
    enum Entries {
        header = 0,
        eventHeaderLabel = 1,
        upToggle = 2,
        downToggle = 3,
        leftToggle = 4,
        rightToggle = 5,
        kickToggle = 6,
        benchToggle = 7,
        pauseToggle = 8,
        replayToggle = 9,
        saveHighlightToggle = 10,
        zoomInToggle = 11,
        zoomOutToggle = 12,
        invertedToggle = 13,
        upStatus = 14,
        downStatus = 15,
        leftStatus = 16,
        rightStatus = 17,
        kickStatus = 18,
        benchStatus = 19,
        pauseStatus = 20,
        replayStatus = 21,
        saveHighlightStatus = 22,
        zoomInStatus = 23,
        zoomOutStatus = 24,
        invertedStatus = 25,
        ok = 26,
        cancel = 27,
        prev = 28,
        exit = 29,
        next = 30,
        warning = 31,
        firstEventToggle = 2,
        lastEventToggle = 13,
        firstEventStatus = 14,
        lastEventStatus = 25,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& eventHeaderLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& upToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& downToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& leftToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& rightToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& kickToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& benchToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& pauseToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& replayToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& saveHighlightToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& zoomInToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& zoomOutToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& invertedToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& upStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& downStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& leftStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& rightStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& kickStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& benchStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& pauseStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& replayStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& saveHighlightStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& zoomInStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& zoomOutStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& invertedStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& okEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& cancelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 27 * sizeof(MenuEntry));
    static auto& prevEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 28 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 29 * sizeof(MenuEntry));
    static auto& nextEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 30 * sizeof(MenuEntry));
    static auto& warningEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 31 * sizeof(MenuEntry));
    static auto& firstEventToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& lastEventToggleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& firstEventStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& lastEventStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));

    constexpr int kLowerButtonsGap = 10;
    constexpr int kOnColor = 14;
    constexpr int kOffColor = 7;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void selectPreviousEvent() {}
static void selectGameControlEventsOnInit() {}
static void onOk() {}
static void updateWarning() {}
static void selectGameControlEventsOnDraw() {}
static void toggleInverted() {}
static void toggleEvent() {}
static void selectGameControlEventsOnRestore() {}
static void toggleEventFromStatus() {}
static void selectNextEvent() {}
static void onCancel() {}
#endif
