#include "playerFunctions.h"
#include "../../stdinc.h"
#include "../../util/log.h"

using namespace SwosVM;

// Memory access macros for playerFunctions.cpp
// All constants in this file (kDefaultDestinations, kGameState, kBallDefensiveX, etc.)
// are ABSOLUTE addresses into g_memByte, NOT offsets from kMemStart.
// Therefore we access g_memByte directly, not via kMemStart (which adds 256).
#define MEM_WORD(offset) (*(int16_t*)(&g_memByte[(offset)]))

// Memory offsets for sprites and variables
constexpr uint32_t kBallSpriteOffset = 328988;
constexpr uint32_t kThrowInDeadVar = 455926;

// Tackling downtime tables
constexpr uint32_t kPlayerTacklingDownTime = 524012;
constexpr uint32_t kComputerTacklingDownTime = 524028;

// Position offset tables
constexpr uint32_t kPlayerWithBallOffsets = 524212;
constexpr uint32_t kBallPlOffsets = 523940;

// Goalkeeper variables
constexpr uint32_t kGoalieCatchingBallAnimTable = 453658;
constexpr uint32_t kGoalkeeperCatchSpeed = 324032;
constexpr uint32_t kBallNextGroundX = 524764;
constexpr uint32_t kBallNextGroundY = 524766;
constexpr uint32_t kTopTeamDataOffset = 522804;

// Header variables
constexpr uint32_t kDefaultDestinations = 523306;
constexpr uint32_t kStaticHeaderAttemptAnimTable = 452194;
constexpr uint32_t kStaticHeaderPlayerSpeed = 524736;

// Lookup table for player offsets within team data
constexpr uint32_t kInGameTeamPlayerOffsets = 332510;

// Sprite field offsets (matching assembly Sprite struc)
constexpr int kSpriteX = 30;           // FixedPoint x (4 bytes)
constexpr int kSpriteY = 34;           // FixedPoint y (4 bytes)
constexpr int kSpriteZ = 38;           // FixedPoint z (4 bytes)
constexpr int kSpriteDirection = 42;   // direction (2 bytes)
constexpr int kSpriteSpeed = 44;       // speed (2 bytes)
constexpr int kSpriteDeltaZ = 54;      // FixedPoint deltaZ (4 bytes)
constexpr int kSpriteDestX = 58;       // destX (2 bytes)
constexpr int kSpriteDestY = 60;       // destY (2 bytes)
constexpr int kSpriteFullDirection = 82; // fullDirection (1 byte at low)

// Player struct size
constexpr int16_t kPlayerStructSize = 61;

// Speed tables
constexpr uint32_t kPlayerSpeedsGameInProgress = 524044;
constexpr uint32_t kPlayerSpeedsGameStopped = 524060;
constexpr uint32_t kInjuriesSpeedHandicap = 524196;

// Game state variables
constexpr uint32_t kGameStatePl = 523128;
constexpr uint32_t kGameState = 523130;
constexpr uint32_t kStoppageTimerTotal = 523124;
constexpr uint32_t kRunSlower = 457534;

// Game state constants
constexpr int16_t ST_PLAYERS_TO_INITIAL = 0;
constexpr int16_t ST_GAME_IN_PROGRESS = 100;
constexpr int16_t ST_FIRST_HALF_ENDED = 29;
constexpr int16_t ST_GAME_ENDED = 30;
constexpr int16_t ST_GOING_TO_HALFTIME = 23;
constexpr int16_t ST_PLAYERS_GOING_TO_SHOWER = 24;

// Player state constants
constexpr int8_t PL_NORMAL = 0;
constexpr int8_t PL_GOALIE_DIVING_HIGH = 6;
constexpr int8_t PL_GOALIE_DIVING_LOW = 7;

// Goalkeeper jumping constants
constexpr uint32_t kGoalkeeperNearJumpSpeed = 524010;
constexpr uint32_t kGoalkeeperFarJumpSpeed = 324036;
constexpr uint32_t kGoalkeeperFarJumpSlowerSpeed = 324038;
constexpr uint32_t kCurrentGameTick = 323904;
constexpr uint32_t kBallNotHighZ = 524756;
constexpr uint32_t kBottomTeamData = 522952;

// Goalkeeper animation tables
constexpr uint32_t kLeftGoalieJumpingHighAnimTable = 454308;
constexpr uint32_t kRightGoalieJumpingHighAnimTable = 454568;
constexpr uint32_t kLeftGoalieJumpingLowAnimTable = 454698;
constexpr uint32_t kRightGoalieJumpingLowAnimTable = 454828;

// Sprite field offsets
constexpr int kSpriteBallDistance = 74;
constexpr int kSpriteTackleState = 96;
constexpr int kSpriteTacklingTimer = 106;

// Tackling constants
constexpr int8_t PL_TACKLING = 1;
constexpr uint32_t kPlTacklingAnimTable = 453364;
constexpr uint32_t kPlayerDownTacklingInterval = 540868;
constexpr uint32_t kPlayerTacklingSpeed = 325742;
constexpr int kPlayerGameTackling = 72;
constexpr int kPlayerGameBallControl = 73;  // ballControl in PlayerGameHeader struct

// Jump header constants
constexpr uint32_t kJumpHeaderAttemptAnimTable = 452584;
constexpr uint32_t kJumpHeaderHitAnimTable = 452714;
constexpr uint32_t kStaticHeaderHitAnimTable = 452454;
constexpr uint32_t kJumpHeaderSpeed = 325744;
constexpr int8_t PL_JUMP_HEADING = 9;

// AI kick constants
constexpr uint32_t kDeadVarAlways0 = 449467;
constexpr uint32_t kDseg1309C1 = 449493;

// Ball physics constants
constexpr uint32_t kGravityConstantOffset = 325648;
constexpr uint32_t kBallNextGroundXVar = 524764;
constexpr uint32_t kBallNextGroundYVar = 524766;
constexpr uint32_t kBallNextZDeadVar = 524768;
constexpr int kSpriteDeltaX = 46;       // FixedPoint deltaX (4 bytes)
constexpr int kSpriteDeltaY = 50;       // FixedPoint deltaY (4 bytes)

// Defensive AI positioning variables
constexpr uint32_t kBallDefensiveX = 524746;
constexpr uint32_t kBallDefensiveY = 524748;
constexpr uint32_t kBallDefensiveZ = 524750;
constexpr uint32_t kBallNotHighX = 524752;
constexpr uint32_t kBallNotHighY = 524754;
// kBallNotHighZ = 524756 already defined above
constexpr uint32_t kStrikeDestX = 524758;
constexpr uint32_t kStrikeDestY = 524760;
constexpr uint32_t kStrikeDestZ = 524762;
// kBottomTeamData = 522952 already defined above

// Extern assembly functions
extern void SetAnimationTableAndPictureIndex();
extern void GetBallDestCoordinatesTable();
extern void PlayerTackled();
extern void TryBookingThePlayer();
extern void TrySendingOffThePlayer();
extern void TestFoulForPenaltyAndFreeKick();
extern void GetClosestNonControlledPlayerInDirection();
extern void PlayStopGoodPassSampleIfNeeded();
extern void Rand();
extern void FindClosestPlayerToBallFacing();

namespace PlayerFunctions {

// ResetBothTeamSpinTimers - resets spin timers for both teams
// Assembly at 104664-104669
void ResetBothTeamSpinTimers() {
    swos.topTeamData.spinTimer = -1;
    swos.bottomTeamData.spinTimer = -1;
}

// ThrowInDeadProc - sets throw-in dead variable
// Assembly at 90196-90200
void ThrowInDeadProc() {
    MEM_WORD(kThrowInDeadVar) = 1;
}

// GetPlayerPointerFromShirtNumber - gets player game data pointer from sprite
// Assembly at 105408-105429
// Input: A1 -> player sprite, A6 -> team data
// Output: A4 -> player game header
void GetPlayerPointerFromShirtNumber() {
    // mov ax, [esi+Sprite.playerOrdinal]
    int16_t ordinal = (int16_t)readMemory(A1 + 2, 2);

    // sub word ptr D0, 1
    int16_t index = ordinal - 1;

    // shl word ptr D0, 1 (multiply by 2 for table index)
    int16_t tableIndex = index << 1;

    // mov ax, [esi+ebx] - read offset from inGameTeamPlayerOffsets table
    int16_t playerOffset = (int16_t)readMemory(kInGameTeamPlayerOffsets + tableIndex, 2);

    // mov eax, [esi+TeamGeneralInfo.inGameTeamPtr]
    uint32_t teamPtr = readMemory(A6 + 10, 4);

    // add eax, ebx -> A4
    A4 = teamPtr + playerOffset;

}

// UpdateTeamShotChanceTable - updates shot chance table based on player type
// Assembly at 105372-105397
// Input: A1 -> player sprite, A6 -> team data
void UpdateTeamShotChanceTable() {
    // First get the player pointer
    GetPlayerPointerFromShirtNumber();

    PlayerGameHeader* player = reinterpret_cast<PlayerGameHeader*>(offsetToPtr(A4));
    TeamGeneralInfo* team = reinterpret_cast<TeamGeneralInfo*>(offsetToPtr(A6));

    // Check if goalkeeper (position == 0)
    if (player->position == PlayerPosition::kGoalkeeper) {
        uint8_t goalieSkill = player->goalieDirection;
        team->shotChanceTable = swos.kGoalieSkillTables[goalieSkill].getRaw();
    } else {
        team->shotChanceTable = ptrToOffset(&swos.kPlayerShotChanceTable[0]);
    }
}

// SetThrowInPlayerDestinationCoordinates - positions player for throw-in
// Assembly at 109352-109384
// Input: A1 -> player sprite
void SetThrowInPlayerDestinationCoordinates() {
    // Get ball x position (whole part)
    int16_t ballX = (int16_t)readMemory(kBallSpriteOffset + kSpriteX + 2, 2);

    // Adjust x by +/-3 based on which half of pitch
    int16_t newX;
    if (ballX < 336) {
        newX = ballX - 3;
    } else {
        newX = ballX + 3;
    }

    // Set player x position and destination
    writeMemory(A1 + kSpriteX + 2, 2, newX);
    writeMemory(A1 + kSpriteDestX, 2, newX);

    // Get ball y position (whole part)
    int16_t ballY = (int16_t)readMemory(kBallSpriteOffset + kSpriteY + 2, 2);

    // Set player y position and destination
    writeMemory(A1 + kSpriteY + 2, 2, ballY);
    writeMemory(A1 + kSpriteDestY, 2, ballY);
}

// SetPlayerDowntimeAfterTackle - sets player downtime after tackling
// Assembly at 105105-105129
// Input: A1 -> player sprite, A6 -> team data
void SetPlayerDowntimeAfterTackle() {
    // Get player game header
    GetPlayerPointerFromShirtNumber();

    // Check tacklingTimer at offset 106 in sprite
    int16_t tacklingTimer = (int16_t)readMemory(A1 + kSpriteTacklingTimer, 2);

    // Select table based on whether player is human or computer controlled
    uint32_t tableOffset;
    const char* controlType;
    if (tacklingTimer == -1) {
        tableOffset = kComputerTacklingDownTime;
        controlType = "CPU";
    } else {
        tableOffset = kPlayerTacklingDownTime;
        controlType = "HUMAN";
    }

    // Get tackling skill from player game header (offset 72 in PlayerGameHeader)
    int8_t tackling = (int8_t)readMemory(A4 + kPlayerGameTackling, 1);
    int16_t tableIndex = tackling * 2;

    // Look up downtime and set on sprite (offset 13 = playerDownTimer)
    // NOTE: kPlayerTacklingDownTime and kComputerTacklingDownTime are ABSOLUTE addresses
    // into g_memByte, so we use readMemory() which reads from g_memByte[addr] directly,
    // rather than MEM_WORD which uses kMemStart (g_memByte + 256).
    int32_t tableAddress = static_cast<int32_t>(tableOffset) + tableIndex;
    int16_t downtime = (int16_t)readMemory(tableAddress, 2);

    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);

    logInfo("[TACKLE_DOWN] Player %d at (%d,%d): tacklingTimer=%d (%s) tackleSkill=%d tableOffset=%u tableIdx=%d tableAddr=%d downtime=%d",
            playerOrdinal, playerX, playerY, tacklingTimer, controlType, tackling, tableOffset, tableIndex, tableAddress, downtime);

    writeMemory(A1 + 13, 1, downtime & 0xFF);
}

// UpdatePlayerWithBall - positions player relative to ball based on direction
// Assembly at 104616-104653
// Input: A1 -> player sprite
void UpdatePlayerWithBall() {
    // mov A0, offset ballSprite
    A0 = kBallSpriteOffset;

    // Get ball x position (whole part at offset +32 from sprite base)
    // mov ax, word ptr [esi+(Sprite.x+2)]
    int16_t ballX = (int16_t)readMemory(A0 + kSpriteX + 2, 2);

    // Get ball y position (whole part)
    int16_t ballY = (int16_t)readMemory(A0 + kSpriteY + 2, 2);

    // Get player direction
    // mov ax, [esi+Sprite.direction]
    int16_t direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);

    // Calculate table index (direction * 4)
    int16_t tableIndex = direction << 2;

    // mov A0, offset kPlayerWithBallOffsets
    A0 = kPlayerWithBallOffsets;

    // Get x and y offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new position
    int16_t newX = ballX + xOffset;
    int16_t newY = ballY + yOffset;

    // Set player x position (whole part)
    writeMemory(A1 + kSpriteX + 2, 2, newX);
    // Set player y position (whole part)
    writeMemory(A1 + kSpriteY + 2, 2, newY);
    // Set destX
    writeMemory(A1 + kSpriteDestX, 2, newX);
    // Set destY
    writeMemory(A1 + kSpriteDestY, 2, newY);

    // Reset spin timers
    ResetBothTeamSpinTimers();
}

// UpdateControllingPlayer - positions ball relative to controlling player
// Assembly at 109295-109342
// Input: A1 -> player sprite
void UpdateControllingPlayer() {
    // Get player direction
    int16_t direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t tableIndex = direction << 2;

    // mov A0, offset kBallPlOffsets
    A0 = kBallPlOffsets;

    // Get player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);

    // Get offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new ball position
    int16_t newX = playerX + xOffset;
    int16_t newY = playerY + yOffset;

    // mov A0, offset ballSprite
    A0 = kBallSpriteOffset;

    // Set ball speed to 0
    writeMemory(A0 + kSpriteSpeed, 2, 0);
    // Set ball x position (whole part)
    writeMemory(A0 + kSpriteX + 2, 2, newX);
    // Set ball y position (whole part)
    writeMemory(A0 + kSpriteY + 2, 2, newY);
    // Set ball destX
    writeMemory(A0 + kSpriteDestX, 2, newX);
    // Set ball destY
    writeMemory(A0 + kSpriteDestY, 2, newY);

    // Set ball z to 0 (whole part)
    writeMemory(A0 + kSpriteZ + 2, 2, 0);

    // Halve ball deltaZ (arithmetic shift right)
    int32_t deltaZ = (int32_t)readMemory(A0 + kSpriteDeltaZ, 4);
    deltaZ >>= 1;
    writeMemory(A0 + kSpriteDeltaZ, 4, deltaZ);

    // Reset spin timers
    ResetBothTeamSpinTimers();
}

// GoalkeeperCaughtTheBall - handles goalkeeper catching the ball
// Assembly at 106751-106795
// Input: A1 -> goalkeeper sprite, A6 -> team data
void GoalkeeperCaughtTheBall() {
    // Set direction based on which team
    int16_t direction;
    if (A6 == kTopTeamDataOffset) {
        direction = 4;  // Down
    } else {
        direction = 0;  // Up
    }
    writeMemory(A1 + kSpriteDirection, 2, direction);

    // Clear goalkeeperJumpingLeft (offset 82 in TeamGeneralInfo)
    writeMemory(A6 + 82, 2, 0);

    // Set animation table
    A0 = kGoalieCatchingBallAnimTable;
    SetPlayerAnimationTable();

    // Set player state (offset 12) to PL_GOALIE_CATCHING_BALL (4)
    writeMemory(A1 + 12, 1, 4);
    // Set playerDownTimer (offset 13) to 15
    writeMemory(A1 + 13, 1, 15);

    // Set speed from constant
    int16_t speed = MEM_WORD(kGoalkeeperCatchSpeed);
    writeMemory(A1 + kSpriteSpeed, 2, speed);

    // Set destination to predicted ball landing position
    int16_t destX = MEM_WORD(kBallNextGroundX);
    int16_t destY = MEM_WORD(kBallNextGroundY);

    writeMemory(A1 + kSpriteDestX, 2, destX);

    // Clamp destY to valid pitch range (137-761)
    if (destY <= 137) {
        destY = 137;
    } else if (destY >= 761) {
        destY = 761;
    }
    writeMemory(A1 + kSpriteDestY, 2, destY);
}

