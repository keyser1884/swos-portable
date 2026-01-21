#pragma once

uint32_t initialHash();
uint32_t updateHash(size_t hash, char c, size_t index);
uint32_t hash(const char *str);
uint32_t hash(const void *buffer, size_t length);
