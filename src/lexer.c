#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"

static void fAdvance(struct FilePos *fpos, int ch)
{
    fpos->col = fpos->col + fpos->len;
    fpos->len = 1;

    if (ch == '\n')
    {
        fpos->line++;
        fpos->col = 0;
    }
}

static void fLen(struct FilePos *fpos, uint32_t len)
{
    fpos->len = len;
}

/**
 * Gets a token from the Shove source file.
 * @param src The in source file.
 * @param ch The last character read. Just pass in LF if this is the first read. This will be set to the last character read by this function.
 * @param id The place to put a string. For identifiers, will contain the identifier string. Otherwise may be used as a workspace.
            Any data inputted may be overwritten.
 * @param idMax Buffer size of id.
 * @param numberLiteral Spot for putting numeric values. Will be filled in if is a number literal token.
 * @param fpos A pointer to a FilePos struct to fill in. This will be read and overwritten.
               First read should pass in `{ .filename = <name>, .line = 0, .col = 0, .len = 1 }`.
 * @param lastTok The last token read. Just pass in TOKEN_EOF if not read yet.
 * @return The token kind.
**/
static enum TokenKind getToken(
        FILE *src,
        int *ch,
        char *id,
        size_t idMax,
        union Num64 *numberLiteral,
        struct FilePos *fpos,
        enum TokenKind lastTok
)
{
    int lastChar = *ch;
    fAdvance(fpos, lastChar);

    while(isspace(lastChar))
    {
        lastChar = getc(src);
        fAdvance(fpos, lastChar);
    }


    if(isalpha(lastChar) || lastChar == '$')
    {
        bool isLocalVar = lastChar == '$';
        if(isLocalVar) lastChar = getc(src);

        size_t idOffset = 0;
        id[idOffset++] = lastChar;
        while (isalnum((lastChar = getc(src))))
        {
            id[idOffset++] = lastChar;
            if(idOffset + 1 >= idMax) break;
        }
        fLen(fpos, idOffset);
        id[idOffset] = '\0';
        *ch = lastChar;

        if(isLocalVar) return TOKEN_LOCAL_VAR;
        else if(!strncmp(id, "return", idMax)) return TOKEN_RETURN;
        else if(!strncmp(id, "this", idMax)) return TOKEN_THIS;
        else if(!strncmp(id, "import", idMax)) return TOKEN_IMPORT;
        else if(!strncmp(id, "extern", idMax)) return TOKEN_EXTERN;
        else if(!strncmp(id, "module", idMax)) return TOKEN_MODULE;
        else return TOKEN_IDENTIFIER;
    }
    else if(isdigit(lastChar))
    {
        size_t idOffset = 0;
        char *end;
        bool isFloatingPoint = false;
        bool isUnsigned = false;
        int radix;

        if(lastChar != '0') goto Base10;
        else lastChar = getc(src);

        switch (lastChar)
        {
            case 'X':
            case 'x':
                while(isdigit((lastChar = getc(src))) ||
                     (lastChar >= 'a' && lastChar <= 'f') ||
                     (lastChar >= 'A' && lastChar <= 'F'))
                {
                    id[idOffset++] = lastChar;
                }

                radix = 16;
                fLen(fpos, idOffset + 2);
                break;
                // Use 0o instead of just 0 for octals
            case 'O':
            case 'o':
                while((lastChar = getc(src)) >= '0' && lastChar <= '8')
                {
                    id[idOffset++] = lastChar;
                }
                radix = 8;
                fLen(fpos, idOffset + 2);
                break;
            case 'B':
            case 'b':
                while ((lastChar = getc(src)) == '0' || lastChar == '1')
                {
                    fAdvance(fpos, lastChar + 2);
                    id[idOffset++] = lastChar;
                }
                radix = 2;
                fLen(fpos, idOffset + 2);
                break;
            default:
            Base10:
                do
                {
                    if(lastChar == '.')
                    {
                        if(!isFloatingPoint) isFloatingPoint = true;
                        else
                        {
                            return 0;
                        }
                    }
                    id[idOffset++] = lastChar;
                }
                while(isdigit((lastChar = getc(src))) || lastChar == '.');

                radix = 10;
                fLen(fpos, idOffset);

                if(lastChar == 'd' || lastChar == 'D')
                {
                    isFloatingPoint = true;
                    fLen(fpos, fpos->len + 1);
                    lastChar = getc(src);
                }
                if(isFloatingPoint)
                {
                    id[idOffset] = '\0';

                    int oldErrno = errno;
                    errno = 0;

                    numberLiteral->f64 = strtod(id, &end);

                    int newErrno = errno;
                    errno = oldErrno;

                    return !newErrno || newErrno == ERANGE ? TOKEN_FLOAT : 0;
                }
                break;
        }

        id[idOffset] = '\0';
        if(lastChar == 'u' || lastChar == 'U')
        {
            isUnsigned = true;
            fLen(fpos, fpos->len + 1);
            lastChar = getc(src);
        }

        int oldErrno = errno;
        errno = 0;

        numberLiteral->i64 = strtoll(id, &end, radix);
        int newErrno = errno;
        errno = oldErrno;
        *ch = lastChar;

        // if not specified unsigned and no errors (or were too low)
        if(!isUnsigned && (!errno || (errno == ERANGE && numberLiteral->i64 == INT64_MIN)))
        {
            return newErrno ? 0 : TOKEN_SIGNED_INT;
        }
        // otherwise if the number is too high for an int or is specified uint
        else if(isUnsigned || errno == ERANGE)
        {
            errno = 0;
            numberLiteral->u64 = strtoull(id, &end, radix);
            newErrno = errno;
            errno = oldErrno;

            return newErrno ? 0 : TOKEN_UNSIGNED_INT;
        }
        // otherwise if an error happened
        else
        {
            errno = oldErrno;
            return 0;
        }
    }
    else if(lastChar == EOF)
    {
        *ch = lastChar;
        return TOKEN_EOF;
    }
    else
    {
        *ch = getc(src);
        switch(lastChar)
        {
            case '-':
                return TOKEN_MINUS;
            case '+':
                return TOKEN_PLUS;
            case '*':
                return TOKEN_STAR;
            case '/':
                return TOKEN_SLASH;
            case '%':
                return TOKEN_MODULO;
            case '^':
                if(*ch == '^')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_POW;
                }
                else return TOKEN_BITWISE_XOR;
            case '&':
                if (*ch == '&')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_LOGICAL_AND;
                }
                else return TOKEN_AMPERSAND;
            case '|':
                if (*ch == '|')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_LOGICAL_OR;
                }
                else return TOKEN_BITWISE_OR;
            case '!':
                return TOKEN_BANG;
            case '~':
                return TOKEN_BITWISE_NOT;
            case '=':
                return TOKEN_EQUAL;
            case ';':
                return TOKEN_TERMINATE;
            case '<':
                if(*ch == '<')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);

                    return TOKEN_SHIFT_LEFT;
                }
                else if(*ch == '=')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_LTE;
                }
                else return TOKEN_LT;
            case '>':
                if(*ch == '>')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_SHIFT_RIGHT;
                }
                else if(*ch == '=')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_GTE;
                }
                else return TOKEN_GT;
            case ':':
                if(*ch == ':')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_SCOPE;
                }
                else if(*ch == '=')
                {
                    *ch = getc(src);
                    fLen(fpos, 2);
                    return TOKEN_EQUAL;
                }
                else return 0;
            case '{':
                return TOKEN_BLOCK_OPEN;
            case '}':
                return TOKEN_BLOCK_CLOSE;
            case '(':
                return TOKEN_PAREN_OPEN;
            case ')':
                return TOKEN_PAREN_CLOSE;

        }

        return 0;
    }

}

