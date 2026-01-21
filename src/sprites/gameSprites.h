#pragma once

using PlayerSprites = Sprite * const *;

void initGameSprites(const TeamGame *topTeam, const TeamGame *bottomTeam);
void initDisplaySprites();
void initializePlayerSpriteFrameIndices();
void drawSprites(float xOffset, float yOffset);
int getGoalkeeperSpriteOffset(bool topTeam, int face);
int getPlayerSpriteOffsetFromFace(int face);
void updateCornerFlags();

PlayerSprites getPlayerSprites();
#ifdef SWOS_TEST
Sprite *spriteAt(unsigned index);
int totalSprites();
#endif
