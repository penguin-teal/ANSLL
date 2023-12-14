#include <token.h>

/**
 * Gets a human readable string.
 * This is basically the "*" part of the corresponding TOKEN_* enum value.
 * If it represents a keyword it will be all lower-case, or if it
 * is rather a "placeholder," such as `Identifier` it will start with
 * an upper-case letter (with the exception of `EOF`).
 *
 * @remarks The string must NOT be freed or managed by the caller.
 * @return A null-terminated string identifying the given token symbol,
 *         or `"Unknown"` if invalid.
 */
const char *getTokenKindString(enum TokenKind k)
{
    switch(k)
    {
        case TOKEN_EOF:
            return "EOF";
        case TOKEN_MODULE:
            return "module";
        case TOKEN_IMPORT:
            return "import";
        case TOKEN_EXTERN:
            return "extern";
        case TOKEN_SIGNED_INT:
            return "Int";
        case TOKEN_UNSIGNED_INT:
            return "Unsigned Int";
        case TOKEN_FLOAT:
            return "Float";
        case TOKEN_IDENTIFIER:
            return "Identifier";
        case TOKEN_RETURN:
            return "return";
        case TOKEN_BITWISE_NOT:
            return "~";
        case TOKEN_BANG:
            return "!";
        case TOKEN_PLUS:
            return "+";
        case TOKEN_MINUS:
            return "-";
        case TOKEN_STAR:
            return "*";
        case TOKEN_SLASH:
            return "/";
        case TOKEN_MODULO:
            return "%";
        case TOKEN_AMPERSAND:
            return "&";
        case TOKEN_BITWISE_OR:  
            return "|";
        case TOKEN_BITWISE_XOR:
            return "^";
        case TOKEN_LOGICAL_AND:
            return "&&";
        case TOKEN_LOGICAL_OR:
            return "||";
        case TOKEN_GT:
            return ">";
        case TOKEN_LT:
            return "<";
        case TOKEN_GTE:
            return ">=";
        case TOKEN_LTE:
            return "<=";
        case TOKEN_POW:
            return "^^";
        case TOKEN_SCOPE:
            return "::";
        case TOKEN_EQUAL:
            return "=";
        case TOKEN_TERMINATE:
            return ";";
        case TOKEN_SHIFT_LEFT:
            return "<<";
        case TOKEN_SHIFT_RIGHT:
            return ">>";
        case TOKEN_BLOCK_OPEN:
            return "{";
        case TOKEN_BLOCK_CLOSE:
            return "}";
        case TOKEN_THIS:
            return "this";
        case TOKEN_VOID:
            return "void";
        case TOKEN_PAREN_OPEN:
            return "(";
        case TOKEN_PAREN_CLOSE:
            return ")";
        case TOKEN_USING:
            return "using";
        case TOKEN_CASE:
            return "case";
        case TOKEN_COLON:
            return "colon";
        case TOKEN_FALL:
            return "fall";
        case TOKEN_BREAK:
            return "break";
        case TOKEN_WHILE:
            return "while";
        case TOKEN_GOTO:
            return "goto";
        case TOKEN_IS_EQUAL:
            return "==";
        case TOKEN_UNITDEF:
            return "unitdef";
        case TOKEN_UNITALIAS:
            return "unitalias";
        case TOKEN_FN:
            return "fn";
        case TOKEN_CONTINUE:
            return "continue";
        case TOKEN_LOCAL_VAR:
            return "$Identifier";
        default:
            return "Unknown";
    }
}
