#pragma once

#include <cstdint>

/**
 * ============================================================================
 * COMPETITION DATA STRUCTURES FOR SWOS
 * ============================================================================
 *
 * This file defines the data structures for domestic leagues and cup
 * competitions. All values use named constants to be human-readable.
 *
 * ----------------------------------------------------------------------------
 * HOW TO READ LEAGUE DEFINITIONS
 * ----------------------------------------------------------------------------
 *
 * Example: Danish League
 *
 *   makeLeague(
 *       Id::Europe::Denmark,    // Which country/competition
 *       12,                     // 12 teams in the league
 *       July, May,              // Season runs July to May
 *       None,                   // No special flags
 *       1,                      // 1 division only
 *       4,                      // Teams play each other 4 times
 *       3,                      // 3 points for a win
 *       Subs3_Bench5,           // 3 substitutes allowed, 5 players on bench
 *       SingleDivision          // Single division league format
 *   )
 *
 * ----------------------------------------------------------------------------
 * HOW TO READ CUP DEFINITIONS
 * ----------------------------------------------------------------------------
 *
 * Example: English FA Cup
 *
 *   makeCup(
 *       Id::Cups::EnglishFACup, // Which cup competition
 *       64,                     // 64 teams enter
 *       December, MayNextYear,  // Runs December to May (spans year boundary)
 *       15,                     // 15 total rounds
 *       64,                     // Top teams enter at round of 64
 *       0,                      // 0 = single matches, 2 = two-legged ties
 *       Subs3_Bench3,           // 3 subs, 3 on bench
 *       3                       // 3 rounds have special format
 *   )
 *
 *   Round formats (how each round is played):
 *   - HomeWithReplay:  Single match at home team's ground, replay if drawn
 *   - TwoLegs:         Home and away matches (aggregate score wins)
 *   - NeutralVenue:    Single match at neutral ground (used for finals)
 *
 * ----------------------------------------------------------------------------
 * HOW TO READ PROMOTION/RELEGATION
 * ----------------------------------------------------------------------------
 *
 * Example: English Division One
 *
 *   makeDivision(
 *       2,      // 2 teams directly promoted (1st and 2nd place)
 *       4,      // 4 teams in promotion playoff (3rd-6th play for 1 more spot)
 *       0x2D,   // Promoted to competition 0x2D (Premier League)
 *       3,      // 3 teams directly relegated (bottom 3)
 *       0,      // No relegation playoff
 *       0x2F    // Relegated to competition 0x2F (Division Two)
 *   )
 *
 * ----------------------------------------------------------------------------
 * SUBSTITUTES AND BENCH ENCODING
 * ----------------------------------------------------------------------------
 *
 *   Subs3_Bench3 (0x33): 3 substitutions allowed, 3 players on the bench
 *   Subs3_Bench5 (0x35): 3 substitutions allowed, 5 players on the bench
 *
 *   Most leagues use Subs3_Bench5 (5-man bench).
 *   English cups historically used Subs3_Bench3 (3-man bench).
 *
 * ----------------------------------------------------------------------------
 * COMPETITION ID RANGES
 * ----------------------------------------------------------------------------
 *
 *   0x00 - 0x23: International competitions (World Cup, Euro, etc.)
 *   0x24 - 0x54: European domestic leagues
 *   0x55 - 0x6B: Rest of world leagues (Americas, Asia, Africa)
 *   0x7D - 0xB1: Domestic cup competitions
 *
 * ============================================================================
 */

#pragma pack(push, 1)

namespace Competition {

// ============================================================================
// SEASON MONTHS
// ============================================================================
// Months are encoded as (month_number * 8) in the original data.
// These constants make the data readable.

namespace Month {
    constexpr uint8_t January   = 1 * 8;   // 0x08
    constexpr uint8_t February  = 2 * 8;   // 0x10
    constexpr uint8_t March     = 3 * 8;   // 0x18
    constexpr uint8_t April     = 4 * 8;   // 0x20
    constexpr uint8_t May       = 5 * 8;   // 0x28
    constexpr uint8_t June      = 6 * 8;   // 0x30
    constexpr uint8_t July      = 7 * 8;   // 0x38
    constexpr uint8_t August    = 8 * 8;   // 0x40
    constexpr uint8_t September = 9 * 8;   // 0x48
    constexpr uint8_t October   = 10 * 8;  // 0x50
    constexpr uint8_t November  = 11 * 8;  // 0x58
    constexpr uint8_t December  = 12 * 8;  // 0x60