// UpdateBallWithControllingGoalkeeper - positions ball relative to controlling goalkeeper
// Assembly at 107974-108023
// Input: A1 -> goalkeeper sprite
void UpdateBallWithControllingGoalkeeper() {
    // Get keeper direction
    int16_t direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t tableIndex = direction << 2;

    // mov A0, offset kBallPlOffsets
    A0 = kBallPlOffsets;

    // Get keeper position (whole part)
    int16_t keeperX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t keeperY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);

    // Get offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new ball position
    int16_t newX = keeperX + xOffset;
    int16_t newY = keeperY + yOffset;

    // mov A0, offset ballSprite
    A0 = kBallSpriteOffset;

    // Set ball speed to 0
    writeMemory(A0 + kSpriteSpeed, 2, 0);
    // Set ball x position (whole part)
    writeMemory(A0 + kSpriteX + 2, 2, newX);
    // Set ball y position (whole part)
    writeMemory(A0 + kSpriteY + 2, 2, newY);
    // Set ball destX
    writeMemory(A0 + kSpriteDestX, 2, newX);
    // Set ball destY
    writeMemory(A0 + kSpriteDestY, 2, newY);

    // Halve deltaZ and make it negative if positive
    // sar D0, 1 / js @@delta_z_negative / neg D0
    int32_t deltaZ = (int32_t)readMemory(A0 + kSpriteDeltaZ, 4);
    deltaZ >>= 1;  // arithmetic shift right
    if (deltaZ >= 0) {
        deltaZ = -deltaZ;
    }
    writeMemory(A0 + kSpriteDeltaZ, 4, deltaZ);

    // Reset spin timers
    ResetBothTeamSpinTimers();
}

// AttemptStaticHeader - sets up player to attempt a static header
// Assembly at 112385-112426
// Input: D0 -> direction, A1 -> player sprite
void AttemptStaticHeader() {
    // Clear heading field (offset 98)
    writeMemory(A1 + 98, 2, 0);

    // Set player direction from D0
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A1 + kSpriteDirection, 2, direction);

    // Calculate table index (direction * 4)
    int16_t tableIndex = direction << 2;

    // Get destination offsets from kDefaultDestinations table
    // NOTE: kDefaultDestinations is an absolute address, use readMemory()
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);

    // Get current player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);

    // Set destination
    writeMemory(A1 + kSpriteDestX, 2, playerX + destOffsetX);
    writeMemory(A1 + kSpriteDestY, 2, playerY + destOffsetY);

    // Set speed from constant
    int16_t speed = MEM_WORD(kStaticHeaderPlayerSpeed);
    writeMemory(A1 + kSpriteSpeed, 2, speed);

    // Set animation table
    A0 = kStaticHeaderAttemptAnimTable;
    SetPlayerAnimationTable();

    // Set player state (offset 12) to PL_STATIC_HEADING (8)
    writeMemory(A1 + 12, 1, 8);
    // Set playerDownTimer (offset 13) to 20
    writeMemory(A1 + 13, 1, 20);
}

// UpdatePlayerSpeed - calculates and sets player movement speed
// Assembly at 105138-105413
// Input: A1 -> player sprite, A6 -> team data
void UpdatePlayerSpeed() {
    // Only update speed for players in normal state
    int8_t playerState = (int8_t)readMemory(A1 + 12, 1);
    if (playerState != PL_NORMAL) {
        return;
    }

    // Check if game is in progress and player is goalkeeper
    int16_t gameStatePl = (int16_t)readMemory(kGameStatePl, 2);
    if (gameStatePl == ST_GAME_IN_PROGRESS) {
        int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);
        if (playerOrdinal == 1) {
            // Player is goalkeeper - check if they're NOT the controlled player
            uint32_t controlledSprite = readMemory(A6 + 32, 4);
            if (A1 != controlledSprite) {
                // Non-controlled goalkeeper during game - skip (special AI handling)
                return;
            }
        }
    }

    // Get player speed skill and look up base speed
    // NOTE: Rounding differences in speed table lookup or subsequent shift operations
    // may cause slight speed variations compared to original assembly (slow players
    // may appear marginally faster)
    GetPlayerPointerFromShirtNumber();
    uint8_t speedSkill = (uint8_t)readMemory(A4 + 74, 1);  // PlayerGameHeader.speed (0-7, unsigned)
    uint16_t tableIndex = speedSkill << 1;  // shl word ptr D0, 1

    // Select speed table based on game state
    A0 = kPlayerSpeedsGameInProgress;
    if ((int16_t)readMemory(kGameStatePl, 2) != ST_GAME_IN_PROGRESS) {
        A0 = kPlayerSpeedsGameStopped;
    }

    int16_t speed = (int16_t)readMemory(A0 + tableIndex, 2);
    writeMemory(A1 + kSpriteSpeed, 2, speed);

    // Check runSlower flag - reduce speed by 3/8 if set
    int16_t runSlower = (int16_t)readMemory(kRunSlower, 2);
    if (runSlower != 0) {
        speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
        int16_t reduction = speed >> 2;  // speed / 4
        speed -= reduction;
        reduction >>= 1;  // speed / 8
        speed -= reduction;
        writeMemory(A1 + kSpriteSpeed, 2, speed);
    }

    // Check if player is controlled (by human or coach) and has injuries
    int16_t playerNumber = (int16_t)readMemory(A6 + 4, 2);
    if (playerNumber != 0) {
        goto player_controlling;
    }
    {  // Scope for playerCoachNumber
    int16_t playerCoachNumber = (int16_t)readMemory(A6 + 6, 2);
    if (playerCoachNumber == 0) {
        goto player_not_injured;
    }
    }  // End scope for playerCoachNumber

player_controlling:
    {
        uint8_t injuriesBitfield = (uint8_t)readMemory(A4 + 77, 1);
        if (injuriesBitfield != 0) {
            // Extract injury severity (bits 5-7) and apply handicap
            uint16_t injuryIndex = (injuriesBitfield & 0xE0) >> 4;
            int16_t handicap = (int16_t)readMemory(kInjuriesSpeedHandicap + injuryIndex, 2);
            speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            speed += handicap;  // handicap is negative
            writeMemory(A1 + kSpriteSpeed, 2, speed);
        }
    }

player_not_injured:

    // If this is the controlled player with the ball, reduce speed slightly
    uint32_t controlledSprite = readMemory(A6 + 32, 4);
    if (A1 == controlledSprite) {
        int16_t playerHasBall = (int16_t)readMemory(A6 + 40, 2);
        if (playerHasBall != 0) {
            speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            int16_t reduction = speed >> 3;  // speed / 8
            speed -= reduction;
            writeMemory(A1 + kSpriteSpeed, 2, speed);
        }
    }

    // Check if this player is the pass-to target
    int16_t teamPlayerNumber = (int16_t)readMemory(A6 + 4, 2);
    if (teamPlayerNumber != 0) {
        uint32_t passToPlayer = readMemory(A6 + 36, 4);
        if (A1 == passToPlayer) {
            int16_t passingToPlayer = (int16_t)readMemory(A6 + 90, 2);
            if (passingToPlayer != 0) {
                // Check if long pass or spin pass
                int32_t longPass = (int32_t)readMemory(A6 + 124, 4);
                int16_t leftSpin = (int16_t)readMemory(A6 + 120, 2);
                int16_t rightSpin = (int16_t)readMemory(A6 + 122, 2);

                if (longPass != 0 || leftSpin != 0 || rightSpin != 0) {
                    // Check ball speed and direction match
                    int16_t ballSpeed = (int16_t)readMemory(kBallSpriteOffset + kSpriteSpeed, 2);
                    if (ballSpeed != 0) {
                        int8_t ballDir = (int8_t)readMemory(kBallSpriteOffset + kSpriteFullDirection, 1);
                        int8_t playerDir = (int8_t)readMemory(A1 + kSpriteFullDirection, 1);
                        int8_t dirDiff = ballDir - playerDir;

                        // Direction difference must be within +/-7
                        if (dirDiff <= 7 && dirDiff >= -7) {
                            // Set pass-catching speed based on direction alignment
                            int16_t passSpeed;
                            if (dirDiff > 5 || dirDiff < -5) {
                                passSpeed = 512;  // Less aligned - faster
                            } else {
                                passSpeed = 256;  // Well aligned - slower
                            }
                            writeMemory(A1 + kSpriteSpeed, 2, passSpeed);
                        }
                    }
                }
            }
        }
    }

    // Handle special game states
    int16_t gameState = (int16_t)readMemory(kGameState, 2);
    gameStatePl = (int16_t)readMemory(kGameStatePl, 2);

    if (gameStatePl != ST_GAME_IN_PROGRESS) {
        if (gameState == ST_FIRST_HALF_ENDED || gameState == ST_GAME_ENDED) {
            // Half or game end - reduce speed based on stoppage timer
            int16_t stoppageTimer = (int16_t)readMemory(kStoppageTimerTotal, 2);
            int16_t reduction = stoppageTimer << 5;
            speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            speed -= reduction;
            if (speed < 0) {
                speed = 0;
            }
            writeMemory(A1 + kSpriteSpeed, 2, speed);
        } else if (gameState == ST_GOING_TO_HALFTIME || gameState == ST_PLAYERS_GOING_TO_SHOWER) {
            // Players leaving pitch - scale speed by stoppage timer percentage
            int16_t stoppageTimer = (int16_t)readMemory(kStoppageTimerTotal, 2);
            int16_t percent = stoppageTimer << 2;
            if (percent > 100) {
                percent = 100;
            }
            speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            speed = (speed * percent) / 100;
            writeMemory(A1 + kSpriteSpeed, 2, speed);
        }
    }

    // Calculate frame delay based on speed (slower = higher delay)
    speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    int16_t frameDelay = 1280 - speed;
    if (frameDelay < 0) {
        frameDelay = 0;
    }
    frameDelay = (frameDelay >> 7) + 6;
    writeMemory(A1 + 24, 2, frameDelay);  // Sprite.frameDelay
}

// GoalkeeperJumping - handles goalkeeper diving/jumping to save
// Assembly at 106132-106246
// Input: D0 -> direction, D1 -> speed flag, D3 -> destination direction
//        A1 -> goalkeeper sprite, A2 -> ball sprite, A6 -> team data
void GoalkeeperJumping() {
    // Check ball distance and set speed accordingly
    uint32_t ballDistance = readMemory(A1 + kSpriteBallDistance, 4);
    int16_t gkSpeed = 0;

    if (ballDistance <= 128) {
        // Ball is close - use near jump speed
        int16_t nearSpeed = (int16_t)readMemory(kGoalkeeperNearJumpSpeed, 2);
        D1 = nearSpeed;
        writeMemory(A1 + kSpriteSpeed, 2, nearSpeed);
        gkSpeed = nearSpeed;
    } else {
        // Ball is far away
        int16_t farSpeed = (int16_t)readMemory(kGoalkeeperFarJumpSpeed, 2);
        writeMemory(A1 + kSpriteSpeed, 2, farSpeed);
        gkSpeed = farSpeed;

        int16_t d1Val = (int16_t)(D1 & 0xFFFF);
        if (d1Val != 0) {
            // Use slower speed
            int16_t slowerSpeed = (int16_t)readMemory(kGoalkeeperFarJumpSlowerSpeed, 2);
            writeMemory(A1 + kSpriteSpeed, 2, slowerSpeed);
            gkSpeed = slowerSpeed;

            if (d1Val != 1) {
                // Add randomness based on game tick
                int16_t gameTick = (int16_t)readMemory(kCurrentGameTick, 2);
                int16_t randomComponent = gameTick & 0xFF;
                int16_t nearJumpSpeed = (int16_t)readMemory(kGoalkeeperNearJumpSpeed, 2);
                int16_t finalSpeed = randomComponent + nearJumpSpeed;
                writeMemory(A1 + kSpriteSpeed, 2, finalSpeed);
                gkSpeed = finalSpeed;
            }
        }
    }

    // Calculate x difference between ball (A2) and goalkeeper (A1)
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t keeperX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t xDiff = ballX - keeperX;
    (void)xDiff;  // Used for direction determination in original

    // Clear some team fields
    writeMemory(A6 + 70, 1, 0);   // TeamGeneralInfo.field_46
    writeMemory(A6 + 80, 2, 0);   // TeamGeneralInfo.goalkeeperDivingRight

    // Check ball height to determine diving high or low
    int16_t ballNotHighZ = (int16_t)readMemory(kBallNotHighZ, 2);
    const char* diveType = nullptr;

    if (ballNotHighZ <= 5) {
        // Diving low
        writeMemory(A1 + 12, 1, PL_GOALIE_DIVING_LOW);
        diveType = "LOW";

        if (A6 == kBottomTeamData) {
            A0 = kRightGoalieJumpingLowAnimTable;
        } else {
            A0 = kLeftGoalieJumpingLowAnimTable;
        }
    } else {
        // Diving high
        writeMemory(A1 + 12, 1, PL_GOALIE_DIVING_HIGH);
        diveType = "HIGH";

        if (A6 == kBottomTeamData) {
            A0 = kRightGoalieJumpingHighAnimTable;
        } else {
            A0 = kLeftGoalieJumpingHighAnimTable;
        }
    }

    SetPlayerAnimationTable();

    // Set down timer
    writeMemory(A1 + 13, 1, 75);  // Sprite.playerDownTimer

    // Store controlled player direction
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A6 + 56, 2, direction);  // TeamGeneralInfo.controlledPlDirection

    // Calculate destination from direction using kDefaultDestinations table
    int16_t destDirection = (int16_t)(D3 & 0xFFFF);
    uint16_t tableIndex = destDirection << 2;

    // Get x offset from table and add to keeper position
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t newDestX = destOffsetX + keeperX;
    writeMemory(A1 + kSpriteDestX, 2, newDestX);

    // Get y offset from table and add to keeper position
    int16_t keeperY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);
    int16_t newDestY = destOffsetY + keeperY;
    writeMemory(A1 + kSpriteDestY, 2, newDestY);

    logInfo("[GK_JUMP] DIVING %s! ballDist=%d gkSpeed=%d ballZ=%d dir=%d destDir=%d gkPos=(%d,%d) dest=(%d,%d)",
            diveType, ballDistance, gkSpeed, ballNotHighZ, direction, destDirection,
            keeperX, keeperY, newDestX, newDestY);
}

// PlayerBeginTackling - initiates a sliding tackle
// Assembly at 106261-106335
// Input: D0 -> direction, A1 -> player sprite, A6 -> team data
void PlayerBeginTackling() {
    // Clear tackle state
    writeMemory(A1 + kSpriteTackleState, 2, 0);

    // Store direction
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A6 + 56, 2, direction);  // TeamGeneralInfo.controlledPlDirection
    writeMemory(A1 + kSpriteDirection, 2, direction);

    // Set tackling animation
    A0 = kPlTacklingAnimTable;
    SetPlayerAnimationTable();

    // Set player state to tackling
    writeMemory(A1 + 12, 1, PL_TACKLING);

    // Set initial down timer from global
    uint8_t downInterval = (uint8_t)readMemory(kPlayerDownTacklingInterval, 1);
    writeMemory(A1 + 13, 1, downInterval);

    // Get player game header to check fasterTackle
    int16_t ordinal = (int16_t)readMemory(A1 + 2, 2);
    int16_t index = ordinal - 1;
    uint16_t tableIndex = index << 1;
    int16_t playerOffset = (int16_t)readMemory(kInGameTeamPlayerOffsets + tableIndex, 2);
    uint32_t teamPtr = readMemory(A6 + 10, 4);
    A0 = teamPtr + playerOffset;

    // Note: Assembly also checks fasterTackle and sets to 25, but then ALWAYS
    // overwrites with -1 on lines 106302-106303 (dead code in original).
    // The -1 value means the tackle ends via speed reaching 0 (calling
    // SetPlayerDowntimeAfterTackle) rather than playerDownTimer reaching 0.
    writeMemory(A1 + 13, 1, (uint8_t)-1);

    // Calculate destination from direction using kDefaultDestinations table
    uint16_t destTableIndex = direction << 2;

    // Get x offset and add to player position
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + destTableIndex, 2);
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t newDestX = destOffsetX + playerX;
    writeMemory(A1 + kSpriteDestX, 2, newDestX);

    // Get y offset and add to player position
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + destTableIndex + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t newDestY = destOffsetY + playerY;
    writeMemory(A1 + kSpriteDestY, 2, newDestY);

    // Set tackling speed
    int16_t tacklingSpeed = (int16_t)readMemory(kPlayerTacklingSpeed, 2);
    writeMemory(A1 + kSpriteSpeed, 2, tacklingSpeed);

    // Clear tackling timer
    writeMemory(A1 + kSpriteTacklingTimer, 2, 0);
}

