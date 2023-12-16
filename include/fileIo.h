#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdbool.h>

#include <dirent.h>

// NOTE: Platform specific IO details are abstracted so Windows support
// can be added at some point

/**
 * A directory for iterating through files with.
 * This type is opqaue.
 */
typedef DIR FDir;

/**
 * Gets the size of a file in bytes.
 * @param f The `FILE*` to retrieve the size of.
 * @return The size of the given file, in bytes.
 */
size_t fGetSize(FILE *f);

/**
 * Opens a directory for iterating through its contents.
 * This must be closed when done via `fCloseDir`.
 * @param The path of the directory.
 * @return A pointer to a FDir directory.
 */
FDir* fOpenDir(const char *path);

/**
 * Closes an `FDir*` directory opened by `fOpenDir`.
 * @return `true` if succeeded.
 */
bool fCloseDir(FDir *dir);

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
bool fReadFileDir(FDir *dir, char *name, size_t nameSize);

#endif

