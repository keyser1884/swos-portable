#pragma once

namespace PlayerFunctions {

void ResetBothTeamSpinTimers();
void ThrowInDeadProc();
void GetPlayerPointerFromShirtNumber();
void UpdateTeamShotChanceTable();
void SetThrowInPlayerDestinationCoordinates();
void SetPlayerDowntimeAfterTackle();
void UpdatePlayerWithBall();
void UpdateControllingPlayer();
void GoalkeeperCaughtTheBall();
void UpdateBallWithControllingGoalkeeper();
void AttemptStaticHeader();
void UpdatePlayerSpeed();
void GoalkeeperJumping();
void PlayerBeginTackling();
void PlayersTackledTheBallStrong();
void SetJumpHeaderHitAnimTable();
void SetStaticHeaderDirection();
void PlayerAttemptingJumpHeader();
void AI_Kick();
void CalculateBallNextGroundXYPositions();
void UpdateBallVariables();
void ApplyBallAfterTouch();
void PlayerHittingStaticHeader();
void PlayerHittingJumpHeader();
void PlayerTackledTheBallWeak();
void PlayerKickingBall();
void ShouldGoalkeeperDive();
void PlayerTacklingTestFoul();
void DoPass();
void CalculateIfPlayerWinsBall();

} // namespace PlayerFunctions
