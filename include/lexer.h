#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>
#include "token.h"

bool lexFile(FILE *src, size_t srcSize, const char *srcName, struct Token **tokens, char **strings);

#endif

