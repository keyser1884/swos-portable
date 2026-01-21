#pragma once

namespace SWOS {
    int randomize(int value);
    int randomize2(int value);
    int rand();
    int rand2();
    void setRandHook(std::function<int()> hook);

    // RNG state for save/restore (used in comparison testing)
    struct RngState {
        byte seed;
        byte xorKey;
        byte xorIndex;
    };

    RngState saveRngState();
    void restoreRngState(const RngState& state);
}
