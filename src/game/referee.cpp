#include "referee.h"
#include "camera.h"
#include "random.h"
#include "sprites.h"
#include "gameSprites.h"
#include "updateSprite.h"
#include "comments.h"
#include "pitchConstants.h"

constexpr int kRefereeSpeed = 1'024;

constexpr int kRefereeHidingPlaceX = 276;
constexpr int kRefereeHidingPlaceY = 439;

constexpr int kRefereeLeavingTopDestY = 129;
constexpr int kRefereeLeavingBottomDestY = 770;

constexpr int kSentOffPlayerX = -20;
constexpr int kSentOffPlayerY = 449;

constexpr int kPlayerNumberOffset = 20;

enum RefereeState
{
    kRefOffScreen = 0,
    kRefIncoming = 1,
    kRefWaitingPlayer = 2,
    kRefAboutToGiveCard = 3,
    kRefBooking = 4,
    kRefLeaving = 5,
};

enum CardHanding
{
    kNoCard = 0,
    kYellowCard = 1,
    kRedCard = 2,
    kSecondYellowCard = 3,
};

// set team number so they show up in replays
static Sprite m_refereeSprite{ 3 };
static Sprite m_bookedPlayerNumberSprite{ 3 };

static void updateRefereeState();
static void putRefereeToLeavingState();
static void sendPlayerAway();
static void initRefereeAnimationTable(SwosDataPointer<void> animTable);

void activateReferee()
{
    m_refereeSprite.destX = swos.foulXCoordinate + 28;
    m_refereeSprite.destY = swos.foulYCoordinate + 5;

    int xOffset = SWOS::rand() / 8;

    if (swos.foulXCoordinate >= kPitchCenterX)
        xOffset = -xOffset;

    int cameraY = static_cast<int>(getCameraY());
    int refStartY = cameraY - 20;

    if (swos.foulYCoordinate <= kPitchCenterY)
        refStartY = cameraY + 215;

    m_refereeSprite.x = m_refereeSprite.destX + xOffset;
    m_refereeSprite.y = refStartY;
    m_refereeSprite.speed = kRefereeSpeed;
    m_refereeSprite.show();

    initDisplaySprites();
    initRefereeAnimationTable(&swos.refComingAnimTable);

    swos.refState = kRefIncoming;
}

void SWOS::ActivateReferee()
{
    activateReferee();
}

bool refereeActive()
{
    return swos.refState != kRefOffScreen;
}

bool cardHandingInProgress()
{
    return swos.whichCard != kNoCard;
}

void updateReferee()
{
    if (refereeActive() && m_refereeSprite.visible) {
        if (m_refereeSprite.onScreen) {
            updateSpriteAnimation(m_refereeSprite);
            updateRefereeState();
        } else if (swos.gameStatePl == GameState::kInProgress) {
            moveSprite(m_refereeSprite);
        } else {
            updateRefereeState();
        }
    }
}

void updateBookedPlayerNumberSprite()
{
    m_bookedPlayerNumberSprite.clearImage();
    if (swos.whichCard && swos.refState == kRefBooking) {
        assert(swos.bookedPlayer);
        if (swos.bookedPlayer->state == PlayerState::kBooked) {
            static const std::array<int8_t, 30> kPlayerNumberBlinkTable = {
                0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1,
            };
            swos.refTimer += swos.lastFrameTicks;

            size_t index = swos.refTimer >> 3;
            assert(index < kPlayerNumberBlinkTable.size());
            int action = kPlayerNumberBlinkTable[index];

#ifdef SWOS_TEST
            // emulate SWOS bug: the table for second yellow card is 3 bytes short,
            // and accessing it randomly brings in the bytes from the array that follows it
            constexpr int kFaultySize = 27;
            if (swos.whichCard != kRedCard && swos.whichCard != kYellowCard && index >= kFaultySize) {
                static const std::array<int8_t, 3> kJunkBytes = { 96, 3, -98, };
                action = kJunkBytes[index - kFaultySize];
            }
#endif
            if (action > 0) {
                assert(swos.lastTeamBooked);
                const auto teamGame = swos.lastTeamBooked->inGameTeamPtr.asAligned();
                auto& player = teamGame->players[swos.bookedPlayer->playerOrdinal - 1];
                int imageIndex = kSmallDigit1 + player.shirtNumber - 1;
                m_bookedPlayerNumberSprite.setImage(imageIndex);
#ifdef SWOS_TEST
                m_bookedPlayerNumberSprite.x.setWhole(swos.bookedPlayer->x.whole());
                m_bookedPlayerNumberSprite.y.setWhole(swos.bookedPlayer->y.whole());
#else
                m_bookedPlayerNumberSprite.x = swos.bookedPlayer->x;
                m_bookedPlayerNumberSprite.y = swos.bookedPlayer->y;
#endif
                m_bookedPlayerNumberSprite.z = kPlayerNumberOffset;
            } else if (action < 0) {
                putRefereeToLeavingState();

                if (swos.whichCard & kRedCard)
                    sendPlayerAway();

                swos.whichCard = 0;
                swos.bookedPlayer.reset();
            }
        }
    }
}

