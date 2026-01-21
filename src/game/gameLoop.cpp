#include "gameLoop.h"
#include "game.h"
#include "render.h"
#include "windowManager.h"
#include "timer.h"
#include "gameControls.h"
#include "spinningLogo.h"
#include "playerNameDisplay.h"
#include "sprites.h"
#include "gameSprites.h"
#include "updateSprite.h"
#include "updatePlayers.h"
#include "gameTime.h"
#include "bench.h"
#include "drawBench.h"
#include "result.h"
#include "stats.h"
#include "pitch.h"
#include "pitchConstants.h"
#include "referee.h"
#include "menus.h"
#include "menuBackground.h"
#include "camera.h"
#include "controls.h"
#include "keyBuffer.h"
#include "audio.h"
#include "music.h"
#include "comments.h"
#include "sfx.h"
#include "chants.h"
#include "options.h"
#include "replays.h"
#include "stadiumMenu.h"
#include "replayExitMenu.h"
#include "util.h"
#include "FixedPoint.h"
#include "ball.h"

constexpr FixedPoint kGameEndCameraX = 176;
constexpr FixedPoint kGameEndCameraY = 80;

static bool m_fadeAndSaveReplay;
static bool m_fadeAndInstantReplay;
static bool m_fadeAndReplayHighlights;

static bool m_doFadeIn;

static bool m_playingMatch;

#ifdef SWOS_TEST
static std::function<void()> m_gameLoopStartHook = []() {};
static std::function<void()> m_gameLoopEndHook = []() {};
#endif

static void initGameLoop();
static void drawFrame(bool recordingEnabled);
static void gameFadeOut();
static void gameFadeIn();
static void updateTimers();
static void handlePauseAndStats();
static void handleKeys();
static void coreGameUpdate();
static void handleHighlightsAndReplays();
static void gameOver();
static bool gameEnded(TeamGame *topTeam, TeamGame *bottomTeam);
static void pausedLoop();
static void showStatsLoop();
static void loadCrowdChantSampleIfNeeded();
static void initGoalSprites();

void gameLoop(TeamGame *topTeam, TeamGame *bottomTeam)
{
    logInfo("=== gameLoop START ===");
    logInfo("topTeam=%p, bottomTeam=%p", (void*)topTeam, (void*)bottomTeam);
    flushLog();

    swos.playGame = 1;

    logInfo("Calling showStadiumScreenAndFadeOutMusic...");
    flushLog();
    showStadiumScreenAndFadeOutMusic(topTeam, bottomTeam, swos.gameMaxSubstitutes);
    logInfo("showStadiumScreenAndFadeOutMusic completed");
    flushLog();

    do {
        logInfo("Calling initGameLoop...");
        flushLog();
        initGameLoop();
        logInfo("initGameLoop completed");
        flushLog();
        markFrameStartTime();

        // the really real main game loop ;)
        while (true) {
#ifdef SWOS_TEST
            m_gameLoopStartHook();
#endif
            loadCrowdChantSampleIfNeeded();
            updateTimers();
            handlePauseAndStats();

            handleKeys();

            coreGameUpdate();
            drawFrame(true);

            bool skipUpdate = false;

            if (m_doFadeIn) {
                gameFadeIn();
                m_doFadeIn = false;
                skipUpdate = true;
                swos.currentGameTick = 0;   // for tests (to remain original game compatible)
                swos.lastGameTick = 0;
            }

            handleHighlightsAndReplays();

            if (!swos.playGame) {
                gameFadeOut();
                break;
            }

            if (!skipUpdate)
                updateScreen(true);

#ifdef SWOS_TEST
            m_gameLoopEndHook();
#endif
        }
    } while (!gameEnded(topTeam, bottomTeam));

    m_playingMatch = false;
}

