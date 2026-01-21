// automatically generated from joypadConfig.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void joypadConfigMenuOnReturn();
static void testJoypad();
static void configureSelected();
static void decreaseElementIndex();
static void inputCurrentElementIndex();
static void increaseElementIndex();
static void joypadConfigMenuOnInit();
static void resetConfigToDefault();
static void joypadQuickConfig();
static void joypadConfigMenuOnDraw();

#pragma pack(push, 1)
struct SwosMenu_joypadConfigMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, joypadConfigMenuOnInit, joypadConfigMenuOnReturn, joypadConfigMenuOnDraw, 0, 35, { true, true, true, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "CONTROLLER SETUP" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    Entry eb01{ 30, 30, 64, 10 };
         EntryTextNative et01{ 32768, "NAME:" };
    EntryEnd ee01{};

    Entry eb02{ 30, (((30))) +(10), 64, 10 };
         EntryTextNative et02{ 32768, "GUID:" };
    EntryEnd ee02{};

    Entry eb03{ 30, ((((((30))) +(10)))) +(10), 64, 10 };
         EntryTextNative et03{ 32768, "ID:" };
    EntryEnd ee03{};

    Entry eb04{ 30, (((((((((30))) +(10)))) +(10)))) +(10), 64, 10 };
         EntryTextNative et04{ 32768, "POWER:" };
    EntryEnd ee04{};

    Entry eb05{ 30, ((((((((((((30))) +(10)))) +(10)))) +(10)))) +(10), 64, 10 };
         EntryTextNative et05{ 32768, "CONFIG:" };
    EntryEnd ee05{};

    TemplateEntry te00{};

    Entry eb30006{ 0, 0, 1, 1 };
         EntryText et30006{ (15) | (32768), -1 };
    EntryEnd ee30006{};

    Entry eb06{ ((30) +(64)), 30, 260, 10 };
    EntryEnd ee06{};

    Entry eb07{ ((30) +(64)), (((30))) +(10), 260, 10 };
    EntryEnd ee07{};

    Entry eb08{ ((30) +(64)), ((((((30))) +(10)))) +(10), 260, 10 };
    EntryEnd ee08{};

    Entry eb09{ ((30) +(64)), (((((((((30))) +(10)))) +(10)))) +(10), 260, 10 };
    EntryEnd ee09{};

    Entry eb10{ ((30) +(64)), ((((((((((((30))) +(10)))) +(10)))) +(10)))) +(10), 260, 10 };
    EntryEnd ee10{};

    ResetTemplateEntry rte00{};

