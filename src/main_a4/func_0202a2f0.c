#include <nitro/types.h>
#include <main_sdk2/stdarg.h>

int func_0202a324(char *s, u32 n, const char *fmt, va_list args);

int func_0202a2f0(char *s, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    return func_0202a324(s, -1, fmt, args);
}
