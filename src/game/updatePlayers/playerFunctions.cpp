#include "playerFunctions.h"
#include "MemoryOffsets.h"
#include "../../stdinc.h"
#include "../../util/log.h"

using namespace SwosVM;

// Memory access macros for playerFunctions.cpp
// All constants are ABSOLUTE addresses into g_memByte, NOT offsets from kMemStart.
// Therefore we access g_memByte directly, not via kMemStart (which adds 256).
#define MEM_WORD(offset) (*(int16_t*)(&g_memByte[(offset)]))

// Namespace aliases for cleaner code
namespace SpriteOff = SwosMemory::Sprite::Offsets;
namespace TeamOff = SwosMemory::Team::Offsets;
namespace TeamAddr = SwosMemory::Team::Addresses;
namespace PlayerOff = SwosMemory::PlayerGame::Offsets;
namespace GS = SwosMemory::GameState;
namespace GSVal = SwosMemory::GameState::Values;
namespace GSAddr = SwosMemory::GameState::Addresses;
namespace PS = SwosMemory::PlayerState;
namespace TS = SwosMemory::TackleState;
namespace GK = SwosMemory::Goalkeeper;
namespace Head = SwosMemory::Heading;
namespace Tackle = SwosMemory::Tackling;
namespace BallPhys = SwosMemory::Ball::Physics;
namespace BallPos = SwosMemory::Ball::Positioning;
namespace BallAddr = SwosMemory::Ball::Addresses;
namespace Pass = SwosMemory::Passing;
namespace PlayerSpd = SwosMemory::Player::SpeedTables;
namespace PlayerDisc = SwosMemory::Player::Discipline;
namespace Tbl = SwosMemory::Tables;
namespace Dead = SwosMemory::DeadVars;

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
    MEM_WORD(Dead::kThrowInDeadVar) = 1;
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
    int16_t playerOffset = (int16_t)readMemory(Tbl::kInGameTeamPlayerOffsets + tableIndex, 2);

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
    int16_t ballX = (int16_t)readMemory(BallAddr::kSpriteOffset + SpriteOff::kX + 2, 2);

    // Adjust x by +/-3 based on which half of pitch
    int16_t newX;
    if (ballX < 336) {
        newX = ballX - 3;
    } else {
        newX = ballX + 3;
    }

    // Set player x position and destination
    writeMemory(A1 + SpriteOff::kX + 2, 2, newX);
    writeMemory(A1 + SpriteOff::kDestX, 2, newX);

    // Get ball y position (whole part)
    int16_t ballY = (int16_t)readMemory(BallAddr::kSpriteOffset + SpriteOff::kY + 2, 2);

    // Set player y position and destination
    writeMemory(A1 + SpriteOff::kY + 2, 2, ballY);
    writeMemory(A1 + SpriteOff::kDestY, 2, ballY);
}

// SetPlayerDowntimeAfterTackle - sets player downtime after tackling
// Assembly at 105105-105129
// Input: A1 -> player sprite, A6 -> team data
void SetPlayerDowntimeAfterTackle() {
    // Get player game header
    GetPlayerPointerFromShirtNumber();

    // Check tacklingTimer at offset 106 in sprite
    int16_t tacklingTimer = (int16_t)readMemory(A1 + SpriteOff::kTacklingTimer, 2);

    // Select table based on whether player is human or computer controlled
    uint32_t tableOffset;
    const char* controlType;
    if (tacklingTimer == -1) {
        tableOffset = Tackle::DownTimeTables::kComputerDownTime;
        controlType = "CPU";
    } else {
        tableOffset = Tackle::DownTimeTables::kPlayerDownTime;
        controlType = "HUMAN";
    }

    // Get tackling skill from player game header (offset 72 in PlayerGameHeader)
    int8_t tackling = (int8_t)readMemory(A4 + PlayerOff::kTackling, 1);
    int16_t tableIndex = tackling * 2;

    // Look up downtime and set on sprite (offset 13 = playerDownTimer)
    // NOTE: Tackle::DownTimeTables::kPlayerDownTime and Tackle::DownTimeTables::kComputerDownTime are ABSOLUTE addresses
    // into g_memByte, so we use readMemory() which reads from g_memByte[addr] directly,
    // rather than MEM_WORD which uses kMemStart (g_memByte + 256).
    int32_t tableAddress = static_cast<int32_t>(tableOffset) + tableIndex;
    int16_t downtime = (int16_t)readMemory(tableAddress, 2);

    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);

    logDebug("[TACKLE_DOWN] Player %d at (%d,%d): tacklingTimer=%d (%s) tackleSkill=%d tableOffset=%u tableIdx=%d tableAddr=%d downtime=%d",
            playerOrdinal, playerX, playerY, tacklingTimer, controlType, tackling, tableOffset, tableIndex, tableAddress, downtime);

    writeMemory(A1 + 13, 1, downtime & 0xFF);
}

// UpdatePlayerWithBall - positions player relative to ball based on direction
// Assembly at 104616-104653
// Input: A1 -> player sprite
void UpdatePlayerWithBall() {
    // mov A0, offset ballSprite
    A0 = BallAddr::kSpriteOffset;

    // Get ball x position (whole part at offset +32 from sprite base)
    // mov ax, word ptr [esi+(Sprite.x+2)]
    int16_t ballX = (int16_t)readMemory(A0 + SpriteOff::kX + 2, 2);

    // Get ball y position (whole part)
    int16_t ballY = (int16_t)readMemory(A0 + SpriteOff::kY + 2, 2);

    // Get player direction
    // mov ax, [esi+Sprite.direction]
    int16_t direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);

    // Calculate table index (direction * 4)
    int16_t tableIndex = direction << 2;

    // mov A0, offset Tbl::kPlayerWithBallOffsets
    A0 = Tbl::kPlayerWithBallOffsets;

    // Get x and y offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new position
    int16_t newX = ballX + xOffset;
    int16_t newY = ballY + yOffset;

    // Set player x position (whole part)
    writeMemory(A1 + SpriteOff::kX + 2, 2, newX);
    // Set player y position (whole part)
    writeMemory(A1 + SpriteOff::kY + 2, 2, newY);
    // Set destX
    writeMemory(A1 + SpriteOff::kDestX, 2, newX);
    // Set destY
    writeMemory(A1 + SpriteOff::kDestY, 2, newY);

    // Reset spin timers
    ResetBothTeamSpinTimers();
}

// UpdateControllingPlayer - positions ball relative to controlling player
// Assembly at 109295-109342
// Input: A1 -> player sprite
void UpdateControllingPlayer() {
    // Get player direction
    int16_t direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    int16_t tableIndex = direction << 2;

    // mov A0, offset Tbl::kBallPlOffsets
    A0 = Tbl::kBallPlOffsets;

    // Get player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);

    // Get offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new ball position
    int16_t newX = playerX + xOffset;
    int16_t newY = playerY + yOffset;

    // mov A0, offset ballSprite
    A0 = BallAddr::kSpriteOffset;

    // Set ball speed to 0
    writeMemory(A0 + SpriteOff::kSpeed, 2, 0);
    // Set ball x position (whole part)
    writeMemory(A0 + SpriteOff::kX + 2, 2, newX);
    // Set ball y position (whole part)
    writeMemory(A0 + SpriteOff::kY + 2, 2, newY);
    // Set ball destX
    writeMemory(A0 + SpriteOff::kDestX, 2, newX);
    // Set ball destY
    writeMemory(A0 + SpriteOff::kDestY, 2, newY);

    // Set ball z to 0 (whole part)
    writeMemory(A0 + SpriteOff::kZ + 2, 2, 0);

    // Halve ball deltaZ (arithmetic shift right)
    int32_t deltaZ = (int32_t)readMemory(A0 + SpriteOff::kDeltaZ, 4);
    deltaZ >>= 1;
    writeMemory(A0 + SpriteOff::kDeltaZ, 4, deltaZ);

    // Reset spin timers
    ResetBothTeamSpinTimers();
}

// GoalkeeperCaughtTheBall - handles goalkeeper catching the ball
// Assembly at 106751-106795
// Input: A1 -> goalkeeper sprite, A6 -> team data
void GoalkeeperCaughtTheBall() {
    // Set direction based on which team
    int16_t direction;
    if (A6 == TeamAddr::kTopTeamData) {
        direction = 4;  // Down
    } else {
        direction = 0;  // Up
    }
    writeMemory(A1 + SpriteOff::kDirection, 2, direction);

    // Clear goalkeeperJumpingLeft (offset 82 in TeamGeneralInfo)
    writeMemory(A6 + 82, 2, 0);

    // Set animation table
    A0 = GK::AnimationTables::kCatchingBall;
    SetPlayerAnimationTable();

    // Set player state (offset 12) to PL_GOALIE_CATCHING_BALL (4)
    writeMemory(A1 + 12, 1, 4);
    // Set playerDownTimer (offset 13) to 15
    writeMemory(A1 + 13, 1, 15);

    // Set speed from constant
    int16_t speed = MEM_WORD(GK::Speeds::kCatchSpeed);
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);

    // Set destination to predicted ball landing position
    int16_t destX = MEM_WORD(BallPos::kNextGroundX);
    int16_t destY = MEM_WORD(BallPos::kNextGroundY);

    writeMemory(A1 + SpriteOff::kDestX, 2, destX);

    // Clamp destY to valid pitch range (137-761)
    if (destY <= 137) {
        destY = 137;
    } else if (destY >= 761) {
        destY = 761;
    }
    writeMemory(A1 + SpriteOff::kDestY, 2, destY);
}

// UpdateBallWithControllingGoalkeeper - positions ball relative to controlling goalkeeper
// Assembly at 107974-108023
// Input: A1 -> goalkeeper sprite
void UpdateBallWithControllingGoalkeeper() {
    // Get keeper direction
    int16_t direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    int16_t tableIndex = direction << 2;

    // mov A0, offset Tbl::kBallPlOffsets
    A0 = Tbl::kBallPlOffsets;

    // Get keeper position (whole part)
    int16_t keeperX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t keeperY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);

    // Get offsets from table
    int16_t xOffset = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t yOffset = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Calculate new ball position
    int16_t newX = keeperX + xOffset;
    int16_t newY = keeperY + yOffset;

    // mov A0, offset ballSprite
    A0 = BallAddr::kSpriteOffset;

    // Set ball speed to 0
    writeMemory(A0 + SpriteOff::kSpeed, 2, 0);
    // Set ball x position (whole part)
    writeMemory(A0 + SpriteOff::kX + 2, 2, newX);
    // Set ball y position (whole part)
    writeMemory(A0 + SpriteOff::kY + 2, 2, newY);
    // Set ball destX
    writeMemory(A0 + SpriteOff::kDestX, 2, newX);
    // Set ball destY
    writeMemory(A0 + SpriteOff::kDestY, 2, newY);

    // Halve deltaZ and make it negative if positive
    // sar D0, 1 / js @@delta_z_negative / neg D0
    int32_t deltaZ = (int32_t)readMemory(A0 + SpriteOff::kDeltaZ, 4);
    deltaZ >>= 1;  // arithmetic shift right
    if (deltaZ >= 0) {
        deltaZ = -deltaZ;
    }
    writeMemory(A0 + SpriteOff::kDeltaZ, 4, deltaZ);

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
    writeMemory(A1 + SpriteOff::kDirection, 2, direction);

    // Calculate table index (direction * 4)
    int16_t tableIndex = direction << 2;

    // Get destination offsets from Tbl::kDefaultDestinations table
    // NOTE: Tbl::kDefaultDestinations is an absolute address, use readMemory()
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);

    // Get current player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);

    // Set destination
    writeMemory(A1 + SpriteOff::kDestX, 2, playerX + destOffsetX);
    writeMemory(A1 + SpriteOff::kDestY, 2, playerY + destOffsetY);

    // Set speed from constant
    int16_t speed = MEM_WORD(Head::Speeds::kStaticHeaderPlayer);
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);

    // Set animation table
    A0 = Head::AnimationTables::kStaticAttempt;
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
    if (playerState != PS::kNormal) {
        return;
    }

    // Check if game is in progress and player is goalkeeper
    int16_t gameStatePl = (int16_t)readMemory(GSAddr::kGameStatePl, 2);
    if (gameStatePl == GSVal::kGameInProgress) {
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
    A0 = PlayerSpd::kGameInProgress;
    if ((int16_t)readMemory(GSAddr::kGameStatePl, 2) != GSVal::kGameInProgress) {
        A0 = PlayerSpd::kGameStopped;
    }

    int16_t speed = (int16_t)readMemory(A0 + tableIndex, 2);
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);

    // Check runSlower flag - reduce speed by 3/8 if set
    int16_t runSlower = (int16_t)readMemory(GSAddr::kRunSlower, 2);
    if (runSlower != 0) {
        speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
        int16_t reduction = speed >> 2;  // speed / 4
        speed -= reduction;
        reduction >>= 1;  // speed / 8
        speed -= reduction;
        writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
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
            int16_t handicap = (int16_t)readMemory(PlayerSpd::kInjuriesHandicap + injuryIndex, 2);
            speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            speed += handicap;  // handicap is negative
            writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
        }
    }

