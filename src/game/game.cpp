#include "game.h"
#include "log.h"
#include "gameLoop.h"
#include "windowManager.h"
#include "render.h"
#include "audio.h"
#include "music.h"
#include "chants.h"
#include "comments.h"
#include "sfx.h"
#include "options.h"
#include "controls.h"
#include "amigaMode.h"
#include "gameControls.h"
#include "keyBuffer.h"
#include "dump.h"
#include "util.h"
#include "replays.h"
#include "pitch.h"
#include "bench.h"
#include "updateBench.h"
#include "sprites.h"
#include "gameSprites.h"
#include "gameTime.h"
#include "playerNameDisplay.h"
#include "spinningLogo.h"
#include "result.h"
#include "stats.h"
#include "random.h"
#include "menus.h"
#include "drawMenu.h"
#include "versusMenu.h"
#include "stadiumMenu.h"

constexpr int kWheelZoomFrames = 6;

static TeamGame m_topTeamSaved;
static TeamGame m_bottomTeamSaved;

static bool m_gamePaused;

static bool m_blockZoom;
static int m_zoomFrames;

static void saveTeams();
static void restoreTeams();
static void cancelGame();
static void initializeIngameTeamsAndStartGame(TeamFile *team1, TeamFile *team2, int minSubs, int maxSubs, int paramD7);
static void processPostGameData(TeamFile *team1, TeamFile *team2, int paramD7);
static void initPlayerCardChance();
static void determineStartingTeamAndTeamPlayingUp();
static void initPitchBallFactors();
static void initGameVariables();

// Initializes everything except the sprite graphics, which are needed for the stadium menu.
void initMatch(TeamGame *topTeam, TeamGame *bottomTeam, bool saveOrRestoreTeams)
{
    saveOrRestoreTeams ? saveTeams() : restoreTeams();

    initMatchSprites(topTeam, bottomTeam);
    setPitchTypeAndNumber();
    loadPitch();
    //InitAdvertisements

    if (!replayingNow()) {
        swos.topTeamPtr = topTeam;
        swos.bottomTeamPtr = bottomTeam;

        initPlayerCardChance();

        swos.gameRandValue = SWOS::rand();

        A4 = topTeam;
        invokeWithSaved68kRegisters(ApplyTeamTactics);
        A4 = bottomTeam;
        invokeWithSaved68kRegisters(ApplyTeamTactics);

        determineStartingTeamAndTeamPlayingUp();

//arrange pitch type pattern coloring
        initPitchBallFactors();
        initGameVariables();

        initDisplaySprites();
        resetGameTime();
        resetResult(topTeam->teamName, bottomTeam->teamName);
        initStats();
//    InitAnimatedPatterns();
        StartingMatch();

        // Let the assembly state machine handle everything:
        // - ST_STARTING_GAME handles walk-on animation via InitPlayersBeforeEnteringPitch
        // - Players are placed OFF-SCREEN (x+591, y+449) and walk onto the pitch
        // - Player presses fire or timer expires -> PrepareForInitialKick is called internally
        // - Formation positions are calculated dynamically by SetPlayerWithNoBallDestination
        // DO NOT modify player positions or game state here - it breaks the flow!

        if (!swos.g_trainingGame)
            swos.showFansCounter = 100;

        loadCommentary();
    }

    loadSoundEffects();
    loadIntroChant();

    m_blockZoom = false;
    m_zoomFrames = 0;
}

