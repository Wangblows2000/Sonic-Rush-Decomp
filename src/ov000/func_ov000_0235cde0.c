#include <nitro/types.h>

u8 *func_ov000_0235cd90(void);

#pragma thumb on
u8 func_ov000_0235cde0(void) {
    return *func_ov000_0235cd90();
}
#pragma thumb reset
