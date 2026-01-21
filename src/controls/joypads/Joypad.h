#pragma once

#include "gameControlEvents.h"
#include "JoypadElementValue.h"

class JoypadConfig;

class Joypad
{
public:
    static constexpr int kInvalidJoypadId = -1;

    Joypad(int index);
    bool tryReopening(int index);
    bool isOpen() const { return m_handle != nullptr; }
    GameControlEvents events() const;
    GameControlEvents allEventsMask() const;

    SDL_JoystickGUID guid() const { return SDL_JoystickGetGUID(m_handle); }
    SDL_JoystickID id() const { return m_id; }
    const char *powerLevel() const;

    int numButtons() const { return m_numButtons; }
    int numHats() const { return m_numHats; }
    int numAxes() const { return m_numAxes; }
    int numBalls() const { return m_numBalls; }

    const char *name() const;
    const char *baseName() const;
    size_t hash() const;
    int instance() const;
    void setNameSuffix(int suffix);

    Uint8 getButton(int index) const;
    Uint8 getHat(int index) const;
    Sint16 getAxis(int index) const;
    std::pair<int, int> getBall(int index) const;

    JoypadElementValueList elementValues() const;

    void setConfig(JoypadConfig *config);
    JoypadConfig *config() { return m_config; }

    bool guidEqual(const SDL_JoystickGUID& guid) const;
    bool anyUnassignedButtonDown() const;
    bool anyButtonDown() const;

    uint64_t lastSelected() const;
    void updateLastSelected(uint64_t time);

private:
    static JoypadElementValue& initButton(JoypadElementValue& element, int index);
    static JoypadElementValue& initHat(JoypadElementValue& element, int index, int mask);
    static JoypadElementValue& initAxis(JoypadElementValue& element, int index, int16_t value);
    static JoypadElementValue& initBall(JoypadElementValue& element, int index, int dx, int dy);

    void fetchJoypadInfo();

    SDL_Joystick *m_handle = nullptr;
    SDL_JoystickID m_id = kInvalidJoypadId;

    std::string m_name;
    size_t m_hash;
    int m_instance = 0;

    int m_numButtons = 0;
    int m_numHats = 0;
    int m_numAxes = 0;
    int m_numBalls = 0;

    JoypadConfig *m_config = nullptr;
    uint64_t m_lastSelected = 0;
};
