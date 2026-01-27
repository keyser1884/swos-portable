#include "comments.h"
#include "audio.h"
#include "chants.h"
#include "comments.h"
#include "file.h"
#include "hash.h"
#include "swos_zip.h"
#include "SampleTable.h"
#include "SoundSample.h"
#include "legacyCommentaryTables.h"

#include <unordered_map>
#include <iterator>

constexpr int kEnqueuedSampleDelay = 70;
constexpr int kEnqueuedCardSampleDelay = 100;


static SoundSample m_endGameCrowdSample;

static Mix_Chunk *m_lastPlayedComment;
static size_t m_lastPlayedCommentHash;
static int m_lastPlayedCategory;

static int m_commentaryChannel = -1;

static bool m_muteCommentary;
static bool m_commentaryLoaded;

static bool m_performingPenalty;

static int m_tacticsChangedSampleTimer;
static int m_substituteSampleTimer;
static int m_playingYellowCardTimer;
static int m_playingRedCardTimer;

static void loadZipComments();
static bool sampleTablesEmpty();
static void playGoodPassComment();
static void playYellowCardSample();
static void playRedCardSample();
static void playCornerSample();
static void playTacticsChangeSample();
static void playThrowInSample();
static void playSubstituteSample();
static void loadAndPlayEndGameCrowdSample(int index);
static void playDrawComment();
static void playSensationalGameComment();
static void playItsBeenACompleteRoutComment();
static void loadLegacyCommentaryFromCdLayout();

//
// sample loading
//

// these must match exactly with directories in the sample tables below
enum CommentarySampleTableIndex {
    kCorner, kDirtyTackle, kEndGameRout, kEndGameSensational, kEndGameSoClose, kFoul, kGoal, kGoodPass, kGoodTackle,
    kHeader, kHitBar, kHitPost, kInjury, kKeeperClaimed, kKeeperSaved, kNearMiss, kOwnGoal, kPenalty, kPenaltyMissed,
    kPenaltySaved, kPenaltyGoal, kRedCard, kSubstitution, kChangeTactics, kThrowIn, kYellowCard,
    kNumSampleTables,
};

// all the categories of comments heard in the game
static std::array<SampleTable, kNumSampleTables> m_sampleTables = {{
    { "corner", 6, 79207 },
    { "dirty_tackle", 12, 4961164 },
    { "end_game_rout", 13, 9908804 },
    { "end_game_sensational", 20, 1268374675 },
    { "end_game_so_close", 17, 158539751 },
    { "free_kick", 9, 625437 },
    { "goal", 4, 19087 },
    { "good_play", 9, 616451 },
    { "good_tackle", 11, 2461583 },
    { "header", 6, 78775 },
    { "hit_bar", 7, 158036 },
    { "hit_post", 8, 316199 },
    { "injury", 6, 77918 },
    { "keeper_claimed", 14, 20053633 },
    { "keeper_saved", 12, 5012667 },
    { "near_miss", 9, 625525 },
    { "own_goal", 8, 311795 },
    { "penalty", 7, 156363 },
    { "penalty_missed", 14, 20030505 },
    { "penalty_saved", 13, 10016272 },
    { "penalty_scored", 14, 20031917 },
    { "red_card", 8, 310733 },
    { "substitution", 12, 5082073 },
    { "tactic_changed", 14, 19200561 },
    { "throw_in", 8, 305761 },
    { "yellow_card", 11, 2401124 },
}};

static void loadCustomCommentary();

void loadCommentary()
{
    if (!soundEnabled() || !commentaryEnabled() || swos.g_trainingGame)
        return;

    if (sampleTablesEmpty() || !m_commentaryLoaded) {
        loadCustomCommentary();
        m_commentaryLoaded = true;
    }

    m_muteCommentary = !commentaryEnabled();
}

void playEndGameCrowdSampleAndComment()
{
    if (!soundEnabled() || !commentaryEnabled())
        return;

    auto team2Losing = [] {
        return swos.team2GoalsDigit1 < swos.team1GoalsDigit1 ||
            swos.team2GoalsDigit1 == swos.team1GoalsDigit1 && swos.team2GoalsDigit2 < swos.team1GoalsDigit2;
    };

    int index;
    if (swos.team1GoalsDigit1 == swos.team2GoalsDigit1 && swos.team1GoalsDigit2 == swos.team2GoalsDigit2)
        index = 2;
    else if (team2Losing())
        index = 0;
    else
        index = 1;

    loadAndPlayEndGameCrowdSample(index);

    int goalDiff = std::abs(swos.statsTeam1Goals - swos.statsTeam2Goals);

    if (goalDiff >= 3)
        playItsBeenACompleteRoutComment();
    else if (!goalDiff)
        playDrawComment();
    else if (swos.statsTeam1Goals + swos.statsTeam2Goals >= 4)
        playSensationalGameComment();
}

