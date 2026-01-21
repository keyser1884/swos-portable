#pragma once

#include "JoypadGuidConfig.h"

class JoypadConfig;

class JoypadConfigRegister
{
public:
    JoypadConfig *config(const SDL_JoystickGUID& guid, bool secondary = false);
    void resetConfig(const SDL_JoystickGUID& guid, bool secondary);
    void loadConfig(const CSimpleIni& ini);
    void saveConfig(CSimpleIni& ini);

private:
    JoypadGuidConfig *findConfig(const SDL_JoystickGUID& guid);

    std::list<JoypadGuidConfig> m_config;
};
