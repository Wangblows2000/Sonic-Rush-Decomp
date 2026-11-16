/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjShift_Set();
extern u8 data_022c4ef8[];

s32 MapObj13_State_Unk_ov000_0230c1c8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 != 0) {
        *(u32 *)((u8 *)t0 + 0xc) = (*(u32 *)((u8 *)t0 + 0xc) + 0x1);
    }
    s32 t1 = ObjShift_Set(*(u32 *)((u8 *)p0 + 0x8), 0, 4, 0, 0);
    *(u16 *)((u8 *)p0 + 0x36) = (*(u32 *)((u8 *)p0 + 0x8) - t1);
    *(u32 *)((u8 *)p0 + 0x8) = t1;
    s32 t2 = *(u32 *)((u8 *)p0 + 0x18);
    if (t2 <= 0x8000) {
        return t2;
    }
    s32 t3 = *(s16 *)data_022c4ef8;
    if (t3 == 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
    s32 t4 = (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) & 1);
    if (t4 == 0) {
        s32 t5 = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)p0 + 0x54) = t5;
        return t5;
    }
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u16 *)((u8 *)p0 + 0x32) = 0;
    return 0;
}
