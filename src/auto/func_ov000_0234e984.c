/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234307c();
s32 func_ov000_02336050();
s32 Sound_PlaySeqArcEx();
s32 func_ov000_0235aea8();
s32 func_ov000_02349038();
s32 ScreenShake_Start();

void func_ov000_0234e984(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u16 t1 = *(u16 *)((u8 *)t0 + 0x62);
    if ((t1 & 0xf) != 0) {
        return;
    }
    u32 t2 = *(u32 *)((u8 *)t0 + 0x32c);
    if ((t2 & 0x10000000) == 0) {
        func_ov000_0234307c(t0, -0x4b0);
        s16 t3 = *(s16 *)((u8 *)(t0 + 0x300) + 0x50);
        func_ov000_02336050(((u32)(t3 << 0xc) >> 0x10), 1);
    }
    u32 t4 = *(u32 *)((u8 *)p1 + 0x20);
    if ((t4 & 0x20) != 0) {
        s16 t5 = *(s16 *)((u8 *)(t0 + 0x300) + 0xae);
        if (t5 != 0) {
            Sound_PlaySeqArcEx((t0 + 0x384), 0, 5);
            func_ov000_0235aea8(t0);
        }
        func_ov000_02349038(t0);
        return;
    }
    u32 t6 = *(u32 *)((u8 *)t0 + 0x32c);
    if ((t6 & 0x10000000) == 0) {
        s16 t7 = *(s16 *)((u8 *)(t0 + 0x300) + 0xae);
        if (t7 == 0) {
            func_ov000_02349038(t0, (t6 & 0x10000000));
            return;
        }
    }
    if ((t6 & 0x10000000) == 0) {
        Sound_PlaySeqArcEx((t0 + 0x384), 0, 5);
        func_ov000_0235aea8(t0);
    }
    u32 t8 = *(u32 *)((u8 *)t0 + 0x32c);
    *(u32 *)((u8 *)t0 + 0x32c) = (t8 & ~0x30000000);
    ScreenShake_Start(3, (t8 & ~0x30000000));
    s16 t9 = *(s16 *)((u8 *)(t0 + 0x300) + 0x76);
    *(u16 *)((u8 *)(t0 + 0x300) + 0xb2) = t9;
    u32 t10 = *(u32 *)((u8 *)t0 + 0x2c4);
    *(u32 *)((u8 *)t0 + 0x2c4) = (t10 | 5);
    *(u16 *)((u8 *)t0 + 0x2e) = 0;
    *(u16 *)((u8 *)t0 + 0x30) = 0;
    *(u16 *)((u8 *)t0 + 0x34) = 0;
    u8 t11 = *(u8 *)((u8 *)t0 + 0x324);
    if (t11 != 1) {
        Sound_PlaySeqArcEx((t0 + 0x384), 0, 0x21);
    } else {
        Sound_PlaySeqArcEx((t0 + 0x384), 0, 0x2a);
    }
    *(u16 *)((u8 *)t0 + 0x62) = 0x80;
}