player_not_injured:

    // If this is the controlled player with the ball, reduce speed slightly
    uint32_t controlledSprite = readMemory(A6 + 32, 4);
    if (A1 == controlledSprite) {
        int16_t playerHasBall = (int16_t)readMemory(A6 + 40, 2);
        if (playerHasBall != 0) {
            speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            int16_t reduction = speed >> 3;  // speed / 8
            speed -= reduction;
            writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
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
                    int16_t ballSpeed = (int16_t)readMemory(BallAddr::kSpriteOffset + SpriteOff::kSpeed, 2);
                    if (ballSpeed != 0) {
                        int8_t ballDir = (int8_t)readMemory(BallAddr::kSpriteOffset + SpriteOff::kFullDirection, 1);
                        int8_t playerDir = (int8_t)readMemory(A1 + SpriteOff::kFullDirection, 1);
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
                            writeMemory(A1 + SpriteOff::kSpeed, 2, passSpeed);
                        }
                    }
                }
            }
        }
    }

    // Handle special game states
    int16_t gameState = (int16_t)readMemory(GSAddr::kGameState, 2);
    gameStatePl = (int16_t)readMemory(GSAddr::kGameStatePl, 2);

    if (gameStatePl != GSVal::kGameInProgress) {
        if (gameState == GSVal::kFirstHalfEnded || gameState == GSVal::kGameEnded) {
            // Half or game end - reduce speed based on stoppage timer
            int16_t stoppageTimer = (int16_t)readMemory(GSAddr::kStoppageTimerTotal, 2);
            int16_t reduction = stoppageTimer << 5;
            speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            speed -= reduction;
            if (speed < 0) {
                speed = 0;
            }
            writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
        } else if (gameState == GSVal::kGoingToHalftime || gameState == GSVal::kPlayersGoingToShower) {
            // Players leaving pitch - scale speed by stoppage timer percentage
            int16_t stoppageTimer = (int16_t)readMemory(GSAddr::kStoppageTimerTotal, 2);
            int16_t percent = stoppageTimer << 2;
            if (percent > 100) {
                percent = 100;
            }
            speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            speed = (speed * percent) / 100;
            writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
        }
    }

    // Calculate frame delay based on speed (slower = higher delay)
    speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
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
    uint32_t ballDistance = readMemory(A1 + SpriteOff::kBallDistance, 4);
    int16_t gkSpeed = 0;

    if (ballDistance <= 128) {
        // Ball is close - use near jump speed
        int16_t nearSpeed = (int16_t)readMemory(GK::Speeds::kNearJumpSpeed, 2);
        D1 = nearSpeed;
        writeMemory(A1 + SpriteOff::kSpeed, 2, nearSpeed);
        gkSpeed = nearSpeed;
    } else {
        // Ball is far away
        int16_t farSpeed = (int16_t)readMemory(GK::Speeds::kFarJumpSpeed, 2);
        writeMemory(A1 + SpriteOff::kSpeed, 2, farSpeed);
        gkSpeed = farSpeed;

        int16_t d1Val = (int16_t)(D1 & 0xFFFF);
        if (d1Val != 0) {
            // Use slower speed
            int16_t slowerSpeed = (int16_t)readMemory(GK::Speeds::kFarJumpSlowerSpeed, 2);
            writeMemory(A1 + SpriteOff::kSpeed, 2, slowerSpeed);
            gkSpeed = slowerSpeed;

            if (d1Val != 1) {
                // Add randomness based on game tick
                int16_t gameTick = (int16_t)readMemory(GSAddr::kCurrentGameTick, 2);
                int16_t randomComponent = gameTick & 0xFF;
                int16_t nearJumpSpeed = (int16_t)readMemory(GK::Speeds::kNearJumpSpeed, 2);
                int16_t finalSpeed = randomComponent + nearJumpSpeed;
                writeMemory(A1 + SpriteOff::kSpeed, 2, finalSpeed);
                gkSpeed = finalSpeed;
            }
        }
    }

    // Calculate x difference between ball (A2) and goalkeeper (A1)
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t keeperX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t xDiff = ballX - keeperX;
    (void)xDiff;  // Used for direction determination in original

    // Clear some team fields
    writeMemory(A6 + 70, 1, 0);   // TeamGeneralInfo.field_46
    writeMemory(A6 + 80, 2, 0);   // TeamGeneralInfo.goalkeeperDivingRight

    // Check ball height to determine diving high or low
    int16_t ballNotHighZ = (int16_t)readMemory(BallPos::kNotHighZ, 2);
    const char* diveType = nullptr;

    if (ballNotHighZ <= 5) {
        // Diving low
        writeMemory(A1 + 12, 1, PS::kGoalieDivingLow);
        diveType = "LOW";

        if (A6 == TeamAddr::kBottomTeamData) {
            A0 = GK::AnimationTables::kRightJumpingLow;
        } else {
            A0 = GK::AnimationTables::kLeftJumpingLow;
        }
    } else {
        // Diving high
        writeMemory(A1 + 12, 1, PS::kGoalieDivingHigh);
        diveType = "HIGH";

        if (A6 == TeamAddr::kBottomTeamData) {
            A0 = GK::AnimationTables::kRightJumpingHigh;
        } else {
            A0 = GK::AnimationTables::kLeftJumpingHigh;
        }
    }

    SetPlayerAnimationTable();

    // Set down timer
    writeMemory(A1 + 13, 1, 75);  // Sprite.playerDownTimer

    // Store controlled player direction
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A6 + 56, 2, direction);  // TeamGeneralInfo.controlledPlDirection

    // Calculate destination from direction using Tbl::kDefaultDestinations table
    int16_t destDirection = (int16_t)(D3 & 0xFFFF);
    uint16_t tableIndex = destDirection << 2;

    // Get x offset from table and add to keeper position
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t newDestX = destOffsetX + keeperX;
    writeMemory(A1 + SpriteOff::kDestX, 2, newDestX);

    // Get y offset from table and add to keeper position
    int16_t keeperY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);
    int16_t newDestY = destOffsetY + keeperY;
    writeMemory(A1 + SpriteOff::kDestY, 2, newDestY);

    logDebug("[GK_JUMP] DIVING %s! ballDist=%d gkSpeed=%d ballZ=%d dir=%d destDir=%d gkPos=(%d,%d) dest=(%d,%d)",
            diveType, ballDistance, gkSpeed, ballNotHighZ, direction, destDirection,
            keeperX, keeperY, newDestX, newDestY);
}

// PlayerBeginTackling - initiates a sliding tackle
// Assembly at 106261-106335
// Input: D0 -> direction, A1 -> player sprite, A6 -> team data
void PlayerBeginTackling() {
    // Clear tackle state
    writeMemory(A1 + SpriteOff::kTackleState, 2, 0);

    // Store direction
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A6 + 56, 2, direction);  // TeamGeneralInfo.controlledPlDirection
    writeMemory(A1 + SpriteOff::kDirection, 2, direction);

    // Set tackling animation
    A0 = Tackle::Addresses::kAnimTable;
    SetPlayerAnimationTable();

    // Set player state to tackling
    writeMemory(A1 + 12, 1, PS::kTackling);

    // Set initial down timer from global
    uint8_t downInterval = (uint8_t)readMemory(Tackle::Addresses::kDownInterval, 1);
    writeMemory(A1 + 13, 1, downInterval);

    // Get player game header to check fasterTackle
    int16_t ordinal = (int16_t)readMemory(A1 + 2, 2);
    int16_t index = ordinal - 1;
    uint16_t tableIndex = index << 1;
    int16_t playerOffset = (int16_t)readMemory(Tbl::kInGameTeamPlayerOffsets + tableIndex, 2);
    uint32_t teamPtr = readMemory(A6 + 10, 4);
    A0 = teamPtr + playerOffset;

    // Note: Assembly also checks fasterTackle and sets to 25, but then ALWAYS
    // overwrites with -1 on lines 106302-106303 (dead code in original).
    // The -1 value means the tackle ends via speed reaching 0 (calling
    // SetPlayerDowntimeAfterTackle) rather than playerDownTimer reaching 0.
    writeMemory(A1 + 13, 1, (uint8_t)-1);

    // Calculate destination from direction using Tbl::kDefaultDestinations table
    uint16_t destTableIndex = direction << 2;

    // Get x offset and add to player position
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + destTableIndex, 2);
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t newDestX = destOffsetX + playerX;
    writeMemory(A1 + SpriteOff::kDestX, 2, newDestX);

    // Get y offset and add to player position
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + destTableIndex + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t newDestY = destOffsetY + playerY;
    writeMemory(A1 + SpriteOff::kDestY, 2, newDestY);

    // Set tackling speed
    int16_t tacklingSpeed = (int16_t)readMemory(Tackle::Addresses::kSpeed, 2);
    writeMemory(A1 + SpriteOff::kSpeed, 2, tacklingSpeed);

    // Clear tackling timer
    writeMemory(A1 + SpriteOff::kTacklingTimer, 2, 0);
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
        controlDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    }

    // Set A2 to ball sprite
    A2 = BallAddr::kSpriteOffset;

    // Calculate deflection based on difference between player direction and control direction
    int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
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
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t newDestX = destOffsetX + ballX;
    writeMemory(A2 + SpriteOff::kDestX, 2, newDestX);

    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);
    int16_t newDestY = destOffsetY + ballY;
    writeMemory(A2 + SpriteOff::kDestY, 2, newDestY);

    // Set ball speed based on whether player is human or CPU
    int16_t playerNumber = (int16_t)readMemory(A6 + 4, 2);
    int16_t playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);

    if (playerNumber == 0) {
        // CPU player - ball speed = player speed (100%)
        writeMemory(A2 + SpriteOff::kSpeed, 2, playerSpeed);
    } else {
        // Human player - ball speed = 125% of player speed
        int16_t ballSpeed = (playerSpeed >> 2) + playerSpeed;
        writeMemory(A2 + SpriteOff::kSpeed, 2, ballSpeed);
        // Apply 125% again (original code does this twice)
        ballSpeed = (playerSpeed >> 2) + playerSpeed;
        writeMemory(A2 + SpriteOff::kSpeed, 2, ballSpeed);
    }

    // Halve player speed
    playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    writeMemory(A1 + SpriteOff::kSpeed, 2, playerSpeed >> 1);

    // Set tackle state to 1 (tackled)
    writeMemory(A1 + SpriteOff::kTackleState, 2, 1);

    // Check for good tackle - get opponent's controlled player
    uint32_t opponentTeam = readMemory(A6, 4);  // opponentsTeam
    uint32_t opponentPlayer = readMemory(opponentTeam + 32, 4);  // controlledPlayerSprite

    if (opponentPlayer != 0) {
        // Check if opponent is far enough from ball (>= 9)
        uint32_t opponentBallDist = readMemory(opponentPlayer + SpriteOff::kBallDistance, 4);
        if (opponentBallDist >= 9) {
            // Calculate squared distance between players
            int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
            int16_t opponentX = (int16_t)readMemory(opponentPlayer + SpriteOff::kX + 2, 2);
            int16_t dx = playerX - opponentX;
            int32_t dxSquared = (int32_t)dx * dx;

            int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
            int16_t opponentY = (int16_t)readMemory(opponentPlayer + SpriteOff::kY + 2, 2);
            int16_t dy = playerY - opponentY;
            int32_t dySquared = (int32_t)dy * dy;

            int32_t distSquared = dxSquared + dySquared;

            // If distance > 32 (squared), it's a good tackle
            if (distSquared > 32) {
                SWOS::PlayGoodTackleComment();
                writeMemory(A1 + SpriteOff::kTackleState, 2, 2);  // TS::kGoodTackle
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
    if (animTable == Head::AnimationTables::kJumpHit) {
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
    uint16_t gameTick = (uint16_t)readMemory(GSAddr::kCurrentGameTick, 2);
    if ((gameTick & 0x0200) == 0) {  // bit 1 of high byte = bit 9 overall
        return;
    }

    // All conditions passed - set animation table and halve speed
    A0 = Head::AnimationTables::kJumpHit;
    SetAnimationTableAndPictureIndex();

    // Halve speed
    int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed >> 1);
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
    if (animTable != Head::AnimationTables::kStaticAttempt) {
        return;
    }

    // Get player direction and calculate difference
    int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
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
        playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        playerDir += 1;
    } else {
        // Turn left (subtract 1)
        playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        playerDir -= 1;
    }

    // Normalize direction to 0-7
    playerDir &= 7;
    writeMemory(A1 + SpriteOff::kDirection, 2, playerDir);
}

// PlayerAttemptingJumpHeader - sets up player to attempt a jump header
// Assembly at 108929-108970
// Input: D0 -> direction, A1 -> player sprite
void PlayerAttemptingJumpHeader() {
    // Clear heading field (offset 98)
    writeMemory(A1 + 98, 2, 0);

    // Set player direction from D0
    int16_t direction = (int16_t)(D0 & 0xFFFF);
    writeMemory(A1 + SpriteOff::kDirection, 2, direction);

    // Set jump header attempt animation
    A0 = Head::AnimationTables::kJumpAttempt;
    SetPlayerAnimationTable();

    // Set player down timer to 55
    writeMemory(A1 + 13, 1, 55);

    // Set player state to PS::kJumpHeading (9)
    writeMemory(A1 + 12, 1, PS::kJumpHeading);

    // Calculate destination using Tbl::kDefaultDestinations table
    uint16_t tableIndex = direction << 2;

    // Get destination offsets (Tbl::kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);

    // Get current player position (whole part)
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);

    // Set destination
    writeMemory(A1 + SpriteOff::kDestX, 2, playerX + destOffsetX);
    writeMemory(A1 + SpriteOff::kDestY, 2, playerY + destOffsetY);

    // Set speed from Head::Speeds::kJumpHeader constant
    int16_t speed = MEM_WORD(Head::Speeds::kJumpHeader);
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
}

