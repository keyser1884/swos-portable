#pragma once

enum class BenchState
{
    kInitial,
    kAboutToSubstitute,
    kFormationMenu,
    kMarkingPlayers,
    kOpponentsBench,
};

void initBenchControls();
bool benchCheckControls();
BenchState getBenchState();
bool trainingTopTeam();
void setTrainingTopTeam(bool value);
void requestBench1();
void requestBench2();
int getBenchPlayerIndex();
int getBenchPlayerShirtNumber(bool topTeam, int index);
int getBenchMenuSelectedPlayer();
int getSelectedFormationEntry();
bool inBenchOrGoingTo();
bool goingToBenchDelay();
int playerToEnterGameIndex();
int playerToBeSubstitutedIndex();
int playerToBeSubstitutedPos();
bool substituteInProgress();
bool newPlayerAboutToGoIn();
void setSubstituteInProgress();
const TeamGeneralInfo *getBenchTeam();
const TeamGame *getBenchTeamData();

#ifdef SWOS_TEST
struct BenchData;
void fillBenchData(BenchData& data);
#endif
