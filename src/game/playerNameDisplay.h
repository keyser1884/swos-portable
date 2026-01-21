#pragma once

void updateCurrentPlayerName();
void drawPlayerName();
#ifdef SWOS_TEST
std::pair<bool, int> getDisplayedPlayerNumberAndTeam();
#endif
