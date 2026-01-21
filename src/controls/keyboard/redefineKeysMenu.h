#pragma once

#include "gameControlEvents.h"
#include "keyboard.h"

std::pair<bool, DefaultScancodesPack> promptForDefaultKeys(Keyboard keyboard);
SDL_Scancode getControlKey(Keyboard keyboard, int warningY, std::function<void()> redrawMenuFn);
