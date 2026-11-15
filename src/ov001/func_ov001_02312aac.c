#include <nitro/types.h>

u16 func_ov001_02312ac8(s32 v);

void func_ov001_02312aac(u8 *p) {
    *(u16 *)(p + 0x16) = func_ov001_02312ac8(*(s32 *)(p + 0x20));
}
