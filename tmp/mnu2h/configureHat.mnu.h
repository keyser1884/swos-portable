// automatically generated from configureHat.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void configureHatMenuOnRestore();
static void toggleInverted();
static void inputCurrentBinding();
static void pickEvents();
static void createBinding();
static void showPreviousBinding();
static void configureHatMenuOnInit();
static void deleteCurrentBinding();
static void toggleEvent();
static void showNextBinding();
static void configureHatMenuOnDraw();

#pragma pack(push, 1)
struct SwosMenu_configureHatMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, configureHatMenuOnInit, configureHatMenuOnRestore, configureHatMenuOnDraw, 0, 18, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryText et00{ 0, -1 };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ -8, 20, 320, 10 };
         EntryText et01{ 0, -1 };
    EntryEnd ee01{};

    Entry eb02{ ((320) -2 * (20) -(20) -2 * 2) / 2 -8, ((20) +(10)), 20, 10 };
         EntryForegroundSprite efs02{ 179 };
         EntryColor ec02{ 3 };
         EntryNextPositions ep02{ 255, 3, 255, 6 };
         EntryOnSelectFunctionNative eosf02{ showPreviousBinding };
    EntryEnd ee02{};

    Entry eb03{ (((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20))) +2, ((((20) +(10)))), 20, 10 };
         EntryNumber en03{ 0, 0 };
         EntryColor ec03{ 13 };
         EntryNextPositions ep03{ 2, 4, 255, 6 };
         EntryOnSelectFunctionNative eosf03{ inputCurrentBinding };
    EntryEnd ee03{};

    Entry eb04{ ((((((((320) -2 * (20) -(20) -2 * 2) / 2 -8 + 20))) +2 + 20))) +2, ((((((20) +(10)))))), 20, 10 };
         EntryForegroundSprite efs04{ 175 };
         EntryColor ec04{ 3 };
         EntryNextPositions ep04{ 3, 255, 255, 6 };
         EntryOnSelectFunctionNative eosf04{ showNextBinding };
    EntryEnd ee04{};

    Entry eb05{ -8, ((56) -(10)), 320, 10 };
         EntryTextNative et05{ 0, "HAT MASK:" };
    EntryEnd ee05{};

    TemplateEntry te00{};

    Entry eb30006{ 0, 0, 1, 1 };
         EntryColor ec30006{ 7 };
    EntryEnd ee30006{};

    Entry eb06{ (((320) -(140) -(26) -2) / 2 -8), 56, 140, 10 };
         EntryTextNative et06{ 0, "LEFT" };
         EntryNextPositions ep06{ 255, 13, 2, 7 };
         EntryOnSelectFunctionNative eosf06{ toggleEvent };
    EntryEnd ee06{};

    Entry eb07{ (((320) -(140) -(26) -2) / 2 -8), ((((56 + 10)))) +(2), 140, 10 };
         EntryTextNative et07{ 0, "RIGHT" };
         EntryNextPositions ep07{ 255, 14, 6, 8 };
         EntryOnSelectFunctionNative eosf07{ toggleEvent };
    EntryEnd ee07{};

    Entry eb08{ (((320) -(140) -(26) -2) / 2 -8), ((((((((56 + 10)))) +(2) + 10)))) +(2), 140, 10 };
         EntryTextNative et08{ 0, "UP" };
         EntryNextPositions ep08{ 255, 15, 7, 9 };
         EntryOnSelectFunctionNative eosf08{ toggleEvent };
    EntryEnd ee08{};

    Entry eb09{ (((320) -(140) -(26) -2) / 2 -8), ((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2), 140, 10 };
         EntryTextNative et09{ 0, "DOWN" };
         EntryNextPositions ep09{ 255, 16, 8, 10 };
         EntryOnSelectFunctionNative eosf09{ toggleEvent };
    EntryEnd ee09{};

    Entry eb10{ (((320) -(140) -(26) -2) / 2 -8), ((((((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2), 140, 10 };
         EntryTextNative et10{ 0, "INVERTED" };
         EntryNextPositions ep10{ 255, 17, 9, 12 };
         EntryOnSelectFunctionNative eosf10{ toggleInverted };
    EntryEnd ee10{};

    ResetTemplateEntry rte00{};

    Entry eb11{ -8, ((((((((((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2), 320, 10 };
         EntryTextNative et11{ 0, "EVENTS:" };
    EntryEnd ee11{};

    Entry eb12{ (((320) -(140) -(26) -2) / 2 -8), ((((((((((((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2))) +(10), ((140) +(26) +2), 10 };
         EntryText et12{ 0, -1 };
         EntryColor ec12{ 13 };
         EntryNextPositions ep12{ 255, 255, 10, 18 };
         EntryOnSelectFunctionNative eosf12{ pickEvents };
    EntryEnd ee12{};

    TemplateEntry te01{};

    Entry eb30015{ 0, 0, 1, 1 };
         EntryText et30015{ 0, -1 };
         EntryColor ec30015{ 7 };
    EntryEnd ee30015{};

    Entry eb13{ ((((320) -(140) -(26) -2) / 2 -8) +(140) +2), 56, 26, 10 };
         EntryNextPositions ep13{ 6, 255, 4, 14 };
         EntryOnSelectFunctionNative eosf13{ toggleEvent };
    EntryEnd ee13{};

    Entry eb14{ ((((320) -(140) -(26) -2) / 2 -8) +(140) +2), ((((56 + 10)))) +(2), 26, 10 };
         EntryNextPositions ep14{ 7, 255, 13, 15 };
         EntryOnSelectFunctionNative eosf14{ toggleEvent };
    EntryEnd ee14{};

    Entry eb15{ ((((320) -(140) -(26) -2) / 2 -8) +(140) +2), ((((((((56 + 10)))) +(2) + 10)))) +(2), 26, 10 };
         EntryNextPositions ep15{ 8, 255, 14, 16 };
         EntryOnSelectFunctionNative eosf15{ toggleEvent };
    EntryEnd ee15{};

    Entry eb16{ ((((320) -(140) -(26) -2) / 2 -8) +(140) +2), ((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2), 26, 10 };
         EntryNextPositions ep16{ 9, 255, 15, 17 };
         EntryOnSelectFunctionNative eosf16{ toggleEvent };
    EntryEnd ee16{};

    Entry eb17{ ((((320) -(140) -(26) -2) / 2 -8) +(140) +2), ((((((((((((((((56 + 10)))) +(2) + 10)))) +(2) + 10)))) +(2) + 10)))) +(2), 26, 10 };
         EntryNextPositions ep17{ 10, 255, 16, 12 };
         EntryOnSelectFunctionNative eosf17{ toggleInverted };
    EntryEnd ee17{};

    ResetTemplateEntry rte01{};

    Entry eb18{ (((320) -(125)) / 2 -8), ((200) -(15)) -2 * (15) -4 * 2, 125, 15 };
         EntryTextNative et18{ 0, "CREATE NEW BINDING" };
         EntryColor ec18{ 14 };
         EntryNextPositions ep18{ 255, 255, 12, 19 };
         EntryOnSelectFunctionNative eosf18{ createBinding };
    EntryEnd ee18{};

    Entry eb19{ (((320) -(125)) / 2 -8), ((200) -(15)) -(15) -2 * 2, 125, 15 };
         EntryTextNative et19{ 0, "DELETE BINDING" };
         EntryColor ec19{ 14 };
         EntryNextPositions ep19{ 255, 255, 18, 20 };
         EntryOnSelectFunctionNative eosf19{ deleteCurrentBinding };
    EntryEnd ee19{};

    Entry eb20{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryTextNative et20{ 0, "EXIT" };
         EntryColor ec20{ 4 };
         EntryNextPositions ep20{ 255, 255, 19, 255 };
         EntryOnSelectFunctionNative eosf20{ SetExitMenuFlag };
    EntryEnd ee20{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const configureHatMenu
#endif
;
namespace ConfigureHatMenu {
    enum Entries {
        header = 0,
        bindingLabel = 1,
        prevBinding = 2,
        currentBinding = 3,
        nextBinding = 4,
        leftLabel = 6,
        rightLabel = 7,
        upLabel = 8,
        downLabel = 9,
        invertedLabel = 10,
        eventsLabel = 11,
        events = 12,
        leftStatus = 13,
        rightStatus = 14,
        upStatus = 15,
        downStatus = 16,
        invertedStatus = 17,
        createNewBinding = 18,
        deleteBinding = 19,
        exit = 20,
        firstEventLabel = 6,
        lastEventLabel = 9,
        firstEventStatus = 13,
        lastEventStatus = 16,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& bindingLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& prevBindingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& currentBindingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& nextBindingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& leftLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& rightLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& upLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& downLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& invertedLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& eventsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& eventsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& leftStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& rightStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& upStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& downStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& invertedStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& createNewBindingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& deleteBindingEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& firstEventLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& lastEventLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& firstEventStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& lastEventStatusEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));

    constexpr int kRowGap = 2;
    constexpr int kOnColor = 14;
    constexpr int kOffColor = 7;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void configureHatMenuOnRestore() {}
static void toggleInverted() {}
static void inputCurrentBinding() {}
static void pickEvents() {}
static void createBinding() {}
static void showPreviousBinding() {}
static void configureHatMenuOnInit() {}
static void deleteCurrentBinding() {}
static void toggleEvent() {}
static void showNextBinding() {}
static void configureHatMenuOnDraw() {}
#endif
