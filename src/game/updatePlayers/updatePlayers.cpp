#include "updatePlayers.h"
#include "playerFunctions.h"
#include "../ai/ai.h"
#include "../../options/portedFunctions.h"

// Assembly function externs for toggle system
extern void AI_SetControlsDirection();
extern void ResetBothTeamSpinTimers();
extern void ThrowInDeadProc();
extern void GetPlayerPointerFromShirtNumber();
extern void UpdateTeamShotChanceTable();
extern void SetThrowInPlayerDestinationCoordinates();
extern void SetPlayerDowntimeAfterTackle();
extern void UpdatePlayerWithBall();
extern void UpdateControllingPlayer();
extern void GoalkeeperCaughtTheBall();
extern void UpdateBallWithControllingGoalkeeper();
extern void AttemptStaticHeader();
extern void UpdatePlayerSpeed();
extern void GoalkeeperJumping();
extern void PlayerBeginTackling();
extern void PlayersTackledTheBallStrong();
extern void SetJumpHeaderHitAnimTable();
extern void SetStaticHeaderDirection();
extern void PlayerAttemptingJumpHeader();
extern void AI_Kick();
extern void CalculateBallNextGroundXYPositions();
extern void UpdateBallVariables();
extern void ApplyBallAfterTouch();
extern void PlayerHittingStaticHeader();
extern void PlayerHittingJumpHeader();
extern void PlayerTackledTheBallWeak();
extern void PlayerKickingBall();
extern void ShouldGoalkeeperDive();
extern void PlayerTacklingTestFoul();
extern void DoPass();
extern void CalculateIfPlayerWinsBall();

using namespace SwosVM;