// PlayersTackledTheBallStrong - handles successful strong tackle on the ball
// Assembly at 106351-106514
// Input: A1 -> player sprite, A6 -> team data
// Adjusts ball direction based on tackle direction, sets ball speed, checks for good tackle
void PlayersTackledTheBallStrong() {
    // Get allowed direction - if negative, use player's direction
    int16_t allowedDir = (int16_t)readMemory(A6 + 44, 2);  // currentAllowedDirection
    int16_t controlDir;
    if (allowedDir >= 0) {
        controlDir = allowedDir;
    } else {
        controlDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    }

    // Set A2 to ball sprite
    A2 = kBallSpriteOffset;

    // Calculate deflection based on difference between player direction and control direction
    int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t dirDiff = playerDir - controlDir;

    int16_t newDir;
    if (dirDiff == 0) {
        newDir = playerDir;
    } else {
        dirDiff &= 7;  // Wrap to 0-7
        if (dirDiff == 4) {
            newDir = playerDir;
        } else if (dirDiff < 4) {
            // Controls leaning leftward - subtract 1 from direction
            newDir = playerDir - 1;
        } else {
            // Controls leaning rightward - add 1 to direction
            newDir = playerDir + 1;
        }
    }

    // Mask direction to 0-7
    newDir &= 7;

    // Store direction
    writeMemory(A6 + 56, 2, newDir);  // controlledPlDirection

    // Calculate ball destination using direction offset table
    uint16_t tableIndex = newDir << 2;
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t newDestX = destOffsetX + ballX;
    writeMemory(A2 + kSpriteDestX, 2, newDestX);

    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);
    int16_t newDestY = destOffsetY + ballY;
    writeMemory(A2 + kSpriteDestY, 2, newDestY);

    // Set ball speed based on whether player is human or CPU
    int16_t playerNumber = (int16_t)readMemory(A6 + 4, 2);
    int16_t playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);

    if (playerNumber == 0) {
        // CPU player - ball speed = player speed (100%)
        writeMemory(A2 + kSpriteSpeed, 2, playerSpeed);
    } else {
        // Human player - ball speed = 125% of player speed
        int16_t ballSpeed = (playerSpeed >> 2) + playerSpeed;
        writeMemory(A2 + kSpriteSpeed, 2, ballSpeed);
        // Apply 125% again (original code does this twice)
        ballSpeed = (playerSpeed >> 2) + playerSpeed;
        writeMemory(A2 + kSpriteSpeed, 2, ballSpeed);
    }

    // Halve player speed
    playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    writeMemory(A1 + kSpriteSpeed, 2, playerSpeed >> 1);

    // Set tackle state to 1 (tackled)
    writeMemory(A1 + kSpriteTackleState, 2, 1);

    // Check for good tackle - get opponent's controlled player
    uint32_t opponentTeam = readMemory(A6, 4);  // opponentsTeam
    uint32_t opponentPlayer = readMemory(opponentTeam + 32, 4);  // controlledPlayerSprite

    if (opponentPlayer != 0) {
        // Check if opponent is far enough from ball (>= 9)
        uint32_t opponentBallDist = readMemory(opponentPlayer + kSpriteBallDistance, 4);
        if (opponentBallDist >= 9) {
            // Calculate squared distance between players
            int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
            int16_t opponentX = (int16_t)readMemory(opponentPlayer + kSpriteX + 2, 2);
            int16_t dx = playerX - opponentX;
            int32_t dxSquared = (int32_t)dx * dx;

            int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
            int16_t opponentY = (int16_t)readMemory(opponentPlayer + kSpriteY + 2, 2);
            int16_t dy = playerY - opponentY;
            int32_t dySquared = (int32_t)dy * dy;

            int32_t distSquared = dxSquared + dySquared;

            // If distance > 32 (squared), it's a good tackle
            if (distSquared > 32) {
                SWOS::PlayGoodTackleComment();
                writeMemory(A1 + kSpriteTackleState, 2, 2);  // TS_GOOD_TACKLE
            }
        }
    }

    // Play kick sample and reset spin timers
    SWOS::PlayKickSample();
    ResetBothTeamSpinTimers();
}

// SetJumpHeaderHitAnimTable - checks conditions and sets jump header hit animation
// Assembly at 112158-112182
// Input: A1 -> player sprite
void SetJumpHeaderHitAnimTable() {
    // Check if already at jump header hit animation table
    uint32_t animTable = readMemory(A1 + 18, 4);  // Sprite.animationTable
    if (animTable == kJumpHeaderHitAnimTable) {
        return;
    }

    // Check if heading is not 0 (Sprite.heading at offset 98)
    int16_t heading = (int16_t)readMemory(A1 + 98, 2);
    if (heading != 0) {
        return;
    }

    // Check if playerDownTimer == 40
    uint8_t downTimer = (uint8_t)readMemory(A1 + 13, 1);  // Sprite.playerDownTimer
    if (downTimer != 40) {
        return;
    }

    // Check if frameSwitchCounter <= 2
    int16_t frameSwitchCounter = (int16_t)readMemory(A1 + 22, 2);  // Sprite.frameSwitchCounter
    if (frameSwitchCounter > 2) {
        return;
    }

    // Check if bit 1 of high byte of currentGameTick is set
    // test byte ptr currentGameTick+1, 2
    uint16_t gameTick = (uint16_t)readMemory(kCurrentGameTick, 2);
    if ((gameTick & 0x0200) == 0) {  // bit 1 of high byte = bit 9 overall
        return;
    }

    // All conditions passed - set animation table and halve speed
    A0 = kJumpHeaderHitAnimTable;
    SetAnimationTableAndPictureIndex();

    // Halve speed
    int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    writeMemory(A1 + kSpriteSpeed, 2, speed >> 1);
}

// SetStaticHeaderDirection - adjusts player direction during static header
// Assembly at 112193-112234
// Input: A1 -> player sprite, A6 -> team data
void SetStaticHeaderDirection() {
    // Get current allowed direction from team data (offset 44)
    int16_t allowedDir = (int16_t)readMemory(A6 + 44, 2);  // currentAllowedDirection

    // If allowed direction is negative, exit
    if (allowedDir < 0) {
        return;
    }

    // Check if playerDownTimer > 18
    uint8_t downTimer = (uint8_t)readMemory(A1 + 13, 1);
    if (downTimer > 18) {
        return;
    }

    // Check if animation table is staticHeaderAttemptAnimTable
    uint32_t animTable = readMemory(A1 + 18, 4);
    if (animTable != kStaticHeaderAttemptAnimTable) {
        return;
    }

    // Get player direction and calculate difference
    int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t diff = allowedDir - playerDir;

    // If already facing the right direction, exit
    if (diff == 0) {
        return;
    }

    // Wrap difference to 0-7
    diff &= 7;

    // If opposite direction (diff == 4), exit
    if (diff == 4) {
        return;
    }

    // Turn toward the target direction
    if (diff < 4) {
        // Turn right (add 1)
        playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        playerDir += 1;
    } else {
        // Turn left (subtract 1)
        playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        playerDir -= 1;
    }

    // Normalize direction to 0-7
    playerDir &= 7;
    writeMemory(A1 + kSpriteDirection, 2, playerDir);
}

// PlayerAttemptingJumpHeader - sets up player to attempt a jump header
// Assembly at 108929-108970
// Input: D0 -> direction, A1 -> player sprite
void PlayerAttemptingJumpHeader() {
    // Clear heading field (offset 98)
    writeMemory(A1 + 98, 2, 0);

    // Set player direction from D0
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A1 + kSpriteDirection, 2, direction);

    // Set jump header attempt animation
    A0 = kJumpHeaderAttemptAnimTable;
    SetPlayerAnimationTable();

    // Set player down timer to 55
    writeMemory(A1 + 13, 1, 55);

    // Set player state to PL_JUMP_HEADING (9)
    writeMemory(A1 + 12, 1, PL_JUMP_HEADING);

    // Calculate destination using kDefaultDestinations table
    uint16_t tableIndex = direction << 2;

    // Get destination offsets (kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);

    // Get current player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);

    // Set destination
    writeMemory(A1 + kSpriteDestX, 2, playerX + destOffsetX);
    writeMemory(A1 + kSpriteDestY, 2, playerY + destOffsetY);

    // Set speed from kJumpHeaderSpeed constant
    int16_t speed = MEM_WORD(kJumpHeaderSpeed);
    writeMemory(A1 + kSpriteSpeed, 2, speed);
}

// AI_Kick - AI decides whether to kick/tackle based on opponent proximity
// Assembly at 119373-119441
// Input: A1 -> player sprite, A6 -> team data
void AI_Kick() {
    // Check deadVarAlways0 - if non-zero, check team restrictions
    int16_t deadVar = MEM_WORD(kDeadVarAlways0);
    if (deadVar != 0) {
        uint32_t dseg1309c1 = readMemory(kDseg1309C1, 4);
        if (A6 == dseg1309c1) {
            return;
        }

        uint32_t opponentTeam = readMemory(A6, 4);  // TeamGeneralInfo.opponentsTeam
        if (opponentTeam == dseg1309c1) {
            return;
        }
    }

    // Check ball distance - exit if > 200
    uint32_t ballDistance = readMemory(A1 + kSpriteBallDistance, 4);
    if (ballDistance > 200) {
        return;
    }

    // Save player's direction in D7
    int16_t myDirection = (int16_t)readMemory(A1 + kSpriteDirection, 2);

    // Get opponent's team
    uint32_t opponentTeam = readMemory(A6, 4);

    // Check if opponent has the ball
    int16_t opponentHasBall = (int16_t)readMemory(opponentTeam + 40, 2);  // playerHasBall
    if (opponentHasBall == 0) {
        return;
    }

    // Get opponent's controlled player sprite
    uint32_t opponentPlayer = readMemory(opponentTeam + 32, 4);  // controlledPlayerSprite
    if (opponentPlayer == 0) {
        return;
    }

    // Get opponent player's direction
    int16_t opponentDir = (int16_t)readMemory(opponentPlayer + kSpriteDirection, 2);

    // Calculate direction difference using byte arithmetic
    // shl both directions by 5, then subtract as bytes
    uint8_t oppDirScaled = (uint8_t)((opponentDir << 5) & 0xFF);
    uint8_t myDirScaled = (uint8_t)((myDirection << 5) & 0xFF);
    uint8_t diff = oppDirScaled - myDirScaled;

    // Check if should kick:
    // cmp byte ptr D0, 224 -> jl @@kick (if diff < 224 signed, kick)
    // cmp byte ptr D0, 32 -> jle @@out (if diff <= 32, exit)
    int8_t signedDiff = (int8_t)diff;
    bool shouldKick = false;

    if (signedDiff < (int8_t)224) {  // 224 = -32 as signed byte
        shouldKick = true;
    } else if (diff <= 32) {
        return;  // Don't kick
    } else {
        shouldKick = true;
    }

    if (shouldKick) {
        // Set current allowed direction to player's direction
        writeMemory(A6 + 44, 2, myDirection);  // currentAllowedDirection

        // Trigger kick
        writeMemory(A6 + 49, 1, 1);  // firePressed
        writeMemory(A6 + 50, 1, 1);  // fireThisFrame
        writeMemory(A6 + 48, 1, 1);  // normalFire
    }
}

// CalculateBallNextGroundXYPositions - predicts where ball will land
// Assembly at 106019-106116
// Input: A2 -> ball sprite
// Output: ballNextGroundX, ballNextGroundY variables set
void CalculateBallNextGroundXYPositions() {
    // Get ball position and velocity (all FixedPoint 16.16 format)
    int32_t ballX = (int32_t)readMemory(A2 + kSpriteX, 4);
    int32_t ballY = (int32_t)readMemory(A2 + kSpriteY, 4);
    int32_t ballZ = (int32_t)readMemory(A2 + kSpriteZ, 4);
    int32_t deltaX = (int32_t)readMemory(A2 + kSpriteDeltaX, 4);
    int32_t deltaY = (int32_t)readMemory(A2 + kSpriteDeltaY, 4);

    // Check if ball is barely moving (both deltas within -1.0 to 1.0 in FixedPoint)
    // -1.0 = -0x10000, 1.0 = 0x10000
    bool ballMoving = false;
    if (deltaY < -0x10000 || deltaY > 0x10000) {
        ballMoving = true;
    } else if (deltaX < -0x10000 || deltaX > 0x10000) {
        ballMoving = true;
    }

    if (!ballMoving) {
        // Ball is standing still
        MEM_WORD(kBallNextGroundXVar) = -1;
        return;
    }

    // Ball is moving - simulate trajectory
    int32_t deltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    int32_t gravity = (int32_t)readMemory(kGravityConstantOffset, 4);
    int32_t zStep = 0x140000;  // 20.0 in FixedPoint

    // Loop: try to subtract zStep from ballZ until it goes negative
    // If it goes negative immediately, decrease zStep and retry
    while (true) {
        int32_t testZ = ballZ - zStep;
        if (testZ >= 0) {
            // Z is still positive after subtraction, continue with trajectory
            ballZ = testZ;
            break;
        } else {
            // Z would go negative
            if (zStep == 0xD0000) {  // 13.0 in FixedPoint - minimum step
                // Ball is effectively standing
                MEM_WORD(kBallNextGroundXVar) = -1;
                return;
            }
            // Decrease step and try again
            zStep -= 0x10000;  // Decrease by 1.0
        }
    }

    // Simulate until ball hits ground (z < 0)
    while (ballZ >= 0) {
        ballX += deltaX;
        ballY += deltaY;
        deltaZ -= gravity;
        ballZ += deltaZ;
    }

    // Ball hit the ground - restore Z slightly
    ballZ += zStep;

    // Extract whole parts (high 16 bits of 32-bit FixedPoint)
    // The assembly uses xchg to swap bytes, effectively extracting the whole part
    int16_t groundX = (int16_t)(ballX >> 16);
    int16_t groundY = (int16_t)(ballY >> 16);

    // Store predicted landing position
    MEM_WORD(kBallNextGroundXVar) = groundX;
    MEM_WORD(kBallNextGroundYVar) = groundY;
    MEM_WORD(kBallNextZDeadVar) = 0;
}

// Helper to extract whole part from FixedPoint (16.16)
static inline int16_t fixedToWhole(int32_t fp) {
    return (int16_t)(fp >> 16);
}

// Helper to clamp Z to non-negative
static inline int16_t clampZPositive(int16_t z) {
    return z < 0 ? 0 : z;
}

