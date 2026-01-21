#pragma once

enum PlayerNumber {
    kNoPlayer = -1,
    kPlayer1 = 0,
    kPlayer2 = 1,
};

enum Controls {
    kNone,
    kKeyboard1,
    kKeyboard2,
    kJoypad,
    kMouse,                 // :D
    kNumControls,
    kNumUniqueControls = kNumControls - 1,
};

constexpr int kNoJoypad = -1;

const char *controlsToString(Controls controls);

Controls getPl1Controls();
Controls getPl2Controls();
Controls getControls(PlayerNumber player);
bool setControls(PlayerNumber player, Controls controls, int joypadIndex = kNoJoypad);
void setPl1Controls(Controls controls, int joypadIndex = kNoJoypad);
void setPl2Controls(Controls controls, int joypadIndex = kNoJoypad);
void unsetKeyboardControls();

bool anyInputActive();
std::tuple<bool, int, int> mouseClickAndRelease();
void waitForKeyboardAndMouseIdle();
int mouseWheelAmount();

void waitForKeyPress();
std::tuple<bool, SDL_Scancode, int, int> getKeyInterruptible();
void processControlEvents();

void setGlobalShortcutsEnabled(bool enabled);
bool getShowSelectMatchControlsMenu();
void setShowSelectMatchControlsMenu(bool value);

void loadControlOptions(const CSimpleIni& ini);
void saveControlOptions(CSimpleIni& ini);

void initGameControls();

bool gotMousePlayer();
bool testForPlayerKeys(SDL_Scancode key);