    // Special encoding for cups that span year boundary
    constexpr uint8_t MayNextYear = 16 * 8; // 0x80 - May of following year
}

// ============================================================================
// CUP MATCH FORMATS
// ============================================================================
// These define how each round of a cup competition is played.

namespace CupFormat {
    // Single match at home team's ground, replay if drawn
    constexpr uint8_t HomeWithReplay = 0x68;

    // Single match at neutral venue (used for finals)
    constexpr uint8_t NeutralVenue = 0x28;

    // Two-legged tie (home and away)
    constexpr uint8_t TwoLegs = 0x94;
}

// ============================================================================
// SUBSTITUTES AND RESERVES ENCODING
// ============================================================================
// This byte encodes both the number of substitutes allowed and reserve players.
// Format: High nibble = reserves info, Low nibble = subs info
// Common values:
//   0x33 = 3 subs allowed, 3 on bench (English cups style)
//   0x35 = 3 subs allowed, 5 on bench (Most leagues)

namespace SubsReserves {
    constexpr uint8_t Subs3_Bench3 = 0x33;  // 3 substitutes, 3 on bench
    constexpr uint8_t Subs3_Bench5 = 0x35;  // 3 substitutes, 5 on bench
}

// ============================================================================
// COMPETITION FLAGS
// ============================================================================
// Various flags that modify competition behavior.

namespace Flags {
    constexpr uint8_t None = 0x00;

    // League-specific flags (extraFlags field)
    constexpr uint8_t SingleDivision = 0x0C;    // Single division league
    constexpr uint8_t TwoDivisions = 0x12;      // Two division league
    constexpr uint8_t FourDivisions = 0x14;     // Four division league (England, Scotland)

