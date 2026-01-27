#include "portedFunctions.h"
#include "file.h"
#include "util.h"
#include "swos.h"
#include "options.h"
#include "init.h"
#include "game.h"
#include "gameLoop.h"
#include "sprites.h"
#include "controls.h"
#include "gameControls.h"
#include "replays.h"
#include "music.h"
#include "mainMenu.h"
#include "../stdinc.h"
#include "../util/random.h"
#include <cstdio>
#include <cstring>

PortedFunctionToggles g_portedToggles;
bool g_comparePortedFunctions = false;
bool g_breakOnMismatch = false;
int g_comparisonMismatchCount = 0;

static FILE* s_comparisonLogFile = nullptr;

static constexpr char kIniFilename[] =
#ifdef DEBUG
    "swos-debug.ini";
#else
    "swos.ini";
#endif

static const char kPortedFunctionsSection[] = "portedFunctions";

// Toggle info array for menu and INI access
static const ToggleInfo s_toggleInfo[] = {
    { "AI_SetControlsDirection", &g_portedToggles.useCpp_AI_SetControlsDirection },
    { "ResetBothTeamSpinTimers", &g_portedToggles.useCpp_ResetBothTeamSpinTimers },
    { "ThrowInDeadProc", &g_portedToggles.useCpp_ThrowInDeadProc },
    { "GetPlayerPointerFromShirtNumber", &g_portedToggles.useCpp_GetPlayerPointerFromShirtNumber },
    { "UpdateTeamShotChanceTable", &g_portedToggles.useCpp_UpdateTeamShotChanceTable },
    { "SetThrowInPlayerDestinationCoordinates", &g_portedToggles.useCpp_SetThrowInPlayerDestinationCoordinates },
    { "SetPlayerDowntimeAfterTackle", &g_portedToggles.useCpp_SetPlayerDowntimeAfterTackle },
    { "UpdatePlayerWithBall", &g_portedToggles.useCpp_UpdatePlayerWithBall },
    { "UpdateControllingPlayer", &g_portedToggles.useCpp_UpdateControllingPlayer },
    { "GoalkeeperCaughtTheBall", &g_portedToggles.useCpp_GoalkeeperCaughtTheBall },
    { "UpdateBallWithControllingGoalkeeper", &g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper },
    { "AttemptStaticHeader", &g_portedToggles.useCpp_AttemptStaticHeader },
    { "UpdatePlayerSpeed", &g_portedToggles.useCpp_UpdatePlayerSpeed },
    { "GoalkeeperJumping", &g_portedToggles.useCpp_GoalkeeperJumping },
    { "PlayerBeginTackling", &g_portedToggles.useCpp_PlayerBeginTackling },
    { "PlayersTackledTheBallStrong", &g_portedToggles.useCpp_PlayersTackledTheBallStrong },
    { "SetJumpHeaderHitAnimTable", &g_portedToggles.useCpp_SetJumpHeaderHitAnimTable },
    { "SetStaticHeaderDirection", &g_portedToggles.useCpp_SetStaticHeaderDirection },
    { "PlayerAttemptingJumpHeader", &g_portedToggles.useCpp_PlayerAttemptingJumpHeader },
    { "AI_Kick", &g_portedToggles.useCpp_AI_Kick },
    { "CalculateBallNextGroundXYPositions", &g_portedToggles.useCpp_CalculateBallNextGroundXYPositions },
    { "UpdateBallVariables", &g_portedToggles.useCpp_UpdateBallVariables },
    { "ApplyBallAfterTouch", &g_portedToggles.useCpp_ApplyBallAfterTouch },
    { "PlayerHittingStaticHeader", &g_portedToggles.useCpp_PlayerHittingStaticHeader },
    { "PlayerHittingJumpHeader", &g_portedToggles.useCpp_PlayerHittingJumpHeader },
    { "PlayerTackledTheBallWeak", &g_portedToggles.useCpp_PlayerTackledTheBallWeak },
    { "PlayerKickingBall", &g_portedToggles.useCpp_PlayerKickingBall },
    { "ShouldGoalkeeperDive", &g_portedToggles.useCpp_ShouldGoalkeeperDive },
    { "PlayerTacklingTestFoul", &g_portedToggles.useCpp_PlayerTacklingTestFoul },
    { "DoPass", &g_portedToggles.useCpp_DoPass },
    { "CalculateIfPlayerWinsBall", &g_portedToggles.useCpp_CalculateIfPlayerWinsBall },
};

static_assert(sizeof(s_toggleInfo) / sizeof(s_toggleInfo[0]) == kNumPortedFunctionToggles,
    "Toggle info array size mismatch");

const ToggleInfo* getPortedFunctionToggleInfo()
{
    return s_toggleInfo;
}

void loadPortedFunctionToggles()
{
    auto path = pathInRootDir(kIniFilename);
    logDebug("Loading ported function toggles from %s", path.c_str());

    CSimpleIniA ini(true);
    auto errorCode = ini.LoadFile(path.c_str());

    if (errorCode < 0) {
        logDebug("Could not load INI for ported functions (error %d), using defaults", errorCode);
        return;
    }

    for (const auto& toggle : s_toggleInfo) {
        *toggle.toggle = ini.GetBoolValue(kPortedFunctionsSection, toggle.name, true);
    }
}

void savePortedFunctionToggles()
{
    auto path = pathInRootDir(kIniFilename);
    logDebug("Saving ported function toggles to %s", path.c_str());

    CSimpleIniA ini(true);
    ini.LoadFile(path.c_str());  // Load existing content first

    ini.SetValue(kPortedFunctionsSection, nullptr, nullptr,
        "; Toggle between C++ ports (1) and original assembly (0) for debugging\n");

    for (const auto& toggle : s_toggleInfo) {
        ini.SetBoolValue(kPortedFunctionsSection, toggle.name, *toggle.toggle);
    }

    auto tmpFile = path + ".tmp";
    auto errorCode = ini.SaveFile(tmpFile.c_str());

    if (errorCode != SI_OK) {
        beep();
        logWarn("Failed to save ported function toggles, error code: %d", errorCode);
        return;
    }

    if (!renameFile(tmpFile.c_str(), path.c_str())) {
        beep();
        logWarn("Failed to rename temp INI file");
    }
}

// AI Comparison Mode Implementation

bool AICompareState::operator==(const AICompareState& other) const
{
    return currentAllowedDirection == other.currentAllowedDirection &&
           quickFire == other.quickFire &&
           normalFire == other.normalFire &&
           firePressed == other.firePressed &&
           fireThisFrame == other.fireThisFrame &&
           ofs134 == other.ofs134 &&
           ofs136 == other.ofs136 &&
           ofs138 == other.ofs138 &&
           AITimer == other.AITimer;
}

