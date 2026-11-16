#include <nitro/types.h>

u32 func_0200e610(u32 cmd, u16 *out);

u32 func_0200e300(BOOL *a, BOOL *b)
{
    u16 status;
    u32 result = func_0200e610(0, &status);
    if (result != 0) {
        return result;
    }
    if (a != NULL) {
        *a = (status & 8) ? TRUE : FALSE;
    }
    if (b != NULL) {
        *b = (status & 4) ? TRUE : FALSE;
    }
    return result;
}