void initCommentsBeforeTheGame()
{
    m_commentaryChannel = -1;

    m_lastPlayedCategory = -1;
    m_lastPlayedComment = nullptr;

    m_endGameCrowdSample.free();
}

void enqueueTacticsChangedSample()
{
    m_tacticsChangedSampleTimer = kEnqueuedSampleDelay;
}

void enqueueSubstituteSample()
{
    m_substituteSampleTimer = kEnqueuedSampleDelay;
}

void enqueueYellowCardSample()
{
    m_playingYellowCardTimer = kEnqueuedCardSampleDelay;
}

void enqueueRedCardSample()
{
    m_playingRedCardTimer = kEnqueuedCardSampleDelay;
}

void playEnqueuedSamples()
{
    if (m_playingYellowCardTimer >= 0 && !--m_playingYellowCardTimer) {
        playYellowCardSample();
        m_playingYellowCardTimer = -1;
    } else if (m_playingRedCardTimer >= 0 && !--m_playingRedCardTimer) {
        playRedCardSample();
        m_playingRedCardTimer = -1;
    } else if (swos.playingGoodPassTimer >= 0 && !--swos.playingGoodPassTimer) {
        playGoodPassComment();
        swos.playingGoodPassTimer = -1;
    } else if (swos.playingThrowInSample >= 0 && !--swos.playingThrowInSample) {
        playThrowInSample();
        swos.playingThrowInSample = -1;
    } else if (swos.playingCornerSample >= 0 && !--swos.playingCornerSample) {
        playCornerSample();
        swos.playingCornerSample = -1;
    } else if (m_substituteSampleTimer >= 0 && !--m_substituteSampleTimer) {
        playSubstituteSample();
        m_substituteSampleTimer = -1;
    } else if (m_tacticsChangedSampleTimer >= 0 && !--m_tacticsChangedSampleTimer) {
        playTacticsChangeSample();
        m_tacticsChangedSampleTimer = -1;
    }

    // strange place to decrement this...
    if (swos.goalCounter)
        swos.goalCounter--;

    playCrowdChants();
}

bool commenteryOnChannelFinished(int channel)
{
    if (channel == m_commentaryChannel) {
        logDebug("Commentary finished playing on channel %d", channel);
        m_commentaryChannel = -1;
        return true;
    }

    return false;
}

void toggleMuteCommentary()
{
    m_muteCommentary = !m_muteCommentary;
}

static void loadCustomCommentary()
{
    assert(m_sampleTables.size() == kNumSampleTables);
    assert(!strcmp(m_sampleTables[kEndGameSoClose].dir(), "end_game_so_close"));
    assert(!strcmp(m_sampleTables[kYellowCard].dir(), "yellow_card"));

    if (usingCustomAudioDir()) {
        const std::string audioPath = joinPaths("audio", "commentary");

        for (int i = 0; i < kNumSampleTables; i++) {
            auto& table = m_sampleTables[i];
            table.loadSamples(audioPath);
        }

        loadZipComments();
    } else {
        loadLegacyCommentaryFromCdLayout();
    }
}

static void loadZipComments()
{
    // Uniquely maps directory hash values mod 97 to their indices in the sample tables.
    constexpr int kModValue = 97;
    static const int8_t kHashToIndex[kModValue] = {
        0, 0, 2, 0, 0, 19, 0, 0, 0, 0, 0, 10, 0, 0, 9, 0, 8, 25, 0, 0, 0, 0, 0, 11, 0, 0, 0, 13,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 5, 22, 0, 0, 0, 0, 14, 0, 23, 0, 0, 20, 0, 0, 1,
        4, 0, 0, 21, 3, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 7, 12, 0, 6, 0, 0, 0, 0, 26,
        0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 15, 18
    };
    assert(m_sampleTables.size() == 26);
    assert(m_sampleTables[18].dirHash() % kModValue == 5 && m_sampleTables[17].dirHash() % kModValue == 96);

    SampleTable *table{};
    auto filterCustomComments = [&](const char *path, int pathLength) {
        auto hash = initialHash();
        for (int i = 0; i < pathLength && path[i] && path[i] != '/' && path[i] != '\\'; i++)
            hash = updateHash(hash, path[i], i);
        int index = kHashToIndex[hash % kModValue] - 1;
        if (index >= 0) {
            table = &m_sampleTables[index];
            if (table->dirHash() == hash && !_strnicmp(table->dir(), path, table->dirLen()))
                return SoundSample::additionalHeaderSize(path);
        }
        return -1;
    };

    auto processCommentData = [&](const char *path, int pathLength, char *buffer, int bufferLength) {
        assert(table);
        table->addSample(path, pathLength, buffer, bufferLength);
    };

    const auto& relativePath = joinPaths("audio", "commentary.zip");
    const auto& zipPath = pathInRootDir(relativePath.c_str());

    if (!traverseZipFile(zipPath.c_str(), filterCustomComments, processCommentData))
        logWarn("Couldn't open commentary zip file");
}