AICompareState captureAIState(void* teamPtr)
{
    auto team = reinterpret_cast<TeamGeneralInfo*>(teamPtr);
    AICompareState state;
    state.currentAllowedDirection = team->currentAllowedDirection;
    state.quickFire = team->quickFire;
    state.normalFire = team->normalFire;
    state.firePressed = team->firePressed;
    state.fireThisFrame = team->fireThisFrame;
    state.ofs134 = team->ofs134;
    state.ofs136 = team->ofs136;
    state.ofs138 = team->ofs138;
    state.AITimer = team->AITimer;
    return state;
}

void initComparisonLog()
{
    if (s_comparisonLogFile) {
        fclose(s_comparisonLogFile);
    }

    auto path = pathInRootDir("ai_comparison.log");
    s_comparisonLogFile = fopen(path.c_str(), "w");
    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "AI Comparison Log - ASM vs C++\n");
        fprintf(s_comparisonLogFile, "================================\n\n");
        fflush(s_comparisonLogFile);
        logDebug("Comparison log opened: %s", path.c_str());
    } else {
        logWarn("Failed to open comparison log: %s", path.c_str());
    }
    g_comparisonMismatchCount = 0;
}

void closeComparisonLog()
{
    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "\n================================\n");
        fprintf(s_comparisonLogFile, "Total mismatches: %d\n", g_comparisonMismatchCount);
        fclose(s_comparisonLogFile);
        s_comparisonLogFile = nullptr;
        logDebug("Comparison log closed. Total mismatches: %d", g_comparisonMismatchCount);
    }
}

void logAICompareMismatch(uint32_t frame, void* teamPtr,
                          const AICompareState& asmState,
                          const AICompareState& cppState,
                          const AICompareInputState* inputState)
{
    g_comparisonMismatchCount++;

    auto team = reinterpret_cast<TeamGeneralInfo*>(teamPtr);
    const char* teamName = (team == &swos.topTeamData) ? "TOP" : "BOTTOM";

    // Log to file if open
    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "MISMATCH #%d at frame %u, team %s:\n",
                g_comparisonMismatchCount, frame, teamName);

        // Log input state if provided (helps diagnose which code path was taken)
        if (inputState) {
            fprintf(s_comparisonLogFile, "  INPUT: spinTimer=%d, plVeryClose=%d, plClose=%d, aiRand=%u (odd=%d), hasPlayer=%d, playerDir=%d\n",
                    inputState->spinTimer, inputState->plVeryCloseToBall, inputState->plCloseToBall,
                    inputState->aiRandValue, inputState->aiRandValue & 1, inputState->hasControlledPlayer ? 1 : 0, inputState->playerDirection);
            fprintf(s_comparisonLogFile, "  INPUT: ofs134_init=%d, ofs136_init=%d, ofs138_init=%d, isPenalties=%d\n",
                    inputState->ofs134_initial, inputState->ofs136_initial, inputState->ofs138_initial, inputState->isPenalties ? 1 : 0);
            fprintf(s_comparisonLogFile, "  BALL: z=%d, deltaZ=%d, ctrlDist=%u, goalDistSqr=%d\n",
                    inputState->ballZ, inputState->ballDeltaZ, inputState->ctrlBallDist, inputState->goalDistSqr);
            fprintf(s_comparisonLogFile, "  OPPONENT: ctrlDist=%u, passDist=%u\n",
                    inputState->oppCtrlDist, inputState->oppPassDist);

            // Derive expected path based on input state
            if (inputState->spinTimer >= 0) {
                // Should enter handleBallAfterTouch
                bool shouldNoAfterTouch = inputState->isPenalties || (inputState->aiRandValue & 1) == 0 || inputState->ofs138_initial == 0;
                if (shouldNoAfterTouch && inputState->ofs136_initial == 1) {
                    fprintf(s_comparisonLogFile, "  EXPECTED: direction=-1 (noAfterTouch, ofs136==1)\n");
                } else if (shouldNoAfterTouch) {
                    fprintf(s_comparisonLogFile, "  EXPECTED: longKick path (noAfterTouch, ofs136=%d)\n", inputState->ofs136_initial);
                } else {
                    fprintf(s_comparisonLogFile, "  EXPECTED: spin path (ofs138=%d)\n", inputState->ofs138_initial);
                }
            } else {
                fprintf(s_comparisonLogFile, "  EXPECTED: handleBallControl or handleNoBallNearby (spinTimer<0)\n");
            }
        }

        if (asmState.currentAllowedDirection != cppState.currentAllowedDirection) {
            fprintf(s_comparisonLogFile, "  currentAllowedDirection: ASM=%d, C++=%d\n",
                    asmState.currentAllowedDirection, cppState.currentAllowedDirection);
        }
        if (asmState.quickFire != cppState.quickFire) {
            fprintf(s_comparisonLogFile, "  quickFire: ASM=%d, C++=%d\n",
                    asmState.quickFire, cppState.quickFire);
        }
        if (asmState.normalFire != cppState.normalFire) {
            fprintf(s_comparisonLogFile, "  normalFire: ASM=%d, C++=%d\n",
                    asmState.normalFire, cppState.normalFire);
        }
        if (asmState.firePressed != cppState.firePressed) {
            fprintf(s_comparisonLogFile, "  firePressed: ASM=%d, C++=%d\n",
                    asmState.firePressed, cppState.firePressed);
        }
        if (asmState.fireThisFrame != cppState.fireThisFrame) {
            fprintf(s_comparisonLogFile, "  fireThisFrame: ASM=%d, C++=%d\n",
                    asmState.fireThisFrame, cppState.fireThisFrame);
        }
        if (asmState.ofs134 != cppState.ofs134) {
            fprintf(s_comparisonLogFile, "  ofs134 (field_84): ASM=%d, C++=%d\n",
                    asmState.ofs134, cppState.ofs134);
        }
        if (asmState.ofs136 != cppState.ofs136) {
            fprintf(s_comparisonLogFile, "  ofs136 (afterTouch): ASM=%d, C++=%d\n",
                    asmState.ofs136, cppState.ofs136);
        }
        if (asmState.ofs138 != cppState.ofs138) {
            fprintf(s_comparisonLogFile, "  ofs138 (spinDir): ASM=%d, C++=%d\n",
                    asmState.ofs138, cppState.ofs138);
        }
        if (asmState.AITimer != cppState.AITimer) {
            fprintf(s_comparisonLogFile, "  AITimer: ASM=%d, C++=%d\n",
                    asmState.AITimer, cppState.AITimer);
        }
        fprintf(s_comparisonLogFile, "\n");
        fflush(s_comparisonLogFile);
    }

    // Also log to console (first 10 mismatches only to avoid spam)
    if (g_comparisonMismatchCount <= 10) {
        logWarn("AI MISMATCH #%d at frame %u, team %s (see ai_comparison.log)",
                g_comparisonMismatchCount, frame, teamName);
    }

    // Trigger debugger break if requested
    if (g_breakOnMismatch) {
#ifdef _MSC_VER
        __debugbreak();
#else
        __builtin_trap();
#endif
    }
}

