#pragma once

void showVersusMenu(const TeamGame *team1, const TeamGame *team2,
    const char *gameName, const char *gameRound, std::function<void()> callback);
void setTeamNameAndColor(MenuEntry& entry, const TeamGame& team, word coachNo, word playerNo);