// AI_Kick - AI decides whether to kick/tackle based on opponent proximity
// Assembly at 119373-119441
// Input: A1 -> player sprite, A6 -> team data
void AI_Kick() {
    // Check deadVarAlways0 - if non-zero, check team restrictions
    int16_t deadVar = MEM_WORD(Dead::kDeadVarAlways0);
    if (deadVar != 0) {
        uint32_t dseg1309c1 = readMemory(Dead::kDseg1309C1, 4);
        if (A6 == dseg1309c1) {
            return;
        }

        uint32_t opponentTeam = readMemory(A6, 4);  // TeamGeneralInfo.opponentsTeam
        if (opponentTeam == dseg1309c1) {
            return;
        }
    }

    // Check ball distance - exit if > 200
    uint32_t ballDistance = readMemory(A1 + SpriteOff::kBallDistance, 4);
    if (ballDistance > 200) {
        return;
    }

    // Save player's direction in D7
    int16_t myDirection = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);

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
    int16_t opponentDir = (int16_t)readMemory(opponentPlayer + SpriteOff::kDirection, 2);

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
    int32_t ballX = (int32_t)readMemory(A2 + SpriteOff::kX, 4);
    int32_t ballY = (int32_t)readMemory(A2 + SpriteOff::kY, 4);
    int32_t ballZ = (int32_t)readMemory(A2 + SpriteOff::kZ, 4);
    int32_t deltaX = (int32_t)readMemory(A2 + SpriteOff::kDeltaX, 4);
    int32_t deltaY = (int32_t)readMemory(A2 + SpriteOff::kDeltaY, 4);

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
        MEM_WORD(BallPos::kNextGroundX) = -1;
        return;
    }

    // Ball is moving - simulate trajectory
    int32_t deltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    int32_t gravity = (int32_t)readMemory(BallPhys::kGravityConstant, 4);
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
                MEM_WORD(BallPos::kNextGroundX) = -1;
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
    MEM_WORD(BallPos::kNextGroundX) = groundX;
    MEM_WORD(BallPos::kNextGroundY) = groundY;
    MEM_WORD(BallPos::kNextZDeadVar) = 0;
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
    int32_t ballX = (int32_t)readMemory(A2 + SpriteOff::kX, 4);
    int32_t ballY = (int32_t)readMemory(A2 + SpriteOff::kY, 4);
    int32_t ballZ = (int32_t)readMemory(A2 + SpriteOff::kZ, 4);
    int32_t deltaX = (int32_t)readMemory(A2 + SpriteOff::kDeltaX, 4);
    int32_t deltaY = (int32_t)readMemory(A2 + SpriteOff::kDeltaY, 4);
    int32_t deltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    int32_t gravity = (int32_t)readMemory(BallPhys::kGravityConstant, 4);

    // Determine ball direction
    bool ballGoingUp = deltaY < -0x10000;      // < -1.0
    bool ballGoingDown = deltaY > 0x10000;     // > 1.0
    bool ballGoingLeft = deltaX < -0x10000;    // < -1.0
    bool ballGoingRight = deltaX > 0x10000;    // > 1.0

    if (ballGoingUp) {
        // Ball going toward top team's goal - only process for top team
        if (A6 != TeamAddr::kTopTeamData) {
            goto ball_not_moving;
        }

        int32_t playerY = (int32_t)readMemory(A1 + SpriteOff::kY, 4);
        int32_t relY = ballY - playerY;

        if (relY < 0) {
            // Ball is closer to goal than player - use current ball position
            MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(ballX);
            MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(ballY);
            MEM_WORD(BallPos::kDefensiveZ) = fixedToWhole(ballZ);
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

            MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(simY);
            MEM_WORD(BallPos::kDefensiveZ) = clampZPositive(fixedToWhole(simZ));
            relY = simY;
        }

        // Check if ball is above Y=135
        if (relY - 0x870000 < 0) {  // 135.0
            MEM_WORD(BallPos::kNotHighX) = MEM_WORD(BallPos::kDefensiveX);
            MEM_WORD(BallPos::kNotHighY) = MEM_WORD(BallPos::kDefensiveY);
            MEM_WORD(BallPos::kNotHighZ) = MEM_WORD(BallPos::kDefensiveZ);
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
            MEM_WORD(BallPos::kNotHighX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kNotHighY) = 135;
            MEM_WORD(BallPos::kNotHighZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check strike destination (Y=129)
        if (relY - 0x810000 < 0) {  // 129.0
            MEM_WORD(BallPos::kStrikeDestX) = 0;
            MEM_WORD(BallPos::kStrikeDestY) = 0;
            MEM_WORD(BallPos::kStrikeDestZ) = 0;
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
            MEM_WORD(BallPos::kStrikeDestX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kStrikeDestY) = 129;
            MEM_WORD(BallPos::kStrikeDestZ) = clampZPositive(fixedToWhole(simZ));
        }
        return;
    }

    if (ballGoingDown) {
        // Ball going toward bottom team's goal - only process for bottom team
        if (A6 != TeamAddr::kBottomTeamData) {
            goto ball_not_moving;
        }

        int32_t playerY = (int32_t)readMemory(A1 + SpriteOff::kY, 4);
        int32_t relY = ballY - playerY;

        if (relY >= 0) {
            // Ball is closer to goal than player
            MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(ballX);
            MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(ballY);
            MEM_WORD(BallPos::kDefensiveZ) = fixedToWhole(ballZ);
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

            MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(simY);
            MEM_WORD(BallPos::kDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check if ball reaches Y=763
        int32_t relY763 = ballY - 0x2FB0000;  // 763.0
        if (relY763 >= 0) {
            MEM_WORD(BallPos::kNotHighX) = MEM_WORD(BallPos::kDefensiveX);
            MEM_WORD(BallPos::kNotHighY) = MEM_WORD(BallPos::kDefensiveY);
            MEM_WORD(BallPos::kNotHighZ) = MEM_WORD(BallPos::kDefensiveZ);
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
            MEM_WORD(BallPos::kNotHighX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kNotHighY) = 763;
            MEM_WORD(BallPos::kNotHighZ) = clampZPositive(fixedToWhole(simZ));
        }

        // Check strike destination (Y=769)
        int32_t relY769 = ballY - 0x3010000;  // 769.0
        if (relY769 >= 0) {
            MEM_WORD(BallPos::kStrikeDestX) = 0;
            MEM_WORD(BallPos::kStrikeDestY) = 0;
            MEM_WORD(BallPos::kStrikeDestZ) = 0;
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
            MEM_WORD(BallPos::kStrikeDestX) = fixedToWhole(simX);
            MEM_WORD(BallPos::kStrikeDestY) = 769;
            MEM_WORD(BallPos::kStrikeDestZ) = clampZPositive(fixedToWhole(simZ));
        }
        return;
    }

    if (ballGoingLeft) {
        int32_t playerX = (int32_t)readMemory(A1 + SpriteOff::kX, 4);
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

        MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(simX);
        MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(simY);
        MEM_WORD(BallPos::kDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        goto set_not_high_from_defensive;
    }

    if (ballGoingRight) {
        int32_t playerX = (int32_t)readMemory(A1 + SpriteOff::kX, 4);
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

        MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(simX);
        MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(simY);
        MEM_WORD(BallPos::kDefensiveZ) = clampZPositive(fixedToWhole(simZ));
        goto set_not_high_from_defensive;
    }

ball_not_moving:
    // Ball not moving significantly - use current position
    MEM_WORD(BallPos::kDefensiveX) = fixedToWhole(ballX);
    MEM_WORD(BallPos::kDefensiveY) = fixedToWhole(ballY);
    MEM_WORD(BallPos::kDefensiveZ) = fixedToWhole(ballZ);

set_not_high_from_defensive:
    MEM_WORD(BallPos::kNotHighX) = MEM_WORD(BallPos::kDefensiveX);
    MEM_WORD(BallPos::kNotHighY) = MEM_WORD(BallPos::kDefensiveY);
    MEM_WORD(BallPos::kNotHighZ) = MEM_WORD(BallPos::kDefensiveZ);
    MEM_WORD(BallPos::kStrikeDestX) = 0;
    MEM_WORD(BallPos::kStrikeDestY) = 0;
    MEM_WORD(BallPos::kStrikeDestZ) = 0;
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


// Player header speed increase table (indexed by heading skill 0-12)
static const int16_t kPlayerHeaderSpeedIncrease[13] = {
    -336, -288, -240, -192, -144, -96, -48, 0, 513, 1027, 1541, 2055, 2569
};


// Ball speed tables (indexed by shooting/finishing skill 0-7)
static const int16_t kBallSpeedKicking[8] = {
    (int16_t)65152, (int16_t)65266, (int16_t)65374, (int16_t)65482, 54, 162, 270, 384
};
static const int16_t kBallSpeedFinishing[8] = {
    (int16_t)65248, (int16_t)65376, (int16_t)65504, 96, 224, 352, 480, 608
};


// ApplyBallAfterTouch - applies spin/aftertouch to ball during kicks and passes
// Assembly at 104681-105093
// Input: A6 -> team data (TeamGeneralInfo)
void ApplyBallAfterTouch() {
    // Check if pass is in progress
    int16_t passInProgress = (int16_t)readMemory(A6 + TeamOff::kPassInProgress, 2);

    if (passInProgress == 0) {
        // Not passing - it's a kick
        // Check if opponent's goalkeeper saved
        uint32_t opponentsTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
        int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + TeamOff::kGoalkeeperSavedCommentTimer, 2);

        if (goalkeeperTimer < 0) {
            // Reset spin timer and exit
            writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
            return;
        }

        // Check game state
        int16_t gameStatePlVal = MEM_WORD(GSAddr::kGameStatePl);
        if (gameStatePlVal != GSVal::kGameInProgress) {
            int16_t gameStateVal = MEM_WORD(GSAddr::kGameState);
            if (gameStateVal == GSVal::kKeeperHoldsBall) {
                writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
                return;
            }
        }

        // Check spin timer
        int16_t spinTimer = (int16_t)readMemory(A6 + TeamOff::kSpinTimer, 2);
        if (spinTimer < 0) {
            return;  // Spin not active
        }

        // If spin timer is 0, reset spin flags
        if (spinTimer == 0) {
            writeMemory(A6 + TeamOff::kLeftSpin, 2, 0);
            writeMemory(A6 + TeamOff::kRightSpin, 2, 0);
        }

        // A1 -> ball sprite
        A1 = BallAddr::kSpriteOffset;

        // Check if already spinning
        int16_t leftSpin = (int16_t)readMemory(A6 + TeamOff::kLeftSpin, 2);
        int16_t rightSpin = (int16_t)readMemory(A6 + TeamOff::kRightSpin, 2);

        int16_t spinDirection = -1;  // -1 = not spinning, 0 = left, 2 = right (element indices)

        if (leftSpin != 0) {
            spinDirection = 0;  // Already spinning left
        } else if (rightSpin != 0) {
            spinDirection = 2;  // Already spinning right
        } else {
            // Not spinning yet - check if player is starting spin
            int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
            if (currentAllowedDir < 0) {
                spinDirection = -1;  // No direction pressed
            } else {
                int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
                int16_t diff = controlledPlDir - currentAllowedDir;
                if (diff == 0) {
                    spinDirection = -1;  // Same direction
                } else {
                    diff &= 7;
                    if (diff == 4) {
                        spinDirection = -1;  // Opposite direction
                    } else if (diff < 4) {
                        // Spin left
                        writeMemory(A6 + TeamOff::kLeftSpin, 2, 1);
                        spinDirection = 0;
                    } else {
                        // Spin right
                        writeMemory(A6 + TeamOff::kRightSpin, 2, 1);
                        spinDirection = 2;
                    }
                }
            }
        }

        // Apply spin if spinning
        if (spinDirection >= 0) {
            // Get controlled player direction for table lookup
            int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
            int16_t tableIndex = (controlledPlDir << 2) + spinDirection;

            // Get spin multiplier based on timer
            int16_t multiplier = kSpinMultiplierFactor[spinTimer];

            // Apply X spin
            int16_t spinX = kKickSpinFactor[tableIndex];
            int16_t spinXFinal = (spinX * multiplier);
            int16_t destX = (int16_t)readMemory(A1 + SpriteOff::kDestX, 2);
            writeMemory(A1 + SpriteOff::kDestX, 2, destX + spinXFinal);

            // Apply Y spin
            int16_t spinY = kKickSpinFactor[tableIndex + 1];
            int16_t spinYFinal = (spinY * multiplier);
            int16_t destY = (int16_t)readMemory(A1 + SpriteOff::kDestY, 2);
            writeMemory(A1 + SpriteOff::kDestY, 2, destY + spinYFinal);

            int16_t ballSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            logDebug("[SPIN_KICK] spinDir=%s timer=%d mult=%d plDir=%d tableIdx=%d spinFactor=(%d,%d) spinApplied=(%d,%d) dest:(%d,%d)->(%d,%d) ballSpeed=%d",
                    spinDirection == 0 ? "LEFT" : "RIGHT", spinTimer, multiplier, controlledPlDir, tableIndex,
                    spinX, spinY, spinXFinal, spinYFinal,
                    destX, destY, destX + spinXFinal, destY + spinYFinal, ballSpeed);
        }

        // Check for high kick when spin timer == 4
        if (spinTimer == 4) {
            int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
            bool doNormalKick = false;
            bool doHighKick = false;

            if (currentAllowedDir < 0) {
                // ASM: js @@normal_not_high_kick - if no direction pressed, do normal kick
                doNormalKick = true;
            } else {
                int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
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
                int16_t origSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                writeMemory(A1 + SpriteOff::kDeltaZ, 4, BallPhys::kHighKickDeltaZ);
                writeMemory(A1 + SpriteOff::kSpeed, 2, BallPhys::kHighKickSpeed);

                // Adjust speed based on direction
                int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
                if (controlledPlDir != 0 && controlledPlDir != 4) {
                    if ((controlledPlDir & 1) != 0) {
                        // Diagonal direction - reduce to 87.5%
                        int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                        int16_t reduction = speed >> 2;
                        int16_t addition = speed >> 3;
                        speed = speed - reduction + addition;
                        writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                        logDebug("[KICK_HIGH] High kick (diagonal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                                controlledPlDir, origSpeed, BallPhys::kHighKickSpeed, speed, BallPhys::kHighKickDeltaZ);
                    }
                } else {
                    // Cardinal direction - reduce to 75%
                    int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                    speed = speed - (speed >> 2);
                    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                    logDebug("[KICK_HIGH] High kick (cardinal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                            controlledPlDir, origSpeed, BallPhys::kHighKickSpeed, speed, BallPhys::kHighKickDeltaZ);
                }
            } else if (doNormalKick) {
                // Normal kick
                int16_t origSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                writeMemory(A1 + SpriteOff::kDeltaZ, 4, BallPhys::kNormalKickDeltaZ);
                writeMemory(A1 + SpriteOff::kSpeed, 2, BallPhys::kNormalKickSpeed);

                // Adjust speed
                int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
                if (controlledPlDir != 0 && controlledPlDir != 4) {
                    if ((controlledPlDir & 1) != 0) {
                        int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                        int16_t reduction = speed >> 2;
                        int16_t addition = speed >> 3;
                        speed = speed - reduction + addition;
                        writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                        logDebug("[KICK_NORMAL] Normal kick (diagonal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                                controlledPlDir, origSpeed, BallPhys::kNormalKickSpeed, speed, BallPhys::kNormalKickDeltaZ);
                    }
                } else {
                    int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                    speed = speed - (speed >> 2);
                    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                    logDebug("[KICK_NORMAL] Normal kick (cardinal): plDir=%d origSpeed=%d -> baseSpeed=%d -> finalSpeed=%d deltaZ=0x%08X",
                            controlledPlDir, origSpeed, BallPhys::kNormalKickSpeed, speed, BallPhys::kNormalKickDeltaZ);
                }
            }
        }

        // Increment spin timer
        spinTimer = (int16_t)readMemory(A6 + TeamOff::kSpinTimer, 2);
        spinTimer++;
        writeMemory(A6 + TeamOff::kSpinTimer, 2, spinTimer);

        // Reset at 10
        if (spinTimer == 10) {
            writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
        }
        return;
    }

    // Pass in progress
    uint32_t opponentsTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
    int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + TeamOff::kGoalkeeperSavedCommentTimer, 2);

    if (goalkeeperTimer < 0) {
        writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
        return;
    }

    int16_t gameStatePlVal = MEM_WORD(GSAddr::kGameStatePl);
    if (gameStatePlVal != GSVal::kGameInProgress) {
        int16_t gameStateVal = MEM_WORD(GSAddr::kGameState);
        if (gameStateVal == GSVal::kKeeperHoldsBall) {
            writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
            return;
        }
    }

    int16_t spinTimer = (int16_t)readMemory(A6 + TeamOff::kSpinTimer, 2);
    if (spinTimer < 0) {
        return;
    }

    // If spin timer is 0, reset all spin/pass flags
    if (spinTimer == 0) {
        writeMemory(A6 + TeamOff::kLeftSpin, 2, 0);
        writeMemory(A6 + TeamOff::kRightSpin, 2, 0);
        writeMemory(A6 + TeamOff::kLongPass, 2, 0);
        writeMemory(A6 + TeamOff::kLongSpinPass, 2, 0);
    }

    A1 = BallAddr::kSpriteOffset;

    // Check spin direction
    int16_t leftSpin = (int16_t)readMemory(A6 + TeamOff::kLeftSpin, 2);
    int16_t rightSpin = (int16_t)readMemory(A6 + TeamOff::kRightSpin, 2);

    int16_t spinDirection = -1;  // -1 = not spinning, 0 = left, 2 = right (element indices)

    if (leftSpin != 0) {
        spinDirection = 0;
    } else if (rightSpin != 0) {
        spinDirection = 2;
    } else {
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
        if (currentAllowedDir >= 0) {
            int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
            int16_t diff = controlledPlDir - currentAllowedDir;
            if (diff != 0) {
                diff &= 7;
                if (diff != 4) {
                    if (diff < 4) {
                        writeMemory(A6 + TeamOff::kLeftSpin, 2, 1);
                        spinDirection = 0;
                    } else {
                        writeMemory(A6 + TeamOff::kRightSpin, 2, 1);
                        spinDirection = 2;
                    }
                }
            }
        }
    }

    // Apply pass spin if spinning
    if (spinDirection >= 0) {
        // Use ball direction for pass spin (not player direction)
        int16_t ballDirection = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        int16_t tableIndex = (ballDirection << 2) + spinDirection;

        int16_t multiplier = kSpinMultiplierFactor[spinTimer];

        // Apply X spin
        int16_t spinX = kPassingSpinFactor[tableIndex];
        int16_t spinXFinal = (spinX * multiplier);
        int16_t destX = (int16_t)readMemory(A1 + SpriteOff::kDestX, 2);
        writeMemory(A1 + SpriteOff::kDestX, 2, destX + spinXFinal);

        // Apply Y spin
        int16_t spinY = kPassingSpinFactor[tableIndex + 1];
        int16_t spinYFinal = (spinY * multiplier);
        int16_t destY = (int16_t)readMemory(A1 + SpriteOff::kDestY, 2);
        writeMemory(A1 + SpriteOff::kDestY, 2, destY + spinYFinal);

        int16_t ballSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
        logDebug("[SPIN_PASS] spinDir=%s timer=%d mult=%d ballDir=%d tableIdx=%d spinFactor=(%d,%d) spinApplied=(%d,%d) dest:(%d,%d)->(%d,%d) ballSpeed=%d",
                spinDirection == 0 ? "LEFT" : "RIGHT", spinTimer, multiplier, ballDirection, tableIndex,
                spinX, spinY, spinXFinal, spinYFinal,
                destX, destY, destX + spinXFinal, destY + spinYFinal, ballSpeed);
    }

    // Check for long pass mechanics
    int32_t longPassFlags = (int32_t)readMemory(A6 + TeamOff::kLongPass, 4);  // Reads both longPass and longSpinPass
    if (longPassFlags == 0) {
        // Check for starting long pass
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
        if (currentAllowedDir < 0) {
            // No direction pressed - ASM jumps to @@holding_left_or_right and applies long pass boost
            writeMemory(A6 + TeamOff::kLongPass, 2, 1);
            int16_t oldSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
            int16_t speed = oldSpeed + (oldSpeed >> 3);
            writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
            logDebug("[PASS_LONG] Long pass (no direction): speed %d -> %d (+12.5%%)", oldSpeed, speed);
        } else {
            int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
            int16_t diff = controlledPlDir - currentAllowedDir;
            if (diff != 0) {
                diff &= 7;
                // Check for left/right (2, 6)
                if (diff == 2 || diff == 6) {
                    // Long pass - increase speed by 12.5%
                    writeMemory(A6 + TeamOff::kLongPass, 2, 1);
                    int16_t oldSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                    int16_t speed = oldSpeed + (oldSpeed >> 3);
                    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                    logDebug("[PASS_LONG] Long pass: plDir=%d allowedDir=%d diff=%d speed %d -> %d (+12.5%%)",
                            controlledPlDir, currentAllowedDir, diff, oldSpeed, speed);
                } else if (diff >= 3 && diff <= 5) {
                    // Long spin pass - increase speed by 12.5%
                    writeMemory(A6 + TeamOff::kLongSpinPass, 2, 1);
                    int16_t oldSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
                    int16_t speed = oldSpeed + (oldSpeed >> 3);
                    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
                    logDebug("[PASS_LONGSPIN] Long spin pass: plDir=%d allowedDir=%d diff=%d speed %d -> %d (+12.5%%)",
                            controlledPlDir, currentAllowedDir, diff, oldSpeed, speed);
                }
            }
        }
    }

    // Increment spin timer
    spinTimer = (int16_t)readMemory(A6 + TeamOff::kSpinTimer, 2);
    spinTimer++;
    writeMemory(A6 + TeamOff::kSpinTimer, 2, spinTimer);

    if (spinTimer == 10) {
        writeMemory(A6 + TeamOff::kSpinTimer, 2, (uint16_t)-1);
    }
}

// PlayerHittingStaticHeader - player performs standing header on the ball
// Assembly at 112247-112374
// Input: A1 -> player sprite, A6 -> team data
void PlayerHittingStaticHeader() {
    // Clear pass in progress flag
    writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);

    // Check if already at static header hit animation
    uint32_t animTable = readMemory(A1 + 18, 4);  // Sprite.animationTable
    if (animTable != Head::AnimationTables::kStaticHit) {
        // Try to turn player toward current allowed direction
        int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
        int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        int16_t diff = currentAllowedDir - playerDir;

        if (diff != 0) {
            diff &= 7;
            if (diff != 4) {  // Not opposite direction
                if (diff >= 4) {
                    // Turn player left (counter-clockwise)
                    int16_t savedSpeed = (int16_t)readMemory(A6 + SpriteOff::kSpeed, 2);  // Note: this reads from A6, odd but matches assembly
                    playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
                    playerDir -= 1;
                    playerDir &= 7;
                    writeMemory(A1 + SpriteOff::kDirection, 2, playerDir);

                    // Turn twice if not at target yet
                    if (savedSpeed != playerDir) {
                        playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
                        playerDir -= 1;
                        playerDir &= 7;
                        writeMemory(A1 + SpriteOff::kDirection, 2, playerDir);
                    }
                } else {
                    // Turn player right (clockwise)
                    int16_t savedSpeed = (int16_t)readMemory(A6 + SpriteOff::kSpeed, 2);
                    playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
                    playerDir += 1;
                    playerDir &= 7;
                    writeMemory(A1 + SpriteOff::kDirection, 2, playerDir);

                    // Turn twice if not at target yet
                    if (savedSpeed != playerDir) {
                        playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
                        playerDir += 1;
                        playerDir &= 7;
                        writeMemory(A1 + SpriteOff::kDirection, 2, playerDir);
                    }
                }
            }
        }
    }

    // Set static header hit animation
    A0 = Head::AnimationTables::kStaticHit;
    SetAnimationTableAndPictureIndex();

    // Get direction for ball trajectory
    int16_t direction = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
    if (direction < 0) {
        direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    }

    // Store controlled player direction
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, direction);

    // Set ball destination using direction table
    A2 = BallAddr::kSpriteOffset;
    uint16_t tableIndex = direction << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);

    // Get destination offsets from table (Tbl::kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + SpriteOff::kDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + SpriteOff::kDestY, 2, ballY + destOffsetY);

    // Set base ball speed
    writeMemory(A2 + SpriteOff::kSpeed, 2, Head::Speeds::kStaticHeaderBall);

    // Get player's heading skill to adjust speed
    GetPlayerPointerFromShirtNumber();
    int8_t headingSkill = (int8_t)readMemory(A4 + PlayerOff::kHeading, 1);
    int16_t speedBonus = kPlayerHeaderSpeedIncrease[headingSkill];
    int16_t currentSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    writeMemory(A2 + SpriteOff::kSpeed, 2, currentSpeed + speedBonus);

    // Reverse and halve ball's deltaZ
    int32_t deltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    deltaZ = (-deltaZ) >> 1;
    writeMemory(A2 + SpriteOff::kDeltaZ, 4, deltaZ);

    // Set player heading flag
    writeMemory(A1 + SpriteOff::kHeading, 2, 1);

    // Play kick sample and reset spin timers
    SWOS::PlayKickSample();
    ResetBothTeamSpinTimers();
}

// Helper: Set jump header hit animation table if frameSwitchCounter <= 2
static void SetPlayerJumpHeaderHitAnimationTable() {
    int16_t frameSwitchCounter = (int16_t)readMemory(A1 + SpriteOff::kFrameSwitchCounter, 2);
    if (frameSwitchCounter > 2) {
        return;
    }
    A0 = Head::AnimationTables::kJumpHit;
    SetAnimationTableAndPictureIndex();
}

// Helper: DoFlyingHeader - low header trajectory
static void DoFlyingHeader() {
    int16_t oldSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    int32_t oldDeltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);

    // Set ball deltaZ to low jump height
    writeMemory(A2 + SpriteOff::kDeltaZ, 4, Head::Heights::kLowJumpHeight);

    // Reduce ball speed to 75%
    int16_t speed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    int16_t reduction = speed >> 2;
    writeMemory(A2 + SpriteOff::kSpeed, 2, speed - reduction);

    logDebug("[HEADER_FLYING] DoFlyingHeader: oldDeltaZ=0x%08X -> newDeltaZ=0x%08X (Head::Heights::kLowJumpHeight), oldSpeed=%d -> newSpeed=%d",
            oldDeltaZ, Head::Heights::kLowJumpHeight, oldSpeed, speed - reduction);

    SetPlayerJumpHeaderHitAnimationTable();
}

// Helper: DoLobHeader - high header trajectory
static void DoLobHeader() {
    int16_t oldSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    int32_t oldDeltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);

    // Set ball deltaZ to high jump height
    writeMemory(A2 + SpriteOff::kDeltaZ, 4, Head::Heights::kHighJumpHeight);

    // Reduce ball speed to 93.75%
    int16_t speed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    int16_t reduction = speed >> 4;
    writeMemory(A2 + SpriteOff::kSpeed, 2, speed - reduction);

    logDebug("[HEADER_LOB] DoLobHeader: oldDeltaZ=0x%08X -> newDeltaZ=0x%08X (Head::Heights::kHighJumpHeight), oldSpeed=%d -> newSpeed=%d",
            oldDeltaZ, Head::Heights::kHighJumpHeight, oldSpeed, speed - reduction);

    SetPlayerJumpHeaderHitAnimationTable();
}

// PlayerHittingJumpHeader - player performs jumping header on the ball
// Assembly at 108981-109144
// Input: A1 -> player sprite, A6 -> team data
void PlayerHittingJumpHeader() {
    // Clear pass in progress flag
    writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);

    // Get direction - use currentAllowedDirection if valid, else use player's direction
    int16_t direction = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
    int16_t origAllowedDir = direction;
    if (direction < 0) {
        direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    }

    // Setup ball sprite reference
    A2 = BallAddr::kSpriteOffset;

    // Get initial ball state for logging
    int32_t origBallDeltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    int16_t origBallSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    int16_t ballZ = (int16_t)readMemory(A2 + SpriteOff::kZ + 2, 2);

    // Set initial ball deltaZ
    writeMemory(A2 + SpriteOff::kDeltaZ, 4, Head::Heights::kBallJumpHeaderDeltaZ);

    // Set ball speed to 125% of player speed
    int16_t playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    int16_t ballSpeed = playerSpeed + (playerSpeed >> 2);
    writeMemory(A2 + SpriteOff::kSpeed, 2, ballSpeed);

    // Calculate direction difference: player direction - controls direction
    int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    int16_t diff = playerDir - direction;
    int16_t origDiff = diff;

    // Get current allowed direction again
    int16_t currentAllowedDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);

    int16_t finalDirection;
    const char* headerType = "UNKNOWN";
    if (currentAllowedDir < 0) {
        // No direction pressed - do flying header with player's direction
        headerType = "FLYING (no dir)";
        DoFlyingHeader();
        finalDirection = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);  // Note: reads from wrong offset in asm but matches behavior
    } else {
        diff &= 7;
        if (diff == 0) {
            // Same direction - use allowed direction
            headerType = "SAME_DIR";
            finalDirection = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);  // Note: matches assembly (reads Sprite offset)
        } else if (diff == 4) {
            // Opposite direction - lob header
            headerType = "LOB (opposite)";
            DoLobHeader();
            finalDirection = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        } else if (diff == 1) {
            // Aim left
            headerType = "FLYING aim left (diff=1)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir - 1;
        } else if (diff == 7) {
            // Aim right
            headerType = "FLYING aim right (diff=7)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir + 1;
        } else if (diff == 2) {
            // Left held - flying header aim left
            headerType = "FLYING aim left (diff=2)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir - 1;
        } else if (diff == 6) {
            // Right held - flying header aim right
            headerType = "FLYING aim right (diff=6)";
            DoFlyingHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir + 1;
        } else if (diff == 3) {
            // Down-left held - lob header aim left
            headerType = "LOB aim left (diff=3)";
            DoLobHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir - 1;
        } else {
            // diff == 5, down-right - lob header aim right
            headerType = "LOB aim right (diff=5)";
            DoLobHeader();
            playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
            finalDirection = playerDir + 1;
        }
    }

    // Normalize direction to 0-7
    finalDirection &= 7;

    // Store controlled player direction
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, finalDirection);

    // Set ball destination using direction table
    uint16_t tableIndex = finalDirection << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);

    // Get destination offsets from table (Tbl::kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + SpriteOff::kDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + SpriteOff::kDestY, 2, ballY + destOffsetY);

    // Get player's heading skill to adjust speed
    GetPlayerPointerFromShirtNumber();
    int8_t headingSkill = (int8_t)readMemory(A4 + PlayerOff::kHeading, 1);
    int16_t speedBonus = kPlayerHeaderSpeedIncrease[headingSkill];
    int16_t currentSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
    writeMemory(A2 + SpriteOff::kSpeed, 2, currentSpeed + speedBonus);

    // Halve player speed
    playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    writeMemory(A1 + SpriteOff::kSpeed, 2, playerSpeed >> 1);

    // Set player heading flag
    writeMemory(A1 + SpriteOff::kHeading, 2, 1);

    // Get final ball state for logging
    int32_t finalBallDeltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    int16_t finalBallSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);

    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);

    logDebug("[HEADER_JUMP] Player %d at (%d,%d): type=%s playerDir=%d allowedDir=%d diff=%d finalDir=%d",
            playerOrdinal, playerX, playerY, headerType, playerDir, origAllowedDir, origDiff, finalDirection);
    logDebug("[HEADER_JUMP]   ballPos=(%d,%d,z=%d) destOffset=(%d,%d) ballDest=(%d,%d)",
            ballX, ballY, ballZ, destOffsetX, destOffsetY, ballX + destOffsetX, ballY + destOffsetY);
    logDebug("[HEADER_JUMP]   deltaZ: 0x%08X -> 0x%08X, speed: %d -> %d (skill=%d bonus=%d)",
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
    writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);

    // Get direction - use currentAllowedDirection if valid, else use player's direction
    int16_t controlDir = (int16_t)readMemory(A6 + TeamOff::kCurrentAllowedDirection, 2);
    if (controlDir < 0) {
        controlDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    }

    A2 = BallAddr::kSpriteOffset;

    // Calculate direction difference: player direction - controls direction
    int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
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
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, newDirection);

    // Set ball destination using direction table
    uint16_t tableIndex = newDirection << 2;

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);

    // Get destination offsets from table (Tbl::kDefaultDestinations is absolute address)
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + SpriteOff::kDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + SpriteOff::kDestY, 2, ballY + destOffsetY);

    // Reduce player speed to 50% (padded to odd)
    int16_t playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    int16_t halfSpeed = playerSpeed >> 1;
    playerSpeed = playerSpeed - halfSpeed;
    playerSpeed |= 1;  // Pad to odd
    writeMemory(A1 + SpriteOff::kSpeed, 2, playerSpeed);

    // Ball speed = 75% of player speed (player is now at 50%)
    // Actually: ball speed = playerSpeed + (playerSpeed / 2) = 1.5 * 50% = 75%
    int16_t ballSpeed = playerSpeed + (playerSpeed >> 1);
    writeMemory(A2 + SpriteOff::kSpeed, 2, ballSpeed);

    // Set tackle state
    writeMemory(A1 + SpriteOff::kTackleState, 2, TS::kTacklingTheBall);

    // Check if opponent is far enough for good tackle
    uint32_t opponentsTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
    uint32_t opponentPlayer = readMemory(opponentsTeam + TeamOff::kControlledPlayerSprite, 4);

    if (opponentPlayer != 0) {
        // Check ball distance
        uint32_t ballDist = readMemory(opponentPlayer + SpriteOff::kBallDistance, 4);
        if (ballDist >= 9) {
            // Calculate distance squared between players
            int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
            int16_t opponentX = (int16_t)readMemory(opponentPlayer + SpriteOff::kX + 2, 2);
            int16_t dx = playerX - opponentX;
            // Assembly bug at 109508-109510: uses movzx instead of movsx for second operand
            // movsx eax, dx ; sign-extend: -2 -> 0xFFFFFFFE
            // movzx ebx, dx ; zero-extend: 0xFFFE -> 0x0000FFFE = 65534
            // imul ebx      ; signed multiply: (-2) * (65534) = -131068
            // This causes dxSquared to be wrong for negative dx values
            int32_t dxSquared = (int32_t)dx * (int32_t)(uint16_t)dx;

            int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
            int16_t opponentY = (int16_t)readMemory(opponentPlayer + SpriteOff::kY + 2, 2);
            int16_t dy = playerY - opponentY;
            // Y uses correct 16-bit signed multiply (imul bx)
            int32_t dySquared = (int32_t)dy * dy;

            int32_t distSquared = dxSquared + dySquared;

            // Comparison is unsigned (jbe)
            if ((uint32_t)distSquared > 32) {
                writeMemory(A1 + SpriteOff::kTackleState, 2, TS::kGoodTackle);
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
    MEM_WORD(GSAddr::kStateGoal) = 0;

    // Get controlled player sprite
    uint32_t controlledPlayer = readMemory(A6 + TeamOff::kControlledPlayerSprite, 4);
    A0 = controlledPlayer;

    // Get and store direction
    int16_t direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, direction);

    // Get ball destination coordinates table (sets A0 to appropriate table)
    D0 = direction;
    GetBallDestCoordinatesTable();

    A2 = BallAddr::kSpriteOffset;

    // Calculate table index
    uint16_t tableIndex = (direction << 2);

    // Get ball position
    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);

    // Get destination offsets from table (A0 was set by GetBallDestCoordinatesTable)
    int16_t destOffsetX = (int16_t)readMemory(A0 + tableIndex, 2);
    int16_t destOffsetY = (int16_t)readMemory(A0 + tableIndex + 2, 2);

    // Set ball destination
    writeMemory(A2 + SpriteOff::kDestX, 2, ballX + destOffsetX);
    writeMemory(A2 + SpriteOff::kDestY, 2, ballY + destOffsetY);

    // Set ball speed and deltaZ
    writeMemory(A2 + SpriteOff::kSpeed, 2, BallPhys::kKickingSpeed);
    writeMemory(A2 + SpriteOff::kDeltaZ, 4, BallPhys::kKickingDeltaZ);

    ResetBothTeamSpinTimers();

    // Check if game in progress
    int16_t gameStatePlVal = MEM_WORD(GSAddr::kGameStatePl);
    int16_t gameStateVal = MEM_WORD(GSAddr::kGameState);

    if (gameStatePlVal != GSVal::kGameInProgress) {
        // Check for throw-in
        if (gameStateVal >= GSVal::kThrowInForwardRight && gameStateVal <= GSVal::kThrowInBackLeft) {
            // Throw-in in progress - just exit without kick sample
            return;
        }
    }

    // Check if this is a shot on goal
    int16_t controlledPlDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
    bool possibleShot = false;

    if (A6 != TeamAddr::kTopTeamData) {
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
                int8_t finishingSkill = (int8_t)readMemory(A4 + PlayerOff::kFinishing, 1);
                int16_t speedBonus = kBallSpeedFinishing[finishingSkill];
                int16_t currentSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
                writeMemory(A2 + SpriteOff::kSpeed, 2, currentSpeed + speedBonus);
            } else {
                // Long shot - use shooting skill
                int8_t shootingSkill = (int8_t)readMemory(A4 + PlayerOff::kShooting, 1);
                int16_t speedBonus = kBallSpeedKicking[shootingSkill];
                int16_t currentSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
                writeMemory(A2 + SpriteOff::kSpeed, 2, currentSpeed + speedBonus);
            }
        } else {
            // Long shot
            GetPlayerPointerFromShirtNumber();
            int8_t shootingSkill = (int8_t)readMemory(A4 + PlayerOff::kShooting, 1);
            int16_t speedBonus = kBallSpeedKicking[shootingSkill];
            int16_t currentSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
            writeMemory(A2 + SpriteOff::kSpeed, 2, currentSpeed + speedBonus);
        }
    }

    // Check if player is goalkeeper (ordinal 1) kicking left or right
    int16_t playerOrdinal = (int16_t)readMemory(A1 + 2, 2);  // Sprite.playerOrdinal
    if (playerOrdinal == 1) {
        int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
        if (playerDir == 2 || playerDir == 6) {
            // Goalkeeper kicking left/right - skip spin timer setup
            writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);
            SWOS::PlayKickSample();
            return;
        }
    }

    // Check opponent's goalkeeper saved timer
    uint32_t opponentsTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
    int16_t goalkeeperTimer = (int16_t)readMemory(opponentsTeam + TeamOff::kGoalkeeperSavedCommentTimer, 2);
    if (goalkeeperTimer >= 0) {
        // Set up spin timer
        writeMemory(A6 + TeamOff::kSpinTimer, 2, 0);
    }

    writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);
    SWOS::PlayKickSample();
}


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
    int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);
    int16_t goalkeeperY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t yDiff = ballY - goalkeeperY;

    int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t goalkeeperX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t ballSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);

    logDebug("[GK_DIVE] Called: ballPos=(%d,%d) gkPos=(%d,%d) yDiff=%d ballSpeed=%d team=%s",
            ballX, ballY, goalkeeperX, goalkeeperY, yDiff, ballSpeed,
            A6 == TeamAddr::kTopTeamData ? "top" : "bottom");

    // Flip sign for bottom team
    if (A6 != TeamAddr::kTopTeamData) {
        yDiff = -yDiff;
    }

    // Check if ball is behind goalkeeper
    if (yDiff < 0) {
        if (yDiff < -10) {
            // Ball too far behind - won't dive
            logDebug("[GK_DIVE] REJECT: Ball too far behind (yDiff=%d < -10)", yDiff);
            D0 = 0;
            return;
        }
        // Ball slightly behind - try saving anyway
        logDebug("[GK_DIVE] ACCEPT: Ball slightly behind, attempting save (yDiff=%d)", yDiff);
        D0 = 1;
        return;
    }

    // Ball is in front of goalkeeper
    int16_t playingPenalties = MEM_WORD(GSAddr::kPlayingPenalties);
    int16_t penalty = MEM_WORD(GSAddr::kPenalty);

    if (playingPenalties != 0 || penalty != 0) {
        // Penalty shot - use random distance threshold
        int16_t threshold = GK::SaveDistances::kPenaltySaveFar;  // 25% chance (default)

        // Save D0 temporarily
        int16_t savedYDiff = yDiff;

        // Call Rand
        SWOS::Rand();
        int16_t randVal = D0 & 0xFFFF;

        // Restore yDiff
        yDiff = savedYDiff;

        if ((randVal & 0x18) != 0) {
            threshold = GK::SaveDistances::kPenaltySaveNear;  // 75% chance
        }

        if (yDiff > threshold) {
            // Ball too far - won't dive
            logDebug("[GK_DIVE] REJECT penalty: yDiff=%d > threshold=%d", yDiff, threshold);
            D0 = 0;
            return;
        }

        // At penalty goalkeeper always dives
        logDebug("[GK_DIVE] ACCEPT penalty: yDiff=%d <= threshold=%d", yDiff, threshold);
        D0 = 1;
        return;
    }

    // Normal shot
    if (yDiff > GK::SaveDistances::kNormalSave) {
        // Ball too far - won't dive
        logDebug("[GK_DIVE] REJECT: Ball too far (yDiff=%d > GK::SaveDistances::kNormalSave=%d)", yDiff, GK::SaveDistances::kNormalSave);
        D0 = 0;
        return;
    }

    // Calculate abs(ball y - goalkeeper y) for frame calculation
    int16_t absYDiff = ballY - goalkeeperY;
    if (absYDiff < 0) {
        absYDiff = -absYDiff;
    }

    // Get ball delta Y
    int32_t ballDeltaY = (int32_t)readMemory(A2 + SpriteOff::kDeltaY, 4);

    // Calculate frames until ball reaches goalkeeper Y
    int16_t framesY = GetFramesNeededToCoverDistance(ballDeltaY, absYDiff);
    if (framesY == 0) {
        // Ball not moving in Y - won't dive
        logDebug("[GK_DIVE] REJECT: Ball not moving in Y (ballDeltaY=0x%08X, absYDiff=%d, framesY=0)", ballDeltaY, absYDiff);
        D0 = 0;
        return;
    }

    // Calculate abs(ball x - goalkeeper x) using ballDefensiveX
    int16_t ballDefX = MEM_WORD(BallPos::kDefensiveX);
    int16_t absXDiff = ballDefX - goalkeeperX;
    if (absXDiff < 0) {
        absXDiff = -absXDiff;
    }

    // Get goalkeeper delta X (note: we use A1 which is the goalkeeper)
    int32_t goalkeeperDeltaX = (int32_t)readMemory(A1 + SpriteOff::kDeltaX, 4);

    // Calculate frames until goalkeeper reaches ball X
    int16_t framesX = GetFramesNeededToCoverDistance(goalkeeperDeltaX, absXDiff);
    if (framesX == 0) {
        // Already at ball X - won't dive
        logDebug("[GK_DIVE] REJECT: Already at ball X (gkDeltaX=0x%08X, absXDiff=%d, framesX=0)", goalkeeperDeltaX, absXDiff);
        D0 = 0;
        return;
    }

    // If goalkeeper can reach ball X before ball reaches goalkeeper Y, no need to dive
    if (framesX <= framesY) {
        logDebug("[GK_DIVE] REJECT: GK can reach ball without diving (framesX=%d <= framesY=%d)", framesX, framesY);
        D0 = 0;
        return;
    }

    logDebug("[GK_DIVE] Checking dive timing: framesX=%d > framesY=%d, absXDiff=%d, ballDefX=%d, gkX=%d",
            framesX, framesY, absXDiff, ballDefX, goalkeeperX);

    // At this point goalkeeper can't reach ball by X but it's in range by Y
    // Recalculate absXDiff
    absXDiff = ballDefX - goalkeeperX;
    if (absXDiff < 0) {
        absXDiff = -absXDiff;
    }

    // Look up dive speed from shot chance table based on current game tick
    uint32_t shotChanceTable = readMemory(A6 + TeamOff::kShotChanceTable, 4);
    int16_t tickIndex = MEM_WORD(GSAddr::kCurrentGameTick) & 0x0F;  // 0..15
    tickIndex <<= 1;  // multiply by 2 for word offset

    int16_t skillIndex = (int16_t)readMemory(shotChanceTable + tickIndex + 10, 2);
    skillIndex--;
    if (skillIndex < 0) {
        skillIndex = 0;
    }

    // Look up dive delta from table
    int32_t diveOffset = skillIndex << 2;  // multiply by 4
    int32_t diveDelta = (int32_t)readMemory(GK::AnimationTables::kDiveDeltas + diveOffset, 4);

    // Increment dead var
    MEM_WORD(GK::Addresses::kDiveDeadVar) += 1;

    // Calculate frames to dive to ball position
    int16_t framesDive = GetFramesNeededToCoverDistance(diveDelta, absXDiff);
    logDebug("[GK_DIVE] Dive calc: skillIndex=%d, diveDelta=0x%08X, absXDiff=%d, framesDive=%d, framesY=%d",
            skillIndex, diveDelta, absXDiff, framesDive, framesY);

    if (framesDive == 0) {
        // Can't dive - won't dive
        logDebug("[GK_DIVE] REJECT: framesDive=0 (diveDelta=0x%08X, absXDiff=%d)", diveDelta, absXDiff);
        D0 = 0;
        return;
    }

    // If dive frames >= frames until ball reaches goalkeeper Y, we can save
    if (framesDive >= framesY) {
        logDebug("[GK_DIVE] ACCEPT: framesDive=%d >= framesY=%d - WILL DIVE!", framesDive, framesY);
        D0 = 1;
        return;
    }

    // Can't reach in time - won't dive
    logDebug("[GK_DIVE] REJECT: Can't reach in time (framesDive=%d < framesY=%d)", framesDive, framesY);
    D0 = 0;
}


