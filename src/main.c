#include <stdio.h>
#include "lexer.h"
#include "fileIo.h"

int main(int argc, char **argv)
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
        return 1;
    }

    printf("Compilation succeeded.");

    return 0;
}