void updatePlayers(TeamGeneralInfo *team)
{
    A6 = ptrToOffset(team);                 // Set A6 to team pointer for assembly code
    eax = g_memDword[130777];               // mov eax, lastPlayerPlayed
    g_memWord[262369] = eax & 0xffff;
    g_memWord[262370] = eax >> 16;          // mov prevLastPlayer, eax
    eax = g_memDword[130776];               // mov eax, lastTeamPlayed
    g_memWord[262371] = eax & 0xffff;
    g_memWord[262372] = eax >> 16;          // mov prevLastTeamPlayed, eax
    eax = g_memByte[523099] | (g_memDword[130775] << 8); // mov eax, lastKeeperPlayed
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (eax & 0x80000000) != 0;
    flags.zero = eax == 0;                  // or eax, eax
    if (flags.zero)
        goto l_update_goalkeeper_saved_timer; // jz short @@update_goalkeeper_saved_timer

    {
        dword src = g_memDword[130777];
        int32_t dstSigned = src;
        int32_t srcSigned = 326524;
        dword res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
        flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
        flags.sign = (res & 0x80000000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp lastPlayerPlayed, offset goalie1Sprite
    if (flags.zero)
        goto l_update_goalkeeper_saved_timer; // jz short @@update_goalkeeper_saved_timer

    {
        dword src = g_memDword[130777];
        int32_t dstSigned = src;
        int32_t srcSigned = 327756;
        dword res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
        flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
        flags.sign = (res & 0x80000000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp lastPlayerPlayed, offset goalie2Sprite
    if (flags.zero)
        goto l_update_goalkeeper_saved_timer; // jz short @@update_goalkeeper_saved_timer

    g_memByte[523099] = 0;
    g_memDword[130775] = g_memByte[523103] << 24; // mov lastKeeperPlayed, 0

l_update_goalkeeper_saved_timer:;
    esi = A6;                               // mov esi, A6
    ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (ax & 0x8000) != 0;
    flags.zero = ax == 0;                   // or ax, ax
    if (flags.zero)
        goto l_update_pass_kick_timer;      // jz short @@update_pass_kick_timer

    if (flags.sign)
        goto l_decay_goalkeeper_saved_timer; // js short @@decay_goalkeeper_saved_timer

    {
        word src = (word)readMemory(esi + 76, 2);
        int16_t dstSigned = src;
        int16_t srcSigned = 1;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
        src = res;
        writeMemory(esi + 76, 2, src);
    }                                       // sub [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer], 1
    goto l_update_pass_kick_timer;          // jmp short @@update_pass_kick_timer

l_decay_goalkeeper_saved_timer:;
    esi = A6;                               // mov esi, A6
    {
        word src = (word)readMemory(esi + 76, 2);
        int16_t dstSigned = src;
        int16_t srcSigned = 1;
        word res = dstSigned + srcSigned;
        src = res;
        writeMemory(esi + 76, 2, src);
    }                                       // add [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer], 1

l_update_pass_kick_timer:;
    esi = A6;                               // mov esi, A6
    ax = (word)readMemory(esi + 102, 2);    // mov ax, [esi+TeamGeneralInfo.passKickTimer]
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (ax & 0x8000) != 0;
    flags.zero = ax == 0;                   // or ax, ax
    if (flags.zero)
        goto l_update_won_the_ball_timer;   // jz short @@update_won_the_ball_timer

    {
        word src = (word)readMemory(esi + 102, 2);
        int16_t dstSigned = src;
        int16_t srcSigned = 1;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
        src = res;
        writeMemory(esi + 102, 2, src);
    }                                       // sub [esi+TeamGeneralInfo.passKickTimer], 1
    if (!flags.zero)
        goto l_update_won_the_ball_timer;   // jnz short @@update_won_the_ball_timer

    writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0

l_update_won_the_ball_timer:;
    esi = A6;                               // mov esi, A6
    ax = (word)readMemory(esi + 138, 2);    // mov ax, [esi+TeamGeneralInfo.wonTheBallTimer]
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (ax & 0x8000) != 0;
    flags.zero = ax == 0;                   // or ax, ax
    if (flags.zero)
        goto l_update_player_switch_timer;  // jz short @@update_player_switch_timer

    {
        word src = (word)readMemory(esi + 138, 2);
        int16_t dstSigned = src;
        int16_t srcSigned = 1;
        word res = dstSigned - srcSigned;
        src = res;
        writeMemory(esi + 138, 2, src);
    }                                       // sub [esi+TeamGeneralInfo.wonTheBallTimer], 1

l_update_player_switch_timer:;
    esi = A6;                               // mov esi, A6
    ax = (word)readMemory(esi + 92, 2);     // mov ax, [esi+TeamGeneralInfo.playerSwitchTimer]
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (ax & 0x8000) != 0;
    flags.zero = ax == 0;                   // or ax, ax
    if (flags.zero)
        goto l_apply_after_touch_and_set_ball_location_flags; // jz short @@apply_after_touch_and_set_ball_location_flags

    {
        word src = (word)readMemory(esi + 92, 2);
        int16_t dstSigned = src;
        int16_t srcSigned = 1;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
        src = res;
        writeMemory(esi + 92, 2, src);
    }                                       // sub [esi+TeamGeneralInfo.playerSwitchTimer], 1

l_apply_after_touch_and_set_ball_location_flags:;
    CompareApplyBallAfterTouch(g_portedToggles.useCpp_ApplyBallAfterTouch); // call ApplyBallAfterTouch with comparison
    A2 = 328988;                            // mov A2, offset ballSprite
    esi = A2;                               // mov esi, A2
    ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
    *(word *)&D1 = ax;                      // mov word ptr D1, ax
    ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
    *(word *)&D2 = ax;                      // mov word ptr D2, ax
    g_memWord[262385] = 0;                  // mov ballInUpperPenaltyArea, 0
    g_memWord[262386] = 0;                  // mov ballInLowerPenaltyArea, 0
    g_memWord[262387] = 0;                  // mov ballInGoalkeeperArea, 0
    {
        int16_t dstSigned = *(word *)&D1;
        int16_t srcSigned = 193;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D1, 193
    if (flags.sign != flags.overflow)
        goto l_not_in_penalty_area;         // jl @@not_in_penalty_area

    {
        int16_t dstSigned = *(word *)&D1;
        int16_t srcSigned = 478;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D1, 478
    if (!flags.zero && flags.sign == flags.overflow)
        goto l_not_in_penalty_area;         // jg short @@not_in_penalty_area

    {
        int16_t dstSigned = *(word *)&D2;
        int16_t srcSigned = 129;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D2, 129
    if (flags.sign != flags.overflow)
        goto l_not_in_penalty_area;         // jl short @@not_in_penalty_area

    {
        int16_t dstSigned = *(word *)&D2;
        int16_t srcSigned = 769;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D2, 769
    if (!flags.zero && flags.sign == flags.overflow)
        goto l_not_in_penalty_area;         // jg short @@not_in_penalty_area

    {
        int16_t dstSigned = *(word *)&D2;
        int16_t srcSigned = 216;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D2, 216
    if (flags.zero || flags.sign != flags.overflow)
        goto l_upper_penalty_area;          // jle short @@upper_penalty_area

    {
        int16_t dstSigned = *(word *)&D2;
        int16_t srcSigned = 682;
        word res = dstSigned - srcSigned;
        flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
        flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
        flags.sign = (res & 0x8000) != 0;
        flags.zero = res == 0 != 0;
    }                                       // cmp word ptr D2, 682
    if (flags.sign != flags.overflow)
        goto l_not_in_penalty_area;         // jl short @@not_in_penalty_area

    g_memWord[262386] = 1;                  // mov ballInLowerPenaltyArea, 1
    goto l_check_goalkeeper_area;           // jmp short @@check_goalkeeper_area

l_upper_penalty_area:;
    g_memWord[262385] = 1;                  // mov ballInUpperPenaltyArea, 1

l_check_goalkeeper_area:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 273;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 273
if (flags.sign != flags.overflow)
goto l_not_in_penalty_area;         // jl short @@not_in_penalty_area

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 398;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 398
if (!flags.zero && flags.sign == flags.overflow)
goto l_not_in_penalty_area;         // jg short @@not_in_penalty_area

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 158;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 158
if (flags.zero || flags.sign != flags.overflow)
goto l_goalkeeper_area;             // jle short @@goalkeeper_area

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 740;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 740
if (flags.sign != flags.overflow)
goto l_not_in_penalty_area;         // jl short @@not_in_penalty_area

l_goalkeeper_area:;
g_memWord[262387] = 1;                  // mov ballInGoalkeeperArea, 1

l_not_in_penalty_area:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_update_player_index;         // jnz short @@update_player_index

ax = g_memWord[261557];                 // mov ax, goalOut
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_player_index;         // jz short @@update_player_index

eax = g_memWord[262385] | g_memWord[262386] << 16; // mov eax, dword ptr ballInUpperPenaltyArea
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto l_update_player_index;         // jnz short @@update_player_index

g_memWord[261557] = 0;                  // mov goalOut, 0

l_update_player_index:;
esi = A6;                               // mov esi, A6
{
    word src = (word)readMemory(esi + 30, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    writeMemory(esi + 30, 2, src);
}                                       // add [esi+TeamGeneralInfo.updatePlayerIndex], 1
{
    word src = (word)readMemory(esi + 30, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 11;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+TeamGeneralInfo.updatePlayerIndex], 11
if (!flags.zero)
goto l_init_players_loop;           // jnz short @@init_players_loop

writeMemory(esi + 30, 2, 0);            // mov [esi+TeamGeneralInfo.updatePlayerIndex], 0

l_init_players_loop:;
*(word *)&D4 = 10;                      // mov word ptr D4, 10
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 20, 4);          // mov eax, [esi+TeamGeneralInfo.spritesTable]
A3 = eax;                               // mov A3, eax

l_players_loop:;
push(D4);                               // push small [word ptr D4]
esi = A3;                               // mov esi, A3
eax = readMemory(esi, 4);               // mov eax, [esi]
{
    int32_t dstSigned = A3;
    int32_t srcSigned = 4;
    dword res = dstSigned + srcSigned;
    A3 = res;
}                                       // add A3, 4
A1 = eax;                               // mov A1, eax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
writeMemory(esi + 92, 2, ax);           // mov [esi+Sprite.playerDirection], ax
eax = readMemory(esi + 46, 4);          // mov eax, [esi+Sprite.deltaX]
D0 = eax;                               // mov D0, eax
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
D0 |= eax;
flags.carry = false;
flags.overflow = false;
flags.sign = (D0 & 0x80000000) != 0;
flags.zero = D0 == 0;                   // or D0, eax
writeMemory(esi + 94, 1, 0);            // mov byte ptr [esi+Sprite.isMoving], 0
if (flags.zero)
goto l_test_if_player_tackled;      // jz short @@test_if_player_tackled

writeMemory(esi + 94, 1, -1);           // mov byte ptr [esi+Sprite.isMoving], -1

l_test_if_player_tackled:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 3;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_TACKLED
if (flags.zero)
goto l_player_tackled;              // jz @@player_tackled

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 13;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_ROLLING_INJURED
if (flags.zero)
goto l_player_injured;              // jz @@player_injured

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_update_player_ball_distance; // jz short @@update_player_ball_distance

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_update_player_ball_distance; // jz short @@update_player_ball_distance

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_update_player_ball_distance; // jz short @@update_player_ball_distance

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 0;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_NORMAL
if (!flags.zero)
goto l_update_player_ball_distance; // jnz short @@update_player_ball_distance

ax = g_memWord[261817];                 // mov ax, injuriesForever
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_jmp_not_controlled_player;   // jz short @@jmp_not_controlled_player

{
    word src = (word)readMemory(esi + 104, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = -1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.injuryLevel], -1
if (flags.zero)
goto l_injury_forever;              // jz @@injury_forever

l_jmp_not_controlled_player:;
goto l_not_controlled_player;           // jmp @@not_controlled_player

l_update_player_ball_distance:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
writeMemory(esi + 69, 1, al);           // mov [esi+TeamGeneralInfo.prevPlVeryCloseToBall], al
writeMemory(esi + 61, 1, 0);            // mov [esi+TeamGeneralInfo.plVeryCloseToBall], 0
writeMemory(esi + 62, 1, 0);            // mov [esi+TeamGeneralInfo.plCloseToBall], 0
writeMemory(esi + 63, 1, 0);            // mov [esi+TeamGeneralInfo.plNotFarFromBall], 0
esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
D0 = eax;                               // mov D0, eax
{
    int32_t dstSigned = D0;
    int32_t srcSigned = 32;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, 32
if (flags.carry || flags.zero)
goto l_very_close_to_ball;          // jbe short @@very_close_to_ball

{
    int32_t dstSigned = D0;
    int32_t srcSigned = 72;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, 72
if (flags.carry || flags.zero)
goto l_close_to_ball;               // jbe short @@close_to_ball

{
    int32_t dstSigned = D0;
    int32_t srcSigned = 2450;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, 2450
if (flags.carry || flags.zero)
goto l_somewhat_close_to_ball;      // jbe short @@somewhat_close_to_ball

goto l_update_ball_height;              // jmp short @@update_ball_height

l_very_close_to_ball:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 61, 1, 1);            // mov [esi+TeamGeneralInfo.plVeryCloseToBall], 1
goto l_update_ball_height;              // jmp short @@update_ball_height

l_close_to_ball:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 62, 1, 1);            // mov [esi+TeamGeneralInfo.plCloseToBall], 1
goto l_update_ball_height;              // jmp short @@update_ball_height

l_somewhat_close_to_ball:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 63, 1, 1);            // mov [esi+TeamGeneralInfo.plNotFarFromBall], 1

l_update_ball_height:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 64, 1, 0);            // mov [esi+TeamGeneralInfo.ballLessEqual4], 0
writeMemory(esi + 65, 1, 0);            // mov [esi+TeamGeneralInfo.ball4To8], 0
writeMemory(esi + 66, 1, 0);            // mov [esi+TeamGeneralInfo.ball8To12], 0
writeMemory(esi + 67, 1, 0);            // mov [esi+TeamGeneralInfo.ball12To17], 0
writeMemory(esi + 68, 1, 0);            // mov [esi+TeamGeneralInfo.ballAbove17], 0
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 40, 2);     // mov ax, word ptr [esi+(Sprite.z+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 17;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 17
if (!flags.carry && !flags.zero)
goto l_ball_too_high;               // ja short @@ball_too_high

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 12
if (!flags.carry && !flags.zero)
goto l_ball_high;                   // ja short @@ball_high

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 8;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 8
if (!flags.carry && !flags.zero)
goto l_ball_medium_height;          // ja short @@ball_medium_height

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 4;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 4
if (!flags.carry && !flags.zero)
goto l_ball_low;                    // ja short @@ball_low

esi = A6;                               // mov esi, A6
writeMemory(esi + 64, 1, 1);            // mov [esi+TeamGeneralInfo.ballLessEqual4], 1
goto l_check_player_state;              // jmp short @@check_player_state

l_ball_low:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 65, 1, 1);            // mov [esi+TeamGeneralInfo.ball4To8], 1
goto l_check_player_state;              // jmp short @@check_player_state

l_ball_medium_height:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 66, 1, 1);            // mov [esi+TeamGeneralInfo.ball8To12], 1
goto l_check_player_state;              // jmp short @@check_player_state

l_ball_high:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 67, 1, 1);            // mov [esi+TeamGeneralInfo.ball12To17], 1
goto l_check_player_state;              // jmp short @@check_player_state

l_ball_too_high:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 68, 1, 1);            // mov [esi+TeamGeneralInfo.ballAbove17], 1

l_check_player_state:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_TACKLING
if (flags.zero)
goto l_player_tackling;             // jz @@player_tackling

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 9;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_JUMP_HEADING
if (flags.zero)
goto l_player_jump_heading;         // jz @@player_jump_heading

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 8;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_STATIC_HEADING
if (flags.zero)
goto l_player_doing_static_header;  // jz @@player_doing_static_header

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 5;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_THROW_IN
if (flags.zero)
goto l_player_taking_throw_in;      // jz @@player_taking_throw_in

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 6;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_DIVING_HIGH
if (flags.zero)
goto l_goalie_diving;               // jz @@goalie_diving

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 7;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_DIVING_LOW
if (flags.zero)
goto l_goalie_diving;               // jz @@goalie_diving

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 4;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_CATCHING_BALL
if (flags.zero)
goto l_goalie_catching_the_ball;    // jz @@goalie_catching_the_ball

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 10;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_DOWN
if (flags.zero)
goto l_player_down_st_10;           // jz @@player_down_st_10

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 12;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_BOOKED
if (flags.zero)
goto l_player_booked;               // jz @@player_booked

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 14;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_SAD
if (flags.zero)
goto l_player_sad_or_happy;         // jz @@player_sad_or_happy

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 15;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_HAPPY
if (flags.zero)
goto l_player_sad_or_happy;         // jz @@player_sad_or_happy

{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 11;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_CLAIMED
if (flags.zero)
goto l_goalie_claimed;              // jz @@goalie_claimed

{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_player_goalkeeper;           // jz short @@player_goalkeeper

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_its_controlled_player;       // jz @@its_controlled_player

eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_player_expecting_pass;       // jz @@player_expecting_pass

debugBreak();                           // int 3

l_endless_loop:;
goto l_endless_loop;                    // jmp short @@endless_loop

l_player_goalkeeper:;
g_memWord[228128] = 0;                  // mov goalTypeScored, GT_REGULAR
ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_shot_chance_table_for_goalie; // jz short @@update_shot_chance_table_for_goalie

{
    word src = g_memWord[261584];
    int16_t dstSigned = src;
    int16_t srcSigned = g_memWord[270438];
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp penaltiesTimer, penaltyLockInterval
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

l_update_shot_chance_table_for_goalie:;
if (g_portedToggles.useCpp_UpdateTeamShotChanceTable) PlayerFunctions::UpdateTeamShotChanceTable(); else UpdateTeamShotChanceTable();
ax = g_memByte[449805] | (g_memByte[449806] << 8); // mov ax, lastPlayerTurnFlags+1
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_this_player_last_played;     // jnz @@this_player_last_played

g_memWord[262382] = -1;                 // mov ballNextGroundX, -1
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 140, 2);    // mov ax, [esi+TeamGeneralInfo.goalkeeperPlaying]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_ball_out_or_keepers;  // jz short @@update_ball_out_or_keepers

writeMemory(esi + 86, 1, 1);            // mov byte ptr [esi+TeamGeneralInfo.goaliePlayingOrOut], 1
writeMemory(esi + 84, 1, 0);            // mov byte ptr [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], 0
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_its_controlled_player;       // jz @@its_controlled_player

writeMemory(esi + 140, 2, 0);           // mov [esi+TeamGeneralInfo.goalkeeperPlaying], 0

l_update_ball_out_or_keepers:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 84, 1, 0);            // mov byte ptr [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], 0
writeMemory(esi + 86, 1, 0);            // mov byte ptr [esi+TeamGeneralInfo.goaliePlayingOrOut], 0
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_update_goalkeeper_speed;     // jz short @@update_goalkeeper_speed

writeMemory(esi + 86, 1, -1);           // mov byte ptr [esi+TeamGeneralInfo.goaliePlayingOrOut], -1
writeMemory(esi + 84, 1, -1);           // mov byte ptr [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], -1
ax = g_memWord[162812];                 // mov ax, kGoalkeeperSpeedWhenGameStopped
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax

l_update_goalkeeper_speed:;
ax = g_memWord[162870];                 // mov ax, kGoalkeeperGameSpeed
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 449;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 449
if (!flags.carry && !flags.zero)
goto l_goalkeeper_in_lower_half;    // ja short @@goalkeeper_in_lower_half

{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto l_this_player_last_played;     // jz @@this_player_last_played

goto l_goalkeeper_in_upper_half;        // jmp short @@goalkeeper_in_upper_half

l_goalkeeper_in_lower_half:;
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522804;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset topTeamData
if (flags.zero)
goto l_this_player_last_played;     // jz @@this_player_last_played

l_goalkeeper_in_upper_half:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 4;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_CATCHING_BALL
if (!flags.zero)
goto l_goalie_not_catching_the_ball; // jnz @@goalie_not_catching_the_ball

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (flags.zero)
goto l_player_got_up;               // jz short @@player_got_up

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto l_increase_y_dest;             // jz short @@increase_y_dest

{
    word src = (word)readMemory(esi + 60, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 60, 2, src);
}                                       // sub [esi+Sprite.destY], 1
goto cseg_7FCD0;                        // jmp cseg_7FCD0

l_increase_y_dest:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 60, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 60, 2, src);
}                                       // add [esi+Sprite.destY], 1
goto cseg_7FCD0;                        // jmp cseg_7FCD0

l_player_got_up:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable

l_goalie_not_catching_the_ball:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_its_controlled_player;       // jz @@its_controlled_player

eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_player_expecting_pass;       // jz @@player_expecting_pass

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_not_controlled_player;       // jnz @@not_controlled_player

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto cseg_7F0DF;                    // jz short cseg_7F0DF

ax = g_memWord[262385];                 // mov ax, ballInUpperPenaltyArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_pass_to_player;        // jz @@check_pass_to_player

goto l_ball_in_penalty_area;            // jmp short @@ball_in_penalty_area

cseg_7F0DF:;
ax = g_memWord[262386];                 // mov ax, ballInLowerPenaltyArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_pass_to_player;        // jz @@check_pass_to_player

l_ball_in_penalty_area:;
eax = g_memDword[130777];               // mov eax, lastPlayerPlayed
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_this_player_last_played;     // jz @@this_player_last_played

CompareUpdateBallVariables(g_portedToggles.useCpp_UpdateBallVariables); // call UpdateBallVariables with comparison
CompareCalculateBallNextGroundXYPositions(g_portedToggles.useCpp_CalculateBallNextGroundXYPositions); // call CalculateBallNextGroundXYPositions with comparison
ax = g_memWord[262387];                 // mov ax, ballInGoalkeeperArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_ball_in_lower_goalkeeper_area; // jnz short @@ball_in_lower_goalkeeper_area

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 240;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0F0h
{
    word res = *(word *)&D0 >> 4;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 4
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+58]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.carry)
goto l_ball_standing_in_goalkeeper_area; // jnb @@ball_standing_in_goalkeeper_area

l_ball_in_lower_goalkeeper_area:;
ax = g_memWord[262382];                 // mov ax, ballNextGroundX
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_ball_standing_in_goalkeeper_area; // js @@ball_standing_in_goalkeeper_area

ax = g_memWord[262382];                 // mov ax, ballNextGroundX
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = g_memWord[262383];                 // mov ax, ballNextYGroundY
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto cseg_7F1C7;                    // jz short cseg_7F1C7

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 137;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 137
if (flags.sign != flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jl @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 216;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 216
if (!flags.zero && flags.sign == flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jg @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 193;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 193
if (flags.sign != flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jl @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 478;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 478
if (!flags.zero && flags.sign == flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jg @@ball_standing_in_goalkeeper_area

goto l_in_penalty_area;                 // jmp short @@in_penalty_area

cseg_7F1C7:;
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 682;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 682
if (flags.sign != flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jl @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 761;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 761
if (!flags.zero && flags.sign == flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jg @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 193;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 193
if (flags.sign != flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jl @@ball_standing_in_goalkeeper_area

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 478;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 478
if (!flags.zero && flags.sign == flags.overflow)
goto l_ball_standing_in_goalkeeper_area; // jg @@ball_standing_in_goalkeeper_area

l_in_penalty_area:;
ax = g_memWord[262382];                 // mov ax, ballNextGroundX
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
ax = g_memWord[262383];                 // mov ax, ballNextYGroundY
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
ax = D0;                                // mov ax, word ptr D0
bx = D0;                                // mov bx, word ptr D0
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D0 = ax;                      // mov word ptr D0, ax
*(word *)((byte *)&D0 + 2) = dx;        // mov word ptr D0+2, dx
ax = D1;                                // mov ax, word ptr D1
bx = D1;                                // mov bx, word ptr D1
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D1 = ax;                      // mov word ptr D1, ax
*(word *)((byte *)&D1 + 2) = dx;        // mov word ptr D1+2, dx
eax = D0;                               // mov eax, D0
{
    int32_t dstSigned = D1;
    int32_t srcSigned = eax;
    dword res = dstSigned + srcSigned;
    D1 = res;
}                                       // add D1, eax
ax = g_memWord[262382];                 // mov ax, ballNextGroundX
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
ax = g_memWord[262383];                 // mov ax, ballNextYGroundY
*(word *)&D2 = ax;                      // mov word ptr D2, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D2 = res;
}                                       // sub word ptr D2, ax
ax = D0;                                // mov ax, word ptr D0
bx = D0;                                // mov bx, word ptr D0
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D0 = ax;                      // mov word ptr D0, ax
*(word *)((byte *)&D0 + 2) = dx;        // mov word ptr D0+2, dx
ax = D2;                                // mov ax, word ptr D2
bx = D2;                                // mov bx, word ptr D2
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D2 = ax;                      // mov word ptr D2, ax
*(word *)((byte *)&D2 + 2) = dx;        // mov word ptr D2+2, dx
eax = D0;                               // mov eax, D0
{
    int32_t dstSigned = D2;
    int32_t srcSigned = eax;
    dword res = dstSigned + srcSigned;
    D2 = res;
}                                       // add D2, eax
{
    dword res = D1 << 2;
    D1 = res;
}                                       // shl D1, 2
eax = D2;                               // mov eax, D2
{
    int32_t dstSigned = D1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D1, eax
if (!flags.carry && !flags.zero)
goto l_ball_standing_in_goalkeeper_area; // ja short @@ball_standing_in_goalkeeper_area

ax = g_memWord[262382];                 // mov ax, ballNextGroundX
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = g_memWord[262383];                 // mov ax, ballNextYGroundY
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
ax = g_memWord[162017];                 // mov ax, kGoalkeeperMoveToBallSpeed
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
goto cseg_7FCD0;                        // jmp cseg_7FCD0

l_ball_standing_in_goalkeeper_area:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto cseg_7FCA1;                    // jnz cseg_7FCA1

{
    word src = g_memWord[262379];
    int16_t dstSigned = src;
    int16_t srcSigned = 295;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp strikeDestX, 295
if (flags.carry)
goto l_shot_on_goal_or_close;       // jb short @@shot_on_goal_or_close

{
    word src = g_memWord[262379];
    int16_t dstSigned = src;
    int16_t srcSigned = 376;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp strikeDestX, 376
if (flags.carry || flags.zero)
goto l_goal_attempt;                // jbe @@goal_attempt

l_shot_on_goal_or_close:;
esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 512;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 512
if (!flags.carry)
goto l_goalkeeper_dont_throw;       // jnb short @@goalkeeper_dont_throw

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_goalkeeper_dont_throw;       // jnz short @@goalkeeper_dont_throw

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto cseg_7FCD0;                        // jmp cseg_7FCD0

l_goalkeeper_dont_throw:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_7F511;                    // jnz cseg_7F511

ax = g_memWord[262387];                 // mov ax, ballInGoalkeeperArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_center_goalkeeper_on_ball_x; // jnz short @@center_goalkeeper_on_ball_x

esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 449;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 449
if (flags.carry)
goto cseg_7F3F8;                    // jb short cseg_7F3F8

eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (flags.zero || flags.sign != flags.overflow)
goto cseg_7F511;                    // jle cseg_7F511

goto cseg_7F409;                        // jmp short cseg_7F409

cseg_7F3F8:;
esi = A2;                               // mov esi, A2
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.sign)
goto cseg_7F511;                    // jns cseg_7F511

cseg_7F409:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 336;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 336
{
    int16_t res = *(int16_t *)&D0 >> 1;
    *(int16_t *)&D0 = res;
}                                       // sar word ptr D0, 1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 336;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 336
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
goto cseg_7F458;                        // jmp short cseg_7F458

l_center_goalkeeper_on_ball_x:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax

cseg_7F458:;
esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 449;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 449
if (flags.carry)
goto cseg_7F49A;                    // jb short cseg_7F49A

*(word *)&D0 = 769;                     // mov word ptr D0, 769
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D0 = res;
}                                       // add word ptr D0, ax
goto cseg_7F4C3;                        // jmp short cseg_7F4C3

cseg_7F49A:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 129
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 129;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 129

cseg_7F4C3:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 6, 2);      // mov ax, [esi+6]
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 6, 2);      // mov ax, [esi+6]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 1;
    flags.carry = ((word) * (word *)&D0 >> 15) & 1;
    flags.overflow = (((*(word *)&D0 >> 15) & 1)) != 0;
    *(word *)&D0 = res;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // shr word ptr D0, 1
goto cseg_7FCD0;                        // jmp cseg_7FCD0

cseg_7F511:;
esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 16;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 16
if (!flags.carry && !flags.zero)
goto cseg_7F626;                    // ja cseg_7F626

ax = g_memWord[262373];                 // mov ax, ballDefensiveX
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto cseg_7F626;                    // js cseg_7F626

esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
D0 = eax;                               // mov D0, eax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
A0 = eax;                               // mov A0, eax
{
    int32_t dstSigned = A0;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A0, 0
if (flags.zero)
goto cseg_7F56B;                    // jz short cseg_7F56B

esi = A0;                               // mov esi, A0
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (!flags.carry && !flags.zero)
goto cseg_7F626;                    // ja cseg_7F626

cseg_7F56B:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
A0 = eax;                               // mov A0, eax
{
    int32_t dstSigned = A0;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A0, 0
if (flags.zero)
goto cseg_7F597;                    // jz short cseg_7F597

esi = A0;                               // mov esi, A0
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (!flags.carry && !flags.zero)
goto cseg_7F626;                    // ja cseg_7F626

cseg_7F597:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
A0 = eax;                               // mov A0, eax
{
    int32_t dstSigned = A0;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A0, 0
if (flags.zero)
goto cseg_7F5CC;                    // jz short cseg_7F5CC

esi = A0;                               // mov esi, A0
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (!flags.carry && !flags.zero)
goto cseg_7F626;                    // ja short cseg_7F626

cseg_7F5CC:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
A0 = eax;                               // mov A0, eax
{
    int32_t dstSigned = A0;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A0, 0
if (flags.zero)
goto cseg_7F601;                    // jz short cseg_7F601

esi = A0;                               // mov esi, A0
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (!flags.carry && !flags.zero)
goto cseg_7F626;                    // ja short cseg_7F626

cseg_7F601:;
ax = g_memWord[262373];                 // mov ax, ballDefensiveX
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = g_memWord[262374];                 // mov ax, ballDefensiveY
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto cseg_7FCD0;                        // jmp cseg_7FCD0

cseg_7F626:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_this_player_last_played;     // jnz @@this_player_last_played

goto l_this_player_last_played;         // jmp @@this_player_last_played

l_goal_attempt:;
eax = g_memDword[130776];               // mov eax, lastTeamPlayed
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto cseg_7F65A;                    // jnz short cseg_7F65A

ax = g_memByte[523097] | (g_memByte[523098] << 8); // mov ax, playerHadBall
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_7FBEF;                    // jz cseg_7FBEF

cseg_7F65A:;
esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 512;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 512
if (!flags.carry)
goto cseg_7F6A3;                    // jnb short cseg_7F6A3

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_7F6A3;                    // jnz short cseg_7F6A3

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_shot_at_goal;                    // jmp @@shot_at_goal

cseg_7F6A3:;
esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 2048;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 2048
if (!flags.carry && !flags.zero)
goto cseg_7F7BC;                    // ja cseg_7F7BC

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_7F7BC;                    // jnz cseg_7F7BC

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 336;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 336
{
    int16_t res = *(int16_t *)&D0 >> 1;
    *(int16_t *)&D0 = res;
}                                       // sar word ptr D0, 1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 336;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 336
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 449;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 449
if (flags.carry)
goto cseg_7F745;                    // jb short cseg_7F745

*(word *)&D0 = 769;                     // mov word ptr D0, 769
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D0 = res;
}                                       // add word ptr D0, ax
goto cseg_7F76E;                        // jmp short cseg_7F76E

cseg_7F745:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 129
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 129;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 129

cseg_7F76E:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 6, 2);      // mov ax, [esi+6]
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 6, 2);      // mov ax, [esi+6]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 1;
    flags.carry = ((word) * (word *)&D0 >> 15) & 1;
    flags.overflow = (((*(word *)&D0 >> 15) & 1)) != 0;
    *(word *)&D0 = res;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // shr word ptr D0, 1
goto l_shot_at_goal;                    // jmp @@shot_at_goal

cseg_7F7BC:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7FC48;                    // jnz cseg_7FC48

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7FC48;                    // jnz cseg_7FC48

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_7FC48;                    // jnz cseg_7FC48

eax = g_memDword[130776];               // mov eax, lastTeamPlayed
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_check_shot_at_goal_speed;    // jnz short @@check_shot_at_goal_speed

ax = g_memByte[523097] | (g_memByte[523098] << 8); // mov ax, playerHadBall
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_7FBEF;                    // jz cseg_7FBEF

l_check_shot_at_goal_speed:;
ax = g_memWord[162035];                 // mov ax, kShotAtGoalMinumumSpeed
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (flags.carry)
goto l_shot_at_goal;                // jb short @@shot_at_goal

esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 5000;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 5000
if (!flags.carry && !flags.zero)
goto cseg_7FC01;                    // ja cseg_7FC01

esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 12
if (!flags.carry && !flags.zero)
goto cseg_7FC01;                    // ja cseg_7FC01

{
    dword src = readMemory(esi + 54, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 32768;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.deltaZ], 8000h
if (!flags.zero && flags.sign == flags.overflow)
goto cseg_7FC01;                    // jg cseg_7FC01

{
    dword src = readMemory(esi + 54, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 32768;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.deltaZ], 8000h
if (flags.sign != flags.overflow)
goto cseg_7FC01;                    // jl cseg_7FC01

goto cseg_7FC48;                        // jmp cseg_7FC48

l_shot_at_goal:;
esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 16;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 16
if (!flags.carry && !flags.zero)
goto cseg_7FC01;                    // ja cseg_7FC01

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero && flags.sign == flags.overflow)
goto l_goalkeeper_saved;            // jg @@goalkeeper_saved

esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 128;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 128
if (!flags.carry && !flags.zero)
goto l_goalkeeper_saved;            // ja @@goalkeeper_saved

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_goalkeeper_saved;            // jnz @@goalkeeper_saved

eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
al = (byte)readMemory(esi + 50, 1);     // mov al, [esi+TeamGeneralInfo.firePressed]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_goalkeeper_saved;            // jz @@goalkeeper_saved

ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7F910;                    // jnz short cseg_7F910

{
    word src = (word)readMemory(esi + 102, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 22;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+TeamGeneralInfo.passKickTimer], 22
if (flags.carry)
goto l_goalkeeper_saved;            // jb @@goalkeeper_saved

cseg_7F910:;
*(word *)&D1 = 0;                       // mov word ptr D1, 0
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
eax = readMemory(esi + 72, 4);          // mov eax, [esi+TeamGeneralInfo.lastHeadingTacklingPlayer]
A0 = eax;                               // mov A0, eax
{
    int32_t dstSigned = A0;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A0, 0
if (flags.zero)
goto l_get_goalie_skill;            // jz short @@get_goalie_skill

{
    dword tmp = A1;
    A1 = eax;
    eax = tmp;
}                                       // xchg eax, A1
A0 = eax;                               // mov A0, eax
if (g_portedToggles.useCpp_GetPlayerPointerFromShirtNumber) PlayerFunctions::GetPlayerPointerFromShirtNumber(); else GetPlayerPointerFromShirtNumber();
eax = A0;                               // mov eax, A0
{
    dword tmp = A1;
    A1 = eax;
    eax = tmp;
}                                       // xchg eax, A1
A0 = eax;                               // mov A0, eax
esi = A4;                               // mov esi, A4
al = (byte)readMemory(esi + 75, 1);     // mov al, [esi+PlayerGameHeader.finishing]
*(byte *)&D1 = al;                      // mov byte ptr D1, al

l_get_goalie_skill:;
if (g_portedToggles.useCpp_GetPlayerPointerFromShirtNumber) PlayerFunctions::GetPlayerPointerFromShirtNumber(); else GetPlayerPointerFromShirtNumber();
esi = A4;                               // mov esi, A4
al = (byte)readMemory(esi + 76, 1);     // mov al, [esi+PlayerGameHeader.goalieSkill]
{
    int8_t dstSigned = *(byte *)&D1;
    int8_t srcSigned = al;
    byte res = dstSigned - srcSigned;
    *(byte *)&D1 = res;
}                                       // sub byte ptr D1, al
al = D1;                                // mov al, byte ptr D1
ah = (int8_t)al < 0 ? -1 : 0;           // cbw
*(word *)&D1 = ax;                      // mov word ptr D1, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 7;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 7
A0 = 526900;                            // mov A0, offset goalScoredChances
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
{
    word res = *(word *)&D0 & 15;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0Fh
esi = A0;                               // mov esi, A0
ebx = *(word *)&D1;                     // movzx ebx, word ptr D1
al = (byte)readMemory(esi + ebx, 1);    // mov al, [esi+ebx]
{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = al;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D0, al
if (flags.carry)
goto l_goal_scored;                 // jb short @@goal_scored

esi = A6;                               // mov esi, A6
writeMemory(esi + 76, 2, 5);            // mov [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer], 5
goto l_goalkeeper_saved;                // jmp @@goalkeeper_saved

l_goal_scored:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 76, 2, -5);           // mov [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer], -5
*(word *)&D1 = 2;                       // mov word ptr D1, 2
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (flags.carry)
goto cseg_7FA33;                    // jb short cseg_7FA33

*(word *)&D0 = 2;                       // mov word ptr D0, 2
*(word *)&D3 = 2;                       // mov word ptr D3, 2
goto cseg_7FA45;                        // jmp short cseg_7FA45

cseg_7FA33:;
*(word *)&D0 = 6;                       // mov word ptr D0, 6
*(word *)&D3 = 6;                       // mov word ptr D3, 6

cseg_7FA45:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
ax = g_memWord[164516];                 // mov ax, ballSprite.speed
g_memWord[168589] = ax;                 // mov ballSpeed, ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
{
    word src = g_memWord[164516];
    int16_t dstSigned = src;
    int16_t srcSigned = 1536;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballSprite.speed, 1536
if (flags.carry || flags.zero)
goto cseg_7FA8E;                    // jbe short cseg_7FA8E

g_memWord[164516] = 1536;               // mov ballSprite.speed, 1536

cseg_7FA8E:;
push(A1);                               // push A1
if (g_portedToggles.useCpp_GoalkeeperJumping) PlayerFunctions::GoalkeeperJumping(); else GoalkeeperJumping(); // call GoalkeeperJumping
pop(A1);                                // pop A1
ax = g_memWord[164516];                 // mov ax, ballSprite.speed
g_memWord[168590] = ax;                 // mov dseg_114EA6, ax
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 0);           // mov [esi+TeamGeneralInfo.passKickTimer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_clamp_ball_y_inside_pitch;       // jmp @@clamp_ball_y_inside_pitch

l_goalkeeper_saved:;
if (g_portedToggles.useCpp_ShouldGoalkeeperDive) PlayerFunctions::ShouldGoalkeeperDive(); else ShouldGoalkeeperDive(); // call ShouldGoalkeeperDive
ax = D0;                                // mov ax, word ptr D0
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_7FC01;                    // jz cseg_7FC01

ax = g_memWord[262373];                 // mov ax, ballDefensiveX
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (flags.carry)
goto cseg_7FB57;                    // jb short cseg_7FB57

*(word *)&D1 = 0;                       // mov word ptr D1, 0
ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7FB1B;                    // jnz short cseg_7FB1B

ax = g_memWord[261556];                 // mov ax, penalty
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_7FB43;                    // jz short cseg_7FB43

cseg_7FB1B:;
*(word *)&D1 = 1;                       // mov word ptr D1, 1
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 6;
    *(word *)&D0 = res;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // and word ptr D0, 6
if (flags.zero)
goto cseg_7FB9E;                    // jz short cseg_7FB9E

*(word *)&D1 = 0;                       // mov word ptr D1, 0

cseg_7FB43:;
*(word *)&D0 = 2;                       // mov word ptr D0, 2
*(word *)&D3 = 2;                       // mov word ptr D3, 2
goto cseg_7FBB0;                        // jmp short cseg_7FBB0

cseg_7FB57:;
*(word *)&D1 = 0;                       // mov word ptr D1, 0
ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7FB76;                    // jnz short cseg_7FB76

ax = g_memWord[261556];                 // mov ax, penalty
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_7FB9E;                    // jz short cseg_7FB9E

cseg_7FB76:;
*(word *)&D1 = 1;                       // mov word ptr D1, 1
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 12;
    *(word *)&D0 = res;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // and word ptr D0, 0Ch
if (flags.zero)
goto cseg_7FB43;                    // jz short cseg_7FB43

*(word *)&D1 = 0;                       // mov word ptr D1, 0

cseg_7FB9E:;
*(word *)&D0 = 6;                       // mov word ptr D0, 6
*(word *)&D3 = 6;                       // mov word ptr D3, 6

cseg_7FBB0:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
push(A1);                               // push A1
if (g_portedToggles.useCpp_GoalkeeperJumping) PlayerFunctions::GoalkeeperJumping(); else GoalkeeperJumping(); // call GoalkeeperJumping
pop(A1);                                // pop A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 0);           // mov [esi+TeamGeneralInfo.passKickTimer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_clamp_ball_y_inside_pitch;       // jmp @@clamp_ball_y_inside_pitch

cseg_7FBEF:;
ax = g_memWord[162014];                 // mov ax, dseg_1105EF
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
goto cseg_7FC23;                        // jmp short cseg_7FC23

cseg_7FC01:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 8, 2);      // mov ax, [esi+8]
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax

cseg_7FC23:;
ax = g_memWord[262376];                 // mov ax, ballNotHighX
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = g_memWord[262377];                 // mov ax, ballNotHighY
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto cseg_7FCD0;                        // jmp cseg_7FCD0

cseg_7FC48:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_7FC01;                    // jnz short cseg_7FC01

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 6, 2);      // mov ax, [esi+6]
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
goto cseg_7FCD0;                        // jmp short cseg_7FCD0

cseg_7FCA1:;
goto l_this_player_last_played;         // jmp @@this_player_last_played

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax

cseg_7FCD0:;
eax = g_memDword[130776];               // mov eax, lastTeamPlayed
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_opponent_last_played;        // jnz short @@opponent_last_played

ax = g_memByte[523097] | (g_memByte[523098] << 8); // mov ax, playerHadBall
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_goalie_cant_catch_ball;      // jz @@goalie_cant_catch_ball

l_opponent_last_played:;
ax = g_memWord[262387];                 // mov ax, ballInGoalkeeperArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_7FD39;                    // jnz short cseg_7FD39

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 240;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0F0h
{
    word res = *(word *)&D0 >> 4;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 4
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+58]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.carry)
goto l_goalie_cant_catch_ball;      // jnb @@goalie_cant_catch_ball

cseg_7FD39:;
ax = g_memWord[262382];                 // mov ax, ballNextGroundX
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_goalie_cant_catch_ball;      // js @@goalie_cant_catch_ball

{
    word src = g_memWord[262375];
    int16_t dstSigned = src;
    int16_t srcSigned = g_memWord[270440];
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballDefensiveZ, goalkeeperCatchLimitZ
if (!flags.zero && flags.sign == flags.overflow)
goto l_goalie_cant_catch_ball;      // jg @@goalie_cant_catch_ball

{
    word src = g_memWord[262375];
    int16_t dstSigned = src;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballDefensiveZ, 12
if (flags.zero || flags.sign != flags.overflow)
goto l_goalie_cant_catch_ball;      // jle short @@goalie_cant_catch_ball

esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 2116;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 2116
if (!flags.carry && !flags.zero)
goto l_goalie_cant_catch_ball;      // ja short @@goalie_cant_catch_ball

ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = g_memWord[262373];                 // mov ax, ballDefensiveX
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 12
if (!flags.zero && flags.sign == flags.overflow)
goto l_goalie_cant_catch_ball;      // jg short @@goalie_cant_catch_ball

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = -12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, -12
if (flags.sign != flags.overflow)
goto l_goalie_cant_catch_ball;      // jl short @@goalie_cant_catch_ball

ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = g_memWord[262374];                 // mov ax, ballDefensiveY
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 12
if (!flags.zero && flags.sign == flags.overflow)
goto l_goalie_cant_catch_ball;      // jg short @@goalie_cant_catch_ball

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = -12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, -12
if (flags.sign != flags.overflow)
goto l_goalie_cant_catch_ball;      // jl short @@goalie_cant_catch_ball

if (g_portedToggles.useCpp_GoalkeeperCaughtTheBall) PlayerFunctions::GoalkeeperCaughtTheBall(); else GoalkeeperCaughtTheBall(); // call GoalkeeperCaughtTheBall

l_goalie_cant_catch_ball:;
eax = g_memDword[130777];               // mov eax, lastPlayerPlayed
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_clamp_ball_y_inside_pitch;   // jz @@clamp_ball_y_inside_pitch

esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 72;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 72
if (!flags.carry && !flags.zero)
goto l_clamp_ball_y_inside_pitch;   // ja @@clamp_ball_y_inside_pitch

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_clamp_ball_y_inside_pitch;   // jnz @@clamp_ball_y_inside_pitch

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 4;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_CATCHING_BALL
if (flags.zero)
goto cseg_7FE29;                    // jz short cseg_7FE29

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 67, 1);     // mov al, [esi+TeamGeneralInfo.ball12To17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_clamp_ball_y_inside_pitch;   // jnz @@clamp_ball_y_inside_pitch

cseg_7FE29:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
eax = g_memDword[130776];               // mov eax, lastTeamPlayed
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_opponent_player_touched_the_ball; // jnz @@opponent_player_touched_the_ball

ax = g_memByte[523097] | (g_memByte[523098] << 8); // mov ax, playerHadBall
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_opponent_player_touched_the_ball; // jnz @@opponent_player_touched_the_ball

if (g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper) PlayerFunctions::UpdateBallWithControllingGoalkeeper(); else UpdateBallWithControllingGoalkeeper(); // call UpdateBallWithControllingGoalkeeper
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 32, 4, eax);          // mov [esi+TeamGeneralInfo.controlledPlayerSprite], eax
writeMemory(esi + 36, 4, 0);            // mov [esi+TeamGeneralInfo.passToPlayerPtr], 0
writeMemory(esi + 92, 2, 25);           // mov [esi+TeamGeneralInfo.playerSwitchTimer], 25
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 0);           // mov [esi+TeamGeneralInfo.passKickTimer], 0
writeMemory(esi + 104, 4, 0);           // mov dword ptr [esi+104], 0
writeMemory(esi + 116, 2, 0);           // mov word ptr [esi+116], 0
writeMemory(esi + 140, 2, 1);           // mov [esi+TeamGeneralInfo.goalkeeperPlaying], 1
writeMemory(esi + 86, 1, 1);            // mov byte ptr [esi+TeamGeneralInfo.goaliePlayingOrOut], 1
writeMemory(esi + 84, 1, 1);            // mov byte ptr [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], 1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_opponent_player_touched_the_ball:;
eax = g_memDword[130777];               // mov eax, lastPlayerPlayed
g_memByte[523099] = eax & 0xff;
g_memDword[130775] = (eax >> 8) | (g_memByte[523103] << 24); // mov lastKeeperPlayed, eax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
if (g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper) PlayerFunctions::UpdateBallWithControllingGoalkeeper(); else UpdateBallWithControllingGoalkeeper(); // call UpdateBallWithControllingGoalkeeper
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall

l_clamp_ball_y_inside_pitch:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 60, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.destY], 129
if (flags.sign == flags.overflow)
goto l_check_ball_y_inside_bottom;  // jge short @@check_ball_y_inside_bottom

writeMemory(esi + 60, 2, 129);          // mov [esi+Sprite.destY], 129

l_check_ball_y_inside_bottom:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 60, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 769;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.destY], 769
if (flags.zero || flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jle @@update_player_speed_and_deltas

writeMemory(esi + 60, 2, 769);          // mov [esi+Sprite.destY], 769
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_this_player_last_played:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_not_controlled_player;       // jnz @@not_controlled_player

ax = g_memWord[161949];                 // mov ax, frameCount
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 14;
    *(word *)&D0 = res;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // and word ptr D0, 0Eh
if (!flags.zero)
goto l_clamp_ball_y_inside_pitch;   // jnz short @@clamp_ball_y_inside_pitch

goto l_this_is_substituted_player;      // jmp @@this_is_substituted_player

l_check_pass_to_player:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 84, 2, 0);            // mov [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], 0
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (!flags.zero)
goto l_this_player_last_played;     // jnz short @@this_player_last_played

writeMemory(esi + 36, 4, 0);            // mov [esi+TeamGeneralInfo.passToPlayerPtr], 0
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
goto l_this_player_last_played;         // jmp short @@this_player_last_played

l_player_booked:;
ax = g_memWord[261825];                 // mov ax, whichCard
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

l_go_back_to_normal_state:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_sad_or_happy:;
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 23;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_GOING_TO_HALFTIME
if (flags.zero)
goto l_go_back_to_normal_state;     // jz short @@go_back_to_normal_state

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 24;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_PLAYERS_GOING_TO_SHOWER
if (flags.zero)
goto l_go_back_to_normal_state;     // jz short @@go_back_to_normal_state

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_down_st_10:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_goalie_catching_the_ball:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_check_diving_side;           // jnz short @@check_diving_side

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 82, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperDivingLeft]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

writeMemory(esi + 82, 2, 0);            // mov [esi+TeamGeneralInfo.goalkeeperDivingLeft], 0
if (g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper) PlayerFunctions::UpdateBallWithControllingGoalkeeper(); else UpdateBallWithControllingGoalkeeper(); // call UpdateBallWithControllingGoalkeeper
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall
esi = A2;                               // mov esi, A2
writeMemory(esi + 40, 2, 5);            // mov word ptr [esi+(Sprite.z+2)], 5
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_check_diving_side:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 82, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperDivingLeft]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_if_goalie_close_to_the_ball; // jz short @@check_if_goalie_close_to_the_ball

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_check_if_goalie_close_to_the_ball:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

eax = readMemory(esi + 24, 4);          // mov eax, [esi+TeamGeneralInfo.shotChanceTable]
A0 = eax;                               // mov A0, eax
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D1 = ax;                      // mov word ptr D1, ax
{
    word res = *(word *)&D1 & 240;
    *(word *)&D1 = res;
}                                       // and word ptr D1, 0F0h
{
    word res = *(word *)&D1 >> 4;
    *(word *)&D1 = res;
}                                       // shr word ptr D1, 4
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 48, 2);     // mov ax, [esi+48]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
if (flags.sign)
goto l_goalie_catches_the_ball;     // js short @@goalie_catches_the_ball

//cseg_78D9A();                           // call cseg_78D9A
push(A0);                               // push A0
SWOS::PlayKeeperClaimedComment();       // call PlayKeeperClaimedComment
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_goalie_catches_the_ball:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
esi = A2;                               // mov esi, A2
writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
esi = A6;                               // mov esi, A6
writeMemory(esi + 82, 2, 1);            // mov [esi+Sprite.fullDirection], 1
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall
push(A0);                               // push A0
SWOS::PlayKeeperClaimedComment();       // call PlayKeeperClaimedComment
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_goalie_claimed:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
push(A0);                               // push A0
SWOS::PlayKeeperClaimedComment();       // call PlayKeeperClaimedComment
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_goalie_diving:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_goalkeeper_still_diving;     // jnz short @@goalkeeper_still_diving

l_goalkeeper_rise:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_goalkeeper_still_diving:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 80, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperDivingRight]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_goalie_diving_left;          // jz short @@goalie_diving_left

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 42;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 42
if (!flags.carry && !flags.zero)
goto l_goalie_diving_left;          // ja short @@goalie_diving_left

esi = A6;                               // mov esi, A6
writeMemory(esi + 80, 2, 0);            // mov [esi+TeamGeneralInfo.goalkeeperDivingRight], 0
if (g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper) PlayerFunctions::UpdateBallWithControllingGoalkeeper(); else UpdateBallWithControllingGoalkeeper(); // call UpdateBallWithControllingGoalkeeper
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall
esi = A2;                               // mov esi, A2
writeMemory(esi + 40, 2, 5);            // mov word ptr [esi+(Sprite.z+2)], 5
goto l_goalkeeper_rise;                 // jmp short @@goalkeeper_rise

l_goalie_diving_left:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, 100
if (!flags.zero)
goto cseg_80282;                    // jnz short cseg_80282

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 60;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 60
if (flags.carry || flags.zero)
goto l_goalkeeper_rise;             // jbe short @@goalkeeper_rise

cseg_80282:;
ax = g_memWord[162031];                 // mov ax, dseg_110611
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 70, 1);     // mov al, byte ptr [esi+TeamGeneralInfo.field_46]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_802A3;                    // jz short cseg_802A3

goto l_set_new_goalkeeper_speed;        // jmp @@set_new_goalkeeper_speed

cseg_802A3:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 976;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], SPR_GOALIE1_CAUGHT_BALL_RIGHT
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 978;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], SPR_GOALIE1_CAUGHT_BALL_LEFT
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1106;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], SPR_GOALIE2_CAUGHT_BALL_RIGHT
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1108;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], SPR_GOALIE2_CAUGHT_BALL_LEFT
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1092;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], 1092
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz short @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1094;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], 1094
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz short @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 990;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], 990
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz short @@goalkeeper_on_ground

