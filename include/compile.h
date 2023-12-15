#ifndef COMPILE_H
#define COMPILE_H

#include <stdbool.h>
#include "token.h"

/**
 * Compiles a list of tokens and the associated string table into
 * an object file of a name specified by `objFilename`.
 * @param tokens The array of tokens terminated by an EOF token.
 * @param strings A string table that is a concatenation of every string,
 *                null-terminated.
 * @param objFilename The name of the object file to output to (typically *.o).
 * @param verbose `true` to print verbose messages to stdout of what is going
 *                on.
 * @return `true` if compilation succeeded and `objFilename` was written to.
 */
bool compileToObject(
    const struct Token *tokens,
    const char *strings,
    const char *objFilename,
    bool verbose
);

#endif

