#include "JoypadGuidConfig.h"
#include "Joypad.h"
#include "VirtualJoypad.h"

static constexpr char kConfigPrefix[] = "controllerConfig-";
static constexpr int kConfigPrefixLen = sizeof(kConfigPrefix) - 1;

JoypadGuidConfig::JoypadGuidConfig(const SDL_JoystickGUID& guid)
    : m_guid(guid), m_pl2Config(2)
{
    loadDefaultConfig();
    m_pl1Config = m_defaultConfig;
}

JoypadGuidConfig::JoypadGuidConfig(const CSimpleIni& ini, const char *sectionName, const SDL_JoystickGUID& guid)
    : JoypadGuidConfig(guid)
{
    loadFromIni(ini, sectionName);
}

void JoypadGuidConfig::initPl2Config()
{
    m_pl2Config = m_defaultConfig;
    m_gotPl2Config = true;
}

const JoypadConfig& JoypadGuidConfig::config(PlayerNumber player) const
{
    if (player == kPlayer1)
        return m_pl1Config;
    return m_gotPl2Config ? m_pl2Config : m_pl1Config;
}

void JoypadGuidConfig::resetPrimary()
{
    m_pl1Config = m_defaultConfig;
}

void JoypadGuidConfig::resetSecondary()
{
    m_pl2Config = m_defaultConfig;
}

void JoypadGuidConfig::loadFromIni(const CSimpleIni& ini, const char *sectionName)
{
    auto config = &m_pl1Config;

    auto dash = strrchr(sectionName, '-');
    if (dash && dash[1] == '2' && !dash[2])
        config = &m_pl2Config;

    config->clear();
    config->loadFromIni(ini, sectionName);
}

void JoypadGuidConfig::saveToIni(CSimpleIni& ini)
{
    m_pl1Config.canonize();
    if (!m_pl1Config.empty() && m_pl1Config != m_defaultConfig)
        saveToIni(ini, m_pl1Config, 1);

    m_pl2Config.canonize();
    if (!m_pl2Config.empty() && m_pl2Config != m_defaultConfig)
        saveToIni(ini, m_pl2Config, 2);
}

void JoypadGuidConfig::loadDefaultConfig()
{
    if (auto mapping = internalMapping(m_guid)) {
        m_defaultConfig.initWithMapping(mapping);
    } else if (auto mapping = SDL_GameControllerMappingForGUID(m_guid)) {
        m_defaultConfig.initWithMapping(mapping);
        SDL_free(mapping);
    } else {
        m_defaultConfig.init();
    }
}

const char *JoypadGuidConfig::internalMapping(const SDL_JoystickGUID& guid)
{
    struct Mapping {
        SDL_JoystickGUID guid;
        const char *sdlMappingString;
    } static const kInternalMappings[] = {
        {
            // Gamepad Media-Tech MT-169 Warrior, 1 hat 6-front 4-back buttons
            { 0x03, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
            ",,a:b0,b:b1,leftx:a0,lefty:a4,"
        },
#ifdef VIRTUAL_JOYPAD
        {
            VirtualJoypad::kGuid,
            ",,a:b0,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,",
        },
#endif
    };

    for (const auto& mapping : kInternalMappings)
        if (!memcmp(&mapping.guid, &guid, sizeof(guid)))
            return mapping.sdlMappingString;

    return nullptr;
}

void JoypadGuidConfig::fillSectionName(char *buf, size_t bufSize, int configNum) const
{
    assert(bufSize > kConfigPrefixLen + 1 + sizeof(SDL_JoystickGUID));

    memcpy(buf, kConfigPrefix, kConfigPrefixLen + 1);
    SDL_JoystickGetGUIDString(m_guid, buf + kConfigPrefixLen, bufSize - kConfigPrefixLen);
    if (configNum > 1)
        strcat(buf, "-2");
}

void JoypadGuidConfig::loadFromIni(const CSimpleIni& ini, JoypadConfig& config, int configNum) const
{
    char sectionName[128];
    fillSectionName(sectionName, sizeof(sectionName), configNum);

    config.loadFromIni(ini, sectionName);
}

void JoypadGuidConfig::saveToIni(CSimpleIni& ini, JoypadConfig& config, int configNum) const
{
    char sectionName[128];
    fillSectionName(sectionName, sizeof(sectionName), configNum);

    config.saveToIni(ini, sectionName);
}