    // Header flags
    constexpr uint8_t HasPlayoffs = 0x21;       // League has promotion playoffs
    constexpr uint8_t ItalianStyle = 0x15;      // Italian league format
}

// ============================================================================
// STRUCTURES
// ============================================================================

/**
 * League Competition Header (14 bytes)
 *
 * Contains the main settings for a domestic league competition.
 */
struct LeagueHeader {
    uint8_t compNumber;         // Unique ID for this competition (e.g., 0x2D = English leagues)
    uint8_t type;               // 0 = League, 1 = Cup
    uint8_t teamsPerDivision;   // Number of teams in each division (e.g., 20, 22)
    uint8_t startMonth;         // When season starts (use Month:: constants)
    uint8_t endMonth;           // When season ends (use Month:: constants)
    uint8_t flags;              // Competition flags (use Flags:: constants)
    uint8_t reserved1;          // Unused
    uint8_t reserved2;          // Unused
    uint8_t reserved3;          // Unused
    uint8_t divisionsCount;     // Number of divisions (1-4)
    uint8_t rounds;             // Times each team plays each other (usually 2 = home & away)
    uint8_t pointsForWin;       // Points awarded for a win (usually 3)
    uint8_t subsReserves;       // Substitutes/bench encoding (use SubsReserves:: constants)
    uint8_t extraFlags;         // Extra flags (use Flags:: constants)
};

static_assert(sizeof(LeagueHeader) == 14, "LeagueHeader must be 14 bytes");

/**
 * Division Configuration (6 bytes)
 *
 * Defines promotion and relegation rules for one division within a league.
 */
struct DivisionConfig {
    uint8_t promotions;         // Teams directly promoted (e.g., 2 = top 2 go up)
    uint8_t promotionPlayoff;   // Teams in promotion playoff (e.g., 4 = 3rd-6th playoff)
    uint8_t promotionTarget;    // Competition ID they're promoted to (0 = European/top)
    uint8_t relegations;        // Teams directly relegated (e.g., 3 = bottom 3 go down)
    uint8_t relegationPlayoff;  // Teams in relegation playoff
    uint8_t relegationTarget;   // Competition ID they're relegated to (0 = none)
};

static_assert(sizeof(DivisionConfig) == 6, "DivisionConfig must be 6 bytes");

/**
 * Cup Competition Header (14 bytes)
 *
 * Contains the main settings for a knockout cup competition.
 */
struct CupHeader {
    uint8_t compNumber;         // Unique ID for this competition (e.g., 0x86 = English FA Cup)
    uint8_t type;               // 0 = League, 1 = Cup
    uint8_t maxTeams;           // Maximum teams in competition (e.g., 64)
    uint8_t startMonth;         // When cup starts (use Month:: constants)
    uint8_t endMonth;           // When cup ends (use Month:: constants)
    uint8_t totalRounds;        // Total number of rounds in the cup
    uint8_t reserved1;          // Unused
    uint8_t reserved2;          // Unused
    uint8_t reserved3;          // Unused
    uint8_t reserved4;          // Unused
    uint8_t entryRound;         // When top division teams enter (e.g., 64 = round of 64)
    uint8_t twoLegged;          // 0 = single matches, 2 = two-legged ties
    uint8_t subsReserves;       // Substitutes/bench encoding (use SubsReserves:: constants)
    uint8_t roundsConfig;       // Number of rounds with special format
};

static_assert(sizeof(CupHeader) == 14, "CupHeader must be 14 bytes");

/**
 * Single Division League (20 bytes)
 *
 * Used for countries with just one top division (most countries).
 * Examples: Denmark, Belgium, Greece, etc.
 */
struct SingleDivisionLeague {
    LeagueHeader header;
    DivisionConfig division;
};

static_assert(sizeof(SingleDivisionLeague) == 20, "SingleDivisionLeague must be 20 bytes");

// ============================================================================
// COMPETITION IDs
// ============================================================================
// Unique identifiers for each competition in the game.

namespace Id {
    // === INTERNATIONAL COMPETITIONS ===
    namespace International {
        constexpr uint8_t EuropeanCup = 0x00;        // Champions League predecessor
        constexpr uint8_t CupWinnersCup = 0x01;     // UEFA Cup Winners' Cup
        constexpr uint8_t UefaCup = 0x02;           // UEFA Cup (now Europa League)
        constexpr uint8_t WorldCup = 0x1E;
        constexpr uint8_t AfricanNationsCup = 0x20;
        constexpr uint8_t GoldCup = 0x21;           // North American championship
        constexpr uint8_t AsianCup = 0x22;
        constexpr uint8_t OceaniaCup = 0x23;
    }

    // === EUROPEAN LEAGUES (0x24-0x54) ===
    namespace Europe {
        constexpr uint8_t Albania = 0x24;
        constexpr uint8_t Austria = 0x25;
        constexpr uint8_t Belarus = 0x26;
        constexpr uint8_t Belgium = 0x27;
        constexpr uint8_t Bulgaria = 0x28;
        constexpr uint8_t Croatia = 0x29;
        constexpr uint8_t Cyprus = 0x2A;
        constexpr uint8_t CzechRepublic = 0x2B;
        constexpr uint8_t Denmark = 0x2C;
        constexpr uint8_t England = 0x2D;
        // 0x2E, 0x2F, 0x30 = English lower divisions
        constexpr uint8_t Estonia = 0x31;
        constexpr uint8_t FaroeIslands = 0x32;
        constexpr uint8_t Finland = 0x33;
        constexpr uint8_t France = 0x34;
        constexpr uint8_t Germany = 0x35;
        constexpr uint8_t Greece = 0x36;
        constexpr uint8_t Netherlands = 0x37;
        // 0x38 = Dutch second division
        constexpr uint8_t Hungary = 0x39;
        constexpr uint8_t Iceland = 0x3A;
        constexpr uint8_t Israel = 0x3B;
        constexpr uint8_t Italy = 0x3C;
        constexpr uint8_t Latvia = 0x3D;
        constexpr uint8_t Lithuania = 0x3E;
        constexpr uint8_t Luxembourg = 0x3F;
        constexpr uint8_t Malta = 0x40;
        constexpr uint8_t NorthernIreland = 0x41;
        constexpr uint8_t Norway = 0x42;
        constexpr uint8_t Poland = 0x43;
        constexpr uint8_t Portugal = 0x44;
        constexpr uint8_t Ireland = 0x45;
        constexpr uint8_t Romania = 0x46;
        constexpr uint8_t Russia = 0x47;
        constexpr uint8_t SanMarino = 0x48;
        constexpr uint8_t Scotland = 0x49;
        // 0x4A, 0x4B, 0x4C = Scottish lower divisions (overlaps with Slovakia/Slovenia IDs)
        constexpr uint8_t Slovakia = 0x4B;
        constexpr uint8_t Slovenia = 0x4C;
        constexpr uint8_t Spain = 0x4D;
        // 0x4E = Spanish second division
        constexpr uint8_t Sweden = 0x4F;
        constexpr uint8_t Switzerland = 0x50;
        constexpr uint8_t Turkey = 0x51;
        constexpr uint8_t Ukraine = 0x52;
        constexpr uint8_t Wales = 0x53;
        constexpr uint8_t Yugoslavia = 0x54;
    }

