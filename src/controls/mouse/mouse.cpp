#include "mouse.h"

static GameControlEvents m_events;
static int m_longKickFrames;

static bool longKickRunning()
{
    return m_longKickFrames > 0;
}

static GameControlEvents getOppositeDirections(GameControlEvents events)
{
    auto oppositeDirections = kNoGameEvents;

    if (events & kGameEventLeft)
        oppositeDirections |= kGameEventRight;
    if (events & kGameEventRight)
        oppositeDirections |= kGameEventLeft;
    if (events & kGameEventUp)
        oppositeDirections |= kGameEventDown;
    if (events & kGameEventDown)
        oppositeDirections |= kGameEventUp;

    return oppositeDirections;
}

GameControlEvents mouseEvents()
{
    if (longKickRunning() && !--m_longKickFrames)
        return getOppositeDirections(m_events) | kGameEventKick;

    return m_events;
}

void resetMouse()
{
    m_events = kNoGameEvents;
    m_longKickFrames = 0;
}

static void startLongKick()
{
    static constexpr int kLongKickFrames = 10;  // can't be less

    m_longKickFrames = kLongKickFrames;
    m_events |= kGameEventKick;
}

void updateMouseButtons(Uint8 button, Uint8 state)
{
    if (longKickRunning())
        return;

    if (state == SDL_PRESSED) {
        switch (button) {
        case SDL_BUTTON_LEFT:
            if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_RMASK)
                m_events |= kGameEventBench;
            else
                m_events |= kGameEventKick;
            break;

        case SDL_BUTTON_RIGHT:
            if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK)
                m_events |= kGameEventBench;
            else
                startLongKick();
            break;

        case SDL_BUTTON_MIDDLE:
            m_events = kNoGameEvents;
            break;
        }
    } else {
        assert(state == SDL_RELEASED);

        switch (button) {
        case SDL_BUTTON_LEFT:
            m_events &= ~(kGameEventKick | kGameEventBench);
            break;

        case SDL_BUTTON_RIGHT:
            m_events &= ~kGameEventBench;
            break;
        }
    }
}

void updateMouseMovement(Sint32 xrel, Sint32 yrel, Uint32 state)
{
    if (longKickRunning())
        return;

    m_events = kNoGameEvents;

    if (state & SDL_BUTTON_MMASK)
        return;

    if (xrel < 0)
        m_events |= kGameEventLeft;

    if (xrel > 0)
        m_events |= kGameEventRight;

    if (yrel < 0)
        m_events |= kGameEventUp;

    if (yrel > 0)
        m_events |= kGameEventDown;

    if ((state & SDL_BUTTON_LMASK) && (state & SDL_BUTTON_RMASK))
        m_events |= kGameEventBench;
    else if (state & SDL_BUTTON_LMASK)
        m_events |= kGameEventKick;
    else if ((state & SDL_BUTTON_RMASK) && !longKickRunning())
        startLongKick();
}
