// automatically generated from videoOptions.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void enterZoom();
static void setTouchTrailsEntryText();
static void decreaseZoom();
static void videoOptionsMenuOnInit();
static void zoomBeforeDraw();
static void openWindowModeMenu();
static void changeVirtualJoypadTransparentButtons();
static void setTransparentButtonsEntryText();
static void changeShowTouchTrails();
static void increaseZoom();

#pragma pack(push, 1)
struct SwosMenu_videoOptionsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, videoOptionsMenuOnInit, videoOptionsMenuOnInit, 0, 0, 13, { true, true, false, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "VIDEO OPTIONS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    TemplateEntry te00{};

    Entry eb30001{ 0, 0, 1, 1 };
         EntryColor ec30001{ 7 };
    EntryEnd ee30001{};

    Entry eb01{ 10, ((60) -(11)), 140, 11 };
         EntryTextNative et01{ 0, "ZOOM FACTOR" };
    EntryEnd ee01{};

    Entry eb02{ 10, ((((((60) -(11)) + 11)))) +(1), 140, 11 };
         EntryTextNative et02{ 0, "LINEAR FILTERING" };
    EntryEnd ee02{};

    Entry eb03{ 10, ((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et03{ 0, "FLASH MENU CURSOR" };
    EntryEnd ee03{};

    Entry eb04{ 10, ((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et04{ 0, "MENU GRADIENT BACKGROUND" };
    EntryEnd ee04{};

    Entry eb05{ 10, ((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et05{ 0, "SHOW FPS INDICATOR" };
    EntryEnd ee05{};

    Entry eb06{ 10, ((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et06{ 0, "CLEAR SCREEN" };
    EntryEnd ee06{};

    Entry eb07{ 10, ((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et07{ 0, "SHOW TOUCH TRAILS" };
    EntryEnd ee07{};

    Entry eb08{ 10, ((((((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryTextNative et08{ 0, "BLEND VIRTUAL JOYPAD" };
    EntryEnd ee08{};

    TemplateEntry te01{};

    Entry eb30010{ 0, 0, 1, 1 };
         EntryColor ec30010{ 13 };
    EntryEnd ee30010{};

    Entry eb09{ 154, ((60) -(11)), 16, 11 };
         EntryTextNative et09{ 0, "-" };
         EntryColor ec09{ 3 };
         EntryNextPositions ep09{ 255, 10, 255, 12 };
         EntryOnSelectFunctionNative eosf09{ decreaseZoom };
    EntryEnd ee09{};

    Entry eb10{ (((154 + 16))) +1, ((60) -(11)), ((140) -2 * (16) -2), 11 };
         EntryText et10{ 0, -1 };
         EntryNextPositions ep10{ 9, 11, 255, 12 };
         EntryOnSelectFunctionNative eosf10{ enterZoom };
         EntryBeforeDrawFunctionNative ebdf10{ zoomBeforeDraw };
    EntryEnd ee10{};

    Entry eb11{ ((((((154 + 16))) +1 + ((140) -2 * (16) -2)))) +1, ((60) -(11)), 16, 11 };
         EntryTextNative et11{ 0, "+" };
         EntryColor ec11{ 3 };
         EntryNextPositions ep11{ 10, 255, 255, 12 };
         EntryOnSelectFunctionNative eosf11{ increaseZoom };
    EntryEnd ee11{};

    Entry eb12{ 154, ((((((60) -(11)) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo12{ getLinearFiltering, setLinearFiltering, "linear filtering" };
         EntryNextPositions ep12{ 255, 255, 10, 13 };
    EntryEnd ee12{};

    Entry eb13{ 154, ((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo13{ cursorFlashingEnabled, setFlashMenuCursor, "cursor flashing" };
         EntryNextPositions ep13{ 255, 255, 12, 14 };
    EntryEnd ee13{};

    Entry eb14{ 154, ((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo14{ menuGradientFillEnabled, enableMenuGradientFill, "menu background gradient" };
         EntryNextPositions ep14{ 255, 255, 13, 15 };
    EntryEnd ee14{};

    Entry eb15{ 154, ((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo15{ getShowFps, setShowFps, "FPS indicator" };
         EntryNextPositions ep15{ 255, 255, 14, 16 };
    EntryEnd ee15{};

    Entry eb16{ 154, ((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryBoolOption ebo16{ getClearScreen, setClearScreen, "clear screen" };
         EntryNextPositions ep16{ 255, 255, 15, 17 };
    EntryEnd ee16{};

    Entry eb17{ 154, ((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryText et17{ 0, -1 };
         EntryNextPositions ep17{ 255, 255, 16, 18 };
         EntryOnSelectFunctionNative eosf17{ changeShowTouchTrails };
         EntryBeforeDrawFunctionNative ebdf17{ setTouchTrailsEntryText };
    EntryEnd ee17{};

    Entry eb18{ 154, ((((((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1), 140, 11 };
         EntryText et18{ 0, -1 };
         EntryNextPositions ep18{ 255, 255, 17, 19 };
         EntryOnSelectFunctionNative eosf18{ changeVirtualJoypadTransparentButtons };
         EntryBeforeDrawFunctionNative ebdf18{ setTransparentButtonsEntryText };
    EntryEnd ee18{};

    Entry eb19{ ((320) -(140)) / 2 -8, (((((((((((((((((((((((((((((((((60) -(11)) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11)))) +(1) + 11))) +3 * 1, 140, 11 };
         EntryTextNative et19{ 0, "WINDOW MODE" };
         EntryColor ec19{ 14 };
         EntryNextPositions ep19{ 255, 255, 18, 20 };
         EntryOnSelectFunctionNative eosf19{ openWindowModeMenu };
    EntryEnd ee19{};

    Entry eb20{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et20{ 0, SwosVM::Offsets::aExit };
         EntryColor ec20{ 4 };
         EntryNextPositions ep20{ 255, 255, 19, 255 };
         EntryOnSelectFunctionNative eosf20{ SetExitMenuFlag };
    EntryEnd ee20{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const videoOptionsMenu
#endif
;
namespace VideoOptionsMenu {
    enum Entries {
        header = 0,
        zoomLabel = 1,
        linearFilteringLabel = 2,
        flashMenuCursorLabel = 3,
        fancyItemBackgroundLabel = 4,
        showFpsLabel = 5,
        clearScreenLabel = 6,
        showTouchTrailsLabel = 7,
        transparentVirtualJoypadButtonsLabel = 8,
        decreaseZoomButton = 9,
        zoom = 10,
        increaseZoomButton = 11,
        linearFiltering = 12,
        flashCursor = 13,
        fancyItemBackground = 14,
        showFps = 15,
        clearScreen = 16,
        showTouchTrails = 17,
        transparentVirtualJoypadButtons = 18,
        showWindowMode = 19,
        exit = 20,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& zoomLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& linearFilteringLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& flashMenuCursorLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& fancyItemBackgroundLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& showFpsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& clearScreenLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& showTouchTrailsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& transparentVirtualJoypadButtonsLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& decreaseZoomButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& zoomEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& increaseZoomButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& linearFilteringEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& flashCursorEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& fancyItemBackgroundEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& showFpsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& clearScreenEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& showTouchTrailsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& transparentVirtualJoypadButtonsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& showWindowModeEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void enterZoom() {}
static void setTouchTrailsEntryText() {}
static void decreaseZoom() {}
static void videoOptionsMenuOnInit() {}
static void zoomBeforeDraw() {}
static void openWindowModeMenu() {}
static void changeVirtualJoypadTransparentButtons() {}
static void setTransparentButtonsEntryText() {}
static void changeShowTouchTrails() {}
static void increaseZoom() {}
#endif