// PlayerTacklingTestFoul - tests if a tackle was a foul and handles cards
// Assembly at 106916-107198
// Input: A1 -> tackling player sprite, A6 -> tackling player's team
void PlayerTacklingTestFoul() {
    // Get opponent's team
    uint32_t opponentsTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);

    // Get opponent's controlled player sprite
    uint32_t opponentControlledSprite = readMemory(opponentsTeam + TeamOff::kControlledPlayerSprite, 4);

    if (opponentControlledSprite == 0) {
        return;  // No opponent controlled player
    }

    // Save A2 for opponents controlled player
    uint32_t savedA2 = A2;
    A2 = opponentControlledSprite;

    // Calculate squared distance between players
    int16_t thisX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t oppX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int32_t dx = thisX - oppX;
    int32_t dxSq = dx * dx;

    int16_t thisY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t oppY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);
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
        int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
        speed >>= 2;  // Divide by 4
        speed |= 1;   // Ensure odd (at least 1)
        writeMemory(A1 + SpriteOff::kSpeed, 2, speed);
        A2 = savedA2;
        return;
    }

    // Check if opponent's player is within field bounds
    int16_t oppPlayerX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t oppPlayerY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);

    if (oppPlayerX < 81 || oppPlayerY < 129 || oppPlayerX > 590 || oppPlayerY > 769) {
        A2 = savedA2;
        return;  // Outside field
    }

    // Opponent's player is inside the field - slow down tackling player
    // Assembly does: shr speed,1; shr speed,1; or speed,1
    int16_t speed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    speed >>= 2;  // Divide by 4
    speed |= 1;   // Ensure odd (at least 1)
    writeMemory(A1 + SpriteOff::kSpeed, 2, speed);

    // Call PlayerTackled for the opponent
    uint32_t savedA1 = A1;
    uint32_t savedA6 = A6;
    A1 = A2;
    A6 = opponentsTeam;
    PlayerTackled();
    A6 = savedA6;
    A1 = savedA1;

    // Check if opponent player was close to ball
    uint32_t ballDist = readMemory(A2 + SpriteOff::kBallDistance, 4);
    if (ballDist > 800) {
        A2 = savedA2;
        return;  // Too far from ball - no foul
    }

    // Check tackle state
    int16_t tackleState = (int16_t)readMemory(A1 + SpriteOff::kTackleState, 2);
    if (tackleState == 0) {
        // No tackle state - it's a foul
        goto foul_conceded;
    }

    if (tackleState == TS::kGoodTackle) {
        // Good tackle - just play dangerous play comment
        SWOS::PlayDangerousPlayComment();
        A2 = savedA2;
        return;
    }

    {  // Scope for direction comparison variables
    // Check if players facing similar direction (tackle from behind)
    int16_t tacklerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    int16_t tackledDir = (int16_t)readMemory(A2 + SpriteOff::kDirection, 2);
    int16_t dirDiff = tacklerDir - tackledDir;

    if (dirDiff < -1 || dirDiff > 1) {
        A2 = savedA2;
        return;  // Not facing same direction - not a foul from behind
    }
    }  // End scope for direction comparison variables

