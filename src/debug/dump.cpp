#ifndef NDEBUG

#include "dump.h"
#include "text.h"
#include "gameLoop.h"

static bool m_debugOutput = true;

void dumpVariables()
{
    char buf[256];
    if (m_debugOutput && isMatchRunning()) {
        sprintf(buf, "DEF X: %hd", SwosVM::g_memWord[524746/2]);
        drawText(0, 16, buf, -1, kYellowText);
        sprintf(buf, "BALL X: %hd", swos.ballSprite.x.whole());
        drawText(0, 23, buf, -1, kYellowText);
        //for (size_t i = 0; i < std::size(swos.team1SpritesTable); i++) {
        //    auto& sprite1 = swos.team1SpritesTable[i];
        //    auto& sprite2 = swos.team2SpritesTable[i];
        //    snprintf(buf, sizeof(buf), "%d: %hhd %hhd",
        //        (int)i, sprite1->state, sprite2->state);
        //    drawText(0, 16 + (kSmallFontHeight + 1) * (i - 1), buf, -1, kYellowText);
        //}
        //sprintf(buf, "GAME STATE: %d", swos.gameState);
        //drawText(0, 16 + (kSmallFontHeight + 1) * 11, buf, -1, kYellowText);
    }
}

void toggleDebugOutput()
{
    m_debugOutput ^= 1;
}

#endif
