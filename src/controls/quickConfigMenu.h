#pragma once

#include "gameControlEvents.h"
#include "controls.h"
#include "keyboard.h"

enum class QuickConfigControls
{
    kKeyboard1,
    kKeyboard2,
    kJoypad,
};

enum class QuickConfigStatus
{
    kNoInput,
    kContinue,
    kAbort,
    kDone,
    kAlreadyUsed,
    kTakenByOtherPlayer,
};

struct QuickConfigContext
{
    using GetControlFunction = std::pair<QuickConfigStatus, const char *>(*)(QuickConfigContext& context);
    using ResetFunction = std::function<void(QuickConfigContext& context)>;

    static constexpr int kElementNameBufferSize = 32;

    // VS is driving me crazy... it wouldn't compile without a constructor under x86 and C++17 only
    QuickConfigContext(PlayerNumber player, QuickConfigControls controls, int joypadIndex, const char *selectWhat, const char *abortText,
        GetControlFunction getControlFn, ResetFunction resetFn, std::function<void()> redrawMenuFn = {}, bool benchRequired = false)
        : player(player), controls(controls), joypadIndex(joypadIndex), elementNames{}, selectWhat(selectWhat),
        abortText(abortText), getControlFn(getControlFn), resetFn(resetFn), redrawMenuFn(redrawMenuFn), benchRequired(benchRequired) {}

    PlayerNumber player;
    QuickConfigControls controls;
    int joypadIndex;
    char elementNames[kNumDefaultGameControlEvents][kElementNameBufferSize];
    char scratchBuf[kElementNameBufferSize];
    const char *selectWhat;
    const char *abortText;
    GetControlFunction getControlFn;
    ResetFunction resetFn;
    std::function<void()> redrawMenuFn;
    int currentSlot = -1;
    int warningY = -1;
    bool benchRequired = false;
    bool redefiningControls = true;

    void reset() {
        currentSlot = -1;
        std::for_each(std::begin(elementNames), std::end(elementNames), [](auto *name) { name[0] = '\0'; });

        assert(resetFn);
        resetFn(*this);
    }

    std::pair<QuickConfigStatus, const char *> getNextControl() {
        assert(getControlFn);
        return getControlFn(*this);
    }

    static QuickConfigContext getKeyboardContext(Keyboard keyboard, GetControlFunction get,
        ResetFunction reset, std::function<void()> redrawMenu = {})
    {
        auto controls = keyboard == Keyboard::kSet1 ? QuickConfigControls::kKeyboard1 : QuickConfigControls::kKeyboard2;
#ifdef __ANDROID__
        return QuickConfigContext(kPlayer1, controls, kNoJoypad, "KEY", "(TAP ABORTS)", get, reset, redrawMenu, true);
#else
        return QuickConfigContext(kPlayer1, controls, kNoJoypad, "KEY", "(MOUSE CLICK ABORTS)", get, reset, redrawMenu, true);
#endif
    }
    static QuickConfigContext getJoypadContext(PlayerNumber player, int joypadIndex, GetControlFunction get, ResetFunction reset) {
#ifdef __ANDROID__
        return QuickConfigContext(player, QuickConfigControls::kJoypad, joypadIndex, "CONTROL", "(TAP/BACK ABORTS)", get, reset);
#else
        return QuickConfigContext(player, QuickConfigControls::kJoypad, joypadIndex, "CONTROL", "(MOUSE CLICK/ESCAPE ABORTS)", get, reset);
#endif
    }
};

bool promptForDefaultGameEvents(QuickConfigContext& context);
QuickConfigStatus getNextControl(QuickConfigContext& context);