// ============== Kick/Shot Comparison Implementation ==============

KickCompareState captureKickState(uint32_t teamPtr)
{
    KickCompareState state;
    // Ball sprite is at a fixed offset (from g_memByte + 256)
    constexpr uint32_t kBallSpriteOffset = 523128;
    constexpr int kSpriteSpeed = 68;
    constexpr int kSpriteDeltaZ = 56;
    constexpr int kSpriteDestX = 60;
    constexpr int kSpriteDestY = 62;
    constexpr int kTeamPassInProgress = 80;
    constexpr int kTeamSpinTimer = 88;

    auto readMem16 = [](uint32_t addr) -> int16_t {
        return *reinterpret_cast<int16_t*>(SwosVM::g_memByte + addr);
    };
    auto readMem32 = [](uint32_t addr) -> int32_t {
        return *reinterpret_cast<int32_t*>(SwosVM::g_memByte + addr);
    };

    state.ballSpeed = readMem16(kBallSpriteOffset + kSpriteSpeed);
    state.ballDeltaZ = readMem32(kBallSpriteOffset + kSpriteDeltaZ);
    state.ballDestX = readMem16(kBallSpriteOffset + kSpriteDestX);
    state.ballDestY = readMem16(kBallSpriteOffset + kSpriteDestY);
    state.passInProgress = readMem16(teamPtr + kTeamPassInProgress);
    state.spinTimer = readMem16(teamPtr + kTeamSpinTimer);

    return state;
}

void logKickMismatch(uint32_t frame, const char* functionName,
                     const KickCompareState& asmState,
                     const KickCompareState& cppState,
                     const KickInputState* inputState)
{
    static int s_kickMismatchCount = 0;
    s_kickMismatchCount++;

    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "KICK MISMATCH #%d at frame %u in %s:\n",
                s_kickMismatchCount, frame, functionName);

        if (inputState) {
            fprintf(s_comparisonLogFile, "  INPUT: dir=%d, ordinal=%d, ballXY=(%d,%d), team=%s\n",
                    inputState->playerDirection, inputState->playerOrdinal,
                    inputState->ballX, inputState->ballY,
                    inputState->isTopTeam ? "TOP" : "BOTTOM");
            fprintf(s_comparisonLogFile, "  INPUT: shootingSkill=%d, finishingSkill=%d, gameState=%d, gameStatePl=%d\n",
                    inputState->shootingSkill, inputState->finishingSkill,
                    inputState->gameState, inputState->gameStatePl);
        }

        if (asmState.ballSpeed != cppState.ballSpeed) {
            fprintf(s_comparisonLogFile, "  ballSpeed: ASM=%d, C++=%d (diff=%d)\n",
                    asmState.ballSpeed, cppState.ballSpeed,
                    cppState.ballSpeed - asmState.ballSpeed);
        }
        if (asmState.ballDeltaZ != cppState.ballDeltaZ) {
            fprintf(s_comparisonLogFile, "  ballDeltaZ: ASM=%d (0x%08X), C++=%d (0x%08X)\n",
                    asmState.ballDeltaZ, asmState.ballDeltaZ,
                    cppState.ballDeltaZ, cppState.ballDeltaZ);
        }
        if (asmState.ballDestX != cppState.ballDestX) {
            fprintf(s_comparisonLogFile, "  ballDestX: ASM=%d, C++=%d\n",
                    asmState.ballDestX, cppState.ballDestX);
        }
        if (asmState.ballDestY != cppState.ballDestY) {
            fprintf(s_comparisonLogFile, "  ballDestY: ASM=%d, C++=%d\n",
                    asmState.ballDestY, cppState.ballDestY);
        }
        if (asmState.passInProgress != cppState.passInProgress) {
            fprintf(s_comparisonLogFile, "  passInProgress: ASM=%d, C++=%d\n",
                    asmState.passInProgress, cppState.passInProgress);
        }
        if (asmState.spinTimer != cppState.spinTimer) {
            fprintf(s_comparisonLogFile, "  spinTimer: ASM=%d, C++=%d\n",
                    asmState.spinTimer, cppState.spinTimer);
        }
        fprintf(s_comparisonLogFile, "\n");
        fflush(s_comparisonLogFile);
    }

    if (s_kickMismatchCount <= 10) {
        logWarn("KICK MISMATCH #%d at frame %u in %s (see ai_comparison.log)",
                s_kickMismatchCount, frame, functionName);
    }
}

// ============== Ball State Save/Restore for Comparison ==============

// Complete ball sprite state for save/restore
struct BallSpriteState {
    FixedPoint x, y, z;
    int16_t direction;
    int16_t speed;
    FixedPoint deltaX, deltaY, deltaZ;
    int16_t destX, destY;
};

// Team kick-related state
struct TeamKickState {
    int16_t passInProgress;
    int16_t spinTimer;
    int16_t leftSpin;
    int16_t rightSpin;
    int16_t longPass;
    int16_t longSpinPass;
    int16_t passKickTimer;
    int16_t ballInPlay;
};

static BallSpriteState saveBallState()
{
    BallSpriteState state;
    state.x = swos.ballSprite.x;
    state.y = swos.ballSprite.y;
    state.z = swos.ballSprite.z;
    state.direction = swos.ballSprite.direction;
    state.speed = swos.ballSprite.speed;
    state.deltaX = swos.ballSprite.deltaX;
    state.deltaY = swos.ballSprite.deltaY;
    state.deltaZ = swos.ballSprite.deltaZ;
    state.destX = swos.ballSprite.destX;
    state.destY = swos.ballSprite.destY;
    return state;
}

static void restoreBallState(const BallSpriteState& state)
{
    swos.ballSprite.x = state.x;
    swos.ballSprite.y = state.y;
    swos.ballSprite.z = state.z;
    swos.ballSprite.direction = state.direction;
    swos.ballSprite.speed = state.speed;
    swos.ballSprite.deltaX = state.deltaX;
    swos.ballSprite.deltaY = state.deltaY;
    swos.ballSprite.deltaZ = state.deltaZ;
    swos.ballSprite.destX = state.destX;
    swos.ballSprite.destY = state.destY;
}

static TeamKickState saveTeamKickState(TeamGeneralInfo* team)
{
    TeamKickState state;
    state.passInProgress = team->passInProgress;
    state.spinTimer = team->spinTimer;
    state.leftSpin = team->leftSpin;
    state.rightSpin = team->rightSpin;
    state.longPass = team->longPass;
    state.longSpinPass = team->longSpinPass;
    state.passKickTimer = team->passKickTimer;
    state.ballInPlay = team->ballInPlay;
    return state;
}

