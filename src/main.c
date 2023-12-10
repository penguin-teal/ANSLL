#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "fileIo.h"

int main(int argc, char **argv)
{
    int ret = 0;
    bool verbose = true;

    for(int i = 0; i < 1; i++)
    {
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
            fprintf(stderr, "Compilation Failed - Lexing failed.");
            ret = 1;
            goto LexingFailed;
        }

        if(false)
        {
            fprintf(stderr, "Compilation Failed - Compiling failed.");
            ret = 1;
            goto CompilingFailed;
        }

        if(verbose) printf("Compiled file '%s'.", "testcode.ansll");

    CompilingFailed:
        free(tokens);
        free(strings);
    LexingFailed:
        
        if(ret) return ret;
    }

    printf("Compilation succeeded.");

    return 0;
}