{
    word src = (word)readMemory(esi + 70, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 992;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.imageIndex], 992
if (flags.zero)
goto l_goalkeeper_on_ground;        // jz short @@goalkeeper_on_ground

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522804;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset topTeamData
if (!flags.zero)
goto cseg_80360;                    // jnz short cseg_80360

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = -5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, -5
if (flags.sign != flags.overflow)
goto cseg_8037A;                    // jl short cseg_8037A

goto l_set_new_goalkeeper_speed;        // jmp short @@set_new_goalkeeper_speed

cseg_80360:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 5
if (!flags.zero && flags.sign == flags.overflow)
goto cseg_8037A;                    // jg short cseg_8037A

goto l_set_new_goalkeeper_speed;        // jmp short @@set_new_goalkeeper_speed

l_goalkeeper_on_ground:;
ax = g_memWord[162032];                 // mov ax, dseg_110613
*(word *)&D0 = ax;                      // mov word ptr D0, ax
goto l_set_new_goalkeeper_speed;        // jmp short @@set_new_goalkeeper_speed

cseg_8037A:;
ax = g_memWord[162033];                 // mov ax, dseg_110615
*(word *)&D0 = ax;                      // mov word ptr D0, ax

l_set_new_goalkeeper_speed:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
if (!flags.sign)
goto cseg_803A4;                    // jns short cseg_803A4

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0

cseg_803A4:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, 100
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 70, 1);     // mov al, byte ptr [esi+TeamGeneralInfo.field_46]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_803C4;                    // jz short cseg_803C4

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_803C4:;
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto cseg_803E8;                    // jz short cseg_803E8

esi = A2;                               // mov esi, A2
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
D0 = eax;                               // mov D0, eax
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.sign)
goto l_update_player_speed_and_deltas; // jns @@update_player_speed_and_deltas

goto cseg_80404;                        // jmp short cseg_80404

cseg_803E8:;
esi = A2;                               // mov esi, A2
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
D0 = eax;                               // mov D0, eax
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

if (flags.sign)
goto l_update_player_speed_and_deltas; // js @@update_player_speed_and_deltas

cseg_80404:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 70, 2);     // mov ax, [esi+Sprite.imageIndex]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_update_player_speed_and_deltas; // js @@update_player_speed_and_deltas