static void restoreTeamKickState(TeamGeneralInfo* team, const TeamKickState& state)
{
    team->passInProgress = state.passInProgress;
    team->spinTimer = state.spinTimer;
    team->leftSpin = state.leftSpin;
    team->rightSpin = state.rightSpin;
    team->longPass = state.longPass;
    team->longSpinPass = state.longSpinPass;
    team->passKickTimer = state.passKickTimer;
    team->ballInPlay = state.ballInPlay;
}

// External assembly function
extern void PlayerKickingBall();

// C++ ported function
namespace PlayerFunctions {
    extern void PlayerKickingBall();
}

void ComparePlayerKickingBall(bool useCpp)
{
    using namespace SwosVM;

    TeamGeneralInfo* teamPtr = reinterpret_cast<TeamGeneralInfo*>(offsetToPtr(A6));

    // Skip comparison if not enabled
    if (!g_comparePortedFunctions) {
        if (useCpp) {
            PlayerFunctions::PlayerKickingBall();
        } else {
            ::PlayerKickingBall();
        }
        return;
    }

    // Get kicking player for diagnostics
    Sprite* kickingPlayer = reinterpret_cast<Sprite*>(offsetToPtr(A1));

    // Save original state BEFORE running either version
    auto ballStateOriginal = saveBallState();
    auto teamKickStateOriginal = saveTeamKickState(teamPtr);
    auto rngStateOriginal = SWOS::saveRngState();

    // Save registers that PlayerKickingBall might modify
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;
    Register savedA4 = A4, savedA5 = A5;

    // Run C++ version first
    PlayerFunctions::PlayerKickingBall();

    // Capture C++ output
    KickCompareState cppOutput = captureKickState(ptrToOffset(teamPtr));

    // Save C++ final state (for restore if useCpp)
    auto ballStateCpp = saveBallState();
    auto teamKickStateCpp = saveTeamKickState(teamPtr);
    auto rngStateCpp = SWOS::saveRngState();
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppD4 = D4, cppD5 = D5, cppD6 = D6, cppD7 = D7;
    Register cppA0 = A0, cppA1 = A1, cppA2 = A2, cppA3 = A3;
    Register cppA4 = A4, cppA5 = A5;

    // Capture input state for diagnostic logging (from ORIGINAL state)
    KickInputState inputState = {};
    if (kickingPlayer) {
        inputState.playerDirection = kickingPlayer->direction;
        inputState.playerOrdinal = kickingPlayer->playerOrdinal;

        // Get player skills from the team's player data
        if (kickingPlayer->playerOrdinal >= 1 && kickingPlayer->playerOrdinal <= 11) {
            TeamGame* teamGame = teamPtr->inGameTeamPtr.asPtr();
            if (teamGame) {
                int playerIdx = kickingPlayer->playerOrdinal - 1;
                PlayerGame* player = &teamGame->players[playerIdx];
                inputState.shootingSkill = player->shooting;
                inputState.finishingSkill = player->finishing;
                inputState.passingSkill = player->passing;
                inputState.ballControlSkill = player->ballControl;
            }
        }
    }
    inputState.ballX = ballStateOriginal.x.whole();
    inputState.ballY = ballStateOriginal.y.whole();
    inputState.isTopTeam = (teamPtr == &swos.topTeamData);
    inputState.gameState = static_cast<int16_t>(swos.gameState);
    inputState.gameStatePl = static_cast<int16_t>(swos.gameStatePl);
    inputState.fireCounter = teamPtr->fireCounter;
    inputState.quickFire = teamPtr->quickFire;
    inputState.normalFire = teamPtr->normalFire;

    // Restore to original state
    restoreBallState(ballStateOriginal);
    restoreTeamKickState(teamPtr, teamKickStateOriginal);
    SWOS::restoreRngState(rngStateOriginal);
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;
    A4 = savedA4; A5 = savedA5;

    // Run ASM version from the same initial state
    ::PlayerKickingBall();

    // Capture ASM output
    KickCompareState asmOutput = captureKickState(ptrToOffset(teamPtr));

    // Restore state to what the selected version (C++ or ASM) would produce
    if (useCpp) {
        // Restore to C++ state
        restoreBallState(ballStateCpp);
        restoreTeamKickState(teamPtr, teamKickStateCpp);
        SWOS::restoreRngState(rngStateCpp);
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        D4 = cppD4; D5 = cppD5; D6 = cppD6; D7 = cppD7;
        A0 = cppA0; A1 = cppA1; A2 = cppA2; A3 = cppA3;
        A4 = cppA4; A5 = cppA5;
    }
    // If using ASM, the state is already from ASM run

    // Log mismatch if C++ differs from ASM
    if (asmOutput != cppOutput) {
        logKickMismatch(swos.frameCount, "PlayerKickingBall", asmOutput, cppOutput, &inputState);
    }
}

// ============== Ball Physics Comparison ==============

// Ball physics state for comparison (more detailed than KickCompareState)
struct BallPhysicsState {
    // Position
    int32_t x_raw, y_raw, z_raw;
    // Velocity
    int32_t deltaX_raw, deltaY_raw, deltaZ_raw;
    // Other
    int16_t speed;
    int16_t destX, destY;
    int16_t direction;

    bool operator==(const BallPhysicsState& other) const {
        return x_raw == other.x_raw && y_raw == other.y_raw && z_raw == other.z_raw &&
               deltaX_raw == other.deltaX_raw && deltaY_raw == other.deltaY_raw && deltaZ_raw == other.deltaZ_raw &&
               speed == other.speed && destX == other.destX && destY == other.destY &&
               direction == other.direction;
    }
    bool operator!=(const BallPhysicsState& other) const { return !(*this == other); }
};

static BallPhysicsState captureBallPhysicsState()
{
    BallPhysicsState state;
    state.x_raw = swos.ballSprite.x.raw();
    state.y_raw = swos.ballSprite.y.raw();
    state.z_raw = swos.ballSprite.z.raw();
    state.deltaX_raw = swos.ballSprite.deltaX.raw();
    state.deltaY_raw = swos.ballSprite.deltaY.raw();
    state.deltaZ_raw = swos.ballSprite.deltaZ.raw();
    state.speed = swos.ballSprite.speed;
    state.destX = swos.ballSprite.destX;
    state.destY = swos.ballSprite.destY;
    state.direction = swos.ballSprite.direction;
    return state;
}

