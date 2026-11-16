/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void func_ov000_0231aed8();

void func_ov000_0231af10(u8 *p0) {
    StageObj_SetAnimation(p0, 0);
    *(u32 *)((u8 *)p0 + 0xc) = ((u32)((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) * 0x64 + *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x7)) << 0x10) >> 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0231aed8;
}
