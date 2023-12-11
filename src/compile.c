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

