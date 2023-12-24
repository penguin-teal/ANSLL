#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "utf8encoder.h"
#include "unit.h"

static inline size_t min(size_t a, size_t b)
{
    return a < b ? a : b;
}

static size_t print(char *dest, const char *src, size_t srcLen, size_t currentLen, size_t bufferSize)
{
    if(!srcLen) srcLen = strlen(src);
    if(dest) memcpy(dest + currentLen, src, min(srcLen, bufferSize - currentLen));
    return currentLen + srcLen;
}

static size_t printUtf8Char(char *dest, const char *c, size_t currentLen, size_t bufferSize)
{
    if(isalnum(c[0]) || c[0] == '_')
    {
        if(dest && currentLen < bufferSize - 1) dest[currentLen] = c[0];
        return currentLen + 1;
    }
    else
    {
        if(dest && currentLen < bufferSize - 1) dest[currentLen] = '@';

        char codePoint[7];
        size_t codePointLen;
        char lenChar[2];
        size_t charSize = utf8Size(c[0]);
        if(
            charSize == 0 ||
            (charSize > 1 && c[1] == '\0') ||
            (charSize > 2 && c[2] == '\0') ||
            (charSize > 3 && c[3] == '\0')
        )
        {
            fprintf(stderr, "Invalid UTF-8 character while mangling identifier.\n");
            return 0;
        }

        uint32_t binary = c[0];
        if(charSize >= 2) binary = binary << 8 | c[1];
        if(charSize >= 3) binary = binary << 8 | c[2];
        if(charSize == 4) binary = binary << 8 | c[3];

        uint64_t cpInt = utf8Decode(binary);
        snprintf(codePoint, sizeof codePoint, "%lx", cpInt);
        codePointLen = strlen(codePoint);
        snprintf(lenChar, sizeof lenChar, "%lx", codePointLen);

        if(dest)
        {
            if(currentLen + 1 < bufferSize - 1) dest[currentLen + 1] = lenChar[0];
            memcpy(dest + currentLen + 2, codePoint, min(codePointLen, bufferSize - currentLen - 2));
        }
        return currentLen + 1 + 1 + codePointLen;
    }
}

static size_t printUtf8String(char *dest, const char *s, size_t currentLen, size_t bufferSize)
{
    size_t len = currentLen;
    const char *inp = s;
    while(inp++[0])
    {
        len = printUtf8Char(dest, inp, len, bufferSize);
    }
    return len;
}

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
size_t mangleFunction(char *mangled, size_t mangledLen, const char *moduleName, const char *fnName, const struct Unit *paramUnits, uint64_t paramCount)
{
    // ANSLL Standard 1a

    size_t len = 0;

    // print doesn't guarantee a terminating NUL so ensure there is one
    // if print fails
    size_t bufLen = mangledLen - 1;
    if(mangled) mangled[bufLen] = '\0';

    len = print(mangled, "_AF$", 4, len, bufLen);
    
    uint64_t i = 0;
    while(moduleName[i])
    {
        // moduleName[i + 1] will always be another char or NUL
        if(moduleName[i] == ':' && moduleName[i + 1] == ':')
        {
            // effectively replace "::" with "?"
            len = printUtf8Char(mangled, "?", len, bufLen);
            // advance the extra ':'
            i += 2;
        }
        else
        {
            len = printUtf8Char(mangled, moduleName + i, len, bufLen);
            i++;
        }
    }

    len = print(mangled, "$", 1, len, bufLen);

    len = printUtf8String(mangled, fnName, len, bufLen);

    len = print(mangled, "$P$", 2, len, bufLen);

    for(uint64_t u = 0; u < paramCount; u++)
    {
        const char *uName = paramUnits[u].name;
        len = printUtf8Char(mangled, uName, len, bufLen);
        len = print(mangled, "$", 1, len, bufLen);
    }

    if(len < mangledLen) mangled[len] = '\0';

    return len;
}

