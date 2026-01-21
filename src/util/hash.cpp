#include "hash.h"

constexpr size_t kInitialHashValue = 1021;

uint32_t initialHash()
{
    return kInitialHashValue;
}

uint32_t updateHash(size_t hash, char c, size_t index)
{
    hash += index + c;
    hash = _rotl(hash, 1);
    return hash ^ index + c;
}

uint32_t hash(const char *str)
{
    size_t hash = kInitialHashValue;
    size_t index = 0;

    while (*str++)
        hash = updateHash(hash, str[-1], index++);

    return hash;
}

uint32_t hash(const void *buffer, size_t length)
{
    size_t hash = kInitialHashValue;
    auto p = reinterpret_cast<const char *>(buffer);

    for (size_t i = 0; i < length; i++, p++)
        hash = updateHash(hash, *p, i);

    return hash;
}
