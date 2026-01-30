#pragma once

#include <cstdint>

/**
 * ============================================================================
 * MEMORY OFFSETS AND CONSTANTS FOR PLAYER FUNCTIONS
 * ============================================================================
 *
 * This file defines memory offsets and constants used by playerFunctions.cpp.
 * These values represent absolute addresses into g_memByte (the SWOS memory
 * space) and field offsets within game structures.
 *
 * ----------------------------------------------------------------------------
 * MEMORY ACCESS NOTES
 * ----------------------------------------------------------------------------
 *
 * All constants prefixed with 'k' followed by an address value (e.g., 524764)
 * are ABSOLUTE addresses into g_memByte, NOT offsets from kMemStart.
 * Therefore we access g_memByte directly, not via kMemStart (which adds 256).
 *
 * Field offsets (e.g., kSpriteX = 30) are byte offsets within their respective
 * structs and should be added to a base pointer.
 *
 * ----------------------------------------------------------------------------
 * NAMESPACE ORGANIZATION
 * ----------------------------------------------------------------------------
 *
 *   SwosMemory::Sprite::         - Sprite struct field offsets
 *   SwosMemory::Team::           - TeamGeneralInfo struct field offsets
 *   SwosMemory::PlayerGame::     - PlayerGame struct field offsets
 *   SwosMemory::GameState::      - Game state variables and constants
 *   SwosMemory::PlayerState::    - Player state constants
 *   SwosMemory::Goalkeeper::     - Goalkeeper speeds, distances, animations
 *   SwosMemory::Heading::        - Heading animations, speeds, heights
 *   SwosMemory::Tackling::       - Tackling constants and animations
 *   SwosMemory::Ball::           - Ball physics and positioning
 *   SwosMemory::Passing::        - Pass speeds and skill tables
 *   SwosMemory::Player::         - Player speed tables and discipline
 *   SwosMemory::Tables::         - General lookup tables
 *   SwosMemory::DeadVars::       - Legacy unused variables (kept for reference)
 *
 * ============================================================================
 */

namespace SwosMemory {

// ============================================================================
// SPRITE STRUCT FIELD OFFSETS
// ============================================================================
// Byte offsets within the Sprite structure. Add these to a sprite pointer
// to access the corresponding field.

namespace Sprite {
namespace Offsets {
    // Position fields (FixedPoint - 4 bytes each)
    constexpr int kX = 30;                    // FixedPoint x position
    constexpr int kY = 34;                    // FixedPoint y position
    constexpr int kZ = 38;                    // FixedPoint z position (height)

    // Movement fields
    constexpr int kDirection = 42;            // Direction (0-7), 2 bytes
    constexpr int kSpeed = 44;                // Movement speed, 2 bytes
    constexpr int kDeltaX = 46;               // FixedPoint deltaX (velocity), 4 bytes
    constexpr int kDeltaY = 50;               // FixedPoint deltaY (velocity), 4 bytes
    constexpr int kDeltaZ = 54;               // FixedPoint deltaZ (vertical velocity), 4 bytes

    // Destination fields
    constexpr int kDestX = 58;                // Destination X coordinate, 2 bytes
    constexpr int kDestY = 60;                // Destination Y coordinate, 2 bytes