void initializeIngameTeams(int minSubs, int maxSubs, TeamFile *team1, TeamFile *team2)
{
    swos.gameMinSubstitutes = minSubs;
    swos.gameMaxSubstitutes = maxSubs;
    swos.gameTeam1 = team1;
    swos.gameTeam2 = team2;

    A0 = team1;
    D0 = maxSubs;
    SetupTeamSubstitutes();

    A0 = team2;
    D0 = maxSubs;
    SetupTeamSubstitutes();

    swos.teamsLoaded = 0;
    swos.poolplyrLoaded = 0;

    swos.team1Computer = 0;
    swos.team2Computer = 0;

    if (team1->teamControls == kComputerTeam)
        swos.team1Computer = -1;
    if (team2->teamControls == kComputerTeam)
        swos.team2Computer = -1;

    D0 = team1->prShirtType;
    D1 = team1->prStripesColor;
    D2 = team1->prBasicColor;
    D3 = team1->prShortsColor;
    D4 = team1->prSocksColor;
    A1 = &swos.topTeamInGame;
    SetTeamSecondaryColors();

    D0 = team2->prShirtType;
    D1 = team2->prStripesColor;
    D2 = team2->prBasicColor;
    D3 = team2->prShortsColor;
    D4 = team2->prSocksColor;
    A1 = &swos.bottomTeamInGame;
    SetTeamSecondaryColors();

    A2 = team1;
    A3 = team2;
    SetInGameTeamsPrimaryColors();

    if (swos.g_gameType != kGameTypeCareer && swos.g_allPlayerTeamsEqual) {
        GetAveragePlayerPriceInSelectedTeams();
        swos.averagePlayerPrice = D0;
    }

    A2 = team1;
    A4 = &swos.topTeamInGame;
    A6 = &swos.team1AppPercent;
    InitInGameTeamStructure();

    A2 = team2;
    A4 = &swos.bottomTeamInGame;
    A6 = &swos.team2AppPercent;
    InitInGameTeamStructure();

    if (swos.isGameFriendly) {
        swos.team1NumAllowedInjuries = 4;
        swos.team2NumAllowedInjuries = 4;
    } else {
        int maxInjuries = swos.g_gameType == kGameTypeCareer ? 4 : 2;

        D0 = -1;
        D1 = 0;
        A0 = team1;
        GetNumberOfAvailablePlayers();

        swos.team1NumAllowedInjuries = std::max(0, maxInjuries - D6.asWord());

        D0 = -1;
        D1 = 0;
        A0 = team2;
        GetNumberOfAvailablePlayers();

        swos.team2NumAllowedInjuries = std::max(0, maxInjuries - D6.asWord());
    }

    if (showPreMatchMenus()) {
        showVersusMenu(&swos.topTeamInGame, &swos.bottomTeamInGame, swos.gameName, swos.gameRound, []() {
            loadStadiumSprites(&swos.topTeamInGame, &swos.bottomTeamInGame);
        });
    }

    if (!swos.isGameFriendly && swos.g_gameType != kGameTypeDiyCompetition)
        swos.gameLengthInGame = 0;
    else
        swos.gameLengthInGame = swos.g_gameLength;
}

void matchEnded()
{
    finishCurrentReplay();
}

void startMainGameLoop()
{
    startFadingOutMusic();

    initGameControls();
    initNewReplay();
    updateCursor(true);

    gameLoop(&swos.topTeamInGame, &swos.bottomTeamInGame);

    updateCursor(false);
}

void checkGlobalKeyboardShortcuts(SDL_Scancode scancode, bool pressed)
{
    static SDL_Scancode lastScancode;

    switch (scancode) {
    case SDL_SCANCODE_F1:
        // preserve alt-F1, ultra fast exit from SWOS (actually meant for invoking the debugger ;))
        if (pressed && (SDL_GetModState() & KMOD_ALT)) {
            logInfo("Shutting down via keyboard shortcut...");
            std::exit(EXIT_SUCCESS);
        }
        break;
    case SDL_SCANCODE_F2:
        if (pressed && scancode != lastScancode)
            makeScreenshot();
        break;
    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_KP_ENTER:
        {
            auto mod = SDL_GetModState();
            if (pressed && (mod & KMOD_ALT))
                (mod & KMOD_SHIFT) ? toggleFullScreenMode() : toggleBorderlessMaximizedMode();

            updateCursor(isMatchRunning());
        }
        break;
    default:
        break;
    }

    lastScancode = pressed ? scancode : SDL_SCANCODE_UNKNOWN;
}

bool checkGameKeys()
{
    bool zoomChanged = checkZoomKeys();

    auto key = getKey();

    if (key == SDL_SCANCODE_UNKNOWN || testForPlayerKeys(key))
        return zoomChanged;

    if (key == SDL_SCANCODE_D) {
        toggleDebugOutput();
        return zoomChanged;
    }

    if (isGamePaused()) {
        if (key == SDL_SCANCODE_P && !inBench())
            m_gamePaused = false;
    } else if (showingUserRequestedStats()) {
        if (key == SDL_SCANCODE_S)
            hideStats();
    } else if (!replayingNow()) {
        switch (key) {
        case SDL_SCANCODE_P:
            if (!inBench())
                togglePause();
            break;
        case SDL_SCANCODE_H:
            if (swos.gameState == GameState::kResultAfterTheGame)
                requestFadeAndReplayHighlights();
            break;
        case SDL_SCANCODE_R:
            if (!inBench() && swos.gameState != GameState::kResultAfterTheGame &&
                swos.gameState != GameState::kResultOnHalftime)
                requestFadeAndInstantReplay();
            break;
        case SDL_SCANCODE_S:
            if (swos.gameStatePl != GameState::kInProgress && !inBench() &&
                (swos.gameState < GameState::kStartingGame || swos.gameState > GameState::kGameEnded))
                toggleStats();
            break;
        case SDL_SCANCODE_SPACE:
            {
                constexpr Uint32 kSaveReplayRequestCooldown = 1'500;
                static Uint32 lastSaveRequestTime;
                auto now = SDL_GetTicks();
                if (now - lastSaveRequestTime > kSaveReplayRequestCooldown) {
                    requestFadeAndSaveReplay();
                    lastSaveRequestTime = now;
                }
            }
            break;
        case SDL_SCANCODE_F8:
            toggleMuteCommentary();
            break;
        case SDL_SCANCODE_F9:
            enableSpinningLogo(!spinningLogoEnabled());
            break;
        case SDL_SCANCODE_F10:
            {
                bool chantsEnabled = areCrowdChantsEnabled();
                setCrowdChantsEnabled(!chantsEnabled);
            }
            break;
        case SDL_SCANCODE_PAGEUP:
            if (swos.bottomTeamData.playerNumber || swos.bottomTeamData.playerCoachNumber)
                requestBench2();
            break;
        case SDL_SCANCODE_PAGEDOWN:
            if (swos.topTeamData.playerNumber || swos.topTeamData.playerCoachNumber)
                requestBench1();
            break;
        case SDL_SCANCODE_ESCAPE:
            cancelGame();
            break;
        default:
            break;
        }
    }

    return zoomChanged;
}

