#pragma once

#include "SoundSample.h"

class SampleTable
{
public:
    SampleTable(const char *dir, int dirLen, uint32_t dirHash);
    void reset();
    bool empty() const;
    const char *dir() const;
    int dirLen() const;
    uint32_t dirHash() const;
    Mix_Chunk *getRandomSample(const Mix_Chunk *lastPlayedSample, size_t lastPlayedHash);
    void loadSamples(const std::string& baseDir);
    void loadSamplesFromDir(const std::string& dirPath);
    void addSampleFromFile(const char *path, int chance);
    void addSample(const char *filename, int filenameLen, char *buf, int bufLen);

private:
    void addSample(const SoundSample& sample, const char *path);
    int getRandomSampleIndex() const;
    void fixIndexToSkipLastPlayedComment(int& sampleIndex, const Mix_Chunk *lastPlayedSample, size_t lastPlayedHash);
    static bool isLastPlayedComment(SoundSample& sample, const Mix_Chunk *lastPlayedSample, size_t lastPlayedHash);
    static int parseSampleChanceMultiplier(const char *str, size_t len);

    const char *m_dir;
    int m_dirLen;
    uint32_t m_dirHash;
    std::vector<SoundSample> m_samples;
    int m_lastPlayedIndex = -1;
    int m_totalSampleChance = 0;
};
