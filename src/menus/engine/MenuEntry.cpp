#include "MenuEntry.h"
#include "log.h"

namespace {

// Helper to safely get string pointer, handling both VM offsets and native pointers
// that may have been stored directly by assembly code (like CareerGameListCommon)
char* safeGetStringPtr(SwosDataPointer<char>& ptr)
{
    auto raw = ptr.getRaw();

    // Handle null and sentinel values first
    if (raw == 0)
        return nullptr;

    if (raw == static_cast<uint32_t>(-1))
        return const_cast<char*>(kSentinel);

    // Check if it's a registered external pointer (has kExternalPointerMask set)
    constexpr uint32_t kExternalPointerMask = 1u << 31;
    if (raw & kExternalPointerMask) {
        // Use the normal path which handles external pointers via the pointer pool
        return ptr.asAlignedCharPtr();
    }

    // Check if it's a valid VM offset (should be < ~600KB for normal SWOS data)
    constexpr uint32_t kMaxValidVmOffset = 700000;
    if (raw <= kMaxValidVmOffset) {
        // Valid VM offset, use normal path
        return ptr.asAlignedCharPtr();
    }

    // Value is > kMaxValidVmOffset and doesn't have kExternalPointerMask
    // This is likely garbage data from partial 2-byte writes. Return empty string
    // instead of nullptr to avoid crashes in rendering code.
    return const_cast<char*>("");
}

const char* safeGetStringPtrConst(const SwosDataPointer<char>& ptr)
{
    auto raw = ptr.getRaw();

    // Handle null and sentinel values first
    if (raw == 0)
        return nullptr;

    if (raw == static_cast<uint32_t>(-1))
        return kSentinel;

    // Check if it's a registered external pointer (has kExternalPointerMask set)
    constexpr uint32_t kExternalPointerMask = 1u << 31;
    if (raw & kExternalPointerMask) {
        return ptr.asAlignedConstCharPtr();
    }

    // Check if it's a valid VM offset
    constexpr uint32_t kMaxValidVmOffset = 700000;
    if (raw <= kMaxValidVmOffset) {
        return ptr.asAlignedConstCharPtr();
    }

    // Value is > kMaxValidVmOffset - return empty string to avoid crashes
    return "";
}

}

// Common validation for SwosDataPointer raw values - returns true if pointer is likely valid
static bool isValidVmPointerRaw(uint32_t raw)
{
    // Null is valid (indicates no data)
    if (raw == 0)
        return true;

    // External pointers have kExternalPointerMask set
    constexpr uint32_t kExternalPointerMask = 1u << 31;
    if (raw & kExternalPointerMask)
        return true;

    // Valid VM offsets should be < ~700KB for normal SWOS data
    constexpr uint32_t kMaxValidVmOffset = 700000;
    return raw <= kMaxValidVmOffset;
}

// Helper to safely get multiline text pointer, handling corrupted values from partial writes
const char* safeGetMultilineText(const SwosDataPointer<MultilineText>& ptr)
{
    auto raw = ptr.getRaw();

    if (!isValidVmPointerRaw(raw))
        return nullptr;

    if (raw == 0)
        return nullptr;

    return ptr.asConstCharPtr();
}

// Helper to safely validate stringTable pointer, handling corrupted values from partial writes
bool isValidStringTablePtr(const SwosDataPointer<StringTable>& ptr)
{
    return isValidVmPointerRaw(ptr.getRaw());
}

// Validate sprite index to prevent crashes from corrupted values
bool isValidSpriteIndex(word spriteIndex)
{
    // Sprite index 0 means no sprite, valid indices are typically small positive numbers
    // Upper bound is a reasonable sanity check - no menu should have thousands of sprites
    constexpr word kMaxReasonableSpriteIndex = 1000;
    return spriteIndex <= kMaxReasonableSpriteIndex;
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
    return safeGetStringPtrConst(fg.string);
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

    auto start = string();
    if (!start || start == kSentinel) {
        logWarn("MenuEntry::copyString: Invalid string pointer, cannot copy");
        return;
    }

    while (*str == ' ')
        str++;

    auto end = start + kStdMenuTextSize;
    auto p = start;

    while (p < end && *str)
        *p++ = *str++;

    if (p < end)
        *p = '\0';
    else
        *(end - 1) = '\0';

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