// UpdateBallVariables - calculates defensive AI positioning variables
// Assembly at 105439-106011
// Input: A1 -> player sprite, A2 -> ball sprite, A6 -> team data
void UpdateBallVariables() {
    // Get ball position and velocity (all FixedPoint 16.16)
    int32_t ballX = (int32_t)readMemory(A2 + kSpriteX, 4);
    int32_t ballY = (int32_t)readMemory(A2 + kSpriteY, 4);
    int32_t ballZ = (int32_t)readMemory(A2 + kSpriteZ, 4);
    int32_t deltaX = (int32_t)readMemory(A2 + kSpriteDeltaX, 4);
    int32_t deltaY = (int32_t)readMemory(A2 + kSpriteDeltaY, 4);
    int32_t deltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    int32_t gravity = (int32_t)readMemory(kGravityConstantOffset, 4);

    // Determine ball direction
    bool ballGoingUp = deltaY < -0x10000;      // < -1.0
    bool ballGoingDown = deltaY > 0x10000;     // > 1.0
    bool ballGoingLeft = deltaX < -0x10000;    // < -1.0
    bool ballGoingRight = deltaX > 0x10000;    // > 1.0

    if (ballGoingUp) {
        // Ball going toward top team's goal - only process for top team
        if (A6 != kTopTeamDataOffset) {
            goto ball_not_moving;
        }

        int32_t playerY = (int32_t)readMemory(A1 + kSpriteY, 4);
        int32_t relY = ballY - playerY;

        if (relY < 0) {
            // Ball is closer to goal than player - use current ball position
            MEM_WORD(kBallDefensiveX) = fixedToWhole(ballX);
            MEM_WORD(kBallDefensiveY) = fixedToWhole(ballY);
            MEM_WORD(kBallDefensiveZ) = fixedToWhole(ballZ);
            relY = ballY;  // Reset for further calculations
        } else {
            // Simulate ball trajectory until it passes the player
            int32_t simX = ballX, simY = ballY - playerY, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY >= 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            // Back out one step
            simX -= deltaX;
            simY -= deltaY;
            simZ -= simDeltaZ;
            simY += playerY;

            MEM_WORD(kBallDefensiveX) = fixedToWhole(simX);
            MEM_WORD(kBallDefensiveY) = fixedToWhole(simY);
            MEM_WORD(kBallDefensiveZ) = clampZPositive(fixedToWhole(simZ));
            relY = simY;
        }

        // Check if ball is above Y=135
        if (relY - 0x870000 < 0) {  // 135.0
            MEM_WORD(kBallNotHighX) = MEM_WORD(kBallDefensiveX);
            MEM_WORD(kBallNotHighY) = MEM_WORD(kBallDefensiveY);
            MEM_WORD(kBallNotHighZ) = MEM_WORD(kBallDefensiveZ);
        } else {
            // Simulate until Y reaches 135
            int32_t simX = ballX, simY = relY - 0x870000, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY >= 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            simX -= deltaX;
            simZ -= simDeltaZ;
            MEM_WORD(kBallNotHighX) = fixedToWhole(simX);
            MEM_WORD(kBallNotHighY) = 135;
            MEM_WORD(kBallNotHighZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check strike destination (Y=129)
        if (relY - 0x810000 < 0) {  // 129.0
            MEM_WORD(kStrikeDestX) = 0;
            MEM_WORD(kStrikeDestY) = 0;
            MEM_WORD(kStrikeDestZ) = 0;
        } else {
            int32_t simX = ballX, simY = relY - 0x810000, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY >= 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            simX -= deltaX;
            simZ -= simDeltaZ;
            MEM_WORD(kStrikeDestX) = fixedToWhole(simX);
            MEM_WORD(kStrikeDestY) = 129;
            MEM_WORD(kStrikeDestZ) = clampZPositive(fixedToWhole(simZ));
        }
        return;
    }

    if (ballGoingDown) {
        // Ball going toward bottom team's goal - only process for bottom team
        if (A6 != kBottomTeamData) {
            goto ball_not_moving;
        }

        int32_t playerY = (int32_t)readMemory(A1 + kSpriteY, 4);
        int32_t relY = ballY - playerY;

        if (relY >= 0) {
            // Ball is closer to goal than player
            MEM_WORD(kBallDefensiveX) = fixedToWhole(ballX);
            MEM_WORD(kBallDefensiveY) = fixedToWhole(ballY);
            MEM_WORD(kBallDefensiveZ) = fixedToWhole(ballZ);
        } else {
            // Simulate until ball passes player
            int32_t simX = ballX, simY = relY, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY < 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            simX -= deltaX;
            simY -= deltaY;
            simZ -= simDeltaZ;
            simY += playerY;

            MEM_WORD(kBallDefensiveX) = fixedToWhole(simX);
            MEM_WORD(kBallDefensiveY) = fixedToWhole(simY);
            MEM_WORD(kBallDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check if ball reaches Y=763
        int32_t relY763 = ballY - 0x2FB0000;  // 763.0
        if (relY763 >= 0) {
            MEM_WORD(kBallNotHighX) = MEM_WORD(kBallDefensiveX);
            MEM_WORD(kBallNotHighY) = MEM_WORD(kBallDefensiveY);
            MEM_WORD(kBallNotHighZ) = MEM_WORD(kBallDefensiveZ);
        } else {
            int32_t simX = ballX, simY = relY763, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY < 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            simX -= deltaX;
            simZ -= simDeltaZ;
            MEM_WORD(kBallNotHighX) = fixedToWhole(simX);
            MEM_WORD(kBallNotHighY) = 763;
            MEM_WORD(kBallNotHighZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check strike destination (Y=769)
        int32_t relY769 = ballY - 0x3010000;  // 769.0
        if (relY769 >= 0) {
            MEM_WORD(kStrikeDestX) = 0;
            MEM_WORD(kStrikeDestY) = 0;
            MEM_WORD(kStrikeDestZ) = 0;
        } else {
            int32_t simX = ballX, simY = relY769, simZ = ballZ;
            int32_t simDeltaZ = deltaZ;
            while (simY < 0) {
                simDeltaZ -= gravity;
                simZ += simDeltaZ;
                simX += deltaX;
                simY += deltaY;
            }
            simX -= deltaX;
            simZ -= simDeltaZ;
            MEM_WORD(kStrikeDestX) = fixedToWhole(simX);
            MEM_WORD(kStrikeDestY) = 769;
            MEM_WORD(kStrikeDestZ) = clampZPositive(fixedToWhole(simZ));
        }
        return;
    }

    if (ballGoingLeft) {
        int32_t playerX = (int32_t)readMemory(A1 + kSpriteX, 4);
        int32_t relX = ballX - playerX;
        if (relX < 0) {
            goto ball_not_moving;
        }
        // Simulate until ball passes player X
        int32_t simX = relX, simY = ballY, simZ = ballZ;
        int32_t simDeltaZ = deltaZ;
        while (simX >= 0) {
            simDeltaZ -= gravity;
            simZ += simDeltaZ;
            simY += deltaY;
            simX += deltaX;
        }
        simX -= deltaX;
        simY -= deltaY;
        simZ -= simDeltaZ;
        simX += playerX;

        MEM_WORD(kBallDefensiveX) = fixedToWhole(simX);
        MEM_WORD(kBallDefensiveY) = fixedToWhole(simY);
        MEM_WORD(kBallDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        goto set_not_high_from_defensive;
    }

    if (ballGoingRight) {
        int32_t playerX = (int32_t)readMemory(A1 + kSpriteX, 4);
        int32_t relX = ballX - playerX;
        if (relX >= 0) {
            goto ball_not_moving;
        }
        // Simulate until ball passes player X
        int32_t simX = relX, simY = ballY, simZ = ballZ;
        int32_t simDeltaZ = deltaZ;
        while (simX < 0) {
            simDeltaZ -= gravity;
            simZ += simDeltaZ;
            simY += deltaY;
            simX += deltaX;
        }
        simX -= deltaX;
        simY -= deltaY;
        simZ -= simDeltaZ;
        simX += playerX;

        MEM_WORD(kBallDefensiveX) = fixedToWhole(simX);
        MEM_WORD(kBallDefensiveY) = fixedToWhole(simY);
        MEM_WORD(kBallDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        goto set_not_high_from_defensive;
    }

ball_not_moving:
    // Ball not moving significantly - use current position
    MEM_WORD(kBallDefensiveX) = fixedToWhole(ballX);
    MEM_WORD(kBallDefensiveY) = fixedToWhole(ballY);
    MEM_WORD(kBallDefensiveZ) = fixedToWhole(ballZ);

set_not_high_from_defensive:
    MEM_WORD(kBallNotHighX) = MEM_WORD(kBallDefensiveX);
    MEM_WORD(kBallNotHighY) = MEM_WORD(kBallDefensiveY);
    MEM_WORD(kBallNotHighZ) = MEM_WORD(kBallDefensiveZ);
    MEM_WORD(kStrikeDestX) = 0;
    MEM_WORD(kStrikeDestY) = 0;
    MEM_WORD(kStrikeDestZ) = 0;
}

// Spin factor tables (constant data)
// Format: 4 words per direction (left_dx, left_dy, right_dx, right_dy)
static const int16_t kSpinMultiplierFactor[10] = {5, 4, 3, 2, 2, 2, 2, 1, 1, 1};

// Kick spin factor table - 8 directions * 4 values each (left/right spin, x/y)
static const int16_t kKickSpinFactor[32] = {
    -32, 0, 32, 0,     // Direction 0
    0, -23, 23, 0,     // Direction 1
    0, -32, 0, 32,     // Direction 2
    23, 0, 0, 23,      // Direction 3
    32, 0, -32, 0,     // Direction 4
    0, 23, -23, 0,     // Direction 5
    0, 32, 0, -32,     // Direction 6
    -23, 0, 0, -23     // Direction 7
};

// Pass spin factor table - half the values of kick spin
static const int16_t kPassingSpinFactor[32] = {
    -16, 0, 16, 0,     // Direction 0
    0, -11, 11, 0,     // Direction 1
    0, -16, 0, 16,     // Direction 2
    11, 0, 0, 11,      // Direction 3
    16, 0, -16, 0,     // Direction 4
    0, 11, -11, 0,     // Direction 5
    0, 16, 0, -16,     // Direction 6
    -11, 0, 0, -11     // Direction 7
};

// Ball kick constants
constexpr int32_t kHighKickDeltaZ = 0x20000;     // 2.0 in FixedPoint
constexpr int16_t kHighKickBallSpeed = 2688;     // 10.5
constexpr int32_t kNormalKickDeltaZ = 0x16000;   // 1.375 in FixedPoint
constexpr int16_t kNormalKickBallSpeed = 2560;   // 10.0

// TeamGeneralInfo offsets
constexpr int kTeamOpponentsTeam = 0;
constexpr int kTeamGoalkeeperSavedCommentTimer = 76;
constexpr int kTeamCurrentAllowedDirection = 44;
constexpr int kTeamControlledPlDirection = 56;   // allowedPlDirection in C++ struct
constexpr int kTeamSpinTimer = 118;
constexpr int kTeamLeftSpin = 120;
constexpr int kTeamRightSpin = 122;
constexpr int kTeamLongPass = 124;
constexpr int kTeamLongSpinPass = 126;
constexpr int kTeamPassInProgress = 128;

// Keeper holds ball state (GameState::kKeeperHoldsTheBall = 3)
constexpr int16_t ST_KEEPER_HOLDS_BALL = 3;

// Header action constants
constexpr int16_t kStaticHeaderBallSpeed = 1792;
constexpr int kPlayerGameHeading = 71;   // PlayerGameHeader.heading offset (was incorrectly 29)
constexpr int kSpriteHeading = 98;       // Sprite.heading offset

// Player header speed increase table (indexed by heading skill 0-12)
static const int16_t kPlayerHeaderSpeedIncrease[13] = {
    -336, -288, -240, -192, -144, -96, -48, 0, 513, 1027, 1541, 2055, 2569
};

// Jump header constants
constexpr int32_t kBallJumpHeaderDeltaZ = 0xA000;    // Initial ball deltaZ for jump header
constexpr int32_t kHeaderLowJumpHeight = 0x20000;   // 2.0 in fixed point
constexpr int32_t kHeaderHighJumpHeight = 0x24000;  // 2.25 in fixed point
constexpr int kSpriteFrameSwitchCounter = 22;       // Sprite.frameSwitchCounter offset

// Tackle state constants
constexpr int16_t TS_TACKLING_THE_BALL = 1;
constexpr int16_t TS_GOOD_TACKLE = 2;

// TeamGeneralInfo more offsets
constexpr int kTeamControlledPlayerSprite = 32;

// Kicking/shooting constants
constexpr int16_t kBallKickingSpeed = 2208;
constexpr int32_t kBallKickingDeltaZConst = 0x14000;   // 1.25 in fixed point
constexpr uint32_t kStateGoal = 544107;         // stateGoal memory address

// Player skill offsets (in PlayerGameHeader - includes 42-byte header)
constexpr int kPlayerGameShooting = 70;   // 42 (header) + 28 (shooting in PlayerGame)
constexpr int kPlayerGameFinishing = 75;  // 42 (header) + 33 (finishing in PlayerGame)

// Ball speed tables (indexed by shooting/finishing skill 0-7)
static const int16_t kBallSpeedKicking[8] = {
    (int16_t)65152, (int16_t)65266, (int16_t)65374, (int16_t)65482, 54, 162, 270, 384
};
static const int16_t kBallSpeedFinishing[8] = {
    (int16_t)65248, (int16_t)65376, (int16_t)65504, 96, 224, 352, 480, 608
};

// Throw-in game states
constexpr int16_t ST_THROW_IN_FORWARD_RIGHT = 7;
constexpr int16_t ST_THROW_IN_BACK_LEFT = 10;

// Top team data offset for comparison
constexpr uint32_t kTopTeamData = 522804;

// ApplyBallAfterTouch - applies spin/aftertouch to ball during kicks and passes
// Assembly at 104681-105093
// Input: A6 -> team data (TeamGeneralInfo)
void ApplyBallAfterTouch() {
    // Check if pass is in progress
    int16_t passInProgress = (int16_t)readMemory(A6 + kTeamPassInProgress, 2);

    if (passInProgress == 0) {
        // Not passing - it's a kick
        // Check if opponent's goalkeeper saved
        uint32_t opponentsTeam = readMemory(A6 + kTeamOpponentsTeam, 4);
        int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + kTeamGoalkeeperSavedCommentTimer, 2);

        if (goalkeeperTimer < 0) {
            // Reset spin timer and exit
            writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
            return;
        }

        // Check game state
        int16_t gameStatePlVal = MEM_WORD(kGameStatePl);
        if (gameStatePlVal != ST_GAME_IN_PROGRESS) {
            int16_t gameStateVal = MEM_WORD(kGameState);
            if (gameStateVal == ST_KEEPER_HOLDS_BALL) {
                writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
                return;
            }
        }

        // Check spin timer
        int16_t spinTimer = (int16_t)readMemory(A6 + kTeamSpinTimer, 2);
        if (spinTimer < 0) {
            return;  // Spin not active
        }

        // If spin timer is 0, reset spin flags
        if (spinTimer == 0) {
            writeMemory(A6 + kTeamLeftSpin, 2, 0);
            writeMemory(A6 + kTeamRightSpin, 2, 0);
        }

        // A1 -> ball sprite
        A1 = kBallSpriteOffset;

        // Check if already spinning
        int16_t leftSpin = (int16_t)readMemory(A6 + kTeamLeftSpin, 2);
        int16_t rightSpin = (int16_t)readMemory(A6 + kTeamRightSpin, 2);

        int16_t spinDirection = -1;  // -1 = not spinning, 0 = left, 2 = right (element indices)

        if (leftSpin != 0) {
            spinDirection = 0;  // Already spinning left
        } else if (rightSpin != 0) {
            spinDirection = 2;  // Already spinning right
        } else {
            // Not spinning yet - check if player is starting spin
            int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
            if (currentAllowedDir < 0) {
                spinDirection = -1;  // No direction pressed
            } else {
                int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
                int16_t diff = controlledPlDir - currentAllowedDir;
                if (diff == 0) {
                    spinDirection = -1;  // Same direction
                } else {
                    diff &= 7;
                    if (diff == 4) {
                        spinDirection = -1;  // Opposite direction
                    } else if (diff < 4) {
                        // Spin left
                        writeMemory(A6 + kTeamLeftSpin, 2, 1);
                        spinDirection = 0;
                    } else {
                        // Spin right
                        writeMemory(A6 + kTeamRightSpin, 2, 1);
                        spinDirection = 2;
                    }
                }
            }
        }

        // Apply spin if spinning
        if (spinDirection >= 0) {
            // Get controlled player direction for table lookup
            int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
            int16_t tableIndex = (controlledPlDir << 2) + spinDirection;

            // Get spin multiplier based on timer
            int16_t multiplier = kSpinMultiplierFactor[spinTimer];

            // Apply X spin
            int16_t spinX = kKickSpinFactor[tableIndex];
            int16_t spinXFinal = (spinX * multiplier);
            int16_t destX = (int16_t)readMemory(A1 + kSpriteDestX, 2);
            writeMemory(A1 + kSpriteDestX, 2, destX + spinXFinal);

            // Apply Y spin
            int16_t spinY = kKickSpinFactor[tableIndex + 1];
            int16_t spinYFinal = (spinY * multiplier);
            int16_t destY = (int16_t)readMemory(A1 + kSpriteDestY, 2);
            writeMemory(A1 + kSpriteDestY, 2, destY + spinYFinal);

            int16_t ballSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            logInfo("[SPIN_KICK] spinDir=%s timer=%d mult=%d plDir=%d tableIdx=%d spinFactor=(%d,%d) spinApplied=(%d,%d) dest:(%d,%d)->(%d,%d) ballSpeed=%d",
                    spinDirection == 0 ? "LEFT" : "RIGHT", spinTimer, multiplier, controlledPlDir, tableIndex,
                    spinX, spinY, spinXFinal, spinYFinal,
                    destX, destY, destX + spinXFinal, destY + spinYFinal, ballSpeed);
        }

        // Check for high kick when spin timer == 4
        if (spinTimer == 4) {
            int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
            bool doNormalKick = false;
            bool doHighKick = false;

            if (currentAllowedDir < 0) {
                // ASM: js @@normal_not_high_kick - if no direction pressed, do normal kick
                doNormalKick = true;
            } else {
                int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
                int16_t diff = controlledPlDir - currentAllowedDir;
                if (diff != 0) {
                    diff &= 7;
                    // ASM logic:
                    // diff 0, 1: skip (no kick adjustment) - but diff==0 already handled above
                    // diff 2: normal kick
                    // diff 3, 4, 5: high kick
                    // diff 6: normal kick
                    // diff 7: skip (no kick adjustment)
                    if (diff >= 3 && diff <= 5) {
                        doHighKick = true;
                    } else if (diff == 2 || diff == 6) {
                        doNormalKick = true;
                    }
                    // else: diff 1 or 7 - skip kick adjustment (no deltaZ/speed change)
                }
            }

            if (doHighKick) {
                // High kick (diff 3, 4, 5 = back-left, back, back-right)
                int16_t origSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                writeMemory(A1 + kSpriteDeltaZ, 4, kHighKickDeltaZ);
                writeMemory(A1 + kSpriteSpeed, 2, kHighKickBallSpeed);

                // Adjust speed based on direction
                int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
                if (controlledPlDir != 0 && controlledPlDir != 4) {
                    if ((controlledPlDir & 1) != 0) {
                        // Diagonal direction - reduce to 87.5%
                        int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                        int16_t reduction = speed >> 2;
                        int16_t addition = speed >> 3;
                        speed = speed - reduction + addition;
                        writeMemory(A1 + kSpriteSpeed, 2, speed);
                        logInfo("[KICK_HIGH] High kick (diagonal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                                controlledPlDir, origSpeed, kHighKickBallSpeed, speed, kHighKickDeltaZ);
                    }
                } else {
                    // Cardinal direction - reduce to 75%
                    int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                    speed = speed - (speed >> 2);
                    writeMemory(A1 + kSpriteSpeed, 2, speed);
                    logInfo("[KICK_HIGH] High kick (cardinal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                            controlledPlDir, origSpeed, kHighKickBallSpeed, speed, kHighKickDeltaZ);
                }
            } else if (doNormalKick) {
                // Normal kick
                int16_t origSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                writeMemory(A1 + kSpriteDeltaZ, 4, kNormalKickDeltaZ);
                writeMemory(A1 + kSpriteSpeed, 2, kNormalKickBallSpeed);

                // Adjust speed
                int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
                if (controlledPlDir != 0 && controlledPlDir != 4) {
                    if ((controlledPlDir & 1) != 0) {
                        int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                        int16_t reduction = speed >> 2;
                        int16_t addition = speed >> 3;
                        speed = speed - reduction + addition;
                        writeMemory(A1 + kSpriteSpeed, 2, speed);
                        logInfo("[KICK_NORMAL] Normal kick (diagonal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                                controlledPlDir, origSpeed, kNormalKickBallSpeed, speed, kNormalKickDeltaZ);
                    }
                } else {
                    int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                    speed = speed - (speed >> 2);
                    writeMemory(A1 + kSpriteSpeed, 2, speed);
                    logInfo("[KICK_NORMAL] Normal kick (cardinal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                            controlledPlDir, origSpeed, kNormalKickBallSpeed, speed, kNormalKickDeltaZ);
                }
            }
        }

        // Increment spin timer
        spinTimer = (int16_t)readMemory(A6 + kTeamSpinTimer, 2);
        spinTimer++;
        writeMemory(A6 + kTeamSpinTimer, 2, spinTimer);

        // Reset at 10
        if (spinTimer == 10) {
            writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
        }
        return;
    }

    // Pass in progress
    uint32_t opponentsTeam = readMemory(A6 + kTeamOpponentsTeam, 4);
    int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + kTeamGoalkeeperSavedCommentTimer, 2);

    if (goalkeeperTimer < 0) {
        writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
        return;
    }

    int16_t gameStatePlVal = MEM_WORD(kGameStatePl);
    if (gameStatePlVal != ST_GAME_IN_PROGRESS) {
        int16_t gameStateVal = MEM_WORD(kGameState);
        if (gameStateVal == ST_KEEPER_HOLDS_BALL) {
            writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
            return;
        }
    }

    int16_t spinTimer = (int16_t)readMemory(A6 + kTeamSpinTimer, 2);
    if (spinTimer < 0) {
        return;
    }

    // If spin timer is 0, reset all spin/pass flags
    if (spinTimer == 0) {
        writeMemory(A6 + kTeamLeftSpin, 2, 0);
        writeMemory(A6 + kTeamRightSpin, 2, 0);
        writeMemory(A6 + kTeamLongPass, 2, 0);
        writeMemory(A6 + kTeamLongSpinPass, 2, 0);
    }

    A1 = kBallSpriteOffset;

    // Check spin direction
    int16_t leftSpin = (int16_t)readMemory(A6 + kTeamLeftSpin, 2);
    int16_t rightSpin = (int16_t)readMemory(A6 + kTeamRightSpin, 2);

    int16_t spinDirection = -1;  // -1 = not spinning, 0 = left, 2 = right (element indices)

    if (leftSpin != 0) {
        spinDirection = 0;
    } else if (rightSpin != 0) {
        spinDirection = 2;
    } else {
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
        if (currentAllowedDir >= 0) {
            int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
            int16_t diff = controlledPlDir - currentAllowedDir;
            if (diff != 0) {
                diff &= 7;
                if (diff != 4) {
                    if (diff < 4) {
                        writeMemory(A6 + kTeamLeftSpin, 2, 1);
                        spinDirection = 0;
                    } else {
                        writeMemory(A6 + kTeamRightSpin, 2, 1);
                        spinDirection = 2;
                    }
                }
            }
        }
    }

    // Apply pass spin if spinning
    if (spinDirection >= 0) {
        // Use ball direction for pass spin (not player direction)
        int16_t ballDirection = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        int16_t tableIndex = (ballDirection << 2) + spinDirection;

        int16_t multiplier = kSpinMultiplierFactor[spinTimer];

        // Apply X spin
        int16_t spinX = kPassingSpinFactor[tableIndex];
        int16_t spinXFinal = (spinX * multiplier);
        int16_t destX = (int16_t)readMemory(A1 + kSpriteDestX, 2);
        writeMemory(A1 + kSpriteDestX, 2, destX + spinXFinal);

        // Apply Y spin
        int16_t spinY = kPassingSpinFactor[tableIndex + 1];
        int16_t spinYFinal = (spinY * multiplier);
        int16_t destY = (int16_t)readMemory(A1 + kSpriteDestY, 2);
        writeMemory(A1 + kSpriteDestY, 2, destY + spinYFinal);

        int16_t ballSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
        logInfo("[SPIN_PASS] spinDir=%s timer=%d mult=%d ballDir=%d tableIdx=%d spinFactor=(%d,%d) spinApplied=(%d,%d) dest:(%d,%d)->(%d,%d) ballSpeed=%d",
                spinDirection == 0 ? "LEFT" : "RIGHT", spinTimer, multiplier, ballDirection, tableIndex,
                spinX, spinY, spinXFinal, spinYFinal,
                destX, destY, destX + spinXFinal, destY + spinYFinal, ballSpeed);
    }

    // Check for long pass mechanics
    int32_t longPassFlags = (int32_t)readMemory(A6 + kTeamLongPass, 4);  // Reads both longPass and longSpinPass
    if (longPassFlags == 0) {
        // Check for starting long pass
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
        if (currentAllowedDir < 0) {
            // No direction pressed - ASM jumps to @@holding_left_or_right and applies long pass boost
            writeMemory(A6 + kTeamLongPass, 2, 1);
            int16_t oldSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
            int16_t speed = oldSpeed + (oldSpeed >> 3);
            writeMemory(A1 + kSpriteSpeed, 2, speed);
            logInfo("[PASS_LONG] Long pass (no direction): speed %d -> %d (+12.5%%)", oldSpeed, speed);
        } else {
            int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
            int16_t diff = controlledPlDir - currentAllowedDir;
            if (diff != 0) {
                diff &= 7;
                // Check for left/right (2, 6)
                if (diff == 2 || diff == 6) {
                    // Long pass - increase speed by 12.5%
                    writeMemory(A6 + kTeamLongPass, 2, 1);
                    int16_t oldSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                    int16_t speed = oldSpeed + (oldSpeed >> 3);
                    writeMemory(A1 + kSpriteSpeed, 2, speed);
                    logInfo("[PASS_LONG] Long pass: plDir=%d allowedDir=%d diff=%d speed %d -> %d (+12.5%%)",
                            controlledPlDir, currentAllowedDir, diff, oldSpeed, speed);
                } else if (diff >= 3 && diff <= 5) {
                    // Long spin pass - increase speed by 12.5%
                    writeMemory(A6 + kTeamLongSpinPass, 2, 1);
                    int16_t oldSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
                    int16_t speed = oldSpeed + (oldSpeed >> 3);
                    writeMemory(A1 + kSpriteSpeed, 2, speed);
                    logInfo("[PASS_LONGSPIN] Long spin pass: plDir=%d allowedDir=%d diff=%d speed %d -> %d (+12.5%%)",
                            controlledPlDir, currentAllowedDir, diff, oldSpeed, speed);
                }
            }
        }
    }

    // Increment spin timer
    spinTimer = (int16_t)readMemory(A6 + kTeamSpinTimer, 2);
    spinTimer++;
    writeMemory(A6 + kTeamSpinTimer, 2, spinTimer);

    if (spinTimer == 10) {
        writeMemory(A6 + kTeamSpinTimer, 2, (uint16_t)-1);
    }
}

// PlayerHittingStaticHeader - player performs standing header on the ball
// Assembly at 112247-112374
// Input: A1 -> player sprite, A6 -> team data
void PlayerHittingStaticHeader() {
    // Clear pass in progress flag
    writeMemory(A6 + kTeamPassInProgress, 2, 0);

    // Check if already at static header hit animation
    uint32_t animTable = readMemory(A1 + 18, 4);  // Sprite.animationTable
    if (animTable != kStaticHeaderHitAnimTable) {
        // Try to turn player toward current allowed direction
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
        int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        int16_t diff = currentAllowedDir - playerDir;

        if (diff != 0) {
            diff &= 7;
            if (diff != 4) {  // Not opposite direction
                if (diff >= 4) {
                    // Turn player left (counter-clockwise)
                    int16_t savedSpeed = (int16_t)readMemory(A6 + kSpriteSpeed, 2);  // Note: this reads from A6, odd but matches assembly
                    playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
                    playerDir -= 1;
                    playerDir &= 7;
                    writeMemory(A1 + kSpriteDirection, 2, playerDir);

                    // Turn twice if not at target yet
                    if (savedSpeed != playerDir) {
                        playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
                        playerDir -= 1;
                        playerDir &= 7;
                        writeMemory(A1 + kSpriteDirection, 2, playerDir);
                    }
                } else {
                    // Turn player right (clockwise)
                    int16_t savedSpeed = (int16_t)readMemory(A6 + kSpriteSpeed, 2);
                    playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
                    playerDir += 1;
                    playerDir &= 7;
                    writeMemory(A1 + kSpriteDirection, 2, playerDir);

                    // Turn twice if not at target yet
                    if (savedSpeed != playerDir) {
                        playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
                        playerDir += 1;
                        playerDir &= 7;
                        writeMemory(A1 + kSpriteDirection, 2, playerDir);
                    }
                }
            }
        }
    }

    // Set static header hit animation
    A0 = kStaticHeaderHitAnimTable;
    SetAnimationTableAndPictureIndex();

    // Get direction for ball trajectory
    int16_t direction = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
    if (direction < 0) {
        direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    }

    // Store controlled player direction
    writeMemory(A6 + kTeamControlledPlDirection, 2, direction);

    // Set ball destination using direction table
    A2 = kBallSpriteOffset;
    uint16_t tableIndex = direction << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);

    // Get destination offsets from table (kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + kSpriteDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + kSpriteDestY, 2, ballY + destOffsetY);

    // Set base ball speed
    writeMemory(A2 + kSpriteSpeed, 2, kStaticHeaderBallSpeed);

    // Get player's heading skill to adjust speed
    GetPlayerPointerFromShirtNumber();
    int8_t headingSkill = (int8_t)readMemory(A4 + kPlayerGameHeading, 1);
    int16_t speedBonus = kPlayerHeaderSpeedIncrease[headingSkill];
    int16_t currentSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    writeMemory(A2 + kSpriteSpeed, 2, currentSpeed + speedBonus);

    // Reverse and halve ball's deltaZ
    int32_t deltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    deltaZ = (-deltaZ) >> 1;
    writeMemory(A2 + kSpriteDeltaZ, 4, deltaZ);

    // Set player heading flag
    writeMemory(A1 + kSpriteHeading, 2, 1);

    // Play kick sample and reset spin timers
    SWOS::PlayKickSample();
    ResetBothTeamSpinTimers();
}

// Helper: Set jump header hit animation table if frameSwitchCounter <= 2
static void SetPlayerJumpHeaderHitAnimationTable() {
    int16_t frameSwitchCounter = (int16_t)readMemory(A1 + kSpriteFrameSwitchCounter, 2);
    if (frameSwitchCounter > 2) {
        return;
    }
    A0 = kJumpHeaderHitAnimTable;
    SetAnimationTableAndPictureIndex();
}

// Helper: DoFlyingHeader - low header trajectory
static void DoFlyingHeader() {
    int16_t oldSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    int32_t oldDeltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);

    // Set ball deltaZ to low jump height
    writeMemory(A2 + kSpriteDeltaZ, 4, kHeaderLowJumpHeight);

    // Reduce ball speed to 75%
    int16_t speed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    int16_t reduction = speed >> 2;
    writeMemory(A2 + kSpriteSpeed, 2, speed - reduction);

    logInfo("[HEADER_FLYING] DoFlyingHeader: oldDeltaZ=0x%08X -> newDeltaZ=0x%08X (kHeaderLowJumpHeight), oldSpeed=%d -> newSpeed=%d",
            oldDeltaZ, kHeaderLowJumpHeight, oldSpeed, speed - reduction);

    SetPlayerJumpHeaderHitAnimationTable();
}

// Helper: DoLobHeader - high header trajectory
static void DoLobHeader() {
    int16_t oldSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    int32_t oldDeltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);

    // Set ball deltaZ to high jump height
    writeMemory(A2 + kSpriteDeltaZ, 4, kHeaderHighJumpHeight);

    // Reduce ball speed to 93.75%
    int16_t speed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    int16_t reduction = speed >> 4;
    writeMemory(A2 + kSpriteSpeed, 2, speed - reduction);

    logInfo("[HEADER_LOB] DoLobHeader: oldDeltaZ=0x%08X -> newDeltaZ=0x%08X (kHeaderHighJumpHeight), oldSpeed=%d -> newSpeed=%d",
            oldDeltaZ, kHeaderHighJumpHeight, oldSpeed, speed - reduction);

    SetPlayerJumpHeaderHitAnimationTable();
}

// PlayerHittingJumpHeader - player performs jumping header on the ball
// Assembly at 108981-109144
// Input: A1 -> player sprite, A6 -> team data
void PlayerHittingJumpHeader() {
    // Clear pass in progress flag
    writeMemory(A6 + kTeamPassInProgress, 2, 0);

    // Get direction - use currentAllowedDirection if valid, else use player's direction
    int16_t direction = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
    int16_t origAllowedDir = direction;
    if (direction < 0) {
        direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    }

    // Setup ball sprite reference
    A2 = kBallSpriteOffset;

    // Get initial ball state for logging
    int32_t origBallDeltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    int16_t origBallSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    int16_t ballZ = (int16_t)readMemory(A2 + kSpriteZ + 2, 2);

    // Set initial ball deltaZ
    writeMemory(A2 + kSpriteDeltaZ, 4, kBallJumpHeaderDeltaZ);

    // Set ball speed to 125% of player speed
    int16_t playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    int16_t ballSpeed = playerSpeed + (playerSpeed >> 2);
    writeMemory(A2 + kSpriteSpeed, 2, ballSpeed);

    // Calculate direction difference: player direction - controls direction
    int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t diff = playerDir - direction;
    int16_t origDiff = diff;

    // Get current allowed direction again
    int16_t currentAllowedDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);

    int16_t finalDirection;
    const char* headerType = "UNKNOWN";
    if (currentAllowedDir < 0) {
        // No direction pressed - do flying header with player's direction
        headerType = "FLYING (no dir)";
        DoFlyingHeader();
        finalDirection = (int16_t)readMemory(A1 + kSpriteDirection, 2);  // Note: reads from wrong offset in asm but matches behavior
    } else {
        diff &= 7;
        if (diff == 0) {
            // Same direction - use allowed direction
            headerType = "SAME_DIR";
            finalDirection = (int16_t)readMemory(A1 + kSpriteDirection, 2);  // Note: matches assembly (reads Sprite offset)
        } else if (diff == 4) {
            // Opposite direction - lob header
            headerType = "LOB (opposite)";
            DoLobHeader();
            finalDirection = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        } else if (diff == 1) {
            // Aim left
            headerType = "FLYING aim left (diff=1)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir - 1;
        } else if (diff == 7) {
            // Aim right
            headerType = "FLYING aim right (diff=7)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir + 1;
        } else if (diff == 2) {
            // Left held - flying header aim left
            headerType = "FLYING aim left (diff=2)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir - 1;
        } else if (diff == 6) {
            // Right held - flying header aim right
            headerType = "FLYING aim right (diff=6)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir + 1;
        } else if (diff == 3) {
            // Down-left held - lob header aim left
            headerType = "LOB aim left (diff=3)";
            DoLobHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir - 1;
        } else {
            // diff == 5, down-right - lob header aim right
            headerType = "LOB aim right (diff=5)";
            DoLobHeader();
            playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
            finalDirection = playerDir + 1;
        }
    }

    // Normalize direction to 0-7
    finalDirection &= 7;

    // Store controlled player direction
    writeMemory(A6 + kTeamControlledPlDirection, 2, finalDirection);

    // Set ball destination using direction table
    uint16_t tableIndex = finalDirection << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);

    // Get destination offsets from table (kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + kSpriteDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + kSpriteDestY, 2, ballY + destOffsetY);

    // Get player's heading skill to adjust speed
    GetPlayerPointerFromShirtNumber();
    int8_t headingSkill = (int8_t)readMemory(A4 + kPlayerGameHeading, 1);
    int16_t speedBonus = kPlayerHeaderSpeedIncrease[headingSkill];
    int16_t currentSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
    writeMemory(A2 + kSpriteSpeed, 2, currentSpeed + speedBonus);

    // Halve player speed
    playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    writeMemory(A1 + kSpriteSpeed, 2, playerSpeed >> 1);

    // Set player heading flag
    writeMemory(A1 + kSpriteHeading, 2, 1);

    // Get final ball state for logging
    int32_t finalBallDeltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    int16_t finalBallSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);

    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);

    logInfo("[HEADER_JUMP] Player %d at (%d,%d): type=%s playerDir=%d allowedDir=%d diff=%d finalDir=%d",
            playerOrdinal, playerX, playerY, headerType, playerDir, origAllowedDir, origDiff, finalDirection);
    logInfo("[HEADER_JUMP]   ballPos=(%d,%d,z=%d) destOffset=(%d,%d) ballDest=(%d,%d)",
            ballX, ballY, ballZ, destOffsetX, destOffsetY, ballX + destOffsetX, ballY + destOffsetY);
    logInfo("[HEADER_JUMP]   deltaZ: 0x%08X -> 0x%08X, speed: %d -> %d (skill=%d bonus=%d)",
            origBallDeltaZ, finalBallDeltaZ, origBallSpeed, finalBallSpeed, headingSkill, speedBonus);

    // Play kick sample and reset spin timers
    SWOS::PlayKickSample();
    ResetBothTeamSpinTimers();
}

// PlayerTackledTheBallWeak - player performs weak tackle on the ball
// Assembly at 109395-109535
// Input: A1 -> player sprite, A6 -> team data
void PlayerTackledTheBallWeak() {
    // Clear pass in progress flag
    writeMemory(A6 + kTeamPassInProgress, 2, 0);

    // Get direction - use currentAllowedDirection if valid, else use player's direction
    int16_t controlDir = (int16_t)readMemory(A6 + kTeamCurrentAllowedDirection, 2);
    if (controlDir < 0) {
        controlDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    }

    A2 = kBallSpriteOffset;

    // Calculate direction difference: player direction - controls direction
    int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t diff = playerDir - controlDir;

    int16_t newDirection;
    if (diff == 0) {
        // Same direction
        newDirection = playerDir;
    } else {
        diff &= 7;
        if (diff == 4) {
            // Opposite direction
            newDirection = playerDir;
        } else if (diff < 4) {
            // Strive left
            newDirection = playerDir - 1;
        } else {
            // Strive right
            newDirection = playerDir + 1;
        }
    }

    newDirection &= 7;

    // Store controlled player direction
    writeMemory(A6 + kTeamControlledPlDirection, 2, newDirection);

    // Set ball destination using direction table
    uint16_t tableIndex = newDirection << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);

    // Get destination offsets from table (kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + kSpriteDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + kSpriteDestY, 2, ballY + destOffsetY);

    // Reduce player speed to 50% (padded to odd)
    int16_t playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    int16_t halfSpeed = playerSpeed >> 1;
    playerSpeed = playerSpeed - halfSpeed;
    playerSpeed |= 1;  // Pad to odd
    writeMemory(A1 + kSpriteSpeed, 2, playerSpeed);

    // Ball speed = 75% of player speed (player is now at 50%)
    // Actually: ball speed = playerSpeed + (playerSpeed / 2) = 1.5 * 50% = 75%
    int16_t ballSpeed = playerSpeed + (playerSpeed >> 1);
    writeMemory(A2 + kSpriteSpeed, 2, ballSpeed);

    // Set tackle state
    writeMemory(A1 + kSpriteTackleState, 2, TS_TACKLING_THE_BALL);

    // Check if opponent is far enough for good tackle
    uint32_t opponentsTeam = readMemory(A6 + kTeamOpponentsTeam, 4);
    uint32_t opponentPlayer = readMemory(opponentsTeam + kTeamControlledPlayerSprite, 4);

    if (opponentPlayer != 0) {
        // Check ball distance
        uint32_t ballDist = readMemory(opponentPlayer + kSpriteBallDistance, 4);
        if (ballDist >= 9) {
            // Calculate distance squared between players
            int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
            int16_t opponentX = (int16_t)readMemory(opponentPlayer + kSpriteX + 2, 2);
            int16_t dx = playerX - opponentX;
            // Assembly bug at 109508-109510: uses movzx instead of movsx for second operand
            // movsx eax, dx ; sign-extend: -2 -> 0xFFFFFFFE
            // movzx ebx, dx ; zero-extend: 0xFFFE -> 0x0000FFFE = 65534
            // imul ebx      ; signed multiply: (-2) * (65534) = -131068
            // This causes dxSquared to be wrong for negative dx values
            int32_t dxSquared = (int32_t)dx * (int32_t)(uint16_t)dx;

            int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
            int16_t opponentY = (int16_t)readMemory(opponentPlayer + kSpriteY + 2, 2);
            int16_t dy = playerY - opponentY;
            // Y uses correct 16-bit signed multiply (imul bx)
            int32_t dySquared = (int32_t)dy * dy;

            int32_t distSquared = dxSquared + dySquared;

            // Comparison is unsigned (jbe)
            if ((uint32_t)distSquared > 32) {
                writeMemory(A1 + kSpriteTackleState, 2, TS_GOOD_TACKLE);
            }
        }
    }

    // Play kick sample and reset spin timers
    SWOS::PlayKickSample();
    ResetBothTeamSpinTimers();
}

// PlayerKickingBall - player kicks the ball
// Assembly at 108582-108758
// Input: A1 -> player sprite, A6 -> team data
void PlayerKickingBall() {
    // Clear stateGoal
    MEM_WORD(kStateGoal) = 0;

    // Get controlled player sprite
    uint32_t controlledPlayer = readMemory(A6 + kTeamControlledPlayerSprite, 4);
    A0 = controlledPlayer;

    // Get and store direction
    int16_t direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    writeMemory(A6 + kTeamControlledPlDirection, 2, direction);

    // Get ball destination coordinates table (sets A0 to appropriate table)
    D0 = direction;
    GetBallDestCoordinatesTable();

    A2 = kBallSpriteOffset;

    // Calculate table index
    uint16_t tableIndex = (direction << 2);

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);

    // Get destination offsets from table (A0 was set by GetBallDestCoordinatesTable)
    int16_t destOffsetX = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + kSpriteDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + kSpriteDestY, 2, ballY + destOffsetY);

    // Set ball speed and deltaZ
    writeMemory(A2 + kSpriteSpeed, 2, kBallKickingSpeed);
    writeMemory(A2 + kSpriteDeltaZ, 4, kBallKickingDeltaZConst);

    ResetBothTeamSpinTimers();

    // Check if game in progress
    int16_t gameStatePlVal = MEM_WORD(kGameStatePl);
    int16_t gameStateVal = MEM_WORD(kGameState);

    if (gameStatePlVal != ST_GAME_IN_PROGRESS) {
        // Check for throw-in
        if (gameStateVal >= ST_THROW_IN_FORWARD_RIGHT && gameStateVal <= ST_THROW_IN_BACK_LEFT) {
            // Throw-in in progress - just exit without kick sample
            return;
        }
    }

    // Check if this is a shot on goal
    int16_t controlledPlDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
    bool possibleShot = false;

    if (A6 != kTopTeamData) {
        // Bottom team shooting at top goal
        if (ballY <= 342) {
            // Check direction
            if (controlledPlDir == 0 || controlledPlDir == 1 || controlledPlDir == 7) {
                possibleShot = true;
            }
        }
    } else {
        // Top team shooting at bottom goal
        if (ballY >= 556) {
            if (controlledPlDir == 4 || controlledPlDir == 3 || controlledPlDir == 5) {
                possibleShot = true;
            }
        }
    }

    if (possibleShot) {
        // Check X position for shot type
        if (ballX >= 241 && ballX <= 431) {
            // Ball is within shooting range
            bool isFinishingShot = false;
            if (ballY < 204 || ballY >= 694) {
                isFinishingShot = true;
            }

            GetPlayerPointerFromShirtNumber();

            if (isFinishingShot) {
                // Finishing shot - use finishing skill
                int8_t finishingSkill = (int8_t)readMemory(A4 + kPlayerGameFinishing, 1);
                int16_t speedBonus = kBallSpeedFinishing[finishingSkill];
                int16_t currentSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
                writeMemory(A2 + kSpriteSpeed, 2, currentSpeed + speedBonus);
            } else {
                // Long shot - use shooting skill
                int8_t shootingSkill = (int8_t)readMemory(A4 + kPlayerGameShooting, 1);
                int16_t speedBonus = kBallSpeedKicking[shootingSkill];
                int16_t currentSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
                writeMemory(A2 + kSpriteSpeed, 2, currentSpeed + speedBonus);
            }
        } else {
            // Long shot
            GetPlayerPointerFromShirtNumber();
            int8_t shootingSkill = (int8_t)readMemory(A4 + kPlayerGameShooting, 1);
            int16_t speedBonus = kBallSpeedKicking[shootingSkill];
            int16_t currentSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
            writeMemory(A2 + kSpriteSpeed, 2, currentSpeed + speedBonus);
        }
    }

    // Check if player is goalkeeper (ordinal 1) kicking left or right
    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);  // Sprite.playerOrdinal
    if (playerOrdinal == 1) {
        int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
        if (playerDir == 2 || playerDir == 6) {
            // Goalkeeper kicking left/right - skip spin timer setup
            writeMemory(A6 + kTeamPassInProgress, 2, 0);
            SWOS::PlayKickSample();
            return;
        }
    }

    // Check opponent's goalkeeper saved timer
    uint32_t opponentsTeam = readMemory(A6 + kTeamOpponentsTeam, 4);
    int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + kTeamGoalkeeperSavedCommentTimer, 2);
    if (goalkeeperTimer >= 0) {
        // Set up spin timer
        writeMemory(A6 + kTeamSpinTimer, 2, 0);
    }

    writeMemory(A6 + kTeamPassInProgress, 2, 0);
    SWOS::PlayKickSample();
}