    // State fields
    constexpr int kFrameSwitchCounter = 22;   // Animation frame counter, 2 bytes
    constexpr int kBallDistance = 74;         // Distance to ball, 4 bytes
    constexpr int kFullDirection = 82;        // Full direction (0-31), 1 byte
    constexpr int kTackleState = 96;          // Tackling state, 2 bytes
    constexpr int kHeading = 98;              // Heading state, 2 bytes
    constexpr int kTacklingTimer = 106;       // Tackling timer, 2 bytes
    constexpr int kSentAway = 4;              // Sent off flag, offset 4
}  // namespace Offsets
}  // namespace Sprite

// ============================================================================
// TEAM STRUCT FIELD OFFSETS (TeamGeneralInfo)
// ============================================================================
// Byte offsets within the TeamGeneralInfo structure.

namespace Team {
namespace Offsets {
    constexpr int kOpponentsTeam = 0;             // Pointer to opponent's team
    constexpr int kPlayerNumber = 4;              // Human player number (0 = CPU)
    constexpr int kInGameTeamPtr = 10;            // Pointer to in-game team data
    constexpr int kTeamStatsPtr = 14;             // Pointer to TeamStatisticsData
    constexpr int kSpritesTable = 20;             // Pointer to team's sprites table
    constexpr int kControlledPlayerSprite = 32;   // Currently controlled player sprite
    constexpr int kPassToPlayerPtr = 36;          // Player to pass to
    constexpr int kPlayerHasBall = 40;            // Flag: does controlled player have ball
    constexpr int kCurrentAllowedDirection = 44;  // Allowed direction for actions
    constexpr int kControlledPlDirection = 56;    // Controlled player's direction
    constexpr int kGoalkeeperSavedCommentTimer = 76;  // Timer for save commentary
    constexpr int kPassingBall = 88;              // Flag: is pass in progress
    constexpr int kPassingToPlayer = 90;          // Flag: passing to specific player
    constexpr int kShotChanceTable = 90;          // Shot chance lookup table
    constexpr int kBallControlTurnTimer = 108;    // Ball control turn timer
    constexpr int kSpinTimer = 118;               // Spin move timer
    constexpr int kLeftSpin = 120;                // Left spin flag
    constexpr int kRightSpin = 122;               // Right spin flag
    constexpr int kLongPass = 124;                // Long pass flag
    constexpr int kLongSpinPass = 126;            // Long spin pass flag
    constexpr int kPassInProgress = 128;          // Pass in progress flag
    constexpr int kWonTheBallTimer = 138;         // Timer after winning ball
}  // namespace Offsets

namespace Addresses {
    constexpr uint32_t kTopTeamData = 522804;     // Top team TeamGeneralInfo address
    constexpr uint32_t kBottomTeamData = 522952;  // Bottom team TeamGeneralInfo address
}  // namespace Addresses
}  // namespace Team

// ============================================================================
// PLAYER GAME STRUCT FIELD OFFSETS (PlayerGameHeader)
// ============================================================================
// Byte offsets within the PlayerGameHeader structure.

namespace PlayerGame {
namespace Offsets {
    constexpr int kPassing = 69;          // Passing skill (0-7)
    constexpr int kShooting = 70;         // Shooting skill (0-7)
    constexpr int kHeading = 71;          // Heading skill (0-7)
    constexpr int kTackling = 72;         // Tackling skill (0-7)
    constexpr int kBallControl = 73;      // Ball control skill (0-7)
    constexpr int kSpeed = 74;            // Speed skill (0-7)
    constexpr int kFinishing = 75;        // Finishing skill (0-7)
}  // namespace Offsets

constexpr int16_t kStructSize = 61;       // Size of player struct in team data
}  // namespace PlayerGame

// ============================================================================
// TEAM STATISTICS STRUCT FIELD OFFSETS
// ============================================================================

namespace TeamStats {
namespace Offsets {
    constexpr int kFoulsConceded = 8;     // Fouls conceded count
}  // namespace Offsets
}  // namespace TeamStats

// ============================================================================
// GAME STATE VARIABLES AND CONSTANTS
// ============================================================================

namespace GameState {
namespace Addresses {
    constexpr uint32_t kGameStatePl = 523128;       // Player-level game state
    constexpr uint32_t kGameState = 523130;         // Main game state
    constexpr uint32_t kStoppageTimerTotal = 523124;  // Stoppage time counter
    constexpr uint32_t kRunSlower = 457534;         // Flag: players run slower
    constexpr uint32_t kCurrentGameTick = 323904;   // Current game tick counter
    constexpr uint32_t kCurrentTick = 323902;       // Alternative tick counter
    constexpr uint32_t kStateGoal = 544107;         // Goal scored state
    constexpr uint32_t kPlayingPenalties = 523160;  // Penalty shootout flag
    constexpr uint32_t kPenalty = 523112;           // Penalty kick in progress
    constexpr uint32_t kCardsDisallowed = 455934;   // Flag: cards not allowed
    constexpr uint32_t kTrainingGame = 544108;      // Training mode flag
}  // namespace Addresses

// Game state constants (values for kGameStatePl and kGameState)
namespace Values {
    constexpr int16_t kPlayersToInitial = 0;        // Players moving to initial positions
    constexpr int16_t kGameInProgress = 100;        // Game is being played
    constexpr int16_t kFirstHalfEnded = 29;         // First half has ended
    constexpr int16_t kGameEnded = 30;              // Match has ended
    constexpr int16_t kGoingToHalftime = 23;        // Players going to halftime
    constexpr int16_t kPlayersGoingToShower = 24;   // Players leaving pitch
    constexpr int16_t kKeeperHoldsBall = 3;         // Goalkeeper is holding ball
    constexpr int16_t kThrowInForwardRight = 7;     // Throw-in direction
    constexpr int16_t kThrowInBackLeft = 10;        // Throw-in direction
}  // namespace Values
}  // namespace GameState

// ============================================================================
// PLAYER STATE CONSTANTS
// ============================================================================
// Values for Sprite.playerState field at offset 12.

namespace PlayerState {
    constexpr int8_t kNormal = 0;                   // Normal movement
    constexpr int8_t kTackling = 1;                 // Performing slide tackle
    constexpr int8_t kGoalieCatchingBall = 4;       // Goalkeeper catching ball
    constexpr int8_t kGoalieDivingHigh = 6;         // Goalkeeper diving high
    constexpr int8_t kGoalieDivingLow = 7;          // Goalkeeper diving low
    constexpr int8_t kStaticHeading = 8;            // Performing static header
    constexpr int8_t kJumpHeading = 9;              // Performing jump header
}  // namespace PlayerState

// ============================================================================
// TACKLE STATE CONSTANTS
// ============================================================================
// Values for Sprite.tackleState field.

namespace TackleState {
    constexpr int16_t kTacklingTheBall = 1;         // Currently tackling ball
    constexpr int16_t kGoodTackle = 2;              // Clean tackle completed
}  // namespace TackleState

// ============================================================================
// GOALKEEPER CONSTANTS
// ============================================================================

namespace Goalkeeper {
namespace Speeds {
    constexpr uint32_t kCatchSpeed = 324032;        // Speed when catching ball
    constexpr uint32_t kNearJumpSpeed = 524010;     // Speed for close dive
    constexpr uint32_t kFarJumpSpeed = 324036;      // Speed for far dive
    constexpr uint32_t kFarJumpSlowerSpeed = 324038;  // Slower far dive speed
}  // namespace Speeds

namespace SaveDistances {
    constexpr int16_t kNormalSave = 16;             // Normal shot save threshold
    constexpr int16_t kPenaltySaveFar = 20;         // Penalty save (25% chance)
    constexpr int16_t kPenaltySaveNear = 12;        // Penalty save (75% chance)
}  // namespace SaveDistances

namespace AnimationTables {
    constexpr uint32_t kCatchingBall = 453658;      // Catching ball animation
    constexpr uint32_t kLeftJumpingHigh = 454308;   // Left side dive high
    constexpr uint32_t kRightJumpingHigh = 454568;  // Right side dive high
    constexpr uint32_t kLeftJumpingLow = 454698;    // Left side dive low
    constexpr uint32_t kRightJumpingLow = 454828;   // Right side dive low
    constexpr uint32_t kDiveDeltas = 324072;        // Dive speed by skill table
}  // namespace AnimationTables

namespace Addresses {
    constexpr uint32_t kDiveDeadVar = 337190;       // Dead var (unused)
}  // namespace Addresses
}  // namespace Goalkeeper

// ============================================================================
// HEADING CONSTANTS
// ============================================================================

namespace Heading {
namespace AnimationTables {
    constexpr uint32_t kStaticAttempt = 452194;     // Static header attempt animation
    constexpr uint32_t kStaticHit = 452454;         // Static header hit animation
    constexpr uint32_t kJumpAttempt = 452584;       // Jump header attempt animation
    constexpr uint32_t kJumpHit = 452714;           // Jump header hit animation
}  // namespace AnimationTables

namespace Speeds {
    constexpr uint32_t kStaticHeaderPlayer = 524736;  // Player speed during static header
    constexpr uint32_t kJumpHeader = 325744;          // Player speed during jump header
    constexpr int16_t kStaticHeaderBall = 1792;       // Ball speed after static header
}  // namespace Speeds

namespace Heights {
    constexpr int32_t kBallJumpHeaderDeltaZ = 0xA000;   // Initial ball deltaZ for jump header
    constexpr int32_t kLowJumpHeight = 0x20000;         // 2.0 in fixed point
    constexpr int32_t kHighJumpHeight = 0x24000;        // 2.25 in fixed point
}  // namespace Heights
}  // namespace Heading

// ============================================================================
// TACKLING CONSTANTS
// ============================================================================

namespace Tackling {
namespace DownTimeTables {
    constexpr uint32_t kPlayerDownTime = 524012;      // Human player downtime table
    constexpr uint32_t kComputerDownTime = 524028;    // CPU player downtime table
}  // namespace DownTimeTables

namespace Addresses {
    constexpr uint32_t kAnimTable = 453364;           // Tackling animation table
    constexpr uint32_t kDownInterval = 540868;        // Base down interval
    constexpr uint32_t kSpeed = 325742;               // Tackling speed
}  // namespace Addresses
}  // namespace Tackling

// ============================================================================
// BALL CONSTANTS
// ============================================================================

namespace Ball {
namespace Physics {
    // High kick (lob/chip)
    constexpr int32_t kHighKickDeltaZ = 0x20000;      // 2.0 in FixedPoint
    constexpr int16_t kHighKickSpeed = 2688;          // Ball speed (10.5)

