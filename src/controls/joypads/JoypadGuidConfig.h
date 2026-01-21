#pragma once

#include "JoypadConfig.h"
#include "controls.h"

class Joypad;

class JoypadGuidConfig
{
public:
    JoypadGuidConfig(const SDL_JoystickGUID& guid);
    JoypadGuidConfig(const CSimpleIni& ini, const char *sectionName, const SDL_JoystickGUID& guid);
    void initPl2Config();
    const JoypadConfig& config(PlayerNumber player) const;

    SDL_JoystickGUID guid() const { return m_guid; }
    JoypadConfig *primaryConfig() { return &m_pl1Config; }
    JoypadConfig *secondaryConfig() { return &m_pl2Config; }
    void resetPrimary();
    void resetSecondary();
    bool gotPl2Config() const { return m_gotPl2Config; }

    void loadFromIni(const CSimpleIni& ini, const char *sectionName);
    void saveToIni(CSimpleIni& ini);

private:
    void loadDefaultConfig();
    static const char *internalMapping(const SDL_JoystickGUID& guid);
    void fillSectionName(char *buf, size_t bufSize, int configNum) const;
    void loadFromIni(const CSimpleIni& ini, JoypadConfig& config, int configNum) const;
    void saveToIni(CSimpleIni& ini, JoypadConfig& config, int configNum) const;

    SDL_JoystickGUID m_guid;
    JoypadConfig m_pl1Config;
    JoypadConfig m_pl2Config;
    JoypadConfig m_defaultConfig;
    bool m_gotPl2Config = false;
};
