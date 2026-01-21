#include "selectGameControlEventsMenu.h"
#include "text.h"
#include "selectGameControlEvents.mnu.h"

using namespace SelectGameControlEvents;

static_assert(lastEventStatus - firstEventStatus == lastEventToggle - firstEventToggle, "Menu error");

static const char *m_title;
static int m_index;
SetGameControlEventsFunction m_setFunction;
GetGameControlEventsFunction m_getFunction;
GameControlEventsDisconnectedFunction m_disconnectedFunction;
static bool m_addNew;
static bool m_showInverted;
static bool m_inverted;

static bool m_success;
static GameControlEvents m_events;

constexpr int kWarningTicks = 2'000;
static Uint32 m_showWarningTicks;

static const std::vector<int> kPrevExitNextButtons = { prev, SelectGameControlEvents::exit, next };

static_assert(kMaxGameEvent == 1'025, "New game event added, menu needs redressing!");

std::pair<bool, GameControlEvents> getNewGameControlEvents(const char *title)
{
    assert(title);

    m_events = kNoGameEvents;
    m_success = false;

    m_title = title;
    m_addNew = true;
    m_showInverted = false;

    showMenu(selectGameControlEvents);

    return { m_success, m_events };
}

void updateGameControlEvents(int index, SetGameControlEventsFunction setEvents, GetGameControlEventsFunction getEvents,
    GameControlEventsDisconnectedFunction disconnected /* = {} */, bool showInverted /* = false */)
{
    assert(setEvents && getEvents);

    m_events = kNoGameEvents;
    m_success = false;

    m_title = nullptr;
    m_index = index;
    m_getFunction = getEvents;
    m_setFunction = setEvents;
    m_disconnectedFunction = disconnected;
    m_showInverted = showInverted;
    m_addNew = false;

    showMenu(selectGameControlEvents);
}

static void setHeaderAndLabel();
static void initExitButtons();
static void initEventEntries();
static void updateCurrentData();
static void initEventEntriesColorAndStatus();
static void initInvertedButton();

static void selectGameControlEventsOnInit()
{
    setHeaderAndLabel();
    initExitButtons();
    initEventEntries();
}

static void selectGameControlEventsOnRestore()
{
    initEventEntries();
}

static void selectGameControlEventsOnDraw()
{
    auto keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_AC_BACK])
        m_addNew ? onCancel() : SetExitMenuFlag();
    else if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2])
        m_addNew ? onOk() : SetExitMenuFlag();

    if (m_disconnectedFunction && m_disconnectedFunction())
        SetExitMenuFlag();
}

static void initEventEntries()
{
    updateCurrentData();
    initEventEntriesColorAndStatus();
    initInvertedButton();
    m_showWarningTicks = 0;
}

static void updateCurrentData()
{
    if (!m_addNew) {
        auto titleBuffer = eventHeaderLabelEntry.string();
        std::tie(m_events, m_inverted) = m_getFunction(m_index, titleBuffer, kStdMenuTextSize);
    }
}

static void initEventEntriesColorAndStatus()
{
    for (int i = firstEventToggle; i <= lastEventToggle; i++) {
        auto event = 1 << (i - firstEventToggle);
        bool on = (event & m_events) != 0;

        auto toggleEntry = getMenuEntry(i);
        toggleEntry->setBackgroundColor(on ? kOnColor : kOffColor);

        auto statusEntry = getMenuEntry(i - firstEventToggle + firstEventStatus);
        statusEntry->setBackgroundColor(on ? kOnColor : kOffColor);
        statusEntry->copyString(on ? "ON" : "OFF");
    }
}

static void setHeaderAndLabel()
{
    copyStringToEntry(header, m_addNew ? "SET EVENTS" : "UPDATE EVENTS");
    if (m_addNew)
        copyStringToEntry(eventHeaderLabel, m_title);
}

static void changeEntryReferences(const std::vector<std::pair<int, int>>& fromToPairs)
{
    for (auto entry = getCurrentMenu()->entries(); entry->ordinal <= lastEventStatus; entry++) {
        for (const auto& pair : fromToPairs) {
            assert(&entry->leftEntry + 3 == &entry->downEntry);
            for (byte *nextEntry = &entry->leftEntry; nextEntry <= &entry->downEntry; nextEntry++) {
                if (*nextEntry == pair.first)
                    *nextEntry = pair.second;
            }
        }
    }
}