eax = g_memDword[80979];                // mov eax, g_spriteGraphicsPtr
A0 = eax;                               // mov A0, eax
{
    word res = *(word *)&D0 << 2;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 2
esi = A0;                               // mov esi, A0
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
eax = readMemory(esi + ebx, 4);         // mov eax, [esi+ebx]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 10, 2);     // mov ax, [esi+SpriteGraphics.pixWidth]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
eax = readMemory(esi + 46, 4);          // mov eax, [esi+Sprite.deltaX]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.sign)
goto cseg_804C6;                    // jns short cseg_804C6

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, 2
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 6;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 6
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 3;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, 3
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (!flags.carry)
goto l_update_player_speed_and_deltas; // jnb @@update_player_speed_and_deltas

ax = D0;                                // mov ax, word ptr D0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (flags.carry || flags.zero)
goto l_update_player_speed_and_deltas; // jbe @@update_player_speed_and_deltas

goto cseg_80519;                        // jmp short cseg_80519

cseg_804C6:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 2;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 2
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 6;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 6
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 3;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 3
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

ax = D0;                                // mov ax, word ptr D0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

cseg_80519:;
esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 20;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 20
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80540;                    // jnz short cseg_80540

ax = g_memWord[261556];                 // mov ax, penalty
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_805A7;                    // jz short cseg_805A7

cseg_80540:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 5
if (!flags.zero && flags.sign == flags.overflow)
goto l_update_player_speed_and_deltas; // jg @@update_player_speed_and_deltas

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = -5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, -5
if (flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jl @@update_player_speed_and_deltas

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 2;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 2
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
goto cseg_80616;                        // jmp short cseg_80616

cseg_805A7:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 7;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 7
if (!flags.zero && flags.sign == flags.overflow)
goto l_update_player_speed_and_deltas; // jg @@update_player_speed_and_deltas

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = -7;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, -7
if (flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jl @@update_player_speed_and_deltas

esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0

cseg_80616:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 70, 1, 1);            // mov byte ptr [esi+70], 1
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 6;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_GOALIE_DIVING_HIGH
if (flags.zero)
goto l_goalie_jumping_high;         // jz short @@goalie_jumping_high

{
    int16_t res = *(int16_t *)&D0 >> 1;
    flags.carry = ((word) * (int16_t *)&D0 >> 15) & 1;
    flags.overflow = (((*(int16_t *)&D0 >> 15) & 1)) != 0;
    *(int16_t *)&D0 = res;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // sar word ptr D0, 1
goto cseg_8064D;                        // jmp short cseg_8064D

l_goalie_jumping_high:;
{
    int16_t res = *(int16_t *)&D0 >> 2;
    *(int16_t *)&D0 = res;
}                                       // sar word ptr D0, 2

cseg_8064D:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 24, 4);          // mov eax, [esi+24]
A0 = eax;                               // mov A0, eax
ax = g_memWord[261556];                 // mov ax, penalty
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80681;                    // jnz short cseg_80681

ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_8068B;                    // jz short cseg_8068B

cseg_80681:;
A0 = 526915;                            // mov A0, offset dseg_17EECC

cseg_8068B:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A5 = eax;                               // mov A5, eax
esi = A5;                               // mov esi, A5
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto cseg_806EE;                    // js short cseg_806EE

esi = A5;                               // mov esi, A5
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0

cseg_806EE:;
ax = g_memWord[262387];                 // mov ax, ballInGoalkeeperArea
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_ball_not_in_goalkeeper_area; // jnz short @@ball_not_in_goalkeeper_area

esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A5 = eax;                               // mov A5, eax
esi = A5;                               // mov esi, A5
eax = readMemory(esi + 14, 4);          // mov eax, [esi+TeamGeneralInfo.teamStatsPtr]
A5 = eax;                               // mov A5, eax
esi = A5;                               // mov esi, A5
{
    word src = (word)readMemory(esi + 10, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 10, 2, src);
}                                       // add [esi+TeamStatisticsData.goalAttempts], 1

l_ball_not_in_goalkeeper_area:;
if (g_portedToggles.useCpp_ResetBothTeamSpinTimers) PlayerFunctions::ResetBothTeamSpinTimers(); else ResetBothTeamSpinTimers();
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto cseg_8077F;                    // jns short cseg_8077F

ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D1 = ax;                      // mov word ptr D1, ax
{
    word res = *(word *)&D1 & 240;
    *(word *)&D1 = res;
}                                       // and word ptr D1, 0F0h
{
    word res = *(word *)&D1 >> 4;
    *(word *)&D1 = res;
}                                       // shr word ptr D1, 4
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
if (flags.sign)
goto cseg_807CB;                    // js short cseg_807CB

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
if (flags.sign)
goto cseg_80A1B;                    // js cseg_80A1B

goto cseg_808BF;                        // jmp cseg_808BF

cseg_8077F:;
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D1 = ax;                      // mov word ptr D1, ax
{
    word res = *(word *)&D1 & 240;
    *(word *)&D1 = res;
}                                       // and word ptr D1, 0F0h
{
    word res = *(word *)&D1 >> 4;
    *(word *)&D1 = res;
}                                       // shr word ptr D1, 4
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
if (flags.sign)
goto cseg_807CB;                    // js short cseg_807CB

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
if (flags.sign)
goto cseg_808BF;                    // js cseg_808BF

goto cseg_80A1B;                        // jmp cseg_80A1B

cseg_807CB:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 6;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 6
if (!flags.zero && flags.sign == flags.overflow)
goto cseg_80A1B;                    // jg cseg_80A1B

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = -6;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, -6
if (flags.sign != flags.overflow)
goto cseg_80A1B;                    // jl cseg_80A1B

eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A2;                               // mov esi, A2
writeMemory(esi + 32, 2, ax);           // mov word ptr [esi+(Sprite.x+2)], ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A2;                               // mov esi, A2
writeMemory(esi + 36, 2, ax);           // mov word ptr [esi+(Sprite.y+2)], ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
esi = A6;                               // mov esi, A6
writeMemory(esi + 80, 2, 1);            // mov [esi+TeamGeneralInfo.goalkeeperDivingRight], 1
push(D0);                               // push D0
push(A6);                               // push A6
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall
pop(A6);                                // pop A6
pop(D0);                                // pop D0
push(A0);                               // push A0
SWOS::PlayGoalkeeperSavedComment();     // call PlayGoalkeeperSavedComment
SWOS::PlayMissGoalSample();             // call PlayMissGoalSample
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_808BF:;
eax = g_memDword[130777];               // mov eax, lastPlayerPlayed
g_memByte[523099] = eax & 0xff;
g_memDword[130775] = (eax >> 8) | (g_memByte[523103] << 24); // mov lastKeeperPlayed, eax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 2;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 2
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
{
    word src = g_memWord[164516];
    int16_t dstSigned = src;
    int16_t srcSigned = 1792;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballSprite.speed, 1792
if (flags.carry || flags.zero)
goto cseg_80919;                    // jbe short cseg_80919

g_memWord[164516] = 1792;               // mov ballSprite.speed, 1792

cseg_80919:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto cseg_8095F;                    // jns short cseg_8095F

ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 15;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0Fh
{
    word res = *(word *)&D0 << 7;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 7
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 960;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 960
ax = D0;                                // mov ax, word ptr D0
{
    word src = g_memWord[164523];
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    g_memWord[164523] = src;
}                                       // add ballSprite.destX, ax
goto cseg_80B1D;                        // jmp cseg_80B1D

cseg_8095F:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 60, 2);     // mov ax, [esi+Sprite.destY]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = D1;                                // mov ax, word ptr D1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    word res = *(word *)&D2 & 14;
    *(word *)&D2 = res;
}                                       // and word ptr D2, 0Eh
A0 = 325544;                            // mov A0, offset dseg_110BDB
esi = A0;                               // mov esi, A0
ebx = *(word *)&D2;                     // movzx ebx, word ptr D2
ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
cl = D2;                                // mov cl, byte ptr D2
{
    byte shiftCount = (int8_t)cl & 0x1f;
    if (shiftCount) {
        int16_t res = *(int16_t *)&D0 >> shiftCount;
        *(int16_t *)&D0 = res;
    }
}                                       // sar word ptr D0, cl
*(int16_t *)&D0 = -*(int16_t *)&D0;     // neg word ptr D0
ax = D0;                                // mov ax, word ptr D0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
ax = D1;                                // mov ax, word ptr D1
esi = A2;                               // mov esi, A2
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+Sprite.destX]
esi = A2;                               // mov esi, A2
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
goto cseg_80B1D;                        // jmp cseg_80B1D

cseg_80A1B:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 60, 2);     // mov ax, [esi+Sprite.destY]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = D1;                                // mov ax, word ptr D1
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
ax = D0;                                // mov ax, word ptr D0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
ax = g_memWord[161952];                 // mov ax, currentGameTick
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 & 31;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 1Fh
{
    word res = *(word *)&D0 << 4;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 4
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 256;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 256
ax = D0;                                // mov ax, word ptr D0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
ax = D1;                                // mov ax, word ptr D1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+Sprite.destX]
esi = A2;                               // mov esi, A2
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 2;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 2
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
{
    byte src = g_memByte[323904];
    byte res = src & 16;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // test byte ptr currentGameTick, 10h
if (flags.zero)
goto cseg_80B07;                    // jz short cseg_80B07

{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax

cseg_80B07:;
{
    word src = g_memWord[164516];
    int16_t dstSigned = src;
    int16_t srcSigned = 1536;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballSprite.speed, 1536
if (flags.carry || flags.zero)
goto cseg_80B1B;                    // jbe short cseg_80B1B

g_memWord[164516] = 1536;               // mov ballSprite.speed, 1536

cseg_80B1B:;

cseg_80B1D:;
*(word *)&D0 = 1;                       // mov word ptr D0, 1
{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522804;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset topTeamData
if (flags.zero)
goto cseg_80B39;                    // jz short cseg_80B39

*(int16_t *)&D0 = -*(int16_t *)&D0;     // neg word ptr D0

cseg_80B39:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A2;                               // mov esi, A2
writeMemory(esi + 36, 2, ax);           // mov word ptr [esi+(Sprite.y+2)], ax
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 36, 2, src);
}                                       // add word ptr [esi+(Sprite.y+2)], ax

cseg_80B5F:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 76, 2);     // mov ax, [esi+TeamGeneralInfo.goalkeeperSavedCommentTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto cseg_80BA0;                    // js short cseg_80BA0

esi = A0;                               // mov esi, A0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0

cseg_80BA0:;
push(A0);                               // push A0
SWOS::PlayGoalkeeperSavedComment();     // call PlayGoalkeeperSavedComment
SWOS::PlayMissGoalSample();             // call PlayMissGoalSample
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_its_controlled_player:;
ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_for_cpu_team;          // jz short @@check_for_cpu_team

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 31;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_PENALTIES
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

l_check_for_cpu_team:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80C0C;                    // jnz short cseg_80C0C

push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
AI::CallWithComparison(g_portedToggles.useCpp_AI_SetControlsDirection);
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1

cseg_80C0C:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
writeMemory(esi + 42, 2, ax);           // mov [esi+TeamGeneralInfo.allowedDirections], ax
writeMemory(esi + 40, 2, 0);            // mov [esi+TeamGeneralInfo.playerHasBall], 0
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+TeamGeneralInfo.shooting]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto cseg_80C56;                    // jz short cseg_80C56

ax = (word)readMemory(esi + 50, 2);     // mov ax, word ptr [esi+TeamGeneralInfo.firePressed]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80C56;                    // jnz short cseg_80C56

writeMemory(esi + 58, 2, 0);            // mov [esi+TeamGeneralInfo.shooting], 0

cseg_80C56:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_skip_break_handling;         // jz @@skip_break_handling

eax = g_memDword[130780];               // mov eax, lastTeamPlayedBeforeBreak
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (!flags.zero)
goto cseg_80CB3;                    // jnz short cseg_80CB3

ax = g_memWord[261570];                 // mov ax, cameraDirection
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
{
    word src = g_memWord[227970];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    g_memWord[227970] = src;
}                                       // add dseg_132806, 1

cseg_80CB3:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (!flags.zero)
goto l_turn_flags_acceptable;       // jnz short @@turn_flags_acceptable

*(word *)&D0 = 7;                       // mov word ptr D0, 7

l_find_acceptable_turn_flags_loop:;
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (!flags.zero)
goto l_created_acceptable_turn_flags; // jnz short @@created_acceptable_turn_flags

(*(int16_t *)&D0)--;
flags.overflow = (int16_t)(*(int16_t *)&D0) == INT16_MIN;
flags.sign = (*(int16_t *)&D0 & 0x8000) != 0;
flags.zero = *(int16_t *)&D0 == 0;      // dec word ptr D0
if (!flags.sign)
goto l_find_acceptable_turn_flags_loop; // jns short @@find_acceptable_turn_flags_loop

*(word *)&D0 = 0;                       // mov word ptr D0, 0

l_created_acceptable_turn_flags:;
ax = D0;                                // mov ax, word ptr D0
g_memWord[261570] = ax;                 // mov cameraDirection, ax
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
{
    word src = g_memWord[227972];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    g_memWord[227972] = src;
}                                       // add deadThrowInDirectionVar, 1

l_turn_flags_acceptable:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80D49;                    // jnz short cseg_80D49

{
    word src = g_memWord[261563];
    int16_t dstSigned = src;
    int16_t srcSigned = g_memWord[270442];
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp stoppageTimerActive, hideResultInterval
if (!flags.carry && !flags.zero)
goto l_hide_result;                 // ja short @@hide_result

goto l_test_fire;                       // jmp short @@test_fire

cseg_80D49:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto l_joy_any_fire_pressed;        // jns short @@joy_any_fire_pressed

l_test_fire:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 50, 1);     // mov al, [esi+TeamGeneralInfo.firePressed]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_joy_any_fire_pressed;        // jnz short @@joy_any_fire_pressed

al = (byte)readMemory(esi + 48, 1);     // mov al, [esi+TeamGeneralInfo.quickFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_joy_any_fire_pressed;        // jnz short @@joy_any_fire_pressed

al = (byte)readMemory(esi + 49, 1);     // mov al, [esi+TeamGeneralInfo.normalFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_test_direction;              // jz short @@test_direction

l_joy_any_fire_pressed:;
ax = g_memWord[224870];                 // mov ax, statsTimer
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_hide_result;                 // jz short @@hide_result

g_memWord[261559] = 1;                  // mov fireBlocked, 1

l_hide_result:;
{
    int16_t src = g_memWord[224872];
    src = -src;
    g_memWord[224872] = src;
}                                       // neg timeVar
{
    int16_t src = g_memWord[224871];
    src = -src;
    g_memWord[224871] = src;
}                                       // neg resultTimer

l_test_direction:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto cseg_80DCC;                    // jns short cseg_80DCC

cseg_80DB6:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
esi = A6;                               // mov esi, A6
writeMemory(esi + 44, 2, ax);           // mov [esi+TeamGeneralInfo.currentAllowedDirection], ax
goto l_skip_break_handling;             // jmp short @@skip_break_handling

cseg_80DCC:;
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (flags.zero)
goto cseg_80DB6;                    // jz short cseg_80DB6

ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
if (g_portedToggles.useCpp_UpdatePlayerWithBall) PlayerFunctions::UpdatePlayerWithBall(); else UpdatePlayerWithBall(); // call UpdatePlayerWithBall

l_skip_break_handling:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto cseg_80E41;                    // jns short cseg_80E41

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax

cseg_80E41:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_test_quick_fire;             // jnz @@test_quick_fire

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 48, 2);     // mov ax, word ptr [esi+TeamGeneralInfo.quickFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_test_quick_fire;             // jnz @@test_quick_fire

ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+TeamGeneralInfo.shooting]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_test_quick_fire;             // jnz @@test_quick_fire

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_80E8F;                    // jnz short cseg_80E8F

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_80ECF;                    // jz short cseg_80ECF

cseg_80E8F:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+TeamGeneralInfo.allowedDirections]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_80EAA;                    // jnz short cseg_80EAA

writeMemory(esi + 108, 2, 0);           // mov [esi+TeamGeneralInfo.unkBallTimer], 0

cseg_80EAA:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 40, 2, 1);            // mov [esi+TeamGeneralInfo.playerHasBall], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1

cseg_80ECF:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto cseg_80FFD;                    // jns cseg_80FFD

ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_jmp_update_player_speed;     // jz @@jmp_update_player_speed

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_jmp_update_player_speed;     // jz @@jmp_update_player_speed

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 51, 1);     // mov al, [esi+TeamGeneralInfo.fireThisFrame]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_jmp_update_player_speed;     // jz @@jmp_update_player_speed

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto l_jmp_update_player_speed;     // jns @@jmp_update_player_speed

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_80F61;                    // jnz short cseg_80F61

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_80F61;                    // jnz short cseg_80F61

al = (byte)readMemory(esi + 63, 1);     // mov al, [esi+TeamGeneralInfo.plNotFarFromBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_jmp_update_player_speed;     // jz @@jmp_update_player_speed

cseg_80F61:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 66, 1);     // mov al, [esi+TeamGeneralInfo.ball8To12]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_80F88;                    // jnz short cseg_80F88

al = (byte)readMemory(esi + 67, 1);     // mov al, [esi+TeamGeneralInfo.ball12To17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_80F88;                    // jnz short cseg_80F88

al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_jmp_update_player_speed;     // jz short @@jmp_update_player_speed

cseg_80F88:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 52, 2, 1);            // mov [esi+TeamGeneralInfo.headerOrTackle], 1
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_AttemptStaticHeader) PlayerFunctions::AttemptStaticHeader(); else AttemptStaticHeader(); // call AttemptStaticHeader
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_jmp_update_player_speed:;
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_80FFD:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_81028;                    // jnz short cseg_81028

esi = A6;                               // mov esi, A6
writeMemory(esi + 138, 2, 0);           // mov [esi+TeamGeneralInfo.wonTheBallTimer], 0

cseg_81028:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 138, 2);    // mov ax, [esi+TeamGeneralInfo.wonTheBallTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_81793;                    // jnz cseg_81793

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_test_quick_fire;             // jz @@test_quick_fire

al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_test_quick_fire;             // jnz @@test_quick_fire

al = (byte)readMemory(esi + 67, 1);     // mov al, [esi+TeamGeneralInfo.ball12To17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_test_quick_fire;             // jnz @@test_quick_fire

al = (byte)readMemory(esi + 69, 1);     // mov al, [esi+TeamGeneralInfo.prevPlVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_8108A;                    // jnz short cseg_8108A

writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0

cseg_8108A:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 112, 2);    // mov ax, [esi+TeamGeneralInfo.ballControllingPlayerDirection]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (flags.zero)
goto l_ball_becomes_free;           // jz short @@ball_becomes_free

writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0

l_ball_becomes_free:;
esi = A6;                               // mov esi, A6
{
    word src = (word)readMemory(esi + 110, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    writeMemory(esi + 110, 2, src);
}                                       // add [esi+TeamGeneralInfo.ballCanBeControlled], 1
ax = (word)readMemory(esi + 110, 2);    // mov ax, [esi+TeamGeneralInfo.ballCanBeControlled]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = D0;                                // mov ax, word ptr D0
writeMemory(esi + 112, 2, ax);          // mov [esi+TeamGeneralInfo.ballControllingPlayerDirection], ax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_calculate_if_player_wins_ball; // jz short @@calculate_if_player_wins_ball

g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1

l_calculate_if_player_wins_ball:;
push(D0);                               // push D0
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_CalculateIfPlayerWinsBall) PlayerFunctions::CalculateIfPlayerWinsBall(); else CalculateIfPlayerWinsBall(); // call CalculateIfPlayerWinsBall
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
pop(D0);                                // pop D0
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto cseg_81793;                        // jmp cseg_81793

l_test_quick_fire:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 48, 1);     // mov al, [esi+TeamGeneralInfo.quickFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_no_passing;                  // jz @@no_passing

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_no_passing;                  // js @@no_passing

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_811DF;                    // jnz short cseg_811DF

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_no_passing;                  // jz @@no_passing

cseg_811DF:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto cseg_81203;                    // jz short cseg_81203

cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (flags.zero)
goto l_no_passing;                  // jz @@no_passing

cseg_81203:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto cseg_81221;                    // jz short cseg_81221

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_no_passing;                  // jz @@no_passing

cseg_81221:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
esi = A2;                               // mov esi, A2
writeMemory(esi + 40, 2, 0);            // mov word ptr [esi+(Sprite.z+2)], 0
push(D0);                               // push D0
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_DoPass) PlayerFunctions::DoPass(); else DoPass(); // call DoPass
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
pop(D0);                                // pop D0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto cseg_812C6;                    // jz short cseg_812C6

push(D0);                               // push D0
//ThrowInDeadProc();                      // call ThrowInDeadProc
pop(D0);                                // pop D0
g_memWord[261558] = 0;                  // mov gameNotInProgressCounterWriteOnly, 0

cseg_812C6:;
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 14;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_PENALTY
if (!flags.zero)
goto l_not_penalty;                 // jnz short @@not_penalty

g_memWord[261556] = 1;                  // mov penalty, 1

l_not_penalty:;
g_memWord[261564] = 100;                // mov gameStatePl, 100
g_memWord[261565] = 100;                // mov gameState, ST_GAME_IN_PROGRESS
esi = A6;                               // mov esi, A6
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
esi = A6;                               // mov esi, A6
writeMemory(esi + 32, 4, 0);            // mov [esi+TeamGeneralInfo.controlledPlayerSprite], 0
eax = A1;                               // mov eax, A1
writeMemory(esi + 104, 4, eax);         // mov [esi+TeamGeneralInfo.passingKickingPlayer], eax
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
eax = readMemory(esi, 4);               // mov eax, [esi]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_stop_player;                     // jmp @@stop_player

l_no_passing:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 49, 1);     // mov al, [esi+TeamGeneralInfo.normalFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_check_if_goalkeeper;         // jz @@check_if_goalkeeper

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_check_if_goalkeeper;         // js @@check_if_goalkeeper

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_813DA;                    // jnz short cseg_813DA

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_check_if_goalkeeper;         // jz @@check_if_goalkeeper

cseg_813DA:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, 100
if (flags.zero)
goto cseg_813FE;                    // jz short cseg_813FE

cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (flags.zero)
goto l_check_if_goalkeeper;         // jz @@check_if_goalkeeper

cseg_813FE:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto cseg_8141C;                    // jz short cseg_8141C

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_check_if_goalkeeper;         // jz @@check_if_goalkeeper

cseg_8141C:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
push(D0);                               // push D0
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
ComparePlayerKickingBall(g_portedToggles.useCpp_PlayerKickingBall); // call PlayerKickingBall with comparison
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
pop(D0);                                // pop D0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, 100
if (flags.zero)
goto cseg_814B5;                    // jz short cseg_814B5

push(D0);                               // push D0
//ThrowInDeadProc();                      // call ThrowInDeadProc
pop(D0);                                // pop D0
g_memWord[261558] = 0;                  // mov gameNotInProgressCounterWriteOnly, 0

cseg_814B5:;
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 14;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_PENALTY
if (!flags.zero)
goto l_not_penalty2;                // jnz short @@not_penalty2

g_memWord[261556] = 1;                  // mov penalty, 1

l_not_penalty2:;
g_memWord[261564] = 100;                // mov gameStatePl, 100
g_memWord[261565] = 100;                // mov gameState, ST_GAME_IN_PROGRESS
esi = A6;                               // mov esi, A6
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
esi = A6;                               // mov esi, A6
writeMemory(esi + 32, 4, 0);            // mov [esi+TeamGeneralInfo.controlledPlayerSprite], 0
eax = A1;                               // mov eax, A1
writeMemory(esi + 104, 4, eax);         // mov [esi+TeamGeneralInfo.passingKickingPlayer], eax
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
writeMemory(esi + 58, 2, 1);            // mov [esi+TeamGeneralInfo.shooting], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_stop_player;                     // jmp @@stop_player

l_check_if_goalkeeper:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto cseg_8167F;                    // jz cseg_8167F

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 51, 1);     // mov al, [esi+TeamGeneralInfo.fireThisFrame]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_8167F;                    // jz cseg_8167F

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto cseg_8167F;                    // js cseg_8167F

al = (byte)readMemory(esi + 63, 1);     // mov al, [esi+TeamGeneralInfo.plNotFarFromBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_8167F;                    // jz cseg_8167F

al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_815F7;                    // jnz short cseg_815F7

al = (byte)readMemory(esi + 65, 1);     // mov al, [esi+TeamGeneralInfo.ball4To8]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_8167F;                    // jz cseg_8167F

cseg_815F7:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
A5 = eax;                               // mov A5, eax
{
    int32_t dstSigned = A5;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A5, 0
if (flags.zero)
goto l_begin_tackling;              // jz short @@begin_tackling

esi = A5;                               // mov esi, A5
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
D1 = eax;                               // mov D1, eax
esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D1, eax
if (flags.carry)
goto cseg_8167F;                    // jb short cseg_8167F

l_begin_tackling:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 52, 2, 1);            // mov [esi+TeamGeneralInfo.headerOrTackle], 1
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerBeginTackling) PlayerFunctions::PlayerBeginTackling(); else PlayerBeginTackling(); // call PlayerBeginTackling
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
esi = A6;                               // mov esi, A6
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_8167F:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_not_a_header;                // jz @@not_a_header

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 51, 1);     // mov al, [esi+TeamGeneralInfo.fireThisFrame]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_not_a_header;                // jz @@not_a_header

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_not_a_header;                // js @@not_a_header

