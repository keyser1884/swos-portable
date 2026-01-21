// automatically generated from audioOptions.mnu, do not edit
#pragma once

#include "menus.h"
#include "swossym.h"

using namespace SwosMenu;

static void decreaseMusicVolume();
static void increaseMusicVolume();
static void enterMasterVolume();
static void decreaseVolume();
static void initAudioOptionsMenu();
static void increaseVolume();
static void enterMusicVolume();

#pragma pack(push, 1)
struct SwosMenu_audioOptionsMenu : public BaseMenu
{
    MenuHeaderV2 header{ kMenuHeaderV2Mark, initAudioOptionsMenu, initAudioOptionsMenu, 0, 0, 7, { true, true, false, false, }};

    Entry eb00{ (((320) -(125)) / 2 -8), 0, 125, 15 };
         EntryTextNative et00{ 0, "AUDIO OPTIONS" };
         EntryColor ec00{ (7 | (16)) };
    EntryEnd ee00{};

    TemplateEntry te00{};

    Entry eb30001{ 0, 0, 1, 1 };
         EntryColor ec30001{ 7 };
    EntryEnd ee30001{};

    Entry eb01{ 10, 60, 140, 11 };
         EntryTextNative et01{ 0, "SOUND" };
    EntryEnd ee01{};

    Entry eb02{ 10, ((((60 + 11)))) +(1) +1, 140, 11 };
         EntryTextNative et02{ 0, "MENU MUSIC" };
    EntryEnd ee02{};

