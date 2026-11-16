/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void func_ov000_02301738();

void func_ov000_0230187c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x238);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x238) & 0x10000);
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x238) = (t0 & ~0x10000);
    } else {
        StageObj_SetAnimation(p0, 0, t1);
    }
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02301738;
}
