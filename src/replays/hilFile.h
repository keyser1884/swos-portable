#pragma once

#pragma pack(push, 1)
struct HilV1Header
{
    dword magic1;
    dword magic2;
    TeamGame team1;
    TeamGame team2;
    char gameName[100];
    char gameRound[100];
    word numScenes;
    word team1Goals;
    word team2Goals;
    byte pitchType;
    byte pitchNumber;
    word numMaxSubstitutes;
};

static_assert(sizeof(HilV1Header) == 3'626, "HilV1Header succumbs to heat");

struct HilV2Header
{
    char magic[4];
    union {
        struct {
            word major;
            word minor;
        };
        dword version;
    };
    dword dataBufferOffset;
    TeamGame team1;
    TeamGame team2;
    char gameName[40];
    char gameRound[40];
    word numScenes;
    word team1Goals;
    word team2Goals;
    byte pitchType;
    byte pitchNumber;
    word numMaxSubstitutes;
    word padding;
};
#pragma pack(pop)

static_assert(sizeof(HilV2Header) % 4 == 0, "Header size must be dword-aligned");

constexpr int kHilV1Magic1 = 8;
constexpr int kHilV1Magic2 = 193'626;

constexpr char kHilV2Magic[4] = { 'H', 'I', 'L', '2' };