foul_conceded:
    // Record foul
    uint32_t teamStats = readMemory(A6 + TeamOff::kTeamStatsPtr, 4);
    int16_t fouls = (int16_t)readMemory(teamStats + SwosMemory::TeamStats::Offsets::kFoulsConceded, 2);
    writeMemory(teamStats + SwosMemory::TeamStats::Offsets::kFoulsConceded, 2, fouls + 1);

    // Check if cards are allowed
    int16_t cardsDisallowed = MEM_WORD(GSAddr::kCardsDisallowed);
    if (cardsDisallowed != 0) {
        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
        return;
    }

    int16_t trainingGame = MEM_WORD(GSAddr::kTrainingGame);
    if (trainingGame != 0) {
        TestFoulForPenaltyAndFreeKick();
        A2 = savedA2;
        return;
    }

    // Check if in penalty area
    int16_t foulX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
    int16_t foulY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);
    bool inPenaltyArea = false;

    if (foulX >= 193 && foulX <= 478 && foulY >= 129 && foulY <= 769) {
        if (A6 == TeamAddr::kTopTeamData) {
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
        int16_t goalY = (A6 == TeamAddr::kTopTeamData) ? 129 : 769;

        // Calculate distance from tackled player to goal
        int32_t oppDistX = foulX - goalX;
        int32_t oppDistY = foulY - goalY;
        int32_t oppDistSq = oppDistX * oppDistX + oppDistY * oppDistY;

        // Store tackled player as default closest
        uint32_t closestPlayer = A2;

        // Check all teammates of the tackling player
        uint32_t spritesTable = readMemory(A6 + TeamOff::kSpritesTable, 4);

        for (int i = 0; i < 11; i++) {
            uint32_t playerSprite = readMemory(spritesTable + i * 4, 4);

            // Skip sent away players
            int16_t sentAway = (int16_t)readMemory(playerSprite + SpriteOff::kSentAway, 2);
            if (sentAway != 0) continue;

            // Skip goalkeeper
            int16_t ordinal = (int16_t)readMemory(playerSprite + 2, 2);
            if (ordinal == 1) continue;

            // Skip the tackling player
            if (playerSprite == A1) continue;

            // Calculate distance to goal
            int16_t plX = (int16_t)readMemory(playerSprite + SpriteOff::kX + 2, 2);
            int16_t plY = (int16_t)readMemory(playerSprite + SpriteOff::kY + 2, 2);
            int32_t plDistX = plX - goalX;
            int32_t plDistY = plY - goalY;
            int32_t plDistSq = plDistX * plDistX + plDistY * plDistY;

            if (plDistSq <= oppDistSq) {
                oppDistSq = plDistSq;
                closestPlayer = playerSprite;
            }
        }

        // Store closest player
        writeMemory(Dead::kDseg114EC2, 4, closestPlayer);

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
        int16_t gameTick = MEM_WORD(GSAddr::kCurrentGameTick);
        int16_t cardIndex = (gameTick & 0x1E) >> 1;  // 0..15
        int16_t cardChance = MEM_WORD(PlayerDisc::kCardChance);

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
        int16_t gameTick = MEM_WORD(GSAddr::kCurrentGameTick);
        int16_t cardIndex = (gameTick & 0x1E) >> 1;
        int16_t cardChance = MEM_WORD(PlayerDisc::kCardChance);

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


// DoPass - performs a pass to a teammate
// Assembly at 108290-108573
// Input: A1 -> player sprite, A6 -> team data
void DoPass() {
    // Reset pass variables
    MEM_WORD(Pass::Tables::kGoodPassSampleCommand) = 0;
    MEM_WORD(GSAddr::kStateGoal) = 0;

    // Get controlled player sprite
    uint32_t controlledPlayer = readMemory(A6 + TeamOff::kControlledPlayerSprite, 4);
    A0 = controlledPlayer;

    // Get player direction
    int16_t direction = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    D0 = direction;
    int16_t passDirection = direction;

    // Store direction
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, direction);

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

        A1 = BallAddr::kSpriteOffset;

        // Calculate destination based on direction
        int16_t tableIndex = passDirection << 2;
        int16_t ballX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
        int16_t destOffsetX = (int16_t)readMemory(A0 + tableIndex, 2);
        writeMemory(A1 + SpriteOff::kDestX, 2, ballX + destOffsetX);

        int16_t ballY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
        int16_t destOffsetY = (int16_t)readMemory(A0 + tableIndex + 2, 2);
        writeMemory(A1 + SpriteOff::kDestY, 2, ballY + destOffsetY);

        // Set ball speed for free pass
        writeMemory(A1 + SpriteOff::kSpeed, 2, Pass::Speeds::kFreePassReleasingBall);

        goto reset_spin_timers;
    }

    {  // Scope for variables that gotos may cross
    // Found a player to pass to
    uint32_t passToPlayer = A0;

    // Store pass target
    writeMemory(A6 + TeamOff::kPassToPlayerPtr, 4, passToPlayer);
    writeMemory(A6 + TeamOff::kPassingBall, 2, 1);
    writeMemory(A6 + TeamOff::kPassingToPlayer, 2, 1);

    // Check game state
    int16_t gameStatePlVal = MEM_WORD(GSAddr::kGameStatePl);
    if (gameStatePlVal != GSVal::kGameInProgress) {
        goto calculate_pass_destination;
    }

    {  // Scope for playerNumber and subsequent variables
    // Check if CPU is passing (playerNumber == 0)
    int16_t playerNumber = (int16_t)readMemory(A6 + TeamOff::kPlayerNumber, 2);
    if (playerNumber != 0) {
        goto calculate_pass_destination;
    }

    {  // Scope for CPU pass failure calculation
    // CPU passing - check for pass failure based on skill
    int8_t passingSkill = (int8_t)readMemory(A4 + PlayerOff::kPassing, 1);
    int16_t skillIndex = ((int16_t)passingSkill) << 1;

    // Get random value based on game tick
    int16_t gameTick = MEM_WORD(GSAddr::kCurrentGameTick);
    int16_t randVal = (gameTick & 0x1E) >> 1;  // 0..15

    // Check pass failure chance table
    int16_t failChance = (int16_t)readMemory(Pass::Tables::kAIFailedPassChance + skillIndex, 2);
    if (randVal >= failChance) {
        goto calculate_pass_destination;
    }

    // CPU botches the pass - mess up the angle
    MEM_WORD(Pass::Tables::kGoodPassSampleCommand) = (int16_t)-2;

    A1 = BallAddr::kSpriteOffset;

    {  // Scope for messed up destination calculation
    // Calculate messed up destination
    int16_t targetX = (int16_t)readMemory(passToPlayer + SpriteOff::kX + 2, 2);
    int16_t currBallX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t deltaX = targetX - currBallX;

    // Either halve X or Y delta based on game tick bit
    if ((gameTick & 0x20) == 0) {
        deltaX >>= 1;
    }
    deltaX <<= 5;  // Multiply by 32 (or 16 if halved)
    writeMemory(A1 + SpriteOff::kDestX, 2, currBallX + deltaX);

    int16_t targetY = (int16_t)readMemory(passToPlayer + SpriteOff::kY + 2, 2);
    int16_t currBallY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t deltaY = targetY - currBallY;

    // Opposite condition for Y
    if ((gameTick & 0x20) != 0) {
        deltaY >>= 1;
    }
    deltaY <<= 5;
    writeMemory(A1 + SpriteOff::kDestY, 2, currBallY + deltaY);

    goto determine_ball_speed;
    }  // End scope for messed up destination calculation
    }  // End scope for CPU pass failure calculation
    }  // End scope for playerNumber and subsequent variables

calculate_pass_destination:
    {
        A1 = BallAddr::kSpriteOffset;

        // Get delta to pass target
        int16_t passTargetX = (int16_t)readMemory(passToPlayer + SpriteOff::kX + 2, 2);
        int16_t ballPosX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
        int16_t dx = passTargetX - ballPosX;

        int16_t passTargetY = (int16_t)readMemory(passToPlayer + SpriteOff::kY + 2, 2);
        int16_t ballPosY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
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
        writeMemory(A1 + SpriteOff::kDestX, 2, ballPosX + dx);
        writeMemory(A1 + SpriteOff::kDestY, 2, ballPosY + dy);
    }

determine_ball_speed:
    {
        // Determine ball speed based on distance
        uint32_t ballDistance = readMemory(passToPlayer + SpriteOff::kBallDistance, 4);
        int16_t baseSpeed;

        if (ballDistance < 2500) {
            baseSpeed = Pass::Speeds::kCloserThan2500;
        } else if (ballDistance < 10000) {
            baseSpeed = Pass::Speeds::k2500To10000;
        } else if (ballDistance < 22500) {
            baseSpeed = Pass::Speeds::k10000To22500;
        } else if (ballDistance < 40000) {
            baseSpeed = Pass::Speeds::k22500To40000;
        } else if (ballDistance < 62500) {
            baseSpeed = Pass::Speeds::k40000To62500;
        } else if (ballDistance < 90000) {
            baseSpeed = Pass::Speeds::k62500To90000;
        } else if (ballDistance < 122500) {
            baseSpeed = Pass::Speeds::k90000To122500;
        } else {
            baseSpeed = Pass::Speeds::kFurtherThan122500;
        }

        // Add bonus based on passing skill
        int8_t passSkill = (int8_t)readMemory(A4 + PlayerOff::kPassing, 1);
        int16_t skillIdx = ((int16_t)passSkill) << 1;
        int16_t speedBonus = (int16_t)readMemory(Pass::Tables::kSpeedIncrease + skillIdx, 2);
        int16_t finalSpeed = baseSpeed + speedBonus;

        logDebug("[PASS_SPEED] dist=%u baseSpeed=%d passSkill=%d speedBonus=%d finalSpeed=%d",
                ballDistance, baseSpeed, passSkill, speedBonus, finalSpeed);

        writeMemory(A1 + SpriteOff::kSpeed, 2, finalSpeed);
        MEM_WORD(Pass::Tables::kGoodPassSampleCommand) = (int16_t)-1;
    }
    }  // End scope for variables that gotos may cross

reset_spin_timers:
    ResetBothTeamSpinTimers();

    // If human player, set spin timer
    int16_t playerNum = (int16_t)readMemory(A6 + TeamOff::kPlayerNumber, 2);
    if (playerNum != 0) {
        writeMemory(A6 + TeamOff::kSpinTimer, 2, 0);
    }

    // Set pass in progress flag
    writeMemory(A6 + TeamOff::kPassInProgress, 2, 1);

    // Check if should play samples
    int16_t gameStateCheck = MEM_WORD(GSAddr::kGameStatePl);
    if (gameStateCheck == GSVal::kGameInProgress) {
        goto play_samples;
    }

    {  // Scope for throwInState
    int16_t throwInState = MEM_WORD(GSAddr::kGameState);
    if (throwInState < GSVal::kThrowInForwardRight) {
        goto play_samples;
    }
    if (throwInState > GSVal::kThrowInBackLeft) {
        goto play_samples;
    }

    // Throw-in - don't play kick sample
    return;
    }  // End scope for throwInState

play_samples:
    SWOS::PlayKickSample();
    PlayStopGoodPassSampleIfNeeded();
}


