/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBossStatus[];
void func_ov000_022de288();

void func_ov000_022de304(u8 *p0) {
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x14;
    *(u32 *)((u8 *)p0 + 0x220) = (u32)func_ov000_022de288;
    *(u32 *)((u8 *)p0 + 0x224) = 0;
    *(u32 *)((u8 *)p0 + 0xbc4) = 0x10;
    *(u32 *)((u8 *)p0 + 0xbc8) = -0x600;
    *(u32 *)((u8 *)p0 + 0xbcc) = -0x600;
}