static bool sampleTablesEmpty()
{
    return std::all_of(m_sampleTables.begin(), m_sampleTables.end(), [](const auto& table) {
        return table.empty();
    });
}

static bool commentPlaying()
{
    return m_commentaryChannel >= 0 && Mix_Playing(m_commentaryChannel);
}

static void playComment(Mix_Chunk *chunk, bool interrupt = true)
{
    assert(chunk);

    if (commentPlaying()) {
        if (!interrupt) {
            logDebug("Playing comment aborted since the previous comment is still playing");
            return;
        }

        logDebug("Interrupting previous comment");
        Mix_HaltChannel(m_commentaryChannel);
    }

    Mix_VolumeChunk(chunk, MIX_MAX_VOLUME);
    m_commentaryChannel = Mix_PlayChannel(-1, chunk, 0);

    m_lastPlayedComment = chunk;
    m_lastPlayedCommentHash = hash(chunk->abuf, chunk->alen);
}

static void playComment(CommentarySampleTableIndex tableIndex, bool interrupt = true)
{
    if (!soundEnabled() || !commentaryEnabled() || m_muteCommentary || swos.g_trainingGame)
        return;

    auto& table = m_sampleTables[tableIndex];

    if (!table.empty()) {
        if (auto chunk = table.getRandomSample(m_lastPlayedComment, m_lastPlayedCommentHash)) {
            playComment(chunk, interrupt);
            m_lastPlayedCategory = tableIndex;
        }
    } else {
        logWarn("Comment table %d empty", tableIndex);
    }
}

#ifdef __ANDROID__
static void vibrate()
{
    if (auto env = reinterpret_cast<JNIEnv *>(SDL_AndroidGetJNIEnv())) {
        if (auto activity = reinterpret_cast<jobject>(SDL_AndroidGetActivity())) {
            if (auto swosClass = env->FindClass("com/sensible_portware/SWOS")) {
                if (auto vibrateMethodId = env->GetMethodID(swosClass, "vibrate", "()V"))
                    env->CallVoidMethod(activity, vibrateMethodId);
                env->DeleteLocalRef(swosClass);
            }
            env->DeleteLocalRef(activity);
        }
    }
}
#endif

//
// SWOS sound hooks
//

// in:
//     A6 -> player's team
//
void SWOS::PlayHeaderComment()
{
    auto team = A6.as<TeamGeneralInfo *>();
    assert(team);

    if (team->playerNumber)
        playComment(kHeader, false);
}

// in:
//     A6 -> player's team
//
void SWOS::PlayInjuryComment()
{
    auto team = A6.as<TeamGeneralInfo *>();
    assert(team);

    if (team->playerNumber)
        playComment(kInjury);
}

static void playPenaltyGoalComment()
{
    playComment(kPenaltyGoal);
    m_performingPenalty = false;
}

static void playPenaltyMissComment()
{
    playComment(kPenaltyMissed);
    m_performingPenalty = false;
}

static void playPenaltySavedComment()
{
    playComment(kPenaltySaved);
    m_performingPenalty = false;
}

void SWOS::PlayPenaltyComment()
{
    m_performingPenalty = true;
    playComment(kPenalty);
}

void SWOS::PlayFoulComment()
{
    playComment(kFoul);
}

void SWOS::PlayDangerousPlayComment()
{
    playComment(kDirtyTackle);
}

static void playGoodPassComment()
{
    playComment(kGoodPass, false);
}

void SWOS::PlayGoodTackleComment()
{
    playComment(kGoodTackle, false);
}

void SWOS::PlayPostHitComment()
{
    playComment(kHitPost);
}

void SWOS::PlayBarHitComment()
{
    playComment(kHitBar);
}

