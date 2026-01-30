/**
 * Competition Data - SWOS League and Cup Definitions
 *
 * This file contains all domestic competition data in a readable format.
 * Each competition is documented with its real-world equivalent.
 *
 * Original data: swos/swos.asm lines 188870-192850
 */

#include "Competition.h"

using namespace Competition;
using namespace Competition::Month;
using namespace Competition::CupFormat;
using namespace Competition::SubsReserves;
using namespace Competition::Flags;

namespace Competition {

// ============================================================================
// DIVISION NAME STRINGS
// ============================================================================
// These are the names shown in-game for each division.

namespace Names {
    // English
    constexpr const char* PremierLeague = "PREMIER LEAGUE";
    constexpr const char* Premier = "PREMIER";
    constexpr const char* DivisionOne = "DIVISION ONE";
    constexpr const char* DivisionTwo = "DIVISION TWO";
    constexpr const char* DivisionThree = "DIVISION THREE";

    // English Cups
    constexpr const char* FACup = "CUP";
    constexpr const char* LeagueCup = "LEAGUE CUP";

    // Italian
    constexpr const char* SerieA = "SERIE A";
    constexpr const char* SerieB = "SERIE B";
    constexpr const char* CoppaItalia = "COPPA ITALIA";
    constexpr const char* Coppa = "COPPA";

    // French
    constexpr const char* PremiereDivision = "PREMIERE DIVISION";
    constexpr const char* Premiere = "PREMIERE";
    constexpr const char* DeuxiemeDivision = "DEUXIEME DIVISION";
    constexpr const char* Deuxieme = "DEUXIEME";
    constexpr const char* CoupeDeFrance = "COUPE DE FRANCE";
    constexpr const char* Coupe = "COUPE";

    // German
    constexpr const char* Bundesliga = "1. LIGA";
    constexpr const char* Bundesliga2 = "2. LIGA";
    constexpr const char* DFBPokal = "D.F.B. POKAL";
    constexpr const char* Pokal = "POKAL";

    // Spanish
    constexpr const char* LaLiga = "PRIMERA DIVISION";
    constexpr const char* Primera = "PRIMERA";
    constexpr const char* SegundaDivision = "SEGUNDA DIVISION";
    constexpr const char* Segunda = "SEGUNDA";
    constexpr const char* CopadelRey = "COPA";

    // Japanese
    constexpr const char* JLeague = "J. LEAGUE";
    constexpr const char* EmperorsCup = "EMPEROR'S CUP";
    constexpr const char* EmpCup = "EMP. CUP";

