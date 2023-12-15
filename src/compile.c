#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <llvm-c/Core.h>
#include "token.h"

static void createLlvm(
    LLVMContextRef *context,
    LLVMBuilderRef *builder,
    LLVMModuleRef *module,
    LLVMPassManagerRef *pass,
    const char *moduleName
)
{
    *context = LLVMContextCreate();
    *builder = LLVMCreateBuilderInContext(*context);

    *module =
        LLVMModuleCreateWithNameInContext(moduleName, *context);
    
    *pass = LLVMCreateFunctionPassManagerForModule(*module);
}

static void destroyLlvm(LLVMContextRef context,
                        LLVMBuilderRef builder,
                        LLVMModuleRef module,
                        LLVMPassManagerRef pass
)
{
    LLVMDisposePassManager(pass);
    LLVMDisposeBuilder(builder);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);
}

static bool expect(enum TokenKind expected, struct Token actual)
{
    if(expected == actual.symbol)
    {
        return true;
    }
    else
    {
        fprintf(
                stderr, "Expected token '%s' but got '%s'.",
                getTokenKindString(expected), getTokenKindString(actual.symbol)
        );
        return false;
    }
}

static bool compileTokens(
    const struct Token *tokens,
    const char *strings,
    bool verbose
)
{
    if(!expect(TOKEN_MODULE, tokens[0])) return false;
    if(!expect(TOKEN_IDENTIFIER, tokens[1])) return false;
    if(!expect(TOKEN_TERMINATE, tokens[2])) return false;

    const struct Token *atToken = tokens + 3;
    while(true)
    {
        switch(atToken->symbol)
        {
            case TOKEN_FN:
                break;
            case TOKEN_UNITDEF:
                break;
            case TOKEN_UNITALIAS:
                break;
            default:
                fprintf(
                        stderr, "Invalid token '%s' at top-level.",
                        getTokenKindString(atToken->symbol)
                );
                return false;
        }
    }
}

bool compileToObject(
    const struct Token *tokens,
    const char *strings,
    const char *objFilename,
    bool verbose
)
{
    LLVMContextRef context;
    LLVMBuilderRef builder;
    LLVMModuleRef module;
    LLVMPassManagerRef pass;

    if(tokens[0].symbol != TOKEN_MODULE)
    {
        fprintf(stderr, "First thing (besides comments) in a file must be the module declaration.");
        return false;
    }
    const char *moduleName = strings + tokens[1].value.u64;

    createLlvm(&context, &builder, &module, &pass, moduleName);

    // TODO: Compile Tokens

    if(verbose)
    {
        printf("Emitting object file '%s'.\n", objFilename);
    }

    // TODO: Emit to Object File

    destroyLlvm(context, builder, module, pass);

    return true;
}

