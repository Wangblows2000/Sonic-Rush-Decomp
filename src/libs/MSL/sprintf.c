#include <nitro/types.h>
#include <main_sdk2/stdarg.h>

int vsnprintf(char *s, u32 n, const char *fmt, va_list args);

int sprintf(char *s, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    return vsnprintf(s, -1, fmt, args);
}
