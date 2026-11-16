/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4ef0[];

void func_ov000_0235264c(u8 *p0, s32 p1) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != (s32)p1) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x3e8) = 0;
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x1000);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x800);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~8);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x3ec) = 0;
    *(u16 *)((u8 *)p0 + 0x3ee) = 0;
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~0x40000);
    *(u32 *)data_022c4ef0 = (*(u32 *)data_022c4ef0 & ~0x100);
}
