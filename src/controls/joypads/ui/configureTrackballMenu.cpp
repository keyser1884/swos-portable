#include "configureTrackballMenu.h"
#include "joypads.h"
#include "gameControlEvents.h"
#include "selectGameControlEventsMenu.h"
#include "configureTrackball.mnu.h"

static int m_joypadIndex;
static SDL_JoystickID m_joypadId;
static int m_ballIndex;
static int m_numBalls;
static JoypadConfig::BallBinding *m_currentBall;

using namespace ConfigureTrackballMenu;

void showConfigureTrackballMenu(int joypadIndex, int ballIndex)
{
    m_joypadIndex = joypadIndex;
    m_ballIndex = ballIndex;

    showMenu(configureTrackballMenu);
}

static void initMenu();
static void setTrackballLabel();
static void setEventFields();
static bool exitIfDisconnected();

static void configureTrackballMenuOnInit()
{
    m_joypadId = joypadId(m_joypadIndex);
    m_numBalls = joypadNumBalls(m_joypadIndex);
    m_currentBall = &joypadBall(m_joypadIndex, m_ballIndex);

    if (!exitIfDisconnected())
        initMenu();
}

static void configureTrackballMenuOnRestore()
{
    if (!exitIfDisconnected())
        initMenu();
}

static void configureTrackballMenuOnDraw()
{
    exitIfDisconnected();
}

static void selectEvents()
{
    auto entry = A5.asMenuEntry();
    int index = entry->ordinal - xPosEvent;

    assert(index >= 0 && index <= 3);

    auto getMovementAndEvents = [](int index) {
        assert(index >= 0 && index <= 3);
        switch (index) {
        default: assert(false);
        case 0: return std::make_pair("X+", &m_currentBall->xPosEvents);
        case 1: return std::make_pair("X-", &m_currentBall->xNegEvents);
        case 2: return std::make_pair("Y+", &m_currentBall->yPosEvents);
        case 3: return std::make_pair("Y-", &m_currentBall->yNegEvents);
        }
    };

    auto setFunction = [getMovementAndEvents](int index, GameControlEvents events, bool) {
        auto destEvents = getMovementAndEvents(index).second;
        *destEvents = events;
    };

    auto getFunction = [getMovementAndEvents](int& index, char *titleBuf, int titleBufSize) {
        index = std::min(index, 3);

        const char *movement;
        GameControlEvents *events;
        std::tie(movement, events) = getMovementAndEvents(index);

        snprintf(titleBuf, titleBufSize, "EVENTS FOR TRACKBALL %d %s MOVEMENT", m_ballIndex, movement);
        return std::make_pair(*events, false);
    };

    updateGameControlEvents(index, setFunction, getFunction);
    setEventFields();
}

static void goToPreviousBall()
{
    if (m_ballIndex > 0) {
        m_currentBall = &joypadBall(m_joypadIndex, --m_ballIndex);
        initMenu();
    }
}

static void goToNextBall()
{
    if (m_ballIndex < m_numBalls - 1) {
        m_currentBall = &joypadBall(m_joypadIndex, ++m_ballIndex);
        initMenu();
    }
}

static void initMenu()
{
    setTrackballLabel();
    setEventFields();
}

static void setTrackballLabel()
{
    auto buf = getMenuEntry(trackballLabel)->string();
    snprintf(buf, kStdMenuTextSize, "TRACKBALL %d %s", m_ballIndex + 1, joypadName(m_joypadIndex));
}

static void setEventFields()
{
    assert(m_currentBall);

    gameControlEventToString(m_currentBall->xPosEvents, getMenuEntry(xPosEvent)->string(), kStdMenuTextSize);
    gameControlEventToString(m_currentBall->xNegEvents, getMenuEntry(xNegEvent)->string(), kStdMenuTextSize);
    gameControlEventToString(m_currentBall->yPosEvents, getMenuEntry(yPosEvent)->string(), kStdMenuTextSize);
    gameControlEventToString(m_currentBall->yNegEvents, getMenuEntry(yNegEvent)->string(), kStdMenuTextSize);
}

static bool exitIfDisconnected()
{
    if (joypadDisconnected(m_joypadId)) {
        SetExitMenuFlag();
        return true;
    }

    return false;
}
