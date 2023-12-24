#include <stdio.h>
#include <string.h>
#include "mangleName.h"

int main(void)
{
    char name[256];
    size_t size = mangleFunction(name, sizeof name, "std::namespace", "function", NULL, 0);
    if(size > sizeof name || strcmp(name, "_AF$std?namespace$function$P$"))
    {
        fprintf(stderr, "Function 'std::namespace::function()' mangled to '%s' (%lu chars).\n", name, size);
    }

    size = mangleFunction(name, sizeof name, "unicode", "greek\xCF\x91THETA", NULL, 0);
    if(size > sizeof name || strcmp(name, "_AF$unicode$greek@33D1THETA$P$"))
    {
        fprintf(stderr, "Function 'unicode::greek\xCF\x91THETA()' mangled to '%s' (%lu chars).\n", name, size);
    }

    return 0;
}

