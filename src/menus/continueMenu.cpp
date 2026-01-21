#include "continueMenu.h"
#include "continue.mnu.h"

using namespace ContinueMenu;

static const char *m_errorText;

void showErrorMenu(const char *errorText)
{
    m_errorText = errorText;
    showMenu(continueMenu);
}

static void continueOnInit()
{
    errorTextEntry.copyString(m_errorText);
}

// in:
//     A0 -> error message
//
void SWOS::ShowErrorMenu()
{
    showErrorMenu(A0.asPtr());
}
