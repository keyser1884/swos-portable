#pragma once

#include "gameControlEvents.h"
#include "controls.h"

void resetGameControls();
bool updateFireBlocked();
TeamGeneralInfo *selectTeamForUpdate();
void updateTeamControls(TeamGeneralInfo *team);
void postUpdateTeamControls(TeamGeneralInfo *team);
GameControlEvents getPlayerEvents(PlayerNumber player);
bool isPlayerFiring(PlayerNumber player);
bool getFireStartedAndBumpFireCounter(bool currentFire, PlayerNumber player = kPlayer1);
int16_t eventsToDirection(GameControlEvents events);
GameControlEvents directionToEvents(int16_t direction);
bool isAnyPlayerFiring();
