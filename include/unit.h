#ifndef UNIT_H
#define UNIT_H

#include <stdint.h>

struct Unit
{
    /**
     * Name of the unit (e.g. 's', 'in', 'W').
     */
    const char *name;

    /**
     * List of units depended on by name (e.g. 'km' depends on 'm').
     */
    const char **dependencies;
    /**
     * Count of units in the `dependencies` list.
     */
    uint64_t dependencyCount;

    /**
     * The mangled name of the function that converts this unit into
     * the unit specified in `dependencies` if `dependencyCount` is `1`.
     * If `dependencyCount` is not `1`, `NULL`.
     */
    const char *toDependencyFnName;
};

#endif

