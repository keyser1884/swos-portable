#include "swos_zip.h"
#include "log.h"
#include "file.h"
#include <mz.h>
#include <mz_zip.h>
#include <mz_strm.h>
#include <mz_zip_rw.h>

bool traverseZipFile(const char *path, std::function<int(const char *path, int pathLength)> filterEntryFunc,
    std::function<void(const char *path, int pathLength, char *buffer, int bufferLength)> processEntryFunc)
{
    auto data = loadFile(path);
    auto buf = std::unique_ptr<uint8_t[]>(reinterpret_cast<uint8_t *>(data.first));
    auto bufLen = data.second;

    if (!buf) {
#ifdef SWOS_TEST
        // for tests just pretend to succeed even if the file is missing
        return true;
#endif
        logWarn("Zip file \"%s\" not found", path);
        return false;
    }

    bool result = true;
    void *zipReader = nullptr;

    // minizip-ng 4.x changed the API: create() now returns the handle directly
    // instead of taking a pointer-to-pointer argument
#if MZ_VERSION_BUILD >= 0x040000
    zipReader = mz_zip_reader_create();
#else
    mz_zip_reader_create(&zipReader);
#endif

    if (zipReader) {
        if (mz_zip_reader_open_buffer(zipReader, buf.get(), static_cast<int32_t>(bufLen), 0) == MZ_OK) {
            if (mz_zip_reader_goto_first_entry(zipReader) == MZ_OK) {
                int i = 0;
                do {
                    mz_zip_file *fileInfo{};
                    if (mz_zip_reader_entry_get_info(zipReader, &fileInfo) != MZ_OK) {
                        logWarn("Unable to get zip file \"%s\" entry %d info", path, i);
                        result = false;
                        break;
                    }
                    if (mz_zip_reader_entry_is_dir(zipReader) != MZ_OK) {
                        int extraSpace = filterEntryFunc(fileInfo->filename, fileInfo->filename_size);
                        if (extraSpace >= 0) {
                            if (mz_zip_reader_entry_open(zipReader) == MZ_OK) {
                                int size = static_cast<int>(fileInfo->uncompressed_size) + extraSpace;
                                auto buffer = new char[size];
                                int bytesRead = mz_zip_reader_entry_read(zipReader, buffer + extraSpace, size);
                                if (bytesRead + extraSpace == size) {
                                    processEntryFunc(fileInfo->filename, fileInfo->filename_size, buffer, size);
                                } else {
                                    logWarn("Unable to read zip file \"%s\" entry %d info data (total %d, read %d)",
                                        path, i, size, bytesRead);
                                    delete[] buffer;
                                    result = false;
                                    break;
                                }
                                mz_zip_reader_entry_close(zipReader);
                            } else {
                                logWarn("Unable to open zip file \"%s\" entry %d", path, i);
                                result = false;
                                break;
                            }
                        }
                    }
                    i++;
                } while (mz_zip_reader_goto_next_entry(zipReader) == MZ_OK);
            } else {
                logWarn("Unable to go to the first entry for zip file \"%s\"", path);
                result = false;
            }
        } else {
            logWarn("Unable to open zip buffer for zip file \"%s\"", path);
            result = false;
        }
        mz_zip_reader_delete(&zipReader);
    } else {
        logWarn("Unable to create zip reader for zip file \"%s\"", path);
        result = false;
    }

    return result;
}
