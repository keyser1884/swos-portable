#include "loadTexture.h"
#include "assetManager.h"
#include "embeddedAssets.h"
#include "render.h"
#include "util.h"
#include "file.h"

SDL_Texture *loadTexture(const char *path)
{
    SDL_Texture *texture{};

    auto resPath = joinPaths(getAssetDir(), path);

    if (auto f = openFile(resPath.c_str())) {
        // Use typed loader for embedded assets (SDL_RWFromConstMem needs format hint)
        if (hasEmbeddedAsset(resPath.c_str())) {
            auto surface = IMG_LoadTyped_RW(f, true, "PNG");
            if (surface) {
                texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
                SDL_FreeSurface(surface);
            }
        } else {
            texture = IMG_LoadTexture_RW(getRenderer(), f, true);
        }
    } else {
        errorExit("Failed to open texture file %s", path);
    }

    if (!texture)
        errorExit("Failed to load texture %s: %s", path, IMG_GetError());

    return texture;
}
