#pragma once

struct GameStats
{
    TeamStatsData team1;
    TeamStatsData team2;
};

void initStats();
void toggleStats();
void hideStats();
bool statsEnqueued();
bool showingUserRequestedStats();
bool showingPostGameStats();
void updateStatistics();
const GameStats getStats();
void drawStats(int team1Goals, int team2Goals, const GameStats& stats);
void drawStatsIfNeeded();