    Entry eb11{ (((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8), 84, 260, 10 };
         EntryTextNative et11{ 32768, "HATS:" };
    EntryEnd ee11{};

    Entry eb12{ (((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8), ((((84 + 10)))) +2, 260, 10 };
         EntryTextNative et12{ 32768, "BUTTONS:" };
    EntryEnd ee12{};

    Entry eb13{ (((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8), ((((((((84 + 10)))) +2 + 10)))) +2, 260, 10 };
         EntryTextNative et13{ 32768, "AXES:" };
    EntryEnd ee13{};

    Entry eb14{ (((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, 260, 10 };
         EntryTextNative et14{ 32768, "TRACKBALLS:" };
    EntryEnd ee14{};

    TemplateEntry te01{};

    Entry eb30017{ 0, 0, 1, 1 };
         EntryText et30017{ 0, SwosVM::Offsets::aMinus };
         EntryColor ec30017{ 13 };
         EntryOnSelectFunctionNative eosf30017{ decreaseElementIndex };
    EntryEnd ee30017{};

    Entry eb15{ ((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2), 84, 10, 10 };
         EntryNextPositions ep15{ 255, 19, 255, 16 };
    EntryEnd ee15{};

    Entry eb16{ ((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2), ((((84 + 10)))) +2, 10, 10 };
         EntryNextPositions ep16{ 255, 20, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ ((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2), ((((((((84 + 10)))) +2 + 10)))) +2, 10, 10 };
         EntryNextPositions ep17{ 255, 21, 16, 18 };
    EntryEnd ee17{};

    Entry eb18{ ((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, 10, 10 };
         EntryNextPositions ep18{ 255, 22, 17, 39 };
    EntryEnd ee18{};

    ResetTemplateEntry rte01{};

    TemplateEntry te02{};

    Entry eb30023{ 0, 0, 1, 1 };
         EntryNumber en30023{ 0, 0 };
         EntryColor ec30023{ 13 };
         EntryOnSelectFunctionNative eosf30023{ inputCurrentElementIndex };
    EntryEnd ee30023{};

    Entry eb19{ (((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1), 84, 30, 10 };
         EntryNextPositions ep19{ 15, 23, 255, 20 };
    EntryEnd ee19{};

    Entry eb20{ (((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1), ((((84 + 10)))) +2, 30, 10 };
         EntryNextPositions ep20{ 16, 24, 19, 21 };
    EntryEnd ee20{};

    Entry eb21{ (((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1), ((((((((84 + 10)))) +2 + 10)))) +2, 30, 10 };
         EntryNextPositions ep21{ 17, 25, 20, 22 };
    EntryEnd ee21{};

    Entry eb22{ (((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, 30, 10 };
         EntryNextPositions ep22{ 18, 26, 21, 39 };
    EntryEnd ee22{};

    TemplateEntry te03{};

    Entry eb30028{ 0, 0, 1, 1 };
         EntryText et30028{ 0, SwosVM::Offsets::aPlus };
         EntryColor ec30028{ 13 };
         EntryOnSelectFunctionNative eosf30028{ increaseElementIndex };
    EntryEnd ee30028{};

    Entry eb23{ ((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1), 84, 10, 10 };
         EntryNextPositions ep23{ 19, 35, 255, 24 };
    EntryEnd ee23{};

    Entry eb24{ ((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1), ((((84 + 10)))) +2, 10, 10 };
         EntryNextPositions ep24{ 20, 36, 23, 25 };
    EntryEnd ee24{};

    Entry eb25{ ((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1), ((((((((84 + 10)))) +2 + 10)))) +2, 10, 10 };
         EntryNextPositions ep25{ 21, 37, 24, 26 };
    EntryEnd ee25{};

    Entry eb26{ ((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, 10, 10 };
         EntryNextPositions ep26{ 22, 38, 25, 39 };
    EntryEnd ee26{};

    ResetTemplateEntry rte02{};

    TemplateEntry te04{};

    Entry eb30034{ 0, 0, 1, 1 };
         EntryTextNative et30034{ 0, "OF" };
    EntryEnd ee30034{};

    Entry eb27{ (((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2), 84, 18, 10 };
    EntryEnd ee27{};

    Entry eb28{ (((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2), ((((84 + 10)))) +2, 18, 10 };
    EntryEnd ee28{};

    Entry eb29{ (((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2), ((((((((84 + 10)))) +2 + 10)))) +2, 18, 10 };
    EntryEnd ee29{};

    Entry eb30{ (((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, 18, 10 };
    EntryEnd ee30{};

    TemplateEntry te05{};

    Entry eb30039{ 0, 0, 1, 1 };
         EntryNumber en30039{ 0, 0 };
    EntryEnd ee30039{};

    Entry eb31{ ((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1), 84, ((18) +2), 10 };
    EntryEnd ee31{};

    Entry eb32{ ((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1), ((((84 + 10)))) +2, ((18) +2), 10 };
    EntryEnd ee32{};

    Entry eb33{ ((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1), ((((((((84 + 10)))) +2 + 10)))) +2, ((18) +2), 10 };
    EntryEnd ee33{};

    Entry eb34{ ((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, ((18) +2), 10 };
    EntryEnd ee34{};

    TemplateEntry te06{};

    Entry eb30044{ 0, 0, 1, 1 };
         EntryTextNative et30044{ 0, "CONFIGURE" };
         EntryColor ec30044{ 3 };
         EntryOnSelectFunctionNative eosf30044{ configureSelected };
    EntryEnd ee30044{};

    Entry eb35{ (((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1) +((18) +2) +2), 84, ((50) +6), 10 };
         EntryNextPositions ep35{ 23, 255, 255, 36 };
    EntryEnd ee35{};

    Entry eb36{ (((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1) +((18) +2) +2), ((((84 + 10)))) +2, ((50) +6), 10 };
         EntryNextPositions ep36{ 24, 255, 35, 37 };
    EntryEnd ee36{};

    Entry eb37{ (((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1) +((18) +2) +2), ((((((((84 + 10)))) +2 + 10)))) +2, ((50) +6), 10 };
         EntryNextPositions ep37{ 25, 255, 36, 38 };
    EntryEnd ee37{};

    Entry eb38{ (((((((((320) -((66) +2 +2 * (10) +(30) +4 +(18) +1 +((18) +2) +2 +((50) +6))) / 2 -8) +(66) +2) +(10) +1) +(30) +1) +(10) +2) +(18) +1) +((18) +2) +2), ((((((((((((84 + 10)))) +2 + 10)))) +2 + 10)))) +2, ((50) +6), 10 };
         EntryNextPositions ep38{ 26, 255, 37, 39 };
    EntryEnd ee38{};

    ResetTemplateEntry rte03{};

    Entry eb39{ (((320) -(125)) / 2 -8), (((200) -(15)) -3 * ((15) +2)), 125, 15 };
         EntryTextNative et39{ 0, "QUICK CONFIG" };
         EntryColor ec39{ 3 };
         EntryNextPositions ep39{ 255, 255, 22, 40 };
         EntryOnSelectFunctionNative eosf39{ joypadQuickConfig };
    EntryEnd ee39{};

    Entry eb40{ (((320) -(125)) / 2 -8), (((((((200) -(15)) -3 * ((15) +2)) + 15)))) +2, 125, 15 };
         EntryTextNative et40{ 0, "RESET TO DEFAULT" };
         EntryColor ec40{ 3 };
         EntryNextPositions ep40{ 255, 255, 39, 41 };
         EntryOnSelectFunctionNative eosf40{ resetConfigToDefault };
    EntryEnd ee40{};

    Entry eb41{ (((320) -(125)) / 2 -8), (((((((((((200) -(15)) -3 * ((15) +2)) + 15)))) +2 + 15)))) +2, 125, 15 };
         EntryTextNative et41{ 0, "TEST" };
         EntryColor ec41{ 3 };
         EntryNextPositions ep41{ 255, 255, 40, 42 };
         EntryOnSelectFunctionNative eosf41{ testJoypad };
    EntryEnd ee41{};

    Entry eb42{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et42{ 0, SwosVM::Offsets::aExit };
         EntryColor ec42{ 4 };
         EntryNextPositions ep42{ 255, 255, 41, 255 };
         EntryOnSelectFunctionNative eosf42{ SetExitMenuFlag };
    EntryEnd ee42{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const joypadConfigMenu
#endif
;
namespace JoypadConfigMenu {
    enum Entries {
        header = 0,
        name = 6,
        guid = 7,
        id = 8,
        powerLevel = 9,
        configuration = 10,
        hatsLabel = 11,
        buttonsLabel = 12,
        axesLabel = 13,
        ballsLabel = 14,
        decreaseHats = 15,
        decreaseButtons = 16,
        decreaseAxes = 17,
        decreaseBalls = 18,
        currentHat = 19,
        currentButton = 20,
        currentAxis = 21,
        currentBall = 22,
        increaseHats = 23,
        increaseButtons = 24,
        increaseAxes = 25,
        increaseBalls = 26,
        numHats = 31,
        numButtons = 32,
        numAxes = 33,
        numBalls = 34,
        configureHats = 35,
        configureButtons = 36,
        configureAxes = 37,
        configureBalls = 38,
        quickConfig = 39,
        reset = 40,
        test = 41,
        exit = 42,
        lastInfoEntry = 10,
        lastElementEntry = 38,
        topmostButton = 39,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& nameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& guidEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& idEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& powerLevelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& configurationEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& hatsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& buttonsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& axesLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& ballsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& decreaseHatsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& decreaseButtonsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& decreaseAxesEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& decreaseBallsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& currentHatEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& currentButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& currentAxisEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& currentBallEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& increaseHatsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& increaseButtonsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& increaseAxesEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& increaseBallsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& numHatsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 31 * sizeof(MenuEntry));
    static auto& numButtonsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 32 * sizeof(MenuEntry));
    static auto& numAxesEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 33 * sizeof(MenuEntry));
    static auto& numBallsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 34 * sizeof(MenuEntry));
    static auto& configureHatsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 35 * sizeof(MenuEntry));
    static auto& configureButtonsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 36 * sizeof(MenuEntry));
    static auto& configureAxesEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 37 * sizeof(MenuEntry));
    static auto& configureBallsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 38 * sizeof(MenuEntry));
    static auto& quickConfigEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 39 * sizeof(MenuEntry));
    static auto& resetEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 40 * sizeof(MenuEntry));
    static auto& testEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 41 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 42 * sizeof(MenuEntry));
    static auto& lastInfoEntryEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& lastElementEntryEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 38 * sizeof(MenuEntry));
    static auto& topmostButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 39 * sizeof(MenuEntry));

    constexpr int kInfoFieldWidth = 260;
    constexpr int kResetTextY = 20;
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void joypadConfigMenuOnReturn() {}
static void testJoypad() {}
static void configureSelected() {}
static void decreaseElementIndex() {}
static void inputCurrentElementIndex() {}
static void increaseElementIndex() {}
static void joypadConfigMenuOnInit() {}
static void resetConfigToDefault() {}
static void joypadQuickConfig() {}
static void joypadConfigMenuOnDraw() {}
#endif
