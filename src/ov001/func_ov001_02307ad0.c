#include <nitro/types.h>

extern u8 *data_ov001_0231f5bc;

void func_ov001_02307ad0(void) {
    s32 v = *(s32 *)(data_ov001_0231f5bc + 0xbd4);
    if (v < 0x3e) {
        u32 plane = (*(vu32 *)0x04001000 & 0x1f00) >> 8;
        *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | ((plane & ~2) << 8);
    } else if (v == 0x3e) {
        u32 plane = (*(vu32 *)0x04001000 & 0x1f00) >> 8;
        *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | ((plane | 2) << 8);
    }
}
