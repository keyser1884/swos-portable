#pragma once

#pragma pack(push, 1)
struct WaveFileHeader {
    dword id;
    dword size;
    dword format;
};

struct WaveFormatHeader {
    dword id;
    dword size;
    word format;
    word channels;
    dword frequency;
    dword byteRate;
    word sampleSize;
    word bitsPerSample;
};

struct WaveDataHeader {
    dword id;
    dword length;
};
#pragma pack(pop)

constexpr int kSizeofWaveHeader = sizeof(WaveFileHeader) + sizeof(WaveFormatHeader) + sizeof(WaveDataHeader);

static constexpr char kWaveHeader[] = {
    'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E',                                 // file header
    'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 8, 0,    // format header
    'd', 'a', 't', 'a', 0, 0, 0, 0,                                                     // data header
};
static_assert(sizeof(kWaveHeader) == kSizeofWaveHeader, "Wave header format invalid");
