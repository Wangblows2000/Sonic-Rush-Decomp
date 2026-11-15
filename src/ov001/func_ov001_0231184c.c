#include <nitro/types.h>

BOOL func_ov001_02312138(void *p);
void func_ov001_023122d8(void *p, s32 v);
void func_ov001_02312aac(void *p);
s32 func_ov001_02312a5c(u16 a);
void func_ov001_02312258(void *p);

void func_ov001_0231184c(u8 *p) {
    s32 d;
    if (func_ov001_02312138(p)) {
        *(s32 *)(p + 0x124) = 0;
        func_ov001_023122d8(p, 5);
        return;
    }
    func_ov001_02312aac(p);
    d = func_ov001_02312a5c(*(s32 *)(p + 0x134)) - *(s32 *)(p + 0x20);
    if (d > 0x1400) {
        d = 0x1400;
    } else if (d < -0x1400) {
        d = -0x1400;
    }
    *(s32 *)(p + 0x20) += d;
    func_ov001_02312258(p);
}
