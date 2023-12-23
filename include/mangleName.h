#ifndef MANGLENAME_H
#define MANGLENAME_H

#include <stddef.h>
#include "unit.h"

/**
 * @param mangled The buffer to copy the mangled name into, or `NULL` if you
 *                just want the size in the return value.
 * @param mangledLen The size of the `mangled` buffer, or `0` if it is `NULL`.
 * @param moduleName The name of the module that contains the function,
 *                   including the `::` separator(s) if applicable.
 * @param fnName The name of the function to mangle.
 * @param paramUnits A list of the units of the parameters or `NULL` if there
 *                   are none.
 * @param paramCount The length of the `paramUnits` list.
 * @return Size (including the NUL) that would be copied into `mangled`,
 *         regardless of whether it succeeded or not.
 */
size_t mangleFunction(char *mangled, size_t mangledLen, const char *moduleName, const char *fnName, const struct Unit *paramUnits, uint64_t paramCount);

#endif

