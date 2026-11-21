/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Needle_Action_Idle();

void Needle_State_Slide(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + 0x1);
    s32 t0 = *(s16 *)((u8 *)p0 + 0x1d6);
    *(u16 *)((u8 *)p0 + 0x1d6) = (*(s16 *)((u8 *)p0 + 0x1d6) + *(u32 *)((u8 *)p0 + 0xc));
    if ((u32)*(u32 *)((u8 *)p0 + 0x8) < (u32)8) {
        return;
    }
    s32 t1 = *(u16 *)((u8 *)p0 + 0x1f8);
    s32 v2 = (s32)((u8 *)p0 + 0x100);
    s32 v3 = t1;
    if (t1 == 2) {
        *(u16 *)((u8 *)v2 + 0xf8) = 0;
        s32 t4 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
        *(u16 *)((u8 *)p0 + 0x5c) = t4;
        v2 = t4;
        v3 = 0;
    } else {
        s32 v5 = v3;
        if (v3 == 1) {
            *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~2);
            *(u16 *)((u8 *)v2 + 0xf8) = 0;
            v5 = 0;
        }
        v3 = v5;
    }
    Needle_Action_Idle(p0, v2, v3);
}
