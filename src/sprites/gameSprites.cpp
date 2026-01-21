#include "gameSprites.h"
#include "log.h"
#include "sprites.h"
#include "renderSprites.h"
#include "colorizeSprites.h"
#include "referee.h"
#include "replays.h"
#include "camera.h"
#ifdef SWOS_TEST
# include "render.h"
#endif

static constexpr int kPlayerSpritesStart = 4;

static Sprite m_cornerFlagSpriteTopLeft, m_cornerFlagSpriteTopRight,
    m_cornerFlagSpriteBottomLeft, m_cornerFlagSpriteBottomRight;

static const std::array<Sprite *, 4> kCornerFlagSprites = {
    &m_cornerFlagSpriteTopLeft, &m_cornerFlagSpriteTopRight,
    &m_cornerFlagSpriteBottomLeft, &m_cornerFlagSpriteBottomRight,
};

static Sprite * const kAllSprites[] = {
    &swos.ballShadowSprite,
    &swos.ballSprite,
    &swos.goal1TopSprite,
    &swos.goal2BottomSprite,
    &swos.goalie1Sprite,
    &swos.team1Player2Sprite,
    &swos.team1Player3Sprite,
    &swos.team1Player4Sprite,
    &swos.team1Player5Sprite,
    &swos.team1Player6Sprite,
    &swos.team1Player7Sprite,
    &swos.team1Player8Sprite,
    &swos.team1Player9Sprite,
    &swos.team1Player10Sprite,
    &swos.team1Player11Sprite,
    &swos.goalie2Sprite,
    &swos.team2Player2Sprite,
    &swos.team2Player3Sprite,
    &swos.team2Player4Sprite,
    &swos.team2Player5Sprite,
    &swos.team2Player6Sprite,
    &swos.team2Player7Sprite,
    &swos.team2Player8Sprite,
    &swos.team2Player9Sprite,
    &swos.team2Player10Sprite,
    &swos.team2Player11Sprite,
    &swos.team1CurPlayerNumSprite,
    &swos.team2CurPlayerNumSprite,
    &swos.playerMarkSprite,
    bookedPlayerNumberSprite(),
    refereeSprite(),
    &m_cornerFlagSpriteTopLeft,
    &m_cornerFlagSpriteTopRight,
    &m_cornerFlagSpriteBottomLeft,
    &m_cornerFlagSpriteBottomRight,
};

static std::array<Sprite *, std::size(kAllSprites)> m_sortedSprites;
static int m_numSpritesToRender;

static const TeamGame *m_topTeam;
static const TeamGame *m_bottomTeam;

static void sortDisplaySprites();
static bool shouldZoomSprite(int imageIndex);

void initGameSprites(const TeamGame *topTeam, const TeamGame *bottomTeam)
{
    logInfo("=== initGameSprites START ===");
    logInfo("topTeam=%p, bottomTeam=%p", (void*)topTeam, (void*)bottomTeam);
    flushLog();

    m_topTeam = topTeam;
    m_bottomTeam = bottomTeam;

    logInfo("Initializing %zu sprites...", std::size(kAllSprites));
    for (auto sprite : kAllSprites)
        sprite->init();
    logInfo("Base sprites initialized");
    for (auto sprite : kCornerFlagSprites) {
        sprite->init();
        sprite->teamNumber = 3;
    }

    constexpr int kGoalX = 300;
    constexpr int kTopGoalY = 129;
    constexpr int kBottomGoalY = 778;

    swos.goal1TopSprite.x = kGoalX;
    swos.goal1TopSprite.y = kTopGoalY;
    swos.goal1TopSprite.destX = kGoalX;
    swos.goal1TopSprite.destY = kTopGoalY;
    swos.goal1TopSprite.setImage(kTopGoalSprite);
    swos.goal2BottomSprite.x = kGoalX;
    swos.goal2BottomSprite.y = kBottomGoalY;
    swos.goal2BottomSprite.destX = kGoalX;
    swos.goal2BottomSprite.destY = kBottomGoalY;
    swos.goal2BottomSprite.setImage(kBottomGoalSprite);

    bookedPlayerNumberSprite()->setImage(kPlayerMarkSprite);

    logInfo("Calling initializePlayerSpriteFrameIndices...");
    flushLog();
    initializePlayerSpriteFrameIndices();
    logInfo("initializePlayerSpriteFrameIndices completed");
    logInfo("=== initGameSprites COMPLETED ===");
    flushLog();
}

// Prepares display sprites for rendering. Sorts them by y-axis.
void initDisplaySprites()
{
    logInfo("=== initDisplaySprites START ===");
    m_numSpritesToRender = 0;

    for (auto sprite : kAllSprites)
        if (sprite->visible)
            m_sortedSprites[m_numSpritesToRender++] = sprite;

    logInfo("Visible sprites to render: %d", m_numSpritesToRender);
    sortDisplaySprites();
    logInfo("=== initDisplaySprites COMPLETED ===");
    flushLog();
}