void showStadiumScreenAndFadeOutMusic(TeamGame *topTeam, TeamGame *bottomTeam, int maxSubstitutes)
{
    logInfo("showStadiumScreenAndFadeOutMusic: showPreMatchMenus=%d", showPreMatchMenus());
    flushLog();

    if (showPreMatchMenus()) {
        logInfo("Showing stadium menu with initMatch callback...");
        flushLog();
        showStadiumMenu(topTeam, bottomTeam, maxSubstitutes, [&]() {
            logInfo("Stadium menu callback - calling initMatch...");
            flushLog();
            initMatch(topTeam, bottomTeam, true);
            logInfo("Stadium menu callback - initMatch returned");
            flushLog();
        });
        logInfo("showStadiumMenu returned");
        flushLog();
    } else {
        logInfo("No pre-match menus, calling initMatch directly...");
        flushLog();
        initMatch(topTeam, bottomTeam, true);
        logInfo("initMatch returned (no menus path)");
        flushLog();
    }

    logInfo("Waiting for music to fade out...");
    waitForMusicToFadeOut();
    logInfo("Music fade out complete");
    flushLog();
}

void requestFadeAndSaveReplay()
{
    m_fadeAndSaveReplay = true;
}

void requestFadeAndInstantReplay()
{
    m_fadeAndInstantReplay = true;
}

void requestFadeAndReplayHighlights()
{
    m_fadeAndReplayHighlights = true;
}

bool isMatchRunning()
{
    return m_playingMatch;
}

#ifdef SWOS_TEST
void setGameLoopStartHook(std::function<void()> hook)
{
    m_gameLoopStartHook = hook ? hook : []() {};
}

void setGameLoopEndHook(std::function<void()> hook)
{
    m_gameLoopEndHook = hook ? hook : []() {};
}
#endif

static void initGameLoop()
{
    logInfo("=== initGameLoop START ===");
    flushLog();

    m_playingMatch = true;
    m_doFadeIn = true;

    logInfo("Calling initGameAudio...");
    initGameAudio();
    logInfo("initGameAudio completed");

    logInfo("Calling playCrowdNoise...");
    playCrowdNoise();
    logInfo("playCrowdNoise completed");

    if (swos.playGame) {
        logInfo("Calling playFansChant4lSample...");
        playFansChant4lSample();
        logInfo("playFansChant4lSample completed");
    }

    flushKeyBuffer();
    logInfo("Key buffer flushed");

    logInfo("Calling initBenchBeforeMatch...");
    flushLog();
    initBenchBeforeMatch();
    logInfo("initBenchBeforeMatch completed");
    flushLog();

    swos.trainingGameCopy = swos.g_trainingGame;
    swos.gameCanceled = 0;
    swos.saveHighlightScene = 0;
    swos.instantReplayFlag = 0;

    m_fadeAndSaveReplay = false;
    m_fadeAndInstantReplay = false;
    m_fadeAndReplayHighlights = false;

    setCameraToInitialPosition();

    ReadTimerDelta();

    waitForKeyboardAndMouseIdle();

    swos.currentGameTick = 0;
    swos.lastFrameTicks = 0;

    initFrameTicks();

    resetGameControls();
}

static void drawFrame(bool recordingEnabled)
{
    setReplayRecordingEnabled(recordingEnabled);
    float xOffset, yOffset;
    std::tie(xOffset, yOffset) = drawPitchAtCurrentCamera();
    startNewHighlightsFrame();
    drawSprites(xOffset, yOffset);
    drawBench(xOffset, yOffset);
    drawPlayerName();
    drawGameTime();
    drawStatsIfNeeded();
    drawResult();
    drawSpinningLogo();
}

static void gameFadeOut()
{
    fadeOut([]() { drawFrame(false); });
}

static void gameFadeIn()
{
    fadeIn([]() { drawFrame(false); });
}

static void updateTimers()
{
    ReadTimerDelta();

    swos.frameCount++;

    if (swos.spaceReplayTimer)
        swos.spaceReplayTimer--;
}

