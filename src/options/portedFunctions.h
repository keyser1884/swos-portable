#pragma once

#include <cstdint>

// Toggle configuration for switching between C++ ported functions and original assembly
// This allows isolating which ported function may have bugs

// Comparison mode: runs both ASM and C++ versions and logs differences
extern bool g_comparePortedFunctions;
extern bool g_breakOnMismatch;  // Trigger debugger break when mismatch found
extern int g_comparisonMismatchCount;

struct PortedFunctionToggles {
    // AI functions
    bool useCpp_AI_SetControlsDirection = true;

    // Player functions (30 total)
    bool useCpp_ResetBothTeamSpinTimers = true;
    bool useCpp_ThrowInDeadProc = true;
    bool useCpp_GetPlayerPointerFromShirtNumber = true;
    bool useCpp_UpdateTeamShotChanceTable = true;
    bool useCpp_SetThrowInPlayerDestinationCoordinates = true;
    bool useCpp_SetPlayerDowntimeAfterTackle = true;
    bool useCpp_UpdatePlayerWithBall = true;
    bool useCpp_UpdateControllingPlayer = true;
    bool useCpp_GoalkeeperCaughtTheBall = true;
    bool useCpp_UpdateBallWithControllingGoalkeeper = true;
    bool useCpp_AttemptStaticHeader = true;
    bool useCpp_UpdatePlayerSpeed = true;
    bool useCpp_GoalkeeperJumping = true;
    bool useCpp_PlayerBeginTackling = true;
    bool useCpp_PlayersTackledTheBallStrong = true;
    bool useCpp_SetJumpHeaderHitAnimTable = true;
    bool useCpp_SetStaticHeaderDirection = true;
    bool useCpp_PlayerAttemptingJumpHeader = true;
    bool useCpp_AI_Kick = true;
    bool useCpp_CalculateBallNextGroundXYPositions = true;
    bool useCpp_UpdateBallVariables = true;
    bool useCpp_ApplyBallAfterTouch = true;
    bool useCpp_PlayerHittingStaticHeader = true;
    bool useCpp_PlayerHittingJumpHeader = true;
    bool useCpp_PlayerTackledTheBallWeak = true;
    bool useCpp_PlayerKickingBall = true;
    bool useCpp_ShouldGoalkeeperDive = true;
    bool useCpp_PlayerTacklingTestFoul = true;
    bool useCpp_DoPass = true;
    bool useCpp_CalculateIfPlayerWinsBall = true;
};

extern PortedFunctionToggles g_portedToggles;

// Load/save toggle settings from swos.ini
void loadPortedFunctionToggles();
void savePortedFunctionToggles();

// Helper to get toggle count for menu
constexpr int kNumPortedFunctionToggles = 31;

// Toggle information for menu display
struct ToggleInfo {
    const char* name;
    bool* toggle;
};

const ToggleInfo* getPortedFunctionToggleInfo();

// AI State capture for comparison testing
struct AICompareState {
    // TeamGeneralInfo outputs
    int16_t currentAllowedDirection;
    uint8_t quickFire;
    uint8_t normalFire;
    uint8_t firePressed;
    uint8_t fireThisFrame;
    int16_t ofs134;  // field_84 (retry pass counter)
    int16_t ofs136;  // AI_afterTouchStrength
    int16_t ofs138;  // AI_ballSpinDirection
    int16_t AITimer;

    bool operator==(const AICompareState& other) const;
    bool operator!=(const AICompareState& other) const { return !(*this == other); }
};

// Capture current AI output state for a team
AICompareState captureAIState(void* teamPtr);

// Diagnostic state captured before running either version
struct AICompareInputState {
    int16_t spinTimer;
    uint8_t plVeryCloseToBall;
    uint8_t plCloseToBall;
    uint16_t aiRandValue;  // Avoid name collision with AI_rand macro
    bool hasControlledPlayer;
    int16_t playerDirection;
    // Additional diagnostics for handleBallAfterTouch path
    int16_t ofs136_initial;  // AI_afterTouchStrength at entry
    int16_t ofs138_initial;  // AI_ballSpinDirection at entry
    bool isPenalties;        // playingPenalties || penalty
    // Additional diagnostics for fireThisFrame and shooting paths
    int16_t ballZ;           // Ball height
    int32_t ballDeltaZ;      // Ball vertical velocity
    uint32_t ctrlBallDist;   // Controlled player's ball distance
    int32_t goalDistSqr;     // Distance to opponent's goal squared
    uint32_t oppCtrlDist;    // Opponent's controlled player ball distance (0 if none)
    uint32_t oppPassDist;    // Opponent's pass-to player ball distance (0 if none)
    int16_t ofs134_initial;  // field_84 (retry pass counter)
};

// Log a mismatch between ASM and C++ outputs
void logAICompareMismatch(uint32_t frame, void* teamPtr,
                          const AICompareState& asmState,
                          const AICompareState& cppState,
                          const AICompareInputState* inputState = nullptr);

// Initialize/close comparison log file
void initComparisonLog();
void closeComparisonLog();

// ============== Kick/Shot Comparison ==============

// Ball state after a kick
struct KickCompareState {
    int16_t ballSpeed;       // Ball sprite speed
    int32_t ballDeltaZ;      // Ball vertical velocity (raw fixed-point)
    int16_t ballDestX;       // Ball destination X
    int16_t ballDestY;       // Ball destination Y
    int16_t passInProgress;  // Team pass in progress flag
    int16_t spinTimer;       // Team spin timer

    bool operator==(const KickCompareState& other) const {
        return ballSpeed == other.ballSpeed &&
               ballDeltaZ == other.ballDeltaZ &&
               ballDestX == other.ballDestX &&
               ballDestY == other.ballDestY &&
               passInProgress == other.passInProgress &&
               spinTimer == other.spinTimer;
    }
    bool operator!=(const KickCompareState& other) const { return !(*this == other); }
};

// Input state for kick diagnosis
struct KickInputState {
    int16_t playerDirection; // Kicking player's direction
    int16_t playerOrdinal;   // Player ordinal (1 = goalkeeper)
    int16_t ballX;           // Ball X position
    int16_t ballY;           // Ball Y position
    int8_t shootingSkill;    // Player's shooting skill (0-7)
    int8_t finishingSkill;   // Player's finishing skill (0-7)
    int8_t passingSkill;     // Player's passing skill (0-7)
    int8_t ballControlSkill; // Player's ball control skill (0-7)
    bool isTopTeam;          // Is kicking team the top team
    int16_t gameState;       // Current game state
    int16_t gameStatePl;     // Game state player
    int16_t fireCounter;     // Fire button held counter
    uint8_t quickFire;       // Quick fire flag
    uint8_t normalFire;      // Normal fire flag
};

// Capture ball/kick state
KickCompareState captureKickState(uint32_t teamPtr);

// Log kick mismatch
void logKickMismatch(uint32_t frame, const char* functionName,
                     const KickCompareState& asmState,
                     const KickCompareState& cppState,
                     const KickInputState* inputState = nullptr);

// Comparison wrapper - runs both ASM and C++ from same state, logs mismatches
// Call this from updatePlayers.cpp instead of the simple toggle
void ComparePlayerKickingBall(bool useCpp);

// Ball physics comparison wrappers
void CompareApplyBallAfterTouch(bool useCpp);
void CompareUpdateBallVariables(bool useCpp);
void CompareCalculateBallNextGroundXYPositions(bool useCpp);

// Quick match mode for automated testing
extern bool g_quickMatchMode;
void startQuickMatch();
