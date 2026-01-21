#pragma once

// keep synchronized with sprite compilation script
struct PackedSprite
{
    // float coordinates are in VGA coordinate space
    SDL_Rect frame;
    int16_t xOffset;
    int16_t yOffset;
    float xOffsetF;
    float yOffsetF;
    int16_t width;
    int16_t height;
    float widthF;
    float heightF;
    int16_t centerX;
    int16_t centerY;
    float centerXF;
    float centerYF;
    int8_t texture;
    bool rotated;
};