// CalculateIfPlayerWinsBall - determines if player wins contested ball
// Assembly at 108037-108281
// Input: D0 = direction, A1 -> player sprite, A6 -> team data
void CalculateIfPlayerWinsBall() {
    // Reset pass in progress
    writeMemory(A6 + TeamOff::kPassInProgress, 2, 0);

    // Get opponent team
    uint32_t opponentTeam = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
    A0 = opponentTeam;

    // Check if opponent's won ball timer is active
    int16_t wonBallTimer = (int16_t)readMemory(A0 + TeamOff::kWonTheBallTimer, 2);
    if (wonBallTimer != 0) {
        goto set_team_direction;
    }

    {  // Scope for oppHasBall and oppAllowedDir
    // Check if opponent has the ball
    int16_t oppHasBall = (int16_t)readMemory(A0 + TeamOff::kPlayerHasBall, 2);
    if (oppHasBall == 0) {
        goto set_team_direction;
    }

    // Check if opponent is moving
    int16_t oppAllowedDir = (int16_t)readMemory(A0 + TeamOff::kCurrentAllowedDirection, 2);
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
        uint32_t opponentControlledSprite = readMemory(A0 + TeamOff::kControlledPlayerSprite, 4);
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
        int8_t firstTackling = (int8_t)readMemory(A4 + PlayerOff::kTackling, 1);
        int8_t firstBallControl = (int8_t)readMemory(A4 + PlayerOff::kBallControl, 1);
        int16_t firstAvg = (firstTackling + firstBallControl) >> 1;  // D1 in assembly

        // Second lookup: Switch A6 to opponent team, get their controlled sprite
        // Lines 108072-108080: A6 = opponentsTeam, A1 = A6.controlledPlayerSprite
        A6 = opponentTeam;
        uint32_t theirControlledSprite = readMemory(A6 + TeamOff::kControlledPlayerSprite, 4);
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
        int8_t secondTackling = (int8_t)readMemory(A4 + PlayerOff::kTackling, 1);
        int8_t secondBallControl = (int8_t)readMemory(A4 + PlayerOff::kBallControl, 1);
        int16_t secondAvg = (secondTackling + secondBallControl) >> 1;  // D0 in assembly

        // Calculate skill difference: D1 - D0 (first - second)
        // Lines 108089-108094
        int16_t skillDiff = firstAvg - secondAvg;
        if (skillDiff < 0) {
            // Opponent's player is more skilled - negate diff, set A6 to our team
            // (favors opponent in random check)
            skillDiff = -skillDiff;
            A6 = readMemory(A6 + TeamOff::kOpponentsTeam, 4);  // A6 = opponent's opponent = our team
        }
        // else: Our equivalent player is more skilled - keep A6 as opponent team

        // Random chance based on skill difference
        SWOS::Rand();
        int16_t randVal = D0 & 0x1F;

        int8_t chanceThreshold = (int8_t)readMemory(Tbl::kAvgTacklingBallControlDiffChance + skillDiff, 1);
        if (randVal >= chanceThreshold) {
            // Opponent wins - switch teams
            A6 = readMemory(A6 + TeamOff::kOpponentsTeam, 4);
        }

        // Winner gets control
        writeMemory(A6 + TeamOff::kWonTheBallTimer, 2, 12);

        // Stop the ball
        A2 = BallAddr::kSpriteOffset;
        writeMemory(A2 + SpriteOff::kSpeed, 2, 0);
        int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
        writeMemory(A2 + SpriteOff::kDestX, 2, ballX);
        int16_t ballY = (int16_t)readMemory(A2 + SpriteOff::kY + 2, 2);
        writeMemory(A2 + SpriteOff::kDestY, 2, ballY);

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
    writeMemory(A6 + TeamOff::kControlledPlDirection, 2, direction);

    // A2 -> ball sprite
    A2 = BallAddr::kSpriteOffset;

    // If ball going up, make it go down
    int32_t ballDeltaZ = (int32_t)readMemory(A2 + SpriteOff::kDeltaZ, 4);
    if (ballDeltaZ > 0) {
        writeMemory(A2 + SpriteOff::kDeltaZ, 4, (uint32_t)-1);
    }

    // Nudge ball away from player if direction is 0 and close
    if (direction == 0) {
        int16_t ballX = (int16_t)readMemory(A2 + SpriteOff::kX + 2, 2);
        int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
        int16_t xDiff = ballX - playerX;

        if (xDiff > -4 && xDiff < 4) {
            if (xDiff < 0) {
                writeMemory(A2 + SpriteOff::kX + 2, 2, ballX - 1);
            } else {
                writeMemory(A2 + SpriteOff::kX + 2, 2, ballX + 1);
            }
        }
    }

    // Set ball destination based on player position and direction
    int16_t tableIndex = direction << 2;
    int16_t playerX = (int16_t)readMemory(A1 + SpriteOff::kX + 2, 2);
    int16_t destOffsetX = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex, 2);
    writeMemory(A2 + SpriteOff::kDestX, 2, playerX + destOffsetX);

    int16_t playerY = (int16_t)readMemory(A1 + SpriteOff::kY + 2, 2);
    int16_t destOffsetY = (int16_t)readMemory(Tbl::kDefaultDestinations + tableIndex + 2, 2);
    writeMemory(A2 + SpriteOff::kDestY, 2, playerY + destOffsetY);

    // Get player skill for ball speed
    GetPlayerPointerFromShirtNumber();

    int16_t speedDelta = 0;
    int16_t currentTick = MEM_WORD(GSAddr::kCurrentTick);
    if ((currentTick & 2) != 0) {
        // Add speed based on ball control skill at odd frames
        int8_t ballControl = (int8_t)readMemory(A4 + PlayerOff::kBallControl, 1);
        int16_t skillIndex = ((int16_t)ballControl) << 1;
        speedDelta = (int16_t)readMemory(Tbl::kBallSpeedDeltaWhenControlled + skillIndex, 2);
    }

    // Ball speed = player speed + offset
    int16_t playerSpeed = (int16_t)readMemory(A1 + SpriteOff::kSpeed, 2);
    writeMemory(A2 + SpriteOff::kSpeed, 2, playerSpeed + speedDelta);

    // Check if player is turning
    int16_t fullDir = (int16_t)readMemory(A1 + SpriteOff::kFullDirection, 2) & 0xFF;
    int16_t oppositeDir = (fullDir + 128) & 0xFF;

    int16_t playerDir = (int16_t)readMemory(A1 + SpriteOff::kDirection, 2);
    int16_t dirAngle = playerDir << 5;

    int16_t angleDiff = oppositeDir - dirAngle;
    if ((int8_t)angleDiff >= 64 || (int8_t)angleDiff <= -64) {
        // Player turning sharply - increase ball speed
        int16_t ballSpeed = (int16_t)readMemory(A2 + SpriteOff::kSpeed, 2);
        writeMemory(A2 + SpriteOff::kSpeed, 2, ballSpeed + 256);
    }

    // Check if player changing direction
    int16_t controlledDir = (int16_t)readMemory(A6 + TeamOff::kControlledPlDirection, 2);
    if (controlledDir != playerDir) {
        // Player changing direction - increment turn timer
        int16_t turnTimer = (int16_t)readMemory(A6 + TeamOff::kBallControlTurnTimer, 2);
        writeMemory(A6 + TeamOff::kBallControlTurnTimer, 2, turnTimer + 1);

        // Check ball control threshold based on player skill
        int8_t ballControl = (int8_t)readMemory(A4 + PlayerOff::kBallControl, 1);
        int16_t skillIndex = ((int16_t)ballControl) << 1;
        int16_t threshold = (int16_t)readMemory(Tbl::kBallControlTurnTimer + skillIndex, 2);

        if (threshold <= turnTimer + 1) {
            writeMemory(A6 + TeamOff::kWonTheBallTimer, 2, 8);
        }
    }

    ResetBothTeamSpinTimers();
}

} // namespace PlayerFunctions
