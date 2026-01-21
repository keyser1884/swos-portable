#pragma once

const char *scancodeToString(SDL_Scancode scancode);
const char *scancodeToString(SDL_Scancode scancode, char *buf, size_t bufSize);
uint8_t sdlScancodeToPc(SDL_Scancode scancode);