static void logBallPhysicsMismatch(uint32_t frame, const char* functionName,
                                    const BallPhysicsState& asmState,
                                    const BallPhysicsState& cppState)
{
    static int s_ballPhysMismatchCount = 0;
    s_ballPhysMismatchCount++;

    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "BALL PHYSICS MISMATCH #%d at frame %u in %s:\n",
                s_ballPhysMismatchCount, frame, functionName);

        if (asmState.x_raw != cppState.x_raw || asmState.y_raw != cppState.y_raw || asmState.z_raw != cppState.z_raw) {
            fprintf(s_comparisonLogFile, "  position: ASM=(%d.%04X, %d.%04X, %d.%04X) C++=(%d.%04X, %d.%04X, %d.%04X)\n",
                    asmState.x_raw >> 16, asmState.x_raw & 0xFFFF,
                    asmState.y_raw >> 16, asmState.y_raw & 0xFFFF,
                    asmState.z_raw >> 16, asmState.z_raw & 0xFFFF,
                    cppState.x_raw >> 16, cppState.x_raw & 0xFFFF,
                    cppState.y_raw >> 16, cppState.y_raw & 0xFFFF,
                    cppState.z_raw >> 16, cppState.z_raw & 0xFFFF);
        }
        if (asmState.deltaX_raw != cppState.deltaX_raw || asmState.deltaY_raw != cppState.deltaY_raw || asmState.deltaZ_raw != cppState.deltaZ_raw) {
            fprintf(s_comparisonLogFile, "  velocity: ASM=(0x%08X, 0x%08X, 0x%08X) C++=(0x%08X, 0x%08X, 0x%08X)\n",
                    asmState.deltaX_raw, asmState.deltaY_raw, asmState.deltaZ_raw,
                    cppState.deltaX_raw, cppState.deltaY_raw, cppState.deltaZ_raw);
        }
        if (asmState.speed != cppState.speed) {
            fprintf(s_comparisonLogFile, "  speed: ASM=%d, C++=%d (diff=%d)\n",
                    asmState.speed, cppState.speed, cppState.speed - asmState.speed);
        }
        if (asmState.destX != cppState.destX || asmState.destY != cppState.destY) {
            fprintf(s_comparisonLogFile, "  dest: ASM=(%d,%d) C++=(%d,%d)\n",
                    asmState.destX, asmState.destY, cppState.destX, cppState.destY);
        }
        if (asmState.direction != cppState.direction) {
            fprintf(s_comparisonLogFile, "  direction: ASM=%d, C++=%d\n",
                    asmState.direction, cppState.direction);
        }
        fprintf(s_comparisonLogFile, "\n");
        fflush(s_comparisonLogFile);
    }

    if (s_ballPhysMismatchCount <= 10) {
        logWarn("BALL PHYSICS MISMATCH #%d at frame %u in %s (see ai_comparison.log)",
                s_ballPhysMismatchCount, frame, functionName);
    }
}

// External ASM functions
extern void ApplyBallAfterTouch();
extern void UpdateBallVariables();
extern void CalculateBallNextGroundXYPositions();

// C++ ported functions
namespace PlayerFunctions {
    extern void ApplyBallAfterTouch();
    extern void UpdateBallVariables();
    extern void CalculateBallNextGroundXYPositions();
}

void CompareApplyBallAfterTouch(bool useCpp)
{
    using namespace SwosVM;

    if (!g_comparePortedFunctions) {
        if (useCpp) PlayerFunctions::ApplyBallAfterTouch();
        else ::ApplyBallAfterTouch();
        return;
    }

    // Save original state - must include team state since A6 points to team and spinTimer is modified
    auto ballStateOriginal = saveBallState();
    auto rngStateOriginal = SWOS::saveRngState();
    auto topTeamStateOriginal = saveTeamKickState(&swos.topTeamData);
    auto bottomTeamStateOriginal = saveTeamKickState(&swos.bottomTeamData);
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;
    Register savedA4 = A4, savedA5 = A5, savedA6 = A6;

    // Run C++ version
    PlayerFunctions::ApplyBallAfterTouch();
    BallPhysicsState cppOutput = captureBallPhysicsState();

    // Save C++ state
    auto ballStateCpp = saveBallState();
    auto rngStateCpp = SWOS::saveRngState();
    auto topTeamStateCpp = saveTeamKickState(&swos.topTeamData);
    auto bottomTeamStateCpp = saveTeamKickState(&swos.bottomTeamData);
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppD4 = D4, cppD5 = D5, cppD6 = D6, cppD7 = D7;
    Register cppA0 = A0, cppA1 = A1, cppA2 = A2, cppA3 = A3;
    Register cppA4 = A4, cppA5 = A5, cppA6 = A6;

    // Restore original state
    restoreBallState(ballStateOriginal);
    SWOS::restoreRngState(rngStateOriginal);
    restoreTeamKickState(&swos.topTeamData, topTeamStateOriginal);
    restoreTeamKickState(&swos.bottomTeamData, bottomTeamStateOriginal);
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;
    A4 = savedA4; A5 = savedA5; A6 = savedA6;

    // Run ASM version
    ::ApplyBallAfterTouch();
    BallPhysicsState asmOutput = captureBallPhysicsState();

    // Restore to selected version's state
    if (useCpp) {
        restoreBallState(ballStateCpp);
        SWOS::restoreRngState(rngStateCpp);
        restoreTeamKickState(&swos.topTeamData, topTeamStateCpp);
        restoreTeamKickState(&swos.bottomTeamData, bottomTeamStateCpp);
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        D4 = cppD4; D5 = cppD5; D6 = cppD6; D7 = cppD7;
        A0 = cppA0; A1 = cppA1; A2 = cppA2; A3 = cppA3;
        A4 = cppA4; A5 = cppA5; A6 = cppA6;
    }

    if (asmOutput != cppOutput) {
        logBallPhysicsMismatch(swos.frameCount, "ApplyBallAfterTouch", asmOutput, cppOutput);
    }
}

void CompareUpdateBallVariables(bool useCpp)
{
    using namespace SwosVM;

    if (!g_comparePortedFunctions) {
        if (useCpp) PlayerFunctions::UpdateBallVariables();
        else ::UpdateBallVariables();
        return;
    }

    // Save original state
    auto ballStateOriginal = saveBallState();
    auto rngStateOriginal = SWOS::saveRngState();
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;

    // Run C++ version
    PlayerFunctions::UpdateBallVariables();
    BallPhysicsState cppOutput = captureBallPhysicsState();

    // Save C++ state
    auto ballStateCpp = saveBallState();
    auto rngStateCpp = SWOS::saveRngState();
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppD4 = D4, cppD5 = D5, cppD6 = D6, cppD7 = D7;
    Register cppA0 = A0, cppA1 = A1, cppA2 = A2, cppA3 = A3;

    // Restore original state
    restoreBallState(ballStateOriginal);
    SWOS::restoreRngState(rngStateOriginal);
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;

    // Run ASM version
    ::UpdateBallVariables();
    BallPhysicsState asmOutput = captureBallPhysicsState();

    // Restore to selected version's state
    if (useCpp) {
        restoreBallState(ballStateCpp);
        SWOS::restoreRngState(rngStateCpp);
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        D4 = cppD4; D5 = cppD5; D6 = cppD6; D7 = cppD7;
        A0 = cppA0; A1 = cppA1; A2 = cppA2; A3 = cppA3;
    }

    if (asmOutput != cppOutput) {
        logBallPhysicsMismatch(swos.frameCount, "UpdateBallVariables", asmOutput, cppOutput);
    }
}