void initializePlayerSpriteFrameIndices()
{
    logInfo("=== initializePlayerSpriteFrameIndices START ===");
    const auto kTeamData = {
        std::make_tuple(swos.team1SpritesTable, m_topTeam, true),
        std::make_tuple(swos.team2SpritesTable, m_bottomTeam, false)
    };

    int teamIndex = 0;
    for (const auto& teamData : kTeamData) {
        auto team = std::get<1>(teamData);
        auto spriteTable = std::get<0>(teamData);
        bool topTeam = std::get<2>(teamData);

        logInfo("Processing team %d: team=%p, spriteTable=%p, isTop=%d", teamIndex, (void*)team, (void*)spriteTable, topTeam);

        if (!team) {
            logError("ERROR: team pointer is NULL for team %d!", teamIndex);
            flushLog();
            teamIndex++;
            continue;
        }

        if (!spriteTable[0]) {
            logError("ERROR: spriteTable[0] is NULL for team %d!", teamIndex);
            flushLog();
        } else {
            spriteTable[0]->frameOffset = getGoalkeeperSpriteOffset(topTeam, team[0].players[0].face);
            logInfo("Team %d goalkeeper sprite offset set", teamIndex);
        }

        for (size_t i = 1; i < std::size(swos.team1SpritesTable); i++) {
            auto& player = team->players[i];
            auto& sprite = spriteTable[i];

            if (!sprite) {
                logError("ERROR: spriteTable[%zu] is NULL for team %d!", i, teamIndex);
                continue;
            }

            assert(player.face <= 3);
            sprite->frameOffset = getPlayerSpriteOffsetFromFace(player.face);
        }
        logInfo("Team %d all player sprites processed", teamIndex);
        teamIndex++;
    }
    logInfo("=== initializePlayerSpriteFrameIndices COMPLETED ===");
    flushLog();
}

#ifdef DEBUG
static void verifySprites()
{
    for (const auto& sprite : kAllSprites) {
        auto assertIn = [sprite](int start, int end, bool allowEmpty = false) {
            if (!allowEmpty || sprite->hasImage())
                assert(sprite->imageIndex >= start && sprite->imageIndex <= end);
        };

        if (sprite == &swos.ballShadowSprite)
            assert(sprite->imageIndex == kBallShadowSprite || sprite->hasNoImage());
        else if (sprite == &swos.ballSprite)
            assertIn(kBallSprite1, kBallSprite4, true);
        else if (sprite == &swos.goal1TopSprite)
            assert(sprite->imageIndex == kTopGoalSprite);
        else if (sprite == &swos.goal2BottomSprite)
            assert(sprite->imageIndex == kBottomGoalSprite);
        else if (sprite == &swos.goalie1Sprite)
            assertIn(kTeam1MainGoalkeeperSpriteStart, kTeam1ReserveGoalkeeperSpriteEnd);
        else if (sprite == &swos.goalie2Sprite)
            assertIn(kTeam2MainGoalkeeperSpriteStart, kTeam2ReserveGoalkeeperSpriteEnd);
        else if (sprite == &swos.team1Player2Sprite || sprite == &swos.team1Player3Sprite ||
            sprite == &swos.team1Player4Sprite || sprite == &swos.team1Player5Sprite ||
            sprite == &swos.team1Player6Sprite || sprite == &swos.team1Player7Sprite ||
            sprite == &swos.team1Player8Sprite || sprite == &swos.team1Player9Sprite ||
            sprite == &swos.team1Player10Sprite || sprite == &swos.team1Player11Sprite)
            assertIn(kTeam1WhitePlayerSpriteStart, kTeam1BlackPlayerSpriteEnd);
        else if (sprite == &swos.team2Player2Sprite || sprite == &swos.team2Player3Sprite ||
            sprite == &swos.team2Player4Sprite || sprite == &swos.team2Player5Sprite ||
            sprite == &swos.team2Player6Sprite || sprite == &swos.team2Player7Sprite ||
            sprite == &swos.team2Player8Sprite || sprite == &swos.team2Player9Sprite ||
            sprite == &swos.team2Player10Sprite || sprite == &swos.team2Player11Sprite)
            assertIn(kTeam2WhitePlayerSpriteStart, kTeam2BlackPlayerSpriteEnd);
        else if (sprite == &swos.team1CurPlayerNumSprite || sprite == &swos.team2CurPlayerNumSprite)
            assertIn(kSmallDigit1, kSmallDigit16, true);
        else if (sprite == &swos.playerMarkSprite)
            assert(sprite->imageIndex == kPlayerMarkSprite || sprite->imageIndex == -1);
        else if (sprite == bookedPlayerNumberSprite())
            assert(sprite->imageIndex >= kSmallDigit1 && sprite->imageIndex <= kSmallDigit16 ||
                sprite->imageIndex == kRedCardSprite || sprite->imageIndex == kYellowCardSprite ||
                sprite->hasNoImage());
        else if (sprite == refereeSprite())
            assertIn(kRefereeSpriteStart, kRefereeSpriteEnd, true);
        else if (sprite == &m_cornerFlagSpriteTopLeft || sprite == &m_cornerFlagSpriteTopRight ||
            sprite == &m_cornerFlagSpriteBottomLeft || sprite == &m_cornerFlagSpriteBottomRight)
            assertIn(kCornerFlagSpriteStart, kCornerFlagSpriteEnd);
    }
}
#endif

