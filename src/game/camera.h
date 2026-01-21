#pragma once

#include "FixedPoint.h"

FixedPoint getCameraX();
FixedPoint getCameraY();
void setCameraX(FixedPoint value);
void setCameraY(FixedPoint value);

void moveCamera();
void setCameraToInitialPosition();
void switchCameraToLeavingBenchMode();

