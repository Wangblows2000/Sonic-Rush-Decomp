#include <nitro/types.h>

u8 *func_ov000_0235cd90(void);

#pragma thumb on
BOOL func_ov000_0235cda0(void) {
    if (func_ov000_0235cd90()[2] != 0)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
