/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0235020c();
s32 Sound_PlaySeqArcEx();

void func_ov000_02350440(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0235020c;
    s32 t0 = *(s16 *)((u8 *)p0 + 0x34);
    s32 v1 = t0;
    if (t0 < 0) {
        v1 = -v1;
    }
    *(u16 *)((u8 *)p0 + 0x30) = -v1;
    s32 t2 = *(s16 *)((u8 *)p0 + 0x30);
    s32 v3 = t2;
    if (t2 < -0xf00) {
        v3 = -0xf00;
    } else {
        s32 v4 = v3;
        if (v3 > 0xf00) {
            v4 = 0xf00;
        }
        v3 = v4;
    }
    *(u16 *)((u8 *)p0 + 0x30) = v3;
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~0xc0);
    *(u32 *)((u8 *)p0 + 0x330) = (*(u32 *)((u8 *)p0 + 0x330) & ~8);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x3f0) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u16 *)((u8 *)p0 + 0x3ee) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x40;
    *(u16 *)((u8 *)p0 + 0x3ca) = 0;
    Sound_PlaySeqArcEx(((u8 *)p0 + 0x384), 0, 0x40, ((u8 *)p0 + 0x300));
}
