/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234c4a4();
void func_ov000_0234a288();
s32 ObjRect_SetAtkDefFlags();
s32 Sound_PlaySeqArcEx();

void func_ov000_023547f0(u8 *p0) {
    func_ov000_0234c4a4(p0, 0x30);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x8000);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0234a288;
    *(u16 *)((u8 *)p0 + 0x2e) = ((s32)*(u32 *)((u8 *)p0 + 0x3f4) >> 0x11);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x3f4);
    *(u16 *)((u8 *)p0 + 0x30) = t0;
    ObjRect_SetAtkDefFlags(((u8 *)p0 + 0x2cc), 0x13, 0, t0);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0xf);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 1);
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x32c) | 2);
    *(u32 *)((u8 *)p0 + 0x32c) = t1;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u16 *)((u8 *)p0 + 0x32a) = 0;
    *(u8 *)((u8 *)p0 + 0x3cf) = 0;
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x4e, t1);
}