bool checkZoomKeys()
{
    constexpr auto kZoomStep = .25f;

    int wheelDirection = mouseWheelAmount();
    if (wheelDirection)
        m_zoomFrames = wheelDirection > 0 ? kWheelZoomFrames : -kWheelZoomFrames;

    auto keys = SDL_GetKeyboardState(nullptr);
    bool controlHeld = keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL];
    bool shiftHeld = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];

    bool zoomInRequested = keys[SDL_SCANCODE_KP_PLUS] || m_zoomFrames > 0;
    bool zoomOutRequested = keys[SDL_SCANCODE_KP_MINUS] || m_zoomFrames < 0;

    bool zoomKeysHeld = zoomInRequested || zoomOutRequested;
    bool resetToDefault = keys[SDL_SCANCODE_KP_MULTIPLY] != 0;

    if (resetToDefault) {
        return resetZoom();
    } else if ((controlHeld || shiftHeld) && zoomKeysHeld) {
        if (m_blockZoom)
            return false;
        m_blockZoom = true;
    } else {
        m_blockZoom = false;
    }

    if (m_zoomFrames)
        m_zoomFrames > 0 ? m_zoomFrames-- : m_zoomFrames++;

    auto step = controlHeld ? kZoomStep : 0;

    if (zoomInRequested)
        return zoomIn(step);
    else if (zoomOutRequested)
        return zoomOut(step);
    else
        return false;
}

void updateCursor(bool matchRunning)
{
    bool fullScreen = getWindowMode() != kModeWindow;
    bool disableCursor = matchRunning && fullScreen && !gotMousePlayer();
    SDL_ShowCursor(disableCursor ? SDL_DISABLE : SDL_ENABLE);
}

bool isGamePaused()
{
    return m_gamePaused;
}

void pauseTheGame()
{
    m_gamePaused = true;
}

void togglePause()
{
    m_gamePaused = !m_gamePaused;
}

static void saveTeams()
{
    m_topTeamSaved = swos.topTeamInGame;
    m_bottomTeamSaved = swos.bottomTeamInGame;
}

static void restoreTeams()
{
    swos.topTeamInGame = m_topTeamSaved;
    swos.bottomTeamInGame = m_bottomTeamSaved;
}

static void rigTheScoreForPlayerToLose(int playerNo)
{
    bool team1Player = playerNo == 1;
    auto& loserTotalGoals = team1Player ? swos.team1TotalGoals : swos.team2TotalGoals;
    auto& loserStatGoals = team1Player ? swos.statsTeam1Goals : swos.statsTeam1Goals;
    auto& loserStatGoalsCopy = team1Player ? swos.statsTeam1GoalsCopy : swos.statsTeam1GoalsCopy;
    auto& winnerTotalGoals = team1Player ? swos.team2TotalGoals : swos.team1TotalGoals;
    auto& winnerStatGoals = team1Player ? swos.statsTeam2Goals : swos.statsTeam1Goals;
    auto& winnerStatsGoalsCopy = team1Player ? swos.statsTeam2GoalsCopy : swos.statsTeam1GoalsCopy;

    winnerTotalGoals += 5;
    winnerStatGoals += 5;
    while (loserTotalGoals >= winnerTotalGoals) {
        winnerTotalGoals++;
        winnerStatGoals++;
    }

    D0 = playerNo;
    D5 = winnerTotalGoals - loserTotalGoals;
    AssignFakeGoalsToScorers();

    winnerStatsGoalsCopy = winnerStatGoals;
    loserStatGoalsCopy = loserStatGoals;
}

