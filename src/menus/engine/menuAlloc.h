#pragma once

void resetMenuAllocator();

char *menuAlloc(size_t size);
void menuFree(size_t size);

char *menuAllocString(const char *str);
char *menuAllocStringTable(size_t size);
char *menuDupAlloc(const void *data, size_t size);

template<typename T>
T *menuDupAllocNonSwos(T *data, size_t size)
{
    return SwosVM::isSwosPtr(data) ? data : reinterpret_cast<T *>(menuDupAlloc(data, size));
}

template<typename T>
const T *menuDupAllocNonSwos(const T *data, size_t size)
{
    return SwosVM::isSwosPtr(data) ? data : reinterpret_cast<const T *>(menuDupAlloc(data, size));
}
