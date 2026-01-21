#pragma once

#include "ReplayDataStorage.h"

class LegacyReplayConverter
{
public:
    LegacyReplayConverter(ReplayDataStorage& storage) : m_storage(storage) {}
    void convertLegacyData(const ReplayDataStorage::DataStore& legacyData, bool isReplay);
    int processLegacyFrame(const ReplayDataStorage::DataStore& legacyData, int& current, int sceneStart, int sceneEnd);
    int processLegacySprite(const ReplayDataStorage::DataStore& legacyData, int& current, int sceneStart, int sceneEnd);
    static void convertLegacyHeader(const HilV1Header& v1Header, HilV2Header& v2Header);
    static int findLegacyFrameStart(const ReplayDataStorage::DataStore& legacyData, int& current, int sceneStart, int sceneEnd);
    static void wrappedIncrement(int& current, int sceneStart, int sceneEnd);
    template<size_t N> int signExtend(int value);

private:
    ReplayDataStorage& m_storage;
};
