/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjSpd_UpSet();
s32 func_ov000_023141b8();

void func_ov000_02314200(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0xc) & 0x10) != 0) {
        s32 t0 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x30), -16, 0x40);
        *(u16 *)((u8 *)p0 + 0x30) = t0;
    } else {
        s32 t1 = ObjSpd_UpSet(*(s16 *)((u8 *)p0 + 0x30), 0x10, 0x40);
        *(u16 *)((u8 *)p0 + 0x30) = t1;
    }
    s32 t2 = *(u32 *)((u8 *)p0 + 0xc);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (t2 - 0x1);
        s32 t3 = *(u32 *)((u8 *)p0 + 0xc);
        if (t3 <= 0xc) {
            *(u16 *)((u8 *)p0 + 0x30) = 0;
        }
        s32 t4 = *(u32 *)((u8 *)p0 + 0xc);
        if (t4 == 8) {
            *(u16 *)((u8 *)p0 + 0x62) = (*(u16 *)((u8 *)p0 + 0x62) | 0x80);
        }
        return;
    }
    func_ov000_023141b8(p0);
}
