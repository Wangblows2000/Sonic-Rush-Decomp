#include <nitro/types.h>

u8 *func_ov001_022ffce8(void);

void func_ov001_022fa558(u8 *p) {
    *(s32 *)(p + 0x18) = *(s32 *)(func_ov001_022ffce8() + 0x31c);
}