void SWOS::PlayKeeperClaimedComment()
{
    if (m_performingPenalty) {
        playComment(kPenaltySaved);
        m_performingPenalty = false;
    } else {
        // don't interrupt penalty saved comments
        if (m_lastPlayedCategory != kPenaltySaved || !commentPlaying())
            playComment(kKeeperClaimed);
    }
}

void SWOS::PlayNearMissComment()
{
    if (m_performingPenalty || swos.penaltiesState == -1)
        playPenaltyMissComment();
    else
        playComment(kNearMiss);
}

void SWOS::PlayGoalkeeperSavedComment()
{
    if (m_performingPenalty || swos.penaltiesState == -1)
        playPenaltySavedComment();
    else
        playComment(kKeeperSaved);
}

// fix original SWOS bug where penalty flag remains set when penalty is missed, but it's not near miss
void SWOS::FixPenaltyBug()
{
    m_performingPenalty = 0;
}

void SWOS::PlayOwnGoalComment()
{
    playComment(kOwnGoal);
#ifdef __ANDROID__
    vibrate();
#endif
}

void SWOS::PlayGoalComment()
{
    if (m_performingPenalty || swos.penaltiesState == -1)
        playPenaltyGoalComment();
    else
        playComment(kGoal);
#ifdef __ANDROID__
    vibrate();
#endif
}

static void playYellowCardSample()
{
    playComment(kYellowCard);
}

static void playRedCardSample()
{
    playComment(kRedCard);
}

static void playCornerSample()
{
    playComment(kCorner);
}

static void playTacticsChangeSample()
{
    playComment(kChangeTactics);
}

static void playThrowInSample()
{
    playComment(kThrowIn);
}

static void playSubstituteSample()
{
    playComment(kSubstitution);
}

static void loadAndPlayEndGameCrowdSample(int index)
{
    if (swos.g_trainingGame || !commentaryEnabled())
        return;

    assert(index >= 0 && index <= 3);

    // no need to worry about this still being played
    m_endGameCrowdSample.free();

    auto filename = swos.endGameCrowdSamples[index].asPtr();

    std::string path;
    if (usingCustomAudioDir()) {
        path = joinPaths("audio", "fx") + getDirSeparator() + (filename + 5);   // skip "hard\" part
        m_endGameCrowdSample.loadFromFile(path.c_str());
    } else {
        m_endGameCrowdSample.loadFromFile(filename);
    }

    auto chunk = m_endGameCrowdSample.chunk();
    if (chunk) {
        Mix_VolumeChunk(chunk, MIX_MAX_VOLUME);
        Mix_PlayChannel(-1, chunk, 0);
    } else {
        logWarn("Failed to load end game sample %s", usingCustomAudioDir() ? path.c_str() : filename);
    }
}

