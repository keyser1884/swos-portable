#pragma once

void initMatch(TeamGame *topTeam, TeamGame *bottomTeam, bool saveOrRestoreTeams);
void matchEnded();
void startMainGameLoop();
void initializeIngameTeams(int minSubs, int maxSubs, TeamFile *team1, TeamFile *team2);
void checkGlobalKeyboardShortcuts(SDL_Scancode scancode, bool pressed);
bool checkGameKeys();
bool checkZoomKeys();
void updateCursor(bool matchRunning);
bool isMatchRunning();
bool isGamePaused();
void pauseTheGame();
void togglePause();
