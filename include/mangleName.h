#ifndef MANGLENAME_H
#define MANGLENAME_H

#include <stddef.h>
#include "unit.h"

size_t mangleFunction(char *mangled, size_t mangledLen, const char *moduleName, const char *fnName, const struct Unit *paramUnits, uint64_t paramCount);

#endif

