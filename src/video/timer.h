#pragma once

constexpr int kTargetFpsPC = 70;
constexpr int kTargetFpsAmiga = 50;

void initTimer();
void initFrameTicks();
int targetFps();
void setTargetFps(int fps);
void markFrameStartTime();
void menuFrameDelay();
void gameFrameDelay();
void measureRendering(std::function<void()> render);
