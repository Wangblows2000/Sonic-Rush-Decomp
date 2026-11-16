/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjShift_Set();
s32 func_ov000_0234a064();

void Player_State_SandSlider(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x3e8) != 0) {
        *(u32 *)((u8 *)p0 + 0x1c) = *(u32 *)((u8 *)p0 + 0x10);
        s32 t0 = ObjShift_Set(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x3e8) + 0x10), 4, 0, 0x100);
        *(u32 *)((u8 *)p0 + 0x10) = t0;
        s32 t1 = ObjShift_Set(*(u32 *)((u8 *)p0 + 0x18), 0, 1, 0, 0x100);
        *(u32 *)((u8 *)p0 + 0x18) = t1;
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t2 = *(s16 *)((u8 *)p0 + 0x30);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x3e8) = 0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    s32 t3 = (*(u32 *)((u8 *)p0 + 0x32c) & ~0x100000);
    *(u32 *)((u8 *)p0 + 0x32c) = t3;
    *(u8 *)((u8 *)p0 + 0x3) = 0;
    *(u8 *)((u8 *)p0 + 0x2) = 0;
    *(u8 *)((u8 *)p0 + 0x1) = 0;
    func_ov000_0234a064(p0, 0, t3);
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
    *(u16 *)((u8 *)p0 + 0x30) = t2;
}
