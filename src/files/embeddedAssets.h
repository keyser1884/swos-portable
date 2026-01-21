// Auto-generated embedded assets - do not edit!
#pragma once

#include <SDL.h>

// Check if embedded assets are available
bool hasEmbeddedAsset(const char* path);

// Open an embedded asset as SDL_RWops (returns nullptr if not found)
// The caller must close the returned SDL_RWops when done
SDL_RWops* openEmbeddedAsset(const char* path);

// Initialize embedded assets (call once at startup)
void initEmbeddedAssets();