static size_t pushSpace(void **space, void **at, size_t size, const void *item, size_t itemSize)
{
    size_t needed = (char*)*at - (char*)*space + itemSize;
    if(needed > size)
    {
        if(size * 2 < needed) size *= 2;
        else size = needed;

        void *newSpace = realloc(*space, size);
        if(!newSpace) return 0;

        *space = newSpace;
    }

    memcpy(*at, item, itemSize);
    *at += itemSize;
    return size;
}

bool lexFile(FILE *src, size_t srcSize, const char *srcName, struct Token **tokens, char **strings)
{
    int32_t tok = -1;
    int ch = '\n';
    char identifier[1024];
    union Num64 numberLiteral = { 0 };
    struct FilePos fpos =
    {
        // This does mean that the struct relies on the caller's string
        .filename = srcName,

        .line = 0,
        .col = 0
    };
    bool success = false;

    size_t stringSpaceSize = srcSize;
    char *stringSpace = malloc(stringSpaceSize);
    if(!stringSpace)
    {
        fprintf(stderr, "Not enough memory for the lexer identifiers.");
        return false;
    }
    char *atStringSpace = stringSpace;

    size_t tokenSpaceSize = srcSize * sizeof(struct Token);
    struct Token *tokenSpace = malloc(tokenSpaceSize);
    if(!tokenSpace)
    {
        fprintf(stderr, "Not enough memory for the lexer tokens.");
        free(stringSpace);
        return false;
    }
    struct Token *atTokenSpace = tokenSpace;

    struct Token tokStruct;
    size_t offset;

    while(true)
    {
        tok = getToken(
            src,
            &ch,
            identifier,
            sizeof identifier,
            &numberLiteral,
            &fpos,
            tok
        );

        tokStruct.symbol = tok;
        // This will take a copy
        tokStruct.fpos = fpos;

        switch(tok)
        {
            case 0:
                goto Cleanup;
            case TOKEN_EOF:
                goto ExitWhile;
            case TOKEN_SIGNED_INT:
            case TOKEN_UNSIGNED_INT:
            case TOKEN_FLOAT:
                tokStruct.value = numberLiteral;
                break;
            case TOKEN_LOCAL_VAR:
            case TOKEN_IDENTIFIER:
                // Use offset instead of pointers in case we realloc
                offset = atStringSpace - stringSpace;

                size_t strLen = strlen(identifier) + 1;
                stringSpaceSize = pushSpace(
                    (void**)&stringSpace,
                    (void**)&atStringSpace,
                    stringSpaceSize,
                    identifier,
                    strLen
                );
                if(!stringSpaceSize)
                {
                    fprintf(stderr, "Not enough memory to resize lexer identifiers.");
                    goto Cleanup;
                }

                tokStruct.value.u64 = offset;
                break;
            default:
                tokStruct.value.u64 = 0x0;
                break;
        }

        tokenSpaceSize = pushSpace(
            (void**)&tokenSpace,
            (void**)&atTokenSpace,
            tokenSpaceSize,
            &tokStruct,
            sizeof(struct Token)
        );
        if(!tokenSpaceSize)
        {
            fprintf(stderr, "Not enough memory to resize lexer tokens.");
            goto Cleanup;
        }

    }

ExitWhile:
    tokStruct.symbol = TOKEN_EOF;
    tokStruct.value.u64 = 0;

    tokenSpaceSize = pushSpace(
        (void**)&tokenSpace,
        (void**)&atTokenSpace,
        tokenSpaceSize,
        &tokStruct,
        sizeof(struct Token)
    );

    // We won't cleanup here since we're returning the buffers
    *tokens = tokenSpace;
    *strings = stringSpace;
    return true;

Cleanup:
    free(stringSpace);
    free(tokenSpace);
    return success;
}
