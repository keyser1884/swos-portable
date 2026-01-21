#include "testJoypadMenu.h"
#include "testControlsMenu.h"
#include "joypads.h"

static int m_joypadIndex;

static std::vector<std::string> getControlNames()
{
    auto elements = joypadElementValues(m_joypadIndex);

    std::vector<std::string> controls;

    for (const auto& element : elements)
        controls.emplace_back(element.toString(true));

    return controls;
}

static GameControlEvents getEvents()
{
    return joypadEvents(m_joypadIndex);
}

void showTestJoypadMenu(int joypadIndex)
{
    m_joypadIndex = joypadIndex;

    char titleBuf[128];
    snprintf(titleBuf, sizeof(titleBuf), "TEST %s", joypadName(joypadIndex));

    showTestControlsMenu(titleBuf, "CONTROLLER INPUTS:", true, getControlNames, getEvents);
}
