#pragma once

#include "gameControlEvents.h"

using SetGameControlEventsFunction = std::function<void(int, GameControlEvents, bool)>;
using GetGameControlEventsFunction = std::function<std::pair<GameControlEvents, bool>(int&, char *, int)>;
using GameControlEventsDisconnectedFunction = std::function<bool()>;

std::pair<bool, GameControlEvents> getNewGameControlEvents(const char *title);
void updateGameControlEvents(int index, SetGameControlEventsFunction setFunction, GetGameControlEventsFunction getFunction,
    GameControlEventsDisconnectedFunction disconnected = {}, bool showInverted = false);
