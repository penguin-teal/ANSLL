#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "appArgs.h"
#include "lexer.h"
#include "fileIo.h"
#include "compile.h"

static inline bool strEndsWith(const char *s, size_t sLen, const char *substr, size_t substrLen)
{
    if(!sLen) sLen = strlen(s);
    if(!substrLen) substrLen = strlen(substr);
    if(substrLen > sLen) return false;
    return !memcmp(s + sLen - substrLen, substr, substrLen);
}

int main(int argc, char **argv)
{
    int ret = 0;

    struct AppArgs args;
    if(!doArgp(&args, argc, argv)) return 2;
    
    const char *dirName = args.args[0];
    uint64_t dirNameLen = strlen(dirName);
    FDir *srcDir = fOpenDir(dirName);
    if(!srcDir)
    {
        fprintf(stderr, "Couldn't open directory '%s'.\n", dirName);
        return 2;
    }

    while(true)
    {
        errno = 0;
        char fileName[256];
        if(!fReadFileDir(srcDir, fileName, sizeof fileName))
        {
            if(!errno) break;
            else
            {
                fprintf(
                        stderr,
                        "Error when getting file from dir (0x%3x): %s.\n",
                        errno,
                        strerror(errno)
                );
                ret = 1;
                goto ReadDirFailed;
            }
        }

        uint64_t fileNameLen = strlen(fileName);

        if(!strEndsWith(fileName, fileNameLen, ".ansll", 6))
        {
            if(args.verbose) printf("Skipping file '%s' since it doesn't have the '.ansll' extension.\n", fileName);
            continue;
        }

        // Make filePath = dirName + '/' + fileName
        // Also handle if srcDir has a separator ('/') at end
        bool hasSep = dirName[dirNameLen - 1] == PATH_SEP;
#ifdef _WIN32
        // For Windows we'll handle if it has '/' or '\\' since either are valid
        if(!hasSep) hasSep = dirName[dirNameLen - 1] == '/';
#endif
        char *filePath = malloc(dirNameLen + fileNameLen + (hasSep ? 0 : 1) + 1);
        if(!filePath)
        {
            fprintf(stderr, "Failed to allocate memory for source file path.\n");
            ret = 1;
            goto ReadDirFailed;
        }
        memcpy(filePath, dirName, dirNameLen);
        if(!hasSep) filePath[dirNameLen] = PATH_SEP;
        // Also copy the NUL
        memcpy(filePath + dirNameLen + (hasSep ? 0 : 1), fileName, fileNameLen + 1);

        // Open source file for reading
        FILE *srcF = fopen(filePath, "r");
        if(!srcF)
        {
            fprintf(stderr, "Couldn't open source file '%s'.\n", filePath);
            ret = 1;
            goto OpenSourceFileFailed;
        }

        // Size of file in bytes
        size_t fSize = fGetSize(srcF);

        // Lex file and get tokens
        struct Token *tokens;
        char *strings;
        if(!lexFile(srcF, fSize, fileName, &tokens, &strings))
        {
            // lexFile guarantees that tokens and strings must not be
            // freed if it returns false
            fprintf(stderr, "Compilation Failed - Lexing failed.\n");
            ret = 1;
            goto LexingFailed;
        }

        // Compile tokens into object file
        // NOTE: Currently compiling to hard-coded object file name
        if(!compileToObject(tokens, strings, "./testout.o", args.verbose))
        {
            fprintf(stderr, "Compilation Failed - Compiling failed.\n");
            ret = 1;
            goto CompilingFailed;
        }

        if(args.verbose) printf("Compiled file '%s'.\n", "testcode.ansll");

        // Using goto like this allows for clean up when each
        // fail incrementally increases stuff that must be freed
    CompilingFailed:
        free(tokens);
        free(strings);
    LexingFailed:
        fclose(srcF);
    OpenSourceFileFailed:
        free(filePath);
    ReadDirFailed:

        // If we have no failures, we won't abort early
        if(ret)
        {
            fCloseDir(srcDir);
            return ret;
        }
    }

    fCloseDir(srcDir);

    printf("Compilation succeeded.\n");

    return 0;
}

