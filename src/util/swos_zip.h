#pragma once

bool traverseZipFile(const char *path, std::function<int(const char *path, int pathLength)> filterEntryFunc,
    std::function<void(const char *path, int pathLength, char *buffer, int bufferLength)> processEntryFunc);
