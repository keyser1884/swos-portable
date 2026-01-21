#include "ai.h"
#include "../../stdinc.h"
#include "../../options/portedFunctions.h"
#include "../../util/random.h"

using namespace SwosVM;

// AI Variables - these are in filler regions of SwosVariables
// Offsets calculated from assembly analysis

// Helper macro to access memory at specific byte offset
#define MEM_WORD(offset) (*(int16_t*)(kMemStart + (offset)))
#define MEM_BYTE(offset) (*(int8_t*)(kMemStart + (offset)))
#define MEM_UWORD(offset) (*(uint16_t*)(kMemStart + (offset)))

// AI variables offsets (relative to kMemStart, which is g_memByte + 256)
// The enum values in vm.h are raw offsets into g_memByte, so we subtract 256.
// AI_turnDirection is at offset 526780 from kMemStart (verified by debug output)
// These follow sequentially:
constexpr uint32_t kAI_resumePlayTimer = 526782;  // word (2 bytes) - AI_turnDirection + 2
constexpr uint32_t kAI_rand = 526784;             // word (2 bytes) - followed by 2 bytes padding
constexpr uint32_t kAI_randomRotateTable = 526788;
constexpr uint32_t kAI_leftSpinTable = 526792;
constexpr uint32_t kAI_rotateRightTable = 526798;
constexpr uint32_t kAI_longKickTable = 526804;
constexpr uint32_t kAI_counter = 526810;
constexpr uint32_t kAI_attackHalf = 526812;
constexpr uint32_t kAI_counterWriteOnly = 526814;
constexpr uint32_t kAI_throwInDirections = 526816;
constexpr uint32_t kAI_maxStoppageTime = 455734;  // 455990 - 256 (NOT 449800 which is hideBall!)

// Accessor macros for AI variables
#define AI_counter MEM_WORD(kAI_counter)
#define AI_resumePlayTimer MEM_WORD(kAI_resumePlayTimer)
#define AI_rand MEM_UWORD(kAI_rand)
#define AI_maxStoppageTime MEM_WORD(kAI_maxStoppageTime)
#define AI_attackHalf MEM_WORD(kAI_attackHalf)
#define AI_counterWriteOnly MEM_WORD(kAI_counterWriteOnly)

// Table accessors
#define AI_randomRotateTable ((int16_t*)(kMemStart + kAI_randomRotateTable))
#define AI_leftSpinTable ((int16_t*)(kMemStart + kAI_leftSpinTable))
#define AI_rotateRightTable ((int16_t*)(kMemStart + kAI_rotateRightTable))
#define AI_longKickTable ((int16_t*)(kMemStart + kAI_longKickTable))
#define AI_throwInDirections ((uint8_t*)(kMemStart + kAI_throwInDirections))

// Game state constants (matching GameState enum values)
constexpr int16_t ST_PLAYERS_TO_INITIAL = 0;
constexpr int16_t ST_GOAL_OUT_LEFT = 1;
constexpr int16_t ST_GOAL_OUT_RIGHT = 2;
constexpr int16_t ST_KEEPER_HOLDS_BALL = 3;
constexpr int16_t ST_CORNER_LEFT = 4;
constexpr int16_t ST_CORNER_RIGHT = 5;
constexpr int16_t ST_FREE_KICK_LEFT1 = 6;
constexpr int16_t ST_FREE_KICK_RIGHT3 = 12;
constexpr int16_t ST_FOUL = 13;
constexpr int16_t ST_PENALTY = 14;
constexpr int16_t ST_THROW_IN_FORWARD_RIGHT = 15;
constexpr int16_t ST_THROW_IN_BACK_LEFT = 20;
constexpr int16_t ST_STARTING_GAME = 21;
constexpr int16_t ST_RESULT_ON_HALFTIME = 25;
constexpr int16_t ST_RESULT_AFTER_THE_GAME = 26;
constexpr int16_t ST_GAME_ENDED = 30;
constexpr int16_t ST_GAME_IN_PROGRESS = 100;
constexpr int16_t ST_PENALTIES = 101;

// External ASM function (declared in global namespace)
extern void AI_SetControlsDirection();