static void loadLegacyCommentaryFromCdLayout()
{
    auto getFileSize = [](const char *path) -> int {
        auto f = openFile(path);
        if (!f)
            return -1;
        auto size = SDL_RWsize(f);
        SDL_RWclose(f);
        return size < 0 ? -1 : static_cast<int>(size);
    };

    auto buildOffsetIndex = [&]() {
        std::unordered_map<uint32_t, size_t> offsets;
        uint32_t currentOffset = 0;

        for (size_t i = 0; i < std::size(kLegacyCommentaryFiles); i++) {
            const auto *path = kLegacyCommentaryFiles[i];
            int size = getFileSize(path);
            if (size > 0) {
                offsets[currentOffset] = i;
                currentOffset += static_cast<uint32_t>(size);
            } else {
                logWarn("Legacy commentary file missing: %s", path);
            }
        }

        return offsets;
    };

    auto addSamplesFromOffsets = [&](SampleTable& table, const LegacyOffsetSample *samples, size_t count,
        const std::unordered_map<uint32_t, size_t>& offsetIndex) {
        std::unordered_map<std::string, int> counts;
        for (size_t i = 0; i < count; i++) {
            auto it = offsetIndex.find(samples[i].offset);
            if (it == offsetIndex.end()) {
                logWarn("No legacy commentary file for offset %u", samples[i].offset);
                continue;
            }
            const char *path = kLegacyCommentaryFiles[it->second];
            counts[path]++;
        }
        for (const auto& entry : counts)
            table.addSampleFromFile(entry.first.c_str(), entry.second);
    };

    auto addSamplesFromPaths = [&](SampleTable& table, const char *const *paths, size_t count) {
        std::unordered_map<std::string, int> counts;
        for (size_t i = 0; i < count; i++)
            counts[paths[i]]++;
        for (const auto& entry : counts)
            table.addSampleFromFile(entry.first.c_str(), entry.second);
    };

    const auto offsetIndex = buildOffsetIndex();

    for (int i = 0; i < kNumSampleTables; i++)
        m_sampleTables[i].reset();

    addSamplesFromOffsets(m_sampleTables[kGoal], kLegacy_goalSamples, std::size(kLegacy_goalSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kKeeperSaved], kLegacy_goalkeeperSavedSamples, std::size(kLegacy_goalkeeperSavedSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kOwnGoal], kLegacy_ownGoalSamples, std::size(kLegacy_ownGoalSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kNearMiss], kLegacy_nearMissesSamples, std::size(kLegacy_nearMissesSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kHitPost], kLegacy_postHitSamples, std::size(kLegacy_postHitSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kHitBar], kLegacy_barHitSamples, std::size(kLegacy_barHitSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kKeeperClaimed], kLegacy_keeperClaimedSamples, std::size(kLegacy_keeperClaimedSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kGoodTackle], kLegacy_goodTackleSamples, std::size(kLegacy_goodTackleSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kGoodPass], kLegacy_goodPassSamples, std::size(kLegacy_goodPassSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kFoul], kLegacy_foulSamples, std::size(kLegacy_foulSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kDirtyTackle], kLegacy_dangerousPlaySamples, std::size(kLegacy_dangerousPlaySamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kPenalty], kLegacy_itsPenaltySamples, std::size(kLegacy_itsPenaltySamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kPenaltySaved], kLegacy_savedPenaltySamples, std::size(kLegacy_savedPenaltySamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kPenaltyMissed], kLegacy_missedPenaltySamples, std::size(kLegacy_missedPenaltySamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kPenaltyGoal], kLegacy_penaltyGoalSamples, std::size(kLegacy_penaltyGoalSamples), offsetIndex);
    addSamplesFromOffsets(m_sampleTables[kHeader], kLegacy_headerSamples, std::size(kLegacy_headerSamples), offsetIndex);

    addSamplesFromPaths(m_sampleTables[kCorner], kLegacy_cornerSamplesTable, std::size(kLegacy_cornerSamplesTable));
    addSamplesFromPaths(m_sampleTables[kChangeTactics], kLegacy_tacticsChangeSamplesTable, std::size(kLegacy_tacticsChangeSamplesTable));
    addSamplesFromPaths(m_sampleTables[kSubstitution], kLegacy_playerSubstitutedSamplesTable, std::size(kLegacy_playerSubstitutedSamplesTable));
    addSamplesFromPaths(m_sampleTables[kRedCard], kLegacy_redCardSamplesTable, std::size(kLegacy_redCardSamplesTable));
    addSamplesFromPaths(m_sampleTables[kYellowCard], kLegacy_yellowCardSamplesTable, std::size(kLegacy_yellowCardSamplesTable));
    addSamplesFromPaths(m_sampleTables[kEndGameSoClose], kLegacy_drawSamplesTable, std::size(kLegacy_drawSamplesTable));
    addSamplesFromPaths(m_sampleTables[kEndGameRout], kLegacy_completeRoutSamplesTable, std::size(kLegacy_completeRoutSamplesTable));
    addSamplesFromPaths(m_sampleTables[kEndGameSensational], kLegacy_sensationalGameSamplesTable, std::size(kLegacy_sensationalGameSamplesTable));
    addSamplesFromPaths(m_sampleTables[kThrowIn], kLegacy_throwInSamplesTable, std::size(kLegacy_throwInSamplesTable));
}

static void playDrawComment()
{
    playComment(kEndGameSoClose);
}

static void playSensationalGameComment()
{
    playComment(kEndGameSensational);
}

static void playItsBeenACompleteRoutComment()
{
    playComment(kEndGameRout);
}

#ifdef SWOS_TEST
void clearCommentsSampleCache()
{
    for (auto& table : m_sampleTables)
        table.reset();
    setEnqueueTimers({ -1,-1,-1,-1,-1,-1,-1 });
    m_performingPenalty = false;
    m_commentaryLoaded = false;
}

void setEnqueueTimers(const std::vector<int>& values)
{
    assert(values.size() >= 7);

    m_playingYellowCardTimer = values[0];
    m_playingRedCardTimer = values[1];
    swos.playingGoodPassTimer = values[2];
    swos.playingThrowInSample = values[3];
    swos.playingCornerSample = values[4];
    m_substituteSampleTimer = values[5];
    m_tacticsChangedSampleTimer = values[6];
}
#endif
