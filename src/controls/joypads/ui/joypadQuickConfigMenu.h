#pragma once

#include "controls.h"
#include "JoypadElementValue.h"

std::pair<bool, DefaultJoypadElementList> promptForDefaultJoypadEvents(PlayerNumber player, int joypadIndex);
