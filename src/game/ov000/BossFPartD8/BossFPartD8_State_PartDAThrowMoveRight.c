/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void BossFPartD8_State_PartDAThrowMoveToPos();

void BossFPartD8_State_PartDAThrowMoveRight(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x45e);
    if (t0 != 0x4000) {
        *(u16 *)((u8 *)p0 + 0x45e) = (t0 + 0x200);
        s32 t1 = *(u16 *)((u8 *)p0 + 0x45e);
        if ((u32)t1 > (u32)0x4000) {
            *(u16 *)((u8 *)p0 + 0x45e) = 0x4000;
        }
    }
    *(u16 *)((u8 *)p0 + 0x2e) = (*(s16 *)((u8 *)p0 + 0x2e) + 0x80);
    s32 t2 = *(s16 *)((u8 *)p0 + 0x2e);
    if (t2 > 0x1000) {
        *(u16 *)((u8 *)p0 + 0x2e) = 0x1000;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x10);
    if (t3 > 0x3b800) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_PartDAThrowMoveToPos;
    }
}
