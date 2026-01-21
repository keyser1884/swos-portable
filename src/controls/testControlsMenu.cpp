#include "testControlsMenu.h"
#include "menuBackground.h"
#include "text.h"

constexpr int kHeaderY = 20;
constexpr int kDescriptionY = 40;
constexpr int kAbortInfoY = 170;

constexpr int kControlsAndEventsY = 50;
constexpr int kControlsAndEventsLineHeight = 10;
constexpr int kNumControlsAndEventLines = (kAbortInfoY - kControlsAndEventsY) / kControlsAndEventsLineHeight;

constexpr int kCenterX = kMenuScreenWidth / 2;
constexpr int kMargin = 20;
constexpr int kColumn1X = kMargin + (kMenuScreenWidth - 2 * kMargin) / 4;
constexpr int kColumn2X = kMargin + 3 * (kMenuScreenWidth - 2 * kMargin) / 4;

constexpr char kNothingPressed[] = "N/A";

using namespace TestControlsMenu;

static void drawFixedPart(const char *title, const char *controlsTitle, bool allowEscapeExit)
{
    assert(controlsTitle[strlen(controlsTitle) - 1] == ':');

    drawMenuBackground();
    drawTextCentered(kCenterX, kHeaderY, title);
    drawTextCentered(kColumn1X, kDescriptionY, controlsTitle);
    drawTextCentered(kColumn2X, kDescriptionY, "EVENTS TRIGGERING:");
#ifdef __ANDROID__
    drawTextCentered(kCenterX, kAbortInfoY, allowEscapeExit ? "(TAP/BACK EXITS)" : "(TAP EXITS)");
#else
    drawTextCentered(kCenterX, kAbortInfoY, allowEscapeExit ? "(MOUSE CLICK/ESCAPE EXITS)" : "(MOUSE CLICK EXITS)");
#endif
    updateScreen();
}

static void drawControls(GetControlsFn getControls)
{
    int y = kControlsAndEventsY;
    bool shorten = false;

    auto controls = getControls();
    int numControlsToShow = controls.size();

    if (controls.empty()) {
        drawTextCentered(kColumn1X, y, kNothingPressed);
        return;
    }

    if (numControlsToShow > kNumControlsAndEventLines) {
        shorten = true;
        numControlsToShow = kNumControlsAndEventLines - 1;
    }

    while (numControlsToShow--) {
        drawTextCentered(kColumn1X, y, controls[numControlsToShow].c_str());
        y += kControlsAndEventsLineHeight;
    }

    if (shorten)
        drawTextCentered(kColumn1X, y, "...");
}

static void drawEvents(GetEventsFn getEvents)
{
    auto events = getEvents();
    int y = kControlsAndEventsY;

    if (!events) {
        drawTextCentered(kColumn2X, y, kNothingPressed);
    } else {
        traverseEvents(events, [&y](auto event) {
            auto str = gameControlEventToString(event);
            drawTextCentered(kColumn2X, y, str.first);
            y += kControlsAndEventsLineHeight;
        });
    }
}

static bool aborted(bool allowEscapeExit)
{
    bool aborted = std::get<0>(mouseClickAndRelease());

    if (!aborted && allowEscapeExit) {
        auto keys = SDL_GetKeyboardState(nullptr);
        aborted = keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_AC_BACK];
    }

    return aborted;
}

void showTestControlsMenu(const char *title, const char *controlsTitle, bool allowEscapeExit,
    GetControlsFn getControls, GetEventsFn getEvents)
{
    assert(title && controlsTitle && getControls && getEvents);

    drawFixedPart(title, controlsTitle, allowEscapeExit);

    waitForKeyboardAndMouseIdle();

    while (!aborted(allowEscapeExit)) {
        drawMenuBackground();
        drawControls(getControls);
        drawEvents(getEvents);
        updateScreen();

        processControlEvents();
        SDL_Delay(50);
    }
}
