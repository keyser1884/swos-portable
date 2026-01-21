// automatically generated from selectFiles.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void saveFileSelected();
static void scrollDown();
static void selectFilesOnReturn();
static void abortSelectFile();
static void drawDescription();
static void selectFile();
static void selectFilesOnInit();
static void scrollUp();
static void inputFilenameToSave();

#pragma pack(push, 1)
struct SwosMenu_selectFilesMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, selectFilesOnInit, selectFilesOnReturn, 0, 0, 16, { true, true, false, false, }};

    Entry eb00{ 2, 0, 300, 15 };
         EntryText et00{ 0, nullptr };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    TemplateEntry te00{};

    Entry eb30001{ 0, 0, 42, 1 };
         EntryText et30001{ 0, -1 };
         EntryColor ec30001{ 6 };
         EntryOnSelectFunctionNative eosf30001{ selectFile };
         EntryBeforeDrawFunctionNative ebdf30001{ drawDescription };
    EntryEnd ee30001{};

    Entry eb01{ 0, 0, 42, 8 };
         EntryNextPositions ep01{ 255, 16, 50, 2 };
    EntryEnd ee01{};

    Entry eb02{ 0, 0, 42, 8 };
         EntryNextPositions ep02{ 255, 17, 1, 3 };
         EntryLeftSkip els02{ 15, 2 };
    EntryEnd ee02{};

    Entry eb03{ 0, 0, 42, 8 };
         EntryNextPositions ep03{ 255, 18, 2, 4 };
         EntryLeftSkip els03{ 15, 2 };
    EntryEnd ee03{};

    Entry eb04{ 0, 0, 42, 8 };
         EntryNextPositions ep04{ 255, 19, 3, 5 };
         EntryLeftSkip els04{ 15, 2 };
    EntryEnd ee04{};

    Entry eb05{ 0, 0, 42, 8 };
         EntryNextPositions ep05{ 255, 20, 4, 6 };
         EntryLeftSkip els05{ 15, 2 };
    EntryEnd ee05{};

    Entry eb06{ 0, 0, 42, 8 };
         EntryNextPositions ep06{ 255, 21, 5, 7 };
         EntryLeftSkip els06{ 15, 2 };
    EntryEnd ee06{};

    Entry eb07{ 0, 0, 42, 8 };
         EntryNextPositions ep07{ 255, 22, 6, 8 };
         EntryLeftSkip els07{ 15, 2 };
    EntryEnd ee07{};

    Entry eb08{ 0, 0, 42, 8 };
         EntryNextPositions ep08{ 255, 23, 7, 9 };
         EntryLeftSkip els08{ 15, 2 };
    EntryEnd ee08{};

    Entry eb09{ 0, 0, 42, 8 };
         EntryNextPositions ep09{ 255, 24, 8, 10 };
         EntryLeftSkip els09{ 15, 2 };
    EntryEnd ee09{};

    Entry eb10{ 0, 0, 42, 8 };
         EntryNextPositions ep10{ 255, 25, 9, 11 };
         EntryLeftSkip els10{ 15, 2 };
    EntryEnd ee10{};

    Entry eb11{ 0, 0, 42, 8 };
         EntryNextPositions ep11{ 255, 26, 10, 12 };
         EntryLeftSkip els11{ 15, 2 };
    EntryEnd ee11{};

    Entry eb12{ 0, 0, 42, 8 };
         EntryNextPositions ep12{ 255, 27, 11, 13 };
         EntryLeftSkip els12{ 15, 2 };
    EntryEnd ee12{};

    Entry eb13{ 0, 0, 42, 8 };
         EntryNextPositions ep13{ 255, 28, 12, 14 };
         EntryLeftSkip els13{ 15, 2 };
    EntryEnd ee13{};

    Entry eb14{ 0, 0, 42, 8 };
         EntryNextPositions ep14{ 255, 29, 13, 15 };
         EntryLeftSkip els14{ 15, 2 };
    EntryEnd ee14{};

    Entry eb15{ 0, 0, 42, 8 };
         EntryNextPositions ep15{ 255, 30, 14, 47 };
         EntryLeftSkip els15{ 15, 2 };
    EntryEnd ee15{};

    Entry eb16{ 0, 0, 42, 8 };
         EntryNextPositions ep16{ 1, 31, 50, 17 };
    EntryEnd ee16{};

    Entry eb17{ 0, 0, 42, 8 };
         EntryNextPositions ep17{ 2, 32, 16, 18 };
    EntryEnd ee17{};

    Entry eb18{ 0, 0, 42, 8 };
         EntryNextPositions ep18{ 3, 33, 17, 19 };
         EntryRightSkip ers18{ 30, 2 };
    EntryEnd ee18{};

    Entry eb19{ 0, 0, 42, 8 };
         EntryNextPositions ep19{ 4, 34, 18, 20 };
         EntryRightSkip ers19{ 30, 2 };
    EntryEnd ee19{};

    Entry eb20{ 0, 0, 42, 8 };
         EntryNextPositions ep20{ 5, 35, 19, 21 };
         EntryRightSkip ers20{ 30, 2 };
    EntryEnd ee20{};

    Entry eb21{ 0, 0, 42, 8 };
         EntryNextPositions ep21{ 6, 36, 20, 22 };
         EntryRightSkip ers21{ 30, 2 };
    EntryEnd ee21{};

    Entry eb22{ 0, 0, 42, 8 };
         EntryNextPositions ep22{ 7, 37, 21, 23 };
         EntryRightSkip ers22{ 30, 2 };
    EntryEnd ee22{};

    Entry eb23{ 0, 0, 42, 8 };
         EntryNextPositions ep23{ 8, 38, 22, 24 };
         EntryRightSkip ers23{ 30, 2 };
    EntryEnd ee23{};

    Entry eb24{ 0, 0, 42, 8 };
         EntryNextPositions ep24{ 9, 39, 23, 25 };
         EntryRightSkip ers24{ 30, 2 };
    EntryEnd ee24{};

    Entry eb25{ 0, 0, 42, 8 };
         EntryNextPositions ep25{ 10, 40, 24, 26 };
         EntryRightSkip ers25{ 30, 2 };
    EntryEnd ee25{};

    Entry eb26{ 0, 0, 42, 8 };
         EntryNextPositions ep26{ 11, 41, 25, 27 };
         EntryRightSkip ers26{ 30, 2 };
    EntryEnd ee26{};

    Entry eb27{ 0, 0, 42, 8 };
         EntryNextPositions ep27{ 12, 42, 26, 28 };
         EntryRightSkip ers27{ 30, 2 };
    EntryEnd ee27{};

    Entry eb28{ 0, 0, 42, 8 };
         EntryNextPositions ep28{ 13, 43, 27, 29 };
         EntryRightSkip ers28{ 30, 2 };
    EntryEnd ee28{};

    Entry eb29{ 0, 0, 42, 8 };
         EntryNextPositions ep29{ 14, 44, 28, 30 };
         EntryRightSkip ers29{ 30, 2 };
    EntryEnd ee29{};

    Entry eb30{ 0, 0, 42, 8 };
         EntryNextPositions ep30{ 15, 45, 29, 47 };
         EntryRightSkip ers30{ 30, 2 };
    EntryEnd ee30{};

    Entry eb31{ 0, 0, 42, 8 };
         EntryNextPositions ep31{ 16, 50, 50, 32 };
    EntryEnd ee31{};

    Entry eb32{ 0, 0, 42, 8 };
         EntryNextPositions ep32{ 17, 50, 31, 33 };
    EntryEnd ee32{};

    Entry eb33{ 0, 0, 42, 8 };
         EntryNextPositions ep33{ 18, 50, 32, 34 };
         EntryRightSkip ers33{ 45, 2 };
    EntryEnd ee33{};

    Entry eb34{ 0, 0, 42, 8 };
         EntryNextPositions ep34{ 19, 50, 33, 35 };
         EntryRightSkip ers34{ 45, 2 };
    EntryEnd ee34{};

    Entry eb35{ 0, 0, 42, 8 };
         EntryNextPositions ep35{ 20, 50, 34, 36 };
         EntryRightSkip ers35{ 45, 2 };
    EntryEnd ee35{};

    Entry eb36{ 0, 0, 42, 8 };
         EntryNextPositions ep36{ 21, 50, 35, 37 };
         EntryRightSkip ers36{ 45, 2 };
    EntryEnd ee36{};

    Entry eb37{ 0, 0, 42, 8 };
         EntryNextPositions ep37{ 22, 50, 36, 38 };
         EntryRightSkip ers37{ 45, 2 };
    EntryEnd ee37{};

    Entry eb38{ 0, 0, 42, 8 };
         EntryNextPositions ep38{ 23, 51, 37, 39 };
         EntryRightSkip ers38{ 45, 2 };
    EntryEnd ee38{};

    Entry eb39{ 0, 0, 42, 8 };
         EntryNextPositions ep39{ 24, 51, 38, 40 };
         EntryRightSkip ers39{ 45, 2 };
    EntryEnd ee39{};

    Entry eb40{ 0, 0, 42, 8 };
         EntryNextPositions ep40{ 25, 51, 39, 41 };
         EntryRightSkip ers40{ 45, 2 };
    EntryEnd ee40{};

    Entry eb41{ 0, 0, 42, 8 };
         EntryNextPositions ep41{ 26, 51, 40, 42 };
         EntryRightSkip ers41{ 45, 2 };
    EntryEnd ee41{};

    Entry eb42{ 0, 0, 42, 8 };
         EntryNextPositions ep42{ 27, 51, 41, 43 };
         EntryRightSkip ers42{ 45, 2 };
    EntryEnd ee42{};

    Entry eb43{ 0, 0, 42, 8 };
         EntryNextPositions ep43{ 28, 51, 42, 44 };
         EntryRightSkip ers43{ 45, 2 };
    EntryEnd ee43{};

    Entry eb44{ 0, 0, 42, 8 };
         EntryNextPositions ep44{ 29, 51, 43, 45 };
         EntryRightSkip ers44{ 45, 2 };
    EntryEnd ee44{};

    Entry eb45{ 0, 0, 42, 8 };
         EntryNextPositions ep45{ 30, 51, 44, 47 };
         EntryRightSkip ers45{ 45, 2 };
    EntryEnd ee45{};

    ResetTemplateEntry rte00{};

    Entry eb46{ 0, 0, 39, 8 };
         EntryText et46{ 0, nullptr };
         EntryColor ec46{ 5 };
         EntryInvisible ei46{};
    EntryEnd ee46{};

    Entry eb47{ ((320) -(((38) +1) * 6) -8) / 2, 152, (((38) +1) * 6), 8 };
         EntryText et47{ 0, -1 };
         EntryColor ec47{ 13 };
         EntryInvisible ei47{};
         EntryNextPositions ep47{ 255, 255, 30, 48 };
         EntryOnSelectFunctionNative eosf47{ inputFilenameToSave };
    EntryEnd ee47{};

    Entry eb48{ 121, 165, 62, 15 };
         EntryText et48{ 0, SwosVM::Offsets::aSave };
         EntryColor ec48{ 14 };
         EntryInvisible ei48{};
         EntryNextPositions ep48{ 15, 45, 47, 49 };
         EntryOnSelectFunctionNative eosf48{ saveFileSelected };
    EntryEnd ee48{};

    Entry eb49{ 121, 185, 62, 15 };
         EntryText et49{ 0, SwosVM::Offsets::aAbort };
         EntryColor ec49{ 10 };
         EntryNextPositions ep49{ 15, 45, 48, 255 };
         EntryOnSelectFunctionNative eosf49{ abortSelectFile };
    EntryEnd ee49{};

    Entry eb50{ ((320) -8 -(14)), 0, 8, 14 };
         EntryForegroundSprite efs50{ 183 };
         EntryNextPositions ep50{ 255, 255, 255, 51 };
         EntryOnSelectFunctionNative eosf50{ scrollUp };
    EntryEnd ee50{};

    Entry eb51{ ((320) -8 -(14)), 0, 8, 14 };
         EntryForegroundSprite efs51{ 184 };
         EntryNextPositions ep51{ 255, 255, 50, 255 };
         EntryOnSelectFunctionNative eosf51{ scrollDown };
    EntryEnd ee51{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const selectFilesMenu
#endif
;
namespace SelectFilesMenu {
    enum Entries {
        title = 0,
        fileEntry_00 = 1,
        fileEntry_01 = 2,
        fileEntry_02 = 3,
        fileEntry_03 = 4,
        fileEntry_04 = 5,
        fileEntry_05 = 6,
        fileEntry_06 = 7,
        fileEntry_07 = 8,
        fileEntry_08 = 9,
        fileEntry_09 = 10,
        fileEntry_10 = 11,
        fileEntry_11 = 12,
        fileEntry_12 = 13,
        fileEntry_13 = 14,
        fileEntry_14 = 15,
        fileEntry_15 = 16,
        fileEntry_16 = 17,
        fileEntry_17 = 18,
        fileEntry_18 = 19,
        fileEntry_19 = 20,
        fileEntry_20 = 21,
        fileEntry_21 = 22,
        fileEntry_22 = 23,
        fileEntry_23 = 24,
        fileEntry_24 = 25,
        fileEntry_25 = 26,
        fileEntry_26 = 27,
        fileEntry_27 = 28,
        fileEntry_28 = 29,
        fileEntry_29 = 30,
        fileEntry_30 = 31,
        fileEntry_31 = 32,
        fileEntry_32 = 33,
        fileEntry_33 = 34,
        fileEntry_34 = 35,
        fileEntry_35 = 36,
        fileEntry_36 = 37,
        fileEntry_37 = 38,
        fileEntry_38 = 39,
        fileEntry_39 = 40,
        fileEntry_40 = 41,
        fileEntry_41 = 42,
        fileEntry_42 = 43,
        fileEntry_43 = 44,
        fileEntry_44 = 45,
        description = 46,
        inputSaveFilename = 47,
        saveLabel = 48,
        abort = 49,
        arrowUp = 50,
        arrowDown = 51,
    };

    // entry references
    static auto& titleEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& fileEntry_00Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& fileEntry_01Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& fileEntry_02Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& fileEntry_03Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 4 * sizeof(MenuEntry));
    static auto& fileEntry_04Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& fileEntry_05Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& fileEntry_06Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& fileEntry_07Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& fileEntry_08Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& fileEntry_09Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& fileEntry_10Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& fileEntry_11Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& fileEntry_12Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& fileEntry_13Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& fileEntry_14Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& fileEntry_15Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& fileEntry_16Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
    static auto& fileEntry_17Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 18 * sizeof(MenuEntry));
    static auto& fileEntry_18Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 19 * sizeof(MenuEntry));
    static auto& fileEntry_19Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 20 * sizeof(MenuEntry));
    static auto& fileEntry_20Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 21 * sizeof(MenuEntry));
    static auto& fileEntry_21Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 22 * sizeof(MenuEntry));
    static auto& fileEntry_22Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 23 * sizeof(MenuEntry));
    static auto& fileEntry_23Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 24 * sizeof(MenuEntry));
    static auto& fileEntry_24Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 25 * sizeof(MenuEntry));
    static auto& fileEntry_25Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 26 * sizeof(MenuEntry));
    static auto& fileEntry_26Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 27 * sizeof(MenuEntry));
    static auto& fileEntry_27Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 28 * sizeof(MenuEntry));
    static auto& fileEntry_28Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 29 * sizeof(MenuEntry));
    static auto& fileEntry_29Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 30 * sizeof(MenuEntry));
    static auto& fileEntry_30Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 31 * sizeof(MenuEntry));
    static auto& fileEntry_31Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 32 * sizeof(MenuEntry));
    static auto& fileEntry_32Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 33 * sizeof(MenuEntry));
    static auto& fileEntry_33Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 34 * sizeof(MenuEntry));
    static auto& fileEntry_34Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 35 * sizeof(MenuEntry));
    static auto& fileEntry_35Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 36 * sizeof(MenuEntry));
    static auto& fileEntry_36Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 37 * sizeof(MenuEntry));
    static auto& fileEntry_37Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 38 * sizeof(MenuEntry));
    static auto& fileEntry_38Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 39 * sizeof(MenuEntry));
    static auto& fileEntry_39Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 40 * sizeof(MenuEntry));
    static auto& fileEntry_40Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 41 * sizeof(MenuEntry));
    static auto& fileEntry_41Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 42 * sizeof(MenuEntry));
    static auto& fileEntry_42Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 43 * sizeof(MenuEntry));
    static auto& fileEntry_43Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 44 * sizeof(MenuEntry));
    static auto& fileEntry_44Entry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 45 * sizeof(MenuEntry));
    static auto& descriptionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 46 * sizeof(MenuEntry));
    static auto& inputSaveFilenameEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 47 * sizeof(MenuEntry));
    static auto& saveLabelEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 48 * sizeof(MenuEntry));
    static auto& abortEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 49 * sizeof(MenuEntry));
    static auto& arrowUpEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 50 * sizeof(MenuEntry));
    static auto& arrowDownEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 51 * sizeof(MenuEntry));

    constexpr int kMaxColumns = 3;
    constexpr int kMaxEntriesPerColumn = 15;
    constexpr int kNumFilenameItems = 3 * (15);
    constexpr int kFilenameHeight = 8;
    constexpr int kMaxSaveFilenameChars = 38;
    constexpr int kFirstFileEntry = 0 +1;
    constexpr int kLastFilenameOrdinal = 45;
    constexpr int kDefaultArrowX = (320) -8 -(14);
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void saveFileSelected() {}
static void scrollDown() {}
static void selectFilesOnReturn() {}
static void abortSelectFile() {}
static void drawDescription() {}
static void selectFile() {}
static void selectFilesOnInit() {}
static void scrollUp() {}
static void inputFilenameToSave() {}
#endif
