/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234c4a4();
void func_ov000_0234b810();
s32 ObjRect_SetAtkDefFlags();
s32 func_ov000_023340f0();
s32 Sound_PlaySeqArcEx();

void func_ov000_0234ba48(u8 *p0) {
    func_ov000_0234c4a4(p0, 0x32);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0234b810;
    *(u16 *)((u8 *)p0 + 0x32a) = *(s16 *)((u8 *)p0 + 0x35c);
    ObjRect_SetAtkDefFlags(((u8 *)p0 + 0x2cc), 0x13, 0, ((u8 *)p0 + 0x300));
    *(u32 *)((u8 *)p0 + 0x2e8) = 0;
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x2d6) & ~8);
    *(u16 *)((u8 *)p0 + 0x2d6) = t0;
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 1);
    if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) == 0) {
        func_ov000_023340f0(p0, t0);
    }
    if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
        Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0);
        return;
    }
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x11);
}