// Goalkeeper dive constants
constexpr int16_t kKeeperSaveDistance = 16;              // Normal shot threshold (was incorrectly 96)
constexpr int16_t kKeeperPenaltySaveDistanceFar = 20;    // Used 25% of time (was incorrectly 16)
constexpr int16_t kKeeperPenaltySaveDistanceNear = 12;   // Used 75% of time (was incorrectly 48)
constexpr uint32_t kGoalkeeperDiveDeltas = 324072;       // Table of dive speeds by skill (was incorrectly 524780)
constexpr uint32_t kGoalkeeperDiveDeadVar = 337190;      // Dead var incremented during dive calc (was incorrectly 524072)
constexpr uint32_t kPlayingPenalties = 523160;
constexpr uint32_t kPenalty = 523112;
constexpr uint32_t kTeamShotChanceTable = 90;            // TeamGeneralInfo.shotChanceTable offset

// GetFramesNeededToCoverDistance - calculates frames needed to cover a distance
// Assembly at 106698-106743
// Input: D0 = movement delta (fixed point, abs taken), D4 = distance to cover (integer)
// Output: D7 = frames needed
static int16_t GetFramesNeededToCoverDistance(int32_t movementDelta, int16_t distance) {
    if (movementDelta == 0) {
        return 0;
    }

    // Take absolute value
    if (movementDelta < 0) {
        movementDelta = -movementDelta;
    }

    // D5 = num frames per subtraction, 1 if delta >= 1.0
    int16_t framesPerSubtraction = 1;

    // If delta < 1.0, scale up
    while (movementDelta < 0x10000) {
        movementDelta <<= 1;
        framesPerSubtraction <<= 1;
    }

    // Convert distance to fixed point (value.0)
    int32_t distanceFixed = ((int32_t)distance) << 16;

    // Count frames by subtracting delta until distance <= 0
    int16_t frames = 0;
    while (distanceFixed >= 0) {
        frames += framesPerSubtraction;
        distanceFixed -= movementDelta;
    }

    return frames;
}

