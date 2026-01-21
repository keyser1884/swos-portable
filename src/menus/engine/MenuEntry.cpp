#include "MenuEntry.h"
#include "log.h"

namespace {

// Helper to safely get string pointer, handling both VM offsets and native pointers
// that may have been stored directly by assembly code (like CareerGameListCommon)
const char* safeGetStringPtr(const SwosDataPointer<char>& ptr)
{
    auto raw = ptr.getRaw();

    // Check if it looks like an unregistered native pointer:
    // - Not null (0)
    // - Not sentinel (-1 / 0xFFFFFFFF)
    // - Doesn't have kExternalPointerMask (0x80000000) set
    // - Is above the valid VM offset range (~600KB)
    constexpr uint32_t kExternalPointerMask = 1u << 31;
    constexpr uint32_t kMaxValidVmOffset = 700000;

    if (raw != 0 &&
        raw != static_cast<uint32_t>(-1) &&
        !(raw & kExternalPointerMask) &&
        raw > kMaxValidVmOffset) {
        // This is a native pointer stored directly by assembly code
        logWarn("MenuEntry::string: Detected unregistered native pointer 0x%x", raw);
        return reinterpret_cast<const char*>(static_cast<uintptr_t>(raw));
    }

    // For all other cases, use the normal path
    return ptr.asAlignedConstCharPtr();
}

char* safeGetStringPtr(SwosDataPointer<char>& ptr)
{
    return const_cast<char*>(safeGetStringPtr(const_cast<const SwosDataPointer<char>&>(ptr)));
}

}

int MenuEntry::centerX() const
{
    return x + width / 2;
}

int MenuEntry::endX() const
{
    return x + width;
}

int MenuEntry::endY() const
{
    return y + height;
}

const char *MenuEntry::typeToString() const
{
    switch (type) {
    case kEntryNoForeground: return "empty";
    case kEntryContentFunction: return "function";
    case kEntryString: return "string";
    case kEntrySprite2: return "sprite";
    case kEntryStringTable: return "string table";
    case kEntryMultilineText: return "multi-line string";
    case kEntryNumber: return "number";
    case kEntryColorConvertedSprite: return "color converted sprite";
    default: assert(false); return "";
    }
}

bool MenuEntry::visible() const
{
    return !invisible;
}

void MenuEntry::hide()
{
    invisible = 1;
}

void MenuEntry::show()
{
    invisible = 0;
}

void MenuEntry::setVisible(bool visible)
{
    invisible = !visible;
}

void MenuEntry::disable()
{
    disabled = 1;
}

void MenuEntry::enable()
{
    disabled = 0;
}

void MenuEntry::setEnabled(bool enabled)
{
    disabled = !enabled;
}

bool MenuEntry::selectable() const
{
    return !invisible && !disabled;
}

bool MenuEntry::isString() const
{
    return type == kEntryString;
}

char *MenuEntry::string()
{
    assert(type == kEntryString);
    return safeGetStringPtr(fg.string);
}

const char *MenuEntry::string() const
{
    assert(type == kEntryString);
    return safeGetStringPtr(fg.string);
}

void MenuEntry::setString(char *str)
{
    assert(type == kEntryString);
    fg.string.set(str);
}

void MenuEntry::setString(const char *str)
{
    assert(type == kEntryString);
    fg.string.set(const_cast<char *>(str));
}

// Copies string to entry's text and removes spaces from beginning and end.
void MenuEntry::copyString(const char *str)
{
    assert(type == kEntryString && str);

    while (*str == ' ')
        str++;

    auto start = string();
    auto end = start + kStdMenuTextSize;
    auto p = start;

    while (p < end && *str)
        *p++ = *str++;

    if (p < end)
        *p = '\0';
    else
        *--p = '\0';

    while (--p >= start && *p == ' ')
        *p = '\0';
}

bool MenuEntry::bigFont() const
{
    assert(type == kEntryString || type == kEntryStringTable || type == kEntryMultilineText ||
        type == kEntryNumber || type == kEntryBoolOption);
    return ((stringFlags >> 4) & 1) != 0;
}

void MenuEntry::setNumber(int number)
{
    assert(type == kEntryNumber);
    fg.number = number;
}

void MenuEntry::setSprite(int index)
{
    assert(type == kEntrySprite2 || type == kEntryMenuSpecificSprite);
    fg.spriteIndex = index;
}

void MenuEntry::setBackgroundColor(int color)
{
    bg.entryColor = color;
}

void MenuEntry::setFrameColor(int color)
{
    bg.entryColor = (bg.entryColor & 0x0f) | (color << 4);
}

word MenuEntry::backgroundColor() const
{
    assert(background == kEntryFrameAndBackColor);
    return bg.entryColor & 0x0f;
}

word MenuEntry::innerFrameColor() const
{
    assert(background == kEntryFrameAndBackColor);
    return (bg.entryColor >> 4) & 0x0f;
}

int MenuEntry::solidTextColor() const
{
    assert(type == kEntryString || type == kEntryStringTable || type == kEntryMultilineText ||
        type == kEntryNumber || type == kEntryBoolOption);
    int color = stringFlags & 0xf;
    return color ? color : kWhiteText2;
}
