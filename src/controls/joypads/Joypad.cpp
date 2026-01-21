#include "Joypad.h"
#include "JoypadConfig.h"
#include "hash.h"
#include "util.h"

Joypad::Joypad(int index)
{
    m_handle = SDL_JoystickOpen(index);
    if (m_handle) {
        logInfo("Controller \"%s\" opened successfully", SDL_JoystickName(m_handle));
        fetchJoypadInfo();
    } else {
        logWarn("Failed to open joypad %d, SDL error: %s", index, SDL_GetError());
    }
}

bool Joypad::tryReopening(int index)
{
    if (!m_handle && (m_handle = SDL_JoystickOpen(index))) {
        m_id = SDL_JoystickInstanceID(m_handle);
        fetchJoypadInfo();
    }

    return m_handle != nullptr;
}

GameControlEvents Joypad::events() const
{
    assert(m_config);
    return m_config->events(*this);
}

GameControlEvents Joypad::allEventsMask() const
{
    assert(m_config);
    return m_config->allEventsMask();
}

const char *Joypad::name() const
{
    return !m_name.empty() ? m_name.c_str() : baseName();
}

const char *Joypad::baseName() const
{
    if (m_handle)
        return SDL_JoystickName(m_handle);
    else
        return nullptr;
}

size_t Joypad::hash() const
{
    return m_hash;
}

int Joypad::instance() const
{
    return m_instance;
}

void Joypad::setNameSuffix(int suffix)
{
    assert(suffix > 0);

    m_name = SDL_JoystickName(m_handle);
    m_name += ' ';
    m_name += '(';
    m_name += std::to_string(suffix);
    m_name += ')';

    m_instance = suffix;
}

Uint8 Joypad::getButton(int index) const
{
    assert(index >= 0);

    if (m_handle && index >= 0 && index < m_numButtons)
        return SDL_JoystickGetButton(m_handle, index);
    else
        return 0;
}

Uint8 Joypad::getHat(int index) const
{
    assert(index >= 0);

    if (m_handle && index >= 0 && index < m_numHats)
        return SDL_JoystickGetHat(m_handle, index);
    else
        return 0;
}
Sint16 Joypad::getAxis(int index) const
{
    assert(index >= 0);

    if (m_handle && index >= 0 && index < m_numAxes)
        return SDL_JoystickGetAxis(m_handle, index);
    else
        return 0;
}

std::pair<int, int> Joypad::getBall(int index) const
{
    assert(index >= 0);

    int dx, dy;

    if (m_handle && index >= 0 && index < m_numBalls && SDL_JoystickGetBall(m_handle, index, &dx, &dy) == 0)
        return { dx, dy };
    else
        return { 0, 0 };
}

auto Joypad::elementValues() const -> JoypadElementValueList
{
    JoypadElementValueList result;

    if (!m_handle)
        return result;

    JoypadElementValue el;

    for (int i = 0; i < m_numButtons; i++)
        if (SDL_JoystickGetButton(m_handle, i))
            result.push_back(initButton(el, i));

    for (int i = 0; i < m_numHats; i++)
        if (auto mask = SDL_JoystickGetHat(m_handle, i))
            result.push_back(initHat(el, i, mask));

    for (int i = 0; i < m_numAxes; i++)
        if (auto value = SDL_JoystickGetAxis(m_handle, i))
            result.push_back(initAxis(el, i, value));

    for (int i = 0; i < m_numBalls; i++) {
        int dx, dy;
        if (!SDL_JoystickGetBall(m_handle, i, &dx, &dy)) {
            if (dx)
                result.push_back(initBall(el, i, dx, 0));
            if (dy)
                result.push_back(initBall(el, i, 0, dy));
        }
    }

    return result;
}

void Joypad::setConfig(JoypadConfig *config)
{
    assert(config);
    m_config = config;
}

const char *Joypad::powerLevel() const
{
    auto powerLevel = m_handle ? SDL_JoystickCurrentPowerLevel(m_handle) : SDL_JOYSTICK_POWER_UNKNOWN;

    switch (powerLevel) {
    case SDL_JOYSTICK_POWER_EMPTY:
        return "EMPTY";
    case SDL_JOYSTICK_POWER_LOW:
        return "LOW";
    case SDL_JOYSTICK_POWER_MEDIUM:
        return "MEDIUM";
    case SDL_JOYSTICK_POWER_FULL:
        return "FULL";
    case SDL_JOYSTICK_POWER_WIRED:
        return "WIRED";
    case SDL_JOYSTICK_POWER_MAX:
        return "MAX";
    case SDL_JOYSTICK_POWER_UNKNOWN:
    default:
        return "UNKNOWN";
    }
}

bool Joypad::guidEqual(const SDL_JoystickGUID& guid) const
{
    return ::guidEqual(guid, this->guid());
}

bool Joypad::anyUnassignedButtonDown() const
{
    if (m_handle) {
        for (int i = 0; i < m_numButtons; i++) {
            bool pressed = SDL_JoystickGetButton(m_handle, i) != 0;
            if (!m_config)
                return pressed;

            auto buttonConfig = m_config->getButtonEvents(i);
            bool inverted = buttonConfig.second;

            if (buttonConfig.first == kNoGameEvents && pressed != inverted)
                return true;
        }
    }

    return false;
}

bool Joypad::anyButtonDown() const
{
    if (m_handle) {
        for (int i = 0; i < m_numButtons; i++)
            if (SDL_JoystickGetButton(m_handle, i))
                return true;
    }

    return false;
}

uint64_t Joypad::lastSelected() const
{
    return m_lastSelected;
}

void Joypad::updateLastSelected(uint64_t time)
{
    m_lastSelected = time;
}

auto Joypad::initButton(JoypadElementValue& element, int index) -> JoypadElementValue&
{
    element.type = JoypadElement::kButton;
    element.index = index;
    element.pressed = true;
    return element;
}

auto Joypad::initHat(JoypadElementValue& element, int index, int mask) -> JoypadElementValue&
{
    element.type = JoypadElement::kHat;
    element.index = index;
    element.hatMask = mask;
    return element;
}

auto Joypad::initAxis(JoypadElementValue& element, int index, int16_t value) -> JoypadElementValue&
{
    element.type = JoypadElement::kAxis;
    element.index = index;
    element.axisValue = value;
    return element;
}

auto Joypad::initBall(JoypadElementValue& element, int index, int dx, int dy) -> JoypadElementValue&
{
    element.type = JoypadElement::kBall;
    element.index = index;
    element.ball.dx = dx;
    element.ball.dy = dy;
    return element;
}

void Joypad::fetchJoypadInfo()
{
    if (m_handle) {
        m_id = SDL_JoystickInstanceID(m_handle);
        m_numButtons = SDL_JoystickNumButtons(m_handle);
        m_numHats = SDL_JoystickNumHats(m_handle);
        m_numAxes = SDL_JoystickNumAxes(m_handle);
        m_numBalls = SDL_JoystickNumBalls(m_handle);
        m_hash = ::hash(SDL_JoystickName(m_handle));
    }
}
