#pragma once

constexpr int kMaxFilenameLength = 256;
constexpr int kMaxPath = 260;

SDL_RWops *openFile(const char *path, const char *mode = "rb");
int loadFile(const char *path, void *buffer, int maxSize = -1, int skipBytes = 0, bool required = true);
std::pair<char *, int> loadFile(const char *path, int bufferOffset = 0, int skipBytes = 0);
bool saveFile(const char *path, void *buffer, int size);
bool renameFile(const char *oldPath, const char *newPath);

void setRootDir(const char *dir);
std::string rootDir();
std::string pathInRootDir(const char *filename);

#ifdef _WIN32
# define DIR_SEPARATOR "\\"
#else
# define DIR_SEPARATOR "/"
#endif

inline constexpr char getDirSeparator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

inline constexpr bool isFileSystemCaseSensitive()
{
#ifdef _WIN32
    return false;
#else
    return true;
#endif
}

int pathCompare(const char *path1, const char *path2);
int pathNCompare(const char *path1, const char *path2, int count);

std::string joinPaths(const char *path1, const char *path2);
bool fileExists(const char *path);
bool dirExists(const char *path);
bool createDir(const char *path);

const char *getFileExtension(const char *path);
const char *getBasename(const char *path);

struct FoundFile {
    std::string name;
    int extensionOffset;
    FoundFile(const std::string& name, int extensionOffset) : name(name), extensionOffset(extensionOffset) {}
};
using FoundFileList = std::vector<FoundFile>;

FoundFileList findFiles(const char *extension, const char *dirName = nullptr,
    const char **allowedExtensions = nullptr, int numAllowedExtensions = 0);
void traverseDirectory(const char *directory, const char *extension,
    std::function<bool(const char *, int, const char *)> f);
