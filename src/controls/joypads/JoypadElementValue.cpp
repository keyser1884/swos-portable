#include "JoypadElementValue.h"
#include "util.h"

const char *JoypadElementValue::toString(bool includeValue /* = false */) const
{
    static char buf[128];
    return toString(buf, sizeof(buf), includeValue);
}

const char *JoypadElementValue::toString(char *buf, size_t bufSize, bool includeValue /* = false */) const
{
    char valueBuf[32] = "";
    const char *sign = "";
    const char *extraDesc = "";

    switch (type) {
    case JoypadElement::kAxis:
        if (includeValue)
            snprintf(valueBuf, sizeof(valueBuf), ": %d", axisValue);
        else
            sign = axisValue > 0 ? "+" : "-";
        break;
    case JoypadElement::kBall:
        {
            const auto& ball = getBallAxis();
            extraDesc = ball.first == kBallX? " X" : " Y";
            if (includeValue)
                snprintf(valueBuf, sizeof(valueBuf), ": %d", ball.second);
            else
                sign = sgn(ball.second) >= 0 ? "+" : "-";
        }
        break;
    case JoypadElement::kHat:
        extraDesc = hatMaskToString(hatMask, " ");
        break;
    }

    snprintf(buf, bufSize, "%s %d%s%s%s", elementToString(type), index + 1, extraDesc, valueBuf, sign);
    return buf;
}

auto JoypadElementValue::getBallAxis() const -> std::pair<BallAxis, int>
{
    assert(type == JoypadElement::kBall);

    if (ball.dx && !ball.dy)
        return { kBallX, ball.dx };
    else if (!ball.dx && ball.dy)
        return { kBallY, ball.dy };
    else {
        if (std::abs(ball.dx) > std::abs(ball.dy))
            return { kBallX, ball.dx };
        else
            return { kBallY, ball.dy };
    }
}

const char *JoypadElementValue::hatMaskToString(int mask, const char *prefix /* = "" */)
{
    static char pos[64];

    assert(strlen(prefix) + 16 < sizeof(pos));

    strcpy(pos, prefix);

    if (mask & SDL_HAT_LEFT)
        strcat(pos, "LEFT");
    if (mask & SDL_HAT_RIGHT)
        strcat(pos, "RIGHT");
    if (mask & SDL_HAT_UP)
        strcat(pos, "UP");
    if (mask & SDL_HAT_DOWN)
        strcat(pos, "DOWN");

    return pos;
}

const char *JoypadElementValue::elementToString(JoypadElement element)
{
    switch (element) {
    case JoypadElement::kButton:
        return "BUTTON";
    case JoypadElement::kHat:
        return "HAT";
    case JoypadElement::kAxis:
        return "AXIS";
    case JoypadElement::kBall:
        return "BALL";
    default:
        assert(false);
        return "";
    }
}
