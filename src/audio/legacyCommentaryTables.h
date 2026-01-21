// Auto-generated from swos/swos.asm. Do not edit by hand.
#pragma once

#include <cstdint>

struct LegacyOffsetSample {
    uint32_t offset;
    uint32_t length;
};

constexpr const char *kLegacyCommentaryFiles[] = {
    "SFX\\PIERCE\\m158_1_.raw",
    "SFX\\PIERCE\\m158_2_.raw",
    "SFX\\PIERCE\\m158_3_.raw",
    "SFX\\PIERCE\\m158_4_.raw",
    "SFX\\PIERCE\\m158_5_.raw",
    "SFX\\PIERCE\\m158_7_.raw",
    "SFX\\PIERCE\\m313_7_.raw",
    "SFX\\PIERCE\\m313_A_.raw",
    "SFX\\PIERCE\\m313_B_.raw",
    "SFX\\PIERCE\\m196_W_.raw",
    "SFX\\PIERCE\\m196_X_.raw",
    "SFX\\PIERCE\\m196_Z_.raw",
    "SFX\\PIERCE\\m233_1_.raw",
    "SFX\\PIERCE\\M158_P_.raw",
    "SFX\\PIERCE\\M158_Q_.raw",
    "SFX\\PIERCE\\M158_S_.raw",
    "SFX\\PIERCE\\M158_T_.raw",
    "SFX\\PIERCE\\M158_X_.raw",
    "SFX\\PIERCE\\M158_Y_.raw",
    "SFX\\PIERCE\\M10_j_.raw",
    "SFX\\PIERCE\\M10_k_.raw",
    "SFX\\PIERCE\\m313_6_.raw",
    "SFX\\PIERCE\\m313_8_.raw",
    "SFX\\PIERCE\\m313_9_.raw",
    "SFX\\PIERCE\\m313_C_.raw",
    "SFX\\PIERCE\\m313_D_.raw",
    "SFX\\PIERCE\\m313_G_.raw",
    "SFX\\PIERCE\\m313_H_.raw",
    "SFX\\PIERCE\\m10_D_.raw",
    "SFX\\PIERCE\\m10_E_.raw",
    "SFX\\PIERCE\\m10_F_.raw",
    "SFX\\PIERCE\\m10_G_.raw",
    "SFX\\PIERCE\\m10_H_.raw",
    "SFX\\PIERCE\\m10_R_.raw",
    "SFX\\PIERCE\\m10_S_.raw",
    "SFX\\PIERCE\\m10_T_.raw",
    "SFX\\PIERCE\\m10_I_.raw",
    "SFX\\PIERCE\\m10_U_.raw",
    "SFX\\PIERCE\\M349_7_.raw",
    "SFX\\PIERCE\\M349_8_.raw",
    "SFX\\PIERCE\\M349_E_.raw",
    "SFX\\PIERCE\\M365_1_.raw",
    "SFX\\PIERCE\\M313_i_.raw",
    "SFX\\PIERCE\\M313_j_.raw",
    "SFX\\PIERCE\\M349_4_.raw",
    "SFX\\PIERCE\\M313_o_.raw",
    "SFX\\PIERCE\\M158_Z_.RAW",
    "SFX\\PIERCE\\M195.RAW",
    "SFX\\PIERCE\\M196_1_.RAW",
    "SFX\\PIERCE\\M196_2_.RAW",
    "SFX\\PIERCE\\M196_4_.RAW",
    "SFX\\PIERCE\\M196_6_.RAW",
    "SFX\\PIERCE\\M196_7_.RAW",
    "SFX\\PIERCE\\M196_K_.RAW",
    "SFX\\PIERCE\\M196_L_.RAW",
    "SFX\\PIERCE\\M196_M_.RAW",
    "SFX\\PIERCE\\M196_N_.RAW",
    "SFX\\PIERCE\\M196_T_.raw",
    "SFX\\PIERCE\\M196_U_.raw",
    "SFX\\PIERCE\\M196_V_.raw",
    "SFX\\PIERCE\\M233_2_.raw",
    "SFX\\PIERCE\\M233_4_.raw",
    "SFX\\PIERCE\\M233_5_.raw",
    "SFX\\PIERCE\\M233_6_.raw",
    "SFX\\PIERCE\\M233_7_.raw",
    "SFX\\PIERCE\\M233_8_.raw",
    "SFX\\PIERCE\\M233_9_.raw",
    "SFX\\PIERCE\\M233_c_.raw",
    "SFX\\PIERCE\\M33_2_.raw",
    "SFX\\PIERCE\\M33_3_.raw",
    "SFX\\PIERCE\\M33_4_.raw",
    "SFX\\PIERCE\\M33_5_.raw",
    "SFX\\PIERCE\\M33_6_.raw",
    "SFX\\PIERCE\\M33_8_.raw",
    "SFX\\PIERCE\\M33_A_.raw",
    "SFX\\PIERCE\\M33_B_.raw",
};