al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_816E5;                    // jnz short cseg_816E5

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_816E5;                    // jnz short cseg_816E5

al = (byte)readMemory(esi + 63, 1);     // mov al, [esi+TeamGeneralInfo.plNotFarFromBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_not_a_header;                // jz @@not_a_header

cseg_816E5:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 66, 1);     // mov al, [esi+TeamGeneralInfo.ball8To12]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_its_a_header;                // jnz short @@its_a_header

al = (byte)readMemory(esi + 67, 1);     // mov al, [esi+TeamGeneralInfo.ball12To17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_its_a_header;                // jnz short @@its_a_header

al = (byte)readMemory(esi + 68, 1);     // mov al, [esi+TeamGeneralInfo.ballAbove17]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_not_a_header;                // jz short @@not_a_header

l_its_a_header:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 52, 2, 1);            // mov [esi+TeamGeneralInfo.headerOrTackle], 1
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerAttemptingJumpHeader) PlayerFunctions::PlayerAttemptingJumpHeader(); else PlayerAttemptingJumpHeader(); // call PlayerAttemptingJumpHeader
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_not_a_header:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_update_player_speed_and_deltas; // js @@update_player_speed_and_deltas

cseg_81793:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
*(byte *)&D3 = 0;                       // mov byte ptr D3, 0
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 79;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 79
if (flags.sign == flags.overflow)
goto l_inside_pitch_left_x;         // jge short @@inside_pitch_left_x

*(byte *)&D3 |= 224;                    // or byte ptr D3, 0E0h

l_inside_pitch_left_x:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 592;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 592
if (flags.zero || flags.sign != flags.overflow)
goto l_inside_pitch_right_x;        // jle short @@inside_pitch_right_x

*(byte *)&D3 |= 14;                     // or byte ptr D3, 0Eh

l_inside_pitch_right_x:;
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 127;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 127
if (flags.sign == flags.overflow)
goto l_inside_pitch_top_y;          // jge short @@inside_pitch_top_y

*(byte *)&D3 |= 131;                    // or byte ptr D3, 83h

l_inside_pitch_top_y:;
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 771;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 771
if (flags.zero || flags.sign != flags.overflow)
goto l_inside_pitch_bottom_y;       // jle short @@inside_pitch_bottom_y

*(byte *)&D3 |= 56;                     // or byte ptr D3, 38h

l_inside_pitch_bottom_y:;
cx = D0;                                // mov cx, word ptr D0
eax = 1;                                // mov eax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        dword res = eax << shiftCount;
        eax = res;
    }
}                                       // shl eax, cl
{
    dword res = D3 & eax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test D3, eax
if (flags.zero)
goto l_update_player_dest_x_y;      // jz short @@update_player_dest_x_y

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_update_player_dest_x_y:;
A5 = 523306;                            // mov A5, offset kDefaultDestinations
{
    word res = *(word *)&D0 << 2;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 2
eax = A5;                               // mov eax, A5
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
{
    int32_t dstSigned = eax;
    int32_t srcSigned = ebx;
    dword res = dstSigned + srcSigned;
    eax = res;
}                                       // add eax, ebx
A5 = eax;                               // mov A5, eax
esi = A5;                               // mov esi, A5
ax = (word)readMemory(esi, 2);          // mov ax, [esi]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
esi = A5;                               // mov esi, A5
ax = (word)readMemory(esi + 2, 2);      // mov ax, [esi+2]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D2 = res;
}                                       // add word ptr D2, ax
ax = D1;                                // mov ax, word ptr D1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = D2;                                // mov ax, word ptr D2
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_taking_throw_in:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_test_allowed_turn_flags;     // jnz short @@test_allowed_turn_flags

push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
AI::CallWithComparison(g_portedToggles.useCpp_AI_SetControlsDirection);
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1

l_test_allowed_turn_flags:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    byte src = g_memByte[523142];
    byte res = src & al;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // test byte ptr playerTurnFlags, al
if (!flags.zero)
goto l_test_turn_flags_with_camera_direction; // jnz short @@test_turn_flags_with_camera_direction

ax = g_memWord[261570];                 // mov ax, cameraDirection
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
{
    word src = g_memWord[227975];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    g_memWord[227975] = src;
}                                       // add disallowedTurnFlagsCounter, 1

l_test_turn_flags_with_camera_direction:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    byte src = g_memByte[523142];
    byte res = src & al;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // test byte ptr playerTurnFlags, al
if (!flags.zero)
goto l_check_if_throw_in_taker_substituted; // jnz short @@check_if_throw_in_taker_substituted

*(word *)&D0 = 7;                       // mov word ptr D0, 7

l_next_direction:;
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    byte src = g_memByte[523142];
    byte res = src & al;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // test byte ptr playerTurnFlags, al
if (!flags.zero)
goto l_found_direction;             // jnz short @@found_direction

(*(int16_t *)&D0)--;
flags.overflow = (int16_t)(*(int16_t *)&D0) == INT16_MIN;
flags.sign = (*(int16_t *)&D0 & 0x8000) != 0;
flags.zero = *(int16_t *)&D0 == 0;      // dec word ptr D0
if (!flags.sign)
goto l_next_direction;              // jns short @@next_direction

*(word *)&D0 = 0;                       // mov word ptr D0, 0

l_found_direction:;
ax = D0;                                // mov ax, word ptr D0
g_memWord[261570] = ax;                 // mov cameraDirection, ax
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
{
    word src = g_memWord[227972];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    g_memWord[227972] = src;
}                                       // add deadThrowInDirectionVar, 1

l_check_if_throw_in_taker_substituted:;
ax = g_memWord[243084];                 // mov ax, g_substituteInProgress
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_throw_in_game_state;   // jz short @@check_throw_in_game_state

eax = g_memDword[121543];               // mov eax, substitutedPlSprite
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_abort_throw_in;              // jz @@abort_throw_in

l_check_throw_in_game_state:;
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 15;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_THROW_IN_FORWARD_RIGHT
if (flags.carry)
goto l_abort_throw_in;              // jb @@abort_throw_in

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 20;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_THROW_IN_BACK_LEFT
if (!flags.carry && !flags.zero)
goto l_abort_throw_in;              // ja @@abort_throw_in

esi = A1;                               // mov esi, A1
al = (byte)readMemory(esi + 13, 1);     // mov al, [esi+Sprite.playerDownTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_ready_for_throw_in;          // jz @@ready_for_throw_in

{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (flags.zero)
goto l_throw_in_over;               // jz @@throw_in_over

{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 18;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 18
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

ax = g_memWord[243077];                 // mov ax, g_inSubstitutesMenu
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_throw_in_done_check_pass_or_kick; // jz short @@throw_in_done_check_pass_or_kick

A0 = 453788;                            // mov A0, offset aboutToThrowInAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 5);            // mov [esi+Sprite.playerState], PL_THROW_IN
writeMemory(esi + 13, 1, 0);            // mov [esi+Sprite.playerDownTimer], 0
goto l_ready_for_throw_in;              // jmp short @@ready_for_throw_in

l_throw_in_done_check_pass_or_kick:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A2;                               // mov esi, A2
writeMemory(esi + 40, 2, 12);           // mov word ptr [esi+(Sprite.z+2)], 12
g_memWord[224900] = 0;                  // mov hideBall, 0
ax = g_memWord[261576];                 // mov ax, throwInPassOrKick
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_do_throw_in_pass;            // jnz @@do_throw_in_pass

goto l_do_throw_in_kick;                // jmp @@do_throw_in_kick

l_ready_for_throw_in:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_throw_in_check_input_direction; // jnz short @@throw_in_check_input_direction

{
    word src = g_memWord[261563];
    int16_t dstSigned = src;
    int16_t srcSigned = g_memWord[270442];
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp stoppageTimerActive, hideResultInterval
if (!flags.carry && !flags.zero)
goto l_throw_in_hide_result;        // ja short @@throw_in_hide_result

goto l_throw_in_check_fire;             // jmp short @@throw_in_check_fire

l_throw_in_check_input_direction:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto l_check_if_stats_showing;      // jns short @@check_if_stats_showing

l_throw_in_check_fire:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 50, 1);     // mov al, [esi+TeamGeneralInfo.firePressed]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_check_if_stats_showing;      // jnz short @@check_if_stats_showing

al = (byte)readMemory(esi + 48, 1);     // mov al, [esi+TeamGeneralInfo.quickFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_check_if_stats_showing;      // jnz short @@check_if_stats_showing

al = (byte)readMemory(esi + 49, 1);     // mov al, [esi+TeamGeneralInfo.normalFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_throw_in_check_direction;    // jz short @@throw_in_check_direction

l_check_if_stats_showing:;
ax = g_memWord[224870];                 // mov ax, statsTimer
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_throw_in_hide_result;        // jz short @@throw_in_hide_result

g_memWord[261559] = 1;                  // mov fireBlocked, 1

l_throw_in_hide_result:;
{
    int16_t src = g_memWord[224872];
    src = -src;
    g_memWord[224872] = src;
}                                       // neg timeVar
{
    int16_t src = g_memWord[224871];
    src = -src;
    g_memWord[224871] = src;
}                                       // neg resultTimer

l_throw_in_check_direction:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto l_throw_in_got_input_direction; // jns short @@throw_in_got_input_direction

l_throw_in_use_sprite_direction:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
esi = A6;                               // mov esi, A6
writeMemory(esi + 44, 2, ax);           // mov [esi+TeamGeneralInfo.currentAllowedDirection], ax
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
goto l_throw_in_check_quick_fire;       // jmp short @@throw_in_check_quick_fire

l_throw_in_got_input_direction:;
cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    byte src = g_memByte[523142];
    byte res = src & al;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // test byte ptr playerTurnFlags, al
if (flags.zero)
goto l_throw_in_use_sprite_direction; // jz short @@throw_in_use_sprite_direction

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (flags.zero)
goto l_throw_in_check_quick_fire;   // jz short @@throw_in_check_quick_fire

ax = D0;                                // mov ax, word ptr D0
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
push(D0);                               // push D0
if (g_portedToggles.useCpp_SetThrowInPlayerDestinationCoordinates) PlayerFunctions::SetThrowInPlayerDestinationCoordinates(); else SetThrowInPlayerDestinationCoordinates(); // call SetThrowInPlayerDestinationCoordinates
pop(D0);                                // pop D0
A0 = 453788;                            // mov A0, offset aboutToThrowInAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 5);            // mov [esi+Sprite.playerState], PL_THROW_IN
writeMemory(esi + 13, 1, 0);            // mov [esi+Sprite.playerDownTimer], 0

l_throw_in_check_quick_fire:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 48, 1);     // mov al, [esi+TeamGeneralInfo.quickFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_throw_in_check_normal_fire;  // jz short @@throw_in_check_normal_fire

cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (flags.zero)
goto l_throw_in_check_normal_fire;  // jz short @@throw_in_check_normal_fire

g_memWord[261576] = 1;                  // mov throwInPassOrKick, 1
A0 = 453918;                            // mov A0, offset throwInPassAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 13, 1, 20);           // mov [esi+Sprite.playerDownTimer], 20
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_throw_in_check_normal_fire:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 49, 1);     // mov al, [esi+TeamGeneralInfo.normalFire]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

cl = D0;                                // mov cl, byte ptr D0
ax = 1;                                 // mov ax, 1
{
    byte shiftCount = cl & 0x1f;
    if (shiftCount) {
        word res = ax << shiftCount;
        ax = res;
    }
}                                       // shl ax, cl
{
    word src = g_memWord[261571];
    word res = src & ax;
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // test playerTurnFlags, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

g_memWord[261576] = 0;                  // mov throwInPassOrKick, 0
A0 = 454048;                            // mov A0, offset throwInKickAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 13, 1, 25);           // mov [esi+Sprite.playerDownTimer], 25
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_do_throw_in_pass:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1
push(D0);                               // push D0
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_DoPass) PlayerFunctions::DoPass(); else DoPass(); // call DoPass
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
pop(D0);                                // pop D0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
esi = A2;                               // mov esi, A2
writeMemory(esi + 54, 4, 1);            // mov [esi+Sprite.deltaZ], 1
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_throw_in_ball_passed;        // jz short @@throw_in_ball_passed

push(D0);                               // push D0
//ThrowInDeadProc();                      // call ThrowInDeadProc
pop(D0);                                // pop D0
g_memWord[261558] = 0;                  // mov gameNotInProgressCounterWriteOnly, 0

l_throw_in_ball_passed:;
g_memWord[261564] = 100;                // mov gameStatePl, ST_GAME_IN_PROGRESS
g_memWord[261565] = 100;                // mov gameState, ST_GAME_IN_PROGRESS
esi = A6;                               // mov esi, A6
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
esi = A6;                               // mov esi, A6
writeMemory(esi + 32, 4, 0);            // mov [esi+TeamGeneralInfo.controlledPlayerSprite], 0
eax = A1;                               // mov eax, A1
writeMemory(esi + 104, 4, eax);         // mov [esi+TeamGeneralInfo.passingKickingPlayer], eax
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_do_throw_in_kick:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1
push(D0);                               // push D0
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
ComparePlayerKickingBall(g_portedToggles.useCpp_PlayerKickingBall); // call PlayerKickingBall with comparison
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
pop(D0);                                // pop D0
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, 100
if (flags.zero)
goto l_throw_in_ball_kicked;        // jz short @@throw_in_ball_kicked

g_memWord[261558] = 0;                  // mov gameNotInProgressCounterWriteOnly, 0

l_throw_in_ball_kicked:;
g_memWord[261564] = 100;                // mov gameStatePl, ST_GAME_IN_PROGRESS
g_memWord[261565] = 100;                // mov gameState, ST_GAME_IN_PROGRESS
esi = A6;                               // mov esi, A6
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 94, 2, 1);            // mov [esi+TeamGeneralInfo.ballInPlay], 1
writeMemory(esi + 96, 2, 1);            // mov [esi+TeamGeneralInfo.ballOutOfPlay], 1
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
esi = A6;                               // mov esi, A6
writeMemory(esi + 32, 4, 0);            // mov [esi+TeamGeneralInfo.controlledPlayerSprite], 0
eax = A1;                               // mov eax, A1
writeMemory(esi + 104, 4, eax);         // mov [esi+TeamGeneralInfo.passingKickingPlayer], eax
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_abort_throw_in:;
g_memWord[224900] = 0;                  // mov hideBall, 0

l_throw_in_over:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_tackling:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 106, 2);    // mov ax, [esi+Sprite.tacklingTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_bump_player_down_timer;      // js short @@bump_player_down_timer

{
    word src = (word)readMemory(esi + 106, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    src = res;
    writeMemory(esi + 106, 2, src);
}                                       // add [esi+Sprite.tacklingTimer], 1

l_bump_player_down_timer:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_player_down_tackling;        // jnz short @@player_down_tackling

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_player_down_tackling:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 106, 2);    // mov ax, [esi+Sprite.tacklingTimer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_computer_tackling;           // js short @@computer_tackling

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_computer_tackling;           // jz short @@computer_tackling

al = (byte)readMemory(esi + 50, 1);     // mov al, [esi+TeamGeneralInfo.firePressed]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_computer_tackling;           // jnz short @@computer_tackling

esi = A1;                               // mov esi, A1
{
    int16_t src = (int16_t)readMemory(esi + 106, 2);
    src = -src;
    writeMemory(esi + 106, 2, src);
}                                       // neg [esi+Sprite.tacklingTimer]
{
    word src = (word)readMemory(esi + 106, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = -2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.tacklingTimer], -2
if (flags.sign != flags.overflow)
goto l_computer_tackling;           // jl short @@computer_tackling

writeMemory(esi + 106, 2, -1);          // mov [esi+Sprite.tacklingTimer], -1

l_computer_tackling:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

ax = g_memWord[162816];                 // mov ax, kPlayerGroundConstant
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
if (!flags.zero && flags.sign == flags.overflow)
goto l_player_still_tackling_and_moving; // jg short @@player_still_tackling_and_moving

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
if (g_portedToggles.useCpp_SetPlayerDowntimeAfterTackle) PlayerFunctions::SetPlayerDowntimeAfterTackle(); else SetPlayerDowntimeAfterTackle(); // call SetPlayerDowntimeAfterTackle
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_still_tackling_and_moving:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 73;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 73
if (flags.sign != flags.overflow)
goto l_pl_tackling_out_of_pitch;    // jl @@pl_tackling_out_of_pitch

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 598;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 598
if (!flags.zero && flags.sign == flags.overflow)
goto l_pl_tackling_out_of_pitch;    // jg @@pl_tackling_out_of_pitch

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 129
if (flags.sign != flags.overflow)
goto l_pl_tackling_out_of_pitch_y;  // jl short @@pl_tackling_out_of_pitch_y

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 769;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 769
if (flags.zero || flags.sign != flags.overflow)
goto l_pl_tackling_in_pitch;        // jle @@pl_tackling_in_pitch

l_pl_tackling_out_of_pitch_y:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 275;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 275
if (flags.sign != flags.overflow)
goto l_pl_tackling_out_of_goalie_area; // jl short @@pl_tackling_out_of_goalie_area

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 396;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 396
if (!flags.zero && flags.sign == flags.overflow)
goto l_pl_tackling_out_of_goalie_area; // jg short @@pl_tackling_out_of_goalie_area

{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // shr [esi+Sprite.speed], 1
{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // shr [esi+Sprite.speed], 1
{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // shr [esi+Sprite.speed], 1
{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // shr [esi+Sprite.speed], 1
{
    word src = (word)readMemory(esi + 44, 2);
    src |= 1;
    writeMemory(esi + 44, 2, src);
    flags.carry = false;
    flags.overflow = false;
    flags.sign = (src & 0x8000) != 0;
    flags.zero = src == 0;
}                                       // or [esi+Sprite.speed], 1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_pl_tackling_out_of_goalie_area:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 121;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 121
if (flags.sign != flags.overflow)
goto l_pl_tackling_out_of_pitch;    // jl short @@pl_tackling_out_of_pitch

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 777;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 777
if (flags.zero || flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jle @@update_player_speed_and_deltas

l_pl_tackling_out_of_pitch:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 3;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 3
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_pl_tackling_in_pitch:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_tackling_empty_space;        // jnz @@tackling_empty_space

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 96, 2);     // mov ax, [esi+Sprite.tackleState]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_tackling_empty_space;        // jnz @@tackling_empty_space

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_tackling_near_the_ball;      // jnz short @@tackling_near_the_ball

{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 64;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 64
if (!flags.carry && !flags.zero)
goto l_tackling_empty_space;        // ja @@tackling_empty_space

l_tackling_near_the_ball:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_player_tackling_the_ball;    // jnz short @@player_tackling_the_ball

al = (byte)readMemory(esi + 65, 1);     // mov al, [esi+TeamGeneralInfo.ball4To8]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto l_tackling_empty_space;        // jz @@tackling_empty_space

l_player_tackling_the_ball:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
esi = A6;                               // mov esi, A6
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 40, 2);     // mov ax, [esi+TeamGeneralInfo.playerHasBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_if_strong_tackle;      // jz short @@check_if_strong_tackle

g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1

l_check_if_strong_tackle:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 106, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = -1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.tacklingTimer], -1
if (!flags.zero)
goto l_strong_tackle;               // jnz short @@strong_tackle

push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerTackledTheBallWeak) PlayerFunctions::PlayerTackledTheBallWeak(); else PlayerTackledTheBallWeak(); // call PlayerTackledTheBallWeak
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
goto cseg_821F5;                        // jmp short cseg_821F5

l_strong_tackle:;
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayersTackledTheBallStrong) PlayerFunctions::PlayersTackledTheBallStrong(); else PlayersTackledTheBallStrong(); // call PlayersTackledTheBallStrong
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0

cseg_821F5:;
eax = A1;                               // mov eax, A1
esi = A6;                               // mov esi, A6
writeMemory(esi + 72, 4, eax);          // mov [esi+TeamGeneralInfo.lastHeadingTacklingPlayer], eax
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
writeMemory(esi + 118, 2, -1);          // mov [esi+TeamGeneralInfo.spinTimer], -1
writeMemory(esi + 138, 2, 12);          // mov [esi+TeamGeneralInfo.wonTheBallTimer], 12
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_tackling_empty_space:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 512;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.speed], 512
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerTacklingTestFoul) PlayerFunctions::PlayerTacklingTestFoul(); else PlayerTacklingTestFoul(); // call PlayerTacklingTestFoul
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_doing_static_header:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_player_down_with_static_header; // jnz short @@player_down_with_static_header

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_player_down_with_static_header:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 16;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], 16
if (!flags.sign)
goto cseg_822E4;                    // jns short cseg_822E4

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0

