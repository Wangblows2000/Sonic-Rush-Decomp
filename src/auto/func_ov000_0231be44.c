/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_SetAnimation();
void func_ov000_0231bdf8();
s32 func_0201c070();
s32 Sound_PlaySeqArcEx();
extern u8 data_ov000_0236ac60[];
s32 Sound_SetPositional();

void func_ov000_0231be44(u8 *p0) {
    StageObj_SetAnimation(p0, 1);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x16a) & ~0x80);
    *(u16 *)((u8 *)p0 + 0x16a) = t0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x28;
    *(u32 *)((u8 *)p0 + 0x20c) = (*(u32 *)((u8 *)p0 + 0x20c) | 1);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0231bdf8;
    func_0201c070(*(u32 *)((u8 *)p0 + 0x1dc), 0, (u32)func_ov000_0231bdf8, t0);
    Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x57);
    Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
}
