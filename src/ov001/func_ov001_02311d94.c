#include <nitro/types.h>

void func_ov001_023122d8(void *p, s32 v);

void func_ov001_02311d94(u8 *p) {
    *(s32 *)(*(u8 **)(p + 0xac) + 0x114) += 0x40;
    if (*(s32 *)(*(u8 **)(p + 0xac) + 0x114) < 0x3000) {
        return;
    }
    (*(u8 **)(p + 0xac))[0x10c] &= ~4;
    if ((*(u8 **)(p + 0xac))[0x10c] & 2) {
        func_ov001_023122d8(p, 2);
    }
}