Sprite *refereeSprite()
{
    return &m_refereeSprite;
}

Sprite *bookedPlayerNumberSprite()
{
    return &m_bookedPlayerNumberSprite;
}

static void removeReferee()
{
    swos.refState = kRefOffScreen;
    m_refereeSprite.hide();
#ifdef SWOS_TEST
    m_refereeSprite.x.setWhole(kRefereeHidingPlaceX);
    m_refereeSprite.y.setWhole(kRefereeHidingPlaceY);
#else
    m_refereeSprite.x = kRefereeHidingPlaceX;
    m_refereeSprite.y = kRefereeHidingPlaceY;
#endif
    m_refereeSprite.z = 0;
    m_refereeSprite.destX = kRefereeHidingPlaceX;
    m_refereeSprite.destY = kRefereeHidingPlaceY;
    m_refereeSprite.speed = 0;
    m_refereeSprite.playerDownTimer = 0;
    m_refereeSprite.frameIndex = -1;
    m_refereeSprite.cycleFramesTimer = 1;
    m_refereeSprite.clearImage();
    m_refereeSprite.direction = kFacingTop;
    m_refereeSprite.onScreen = 1;
    initRefereeAnimationTable(&swos.refWaitingAnimTable);
}

void SWOS::RemoveReferee()
{
    removeReferee();
}

static void updateRefereeState()
{
    switch (swos.refState) {
    case kRefAboutToGiveCard:
        assert(swos.whichCard != kNoCard);
        swos.refState = kRefBooking;

        switch (swos.whichCard) {
        case kRedCard:
            initRefereeAnimationTable(&swos.refRedCardAnimTable);
            enqueueRedCardSample();
            break;
        case kYellowCard:
            initRefereeAnimationTable(&swos.refYellowCardAnimTable);
            enqueueYellowCardSample();
            break;
        case kSecondYellowCard:
            initRefereeAnimationTable(&swos.refSecondYellowAnimTable);
            enqueueRedCardSample();
            break;
        }
        break;

    case kRefLeaving:
        if (!m_refereeSprite.onScreen) {
            swos.refState = kRefOffScreen;
            m_refereeSprite.hide();
            initDisplaySprites();
            break;
        }
        // fall-through

    case kRefIncoming:
        m_refereeSprite.speed = kRefereeSpeed;
        auto oldDirection = m_refereeSprite.direction;
        updateSpriteDirectionAndDeltas(m_refereeSprite);

#ifdef SWOS_TEST
        m_refereeSprite.fullDirection = 0;
#endif

        if (oldDirection != m_refereeSprite.direction)
            initRefereeAnimationTable(&swos.refComingAnimTable);

        moveSprite(m_refereeSprite);

        if (m_refereeSprite.stationary()) {
            swos.refState = kRefWaitingPlayer;
            m_refereeSprite.direction = kFacingLeft;
            initRefereeAnimationTable(&swos.refWaitingAnimTable);
        }
        break;
    }
}

static void putRefereeToLeavingState()
{
    int xOffset = SWOS::rand() / 4 - 32;    // -32..31
    m_refereeSprite.destX = m_refereeSprite.x.whole() + xOffset;

    int destY = swos.foulYCoordinate > kPitchCenterY ? kRefereeLeavingTopDestY : kRefereeLeavingBottomDestY;
    m_refereeSprite.destY = destY;

    initRefereeAnimationTable(&swos.refComingAnimTable);

    swos.refState = kRefLeaving;
}

static void sendPlayerAway()
{
    assert(swos.bookedPlayer && swos.lastTeamBooked);
    auto& player = *swos.bookedPlayer;

    auto gameTeam = swos.lastTeamBooked->inGameTeamPtr.asAligned();
    if (gameTeam->markedPlayer == player.playerOrdinal - 1)
        gameTeam->markedPlayer = -1;

    player.cards = -1;
    player.sentAway = 1;
    player.destX = kSentOffPlayerX;
    player.destY = kSentOffPlayerY;
}

static void initRefereeAnimationTable(SwosDataPointer<void> animTable)
{
    auto delay = animTable.as<RefereeAnimationTable *>()->numCycles;
    auto frameTable = animTable.asAligned<RefereeAnimationTable *>()->indicesTable;

    m_refereeSprite.frameDelay = delay;
    m_refereeSprite.frameIndicesTable = frameTable[m_refereeSprite.direction];
    assert(m_refereeSprite.frameIndicesTable.asAligned());

    m_refereeSprite.frameSwitchCounter = -1;
    m_refereeSprite.frameIndex = -1;
    m_refereeSprite.cycleFramesTimer = 1;
}