    // Generic (used by many countries)
    constexpr const char* League = "LEAGUE";
    constexpr const char* Cup = "CUP";
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
// These create the data structures with readable parameters.

/**
 * Creates a league header with named parameters.
 *
 * @param id           Competition ID (use Id::Europe:: or Id::World:: constants)
 * @param teams        Teams per division
 * @param seasonStart  Month season begins (use Month:: constants)
 * @param seasonEnd    Month season ends (use Month:: constants)
 * @param flags        Competition flags
 * @param divisions    Number of divisions (1-4)
 * @param homeAndAway  Times teams play each other (2 = home & away)
 * @param winPoints    Points for a win (usually 3)
 * @param subs         Substitutes/bench config (use SubsReserves:: constants)
 * @param divFlags     Division structure flags (use Flags:: constants)
 */
constexpr LeagueHeader makeLeague(
    uint8_t id,
    uint8_t teams,
    uint8_t seasonStart,
    uint8_t seasonEnd,
    uint8_t flags,
    uint8_t divisions,
    uint8_t homeAndAway,
    uint8_t winPoints,
    uint8_t subs,
    uint8_t divFlags
) {
    return LeagueHeader{
        id, 0, teams, seasonStart, seasonEnd, flags,
        0, 0, 0,  // reserved
        divisions, homeAndAway, winPoints, subs, divFlags
    };
}

/**
 * Creates division promotion/relegation rules.
 *
 * @param directUp     Teams directly promoted (top N)
 * @param playoffUp    Teams in promotion playoff (e.g., 4 = positions 3-6)
 * @param upTo         Competition ID promoted to (0 = European qualification/none)
 * @param directDown   Teams directly relegated (bottom N)
 * @param playoffDown  Teams in relegation playoff
 * @param downTo       Competition ID relegated to (0 = none)
 */
constexpr DivisionConfig makeDivision(
    uint8_t directUp, uint8_t playoffUp, uint8_t upTo,
    uint8_t directDown, uint8_t playoffDown, uint8_t downTo
) {
    return DivisionConfig{directUp, playoffUp, upTo, directDown, playoffDown, downTo};
}

/**
 * Creates a cup competition header.
 *
 * @param id           Competition ID (use Id::Cups:: constants)
 * @param teams        Maximum teams in the cup
 * @param cupStart     Month cup begins
 * @param cupEnd       Month cup ends (use MayNextYear for cups spanning years)
 * @param rounds       Total rounds in competition
 * @param topTeamEntry Round when top division teams enter
 * @param legs         0 = single matches, 2 = two-legged ties
 * @param subs         Substitutes config
 * @param formatRounds Rounds with special format
 */
constexpr CupHeader makeCup(
    uint8_t id,
    uint8_t teams,
    uint8_t cupStart,
    uint8_t cupEnd,
    uint8_t rounds,
    uint8_t topTeamEntry,
    uint8_t legs,
    uint8_t subs,
    uint8_t formatRounds
) {
    return CupHeader{
        id, 1, teams, cupStart, cupEnd, rounds,
        0, 0, 0, 0,  // reserved
        topTeamEntry, legs, subs, formatRounds
    };
}

// ============================================================================
//
//                    SINGLE DIVISION LEAGUES
//
// ============================================================================
// Countries with one top-flight division only.
// Most smaller European nations and world leagues fall into this category.

// ----------------------------------------------------------------------------
// NORTHERN EUROPE
// ----------------------------------------------------------------------------

/**
 * Danish Superliga
 * 12 teams, plays 3 rounds (each team plays others 4 times)
 * Summer-ish season due to Scandinavian climate
 */
const SingleDivisionLeague danishLeague = {
    makeLeague(
        Id::Europe::Denmark,        // Competition ID
        12,                         // 12 teams
        July, May,                  // Season: July to May
        None,                       // No special flags
        1,                          // 1 division
        4,                          // Play each team 4 times (3 rounds)
        3,                          // 3 points for a win
        Subs3_Bench5,               // 3 subs allowed, 5 on bench
        SingleDivision              // Single division format
    ),
    makeDivision(0, 0, 0, 0, 0, 0)  // No promotion/relegation in top flight
};

/**
 * Norwegian Tippeligaen
 * 14 teams, summer season (April-October due to winter climate)
 */
const SingleDivisionLeague norwegianLeague = {
    makeLeague(Id::Europe::Norway, 14, April, October, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Swedish Allsvenskan
 * 14 teams, summer season (April-October)
 */
const SingleDivisionLeague swedishLeague = {
    makeLeague(Id::Europe::Sweden, 14, April, October, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Finnish Veikkausliiga
 * 12 teams, short summer season (May-October)
 */
const SingleDivisionLeague finnishLeague = {
    makeLeague(Id::Europe::Finland, 12, May, October, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Icelandic Úrvalsdeild
 * 10 teams, very short summer season (May-September)
 */
const SingleDivisionLeague icelandicLeague = {
    makeLeague(Id::Europe::Iceland, 10, May, September, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Faroe Islands Premier League
 * 10 teams, summer season
 */
const SingleDivisionLeague faroeIslandsLeague = {
    makeLeague(Id::Europe::FaroeIslands, 10, May, October, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// BALTIC STATES
// ----------------------------------------------------------------------------

/**
 * Estonian Meistriliiga
 * 10 teams, summer season, 4x matches due to small league
 */
const SingleDivisionLeague estonianLeague = {
    makeLeague(Id::Europe::Estonia, 10, April, November, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Latvian Virslīga
 * 8 teams (small country), 4x matches
 */
const SingleDivisionLeague latvianLeague = {
    makeLeague(Id::Europe::Latvia, 8, April, November, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Lithuanian A Lyga
 * 8 teams, 4x matches
 */
const SingleDivisionLeague lithuanianLeague = {
    makeLeague(Id::Europe::Lithuania, 8, April, November, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// CENTRAL EUROPE
// ----------------------------------------------------------------------------

/**
 * Austrian Bundesliga
 * 10 teams, plays 4x due to small league
 */
const SingleDivisionLeague austrianLeague = {
    makeLeague(Id::Europe::Austria, 10, July, June, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Swiss Super League
 * 12 teams, plays 4x
 */
const SingleDivisionLeague swissLeague = {
    makeLeague(Id::Europe::Switzerland, 12, July, May, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Belgian First Division
 * 18 teams, standard format
 */
const SingleDivisionLeague belgianLeague = {
    makeLeague(Id::Europe::Belgium, 18, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Czech First League
 * 16 teams
 */
const SingleDivisionLeague czechLeague = {
    makeLeague(Id::Europe::CzechRepublic, 16, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Slovak Super Liga
 * 16 teams
 */
const SingleDivisionLeague slovakianLeague = {
    makeLeague(Id::Europe::Slovakia, 16, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Hungarian NB I
 * 18 teams
 */
const SingleDivisionLeague hungarianLeague = {
    makeLeague(Id::Europe::Hungary, 18, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Polish Ekstraklasa
 * 18 teams
 */
const SingleDivisionLeague polishLeague = {
    makeLeague(Id::Europe::Poland, 18, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Slovenian PrvaLiga
 * 12 teams
 */
const SingleDivisionLeague slovenianLeague = {
    makeLeague(Id::Europe::Slovenia, 12, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Croatian First League
 * 16 teams
 */
const SingleDivisionLeague croatianLeague = {
    makeLeague(Id::Europe::Croatia, 16, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// EASTERN EUROPE
// ----------------------------------------------------------------------------

/**
 * Russian Premier League
 * 18 teams, spring-to-autumn season due to winter
 */
const SingleDivisionLeague russianLeague = {
    makeLeague(Id::Europe::Russia, 18, March, November, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Ukrainian Premier League
 * 18 teams, spring-to-autumn season
 */
const SingleDivisionLeague ukrainianLeague = {
    makeLeague(Id::Europe::Ukraine, 18, March, November, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Belarusian Premier League
 * 17 teams (odd number is unusual)
 */
const SingleDivisionLeague belarussianLeague = {
    makeLeague(Id::Europe::Belarus, 17, April, November, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Romanian Liga I
 * 18 teams
 */
const SingleDivisionLeague romanianLeague = {
    makeLeague(Id::Europe::Romania, 18, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Bulgarian First League
 * 16 teams
 */
const SingleDivisionLeague bulgarianLeague = {
    makeLeague(Id::Europe::Bulgaria, 16, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Yugoslav First League (pre-breakup)
 * 18 teams
 */
const SingleDivisionLeague yugoslavianLeague = {
    makeLeague(Id::Europe::Yugoslavia, 18, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// SOUTHERN EUROPE
// ----------------------------------------------------------------------------

/**
 * Greek Super League
 * 18 teams
 */
const SingleDivisionLeague greekLeague = {
    makeLeague(Id::Europe::Greece, 18, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Portuguese Primeira Liga
 * 18 teams
 */
const SingleDivisionLeague portugueseLeague = {
    makeLeague(Id::Europe::Portugal, 18, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Turkish Süper Lig
 * 18 teams
 */
const SingleDivisionLeague turkishLeague = {
    makeLeague(Id::Europe::Turkey, 18, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Cypriot First Division
 * 14 teams
 */
const SingleDivisionLeague cypriotLeague = {
    makeLeague(Id::Europe::Cyprus, 14, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Israeli Premier League
 * 16 teams
 */
const SingleDivisionLeague israeliLeague = {
    makeLeague(Id::Europe::Israel, 16, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Maltese Premier League
 * 10 teams (tiny country)
 */
const SingleDivisionLeague malteseLeague = {
    makeLeague(Id::Europe::Malta, 10, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// SMALL EUROPEAN NATIONS
// ----------------------------------------------------------------------------

/**
 * Albanian Superliga
 * 14 teams
 */
const SingleDivisionLeague albanianLeague = {
    makeLeague(Id::Europe::Albania, 14, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * San Marino Championship
 * 15 teams (odd - possibly includes B teams)
 */
const SingleDivisionLeague sanMarinoLeague = {
    makeLeague(Id::Europe::SanMarino, 15, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Luxembourg National Division
 * 12 teams
 */
const SingleDivisionLeague luxembourgLeague = {
    makeLeague(Id::Europe::Luxembourg, 12, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Welsh Premier League
 * 18 teams
 */
const SingleDivisionLeague welshLeague = {
    makeLeague(Id::Europe::Wales, 18, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Northern Irish Premiership
 * 16 teams
 */
const SingleDivisionLeague northernIrishLeague = {
    makeLeague(Id::Europe::NorthernIreland, 16, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Irish League of Ireland
 * 12 teams
 */
const SingleDivisionLeague irishLeague = {
    makeLeague(Id::Europe::Ireland, 12, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// SOUTH AMERICA
// ----------------------------------------------------------------------------

/**
 * Brazilian Série A
 * 24 teams - one of the largest top divisions in the world
 */
const SingleDivisionLeague brazilianLeague = {
    makeLeague(Id::World::Brazil, 24, April, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Argentine Primera División
 * 20 teams
 */
const SingleDivisionLeague argentinianLeague = {
    makeLeague(Id::World::Argentina, 20, August, June, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Uruguayan Primera División
 * 16 teams
 */
const SingleDivisionLeague uruguayanLeague = {
    makeLeague(Id::World::Uruguay, 16, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Chilean Primera División
 * 16 teams
 */
const SingleDivisionLeague chileanLeague = {
    makeLeague(Id::World::Chile, 16, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Colombian Categoría Primera A
 * 18 teams
 */
const SingleDivisionLeague colombianLeague = {
    makeLeague(Id::World::Colombia, 18, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Paraguayan Primera División
 * 12 teams
 */
const SingleDivisionLeague paraguayanLeague = {
    makeLeague(Id::World::Paraguay, 12, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Peruvian Primera División
 * 16 teams
 */
const SingleDivisionLeague peruvianLeague = {
    makeLeague(Id::World::Peru, 16, March, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Bolivian Liga de Fútbol Profesional
 * 12 teams
 */
const SingleDivisionLeague bolivianLeague = {
    makeLeague(Id::World::Bolivia, 12, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Ecuadorian Serie A
 * 12 teams
 */
const SingleDivisionLeague ecuadorLeague = {
    makeLeague(Id::World::Ecuador, 12, February, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Venezuelan Primera División
 * 12 teams
 */
const SingleDivisionLeague venezuelanLeague = {
    makeLeague(Id::World::Venezuela, 12, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Surinamese Hoofdklasse
 * 10 teams
 */
const SingleDivisionLeague surinamLeague = {
    makeLeague(Id::World::Suriname, 10, March, December, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ----------------------------------------------------------------------------
// REST OF WORLD
// ----------------------------------------------------------------------------

/**
 * US Major League Soccer (MLS)
 * 12 teams (was smaller in 1990s)
 */
const SingleDivisionLeague americanLeague = {
    makeLeague(Id::World::USA, 12, April, October, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Algerian Ligue Professionnelle 1
 * 16 teams
 */
const SingleDivisionLeague algerianLeague = {
    makeLeague(Id::World::Algeria, 16, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * South African Premier Division
 * 18 teams
 */
const SingleDivisionLeague southAfricanLeague = {
    makeLeague(Id::World::SouthAfrica, 18, August, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Ghana Premier League
 * 16 teams
 */
const SingleDivisionLeague ghanaLeague = {
    makeLeague(Id::World::Ghana, 16, September, May, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Indian I-League
 * 12 teams
 */
const SingleDivisionLeague indianLeague = {
    makeLeague(Id::World::India, 12, November, April, None, 1, 2, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

/**
 * Taiwanese Premier Football League
 * 6 teams (very small), plays 4x
 */
const SingleDivisionLeague taiwaneseLeague = {
    makeLeague(Id::World::Taiwan, 6, March, November, None, 1, 4, 3, Subs3_Bench5, SingleDivision),
    makeDivision(0, 0, 0, 0, 0, 0)
};

// ============================================================================
//
//                    MULTI-DIVISION LEAGUES
//
// ============================================================================
// Major nations with pyramid systems (promotion/relegation between divisions).

// ----------------------------------------------------------------------------
// ENGLAND - 4 Divisions
// ----------------------------------------------------------------------------

/**
 * English Football League System
 *
 * 4 divisions with full promotion/relegation:
 * - Premier League: 20 teams, top 3-4 to Europe, bottom 3 relegated
 * - Division One: 22 teams, top 2 promoted + playoff, bottom 3 relegated
 * - Division Two: 22 teams, top 2 promoted + playoff, bottom 4 relegated
 * - Division Three: 22 teams, top 3 promoted + playoff, no relegation
 */
struct EnglishLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[4];
    const char* divisionNames[8];  // 2 names per division (full name, short name)
};

const EnglishLeagues englishLeagues = {
    makeLeague(
        Id::Europe::England,
        22,                 // Teams per division (Premier has 20 but structure uses 22)
        July, May,          // July to May season
        HasPlayoffs,        // Has promotion playoffs
        4,                  // 4 divisions
        2,                  // Home and away
        3,                  // 3 points for win
        Subs3_Bench3,       // 3 subs, 3 on bench
        FourDivisions       // 4-division structure
    ),
    {0, 0},  // padding
    {
        // Premier League: Top 3 to Europe, bottom 3 relegated to Div 1
        makeDivision(3, 0, 0, 3, 0, 0x2E),

        // Division One: Top 2 promoted + 4 in playoff, bottom 3 to Div 2
        makeDivision(2, 4, Id::Europe::England, 3, 0, 0x2F),

        // Division Two: Top 2 promoted + 4 in playoff, bottom 4 to Div 3
        makeDivision(2, 4, 0x2E, 4, 0, 0x30),

        // Division Three: Top 3 promoted + 4 in playoff, no relegation
        makeDivision(3, 4, 0x2F, 0, 0, 0),
    },
    {
        Names::PremierLeague, Names::Premier,
        Names::DivisionOne, Names::DivisionOne,
        Names::DivisionTwo, Names::DivisionTwo,
        Names::DivisionThree, Names::DivisionThree,
    }
};

// ----------------------------------------------------------------------------
// SCOTLAND - 4 Divisions
// ----------------------------------------------------------------------------

/**
 * Scottish Football League System
 *
 * 4 divisions, smaller than England (10 teams each):
 * - Premier Division: 10 teams, plays 4x
 */
struct ScottishLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[4];
    const char* divisionNames[8];
};

const ScottishLeagues scottishLeagues = {
    makeLeague(Id::Europe::Scotland, 10, August, May, None, 4, 4, 3, Subs3_Bench5, FourDivisions),
    {0, 0},
    {
        makeDivision(2, 0, 0, 1, 0, 0x4A),           // Premier: 2 to Europe, 1 relegated
        makeDivision(1, 0, Id::Europe::Scotland, 1, 0, 0x4B),  // Div 1
        makeDivision(1, 0, 0x4A, 1, 0, 0x4C),        // Div 2
        makeDivision(1, 0, 0x4B, 0, 0, 0),           // Div 3: no relegation
    },
    {
        Names::Premier, Names::Premier,
        Names::DivisionOne, Names::DivisionOne,
        Names::DivisionTwo, Names::DivisionTwo,
        Names::DivisionThree, Names::DivisionThree,
    }
};

// ----------------------------------------------------------------------------
// ITALY - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * Italian Football League System
 *
 * Serie A: 20 teams, top 4 to Europe, bottom 4 relegated
 * Serie B: 20 teams, top 4 promoted, no relegation modeled
 */
struct ItalianLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const ItalianLeagues italianLeagues = {
    makeLeague(Id::Europe::Italy, 20, August, May, ItalianStyle, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(4, 0, 0, 4, 0, 0x3D),    // Serie A: 4 to Europe, 4 relegated
        makeDivision(4, 0, Id::Europe::Italy, 0, 0, 0),  // Serie B: 4 promoted
    },
    {
        Names::SerieA, Names::SerieA,
        Names::SerieB, Names::SerieB,
    }
};

// ----------------------------------------------------------------------------
// GERMANY - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * German Bundesliga System
 *
 * Bundesliga: 18 teams
 * 2. Bundesliga: 18 teams
 */
struct GermanLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const GermanLeagues germanLeagues = {
    makeLeague(Id::Europe::Germany, 18, August, May, None, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(3, 0, 0, 3, 0, 0x36),    // Bundesliga: 3 to Europe, 3 relegated
        makeDivision(3, 0, Id::Europe::Germany, 0, 0, 0),  // 2. Bundesliga: 3 promoted
    },
    {
        Names::Bundesliga, Names::Bundesliga,
        Names::Bundesliga2, Names::Bundesliga2,
    }
};

// ----------------------------------------------------------------------------
// FRANCE - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * French Ligue System
 *
 * Ligue 1 (Premiere Division): 20 teams
 * Ligue 2 (Deuxieme Division): 20 teams
 */
struct FrenchLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const FrenchLeagues frenchLeagues = {
    makeLeague(Id::Europe::France, 20, August, May, None, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(3, 0, 0, 3, 0, 0x35),
        makeDivision(3, 0, Id::Europe::France, 0, 0, 0),
    },
    {
        Names::PremiereDivision, Names::Premiere,
        Names::DeuxiemeDivision, Names::Deuxieme,
    }
};

// ----------------------------------------------------------------------------
// SPAIN - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * Spanish La Liga System
 *
 * La Liga (Primera Division): 20 teams
 * Segunda Division: 20 teams
 */
struct SpanishLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const SpanishLeagues spanishLeagues = {
    makeLeague(Id::Europe::Spain, 20, September, May, None, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(4, 0, 0, 4, 0, 0x4E),    // La Liga: 4 to Europe, 4 relegated
        makeDivision(4, 0, Id::Europe::Spain, 0, 0, 0),
    },
    {
        Names::LaLiga, Names::Primera,
        Names::SegundaDivision, Names::Segunda,
    }
};

// ----------------------------------------------------------------------------
// NETHERLANDS - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * Dutch Eredivisie System
 *
 * Eredivisie: 18 teams
 * Eerste Divisie: 18 teams
 */
struct DutchLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const DutchLeagues dutchLeagues = {
    makeLeague(Id::Europe::Netherlands, 18, August, May, None, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(3, 0, 0, 2, 0, 0x38),    // Eredivisie
        makeDivision(2, 0, Id::Europe::Netherlands, 0, 0, 0),
    },
    {
        Names::League, Names::League,
        Names::League, Names::League,
    }
};

// ----------------------------------------------------------------------------
// JAPAN - 2 Divisions
// ----------------------------------------------------------------------------

/**
 * Japanese J.League System
 *
 * J1 League: 18 teams
 * J2 League: 18 teams (in 1990s was smaller)
 */
struct JapaneseLeagues {
    LeagueHeader header;
    uint8_t padding[2];
    DivisionConfig divisions[2];
    const char* divisionNames[4];
};

const JapaneseLeagues japaneseLeagues = {
    makeLeague(Id::World::Japan, 18, March, December, None, 2, 2, 3, Subs3_Bench5, TwoDivisions),
    {0, 0},
    {
        makeDivision(0, 0, 0, 2, 0, 0x60),    // J1: 2 relegated
        makeDivision(2, 0, Id::World::Japan, 0, 0, 0),  // J2: 2 promoted
    },
    {
        Names::JLeague, Names::JLeague,
        Names::League, Names::League,
    }
};

// ============================================================================
//
//                    CUP COMPETITIONS
//
// ============================================================================

/**
 * Cup Competition Data Structure
 *
 * Contains the cup header plus round format information.
 * roundFormats specifies how each round is played:
 * - HomeWithReplay (0x68): Single match at home team's ground, replay if drawn
 * - TwoLegs (0x94): Home and away ties
 * - NeutralVenue (0x28): Single match at neutral ground (finals)
 */
struct CupData {
    CupHeader header;
    uint8_t roundFormats[6];
    const char* names[2];  // Full name, short name
};

// ----------------------------------------------------------------------------
// ENGLISH CUPS
// ----------------------------------------------------------------------------

/**
 * English FA Cup
 *
 * The world's oldest cup competition.
 * 64 teams (top 4 divisions), single matches with replays, neutral final.
 */
const CupData englishFACup = {
    makeCup(
        Id::Cups::EnglishFACup,
        64,                         // 64 teams
        December, MayNextYear,      // December to May (spans year boundary)
        15,                         // 15 rounds total
        64,                         // Premier League teams enter at round of 64
        0,                          // Single matches (not two-legged)
        Subs3_Bench3,
        3                           // 3 rounds with special format
    ),
    {
        HomeWithReplay,     // Early rounds: home with replay
        HomeWithReplay,
        HomeWithReplay,
        HomeWithReplay,
        NeutralVenue,       // Semi-final at neutral venue
        NeutralVenue        // Final at Wembley
    },
    {Names::FACup, Names::FACup}
};

/**
 * English League Cup (Carling Cup, EFL Cup, etc.)
 *
 * Secondary cup, two-legged ties in later rounds.
 */
const CupData englishLeagueCup = {
    makeCup(
        Id::Cups::EnglishLeagueCup,
        64,
        August, February,    // Runs alongside league season
        15,
        64,
        2,                   // Two-legged ties
        Subs3_Bench3,
        2
    ),
    {
        TwoLegs,             // Two-legged early rounds
        HomeWithReplay,
        HomeWithReplay,
        HomeWithReplay,
        TwoLegs,             // Semi-final is two legs
        NeutralVenue         // Final at Wembley
    },
    {Names::LeagueCup, Names::LeagueCup}
};

// ----------------------------------------------------------------------------
// OTHER MAJOR EUROPEAN CUPS
// ----------------------------------------------------------------------------

/**
 * Italian Coppa Italia
 * 32 teams, two-legged ties
 */
const CupData italianCup = {
    makeCup(Id::Cups::ItalianCup, 32, August, May, 10, 32, 2, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::CoppaItalia, Names::Coppa}
};

/**
 * German DFB-Pokal
 * 64 teams, single matches
 */
const CupData germanCup = {
    makeCup(Id::Cups::GermanCup, 64, August, June, 12, 64, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::DFBPokal, Names::Pokal}
};

/**
 * French Coupe de France
 * 64 teams
 */
const CupData frenchCup = {
    makeCup(Id::Cups::FrenchCup, 64, September, May, 12, 64, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::CoupeDeFrance, Names::Coupe}
};

/**
 * Spanish Copa del Rey
 * 32 teams, two-legged
 */
const CupData spanishCup = {
    makeCup(Id::Cups::SpanishCup, 32, September, June, 10, 32, 2, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::CopadelRey, Names::CopadelRey}
};

/**
 * Scottish FA Cup
 * 40 teams
 */
const CupData scottishFACup = {
    makeCup(Id::Cups::ScottishFACup, 40, January, May, 10, 40, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Scottish League Cup
 * 40 teams, runs early in season
 */
const CupData scottishLeagueCup = {
    makeCup(Id::Cups::ScottishLeagueCup, 40, August, November, 8, 40, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::LeagueCup, Names::LeagueCup}
};

/**
 * Dutch KNVB Cup
 */
const CupData dutchCup = {
    makeCup(Id::Cups::DutchCup, 32, September, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Belgian Cup
 */
const CupData belgianCup = {
    makeCup(Id::Cups::BelgianCup, 32, September, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Portuguese Taça de Portugal
 */
const CupData portugueseCup = {
    makeCup(Id::Cups::PortugueseCup, 32, September, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Greek Cup
 */
const CupData greekCup = {
    makeCup(Id::Cups::GreekCup, 32, September, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Turkish Cup
 */
const CupData turkishCup = {
    makeCup(Id::Cups::TurkishCup, 32, September, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Russian Cup
 */
const CupData russianCup = {
    makeCup(Id::Cups::RussianCup, 32, June, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Swedish Cup
 */
const CupData swedishCup = {
    makeCup(Id::Cups::SwedishCup, 32, February, October, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Norwegian Cup
 */
const CupData norwegianCup = {
    makeCup(Id::Cups::NorwegianCup, 32, April, October, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Danish Cup
 */
const CupData danishCup = {
    makeCup(Id::Cups::DanishCup, 32, August, May, 10, 32, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::Cup, Names::Cup}
};

/**
 * Japanese Emperor's Cup
 * Traditional cup competition dating back to 1921
 */
const CupData japaneseEmperorsCup = {
    makeCup(Id::Cups::JapaneseEmperorsCup, 48, September, January, 10, 48, 0, Subs3_Bench3, 2),
    {HomeWithReplay, HomeWithReplay, HomeWithReplay, HomeWithReplay, NeutralVenue, NeutralVenue},
    {Names::EmperorsCup, Names::EmpCup}
};

} // namespace Competition
