#pragma once

#include "file.h"

std::string showSelectFilesMenu(const char *menuTitle, const FoundFileList& filenames,
    const char *saveExtension = nullptr, char *saveFilenameBuffer = nullptr);