    // Normal kick
    constexpr int32_t kNormalKickDeltaZ = 0x16000;    // 1.375 in FixedPoint
    constexpr int16_t kNormalKickSpeed = 2560;        // Ball speed (10.0)

    // Kicking
    constexpr int16_t kKickingSpeed = 2208;           // Standard kick speed
    constexpr int32_t kKickingDeltaZ = 0x14000;       // 1.25 in fixed point

    // Gravity
    constexpr uint32_t kGravityConstant = 325648;     // Gravity constant address
}  // namespace Physics

namespace Positioning {
    // Predicted ball landing positions
    constexpr uint32_t kNextGroundX = 524764;         // Predicted X landing position
    constexpr uint32_t kNextGroundY = 524766;         // Predicted Y landing position
    constexpr uint32_t kNextZDeadVar = 524768;        // Dead var (unused)

    // Ball defensive AI positioning
    constexpr uint32_t kDefensiveX = 524746;          // Defensive target X
    constexpr uint32_t kDefensiveY = 524748;          // Defensive target Y
    constexpr uint32_t kDefensiveZ = 524750;          // Defensive target Z

    // Ball "not high" position (for ground-level plays)
    constexpr uint32_t kNotHighX = 524752;            // Ground X position
    constexpr uint32_t kNotHighY = 524754;            // Ground Y position
    constexpr uint32_t kNotHighZ = 524756;            // Ground Z position