namespace {

// Local variables used during AI processing (mirrors assembly registers)
TeamGeneralInfo* team;          // A6 - current team being processed
Sprite* controlledPlayer;       // A5 - controlled player sprite
Sprite* passToPlayer;           // A4 - player receiving pass
int16_t playerDirection;        // D7 - controlled player direction (0-7, -1 if none)
int16_t goalAngle;              // D5 - angle from ball to opponent's goal center (0-255)
int32_t goalDistanceSqr;        // D6 - squared distance to goal

// Helper: convert angle to 8-direction with rounding
inline int16_t angleToDirection(int16_t angle) {
    return ((angle + 16) & 0xFF) >> 5;
}

// AI_ResumeGameDelay - checks if it's time to resume play
// Assembly: cmp passKickTimer, 13; retn -> jnb return
// jnb (jump if not below) taken when passKickTimer >= 13 (don't shoot)
// Continue to shooting when passKickTimer < 13
// Returns true if we can continue (passKickTimer < 13)
bool AI_ResumeGameDelay() {
    return team->passKickTimer < 13;
}

// AI_SetDirectionTowardOpponentsGoal - sets direction toward goal when AI_counter active
// Assembly at 119102-119151
void AI_SetDirectionTowardOpponentsGoal() {
    if (AI_counter == 0)
        return;

    int16_t ballX = swos.ballSprite.x.whole();

    // Determine direction based on which half of pitch and which team
    // Ball position relative to center (336) determines whether to go diagonally left or right
    // IMPORTANT: Only the team that's actually attacking should process this!
    int16_t direction;
    if (AI_attackHalf == 1) {
        // Attacking upper half - only bottom team should process
        if (team != &swos.bottomTeamData)
            return;
        if (ballX < 300) direction = 1;      // up-right (ball on left, go right toward center)
        else if (ballX > 371) direction = 7; // up-left (ball on right, go left toward center)
        else direction = 0;                   // straight up (ball near center)
    } else {
        // Attacking lower half - only top team should process
        if (team != &swos.topTeamData)
            return;
        if (ballX < 300) direction = 3;      // down-right (ball on left, go right toward center)
        else if (ballX > 371) direction = 5; // down-left (ball on right, go left toward center)
        else direction = 4;                   // straight down (ball near center)
    }

    team->currentAllowedDirection = direction;
}

// AI_DecideWhetherToTriggerFire - decides if AI should shoot/pass
// Returns true if should NOT fire (no action taken)
bool AI_DecideWhetherToTriggerFire() {
    if (!controlledPlayer)
        return true;

    // Goalkeeper never fires in this context
    if (controlledPlayer->playerOrdinal == 1)
        return true;

    // Check if player is facing toward opponent's goal
    bool facingGoal = false;
    if (team == &swos.topTeamData) {
        // Top team attacks downward (directions 3,4,5)
        facingGoal = (playerDirection >= 3 && playerDirection <= 5);
    } else {
        // Bottom team attacks upward (directions 7,0,1)
        facingGoal = (playerDirection == 7 || playerDirection == 0 || playerDirection == 1);
    }

    if (!facingGoal)
        return true;

    // Check if player is close enough to ball (distance < 648 = 18^2 + 18^2)
    if (controlledPlayer->ballDistance > 648)
        return true;

    // Check ball height conditions
    // IMPORTANT: ASM checks if raw deltaZ value is negative (sign bit), not truncated integer
    // FixedPoint is 16.16 format, so we check raw() < 0 to match ASM's "js" instruction
    int32_t ballDeltaZRaw = swos.ballSprite.deltaZ.raw();
    int16_t ballZ = swos.ballSprite.z.whole();

    if (ballDeltaZRaw < 0) {
        // Ball falling - acceptable height range [12, 20]
        if (ballZ < 12 || ballZ > 20)
            return true;
    } else {
        // Ball rising or on ground - acceptable height range [8, 14]
        if (ballZ < 8 || ballZ > 14)
            return true;
    }

    // Trigger fire
    team->fireThisFrame = 1;

    // Set direction opposite from controlled player
    int16_t fullDir = controlledPlayer->fullDirection;
    int16_t oppositeDir = ((fullDir + 128) & 0xFF) >> 5;
    team->currentAllowedDirection = oppositeDir;

    // Check if rounded direction matches player direction
    int16_t roundedDir = ((fullDir + 128 + 16) & 0xFF) >> 5;
    if (roundedDir != playerDirection)
        return true;

    team->currentAllowedDirection = roundedDir;
    AI_counter = 15;
    AI_counterWriteOnly = roundedDir;
    AI_attackHalf = (team == &swos.topTeamData) ? 2 : 1;

    return false;  // Fire triggered
}

// FindClosestPlayerToBallFacing - finds closest player (from EITHER team) facing specified direction
// ASM searches both teams and the caller checks if result is a teammate before passing
Sprite* FindClosestPlayerToBallFacing(int16_t targetDirection) {
    Sprite* closestPlayer = nullptr;
    uint32_t closestDistance = UINT32_MAX;

    // Search BOTH teams' players (ASM behavior)
    // This allows the caller to check if closest is teammate or opponent
    SwosDataPointer<Sprite>* allPlayers[2] = {
        swos.topTeamData.players,
        swos.bottomTeamData.players
    };

    for (int t = 0; t < 2; t++) {
        SwosDataPointer<Sprite>* sprites = allPlayers[t];
        for (int i = 0; i < 11; i++) {
            Sprite* player = sprites[i];
            if (!player) continue;

            // Skip controlled player (from current team only)
            if (player == team->controlledPlayerSprite) continue;

            // Skip sent-off players
            if (player->sentAway) continue;

            // Skip players not in normal state
            if (player->state != PlayerState::kNormal) continue;

            // Check direction match (+/- 16 from target, in 0-255 scale)
            int16_t dirDiff = static_cast<int16_t>(player->fullDirection) - targetDirection;
            // Handle wrap-around
            if (dirDiff > 128) dirDiff -= 256;
            if (dirDiff < -128) dirDiff += 256;

            if (dirDiff < -16 || dirDiff > 16) continue;

            // Track closest
            if (player->ballDistance < closestDistance) {
                closestDistance = player->ballDistance;
                closestPlayer = player;
            }
        }
    }

    D2 = closestDistance;
    return closestPlayer;
}

// Handle game stopped states (throw-ins, free kicks, etc.)
bool handleStoppedState() {
    int16_t gameState = static_cast<int16_t>(swos.gameState);


    // Assembly at @@game_not_over (117903-117912)
    // Timing check: if stoppageTimer is low, use simpler direction handling
    int16_t threshold = (swos.currentGameTick & 0x3F) + 100;
    if (threshold > swos.stoppageTimerActive) {
        // Go to simplified direction handling (@@set_direction path at 117863)
        // Check if result is showing (@@check_if_result_shown at 117867)
        if (swos.resultTimer)
            return true;

        // Assembly: ST_GOAL_OUT_LEFT, ST_GOAL_OUT_RIGHT, ST_KEEPER_HOLDS_BALL -> updateTurnDirection
        if (gameState == ST_GOAL_OUT_LEFT || gameState == ST_GOAL_OUT_RIGHT ||
            gameState == ST_KEEPER_HOLDS_BALL) {
            goto updateTurnDirection;
        }
        // Assembly: throw-ins -> updateTurnDirection
        if (gameState >= ST_THROW_IN_FORWARD_RIGHT && gameState <= ST_THROW_IN_BACK_LEFT) {
            goto updateTurnDirection;
        }

        // For foul or free kick -> set direction toward goal (@@foul_or_free_kick at 117891)
        if (gameState == ST_FOUL ||
            (gameState >= ST_FREE_KICK_LEFT1 && gameState <= ST_FREE_KICK_RIGHT3)) {
            int16_t targetDir = angleToDirection(goalAngle);
            team->currentAllowedDirection = targetDir;
            return true;
        }

        // Otherwise just return (assembly returns for ST_PLAYERS_TO_INITIAL and other states)
        return true;
    }

    // Detailed game state handling (stoppageTimer is high enough) - @@game_not_over path
    // NOTE: Assembly does NOT check resultTimer here - it goes directly into state checks

    // Check for penalties first (assembly 117913-117917)
    if (swos.playingPenalties || gameState == ST_PENALTY) {
        // @@doing_penalties (118027-118052)
        int16_t randDir = AI_rand & 7;
        int16_t dirMask = 1 << randDir;
        if (swos.playerTurnFlags & dirMask) {
            team->currentAllowedDirection = randDir;
            return true;
        }
        // Random direction not allowed - check if player facing up or down
        if (controlledPlayer) {
            if (controlledPlayer->direction == 0 || controlledPlayer->direction == 4)
                return true;
        }
        // Continue to @@apply_after_touch
        goto applyAfterTouch;
    }

    // Handle kickoff (ST_PLAYERS_TO_INITIAL = 0)
    // Assembly at @@goal_scored (117983)
    if (gameState == ST_PLAYERS_TO_INITIAL) {
        // Wait at least 150 ticks before kicking off
        if (swos.stoppageTimerActive < 150) {
            goto findClosestPlayer;
        }
        // If player facing camera direction, kick toward goal
        if (playerDirection == swos.cameraDirection) {
            goto applyAfterTouch;
        }
        // Otherwise find closest player
        goto findClosestPlayer;
    }

    // Handle keeper's ball and goal kicks (@@keepers_ball at 117957)
    if (gameState == ST_GOAL_OUT_LEFT || gameState == ST_GOAL_OUT_RIGHT ||
        gameState == ST_KEEPER_HOLDS_BALL) {
        // 50% chance to find closest player directly
        if ((AI_rand & 1) == 0) {
            goto findClosestPlayer;
        }
        // Check if player facing camera direction
        if (playerDirection == swos.cameraDirection) {
            goto applyAfterTouch;
        }
        // Check ball position and player direction
        int16_t ballX = swos.ballSprite.x.whole();
        if (ballX <= 336) {
            // Ball on left side - directions 1,3 ok
            if (playerDirection == 1 || playerDirection == 3) {
                goto applyAfterTouch;
            }
        } else {
            // Ball on right side - directions 5,7 ok
            if (playerDirection == 5 || playerDirection == 7) {
                goto applyAfterTouch;
            }
        }
        // None of the above - find closest player
        goto findClosestPlayer;
    }

    // Throw-ins
    if (gameState >= ST_THROW_IN_FORWARD_RIGHT && gameState <= ST_THROW_IN_BACK_LEFT) {
        // Get throw-in direction from table
        int idx = gameState - ST_THROW_IN_FORWARD_RIGHT;
        uint8_t throwDir = AI_throwInDirections[idx];

        // Flip direction if TOP team (NOT bottom team)
        // Assembly: jz cseg_84671 (skip rotation if bottom team)
        if (team != &swos.bottomTeamData) {
            throwDir = (throwDir >> 4) | (throwDir << 4);  // ror 4
        }

        // Only 1/16 chance to check direction mask
        // Assembly: jnz cseg_84775 (if != 0, go find closest player)
        if ((AI_rand & 0x0F) == 0) {
            // Check if current direction allows throw
            int dirMask = 1 << playerDirection;
            if (throwDir & dirMask) {
                // Apply after-touch
                goto applyAfterTouch;
            }
        }
        goto findClosestPlayer;
    }

    // Free kicks and fouls
    if (gameState == ST_FOUL ||
        (gameState >= ST_FREE_KICK_LEFT1 && gameState <= ST_FREE_KICK_RIGHT3)) {
        // Assembly at @@free_kick (118004-118010):
        // 1/16 chance to find closest player, otherwise check direction
        if ((AI_rand & 0x0F) == 0) {
            goto findClosestPlayer;
        }
        // Check if player direction matches goal direction
        int16_t goalDir = angleToDirection(goalAngle);
        if (playerDirection == goalDir) {
            // Player facing goal - shoot
            goto applyAfterTouch;
        }
        // Not facing goal - set direction toward goal (cseg_84815)
        team->currentAllowedDirection = goalDir;
        return true;
    }

    // Default: set direction toward goal
    {
        int16_t targetDir = angleToDirection(goalAngle);
        team->currentAllowedDirection = targetDir;
        return true;
    }

findClosestPlayer:
    // Assembly at cseg_84775 (118079)
    // Find closest player facing same direction and pass to them
    {
        int16_t searchDir = playerDirection << 5;
        Sprite* target = FindClosestPlayerToBallFacing(searchDir);

        if (target && controlledPlayer &&
            target->teamNumber == controlledPlayer->teamNumber) {
            // Our player is closest - go to @@our_player_closest (118537)
            // Assembly: if timers not ready, just RETURN (don't change direction!)
            team->ofs134 = 0;  // field_84 at offset 0x84 = 132

            if (AI_resumePlayTimer != 0)
                return true;  // Wait for timer
            if (!AI_ResumeGameDelay())
                return true;  // Wait for passKickTimer < 13

            // Timer ready - quick pass
            AI_resumePlayTimer = 15;
            team->currentAllowedDirection = playerDirection;
            team->quickFire = 1;
            // Update max stoppage time
            if (AI_maxStoppageTime <= swos.stoppageTimerActive) {
                AI_maxStoppageTime = swos.stoppageTimerActive;
            }
            return true;
        }
    }
    // No matching player found - fall through to turn direction update

updateTurnDirection:
    // Assembly at @@update_turn_direction (118124-118175)
    // Gradual turn - only process on certain frames
    if ((swos.currentGameTick & 0x0E) != 0)
        return true;

    {
        int16_t turnDir = swos.AI_turnDirection;
        int16_t saveDir;  // D1 in assembly - saved if direction blocked

        if (turnDir < 0) {
            // Negative turn direction
            if (turnDir != -1) {
                // Not at -1 yet, increment toward it
                swos.AI_turnDirection++;
                if (swos.AI_turnDirection != -1)
                    return true;
                // Fall through - reached -1
            }
            // turnDir == -1: turn LEFT (add -1 to direction)
            saveDir = 1;  // Save +1 if blocked
        } else if (turnDir > 0) {
            // Positive turn direction
            if (turnDir != 1) {
                // Not at +1 yet, decrement toward it
                swos.AI_turnDirection--;
                if (swos.AI_turnDirection != 1)
                    return true;
                // Fall through - reached +1
            }
            // turnDir == +1: turn RIGHT (add +1 to direction)
            saveDir = -1;  // Save -1 if blocked
        } else {
            // turnDir == 0: assembly treats this same as positive case (jns jumps when >= 0)
            // Decrement toward -1 and return without applying direction
            swos.AI_turnDirection--;
            return true;
        }

        // Calculate new direction: playerDirection + AI_turnDirection
        int16_t newDir = (playerDirection + swos.AI_turnDirection) & 7;

        // Check if this direction is allowed
        if (swos.playerTurnFlags & (1 << newDir)) {
            team->currentAllowedDirection = newDir;
        } else {
            // Direction blocked - save opposite direction for next frame
            swos.AI_turnDirection = saveDir;
        }
    }
    return true;

applyAfterTouch:
    // Assembly at 118071-118072: Early return if playerDirection < 0
    if (playerDirection < 0)
        return true;

    // Apply ball spin based on direction
    // Assembly at 118073-118075: D2 = (D7 << 5) - D5.lo (byte subtraction)
    int8_t dirDiffByte = static_cast<int8_t>((playerDirection << 5) - goalAngle);

    // Update max stoppage time tracking (@@update_max_stoppage_time at 118730-118737)
    if (AI_maxStoppageTime <= swos.stoppageTimerActive) {
        AI_maxStoppageTime = swos.stoppageTimerActive;
    }

    AI_resumePlayTimer = 15;

    // Assembly at 118742-118795: Set AI_afterTouchStrength based on gameState and distance
    // This was missing from the original port!
    int16_t afterTouchStrength;
    if (gameState == ST_PENALTY || gameState == ST_PENALTIES) {
        afterTouchStrength = 0;  // weak for penalties
    } else if (gameState >= ST_FREE_KICK_LEFT1 && gameState <= ST_FREE_KICK_RIGHT3) {
        // Free kicks: check distance
        goto checkDistanceForStrength;
    } else if (gameState == ST_CORNER_LEFT || gameState == ST_CORNER_RIGHT) {
        afterTouchStrength = 1;  // medium for corners
    } else {
        // Not penalty, free kick, or corner - random check
        // Assembly at 118760-118768: if (AI_rand & 0x18) != 0 -> check distance
        // If (AI_rand & 0x18) == 0 -> strong (strength 2)
        if ((AI_rand & 0x18) != 0) {
            goto checkDistanceForStrength;
        }
        afterTouchStrength = 2;  // strong
    }
    goto setStrengthDone;

checkDistanceForStrength:
    // Assembly at 118771-118777: use goalDistanceSqr (D6) to determine strength
    if (goalDistanceSqr < 28800) {
        afterTouchStrength = 0;  // weak
    } else if (goalDistanceSqr < 57800) {
        afterTouchStrength = 1;  // medium
    } else {
        afterTouchStrength = 2;  // strong
    }

setStrengthDone:
    team->ofs136 = afterTouchStrength;  // AI_afterTouchStrength (offset 134)

    team->currentAllowedDirection = playerDirection;
    team->normalFire = 1;

    // Assembly at 118804-118811: Check for penalties and corners AFTER setting normalFire
    // Penalties: no spin (@@no_after_touch at 118861-118865)
    // Note: gameState is already defined in handleStoppedState's outer scope
    if (gameState == ST_PENALTY || gameState == ST_PENALTIES) {
        team->ofs138 = 0;  // No spin for penalties
        return true;
    }

    // Corners: special spin handling (@@corner at 118850-118892)
    if (gameState == ST_CORNER_LEFT || gameState == ST_CORNER_RIGHT) {
        int16_t randVal = AI_rand & 7;
        if (randVal >= 6) {
            // No spin
            team->ofs138 = 0;
            return true;
        }
        // Try to turn direction and set spin
        int16_t spinDir, newDir;
        if (randVal < 3) {
            // Turn right, spin +1
            spinDir = 1;
            newDir = (playerDirection + 1) & 7;
        } else {
            // Turn left, spin -1
            spinDir = -1;
            newDir = (playerDirection - 1) & 7;
        }
        // Check if turn direction is allowed
        int16_t dirMask = 1 << newDir;
        if (!(swos.playerTurnFlags & dirMask)) {
            // Direction not allowed - no spin
            team->ofs138 = 0;
            return true;
        }
        team->ofs138 = spinDir;
        return true;
    }

    // Normal case: Determine spin direction based on angle difference
    // Assembly at 118836-118840: compares D2.lo as SIGNED BYTE
    int16_t spinDir = (dirDiffByte >= 0) ? -1 : 1;
    team->ofs138 = spinDir;  // AI_ballSpinDirection (offset 136) equivalent

    return true;
}

// Handle penalties
bool handlePenalties() {
    // Random direction selection for penalties
    int16_t randDir = AI_rand & 7;
    int16_t dirMask = 1 << randDir;

    if (swos.playerTurnFlags & dirMask) {
        // Random direction is allowed
        team->currentAllowedDirection = randDir;
        return true;
    }

    // If player facing up or down, don't change
    if (controlledPlayer) {
        if (controlledPlayer->direction == 0 || controlledPlayer->direction == 4)
            return true;
    }

    // Otherwise apply after-touch logic
    return false;  // Continue to after-touch
}

// NOTE: handleKeepersBall functionality is inlined in handleStoppedState (lines 295-321)
// where it handles ST_GOAL_OUT_LEFT, ST_GOAL_OUT_RIGHT, and ST_KEEPER_HOLDS_BALL states

// Actually trigger the shot - called after angle check passes
// Matches assembly at cseg_850F9 (118682-118727)
bool triggerShot(int16_t afterTouchStrength) {
    // Check cooldown timer
    if (AI_resumePlayTimer > 0)
        return true;  // Can't shoot yet, still in cooldown

    // Check if it's time to resume play
    if (!AI_ResumeGameDelay())
        return true;  // Not ready yet

    // Set up the shot
    team->ofs136 = afterTouchStrength;  // AI_afterTouchStrength (offset 134)
    AI_resumePlayTimer = 15;
    team->currentAllowedDirection = playerDirection;
    team->normalFire = 1;

    // Calculate spin direction based on angle diff
    // Assembly at 118717-118721: compares D2.lo as a SIGNED BYTE, not int16
    int8_t angleDiffByte = static_cast<int8_t>((playerDirection << 5) - goalAngle);
    int16_t spinDir = (angleDiffByte >= 0) ? -1 : 1;
    team->ofs138 = spinDir;  // AI_ballSpinDirection (offset 136)

    return true;  // Shot triggered
}

// Handle shooting decision when close to goal
// Matches assembly at cseg_84A53 (118268-118300) -> cseg_850F9
bool handleShootingOpportunity() {
    // Check distance thresholds for shooting
    if (goalDistanceSqr > 28800)  // Too far
        return false;

    // Random chance for medium distances
    if (goalDistanceSqr >= 12800) {
        if ((AI_rand & 3) != 0)
            return false;  // 75% chance to skip shooting at medium range
    }

    // Determine angle threshold based on distance
    int16_t threshold = (goalDistanceSqr < 3200) ? 50 : 15;

    // Check if player direction is valid
    if (playerDirection < 0)
        return false;

    // Calculate angle difference: (playerDirection * 32) - goalAngle
    int16_t dirScaled = playerDirection << 5;
    int8_t angleDiff = static_cast<int8_t>(dirScaled - goalAngle);

    // Check if player is facing toward goal (within threshold)
    // Assembly: if D2 > threshold, skip; if D2 > -threshold, shoot
    // So shoot when: -threshold < angleDiff <= threshold
    if (angleDiff > threshold)
        return false;  // Player facing too far from goal direction
    if (angleDiff > -threshold) {
        // Within threshold - trigger shot
        // Assembly at cseg_850F9 (118689): ALWAYS uses afterTouchStrength = 0
        return triggerShot(0);
    }

    return false;  // angleDiff <= -threshold, too far off
}

// Trigger a quick pass - matches @@our_player_closest (118537-118562)
bool triggerPass() {
    // Check cooldown timer
    if (AI_resumePlayTimer > 0)
        return false;  // Can't pass yet

    // Check if it's time to act
    if (!AI_ResumeGameDelay())
        return false;  // Not ready yet

    // Execute the pass
    AI_resumePlayTimer = 15;
    team->currentAllowedDirection = playerDirection;
    team->quickFire = 1;

    // Update max stoppage time tracking
    if (AI_maxStoppageTime <= swos.stoppageTimerActive) {
        AI_maxStoppageTime = swos.stoppageTimerActive;
    }

    return true;
}

// Long-range shooting when under pressure
// Matches assembly at cseg_84F4B (118565-118627)
// Called when opponent is very close but player is far from goal
bool triggerLongRangeShot() {
    // Check if player is roughly facing goal (within 1 direction step)
    // Assembly: convert goalAngle to direction and compare
    int16_t goalDir = ((goalAngle + 16) & 0xFF) >> 5;
    int16_t dirDiff = (goalDir - playerDirection) & 7;

    // Assembly at 118575-118580: only shoot if dirDiff is 0 or 1
    // NOTE: ASM has "cmp D0, -1" at 118578 which NEVER matches after "and D0, 7"
    // because D0 is 0-7 after AND, never 0xFFFF. So ASM only accepts 0 or 1.
    if (dirDiff != 0 && dirDiff != 1) {
        return false;  // Not facing goal, can't shoot
    }

    // NOTE: Assembly at cseg_84FA0 (118583+) does NOT check timer before shooting!
    // Long-range shots are executed immediately when direction is correct.
    // This differs from close-range shots at cseg_850F9 which do check the timer.

    // Set afterTouchStrength based on distance
    // Assembly at 118586-118590: strength = 2 if distance > 115600, else 1
    int16_t afterTouchStrength = (goalDistanceSqr > 115600) ? 2 : 1;
    team->ofs136 = afterTouchStrength;

    AI_resumePlayTimer = 15;
    team->currentAllowedDirection = playerDirection;
    team->normalFire = 1;

    // Calculate initial spin direction (same signed byte logic as triggerShot)
    // Assembly at 118598-118608
    int8_t angleDiffByte = static_cast<int8_t>((playerDirection << 5) - goalAngle);
    int16_t spinDir = (angleDiffByte >= 0) ? -1 : 1;

    // Assembly at 118610-118678: Adjust spin based on ball position and game timing
    // This can modify spinDir to 0 or negate it
    // IMPORTANT: ASM checks raw deltaY value sign, not truncated float-to-int
    int32_t ballDeltaYRaw = swos.ballSprite.deltaY.raw();
    int16_t ballY = swos.ballSprite.y.whole();
    int16_t ballX = swos.ballSprite.x.whole();

    // Early exit conditions - use original spinDir
    bool adjustSpin = true;
    if (ballDeltaYRaw >= 0) {
        // Ball moving down or stationary (118611-118616)
        if (ballY > 555) {
            adjustSpin = false;  // cseg_850E1
        }
    } else {
        // Ball moving up (118619-118621)
        if (ballY < 342) {
            adjustSpin = false;  // cseg_850E1
        }
    }

    if (adjustSpin) {
        // Assembly at cseg_8504E (118623-118678): Complex ball position/timing logic
        int16_t tickMod = (swos.currentGameTick & 0x1C) >> 2;  // D1, range 0-7

        if (ballX >= 193 && ballX < 478) {
            // cseg_85097: Ball in center-ish X range
            if (tickMod == 0) {
                spinDir = 0;  // cseg_850CF
            } else if (tickMod >= 5) {
                spinDir = -spinDir;  // cseg_850DA
            }
            // else: use original spinDir (cseg_850E1)
        } else if (ballX >= 118 && ballX <= 553) {
            // cseg_850AE: Ball in middle X range but not center
            if (tickMod == 0) {
                spinDir = -spinDir;  // cseg_850DA
            } else if (tickMod < 4) {
                spinDir = 0;  // cseg_850CF
            }
            // else: use original spinDir (cseg_850E1)
        } else {
            // cseg_850C5: Ball at extreme X (< 118 or > 553)
            if (tickMod >= 4) {
                spinDir = 0;  // cseg_850CF
            }
            // else: use original spinDir (cseg_850E1)
        }
    }

    team->ofs138 = spinDir;

    return true;
}

// Main ball control logic when player has ball
// Matches assembly at @@theres_a_player_near (118228) and following
void handleBallControl() {
    // Assembly at cseg_84A0D (118246-118267): Check if player facing sideways near own goal
    // Skip shooting if player facing left (2) or right (6) and ball is near their own goal
    // This prevents the AI from shooting when they might score an own goal
    bool skipShooting = false;
    if (playerDirection == 2 || playerDirection == 6) {
        int16_t ballY = swos.ballSprite.y.whole();
        if (team == &swos.topTeamData) {
            // Top team's goal is at y >= 740
            if (ballY >= 740) {
                skipShooting = true;
            }
        } else {
            // Bottom team's goal is at y <= 158
            if (ballY <= 158) {
                skipShooting = true;
            }
        }
    }

    // Check shooting opportunity based on distance to goal
    // Assembly at cseg_84A53 (118268)
    if (!skipShooting && handleShootingOpportunity())
        return;

    // Track if we tried to pass but failed (for alternating turn logic)
    // Must be declared before retry pass block since goto can jump here
    bool passingFailed = false;

    // Assembly at cseg_84AEB (118302-118323): "Retry pass" mechanism
    // If field_84 > 0, decrement it and try to find pass target before doing opponent checks
    if (team->ofs134 > 0) {
        team->ofs134--;
        int16_t searchDir = playerDirection << 5;
        Sprite* target = FindClosestPlayerToBallFacing(searchDir);

        if (target && controlledPlayer &&
            target->teamNumber == controlledPlayer->teamNumber) {
            // Our player closest - go to pass (@@our_player_closest)
            team->ofs134 = 0;
            if (AI_resumePlayTimer != 0)
                return;
            if (!AI_ResumeGameDelay())
                return;

            AI_resumePlayTimer = 15;
            team->currentAllowedDirection = playerDirection;
            team->quickFire = 1;
            if (AI_maxStoppageTime <= swos.stoppageTimerActive) {
                AI_maxStoppageTime = swos.stoppageTimerActive;
            }
            return;
        }
        // No matching player - fall through to alternating turns (cseg_84D57)
        // ASM always does alternating turns here, so set passingFailed = true
        passingFailed = true;
        goto alternatingTurns;
    }

    // Check if opponents are close - assembly at cseg_84B5B (118326)
    if (goalDistanceSqr >= 9800 && controlledPlayer) {
        TeamGeneralInfo* opponents = team->opponentTeam;

        if (opponents) {
            Sprite* oppControlled = opponents->controlledPlayerSprite;
            Sprite* oppPassTarget = opponents->passToPlayerPtr;

            // Check if opponent is very close (threatening)
            // Assembly at 118337-118358: Only check oppPassTarget if oppControlled is null
            // or has distance >= 5000. Do NOT check oppPassTarget if oppControlled is
            // moderately close (800-5000).
            bool veryClose = false;
            bool moderatelyClose = false;
            bool checkPassTarget = true;

            if (oppControlled) {
                if (oppControlled->ballDistance < 800) {
                    veryClose = true;
                    checkPassTarget = false;  // Assembly jumps to cseg_84C00, skips passTarget check
                } else if (oppControlled->ballDistance < 5000) {
                    moderatelyClose = true;
                    checkPassTarget = false;  // Assembly jumps to cseg_84C93, skips passTarget check
                }
                // If distance >= 5000, checkPassTarget stays true (assembly falls through to cseg_84BBE)
            }

            if (checkPassTarget && oppPassTarget) {
                if (oppPassTarget->ballDistance < 800) veryClose = true;
                else if (oppPassTarget->ballDistance < 5000) moderatelyClose = true;
            }

            // If opponent very close
            if (veryClose) {
                // Assembly at cseg_84C00 (118363-118364):
                // If far from goal (distance > 180000), shoot long range!
                if (goalDistanceSqr > 180000) {
                    if (triggerLongRangeShot())
                        return;
                    // If can't shoot, fall through to alternating turns
                    passingFailed = true;
                } else {
                    // Try to pass
                    int16_t searchDir = playerDirection << 5;
                    Sprite* target = FindClosestPlayerToBallFacing(searchDir);

                    if (target && target->teamNumber == controlledPlayer->teamNumber) {
                        // Our player is closest - pass to them
                        team->ofs134 = 0;  // field_84 at offset 0x84 = 132
                        triggerPass();
                        return;
                    }
                    // Passing search failed - will use alternating turns
                    passingFailed = true;
                }
            }

            // If moderately close - assembly at cseg_84C93 (118390-118395)
            // Bug 12 fix: condition is AI_rand > 8, not < 8!
            if (moderatelyClose) {
                // If rand <= 8 AND distance > 48400, shoot!
                if (AI_rand <= 8 && goalDistanceSqr > 48400) {
                    if (triggerLongRangeShot())
                        return;
                    // If can't shoot, fall through to alternating turns
                    passingFailed = true;
                }
                // If rand > 8 OR distance <= 48400, try passing
                else if ((swos.currentGameTick & 0x0C) == 0) {
                    int16_t searchDir = playerDirection << 5;
                    Sprite* target = FindClosestPlayerToBallFacing(searchDir);

                    if (target && target->teamNumber == controlledPlayer->teamNumber) {
                        team->ofs134 = 0;  // field_84 at offset 0x84 = 132
                        triggerPass();
                        return;
                    }
                    // Passing search failed - will use alternating turns
                    passingFailed = true;
                }
            }
        }
    }

    // cseg_84D10 (118418-118430): Check if we should start retry pass window
    // Only when passingFailed and field_84 == 0 and (frameCount & 0x7F) < 0x20
    if (passingFailed) {
        if (team->ofs134 == 0 && (swos.frameCount & 0x7F) < 0x20) {
            team->ofs134 = 4;  // Start 4-frame retry window
            // Fall through to alternating turns (cseg_84D57)
        } else {
            // Go to normal dribbling (cseg_84DD3) instead of alternating turns
            passingFailed = false;
        }
    }

alternatingTurns:
    // Dribbling logic - two paths based on whether passing failed
    if (team->plVeryCloseToBall) {
        if (passingFailed) {
            // Assembly at cseg_84D57 (118432-118460): Alternating turns when passing failed
            // Creates a "jinking" motion to evade defenders
            int16_t newDir;
            if (swos.currentGameTick & 0x80) {
                // Turn RIGHT (direction + 1)
                newDir = (playerDirection + 1) & 7;
            } else {
                // Turn LEFT (direction - 1)
                newDir = (playerDirection - 1) & 7;
            }
            team->currentAllowedDirection = newDir;
        } else {
            // Assembly at cseg_84DD3/cseg_84DE0 (118463-118479): Turn toward goal
            int16_t targetDir = angleToDirection(goalAngle);
            team->currentAllowedDirection = targetDir;
        }
    } else {
        // Not very close to ball - keep current direction (cseg_84E16)
        team->currentAllowedDirection = playerDirection;
    }
}

// Handle no ball nearby - positioning
// Assembly at @@noone_near (118895-118980)
void handleNoBallNearby() {
    // Decide whether to try headers/volleys
    if (!AI_DecideWhetherToTriggerFire())
        return;  // Fire was triggered

    // Check if pass target exists and is further from ball than controlled player
    // Assembly at 118899-118922
    if (passToPlayer) {
        int32_t theirDist = passToPlayer->ballDistance;
        int32_t ourDist = controlledPlayer ? controlledPlayer->ballDistance : 0;
        int32_t diff = theirDist - ourDist;

        // Assembly: if D0 >= 50, swap players and return WITHOUT setting direction
        // ASM does `jmp return` at line 118922, leaving direction at -1
        if (diff >= 50) {
            // Swap controlling and pass-to player
            Sprite* temp = team->controlledPlayerSprite;
            team->controlledPlayerSprite = team->passToPlayerPtr;
            team->passToPlayerPtr = temp;
            // ASM does NOT set direction here - it stays at the initial -1
            return;
        }
    }

    // Assembly at @@pass_to_player_too_far_or_null (118925-118945)
    // Check if there's a human player in the game
    bool humanPlaying = (swos.topTeamData.playerNumber != 0 ||
                         swos.bottomTeamData.playerNumber != 0);

    if (!passToPlayer || humanPlaying) {
        // Use @@decide_if_flipping_direction logic (118491-118524)
        // Calculate opposite direction
        int16_t fullDir = controlledPlayer ? controlledPlayer->fullDirection : 0;
        int16_t oppositeDir = ((fullDir + 128 + 16) & 0xFF) >> 5;

        // Check if rounded opposite equals current direction
        if (oppositeDir == playerDirection) {
            team->currentAllowedDirection = oppositeDir;
            return;
        }

        // Check if close to ball (< 800 = ~28 pixels squared distance)
        if (controlledPlayer && controlledPlayer->ballDistance < 800) {
            team->currentAllowedDirection = oppositeDir;
            return;
        }

        // 1/8 chance to flip direction (using 0x0E mask)
        if ((swos.currentGameTick & 0x0E) == 0) {
            team->currentAllowedDirection = oppositeDir;
            return;
        }

        // Otherwise keep current direction
        team->currentAllowedDirection = playerDirection;
        return;
    }

    // CPU vs CPU: use @@randomly_flip_or_continue_direction logic (118956-118980)
    // 3/4 chance to use current direction (when any bit in 0x18 is set)
    if ((swos.currentGameTick & 0x18) != 0) {
        team->currentAllowedDirection = playerDirection;
        return;
    }

    // 1/4 chance: apply random rotation from table, then flip direction
    // Assembly uses AI_randomRotateTable with index (AI_rand & 2), giving 0 or 2
    int16_t rotateIndex = AI_rand & 2;  // 0 or 2 (word index)
    int16_t rotation = AI_randomRotateTable[rotateIndex / 2];  // Convert to array index

    int16_t fullDir = controlledPlayer ? controlledPlayer->fullDirection : 0;
    int16_t newDir = fullDir + rotation;  // Apply random rotation
    newDir = ((newDir + 128 + 16) & 0xFF) >> 5;  // Flip and round to 0-7
    team->currentAllowedDirection = newDir;
}

// Ball after-touch when kicking/passing
// Assembly at 119011-119084
void handleBallAfterTouch() {
    int16_t ctrlDir = team->allowedPlDirection;  // Assembly uses controlledPlDirection at offset 56

    // Check if penalties - go to no-after-touch path
    if (swos.playingPenalties || swos.penalty) {
        goto noAfterTouch;
    }

    // Random chance - 50% chance to skip spin and go to no-after-touch path
    if ((AI_rand & 1) == 0) {
        goto noAfterTouch;
    }

    // Apply spin based on spinDir (AI_ballSpinDirection)
    {
        int16_t spinDir = team->ofs138;
        int16_t* table;

        if (spinDir < 0) {
            // Left spin - use AI_leftSpinTable
            table = AI_leftSpinTable;
        } else if (spinDir > 0) {
            // Right spin - use AI_rotateRightTable
            table = AI_rotateRightTable;
        } else {
            // spinDir == 0, fall through to noAfterTouch
            goto noAfterTouch;
        }

        // Apply table adjustment: newDir = (ctrlDir + table[afterTouchStrength]) & 7
        // Note: Assembly's (ctrlDir -/+ 1) calculations are dead code - D0 gets overwritten
        int16_t adjustment = table[team->ofs136];
        int16_t newDir = (ctrlDir + adjustment) & 7;
        team->currentAllowedDirection = newDir;
        return;
    }

noAfterTouch:
    // Assembly @@no_ball_after_touch at 119032-119050
    // If afterTouchStrength == 1 (medium), set direction to -1 and return
    if (team->ofs136 == 1) {
        team->currentAllowedDirection = -1;
        return;
    }

    // Otherwise, do long kick - apply AI_longKickTable adjustment
    {
        int16_t adjustment = AI_longKickTable[team->ofs136];
        int16_t newDir = (ctrlDir + adjustment) & 7;
        team->currentAllowedDirection = newDir;
    }
}

} // anonymous namespace