static void initExitButtons()
{
    static const std::vector<int> kOkCancelButtons = { ok, cancel };

    if (m_addNew) {
        setEntriesVisibility(kOkCancelButtons, true);
        setEntriesVisibility(kPrevExitNextButtons, false);
        changeEntryReferences({ { prev, ok }, { SelectGameControlEvents::exit, ok }, { next, cancel } });
    } else {
        setEntriesVisibility(kOkCancelButtons, false);
        setEntriesVisibility(kPrevExitNextButtons, true);
        changeEntryReferences({ { ok, prev }, { cancel, next } });
    }
}

static void setRow(MenuEntry *statusEntry, MenuEntry *toggleEntry, bool turningOn)
{
    toggleEntry->setBackgroundColor(turningOn ? kOnColor : kOffColor);

    statusEntry->copyString(turningOn ? "ON" : "OFF");
    statusEntry->setBackgroundColor(turningOn ? kOnColor : kOffColor);
}

static void setInvertedEntry()
{
    assert(m_showInverted);

    auto invertedStatusEntry = getMenuEntry(invertedStatus);
    auto invertedToggleEntry = getMenuEntry(invertedToggle);

    setRow(invertedStatusEntry, invertedToggleEntry, m_inverted);
}

static void shiftExitButtonsDown()
{
    for (auto entryIndex : kPrevExitNextButtons) {
        auto entry = getMenuEntry(entryIndex);
        entry->y = invertedStatusEntry.endY() + kLowerButtonsGap;
    }
}

static void initInvertedButton()
{
    static const std::vector<int> kInvertedEntries = { invertedStatus, invertedToggle };

    if (m_showInverted) {
        setEntriesVisibility(kInvertedEntries, true);

        zoomOutStatusEntry.downEntry = invertedStatus;
        zoomOutToggleEntry.downEntry = invertedToggle;
        benchStatusEntry.rightEntry = invertedToggle;
        benchStatusEntry.downEntry = SelectGameControlEvents::exit;

        setInvertedEntry();
        shiftExitButtonsDown();
    } else if (!m_addNew) {
        benchStatusEntry.downEntry = SelectGameControlEvents::exit;
    }
}

static void toggleEvent(MenuEntry *toggleEntry, MenuEntry *statusEntry, int index)
{
    bool turningOn = toggleEntry->backgroundColor() == kOffColor;
    setRow(statusEntry, toggleEntry, turningOn);

    auto currentEvent = 1 << index;

    if (turningOn) {
        getMenuEntry(warning)->hide();
        m_events |= currentEvent;
    } else {
        m_events &= ~currentEvent;
    }

    if (!m_addNew)
        m_setFunction(m_index, m_events, m_inverted);
}

static void toggleEvent()
{
    auto toggleEntry = A5.asMenuEntry();

    int index = toggleEntry->ordinal - firstEventToggle;
    assert(index >= 0);

    auto statusEntry = getMenuEntry(firstEventStatus + index);

    toggleEvent(toggleEntry, statusEntry, index);
}

static void toggleEventFromStatus()
{
    auto statusEntry = A5.asMenuEntry();

    int index = statusEntry->ordinal - firstEventStatus;
    assert(index >= 0);

    auto toggleEntry = getMenuEntry(firstEventToggle + index);

    toggleEvent(toggleEntry, statusEntry, index);
}

static void toggleInverted()
{
    assert(m_showInverted);
    assert(m_inverted == (strcmp(getMenuEntry(invertedStatus)->string(), "ON") == 0));

    m_inverted = !m_inverted;

    m_setFunction(m_index, m_events, m_inverted);
    setInvertedEntry();
}

static void showWarning(const char *str)
{
    assert(str);

    warningEntry.copyString(str);
    m_showWarningTicks = SDL_GetTicks();
    warningEntry.show();
}

static void onOk()
{
    if (!m_events) {
        showWarning("PLEASE SELECT AT LEAST ONE EVENT");
    } else {
        m_success = true;
        SetExitMenuFlag();
    }
}

static void onCancel()
{
    m_success = false;
    SetExitMenuFlag();
}

static void updateWarning()
{
    if (m_showWarningTicks + kWarningTicks < SDL_GetTicks())
        A5.asMenuEntry()->hide();
}

static void selectPreviousEvent()
{
    if (m_index > 0) {
        m_index--;
        initEventEntries();
    }
}

static void selectNextEvent()
{
    m_index++;
    initEventEntries();
}