static void cancelGame()
{
    if (swos.playGame) {
        swos.playGame = 0;
        if (swos.gameState == GameState::kInProgress ||
            swos.gameState != GameState::kPlayersGoingToShower && swos.gameState != GameState::kResultAfterTheGame) {
            swos.stateGoal = 0;
            auto team1 = &swos.topTeamData;
            auto team2 = &swos.bottomTeamData;
            if (swos.topTeamData.teamNumber != 1)
                std::swap(team1, team2);

            bool team1Cpu = !team1->playerNumber && !team1->isPlCoach;
            bool team2Cpu = !team2->playerNumber && !team2->isPlCoach;
            bool playerVsCpu = team1Cpu && !team2Cpu || !team1Cpu && team2Cpu;

            if (playerVsCpu && !gameAtZeroMinute()) {
                int teamNo = team1Cpu ? 2 : 1;
                rigTheScoreForPlayerToLose(teamNo);
            } else {
                swos.gameCanceled = 1;
            }
            swos.extraTimeState = 0;
            swos.penaltiesState = 0;
        }
    }
}

static void initializeIngameTeamsAndStartGame(TeamFile *team1, TeamFile *team2, int minSubs, int maxSubs, int paramD7)
{
    initializeIngameTeams(minSubs, maxSubs, team1, team2);
    saveCurrentMenuAndStartGameLoop();
    processPostGameData(team1, team2, paramD7);

    startMenuSong();
    enqueueMenuFadeIn();
}

static void processPostGameData(TeamFile *team1, TeamFile *team2, int paramD7)
{
    if (!swos.gameCanceled) {
        if (!swos.g_trainingGame) {
            A1 = team1;
            A2 = team2;
            D0 = swos.plg_D3_param;
            ProcessPostGameStats();

            if (paramD7 < 0) {
                return;
            } else {
                for (const auto& teamData : { std::make_pair(team1, &swos.topTeamInGame), std::make_pair(team2, &swos.bottomTeamInGame) }) {
                    auto teamFile = teamData.first;
                    auto teamGame = teamData.second;

                    A1 = teamGame;
                    A2 = teamFile;
                    D7 = paramD7;
                    UpdatePlayerGoalStats();

                    A1 = teamGame;
                    A2 = teamFile;
                    UpdatePlayerInjuries();

                    A1 = teamGame;
                    A2 = teamFile;
                    UpdatePlayerCareerStats();

                    A1 = teamGame;
                    A2 = teamFile;
                    UpdateCareerTeamPlayers();
                }
            }
        } else {
            swos.gameTeam1->andWith0xFE |= 1;
            swos.gameTeam2->andWith0xFE |= 1;

            A1 = &swos.topTeamInGame;
            A2 = team1;
            UpdatePlayerInjuries();

            A1 = &swos.bottomTeamInGame;
            A2 = team2;
            UpdatePlayerInjuries();
        }
    }
}

static void initPlayerCardChance()
{
    assert(swos.gameLengthInGame <= 3);

    // when the foul is made 4 bits (1-4) from currentGameTick are extracted and compared to this value;
    // if greater the player gets a card (yellow or red)
    static const int kPlayerCardChancesPerGameLength[16][4] = {
        4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 8, 9, 10,    // 3 min
        2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 6,     // 5 min
        1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4,     // 7 min
        1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3,     // 10 min
    };

    auto chanceTable = kPlayerCardChancesPerGameLength[swos.gameLengthInGame];

    int chanceIndex = (SWOS::rand() & 0x1e) >> 1;

    swos.playerCardChance = chanceTable[chanceIndex];
}

static void determineStartingTeamAndTeamPlayingUp()
{
    swos.teamPlayingUp = (SWOS::rand() & 1) + 1;
    swos.teamStarting = (SWOS::rand() & 1) + 1;
}

static void initPitchBallFactors()
{
    static const int kPitchBallSpeedInfluence[] = { -3, 4, 1, 0, 0, -1, -1 };
    static const int kPitchBallSpeedInfluenceAmiga[] = { -2, 2, 3, 0, 0, -1, -1 };
    static const int kBallSpeedBounceFactorTable[] = { 24, 80, 80, 72, 64, 40, 32 };
    static const int kBallBounceFactorTable[] = { 88, 112, 104, 104, 96, 88, 80 };

    int pitchType = getPitchType();
    assert(static_cast<size_t>(pitchType) <= 6);

    const auto& pitchBallSpeedInfluence = amigaModeActive() ? kPitchBallSpeedInfluenceAmiga : kPitchBallSpeedInfluence;
    swos.pitchBallSpeedFactor = pitchBallSpeedInfluence[pitchType];
    swos.ballSpeedBounceFactor = kBallSpeedBounceFactorTable[pitchType];
    swos.ballBounceFactor = kBallBounceFactorTable[pitchType];
}