// ShouldGoalkeeperDive - determines if goalkeeper should attempt a diving save
// Assembly at 106527-106679
// Input: A1 -> goalkeeper sprite, A2 -> ball sprite, A6 -> team data
// Output: D0 = 1 if should dive, 0 if not (also sets zero flag)
void ShouldGoalkeeperDive() {
    // Calculate ball y - goalkeeper y
    int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);
    int16_t goalkeeperY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t yDiff = ballY - goalkeeperY;

    int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t goalkeeperX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t ballSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);

    logInfo("[GK_DIVE] Called: ballPos=(%d,%d) gkPos=(%d,%d) yDiff=%d ballSpeed=%d team=%s",
            ballX, ballY, goalkeeperX, goalkeeperY, yDiff, ballSpeed,
            A6 == kTopTeamDataOffset ? "top" : "bottom");

    // Flip sign for bottom team
    if (A6 != kTopTeamDataOffset) {
        yDiff = -yDiff;
    }

    // Check if ball is behind goalkeeper
    if (yDiff < 0) {
        if (yDiff < -10) {
            // Ball too far behind - won't dive
            logInfo("[GK_DIVE] REJECT: Ball too far behind (yDiff=%d < -10)", yDiff);
            D0 = 0;
            return;
        }
        // Ball slightly behind - try saving anyway
        logInfo("[GK_DIVE] ACCEPT: Ball slightly behind, attempting save (yDiff=%d)", yDiff);
        D0 = 1;
        return;
    }

    // Ball is in front of goalkeeper
    int16_t playingPenalties = MEM_WORD(kPlayingPenalties);
    int16_t penalty = MEM_WORD(kPenalty);

    if (playingPenalties != 0 || penalty != 0) {
        // Penalty shot - use random distance threshold
        int16_t threshold = kKeeperPenaltySaveDistanceFar;  // 25% chance (default)

        // Save D0 temporarily
        int16_t savedYDiff = yDiff;

        // Call Rand
        SWOS::Rand();
        int16_t randVal = D0 & 0xFFFF;

        // Restore yDiff
        yDiff = savedYDiff;

        if ((randVal & 0x18) != 0) {
            threshold = kKeeperPenaltySaveDistanceNear;  // 75% chance
        }

        if (yDiff > threshold) {
            // Ball too far - won't dive
            logInfo("[GK_DIVE] REJECT penalty: yDiff=%d > threshold=%d", yDiff, threshold);
            D0 = 0;
            return;
        }

        // At penalty goalkeeper always dives
        logInfo("[GK_DIVE] ACCEPT penalty: yDiff=%d <= threshold=%d", yDiff, threshold);
        D0 = 1;
        return;
    }

    // Normal shot
    if (yDiff > kKeeperSaveDistance) {
        // Ball too far - won't dive
        logInfo("[GK_DIVE] REJECT: Ball too far (yDiff=%d > kKeeperSaveDistance=%d)", yDiff, kKeeperSaveDistance);
        D0 = 0;
        return;
    }

    // Calculate abs(ball y - goalkeeper y) for frame calculation
    int16_t absYDiff = ballY - goalkeeperY;
    if (absYDiff < 0) {
        absYDiff = -absYDiff;
    }

    // Get ball delta Y
    int32_t ballDeltaY = (int32_t)readMemory(A2 + kSpriteDeltaY, 4);

    // Calculate frames until ball reaches goalkeeper Y
    int16_t framesY = GetFramesNeededToCoverDistance(ballDeltaY, absYDiff);
    if (framesY == 0) {
        // Ball not moving in Y - won't dive
        logInfo("[GK_DIVE] REJECT: Ball not moving in Y (ballDeltaY=0x%08X, absYDiff=%d, framesY=0)", ballDeltaY, absYDiff);
        D0 = 0;
        return;
    }

    // Calculate abs(ball x - goalkeeper x) using ballDefensiveX
    int16_t ballDefX = MEM_WORD(kBallDefensiveX);
    int16_t absXDiff = ballDefX - goalkeeperX;
    if (absXDiff < 0) {
        absXDiff = -absXDiff;
    }

    // Get goalkeeper delta X (note: we use A1 which is the goalkeeper)
    int32_t goalkeeperDeltaX = (int32_t)readMemory(A1 + kSpriteDeltaX, 4);

    // Calculate frames until goalkeeper reaches ball X
    int16_t framesX = GetFramesNeededToCoverDistance(goalkeeperDeltaX, absXDiff);
    if (framesX == 0) {
        // Already at ball X - won't dive
        logInfo("[GK_DIVE] REJECT: Already at ball X (gkDeltaX=0x%08X, absXDiff=%d, framesX=0)", goalkeeperDeltaX, absXDiff);
        D0 = 0;
        return;
    }

    // If goalkeeper can reach ball X before ball reaches goalkeeper Y, no need to dive
    if (framesX <= framesY) {
        logInfo("[GK_DIVE] REJECT: GK can reach ball without diving (framesX=%d <= framesY=%d)", framesX, framesY);
        D0 = 0;
        return;
    }

    logInfo("[GK_DIVE] Checking dive timing: framesX=%d > framesY=%d, absXDiff=%d, ballDefX=%d, gkX=%d",
            framesX, framesY, absXDiff, ballDefX, goalkeeperX);

    // At this point goalkeeper can't reach ball by X but it's in range by Y
    // Recalculate absXDiff
    absXDiff = ballDefX - goalkeeperX;
    if (absXDiff < 0) {
        absXDiff = -absXDiff;
    }

    // Look up dive speed from shot chance table based on current game tick
    uint32_t shotChanceTable = readMemory(A6 + kTeamShotChanceTable, 4);
    int16_t tickIndex = MEM_WORD(kCurrentGameTick) & 0x0F;  // 0..15
    tickIndex <<= 1;  // multiply by 2 for word offset

    int16_t skillIndex = (int16_t)readMemory(shotChanceTable + tickIndex + 10, 2);
    skillIndex--;
    if (skillIndex < 0) {
        skillIndex = 0;
    }

    // Look up dive delta from table
    int32_t diveOffset = skillIndex << 2;  // multiply by 4
    int32_t diveDelta = (int32_t)readMemory(kGoalkeeperDiveDeltas + diveOffset, 4);

    // Increment dead var
    MEM_WORD(kGoalkeeperDiveDeadVar) += 1;

    // Calculate frames to dive to ball position
    int16_t framesDive = GetFramesNeededToCoverDistance(diveDelta, absXDiff);
    logInfo("[GK_DIVE] Dive calc: skillIndex=%d, diveDelta=0x%08X, absXDiff=%d, framesDive=%d, framesY=%d",
            skillIndex, diveDelta, absXDiff, framesDive, framesY);

    if (framesDive == 0) {
        // Can't dive - won't dive
        logInfo("[GK_DIVE] REJECT: framesDive=0 (diveDelta=0x%08X, absXDiff=%d)", diveDelta, absXDiff);
        D0 = 0;
        return;
    }

    // If dive frames >= frames until ball reaches goalkeeper Y, we can save
    if (framesDive >= framesY) {
        logInfo("[GK_DIVE] ACCEPT: framesDive=%d >= framesY=%d - WILL DIVE!", framesDive, framesY);
        D0 = 1;
        return;
    }

    // Can't reach in time - won't dive
    logInfo("[GK_DIVE] REJECT: Can't reach in time (framesDive=%d < framesY=%d)", framesDive, framesY);
    D0 = 0;
}

