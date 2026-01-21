#include "JoypadConfigRegister.h"
#include "Joypad.h"
#include "util.h"

static constexpr char kConfigPrefix[] = "controllerConfig-";
static constexpr int kConfigPrefixLen = sizeof(kConfigPrefix) - 1;

JoypadConfig *JoypadConfigRegister::config(const SDL_JoystickGUID& guid, bool secondary /* = false */)
{
    if (auto config = findConfig(guid)) {
        return secondary ? config->secondaryConfig() : config->primaryConfig();
    } else {
        assert(!secondary);
        m_config.emplace_back(guid);
        return m_config.back().primaryConfig();
    }
}

void JoypadConfigRegister::resetConfig(const SDL_JoystickGUID& guid, bool secondary)
{
    if (auto config = findConfig(guid)) {
        if (secondary)
            config->resetSecondary();
        else
            config->resetPrimary();
    }
}

void JoypadConfigRegister::loadConfig(const CSimpleIni& ini)
{
    CSimpleIni::TNamesDepend sectionList;
    ini.GetAllSections(sectionList);

    for (const auto& section : sectionList) {
        if (!strncmp(section.pItem, kConfigPrefix, kConfigPrefixLen)) {
            auto guidPart = section.pItem + kConfigPrefixLen;
            assert(strlen(guidPart) >= 32);

            char guidStr[64];
            int i = 0;
            for (; i < sizeof(guidStr) && isxdigit(guidPart[i]); i++)
                guidStr[i] = guidPart[i];

            guidStr[std::min<int>(i, sizeof(guidStr) - 1)] = '\0';

            auto guid = SDL_JoystickGetGUIDFromString(guidStr);
            auto it = std::find_if(m_config.begin(), m_config.end(), [&guid](const auto& config) {
                return guidEqual(config.guid(), guid);
            });

            if (it != m_config.end())
                it->loadFromIni(ini, section.pItem);
            else
                m_config.emplace_back(ini, section.pItem, guid);
        }
    }
}

void JoypadConfigRegister::saveConfig(CSimpleIni& ini)
{
    for (auto& config : m_config)
        config.saveToIni(ini);
}

JoypadGuidConfig *JoypadConfigRegister::findConfig(const SDL_JoystickGUID& guid)
{
    auto it = std::find_if(m_config.begin(), m_config.end(), [&guid](const auto& config) {
        return guidEqual(config.guid(), guid);
    });

    return it != m_config.end() ? &*it : nullptr;
}