static void initGameVariables()
{
    swos.playingPenalties = 0;
    swos.dontShowScorers = 0;
    swos.statsTimer = 0;
    swos.g_waitForPlayerToGoInTimer = 0;
    swos.g_substituteInProgress = 0;
    swos.statsTeam1Goals = 0;
    swos.team1GoalsDigit1 = 0;
    swos.team1GoalsDigit2 = 0;
    swos.statsTeam2Goals = 0;
    swos.team2GoalsDigit1 = 0;
    swos.team2GoalsDigit2 = 0;

    swos.team1TotalGoals = 0;
    swos.team2TotalGoals = 0;
    if (swos.secondLeg) {
        swos.team1TotalGoals = swos.team1GoalsFirstLeg;
        swos.team2TotalGoals = swos.team2GoalsFirstLeg;
    }

    swos.team1NumSubs = 0;
    swos.team2NumSubs = 0;

    memset(&swos.team1StatsData, 0, sizeof(TeamStatsData));
    memset(&swos.team2StatsData, 0, sizeof(TeamStatsData));

    memset((char *)&swos.topTeamData + 24, 0, sizeof(swos.topTeamData) - 24);
    memset((char *)&swos.bottomTeamData + 24, 0, sizeof(swos.bottomTeamData) - 24);

    swos.goalCounter = 0;
    swos.stateGoal = 0;

    swos.frameCount = 0;
    swos.cameraXVelocity = swos.cameraYVelocity = 0;

    swos.pl1Fire = 0;
    swos.pl2Fire = 0;

    swos.longFireFlag = swos.longFireTime = 0;

    swos.currentGameTick = 0;
    swos.currentTick = 0;

    swos.AI_turnDirection = 1;
}

// in:
//      D0 = 1
//      D1 = min substitutes
//      D2 = max substitutes
//      D3 = 0
//      D7 = -1
//      A1 -> team 1 (structures from file)
//      A2 -> team 2
//
void SWOS::InitializeInGameTeamsAndStartGame()
{
    swos.plg_D0_param = D0;
    swos.plg_D3_param = D3;

    invokeWithSaved68kRegisters([]() {
        auto topTeamFile = A1.as<TeamFile *>();
        auto bottomTeamFile = A2.as<TeamFile *>();
        int minSubs = D1.asWord();
        int maxSubs = D2.asWord();
        initializeIngameTeamsAndStartGame(topTeamFile, bottomTeamFile, minSubs, maxSubs, D7.asWord());
    });

    SwosVM::ax = swos.gameCanceled;
    SwosVM::flags.zero = !SwosVM::ax;
}

void SWOS::EndProgram()
{
    std::exit(EXIT_FAILURE);
}

// Fix crash when watching 2 CPU players with at least one top-class goalkeeper in the game.
// Goalkeeper skill is scaled to range 0..7 (in D0) but value clamping is skipped in CPU vs CPU mode.
void SWOS::FixTwoCPUsGameCrash()
{
    if (D0.asInt16() < 0)
        D0 = 0;
    if (D0.asInt16() > 7)
        D0 = 7;
}

// Helper function to stop all players on a team
static void stopPlayersOnTeam(TeamGeneralInfo* team)
{
    if (team->players.isNull())
        return;

    SwosDataPointer<Sprite>* spritesTable = team->players;

    for (int i = 0; i < 11; i++) {
        if (spritesTable[i].isNull())
            continue;
        
        Sprite* player = spritesTable[i];

        // Only stop players in normal state and not sent away
        if (player->state == PlayerState::kNormal && !player->sentAway) {
            player->destX = player->x.whole();
            player->destY = player->y.whole();
        }
    }
}

// Clear team state when stopping all players
static void clearTeamState(TeamGeneralInfo* team)
{
    team->ballInPlay = 0;
    team->ballOutOfPlay = 0;
    team->controlledPlayerSprite = nullptr;
    team->passToPlayerPtr = nullptr;
    team->passingBall = 0;
    team->passingToPlayer = 0;
    team->playerSwitchTimer = 0;
    team->passingKickingPlayer = nullptr;
    team->playerHasBall = 0;
    team->aiBallSpinDirection = 0;  // Reset spin direction
    team->currentAllowedDirection = static_cast<word>(-1);
    team->passKickTimer = 0;
    team->ballCanBeControlled = 0;
    team->goalkeeperJumpingRight = 0;
    team->goalkeeperJumpingLeft = 0;
    team->ballOutOfPlayOrKeeper = 0;
    team->goaliePlayingOrOut = 0;
    team->spinTimer = static_cast<word>(-1);
    team->ballControllingPlayerDirection = static_cast<word>(-1);
}

