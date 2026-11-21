/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void IronBallPart_State_FollowParent(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        s32 t1 = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)p0 + 0x54) = t1;
        return;
    }
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x14);
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + *(s16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x34));
    *(u8 *)((u8 *)p0 + 0x1) = ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 8);
}