namespace AI {

void SetControlsDirection() {
    // Get team pointer from A6 register
    team = reinterpret_cast<TeamGeneralInfo*>(offsetToPtr(A6));

    // Early exit if controls reset
    if (team->resetControls)
        return;

    // Decrement timers
    if (AI_counter > 0)
        AI_counter--;
    if (AI_resumePlayTimer > 0)
        AI_resumePlayTimer--;

    // Generate random number
    SWOS::Rand();
    AI_rand = static_cast<uint16_t>(D0.asWord());

    // Increment AI timer
    team->AITimer++;

    // Reset control variables
    team->currentAllowedDirection = -1;
    team->firePressed = 0;
    team->fireThisFrame = 0;
    team->quickFire = 0;
    team->normalFire = 0;

    // Don't process if in substitutes menu
    if (swos.g_inSubstitutesMenu)
        return;

    // Set up local state
    playerDirection = -1;
    controlledPlayer = team->controlledPlayerSprite;
    passToPlayer = team->passToPlayerPtr;

    if (controlledPlayer) {
        playerDirection = controlledPlayer->direction;
    }

    // Calculate distance and angle to opponent's goal
    int16_t goalLineY = (team == &swos.bottomTeamData) ? 129 : 769;
    int16_t pitchCenterX = 336;
    int16_t ballX = swos.ballSprite.x.whole();
    int16_t ballY = swos.ballSprite.y.whole();

    int32_t dx = ballX - pitchCenterX;
    int32_t dy = ballY - goalLineY;
    goalDistanceSqr = dx * dx + dy * dy;

    // Calculate angle to goal using assembly function
    D3 = ballX;
    D4 = ballY;
    D1 = pitchCenterX;
    D2 = goalLineY;
    D0 = 256;
    D5 = 0;
    SWOS::CalculateDeltaXAndY();
    goalAngle = (D0.asInt16() < 0) ? 0 : D0.asWord();

    // Main game state handling
    int16_t gameStatePl = static_cast<int16_t>(swos.gameStatePl);

    if (gameStatePl != ST_GAME_IN_PROGRESS) {
        // Game not in progress - only process the team that was playing before the break
        // ASM: cmp A6, lastTeamPlayedBeforeBreak; jnz return
        if (static_cast<TeamGeneralInfo*>(swos.lastTeamPlayedBeforeBreak) != team)
            return;

        // Handle end-of-game CPU auto-skip
        int16_t gs = static_cast<int16_t>(swos.gameState);
        if (gs >= ST_STARTING_GAME && gs <= ST_GAME_ENDED) {
            if (swos.team1Computer && swos.team2Computer) {
                int16_t threshold = (gs == ST_RESULT_ON_HALFTIME ||
                                     gs == ST_RESULT_AFTER_THE_GAME) ? 660 : 385;
                if (swos.stoppageTimerTotal >= threshold) {
                    team->firePressed = 1;
                    return;
                }
                return;
            }
        }

        // Handle stopped game states
        handleStoppedState();
        return;
    }

    // Game in progress

    // Check for penalties
    if (swos.playingPenalties || swos.penalty) {
        if (static_cast<int16_t>(team->spinTimer) >= 0) {
            // Ball after-touch allowed
            handleBallAfterTouch();
            return;
        }
    }

    // AI counter active - set direction toward goal
    if (AI_counter > 0) {
        AI_SetDirectionTowardOpponentsGoal();
    }

    // Need controlled player for the rest
    if (!controlledPlayer)
        return;

    // Check if ball after-touch is active (spinTimer is signed in ASM)
    if (static_cast<int16_t>(team->spinTimer) >= 0) {
        handleBallAfterTouch();
        return;
    }

    // Check ball proximity
    if (team->plVeryCloseToBall || team->plCloseToBall) {
        // Player near ball
        if (!AI_DecideWhetherToTriggerFire()) {
            return;  // Fire was triggered
        }

        // Additional shooting/passing checks
        handleBallControl();
        return;
    }

    // Ball not nearby
    handleNoBallNearby();
}

// Structure to save AI global state for comparison
struct AIGlobalState {
    int16_t counter;
    int16_t resumePlayTimer;
    uint16_t randVal;
    int16_t attackHalf;
    int16_t counterWriteOnly;
    int16_t turnDirection;
};

static AIGlobalState saveAIGlobalState()
{
    AIGlobalState state;
    state.counter = AI_counter;
    state.resumePlayTimer = AI_resumePlayTimer;
    state.randVal = AI_rand;
    state.attackHalf = AI_attackHalf;
    state.counterWriteOnly = AI_counterWriteOnly;
    state.turnDirection = swos.AI_turnDirection;
    return state;
}

static void restoreAIGlobalState(const AIGlobalState& state)
{
    AI_counter = state.counter;
    AI_resumePlayTimer = state.resumePlayTimer;
    AI_rand = state.randVal;
    AI_attackHalf = state.attackHalf;
    AI_counterWriteOnly = state.counterWriteOnly;
    swos.AI_turnDirection = state.turnDirection;
}

// Save TeamGeneralInfo fields that get modified by AI
struct TeamAIState {
    int16_t currentAllowedDirection;
    byte quickFire;
    byte normalFire;
    byte firePressed;
    byte fireThisFrame;
    int16_t AITimer;
    int16_t ofs134;
    int16_t ofs136;
    int16_t ofs138;
    // These are swapped in handleNoBallNearby and must be saved/restored
    SwosDataPointer<Sprite> controlledPlayerSprite;
    SwosDataPointer<Sprite> passToPlayerPtr;
};

static TeamAIState saveTeamAIState(TeamGeneralInfo* team)
{
    TeamAIState state;
    state.currentAllowedDirection = team->currentAllowedDirection;
    state.quickFire = team->quickFire;
    state.normalFire = team->normalFire;
    state.firePressed = team->firePressed;
    state.fireThisFrame = team->fireThisFrame;
    state.AITimer = team->AITimer;
    state.ofs134 = team->ofs134;
    state.ofs136 = team->ofs136;
    state.ofs138 = team->ofs138;
    state.controlledPlayerSprite = team->controlledPlayerSprite;
    state.passToPlayerPtr = team->passToPlayerPtr;
    return state;
}

static void restoreTeamAIState(TeamGeneralInfo* team, const TeamAIState& state)
{
    team->currentAllowedDirection = state.currentAllowedDirection;
    team->quickFire = state.quickFire;
    team->normalFire = state.normalFire;
    team->firePressed = state.firePressed;
    team->fireThisFrame = state.fireThisFrame;
    team->AITimer = state.AITimer;
    team->ofs134 = state.ofs134;
    team->ofs136 = state.ofs136;
    team->ofs138 = state.ofs138;
    team->controlledPlayerSprite = state.controlledPlayerSprite;
    team->passToPlayerPtr = state.passToPlayerPtr;
}

void CallWithComparison(bool useCpp)
{
    TeamGeneralInfo* teamPtr = reinterpret_cast<TeamGeneralInfo*>(offsetToPtr(A6));

    // During non-gameplay states (halftime, etc.), always use ASM to avoid crashes
    // The C++ code has issues with these transition states
    if (static_cast<int16_t>(swos.gameStatePl) != ST_GAME_IN_PROGRESS) {
        AI_SetControlsDirection();
        return;
    }

    // Skip comparison if not enabled
    if (!g_comparePortedFunctions) {
        if (useCpp) {
            SetControlsDirection();
        } else {
            AI_SetControlsDirection();
        }
        return;
    }

    // Comparison mode - run both versions from the same initial state

    // Save original state BEFORE running either version
    auto globalStateOriginal = saveAIGlobalState();
    auto teamStateOriginal = saveTeamAIState(teamPtr);
    auto rngStateOriginal = SWOS::saveRngState();
    Register savedA0 = A0, savedA4 = A4, savedA5 = A5;
    Register savedD0 = D0, savedD1 = D1, savedD2 = D2, savedD3 = D3;
    Register savedD4 = D4, savedD5 = D5, savedD6 = D6, savedD7 = D7;

    // Run C++ version first
    SetControlsDirection();

    // Capture C++ output
    AICompareState cppOutput = captureAIState(teamPtr);

    // Save C++ final state (for restore if useCpp)
    auto globalStateCpp = saveAIGlobalState();
    auto teamStateCpp = saveTeamAIState(teamPtr);
    auto rngStateCpp = SWOS::saveRngState();
    Register cppA0 = A0, cppA4 = A4, cppA5 = A5;
    Register cppD0 = D0, cppD1 = D1, cppD2 = D2, cppD3 = D3;
    Register cppD4 = D4, cppD5 = D5, cppD6 = D6, cppD7 = D7;

    // Capture input state for diagnostic logging (values that don't change during AI execution)
    // AI_rand was just set by C++ and will be same for ASM after restore (same RNG state)
    AICompareInputState inputState;
    inputState.spinTimer = static_cast<int16_t>(teamPtr->spinTimer);
    inputState.plVeryCloseToBall = teamPtr->plVeryCloseToBall;
    inputState.plCloseToBall = teamPtr->plCloseToBall;
    inputState.aiRandValue = AI_rand;  // This is what both versions use
    inputState.hasControlledPlayer = !teamPtr->controlledPlayerSprite.isNull();
    if (inputState.hasControlledPlayer) {
        Sprite* ctrl = teamPtr->controlledPlayerSprite.asPtr();
        inputState.playerDirection = ctrl ? ctrl->direction : -1;
    } else {
        inputState.playerDirection = -1;
    }
    // Capture initial values for handleBallAfterTouch path analysis
    // These are captured from the ORIGINAL state (restored before ASM runs)
    inputState.ofs136_initial = teamStateOriginal.ofs136;
    inputState.ofs138_initial = teamStateOriginal.ofs138;
    inputState.isPenalties = (swos.playingPenalties || swos.penalty);

    // Additional diagnostics for fireThisFrame and shooting paths
    inputState.ballZ = swos.ballSprite.z.whole();
    inputState.ballDeltaZ = swos.ballSprite.deltaZ.raw();  // Raw 16.16 fixed-point value
    inputState.ctrlBallDist = 0;
    if (inputState.hasControlledPlayer) {
        Sprite* ctrl = teamPtr->controlledPlayerSprite.asPtr();
        if (ctrl) inputState.ctrlBallDist = ctrl->ballDistance;
    }
    // Calculate goal distance squared (same formula as SetControlsDirection)
    int16_t goalLineY = (teamPtr == &swos.bottomTeamData) ? 129 : 769;
    int16_t ballX = swos.ballSprite.x.whole();
    int16_t ballY = swos.ballSprite.y.whole();
    int32_t dx = ballX - 336;  // pitch center X
    int32_t dy = ballY - goalLineY;
    inputState.goalDistSqr = dx * dx + dy * dy;
    inputState.ofs134_initial = teamStateOriginal.ofs134;

    // Opponent info
    inputState.oppCtrlDist = 0;
    inputState.oppPassDist = 0;
    TeamGeneralInfo* opponents = teamPtr->opponentTeam;
    if (opponents) {
        Sprite* oppCtrl = opponents->controlledPlayerSprite.asPtr();
        Sprite* oppPass = opponents->passToPlayerPtr.asPtr();
        if (oppCtrl) inputState.oppCtrlDist = oppCtrl->ballDistance;
        if (oppPass) inputState.oppPassDist = oppPass->ballDistance;
    }

    // Restore to original state
    restoreAIGlobalState(globalStateOriginal);
    restoreTeamAIState(teamPtr, teamStateOriginal);
    SWOS::restoreRngState(rngStateOriginal);
    A0 = savedA0; A4 = savedA4; A5 = savedA5;
    D0 = savedD0; D1 = savedD1; D2 = savedD2; D3 = savedD3;
    D4 = savedD4; D5 = savedD5; D6 = savedD6; D7 = savedD7;

    // Run ASM version from the same initial state
    AI_SetControlsDirection();

    // Capture ASM output
    AICompareState asmOutput = captureAIState(teamPtr);

    // Restore state to what the selected version (C++ or ASM) would produce
    if (useCpp) {
        // Restore to C++ state
        restoreAIGlobalState(globalStateCpp);
        restoreTeamAIState(teamPtr, teamStateCpp);
        SWOS::restoreRngState(rngStateCpp);
        A0 = cppA0; A4 = cppA4; A5 = cppA5;
        D0 = cppD0; D1 = cppD1; D2 = cppD2; D3 = cppD3;
        D4 = cppD4; D5 = cppD5; D6 = cppD6; D7 = cppD7;
    }
    // If using ASM, the state is already from ASM run

    // Log mismatch if C++ differs from ASM
    if (asmOutput != cppOutput) {
        logAICompareMismatch(swos.frameCount, teamPtr, asmOutput, cppOutput, &inputState);
    }
}

} // namespace AI
