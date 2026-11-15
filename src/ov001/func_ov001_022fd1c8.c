#include <nitro/types.h>

void func_ov001_022fd1c8(u8 *p) {
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) <= 0) {
        *(u32 *)(p + 0x14) |= 8;
    }
}