cseg_822E4:;
if (g_portedToggles.useCpp_SetStaticHeaderDirection) PlayerFunctions::SetStaticHeaderDirection(); else SetStaticHeaderDirection(); // call SetStaticHeaderDirection
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 98, 2);     // mov ax, [esi+Sprite.heading]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 64;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 64
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 8;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 8
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 15;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 15
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_update_player_speed_and_deltas; // js @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerHittingStaticHeader) PlayerFunctions::PlayerHittingStaticHeader(); else PlayerHittingStaticHeader(); // call PlayerHittingStaticHeader
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_jump_heading:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_still_jump_heading;          // jnz short @@still_jump_heading

l_set_player_to_normal:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_still_jump_heading:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_if_jump_header_inside_pitch; // jz short @@check_if_jump_header_inside_pitch

if (g_portedToggles.useCpp_SetJumpHeaderHitAnimTable) PlayerFunctions::SetJumpHeaderHitAnimTable(); else SetJumpHeaderHitAnimTable(); // call SetJumpHeaderHitAnimTable
esi = A1;                               // mov esi, A1
{
    dword src = readMemory(esi + 6, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 452584;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.animationTable], offset jumpHeaderAttemptAnimTable
if (flags.zero)
goto cseg_8245D;                    // jz short cseg_8245D

{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 17;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 17
if (!flags.carry && !flags.zero)
goto l_update_heading_speed;        // ja short @@update_heading_speed

{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    flags.carry = ((word)src >> 15) & 1;
    flags.overflow = (((src >> 15) & 1)) != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // shr [esi+Sprite.speed], 1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_8245D:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 37;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 37
if (!flags.carry && !flags.zero)
goto l_update_heading_speed;        // ja short @@update_heading_speed

{
    word src = (word)readMemory(esi + 44, 2);
    word res = src >> 1;
    flags.carry = ((word)src >> 15) & 1;
    flags.overflow = (((src >> 15) & 1)) != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // shr [esi+Sprite.speed], 1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_update_heading_speed:;
ax = g_memWord[162817];                 // mov ax, kPlayerAirConstant
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
if (!flags.sign)
goto l_check_if_jump_header_inside_pitch; // jns short @@check_if_jump_header_inside_pitch

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_check_if_jump_header_inside_pitch:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 73;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 73
if (flags.sign != flags.overflow)
goto l_slow_down_heading_player_and_leave; // jl @@slow_down_heading_player_and_leave

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 598;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 598
if (!flags.zero && flags.sign == flags.overflow)
goto l_slow_down_heading_player_and_leave; // jg @@slow_down_heading_player_and_leave

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 132;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 132
if (flags.sign != flags.overflow)
goto l_heading_close_to_goal_lines; // jl short @@heading_close_to_goal_lines

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 766;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 766
if (flags.zero || flags.sign != flags.overflow)
goto l_check_if_header_winded_up;   // jle @@check_if_header_winded_up

l_heading_close_to_goal_lines:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 296;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 296
if (flags.sign != flags.overflow)
goto cseg_8253D;                    // jl short cseg_8253D

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 375;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 375
if (!flags.zero && flags.sign == flags.overflow)
goto cseg_8253D;                    // jg short cseg_8253D

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 305;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 305
if (flags.sign != flags.overflow)
goto l_heading_finished;            // jl short @@heading_finished

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 366;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 366
if (flags.carry || flags.zero)
goto cseg_8253D;                    // jbe short cseg_8253D

l_heading_finished:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
writeMemory(esi + 13, 1, 0);            // mov [esi+Sprite.playerDownTimer], 0
goto l_set_player_to_normal;            // jmp @@set_player_to_normal

cseg_8253D:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 129
if (flags.sign != flags.overflow)
goto l_heading_into_the_goal;       // jl short @@heading_into_the_goal

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 769;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 769
if (flags.zero || flags.sign != flags.overflow)
goto l_check_if_header_winded_up;   // jle short @@check_if_header_winded_up

l_heading_into_the_goal:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 290;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 290
if (flags.sign != flags.overflow)
goto l_check_if_heading_inside_a_goal; // jl short @@check_if_heading_inside_a_goal

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 381;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 381
if (flags.zero || flags.sign != flags.overflow)
goto l_heading_finished;            // jle short @@heading_finished

l_check_if_heading_inside_a_goal:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 121;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 121
if (flags.sign != flags.overflow)
goto l_slow_down_heading_player_and_leave; // jl short @@slow_down_heading_player_and_leave

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 777;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 777
if (flags.zero || flags.sign != flags.overflow)
goto l_check_if_header_winded_up;   // jle short @@check_if_header_winded_up

l_slow_down_heading_player_and_leave:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 2;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 2
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
{
    word res = *(word *)&D0 >> 1;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 1
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_check_if_header_winded_up:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 42;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerDownTimer], 42
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

ax = (word)readMemory(esi + 98, 2);     // mov ax, [esi+Sprite.heading]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 64;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 64
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

esi = A2;                               // mov esi, A2
{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 8;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 8
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

{
    word src = (word)readMemory(esi + 40, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 15;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.z+2)], 15
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 1 & 0xff;
g_memByte[523098] = 1 >> 8;             // mov playerHadBall, 1
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerHittingJumpHeader) PlayerFunctions::PlayerHittingJumpHeader(); else PlayerHittingJumpHeader(); // call PlayerHittingJumpHeader
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 25);          // mov [esi+TeamGeneralInfo.passKickTimer], 25
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_injured:;
ax = g_memWord[261817];                 // mov ax, injuriesForever
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_player_tackled:;
esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 13, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 1;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 13, 1, src);
}                                       // sub [esi+Sprite.playerDownTimer], 1
if (!flags.zero)
goto l_player_on_the_ground;        // jnz short @@player_on_the_ground

ax = (word)readMemory(esi + 104, 2);    // mov ax, [esi+Sprite.injuryLevel]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_get_up_normally;             // jz short @@get_up_normally

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
writeMemory(esi + 12, 1, 13);           // mov [esi+Sprite.playerState], PL_ROLLING_INJURED
writeMemory(esi + 13, 1, al);           // mov [esi+Sprite.playerDownTimer], al
A0 = 455608;                            // mov A0, offset plInjuredAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_get_up_normally:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 0);            // mov [esi+Sprite.playerState], PL_NORMAL
A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_stop_player;                     // jmp @@stop_player

l_player_on_the_ground:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 159;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 159
if (flags.sign != flags.overflow)
goto l_in_goalkeepers_area_by_y;    // jl short @@in_goalkeepers_area_by_y

{
    word src = (word)readMemory(esi + 36, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 739;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.y+2)], 739
if (flags.zero || flags.sign != flags.overflow)
goto l_player_not_in_goalkeepers_area; // jle short @@player_not_in_goalkeepers_area

l_in_goalkeepers_area_by_y:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 265;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 265
if (flags.sign != flags.overflow)
goto l_player_not_in_goalkeepers_area; // jl short @@player_not_in_goalkeepers_area

{
    word src = (word)readMemory(esi + 32, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 406;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+(Sprite.x+2)], 406
if (!flags.zero && flags.sign == flags.overflow)
goto l_player_not_in_goalkeepers_area; // jg short @@player_not_in_goalkeepers_area

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    word res = *(word *)&D0 >> 2;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 2
ax = D0;                                // mov ax, word ptr D0
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax

l_player_not_in_goalkeepers_area:;
ax = g_memWord[162816];                 // mov ax, kPlayerGroundConstant
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 44, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 44, 2, src);
}                                       // sub [esi+Sprite.speed], ax
if (!flags.sign)
goto l_update_player_speed_and_deltas; // jns @@update_player_speed_and_deltas

writeMemory(esi + 44, 2, 0);            // mov [esi+Sprite.speed], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_expecting_pass:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 81;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 81
if (flags.sign != flags.overflow)
goto l_player_passed_to_out_of_pitch; // jl short @@player_passed_to_out_of_pitch

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 590;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 590
if (!flags.zero && flags.sign == flags.overflow)
goto l_player_passed_to_out_of_pitch; // jg short @@player_passed_to_out_of_pitch

ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 129;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 129
if (flags.sign != flags.overflow)
goto l_player_passed_to_out_of_pitch; // jl short @@player_passed_to_out_of_pitch

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 769;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 769
if (flags.zero || flags.sign != flags.overflow)
goto l_passed_to_player_inside_pitch; // jle short @@passed_to_player_inside_pitch

l_player_passed_to_out_of_pitch:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
goto l_stop_player;                     // jmp @@stop_player

l_passed_to_player_inside_pitch:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_cpu_passing_to;              // jz @@cpu_passing_to

eax = readMemory(esi + 124, 4);         // mov eax, dword ptr [esi+TeamGeneralInfo.longPass]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto l_test_for_long_pass;          // jnz short @@test_for_long_pass

ax = (word)readMemory(esi + 120, 2);    // mov ax, [esi+TeamGeneralInfo.leftSpin]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_test_for_long_pass;          // jnz short @@test_for_long_pass

ax = (word)readMemory(esi + 122, 2);    // mov ax, [esi+TeamGeneralInfo.rightSpin]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_cpu_passing_to;              // jz @@cpu_passing_to

l_test_for_long_pass:;
{
    word src = g_memWord[164516];
    int16_t dstSigned = src;
    int16_t srcSigned = 512;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballSprite.speed, 512
if (flags.carry)
goto l_cpu_passing_to;              // jb @@cpu_passing_to

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 90, 2);     // mov ax, [esi+TeamGeneralInfo.passingToPlayer]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_cpu_passing_to;              // jz @@cpu_passing_to

ax = g_memWord[164515];                 // mov ax, ballSprite.direction
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_ball_got_no_direction;       // js short @@ball_got_no_direction

ax = g_memWord[164535];                 // mov ax, ballSprite.fullDirection
*(word *)&D0 = ax;                      // mov word ptr D0, ax
esi = A1;                               // mov esi, A1
al = (byte)readMemory(esi + 82, 1);     // mov al, byte ptr [esi+Sprite.fullDirection]
{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = al;
    byte res = dstSigned - srcSigned;
    *(byte *)&D0 = res;
}                                       // sub byte ptr D0, al
{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = 64;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D0, 64
if (!flags.zero && flags.sign == flags.overflow)
goto l_player_not_turned_enough_toward_ball; // jg short @@player_not_turned_enough_toward_ball

{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = 192;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D0, 192
if (flags.sign == flags.overflow)
goto cseg_82952;                    // jge short cseg_82952

l_player_not_turned_enough_toward_ball:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
goto l_cpu_passing_to;                  // jmp @@cpu_passing_to

cseg_82952:;
{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = 4;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D0, 4
if (!flags.zero && flags.sign == flags.overflow)
goto cseg_82991;                    // jg short cseg_82991

{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = 252;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D0, 252
if (flags.sign != flags.overflow)
goto cseg_82991;                    // jl short cseg_82991

l_ball_got_no_direction:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
esi = A6;                               // mov esi, A6
writeMemory(esi + 98, 2, ax);           // mov [esi+TeamGeneralInfo.ballX], ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
esi = A6;                               // mov esi, A6
writeMemory(esi + 100, 2, ax);          // mov [esi+TeamGeneralInfo.ballY], ax
goto l_cpu_passing_to;                  // jmp @@cpu_passing_to

cseg_82991:;
*(byte *)&D1 = 192;                     // mov byte ptr D1, 192
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 124, 4);         // mov eax, dword ptr [esi+TeamGeneralInfo.longPass]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (flags.zero)
goto cseg_829AC;                    // jz short cseg_829AC

*(byte *)&D1 = 224;                     // mov byte ptr D1, 224

cseg_829AC:;
al = D0;                                // mov al, byte ptr D0
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.sign != flags.overflow)
goto cseg_829BB;                    // jl short cseg_829BB

*(int8_t *)&D1 = -*(int8_t *)&D1;       // neg byte ptr D1

cseg_829BB:;
ax = g_memWord[164535];                 // mov ax, ballSprite.fullDirection
*(word *)&D0 = ax;                      // mov word ptr D0, ax
al = D1;                                // mov al, byte ptr D1
{
    int8_t dstSigned = *(byte *)&D0;
    int8_t srcSigned = al;
    byte res = dstSigned + srcSigned;
    *(byte *)&D0 = res;
}                                       // add byte ptr D0, al
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 4;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 4
{
    word res = *(word *)&D0 & 255;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0FFh
A0 = 523178;                            // mov A0, offset kBallFriction
{
    word res = *(word *)&D0 & 255;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0FFh
{
    word res = *(word *)&D0 >> 3;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 3
{
    word res = *(word *)&D0 << 2;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 2
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
esi = A0;                               // mov esi, A0
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
ax = D1;                                // mov ax, word ptr D1
esi = A6;                               // mov esi, A6
writeMemory(esi + 98, 2, ax);           // mov [esi+TeamGeneralInfo.ballX], ax
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + ebx + 2, 2); // mov ax, [esi+ebx+2]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
ax = D1;                                // mov ax, word ptr D1
esi = A6;                               // mov esi, A6
writeMemory(esi + 100, 2, ax);          // mov [esi+TeamGeneralInfo.ballY], ax

l_cpu_passing_to:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_82AAF;                    // jnz short cseg_82AAF

push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_AI_Kick) PlayerFunctions::AI_Kick(); else AI_Kick(); // call AI_Kick
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1

cseg_82AAF:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto cseg_82C41;                    // jz cseg_82C41

eax = g_memDword[130780];               // mov eax, lastTeamPlayedBeforeBreak
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 3;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_KEEPER_HOLDS_BALL
if (!flags.zero)
goto cseg_82AE5;                    // jnz short cseg_82AE5

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto cseg_82AF6;                    // jz short cseg_82AF6

cseg_82AE5:;
esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto cseg_82EC2;                    // jnz cseg_82EC2

cseg_82AF6:;
{
    word src = g_memWord[261570];
    int16_t dstSigned = src;
    int16_t srcSigned = 8;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp cameraDirection, 8
if (flags.carry)
goto l_pass_success;                // jb short @@pass_success

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
g_memWord[261570] = ax;                 // mov cameraDirection, ax
{
    word src = g_memWord[227969];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    g_memWord[227969] = src;
}                                       // add dseg_132804, 1

l_pass_success:;
ax = g_memWord[261570];                 // mov ax, cameraDirection
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
writeMemory(esi + 32, 4, eax);          // mov [esi+TeamGeneralInfo.controlledPlayerSprite], eax
writeMemory(esi + 36, 4, 0);            // mov [esi+TeamGeneralInfo.passToPlayerPtr], 0
writeMemory(esi + 92, 2, 25);           // mov [esi+TeamGeneralInfo.playerSwitchTimer], 25
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 58, 2, 0);            // mov [esi+TeamGeneralInfo.shooting], 0
esi = A2;                               // mov esi, A2
writeMemory(esi + 44, 2, 0);            // mov [esi+TeamGeneralInfo.currentAllowedDirection], 0
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
if (g_portedToggles.useCpp_UpdatePlayerWithBall) PlayerFunctions::UpdatePlayerWithBall(); else UpdatePlayerWithBall(); // call UpdatePlayerWithBall
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 0);           // mov [esi+TeamGeneralInfo.passKickTimer], 0
writeMemory(esi + 104, 4, 0);           // mov dword ptr [esi+104], 0
writeMemory(esi + 116, 2, 0);           // mov word ptr [esi+116], 0
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 15;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_THROW_IN_FORWARD_RIGHT
if (flags.carry)
goto l_update_player_speed_and_deltas; // jb @@update_player_speed_and_deltas

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 20;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_THROW_IN_BACK_LEFT
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
push(D0);                               // push D0
if (g_portedToggles.useCpp_SetThrowInPlayerDestinationCoordinates) PlayerFunctions::SetThrowInPlayerDestinationCoordinates(); else SetThrowInPlayerDestinationCoordinates(); // call SetThrowInPlayerDestinationCoordinates
pop(D0);                                // pop D0
A0 = 453788;                            // mov A0, offset aboutToThrowInAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 5);            // mov [esi+Sprite.playerState], PL_THROW_IN
writeMemory(esi + 13, 1, 0);            // mov [esi+Sprite.playerDownTimer], 0
g_memWord[224900] = 1;                  // mov hideBall, 1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_82C41:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto cseg_82D59;                    // jz cseg_82D59

esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 51, 1);     // mov al, [esi+TeamGeneralInfo.fireThisFrame]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_82D59;                    // jz cseg_82D59

ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+TeamGeneralInfo.currentAllowedDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto cseg_82D59;                    // js cseg_82D59

al = (byte)readMemory(esi + 63, 1);     // mov al, [esi+TeamGeneralInfo.plNotFarFromBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_82D59;                    // jz cseg_82D59

al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto cseg_82CAB;                    // jnz short cseg_82CAB

al = (byte)readMemory(esi + 65, 1);     // mov al, [esi+TeamGeneralInfo.ball4To8]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_82D59;                    // jz cseg_82D59

cseg_82CAB:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
A5 = eax;                               // mov A5, eax
{
    int32_t dstSigned = A5;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A5, 0
if (flags.zero)
goto cseg_82CE1;                    // jz short cseg_82CE1

esi = A5;                               // mov esi, A5
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
D1 = eax;                               // mov D1, eax
esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D1, eax
if (flags.carry)
goto cseg_82D59;                    // jb short cseg_82D59

cseg_82CE1:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 52, 2, 1);            // mov [esi+TeamGeneralInfo.headerOrTackle], 1
push(A0);                               // push A0
push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
if (g_portedToggles.useCpp_PlayerBeginTackling) PlayerFunctions::PlayerBeginTackling(); else PlayerBeginTackling(); // call PlayerBeginTackling
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1
pop(A0);                                // pop A0
esi = A6;                               // mov esi, A6
writeMemory(esi + 110, 2, 0);           // mov [esi+TeamGeneralInfo.ballCanBeControlled], 0
eax = readMemory(esi, 4);               // mov eax, [esi+TeamGeneralInfo.opponentsTeam]
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_82D59:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 64, 1);     // mov al, [esi+TeamGeneralInfo.ballLessEqual4]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_82EC2;                    // jz cseg_82EC2

{
    word src = g_memWord[164516];
    int16_t dstSigned = src;
    int16_t srcSigned = 1536;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp ballSprite.speed, 1536
if (flags.carry)
goto cseg_82D82;                    // jb short cseg_82D82

al = (byte)readMemory(esi + 62, 1);     // mov al, [esi+TeamGeneralInfo.plCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_passed_to_player_becomes_main; // jnz short @@passed_to_player_becomes_main

cseg_82D82:;
esi = A6;                               // mov esi, A6
al = (byte)readMemory(esi + 61, 1);     // mov al, [esi+TeamGeneralInfo.plVeryCloseToBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (flags.zero)
goto cseg_82EC2;                    // jz cseg_82EC2

l_passed_to_player_becomes_main:;
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 36, 4);          // mov eax, [esi+TeamGeneralInfo.passToPlayerPtr]
writeMemory(esi + 32, 4, eax);          // mov [esi+TeamGeneralInfo.controlledPlayerSprite], eax
writeMemory(esi + 36, 4, 0);            // mov [esi+TeamGeneralInfo.passToPlayerPtr], 0
writeMemory(esi + 92, 2, 25);           // mov [esi+TeamGeneralInfo.playerSwitchTimer], 25
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
writeMemory(esi + 58, 2, 0);            // mov [esi+TeamGeneralInfo.shooting], 0
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+TeamGeneralInfo.controlledPlayerSprite]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (!flags.zero)
goto l_update_controlling_player;   // jnz short @@update_controlling_player

if (g_portedToggles.useCpp_UpdateBallWithControllingGoalkeeper) PlayerFunctions::UpdateBallWithControllingGoalkeeper(); else UpdateBallWithControllingGoalkeeper(); // call UpdateBallWithControllingGoalkeeper
goto cseg_82E23;                        // jmp short cseg_82E23

l_update_controlling_player:;
if (g_portedToggles.useCpp_UpdateControllingPlayer) PlayerFunctions::UpdateControllingPlayer(); else UpdateControllingPlayer(); // call UpdateControllingPlayer

cseg_82E23:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 102, 2, 0);           // mov [esi+TeamGeneralInfo.passKickTimer], 0
writeMemory(esi + 104, 4, 0);           // mov [esi+TeamGeneralInfo.passingKickingPlayer], 0
writeMemory(esi + 116, 2, 0);           // mov [esi+TeamGeneralInfo.field_74], 0
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (!flags.zero)
goto cseg_82E97;                    // jnz short cseg_82E97

eax = g_memDword[130776];               // mov eax, lastTeamPlayed
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto cseg_82E92;                    // jnz short cseg_82E92

ax = g_memByte[523097] | (g_memByte[523098] << 8); // mov ax, playerHadBall
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_82E92;                    // jnz short cseg_82E92

