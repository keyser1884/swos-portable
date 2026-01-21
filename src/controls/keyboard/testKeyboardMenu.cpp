#include "testKeyboardMenu.h"
#include "testControlsMenu.h"
#include "scancodes.h"
#include "keyboard.h"
#include "controls.h"

static Keyboard m_keyboard;

static std::vector<std::string> getControlNames()
{
    std::vector<std::string> keys;

    int totalKeys;
    auto keyboardState = SDL_GetKeyboardState(&totalKeys);

    while (totalKeys--) {
        if (keyboardState[totalKeys])
            keys.emplace_back(scancodeToString(static_cast<SDL_Scancode>(totalKeys)));
    }

    return keys;
}

static GameControlEvents getEvents()
{
    return keyboardEvents(m_keyboard);
}

void showTestKeyboardMenu(Keyboard keyboard)
{
    m_keyboard = keyboard;
    setGlobalShortcutsEnabled(false);

    showTestControlsMenu("TEST KEYBOARD", "KEYS PRESSED:", false, getControlNames, getEvents);

    setGlobalShortcutsEnabled(true);
}
