// automatically generated from main.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void mainMenuOnInit();
static void showQuitMenu();

#pragma pack(push, 1)
struct SwosMenu_mainMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, mainMenuOnInit, 0, 0, 0, 5, { true, false, false, false, }};

    MenuXY menuXY00{0, 1};

    Entry eb00{ 0, 40, 150, 16 };
         EntryText et00{ 16, SwosVM::Offsets::aEditTactics };
         EntryColor ec00{ 3 };
         EntryNextPositions ep00{ 255, 5, 255, 1 };
         EntryOnSelectFunction eosf00{ SwosVM::Procs::EditTacticsMenu };
    EntryEnd ee00{};

    Entry eb01{ 0, ((((40 + 16)))) +4, 150, 16 };
         EntryText et01{ 16, SwosVM::Offsets::aEditCustomTeam };
         EntryColor ec01{ 3 };
         EntryNextPositions ep01{ 255, 6, 0, 2 };
         EntryOnSelectFunction eosf01{ SwosVM::Procs::EditCustomTeamMenu };
    EntryEnd ee01{};

    Entry eb02{ 0, ((((((((40 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryTextNative et02{ 16, "REPLAYS" };
         EntryColor ec02{ 11 };
         EntryNextPositions ep02{ 255, 7, 1, 3 };
         EntryOnSelectFunctionNative eosf02{ showReplaysMenu };
    EntryEnd ee02{};

    Entry eb03{ 0, ((((((((((((40 + 16)))) +4 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryText et03{ 16, SwosVM::Offsets::aOptions };
         EntryColor ec03{ 3 };
         EntryNextPositions ep03{ 255, 8, 2, 9 };
         EntryOnSelectFunctionNative eosf03{ showOptionsMenu };
    EntryEnd ee03{};

    Entry eb04{ 0, ((((((((((((((((40 + 16)))) +4 + 16)))) +4 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryInvisible ei04{};
    EntryEnd ee04{};

    Entry eb05{ 154, 40, 150, 16 };
         EntryText et05{ 16, SwosVM::Offsets::aFriendly };
         EntryColor ec05{ 12 };
         EntryNextPositions ep05{ 0, 255, 255, 6 };
         EntryOnSelectFunction eosf05{ SwosVM::Procs::FriendlyMenu };
    EntryEnd ee05{};

    Entry eb06{ 154, ((((40 + 16)))) +4, 150, 16 };
         EntryText et06{ 16, SwosVM::Offsets::aDiyCompetition };
         EntryColor ec06{ 4 };
         EntryNextPositions ep06{ 1, 255, 5, 7 };
         EntryOnSelectFunction eosf06{ SwosVM::Procs::DIYCompetitionMenu };
    EntryEnd ee06{};

    Entry eb07{ 154, ((((((((40 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryText et07{ 16, SwosVM::Offsets::aPresetCompetit };
         EntryColor ec07{ 4 };
         EntryNextPositions ep07{ 2, 255, 6, 8 };
         EntryOnSelectFunction eosf07{ SwosVM::Procs::PresetCompetitionMenu };
    EntryEnd ee07{};

    Entry eb08{ 154, ((((((((((((40 + 16)))) +4 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryText et08{ 16, SwosVM::Offsets::aSeason };
         EntryColor ec08{ 4 };
         EntryNextPositions ep08{ 3, 255, 7, 9 };
         EntryOnSelectFunction eosf08{ SwosVM::Procs::SeasonMenu };
    EntryEnd ee08{};

    Entry eb09{ 79, ((((((((((((((((40 + 16)))) +4 + 16)))) +4 + 16)))) +4 + 16)))) +4, 150, 16 };
         EntryText et09{ 16, SwosVM::Offsets::aCareer };
         EntryColor ec09{ 4 };
         EntryNextPositions ep09{ 255, 255, 8, 10 };
         EntryOnSelectFunction eosf09{ SwosVM::Procs::CareerMenu };
    EntryEnd ee09{};

    MenuXY menuXY10{0, 0};

    Entry eb10{ 27, 144, 250, 14 };
         EntryText et10{ 0, -1 };
         EntryColor ec10{ 14 };
         EntryNextPositions ep10{ 255, 255, 9, 11 };
         EntryOnSelectFunction eosf10{ SwosVM::Procs::StartContest };
    EntryEnd ee10{};

    Entry eb11{ 27, 165, 250, 14 };
         EntryText et11{ 0, SwosVM::Offsets::saveFileTitle };
         EntryColor ec11{ 10 };
         EntryNextPositions ep11{ 255, 255, 10, 12 };
         EntryOnSelectFunction eosf11{ SwosVM::Procs::SaveContest };
    EntryEnd ee11{};

    Entry eb12{ 27, 182, 250, 14 };
         EntryText et12{ 0, SwosVM::Offsets::aLoadOldCompeti };
         EntryColor ec12{ 4 };
         EntryNextPositions ep12{ 13, 255, 11, 13 };
         EntryOnSelectFunction eosf12{ SwosVM::Procs::LoadOldCompetitionMenu };
    EntryEnd ee12{};

    Entry eb13{ 0 -6, (200) -(17) -2, 31, 17 };
         EntryForegroundSprite efs13{ 1334 };
         EntryNextPositions ep13{ 255, 12, 12, 255 };
         EntryOnSelectFunctionNative eosf13{ showQuitMenu };
    EntryEnd ee13{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const mainMenu
#endif
;
namespace MainMenu {
    enum Entries {
        editTactics = 0,
        editCustomTeams = 1,
        replays = 2,
        options = 3,
        friendly = 5,
        diyCompetition = 6,
        presetCompetition = 7,
        season = 8,
        career = 9,
        continueContest = 10,
        saveContest = 11,
        loadOldCompetition = 12,
        exit = 13,
    };

    // entry references
    static auto& editTacticsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& editCustomTeamsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 1 * sizeof(MenuEntry));
    static auto& replaysEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 2 * sizeof(MenuEntry));
    static auto& optionsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 3 * sizeof(MenuEntry));
    static auto& friendlyEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 5 * sizeof(MenuEntry));
    static auto& diyCompetitionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 6 * sizeof(MenuEntry));
    static auto& presetCompetitionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& seasonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& careerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& continueContestEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& saveContestEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& loadOldCompetitionEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void mainMenuOnInit() {}
static void showQuitMenu() {}
#endif