void CompareCalculateBallNextGroundXYPositions(bool useCpp)
{
    using namespace SwosVM;

    if (!g_comparePortedFunctions) {
        if (useCpp) PlayerFunctions::CalculateBallNextGroundXYPositions();
        else ::CalculateBallNextGroundXYPositions();
        return;
    }

    // Save original state
    auto ballStateOriginal = saveBallState();
    auto rngStateOriginal = SWOS::saveRngState();
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;

    // Run C++ version
    PlayerFunctions::CalculateBallNextGroundXYPositions();
    BallPhysicsState cppOutput = captureBallPhysicsState();

    // Save C++ state
    auto ballStateCpp = saveBallState();
    auto rngStateCpp = SWOS::saveRngState();
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppD4 = D4, cppD5 = D5, cppD6 = D6, cppD7 = D7;
    Register cppA0 = A0, cppA1 = A1, cppA2 = A2, cppA3 = A3;

    // Restore original state
    restoreBallState(ballStateOriginal);
    SWOS::restoreRngState(rngStateOriginal);
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;

    // Run ASM version
    ::CalculateBallNextGroundXYPositions();
    BallPhysicsState asmOutput = captureBallPhysicsState();

    // Restore to selected version's state
    if (useCpp) {
        restoreBallState(ballStateCpp);
        SWOS::restoreRngState(rngStateCpp);
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        D4 = cppD4; D5 = cppD5; D6 = cppD6; D7 = cppD7;
        A0 = cppA0; A1 = cppA1; A2 = cppA2; A3 = cppA3;
    }

    if (asmOutput != cppOutput) {
        logBallPhysicsMismatch(swos.frameCount, "CalculateBallNextGroundXYPositions", asmOutput, cppOutput);
    }
}

// ============== Goalkeeper Comparison Implementation ==============

// External ASM functions
extern void ShouldGoalkeeperDive();
extern void GoalkeeperJumping();

// C++ ported functions
namespace PlayerFunctions {
    extern void ShouldGoalkeeperDive();
    extern void GoalkeeperJumping();
}

// Sprite offsets needed for goalkeeper state (must match playerFunctions.cpp)
constexpr int kSpriteSpeed = 44;       // speed (2 bytes)
constexpr int kSpriteDestX = 58;       // destX (2 bytes)
constexpr int kSpriteDestY = 60;       // destY (2 bytes)
constexpr int kSpritePlayerState = 12;
constexpr int kSpritePlayerDownTimer = 13;
constexpr int kSpriteX = 30;           // FixedPoint x (4 bytes)
constexpr int kSpriteY = 34;           // FixedPoint y (4 bytes)
constexpr int kSpriteDeltaY = 50;      // FixedPoint deltaY (4 bytes)
constexpr int kSpriteBallDistance = 74;
constexpr int kTeamControlledPlDirection = 56;
constexpr int kTeamGoalkeeperDivingRight = 80;
constexpr uint32_t kBallNotHighZ = 524756;
constexpr uint32_t kBallDefensiveX = 524746;
constexpr uint32_t kPlayingPenalties_GK = 455942;
constexpr uint32_t kPenalty_GK = 455924;

static GoalkeeperDiveCompareState captureGoalkeeperDiveState()
{
    GoalkeeperDiveCompareState state;
    state.shouldDive = static_cast<int16_t>(D0 & 0xFFFF);
    return state;
}

static GoalkeeperJumpCompareState captureGoalkeeperJumpState(uint32_t goalkeeperSprite, uint32_t teamPtr)
{
    using namespace SwosVM;
    GoalkeeperJumpCompareState state;

    auto readMem16 = [](uint32_t addr) -> int16_t {
        return *reinterpret_cast<int16_t*>(g_memByte + addr);
    };
    auto readMem8 = [](uint32_t addr) -> uint8_t {
        return g_memByte[addr];
    };

    state.speed = readMem16(goalkeeperSprite + kSpriteSpeed);
    state.destX = readMem16(goalkeeperSprite + kSpriteDestX);
    state.destY = readMem16(goalkeeperSprite + kSpriteDestY);
    state.playerState = readMem8(goalkeeperSprite + kSpritePlayerState);
    state.playerDownTimer = readMem8(goalkeeperSprite + kSpritePlayerDownTimer);
    state.controlledPlDirection = readMem16(teamPtr + kTeamControlledPlDirection);
    state.goalkeeperDivingRight = readMem16(teamPtr + kTeamGoalkeeperDivingRight);

    return state;
}

void logGoalkeeperDiveMismatch(uint32_t frame,
                                const GoalkeeperDiveCompareState& asmState,
                                const GoalkeeperDiveCompareState& cppState,
                                const GoalkeeperDiveInputState* inputState)
{
    static int s_gkDiveMismatchCount = 0;
    s_gkDiveMismatchCount++;
    g_comparisonMismatchCount++;

    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "GOALKEEPER DIVE MISMATCH #%d at frame %u:\n",
                s_gkDiveMismatchCount, frame);

        if (inputState) {
            fprintf(s_comparisonLogFile, "  INPUT: ballPos=(%d,%d) gkPos=(%d,%d) yDiff=%d\n",
                    inputState->ballX, inputState->ballY,
                    inputState->goalkeeperX, inputState->goalkeeperY,
                    inputState->yDiff);
            fprintf(s_comparisonLogFile, "  INPUT: ballSpeed=%d ballDeltaY=0x%08X ballDefX=%d\n",
                    inputState->ballSpeed, inputState->ballDeltaY, inputState->ballDefensiveX);
            fprintf(s_comparisonLogFile, "  INPUT: team=%s isPenalty=%d\n",
                    inputState->isTopTeam ? "TOP" : "BOTTOM",
                    inputState->isPenalty ? 1 : 0);
        }

        fprintf(s_comparisonLogFile, "  shouldDive: ASM=%d (%s), C++=%d (%s)\n",
                asmState.shouldDive, asmState.shouldDive ? "DIVE" : "NO DIVE",
                cppState.shouldDive, cppState.shouldDive ? "DIVE" : "NO DIVE");
        fprintf(s_comparisonLogFile, "\n");
        fflush(s_comparisonLogFile);
    }

    if (s_gkDiveMismatchCount <= 20) {
        logWarn("GK DIVE MISMATCH #%d at frame %u: ASM=%d C++=%d (see ai_comparison.log)",
                s_gkDiveMismatchCount, frame, asmState.shouldDive, cppState.shouldDive);
    }
}

