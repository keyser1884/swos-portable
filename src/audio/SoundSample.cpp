#include "SoundSample.h"
#include "wavFormat.h"
#include "hash.h"
#include "file.h"

const std::array<const char *, 5> SoundSample::kSupportedAudioExtensions = {{
    "raw", "mp3", "wav", "ogg", "flac",
}};

SoundSample::SoundSample(const char *path, int chance /* = 1 */)
{
    loadFromFile(path);
    m_chanceModifier = chance;
}

SoundSample::SoundSample(const char *path, char *buf, int bufSize, int chance /* = 1 */)
{
    load(path, buf, bufSize);
    assert(!m_isRaw || bufSize > sizeof(kWaveHeader));
    m_chanceModifier = chance;
}

SoundSample::SoundSample(const SoundSample& other)
{
    assign(other);
    m_buffer.reset(new char[m_size]);
    memcpy(m_buffer.get(), other.m_buffer.get(), m_size);
}

SoundSample::SoundSample(SoundSample&& other) noexcept
{
    *this = std::move(other);
}

SoundSample::~SoundSample()
{
    free();
}

SoundSample& SoundSample::operator=(SoundSample&& other) noexcept
{
    if (this != &other) {
        assign(other);
        std::swap(m_buffer, other.m_buffer);
        std::swap(m_chunk, other.m_chunk);
    }

    return *this;
}

int SoundSample::additionalHeaderSize(const char *path)
{
    auto ext = getFileExtension(path);
    bool isRaw = isRawExtension(ext + 1);
    return getAudioFileOffset(isRaw);
}

void SoundSample::free()
{
    Mix_FreeChunk(m_chunk);
    m_chunk = nullptr;
    m_buffer.release();
}

bool SoundSample::loadFromFile(const char *path)
{
    return load(path);
}

Mix_Chunk *SoundSample::chunk()
{
    if (!m_chunk)
        loadChunk();

    return m_chunk;
}

bool SoundSample::isChunkLoaded() const
{
    return m_chunk != nullptr;
}

bool SoundSample::hasData() const
{
    return m_buffer != nullptr;
}

int SoundSample::chanceModifier() const
{
    return m_chanceModifier;
}

void SoundSample::setChanceModifier(int chanceModifier)
{
    m_chanceModifier = chanceModifier;
}

unsigned SoundSample::hash() const
{
    return m_hash;
}

bool SoundSample::operator==(const SoundSample& other) const
{
    if (!m_buffer || !other.m_buffer)
        return false;

    return m_hash == other.m_hash && m_size == other.m_size && !memcmp(m_buffer.get(), other.m_buffer.get(), m_size);
}

void SoundSample::assign(const SoundSample& other)
{
    m_size = other.m_size;
    m_isRaw = other.m_isRaw;
    m_is11Khz = other.m_is11Khz;
    m_hash = other.m_hash;
    m_chanceModifier = other.m_chanceModifier;
}

bool SoundSample::load(const char *path, char *buf /* = nullptr */, int bufSize /* = 0 */)
{
    assert(path);

    auto ext = getFileExtension(path);
    bool isRaw = isRawExtension(ext + 1);
    auto bufferOffset = getAudioFileOffset(isRaw);

    if (!buf) {
        auto data = loadFile(path, bufferOffset);

        if (!data.first) {
            // we will try to load the initial given extension twice, but it's fine, with retry maybe it will succeed
            auto baseNameLength = static_cast<unsigned>(ext - path);
            std::tie(data.first, data.second, isRaw) = loadWithAnyAudioExtension(path, baseNameLength, ext);

            if (!data.first) {
                assert(false);
                logWarn("`%s' not found with any supported extension", path);
                return false;
            }
        }

        std::tie(buf, bufSize) = data;
    }

    m_buffer.reset(buf);
    m_size = bufSize;
    m_isRaw = isRaw;
    m_is11Khz = is11KhzSample(path);

    return true;
}

// "Dresses up" raw into wave format since SDL Mix doesn't understand raw files.
// Internal buffer was allocated with extra space at the beginning for the wave header.
void SoundSample::convertRawToWav()
{
    assert(m_size >= kSizeofWaveHeader);

    memcpy(m_buffer.get(), kWaveHeader, kSizeofWaveHeader);

    auto waveHeader = reinterpret_cast<WaveFileHeader *>(m_buffer.get());
    waveHeader->size = sizeof(kWaveHeader) + m_size - kSizeofWaveHeader - 8;

    auto waveFormatHeader = reinterpret_cast<WaveFormatHeader *>(reinterpret_cast<char *>(waveHeader) + sizeof(*waveHeader));
    waveFormatHeader->frequency = m_is11Khz ? 11'025 : 22'050;
    waveFormatHeader->byteRate = waveFormatHeader->frequency;

    auto waveDataHeader = reinterpret_cast<WaveDataHeader *>(reinterpret_cast<char *>(waveFormatHeader) + sizeof(*waveFormatHeader));
    waveDataHeader->length = m_size - kSizeofWaveHeader;

    auto rwOps = SDL_RWFromMem(m_buffer.get(), m_size);
    m_chunk = rwOps ? Mix_LoadWAV_RW(rwOps, 1) : nullptr;
}

void SoundSample::loadChunk()
{
    assert(!!m_buffer);

    if (m_buffer && !m_chunk) {
        if (m_isRaw)
            convertRawToWav();
        else if (auto rwOps = SDL_RWFromMem(m_buffer.get(), m_size))
            m_chunk = Mix_LoadWAV_RW(rwOps, 0);

        if (m_chunk)
            m_hash = ::hash(m_chunk->abuf, m_chunk->alen);
    }

    assert(m_chunk);
}

std::tuple<char *, unsigned, bool> SoundSample::loadWithAnyAudioExtension(const char *path, unsigned baseNameLength, const char *ext)
{
    char buf[kMaxFilenameLength];

    if (baseNameLength >= sizeof(buf) - getMaxAudioExensionLength() - 1)
        return {};

    memcpy(buf, path, baseNameLength);
    buf[baseNameLength++] = '.';

    for (auto ext : kSupportedAudioExtensions) {
        bool isRaw = ext[0] == 'r';
        auto offset = getAudioFileOffset(isRaw);

        assert(baseNameLength + strlen(ext) < sizeof(buf));
        strcpy(buf + baseNameLength, ext);

        auto data = loadFile(buf, offset);

        if (data.first)
            return { data.first, static_cast<unsigned>(data.second), isRaw };
    }

    return {};
}

int SoundSample::getAudioFileOffset(bool isRaw)
{
    return isRaw ? kSizeofWaveHeader : 0;
}

bool SoundSample::isRawExtension(const char *ext)
{
    return tolower(ext[0]) == 'r' && tolower(ext[1]) == 'a' && tolower(ext[2]) == 'w' && ext[3] == '\0';
}

bool SoundSample::is11KhzSample(const char *name)
{
    auto len = strlen(name);
    return len >= 13 && !_stricmp(name + len - 13, "\\endgamew.raw") || len >= 9 && !_stricmp(name + len - 9, "\\foul.raw");
}

int SoundSample::getMaxAudioExensionLength()
{
    static int maxLength = -1;

    if (maxLength < 0)
        for (auto ext : kSupportedAudioExtensions)
            maxLength = std::max(maxLength, static_cast<int>(strlen(ext)));

    return maxLength;
}