// PlayerTacklingTestFoul constants
constexpr uint32_t kCardsDisallowed = 455934;
constexpr uint32_t kTrainingGame = 544108;
constexpr uint32_t kPlayerCardChance = 525142;
constexpr uint32_t kDseg114EC2 = 544106;                 // Store last closest player
constexpr int kSpriteSentAway = 4;                        // Sprite.sentAway offset
constexpr int kTeamStatsPtr = 14;                         // TeamGeneralInfo.teamStatsPtr offset
constexpr int kTeamSpritesTable = 20;                     // TeamGeneralInfo.spritesTable offset
constexpr int kStatsFoulsConceded = 8;                    // TeamStatisticsData.foulsConceded offset

// PlayerTacklingTestFoul - tests if a tackle was a foul and handles cards
// Assembly at 106916-107198
// Input: A1 -> tackling player sprite, A6 -> tackling player's team
void PlayerTacklingTestFoul() {
    // Get opponent's team
    uint32_t opponentsTeam = readMemory(A6 + kTeamOpponentsTeam, 4);

    // Get opponent's controlled player sprite
    uint32_t opponentControlledSprite = readMemory(opponentsTeam + kTeamControlledPlayerSprite, 4);

    if (opponentControlledSprite == 0) {
        return;  // No opponent controlled player
    }

    // Save A2 for opponents controlled player
    uint32_t savedA2 = A2;
    A2 = opponentControlledSprite;

    // Calculate squared distance between players
    int16_t thisX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t oppX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int32_t dx = thisX - oppX;
    int32_t dxSq = dx * dx;

    int16_t thisY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t oppY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);
    int32_t dy = thisY - oppY;
    int32_t dySq = dy * dy;

    int32_t distSq = dxSq + dySq;

    if (distSq > 32) {
        A2 = savedA2;
        return;  // Players not close enough
    }

    // Check if opponent's player is goalkeeper
    int16_t oppOrdinal = (int16_t)readMemory(A2 + 2, 2);  // Sprite.playerOrdinal
    if (oppOrdinal == 1) {
        // Just slow down tackling player and return
        // Assembly does: shr speed,1; shr speed,1; or speed,1
        int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
        speed >>= 2;  // Divide by 4
        speed |= 1;   // Ensure odd (at least 1)
        writeMemory(A1 + kSpriteSpeed, 2, speed);
        A2 = savedA2;
        return;
    }

    // Check if opponent's player is within field bounds
    int16_t oppPlayerX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t oppPlayerY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);

    if (oppPlayerX < 81 || oppPlayerY < 129 || oppPlayerX > 590 || oppPlayerY > 769) {
        A2 = savedA2;
        return;  // Outside field
    }

    // Opponent's player is inside the field - slow down tackling player
    // Assembly does: shr speed,1; shr speed,1; or speed,1
    int16_t speed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    speed >>= 2;  // Divide by 4
    speed |= 1;   // Ensure odd (at least 1)
    writeMemory(A1 + kSpriteSpeed, 2, speed);

    // Call PlayerTackled for the opponent
    uint32_t savedA1 = A1;
    uint32_t savedA6 = A6;
    A1 = A2;
    A6 = opponentsTeam;
    PlayerTackled();
    A6 = savedA6;
    A1 = savedA1;

    // Check if opponent player was close to ball
    uint32_t ballDist = readMemory(A2 + kSpriteBallDistance, 4);
    if (ballDist > 800) {
        A2 = savedA2;
        return;  // Too far from ball - no foul
    }

    // Check tackle state
    int16_t tackleState = (int16_t)readMemory(A1 + kSpriteTackleState, 2);
    if (tackleState == 0) {
        // No tackle state - it's a foul
        goto foul_conceded;
    }

    if (tackleState == TS_GOOD_TACKLE) {
        // Good tackle - just play dangerous play comment
        SWOS::PlayDangerousPlayComment();
        A2 = savedA2;
        return;
    }

    {  // Scope for direction comparison variables
    // Check if players facing similar direction (tackle from behind)
    int16_t tacklerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t tackledDir = (int16_t)readMemory(A2 + kSpriteDirection, 2);
    int16_t dirDiff = tacklerDir - tackledDir;

    if (dirDiff < -1 || dirDiff > 1) {
        A2 = savedA2;
        return;  // Not facing same direction - not a foul from behind
    }
    }  // End scope for direction comparison variables

foul_conceded:
    // Record foul
    uint32_t teamStats = readMemory(A6 + kTeamStatsPtr, 4);
    int16_t fouls = (int16_t)readMemory(teamStats + kStatsFoulsConceded, 2);
    writeMemory(teamStats + kStatsFoulsConceded, 2, fouls + 1);

    // Check if cards are allowed
    int16_t cardsDisallowed = MEM_WORD(kCardsDisallowed);
    if (cardsDisallowed != 0) {
        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
        return;
    }

    int16_t trainingGame = MEM_WORD(kTrainingGame);
    if (trainingGame != 0) {
        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
        return;
    }

    // Check if in penalty area
    int16_t foulX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
    int16_t foulY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);
    bool inPenaltyArea = false;

    if (foulX >= 193 && foulX <= 478 && foulY >= 129 && foulY <= 769) {
        if (A6 == kTopTeamDataOffset) {
            if (foulY <= 216) {
                inPenaltyArea = true;
            }
        } else {
            if (foulY >= 682) {
                inPenaltyArea = true;
            }
        }
    }

    if (!inPenaltyArea) {
        // Find closest player to goal line for potential card
        int16_t goalX = 336;
        int16_t goalY = (A6 == kTopTeamDataOffset) ? 129 : 769;

        // Calculate distance from tackled player to goal
        int32_t oppDistX = foulX - goalX;
        int32_t oppDistY = foulY - goalY;
        int32_t oppDistSq = oppDistX * oppDistX + oppDistY * oppDistY;

        // Store tackled player as default closest
        uint32_t closestPlayer = A2;

        // Check all teammates of the tackling player
        uint32_t spritesTable = readMemory(A6 + kTeamSpritesTable, 4);

        for (int i = 0; i < 11; i++) {
            uint32_t playerSprite = readMemory(spritesTable + i * 4, 4);

            // Skip sent away players
            int16_t sentAway = (int16_t)readMemory(playerSprite + kSpriteSentAway, 2);
            if (sentAway != 0) continue;

            // Skip goalkeeper
            int16_t ordinal = (int16_t)readMemory(playerSprite + 2, 2);
            if (ordinal == 1) continue;

            // Skip the tackling player
            if (playerSprite == A1) continue;

            // Calculate distance to goal
            int16_t plX = (int16_t)readMemory(playerSprite + kSpriteX + 2, 2);
            int16_t plY = (int16_t)readMemory(playerSprite + kSpriteY + 2, 2);
            int32_t plDistX = plX - goalX;
            int32_t plDistY = plY - goalY;
            int32_t plDistSq = plDistX * plDistX + plDistY * plDistY;

            if (plDistSq <= oppDistSq) {
                oppDistSq = plDistSq;
                closestPlayer = playerSprite;
            }
        }

        // Store closest player
        writeMemory(kDseg114EC2, 4, closestPlayer);

        if (closestPlayer != A2) {
            // Another player closer to goal - check for card with random chance
            // In this case: rand < 32 -> direct red, else -> yellow
            goto check_card_normal;
        }

        // Tackled player IS the closest (last man scenario)
        // In assembly cseg_79444: rand < 32 -> yellow, else -> direct red (inverted!)
        goto check_card_last_man;
    }

    // In penalty area - normal card logic: rand < 32 -> direct red, else -> yellow
    {
check_card_normal:
        // Random card chance check
        int16_t gameTick = MEM_WORD(kCurrentGameTick);
        int16_t cardIndex = (gameTick & 0x1E) >> 1;  // 0..15
        int16_t cardChance = MEM_WORD(kPlayerCardChance);

        if (cardIndex >= cardChance) {
            TestFoulForPenaltyAndFreeKick();
            A2 = savedA2;
            return;
        }

        // Call Rand to determine card type
        SWOS::Rand();
        int16_t randVal = D0 & 0xFFFF;

        if (randVal < 32) {
            // Direct red card (~12.5% chance)
            TrySendingOffThePlayer();
            if ((D0 & 0xFFFF) == 0) {
                TestFoulForPenaltyAndFreeKick();
                uint32_t savedA1_2 = A1;
                SWOS::ActivateReferee();
                A1 = savedA1_2;
                A2 = savedA2;
                return;
            }
        } else {
            // Yellow card (~87.5% chance)
            TryBookingThePlayer();
            if ((D0 & 0xFFFF) == 0) {
                TestFoulForPenaltyAndFreeKick();
                uint32_t savedA1_2 = A1;
                SWOS::ActivateReferee();
                A1 = savedA1_2;
                A2 = savedA2;
                return;
            }
        }

        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
        return;
    }

check_card_last_man:
    // Last man scenario - inverted: rand < 32 -> yellow, else -> direct red
    {
        int16_t gameTick = MEM_WORD(kCurrentGameTick);
        int16_t cardIndex = (gameTick & 0x1E) >> 1;
        int16_t cardChance = MEM_WORD(kPlayerCardChance);

        if (cardIndex >= cardChance) {
            TestFoulForPenaltyAndFreeKick();
            A2 = savedA2;
            return;
        }

        SWOS::Rand();
        int16_t randVal = D0 & 0xFFFF;

        if (randVal < 32) {
            // Yellow card in last man case (~12.5% chance)
            TryBookingThePlayer();
            if ((D0 & 0xFFFF) == 0) {
                TestFoulForPenaltyAndFreeKick();
                uint32_t savedA1_2 = A1;
                SWOS::ActivateReferee();
                A1 = savedA1_2;
                A2 = savedA2;
                return;
            }
        } else {
            // Direct red card in last man case (~87.5% chance)
            TrySendingOffThePlayer();
            if ((D0 & 0xFFFF) == 0) {
                TestFoulForPenaltyAndFreeKick();
                uint32_t savedA1_2 = A1;
                SWOS::ActivateReferee();
                A1 = savedA1_2;
                A2 = savedA2;
                return;
            }
        }

        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
    }
}

// DoPass constants
constexpr uint32_t kGoodPassSampleCommand = 523670;      // Was incorrectly 524070
constexpr uint32_t kPassToPlayerPtr = 36;                // TeamGeneralInfo.passToPlayerPtr offset
constexpr uint32_t kPassingBall = 88;                    // TeamGeneralInfo.passingBall offset
constexpr uint32_t kPassingToPlayer = 90;                // TeamGeneralInfo.passingToPlayer offset
constexpr uint32_t kTeamPlayerNumber = 4;                // TeamGeneralInfo.playerNumber offset
constexpr uint32_t kAIFailedPassChance = 523884;         // Table of AI pass failure chances by skill (was incorrectly 524076)
constexpr uint32_t kBallSpeedPassingIncrease = 523818;   // Table of speed increase by passing skill (was incorrectly 524092)
constexpr uint32_t kPassInProgress = 128;                // TeamGeneralInfo.passInProgress offset (same as kTeamPassInProgress)
constexpr int kPlayerGamePassing = 69;                   // PlayerGameHeader.passing offset (was incorrectly 30)

// Passing speed constants by distance (values from memory at 523834+)
constexpr int16_t kPassingSpeedCloserThan2500 = 1536;      // Was incorrectly 1280
constexpr int16_t kPassingSpeed_2500_10000 = 1664;         // Was incorrectly 1408
constexpr int16_t kPassingSpeed_10000_22500 = 1792;        // Was incorrectly 1536
constexpr int16_t kPassingSpeed_22500_40000 = 1877;        // Was incorrectly 1664
constexpr int16_t kPassingSpeed_40000_62500 = 1962;        // Was incorrectly 1792
constexpr int16_t kPassingSpeed_62500_90000 = 2048;        // Was incorrectly 1920
constexpr int16_t kPassingSpeed_90000_122500 = 2133;       // Was incorrectly 2048
constexpr int16_t kPassingSpeedFurtherThan122500 = 2218;   // Was incorrectly 2176
constexpr int16_t kFreePassReleasingBallSpeed = 1792;      // Was incorrectly 1536

