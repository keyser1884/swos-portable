#include "game.h"
#include "timer.h"

static constexpr int kMenuTargetFps = 30;

static constexpr int kMaxLastFrames = 32;

static int m_targetFps = kTargetFpsPC;

static Sint64 m_frequency;
static Uint64 m_frameStartTime;
static Uint64 m_ticksPerFrame;

class TicksPerFrame {
public:
    void init(int targetFps, Uint64 frequency) {
        m_targetFps = targetFps;
        m_ticksPerFrame = frequency / targetFps;
        m_remainderTicksPerFrame = frequency % targetFps;
        m_additionalTicks = 0;
    }
    Sint64 calculateDelay(Sint64 skew) {
        auto desiredDelay = m_ticksPerFrame - skew;
        m_additionalTicks += m_remainderTicksPerFrame;
        if (m_additionalTicks > m_targetFps) {
            m_additionalTicks -= m_targetFps;
            desiredDelay++;
        }
        desiredDelay = std::max<Sint64>(0, desiredDelay);
        Sint64 diff = SDL_GetPerformanceCounter() - m_frameStartTime;
        return desiredDelay - diff;
    }
    Uint64 ticksPerFrame() const {
        return m_ticksPerFrame;
    }
private:
    Uint64 m_ticksPerFrame;
    int m_targetFps;
    int m_remainderTicksPerFrame;
    int m_additionalTicks;
};

static TicksPerFrame m_ticksPerGameFrame;
static TicksPerFrame m_ticksPerMenuFrame;

class DurationsBuffer {
public:
    void reset() {
        m_durations.fill(0);
        m_averageDuration = 0;
        m_durationsIndex = 0;
    }
    Uint64 average() const {
        return m_averageDuration;
    }
    void enterNewMeasurement(Sint64 ticks) {
        // make sure nothing huge enters durations array, or it might skew the measurements
        auto limit = m_averageDuration ? m_averageDuration * m_durations.size() : m_frequency / 2;
        auto clampedTicks = std::min<Sint64>(ticks, m_durations.size() * limit);
        m_durations[m_durationsIndex] = clampedTicks;
        m_durationsIndex = (m_durationsIndex + 1) % m_durations.size();
        m_averageDuration = recalculateAverageTime();
    }
private:
    Uint64 recalculateAverageTime() {
        auto sum = std::accumulate(m_durations.begin(), m_durations.end(), 0LL);
        return (sum + m_durations.size() / 2) / m_durations.size();
    }

    std::array<Sint64, kMaxLastFrames> m_durations;
    Uint64 m_averageDuration = 0;
    Uint64 m_targetTicksPerFrame;
    unsigned m_durationsIndex = 0;
};

static DurationsBuffer m_renderTimes;

static Sint64 calculateDelay(TicksPerFrame& ticks);
static void timerProc();
static void delay(Sint64 ticks);
static void sleep(Sint64 ticks);

// One-time initialization.
void initTimer()
{
    m_frequency = SDL_GetPerformanceFrequency();
    m_ticksPerMenuFrame.init(kMenuTargetFps, m_frequency);
}

// Per-match initialization + once at startup.
void initFrameTicks()
{
    m_ticksPerGameFrame.init(m_targetFps, m_frequency);
    m_renderTimes.reset();
}

int targetFps()
{
    return m_targetFps;
}

void setTargetFps(int fps)
{
    m_targetFps = fps;
}

void markFrameStartTime()
{
    m_frameStartTime = SDL_GetPerformanceCounter();
}

void menuFrameDelay()
{
    auto ticks = calculateDelay(m_ticksPerMenuFrame);
    sleep(ticks);
    swos.currentTick++; // keep this going since it's used as a randomization value
    m_ticksPerFrame = m_ticksPerMenuFrame.ticksPerFrame();
}

void gameFrameDelay()
{
    auto ticks = calculateDelay(m_ticksPerGameFrame);
    delay(ticks);
    timerProc();
    m_ticksPerFrame = m_ticksPerGameFrame.ticksPerFrame();
}

void measureRendering(std::function<void()> render)
{
    auto start = SDL_GetPerformanceCounter();
    render();
    auto renderingTime = SDL_GetPerformanceCounter() - start;

    m_renderTimes.enterNewMeasurement(renderingTime);

    m_frameStartTime = SDL_GetPerformanceCounter();
}

static Sint64 calculateDelay(TicksPerFrame& ticks)
{
    return ticks.calculateDelay(m_renderTimes.average());
}

// Simulates SWOS procedure executed at each interrupt 8 tick.
// This will have to change and be calculated dynamically based on time elapsed since the last call.
static void timerProc()
{
    auto now = SDL_GetPerformanceCounter();
    auto ticksElapsed = now - m_frameStartTime;

    int framesElapsed = std::max(std::lround(ticksElapsed / m_ticksPerFrame), 1l);
    framesElapsed = std::min(framesElapsed, 6);

    swos.currentTick += framesElapsed;

    if (!isGamePaused())
        swos.currentGameTick += framesElapsed;
}

// Returns how much the wait missed the target. Positive = overshoot, negative = undershoot.
static void delay(Sint64 ticks)
{
    constexpr int kMinTicksToWait = 50;

    if (ticks > kMinTicksToWait) {
        auto start = SDL_GetPerformanceCounter();
        auto targetTicks = start + ticks;

        // sleep with the intention of waking up just a little bit before the right moment
        sleep(ticks);

        Uint64 currentTicks;
        do {
            // do a little bit of busy waiting to get the render start moment precisely
            currentTicks = SDL_GetPerformanceCounter();
        } while (currentTicks < targetTicks);
    }
}

static void sleep(Sint64 ticks)
{
    // measure deviation from the desired sleep value and what the system actually delivers
    constexpr int kNumFramesForSlackValue = 64;
    static std::array<Uint64, kNumFramesForSlackValue> s_slackValues;
    static int s_slackValueIndex;

    auto slackValue = std::accumulate(s_slackValues.begin(), s_slackValues.end(), 0LL);
    slackValue = (slackValue + (slackValue > 0 ? kNumFramesForSlackValue : -kNumFramesForSlackValue) / 2) / kNumFramesForSlackValue;

    if (ticks > slackValue) {
        auto intendedDelay = 1'000 * (ticks - slackValue) / m_frequency;

        auto delayStart = SDL_GetPerformanceCounter();
        SDL_Delay(static_cast<Uint32>(intendedDelay));
        auto actualDelay = SDL_GetPerformanceCounter() - delayStart;

        s_slackValues[s_slackValueIndex] = actualDelay - intendedDelay * m_frequency / 1'000;
        s_slackValueIndex = (s_slackValueIndex + 1) % kNumFramesForSlackValue;
    }
}