// C++ implementation of PrepareForInitialKick
// Sets ball to center, determines kickoff team, and prepares game state
void SWOS::PrepareForInitialKick()
{
    // Set ball position to center of pitch (336, 449)
    D1 = 336;
    D2 = 449;
    SetBallPosition();

    // Determine starting team direction
    TeamGeneralInfo* kickoffTeam;
    int16_t cameraDir;
    uint8_t turnFlags;

    if (swos.teamStarting == swos.teamPlayingUp) {
        // Upper team (playing upward) is starting, kicking south
        kickoffTeam = &swos.topTeamData;
        cameraDir = 4;      // south
        turnFlags = 0x7C;   // only allow E, SE, S, SW, W
    } else {
        // Lower team (playing downward) is starting, kicking north
        kickoffTeam = &swos.bottomTeamData;
        cameraDir = 0;      // north
        turnFlags = 0xC7;   // only allow E, NE, N, NW, W
    }

    swos.gameState = GameState::kPlayersGoingToInitialPositions;
    swos.breakCameraMode = -1;
    swos.gameStatePl = GameState::kStopped;
    swos.gameNotInProgressCounterWriteOnly = 0;
    swos.foulXCoordinate = 336;
    swos.foulYCoordinate = 449;
    swos.cameraDirection = cameraDir;
    swos.playerTurnFlags = turnFlags;
    swos.lastTeamPlayedBeforeBreak = kickoffTeam;
    swos.stoppageTimerTotal = 0;
    swos.stoppageTimerActive = 0;

    // Save camera state before stopping players
    swos.dseg_130FF9 = swos.cameraDirection;
    swos.lastPlayerTurnFlags = swos.playerTurnFlags;

    // Stop all players on both teams
    stopPlayersOnTeam(&swos.topTeamData);
    clearTeamState(&swos.topTeamData);
    swos.topTeamData.goalkeeperPlaying = 0;
    stopPlayersOnTeam(&swos.bottomTeamData);
    clearTeamState(&swos.bottomTeamData);
    swos.bottomTeamData.goalkeeperPlaying = 0;

    // Clear last played state to prevent stale goalkeeper claiming
    swos.lastTeamPlayed = nullptr;
    swos.lastPlayerPlayed = nullptr;

    swos.cameraXVelocity = 0;
    swos.cameraYVelocity = 0;
}

// C++ implementation of InitPlayersBeforeEnteringPitch
// Sets up walk-on positions for all players
void SWOS::InitPlayersBeforeEnteringPitch()
{
    RemoveReferee();

    // Walk-on starting coordinates (x, y pairs for 22 players)
    // First 11 pairs are for team playing up, next 11 for team playing down
    static const int16_t kTeamsStartingCoords[44] = {
        300, 69, 280, 46, 260, 34, 240, 24, 220, 16, 200, 9, 180, 3, 160, -2,
        140, -8, 120, -9, 100, -11,  // First team (11 players)
        300, -65, 280, -42, 260, -30, 240, -20, 220, -12, 200, -5, 180, 1, 160, 6,
        140, 12, 120, 13, 100, 15    // Second team (11 players)
    };

    // Determine which sprite table to use first based on teamPlayingUp
    SwosDataPointer<Sprite>* firstTeamSprites = (swos.teamPlayingUp == 1) ?
        swos.team1SpritesTable : swos.team2SpritesTable;
    SwosDataPointer<Sprite>* secondTeamSprites = (swos.teamPlayingUp == 2) ?
        swos.team1SpritesTable : swos.team2SpritesTable;

    const int16_t* coordPtr = kTeamsStartingCoords;
    bool isStartingGame = (swos.gameState == GameState::kStartingGame);

    // Process both teams
    for (int teamIdx = 0; teamIdx < 2; teamIdx++) {
        SwosDataPointer<Sprite>* teamSprites = (teamIdx == 0) ? firstTeamSprites : secondTeamSprites;

        for (int playerIdx = 0; playerIdx < 11; playerIdx++) {
            Sprite* player = teamSprites[playerIdx];
            if (!player)
                continue;

            // Get base coordinates from table
            int16_t baseX = *coordPtr++;
            int16_t baseY = *coordPtr++;

            // Apply walk-on offset (+591 for X, +449 for Y) to place players off-screen
            int16_t x = baseX + 591;
            int16_t y = baseY + 449;

            // Add small random variation to X (0-7)
            x += SWOS::rand() & 7;

            // Set player position (clear fractions to avoid position drift)
            player->x.set(x, 0);
            player->y.set(y, 0);
            player->z.set(0, 0);

            // Destination is same as position (player stands still initially)
            player->destX = x;
            player->destY = y;

            // Reset player state
            player->speed = 0;
            player->state = PlayerState::kNormal;
            player->playerDownTimer = 0;
            player->frameIndex = -1;
            player->cycleFramesTimer = 1;
            player->imageIndex = -1;
            player->direction = 0;
            player->onScreen = 1;

            // Only reset cards/injuries at game start (not halftime)
            if (isStartingGame) {
                player->sentAway = 0;
                player->cards = 0;
                player->injuryLevel = 0;
            }

            // Set animation table
            A1 = player;
            A0 = &swos.playerNormalStandingAnimTable;
            SetPlayerAnimationTable();
        }
    }
}

