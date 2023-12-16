#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "appArgs.h"
#include "lexer.h"
#include "fileIo.h"
#include "compile.h"

int main(int argc, char **argv)
{
    int ret = 0;

    struct AppArgs args;
    if(!doArgp(&args, argc, argv)) return 2;

    // TODO: Loop through each file
    for(int i = 0; i < 1; i++)
    {
        // NOTE: For right now for testing hard-code a source file
        FILE *srcF = fopen("./testcode.ansll", "r");
        if(!srcF)
        {
            fprintf(stderr, "Couldn't open source file.");
            return 1;
        }

        size_t fSize = fGetSize(srcF);

        struct Token *tokens;
        char *strings;
        if(!lexFile(srcF, fSize, "testcode.ansll", &tokens, &strings))
        {
            // lexFile guarantees that tokens and strings must not be
            // freed if it returns false
            fprintf(stderr, "Compilation Failed - Lexing failed.\n");
            ret = 1;
            goto LexingFailed;
        }

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
        
        // If we have no failures, we won't abort early
        if(ret) return ret;
    }

    printf("Compilation succeeded.");

    return 0;
}

