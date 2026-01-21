#pragma once

// AI module for SWOS port
// Contains AI_SetControlsDirection and related helper functions

namespace AI {

// Main AI function - sets controls direction for CPU-controlled teams
// Called from UpdatePlayers
void SetControlsDirection();

// Call AI with comparison mode support
// If comparison mode is enabled, runs both ASM and C++ versions and logs differences
// teamPtr should be the value from A6 register
void CallWithComparison(bool useCpp);

} // namespace AI