// DoPass - performs a pass to a teammate
// Assembly at 108290-108573
// Input: A1 -> player sprite, A6 -> team data
void DoPass() {
    // Reset pass variables
    MEM_WORD(kGoodPassSampleCommand) = 0;
    MEM_WORD(kStateGoal) = 0;

    // Get controlled player sprite
    uint32_t controlledPlayer = readMemory(A6 + kTeamControlledPlayerSprite, 4);
    A0 = controlledPlayer;

    // Get player direction
    int16_t direction = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    D0 = direction;
    int16_t passDirection = direction;

    // Store direction
    writeMemory(A6 + kTeamControlledPlDirection, 2, direction);

    // Save D0 and call GetPlayerPointerFromShirtNumber
    uint32_t savedD0 = D0;
    GetPlayerPointerFromShirtNumber();
    D0 = savedD0;

    // Save D7 and call GetClosestNonControlledPlayerInDirection
    int16_t savedD7 = passDirection;
    D7 = passDirection;
    GetClosestNonControlledPlayerInDirection();
    passDirection = savedD7;

    // Check if we found a player to pass to
    if (A0 == (uint32_t)-1) {
        // No closest player - free-form pass
        GetBallDestCoordinatesTable();

        A1 = kBallSpriteOffset;

        // Calculate destination based on direction
        int16_t tableIndex = passDirection << 2;
        int16_t ballX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
        int16_t destOffsetX = (int16_t)readMemory(A0 + tableIndex, 2);
        writeMemory(A1 + kSpriteDestX, 2, ballX + destOffsetX);

        int16_t ballY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
        int16_t destOffsetY = (int16_t)readMemory(A0 + tableIndex + 2, 2);
        writeMemory(A1 + kSpriteDestY, 2, ballY + destOffsetY);

        // Set ball speed for free pass
        writeMemory(A1 + kSpriteSpeed, 2, kFreePassReleasingBallSpeed);

        goto reset_spin_timers;
    }

    {  // Scope for variables that gotos may cross
    // Found a player to pass to
    uint32_t passToPlayer = A0;

    // Store pass target
    writeMemory(A6 + kPassToPlayerPtr, 4, passToPlayer);
    writeMemory(A6 + kPassingBall, 2, 1);
    writeMemory(A6 + kPassingToPlayer, 2, 1);

    // Check game state
    int16_t gameStatePlVal = MEM_WORD(kGameStatePl);
    if (gameStatePlVal != ST_GAME_IN_PROGRESS) {
        goto calculate_pass_destination;
    }

    {  // Scope for playerNumber and subsequent variables
    // Check if CPU is passing (playerNumber == 0)
    int16_t playerNumber = (int16_t)readMemory(A6 + kTeamPlayerNumber, 2);
    if (playerNumber != 0) {
        goto calculate_pass_destination;
    }

    {  // Scope for CPU pass failure calculation
    // CPU passing - check for pass failure based on skill
    int8_t passingSkill = (int8_t)readMemory(A4 + kPlayerGamePassing, 1);
    int16_t skillIndex = ((int16_t)passingSkill) << 1;

    // Get random value based on game tick
    int16_t gameTick = MEM_WORD(kCurrentGameTick);
    int16_t randVal = (gameTick & 0x1E) >> 1;  // 0..15

    // Check pass failure chance table
    int16_t failChance = (int16_t)readMemory(kAIFailedPassChance + skillIndex, 2);
    if (randVal >= failChance) {
        goto calculate_pass_destination;
    }

    // CPU botches the pass - mess up the angle
    MEM_WORD(kGoodPassSampleCommand) = (int16_t)-2;

    A1 = kBallSpriteOffset;

    {  // Scope for messed up destination calculation
    // Calculate messed up destination
    int16_t targetX = (int16_t)readMemory(passToPlayer + kSpriteX + 2, 2);
    int16_t currBallX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t deltaX = targetX - currBallX;

    // Either halve X or Y delta based on game tick bit
    if ((gameTick & 0x20) == 0) {
        deltaX >>= 1;
    }
    deltaX <<= 5;  // Multiply by 32 (or 16 if halved)
    writeMemory(A1 + kSpriteDestX, 2, currBallX + deltaX);

    int16_t targetY = (int16_t)readMemory(passToPlayer + kSpriteY + 2, 2);
    int16_t currBallY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t deltaY = targetY - currBallY;

    // Opposite condition for Y
    if ((gameTick & 0x20) != 0) {
        deltaY >>= 1;
    }
    deltaY <<= 5;
    writeMemory(A1 + kSpriteDestY, 2, currBallY + deltaY);

    goto determine_ball_speed;
    }  // End scope for messed up destination calculation
    }  // End scope for CPU pass failure calculation
    }  // End scope for playerNumber and subsequent variables

calculate_pass_destination:
    {
        A1 = kBallSpriteOffset;

        // Get delta to pass target
        int16_t passTargetX = (int16_t)readMemory(passToPlayer + kSpriteX + 2, 2);
        int16_t ballPosX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
        int16_t dx = passTargetX - ballPosX;

        int16_t passTargetY = (int16_t)readMemory(passToPlayer + kSpriteY + 2, 2);
        int16_t ballPosY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
        int16_t dy = passTargetY - ballPosY;

        // If both are 0, force dx to 1
        if (dx == 0 && dy == 0) {
            dx = 1;
        }

        // Double the distances until one goes out of bounds
        while (true) {
            int16_t testX = ballPosX + dx;
            if (testX < 0 || testX >= 672) break;

            int16_t testY = ballPosY + dy;
            if (testY < 0 || testY >= 880) break;

            dx <<= 1;
            dy <<= 1;
        }

        // Set destination
        writeMemory(A1 + kSpriteDestX, 2, ballPosX + dx);
        writeMemory(A1 + kSpriteDestY, 2, ballPosY + dy);
    }

determine_ball_speed:
    {
        // Determine ball speed based on distance
        uint32_t ballDistance = readMemory(passToPlayer + kSpriteBallDistance, 4);
        int16_t baseSpeed;

        if (ballDistance < 2500) {
            baseSpeed = kPassingSpeedCloserThan2500;
        } else if (ballDistance < 10000) {
            baseSpeed = kPassingSpeed_2500_10000;
        } else if (ballDistance < 22500) {
            baseSpeed = kPassingSpeed_10000_22500;
        } else if (ballDistance < 40000) {
            baseSpeed = kPassingSpeed_22500_40000;
        } else if (ballDistance < 62500) {
            baseSpeed = kPassingSpeed_40000_62500;
        } else if (ballDistance < 90000) {
            baseSpeed = kPassingSpeed_62500_90000;
        } else if (ballDistance < 122500) {
            baseSpeed = kPassingSpeed_90000_122500;
        } else {
            baseSpeed = kPassingSpeedFurtherThan122500;
        }

        // Add bonus based on passing skill
        int8_t passSkill = (int8_t)readMemory(A4 + kPlayerGamePassing, 1);
        int16_t skillIdx = ((int16_t)passSkill) << 1;
        int16_t speedBonus = (int16_t)readMemory(kBallSpeedPassingIncrease + skillIdx, 2);
        int16_t finalSpeed = baseSpeed + speedBonus;

        logInfo("[PASS_SPEED] dist=%u baseSpeed=%d passSkill=%d speedBonus=%d finalSpeed=%d",
                ballDistance, baseSpeed, passSkill, speedBonus, finalSpeed);

        writeMemory(A1 + kSpriteSpeed, 2, finalSpeed);
        MEM_WORD(kGoodPassSampleCommand) = (int16_t)-1;
    }
    }  // End scope for variables that gotos may cross

reset_spin_timers:
    ResetBothTeamSpinTimers();

    // If human player, set spin timer
    int16_t playerNum = (int16_t)readMemory(A6 + kTeamPlayerNumber, 2);
    if (playerNum != 0) {
        writeMemory(A6 + kTeamSpinTimer, 2, 0);
    }

    // Set pass in progress flag
    writeMemory(A6 + kPassInProgress, 2, 1);

    // Check if should play samples
    int16_t gameStateCheck = MEM_WORD(kGameStatePl);
    if (gameStateCheck == ST_GAME_IN_PROGRESS) {
        goto play_samples;
    }

    {  // Scope for throwInState
    int16_t throwInState = MEM_WORD(kGameState);
    if (throwInState < ST_THROW_IN_FORWARD_RIGHT) {
        goto play_samples;
    }
    if (throwInState > ST_THROW_IN_BACK_LEFT) {
        goto play_samples;
    }

    // Throw-in - don't play kick sample
    return;
    }  // End scope for throwInState

play_samples:
    SWOS::PlayKickSample();
    PlayStopGoodPassSampleIfNeeded();
}

// CalculateIfPlayerWinsBall constants
// NOTE: These are ABSOLUTE addresses into g_memByte
constexpr uint32_t kPlAvgTacklingBallControlDiffChance = 523932;  // Was incorrectly 524838
constexpr uint32_t kBallSpeedDeltaWhenControlled = 523916;        // Was incorrectly 524854
constexpr uint32_t kDseg17E276 = 523868;  // Ball control turn timer table (was incorrectly 524870)
constexpr uint32_t kCurrentTick = 323902;
constexpr int kTeamPlayerHasBall = 40;  // playerHasBall in TeamGeneralInfo struct
constexpr int kTeamWonTheBallTimer = 138;  // ofs138 in struct, wonTheBallTimer in assembly
constexpr int kTeamUnkBallTimer = 108;  // unkBallTimer in assembly struct

// CalculateIfPlayerWinsBall - determines if player wins contested ball
// Assembly at 108037-108281
// Input: D0 = direction, A1 -> player sprite, A6 -> team data
void CalculateIfPlayerWinsBall() {
    // Reset pass in progress
    writeMemory(A6 + kPassInProgress, 2, 0);

    // Get opponent team
    uint32_t opponentTeam = readMemory(A6 + kTeamOpponentsTeam, 4);
    A0 = opponentTeam;

    // Check if opponent's won ball timer is active
    int16_t wonBallTimer = (int16_t)readMemory(A0 + kTeamWonTheBallTimer, 2);
    if (wonBallTimer != 0) {
        goto set_team_direction;
    }

    {  // Scope for oppHasBall and oppAllowedDir
    // Check if opponent has the ball
    int16_t oppHasBall = (int16_t)readMemory(A0 + kTeamPlayerHasBall, 2);
    if (oppHasBall == 0) {
        goto set_team_direction;
    }

    // Check if opponent is moving
    int16_t oppAllowedDir = (int16_t)readMemory(A0 + kTeamCurrentAllowedDirection, 2);
    if (oppAllowedDir < 0) {
        goto set_team_direction;
    }
    }  // End scope for oppHasBall and oppAllowedDir

    // Ball can be contested - compare player skills
    // Assembly at 108055-108110
    {
        uint32_t savedD0 = D0;
        uint32_t savedD1 = D1;
        uint32_t savedA1 = A1;
        uint32_t savedA6 = A6;

        // IMPORTANT: Assembly reads OPPONENT's controlledPlayerSprite (from A0)
        // but looks it up in OUR team (A6) first!
        // This compares our player with the same number vs their controlled player.

        // First lookup: Get opponent's controlled sprite, look up in OUR team
        // Lines 108059-108064: A1 = A0.controlledPlayerSprite, then GetPlayerPointerFromShirtNumber with A6=our team
        uint32_t opponentControlledSprite = readMemory(A0 + kTeamControlledPlayerSprite, 4);
        A1 = opponentControlledSprite;

        if (A1 == 0) {
            A6 = savedA6;
            A1 = savedA1;
            D1 = savedD1;
            D0 = savedD0;
            goto set_team_direction;
        }

        // A6 is still OUR team, A1 is OPPONENT's controlled player sprite
        // This gets OUR player with the same shirt number as their controlled player
        GetPlayerPointerFromShirtNumber();
        int8_t firstTackling = (int8_t)readMemory(A4 + kPlayerGameTackling, 1);
        int8_t firstBallControl = (int8_t)readMemory(A4 + kPlayerGameBallControl, 1);
        int16_t firstAvg = (firstTackling + firstBallControl) >> 1;  // D1 in assembly

        // Second lookup: Switch A6 to opponent team, get their controlled sprite
        // Lines 108072-108080: A6 = opponentsTeam, A1 = A6.controlledPlayerSprite
        A6 = opponentTeam;
        uint32_t theirControlledSprite = readMemory(A6 + kTeamControlledPlayerSprite, 4);
        A1 = theirControlledSprite;

        if (A1 == 0) {
            A6 = savedA6;
            A1 = savedA1;
            D1 = savedD1;
            D0 = savedD0;
            goto set_team_direction;
        }

        // A6 is opponent team, A1 is opponent's controlled player sprite
        // This gets their actual controlled player
        GetPlayerPointerFromShirtNumber();
        int8_t secondTackling = (int8_t)readMemory(A4 + kPlayerGameTackling, 1);
        int8_t secondBallControl = (int8_t)readMemory(A4 + kPlayerGameBallControl, 1);
        int16_t secondAvg = (secondTackling + secondBallControl) >> 1;  // D0 in assembly

        // Calculate skill difference: D1 - D0 (first - second)
        // Lines 108089-108094
        int16_t skillDiff = firstAvg - secondAvg;
        if (skillDiff < 0) {
            // Opponent's player is more skilled - negate diff, set A6 to our team
            // (favors opponent in random check)
            skillDiff = -skillDiff;
            A6 = readMemory(A6 + kTeamOpponentsTeam, 4);  // A6 = opponent's opponent = our team
        }
        // else: Our equivalent player is more skilled - keep A6 as opponent team

        // Random chance based on skill difference
        SWOS::Rand();
        int16_t randVal = D0 & 0x1F;

        int8_t chanceThreshold = (int8_t)readMemory(kPlAvgTacklingBallControlDiffChance + skillDiff, 1);
        if (randVal >= chanceThreshold) {
            // Opponent wins - switch teams
            A6 = readMemory(A6 + kTeamOpponentsTeam, 4);
        }

        // Winner gets control
        writeMemory(A6 + kTeamWonTheBallTimer, 2, 12);

        // Stop the ball
        A2 = kBallSpriteOffset;
        writeMemory(A2 + kSpriteSpeed, 2, 0);
        int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
        writeMemory(A2 + kSpriteDestX, 2, ballX);
        int16_t ballY = (int16_t)readMemory(A2 + kSpriteY + 2, 2);
        writeMemory(A2 + kSpriteDestY, 2, ballY);

        A6 = savedA6;
        A1 = savedA1;
        D1 = savedD1;
        D0 = savedD0;

        // Assembly at 108129: jmp @@out - DOES NOT call ResetBothTeamSpinTimers
        // The spin timer reset only happens at the end of the @@set_team_direction path
        return;
    }

set_team_direction:
    // Store direction
    int16_t direction = D0 & 0xFFFF;
    writeMemory(A6 + kTeamControlledPlDirection, 2, direction);

    // A2 -> ball sprite
    A2 = kBallSpriteOffset;

    // If ball going up, make it go down
    int32_t ballDeltaZ = (int32_t)readMemory(A2 + kSpriteDeltaZ, 4);
    if (ballDeltaZ > 0) {
        writeMemory(A2 + kSpriteDeltaZ, 4, (uint32_t)-1);
    }

    // Nudge ball away from player if direction is 0 and close
    if (direction == 0) {
        int16_t ballX = (int16_t)readMemory(A2 + kSpriteX + 2, 2);
        int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
        int16_t xDiff = ballX - playerX;

        if (xDiff > -4 && xDiff < 4) {
            if (xDiff < 0) {
                writeMemory(A2 + kSpriteX + 2, 2, ballX - 1);
            } else {
                writeMemory(A2 + kSpriteX + 2, 2, ballX + 1);
            }
        }
    }

    // Set ball destination based on player position and direction
    int16_t tableIndex = direction << 2;
    int16_t playerX = (int16_t)readMemory(A1 + kSpriteX + 2, 2);
    int16_t destOffsetX = (int16_t)readMemory(kDefaultDestinations + tableIndex, 2);
    writeMemory(A2 + kSpriteDestX, 2, playerX + destOffsetX);

    int16_t playerY = (int16_t)readMemory(A1 + kSpriteY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(kDefaultDestinations + tableIndex + 2, 2);
    writeMemory(A2 + kSpriteDestY, 2, playerY + destOffsetY);

    // Get player skill for ball speed
    GetPlayerPointerFromShirtNumber();

    int16_t speedDelta = 0;
    int16_t currentTick = MEM_WORD(kCurrentTick);
    if ((currentTick & 2) != 0) {
        // Add speed based on ball control skill at odd frames
        int8_t ballControl = (int8_t)readMemory(A4 + kPlayerGameBallControl, 1);
        int16_t skillIndex = ((int16_t)ballControl) << 1;
        speedDelta = (int16_t)readMemory(kBallSpeedDeltaWhenControlled + skillIndex, 2);
    }

    // Ball speed = player speed + offset
    int16_t playerSpeed = (int16_t)readMemory(A1 + kSpriteSpeed, 2);
    writeMemory(A2 + kSpriteSpeed, 2, playerSpeed + speedDelta);

    // Check if player is turning
    int16_t fullDir = (int16_t)readMemory(A1 + kSpriteFullDirection, 2) & 0xFF;
    int16_t oppositeDir = (fullDir + 128) & 0xFF;

    int16_t playerDir = (int16_t)readMemory(A1 + kSpriteDirection, 2);
    int16_t dirAngle = playerDir << 5;

    int16_t angleDiff = oppositeDir - dirAngle;
    if ((int8_t)angleDiff >= 64 || (int8_t)angleDiff <= -64) {
        // Player turning sharply - increase ball speed
        int16_t ballSpeed = (int16_t)readMemory(A2 + kSpriteSpeed, 2);
        writeMemory(A2 + kSpriteSpeed, 2, ballSpeed + 256);
    }

    // Check if player changing direction
    int16_t controlledDir = (int16_t)readMemory(A6 + kTeamControlledPlDirection, 2);
    if (controlledDir != playerDir) {
        // Player changing direction
        int16_t unkTimer = (int16_t)readMemory(A6 + kTeamUnkBallTimer, 2);
        writeMemory(A6 + kTeamUnkBallTimer, 2, unkTimer + 1);

        // Check ball control threshold
        int8_t ballControl = (int8_t)readMemory(A4 + kPlayerGameBallControl, 1);
        int16_t skillIndex = ((int16_t)ballControl) << 1;
        int16_t threshold = (int16_t)readMemory(kDseg17E276 + skillIndex, 2);

        if (threshold <= unkTimer + 1) {
            writeMemory(A6 + kTeamWonTheBallTimer, 2, 8);
        }
    }

    ResetBothTeamSpinTimers();
}

} // namespace PlayerFunctions