static void handlePauseAndStats()
{
    pausedLoop();

    if (statsEnqueued())
        showStatsLoop();
}

static void handleKeys()
{
    processControlEvents();
    checkGameKeys();
}

static void coreGameUpdate()
{
    static int frameCounter = 0;
    frameCounter++;

    // Log every 60 frames (approximately once per second)
    bool shouldLog = (frameCounter % 60 == 1);

    if (shouldLog) {
        logInfo("--- coreGameUpdate frame %d ---", frameCounter);
        logInfo("gameState=%d gameStatePl=%d", static_cast<int>(swos.gameState), static_cast<int>(swos.gameStatePl));
    }

    moveCamera();
    playEnqueuedSamples();
    updateGameTime();
    initGoalSprites();
    UpdateCameraBreakMode();    // convert

    if (!updateFireBlocked()) {
        auto team = selectTeamForUpdate();
        if (shouldLog) {
            logInfo("Updating team controls and players for team %p", (void*)team);
        }
        updateTeamControls(team);
        updatePlayers(team);    // main game engine update
        postUpdateTeamControls(team);
    }

    // Log ball state before UpdateBall for debugging teleport issue
    static int ballDebugCounter = 0;
    if (ballDebugCounter < 120) {  // Log first 120 frames
        logInfo("PRE-UpdateBall[%d]: pos=(%d,%d) dest=(%d,%d) speed=%d deltaX=%d deltaY=%d gameState=%d",
                ballDebugCounter,
                swos.ballSprite.x.whole(), swos.ballSprite.y.whole(),
                swos.ballSprite.destX, swos.ballSprite.destY,
                swos.ballSprite.speed,
                swos.ballSprite.deltaX.whole(), swos.ballSprite.deltaY.whole(),
                static_cast<int>(swos.gameState));
    }

    UpdateBall();

    if (ballDebugCounter < 120) {
        logInfo("POST-UpdateBall[%d]: pos=(%d,%d) dest=(%d,%d) speed=%d deltaX=%d deltaY=%d",
                ballDebugCounter,
                swos.ballSprite.x.whole(), swos.ballSprite.y.whole(),
                swos.ballSprite.destX, swos.ballSprite.destY,
                swos.ballSprite.speed,
                swos.ballSprite.deltaX.whole(), swos.ballSprite.deltaY.whole());
        ballDebugCounter++;
        flushLog();
    }

    if (shouldLog) {
        logInfo("Ball: X=%d Y=%d Z=%d", swos.ballSprite.x.whole(), swos.ballSprite.y.whole(), swos.ballSprite.z.whole());
        flushLog();
    }



    movePlayers();
    updateReferee();
    updateCornerFlags();
    updateSpinningLogo();
    //ManageAdvertisements();
    DoGoalkeeperSprites();
    UpdateControlledPlayerNumbers();
    MarkPlayer();
    updateCurrentPlayerName();
    updateBookedPlayerNumberSprite();
    updateResult();
    SWOS::DrawAnimatedPatterns(); // remove/re-implement
    updateBench();
    updateStatistics();
}

static void handleHighlightsAndReplays()
{
    // TODO: remove when UpdateCameraBreakMode() is converted
    if (swos.saveHighlightScene) {
        saveHighlightScene();
        swos.saveHighlightScene = 0;
    }

    if (m_fadeAndSaveReplay) {
        gameFadeOut();
        saveHighlightScene();
        m_doFadeIn = true;
        m_fadeAndSaveReplay = false;
    }

    // remove instantReplayFlag when UpdateCameraBreakMode() is converted
    if (m_fadeAndInstantReplay || swos.instantReplayFlag) {
        gameFadeOut();

        bool userRequested = true;
        if (swos.instantReplayFlag) {
            userRequested = false;
            swos.instantReplayFlag = 0;
            swos.goalCameraMode = 0;
        }

        playInstantReplay(userRequested);

        m_doFadeIn = true;
        m_fadeAndInstantReplay = false;
        swos.goalCameraMode = 0;    // is this necessary?
    }

    if (m_fadeAndReplayHighlights) {
        gameFadeOut();
        playHighlights(true);
        m_doFadeIn = true;
        m_fadeAndReplayHighlights = false;
    }
}