// The place where game sprites get drawn to the screen.
void drawSprites(float xOffset, float yOffset)
{
#ifdef DEBUG
    verifySprites();
#endif

    sortDisplaySprites();

    int screenWidth, screenHeight;
    std::tie(screenWidth, screenHeight) = getWindowSize();

    auto cameraX = getCameraX();
    auto cameraY = getCameraY();

    for (int i = 0; i < m_numSpritesToRender; i++) {
        auto sprite = m_sortedSprites[i];

        if (sprite->hasNoImage())
            continue;

        assert(sprite->visible);

        auto x = sprite->x - cameraX;
        auto y = sprite->y - cameraY - sprite->z;

        auto zoom = shouldZoomSprite(sprite->imageIndex);
        sprite->onScreen = drawSprite(sprite->imageIndex, x, y, zoom, xOffset, yOffset);

#ifdef SWOS_TEST
        const auto& sprImage = getSprite(sprite->imageIndex);
        int iX = sprite->x.whole() - cameraX.whole() - static_cast<int>(sprImage.centerXF);
        int iY = sprite->y.whole() - cameraY.whole() - sprite->z.whole() - static_cast<int>(sprImage.centerYF);
        // SWOS uses 336 instead of 320 for clipping
        constexpr int kClipWidth = 336;
        sprite->onScreen = iX < kClipWidth && iY < kVgaHeight && iX > -sprImage.width && iY > -sprImage.height;
#endif

        // since screen can potentially be huge don't reject any sprites for highlights, just dump them all there
        if (sprite->teamNumber)
            saveCoordinatesForHighlights(sprite->imageIndex, x, y);
    }
}

int getGoalkeeperSpriteOffset(bool topTeam, int face)
{
    constexpr int kNumGoalkeeperSprites = kTeam1ReserveGoalkeeperSpriteStart - kTeam1MainGoalkeeperSpriteStart;

    assert(face >= 0 && face <= 3);

    auto goalie = getGoalkeeperIndexFromFace(topTeam, face);
    assert(goalie == 0 || goalie == 1);

    return goalie * kNumGoalkeeperSprites;
}

int getPlayerSpriteOffsetFromFace(int face)
{
    constexpr int kNumPlayerSprites = kTeam1GingerPlayerSpriteStart - kTeam1WhitePlayerSpriteStart;

    assert(face >= 0 && face <= 3);

    return face * kNumPlayerSprites;
}

void updateCornerFlags()
{
    constexpr int kLeftCornerFlagX = 81;
    constexpr int kRightCornerFlagX = 590;
    constexpr int kTopCornerFlagY = 129;
    constexpr int kBottomCornerFlagY = 769;

    static const std::array<std::pair<int, int>, 4> kCornerFlagCoordinates = {{
        { kLeftCornerFlagX, kTopCornerFlagY }, { kRightCornerFlagX, kTopCornerFlagY },
        { kLeftCornerFlagX, kBottomCornerFlagY }, { kRightCornerFlagX, kBottomCornerFlagY },
    }};

    static const std::array<uint8_t, 32> kCornerFlagFrameOffsets = {
        0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
    };

    for (size_t i = 0; i < kCornerFlagSprites.size(); i++) {
        auto& sprite = kCornerFlagSprites[i];

        sprite->x = kCornerFlagCoordinates[i].first;
        sprite->y = kCornerFlagCoordinates[i].second;

        int frame = (swos.frameCount >> 1) & 0x1f;
        sprite->setImage(kCornerFlagSpriteStart + kCornerFlagFrameOffsets[frame]);

        sprite->show();
    }
}

PlayerSprites getPlayerSprites()
{
    assert(std::all_of(&kAllSprites[kPlayerSpritesStart], &kAllSprites[kPlayerSpritesStart] + 2 * kNumPlayersInLineup,
        [](const auto& sprite) {
            int index = &sprite - &kAllSprites[kPlayerSpritesStart];
            return sprite->teamNumber == (index > 10 ? 2 : 1) &&
                sprite->playerOrdinal == (index + 1 - (sprite->teamNumber == 2 ? 11 : 0));
        }
    ));
    return &kAllSprites[kPlayerSpritesStart];
}

#ifdef SWOS_TEST
Sprite *spriteAt(unsigned index)
{
    assert(index < std::size(kAllSprites));
    return kAllSprites[index];
}

int totalSprites()
{
    return std::size(kAllSprites);
}
#endif

static void sortDisplaySprites()
{
    std::sort(m_sortedSprites.begin(), m_sortedSprites.begin() + m_numSpritesToRender, [](const auto& spr1, const auto& spr2) {
        return spr1->y < spr2->y;
    });
}

static bool shouldZoomSprite(int imageIndex)
{
    return imageIndex >= kTeam1WhitePlayerSpriteStart && imageIndex <= kBottomGoalSprite ||
        imageIndex >= kRefereeSpriteStart;
}