void logGoalkeeperJumpMismatch(uint32_t frame,
                                const GoalkeeperJumpCompareState& asmState,
                                const GoalkeeperJumpCompareState& cppState,
                                const GoalkeeperJumpInputState* inputState)
{
    static int s_gkJumpMismatchCount = 0;
    s_gkJumpMismatchCount++;
    g_comparisonMismatchCount++;

    if (s_comparisonLogFile) {
        fprintf(s_comparisonLogFile, "GOALKEEPER JUMP MISMATCH #%d at frame %u:\n",
                s_gkJumpMismatchCount, frame);

        if (inputState) {
            fprintf(s_comparisonLogFile, "  INPUT: ballPos=(%d,%d) gkPos=(%d,%d)\n",
                    inputState->ballX, inputState->ballY,
                    inputState->goalkeeperX, inputState->goalkeeperY);
            fprintf(s_comparisonLogFile, "  INPUT: ballDist=%u ballNotHighZ=%d d1=%d d3Dir=%d team=%s\n",
                    inputState->ballDistance, inputState->ballNotHighZ,
                    inputState->d1Value, inputState->d3Direction,
                    inputState->isTopTeam ? "TOP" : "BOTTOM");
        }

        if (asmState.speed != cppState.speed) {
            fprintf(s_comparisonLogFile, "  speed: ASM=%d, C++=%d\n", asmState.speed, cppState.speed);
        }
        if (asmState.destX != cppState.destX || asmState.destY != cppState.destY) {
            fprintf(s_comparisonLogFile, "  dest: ASM=(%d,%d), C++=(%d,%d)\n",
                    asmState.destX, asmState.destY, cppState.destX, cppState.destY);
        }
        if (asmState.playerState != cppState.playerState) {
            fprintf(s_comparisonLogFile, "  playerState: ASM=%d, C++=%d\n",
                    asmState.playerState, cppState.playerState);
        }
        if (asmState.playerDownTimer != cppState.playerDownTimer) {
            fprintf(s_comparisonLogFile, "  playerDownTimer: ASM=%d, C++=%d\n",
                    asmState.playerDownTimer, cppState.playerDownTimer);
        }
        if (asmState.controlledPlDirection != cppState.controlledPlDirection) {
            fprintf(s_comparisonLogFile, "  controlledPlDirection: ASM=%d, C++=%d\n",
                    asmState.controlledPlDirection, cppState.controlledPlDirection);
        }
        if (asmState.goalkeeperDivingRight != cppState.goalkeeperDivingRight) {
            fprintf(s_comparisonLogFile, "  goalkeeperDivingRight: ASM=%d, C++=%d\n",
                    asmState.goalkeeperDivingRight, cppState.goalkeeperDivingRight);
        }
        fprintf(s_comparisonLogFile, "\n");
        fflush(s_comparisonLogFile);
    }

    if (s_gkJumpMismatchCount <= 20) {
        logWarn("GK JUMP MISMATCH #%d at frame %u (see ai_comparison.log)", s_gkJumpMismatchCount, frame);
    }
}

void CompareShouldGoalkeeperDive(bool useCpp)
{
    using namespace SwosVM;

    if (!g_comparePortedFunctions) {
        if (useCpp) PlayerFunctions::ShouldGoalkeeperDive();
        else ::ShouldGoalkeeperDive();
        return;
    }

    // Capture input state for diagnostics
    auto readMem16 = [](uint32_t addr) -> int16_t {
        return *reinterpret_cast<int16_t*>(g_memByte + addr);
    };
    auto readMem32 = [](uint32_t addr) -> int32_t {
        return *reinterpret_cast<int32_t*>(g_memByte + addr);
    };

    GoalkeeperDiveInputState inputState = {};
    inputState.ballX = readMem16(A2 + kSpriteX + 2);
    inputState.ballY = readMem16(A2 + kSpriteY + 2);
    inputState.goalkeeperX = readMem16(A1 + kSpriteX + 2);
    inputState.goalkeeperY = readMem16(A1 + kSpriteY + 2);
    inputState.ballSpeed = readMem16(A2 + kSpriteSpeed);
    inputState.ballDeltaY = readMem32(A2 + kSpriteDeltaY);
    inputState.ballDefensiveX = readMem16(kBallDefensiveX);
    inputState.yDiff = inputState.ballY - inputState.goalkeeperY;
    inputState.isTopTeam = (A6 == ptrToOffset(&swos.topTeamData));
    inputState.isPenalty = (readMem16(kPlayingPenalties_GK) != 0 || readMem16(kPenalty_GK) != 0);

    // Save registers
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;
    Register savedA4 = A4, savedA5 = A5, savedA6 = A6;
    auto rngStateOriginal = SWOS::saveRngState();

    // Run C++ version
    PlayerFunctions::ShouldGoalkeeperDive();
    GoalkeeperDiveCompareState cppOutput = captureGoalkeeperDiveState();

    // Save C++ state
    Register cppD0 = D0;
    auto rngStateCpp = SWOS::saveRngState();

    // Restore original state
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;
    A4 = savedA4; A5 = savedA5; A6 = savedA6;
    SWOS::restoreRngState(rngStateOriginal);

    // Run ASM version
    ::ShouldGoalkeeperDive();
    GoalkeeperDiveCompareState asmOutput = captureGoalkeeperDiveState();

    // Restore to selected version's state
    if (useCpp) {
        D0 = cppD0;
        SWOS::restoreRngState(rngStateCpp);
    }

    if (asmOutput != cppOutput) {
        logGoalkeeperDiveMismatch(swos.frameCount, asmOutput, cppOutput, &inputState);
    }
}

