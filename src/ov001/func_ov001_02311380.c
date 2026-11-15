#include <nitro/types.h>

BOOL func_ov001_02312138(void *p);
void func_ov001_023122d8(void *p, s32 v);

void func_ov001_02311380(u8 *p) {
    if (*(s32 *)(p + 0x15c) < 2) {
        u16 old = *(u16 *)(p + 0x12);
        *(u16 *)(p + 0x12) = old + 0x780;
        if (old < 0x8000 && *(u16 *)(p + 0x12) >= 0x8000) {
            *(s32 *)(p + 0x15c) += 1;
            if (*(s32 *)(p + 0x15c) >= 2) {
                *(u16 *)(p + 0x12) = 0x8000;
            }
        }
    }
    if (func_ov001_02312138(p)) {
        func_ov001_023122d8(p, 5);
    }
}
