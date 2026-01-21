#include "redefineKeysMenu.h"
#include "keyBuffer.h"
#include "keyboard.h"
#include "controls.h"
#include "gameControlEvents.h"
#include "quickConfigMenu.h"

static DefaultScancodesPack m_keys;

static QuickConfigStatus getScanCodeStatus(Keyboard keyboard, SDL_Scancode key, int numKeys, bool redefineKeys)
{
    assert(keyboard == Keyboard::kSet1 || keyboard == Keyboard::kSet2);

    if (std::find(m_keys.begin(), m_keys.begin() + numKeys, key) != m_keys.begin() + numKeys)
        return QuickConfigStatus::kAlreadyUsed;

    if (!redefineKeys && (keyboard == Keyboard::kSet1 && keyboard1HasScancode(key) ||
        keyboard == Keyboard::kSet2 && keyboard2HasScancode(key)))
        return QuickConfigStatus::kAlreadyUsed;

    if (keyboard == Keyboard::kSet1 && getPl2Controls() == kKeyboard2) {
        if (keyboard2HasScancode(key))
            return QuickConfigStatus::kTakenByOtherPlayer;
    } else if (keyboard == Keyboard::kSet2 && getPl1Controls() == kKeyboard1) {
        if (keyboard1HasScancode(key))
            return QuickConfigStatus::kTakenByOtherPlayer;
    }

    return QuickConfigStatus::kContinue;
}

static void resetKeys(QuickConfigContext& context)
{
    m_keys.fill(SDL_SCANCODE_UNKNOWN);
    context.currentSlot = 0;
}

static void addKey(QuickConfigContext& context, SDL_Scancode key)
{
    m_keys[context.currentSlot] = key;
    auto& elementName = context.elementNames[context.currentSlot];
    scancodeToString(key, elementName, sizeof(elementName));
    context.currentSlot++;
}

static std::pair<QuickConfigStatus, const char *> getControlKey(QuickConfigContext& context)
{
    assert(context.controls == QuickConfigControls::kKeyboard1 || context.controls == QuickConfigControls::kKeyboard2);

    auto keyClick = getKeyInterruptible();
    auto clicked = std::get<0>(keyClick);
    auto key = std::get<1>(keyClick);

    if (clicked)
        return { QuickConfigStatus::kAbort, nullptr };

    if (key != SDL_SCANCODE_UNKNOWN) {
        waitForKeyboardAndMouseIdle();

        auto keyboard = context.controls == QuickConfigControls::kKeyboard1 ? Keyboard::kSet1 : Keyboard::kSet2;
        auto status = getScanCodeStatus(keyboard, key, context.currentSlot, context.redefiningControls);

        if (status == QuickConfigStatus::kContinue) {
            addKey(context, key);
            status = context.currentSlot >= kNumDefaultGameControlEvents ? QuickConfigStatus::kDone : status;
            return { status, nullptr };
        } else {
            auto keyName = scancodeToString(key, context.scratchBuf, sizeof(context.scratchBuf));
            return { status, keyName };
        }
    }

    return { QuickConfigStatus::kNoInput, nullptr };
}

SDL_Scancode getControlKey(Keyboard keyboard, int warningY, std::function<void()> redrawMenuFn)
{
    auto context = QuickConfigContext::getKeyboardContext(keyboard, getControlKey, nullptr, redrawMenuFn);
    context.currentSlot = 0;
    context.warningY = warningY;
    context.redefiningControls = false;

    auto status = getNextControl(context);

    return status == QuickConfigStatus::kContinue ? m_keys[0] : SDL_SCANCODE_UNKNOWN;
}

std::pair<bool, DefaultScancodesPack> promptForDefaultKeys(Keyboard keyboard)
{
    waitForKeyboardAndMouseIdle();

    auto context = QuickConfigContext::getKeyboardContext(keyboard, getControlKey, resetKeys);
    auto result = promptForDefaultGameEvents(context);

    return { result, m_keys };
}