    // === WORLD LEAGUES (0x55-0x6B) ===
    namespace World {
        constexpr uint8_t Algeria = 0x55;
        constexpr uint8_t Argentina = 0x56;
        constexpr uint8_t Australia = 0x57;
        constexpr uint8_t Bolivia = 0x58;
        constexpr uint8_t Brazil = 0x59;
        constexpr uint8_t Chile = 0x5A;
        constexpr uint8_t Colombia = 0x5B;
        constexpr uint8_t Ecuador = 0x5C;
        constexpr uint8_t ElSalvador = 0x5D;
        constexpr uint8_t India = 0x5E;
        constexpr uint8_t Japan = 0x5F;
        constexpr uint8_t Mexico = 0x60;
        constexpr uint8_t NewZealand = 0x61;
        constexpr uint8_t Paraguay = 0x62;
        constexpr uint8_t Peru = 0x63;
        constexpr uint8_t SouthAfrica = 0x64;
        constexpr uint8_t Suriname = 0x65;
        constexpr uint8_t Taiwan = 0x66;
        constexpr uint8_t Uruguay = 0x67;
        // 0x68 = unused
        constexpr uint8_t USA = 0x69;
        constexpr uint8_t Venezuela = 0x6A;
        constexpr uint8_t Ghana = 0x6B;
    }

    // === DOMESTIC CUPS (0x7D-0xB1) ===
    namespace Cups {
        constexpr uint8_t AlbanianCup = 0x7D;
        constexpr uint8_t AustrianCup = 0x7E;
        constexpr uint8_t BelarussianCup = 0x7F;
        constexpr uint8_t BelgianCup = 0x80;
        constexpr uint8_t BulgarianCup = 0x81;
        constexpr uint8_t CroatianCup = 0x82;
        constexpr uint8_t CypriotCup = 0x83;
        constexpr uint8_t CzechCup = 0x84;
        constexpr uint8_t DanishCup = 0x85;
        constexpr uint8_t EnglishFACup = 0x86;
        constexpr uint8_t EnglishLeagueCup = 0x87;
        constexpr uint8_t FrenchCup = 0x8D;
        constexpr uint8_t GermanCup = 0x8E;
        constexpr uint8_t GreekCup = 0x8F;
        constexpr uint8_t ItalianCup = 0x94;
        constexpr uint8_t DutchCup = 0x99;
        constexpr uint8_t NorwegianCup = 0x9B;
        constexpr uint8_t PortugueseCup = 0x9D;
        constexpr uint8_t RussianCup = 0x9F;
        constexpr uint8_t ScottishFACup = 0xA1;
        constexpr uint8_t ScottishLeagueCup = 0xA2;
        constexpr uint8_t SpanishCup = 0xA5;
        constexpr uint8_t SwedishCup = 0xA6;
        constexpr uint8_t TurkishCup = 0xA8;
        constexpr uint8_t JapaneseLeagueCup = 0xAE;
        constexpr uint8_t JapaneseEmperorsCup = 0xAF;
    }
}

} // namespace Competition

#pragma pack(pop)
