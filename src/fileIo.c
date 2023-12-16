#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "fileIo.h"

/**
 * Gets the size of a file in bytes.
 * @param f The `FILE*` to retrieve the size of.
 * @return The size of the given file, in bytes.
 */
size_t fGetSize(FILE *f)
{
    // NOTE: Not Windows-compatible
    // Get the size from stat rather than just seeking
    // because the seeking method is non-standard
    struct stat statStruct;
    int descriptor = fileno(f);
    int failed = fstat(descriptor, &statStruct);
    return failed ? 0 : statStruct.st_size;
}

/**
 * Opens a directory for iterating through its contents.
 * This must be closed when done via `fCloseDir`.
 * @param The path of the directory.
 * @return A pointer to a FDir directory.
 */
FDir* fOpenDir(const char *path)
{
    // NOTE: Not Windows-compatible
    return opendir(path);
}

/**
 * Closes an `FDir*` directory opened by `fOpenDir`.
 * @return `true` if succeeded.
 */
bool fCloseDir(FDir *dir)
{
    // NOTE: Not Windows-compatible
    return !closedir(dir);
}

static inline size_t sizeMin(size_t a, size_t b)
{
    return a < b ? a : b;
}

/**
 * Iterates through `FDir` files, copying their name into a buffer.
 * `name` is a buffer containing the file name, but not the full path.
 * The biggest the name can be is 256 bytes. Only catches actual
 * regular files, not directories, etc. if the directory supports it.
 * @param dir      The directory to read from.
 * @param name     The buffer for the null-terminated name of the file to be
 *                 copied into, not the full path.
 * @param nameSize The size of the `name` buffer. The biggest the file name
 *                 could ever be is 256 bytes (including the NUL),
 *                 so should probably be 256.
 * @return `true` if succeeded, or `false` if there are no more files or if
 *         an error occurred. Sets `errno` if an error occurred.
 */
bool fReadFileDir(FDir *dir, char *name, size_t nameSize)
{
    // NOTE: Not Windows-compatible
    struct dirent64 *entry;

    // If entry is NULL, will exit and return false
    while((entry = readdir64(dir)))
    {
        char type = entry->d_type;
        if(type != DT_REG && type != DT_UNKNOWN) continue;

        strncpy(name, entry->d_name, sizeMin(sizeof entry->d_name, nameSize));
        return true;
    }

    return false;
}

