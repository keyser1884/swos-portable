#pragma once

#include "controls.h"
#include "gameControlEvents.h"

namespace TestControlsMenu {
    using GetControlsFn = std::function<std::vector<std::string>()>;
    using GetEventsFn = std::function<GameControlEvents()>;
}

void showTestControlsMenu(const char *title, const char *controlsTitle, bool allowEscapeExit,
    TestControlsMenu::GetControlsFn getControls, TestControlsMenu::GetEventsFn getEvents);
