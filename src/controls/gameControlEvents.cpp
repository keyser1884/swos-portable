#include "gameControlEvents.h"

std::pair<const char *, size_t> gameControlEventToString(GameControlEvents event)
{
    static const std::tuple<GameControlEvents, const char *, size_t> kEventsToStrings[] = {
        { kGameEventUp, "UP", 2 },
        { kGameEventDown, "DOWN", 4 },
        { kGameEventLeft, "LEFT", 4 },
        { kGameEventRight, "RIGHT", 5 },
        { kGameEventKick, "KICK", 4 },
        { kGameEventBench, "BENCH", 5 },
        { kGameEventPause, "PAUSE", 5 },
        { kGameEventReplay, "REPLAY", 6 },
        { kGameEventSaveHighlight, "SAVE HIGHLIGHT", 14 },
        { kGameEventZoomIn, "ZOOM IN", 7 },
        { kGameEventZoomOut, "ZOOM OUT", 8 },
    };

    for (const auto& ev : kEventsToStrings) {
        if (event & std::get<0>(ev))
            return { std::get<1>(ev), std::get<2>(ev) };
    }

    assert(false);
    return { "", 0 };
}

int gameControlEventToString(GameControlEvents events, char *buffer, size_t bufferSize, const char *noneString /* = "N/A" */)
{
    static_assert(kMaxGameEvent == 1'025, "action event stringifying needs an update");

    *buffer = '\0';
    auto start = buffer;
    auto sentinel = buffer + bufferSize;

    strncpy_s(buffer, noneString, bufferSize);

    auto event = static_cast<GameControlEvents>(1);
    while (event <= kMaxGameEvent) {
        if (events & event) {
            const char *eventName;
            size_t eventNameLength;
            std::tie(eventName, eventNameLength) = gameControlEventToString(event);

            strncpy_s(buffer, eventName, sentinel - buffer);
            buffer += eventNameLength;

            if (sentinel - buffer > 1) {
                memcpy(buffer, "/", 2);
                buffer += 1;
            } else {
                return buffer - start;
            }
        }

        event <<= 1;
    }

    if (buffer - 1 >= sentinel - bufferSize)
        *--buffer = '\0';

    return buffer - start;
}

bool convertEvents(GameControlEvents& events, int intEvents)
{
    events = static_cast<GameControlEvents>(intEvents);
    return events <= kMaxGameEvent;
}

void traverseEvents(GameControlEvents events, std::function<void(GameControlEvents)> f)
{
    for (int i = 1; i <= kMaxGameEvent; i <<= 1) {
        if (events & i)
            f(static_cast<GameControlEvents>(i));
    }
}