// C++ implementation of StopAllPlayers
// Saves camera state and stops all players on both teams
void SWOS::StopAllPlayers()
{
    // Save camera state
    swos.dseg_130FF9 = swos.cameraDirection;
    swos.lastPlayerTurnFlags = swos.playerTurnFlags;

    // Stop top team players and clear state
    stopPlayersOnTeam(&swos.topTeamData);
    clearTeamState(&swos.topTeamData);
    swos.topTeamData.goalkeeperPlaying = 0;

    // Stop bottom team players and clear state
    stopPlayersOnTeam(&swos.bottomTeamData);
    clearTeamState(&swos.bottomTeamData);
    swos.bottomTeamData.goalkeeperPlaying = 0;

    // Clear last played state to prevent stale goalkeeper claiming
    swos.lastTeamPlayed = nullptr;
    swos.lastPlayerPlayed = nullptr;
}

// C++ implementation of SetPlayerWithNoBallDestination
// Calculates destination for a player without the ball based on tactics
// in: A1 = player sprite, A6 = team general info
//     D6 = ball X, D7 = ball Y
void SWOS::SetPlayerWithNoBallDestination()
{
    Sprite* player = reinterpret_cast<Sprite*>(SwosVM::offsetToPtr(A1));
    TeamGeneralInfo* team = reinterpret_cast<TeamGeneralInfo*>(SwosVM::offsetToPtr(A6));

    // Get tactics for this team
    int tacticsIndex = team->tactics;
    TeamTactics* tactics = swos.g_tacticsTable[tacticsIndex];

    // Check for special game states (keeper's ball or goal out)
    if (swos.gameState == GameState::kKeeperHoldsTheBall ||
        swos.gameState == GameState::kGoalOutLeft ||
        swos.gameState == GameState::kGoalOutRight) {
        // Use ball-out-of-play tactics
        tacticsIndex = tactics->ballOutOfPlayTactics;
        tactics = swos.g_tacticsTable[tacticsIndex];
    }

    // Get player ordinal (1-11), convert to index (0-9 for outfield, skip GK)
    int playerOrdinal = player->playerOrdinal;
    int playerIndex = playerOrdinal - 2;  // -2 to skip goalkeeper (ordinal 1)

    if (playerIndex < 0) {
        // This is a goalkeeper - special positioning
        goto goalkeeper_positioning;
    }

    {
        // Get base quadrant offset
        int16_t quadrantXOffset = swos.playerXQuadrantOffset;
        int16_t quadrantYOffset = swos.playerYQuadrantOffset;

        int16_t destX, destY;
        uint8_t codedQuadrant;

        if (team == &swos.topTeamData) {
            // Top team uses ball quadrant directly
            int quadrant = swos.ballQuadrantIndex;
            codedQuadrant = tactics->positions[playerIndex].positions[quadrant];
        } else {
            // Bottom team inverts ball quadrant
            int invertedQuadrant = 34 - swos.ballQuadrantIndex;
            uint8_t pos = tactics->positions[playerIndex].positions[invertedQuadrant];
            codedQuadrant = 0xEF - pos;  // Invert the coded position
        }

        // Decode quadrant (x,y packed in nibbles)
        int xQuadrant = (codedQuadrant >> 4) & 0x0F;
        int yQuadrant = codedQuadrant & 0x0F;

        // Look up coordinates from quadrant tables
        destX = swos.playerXQuadrantsCoordinates[xQuadrant] + quadrantXOffset;
        destY = swos.playerYQuadrantCoordinates[yQuadrant] + quadrantYOffset;

        // Adjust X based on team (approach from different sides)
        if (team == &swos.topTeamData) {
            destX -= 4;  // Top team approaches from left
        } else {
            destX += 4;  // Bottom team approaches from right
        }

        // Clip to pitch boundaries
        if (destX < 81) destX = 81;
        if (destX > 590) destX = 590;
        if (destY < 129) destY = 129;
        if (destY > 769) destY = 769;

        player->destX = destX;
        player->destY = destY;
        return;
    }

goalkeeper_positioning:
    {
        // Goalkeeper positioning based on ball position
        int16_t ballX = D6.asWord();
        int16_t ballY = D7.asWord();

        // X positioning: scale ball X to goalkeeper movement range (285-387)
        int16_t gkLeftX = 285;
        int16_t gkRightX = 387;
        int16_t gkWidth = gkRightX - gkLeftX + 1;  // 103

        int relBallX = ballX - 81;  // Relative to left pitch border
        int gkDestX = (relBallX * gkWidth) / 510 + gkLeftX;

        player->destX = static_cast<int16_t>(gkDestX);

        // Y positioning: different ranges for top vs bottom goalkeeper
        int16_t gkTopY, gkBottomY;
        if (team == &swos.topTeamData) {
            gkTopY = 135;
            gkBottomY = 161;
        } else {
            gkTopY = 737;
            gkBottomY = 763;
        }

        int16_t gkHeight = gkBottomY - gkTopY + 1;
        int relBallY = ballY - 129;  // Relative to top pitch border
        int gkDestY = (relBallY * gkHeight) / 641 + gkTopY;

        player->destY = static_cast<int16_t>(gkDestY);
    }
}

