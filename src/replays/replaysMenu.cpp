#include "replaysMenu.h"
#include "replays.h"
#include "ReplayDataStorage.h"
#include "continueMenu.h"
#include "menuMouse.h"
#include "selectFilesMenu.h"
#include "continueAbortMenu.h"
#include "replays.mnu.h"

using namespace ReplaysMenu;

using EntryList = std::array<int, 2>;

static void updateMenuButtonsState();
static void updateReplaysAndHighlightsState();
static void updateButtonState(bool enabled, const EntryList& entries);
static void showError(FileStatus error);
static bool promptOverwrite(const char *filename);

void showReplaysMenu()
{
    showMenu(replaysMenu);
    CommonMenuExit();
}

static void replaysMenuOnInit()
{
    updateMenuButtonsState();
}

static void playHighlights()
{
    playHighlights(false);
    updateMenuButtonsState();
}

static void selectHighlightToLoad()
{
    auto files = findFiles(".hil", kHighlightsDir);
    auto menuTitle = "LOAD HIGHLIGHTS";
    auto selectedFilename = showSelectFilesMenu(menuTitle, files);

    if (!selectedFilename.empty()) {
        auto status = loadHighlightsFile(selectedFilename.c_str());
        if (status == FileStatus::kOk)
            highlightEntry(viewHighlights);
        else
            showError(status);
    }

    updateReplaysAndHighlightsState();
}

static void selectHighlightToSave()
{
    auto files = findFiles(".hil", kHighlightsDir);
    auto menuTitle = "SAVE HIGHLIGHTS";

    char hilFilename[kMaxFilenameLength] = {};
    showSelectFilesMenu(menuTitle, files, ".hil", hilFilename);

    if (hilFilename[0]) {
        auto path = joinPaths(kHighlightsDir, hilFilename);
        if (promptOverwrite(hilFilename) && !saveHighlightsFile(path.c_str()))
            showErrorMenu("ERROR SAVING HIGHLIGHTS FILE");
    }
}

static void playReplay()
{
    playFullReplay();
    updateMenuButtonsState();
}

static void selectReplayToLoad()
{
    auto files = findFiles(".rpl", kReplaysDir);
    auto menuTitle = "LOAD REPLAY";
    auto selectedFilename = showSelectFilesMenu(menuTitle, files);

    if (!selectedFilename.empty()) {
        auto status = loadReplayFile(selectedFilename.c_str());
        if (status == FileStatus::kOk)
            highlightEntry(viewReplays);
        else
            showError(status);
    }

    updateReplaysAndHighlightsState();
}

static void selectReplayToSave()
{
    auto files = findFiles(".rpl", kReplaysDir);
    auto menuTitle = "SAVE REPLAYS";

    char rplFilename[kMaxFilenameLength] = {};
    showSelectFilesMenu(menuTitle, files, ".rpl", rplFilename);
    if (rplFilename[0]) {
        auto path = joinPaths(kReplaysDir, rplFilename);
        if (promptOverwrite(rplFilename) && !saveReplayFile(path.c_str()))
            showErrorMenu("ERROR SAVING REPLAY FILE");
    }
}

static void updateMenuButtonsState()
{
    updateReplaysAndHighlightsState();
    determineReachableEntries();

    if (getMenuEntry(viewHighlights)->disabled)
        highlightEntry(viewReplaysEntry.disabled ? loadHighlights : viewReplays);
}

static void updateReplaysAndHighlightsState()
{
    updateButtonState(gotHighlights(), { viewHighlights, saveHighlights });
    updateButtonState(gotReplay(), { viewReplays, saveReplays });
}

static void updateButtonState(bool enabled, const EntryList& entries)
{
    int color = enabled ? kGreen : kGray;

    for (auto entryIndex : entries) {
        auto entry = getMenuEntry(entryIndex);
        entry->setEnabled(enabled);
        entry->setBackgroundColor(color);
    }
}

static void showError(FileStatus error)
{
    switch (error) {
    case FileStatus::kOk:
        assert(false);
        break;
    case FileStatus::kCorrupted:
        showErrorMenu("THE FILE IS CORRUPTED");
        break;
    case FileStatus::kUnsupportedVersion:
        showErrorMenu("UNSUPPORTED FILE VERSION");
        break;
    case FileStatus::kIoError:
        showErrorMenu("FILE I/O ERROR");
        break;
    }
}

static bool promptOverwrite(const char *filename)
{
    return showContinueAbortPrompt("CONFIRM OVERWRITE", "YES", "NO",
        { "ARE YOU SURE YOU WANT TO OVERWRITE", filename }, true);
}
