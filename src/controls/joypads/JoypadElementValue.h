#pragma once

#include "gameControlEvents.h"

enum class JoypadElement {
    kNone,
    kButton,
    kHat,
    kAxis,
    kBall,
};

struct JoypadElementValue {
    JoypadElementValue() = default;
    JoypadElementValue(JoypadElement type, int index, int hatMask)
        : type(type), index(index), hatMask(hatMask) {}
    JoypadElement type;
    int index;
    union {
        int hatMask;
        bool pressed;
        int16_t axisValue;
        struct {
            int dx;
            int dy;
        } ball;
    };

    const char *toString(bool includeValue = false) const;
    const char *toString(char *buf, size_t bufSize, bool includeValue = false) const;

    enum BallAxis {
        kBallX,
        kBallY,
    };

    std::pair<BallAxis, int> getBallAxis() const;

private:
    static const char *hatMaskToString(int mask, const char *prefix = "");
    static const char *elementToString(JoypadElement element);
};

using JoypadElementValueList = std::vector<JoypadElementValue>;
using DefaultJoypadElementList = std::array<JoypadElementValue, kNumDefaultGameControlEvents>;
