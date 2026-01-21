#pragma once

// enable enumeration members to be used as flags
#define ENABLE_FLAGS(ENUM) \
constexpr ENUM operator|(ENUM a, int b) \
{ \
    return static_cast<ENUM>(static_cast<int>(a) | b); \
} \
constexpr ENUM operator|(ENUM a, ENUM b) \
{ \
    return a | static_cast<int>(b); \
} \
constexpr ENUM operator|=(ENUM& a, int b) \
{ \
    return a = a | b; \
} \
constexpr ENUM operator|=(ENUM& a, ENUM b) \
{ \
    return a = a | b; \
} \
constexpr ENUM operator&(ENUM a, int b) \
{ \
    return static_cast<ENUM>(static_cast<int>(a) & b); \
} \
constexpr ENUM operator&(ENUM a, ENUM b) \
{ \
    return a & static_cast<int>(b); \
} \
constexpr ENUM operator&=(ENUM& a, int b) \
{ \
    return a = a & b; \
} \
constexpr ENUM operator&=(ENUM& a, ENUM b) \
{ \
    return a = a & b; \
} \
constexpr ENUM operator<<(ENUM a, int b) \
{ \
    return static_cast<ENUM>(static_cast<int>(a) << b); \
} \
constexpr ENUM operator<<=(ENUM& a, int b) \
{ \
    return a = a << b; \
} \
constexpr ENUM operator^(ENUM a, int b) \
{ \
    return static_cast<ENUM>(static_cast<int>(a) ^ b); \
} \
constexpr ENUM operator^(ENUM a, ENUM b) \
{ \
    return a ^ static_cast<int>(b); \
} \
constexpr ENUM operator~(ENUM a) \
{ \
    return static_cast<ENUM>(~static_cast<int>(a)); \
}
