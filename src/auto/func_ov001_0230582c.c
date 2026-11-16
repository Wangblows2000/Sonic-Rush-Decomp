/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_GetWork();

void func_ov001_0230582c(void) {
    s32 t0 = ExtraBoss_GetWork();
    *(u32 *)((u8 *)t0 + 0x31c) = 0x80000;
    *(u32 *)((u8 *)t0 + 0x320) = 0xc0000;
    *(u32 *)((u8 *)t0 + 0x318) = 0;
    *(u32 *)((u8 *)t0 + 0x324) = 0;
    *(u32 *)((u8 *)t0 + 0x328) = 0x20000;
    *(u32 *)((u8 *)t0 + 0x32c) = -0x20000;
    *(u32 *)((u8 *)t0 + 0x330) = 0;
    *(u32 *)((u8 *)t0 + 0x334) = 0x2000;
}
