/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjShift_SetFromRef();

void EffVsTensionMax_State_Main(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        return;
    }
    if (*(s16 *)((u8 *)p0 + 0x44) == 8) {
        if (*(s16 *)((u8 *)p0 + 0x46) == 0x180) {
            *(u16 *)((u8 *)p0 + 0x44) = 0x100;
            *(u16 *)((u8 *)p0 + 0x46) = 0x100;
            *(u32 *)((u8 *)p0 + 0xc) = 0;
            *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + 0x1);
        }
    }
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = ((*(u32 *)((u8 *)t0 + 0x14) - 0x1000) - (*(u32 *)((u8 *)p0 + 0xc) * 0x140));
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    s32 t1 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x44), 8, 0x100, 2, 0, 0);
    *(u16 *)((u8 *)p0 + 0x44) = t1;
    s32 t2 = ObjShift_SetFromRef(*(s16 *)((u8 *)p0 + 0x46), 0x180, 0x100, 2, 0, 0);
    *(u16 *)((u8 *)p0 + 0x46) = t2;
    s32 t3 = *(u32 *)((u8 *)p0 + 0x8);
    if ((u32)t3 > (u32)1) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
