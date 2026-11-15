#include <nitro/types.h>

void func_02048fdc(s32 a, s32 b, s32 c, s32 d, void *e);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov001_022f4068(void);

void func_ov001_022f40b0(u8 *p) {
    *(u32 *)(p + 0x14) ^= 0x30000;
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) <= 0) {
        *(u16 *)(p + 0x1c8) = 0;
        func_02048fdc(0x20, 0, 0x10, 3, p + 0x1c8);
        *(void (**)(void))(p + 0x74) = func_ov001_022f4068;
        return;
    }
    if (*(s32 *)(p + 0x1c8) == 0xc1) {
        func_020366f4(0, 0, 0x2c);
    }
}