esi = A6;                               // mov esi, A6
writeMemory(esi + 140, 2, 1);           // mov [esi+TeamGeneralInfo.goalkeeperPlaying], 1
writeMemory(esi + 86, 1, 1);            // mov byte ptr [esi+TeamGeneralInfo.goaliePlayingOrOut], 1
writeMemory(esi + 84, 1, 1);            // mov byte ptr [esi+TeamGeneralInfo.ballOutOfPlayOrKeeper], 1
goto cseg_82E97;                        // jmp short cseg_82E97

cseg_82E92:;
GoalkeeperClaimedTheBall();             // call GoalkeeperClaimedTheBall

cseg_82E97:;
eax = A6;                               // mov eax, A6
g_memDword[130776] = eax;               // mov lastTeamPlayed, eax
eax = A1;                               // mov eax, A1
g_memDword[130777] = eax;               // mov lastPlayerPlayed, eax
g_memWord[261556] = 0;                  // mov penalty, 0
g_memByte[523097] = 0;
g_memByte[523098] = 0;                  // mov playerHadBall, 0
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_82EC2:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 88, 2);     // mov ax, [esi+TeamGeneralInfo.passingBall]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_player_chase_ball;           // jnz @@player_chase_ball

eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
A5 = eax;                               // mov A5, eax
{
    int32_t dstSigned = A5;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A5, 0
if (flags.zero)
goto l_its_a_pass;                  // jz short @@its_a_pass

esi = A5;                               // mov esi, A5
{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 3200;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 3200
if (!flags.carry && !flags.zero)
goto l_its_a_pass;                  // ja short @@its_a_pass

eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
D0 = eax;                               // mov D0, eax
esi = A1;                               // mov esi, A1
eax = readMemory(esi + 74, 4);          // mov eax, [esi+Sprite.ballDistance]
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (!flags.carry && !flags.zero)
goto l_its_a_pass;                  // ja short @@its_a_pass

{
    dword src = readMemory(esi + 74, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 3200;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.ballDistance], 3200
if (!flags.carry && !flags.zero)
goto l_player_chase_ball;           // ja short @@player_chase_ball

ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_its_a_pass:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 88, 2, 1);            // mov [esi+TeamGeneralInfo.passingBall], 1

l_player_chase_ball:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 98, 2);     // mov ax, [esi+TeamGeneralInfo.ballX]
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 100, 2);    // mov ax, [esi+TeamGeneralInfo.ballY]
esi = A1;                               // mov esi, A1
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+Sprite.destX]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (!flags.zero)
goto l_player_still_moving;         // jnz short @@player_still_moving

ax = (word)readMemory(esi + 60, 2);     // mov ax, [esi+Sprite.destY]
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

l_player_still_moving:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto cseg_8308D;                    // jnz cseg_8308D

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (!flags.zero)
goto cseg_8308D;                    // jnz cseg_8308D

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 140, 2);    // mov ax, [esi+TeamGeneralInfo.goalkeeperPlaying]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto cseg_8308D;                    // jnz cseg_8308D

