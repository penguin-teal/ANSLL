#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

struct FilePos
{
    const char *filename;
    uint64_t line;
    uint64_t col;
    uint64_t len;
};

union Num64
{
    uint64_t u64;
    int64_t i64;
    double f64;
};

/**
 * Represents a token and associated information.
 */
struct Token
{
    /**
     * The int ID of this token. Should be a TOKEN_* macro.
     */
    int32_t symbol;

    struct FilePos fpos;

    union Num64 value;
};

enum TokenKind
{
    TOKEN_EOF             =   -1,  //
    TOKEN_MODULE          =   -2,  // module i
    TOKEN_IMPORT          =   -3,  // import i
    TOKEN_EXTERN          =   -4,  // extern i
    TOKEN_FLOAT           =   -5,  // <float literal>
    TOKEN_IDENTIFIER      =   -6,  // <identifier>
    TOKEN_RETURN          =   -7,  // return a

    TOKEN_BITWISE_NOT     =   -8, // ~a
    TOKEN_BANG            =   -9, // !a
    TOKEN_PLUS            =   -10, // a + b OR +a
    TOKEN_MINUS           =   -11, // a - b OR -a
    TOKEN_STAR            =   -12, // a * b
    TOKEN_SLASH           =   -13, // a / b
    TOKEN_MODULO          =   -14, // a % b
    TOKEN_AMPERSAND       =   -15, // a & b
    TOKEN_BITWISE_OR      =   -16, // a | b
    TOKEN_BITWISE_XOR     =   -17, // a ^ b
    TOKEN_LOGICAL_AND     =   -18, // a && b
    TOKEN_LOGICAL_OR      =   -19, // a || b
    TOKEN_GT              =   -20, // a > b
    TOKEN_LT              =   -21, // a < b
    TOKEN_GTE             =   -22, // a >= b
    TOKEN_LTE             =   -23, // a <= b
    TOKEN_POW             =   -24, // a ^^ b
    TOKEN_SCOPE           =   -25, // ::
    TOKEN_EQUAL           =   -26, // i := b
    TOKEN_TERMINATE       =   -27, // a;
    TOKEN_SHIFT_LEFT      =   -28, // a << b
    TOKEN_SHIFT_RIGHT     =   -29, // a >> b
    TOKEN_BLOCK_OPEN      =   -30, // {
    TOKEN_BLOCK_CLOSE     =   -31, // }
    TOKEN_THIS            =   -32, // this
    TOKEN_VOID            =   -33, // void
    TOKEN_PAREN_OPEN      =   -34, // (
    TOKEN_PAREN_CLOSE     =   -35, // )
    TOKEN_CASE            =   -36, // case a
    TOKEN_COLON           =   -37, // a:
    TOKEN_FALL            =   -38, // fall <case>
    TOKEN_BREAK           =   -39, // break
    TOKEN_WHILE           =   -40, // while
    TOKEN_GOTO            =   -41, // goto
    TOKEN_CONTINUE        =   -42, // continue

    TOKEN_SIGNED_INT      =   -43, // <int literal>
    TOKEN_UNSIGNED_INT    =   -44, // <uint literal>
    TOKEN_LOCAL_VAR       =   -45, // $<identifier>
    TOKEN_POUND           =   -46, // # comment
    TOKEN_IS_EQUAL        =   -47, // a == b
    TOKEN_FN              =   -48, // fn i
    TOKEN_UNITDEF         =   -49, // unitdef i OR unitdef i i
    TOKEN_UNITALIAS       =   -50, // unitalias i i
    TOKEN_USING           =   -51, // using i
};

#endif

