#include "portedFunctionsMenu.h"
#include "portedFunctions.h"
#include "menus.h"
#include "drawMenu.h"

static constexpr int kVisibleToggles = 12;
static int m_scrollOffset = 0;

#include "portedFunctions.mnu.h"

using namespace PortedFunctionsMenu;

void showPortedFunctionsMenu()
{
    m_scrollOffset = 0;
    showMenu(portedFunctionsMenu);
}

static void portedFunctionsMenuOnInit()
{
    m_scrollOffset = 0;
}

static void portedFunctionsMenuOnReturn()
{
}

static void updateScrollInfo()
{
    auto entry = A5.asMenuEntry();
    int first = m_scrollOffset + 1;
    int last = std::min(m_scrollOffset + kVisibleToggles, kNumPortedFunctionToggles);
    char buf[32];
    sprintf(buf, "%d-%d/%d", first, last, kNumPortedFunctionToggles);
    entry->copyString(buf);
}

static int getRowIndexFromEntry(MenuEntry* entry)
{
    // Calculate which row this entry represents based on its ordinal
    // Labels start at kFirstLabelEntry, toggles at kFirstToggleEntry
    int ordinal = entry->ordinal;

    if (ordinal >= kFirstToggleEntry && ordinal < kFirstToggleEntry + kVisibleToggles) {
        return ordinal - kFirstToggleEntry;
    }
    if (ordinal >= kFirstLabelEntry && ordinal < kFirstLabelEntry + kVisibleToggles) {
        return ordinal - kFirstLabelEntry;
    }
    return -1;
}

static int getToggleIndex(int rowIndex)
{
    return m_scrollOffset + rowIndex;
}

static const ToggleInfo* getToggleInfoForEntry(MenuEntry* entry)
{
    int rowIndex = getRowIndexFromEntry(entry);
    if (rowIndex < 0) return nullptr;

    int toggleIndex = getToggleIndex(rowIndex);
    if (toggleIndex >= 0 && toggleIndex < kNumPortedFunctionToggles) {
        return &getPortedFunctionToggleInfo()[toggleIndex];
    }
    return nullptr;
}

static void drawToggleLabel()
{
    auto entry = A5.asMenuEntry();
    auto info = getToggleInfoForEntry(entry);
    if (info) {
        entry->copyString(info->name);
        entry->show();
    } else {
        entry->copyString("");
        entry->hide();
    }
}

static void drawToggleValue()
{
    auto entry = A5.asMenuEntry();
    auto info = getToggleInfoForEntry(entry);
    if (info) {
        entry->copyString(*info->toggle ? "C++" : "ASM");
        entry->show();
    } else {
        entry->copyString("");
        entry->hide();
    }
}

static void selectToggle()
{
    auto entry = A5.asMenuEntry();
    auto info = getToggleInfoForEntry(entry);
    if (info) {
        *info->toggle = !*info->toggle;
    }
}

static void doScrollUp()
{
    if (m_scrollOffset > 0) {
        m_scrollOffset--;
    }
}

static void doScrollDown()
{
    int maxOffset = kNumPortedFunctionToggles - kVisibleToggles;
    if (m_scrollOffset < maxOffset) {
        m_scrollOffset++;
    }
}

static void doEnableAllCpp()
{
    auto toggles = getPortedFunctionToggleInfo();
    for (int i = 0; i < kNumPortedFunctionToggles; i++) {
        *toggles[i].toggle = true;
    }
}

static void doEnableAllAsm()
{
    auto toggles = getPortedFunctionToggleInfo();
    for (int i = 0; i < kNumPortedFunctionToggles; i++) {
        *toggles[i].toggle = false;
    }
}

static void doSaveAndExit()
{
    savePortedFunctionToggles();
    SetExitMenuFlag();
}

static void drawCompareToggle()
{
    auto entry = A5.asMenuEntry();
    if (g_comparePortedFunctions) {
        entry->copyString("COMPARE: ON");
    } else {
        entry->copyString("COMPARE: OFF");
    }
}

static void toggleCompareMode()
{
    g_comparePortedFunctions = !g_comparePortedFunctions;
    if (g_comparePortedFunctions) {
        initComparisonLog();
    } else {
        closeComparisonLog();
    }
}

static void drawCompareStatus()
{
    auto entry = A5.asMenuEntry();
    char buf[64];
    if (g_comparePortedFunctions) {
        sprintf(buf, "Mismatches: %d", g_comparisonMismatchCount);
    } else {
        sprintf(buf, "(disabled)");
    }
    entry->copyString(buf);
}