// C++ implementation of SetBallPosition
// Stops ball and puts it at specified position (on ground)
// in: D1 = ball x, D2 = ball y
// Note: Must clear fractional parts to match assembly behavior (swos.asm:104136-104162)
void SWOS::SetBallPosition()
{
    int16_t x = D1.asWord();
    int16_t y = D2.asWord();

    swos.ballSprite.speed = 0;
    swos.ballSprite.x.set(x, 0);      // Clear fraction to 0 (matches assembly)
    swos.ballSprite.y.set(y, 0);      // Clear fraction to 0 (matches assembly)
    swos.ballSprite.z.set(0, 0);      // Set z to 0 with cleared fraction
    swos.ballSprite.destX = x;
    swos.ballSprite.destY = y;
    swos.ballSprite.deltaZ = FixedPoint(0);
    // Also clear deltas to ensure ball stops moving
    swos.ballSprite.deltaX = FixedPoint(0);
    swos.ballSprite.deltaY = FixedPoint(0);
}

// C++ implementation of FirstHalfJustEnded
// Transitions to halftime state when first half ends
void SWOS::FirstHalfJustEnded()
{
    swos.hideBall = 0;
    swos.stoppageEventTimer = 275;
    swos.gameState = GameState::kGoingToHalftime;
    swos.breakCameraMode = -1;
    swos.gameStatePl = GameState::kStopped;
    swos.gameNotInProgressCounterWriteOnly = 0;
    swos.cameraDirection = -1;
    swos.lastTeamPlayedBeforeBreak = &swos.topTeamData;
    swos.stoppageTimerTotal = 0;
    swos.stoppageTimerActive = 0;
    StopAllPlayers();
    swos.cameraXVelocity = 0;
    swos.cameraYVelocity = 0;
    swos.stateGoal = 0;
}

// C++ implementation of PlayersLeavingPitch
// Transitions to players going to shower state
void SWOS::PlayersLeavingPitch()
{
    swos.hideBall = 0;
    swos.stoppageEventTimer = 275;
    swos.gameState = GameState::kPlayersGoingToShower;
    swos.breakCameraMode = -1;
    swos.gameStatePl = GameState::kStopped;
    swos.gameNotInProgressCounterWriteOnly = 0;
    swos.cameraDirection = -1;
    swos.lastTeamPlayedBeforeBreak = &swos.topTeamData;
    swos.stoppageTimerTotal = 0;
    swos.stoppageTimerActive = 0;
    StopAllPlayers();
    swos.cameraXVelocity = 0;
    swos.cameraYVelocity = 0;
    swos.stateGoal = 0;
}

// C++ implementation of GoToHalftime
// Shows halftime result screen
void SWOS::GoToHalftime()
{
    // Put ball off-screen
    D1 = 1672;
    D2 = 449;
    SetBallPosition();

    swos.resultTimer = 30000;
    swos.timeVar = 32000;
    swos.stoppageEventTimer = 770;
    swos.gameState = GameState::kResultOnHalftime;
    swos.breakCameraMode = -1;
    swos.gameStatePl = GameState::kStopped;
    swos.gameNotInProgressCounterWriteOnly = 0;
    swos.cameraDirection = -1;
    swos.lastTeamPlayedBeforeBreak = &swos.topTeamData;
    swos.stoppageTimerTotal = 0;
    swos.stoppageTimerActive = 0;
    StopAllPlayers();
    swos.cameraXVelocity = 0;
    swos.cameraYVelocity = 0;
}

// C++ implementation of CheckIfGoalkeeperClaimedTheBall
// Handles the state when goalkeeper has claimed the ball vs other stopped states
void SWOS::CheckIfGoalkeeperClaimedTheBall()
{
    if (swos.gameState == GameState::kKeeperHoldsTheBall) {
        // Goalkeeper claimed the ball - set up for goal kick
        TeamGeneralInfo* team = swos.lastTeamPlayedBeforeBreak;
        A6 = team;

        // Get goalkeeper sprite (first in sprites table)
        SwosDataPointer<Sprite>* spritesTable = team->players;
        Sprite* goalkeeper = spritesTable[0];
        A1 = goalkeeper;

        // Set A2 to ball sprite - the original game fails to set this
        A2 = &swos.ballSprite;

        GoalkeeperClaimedTheBall();
    } else {
        // Game stopped for other reason
        swos.breakCameraMode = -1;
        swos.gameStatePl = GameState::kStopped;
        swos.gameNotInProgressCounterWriteOnly = 0;
        swos.stoppageTimerTotal = 0;
        swos.stoppageTimerActive = 0;
        StopAllPlayers();
        swos.cameraXVelocity = 0;
        swos.cameraYVelocity = 0;
    }
}
