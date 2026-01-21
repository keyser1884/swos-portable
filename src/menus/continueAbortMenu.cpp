#include "continueAbortMenu.h"
#include "text.h"
#include "menuAlloc.h"
#include "continueAbort.mnu.h"

using namespace ContinueAbortMenu;

constexpr int kMaxHeaderWidth = 100;
constexpr int kHeaderMargin = 20;

constexpr int kBufferSize = 512;

static const char *m_header;
static const char *m_continueText;
static const char *m_abortText;
static MultilineText *m_warningText;

static bool m_abortDefault;
static bool m_result;

static MultilineText *formMultilineText(const std::vector<const char *>& messageLines);

bool showContinueAbortPrompt(const char *header, const char *continueText, const char *abortText,
    const std::vector<const char *>& messageLines, bool abortDefault /* = true */)
{
    assert(messageLines.size() < 7);

    m_abortDefault = abortDefault;
    m_result = false;

    m_header = menuAllocString(header);
    m_continueText = menuAllocString(continueText);
    m_abortText = menuAllocString(abortText);

    m_warningText = formMultilineText(messageLines);

    showMenu(continueAbortMenu);

    return m_result;
}

// in:
//      A0 -> header
//      A1 -> "what's changing" text:
//            first byte - number of lines
//            text following, lines are zero-terminated
//      A2 -> continue text
//      A3 -> abort text
//      D0 -  default entry, 0 = abort, !0 = continue
// out:
//      D0 - 1 - abort
//           0 - continue
//           zero flag set accordingly
//
void SWOS::DoContinueAbortMenu()
{
    m_abortDefault = D0.asWord() != 0;
    m_result = false;

    m_header = A0.asPtr();
    m_continueText = A2.asPtr();
    m_abortText = A3.asPtr();

    m_warningText = A1.as<MultilineText *>();

    showMenu(continueAbortMenu);

    D0 = !m_result;
    SwosVM::flags.zero = !D0;
}

static void setupHeader()
{
    if (m_header) {
        int len = getStringPixelLength(m_header);
        headerEntry.width = std::min(len + kHeaderMargin, kMaxHeaderWidth);
        headerEntry.x -= headerEntry.width / 2;
        headerEntry.setString(m_header);
    } else {
        headerEntry.hide();
    }
}

static void continueAbortMenuInit()
{
    setupHeader();

    warningTextEntry.fg.multilineText = m_warningText;
    contEntry.setString(m_continueText);

    if (m_abortText)
        abortEntry.setString(m_abortText);
    else
        abortEntry.hide();

    if (!m_abortDefault)
        setCurrentEntry(cont);
}

static void continueSelected()
{
    m_result = true;
    SetExitMenuFlag();
}

static void abortSelected()
{
    m_result = false;
    SetExitMenuFlag();
}

static MultilineText *formMultilineText(const std::vector<const char *>& messageLines)
{
    auto buf = menuAlloc(kBufferSize + 1);

    auto p = buf + 1;
    auto sentinel = buf + kBufferSize;

    for (auto line : messageLines) {
        while (p < sentinel && *line)
            *p++ = *line++;

        while (p - 1 > buf && p[-1] == ' ')
            p--;

        assert(p > buf);

        *p++ = '\0';
    }

    assert(p - buf <= kBufferSize);

    buf[0] = static_cast<char>(messageLines.size());

    return reinterpret_cast<MultilineText *>(buf);
}