// Executed when the match finishes.
static void gameOver()
{
    constexpr int kBallOffCourtX = 1672;

    gameFadeOut();

#ifdef SWOS_TEST
    setCameraX(FixedPoint(kGameEndCameraX.whole(), getCameraX().fraction()));
    setCameraY(FixedPoint(kGameEndCameraY.whole(), getCameraY().fraction()));
#else
    setCameraX(kGameEndCameraX);
    setCameraY(kGameEndCameraY);
#endif
    drawPitchAtCurrentCamera();

    D1 = kBallOffCourtX;
    D2 = kPitchCenterY;
    SWOS::SetBallPosition();

    swos.resultTimer = 30'000;
    swos.stoppageEventTimer = 1'650;
    swos.gameState = GameState::kResultAfterTheGame;
    swos.breakCameraMode = -1;
    swos.gameStatePl = GameState::kStopped;
    swos.cameraDirection = -1;
    swos.lastTeamPlayedBeforeBreak = &swos.topTeamData;
    swos.stoppageTimerTotal = 0;
    swos.stoppageTimerActive = 0;

    SWOS::StopAllPlayers();

    swos.cameraXVelocity = 0;
    swos.cameraYVelocity = 0;

    playEndGameCrowdSampleAndComment();

    m_doFadeIn = true;
}

void SWOS::GameOver()
{
    gameOver();
}

static bool gameEnded(TeamGame *topTeam, TeamGame *bottomTeam)
{
    stopAudio();
    matchEnded();
    refreshReplayGameData();
    setStandardMenuBackgroundImage();

    if (!swos.isGameFriendly || swos.g_trainingGame)
        return true;

    bool replaySelected = showReplayExitMenuAfterFriendly();
    if (!replaySelected)
        return true;

    swos.team1NumAllowedInjuries = 4;
    swos.team2NumAllowedInjuries = 4;

    swos.playGame = 1;

    initMatch(swos.topTeamPtr, swos.bottomTeamPtr, false);

    return false;
}

static void pausedLoop()
{
    int oldWidth = -1, oldHeight = -1;

    while (isGamePaused()) {
        processControlEvents();

        int width, height;
        std::tie(width, height) = getWindowSize();

        if (checkGameKeys() || width != oldWidth || height != oldHeight) {
            oldWidth = width;
            oldHeight = height;
            markFrameStartTime();
            drawFrame(false);
            updateScreen(true);
            continue;
        }

        auto events = getPlayerEvents(kPlayer1) | getPlayerEvents(kPlayer2);
        if (events & (kGameEventKick | kGameEventPause)) {
            togglePause();
            break;
        }

        SDL_Delay(100);

        // make sure to reset the timer or delta time will be heavily skewed after the pause
        markFrameStartTime();
    }
}

static void showStatsLoop()
{
    drawFrame(false);
    updateScreen();

    do {
        SDL_Delay(100);
        processControlEvents();
        checkGameKeys();
        if (isAnyPlayerFiring()) {
            swos.fireBlocked = 1;
            hideStats();
        }
    } while (showingUserRequestedStats());
}

static void loadCrowdChantSampleIfNeeded()
{
    if (swos.loadCrowdChantSampleFlag) {
        loadCrowdChantSample();
        swos.loadCrowdChantSampleFlag = 0;
    }
}

static void initGoalSprites()
{
    swos.goal1TopSprite.setImage(kTopGoalSprite);
    swos.goal2BottomSprite.setImage(kBottomGoalSprite);
}