constexpr LegacyOffsetSample kLegacy_goalSamples[] = {
    { 0u, 29952u },
    { 29952u, 44032u },
    { 73984u, 40576u },
    { 114560u, 22528u },
    { 137088u, 55311u },
    { 192399u, 28160u },
    { 29952u, 44032u },
    { 0u, 29952u },
};

constexpr LegacyOffsetSample kLegacy_goalkeeperSavedSamples[] = {
    { 220559u, 31744u },
    { 252303u, 36224u },
    { 288527u, 28672u },
    { 317199u, 35188u },
    { 352387u, 31185u },
    { 383572u, 49664u },
    { 433236u, 49646u },
    { 220559u, 31744u },
};

constexpr LegacyOffsetSample kLegacy_ownGoalSamples[] = {
    { 482882u, 45458u },
    { 528340u, 48199u },
    { 576539u, 47754u },
    { 624293u, 17024u },
    { 641317u, 67660u },
    { 708977u, 52992u },
    { 624293u, 17024u },
    { 576539u, 47754u },
};

constexpr LegacyOffsetSample kLegacy_nearMissesSamples[] = {
    { 761969u, 46336u },
    { 808305u, 66099u },
    { 1073388u, 40448u },
    { 1113836u, 51818u },
};

constexpr LegacyOffsetSample kLegacy_postHitSamples[] = {
    { 1073388u, 40448u },
    { 1113836u, 51818u },
    { 1165654u, 50129u },
    { 1215783u, 43554u },
    { 1259337u, 47548u },
    { 1306885u, 25088u },
    { 1331973u, 44215u },
    { 1376188u, 44160u },
};

constexpr LegacyOffsetSample kLegacy_hitFrameSamples[] = {
    { 1073388u, 40448u },
    { 1113836u, 50129u },
    { 1165654u, 43554u },
    { 1215783u, 43554u },
    { 1306885u, 25088u },
    { 1331973u, 44215u },
    { 1331973u, 44215u },
    { 1215783u, 43554u },
};

constexpr LegacyOffsetSample kLegacy_barHitSamples[] = {
    { 1420348u, 33280u },
    { 1453628u, 39179u },
};

constexpr LegacyOffsetSample kLegacy_keeperClaimedSamples[] = {
    { 874404u, 36992u },
    { 911396u, 31190u },
    { 942586u, 20736u },
    { 963322u, 34816u },
    { 998138u, 37618u },
    { 1035756u, 0u },
    { 1035756u, 15360u },
    { 1051116u, 22272u },
};

constexpr LegacyOffsetSample kLegacy_goodTackleSamples[] = {
    { 1492807u, 21760u },
    { 1514567u, 49852u },
    { 1564419u, 28928u },
    { 1593347u, 17408u },
};

constexpr LegacyOffsetSample kLegacy_goodPassSamples[] = {
    { 1610755u, 32256u },
    { 1643011u, 31488u },
    { 1674499u, 22016u },
    { 1696515u, 31744u },
};

constexpr LegacyOffsetSample kLegacy_foulSamples[] = {
    { 1728259u, 18460u },
    { 1746719u, 30720u },
    { 1777439u, 42496u },
    { 1819935u, 45973u },
    { 1865908u, 50176u },
    { 1916084u, 17985u },
    { 1934069u, 29745u },
    { 1728259u, 18460u },
};

constexpr LegacyOffsetSample kLegacy_dangerousPlaySamples[] = {
    { 1728259u, 18460u },
    { 1746719u, 30720u },
    { 1865908u, 50176u },
    { 1728259u, 18460u },
};

constexpr LegacyOffsetSample kLegacy_itsPenaltySamples[] = {
    { 1963814u, 18560u },
    { 1982374u, 40800u },
    { 2023174u, 22572u },
    { 2045746u, 53504u },
};

constexpr LegacyOffsetSample kLegacy_savedPenaltySamples[] = {
    { 2099250u, 36409u },
    { 2135659u, 44535u },
    { 2180194u, 28928u },
    { 252303u, 36224u },
    { 383572u, 49664u },
    { 433236u, 49646u },
    { 317199u, 35188u },
    { 220559u, 31744u },
};