{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (flags.zero)
goto cseg_8303E;                    // jz short cseg_8303E

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 206;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 206
if (!flags.zero && flags.sign == flags.overflow)
goto l_cancel_pass;                 // jg short @@cancel_pass

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 203;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 203
if (flags.sign != flags.overflow)
goto l_cancel_pass;                 // jl short @@cancel_pass

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 468;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 468
if (!flags.zero && flags.sign == flags.overflow)
goto l_cancel_pass;                 // jg short @@cancel_pass

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_8303E:;
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 692;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 692
if (flags.sign != flags.overflow)
goto l_cancel_pass;                 // jl short @@cancel_pass

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 203;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 203
if (flags.sign != flags.overflow)
goto l_cancel_pass;                 // jl short @@cancel_pass

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 468;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 468
if (flags.zero || flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jle @@update_player_speed_and_deltas

l_cancel_pass:;
esi = A6;                               // mov esi, A6
writeMemory(esi + 36, 4, 0);            // mov [esi+TeamGeneralInfo.passToPlayerPtr], 0
writeMemory(esi + 88, 2, 0);            // mov [esi+TeamGeneralInfo.passingBall], 0
writeMemory(esi + 90, 2, 0);            // mov [esi+TeamGeneralInfo.passingToPlayer], 0
goto l_stop_player;                     // jmp @@stop_player

cseg_8308D:;
ax = g_memWord[261557];                 // mov ax, goalOut
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 183;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 183
if (flags.sign != flags.overflow)
goto l_update_player_speed_and_deltas; // jl @@update_player_speed_and_deltas

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 488;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 488
if (!flags.zero && flags.sign == flags.overflow)
goto l_update_player_speed_and_deltas; // jg @@update_player_speed_and_deltas

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 226;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 226
if (flags.zero || flags.sign != flags.overflow)
goto l_stop_player;                 // jle @@stop_player

{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 672;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, 672
if (flags.sign == flags.overflow)
goto l_stop_player;                 // jge @@stop_player

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_injury_forever:;
ax = g_memWord[162968];                 // mov ax, dseg_110D8D
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
{
    word src = (word)readMemory(esi + 60, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 8;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    src = res;
    writeMemory(esi + 60, 2, src);
}                                       // sub [esi+Sprite.destY], 8
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_not_controlled_player:;
ax = g_memWord[243084];                 // mov ax, g_substituteInProgress
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_test_update_player_index;    // jz short @@test_update_player_index

eax = g_memDword[121543];               // mov eax, substitutedPlSprite
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_this_is_substituted_player;  // jz @@this_is_substituted_player

l_test_update_player_index:;
esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 30, 2);     // mov ax, [esi+TeamGeneralInfo.updatePlayerIndex]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = (word)stack[stackTop];             // mov ax, [esp]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.zero)
goto l_next_player;                 // jnz @@next_player

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_this_is_substituted_player;  // jz @@this_is_substituted_player

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 30;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_GAME_ENDED
if (!flags.zero)
goto l_this_is_substituted_player;  // jnz @@this_is_substituted_player

eax = g_memDword[84155];                // mov eax, winningTeamPtr
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (flags.zero)
goto l_this_is_substituted_player;  // jz @@this_is_substituted_player

esi = A1;                               // mov esi, A1
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 0;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_NORMAL
if (!flags.zero)
goto l_this_is_substituted_player;  // jnz @@this_is_substituted_player

{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_this_is_substituted_player;  // jz @@this_is_substituted_player

eax = readMemory(esi + 46, 4);          // mov eax, [esi+Sprite.deltaX]
D0 = eax;                               // mov D0, eax
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
D0 |= eax;
flags.carry = false;
flags.overflow = false;
flags.sign = (D0 & 0x80000000) != 0;
flags.zero = D0 == 0;                   // or D0, eax
if (!flags.zero)
goto l_this_is_substituted_player;  // jnz short @@this_is_substituted_player

SWOS::Rand();                           // call Rand
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 64;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 64
if (!flags.carry && !flags.zero)
goto l_this_is_substituted_player;  // ja short @@this_is_substituted_player

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 10, 4);          // mov eax, [esi+TeamGeneralInfo.inGameTeamPtr]
D0 = eax;                               // mov D0, eax
eax = g_memDword[84155];                // mov eax, winningTeamPtr
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, eax
if (flags.zero)
goto l_player_in_winning_team;      // jz short @@player_in_winning_team

A0 = 452844;                            // mov A0, offset playerLosingReactionAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 14);           // mov [esi+Sprite.playerState], PL_SAD
goto l_this_is_substituted_player;      // jmp short @@this_is_substituted_player

l_player_in_winning_team:;
A0 = 452974;                            // mov A0, offset playerWinningReactionAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 15);           // mov [esi+Sprite.playerState], PL_HAPPY

l_this_is_substituted_player:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_update_destination_reached_state; // jnz short @@update_destination_reached_state

{
    word src = g_memWord[261572];
    int16_t dstSigned = src;
    int16_t srcSigned = 25;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp inGameCounter, 25
if (!flags.carry && !flags.zero)
goto l_update_destination_reached_state; // ja short @@update_destination_reached_state

{
    word src = g_memWord[261573];
    int16_t dstSigned = src;
    int16_t srcSigned = 4;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp breakState, ST_CORNER_LEFT
if (flags.zero)
goto l_next_player;                 // jz @@next_player

{
    word src = g_memWord[261573];
    int16_t dstSigned = src;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp breakState, ST_CORNER_RIGHT
if (flags.zero)
goto l_next_player;                 // jz @@next_player

{
    word src = g_memWord[261573];
    int16_t dstSigned = src;
    int16_t srcSigned = 6;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp breakState, ST_FREE_KICK_LEFT1
if (flags.carry)
goto l_update_destination_reached_state; // jb short @@update_destination_reached_state

{
    word src = g_memWord[261573];
    int16_t dstSigned = src;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp breakState, ST_FREE_KICK_RIGHT3
if (flags.carry || flags.zero)
goto l_next_player;                 // jbe @@next_player

l_update_destination_reached_state:;
esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 100, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.destReachedState], 1
if (!flags.zero)
goto l_check_for_controlled_player; // jnz short @@check_for_controlled_player

writeMemory(esi + 100, 2, 2);           // mov [esi+Sprite.destReachedState], 2

l_check_for_controlled_player:;
esi = A6;                               // mov esi, A6
{
    dword src = readMemory(esi + 32, 4);
    int32_t dstSigned = src;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+TeamGeneralInfo.controlledPlayerSprite], 0
if (!flags.zero)
goto l_check_if_this_player_getting_booked; // jnz short @@check_if_this_player_getting_booked

ax = (word)readMemory(esi + 4, 2);      // mov ax, [esi+TeamGeneralInfo.playerNumber]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_check_if_this_player_getting_booked; // jnz short @@check_if_this_player_getting_booked

push(A1);                               // push A1
push(A2);                               // push A2
push(A3);                               // push A3
AI::CallWithComparison(g_portedToggles.useCpp_AI_SetControlsDirection);
pop(A3);                                // pop A3
pop(A2);                                // pop A2
pop(A1);                                // pop A1

l_check_if_this_player_getting_booked:;
eax = g_memDword[130913];               // mov eax, bookedPlayer
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (!flags.zero)
goto l_check_for_substituted_player; // jnz @@check_for_substituted_player

ax = g_memWord[261568];                 // mov ax, foulXCoordinate
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = g_memWord[261569];                 // mov ax, foulYCoordinate
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 21;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 21
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (!flags.zero)
goto l_player_not_by_foul_spot;     // jnz @@player_not_by_foul_spot

ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D2, ax
if (!flags.zero)
goto l_player_not_by_foul_spot;     // jnz short @@player_not_by_foul_spot

{
    word src = g_memWord[261816];
    int16_t dstSigned = src;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp refState, REF_WAITING_PLAYER
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

g_memWord[261816] = 3;                  // mov refState, REF_ABOUT_TO_GIVE_CARD
{
    word src = g_memWord[261825];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp whichCard, CARD_YELLOW
if (flags.zero)
goto l_player_getting_yellow_card;  // jz short @@player_getting_yellow_card

{
    word src = g_memWord[261825];
    int16_t dstSigned = src;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp whichCard, CARD_RED
if (flags.zero)
goto l_player_getting_red_card;     // jz short @@player_getting_red_card

A0 = 455478;                            // mov A0, offset plGetting2ndYellowCardAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_set_player_state_booked;         // jmp short @@set_player_state_booked

l_player_getting_red_card:;
A0 = 455348;                            // mov A0, offset plGettingRedCardAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_set_player_state_booked;         // jmp short @@set_player_state_booked

l_player_getting_yellow_card:;
A0 = 455218;                            // mov A0, offset plGettingYellowCardAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable

l_set_player_state_booked:;
esi = A1;                               // mov esi, A1
writeMemory(esi + 12, 1, 12);           // mov [esi+Sprite.playerState], PL_BOOKED
writeMemory(esi + 13, 1, 1);            // mov [esi+Sprite.playerDownTimer], 1
//nullsub_1();                            // call nullsub_1
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_player_not_by_foul_spot:;
ax = D1;                                // mov ax, word ptr D1
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = D2;                                // mov ax, word ptr D2
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_check_for_substituted_player:;
ax = g_memWord[243084];                 // mov ax, g_substituteInProgress
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_if_sent_away;          // jz @@check_if_sent_away

if (flags.sign)
goto l_set_player_going_in_speed;   // js @@set_player_going_in_speed

eax = g_memDword[121543];               // mov eax, substitutedPlSprite
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (!flags.zero)
goto l_check_if_sent_away;          // jnz @@check_if_sent_away

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 104, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = -2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.injuryLevel], -2
if (flags.zero)
goto l_new_player_about_to_go_in;   // jz short @@new_player_about_to_go_in

ax = g_memWord[261574];                 // mov ax, substitutedPlDestX
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.zero)
goto l_set_substituted_player_destination; // jnz @@set_substituted_player_destination

ax = g_memWord[261575];                 // mov ax, substitutedPlDestY
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.zero)
goto l_set_substituted_player_destination; // jnz @@set_substituted_player_destination

{
    word src = g_memWord[243084];
    int16_t dstSigned = src;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp g_substituteInProgress, 2
if (flags.zero)
goto l_new_player_about_to_go_in;   // jz short @@new_player_about_to_go_in

g_memWord[243084] = 2;                  // mov g_substituteInProgress, 2
ax = g_memWord[243080];                 // mov ax, plComingX
g_memWord[261574] = ax;                 // mov substitutedPlDestX, ax
ax = g_memWord[243081];                 // mov ax, plComingY
g_memWord[261575] = ax;                 // mov substitutedPlDestY, ax
goto l_set_substituted_player_destination; // jmp short @@set_substituted_player_destination

l_new_player_about_to_go_in:;
g_memWord[243084] = -1;                 // mov g_substituteInProgress, -1
ax = g_memWord[243082];                 // mov ax, plSubstitutedX
esi = A1;                               // mov esi, A1
writeMemory(esi + 32, 2, ax);           // mov word ptr [esi+(Sprite.x+2)], ax
ax = g_memWord[243083];                 // mov ax, plSubstitutedY
writeMemory(esi + 36, 2, ax);           // mov word ptr [esi+(Sprite.y+2)], ax
ax = g_memWord[162810];                 // mov ax, kSubstitutedPlayerSpeed
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
writeMemory(esi + 108, 2, 0);           // mov [esi+Sprite.sentAway], 0
goto l_check_if_sent_away;              // jmp short @@check_if_sent_away

goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_set_substituted_player_destination:;
ax = g_memWord[261574];                 // mov ax, substitutedPlDestX
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = g_memWord[261575];                 // mov ax, substitutedPlDestY
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
ax = g_memWord[162810];                 // mov ax, kSubstitutedPlayerSpeed
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_set_player_going_in_speed:;
eax = g_memDword[121543];               // mov eax, substitutedPlSprite
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (!flags.zero)
goto l_check_if_sent_away;          // jnz short @@check_if_sent_away

ax = g_memWord[162810];                 // mov ax, kSubstitutedPlayerSpeed
esi = A1;                               // mov esi, A1
writeMemory(esi + 44, 2, ax);           // mov [esi+Sprite.speed], ax
eax = readMemory(esi + 46, 4);          // mov eax, [esi+Sprite.deltaX]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto l_check_if_sent_away;          // jnz short @@check_if_sent_away

eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto l_check_if_sent_away;          // jnz short @@check_if_sent_away

g_memWord[243084] = 0;                  // mov g_substituteInProgress, 0

l_check_if_sent_away:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 108, 2);    // mov ax, [esi+Sprite.sentAway]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_update_player_speed_and_deltas; // jnz @@update_player_speed_and_deltas

ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_set_player_positions_if_game_break; // jz short @@set_player_positions_if_game_break

*(word *)&D0 = 31;                      // mov word ptr D0, ST_PENALTIES
goto l_get_foul_coordinates;            // jmp short @@get_foul_coordinates

l_set_player_positions_if_game_break:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_game_interrupted_get_ball_x_y; // jnz short @@game_interrupted_get_ball_x_y

esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D6 = ax;                      // mov word ptr D6, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D7 = ax;                      // mov word ptr D7, ax
goto l_set_player_with_no_ball_destination; // jmp @@set_player_with_no_ball_destination

l_game_interrupted_get_ball_x_y:;
ax = g_memWord[261565];                 // mov ax, gameState
*(word *)&D0 = ax;                      // mov word ptr D0, ax

l_get_foul_coordinates:;
ax = g_memWord[261568];                 // mov ax, foulXCoordinate
*(word *)&D6 = ax;                      // mov word ptr D6, ax
ax = g_memWord[261569];                 // mov ax, foulYCoordinate
*(word *)&D7 = ax;                      // mov word ptr D7, ax
{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 29;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_FIRST_HALF_ENDED
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 30;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_GAME_ENDED
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 3;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_KEEPER_HOLDS_BALL
if (flags.zero)
goto l_set_player_with_no_ball_destination; // jz @@set_player_with_no_ball_destination

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_GOAL_OUT_LEFT
if (flags.zero)
goto l_set_player_with_no_ball_destination; // jz @@set_player_with_no_ball_destination

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_GOAL_OUT_RIGHT
if (flags.zero)
goto l_set_player_with_no_ball_destination; // jz @@set_player_with_no_ball_destination

{
    word src = g_memWord[227985];
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp forceLeftTeam, 1
if (!flags.zero)
goto l_check_for_penalty_shootout;  // jnz short @@check_for_penalty_shootout

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 4;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_CORNER_LEFT
if (flags.zero)
goto l_corner;                      // jz short @@corner

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_CORNER_RIGHT
if (!flags.zero)
goto l_check_for_penalty_shootout;  // jnz short @@check_for_penalty_shootout

l_corner:;
{
    int16_t dstSigned = *(word *)&D7;
    int16_t srcSigned = 449;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D7, 449
if (flags.sign != flags.overflow)
goto l_top_break;                   // jl short @@top_break

l_check_for_penalty_shootout:;
ax = g_memWord[261580];                 // mov ax, playingPenalties
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.zero)
goto l_check_team_for_penalty_positions; // jz short @@check_team_for_penalty_positions

{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522952;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset bottomTeamData
if (!flags.zero)
goto l_top_break;                   // jnz short @@top_break

goto l_bottom_break;                    // jmp short @@bottom_break

l_check_team_for_penalty_positions:;
eax = g_memDword[130780];               // mov eax, lastTeamPlayedBeforeBreak
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (!flags.zero)
goto l_top_break;                   // jnz short @@top_break

l_bottom_break:;
A5 = 524792;                            // mov A5, offset bottomBallOutOfPlayPositions
goto cseg_8364D;                        // jmp short cseg_8364D

l_top_break:;
A5 = 524920;                            // mov A5, offset topBallOutOfPlayPositions

cseg_8364D:;
{
    word res = *(word *)&D0 << 2;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 2
esi = A5;                               // mov esi, A5
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
eax = readMemory(esi + ebx, 4);         // mov eax, [esi+ebx]
A5 = eax;                               // mov A5, eax
{
    int32_t dstSigned = A5;
    int32_t srcSigned = 0;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A5, 0
if (flags.zero)
goto l_set_player_with_no_ball_destination; // jz @@set_player_with_no_ball_destination

g_memWord[262395] = 0;                  // mov freeKickDestX, 0
eax = g_memDword[130780];               // mov eax, lastTeamPlayedBeforeBreak
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (flags.zero)
goto l_not_in_free_kick_state;      // jz @@not_in_free_kick_state

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 6;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_FREE_KICK_LEFT1
if (flags.carry)
goto l_not_in_free_kick_state;      // jb @@not_in_free_kick_state

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_FREE_KICK_RIGHT3
if (!flags.carry && !flags.zero)
goto l_not_in_free_kick_state;      // ja @@not_in_free_kick_state

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 6;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_FREE_KICK_LEFT1
if (flags.zero)
goto l_not_in_free_kick_state;      // jz @@not_in_free_kick_state

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 12;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_FREE_KICK_RIGHT3
if (flags.zero)
goto l_not_in_free_kick_state;      // jz @@not_in_free_kick_state

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 2;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 2
if (flags.carry)
goto l_not_in_free_kick_state;      // jb @@not_in_free_kick_state

{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 5
if (!flags.carry && !flags.zero)
goto l_not_in_free_kick_state;      // ja @@not_in_free_kick_state

*(word *)&D1 = 2;                       // mov word ptr D1, 2
*(word *)&D0 = 2;                       // mov word ptr D0, 2
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 20, 4);          // mov eax, [esi+TeamGeneralInfo.spritesTable]
A4 = eax;                               // mov A4, eax
{
    int32_t dstSigned = A4;
    int32_t srcSigned = 4;
    dword res = dstSigned + srcSigned;
    A4 = res;
}                                       // add A4, 4

l_pl_loop_start:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 2, 2);      // mov ax, [esi+Sprite.playerOrdinal]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, ax
if (flags.zero)
goto cseg_8375A;                    // jz short cseg_8375A

esi = A4;                               // mov esi, A4
eax = readMemory(esi, 4);               // mov eax, [esi]
{
    int32_t dstSigned = A4;
    int32_t srcSigned = 4;
    dword res = dstSigned + srcSigned;
    A4 = res;
}                                       // add A4, 4
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 102, 2);    // mov ax, [esi+Sprite.cards]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_player_got_red_card_get_next; // js short @@player_got_red_card_get_next

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 1

l_player_got_red_card_get_next:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 1
goto l_pl_loop_start;                   // jmp short @@pl_loop_start

cseg_8375A:;
ax = g_memWord[261565];                 // mov ax, gameState
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 6;
    word res = dstSigned - srcSigned;
    *(word *)&D2 = res;
}                                       // sub word ptr D2, 6
{
    word res = *(word *)&D2 << 1;
    *(word *)&D2 = res;
}                                       // shl word ptr D2, 1
A0 = 524776;                            // mov A0, offset freeKickFactorsX
esi = A0;                               // mov esi, A0
ebx = *(word *)&D2;                     // movzx ebx, word ptr D2
ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    word res = *(word *)&D2 << 2;
    *(word *)&D2 = res;
}                                       // shl word ptr D2, 2
ax = D2;                                // mov ax, word ptr D2
g_memWord[262395] = ax;                 // mov freeKickDestX, ax
{
    word res = *(word *)&D2 >> 2;
    *(word *)&D2 = res;
}                                       // shr word ptr D2, 2
*(word *)&D1 = 3;                       // mov word ptr D1, 3
esi = A6;                               // mov esi, A6
eax = readMemory(esi + 20, 4);          // mov eax, [esi+TeamGeneralInfo.spritesTable]
A4 = eax;                               // mov A4, eax
{
    int32_t dstSigned = A4;
    int32_t srcSigned = 4;
    dword res = dstSigned + srcSigned;
    A4 = res;
}                                       // add A4, 4

l_next_free_kick_taker:;
esi = A4;                               // mov esi, A4
eax = readMemory(esi, 4);               // mov eax, [esi]
{
    int32_t dstSigned = A4;
    int32_t srcSigned = 4;
    dword res = dstSigned + srcSigned;
    A4 = res;
}                                       // add A4, 4
A0 = eax;                               // mov A0, eax
esi = A0;                               // mov esi, A0
ax = (word)readMemory(esi + 102, 2);    // mov ax, [esi+Sprite.cards]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (flags.sign)
goto l_free_kick_taker_has_red_card; // js short @@free_kick_taker_has_red_card

ax = D2;                                // mov ax, word ptr D2
{
    word src = g_memWord[262395];
    int16_t dstSigned = src;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    src = res;
    g_memWord[262395] = src;
}                                       // sub freeKickDestX, ax

l_free_kick_taker_has_red_card:;
(*(int16_t *)&D1)--;
flags.overflow = (int16_t)(*(int16_t *)&D1) == INT16_MIN;
flags.sign = (*(int16_t *)&D1 & 0x8000) != 0;
flags.zero = *(int16_t *)&D1 == 0;      // dec word ptr D1
if (!flags.sign)
goto l_next_free_kick_taker;        // jns short @@next_free_kick_taker

goto cseg_8381B;                        // jmp short cseg_8381B

l_not_in_free_kick_state:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 2, 2);      // mov ax, [esi+Sprite.playerOrdinal]
*(word *)&D0 = ax;                      // mov word ptr D0, ax

cseg_8381B:;
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 1
{
    word res = *(word *)&D0 << 2;
    *(word *)&D0 = res;
}                                       // shl word ptr D0, 2
esi = A5;                               // mov esi, A5
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
{
    word src = (word)readMemory(esi + ebx, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 22222;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr [esi+ebx], 22222
if (flags.zero)
goto l_set_player_with_no_ball_destination; // jz @@set_player_with_no_ball_destination

{
    int32_t dstSigned = A6;
    int32_t srcSigned = 522804;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, offset topTeamData
if (!flags.zero)
goto cseg_838AA;                    // jnz short cseg_838AA

ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + ebx + 2, 2); // mov ax, [esi+ebx+2]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = -1000;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, -1000
if (flags.zero || flags.sign != flags.overflow)
goto cseg_83981;                    // jle cseg_83981

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 1000
if (flags.sign == flags.overflow)
goto cseg_83999;                    // jge cseg_83999

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 5;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 5
goto cseg_8394B;                        // jmp cseg_8394B

cseg_838AA:;
*(word *)&D3 = -5;                      // mov word ptr D3, -5
esi = A5;                               // mov esi, A5
ebx = *(word *)&D0;                     // movzx ebx, word ptr D0
ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + ebx + 2, 2); // mov ax, [esi+ebx+2]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = -1000;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, -1000
if (flags.zero || flags.sign != flags.overflow)
goto cseg_839B1;                    // jle cseg_839B1

{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D1, 1000
if (flags.sign == flags.overflow)
goto cseg_839D7;                    // jge cseg_839D7

*(word *)&D1 = 509;                     // mov word ptr D1, 509
ax = (word)readMemory(esi + ebx, 2);    // mov ax, [esi+ebx]
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 5;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, 5
*(word *)&D2 = 640;                     // mov word ptr D2, 640
ax = (word)readMemory(esi + ebx + 2, 2); // mov ax, [esi+ebx+2]
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D2 = res;
}                                       // sub word ptr D2, ax

cseg_8394B:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 81;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 81
ax = D1;                                // mov ax, word ptr D1
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = 129;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D2 = res;
}                                       // add word ptr D2, 129
ax = D2;                                // mov ax, word ptr D2
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

cseg_83981:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 1000
ax = g_memWord[262395];                 // mov ax, freeKickDestX
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
goto l_update_goalie_dest_x_y;          // jmp short @@update_goalie_dest_x_y

cseg_83999:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, 1000
ax = g_memWord[262395];                 // mov ax, freeKickDestX
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
goto l_update_goalie_dest_x_y;          // jmp short @@update_goalie_dest_x_y

cseg_839B1:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, 1000
*(int16_t *)&D1 = -*(int16_t *)&D1;     // neg word ptr D1
*(int16_t *)&D2 = -*(int16_t *)&D2;     // neg word ptr D2
ax = g_memWord[262395];                 // mov ax, freeKickDestX
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
goto l_update_goalie_dest_x_y;          // jmp short @@update_goalie_dest_x_y

cseg_839D7:;
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = 1000;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, 1000
*(int16_t *)&D1 = -*(int16_t *)&D1;     // neg word ptr D1
*(int16_t *)&D2 = -*(int16_t *)&D2;     // neg word ptr D2
ax = g_memWord[262395];                 // mov ax, freeKickDestX
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax

l_update_goalie_dest_x_y:;
ax = D6;                                // mov ax, word ptr D6
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    *(word *)&D1 = res;
}                                       // add word ptr D1, ax
ax = D7;                                // mov ax, word ptr D7
{
    int16_t dstSigned = *(word *)&D2;
    int16_t srcSigned = ax;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D2 = res;
}                                       // add word ptr D2, ax
ax = D1;                                // mov ax, word ptr D1
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = D2;                                // mov ax, word ptr D2
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax
goto l_update_player_speed_and_deltas;  // jmp @@update_player_speed_and_deltas

l_set_player_with_no_ball_destination:;
SWOS::SetPlayerWithNoBallDestination();       // call SetPlayerWithNoBallDestination

cseg_83A41:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

esi = A1;                               // mov esi, A1
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

eax = g_memDword[130780];               // mov eax, lastTeamPlayedBeforeBreak
{
    int32_t dstSigned = A6;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A6, eax
if (flags.zero)
goto l_update_player_speed_and_deltas; // jz @@update_player_speed_and_deltas

ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+Sprite.destX]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = g_memWord[261568];                 // mov ax, foulXCoordinate
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, ax
ax = D0;                                // mov ax, word ptr D0
bx = D0;                                // mov bx, word ptr D0
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D0 = ax;                      // mov word ptr D0, ax
*(word *)((byte *)&D0 + 2) = dx;        // mov word ptr D0+2, dx
ax = (word)readMemory(esi + 60, 2);     // mov ax, [esi+Sprite.destY]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = g_memWord[261569];                 // mov ax, foulYCoordinate
{
    int16_t dstSigned = *(word *)&D1;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    *(word *)&D1 = res;
}                                       // sub word ptr D1, ax
ax = D1;                                // mov ax, word ptr D1
bx = D1;                                // mov bx, word ptr D1
{
    int32_t res = (int16_t)ax * (int16_t)bx;
    ax = res & 0xffff;
    dx = res >> 16;
}                                       // imul bx
*(word *)&D1 = ax;                      // mov word ptr D1, ax
*(word *)((byte *)&D1 + 2) = dx;        // mov word ptr D1+2, dx
eax = D1;                               // mov eax, D1
{
    int32_t dstSigned = D0;
    int32_t srcSigned = eax;
    dword res = dstSigned + srcSigned;
    D0 = res;
}                                       // add D0, eax
{
    int32_t dstSigned = D0;
    int32_t srcSigned = 4225;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp D0, 4225
if (!flags.carry && !flags.zero)
goto l_update_player_speed_and_deltas; // ja short @@update_player_speed_and_deltas

ax = g_memWord[261568];                 // mov ax, foulXCoordinate
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 336;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, 336
if (flags.sign != flags.overflow)
goto cseg_83B20;                    // jl short cseg_83B20

{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 70;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D0 = res;
}                                       // sub word ptr D0, 70
goto cseg_83B28;                        // jmp short cseg_83B28

cseg_83B20:;
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 70;
    word res = dstSigned + srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned + srcSigned) < (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 70

cseg_83B28:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
goto l_update_player_speed_and_deltas;  // jmp short @@update_player_speed_and_deltas

l_stop_player:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
writeMemory(esi + 58, 2, ax);           // mov [esi+Sprite.destX], ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
writeMemory(esi + 60, 2, ax);           // mov [esi+Sprite.destY], ax

l_update_player_speed_and_deltas:;
if (g_portedToggles.useCpp_UpdatePlayerSpeed) PlayerFunctions::UpdatePlayerSpeed(); else UpdatePlayerSpeed(); // call UpdatePlayerSpeed
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 58, 2);     // mov ax, [esi+Sprite.destX]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 60, 2);     // mov ax, [esi+Sprite.destY]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D3 = ax;                      // mov word ptr D3, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D4 = ax;                      // mov word ptr D4, ax
ax = (word)readMemory(esi + 44, 2);     // mov ax, [esi+Sprite.speed]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
push(A1);                               // push A1
SWOS::CalculateDeltaXAndY();            // call CalculateDeltaXAndY
pop(A1);                                // pop A1
eax = D1;                               // mov eax, D1
esi = A1;                               // mov esi, A1
writeMemory(esi + 46, 4, eax);          // mov [esi+Sprite.deltaX], eax
eax = D2;                               // mov eax, D2
writeMemory(esi + 50, 4, eax);          // mov [esi+Sprite.deltaY], eax
{
    word src = (word)readMemory(esi + 2, 2);
    int16_t dstSigned = src;
    int16_t srcSigned = 1;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerOrdinal], 1
if (!flags.zero)
goto l_not_goalkeeper;              // jnz short @@not_goalkeeper

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (!flags.zero)
goto l_not_goalkeeper;              // jnz short @@not_goalkeeper

esi = A6;                               // mov esi, A6
ax = (word)readMemory(esi + 140, 2);    // mov ax, [esi+TeamGeneralInfo.goalkeeperPlaying]
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.zero)
goto l_not_goalkeeper;              // jnz short @@not_goalkeeper

eax = g_memWord[262385] | g_memWord[262386] << 16; // mov eax, dword ptr ballInUpperPenaltyArea
flags.carry = false;
flags.overflow = false;
flags.sign = (eax & 0x80000000) != 0;
flags.zero = eax == 0;                  // or eax, eax
if (!flags.zero)
goto l_calc_direction;              // jnz short @@calc_direction

l_not_goalkeeper:;
ax = D0;                                // mov ax, word ptr D0
flags.carry = false;
flags.overflow = false;
flags.sign = (ax & 0x8000) != 0;
flags.zero = ax == 0;                   // or ax, ax
if (!flags.sign)
goto l_got_movement;                // jns short @@got_movement

esi = A6;                               // mov esi, A6
eax = readMemory(esi + 32, 4);          // mov eax, [esi+TeamGeneralInfo.controlledPlayerSprite]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_skip_setting_direction;      // jz short @@skip_setting_direction

eax = readMemory(esi + 104, 4);         // mov eax, [esi+TeamGeneralInfo.passingKickingPlayer]
{
    int32_t dstSigned = A1;
    int32_t srcSigned = eax;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp A1, eax
if (flags.zero)
goto l_skip_setting_direction;      // jz short @@skip_setting_direction

l_calc_direction:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 82, 2);     // mov ax, [esi+Sprite.fullDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 128;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 128
{
    word res = *(word *)&D0 & 255;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0FFh

l_got_movement:;
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = 16;
    word res = dstSigned + srcSigned;
    *(word *)&D0 = res;
}                                       // add word ptr D0, 16
{
    word res = *(word *)&D0 & 255;
    *(word *)&D0 = res;
}                                       // and word ptr D0, 0FFh
{
    word res = *(word *)&D0 >> 5;
    *(word *)&D0 = res;
}                                       // shr word ptr D0, 5
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 42, 2, ax);           // mov [esi+Sprite.direction], ax

l_skip_setting_direction:;
{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto l_ball_going_to_player;        // jz short @@ball_going_to_player

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 20;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_THROW_IN_BACK_LEFT
if (!flags.carry && !flags.zero)
goto l_ball_going_to_player;        // ja short @@ball_going_to_player

ax = g_memWord[261568];                 // mov ax, foulXCoordinate
*(word *)&D3 = ax;                      // mov word ptr D3, ax
ax = g_memWord[261569];                 // mov ax, foulYCoordinate
*(word *)&D4 = ax;                      // mov word ptr D4, ax
goto l_calculate_player_ball_direction; // jmp short @@calculate_player_ball_direction

l_ball_going_to_player:;
esi = A2;                               // mov esi, A2
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D3 = ax;                      // mov word ptr D3, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D4 = ax;                      // mov word ptr D4, ax

l_calculate_player_ball_direction:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 32, 2);     // mov ax, word ptr [esi+(Sprite.x+2)]
*(word *)&D1 = ax;                      // mov word ptr D1, ax
ax = (word)readMemory(esi + 36, 2);     // mov ax, word ptr [esi+(Sprite.y+2)]
*(word *)&D2 = ax;                      // mov word ptr D2, ax
*(word *)&D0 = 256;                     // mov word ptr D0, 256
push(A1);                               // push A1
SWOS::CalculateDeltaXAndY();            // call CalculateDeltaXAndY
pop(A1);                                // pop A1
if (!flags.sign)
goto l_turn_player_toward_ball;     // jns short @@turn_player_toward_ball

*(word *)&D0 = 0;                       // mov word ptr D0, 0

l_turn_player_toward_ball:;
ax = D0;                                // mov ax, word ptr D0
esi = A1;                               // mov esi, A1
writeMemory(esi + 82, 2, ax);           // mov [esi+Sprite.fullDirection], ax
{
    byte src = (byte)readMemory(esi + 12, 1);
    int8_t dstSigned = src;
    int8_t srcSigned = 0;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp [esi+Sprite.playerState], PL_NORMAL
if (!flags.zero)
goto l_next_player;                 // jnz @@next_player

eax = readMemory(esi + 46, 4);          // mov eax, [esi+Sprite.deltaX]
D1 = eax;                               // mov D1, eax
eax = readMemory(esi + 50, 4);          // mov eax, [esi+Sprite.deltaY]
D1 |= eax;
flags.carry = false;
flags.overflow = false;
flags.sign = (D1 & 0x80000000) != 0;
flags.zero = D1 == 0;                   // or D1, eax
*(byte *)&D1 = 0;                       // mov byte ptr D1, 0
if (flags.zero)
goto l_check_player_movement;       // jz short @@check_player_movement

*(byte *)&D1 = -1;                      // mov byte ptr D1, -1

l_check_player_movement:;
esi = A1;                               // mov esi, A1
al = (byte)readMemory(esi + 94, 1);     // mov al, byte ptr [esi+Sprite.isMoving]
{
    int8_t dstSigned = *(byte *)&D1;
    int8_t srcSigned = al;
    byte res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT8_MIN : srcSigned < dstSigned - INT8_MAX;
    flags.carry = (byte)(dstSigned - srcSigned) > (byte)dstSigned;
    flags.sign = (res & 0x80) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp byte ptr D1, al
if (flags.zero)
goto l_no_change_in_movement;       // jz short @@no_change_in_movement

cseg_83D7F:;
al = D1;                                // mov al, byte ptr D1
flags.carry = false;
flags.overflow = false;
flags.sign = (al & 0x80) != 0;
flags.zero = al == 0;                   // or al, al
if (!flags.zero)
goto l_player_is_running;           // jnz short @@player_is_running

A0 = 453234;                            // mov A0, offset playerNormalStandingAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_next_player;                     // jmp short @@next_player

l_player_is_running:;
A0 = 453104;                            // mov A0, offset playerRunningAnimTable
SetPlayerAnimationTable();              // call SetPlayerAnimationTable
goto l_next_player;                     // jmp short @@next_player

l_no_change_in_movement:;
esi = A1;                               // mov esi, A1
ax = (word)readMemory(esi + 92, 2);     // mov ax, [esi+Sprite.playerDirection]
*(word *)&D0 = ax;                      // mov word ptr D0, ax
ax = (word)readMemory(esi + 42, 2);     // mov ax, [esi+Sprite.direction]
{
    int16_t dstSigned = *(word *)&D0;
    int16_t srcSigned = ax;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp word ptr D0, ax
if (!flags.zero)
goto cseg_83D7F;                    // jnz short cseg_83D7F

l_next_player:;
{
    int32_t val = stack[stackTop++];
    *(word *)&D4 = val;
}                                       // pop small [word ptr D4]
(*(int16_t *)&D4)--;
flags.overflow = (int16_t)(*(int16_t *)&D4) == INT16_MIN;
flags.sign = (*(int16_t *)&D4 & 0x8000) != 0;
flags.zero = *(int16_t *)&D4 == 0;      // dec word ptr D4
if (!flags.sign)
goto l_players_loop;                // jns @@players_loop

{
    word src = g_memWord[261564];
    int16_t dstSigned = src;
    int16_t srcSigned = 100;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameStatePl, ST_GAME_IN_PROGRESS
if (flags.zero)
goto cseg_83DF5;                    // jz short cseg_83DF5

{
    word src = g_memWord[261565];
    int16_t dstSigned = src;
    int16_t srcSigned = 3;
    word res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT16_MIN : srcSigned < dstSigned - INT16_MAX;
    flags.carry = (word)(dstSigned - srcSigned) > (word)dstSigned;
    flags.sign = (res & 0x8000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp gameState, ST_KEEPER_HOLDS_BALL
if (!flags.zero)
return;                             // jnz short @@out

cseg_83DF5:;
{
    dword src = g_memDword[130777];
    int32_t dstSigned = src;
    int32_t srcSigned = 326524;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp lastPlayerPlayed, offset goalie1Sprite
if (!flags.zero)
goto l_check_goalie2;               // jnz short @@check_goalie2

{
    dword src = g_memWord[262369] | g_memWord[262370] << 16;
    int32_t dstSigned = src;
    int32_t srcSigned = 326524;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp prevLastPlayer, offset goalie1Sprite
if (flags.zero)
return;                             // jz short @@out

goto cseg_83E27;                        // jmp short cseg_83E27

l_check_goalie2:;
{
    dword src = g_memDword[130777];
    int32_t dstSigned = src;
    int32_t srcSigned = 327756;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp lastPlayerPlayed, offset goalie2Sprite
if (!flags.zero)
return;                             // jnz short @@out

{
    dword src = g_memWord[262369] | g_memWord[262370] << 16;
    int32_t dstSigned = src;
    int32_t srcSigned = 327756;
    dword res = dstSigned - srcSigned;
    flags.overflow = dstSigned < 0 ? srcSigned > dstSigned - INT32_MIN : srcSigned < dstSigned - INT32_MAX;
    flags.carry = (dword)(dstSigned - srcSigned) > (dword)dstSigned;
    flags.sign = (res & 0x80000000) != 0;
    flags.zero = res == 0 != 0;
}                                       // cmp prevLastPlayer, offset goalie2Sprite
if (flags.zero)
return;                             // jz short @@out

cseg_83E27:;
eax = g_memWord[262369] | g_memWord[262370] << 16; // mov eax, prevLastPlayer
g_memDword[130692] = eax;               // mov lastPlayerBeforeGoalkeeper, eax
eax = g_memWord[262371] | g_memWord[262372] << 16; // mov eax, prevLastTeamPlayed
g_memDword[130691] = eax;               // mov lastTeamScored, eax
g_memWord[228771] = 50;                 // mov nobodysBallTimer, 50
}