void CompareGoalkeeperJumping(bool useCpp)
{
    using namespace SwosVM;

    if (!g_comparePortedFunctions) {
        if (useCpp) PlayerFunctions::GoalkeeperJumping();
        else ::GoalkeeperJumping();
        return;
    }

    // Capture input state for diagnostics
    auto readMem16 = [](uint32_t addr) -> int16_t {
        return *reinterpret_cast<int16_t*>(g_memByte + addr);
    };
    auto readMem32 = [](uint32_t addr) -> uint32_t {
        return *reinterpret_cast<uint32_t*>(g_memByte + addr);
    };

    uint32_t goalkeeperSprite = A1;
    uint32_t teamPtr = A6;

    GoalkeeperJumpInputState inputState = {};
    inputState.ballX = readMem16(A2 + kSpriteX + 2);
    inputState.ballY = readMem16(A2 + kSpriteY + 2);
    inputState.goalkeeperX = readMem16(A1 + kSpriteX + 2);
    inputState.goalkeeperY = readMem16(A1 + kSpriteY + 2);
    inputState.ballDistance = readMem32(A1 + kSpriteBallDistance);
    inputState.ballNotHighZ = readMem16(kBallNotHighZ);
    inputState.d1Value = static_cast<int16_t>(D1 & 0xFFFF);
    inputState.d3Direction = static_cast<int16_t>(D3 & 0xFFFF);
    inputState.isTopTeam = (A6 == ptrToOffset(&swos.topTeamData));

    // Save full sprite and team state for goalkeeper
    auto saveGkSpriteState = [&]() {
        return captureGoalkeeperJumpState(goalkeeperSprite, teamPtr);
    };

    // Save initial state
    GoalkeeperJumpCompareState initialState = saveGkSpriteState();

    // Save registers
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;
    Register savedA0 = A0, savedA1 = A1, savedA2 = A2, savedA3 = A3;
    Register savedA4 = A4, savedA5 = A5, savedA6 = A6;
    auto rngStateOriginal = SWOS::saveRngState();

    // Function to restore goalkeeper sprite to initial state
    auto restoreGkSpriteState = [&](const GoalkeeperJumpCompareState& state) {
        auto writeMem16 = [](uint32_t addr, int16_t val) {
            *reinterpret_cast<int16_t*>(g_memByte + addr) = val;
        };
        auto writeMem8 = [](uint32_t addr, uint8_t val) {
            g_memByte[addr] = val;
        };

        writeMem16(goalkeeperSprite + kSpriteSpeed, state.speed);
        writeMem16(goalkeeperSprite + kSpriteDestX, state.destX);
        writeMem16(goalkeeperSprite + kSpriteDestY, state.destY);
        writeMem8(goalkeeperSprite + kSpritePlayerState, state.playerState);
        writeMem8(goalkeeperSprite + kSpritePlayerDownTimer, state.playerDownTimer);
        writeMem16(teamPtr + kTeamControlledPlDirection, state.controlledPlDirection);
        writeMem16(teamPtr + kTeamGoalkeeperDivingRight, state.goalkeeperDivingRight);
    };

    // Run C++ version
    PlayerFunctions::GoalkeeperJumping();
    GoalkeeperJumpCompareState cppOutput = saveGkSpriteState();

    // Save C++ registers
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppA0 = A0;
    auto rngStateCpp = SWOS::saveRngState();

    // Restore to initial state
    restoreGkSpriteState(initialState);
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;
    A0 = savedA0; A1 = savedA1; A2 = savedA2; A3 = savedA3;
    A4 = savedA4; A5 = savedA5; A6 = savedA6;
    SWOS::restoreRngState(rngStateOriginal);

    // Run ASM version
    ::GoalkeeperJumping();
    GoalkeeperJumpCompareState asmOutput = saveGkSpriteState();

    // Restore to selected version's state
    if (useCpp) {
        restoreGkSpriteState(cppOutput);
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        A0 = cppA0;
        SWOS::restoreRngState(rngStateCpp);
    }

    if (asmOutput != cppOutput) {
        logGoalkeeperJumpMismatch(swos.frameCount, asmOutput, cppOutput, &inputState);
    }
}

// Quick Match Mode Implementation
bool g_quickMatchMode = false;

// Create a simple test team with basic data
static void createTestTeam(TeamFile* team, const char* name, int teamNum)
{
    memset(team, 0, sizeof(TeamFile));

    team->teamFileNo = 0;
    team->teamOrdinal = static_cast<byte>(teamNum);
    team->globalTeamNumber = static_cast<word>(teamNum);
    team->teamControls = kComputerTeam;  // CPU controlled
    strncpy(team->teamName, name, 16);
    team->teamName[16] = '\0';
    team->tactics = 0;  // 4-4-2
    team->league = 0;

    // Team colors (red vs blue)
    if (teamNum == 1) {
        team->prShirtType = 0;     // Plain shirt
        team->prBasicColor = 4;    // Red
        team->prShortsColor = 0;   // White
        team->prSocksColor = 4;    // Red
    } else {
        team->prShirtType = 0;     // Plain shirt
        team->prBasicColor = 1;    // Blue
        team->prShortsColor = 0;   // White
        team->prSocksColor = 1;    // Blue
    }

    // Create 16 basic players
    for (int i = 0; i < 16; i++) {
        PlayerFile* p = &team->players[i];
        p->nationality = 0;  // English
        p->shirtNumber = static_cast<byte>(i + 1);

        snprintf(p->playerName, sizeof(p->playerName), "PLAYER %d", i + 1);

        // Position: GK for player 1, then defenders, midfielders, attackers
        if (i == 0) {
            p->positionAndFace = 0;   // Goalkeeper
        } else if (i <= 4) {
            p->positionAndFace = 1;   // Defender
        } else if (i <= 8) {
            p->positionAndFace = 2;   // Midfielder
        } else {
            p->positionAndFace = 3;   // Attacker
        }

        // Stats (moderate abilities)
        p->passing = 5;
        p->shootingHeading = 5;
        p->tacklingBallControl = 5;
        p->speedFinishing = 5;
        p->price = 50;

        team->playerNumbers[i] = static_cast<byte>(i + 1);
    }
}

void startQuickMatch()
{
    logDebug("Starting quick match for AI comparison testing...");

    // Initialize the game core (sprites, audio, timers, etc.)
    initializeGameCore();

    // Set up minimal game state without calling menu-related assembly
    swos.twoPlayers = -1;
    swos.flipOnOff = 1;
    swos.inFriendlyMenu = 0;
    swos.isNationalTeam = 0;
    swos.g_gameType = kGameTypeNoGame;
    swos.g_exitMenu = 0;
    swos.fireResetFlag = 0;
    swos.competitionOver = 0;
    swos.teamsLoaded = 0;
    swos.poolplyrLoaded = 0;

    // Create two test teams
    static TeamFile testTeam1, testTeam2;
    createTestTeam(&testTeam1, "TEST TEAM A", 1);
    createTestTeam(&testTeam2, "TEST TEAM B", 2);

    // Set up game parameters
    swos.isGameFriendly = 1;
    swos.gameMinSubstitutes = 0;
    swos.gameMaxSubstitutes = 3;
    swos.g_gameLength = 0;  // Shortest game length
    swos.playMatchTeam1Ptr = &testTeam1;
    swos.playMatchTeam2Ptr = &testTeam2;
    swos.gameTeam1 = &testTeam1;
    swos.gameTeam2 = &testTeam2;

    logDebug("About to initialize ingame teams...");

    // Initialize the ingame teams
    initializeIngameTeams(0, 3, &testTeam1, &testTeam2);

    logDebug("Teams initialized: %s vs %s", swos.topTeamInGame.teamName, swos.bottomTeamInGame.teamName);

    // Disable pre-match menus for faster start
    setPreMatchMenus(false);

    // Initialize game controls for CPU vs CPU
    initGameControls();

    // Start the replay system
    initNewReplay();

    // Start the game!
    logDebug("Starting game loop...");
    gameLoop(&swos.topTeamInGame, &swos.bottomTeamInGame);

    logDebug("Quick match ended.");

    // Show comparison results
    if (g_comparePortedFunctions) {
        closeComparisonLog();
        logDebug("Total mismatches found: %d", g_comparisonMismatchCount);
    }
}