constexpr LegacyOffsetSample kLegacy_missedPenaltySamples[] = {
    { 2209122u, 48128u },
    { 2257250u, 29696u },
    { 2286946u, 43316u },
    { 2330262u, 69120u },
};

constexpr LegacyOffsetSample kLegacy_penaltyGoalSamples[] = {
    { 2399382u, 76544u },
    { 2475926u, 37120u },
    { 2513046u, 38726u },
    { 2551772u, 47400u },
};

constexpr LegacyOffsetSample kLegacy_headerSamples[] = {
    { 2599172u, 28416u },
    { 2627588u, 14592u },
    { 2642180u, 23808u },
    { 2665988u, 22528u },
    { 2688516u, 56064u },
    { 2744580u, 20992u },
    { 2765572u, 19968u },
    { 2785540u, 26880u },
};

constexpr const char *kLegacy_cornerSamplesTable[] = {
    "hard\\M10_W_.RAW",
    "hard\\M10_W_.RAW",
    "hard\\M10_Y_.RAW",
    "hard\\M10_Y_.RAW",
    "hard\\M313_1_.RAW",
    "hard\\M313_2_.RAW",
    "hard\\M313_3_.RAW",
};

constexpr const char *kLegacy_tacticsChangeSamplesTable[] = {
    "hard\\M10_7_.RAW",
    "hard\\M10_8_.RAW",
    "hard\\M10_9_.RAW",
    "hard\\M10_A_.RAW",
    "hard\\M10_B_.RAW",
    "hard\\M10_7_.RAW",
    "hard\\M10_5_.RAW",
};

constexpr const char *kLegacy_playerSubstitutedSamplesTable[] = {
    "hard\\M233_K_.RAW",
    "hard\\M233_L_.RAW",
    "hard\\M233_M_.RAW",
    "hard\\M10_3_.RAW",
    "hard\\M10_4_.RAW",
    "hard\\M233_K_.RAW",
    "hard\\M233_J_.RAW",
};

constexpr const char *kLegacy_redCardSamplesTable[] = {
    "hard\\M196_9_.RAW",
    "hard\\M196_A_.RAW",
    "hard\\M196_B_.RAW",
    "hard\\M196_C_.RAW",
    "hard\\M196_D_.RAW",
    "hard\\M196_E_.RAW",
    "hard\\M196_F_.RAW",
    "hard\\M196_G_.RAW",
    "hard\\M196_H_.RAW",
    "hard\\M196_I_.RAW",
    "hard\\M196_J_.RAW",
    "hard\\M196_8_.RAW",
    "hard\\M196_9_.RAW",
    "hard\\M196_A_.RAW",
    "hard\\M196_B_.RAW",
};

constexpr const char *kLegacy_yellowCardSamplesTable[] = {
    "hard\\M443_8_.RAW",
    "hard\\M443_9_.RAW",
    "hard\\M443_A_.RAW",
    "hard\\M443_B_.RAW",
    "hard\\M443_C_.RAW",
    "hard\\M443_D_.RAW",
    "hard\\M443_E_.RAW",
    "hard\\M443_F_.RAW",
    "hard\\M443_G_.RAW",
    "hard\\M443_H_.RAW",
    "hard\\M443_I_.RAW",
    "hard\\M443_J_.RAW",
    "hard\\M443_7_.RAW",
    "hard\\M443_8_.RAW",
    "hard\\M443_9_.RAW",
};

constexpr const char *kLegacy_drawSamplesTable[] = {
    "hard\\M406_g_.RAW",
    "hard\\M406_f_.RAW",
    "hard\\M406_g_.RAW",
    "hard\\M406_f_.RAW",
    "hard\\M406_g_.RAW",
    "hard\\M406_f_.RAW",
    "hard\\M406_g_.RAW",
};

constexpr const char *kLegacy_completeRoutSamplesTable[] = {
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
    "hard\\M406_h_.RAW",
};

constexpr const char *kLegacy_sensationalGameSamplesTable[] = {
    "hard\\M406_j_.RAW",
    "hard\\M406_i_.RAW",
    "hard\\M406_j_.RAW",
    "hard\\M406_i_.RAW",
    "hard\\M406_j_.RAW",
    "hard\\M406_i_.RAW",
    "hard\\M406_j_.RAW",
};

constexpr const char *kLegacy_throwInSamplesTable[] = {
    "hard\\M406_8_.RAW",
    "hard\\M406_7_.RAW",
    "hard\\M406_9_.RAW",
    "hard\\M406_3_.RAW",
    "hard\\M406_8_.RAW",
    "hard\\M406_7_.RAW",
    "hard\\M406_9_.RAW",
};

