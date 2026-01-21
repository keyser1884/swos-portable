#include "result.h"

static bool bumpTeamGoals(int teamNum)
{
    constexpr int kMaxGoals = 99;

    static word *goalVars[2][2] = {
        &swos.team1TotalGoals, &swos.team2TotalGoals,
        &swos.team1PenaltyGoals, &swos.team2PenaltyGoals,
    };

    bool isSecondTeam = teamNum == 2;

    if (*goalVars[0][isSecondTeam] == kMaxGoals)
        return false;

    ++*goalVars[swos.playingPenalties != 0][isSecondTeam];

    static word * const kDigitsAndStats[2][3] = {
        &swos.team1GoalsDigit1, &swos.team1GoalsDigit2, &swos.statsTeam1Goals,
        &swos.team2GoalsDigit1, &swos.team2GoalsDigit2, &swos.statsTeam2Goals,
    };

    auto teamDigitsAndStats = kDigitsAndStats[isSecondTeam];

    if (++*teamDigitsAndStats[1] == 10) {
        ++*teamDigitsAndStats[0];
        *teamDigitsAndStats[1] = 0;
    }
    ++*teamDigitsAndStats[2];

    return true;
}

static void goalScored(int teamNum, Sprite *scorer)
{
    assert(scorer);

    swos.goalScored = 1;
    swos.runSlower = 1;
    swos.lastTeamScoredNumber = teamNum;
    swos.lastPlayerScored = scorer;
    swos.currentScorer = scorer;

    auto teamGame = scorer->teamNumber == 1 ? &swos.topTeamInGame : &swos.bottomTeamInGame;
    auto team = swos.topTeamData.inGameTeamPtr.asAligned() == teamGame ? &swos.topTeamData : &swos.bottomTeamData;

    swos.lastTeamScored = team;

    if (!bumpTeamGoals(teamNum) || swos.playingPenalties)
        return;

    auto goalType = GoalType::kRegular;
    swos.goalTypeScored = static_cast<int>(GoalType::kRegular);

    if (scorer->teamNumber != teamNum) {
        goalType = GoalType::kOwnGoal;
        swos.goalTypeScored = static_cast<int>(GoalType::kOwnGoal);
    } else if (swos.penalty) {
        goalType = GoalType::kPenalty;
    }

    registerScorer(*scorer, teamNum, goalType);
}

// in:
//      D0  - number of team that scored (1 or 2)
//      A0 -> in-game sprite of the player that scored
//
// Updates goal variables. Puts name of scorer and minute into scorers list.
// If the goal list is too long, it will be broken into up to 8 lines.
// Each line is one sprite. Skips result sprites updating if it's penalty shootout.
//
void SWOS::GoalScored()
{
    goalScored(D0.asWord(), A0.as<Sprite *>());
}
