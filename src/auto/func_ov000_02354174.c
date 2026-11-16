/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023470ec();
s32 func_0201c070();
s32 func_ov000_0234ab34();
s32 func_ov000_0234a064();
s32 func_ov000_02348860();
s32 func_ov000_023470a0();
s32 func_ov000_0234b424();
s32 func_ov000_02347870();
extern u8 data_022c4560[];
extern u8 data_ov000_0236aad8[];
s32 func_ov000_0232929c();

void func_ov000_02354174(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)p0 + 0x37c);
    s32 t1 = *(s16 *)((u8 *)p0 + 0x34);
    s32 v2 = t1;
    if (t1 < 0) {
        v2 = -v2;
    }
    if (v2 < t0) {
        s32 t3 = func_ov000_023470ec(p0, v2);
        if (t3 != 0) {
            func_0201c070(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            s32 t4 = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            *(u32 *)((u8 *)p0 + 0x58) = t4;
            func_ov000_0234ab34(p0, t4);
            return;
        }
    }
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        if (*(u32 *)((u8 *)p0 + 0x340) != 0) {
            func_0201c070(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340));
            return;
        }
    }
    s32 t5 = *(s16 *)((u8 *)p0 + 0x32a);
    if (t5 != 0) {
        *(u16 *)((u8 *)p0 + 0x32a) = (t5 - 0x1);
    } else {
        if ((*(u32 *)((u8 *)p0 + 0x58) & 0xd) != 0) {
            func_0201c070(((u8 *)p0 + 0x384), 0);
            *(u8 *)((u8 *)p0 + 0x1) = 0;
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
            *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
            s32 t6 = *(s16 *)((u8 *)p0 + 0x34);
            *(u16 *)((u8 *)p0 + 0x2e) = t6;
            func_ov000_0234a064(p0, 0, t6);
            if (*(u32 *)((u8 *)p0 + 0x3f8) == 2) {
                func_ov000_02348860(p0);
                return;
            }
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
            return;
        }
    }
    func_ov000_023470a0(p0, *(s16 *)((u8 *)p0 + 0x34));
    func_ov000_0234b424(p0);
    func_ov000_02347870(p0);
    if ((*(u32 *)((u8 *)data_022c4560 + 0x20) & 7) != 0) {
        return;
    }
    func_ov000_0232929c(p0, 0, 0, *(u16 *)((u8 *)data_ov000_0236aad8 + 0x62));
}