    // Strike destination
    constexpr uint32_t kStrikeDestX = 524758;         // Strike target X
    constexpr uint32_t kStrikeDestY = 524760;         // Strike target Y
    constexpr uint32_t kStrikeDestZ = 524762;         // Strike target Z
}  // namespace Positioning

namespace Addresses {
    constexpr uint32_t kSpriteOffset = 328988;        // Ball sprite address in memory
}  // namespace Addresses
}  // namespace Ball

// ============================================================================
// PASSING CONSTANTS
// ============================================================================

namespace Passing {
// Distance-based pass speeds (squared distance thresholds)
namespace Speeds {
    constexpr int16_t kCloserThan2500 = 1536;         // Distance < 50
    constexpr int16_t k2500To10000 = 1664;            // Distance 50-100
    constexpr int16_t k10000To22500 = 1792;           // Distance 100-150
    constexpr int16_t k22500To40000 = 1877;           // Distance 150-200
    constexpr int16_t k40000To62500 = 1962;           // Distance 200-250
    constexpr int16_t k62500To90000 = 2048;           // Distance 250-300
    constexpr int16_t k90000To122500 = 2133;          // Distance 300-350
    constexpr int16_t kFurtherThan122500 = 2218;      // Distance > 350
    constexpr int16_t kFreePassReleasingBall = 1792;  // Free pass release speed
}  // namespace Speeds

namespace Tables {
    constexpr uint32_t kAIFailedPassChance = 523884;  // AI pass failure by skill
    constexpr uint32_t kSpeedIncrease = 523818;       // Speed increase by skill
    constexpr uint32_t kGoodPassSampleCommand = 523670;  // Good pass sound trigger
}  // namespace Tables
}  // namespace Passing

// ============================================================================
// PLAYER CONSTANTS
// ============================================================================

namespace Player {
namespace SpeedTables {
    constexpr uint32_t kGameInProgress = 524044;      // Speeds during play
    constexpr uint32_t kGameStopped = 524060;         // Speeds when stopped
    constexpr uint32_t kInjuriesHandicap = 524196;    // Speed reduction for injuries
}  // namespace SpeedTables

namespace Discipline {
    constexpr uint32_t kCardChance = 525142;          // Card chance table
}  // namespace Discipline
}  // namespace Player

// ============================================================================
// GENERAL LOOKUP TABLES
// ============================================================================

namespace Tables {
    constexpr uint32_t kDefaultDestinations = 523306;    // Direction->offset table
    constexpr uint32_t kInGameTeamPlayerOffsets = 332510;  // Player offset lookup
    constexpr uint32_t kPlayerWithBallOffsets = 524212;  // Ball carrier offsets
    constexpr uint32_t kBallPlOffsets = 523940;          // Ball positioning offsets

    // Ball control tables
    constexpr uint32_t kAvgTacklingBallControlDiffChance = 523932;  // Tackle vs control
    constexpr uint32_t kBallSpeedDeltaWhenControlled = 523916;      // Control speed delta
    constexpr uint32_t kBallControlTurnTimer = 523868;              // Turn timer lookup
}  // namespace Tables

// ============================================================================
// DEAD/LEGACY VARIABLES
// ============================================================================
// These variables appear unused in the original code but are kept for
// reference during reverse engineering.

namespace DeadVars {
    constexpr uint32_t kThrowInDeadVar = 455926;      // Throw-in dead variable
    constexpr uint32_t kDeadVarAlways0 = 449467;      // Always reads as 0
    constexpr uint32_t kDseg1309C1 = 449493;          // Unknown purpose
    constexpr uint32_t kDseg114EC2 = 544106;          // Stores last closest player
}  // namespace DeadVars

}  // namespace SwosMemory
