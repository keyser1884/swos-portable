#pragma once

#include "gameControlEvents.h"

GameControlEvents mouseEvents();
void resetMouse();
void updateMouseButtons(Uint8 button, Uint8 state);
void updateMouseMovement(Sint32 xrel, Sint32 yrel, Uint32 state);