    Entry eb03{ 10, ((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryTextNative et03{ 0, "COMMENTARY" };
    EntryEnd ee03{};

    Entry eb04{ 10, ((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryTextNative et04{ 0, "CROWD CHANTS" };
    EntryEnd ee04{};

    Entry eb05{ 10, ((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryTextNative et05{ 0, "MATCH VOLUME" };
    EntryEnd ee05{};

    Entry eb06{ 10, ((((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryTextNative et06{ 0, "MUSIC VOLUME" };
    EntryEnd ee06{};

    TemplateEntry te01{};

    Entry eb30008{ 0, 0, 1, 1 };
         EntryColor ec30008{ 13 };
    EntryEnd ee30008{};

    Entry eb07{ 154, 60, 140, 11 };
         EntryBoolOption ebo07{ soundEnabled, setSoundEnabled, "master sound" };
         EntryNextPositions ep07{ 255, 255, 255, 8 };
    EntryEnd ee07{};

    Entry eb08{ 154, ((((60 + 11)))) +(1) +1, 140, 11 };
         EntryBoolOption ebo08{ musicEnabled, setMusicEnabled, "music" };
         EntryNextPositions ep08{ 255, 255, 7, 9 };
    EntryEnd ee08{};

    Entry eb09{ 154, ((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryBoolOption ebo09{ commentaryEnabled, setCommentaryEnabled, "commentary" };
         EntryNextPositions ep09{ 255, 255, 8, 10 };
    EntryEnd ee09{};

    Entry eb10{ 154, ((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1, 140, 11 };
         EntryBoolOption ebo10{ areCrowdChantsEnabled, setCrowdChantsEnabled, "crowd chants" };
         EntryNextPositions ep10{ 255, 255, 9, 12 };
    EntryEnd ee10{};

    Entry eb11{ 154, ((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1, 16, 11 };
         EntryText et11{ 0, SwosVM::Offsets::aMinus };
         EntryColor ec11{ 3 };
         EntryNextPositions ep11{ 255, 12, 10, 14 };
         EntryOnSelectFunctionNative eosf11{ decreaseVolume };
    EntryEnd ee11{};

    Entry eb12{ (((154 + 16))) +1, ((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1)), ((140) -2 * (16) -2), 11 };
         EntryNumber en12{ 0, 0 };
         EntryNextPositions ep12{ 11, 13, 10, 15 };
         EntryOnSelectFunctionNative eosf12{ enterMasterVolume };
    EntryEnd ee12{};

    Entry eb13{ ((((((154 + 16))) +1 + ((140) -2 * (16) -2)))) +1, ((((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1)))), 16, 11 };
         EntryTextNative et13{ 0, "+" };
         EntryColor ec13{ 3 };
         EntryNextPositions ep13{ 12, 255, 10, 16 };
         EntryOnSelectFunctionNative eosf13{ increaseVolume };
    EntryEnd ee13{};

    Entry eb14{ 154, ((((((((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1)))) + 11)))) +(1) +1, 16, 11 };
         EntryTextNative et14{ 0, "-" };
         EntryColor ec14{ 3 };
         EntryNextPositions ep14{ 255, 15, 11, 17 };
         EntryOnSelectFunctionNative eosf14{ decreaseMusicVolume };
    EntryEnd ee14{};

    Entry eb15{ (((154 + 16))) +1, ((((((((((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1)))) + 11)))) +(1) +1)), ((140) -2 * (16) -2), 11 };
         EntryNumber en15{ 0, 0 };
         EntryNextPositions ep15{ 14, 16, 12, 17 };
         EntryOnSelectFunctionNative eosf15{ enterMusicVolume };
    EntryEnd ee15{};

    Entry eb16{ ((((((154 + 16))) +1 + ((140) -2 * (16) -2)))) +1, ((((((((((((((((((((((((((((60 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1 + 11)))) +(1) +1)))) + 11)))) +(1) +1)))), 16, 11 };
         EntryText et16{ 0, SwosVM::Offsets::aPlus };
         EntryColor ec16{ 3 };
         EntryNextPositions ep16{ 15, 255, 13, 17 };
         EntryOnSelectFunctionNative eosf16{ increaseMusicVolume };
    EntryEnd ee16{};

    Entry eb17{ (((320) -(125)) / 2 -8), ((200) -(15)), 125, 15 };
         EntryText et17{ 0, SwosVM::Offsets::aExit };
         EntryColor ec17{ 4 };
         EntryNextPositions ep17{ 255, 255, 14, 255 };
         EntryOnSelectFunctionNative eosf17{ SetExitMenuFlag };
    EntryEnd ee17{};

    MenuEnd menuEnd{};
}
#ifndef SWOS_STUB_MENU_DATA
static const audioOptionsMenu
#endif
;
namespace AudioOptionsMenu {
    enum Entries {
        header = 0,
        masterSound = 7,
        menuMusic = 8,
        commentary = 9,
        crowdChants = 10,
        decreaseVolumeButton = 11,
        volume = 12,
        increaseVolumeButton = 13,
        decreaseMusicVolumeButton = 14,
        musicVolume = 15,
        increaseMusicVolumeButton = 16,
        exit = 17,
    };

    // entry references
    static auto& headerEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 0 * sizeof(MenuEntry));
    static auto& masterSoundEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 7 * sizeof(MenuEntry));
    static auto& menuMusicEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 8 * sizeof(MenuEntry));
    static auto& commentaryEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 9 * sizeof(MenuEntry));
    static auto& crowdChantsEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 10 * sizeof(MenuEntry));
    static auto& decreaseVolumeButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 11 * sizeof(MenuEntry));
    static auto& volumeEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 12 * sizeof(MenuEntry));
    static auto& increaseVolumeButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 13 * sizeof(MenuEntry));
    static auto& decreaseMusicVolumeButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 14 * sizeof(MenuEntry));
    static auto& musicVolumeEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 15 * sizeof(MenuEntry));
    static auto& increaseMusicVolumeButtonEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 16 * sizeof(MenuEntry));
    static auto& exitEntry = *reinterpret_cast<MenuEntry *>(swos.g_currentMenu + sizeof(Menu) + 17 * sizeof(MenuEntry));
}
#pragma pack(pop)

#ifdef SWOS_STUB_MENU_DATA
static void decreaseMusicVolume() {}
static void increaseMusicVolume() {}
static void enterMasterVolume() {}
static void decreaseVolume() {}
static void initAudioOptionsMenu() {}
static void increaseVolume() {}
static void enterMusicVolume() {}
#endif
