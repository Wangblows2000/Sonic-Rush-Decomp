/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023475fc();
s32 func_02068db4();
s32 func_ov000_0234a064();

void func_ov000_02352ee0(u8 *p0) {
    func_ov000_023475fc();
    s32 t0 = func_02068db4(*(u8 *)((u8 *)p0 + 0x3), 2);
    *(u8 *)((u8 *)p0 + 0x3) = t0;
    s32 t1 = func_02068db4(*(s16 *)((u8 *)p0 + 0x2e), 0x60);
    *(u16 *)((u8 *)p0 + 0x2e) = t1;
    if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
        s32 t2 = func_02068db4(*(s16 *)((u8 *)p0 + 0x30), 0x60);
        *(u16 *)((u8 *)p0 + 0x30) = t2;
    }
    *(u32 *)((u8 *)p0 + 0xc) = 1;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    s32 t3 = *(s16 *)((u8 *)p0 + 0x2e);
    s32 v4 = t3;
    if (t3 < 0) {
        v4 = -v4;
    }
    if (v4 >= 0x400) {
        return;
    }
    s32 t5 = *(s16 *)((u8 *)p0 + 0x30);
    s32 v6 = t5;
    if (t5 < 0) {
        v6 = -v6;
    }
    if (v6 >= 0x400) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x18) = 0;
    *(u8 *)((u8 *)p0 + 0x3) = 0;
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x100000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    s32 t7 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t7 != 0) {
        *(u16 *)((u8 *)p0 + 0x2e) = *(s16 *)((u8 *)p0 + 0x34);
    }
    func_ov000_0234a064(p0, 0);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
}
