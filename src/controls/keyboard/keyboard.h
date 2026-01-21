#pragma once

#include "controls.h"
#include "scancodes.h"
#include "gameControlEvents.h"
#include "KeyConfig.h"

bool keyboardPresent();

enum class Keyboard {
    kSet1,
    kSet2,
};

GameControlEvents eventsFromAllKeysets();
GameControlEvents keyboard1Events();
GameControlEvents keyboard2Events();
GameControlEvents keyboardEvents(Keyboard keyboard);

bool keyboard1HasScancode(SDL_Scancode scancode);
bool keyboard2HasScancode(SDL_Scancode scancode);
bool keyboardHasScancode(Keyboard keyboard, SDL_Scancode key);
bool keyboard1HasBasicBindings();
bool keyboard2HasBasicBindings();
bool keyboardHasBasicBindings(Keyboard keyboard);

const KeyConfig::KeyBindings& getKeyboardKeyBindings(Keyboard keyboard);
const KeyConfig::KeyBinding& getKeyboardKeyBindingAt(Keyboard keyboard, size_t index);

void addKeyboardKeyBinding(Keyboard keyboard, SDL_Scancode key, GameControlEvents events);
void updateKeyboardKeyBindingEventsAt(Keyboard keyboard, size_t index, GameControlEvents events);
void setDefaultKeyPackForKeyboard(Keyboard keyboard, const DefaultScancodesPack& scancodes);

void deleteKeyBindingAt(Keyboard keyboard, size_t index);

void loadKeyboardConfig(const CSimpleIni& ini);
void saveKeyboardConfig(CSimpleIni& ini);
